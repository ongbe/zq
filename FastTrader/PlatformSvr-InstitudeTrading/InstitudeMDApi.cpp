
#include "stdafx.h"
#include "InstitudeMDApi.h"

CInstitudeMdApi* CInstitudeMdApi::CreateInstitudeMdApi(const char *pszFlowPath, const bool bIsUsingUdp)
{
	return new CInstitudeMdApi();
}

CInstitudeMdApi::CInstitudeMdApi()
{
	m_pSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	m_eventExit = ::CreateEvent(NULL, TRUE, FALSE, "");
	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CInstitudeMdApi::~CInstitudeMdApi()
{
	CloseHandle(m_eventExit);
	CloseHandle(m_mutex);
}

///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void CInstitudeMdApi::Release()
{
    //LOG_INFO("CInstitudeTraderApi::Release() Start");
	if(m_pTcpConnect != NULL) {
		m_pTcpConnect->Stop();
		m_pTcpConnect->Release();
		delete m_pTcpConnect;
		m_pTcpConnect = NULL;
	}
    //LOG_INFO("CInstitudeTraderApi::Release() End");
	SetEvent(m_eventExit);
	delete this;
}
	
///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void CInstitudeMdApi::Init()
{
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
	ResetEvent(m_eventExit);
}
	
///等待接口线程结束运行
///@return 线程退出代码
int CInstitudeMdApi::Join()
{
	WaitForSingleObject(m_eventExit, INFINITE);
	return 0;
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void CInstitudeMdApi::RegisterFront(char* pszFrontAddress)
{
	//if(strstr(pszFrontAddress, "tcp://")==NULL)
	if(strncmp(pszFrontAddress, "tcp://", strlen("tcp://"))!=0)
		return;

	char strAddr[128];
	char strPort[8];

	ZeroMemory(strAddr, sizeof(strAddr));
	ZeroMemory(strPort, sizeof(strPort));
	sscanf_s(pszFrontAddress+strlen("tcp://"), "%[^:]:%s", strAddr, sizeof(strAddr)-1, 
			strPort, sizeof(strPort)-1);
	m_strSvrIP = strAddr;
	m_wSvrPort = atoi(strPort);
	//CTcpLayer::SetSvrAddr(string(strAddr), atoi(strPort));
}
	
///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CInstitudeMdApi::RegisterSpi(CQuotSpi* pSpi)
{
	LockMe();
	m_pSpi = pSpi;
	UnLockMe();
}
	
///订阅行情。
///@param ppInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int CInstitudeMdApi::SubscribeMarketData(char* ppInstrumentID[], int nCount)
{
	if(m_pTcpConnect != NULL) {
		for(int i=0; i<nCount; i++) {
			m_pTcpConnect->SendData(Cmd_SubscribeQuotEvent_Req, (void*)ppInstrumentID[i], 
					strlen(ppInstrumentID[i])+1, 0, 0, 0);
		}
		return 0;
	}
	else
		return 999;
}

///退订行情。
///@param ppInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int CInstitudeMdApi::UnSubscribeMarketData(char* ppInstrumentID[], int nCount)
{
	if(m_pTcpConnect != NULL) {
		for(int i=0; i<nCount; i++) {
			m_pTcpConnect->SendData(Cmd_UnSubscribeQuotEvent_Req, (void*)ppInstrumentID[i], 
					strlen(ppInstrumentID[i])+1, 0, 0, 0);
		}
		return 0;
	}
	else
		return 999;
}

///用户登录请求
int CInstitudeMdApi::ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID)
{
	CThostFtdcRspUserLoginField m_tRspUserLogin = {0};
	CThostFtdcRspInfoField m_tpRspInfo = {0};
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnRspUserLogin(&m_tRspUserLogin, &m_tpRspInfo, nRequestID, true);
	UnLockMe();
	return 0;
}
	
///登出请求
int CInstitudeMdApi::ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID)
{
	CThostFtdcUserLogoutField m_tRspUserLogout = {0};
	CThostFtdcRspInfoField m_tpRspInfo = {0};
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnRspUserLogout(&m_tRspUserLogout, &m_tpRspInfo, nRequestID, true);
	UnLockMe();
	return 0;
}

void CInstitudeMdApi::OnFrontConnected()
{
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnFrontConnected();
	UnLockMe();
}

void CInstitudeMdApi::OnFrontDisconnected()
{
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnFrontDisconnected(0x2001);
	UnLockMe();
}

// 解析数据，仅在收到tcp数据时调用
BOOL CInstitudeMdApi::ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;
#if 1
	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
	case Cmd_SubscribeQuotEvent_Rsp:
		break;
	case Cmd_UnSubscribeQuotEvent_Rsp:
		break;

	case Cmd_QuotEvent_Push:
	{
		if(nDataLen==sizeof(PlatformStru_DepthMarketData)) {

			PlatformStru_DepthMarketData depthData;
			ZeroMemory(&depthData, sizeof(depthData));

			memcpy(&depthData, pData, sizeof(depthData));

			//TRACE("%s, TradingDay := %s, UpdateTime := %s\n", 
			//		depthData.InstrumentID, 
			//		depthData.TradingDay, depthData.UpdateTime);

			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRtnDepthMarketData(&depthData);
			UnLockMe();
		}
	}
		break;
	};
#endif
	return 0;
}
