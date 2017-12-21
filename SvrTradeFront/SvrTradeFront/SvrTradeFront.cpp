// SvrTradeFront.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
//#include "stdafx.h"
#include <math.h>
#include <windows.h>
#include <map>
#include "SvrTradeFront.h"
#include "tmpstackbuf.h"
#include "FTTraderApi.h"
#include "LinkEntity.h"


//-----------------------------------------------------------------------------------
//	import Tools4dll库
//-----------------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

//-----------------------------------------------------------------------------------
//	下面import本模块需要引用的其它模块
//-----------------------------------------------------------------------------------
#pragma comment(lib, "SvrTcp.lib")
#pragma comment(lib, "thosttraderapi.lib")


Ceasymutex					g_mutex;
map<int,CFTTraderApi*>		g_mapSocket2API;
set<string>					g_setTradeFrontAdds;
set<string>					g_setQuotFrontAdds;

CDataPackageForwarding		g_dataPackageForwarding;

CDataPackageForwarding&		_GetForwarding() {
	return g_dataPackageForwarding;
}

LONG	g_nCurrSeqID = 1;

int _GetNextSeq() {
	return InterlockedExchangeAdd(&g_nCurrSeqID, 1);
}

DWORD dwTlsIndex = TlsAlloc();

void _SetThreadServData(CDataPackageForwarding::Stru_ThreadData& tdata)
{
	LPVOID lpvData = TlsGetValue(dwTlsIndex); 
	if(lpvData==NULL) {
		lpvData = (LPVOID)LocalAlloc(LPTR, sizeof(CDataPackageForwarding::Stru_ThreadData)); 
		TlsSetValue(dwTlsIndex, lpvData);
	}
	if(lpvData!=NULL) {
		*(CDataPackageForwarding::Stru_ThreadData*)lpvData = tdata;
	}
}

bool _GetThreadServData(CDataPackageForwarding::Stru_ThreadData& tdata)
{
	LPVOID lpvData = TlsGetValue(dwTlsIndex); 
	if(lpvData!=NULL) {
		tdata = *(CDataPackageForwarding::Stru_ThreadData*)lpvData;
		return true;
	}
	else
		return false;
}

//线程参数
HANDLE				g_hThread=NULL;
HANDLE				g_hSSLAcceptThread=NULL;
HANDLE				g_hSSLCloseThread=NULL;
DWORD				g_idThread=0;
UINT				g_nSSLCloseMsgID = ::RegisterWindowMessage("g_nSSLCloseMsgID");
DWORD ThreadWorker(void *arg);
DWORD WINAPI SSLAcceptProc(LPVOID pParameter);
DWORD WINAPI SSLCloseProc(LPVOID pParameter);
//处理一个接收到的数据包
//void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);
void ProcessOneUniPkg_InThread(int hSocket,const void* pPackage);


//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRTRADEFRONT_API void InitFunc(void)
{   
	CInterface_SvrTcp::getObj();
	_GetForwarding().Init();
	CLinkEntity::InitSSL();
	if(!_LoadFrontAddrs(TRADE_TYPE, g_setTradeFrontAdds) /*|| !_LoadFrontAddrs(QUOT_TYPE, g_setQuotFrontAdds)*/)
		printf("Load Front Address error,Please check !!!\n");

	DWORD dwThreadID = 0;
	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	g_hSSLAcceptThread=CreateThread(NULL, 0, SSLAcceptProc, 0, 0, &dwThreadID);
	g_hSSLCloseThread=CreateThread(NULL, 0, SSLCloseProc, 0, 0, &dwThreadID);
}

//模块结束，释放资源，关闭工作线程
SVRTRADEFRONT_API void ReleaseFunc(void)
{ 
	CloseHandle(g_hSSLAcceptThread);
	g_hSSLAcceptThread = NULL;

	if(g_hThread) {
		//退订数据包
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//发送WM_QUIT通知线程结束
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//等待线程退出
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)g_hThread,8000);
		if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThread,0);
		CloseHandle(g_hThread);
		g_hThread=NULL;
		g_idThread=0;
	}

	_ClearAPI();
	CLinkEntity::ReleaseSSL();
	_GetForwarding().Release();
}

