// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrBrokerInfo.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "..\\OpenSource\\tinyxml\\tinyxml.h"
#include "Interface_SvrBrokerInfo.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "SvrBrokerInfoImpl.h"
#include "MyXML.h"


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

#define WRITELOGID_SvrBrokerInfo




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);

//处理一个接收到的事件包
void ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);
//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//extend
set<int> g_setServerSocket; 
eServerRunMode g_bServerRunMode = ServerRunMode_Master;


//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRBROKERINFO_API void InitFunc(void)
{
    //读取Master/Slave
	bool mbSlave = CMyXML("InstitutionalTrading.xml").GetNodeContentbool2(false,"root","SlaveServer");
    g_bServerRunMode = mbSlave?ServerRunMode_Slave:ServerRunMode_Master;

	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	CInterface_SvrBrokerInfo::getObj();

    CSvrBrokerInfoImpl::getObj().SetServerRunMode(g_bServerRunMode);
    if(ServerRunMode_Master == g_bServerRunMode)
    {
        //从数据库中加载数据
        CSvrBrokerInfoImpl::getObj().LoadDB();
    }
}

//模块结束，释放资源，关闭工作线程
SVRBROKERINFO_API void ReleaseFunc(void)
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

        CSvrBrokerInfoImpl::deleteObj();
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
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddBrokerInfo_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyBrokerInfo_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelBrokerInfo_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryBrokerInfo_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddServerGroup_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyServerGroup_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelServerGroup_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryServerGroup_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddServerAddress_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyServerAddress_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelServerAddress_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryServerAddress_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyBrokerInfoQuot_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddServerAddressQuot_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyServerAddressQuot_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelServerAddressQuot_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryServerAddressQuot_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddTradeAccount_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyTradeAccount_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelTradeAccount_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryTradeAccount_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryTradeAccountAndOrgRelation_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveTradeAccountAndOrgRelation_Req,g_idThread);

    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Broker,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrBrokerInfo::getObj()));
    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_ServerGroup,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrBrokerInfo::getObj()));
    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_ServerAddress,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrBrokerInfo::getObj()));

    if(ServerRunMode_Master == g_bServerRunMode)
    {
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryBrokerModule_Req,g_idThread);
    }
    else if(ServerRunMode_Slave == g_bServerRunMode)
    {
        CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeSlaverLoginedMaster ,  g_idThread); 
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryBrokerModule_Rsp,g_idThread);
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
            if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,&ls,&nLen,nLen,NULL))
            {			
                //调用数据包处理函数处理数据包
                ParseDataChangedEvent(ls);			
                //释放EventParam
                CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
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
	}
	return 0;
}

// 解析一个数据通知事件
void ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
    switch(dataEvt.meEventType) 
    {
    case  EventTypeSlaverLoginedMaster:
        {
            int nSocket = dataEvt.mnDataID;
            CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_QueryBrokerModule_Req,NULL,0,dataEvt.mnValidateLen);
            break;
        }

    default: break;
    };
}

