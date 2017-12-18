#include "StdAfx.h"
#include "FtdcRiskUserApi.h"
#include "UserApi.h"
#include "UserSpi.h"
#include "Convert2Ctp.h"
#define CONVERTSTRUCT2CTP(TYPE)\
	CShfeFtdc##TYPE##Field* np##TYPE=new CShfeFtdc##TYPE##Field();\
	CSafePtr<CShfeFtdc##TYPE##Field> atuo_p##TYPE(np##TYPE);\
	Convert2Ctp::Get##TYPE##Field(*p##TYPE,*np##TYPE);


#define CONVERTSTRUCT2CTP_ARRAY(TYPE,CNT)\
	CShfeFtdc##TYPE##Field* np##TYPE=new CShfeFtdc##TYPE##Field[CNT];\
	CSafeArrayPtr<CShfeFtdc##TYPE##Field> atuo_p##TYPE(np##TYPE);\
	for (int i=0;i<CNT;++i)\
	{\
	Convert2Ctp::Get##TYPE##Field(*(p##TYPE+i),*(np##TYPE+i));\
	}

UserApi::UserApi(const char *pszFlowPath)
:m_Api(NULL)
{
	m_pMaxNo=new MaxNo();
	m_Api=CShfeFtdcRiskUserApi::CreateFtdcRiskUserApi(pszFlowPath);
}
UserApi::~UserApi()
{
	SAFE_DELETE(m_pMaxNo);
}
UserApi* UserApi::CreateUserApi(const char *pszFlowPath)
{
	return new UserApi(pszFlowPath);
}
///删除接口对象本身
void UserApi::Release()
{
	m_Api->RegisterSpi(NULL);
	m_Api->Release();
	SAFE_DELETE(m_Spi);
	delete this;
}
///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void UserApi::Init()
{
	m_Api->Init();
}

///等待接口线程结束运行
///@return 线程退出代码
int UserApi::Join()
{
	return m_Api->Join();
}

///获取当前交易日
///@retrun 获取到的交易日
///@remark 只有登录成功后,才能得到正确的交易日
const char *UserApi::GetTradingDay()
{
	m_strTradingDay=m_Api->GetTradingDay();
	return m_strTradingDay.c_str();
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void UserApi::RegisterFront(char *pszFrontAddress)
{
	m_Api->RegisterFront(pszFrontAddress);
}

///注册回调接口
///@param pSpi 派生自回调接口类的实例
void UserApi::RegisterSpi()
{
	m_Spi=new UserSpi(*m_pMaxNo);
	m_Api->RegisterSpi(static_cast<CShfeFtdcRiskUserSpi*>(m_Spi));
}

///订阅市场行情。
int UserApi::ReqRiskUserLogin(ReqRiskUserLoginField *pReqRiskUserLogin)
{
	CONVERTSTRUCT2CTP(ReqRiskUserLogin);										
	return m_Api->ReqRiskUserLogin(npReqRiskUserLogin,GetNextID());
}
///查询投资者保证金率
int UserApi::ReqQryInvestorMarginRate(QryInvestorMarginRateField *pQryInvestorMarginRate)
{
	CONVERTSTRUCT2CTP(QryInvestorMarginRate);										
	return  m_Api->ReqQryInvestorMarginRate(npQryInvestorMarginRate,GetNextID());
}

///查询报单排名请求
int UserApi::ReqQryOrderStat(QryStatField *pQryStat)
{
	CONVERTSTRUCT2CTP(QryStat);										
	return  m_Api->ReqQryOrderStat(npQryStat,GetNextID());										
}

///订阅风控实时行情
int UserApi::ReqSubRiskMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt)
{	
	CONVERTSTRUCT2CTP_ARRAY(SubMarketData,SubMarketDataCnt);										
	return m_Api->ReqSubRiskMarketData(npSubMarketData, SubMarketDataCnt,GetNextID());
}

