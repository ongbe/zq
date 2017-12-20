#pragma once

#include "TcpConnectSpi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TcpDataConnect.h"
#include "BaseBusinessSpi.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"


class CStrategyMdApi : public CTcpConnectSpi
{
public:
	static CStrategyMdApi *CreateStrategyMdApi();
    
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	void Release();
	
	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	void Init();

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	void RegisterFront(char* pszFrontAddress);
	
	///用户登录请求
	int ReqUserLogin(const string& strUserID, const string& strPasswd, int nRequestID);	

	///登出请求
	int ReqUserLogout(const string& strUserID, int nRequestID);

	int ReqQryDepthMarketData(const string& strMinute, int nRequestID);
		
	void OnFrontConnected();
	void OnFrontDisconnected();

	// 解析数据，仅在收到tcp数据时调用
	BOOL OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4, char* pSrcPackage);

public:
	void SetBuzSpi(CBaseBusinessSpi* pBuzSpi) {
		LockMe();
		m_pBuzSpi = pBuzSpi;
		UnLockMe();
	};
	CBaseBusinessSpi* GetBuzSpi() {
		LockMe();
		CBaseBusinessSpi* pSpi = m_pBuzSpi;
		UnLockMe();
		return pSpi;
	};

protected:
    CStrategyMdApi();
	virtual ~CStrategyMdApi();

	void LockMe() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockMe() {
		::ReleaseMutex(m_mutex);
	};

protected:
	HANDLE							m_mutex;
	CTcpDataConnect*				m_pTcpConnect;
	CBaseBusinessSpi*				m_pBuzSpi;
	string							m_strSvrIP;
	WORD							m_wSvrPort;
	string							m_strUserID;
	string							m_strPasswd;

	std::map<std::string, PlatformStru_DepthMarketData>
									m_mapMDQuot;
};
