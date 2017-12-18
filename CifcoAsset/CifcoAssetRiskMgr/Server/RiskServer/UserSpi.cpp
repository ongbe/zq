#include "StdAfx.h"
#include "UserSpi.h"
#include "RiskMsgQueue.h"
#include "ConvertFromCtp.h"
#include "DumpFromCtp.h"
#include "Tools/WriteLog.h"
//#if 0
//#define LOG_INFO(fmt, ...) 
//#else
bool g_NeedSaveUserSpi=false;
#define LOG_INFO(fmt, ...) \
	do{\
	if(g_NeedSaveUserSpi&&m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("UserSpi", WriteLogLevel_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
//#endif
#define SAFE_ADD_MSG		if(CRiskMsgQueue::Get()) CRiskMsgQueue::Get()->AddMsg
#define SAFE_ADD_MSG_QUOT	if(CRiskMsgQueue_Quot::Get()) CRiskMsgQueue_Quot::Get()->AddMsg
#define SAFE_ADD_MSG_SYNC	if(CRiskMsgQueue_Sync::Get()) CRiskMsgQueue_Sync::Get()->AddMsg
#define CONVERTSTRUCTFROMCTP(TYPE)\
	TYPE##Field* np##TYPE=new TYPE##Field();\
	std::auto_ptr<TYPE##Field> atuo_p##TYPE(np##TYPE);\
	std::string str##TYPE;\
	DumpFromCtp::Dump##TYPE##Field(str##TYPE,*p##TYPE);\
	LOG_INFO("%s",str##TYPE.c_str());\
	ConvertFromCtp::Get##TYPE##Field(*p##TYPE,*np##TYPE);
#define SAVE_MAX(tag,no) if(tag&&tag->UniqSequenceNo>m_MaxNo.no) m_MaxNo.no=tag->UniqSequenceNo

UserSpi::UserSpi(MaxNo& maxno)
:m_MaxNo(maxno)
{
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"UserSpi.log");
}

UserSpi::~UserSpi(void)
{
	SAFE_DELETE(m_pWriteLog);
}
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。	
void UserSpi::OnFrontConnected()	
{	
	LOG_INFO("OnFrontConnected");
	SAFE_ADD_MSG(MSGID_FrontConnected,0,0);
}	


///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。	
///@param nReason 错误原因	
///        0x1001 网络读失败	
///        0x1002 网络写失败	
///        0x2001 接收心跳超时	
///        0x2002 发送心跳失败	
///        0x2003 收到错误报文	
void UserSpi::OnFrontDisconnected(int nReason)	
{	
	LOG_INFO("OnFrontDisconnected(nReason=%d)\n",nReason);
	SAFE_ADD_MSG(MSGID_FrontDisconnected,nReason);
}	


///心跳超时警告。当长时间未收到报文时，该方法被调用。	
///@param nTimeLapse 距离上次接收报文的时间	
void UserSpi::OnHeartBeatWarning(int nTimeLapse)	
{	
	LOG_INFO("OnHeartBeatWarning(%d)\n",nTimeLapse);
	SAFE_ADD_MSG(MSGID_HeartBeatWarning,nTimeLapse);
}	
///错误应答			
void UserSpi::OnRspError(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspError");		
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspError,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///席位资金回报			
void UserSpi::OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit) 			
{			
	LOG_INFO("OnRtnBrokerDeposit");		
	CONVERTSTRUCTFROMCTP(BrokerDeposit);	
	SAFE_ADD_MSG(MSGID_RtnBrokerDeposit,sizeof(BrokerDepositField),npBrokerDeposit );		
}			


///投资者信息摘要			
void UserSpi::OnRtnInvestorSumInfo(CShfeFtdcInvestorSumInfoField *pInvestorSumInfo) 			
{			
	LOG_INFO("OnRtnInvestorSumInfo");		
	CONVERTSTRUCTFROMCTP(InvestorSumInfo);	
	SAFE_ADD_MSG(MSGID_RtnInvestorSumInfo,sizeof(InvestorSumInfoField),npInvestorSumInfo );		
}			


///客户端结算组数据同步开始			
void UserSpi::OnRtnClientSGDataSyncStart(CShfeFtdcSettlementSessionField *pSettlementSession) 			
{			
	LOG_INFO("OnRtnClientSGDataSyncStart");		
	CONVERTSTRUCTFROMCTP(SettlementSession);	
	SAFE_ADD_MSG(MSGID_RtnClientSGDataSyncStart,sizeof(SettlementSessionField),npSettlementSession );		
}			


///客户端结算组数据同步结束			
void UserSpi::OnRtnClientSGDataSyncEnd(CShfeFtdcSettlementSessionField *pSettlementSession) 			
{			
	LOG_INFO("OnRtnClientSGDataSyncEnd");		
	CONVERTSTRUCTFROMCTP(SettlementSession);	
	SAFE_ADD_MSG(MSGID_RtnClientSGDataSyncEnd,sizeof(SettlementSessionField),npSettlementSession );		
}			


///登录请求响应			
void UserSpi::OnRspRiskUserLogin(CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspRiskUserLogin");		
	CONVERTSTRUCTFROMCTP(RspRiskUserLogin);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskUserLogin,sizeof(RspRiskUserLoginField),npRspRiskUserLogin,npRspInfo,nRequestID,bIsLast);		
}			


///			
void UserSpi::OnRspQryInvestorMarginRate(CShfeFtdcInvestorMarginRateField *pInvestorMarginRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorMarginRate");		
	CONVERTSTRUCTFROMCTP(InvestorMarginRate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorMarginRate,sizeof(InvestorMarginRateField),npInvestorMarginRate,npRspInfo,nRequestID,bIsLast);		
}			


///产品信息			
void UserSpi::OnRtnProduct(CShfeFtdcProductField *pProduct) 			
{			
	LOG_INFO("OnRtnProduct");		
	CONVERTSTRUCTFROMCTP(Product);	
	SAFE_ADD_MSG(MSGID_RtnProduct,sizeof(ProductField),npProduct );		
}			


///合约信息			
void UserSpi::OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument) 			
{			
	LOG_INFO("OnRtnInstrument");		
	CONVERTSTRUCTFROMCTP(Instrument);	
	SAFE_ADD_MSG(MSGID_RtnInstrument,sizeof(InstrumentField),npInstrument );		
}			


///查询报单排名应答			
void UserSpi::OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspQryOrderStat");	
	CONVERTSTRUCTFROMCTP(OrderStat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryOrderStat,sizeof(OrderStatField),npOrderStat,npRspInfo,nRequestID,bIsLast);		
}			


