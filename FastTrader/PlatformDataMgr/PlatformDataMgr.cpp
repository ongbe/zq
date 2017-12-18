#pragma warning(disable:4996)

#include "PlatformDataMgr.h"

#include <string>
#include <map>
using namespace std;

#include "FileOpr.h"
#include "Module-Misc2/tools_util.h"
#include "Module-Misc2/GlobalFunc.h"
#include "PlatformDataMgrMacro.h"



#ifdef WIN32
#pragma unmanaged
#endif 


volatile long CPlatformDataMgr::m_baseFTID=1;                              //FTID基准值，每次使用后递增
volatile long CPlatformDataMgr::m_baseUpdateSeq=1;                         //UpdateSeq基准值，每次使用后递增


CPlatformDataMgr::CPlatformDataMgr( bool bCalculate_OnRtnTrade,
                                    bool bUseComb,
                                    int DiscountCZCE_Comb,
                                    int DiscountDCE_Comb,
                                    int UnwindPrioritiesCZCE_Comb,
                                    int UnwindPrioritiesDCE_Comb,
                                    int PriceForMarginOfTodayPositionCZCE_Comb,
                                    int PriceForMarginOfTodayPositionDCE_Comb,
                                    bool bFreezeOrderUnknownStatus,
                                    int PlatformID,
                                    int MaxPlatformID,
                                    bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
:   m_PlatformID(PlatformID),
    m_MaxPlatformID(MaxPlatformID),
    m_bInvestorInfoValid(false),
    m_bCalculate_OnRtnTrade(bCalculate_OnRtnTrade),
    m_bUseComb(bUseComb),
    m_DiscountCZCE_Comb(DiscountCZCE_Comb),
    m_DiscountDCE_Comb(DiscountDCE_Comb),
    m_UnwindPrioritiesCZCE_Comb(UnwindPrioritiesCZCE_Comb),
    m_UnwindPrioritiesDCE_Comb(UnwindPrioritiesDCE_Comb),
    m_PriceForMarginOfTodayPositionCZCE_Comb(PriceForMarginOfTodayPositionCZCE_Comb),
    m_PriceForMarginOfTodayPositionDCE_Comb(PriceForMarginOfTodayPositionDCE_Comb),
    m_bHaveBrokerTradingParam(false),
    m_LoginInfo(),
    m_Instruments(m_bUseComb),
    m_Rates(),
    m_Quots(m_Instruments),
    m_Trades(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates),
    m_TradeTotals(m_baseFTID,m_baseUpdateSeq,m_Trades),
    m_Orders(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates,m_Quots,m_DiscountCZCE_Comb,m_DiscountDCE_Comb,bFreezeOrderUnknownStatus),
    m_PositionDetailCombs(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Quots,m_Trades,m_Orders),
    m_PositionDetails(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates,m_Quots,m_Trades,m_Orders,m_PositionDetailCombs,m_DiscountCZCE_Comb,m_DiscountDCE_Comb,m_UnwindPrioritiesCZCE_Comb,m_UnwindPrioritiesDCE_Comb,m_PriceForMarginOfTodayPositionCZCE_Comb,m_PriceForMarginOfTodayPositionDCE_Comb,bUseSettlementPriceToCalculateProfitAndMarginAsPossible),
    m_PositionCombs(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Quots,m_Trades,m_Orders,m_PositionDetailCombs,m_PositionDetails),
    m_Positions(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates,m_Quots,m_Trades,m_Orders,m_PositionDetails,bUseSettlementPriceToCalculateProfitAndMarginAsPossible),
    m_TradingAccount(m_Instruments,m_Quots,m_Trades,m_Orders,m_PositionDetailCombs,m_PositionDetails,m_PositionCombs,m_Positions)
{
    memset(&m_BrokerTradingParam,0,sizeof(m_BrokerTradingParam));
    memset(&m_InvestorInfo,0,sizeof(m_InvestorInfo));
    m_bSettlementInfoIsReady=false;
}

CPlatformDataMgr::~CPlatformDataMgr(void)
{
    LOG_INFO("CPlatformDataMgr::~CPlatformDataMgr(析构CPlatformDataMgr对象)");
}






//设置投资者信息
void CPlatformDataMgr::SetInvestorInfo(const CThostFtdcInvestorField& inData)
{
    Lock_CPlatformDataMgr;

    m_InvestorInfo=inData;
    m_bInvestorInfoValid=true;

    Unlock_CPlatformDataMgr;

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



SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
{
	return m_ServerParamCfg;
}

//清除所有数据
void CPlatformDataMgr::clear(bool bAlwaysClearInstruments_Rates)
{
    Lock_CPlatformDataMgr;

	m_LoginInfo.clear();
	if(bAlwaysClearInstruments_Rates)
	{
		m_Instruments.clear();
		m_Rates.clear();
	}
	m_Quots.clear();
	m_Trades.clear();
	m_TradeTotals.clear();
	m_Orders.clear();
	m_PositionDetailCombs.clear();
	m_PositionCombs.clear();
	m_PositionDetails.clear();
	m_Positions.clear();
	m_TradingAccount.clear();

    Unlock_CPlatformDataMgr;
}


//存储客户签约银行名称
void CPlatformDataMgr::SaveBankName(CThostFtdcContractBankField& inData)
{
    m_BankName[inData.BankName]=inData;
}

//保存汇率信息
void CPlatformDataMgr::SaveExchangeRate(const PlatformStru_ExchangeRate& ExchangeRate)
{
    m_ExchangeRates[pair<string,string>(ExchangeRate.FromCurrencyID,ExchangeRate.ToCurrencyID)]=ExchangeRate;
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

    string strBankName(ContractBank.BankName);
	m_ContractBankInfo[strBankName]=ContractBank;

    //保存银行对应的币种信息
    string strCurrency(ContractBank.CurrencyID);
    map<string,vector<string>>::iterator itCurrency=m_BankCurrency.find(strBankName);
    if(itCurrency==m_BankCurrency.end())
    {
        m_BankCurrency[strBankName]=vector<string>();
        itCurrency=m_BankCurrency.find(strBankName);
    }
    if(!CTools_Ansi::IsValueInVector<string>(itCurrency->second,strCurrency))
        itCurrency->second.push_back(strCurrency);


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

//处理深度行情
void CPlatformDataMgr::OnRtnDepthMarketData(const PlatformStru_DepthMarketData& MarketData)
{
    string strInstrument(MarketData.InstrumentID);
    if(strInstrument.empty()) return;

    //取合约信息
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo)) return;

    m_Quots.SetQuotInfo(MarketData);

    static bool b1=true,b2=true,b3=true;
    //更新持仓明细记录的持仓盈亏、浮动盈亏、结算价、昨结算价、现价
    if(b1) m_PositionDetails.UpdatePositionDetail_RtnDepthMarketData(MarketData,InstrumentInfo);
    //更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)、盯市总盈亏、结算价、昨结算价
    if(b2) m_Positions.UpdatePosition_RtnQuot(MarketData,InstrumentInfo,false);
    //更新资金账户信息
    if(b3) m_TradingAccount.DynamicUpdateTradingAccount_RtnDepthMarketData(strInstrument);
}

//请求查询行情响应。当组合合约报单需要行情的昨结算价时，会触发查询单腿合约的行情
void CPlatformDataMgr::OnRspQryDepthMarketData(const PlatformStru_DepthMarketData& MarketData)
{
    string strInstrument(MarketData.InstrumentID);
    if(strInstrument.empty()) return;

    //取合约信息
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo)) return;

    m_Quots.SetQuotInfo(MarketData);

    //更新报单冻结保证金和冻结手续费
    bool bUpdateMargin=m_Orders.UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(strInstrument,InstrumentInfo);
    bool bUpdateCommission=m_Orders.UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(strInstrument,InstrumentInfo);
    if(bUpdateMargin||bUpdateCommission)
    {
        //更新资金中的冻结数据
        m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
    }
}

