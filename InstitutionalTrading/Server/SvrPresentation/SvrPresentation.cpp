// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrPresentation.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"
#include "..\\SvrTradeData\Interface_DBOpr.h"
#include "..\\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
#include "..\\SvrLogin\\Interface_SvrLogin.h"
#include "RealSvrPresentation.h"
#include <vector>
#include "CommonMacro.h"
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
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrDBOpr.lib")
#pragma comment(lib, "SvrBrokerInfo.lib")
#pragma comment(lib, "SvrLogin.lib")
#pragma comment(lib, "SvrTradeData.lib")
#pragma comment(lib, "SvrFinanceProduct.lib")



#define WRITELOGID_SvrPresentation




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);


//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);
void ProcessAdminQuery(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRPRESENTATION_API void InitFunc(void)
{
	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	////下面订阅本线程感兴趣的数据包
	////CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_SubscribeStatistics_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_UnSubscribeStatistics_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_SubscribeUserFund_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_UnSubscribeUserFund_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryAvilabeInstrument_Req,g_idThread);
}

//模块结束，释放资源，关闭工作线程
SVRPRESENTATION_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
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
		ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);
		ProcessAdminQuery(hSocket,PkgHead,pPkgData);
	}
}
//工作线程
DWORD ThreadWorker(void *arg)
{
	//下面订阅本线程感兴趣的数据包
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_SubscribeStatistics_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_UnSubscribeStatistics_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_SubscribeUserFund_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_UnSubscribeUserFund_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryAvilabeInstrument_Req,g_idThread);

	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryHistoryFundInfo_Req, g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryHistoryPosition_Req, g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryHistoryPositionDetail_Req, g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryHistoryTrade_Req, g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryHistoryOrders_Req, g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_QryHistoryFundInOut_Req, g_idThread);

	//如果是主服务，每隔两s统计一次
	UINT_PTR lSaveTime = 0;
	if(!CInterface_SvrLogin::getObj().IsSlaver())
		lSaveTime = SetTimer(NULL,NULL,2000,(TIMERPROC)(NULL));
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message == WM_TIMER )
		{
			CRealSvrPresentation::getObj().OnTimerStatisc();	
		}
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
		{
			//数据包的传输ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//数据包长度
			int PkgLen;
			//Socket句柄
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL))
			{
				////申请临时空间并获取数据包
				//AllocTmpStackBuf(TmpPkg,PkgLen);
				//if(TmpPkg.m_pbuf&&
				//	CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
				//	Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
				//{
				//	Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
				//	void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);

				//	//调用数据包处理函数处理数据包
				//	ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);
				//}
				DealPakage(EventParamID,PkgLen,hSocket);

				//释放EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}

		}
	}
	KillTimer(NULL,lSaveTime);
	return 0;
}