///交易所的基本信息			
void UserSpi::OnRtnExchange(CShfeFtdcExchangeField *pExchange) 			
{			
	LOG_INFO("OnRtnExchange");		
	CONVERTSTRUCTFROMCTP(Exchange);	
	SAFE_ADD_MSG(MSGID_RtnExchange,sizeof(ExchangeField),npExchange );		
}			


///投资者持仓统计查询应答-ReqInvestorPosition, ftd version 6 deprecated			
void UserSpi::OnRspInvestorPositionStatic(CShfeFtdcInvestorPositionStaticField *pInvestorPositionStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspInvestorPositionStatic");	
	CONVERTSTRUCTFROMCTP(InvestorPositionStatic);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspInvestorPositionStatic,sizeof(InvestorPositionStaticField),npInvestorPositionStatic,npRspInfo,nRequestID,bIsLast);		
}			


///投资者成交统计查询应答-ReqInvestorTrade, ftd version 6 deprecated			
void UserSpi::OnRspInvestorTradeStatic(CShfeFtdcInvestorTradeStaticField *pInvestorTradeStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspInvestorTradeStatic");		
	CONVERTSTRUCTFROMCTP(InvestorTradeStatic);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspInvestorTradeStatic,sizeof(InvestorTradeStaticField),npInvestorTradeStatic,npRspInfo,nRequestID,bIsLast);		
}			


///风控深度行情通知			
void UserSpi::OnRtnRiskDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData) 			
{			
	LOG_INFO("OnRtnRiskDepthMarketData");	
	CONVERTSTRUCTFROMCTP(DepthMarketData);	
	SAFE_ADD_MSG_QUOT(MSGID_RtnRiskDepthMarketData,sizeof(DepthMarketDataField),npDepthMarketData );		
}			


///时间同步			
void UserSpi::OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime) 			
{			
	LOG_INFO("OnRtnTimeSync");		
	CONVERTSTRUCTFROMCTP(CurrentTime);	
	SAFE_ADD_MSG(MSGID_RtnTimeSync,sizeof(CurrentTimeField),npCurrentTime );		
}			


///合约持仓比例应答			
void UserSpi::OnRspInstPositionRate(CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspInstPositionRate");	
	CONVERTSTRUCTFROMCTP(RspInstPositionRate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspInstPositionRate,sizeof(RspInstPositionRateField),npRspInstPositionRate,npRspInfo,nRequestID,bIsLast);		
}			


///产品持仓比例应答			
void UserSpi::OnRspProductPositionRate(CShfeFtdcRspProductPositionRateField *pRspProductPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspProductPositionRate");		
	CONVERTSTRUCTFROMCTP(RspProductPositionRate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspProductPositionRate,sizeof(RspProductPositionRateField),npRspProductPositionRate,npRspInfo,nRequestID,bIsLast);		
}			


