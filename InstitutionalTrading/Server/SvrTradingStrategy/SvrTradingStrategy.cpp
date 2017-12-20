// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrTradingStrategy.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "SvrTradingStrategyBusiness.h"
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
#pragma comment(lib, "SvrDBOpr.lib")
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrUserOrg.lib")


#define WRITELOGID_SvrTradingStrategy




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);


//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRTRADINGSTRATEGY_API void InitFunc(void)
{
	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	
    CInterface_SvrTradingStrategy::getObj();

    //从数据库得到交易策略
    CSvrTradingStrategyBusiness::getObj().LoadDataFromDB();
}

//模块结束，释放资源，关闭工作线程
SVRTRADINGSTRATEGY_API void ReleaseFunc(void)
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

        CSvrTradingStrategyBusiness::deleteObj();
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
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DeleteTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveStrategyOrgRelation_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryStrategyOrgRelation_Req,g_idThread);

    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Orgnazation,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradingStrategy::getObj()));
    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_TradeStrategy,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradingStrategy::getObj()));


	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
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

//处理一个SvrTcp推送过来的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
    //-----------------------------------------------------------------------------------
    //	下面根据数据包的命令字，处理数据包
    //-----------------------------------------------------------------------------------
    switch(PkgHead.cmdid)
    {
    case Cmd_AddTradeStrategy_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeStrategy),hSocket,Cmd_AddTradeStrategy_Rsp,PkgHead.seq);
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_ADD))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddTradeStrategy_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            TradeStrategy *pStrategy = (TradeStrategy*)pPkgData;

            int nPKID = 0;
            int nErrorCode = CSvrTradingStrategyBusiness::getObj().AddStrategy(PkgHead.userdata1,pStrategy, nPKID);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //发送错误回传
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddTradeStrategy_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddTradeStrategy_Rsp,
                                                        (void*)&nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //发送一个广播事件		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGY),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_DeleteTradeStrategy_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DeleteTradeStrategy_Rsp,PkgHead.seq);
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_DEL))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DeleteTradeStrategy_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            int nStrategyID = *(int*)pPkgData;

            //问询服务器状态
            CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_TradeStrategy,
                                                                            AskOperationTypeDelete,
                                                                            nStrategyID,
                                                                            NULL);
            if(lRet != CF_ERROR_SUCCESS)			
            {
                //发送错误回传
                SendRspError(hSocket,
                            Cmd_DeleteTradeStrategy_Rsp,
                            PkgHead.seq,
                            lRet,
                            lRet,
                            0,0,0);
                break;
            }

            int nErrorCode = CSvrTradingStrategyBusiness::getObj().DeleteStrategy(PkgHead.userdata1,nStrategyID);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //发送错误回传
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DeleteTradeStrategy_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DeleteTradeStrategy_Rsp,
                                            &nStrategyID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGY),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyDelete,nStrategyID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyTradeStrategy_Req:
        {            
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeStrategy),hSocket,Cmd_ModifyTradeStrategy_Rsp,PkgHead.seq);
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_MODIFY))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyTradeStrategy_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            TradeStrategy *pStrategy = (TradeStrategy*)pPkgData;

            CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_TradeStrategy,
                                                                            AskOperationTypeUpdate,
                                                                            pStrategy->nID,
                                                                            NULL);
            if(lRet != CF_ERROR_SUCCESS)			
            {
                //发送错误回传
                SendRspError(hSocket,
                    Cmd_ModifyTradeStrategy_Rsp,
                    PkgHead.seq,
                    lRet,
                    lRet,
                    0,0,0);
                break;
            }

            int nErrorCode = CSvrTradingStrategyBusiness::getObj().ModifyStrategy(PkgHead.userdata1,pStrategy);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //发送错误回传
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyTradeStrategy_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyTradeStrategy_Rsp,
                            (void*)&(pStrategy->nID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //发送一个广播事件	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGY),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyUpdate,pStrategy->nID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryTradeStrategy_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryTradeStrategy_Rsp,PkgHead.seq);

            std::vector<TradeStrategy> vec;
            CSvrTradingStrategyBusiness::getObj().GetTradeStrategy(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryTradeStrategy_Rsp,
                            &vec[0],sizeof(TradeStrategy)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryTradeStrategy_Rsp,
                                                        NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_SaveStrategyOrgRelation_Req:
        {
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_ASSIGN_ORGANIZATION))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveStrategyOrgRelation_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            int nStrategyID = *(int*)pPkgData;
            int nCount = (PkgHead.len - sizeof(int)) / sizeof(int);
            std::vector<int> vec;
            for ( int i = 0; i < nCount; i++ )
            {
                int nOrgID = *(int*)((char*)pPkgData + sizeof(int) + sizeof(int)*i);
                vec.push_back(nOrgID);
            }

            //问询
            CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_OrgAndStrategy,
                                                                            AskOperationTypeUpdate,
                                                                            nStrategyID,
                                                                            NULL);
            if(lRet != CF_ERROR_SUCCESS)			
            {
                //发送错误回传
                SendRspError(hSocket,
                    Cmd_SaveStrategyOrgRelation_Rsp,
                    PkgHead.seq,
                    lRet,
                    lRet,
                    0,0,0);
                break;
            }

            int nErrorCode = CSvrTradingStrategyBusiness::getObj().SaveStrategyOrganizationRelation(PkgHead.userdata1,nStrategyID,vec);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //发送错误回传
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveStrategyOrgRelation_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveStrategyOrgRelation_Rsp,
                                                (void*)&nStrategyID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //发送一个广播事件
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGYANDORG),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyAndOrgRelation,nStrategyID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }

    case Cmd_QueryStrategyOrgRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryStrategyOrgRelation_Rsp,PkgHead.seq);

            std::vector<AssetOrgStrategyRelation> vec;
            CSvrTradingStrategyBusiness::getObj().GetStrategyOrganizationRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryStrategyOrgRelation_Rsp,
                    &vec[0],sizeof(AssetOrgStrategyRelation)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryStrategyOrgRelation_Rsp,
                                                        NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    }
}