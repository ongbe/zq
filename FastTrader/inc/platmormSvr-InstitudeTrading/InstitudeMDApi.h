#pragma once

#include "ctp/ThostFtdcMdApi.h"
#include "InstitudeApi.h"
#include "QuotSpi.h"
//#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"

class CQuotSpi;

class CInstitudeMdApi : public CInstitudeApi
{
public:
	static CInstitudeMdApi *CreateInstitudeMdApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
    
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	void Release();
	
	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	void Init();
	
	///等待接口线程结束运行
	///@return 线程退出代码
	int Join();
	
	///获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	const char* GetTradingDay() {
		return "";
	};
	
	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	void RegisterFront(char* pszFrontAddress);
	
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterSpi(CQuotSpi* pSpi);
	
	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int SubscribeMarketData(char* ppInstrumentID[], int nCount);

	///退订行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int UnSubscribeMarketData(char* ppInstrumentID[], int nCount);

	///用户登录请求
	int ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID);
	
	///登出请求
	int ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID);

	void OnFrontConnected();
	void OnFrontDisconnected();
	// 解析数据，仅在收到tcp数据时调用
	BOOL ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4);

protected:
    CInstitudeMdApi();
	virtual ~CInstitudeMdApi();

	void LockMe() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockMe() {
		::ReleaseMutex(m_mutex);
	};


protected:
    CInstitudeMdApi(const CInstitudeMdApi&);
    CInstitudeMdApi& operator= (const CInstitudeMdApi&);

protected:
	HANDLE							m_eventExit;
	HANDLE							m_mutex;
	CQuotSpi*						m_pSpi;
	CTcpDataConnect*				m_pTcpConnect;
	string							m_strSvrIP;
	WORD							m_wSvrPort;
	CThostFtdcReqUserLoginField		m_ReqUser;
};