///投资者组			
void UserSpi::OnRtnInvestorGroup(CShfeFtdcInvestorGroupField *pInvestorGroup) 			
{			
	LOG_INFO("OnRtnInvestorGroup");		
	CONVERTSTRUCTFROMCTP(InvestorGroup);	
	SAFE_ADD_MSG(MSGID_RtnInvestorGroup,sizeof(InvestorGroupField),npInvestorGroup );		
}			


///交易所保证金率，删除则用RtnDelRtnExchangeMarginRate			
void UserSpi::OnRtnExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) 			
{			
	LOG_INFO("OnRtnExchangeMarginRate");	
	CONVERTSTRUCTFROMCTP(ExchangeMarginRate);	
	SAFE_ADD_MSG(MSGID_RtnExchangeMarginRate,sizeof(ExchangeMarginRateField),npExchangeMarginRate );		
}			


///交易所保证金率调整，删除则用RtnDelRtnExchangeMarginRateAdjust			
void UserSpi::OnRtnExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) 			
{			
	LOG_INFO("OnRtnExchangeMarginRateAdjust");		
	CONVERTSTRUCTFROMCTP(ExchangeMarginRateAdjust);	
	SAFE_ADD_MSG(MSGID_RtnExchangeMarginRateAdjust,sizeof(ExchangeMarginRateAdjustField),npExchangeMarginRateAdjust );		
}			


///查询交易编码Hash值应答			
void UserSpi::OnRspQryTradingCodeHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryTradingCodeHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryTradingCodeHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///查询交易编码应答			
void UserSpi::OnRspQryTradingCode(CShfeFtdcTradingCodeField *pTradingCode, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryTradingCode");		
	CONVERTSTRUCTFROMCTP(TradingCode);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryTradingCode,sizeof(TradingCodeField),npTradingCode,npRspInfo,nRequestID,bIsLast);		
}			


///交易编码回报			
void UserSpi::OnRtnTradingCode(CShfeFtdcTradingCodeField *pTradingCode) 			
{			
	LOG_INFO("OnRtnTradingCode");		
	CONVERTSTRUCTFROMCTP(TradingCode);	
	SAFE_ADD_MSG(MSGID_RtnTradingCode,sizeof(TradingCodeField),npTradingCode );		
}			


///删除交易编码回报			
void UserSpi::OnRtnDelTradingCode(CShfeFtdcTradingCodeField *pTradingCode) 			
{			
	LOG_INFO("OnRtnDelTradingCode");		
	CONVERTSTRUCTFROMCTP(TradingCode);	
	SAFE_ADD_MSG(MSGID_RtnDelTradingCode,sizeof(TradingCodeField),npTradingCode );		
}			


///有序的经纪公司用户事件回报，对应订阅请求ReqSubBrokerUserEvent			
void UserSpi::OnRtnSequencialBrokerUserEvent(CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent) 			
{			
	LOG_INFO("OnRtnSequencialBrokerUserEvent");		
	CONVERTSTRUCTFROMCTP(SequencialBrokerUserEvent);	
	SAFE_ADD_MSG(MSGID_RtnSequencialBrokerUserEvent,sizeof(SequencialBrokerUserEventField),npSequencialBrokerUserEvent );		
}			


///有序的成交回报，对应订阅请求ReqSubscribeTrade			
void UserSpi::OnRtnSequencialTrade(CShfeFtdcSequencialTradeField *pSequencialTrade) 			
{
	SAVE_MAX(pSequencialTrade,MaxTradeNo);
	LOG_INFO("OnRtnSequencialTrade");		
	CONVERTSTRUCTFROMCTP(SequencialTrade);	
	SAFE_ADD_MSG(MSGID_RtnSequencialTrade,sizeof(SequencialTradeField),npSequencialTrade );		
}			


///有序的报单回报，对应订阅请求ReqSubscribeOrder			
void UserSpi::OnRtnSequencialOrder(CShfeFtdcSequencialOrderField *pSequencialOrder) 			
{
	SAVE_MAX(pSequencialOrder,MaxOrderNo);			
	LOG_INFO("OnRtnSequencialOrder");		
	CONVERTSTRUCTFROMCTP(SequencialOrder);
	SAFE_ADD_MSG(MSGID_RtnSequencialOrder,sizeof(SequencialOrderField),npSequencialOrder );		
}			


///风控报单录入请求响应			
void UserSpi::OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskOrderInsert");		
	CONVERTSTRUCTFROMCTP(InputOrder);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskOrderInsert,sizeof(InputOrderField),npInputOrder,npRspInfo,nRequestID,bIsLast);		
}			


