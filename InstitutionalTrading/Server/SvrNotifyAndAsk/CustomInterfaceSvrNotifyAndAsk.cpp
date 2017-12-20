// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "stdafx.h"
#include "CustomInterfaceSvrNotifyAndAsk.h"
#include "SvrNotifyAndAsk.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonMacro.h"
#include "Tools_Ansi.h"

#include "..\\SvrTcp\\Interface_SvrTcp.h"

CCustomInterfaceSvrNotifyAndAsk* CCustomInterfaceSvrNotifyAndAsk::m_pObj=NULL;


CCustomInterfaceSvrNotifyAndAsk::CCustomInterfaceSvrNotifyAndAsk()
{
	CPopMsgWin::Stru_Param lParam;
	lParam.bInServiceID = true;
	lParam.bCreateOKBtn = false;
	lParam.InitWid = 640;
	lParam.InitHei = 480;
	lParam.MaxMsgLen = 50000; //(true,false,true,false,true,true,true,true,100,100,640,480,50000);
    strncpy(lParam.szTitle,"机构版交易系统服务器信息",sizeof(lParam.szTitle)-1);
	mpPopMsgWnd = new CPopMsgWin(lParam);
}

CCustomInterfaceSvrNotifyAndAsk::~CCustomInterfaceSvrNotifyAndAsk()
{
	if(mpPopMsgWnd)
		delete mpPopMsgWnd;
}

CCustomInterfaceSvrNotifyAndAsk& CCustomInterfaceSvrNotifyAndAsk::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CCustomInterfaceSvrNotifyAndAsk();
    g_mutex.unlock();
    return *m_pObj;
}


//注册问询的接口函数
void CCustomInterfaceSvrNotifyAndAsk::RegistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface) 
{
	if(!npInterface) return;
	CeasymutexGuard guard(g_mutex);
	g_mapDataType2Interface.insert(std::make_pair(neDataType,npInterface));
}
void CCustomInterfaceSvrNotifyAndAsk::UnregistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface)
{
	CeasymutexGuard guard(g_mutex);
	CTools_Ansi::EraseKeyValueInMultimap<eAskType, CInterface_SvrCommon* const>(g_mapDataType2Interface,neDataType,npInterface);
}
void CCustomInterfaceSvrNotifyAndAsk::UnregistAsk(CInterface_SvrCommon * const npInterface)
{
	CeasymutexGuard guard(g_mutex);
	CTools_Ansi::EraseValueInMultimap<eAskType, CInterface_SvrCommon* const>(g_mapDataType2Interface,npInterface);
}


//业务模块订阅内部数据变化通知，需指定数据类型和线程ID。收到通知后，Post到对应线程中
void CCustomInterfaceSvrNotifyAndAsk::SubscribeNotifyEvent(const eEventType& neDataType,const unsigned int& idThread) 
{
	CeasymutexGuard guard(g_mutexEvent);
	if(!CTools_Ansi::IsKeyValueInMultimap<eEventType,unsigned int>(g_mapDataType2Thread,neDataType,idThread))
		g_mapDataType2Thread.insert(pair<eEventType,unsigned int>(neDataType,idThread));
}

//模块退订内部数据变化通知，需指定数据类型和线程ID
void CCustomInterfaceSvrNotifyAndAsk::UnsubscribeNotifyEvent(const eEventType&  neDataType,const unsigned int & idThread) 
{
	CeasymutexGuard guard(g_mutexEvent);
	CTools_Ansi::EraseKeyValueInMultimap<eEventType,unsigned int>(g_mapDataType2Thread,neDataType,idThread);
}

//模块退订全部内部数据变化通知，需指定线程ID
void CCustomInterfaceSvrNotifyAndAsk::UnsubscribeNotifyEvent(const unsigned int& idThread) 
{
	CeasymutexGuard guard(g_mutexEvent);
	CTools_Ansi::EraseValueInMultimap<eEventType,unsigned int>(g_mapDataType2Thread,idThread);
}

//发送一个通知，同时广播到其它服务器
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::NotifyEvent(const Stru_NotifyEvent& nDataChangeEvent) 
{
	return NotifyEvent2(nDataChangeEvent,0);
}
//发送一个通知，广播到其它服务器时排除指定服务器
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::NotifyEvent2(const Stru_NotifyEvent& nDataChangeEvent,int ExcludeSocket)
{
	CeasymutexGuard guard(g_mutexEvent);

	//先通知本服务器的模块
	multimap<eEventType,unsigned int>::iterator it2lower=g_mapDataType2Thread.lower_bound(nDataChangeEvent.meEventType);
	multimap<eEventType,unsigned int>::iterator it2upper=g_mapDataType2Thread.upper_bound(nDataChangeEvent.meEventType);
	for(;it2lower!=it2upper;it2lower++)
	{
		unsigned int EventParamID;
		if(CInterface_SvrTcp::getObj().getEventParamObj().CreateEventParam(EventParamID,NULL,NULL,&nDataChangeEvent,sizeof(nDataChangeEvent),0))
		{
			int iCount = 0;
			while(!PostThreadMessage(it2lower->second,WM_COMMAND,WndCmd_NotifyEventArrival,(LPARAM)EventParamID))
			{		
				Sleep(10);
				NOTIFY_LOG("PostTheadMessage Faild: Event ID %d Retry Count %d,THread %d Error %d",EventParamID,iCount+1,it2lower->second,GetLastError());
				if((++iCount) == 4)
					break;							
			}
		}
	}

	//然后通知其它服务器
	set<int> ServerSockets=CInterface_SvrTcp::getObj().GetAllServerSockets();
	for(set<int>::const_iterator it=ServerSockets.begin();it!=ServerSockets.end();it++)
	{
		if(*it==ExcludeSocket) continue;
		CInterface_SvrTcp::getObj().SendPkgData(*it,CMDID_NotifyAndAsk_BroadcastNotify,&nDataChangeEvent,sizeof(nDataChangeEvent),0);
	}

	return CF_ERROR_SUCCESS;
}

