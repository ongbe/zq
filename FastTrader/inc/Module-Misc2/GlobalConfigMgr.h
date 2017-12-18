#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

#ifdef WIN32
#pragma managed(push,off)
#endif 

class GlobalConfigManager
{
public:
	static void Load();

	static bool IsCalculate_OnRtnTrade();

    //获取成交回报后是否触发的参数
    static bool IsReq_OnRtnTrade(void);

    //获取成交回报后触发查询的延迟秒数
    static int GetDelayBefReq_OnRtnTrade(void);

    //获取成交触发查询持仓明细的最大记录数量阀值，超过这个值就不自动查询持仓明细
    static int GetMaxRecordReqPositionDetail_OnRtnTrade(void);

    //列表框自动排序时，最大行数。超过这个行数，停止自动排序，以免影响速度
    static int GetMaxItemCountForAutoResort_ListCtrl(void);

    //是否启动CifcoServicesHall功能
	static bool IsCifcoServicesHall_ExtraFunction();
	

    //对状态为未知的报单(如手工审核单)，是否执行资金冻结
    static bool IsFreezeOrderUnknownStatus();

    // 标准下单面板
	static bool IsOrderType_UIOT_PANE_STANDARD();
	// 批量下单面板
    static bool IsOrderType_UIOT_PANE_BATCH();
	// 条件单下单面板
    static bool IsOrderType_UIOT_PANE_CONDITION();
	// Fak/fok下单面板
    static bool IsOrderType_UIOT_PANE_FAKFOK();
	// 盈损单下单面板
    static bool IsOrderType_UIOT_PANE_PROFIT();
	// 停止单下单面板
    static bool IsOrderType_UIOT_PANE_STOPLIMIT();
	// 模拟市价功能
    static bool IsOrderType_UIOT_CMD_SIMULATEMARKET();
	// 预埋单按钮
    static bool IsOrderType_UIOT_CMD_PARKED();
	// pats改单菜单项
    static bool IsOrderType_UIOT_CMD_REPLACEORDER();
	// 下单板保值单项
    static bool IsOrderType_UIOT_ITEM_HEDGE();
	// 下单板开平项
    static bool IsOrderType_UIOT_ITEM_OPENCLOSE();
	// 下单板帐号项
    static bool IsOrderType_UIOT_ITEM_ACCOUNT();
	// 本地条件单控制项
    static bool IsOrderType_UIOT_ITEM_CONDITION_LOCAL();
	// 服务器条件单控制项
    static bool IsOrderType_UIOT_ITEM_CONDITION_SERVER();
	// 本地预埋单控制项
    static bool IsOrderType_UIOT_ITEM_PARKED_LOCAL();
	// 服务器预埋单控制项
    static bool IsOrderType_UIOT_ITEM_PARKED_SERVER();
	// 本地预埋单控制项
    static bool IsOrderType_UIOT_ITEM_PROFIT_LOCAL();
	// 服务器预埋单控制项
    static bool IsOrderType_UIOT_ITEM_PROFIT_SERVER();

	// 是否保存发送、接收报单及回报的本地时间
    static bool HasSaveOrderTime();

    //是否支持指定产品类型
    enum eProductClassType
    {
        PC_All,         ///全部
        PC_Futures,     ///期权
        PC_Options,     ///期权
        PC_Combination, ///组合
        PC_Spot,        ///即期
        PC_EFP,         ///期转现
    };
    static bool IsSupportProductClassType(eProductClassType ProductClassType);


private:
    //是否已经加载
    static bool m_bLoad;

    //成交回报时是否计算并更新持仓、持仓明细和资金
	static bool m_bCalculate_OnRtnTrade;

    //成交回报后是否触发持仓、持仓明细和资金的查询
    static bool m_bReq_OnRtnTrade;

    //成交回报后延迟查询持仓、持仓明细和资金的时间。单位为秒
    static int m_nDelayBefReq_OnRtnTrade;

    //成交触发查询持仓明细的最大记录数量阀值，超过这个值就不自动查询持仓明细
    static int m_nMaxRecordReqPositionDetail_OnRtnTrade;


    //列表框自动排序时，最大行数。超过这个行数，停止自动排序，以免影响速度
    static int m_nMaxItemCountForAutoResort_ListCtrl;

    //是否启动CifcoServicesHall功能
    static bool m_bCifcoServicesHall_ExtraFunction;

public:
    //是否使用组合单功能
    static bool m_bUseComb;

	//组合单保证金优惠策略, 0-不优惠，1-第一腿优惠，2-第二腿优惠，3-按套利组合中高腿收取，4-大额单边保证金收取(上期)，根据合约信息中的MaxMarginSideAlgorithm
    //注意，选取大边时，按交易所保证金计算
	static int m_DiscountCZCE;
	static int m_DiscountDCE;
	static int m_DiscountSHFE;
	static int m_DiscountCFFEX;

    //郑商所锁仓是否保证金优惠。郑商所锁仓针对普通持仓，相同合约相反方向的持仓，只收单边保证金
    static bool m_LockPositionDiscountCZCE;

    //平仓顺序: 0: 统一先开先平；1: 先单一后组合先开先平
    static int m_UnwindPrioritiesCZCE;
    static int m_UnwindPrioritiesDCE;
    static int m_UnwindPrioritiesSHFE;
    static int m_UnwindPrioritiesCFFEX;
    //计算今仓保证金使用的价格: 0: 按成交价计算(缺省)；1: 按昨结算价计算
    static int m_PriceForMarginOfTodayPositionCZCE;
    static int m_PriceForMarginOfTodayPositionDCE;
    static int m_PriceForMarginOfTodayPositionSHFE;
    static int m_PriceForMarginOfTodayPositionCFFEX;