///风控报单操作请求响应			
void UserSpi::OnRspRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskOrderAction");		
	CONVERTSTRUCTFROMCTP(InputOrderAction);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskOrderAction,sizeof(InputOrderActionField),npInputOrderAction,npRspInfo,nRequestID,bIsLast);		
}			


///有序的持仓回报，对应订阅请求ReqSubscribePosition			
void UserSpi::OnRtnSequencialPosition(CShfeFtdcSequencialPositionField *pSequencialPosition) 			
{
	SAVE_MAX(pSequencialPosition,MaxPositionNo);				
	LOG_INFO("OnRtnSequencialPosition");		
	CONVERTSTRUCTFROMCTP(SequencialPosition);	
	SAFE_ADD_MSG(MSGID_RtnSequencialPosition,sizeof(SequencialPositionField),npSequencialPosition );		
}			


///客户风险通知发送命令应答			
void UserSpi::OnRspRiskNotifyCommand(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskNotifyCommand");	
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskNotifyCommand,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///批量强平计算请求应答			
void UserSpi::OnRspBatchForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspBatchForceCloseCalc");		
	CONVERTSTRUCTFROMCTP(RspForceClosePosition);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspBatchForceCloseCalc,sizeof(RspForceClosePositionField),npRspForceClosePosition,npRspInfo,nRequestID,bIsLast);		
}			


///单个投资者强平计算请求应答			
void UserSpi::OnRspForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspForceCloseCalc");	
	CONVERTSTRUCTFROMCTP(RspForceClosePosition);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspForceCloseCalc,sizeof(RspForceClosePositionField),npRspForceClosePosition,npRspInfo,nRequestID,bIsLast);		
}			


///净持仓保证金指标参数设置应答			
void UserSpi::OnRspSetIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspSetIndexNPPPara");		
	CONVERTSTRUCTFROMCTP(IndexNPP);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspSetIndexNPPPara,sizeof(IndexNPPField),npIndexNPP,npRspInfo,nRequestID,bIsLast);		
}			


///净持仓保证金指标回报；如删除指标，则使用RtnDelIndexNPP			
void UserSpi::OnRtnIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) 			
{			
	LOG_INFO("OnRtnIndexNPP");		
	CONVERTSTRUCTFROMCTP(FullIndexNPP);	
	SAFE_ADD_MSG(MSGID_RtnIndexNPP,sizeof(FullIndexNPPField),npFullIndexNPP );		
}			


///删除净持仓保证金指标参数应答			
void UserSpi::OnRspRiskDelIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskDelIndexNPPPara");		
	CONVERTSTRUCTFROMCTP(IndexNPP);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskDelIndexNPPPara,sizeof(IndexNPPField),npIndexNPP,npRspInfo,nRequestID,bIsLast);		
}			


///强平计算请求资金账户应答			
void UserSpi::OnRspForceCloseAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspForceCloseAccount");		
	CONVERTSTRUCTFROMCTP(InvestorRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspForceCloseAccount,sizeof(InvestorRiskAccountField),npInvestorRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///查询登陆请求应答			
void UserSpi::OnRspQryLogin(CShfeFtdcNormalRiskQueryField *pNormalRiskQuery, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryLogin");	
	CONVERTSTRUCTFROMCTP(NormalRiskQuery);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryLogin,sizeof(NormalRiskQueryField),npNormalRiskQuery,npRspInfo,nRequestID,bIsLast);		
}			


///查询安全价格波动范围应答			
void UserSpi::OnRspQrySafePriceRange(CShfeFtdcRspSafePriceRangeField *pRspSafePriceRange, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQrySafePriceRange");		
	CONVERTSTRUCTFROMCTP(RspSafePriceRange);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQrySafePriceRange,sizeof(RspSafePriceRangeField),npRspSafePriceRange,npRspInfo,nRequestID,bIsLast);		
}			


///查询安全价格波动范围资金应答			
void UserSpi::OnRspSafePriceAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspSafePriceAccount");		
	CONVERTSTRUCTFROMCTP(InvestorRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspSafePriceAccount,sizeof(InvestorRiskAccountField),npInvestorRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///查询价格波动对投资者风险的影响应答			
void UserSpi::OnRspQryPriceVaryEffect(CShfeFtdcPriceVaryParamField *pPriceVaryParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryPriceVaryEffect");	
	CONVERTSTRUCTFROMCTP(PriceVaryParam);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryPriceVaryEffect,sizeof(PriceVaryParamField),npPriceVaryParam,npRspInfo,nRequestID,bIsLast);		
}			


