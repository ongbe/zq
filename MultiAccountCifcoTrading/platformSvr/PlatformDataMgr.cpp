#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
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


CPlatformDataMgr::CPlatformDataMgr(CPlatFormService* pHost):
m_PlatFormService(pHost)
{
    InitializeCriticalSection(&m_CS);
    InitializeCriticalSection(&m_CS_InstrumentInfo);
    InitializeCriticalSection(&m_CS_Quot);

    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PlatformDataMgr.log");

    m_TradingAccount.Head.BID=BID_MAX;//标记m_TradingAccount无效
    m_bNeedCheckCommissionRateAndMarginRate=false;

    m_InvestorInfo.Head.BID=BID_MAX;  //标记m_InvestorInfo无效
    m_bSettlementInfoIsReady=false;
    m_bHavePostionDetail=false;
    m_ModuleDataQry=true;
	m_combinLegs.clear();
	m_tempOriPositionDetail.clear();
	m_tempTradeCombinLegs.clear();
	m_reqMarginRateError.clear();
	m_reqCommissionRateError.clear();

    //查询结果返回时，是否需要创建mapQryRlt. 对于持仓，需要创建，用于合并当日持仓和历史持仓
    m_PositionData.SetbNeedsetKeyOfQryRlt(true);
}

CPlatformDataMgr::~CPlatformDataMgr(void)
{
    LOG_INFO("CPlatformDataMgr::~CPlatformDataMgr(析构CPlatformDataMgr对象)");
    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }


    DeleteCriticalSection(&m_CS_Quot);
    DeleteCriticalSection(&m_CS_InstrumentInfo);
    DeleteCriticalSection(&m_CS);
}


///设置交易资金账户信息
void CPlatformDataMgr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    LOG_INFO("CPlatformDataMgr::SetTradingAccount1");

    Lock_CPlatformDataMgr;

    //SetAccountField(m_TradingAccount,inData.Thost );
    memcpy( &m_TradingAccount.TradingAccountField, &inData, sizeof(PlatformStru_TradingAccountInfo));

    Unlock_CPlatformDataMgr;


    LOG_INFO("CPlatformDataMgr::SetTradingAccount1-end");

}
void CPlatformDataMgr::SetAccountField(DataRspQryTradingAccountEx& accex, const CThostFtdcTradingAccountFieldEx& field )
{
	//SetAccountField(accex,*((CThostFtdcTradingAccountField*)&field));
}
void CPlatformDataMgr::SetAccountField(DataRspQryTradingAccount& field )
{
	PlatformStru_TradingAccountInfo& TradingAccountField=field.TradingAccountField;

	//if(PLATFORM_DEFAULT==PTYPE_CTP)
	{
		TradingAccountField.StaticProfit = TradingAccountField.PreBalance
			- TradingAccountField.PreCredit - TradingAccountField.PreMortgage
			+ TradingAccountField.Mortgage - TradingAccountField.Withdraw 
			+ TradingAccountField.Deposit;
        RefreshDynamicProficOfTradingAccount(TradingAccountField);
        RefreshAvailOfTradingAccount(TradingAccountField);
		if(util::isInvalidValue(TradingAccountField.FrozenMargin)
			||util::isInvalidValue(TradingAccountField.FrozenCommission))
		{
			TradingAccountField.FrozenCash=util::GetDoubleInvalidValue();

		}
		else
		{
			TradingAccountField.FrozenCash = TradingAccountField.FrozenMargin 
				+ TradingAccountField.FrozenCommission;
		}
        RefreshRiskDegreeOfTradingAccount(TradingAccountField);
	}
	//else
		TradingAccountField.DynamicProfit = TradingAccountField.CashIn;

}

//更新资金数据中的动态权益值
void CPlatformDataMgr::RefreshDynamicProficOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField)
{
    if(util::isInvalidValue(TradingAccountField.StaticProfit)||
        util::isInvalidValue(TradingAccountField.CloseProfit)||
        util::isInvalidValue(TradingAccountField.PositionProfit)||
        util::isInvalidValue(TradingAccountField.Commission))
    {
        TradingAccountField.DynamicProfit=util::GetDoubleInvalidValue();
    }
    else
    {
		TradingAccountField.DynamicProfit =  TradingAccountField.StaticProfit
		                                                    + TradingAccountField.CloseProfit + 
                                                            + TradingAccountField.PositionProfit
		                                                    - TradingAccountField.Commission;
    }        
}

//更新资金数据中的风险度值
void CPlatformDataMgr::RefreshRiskDegreeOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField)
{
    if(util::isInvalidValue(TradingAccountField.DynamicProfit))
        TradingAccountField.RiskDegree=util::GetDoubleInvalidValue();
	else if ( util::equals( TradingAccountField.DynamicProfit, 0) )
	{
		TradingAccountField.RiskDegree = 0;
	}
	else if(!util::isInvalidValue(TradingAccountField.CurrMargin)&&
            !util::isInvalidValue(TradingAccountField.DeliveryMargin))
	{
		TradingAccountField.RiskDegree = ( TradingAccountField.CurrMargin + 
                                                            TradingAccountField.DeliveryMargin )
			                                             *100
                                                         /TradingAccountField.DynamicProfit;
	}
    else TradingAccountField.RiskDegree=util::GetDoubleInvalidValue(); 
}