//处理一个SvrTcp推送过来的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	下面根据数据包的命令字，处理数据包
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{
	case Cmd_RM_SubscribeStatistics_Req://订阅统计请求	
		{
			
			int pkgLen=PkgHead.len;
			int AccCnt=pkgLen/sizeof(AssetOrgProRelation);
			int AccRem=pkgLen%sizeof(AssetOrgProRelation);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<StatisticsEntry> vec;
				AssetOrgProRelation* pOrgID = (AssetOrgProRelation*)pPkgData;
				for (int index=0;index<AccCnt;++index)
				{
					AssetOrgProRelation& OrgID=pOrgID[index];	
					CRealSvrPresentation::getObj().SubscribeStatistics(OrgID,hSocket);
					CRealSvrPresentation::getObj().GetStatisticsEntryList(OrgID,vec);
				}
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_SubscribeStatistics_Rsp,
					vec.size() == 0 ? NULL : &vec[0],
					sizeof(StatisticsEntry)*vec.size(),
					PkgHead.seq,
					0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS,0);
			}
			else
			{
				SendRspError(hSocket,Cmd_RM_SubscribeStatistics_Rsp,
					PkgHead.seq,
					CF_ERROR_COMMON_INPUT_PARAM,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_COMMON_INPUT_PARAM,0);
			}
		}
		break;
	case Cmd_RM_UnSubscribeStatistics_Req://退订统计请求	
		{
			
			int pkgLen=PkgHead.len;
			int AccCnt=pkgLen/sizeof(AssetOrgProRelation);
			int AccRem=pkgLen%sizeof(AssetOrgProRelation);
			if (pkgLen>0||AccRem==0)
			{
				AssetOrgProRelation* pOrgID = (AssetOrgProRelation*)pPkgData;
				for (int index=0;index<AccCnt;++index)
				{
					AssetOrgProRelation& OrgID=pOrgID[index];			
					CRealSvrPresentation::getObj().UnSubscribeStatistics(OrgID,hSocket);			

				}
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_UnSubscribeStatistics_Rsp,
					NULL,
					0,
					PkgHead.seq,
					0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS,0);
			}
			else
			{
				SendRspError(hSocket,Cmd_RM_UnSubscribeStatistics_Rsp,
					PkgHead.seq,
					CF_ERROR_COMMON_INPUT_PARAM,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_COMMON_INPUT_PARAM,0);
			}
		}
		break;
	case Cmd_RM_SubscribeUserFund_Req:
		{
			
			int pkgLen=PkgHead.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<sFundInfoEx> vec;			
				int* pAccountID = (int*)pPkgData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					CRealSvrPresentation::getObj().SubscribeFund(AccID,hSocket);
					CRealSvrPresentation::getObj().GetFundList(AccID,vec);				
				}
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_SubscribeUserFund_Rsp,
					vec.size() == 0 ? NULL : &vec[0],
					sizeof(sFundInfoEx)*vec.size(),
					PkgHead.seq,
					0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS,0);
			}
			else
			{
				SendRspError(hSocket,Cmd_RM_SubscribeUserFund_Rsp,
					PkgHead.seq,
					CF_ERROR_COMMON_INPUT_PARAM,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_COMMON_INPUT_PARAM,0);
			}
		}
		break;
	case Cmd_RM_UnSubscribeUserFund_Req:		
		{
	
			int pkgLen=PkgHead.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)pPkgData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];			
					CRealSvrPresentation::getObj().UnSubscribeFund(AccID,hSocket);			

				}
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_UnSubscribeUserFund_Rsp,
					NULL,
					0,
					PkgHead.seq,
					0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS,0);
			}
			else
			{
				SendRspError(hSocket,Cmd_RM_UnSubscribeUserFund_Rsp,
					PkgHead.seq,
					CF_ERROR_COMMON_INPUT_PARAM,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_COMMON_INPUT_PARAM,0);
			}
		}
		break;
	case Cmd_RM_QryAvilabeInstrument_Req:
		{
			if(PkgHead.len != 0)
			{
				SendRspError(hSocket,Cmd_RM_QryAvilabeInstrument_Rsp,
				PkgHead.seq,
				CF_ERROR_COMMON_INPUT_PARAM,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_COMMON_INPUT_PARAM,0);
				break;
			}
			//获取合约表里面最新一天的合约
			char szBuffer[200] = {0};	
			strcpy_s(szBuffer, "select * from (select * from TRADINGDAY t order by t.tradingday DESC) where rownum <= 1 ");

			int nErrorCode = 0;
			std::string lsLastTradingDay ;
			std::vector<std::vector<_variant_t>> vNode;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
			{
				break;
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					char szDay[11] = {0};
					std::vector<_variant_t> vValue = vNode[j];
					int i = 0;
					strcpy(szDay, vValue[i++].operator _bstr_t());	
					lsLastTradingDay = szDay;
				}
			} 

			if(!lsLastTradingDay.empty())
			{
				std::vector<PlatformStru_InstrumentInfo> lVecInstrument;	
				CInterface_SvrTradeData::getObj().GetInterfaceDBOpr()->LoadTodayInstrument(lsLastTradingDay,lVecInstrument) ;

				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryAvilabeInstrument_Rsp,
					lVecInstrument.size() == 0 ? NULL : &lVecInstrument[0],
					sizeof(PlatformStru_InstrumentInfo)*lVecInstrument.size(),
					PkgHead.seq,
					0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS,0);
			}
			else
			{
				SendRspError(hSocket,Cmd_RM_QryAvilabeInstrument_Rsp,
					PkgHead.seq,
					CF_ERROR_TRADEDATA_NO_INSTRUMENTS,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_TRADEDATA_NO_INSTRUMENTS,0);
				break;
			}

			break;
		}

	}
}
void GenerateSQL(SAdminQuery* pAdminQuery, std::string& strSql)
{
	std::string strInstrument="";			
	for(int i = 0; i< (int)pAdminQuery->nCount; i++)
	{
		std::string str = pAdminQuery->strAccount[i];
		if(i == 0)
		{
			std::string strName = " (investorid = ''";
			strInstrument = strName;
			int nLength = strName.length() -1;
			strInstrument.insert(nLength,  str);
			continue;
		}					

		std::string strNameOr = " or investorid =''";
		int nLengthOr = strNameOr.length() -1;
		strInstrument.insert(strInstrument.length(),  strNameOr);		
		strInstrument.insert(strInstrument.length()-1,  str);					
	}	
	if(strInstrument.length())
		strInstrument.append(1, ')');				

	bool bAnd = false;
	if(strInstrument.length() >0)
	{
		strSql.insert(strSql.length(), strInstrument);
		bAnd = true;
	}

	if(strcmp(pAdminQuery->szDateBegin, "") != 0 && strcmp(pAdminQuery->szDateEnd, "") != 0)
	{
		std::string strName = " to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date(''";
		if(bAnd)
			strName = " and  to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date(''";
		strName.insert(strName.length()-1, pAdminQuery->szDateBegin);
		strSql.insert(strSql.length() , strName);

		std::string strName2 = ",'YYYY-MM-DD')	and to_date(t.TRADINGDAY,'YYYYMMDD') <= to_date(''";
		strName2.insert(strName2.length()-1, pAdminQuery->szDateEnd);
		strSql.insert(strSql.length() , strName2);

		std::string strName3 = ",'YYYY-MM-DD') order by t.TRADINGDAY";			
		strSql.insert(strSql.length() , strName3);
		bAnd = true;
	}	
}