///组织架构			
void UserSpi::OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment) 			
{			
	LOG_INFO("OnRtnDepartment");		
	CONVERTSTRUCTFROMCTP(Department);	
	SAFE_ADD_MSG(MSGID_RtnDepartment,sizeof(DepartmentField),npDepartment );		
}			


///IndexNPP应答			
void UserSpi::OnRspIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspIndexNPP");		
	CONVERTSTRUCTFROMCTP(FullIndexNPP);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspIndexNPP,sizeof(FullIndexNPPField),npFullIndexNPP,npRspInfo,nRequestID,bIsLast);		
}			


///交易系统参数			
void UserSpi::OnRtnTradeParam(CShfeFtdcTradeParamField *pTradeParam) 			
{			
	LOG_INFO("OnRtnTradeParam");		
	CONVERTSTRUCTFROMCTP(TradeParam);	
	SAFE_ADD_MSG(MSGID_RtnTradeParam,sizeof(TradeParamField),npTradeParam );		
}			


///风控预埋单录入应答			
void UserSpi::OnRspRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskParkedOrderInsert");		
	CONVERTSTRUCTFROMCTP(RiskParkedOrder);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskParkedOrderInsert,sizeof(RiskParkedOrderField),npRiskParkedOrder,npRspInfo,nRequestID,bIsLast);		
}			


///删除预埋单响应			
void UserSpi::OnRspRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemoveRiskParkedOrder");		
	CONVERTSTRUCTFROMCTP(RemoveRiskParkedOrder);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemoveRiskParkedOrder,sizeof(RemoveRiskParkedOrderField),npRemoveRiskParkedOrder,npRspInfo,nRequestID,bIsLast);		
}			


///有序的风控预埋单回报			
void UserSpi::OnRtnSeqRiskParkedOrder(CShfeFtdcSeqRiskParkedOrderField *pSeqRiskParkedOrder) 			
{			
	LOG_INFO("OnRtnSeqRiskParkedOrder");		
	CONVERTSTRUCTFROMCTP(SeqRiskParkedOrder);	
	SAFE_ADD_MSG(MSGID_RtnSeqRiskParkedOrder,sizeof(SeqRiskParkedOrderField),npSeqRiskParkedOrder );		
}			


///风控用户密码修改应答			
void UserSpi::OnRspRiskUserPasswordUpd(CShfeFtdcUserPasswordUpdateField *pUserPasswordUpdate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskUserPasswordUpd");	
	CONVERTSTRUCTFROMCTP(UserPasswordUpdate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskUserPasswordUpd,sizeof(UserPasswordUpdateField),npUserPasswordUpdate,npRspInfo,nRequestID,bIsLast);		
}			


///有序的出入金回报，对应订阅请求ReqSubSeqDeposit			
void UserSpi::OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit) 			
{
	SAVE_MAX(pSeqDeposit,MaxDepositNo);				
	LOG_INFO("OnRtnSeqDeposit");		
	CONVERTSTRUCTFROMCTP(SeqDeposit);	
	SAFE_ADD_MSG(MSGID_RtnSeqDeposit,sizeof(SeqDepositField),npSeqDeposit );		
}			


///插入风控用户事件应答			
void UserSpi::OnRspAddRiskUserEvent(CShfeFtdcRiskUserEventField *pRiskUserEvent, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddRiskUserEvent");		
	CONVERTSTRUCTFROMCTP(RiskUserEvent);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddRiskUserEvent,sizeof(RiskUserEventField),npRiskUserEvent,npRspInfo,nRequestID,bIsLast);		
}			


///风险预算资金应答			
void UserSpi::OnRspPredictRiskAccount(CShfeFtdcPredictRiskAccountField *pPredictRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspPredictRiskAccount");		
	CONVERTSTRUCTFROMCTP(PredictRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspPredictRiskAccount,sizeof(PredictRiskAccountField),npPredictRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///风险预算持仓应答			
void UserSpi::OnRspPredictRiskPosition(CShfeFtdcPredictRiskPositionField *pPredictRiskPosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspPredictRiskPosition");		
	CONVERTSTRUCTFROMCTP(PredictRiskPosition);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspPredictRiskPosition,sizeof(PredictRiskPositionField),npPredictRiskPosition,npRspInfo,nRequestID,bIsLast);		
}			


///投资者基础信息回报，只同步盘中变化数据，可以配合{ReqQryInvestorLinkMan,RspQryInvestorLinkMan}使用			
void UserSpi::OnRtnRiskInvestor(CShfeFtdcRiskSyncInvestorField *pRiskSyncInvestor) 			
{			
	LOG_INFO("OnRtnRiskInvestor");		
	CONVERTSTRUCTFROMCTP(RiskSyncInvestor);	
	SAFE_ADD_MSG(MSGID_RtnRiskInvestor,sizeof(RiskSyncInvestorField),npRiskSyncInvestor );		
}			


///查询投资者和联系人信息应答			
void UserSpi::OnRspQryInvestorLinkMan(CShfeFtdcInvestorLinkManField *pInvestorLinkMan, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorLinkMan");		
	CONVERTSTRUCTFROMCTP(InvestorLinkMan);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorLinkMan,sizeof(InvestorLinkManField),npInvestorLinkMan,npRspInfo,nRequestID,bIsLast);		
}			