//更新资金数据中的可用资金
void CPlatformDataMgr::RefreshAvailOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField)
{
    if( util::isInvalidValue(TradingAccountField.DynamicProfit)||
        util::isInvalidValue(TradingAccountField.CurrMargin)||
        util::isInvalidValue(TradingAccountField.FrozenMargin)||
        util::isInvalidValue(TradingAccountField.FrozenCommission)||
        util::isInvalidValue(TradingAccountField.DeliveryMargin)||
        util::isInvalidValue(TradingAccountField.Credit)||
        util::isInvalidValue(TradingAccountField.PositionProfit))
    {
        TradingAccountField.Available = util::GetDoubleInvalidValue();
    }
    else
    {
	    TradingAccountField.Available =    TradingAccountField.DynamicProfit 
                                                          - TradingAccountField.CurrMargin 
                                                          - TradingAccountField.FrozenMargin 
		                                                  - TradingAccountField.FrozenCommission 
                                                          - TradingAccountField.DeliveryMargin 
                                                          + TradingAccountField.Credit;
        if(TradingAccountField.PositionProfit>0)
            TradingAccountField.Available-=TradingAccountField.PositionProfit;
    }
}

///设置交易资金账户信息
void CPlatformDataMgr::SetTradingAccount(const DataRspQryTradingAccount& outData)
{
    LOG_INFO("CPlatformDataMgr::SetTradingAccount2");

    Lock_CPlatformDataMgr;

    m_TradingAccount.Head = outData.Head;
	m_TradingAccount.nRequestID = outData.nRequestID;
	m_TradingAccount.bIsLast = outData.bIsLast;
	memcpy( &m_TradingAccount, &outData, sizeof(DataRspQryTradingAccount));
	m_TradingAccount.RspInfoField = outData.RspInfoField;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetTradingAccount2-end");
}
int CPlatformDataMgr::GetTradingAccountAvailable(double& fAvailable)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
        fAvailable=m_TradingAccount.TradingAccountField.Available;
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    return ret;   
}
int CPlatformDataMgr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
        fWithdrawQuota=m_TradingAccount.TradingAccountField.WithdrawQuota;
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    return ret;   
}
int CPlatformDataMgr::GetTradingAccountID(char* AccountID)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
		strcpy(AccountID,m_TradingAccount.TradingAccountField.AccountID);
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    return ret;   
}
///获取交易资金账户信息
int CPlatformDataMgr::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    int ret;

    LOG_INFO("CPlatformDataMgr::GetTradingAccount");

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        memset(&outData,0,sizeof(outData));
        ret= -1;
    }
    else
    {
		SetAccountField(m_TradingAccount);
		memcpy(&outData,&m_TradingAccount.TradingAccountField,sizeof(PlatformStru_TradingAccountInfo));
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetTradingAccount-end");
    return ret;   
}

