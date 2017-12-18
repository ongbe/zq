//维护底层持仓明细信息

#pragma warning(disable:4996)

#include <assert.h>
#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CPlatformDataMgr_PositionDetails::CPlatformDataMgr_PositionDetails( 
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Rates& refRates,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders,
    CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
    int DiscountCZCE_Comb,
    int DiscountDCE_Comb,
    int UnwindPrioritiesCZCE_Comb,
    int UnwindPrioritiesDCE_Comb,
    int PriceForMarginOfTodayPositionCZCE_Comb,
    int PriceForMarginOfTodayPositionDCE_Comb,
    bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
: m_RefInstruments(refInstruments),
  m_RefRates(refRates),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetailCombs(refPositionDetailCombs),
  m_DiscountCZCE_Comb(DiscountCZCE_Comb),
  m_DiscountDCE_Comb(DiscountDCE_Comb),
  m_PositionDetailData(baseFTID,baseUpdateSeq),
  m_UnwindPrioritiesCZCE_Comb(UnwindPrioritiesCZCE_Comb),
  m_UnwindPrioritiesDCE_Comb(UnwindPrioritiesDCE_Comb),
  m_PriceForMarginOfTodayPositionCZCE_Comb(PriceForMarginOfTodayPositionCZCE_Comb),
  m_PriceForMarginOfTodayPositionDCE_Comb(PriceForMarginOfTodayPositionDCE_Comb),
  m_PriceForRoyalty_Option(0),
  m_bHaveComb(false),
  m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible(bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
{
}

CPlatformDataMgr_PositionDetails::~CPlatformDataMgr_PositionDetails(void)
{
    LOG_INFO("CPlatformDataMgr_PositionDetails::~CPlatformDataMgr_PositionDetails(析构CPlatformDataMgr_PositionDetails对象)");
}


//查询回报到来
void CPlatformDataMgr_PositionDetails::QryPositionDetailReach( PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_PositionDetails::QryPositionDetailReach:InstrumentID=%s",PositionDetailInfo.InstrumentID);

    if(ErrID==0)
    {
        //设置对应的PriceTick
        string strInstrumentID=string(PositionDetailInfo.InstrumentID);
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            PositionDetailInfo.PriceTick=InstrumentInfo.PriceTick;

        //设置是否今仓
        PositionDetailInfo.bToday=m_RefInstruments.IsToday(PositionDetailInfo.OpenDate);
        //设置开仓时间, 今仓为对应的开仓成交时间, 昨仓为00:00:00
        memset(PositionDetailInfo.OpenTime,0,sizeof(PositionDetailInfo.OpenTime));
        if(!PositionDetailInfo.bToday) 
            strncpy(PositionDetailInfo.OpenTime,"00:00:00",sizeof(PositionDetailInfo.OpenTime));
        else
        {
            vector<TradeKey> TradeKeys;
            PlatformStru_TradeInfo TradeInfo;
            m_RefTrades.GetTradeKeysOfTradeID(PositionDetailInfo.TradeID,TradeKeys);
            for(int i=0;i<(int)TradeKeys.size();i++)
            {
                if(m_RefTrades.GetTradeInfo(TradeKeys[i],TradeInfo)&&strInstrumentID==string(TradeInfo.InstrumentID))
                {
                    strncpy(PositionDetailInfo.OpenTime,TradeInfo.TradeTime,sizeof(PositionDetailInfo.OpenTime)-1);
                    break;
                }
            }
        }
        PositionDetailInfo.OpenTimeUTC=(int)CTools_Ansi::Get_time_t_utc_from_strTime(PositionDetailInfo.OpenDate,PositionDetailInfo.OpenTime);
    }

    m_mutex.lock();

    //将数据送到数据集中
    m_PositionDetailData.QryRltReach_Internal(PositionDetailInfo,ErrID,bIsLast);

    if(ErrID==0)
    {
        CTools_Ansi::AddKeyValueToMultimap<int,PositionDetailKey>(m_mapOpenTime2Key,PositionDetailInfo.OpenTimeUTC,PositionDetailKey(PositionDetailInfo));
        if(PositionDetailInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived)
            m_bHaveComb=true;
    }

    if(bIsLast)
    {
        //更新持仓明细的持仓盈亏, 浮动盈亏, 昨结算价, 结算价, 现价
        set<string> setInstruments;
        m_PositionDetailData.GetInstrumentSetOfQryRlt(setInstruments);

        set<string>::iterator it_Instrument;
        PlatformStru_DepthMarketData QuotData;
        PlatformStru_InstrumentInfo InstrumentInfo;

        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
            if(m_RefInstruments.GetInstrumentInfo(*it_Instrument,InstrumentInfo)&&
                m_RefQuots.GetQuotInfo(*it_Instrument,QuotData))
            {
               UpdatePositionDetailByQuot2_Internal(QuotData,InstrumentInfo,true);
            }
        }
    }

    m_mutex.unlock();

    LOG_INFO("CPlatformDataMgr_PositionDetails::QryPositionDetailReach-end");
}



//动态更新持仓明细记录的持仓盈亏, 浮动盈亏, 结算价, 昨结算价, 现价, 根据行情, 得到新行情时做一遍
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnDepthMarketData(const PlatformStru_DepthMarketData& QuotData,
                                                                               const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    CeasymutexGuard guard(m_mutex);

    UpdatePositionDetailByQuot2_Internal(QuotData,InstrumentInfo,false);
}

//更新合约对应的持仓明细记录的持仓盈亏, 浮动盈亏, 结算价, 昨结算价, 现价, 根据行情, 得到新行情时做一遍
//新行情更新bForceCalculate为false, 表示仅在价格变动时更新, 其它情况为true
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailByQuot2_Internal(const PlatformStru_DepthMarketData& QuotData,
                                                                            const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                            bool bForceCalculate) 
{
    map<string,set<long>>::iterator it_Index2=m_PositionDetailData.m_allIndex2.find(string(QuotData.InstrumentID));
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            PlatformStru_PositionDetail* pdetail;
            if(m_PositionDetailData.GetValue2_Internal(pdetail,*itFTID))
            {
                bool bUpdate;
                UpdatePositionDetailByQuot_Internal(QuotData,InstrumentInfo,bForceCalculate,*pdetail,bUpdate);
                if(bUpdate) pdetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pdetail->FTID);
            }
        }
    }
}

//更新持仓明细记录的持仓盈亏(盯市), 浮动盈亏(逐笔), 结算价, 昨结算价, 现价, 根据行情
//新行情更新bForceCalculate为false, 表示仅在价格变动时更新, 其它情况为true
//计算盈亏优先使用的价格顺序为: 结算价 > 最新价 > 昨结算价
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailByQuot_Internal(const PlatformStru_DepthMarketData& QuotData,
                                                                           const PlatformStru_InstrumentInfo InstrumentInfo,
                                                                           bool bForceCalculate,
                                                                           PlatformStru_PositionDetail& detail,
                                                                           bool& bUpdate)
{
    bUpdate=false;

    double CurPrice;
    if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(QuotData.SettlementPrice)) CurPrice=QuotData.SettlementPrice;
    else if(QuotData.Volume>0) CurPrice=QuotData.LastPrice;
    else CurPrice=QuotData.PreSettlementPrice;
    if(util::isInvalidValue(CurPrice))
        return;

    //更新结算价、昨结算价
    if(util::isInvalidValue(detail.LastSettlementPrice)&&!util::isInvalidValue(QuotData.PreSettlementPrice)) { detail.LastSettlementPrice=QuotData.PreSettlementPrice;  bUpdate=true; }
    if(util::isInvalidValue(detail.SettlementPrice)&&!util::isInvalidValue(QuotData.SettlementPrice)) { detail.SettlementPrice=QuotData.SettlementPrice;  bUpdate=true; }

    //非强制计算时, 如价格未更新, 不进行计算
    if(!bForceCalculate&&CurPrice==detail.CurPrice) return;

    //更新现价
    if(CurPrice!=detail.CurPrice) { detail.CurPrice=CurPrice;  bUpdate=true; }

    //计算逐日盯市持仓盈亏
    double LastPrice = detail.bToday ? detail.OpenPrice : detail.LastSettlementPrice;
    double DeltaPrice = detail.Direction==THOST_FTDC_D_Buy ? CurPrice-LastPrice : LastPrice-CurPrice;
    double PositionProfitByDate = DeltaPrice*detail.Volume*InstrumentInfo.VolumeMultiple;
    if(PositionProfitByDate==-0) PositionProfitByDate=0;

    //计算触笔对冲持仓盈亏(浮动盈亏)
    DeltaPrice = detail.Direction==THOST_FTDC_D_Buy ? CurPrice-detail.OpenPrice : detail.OpenPrice-CurPrice;
    double PositionProfitByTrade=DeltaPrice*detail.Volume*InstrumentInfo.VolumeMultiple;
    if(PositionProfitByTrade==-0) PositionProfitByTrade=0;

    if( PositionProfitByDate!=detail.PositionProfitByDate){ detail.PositionProfitByDate=PositionProfitByDate; bUpdate=true; }
    if( PositionProfitByTrade!=detail.PositionProfitByTrade){ detail.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
}

