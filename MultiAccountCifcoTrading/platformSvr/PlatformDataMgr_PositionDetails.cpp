//维护底层持仓明细信息

#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#pragma warning(disable:4996)

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
        char logbuf[512],logbuf2[512];\
        logbuf[0]=1;  logbuf2[0]=1;\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr-PositionDetail", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


//查询持仓明细记录的结果到来。注意，可能是查询指定合约的持仓明细
void CPlatformDataMgr::QryPositionDetailReach(PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryPositionDetailReach:InstrumentID=%s",PositionDetailInfo.InstrumentID);

    std::set<string> setInstruments;
    std::set<string>::iterator it_Instrument;

    Lock_CPlatformDataMgr;

    //需要过滤掉Volume为零的持仓明细
    if(PositionDetailInfo.Volume==0)
    {
        if(ErrID==0) ErrID=-1;
    }

    if(ErrID==0)
	    m_tempOriPositionDetail[PositionDetailInfo.TradeID] = PositionDetailInfo;

    if(ErrID==0)
    {
        //设置对应的PriceTick
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(GetInstrumentInfo(std::string(PositionDetailInfo.InstrumentID),InstrumentInfo)==0)
        {
            PositionDetailInfo.PriceTick=InstrumentInfo.PriceTick;
        }
        PositionDetailInfo.bToday=IsTodayPosition( PositionDetailInfo.OpenDate);
    }

    m_PositionDetailData.QryRltReach_Internal(PositionDetailInfo,ErrID,bIsLast);

    if(bIsLast)
    {
        m_PositionDetailData.GetInstrumentSetOfQryRlt(setInstruments);
    }

    Unlock_CPlatformDataMgr;

    if(bIsLast&&!setInstruments.empty())
    {
        PlatformStru_DepthMarketData QuotData;
        //动态更新持仓明细的持仓盈亏和浮动盈亏
        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
            PlatformStru_InstrumentInfo InstrumentInfo;
            if(GetInstrumentInfo(*it_Instrument,InstrumentInfo)==0&&
                GetQuotInfo(*it_Instrument,QuotData)==0)
            {
               UpdatePositionDetailByQuot(QuotData,InstrumentInfo,true);
            }
        }

        //触发取费率
        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
			if ( m_PlatFormService )
			{
				m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(*it_Instrument);
				m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(*it_Instrument);
			}
        }
    }

    LOG_INFO("CPlatformDataMgr::QryPositionDetailReach-end");

}

//取出持仓明细中的记录总数
int CPlatformDataMgr::GetRecordCountOfPositionDetail(void)
{
    int count;

    Lock_CPlatformDataMgr;

    count = m_PositionDetailData.m_all.size();

    Unlock_CPlatformDataMgr;

    return count;
}

