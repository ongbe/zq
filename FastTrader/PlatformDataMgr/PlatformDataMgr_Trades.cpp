//维护底层成交记录信息

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
#include <set>
using std::string;
using std::map;
using std::set;

#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"


#ifdef WIN32
#pragma unmanaged
#endif 


#ifndef __CUSTOM_FILE__
#define __CUSTOM_FILE__    (string(__FILE__).substr(string(__FILE__).rfind('\\')+1).c_str())
#endif

#define LOG_INFO2(fmt, ...) \
    {\
        char logbuf[512]={0},logbuf2[512]={0};\
        CFileOpr::getObj().writelocallog(__CUSTOM_FILE__, "[%d]:"fmt, __LINE__, __VA_ARGS__);\
	}


//构造：
CPlatformDataMgr_Trades::CPlatformDataMgr_Trades(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Rates& refRates)
: m_RefInstruments(refInstruments),
  m_RefRates(refRates),
  m_TradeData(baseFTID,baseUpdateSeq,true)
{
}


CPlatformDataMgr_Trades::~CPlatformDataMgr_Trades(void)
{
    LOG_INFO("CPlatformDataMgr_Trades::~CPlatformDataMgr_Trades(析构CPlatformDataMgr_Trades对象)");
}



//查询的成交回报到来
void CPlatformDataMgr_Trades::QryTradeReach( PlatformStru_TradeInfo& TradeInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_Trades::QryTradeReach");

    string strInstrumentID;
    TradeKey key;
    string strTradeID;
    string strOrderSysID;
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(ErrID==0)
    {
        strInstrumentID=string(TradeInfo.InstrumentID);
        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) ErrID=-1;
        else 
        {
            TradeInfo.PriceTick=InstrumentInfo.PriceTick;
            key=TradeKey(TradeInfo);
            strTradeID=string(TradeInfo.TradeID);
            strOrderSysID=string(TradeInfo.OrderSysID);
        }
    }

    Lock_CPlatformDataMgr;

    if(ErrID==0)
    {
        //从原有的成交回报数据中取出平今量、平仓盈亏、手续费，这几个值是根据持仓明细计算的，认为不会变化。
        PlatformStru_TradeInfo* pOldTrade;
        if(m_TradeData.GetValue_Internal(pOldTrade,key))
        {
            TradeInfo.CloseTodayVolume=pOldTrade->CloseTodayVolume;
            TradeInfo.CloseProfitByDate=pOldTrade->CloseProfitByDate;
            TradeInfo.CloseProfitByTrade=pOldTrade->CloseProfitByTrade;
            TradeInfo.TradeCommission=pOldTrade->TradeCommission;
        }
    }

    //将成交数据送到数据集中
    m_TradeData.QryRltReach_Internal(TradeInfo,ErrID,bIsLast);

    if(ErrID==0)
    {
        //维护OrderSysID 到 TradeKey 的映射关系。主要用于日内组合成交的配对查询
        if(!CTools_Ansi::IsKeyValueInMultimap<string, TradeKey>(m_mapOrderSysID2TradeKey,strOrderSysID,key))
            m_mapOrderSysID2TradeKey.insert(make_pair(strOrderSysID,key));

        //维护TradeID 到 TradeKey 的映射关系。主要用于日内组合成交的配对查询
        if(!CTools_Ansi::IsKeyValueInMultimap<string, TradeKey>(m_mapTradeID2TradeKey,strTradeID,key))
            m_mapTradeID2TradeKey.insert(make_pair(strTradeID,key));
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Trades::QryTradeReach-end");
}