//结算时, 用结算价更新持仓明细记录的持仓盈亏(盯市), 浮动盈亏(逐笔), 保证金, 结算价, 昨结算价, 现价
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_OnSettlement() 
{
    m_mutex.lock();

    PlatformStru_DepthMarketData QuotData;
    PlatformStru_InstrumentInfo InstrumentInfo;
    PlatformStru_InstrumentMarginRate MarginRate;
    map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
    map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;
    for(set<long>::iterator it=m_PositionDetailData.m_allFTID.begin();it!=m_PositionDetailData.m_allFTID.end();it++)
    {
        map<long,PlatformStru_PositionDetail*>::iterator it_Info2=m_PositionDetailData.m_all2.find(*it);
        if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL)
        {
            if(m_RefInstruments.GetInstrumentInfo(string(it_Info2->second->InstrumentID),InstrumentInfo)&&
                m_RefQuots.GetQuotInfo(string(it_Info2->second->InstrumentID),QuotData)&&
                m_RefRates.GetMarginRate(string(it_Info2->second->InstrumentID),MarginRate)==0)
            {
                bool bUpdate1,bUpdate2;

                PlatformStru_PositionDetail detail=*(it_Info2->second);

                //更新持仓明细的持仓盈亏, 浮动盈亏, 昨结算价, 结算价, 现价
                UpdatePositionDetailByQuot_Internal(QuotData,InstrumentInfo,true,*(it_Info2->second),bUpdate1); 

                //这时持仓明细中的结算价已经有了, 计算保证金(不考虑组合优惠)
                CalculateMargin_Internal(MarginRate,InstrumentInfo,*(it_Info2->second),bUpdate2);

                if(bUpdate1||bUpdate2) 
                {
                    it_Info2->second->UpdateSeq=m_PositionDetailData.NewUpdateSeq(it_Info2->second->FTID);
                    PositionDetailKey key(detail);
                    if(ModifiedPositionDetails.find(key)==ModifiedPositionDetails.end())
                        ModifiedPositionDetails[key]=detail;
                }
            }
        }
    }

    m_mutex.unlock();

    //针对有变化的持仓明细, 计算组合持仓明细的保证金优惠. 需要在更新持仓明细和更新组合持仓明细后进行. 
    CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(ModifiedPositionDetails,ModifiedPositionDetailCombs);
}


//取出持仓明细中的记录总数
int CPlatformDataMgr_PositionDetails::GetRecordCountOfPositionDetail(void)
{
    CeasymutexGuard guard(m_mutex);

    return (int)(m_PositionDetailData.m_all.size());
}

//从成交记录更新持仓明细. 会修改TradeInfo的平今量, 平仓盈亏
//ModifiedPositionDetails保存受到影响的持仓明细记录, n*2+0是原来的数据, n*2+1是新数据
//ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade(PlatformStru_TradeInfo& TradeInfo,
                                                                     const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                     map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                     map<string,int>& ComTradeID2CloseVolume)
{
    LOG_INFO("CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade(成交回报更新持仓明细):InstrumentID=%s",TradeInfo.InstrumentID);

    string strInstrument(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    CeasymutexGuard guard(m_mutex);

    m_DerivedInstrumentIDs_OnCloseTrade.clear();

    UpdatePositionDetail_RtnTrade_Internal(TradeInfo,false,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,m_PositionDetailData);

    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        //当前持仓明细的合约ID
        string thisInstrumentID(it->first.InstrumentID);

        //检查平仓成交中是否有被打散而衍生的普通持仓明细的合约ID. 在GUI中更新持仓明细时, 需要更新此衍生合约的持仓明细
        if(thisInstrumentID!=strInstrument)
        {
            if(m_DerivedInstrumentIDs_OnCloseTrade.find(thisInstrumentID)==m_DerivedInstrumentIDs_OnCloseTrade.end())
                m_DerivedInstrumentIDs_OnCloseTrade.insert(thisInstrumentID);
        }

        //如果是新建的持仓明细, 包括开仓成交和平仓打散组合两种情况, 维护m_mapOpenTime2Key和m_bHaveComb
        PlatformStru_PositionDetail* pdetail;
        if(it->second.IsInvalidRecord()&&
            m_PositionDetailData.GetValue_Internal(pdetail,it->first))
        {
            CTools_Ansi::AddKeyValueToMultimap<int,PositionDetailKey>(m_mapOpenTime2Key,pdetail->OpenTimeUTC,PositionDetailKey(*pdetail));
            if(it->first.TradeType==THOST_FTDC_TRDT_CombinationDerived)
                m_bHaveComb=true;
        }
    }

    LOG_INFO("CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade-end");
}

//从成交记录更新持仓明细
//返回平掉的持仓明细的记录集, 用于后续计算. ModifiedPositionDetails保存受到影响的持仓明细记录, n*2+0是原来的数据, n*2+1是新数据
//会修改TradeInfo的平今量, 平仓盈亏
//bPartCalcu:只计算平仓量, 平仓盈亏, 不计算保证金, 持仓盈亏等
//ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                                              bool bPartCalcu,
                                                                              const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                              map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                              map<string,int>& ComTradeID2CloseVolume,
                                                                              CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetails)
{
    string strInstrument(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;


    TradeInfo.CloseTodayVolume=0;
    TradeInfo.CloseProfitByDate=0;
    TradeInfo.CloseProfitByTrade=0;

    //1. 从成交回报计算相关持仓明细的仓位变化
    UpdatePositionDetailVolumeOnTrade_Internal(TradeInfo,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,PositionDetails,m_RefPositionDetailCombs,true);

    //2. 计算持仓明细受影响的其它值
    PlatformStru_DepthMarketData QuotInfo;
    bool bHaveQuot = bPartCalcu ? false : m_RefQuots.GetQuotInfo(strInstrument,QuotInfo);

    PlatformStru_InstrumentMarginRate MarginRate;
    bool bHaveMarginRate = bPartCalcu ? false : m_RefRates.GetMarginRate(strInstrument,MarginRate)==0;

    PlatformStru_InstrumentInfo otherInstrumentInfo;
    PlatformStru_InstrumentMarginRate otherMarginRate;
    PlatformStru_DepthMarketData otherQuotInfo;
    PlatformStru_PositionDetail *pdetail;
    bool bUpdate;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        if(!PositionDetails.GetValue_Internal(pdetail,it->first)) continue;

        //当前持仓明细的合约ID
        string thisInstrumentID=string(it->first.InstrumentID);

        //当前持仓明细的合约信息
        if(thisInstrumentID!=strInstrument&&
            !m_RefInstruments.GetInstrumentInfo(thisInstrumentID,otherInstrumentInfo)) 
            continue;
        const PlatformStru_InstrumentInfo& thisInstrumentInfo = thisInstrumentID==strInstrument ? InstrumentInfo : otherInstrumentInfo;

        if(!bPartCalcu)
        {
            //更新持仓明细的结算价, 昨结算价, 现价, 持仓盈亏(盯市), 浮动盈亏(逐笔)
            if(thisInstrumentID==strInstrument ? bHaveQuot : (m_RefQuots.GetQuotInfo(thisInstrumentID,otherQuotInfo)))
            {
                UpdatePositionDetailByQuot_Internal(thisInstrumentID==strInstrument?QuotInfo:otherQuotInfo,thisInstrumentInfo,true,*pdetail,bUpdate); 
            }

            //更新持仓明细的保证金, 保证金率, 交易所保证金。这里不考虑组合优惠
            if(thisInstrumentID==strInstrument ? bHaveMarginRate : (m_RefRates.GetMarginRate(thisInstrumentID,otherMarginRate)==0))
            {
                CalculateMargin_Internal(thisInstrumentID==strInstrument?MarginRate:otherMarginRate,thisInstrumentInfo,*pdetail,bUpdate);
            }
        }

        pdetail->UpdateSeq=PositionDetails.NewUpdateSeq(pdetail->FTID);
    }

}