///获取账户资金文本信息
int CPlatformDataMgr::GetAccountText(std::string& outData,int language)
{
    int ret=0;

    LOG_INFO("CPlatformDataMgr::GetAccountText");

    Lock_CPlatformDataMgr;

    std::string strTemp,strBlank,strSeparator,strSeparator2;
	strBlank.append(10,' ');
	strSeparator.append(50,'=');
	strSeparator+="\r\n";
	strSeparator2.append(100,'-');
	strSeparator2+="\r\n";
	outData+=strSeparator;
#define FMT_BUFFERSIZE  512
#define AC_FM(tag)		util::isInvalidValue(tag)?"-":GlobalFunc::GetAccountFormatString(tag,2).c_str()
	char temp[FMT_BUFFERSIZE+1]={0};
	if(language==LANGUAGE_CHINESE)
	{

		_snprintf(temp,FMT_BUFFERSIZE,"  上次结算准备金:                         %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 上次信用额度                             %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 上次质押金额:                            %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 质押金额:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 今日出金:                                  %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 今日入金:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= 今日静态权益:                           %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 平仓盈亏:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		if(m_TradingAccount.TradingAccountField.PositionProfit>0.0)
			_snprintf(temp,FMT_BUFFERSIZE,"+ 持仓盈亏:                                 %s(不计入可用资金)\r\n",AC_FM(m_TradingAccount.TradingAccountField.PositionProfit));
		else
			_snprintf(temp,FMT_BUFFERSIZE,"+ 持仓盈亏:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 手续费:                                     %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= 动态权益:                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 占用保证金:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 冻结保证金:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 冻结手续费:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 交割保证金:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 信用金额                                  %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= 可用资金:                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= 保底资金:                                 %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= 可取资金:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}
	if(language==LANGUAGE_ENGLISH)
	{
		_snprintf(temp,FMT_BUFFERSIZE,"  PreBalance:                                        %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreCredit                                            %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreMortgage:                                      %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Mortgage:                                         %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Today Withdraw:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Today Deposit:                                   %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Today Static Profit:                             %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Close Profit:                                        %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Position Profit:                                     %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Commission:                                         %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Dynamic Profit:                                   %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Current Margin:                                    %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Margin:                                     %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Commission:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Delivery Margin:                                    %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Credit                                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= Available:                                           %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= Minimum Quota:                                  %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= Withdraw Quota:                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}

    Unlock_CPlatformDataMgr;

	LOG_INFO("CPlatformDataMgr::GetAccountText-end");
	return ret;   
}

///动态更新资金账户中的冻结资金，查询报单返回、查询费率返回
void CPlatformDataMgr::DynamicUpdateTradingAccount_Freeze()
{
    Lock_CPlatformDataMgr;

    m_TradingAccount.TradingAccountField.FrozenMargin=0;
    m_TradingAccount.TradingAccountField.FrozenCommission=0;

    std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator iter;
    std::string strInstrumentID;

    for(iter = m_OrderData.m_all.begin();iter != m_OrderData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&
            (iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
        {
            if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_Open)
            {
                if(!util::isInvalidValue(iter->second->freezeMargin)&&iter->second->freezeMargin>0)
                    m_TradingAccount.TradingAccountField.FrozenMargin+=iter->second->freezeMargin;
            }

            if(!util::isInvalidValue(iter->second->troubleMoney)&&iter->second->troubleMoney>0)
                m_TradingAccount.TradingAccountField.FrozenCommission+=iter->second->troubleMoney;
        }
    }

    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///动态更新资金账户中的冻结资金，报单回报时调用
void CPlatformDataMgr::DynamicUpdateTradingAccount_Freeze_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                                    const PlatformStru_OrderInfo& OldOrderInfo)
{
    double DeltaValue;

    Lock_CPlatformDataMgr;

    DeltaValue=util::GetDoubleInvalidValue();
    if(OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
        DeltaValue=Order.freezeMargin;
    else if(!OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
    {
        if(Order.freezeMargin!=util::GetDoubleInvalidValue()&&
            OldOrderInfo.freezeMargin!=util::GetDoubleInvalidValue())
            DeltaValue=Order.freezeMargin-OldOrderInfo.freezeMargin;
    }
    if(DeltaValue==util::GetDoubleInvalidValue()&&
        m_TradingAccount.TradingAccountField.FrozenMargin!=util::GetDoubleInvalidValue())
        m_TradingAccount.TradingAccountField.FrozenMargin=util::GetDoubleInvalidValue();
    else m_TradingAccount.TradingAccountField.FrozenMargin += DeltaValue;

    DeltaValue=util::GetDoubleInvalidValue();
    if(OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
        DeltaValue=Order.troubleMoney;
    else if(!OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
    {
        if(Order.troubleMoney!=util::GetDoubleInvalidValue()&&
            OldOrderInfo.troubleMoney!=util::GetDoubleInvalidValue())
            DeltaValue=Order.troubleMoney-OldOrderInfo.troubleMoney;
    }
    if(DeltaValue==util::GetDoubleInvalidValue()&&
        m_TradingAccount.TradingAccountField.FrozenCommission!=util::GetDoubleInvalidValue())
        m_TradingAccount.TradingAccountField.FrozenCommission=util::GetDoubleInvalidValue();
    else m_TradingAccount.TradingAccountField.FrozenCommission += DeltaValue;

    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///动态更新资金账户中的交易佣金，查询成交返回、成交回报、查询费率返回
void CPlatformDataMgr::DynamicUpdateTradingAccount_TradeCommission_Internal()
{
    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount_TradeCommission_Internal");

    m_TradingAccount.TradingAccountField.Commission=0;

    std::map<TradeKey,  PlatformStru_TradeInfo*>::iterator iter;
    std::string strInstrumentID;

    for(iter = m_TradeData.m_all.begin();iter != m_TradeData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&!util::isInvalidValue(iter->second->TradeCommission))
            m_TradingAccount.TradingAccountField.Commission+=iter->second->TradeCommission;
        //else
        //{
        //    m_TradingAccount.TradingAccountField.Commission=util::GetDoubleInvalidValue();
        //    break;
        //}
    }

    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount_TradeCommission_Internal-end");
}



///行情更新时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RtnDepthMarketData(const std::string InstrumentID)
{
    Lock_CPlatformDataMgr;


    //如果有持仓明细，才进行更新
    if(m_PositionDetailData.HaveDataOfInstrumentID(InstrumentID))
    {
        GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);

        //更新资金数据中的动态权益值
        RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
        //更新资金数据中的风险度值
        RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
        //更新资金数据中的可用资金
        RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);
    }

    Unlock_CPlatformDataMgr;

}


///报单回报时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                            const PlatformStru_OrderInfo& OldOrderInfo)
{
    Lock_CPlatformDataMgr;

    DynamicUpdateTradingAccount_Freeze_RtnOrder(Order,OldOrderInfo);

    Unlock_CPlatformDataMgr;

}

///成交回报时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RtnTrade(void)
{
    Lock_CPlatformDataMgr;

    GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);
    GetAllMargin_CloseProfitByDate_Internal(m_TradingAccount.TradingAccountField.CurrMargin,m_TradingAccount.TradingAccountField.CloseProfit);

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //更新资金数据中的动态权益值
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的风险度值
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}


///查询持仓响应时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryInvestorPositionLast(void)
{
    Lock_CPlatformDataMgr;

    GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);
    GetAllMargin_CloseProfitByDate_Internal(m_TradingAccount.TradingAccountField.CurrMargin,m_TradingAccount.TradingAccountField.CloseProfit);

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //更新资金数据中的动态权益值
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的风险度值
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}


///查询成交响应时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryTrade(void)
{
    Lock_CPlatformDataMgr;

    GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);
    GetAllMargin_CloseProfitByDate_Internal(m_TradingAccount.TradingAccountField.CurrMargin,m_TradingAccount.TradingAccountField.CloseProfit);

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //更新资金数据中的动态权益值
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的风险度值
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///查询报单响应时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryOrder(void)
{
    DynamicUpdateTradingAccount_Freeze();
}


///查询保证金率响应时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryRate(void)
{
    Lock_CPlatformDataMgr;

    //更新资金数据中的动态权益值(查询保证金率响应时不需要更新动态权益)
    //RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的风险度值
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///查询手续费率响应时，动态更新资金账户信息
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void)
{
    Lock_CPlatformDataMgr;

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //更新资金数据中的动态权益值
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的风险度值
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //更新资金数据中的可用资金
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}


const string CPlatformDataMgr::Instrument2ExchangeID(const string& Instrument)
{
	//只适合测试服务器
	const string SHFE="SHFE";
	const string CZCE="CZCE";
	const string DCE="DCE";
	const string CFFEX="CFFEX";
	size_t len= Instrument.length();
	if(len==6)
	{
//		string produceID=Instrument.substr(0,2);
//		transform( produceID.begin(), produceID.end(), produceID.begin(), toupper);
//		if(produceID.compare("IF")==0)
        char ch1=Instrument.at(0);
        char ch2=Instrument.at(1);
        if(isupper(ch1)&&isupper(ch2))  //like IFxxxx、TFxxxx, which is instrument of cffex
			return CFFEX;
		else
			return SHFE;
	}
	else if(len==5)
	{
		char ch=Instrument.at(1);
		if(ch>='0'&&ch<='9')
			return DCE;
		else
			return CZCE;
	}
	else
		return "";
}


///设置指定合约信息
void CPlatformDataMgr::SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData)
{
	LOG_INFO("CPlatformDataMgr::SetInstrumentInfo");

    PlatformStru_InstrumentInfo  newData;
	memcpy(&newData,&inData,sizeof(PlatformStru_InstrumentInfo));

    //m_ServerParamCfg不需要保护(from ouyang)
    if(m_ServerParamCfg.vec[m_ServerParamCfg.current].CTPSimulate==1)
	{
		string strNewID=Instrument2ExchangeID(newData.InstrumentID);
		if(!strNewID.empty())
			strcpy(newData.ExchangeID,strNewID.c_str());
	}

    EnterCriticalSection(&m_CS_InstrumentInfo);

    m_allInstruments[InstrumentID] = newData;
    m_allInstrumentList.insert(InstrumentID);
    if(inData.ProductClass!=THOST_FTDC_PC_Combination)
        m_allInstrumentListWithoutComb.insert(InstrumentID);
    m_allProductID[inData.ProductID].insert(InstrumentID);

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    //这个也不需要保护
    m_bNeedCheckCommissionRateAndMarginRate=true;


    LOG_INFO("CPlatformDataMgr::SetInstrumentInfo-end");
}
///获取指定合约信息
int CPlatformDataMgr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo");
    int ret;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
    {
        memset(&outData,0,sizeof(outData));
        ret=-1;
    }
    else 
    {
        outData = it->second;
        ret=0;
    }
    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo-end");
    return ret;
}

///指定合约是否合法(存在)
bool CPlatformDataMgr::InstrumentIsValid(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::InstrumentIsValid");
    bool ret;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end()) ret=false;
    else ret=true;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::InstrumentIsValid-end");
    return ret;
}


///获取合约的产品类型，失败返回-1
char CPlatformDataMgr::GetProductClassType(const std::string& InstrumentID)
{
    //LOG_INFO("CPlatformDataMgr::GetProductClassType");
    char ProductClassType;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        ProductClassType=-1;
    else 
        ProductClassType = it->second.ProductClass;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    //LOG_INFO("CPlatformDataMgr::GetProductClassType-end");
    return ProductClassType;
}
///获取合约列表
int CPlatformDataMgr::GetInstrumentList(std::vector<GroupInfo> &outData)
{
	outData.clear();
    LOG_INFO("CPlatformDataMgr::GetInstrumentList");

    std::map<std::string,PlatformStru_InstrumentInfo> allInstruments; //当前所有合约信息
    std::map<std::string,PlatformStru_InstrumentInfo>::iterator it_all;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    allInstruments=m_allInstruments;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    Lock_CPlatformDataMgr;

	std::map<std::string,int> GroupName2ID;
	std::map<std::string,int>::iterator GroupName2ID_it;
    for(it_all=allInstruments.begin();it_all!=allInstruments.end();it_all++)
	{
#ifndef _USE_COMBOINSTRUMENT
        if(it_all->second.ProductClass==THOST_FTDC_PC_Combination)
            continue;
#endif
        GroupName2ID_it=GroupName2ID.find(it_all->second.ExchangeID);
		if(GroupName2ID_it==GroupName2ID.end())
		{
			GroupInfo ginfo;
            ginfo.GroupName=it_all->second.ExchangeID;
			ginfo.InstrVec.push_back(InstrumentInfo(it_all->second.InstrumentID,it_all->second.InstrumentName));
			outData.push_back(ginfo);
			GroupName2ID[it_all->second.ExchangeID]=outData.size()-1;
		}
		else
		{
			outData[GroupName2ID_it->second].InstrVec.push_back(InstrumentInfo(it_all->second.InstrumentID,it_all->second.InstrumentName));
		}
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
	return 0;
}
///获取合约列表
int CPlatformDataMgr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentList");

    outData.clear();

    EnterCriticalSection(&m_CS_InstrumentInfo);

    if(ExchangeID.empty())
    {
#ifndef _USE_COMBOINSTRUMENT
		outData = m_allInstrumentListWithoutComb;
#else
        outData = m_allInstrumentList;
#endif
    }
	else
	{
		outData.clear();
        std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it2;
        for(it2=m_allInstruments.begin();it2!=m_allInstruments.end();it2++)
        {
#ifndef _USE_COMBOINSTRUMENT
            if(it2->second.ProductClass==THOST_FTDC_PC_Combination)
                continue;
#endif
            if(std::string(it2->second.ExchangeID)==ExchangeID)
                outData.insert(it2->first);
        }
	}
    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
    return outData.size();
}
//添加主力合约列表
int CPlatformDataMgr::AddMainInstrumentList(std::string instrument)
{
    Lock_CPlatformDataMgr;

    m_allMainInstrumentList.insert(instrument);

    Unlock_CPlatformDataMgr;

	return m_allMainInstrumentList.size();
}
///获取合约列表
int CPlatformDataMgr::GetMainInstrumentList(std::set<std::string> &outData)
{
    LOG_INFO("CPlatformDataMgr::GetMainInstrumentList");

    outData.clear();


#ifdef _NEED_MAIN_INSTRUMENT
        Lock_CPlatformDataMgr;
		outData = m_allMainInstrumentList;
        Unlock_CPlatformDataMgr;
#else
        EnterCriticalSection(&m_CS_InstrumentInfo);
#ifndef _USE_COMBOINSTRUMENT
		outData = m_allInstrumentListWithoutComb;
#else
        outData = m_allInstrumentList;
#endif
        LeaveCriticalSection(&m_CS_InstrumentInfo);
#endif	


    LOG_INFO("CPlatformDataMgr::GetMainInstrumentList-end");
    return outData.size();
}
///设置合约容差列表
void CPlatformDataMgr::SetInstrumentVarietyMap(map<string, string>& inData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap");

    Lock_CPlatformDataMgr;

    m_allInstrumentVarietyMap = inData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap-end");
}

// 获取合约容差列表
int CPlatformDataMgr::GetInstrumentVarietyMap(map<string, string>& outData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap");

    Lock_CPlatformDataMgr;

    outData = m_allInstrumentVarietyMap;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap-end");
    return m_allInstrumentVarietyMap.size();
}

///获取指定品种的合约列表,返回合约个数
int CPlatformDataMgr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID");
    int ret;
    std::map<std::string, std::set<std::string> >::iterator it;

    outData.clear();

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it=m_allProductID.find(ProductID);
    if(it==m_allProductID.end()) 
        ret=-1;
    else
    {
        outData = it->second;
        ret=outData.size();
    }

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID-end");
    return ret;
}

///获取合约品种代码
int CPlatformDataMgr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr::GetProductID");

    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it2; m_allInstruments;
    outData.clear();
    EnterCriticalSection(&m_CS_InstrumentInfo);

    outData=m_allProductID;
	if(!ExchangeID.empty())
	{
		std::map<std::string, std::set<std::string> >::iterator it;
		for(it=outData.begin();it!=outData.end();)
		{
			if(it->second.size()==0||
                (it2=m_allInstruments.find(*(it->second.begin())))==m_allInstruments.end()||
                std::string(it2->second.ExchangeID)!=ExchangeID)
			{
				it=outData.erase(it);
			}
			else
				++it;
		}
	}

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    return outData.size(); 
    LOG_INFO("CPlatformDataMgr::GetProductID-end");
}