//新的成交回报到来
void CPlatformDataMgr_Trades::NewTradeReady(PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    LOG_INFO("CPlatformDataMgr_Trades::NewTradeReady InstrumentID=%s TradeID=%s",TradeInfo.InstrumentID,TradeInfo.TradeID);

    string strInstrumentID(TradeInfo.InstrumentID);

    //设置对应的PriceTick
    TradeInfo.PriceTick=InstrumentInfo.PriceTick;

    Lock_CPlatformDataMgr;

    //将成交数据送到数据集中
    m_TradeData.NewReach_Internal(TradeInfo);

    TradeKey key(TradeInfo);

    //维护OrderSysID 到 TradeKey 的映射关系。主要用于日内组合成交的配对查询
    if(!CTools_Ansi::IsKeyValueInMultimap<string, TradeKey>(m_mapOrderSysID2TradeKey,string(TradeInfo.OrderSysID),key))
        m_mapOrderSysID2TradeKey.insert(make_pair(string(TradeInfo.OrderSysID),key));

    //维护TradeID 到 TradeKey 的映射关系。主要用于日内组合成交的配对查询
    if(!CTools_Ansi::IsKeyValueInMultimap<string, TradeKey>(m_mapTradeID2TradeKey,string(TradeInfo.TradeID),key))
        m_mapTradeID2TradeKey.insert(make_pair(string(TradeInfo.TradeID),key));

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Trades::NewTradeReady-end TradeID=%s FTID=%u",TradeInfo.TradeID,TradeInfo.FTID);

}

//更新成交记录的平今量和平仓盈亏，重新计算手续费。
void CPlatformDataMgr_Trades::UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(long TradeFTID,
                                                                                        const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                                        int CloseTodayVolume,
                                                                                        double CloseProfitByDate,
                                                                                        double CloseProfitByTrade,
																						double& outCommission)
{
    Lock_CPlatformDataMgr;

    PlatformStru_TradeInfo* pTrade=NULL;
    if(m_TradeData.GetValue2_Internal(pTrade,TradeFTID))
    {
        pTrade->CloseTodayVolume=CloseTodayVolume;
        pTrade->CloseProfitByDate=CloseProfitByDate;
        pTrade->CloseProfitByTrade=CloseProfitByTrade;

        PlatformStru_InstrumentCommissionRate CommissionRate;
        if(m_RefRates.GetCommissionRate(string(pTrade->InstrumentID),CommissionRate)==0)
		{
            CalcuTradeCommission_Internal(*pTrade,InstrumentInfo,CommissionRate);
			outCommission=pTrade->TradeCommission;
		}
        
        pTrade->UpdateSeq=m_TradeData.NewUpdateSeq(pTrade->FTID);
    }

    Unlock_CPlatformDataMgr;
}