//报单回报
void CPlatformDataMgr::OnRtnOrder(PlatformStru_OrderInfo& OrderInfo)
{
    string strInstrumentID(OrderInfo.InstrumentID);
    if(strInstrumentID.empty()) return;

    //取合约信息
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) return;

    PlatformStru_OrderInfo OldOrderInfo;

    //更新报单数据
    m_Orders.NewOrderReady(OrderInfo,OldOrderInfo,InstrumentInfo);

    //更新单合约持仓冻结和平仓量。
    m_Positions.UpdatePositionFreezeFields_RtnOrder(OrderInfo, OldOrderInfo, InstrumentInfo);

    if(1)
    {
        PlatformStru_OrderInfo tmpOrderInfo;
        if(m_Orders.GetOrder(OrderKey(OrderInfo),tmpOrderInfo))
        {
            char buf[1024];
            sprintf(buf,"BID_RtnOrder: %s/%s/%g/%g  ",tmpOrderInfo.InstrumentID,tmpOrderInfo.OrderSysID,tmpOrderInfo.freezeMargin,tmpOrderInfo.troubleMoney);
            CFileOpr::getObj().writelocallog("报单冻结计算",buf);
        }
    }

    //更新资金数据
    m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
}

//成交回报
void CPlatformDataMgr::OnRtnTrade(PlatformStru_TradeInfo& TradeInfo)
{
    string strInstrumentID(TradeInfo.InstrumentID);
    if(strInstrumentID.empty()) return;

    //取合约信息
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) return;

    //更新成交记录
    m_Trades.NewTradeReady(TradeInfo,InstrumentInfo);

    //更新成交统计
	m_TradeTotals.UpdateTradeTotal_RtnTrade(TradeInfo);

	if(m_bCalculate_OnRtnTrade)
	{
        //被成交修改的持仓明细记录
        map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
        //被修改的组合持仓明细记录
        map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;
        //平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
        map<string,int> ComTradeID2CloseVolume;

        //更新持仓明细
		m_PositionDetails.UpdatePositionDetail_RtnTrade(
            TradeInfo,
            InstrumentInfo,
            ModifiedPositionDetails,
            ComTradeID2CloseVolume);

        //更新成交记录中的平今量，平仓盈亏，重算手续费。
        m_Trades.UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(
            TradeInfo.FTID,
            InstrumentInfo,
            TradeInfo.CloseTodayVolume,
            TradeInfo.CloseProfitByDate,
            TradeInfo.CloseProfitByTrade,
            TradeInfo.TradeCommission);

        
        //更新组合持仓明细。
        map<PositionDetailKey,int> mapCloseDeltaVolume;
        if(TradeInfo.OffsetFlag!=THOST_FTDC_OF_Open)
            m_PositionDetails.GetCloseDeltaVolume(ModifiedPositionDetails,mapCloseDeltaVolume);
        m_PositionDetailCombs.Update_RtnTrade(
            TradeInfo,
            mapCloseDeltaVolume,
            ComTradeID2CloseVolume,
            ModifiedPositionDetailCombs);

        //针对有变化的持仓明细，将保证金数据设置到组合持仓明细中。此时未计算优惠
        m_PositionDetails.SetMarginOfPositionDetailComb(ModifiedPositionDetails);

        //针对有变化的持仓明细，计算组合持仓明细的保证金优惠。需要在更新持仓明细和更新组合持仓明细后进行。根据保证金优惠设置进行计算，更新相应持仓明细和组合持仓明细的保证金字段，(为保证不漏掉，两腿都算，有点重复)
        //会将保证金数据设置到持仓明细和组合持仓明细中去
        m_PositionDetails.CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(ModifiedPositionDetails,ModifiedPositionDetailCombs);

        //开仓成交时将组合合约ID设置到对应的持仓明细中。根据组合持仓明细的变化
        if(TradeInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived&&TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
            m_PositionDetails.SetCombInstrumentID_OpenTrade(ModifiedPositionDetailCombs);

        //更新持仓
        m_Positions.UpdatePosition_RtnTrade(TradeInfo,ModifiedPositionDetails,InstrumentInfo);

        //更新组合持仓
        m_PositionCombs.Update_RtnTrade(TradeInfo,ModifiedPositionDetailCombs);

        //更新资金
		m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
	}
}