///获取合约乘数，成功返回合约乘数，失败返回-1
int CPlatformDataMgr::GetInstrumentMultiple(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple");
    int Multiple;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        Multiple=-1;
    else 
        Multiple= it->second.VolumeMultiple;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple-end");
    return Multiple;
}

///获取合约手续费率
int CPlatformDataMgr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetCommissionRate");
    int ret;

    Lock_CPlatformDataMgr;

    ret=GetCommissionRate_Internal(InstrumentID,outData);

	if(m_reqCommissionRateError[InstrumentID]>3)	
		ret=CustomErrorID;
    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr::GetCommissionRate-end");
    return ret;
}

///获取合约手续费率，内部调用，不用加锁
int CPlatformDataMgr::GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal");
    int ret;
    std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator it1;
    it1=m_InstrumentCommissionRate.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it1==m_InstrumentCommissionRate.end())
    {
        ret=-1;
    }
    else
    {
        outData = it1->second;
        ret=0;
    }
    LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal-end");
    return ret;
}

///设置合约手续费率
void CPlatformDataMgr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::SetCommissionRate");

    Lock_CPlatformDataMgr;

    m_InstrumentCommissionRate[InstrumentID]=outData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetCommissionRate-end");
}

///设置正在查询手续费率的合约,用于查询返回时对应
void CPlatformDataMgr::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::SetReqCommissionRateInstrumentID");

    Lock_CPlatformDataMgr;

    m_ReqCommissionRateInstrumentID=InstrumentID;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetReqCommissionRateInstrumentID-end");
}