///退订风控实时行情
int UserApi::ReqUnSubRiskMarketData(SubMarketDataField *pSubMarketData)
{
	CONVERTSTRUCT2CTP(SubMarketData);										
	return m_Api->ReqUnSubRiskMarketData(npSubMarketData,GetNextID());
}

///合约持仓比例查询
int UserApi::ReqQryInstPositionRate(QryInstPositionRateField *pQryInstPositionRate, int QryInstPositionRateCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(QryInstPositionRate,QryInstPositionRateCnt);										
	return  m_Api->ReqQryInstPositionRate(npQryInstPositionRate, QryInstPositionRateCnt,GetNextID());
}

///产品持仓比例查询
int UserApi::ReqQryProductPositionRate(QryProductPositionRateField *pQryProductPositionRate)
{
	CONVERTSTRUCT2CTP(QryProductPositionRate);										
	return  m_Api->ReqQryProductPositionRate(npQryProductPositionRate,GetNextID());
}

///查询交易编码Hash值请求
// InvestorIDBeg    起始投资者代码，为空表示用户权限下的最小投资者代码
// InvestorIdEnd    结束投资者代码，为空表示用户权限下的最大投资者代码
// 如果InvestorIDBeg和InvestorIDEnd都为空，则服务器会自动分多个区间返回哈希值；否则只计算请求区间一个哈希值
// hash值计算方法：
int UserApi::ReqQryTradingCodeHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);										
	return  m_Api->ReqQryTradingCodeHash(npInvestorIDRange,GetNextID());
}

///查询交易编码请求，使用方法类似ReqQryInvestorLinkMan
int UserApi::ReqQryTradingCode(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);										
	return  m_Api->ReqQryTradingCode(npInvestorIDRange, InvestorIDRangeCnt,GetNextID());
}

///订阅有序的成交，可根据已接收的RtnSequencialTrade中最大的UniqSequenceNo来实现续订
int UserApi::ReqSubscribeTrade(NotifySequenceField *pNotifySequence)
{
	if(pNotifySequence&&m_pMaxNo)
		pNotifySequence->SequenceNo=m_pMaxNo->MaxTradeNo;
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubscribeTrade(npNotifySequence,GetNextID());
}

///订阅有序的成交，可根据已接收的RtnSequencialOrder中最大的UniqSequenceNo来实现续订
int UserApi::ReqSubscribeOrder(NotifySequenceField *pNotifySequence)
{
	if(pNotifySequence&&m_pMaxNo)
		pNotifySequence->SequenceNo=m_pMaxNo->MaxOrderNo;
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubscribeOrder(npNotifySequence,GetNextID());
}

///订阅有序的经纪公司用户事件，可根据已接收的RtnSequencialBrokerUserEvent中最大的UniqSequenceNo来实现续订
int UserApi::ReqSubBrokerUserEvent(NotifySequenceField *pNotifySequence)
{
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubBrokerUserEvent(npNotifySequence,GetNextID());
}

///风控报单录入请求
int UserApi::ReqRiskOrderInsert(RiskForceCloseOrderField *pRiskForceCloseOrder)
{
	CONVERTSTRUCT2CTP(RiskForceCloseOrder);										
	return m_Api->ReqRiskOrderInsert(npRiskForceCloseOrder,GetNextID());
}

///风控报单操作请求
int UserApi::ReqRiskOrderAction(InputOrderActionField *pInputOrderAction)
{
	CONVERTSTRUCT2CTP(InputOrderAction);										
	return m_Api->ReqRiskOrderAction(npInputOrderAction,GetNextID());
}

///订阅有序的持仓，可根据已接收的RtnSequencialPosition中最大的UniqSequenceNo来实现续订
int UserApi::ReqSubscribePosition(NotifySequenceField *pNotifySequence)
{
	if(pNotifySequence&&m_pMaxNo)
		pNotifySequence->SequenceNo=m_pMaxNo->MaxPositionNo;
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubscribePosition(npNotifySequence,GetNextID());
}

