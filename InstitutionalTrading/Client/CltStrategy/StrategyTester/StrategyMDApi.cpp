
#include "stdafx.h"
#include "StrategyMDApi.h"
#include "MD5.h"

CStrategyMdApi* CStrategyMdApi::CreateStrategyMdApi()
{
	return new CStrategyMdApi();
}

CStrategyMdApi::CStrategyMdApi()
{
	m_pBuzSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CStrategyMdApi::~CStrategyMdApi()
{
	CloseHandle(m_mutex);
}

///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void CStrategyMdApi::Release()
{
    //LOG_INFO("CInstitudeTraderApi::Release() Start");
	if(m_pTcpConnect != NULL) {
		m_pTcpConnect->Stop();
		m_pTcpConnect->Release();
		delete m_pTcpConnect;
		m_pTcpConnect = NULL;
	}
    //LOG_INFO("CInstitudeTraderApi::Release() End");
	delete this;
}
	
///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void CStrategyMdApi::Init()
{
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void CStrategyMdApi::RegisterFront(char* pszFrontAddress)
{
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
}

///用户登录请求
int CStrategyMdApi::ReqUserLogin(const string& strUserID, const string& strPasswd, int nRequestID)
{
	m_strUserID = strUserID;
	m_strPasswd = strPasswd;

	sLoginReq sLogin;
	strncpy(sLogin.msUserName, strUserID.c_str(), sizeof(sLogin.msUserName)-1);
	strncpy(sLogin.msPassWord, strPasswd.c_str(), sizeof(sLogin.msPassWord)-1);	
	MD5 md5(sLogin.msPassWord);
	strcpy(sLogin.msPassWord, md5.toString().c_str());
	sLogin.meUserType = USER_TYPE_TRADER;
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Trader_Login_Req, (void*)&sLogin,sizeof(sLogin), 0)?0:999;
	else
		return 999;
}

///登出请求
int CStrategyMdApi::ReqUserLogout(const string& strUserID, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Logoff_Req, NULL, 0, 0)?0:999;
	else
		return 999;
}

///请求查询行情
int CStrategyMdApi::ReqQryDepthMarketData(const string& strMinute, int nRequestID)
{
	if(m_pTcpConnect != NULL) {
		char csText[32];
		memset(csText, 0, sizeof(csText));
		strncpy(csText, strMinute.c_str(), sizeof(csText)-1);
		return m_pTcpConnect->SendData(Cmd_QueryQuotDataEvent_Req, (void*)csText, 
			18, 0, 0, nRequestID) ?0:999;
	}
	else
		return 999;
}

void CStrategyMdApi::OnFrontConnected()
{
	LockMe();
	if(m_pBuzSpi != NULL)
		m_pBuzSpi->OnQuotConnected();
	UnLockMe();
}

void CStrategyMdApi::OnFrontDisconnected()
{
	LockMe();
	if(m_pBuzSpi != NULL)
		m_pBuzSpi->OnQuotDisconnected();
	UnLockMe();
}

void ConvEx2Std(PlatformStru_DepthMarketData_EX& ex, PlatformStru_DepthMarketData& depth)
{
    strncpy(depth.TradingDay, ex.TradingDay,sizeof(depth.TradingDay)-1);
    strncpy(depth.InstrumentID, ex.InstrumentID,sizeof(depth.InstrumentID)-1);
    strncpy(depth.ExchangeID, ex.ExchangeID,sizeof(depth.ExchangeID)-1);
    depth.LastPrice = ex.LastPrice;
    depth.PreSettlementPrice = ex.PreSettlementPrice;
    depth.PreClosePrice = ex.PreClosePrice;
    depth.PreOpenInterest = ex.PreOpenInterest;
    depth.OpenPrice = ex.OpenPrice;
    depth.HighestPrice = ex.HighestPrice;
    depth.LowestPrice = ex.LowestPrice;
    depth.Volume = ex.Volume;
    depth.Turnover = ex.Turnover;
    depth.OpenInterest = ex.OpenInterest;
    depth.ClosePrice = ex.ClosePrice;
    depth.SettlementPrice = ex.SettlementPrice;
    depth.UpperLimitPrice = ex.UpperLimitPrice;
    depth.LowerLimitPrice = ex.LowerLimitPrice;
    strcpy(depth.UpdateTime, ex.UpdateTime);
    depth.UpdateMillisec = ex.UpdateMillisec;
    depth.BidPrice1 = ex.BidPrice1;
    depth.BidVolume1 = ex.BidVolume1;
    depth.AskPrice1 = ex.AskPrice1;
    depth.AskVolume1 = ex.AskVolume1;
    depth.AveragePrice = ex.AveragePrice;
}

// 解析数据，仅在收到tcp数据时调用
BOOL CStrategyMdApi::OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4, char* pSrcPackage)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;
#if 1
	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
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
			m_mapMDQuot[depthData.InstrumentID] = depthData;
			if(m_pBuzSpi != NULL) 
				m_pBuzSpi->OnDepthMarketData(depthData, 0);
			UnLockMe();
		}
	}
		break;
	case Cmd_QueryQuotDataEvent_Rsp:
	{
		PlatformStru_DepthMarketData depth;
		PlatformStru_DepthMarketData_EX temp;
		vector<PlatformStru_DepthMarketData> vecQuot;
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_DepthMarketData_EX)==0) {
				int nItemCount = nDataLen/sizeof(PlatformStru_DepthMarketData_EX);
				if(nItemCount > 0) {
					for(int i=0; i<nItemCount; i++) {
						memcpy(&temp, pData+i*sizeof(temp), sizeof(temp));
						ConvEx2Std(temp, depth);
						vecQuot.push_back(depth);
					}
				}
			}
		}
		LockMe();
		if(m_pBuzSpi!=NULL) {
			m_pBuzSpi->OnQueryQuotData(vecQuot, nUserData1);
		}
		UnLockMe();
	}
		break;
	};
#endif

	//std::map<UINT, std::set<HWND>>::iterator it;
	//std::set<HWND>::iterator itWnd;

	//LockMe();

	//it = m_mapSubs.find(nCmdId);
	//for(itWnd = it->second.begin(); itWnd != it->second.end(); itWnd++) {
	//	if(IsWindow(*itWnd)) {
	//		SendMessage(*itWnd, _GetDataRecvMsgID(), WndUserCmd_YourPkgArrival, (LPARAM)pData);
	//	}
	//}

	//UnLockMe();

	return 0;
}
