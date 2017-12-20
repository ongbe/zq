#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "StdAfx.h"
#include <windows.h>
#include "SvrOffer.h"
#include "CommonPkg.h"

#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"

#include "OfferMainInterface.h"
#include "CommonDataChangeEvent.h"
#include "ToolThread.h"

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
#pragma comment(lib, "SvrTradeData.lib")
#pragma comment(lib, "SvrBrokerInfo.lib")
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrLogin.lib")



#define WRITELOGID_SvrOffer




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;


COfferMainInterface*			g_pOfferMain;
UINT_PTR            g_lTime;//定时检查账号有注册前端
extern UINT_PTR g_lTimeDisconnect;//账号断开
extern UINT_PTR g_lTimeQuto;//订阅行情
//CToolThread*		g_hThreadProcessTradeData;
//DWORD WINAPI ThreadProcTradeDataQuery (void* pParam);
DWORD ThreadWorker(void *arg);

//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//处理内部通知的事件
bool ParseDataChangedEvent(Stru_NotifyEvent& dataEvt);

bool LoadXmlConfig(std::string strConfig, std::string& strIP, int& nPort)
{
	std::string apppath=strConfig;	
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr( 0, nPos + 1 );
		apppath = strSystemPath + apppath;
	}	
	
	TiXmlDocument* pDocument=new TiXmlDocument(apppath.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			const char* pRootElement = RootElement->Value();
			if(strcmp(pRootElement, "root") ==0)
			{
				TiXmlNode* pNode=RootElement->FirstChild();
				if(pNode)
				{
					const char* pElement = pNode->Value();
					if(strcmp(pElement, "MarketData") ==0)
					{
						TiXmlElement* pChild=pNode->FirstChildElement();
						if(pChild)
						{
							std::string str = pChild->GetText();

							int nPos = str.find(':');
							if ( nPos != -1 )
							{
								strIP = str.substr(0, nPos);
								nPort = atoi(str.substr(nPos+1, str.length()).c_str());
							//	CTCPDataService::GetInstance()->Connect(strIP, nPort);
							}
						}

					}
					
				}

			}			
		}
	}

	return true;
}

//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVROFFER_API void InitFunc(void)
{		
	g_pOfferMain = NULL;
	

	

	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	//g_hThreadProcessTradeData = new CToolThread((LPTHREAD_START_ROUTINE)ThreadProcTradeDataQuery,NULL);
		
	//下面订阅本线程感兴趣的数据包
//	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ReInitQuotServerEvent_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuotServerTimeResetEvent_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryQuotServerTimeEvent_Req,g_idThread);
	//	

	////订阅交易数据管理模块的数据包
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryInstrument, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryCommission, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryMargin, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryOrder, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryTrader, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedSingleLoginTrade, g_idThread);

	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedGetInstrument, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginTrade, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginSettlement, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedLogoffSettlement, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartSettlement, g_idThread);//断开ctp连接
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeInitFinish, g_idThread);

	
	
	//CInterface_SvrTradeData::getObj().TradeDataInit();

	

	
}

