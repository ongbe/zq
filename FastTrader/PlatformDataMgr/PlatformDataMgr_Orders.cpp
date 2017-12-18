//维护底层报单信息

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 

bool CPlatformDataMgr_Orders::m_bFreezeOrderUnknownStatus=true;

//构造: 
CPlatformDataMgr_Orders::CPlatformDataMgr_Orders(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Rates& refRates,
    CPlatformDataMgr_Quots& refQuots,
    int DiscountCZCE_Comb,
    int DiscountDCE_Comb,
    bool bFreezeOrderUnknownStatus)
: m_RefInstruments(refInstruments),
  m_RefRates(refRates),
  m_RefQuots(refQuots),
  m_DiscountCZCE_Comb(DiscountCZCE_Comb),
  m_DiscountDCE_Comb(DiscountDCE_Comb),
  m_OrderData(baseFTID,baseUpdateSeq,true),
  m_bmapInstrumentMarginOK(false),
  m_TotalMargin(0),
  m_TotalCommission(0)
{
    m_bFreezeOrderUnknownStatus=bFreezeOrderUnknownStatus;
}


CPlatformDataMgr_Orders::~CPlatformDataMgr_Orders(void)
{
    LOG_INFO("CPlatformDataMgr_Orders::~CPlatformDataMgr_Orders(析构CPlatformDataMgr_Orders对象)");
}



//原始报单到来. 报单查询响应时调用
void CPlatformDataMgr_Orders::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_Orders::QryOrderReach");

    string strInstrumentID;

    if(ErrID==0)
    {
        PlatformStru_InstrumentInfo InstrumentInfo;
        strInstrumentID=string(OrderInfo.InstrumentID);
        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            ErrID=-1;
        else OrderInfo.PriceTick=InstrumentInfo.PriceTick;
    }

    Lock_CPlatformDataMgr;

    //将查询结果设置到数据集中
    m_OrderData.QryRltReach_Internal(OrderInfo,ErrID,bIsLast);

    if(ErrID==0)
    {
        //维护OrderSysID 到 OrderKey 的映射关系
        string OrderSysID(OrderInfo.OrderSysID);
        if(!OrderSysID.empty())
        {
            map<string, OrderKey>::const_iterator it=m_mapOrderSysID2OrderKey.find(OrderSysID);
            if(it==m_mapOrderSysID2OrderKey.end())
                m_mapOrderSysID2OrderKey[OrderSysID]=OrderKey(OrderInfo);
        }
    }

    if(bIsLast)
    {
        //如果报单收全, 计算冻结保证金和手续费. 并在需要的时候触发查询费率
        PlatformStru_InstrumentInfo InstrumentInfo;
        map<OrderKey,  PlatformStru_OrderInfo*>::iterator it_all;
        string                          Leg1InstrumentID,Leg2InstrumentID;
        PlatformStru_InstrumentInfo     Leg1InstrumentInfo,Leg2InstrumentInfo;

        //维护单腿合约到组合报单FTID的对应关系
        for(it_all=m_OrderData.m_all.begin();it_all!=m_OrderData.m_all.end();it_all++)
        {
            if(it_all->second==NULL) continue;

            strInstrumentID=string(it_all->first.InstrumentID);
            if(strInstrumentID.empty()) continue;
            if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) continue;

            if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination&&
                m_RefInstruments.GetLegInstrumentID(strInstrumentID,Leg1InstrumentID,Leg2InstrumentID))
            {
                if(!CTools_Ansi::IsKeyValueInMultimap<string,long>(m_mapLegID12CombOrderFTID,Leg1InstrumentID,it_all->second->FTID))
                    m_mapLegID12CombOrderFTID.insert(pair<string,long>(Leg1InstrumentID,it_all->second->FTID));
                if(!CTools_Ansi::IsKeyValueInMultimap<string,long>(m_mapLegID22CombOrderFTID,Leg2InstrumentID,it_all->second->FTID))
                    m_mapLegID22CombOrderFTID.insert(pair<string,long>(Leg2InstrumentID,it_all->second->FTID));
            }
        }

        //计算报单冻结的保证金和手续费. 如果是组合合约, 按分腿计算
        for(it_all=m_OrderData.m_all.begin();it_all!=m_OrderData.m_all.end();it_all++)
        {
            if(it_all->second==NULL) continue;

            strInstrumentID=string(it_all->first.InstrumentID);
            if(strInstrumentID.empty()) continue;
            if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) continue;

            bool bUpdate=false;
            if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Futures)
            {
                //期货保证金手续费计算
                CalcuOrderFreezeCommission_Internal(*(it_all->second),bUpdate,InstrumentInfo);
                CalcuOrderFreezeMargin_Internal(*(it_all->second),bUpdate,InstrumentInfo);
            }
            else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
            {
                //期权保证金手续费计算
                CalcuOrderFreezeCommission_Internal(*(it_all->second),bUpdate,InstrumentInfo);
                CalcuOrderFreezeMargin_Option_Internal(*(it_all->second),bUpdate,InstrumentInfo);
            }
            else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Futures&&
                    m_RefInstruments.GetLegInstrumentID(strInstrumentID,Leg1InstrumentID,Leg2InstrumentID)&&
                    m_RefInstruments.GetInstrumentInfo(Leg1InstrumentID,Leg1InstrumentInfo)&&
                    m_RefInstruments.GetInstrumentInfo(Leg2InstrumentID,Leg2InstrumentInfo))
            {
                //期货组合保证金手续费计算
                CalcuOrderFreezeCommissionComb_Internal(*(it_all->second),bUpdate,Leg1InstrumentInfo,Leg2InstrumentInfo);
                CalcuOrderFreezeMarginComb_Internal(*(it_all->second),bUpdate,Leg1InstrumentInfo,Leg2InstrumentInfo);
            }
            if(bUpdate) it_all->second->UpdateSeq=m_OrderData.NewUpdateSeq(it_all->second->FTID);

        }

        //更新全部冻结保证金
        UpdateAllMargin_Internal();
        //更新全部冻结手续费
        UpdateAllCommission_Internal();
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Orders::QryOrderReach-end");
}


//新报单到来. 报单回报时调用
//返回原来(更新前)的报单信息, 如果是新报单, 则返回的原报单信息无效
void CPlatformDataMgr_Orders::NewOrderReady(PlatformStru_OrderInfo& OrderInfo,
                                            PlatformStru_OrderInfo& OldOrderInfo,
                                            const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    LOG_INFO("CPlatformDataMgr_Orders::NewOrderReady");

    Lock_CPlatformDataMgr;

    PlatformStru_OrderInfo *pOrder;
    OrderKey key(OrderInfo);
    string Leg1InstrumentID,Leg2InstrumentID;
    PlatformStru_InstrumentInfo Leg1InstrumentInfo,Leg2InstrumentInfo;
    PlatformStru_DepthMarketData Leg1QuotData,Leg2QuotData;

    if(m_OrderData.GetValue_Internal(pOrder,key)) OldOrderInfo=*pOrder;
    else OldOrderInfo=PlatformStru_OrderInfo(true); //binvalid=true

    //设置对应的PriceTick
    OrderInfo.PriceTick=InstrumentInfo.PriceTick;

    bool bUpdate=false;

    //计算报单冻结的保证金和手续费. 如果是组合合约, 按分腿计算
    if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Futures)
    {
        CalcuOrderFreezeCommission_Internal(OrderInfo,bUpdate,InstrumentInfo);
        CalcuOrderFreezeMargin_Internal(OrderInfo,bUpdate,InstrumentInfo);
    }
    else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
    {
        //期权保证金手续费计算
        CalcuOrderFreezeCommission_Internal(OrderInfo,bUpdate,InstrumentInfo);
        CalcuOrderFreezeMargin_Option_Internal(OrderInfo,bUpdate,InstrumentInfo);
    }
    else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination&&
        m_RefInstruments.GetLegInstrumentID(string(OrderInfo.InstrumentID),Leg1InstrumentID,Leg2InstrumentID)&&
        m_RefInstruments.GetInstrumentInfo(Leg1InstrumentID,Leg1InstrumentInfo)&&
        m_RefInstruments.GetInstrumentInfo(Leg2InstrumentID,Leg2InstrumentInfo))
    {
        CalcuOrderFreezeCommissionComb_Internal(OrderInfo,bUpdate,Leg1InstrumentInfo,Leg2InstrumentInfo);
        CalcuOrderFreezeMarginComb_Internal(OrderInfo,bUpdate,Leg1InstrumentInfo,Leg2InstrumentInfo);
    }

    m_OrderData.NewReach_Internal(OrderInfo);

    //维护OrderSysID 到 OrderKey 的映射关系
    string OrderSysID(OrderInfo.OrderSysID);
    if(!OrderSysID.empty())
    {
        map<string, OrderKey>::const_iterator it=m_mapOrderSysID2OrderKey.find(OrderSysID);
        if(it==m_mapOrderSysID2OrderKey.end())
            m_mapOrderSysID2OrderKey[OrderSysID]=key;
    }

    //维护单腿合约到组合报单key的对应关系
    if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination)
    {
        if(!CTools_Ansi::IsKeyValueInMultimap<string,long>(m_mapLegID12CombOrderFTID,Leg1InstrumentID,OrderInfo.FTID))
            m_mapLegID12CombOrderFTID.insert(pair<string,long>(Leg1InstrumentID,OrderInfo.FTID));
        if(!CTools_Ansi::IsKeyValueInMultimap<string,long>(m_mapLegID22CombOrderFTID,Leg2InstrumentID,OrderInfo.FTID))
            m_mapLegID22CombOrderFTID.insert(pair<string,long>(Leg2InstrumentID,OrderInfo.FTID));
    }

    //根据报单变化, 调整保证金
    UpdateMarginByOrder_Internal(OrderInfo,OldOrderInfo);
    //根据报单变化, 调整手续费
    UpdateCommissionByOrder_Internal(OrderInfo,OldOrderInfo);

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Orders::NewOrderReady-end");
}


