using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace com.cifco.zqstp.Trader
{
    //public class DealerInfo
    //{
    //    ///经纪公司代码
    //    public string BrokerID = "";
    //    ///投资者帐号
    //    public string AccountID = "";
    //}

    //public class InstrumentInfo
    //{
    //    ///合约代码
    //    public string InstrumentID = "";
    //    ///交易所代码
    //    public string ExchangeID = "";
    //    ///合约名称
    //    public string InstrumentName = "";
    //    ///合约在交易所的代码
    //    public string ExchangeInstID = "";
    //    ///产品代码
    //    public string ProductID = "";
    //    ///产品类型
    //    public char ProductClass = '0';
    //    ///交割年份
    //    public int DeliveryYear = 0;
    //    ///交割月
    //    public int DeliveryMonth = 0;
    //    ///市价单最大下单量
    //    public int MaxMarketOrderVolume = 0;
    //    ///市价单最小下单量
    //    public int MinMarketOrderVolume = 0;
    //    ///限价单最大下单量
    //    public int MaxLimitOrderVolume = 0;
    //    ///限价单最小下单量
    //    public int MinLimitOrderVolume = 0;
    //    ///合约数量乘数
    //    public int VolumeMultiple = 0;
    //    ///最小变动价位
    //    public double PriceTick = 0.0;
    //    ///创建日
    //    public string CreateDate = "";
    //    ///上市日
    //    public string OpenDate = "";
    //    ///到期日
    //    public string ExpireDate = "";
    //    ///开始交割日
    //    public string StartDelivDate = "";
    //    ///结束交割日
    //    public string EndDelivDate = "";
    //    ///合约生命周期状态
    //    public char InstLifePhase = '0';
    //    ///当日是否交易
    //    public bool IsTrading = true;
    //    ///持仓类型
    //    public char PositionType = '0';
    //    ///持仓日期类型
    //    public char PositionDateType = '0';
    //    ///多头保证金率
    //    public double LongMarginRatio = 0.0;
    //    ///空头保证金率
    //    public double ShortMarginRatio = 0.0;
    //    ///是否使用大额单边保证金算法(上期所套利规则：同一客户在同一会员处的同品种双向持仓直接按照保证金金额较大的一边收取交易保证金；对交割月份合约在最后5个交易日不参加单向大边折抵)
    //    public char MaxMarginSideAlgorithm = '0';

    //    ///组合保证金优惠方法：0-不优惠(缺省)，1-第一腿优惠，2-第二腿优惠，3-按套利组合中高腿收取，4-大额单边保证金收取(上期)
    //    public int CombMarginDiscountMode = 0;
    //    ///平仓顺序: 0: 统一先开先平；1: 先单一后组合先开先平
    //    public int UnwindPriorities = 0;
    //    ///计算今仓保证金使用的价格: 0: 按成交价计算(缺省)；1: 按昨结算价计算
    //    public int PriceForMarginOfTodayPosition = 0;
    //    ///对平今指令的支持，0:不支持(但ctp可能会转换成平仓)；1:支持
    //    public int CloseTodayInstructionSupport = 0;
    //    ///对平仓指令的支持，0:不支持；1:支持，可以平昨仓和今仓；2:支持，只能平昨仓
    //    public int CloseInstructionSupport = 0;
    //}

    //public class InstrumentMarginRate
    //{
    //}

    //public class InstrumentCommissionRate
    //{
    //}

    // 投资者资金帐户
    public class TradingAccount
    {
	    ///经纪公司代码
	    public string BrokerID = "";
	    ///投资者帐号
	    public string AccountID = "";
	    ///上次质押金额
	    //public double PreMortgage = 0.0;
	    ///上次信用额度
	    //public double PreCredit = 0.0;
	    ///上次存款额
	    //public double PreDeposit = 0.0;
	    ///上次结算准备金
	    //public double PreBalance = 0.0;
	    ///上次占用的保证金
	    //public double PreMargin = 0.0;
	    ///利息基数
	    //public double InterestBase = 0.0;
	    ///利息收入
	    //public double Interest = 0.0;
	    ///入金金额
	    public double Deposit = 0.0;
	    ///出金金额
	    public double Withdraw = 0.0;
	    ///冻结的保证金
	    public double FrozenMargin = 0.0;
	    ///冻结的资金
	    public double FrozenCash = 0.0;
	    ///冻结的手续费
	    public double FrozenCommission = 0.0;
	    ///当前保证金总额
	    public double CurrMargin = 0.0;
	    ///资金差额
	    public double CashIn = 0.0;
	    ///手续费
	    public double Commission = 0.0;
	    ///平仓盈亏
	    public double CloseProfit = 0.0;
	    ///持仓盈亏
	    public double PositionProfit = 0.0;
	    ///期货结算准备金
	    public double Balance = 0.0;
	    ///可用资金
	    public double Available = 0.0;
	    ///可取资金
	    public double WithdrawQuota = 0.0;
	    ///基本准备金
	    public double Reserve = 0.0;
	    ///交易日
	    public string TradingDay = "";
	    ///结算编号
	    //public int SettlementID = 0;
	    ///信用额度
	    public double Credit = 0.0;
	    ///质押金额
	    public double Mortgage = 0.0;
	    ///交易所保证金
	    public double ExchangeMargin = 0.0;
	    ///投资者交割保证金
	    public double DeliveryMargin = 0.0;
	    ///交易所交割保证金
	    public double ExchangeDeliveryMargin = 0.0;
	    ///静态权益
	    public double StaticProfit = 0.0;
	    ///动态权益
	    public double DynamicProfit = 0.0;
	    ///风险度
	    public double RiskDegree = 0.0;
    }

    // 报单信息
    public class OrderInfo
    {
	    ///经纪公司代码
	    public string BrokerID = "";
	    ///投资者代码
	    public string InvestorID = "";
	    ///合约代码
	    public string InstrumentID = "";
	    ///报单引用
	    public string OrderRef = "";
	    ///用户代码
	    //public string UserID = "";
	    ///报单价格条件
	    public char OrderPriceType = '0';
	    ///买卖方向
	    public char Direction = '0';
	    ///组合开平标志
	    public string CombOffsetFlag = "";
	    ///组合投机套保标志
	    public string CombHedgeFlag = "";
	    ///价格
	    public double LimitPrice = 0.0;
	    ///数量
	    public int VolumeTotalOriginal = 0;
	    ///有效期类型
	    //public char TimeCondition = '0';
	    ///GTD日期
	    //public string GTDDate = "";
	    ///成交量类型
	    public char VolumeCondition = '0';
	    ///最小成交量
	    public int MinVolume = 0;
	    ///触发条件
	    //public char ContingentCondition = '0';
	    ///止损价
	    //public double StopPrice = 0.0;
	    ///强平原因
	    //public int ForceCloseReason = 0;
	    ///自动挂起标志
	    //public bool IsAutoSuspend = false;
	    ///业务单元
	    //public string BusinessUnit = "";
	    ///请求编号
	    public int RequestID = 0;
	    ///本地报单编号
	    public string OrderLocalID = "";
	    ///交易所代码
	    public string ExchangeID = "";
	    ///会员代码
	    //public string ParticipantID = "";
	    ///客户代码
	    //public string ClientID = "";
	    ///合约在交易所的代码
	    //public string ExchangeInstID = "";
	    ///交易所交易员代码
	    //public string TraderID = "";
	    ///安装编号
	    //public int InstallID = 0;
	    ///报单提交状态
	    public char OrderSubmitStatus = '0';
	    ///报单提示序号
	    public int NotifySequence = 0;
	    ///交易日
	    public string TradingDay = "";
	    ///结算编号
	    //public int SettlementID = 0;
	    ///报单编号
	    public string OrderSysID = "";
	    ///报单来源
	    //public char OrderSource = '0';
	    ///报单状态
	    public char OrderStatus = 'a';
	    ///报单类型
	    //public char OrderType = '0';
	    ///今成交数量
	    public int VolumeTraded = 0;
	    ///剩余数量
	    public int VolumeTotal = 0;
	    ///报单日期
	    public string InsertDate = "";
	    ///委托时间
	    public string InsertTime = "";
	    ///激活时间
	    //public string ActiveTime = "";
	    ///挂起时间
	    //public string SuspendTime = "";
	    ///最后修改时间
	    public string UpdateTime = "";
	    ///撤销时间
	    public string CancelTime = "";
	    ///最后修改交易所交易员代码
	    //public string ActiveTraderID = "";
	    ///结算会员编号
	    //public string ClearingPartID = "";
	    ///序号
	    public int SequenceNo = 0;
	    ///前置编号
	    public int FrontID = 0;
	    ///会话编号
	    public int SessionID = 0;
	    ///用户端产品信息
	    //public string UserProductInfo = "";
	    ///状态信息
	    public string StatusMsg = "";
	    ///用户强评标志
	    //public bool UserForceClose = false;
	    ///操作用户代码
	    //public string ActiveUserID = "";
	    ///经纪公司报单编号
	    public int BrokerOrderSeq = 0;
	    ///相关报单
	    public string RelativeOrderSysID = "";
	    //成交均价
	    //public double AvgPrice = 0.0;

	    //冻结保证金
        //public double FreezeMargin = 0.0;
        //交易所冻结保证金，用于计算套利保证金优惠
	    //public double FreezeExchMargin = 0.0;
        //冻结手续费
	    //public double TroubleMoney = 0.0;

        //组合合约报单的第一腿冻结保证金
	    //public double Leg1FreezeMargin = 0.0;
        //组合合约报单的第一腿冻结交易所保证金
	    //public double Leg1FreezeExchMargin = 0.0;
        //组合合约报单的第一腿冻结手续费
	    //public double Leg1TroubleMoney = 0.0;
        //组合合约报单的第二腿冻结保证金
	    //public double Leg2FreezeMargin = 0.0;
        //组合合约报单的第二腿冻结交易所保证金
	    //public double Leg2FreezeExchMargin = 0.0;
        //组合合约报单的第二腿冻结手续费
	    //public double Leg2TroubleMoney = 0.0;
    }

    // 成交信息
    public class TradeInfo
    {
	    ///经纪公司代码
	    public string BrokerID;
	    ///投资者代码
	    public string InvestorID;
	    ///合约代码
	    public string InstrumentID;
	    ///报单引用
	    public string OrderRef;
	    ///用户代码
	    //public string UserID;
	    ///交易所代码
	    public string ExchangeID;
	    ///成交编号
	    public string TradeID;
	    ///买卖方向
	    public char Direction;
	    ///报单编号
	    public string OrderSysID;
	    ///会员代码
	    //public string ParticipantID;
	    ///客户代码
	    //public string ClientID;
	    ///交易角色
	    //public char TradingRole;
	    ///合约在交易所的代码
	    //public string ExchangeInstID;
	    ///开平标志
	    public char OffsetFlag;
	    ///投机套保标志
	    public char HedgeFlag;
	    ///价格
	    public double Price;
	    ///数量
	    public int Volume;
	    ///成交时期
	    public string TradeDate;
	    ///成交时间
	    public string TradeTime;
	    ///成交类型
	    public char TradeType;
	    ///成交价来源
	    //public char PriceSource;
	    ///交易所交易员代码
	    //public string TraderID;
	    ///本地报单编号
	    public string OrderLocalID;
	    ///结算会员编号
	    //public string ClearingPartID;
	    ///业务单元
	    //public string BusinessUnit;
	    ///序号
	    public int SequenceNo;
	    ///交易日
	    public string TradingDay;
	    ///结算编号
	    //public int SettlementID;
	    ///经纪公司报单编号
	    public int BrokerOrderSeq;
	    ///成交来源
	    //public char TradeSource;


        //平今量。平仓成交有效。需要根据持仓明细来计算。成交回报、查询成交结束、查询持仓明细结束都会(在持仓明细中)尝试计算平今量，上述情况和查询费率结束会计算手续费，
        //public int CloseTodayVolume;
	    //手续费。考虑平今优惠的问题，需要明确平今量后才能计算手续费
	    //public double TradeCommission;

        ///逐日盯市平仓盈亏。对于查询得到的成交记录，平仓盈亏为无效值。对于成交回报得到的成交记录，平仓盈亏在更新持仓明细后得到。用于机构版服务器端结算
	    //public double CloseProfitByDate;
	    ///逐笔对冲平仓盈亏。注意事项同上
	    //public double CloseProfitByTrade;
    }

    // 投资者持仓
    public class PositionInfo
    {
	    ///合约代码
	    public string InstrumentID = "";
	    ///经纪公司代码
	    public string BrokerID = "";
	    ///投资者代码
        public string InvestorID = "";
	    ///持仓多空方向
	    public char PosiDirection = '0';
	    ///投机套保标志
	    public char HedgeFlag = '0';
	    ///持仓日期类型
	    //public char PositionDate;// = '0';
	    ///交易日
        public string TradingDay = "";
	    ///结算编号
	    //public int SettlementID;// = 0;

	    ///当前净持仓
	    public int Position = 0;
	    ///今日持仓(指今日净持仓)
	    public int TodayPosition = 0;
	    ///上日持仓(指历史持仓总和，包括今日平掉的部分)
	    public int YdPosition = 0;

	    ///开仓量(指今日开仓量)
	    public int OpenVolume = 0;
	    ///平仓量(指今日平仓量)
	    public int CloseVolume = 0;
	    ///开仓金额(指今开仓金额)
	    public double OpenAmount = 0.0;
	    ///平仓金额(指今平仓金额)
	    public double CloseAmount = 0.0;
	    ///持仓成本(当前净持仓的持仓成本)
	    public double PositionCost = 0.0;
	    ///开仓成本(未平持仓明细的开仓金额的总和(只计算未平部分))
	    public double OpenCost = 0.0;

	    ///多头冻结(多头持仓: 开买冻结；空头持仓：平买冻结)
	    public int LongFrozen = 0;
	    ///空头冻结(多头持仓: 平卖冻结；空头持仓：开卖冻结)
	    public int ShortFrozen = 0;
	    ///多头冻结金额
	    public double LongFrozenAmount = 0.0;
	    ///空头冻结金额
	    public double ShortFrozenAmount = 0.0;
	    ///冻结的保证金
	    public double FrozenMargin = 0.0;
	    ///冻结的手续费
	    public double FrozenCommission = 0.0;
	    ///冻结的资金
	    public double FrozenCash = 0.0;

	    ///手续费
	    public double Commission = 0.0;
	    ///上次占用的保证金
	    //public double PreMargin;// = 0.0;
	    ///占用的保证金
	    public double UseMargin = 0.0;
	    ///交易所保证金
	    public double ExchangeMargin = 0.0;
	    ///保证金率
	    public double MarginRateByMoney = 0.0;
	    ///保证金率(按手数)
	    public double MarginRateByVolume = 0.0;

	    ///资金差额
	    //public double CashIn;// = 0.0;

	    ///持仓盈亏(盯市)
	    public double PositionProfit = 0.0;
	    ///平仓盈亏(盯市)
	    public double CloseProfit = 0.0;
	    ///逐日盯市平仓盈亏
	    public double CloseProfitByDate = 0.0;
	    ///逐笔对冲平仓盈亏
	    public double CloseProfitByTrade = 0.0;

	    ///上次结算价
	    public double PreSettlementPrice = 0.0;
	    ///本次结算价
	    public double SettlementPrice = 0.0;

	    ///组合成交形成的持仓
	    public int CombPosition = 0;
	    ///组合多头冻结
	    public int CombLongFrozen = 0;
	    ///组合空头冻结
	    public int CombShortFrozen = 0;

        //交易所代码
	    public string ExchangeID = "";
        //浮动盈亏，所有持仓明细浮盈之和，就是逐笔对冲的持仓盈亏
	    public double PositionProfitByTrade = 0.0;
        //盯市总盈亏，盯市平仓盈亏和持仓盈亏之和
	    public double TotalPositionProfitByDate = 0.0;

        //总平仓挂单量
	    //public int TotalCloseOrderVolume;// = 0;
        //平仓挂单量，指平仓单，区别于平今和平昨
	    //public int CloseOrderVolume;// = 0;
        //平仓挂单量的今仓部分
        //public int CloseOrderVolumeTdPart;// = 0;
        //平仓挂单量的昨仓部分
        //public int CloseOrderVolumeYdPart;// = 0;
        //平今挂单量
	    //public int CloseTodayOrderVolume;// = 0;
        //平昨挂单量
	    //public int CloseYesterdayOrderVolume;// = 0;
        //可平量Position-TotalCloseOrderVolume
	    public int CanCloseVolume = 0;
        //可平今量TodayPosition-CloseTodayOrderVolume
	    public int CanCloseTodayVolume = 0;
        //可平昨量YdPosition-CloseYesterdayOrderVolume
	    public int CanCloseydVolume = 0;

        //持仓均价(持仓成本/持仓量)
	    public double AveragePositionPrice = 0.0;
        //开仓均价((今日开仓成本+历史开仓成本)/总开仓量)
	    public double AverageOpenPrice = 0.0;

        //今平历史仓量
	    public int TodayCloseYdVolume = 0;
        //上日剩余仓位，从持仓明细计算
	    public int YdPositionRemain = 0;

    }

    // 投资者持仓明细
    public class PositionDetailInfo
    {
        ///合约代码
        public string InstrumentID = "";
        ///经纪公司代码
        public string BrokerID = "";
        ///投资者代码
        public string InvestorID = "";
        ///投机套保标志
        public char HedgeFlag = '0';
        ///买卖
        public char Direction = '0';
        ///开仓日期
        public string OpenDate = "";
        ///成交编号
        public string TradeID = "";
        ///数量 外盘净仓位
        public int Volume = 0;
        ///开仓价
        public double OpenPrice = 0.0;
        ///交易日
        public string TradingDay = "";
        /////结算编号
        //public int SettlementID = 0;
        ///成交类型
        public char TradeType = '0';
        ///组合合约代码
        public string CombInstrumentID = "";
        ///交易所代码
        public string ExchangeID = "";
        ///逐日盯市平仓盈亏
        public double CloseProfitByDate = 0.0;
        ///逐笔对冲平仓盈亏
        public double CloseProfitByTrade = 0.0;
        ///逐日盯市持仓盈亏
        public double PositionProfitByDate = 0.0;
        ///逐笔对冲持仓盈亏
        public double PositionProfitByTrade = 0.0;
        ///投资者保证金
        public double Margin = 0.0;
        /////交易所保证金
        //public double ExchMargin = 0.0;
        /////保证金率
        //public double MarginRateByMoney = 0.0;
        /////保证金率(按手数)
        //public double MarginRateByVolume = 0.0;
        ///昨结算价
        public double LastSettlementPrice = 0.0;
        ///结算价
        public double SettlementPrice = 0.0;
        /////平仓量
        //public int CloseVolume = 0;
        /////平仓金额
        //public double CloseAmount = 0.0;

        ////是否今日持仓明细
        //public bool IsToday;
        ////开仓时间。昨仓为00:00:00，今仓为对应的开仓成交时间
        //public string OpenTime;
        ////开仓时间，utc秒数。昨仓为开仓日零时的时间，今仓为正确时间
        //public int OpenTimeUTC;
    }

    //public class PositionDetailCombInfo
    //{
    //    ///交易日
    //    public string TradingDay = "";
    //    ///开仓日期
    //    public string OpenDate = "";
    //    ///交易所代码
    //    public string ExchangeID = "";
    //    ///结算编号
    //    public int SettlementID = 0;
    //    ///经纪公司代码
    //    public string BrokerID = "";
    //    ///投资者代码
    //    public string InvestorID = "";
    //    ///组合编号
    //    public string ComTradeID = "";
    //    ///撮合编号
    //    public string TradeID = "";
    //    ///合约代码
    //    public string InstrumentID = "";
    //    ///投机套保标志
    //    public char HedgeFlag = '0';
    //    ///买卖
    //    public char Direction = '0';
    //    ///持仓量
    //    public int TotalAmt = 0;
    //    ///投资者保证金
    //    public double Margin = 0.0;
    //    ///交易所保证金
    //    public double ExchMargin = 0.0;
    //    ///保证金率
    //    public double MarginRateByMoney = 0.0;
    //    ///保证金率(按手数)
    //    public double MarginRateByVolume = 0.0;
    //    ///单腿编号(整型值，0/1)
    //    public int LegID = 0;
    //    ///单腿乘数(1)
    //    public int LegMultiple = 0;
    //    ///组合持仓合约编码
    //    public string CombInstrumentID = "";
    //}

    //public class OrderInsert
    //{
    //}

    public class OrderAction
    {
        ///经纪公司代码
        public string BrokerID = "";
        ///投资者代码
        public string InvestorID = "";
        ///报单操作引用
        public int OrderActionRef = 0;
        ///报单引用
        public string OrderRef = "";
        ///请求编号
        public int RequestID = 0;
        ///前置编号
        public int FrontID = 0;
        ///会话编号
        public int SessionID = 0;
        ///交易所代码
        public string ExchangeID = "";
        ///报单编号
        public string OrderSysID = "";
        ///操作标志
        public char ActionFlag = '0';
        ///价格
        public double LimitPrice = 0.0;
        ///数量变化
        public int VolumeChange = 0;
        ///用户代码
        public string UserID = "";
        ///合约代码
        public string InstrumentID = "";
        ///预埋撤单单编号
        public string ParkedOrderActionID = "";
        ///用户类型
        public char UserType = '0';
        ///预埋撤单状态
        public char Status = '0';
        ///错误代码
        public int ErrorID = 0;
        ///错误信息
        public string ErrorMsg = "";
    }

    public class OrderInput
    {
        ///经纪公司代码
        public string brokerID = "";
        ///投资者代码
        public string InvestorID = "";
        ///合约代码
        public string InstrumentID = "";
        ///报单引用
        public string OrderRef = "";
        ///用户代码
        public string UserID = "";
        ///报单价格条件
        public char OrderPriceType = '1';
        ///买卖方向
        public char Direction = '0';
        ///组合开平标志
        public string CombOffsetFlag = "";
        ///组合投机套保标志
        public string CombHedgeFlag = "";
        ///价格
        public double LimitPrice = 0.0;
        ///数量
        public int VolumeTotalOriginal = 0;
        ///有效期类型
        public char TimeCondition = '0';
        ///GTD日期
        public string GTDDate = "";
        ///成交量类型
        public char VolumeCondition = '0';
        ///最小成交量
        public int MinVolume = 1;
        ///触发条件
        public char ContingentCondition = '0';
        ///止损价
        public double StopPrice = 0.0;
        ///强平原因
        public char ForceCloseReason = '0';
        ///自动挂起标志
        public bool IsAutoSuspend = false;
        ///业务单元
        public string BusinessUnit = "";
        ///请求编号
        public int RequestID = 0;
        ///用户强评标志
        public bool UserForceClose = false;
    }

    public class ActionInput
    {
        ///经纪公司代码
        public string BrokerID = "";
        ///投资者代码
        public string InvestorID = "";
        ///报单操作引用
        public int OrderActionRef = 0;
        ///报单引用
        public string OrderRef = "";
        ///请求编号
        public int RequestID = 0;
        ///前置编号
        public int FrontID = 0;
        ///会话编号
        public int SessionID = 0;
        ///交易所代码
        public string ExchangeID = "";
        ///报单编号
        public string OrderSysID = "";
        ///操作标志
        public char ActionFlag = '0';
        ///价格
        public double LimitPrice = 0.0;
        ///数量变化
        public int VolumeChange = 0;
        ///用户代码
        public string UserID = "";
        ///合约代码
        public string InstrumentID = "";
    }

    // 请求下单结构
    public class InputOrder
    {
        public string DealerID = "";
        public string BrokerID = "";
        public string InstrumentID = "";
        public char Direction = '1';
        public char OpenClose = '1';
        public char Hedge = '1';
        public double Price = 0.0;
        public bool IsMarket = false;
        public int Qty = 0;
    }
        
    // 请求撤单结构
    public class InputAction
    {
        public string DealerID = "";
        public string BrokerID = "";
        public string OrderRef = "";
        public int Session = 0;
        public int FrontID = 0;
        public string OrderSysID = "";
        public string ExchangeID = "";
        public string InstrumentID = "";
    }
        
    // 查询交易员资金帐号结构
    public class QryTradingAccount
    {
        public string strDealerID = "";
        public string strBrokerID = "";
    }
        
    // 查询交易员持仓结构
    public class QryPosition
    {
        public string DealerID = "";
        public string BrokerID = "";
        public string InstrumentID = "";
        public char PosiDirection = '-';    // '-'表示查询时查询所有的方向
    }

    // 查询交易员持仓明细结构
    public class QryPositionDetail
    {
        public string DealerID = "";
        public string BrokerID = "";
        public string InstrumentID = "";
        public char Direction = '-';        // - 双向，0 买入，1 卖出
    }

    //// 请求查询交易员组合持仓明细
    //public class QryPositionDetailComb
    //{
    //    public string DealerID = "";
    //    public string BrokerID = "";
    //    public string InstrumentID = "";
    //    public char Direction = '-';        // '-'表示查询时查询所有的方向
    //}

    // 请求查询报单
    public class QryOrder
    {
        public string strDealerID = "";
        public string strBrokerID = "";
        public string InstrumentID = "";
    }

    // 请求查询成交
    public class QryTrade
    {
        public string strDealerID = "";
        public string strBrokerID = "";
        public string InstrumentID = "";
    }

}
