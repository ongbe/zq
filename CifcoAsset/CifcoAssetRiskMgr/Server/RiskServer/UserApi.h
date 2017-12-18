#pragma once
class CShfeFtdcRiskUserApi;
class UserSpi;
class MaxNo;
class UserApi
{
public:
	///创建UserApi
	static UserApi *CreateUserApi(const char *pszFlowPath = "");
	///删除接口对象本身
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
	const char *GetTradingDay();

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	void RegisterFront(char *pszFrontAddress);

	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterSpi();

	///订阅市场行情。
	int ReqRiskUserLogin(ReqRiskUserLoginField *pReqRiskUserLogin);



	///查询投资者保证金率
	int ReqQryInvestorMarginRate(QryInvestorMarginRateField *pQryInvestorMarginRate);

	///查询报单排名请求
	int ReqQryOrderStat(QryStatField *pQryStat);

	///订阅风控实时行情
	int ReqSubRiskMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt);

	///退订风控实时行情
	int ReqUnSubRiskMarketData(SubMarketDataField *pSubMarketData);

	///合约持仓比例查询
	int ReqQryInstPositionRate(QryInstPositionRateField *pQryInstPositionRate, int QryInstPositionRateCnt);

	///产品持仓比例查询
	int ReqQryProductPositionRate(QryProductPositionRateField *pQryProductPositionRate);

	///查询交易编码Hash值请求
	// InvestorIDBeg    起始投资者代码，为空表示用户权限下的最小投资者代码
	// InvestorIdEnd    结束投资者代码，为空表示用户权限下的最大投资者代码
	// 如果InvestorIDBeg和InvestorIDEnd都为空，则服务器会自动分多个区间返回哈希值；否则只计算请求区间一个哈希值
	// hash值计算方法：
	int ReqQryTradingCodeHash(InvestorIDRangeField *pInvestorIDRange);

	///查询交易编码请求，使用方法类似ReqQryInvestorLinkMan
	int ReqQryTradingCode(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);

	///订阅有序的成交，可根据已接收的RtnSequencialTrade中最大的UniqSequenceNo来实现续订
	int ReqSubscribeTrade(NotifySequenceField *pNotifySequence);

	///订阅有序的成交，可根据已接收的RtnSequencialOrder中最大的UniqSequenceNo来实现续订
	int ReqSubscribeOrder(NotifySequenceField *pNotifySequence);

	///订阅有序的经纪公司用户事件，可根据已接收的RtnSequencialBrokerUserEvent中最大的UniqSequenceNo来实现续订
	int ReqSubBrokerUserEvent(NotifySequenceField *pNotifySequence);

	///风控报单录入请求
	int ReqRiskOrderInsert(RiskForceCloseOrderField *pRiskForceCloseOrder);

	///风控报单操作请求
	int ReqRiskOrderAction(InputOrderActionField *pInputOrderAction);

	///订阅有序的持仓，可根据已接收的RtnSequencialPosition中最大的UniqSequenceNo来实现续订
	int ReqSubscribePosition(NotifySequenceField *pNotifySequence);

	///客户风险通知发送命令请求
	// v4.2之前 RiskNotifyCommand.Reserve为空，只能发送系统通知
	// v4.2 RiskNotifyCommand.Reserve=NoDup或Dup，可以发送系统或邮件通知。NoDup-不允许重复发送更低或相同级别风险通知；Dup-允许重复发送更低或相同级别风险通知
	// v5.2 增加IsNormal和IsWarn字段
	int ReqRiskNotifyCommand(RiskNotifyCommandField *pRiskNotifyCommand);

	///批量强平计算请求, version:{6,v541},{7:v551}
	int ReqBatchForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionRuleField *pForceClosePositionRule, int ForceClosePositionRuleCnt, 
		ForceCloseListField *pForceCloseList, int ForceCloseListCnt);

	///单个投资者强平计算请求, version:{6,v541},{7:v551}
	int ReqForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionField *pForceClosePosition, int ForceClosePositionCnt, 
		ForceCloseListField *pForceCloseList);

	///净持仓保证金指标参数设置请求；对应应答RspSetIndexNPPPara，对应回报RtnIndexNPP
	int ReqSetIndexNPPPara(IndexNPPField *pIndexNPP);

	///删除净持仓保证金指标参数请求；对应应答RspRiskDelIndexNPPPara，对应回报RtnDelIndexNPP
	int ReqRiskDelIndexNPPPara(IndexNPPField *pIndexNPP);

	///查询登陆请求
	int ReqQryLogin(NormalRiskQueryField *pNormalRiskQuery);

	///查询价格波动对投资者风险的影响
	int ReqQryPriceVaryEffect(QryPriceVaryEffectField *pQryPriceVaryEffect);

	///权益反向计算请，应答接口RspQrySafePriceRange和RspSafePriceAccount
	int ReqCalSafePriceRange(QrySafePriceRangeField *pQrySafePriceRange, PriceVaryParamField *pPriceVaryParam, int PriceVaryParamCnt, 
		BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt);

	///风控预埋单录入请求
	int ReqRiskParkedOrderInsert(RiskParkedOrderField *pRiskParkedOrder);

	///请求删除预埋单
	int ReqRemoveRiskParkedOrder(RemoveRiskParkedOrderField *pRemoveRiskParkedOrder);

	///订阅有序的风控预埋单
	int ReqSubRiskParkedOrder(NotifySequenceField *pNotifySequence);

	///风控用户密码修改请求
	int ReqRiskUserPasswordUpd(UserPasswordUpdateField *pUserPasswordUpdate);

	///订阅有序的出入金，可根据已接收的RtnSeqDeposit中最大的UniqSequenceNo来实现续订
	int ReqSubSeqDeposit(RiskNtfSequenceField *pRiskNtfSequence);

	///插入风控用户事件请求
	int ReqAddRiskUserEvent(RiskUserEventField *pRiskUserEvent);

	///风险预算请求(InstrumentPrice在v5.1_2之后无效)
	int ReqPredictRisk(PredictRiskParamField *pPredictRiskParam, BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt, 
		ProductLimitsField *pProductLimits, int ProductLimitsCnt, 
		InstrumentPriceField *pInstrumentPrice, int InstrumentPriceCnt);

	///查询投资者和联系人信息请求
	int ReqQryInvestorLinkMan(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);
	///查询组织架构投资者对应关系请求，使用方法类似ReqQryInvestorLinkMan
	int ReqQryInvestorDepartment(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);







	///订阅有序的昨日资金，可根据已接收的RtnSeqPreRiskAccount中最大的UniqSequenceNo来实现续订；当日资金使用RtnRiskSyncAccount
	int ReqSubPreRiskAccount(RiskNtfSequenceField *pRiskNtfSequence);





	///更新通知模版请求
	int ReqModNoticePattern(NoticePatternField *pNoticePattern);

	///订阅变化行情请求，目前只订阅最新价
	// 订阅之后，最新价变化时会发送RtnVaryMarketData回报，一种情况除外：上次最新价和最新价都介于PriceRange.Price1和Price2之间时，不发送RtnVaryMarketData回报
	int ReqSubVaryMarketData(PriceRangeField *pPriceRange, int PriceRangeCnt);

	///退订变化行情请求，目前只退订最新价
	int ReqUnSubVaryMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt);

	///新增风险通知请求
	int ReqAddRiskNotifyA(RiskNotifyAField *pRiskNotifyA);

	///新增业务通知请求
	int ReqAddBizNotice(BizNoticeField *pBizNotice);

	///订阅有序数据请求
	int ReqSubSeqData(RiskNtfSequenceField *pRiskNtfSequence);

	///查询席位资金请求
	int ReqRiskQryBrokerDeposit(QueryBrokerDepositField *pQueryBrokerDeposit);

	///修改投资者参数请求
	int ReqModRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam);

	///删除投资者参数请求
	int ReqRemRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam);

	///强制风控用户登出请求
	int ReqForceRiskUserLogout(RiskLoginInfoField *pRiskLoginInfo);

	///增加风控通知模板请求
	int ReqAddRiskPattern(RiskPatternField *pRiskPattern);

	///修改风控通知模板请求
	int ReqModRiskPattern(RiskPatternField *pRiskPattern);

	///删除风控通知模板请求
	int ReqRemRiskPattern(RiskPatternField *pRiskPattern);

	///增加投资者通知模板请求
	int ReqAddInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt);

	///修改投资者通知模板请求
	int ReqModInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt);

	///删除投资者通知模板请求
	int ReqRemInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt);

	///订阅有序的风险通知版本B，可根据已接收的RtnSeqRiskNotifyB中最大的UniqSequenceNo来实现续订
	int ReqSubSeqRiskNotifyB(RiskNtfSequenceField *pRiskNtfSequence);

	///查询持仓排名请求
	int ReqQryPositionStat(QryStatField *pQryStat);

	///查询成交排名请求
	int ReqQryTradeStat(QryStatField *pQryStat);

	///投资者及联系人信息哈希值查询请求
	// InvestorIDBeg    起始投资者代码，为空表示用户权限下的最小投资者代码
	// InvestorIdEnd    结束投资者代码，为空表示用户权限下的最大投资者代码
	// 如果InvestorIDBeg和InvestorIDEnd都为空，则服务器会自动分多个区间返回哈希值；否则只计算请求区间一个哈希值
	int ReqQryInvestorLinkManHash(InvestorIDRangeField *pInvestorIDRange);

	///查询投资者组织架构Hash值请求
	// InvestorIDBeg    起始投资者代码，为空表示用户权限下的最小投资者代码
	// InvestorIdEnd    结束投资者代码，为空表示用户权限下的最大投资者代码
	// 如果InvestorIDBeg和InvestorIDEnd都为空，则服务器会自动分多个区间返回哈希值；否则只计算请求区间一个哈希值
	int ReqQryInvestorDepartmentHash(InvestorIDRangeField *pInvestorIDRange);

	///压力测试请求
	int ReqQryStressTest(STPriceField *pSTPrice, int STPriceCnt, 
		STMarginRateField *pSTMarginRate, int STMarginRateCnt);

	///查询低保投资者hash值请求
	int ReqQryLowMarginInvestorHash(InvestorIDRangeField *pInvestorIDRange);

	///查询低保投资者请求
	int ReqQryLowMarginInvestor(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);
protected:
	UserApi(const char *pszFlowPath);
	~UserApi();
	int GetNextID();
	MaxNo* m_pMaxNo;
private:
	CShfeFtdcRiskUserApi* m_Api;
	UserSpi* m_Spi;
	std::string m_strTradingDay;
	template<class T>
	class CSafePtr
	{
	public:
		CSafePtr(T* p):m_ptr(p){}
		~CSafePtr(){if(m_ptr) delete m_ptr;}
	private:
		T* m_ptr;
	};
	template<class T>
	class CSafeArrayPtr
	{
	public:
		CSafeArrayPtr(T* p):m_ptr(p){}
		~CSafeArrayPtr(){if(m_ptr) delete[] m_ptr;}
	private:
		T* m_ptr;
	};
};