//更新合约相关的所有报单中的冻结保证金, 返回合约编号是否有报单, 决定是否需要更新资金面板. 查询保证金率响应时调用
bool CPlatformDataMgr_Orders::UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    //获取和合约相关的报单FTID集合, 包括单腿为该合约的组合报单    
    set<long> FTIDs;
    GetFTIDSetOfInstrument2_Internal(strInstrumentID,FTIDs);

    PlatformStru_InstrumentInfo CombInstrumentInfo;
    string Leg1InstrumentID,Leg2InstrumentID;
    PlatformStru_DepthMarketData Leg1QuotData,Leg2QuotData;
    PlatformStru_InstrumentInfo Leg1InstrumentInfo,Leg2InstrumentInfo;
    PlatformStru_OrderInfo *pOrderInfo;

    //计算每个报单的冻结保证金
    for(set<long>::iterator itFTID=FTIDs.begin();itFTID!=FTIDs.end();itFTID++)
    {
        if(!m_OrderData.GetValue2_Internal(pOrderInfo,*itFTID)||pOrderInfo==NULL) continue;

        string thisInstrumentID(pOrderInfo->InstrumentID);
        if(thisInstrumentID!=strInstrumentID&&!m_RefInstruments.GetInstrumentInfo(thisInstrumentID,CombInstrumentInfo)) continue;

        const PlatformStru_InstrumentInfo& thisInstrumentInfo(thisInstrumentID==strInstrumentID ? InstrumentInfo : CombInstrumentInfo);

        bool bUpdate=false;

        //计算报单冻结的保证金. 如果是组合合约报单, 按分腿计算
        if(thisInstrumentInfo.ProductClass==THOST_FTDC_PC_Futures)
        {
            CalcuOrderFreezeMargin_Internal(*pOrderInfo,bUpdate,thisInstrumentInfo);
        }
        else if(thisInstrumentInfo.ProductClass==THOST_FTDC_PC_Options||thisInstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
        {
            CalcuOrderFreezeMargin_Option_Internal(*pOrderInfo,bUpdate,thisInstrumentInfo);
        }
        else if(thisInstrumentInfo.ProductClass==THOST_FTDC_PC_Combination&&
            m_RefInstruments.GetLegInstrumentID(thisInstrumentID,Leg1InstrumentID,Leg2InstrumentID)&&
            m_RefInstruments.GetInstrumentInfo(Leg1InstrumentID,Leg1InstrumentInfo)&&
            m_RefInstruments.GetInstrumentInfo(Leg2InstrumentID,Leg2InstrumentInfo))
        {
            CalcuOrderFreezeMarginComb_Internal(*pOrderInfo,bUpdate,Leg1InstrumentInfo,Leg2InstrumentInfo);
        }
        if(bUpdate) pOrderInfo->UpdateSeq=m_OrderData.NewUpdateSeq(pOrderInfo->FTID);
    }
    brlt=FTIDs.size()>0?true:false;

    //更新全部冻结保证金
    UpdateAllMargin_Internal();

    Unlock_CPlatformDataMgr;

    return brlt;
}

//更新所有报单中的冻结手续费, 返回合约编号是否有报单, 决定是否需要更新资金面板. 查询手续费率响应时调用
//计算组合合约报单中的冻结保证金时，由于报单中没有分腿报价, 用昨结算价计算分腿冻结保证金，因此需要行情中的昨结算价
bool CPlatformDataMgr_Orders::UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(const string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    set<long> FTIDs;
    GetFTIDSetOfInstrument2_Internal(strInstrumentID,FTIDs);

    PlatformStru_InstrumentInfo CombInstrumentInfo;
    string Leg1InstrumentID,Leg2InstrumentID;
    PlatformStru_DepthMarketData Leg1QuotData,Leg2QuotData;
    PlatformStru_InstrumentInfo Leg1InstrumentInfo,Leg2InstrumentInfo;
    PlatformStru_OrderInfo *pOrderInfo;
    for(set<long>::iterator itFTID=FTIDs.begin();itFTID!=FTIDs.end();itFTID++)
    {
        if(!m_OrderData.GetValue2_Internal(pOrderInfo,*itFTID)||pOrderInfo==NULL) continue;

        string thisInstrumentID(pOrderInfo->InstrumentID);
        if(thisInstrumentID!=strInstrumentID&&!m_RefInstruments.GetInstrumentInfo(thisInstrumentID,CombInstrumentInfo)) continue;

        const PlatformStru_InstrumentInfo& thisInstrumentInfo(thisInstrumentID==strInstrumentID ? InstrumentInfo : CombInstrumentInfo);

        bool bUpdate=false;

        //计算报单冻结的手续费. 如果是组合合约报单, 按分腿计算
        if(thisInstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination)
        {
            CalcuOrderFreezeCommission_Internal(*pOrderInfo,bUpdate,thisInstrumentInfo);
        }
        else if(m_RefInstruments.GetLegInstrumentID(thisInstrumentID,Leg1InstrumentID,Leg2InstrumentID)&&
            m_RefInstruments.GetInstrumentInfo(Leg1InstrumentID,Leg1InstrumentInfo)&&
            m_RefInstruments.GetInstrumentInfo(Leg2InstrumentID,Leg2InstrumentInfo))
        {
            CalcuOrderFreezeCommissionComb_Internal(*pOrderInfo,bUpdate,Leg1InstrumentInfo,Leg2InstrumentInfo);
        }
        if(bUpdate) pOrderInfo->UpdateSeq=m_OrderData.NewUpdateSeq(pOrderInfo->FTID);

    }
    brlt=FTIDs.size()>0?true:false;

    //更新全部冻结手续费
    UpdateAllCommission_Internal();

    Unlock_CPlatformDataMgr;

    return brlt;
}


//计算单合约报单信息中的冻结保证金. 含未知状态的报单。应用于单合约报单。
void CPlatformDataMgr_Orders::CalcuOrderFreezeMargin_Internal(
    PlatformStru_OrderInfo& OrderInfo,
    bool& bUpdate,
    const PlatformStru_InstrumentInfo& InstrumentInfo)
{
	PlatformStru_InstrumentMarginRate MarginRate;
    double freezeMargin=util::GetDoubleInvalidValue();
    double freezeExchMargin=util::GetDoubleInvalidValue();

    //冻结保证金算过就不用再算了
    if(OrderInfo.freezeMargin!=util::GetDoubleInvalidValue())
        return;

	if ( OrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_Open && IsOrderWorking(OrderInfo))
	{	
		if( ( m_RefRates.GetMarginRate( OrderInfo.InstrumentID, MarginRate) ) == 0 )
		{
		    if ( OrderInfo.Direction == THOST_FTDC_D_Buy ) 
		        freezeMargin =  OrderInfo.LimitPrice * OrderInfo.VolumeTotal * InstrumentInfo.VolumeMultiple * MarginRate.LongMarginRatioByMoney +
                                MarginRate.LongMarginRatioByVolume * OrderInfo.VolumeTotal;
            else if ( OrderInfo.Direction == THOST_FTDC_D_Sell ) 
		        freezeMargin =  OrderInfo.LimitPrice * OrderInfo.VolumeTotal * InstrumentInfo.VolumeMultiple * MarginRate.ShortMarginRatioByMoney +
                                MarginRate.ShortMarginRatioByVolume * OrderInfo.VolumeTotal;
            else freezeMargin=0.0;
        }

        if ( OrderInfo.Direction == THOST_FTDC_D_Buy && !util::isInvalidValue(InstrumentInfo.LongMarginRatio)) 
            freezeExchMargin =  OrderInfo.LimitPrice * OrderInfo.VolumeTotal * InstrumentInfo.VolumeMultiple * InstrumentInfo.LongMarginRatio;
        else if ( OrderInfo.Direction == THOST_FTDC_D_Sell && !util::isInvalidValue(InstrumentInfo.ShortMarginRatio)) 
            freezeExchMargin =  OrderInfo.LimitPrice * OrderInfo.VolumeTotal * InstrumentInfo.VolumeMultiple * InstrumentInfo.ShortMarginRatio;
        else if(OrderInfo.Direction != THOST_FTDC_D_Buy && OrderInfo.Direction != THOST_FTDC_D_Sell)
            freezeExchMargin=0.0;
	}
    else
    {
        freezeMargin=0.00;    //其它情况冻结保证金为0
        freezeExchMargin=0.0;
    }

    if(OrderInfo.freezeMargin!=freezeMargin) { OrderInfo.freezeMargin=freezeMargin;  bUpdate=true; }
    if(OrderInfo.freezeExchMargin!=freezeExchMargin) { OrderInfo.freezeExchMargin=freezeExchMargin;  bUpdate=true; }
}

