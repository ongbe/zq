//维护底层持仓信息

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CPlatformDataMgr_Positions::CPlatformDataMgr_Positions(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Rates& refRates,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders,
    CPlatformDataMgr_PositionDetails& refPositionDetails,
    bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
: m_RefInstruments(refInstruments),
  m_RefRates(refRates),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetails(refPositionDetails),
  m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible(bUseSettlementPriceToCalculateProfitAndMarginAsPossible),
  m_PositionData(baseFTID,baseUpdateSeq)
{
    //查询结果返回时，是否需要创建mapQryRlt. 对于持仓，需要创建，用于合并当日持仓和历史持仓
    m_PositionData.SetbNeedKey2QryRlt(true);
}

CPlatformDataMgr_Positions::~CPlatformDataMgr_Positions(void)
{
    LOG_INFO("CPlatformDataMgr_Positions::~CPlatformDataMgr_Positions(析构CPlatformDataMgr_Positions对象)");
}

//查询回报到来
void CPlatformDataMgr_Positions::QryPositionReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_Positions::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID);

    string strInstrumentID(PositionInfo.InstrumentID);
    PlatformStru_InstrumentInfo InstrumentInfo;

    Lock_CPlatformDataMgr;

    //这里只处理单腿合约的持仓。如果是组合合约，只处理bIsLast的情况
    if(ErrID==0)
    {
        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            ErrID=-1;
        else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination)
            ErrID=-1;
        else 
        {
            //设置对应的PriceTick、Currency、ExchangeID(持仓查询结果可能没有ExchangeID信息，从InstrumentInfo中取)
            PositionInfo.PriceTick=InstrumentInfo.PriceTick;
            memset(PositionInfo.Currency,0,sizeof(PositionInfo.Currency));
            strncpy(PositionInfo.Currency,InstrumentInfo.Currency,sizeof(PositionInfo.Currency)-1);
            memset(PositionInfo.ExchangeID,0,sizeof(PositionInfo.ExchangeID));
            strncpy(PositionInfo.ExchangeID,InstrumentInfo.ExchangeID,sizeof(PositionInfo.ExchangeID)-1);

            //有可能同一仓位的今日持仓和历史持仓分两条记录得到，这里将它们合并
	        PositionKey positionKey(PositionInfo);
            PlatformStru_Position *pLastPosition;
            if(m_PositionData.GetpValueInQryRlt_Internal(pLastPosition,positionKey))
            {
		        MergePositions_RspQryPosition(*pLastPosition,PositionInfo);

                //这条数据已经合并到原数据记录中了，新数据已经不再有用。将ErrID设置为-1，下面可以统一处理
                ErrID=-1;
            }
        }
    }

    m_PositionData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);

    if(bIsLast)
    {
        set<string> setInstruments;
        m_PositionData.GetInstrumentSetOfQryRlt(setInstruments);

		//动态更新持仓的持仓盈亏、浮动盈亏和盯市总盈亏
        for(set<string>::iterator it_Instruments=setInstruments.begin();it_Instruments!=setInstruments.end();it_Instruments++)
		{
            PlatformStru_InstrumentInfo InstrumentInfo;
            if(!m_RefInstruments.GetInstrumentInfo(*it_Instruments,InstrumentInfo))
                break;

            //取实时行情数据
		    PlatformStru_DepthMarketData QuotData;
			bool bQuot=m_RefQuots.GetQuotInfo(*it_Instruments,QuotData);

            map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(*it_Instruments);
            if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
            {
                for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
                {
                    PlatformStru_Position* pPosition;
                    if(m_PositionData.GetValue2_Internal(pPosition,*itFTID))
                    {
                        //计算昨仓剩余仓位
                        pPosition->YdPositionRemain=pPosition->Position-pPosition->TodayPosition;

                        //从所有报单记录中计算总平仓挂单量、平仓挂单量、平今挂单量、平昨挂单量、可平仓量、可平今量、可平昨量
                        int CloseVolume,CloseTodayVolume,CloseYesterdayVolume;
                        char CloseOrderDirection = pPosition->PosiDirection==THOST_FTDC_PD_Long?THOST_FTDC_D_Sell:THOST_FTDC_D_Buy;
                        if(m_RefOrders.GetCloseOrderVolume(string(pPosition->Account),*it_Instruments,CloseOrderDirection,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,pPosition->HedgeFlag)==0)
                            CalcuCloseValueInPositionByAllOrders_Internal(*pPosition,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);

                        //计算持仓均价和开仓均价
                        pPosition->AveragePositionPrice = pPosition->Position>0&&InstrumentInfo.VolumeMultiple ? pPosition->PositionCost/pPosition->Position/InstrumentInfo.VolumeMultiple : 0;
                        pPosition->AverageOpenPrice = pPosition->Position>0&&InstrumentInfo.VolumeMultiple ? pPosition->OpenCost/pPosition->Position/InstrumentInfo.VolumeMultiple : 0;

                        pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);

					    //动态更新持仓的持仓盈亏、浮动盈亏和盯市总盈亏
					    if(bQuot) UpdatePositionByQuot_Internal(*pPosition,InstrumentInfo,QuotData,true);
                    }
                }
			}
		}
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Positions::QryPositionReach-end");
}






//查询持仓时合并同一合约同一方向的历史持仓和今日持仓记录
void CPlatformDataMgr_Positions::MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, const PlatformStru_Position& secondPositionInfo )
{
	firstPositionInfo.Position += secondPositionInfo.Position;
	firstPositionInfo.TodayPosition += secondPositionInfo.TodayPosition;
    firstPositionInfo.YdPosition += secondPositionInfo.YdPosition;
	firstPositionInfo.OpenVolume += secondPositionInfo.OpenVolume; 
	firstPositionInfo.CloseVolume += secondPositionInfo.CloseVolume; 
	firstPositionInfo.OpenAmount += secondPositionInfo.OpenAmount;
	firstPositionInfo.CloseAmount += secondPositionInfo.CloseAmount;
	firstPositionInfo.PositionCost += secondPositionInfo.PositionCost;
	firstPositionInfo.OpenCost += secondPositionInfo.OpenCost;

	firstPositionInfo.LongFrozen += secondPositionInfo.LongFrozen;
	firstPositionInfo.ShortFrozen += secondPositionInfo.ShortFrozen;
	firstPositionInfo.LongFrozenAmount += secondPositionInfo.LongFrozenAmount;
	firstPositionInfo.ShortFrozenAmount += secondPositionInfo.ShortFrozenAmount;
	firstPositionInfo.FrozenMargin += secondPositionInfo.FrozenMargin;
	firstPositionInfo.FrozenCommission += secondPositionInfo.FrozenCommission;
	firstPositionInfo.FrozenCash += secondPositionInfo.FrozenCash;

	firstPositionInfo.Commission += secondPositionInfo.Commission;
	firstPositionInfo.PreMargin += secondPositionInfo.PreMargin;
	firstPositionInfo.UseMargin += secondPositionInfo.UseMargin;
	firstPositionInfo.ExchangeMargin += secondPositionInfo.ExchangeMargin;
    if(firstPositionInfo.MarginRateByMoney==util::GetDoubleInvalidValue()&&
        secondPositionInfo.MarginRateByMoney!=util::GetDoubleInvalidValue())
	    firstPositionInfo.MarginRateByMoney = secondPositionInfo.MarginRateByMoney; 
    if(firstPositionInfo.MarginRateByVolume==util::GetDoubleInvalidValue()&&
        secondPositionInfo.MarginRateByVolume!=util::GetDoubleInvalidValue())
	    firstPositionInfo.MarginRateByVolume = secondPositionInfo.MarginRateByVolume;

	firstPositionInfo.CashIn += secondPositionInfo.CashIn;
	
	firstPositionInfo.PositionProfit += secondPositionInfo.PositionProfit;
	firstPositionInfo.CloseProfit += secondPositionInfo.CloseProfit;
	firstPositionInfo.CloseProfitByDate += secondPositionInfo.CloseProfitByDate;
	firstPositionInfo.CloseProfitByTrade += secondPositionInfo.CloseProfitByTrade;

    if(firstPositionInfo.PreSettlementPrice==util::GetDoubleInvalidValue()&&
        secondPositionInfo.PreSettlementPrice!=util::GetDoubleInvalidValue())
	    firstPositionInfo.PreSettlementPrice = secondPositionInfo.PreSettlementPrice; 
    if(firstPositionInfo.SettlementPrice==util::GetDoubleInvalidValue()&&
        secondPositionInfo.SettlementPrice!=util::GetDoubleInvalidValue())
	    firstPositionInfo.SettlementPrice = secondPositionInfo.SettlementPrice; 

	firstPositionInfo.CombPosition += secondPositionInfo.CombPosition;
	firstPositionInfo.CombLongFrozen += secondPositionInfo.CombLongFrozen;
	firstPositionInfo.CombShortFrozen += secondPositionInfo.CombShortFrozen;
}



