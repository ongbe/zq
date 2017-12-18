//维护底层组合持仓信息

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_PositionCombs.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 


CPlatformDataMgr_PositionCombs::CPlatformDataMgr_PositionCombs(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders,
    CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
    CPlatformDataMgr_PositionDetails& refPositionDetails)
: m_RefInstruments(refInstruments),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetailCombs(refPositionDetailCombs),
  m_RefPositionDetails(refPositionDetails),
  m_PositionCombData(baseFTID,baseUpdateSeq)
{
    //查询结果返回时，是否需要创建mapQryRlt. 对于持仓，需要创建，用于合并当日持仓和历史持仓
    m_PositionCombData.SetbNeedKey2QryRlt(true);
}

CPlatformDataMgr_PositionCombs::~CPlatformDataMgr_PositionCombs(void)
{
    LOG_INFO("CPlatformDataMgr_PositionCombs::~CPlatformDataMgr_PositionCombs(析构CPlatformDataMgr_PositionCombs对象)");
}

//查询回报到来
void CPlatformDataMgr_PositionCombs::QryPositionCombReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_PositionCombs::QryPositionCombReach:InstrumentID=%s",PositionInfo.InstrumentID);

    string strInstrumentID(PositionInfo.InstrumentID);
    PlatformStru_InstrumentInfo InstrumentInfo;

    Lock_CPlatformDataMgr;

    //这里只处理组合合约的持仓。如果是单腿合约，只处理bIsLast的情况
    if(ErrID==0)
    {
        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            ErrID=-1;
        else if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination)
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
            bool bMerge=false;
            if(m_PositionCombData.GetpValueInQryRlt_Internal(pLastPosition,positionKey))
            {
                CPlatformDataMgr_Positions::MergePositions_RspQryPosition(*pLastPosition,PositionInfo);

                //这条数据已经合并到原数据记录中了，新数据已经不再有用。将ErrID设置为-1，下面可以统一处理
                ErrID=-1;
            }
        }
    }


    //取出原有记录的持仓成本、开仓成本、持仓均价、开仓均价(这些值是初始化查询后或成交后自己算的)
    if(ErrID==0)
    {
        PlatformStru_Position* pOldPosition;
        if(m_PositionCombData.GetValue_Internal(pOldPosition,PositionKey(PositionInfo)))
        {
            PositionInfo.PositionCost=pOldPosition->PositionCost;
            PositionInfo.OpenCost=pOldPosition->OpenCost;
            PositionInfo.AveragePositionPrice=pOldPosition->AveragePositionPrice;
            PositionInfo.AverageOpenPrice=pOldPosition->AverageOpenPrice;
        }
    }

    //计算昨仓剩余仓位
    if(ErrID==0)
    {
        PositionInfo.YdPositionRemain=PositionInfo.Position-PositionInfo.TodayPosition;
    }

    m_PositionCombData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);

  //  if(bIsLast)
  //  {
  //      set<string> setInstruments;
  //      m_PositionCombData.GetInstrumentSetOfQryRlt(setInstruments);

  //      for(set<string>::iterator it_Instruments=setInstruments.begin();it_Instruments!=setInstruments.end();it_Instruments++)
		//{
  //          map<string,set<long>>::iterator it_Index2=m_PositionCombData.m_allIndex2.find(*it_Instruments);
  //          if(it_Index2!=m_PositionCombData.m_allIndex2.end()&&!it_Index2->second.empty())
  //          {
  //              for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
  //              {
  //                  PlatformStru_Position* pPosition;
  //                  if(m_PositionCombData.GetValue2_Internal(pPosition,*itFTID))
  //                  {
  //                      //计算昨仓剩余仓位
  //                      pPosition->YdPositionRemain=pPosition->Position-pPosition->TodayPosition;
  //                      pPosition->UpdateSeq=m_PositionCombData.NewUpdateSeq(pPosition->FTID);
  //                  }
  //              }
		//	}
		//}
  //  }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_PositionCombs::QryPositionCombReach-end");
}