//计算单合约期权报单信息中的冻结保证金. 含未知状态的报单。未考虑组合期权之情况
//需要用到行情中的昨结算价
//每手卖方交易保证金 = MAX(权利金 + 投资者期权合约保证金不变部分, 投资者期权合约最小保证金)
//权利金 = 价格（冻结使用昨结算价）* 期权合约乘数
void CPlatformDataMgr_Orders::CalcuOrderFreezeMargin_Option_Internal(
    PlatformStru_OrderInfo& OrderInfo,
    bool& bUpdate,
    const PlatformStru_InstrumentInfo& InstrumentInfo)
{
	PlatformStru_InstrumentMarginRate MarginRate;
    PlatformStru_DepthMarketData    QuotData;
    double freezeMargin=util::GetDoubleInvalidValue();
    double freezeExchMargin=util::GetDoubleInvalidValue();

    //冻结保证金算过就不用再算了
    if(OrderInfo.freezeMargin!=util::GetDoubleInvalidValue())
        return;

	if ( OrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_Open && 
         IsOrderWorking(OrderInfo)&&
         OrderInfo.Direction == THOST_FTDC_D_Sell)
	{	
		if( m_RefRates.GetMarginRate( OrderInfo.InstrumentID, MarginRate) == 0 &&
            m_RefQuots.GetQuotInfo(string(OrderInfo.InstrumentID),QuotData)&&
            !util::isInvalidValue(QuotData.PreSettlementPrice))
		{
            //计算权利金(权利金 = 价格（冻结使用昨结算价）* 期权合约乘数)
            double Royalty=QuotData.PreSettlementPrice * InstrumentInfo.VolumeMultiple;

            //计算冻结保证金(每手卖方交易保证金 = MAX(权利金 + 投资者期权合约保证金不变部分, 投资者期权合约最小保证金))
            if(!util::isInvalidValue(MarginRate.FixedMargin)&&!util::isInvalidValue(MarginRate.MiniMargin))
                freezeMargin = ( util::greaterOrEqual(Royalty+MarginRate.FixedMargin,MarginRate.MiniMargin) ? (Royalty+MarginRate.FixedMargin) : MarginRate.MiniMargin ) * OrderInfo.VolumeTotal;
            if(!util::isInvalidValue(MarginRate.ExchFixedMargin)&&!util::isInvalidValue(MarginRate.ExchMiniMargin))
                freezeExchMargin = ( util::greaterOrEqual(Royalty+MarginRate.ExchFixedMargin,MarginRate.ExchMiniMargin) ? (Royalty+MarginRate.ExchFixedMargin) : MarginRate.ExchMiniMargin ) * OrderInfo.VolumeTotal;
        }
	}
    else
    {
        freezeMargin=0.00;    //其它情况冻结保证金为0
        freezeExchMargin=0.0;
    }

    if(OrderInfo.freezeMargin!=freezeMargin) { OrderInfo.freezeMargin=freezeMargin;  bUpdate=true; }
    if(OrderInfo.freezeExchMargin!=freezeExchMargin) { OrderInfo.freezeExchMargin=freezeExchMargin;  bUpdate=true; }
}

//计算组合合约报单中的冻结保证金. 含未知状态的报单
//由于报单中没有分腿报价, 用昨结算价计算分腿冻结保证金
//如果是开仓挂单，需要行情中的昨结算价
void CPlatformDataMgr_Orders::CalcuOrderFreezeMarginComb_Internal(
    PlatformStru_OrderInfo& OrderInfo,
    bool& bUpdate,
    const PlatformStru_InstrumentInfo& Leg1InstrumentInfo,
    const PlatformStru_InstrumentInfo& Leg2InstrumentInfo)
{
    double Leg1FreezeMargin=util::GetDoubleInvalidValue();
    double Leg2FreezeMargin=util::GetDoubleInvalidValue();
    double freezeMargin=util::GetDoubleInvalidValue();
    double Leg1FreezeExchMargin=util::GetDoubleInvalidValue();
    double Leg2FreezeExchMargin=util::GetDoubleInvalidValue();
    double freezeExchMargin=util::GetDoubleInvalidValue();

    //冻结保证金算过就不用再算了
    if(!util::isInvalidValue(OrderInfo.freezeMargin))
        return;

	if ( OrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_Open && IsOrderWorking(OrderInfo))
	{	
        PlatformStru_DepthMarketData    Leg1QuotData,Leg2QuotData;

        if(!m_RefQuots.GetQuotInfo(string(Leg1InstrumentInfo.InstrumentID),Leg1QuotData)||
           !m_RefQuots.GetQuotInfo(string(Leg2InstrumentInfo.InstrumentID),Leg2QuotData))
           return;

        //检查昨结算价是否有效
        if(util::isInvalidValue(Leg1QuotData.PreSettlementPrice)||
           util::isInvalidValue(Leg2QuotData.PreSettlementPrice))
            return;

		PlatformStru_InstrumentMarginRate Leg1MarginRate;
        PlatformStru_InstrumentMarginRate Leg2MarginRate;
        if( m_RefRates.GetMarginRate( Leg1InstrumentInfo.InstrumentID, Leg1MarginRate) == 0 &&
            m_RefRates.GetMarginRate( Leg2InstrumentInfo.InstrumentID, Leg2MarginRate) == 0 )
		{
		    if ( OrderInfo.Direction == THOST_FTDC_D_Buy ) 
            {
                Leg1FreezeMargin =  Leg1MarginRate.LongMarginRatioByMoney * OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple +
                                    Leg1MarginRate.LongMarginRatioByVolume * OrderInfo.VolumeTotal;
                Leg2FreezeMargin =  Leg2MarginRate.ShortMarginRatioByMoney * OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple +
                                    Leg2MarginRate.ShortMarginRatioByVolume * OrderInfo.VolumeTotal;
            }
            else if ( OrderInfo.Direction == THOST_FTDC_D_Sell ) 
            {
                Leg1FreezeMargin =  Leg1MarginRate.ShortMarginRatioByMoney * OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple +
                                    Leg1MarginRate.ShortMarginRatioByVolume * OrderInfo.VolumeTotal;
                Leg2FreezeMargin =  Leg2MarginRate.LongMarginRatioByMoney * OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple +
                                    Leg2MarginRate.LongMarginRatioByVolume * OrderInfo.VolumeTotal;
            }

		    if ( OrderInfo.Direction == THOST_FTDC_D_Buy ) 
            {
                Leg1FreezeExchMargin =  Leg1InstrumentInfo.LongMarginRatio * OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple;
                Leg2FreezeExchMargin =  Leg2InstrumentInfo.ShortMarginRatio * OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple;
            }
            else if ( OrderInfo.Direction == THOST_FTDC_D_Sell ) 
            {
                Leg1FreezeExchMargin =  Leg1InstrumentInfo.ShortMarginRatio * OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple;
                Leg2FreezeExchMargin =  Leg2InstrumentInfo.LongMarginRatio * OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple;
            }

            int Discount=0;
            if(strncmp(Leg1InstrumentInfo.ExchangeID,"CZCE",sizeof(Leg1InstrumentInfo.ExchangeID)-1)==0) Discount=m_DiscountCZCE_Comb;
            else if(strncmp(Leg1InstrumentInfo.ExchangeID,"DCE",sizeof(Leg1InstrumentInfo.ExchangeID)-1)==0) Discount=m_DiscountDCE_Comb;

            if(Discount==1)          //第一腿优惠
            {
                Leg1FreezeMargin=0;
                Leg1FreezeExchMargin=0;
            }
            else if(Discount==2)     //第二腿优惠
            {
                Leg2FreezeMargin=0;
                Leg2FreezeExchMargin=0;
            }
            else if(Discount==3)    //小边优惠，取大边。(多数情况)
            {
                if(util::equals(Leg1FreezeExchMargin,Leg2FreezeExchMargin))   //两边相等, 优惠掉第二边
                {
                    Leg2FreezeMargin=0;
                    Leg2FreezeExchMargin=0;
                }
                else if(Leg1FreezeExchMargin<Leg2FreezeExchMargin) 
                {
                    Leg1FreezeMargin=0;
                    Leg1FreezeExchMargin=0;
                }
                else 
                {
                    Leg2FreezeMargin=0;
                    Leg2FreezeExchMargin=0;
                }
            }
        }
	}
    else
    {
        //其它情况冻结保证金为0
        Leg1FreezeMargin=0.00;    
        Leg2FreezeMargin=0.00;
        Leg1FreezeExchMargin=0.00;    
        Leg2FreezeExchMargin=0.00;
    }

    if(!util::isInvalidValue(Leg1FreezeMargin)&&!util::isInvalidValue(Leg2FreezeMargin))
        freezeMargin=Leg1FreezeMargin+Leg2FreezeMargin;
    if(!util::isInvalidValue(Leg1FreezeExchMargin)&&!util::isInvalidValue(Leg2FreezeExchMargin))
        freezeExchMargin=Leg1FreezeExchMargin+Leg2FreezeExchMargin;

    if(!util::equals(OrderInfo.Leg1FreezeMargin,Leg1FreezeMargin)) { OrderInfo.Leg1FreezeMargin=Leg1FreezeMargin;  bUpdate=true; }
    if(!util::equals(OrderInfo.Leg2FreezeMargin,Leg2FreezeMargin)) { OrderInfo.Leg2FreezeMargin=Leg2FreezeMargin;  bUpdate=true; }
    if(!util::equals(OrderInfo.freezeMargin,freezeMargin)) { OrderInfo.freezeMargin=freezeMargin;  bUpdate=true; }
    if(!util::equals(OrderInfo.Leg1FreezeExchMargin,Leg1FreezeExchMargin)) { OrderInfo.Leg1FreezeExchMargin=Leg1FreezeExchMargin;  bUpdate=true; }
    if(!util::equals(OrderInfo.Leg2FreezeExchMargin,Leg2FreezeExchMargin)) { OrderInfo.Leg2FreezeExchMargin=Leg2FreezeExchMargin;  bUpdate=true; }
    if(!util::equals(OrderInfo.freezeExchMargin,freezeExchMargin)) { OrderInfo.freezeExchMargin=freezeExchMargin;  bUpdate=true; }
}


