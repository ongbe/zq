// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "stdafx.h"
#include <windows.h>
//#include "SvrLogin.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "Tools_Win32.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrUserOrg\\Interface_SvrUserOrg.h"
#include "..\\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"

#include "Interface_SvrLogin.h"
#include "CustomSvrLoginInterface.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "SocketInfoMgr.h"
#include "CommonDataChangeEvent.h"



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
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrNotifyAndAsk.lib")



#define WRITELOGID_SvrLogin




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);
UINT_PTR gLoginTimer = 0;
UINT_PTR gHeartBeatTimer = 0;
SOCKET   gMasterServerSocket = 0;


//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);

void GetNewTime(const _ExchangTimeType& nOldTime,const _ExchangTimeType & nNeedGetTime,_ExchangTimeType & lretTime)
{
	time_t lTime;
	struct tm * lOldTimeinfo; 
	struct tm * lNeedGetTimeInfo; 
	time(&lTime);
	lOldTimeinfo = localtime(&lTime);
	lNeedGetTimeInfo = localtime(&lTime);

	sscanf_s(nOldTime,"%02d:%02d:%02d",&lOldTimeinfo->tm_hour,&lOldTimeinfo->tm_min,&lOldTimeinfo->tm_sec);
	sscanf_s(nNeedGetTime,"%02d:%02d:%02d",&lNeedGetTimeInfo->tm_hour,&lNeedGetTimeInfo->tm_min,&lNeedGetTimeInfo->tm_sec);

	time_t lOldTime = mktime(lOldTimeinfo);
	time_t lNeedGetTime = mktime(lNeedGetTimeInfo);

	lNeedGetTime += (time_t)difftime(lTime,lOldTime);	
	lNeedGetTimeInfo = localtime(&lNeedGetTime);


	sprintf_s(lretTime,sizeof(lretTime),"%02d:%02d:%02d",lNeedGetTimeInfo->tm_hour,lNeedGetTimeInfo->tm_min,lNeedGetTimeInfo->tm_sec);
	
}


//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRLOGIN_API void InitFunc(void)
{
	//读配置文件决定主从模式
	CCunstomSvrLoginInterface::getObj();

	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);


}

//模块结束，释放资源，关闭工作线程
SVRLOGIN_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
		//退订数据包
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//发送WM_QUIT通知线程结束
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//等待线程退出
		CTools_Win32::StopThread((HANDLE)g_hThread,8);
		g_hThread=NULL;
		g_idThread=0;
	}


}