//模块结束，释放资源，关闭工作线程
SVROFFER_API void ReleaseFunc(void)
{
	if(g_pOfferMain)
	{
		delete g_pOfferMain;
		g_pOfferMain = NULL;
	}

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
	}
}
//工作线程
DWORD ThreadWorker(void *arg)
{
	//In the thread to which the message will be posted, call PeekMessage as shown here to force the system to create the message queue.
	MSG MsgTemp;
	PeekMessage(&MsgTemp, NULL, WM_USER, WM_USER, PM_NOREMOVE);//调用这个函数用于建立消息队列

	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ReInitQuotServerEvent_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuotServerTimeResetEvent_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryQuotServerTimeEvent_Req,g_idThread);

	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QrySimulateDay_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QrySimulatePlayMode_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulatePlay_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulatePause_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulateStop_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulateApply_Req,g_idThread);



	//订阅交易数据管理模块的数据包
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryInstrument, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryCommission, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryMargin, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryOrder, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryTrader, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedSingleLoginTrade, g_idThread);

	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryFund, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryPosition, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryPositionDetail, g_idThread);

	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedGetInstrument, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginTrade, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginSettlement, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedLogoffSettlement, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartSettlement, g_idThread);//断开ctp连接
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeInitFinish, g_idThread);
	
	CInterface_SvrTradeData::getObj().TradeDataInit();

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message == WM_TIMER)
		{
			if(g_lTimeDisconnect == Msg.wParam)
			{
				CCTPAccount *pCtpAccount = NULL;
				bool bGet = g_pOfferMain->GetCtpAccountByTime(Msg.wParam, pCtpAccount);
				if(bGet && pCtpAccount)
				{//这种情况是针对断开连接后的
					SLogin login;
					pCtpAccount->GetLogin(login);
					CInterface_SvrTradeData::getObj().SetAccountStatus(login.strUserID, login.strBrokerID, ACCOUNT_STATUS_CannotConnect);
					g_pOfferMain->RemoveCtpAccountByTime(Msg.wParam);
					KillTimer(NULL, Msg.wParam);	

					if(pCtpAccount->GetIsQueryRatio())
					{
						OFFER_INFO("设置查费率账号断开连接ctp状态： %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())
						PUSH_LOG_ERROR(FirstLevelError,true,true,"设置查费率账号断开连接ctp状态： %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())
					}
					else
					{
						OFFER_INFO("设置主账号断开连接ctp状态： %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())
						PUSH_LOG_ERROR(FirstLevelError,true,true,"设置主账号断开连接ctp状态： %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())

					}

					continue;
				}
			}

			if(Msg.wParam == g_lTime)
			{//这种情况针对所有账户连接10秒没连上，算登不上的
				if(g_pOfferMain)
				{				
					g_pOfferMain->CheckAccountsConnect();
					
				}
				KillTimer(NULL, g_lTime);
			}
			if(Msg.wParam == g_lTimeQuto)
			{//订阅行情
				std::map<std::string, int> mapInstrumentID;
				CTCPDataService::GetInstance()->GetQutoInstruments(mapInstrumentID);
				if(g_pOfferMain && mapInstrumentID.size()>0)
				{				
					g_pOfferMain->QueryQuot(mapInstrumentID);
				}				
			}			
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
				//申请临时空间并获取数据包
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
				//数据包的传输ID
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

//处理一个SvrTcp推送过来的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	下面根据数据包的命令字，处理数据包
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{
		case CMDID_HEARTBEAT:
			{
				break;
			}	
		case Cmd_ReInitQuotServerEvent_Req:
			{
				if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_SERVER_QUOT_RESET))
				{
					const char * lErrorString = FormatErrorCode(CF_ERROR_CONTROL_SERVER_QUOT_RESET_NOPRIVILEDGE);
					CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ReInitQuotServerEvent_Rsp, 
						(void*)lErrorString, 
						strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
					break;
				}				
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_ReInitQuotServerEvent_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QuotServerTimeResetEvent_Req:
			{
				if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_TIME_QUOT_SETUP))
				{
					const char * lErrorString = FormatErrorCode(CF_ERROR_CONTROL_TIME_QUOT_SETUP_NOPRIVILEDGE);
					CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_QuotServerTimeResetEvent_Rsp, 
						(void*)lErrorString, 
						strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
					break;
				}

				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QuotServerTimeResetEvent_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QryQuotServerTimeEvent_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QryQuotServerTimeEvent_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QrySimulateDay_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QrySimulateDay_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QrySimulatePlayMode_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QrySimulatePlayMode_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulatePlay_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulatePlay_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulatePause_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulatePause_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulateStop_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulateStop_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulateApply_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulateApply_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
	}
}
// 解析一个数据通知事件
bool ParseDataChangedEvent(Stru_NotifyEvent& dataEvt)
{
	switch(dataEvt.meEventType)
	{
	case EventTypeInitFinish:
		{//初始化完成，连接行情服务器开始订阅，主要为了兼容数据管理准备好持仓的问题。
			std::string strIP;
			int nPort;
			LoadXmlConfig("offer-config.xml", strIP, nPort);
			CTCPDataService::GetInstance()->Connect(strIP, nPort);	
			OFFER_INFO("连接行情服务器：IP[%s], Port[%d]\n", strIP.c_str(), nPort);
			PUSH_LOG_ERROR(Logger,false,true,"连接行情服务器：IP[%s], Port[%d]\n", strIP.c_str(), nPort);
		}
		break;
	case EventTypeNeedQryInstrument:			
		{//查询合约

			if(g_pOfferMain == NULL)
				return false;
		
			g_pOfferMain->QueryInstruments();	
			OFFER_INFO("收到查询合约\n");
			PUSH_LOG_ERROR(Logger,false,true,"收到查询合约任务\n");
		}
		break;
	case EventTypeNeedGetInstrument:
		{
			if(g_pOfferMain)
				g_pOfferMain->AddInstrument();
			OFFER_INFO("合约从交易数据获取\n");
			PUSH_LOG_ERROR(Logger,false,true,"收到从交易数据获取合约任务\n");
		}
		break;
	case EventTypeNeedAllLoginTrade:
		{
			OFFER_INFO("报盘收到登陆所有委托交易帐号指令\n");
			PUSH_LOG_ERROR(Logger,false,true,"报盘收到登陆所有委托交易帐号指令\n");
			KillTimer(NULL, g_lTime);
			if(g_pOfferMain)
			{
				delete g_pOfferMain;
				g_pOfferMain = NULL;
			}
			int nMorning =  dataEvt.mnDataID;//日盘事件ID为0 ,夜盘为1
			#ifdef  SIMU//模拟交易				
				g_pOfferMain = new COfferMain_Simu;
				
			#else
				g_pOfferMain = new COfferMain;				
			#endif

				g_pOfferMain->Init(0, nMorning);//日盘事件ID，nMorning为0 ,夜盘为1	
			//定时10s检查账号是不是前端连接失败
			#ifndef  SIMU
				g_lTime =SetTimer(NULL,TIMER_CHECK_ACCOUNTSTATUS,10000,(TIMERPROC)(NULL)); 	
            #endif

			
//#ifdef  SIMU//模拟交易
//			CInterface_SvrSimulateTrade::getObj().Reinit();
//#endif
		}
		break;
	case EventTypeNeedSingleLoginTrade:
		{//查询费率
			if(g_pOfferMain == NULL)
				return false;

			int nTradeAccount = *(int*) dataEvt.marrDataByte;		
			g_pOfferMain->RestartAccount(nTradeAccount);		
			OFFER_INFO("重启账户： %d\n", nTradeAccount);
			PUSH_LOG_ERROR(Logger,false,true,"重新登陆账户： %d\n", nTradeAccount);
		}
		break;	
	case EventTypeStartSettlement:
		{ //开始结算，断开所有ctp连接
			KillTimer(NULL, g_lTime);
			if(g_pOfferMain)
			{
				delete g_pOfferMain;
				g_pOfferMain = NULL;
			}
			//HANDLE hEvent = (HANDLE)dataEvt.mhEvent;		
			//SetEvent(hEvent);
		}
		break;
	case EventTypeNeedAllLoginSettlement:
		{
			KillTimer(NULL, g_lTime);
			if(g_pOfferMain)
			{
				delete g_pOfferMain;
				g_pOfferMain = NULL;
			}
			#ifdef  SIMU//模拟交易
				g_pOfferMain = new COfferMain_Simu;
			#else
				g_pOfferMain = new COfferMain;
			#endif

			g_pOfferMain->Init(1, 2);	//2这个数值没意义
			//定时10s检查账号是不是前端连接失败
#ifndef  SIMU
			g_lTime =SetTimer(NULL,TIMER_CHECK_ACCOUNTSTATUS,10000,(TIMERPROC)(NULL)); 	
#endif
		}
		break;
	case EventTypeNeedQryCommission:
		{//查询费率
			if(g_pOfferMain == NULL)
				return false;

			BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
			
			g_pOfferMain->QueryCommission(*BAKey);		
			OFFER_INFO("收到查询手续费率： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
			PUSH_LOG_ERROR(Logger,false,true,"收到查询手续费率任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		}
		break;	
	case EventTypeNeedQryMargin:
		{
			if(g_pOfferMain == NULL)
				return false;
			BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
			g_pOfferMain->QueryMargin(*BAKey);	
			OFFER_INFO("收到查询保证金率： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
			PUSH_LOG_ERROR(Logger,false,true,"收到查询保证金率任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		}
		break;   	 
   case EventTypeNeedQryOrder:
	   {
		   if(g_pOfferMain == NULL)
			   return false;
		  BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		  g_pOfferMain->QueryOrder(*BAKey);	
		  OFFER_INFO("收到查询报单： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		  PUSH_LOG_ERROR(Logger,false,true,"收到查询报单任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedQryTrader:
	   {//查询成交
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryTrade(*BAKey);	
		   OFFER_INFO("收到查询成交： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"收到查询成交任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedLogoffSettlement:
	   {//断开ctp账号连接
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->LogoffAccount(*BAKey);
			
	   }
	   break;
   case EventTypeNeedQryFund:
	   {
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryFund(*BAKey);	
		   OFFER_INFO("收到查询资金： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"收到资金任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedQryPosition:
	   {
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryPosition(*BAKey);	
		   OFFER_INFO("收到查询持仓： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"收到持仓任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedQryPositionDetail:
	   {//持仓明细最后查询，作为查询线程结束工作的标识
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryPositionDetail(*BAKey);	
		   OFFER_INFO("收到查询持仓明细： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"收到持仓明细任务： %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
	}
	return true;
}