	//是否根据单腿行情生成组合行情。1生成，0不生成
	static bool m_bCalcuCombQuotOnLegQuot_LastPrice;            //是否自动生成最新价
	static bool m_bCalcuCombQuotOnLegQuot_Volume;               //是否自动生成现量
	static bool m_bCalcuCombQuotOnLegQuot_AskBid;               //是否自动生成买卖价和买卖量
	static bool m_bCalcuCombQuotOnLegQuot_PreSettlementPrice;   //是否自动生成昨结算价
	static bool m_bCalcuCombQuotOnLegQuot_PreClosePrice;        //是否自动生成昨收盘价
	static bool m_bCalcuCombQuotOnLegQuot_LimitPrice;           //是否自动生成涨跌停板

private:
    //对状态为未知的报单(如手工审核单)，是否执行资金冻结
    static bool m_bFreezeOrderUnknownStatus;


    // 标准下单面板
	static bool m_bOrderType_UIOT_PANE_STANDARD;
	// 批量下单面板
    static bool m_bOrderType_UIOT_PANE_BATCH;
	// 条件单下单面板
    static bool m_bOrderType_UIOT_PANE_CONDITION;
	// Fak/fok下单面板
    static bool m_bOrderType_UIOT_PANE_FAKFOK;
	// 盈损单下单面板
    static bool m_bOrderType_UIOT_PANE_PROFIT;
	// 停止单下单面板
    static bool m_bOrderType_UIOT_PANE_STOPLIMIT;
	// 模拟市价功能
    static bool m_bOrderType_UIOT_CMD_SIMULATEMARKET;
	// 预埋单按钮
    static bool m_bOrderType_UIOT_CMD_PARKED;
	// pats改单菜单项
    static bool m_bOrderType_UIOT_CMD_REPLACEORDER;
	// 下单板保值单项
    static bool m_bOrderType_UIOT_ITEM_HEDGE;
	// 下单板开平项
    static bool m_bOrderType_UIOT_ITEM_OPENCLOSE;
	// 下单板帐号项
    static bool m_bOrderType_UIOT_ITEM_ACCOUNT;
	// 本地条件单控制项
    static bool m_bOrderType_UIOT_ITEM_CONDITION_LOCAL;
	// 服务器条件单控制项
    static bool m_bOrderType_UIOT_ITEM_CONDITION_SERVER;
	// 本地预埋单控制项
    static bool m_bOrderType_UIOT_ITEM_PARKED_LOCAL;
	// 服务器预埋单控制项
    static bool m_bOrderType_UIOT_ITEM_PARKED_SERVER;
	// 本地预埋单控制项
    static bool m_bOrderType_UIOT_ITEM_PROFIT_LOCAL;
	// 服务器预埋单控制项
    static bool m_bOrderType_UIOT_ITEM_PROFIT_SERVER;

	// 是否存在发送、接收报单及回报的本地时间
    static bool m_bHasSaveOrderTime;

    ////是否支持产品类型
    static bool m_bSupport_PC_All;
    static bool m_bSupport_PC_Futures;
    static bool m_bSupport_PC_Options;
    static bool m_bSupport_PC_Combination;
    static bool m_bSupport_PC_Spot;
    static bool m_bSupport_PC_EFP;

public:
    //系统订阅行情的最大合约数量，缺省为20
    static int MaxSubscribeCount;
    //插件订阅行情的最大合约数量，MaxSubscribeCountInPlugins<=MaxSubscribeCount，缺省为10
    static int MaxSubscribeCountInPlugins;

    //各交易所对平今指令的支持，0:不支持(但ctp可能会转换成平仓)；1:支持
    static int CloseTodaySupport_SHFE;
    static int CloseTodaySupport_CZCE;
    static int CloseTodaySupport_DCE;
    static int CloseTodaySupport_CFFEX;

    //各交易所对平仓指令的支持，0:不支持；1:支持，可以平昨仓和今仓；2:支持，只能平昨仓
    static int CloseSupport_SHFE;
    static int CloseSupport_CZCE;
    static int CloseSupport_DCE;
    static int CloseSupport_CFFEX;

    //多账号参数
public:
    //是否是多账号网络版
    static bool m_bNetVersion_MultiAccount;
    //是否是多账号单机版
    static bool m_bLocalVersion_MultiAccount;
    //多账号是否乱序下单
    static bool m_bDisorderWhenInsertOrder_MultiAccount;

    //是否允许自动向首账户跟单策略。1允许，0不允许
	static bool m_bAutoDeriveOrderByFirstAccount_MultiAccount;
	//自动向首账户跟单时使用的价格(暂未用)。0买卖价；1最新价；2涨跌停板价；3市价
	static int m_UsePrice_AutoDeriveOrderByFirstAccount_MultiAccount;
	//是否自动拆分并反向开仓
	static bool m_AutoBreak_AutoDeriveOrderByFirstAccount_MultiAccount;

public:
    //是否记录业务包到businessdata.rec中，用于测试回放
    static bool m_Test_bSaveBusinessData;
    //是否进行测试回放，回放businessdata.rec中的业务包
    static bool m_Test_bPlayBusinessData;
    //是否记录线程ID和线程名称到threadid.log文件中
    static bool m_Test_bLogThreadID;

public:
    //运行模式：1: 仅期货；2: 期货+期权；3: 期权及标的物期货。默认为1
    static int m_RunMode;
    //期权：是否支持接收询价通知的功能，默认不支持
    static bool m_CanReceiveForQuote;

public:
    //如果有结算价的话，是否使用结算价计算盈亏和保证金
    static bool m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible;

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


