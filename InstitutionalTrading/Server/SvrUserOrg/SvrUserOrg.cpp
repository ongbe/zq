// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrUserOrg.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "..\\OpenSource\\tinyxml\\tinyxml.h"
#include "SvrUserOrgImpl.h"
#include "CommonMacro.h"
#include "RiskManageStruct.h"
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
#pragma comment(lib, "SvrBrokerInfo.lib")


#define WRITELOGID_SvrUserOrg




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
SVRUSERORG_API void InitFunc(void)
{
    //读取Master/Slave
	bool mbSlave = CMyXML("InstitutionalTrading.xml").GetNodeContentbool2(false,"root","SlaveServer");
    g_bServerRunMode = mbSlave?ServerRunMode_Slave:ServerRunMode_Master;

	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

    CSvrUserOrgImpl::getObj().SetServerRunMode(g_bServerRunMode);
    if(ServerRunMode_Master == g_bServerRunMode)
    {
        //从数据库中加载数据
        CSvrUserOrgImpl::getObj().LoadDB();
    }
}

//模块结束，释放资源，关闭工作线程
SVRUSERORG_API void ReleaseFunc(void)
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

        CSvrUserOrgImpl::deleteObj();
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
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddOrg_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyOrg_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelOrg_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryOrg_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddUser_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelUser_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyUser_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryUser_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserStatus_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryPrivilege_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveRolePrivilegeRelation_Req,g_idThread); 
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryRolePrivilegeRelation_Req,g_idThread); 

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddRole_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DelRole_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyRole_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryRole_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveUserRoleRelation_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryUserRoleRelation_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveUserOrgRelation_FromUser_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveUserOrgRelation_FromOrg_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryUserOrgRelation_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveUserTradeAcctRelation_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryUserTradeAcctRelation_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_UnLockWindow_Req,g_idThread);

    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Orgnazation,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrUserOrg::getObj()));
    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Account,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrUserOrg::getObj()));
    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_QueryPrivilege,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrUserOrg::getObj()));

    if(ServerRunMode_Master == g_bServerRunMode)
    {
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryOrganizationModule_Req,g_idThread);
    }
    else if(ServerRunMode_Slave == g_bServerRunMode)
    {
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryOrg_Req,g_idThread);
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryUser_Req,g_idThread);
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryUserOrgRelation_Req,g_idThread);

        CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeSlaverLoginedMaster ,  g_idThread); 
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryOrganizationModule_Rsp,g_idThread);
    }

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_Master_SetLimitTrade_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_Master_SetManualVerify_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_Master_ModifyUserPassword_Req,g_idThread);


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
            CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_QueryOrganizationModule_Req,NULL,0,dataEvt.mnValidateLen);
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
    case Cmd_QueryOrganizationModule_Req:
        {
            if(ServerRunMode_Slave == g_bServerRunMode)
                break;

            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryOrganizationModule_Rsp,PkgHead.seq);

            int nDataLen = 0;
            char* pData = CSvrUserOrgImpl::getObj().GetData(nDataLen);
            if (pData)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryOrganizationModule_Rsp,
                    pData,nDataLen,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryOrganizationModule_Rsp,
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
    case Cmd_QueryOrganizationModule_Rsp:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
                break;

            char* pBuffer = (char*)pPkgData;
            if(!pBuffer || PkgHead.len==0)
                break;

            std::map<int,Organization> mapOrg;
            std::map<int,UserInfo> mapUserInfo;
            std::map<int,RoleInfo> mapRoleInfo;
            std::vector<Privilege> vecPrivilege;
            std::vector<RoleAndPrivilegeRelation> vecRoleAndPrivilegeRelation;
            std::vector<UserAndTradeAccountRelation> vecUserAndTradeAccountRelation;
            std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
            std::vector<UserRole> vecUserRoleRelation;


            int nOrgSize                         = *(int*)(pBuffer+0*sizeof(int));   
            int nUserInfoSize                    = *(int*)(pBuffer+1*sizeof(int));    
            int nRoleInfoSize                    = *(int*)(pBuffer+2*sizeof(int));    
            int nPrivilegeSize                   = *(int*)(pBuffer+3*sizeof(int));    
            int nRoleAndPrivilegeRelationSize    = *(int*)(pBuffer+4*sizeof(int));    
            int nUserAndTradeAccountRelationSize = *(int*)(pBuffer+5*sizeof(int));    
            int nUserAndOrgRelationSize          = *(int*)(pBuffer+6*sizeof(int));    
            int nUserRoleSize                    = *(int*)(pBuffer+7*sizeof(int)); 


            int i;
            int nSize = 0;
            for ( i = 0; i < nOrgSize; i++ )
            {
                Organization* pOrganization = (Organization*)(pBuffer+8*sizeof(int)+i*sizeof(Organization));
                mapOrg[pOrganization->nAssetMgmtOrgID] = *pOrganization;
            }
            nSize += nOrgSize*sizeof(Organization);

            for ( i = 0; i < nUserInfoSize; i++ )
            {        
                UserInfo* pUserInfo = (UserInfo*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(UserInfo));
                mapUserInfo[pUserInfo->nUserID] = *pUserInfo;
            }
            nSize += nUserInfoSize*sizeof(UserInfo);

            for ( i = 0; i < nRoleInfoSize; i++ )
            {        
                RoleInfo* pRoleInfo = (RoleInfo*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(RoleInfo));
                mapRoleInfo[pRoleInfo->nRoleID] = *pRoleInfo;
            }
            nSize += nRoleInfoSize*sizeof(RoleInfo);

            for ( i = 0; i < nPrivilegeSize; i++ )
            {        
                Privilege* pPrivilege = (Privilege*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(Privilege));
                vecPrivilege.push_back(*pPrivilege);
            }
            nSize += nPrivilegeSize*sizeof(Privilege);

            for ( i = 0; i < nRoleAndPrivilegeRelationSize; i++ )
            {        
                RoleAndPrivilegeRelation* pRelation = (RoleAndPrivilegeRelation*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(RoleAndPrivilegeRelation));
                vecRoleAndPrivilegeRelation.push_back(*pRelation);
            }
            nSize += nRoleAndPrivilegeRelationSize*sizeof(RoleAndPrivilegeRelation);

            for ( i = 0; i < nUserAndTradeAccountRelationSize; i++ )
            {        
                UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(UserAndTradeAccountRelation));
                vecUserAndTradeAccountRelation.push_back(*pRelation);
            }
            nSize += nUserAndTradeAccountRelationSize*sizeof(UserAndTradeAccountRelation);

            for ( i = 0; i < nUserAndOrgRelationSize; i++ )
            {        
                UserAndOrgRelation* pRelation = (UserAndOrgRelation*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(UserAndOrgRelation));
                vecUserAndOrgRelation.push_back(*pRelation);
            }
            nSize += nUserAndOrgRelationSize*sizeof(UserAndOrgRelation);

            for ( i = 0; i < nUserRoleSize; i++ )
            {        
                UserRole* pRelation = (UserRole*)(pBuffer+8*sizeof(int)+nSize+i*sizeof(UserRole));
                vecUserRoleRelation.push_back(*pRelation);
            }

            CSvrUserOrgImpl::getObj().SetData(  mapOrg, mapUserInfo, mapRoleInfo, vecPrivilege, vecRoleAndPrivilegeRelation, 
                vecUserAndTradeAccountRelation, vecUserAndOrgRelation, vecUserRoleRelation); 

            break;
        }
    case Cmd_AddOrg_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(Organization),hSocket,Cmd_AddOrg_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ORGANIZATION_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                Organization org = *(Organization*)pPkgData;

                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Orgnazation,
                    AskOperationTypeAdd,
                    org.nAssetMgmtOrgID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_AddOrg_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nPKID = 0;
                int nErrorCode = CSvrUserOrgImpl::getObj().AddOrg(PkgHead.userdata1,org, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddOrg_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ORG),
                        hSocket,
                        eSendManager|eSendPresentation|eSendSpecialUserClient);

                    Stru_NotifyEvent lsEvent(EventTypeOrgAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddOrg_Req, &org, sizeof(Organization), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                Organization org = *(Organization*)pPkgData;
                int nPKID = 0;
                CSvrUserOrgImpl::getObj().AddOrg(PkgHead.userdata1,org, nPKID);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ORG),
                    hSocket,
                    eSendManager|eSendPresentation|eSendRisk);

                Stru_NotifyEvent lsEvent(EventTypeOrgAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyOrg_Req:
        {          
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(Organization),hSocket,Cmd_ModifyOrg_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ORGANIZATION_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                Organization org = *(Organization*)pPkgData;

                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Orgnazation,
                    AskOperationTypeUpdate,
                    org.nAssetMgmtOrgID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_ModifyOrg_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrUserOrgImpl::getObj().ModifyOrg(PkgHead.userdata1,org);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyOrg_Rsp,
                        &(org.nAssetMgmtOrgID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ORG),
                        hSocket,
                        eSendManager|eSendPresentation|eSendSpecialUserClient);
                    Stru_NotifyEvent lsEvent(EventTypeOrgUpdate,org.nAssetMgmtOrgID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyOrg_Req, &org, sizeof(Organization), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                Organization org = *(Organization*)pPkgData;
                CSvrUserOrgImpl::getObj().ModifyOrg(PkgHead.userdata1,org);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ORG),
                    hSocket,
                    eSendManager|eSendPresentation|eSendSpecialUserClient);
                Stru_NotifyEvent lsEvent(EventTypeOrgUpdate,org.nAssetMgmtOrgID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

            }

            break;
        }
    case Cmd_DelOrg_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelOrg_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ORGANIZATION_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nOrgID = *(int*)pPkgData;

                //问询交易员是否在用
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Orgnazation,
                    AskOperationTypeDelete,
                    nOrgID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_DelOrg_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrUserOrgImpl::getObj().DelOrg(PkgHead.userdata1,nOrgID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelOrg_Rsp,
                        &nOrgID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ORG),
                        hSocket,
                        eSendManager|eSendPresentation|eSendSpecialUserClient);
                    Stru_NotifyEvent lsEvent(EventTypeOrgDelete,nOrgID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelOrg_Req, &nOrgID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nOrgID = *(int*)pPkgData;
                CSvrUserOrgImpl::getObj().DelOrg(PkgHead.userdata1,nOrgID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ORG),
                    hSocket,
                    eSendManager|eSendPresentation|eSendSpecialUserClient);
                Stru_NotifyEvent lsEvent(EventTypeOrgDelete,nOrgID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryOrg_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryOrg_Rsp,PkgHead.seq);

            std::vector<Organization> vec;
            CSvrUserOrgImpl::getObj().GetOrgs(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryOrg_Rsp,
                    &vec[0],sizeof(Organization)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryOrg_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_AddUser_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(UserInfo),hSocket,Cmd_AddUser_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                UserInfo userInfo = *(UserInfo*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrUserOrgImpl::getObj().AddUserInfo(PkgHead.userdata1,userInfo, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddUser_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    int nSendBit;
                    switch(userInfo.nUserType)
                    {	
                    case USER_TYPE_MANAGE:		//管理员
                    case USER_TYPE_RISK_CONTROL://风控员                        
                    case USER_TYPE_VIEWER:		//展现员
                    case USER_TYPE_INVALID:		//无效的用户类型
                        nSendBit = eSendManager;
                        break;
                    case USER_TYPE_TRADER:		//交易员
                        nSendBit = eSendManager/*|eSendSpecialUserClient*/;
                        break;                        
                    };

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        nSendBit);
                    std::vector<UserInfo> vec;
                    CSvrUserOrgImpl::getObj().GetUserInfosByOrgID(ORGNIZATION_ROOT,USER_TYPE_RISK_CONTROL,0,vec);
                    int nCount=0;
                    for (std::vector<UserInfo>::iterator it = vec.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vec.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = it->nUserID;

                    Stru_NotifyEvent lsEvent(EventTypeUserUpdate,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddUser_Req, &userInfo, sizeof(UserInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                UserInfo userInfo = *(UserInfo*)pPkgData;
                int nPKID = 0;
                CSvrUserOrgImpl::getObj().AddUserInfo(PkgHead.userdata1,userInfo, nPKID);

                int nSendBit;
                switch(userInfo.nUserType)
                {	
                case USER_TYPE_MANAGE:		//管理员
                case USER_TYPE_RISK_CONTROL://风控员                        
                case USER_TYPE_VIEWER:		//展现员
                case USER_TYPE_INVALID:		//无效的用户类型
                    nSendBit = eSendManager;
                    break;
                case USER_TYPE_TRADER:		//交易员
                    nSendBit = /*eSendManager|*/eSendSpecialUserClient;
                    break;                        
                };

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                    hSocket,
                    nSendBit);
                std::vector<UserInfo> vec;
                CSvrUserOrgImpl::getObj().GetUserInfosByOrgID(ORGNIZATION_ROOT,USER_TYPE_RISK_CONTROL,0,vec);
                int nCount=0;
                for (std::vector<UserInfo>::iterator it = vec.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vec.end();it++)
                    ssErrorInfo.mnSpecialUserIDArray[nCount++] = it->nUserID;

                Stru_NotifyEvent lsEvent(EventTypeUserUpdate,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyUser_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(UserInfo),hSocket,Cmd_ModifyUser_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                UserInfo userInfo = *(UserInfo*)pPkgData;

                int nErrorCode = CSvrUserOrgImpl::getObj().ModifyUserInfo(PkgHead.userdata1,userInfo);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyUser_Rsp,
                        &(userInfo.nUserID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeUserUpdate,userInfo.nUserID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyUser_Req, &userInfo, sizeof(UserInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                UserInfo userInfo = *(UserInfo*)pPkgData;
                CSvrUserOrgImpl::getObj().ModifyUserInfo(PkgHead.userdata1,userInfo);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                    hSocket,
                    eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeUserUpdate,userInfo.nUserID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_DelUser_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelUser_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nUserID = *(int*)pPkgData;

                int nErrorCode = CSvrUserOrgImpl::getObj().DelUserInfo(PkgHead.userdata1,nUserID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelUser_Rsp,
                        &nUserID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeUserDelete,nUserID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelUser_Req, &nUserID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nUserID = *(int*)pPkgData;
                CSvrUserOrgImpl::getObj().DelUserInfo(PkgHead.userdata1,nUserID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                    hSocket,
                    eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeUserDelete,nUserID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QryUser_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryUser_Rsp,PkgHead.seq);

            std::vector<UserInfo> vec;
            CSvrUserOrgImpl::getObj().GetUserInfos(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUser_Rsp,
                    &vec[0],sizeof(UserInfo)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUser_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_RM_Master_ModifyUserPassword_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(UserPassword),hSocket,Cmd_RM_Master_ModifyUserPassword_Rsp,PkgHead.seq);

                UserPassword userPassword = *(UserPassword*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrUserOrgImpl::getObj().ModifyUserPasswordFromRisk(userPassword, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_Master_ModifyUserPassword_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode,0,0,PkgHead.userdata4);	
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_Master_ModifyUserPassword_Rsp,
                        (void*)pPkgData, PkgHead.len,0,0,CF_ERROR_SUCCESS,0,0,PkgHead.userdata4);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeUserUpdate,userPassword.nUserID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_RM_Master_ModifyUserPassword_Req, &userPassword, sizeof(UserPassword), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                UserPassword userPassword = *(UserPassword*)pPkgData;
                int nPKID = 0;
                CSvrUserOrgImpl::getObj().ModifyUserPasswordFromRisk(userPassword, nPKID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                    hSocket,
                    eSendRisk);
                Stru_NotifyEvent lsEvent(EventTypeUserUpdate,userPassword.nUserID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_RM_UnLockWindow_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(UserPassword),hSocket,Cmd_RM_UnLockWindow_Rsp,PkgHead.seq);

            UserPassword userPassword = *(UserPassword*)pPkgData;

            int nPKID = 0;
            int nErrorCode = CSvrUserOrgImpl::getObj().UnLockPassword(userPassword, nPKID);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //发送错误回传
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_UnLockWindow_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_UnLockWindow_Rsp,
                    &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_UserStatus_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(UserStatus),hSocket,Cmd_UserStatus_Rsp,PkgHead.seq);

                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_START))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserStatus_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                UserStatus userStatus = *(UserStatus*)pPkgData;

                int nErrorCode = CSvrUserOrgImpl::getObj().SetUserStatus(userStatus);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserStatus_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {                
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_UserStatus_Rsp,
                        &userStatus.nUserID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeUserUpdate,userStatus.nUserID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_UserStatus_Req, &userStatus, sizeof(UserStatus), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                UserStatus userStatus = *(UserStatus*)pPkgData;
                CSvrUserOrgImpl::getObj().SetUserStatus(userStatus);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                    hSocket,
                    eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeUserUpdate,userStatus.nUserID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QryPrivilege_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryPrivilege_Rsp,PkgHead.seq);

            std::vector<Privilege> vec;
            CSvrUserOrgImpl::getObj().GetPrivileges(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryPrivilege_Rsp,
                    &vec[0],sizeof(Privilege)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryPrivilege_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_SaveRolePrivilegeRelation_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,PRIVILEGE_ROLE_ASSIGN_PRIVILEGE))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveRolePrivilegeRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nRoleID = *(int*)pPkgData;
                int nCount = (PkgHead.len - sizeof(int)) / sizeof(int);
                std::vector<int> vec;
                for ( int i = 0; i < nCount; i++ )
                {
                    int nPrivilegeID = *(int*)((char*)pPkgData + sizeof(int) + sizeof(int)*i);
                    vec.push_back(nPrivilegeID);
                }

                int nDataLen = 1*sizeof(int)+vec.size()*sizeof(int);

                //问询
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_RoleAndPrivelege,
                    AskOperationTypeUpdate,
                    nRoleID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_SaveRolePrivilegeRelation_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrUserOrgImpl::getObj().SaveRolePrivilegeRelation(PkgHead.userdata1, nRoleID, vec);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveRolePrivilegeRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveRolePrivilegeRelation_Rsp,
                        &nRoleID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLEANDPRIVILEGE),
                        hSocket,
                        eSendManager|eSendSpecialUserClient);  
                    std::vector<int> vUserID;
                    CSvrUserOrgImpl::getObj().GetUserIDByRoleID( nRoleID, vUserID );
                    int nCount=0;
                    for (std::vector<int>::iterator it = vUserID.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vUserID.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it;

                    Stru_NotifyEvent lsEvent(EventTypeRoleAndPrivelegeRelation,nRoleID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveRolePrivilegeRelation_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nRoleID = *(int*)pPkgData;
                int nCount = (PkgHead.len - sizeof(int)) / sizeof(int);
                std::vector<int> vec;
                for ( int i = 0; i < nCount; i++ )
                {
                    int nPrivilegeID = *(int*)((char*)pPkgData + sizeof(int) + sizeof(int)*i);
                    vec.push_back(nPrivilegeID);
                }

                CSvrUserOrgImpl::getObj().SaveRolePrivilegeRelation(PkgHead.userdata1, nRoleID, vec);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLEANDPRIVILEGE),
                    hSocket,
                    eSendManager|eSendSpecialUserClient);  
                std::vector<int> vUserID;
                CSvrUserOrgImpl::getObj().GetUserIDByRoleID( nRoleID, vUserID );
                nCount=0;
                for (std::vector<int>::iterator it = vUserID.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vUserID.end();it++)
                    ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it;

                Stru_NotifyEvent lsEvent(EventTypeRoleAndPrivelegeRelation,nRoleID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryRolePrivilegeRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryUserRoleRelation_Rsp,PkgHead.seq);

            std::vector<RoleAndPrivilegeRelation> vec;
            CSvrUserOrgImpl::getObj().GetRolePrivilegeRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryRolePrivilegeRelation_Rsp,
                    &vec[0],sizeof(RoleAndPrivilegeRelation)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryRolePrivilegeRelation_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_AddRole_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(RoleInfo),hSocket,Cmd_AddRole_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,PRIVILEGE_ROLE_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddRole_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                RoleInfo oRoleInfo = *(RoleInfo*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrUserOrgImpl::getObj().AddRole(PkgHead.userdata1,oRoleInfo, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddRole_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddRole_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLE),
                        hSocket,
                        eSendManager);
                    Stru_NotifyEvent lsEvent(EventTypeRoleAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddRole_Req, &oRoleInfo, sizeof(RoleInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                RoleInfo oRoleInfo = *(RoleInfo*)pPkgData;
                int nPKID = 0;
                CSvrUserOrgImpl::getObj().AddRole(PkgHead.userdata1,oRoleInfo, nPKID);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLE),
                    hSocket,
                    eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeRoleAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyRole_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(RoleInfo),hSocket,Cmd_ModifyRole_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,PRIVILEGE_ROLE_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyRole_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                RoleInfo oRoleInfo = *(RoleInfo*)pPkgData;

                std::vector<int> vecUserID;
                CSvrUserOrgImpl::getObj().GetUserIDByRoleID(oRoleInfo.nRoleID,vecUserID);
                for (std::vector<int>::iterator it = vecUserID.begin();it!=vecUserID.end();it++)
                {
                    int nUserID = *it;
                    CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_UserLoginCheck,
                        AskOperationTypeUpdate,
                        nUserID,
                        NULL);
                    if(lRet == CF_ERROR_TRADEDATA_USER_LOGIN)			
                    {
                        //发送错误回传
                        SendRspError(hSocket,
                            Cmd_ModifyRole_Rsp,
                            PkgHead.seq,
                            lRet,
                            lRet,
                            0,0,0);

                        return;
                    }
                }


                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_Role,
                    AskOperationTypeUpdate,
                    oRoleInfo.nRoleID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //发送错误回传
                    SendRspError(hSocket,
                        Cmd_ModifyRole_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrUserOrgImpl::getObj().ModifyRole(PkgHead.userdata1,oRoleInfo);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyRole_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyRole_Rsp,
                        &(oRoleInfo.nRoleID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLE),
                        hSocket,
                        eSendManager|eSendRisk);
                    Stru_NotifyEvent lsEvent(EventTypeRoleUpdate,oRoleInfo.nRoleID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyRole_Req, &oRoleInfo, sizeof(RoleInfo), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                RoleInfo oRoleInfo = *(RoleInfo*)pPkgData;
                CSvrUserOrgImpl::getObj().ModifyRole(PkgHead.userdata1,oRoleInfo);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLE),
                    hSocket,
                    eSendManager|eSendRisk);
                Stru_NotifyEvent lsEvent(EventTypeRoleUpdate,oRoleInfo.nRoleID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_DelRole_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DelRole_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,PRIVILEGE_ROLE_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelRole_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nRoleID = *(int*)pPkgData;
                std::vector<int> vecUserID;
                CSvrUserOrgImpl::getObj().GetUserIDByRoleID(nRoleID,vecUserID);
                for (std::vector<int>::iterator it = vecUserID.begin();it!=vecUserID.end();it++)
                {
                    int nUserID = *it;
                    CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_UserLoginCheck,
                        AskOperationTypeDelete,
                        nUserID,
                        NULL);
                    if(lRet == CF_ERROR_TRADEDATA_USER_LOGIN)			
                    {
                        //发送错误回传
                        SendRspError(hSocket,
                            Cmd_DelRole_Rsp,
                            PkgHead.seq,
                            lRet,
                            lRet,
                            0,0,0);

                        return;
                    }
                }

                int nErrorCode = CSvrUserOrgImpl::getObj().DelRole(PkgHead.userdata1,nRoleID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DelRole_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DelRole_Rsp,
                        &nRoleID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLE),
                        hSocket,
                        eSendManager|eSendRisk);
                    Stru_NotifyEvent lsEvent(EventTypeRoleDelete,nRoleID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DelRole_Req, &nRoleID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nRoleID = *(int*)pPkgData;
                CSvrUserOrgImpl::getObj().DelRole(PkgHead.userdata1,nRoleID);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_ROLE),
                    hSocket,
                    eSendManager|eSendRisk);
                Stru_NotifyEvent lsEvent(EventTypeRoleDelete,nRoleID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QryRole_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryRole_Rsp,PkgHead.seq);

            std::vector<RoleInfo> vec;
            CSvrUserOrgImpl::getObj().GetRole(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryRole_Rsp,
                    &vec[0],sizeof(RoleInfo)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryRole_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_QryUserRoleRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryUserRoleRelation_Rsp,PkgHead.seq);

            std::vector<UserRole> vec;
            CSvrUserOrgImpl::getObj().GetUserRoles(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUserRoleRelation_Rsp,
                    &vec[0],sizeof(UserRole)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUserRoleRelation_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_QryUserOrgRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryUserOrgRelation_Rsp,PkgHead.seq);

            std::vector<UserAndOrgRelation> vec;
            CSvrUserOrgImpl::getObj().GetUserAndOrgRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUserOrgRelation_Rsp,
                    &vec[0],sizeof(UserAndOrgRelation)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUserOrgRelation_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_QryUserTradeAcctRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QryUserTradeAcctRelation_Rsp,PkgHead.seq);

            std::vector<UserAndTradeAccountRelation> vec;
            CSvrUserOrgImpl::getObj().GetUserAndTradeAccountRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUserTradeAcctRelation_Rsp,
                    &vec[0],sizeof(UserAndTradeAccountRelation)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QryUserTradeAcctRelation_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_SaveUserRoleRelation_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                int nUserID = *(int*)pPkgData;
                int nCount = (PkgHead.len - sizeof(int))/sizeof(int);
                std::vector<int> vec;
                for ( int i = 0; i < nCount; i++ )
                {
                    int nRoleID = *(int*)((char*)pPkgData + sizeof(int) + sizeof(int)*i);
                    vec.push_back(nRoleID);
                }

                int nDataLen = 1*sizeof(int)+vec.size()*sizeof(int);

                int nPKID = 0;
                int nErrorCode = CSvrUserOrgImpl::getObj().SaveUserRole(PkgHead.userdata1, nUserID, vec);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserRoleRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveUserRoleRelation_Rsp,
                        NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDROLE),
                        hSocket,
                        eSendManager|eSendSpecialUserClient);

                    if(CSvrUserOrgImpl::getObj().GetUserType(nUserID) == USER_TYPE_RISK_CONTROL)
                        ssErrorInfo.mnSpecialUserIDArray[0] = nUserID;

                    Stru_NotifyEvent lsEvent(EventTypeUserAndRoleRelation,nUserID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserRoleRelation_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nUserID = *(int*)pPkgData;
                int nCount = (PkgHead.len - sizeof(int))/sizeof(int);
                std::vector<int> vec;
                for ( int i = 0; i < nCount; i++ )
                {
                    int nRoleID = *(int*)((char*)pPkgData + sizeof(int) + sizeof(int)*i);
                    vec.push_back(nRoleID);
                }

                CSvrUserOrgImpl::getObj().SaveUserRole(PkgHead.userdata1, nUserID, vec);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDROLE),
                    hSocket,
                    eSendManager|eSendSpecialUserClient);

                if(CSvrUserOrgImpl::getObj().GetUserType(nUserID) == USER_TYPE_RISK_CONTROL)
                    ssErrorInfo.mnSpecialUserIDArray[0] = nUserID;

                Stru_NotifyEvent lsEvent(EventTypeUserAndRoleRelation,nUserID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }  
    case Cmd_SaveUserOrgRelation_FromUser_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                int nErrorCode = 0;
                int nTraderID = 0; 
                int nRiskerID = 0;
                int nDataLen = 0;
                std::set<int> vSet;
                int nUserType =  *(int*)(char*)pPkgData;     

                if ( USER_TYPE_TRADER == nUserType )//交易员
                {
                    if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_TRADER_ASSIGN_ORGANIZATION))
                    {
                        const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                        CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserOrgRelation_FromUser_Rsp, 
                            (void*)lErrorString, 
                            strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                        return;
                    }

                    //现在的
                    int nRelationType = *(int*)((char*)pPkgData+sizeof(int));
                    int nOrgID =  *(int*)((char*)pPkgData+2*sizeof(int));
                    nTraderID = *(int*)((char*)pPkgData+3*sizeof(int));

                    nDataLen = 4*sizeof(int);

                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);

                    //原来的
                    int nPreOrgID;
                    CSvrUserOrgImpl::getObj().GetOrgIDByUserID(nTraderID,nPreOrgID);
                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nPreOrgID,vSet); 

                    nErrorCode = CSvrUserOrgImpl::getObj().SaveUserOrgRelation(PkgHead.userdata1, nOrgID, nUserType, nTraderID, nRelationType);
                }
                else if( USER_TYPE_RISK_CONTROL == nUserType )//风控员
                {
                    if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_RISKER_SETUP_ORGANIZATION))
                    {
                        const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                        CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserOrgRelation_FromUser_Rsp, 
                            (void*)lErrorString, 
                            strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                        return;
                    }

                    int nAttachOrg;
                    std::vector<int> vecMonitorOrg;

                    nRiskerID = *(int*)((char*)pPkgData+sizeof(int));
                    int nRelationType =  *(int*)((char*)pPkgData+2*sizeof(int));
                    if(USER_ORG_RELATION_ATTACH == nRelationType)
                    {
                        nAttachOrg = *(int*)((char*)pPkgData+3*sizeof(int));
                        CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nAttachOrg,vSet);
                    }

                    nRelationType =  *(int*)((char*)pPkgData+4*sizeof(int));
                    if(USER_ORG_RELATION_MONITOR == nRelationType)
                    {
                        int nMonitorOrgCount = *(int*)((char*)pPkgData+5*sizeof(int));                                    
                        for ( int i = 0; i < nMonitorOrgCount; i++ )
                        {
                            int nOrgID = *(int*)((char*)pPkgData + 6*sizeof(int) + sizeof(int)*i);
                            vecMonitorOrg.push_back(nOrgID);
                            CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);
                        }
                    }

                    nDataLen = 6*sizeof(int)+vecMonitorOrg.size()*sizeof(int);

                    nErrorCode = CSvrUserOrgImpl::getObj().SaveUserOrgRelation(PkgHead.userdata1,nRiskerID, nAttachOrg, vecMonitorOrg);
                }

                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserOrgRelation_FromUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveUserOrgRelation_FromUser_Rsp,
                        &nUserType,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    int nSendBit; 
                    if( USER_TYPE_RISK_CONTROL == nUserType )
                        nSendBit = eSendManager|eSendSpecialUserClient;
                    else if( USER_TYPE_TRADER == nUserType )
                        nSendBit = eSendManager|eSendPresentation|eSendSpecialUserClient;

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDORG),
                        hSocket,
                        nSendBit);

                    int nCount=0;
                    for (std::set<int>::iterator it=vSet.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vSet.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it;                

                    Stru_NotifyEvent lsEvent(EventTypeUserAndOrgRelation,(USER_TYPE_TRADER == nUserType)?nTraderID:nRiskerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserOrgRelation_FromUser_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nTraderID = 0; 
                int nRiskerID = 0;
                int nDataLen = 0;
                std::set<int> vSet;
                int nUserType =  *(int*)(char*)pPkgData;     

                if ( USER_TYPE_TRADER == nUserType )//交易员
                {
                    //现在的
                    int nRelationType = *(int*)((char*)pPkgData+sizeof(int));
                    int nOrgID =  *(int*)((char*)pPkgData+2*sizeof(int));
                    nTraderID = *(int*)((char*)pPkgData+3*sizeof(int));

                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);

                    //原来的
                    int nPreOrgID;
                    CSvrUserOrgImpl::getObj().GetOrgIDByUserID(nTraderID,nPreOrgID);
                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nPreOrgID,vSet); 

                    CSvrUserOrgImpl::getObj().SaveUserOrgRelation(PkgHead.userdata1, nOrgID, nUserType, nTraderID, nRelationType);
                }
                else if( USER_TYPE_RISK_CONTROL == nUserType )//风控员
                {
                    int nAttachOrg;
                    std::vector<int> vecMonitorOrg;

                    nRiskerID = *(int*)((char*)pPkgData+sizeof(int));
                    int nRelationType =  *(int*)((char*)pPkgData+2*sizeof(int));
                    if(USER_ORG_RELATION_ATTACH == nRelationType)
                    {
                        nAttachOrg = *(int*)((char*)pPkgData+3*sizeof(int));
                        CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nAttachOrg,vSet);
                    }

                    nRelationType =  *(int*)((char*)pPkgData+4*sizeof(int));
                    if(USER_ORG_RELATION_MONITOR == nRelationType)
                    {
                        int nMonitorOrgCount = *(int*)((char*)pPkgData+5*sizeof(int));                                    
                        for ( int i = 0; i < nMonitorOrgCount; i++ )
                        {
                            int nOrgID = *(int*)((char*)pPkgData + 6*sizeof(int) + sizeof(int)*i);
                            vecMonitorOrg.push_back(nOrgID);
                            CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);
                        }
                    }

                    CSvrUserOrgImpl::getObj().SaveUserOrgRelation(PkgHead.userdata1,nRiskerID, nAttachOrg, vecMonitorOrg);

                    int nSendBit; 
                    if( USER_TYPE_RISK_CONTROL == nUserType )
                        nSendBit = eSendManager|eSendSpecialUserClient;
                    else if( USER_TYPE_TRADER == nUserType )
                        nSendBit = eSendManager|eSendPresentation|eSendSpecialUserClient;

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDORG),
                        hSocket,
                        nSendBit);

                    int nCount=0;
                    for (std::set<int>::iterator it=vSet.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vSet.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it;                

                    Stru_NotifyEvent lsEvent(EventTypeUserAndOrgRelation,(USER_TYPE_TRADER == nUserType)?nTraderID:nRiskerID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
                }
            }

            break;
        } 
    case Cmd_SaveUserOrgRelation_FromOrg_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ORGANIZATION_ASSIGN_TRADER))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserOrgRelation_FromOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    return;
                }

                int nErrorCode = 0;
                int nOrgID = 0;
                int nDataLen = 0;
                std::set<int> vSet;
                int nUserType =  *(int*)(char*)pPkgData;     

                if ( USER_TYPE_TRADER == nUserType )//交易员
                {
                    int nRelationType = *(int*)((char*)pPkgData+sizeof(int));
                    nOrgID =  *(int*)((char*)pPkgData+2*sizeof(int));
                    int nCountIn = *(int*)((char*)pPkgData+3*sizeof(int));
                    std::vector<int> vecTraderIn;
                    for ( int i = 0; i < nCountIn; i++ )
                    {
                        int nTraderID = *(int*)((char*)pPkgData + 4*sizeof(int) + sizeof(int)*i);
                        vecTraderIn.push_back(nTraderID);
                    }

                    int nCountOut = *(int*)((char*)pPkgData+4*sizeof(int)+nCountIn*sizeof(int));
                    std::vector<int> vecTraderOut;
                    for ( int i = 0; i < nCountOut; i++ )
                    {
                        int nTraderID = *(int*)((char*)pPkgData + 4*sizeof(int) + nCountIn*sizeof(int) + sizeof(int) + sizeof(int)*i);
                        vecTraderOut.push_back(nTraderID);
                    }

                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);

                    nDataLen = 4*sizeof(int)+vecTraderIn.size()*sizeof(int)+1*sizeof(int)+vecTraderOut.size()*sizeof(int);

                    nErrorCode = CSvrUserOrgImpl::getObj().SaveUserOrgRelation(PkgHead.userdata1, nOrgID, nUserType, vecTraderIn, vecTraderOut, nRelationType);
                }

                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //发送错误回传
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserOrgRelation_FromOrg_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveUserOrgRelation_FromOrg_Rsp,
                        &nUserType,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    int nSendBit; 
                    if( USER_TYPE_RISK_CONTROL == nUserType )
                        nSendBit = eSendManager|eSendSpecialUserClient;
                    else if( USER_TYPE_TRADER == nUserType )
                        nSendBit = eSendManager|eSendPresentation|eSendSpecialUserClient;

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDORG),
                        hSocket,
                        nSendBit);

                    int nCount=0;
                    for (std::set<int>::iterator it=vSet.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vSet.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it; 

                    Stru_NotifyEvent lsEvent(EventTypeUserAndOrgRelation,nOrgID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserOrgRelation_FromOrg_Req, (char*)pPkgData, nDataLen, PkgHead.seq);
                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nErrorCode = 0;
                int nOrgID = 0;
                int nDataLen = 0;
                std::set<int> vSet;
                int nUserType =  *(int*)(char*)pPkgData;     

                if ( USER_TYPE_TRADER == nUserType )//交易员
                {
                    int nRelationType = *(int*)((char*)pPkgData+sizeof(int));
                    nOrgID =  *(int*)((char*)pPkgData+2*sizeof(int));
                    int nCountIn = *(int*)((char*)pPkgData+3*sizeof(int));
                    std::vector<int> vecTraderIn;
                    for ( int i = 0; i < nCountIn; i++ )
                    {
                        int nTraderID = *(int*)((char*)pPkgData + 4*sizeof(int) + sizeof(int)*i);
                        vecTraderIn.push_back(nTraderID);
                    }

                    int nCountOut = *(int*)((char*)pPkgData+4*sizeof(int)+nCountIn*sizeof(int));
                    std::vector<int> vecTraderOut;
                    for ( int i = 0; i < nCountOut; i++ )
                    {
                        int nTraderID = *(int*)((char*)pPkgData + 4*sizeof(int) + nCountIn*sizeof(int) + sizeof(int) + sizeof(int)*i);
                        vecTraderOut.push_back(nTraderID);
                    }

                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);

                    CSvrUserOrgImpl::getObj().SaveUserOrgRelation(PkgHead.userdata1, nOrgID, nUserType, vecTraderIn, vecTraderOut, nRelationType);

                    int nSendBit; 
                    if( USER_TYPE_RISK_CONTROL == nUserType )
                        nSendBit = eSendManager|eSendSpecialUserClient;
                    else if( USER_TYPE_TRADER == nUserType )
                        nSendBit = eSendManager|eSendPresentation|eSendSpecialUserClient;

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDORG),
                        hSocket,
                        nSendBit);

                    int nCount=0;
                    for (std::set<int>::iterator it=vSet.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vSet.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it; 

                    Stru_NotifyEvent lsEvent(EventTypeUserAndOrgRelation,nOrgID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
                }
            }

            break;
        } 

    case Cmd_SaveUserTradeAcctRelation_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(UserAndTradeAccountRelation),hSocket,Cmd_SaveUserTradeAcctRelation_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_TRADER_ASSIGN_ACCOUNT))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserTradeAcctRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                const char* pBuffer = (char*)pPkgData;

                UserAndTradeAccountRelation oRelation = *(UserAndTradeAccountRelation*)pPkgData;
                int nDataLen = sizeof(UserAndTradeAccountRelation);

                CF_ERROR lRet;
                int bHaveRela = 0;
                std::set<int> vSet;
                if(CSvrUserOrgImpl::getObj().IsAddUserTradeAcctRelation(oRelation.nUserID,oRelation.nTradeAccountID,bHaveRela))
                {
                    if(bHaveRela)
                    {
                        lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_UserAndTradeAccount,
                            AskOperationTypeDelete,
                            oRelation.nUserID,
                            pBuffer);

                        if(lRet == CF_ERROR_SUCCESS)
                        {
                            lRet = CSvrUserOrgImpl::getObj().DelUserAndTradeAccountRelation(PkgHead.userdata1, oRelation.nUserID);
                            if(lRet == CF_ERROR_SUCCESS)
                            {
                                lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_UserAndTradeAccount,
                                    AskOperationTypeAdd,
                                    oRelation.nUserID,
                                    pBuffer);
                                if(lRet == CF_ERROR_SUCCESS)
                                {
                                    lRet = CSvrUserOrgImpl::getObj().AddUserAndTradeAccountRelation(PkgHead.userdata1, oRelation);
                                    if(lRet == CF_ERROR_SUCCESS)
                                    {
                                        CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveUserTradeAcctRelation_Rsp,
                                            &(oRelation),sizeof(UserAndTradeAccountRelation),PkgHead.seq,0,CF_ERROR_SUCCESS);

                                        //发送一个广播事件	
                                        sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                            (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                                            hSocket,
                                            eSendManager);

                                        Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oRelation.nUserID);;
                                        lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                                        CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                                        //发送给从服务器
                                        for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                                            CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserTradeAcctRelation_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                                        break;
                                    }
                                }
                            }
                        }

                        const char * lErrorString = FormatErrorCode(lRet);
                        CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserTradeAcctRelation_Rsp, 
                            (void*)lErrorString, 
                            strlen(lErrorString)+1, PkgHead.seq,0,lRet);

                        break;
                    }
                    else
                    {
                        lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_UserAndTradeAccount,
                            AskOperationTypeAdd,
                            oRelation.nUserID,
                            pBuffer);

                        if(lRet == CF_ERROR_SUCCESS) 
                        {
                            lRet = CSvrUserOrgImpl::getObj().AddUserAndTradeAccountRelation(PkgHead.userdata1, oRelation);
                            if(lRet == CF_ERROR_SUCCESS)
                            {
                                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveUserTradeAcctRelation_Rsp,
                                    &(oRelation),sizeof(UserAndTradeAccountRelation),PkgHead.seq,0,CF_ERROR_SUCCESS);

                                //发送一个广播事件	
                                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                                    hSocket,
                                    eSendManager);

                                Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oRelation.nUserID);;
                                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                                //发送给从服务器
                                for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                                    CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserTradeAcctRelation_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                                break;
                            }
                        }

                        const char * lErrorString = FormatErrorCode(lRet);
                        CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserTradeAcctRelation_Rsp, 
                            (void*)lErrorString, 
                            strlen(lErrorString)+1, PkgHead.seq,0,lRet);

                        break;
                    }
                }
                else
                {
                    //已存在相应的关系 只更新属性,不用问询
                    lRet = CSvrUserOrgImpl::getObj().UpdateLimitTradeAndManualVerify(PkgHead.userdata1, oRelation);
                    if(lRet == CF_ERROR_SUCCESS)
                    {
                        CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveUserTradeAcctRelation_Rsp,
                            &(oRelation),sizeof(UserAndTradeAccountRelation),PkgHead.seq,0,CF_ERROR_SUCCESS);

                        //发送一个广播事件	
                        sErrorInfo ssErrorInfo(OperationLogNeedSend,
                            (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                            hSocket,
                            eSendManager|eSendSpecialUserClient);

                        //发送给从服务器
                        for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                            CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserTradeAcctRelation_Req, (char*)pPkgData, nDataLen, PkgHead.seq);


                        int nOrgID = CSvrUserOrgImpl::getObj().GetAttachOrgIDByUserID(oRelation.nUserID);
                        CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);
                        int nCount=0;
                        for (std::set<int>::iterator it=vSet.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vSet.end();it++)
                            ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it;    


                        Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oRelation.nUserID);;
                        lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                        CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                        break;
                    }                

                    const char * lErrorString = FormatErrorCode(lRet);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveUserTradeAcctRelation_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,lRet);
                }
            } 
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                const char* pBuffer = (char*)pPkgData;
                UserAndTradeAccountRelation oRelation = *(UserAndTradeAccountRelation*)pPkgData;
                int nDataLen = sizeof(UserAndTradeAccountRelation);

                CF_ERROR lRet;
                int bHaveRela = 0;
                std::set<int> vSet;
                if(CSvrUserOrgImpl::getObj().IsAddUserTradeAcctRelation(oRelation.nUserID,oRelation.nTradeAccountID,bHaveRela))
                {
                    if(bHaveRela)
                    {
                        lRet = CSvrUserOrgImpl::getObj().DelUserAndTradeAccountRelation(PkgHead.userdata1, oRelation.nUserID);
                        if(lRet == CF_ERROR_SUCCESS)
                            CSvrUserOrgImpl::getObj().AddUserAndTradeAccountRelation(PkgHead.userdata1, oRelation);
                    }
                    else
                        CSvrUserOrgImpl::getObj().AddUserAndTradeAccountRelation(PkgHead.userdata1, oRelation);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        eSendManager);

                    Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oRelation.nUserID);;
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
                    
                    break;
                }
                else
                {
                    //已存在相应的关系 只更新属性,不用问询
                    CSvrUserOrgImpl::getObj().UpdateLimitTradeAndManualVerify(PkgHead.userdata1, oRelation);

                    //发送一个广播事件	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                        hSocket,
                        eSendManager|eSendSpecialUserClient);

                    //发送给从服务器
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveUserTradeAcctRelation_Req, (char*)pPkgData, nDataLen, PkgHead.seq);


                    int nOrgID = CSvrUserOrgImpl::getObj().GetAttachOrgIDByUserID(oRelation.nUserID);
                    CSvrUserOrgImpl::getObj().GetMonitorUserIDByOrgID(nOrgID,vSet);
                    int nCount=0;
                    for (std::set<int>::iterator it=vSet.begin();nCount<MAX_SEND_SPECIAL_USER_SIZE && it!=vSet.end();it++)
                        ssErrorInfo.mnSpecialUserIDArray[nCount++] = *it;    

                    Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oRelation.nUserID);;
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    break;
                }
            }
        }
   case Cmd_RM_Master_SetLimitTrade_Req:
       {
           int nRiskerID = PkgHead.userdata3;
           if(ServerRunMode_Master == g_bServerRunMode)
           {
               BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(SLimitTrade),hSocket,Cmd_RM_Master_SetLimitTrade_Rsp,PkgHead.seq);
               
               if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(nRiskerID,RISK_LIMIT_TRADE))
               {
                   const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                   CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_Master_SetLimitTrade_Rsp, 
                       (void*)lErrorString, 
                       strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                   break;
               }

               SLimitTrade oLimitTrade = *(SLimitTrade*)pPkgData;

               int nErrorCode = CSvrUserOrgImpl::getObj().SetLimitTradeFromRisk(oLimitTrade.nUserID,oLimitTrade.bLimitTrade);
               if(CF_ERROR_SUCCESS != nErrorCode)
               {
                   //发送错误回传
                   const char * lErrorString = FormatErrorCode(nErrorCode);
                   CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_Master_SetLimitTrade_Rsp, 
                       (void*)lErrorString, 
                       strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode,0,0,PkgHead.userdata4);	
               }
               else
               {
                   CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_Master_SetLimitTrade_Rsp,
                       (void*)pPkgData, PkgHead.len,0,0,CF_ERROR_SUCCESS,0,0,PkgHead.userdata4);

                   //发送一个广播事件	
                   sErrorInfo ssErrorInfo(OperationLogNeedSend,
                       (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                       hSocket,
                       eSendManager);

                   Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oLimitTrade.nUserID);
                   lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                   CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                   //发送给从服务器
                   for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                       CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_RM_Master_SetLimitTrade_Req, &oLimitTrade, sizeof(SLimitTrade), PkgHead.seq,0,
                       PkgHead.userdata1,PkgHead.userdata2,PkgHead.userdata3,PkgHead.userdata4);

               }
           }
           else if(ServerRunMode_Slave == g_bServerRunMode)
           {
               SLimitTrade oLimitTrade = *(SLimitTrade*)pPkgData;

               CSvrUserOrgImpl::getObj().SetLimitTradeFromRisk(oLimitTrade.nUserID,oLimitTrade.bLimitTrade);

               //发送一个广播事件	
               sErrorInfo ssErrorInfo(OperationLogNeedSend,
                   (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                   hSocket,
                   eSendRisk);

               Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oLimitTrade.nUserID);;
               lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
               CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
           }

           break;
       } 
   case Cmd_RM_Master_SetManualVerify_Req:
       {
           int nRiskerID = PkgHead.userdata3;
           if(ServerRunMode_Master == g_bServerRunMode)
           {
               BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(SManualVerify),hSocket,Cmd_RM_Master_SetManualVerify_Rsp,PkgHead.seq);
               if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(nRiskerID,RISK_MANUAL_VERIFY))
               {
                   const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                   CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_Master_SetManualVerify_Rsp, 
                       (void*)lErrorString, 
                       strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                   break;
               }

               SManualVerify oManualVerify = *(SManualVerify*)pPkgData;

               int nErrorCode = CSvrUserOrgImpl::getObj().SetManualVerifyFromRisk(oManualVerify.nUserID,oManualVerify.bManualVerify);
               if(CF_ERROR_SUCCESS != nErrorCode)
               {
                   //发送错误回传
                   const char * lErrorString = FormatErrorCode(nErrorCode);
                   CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_RM_Master_SetManualVerify_Rsp, 
                       (void*)lErrorString, 
                       strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode,0,0,PkgHead.userdata4);	
               }
               else
               {
                   CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_RM_Master_SetManualVerify_Rsp,
                      (void*)pPkgData, PkgHead.len,0,0,CF_ERROR_SUCCESS,0,0,PkgHead.userdata4);

                   //发送一个广播事件	
                   sErrorInfo ssErrorInfo(OperationLogNeedSend,
                       (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                       hSocket,
                       eSendManager);

                   Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oManualVerify.nUserID);;
                   lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                   CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                   //发送给从服务器
                   for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                       CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_RM_Master_SetManualVerify_Req, &oManualVerify, sizeof(SManualVerify), PkgHead.seq,0,
                       PkgHead.userdata1,PkgHead.userdata2,PkgHead.userdata3,PkgHead.userdata4);

               }
           }
           else if(ServerRunMode_Slave == g_bServerRunMode)
           {
               SManualVerify oManualVerify = *(SManualVerify*)pPkgData;

                CSvrUserOrgImpl::getObj().SetManualVerifyFromRisk(oManualVerify.nUserID,oManualVerify.bManualVerify);

               //发送一个广播事件	
               sErrorInfo ssErrorInfo(OperationLogNeedSend,
                   (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USER),
                   hSocket,
                   eSendRisk);

               Stru_NotifyEvent lsEvent(EventTypeUserLimitAndVerifyUpdate,oManualVerify.nUserID);;
               lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
               CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
           }

           break;
       } 
    }
}