///删除交易所保证金率			
void UserSpi::OnRtnDelExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) 			
{			
	LOG_INFO("OnRtnDelExchangeMarginRate");		
	CONVERTSTRUCTFROMCTP(ExchangeMarginRate);	
	SAFE_ADD_MSG(MSGID_RtnDelExchangeMarginRate,sizeof(ExchangeMarginRateField),npExchangeMarginRate );		
}			


///删除交易所保证金率调整			
void UserSpi::OnRtnDelExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) 			
{			
	LOG_INFO("OnRtnDelExchangeMarginRateAdjust");		
	CONVERTSTRUCTFROMCTP(ExchangeMarginRateAdjust);	
	SAFE_ADD_MSG(MSGID_RtnDelExchangeMarginRateAdjust,sizeof(ExchangeMarginRateAdjustField),npExchangeMarginRateAdjust );		
}			


///查询组织架构投资者对应关系应答			
void UserSpi::OnRspQryInvestorDepartment(CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorDepartment");		
	CONVERTSTRUCTFROMCTP(InvestorDepartmentFlat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorDepartment,sizeof(InvestorDepartmentFlatField),npInvestorDepartmentFlat,npRspInfo,nRequestID,bIsLast);		
}			


///删除净持仓保证金指标回报，与RtnIndexNPP对应			
void UserSpi::OnRtnDelIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) 			
{			
	LOG_INFO("OnRtnDelIndexNPP");		
	CONVERTSTRUCTFROMCTP(FullIndexNPP);	
	SAFE_ADD_MSG(MSGID_RtnDelIndexNPP,sizeof(FullIndexNPPField),npFullIndexNPP );		
}			


///风控用户权限回报			
void UserSpi::OnRtnRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) 			
{			
	LOG_INFO("OnRtnRiskUserFunction");		
	CONVERTSTRUCTFROMCTP(RiskUserFunction);	
	SAFE_ADD_MSG(MSGID_RtnRiskUserFunction,sizeof(RiskUserFunctionField),npRiskUserFunction );		
}			


///删除风控用户权限回报			
void UserSpi::OnRtnDelRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) 			
{			
	LOG_INFO("OnRtnDelRiskUserFunction");		
	CONVERTSTRUCTFROMCTP(RiskUserFunction);	
	SAFE_ADD_MSG(MSGID_RtnDelRiskUserFunction,sizeof(RiskUserFunctionField),npRiskUserFunction );		
}			


///投资者当日资金回报；昨日资金使用{ReqSubPreRiskAccount,RtnSeqPreRiskAccount}			
void UserSpi::OnRtnRiskSyncAccount(CShfeFtdcRiskSyncAccountField *pRiskSyncAccount) 			
{			
	LOG_INFO("OnRtnRiskSyncAccount");	
	CONVERTSTRUCTFROMCTP(RiskSyncAccount);	
	SAFE_ADD_MSG_SYNC(MSGID_RtnRiskSyncAccount,sizeof(RiskSyncAccountField),npRiskSyncAccount );		
}			


///有序的昨日资金回报，对应订阅请求ReqSubPreRiskAccount			
void UserSpi::OnRtnSeqPreRiskAccount(CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount) 			
{	
	SAVE_MAX(pSeqPreRiskAccount,MaxPreRiskAccountNo);
	LOG_INFO("OnRtnSeqPreRiskAccount");		
	CONVERTSTRUCTFROMCTP(SeqPreRiskAccount);	
	SAFE_ADD_MSG(MSGID_RtnSeqPreRiskAccount,sizeof(SeqPreRiskAccountField),npSeqPreRiskAccount );		
}			


///模板替换字段回报			
void UserSpi::OnRtnNoticeToken(CShfeFtdcNoticeTokenField *pNoticeToken) 			
{			
	LOG_INFO("OnRtnNoticeToken");		
	CONVERTSTRUCTFROMCTP(NoticeToken);	
	SAFE_ADD_MSG(MSGID_RtnNoticeToken,sizeof(NoticeTokenField),npNoticeToken );		
}			


