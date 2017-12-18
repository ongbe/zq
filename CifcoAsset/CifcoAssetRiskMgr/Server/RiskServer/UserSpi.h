#pragma once
#include "FtdcRiskUserApi.h"
class CWriteLog;
class CRiskMsgQueue;
class MaxNo
{
public:
	MaxNo()
	{
		memset(this,0,sizeof(*this));
	}
	int MaxTradeNo;
	int MaxOrderNo;
	int MaxPositionNo;
	int MaxDepositNo;
	int	MaxPreRiskAccountNo;
};
class UserSpi :
	public CShfeFtdcRiskUserSpi
{
public:
	UserSpi(MaxNo& maxno);
	~UserSpi(void);
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void OnHeartBeatWarning(int nTimeLapse);


	///错误应答
	void OnRspError(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///席位资金回报
	void OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit) ;

	///投资者信息摘要
	void OnRtnInvestorSumInfo(CShfeFtdcInvestorSumInfoField *pInvestorSumInfo) ;

	///客户端结算组数据同步开始
	void OnRtnClientSGDataSyncStart(CShfeFtdcSettlementSessionField *pSettlementSession) ;

	///客户端结算组数据同步结束
	void OnRtnClientSGDataSyncEnd(CShfeFtdcSettlementSessionField *pSettlementSession) ;

	///登录请求响应
	void OnRspRiskUserLogin(CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///
	void OnRspQryInvestorMarginRate(CShfeFtdcInvestorMarginRateField *pInvestorMarginRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///产品信息
	void OnRtnProduct(CShfeFtdcProductField *pProduct) ;

	///合约信息
	void OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument) ;

	///查询报单排名应答
	void OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///交易所的基本信息
	void OnRtnExchange(CShfeFtdcExchangeField *pExchange) ;

	///投资者持仓统计查询应答-ReqInvestorPosition, ftd version 6 deprecated
	void OnRspInvestorPositionStatic(CShfeFtdcInvestorPositionStaticField *pInvestorPositionStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者成交统计查询应答-ReqInvestorTrade, ftd version 6 deprecated
	void OnRspInvestorTradeStatic(CShfeFtdcInvestorTradeStaticField *pInvestorTradeStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///风控深度行情通知
	void OnRtnRiskDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData) ;

	///时间同步
	void OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime) ;

	///合约持仓比例应答
	void OnRspInstPositionRate(CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///产品持仓比例应答
	void OnRspProductPositionRate(CShfeFtdcRspProductPositionRateField *pRspProductPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者组
	void OnRtnInvestorGroup(CShfeFtdcInvestorGroupField *pInvestorGroup) ;

	///交易所保证金率，删除则用RtnDelRtnExchangeMarginRate
	void OnRtnExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) ;

	///交易所保证金率调整，删除则用RtnDelRtnExchangeMarginRateAdjust
	void OnRtnExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) ;

	///查询交易编码Hash值应答
	void OnRspQryTradingCodeHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询交易编码应答
	void OnRspQryTradingCode(CShfeFtdcTradingCodeField *pTradingCode, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///交易编码回报
	void OnRtnTradingCode(CShfeFtdcTradingCodeField *pTradingCode) ;

	///删除交易编码回报
	void OnRtnDelTradingCode(CShfeFtdcTradingCodeField *pTradingCode) ;

	///有序的经纪公司用户事件回报，对应订阅请求ReqSubBrokerUserEvent
	void OnRtnSequencialBrokerUserEvent(CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent) ;

	///有序的成交回报，对应订阅请求ReqSubscribeTrade
	void OnRtnSequencialTrade(CShfeFtdcSequencialTradeField *pSequencialTrade) ;

	///有序的报单回报，对应订阅请求ReqSubscribeOrder
	void OnRtnSequencialOrder(CShfeFtdcSequencialOrderField *pSequencialOrder) ;

	///风控报单录入请求响应
	void OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///风控报单操作请求响应
	void OnRspRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///有序的持仓回报，对应订阅请求ReqSubscribePosition
	void OnRtnSequencialPosition(CShfeFtdcSequencialPositionField *pSequencialPosition) ;

	///客户风险通知发送命令应答
	void OnRspRiskNotifyCommand(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///批量强平计算请求应答
	void OnRspBatchForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///单个投资者强平计算请求应答
	void OnRspForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///净持仓保证金指标参数设置应答
	void OnRspSetIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///净持仓保证金指标回报；如删除指标，则使用RtnDelIndexNPP
	void OnRtnIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) ;

	///删除净持仓保证金指标参数应答
	void OnRspRiskDelIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///强平计算请求资金账户应答
	void OnRspForceCloseAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询登陆请求应答
	void OnRspQryLogin(CShfeFtdcNormalRiskQueryField *pNormalRiskQuery, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询安全价格波动范围应答
	void OnRspQrySafePriceRange(CShfeFtdcRspSafePriceRangeField *pRspSafePriceRange, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询安全价格波动范围资金应答
	void OnRspSafePriceAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询价格波动对投资者风险的影响应答
	void OnRspQryPriceVaryEffect(CShfeFtdcPriceVaryParamField *pPriceVaryParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///组织架构
	void OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment) ;

	///IndexNPP应答
	void OnRspIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///交易系统参数
	void OnRtnTradeParam(CShfeFtdcTradeParamField *pTradeParam) ;

	///风控预埋单录入应答
	void OnRspRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除预埋单响应
	void OnRspRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///有序的风控预埋单回报
	void OnRtnSeqRiskParkedOrder(CShfeFtdcSeqRiskParkedOrderField *pSeqRiskParkedOrder) ;

	///风控用户密码修改应答
	void OnRspRiskUserPasswordUpd(CShfeFtdcUserPasswordUpdateField *pUserPasswordUpdate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///有序的出入金回报，对应订阅请求ReqSubSeqDeposit
	void OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit) ;

	///插入风控用户事件应答
	void OnRspAddRiskUserEvent(CShfeFtdcRiskUserEventField *pRiskUserEvent, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///风险预算资金应答
	void OnRspPredictRiskAccount(CShfeFtdcPredictRiskAccountField *pPredictRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///风险预算持仓应答
	void OnRspPredictRiskPosition(CShfeFtdcPredictRiskPositionField *pPredictRiskPosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者基础信息回报，只同步盘中变化数据，可以配合{ReqQryInvestorLinkMan,RspQryInvestorLinkMan}使用
	void OnRtnRiskInvestor(CShfeFtdcRiskSyncInvestorField *pRiskSyncInvestor) ;

	///查询投资者和联系人信息应答
	void OnRspQryInvestorLinkMan(CShfeFtdcInvestorLinkManField *pInvestorLinkMan, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除交易所保证金率
	void OnRtnDelExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) ;

	///删除交易所保证金率调整
	void OnRtnDelExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) ;

	///查询组织架构投资者对应关系应答
	void OnRspQryInvestorDepartment(CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除净持仓保证金指标回报，与RtnIndexNPP对应
	void OnRtnDelIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) ;

	///风控用户权限回报
	void OnRtnRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) ;

	///删除风控用户权限回报
	void OnRtnDelRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) ;

	///投资者当日资金回报；昨日资金使用{ReqSubPreRiskAccount,RtnSeqPreRiskAccount}
	void OnRtnRiskSyncAccount(CShfeFtdcRiskSyncAccountField *pRiskSyncAccount) ;

	///有序的昨日资金回报，对应订阅请求ReqSubPreRiskAccount
	void OnRtnSeqPreRiskAccount(CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount) ;

	///模板替换字段回报
	void OnRtnNoticeToken(CShfeFtdcNoticeTokenField *pNoticeToken) ;

	///通知模板回报
	void OnRtnNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern) ;

	///更新通知模版应答
	void OnRspModNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///变化行情回报，目前只订阅最新价
	void OnRtnVaryMarketData(CShfeFtdcVaryMarketDataField *pVaryMarketData) ;

	///新增风险通知应答
	void OnRspAddRiskNotifyA(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///新增业务通知应答
	void OnRspAddBizNotice(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///有序的业务通知回报
	void OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice) ;

	///查询席位资金应答
	void OnRspRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///风控参数信息回报
	void OnRtnRiskParamInfo(CShfeFtdcRiskParamInfoField *pRiskParamInfo) ;

	///修改投资者参数应答
	void OnRspModRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除投资者参数应答
	void OnRspRemRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者参数回报
	void OnRtnRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) ;

	///删除投资者参数回报
	void OnRtnDelRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) ;

	///强制风控用户登出应答
	void OnRspForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///强制风控用户登出回报
	void OnRtnForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo) ;

	///增加风控通知模板应答
	void OnRspAddRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///修改风控通知模板应答
	void OnRspModRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除风控通知模板应答
	void OnRspRemRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///风控通知模板回报
	void OnRtnRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) ;

	///删除风控通知模板回报
	void OnRtnDelRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) ;

	///增加投资者通知模板应答
	void OnRspAddInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///修改投资者通知模板应答
	void OnRspModInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除投资者通知模板应答
	void OnRspRemInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者通知模板回报
	void OnRtnInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) ;

	///删除投资者通知模板回报
	void OnRtnDelInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) ;

	///风险通知模板自动替换字段回报
	void OnRtnRiskNotifyToken(CShfeFtdcRiskNotifyTokenField *pRiskNotifyToken) ;

	///有序的风险通知回报版本B，对应订阅请求ReqSubSeqRiskNotifyB
	void OnRtnSeqRiskNotifyB(CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB) ;

	///查询持仓排名应答
	void OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询成交排名应答
	void OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者及联系人信息哈希值查询应答
	void OnRspQryInvestorLinkManHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询投资者组织架构Hash值应答
	void OnRspQryInvestorDepartmentHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///压力测试应答
	void OnRspQryStressTest(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询低保投资者hash值应答
	void OnRspQryLowMarginInvestorHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询低保投资者应答
	void OnRspQryLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///低保投资者回报
	void OnRtnLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) ;

	///删除低保投资者回报
	void OnRtnDelLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) ;
protected:
	CWriteLog* m_pWriteLog;
	MaxNo& m_MaxNo;
};
