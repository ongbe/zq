机构交易系统项目

开始时间：2013.1

服务端：
	SvrTcp			:	网络层处理模块
	SvrBrokerInfo		:	经纪公司及委托交易账号管理
	SvrUserOrg		:   	用户、组织机构、权限、角色等管理
	SvrFinanceProduct	:	理财产品管理模块
	SvrTradingStrategy	:	交易策略管理模块
	SvrTradeData		:	交易数据管理模块
	SvrTradeSupport		:	交易支持模块，实时维护用户的持仓、持仓明细、资金等数据，向交易终端提供这些数据
	SvrLogin		:     	登录模块
	SvrRisk			:	风控业务模块
	SvrOffer		:	报盘模块，含CTP风控接口报盘和交易终端推送报盘
	SvrTSS			:	交易辅助管理
	SvrSettlement		:	结算服务
	SvrMsg			:	消息管理模块
	svrStatistics		:	统计服务
	SvrPresentation		:	展现服务

客户端：
	FastTrader		:     	交易终端
	CltRisk			:     	风控终端
	CltServerMgr		:  	系统管理终端
	CltTSS			:	交易辅助管理终端
	CltPresentation		:	展现终端