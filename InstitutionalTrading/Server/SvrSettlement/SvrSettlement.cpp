// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrSettlement.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonMacro.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"
#include "Interface_SvrSettlement.h"
#include "SvrSettlementBusiness.h"
#include "../SvrTradeData/Interface_SvrTradeData.h"
#include <vector>
#include <map>

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
#pragma comment(lib, "SvrTradeData.lib")
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrBrokerInfo.lib")
#pragma comment(lib, "SvrDBOpr.lib")
#pragma comment(lib, "SvrMsg.lib")

#define WRITELOGID_SvrSettlement




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);
string szSettleTime="";
UINT_PTR lTime =NULL;

//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//// 解析一个数据通知事件
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);

//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRSETTLEMENT_API void InitFunc(void)
{
	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
}

//模块结束，释放资源，关闭工作线程
SVRSETTLEMENT_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
		//退订数据包
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//发送WM_QUIT通知线程结束
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		KillTimer(NULL,lTime);
		lTime=NULL;
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

void  TimerWork()
{
		time_t lTime;
		struct tm * lTimeinfo; 
		time(&lTime);
		lTimeinfo = localtime(&lTime);

		struct tm  lzSettleTimeinfo;
		memcpy(&lzSettleTimeinfo,lTimeinfo,sizeof(tm));
		sscanf_s(szSettleTime.c_str(),"%02d:%02d:%02d",&lzSettleTimeinfo.tm_hour,&lzSettleTimeinfo.tm_min,&lzSettleTimeinfo.tm_sec);
		time_t SettleTime = mktime(&lzSettleTimeinfo);
		time_t lDifftime = lTime - SettleTime;

		if( 0 <= lDifftime && lDifftime < 5*60 )
		{
			SvrSettlementBusiness::getObj().AutoSettlement();
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
	}
}
//工作线程
DWORD ThreadWorker(void *arg)
{
	//下面订阅本线程感兴趣的数据包
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QuerySettlementPrice_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ModifySettlementPrice_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryCommissionMarginRate_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ModifyCommissionMarginRate_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ConfirmCommissionMarginRate_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryTrade_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_AddTrade_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_DeleteTrade_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ModifyTrade_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryPositionDetail_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_AddPositionDetail_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_DeletePositionDetail_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ModifyPositionDetail_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryPosition_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_AddPosition_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_DeletePosition_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ModifyPosition_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_SetSettltmentTime_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ModifyFund_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryFund_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_SettlementFinish_Notify,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_LoadData_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ConfirmTrade_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_StartSettlement_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ConfirmSettlementPrice_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryTradingday_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_TradeCaculate_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg( Cmd_Settlement_SettleBill_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg( Cmd_Settlement_QuerySettltmentTime_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg( Cmd_Settlement_SettlementWinColse_Notify,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpDisconn,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_QueryCTPLoadStatus_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_LoadHistoryCommission_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_Settlement_ImmediatelyAutoSettlement_Req,g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartInit, g_idThread);


	
	if (SvrSettlementBusiness::getObj().LoadSettltTime(szSettleTime))
	{
		//设置一个每5分钟触发一次的定时器
		lTime = SetTimer(NULL,NULL,5*60*1000,(TIMERPROC)(NULL)); 
	}
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message == WM_TIMER)
		{
			TimerWork();	
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
		else if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_NotifyEventArrival)
		{
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//数据包长度
			Stru_NotifyEvent ls;
			int nLen = sizeof(ls);
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,&ls,&nLen,nLen,NULL))
			{			
				//调用数据包处理函数处理数据包
				ParseDataChangedEvent(ls);			
				//释放EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}


		}
	}
	return 0;
}

// 解析一个数据通知事件
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
	OUTPUT_LOG("Event Arrived with id %d",dataEvt.meEventType);
	switch(dataEvt.meEventType) 
	{
	case EventTypeStartInit:    //交易数据管理模块开始初始化			
		{
			OUTPUT_LOG("EventTypeStartInit");
			SvrSettlementBusiness::getObj().SetSettlementStatus(No_Settlementing);

			break;
		}

	default: break;
	};
	return true;
}