//根据日初持仓明细, 日内成交记录, 重新计算成交的平今量, 平仓盈亏, 手续费
//初始化查询完成交和持仓明细后调用
//注意: 1. 大商郑商平今优惠, 不是按平掉的持仓明细是否今仓来判断, 而是按是否有今仓可平来判断. 
//         比如, 有昨仓1手今仓1手, 平1手, 平的是昨仓, 但是由于有今仓, 因此手续费优惠
//      2. 大商、上期、中金按先开先平原则进行平仓，针对某个合约的开平仓，不用考虑组合合约的对手
//         郑商按先单一后组合的原则进行平仓，需要考虑组合合约的对手。但郑商隔夜不进行打散重组，因此其组合持仓明细和持仓明细一一对应，可以从日初持仓明细推算出组合持仓明细
void CPlatformDataMgr_PositionDetails::ReCalcuTradeInfo_OnInitQryCompleted(void)
{
    volatile long baseFTID=1;
    volatile long baseUpdateSeq=1;
    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail> PositionDetails(baseFTID,baseUpdateSeq);

    CeasymutexGuard guard(m_mutex);

    //获取全部成交的FTID, 按合约ID分类. 同一个合约的FTID是排序的
    map<string,vector<long>> TradeFTIDs;
    m_RefTrades.GetFTIDsByInstrument(TradeFTIDs);

    //获取日初持仓明细
    GetStaticPositionDetail_Internal(PositionDetails);

    //获取郑商的日初组合持仓明细
    vector<PlatformStru_PositionDetailComb> vecPositionDetailCombs_CZCE;
    m_RefPositionDetailCombs.GetStaticPositionDetailComb_CZCE(PositionDetails,vecPositionDetailCombs_CZCE);
    CPlatformDataMgr_PositionDetailCombs StaticPositionDetailCombs_CZCE(baseFTID,baseUpdateSeq,m_RefInstruments,m_RefQuots,m_RefTrades,m_RefOrders);
    CPlatformDataMgr_PositionDetailCombs StaticPositionDetailCombs2(baseFTID,baseUpdateSeq,m_RefInstruments,m_RefQuots,m_RefTrades,m_RefOrders);
    for(int i=0;i<(int)vecPositionDetailCombs_CZCE.size();i++)
        StaticPositionDetailCombs_CZCE.QryPositionDetailCombReach(vecPositionDetailCombs_CZCE[i],0,i==(int)vecPositionDetailCombs_CZCE.size()-1?true:false);


    //对每一笔成交, 更新持仓明细, 以及成交相关数据

    map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
    map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;
    map<string,int> ComTradeID2CloseVolume;
    PlatformStru_TradeInfo TradeInfo;
    PlatformStru_InstrumentInfo InstrumentInfo;
    string strExchangeID;

    //大商、上期和中金是先开先平，不用考虑组合打散
    //郑商所是先单一后组合，需要考虑组合打散
    for(map<string,vector<long>>::iterator it=TradeFTIDs.begin();it!=TradeFTIDs.end();it++)
    {
        if(!m_RefInstruments.GetInstrumentInfo(it->first,InstrumentInfo)) continue;
        strExchangeID=string(InstrumentInfo.ExchangeID);

        for(int i=0;i<(int)it->second.size();i++)
        {
            if(!m_RefTrades.GetTradeInfo(it->second[i],TradeInfo)) continue;

            ModifiedPositionDetails.clear();

            TradeInfo.CloseTodayVolume=0;
            TradeInfo.CloseProfitByDate=0;
            TradeInfo.CloseProfitByTrade=0;

            //从成交回报计算相关持仓明细的仓位变化
            if(strExchangeID==string("CZCE"))
                UpdatePositionDetailVolumeOnTrade_Internal(TradeInfo,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,PositionDetails,StaticPositionDetailCombs_CZCE,true);
            else
                UpdatePositionDetailVolumeOnTrade_Internal(TradeInfo,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,PositionDetails,StaticPositionDetailCombs2,false);

            //更新成交记录中的平今量, 平仓盈亏, 重算手续费. 
            m_RefTrades.UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(TradeInfo.FTID,
                InstrumentInfo,
                TradeInfo.CloseTodayVolume,
                TradeInfo.CloseProfitByDate,
                TradeInfo.CloseProfitByTrade,
                TradeInfo.TradeCommission);
        }
    }
}


//获取日初持仓明细
void CPlatformDataMgr_PositionDetails::GetStaticPositionDetail_Internal(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail)
{
    StaticPositionDetail.Clear();
    vector<long> FTIDs;
    PlatformStru_PositionDetail detail;

    m_PositionDetailData.GetAllFTID_Internal(FTIDs);
    for(int i=0;i<(int)FTIDs.size();i++)
    {
        if(m_PositionDetailData.GetValue3_Internal(detail,FTIDs[i])&&
            !detail.bToday)
        {
            int CloseVolume=detail.CloseVolume;
            if(CloseVolume>0)
            {
                detail.Volume+=CloseVolume;
                detail.CloseVolume=0;
                detail.CloseAmount=0;
                detail.CloseProfitByDate=0;
                detail.CloseProfitByTrade=0;
                detail.PositionProfitByDate=0;
                detail.PositionProfitByTrade=0;
                detail.CurPrice=detail.LastSettlementPrice;
            }
            StaticPositionDetail.NewReach_Internal(detail);
        }
    }
}