///获取正在查询手续费率的合约
void CPlatformDataMgr::GetReqCommissionRateInstrumentID(std::string& outData)
{
    LOG_INFO("CPlatformDataMgr::GetReqCommissionRateInstrumentID");

    Lock_CPlatformDataMgr;

    outData=m_ReqCommissionRateInstrumentID;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetReqCommissionRateInstrumentID-end");
}

///获取合约保证金率
int CPlatformDataMgr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetMarginRate");
    int ret;

    Lock_CPlatformDataMgr;

    ret=GetMarginRate_Internal(InstrumentID,outData);

	if(m_reqMarginRateError[InstrumentID]>3)	
		ret=CustomErrorID;

	Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetMarginRate-end");
    return ret; 
}

///获取合约保证金率，内部调用，不用加锁
int CPlatformDataMgr::GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal");
    int ret;
    std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator it;
    it=m_InstrumentMarginRate.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it==m_InstrumentMarginRate.end())
    {
        ret=-1;
    }
    else
    {
        outData = it->second;
        ret=0;
    }
    LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal-end");
    return ret; 
}

///设置合约保证金率
void CPlatformDataMgr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::SetMarginRate");

    Lock_CPlatformDataMgr;

    m_InstrumentMarginRate[InstrumentID]=outData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetMarginRate-end");
}