void ProcessAdminQuery(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	switch(PkgHead.cmdid)
	{	
	case Cmd_RM_QryHistoryFundInfo_Req:
		{
			SAdminQuery* pAdminQuery = (SAdminQuery*)pPkgData;
			if(pAdminQuery == NULL)
			{	
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryFundInfo_Rsp, 
					(void*)lErrorString, strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
				return;
			}
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,QUERY_HISTORY_FUND))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_QUERY_HISTORY_FUND_NOPRIVILEDGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryFundInfo_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
			std::string strSql = "select * from SETTLEMENT_FUNDINFO t where ";

			std::string strInstrument="";			
			for(int i = 0; i< (int)pAdminQuery->nCount; i++)
			{
				std::string str = pAdminQuery->strAccount[i];
				if(i == 0)
				{
					std::string strName = " (accountid = ''";
					strInstrument = strName;
					int nLength = strName.length() -1;
					strInstrument.insert(nLength,  str);
					continue;
				}					

				std::string strNameOr = " or accountid =''";
				int nLengthOr = strNameOr.length() -1;
				strInstrument.insert(strInstrument.length(),  strNameOr);		
				strInstrument.insert(strInstrument.length()-1,  str);					
			}	
			if(strInstrument.length())
				strInstrument.append(1, ')');				

			bool bAnd = false;
			if(strInstrument.length() >0)
			{
				strSql.insert(strSql.length(), strInstrument);
				bAnd = true;
			}

			if(strcmp(pAdminQuery->szDateBegin, "") != 0 && strcmp(pAdminQuery->szDateEnd, "") != 0)
			{
				std::string strName = " to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date(''";
				if(bAnd)
					strName = " and  to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date(''";
				strName.insert(strName.length()-1, pAdminQuery->szDateBegin);
				strSql.insert(strSql.length() , strName);

				std::string strName2 = ",'YYYY-MM-DD')	and to_date(t.TRADINGDAY,'YYYYMMDD') <= to_date(''";
				strName2.insert(strName2.length()-1, pAdminQuery->szDateEnd);
				strSql.insert(strSql.length() , strName2);

				std::string strName3 = ",'YYYY-MM-DD') order by t.TRADINGDAY";			
				strSql.insert(strSql.length() , strName3);
				bAnd = true;
			}	
			
			std::vector<PlatformStru_TradingAccountInfo> vec;

			std::vector<std::vector<_variant_t>> vNode;
			int nErrorCode = CF_ERROR_SUCCESS;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(strSql.c_str(), vNode, nErrorCode))
			{
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInfo_Rsp,
					(void*)lErrorString, strlen(lErrorString)+1,PkgHead.seq,0,nErrorCode);	
				return ;
			}
			else if(vNode.size() == 0)
			{
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInfo_Rsp,
			//		NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);	
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];
					PlatformStru_TradingAccountInfo sResult;

					int i = 0;
					strcpy(sResult.BrokerID, vValue[i++].operator _bstr_t());
					strcpy(sResult.AccountID, vValue[i++].operator _bstr_t());					
					sResult.PreMortgage = vValue[i++].dblVal;
					sResult.PreCredit = vValue[i++].dblVal;
					sResult.PreDeposit = vValue[i++].dblVal;
					sResult.PreBalance = vValue[i++].dblVal;
					sResult.PreMargin = vValue[i++].dblVal;
					sResult.InterestBase = vValue[i++].dblVal;
					sResult.Interest = vValue[i++].dblVal;
					sResult.Deposit = vValue[i++].dblVal;
					sResult.Withdraw = vValue[i++].dblVal;
					sResult.FrozenMargin = vValue[i++].dblVal;
					sResult.FrozenCash = vValue[i++].dblVal;
					sResult.FrozenCommission = vValue[i++].dblVal;
					sResult.CurrMargin = vValue[i++].dblVal;
					sResult.CashIn = vValue[i++].dblVal;
					sResult.Commission = vValue[i++].dblVal;
					sResult.CloseProfit = vValue[i++].dblVal;
					sResult.PositionProfit = vValue[i++].dblVal;
					sResult.Balance = vValue[i++].dblVal;
					sResult.Available = vValue[i++].dblVal;
					sResult.WithdrawQuota = vValue[i++].dblVal;
					sResult.Reserve = vValue[i++].dblVal;
					strcpy(sResult.TradingDay, vValue[i++].operator _bstr_t());					
					sResult.SettlementID = vValue[i++].intVal;
					sResult.Credit = vValue[i++].dblVal;
					sResult.Mortgage = vValue[i++].dblVal;
					sResult.ExchangeMargin = vValue[i++].dblVal;
					sResult.DeliveryMargin = vValue[i++].dblVal;
					sResult.ExchangeDeliveryMargin = vValue[i++].dblVal;
					sResult.StaticProfit = vValue[i++].dblVal;
					sResult.DynamicProfit = vValue[i++].dblVal;
					sResult.RiskDegree = vValue[i++].dblVal;

					vec.push_back(sResult);			
				}
			}
			if(vec.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInfo_Rsp,
				&vec[0], sizeof(PlatformStru_TradingAccountInfo)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);	
			else
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInfo_Rsp,
				NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;		
		}
		
	case Cmd_RM_QryHistoryPosition_Req:
		{
			SAdminQuery* pAdminQuery = (SAdminQuery*)pPkgData;
			if(pAdminQuery == NULL)
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryPosition_Rsp, 
					(void*)lErrorString, strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
				return;
			}
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,QUERY_HISTORY_POSITION))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_QUERY_HISTORY_POSITION_NOPRIVILEDGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryPosition_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
			
			std::string strSql = "select * from SETTLEMENT_POSITION t where ";		
			GenerateSQL(pAdminQuery, strSql);

			std::vector<PlatformStru_Position> vec;

			std::vector<std::vector<_variant_t>> vNode;
			int nErrorCode = CF_ERROR_SUCCESS;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(strSql.c_str(), vNode, nErrorCode))
			{
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPosition_Rsp,
					(void*)lErrorString, 
					strlen(lErrorString)+1,PkgHead.seq,0,nErrorCode);	
				return ;
			}
			else if(vNode.size() == 0)
			{
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPosition_Rsp,
			//		NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);	
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];
					PlatformStru_Position sResult;

					int i = 0;
					strcpy(sResult.InstrumentID, vValue[i++].operator _bstr_t());
					strcpy(sResult.BrokerID, vValue[i++].operator _bstr_t());
					strcpy(sResult.InvestorID, vValue[i++].operator _bstr_t());
					sResult.PosiDirection			= vValue[i++].cVal;
					sResult.HedgeFlag				= vValue[i++].cVal;
					strcpy(sResult.TradingDay, vValue[i++].operator _bstr_t());
					sResult.PositionDate			= vValue[i++].cVal;
					sResult.SettlementID            = vValue[i++].intVal;
					sResult.Position                = vValue[i++].intVal;
					sResult.TodayPosition           = vValue[i++].intVal;
					sResult.YdPosition              = vValue[i++].intVal;
					sResult.OpenVolume              = vValue[i++].intVal;
					sResult.CloseVolume             = vValue[i++].intVal;
					sResult.OpenAmount				= vValue[i++].dblVal;
					sResult.CloseAmount				= vValue[i++].dblVal;
					sResult.PositionCost			= vValue[i++].dblVal;
					sResult.OpenCost			    = vValue[i++].dblVal;
					sResult.LongFrozen              = vValue[i++].intVal;
					sResult.ShortFrozen             = vValue[i++].intVal;
					sResult.LongFrozenAmount	    = vValue[i++].dblVal;
					sResult.ShortFrozenAmount	    = vValue[i++].dblVal;
					sResult.FrozenMargin			= vValue[i++].dblVal;
					sResult.FrozenCommission		= vValue[i++].dblVal;
					sResult.FrozenCash				= vValue[i++].dblVal;
					sResult.Commission				= vValue[i++].dblVal;
					sResult.PreMargin				= vValue[i++].dblVal;
					sResult.UseMargin				= vValue[i++].dblVal;
					sResult.ExchangeMargin			= vValue[i++].dblVal;
					sResult.MarginRateByMoney		= vValue[i++].dblVal;
					sResult.MarginRateByVolume		= vValue[i++].dblVal;
					sResult.CashIn					= vValue[i++].dblVal;
					sResult.PositionProfit			= vValue[i++].dblVal;
					sResult.CloseProfit				= vValue[i++].dblVal;
					sResult.CloseProfitByDate		= vValue[i++].dblVal;
					sResult.CloseProfitByTrade		= vValue[i++].dblVal;
					sResult.PreSettlementPrice		= vValue[i++].dblVal;
					sResult.SettlementPrice			= vValue[i++].dblVal;
					sResult.CombPosition            = vValue[i++].intVal;
					sResult.CombLongFrozen          = vValue[i++].intVal;
					sResult.CombShortFrozen         = vValue[i++].intVal;
					sResult.PositionProfitByTrade   = vValue[i++].dblVal;
					sResult.TotalPositionProfitByDate   = vValue[i++].dblVal;

					vec.push_back(sResult);			
				}
			}
			if(vec.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPosition_Rsp,
				&vec[0], sizeof(PlatformStru_Position)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);	
			else
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPosition_Rsp,
				NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;		
		}
	case Cmd_RM_QryHistoryPositionDetail_Req:
		{
			SAdminQuery* pAdminQuery = (SAdminQuery*)pPkgData;
			if(pAdminQuery == NULL)
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryPositionDetail_Rsp, 
					(void*)lErrorString, strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
				return;
			}
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,QUERY_HISTORY_POSITION_DETAIL))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_QUERY_HISTORY_POSITION_DETAIL_NOPRIVILEDGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryPositionDetail_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}

			std::string strSql = "select * from SETTLEMENT_POSITIONDETAIL t where ";		
			GenerateSQL(pAdminQuery, strSql);

			std::vector<PlatformStru_PositionDetail> vec;

			std::vector<std::vector<_variant_t>> vNode;
			int nErrorCode = CF_ERROR_SUCCESS;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(strSql.c_str(), vNode, nErrorCode))
			{
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPositionDetail_Rsp,
					(void*)lErrorString, 
					strlen(lErrorString)+1,PkgHead.seq,0,nErrorCode);	
				return ;
			}
			else if(vNode.size() == 0)
			{
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPositionDetail_Rsp,
			//		NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);	
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];
					PlatformStru_PositionDetail sResult;

					int i = 0;
					strcpy(sResult.InstrumentID, vValue[i++].operator _bstr_t());
					strcpy(sResult.BrokerID, vValue[i++].operator _bstr_t());
					strcpy(sResult.InvestorID, vValue[i++].operator _bstr_t());
					sResult.HedgeFlag				= vValue[i++].cVal;
					sResult.Direction			    = vValue[i++].cVal;
					strcpy(sResult.OpenDate, vValue[i++].operator _bstr_t());
					strcpy(sResult.TradeID, vValue[i++].operator _bstr_t());
					sResult.Volume					= vValue[i++].intVal;
					sResult.OpenPrice				= vValue[i++].dblVal;
					strcpy(sResult.TradingDay, vValue[i++].operator _bstr_t());
					sResult.SettlementID			= vValue[i++].intVal;
					sResult.TradeType				= vValue[i++].cVal;
					strcpy(sResult.CombInstrumentID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ExchangeID, vValue[i++].operator _bstr_t());
					sResult.CloseProfitByDate	    = vValue[i++].dblVal;
					sResult.CloseProfitByTrade	    = vValue[i++].dblVal;
					sResult.PositionProfitByDate    = vValue[i++].dblVal;
					sResult.PositionProfitByTrade   = vValue[i++].dblVal;
					sResult.Margin					= vValue[i++].dblVal;
					sResult.ExchMargin				= vValue[i++].dblVal;
					sResult.MarginRateByMoney		= vValue[i++].dblVal;
					sResult.MarginRateByVolume		= vValue[i++].dblVal;
					sResult.LastSettlementPrice		= vValue[i++].dblVal;
					sResult.SettlementPrice			= vValue[i++].dblVal;
					sResult.CloseVolume				= vValue[i++].intVal;
					sResult.CloseAmount				= vValue[i++].dblVal;
				

					vec.push_back(sResult);			
				}
			}
			if(vec.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPositionDetail_Rsp,
				&vec[0], sizeof(PlatformStru_PositionDetail)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);	
			else
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryPositionDetail_Rsp,
				NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;		
		}
		break;
	case Cmd_RM_QryHistoryTrade_Req:
		{
			SAdminQuery* pAdminQuery = (SAdminQuery*)pPkgData;
			if(pAdminQuery == NULL)
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryTrade_Rsp, 
					(void*)lErrorString, strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
				return;
			}
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,QUERY_HISTORY_TRADE))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_QUERY_HISTORY_TRADE_NOPRIVILEDGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryTrade_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
			std::string strSql = "select * from  SETTLEMENT_TRADE t where ";		
			GenerateSQL(pAdminQuery, strSql);

			std::vector<PlatformStru_TradeInfo> vec;

			std::vector<std::vector<_variant_t>> vNode;
			int nErrorCode = CF_ERROR_SUCCESS;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(strSql.c_str(), vNode, nErrorCode))
			{
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryTrade_Rsp,
					(void*)lErrorString, 
					strlen(lErrorString)+1,PkgHead.seq,0,nErrorCode);	
				return ;
			}
			else if(vNode.size() == 0)
			{
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryTrade_Rsp,
			//		NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);	
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];
					PlatformStru_TradeInfo sResult;

					int i = 0;					
					strcpy(sResult.BrokerID, vValue[i++].operator _bstr_t());
					strcpy(sResult.InvestorID, vValue[i++].operator _bstr_t());
					strcpy(sResult.InstrumentID, vValue[i++].operator _bstr_t());
					strcpy(sResult.OrderRef, vValue[i++].operator _bstr_t());
					strcpy(sResult.UserID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ExchangeID, vValue[i++].operator _bstr_t());
					strcpy(sResult.TradeID, vValue[i++].operator _bstr_t());
					sResult.Direction			    = vValue[i++].cVal;
					strcpy(sResult.OrderSysID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ParticipantID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ClientID, vValue[i++].operator _bstr_t());
					sResult.TradingRole			    = vValue[i++].cVal;
					strcpy(sResult.ExchangeInstID, vValue[i++].operator _bstr_t());
					sResult.OffsetFlag			    = vValue[i++].cVal;
					sResult.HedgeFlag				= vValue[i++].cVal;
					sResult.Price					= vValue[i++].dblVal;
					sResult.Volume					= vValue[i++].intVal;
					strcpy(sResult.TradeDate, vValue[i++].operator _bstr_t());
					strcpy(sResult.TradeTime, vValue[i++].operator _bstr_t());
					sResult.TradeType			    = vValue[i++].cVal;
					sResult.PriceSource				= vValue[i++].cVal;
					strcpy(sResult.TraderID, vValue[i++].operator _bstr_t());
					strcpy(sResult.OrderLocalID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ClearingPartID, vValue[i++].operator _bstr_t());
					strcpy(sResult.BusinessUnit, vValue[i++].operator _bstr_t());
					sResult.SequenceNo					= vValue[i++].intVal;
					strcpy(sResult.TradingDay, vValue[i++].operator _bstr_t());
					sResult.SettlementID					= vValue[i++].intVal;
					sResult.BrokerOrderSeq					= vValue[i++].intVal;
					sResult.TradeSource						= vValue[i++].cVal;
					sResult.CloseProfitByDate				= vValue[i++].dblVal;	
					sResult.CloseProfitByTrade				= vValue[i++].dblVal;	
					sResult.TradeCommission					= vValue[i++].dblVal;	
				
					vec.push_back(sResult);			
				}
			}
			if(vec.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryTrade_Rsp,
				&vec[0], sizeof(PlatformStru_TradeInfo)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);	
			else
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryTrade_Rsp,
				NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;		
		}
	case Cmd_RM_QryHistoryOrders_Req:
		{
			SAdminQuery* pAdminQuery = (SAdminQuery*)pPkgData;
			if(pAdminQuery == NULL)
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryOrders_Rsp, 
					(void*)lErrorString, strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
				return;
			}
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,QUERY_HISTORY_ORDER))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_QUERY_HISTORY_ORDER_NOPRIVILEDGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryOrders_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
			std::string strSql = "select * from  TRADEDATA_ORDERS t, SETTLEMENTDAY t1 where (to_date(t.TRADINGDAY,'YYYYMMDD')= to_date(t1.settlementday,'YYYY-MM-DD')) ";		
		//	GenerateSQL(pAdminQuery, strSql);