//因为AllocTmpStackBuf中申请栈空间只有在函数中才能自动释放，所以用DealPackage处理
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
DWORD ThreadWorker(void *arg)
{
	//下面订阅本线程感兴趣的数据包
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpDisconn,g_idThread); //服务器上有连接断开
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_Login_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_Logoff_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_Trader_Login_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SubscriberAllUserLoginStatus_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_UnsubscriberAllUserLoginStatus_Req,g_idThread);

	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartSettlement,g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserStop,g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartInit, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeErrorCome, g_idThread);

	if(CInterface_SvrLogin::getObj().IsSlaver())
	{
        //断开连接		
        //map<int,pair<unsigned int,unsigned short>> lMapSocket;
		//CInterface_SvrTcp::getObj().GetSocket_RemoteAddr_of_TcpClient(lMapSocket);
		//if(lMapSocket.size() > 0)
		//	CInterface_SvrTcp::getObj().Disconnect(lMapSocket.begin()->first);
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_TcpClientConnect,g_idThread); //从服务器连上主服务器
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_TcpClientDisconn,g_idThread); //从服务器和主服务器断开
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_Login_Rsp,g_idThread);        //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_QueryMasterAllUserLoginStatus_Rsp,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_AddUserLoginStatusToMaster_Rsp,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_RemoveUserLoginStatusToMaster_Rsp,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_AddUserLoginStatusToSlave_Req,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_RemoveUserLoginStatusToSlave_Req,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_NotifySendToSlave_Req,g_idThread);                //从服务器订阅主服务器的登录返回包

	}
	else
	{
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_QueryMasterAllUserLoginStatus_Req,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_AddUserLoginStatusToMaster_Req,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_RemoveUserLoginStatusToMaster_Req,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_AddUserLoginStatusToSlave_Rsp,g_idThread);       //从服务器订阅主服务器的登录返回包
		CInterface_SvrTcp::getObj().SubscribePkg(CMDID_RemoveUserLoginStatusToSlave_Rsp,g_idThread);       //从服务器订阅主服务器的登录返回包

	}


	//if(!CInterface_SvrLogin::getObj().IsSlaver())
	{
		//基础数据改变后的广播事件	
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeAccountAdd ,  g_idThread);     //增加一个委托交易账号  LPARAM为AccountID
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeAccountDelete,  g_idThread);   //删除一个委托交易账号
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeAccountUpdate,  g_idThread);   //更新一个委托交易账号
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserAdd ,   g_idThread);       //增加一个用户
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserDelete,  g_idThread);      //删除一个用户
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserUpdate,    g_idThread);    //更新一个用户
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserLimitAndVerifyUpdate,  g_idThread);      //更新一个用户的下单和手工审核权限
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserAndOrgRelation,   g_idThread);      //用户和组织机构关系改变
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserAndRoleRelation,  g_idThread);      //用户和角色关系改变
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserAndTradeAcctRelation, g_idThread);  //用户和委托交易账号关系改变
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeRoleAdd,   g_idThread);                 //增加一个角色
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeRoleDelete,  g_idThread);               //删除一个角色
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeRoleUpdate,  g_idThread);               //更新一个角色
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeRoleAndPrivelegeRelation,  g_idThread); //角色和权限关系改变
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeOrgAdd,  g_idThread);                  //增加一个组织机构
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeOrgDelete,  g_idThread);               //删除一个组织机构
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeOrgUpdate,   g_idThread);              //更新一个组织机构
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeTradeStrategyAdd,  g_idThread);                  //增加一个交易策略
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeTradeStrategyDelete,  g_idThread);               //删除一个交易策略
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeTradeStrategyUpdate,  g_idThread);               //更新一个交易策略
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeTradeStrategyAndOrgRelation, g_idThread);  //交易策略和组织机构关系改变
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeFinicialProductAdd,  g_idThread);                  //增加一个理财产品
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeFinicialProductDelete,  g_idThread);               //删除一个理财产品
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeFinicialProductUpdate,    g_idThread);             //更新一个理财产品
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeFinicialProductAndUserRelation,  g_idThread); //理财产品和用户关系改变
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeBrokerInfoAdd,   g_idThread);                 //增加一个经纪公司
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeBrokerInfoDelete, g_idThread);                //删除一个经纪公司
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeBrokerInfoUpdate,  g_idThread);               //更新一个经纪公司
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerGroupAdd,    g_idThread);                //增加一个服务器组
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerGroupDelete,  g_idThread);               //删除一个服务器组
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerGroupUpdate,   g_idThread);              //更新一个服务器组
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerAddressAdd,     g_idThread);               //增加一个服务器组地址
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerAddressDelete,    g_idThread);             //删除一个服务器组地址
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerAddressUpdate,     g_idThread);            //更新一个服务器组地址
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeBrokerInfoQuotUpdate,      g_idThread);           //更新一个行情经纪公司
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerAddressQuotAdd,       g_idThread);             //增加一个行情服务器组地址
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerAddressQuotDelete,  g_idThread);               //删除一个行情服务器组地址
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerAddressQuotUpdate,    g_idThread);             //更新一个行情服务器组地址
	}


	if( CInterface_SvrLogin::getObj().IsSlaver())
	{
		//创建心跳包定时器	
		gHeartBeatTimer = SetTimer(NULL,NULL,20000,TIMERPROC(NULL));
	}




	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_NotifyEventArrival)
		{
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//数据包长度
			Stru_NotifyEvent ls;
			int nLen = sizeof(ls);
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,
				NULL,&ls,&nLen,nLen,NULL))
			{			
				//调用数据包处理函数处理数据包
				ParseDataChangedEvent(ls);			
				//释放EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}

		}
		if(Msg.message == WM_TIMER )
		{
			if(Msg.wParam == gLoginTimer && gMasterServerSocket != 0)
			{
                LOGIN_LOG("Send Login Pakage to Master Server");
				//发登录主服务器
				sLoginReq sLogin;
				std::string lsUser,lsPass;
				CCunstomSvrLoginInterface::getObj().GetUserAndPass(lsUser,lsPass);
				strcpy(sLogin.msUserName, lsUser.c_str());
				strcpy(sLogin.msPassWord, lsPass.c_str());			
				sLogin.meUserType = USER_TYPE_SLAVESERVER;

				CInterface_SvrTcp::getObj().SendPkgData(gMasterServerSocket,
					CMDID_Login_Req,
					&sLogin,
					sizeof(sLogin),
					0,
					0,
					0,
					0,
					CF_ERROR_SUCCESS,0);				
			}
			if(Msg.wParam == gHeartBeatTimer && gMasterServerSocket != 0)
			{
				CInterface_SvrTcp::getObj().SendPkgData(gMasterServerSocket,
					CMDID_HEARTBEAT,
					NULL,
					0,
					0,
					0,
					0,
					0,
					CF_ERROR_SUCCESS,0);
				//LOGIN_LOG("Send HeartBeat to Master Server");
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
				
				DealPakage(EventParamID,PkgLen,hSocket);

				//释放EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}

		}
	}
	//杀定时器
	//KillTimer(NULL,lTime);

	return 0;
}