//在持仓数据中，计算平仓量和可平仓量, (根据所有挂单)计算全部
void CPlatformDataMgr_Positions::CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
                                                                               int CloseVolume,
                                                                               int CloseTodayVolume,
                                                                               int CloseYesterdayVolume)
{
    PositionData.CloseOrderVolume = CloseVolume;
    PositionData.CloseTodayOrderVolume = CloseTodayVolume;
    PositionData.CloseYesterdayOrderVolume = CloseYesterdayVolume;
    PositionData.CloseOrderVolumeYdPart = 0;
    PositionData.CloseOrderVolumeTdPart = 0;
    if ( PositionData.CloseOrderVolume > 0 )
	{                               
		if ( PositionData.CloseOrderVolume <= PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume )
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.CloseOrderVolume;
		}
		else
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume;
            PositionData.CloseOrderVolumeTdPart = PositionData.CloseOrderVolume-PositionData.CloseOrderVolumeYdPart;
		}
	}
    PositionData.TotalCloseOrderVolume = PositionData.CloseOrderVolume + PositionData.CloseTodayOrderVolume + PositionData.CloseYesterdayOrderVolume;
    PositionData.CanCloseVolume=PositionData.Position-PositionData.TotalCloseOrderVolume;
    PositionData.CanCloseTodayVolume=PositionData.TodayPosition-PositionData.CloseTodayOrderVolume-PositionData.CloseOrderVolumeTdPart;
    PositionData.CanCloseydVolume=PositionData.YdPositionRemain-PositionData.CloseYesterdayOrderVolume-PositionData.CloseOrderVolumeYdPart;
}

//动态更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)、盯市总盈亏、结算价、昨结算价、现价
//计算盈亏优先使用的价格顺序为：结算价 > 最新价 > 昨结算价
void CPlatformDataMgr_Positions::UpdatePositionByQuot_Internal(PlatformStru_Position& PositionInfo,
                                                               const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                               const PlatformStru_DepthMarketData& QuotData,
                                                               bool bForceCalculate) 
{
    string strInstrument(QuotData.InstrumentID);
    if(strInstrument.empty())
        return;

    if(strInstrument!=string(PositionInfo.InstrumentID))
        return;

    //计算用的价格
    double CurPrice;
    if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(QuotData.SettlementPrice)) CurPrice=QuotData.SettlementPrice;
    else if(QuotData.Volume>0) CurPrice=QuotData.LastPrice;
    else CurPrice=QuotData.PreSettlementPrice;

    //非强制计算时，如价格未更新，不进行计算
    if(!bForceCalculate&&CurPrice==PositionInfo.CurPrice)
        return;


    //用当前价格直接计算持仓盈亏、浮动盈亏、盯市总盈亏
    double PositionProfit=util::GetDoubleInvalidValue();
    double PositionProfitByTrade=util::GetDoubleInvalidValue();
    double TotalPositionProfitByDate=util::GetDoubleInvalidValue();
    if(!util::isInvalidValue(CurPrice))
    {
        PositionProfit=0;
        PositionProfitByTrade=0;
        TotalPositionProfitByDate=0;

        double CurCost=CurPrice*PositionInfo.Position*InstrumentInfo.VolumeMultiple;
        if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long)
        {
            PositionProfit=CurCost-PositionInfo.PositionCost;
            PositionProfitByTrade=CurCost-PositionInfo.OpenCost;
        }
        else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short)
        {
            PositionProfit=PositionInfo.PositionCost-CurCost;
            PositionProfitByTrade=PositionInfo.OpenCost-CurCost;
        }
        TotalPositionProfitByDate=PositionProfit+PositionInfo.CloseProfitByDate;
    }

    bool bUpdate=false;
    if(PositionInfo.PreSettlementPrice!=QuotData.PreSettlementPrice){ PositionInfo.PreSettlementPrice=QuotData.PreSettlementPrice; bUpdate=true; }
    if(PositionInfo.SettlementPrice!=QuotData.SettlementPrice){ PositionInfo.SettlementPrice=QuotData.SettlementPrice; bUpdate=true; }
    if(PositionInfo.PositionProfit!=PositionProfit){ PositionInfo.PositionProfit=PositionProfit; bUpdate=true; }
    if(PositionInfo.PositionProfitByTrade!=PositionProfitByTrade){ PositionInfo.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
    if(PositionInfo.TotalPositionProfitByDate!=TotalPositionProfitByDate){ PositionInfo.TotalPositionProfitByDate=TotalPositionProfitByDate; bUpdate=true; }
    if(PositionInfo.CurPrice!=CurPrice) { PositionInfo.CurPrice=CurPrice;  bUpdate=true;}
    if(bUpdate)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq(PositionInfo.FTID);
}




//报单回报时，更新单腿合约持仓相关的挂单的冻结手续费和冻结保证金
void CPlatformDataMgr_Positions::UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order,
                                                                     const PlatformStru_OrderInfo& OldOrderInfo,
                                                                     const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    string strInstrumentID(Order.InstrumentID);

	Lock_CPlatformDataMgr;

    //根据一个开仓报单变化数据更新持仓的冻结手续费、冻结保证金和冻结金额
    RefreshPositionFreezeByOneOpenOrder_Internal(Order,OldOrderInfo,InstrumentInfo);

    //根据一个平仓报单变化数据更新持仓的挂单量、可平量
    //持仓是单腿合约持仓，报单是组合合约或单腿合约
    RefreshPositionCloseOrderVolumeByOneOrder_Internal(Order,OldOrderInfo,InstrumentInfo);

    Unlock_CPlatformDataMgr;
}