//从成交回报计算相关持仓明细仓位的变化. 注意, 只更新持仓明细的仓位、平仓量、平仓金额, 不更新其它值；更新成交信息中的平今量、平仓盈亏
//PositionDetailData是数据集. 可以是m_PositionDetailData,也可以是临时数据集(为了匹配成交和持仓明细)
//bCareCombOppoDetail:表示持仓明细是组合衍生, 是否对其对手持仓明细进行打散操作。正常情况下需要，在初始化查询完毕重新演算是如果不是郑商所就不需要
//ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailVolumeOnTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                                                  const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                                  map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                                  map<string,int>& ComTradeID2CloseVolume,
                                                                                  CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetailData,
                                                                                  CPlatformDataMgr_PositionDetailCombs& PositionDetailCombData,
                                                                                  bool bCareCombOppoDetail)
{
    if(TradeInfo.Volume<=0)
        return;
    string strInstrument(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    ComTradeID2CloseVolume.clear();

    string strExchangeID(TradeInfo.ExchangeID);

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {                                                   
        //开仓成交创建持仓明细

	    PlatformStru_PositionDetail PositionDetailInfo(TradeInfo);

        //将创建的持仓明细送到数据集中
        PositionDetailData.NewReach_Internal(PositionDetailInfo);

        //保存影响到的持仓明细记录
        PositionDetailKey key(PositionDetailInfo);
        if(ModifiedPositionDetails.find(key)==ModifiedPositionDetails.end())
            ModifiedPositionDetails[key]=PlatformStru_PositionDetail(true);
    }

    else if( TradeInfo.OffsetFlag==THOST_FTDC_OF_Close||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_ForceClose||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
    { 
        //平仓成交修改持仓明细
        //根据TradeInfo的合约, 开平标志, 买卖方向, 平掉相应的仓位, 修正仓位, 平仓量, 平仓金额
        //循环处理, 直到平仓手数处理完
        //1. 郑商大商组合成交, 如果配置了先单一后组合的平仓顺序, 先平单一持仓
        //2. 其它情况按先开先平原则

        int CloseVolume=TradeInfo.Volume;

        //注意: set<PositionDetailKey>中是按时间排序的, 从前往后处理, 就能保证先开先平
        map<string,set<PositionDetailKey>>::iterator it_keyset=PositionDetailData.m_allIndex.find(strInstrument);
        if(it_keyset==PositionDetailData.m_allIndex.end()||it_keyset->second.empty())
            return;

        //第一步, 先平单一持仓. (只对郑商大商配置了先单一后组合的平仓顺序有效)
        //第二步, 对剩余待平仓量, 统一按先开先平原则进行平仓
        for(int step=0;step<2&&CloseVolume>0;step++)
        {
            if(step==0&&
                !(strExchangeID==string("CZCE")&&m_UnwindPrioritiesCZCE_Comb==1||
                  strExchangeID==string("DCE")&&m_UnwindPrioritiesDCE_Comb==1))
            {
                //如果不是配置了先单一后组合的平仓规则, 跳过第一步
                continue;
            }

	        for(set<PositionDetailKey>::iterator itKey=it_keyset->second.begin() ; CloseVolume>0&&itKey!=it_keyset->second.end() ; itKey++)
	        {
                //如果是第一步, 跳过组合持仓明细
                if(step==0&&itKey->TradeType==THOST_FTDC_TRDT_CombinationDerived) continue;

                PlatformStru_PositionDetail *pPositionDetail;
                if(!PositionDetailData.GetValue_Internal(pPositionDetail,*itKey)) continue;

                //跳过手数为零的持仓明细
                if(pPositionDetail->Volume==0) continue;

                //跳过非对应持仓明细
		        if(!IsPositionDetailForCloseTrade_IgnoreTradeType(*pPositionDetail,TradeInfo)) continue;

		        //记录更新前的持仓明细数据
                if(ModifiedPositionDetails.find(*itKey)==ModifiedPositionDetails.end())
                    ModifiedPositionDetails[*itKey]=*pPositionDetail;

		        //PlatformStru_PositionDetail OldPositionDetail=*pPositionDetail;

                //这个持仓明细可以平掉的手数
                int thisVolume = CloseVolume>pPositionDetail->Volume ? pPositionDetail->Volume : CloseVolume;

	            //更新仓位手数
                pPositionDetail->Volume-=thisVolume;
	            CloseVolume-=thisVolume;

                //更新平仓量, 平仓金额
                pPositionDetail->CloseVolume+=thisVolume;
                pPositionDetail->CloseAmount+=thisVolume*InstrumentInfo.VolumeMultiple*TradeInfo.Price;
                if(pPositionDetail->bToday) TradeInfo.CloseTodayVolume+=thisVolume;

                //更新平仓盈亏
                double dLastPrice = pPositionDetail->bToday ? pPositionDetail->OpenPrice : pPositionDetail->LastSettlementPrice;
                double DeltaPrice = pPositionDetail->Direction==THOST_FTDC_D_Buy ? TradeInfo.Price-dLastPrice : dLastPrice-TradeInfo.Price;
                double CloseProfitByDate=DeltaPrice*thisVolume*InstrumentInfo.VolumeMultiple;
                if(CloseProfitByDate==-0) CloseProfitByDate=0;
                pPositionDetail->CloseProfitByDate+=CloseProfitByDate;
                TradeInfo.CloseProfitByDate+=CloseProfitByDate;

                DeltaPrice = pPositionDetail->Direction==THOST_FTDC_D_Buy ? TradeInfo.Price-pPositionDetail->OpenPrice : pPositionDetail->OpenPrice-TradeInfo.Price;
                double CloseProfitByTrade=DeltaPrice*thisVolume*InstrumentInfo.VolumeMultiple;
                if(CloseProfitByTrade==-0) CloseProfitByTrade=0;
                pPositionDetail->CloseProfitByTrade+=CloseProfitByTrade;
                TradeInfo.CloseProfitByTrade+=CloseProfitByTrade;


                //如果持仓明细是组合衍生, 对其对手持仓明细进行打散操作
                //可能有多个对手持仓明细与之对应
                if(pPositionDetail->TradeType==THOST_FTDC_TRDT_CombinationDerived&&
                    bCareCombOppoDetail)
                {
                    int thisVolume2=thisVolume;

                    set<PositionDetailCombKey> combkeys;
                    PositionDetailCombData.GetCombKeysByPositionDetailKey(*itKey,combkeys,true);
                    set<string> ComTradeIDs;
                    for(set<PositionDetailCombKey>::const_iterator itcombkey=combkeys.begin();itcombkey!=combkeys.end();itcombkey++)
                    {
                        string strComTradeID(itcombkey->ComTradeID);
                        if(ComTradeIDs.find(strComTradeID)==ComTradeIDs.end()) ComTradeIDs.insert(strComTradeID);
                    }

                    for(set<string>::const_iterator itComTradeID=ComTradeIDs.begin();thisVolume2>0&&itComTradeID!=ComTradeIDs.end();itComTradeID++)
                    {
                        set<PositionDetailCombKey> combkeys2;
                        PositionDetailCombData.GetCombKeysByComTradeID(*itComTradeID,combkeys2,true);
                        for(set<PositionDetailCombKey>::const_iterator itkey=combkeys2.begin();thisVolume2>0&&itkey!=combkeys2.end();itkey++)
                        {
                            //只处理对手组合持仓明细
                            if(itkey->Direction==pPositionDetail->Direction) continue;

                            PositionDetailKey oppodetailkey;
                            Convert_PositionDetailCombKey_to_PositionDetailKey(*itkey,oppodetailkey);
                            PlatformStru_PositionDetail *poppodetail;
                            if(!PositionDetailData.GetValue_Internal(poppodetail,oppodetailkey)) continue;

                            assert(poppodetail->TradeType==THOST_FTDC_TRDT_CombinationDerived);

                            //衍生(被打散成普通)持仓明细的key
                            PositionDetailKey DerivedKey(oppodetailkey);
                            DerivedKey.TradeType=THOST_FTDC_TRDT_Common;

                            //pExistCommonPositionDetail是衍生的普通的持仓明细, 可能是首次被打散衍生的, 也可能是多次被打散衍生的
                            PlatformStru_PositionDetail* pExistCommonPositionDetail;
                            bool bExistCommonPositionDetail=PositionDetailData.GetValue_Internal(pExistCommonPositionDetail,DerivedKey);
                            if(!bExistCommonPositionDetail)
                            {
                                //从OppoPositionDetail创建空的NewPositionDetail
                                PlatformStru_PositionDetail NewPositionDetail=*poppodetail;

                                NewPositionDetail.Volume=0;
                                NewPositionDetail.TradeType=THOST_FTDC_TRDT_Common;
                                memset(NewPositionDetail.CombInstrumentID,0,sizeof(NewPositionDetail.CombInstrumentID));
                                NewPositionDetail.CloseProfitByDate=0;
                                NewPositionDetail.CloseProfitByTrade=0;
                                NewPositionDetail.PositionProfitByDate=0;
                                NewPositionDetail.PositionProfitByTrade=0;
                                NewPositionDetail.Margin=0;
                                NewPositionDetail.MarginRateByMoney=util::GetDoubleInvalidValue();
                                NewPositionDetail.MarginRateByVolume=util::GetDoubleInvalidValue();
                                NewPositionDetail.ExchMargin=0;
                                NewPositionDetail.CloseVolume=0;
                                NewPositionDetail.CloseAmount=0;

                                PositionDetailData.NewReach_Internal(NewPositionDetail);

                                PositionDetailData.GetValue_Internal(pExistCommonPositionDetail,DerivedKey);
                            }

		                    //记录更新前的持仓明细数据
                            if(ModifiedPositionDetails.find(oppodetailkey)==ModifiedPositionDetails.end())
                                ModifiedPositionDetails[oppodetailkey]=*poppodetail;
                            if(ModifiedPositionDetails.find(DerivedKey)==ModifiedPositionDetails.end())
                                ModifiedPositionDetails[DerivedKey]=*pExistCommonPositionDetail;

                            //更新持仓量
                            int DeltaVolume=poppodetail->Volume;
                            if(DeltaVolume>thisVolume2) DeltaVolume=thisVolume2;

                            thisVolume2-=DeltaVolume;
                            poppodetail->Volume-=DeltaVolume;
                            pExistCommonPositionDetail->Volume+=DeltaVolume;

                            //维护被平掉的组合成交ID和对应平仓量的关系
                            map<string,int>::iterator itComTradeID2CloseVolume=ComTradeID2CloseVolume.find(*itComTradeID);
                            if(itComTradeID2CloseVolume==ComTradeID2CloseVolume.end())
                                ComTradeID2CloseVolume.insert(pair<string,int>(*itComTradeID,DeltaVolume));
                            else itComTradeID2CloseVolume->second+=DeltaVolume;

                            //更新序列号
                            poppodetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(poppodetail->FTID);
                            pExistCommonPositionDetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pExistCommonPositionDetail->FTID);
                        }
                    }
                }
	        }
        }
    }
}



//针对有变化的持仓明细, 计算组合持仓明细的保证金优惠. 需要在更新持仓明细和更新组合持仓明细后进行. 根据保证金优惠设置进行计算, 更新相应持仓明细和组合持仓明细的保证金字段, (为保证不漏掉, 两腿都算, 有点重复)
//持仓明细数据里已经有未优惠的保证金了
void CPlatformDataMgr_PositionDetails::CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                                                          map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs)
{
    CeasymutexGuard guard(m_mutex);

    //把需要计算的PositionDetailKey放到keys里
    set<PositionDetailKey> keys;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        //对于组合衍生持仓明细进行处理.
        if(it->first.TradeType==THOST_FTDC_TRDT_CombinationDerived)
        {
            keys.insert(it->first);
        }
    }

    //取所有持仓明细key对应的ComTradeID
    set<string> ComTradeIDs;
    m_RefPositionDetailCombs.GetComTradeIDsByPositionDetailKeys(keys,ComTradeIDs,true);

    //对每一个ComTradeID进行保证金优惠计算
    for(set<string>::const_iterator it=ComTradeIDs.begin();it!=ComTradeIDs.end();it++)
    {
        set<PositionDetailCombKey> leg1keys;
        set<PositionDetailCombKey> leg2keys;
        m_RefPositionDetailCombs.GetCombKeysByComTradeID2(*it,leg1keys,leg2keys,true);

        if(leg1keys.empty()||leg2keys.empty()) continue;

        string leg1InstrumentID(leg1keys.begin()->InstrumentID);
        string leg2InstrumentID(leg2keys.begin()->InstrumentID);

        string strExchangeID;
        if(!m_RefInstruments.GetExchangeID(leg1InstrumentID,strExchangeID)) continue;

        int Discount=0;
        if(strExchangeID==string("CZCE")) Discount=m_DiscountCZCE_Comb;
        else if(strExchangeID==string("DCE")) Discount=m_DiscountDCE_Comb;

        int discountLegID=-1;
        if(Discount==1||Discount==2)
        {
            //第一腿优惠或第二腿优惠
            discountLegID = Discount==1 ? 0 : 1;
        }
        if(Discount==3)
        {
            //小边优惠
            double leg1ExchMargin=0,leg2ExchMargin=0;
            for(set<PositionDetailCombKey>::const_iterator it2=leg1keys.begin();it2!=leg1keys.end();it2++)
            {
                double exchMargin=m_RefPositionDetailCombs.GetExchMargin(*it2);
                if(util::isInvalidValue(exchMargin))
                {
                    leg1ExchMargin=util::GetDoubleInvalidValue();
                    break;
                }
                else leg1ExchMargin+=exchMargin;
            }
            for(set<PositionDetailCombKey>::const_iterator it2=leg2keys.begin();it2!=leg2keys.end();it2++)
            {
                double exchMargin=m_RefPositionDetailCombs.GetExchMargin(*it2);
                if(util::isInvalidValue(exchMargin))
                {
                    leg2ExchMargin=util::GetDoubleInvalidValue();
                    break;
                }
                else leg2ExchMargin+=exchMargin;
            }

            if(util::isInvalidValue(leg1ExchMargin)||util::isInvalidValue(leg2ExchMargin)) continue;

            if(leg1ExchMargin<leg2ExchMargin) discountLegID=0;
            else if(leg1ExchMargin>leg2ExchMargin) discountLegID=1;
            else
            {
                //两边相等, 优惠掉第二边
                discountLegID = 1;
            }
        }

        const set<PositionDetailCombKey>& tmpcombkeys=discountLegID==0?leg1keys:leg2keys;
        PlatformStru_PositionDetail *pdetail;
        for(set<PositionDetailCombKey>::const_iterator it2=tmpcombkeys.begin();it2!=tmpcombkeys.end();it2++)
        {
            double Margin,ExchMargin;
            m_RefPositionDetailCombs.GetMargin(*it2,Margin,ExchMargin);
            assert(!util::isInvalidValue(Margin)&&!util::isInvalidValue(ExchMargin));

            PositionDetailKey detailkey;
            Convert_PositionDetailCombKey_to_PositionDetailKey(*it2,detailkey);
            if(m_PositionDetailData.GetValue_Internal(pdetail,detailkey))
            {
                if(ModifiedPositionDetails.find(detailkey)==ModifiedPositionDetails.end())
                    ModifiedPositionDetails[detailkey]=*pdetail;

                pdetail->Margin-=Margin;
                pdetail->ExchMargin-=ExchMargin;
                pdetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pdetail->FTID);
            }

            if(ModifiedPositionDetailCombs.find(*it2)==ModifiedPositionDetailCombs.end())
            {
                PlatformStru_PositionDetailComb detailcomb;
                if(m_RefPositionDetailCombs.GetPositionDetailComb(*it2,detailcomb))
                    ModifiedPositionDetailCombs[*it2]=detailcomb;
            }

            m_RefPositionDetailCombs.SetMargin(*it2,0,0);
        }
    }
}

