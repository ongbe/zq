#pragma once

#include "TcpConnectSpi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TcpDataConnect.h"
#include "BaseBusinessSpi.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "..\CommonClasses\IndicatorDataEx.h"
#include "..\CommonClasses\StrategyDataEx.h"


class CStrategyTraderApi : public CTcpConnectSpi
{
public:
	///创建StrategyTraderApi
	static CStrategyTraderApi* CreateStrategyTraderApi();
	
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	void Release();
	
	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	void Init();
		
	///获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	const string GetTradingDay() {
		return m_strTradingDay;
	};
	
	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	void RegisterFront(char *pszFrontAddress);
	
	/////注册回调接口
	/////@param pSpi 派生自回调接口类的实例
	//void RegisterSpi(CTraderSpi* pSpi);
	
	///用户登录请求
	int ReqUserLogin(const string& strUserID, const string& strPasswd, int nRequestID);	

	///登出请求
	int ReqUserLogout(int nRequestID);

	///报单录入请求
	int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID);
	
	///报单操作请求
	int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID);

	///请求查询报单
	int ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID);

	///请求查询成交
	int ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID);

	///请求查询投资者持仓
	int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID);

	///请求查询资金账户
	int ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID);

	///请求查询合约
	int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID);

	///请求查询行情
	int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID);
	
	// 订阅用户消息
	bool SubsUserMsg(int nUserID);

	// 退订用户消息
	bool UnsubsUserMsg(int nUserID);

	int ReqUploadStart(UploadStart& start, int nRequestID);
	int ReqUploadEnd(UploadEnd& end, int nRequestID);

	// 请求上传指标基本信息
	int ReqUploadIndicatorBaseInfo(CIndicatorDataEx& indicatorData, int nRequestID);
	// 请求上传指标DLL或源程序zip
	int ReqUploadIndicatorFile(const std::string& strIndicatorID, const std::string& strFile, 
			int nRequestID, bool bIsDll);

	// 请求下载指标列表
	int ReqDownloadIndicatorList(int nRequestID);
	// 请求下载指定指标信息
	int ReqDownloadIndicatorInfo(const std::string& strIndicatorID, int nRequestID);
	// 请求下载指标DLL或源程序zip，
	// strFile: 文件下载后保存的文件名及路径。
	int ReqDownloadIndicatorFile(const std::string& strIndicatorID, 
			int nRequestID, bool bIsDll);

	// 请求上传策略基本信息
	int ReqUploadStategyBaseInfo(CStrategyDataEx& strategyData, int nRequestID);
	// 请求上传策略DLL或源程序zip
	int ReqUploadStategyFile(const std::string& strStragetyID, const std::string& strFile, 
			int nRequestID, bool bIsDll);

	// 请求下载策略列表
	int ReqDownloadStategyList(int nRequestID);
	// 请求下载指定策略信息
	int ReqDownloadStategyInfo(const std::string& strStrategyID, int nRequestID);
	// 请求下载策略DLL或源程序zip，
	// strFile: 文件下载后保存的文件名及路径。
	int ReqDownloadStategyFile(const std::string& strStragetyID, 
			int nRequestID, bool bIsDll);

	// 请求设置策略的启用禁用状态
	int ReqResetStrategyUseStatus(const std::string& strID, int nStatus, int nRequestID);

	// 请求删除策略
	int ReqDeleteStrategy(const std::string& strID, int nRequestID);

	// 请求下载当前交易员所有的策略运行方案
	int ReqDownloadStrategyInstanceList(int nRequestID);

	// 请求添加一个策略运行方案
	int ReqAddStrategyInstance(std::string& strStrategyID, CStrategyDataEx& strategyData, 
			const std::set<std::string>& setInstrument, 
			bool bRunAfterLoad, bool bOfflinePause, bool bStopThenCancelOrder, 
			bool bUseTimer, int nTimerInterval, int nRequestID);
	// 请求添加一个策略运行方案
	int ReqAddStrategyInstance(std::string& strStrategyID, SStrategyInstance& instance, 
			int nRequestID);
	// 请求重置一个策略运行方案
	int ReqResetStrategyInstance(std::string& strStrategyID, SStrategyInstance& instance, 
			int nRequestID);

	// 请求删除一个策略运行方案
	int ReqDeleteStrategyInstance(const std::string& strID, int nRequestID);

	// 请求启动或停止一个策略运行方案
	int ReqStrategyInstance_RunStop(const std::string& strID, bool bRun, int nRequestID);

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
	CStrategyTraderApi();
	virtual ~CStrategyTraderApi();

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
	string							m_strTradingDay;

	std::map<std::string, PlatformStru_InstrumentInfo>
									m_mapInstrument;

	sTraderLoginRsp					m_UserLogin;

};