///客户风险通知发送命令请求
// v4.2之前 RiskNotifyCommand.Reserve为空，只能发送系统通知
// v4.2 RiskNotifyCommand.Reserve=NoDup或Dup，可以发送系统或邮件通知。NoDup-不允许重复发送更低或相同级别风险通知；Dup-允许重复发送更低或相同级别风险通知
// v5.2 增加IsNormal和IsWarn字段
int UserApi::ReqRiskNotifyCommand(RiskNotifyCommandField *pRiskNotifyCommand)
{
	CONVERTSTRUCT2CTP(RiskNotifyCommand);										
	return m_Api->ReqRiskNotifyCommand(npRiskNotifyCommand,GetNextID());
}

///批量强平计算请求, version:{6,v541},{7:v551}
int UserApi::ReqBatchForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionRuleField *pForceClosePositionRule, int ForceClosePositionRuleCnt, 
									ForceCloseListField *pForceCloseList, int ForceCloseListCnt)		
{			
	CONVERTSTRUCT2CTP(ForceCloseStandard);								
	CONVERTSTRUCT2CTP_ARRAY(ForceClosePositionRule,ForceClosePositionRuleCnt);	
	CONVERTSTRUCT2CTP_ARRAY(ForceCloseList,ForceCloseListCnt);	
	return m_Api->ReqBatchForceCloseCalc(npForceCloseStandard,npForceClosePositionRule,
		ForceClosePositionRuleCnt,npForceCloseList,ForceCloseListCnt,GetNextID());									
}

///单个投资者强平计算请求, version:{6,v541},{7:v551}
int UserApi::ReqForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionField *pForceClosePosition, int ForceClosePositionCnt, 
							   ForceCloseListField *pForceCloseList)				
{
	CONVERTSTRUCT2CTP(ForceCloseStandard);	
	CONVERTSTRUCT2CTP_ARRAY(ForceClosePosition,ForceClosePositionCnt);	
	CONVERTSTRUCT2CTP(ForceCloseList);	
	return m_Api->ReqForceCloseCalc(npForceCloseStandard, npForceClosePosition,ForceClosePositionCnt, npForceCloseList,GetNextID()); 
}

///净持仓保证金指标参数设置请求；对应应答RspSetIndexNPPPara，对应回报RtnIndexNPP
int UserApi::ReqSetIndexNPPPara(IndexNPPField *pIndexNPP)
{
	CONVERTSTRUCT2CTP(IndexNPP);										
	return m_Api->ReqSetIndexNPPPara(npIndexNPP,GetNextID());
}

///删除净持仓保证金指标参数请求；对应应答RspRiskDelIndexNPPPara，对应回报RtnDelIndexNPP
int UserApi::ReqRiskDelIndexNPPPara(IndexNPPField *pIndexNPP)
{
	CONVERTSTRUCT2CTP(IndexNPP);										
	return m_Api->ReqRiskDelIndexNPPPara(npIndexNPP,GetNextID());
}

///查询登陆请求
int UserApi::ReqQryLogin(NormalRiskQueryField *pNormalRiskQuery)
{
	CONVERTSTRUCT2CTP(NormalRiskQuery);										
	return  m_Api->ReqQryLogin(npNormalRiskQuery,GetNextID());										
}

///查询价格波动对投资者风险的影响
int UserApi::ReqQryPriceVaryEffect(QryPriceVaryEffectField *pQryPriceVaryEffect)
{
	CONVERTSTRUCT2CTP(QryPriceVaryEffect);										
	return  m_Api->ReqQryPriceVaryEffect(npQryPriceVaryEffect,GetNextID());
}

///权益反向计算请，应答接口RspQrySafePriceRange和RspSafePriceAccount
int UserApi::ReqCalSafePriceRange(QrySafePriceRangeField *pQrySafePriceRange, PriceVaryParamField *pPriceVaryParam, int PriceVaryParamCnt, 
								  BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt)			
{
	CONVERTSTRUCT2CTP(QrySafePriceRange);	
	CONVERTSTRUCT2CTP_ARRAY(PriceVaryParam,PriceVaryParamCnt);
	CONVERTSTRUCT2CTP_ARRAY(BrokerInvestor,BrokerInvestorCnt);
	return m_Api->ReqCalSafePriceRange(npQrySafePriceRange,npPriceVaryParam, PriceVaryParamCnt,npBrokerInvestor,BrokerInvestorCnt,GetNextID());
}

