//维护底层成交统计记录信息

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
#include "../inc/Module-Misc/FieldValueTool.h"
#pragma warning(disable:4996)

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

extern PlatformSvrMgr* g_pPlatformMgr;

void CPlatformDataMgr::QryTradeTotalReach(const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryTradeTotalReach");

    Lock_CPlatformDataMgr;

    if(bIsLast)
    {
		//先清空原有记录
        PlatformStru_TradeTotalInfo BlankData;
        m_TradeTotalData.WillQry_Internal(std::string(""));

        m_TradeTotalData.QryRltReach_Internal(BlankData,CustomErrorID,true);

        //从成交记录生成成交统计
        std::map<TradeKey,  PlatformStru_TradeInfo*>::iterator it_all;
        std::set<TradeTotalKey> setTradeTotalKey;
        std::set<TradeTotalKey>::iterator it_set;

        for(it_all=m_TradeData.m_all.begin();it_all!=m_TradeData.m_all.end();it_all++)
        {
            if(it_all->second==NULL) continue;
			TradeTotalKey tmpKey(*(it_all->second));
            if(setTradeTotalKey.find(tmpKey)==setTradeTotalKey.end())
            {
                setTradeTotalKey.insert(tmpKey);
            }
        }

		PlatformStru_TradeTotalInfo TradeTotal;
        for(it_set=setTradeTotalKey.begin();it_set!=setTradeTotalKey.end();it_set++)
        {
			if(!GetTradeTotalDataValue_Internal(*it_set,TradeTotal))
				continue;

			m_TradeTotalData.NewReach_Internal(TradeTotal);
        }
    }

    Unlock_CPlatformDataMgr;


    LOG_INFO("CPlatformDataMgr::QryTradeReach-end");
}


//获得手续费率响应时，更新相应合约的成交统计
bool CPlatformDataMgr::UpdateTradeTotal_RspQryCommissionRate(const std::string& strInstrument)
{
	bool brlt=false;

    Lock_CPlatformDataMgr;

	
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
	std::map<long,PlatformStru_TradeTotalInfo*>::iterator it_tradetotal2;
	PlatformStru_TradeTotalInfo TradeTotal;

    it_Index2=m_TradeTotalData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_TradeTotalData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
		    it_tradetotal2=m_TradeTotalData.m_all2.find(*itFTID);
    		
		    if(it_tradetotal2!=m_TradeTotalData.m_all2.end()&&
                it_tradetotal2->second!=NULL)
		    {
                TradeTotalKey tmpkey(*(it_tradetotal2->second));
                if(GetTradeTotalDataValue_Internal(tmpkey,TradeTotal)&&
			       TradeTotal.TradeCommission!=it_tradetotal2->second->TradeCommission)
			    {
				    it_tradetotal2->second->TradeCommission=TradeTotal.TradeCommission;
				    it_tradetotal2->second->UpdateSeq=m_TradeTotalData.NewUpdateSeq();
				    brlt=true;
			    }
		    }
        }
	}

    Unlock_CPlatformDataMgr;

	return brlt;
}

//成交回报时，更新相应合约的成交统计
void CPlatformDataMgr::UpdateTradeTotal_RtnTrade(const PlatformStru_TradeInfo& TradeInfo)
{
    Lock_CPlatformDataMgr;

    UpdateTradeTotalByTrade_Internal(TradeInfo);

    Unlock_CPlatformDataMgr;

}