//判断一个字符串日期是否是当天
bool CPlatformDataMgr::IsTodayPosition(const char *OpenDate)
{
    int tradingday=0;

	if(OpenDate==NULL) return false;

	tradingday=atoi(m_exchangeTime.TradingDay);
    if(tradingday==0) 
    {
    	char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
		time_t tCurdate = time(NULL);
		struct tm local;
		localtime_s(&local, &tCurdate);
		tradingday= ((local.tm_year+1900)*10000+(local.tm_mon+1))*100+local.tm_mday;
    }
    return (tradingday==atoi(OpenDate));
}

bool CPlatformDataMgr::IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail)
{
    int tradingday=0;

	tradingday=atoi(m_exchangeTime.TradingDay);
    if(tradingday==0) 
    {
    	char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
		time_t tCurdate = time(NULL);
		struct tm local;
		localtime_s(&local, &tCurdate);
		tradingday= ((local.tm_year+1900)*10000+(local.tm_mon+1))*100+local.tm_mday;
    }
    return (tradingday==atoi(PositionDetail.OpenDate));
}


//取全部持仓的占用保证金和平仓盈亏(逐日盯市)。
void CPlatformDataMgr::GetAllMargin_CloseProfitByDate_Internal(double& TotalMargin,double&TotalCloseProfitByDate)
{
	TotalMargin=0;
    TotalCloseProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        if(TotalMargin!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->UseMargin!=InvalideValue)
                TotalMargin+=iter->second->UseMargin;
            else TotalMargin=InvalideValue;
        }
        if(TotalCloseProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->CloseProfit!=InvalideValue)
                TotalCloseProfitByDate+=iter->second->CloseProfit;
            else TotalCloseProfitByDate=InvalideValue;
        }
	}
}


//取全部持仓的持仓盈亏(逐日盯市)
void CPlatformDataMgr::GetAllPositionProfitByDate_Internal(double&TotalPositionProfitByDate)
{
	TotalPositionProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        if(TotalPositionProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->PositionProfit!=InvalideValue)
                TotalPositionProfitByDate+=iter->second->PositionProfit;
            else TotalPositionProfitByDate=InvalideValue;
        }
	}
}

//取全部持仓的平仓盈亏(逐日盯市)
void CPlatformDataMgr::GetAllCloseProfitByDate_Internal(double&TotalCloseProfitByDate)
{
	TotalCloseProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        if(TotalCloseProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->CloseProfit!=InvalideValue)
                TotalCloseProfitByDate+=iter->second->CloseProfit;
            else TotalCloseProfitByDate=InvalideValue;
        }
	}
}

bool CompareTradeInfo(PlatformStru_TradeInfo& a,PlatformStru_TradeInfo& b)
{
    int i1,j1,len1,i2,j2,len2;

    for(i1=0;i1<sizeof(a.TradeID);i1++)
        if(a.TradeID[i1]!=' '&&a.TradeID[i1]!='0') break;
    for(j1=sizeof(a.TradeID)-1;j1>i1;j1--)
        if(a.TradeID[j1-1]!=' ') break;
    len1=j1-i1;

    for(i2=0;i2<sizeof(b.TradeID);i2++)
        if(b.TradeID[i2]!=' '&&b.TradeID[i2]!='0') break;
    for(j2=sizeof(b.TradeID)-1;j2>i2;j2--)
        if(b.TradeID[j2-1]!=' ') break;
    len2=j2-i2;

    if(len1<len2) return true;
    else if(len1>len2) return false;
    else return (memcmp(a.TradeID+i1,b.TradeID+i2,len1)<0?true:false);
}


//获取指定合约行情，成功返回0，失败返回-1
int CPlatformDataMgr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    int ret;

    EnterCriticalSection(&m_CS_Quot);

    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
    if(it==m_lastQuot.end())
    {
        memset(&outData,0,sizeof(outData));
        ret=-1;
    }
    else
    {
        outData = it->second;
        ret=0;
    }

    LeaveCriticalSection(&m_CS_Quot);

    return ret;
}

//设置指定合约行情，设置的行情更新，则返回true；否则返回false
//考虑组合合约的行情更新
//bFirstQuot表示是否是合约的首次行情，这种情况下需要更新持仓明细和持仓的占用保证金
bool CPlatformDataMgr::SetQuotInfo(const PlatformStru_DepthMarketData& inData,bool& bFirstQuot)
{
    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it = m_lastQuot.find(std::string(inData.InstrumentID));
    bool toBeContinue=true;

    bFirstQuot=false;


    EnterCriticalSection(&m_CS_Quot);

    if(it==m_lastQuot.end())
    {
        m_lastQuot[std::string(inData.InstrumentID)] = inData;
        bFirstQuot=true;
    }
    else
    {
        PlatformStru_DepthMarketData& OldBusiness= it->second;
        //比较行情的时间，丢掉老行情
        if (memcmp(inData.UpdateTime, OldBusiness.UpdateTime, sizeof(OldBusiness.UpdateTime))<0)
            toBeContinue=false;
        if(toBeContinue)
        {
            m_OldQuot[std::string(inData.InstrumentID)]=m_lastQuot[std::string(inData.InstrumentID)];
            m_lastQuot[std::string(inData.InstrumentID)] = inData;
        }
        else
        {
            LOG_INFO("CPlatformDataMgr::SetQuotInfo:Old BID_RtnDepthMarketData InstrumentID=[%s],OldUpdateTime=[%s],NewUpdateTime=[%s]",
                inData.InstrumentID,
                     OldBusiness.UpdateTime,
                     inData.UpdateTime);
        }
    }

    LeaveCriticalSection(&m_CS_Quot);

    return toBeContinue;

}