///风控预埋单录入请求
int UserApi::ReqRiskParkedOrderInsert(RiskParkedOrderField *pRiskParkedOrder)
{
	CONVERTSTRUCT2CTP(RiskParkedOrder);										
	return m_Api->ReqRiskParkedOrderInsert(npRiskParkedOrder,GetNextID());
}

///请求删除预埋单
int UserApi::ReqRemoveRiskParkedOrder(RemoveRiskParkedOrderField *pRemoveRiskParkedOrder)
{
	CONVERTSTRUCT2CTP(RemoveRiskParkedOrder);										
	return m_Api->ReqRemoveRiskParkedOrder(npRemoveRiskParkedOrder,GetNextID());
}

///订阅有序的风控预埋单
int UserApi::ReqSubRiskParkedOrder(NotifySequenceField *pNotifySequence)
{
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubRiskParkedOrder(npNotifySequence,GetNextID());
}

///风控用户密码修改请求
int UserApi::ReqRiskUserPasswordUpd(UserPasswordUpdateField *pUserPasswordUpdate)
{
	CONVERTSTRUCT2CTP(UserPasswordUpdate);										
	return m_Api->ReqRiskUserPasswordUpd(npUserPasswordUpdate,GetNextID());
}

///订阅有序的出入金，可根据已接收的RtnSeqDeposit中最大的UniqSequenceNo来实现续订
int UserApi::ReqSubSeqDeposit(RiskNtfSequenceField *pRiskNtfSequence)
{
	if(pRiskNtfSequence&&m_pMaxNo)
		pRiskNtfSequence->SequenceNo=m_pMaxNo->MaxDepositNo;
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubSeqDeposit(npRiskNtfSequence,GetNextID());
}

///插入风控用户事件请求
int UserApi::ReqAddRiskUserEvent(RiskUserEventField *pRiskUserEvent)
{
	CONVERTSTRUCT2CTP(RiskUserEvent);										
	return m_Api->ReqAddRiskUserEvent(npRiskUserEvent,GetNextID());
}

///风险预算请求(InstrumentPrice在v5.1_2之后无效)
int UserApi::ReqPredictRisk(PredictRiskParamField *pPredictRiskParam, 
							BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt, 
							ProductLimitsField *pProductLimits, int ProductLimitsCnt, 				
							InstrumentPriceField *pInstrumentPrice, int InstrumentPriceCnt)				
{
	CONVERTSTRUCT2CTP(PredictRiskParam);	
	CONVERTSTRUCT2CTP_ARRAY(BrokerInvestor,BrokerInvestorCnt);
	CONVERTSTRUCT2CTP_ARRAY(ProductLimits,ProductLimitsCnt);
	CONVERTSTRUCT2CTP_ARRAY(InstrumentPrice,InstrumentPriceCnt);
	return m_Api->ReqPredictRisk(npPredictRiskParam, npBrokerInvestor, BrokerInvestorCnt,
		npProductLimits,ProductLimitsCnt,npInstrumentPrice,InstrumentPriceCnt,GetNextID());								
}

