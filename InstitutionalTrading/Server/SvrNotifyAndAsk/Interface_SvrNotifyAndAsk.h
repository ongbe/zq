#pragma once

//************************************************************************************************
//	通知和问询接口
//		1. 通知接口为NotifyEvent()，异步，同时广播给其它服务器。
//		2. 问询接口是Ask()和Ask2()，同步，只能问本服务器的模块。
//************************************************************************************************

#include "Interface_SvrCommon.h"
#include "CommonDataChangeEvent.h"
#include "CommonStruct.h"
#include "socketBase.h"
#include <map>
#include <vector>
#ifndef SVRNOTIFYANDASK_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRNOTIFYANDASK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRNOTIFYANDASK_EXPORTS
#define SVRNOTIFYANDASK_API __declspec(dllexport)
#else
#define SVRNOTIFYANDASK_API __declspec(dllimport)
#endif
#endif


class SVRNOTIFYANDASK_API CInterface_SvrNotifyAndAsk : public CInterface_SvrCommon
{
public:
    static CInterface_SvrNotifyAndAsk& getObj(void);

public:
	//验证用户权限
	virtual bool Ask_ValidateUserPrivilege(const int& nUserID,const ePrivilege& nPrivilege )  = 0;

public:
	//问询函数，问各个模块指定操作是否可以执行。返回CF_ERROR_SUCCESS表示可以。一票否决原则。调用注册模块的ResponseToAsk()
	virtual CF_ERROR Ask(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf) = 0;
	//问询函数，询问各个模块某个问题。返回CF_ERROR_SUCCESS表示Yes，否则表示No。通过时，npRlt可以是任意类型的指针，用于应答者返回数据。一票通过原则。调用注册模块的ResponseToAsk2()
	virtual CF_ERROR Ask2(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf,void* pRlt=NULL) = 0;

	//注册问询的接口函数
	virtual void RegistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface)  = 0;
	virtual void UnregistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface)  = 0;
	virtual void UnregistAsk(CInterface_SvrCommon * const npInterface)  = 0;

	

public:
	//通知一个事件
	virtual CF_ERROR NotifyEvent(const Stru_NotifyEvent& nDataChangeEvent) = 0;

	//业务模块订阅内部数据变化通知，需指定数据类型和线程ID。收到通知后，Post到对应线程中
	virtual void SubscribeNotifyEvent(const eEventType& neDataType,const unsigned int& idThread)  = 0;
	virtual void UnsubscribeNotifyEvent(const eEventType& neDataType,const unsigned int& idThread) = 0;
	virtual void UnsubscribeNotifyEvent(const unsigned int& idThread) = 0;

public:
	//添加一个错误
	virtual CF_ERROR PushErrorInfo(const sErrorInfo& nErrorInfo,bool bSendMgrClient = true,bool bShow = true) = 0;
	//获取所有的错误
	virtual CF_ERROR GetAllError(std::vector<sErrorInfo>& nVecError) = 0;
	//清除所有错误
	virtual void ClearAllError() = 0;
};