//针对有变化的持仓明细，将保证金数据设置到组合持仓明细中。此时未计算优惠
//对每个持仓明细key，找对应的组合持仓明细key和对手组合持仓明细key，再根据这些组合持仓明细key，找相关的持仓明细key
void CPlatformDataMgr_PositionDetails::SetMarginOfPositionDetailComb(map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails)
{
    CeasymutexGuard guard(m_mutex);

    PositionDetailKey tmpdetailkey;
    set<PositionDetailKey> refdetailkeys;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>> combkeys;
        m_RefPositionDetailCombs.GetCombKeysByPositionDetailKey2(it->first,combkeys,true);
        for(map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>::const_iterator it_combkeys=combkeys.begin();it_combkeys!=combkeys.end();it_combkeys++)
        {
            for(set<PositionDetailCombKey>::const_iterator it_combkey2=it_combkeys->second.first.begin();it_combkey2!=it_combkeys->second.first.end();it_combkey2++)
            {
                Convert_PositionDetailCombKey_to_PositionDetailKey(*it_combkey2,tmpdetailkey);
                if(refdetailkeys.find(tmpdetailkey)==refdetailkeys.end())
                    refdetailkeys.insert(tmpdetailkey);
            }
            for(set<PositionDetailCombKey>::const_iterator it_combkey2=it_combkeys->second.second.begin();it_combkey2!=it_combkeys->second.second.end();it_combkey2++)
            {
                Convert_PositionDetailCombKey_to_PositionDetailKey(*it_combkey2,tmpdetailkey);
                if(refdetailkeys.find(tmpdetailkey)==refdetailkeys.end())
                    refdetailkeys.insert(tmpdetailkey);
            }
        }
    }

    PlatformStru_PositionDetail* pdetail;
    for(set<PositionDetailKey>::const_iterator it=refdetailkeys.begin();it!=refdetailkeys.end();it++)
    {
        if(m_PositionDetailData.GetValue_Internal(pdetail,*it))
        {
            m_RefPositionDetailCombs.SetMarginByPositionDetail(*pdetail);
        }
    }
}