//从成交记录更新组合持仓。
//此函数从组合持仓明细中重新创建并计算组合持仓。依赖于组合持仓明细，必须在组合持仓明细更新完后再更新组合持仓
void CPlatformDataMgr_PositionCombs::Update_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
                                                     const map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs)
{
	LOG_INFO("CPlatformDataMgr_PositionCombs::Update_RtnTrade(成交回报更新组合持仓):InstrumentID=%s",TradeInfo.InstrumentID);

    CeasymutexGuard guard(m_mutex);

    //对应的组合持仓key
    set<PositionKey> positionkeys;
    PlatformStru_PositionDetailComb detailcomb;
    for(map<PositionDetailCombKey,PlatformStru_PositionDetailComb>::const_iterator it=ModifiedPositionDetailCombs.begin();it!=ModifiedPositionDetailCombs.end();it++)
    {
        if(!m_RefPositionDetailCombs.GetPositionDetailComb(it->first,detailcomb)) continue;
        string strCombInstrumentID(detailcomb.CombInstrumentID);
        if(strCombInstrumentID.empty()) continue;
        if(detailcomb.LegID!=0&&detailcomb.LegID!=1) continue;

        char PosiDirection;
        if(detailcomb.LegID==0) PosiDirection=detailcomb.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short;
        else PosiDirection=detailcomb.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long;
        PositionKey positionkey(detailcomb.Account,strCombInstrumentID.c_str(),PosiDirection,detailcomb.HedgeFlag);

        if(positionkeys.find(positionkey)==positionkeys.end())
            positionkeys.insert(positionkey);
    }

    for(set<PositionKey>::const_iterator it=positionkeys.begin();it!=positionkeys.end();it++)
    {
        //获取组合合约信息
        PlatformStru_InstrumentInfo CombInstrumentInfo;
        if(!m_RefInstruments.GetInstrumentInfo(string(it->InstrumentID),CombInstrumentInfo))
            continue;

        //检查有没有原有持仓，如果是开仓成交，可能需要创建新的组合持仓
        PlatformStru_Position* pPositionComb;
        if(!m_PositionCombData.GetValue_Internal(pPositionComb,*it))
        {
            PlatformStru_Position NewPositionData;

            strncpy(NewPositionData.Account,it->Account,sizeof(NewPositionData.Account)-1);
            strncpy(NewPositionData.ExchangeID,TradeInfo.ExchangeID,sizeof(NewPositionData.ExchangeID)-1);
            strncpy(NewPositionData.InstrumentID,it->InstrumentID,sizeof(NewPositionData.InstrumentID)-1);
            strncpy(NewPositionData.BrokerID,TradeInfo.BrokerID,sizeof(NewPositionData.BrokerID)-1);
            strncpy(NewPositionData.InvestorID,TradeInfo.InvestorID,sizeof(NewPositionData.InvestorID)-1);
            NewPositionData.PosiDirection=it->PosiDirection;
            NewPositionData.HedgeFlag=it->HedgeFlag;
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

            NewPositionData.CurPrice=util::GetDoubleInvalidValue();

            //设置对应的PriceTick
            NewPositionData.PriceTick=CombInstrumentInfo.PriceTick;
            strncpy(NewPositionData.Currency,CombInstrumentInfo.Currency,sizeof(NewPositionData.Currency)-1);

            m_PositionCombData.NewReach_Internal(NewPositionData);

            if(!m_PositionCombData.GetValue_Internal(pPositionComb,*it))
                continue;
        }



        pPositionComb->Position=0;
        pPositionComb->PositionCost=0;
        pPositionComb->OpenCost=0;
        pPositionComb->AveragePositionPrice=0;
        pPositionComb->AverageOpenPrice=0;
        pPositionComb->UseMargin=0;
        pPositionComb->ExchangeMargin=0;
        pPositionComb->OpenVolume=util::GetIntInvalidValue();
        pPositionComb->OpenAmount=util::GetDoubleInvalidValue();
        pPositionComb->TodayPosition=util::GetIntInvalidValue();
        pPositionComb->YdPosition=util::GetIntInvalidValue();
        pPositionComb->CloseAmount=util::GetDoubleInvalidValue();
        pPositionComb->CloseProfit=util::GetDoubleInvalidValue();
        pPositionComb->CloseProfitByDate=util::GetDoubleInvalidValue();
        pPositionComb->CloseProfitByTrade=util::GetDoubleInvalidValue();

        pPositionComb->PositionProfit=util::GetDoubleInvalidValue();
        pPositionComb->PositionProfitByTrade=util::GetDoubleInvalidValue();
        pPositionComb->TotalPositionProfitByDate=util::GetDoubleInvalidValue();
        pPositionComb->Commission=util::GetDoubleInvalidValue();
        

        UpdateCostByPositionDetialComb_Internal(*it,*pPositionComb,true);
    }


    LOG_INFO("CPlatformDataMgr_PositionCombs::Update_RtnTrade-end");
    return;
}