//////////////////////////////////////////////////////////////////////////
			bool bAnd = true;

			std::string strInstrument="";			
			for(int i = 0; i< (int)pAdminQuery->nCount; i++)
			{
				std::string str = pAdminQuery->strAccount[i];
				if(i == 0)
				{
					std::string strName = " (t.investorid = ''";
					if(bAnd)
						strName = " and (t.investorid = ''";
					strInstrument = strName;
					int nLength = strName.length() -1;
					strInstrument.insert(nLength,  str);
					continue;
				}					

				std::string strNameOr = " or t.investorid =''";
				int nLengthOr = strNameOr.length() -1;
				strInstrument.insert(strInstrument.length(),  strNameOr);		
				strInstrument.insert(strInstrument.length()-1,  str);					
			}	
			if(strInstrument.length())
				strInstrument.append(1, ')');				

			
			if(strInstrument.length() >0)
			{
				strSql.insert(strSql.length(), strInstrument);
				bAnd = true;
			}

			if(strcmp(pAdminQuery->szDateBegin, "") != 0 && strcmp(pAdminQuery->szDateEnd, "") != 0)
			{
				std::string strName = " to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date(''";
				if(bAnd)
					strName = " and  to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date(''";
				strName.insert(strName.length()-1, pAdminQuery->szDateBegin);
				strSql.insert(strSql.length() , strName);

				std::string strName2 = ",'YYYY-MM-DD')	and to_date(t.TRADINGDAY,'YYYYMMDD') <= to_date(''";
				strName2.insert(strName2.length()-1, pAdminQuery->szDateEnd);
				strSql.insert(strSql.length() , strName2);

				std::string strName3 = ",'YYYY-MM-DD') order by t.TRADINGDAY";			
				strSql.insert(strSql.length() , strName3);
				bAnd = true;
			}	