///查询投资者和联系人信息请求
int UserApi::ReqQryInvestorLinkMan(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);										
	return  m_Api->ReqQryInvestorLinkMan(npInvestorIDRange, InvestorIDRangeCnt,GetNextID());
}
///查询组织架构投资者对应关系请求，使用方法类似ReqQryInvestorLinkMan
int UserApi::ReqQryInvestorDepartment(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);										
	return  m_Api->ReqQryInvestorDepartment(npInvestorIDRange, InvestorIDRangeCnt,GetNextID());
}
///订阅有序的昨日资金，可根据已接收的RtnSeqPreRiskAccount中最大的UniqSequenceNo来实现续订；当日资金使用RtnRiskSyncAccount
int UserApi::ReqSubPreRiskAccount(RiskNtfSequenceField *pRiskNtfSequence)
{
	if(pRiskNtfSequence&&m_pMaxNo)
		pRiskNtfSequence->SequenceNo=m_pMaxNo->MaxPreRiskAccountNo;
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubPreRiskAccount(npRiskNtfSequence,GetNextID());
}
///更新通知模版请求
int UserApi::ReqModNoticePattern(NoticePatternField *pNoticePattern)
{
	CONVERTSTRUCT2CTP(NoticePattern);										
	return m_Api->ReqModNoticePattern(npNoticePattern,GetNextID());
}

///订阅变化行情请求，目前只订阅最新价
// 订阅之后，最新价变化时会发送RtnVaryMarketData回报，一种情况除外：上次最新价和最新价都介于PriceRange.Price1和Price2之间时，不发送RtnVaryMarketData回报
int UserApi::ReqSubVaryMarketData(PriceRangeField *pPriceRange, int PriceRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(PriceRange,PriceRangeCnt);										
	return m_Api->ReqSubVaryMarketData(npPriceRange, PriceRangeCnt,GetNextID());
}

///退订变化行情请求，目前只退订最新价
int UserApi::ReqUnSubVaryMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(SubMarketData,SubMarketDataCnt);										
	return m_Api->ReqUnSubVaryMarketData(npSubMarketData, SubMarketDataCnt,GetNextID());
}

///新增风险通知请求
int UserApi::ReqAddRiskNotifyA(RiskNotifyAField *pRiskNotifyA)
{
	CONVERTSTRUCT2CTP(RiskNotifyA);										
	return m_Api->ReqAddRiskNotifyA(npRiskNotifyA,GetNextID());
}

///新增业务通知请求
int UserApi::ReqAddBizNotice(BizNoticeField *pBizNotice)
{
	CONVERTSTRUCT2CTP(BizNotice);										
	return m_Api->ReqAddBizNotice(npBizNotice,GetNextID());
}

///订阅有序数据请求
int UserApi::ReqSubSeqData(RiskNtfSequenceField *pRiskNtfSequence)
{
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubSeqData(npRiskNtfSequence,GetNextID());
}

///查询席位资金请求
int UserApi::ReqRiskQryBrokerDeposit(QueryBrokerDepositField *pQueryBrokerDeposit)
{
	CONVERTSTRUCT2CTP(QueryBrokerDeposit);										
	return m_Api->ReqRiskQryBrokerDeposit(npQueryBrokerDeposit,GetNextID());
}

///修改投资者参数请求
int UserApi::ReqModRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam)
{
	CONVERTSTRUCT2CTP(RiskInvestorParam);										
	return m_Api->ReqModRiskInvestorParam(npRiskInvestorParam,GetNextID());
}

///删除投资者参数请求
int UserApi::ReqRemRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam)
{
	CONVERTSTRUCT2CTP(RiskInvestorParam);										
	return m_Api->ReqRemRiskInvestorParam(npRiskInvestorParam,GetNextID());
}

///强制风控用户登出请求
int UserApi::ReqForceRiskUserLogout(RiskLoginInfoField *pRiskLoginInfo)
{
	CONVERTSTRUCT2CTP(RiskLoginInfo);										
	return m_Api->ReqForceRiskUserLogout(npRiskLoginInfo,GetNextID());
}

///增加风控通知模板请求
int UserApi::ReqAddRiskPattern(RiskPatternField *pRiskPattern)										
{
	CONVERTSTRUCT2CTP(RiskPattern);										
	return m_Api->ReqAddRiskPattern(npRiskPattern,GetNextID());
}

///修改风控通知模板请求
int UserApi::ReqModRiskPattern(RiskPatternField *pRiskPattern)
{
	CONVERTSTRUCT2CTP(RiskPattern);										
	return m_Api->ReqModRiskPattern(npRiskPattern,GetNextID());
}

