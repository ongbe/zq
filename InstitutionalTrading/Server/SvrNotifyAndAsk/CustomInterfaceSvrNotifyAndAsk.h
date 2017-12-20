#pragma once


#include "CommonDataChangeEvent.h"
#include "CommonStruct.h"
#include "socketBase.h"
#include <map>
#include <vector>
#include "Interface_SvrNotifyAndAsk.h"
#include "popmsgwin.h"




class CCustomInterfaceSvrNotifyAndAsk : public CInterface_SvrNotifyAndAsk
{
public:
    static CCustomInterfaceSvrNotifyAndAsk& getObj(void);

public:
	//验证用户权限
	virtual bool Ask_ValidateUserPrivilege(const int& nUserID,const ePrivilege& nPrivilege ) ;

public:
	//问询函数，问各个模块指定操作是否可以执行。返回CF_ERROR_SUCCESS表示可以。一票否决原则	
	virtual CF_ERROR Ask(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf) ;
	//问询函数，询问各个模块某个问题。返回CF_ERROR_SUCCESS表示Yes，否则表示No。一票通过原则。通过时，npRlt可以是任意类型的指针，用于应答者返回数据。调用注册模块的ResponseToAsk2()
	virtual CF_ERROR Ask2(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf,void* pRlt=NULL);

	//注册问询的接口函数
	virtual void RegistAsk(const eAskType& neDataType,CInterface_SvrCommon * const npInterface) ;
	virtual void UnregistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface);
	virtual void UnregistAsk(CInterface_SvrCommon * const npInterface);



public:
	//发送一个通知，同时广播到其它服务器
	virtual CF_ERROR NotifyEvent(const Stru_NotifyEvent& nDataChangeEvent) ;
	//发送一个通知，广播到其它服务器时排除指定服务器
	virtual CF_ERROR NotifyEvent2(const Stru_NotifyEvent& nDataChangeEvent,int ExcludeSocket);

	//业务模块订阅内部数据变化通知，需指定数据类型和线程ID。收到通知后，Post到对应线程中
	void SubscribeNotifyEvent(const eEventType& neEventType,const unsigned int & idThread) ;
	void UnsubscribeNotifyEvent(const eEventType& neEventType,const unsigned int& idThread) ;
	void UnsubscribeNotifyEvent(const unsigned int& idThread) ;

public:
	//添加一个错误
	virtual CF_ERROR PushErrorInfo(const sErrorInfo& nErrorInfo,bool bSendMgrClient = true,bool bShow = true) ;
	//获取所有的错误
	virtual CF_ERROR GetAllError(std::vector<sErrorInfo>& nVecError) ;
	//清除所有错误
	virtual void ClearAllError();


private:
	
    CCustomInterfaceSvrNotifyAndAsk();
    virtual ~ CCustomInterfaceSvrNotifyAndAsk();
    static CCustomInterfaceSvrNotifyAndAsk* m_pObj;

	Ceasymutex mEventMutex;

	Ceasymutex mEventMutexError;
	std::vector<sErrorInfo> mvecError;

	CPopMsgWin * mpPopMsgWnd;
};