//////////////////////////////////////////////////////////////////////////
			std::vector<PlatformStru_OrderInfo> vec;

			std::vector<std::vector<_variant_t>> vNode;
			int nErrorCode = CF_ERROR_SUCCESS;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(strSql.c_str(), vNode, nErrorCode))
			{
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryOrders_Rsp,
					(void*)lErrorString, strlen(lErrorString)+1,PkgHead.seq,0,nErrorCode);	
				return ;
			}
			else if(vNode.size() == 0)
			{
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryOrders_Rsp,
			//		NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);	
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];
					PlatformStru_OrderInfo sResult;

					int i = 0;					
					strcpy(sResult.BrokerID, vValue[i++].operator _bstr_t());
					strcpy(sResult.InvestorID, vValue[i++].operator _bstr_t());
					strcpy(sResult.InstrumentID, vValue[i++].operator _bstr_t());
					strcpy(sResult.OrderRef, vValue[i++].operator _bstr_t());
					strcpy(sResult.UserID, vValue[i++].operator _bstr_t());
					sResult.OrderPriceType			    = vValue[i++].cVal;
					sResult.Direction			    = vValue[i++].cVal;
					strcpy(sResult.CombOffsetFlag, vValue[i++].operator _bstr_t());
					strcpy(sResult.CombHedgeFlag, vValue[i++].operator _bstr_t());						
					sResult.LimitPrice				= vValue[i++].dblVal;
					sResult.VolumeTotalOriginal		= vValue[i++].intVal;
					sResult.TimeCondition			= vValue[i++].cVal;
					strcpy(sResult.GTDDate, vValue[i++].operator _bstr_t());
					sResult.VolumeCondition			= vValue[i++].cVal;
					sResult.MinVolume				= vValue[i++].intVal;
					sResult.ContingentCondition		= vValue[i++].cVal;
					sResult.StopPrice				= vValue[i++].dblVal;
					sResult.ForceCloseReason		= vValue[i++].cVal;
					sResult.IsAutoSuspend			= vValue[i++].intVal;
					strcpy(sResult.BusinessUnit, vValue[i++].operator _bstr_t());
					sResult.RequestID				= vValue[i++].intVal;
					strcpy(sResult.OrderLocalID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ExchangeID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ParticipantID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ClientID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ExchangeInstID, vValue[i++].operator _bstr_t());
					strcpy(sResult.TraderID, vValue[i++].operator _bstr_t());
					sResult.InstallID				= vValue[i++].intVal;
					sResult.OrderSubmitStatus		= vValue[i++].cVal;
					sResult.NotifySequence			= vValue[i++].intVal;
					strcpy(sResult.TradingDay, vValue[i++].operator _bstr_t());
					sResult.SettlementID				= vValue[i++].intVal;
					strcpy(sResult.OrderSysID, vValue[i++].operator _bstr_t());
					sResult.OrderSource		= vValue[i++].cVal;
					sResult.OrderStatus		= vValue[i++].cVal;
					sResult.OrderType		= vValue[i++].cVal;
					sResult.VolumeTraded	= vValue[i++].intVal;
					sResult.VolumeTotal		= vValue[i++].intVal;
					strcpy(sResult.InsertDate, vValue[i++].operator _bstr_t());
					strcpy(sResult.InsertTime, vValue[i++].operator _bstr_t());
					strcpy(sResult.ActiveTime, vValue[i++].operator _bstr_t());
					strcpy(sResult.SuspendTime, vValue[i++].operator _bstr_t());
					strcpy(sResult.UpdateTime, vValue[i++].operator _bstr_t());
					strcpy(sResult.CancelTime, vValue[i++].operator _bstr_t());
					strcpy(sResult.ActiveTraderID, vValue[i++].operator _bstr_t());
					strcpy(sResult.ClearingPartID, vValue[i++].operator _bstr_t());
					sResult.SequenceNo			= vValue[i++].intVal;
					sResult.FrontID				= vValue[i++].intVal;
					sResult.SessionID			= vValue[i++].intVal;
					strcpy(sResult.UserProductInfo, vValue[i++].operator _bstr_t());
					strcpy(sResult.StatusMsg, vValue[i++].operator _bstr_t());
					sResult.UserForceClose			= vValue[i++].intVal;
					strcpy(sResult.ActiveUserID, vValue[i++].operator _bstr_t());
					sResult.BrokerOrderSeq			= vValue[i++].intVal;
					strcpy(sResult.RelativeOrderSysID, vValue[i++].operator _bstr_t());
					sResult.AvgPrice				= vValue[i++].dblVal;
					sResult.ExStatus			= (PlatformStru_OrderInfo::EnumExStatus)vValue[i++].intVal;

					vec.push_back(sResult);			
				}
			}
			if(vec.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryOrders_Rsp,
				&vec[0], sizeof(PlatformStru_OrderInfo)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);	
			else
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryOrders_Rsp,
				NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;		
		}
	case Cmd_RM_QryHistoryFundInOut_Req:
		{			
			SAdminQuery* pAdminQuery = (SAdminQuery*)pPkgData;
			if(pAdminQuery == NULL)
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_QryHistoryFundInOut_Rsp, 
					(void*)lErrorString, strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
				return;
			}

			std::string strSql = "select * from  TRADEDATA_FUNDINOUT t where ";		
			std::string strInstrument="";			
			for(int i = 0; i< (int)pAdminQuery->nCount; i++)
			{
				std::string str = pAdminQuery->strAccount[i];
				if(i == 0)
				{
					std::string strName = " (USERID = ''";
					strInstrument = strName;
					int nLength = strName.length() -1;
					strInstrument.insert(nLength,  str);
					continue;
				}					

				std::string strNameOr = " or USERID =''";
				int nLengthOr = strNameOr.length() -1;
				strInstrument.insert(strInstrument.length(),  strNameOr);		
				strInstrument.insert(strInstrument.length()-1,  str);					
			}	
			if(strInstrument.length())
				strInstrument.append(1, ')');				

			bool bAnd = false;
			if(strInstrument.length() >0)
			{
				strSql.insert(strSql.length(), strInstrument);
				bAnd = true;
			}

			if(strcmp(pAdminQuery->szDateBegin, "") != 0 && strcmp(pAdminQuery->szDateEnd, "") != 0)
			{
				std::string strName = " to_date(t.OPDATE,'YYYY-MM-DD') >= to_date(''";
				if(bAnd)
					strName = " and  to_date(t.OPDATE,'YYYY-MM-DD') >= to_date(''";
				strName.insert(strName.length()-1, pAdminQuery->szDateBegin);
				strSql.insert(strSql.length() , strName);

				std::string strName2 = ",'YYYY-MM-DD')	and to_date(t.OPDATE,'YYYY-MM-DD') <= to_date(''";
				strName2.insert(strName2.length()-1, pAdminQuery->szDateEnd);
				strSql.insert(strSql.length() , strName2);

				std::string strName3 = ",'YYYY-MM-DD') order by t.OPDATE";			
				strSql.insert(strSql.length() , strName3);
				bAnd = true;
			}	

			std::vector<sFundInOut> vec;

			std::vector<std::vector<_variant_t>> vNode;
			int nErrorCode = CF_ERROR_SUCCESS;
			if ( !CInterface_SvrDBOpr::getObj().QueryData(strSql.c_str(), vNode, nErrorCode))
			{
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInOut_Rsp,
					(void*)lErrorString, strlen(lErrorString)+1,PkgHead.seq,0,nErrorCode);	
				return ;
			}
			else if(vNode.size() == 0)
			{
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInOut_Rsp,
			//		NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);	
			}
			else
			{
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];
					sFundInOut sResult;

					int i = 0;					
					strcpy(sResult.mUserID, vValue[i++].operator _bstr_t());
					sResult.meInOut				= (eInOut)vValue[i++].intVal;
					sResult.mdbVolume			= vValue[i++].dblVal;
					strcpy(sResult.mOpAdminID, vValue[i++].operator _bstr_t());
					strcpy(sResult.msDesc, vValue[i++].operator _bstr_t());
					strcpy(sResult.msDay, vValue[i++].operator _bstr_t());
					strcpy(sResult.msTime, vValue[i++].operator _bstr_t());

					vec.push_back(sResult);			
				}
			}
			if(vec.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInOut_Rsp,
				&vec[0], sizeof(sFundInOut)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);	
			else
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_QryHistoryFundInOut_Rsp,
				NULL, 0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;	
		}
	}
	
}