//获取手续费率后，更新所有成交中的手续费，返回值表示相关手续费是否有改变，以此决定是否需要更新资金面板。
bool CPlatformDataMgr_Trades::UpdateTradeCommission_RspQryCommissionRate(const string& strInstrumentID,
                                                                         const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                         const PlatformStru_InstrumentCommissionRate& CommissionRate)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_TradeData.m_allIndex2.find(strInstrumentID);
    PlatformStru_TradeInfo *pTrade;
    if(it_Index2!=m_TradeData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        //注意，对于大商郑商，由于平今优惠按每个合约剩余的平今优惠配额手数来判断，得到手续费率时需要全部重新计算
        string strExchangeID(InstrumentInfo.ExchangeID);
        string strInstrumnetID(InstrumentInfo.InstrumentID);
        if(strExchangeID==string("DCE")||strExchangeID==string("CZCE"))
        {
            if(m_TradeData.m_all.size()>0)
            {
                char logbuf[2048];
                char logbuf2[256];
                memset(logbuf,0,sizeof(logbuf));
                _snprintf(logbuf,sizeof(logbuf)-1,"收到%s手续费率, 修改平今优惠配额手数\n",strInstrumentID.c_str());

                PositionKey positionkey;
                TradeKey firstTradeKey=m_TradeData.m_all.begin()->first;

                positionkey.set(firstTradeKey.Account,firstTradeKey.InstrumentID,THOST_FTDC_PD_Long,THOST_FTDC_HF_Speculation);
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[positionkey]=0;
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",positionkey.tostring(logbuf2,sizeof(logbuf2)-1),0);

                positionkey.set(firstTradeKey.Account,firstTradeKey.InstrumentID,THOST_FTDC_PD_Long,THOST_FTDC_HF_Arbitrage);
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[positionkey]=0;
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",positionkey.tostring(logbuf2,sizeof(logbuf2)-1),0);

                positionkey.set(firstTradeKey.Account,firstTradeKey.InstrumentID,THOST_FTDC_PD_Long,THOST_FTDC_HF_Hedge);
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[positionkey]=0;
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",positionkey.tostring(logbuf2,sizeof(logbuf2)-1),0);

                positionkey.set(firstTradeKey.Account,firstTradeKey.InstrumentID,THOST_FTDC_PD_Short,THOST_FTDC_HF_Speculation);
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[positionkey]=0;
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",positionkey.tostring(logbuf2,sizeof(logbuf2)-1),0);

                positionkey.set(firstTradeKey.Account,firstTradeKey.InstrumentID,THOST_FTDC_PD_Short,THOST_FTDC_HF_Arbitrage);
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[positionkey]=0;
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",positionkey.tostring(logbuf2,sizeof(logbuf2)-1),0);

                positionkey.set(firstTradeKey.Account,firstTradeKey.InstrumentID,THOST_FTDC_PD_Short,THOST_FTDC_HF_Hedge);
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[positionkey]=0;
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",positionkey.tostring(logbuf2,sizeof(logbuf2)-1),0);

                LOG_INFO2("维护平今优惠配额手数:\n%s",logbuf);
                //CFileOpr::getObj().writelocallog("维护平今优惠配额手数.log","%s",logbuf);
            }
        }

        brlt=true;
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            if(!m_TradeData.GetValue2_Internal(pTrade,*itFTID)) continue;
            if(strExchangeID==string("DCE")||strExchangeID==string("CZCE"))
                pTrade->TradeCommission=util::GetDoubleInvalidValue();
            if(util::isInvalidValue(pTrade->TradeCommission))
            {
                CalcuTradeCommission_Internal(*pTrade,InstrumentInfo,CommissionRate);
                pTrade->UpdateSeq=m_TradeData.NewUpdateSeq(pTrade->FTID);
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//获取全部成交单，返回数据项的数量
int CPlatformDataMgr_Trades::GetAllTrades(vector<PlatformStru_TradeInfo>& outData)
{
    int irlt;
    Lock_CPlatformDataMgr;

    irlt=m_TradeData.GetAll2_Internal(outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

///获取指定成交信息
bool CPlatformDataMgr_Trades::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    bool brlt=false;
    Lock_CPlatformDataMgr;

    PlatformStru_TradeInfo *pTrade;
    if(m_TradeData.GetValue_Internal(pTrade,tradekey))
    {
        outData=*pTrade;
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}
bool CPlatformDataMgr_Trades::GetTradeInfo(long FTID, PlatformStru_TradeInfo& outData)
{
    bool brlt=false;
    Lock_CPlatformDataMgr;

    brlt=m_TradeData.GetValue3_Internal(outData,FTID);

    Unlock_CPlatformDataMgr;

    return brlt;
}
//获取全部成交的FTID，按合约ID分类。同一个合约的FTID是排序的
void CPlatformDataMgr_Trades::GetFTIDsByInstrument(map<string,vector<long>>& outFTIDs)
{
    m_TradeData.GetFTIDsByInstrument(outFTIDs);
}


 ///获取全部成交，返回数据项的数量
int CPlatformDataMgr_Trades::GetTradesOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfo>& outData)
{
    int irlt;
    Lock_CPlatformDataMgr;

    irlt=m_TradeData.GetAllAboutInstrument2_Internal(strInstrument,outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

 ///获取全部成交，返回数据项的数量
int CPlatformDataMgr_Trades::GetTradeBriefsOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfoBrief>& outData)
{
    int irlt;
    Lock_CPlatformDataMgr;

    vector<PlatformStru_TradeInfo> oriData;
    if(strInstrument.empty()) irlt=m_TradeData.GetAll2_Internal(oriData);
    else irlt=m_TradeData.GetAllAboutInstrument2_Internal(strInstrument,oriData);
    outData.clear();
    for(int i=0;i<(int)oriData.size();i++)
        outData.push_back(PlatformStru_TradeInfoBrief(oriData[i]));

    Unlock_CPlatformDataMgr;

    return irlt;
}


//获取交易手续费之和
double CPlatformDataMgr_Trades::GetTotalCommission(void)
{
    map<TradeKey,PlatformStru_TradeInfo*>::iterator iter;
    double drlt=0.0;

    Lock_CPlatformDataMgr;

    for(iter = m_TradeData.m_all.begin();iter != m_TradeData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&!util::isInvalidValue(iter->second->TradeCommission))
        {
            drlt+=iter->second->TradeCommission;
        }
        else
        {
            drlt=util::GetDoubleInvalidValue();
            break;
        }
    }
    Unlock_CPlatformDataMgr;

    return drlt;
}

//获取持仓交易手续费
double CPlatformDataMgr_Trades::GetCommission(const string& strInstrument,const string& strAccount,char HedgeFlag,char Direction)
{
    double Commission=0.0;
    Lock_CPlatformDataMgr;

    map<long,PlatformStru_TradeInfo*>::iterator it_TradeInfo2;

    map<string,set<long>>::iterator it_Index2=m_TradeData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_TradeData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_TradeInfo2=m_TradeData.m_all2.find(*itFTID);
            if(it_TradeInfo2==m_TradeData.m_all2.end()) continue;
            if(it_TradeInfo2->second==NULL) continue;
            if(string(it_TradeInfo2->second->Account)!=strAccount) continue;
            if(it_TradeInfo2->second->HedgeFlag!=HedgeFlag) continue;
            if(it_TradeInfo2->second->Direction!=Direction) continue;

            if(it_TradeInfo2->second->TradeCommission!=util::GetDoubleInvalidValue())
            {
                Commission+=it_TradeInfo2->second->TradeCommission;
            }
            else 
            {
                Commission=util::GetDoubleInvalidValue();
                break;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return Commission;
}


//准备查询成交前清空要查询的合约名
void CPlatformDataMgr_Trades::ClearMapQryRltTrades()
{
    Lock_CPlatformDataMgr;

    m_TradeData.WillQry_Internal(string(""));

    Unlock_CPlatformDataMgr;
}

//获取指定TradeID的成交记录Key。
void CPlatformDataMgr_Trades::GetTradeKeysOfTradeID(const string& strTradeID,vector<TradeKey>& outData)
{
    Lock_CPlatformDataMgr;
    CTools_Ansi::GetValuesInMultimap<string, TradeKey>(m_mapTradeID2TradeKey,strTradeID,outData);
    Unlock_CPlatformDataMgr;
}
//组合成交时，获取OrderSysID一致的成交明细集合。Volume>0
//今仓：1. tradekey找OrderSysID；2. OrderSysID找所有TradeID
void CPlatformDataMgr_Trades::GetTradeKeysByTradeComb_OrderSysID(const TradeKey& tradekey,set<TradeKey>& outData)
{
    CeasymutexGuard guard(m_mutex);

    outData.clear();

    PlatformStru_TradeInfo* pTradeInfo;
    if(!m_TradeData.GetValue_Internal(pTradeInfo,tradekey)) return;

    multimap<string, TradeKey>::iterator it_upper,it_lower;
    string strAccount(tradekey.Account);
    string strOrderSysID(pTradeInfo->OrderSysID);
    it_lower=m_mapOrderSysID2TradeKey.lower_bound(strOrderSysID);
    it_upper=m_mapOrderSysID2TradeKey.upper_bound(strOrderSysID);
    for(;it_lower!=it_upper;it_lower++)
    {
        if(string(it_lower->second.Account)==strAccount&&
            it_lower->second.HedgeFlag==tradekey.HedgeFlag&&
            m_TradeData.GetValue_Internal(pTradeInfo,it_lower->second)&&
            pTradeInfo->Volume>0)
        {
            outData.insert(it_lower->second);
        }
    }
}

//组合成交时，获取OrderSysID一致的成交明细集合。Volume>0
//今仓：1. tradekey找OrderSysID；2. OrderSysID找所有TradeID
void CPlatformDataMgr_Trades::GetTradeKeysByTradeComb_OrderSysID2(const TradeKey& tradekey,map<string,TradeKey>& outData)
{
    CeasymutexGuard guard(m_mutex);

    outData.clear();

    PlatformStru_TradeInfo* pTradeInfo;
    if(!m_TradeData.GetValue_Internal(pTradeInfo,tradekey)) return;

    multimap<string, TradeKey>::iterator it_upper,it_lower;
    string strAccount(tradekey.Account);
    string strOrderSysID(pTradeInfo->OrderSysID);
    it_lower=m_mapOrderSysID2TradeKey.lower_bound(strOrderSysID);
    it_upper=m_mapOrderSysID2TradeKey.upper_bound(strOrderSysID);
    for(;it_lower!=it_upper;it_lower++)
    {
        if(string(it_lower->second.Account)==strAccount&&
            it_lower->second.HedgeFlag==tradekey.HedgeFlag&&
            m_TradeData.GetValue_Internal(pTradeInfo,it_lower->second)&&
            pTradeInfo->Volume>0)
        {
            outData[string(it_lower->second.TradeID)]=it_lower->second;
        }
    }
}

//获取指定成交key对应的Volume
bool CPlatformDataMgr_Trades::GetTradeVolume(const TradeKey& tradekey,int& Volume)
{
    CeasymutexGuard guard(m_mutex);
    PlatformStru_TradeInfo* pTradeInfo;
    if(m_TradeData.GetValue_Internal(pTradeInfo,tradekey))
    {
        Volume=pTradeInfo->Volume;
        return true;
    }
    return false;
}


//根据成交记录，获取成交统计key的集合
void  CPlatformDataMgr_Trades::GetTradeTotalKeysByTrades(set<TradeTotalKey>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;
    for(map<TradeKey,PlatformStru_TradeInfo*>::iterator it_all=m_TradeData.m_all.begin();it_all!=m_TradeData.m_all.end();it_all++)
    {
        if(it_all->second==NULL) continue;
		TradeTotalKey tmpKey(*(it_all->second));
        if(outData.find(tmpKey)==outData.end())
            outData.insert(tmpKey);
    }
    Unlock_CPlatformDataMgr;
}

//根据成交记录，获取指定key的成交统计数据
bool CPlatformDataMgr_Trades::GetTradeTotalValue(const TradeTotalKey& TotalKey,PlatformStru_TradeTotalInfo& TradeTotalInfo)
{
    Lock_CPlatformDataMgr;

    bool brlt=false;
    map<long,PlatformStru_TradeInfo*>::iterator it2;

    TradeTotalInfo.Clear();
    map<string,set<long>>::iterator it_Index2=m_TradeData.m_allIndex2.find(TotalKey.InstrumentID);
    if(it_Index2!=m_TradeData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_TradeInfo*>::iterator it2=m_TradeData.m_all2.find(*itFTID);

            if(it2==m_TradeData.m_all2.end()) continue;
            if(it2->second==NULL) continue;
		    if(string(it2->second->Account)!=TotalKey.Account) continue;
            if(it2->second->Direction!=TotalKey.Direction) continue;
            if(it2->second->OffsetFlag!=TotalKey.OffsetFlag) continue;

            TradeTotalInfo.price+=it2->second->Price*it2->second->Volume;
            TradeTotalInfo.volume+=it2->second->Volume;
            if(!util::isInvalidValue(TradeTotalInfo.TradeCommission))
            {
			    if(util::isInvalidValue(it2->second->TradeCommission))
				    TradeTotalInfo.TradeCommission=util::GetDoubleInvalidValue();
			    else
                    TradeTotalInfo.TradeCommission+=it2->second->TradeCommission;
            }

            if(!brlt)
            {
                strncpy(TradeTotalInfo.Account,TotalKey.Account,sizeof(TradeTotalInfo.Account)-1);
                strncpy(TradeTotalInfo.ExchangeID,it2->second->ExchangeID,sizeof(TradeTotalInfo.ExchangeID)-1);
			    strncpy(TradeTotalInfo.InstrumentID,TotalKey.InstrumentID,sizeof(TradeTotalInfo.InstrumentID)-1);
			    TradeTotalInfo.Direction=TotalKey.Direction;
			    TradeTotalInfo.OffsetFlag=TotalKey.OffsetFlag;
                brlt=true;
            }
        }
    }

    if(brlt)
    {
        if(TradeTotalInfo.volume==0) TradeTotalInfo.price=0;
        else TradeTotalInfo.price/=TradeTotalInfo.volume;
    }

    Unlock_CPlatformDataMgr;
    return brlt;
}

void  CPlatformDataMgr_Trades::clear(void)
{
    Lock_CPlatformDataMgr;
    m_TradeData.Clear();
    m_mapOrderSysID2TradeKey.clear();
    m_mapTradeID2TradeKey.clear();
    Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_Trades::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt = m_TradeData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}


//计算成交手续费
void CPlatformDataMgr_Trades::CalcuTradeCommission_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                            const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                            const PlatformStru_InstrumentCommissionRate& CommissionRate)
{
    //成交手续费算过就不用再算了
    if(!util::isInvalidValue(TradeInfo.TradeCommission))
        return;

    string strExchangeID(TradeInfo.ExchangeID);
    string strInstrument(TradeInfo.InstrumentID);

    if(TradeInfo.OffsetFlag == THOST_FTDC_OF_Open&&
        !util::isInvalidValue(CommissionRate.OpenRatioByMoney)&&!util::isInvalidValue(CommissionRate.OpenRatioByVolume))
    {
        TradeInfo.TradeCommission=  TradeInfo.Price*TradeInfo.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.OpenRatioByMoney+
                                    TradeInfo.Volume*CommissionRate.OpenRatioByVolume;

        //维护郑商大商每个合约剩余的平今优惠配额手数
        if(strExchangeID==string("DCE")||strExchangeID==string("CZCE"))
        {
            PositionKey pkey(TradeInfo);
            if(m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE.find(pkey)==m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE.end())
                m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[pkey]=TradeInfo.Volume;
            else m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[pkey]+=TradeInfo.Volume;

            char logbuf[2048];
            char logbuf2[256];
            memset(logbuf,0,sizeof(logbuf));
            _snprintf(logbuf,sizeof(logbuf)-1,"根据成交(%s)计算手续费, 修改平今优惠配额手数\n",TradeKey(TradeInfo).tostring(logbuf2,sizeof(logbuf2)-1));
            _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",pkey.tostring(logbuf2,sizeof(logbuf2)-1),m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[pkey]);
            LOG_INFO2("维护平今优惠配额手数:\n%s",logbuf);
            //CFileOpr::getObj().writelocallog("维护平今优惠配额手数.log","%s",logbuf);
        }

    }
    else if(TradeInfo.OffsetFlag == THOST_FTDC_OF_CloseToday&&
        !util::isInvalidValue(CommissionRate.CloseTodayRatioByMoney)&&!util::isInvalidValue(CommissionRate.CloseTodayRatioByVolume))
    {
        TradeInfo.TradeCommission=  TradeInfo.Price*TradeInfo.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.CloseTodayRatioByMoney+
                                    TradeInfo.Volume*CommissionRate.CloseTodayRatioByVolume;

        //维护郑商大商每个合约剩余的平今优惠配额手数
        if(strExchangeID==string("DCE")||strExchangeID==string("CZCE"))
        {
            PositionKey pkey(TradeInfo);
            map<PositionKey,int>::iterator it=m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE.find(pkey);
            if(it!=m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE.end())
                it->second-=TradeInfo.Volume;

            char logbuf[2048];
            char logbuf2[256];
            memset(logbuf,0,sizeof(logbuf));
            _snprintf(logbuf,sizeof(logbuf)-1,"根据成交(%s)计算手续费, 修改平今优惠配额手数\n",TradeKey(TradeInfo).tostring(logbuf2,sizeof(logbuf2)-1));
            _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",pkey.tostring(logbuf2,sizeof(logbuf2)-1),m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[pkey]);
            LOG_INFO2("维护平今优惠配额手数:\n%s",logbuf);
            //CFileOpr::getObj().writelocallog("维护平今优惠配额手数.log","%s",logbuf);
        }

    }
    else if((TradeInfo.OffsetFlag == THOST_FTDC_OF_Close||
             TradeInfo.OffsetFlag == THOST_FTDC_OF_CloseYesterday||
             TradeInfo.OffsetFlag == THOST_FTDC_OF_ForceClose||
             TradeInfo.OffsetFlag == THOST_FTDC_OF_LocalForceClose)&&
            !util::isInvalidValue(TradeInfo.CloseTodayVolume))
    {
        int CloseTdVolume=TradeInfo.CloseTodayVolume;
        int CloseYdVolume=TradeInfo.Volume-TradeInfo.CloseTodayVolume;

        //使用并维护郑商大商每个合约剩余的平今优惠配额手数
        //针对大商郑商。这两个交易所的平今优惠，不是按平掉的持仓明细是否今仓来判断，而是按是否有今仓可平来判断。
        //      比如，有昨仓1手今仓1手，平1手，平的是昨仓，但是由于有今仓，因此手续费优惠
        if(strExchangeID==string("DCE")||strExchangeID==string("CZCE"))
        {
            PositionKey pkey(TradeInfo);
            map<PositionKey,int>::iterator it=m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE.find(pkey);
            if(it!=m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE.end())
            {
                CloseTdVolume=TradeInfo.Volume;
                if(CloseTdVolume>it->second)
                    CloseTdVolume=it->second;
                CloseYdVolume=TradeInfo.Volume-CloseTdVolume;
                it->second-=CloseTdVolume;

                char logbuf[2048];
                char logbuf2[256];
                memset(logbuf,0,sizeof(logbuf));
                _snprintf(logbuf,sizeof(logbuf)-1,"根据成交(%s)计算手续费, 修改平今优惠配额手数\n",TradeKey(TradeInfo).tostring(logbuf2,sizeof(logbuf2)-1));
                _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\t[%s]-%d\n",pkey.tostring(logbuf2,sizeof(logbuf2)-1),m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE[pkey]);
                LOG_INFO2("维护平今优惠配额手数:\n%s",logbuf);
                //CFileOpr::getObj().writelocallog("维护平今优惠配额手数.log","%s",logbuf);
            }

        }

        if( (CloseTdVolume==0||!util::isInvalidValue(CommissionRate.CloseTodayRatioByMoney)&&!util::isInvalidValue(CommissionRate.CloseTodayRatioByVolume))&&
            (CloseYdVolume==0||!util::isInvalidValue(CommissionRate.CloseRatioByMoney)&&!util::isInvalidValue(CommissionRate.CloseRatioByVolume)))
        {
            TradeInfo.TradeCommission=  CloseTdVolume*TradeInfo.Price*InstrumentInfo.VolumeMultiple*CommissionRate.CloseTodayRatioByMoney+
                                        CloseTdVolume*CommissionRate.CloseTodayRatioByVolume+
                                        CloseYdVolume*TradeInfo.Price*InstrumentInfo.VolumeMultiple*CommissionRate.CloseRatioByMoney+
                                        CloseYdVolume*CommissionRate.CloseRatioByVolume;

            char logbuf[2048];
            char logbuf2[256];
            memset(logbuf,0,sizeof(logbuf));
            _snprintf(logbuf,sizeof(logbuf)-1,"根据平仓成交(%s)计算手续费\n",TradeKey(TradeInfo).tostring(logbuf2,sizeof(logbuf2)-1));
            _snprintf(logbuf+strlen(logbuf),sizeof(logbuf)-strlen(logbuf)-1,"\tCloseTdVolume=%d CloseYdVolume=%d TradeCommission=%g\n",CloseTdVolume,CloseYdVolume,TradeInfo.TradeCommission);
            LOG_INFO2("维护平今优惠配额手数:\n%s",logbuf);
            //CFileOpr::getObj().writelocallog("维护平今优惠配额手数.log","%s",logbuf);
        }
    }
}

///获取大于等于MinFTID的全部数据，返回数据项的数量。结果放在数组中。m_bUpdateSeq2FTID为true时有效
int CPlatformDataMgr_Trades::GetAll_GE_UpdateSeq(long MinUpdateSeq,vector<PlatformStru_TradeInfo>& outDatas)
{
    CeasymutexGuard guard(m_mutex);
    return m_TradeData.GetAll_GE_UpdateSeq_Internal(MinUpdateSeq,outDatas);
}