//从成交记录更新持仓明细
//返回平掉的持仓明细的记录集，用于后续计算
//ModifiedPositionDetails保存受到影响的持仓明细记录，n*2+0是原来的数据，n*2+1是新数据
void CPlatformDataMgr::UpdatePositionDetail_RtnTrade(PlatformStru_TradeInfo& TradeInfo,
                                                     std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                     const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade(成交回报更新持仓明细):InstrumentID=%s",TradeInfo.InstrumentID);

    std::string strInstrument=std::string(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    Lock_CPlatformDataMgr;


    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {                                                   
		PlatformStru_PositionDetail PositionDetailInfo;

//#ifdef _USE_COMBOINSTRUMENT
	    //组合单开仓时，创建组合持仓明细记录
	    if (TradeInfo.TradeType == THOST_FTDC_TRDT_CombinationDerived)
	    {
		    CreatePositionDetailCombFromOpenTrade_Internal(TradeInfo);
	    }
//#endif
        CreatePositionDetailFromOpenTrade_Internal(PositionDetailInfo,TradeInfo);

        //动态更新持仓明细的持仓盈亏和浮动盈亏
	    PlatformStru_DepthMarketData QuotInfo;
	    if(GetQuotInfo(std::string(TradeInfo.InstrumentID),QuotInfo)==0)
            UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetailInfo,InstrumentInfo,true); 

        //动态更新持仓明细记录的保证金、保证金率，成交修改持仓明细和查询保证金率响应时调用
	    PlatformStru_InstrumentMarginRate MarginRate;
	    if(GetMarginRate_Internal(TradeInfo.InstrumentID,MarginRate)==0)
            UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetailInfo,InstrumentInfo);
        else
        {
            LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade (开仓情况)取保证金率失败， InstrumentID=%s",TradeInfo.InstrumentID);
        }

        if(PositionDetailInfo.Volume>0)
        {
            //设置对应的PriceTick
            PositionDetailInfo.PriceTick=InstrumentInfo.PriceTick;
            PositionDetailInfo.bToday=true;

            m_PositionDetailData.NewReach_Internal(PositionDetailInfo);
            LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade(开仓生成持仓明细):%s",PositionDetailInfo.tostring(logbuf,sizeof(logbuf)));

            //保存影响到的持仓明细记录
            ModifiedPositionDetails.push_back(PlatformStru_PositionDetail::GetInvalidRecord());
            ModifiedPositionDetails.push_back(PositionDetailInfo);
        }
    }
    else if( TradeInfo.OffsetFlag==THOST_FTDC_OF_Close||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_ForceClose||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
    { 
        UpdatePositionDetailFromCloseTrade_Internal(TradeInfo,ModifiedPositionDetails,InstrumentInfo);
    }

	//检查被改变的持仓明细，如果被全部平掉，删除之
	int count=ModifiedPositionDetails.size();
	for(int i=0;i<count/2;i++)
	{
		if(ModifiedPositionDetails[i*2+1].Volume==0)
		{
			PositionDetailKey key(ModifiedPositionDetails[i*2+1]);
            LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade(删除全平掉的持仓明细):%s",key.tostring(logbuf,sizeof(logbuf)));
			m_PositionDetailData.DeleteRecord2_Internal(key);
			m_PositionDetailData.NewUpdateSeq();
		}
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade-end");
}

//从开仓成交记录创建持仓明细，盈亏和保证金在后面更新
void CPlatformDataMgr::CreatePositionDetailFromOpenTrade_Internal(PlatformStru_PositionDetail& PositionDetailInfo,PlatformStru_TradeInfo& TradeInfo)
{
    std::string strInstrument=std::string(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    if(TradeInfo.OffsetFlag!=THOST_FTDC_OF_Open)
        return;

    PositionDetailInfo.Clear();

	strncpy(PositionDetailInfo.szAccount,TradeInfo.szAccount,sizeof(PositionDetailInfo.szAccount)-1);
    strncpy(PositionDetailInfo.InstrumentID,TradeInfo.InstrumentID,sizeof(PositionDetailInfo.InstrumentID)-1);
    strncpy(PositionDetailInfo.BrokerID,TradeInfo.BrokerID,sizeof(PositionDetailInfo.BrokerID)-1);
    strncpy(PositionDetailInfo.InvestorID,TradeInfo.InvestorID,sizeof(PositionDetailInfo.InvestorID)-1);
    PositionDetailInfo.HedgeFlag=TradeInfo.HedgeFlag;
    PositionDetailInfo.Direction=TradeInfo.Direction;
    strncpy(PositionDetailInfo.OpenDate,TradeInfo.TradeDate,sizeof(PositionDetailInfo.OpenDate)-1);
    strncpy(PositionDetailInfo.TradeID,TradeInfo.TradeID,sizeof(PositionDetailInfo.TradeID)-1);
    PositionDetailInfo.Volume=TradeInfo.Volume;
    PositionDetailInfo.OpenPrice=TradeInfo.Price;
    strncpy(PositionDetailInfo.TradingDay,TradeInfo.TradingDay,sizeof(PositionDetailInfo.TradingDay)-1);
    PositionDetailInfo.SettlementID=TradeInfo.SettlementID;
    PositionDetailInfo.TradeType=TradeInfo.TradeType;
    strncpy(PositionDetailInfo.ExchangeID,TradeInfo.ExchangeID,sizeof(PositionDetailInfo.ExchangeID)-1);
    PositionDetailInfo.CloseProfitByDate=0;
    PositionDetailInfo.CloseProfitByTrade=0;
    PositionDetailInfo.PositionProfitByDate=0;
    PositionDetailInfo.PositionProfitByTrade=0;

    PositionDetailInfo.Margin=util::GetDoubleInvalidValue();
    PositionDetailInfo.MarginRateByMoney=util::GetDoubleInvalidValue();
    PositionDetailInfo.MarginRateByVolume=util::GetDoubleInvalidValue();
    PositionDetailInfo.ExchMargin=util::GetDoubleInvalidValue();
    PositionDetailInfo.LastSettlementPrice=util::GetDoubleInvalidValue();
    PositionDetailInfo.SettlementPrice=util::GetDoubleInvalidValue();
    PositionDetailInfo.CloseVolume=0;
    PositionDetailInfo.CloseAmount=0;
}

//从平仓成交记录更新持仓明细，ModifiedPositionDetails保存受到影响的持仓明细记录，first是原来的数据，second是新数据
void CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                                   std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                   const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool bHaveMarginRateInfo=false;
    bool bHaveQuot=false;

    if( TradeInfo.OffsetFlag!=THOST_FTDC_OF_Close&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_ForceClose&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_CloseToday&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_CloseYesterday&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_LocalForceClose)
        return;

    std::string strInstrument=std::string(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    int nVolumeMultiple=InstrumentInfo.VolumeMultiple;

	PlatformStru_InstrumentMarginRate MarginRate;
	if(GetMarginRate_Internal(strInstrument,MarginRate)==0)
        bHaveMarginRateInfo=true;

	PlatformStru_DepthMarketData QuotInfo;
	if(GetQuotInfo(strInstrument,QuotInfo)==0)
        bHaveQuot=true;


	//处理平仓
    //根据TradeInfo的合约、开平标志、买卖方向，平掉相应的仓位，修正其中的数量、盈亏、保证金、金额等项
    int CloseVolume=TradeInfo.Volume;


    //循环处理，直到平仓手数处理完
	//第一遍，先处理单一持仓

    //注意：std::set<PositionDetailKey>中是按时间排序的
    std::map<std::string,std::set<PositionDetailKey>>::iterator it_Index;
    std::set<PositionDetailKey>::iterator itKey;
    std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it_PositionDetail;

    it_Index=m_PositionDetailData.m_allIndex.find(strInstrument);
    if(it_Index!=m_PositionDetailData.m_allIndex.end()&&!it_Index->second.empty())
    {
        //第一步，对于普通平仓成交，从普通持仓明细中平仓
        if(TradeInfo.TradeType!=THOST_FTDC_TRDT_CombinationDerived&&CloseVolume>0)
        {
            for(itKey=it_Index->second.begin() ; CloseVolume>0 && itKey!=it_Index->second.end() ; itKey++)
            {
		        it_PositionDetail=m_PositionDetailData.m_all.find(*itKey);
		        if(it_PositionDetail==m_PositionDetailData.m_all.end()) continue;
                if(it_PositionDetail->second==NULL) continue;

                PlatformStru_PositionDetail& PositionDetail = *(it_PositionDetail->second);
                if(!IsOpponentPositionDetailOfTrade_IgnoreTradeType(PositionDetail,TradeInfo)) continue;
                if(PositionDetail.TradeType==THOST_FTDC_TRDT_CombinationDerived) continue;

                PlatformStru_PositionDetail OldPositionDetail=PositionDetail;

                CloseVolume = CloseSinglePosition_Internal(CloseVolume, PositionDetail, nVolumeMultiple, TradeInfo.Price);
                LOG_INFO("CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(第一遍平仓):Old:%s New:%s",OldPositionDetail.tostring(logbuf,sizeof(logbuf)),PositionDetail.tostring(logbuf2,sizeof(logbuf2)));

		        //更新持仓明细的持仓盈亏和浮动盈亏
		        if(bHaveQuot) UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetail,InstrumentInfo,true); 
		        //更新持仓明细记录的保证金、保证金率
		        if(bHaveMarginRateInfo) UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetail,InstrumentInfo);

                //记录更新前后的持仓明细数据
                ModifiedPositionDetails.push_back(OldPositionDetail);
                ModifiedPositionDetails.push_back(PositionDetail);
            }
        }

        //第二步，对于普通平仓成交，从组合持仓明细中打散平仓
        if(TradeInfo.TradeType!=THOST_FTDC_TRDT_CombinationDerived&&CloseVolume>0)
        {
            for(itKey=it_Index->second.begin() ; CloseVolume>0 && itKey!=it_Index->second.end() ; itKey++)
            {
		        it_PositionDetail=m_PositionDetailData.m_all.find(*itKey);
		        if(it_PositionDetail==m_PositionDetailData.m_all.end()) continue;
                if(it_PositionDetail->second==NULL) continue;

                PlatformStru_PositionDetail& PositionDetail = *(it_PositionDetail->second);
                if(!IsOpponentPositionDetailOfTrade_IgnoreTradeType(PositionDetail,TradeInfo)) continue;
                if(PositionDetail.TradeType!=THOST_FTDC_TRDT_CombinationDerived) continue;

                //获取对手仓位
                PlatformStru_PositionDetail& OppoPositionDetail=GetOpponentPositionDetailComb(PositionDetail);
                if(OppoPositionDetail.IsInvalidRecord()) continue;

                PlatformStru_PositionDetail OldPositionDetail=PositionDetail;

                CloseVolume = CloseSinglePosition_Internal(CloseVolume, PositionDetail, nVolumeMultiple, TradeInfo.Price);
                LOG_INFO("CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(第二遍平仓):Old:%s New:%s",OldPositionDetail.tostring(logbuf,sizeof(logbuf)),PositionDetail.tostring(logbuf2,sizeof(logbuf2)));


		        //更新持仓明细的持仓盈亏和浮动盈亏
		        if(bHaveQuot) UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetail,InstrumentInfo,true); 
		        //更新持仓明细记录的保证金、保证金率
		        if(bHaveMarginRateInfo) UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetail,InstrumentInfo);

                //记录更新前后的持仓明细数据
                ModifiedPositionDetails.push_back(OldPositionDetail);
                ModifiedPositionDetails.push_back(PositionDetail);

                //因为是处理组合，一腿被平，另一腿(对手仓位)要打散
                int DeltaVolume=OldPositionDetail.Volume - PositionDetail.Volume;
                BreakupPositionDetailComb(OppoPositionDetail,DeltaVolume,ModifiedPositionDetails);
            }
        }

        //第三步，对于组合衍生成交，从组合持仓明细中平仓
        if(TradeInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived&&CloseVolume>0)
        {
            for(itKey=it_Index->second.begin() ; CloseVolume>0 && itKey!=it_Index->second.end() ; itKey++)
            {
		        it_PositionDetail=m_PositionDetailData.m_all.find(*itKey);
		        if(it_PositionDetail==m_PositionDetailData.m_all.end()) continue;
                if(it_PositionDetail->second==NULL) continue;

                PlatformStru_PositionDetail& PositionDetail = *(it_PositionDetail->second);
                if(!IsOpponentPositionDetailOfTrade_IgnoreTradeType(PositionDetail,TradeInfo)) continue;
                if(PositionDetail.TradeType!=THOST_FTDC_TRDT_CombinationDerived) continue;

                PlatformStru_PositionDetail OldPositionDetail=PositionDetail;

                CloseVolume = CloseSinglePosition_Internal(CloseVolume, PositionDetail, nVolumeMultiple, TradeInfo.Price);
                LOG_INFO("CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(第三遍平仓):Old:%s New:%s",OldPositionDetail.tostring(logbuf,sizeof(logbuf)),PositionDetail.tostring(logbuf2,sizeof(logbuf2)));

		        //更新持仓明细的持仓盈亏和浮动盈亏
		        if(bHaveQuot) UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetail,InstrumentInfo,true); 
		        //更新持仓明细记录的保证金、保证金率
		        if(bHaveMarginRateInfo) UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetail,InstrumentInfo);

                //记录更新前后的持仓明细数据
                ModifiedPositionDetails.push_back(OldPositionDetail);
                ModifiedPositionDetails.push_back(PositionDetail);
            }
        }
    }

}