//更新持仓明细记录的保证金, 保证金率, 交易所保证金, 成交修改持仓明细和查询保证金率响应时调用
//如果有结算价, 按结算价计算; 否则, 按开盘价或昨结算价计算(根据组合持仓明细配置以及今仓昨仓属性)
//对组合持仓明细, 这里不考虑保证金优惠
void CPlatformDataMgr_PositionDetails::CalculateMargin_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                PlatformStru_PositionDetail& PositionDetail,
                                                                bool& bUpdate)
{
    if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
    {
        //期权合约，另行处理
        CalculateMargin_Option_Internal(MarginRate,InstrumentInfo,PositionDetail,bUpdate);
        return;
    }

    bUpdate=false;

    string strInstrument(PositionDetail.InstrumentID);
    if(strInstrument.empty())
        return;

    //CFileOpr::getObj().writelocallog("计算保证金","Start:\tInstrumentID:%s,TradeID:%s,OpenDate:%s,OpenPrice:%g",PositionDetail.InstrumentID,PositionDetail.TradeID,PositionDetail.OpenDate,PositionDetail.OpenPrice);

    double Margin=PositionDetail.Margin;
    double MarginRateByMoney=PositionDetail.MarginRateByMoney;
    double MarginRateByVolume=PositionDetail.MarginRateByVolume;
    double ExchMargin=PositionDetail.ExchMargin;

    //如果有结算价, 按结算价计算; 否则, 按开仓价或昨结算价计算(根据组合持仓明细配置以及今仓昨仓属性)
    //对于组合持仓明细, 需要根据配置, 看看使用开仓价还是昨结算价
    double dPrice;
    if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(PositionDetail.SettlementPrice)) 
    {
        dPrice=PositionDetail.SettlementPrice;
        //CFileOpr::getObj().writelocallog("计算保证金","1.1:\t用结算价");
    }
    else if(!PositionDetail.bToday) 
    {
        dPrice=PositionDetail.LastSettlementPrice;
        //CFileOpr::getObj().writelocallog("计算保证金","1.2:\t用昨结算价");
    }
    else if(PositionDetail.TradeType==THOST_FTDC_TRDT_CombinationDerived&&
        (strncmp(PositionDetail.ExchangeID,"CZCE",sizeof(PositionDetail.ExchangeID)-1)==0&&m_PriceForMarginOfTodayPositionCZCE_Comb==1||
         strncmp(PositionDetail.ExchangeID, "DCE",sizeof(PositionDetail.ExchangeID)-1)==0&&m_PriceForMarginOfTodayPositionDCE_Comb==1))
    {
        dPrice=PositionDetail.LastSettlementPrice;
        //CFileOpr::getObj().writelocallog("计算保证金","1.3:\t用昨结算价");
    }
    else 
    {
        dPrice=PositionDetail.OpenPrice;
        //CFileOpr::getObj().writelocallog("计算保证金","1.4:\t用开仓价");
    }

    //CFileOpr::getObj().writelocallog("计算保证金","2:\tdPrice=%g",dPrice);

    //这里不考虑组合单持仓明细保证金优惠
    if(PositionDetail.Volume==0)
    {
        Margin=0;
        ExchMargin=0;
        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else if(PositionDetail.Direction==THOST_FTDC_D_Buy)
    {
        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(MarginRate.LongMarginRatioByMoney)&&!util::isInvalidValue(MarginRate.LongMarginRatioByVolume))
            Margin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*MarginRate.LongMarginRatioByMoney+
                    PositionDetail.Volume*MarginRate.LongMarginRatioByVolume;
        else Margin=util::GetDoubleInvalidValue();

        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(InstrumentInfo.LongMarginRatio))
            ExchMargin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*InstrumentInfo.LongMarginRatio;
        else ExchMargin=util::GetDoubleInvalidValue();

        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else
    {
        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(MarginRate.ShortMarginRatioByMoney)&&!util::isInvalidValue(MarginRate.ShortMarginRatioByVolume))
            Margin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*MarginRate.ShortMarginRatioByMoney+
                    PositionDetail.Volume*MarginRate.ShortMarginRatioByVolume;
        else Margin=util::GetDoubleInvalidValue();

        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(InstrumentInfo.ShortMarginRatio))
            ExchMargin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*InstrumentInfo.ShortMarginRatio;
        else ExchMargin=util::GetDoubleInvalidValue();

        MarginRateByMoney=MarginRate.ShortMarginRatioByMoney;
        MarginRateByVolume=MarginRate.ShortMarginRatioByVolume;
    }
    //CFileOpr::getObj().writelocallog("计算保证金","3:\tMargin=%g/%g",Margin,ExchMargin);

    if( PositionDetail.Margin!=Margin){ PositionDetail.Margin=Margin; bUpdate=true; }
    if( PositionDetail.MarginRateByMoney!=MarginRateByMoney){ PositionDetail.MarginRateByMoney=MarginRateByMoney; bUpdate=true; }
    if( PositionDetail.MarginRateByVolume!=MarginRateByVolume){ PositionDetail.MarginRateByVolume=MarginRateByVolume; bUpdate=true; }
    if( PositionDetail.ExchMargin!=ExchMargin){ PositionDetail.ExchMargin=ExchMargin; bUpdate=true; }

    //根据持仓明细的保证金数据，设置组合持仓明细的保证金，不考虑优惠。在进行优惠计算前做一下。
    m_RefPositionDetailCombs.SetMarginByPositionDetail(PositionDetail);
}

//更新期权合约持仓明细记录的保证金, 交易所保证金, 成交修改持仓明细和查询保证金率响应时调用
//  每手保证金=MAX(权利金 + 投资者期权合约保证金不变部分, 投资者期权合约最小保证金)
//  昨仓权利金 = if(上期所，[期权合约收盘价与结算价的较大值]，[期权合约结算价]) × 期权合约乘数
//  新仓权利金 = 价格（昨结算价/成交价，由OptionRoyaltyPriceType定）*期权合约乘数
void CPlatformDataMgr_PositionDetails::CalculateMargin_Option_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                PlatformStru_PositionDetail& PositionDetail,
                                                                bool& bUpdate)
{
    bUpdate=false;

    string strInstrument(PositionDetail.InstrumentID);
    if(strInstrument.empty())
        return;

    CFileOpr::getObj().writelocallog("计算期权保证金","Start:\tInstrumentID:%s,TradeID:%s,OpenDate:%s,OpenPrice:%g",PositionDetail.InstrumentID,PositionDetail.TradeID,PositionDetail.OpenDate,PositionDetail.OpenPrice);

    double Margin=PositionDetail.Margin;
    double ExchMargin=PositionDetail.ExchMargin;

    if(PositionDetail.Direction==THOST_FTDC_D_Sell)
    {
        //期权买方才有保证金

        //判断是今仓还是昨仓。如果已经结算，今仓也按昨仓处理
        bool bToday=PositionDetail.bToday;

        //取行情
        PlatformStru_DepthMarketData    QuotData;
        if(!m_RefQuots.GetQuotInfo(string(InstrumentInfo.InstrumentID),QuotData))
            QuotData.clear();

        //CFileOpr::getObj().writelocallog("计算保证金","1.1 取行情:\t用结算价 SettlementPrice=%g PreSettlementPrice=%g PreClosePrice=%g",QuotData.SettlementPrice,QuotData.PreSettlementPrice,QuotData.PreClosePrice);

        //取计算权利金使用的价格
        double dPrice;
        if(bToday)
        {
            //今仓
            if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(QuotData.SettlementPrice)) 
            {
                //已经结算，参照昨仓处理
                if(strncmp(PositionDetail.ExchangeID, "SHFE",sizeof(PositionDetail.ExchangeID)-1)!=0)
                    dPrice = QuotData.SettlementPrice;
                else dPrice = QuotData.SettlementPrice >= QuotData.ClosePrice ? QuotData.SettlementPrice : QuotData.ClosePrice;
                //CFileOpr::getObj().writelocallog("计算保证金","1.2.2 今仓已结算:\tdPrice=%g",dPrice);
            }
            else 
            {
                //未结算
                dPrice = m_PriceForRoyalty_Option==1 ? QuotData.PreSettlementPrice : PositionDetail.OpenPrice;
                //CFileOpr::getObj().writelocallog("计算保证金","1.2.1 今仓未结算:\tdPrice=%g",dPrice);
            }
        }
        else
        {
            //昨仓
            if(strncmp(PositionDetail.ExchangeID, "SHFE",sizeof(PositionDetail.ExchangeID)-1)!=0)
                dPrice = QuotData.PreSettlementPrice;
            else dPrice = QuotData.PreSettlementPrice >= QuotData.PreClosePrice ? QuotData.PreSettlementPrice : QuotData.PreClosePrice;
            //CFileOpr::getObj().writelocallog("计算保证金","1.2.3 昨仓:\tdPrice=%g",dPrice);
        }

        //计算权利金
        double Royalty = util::isInvalidValue(dPrice)? util::GetDoubleInvalidValue() : dPrice*InstrumentInfo.VolumeMultiple;
        //CFileOpr::getObj().writelocallog("计算保证金","1.3 计算权利金:\tRoyalty=%g",Royalty);

        //计算保证金
        if(util::isInvalidValue(Royalty)||util::isInvalidValue(MarginRate.FixedMargin)||util::isInvalidValue(MarginRate.MiniMargin))
            Margin = util::GetDoubleInvalidValue();
        else Margin = (util::greaterOrEqual(Royalty+MarginRate.FixedMargin,MarginRate.MiniMargin) ? (Royalty+MarginRate.FixedMargin) : MarginRate.MiniMargin)*PositionDetail.Volume;

        if(util::isInvalidValue(Royalty)||util::isInvalidValue(MarginRate.ExchFixedMargin)||util::isInvalidValue(MarginRate.ExchMiniMargin))
            ExchMargin = util::GetDoubleInvalidValue();
        else ExchMargin = (util::greaterOrEqual(Royalty+MarginRate.ExchFixedMargin,MarginRate.ExchMiniMargin) ? (Royalty+MarginRate.ExchFixedMargin) : MarginRate.ExchMiniMargin)*PositionDetail.Volume;
        //CFileOpr::getObj().writelocallog("计算保证金","1.4 计算保证金:\tMargin=%g ExchMargin=%g",Margin,ExchMargin);
    }
    else
    {
        //期权买方没有保证金
        Margin=0.0;
        ExchMargin=0.0;

        //CFileOpr::getObj().writelocallog("计算保证金","2.1:\t买方权利金为0");
    }

    if( PositionDetail.Margin!=Margin){ PositionDetail.Margin=Margin; bUpdate=true; }
    if( PositionDetail.ExchMargin!=ExchMargin){ PositionDetail.ExchMargin=ExchMargin; bUpdate=true; }
}

