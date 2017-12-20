#include "StdAfx.h"
#include "CmeCoreApi.h"
#include <string>
#define GET_EXCHANGEID(ExchID,InstID)\
do{\
    map<string,EPCID>::const_iterator iter=m_ContractName2ID.find(InstID);\
    if(iter!=m_ContractName2ID.end())\
    {\
        map<int,Stru_Base_ExchangeInfo>::const_iterator exch_iter=m_ExchangeInfos.find(iter->second.ExchangeID);\
        if(exch_iter!=m_ExchangeInfos.end())\
        {\
            strcpy_s(ExchID,sizeof(ExchID)-1,exch_iter->second.ExchangeCode);\
        }\
     }\
}while(0)
#define STRUCT_SWAP_BEGIN(src)   switch(src){
#define STRUCT_SWAP_END()        default:break;}
#define STRUCT_SWAP_ITEM(dest,srcV,destV)  case srcV:dest=destV;break;
static uint32 DataConvert(const char* pStr)
{
    return 0;
}
static uint32 TimeConvert(const char* pStr)
{
    return 0;
}
//填充PlatformStru_InstrumentInfo结构体
void CCmeCoreApi::FillInstrumentInfo(const int ExchangeID,const Stru_Base_ContractInfo& ContractInfo,PlatformStru_InstrumentInfo& Instrument)
{
    memset(&Instrument,0,sizeof(Instrument));
    Stru_Base_ProductInfo&  ProductInfo=m_ProductInfos[ExchangeID][ContractInfo.ProductID];
    Stru_Base_ExchangeInfo& ExchangeInfo=m_ExchangeInfos[ExchangeID];
    //
    MakeGUIContractID(ExchangeInfo,ProductInfo,ContractInfo,Instrument.InstrumentID);
    strcpy_s(Instrument.ExchangeID,sizeof(Instrument.ExchangeID)-1,ExchangeInfo.ExchangeCode);
    strcpy_s(Instrument.InstrumentName,sizeof(Instrument.InstrumentName)-1,Instrument.InstrumentID);
    strcpy_s(Instrument.ExchangeInstID,sizeof(Instrument.ExchangeInstID)-1,Instrument.InstrumentID);
    strcpy_s(Instrument.ProductID,sizeof(Instrument.ProductID)-1,ProductInfo.ProductName);
    Instrument.ProductClass=THOST_FTDC_PC_Futures;
    struct tm* UTCtime=localtime((time_t *)&ContractInfo.ExpiryDate);
    Instrument.DeliveryYear=UTCtime->tm_year+1900;
    Instrument.DeliveryMonth=UTCtime->tm_mon;

    Instrument.VolumeMultiple=ProductInfo.OnePoint;
    Instrument.PriceTick=ProductInfo.TickSize;
    map<int,Stru_Base_CurrencyInfo>::iterator curr_it=m_CurrencyInfos.find(ProductInfo.CurrencyID);
    if(curr_it!=m_CurrencyInfos.end())
        sprintf_s(Instrument.Currency,sizeof(Instrument.Currency)-1,curr_it->second.CurrencyName);
    Instrument.TicksPerPoint=ProductInfo.TicksPerPoint;
	sprintf_s(Instrument.TickSize,sizeof(Instrument.TickSize)-1,"%f",ProductInfo.TickSize);
}
void CCmeCoreApi::FillDepthMarketData(const Stru_Base_Market_SnapshotData& md,CThostFtdcDepthMarketDataField& field)
{
//    ///交易日
//    {
//        struct tm* UTCtime=localtime((time_t *)&md.TradingDay);
//        sprintf_s(field.TradingDay,sizeof(field.TradingDay)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
//    }
//    ///合约代码,合约在交易所的代码
//    {
//        map<int,string>::const_iterator iter=m_ID2ContractName.find(md.ContractID);
//        if(iter!=m_ID2ContractName.end())
//        {
//            strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,iter->second.c_str());
//            strcpy_s(field.ExchangeInstID,sizeof(field.ExchangeInstID)-1,iter->second.c_str());
//        }
//    }
//    ///交易所代码
//    {
//        map<string,EPCID>::const_iterator iter=m_ContractName2ID.find(field.InstrumentID);
//        if(iter!=m_ContractName2ID.end())
//        {
//            map<int,Stru_Base_ExchangeInfo>::const_iterator exch_iter=m_ExchangeInfos.find(iter->second.ExchangeID);
//            if(exch_iter!=m_ExchangeInfos.end())
//            {
//                strcpy_s(field.ExchangeID,sizeof(field.ExchangeID)-1,exch_iter->second.ExchangeCode);
//            }
//        }
//    }
//	///最新价
//    field.LastPrice=md.LastPrice;
//	///上次结算价
//    field.PreSettlementPrice=md.PreSettlementPrice;
//	///昨收盘
//    field.PreClosePrice=md.PreClosePrice;
// 	///昨持仓量
//    field.PreOpenInterest=md.PreOpenInterest;
//	///今开盘
//    field.OpenPrice=md.OpenPrice;
//	///最高价
//    field.HighestPrice=md.HighestPrice;
//	///最低价
//    field.LowestPrice=md.LowestPrice;   
//	///数量
//    field.Volume=md.Volume;
//	///成交金额
//    field.Turnover=md.Trunover;
//	///持仓量
//    field.OpenInterest=md.OpenInterest;
//	///今收盘
//    field.ClosePrice=md.ClosePrice;
//	///本次结算价
//    field.SettlementPrice=md.SettlementPrice;
//	///涨停板价
//    field.UpperLimitPrice=md.UpperLimitPrice;
//	///跌停板价
//    field.LowerLimitPrice=md.LowerLimitPrice;
//	///昨虚实度
//	field.PreDelta=0;
//	///今虚实度
//	field.CurrDelta=0;
//	///最后修改时间
//    {
//        struct tm* UTCtime=localtime((time_t *)&md.UpdateTime);
//        sprintf_s(field.UpdateTime,sizeof(field.UpdateTime)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
//    }
//	///最后修改毫秒
//	field.UpdateMillisec=GetTickCount()%1000;
//#define BIDASK_N(index)\
//do{\
//    field.BidPrice##index=md.BidPrice[index-1];\
//    field.BidVolume##index=md.BidVolume[index-1];\
//    field.AskPrice##index=md.AskPrice[index-1];\
//    field.AskVolume##index=md.AskVolume[index-1];\
//}while(0)
//    BIDASK_N(1);
//    BIDASK_N(2);
//    BIDASK_N(3);
//    BIDASK_N(4);
//    BIDASK_N(5);
}
void CCmeCoreApi::FillDepthMarketData(const Stru_Client_NewMarketData& md,PlatformStru_DepthMarketData& field)
{
}
void CCmeCoreApi::FillOrderInfo(const Stru_Base_OrderInfo& Info,PlatformStru_OrderInfo& Order)
{
    ///经纪公司代码
	Order.BrokerID;
	///投资者代码
	sprintf_s(Order.InvestorID,sizeof(Order.InvestorID)-1,"%d",Info.AccountID);
	///合约代码
    {
        map<int,string>::const_iterator iter=m_ID2ContractName.find(Info.ContractID);
        if(iter!=m_ID2ContractName.end())
        {
            strcpy_s(Order.InstrumentID,sizeof(Order.InstrumentID)-1,iter->second.c_str());
            strcpy_s(Order.ExchangeInstID,sizeof(Order.ExchangeInstID)-1,iter->second.c_str());
        }
    }
	///报单引用
	Order.OrderRef;
	///用户代码
	Order.UserID;
	///报单价格条件
	Order.OrderPriceType;
	///买卖方向
	Order.Direction;
	///组合开平标志
	Order.CombOffsetFlag;
	///组合投机套保标志
	Order.CombHedgeFlag;
	///价格
	Order.LimitPrice;
	///数量
	Order.VolumeTotalOriginal;
	///有效期类型
	Order.TimeCondition;
	///GTD日期
	Order.GTDDate;
	///成交量类型
	Order.VolumeCondition;
	///最小成交量
	Order.MinVolume;
	///触发条件
	Order.ContingentCondition;
	///止损价
	Order.StopPrice;
	///强平原因
	Order.ForceCloseReason;
	///自动挂起标志
	Order.IsAutoSuspend;
	///业务单元
	Order.BusinessUnit;
	///请求编号
	Order.RequestID;
	///本地报单编号
	Order.OrderLocalID;
	///交易所代码
	Order.ExchangeID;
	///会员代码
	Order.ParticipantID;
	///客户代码
	Order.ClientID;
	///交易所交易员代码
	Order.TraderID;
	///安装编号
	Order.InstallID;
	///报单提交状态
	Order.OrderSubmitStatus;
	///报单提示序号
	Order.NotifySequence;
	///交易日
	Order.TradingDay;
	///结算编号
	Order.SettlementID;
	///报单编号
	Order.OrderSysID;
	///报单来源
	Order.OrderSource;
	///报单状态
	Order.OrderStatus;
	///报单类型
	Order.OrderType;
	///今成交数量
	Order.VolumeTraded;
	///剩余数量
	Order.VolumeTotal;
	///报单日期
	Order.InsertDate;
	///委托时间
	Order.InsertTime;
	///激活时间
	Order.ActiveTime;
	///挂起时间
	Order.SuspendTime;
	///最后修改时间
	Order.UpdateTime;
	///撤销时间
	Order.CancelTime;
	///最后修改交易所交易员代码
	Order.ActiveTraderID;
	///结算会员编号
	Order.ClearingPartID;
	///序号
	Order.SequenceNo;
	///前置编号
	Order.FrontID;
	///会话编号
	Order.SessionID;
	///用户端产品信息
	Order.UserProductInfo;
	///状态信息
	Order.StatusMsg;
	///用户强评标志
	Order.UserForceClose;
	///操作用户代码
	Order.ActiveUserID;
	///经纪公司报单编号
	Order.BrokerOrderSeq;
	///相关报单
	Order.RelativeOrderSysID;
    //成交均价
	Order.AvgPrice;
	//价格2
	Order.Price2;  
}
//查询成交返回
void CCmeCoreApi::FillTradeInfo(const Stru_Base_TradeInfo& src,PlatformStru_TradeInfo& info)
{
}
//查询持仓返回
void CCmeCoreApi::FillPositionInfo(const Stru_Base_PositionInfo& src,PlatformStru_Position& pos)
{
}
//查询资金返回
void CCmeCoreApi::FillTradingAccount(const Stru_Base_EquityInfo& src,CThostFtdcTradingAccountField& field)
{

}
//查询出入金返回
void CCmeCoreApi::FillTradingAccount(const Stru_Client_DepositWithdrawDynaUpdate& src,CThostFtdcTradingAccountField& field)
{
    switch(src.UpdateFlag)
    {
    case UpdateFlag_new: 
    case UpdateFlag_modify:
        FillTradingAccount(src.Info,field);
        break;
    case UpdateFlag_remove:
        memset(&field,0,sizeof(field));
        break;
    }
}
//查询出入金返回
void CCmeCoreApi::FillTradingAccount(const Stru_Base_DepositWithdrawDyna& src,CThostFtdcTradingAccountField& field)
{
    sprintf_s(field.AccountID,sizeof(field.AccountID)-1,"%d",src.AccountID);
    field.Deposit=src.Deposit;;
    field.Withdraw=src.Withdraw;
    struct tm* UTCtime=localtime((time_t *)&src.TradingDay);
    sprintf_s(field.TradingDay,sizeof(field.TradingDay)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
}
//发出报单插入
void CCmeCoreApi::FillOrderCmdInfo(const PlatformStru_InputOrder& src,Stru_Base_OrderCmdInfo& field)
{
    field.OrderCommandID=atoi(src.OrderRef);
    field.OrderCmdFlag=OrderCmdFlag_New;
    field.OrderID=atoi(src.OrderRef);
    field.AccountID=atoi(src.InvestorID);
    map<string,EPCID>::const_iterator thisEPCID=m_ContractName2ID.find(src.InstrumentID);
    if(thisEPCID!=m_ContractName2ID.end())
        field.ContractID=thisEPCID->second.ContractID;
    if(src.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation)field.HedgeFlag=HedgeFlag_Speculation;
    else if(src.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge)  field.HedgeFlag=HedgeFlag_Hedge;
    if(src.Direction==THOST_FTDC_D_Buy)  field.DirectionFlag=DirectionFlag_Long;
    else if(src.Direction==THOST_FTDC_D_Sell)  field.DirectionFlag=DirectionFlag_Short;
    STRUCT_SWAP_BEGIN(src.OrderPriceType)
        STRUCT_SWAP_ITEM(field.OrderType,THOST_FTDC_OPT_LimitPrice,OrderType_Limit)
        STRUCT_SWAP_ITEM(field.OrderType,THOST_FTDC_OPT_AnyPrice,OrderType_Market)
    STRUCT_SWAP_END()
    field.LimitPrice=src.LimitPrice;
    STRUCT_SWAP_BEGIN(src.ForceCloseReason)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,THOST_FTDC_FCC_NotForceClose,EnumForceCloseReason_NotForceClose)          
        STRUCT_SWAP_ITEM(field.ForceCloseReason, THOST_FTDC_FCC_LackDeposit,EnumForceCloseReason_LackDeposit)         
        STRUCT_SWAP_ITEM(field.ForceCloseReason, THOST_FTDC_FCC_ClientOverPositionLimit,EnumForceCloseReason_ClientOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,THOST_FTDC_FCC_MemberOverPositionLimit,EnumForceCloseReason_MemberOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,  THOST_FTDC_FCC_NotMultiple,EnumForceCloseReason_NotMultiple)    
        STRUCT_SWAP_ITEM(field.ForceCloseReason,   THOST_FTDC_FCC_Violation,EnumForceCloseReason_Violation)                      
        STRUCT_SWAP_ITEM(field.ForceCloseReason,    THOST_FTDC_FCC_Other,EnumForceCloseReason_Other)
    STRUCT_SWAP_END()
    if(src.ForceCloseReason==THOST_FTDC_FCC_NotForceClose) field.ForceCloseFlag=ForceCloseFlag_Not;
    else field.ForceCloseFlag=ForceCloseFlag_CGT;
    if(src.ContingentCondition==THOST_FTDC_CC_Immediately) field.TriggerFlag=TriggerFlag_Immediately;
    else if(src.ContingentCondition==THOST_FTDC_CC_ParkedOrder)field.TriggerFlag=TriggerFlag_Time;

    switch(src.TimeCondition){
    case THOST_FTDC_TC_IOC:field.ExpiryFlag=OrderExpiryFlag_Common;break;
    case THOST_FTDC_TC_GTD:field.ExpiryFlag=OrderExpiryFlag_GTD;break;
    case THOST_FTDC_TC_GTC:field.ExpiryFlag=OrderExpiryFlag_GTC;break;
    default:break;
    }
    field.TriggerPriceForStop=src.StopPrice;
    field.TradingDay=DataConvert(src.GTDDate);
    field.VolumeMin=src.MinVolume;
    field.Volume=src.VolumeTotalOriginal;
    field.GTDTime=TimeConvert(src.GTDDate);
}
//发出报单行为
void CCmeCoreApi::FillOrderCmdInfo(const CThostFtdcInputOrderActionField& src,Stru_Base_OrderCmdInfo& field)
{
    field.OrderCommandID=atoi(src.OrderRef);
    if(src.ActionFlag==THOST_FTDC_AF_Delete) field.OrderCmdFlag=OrderCmdFlag_Cancel;
    else if(src.ActionFlag==THOST_FTDC_AF_Modify) field.OrderCmdFlag=OrderCmdFlag_Amend;
    field.OrderID=atoi(src.OrderRef);
    field.AccountID=atoi(src.InvestorID);
    field.LoginSessionID=src.SessionID;
    map<string,EPCID>::const_iterator thisEPCID=m_ContractName2ID.find(src.InstrumentID);
    if(thisEPCID!=m_ContractName2ID.end())
        field.ContractID=thisEPCID->second.ContractID;
    field.TriggerFlag=TriggerFlag_Immediately;
    field.Volume=src.VolumeChange;
}
//发出预埋单插入
void CCmeCoreApi::FillOrderCmdInfo(const CThostFtdcParkedOrderField& src,Stru_Base_OrderCmdInfo& field)
{
    //暂不实现
}
//发出预埋单行为
void CCmeCoreApi::FillOrderCmdInfo(const CThostFtdcParkedOrderActionField& src,Stru_Base_OrderCmdInfo& field)
{
    //暂不实现
}
//报单插入返回
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderField& field)
{
    //暂不实现
}
//报单行为返回
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderActionField& field)
{
    string strInstrument=m_ID2ContractName[src.ContractID];
    ///经纪公司代码 BrokerID;
	///投资者代码 InvestorID	
    sprintf_s(field.InvestorID,sizeof(field.InvestorID)-1,"%d",src.AccountID);
	///报单操作引用OrderActionRef;
	field.OrderActionRef=src.OrderID;	
	///报单引用OrderRef;
    sprintf_s(field.OrderRef,sizeof(field.OrderRef)-1,"%d",src.OrderID);
	///请求编号 RequestID;
	///前置编号 FrontID;
	///会话编号 SessionID;
    field.SessionID=src.LoginSessionID;
	///交易所代码 ExchangeID;
    GET_EXCHANGEID(field.ExchangeID,strInstrument);
	///报单编号
    sprintf_s(field.OrderSysID,sizeof(field.OrderSysID)-1,"%d",src.OrderID);;
	///操作标志
	if(flag==OrderCmdFlag_Amend) field.ActionFlag=THOST_FTDC_AF_Modify;
    else if(flag==OrderCmdFlag_Cancel) field.ActionFlag=THOST_FTDC_AF_Delete;
	///价格
    field.LimitPrice=src.LimitPrice;
	///数量变化
    field.VolumeChange=src.VolumeTraded;
	///用户代码
    sprintf_s(field.UserID,sizeof(field.UserID)-1,"%d",src.AccountID);
	///合约代码
    strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,strInstrument.c_str());
}
//预埋单插入返回
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderField& field)
{
    //暂不实现
}
//预埋单行为返回
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderActionField& field)
{
    //暂不实现
}
//报单状态更新
void CCmeCoreApi::FillRtnOrder(const Stru_Base_OrderInfo& src,PlatformStru_OrderInfo& field)
{
    ///经纪公司代码 BrokerID;
	///投资者代码 InvestorID	
    sprintf_s(field.InvestorID,sizeof(field.InvestorID)-1,"%d",src.AccountID);
	///合约代码InstrumentID;
    string strInstrument=m_ID2ContractName[src.ContractID];
    strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,strInstrument.c_str());
	///报单引用OrderRef;
    sprintf_s(field.OrderRef,sizeof(field.OrderRef)-1,"%d",src.OrderID);
    ///用户代码UserID
    sprintf_s(field.UserID,sizeof(field.UserID)-1,"%d",src.AccountID);
 	///报单价格条件OrderPriceType;
    switch(src.OrderType){
    case OrderType_Market:field.OrderPriceType=THOST_FTDC_OPT_AnyPrice;break;
    case OrderType_Limit:field.OrderPriceType=THOST_FTDC_OPT_LimitPrice;break;
    default:break;
    }
	///买卖方向Direction;
    switch(src.DirectionFlag){
    case DirectionFlag_Long:field.Direction=THOST_FTDC_D_Buy;break;                      //long, also means buy
    case DirectionFlag_Short:field.Direction=THOST_FTDC_D_Sell;break;                      //short, also means sell
    default:break;
    }
	///组合开平标志CombOffsetFlag,外盘为开平信息
    field.CombHedgeFlag[0]=0;
	///组合投机套保标志CombHedgeFlag;
    switch(src.HedgeFlag){
    case HedgeFlag_Speculation:field.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;break;
    case HedgeFlag_Hedge:field.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;break;
    default:break;
    }
	///价格LimitPrice;
    field.LimitPrice=src.LimitPrice;
	///数量VolumeTotalOriginal;
    field.VolumeTotalOriginal=src.Volume;
	///有效期类型TimeCondition;
    switch(src.ExpiryFlag){
    case OrderExpiryFlag_Common:field.TimeCondition=THOST_FTDC_TC_IOC;break;
    case OrderExpiryFlag_GTD:field.TimeCondition=THOST_FTDC_TC_GTD;break;
    case OrderExpiryFlag_GTC:field.TimeCondition=THOST_FTDC_TC_GTC;break;
    default:break;
    }
	///GTD日期GTDDate;
    {
        struct tm* UTCtime=localtime((time_t *)&src.GTDTime);
        sprintf_s(field.GTDDate,sizeof(field.GTDDate)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
    }
    ///成交量类型VolumeCondition;
    field.VolumeCondition= THOST_FTDC_VC_AV;
	///最小成交量 MinVolume;
    field.MinVolume=src.VolumeMin;
	///触发条件 ContingentCondition;
    switch(src.TriggerFlag){
        case TriggerFlag_Immediately:field.ContingentCondition=THOST_FTDC_CC_Immediately;break;
        case TriggerFlag_Time:field.ContingentCondition=THOST_FTDC_CC_ParkedOrder;break;
        default:break;
    }
    //止损价StopPrice;
    field.StopPrice=src.TriggerPriceForStop;
    //强平原因ForceCloseReason;
    STRUCT_SWAP_BEGIN(src.ForceCloseReason)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_NotForceClose,THOST_FTDC_FCC_NotForceClose)          
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_LackDeposit, THOST_FTDC_FCC_LackDeposit)         
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_ClientOverPositionLimit, THOST_FTDC_FCC_ClientOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_MemberOverPositionLimit,THOST_FTDC_FCC_MemberOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_NotMultiple,  THOST_FTDC_FCC_NotMultiple)    
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_Violation,   THOST_FTDC_FCC_Violation)                      
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_Other,    THOST_FTDC_FCC_Other)
    STRUCT_SWAP_END()
	///自动挂起标志 IsAutoSuspend;
    field.IsAutoSuspend=0;
	///业务单元BusinessUnit;
	///请求编号	RequestID;
	///本地报单编号OrderLocalID;
    sprintf_s(field.OrderLocalID,sizeof(field.OrderLocalID)-1,"%d",src.OrderID);
	///交易所代码 ExchangeID;
    GET_EXCHANGEID(field.ExchangeID,strInstrument);
	///会员代码ParticipantID;
	///客户代码ClientID;
    sprintf_s(field.ClientID,sizeof(field.ClientID)-1,"%d",src.AccountID);
	///合约在交易所的代码 ExchangeInstID;
    GET_EXCHANGEID(field.ExchangeInstID,strInstrument);
	///交易所交易员代码 TraderID;
	///安装编号 nstallID;
	///报单提交状态	OrderSubmitStatus
	///报单提示序号NotifySequence;
	///交易日 TradingDay;
    {
        struct tm* UTCtime=localtime((time_t *)&src.TradingDay);
        sprintf_s(field.TradingDay,sizeof(field.TradingDay)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
    }
	///结算编号SettlementID;
	///报单编号 OrderSysID;
    strcpy_s(field.OrderSysID,sizeof(field.OrderSysID)-1,src.OrderExchangeCode);
	///报单来源 OrderSource;
    field.OrderSource=THOST_FTDC_OSRC_Participant;
	///报单状态 OrderStatus;
    STRUCT_SWAP_BEGIN(src.OrderStatus)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_Unknown,THOST_FTDC_OST_Unknown)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_NotTrigged,THOST_FTDC_OST_NotTouched)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_AllTraded,THOST_FTDC_OST_AllTraded)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_NoTradeQueueing,THOST_FTDC_OST_NoTradeQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_NoTradeNotQueueing,THOST_FTDC_OST_PartTradedNotQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_PartTradedQueueing,THOST_FTDC_OST_PartTradedQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_PartTradedNotQueueing,THOST_FTDC_OST_NoTradeNotQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_Canceled,THOST_FTDC_OST_Canceled)
    STRUCT_SWAP_END()
	///报单类型OrderType;
	///今成交数量VolumeTraded;
    field.VolumeTraded=src.VolumeTraded;
	///剩余数量VolumeTotal;
    field.VolumeTotal=src.Volume-src.VolumeTraded;
	///报单日期 InsertDate; 委托时间 InsertTime;   
    {
        struct tm* UTCtime=localtime((time_t *)&src.OrderInsertTime);
        sprintf_s(field.InsertDate,sizeof(field.InsertDate)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
        sprintf_s(field.InsertTime,sizeof(field.InsertTime)-1,"%02d:%02d:%02d",UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec);
    }
	///激活时间 ActiveTime;
	///挂起时间 SuspendTime;
	///最后修改时间 UpdateTime;
    {
        struct tm* UTCtime=localtime((time_t *)&src.UpdateTime);
        sprintf_s(field.UpdateTime,sizeof(field.UpdateTime)-1,"%02d:%02d:%02d",UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec);
    }
	///撤销时间 CancelTime;
    {
        struct tm* UTCtime=localtime((time_t *)&src.OrderCancelTime);
        sprintf_s(field.CancelTime,sizeof(field.CancelTime)-1,"%02d:%02d:%02d",UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec);
    }
	///最后修改交易所交易员代码 ActiveTraderID;
	///结算会员编号 ClearingPartID;
	///序号 SequenceNo;
	///前置编号 FrontID;
	///会话编号 SessionID;
    field.SessionID=src.LoginSessionID;
	///用户端产品信息 UserProductInfo;
    strcpy_s(field.UserProductInfo,sizeof(field.UserProductInfo)-1,src.OrderClientCode);
	///状态信息 StatusMsg;
	///用户强评标志
    if(src.ForceCloseFlag==ForceCloseFlag_Not)  field.UserForceClose=0;
    else if(src.ForceCloseFlag==ForceCloseFlag_CGT)  field.UserForceClose=1;
	///操作用户代码 ActiveUserID;
	///经纪公司报单编号 BrokerOrderSeq;
	///相关报单 RelativeOrderSysID;
	///操作用户代码 ActiveUserID;
	///经纪公司报单编号 BrokerOrderSeq;
	///相关报单 RelativeOrderSysID;
    //成交均价 AvgPrice;
	//价格2 Price2;
    // 本地的请求标号 strLocalRequestID;	
    // 交易账号	strAccount;	
    sprintf_s(field.strAccount,sizeof(field.strAccount)-1,"%d",src.AccountID);		
    // 交易所代码strExchange;	
    GET_EXCHANGEID(field.strExchange,strInstrument);
    //冻结保证金 freezeMargin;              
    //冻结手续费 troubleMoney;              
}
//手续费返回
void CCmeCoreApi::FillCommissionMarginInfo(const Stru_Base_CustomerCommissionInfo& src,PlatformStru_InstrumentCommissionRate& cr)
{
    string strInstrument=m_ID2ContractName[src.ContractID];
    strcpy_s(cr.InstrumentID,sizeof(cr.InstrumentID)-1,strInstrument.c_str());
    sprintf_s(cr.InvestorID,sizeof(cr.InvestorID)-1,"%d",src.CustomerID);
    if(src.AmountOrNumFlag==AmountOrNumFlag_Amount)
    {
        cr.OpenRatioByMoney=src.OpenSpecCommission; 
        cr.CloseRatioByMoney=src.CloseSpecCommission; 
        cr.CloseTodayRatioByMoney=src.CloseTodaySpecCommission;
    }
    else if(src.AmountOrNumFlag==AmountOrNumFlag_Num)
    {
        cr.OpenRatioByVolume=src.OpenSpecCommission;
        cr.CloseRatioByVolume=src.CloseSpecCommission;
        cr.CloseTodayRatioByVolume=src.CloseTodaySpecCommission;
    }
}
//保证金返回
void CCmeCoreApi::FillCommissionMarginInfo(const Stru_Base_CustomerMarginInfo& src,PlatformStru_InstrumentMarginRate&  mr)
{
    string strInstrument=m_ID2ContractName[src.ContractID];
    strcpy_s(mr.InstrumentID,sizeof(mr.InstrumentID)-1,strInstrument.c_str());
    sprintf_s(mr.InvestorID,sizeof(mr.InvestorID)-1,"%d",src.CustomerID);
    if(src.AmountOrNumFlag==AmountOrNumFlag_Amount)
    {
        mr.LongMarginRatioByMoney=src.LongSpecMaintainMargin;
        mr.ShortMarginRatioByMoney=src.ShortSpecMaintainMargin;
    }
    else if(src.AmountOrNumFlag==AmountOrNumFlag_Num)
    {  
        mr.LongMarginRatioByVolume=src.LongSpecMaintainMargin;;
        mr.ShortMarginRatioByVolume=src.ShortSpecMaintainMargin;
    }
    mr.HedgeFlag=THOST_FTDC_HF_Speculation;
}