//根据一个开仓报单变化数据更新持仓的冻结手续费、冻结保证金和冻结金额
//持仓是单腿合约，报单是单腿合约或组合合约
void CPlatformDataMgr_Positions::RefreshPositionFreezeByOneOpenOrder_Internal(const PlatformStru_OrderInfo& Order,
                                                                              const PlatformStru_OrderInfo& OldOrderInfo,
                                                                              const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    string Leg1InstrumentID,Leg2InstrumentID;
    PlatformStru_Position *pPosition;

    if(Order.CombOffsetFlag[0]!=THOST_FTDC_OF_Open) return;

    if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination)
    {
        //单腿合约报单
        PositionKey key(Order);
        if(m_PositionData.GetValue_Internal(pPosition,key))
        {
            bool bUpdate=false;
            if(!util::isInvalidValue(pPosition->FrozenMargin))
            {
                if(!util::isInvalidValue(Order.freezeMargin))
                {
                    pPosition->FrozenMargin+=Order.freezeMargin;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.freezeMargin)) pPosition->FrozenMargin-=OldOrderInfo.freezeMargin;
                        else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(!util::isInvalidValue(pPosition->FrozenCommission))
            {
                if(!util::isInvalidValue(Order.troubleMoney))
                {
                    pPosition->FrozenCommission+=Order.troubleMoney;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.troubleMoney)) pPosition->FrozenCommission-=OldOrderInfo.troubleMoney;
                        else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(bUpdate)
            {
                if(!util::isInvalidValue(pPosition->FrozenMargin)&&!util::isInvalidValue(pPosition->FrozenCommission)) pPosition->FrozenCash=pPosition->FrozenMargin+pPosition->FrozenCommission;
                else pPosition->FrozenCash=util::GetDoubleInvalidValue();
                pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);
            }
        }
    }
    else if(m_RefInstruments.GetLegInstrumentID(string(Order.InstrumentID),Leg1InstrumentID,Leg2InstrumentID))
    {
        //组合合约报单-第一腿持仓
        char LegPosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
        PositionKey key1(Order.Account, Leg1InstrumentID.c_str(), LegPosiDirection, Order.CombHedgeFlag[0]);
        if(m_PositionData.GetValue_Internal(pPosition,key1))
        {
            bool bUpdate=false;
            if(!util::isInvalidValue(pPosition->FrozenMargin))
            {
                if(!util::isInvalidValue(Order.Leg1FreezeMargin))
                {
                    pPosition->FrozenMargin+=Order.Leg1FreezeMargin;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg1FreezeMargin)) pPosition->FrozenMargin-=OldOrderInfo.Leg1FreezeMargin;
                        else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(!util::isInvalidValue(pPosition->FrozenCommission))
            {
                if(!util::isInvalidValue(Order.Leg1TroubleMoney))
                {
                    pPosition->FrozenCommission+=Order.Leg1TroubleMoney;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg1TroubleMoney)) pPosition->FrozenCommission-=OldOrderInfo.Leg1TroubleMoney;
                        else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(bUpdate)
            {
                if(!util::isInvalidValue(pPosition->FrozenMargin)&&!util::isInvalidValue(pPosition->FrozenCommission)) pPosition->FrozenCash=pPosition->FrozenMargin+pPosition->FrozenCommission;
                else pPosition->FrozenCash=util::GetDoubleInvalidValue();
                pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);
            }
        }

        //组合合约报单-第二腿持仓
        LegPosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
        PositionKey key2(Order.Account, Leg2InstrumentID.c_str(), LegPosiDirection, Order.CombHedgeFlag[0]);
        if(m_PositionData.GetValue_Internal(pPosition,key2))
        {
            bool bUpdate=false;
            if(!util::isInvalidValue(pPosition->FrozenMargin))
            {
                if(!util::isInvalidValue(Order.Leg2FreezeMargin))
                {
                    pPosition->FrozenMargin+=Order.Leg2FreezeMargin;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg2FreezeMargin)) pPosition->FrozenMargin-=OldOrderInfo.Leg2FreezeMargin;
                        else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(!util::isInvalidValue(pPosition->FrozenCommission))
            {
                if(!util::isInvalidValue(Order.Leg2TroubleMoney))
                {
                    pPosition->FrozenCommission+=Order.Leg2TroubleMoney;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg2TroubleMoney)) pPosition->FrozenCommission-=OldOrderInfo.Leg2TroubleMoney;
                        else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(bUpdate)
            {
                if(!util::isInvalidValue(pPosition->FrozenMargin)&&!util::isInvalidValue(pPosition->FrozenCommission)) pPosition->FrozenCash=pPosition->FrozenMargin+pPosition->FrozenCommission;
                else pPosition->FrozenCash=util::GetDoubleInvalidValue();
                pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);
            }
        }

    }
}