//将两个单腿合约合成组合合约，返回成功与否。合成时会检查合约列表里是否有该组合合约
bool CPlatformDataMgr::GetComboInstrumentID_Internal(const std::string strInstrumentID1, 
                                                     const std::string strInstrumentID2, 
                                                     std::string& strComInstrumentID,
                                                     std::string& strLeg1InstrumentID,
                                                     std::string& strLeg2InstrumentID)
{
    PlatformStru_InstrumentInfo InstrumentInfo1,InstrumentInfo2,InstrumentInfo3;
    if(GetInstrumentInfo(strInstrumentID1,InstrumentInfo1)==-1||
        GetInstrumentInfo(strInstrumentID2,InstrumentInfo2)==-1)
        return false;

    string Prefix;
    if(string(InstrumentInfo1.ExchangeID)==string("DCE"))
    {
        //大商所，SP a1209&a1301 或SPC l1209&v1209
        if(memcmp(InstrumentInfo1.ProductID,InstrumentInfo2.ProductID,sizeof(InstrumentInfo1.ProductID))==0)
            Prefix="SP ";
        else Prefix="SPC ";
    }
    else if(string(InstrumentInfo1.ExchangeID)==string("CZCE"))
    {
        //郑商所，只有跨期套利：SPD CF205&CF207
        Prefix="SPD ";
    }
    else return false;

    strComInstrumentID="SP "+strInstrumentID1+"&"+strInstrumentID2;
    if(GetInstrumentInfo(strComInstrumentID,InstrumentInfo3)==0) 
    {
        strLeg1InstrumentID=strInstrumentID1;
        strLeg2InstrumentID=strInstrumentID2;
        return true;
    }
    strComInstrumentID="SP "+strInstrumentID2+"&"+strInstrumentID1;
    if(GetInstrumentInfo(strComInstrumentID,InstrumentInfo3)==0) 
    {
        strLeg1InstrumentID=strInstrumentID2;
        strLeg2InstrumentID=strInstrumentID2;
        return true;
    }
    return false;
}

//获取指定合约的最新价
double CPlatformDataMgr::GetCurPrice (const std::string& InstrumentID)
{
    double ret=0.0;
	//组合合约
	char clstype=GetProductClassType(InstrumentID);
	if(clstype==THOST_FTDC_PC_Combination)
	{
		std::string id1,id2;
		ConvertComboInstrumentID(InstrumentID,id1,id2);
		double prc1=GetCurPrice(id1);
		double prc2=GetCurPrice(id2);
		ret= prc1-prc2;
	}
	else if(clstype==THOST_FTDC_PC_Futures)
	{
        EnterCriticalSection(&m_CS_Quot);
		
        std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		if(it!=m_lastQuot.end())
		{
            if(it->second.Volume>0) ret= it->second.LastPrice;
            else ret= it->second.PreSettlementPrice;
		}

        LeaveCriticalSection(&m_CS_Quot);
	}
    return ret;
}

//获取指定合约旧行情，成功返回0，失败返回-1
int CPlatformDataMgr::GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    int ret;

    EnterCriticalSection(&m_CS_Quot);

    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_OldQuot.find(InstrumentID);
    if(it==m_OldQuot.end())
    {
        memset(&outData,0,sizeof(outData));
        ret=-1;
    }
    else
    {
        outData = it->second;
        ret=0;
    }

    LeaveCriticalSection(&m_CS_Quot);

    return ret;
}



//设置投资者信息
void CPlatformDataMgr::SetInvestorInfo(const DataRspQryInvestor& inData)
{
    Lock_CPlatformDataMgr;

    m_InvestorInfo=inData;

    Unlock_CPlatformDataMgr;

}
string CPlatformDataMgr::GetTradingDay(const string& ExchangeID)
{
	if(m_exchangeTime.CurTickCount>0)
		return string(m_exchangeTime.TradingDay);
	return string("");
}

//获取交易所时间信息
unsigned long CPlatformDataMgr::GetExchangeTime(const string& ExchangeID,string& time)
{
    if(ExchangeID=="SHFE")
		time=m_exchangeTime.SHFETime;
	else if(ExchangeID=="DCE")
		time=m_exchangeTime.DCETime;
    else if(ExchangeID=="CZCE")
		time=m_exchangeTime.CZCETime;
	else if(ExchangeID=="CFFEX")
		time=m_exchangeTime.FFEXTime;
    return m_exchangeTime.CurTickCount;
}
int CPlatformDataMgr::GetExchangeTime(ExchangeTime& outData)
{
    if(m_exchangeTime.CurTickCount==0)
    {
        memset(&outData,0,sizeof(outData));
        return -1;
    }
    outData=m_exchangeTime;
    return 0;
}
	//获取全部交易所
int CPlatformDataMgr::GetExchangeIDs(vector<string>& outData)
{
    Lock_CPlatformDataMgr;

	outData.clear();
	outData.push_back("SHFE");
	outData.push_back("CZCE");
	outData.push_back("DCE");
	outData.push_back("CFFEX");

    Unlock_CPlatformDataMgr;

	return outData.size();
}
//设置交易所时间信息
void CPlatformDataMgr::SetExchangeTime(const ExchangeTime& inData)
{
    m_exchangeTime=inData;
}



//获取结算信息，成功返回0，失败返回-1
int CPlatformDataMgr::GetLastSettlementInfo(std::string& outData)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_bSettlementInfoIsReady)
    {
        outData=m_LastSettlementInfo;
        ret=0;
    }
    else
        ret=-1;

    Unlock_CPlatformDataMgr;

    return ret;
}

