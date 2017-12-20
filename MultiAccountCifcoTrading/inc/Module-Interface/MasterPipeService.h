// MasterPipeService.h: interface for the CMasterPipeService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERPIPESERVICE_H__77B25F66_FC48_449E_9D40_B493533A6884__INCLUDED_)
#define AFX_MASTERPIPESERVICE_H__77B25F66_FC48_449E_9D40_B493533A6884__INCLUDED_


#include "AppPipeManager.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMasterPipeService  
{
protected:
	BOOL				m_bInitOK;
	BOOL				m_bHasSubscribeResponse;
	LPVOID				m_lpOwner;
	CAppPipeManager		m_appPipeManager;

public:
	static CMasterPipeService* GetOnlyOneInstance();
	static void ReleaseOnlyOneInstance();
	// 侦听客户端消息的线程过程
	static DWORD WINAPI Proc_MasterPipe(LPVOID lpvParam); 
	// 推送数据给客户端的线程过程
	static DWORD WINAPI Proc_PushToAppPipe(LPVOID lpvParam); 
	// 把个单元数据合并一天有效的协议消息
	// 参数pDataBuff是用来保存消息的换成
	// 参数pAppendDataBuff是需要附加在协议头后的数据，数据长度为dwAppendDataLen
	// 返回值：
	//     0，表示合并失败
	//     >0，表示合并成功，数值为消息的长度
	static DWORD MergePackage(BYTE* pDataBuff, DWORD dwDataBuffSize, ISessionIDType SessionID, 
				DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum, 
				BYTE* pAppendDataBuff=NULL, DWORD dwAppendDataLen=0);
	static BOOL MergeAndSend(string& strAppPipe, BYTE* pDataBuff, DWORD dwDataBuffSize, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum, 
				BYTE* pAppendDataBuff=NULL, DWORD dwAppendDataLen=0);

	static BOOL MergeAndSend_Accounts(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_ContractIDInfos(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_AllOrders(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_AllPositions(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_ExchangeDateTime(string& strAppPipe, string& strExchangeID, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_DepthMarket(string& strAppPipe, string& strInstrumentID, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);

	static int RspCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);

public:
	BOOL IsInitOK() {
		return m_bInitOK;
	};
	BOOL HasSubscribeResponse() {
		return m_bHasSubscribeResponse;
	};

	CAppPipeManager& GetAppPipeManager() {
		return m_appPipeManager;
	};

	BOOL Init(LPVOID lpOwner=NULL);
	void SubscribeResponse();
	// 用于处理登录的过程，解析用户名、密码等
	BOOL Login(ILOGINDATA& tLogin);

	// 解析并处理登录消息
	BOOL ParseLogin(HANDLE hMasterPipe, IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen); 

	// 分类解析并处理消息
	BOOL ParseMsgType(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// 解析并处理A类消息，不包括登录消息
	BOOL ParseMsgType_A(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// 解析并处理C类消息
	BOOL ParseMsgType_C(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// 解析并处理E类消息
	BOOL ParseMsgType_E(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// 解析并处理G类消息
	BOOL ParseMsgType_G(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);

public:
	CMasterPipeService();
	virtual ~CMasterPipeService();

};

#endif // !defined(AFX_MASTERPIPESERVICE_H__77B25F66_FC48_449E_9D40_B493533A6884__INCLUDED_)