//根据一个平仓报单变化数据更新持仓的挂单量、可平量
//持仓是单腿合约持仓，报单是组合合约或单腿合约
void CPlatformDataMgr_Positions::RefreshPositionCloseOrderVolumeByOneOrder_Internal(const PlatformStru_OrderInfo& Order,
                                                                                    const PlatformStru_OrderInfo& OldOrderInfo,
                                                                                    const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    string Leg1InstrumentID,Leg2InstrumentID;
    PositionKey key;
    PlatformStru_Position *pPosition;
    char ClosePosiDirection;

    if(Order.CombOffsetFlag[0]!=THOST_FTDC_OF_Close&&
        Order.CombOffsetFlag[0]!=THOST_FTDC_OF_ForceClose&&
		Order.CombOffsetFlag[0]!=THOST_FTDC_OF_CloseToday&&
		Order.CombOffsetFlag[0]!=THOST_FTDC_OF_CloseYesterday&&
		Order.CombOffsetFlag[0]!=THOST_FTDC_OF_LocalForceClose) return;

    if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination&&
        !m_RefInstruments.GetLegInstrumentID(string(Order.InstrumentID),Leg1InstrumentID,Leg2InstrumentID))
        return;

    for(int i=0;i<2;i++)
    {
        if(i==0)
        {
		    ClosePosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
            if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination) key=PositionKey(Order.Account, Order.InstrumentID, ClosePosiDirection, Order.CombHedgeFlag[0]);
            else key=PositionKey(Order.Account, Leg1InstrumentID.c_str(), ClosePosiDirection, Order.CombHedgeFlag[0]);
        }
        else
        {
            if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination) break;
		    ClosePosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
            key=PositionKey(Order.Account, Leg2InstrumentID.c_str(), ClosePosiDirection, Order.CombHedgeFlag[0]);
        }
        if(m_PositionData.GetValue_Internal(pPosition,key))
        {
            //计算平仓挂单量、平仓挂单量、平今挂单量、平昨挂单量、可平仓量、可平今量、可平昨量
            int CloseVolume=0,CloseTodayVolume=0,CloseYesterdayVolume=0;
            int OldCloseVolume=0,OldCloseTodayVolume=0,OldCloseYesterdayVolume=0;

            if(CPlatformDataMgr_Orders::IsOrderWorking(Order))
            {
                if(Order.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    Order.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    Order.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    CloseVolume = Order.VolumeTotal;
                if(Order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    CloseTodayVolume = Order.VolumeTotal;
                if(Order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    CloseYesterdayVolume = Order.VolumeTotal;
            }
            if(!OldOrderInfo.IsInvalidRecord()&&
                CPlatformDataMgr_Orders::IsOrderWorking(OldOrderInfo))
            {
                if(OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    OldCloseVolume = OldOrderInfo.VolumeTotal;
                if(OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    OldCloseTodayVolume = OldOrderInfo.VolumeTotal;
                if(OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    OldCloseYesterdayVolume = OldOrderInfo.VolumeTotal;

                CloseVolume-=OldCloseVolume;
                CloseTodayVolume-=OldCloseTodayVolume;
                CloseYesterdayVolume-=CloseYesterdayVolume;
            }

            //在持仓数据中，计算平仓量和可平仓量, (根据一个挂单)进行增量计算
            //Deltaxxx是增量，一般来说是负值
            CalcuCloseValueInPositionByOneOrder_Internal(*pPosition,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
        }
    }
}

//在持仓数据中，计算平仓量和可平仓量, (根据一个挂单)进行增量计算
//Deltaxxx是增量，一般来说是负值
//此函数针对单腿合约持仓
void CPlatformDataMgr_Positions::CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
                                                                              int DeltaCloseVolume,int DeltaCloseTodayVolume,int DeltaCloseYesterdayVolume)
{
    PositionData.CloseOrderVolume += DeltaCloseVolume;
    PositionData.CloseTodayOrderVolume += DeltaCloseTodayVolume;
    PositionData.CloseYesterdayOrderVolume += DeltaCloseYesterdayVolume;

    PositionData.CloseOrderVolumeYdPart = 0;
    PositionData.CloseOrderVolumeTdPart = 0;
    if ( PositionData.CloseOrderVolume > 0 )
	{                               
		if ( PositionData.CloseOrderVolume <= PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume )
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.CloseOrderVolume;
		}
		else
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume;
            PositionData.CloseOrderVolumeTdPart = PositionData.CloseOrderVolume-PositionData.CloseOrderVolumeYdPart;
		}
	}
    PositionData.TotalCloseOrderVolume = PositionData.CloseOrderVolume + PositionData.CloseTodayOrderVolume + PositionData.CloseYesterdayOrderVolume;
    PositionData.CanCloseVolume=PositionData.Position-PositionData.TotalCloseOrderVolume;
    PositionData.CanCloseTodayVolume=PositionData.TodayPosition-PositionData.CloseTodayOrderVolume-PositionData.CloseOrderVolumeTdPart;
    PositionData.CanCloseydVolume=PositionData.YdPositionRemain-PositionData.CloseYesterdayOrderVolume-PositionData.CloseOrderVolumeYdPart;
    PositionData.UpdateSeq=m_PositionData.NewUpdateSeq(PositionData.FTID);
}



//从成交记录更新持仓。此函数从持仓明细中重新创建并计算持仓。依赖于持仓明细，必须在持仓明细更新完后再更新持仓
//持仓明细和持仓都是单腿合约
void CPlatformDataMgr_Positions::UpdatePosition_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
                                                         const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                         const PlatformStru_InstrumentInfo& InstrumentInfo)
{
	LOG_INFO("CPlatformDataMgr_Positions::UpdatePosition_RtnTrade(成交回报更新持仓):InstrumentID=%s",TradeInfo.InstrumentID);

	string strInstrument(TradeInfo.InstrumentID);
	if(strInstrument.empty())
		return;

	if(TradeInfo.Direction!=THOST_FTDC_D_Buy&&TradeInfo.Direction!=THOST_FTDC_D_Sell) 
		return;

    //确定持仓方向
    TThostFtdcPosiDirectionType PosiDirection;

	if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
	{
        PosiDirection = TradeInfo.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	}
	else if( TradeInfo.OffsetFlag==THOST_FTDC_OF_Close||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_ForceClose||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
	{
        PosiDirection = TradeInfo.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
	}
    else return;

    //产生持仓key值
    PositionKey CurPositionKey( TradeInfo.Account,
		                        strInstrument.c_str(), 
		                        PosiDirection,
		                        TradeInfo.HedgeFlag);

	PlatformStru_Position* pPositionData;


    Lock_CPlatformDataMgr;

    //先看有没有原有持仓
    bool bExist=m_PositionData.GetValue_Internal(pPositionData,CurPositionKey);
    if(!bExist)
    {
        PlatformStru_Position NewPositionData;

        strncpy(NewPositionData.Account,TradeInfo.Account,sizeof(NewPositionData.Account)-1);
	    strncpy(NewPositionData.InstrumentID,TradeInfo.InstrumentID,sizeof(NewPositionData.InstrumentID)-1);
	    strncpy(NewPositionData.BrokerID,TradeInfo.BrokerID,sizeof(NewPositionData.BrokerID)-1);
	    strncpy(NewPositionData.InvestorID,TradeInfo.InvestorID,sizeof(NewPositionData.InvestorID)-1);
	    NewPositionData.PosiDirection=PosiDirection;
	    NewPositionData.HedgeFlag=TradeInfo.HedgeFlag;
	    NewPositionData.PositionDate=THOST_FTDC_PSD_Today;
        strncpy(NewPositionData.TradingDay,m_RefInstruments.GetTradingDay().c_str(),sizeof(NewPositionData.TradingDay)-1);
	    NewPositionData.SettlementID=TradeInfo.SettlementID;

	    NewPositionData.Position=0;
	    NewPositionData.TodayPosition=0;
	    NewPositionData.YdPosition=0;
        NewPositionData.YdPositionRemain=0;

	    NewPositionData.OpenVolume=0;
	    NewPositionData.CloseVolume=0;
	    NewPositionData.OpenAmount=0;
	    NewPositionData.CloseAmount=0;
	    NewPositionData.PositionCost=0;
	    NewPositionData.OpenCost=0;

	    NewPositionData.LongFrozen=0;
	    NewPositionData.ShortFrozen=0;
	    NewPositionData.LongFrozenAmount=0;
	    NewPositionData.ShortFrozenAmount=0;
	    NewPositionData.FrozenMargin=0;
	    NewPositionData.FrozenCommission=0;
	    NewPositionData.FrozenCash=0;

	    NewPositionData.Commission=0;
	    NewPositionData.PreMargin=0;
	    NewPositionData.UseMargin=0;
	    NewPositionData.ExchangeMargin = 0;
	    NewPositionData.MarginRateByMoney = util::GetDoubleInvalidValue();
	    NewPositionData.MarginRateByVolume = util::GetDoubleInvalidValue();

	    NewPositionData.CashIn=0;

	    NewPositionData.PositionProfit=0;
	    NewPositionData.CloseProfit=0;
	    NewPositionData.CloseProfitByDate=0;
	    NewPositionData.CloseProfitByTrade=0;

	    NewPositionData.PreSettlementPrice=util::GetDoubleInvalidValue();
	    NewPositionData.SettlementPrice=util::GetDoubleInvalidValue();

	    NewPositionData.CombPosition=0;
	    NewPositionData.CombLongFrozen=0;
	    NewPositionData.CombShortFrozen=0;

        NewPositionData.PositionProfitByTrade=0;
	    NewPositionData.TotalPositionProfitByDate=0;

        NewPositionData.AveragePositionPrice=0;
	    NewPositionData.AverageOpenPrice=0;

        NewPositionData.TodayCloseYdVolume=0;

        NewPositionData.CurPrice=util::GetDoubleInvalidValue(); //等待从行情更新

        //设置对应的PriceTick
        NewPositionData.PriceTick=InstrumentInfo.PriceTick;
        strncpy(NewPositionData.Currency,InstrumentInfo.Currency,sizeof(NewPositionData.Currency)-1);
        strncpy(NewPositionData.ExchangeID,InstrumentInfo.ExchangeID,sizeof(NewPositionData.ExchangeID)-1);

        m_PositionData.NewReach_Internal(NewPositionData);

        if(!m_PositionData.GetValue_Internal(pPositionData,CurPositionKey))
        {
            Unlock_CPlatformDataMgr;
            return;
        }
    }

    //计算开仓量、平今量、平昨量
    int OpenVolume,CloseTodayVolume,CloseYdVolume;
	if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {
        OpenVolume=TradeInfo.Volume;
        CloseYdVolume=0;
        CloseTodayVolume=0;
    }
    else if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday)
    {
        OpenVolume=0;
        CloseYdVolume=0;
        CloseTodayVolume=TradeInfo.Volume;
    }
    else
    {
        OpenVolume=0;
        CloseYdVolume = TradeInfo.Volume>=pPositionData->YdPositionRemain ? pPositionData->YdPositionRemain : TradeInfo.Volume;
        CloseTodayVolume=TradeInfo.Volume - CloseYdVolume;
    }

    if(CloseYdVolume>pPositionData->YdPositionRemain||
        CloseYdVolume<0||
        CloseTodayVolume>pPositionData->TodayPosition||
        CloseTodayVolume<0||
        CloseYdVolume+CloseTodayVolume>pPositionData->Position)
    {
        Unlock_CPlatformDataMgr;
        return;
    }

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {   
        //开仓
        //计算持仓、今仓、昨仓、(今)开仓量、(今)平仓量
        pPositionData->Position += OpenVolume;
        pPositionData->TodayPosition += OpenVolume;
        pPositionData->OpenVolume+=OpenVolume;
        pPositionData->OpenAmount+=OpenVolume*TradeInfo.Price*InstrumentInfo.VolumeMultiple;

        //计算持仓成本和开仓成本
        pPositionData->PositionCost += OpenVolume * TradeInfo.Price * InstrumentInfo.VolumeMultiple;
        pPositionData->OpenCost += OpenVolume * TradeInfo.Price * InstrumentInfo.VolumeMultiple;
    }
    else
    {
        //平仓
        //计算持仓、今仓、昨仓、(今)开仓量、(今)平仓量
        pPositionData->Position -= CloseYdVolume+CloseTodayVolume;
        pPositionData->TodayPosition -= CloseTodayVolume;
        pPositionData->YdPositionRemain -= CloseYdVolume;
        pPositionData->CloseVolume += CloseYdVolume+CloseTodayVolume;
        pPositionData->CloseAmount += (CloseYdVolume+CloseTodayVolume)*TradeInfo.Price*InstrumentInfo.VolumeMultiple;

        //组合持仓明细被打散情况对持仓的影响：
        //1. 如果出现被打散的情况，是另一腿被打散
        //2. 成交对应的这一腿持仓明细只会出现持仓明细手数减少的情况
        //3. 另一腿可能出现新建普通持仓明细，或相应普通持仓明细手数增加的情况。但不影响其成本、盈亏，因此不用特殊处理

        //计算持仓成本、开仓成本、平仓盈亏(盯市、逐笔，不需要考虑无效值)（根据持仓明细的变化来计算）
        //int i,imax;
        int ClosedVolume;
        //imax=ModifiedPositionDetails.size();
        PlatformStru_PositionDetail detail;
        for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
        {
            if(!m_RefPositionDetails.GetPositionDetail(it->first,detail)) continue;
            if(CurPositionKey.IsSamePositionKey(detail))
            {
                ClosedVolume=it->second.Volume-detail.Volume;
                if(ClosedVolume>0)
                {
                    if(detail.bToday)
                        pPositionData->PositionCost -= ClosedVolume*detail.OpenPrice*InstrumentInfo.VolumeMultiple;
                    else 
                        pPositionData->PositionCost -= ClosedVolume*detail.LastSettlementPrice*InstrumentInfo.VolumeMultiple;
                    pPositionData->OpenCost -= ClosedVolume*detail.OpenPrice*InstrumentInfo.VolumeMultiple;
                    pPositionData->CloseProfit += detail.CloseProfitByDate - it->second.CloseProfitByDate;
                    pPositionData->CloseProfitByDate += detail.CloseProfitByDate - it->second.CloseProfitByDate;
                    pPositionData->CloseProfitByTrade += detail.CloseProfitByTrade - it->second.CloseProfitByTrade;
                }
            }
        }
    }
    //计算持仓均价、开仓均价
    if(pPositionData->Position>0&&InstrumentInfo.VolumeMultiple>0) 
	{
		pPositionData->AveragePositionPrice=pPositionData->PositionCost/pPositionData->Position/InstrumentInfo.VolumeMultiple;
		pPositionData->AverageOpenPrice=pPositionData->OpenCost/pPositionData->Position/InstrumentInfo.VolumeMultiple;
	}
    else
    {
        pPositionData->AveragePositionPrice=0;
        pPositionData->AverageOpenPrice=0;
    }

    if(!bExist)
    {
	    //如果是新建持仓，更新持仓的报单冻结项。
	    UpdatePositionFreezeFieldsByAllOrders_Internal(*pPositionData);
    }
    else
    {
        //仅需要更新可平今量等值
        CalcuCloseValueInPositionByOneOrder_Internal(*pPositionData,0,0,0);
    }

    //更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)和盯市总盈亏，结算价和昨结算价，根据行情
    PlatformStru_DepthMarketData QuotInfo;
    if(m_RefQuots.GetQuotInfo(strInstrument,QuotInfo))
        UpdatePositionByQuot_Internal(*pPositionData,InstrumentInfo,QuotInfo,true); 
    
    //更新持仓记录的占用保证金
    //这里要考虑组合单被打散的情况。另一腿(指被打散的那腿)会变成普通持仓明细，对应的持仓的保证金会增加
    UpdatePositionByMarginRate_RtnTrade_Internal(*pPositionData,ModifiedPositionDetails);

    //更新持仓记录的手续费
    if(pPositionData->Commission!=util::GetDoubleInvalidValue()&&
        TradeInfo.TradeCommission!=util::GetDoubleInvalidValue())
        pPositionData->Commission += TradeInfo.TradeCommission;
    else if(pPositionData->Commission!=util::GetDoubleInvalidValue())
        pPositionData->Commission=util::GetDoubleInvalidValue();

    pPositionData->UpdateSeq=m_PositionData.NewUpdateSeq(pPositionData->FTID);

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Positions::UpdatePosition_RtnTrade-end");
    return;
}

//更新持仓相关的挂单的冻结手续费和冻结保证金。成交新建持仓时、得到费率时调用
void CPlatformDataMgr_Positions::UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData)
{
	string strInstrumentID(PositionData.InstrumentID);
	if(strInstrumentID.empty())
		return;

	double OldFrozenMargin=PositionData.FrozenMargin;
	double OldFrozenCommission=PositionData.FrozenCommission;
	double OldFrozenCash=PositionData.FrozenCash;

    int OldTotalCloseOrderVolume=PositionData.TotalCloseOrderVolume;
    int OldCloseOrderVolume=PositionData.CloseOrderVolume;
    int OldCloseOrderVolumeTdPart=PositionData.CloseOrderVolumeTdPart;
    int OldCloseOrderVolumeYdPart=PositionData.CloseOrderVolumeYdPart;
    int OldCloseTodayOrderVolume=PositionData.CloseTodayOrderVolume;
    int OldCloseYesterdayOrderVolume=PositionData.CloseYesterdayOrderVolume;
    int OldCanCloseVolume=PositionData.CanCloseVolume;
    int OldCanCloseTodayVolume=PositionData.CanCloseTodayVolume;
    int OldCanCloseydVolume=PositionData.CanCloseydVolume;


	string strAccount(PositionData.Account);

	char Direction,CloseDirection;
	if(PositionData.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
	else if(PositionData.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
	else return;
	CloseDirection = Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;


    //暂时不确定这七项值怎么算
	PositionData.LongFrozen=0;
	PositionData.ShortFrozen=0;
	PositionData.LongFrozenAmount=0;
    PositionData.ShortFrozenAmount=0;
    PositionData.CombPosition=0;
	PositionData.CombLongFrozen=0;
	PositionData.CombShortFrozen=0;


    //下面从报单中计算冻结项
	PositionData.FrozenMargin=0;
	PositionData.FrozenCommission=0;
	PositionData.FrozenCash=0;
    m_RefOrders.GetFreezeMargin_Commission2(strAccount,strInstrumentID,Direction,PositionData.HedgeFlag,
        PositionData.FrozenMargin,PositionData.FrozenCommission);

    if(PositionData.FrozenMargin!=util::GetDoubleInvalidValue()&&
		PositionData.FrozenCommission!=util::GetDoubleInvalidValue())
	{
		PositionData.FrozenCash=PositionData.FrozenMargin+PositionData.FrozenCommission;
	}
	else
	{
		PositionData.FrozenCash=util::GetDoubleInvalidValue();
	}


    //从所有报单记录中计算总平仓挂单量、平仓挂单量、平今挂单量、平昨挂单量、可平仓量、可平今量、可平昨量
    int CloseVolume=0,CloseTodayVolume=0,CloseYesterdayVolume=0;
    if(m_RefOrders.GetCloseOrderVolume(strAccount,strInstrumentID,CloseDirection,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,PositionData.HedgeFlag)==0)
        CalcuCloseValueInPositionByAllOrders_Internal(PositionData,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);

	if( OldFrozenMargin!=PositionData.FrozenMargin||
	    OldFrozenCommission!=PositionData.FrozenCommission||
	    OldFrozenCash!=PositionData.FrozenCash||
        OldTotalCloseOrderVolume!=PositionData.TotalCloseOrderVolume||
        OldCloseOrderVolume!=PositionData.CloseOrderVolume||
        OldCloseOrderVolumeTdPart!=PositionData.CloseOrderVolumeTdPart||
        OldCloseOrderVolumeYdPart!=PositionData.CloseOrderVolumeYdPart||
        OldCloseTodayOrderVolume!=PositionData.CloseTodayOrderVolume||
        OldCloseYesterdayOrderVolume!=PositionData.CloseYesterdayOrderVolume||
        OldCanCloseVolume!=PositionData.CanCloseVolume||
        OldCanCloseTodayVolume!=PositionData.CanCloseTodayVolume||
        OldCanCloseydVolume!=PositionData.CanCloseydVolume)
    {
        PositionData.UpdateSeq=m_PositionData.NewUpdateSeq(PositionData.FTID);
    }
}


//动态更新持仓记录的保证金、保证金率，成交修改持仓时调用
//这里要考虑组合单被打散的情况。另一腿(指被打散的那腿)会变成普通持仓明细，对应的持仓的保证金会增加
void CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
                                                                              const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails) 
{
    LOG_INFO("CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RtnTrade_Internal 更新持仓占用保证金， InstrumentID=%s",PositionInfo.InstrumentID);

    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    double OldUseMargin=PositionInfo.UseMargin;
    double OldExchangeMargin=PositionInfo.ExchangeMargin;

    PositionKey CurPositionKey(PositionInfo);
    //计算保证金（根据持仓明细的变化来计算）
    PlatformStru_PositionDetail detail;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        if(!m_RefPositionDetails.GetPositionDetail(it->first,detail)) continue;

        //这里不计入组合持仓明细中的保证金
        if(detail.TradeType==THOST_FTDC_TRDT_CombinationDerived)
            continue;

        if(CurPositionKey.IsSamePositionKey(detail))
        {
			//这是自己的一腿
            if(!util::isInvalidValue(PositionInfo.UseMargin))
		    {
                if(util::isInvalidValue(detail.Margin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.Margin))
                    PositionInfo.UseMargin=util::GetDoubleInvalidValue();
                else PositionInfo.UseMargin += detail.Margin - (it->second.IsInvalidRecord() ? 0 : it->second.Margin);
		    }
            if(!util::isInvalidValue(PositionInfo.ExchangeMargin))
		    {
                if(util::isInvalidValue(detail.ExchMargin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.ExchMargin))
                    PositionInfo.ExchangeMargin=util::GetDoubleInvalidValue();
                else PositionInfo.ExchangeMargin += detail.ExchMargin - (it->second.IsInvalidRecord() ? 0 : it->second.ExchMargin);
            }
        }
        else
        {
			//这是打散后的另一腿，成为普通持仓，需找到对应的持仓，然后更新保证金
		    PositionKey OppoPositionKey(detail.Account, 
			    detail.InstrumentID,
                detail.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short,
			    detail.HedgeFlag);
            PlatformStru_Position *pOppoPosition;
		    if(m_PositionData.GetValue_Internal(pOppoPosition,OppoPositionKey))
            {
                if(!util::isInvalidValue(pOppoPosition->UseMargin))
                {
                    if(util::isInvalidValue(detail.Margin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.Margin))
                        pOppoPosition->UseMargin=util::GetDoubleInvalidValue();
                    else pOppoPosition->UseMargin += detail.Margin - (it->second.IsInvalidRecord() ? 0 : it->second.Margin);
                    pOppoPosition->UpdateSeq = m_PositionData.NewUpdateSeq(pOppoPosition->FTID);
                }
                if(!util::isInvalidValue(pOppoPosition->ExchangeMargin))
                {
                    if(util::isInvalidValue(detail.ExchMargin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.ExchMargin))
                        pOppoPosition->ExchangeMargin=util::GetDoubleInvalidValue();
                    else pOppoPosition->ExchangeMargin += detail.ExchMargin - (it->second.IsInvalidRecord() ? 0 : it->second.ExchMargin);
                    pOppoPosition->UpdateSeq = m_PositionData.NewUpdateSeq(pOppoPosition->FTID);
                }
            }
        }
    }

    if(PositionInfo.UseMargin!=OldUseMargin||PositionInfo.ExchangeMargin!=OldExchangeMargin)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq(PositionInfo.FTID);

    LOG_INFO("CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RtnTrade_Internal 更新持仓占用保证金完成， InstrumentID=%s UseMargin=%g ExchangeMargin=%g UpdateSeq=%u",PositionInfo.InstrumentID,PositionInfo.UseMargin,PositionInfo.ExchangeMargin,PositionInfo.UpdateSeq);
}