//根据组合持仓明细和持仓明细，更新组合持仓的持仓成本、开仓成本、持仓均价和开仓均价。在初始化查询完毕后调用
void CPlatformDataMgr_PositionCombs::UpdateCostByPositionDetialComb_AfterInitQry(void)
{
    CeasymutexGuard guard(m_mutex);
    for(map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionCombData.m_all.begin();it!=m_PositionCombData.m_all.end();it++)
    {
        UpdateCostByPositionDetialComb_Internal(it->first,*(it->second),false);
    }
}

//根据组合持仓明细和持仓明细，更新组合持仓的持仓数、持仓成本、开仓成本、持仓均价、开仓均价和保证金。
void CPlatformDataMgr_PositionCombs::UpdateCostByPositionDetialComb_Internal(const PositionKey& combkey,PlatformStru_Position& comb,bool bUpdateMargin_Position)
{
    CeasymutexGuard guard(m_mutex);

    CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-开始");

    comb.PositionCost=0;
    comb.OpenCost=0;
    comb.AveragePositionPrice=util::GetIntInvalidValue();
    comb.AverageOpenPrice=util::GetIntInvalidValue();
    if(bUpdateMargin_Position)
    {
        comb.Position=0;
        comb.UseMargin=0;
        comb.ExchangeMargin=0;
    }

    vector<PositionDetailCombKey> detailcombkeys;
    m_RefPositionDetailCombs.GetPositionDetailCombKeysByPositionCombKey(combkey,detailcombkeys);
    int VolumeMultiple=util::GetIntInvalidValue();

    CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-1");

    for(int i=0;i<(int)detailcombkeys.size();i++)
    {
        //取对应的组合持仓明细
        PlatformStru_PositionDetailComb detailcomb;
        if(!m_RefPositionDetailCombs.GetPositionDetailComb(detailcombkeys[i],detailcomb))
        {
            CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-取对应的组合持仓明细失败");
            break;
        }

        //取对应的持仓明细
        PositionDetailKey detailkey(detailcombkeys[i].Account,detailcombkeys[i].InstrumentID,detailcombkeys[i].OpenDate,detailcombkeys[i].TradeID,detailcombkeys[i].Direction,THOST_FTDC_TRDT_CombinationDerived,detailcombkeys[i].HedgeFlag);
        PlatformStru_PositionDetail detail;
        if(!m_RefPositionDetails.GetPositionDetail(detailkey,detail))
        {
            CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-取对应的持仓明细失败");
            break;
        }

        bool bToday=detailcomb.bToday;

        //获取单腿合约信息
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(!m_RefInstruments.GetInstrumentInfo(string(detailcombkeys[i].InstrumentID),InstrumentInfo))
        {
            CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-获取单腿合约信息失败");
            break;
        }

        if(detailcomb.LegID!=0&&detailcomb.LegID!=1)
        {
            CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-LegID不等于0和1");
            break;
        }

        if(util::isInvalidValue(VolumeMultiple))
        {
            VolumeMultiple=InstrumentInfo.VolumeMultiple;
            CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-获取VolumeMultiple=%d",VolumeMultiple);
        }

        //更新组合持仓的持仓成本
        comb.PositionCost += (detailcomb.LegID==0?1:-1)*detailcomb.TotalAmt*InstrumentInfo.VolumeMultiple*(bToday?detail.OpenPrice:detail.LastSettlementPrice);

        //更新组合持仓的开仓成本
        comb.OpenCost += (detailcomb.LegID==0?1:-1)*detailcomb.TotalAmt*InstrumentInfo.VolumeMultiple*detail.OpenPrice;
        
        //更新持仓记录的持仓量和占用保证金
        if(bUpdateMargin_Position)
        {
            if(detailcomb.LegID==0)
                comb.Position+=detailcomb.TotalAmt;

            if(!util::isInvalidValue(comb.UseMargin))
            {
                if(!util::isInvalidValue(detailcomb.Margin))
                    comb.UseMargin+=detailcomb.Margin;
                else comb.UseMargin=util::GetDoubleInvalidValue();
            }
            if(!util::isInvalidValue(comb.ExchangeMargin))
            {
                if(!util::isInvalidValue(detailcomb.ExchMargin))
                    comb.ExchangeMargin+=detailcomb.ExchMargin;
                else comb.ExchangeMargin=util::GetDoubleInvalidValue();
            }
        }
    }
    CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-VolumeMultiple=%d",VolumeMultiple);
    if(!util::isInvalidValue(VolumeMultiple))
    {
        comb.AveragePositionPrice=comb.Position>0&&VolumeMultiple>0?comb.PositionCost/comb.Position/VolumeMultiple:0;
        comb.AverageOpenPrice=comb.Position>0&&VolumeMultiple>0?comb.OpenCost/comb.Position/VolumeMultiple:0;
        CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-AveragePositionPrice=%g AverageOpenPrice=%g",comb.AveragePositionPrice,comb.AverageOpenPrice);
    }
    
    CFileOpr::getObj().writelocallog("计算组合持仓成本.log","UpdateCostByPositionDetialComb_Internal-结束");
    comb.UpdateSeq=m_PositionCombData.NewUpdateSeq(comb.FTID);
}