//平仓成交时，根据持仓明细的变化，取持仓明细仓位变化值
void CPlatformDataMgr_PositionDetails::GetCloseDeltaVolume(const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,map<PositionDetailKey,int>& mapCloseDeltaVolume)
{
    CeasymutexGuard guard(m_mutex);

    PlatformStru_PositionDetail* pdetail;
    mapCloseDeltaVolume.clear();
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        if(m_PositionDetailData.GetValue_Internal(pdetail,it->first))
        {
            mapCloseDeltaVolume[it->first]=it->second.Volume-pdetail->Volume;
        }
    }
}

//开仓成交时及查询组合持仓明细完毕时，将组合合约ID设置到对应的持仓明细中。注意，可能一个持仓明细会对应多个组合持仓明细，用[ | ]连接在一起
void CPlatformDataMgr_PositionDetails::SetCombInstrumentIDs(const vector<PositionDetailCombKey>& combdetailkeys)
{
    CeasymutexGuard guard(m_mutex);

    string CombInstrumentID;
    PositionDetailKey key;
	map<PositionDetailKey,set<string>> key2CombInstrumentID;        //detail key - set of CombInstrumentID
    for(int i=0;i<(int)combdetailkeys.size();i++)
    {
        if(m_RefPositionDetailCombs.GetCombInstrumentID(combdetailkeys[i],CombInstrumentID))
        {
            Convert_PositionDetailCombKey_to_PositionDetailKey(combdetailkeys[i],key);
            map<PositionDetailKey,set<string>>::iterator it=key2CombInstrumentID.find(key);
            if(it==key2CombInstrumentID.end())
            {
                set<string> combinsts;
                combinsts.insert(CombInstrumentID);
                key2CombInstrumentID[key]=combinsts;
            }
            else 
            {
                set<string>& combinsts=it->second;
                if(combinsts.find(CombInstrumentID)==combinsts.end())
                    combinsts.insert(CombInstrumentID);
            }
        }
    }

    PlatformStru_PositionDetail *pdetail;
    for(map<PositionDetailKey,set<string>>::const_iterator it=key2CombInstrumentID.begin();it!=key2CombInstrumentID.end();it++)
    {
        if(m_PositionDetailData.GetValue_Internal(pdetail,it->first))
        {
            string combinst;
            const set<string>& combinsts=it->second;
            assert((int)combinsts.size()>=1);
            set<string>::const_iterator it2=combinsts.begin();
            combinst=*it2;
            for(it2++;it2!=combinsts.end();it2++)
            {
                combinst += string(" | ") + *it2;
            }

            memset(pdetail->CombInstrumentID,0,sizeof(pdetail->CombInstrumentID));
            strncpy(pdetail->CombInstrumentID,combinst.c_str(),sizeof(pdetail->CombInstrumentID)-1);
            pdetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pdetail->FTID);
        }
    }
}

//开仓成交时将组合合约ID设置到对应的持仓明细中. 根据组合持仓明细的变化
void CPlatformDataMgr_PositionDetails::SetCombInstrumentID_OpenTrade(const map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs)
{
    vector<PositionDetailCombKey> combkeys;
    for(map<PositionDetailCombKey,PlatformStru_PositionDetailComb>::const_iterator it=ModifiedPositionDetailCombs.begin();it!=ModifiedPositionDetailCombs.end();it++)
    {
        combkeys.push_back(it->first);
    }
    SetCombInstrumentIDs(combkeys);
}


//判断持仓明细是不是(平仓)成交的对应持仓明细. 依据是方向相反, 其它相同
bool CPlatformDataMgr_PositionDetails::IsPositionDetailForCloseTrade_IgnoreTradeType(const PlatformStru_PositionDetail& PositionDetail,
                                                                                     const PlatformStru_TradeInfo& TradeInfo)
{
    if(PositionDetail.Direction==TradeInfo.Direction) return false;
    if(PositionDetail.HedgeFlag!=TradeInfo.HedgeFlag) return false;

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday&&!PositionDetail.bToday) return false;
    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday&&PositionDetail.bToday) return false;
    return true;
}


//更新持仓明细记录的保证金, 保证金率, 查询保证金率响应时调用, 只对保证金为无效值的项目进行处理. 对于查询回来的持仓明细, 保证金类数据已经有了，不用再算. 
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                                   const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    string strInstrument(MarginRate.InstrumentID);
    map<string,set<long>>::iterator it_Index2;
    set<long>::iterator itFTID;
    map<long,PlatformStru_PositionDetail*>::iterator it_Info2;
    map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
    map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;

    m_mutex.lock();

    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        //针对合约对应的持仓明细，逐条计算保证金
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL&&
                util::isInvalidValue(it_Info2->second->Margin))
            {
                bool bUpdate;
                PlatformStru_PositionDetail oridetail=*(it_Info2->second);

                //计算持仓明细保证金
                CalculateMargin_Internal(MarginRate,InstrumentInfo,*(it_Info2->second),bUpdate);

                if(bUpdate) 
                {
                    it_Info2->second->UpdateSeq=m_PositionDetailData.NewUpdateSeq(it_Info2->second->FTID);
                    
                    PositionDetailKey key(oridetail);
                    if(ModifiedPositionDetails.find(key)!=ModifiedPositionDetails.end())
                        ModifiedPositionDetails[key]=oridetail;
                }
            }
        }
    }

    m_mutex.unlock();

    //针对有变化的持仓明细, 计算组合持仓明细的保证金优惠. 需要在更新持仓明细和更新组合持仓明细后进行. 
    CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(ModifiedPositionDetails,ModifiedPositionDetailCombs);
}

//取有效持仓明细(Volume>0). 返回记录数量, lastseq返回数据更新的序列号
int CPlatformDataMgr_PositionDetails::GetPositionDetails(vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    outData.clear();

    CeasymutexGuard guard(m_mutex);

	map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it= m_PositionDetailData.m_all.begin();
	for( ; it != m_PositionDetailData.m_all.end(); it++)
	{
        if ( it->second!=NULL && it->second->Volume>0)
            outData.push_back(*(it->second));
	}
    lastseq=m_PositionDetailData.GetLastUpdateSeq();

    return outData.size();
}

//取全部持仓明细, 包括数量为零的记录
void CPlatformDataMgr_PositionDetails::GetPositionDetails4(vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    outData.clear();

    CeasymutexGuard guard(m_mutex);

	map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it= m_PositionDetailData.m_all.begin();
	for( ; it != m_PositionDetailData.m_all.end(); it++)
	{
        if ( it->second!=NULL)
            outData.push_back(*(it->second));
	}
    lastseq=m_PositionDetailData.GetLastUpdateSeq();
}