//动态更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)、盯市总盈亏、结算价、昨结算价，得到新行情时做一遍
void CPlatformDataMgr_Positions::UpdatePosition_RtnQuot(const PlatformStru_DepthMarketData& QuotData,
                                                        const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                        bool bForceCalculate) 
{
    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(string(QuotData.InstrumentID));
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionByQuot_Internal(*(it_Info2->second),InstrumentInfo,QuotData,bForceCalculate);
            }
        }
    }

    Unlock_CPlatformDataMgr;

}

//结算时更新持仓。根据结算价，更新持仓的盈亏、保证金
void CPlatformDataMgr_Positions::UpdatePosition_OnSettlement(void)
{
    Lock_CPlatformDataMgr;

    PlatformStru_DepthMarketData QuotData;
    PlatformStru_InstrumentInfo InstrumentInfo;
    PlatformStru_InstrumentMarginRate MarginRate;
    for(std::set<long>::iterator it=m_PositionData.m_allFTID.begin();it!=m_PositionData.m_allFTID.end();it++)
    {
        map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*it);
        if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
        {
            if(m_RefInstruments.GetInstrumentInfo(string(it_Info2->second->InstrumentID),InstrumentInfo)&&
                m_RefQuots.GetQuotInfo(string(it_Info2->second->InstrumentID),QuotData)&&
                m_RefRates.GetMarginRate(string(it_Info2->second->InstrumentID),MarginRate)==0)
            {
                //更新盈亏、结算价等
                UpdatePositionByQuot_Internal(*(it_Info2->second),InstrumentInfo,QuotData,true);
                //这时持仓明细中的保证金已经有了，计算持仓保证金
                UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(MarginRate,*(it_Info2->second),true);
            }
        }
    }

    Unlock_CPlatformDataMgr;
}