// 解析一个数据通知事件
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
	switch(dataEvt.meEventType) 
	{
	case EventTypeStartSettlement:			
		{
			OUTPUT_LOG("Close All User Socket but manager");
			//开始结算，需要断掉所有除管理员的链接
			CCunstomSvrLoginInterface::getObj().CloseAllTraderSocket();
			//HANDLE hEvent = (HANDLE)dataEvt.mhEvent;		
			//SetEvent(hEvent);
		}
		break;
   case EventTypeStartInit:    //交易数据管理模块开始初始化			
		{
			OUTPUT_LOG("EventTypeStartInit Close all User Socket");
			//需要断掉所有除管理员的链接
			CCunstomSvrLoginInterface::getObj().CloseAllTraderSocket();
			break;
		}
	case EventTypeUserStop:
		{		
			int nInvestorID = *(int*) dataEvt.marrDataByte;
			OUTPUT_LOG("Close User Socket %d",nInvestorID);
			CCunstomSvrLoginInterface::getObj().CloseUserSocket(nInvestorID);			
		}
		break;
	case EventTypeErrorCome:
		{
			//推送给管理终端
			sErrorInfo * lError =(sErrorInfo*)dataEvt.marrDataByte;
			if(lError != NULL)
			{
				std::vector<SOCKET> lvecSocket;
				CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_MANAGE,lvecSocket);
				for(int i = 0; i < lvecSocket.size(); i++)
				{
					//发送回传
					CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
						lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
				}

                ////推送给风控终端
                std::vector<SOCKET> lvecSocketRisk;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_RISK_CONTROL,lvecSocketRisk);
					for(int i = 0; i < lvecSocketRisk.size(); i++)
					{
						//发送非自己的回传
						//if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocketRisk[i],CMDID_GetALLError_Push,
							lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}

			}
		}
		break;
	case  EventTypeAccountAdd :
	case  EventTypeAccountDelete:
	case  EventTypeAccountUpdate:
	case  EventTypeUserAdd :
	case  EventTypeUserDelete:
	case  EventTypeUserUpdate:
	case  EventTypeUserLimitAndVerifyUpdate:
	case  EventTypeUserAndOrgRelation:
	case  EventTypeUserAndRoleRelation:
	case  EventTypeUserAndTradeAcctRelation:
	case  EventTypeRoleAdd:
	case  EventTypeRoleDelete:
	case  EventTypeRoleUpdate:
	case  EventTypeRoleAndPrivelegeRelation:
	case  EventTypeOrgAdd:
	case  EventTypeOrgDelete:
	case  EventTypeOrgUpdate:
	case  EventTypeTradeStrategyAdd:
	case  EventTypeTradeStrategyDelete:
	case  EventTypeTradeStrategyUpdate:
	case  EventTypeTradeStrategyAndOrgRelation:
	case  EventTypeFinicialProductAdd:
	case  EventTypeFinicialProductDelete:
	case  EventTypeFinicialProductUpdate:
	case  EventTypeFinicialProductAndUserRelation:
	case  EventTypeBrokerInfoAdd:
	case  EventTypeBrokerInfoDelete:
	case  EventTypeBrokerInfoUpdate:
	case  EventTypeServerGroupAdd:
	case  EventTypeServerGroupDelete:
	case  EventTypeServerGroupUpdate:
	case  EventTypeServerAddressAdd:
	case  EventTypeServerAddressDelete:
	case  EventTypeServerAddressUpdate:
	case  EventTypeBrokerInfoQuotUpdate:
	case  EventTypeServerAddressQuotAdd:
	case  EventTypeServerAddressQuotDelete:
	case  EventTypeServerAddressQuotUpdate:
		{
			//推送给管理终端
			sErrorInfo * lError =(sErrorInfo*)dataEvt.marrDataByte;
			if(lError != NULL)
			{
				if(lError->mnSendBit & eSendManager)
				{
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_MANAGE,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{
						//发送非自己的回传
						if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
								lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}
				}
				if(lError->mnSendBit & eSendRisk)
				{
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_RISK_CONTROL,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{
						/*int nUserID = -1;
						int nFrond,nSessionID;
						CCunstomSvrLoginInterface::getObj().GetSocketUserIDAndFrontSessionID(lError->mhSocket,nUserID,nFrond,nSessionID);
						CInterface_SvrUserOrg::getObj().*/
						//发送非自己的回传
						if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
							lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}
				}
				if(lError->mnSendBit & eSendPresentation)
				{
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_VIEWER,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{
						//发送非自己的回传
						if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
							lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}
				}
				if(lError->mnSendBit & eSendSpecialUserClient)
				{
					for(int i = 0; i < MAX_SEND_SPECIAL_USER_SIZE; i++)
					{
						if(lError->mnSpecialUserIDArray[i] == 0)
							break;
						else
						{
							std::vector<SOCKET> lvecSocket;
							CCunstomSvrLoginInterface::getObj().GetUserSockets(lError->mnSpecialUserIDArray[i],lvecSocket);
							for(int j = 0; j < lvecSocket.size(); j++)
							{									
								CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[j],CMDID_GetALLError_Push,
									lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
							}
						}
					}
				}

				//发送一个事件包给所有从服务
				if(!CCunstomSvrLoginInterface::getObj().IsSlaver())
				{				
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{				
						CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_NotifySendToSlave_Req,
							lError,sizeof(sErrorInfo),0,0,0);
					}	
				}
			}
		}
		break;
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
	switch(PkgHead.cmdid)
	{
		case CMDID_Trader_Login_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(sLoginReq),hSocket,CMDID_Trader_Login_Rsp,PkgHead.seq);

			const sLoginReq * lpLogin = reinterpret_cast<const sLoginReq*>(pPkgData);

			BREAK_ON_NULL_AND_SEND_ERROR_RSP(lpLogin,hSocket,CMDID_Trader_Login_Rsp,PkgHead.seq);
			

            LOGIN_LOG("CMDID_Trader_Login_Req   A trader login server : %s %s %d %d",lpLogin->msUserName,lpLogin->msPassWord,lpLogin->meClientType,lpLogin->meUserType);
			
			int n=GetTickCount();	
			
			//判断密码			
			int nUserID;
			CF_ERROR lRet = CInterface_SvrUserOrg::getObj().IsValidUser(lpLogin->msUserName,
				lpLogin->msPassWord,
				lpLogin->meUserType,nUserID);
			if(CF_ERROR_SUCCESS != lRet)
			{
				//发送错误回传
				SendRspError(hSocket,
					CMDID_Trader_Login_Rsp,
					PkgHead.seq,
					lRet,
					lRet,
					0,0,0);				
				break;
			}

			
			//int n1=GetTickCount();	
			//判断状态和用户数据可用
			lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Login,
				AskOperationTypeAdd,nUserID,(const char*)nUserID);
			if(lRet != CF_ERROR_SUCCESS)			
			{
				//发送错误回传
				SendRspError(hSocket,
					CMDID_Trader_Login_Rsp,
					PkgHead.seq,
					lRet,
					lRet,
					0,0,0);
				break;
			}

		//	int n2=GetTickCount();	
			//生成FrontID和SessionID
			int nFrontID,nSessionID;
			CCunstomSvrLoginInterface::getObj().GenerateUniqueFrontAndSessionID(nFrontID,nSessionID);

			//int n3=GetTickCount();	
			//构建socket info 存储在map中
			sSocketInfo ls(nUserID,lpLogin->meUserType,lpLogin->msUserName,nFrontID,nSessionID,1,hSocket);			

			//int n4=GetTickCount();	
			//构造rsp包
			g_mutex.lock();
			CCunstomSvrLoginInterface::getObj().InsertSocketInfo(ls);
            LOGIN_LOG("CMDID_Trader_Login_Req:InsertSocketInfo   nUserID=%d nFrontID=%d nSessionID=%d hSocket=%d",nUserID,nFrontID,nSessionID,hSocket);
			g_mutex.unlock();			

			//int n5=GetTickCount();
			sExchangeTimeInfo lTimeInfo;
			memset(&lTimeInfo,0,sizeof(lTimeInfo));
			CCunstomSvrLoginInterface::getObj().GetTimeInfo(lTimeInfo);
			sTraderLoginRsp lsRsp;
			memset(&lsRsp,0,sizeof(sTraderLoginRsp));
			lsRsp.mnUserID = nUserID;
			lsRsp.mnFrontID = nFrontID;
			lsRsp.mnSessionID = nSessionID;
			lsRsp.mnMinOrderRef = 1;
		
			_ExchangTimeType lCZCETime,lDCETime,lFFEXTime, lSHFETime;
			GetNewTime(lTimeInfo.GetTime,lTimeInfo.CZCETime,lCZCETime);		
			GetNewTime(lTimeInfo.GetTime,lTimeInfo.DCETime,lDCETime);		
			GetNewTime(lTimeInfo.GetTime,lTimeInfo.FFEXTime,lFFEXTime);	
			GetNewTime(lTimeInfo.GetTime,lTimeInfo.SHFETime,lSHFETime);	
			memcpy(&lsRsp.mCZCETime,&lCZCETime,sizeof(_ExchangTimeType));
			memcpy(&lsRsp.mDCETime,&lDCETime,sizeof(_ExchangTimeType));
			memcpy(&lsRsp.mFFEXTime,&lFFEXTime,sizeof(_ExchangTimeType));
			memcpy(&lsRsp.mSHFETime,&lSHFETime,sizeof(_ExchangTimeType));
			memcpy(&lsRsp.mTradingDay,lTimeInfo.TradingDay,sizeof(_ExchangTimeType));
			//int n6=GetTickCount();
			//发送回传
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_Trader_Login_Rsp,
				&lsRsp,sizeof(lsRsp),PkgHead.seq,0,CF_ERROR_SUCCESS);


            //先增加再广播，否则因同步问题造成取值不同的问题
            CCunstomSvrLoginInterface::getObj().AddOnlineUser(nUserID);

			//int n7=GetTickCount();
			//产生一个广播用户登陆的事件		
			Stru_NotifyEvent lsEvent(EventTypeUserLogin,nUserID);
			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
		
			
			if(CCunstomSvrLoginInterface::getObj().IsSlaver())
			{
				//通知主服务			
				CInterface_SvrTcp::getObj().SendPkgData(gMasterServerSocket,CMDID_AddUserLoginStatusToMaster_Req,
					&ls,sizeof(sSocketInfo),0,0,nUserID);		
			}
			else
			{
				//通知所有从服务
				std::vector<SOCKET> lvecSocket;
				CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
				for(int i = 0; i < lvecSocket.size(); i++)
				{				
					CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_AddUserLoginStatusToSlave_Req,
						&ls,sizeof(sSocketInfo),0,0,nUserID);
				}
			}

			break;
		}

		case  CMDID_Login_Req:
		{		
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(sLoginReq),hSocket,CMDID_Login_Rsp,PkgHead.seq);
		
			const sLoginReq * lpLogin = reinterpret_cast<const sLoginReq*>(pPkgData);
			
			BREAK_ON_NULL_AND_SEND_ERROR_RSP(lpLogin,hSocket,CMDID_Login_Rsp,PkgHead.seq);	
			
			LOGIN_LOG("CMDID_Login_Req Login Req User:%s Pass:%s ",lpLogin->msUserName,lpLogin->msPassWord);
	
			CF_ERROR lRet = CF_ERROR_SUCCESS;
			lRet = CCunstomSvrLoginInterface::getObj().IsAllowedLoginUserType(lpLogin->meUserType) ;
			if(lRet != CF_ERROR_SUCCESS)
			{
				//发送错误回传
				SendRspError(hSocket,
					CMDID_Login_Rsp,
					PkgHead.seq,
					lRet,
					lRet,
					0,0,0);				
				break;
			}

			int nUserID;
		
			if(lpLogin->meUserType == USER_TYPE_SLAVESERVER)
			{			
			
				//获取socket的ip			
				unsigned int lnIP;
				unsigned short lPort;
				CInterface_SvrTcp::getObj().GetRemoteAddr(hSocket,lnIP,lPort);
				lRet = CCunstomSvrLoginInterface::getObj().IsValidSlaveUser(lpLogin->msUserName,
					lpLogin->msPassWord,
					CTools_Ansi::ConvertdwIP(lnIP),nUserID);
				if(CF_ERROR_SUCCESS != lRet)
				{
					//发送错误回传
					SendRspError(hSocket,
						CMDID_Login_Rsp,
						PkgHead.seq,
						lRet,
						lRet,
						0,0,0);				
					break;
				}	
	
			}
			else
			{
				//判断密码
				lRet = CInterface_SvrUserOrg::getObj().IsValidUser(lpLogin->msUserName,
					lpLogin->msPassWord,
					lpLogin->meUserType,nUserID);
				if(CF_ERROR_SUCCESS != lRet)
				{
					//发送错误回传
					SendRspError(hSocket,
						CMDID_Login_Rsp,
						PkgHead.seq,
						lRet,
						lRet,
						0,0,0);				
					break;
				}
			}
		
			//判断状态和用户数据可用
			lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Login,
				AskOperationTypeAdd,nUserID,(const char*)nUserID);
			if(lRet != CF_ERROR_SUCCESS)			
			{
				//发送错误回传
				SendRspError(hSocket,
					CMDID_Login_Rsp,
					PkgHead.seq,
					lRet,
					lRet,
					0,0,0);
				break;
			}

			std::vector<Privilege> lvecPrivilege;
			CInterface_SvrUserOrg::getObj().GetPrivilegesByUserID(nUserID,lvecPrivilege);
			int nFrontID,nSessionID;
			CCunstomSvrLoginInterface::getObj().GenerateUniqueFrontAndSessionID(nFrontID,nSessionID);


			int nPackSize = sizeof(sLoginRsp) + lvecPrivilege.size()*sizeof(Privilege);
			sLoginRsp* lLoginRsp = (sLoginRsp*)malloc(nPackSize);
			memset(lLoginRsp,0,nPackSize);
			lLoginRsp->mnFrontID = nFrontID;
			lLoginRsp->mnSessionID = nSessionID;
			lLoginRsp->mnMinOrderRef = 1;
			lLoginRsp->mnUserID = nUserID;
			lLoginRsp->mnPrivilegeLen = lvecPrivilege.size();
			if(lvecPrivilege.size() != 0)
				memcpy(&lLoginRsp->mnPemission,(const void*)&lvecPrivilege[0],lvecPrivilege.size()*sizeof(Privilege));
			//构建socket info 存储在map中
			sSocketInfo ls(nUserID,lpLogin->meUserType,lpLogin->msUserName,nFrontID,nSessionID,1,hSocket);			

			//构造rsp包
			g_mutex.lock();
			CCunstomSvrLoginInterface::getObj().InsertSocketInfo(ls);
			g_mutex.unlock();			

			//发送回传
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_Login_Rsp,
				lLoginRsp,nPackSize,PkgHead.seq,0,CF_ERROR_SUCCESS);			

            CCunstomSvrLoginInterface::getObj().AddOnlineUser(nUserID);

			//产生一个用户登陆的事件
			Stru_NotifyEvent lsEvent(EventTypeUserLogin,nUserID);
			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
		
			//free memory
			free(lLoginRsp);
			
			if(CCunstomSvrLoginInterface::getObj().IsSlaver())
			{
				//通知主服务			
				CInterface_SvrTcp::getObj().SendPkgData(gMasterServerSocket,CMDID_AddUserLoginStatusToMaster_Req,
					NULL,0,0,0,nUserID);		
			}
			else
			{
				//通知所有从服务
				std::vector<SOCKET> lvecSocket;
				CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
				for(int i = 0; i < lvecSocket.size(); i++)
				{				
					CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_AddUserLoginStatusToSlave_Req,
						NULL,0,0,0,nUserID);
				}
			}
			break;
		}
		case  CMDID_Logoff_Req:
		{
			LOGIN_LOG("CMDID_Logoff_Req ");    

			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,CMDID_Logoff_Rsp,PkgHead.seq);

			//构造rsp包
			bool lbFind = false;

			int nUserID = 0;
			lbFind = CCunstomSvrLoginInterface::getObj().RemoveSocket(hSocket,nUserID);	
            CCunstomSvrLoginInterface::getObj().RemoveOnlineUser(nUserID);	  		

			//发送回传
			if(lbFind)
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_Logoff_Rsp,
					NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
                

				//产生一个用户登出的事件
				Stru_NotifyEvent ls(EventTypeUserLogoff,nUserID);
				CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(ls);
				
				if(CCunstomSvrLoginInterface::getObj().IsSlaver())
				{
					//通知主服务			
					CInterface_SvrTcp::getObj().SendPkgData(gMasterServerSocket,CMDID_RemoveUserLoginStatusToMaster_Req,
						&hSocket,sizeof(int),0,0,nUserID);		
				}
				else
				{
					//通知所有从服务
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{				
						CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_RemoveUserLoginStatusToSlave_Req,
							&hSocket,sizeof(int),0,0,nUserID);
					}
				}				
			}
			else
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_LOGIN_SOCKET_NOT_EXIST);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_Logoff_Rsp,
					(void*)lErrorString,strlen(lErrorString)+1,PkgHead.seq,0,CF_ERROR_LOGIN_SOCKET_NOT_EXIST);

			}
			break;
		}
		case CMDID_SvrTcpDisconn:
		{
            LOGIN_LOG("CMDID_SvrTcpDisconn ");    

			int nUserID = 0;
			CCunstomSvrLoginInterface::getObj().RemoveSocket(hSocket,nUserID);
			CCunstomSvrLoginInterface::getObj().RemoveOnlineUser(nUserID);	
            
            //产生一个用户退出的事件
			Stru_NotifyEvent ls(EventTypeUserDiscon,nUserID);
			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(ls);

			if(CCunstomSvrLoginInterface::getObj().IsSlaver())
			{
				//通知主服务			
				CInterface_SvrTcp::getObj().SendPkgData(gMasterServerSocket,CMDID_RemoveUserLoginStatusToMaster_Req,
					NULL,0,0,0,nUserID);		
			}
			else
			{
                //??只有作为Master服务端才能响应此命令字，而且只是主从断开TCP（策略从服务会收到CMDID_TcpClientDisconn去处理，所有就不用再通知从服务了），并且发送CMDID_RemoveUserLoginStatusToSlave_Req，不太合适
				//通知所有从服务 
				//std::vector<SOCKET> lvecSocket;
				//CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
				//for(int i = 0; i < lvecSocket.size(); i++)
				//{				
				//	CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_RemoveUserLoginStatusToSlave_Req,
				//		&hSocket,sizeof(int),0,0,nUserID);
				//}
			}

			break;
		}
		
		case CMDID_SubscriberAllUserLoginStatus_Req:
		{
			int nSocket = PkgHead.userdata1;
			CCunstomSvrLoginInterface::getObj().AddSubscribSocket(hSocket);

			std::vector<sUserOnlineInfo> lvec;
			CCunstomSvrLoginInterface::getObj().GetAllOnlineUserStatus(lvec);
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_SubscriberAllUserLoginStatus_Rsp,
				lvec.size() == 0 ? NULL :&lvec[0],
				sizeof(sUserOnlineInfo)*lvec.size(),
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);
			break;
		}
		case CMDID_UnsubscriberAllUserLoginStatus_Req:
		{
			int nSocket = PkgHead.userdata1;
			CCunstomSvrLoginInterface::getObj().RemoveSubscribSocket(nSocket);

			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_UnsubscriberAllUserLoginStatus_Req,
				NULL,
				0,
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);
			break;
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Only in master mode
		case CMDID_QueryMasterAllUserLoginStatus_Req:
		{
			LOGIN_LOG("Master Server revice a query all online user status Request");

			std::vector<sUserOnlineInfo> lvec;
			CCunstomSvrLoginInterface::getObj().GetAllOnlineUserStatus(lvec);
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_QueryMasterAllUserLoginStatus_Rsp,
				lvec.size() == 0 ? NULL :&lvec[0],
				sizeof(sUserOnlineInfo)*lvec.size(),
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);

			break;
		}
		case CMDID_AddUserLoginStatusToMaster_Req:
		{
			int nUserID = PkgHead.userdata1;
            sSocketInfo* sInfo = (sSocketInfo*) pPkgData;

			LOGIN_LOG("Master Server revice a add online user Request %d",nUserID);
            CCunstomSvrLoginInterface::getObj().InsertSocketInfo(*sInfo);
            CCunstomSvrLoginInterface::getObj().InsertSocketInfo2(hSocket,nUserID);
			CCunstomSvrLoginInterface::getObj().AddOnlineUser(nUserID);
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_AddUserLoginStatusToMaster_Rsp,
				NULL,
				0,
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);
			break;
		}
		case CMDID_RemoveUserLoginStatusToMaster_Req:
		{
			int nUserID = PkgHead.userdata1;
            int *nSlaverSocket = (int*) pPkgData;

			LOGIN_LOG("Master Server revice a remove online user Request %d",nUserID);            
            CCunstomSvrLoginInterface::getObj().RemoveSocket(*nSlaverSocket,nUserID);
			CCunstomSvrLoginInterface::getObj().RemoveSocket2(hSocket,nUserID);	
			CCunstomSvrLoginInterface::getObj().RemoveOnlineUser(nUserID);
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_RemoveUserLoginStatusToMaster_Rsp,
				NULL,
				0,
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);

            //产生一个用户登出的事件， 主服务（master）端没有必要关心从策略服务器（slaver）端登出的交易员事件
            //Stru_NotifyEvent ls(EventTypeUserLogoff,nUserID);
            //CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(ls);
			break;
		}
		case CMDID_AddUserLoginStatusToSlave_Rsp:
		{
			LOGIN_LOG("Master Server notify slaver add a user online response");
			break;
		}
		case CMDID_RemoveUserLoginStatusToSlave_Rsp:
		{
			LOGIN_LOG("Master Server notify slaver remove a user online response");
			break;
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//only in Slave Mode Case
		case CMDID_TcpClientConnect:
		{
			LOGIN_LOG("CMDID_TcpClientConnect   Connect to Master Server");
			gMasterServerSocket = hSocket;
			//发登录主服务器
			sLoginReq sLogin;
			std::string lsUser,lsPass;
			CCunstomSvrLoginInterface::getObj().GetUserAndPass(lsUser,lsPass);
			strcpy(sLogin.msUserName, lsUser.c_str());
			strcpy(sLogin.msPassWord, lsPass.c_str());			
			sLogin.meUserType = USER_TYPE_SLAVESERVER;

			//CInterface_SvrTcp::getObj().SendPkgData(hSocket,
			//	CMDID_Login_Req,
			//	&sLogin,
			//	sizeof(sLogin),
			//	PkgHead.seq,
			//	0,
			//	PkgHead.userdata1,
			//	PkgHead.userdata2,
			//	CF_ERROR_SUCCESS,0);
			//LOGIN_LOG("Send Login Pakage to Master Server");
			//设置一个8s的定时器，持续登录
			gLoginTimer = SetTimer(NULL,NULL,10000,TIMERPROC(NULL));
			break;
		}
		case CMDID_TcpClientDisconn:
		{
            LOGIN_LOG("CMDID_TcpClientDisconn   Disconnect from Master Server");
            
            gMasterServerSocket = 0;
            if(gLoginTimer != 0)
            {
                KillTimer(NULL,gLoginTimer);
                gLoginTimer = 0;
            }


            //策略从服务登录主服务器时，策略从服务是以USER_TYPE_SLAVESERVER、SLAVE_USER_ID角色登录的
            int nUserID = SLAVE_USER_ID;
            CCunstomSvrLoginInterface::getObj().RemoveSocket(hSocket,nUserID);
            CCunstomSvrLoginInterface::getObj().RemoveOnlineUser(nUserID);
            //CInterface_SvrTcp::getObj().SendPkgData(hSocket,
            //    CMDID_RemoveUserLoginStatusToSlave_Rsp,
            //    NULL,
            //    0,
            //    PkgHead.seq,
            //    0,
            //    PkgHead.userdata1,
            //    PkgHead.userdata2,
            //    CF_ERROR_SUCCESS,0);
			
			//关掉所有的客户端
			CCunstomSvrLoginInterface::getObj().CloseAllTraderSocket();
			break;
		}
		case CMDID_Login_Rsp:
		{
			//如登录成功，关掉定时器
			//发送一个登录成功事件
			//产生一个用户登陆的事件
			if(PkgHead.userdata1 == CF_ERROR_SUCCESS)
			{
				LOGIN_LOG("CMDID_Login_Rsp      Login to Master Server successfully");
				if(gLoginTimer != 0)
					KillTimer(NULL,gLoginTimer);
				//同步所有在线用户
				LOGIN_LOG("Send query all user status to master ");
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					CMDID_QueryMasterAllUserLoginStatus_Req,
					NULL,
					0,
					PkgHead.seq,
					0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS,0);

				Stru_NotifyEvent lsEvent(EventTypeSlaverLoginedMaster,hSocket);
				CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
			}
			else
			{
				const char * lsError = FormatErrorCode(PkgHead.userdata1);
				LOGIN_LOG("Login to Master Server failed: %s",lsError);
			}

			break;
		}
		case CMDID_QueryMasterAllUserLoginStatus_Rsp:
		{		
            LOGIN_LOG("CMDID_QueryMasterAllUserLoginStatus_Rsp     Slave Server Request query all online user status      ");

			sUserOnlineInfo* npUserID = (sUserOnlineInfo*) pPkgData;
			for(int i = 0;npUserID && i < PkgHead.len/sizeof(sUserOnlineInfo);i++, npUserID++)
			{				
#ifdef _DEBUG
                LOGIN_LOG("nUserID=%d nCount=%d",npUserID->mUserID,npUserID->mUserOnlineCount);
#endif
				CCunstomSvrLoginInterface::getObj().InitAddOnlineUser(npUserID->mUserID,npUserID->mUserOnlineCount);
			}
			break;
		}
	
		case CMDID_AddUserLoginStatusToMaster_Rsp:
		{	
			LOGIN_LOG("Slave Server notify master add a user online response");	
			break;
		}
		case CMDID_RemoveUserLoginStatusToMaster_Rsp:
		{
			LOGIN_LOG("Slave Server notify master remove a user online response");
			break;
		}
		case CMDID_AddUserLoginStatusToSlave_Req:
		{		
			int nUserID = PkgHead.userdata1;	
            sSocketInfo* sInfo = (sSocketInfo*) pPkgData;
			LOGIN_LOG("CMDID_AddUserLoginStatusToSlave_Req      Slave Server revice a add online user Request %d",nUserID);
            CCunstomSvrLoginInterface::getObj().InsertSocketInfo(*sInfo);
			CCunstomSvrLoginInterface::getObj().AddOnlineUser(nUserID);
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_AddUserLoginStatusToSlave_Rsp,
				NULL,
				0,
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);

            //主服务端登上交易员（策略员）后要产生一个广播用户登陆的事件		
            Stru_NotifyEvent lsEvent(EventTypeUserLogin,nUserID);
            CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

			break;
		}
		case CMDID_RemoveUserLoginStatusToSlave_Req:
		{		
			int nUserID = PkgHead.userdata1;
            int *nServerSocket = (int*)pPkgData;

			LOGIN_LOG("CMDID_RemoveUserLoginStatusToSlave_Req       Slave Server revice a remove online user Request %d",nUserID);
            CCunstomSvrLoginInterface::getObj().RemoveSocket(*nServerSocket,nUserID);
			CCunstomSvrLoginInterface::getObj().RemoveOnlineUser(nUserID);
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				CMDID_RemoveUserLoginStatusToSlave_Rsp,
				NULL,
				0,
				PkgHead.seq,
				0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS,0);

			//产生一个用户退出的事件
			Stru_NotifyEvent ls(EventTypeUserDiscon,nUserID);
			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(ls);

			break;
		}
		case CMDID_NotifySendToSlave_Req:
		{
			sErrorInfo * lError =(sErrorInfo*)pPkgData;
			if(lError != NULL)
			{
				if(lError->mnSendBit & eSendManager)
				{
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_MANAGE,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{
						//发送非自己的回传
						if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
								lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}
				}
				if(lError->mnSendBit & eSendRisk)
				{
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_RISK_CONTROL,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{
						/*int nUserID = -1;
						int nFrond,nSessionID;
						CCunstomSvrLoginInterface::getObj().GetSocketUserIDAndFrontSessionID(lError->mhSocket,nUserID,nFrond,nSessionID);
						CInterface_SvrUserOrg::getObj().*/
						//发送非自己的回传
						if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
							lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}
				}
				if(lError->mnSendBit & eSendPresentation)
				{
					std::vector<SOCKET> lvecSocket;
					CCunstomSvrLoginInterface::getObj().GetUserTypeSocket(USER_TYPE_VIEWER,lvecSocket);
					for(int i = 0; i < lvecSocket.size(); i++)
					{
						//发送非自己的回传
						if(lvecSocket[i] != lError->mhSocket)						
							CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],CMDID_GetALLError_Push,
							lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
					}
				}
				if(lError->mnSendBit & eSendSpecialUserClient)
				{
					for(int i = 0; i < MAX_SEND_SPECIAL_USER_SIZE; i++)
					{
						if(lError->mnSpecialUserIDArray[i] == 0)
							break;
						else
						{
							std::vector<SOCKET> lvecSocket;
							CCunstomSvrLoginInterface::getObj().GetUserSockets(lError->mnSpecialUserIDArray[i],lvecSocket);
							for(int j = 0; j < lvecSocket.size(); j++)
							{									
								CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[j],CMDID_GetALLError_Push,
									lError,sizeof(sErrorInfo),0,0,CF_ERROR_SUCCESS,0,CF_ERROR_SUCCESS);
							}
						}
					}
				}
			
			}
			break;
		}
		default:break;
	}
}