//处理一个SvrTcp推送过来的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	下面根据数据包的命令字，处理数据包
	//-----------------------------------------------------------------------------------
	OUTPUT_LOG("结算服务端收到消息： %d",  PkgHead.cmdid);
	switch(PkgHead.cmdid)
	{
	case Cmd_Settlement_StartSettlement_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_StartSettlement_Rsp,PkgHead.seq);
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_MANUAL))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_StartSettlement_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
			
			int bSettle = SvrSettlementBusiness::getObj().IsSettled();

			CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_StartSettlement_Rsp,
					&bSettle,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

				break;

		}
	case Cmd_Settlement_QueryTradingday_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryTradingday_Rsp,PkgHead.seq);
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_MANUAL))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryTradingday_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}

			//PlatformStru_QrySettlementInfo Data ;
			//memset(&Data,0,sizeof(PlatformStru_QrySettlementInfo));
			//strncpy(Data.Thost.InvestorID,"0002",sizeof(Data.Thost.InvestorID));
			//string strContent="";
			//SvrSettlementBusiness::getObj().QuerySettlementInfo( Data,strContent);



			string pData="";
			int nErrorCode = SvrSettlementBusiness::getObj().QueryTradingday(pData);
			char pBuf[11]={0};
			strncpy(pBuf,pData.c_str(),sizeof(pBuf));

			//查看系统当前时间是否在该日的日盘结算后
			sTradingTime lTradingDay;
			memset(&lTradingDay,0,sizeof(sTradingTime));
			CInterface_SvrTradeData::getObj().GetTradingTime(lTradingDay);
			time_t lTime;	
			time(&lTime);

			int bEndTrade=0;
			struct tm  lTradingDayEndTimeinfo ;
			memset(&lTradingDayEndTimeinfo,0,sizeof(lTradingDayEndTimeinfo));
			sscanf_s(lTradingDay.mTradingDayEnd,"%02d:%02d:%02d",&lTradingDayEndTimeinfo.tm_hour,&lTradingDayEndTimeinfo.tm_min,&lTradingDayEndTimeinfo.tm_sec);
			sscanf_s(pData.c_str(),"%4d-%02d-%02d",&lTradingDayEndTimeinfo.tm_year,&lTradingDayEndTimeinfo.tm_mon,&lTradingDayEndTimeinfo.tm_mday);
			lTradingDayEndTimeinfo.tm_year -= 1900;
			lTradingDayEndTimeinfo.tm_mon -= 1;
			time_t lTradingDayEndTime = mktime(&lTradingDayEndTimeinfo);
			if(lTradingDayEndTime > lTime )
			{		
				bEndTrade = 0;
			}
			else
			{
				bEndTrade = 1;
			}

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryTradingday_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode,bEndTrade);			
			}
			else
			{

			CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryTradingday_Rsp,
				(void*)pBuf,sizeof(pBuf),PkgHead.seq,0,CF_ERROR_SUCCESS,bEndTrade);
			}

			break;

		}
	case Cmd_Settlement_LoadData_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(_SettlementDate),hSocket,Cmd_Settlement_LoadData_Rsp,PkgHead.seq);
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_MANUAL) && !PkgHead.userdata3
				||(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_HISTORY) && PkgHead.userdata3))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_LoadData_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
            string strDate = (char*)pPkgData;
			//SvrSettlementBusiness::getObj().SetSettlementDay(strDate);

			//如果是同一用户切换加载数据方式，先设置结算状态
			SvrSettlementBusiness::getObj().RemoveSettleSocket(hSocket);

			int nErrorCode =SvrSettlementBusiness::getObj().loadData((eSettlementLoadOption)PkgHead.userdata2,strDate,PkgHead.userdata3);

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_LoadData_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{

                SvrSettlementBusiness::getObj().SetSettleSocket(hSocket);

				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_LoadData_Rsp,
					NULL,0,PkgHead.seq,0,nErrorCode,PkgHead.userdata2);

            }
				break;
			
		}

	case Cmd_Settlement_QueryCTPLoadStatus_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryCTPLoadStatus_Rsp,PkgHead.seq);
			
			string pData="";
			int nErrorCode = SvrSettlementBusiness::getObj().GetCTPLoadStatus(pData);
			//char pBuf[11]={0};
			//strncpy(pBuf,pData.c_str(),sizeof(pBuf));
			//if(CF_ERROR_SUCCESS != nErrorCode)
			//{
			//	//发送错误回传
			//	const char * lErrorString = FormatErrorCode(nErrorCode);
			//	CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryTradingday_Rsp, 
			//		(void*)lErrorString, 
			//		strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			//}
			//else
			//{

				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryCTPLoadStatus_Rsp,
					(void*)pData.c_str(),pData.size()+1,PkgHead.seq,0,CF_ERROR_SUCCESS);
			/*}*/

			break;

		}

	case Cmd_Settlement_QuerySettlementPrice_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QuerySettlementPrice_Rsp,PkgHead.seq);

			std::vector<SettlementPriceField> lVecFiled;
			int nErrorCode =SvrSettlementBusiness::getObj().QuerySettlementPrice(lVecFiled,PkgHead.userdata2);
		

			//发送回传,结算价可能不全，客户端做检查**********************
			if(lVecFiled.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QuerySettlementPrice_Rsp,
				&lVecFiled[0],sizeof(SettlementPriceField)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			else
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QuerySettlementPrice_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}

			break;
		}

	case Cmd_Settlement_ModifySettlementPrice_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(SettlementPriceField),hSocket,Cmd_Settlement_ModifySettlementPrice_Rsp,PkgHead.seq);

			SettlementPriceField pData = *(SettlementPriceField*)pPkgData;
			int userId = PkgHead.userdata1;

			int nErrorCode = SvrSettlementBusiness::getObj().ModifySettlementPrice(pData,userId);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ModifySettlementPrice_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ModifySettlementPrice_Rsp,
					&pData,sizeof(SettlementPriceField),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_ConfirmSettlementPrice_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_ConfirmSettlementPrice_Rsp,PkgHead.seq);

			int nErrorCode = SvrSettlementBusiness::getObj().ConfirmSettlementPrice();
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ConfirmSettlementPrice_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ConfirmSettlementPrice_Rsp,
					NULL,0,PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_QueryCommissionMarginRate_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryCommissionMarginRate_Rsp,PkgHead.seq);

			std::vector<CommissionMarginRate> lVecFiled;
			std::map<RateKey,CommissionMarginRate> lMapFiled;


			int nErrorCode =SvrSettlementBusiness::getObj().QueryCommissionRate(lMapFiled);
			std::map<RateKey,CommissionMarginRate>::iterator lIter = lMapFiled.begin();
			while(lIter != lMapFiled.end())
			{
				lVecFiled.push_back(lIter->second);
				lIter++;
			}

			//发送回传
			if(lVecFiled.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryCommissionMarginRate_Rsp,
				&lVecFiled[0],sizeof(CommissionMarginRate)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			else
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryCommissionMarginRate_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}

			break;
		}

	case Cmd_Settlement_LoadHistoryCommission_Req:
		{
			//BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_LoadHistoryCommission_Rsp,PkgHead.seq);
            std::vector<RateKey> vecRate;
			string strDate="";
			int nCount = PkgHead.len / sizeof(QryHisRateCondition);
			for ( int i = 0; i < nCount; i++ )
			{
				QryHisRateCondition* pField = (QryHisRateCondition*)((char*)pPkgData + i*sizeof(QryHisRateCondition));
                strDate = pField->loadDate;
				vecRate.push_back(pField->conditionKey);
			}

			std::vector<CommissionMarginRate> lVecFiled;
			std::map<RateKey,CommissionMarginRate> lMapFiled;


			int nErrorCode =SvrSettlementBusiness::getObj().QueryHistoryCommissionRate(strDate,vecRate,lMapFiled);
			std::map<RateKey,CommissionMarginRate>::iterator lIter = lMapFiled.begin();
			while(lIter != lMapFiled.end())
			{
				lVecFiled.push_back(lIter->second);
				lIter++;
			}
			
			

			//发送回传
			if(lVecFiled.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_LoadHistoryCommission_Rsp,
				&lVecFiled[0],sizeof(CommissionMarginRate)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			else
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_LoadHistoryCommission_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}

			break;
		}

	case Cmd_Settlement_ModifyCommissionMarginRate_Req:
		{

			int nErrorCode=CF_ERROR_SUCCESS;
			SvrSettlementBusiness::getObj().ModifiedRateClear();
			bool ret=true;
	        int userId = PkgHead.userdata1;
			int nCount = PkgHead.len / sizeof(CommissionMarginRate);
			//先修改内存中费率
			for ( int i = 0; i < nCount; i++ )
			{
				CommissionMarginRate* pField = (CommissionMarginRate*)((char*)pPkgData + i*sizeof(CommissionMarginRate));
				nErrorCode =SvrSettlementBusiness::getObj().ModifyCommissionRate(*pField,userId);
				if (nErrorCode)
				{
					ret=false;
					//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ModifyCommissionMarginRate_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);	

				break;
				}
			}
			

			if (ret)
			{
				//再通知数据管理模块修改
		     	nErrorCode =SvrSettlementBusiness::getObj().SetModifiedRate();
				if (nErrorCode == CF_ERROR_SUCCESS)
				{
					//通知数据管理模块重算
				    nErrorCode = SvrSettlementBusiness::getObj().ConfirmTrade();
				  //重新获取成交数据
					SvrSettlementBusiness::getObj().GetTrade();
				}
				

				if(CF_ERROR_SUCCESS != nErrorCode)
				{
					//发送错误回传
					const char * lErrorString = FormatErrorCode(nErrorCode);
					CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ModifyCommissionMarginRate_Rsp, 
						(void*)lErrorString, 
						strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
				}
				else
				{
					CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ModifyCommissionMarginRate_Rsp,
						NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
				}


			}
				


			break;
		}

	case Cmd_Settlement_ConfirmCommissionMarginRate_Req:
		{
			int nErrorCode=CF_ERROR_SUCCESS;
			//再通知数据管理模块修改
			nErrorCode =SvrSettlementBusiness::getObj().SetModifiedRate();
			if (nErrorCode == CF_ERROR_SUCCESS)
			{
				//通知数据管理模块重算
				nErrorCode = SvrSettlementBusiness::getObj().ConfirmTrade();
				//重新获取成交数据
				SvrSettlementBusiness::getObj().GetTrade();
			}
			break;
		}
	case Cmd_Settlement_QueryTrade_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryTrade_Rsp,PkgHead.seq);

			std::vector<PlatformStru_TradeInfo> lVecFiled;
			int nErrorCode =SvrSettlementBusiness::getObj().QueryTrade(lVecFiled);			

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryTrade_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryTrade_Rsp,
					&lVecFiled[0],sizeof(PlatformStru_TradeInfo)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	
	case Cmd_Settlement_DeleteTrade_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(PlatformStru_TradeInfo),hSocket,Cmd_Settlement_DeleteTrade_Rsp,PkgHead.seq);

			PlatformStru_TradeInfo pData = *(PlatformStru_TradeInfo*)pPkgData;

			int userId = PkgHead.userdata1;
			int nErrorCode = SvrSettlementBusiness::getObj().DeleteTrade(pData,userId);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_DeleteTrade_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_DeleteTrade_Rsp,
					&pData,sizeof(PlatformStru_TradeInfo),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_AddTrade_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(PlatformStru_TradeInfo),hSocket,Cmd_Settlement_AddTrade_Rsp,PkgHead.seq);

			PlatformStru_TradeInfo pData = *(PlatformStru_TradeInfo*)pPkgData;
			int userId = PkgHead.userdata1;

			int nErrorCode = SvrSettlementBusiness::getObj().AddTrade(pData,userId);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_AddTrade_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_AddTrade_Rsp,
					&pData,sizeof(PlatformStru_TradeInfo),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_ModifyTrade_Req:
		{

			PlatformStru_TradeInfo pData1 = *(PlatformStru_TradeInfo*)pPkgData;
			PlatformStru_TradeInfo *pData2 = (PlatformStru_TradeInfo*)((char*)(pPkgData) + sizeof(PlatformStru_TradeInfo));
			int userId = PkgHead.userdata1;

			int nErrorCode = SvrSettlementBusiness::getObj().ModifyTrade(pData1,*pData2,userId);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ModifyTrade_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ModifyTrade_Rsp,
					pData2,sizeof(PlatformStru_TradeInfo),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_ConfirmTrade_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_ConfirmTrade_Rsp,PkgHead.seq);

			int nErrorCode = SvrSettlementBusiness::getObj().ConfirmTrade();
			SvrSettlementBusiness::getObj().LoadFund();
			
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ConfirmTrade_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ConfirmTrade_Rsp,
					NULL,0,PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_TradeCaculate_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_TradeCaculate_Rsp,PkgHead.seq);
            //通知数据管理模块重算
			int nErrorCode = SvrSettlementBusiness::getObj().ConfirmTrade();
			std::vector<PlatformStru_TradeInfo> lVecFiled;
			if (nErrorCode == CF_ERROR_SUCCESS )
			{   //重新获取成交数据
				SvrSettlementBusiness::getObj().GetTrade();
				nErrorCode =SvrSettlementBusiness::getObj().QueryTrade(lVecFiled);
			}
						

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_TradeCaculate_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_TradeCaculate_Rsp,
					&lVecFiled[0],sizeof(PlatformStru_TradeInfo)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			}


			break;
		}

	case Cmd_Settlement_QueryPositionDetail_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryPositionDetail_Rsp,PkgHead.seq);

			std::vector<PlatformStru_PositionDetail> lVecFiled;
			int nErrorCode =SvrSettlementBusiness::getObj().QueryPositionDetail(lVecFiled);

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryPositionDetail_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryPositionDetail_Rsp,
					&lVecFiled[0],sizeof(PlatformStru_PositionDetail)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_AddPositionDetail_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(PlatformStru_PositionDetail),hSocket,Cmd_Settlement_AddPositionDetail_Rsp,PkgHead.seq);

			PlatformStru_PositionDetail pData = *(PlatformStru_PositionDetail*)pPkgData;

			int nErrorCode = SvrSettlementBusiness::getObj().AddPositionDetail(pData);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_AddPositionDetail_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_AddPositionDetail_Rsp,
					&pData,sizeof(PlatformStru_PositionDetail),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_DeletePositionDetail_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(PlatformStru_PositionDetail),hSocket,Cmd_Settlement_DeletePositionDetail_Rsp,PkgHead.seq);

			PlatformStru_PositionDetail pData = *(PlatformStru_PositionDetail*)pPkgData;

			int nErrorCode = SvrSettlementBusiness::getObj().DeletePositionDetail(pData);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_DeletePositionDetail_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_DeletePositionDetail_Rsp,
					&pData,sizeof(PlatformStru_PositionDetail),PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_ModifyPositionDetail_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(PlatformStru_PositionDetail),hSocket,Cmd_Settlement_ModifyPositionDetail_Rsp,PkgHead.seq);

			PlatformStru_PositionDetail pData = *(PlatformStru_PositionDetail*)pPkgData;

			int nErrorCode = SvrSettlementBusiness::getObj().ModifyPositionDetail(pData);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ModifyPositionDetail_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ModifyPositionDetail_Rsp,
					&pData,sizeof(PlatformStru_PositionDetail),PkgHead.seq,0,nErrorCode);
			}


			break;
		}

	case Cmd_Settlement_QueryPosition_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryPosition_Rsp,PkgHead.seq);
			
			std::vector<PlatformStru_Position> lVecFiled;
			int nErrorCode =SvrSettlementBusiness::getObj().QueryPosition(lVecFiled);

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryPosition_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryPosition_Rsp,
					&lVecFiled[0],sizeof(PlatformStru_Position)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			}


			break;
		}

	case Cmd_Settlement_SetSettltmentTime_Req://客户端设置自动结算时间
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(_SettlementDate),hSocket,Cmd_Settlement_SetSettltmentTime_Rsp,PkgHead.seq);
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_AUTO_SETUP))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_SetSettltmentTime_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}

			_SettlementDate *pData = (_SettlementDate*)pPkgData;
			if (PkgHead.userdata2 == 1)//设置自动结算时间--开启
			{
				if (lTime == NULL)
				{
					//设置一个每5分钟触发一次的定时器
				    lTime = SetTimer(NULL,NULL,5*60*1000,(TIMERPROC)(NULL)); 
				}
				
				szSettleTime = *pData;
			}
			if(lTime != NULL && PkgHead.userdata2 == 0)//设置自动结算时间--关闭
			{
				KillTimer(NULL,lTime);
				lTime=NULL;

			}

			int nErrorCode = SvrSettlementBusiness::getObj().SetSettltmentTime(pData,PkgHead.userdata2);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_SetSettltmentTime_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_SetSettltmentTime_Rsp,
					NULL,0,PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_QuerySettltmentTime_Req://客户端查询自动结算时间
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QuerySettltmentTime_Rsp,PkgHead.seq);

            int nErrorCode = CF_ERROR_SUCCESS;
			std::string szSettleTime;
			int flag = SvrSettlementBusiness::getObj().LoadSettltTime(szSettleTime);
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QuerySettltmentTime_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QuerySettltmentTime_Rsp,
					(void*)szSettleTime.c_str(),szSettleTime.size()+1,PkgHead.seq,0,nErrorCode,flag);
			}


			break;
		}

	case Cmd_Settlement_QueryFund_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_QueryFund_Rsp,PkgHead.seq);

			std::vector<PlatformStru_TradingAccountInfo> lVecFiled;
			int nErrorCode =SvrSettlementBusiness::getObj().QueryFund(lVecFiled);

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_QueryFund_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_QueryFund_Rsp,
					&lVecFiled[0],sizeof(PlatformStru_TradingAccountInfo)*lVecFiled.size(),PkgHead.seq,0,nErrorCode);
			}


			break;
		}

	case Cmd_Settlement_ModifyFund_Req:
		{
			
			int nErrorCode=CF_ERROR_SUCCESS;
			int userId = PkgHead.userdata1;
			int nCount = PkgHead.len / sizeof(PlatformStru_TradingAccountInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				PlatformStru_TradingAccountInfo* pField = (PlatformStru_TradingAccountInfo*)((char*)pPkgData + i*sizeof(PlatformStru_TradingAccountInfo));
                nErrorCode = SvrSettlementBusiness::getObj().ModifyFund( *pField ,userId);
				if (CF_ERROR_SUCCESS != nErrorCode)
				break;
			}


			
			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ModifyFund_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ModifyFund_Rsp,
					NULL,0,PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_SettlementFinish_Notify:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_SettlementFinish_Confirm,PkgHead.seq);
			/*if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_MANUAL))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_SettlementFinish_Confirm, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}*/

			//入库
			int nErrorCode =SvrSettlementBusiness::getObj().SaveData();
            SvrSettlementBusiness::getObj().SetSettlementStatus(No_Settlementing);

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_SettlementFinish_Confirm, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_SettlementFinish_Confirm,
					NULL,0,PkgHead.seq,0,nErrorCode);
			}


			break;
		}
	case Cmd_Settlement_SettlementWinColse_Notify:
		{
			
			
			SvrSettlementBusiness::getObj().RemoveSettleSocket(hSocket);

			break;
		}
	case  Cmd_Settlement_SettleBill_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(PlatformStru_QrySettlementInfo),hSocket,Cmd_Settlement_SettleBill_Rsp,PkgHead.seq);
			/*if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_MANUAL))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_SettleBill_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}*/

			PlatformStru_QrySettlementInfo *pData = (PlatformStru_QrySettlementInfo*)pPkgData;
			string strContent="";
			int nErrorCode =SvrSettlementBusiness::getObj().QuerySettlementInfo( *pData,strContent);

			CThostFtdcSettlementInfoField field;
			memset(&field,0,sizeof(field));
			strncpy(field.InvestorID ,pData->Thost.InvestorID,sizeof(field.InvestorID));
			int nTotalPkg = strContent.size() / 500;
			if ( strContent.size() % 500 != 0 )
			{
				nTotalPkg += 1;
			}

			int bIsLast=0;
			for ( int i = 0; i < nTotalPkg; i++ )
			{				
				if (i == nTotalPkg-1)
				{
					bIsLast =1;
				}
				strncpy(field.Content ,strContent.c_str()+i*500,500);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_SettleBill_Rsp,
			&field,sizeof(field),PkgHead.seq,0,0,PkgHead.userdata3,nErrorCode,bIsLast);

			}





			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				strncpy(field.Content ,lErrorString,500);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_SettleBill_Rsp, 
					&field,sizeof(field), PkgHead.seq,0,0,PkgHead.userdata3,CF_ERROR_SUCCESS,1);			
			}
			
				


			break;
		}
	case CMDID_SvrTcpDisconn:
		{				
			SvrSettlementBusiness::getObj().RemoveSettleSocket(hSocket);
			break;
		}
	case Cmd_Settlement_ImmediatelyAutoSettlement_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_Settlement_ImmediatelyAutoSettlement_Rsp,PkgHead.seq);
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SETTLEMENT_MANUAL))
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ImmediatelyAutoSettlement_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}

			//如果是同一用户切换加载数据方式，先设置结算状态
			SvrSettlementBusiness::getObj().RemoveSettleSocket(hSocket);
			int nErrorCode =SvrSettlementBusiness::getObj().ImmediatelyAutoSettlement();

			if(CF_ERROR_SUCCESS != nErrorCode)
			{
				//发送错误回传
				const char * lErrorString = FormatErrorCode(nErrorCode);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_Settlement_ImmediatelyAutoSettlement_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
			}
			else
			{

				SvrSettlementBusiness::getObj().SetSettleSocket(hSocket);

				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_Settlement_ImmediatelyAutoSettlement_Rsp,
					NULL,0,PkgHead.seq,0,nErrorCode,PkgHead.userdata2);

			}

			break;

		}

	}
}