//动态更新持仓记录的保证金、保证金率，查询保证金率响应时调用。返回是否进行了更新
bool CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate) 
{
    bool brlt=false;
    string strInstrument(MarginRate.InstrumentID);

    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            //持仓的保证金一项为无效值才进行更新
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL&&
                util::isInvalidValue(it_Info2->second->UseMargin))
            {
                UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(MarginRate,*(it_Info2->second),false);
                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//动态更新持仓记录的保证金、保证金率，查询保证金率响应和结算时调用
void CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                                             PlatformStru_Position& PositionInfo,
                                                                                             bool bForce)
{
    PlatformStru_InstrumentInfo InstrumentInfo;

    //已经算过了，不再算
    if(!bForce&&PositionInfo.UseMargin!=util::GetDoubleInvalidValue())
    {
        LOG_INFO("CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal 不用再算， InstrumentID=%s，bForce=%d, UseMargin=%g, ExchangeMargin=%g",PositionInfo.InstrumentID,bForce,PositionInfo.UseMargin,PositionInfo.ExchangeMargin);
        return;
    }

    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    string strInstrument(PositionInfo.InstrumentID);

    //必须有相应的合约信息
    if(!m_RefInstruments.GetInstrumentInfo(strInstrument,InstrumentInfo))
        return;

    double MarginRateByMoney=PositionInfo.MarginRateByMoney;
    double MarginRateByVolume=PositionInfo.MarginRateByVolume;

    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long)
    {
        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else
    {
        MarginRateByMoney=MarginRate.ShortMarginRatioByMoney;
        MarginRateByVolume=MarginRate.ShortMarginRatioByVolume;
    }

    //计算指定合约的保证金
    double UseMargin=0,ExchangeMargin=0;
    m_RefPositionDetails.GetMarginByInstrument(strInstrument,string(PositionInfo.Account),Direction,PositionInfo.HedgeFlag,UseMargin,ExchangeMargin);

    bool bUpdate=false;
    if(PositionInfo.UseMargin!=UseMargin){ PositionInfo.UseMargin=UseMargin;  bUpdate=true; }
    if(PositionInfo.ExchangeMargin!=ExchangeMargin){ PositionInfo.ExchangeMargin=ExchangeMargin;  bUpdate=true; }
    if(PositionInfo.MarginRateByMoney!=MarginRateByMoney){ PositionInfo.MarginRateByMoney=MarginRateByMoney;  bUpdate=true; }
    if(PositionInfo.MarginRateByVolume!=MarginRateByVolume){ PositionInfo.MarginRateByVolume=MarginRateByVolume;  bUpdate=true; }
    if(bUpdate)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq(PositionInfo.FTID);
}

