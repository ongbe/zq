
#include "stdafx.h"
#include "StrategyTraderApi.h"
#include <stdlib.h>
#include "MD5.h"
#include <string>
#include <set>
#include "Module-Misc2\GLobalFunc.h"
#include "Module-Misc2\GlobalConfigMgr.h"


///创建TraderApi
///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
///@return 创建出的UserApi
//modify for udp marketdata
CStrategyTraderApi* CStrategyTraderApi::CreateStrategyTraderApi() 
{
	return new CStrategyTraderApi();
}

CStrategyTraderApi::CStrategyTraderApi()
{
	m_pBuzSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	ZeroMemory(&m_UserLogin, sizeof(m_UserLogin));

	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CStrategyTraderApi::~CStrategyTraderApi()
{
	CloseHandle(m_mutex);
}

///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void CStrategyTraderApi::Release()
{
	//CTcpLayer::Stop();
	//CTcpLayer::UnRegisterApi(this);
	//CTcpLayer::Release();
    //LOG_INFO("CStrategyTraderApi::Release() Start");
	if(m_pTcpConnect != NULL) {
		m_pTcpConnect->Stop();
		m_pTcpConnect->Release();
		delete m_pTcpConnect;
		m_pTcpConnect = NULL;
	}
    //LOG_INFO("CStrategyTraderApi::Release() End");
	delete this;
}

///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void CStrategyTraderApi::Init()
{
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void CStrategyTraderApi::RegisterFront(char *pszFrontAddress)
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
int CStrategyTraderApi::ReqUserLogin(const string& strUserID, const string& strPasswd, int nRequestID)
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
int CStrategyTraderApi::ReqUserLogout(int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Logoff_Req, NULL, 0, 0)?0:999;
	else
		return 999;
}

///报单录入请求
int CStrategyTraderApi::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeExcute_InputOrder_Req, (void*)pInputOrder, 
			sizeof(*pInputOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///报单操作请求
int CStrategyTraderApi::ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeExcute_CancelOrder_Req, (void*)pInputOrderAction, 
			sizeof(*pInputOrderAction), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询报单
int CStrategyTraderApi::ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID)
{
	//strncpy(pQryOrder->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryOrder->Thost.BrokerID)-1);
	strncpy(pQryOrder->Thost.InvestorID, m_strUserID.c_str(), sizeof(pQryOrder->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryOrder_Req, (void*)pQryOrder, 
			sizeof(*pQryOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询成交
int CStrategyTraderApi::ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID)
{
	//strncpy(pQryTrade->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryTrade->Thost.BrokerID)-1);
	strncpy(pQryTrade->Thost.InvestorID, m_strUserID.c_str(), sizeof(pQryTrade->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryTrade_Req, (void*)pQryTrade, 
			sizeof(*pQryTrade), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询投资者持仓
int CStrategyTraderApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID)
{
	//strncpy(pQryInvestorPosition->BrokerID, m_ReqUser.BrokerID, sizeof(pQryInvestorPosition->BrokerID)-1);
	strncpy(pQryInvestorPosition->InvestorID, m_strUserID.c_str(), sizeof(pQryInvestorPosition->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryPosition_Req, (void*)pQryInvestorPosition, 
			sizeof(*pQryInvestorPosition), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询资金账户
int CStrategyTraderApi::ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID)
{
	strncpy(pQryTradingAccount->BrokerID, "", sizeof(pQryTradingAccount->BrokerID)-1);
	strncpy(pQryTradingAccount->InvestorID, m_strUserID.c_str(), sizeof(pQryTradingAccount->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryTraderAccount_Req, (void*)pQryTradingAccount, 
			sizeof(*pQryTradingAccount), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询合约
int CStrategyTraderApi::ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryInstrument_Req, (void*)pQryInstrument, 
			sizeof(*pQryInstrument), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询行情
int CStrategyTraderApi::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryDepthMarket_Req, (void*)pQryDepthMarketData, 
			sizeof(*pQryDepthMarketData), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询投资者持仓明细
int CStrategyTraderApi::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID)
{
	//strncpy(pQryInvestorPositionDetail->BrokerID, m_ReqUser.BrokerID, sizeof(pQryInvestorPositionDetail->BrokerID)-1);
	strncpy(pQryInvestorPositionDetail->InvestorID, m_strUserID.c_str(), sizeof(pQryInvestorPositionDetail->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryPositionDetail_Req, (void*)pQryInvestorPositionDetail, 
			sizeof(*pQryInvestorPositionDetail), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///请求查询投资者持仓明细
int CStrategyTraderApi::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID)
{
	//strncpy(pQryInvestorPositionCombineDetail->BrokerID, m_ReqUser.BrokerID, sizeof(pQryInvestorPositionCombineDetail->BrokerID)-1);
	strncpy(pQryInvestorPositionCombineDetail->InvestorID, m_strUserID.c_str(), sizeof(pQryInvestorPositionCombineDetail->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryComPositionDetail_Req, (void*)pQryInvestorPositionCombineDetail, 
			sizeof(*pQryInvestorPositionCombineDetail), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

bool CStrategyTraderApi::SubsUserMsg(int nUserID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_UserMsg_SubsMsg_Req, NULL, 0, 0, nUserID) ? true : false;
	else
		return false;
}

// 退订用户消息
bool CStrategyTraderApi::UnsubsUserMsg(int nUserID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_UserMsg_UnSubsMsg_Req, NULL, 0, 0, nUserID) ? true : false;
	else
		return false;
}

int CStrategyTraderApi::ReqUploadStart(UploadStart& start, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMD_UPLOAD_START_Req, (void*)&start, 
			sizeof(start), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

int CStrategyTraderApi::ReqUploadEnd(UploadEnd& end, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMD_UPLOAD_END_Req, (void*)&end, 
			sizeof(end), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

// 请求上传指标基本信息
int CStrategyTraderApi::ReqUploadIndicatorBaseInfo(CIndicatorDataEx& indicatorData, int nRequestID)
{
	std::string strParamsXml;
	SIndex index = {0};

	strncpy(index.IndexName, indicatorData.GetIndicatorName().c_str(), sizeof(index.IndexName)-1);
	index.nPrivate = indicatorData.GetIndicatorPrivate();
////	index.nType = indicatorInfo.eIndiType;
////	strncpy(index.strComment, indicatorInfo.szDescribe, sizeof(index.strComment)-1);
	strncpy(index.strTraderName, m_strUserID.c_str(), sizeof(index.strTraderName)-1);
	indicatorData.MakeXml(strParamsXml);
	strncpy(index.strPara, strParamsXml.c_str(), sizeof(index.strPara)-1);

	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMD_STRTEGY_UPLOADINDEX_Base_Req, (void*)&index, 
			sizeof(index), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

// 请求上传指标DLL或源程序zip
int CStrategyTraderApi::ReqUploadIndicatorFile(const std::string& strIndicatorID, 
											   const std::string& strFile, 
											   int nRequestID, bool bIsDll)
{
	HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
                       GENERIC_READ,						// open for reading
                       FILE_SHARE_READ,						// share for reading
                       NULL,								// default security
                       OPEN_EXISTING,						// existing file only
                       FILE_ATTRIBUTE_NORMAL,				// normal file
                       NULL);								// no attr. template
	if(hFile==INVALID_HANDLE_VALUE) {
		if(bIsDll)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("指标") + strIndicatorID + ".DLL文件不存在，无法上传").c_str());
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("指标") + strIndicatorID + "源程序不存在，无法上传").c_str());
		// 指定的文件打开失败。
		return 9901;
	}
	DWORD dwFileLenHigh = 0;
	DWORD dwFileLen = ::GetFileSize(hFile, &dwFileLenHigh);
	if(dwFileLen==0) {
		::CloseHandle(hFile);
		if(bIsDll)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("指标") + strIndicatorID + ".DLL文件长度为0，无法上传").c_str());
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("指标") + strIndicatorID + "源程序长度为0，无法上传").c_str());
		// 文件长度为0，无法上传。
		return 9902;
	}

	const int nHeadLen = 4+25+25;
	int nRet = 0;
	BYTE* pFileCache = new BYTE[dwFileLen+nHeadLen];
	ZeroMemory(pFileCache, dwFileLen+nHeadLen);

	DWORD dwRealRead = 0;
	dwFileLenHigh = 0;
	if(!::ReadFile(hFile, pFileCache+nHeadLen, dwFileLen, &dwFileLenHigh, NULL)) {
		::CloseHandle(hFile);
		// 文件读失败。
		return 9903;
	}
	::CloseHandle(hFile);

	*(int*)pFileCache = bIsDll ? 2 : 3;
	strncpy((char*)(pFileCache+4), strIndicatorID.c_str(), 25-1);
	strncpy((char*)(pFileCache+4+25), m_strUserID.c_str(), 25-1);

	if(m_pTcpConnect != NULL) 
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_UPLOADINDEX_File_Req, (void*)pFileCache, 
			dwFileLen+nHeadLen, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		nRet = 999;

	delete pFileCache;
	pFileCache = NULL;

	return nRet;
}

// 请求下载指标列表
int CStrategyTraderApi::ReqDownloadIndicatorList(int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char strUserID[32] ={0};
		strncpy(strUserID, m_strUserID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DOWNLOAD_ALLINDEX_Base_Req, strUserID, 
				25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

	// 请求下载指定指标信息
int CStrategyTraderApi::ReqDownloadIndicatorInfo(const std::string& strIndicatorID, int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char strText[32] ={0};
		strncpy(strText, strIndicatorID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DOWNLOADINDEX_Base_Req, strText, 
				25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求下载指标DLL或源程序zip，
// strFile: 文件下载后保存的文件名及路径。
int CStrategyTraderApi::ReqDownloadIndicatorFile(const std::string& strIndicatorID, 
												 int nRequestID, bool bIsDll)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char csUserID[64] ={0};
		*(int*)csUserID = bIsDll ? 2 : 3;
		strncpy(csUserID+sizeof(int), strIndicatorID.c_str(), 25-1);
		strncpy(csUserID+sizeof(int)+25, m_strUserID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DOWNLOADINDEX_Req, (void*)csUserID, 
				sizeof(int)+25+25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 上传策略基本信息
int CStrategyTraderApi::ReqUploadStategyBaseInfo(CStrategyDataEx& strategyData, int nRequestID)
{
	std::string strParamsXml;
	SStrategy strategy = {0};

	strncpy(strategy.strategyName, strategyData.GetStrategyName().c_str(), sizeof(strategy.strategyName)-1);
	strategy.nPrivate = strategyData.GetStrategyPrivate();
	strncpy(strategy.strTraderName, m_strUserID.c_str(), sizeof(strategy.strTraderName)-1);
	strategyData.MakeXml(strParamsXml);
	strncpy(strategy.strPara, strParamsXml.c_str(), sizeof(strategy.strPara)-1);

	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMD_STRTEGY_UPLOAD_Base_Req, (void*)&strategy, 
			sizeof(strategy), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

// 上传策略DLL或源程序zip
int CStrategyTraderApi::ReqUploadStategyFile(const std::string& strStragetyID, 
											 const std::string& strFile, 
											 int nRequestID, bool bIsDll)
{
	HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
                       GENERIC_READ,						// open for reading
                       FILE_SHARE_READ,						// share for reading
                       NULL,								// default security
                       OPEN_EXISTING,						// existing file only
                       FILE_ATTRIBUTE_NORMAL,				// normal file
                       NULL);								// no attr. template
	if(hFile==INVALID_HANDLE_VALUE) {
		// 指定的文件打开失败。
		if(bIsDll)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("策略") + strStragetyID + ".DLL文件不存在，无法上传").c_str());
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("策略") + strStragetyID + "源程序不存在，无法上传").c_str());
		return 9901;
	}
	DWORD dwFileLenHigh = 0;
	DWORD dwFileLen = ::GetFileSize(hFile, &dwFileLenHigh);
	if(dwFileLen==0) {
		::CloseHandle(hFile);
		if(bIsDll)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("策略") + strStragetyID + ".DLL文件长度为0，无法上传").c_str());
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, (std::string("策略") + strStragetyID + "源程序长度为0，无法上传").c_str());
		// 文件长度为0，无法上传。
		return 9902;
	}

	const int nHeadLen = 4+25+25;
	int nRet = 0;
	BYTE* pFileCache = new BYTE[dwFileLen+nHeadLen];
	ZeroMemory(pFileCache, dwFileLen+nHeadLen);

	DWORD dwRealRead = 0;
	dwFileLenHigh = 0;
	if(!::ReadFile(hFile, pFileCache+nHeadLen, dwFileLen, &dwFileLenHigh, NULL)) {
		::CloseHandle(hFile);
		// 文件读失败。
		return 9903;
	}
	::CloseHandle(hFile);

	*(int*)pFileCache = bIsDll ? 2 : 3;
	strncpy((char*)(pFileCache+4), strStragetyID.c_str(), 25-1);
	strncpy((char*)(pFileCache+4+25), m_strUserID.c_str(), 25-1);

	if(m_pTcpConnect != NULL) 
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_UPLOAD_File_Req, (void*)pFileCache, 
			dwFileLen+nHeadLen, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		nRet = 999;

	delete pFileCache;
	pFileCache = NULL;

	return nRet;
}

// 请求下载策略列表
int CStrategyTraderApi::ReqDownloadStategyList(int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char strUserID[32] ={0};
		strncpy(strUserID, m_strUserID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DOWNLOAD_All_Base_Req, strUserID, 
				25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求下载指定策略信息
int CStrategyTraderApi::ReqDownloadStategyInfo(const std::string& strStrategyID, int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char strText[32] ={0};
		strncpy(strText, strStrategyID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DOWNLOAD_Base_Req, strText, 
				25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求下载策略DLL或源程序zip，
// strFile: 文件下载后保存的文件名及路径。
int CStrategyTraderApi::ReqDownloadStategyFile(const std::string& strStragetyID, 
											   int nRequestID, bool bIsDll)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char csUserID[64] ={0};
		*(int*)csUserID = bIsDll ? 2 : 3;
		strncpy(csUserID+sizeof(int), strStragetyID.c_str(), 25-1);
		strncpy(csUserID+sizeof(int)+25, m_strUserID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DOWNLOAD_Req, (void*)csUserID, 
				sizeof(int)+25+25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求设置策略的启用禁用状态
int CStrategyTraderApi::ReqResetStrategyUseStatus(const std::string& strID, int nStatus, int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		SUseStrategy use = {0};
		use.nUserType = 2;
		strncpy(use.strategyName, strID.c_str(), sizeof(use.strategyName)-1);
		strncpy(use.strTraderName, m_strUserID.c_str(), sizeof(use.strTraderName)-1);
		use.nStatus = nStatus;
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_USE_Req, (void*)&use, 
				sizeof(use), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求删除策略
int CStrategyTraderApi::ReqDeleteStrategy(const std::string& strID, int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		SDelStrategy del = {0};
		del.nUserType = 2;
		strncpy(del.strategyName, strID.c_str(), sizeof(del.strategyName)-1);
		strncpy(del.strTraderName, m_strUserID.c_str(), sizeof(del.strTraderName)-1);
		nRet = m_pTcpConnect->SendData(CMD_STRTEGY_DEL_Req, (void*)&del, 
				sizeof(del), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求下载当前交易员所有的策略运行方案
int CStrategyTraderApi::ReqDownloadStrategyInstanceList(int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		char strUserID[64] = {0};
		strncpy(strUserID, m_strUserID.c_str(), 25-1);
		nRet = m_pTcpConnect->SendData(CMD_INSTANCE_AllDownlaod_Req, (void*)strUserID, 
				25, 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求添加一个策略运行方案
int CStrategyTraderApi::ReqAddStrategyInstance(std::string& strStrategyID, 
						CStrategyDataEx& strategyData, const std::set<std::string>& setInstrument, 
						bool bRunAfterLoad, bool bOfflinePause, bool bStopThenCancelOrder, 
						bool bUseTimer, int nTimerInterval, 
						int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		std::set<std::string>::const_iterator it;
		int i=0;
		std::string strInstrument;
		std::string strParam;
		SStrategyInstance instance = {0};
		strategyData.MakeXml(strParam);
		for(it = setInstrument.begin(); it != setInstrument.end(); it++, i++) {
			strInstrument += (*it);
			if(i < setInstrument.size()-1)
				strInstrument += ",";
		}
		strncpy(instance.Instruments, strInstrument.c_str(), sizeof(instance.Instruments)-1);
		strncpy(instance.strategyName, strStrategyID.c_str(), sizeof(instance.strategyName)-1);
		strncpy(instance.strTraderName, m_strUserID.c_str(), sizeof(instance.strTraderName)-1);
		instance.nRunAfterLoad = bRunAfterLoad ? 1 : 0;
		instance.nRunAfterOffline = bOfflinePause ? 0 : 1;
		instance.nOrderActionBeforeStop = bStopThenCancelOrder ? 1 : 0;
		strncpy(instance.strPara, strParam.c_str(), sizeof(instance.strPara)-1);
		bUseTimer;
		nTimerInterval;
		instance.nStart = 2;
		nRet = m_pTcpConnect->SendData(CMD_INSTANCE_ADD_Req, (void*)&instance, 
				sizeof(instance), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求添加一个策略运行方案
int CStrategyTraderApi::ReqAddStrategyInstance(std::string& strStrategyID, SStrategyInstance& instance, 
						int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		strncpy(instance.strTraderName, m_strUserID.c_str(), sizeof(instance.strTraderName)-1);
		instance.nStart = instance.nRunAfterLoad==0 ? 2 : 1;
		nRet = m_pTcpConnect->SendData(CMD_INSTANCE_ADD_Req, (void*)&instance, 
				sizeof(instance), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求重置一个策略运行方案
int CStrategyTraderApi::ReqResetStrategyInstance(std::string& strStrategyID, SStrategyInstance& instance, 
						int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		instance.nStart = 2;
		nRet = m_pTcpConnect->SendData(CMD_INSTANCE_Modify_Req, (void*)&instance, 
				sizeof(instance), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求删除一个策略运行方案
int CStrategyTraderApi::ReqDeleteStrategyInstance(const std::string& strID, int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		SDelStrategy del = {0};
		del.nUserType = 2;
		strncpy(del.strategyName, strID.c_str(), sizeof(del.strategyName)-1);
		strncpy(del.strTraderName, m_strUserID.c_str(), sizeof(del.strTraderName)-1);
		nRet = m_pTcpConnect->SendData(CMD_INSTANCE_DEL_Req, (void*)&del, 
				sizeof(del), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

// 请求启动或停止一个策略运行方案
int CStrategyTraderApi::ReqStrategyInstance_RunStop(const std::string& strID, bool bRun, int nRequestID)
{
	int nRet = 0;
	if(m_pTcpConnect != NULL) {
		SUseStrategy use = {0};
		use.nUserType = 2;
		strncpy(use.strategyName, strID.c_str(), sizeof(use.strategyName)-1);
		strncpy(use.strTraderName, m_strUserID.c_str(), sizeof(use.strTraderName)-1);
		use.nStatus = bRun ? 1 : 0;
		nRet = m_pTcpConnect->SendData(CMD_INSTANCE_USE_Req, (void*)&use, 
				sizeof(use), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	}
	else
		nRet = 999;
	return nRet;
}

void CStrategyTraderApi::OnFrontConnected()
{
	LockMe();
	if(m_pBuzSpi != NULL)
		m_pBuzSpi->OnTraderConnected();
	UnLockMe();
	//ReqUserLogin(std::string("888001"), std::string("888888"), 0);
}

void CStrategyTraderApi::OnFrontDisconnected()
{
	LockMe();
	if(m_pBuzSpi != NULL)
		m_pBuzSpi->OnTraderDisconnected();
	UnLockMe();
}

// 解析数据，仅在收到tcp数据时调用
BOOL CStrategyTraderApi::OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4, char* pSrcPackage)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;

	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
	case CMDID_Trader_Login_Rsp:
	{
		sTraderLoginRsp loginRsp;
		ZeroMemory(&loginRsp, sizeof(loginRsp));

		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(sTraderLoginRsp)) {

				memcpy(&loginRsp, pData, sizeof(loginRsp));
				m_UserLogin = loginRsp;

				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnTraderLogin(loginRsp, 0, std::string(""));
				UnLockMe();
				
				//SubsUserMsg(m_UserLogin.mnUserID);
			}
		}
		else {
			if(pData[nDataLen-1]==0) {
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnTraderLogin(loginRsp, nUserData1, std::string(pData));
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryInstrument_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentInfo)==0) {
				std::vector<PlatformStru_InstrumentInfo> vecInstr;
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentInfo);
				if(nItemCount > 0) {
					PlatformStru_InstrumentInfo instInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&instInfo, pData+i*sizeof(instInfo), sizeof(instInfo));
						vecInstr.push_back(instInfo);
						LockMe();
						m_mapInstrument[instInfo.InstrumentID] = instInfo;
						UnLockMe();
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnInstrument(vecInstr, 0);
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryTraderAccount_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_TradingAccountInfo)==0) {
				int nItemCount = nDataLen/sizeof(PlatformStru_TradingAccountInfo);
				PlatformStru_TradingAccountInfo fundInfo;
				if(nItemCount > 0) {
					for(int i=0; i<nItemCount; i++) {
						memcpy(&fundInfo, pData+i*sizeof(fundInfo), sizeof(fundInfo));
						//strncpy(fundInfo.BrokerID, m_ReqUser.BrokerID, sizeof(fundInfo.BrokerID)-1);
						strncpy(fundInfo.AccountID, m_strUserID.c_str(), sizeof(fundInfo.AccountID)-1);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnTradingAccount(fundInfo, 0);
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryTrade_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_TradeInfo)==0) {
				int nItemCount = nDataLen/sizeof(PlatformStru_TradeInfo);
				std::vector<PlatformStru_TradeInfo> vecTrade;
				if(nItemCount > 0) {
					PlatformStru_TradeInfo tradeInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&tradeInfo, pData+i*sizeof(tradeInfo), sizeof(tradeInfo));
						vecTrade.push_back(tradeInfo);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnTrades(vecTrade, 0);
				}
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryOrder_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_OrderInfo)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_OrderInfo);
				std::vector<PlatformStru_OrderInfo> vecOrder;
				if(nItemCount > 0) {
					PlatformStru_OrderInfo orderInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&orderInfo, pData+i*sizeof(orderInfo), sizeof(orderInfo));
						vecOrder.push_back(orderInfo);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnOrders(vecOrder, 0);
				}
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryPositionDetail_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_PositionDetail)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_PositionDetail);
				std::vector<PlatformStru_PositionDetail> vecDetail;
				if(nItemCount > 0) {
					PlatformStru_PositionDetail detailInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&detailInfo, pData+i*sizeof(detailInfo), sizeof(detailInfo));
						vecDetail.push_back(detailInfo);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnPositionDetails(vecDetail, 0);
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryPosition_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_Position)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_Position);
				std::vector<PlatformStru_Position> vecPosition;
				if(nItemCount > 0) {
					PlatformStru_Position posiInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&posiInfo, pData+i*sizeof(posiInfo), sizeof(posiInfo));
						vecPosition.push_back(posiInfo);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnPositions(vecPosition, 0);
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryComPositionDetail_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_PositionDetailComb)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_PositionDetailComb);
				std::vector<PlatformStru_PositionDetailComb> vecDComb;
				if(nItemCount > 0) {
					PlatformStru_PositionDetailComb detailCombInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&detailCombInfo, pData+i*sizeof(detailCombInfo), sizeof(detailCombInfo));
						vecDComb.push_back(detailCombInfo);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnPositionDetailCombs(vecDComb, 0);
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryCommissionRat_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentCommissionRate)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentCommissionRate);
				if(nItemCount > 0) {
					PlatformStru_InstrumentCommissionRate csrInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&csrInfo, pData+i*sizeof(csrInfo), sizeof(csrInfo));
						LockMe();
						if(m_pBuzSpi!=NULL)
							m_pBuzSpi->OnCommissionRate(csrInfo, 0);
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryMarginRate_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentMarginRate)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentMarginRate);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_InstrumentMarginRate mgrInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&mgrInfo, pData+i*sizeof(mgrInfo), sizeof(mgrInfo));
						LockMe();
						if(m_pBuzSpi!=NULL)
							m_pBuzSpi->OnMarginRate(mgrInfo, 0);
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeExcute_Trader_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(PlatformStru_TradeInfo)) {
				int nItemCount = nDataLen/sizeof(PlatformStru_TradeInfo);
				if(nItemCount > 0) {
					PlatformStru_TradeInfo tradeInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&tradeInfo, pData+i*sizeof(tradeInfo), sizeof(tradeInfo));
						LockMe();
						if(m_pBuzSpi!=NULL) 
							m_pBuzSpi->OnTrade(tradeInfo, 0);
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeExcute_OrderStatus_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(PlatformStru_OrderInfo)) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_OrderInfo);
				if(nItemCount > 0) {
					PlatformStru_OrderInfo orderInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&orderInfo, pData+i*sizeof(orderInfo), sizeof(orderInfo));

						LockMe();
						if(m_pBuzSpi!=NULL) {
							m_pBuzSpi->OnOrder(orderInfo, 0);
						}
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeExcute_CancelOrder_Fail_RSP:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==(sizeof(PlatformStru_InputOrderAction)+sizeof(CThostFtdcRspInfoField))) {
			
				PlatformStru_InputOrderAction iaInfo;

				memcpy(&iaInfo, pData, sizeof(iaInfo));
				memcpy(&rspInfo, pData+sizeof(iaInfo), sizeof(rspInfo));
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnOrderActionFail(iaInfo, 0);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeExcute_CancelOrder_Error_RSP:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(CThostFtdcOrderActionField)+sizeof(CThostFtdcRspInfoField)) {
			
				CThostFtdcOrderActionField oaInfo;

				memcpy(&oaInfo, pData, sizeof(oaInfo));
				memcpy(&rspInfo, pData+sizeof(oaInfo), sizeof(rspInfo));
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnOrderActionError(oaInfo, 0);
				UnLockMe();
			}
		}
		break;
	//case Cmd_TradeDataManager_QryParkerOrder_Rsp:
	//	if(nUserData3 == CF_ERROR_SUCCESS) {
	//		if(nDataLen==0) {
	//		//if(nDataLen==0 || nDataLen==sizeof(CThostFtdcContractBankField)+sizeof(CThostFtdcRspInfoField)) {
	//			//LockMe();
	//			//if(m_pSpi!=NULL)
	//			//	m_pSpi->OnRspQryParkedOrder(NULL, &rspInfo, nUserData2, true);
	//			//UnLockMe();
	//		}
	//	}
	//	break;
	//case Cmd_TradeDataManager_QryParkerOrderAction_Rsp:
	//	if(nUserData3 == CF_ERROR_SUCCESS) {
	//		if(nDataLen==0) {
	//		//if(nDataLen==0 || nDataLen==sizeof(CThostFtdcContractBankField)+sizeof(CThostFtdcRspInfoField)) {
	//			//LockMe();
	//			//if(m_pSpi!=NULL)
	//			//	m_pSpi->OnRspQryParkedOrderAction(NULL, &rspInfo, nUserData2, true);
	//			//UnLockMe();
	//		}
	//	}
	//	break;
	case Cmd_TradeDataManager_QryDepthMarket_Rsp:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_DepthMarketData)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_DepthMarketData);
				if(nItemCount > 0) {
					PlatformStru_DepthMarketData depth;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&depth, pData+i*sizeof(depth), sizeof(depth));
						LockMe();
						if(m_pBuzSpi!=NULL) {
							m_pBuzSpi->OnDepthMarketData(depth, 0);
						}
						UnLockMe();
					}
				}
			}
		}
		break;
	case CMD_UPLOAD_START_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnUploadStartRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_UPLOAD_END_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnUploadEndRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_STRTEGY_UPLOADINDEX_Base_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnUploadIndicatorInfoRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_STRTEGY_UPLOADINDEX_File_Rsp:
		{
		LockMe();
		int nFileType = 0;
		char csStrategy[32] = {0};
		char csUserID[32] = {0};
		if(m_pBuzSpi!=NULL && (nUserData1 != CF_ERROR_SUCCESS || nUserData1 == CF_ERROR_SUCCESS && nDataLen == sizeof(int)+25+25)) {
			std::string strMsg;
			if(nUserData1 == CF_ERROR_SUCCESS) {
				memcpy(&nFileType, pData, sizeof(nFileType));
				memcpy(csStrategy, pData+4, sizeof(csStrategy));
				memcpy(csUserID, pData+4+25, sizeof(csUserID));
			}
			else
				strMsg = pData;
			m_pBuzSpi->OnUploadIndicatorFileRsp(nUserData1, nFileType, strMsg);
		}
		UnLockMe();
		}
		break;
	case CMD_STRTEGY_DOWNLOAD_ALLINDEX_Base_Rsp:
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(SIndex)==0) {
			
				int nItemCount = nDataLen/sizeof(SIndex);
				std::vector<SIndex> vecRet;
				if(nItemCount > 0) {
					SIndex indicator;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&indicator, pData+i*sizeof(indicator), sizeof(indicator));
						vecRet.push_back(indicator);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadIndicatorListRsp(vecRet, 0, std::string(""));
				}
				UnLockMe();
			}
		}
		break;
	case CMD_STRTEGY_DOWNLOADINDEX_Base_Rsp:
	{
		SIndex indicator;
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(SIndex)) {
				memcpy(&indicator, pData, sizeof(indicator));
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadIndicatorInfoRsp(indicator, 0, std::string(""));
				}
				UnLockMe();
			}
		}
		else {
			LockMe();
			if(m_pBuzSpi!=NULL) {
				std::string strMsg;
				if(nUserData1!=0)
					strMsg = pData;
				m_pBuzSpi->OnDownloadIndicatorInfoRsp(indicator, nUserData1, strMsg);
			}
			UnLockMe();
		}
	}
		break;
	case CMD_STRTEGY_DOWNLOADINDEX_Rsp:
	{
		int nFileType = 0;
		char csIndicator[32] = {0};
		char csUserID[32] = {0};
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen > sizeof(int)+25+25) {
				memcpy(&nFileType, pData, sizeof(nFileType));
				memcpy(csIndicator, pData+4, sizeof(csIndicator));
				memcpy(csUserID, pData+4+25, sizeof(csUserID));
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadIndicatorFileRsp(std::string(csIndicator), 
							nFileType, pData+4+25+25, nDataLen-(4+25+25), 0, std::string(""));
				}
				UnLockMe();
			}
		}
		else {
			LockMe();
			if(m_pBuzSpi!=NULL) {
				std::string strMsg;
				if(nUserData1!=0)
					strMsg = pData;
				m_pBuzSpi->OnDownloadIndicatorFileRsp(std::string(csIndicator), 
							nFileType, NULL, 0, nUserData1, strMsg);
			}
			UnLockMe();
		}
	}
		break;
	case CMD_STRTEGY_UPLOAD_Base_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnUploadStrategyInfoRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_STRTEGY_UPLOAD_File_Rsp:
		{
		LockMe();
		int nFileType = 0;
		char csStrategy[32] = {0};
		char csUserID[32] = {0};
		if(m_pBuzSpi!=NULL && (nUserData1 != CF_ERROR_SUCCESS || nUserData1 == CF_ERROR_SUCCESS && nDataLen == sizeof(int)+25+25)) {
			std::string strMsg;
			if(nUserData1 == CF_ERROR_SUCCESS) {
				memcpy(&nFileType, pData, sizeof(nFileType));
				memcpy(csStrategy, pData+4, sizeof(csStrategy));
				memcpy(csUserID, pData+4+25, sizeof(csUserID));
			}
			else
				strMsg = pData;
			m_pBuzSpi->OnUploadStrategyFileRsp(nUserData1, nFileType, strMsg);
		}
		UnLockMe();
		}
		break;
	case CMD_STRTEGY_DOWNLOAD_All_Base_Rsp:
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(SStrategy)==0) {
			
				std::vector<SStrategy> vecRet;
				int nItemCount = nDataLen/sizeof(SStrategy);
				if(nItemCount > 0) {
					SStrategy strategy;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&strategy, pData+i*sizeof(strategy), sizeof(strategy));
						vecRet.push_back(strategy);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadStrategyListRsp(vecRet, 0, std::string(""));
				}
				UnLockMe();
			}
		}
		break;
	case CMD_STRTEGY_DOWNLOAD_Base_Rsp:
	{
		SStrategy strategy;
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(SStrategy)) {
				memcpy(&strategy, pData, sizeof(strategy));
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadStrategyInfoRsp(strategy, 0, std::string(""));
				}
				UnLockMe();
			}
		}
		else {
			LockMe();
			if(m_pBuzSpi!=NULL) {
				std::string strMsg;
				if(nUserData1!=0)
					strMsg = pData;
				m_pBuzSpi->OnDownloadStrategyInfoRsp(strategy, nUserData1, strMsg);
			}
			UnLockMe();
		}
	}
		break;
	case CMD_STRTEGY_DOWNLOAD_Rsp:
	{
		int nFileType = 0;
		char csStrategy[32] = {0};
		char csUserID[32] = {0};
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen > sizeof(int)+25+25) {
				memcpy(&nFileType, pData, sizeof(nFileType));
				memcpy(csStrategy, pData+4, sizeof(csStrategy));
				memcpy(csUserID, pData+4+25, sizeof(csUserID));
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadStrategyFileRsp(std::string(csStrategy), 
							nFileType, pData+4+25+25, nDataLen-(4+25+25), 0, std::string(""));
				}
				UnLockMe();
			}
		}
		else {
			LockMe();
			if(m_pBuzSpi!=NULL) {
				std::string strMsg;
				if(nUserData1!=0)
					strMsg = pData;
				m_pBuzSpi->OnDownloadStrategyFileRsp(std::string(csStrategy), 
							nFileType, NULL, 0, nUserData1, strMsg);
			}
			UnLockMe();
		}
	}
		break;
	case CMD_STRTEGY_USE_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnResetStrategyUseStatusRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_STRTEGY_DEL_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnDeleteStrategyRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_INSTANCE_AllDownlaod_Rsp:
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(SStrategyInstance)==0) {
			
				std::vector<SStrategyInstance> vecRet;
				int nItemCount = nDataLen/sizeof(SStrategyInstance);
				if(nItemCount > 0) {
					SStrategyInstance instance;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&instance, pData+i*sizeof(instance), sizeof(instance));
						vecRet.push_back(instance);
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL) {
					m_pBuzSpi->OnDownloadStrategyInstanceListRsp(vecRet, 0, std::string(""));
				}
				UnLockMe();
			}
		}
		break;
	case CMD_INSTANCE_ADD_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnStrategyInstanceAddRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_INSTANCE_Modify_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnStrategyInstanceResetRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_INSTANCE_DEL_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnStrategyInstanceDeleteRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	case CMD_INSTANCE_USE_Rsp:
		LockMe();
		if(m_pBuzSpi!=NULL) {
			std::string strMsg;
			if(nUserData1!=0)
				strMsg = pData;
			m_pBuzSpi->OnStrategyInstanceRunStopRsp(nUserData1, strMsg);
		}
		UnLockMe();
		break;
	};

	return 0;
}