//计算单合约报单信息中的冻结手续费. 含未知状态的报单
void CPlatformDataMgr_Orders::CalcuOrderFreezeCommission_Internal(
    PlatformStru_OrderInfo& OrderInfo,
    bool& bUpdate,
    const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    PlatformStru_InstrumentCommissionRate CommissionRate;
	double troubleMoney=util::GetDoubleInvalidValue();

    //冻结手续费算过就不用再算了
    if(OrderInfo.troubleMoney!=util::GetDoubleInvalidValue())
        return;

	//是挂单
    if(IsOrderWorking(OrderInfo))
	{	
        char OffsetFlag=OrderInfo.CombOffsetFlag[0];
        string strExchangeID(InstrumentInfo.ExchangeID);
        //郑商大商的平今是ctp模拟的，实际上是平仓
        if(OffsetFlag==THOST_FTDC_OF_CloseToday&&(strExchangeID==string("DCE")||strExchangeID==string("CZCE")))
            OffsetFlag=THOST_FTDC_OF_Close;
		if( ( m_RefRates.GetCommissionRate( OrderInfo.InstrumentID, CommissionRate) ) == 0 )
        {
            switch (OffsetFlag)
            {
                case THOST_FTDC_OF_Open:            //开仓
                    troubleMoney =    OrderInfo.VolumeTotal * CommissionRate.OpenRatioByVolume+
                                      OrderInfo.VolumeTotal * OrderInfo.LimitPrice * InstrumentInfo.VolumeMultiple * CommissionRate.OpenRatioByMoney;
                    break;
                case THOST_FTDC_OF_CloseToday:  //平今
			        troubleMoney =    OrderInfo.VolumeTotal * CommissionRate.CloseTodayRatioByVolume+
                                      OrderInfo.VolumeTotal * OrderInfo.LimitPrice * InstrumentInfo.VolumeMultiple * CommissionRate.CloseTodayRatioByMoney;
                    break;
                case THOST_FTDC_OF_Close: 
                case THOST_FTDC_OF_ForceClose: 
                case THOST_FTDC_OF_CloseYesterday: //平仓 (对于大商、郑商、中金, 平仓包括平今仓, 但在计算报单冻结时不考虑, 都按平仓计算)
                    troubleMoney =    OrderInfo.VolumeTotal * CommissionRate.CloseRatioByVolume+
                                      OrderInfo.VolumeTotal * OrderInfo.LimitPrice * InstrumentInfo.VolumeMultiple * CommissionRate.CloseRatioByMoney;
                    break;
                default:
                    troubleMoney = 0.0;
            }
        }
	}
    else
    {
        troubleMoney = 0.00;
    }

    if(troubleMoney!=OrderInfo.troubleMoney) { OrderInfo.troubleMoney=troubleMoney;  bUpdate=true; }
}



//计算组合合约报单中的冻结手续费. 需要按双边分别计算. 含未知状态的报单
//由于报单中没有分腿报价, 用昨结算价计算分腿冻结手续费
//如果是挂单，需要行情中的昨结算价
void CPlatformDataMgr_Orders::CalcuOrderFreezeCommissionComb_Internal(
    PlatformStru_OrderInfo& OrderInfo,
    bool& bUpdate,
    const PlatformStru_InstrumentInfo& Leg1InstrumentInfo,
    const PlatformStru_InstrumentInfo& Leg2InstrumentInfo)
{
	double Leg1TroubleMoney=util::GetDoubleInvalidValue();
	double Leg2TroubleMoney=util::GetDoubleInvalidValue();
	double troubleMoney=util::GetDoubleInvalidValue();

    //冻结手续费算过就不用再算了
    if(OrderInfo.troubleMoney!=util::GetDoubleInvalidValue())
        return;

    if(IsOrderWorking(OrderInfo))
	{	
        PlatformStru_DepthMarketData    Leg1QuotData,Leg2QuotData;

        if(!m_RefQuots.GetQuotInfo(string(Leg1InstrumentInfo.InstrumentID),Leg1QuotData)||
           !m_RefQuots.GetQuotInfo(string(Leg2InstrumentInfo.InstrumentID),Leg2QuotData))
           return;

        //检查昨结算价是否有效
        if(Leg1QuotData.PreSettlementPrice==util::GetDoubleInvalidValue()||
            Leg2QuotData.PreSettlementPrice==util::GetDoubleInvalidValue())
            return;

		PlatformStru_InstrumentCommissionRate Leg1CommissionRate;
        PlatformStru_InstrumentCommissionRate Leg2CommissionRate;
        if( m_RefRates.GetCommissionRate( Leg1InstrumentInfo.InstrumentID, Leg1CommissionRate) == 0 &&
            m_RefRates.GetCommissionRate( Leg2InstrumentInfo.InstrumentID, Leg2CommissionRate) == 0 )
        {
            switch ( OrderInfo.CombOffsetFlag[0])
            {
                case THOST_FTDC_OF_Open://开仓
                    Leg1TroubleMoney =  OrderInfo.VolumeTotal * Leg1CommissionRate.OpenRatioByVolume +
                                        OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple * Leg1CommissionRate.OpenRatioByMoney;
                    Leg2TroubleMoney =  OrderInfo.VolumeTotal * Leg2CommissionRate.OpenRatioByVolume +
                                        OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple * Leg2CommissionRate.OpenRatioByMoney;
                    break;
                case THOST_FTDC_OF_CloseToday: //平今
		            Leg1TroubleMoney =  OrderInfo.VolumeTotal * Leg1CommissionRate.CloseTodayRatioByVolume +
                                        OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple * Leg1CommissionRate.CloseTodayRatioByMoney;
                    Leg2TroubleMoney =  OrderInfo.VolumeTotal * Leg2CommissionRate.CloseTodayRatioByVolume +
                                        OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple * Leg2CommissionRate.CloseTodayRatioByMoney;
                    break;
                case THOST_FTDC_OF_Close: 
                case THOST_FTDC_OF_ForceClose: 
                case THOST_FTDC_OF_CloseYesterday: //平仓 -- 普通平仓和平昨
                    Leg1TroubleMoney =  OrderInfo.VolumeTotal * Leg1CommissionRate.CloseRatioByVolume +
                                        OrderInfo.VolumeTotal * Leg1QuotData.PreSettlementPrice * Leg1InstrumentInfo.VolumeMultiple * Leg1CommissionRate.CloseRatioByMoney;
                    Leg2TroubleMoney =  OrderInfo.VolumeTotal * Leg2CommissionRate.CloseRatioByVolume +
                                        OrderInfo.VolumeTotal * Leg2QuotData.PreSettlementPrice * Leg2InstrumentInfo.VolumeMultiple * Leg2CommissionRate.CloseRatioByMoney;
                    break;
                default:
                    Leg1TroubleMoney = 0.0;
                    Leg2TroubleMoney = 0.0;
                    break;
            }
        }
	}
    else
    {
        Leg1TroubleMoney = 0.0;
        Leg2TroubleMoney = 0.0;
    }

    if(!util::isInvalidValue(Leg1TroubleMoney)&&!util::isInvalidValue(Leg2TroubleMoney))
        troubleMoney=Leg1TroubleMoney+Leg2TroubleMoney;

    if(!util::equals(OrderInfo.Leg1TroubleMoney,Leg1TroubleMoney)) { OrderInfo.Leg1TroubleMoney=Leg1TroubleMoney;  bUpdate=true; }
    if(!util::equals(OrderInfo.Leg2TroubleMoney,Leg2TroubleMoney)) { OrderInfo.Leg2TroubleMoney=Leg2TroubleMoney;  bUpdate=true; }
    if(!util::equals(OrderInfo.troubleMoney,troubleMoney)) { OrderInfo.troubleMoney=troubleMoney;  bUpdate=true; }
}