//处理一个SvrTcp推送过来的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	下面根据数据包的命令字，处理数据包
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{
    case Cmd_QueryBrokerModule_Req:
        {
            if(ServerRunMode_Slave == g_bServerRunMode)
                break;

            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryBrokerModule_Rsp,PkgHead.seq);

            int nDataLen = 0;
            char* pData = CSvrBrokerInfoImpl::getObj().GetData(nDataLen);
            if (pData)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryBrokerModule_Rsp,
                    pData,nDataLen,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryBrokerModule_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            if(pData)
            {
                delete pData;
                pData = NULL;
            }           

            g_setServerSocket.insert(hSocket);

            break;
        }
    case Cmd_QueryBrokerModule_Rsp:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
                break;

            char* pBuffer = (char*)pPkgData;
            if(!pBuffer || PkgHead.len==0)
                break;

            std::map<int,TradeAccount> mapTradeAccount;
            std::map<int,BrokerInfo> mapBrokerInfo;
            std::map<int,ServerGroup> mapServerGroup;
            std::map<int,ServerAddress> mapServerAddr;
            std::map<int,ServerAddressQuot> mapServerAddrQuot;  
            std::map<int,TradeAccountAndOrg> mapTradeAccountAndOrg;  

            int nTradeAccountSize   = *(int*)(pBuffer); 
            int nBrokerInfoSize     = *(int*)(pBuffer+1*sizeof(int));
            int nServerGroupSize    = *(int*)(pBuffer+2*sizeof(int));
            int nServerAddrSize     = *(int*)(pBuffer+3*sizeof(int));
            int nServerAddrQuotSize = *(int*)(pBuffer+4*sizeof(int));
            int nTradeAccountAndOrgRelationSize = *(int*)(pBuffer+5*sizeof(int));


            int i;
            int nSize = 0;
            for ( i = 0; i < nTradeAccountSize; i++ )
            {
                TradeAccount* pTradeAccount = (TradeAccount*)(pBuffer+6*sizeof(int)+i*sizeof(TradeAccount));
                mapTradeAccount[pTradeAccount->nTradeAccountID] = *pTradeAccount;
            }
            nSize += nTradeAccountSize*sizeof(TradeAccount);

            for ( i = 0; i < nBrokerInfoSize; i++ )
            {
                BrokerInfo* pBrokerInfo = (BrokerInfo*)(pBuffer+6*sizeof(int)+nSize+i*sizeof(BrokerInfo));
                mapBrokerInfo[pBrokerInfo->nBrokerID] = *pBrokerInfo;
            }
            nSize += nBrokerInfoSize*sizeof(BrokerInfo);

            for ( i = 0; i < nServerGroupSize; i++ )
            {
                ServerGroup* pServerGroup = (ServerGroup*)(pBuffer+6*sizeof(int)+nSize+i*sizeof(ServerGroup));
                mapServerGroup[pServerGroup->nServerID] = *pServerGroup;
            }
            nSize += nServerGroupSize*sizeof(ServerGroup);

            for ( i = 0; i < nServerAddrSize; i++ )
            {
                ServerAddress* pServerAddress = (ServerAddress*)(pBuffer+6*sizeof(int)+nSize+i*sizeof(ServerAddress));
                mapServerAddr[pServerAddress->nAddrID] = *pServerAddress;
            }
            nSize += nServerAddrSize*sizeof(ServerAddress);

            for ( i = 0; i < nServerAddrQuotSize; i++ )
            {
                ServerAddressQuot* pServerAddressQuot = (ServerAddressQuot*)(pBuffer+6*sizeof(int)+nSize+i*sizeof(ServerAddressQuot));
                mapServerAddrQuot[pServerAddressQuot->nAddrID] = *pServerAddressQuot;
            }

            nSize += nServerAddrQuotSize*sizeof(ServerAddressQuot);

            for ( i = 0; i < nTradeAccountAndOrgRelationSize; i++ )
            {
                TradeAccountAndOrg* pTradeAccountAndOrg = (TradeAccountAndOrg*)(pBuffer+6*sizeof(int)+nSize+i*sizeof(TradeAccountAndOrg));
                mapTradeAccountAndOrg[pTradeAccountAndOrg->nTradeAccountID] = *pTradeAccountAndOrg;
            }

            CSvrBrokerInfoImpl::getObj().SetData(   mapTradeAccount, 
                                                    mapBrokerInfo, 
                                                    mapServerGroup, 
                                                    mapServerAddr, 
                                                    mapServerAddrQuot, 
                                                    mapTradeAccountAndOrg ); 

            break;
        }
	case Cmd_AddBrokerInfo_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(BrokerInfo),hSocket,Cmd_AddBrokerInfo_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,BROKER_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddBrokerInfo_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                BrokerInfo info = *(BrokerInfo*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrBrokerInfoImpl::getObj().AddBrokerInfo(PkgHead.userdata1, info, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddBrokerInfo_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);	
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddBrokerInfo_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeBrokerInfoAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddBrokerInfo_Req, &info, sizeof(BrokerInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                BrokerInfo info = *(BrokerInfo*)pPkgData;
                int nPKID = 0;
                CSvrBrokerInfoImpl::getObj().AddBrokerInfo(PkgHead.userdata1, info, nPKID);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                    hSocket,
                    eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeBrokerInfoAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
    case Cmd_ModifyBrokerInfo_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(BrokerInfo),hSocket,Cmd_ModifyBrokerInfo_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,BROKER_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyBrokerInfo_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                BrokerInfo info = *(BrokerInfo*)pPkgData;

                //问询服务器状态
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Broker,
                    AskOperationTypeUpdate,
                    info.nBrokerID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_ModifyBrokerInfo_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrBrokerInfoImpl::getObj().ModifyBrokerInfo(PkgHead.userdata1, info);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyBrokerInfo_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyBrokerInfo_Rsp,
                        &(info.nBrokerID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeBrokerInfoUpdate,info.nBrokerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyBrokerInfo_Req, &info, sizeof(BrokerInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                BrokerInfo info = *(BrokerInfo*)pPkgData;
                CSvrBrokerInfoImpl::getObj().ModifyBrokerInfo(PkgHead.userdata1, info);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeBrokerInfoUpdate,info.nBrokerID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

            }

            break;
		}
    case Cmd_DelBrokerInfo_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelBrokerInfo_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,BROKER_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelBrokerInfo_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nBrokerID = *(int*)pPkgData;

                //问询服务器状态
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Broker,
                    AskOperationTypeDelete,
                    nBrokerID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_DelBrokerInfo_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }


                int nErrorCode = CSvrBrokerInfoImpl::getObj().DelBrokerInfo(PkgHead.userdata1, nBrokerID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelBrokerInfo_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelBrokerInfo_Rsp,
                        &nBrokerID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeBrokerInfoDelete,nBrokerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelBrokerInfo_Req, &nBrokerID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nBrokerID = *(int*)pPkgData;
                CSvrBrokerInfoImpl::getObj().DelBrokerInfo(PkgHead.userdata1, nBrokerID);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeBrokerInfoDelete,nBrokerID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
	case Cmd_QueryBrokerInfo_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryBrokerInfo_Rsp,PkgHead.seq);

			std::vector<BrokerInfo> vec;
			CSvrBrokerInfoImpl::getObj().GetBrokerInfos(vec);
			if (!vec.empty())
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryBrokerInfo_Rsp,
					&vec[0],sizeof(BrokerInfo)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryBrokerInfo_Rsp,
					NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			}

			break;
		}

	case Cmd_AddServerGroup_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(ServerGroup),hSocket,Cmd_AddServerGroup_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVER_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddServerGroup_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                ServerGroup info = *(ServerGroup*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrBrokerInfoImpl::getObj().AddServerGroup(PkgHead.userdata1, info, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddServerGroup_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddServerGroup_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERGROUP),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerGroupAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddServerGroup_Req, &info, sizeof(ServerGroup), PkgHead.seq);
                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                ServerGroup info = *(ServerGroup*)pPkgData;
                int nPKID = 0;
                CSvrBrokerInfoImpl::getObj().AddServerGroup(PkgHead.userdata1, info, nPKID);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERGROUP),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerGroupAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
    case Cmd_ModifyServerGroup_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(ServerGroup),hSocket,Cmd_ModifyServerGroup_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVER_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyServerGroup_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                ServerGroup info = *(ServerGroup*)pPkgData;

                //问询服务器状态
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_ServerGroup,
                    AskOperationTypeUpdate,
                    info.nServerID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_ModifyServerGroup_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrBrokerInfoImpl::getObj().ModifyServerGroup(PkgHead.userdata1, info);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyServerGroup_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyServerGroup_Rsp,
                        &(info.nServerID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERGROUP),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerGroupUpdate,info.nServerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyServerGroup_Req, &info, sizeof(ServerGroup), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                ServerGroup info = *(ServerGroup*)pPkgData;
                CSvrBrokerInfoImpl::getObj().ModifyServerGroup(PkgHead.userdata1, info);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERGROUP),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerGroupUpdate,info.nServerID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

            }

			break;
		}
	case Cmd_DelServerGroup_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelServerGroup_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVER_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelServerGroup_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nServerID = *(int*)pPkgData;
                //问询是否在用
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_ServerGroup,
                    AskOperationTypeDelete,
                    nServerID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_DelServerGroup_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }


                int nErrorCode = CSvrBrokerInfoImpl::getObj().DelServerGroup(PkgHead.userdata1, nServerID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelServerGroup_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelServerGroup_Rsp,
                        &nServerID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERGROUP),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerGroupDelete,nServerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelServerGroup_Req, &nServerID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nServerID = *(int*)pPkgData;
                CSvrBrokerInfoImpl::getObj().DelServerGroup(PkgHead.userdata1, nServerID);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERGROUP),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerGroupDelete,nServerID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
    case Cmd_QueryServerGroup_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryServerGroup_Rsp,PkgHead.seq);

			std::vector<ServerGroup> vec;
			CSvrBrokerInfoImpl::getObj().GetServerGroups(vec);
			if (!vec.empty())
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryServerGroup_Rsp,
					&vec[0],sizeof(ServerGroup)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryServerGroup_Rsp,
					NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			}

			break;
		}

	case Cmd_AddServerAddress_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(ServerAddress),hSocket,Cmd_AddServerAddress_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVERADDR_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddServerAddress_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                ServerAddress info = *(ServerAddress*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrBrokerInfoImpl::getObj().AddServerAddr(PkgHead.userdata1, info, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddServerAddress_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddServerAddress_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerAddressAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddServerAddress_Req, &info, sizeof(ServerAddress), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                ServerAddress info = *(ServerAddress*)pPkgData;
                int nPKID = 0;
                CSvrBrokerInfoImpl::getObj().AddServerAddr(PkgHead.userdata1, info, nPKID);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerAddressAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
		}
	case Cmd_ModifyServerAddress_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(ServerAddress),hSocket,Cmd_ModifyServerAddress_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVERADDR_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyServerAddress_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                ServerAddress info = *(ServerAddress*)pPkgData;

                //问询是否在用
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_ServerAddress,
                    AskOperationTypeUpdate,
                    info.nAddrID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_ModifyServerAddress_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrBrokerInfoImpl::getObj().ModifyServerAddr(PkgHead.userdata1, info);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyServerAddress_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyServerAddress_Rsp,
                        &(info.nAddrID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerAddressUpdate,info.nAddrID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyServerAddress_Req, &info, sizeof(ServerAddress), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                ServerAddress info = *(ServerAddress*)pPkgData;
                CSvrBrokerInfoImpl::getObj().ModifyServerAddr(PkgHead.userdata1, info);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerAddressUpdate,info.nAddrID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
	case Cmd_DelServerAddress_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelServerAddress_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVERADDR_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelServerAddress_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nServerAddrID = *(int*)pPkgData;
                //问询是否在用
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_ServerAddress,
                    AskOperationTypeDelete,
                    nServerAddrID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_DelServerAddress_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrBrokerInfoImpl::getObj().DelServerAddr(PkgHead.userdata1, nServerAddrID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelServerAddress_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelServerAddress_Rsp,
                        &nServerAddrID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerAddressDelete,nServerAddrID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelServerAddress_Req, &nServerAddrID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nServerAddrID = *(int*)pPkgData;
                CSvrBrokerInfoImpl::getObj().DelServerAddr(PkgHead.userdata1, nServerAddrID);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerAddressDelete,nServerAddrID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
    case Cmd_QueryServerAddress_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryServerAddress_Rsp,PkgHead.seq);

            std::vector<ServerAddress> vec;
            CSvrBrokerInfoImpl::getObj().GetServerAddr(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryServerAddress_Rsp,
                    &vec[0],sizeof(ServerAddress)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryServerAddress_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }

    case Cmd_ModifyBrokerInfoQuot_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(BrokerInfo),hSocket,Cmd_ModifyBrokerInfoQuot_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,BROKER_QUOT_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyBrokerInfoQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                BrokerInfo info = *(BrokerInfo*)pPkgData;

                int nErrorCode = CSvrBrokerInfoImpl::getObj().ModifyBrokerInfoQuot(PkgHead.userdata1, info);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyBrokerInfoQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyBrokerInfoQuot_Rsp,
                        &(info.nBrokerID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeBrokerInfoQuotUpdate,info.nBrokerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyBrokerInfoQuot_Req, &info, sizeof(BrokerInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                BrokerInfo info = *(BrokerInfo*)pPkgData;
                CSvrBrokerInfoImpl::getObj().ModifyBrokerInfoQuot(PkgHead.userdata1, info);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_BROKER),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeBrokerInfoQuotUpdate,info.nBrokerID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

            }

            break;
        }
    case Cmd_AddServerAddressQuot_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(ServerAddressQuot),hSocket,Cmd_AddServerAddressQuot_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVERADDR_QUOT_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddServerAddressQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                ServerAddressQuot info = *(ServerAddressQuot*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrBrokerInfoImpl::getObj().AddServerAddrQuot(PkgHead.userdata1, info, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddServerAddressQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddServerAddressQuot_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerAddressQuotAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddServerAddressQuot_Req, &info, sizeof(ServerAddressQuot), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                ServerAddressQuot info = *(ServerAddressQuot*)pPkgData;
                int nPKID = 0;
                CSvrBrokerInfoImpl::getObj().AddServerAddrQuot(PkgHead.userdata1, info, nPKID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerAddressQuotAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyServerAddressQuot_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(ServerAddressQuot),hSocket,Cmd_ModifyServerAddressQuot_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVERADDR_QUOT_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyServerAddressQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                ServerAddressQuot info = *(ServerAddressQuot*)pPkgData;

                int nErrorCode = CSvrBrokerInfoImpl::getObj().ModifyServerAddrQuot(PkgHead.userdata1, info);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyServerAddressQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyServerAddressQuot_Rsp,
                        &(info.nAddrID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeServerAddressQuotUpdate,info.nAddrID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyServerAddressQuot_Req, &info, sizeof(ServerAddressQuot), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                ServerAddressQuot info = *(ServerAddressQuot*)pPkgData;
                CSvrBrokerInfoImpl::getObj().ModifyServerAddrQuot(PkgHead.userdata1, info);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeServerAddressQuotUpdate,info.nAddrID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_DelServerAddressQuot_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelServerAddressQuot_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,SERVERADDR_QUOT_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelServerAddressQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nServerAddrID = *(int*)pPkgData;

                int nErrorCode = CSvrBrokerInfoImpl::getObj().DelServerAddrQuot(PkgHead.userdata1, nServerAddrID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelServerAddressQuot_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelServerAddressQuot_Rsp,
                        &nServerAddrID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeServerAddressQuotDelete,nServerAddrID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelServerAddressQuot_Req, &nServerAddrID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nServerAddrID = *(int*)pPkgData;
                CSvrBrokerInfoImpl::getObj().DelServerAddrQuot(PkgHead.userdata1, nServerAddrID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_SERVERADDR),
                    hSocket,
                    eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeServerAddressQuotDelete,nServerAddrID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryServerAddressQuot_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryServerAddressQuot_Rsp,PkgHead.seq);

            std::vector<ServerAddressQuot> vec;
            CSvrBrokerInfoImpl::getObj().GetServerAddrQuot(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryServerAddressQuot_Rsp,
                    &vec[0],sizeof(ServerAddressQuot)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryServerAddressQuot_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }


	case Cmd_AddTradeAccount_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeAccount),hSocket,Cmd_AddTradeAccount_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ACCOUNT_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddTradeAccount_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                TradeAccount info = *(TradeAccount*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrBrokerInfoImpl::getObj().AddTradeAccount(PkgHead.userdata1, info, nPKID);            
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddTradeAccount_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddTradeAccount_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeAccountAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddTradeAccount_Req, &info, sizeof(TradeAccount), PkgHead.seq);
                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                TradeAccount info = *(TradeAccount*)pPkgData;
                int nPKID = 0;
                CSvrBrokerInfoImpl::getObj().AddTradeAccount(PkgHead.userdata1, info, nPKID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeAccountAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
	case Cmd_ModifyTradeAccount_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeAccount),hSocket,Cmd_ModifyTradeAccount_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ACCOUNT_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyTradeAccount_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                TradeAccount info = *(TradeAccount*)pPkgData;

                //只问询登录状态
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Account,
                    AskOperationTypeUpdate,
                    info.nTradeAccountID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_DelTradeAccount_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                bool bPswModify = false;
                int nErrorCode = CSvrBrokerInfoImpl::getObj().ModifyTradeAccount(PkgHead.userdata1, info,bPswModify);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyTradeAccount_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyTradeAccount_Rsp,
                        &(info.nTradeAccountID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeAccountUpdate,info.nTradeAccountID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyTradeAccount_Req, &info, sizeof(TradeAccount), PkgHead.seq);
                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                TradeAccount info = *(TradeAccount*)pPkgData;
                bool bPswModify = false;
                CSvrBrokerInfoImpl::getObj().ModifyTradeAccount(PkgHead.userdata1, info,bPswModify);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeAccountUpdate,info.nTradeAccountID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
    case Cmd_DelTradeAccount_Req:
		{
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelTradeAccount_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ACCOUNT_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelTradeAccount_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nTradeAccountID = *(int*)pPkgData;

                //只问询登录状态
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Account,
                    AskOperationTypeDelete,
                    nTradeAccountID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_DelTradeAccount_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrBrokerInfoImpl::getObj().DelTradeAccount(PkgHead.userdata1, nTradeAccountID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelTradeAccount_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelTradeAccount_Rsp,
                        &nTradeAccountID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeAccountDelete,nTradeAccountID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelTradeAccount_Req, &nTradeAccountID, sizeof(int), PkgHead.seq);
                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nTradeAccountID = *(int*)pPkgData;
                CSvrBrokerInfoImpl::getObj().DelTradeAccount(PkgHead.userdata1, nTradeAccountID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                    hSocket,
                    eSendManager);

                Stru_NotifyEvent lsEvent(EventTypeAccountDelete,nTradeAccountID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

			break;
		}
    case Cmd_QueryTradeAccount_Req:
		{
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryTradeAccount_Rsp,PkgHead.seq);

			std::vector<TradeAccount> vec;
			CSvrBrokerInfoImpl::getObj().GetTradeAccounts(vec);
			if (!vec.empty())
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryTradeAccount_Rsp,
					&vec[0],sizeof(TradeAccount)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
			}
			else
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryTradeAccount_Rsp,
					NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			}

			break;
		}
    case Cmd_QryTradeAccountAndOrgRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryTradeAccountAndOrgRelation_Rsp,PkgHead.seq);

            std::vector<TradeAccountAndOrg> vec;
            CSvrBrokerInfoImpl::getObj().GetTradeAccountAndOrgRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryTradeAccountAndOrgRelation_Rsp,
                    &vec[0],sizeof(TradeAccountAndOrg)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryTradeAccountAndOrgRelation_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_SaveTradeAccountAndOrgRelation_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeAccountAndOrg),hSocket,Cmd_SaveTradeAccountAndOrgRelation_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ACCOUNT_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveTradeAccountAndOrgRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                TradeAccountAndOrg relation = *(TradeAccountAndOrg*)pPkgData;
                //判断是新增还是修改
                if(CSvrBrokerInfoImpl::getObj().IsUpdateTradeAccountAndOrgRelation(relation))
                {
                    //问询
                    CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Account,
                        AskOperationTypeUpdate,
                        PkgHead.userdata1,
                        NULL);
                    if(lRet != CF_ERROR_SUCCESS)			
                    {
                        //发送错误回传
                        SendRspError(hSocket,
                            Cmd_SaveTradeAccountAndOrgRelation_Rsp,
                            PkgHead.seq,
                            lRet,
                            lRet,
                            0,0,0);
                        break;
                    }
                }               

                int nErrorCode = CSvrBrokerInfoImpl::getObj().SaveTradeAccountAndOrgRelation(PkgHead.userdata1, relation);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveTradeAccountAndOrgRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveTradeAccountAndOrgRelation_Rsp,
                        &(relation),sizeof(TradeAccountAndOrg),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeAccountUpdate,relation.nTradeAccountID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveTradeAccountAndOrgRelation_Req, &relation, sizeof(TradeAccountAndOrg), PkgHead.seq);
                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                TradeAccountAndOrg relation = *(TradeAccountAndOrg*)pPkgData;
                CSvrBrokerInfoImpl::getObj().SaveTradeAccountAndOrgRelation(PkgHead.userdata1,relation);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ACCOUNT),
                    hSocket,
                    eSendRisk);

                Stru_NotifyEvent lsEvent(EventTypeAccountUpdate,relation.nTradeAccountID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
	default:
		break;
	}
}