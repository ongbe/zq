#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "ReqOnRtnTrade.h"
#include "../inc/Module-Misc/GlobalConfigMgr.h"
#pragma warning(disable:4996)


CReqOnRtnTrade::CReqOnRtnTrade(void)
{
    InitializeCriticalSection(&m_CS);

}

//注意：为了避免释放资源时其它线程还使用者m_CS，这里不释放m_CS，待应用程序结束时自动释放
CReqOnRtnTrade::~CReqOnRtnTrade(void)
{
	DeleteCriticalSection(&m_CS);
}




//有成交回报到来
void CReqOnRtnTrade::OnRtnTrade(const std::string& strBrokerID,
                                const std::string& strInvestorID,
                                const std::string& strInstrumentID,
                                bool bPositionComb,
                                int RecordCountOfPositionDetail)
{
    Stru_ReqOnRtnTradeTime ReqTime;


    if(!GlobalConfigManager::IsReq_OnRtnTrade())
        return;

    if(strInstrumentID.empty())
        return;

    Stru_ReqKeyOnRtnTrade ReqKey(strBrokerID,strInvestorID,strInstrumentID);

    EnterCriticalSection(&m_CS);

    std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime>::iterator it;
    it=m_mapReqTime.find(ReqKey);
    if(it==m_mapReqTime.end())
    {
        m_mapReqTime.insert(std::make_pair<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime>(ReqKey,ReqTime));
        it=m_mapReqTime.find(ReqKey);
    }

    if(it!=m_mapReqTime.end())
    {
        it->second.OnRtnTradeTime=GetTickCount();
        it->second.QryPositionDetailTime=0;
        it->second.QryPositionTime=0;
        it->second.QryAccountTime=0;
        it->second.bQryPositionComb=bPositionComb;
        it->second.RecordCountOfPositionDetail=RecordCountOfPositionDetail;
    }

    LeaveCriticalSection(&m_CS);

}


//设置持仓查询优先级，1:持仓；2:持仓明细；3:组合持仓
void CReqOnRtnTrade::SetQryPrior(int QryPrior)
{

    if(QryPrior==1||QryPrior==2||QryPrior==3)
        m_QryPrior=QryPrior;
}


//持仓查询优先级，1:持仓；2:持仓明细；3:组合持仓。
//查询顺序(如m_QryPrior==1)： 持仓、资金、持仓明细、组合持仓
//注意，资金查询一定在持仓之后
void CReqOnRtnTrade::CheckOnTime(CServiceProxy& rServiceProxy)
{
    DWORD dwTime;


    if(!GlobalConfigManager::IsReq_OnRtnTrade())
        return;

    EnterCriticalSection(&m_CS);

    dwTime=GetTickCount();
    std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime>::iterator it;
    for(it=m_mapReqTime.begin();it!=m_mapReqTime.end();)
    {
        if((dwTime<it->second.OnRtnTradeTime||
            (int)(dwTime-it->second.OnRtnTradeTime)>=GlobalConfigManager::GetDelayBefReq_OnRtnTrade()*1000)&&
            rServiceProxy.IsQryQueueEmpty())
        {
            if(m_QryPrior==1&&it->second.QryPositionTime==0)
            {
                ReqPosition_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(m_QryPrior==2&&it->second.QryPositionDetailTime==0&&
                it->second.RecordCountOfPositionDetail<GlobalConfigManager::GetMaxRecordReqPositionDetail_OnRtnTrade())
            {
                ReqPositionDetail_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(m_QryPrior==3&&it->second.QryPositionCombTime==0&&it->second.bQryPositionComb)
            {
                ReqPositionComb_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryPositionTime==0)
            {
                ReqPosition_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryAccountTime==0)
            {
                ReqTradingAccount_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryPositionDetailTime==0&&
                    it->second.RecordCountOfPositionDetail<GlobalConfigManager::GetMaxRecordReqPositionDetail_OnRtnTrade())
            {
                ReqPositionDetail_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryPositionCombTime==0&&it->second.bQryPositionComb)
            {
                ReqPositionComb_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else
            {
                //都查完了
                it=m_mapReqTime.erase(it);
            }
        }
        else 
        {
            it++;
        }
    }
    
    LeaveCriticalSection(&m_CS);
}

//查询持仓
void CReqOnRtnTrade::ReqPosition_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
										  Stru_ReqOnRtnTradeTime& ReqTime,
										  CServiceProxy& rServiceProxy)
{
    PlatformStru_QryInvestorPosition QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);
    strncpy(QryData.InstrumentID,ReqKey.strInstrumentID.c_str(),sizeof(QryData.InstrumentID)-1);

    rServiceProxy.ReqQryInvestorPosition(QryData);
    ReqTime.QryPositionTime=GetTickCount();
}

//查询持仓明细
void CReqOnRtnTrade::ReqPositionDetail_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
												Stru_ReqOnRtnTradeTime& ReqTime,
												CServiceProxy& rServiceProxy)
{
    PlatformStru_QryInvestorPositionDetail QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);
    strncpy(QryData.InstrumentID,ReqKey.strInstrumentID.c_str(),sizeof(QryData.InstrumentID)-1);

	rServiceProxy.ReqQryInvestorPositionDetail(QryData);
    ReqTime.QryPositionDetailTime=GetTickCount();
}

//查询组合持仓明细
void CReqOnRtnTrade::ReqPositionComb_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
											  Stru_ReqOnRtnTradeTime& ReqTime,
											  CServiceProxy& rServiceProxy)
{
    PlatformStru_QryInvestorPositionCombineDetail QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);
    strncpy(QryData.CombInstrumentID,ReqKey.strInstrumentID.c_str(),sizeof(QryData.CombInstrumentID)-1);

    rServiceProxy.ReqQryInvestorPositionCombineDetail(QryData);
    ReqTime.QryPositionCombTime=GetTickCount();
}

//查询资金
void CReqOnRtnTrade::ReqTradingAccount_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
												Stru_ReqOnRtnTradeTime& ReqTime,
												CServiceProxy& rServiceProxy)
{
    PlatformStru_QryTradingAccount QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);

    rServiceProxy.ReqQryTradingAccount(QryData);
    ReqTime.QryAccountTime=GetTickCount();
}