//更新组合持仓记录的保证金、保证金率，查询保证金率响应时调用
bool CPlatformDataMgr_PositionCombs::UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate) 
{
    bool brlt=false;
    string strInstrument(MarginRate.InstrumentID);

    Lock_CPlatformDataMgr;

    map<PositionKey,double> PositionMargin;
    map<PositionKey,double> PositionExchangeMargin;
    map<PositionKey,double>::const_iterator it;
    PlatformStru_Position* pPosition;

    m_RefPositionDetailCombs.GetPositionMarginByLegInstrument(strInstrument,PositionMargin,PositionExchangeMargin);
    for(it=PositionMargin.begin();it!=PositionMargin.end();it++)
    {
        if(m_PositionCombData.GetValue_Internal(pPosition,it->first)&&
            !util::equals(pPosition->UseMargin,it->second)) 
        {
            pPosition->UseMargin=it->second;
            if(!brlt) brlt=true;
        }
    }
    for(it=PositionExchangeMargin.begin();it!=PositionExchangeMargin.end();it++)
    {
        if(m_PositionCombData.GetValue_Internal(pPosition,it->first)&&
            !util::equals(pPosition->ExchangeMargin,it->second)) 
        {
            pPosition->ExchangeMargin=it->second;
            if(!brlt) brlt=true;
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}


//清空查询组合持仓的map
void CPlatformDataMgr_PositionCombs::ClearMapQryRltPositionCombs(string& strQryInstrument)
{
    Lock_CPlatformDataMgr;
	m_PositionCombData.WillQry_Internal(strQryInstrument);
    Unlock_CPlatformDataMgr;
}
//获取组合持仓的集合
int CPlatformDataMgr_PositionCombs::GetPositionCombs(vector<PlatformStru_Position>& outData)
{
	outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL && it->second->Position > 0 )
		{
            outData.push_back(*(it->second));
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}
//获取指定合约的组合持仓集合
int CPlatformDataMgr_PositionCombs::GetPositionCombs2(const string& strInstrument,vector<PlatformStru_Position>& outData)
{
	outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL && it->second->Position > 0 && 
            strncmp(strInstrument.c_str(),it->second->InstrumentID,sizeof(it->second->InstrumentID))==0)
		{
            outData.push_back(*(it->second));
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}

//获取查询的合约ID
void CPlatformDataMgr_PositionCombs::GetQryPositionCombInstrument(string& outQryInstrument)
{
    Lock_CPlatformDataMgr;
    m_PositionCombData.GetQryInstrument(outQryInstrument);
    Unlock_CPlatformDataMgr;
}

//获取组合持仓的FTID集合
int CPlatformDataMgr_PositionCombs::GetPositionCombFTIDList(vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	set<long>::iterator it_FTID = m_PositionCombData.m_allFTID.begin();
	for ( ; it_FTID != m_PositionCombData.m_allFTID.end(); ++it_FTID )
	{
		vec.push_back(*it_FTID);
	}
	Unlock_CPlatformDataMgr;

	return vec.size();
}

//获取指定FTID的组合持仓明细
int CPlatformDataMgr_PositionCombs::GetPositionCombInfo(long lFTID, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionCombData.m_all2.find(lFTID);
	if ( it_FTID != m_PositionCombData.m_all2.end())
	{
		outData = *(it_FTID->second);
	}
	else
	{
		nRet = -1;
	}
	Unlock_CPlatformDataMgr;

	return nRet;
}

//取全部组合持仓的占用保证金
void CPlatformDataMgr_PositionCombs::GetAllMargin(double& TotalMargin)
{
	TotalMargin=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	Lock_CPlatformDataMgr;
	iter = m_PositionCombData.m_all.begin();
	for(;iter != m_PositionCombData.m_all.end(); iter++)
	{
        if(TotalMargin!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->UseMargin!=InvalideValue)
                TotalMargin+=iter->second->UseMargin;
            else TotalMargin=InvalideValue;
        }
	}
	Unlock_CPlatformDataMgr;
}

void CPlatformDataMgr_PositionCombs::clear(void)
{
	Lock_CPlatformDataMgr;
    m_PositionCombData.Clear();
	Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_PositionCombs::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_PositionCombData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}