///删除风控通知模板请求
int UserApi::ReqRemRiskPattern(RiskPatternField *pRiskPattern)
{
	CONVERTSTRUCT2CTP(RiskPattern);										
	return m_Api->ReqRemRiskPattern(npRiskPattern,GetNextID());
}

///增加投资者通知模板请求
int UserApi::ReqAddInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorPattern,InvestorPatternCnt);										
	return m_Api->ReqAddInvestorPattern(npInvestorPattern, InvestorPatternCnt,GetNextID());
}

///修改投资者通知模板请求
int UserApi::ReqModInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorPattern,InvestorPatternCnt);										
	return m_Api->ReqModInvestorPattern(npInvestorPattern, InvestorPatternCnt,GetNextID())	;										
}

///删除投资者通知模板请求
int UserApi::ReqRemInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorPattern,InvestorPatternCnt);										
	return m_Api->ReqRemInvestorPattern(npInvestorPattern, InvestorPatternCnt,GetNextID());
}

///订阅有序的风险通知版本B，可根据已接收的RtnSeqRiskNotifyB中最大的UniqSequenceNo来实现续订
int UserApi::ReqSubSeqRiskNotifyB(RiskNtfSequenceField *pRiskNtfSequence)
{
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubSeqRiskNotifyB(npRiskNtfSequence,GetNextID());
}

///查询持仓排名请求
int UserApi::ReqQryPositionStat(QryStatField *pQryStat)
{
	CONVERTSTRUCT2CTP(QryStat);	
	return  m_Api->ReqQryPositionStat(npQryStat,GetNextID());
}

///查询成交排名请求
int UserApi::ReqQryTradeStat(QryStatField *pQryStat)
{
	CONVERTSTRUCT2CTP(QryStat);
	return  m_Api->ReqQryTradeStat(npQryStat,GetNextID());
}

///投资者及联系人信息哈希值查询请求
// InvestorIDBeg    起始投资者代码，为空表示用户权限下的最小投资者代码
// InvestorIdEnd    结束投资者代码，为空表示用户权限下的最大投资者代码
// 如果InvestorIDBeg和InvestorIDEnd都为空，则服务器会自动分多个区间返回哈希值；否则只计算请求区间一个哈希值
int UserApi::ReqQryInvestorLinkManHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);
	return  m_Api->ReqQryInvestorLinkManHash(npInvestorIDRange,GetNextID());
}

///查询投资者组织架构Hash值请求
// InvestorIDBeg    起始投资者代码，为空表示用户权限下的最小投资者代码
// InvestorIdEnd    结束投资者代码，为空表示用户权限下的最大投资者代码
// 如果InvestorIDBeg和InvestorIDEnd都为空，则服务器会自动分多个区间返回哈希值；否则只计算请求区间一个哈希值
int UserApi::ReqQryInvestorDepartmentHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);
	return  m_Api->ReqQryInvestorDepartmentHash(npInvestorIDRange,GetNextID());
}

///压力测试请求
int UserApi::ReqQryStressTest(STPriceField *pSTPrice, int STPriceCnt, 
							  STMarginRateField *pSTMarginRate, int STMarginRateCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(STPrice,STPriceCnt);
	CONVERTSTRUCT2CTP_ARRAY(STMarginRate,STMarginRateCnt);
	return  m_Api->ReqQryStressTest(npSTPrice,STPriceCnt,npSTMarginRate,STMarginRateCnt,GetNextID());
}

///查询低保投资者hash值请求
int UserApi::ReqQryLowMarginInvestorHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);
	return  m_Api->ReqQryLowMarginInvestorHash(npInvestorIDRange,GetNextID());
}

///查询低保投资者请求
int UserApi::ReqQryLowMarginInvestor(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);
	return  m_Api->ReqQryLowMarginInvestor(npInvestorIDRange,InvestorIDRangeCnt,GetNextID());
}

int UserApi::GetNextID()
{
	return 0;
}