void CPlatformDataMgr::UpdateTradeTotalByTrade_Internal(const PlatformStru_TradeInfo& TradeInfo)
{
    TradeTotalKey tmpKey(TradeInfo);

    std::map<TradeTotalKey,PlatformStru_TradeTotalInfo*>::iterator it;
    it=m_TradeTotalData.m_all.find(tmpKey);
    if(it==m_TradeTotalData.m_all.end()||it->second==NULL)
    {
        PlatformStru_TradeTotalInfo TradeTotal;

        strncpy(TradeTotal.szAccount,TradeInfo.szAccount,sizeof(TradeTotal.szAccount)-1);
        strncpy(TradeTotal.ExchangeID,TradeInfo.ExchangeID,sizeof(TradeTotal.ExchangeID)-1);
        strncpy(TradeTotal.InstrumentID,TradeInfo.InstrumentID,sizeof(TradeTotal.InstrumentID)-1);
        TradeTotal.Direction=TradeInfo.Direction;
        TradeTotal.OffsetFlag=TradeInfo.OffsetFlag;

        UpdateTradeTotalDataByTrade_Internal(TradeInfo,TradeTotal);

        m_TradeTotalData.NewReach_Internal(TradeTotal);
    }
    else
    {
        PlatformStru_TradeTotalInfo OldTradeTotal=*(it->second);

        UpdateTradeTotalDataByTrade_Internal(TradeInfo,*(it->second));

        if(it->second->price!=OldTradeTotal.price||
            it->second->volume!=OldTradeTotal.volume||
            it->second->TradeCommission!=OldTradeTotal.TradeCommission)
        {
            it->second->UpdateSeq=m_TradeTotalData.NewUpdateSeq();
        }
    }
}



///获取成交统计
bool CPlatformDataMgr::GetTradeTotalDataValue_Internal(const TradeTotalKey& TotalKey,PlatformStru_TradeTotalInfo& TradeTotalInfo)
{
    bool brlt=false;
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_TradeInfo*>::iterator it2;

    TradeTotalInfo.Clear();
    it_Index2=m_TradeData.m_allIndex2.find(TotalKey.InstrumentID);
    if(it_Index2!=m_TradeData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it2=m_TradeData.m_all2.find(*itFTID);

            if(it2==m_TradeData.m_all2.end()) continue;
            if(it2->second==NULL) continue;
		    if(std::string(it2->second->szAccount)!=TotalKey.Account) continue;
            if(it2->second->Direction!=TotalKey.Direction) continue;
            if(it2->second->OffsetFlag!=TotalKey.OffsetFlag) continue;

            TradeTotalInfo.price+=it2->second->Price*it2->second->Volume;
            TradeTotalInfo.volume+=it2->second->Volume;
			if(util::isInvalidValue(TradeTotalInfo.TradeCommission)||util::isInvalidValue(it2->second->TradeCommission))
				TradeTotalInfo.TradeCommission=util::GetDoubleInvalidValue();
			else
				TradeTotalInfo.TradeCommission+=it2->second->TradeCommission;
			
            if(!brlt)
            {
                strncpy(TradeTotalInfo.szAccount,TotalKey.Account,sizeof(TradeTotalInfo.szAccount)-1);
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

    return brlt;
}

///根据成交更新成交统计
bool CPlatformDataMgr::UpdateTradeTotalDataByTrade_Internal(const PlatformStru_TradeInfo& TradeInfo,PlatformStru_TradeTotalInfo& TradeTotalInfo)
{
    if(std::string(TradeInfo.szAccount)!=std::string(TradeTotalInfo.szAccount)) return false;
    if(std::string(TradeInfo.ExchangeID)!=std::string(TradeTotalInfo.ExchangeID)) return false;
    if(std::string(TradeInfo.InstrumentID)!=std::string(TradeTotalInfo.InstrumentID)) return false;
    if(TradeInfo.Direction!=TradeTotalInfo.Direction) return false;
    if(TradeInfo.OffsetFlag!=TradeTotalInfo.OffsetFlag) return false;

    if(TradeInfo.Volume==0) return true;

    if(TradeTotalInfo.volume+TradeInfo.Volume==0) TradeTotalInfo.price=0;
    else TradeTotalInfo.price = (TradeTotalInfo.price*TradeTotalInfo.volume+TradeInfo.Price*TradeInfo.Volume)/(TradeTotalInfo.volume+TradeInfo.Volume);
    TradeTotalInfo.volume += TradeInfo.Volume;
	if(util::isInvalidValue(TradeTotalInfo.TradeCommission)||util::isInvalidValue(TradeInfo.TradeCommission))
		TradeTotalInfo.TradeCommission=util::GetDoubleInvalidValue();
	else
		TradeTotalInfo.TradeCommission += TradeInfo.TradeCommission;

    return true;
}




///获取全部成交统计记录
int CPlatformDataMgr::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_TradeTotalData.GetAll2_Internal(outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

///获取指定合约的成交统计记录成功返回0，失败返回-1
int CPlatformDataMgr::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_TradeTotalData.GetAllAboutInstrument2_Internal(strInstrument,outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}