///通知模板回报			
void UserSpi::OnRtnNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern) 			
{			
	LOG_INFO("OnRtnNoticePattern");		
	CONVERTSTRUCTFROMCTP(NoticePattern);	
	SAFE_ADD_MSG(MSGID_RtnNoticePattern,sizeof(NoticePatternField),npNoticePattern );		
}			


///更新通知模版应答			
void UserSpi::OnRspModNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModNoticePattern");		
	CONVERTSTRUCTFROMCTP(NoticePattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModNoticePattern,sizeof(NoticePatternField),npNoticePattern,npRspInfo,nRequestID,bIsLast);		
}			


///变化行情回报，目前只订阅最新价			
void UserSpi::OnRtnVaryMarketData(CShfeFtdcVaryMarketDataField *pVaryMarketData) 			
{			
	LOG_INFO("OnRtnVaryMarketData");		
	CONVERTSTRUCTFROMCTP(VaryMarketData);	
	SAFE_ADD_MSG(MSGID_RtnVaryMarketData,sizeof(VaryMarketDataField),npVaryMarketData );		
}			


///新增风险通知应答			
void UserSpi::OnRspAddRiskNotifyA(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddRiskNotifyA");		
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddRiskNotifyA,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///新增业务通知应答			
void UserSpi::OnRspAddBizNotice(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddBizNotice");		
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddBizNotice,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///有序的业务通知回报			
void UserSpi::OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice) 			
{			
	LOG_INFO("OnRtnSeqBizNotice");		
	CONVERTSTRUCTFROMCTP(SeqBizNotice);	
	SAFE_ADD_MSG(MSGID_RtnSeqBizNotice,sizeof(SeqBizNoticeField),npSeqBizNotice );		
}			


///查询席位资金应答			
void UserSpi::OnRspRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskQryBrokerDeposit");		
	CONVERTSTRUCTFROMCTP(QueryBrokerDeposit);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskQryBrokerDeposit,sizeof(QueryBrokerDepositField),npQueryBrokerDeposit,npRspInfo,nRequestID,bIsLast);		
}			


///风控参数信息回报			
void UserSpi::OnRtnRiskParamInfo(CShfeFtdcRiskParamInfoField *pRiskParamInfo) 			
{			
	LOG_INFO("OnRtnRiskParamInfo");		
	CONVERTSTRUCTFROMCTP(RiskParamInfo);	
	SAFE_ADD_MSG(MSGID_RtnRiskParamInfo,sizeof(RiskParamInfoField),npRiskParamInfo );		
}			


///修改投资者参数应答			
void UserSpi::OnRspModRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam,npRspInfo,nRequestID,bIsLast);		
}			


///删除投资者参数应答			
void UserSpi::OnRspRemRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam,npRspInfo,nRequestID,bIsLast);		
}			


///投资者参数回报			
void UserSpi::OnRtnRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) 			
{			
	LOG_INFO("OnRtnRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);	
	SAFE_ADD_MSG(MSGID_RtnRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam );		
}			


///删除投资者参数回报			
void UserSpi::OnRtnDelRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) 			
{			
	LOG_INFO("OnRtnDelRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);	
	SAFE_ADD_MSG(MSGID_RtnDelRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam );		
}			


///强制风控用户登出应答			
void UserSpi::OnRspForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspForceRiskUserLogout");		
	CONVERTSTRUCTFROMCTP(RiskLoginInfo);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspForceRiskUserLogout,sizeof(RiskLoginInfoField),npRiskLoginInfo,npRspInfo,nRequestID,bIsLast);		
}			


///强制风控用户登出回报			
void UserSpi::OnRtnForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo) 			
{			
	LOG_INFO("OnRtnForceRiskUserLogout");		
	CONVERTSTRUCTFROMCTP(RiskLoginInfo);	
	SAFE_ADD_MSG(MSGID_RtnForceRiskUserLogout,sizeof(RiskLoginInfoField),npRiskLoginInfo );		
}			


///增加风控通知模板应答			
void UserSpi::OnRspAddRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddRiskPattern,sizeof(RiskPatternField),npRiskPattern,npRspInfo,nRequestID,bIsLast);		
}			


///修改风控通知模板应答			
void UserSpi::OnRspModRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModRiskPattern,sizeof(RiskPatternField),npRiskPattern,npRspInfo,nRequestID,bIsLast);		
}			


///删除风控通知模板应答			
void UserSpi::OnRspRemRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemRiskPattern,sizeof(RiskPatternField),npRiskPattern,npRspInfo,nRequestID,bIsLast);		
}			