//请求查询产品响应
void CPlatformDataMgr::OnRspQryProduct(const PlatformStru_ProductInfo& ProductInfo)
{
	if(ProductInfo.ProductID[0]==0) return;
    m_Instruments.SetProductInfo(string(ProductInfo.ProductID),ProductInfo);
}

//请求查询合约响应
void CPlatformDataMgr::OnRspQryInstrument(PlatformStru_InstrumentInfo& InstrumentInfo,bool bCTPSimulate,bool bLast)
{
	if(InstrumentInfo.InstrumentID[0]==0) return;

    m_Instruments.SetInstrumentInfo(InstrumentInfo.InstrumentID,InstrumentInfo,bCTPSimulate,bLast);
    m_Rates.SetNeedCheckCommissionRateAndMarginRate(true);
}

//请求查询投资者持仓响应。ErrorID==0表示数据正确；ErrorID==123456789表示查询无记录
void CPlatformDataMgr::OnRspQryInvestorPosition(PlatformStru_Position& PositionInfo,int ErrorID,bool bIsLast)
{
    string strInstrumentID(PositionInfo.InstrumentID);
    if(strInstrumentID.empty()) return;
    //取合约信息
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) return;

    //检查是否是组合合约持仓
    bool bComb = InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination;
    
    //处理查询结果，将结果送到数据集中
    if(!bComb) m_Positions.QryPositionReach(PositionInfo,ErrorID,bIsLast);
    else if(bIsLast)  m_Positions.QryPositionReach(PositionInfo,-1,bIsLast);

    if(bComb) m_PositionCombs.QryPositionCombReach(PositionInfo,ErrorID,bIsLast);
    else if(bIsLast)  m_PositionCombs.QryPositionCombReach(PositionInfo,-1,bIsLast);

    //更新资金信息
	if(bIsLast)
		m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
}