void DealPakage(unsigned int EventParamID,int& PkgLen,int& hSocket)
{
	AllocTmpStackBuf(TmpPkg,PkgLen);
	if(TmpPkg.m_pbuf&&
		CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
		Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
	{
		Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
		void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);

        //调用数据包处理函数处理数据包
        //ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);	
		ProcessOneUniPkg_InThread(hSocket,(char*)TmpPkg.m_pbuf);	
    }
}

//工作线程
DWORD ThreadWorker(void *arg)
{
	//下面订阅本线程感兴趣的数据包
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpAccept,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpDisconn,g_idThread);
	///用户登录请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqUserLogin,g_idThread);
	///登出请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqUserLogout,g_idThread);
	///用户口令更新请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqUserPasswordUpdate,g_idThread);
	///资金账户口令更新请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqTradingAccountPasswordUpdate,g_idThread);
	///报单录入请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqOrderInsert,g_idThread);
	///预埋单录入请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqParkedOrderInsert,g_idThread);
	///预埋撤单录入请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqParkedOrderAction,g_idThread);
	///报单操作请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqOrderAction,g_idThread);
	///查询最大报单数量请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQueryMaxOrderVolume,g_idThread);
	///投资者结算结果确认
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqSettlementInfoConfirm,g_idThread);
	///请求删除预埋单
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqRemoveParkedOrder,g_idThread);
	///请求删除预埋撤单
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqRemoveParkedOrderAction,g_idThread);
	///询价录入请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqForQuoteInsert,g_idThread);
	///执行宣告录入请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqExecOrderInsert,g_idThread);
	///执行宣告操作请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqExecOrderAction,g_idThread);
	///请求查询报单
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryOrder,g_idThread);
	///请求查询成交
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTrade,g_idThread);
	///请求查询投资者持仓
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestorPosition,g_idThread);
	///请求查询资金账户
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTradingAccount,g_idThread);
	///请求查询投资者
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestor,g_idThread);
	///请求查询合约保证金率
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInstrumentMarginRate,g_idThread);
	///请求查询合约手续费率
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInstrumentCommissionRate,g_idThread);
	///请求查询交易所
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryExchange,g_idThread);
	///请求查询产品
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryProduct,g_idThread);
	///请求查询合约
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInstrument,g_idThread);
	///请求查询投资者结算结果
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQrySettlementInfo,g_idThread);
	///请求查询投资者持仓明细
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestorPositionDetail,g_idThread);
	///请求查询客户通知
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryNotice,g_idThread);
	///请求查询结算信息确认
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQrySettlementInfoConfirm,g_idThread);
	///请求查询投资者持仓明细
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestorPositionCombineDetail,g_idThread);
	///请求查询保证金监管系统经纪公司资金账户密钥
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryCFMMCTradingAccountKey,g_idThread);
	///请求查询期权交易成本
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryOptionInstrTradeCost,g_idThread);
	///请求查询期权合约手续费
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryOptionInstrCommRate,g_idThread);
	///请求查询执行宣告
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryExecOrder,g_idThread);
	///请求查询转帐流水
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTransferSerial,g_idThread);
	///请求查询银期签约关系
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryAccountregister,g_idThread);
	///请求查询签约银行
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryContractBank,g_idThread);
	///请求查询预埋单
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryParkedOrder,g_idThread);
	///请求查询预埋撤单
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryParkedOrderAction,g_idThread);
	///请求查询交易通知
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTradingNotice,g_idThread);
	///请求查询经纪公司交易参数
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryBrokerTradingParams,g_idThread);
	///期货发起银行资金转期货请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqFromBankToFutureByFuture,g_idThread);
	///期货发起期货资金转银行请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqFromFutureToBankByFuture,g_idThread);
	///期货发起查询银行余额请求
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQueryBankAccountMoneyByFuture,g_idThread);

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0)) {
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2) {
			//数据包的传输ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//数据包长度
			int PkgLen;
			//Socket句柄
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().
				GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL)) {
					DealPakage(EventParamID,PkgLen,hSocket);
					//释放EventParam
					CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}
		}
	}

	return 0;
}