//清除结算信息
void CPlatformDataMgr::ClearLastSettlementInfo(void)
{
    Lock_CPlatformDataMgr;

    m_LastSettlementInfo.clear();
    m_bSettlementInfoIsReady=false;

    Unlock_CPlatformDataMgr;

}

//添加结算信息
void CPlatformDataMgr::AppendLastSettlementInfo(const std::string& inData,bool bLast)
{
    Lock_CPlatformDataMgr;

    if(m_bSettlementInfoIsReady)
    {
        m_LastSettlementInfo.clear();
        m_bSettlementInfoIsReady=false;
    }

    m_LastSettlementInfo.append(inData);
    if(bLast)
        m_bSettlementInfoIsReady=true;

    Unlock_CPlatformDataMgr;

}


//业务定制，成功返回0，失败返回-1
int CPlatformDataMgr::SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback)
{
    int ret;

    if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
        return -1;


    Lock_CPlatformDataMgr;

    ret=0;
    try
    {
        m_subscribeInfo[BID][GID]=callback;
    }
    catch(...)
    {
        ret=-2;
    }

    Unlock_CPlatformDataMgr;

    return ret;
}


///取消业务数据的定制，成功返回0，失败返回-1
int CPlatformDataMgr::UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID)
{
    int ret;

    if(BID>=BID_MAX || GID>=GID_MAX)
        return -1;

    Lock_CPlatformDataMgr;

    ret=0;

    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
    if(it1==m_subscribeInfo.end())
        ret=-3;
    else
    {
        std::map<GUIModuleID, GuiCallBackFunc>::iterator it2 = it1->second.find(GID);
        if(it2==it1->second.end())
            ret=-3;
        else
        {
            it1->second.erase(it2);
            if(it1->second.empty())
                m_subscribeInfo.erase(it1);
            ret=0;
        }
    }

    Unlock_CPlatformDataMgr;

    return ret;
}


///获取业务定制信息，成功返回0，失败返回-1
int CPlatformDataMgr::GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData)
{
    int ret=0;
    outData.clear();

    Lock_CPlatformDataMgr;

    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
    if(it1==m_subscribeInfo.end())
        ret=-1;
    else
    {
        outData=it1->second;
        ret=0;
    }

    Unlock_CPlatformDataMgr;

	return ret;
}



SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
{
	return m_ServerParamCfg;
}
void CPlatformDataMgr::SetModuleDataQry(bool bQry)
{
    m_ModuleDataQry=bQry;
}
bool CPlatformDataMgr::GetModuleDataQry()
{
    return m_ModuleDataQry;
}

//存储客户签约银行名称
void CPlatformDataMgr:: SaveBankName(CThostFtdcContractBankField& inData)
{
	LOG_INFO("CPlatformDataMgr::SaveBankName");
   
    m_BankName[inData.BankName]=inData;
    LOG_INFO("CPlatformDataMgr::SaveBankName-end");
}
//更新客户签约银行信息
void CPlatformDataMgr:: UpdateContractBank(CThostFtdcAccountregisterField& inData)
{
	LOG_INFO("CPlatformDataMgr::UpdateContractBank");
    PlatformStru_ContractBank   ContractBank;
	memset(&ContractBank,0,sizeof(PlatformStru_ContractBank));
    strcpy(ContractBank.BankID,inData.BankID);
	strcpy(ContractBank.BankAccount,inData.BankAccount);
	strcpy(ContractBank.CurrencyID,inData.CurrencyID);
	strcpy(ContractBank.BankBranchID,inData.BankBranchID);
	strcpy(ContractBank.BrokerID,inData.BrokerID);


	std::map<std::string,  CThostFtdcContractBankField>::iterator it=m_BankName.begin();
	for(; it!=m_BankName.end(); ++it)
	{
		if(!strcmp(inData.BankID, it->second.BankID))
		{
			strcpy(ContractBank.BankName,it->second.BankName);
			strcpy(ContractBank.BankBranchID,it->second.BankBrchID);
			break;

		}

	}

    Lock_CPlatformDataMgr;

	m_ContractBankInfo[ContractBank.BankName]=ContractBank;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::UpdateContractBank-end");
	//m_Accountregister.push_back(inData.BankID);
	//m_timer.Start(1000,true);
}

//获取客户签约银行信息
int CPlatformDataMgr::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
	LOG_INFO("CPlatformDataMgr::GetContractBank");

    Lock_CPlatformDataMgr;

	outData=m_ContractBankInfo;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetContractBank-end");
	return outData.size();

}


///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
void CPlatformDataMgr::SetCurrentPositionContent(int PositionContentMode)
{
    switch(PositionContentMode)
    {
        case 1:m_PlatFormService->SetQryPrior(1);      break;
        case 2:m_PlatFormService->SetQryPrior(2);      break;
        case 3:m_PlatFormService->SetQryPrior(3);      break;
    }
}
void CPlatformDataMgr::IncrementReqMarginRateError(const std::string& InstrumentID)
{
    Lock_CPlatformDataMgr
	if(m_reqMarginRateError.find(InstrumentID)==m_reqMarginRateError.end())
		m_reqMarginRateError[InstrumentID]=0;
	else
		m_reqMarginRateError[InstrumentID]++;
	Unlock_CPlatformDataMgr
}
void CPlatformDataMgr::IncrementReqCommissionRateError(const std::string& InstrumentID)
{
    Lock_CPlatformDataMgr
	if(m_reqCommissionRateError.find(InstrumentID)==m_reqCommissionRateError.end())
		m_reqCommissionRateError[InstrumentID]=0;
	else
		m_reqCommissionRateError[InstrumentID]++;
	Unlock_CPlatformDataMgr
}