//请求查询投资者持仓明细响应
void CPlatformDataMgr::OnRspQryInvestorPositionDetail(PlatformStru_PositionDetail& PositionDetailInfo,int ErrorID,bool bIsLast)
{
    if(PositionDetailInfo.InstrumentID[0]==0) return;

    //处理查询结果，将结果送到数据集中
    m_PositionDetails.QryPositionDetailReach(PositionDetailInfo,ErrorID,bIsLast);
}

//请求查询投资者组合持仓明细响应
void CPlatformDataMgr::OnRspQryInvestorPositionCombDetail(PlatformStru_PositionDetailComb& PositionDetailCombInfo,int ErrorID,bool bIsLast)
{
    if(PositionDetailCombInfo.InstrumentID[0]==0) return;

	//添加组合持仓明细记录
	m_PositionDetailCombs.QryPositionDetailCombReach(PositionDetailCombInfo,ErrorID,bIsLast);

    if(bIsLast)
    {
        //取出所有组合持仓明细的key, 将组合合约设置到对应的持仓明细中
        vector<PositionDetailCombKey> combdetailkeys;
        m_PositionDetailCombs.GetAllKeys(combdetailkeys);
        m_PositionDetails.SetCombInstrumentIDs(combdetailkeys);
    }
}

//请求查询成交响应
void CPlatformDataMgr::OnRspQryTrade(PlatformStru_TradeInfo& TradeInfo,int ErrorID,bool bIsLast)
{
    if(TradeInfo.InstrumentID[0]==0) return;

    //处理查询结果，将结果送到数据集中
    m_Trades.QryTradeReach(TradeInfo,ErrorID,bIsLast);

    if(bIsLast)
	{
		//成交收全后，计算成交统计
		m_TradeTotals.QryTradeTotalReach(true);
		//更新资金数据
        m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
	}
}

//请求查询报单响应
void CPlatformDataMgr::OnRspQryOrder(PlatformStru_OrderInfo& OrderInfo,int ErrorID,bool bIsLast)
{
    if(OrderInfo.InstrumentID[0]==0) return;

    //处理查询结果，将结果送到数据集中
    m_Orders.QryOrderReach(OrderInfo,ErrorID,bIsLast);
    if(bIsLast)
    {
	    //更新资金数据
        m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
    }
}

//查询交易资金账户信息响应
void CPlatformDataMgr::OnRspQryTradingAccount(PlatformStru_TradingAccountInfo& TradingAccount)
{
    m_TradingAccount.SetTradingAccount(TradingAccount);
}

//请求查询合约保证金率响应
void CPlatformDataMgr::OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate& MarginRate,int ErrorID)
{
    string strInstrument(MarginRate.InstrumentID);
    if(strInstrument.empty()) return;

    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo)) return;

    if(ErrorID==0)
    {
        m_Rates.SetMarginRate(strInstrument,MarginRate);

        //更新报单冻结保证金数据
        if(m_Orders.UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(strInstrument,InstrumentInfo))
        {
            //更新资金中的冻结数据
            m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
        }

        //更新持仓明细保证金数据, 同时考虑组合保证金优惠、组合持仓明细的保证金
        m_PositionDetails.UpdatePositionDetailByMarginRate_RspQryRate(MarginRate,InstrumentInfo);

        //更新持仓中冻结资金数据
		m_Positions.UpdatePositionFreezeFieldsByOrders_RspQryRate(string(MarginRate.InstrumentID));

        //更新持仓中的保证金数据
        bool bUpdatePosition=m_Positions.UpdatePositionByMarginRate_RspQryRate(MarginRate);

        //更新组合持仓中的保证金数据
        bool bUpdatePositionComb=m_PositionCombs.UpdatePositionByMarginRate_RspQryRate(MarginRate);

        if(bUpdatePosition||bUpdatePositionComb)
        {
            //需要时更新资金中的保证金数据
            m_TradingAccount.DynamicUpdateTradingAccount_RspQryRate();
        }
    }
	else if(ErrorID==CustomErrorID)
	{
		//查无记录
		m_Rates.IncrementReqMarginRateError(strInstrument);
	}
}