//获取指定合约对应的有效持仓明细记录(Volume>0). 返回0: 成功, 并返回数据(可能是空数据)； 1: 成功, 但由于没有更新, 不返回数据
//如果lastseq和原始数据中的updateseq一致, 表示数据没有变化, 返回1, 不带具体数据; 
//否则返回0, setFTID是strInstrument对应的全部有效(仓位大于零)记录的FTID, 用于判断有没有需要删除的项; vecValue返回对应的有效数据数组
int CPlatformDataMgr_PositionDetails::GetPositionDetails3(const string& strInstrument,
                                                          set<long>& setFTID,
                                                          vector<PlatformStru_PositionDetail>& vecValue,
                                                          long& lastseq)
{
    setFTID.clear();
    vecValue.clear();

    CeasymutexGuard guard(m_mutex);

    if(lastseq==m_PositionDetailData.GetLastUpdateSeq())
    {
        return 1;
    }
    else
    {
        set<long>* pFTIDSet;
        if(m_PositionDetailData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            PlatformStru_PositionDetail *pdetail;
            set<long>::iterator itFTID;
            for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionDetailData.GetValue2_Internal(pdetail,(*itFTID))&&
                    pdetail->Volume>0)
                {
                    vecValue.push_back(*pdetail);
                    setFTID.insert(pdetail->FTID);
                }
            }
        }

        lastseq=m_PositionDetailData.GetLastUpdateSeq();
        return 0;
    }
}

//根据key取value
bool CPlatformDataMgr_PositionDetails::GetPositionDetail(const PositionDetailKey& key,PlatformStru_PositionDetail& value)
{
    CeasymutexGuard guard(m_mutex);

    return m_PositionDetailData.GetValue4_Internal(value,key);
}

//是否有指定合约的有效持仓明细记录
bool CPlatformDataMgr_PositionDetails::HavePositionDetail(const string& strInstrumentID)
{
    bool brlt=false;

    CeasymutexGuard guard(m_mutex);

    PlatformStru_PositionDetail *pdetail;
    map<string,set<long>>::const_iterator it_Index=m_PositionDetailData.m_allIndex2.find(strInstrumentID);
    if(it_Index!=m_PositionDetailData.m_allIndex2.end()&&!it_Index->second.empty())
    {
        for(set<long>::const_iterator it2=it_Index->second.begin();it2!=it_Index->second.end();it2++)
        {
            if(m_PositionDetailData.GetValue2_Internal(pdetail,*it2)&&pdetail->Volume>0)
            {
                brlt=true;
                break;
            }
        }
    }

    return brlt;
}

//清空查询持仓明细的map
void CPlatformDataMgr_PositionDetails::ClearMapQryRltPositionDetails(const string& strQryInstrument)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.WillQry_Internal(strQryInstrument);
}

//获取查询的合约ID
void CPlatformDataMgr_PositionDetails::GetQryPositionDetailInstrument(string& outQryInstrument)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.GetQryInstrument(outQryInstrument);
}

//计算指定[合约-账号-方向-投保]持仓明细的保证金之和
void CPlatformDataMgr_PositionDetails::GetMarginByInstrument(const string& strInstrumentID,
                                                               const string& strAccount,
                                                               char Direction,
                                                               char HedgeFlag,
                                                               double& outMargin,
                                                               double& outExchMargin)
{
    outMargin=0;
    outExchMargin=0;

    CeasymutexGuard guard(m_mutex);

    //从所有持仓明细中计算保证金
    map<string,set<long>>::iterator it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_PositionDetail*>::iterator it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2==m_PositionDetailData.m_all2.end()) continue;
            if(it_Info2->second==NULL) continue;
		    if(string(it_Info2->second->Account)!=strAccount) continue;
            if(it_Info2->second->Direction!=Direction) continue;
            if(it_Info2->second->HedgeFlag!=HedgeFlag) continue;
            if(it_Info2->second->TradeType==THOST_FTDC_TRDT_CombinationDerived) continue;

            if(it_Info2->second->Volume>0)
            {
                if(!util::isInvalidValue(outMargin))
                {
                    if(!util::isInvalidValue(it_Info2->second->Margin)) outMargin+=it_Info2->second->Margin;
                    else outMargin=util::GetDoubleInvalidValue();
                }
                if(!util::isInvalidValue(outExchMargin))
                {
                    if(!util::isInvalidValue(it_Info2->second->ExchMargin)) outExchMargin+=it_Info2->second->ExchMargin;
                    else outExchMargin=util::GetDoubleInvalidValue();
                }
                if(util::isInvalidValue(outMargin)&&util::isInvalidValue(outExchMargin)) break;
            }
        }
    }
}


//是否有组合持仓明细数据(TradeType为组合衍生的数据)
bool CPlatformDataMgr_PositionDetails::HaveCombPositionDetail(void)
{
    return m_bHaveComb;
}

//获取平仓时打散衍生的持仓明细合约iD
void CPlatformDataMgr_PositionDetails::GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs)
{
    CeasymutexGuard guard(m_mutex);

    InstrumentIDs=m_DerivedInstrumentIDs_OnCloseTrade;
}

///获取查询结果的合约集合
void CPlatformDataMgr_PositionDetails::GetInstrumentSetOfQryRlt(set<string>& outData)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.GetInstrumentSetOfQryRlt(outData);
}

void CPlatformDataMgr_PositionDetails::clear(void)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.Clear();
    m_mapOpenTime2Key.clear();
    m_bHaveComb=false;
}
bool CPlatformDataMgr_PositionDetails::empty(void)
{
    CeasymutexGuard guard(m_mutex);

    return m_PositionDetailData.empty();
}


//从m_mapOpenTime2Key中取出指定时间范围的PositionDetail
//时间是time_t秒数, LowerOpenTime<=结果OpenTimeUTC<UpperOpenTime. 昨仓持仓明细的OpenTimeUTC为开仓日期零时; 今仓是准确的
//持仓明细的OpenTimeUTC=CTools_Ansi::Get_time_t_utc_from_strTime(OpenDate,OpenTime)
void CPlatformDataMgr_PositionDetails::GetPositionDetailsInOpenTimeScope(int LowerOpenTime,int UpperOpenTime,vector<PlatformStru_PositionDetail>& outPositionDetails)
{
    CeasymutexGuard guard(m_mutex);

    outPositionDetails.clear();
    vector<PositionDetailKey> keys;
    PlatformStru_PositionDetail *pdetail;
    GetKeysFrommapOpenTime2Key_Internal(LowerOpenTime,UpperOpenTime,keys);
    for(int i=0;i<(int)keys.size();i++)
      if(m_PositionDetailData.GetValue_Internal(pdetail,keys[i]))
          outPositionDetails.push_back(*pdetail);
}

//从m_mapOpenTime2Key中取出指定开仓时间范围的key
void CPlatformDataMgr_PositionDetails::GetKeysFrommapOpenTime2Key_Internal(int LowerOpenTime,int UpperOpenTime,vector<PositionDetailKey>& outkeys)
{
	multimap<int,PositionDetailKey>::iterator it_upper,it_lower;
    it_lower=m_mapOpenTime2Key.lower_bound(LowerOpenTime);
    it_upper=m_mapOpenTime2Key.lower_bound(UpperOpenTime);
    while(it_lower!=m_mapOpenTime2Key.end()&&it_lower!=it_upper)
    {
        outkeys.push_back(it_lower->second);
	    it_lower++;
    }
}


//获取所有持仓明细记录的FTID
void CPlatformDataMgr_PositionDetails::GetAllFTID(set<long>& outFTIDs)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.GetAllFTID_Internal(outFTIDs);
}

//根据组合持仓明细key，生成持仓明细key
void CPlatformDataMgr_PositionDetails::Convert_PositionDetailCombKey_to_PositionDetailKey(const PositionDetailCombKey& combkey,PositionDetailKey& detailkey)
{
    detailkey.set(combkey.Account,combkey.InstrumentID,combkey.OpenDate,combkey.TradeID,combkey.Direction,THOST_FTDC_TRDT_CombinationDerived,combkey.HedgeFlag);
}
