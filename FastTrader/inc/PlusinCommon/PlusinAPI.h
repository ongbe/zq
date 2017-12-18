#ifndef PLUSINAPI_H
#define PLUSINAPI_H

#include "CommonDef.h"

typedef int (*MsgCallBackFunc)(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

class __declspec(dllexport) PlusinAPI
{
public:
	virtual ~PlusinAPI(){}

	/**
	 * @功能描述: 获取API对象的指针
	 * @返 回 值: API对象指针
	**/
	static PlusinAPI* GetInstance();


	/**
	 * @功能描述: 销毁API对象
	 * @返 回 值: 无
	**/
	static void DestroyInstance();

	/**
	 * @功能描述: 初始化API，设置必要参数
	 * @参数列表: szAppID :			插件唯一标识
	 * @参数列表: szLicense :		插件License
	 * @参数列表: msgCallBackFn :	用于接收消息的回调函数指针
	 * @参数列表: hTargetWnd :		FastTrader顶层窗口句柄，用于向FastTrader进程发送消息
	 * @参数列表: nPos :				插件期望显示的位置
	 * @参数列表: hAPPHWnd :			插件顶层窗口句柄，没有顶层窗口句柄时，为NULL
	 * @返 回 值: 无
	**/
	virtual void Init(const char* szAppID, 
		const char* szLicense, 
		MsgCallBackFunc msgCallBackFn, 
		HWND hTargetWnd,
		int nPos = NEED_NO_POS_NO_BIND, 
		HWND hAPPHWnd = NULL) = 0;

	/**
	 * @功能描述: 向FastTrader进程发起登录请求
	 * @返 回 值: true：发送请求成功 false：发送请求失败
	**/
	virtual bool Logon() = 0;

	/**
	 * @功能描述: 向FastTrader进程发送消息
	 * @参数列表: nCmdId :		消息命令字
	 * @参数列表: pData :		消息内容指针
	 * @参数列表: nDataLen :		消息内容长度
	 * @参数列表: nRequestId :	请求序号
	 * @返 回 值: true：消息发送成功 false：消息发送失败
	**/
	virtual bool Send(int nCmdId, void* pData, int nDataLen, int nRequestId = 0) = 0;

	/**
	 * @功能描述: 退出登录并释放资源
	 * @返 回 值: 无
	**/
	virtual void Logout() = 0;
};

#endif