//动态更新持仓记录的手续费，成交修改持仓和查询手续费率响应时调用
bool CPlatformDataMgr_Positions::UpdatePositionByCommissionRate_RspQryRate(const PlatformStru_InstrumentCommissionRate& CommissionRate) 
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    string strInstrumentID(CommissionRate.InstrumentID);
    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                char Direction;
                if(it_Info2->second->PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
                else if(it_Info2->second->PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;

                //从成交记录中计算手续费
                double Commission=m_RefTrades.GetCommission(strInstrumentID,string(it_Info2->second->Account),it_Info2->second->HedgeFlag,Direction);
                if(it_Info2->second->Commission!=Commission)
                { 
                    it_Info2->second->Commission=Commission;  
                    it_Info2->second->UpdateSeq=m_PositionData.NewUpdateSeq(it_Info2->second->FTID);
                }

                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//费率查询响应时，更新持仓相关的挂单的冻结手续费和冻结保证金
void CPlatformDataMgr_Positions::UpdatePositionFreezeFieldsByOrders_RspQryRate(const string& strInstrument)
{
    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2==m_PositionData.m_all2.end()||it_Info2->second==NULL) continue;

		    if(it_Info2->second->FrozenMargin==util::GetDoubleInvalidValue()||
			    it_Info2->second->FrozenCommission==util::GetDoubleInvalidValue()||
			    it_Info2->second->FrozenCash==util::GetDoubleInvalidValue())
		    {
			    UpdatePositionFreezeFieldsByAllOrders_Internal(*(it_Info2->second));
		    }
        }
    }

    Unlock_CPlatformDataMgr;
}