//验证用户权限
bool CCustomInterfaceSvrNotifyAndAsk::Ask_ValidateUserPrivilege(const int& nUserID,const ePrivilege& nPrivilege ) 
{
    CF_ERROR lRet = Ask(AskType_QueryPrivilege,AskOperationTypeQuery,nUserID,(const char*)&nPrivilege);
    if(lRet != CF_ERROR_SUCCESS)			
        return false;

    return true;
}

//问询函数，问各个模块指定操作是否可以执行。返回CF_ERROR_SUCCESS表示可以
//一票否决原则	
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::Ask(const eAskType& neDataType,
												const eAskOperationType& eOp,
												int nID,
												const char * npDataBuf)
{
	CeasymutexGuard guard(g_mutex);

	CF_ERROR lRet = CF_ERROR_SUCCESS;

	multimap<eAskType, CInterface_SvrCommon*const>::iterator itlower=g_mapDataType2Interface.lower_bound(neDataType);
	multimap<eAskType, CInterface_SvrCommon*const>::iterator itupper=g_mapDataType2Interface.upper_bound(neDataType);
	for(;itlower!=itupper;itlower++)
	{
		CInterface_SvrCommon * const lpInterface = itlower->second;
		if(lpInterface)
		{	
			lRet = lpInterface->ResponseToAsk(neDataType,eOp,nID,npDataBuf);
			if(lRet != CF_ERROR_SUCCESS)
				break;
		}
	}
	return lRet;
}

//问询函数，询问各个模块某个问题。返回CF_ERROR_SUCCESS表示Yes，否则表示No。通过时，npRlt可以是任意类型的指针，用于应答者返回数据。调用注册模块的ResponseToAsk2()
//一票通过原则
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::Ask2(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf,void* pRlt)
{
	CeasymutexGuard guard(g_mutex);

	CF_ERROR lRet = CF_ERROR_FAILURE;

	multimap<eAskType, CInterface_SvrCommon*const>::iterator itlower=g_mapDataType2Interface.lower_bound(neDataType);
	multimap<eAskType, CInterface_SvrCommon*const>::iterator itupper=g_mapDataType2Interface.upper_bound(neDataType);
	for(;itlower!=itupper;itlower++)
	{
		CInterface_SvrCommon * const lpInterface = itlower->second;
		if(lpInterface)
		{	
			lRet = lpInterface->ResponseToAsk2(neDataType,eOp,nID,npDataBuf,pRlt);
			if(lRet == CF_ERROR_SUCCESS)
				break;
		}
	}
	return lRet;
}

//添加一个错误
CF_ERROR  CCustomInterfaceSvrNotifyAndAsk::PushErrorInfo(const sErrorInfo& nErrorInfo,bool bSendMgrClient,bool bShow ) 
{
	if(nErrorInfo.meErrorType == FirstLevelError ||
		nErrorInfo.meErrorType == SecondLevelError ||
		nErrorInfo.meErrorType == ThirdLevelError)
	{
		mEventMutexError.lock();
		mvecError.push_back(nErrorInfo);
		mEventMutexError.unlock();
	}

	NOTIFY_LOG("%d level Error Come: %s ",nErrorInfo.meErrorType+1,nErrorInfo.msError);
	if(bSendMgrClient)
	{
		Stru_NotifyEvent lEvent(EventTypeErrorCome,0);
		lEvent.AddBufer((void*)&nErrorInfo,sizeof(nErrorInfo));
		NotifyEvent(lEvent);
	}

	if(bShow)
	{
		 if(mpPopMsgWnd )
		 {
			 mpPopMsgWnd->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				 CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
				 nErrorInfo.msError);
		 }
	}

	return CF_ERROR_SUCCESS;
}
//获取所有的错误
 CF_ERROR CCustomInterfaceSvrNotifyAndAsk::GetAllError(std::vector<sErrorInfo>& nVecError) 
 {	
	 mEventMutexError.lock();
	 nVecError.clear();
	 nVecError = mvecError;
	 mEventMutexError.unlock();
	 return CF_ERROR_SUCCESS;
 }

	 
void CCustomInterfaceSvrNotifyAndAsk::ClearAllError()
{
	mEventMutexError.lock();
	mvecError.clear();
	mEventMutexError.unlock();
}