//动态更新持仓明细记录的持仓盈亏、浮动盈亏、结算价、昨结算价、现价，根据行情，得到新行情时做一遍
//新行情更新bForceCalculate为false，表示仅在价格变动时更新，其它情况为true
void CPlatformDataMgr::UpdatePositionDetailByQuot(PlatformStru_DepthMarketData& QuotData,const PlatformStru_InstrumentInfo& InstrumentInfo,bool bForceCalculate) 
{
    std::string strInstrument(QuotData.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_PositionDetail*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionDetailByQuot_Internal(QuotData,*(it_Info2->second),InstrumentInfo,bForceCalculate);
            }
        }
    }

    Unlock_CPlatformDataMgr;

}

//动态更新持仓明细记录的持仓盈亏(盯市)、浮动盈亏(逐笔)、结算价、昨结算价、现价，根据行情
//新行情更新bForceCalculate为false，表示仅在价格变动时更新，其它情况为true
void CPlatformDataMgr::UpdatePositionDetailByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                                           PlatformStru_PositionDetail& PositionDetail,
                                                           const PlatformStru_InstrumentInfo InstrumentInfo,
                                                           bool bForceCalculate) 
{
    bool bTodayPositionDetail;

    std::string strInstrument(QuotData.InstrumentID);

    double CurPriceUsedByCalcu=QuotData.Volume>0?QuotData.LastPrice:QuotData.PreSettlementPrice;

    if(CurPriceUsedByCalcu==util::GetDoubleInvalidValue())
        return;

    //非强制计算时，如价格未更新，不进行计算
    if(!bForceCalculate&&
        CurPriceUsedByCalcu==PositionDetail.CurPriceUsedByCalcu)
        return;

    if(strInstrument!=std::string(PositionDetail.InstrumentID))
        return;

    double PositionProfitByDate=PositionDetail.PositionProfitByDate;
    double PositionProfitByTrade=PositionDetail.PositionProfitByTrade;
    double LastSettlementPrice=PositionDetail.LastSettlementPrice;
    double SettlementPrice=PositionDetail.SettlementPrice;


    //计算持仓盈亏(盯市)
    bTodayPositionDetail=IsTodayPositionDetail(PositionDetail);
    if(bTodayPositionDetail)
    {
        if(PositionDetail.Direction==THOST_FTDC_D_Buy)
            PositionProfitByDate=(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //逐日盯市持仓盈亏
        else
            PositionProfitByDate=-1*(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //逐日盯市持仓盈亏
    }
    else 
    {
        if(PositionDetail.Direction==THOST_FTDC_D_Buy)
            PositionProfitByDate=(CurPriceUsedByCalcu-PositionDetail.LastSettlementPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;   //逐日盯市持仓盈亏
        else
            PositionProfitByDate=-1*(CurPriceUsedByCalcu-PositionDetail.LastSettlementPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;   //逐日盯市持仓盈亏
    }
    if(PositionProfitByDate==-0) PositionProfitByDate=0;


    //计算持仓盈亏(浮动即逐笔)
    if(PositionDetail.Direction==THOST_FTDC_D_Buy)
        PositionProfitByTrade=(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //触笔对冲持仓盈亏
    else
        PositionProfitByTrade=-1*(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //触笔对冲持仓盈亏
    if(PositionProfitByTrade==-0) PositionProfitByTrade=0;

    if(LastSettlementPrice==util::GetDoubleInvalidValue())
        LastSettlementPrice=QuotData.PreSettlementPrice;
    if(SettlementPrice==util::GetDoubleInvalidValue())
        SettlementPrice=QuotData.SettlementPrice;


    bool bUpdate=false;
    if( PositionProfitByDate!=PositionDetail.PositionProfitByDate){ PositionDetail.PositionProfitByDate=PositionProfitByDate; bUpdate=true; }
    if( PositionProfitByTrade!=PositionDetail.PositionProfitByTrade){ PositionDetail.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
    if( LastSettlementPrice!=PositionDetail.LastSettlementPrice){ PositionDetail.LastSettlementPrice=LastSettlementPrice; bUpdate=true; }
    if( SettlementPrice!=PositionDetail.SettlementPrice){ PositionDetail.SettlementPrice=SettlementPrice; bUpdate=true; }
    if( PositionDetail.CurPrice!=QuotData.LastPrice) { PositionDetail.CurPrice=QuotData.LastPrice;  bUpdate=true;}
    if( CurPriceUsedByCalcu!=PositionDetail.CurPriceUsedByCalcu) { PositionDetail.CurPriceUsedByCalcu=CurPriceUsedByCalcu; bUpdate=true; }
    if(bUpdate)
        PositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();
}


//动态更新持仓明细记录的保证金、保证金率，查询保证金率响应时调用，只对今仓处理
bool CPlatformDataMgr::UpdatePositionDetailByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate,
                                                        const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    bool brlt=false;
    std::string strInstrument(MarginRate.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_PositionDetail*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionDetailByMarginRate_Internal(MarginRate,*(it_Info2->second),InstrumentInfo);
                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//动态更新持仓明细记录的保证金、保证金率，成交修改持仓明细和查询保证金率响应时调用，只对今仓处理
void CPlatformDataMgr::UpdatePositionDetailByMarginRate_Internal(PlatformStru_InstrumentMarginRate& MarginRate,
                                                                 PlatformStru_PositionDetail& PositionDetail,
                                                                 const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    //已经算过了，不再算
    //if(PositionDetail.Margin!=util::GetDoubleInvalidValue())
    //    return;

    std::string strInstrument(PositionDetail.InstrumentID);
    if(strInstrument.empty())
        return;

	double dPrice;
    if(!IsTodayPositionDetail(PositionDetail))
	{
        dPrice=PositionDetail.LastSettlementPrice;
	}
	else
	{
		dPrice=PositionDetail.OpenPrice;
	}

    double Margin=PositionDetail.Margin;
    double MarginRateByMoney=PositionDetail.MarginRateByMoney;
    double MarginRateByVolume=PositionDetail.MarginRateByVolume;

    if(PositionDetail.Direction==THOST_FTDC_D_Buy)
    {
        Margin=  PositionDetail.Volume*
                                InstrumentInfo.VolumeMultiple*
                                dPrice*
                                MarginRate.LongMarginRatioByMoney+
                                PositionDetail.Volume*
                                MarginRate.LongMarginRatioByVolume;
        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else
    {
        Margin=  PositionDetail.Volume*
                                InstrumentInfo.VolumeMultiple*
                                dPrice*
                                MarginRate.ShortMarginRatioByMoney+
                                PositionDetail.Volume*
                                MarginRate.ShortMarginRatioByVolume;
        MarginRateByMoney=MarginRate.ShortMarginRatioByMoney;
        MarginRateByVolume=MarginRate.ShortMarginRatioByVolume;
    }

    bool bUpdate=false;
    if( PositionDetail.Margin!=Margin){ PositionDetail.Margin=Margin; bUpdate=true; }
    if( PositionDetail.MarginRateByMoney!=MarginRateByMoney){ PositionDetail.MarginRateByMoney=MarginRateByMoney; bUpdate=true; }
    if( PositionDetail.MarginRateByVolume!=MarginRateByVolume){ PositionDetail.MarginRateByVolume=MarginRateByVolume; bUpdate=true; }
    if(bUpdate)
        PositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();

}




int CPlatformDataMgr::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it= m_PositionDetailData.m_all.begin();
	for( ; it != m_PositionDetailData.m_all.end(); it++)
	{
        if ( it->second!=NULL &&
             it->second->Volume > 0)
		{
            outData.push_back(*(it->second));
		}
	}
    lastseq=m_PositionDetailData.GetUpdateSeq();

    Unlock_CPlatformDataMgr;

    return outData.size();
}



//返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
int CPlatformDataMgr::GetPositionDetails3(const std::string& strInstrument,
                                          std::set<long>& setFTID,
                                          std::vector<PlatformStru_PositionDetail>& vecValue,
                                          long& lastseq)
{
    int rlt;

    setFTID.clear();
    vecValue.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionDetailData.GetUpdateSeq())
    {
        rlt=1;
    }
    else
    {
        std::set<long>* pFTIDSet;
        if(m_PositionDetailData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            setFTID=(*pFTIDSet);

            PlatformStru_PositionDetail *pData;
            std::set<long>::iterator itFTID;
            for(itFTID=setFTID.begin();itFTID!=setFTID.end();itFTID++)
            {
                if(m_PositionDetailData.GetValue2_Internal(pData,(*itFTID)))
                    vecValue.push_back(*pData);
            }
        }

        lastseq=m_PositionDetailData.GetUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

bool CPlatformDataMgr::HavePositionDetail(const std::string& strInstrumentID)
{
    bool brlt=false;
    std::map<std::string,std::set<PositionDetailKey>>::iterator it_Index;
    std::set<PositionDetailKey>::iterator itKey;

    Lock_CPlatformDataMgr;

    it_Index=m_PositionDetailData.m_allIndex.find(strInstrumentID);
    if(it_Index!=m_PositionDetailData.m_allIndex.end()&&!it_Index->second.empty())
        brlt=true;

    Unlock_CPlatformDataMgr;

    return brlt;
}

//清空查询持仓明细的map
void CPlatformDataMgr::ClearMapQryRltPositionDetails(std::string& strQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionDetailData.WillQry_Internal(strQryInstrument);

    Unlock_CPlatformDataMgr;

}

//获取查询的合约ID
void CPlatformDataMgr::GetQryPositionDetailInstrument(std::string& outQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionDetailData.GetQryInstrument(outQryInstrument);

    Unlock_CPlatformDataMgr;

}

int CPlatformDataMgr::CloseSinglePosition_Internal( int CloseVolume, PlatformStru_PositionDetail &positionDetail, 
                                           int nVolumeMultiple, double dPrice)
{

	int thisVolume=CloseVolume;
	if(thisVolume>positionDetail.Volume) thisVolume=positionDetail.Volume;

	//更新仓位手数
	positionDetail.Volume-=thisVolume;
	CloseVolume-=thisVolume;

	//更新平仓量
	positionDetail.CloseVolume+=thisVolume;                                            
	//更新平仓金额
	positionDetail.CloseAmount+=thisVolume*nVolumeMultiple*dPrice;

	//更新平仓盈亏(盯市)
	bool bTodayPositionDetail = IsTodayPosition(positionDetail.OpenDate);
	if(bTodayPositionDetail)
	{
		if(positionDetail.Direction==THOST_FTDC_D_Buy)
			positionDetail.CloseProfitByDate+=(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                  //逐日盯市平仓盈亏
		else
			positionDetail.CloseProfitByDate+=-1*(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                  //逐日盯市平仓盈亏
	}
	else 
	{
		if(positionDetail.Direction==THOST_FTDC_D_Buy)
			positionDetail.CloseProfitByDate+=(dPrice-positionDetail.LastSettlementPrice)*thisVolume*nVolumeMultiple;                //逐日盯市平仓盈亏
		else
			positionDetail.CloseProfitByDate+=-1*(dPrice-positionDetail.LastSettlementPrice)*thisVolume*nVolumeMultiple;                //逐日盯市平仓盈亏
	}
	if(positionDetail.CloseProfitByDate==-0) positionDetail.CloseProfitByDate=0;

	//更新平仓盈亏(逐笔)
	if(positionDetail.Direction==THOST_FTDC_D_Buy)
		positionDetail.CloseProfitByTrade+=(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                 //逐笔对冲平仓盈亏
	else
		positionDetail.CloseProfitByTrade+=-1*(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                 //逐笔对冲平仓盈亏
	if(positionDetail.CloseProfitByTrade==-0) positionDetail.CloseProfitByTrade=0;


	positionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();	
	return CloseVolume;
}
//判断持仓明细是不是(平仓)成交的对手持仓
bool CPlatformDataMgr::IsOpponentPositionDetailOfTrade_IgnoreTradeType(PlatformStru_PositionDetail& PositionDetail,PlatformStru_TradeInfo& TradeInfo)
{
    if(PositionDetail.Volume<=0) return false;
    if(PositionDetail.Direction==TradeInfo.Direction) return false;
    if(PositionDetail.HedgeFlag!=TradeInfo.HedgeFlag) return false;

    bool curbTodayPositionDetail=IsTodayPositionDetail(PositionDetail);
    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday&&!curbTodayPositionDetail) return false;
    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday&&curbTodayPositionDetail) return false;
    return true;
}

//获取某一个(组合)持仓明细的对手持仓明细记录，注意，只找最早的配对今仓或昨仓
//用于成交更新持仓明细时打散组合持仓的计算。获取时用到组合持仓明细的数据
PlatformStru_PositionDetail& CPlatformDataMgr::GetOpponentPositionDetailComb(const PlatformStru_PositionDetail& OriPositionDetail)
{
    std::string OriTradeID(std::string(OriPositionDetail.TradeID));
    std::string OppoTradeID;
    std::string OppoInstrumentID;
    char OppoDirection;

    //获取对应的组合持仓明细数据
    PlatformStru_PositionComb& PositionDetailComb=GetPositionComb3_Internal(OriTradeID);
    if(PositionDetailComb.IsInvalidRecord())
        return PlatformStru_PositionDetail::GetInvalidRecord();

    if(OriTradeID==std::string(PositionDetailComb.Leg1TradeID))
    {
        OppoTradeID=std::string(PositionDetailComb.Leg2TradeID);
        OppoInstrumentID=std::string(PositionDetailComb.Leg2InstrumentID);
    }
    else 
    {
        OppoTradeID=std::string(PositionDetailComb.Leg1TradeID);
        OppoInstrumentID=std::string(PositionDetailComb.Leg1InstrumentID);
    }
    if(OriPositionDetail.Direction==THOST_FTDC_D_Buy) OppoDirection=THOST_FTDC_D_Sell;
    else OppoDirection=THOST_FTDC_D_Buy;

    PositionDetailKey OppoKey(OriPositionDetail.szAccount, 
                                OppoInstrumentID.c_str(),
                                OriPositionDetail.OpenDate,
                                OppoTradeID.c_str(),
                                OppoDirection, 
                                OriPositionDetail.HedgeFlag,
                                OriPositionDetail.TradeType);

    std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it_PositionDetail;

    it_PositionDetail=m_PositionDetailData.m_all.find(OppoKey);
    if(it_PositionDetail!=m_PositionDetailData.m_all.end()&&it_PositionDetail->second!=NULL) 
        return *(it_PositionDetail->second);
    else return PlatformStru_PositionDetail::GetInvalidRecord();
}


//普通成交(平仓)需要平组合单时，打散对手持仓明细
//1. 生成或更新衍生的(普通)仓位，数量加上ClosedVolume
//2. 更新对手(组合)仓位，数量减去ClosedVolume
bool CPlatformDataMgr::BreakupPositionDetailComb(PlatformStru_PositionDetail&OppoPositionDetail,
                                                 int ClosedVolume,
                                                 std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails)
{
    if(ClosedVolume<=0) return false;
    if(OppoPositionDetail.TradeType!=THOST_FTDC_TRDT_CombinationDerived) return false;
    if(OppoPositionDetail.Volume<ClosedVolume) return false;

    const double dInvalidDouble=util::GetIntInvalidValue();

    //衍生(被打散成普通)持仓明细的key
    PositionDetailKey DerivedKey(OppoPositionDetail);
    DerivedKey.TradeType=THOST_FTDC_TRDT_Common;

    std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it_DerivedPositionDetail;
    it_DerivedPositionDetail=m_PositionDetailData.m_all.find(DerivedKey);
	if(it_DerivedPositionDetail==m_PositionDetailData.m_all.end()) 
    {
        //从OppoPositionDetail创建空的NewPositionDetail
        PlatformStru_PositionDetail NewPositionDetail=OppoPositionDetail;

        NewPositionDetail.Volume=0;
        NewPositionDetail.TradeType=THOST_FTDC_TRDT_Common;
        memset(NewPositionDetail.CombInstrumentID,0,sizeof(NewPositionDetail.CombInstrumentID));
        NewPositionDetail.CloseProfitByDate=0;
        NewPositionDetail.CloseProfitByTrade=0;
        NewPositionDetail.PositionProfitByDate=0;
        NewPositionDetail.PositionProfitByTrade=0;
        NewPositionDetail.Margin=0;
        NewPositionDetail.ExchMargin=0;
        NewPositionDetail.CloseVolume=0;
        NewPositionDetail.CloseAmount=0;

        NewPositionDetail.nBuyVolumn=0;
        NewPositionDetail.nSellVolumn=0;
        NewPositionDetail.dOpenAvgPrice=0;
        NewPositionDetail.dOpenProfit=0;
        NewPositionDetail.dTotalProfit=0;
        NewPositionDetail.dMarginPerLot=0;
        NewPositionDetail.dPLBurnRate=0;
        NewPositionDetail.dOpenPositionExposure=0;
        NewPositionDetail.dBuyPowerRemain=0;
		NewPositionDetail.dBuyPowerUsed = dInvalidDouble;
		NewPositionDetail.dTotalMarginPaid = dInvalidDouble;
        NewPositionDetail.FTID=0;
        NewPositionDetail.UpdateSeq=0;

        //设置对应的PriceTick
        NewPositionDetail.PriceTick=OppoPositionDetail.PriceTick;
        NewPositionDetail.bToday=OppoPositionDetail.bToday;

        m_PositionDetailData.NewReach_Internal(NewPositionDetail);

        it_DerivedPositionDetail=m_PositionDetailData.m_all.find(DerivedKey);
    }

	if(it_DerivedPositionDetail==m_PositionDetailData.m_all.end()||
        it_DerivedPositionDetail->second==NULL) 
        return false;



    //衍生的(普通)持仓明细
    PlatformStru_PositionDetail& DerivedPositionDetail=*(it_DerivedPositionDetail->second);

    //保存原来的持仓明细
    PlatformStru_PositionDetail OriOppoPositionDetail=OppoPositionDetail;
    PlatformStru_PositionDetail OriDerivedPositionDetail=DerivedPositionDetail;

    //更新持仓明细
    double DeltaValue;   

    //逐日盯市持仓盈亏
    if(OppoPositionDetail.PositionProfitByDate!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.PositionProfitByDate*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.PositionProfitByDate-=DeltaValue;
        DerivedPositionDetail.PositionProfitByDate+=DeltaValue;
    }
    else
        DerivedPositionDetail.PositionProfitByDate=dInvalidDouble;

    //逐笔对冲持仓盈亏
    if(OppoPositionDetail.PositionProfitByTrade!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.PositionProfitByTrade*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.PositionProfitByTrade-=DeltaValue;
        DerivedPositionDetail.PositionProfitByTrade+=DeltaValue;
    }
    else
        DerivedPositionDetail.PositionProfitByTrade=dInvalidDouble;

    //保证金
    if(OppoPositionDetail.Margin!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.Margin*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.Margin-=DeltaValue;
        DerivedPositionDetail.Margin+=DeltaValue;
    }
    else
        DerivedPositionDetail.Margin=dInvalidDouble;

    //交易所保证金
    if(OppoPositionDetail.ExchMargin!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.ExchMargin*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.ExchMargin-=DeltaValue;
        DerivedPositionDetail.ExchMargin+=DeltaValue;
    }

    //持仓量
    OppoPositionDetail.Volume-=ClosedVolume;
    DerivedPositionDetail.Volume+=ClosedVolume;

    //更新序列号
    OppoPositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();
    DerivedPositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();

    //记录更新前后的持仓明细数据
    ModifiedPositionDetails.push_back(OriOppoPositionDetail);
    ModifiedPositionDetails.push_back(OppoPositionDetail);
    ModifiedPositionDetails.push_back(OriDerivedPositionDetail);
    ModifiedPositionDetails.push_back(DerivedPositionDetail);

    //组合持仓明细的数据
    PlatformStru_PositionComb& PositionDetailComb=GetPositionComb3_Internal(OppoPositionDetail.TradeID);
    if(PositionDetailComb.TotalAmt>=ClosedVolume)
    {
        if(PositionDetailComb.Leg1Margin!=dInvalidDouble)
            PositionDetailComb.Leg1Margin -= PositionDetailComb.Leg1Margin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg1Margin = dInvalidDouble;

        if(PositionDetailComb.Leg1ExchMargin!=dInvalidDouble)
            PositionDetailComb.Leg1ExchMargin -= PositionDetailComb.Leg1ExchMargin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg1ExchMargin = dInvalidDouble;

        if(PositionDetailComb.Leg2Margin!=dInvalidDouble)
            PositionDetailComb.Leg2Margin -= PositionDetailComb.Leg2Margin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg2Margin = dInvalidDouble;

        if(PositionDetailComb.Leg2ExchMargin!=dInvalidDouble)
            PositionDetailComb.Leg2ExchMargin -= PositionDetailComb.Leg2ExchMargin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg2ExchMargin = dInvalidDouble;

        PositionDetailComb.TotalAmt-=ClosedVolume;

        PositionDetailComb.UpdateSeq=m_PositionCombData.NewUpdateSeq();
    }


    return true;
}