///获取指定报单
bool CPlatformDataMgr_Orders::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    bool brlt;

    Lock_CPlatformDataMgr;

    PlatformStru_OrderInfo *pOrder;
    if(m_OrderData.GetValue_Internal(pOrder,inOrderKey))
    {
        outData=*pOrder;
        brlt=true;
    }
    else
    {
        outData=PlatformStru_OrderInfo(true);   //binvalid=true
        brlt=false;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

///获取指定报单
bool CPlatformDataMgr_Orders::GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData)
{
    bool brlt;

    Lock_CPlatformDataMgr;

    map<string, OrderKey>::const_iterator it=m_mapOrderSysID2OrderKey.find(strOrderSysID);
    if(it==m_mapOrderSysID2OrderKey.end())
    {
        outData=PlatformStru_OrderInfo(true);   //binvalid=true
        brlt=false;
    }
    else
    {
        PlatformStru_OrderInfo *pOrder;
        if(m_OrderData.GetValue_Internal(pOrder,it->second))
        {
            outData=*pOrder;
            brlt=true;
        }
        else
        {
            outData=PlatformStru_OrderInfo(true);   //binvalid=true
            brlt=false;
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

///获取全部报单
int CPlatformDataMgr_Orders::GetAllOrders(vector<PlatformStru_OrderInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_OrderData.GetAll2_Internal(outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}


///获取全部报单
int CPlatformDataMgr_Orders::GetAllOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_OrderData.GetAllAboutInstrument2_Internal(strInstrument,outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

///获取全部已触发的报单
int CPlatformDataMgr_Orders::GetTriggerOrders(vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if(IsOrderTrigger(*(it->second)))
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}


//获取和合约相关的报单FTID集合, 包括单腿为该合约的组合报单
void CPlatformDataMgr_Orders::GetFTIDSetOfInstrument2_Internal(const string& strInstrument,set<long>&FTIDs)
{
    FTIDs.clear();

    //取合约对应的报单key
    set<long> *pFTIDs;
    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDs,strInstrument))
        FTIDs=*pFTIDs;

    //取合约作为第一腿对应的组合报单FTID
    multimap<string,long>::const_iterator itlower=m_mapLegID12CombOrderFTID.lower_bound(strInstrument);
    multimap<string,long>::const_iterator itupper=m_mapLegID12CombOrderFTID.upper_bound(strInstrument);
    while(itlower!=itupper)
    {
        if(FTIDs.find(itlower->second)==FTIDs.end())
            FTIDs.insert(itlower->second);
        itlower++;
    }

    //取合约作为第二腿对应的组合报单FTID
    itlower=m_mapLegID22CombOrderFTID.lower_bound(strInstrument);
    itupper=m_mapLegID22CombOrderFTID.upper_bound(strInstrument);
    while(itlower!=itupper)
    {
        if(FTIDs.find(itlower->second)==FTIDs.end())
            FTIDs.insert(itlower->second);
        itlower++;
    }
}

///获取合约相关的已触发的报单
int CPlatformDataMgr_Orders::GetTriggerOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    set<long> FTIDs;

    GetFTIDSetOfInstrument2_Internal(strInstrument,FTIDs);

    //取FTIDs对应的报单信息
    PlatformStru_OrderInfo *pOrderInfo;
    for(set<long>::const_iterator it=FTIDs.begin();it!=FTIDs.end();it++)
    {
        if(m_OrderData.GetValue2_Internal(pOrderInfo,*it)&&
            IsOrderTrigger(*pOrderInfo))
        {
            outData.push_back(*pOrderInfo);
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取指定已触发的报单
bool CPlatformDataMgr_Orders::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        IsOrderTrigger(*(it->second)))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}


///获取已成交报单
int CPlatformDataMgr_Orders::GetTradedOrders(vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if(IsOrderTraded(*(it->second)))
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取已成交报单
int CPlatformDataMgr_Orders::GetTradedOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    set<long> FTIDs;
    GetFTIDSetOfInstrument2_Internal(strInstrument,FTIDs);

    //取keys对应的报单信息
    PlatformStru_OrderInfo *pOrderInfo;
    for(set<long>::const_iterator it=FTIDs.begin();it!=FTIDs.end();it++)
    {
        if(m_OrderData.GetValue2_Internal(pOrderInfo,*it)&&
           IsOrderTraded(*pOrderInfo))
        {
            outData.push_back(*pOrderInfo);
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取指定的已成交报单
bool CPlatformDataMgr_Orders::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        IsOrderTraded(*(it->second)))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

///获取已撤单和错误报单
int CPlatformDataMgr_Orders::GetCanceledOrders(vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if( it->second->OrderStatus==THOST_FTDC_OST_Canceled||
            it->second->OrderStatus==THOST_FTDC_OST_ERROE)
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取已撤单和错误报单
int CPlatformDataMgr_Orders::GetCanceledOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    set<long> FTIDs;
    GetFTIDSetOfInstrument2_Internal(strInstrument,FTIDs);

    PlatformStru_OrderInfo *pOrderInfo;
    for(set<long>::const_iterator it=FTIDs.begin();it!=FTIDs.end();it++)
    {
        if(m_OrderData.GetValue2_Internal(pOrderInfo,*it)&&
           IsOrderCanceled(*pOrderInfo))
        {
            outData.push_back(*pOrderInfo);
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取指定的已撤单和错误报单
bool CPlatformDataMgr_Orders::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        (   it->second->OrderStatus==THOST_FTDC_OST_Canceled||
            it->second->OrderStatus==THOST_FTDC_OST_ERROE))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}


///获取未成交报单, 不包含手工审核中的报单
int CPlatformDataMgr_Orders::GetWaitOrders(vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if(IsOrderWaiting(*(it->second)))
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取未成交报单, 不包含手工审核中的报单
int CPlatformDataMgr_Orders::GetWaitOrders3(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    set<long>* pFTIDSet=NULL;
    set<long>::iterator it;
    PlatformStru_OrderInfo *pOrderInfo;
    long FTID;

    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
    {
        for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
        {
            FTID=*it;
            if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
               IsOrderWaiting(*pOrderInfo))
            {
                outData.push_back(*pOrderInfo);
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取指定合约相关的未成交报单, 不包含手工审核中的报单, 合约是单合约, 报单是对应报单, 或包含该合约的组合报单
int CPlatformDataMgr_Orders::GetWaitOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    set<long> FTIDs;
    GetFTIDSetOfInstrument2_Internal(strInstrument,FTIDs);

    //取keys对应的报单信息
    PlatformStru_OrderInfo *pOrderInfo;
    for(set<long>::const_iterator it=FTIDs.begin();it!=FTIDs.end();it++)
    {
        if(m_OrderData.GetValue2_Internal(pOrderInfo,*it)&&
           IsOrderWaiting(*pOrderInfo))
        {
            outData.push_back(*pOrderInfo);
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///获取指定的未成交报单, 不包含手工审核中的报单
bool CPlatformDataMgr_Orders::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        IsOrderWaiting(*(it->second)))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

///获取状态为未知的报单, 如手工审核中报单
int CPlatformDataMgr_Orders::GetUnknownStatusOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if( it->second->OrderStatus==THOST_FTDC_OST_Unknown)
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

//获取指定合约、指定方向、指定开平的挂单数量（手数）, 不包含手工审核中的报单
int  CPlatformDataMgr_Orders::GetWaitOrderVolume(const string& strAccount, const string &strInstrumentID, char Direction, char CloseType, char HedgeFlag)
{
	int tnVolume = 0;
    map<long,  PlatformStru_OrderInfo*>::iterator iter;
    set<long>::iterator itFTID;
    map<string,set<long>>::iterator it_Index2;

    if(strInstrumentID.empty())
        return 0;

    Lock_CPlatformDataMgr;

    it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            iter=m_OrderData.m_all2.find(*itFTID);
            if(iter!=m_OrderData.m_all2.end()&&
                iter->second!=NULL&&
                iter->second->Direction == Direction &&
                iter->second->CombOffsetFlag[0] == CloseType&&
                iter->second->CombHedgeFlag[0]==HedgeFlag&&
                IsOrderWaiting(*(iter->second))&&
			     strAccount==string(iter->second->Account))
                tnVolume += iter->second->VolumeTotal;
        }
    }

    Unlock_CPlatformDataMgr;

	return tnVolume;
}




//获取指定合约指定方向的平仓挂单的平仓量, 返回0表示成功, <0表示失败
//成功时CloseVolume返回平仓单(含强平和本地强平)的手数, CloseTodayVolume返回平今单手数, CloseYesterdayVolume表示平昨单手数
int  CPlatformDataMgr_Orders::GetCloseOrderVolume(const string& strAccount,const string &strInstrumentID, char Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,char HedgeFlag)
{
    int ret;

    Lock_CPlatformDataMgr;

    ret=GetCloseOrderVolume_Internal(strAccount,strInstrumentID,Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);

    Unlock_CPlatformDataMgr;

	return ret;
}

//获取指定合约指定方向的平仓挂单的平仓量, 返回0表示成功, <0表示失败. 内部调用, 不加锁
//成功时CloseVolume返回平仓单(含强平和本地强平)的手数, CloseTodayVolume返回平今单手数, CloseYesterdayVolume表示平昨单手数
int  CPlatformDataMgr_Orders::GetCloseOrderVolume_Internal(const string& strAccount,const string &strInstrumentID, char Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    CloseVolume=CloseTodayVolume=CloseYesterdayVolume=0;

    map<string,set<long>>::iterator it_Index2;
    set<long>::iterator itFTID;
    map<long,  PlatformStru_OrderInfo*>::iterator iter;

    if(strInstrumentID.empty()) return -1;

    it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            iter=m_OrderData.m_all2.find(*itFTID);
            if(iter!=m_OrderData.m_all2.end()&&
                iter->second!=NULL&&
                iter->second->Direction == Direction &&
                iter->second->CombHedgeFlag[0]==HedgeFlag&&
                IsOrderWorking(*(iter->second))&&
			    strAccount==string(iter->second->Account))
            {
                if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    CloseVolume += iter->second->VolumeTotal;
                if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    CloseTodayVolume += iter->second->VolumeTotal;
                if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    CloseYesterdayVolume += iter->second->VolumeTotal;
            }
        }
    }


    //下面从组合合约报单中找单腿(第一腿)平仓量
    PlatformStru_OrderInfo* pOrderInfo;
    multimap<string,long>::const_iterator it_lower=m_mapLegID12CombOrderFTID.lower_bound(strInstrumentID);
    multimap<string,long>::const_iterator it_upper=m_mapLegID12CombOrderFTID.upper_bound(strInstrumentID);
    while(it_lower!=it_upper)
    {
        if(m_OrderData.GetValue2_Internal(pOrderInfo,it_lower->second))
        {
            if( pOrderInfo->Direction == Direction &&
                pOrderInfo->CombHedgeFlag[0]==HedgeFlag&&
                IsOrderWorking(*pOrderInfo)&&
			    strAccount==string(pOrderInfo->Account))
            {
                if( pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    CloseVolume += pOrderInfo->VolumeTotal;
                if(pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    CloseTodayVolume += pOrderInfo->VolumeTotal;
                if(pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    CloseYesterdayVolume += pOrderInfo->VolumeTotal;
            }
        }
        it_lower++;
    }

    //下面从组合合约报单中找单腿(第二腿)平仓量
    it_lower=m_mapLegID22CombOrderFTID.lower_bound(strInstrumentID);
    it_upper=m_mapLegID22CombOrderFTID.upper_bound(strInstrumentID);
    char Direction2 = Direction == THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;
    while(it_lower!=it_upper)
    {
        if(m_OrderData.GetValue2_Internal(pOrderInfo,it_lower->second))
        {
            if( pOrderInfo->Direction == Direction2 &&
                pOrderInfo->CombHedgeFlag[0]==HedgeFlag&&
                IsOrderWorking(*pOrderInfo)&&
			    strAccount==string(pOrderInfo->Account))
            {
                if( pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    CloseVolume += pOrderInfo->VolumeTotal;
                if(pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    CloseTodayVolume += pOrderInfo->VolumeTotal;
                if(pOrderInfo->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    CloseYesterdayVolume += pOrderInfo->VolumeTotal;
            }
        }
        it_lower++;
    }

	return 0;
}


//将所有开仓挂单的冻结保证金按[账号-合约-投保-方向]分类统计
void CPlatformDataMgr_Orders::ClassifyAllMarginByInstrumentID_Internal(void)
{
    m_mapInstrumentMargin.clear();
    m_mapInstrumentExchMargin.clear();
    m_bmapInstrumentMarginOK=true;

    //先计算按合约及方向分类的保证金汇总
    map<Stru_OrderCategory,double>::iterator itMargin;
    map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second!=NULL&&it->second->CombOffsetFlag[0]==THOST_FTDC_OF_Open &&IsOrderWorking(*(it->second)))
        {
            if(util::isInvalidValue(it->second->freezeMargin))
            {
                m_bmapInstrumentMarginOK=false;
                break;
            }

            Stru_OrderCategory category(it->second->Account,it->second->InstrumentID,it->second->CombHedgeFlag[0],it->second->Direction);
            itMargin=m_mapInstrumentMargin.find(category);
            if(itMargin==m_mapInstrumentMargin.end()) m_mapInstrumentMargin[category]=it->second->freezeMargin;
            else itMargin->second+=it->second->freezeMargin;
            itMargin=m_mapInstrumentExchMargin.find(category);
            if(itMargin==m_mapInstrumentExchMargin.end()) m_mapInstrumentExchMargin[category]=it->second->freezeExchMargin;
            else itMargin->second+=it->second->freezeExchMargin;
        }
    }
}
//根据[账号-合约-投保-方向]的保证金分类数据, 计算总冻结保证金
void CPlatformDataMgr_Orders::CalcuTotalMargin_Internal(void)
{
    m_TotalMargin=0;

    if(!m_bmapInstrumentMarginOK) 
    {
        m_TotalMargin=util::GetDoubleInvalidValue();
        return;
    }

    string strInstrument,strProduct;
    map<Stru_OrderCategory,double>::iterator itInstrumentMargin,itProductMargin,itProductMargin2;
    map<Stru_OrderCategory,double>::iterator itInstrumentExchMargin,itProductExchMargin,itProductExchMargin2;

    //进行保证金汇总. 如果合约对应上期大额单边算法, 则放到mapProduct里去
    map<Stru_OrderCategory,double> ProductMargin;
    map<Stru_OrderCategory,double> ProductExchMargin;
    PlatformStru_InstrumentInfo InstrumentInfo;
    for(itInstrumentMargin=m_mapInstrumentMargin.begin();itInstrumentMargin!=m_mapInstrumentMargin.end();itInstrumentMargin++)
    {
        if(!m_RefInstruments.GetInstrumentInfo(itInstrumentMargin->first.InstrumentOrProduct,InstrumentInfo))
        {
            m_TotalMargin=util::GetDoubleInvalidValue();
            break;
        }

        if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption) 
        {
            //期权不考虑大额单边保证金优惠
            m_TotalMargin+=itInstrumentMargin->second;
        }
        else
        {
            if(InstrumentInfo.CombMarginDiscountMode!=4) 
                m_TotalMargin+=itInstrumentMargin->second;
            else
            {
                //对于上期的大额单边保证金优惠，按品种及方向统计保证金，放在ProductMargin/ProductExchMargin里
                strProduct=string(InstrumentInfo.ProductID);
                Stru_OrderCategory ProductCategory(itInstrumentMargin->first.Account,strProduct,itInstrumentMargin->first.HedgeFlag,itInstrumentMargin->first.Direction);
                itProductMargin=ProductMargin.find(ProductCategory);
                if(itProductMargin==ProductMargin.end()) ProductMargin[ProductCategory]=itInstrumentMargin->second;
                else itProductMargin->second+=itInstrumentMargin->second;

                itInstrumentExchMargin=m_mapInstrumentExchMargin.find(itInstrumentMargin->first);
                if(itInstrumentExchMargin!=m_mapInstrumentExchMargin.end())
                {
                    itProductExchMargin=ProductExchMargin.find(ProductCategory);
                    if(itProductExchMargin==ProductExchMargin.end()) ProductExchMargin[ProductCategory]=itInstrumentExchMargin->second;
                    else itProductExchMargin->second+=itInstrumentExchMargin->second;
                }
            }
        }
    }

    //计算上期大额单边保证金
    if(!util::isInvalidValue(m_TotalMargin))
    {
        for(itProductMargin=ProductMargin.begin();itProductMargin!=ProductMargin.end();itProductMargin++)
        {
            if(itProductMargin->second==0) continue;

            Stru_OrderCategory category(itProductMargin->first.Account,itProductMargin->first.InstrumentOrProduct,itProductMargin->first.HedgeFlag,itProductMargin->first.Direction==THOST_FTDC_D_Buy?THOST_FTDC_D_Sell:THOST_FTDC_D_Buy);
            itProductMargin2=ProductMargin.find(category);
            if(itProductMargin2==ProductMargin.end())
            {
                //只有单边保证金
                m_TotalMargin+=itProductMargin->second;
                itProductMargin->second=0;
                continue;
            }
            else
            {
                //有双边保证金, 找大边(按交易所保证金)
                itProductExchMargin=ProductExchMargin.find(itProductMargin->first);
                itProductExchMargin2=ProductExchMargin.find(category);

                if(itProductExchMargin!=ProductExchMargin.end()&&itProductExchMargin2!=ProductExchMargin.end()&&
                    !util::isInvalidValue(itProductExchMargin->second)&&!util::isInvalidValue(itProductExchMargin2->second)&&
                    itProductExchMargin->second!=itProductExchMargin2->second)
                {
                    if(itProductExchMargin->second>itProductExchMargin2->second) m_TotalMargin+=itProductMargin->second;
                    else if(itProductExchMargin->second<itProductExchMargin2->second) m_TotalMargin+=itProductMargin2->second;
                }
                else
                {
                    if(itProductMargin->second>=itProductMargin2->second) m_TotalMargin+=itProductMargin->second;
                    else m_TotalMargin+=itProductMargin2->second;
                }

                itProductMargin2->second=0;
                itProductMargin->second=0;
                continue;
            }
        }
    }
}
//更新全部冻结保证金
void CPlatformDataMgr_Orders::UpdateAllMargin_Internal(void)
{
    //将所有冻结保证金按合约及方向分类统计
    ClassifyAllMarginByInstrumentID_Internal();
    //根据保证金分类数据, 计算总冻结保证金
    CalcuTotalMargin_Internal();
}
//更新全部冻结手续费
void CPlatformDataMgr_Orders::UpdateAllCommission_Internal(void)
{
    m_TotalCommission=0;
    map<OrderKey,  PlatformStru_OrderInfo*>::iterator iter;
    for(iter = m_OrderData.m_all.begin();iter != m_OrderData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&IsOrderWorking(*(iter->second)))
        {
            if(!util::isInvalidValue(iter->second->troubleMoney))
                m_TotalCommission+=iter->second->troubleMoney;
            else 
            {
                m_TotalCommission=util::GetDoubleInvalidValue();
                break;
            }
        }
    }
}

//根据报单变化, 调整保证金
void CPlatformDataMgr_Orders::UpdateMarginByOrder_Internal(const PlatformStru_OrderInfo& Order,const PlatformStru_OrderInfo& OldOrder)
{
    if(!m_bmapInstrumentMarginOK) return;

    double DeltaValue;
    Stru_OrderCategory category;

    if(!Order.IsInvalidRecord())
        category.set(Order.Account,Order.InstrumentID,Order.CombHedgeFlag[0],Order.Direction);
    else if(!OldOrder.IsInvalidRecord())
        category.set(OldOrder.Account,OldOrder.InstrumentID,OldOrder.CombHedgeFlag[0],OldOrder.Direction);
    else return;

    DeltaValue=0;
    if(!Order.IsInvalidRecord()&&util::isInvalidValue(Order.freezeMargin)||!OldOrder.IsInvalidRecord()&&util::isInvalidValue(OldOrder.freezeMargin))
        DeltaValue=util::GetDoubleInvalidValue();
    else
    {
        if(!Order.IsInvalidRecord()) DeltaValue+=Order.freezeMargin;
        if(!OldOrder.IsInvalidRecord()) DeltaValue-=OldOrder.freezeMargin;
    }
    map<Stru_OrderCategory,double>::iterator itMargin=m_mapInstrumentMargin.find(category);
    if(itMargin==m_mapInstrumentMargin.end()) m_mapInstrumentMargin[category]=DeltaValue;
    else itMargin->second+=DeltaValue;

    DeltaValue=0;
    if(!Order.IsInvalidRecord()&&util::isInvalidValue(Order.freezeExchMargin)||!OldOrder.IsInvalidRecord()&&util::isInvalidValue(OldOrder.freezeExchMargin))
        DeltaValue=util::GetDoubleInvalidValue();
    else
    {
        if(!Order.IsInvalidRecord()) DeltaValue+=Order.freezeExchMargin;
        if(!OldOrder.IsInvalidRecord()) DeltaValue-=OldOrder.freezeExchMargin;
    }
    itMargin=m_mapInstrumentExchMargin.find(category);
    if(itMargin==m_mapInstrumentExchMargin.end()) m_mapInstrumentExchMargin[category]=DeltaValue;
    else if(!util::isInvalidValue(itMargin->second))
    {
        if(util::isInvalidValue(DeltaValue)) itMargin->second=util::GetDoubleInvalidValue();
        else itMargin->second+=DeltaValue;
    }


    //根据保证金分类数据, 计算总冻结保证金
    CalcuTotalMargin_Internal();
}
//根据报单变化, 调整手续费
void CPlatformDataMgr_Orders::UpdateCommissionByOrder_Internal(const PlatformStru_OrderInfo& Order,const PlatformStru_OrderInfo& OldOrder)
{
    if(util::isInvalidValue(m_TotalCommission)) return;

    if(Order.IsInvalidRecord()&&OldOrder.IsInvalidRecord())
        return;

    if(!Order.IsInvalidRecord()&&util::isInvalidValue(Order.troubleMoney)||!OldOrder.IsInvalidRecord()&&util::isInvalidValue(OldOrder.troubleMoney))
        m_TotalCommission=util::GetDoubleInvalidValue();
    else
    {
        if(!Order.IsInvalidRecord()) m_TotalCommission+=Order.troubleMoney;
        if(!OldOrder.IsInvalidRecord()) m_TotalCommission-=OldOrder.troubleMoney;
    }
}


//取全部挂单的冻结手续费和冻结保证金. 如果没有费率, 为无效值. 含状态未知报单. 考虑上期大额单边保证金优惠算法
void CPlatformDataMgr_Orders::GetFreezeMargin_Commission(double& FreezeMargin,double& FreezeCommission)
{
    Lock_CPlatformDataMgr;

    FreezeMargin=m_TotalMargin;
    FreezeCommission=m_TotalCommission;

    Unlock_CPlatformDataMgr;
}

//获取指定合约指定方向的的冻结金额, 包括同方向的开仓单和反方向的平仓单. 含状态未知报单. (考虑组合单对单腿合约的影响)
//strInstrumentID为单腿合约
//不考虑上期大额单边保证金优惠
void CPlatformDataMgr_Orders::GetFreezeMargin_Commission2(const string& strAccount,const string &strInstrumentID, char Direction,char HedgeFlag,
                                                          double& FreezeMargin,double& FreezeCommission)
{
    FreezeMargin=0;
    FreezeCommission=0;
	char CloseDirection = Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;

    Lock_CPlatformDataMgr;

    //先计算对应单合约报单的冻结保证金和冻结手续费
    map<string,set<long>>::iterator it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            PlatformStru_OrderInfo* pOrder;
            if(m_OrderData.GetValue2_Internal(pOrder,*itFTID)&&
                pOrder->CombHedgeFlag[0]==HedgeFlag&&
                IsOrderWorking(*pOrder)&&
			    strAccount==string(pOrder->Account))
            {
		        if(pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_Open&&pOrder->Direction==Direction||
		            (pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_Close||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_ForceClose||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_CloseYesterday||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_LocalForceClose)&&pOrder->Direction==CloseDirection)
                {
                    if(!util::isInvalidValue(FreezeMargin))
                    {
                        if(!util::isInvalidValue(pOrder->freezeMargin)) FreezeMargin+=pOrder->freezeMargin;
                        else FreezeMargin=util::GetDoubleInvalidValue();
                    }

                    if(!util::isInvalidValue(FreezeCommission))
                    {
                        if(!util::isInvalidValue(pOrder->troubleMoney)) FreezeCommission+=pOrder->troubleMoney;
                        else FreezeCommission=util::GetDoubleInvalidValue();
                    }
                }
            }
        }
    }
    
    //计算相关组合合约报单的冻结保证金和冻结手续费
    for(int i=0;i<2;i++)
    {
        char LegDirection = i==0 ? Direction : CloseDirection;
        char LegCloseDirection = i==0 ? CloseDirection : Direction;
        multimap<string,long>* pmapLegID2CombOrderFTID = i==0 ? &m_mapLegID12CombOrderFTID : &m_mapLegID22CombOrderFTID;
        multimap<string,long>::const_iterator it_lower=pmapLegID2CombOrderFTID->lower_bound(strInstrumentID);
        multimap<string,long>::const_iterator it_upper=pmapLegID2CombOrderFTID->upper_bound(strInstrumentID);
        while(it_lower!=it_upper)
        {
            PlatformStru_OrderInfo* pOrder;
            if(m_OrderData.GetValue2_Internal(pOrder,it_lower->second)&&
                pOrder->CombHedgeFlag[0]==HedgeFlag&&
                IsOrderWorking(*pOrder)&&
			    strAccount==string(pOrder->Account))
            {
		        if(pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_Open&&pOrder->Direction==LegDirection||
		            (pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_Close||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_ForceClose||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_CloseYesterday||
			         pOrder->CombOffsetFlag[0]==THOST_FTDC_OF_LocalForceClose)&&pOrder->Direction==LegCloseDirection)
                {
                    if(!util::isInvalidValue(FreezeMargin))
                    {
                        double LegFreezeMargin = i==0 ? pOrder->Leg1FreezeMargin : pOrder->Leg2FreezeMargin;
                        if(!util::isInvalidValue(LegFreezeMargin)) FreezeMargin+=LegFreezeMargin;
                        else FreezeMargin=util::GetDoubleInvalidValue();
                    }

                    if(!util::isInvalidValue(FreezeCommission))
                    {
                        double LegFreezeTroubleMoney = i==0 ? pOrder->Leg1TroubleMoney : pOrder->Leg2TroubleMoney;
                        if(!util::isInvalidValue(LegFreezeTroubleMoney)) FreezeCommission+=LegFreezeTroubleMoney;
                        else FreezeCommission=util::GetDoubleInvalidValue();
                    }
                }
            }
            it_lower++;
        }
    }

    Unlock_CPlatformDataMgr;
}



//清空查询报单的map
void CPlatformDataMgr_Orders::ClearMapQryRltOrders()
{
    Lock_CPlatformDataMgr;

    m_OrderData.WillQry_Internal(string(""));

    Unlock_CPlatformDataMgr;

}

int CPlatformDataMgr_Orders::GetAllOrderFTIDList(vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	set<long>::iterator it_FTID = m_OrderData.m_allFTID.begin();
	for ( ; it_FTID != m_OrderData.m_allFTID.end(); ++it_FTID )
	{
		vec.push_back(*it_FTID);
	}
	Unlock_CPlatformDataMgr;

	return vec.size();
}

int CPlatformDataMgr_Orders::GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)
{
	Lock_CPlatformDataMgr;
    bool brlt=m_OrderData.GetValue3_Internal(outData,lFTID);
	Unlock_CPlatformDataMgr;

    return brlt?0:-1;
}

int CPlatformDataMgr_Orders::GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)
{
	Lock_CPlatformDataMgr;
    PlatformStru_OrderInfo* pOrderInfo;
    bool brlt=m_OrderData.GetValue_Internal(pOrderInfo,key);
    if(brlt) outData=*pOrderInfo;
	Unlock_CPlatformDataMgr;

    return brlt?0:-1;
}

//取挂单的合约集
void CPlatformDataMgr_Orders::GetInstrumentIDs_Working(set<string>& setInstrumentID)
{
	Lock_CPlatformDataMgr;

    setInstrumentID.clear();
    string strInstrumentID;
    map<OrderKey,  PlatformStru_OrderInfo*>::iterator it_all;
    for(it_all=m_OrderData.m_all.begin();it_all!=m_OrderData.m_all.end();it_all++)
    {
        if(it_all->second==NULL) continue;

        if(IsOrderWorking(*(it_all->second)))
        {
            strInstrumentID=string(it_all->second->InstrumentID);
            if(setInstrumentID.find(strInstrumentID)==setInstrumentID.end())
                setInstrumentID.insert(strInstrumentID);
        }
    }

    Unlock_CPlatformDataMgr;
}
//取等待行情进行冻结保证金和手续费计算的合约集
void CPlatformDataMgr_Orders::GetInstrumentIDs_WaitQuot(set<string>& setInstrumentID)
{
	Lock_CPlatformDataMgr;

    setInstrumentID.clear();
    set<string> setProcessAlready;
    string strInstrumentID,Leg1InstrumentID,Leg2InstrumentID;
    PlatformStru_InstrumentInfo InstrumentInfo;
    PlatformStru_DepthMarketData    Leg1QuotData,Leg2QuotData,QuotData;
    map<OrderKey,  PlatformStru_OrderInfo*>::iterator it_all;
    for(it_all=m_OrderData.m_all.begin();it_all!=m_OrderData.m_all.end();it_all++)
    {
        if(it_all->second==NULL) continue;
        if(!IsOrderWorking(*(it_all->second))) continue;
        if(!util::isInvalidValue(it_all->second->freezeMargin)&&!util::isInvalidValue(it_all->second->troubleMoney)) continue;

        strInstrumentID=string(it_all->second->InstrumentID);
        if(setProcessAlready.find(strInstrumentID)!=setProcessAlready.end()) continue;
        setProcessAlready.insert(strInstrumentID);

        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) continue;
        if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination&&
            m_RefInstruments.GetLegInstrumentID(strInstrumentID,Leg1InstrumentID,Leg2InstrumentID))
        {
            if(!m_RefQuots.GetQuotInfo(Leg1InstrumentID,Leg1QuotData)&&setInstrumentID.find(Leg1InstrumentID)==setInstrumentID.end()) 
                setInstrumentID.insert(Leg1InstrumentID);
            if(!m_RefQuots.GetQuotInfo(Leg2InstrumentID,Leg1QuotData)&&setInstrumentID.find(Leg2InstrumentID)==setInstrumentID.end()) 
                setInstrumentID.insert(Leg2InstrumentID);
        }
        else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
        {
            if(!m_RefQuots.GetQuotInfo(strInstrumentID,QuotData)&&setInstrumentID.find(strInstrumentID)==setInstrumentID.end()) 
                setInstrumentID.insert(strInstrumentID);
        }
    }

    Unlock_CPlatformDataMgr;
}

//判断报单是否在工作状态，含挂单状态和审核中(未知状态)
bool CPlatformDataMgr_Orders::IsOrderWorking(const PlatformStru_OrderInfo& OrderInfo)
{
	if( OrderInfo.OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||                   //部分成交还在队列中 '1'
	    OrderInfo.OrderStatus == THOST_FTDC_OST_NoTradeQueueing ||	                    //未成交还在队列中 '3'
        OrderInfo.OrderStatus == THOST_FTDC_OST_Unknown&&m_bFreezeOrderUnknownStatus)   //未知状态. 一般是刚发到ctp, 还没返回, 或服务器端表示手工审核中的报单
        return true;
    return false;
}
//判断报单是否在挂单状态
bool CPlatformDataMgr_Orders::IsOrderWaiting(const PlatformStru_OrderInfo& OrderInfo)
{
	if( OrderInfo.OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||                   //部分成交还在队列中 '1'
	    OrderInfo.OrderStatus == THOST_FTDC_OST_NoTradeQueueing)	                    //未成交还在队列中 '3'
        return true;
    return false;
}
//判断报单是否在触发状态
bool CPlatformDataMgr_Orders::IsOrderTrigger(const PlatformStru_OrderInfo& OrderInfo)
{
    if( OrderInfo.ContingentCondition == THOST_FTDC_CC_Immediately ||
	    OrderInfo.ContingentCondition == THOST_FTDC_CC_Touch ||
		OrderInfo.ContingentCondition == THOST_FTDC_CC_TouchProfit)
        return true;
    return false;
}
//判断报单是否已成交，含部分成交和全部成交
bool CPlatformDataMgr_Orders::IsOrderTraded(const PlatformStru_OrderInfo& OrderInfo)
{
	if( OrderInfo.OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
	    OrderInfo.OrderStatus == THOST_FTDC_OST_AllTraded)
        return true;
    return false;
}
//判断报单是否是撤单/错单
bool CPlatformDataMgr_Orders::IsOrderCanceled(const PlatformStru_OrderInfo& OrderInfo)
{
	if( OrderInfo.OrderStatus == THOST_FTDC_OST_Canceled ||
	    OrderInfo.OrderStatus == THOST_FTDC_OST_ERROE)
        return true;
    return false;
}
  
void CPlatformDataMgr_Orders::clear(void)
{
	Lock_CPlatformDataMgr;
    m_OrderData.Clear();
    m_mapOrderSysID2OrderKey.clear();
    m_mapLegID12CombOrderFTID.clear();
    m_mapLegID22CombOrderFTID.clear();
	Unlock_CPlatformDataMgr;
}

bool CPlatformDataMgr_Orders::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_OrderData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}


///获取大于等于MinFTID的全部数据，返回数据项的数量。结果放在数组中。m_bUpdateSeq2FTID为true时有效
int CPlatformDataMgr_Orders::GetAll_GE_UpdateSeq(long MinUpdateSeq,vector<PlatformStru_OrderInfo>& outDatas)
{
    CeasymutexGuard guard(m_mutex);
    return m_OrderData.GetAll_GE_UpdateSeq_Internal(MinUpdateSeq,outDatas);
}