//获取持仓记录键值列表,返回持仓记录的数量
int CPlatformDataMgr_Positions::GetPositionKeySet(set<PositionKey> &PositionKeySet)
{
    LOG_INFO("CPlatformDataMgr_Positions::GetPositionKeySet");

    PositionKeySet.clear();

    Lock_CPlatformDataMgr;

    map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionData.m_all.begin();
    for(;it!=m_PositionData.m_all.end();it++)
    {
        if(it->second!=NULL)
            PositionKeySet.insert(it->first);
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Positions::GetPositionKeySet-end");
    return PositionKeySet.size();
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CPlatformDataMgr_Positions::GetPositionData(const string& strAccount,const string& InstrumentID,char PosiDirection,char HedgeFlag,PlatformStru_Position& PositionData)
{
    //LOG_INFO("CPlatformDataMgr_Positions::GetPositionData");

    PositionKey tmpKey( strAccount.c_str(),
						InstrumentID.c_str(), 
                        PosiDirection,
                        HedgeFlag);

    int irlt;

    Lock_CPlatformDataMgr;

    map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionData.m_all.find(tmpKey);
    if(it!=m_PositionData.m_all.end()&&it->second!=NULL)
    {
        PositionData=*(it->second);
        irlt=0;
    }
    else irlt=-1;

    Unlock_CPlatformDataMgr;

    //LOG_INFO("CPlatformDataMgr_Positions::GetPositionData-end");
    return irlt;
}

//获取所有持仓记录，排除数量为0的记录。返回结果个数
int CPlatformDataMgr_Positions::GetPositions(vector<PlatformStru_Position>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for ( ; it != m_PositionData.m_all.end(); ++it )
	{
        if(it->second!=NULL&&it->second->Position>0)
            outData.push_back(*(it->second));
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}

//获取所有持仓记录，包含数量为0的记录，返回结果个数
int CPlatformDataMgr_Positions::GetPositions4(std::vector<PlatformStru_Position>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for ( ; it != m_PositionData.m_all.end(); ++it )
	{
        outData.push_back(*(it->second));
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}


//返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
int CPlatformDataMgr_Positions::GetPositions2(const string& strInstrument,set<long>& setFTID,vector<PlatformStru_Position>& outData,long& lastseq)
{
    int rlt;

    outData.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionData.GetLastUpdateSeq())
        rlt=1;
    else
    {
        set<long>* pFTIDSet=NULL;
        if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            setFTID=(*pFTIDSet);

            PlatformStru_Position *pData=NULL;
            for(set<long>::iterator itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&pData->Position>0)
                    outData.push_back(*pData);
                else
                    setFTID.erase(*itFTID);
            }
        }
        lastseq=m_PositionData.GetLastUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

//返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
int CPlatformDataMgr_Positions::GetPositions3(const string& strInstrument,vector<PlatformStru_Position>& outData,long& lastseq)
{
    int rlt;

    outData.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionData.GetLastUpdateSeq())
        rlt=1;
    else
    {
        set<long>* pFTIDSet;
        if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            PlatformStru_Position *pData;
            for(set<long>::iterator itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&pData->Position>0)
                    outData.push_back(*pData);
            }
        }

        lastseq=m_PositionData.GetLastUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

//清空查询持仓的map
void CPlatformDataMgr_Positions::ClearMapQryRltPositions(const string& strQryInstrument)
{
    Lock_CPlatformDataMgr;
    m_PositionData.WillQry_Internal(strQryInstrument);
    Unlock_CPlatformDataMgr;
}

//获取查询的合约ID
void CPlatformDataMgr_Positions::GetQryPositionInstrument(string& outQryInstrument)
{
    outQryInstrument.clear();

    Lock_CPlatformDataMgr;
    m_PositionData.GetQryInstrument(outQryInstrument);
    Unlock_CPlatformDataMgr;
}

//获取持仓FTID集合，返回结果个数
int CPlatformDataMgr_Positions::GetPositionFTIDList(vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	set<long>::iterator it_FTID = m_PositionData.m_allFTID.begin();
	for ( ; it_FTID != m_PositionData.m_allFTID.end(); ++it_FTID )
		vec.push_back(*it_FTID);
	Unlock_CPlatformDataMgr;

	return vec.size();
}

//获取指定FTID的持仓，成功返回0，失败返回-1
int CPlatformDataMgr_Positions::GetPositionInfo(long lFTID, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionData.m_all2.find(lFTID);
	if ( it_FTID != m_PositionData.m_all2.end())
		outData = *(it_FTID->second);
	else
		nRet = -1;
	Unlock_CPlatformDataMgr;

	return nRet;
}

//获取指定key的持仓信息，成功返回0，失败返回-1
int CPlatformDataMgr_Positions::GetPositionInfo(PositionKey key, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	map<PositionKey, PlatformStru_Position*>::iterator it_all = m_PositionData.m_all.find(key);
	if ( it_all != m_PositionData.m_all.end())
		outData = *(it_all->second);
	else
		nRet = -1;
	Unlock_CPlatformDataMgr;

	return nRet;
}

//取全部持仓的占用保证金。考虑上期大额单边保证金优惠
void CPlatformDataMgr_Positions::GetAllMargin(double& TotalMargin)
{
	TotalMargin=0;

	Lock_CPlatformDataMgr;

    //保存需要按上期单边保证金优惠计算的持仓保证金。注意，PositionKey的InstrumentID是ProductID
    map<PositionKey,double> ProductMargin;
    map<PositionKey,double> ProductExchangeMargin;
    map<PositionKey,double>::iterator itMargin,itMargin2,itMargin3,itMargin4;

    PlatformStru_InstrumentInfo InstrumentInfo;
	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for(;it != m_PositionData.m_all.end(); it++)
	{
        if(it->second!=NULL&&!util::isInvalidValue(it->second->UseMargin))
        {
            if(!m_RefInstruments.GetInstrumentInfo(string(it->first.InstrumentID),InstrumentInfo))
            {
                TotalMargin=util::GetDoubleInvalidValue();
                break;
            }

            if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption) 
            {
                //期权不考虑大额单边保证金优惠
                TotalMargin+=it->second->UseMargin;
            }
            else
            {
                if(InstrumentInfo.CombMarginDiscountMode!=4) TotalMargin+=it->second->UseMargin;
                else
                {
                    PositionKey tmpkey(it->first.Account,InstrumentInfo.ProductID,it->first.PosiDirection,it->first.HedgeFlag);
                    itMargin2=ProductMargin.find(tmpkey);
                    if(itMargin2==ProductMargin.end()) ProductMargin[tmpkey]=it->second->UseMargin;
                    else itMargin2->second+=it->second->UseMargin;

                    itMargin2=ProductExchangeMargin.find(tmpkey);
                    if(itMargin2==ProductExchangeMargin.end()) ProductExchangeMargin[tmpkey]=it->second->ExchangeMargin;
                    else itMargin2->second+=it->second->ExchangeMargin;
                }
            }
        }
        else 
        {
            TotalMargin=util::GetDoubleInvalidValue();
            break;
        }
	}

    //统计ProductMargin里的保证金。注意，里面的InstrumentID是ProductID. 对于大边优惠，需要根据交易所保证金决定大边
    if(!util::isInvalidValue(TotalMargin))
    {
        for(itMargin=ProductMargin.begin();itMargin!=ProductMargin.end();itMargin++)
        {
            if(itMargin->second==0) continue;

            PositionKey tmpkeys(itMargin->first.Account,itMargin->first.InstrumentID,itMargin->first.PosiDirection==THOST_FTDC_PD_Long?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long,itMargin->first.HedgeFlag);
            itMargin2=ProductMargin.find(tmpkeys);
            if(itMargin2==ProductMargin.end())
            {
                //只有单边保证金
                TotalMargin+=itMargin->second;
                itMargin->second=0;
                continue;
            }
            else
            {
                //有双边保证金，找大边
                itMargin3=ProductExchangeMargin.find(itMargin->first);
                itMargin4=ProductExchangeMargin.find(tmpkeys);
                if(itMargin3!=ProductExchangeMargin.end()&&itMargin4!=ProductExchangeMargin.end()&&
                    !util::isInvalidValue(itMargin3->second)&&!util::isInvalidValue(itMargin4->second)&&
                    itMargin3->second!=itMargin4->second)
                {
                    if(itMargin3->second>itMargin4->second) TotalMargin+=itMargin->second;
                    else TotalMargin+=itMargin2->second;
                }
                else
                {
                    if(itMargin->second>=itMargin2->second) TotalMargin+=itMargin->second;
                    else TotalMargin+=itMargin2->second;
                }

                itMargin2->second=0;
                itMargin->second=0;
                continue;
            }
        }
    }

	Unlock_CPlatformDataMgr;
}

//取全部持仓的平仓盈亏(逐日盯市)
void CPlatformDataMgr_Positions::GetAllCloseProfitByDate(double&TotalCloseProfitByDate)
{
    TotalCloseProfitByDate=0;

	Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for(;it != m_PositionData.m_all.end(); it++)
	{
        if(it->second!=NULL&&!util::isInvalidValue(it->second->CloseProfit))
            TotalCloseProfitByDate+=it->second->CloseProfit;
        else 
        {
            TotalCloseProfitByDate=util::GetDoubleInvalidValue();
            break;
        }
	}

	Unlock_CPlatformDataMgr;
}

//取全部持仓的持仓盈亏(逐日盯市)
void CPlatformDataMgr_Positions::GetAllPositionProfitByDate(double&TotalPositionProfitByDate)
{
	TotalPositionProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	Lock_CPlatformDataMgr;
	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        //排除组合持仓。组合持仓的盈亏在分腿组合持仓中计算
        if(GlobalFunc::IsCombInstrument2(iter->second->InstrumentID))
            continue;
        if(TotalPositionProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->PositionProfit!=InvalideValue)
                TotalPositionProfitByDate+=iter->second->PositionProfit;
            else TotalPositionProfitByDate=InvalideValue;
        }
	}
	Unlock_CPlatformDataMgr;
}


void CPlatformDataMgr_Positions::clear(void)
{
	Lock_CPlatformDataMgr;
    m_PositionData.Clear();
	Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_Positions::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_PositionData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}