///风控通知模板回报			
void UserSpi::OnRtnRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) 			
{			
	LOG_INFO("OnRtnRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);	
	SAFE_ADD_MSG(MSGID_RtnRiskPattern,sizeof(RiskPatternField),npRiskPattern );		
}			


///删除风控通知模板回报			
void UserSpi::OnRtnDelRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) 			
{			
	LOG_INFO("OnRtnDelRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);	
	SAFE_ADD_MSG(MSGID_RtnDelRiskPattern,sizeof(RiskPatternField),npRiskPattern );		
}			


///增加投资者通知模板应答			
void UserSpi::OnRspAddInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern,npRspInfo,nRequestID,bIsLast);		
}			


///修改投资者通知模板应答			
void UserSpi::OnRspModInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern,npRspInfo,nRequestID,bIsLast);		
}			


///删除投资者通知模板应答			
void UserSpi::OnRspRemInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern,npRspInfo,nRequestID,bIsLast);		
}			


///投资者通知模板回报			
void UserSpi::OnRtnInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) 			
{			
	LOG_INFO("OnRtnInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);	
	SAFE_ADD_MSG(MSGID_RtnInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern );		
}			


///删除投资者通知模板回报			
void UserSpi::OnRtnDelInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) 			
{			
	LOG_INFO("OnRtnDelInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);	
	SAFE_ADD_MSG(MSGID_RtnDelInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern );		
}			


///风险通知模板自动替换字段回报			
void UserSpi::OnRtnRiskNotifyToken(CShfeFtdcRiskNotifyTokenField *pRiskNotifyToken) 			
{			
	LOG_INFO("OnRtnRiskNotifyToken");		
	CONVERTSTRUCTFROMCTP(RiskNotifyToken);	
	SAFE_ADD_MSG(MSGID_RtnRiskNotifyToken,sizeof(RiskNotifyTokenField),npRiskNotifyToken );		
}			


///有序的风险通知回报版本B，对应订阅请求ReqSubSeqRiskNotifyB			
void UserSpi::OnRtnSeqRiskNotifyB(CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB) 			
{			
	LOG_INFO("OnRtnSeqRiskNotifyB");		
	CONVERTSTRUCTFROMCTP(SeqRiskNotifyB);	
	SAFE_ADD_MSG(MSGID_RtnSeqRiskNotifyB,sizeof(SeqRiskNotifyBField),npSeqRiskNotifyB );		
}			


///查询持仓排名应答			
void UserSpi::OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryPositionStat");		
	CONVERTSTRUCTFROMCTP(PositionStat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryPositionStat,sizeof(PositionStatField),npPositionStat,npRspInfo,nRequestID,bIsLast);		
}			


///查询成交排名应答			
void UserSpi::OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryTradeStat");		
	CONVERTSTRUCTFROMCTP(TradeStat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryTradeStat,sizeof(TradeStatField),npTradeStat,npRspInfo,nRequestID,bIsLast);		
}			


///投资者及联系人信息哈希值查询应答			
void UserSpi::OnRspQryInvestorLinkManHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorLinkManHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorLinkManHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///查询投资者组织架构Hash值应答			
void UserSpi::OnRspQryInvestorDepartmentHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorDepartmentHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorDepartmentHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///压力测试应答			
void UserSpi::OnRspQryStressTest(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryStressTest");		
	CONVERTSTRUCTFROMCTP(InvestorRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryStressTest,sizeof(InvestorRiskAccountField),npInvestorRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///查询低保投资者hash值应答			
void UserSpi::OnRspQryLowMarginInvestorHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryLowMarginInvestorHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryLowMarginInvestorHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///查询低保投资者应答			
void UserSpi::OnRspQryLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryLowMarginInvestor");		
	CONVERTSTRUCTFROMCTP(BrokerInvestor);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryLowMarginInvestor,sizeof(BrokerInvestorField),npBrokerInvestor,npRspInfo,nRequestID,bIsLast);		
}			


///低保投资者回报			
void UserSpi::OnRtnLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) 			
{			
	LOG_INFO("OnRtnLowMarginInvestor");		
	CONVERTSTRUCTFROMCTP(BrokerInvestor);	
	SAFE_ADD_MSG(MSGID_RtnLowMarginInvestor,sizeof(BrokerInvestorField),npBrokerInvestor );		
}			


///删除低保投资者回报			
void UserSpi::OnRtnDelLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) 			
{			
	LOG_INFO("BrokerInvestor");		
	CONVERTSTRUCTFROMCTP(BrokerInvestor);	
	SAFE_ADD_MSG(MSGID_RtnDelLowMarginInvestor,sizeof(BrokerInvestorField),npBrokerInvestor );		
}			