//请求查询合约手续费率响应。包含期权的手续费率
void CPlatformDataMgr::OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate& CommissionRate,int ErrorID)
{
    string strInstrument(CommissionRate.InstrumentID);
    if(strInstrument.empty()) return;

    if(ErrorID==0)
    {
        //m_allInstrumentIDsByProductID此时已经有数据(查询合约信息必须在 查询手续费率之前)
        bool bHaveOrder=false;
        bool bHaveTrade=false;
        bool bHavePosition=false;

        //是合约ID
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo))
            return;

        m_Rates.SetCommissionRate(strInstrument,CommissionRate);

		//更新报单数据中冻结手续费
        if(m_Orders.UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(strInstrument,InstrumentInfo)&&!bHaveOrder)
            bHaveOrder=true;

		//更新成交明细数据中手续费
        if(m_Trades.UpdateTradeCommission_RspQryCommissionRate(strInstrument,InstrumentInfo,CommissionRate)&&!bHaveTrade)
            bHaveTrade=true;

		//更新成交统计
		m_TradeTotals.UpdateTradeTotal_RspQryCommissionRate(strInstrument);

		//更新持仓中的冻结域
		m_Positions.UpdatePositionFreezeFieldsByOrders_RspQryRate(strInstrument);

        //更新持仓中的手续费数据
        if(m_Positions.UpdatePositionByCommissionRate_RspQryRate(CommissionRate)&&!bHavePosition)
            bHavePosition=true;

        //更新资金中的冻结数据
        if(bHaveOrder)
            m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();

        //更新资金中的手续费数据
        if(bHaveTrade||bHavePosition)
            m_TradingAccount.DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate();
    }
    else if(ErrorID==CustomErrorID)
    {
		//查无记录
		m_Rates.IncrementReqCommissionRateError(strInstrument);
    }
}



//获取币种信息
//1. 首先根据银期签约关系，取银行对应的币种
//2. 额外加上USD、RMB(CNY)
vector<string> CPlatformDataMgr::GetCurrencys(const string& BankName,bool bAddUSD,bool bAddCNY) 
{ 
    vector<string> vecrlt;
    map<string,vector<string>>::const_iterator it=m_BankCurrency.find(BankName);
    if(it!=m_BankCurrency.end())
    {
        vecrlt=it->second;
    }
    if(bAddUSD&&!CTools_Ansi::IsValueInVector<string>(vecrlt,string("USD")))
        vecrlt.push_back(string("USD"));
    if(bAddCNY&&!CTools_Ansi::IsValueInVector<string>(vecrlt,string("CNY"))&&!CTools_Ansi::IsValueInVector<string>(vecrlt,string("RMB")))
    {
        vecrlt.push_back(string("CNY"));
        vecrlt.push_back(string("RMB"));
    }
    return vecrlt;
}
vector<string> CPlatformDataMgr::GetCurrencys(bool bAddUSD,bool bAddCNY) 
{ 
    vector<string> vecrlt;

    set<string> setCurrency;
    for(map<string,vector<string>>::const_iterator it=m_BankCurrency.begin();it!=m_BankCurrency.end();it++)
    {
        for(int i=0;i<(int)(it->second.size());i++)
            if(setCurrency.find(it->second[i])==setCurrency.end())
                setCurrency.insert(it->second[i]);
    }
    if(bAddUSD&&setCurrency.find("USD")==setCurrency.end())
        setCurrency.insert(string("USD"));
    if(bAddCNY&&setCurrency.find("CNY")==setCurrency.end()&&setCurrency.find("RMB")==setCurrency.end())
    {
        setCurrency.insert(string("CNY"));
        setCurrency.insert(string("RMB"));
    }

    CTools_Ansi::ConvertSet2Vector<string>(setCurrency,vecrlt);
    return vecrlt;
}

//设置/获取经纪公司交易参数
void CPlatformDataMgr::SetBrokerTradingParam(const PlatformStru_BrokerTradingParams& BrokerTradingParam)
{
    m_BrokerTradingParam=BrokerTradingParam;
    m_PositionDetails.SetPriceForRoyalty_Option(m_BrokerTradingParam.OptionRoyaltyPriceType==THOST_FTDC_ORPT_PreSettlementPrice?1:0);
    m_bHaveBrokerTradingParam=true;
}
const PlatformStru_BrokerTradingParams& CPlatformDataMgr::GetBrokerTradingParam(void)
{
    return m_BrokerTradingParam;
}
