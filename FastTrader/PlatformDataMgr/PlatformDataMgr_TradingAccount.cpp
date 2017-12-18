//维护底层持仓信息

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_TradingAccount.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CPlatformDataMgr_TradingAccount::CPlatformDataMgr_TradingAccount(CPlatformDataMgr_Instruments& refInstruments,
                                                                 CPlatformDataMgr_Quots& refQuots,
                                                                 CPlatformDataMgr_Trades& refTrades,
                                                                 CPlatformDataMgr_Orders& refOrders,
                                                                 CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
                                                                 CPlatformDataMgr_PositionDetails& refPositionDetails,
                                                                 CPlatformDataMgr_PositionCombs& refPositionCombs,
                                                                 CPlatformDataMgr_Positions& refPositions)
: m_RefInstruments(refInstruments),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetailCombs(refPositionDetailCombs),
  m_RefPositionDetails(refPositionDetails),
  m_RefPositionCombs(refPositionCombs),
  m_RefPositions(refPositions),
  m_bValid(false)
{
}

CPlatformDataMgr_TradingAccount::~CPlatformDataMgr_TradingAccount(void)
{
}

///设置交易资金账户信息
void CPlatformDataMgr_TradingAccount::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    Lock_CPlatformDataMgr;
	memcpy( &m_TradingAccount, &inData, sizeof(m_TradingAccount));

    //查询回来的冻结资金为0，自行计算一下
	if(util::isInvalidValue(m_TradingAccount.FrozenMargin)||util::isInvalidValue(m_TradingAccount.FrozenCommission))
		m_TradingAccount.FrozenCash=util::GetDoubleInvalidValue();
	else
		m_TradingAccount.FrozenCash = m_TradingAccount.FrozenMargin + m_TradingAccount.FrozenCommission;

    //ctp查询资金返回后，静态权益、动态权益、可用额度和风险度需要自己算
    CalcuStaticProfic_Internal();
    CalcuDynamicProfic_Internal();
    CalcuAvailable_Interanl();
    CalcuRiskDegree_Internal();
    m_bValid=true;
    Unlock_CPlatformDataMgr;
}
int CPlatformDataMgr_TradingAccount::GetTradingAccountAvailable(double& fAvailable)
{
    Lock_CPlatformDataMgr;
    fAvailable=m_TradingAccount.Available;
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
double CPlatformDataMgr_TradingAccount::GetTradingAccountDynamicProfit(void)
{
    Lock_CPlatformDataMgr;
	double DynamicProfit=m_bValid?m_TradingAccount.DynamicProfit:util::GetDoubleInvalidValue();
    Unlock_CPlatformDataMgr;
	return DynamicProfit;
}

int CPlatformDataMgr_TradingAccount::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    Lock_CPlatformDataMgr;
    fWithdrawQuota=m_TradingAccount.WithdrawQuota;
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
int CPlatformDataMgr_TradingAccount::GetTradingAccountID(char* AccountID,int rltsize)
{
    Lock_CPlatformDataMgr;
	strncpy(AccountID,m_TradingAccount.AccountID,rltsize);
    AccountID[rltsize-1]=0;
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
///获取交易资金账户信息
int CPlatformDataMgr_TradingAccount::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    Lock_CPlatformDataMgr;
	memcpy(&outData,&m_TradingAccount,sizeof(PlatformStru_TradingAccountInfo));
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
///获取账户资金文本信息
int CPlatformDataMgr_TradingAccount::GetAccountText(std::string& outData,int language)
{
    int ret=0;

    LOG_INFO("CPlatformDataMgr_TradingAccount::GetAccountText");

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

		_snprintf(temp,FMT_BUFFERSIZE,"  上次结算准备金:                         %s\r\n",AC_FM(m_TradingAccount.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 上次信用额度                             %s\r\n",AC_FM(m_TradingAccount.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 上次质押金额:                            %s\r\n",AC_FM(m_TradingAccount.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 质押金额:                                 %s\r\n",AC_FM(m_TradingAccount.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 今日出金:                                  %s\r\n",AC_FM(m_TradingAccount.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 今日入金:                                 %s\r\n",AC_FM(m_TradingAccount.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= 今日静态权益:                           %s\r\n",AC_FM(m_TradingAccount.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 平仓盈亏:                                 %s\r\n",AC_FM(m_TradingAccount.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		if(m_bValid&&m_TradingAccount.PositionProfit>0.0)
			_snprintf(temp,FMT_BUFFERSIZE,"+ 持仓盈亏:                                 %s(不计入可用资金)\r\n",AC_FM(m_TradingAccount.PositionProfit));
		else
			_snprintf(temp,FMT_BUFFERSIZE,"+ 持仓盈亏:                                 %s\r\n",AC_FM(m_TradingAccount.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 手续费:                                     %s\r\n",AC_FM(m_TradingAccount.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= 动态权益:                                %s\r\n",AC_FM(m_TradingAccount.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 占用保证金:                               %s\r\n",AC_FM(m_TradingAccount.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 冻结保证金:                               %s\r\n",AC_FM(m_TradingAccount.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 冻结手续费:                               %s\r\n",AC_FM(m_TradingAccount.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- 交割保证金:                               %s\r\n",AC_FM(m_TradingAccount.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ 信用金额                                  %s\r\n",AC_FM(m_TradingAccount.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= 可用资金:                                %s\r\n",AC_FM(m_TradingAccount.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= 保底资金:                                 %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= 可取资金:                                 %s\r\n",AC_FM(m_TradingAccount.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}
	if(language==LANGUAGE_ENGLISH)
	{
		_snprintf(temp,FMT_BUFFERSIZE,"  PreBalance:                                        %s\r\n",AC_FM(m_TradingAccount.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreCredit                                            %s\r\n",AC_FM(m_TradingAccount.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreMortgage:                                      %s\r\n",AC_FM(m_TradingAccount.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Mortgage:                                         %s\r\n",AC_FM(m_TradingAccount.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Today Withdraw:                                 %s\r\n",AC_FM(m_TradingAccount.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Today Deposit:                                   %s\r\n",AC_FM(m_TradingAccount.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Today Static Profit:                             %s\r\n",AC_FM(m_TradingAccount.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Close Profit:                                        %s\r\n",AC_FM(m_TradingAccount.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Position Profit:                                     %s\r\n",AC_FM(m_TradingAccount.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Commission:                                         %s\r\n",AC_FM(m_TradingAccount.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Dynamic Profit:                                   %s\r\n",AC_FM(m_TradingAccount.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Current Margin:                                    %s\r\n",AC_FM(m_TradingAccount.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Margin:                                     %s\r\n",AC_FM(m_TradingAccount.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Commission:                               %s\r\n",AC_FM(m_TradingAccount.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Delivery Margin:                                    %s\r\n",AC_FM(m_TradingAccount.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Credit                                                %s\r\n",AC_FM(m_TradingAccount.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= Available:                                           %s\r\n",AC_FM(m_TradingAccount.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= Minimum Quota:                                  %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= Withdraw Quota:                                %s\r\n",AC_FM(m_TradingAccount.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}

    Unlock_CPlatformDataMgr;

	LOG_INFO("CPlatformDataMgr_TradingAccount::GetAccountText-end");
	return ret;   
}
///查询报单响应时，动态更新资金账户信息
void CPlatformDataMgr_TradingAccount::UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData(void)
{
    Lock_CPlatformDataMgr;

    //取冻结手续费和冻结保证金
    m_RefOrders.GetFreezeMargin_Commission(m_TradingAccount.FrozenMargin,m_TradingAccount.FrozenCommission);

    //计算冻结金额
	if(util::isInvalidValue(m_TradingAccount.FrozenMargin)||util::isInvalidValue(m_TradingAccount.FrozenCommission))
		m_TradingAccount.FrozenCash=util::GetDoubleInvalidValue();
	else
		m_TradingAccount.FrozenCash = m_TradingAccount.FrozenMargin + m_TradingAccount.FrozenCommission;

    //更新资金数据中的可用资金
    CalcuAvailable_Interanl();

    Unlock_CPlatformDataMgr;
}


///行情更新时，动态更新资金账户信息
void CPlatformDataMgr_TradingAccount::DynamicUpdateTradingAccount_RtnDepthMarketData(const string InstrumentID)
{
    Lock_CPlatformDataMgr;

    //如果有持仓明细，才进行更新
    if(m_RefPositionDetails.HavePositionDetail(InstrumentID))
    {

        //计算逐日盯市的持仓盈亏
        m_RefPositions.GetAllPositionProfitByDate( m_TradingAccount.PositionProfit);
        //更新资金数据中的动态权益值
        CalcuDynamicProfic_Internal();
        //更新资金数据中的可用资金
        CalcuAvailable_Interanl();
        //更新资金数据中的风险度值
        CalcuRiskDegree_Internal();
    }

    Unlock_CPlatformDataMgr;
}
///成交回报时和查询持仓响应时，动态更新资金账户信息
void CPlatformDataMgr_TradingAccount::UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted(void)
{
    Lock_CPlatformDataMgr;

    m_RefPositions.GetAllPositionProfitByDate( m_TradingAccount.PositionProfit);
    m_RefPositions.GetAllCloseProfitByDate(m_TradingAccount.CloseProfit);

    double Margin1=0,Margin2=0;
    m_RefPositions.GetAllMargin(Margin1);
    m_RefPositionCombs.GetAllMargin(Margin2);
    m_TradingAccount.CurrMargin=Margin1+Margin2;

    //更新手续费
    m_TradingAccount.Commission=m_RefTrades.GetTotalCommission();
    //更新资金数据中的动态权益值
    CalcuDynamicProfic_Internal();
    //更新资金数据中的可用资金
    CalcuAvailable_Interanl();
    //更新资金数据中的风险度值
    CalcuRiskDegree_Internal();

    Unlock_CPlatformDataMgr;
}
///查询保证金率响应时，动态更新资金账户信息
void CPlatformDataMgr_TradingAccount::DynamicUpdateTradingAccount_RspQryRate(void)
{
    Lock_CPlatformDataMgr;

    //更新资金数据中的保证金
    double Margin1=0,Margin2=0;
    m_RefPositions.GetAllMargin(Margin1);
    m_RefPositionCombs.GetAllMargin(Margin2);
    m_TradingAccount.CurrMargin=Margin1+Margin2;

    //更新资金数据中的可用资金
    CalcuAvailable_Interanl();
    //更新资金数据中的风险度值
    CalcuRiskDegree_Internal();

    Unlock_CPlatformDataMgr;
}
///查询手续费率响应时，动态更新资金账户信息
void CPlatformDataMgr_TradingAccount::DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void)
{
    Lock_CPlatformDataMgr;

    m_TradingAccount.Commission=m_RefTrades.GetTotalCommission();

    //更新资金数据中的动态权益值
    CalcuDynamicProfic_Internal();
    //更新资金数据中的可用资金
    CalcuAvailable_Interanl();
    //更新资金数据中的风险度值
    CalcuRiskDegree_Internal();

    Unlock_CPlatformDataMgr;
}
void CPlatformDataMgr_TradingAccount::clear(void)
{
    Lock_CPlatformDataMgr;
    m_TradingAccount.clear();
    m_bValid=false;
    Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_TradingAccount::IsValid(void)
{
    return m_bValid;
}

//更新资金数据中的静态权益值
void CPlatformDataMgr_TradingAccount::CalcuStaticProfic_Internal()
{
    if(util::isInvalidValue(m_TradingAccount.PreBalance)||
        util::isInvalidValue(m_TradingAccount.PreCredit)||
        util::isInvalidValue(m_TradingAccount.PreMortgage)||
        util::isInvalidValue(m_TradingAccount.Mortgage)||
        util::isInvalidValue(m_TradingAccount.Withdraw)||
        util::isInvalidValue(m_TradingAccount.Deposit))
    {
        m_TradingAccount.DynamicProfit=util::GetDoubleInvalidValue();
    }
    else
    {
		m_TradingAccount.StaticProfit =  
            m_TradingAccount.PreBalance
            - m_TradingAccount.PreCredit
            - m_TradingAccount.PreMortgage
            + m_TradingAccount.Mortgage
            - m_TradingAccount.Withdraw
            + m_TradingAccount.Deposit;
    }        
}

//更新资金数据中的动态权益值
void CPlatformDataMgr_TradingAccount::CalcuDynamicProfic_Internal()
{
    if(util::isInvalidValue(m_TradingAccount.StaticProfit)||
        util::isInvalidValue(m_TradingAccount.CloseProfit)||
        util::isInvalidValue(m_TradingAccount.PositionProfit)||
        util::isInvalidValue(m_TradingAccount.Commission))
    {
        m_TradingAccount.DynamicProfit=util::GetDoubleInvalidValue();
    }
    else
    {
		m_TradingAccount.DynamicProfit =  
            m_TradingAccount.StaticProfit
            + m_TradingAccount.CloseProfit 
            + m_TradingAccount.PositionProfit
            - m_TradingAccount.Commission;
    }        
}
//更新资金数据中的可用资金
void CPlatformDataMgr_TradingAccount::CalcuAvailable_Interanl()
{
    if( util::isInvalidValue(m_TradingAccount.DynamicProfit)||
        util::isInvalidValue(m_TradingAccount.CurrMargin)||
        util::isInvalidValue(m_TradingAccount.FrozenMargin)||
        util::isInvalidValue(m_TradingAccount.FrozenCommission)||
        util::isInvalidValue(m_TradingAccount.DeliveryMargin)||
        util::isInvalidValue(m_TradingAccount.Credit)||
        util::isInvalidValue(m_TradingAccount.PositionProfit))
    {
        m_TradingAccount.Available = util::GetDoubleInvalidValue();
    }
    else
    {
	    m_TradingAccount.Available =    
            m_TradingAccount.DynamicProfit 
            - m_TradingAccount.CurrMargin 
            - m_TradingAccount.FrozenMargin 
            - m_TradingAccount.FrozenCommission 
            - m_TradingAccount.DeliveryMargin 
            + m_TradingAccount.Credit;
        if(m_TradingAccount.PositionProfit>0)
            m_TradingAccount.Available-=m_TradingAccount.PositionProfit;
    }
}
//更新资金数据中的风险度值
void CPlatformDataMgr_TradingAccount::CalcuRiskDegree_Internal()
{
    if(util::isInvalidValue(m_TradingAccount.DynamicProfit))
        m_TradingAccount.RiskDegree=util::GetDoubleInvalidValue();
	else if ( util::equals( m_TradingAccount.DynamicProfit, 0) )
	{
		m_TradingAccount.RiskDegree = 0;
	}
	else if(!util::isInvalidValue(m_TradingAccount.CurrMargin)&&
            !util::isInvalidValue(m_TradingAccount.DeliveryMargin))
	{
		m_TradingAccount.RiskDegree = 
            ( m_TradingAccount.CurrMargin + m_TradingAccount.DeliveryMargin ) * 100
            /m_TradingAccount.DynamicProfit;
	}
    else m_TradingAccount.RiskDegree=util::GetDoubleInvalidValue(); 
}