DWORD WINAPI SSLAcceptProc(LPVOID pParameter)
{
	struct sockaddr_in sa_cli;
	int client_len = 0;
	int sd = 0;

	client_len = sizeof(sa_cli);
	while((sd = accept(CLinkEntity::sm_SocketListen, (struct sockaddr*) &sa_cli, &client_len)) != -1) {
		CLinkEntity* pEntity = new CLinkEntity();
		pEntity->ResetCloseThread(g_hSSLCloseThread, g_nSSLCloseMsgID);
		pEntity->Init(sd, TRUE);

	}
	int error = WSAGetLastError();
	return 0;
}

DWORD WINAPI SSLCloseProc(LPVOID pParameter)
{
	SOCKET hCloseSocket = 0;

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0)) {
		if(Msg.message==g_nSSLCloseMsgID && Msg.wParam != 0) {
			hCloseSocket = Msg.wParam;
			//CLinkEntity* pEntity = GetLinkEntity(hCloseSocket);
			//if(pEntity != NULL) {
			//	DeleteLinkEntity(hCloseSocket);
			//	pEntity->Release();
			//	delete pEntity;
			//}
		}
	}
	return 0;
}

//处理一个SvrTcp推送过来的数据包
//void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
void ProcessOneUniPkg_InThread(int hSocket,const void* pPackage)
{
    Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)pPackage);
    void* pPkgData=(char*)pPackage+sizeof(Stru_UniPkgHead);

	if(PkgHead.cmdid == CMDID_SvrTcpAccept) {
		LOG_INFO("------>>>CMDID_SvrTcpAccept Strat, Socket[%d]", hSocket);
		_AddAPI(hSocket);
		LOG_INFO("------>>>CMDID_SvrTcpAccept End, Socket[%d]", hSocket);
		return;
	}
	else if(PkgHead.cmdid == CMDID_SvrTcpDisconn) {
		LOG_INFO("------>>>CMDID_SvrTcpDisconn Strat, Socket[%d]", hSocket);
		_DeleteAPI(hSocket);			
		LOG_INFO("------>>>CMDID_SvrTcpDisconn End, Socket[%d]", hSocket);
		return;
	}

	int nServSeq = _GetForwarding().GetNextSeqID();
	_GetForwarding().SendSrcPackage((void*)pPackage, PkgHead.len+sizeof(Stru_UniPkgHead)+4, nServSeq, hSocket);
	
	CDataPackageForwarding::Stru_ThreadData tdata;
	memset(&tdata, 0, sizeof(tdata));
	tdata.base = PkgHead;
	tdata.nServSeq = nServSeq;
	tdata.nSocket = hSocket;
	_SetThreadServData(tdata);

	//-----------------------------------------------------------------------------------
	//	下面根据数据包的命令字，处理数据包
	//-----------------------------------------------------------------------------------
	CFTTraderApi *pApi = _GetAPI(hSocket);
	if(!pApi)
	{
		printf("--------error: FTTraderApi handle is NULL,Please check!----------\n");
		return;
	}

	int nRspCmdID = 0;
	CThostFtdcRspInfoField RspInfo = {0};
	RspInfo.ErrorID = -1;

	switch(PkgHead.cmdid)
	{
		///用户登录请求
	case FTTCMD_ReqUserLogin:
		{
			nRspCmdID = FTTCMD_ReqUserLogin_Rsp;
			CThostFtdcReqUserLoginField* pData = (CThostFtdcReqUserLoginField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqUserLogin(pData, PkgHead.userdata1);
			break;
		}		
		///登出请求
	case FTTCMD_ReqUserLogout:
		{
			nRspCmdID = FTTCMD_ReqUserLogout_Rsp;
			CThostFtdcUserLogoutField* pData = (CThostFtdcUserLogoutField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqUserLogout(pData, PkgHead.userdata1);
			break;
		}
		///用户口令更新请求
	case FTTCMD_ReqUserPasswordUpdate:
		{
			nRspCmdID = FTTCMD_ReqUserPasswordUpdate_Rsp;
			PlatformStru_UserPasswordUpdate* pData = (PlatformStru_UserPasswordUpdate*)pPkgData;				
			RspInfo.ErrorID = pApi->ReqUserPasswordUpdate(pData, PkgHead.userdata1);				
			break;
		}
		///资金账户口令更新请求
	case FTTCMD_ReqTradingAccountPasswordUpdate:
		{
			nRspCmdID = FTTCMD_ReqTradingAccountPasswordUpdate_Rsp;
			PlatformStru_TradingAccountPasswordUpdate* pData = (PlatformStru_TradingAccountPasswordUpdate*)pPkgData;
			RspInfo.ErrorID = pApi->ReqTradingAccountPasswordUpdate(pData, PkgHead.userdata1);
			break;
		}
		///报单录入请求
	case FTTCMD_ReqOrderInsert:
		{
			nRspCmdID = FTTCMD_ReqOrderInsert_Rsp;
			PlatformStru_InputOrder* pData = (PlatformStru_InputOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqOrderInsert(pData, PkgHead.userdata1);
			break;
		}
		///预埋单录入请求
	case FTTCMD_ReqParkedOrderInsert:
		{
			nRspCmdID = FTTCMD_ReqParkedOrderInsert_Rsp;
			PlatformStru_ParkedOrder* pData = (PlatformStru_ParkedOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqParkedOrderInsert(pData, PkgHead.userdata1);
			break;
		}
		///预埋撤单录入请求
	case FTTCMD_ReqParkedOrderAction:
		{
			nRspCmdID = FTTCMD_ReqParkedOrderAction_Rsp;
			PlatformStru_ParkedOrderAction* pData = (PlatformStru_ParkedOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqParkedOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///报单操作请求
	case FTTCMD_ReqOrderAction:
		{
			nRspCmdID = FTTCMD_ReqOrderAction_Rsp;
			PlatformStru_InputOrderAction* pData = (PlatformStru_InputOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///查询最大报单数量请求
	case FTTCMD_ReqQueryMaxOrderVolume:
		{
			nRspCmdID = FTTCMD_ReqQueryMaxOrderVolume_Rsp;
			PlatformStru_QueryMaxOrderVolume* pData = (PlatformStru_QueryMaxOrderVolume*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQueryMaxOrderVolume(pData, PkgHead.userdata1);
			break;
		}
		///投资者结算结果确认
	case FTTCMD_ReqSettlementInfoConfirm:
		{
			nRspCmdID = FTTCMD_ReqSettlementInfoConfirm_Rsp;
			CThostFtdcSettlementInfoConfirmField* pData = (CThostFtdcSettlementInfoConfirmField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqSettlementInfoConfirm(pData, PkgHead.userdata1);
			break;
		}
		///请求删除预埋单
	case FTTCMD_ReqRemoveParkedOrder:
		{
			nRspCmdID = FTTCMD_ReqRemoveParkedOrder_Rsp;
			PlatformStru_RemoveParkedOrder* pData = (PlatformStru_RemoveParkedOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqRemoveParkedOrder(pData, PkgHead.userdata1);
			break;
		}
		///请求删除预埋撤单
	case FTTCMD_ReqRemoveParkedOrderAction:
		{
			nRspCmdID = FTTCMD_ReqRemoveParkedOrderAction_Rsp;
			PlatformStru_RemoveParkedOrderAction* pData = (PlatformStru_RemoveParkedOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqRemoveParkedOrderAction(pData, PkgHead.userdata1);
			break;
		}
	///询价录入请求
	case FTTCMD_ReqForQuoteInsert:
		{
			nRspCmdID = FTTCMD_ReqForQuoteInsert_Rsp;
			PlatformStru_InputForQuoteField* pData = (PlatformStru_InputForQuoteField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqForQuoteInsert(pData, PkgHead.userdata1);
			break;
		}
		///执行宣告录入请求
	case FTTCMD_ReqExecOrderInsert:
		{
			nRspCmdID = FTTCMD_ReqExecOrderInsert_Rsp;
			PlatformStru_InputExecOrderField* pData = (PlatformStru_InputExecOrderField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqExecOrderInsert(pData, PkgHead.userdata1);
			break;
		}
		///执行宣告操作请求
	case FTTCMD_ReqExecOrderAction:
		{
			nRspCmdID = FTTCMD_ReqExecOrderAction_Rsp;
			PlatformStru_InputExecOrderActionField* pData = (PlatformStru_InputExecOrderActionField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqExecOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///请求查询报单
	case FTTCMD_ReqQryOrder:
		{
			nRspCmdID = FTTCMD_ReqQryOrder_Rsp;
			PlatformStru_QryOrder* pData = (PlatformStru_QryOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryOrder(pData, PkgHead.userdata1);
			break;
		}
		///请求查询成交
	case FTTCMD_ReqQryTrade:
		{
			nRspCmdID = FTTCMD_ReqQryTrade_Rsp;
			PlatformStru_QryTrade* pData = (PlatformStru_QryTrade*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTrade(pData, PkgHead.userdata1);
			break;
		}
		///请求查询投资者持仓
	case FTTCMD_ReqQryInvestorPosition:
		{
			nRspCmdID = FTTCMD_ReqQryInvestorPosition_Rsp;
			PlatformStru_QryInvestorPosition* pData = (PlatformStru_QryInvestorPosition*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestorPosition(pData, PkgHead.userdata1);
			break;
		}
		///请求查询资金账户
	case FTTCMD_ReqQryTradingAccount:
		{
			nRspCmdID = FTTCMD_ReqQryTradingAccount_Rsp;
			PlatformStru_QryTradingAccount* pData = (PlatformStru_QryTradingAccount*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTradingAccount(pData, PkgHead.userdata1);
			break;
		}
		///请求查询投资者
	case FTTCMD_ReqQryInvestor:
		{
			nRspCmdID = FTTCMD_ReqQryInvestor_Rsp;
			PlatformStru_QryInvestor* pData = (PlatformStru_QryInvestor*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestor(pData, PkgHead.userdata1);
			break;
		}
		///请求查询合约保证金率
	case FTTCMD_ReqQryInstrumentMarginRate:
		{
			nRspCmdID = FTTCMD_ReqQryInstrumentMarginRate_Rsp;
			CThostFtdcQryInstrumentMarginRateField* pData = (CThostFtdcQryInstrumentMarginRateField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInstrumentMarginRate(pData, PkgHead.userdata1);
			break;
		}
		///请求查询合约手续费率
	case FTTCMD_ReqQryInstrumentCommissionRate:
		{
			nRspCmdID = FTTCMD_ReqQryInstrumentCommissionRate_Rsp;
			CThostFtdcQryInstrumentCommissionRateField* pData = (CThostFtdcQryInstrumentCommissionRateField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInstrumentCommissionRate(pData, PkgHead.userdata1);
			break;
		}
		///请求查询交易所
	case FTTCMD_ReqQryExchange:
		{
			nRspCmdID = FTTCMD_ReqQryExchange_Rsp;
			CThostFtdcQryExchangeField* pData = (CThostFtdcQryExchangeField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryExchange(pData, PkgHead.userdata1);
			break;
		}
		///请求查询产品
	case FTTCMD_ReqQryProduct:
		{
			nRspCmdID = FTTCMD_ReqQryProduct_Rsp;
			PlatformStru_QryProduct* pData = (PlatformStru_QryProduct*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryProduct(pData, PkgHead.userdata1);
			break;
		}
		///请求查询合约
	case FTTCMD_ReqQryInstrument:
		{
			nRspCmdID = FTTCMD_ReqQryInstrument_Rsp;
			PlatformStru_QryInstrument* pData = (PlatformStru_QryInstrument*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInstrument(pData, PkgHead.userdata1);
			break;
		}
		///请求查询投资者结算结果
	case FTTCMD_ReqQrySettlementInfo:
		{
			nRspCmdID = FTTCMD_ReqQrySettlementInfo_Rsp;
			PlatformStru_QrySettlementInfo* pData = (PlatformStru_QrySettlementInfo*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQrySettlementInfo(pData, PkgHead.userdata1);
			break;
		}
		///请求查询投资者持仓明细
	case FTTCMD_ReqQryInvestorPositionDetail:
		{
			nRspCmdID = FTTCMD_ReqQryInvestorPositionDetail_Rsp;
			PlatformStru_QryInvestorPositionDetail* pData = (PlatformStru_QryInvestorPositionDetail*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestorPositionDetail(pData, PkgHead.userdata1);
			break;
		}
		///请求查询客户通知
	case FTTCMD_ReqQryNotice:
		{
			nRspCmdID = FTTCMD_ReqQryNotice_Rsp;
			PlatformStru_QryNotice* pData = (PlatformStru_QryNotice*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryNotice(pData, PkgHead.userdata1);
			break;
		}
		///请求查询结算信息确认
	case FTTCMD_ReqQrySettlementInfoConfirm:
		{
			nRspCmdID = FTTCMD_ReqQrySettlementInfoConfirm_Rsp;
			PlatformStru_QrySettlementInfoConfirm* pData = (PlatformStru_QrySettlementInfoConfirm*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQrySettlementInfoConfirm(pData, PkgHead.userdata1);
			break;
		}
		///请求查询投资者持仓明细
	case FTTCMD_ReqQryInvestorPositionCombineDetail:
		{
			nRspCmdID = FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp;
			PlatformStru_QryInvestorPositionCombineDetail* pData = (PlatformStru_QryInvestorPositionCombineDetail*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestorPositionCombineDetail(pData, PkgHead.userdata1);
			break;
		}
		///请求查询保证金监管系统经纪公司资金账户密钥
	case FTTCMD_ReqQryCFMMCTradingAccountKey:
		{
			nRspCmdID = FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp;
			CThostFtdcQryCFMMCTradingAccountKeyField* pData = (CThostFtdcQryCFMMCTradingAccountKeyField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryCFMMCTradingAccountKey(pData, PkgHead.userdata1);
			break;
		}
		///请求查询期权交易成本
	case FTTCMD_ReqQryOptionInstrTradeCost:
		{
			nRspCmdID = FTTCMD_ReqQryOptionInstrTradeCost_Rsp;
			CThostFtdcQryOptionInstrTradeCostField* pData = (CThostFtdcQryOptionInstrTradeCostField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryOptionInstrTradeCost(pData, PkgHead.userdata1);
			break;
		}
		///请求查询期权合约手续费
	case FTTCMD_ReqQryOptionInstrCommRate:
		{
			nRspCmdID = FTTCMD_ReqQryOptionInstrCommRate_Rsp;
			CThostFtdcQryOptionInstrCommRateField* pData = (CThostFtdcQryOptionInstrCommRateField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryOptionInstrCommRate(pData, PkgHead.userdata1);
			break;
		}
		///请求查询执行宣告
	case FTTCMD_ReqQryExecOrder:
		{
			nRspCmdID = FTTCMD_ReqQryExecOrder_Rsp;
			PlatformStru_QryExecOrderField* pData = (PlatformStru_QryExecOrderField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryExecOrder(pData, PkgHead.userdata1);
			break;
		}
		///请求查询转帐流水
	case FTTCMD_ReqQryTransferSerial:
		{
			nRspCmdID = FTTCMD_ReqQryTransferSerial_Rsp;
			PlatformStru_QryTransferSerial* pData = (PlatformStru_QryTransferSerial*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTransferSerial(pData, PkgHead.userdata1);
			break;
		}
		///请求查询银期签约关系
	case FTTCMD_ReqQryAccountregister:
		{
			nRspCmdID = FTTCMD_ReqQryAccountregister_Rsp;
			PlatformStru_QryAccountRegister* pData = (PlatformStru_QryAccountRegister*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryAccountregister(pData, PkgHead.userdata1);
			break;
		}
		///请求查询签约银行
	case FTTCMD_ReqQryContractBank:
		{
			nRspCmdID = FTTCMD_ReqQryContractBank_Rsp;
			PlatformStru_QryContractBank* pData = (PlatformStru_QryContractBank*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryContractBank(pData, PkgHead.userdata1);
			break;
		}
		///请求查询预埋单
	case FTTCMD_ReqQryParkedOrder:
		{
			nRspCmdID = FTTCMD_ReqQryParkedOrder_Rsp;
			PlatformStru_QryParkedOrder* pData = (PlatformStru_QryParkedOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryParkedOrder(pData, PkgHead.userdata1);
			break;
		}
		///请求查询预埋撤单
	case FTTCMD_ReqQryParkedOrderAction:
		{
			nRspCmdID = FTTCMD_ReqQryParkedOrderAction_Rsp;
			PlatformStru_QryParkedOrderAction* pData = (PlatformStru_QryParkedOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryParkedOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///请求查询交易通知
	case FTTCMD_ReqQryTradingNotice:
		{
			nRspCmdID = FTTCMD_ReqQryTradingNotice_Rsp;
			CThostFtdcQryTradingNoticeField* pData = (CThostFtdcQryTradingNoticeField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTradingNotice(pData, PkgHead.userdata1);
			break;
		}
		///请求查询经纪公司交易参数
	case FTTCMD_ReqQryBrokerTradingParams:
		{
			nRspCmdID = FTTCMD_ReqQryBrokerTradingParams_Rsp;
			CThostFtdcQryBrokerTradingParamsField* pData = (CThostFtdcQryBrokerTradingParamsField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryBrokerTradingParams(pData, PkgHead.userdata1);
			break;
		}
		///期货发起银行资金转期货请求
	case FTTCMD_ReqFromBankToFutureByFuture:
		{
			nRspCmdID = FTTCMD_ReqFromBankToFutureByFuture_Rsp;
			PlatformStru_ReqTransfer* pData = (PlatformStru_ReqTransfer*)pPkgData;
			RspInfo.ErrorID = pApi->ReqFromBankToFutureByFuture(pData, PkgHead.userdata1);
			break;
		}
		///期货发起期货资金转银行请求
	case FTTCMD_ReqFromFutureToBankByFuture:
		{
			nRspCmdID = FTTCMD_ReqFromFutureToBankByFuture_Rsp;
			PlatformStru_ReqTransfer* pData = (PlatformStru_ReqTransfer*)pPkgData;
			RspInfo.ErrorID = pApi->ReqFromFutureToBankByFuture(pData, PkgHead.userdata1);
			break;
		}
		///期货发起查询银行余额请求
	case FTTCMD_ReqQueryBankAccountMoneyByFuture:
		{
			nRspCmdID = FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp;
			PlatformStru_ReqQueryAccount* pData = (PlatformStru_ReqQueryAccount*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQueryBankAccountMoneyByFuture(pData, PkgHead.userdata1);
			break;
		}
	default:
		nRspCmdID = FTTCMD_OnRspError_RON;
		sprintf_s(RspInfo.ErrorMsg, sizeof(RspInfo.ErrorMsg), "FTT: 未知的命令字[%d]", PkgHead.cmdid);
	};

	int nSeq = _GetNextSeq();
	CInterface_SvrTcp::getObj().SendPkgData(hSocket, nRspCmdID, &RspInfo, sizeof(CThostFtdcRspInfoField), nSeq);
	if(RspInfo.ErrorID!=0)
		_GetForwarding().SendPackage(&RspInfo, sizeof(RspInfo), nRspCmdID, nSeq, 
				PkgHead.userdata1, 5, nServSeq, hSocket);
}

