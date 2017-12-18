#pragma once

#include "InstitudeApi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TraderSpi.h"
//#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"

class CTraderSpi;

class CInstitudeTraderApi : public CInstitudeApi
{
public:
	typedef struct {
		int							FrontID;
		int							SessionID;
		TThostFtdcBrokerIDType		BrokerID;
		TThostFtdcInvestorIDType	InvestorID;
		TThostFtdcOrderRefType		OrderRef;
		SYSTEMTIME					DateTime;
	} ORDERTIME;

public:
	///创建TraderApi
	///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
	///@return 创建出的UserApi
	//modify for udp marketdata
	static CInstitudeTraderApi* CreateInstitudeTraderApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
	
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
		return m_UserLogin.mTradingDay;
	};
	
	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	void RegisterFront(char *pszFrontAddress);
	
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterSpi(CTraderSpi* pSpi);
	
	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType);
	
	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType);

	///客户端认证请求
	int ReqAuthenticate(CThostFtdcReqAuthenticateField* pReqAuthenticateField, int nRequestID);

	///用户登录请求
	int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
	

	///登出请求
	int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);

	///用户口令更新请求
	int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID);

	///资金账户口令更新请求
	int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID);

	///报单录入请求
	int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID);

	///预埋单录入请求
	int ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID);

	///预埋撤单录入请求
	int ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID);

	///报单操作请求
	int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID);

	///查询最大报单数量请求
	int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID);

	///投资者结算结果确认
	int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID);

	///请求删除预埋单
	int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID);

	///请求删除预埋撤单
	int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID);

	///请求查询报单
	int ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID);

	///请求查询成交
	int ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID);

	///请求查询投资者持仓
	int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID);

	///请求查询资金账户
	int ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID);

	///请求查询投资者
	int ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID);

	///请求查询交易编码
	int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID);

	///请求查询合约保证金率
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID);

	///请求查询合约手续费率
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID);

	///请求查询交易所
	int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID);

	///请求查询合约
	int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID);

	///请求查询行情
	int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID);

	///请求查询投资者结算结果
	int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID);

	///请求查询转帐银行
	int ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID);

	///请求查询客户通知
	int ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID);

	///请求查询结算信息确认
	int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID);

	///请求查询保证金监管系统经纪公司资金账户密钥
	int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID);

	///请求查询仓单折抵信息
	int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID);

	///请求查询转帐流水
	int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID);

	///请求查询银期签约关系
	int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID);

    ///请求查询汇率
    int ReqQryExchangeRate(PlatformStru_QryExchangeRate *pQryExchangeRate, int nRequestID);

	///请求查询签约银行
	int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID);

	///请求查询预埋单
	int ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID);

	///请求查询预埋撤单
	int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID);

	///请求查询交易通知
	int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID);

	///请求查询经纪公司交易参数
	int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID);

	///请求查询经纪公司交易算法
	int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID);

	///期货发起银行资金转期货请求
	int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID);

	///期货发起期货资金转银行请求
	int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID);

	///期货发起查询银行余额请求
	int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID);
	
	// 订阅用户消息
	bool SubsUserMsg(int nUserID);
	// 退订用户消息
	bool UnsubsUserMsg(int nUserID);

	void OnFrontConnected();
	void OnFrontDisconnected();
	// 解析数据，仅在收到tcp数据时调用
	BOOL ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4);

protected:
	CInstitudeTraderApi();
	virtual ~CInstitudeTraderApi();

	void LockMe() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockMe() {
		::ReleaseMutex(m_mutex);
	};

	void Convert(sTraderLoginRsp& loginRsp, CThostFtdcRspUserLoginField& login);
	BOOL WriteOrderTimeToFile(std::string& strFile, ORDERTIME& item);

protected:
	HANDLE							m_eventExit;
	HANDLE							m_mutex;
	CTraderSpi*						m_pSpi;
	CTcpDataConnect*				m_pTcpConnect;
	string							m_strSvrIP;
	WORD							m_wSvrPort;
	CThostFtdcReqUserLoginField		m_ReqUser;
	PlatformStru_UserPasswordUpdate m_UserPasswordUpdate;
	sTraderLoginRsp					m_UserLogin;
};
