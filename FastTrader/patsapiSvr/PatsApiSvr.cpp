#include "StdAfx.h"

#include "PatsApiSvr.h"
#include "PlatFormService.hpp"
#include "Module-Misc/globalDefines.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

const static string __UNKOWN__ = "未知";
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
{\
	m_pWriteLog->WriteLog_Fmt("CPatsApiSvr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
}\
	}while(0)

///启动平台
int CPatsApiSvr::Start()
{
    m_pWriteLog=new zqWriteLog();
	m_PlatFormService=new CPlatFormService();

	return 0;
}

///关闭平台
int CPatsApiSvr::Stop()
{
	int ret=Logout();
	SAFE_DELETE(m_PlatFormService);


	return ret;
}
int CPatsApiSvr::StopThread()
{
	m_PlatFormService->StopThreads();

	return 0;
}
///密码是否是当前密码
///1：是，其他：不是
int CPatsApiSvr::IsValidPW(const std::string& yourPW)
{
	return m_PlatFormService->IsValidPW(yourPW);
}
int CPatsApiSvr::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts)
{
	return m_PlatFormService->GetBrokerIDUserID(UserID,BrokerID,accounts);
}

///设置交易前置机地址
int CPatsApiSvr::SetTradeFrontAddr(const std::string& addr)
{
	return m_PlatFormService->SetTradeFrontAddr(addr);
}

int CPatsApiSvr::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
	return m_PlatFormService->SetTradeFrontAddrs(TradeAddr);
}

///设置行情前置机地址
int CPatsApiSvr::SetQuotFrontAddr(const std::string& addr)
{
	return m_PlatFormService->SetQuotFrontAddr(addr);
}

int CPatsApiSvr::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
	return m_PlatFormService->SetQuotFrontAddrs(QuotAddr);
}

///设置经纪公司参数信息
int CPatsApiSvr::SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)
{
	return m_PlatFormService->SetParticipantInfo(BrokerID, UserID, UserPW, OneTimePassword);
}


///登入
int CPatsApiSvr::Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPWD)
{
	if(m_isLogin)
		return 0;

	int ret=m_PlatFormService->LoginTrade(InvestorID, InvestorPW, InvestorNewPWD);
	if(ret<0)
		return ret;

	ret=m_PlatFormService->LoginQuot(InvestorID, InvestorPW);
	if(ret<0)
	{
		m_PlatFormService->LogoutTrade();
		return ret;
	}

	m_isLogin=true; 
	return ret;
}

///登出
int CPatsApiSvr::Logout()
{
	m_PlatFormService->LogoutTrade();
	m_PlatFormService->LogoutQuot();
	m_isLogin = false;
	return 0;
}

///定制业务数据
int CPatsApiSvr::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
	return m_PlatFormService->SubscribeBusinessData(BID, GID, callback, sendCurrentInfo);
}

///取消业务数据的定制
int CPatsApiSvr::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
	return m_PlatFormService->UnSubscribeBusinessData(BID, GID);
}

///获取底层服务的当前状态
ServiceStatus CPatsApiSvr::GetCurrentStatus(int* pQuotStatus,int* pTradeStatus)
{
	if(!m_isLogin)
		return SS_NOLOGIN;

	CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
	if(pQuotStatus) *pQuotStatus=RefPlatformParam.QuotStatus;
	if(pTradeStatus) *pTradeStatus=RefPlatformParam.TradeStatus;
	if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Connecting ||
		RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Logining   ||
		RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirming /*  ||
																							   RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Connecting ||
																							   RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Logining*/)
																							   return SS_LOGINING;
	else if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_LoginFailure /*||*/
		/*RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirmFailure ||*/
		/*RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_LoginFailure*/)
		return SS_LOGINFAILED;
	else if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Connected /*&&
																			   RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Connected*/)
																			   return SS_LOGINED;
	else
		return SS_LOSTCONN;

}
///获取合约列表
int CPatsApiSvr::GetInstrumentList(std::vector<GroupInfo> &outData)
{
	int ret=m_PlatFormService->GetInstrumentList(outData);
	for(size_t i=0;i<outData.size();i++)
	{
		outData[i].GroupName=ExchangeID2String(outData[i].GroupName);
	}
	return ret;
}
///获取合约列表
int CPatsApiSvr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
	return m_PlatFormService->GetInstrumentList(outData,ExchangeID);
}
//添加主力合约列表
int CPatsApiSvr::AddMainInstrumentList(std::string instrument)
{
	return m_PlatFormService->AddMainInstrumentList(instrument);
}
///获取主力合约列表
int CPatsApiSvr::GetMainInstrumentList(std::set<std::string> &outData)
{
	return m_PlatFormService->GetMainInstrumentList(outData);
}
///设置合约容差列表
void CPatsApiSvr::SetInstrumentVarietyMap(map<string, string>& inData)
{
	m_PlatFormService->SetInstrumentVarietyMap(inData);
}

// 获取合约容差列表
int CPatsApiSvr::GetInstrumentVarietyMap(map<string, string>& outData)
{
	return m_PlatFormService->GetInstrumentVarietyMap(outData);
}

///获取指定合约信息
int CPatsApiSvr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
	return m_PlatFormService->GetInstrumentInfo(InstrumentID, outData);
}
//设置合约订阅状态
void CPatsApiSvr::SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper)
{
	m_PlatFormService->SetSubscribeStatus(InstrumentID,status,oper);
}
///设置指定合约信息
void CPatsApiSvr::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
	m_PlatFormService->SetInstrumentInfo(InstrumentID, outData);
}
///获取合约的产品类型，失败返回-1
char CPatsApiSvr::GetProductClassType(const std::string& InstrumentID)
{
	return m_PlatFormService->GetProductClassType(InstrumentID);
}

///获取合约品种代码
int CPatsApiSvr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
	return m_PlatFormService->GetProductID(outData,ExchangeID);
}

///获取合约手续费率
int CPatsApiSvr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
	return m_PlatFormService->GetCommissionRate(InstrumentID, outData);
}

///获取合约手续费率，如果没有，则触发查询
int CPatsApiSvr::GetCommissionRate_TriggerReq(const std::string& InstrumentID)
{
    PlatformStru_InstrumentCommissionRate RateData;
	int ret;

	ret=m_PlatFormService->GetCommissionRate(InstrumentID, RateData);

	if(ret==0) return 0;
	else 
	{                                                       //如果当前没有该费率，则放一条到查询队列中
		CThostFtdcQryInstrumentCommissionRateField ReqData;
		memset(&ReqData,0,sizeof(ReqData));
		strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
		strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
		strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);;

		m_PlatFormService->ReqQryInstrumentCommissionRate(&ReqData,0);
		return ret;
	}

}

///获取合约保证金率
int CPatsApiSvr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
	return m_PlatFormService->GetMarginRate(InstrumentID, outData);
}

///获取合约保证金率，如果没有，则触发查询
int CPatsApiSvr::GetMarginRate_TriggerReq(const std::string& InstrumentID)
{
	PlatformStru_InstrumentMarginRate RateData;
	int ret;

	if(InstrumentID.empty())
		return -1;
	ret=m_PlatFormService->GetMarginRate(InstrumentID, RateData);

	if(ret==0) return 0;
	else 
	{                                                       //如果当前没有该费率，则放一条到查询队列中
		CThostFtdcQryInstrumentMarginRateField ReqData;
		memset(&ReqData,0,sizeof(ReqData));
		strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
		strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
		strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);
		ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;


		m_PlatFormService->ReqQryInstrumentMarginRate(&ReqData,0);
		return ret;
	}

}

///设置合约手续费率
int CPatsApiSvr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
	return m_PlatFormService->SetCommissionRate(InstrumentID, outData);
}

///设置合约保证金率
int CPatsApiSvr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
	return m_PlatFormService->SetMarginRate(InstrumentID, outData);
}

///获取合约乘数，成功返回合约乘数，失败返回-1
int CPatsApiSvr::GetInstrumentMultiple(const std::string& InstrumentID)
{
	return m_PlatFormService->GetInstrumentMultiple(InstrumentID);
}
///获取指定品种的合约列表,返回合约个数
int CPatsApiSvr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_PlatFormService->GetInstrumentListByProductID(ProductID,outData);
}
///获取指定合约行情
int CPatsApiSvr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
	return m_PlatFormService->GetQuotInfo(InstrumentID, outData);
}

///获取指定合约旧行情
int CPatsApiSvr::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
	return m_PlatFormService->GetOldQuotInfo(InstrumentID, outData);
}

///获取指定合约的现价，失败返回0.0
double CPatsApiSvr::GetCurPrice(const std::string& InstrumentID)
{
	return m_PlatFormService->GetCurPrice(InstrumentID);
}


int CPatsApiSvr::GetPositions2(const std::string& strInstrument,
								 std::set<long>& setFTID,
								 std::vector<PlatformStru_Position>& outData,
								 long& lastseq)
{
	return m_PlatFormService->GetPositions2(strInstrument,setFTID,outData,lastseq);
}

int CPatsApiSvr::GetPositions3(const std::string& strInstrument,
								 std::vector<PlatformStru_Position>& outData,
								 long& lastseq)
{
	return m_PlatFormService->GetPositions3(strInstrument,outData,lastseq);
}

int CPatsApiSvr::GetPositionDetails3(const std::string& strInstrument,
									   std::set<long>& setFTID,
									   std::vector<PlatformStru_PositionDetail>& vecValue,
									   long& lastseq)
{
	return m_PlatFormService->GetPositionDetails3(strInstrument,setFTID,vecValue,lastseq);
}

int CPatsApiSvr::GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData)
{
	return m_PlatFormService->GetPositionCombs2(strInstrument,outData);
}
int CPatsApiSvr::GetPositions(std::vector<PlatformStru_Position>& outData)
{
	return m_PlatFormService->GetPositions(outData);
}

int CPatsApiSvr::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
	return m_PlatFormService->GetPositionDetails(outData,lastseq);
}

int CPatsApiSvr::GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)
{
	return m_PlatFormService->GetPositionCombs(outData);
}


bool CPatsApiSvr::HavePositionDetail(const std::string& strInstrumentID)
{
	return m_PlatFormService->HavePositionDetail(strInstrumentID);
}

//获取持仓记录键值列表,返回持仓记录的数量
int CPatsApiSvr::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
	return m_PlatFormService->GetPositionKeySet(PositionKeySet);
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CPatsApiSvr::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
	return m_PlatFormService->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}



int  CPatsApiSvr::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
	return m_PlatFormService->GetWaitOrderVolume(strAccount, strInstrumentID, Direction, CloseType,HedgeFlag);
}

//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
int CPatsApiSvr::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
	return m_PlatFormService->GetCloseOrderVolume(strAccount, strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}


///获取交易所时间
string CPatsApiSvr::GetTradingDay(const string& ExchangeID)
{
    return m_PlatFormService->GetTradingDay(ExchangeID);
}

bool CPatsApiSvr::IsTodayPosition( const char* pOpenData )
{
	return m_PlatFormService->IsTodayPosition(pOpenData);
}

///获取交易所时间
unsigned long CPatsApiSvr::GetExchangeTime(const string& ExchangeID,string& time)
{
	return m_PlatFormService->GetExchangeTime(ExchangeID,time);
}
///获取全部交易所
int CPatsApiSvr::GetExchangeIDs(vector<string>& outData)
{
	return m_PlatFormService->GetExchangeIDs(outData);
}
int CPatsApiSvr::GetTradingAccountAvailable(double& fAvailable)
{
	return m_PlatFormService->GetTradingAccountAvailable(fAvailable);
}
int CPatsApiSvr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
	return m_PlatFormService->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CPatsApiSvr::GetTradingAccountID(char* AccountID)
{
	return m_PlatFormService->GetTradingAccountID(AccountID);
}
///获取交易资金账户信息
int CPatsApiSvr::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
	return m_PlatFormService->GetTradingAccount(strAccount, outData);
}
///获取账户资金文本信息
int CPatsApiSvr::GetAccountText(std::string& outData,int language)
{
	return m_PlatFormService->GetAccountText(outData,language);
}
///设置交易资金账户信息
void CPatsApiSvr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
	m_PlatFormService->SetTradingAccount(inData);
}

///请求查询投资者结算结果响应
int CPatsApiSvr::GetLastSettlementInfo(std::string& outData)
{
	return m_PlatFormService->GetLastSettlementInfo(outData);
}

///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
void CPatsApiSvr::SetCurrentPositionContent(int PositionContentMode)
{
    return m_PlatFormService->SetCurrentPositionContent(PositionContentMode);
}

///获取全部报单
int CPatsApiSvr::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	return m_PlatFormService->GetAllOrders(outData);
}
///获取全部报单
int CPatsApiSvr::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetAllOrders2(strInstrument,outData);
}

int CPatsApiSvr::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTriggerOrders(outData);
}

///获取合约相关的已触发的报单
int CPatsApiSvr::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTriggerOrders2(strInstrument,outData);
}

///获取指定已触发的报单
bool CPatsApiSvr::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetTriggerOrder(orderkey,outData);
}

///获取已成交报单
int CPatsApiSvr::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTradedOrders(outData);
}

///获取已成交报单
int CPatsApiSvr::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTradedOrders2(strInstrument,outData);
}

///获取指定的已成交报单
bool CPatsApiSvr::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetTradedOrder(orderkey,outData);
}

///获取已撤单和错误报单
int CPatsApiSvr::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetCanceledOrders(outData);
}

///获取已撤单和错误报单
int CPatsApiSvr::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetCanceledOrders2(strInstrument,outData);
}

///获取指定的已撤单和错误报单
bool CPatsApiSvr::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetCanceledOrder(orderkey,outData);
}

///获取未成交报单
int CPatsApiSvr::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders(outData);
}

///获取未成交报单
int CPatsApiSvr::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders2(strInstrument,outData);
}

///获取指定的未成交报单
bool CPatsApiSvr::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetWaitOrder(orderkey,outData);
}

///获取指定报单
bool CPatsApiSvr::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetOrder(inOrderKey,outData);
}


///获取全部成交单
int CPatsApiSvr::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
	return m_PlatFormService->GetAllTrades(outData);
}
///获取指定成交信息
bool CPatsApiSvr::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
	return m_PlatFormService->GetTradeInfo(tradekey, outData);
}
///获取指定成交信息
int CPatsApiSvr::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_PlatFormService->GetTradesOfInstrument(strInstrument,outData);
}


///获取全部成交统计记录
int CPatsApiSvr::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_PlatFormService->GetAllTradeTotals(outData);
}
///获取指定合约的成交统计记录成功返回0，失败返回-1
int CPatsApiSvr::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
	return m_PlatFormService->GetTradeTotalOfInstrument(strInstrument,outData);
}
///获取全部成交统计数据
int CPatsApiSvr::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
	return m_PlatFormService->GetAllTradeTotalDatas(outData);
}
///在成交统计查找参数rawData
int CPatsApiSvr::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
	return m_PlatFormService->FindIndexFromAllTradeTotalDatas(rawData);
}

bool CPatsApiSvr::IsSettlementInfoConfirmed()
{
	return m_PlatFormService->IsSettlementInfoConfirmed();
}
//获取客户签约银行信息
int CPatsApiSvr::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
    return m_PlatFormService->GetContractBank(outData);
}

//查询队列是否为空
bool CPatsApiSvr::IsQryQueueEmpty(void)
{


	return false;
}


void CPatsApiSvr::SetModuleDataQry(bool bQry)
{
    m_PlatFormService->SetModuleDataQry(bQry);
}
bool CPatsApiSvr::GetModuleDataQry()
{
    return m_PlatFormService->GetModuleDataQry();
}
void CPatsApiSvr::ReConnetServer()
{
	Logout();
	CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
	Login(RefPlatformParam.InvestorID,RefPlatformParam.InvestorPW);
}
CPatsApiSvr::~CPatsApiSvr()
{



	LOG_INFO("CPatsApiSvr::~CPatsApiSvr(析构CPatsApiSvr对象)");
	SAFE_DELETE(m_pWriteLog);
}

CPatsApiSvr::CPatsApiSvr()
: m_PlatFormService( NULL )
, m_isLogin( false )
, m_qrystep(0)
{

}
void CPatsApiSvr::GetErrorField(int& ErrorID)
{
	m_PlatFormService->GetErrorField(ErrorID);
}
void CPatsApiSvr::GetErrorField(string& ErrMsg)
{
	m_PlatFormService->GetErrorField(ErrMsg);
}
void CPatsApiSvr::SetReqID(int nOrderReqID) {
	return m_PlatFormService->SetReqID(nOrderReqID);
};

long CPatsApiSvr::GetReqID() {
	return m_PlatFormService->GetReqID();
};

long CPatsApiSvr::GetNextReqID()
{
	return m_PlatFormService->GetNextReqID();
}
int  CPatsApiSvr::GetAccountList(std::vector<string>& AcctList)
{
	return m_PlatFormService->GetAccountList(AcctList);
}

void CPatsApiSvr::FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec )
{
	return m_PlatFormService->FillQuotData( rawQuot, fieldID, vec );
}

///交易所ID
const string CPatsApiSvr::ExchangeID2String(const string& ExchangeID)
{
	//const string STR_CME="芝加哥商业交易所";
	//const string STR_CME_CBT="芝加哥交易所";
	//const string STR_LIFFE="伦敦国际金融期权期货交易所";

	//if(ExchangeID=="CME"||ExchangeID=="CME_CBT"||ExchangeID=="CME2")
	//	return ExchangeID;
	//else
	//	return __UNKOWN__;
	return ExchangeID;
}
const string CPatsApiSvr::ExchangeName2ID( const string& name)
{
	std::vector<std::string> outData;
	m_PlatFormService->GetExchangeIDs(outData);
	std::vector<std::string>::iterator it=outData.begin();
	for(it;it!=outData.end();++it)
	{
		if(name==*it)
			return name;
	}
	return "";
}
SERVER_PARAM_CFG&  CPatsApiSvr::GetServerParamCfg()
{
	return m_PlatFormService->GetServerParamCfg();
}
bool CPatsApiSvr::IsModuleValid( int nModuleId )
{
	switch( nModuleId )
	{
	case GID_ACCOUNT:
	case GID_QUOT_TABLE:
	case GID_INSTRUMENT:
	case GID_OPEN_ORDER:
	case GID_ALL_ORDER:
	case GID_ORDER_INSERT_PANEL:
	case GID_POSITION:
	//case GID_POSITION_DETAIL:
	case GID_POSITIONALL:
	case GID_THRADE_RECORD:
	//case GID_QUERY_ORDER_BATCH:
	//case GID_QUERY_ORDER_CONDITION:
	//case GID_QUERY_ORDER_POSITION:
	//case GID_QUERY_ORDER_PARKED:
	//case GID_QUERYLOCAL_ORDER:
		return true;
	default:
		return false;
	}
}

bool CPatsApiSvr::IsFieldValid( int nModuleId, int nFieldId )
{
	switch( nModuleId )
	{
	case GID_QUOT_TABLE:
		{
			if ( nFieldId >= QUOT_FIELD_ID_MAX ||
				nFieldId == QUOT_FIELD_ID_OpenInterest ||
				nFieldId == QUOT_FIELD_ID_PreOpenInterest ||
				nFieldId == QUOT_FIELD_ID_Turnover||
				nFieldId == QUOT_FIELD_ID_LastVolume)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_INSTRUMENT:
		{
			if ( nFieldId == INSTRUMENT_FIELD_ID_OPENRATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSERATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSETODAYRATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY ||
				nFieldId == INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME ||
				nFieldId == INSTRUMENT_FIELD_ID_TICKSIZE ||
				nFieldId == INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME ||
				nFieldId == INSTRUMENT_FIELD_ID_LONGMARGINRATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_SHORTMARGINRATIO )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_THRADE_RECORD:
		{
			if ( nFieldId == TRADE_RECORD_FIELD_ID_OPENCLOSE ||
				nFieldId == TRADE_RECORD_FIELD_ID_INSURE||
				nFieldId == TRADE_RECORD_FIELD_ID_DIRECTION)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_THRADE_STATISTICS:
		{
			if ( nFieldId == TRADE_RECORD_FIELD_ID_OPENCLOSE )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_POSITION:
		{
			if ( nFieldId == POSITION_FILED_ID_INSTRUMENTID ||
				nFieldId == POSITION_FILED_ID_TOTALPOSITION ||
				nFieldId == POSITION_FIELD_ID_LASTPRICE ||
				nFieldId == POSITION_FIELD_ID_EXCHANGE ||
				nFieldId == POSITION_FIELD_ID_CURRENCY ||
				nFieldId == POSITION_FIELD_ID_OPENPROFIT ||
				nFieldId == POSITION_FIELD_ID_OPENAVGPRICE ||
				nFieldId == POSITION_FIELD_ID_BUYVOLUMN ||
				nFieldId == POSITION_FIELD_ID_SELVOLUMN ||
				nFieldId == POSITION_FIELD_ID_TOTALPROFIT||
				nFieldId == POSITION_FIELD_ID_MARGINPERLOT||
				nFieldId == POSITION_FIELD_ID_PLBURNRATE||
				nFieldId == POSITION_FIELD_ID_OPENPOSITIONEXPOSURE||
				nFieldId == POSITION_FIELD_ID_BUYPOWERREMAIN||
				nFieldId == POSITION_FIELD_ID_BUYPOWERUSED||
				nFieldId == POSITION_FIELD_ID_TOTALMARGINPAID||
				nFieldId == POSITION_FIELD_ID_ACCOUNT ||
				nFieldId == POSITION_FIELD_ID_CLOSEPROFIT)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_POSITIONDETAIL:
		{
			if ( nFieldId == POSITION_DETAIL_FIELD_ID_INSTRUMENT )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_ACCOUNT:
		{
			if ( nFieldId ==FUNDACCOUNT_STATICPROFIT_COL_ID||
				nFieldId ==FUNDACCOUNT_POSITIONPROFIT_COL_ID||
				nFieldId ==FUNDACCOUNT_CLOSEPROFIT_COL_ID||
				nFieldId ==FUNDACCOUNT_DYNAMICPROFIT_COL_ID||
				nFieldId == FUNDACCOUNT_MARGINPAID_COL_ID ||
				nFieldId == FUNDACCOUNT_EXPOSURE_COL_ID ||
				nFieldId == FUNDACCOUNT_BPUSED_COL_ID ||
				nFieldId == FUNDACCOUNT_BURNRATE_COL_ID||
				nFieldId == FUNDACCOUNT_ACCOUNTID_COL_ID||
				nFieldId == FUNDACCOUNT_BPREMAINING_COL_ID||
                nFieldId == FUNDACCOUNT_TOTALPROFIT_COL_ID)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_ALL_ORDER:
		{
			if ( nFieldId == ALLORDER_UserForceClose ||
				nFieldId == ALLORDER_ForceCloseReason ||
				nFieldId == ALLORDER_UserProductInfo ||
				nFieldId == ALLORDER_CombOffsetFlag ||
				nFieldId == ALLORDER_d_freezeMargin ||
				nFieldId == ALLORDER_d_troubleMoney ||
				nFieldId == ALLORDER_OrderSubmitStatus ||
				nFieldId == ALLORDER_OrderType ||
				nFieldId == ALLORDER_OrderStatus ||
				nFieldId == ALLORDER_TimeCondition ||
				nFieldId == ALLORDER_CombHedgeFlag)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_OPEN_ORDER:
		{
			if ( nFieldId == OPENORDER_CombHedgeFlag ||
				nFieldId == OPENORDER_ForceCloseReason ||
				nFieldId == OPENORDER_UserForceClose ||
				nFieldId == OPENORDER_CombOffsetFlag ||
				nFieldId == OPENORDER_d_freezeMargin ||
				nFieldId == OPENORDER_d_troubleMoney ||
				nFieldId == OPENORDER_OrderSubmitStatus ||
				nFieldId == OPENORDER_OrderType ||
				nFieldId == OPENORDER_OrderStatus ||
				nFieldId == OPENORDER_TimeCondition ||
				nFieldId == OPENORDER_UpdateTime ||
				nFieldId == OPENORDER_d_avgPrice ||
				nFieldId == OPENORDER_UserProductInfo)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ORDER_INSERT_PANEL:
		{
			if ( nFieldId == ORDERNEWINPUT_INSTRUMENTID_ROW_ID ||
					nFieldId == ORDERNEWINPUT_BUYSELL_ROW_ID ||
					//nFieldId == ORDERNEWINPUT_OPENCLOSE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_PRICE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_VOLUME_ROW_ID ||
					nFieldId == ORDERNEWINPUT_TRADEACCOUNT_ROW_ID || 
					nFieldId == ORDERNEWINPUT_ORDERTYPE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_LIMITPRICE_ROW_ID) {
				return true;
			}
			else {
				return false;
			}
		}
		break;
	default:
		return true;
	}
}

void CPatsApiSvr::GetContractShortNames(string& strShortNames)
{
	static string strContractShortName;
	if(strContractShortName.size()>0) {
		strShortNames = strContractShortName;
	}
	else {
		strContractShortName.clear();
		std::map<string, set<string>> outData;
		std::map<string, set<string>>::iterator it;
		int nCount = GetProductID(outData);   
		if(nCount == 0) return;
		for(it = outData.begin(); it != outData.end();) {
			strContractShortName += (it->first + ",");
			strContractShortName += (it->first + ",");
			strContractShortName += it->first;
			it++;
			if(it != outData.end())
				strContractShortName += ",";
		}
		for(int i=0; i<(int)strContractShortName.size(); i++) {
			if(strContractShortName[i]==' ')
				strContractShortName[i]='-';
		}
		strShortNames = strContractShortName;
	}
}
string CPatsApiSvr::GetVersionIconString()
{
	return "ULD_VERSTR_PATS";
}
//是否支持插件
bool CPatsApiSvr::SupportPlugins()
{
	std::string s=m_PlatFormService->GetServerParamCfg().VersionStr;
	return s.find("plugins")!=string::npos;
}
//是否是多账号版本
bool CPatsApiSvr::IsMultipleAccounts()
{
	std::string s=m_PlatFormService->GetServerParamCfg().VersionStr;
	return s.find("multi")!=string::npos;
}
int CPatsApiSvr::SetProxyConnectStr(const std::string& str)
{
	return 0;
}
bool CPatsApiSvr::HaveOrderType(UIOrderType type)
{
	switch(type) {
	case UIOT_PANE_STANDARD:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_STANDARD();
	case UIOT_PANE_BATCH:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_CONDITION();
	case UIOT_PANE_CONDITION:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_BATCH();
	case UIOT_PANE_FAKFOK:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_FAKFOK();
	case UIOT_PANE_PROFIT:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_PROFIT();
	case UIOT_PANE_STOPLIMIT:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_STOPLIMIT();
	case UIOT_CMD_REPLACEORDER:
		return GlobalConfigManager::IsOrderType_UIOT_CMD_REPLACEORDER();
	case UIOT_CMD_SIMULATEMARKET:
		return GlobalConfigManager::IsOrderType_UIOT_CMD_SIMULATEMARKET();
	case UIOT_CMD_PARKED:
		return GlobalConfigManager::IsOrderType_UIOT_CMD_PARKED();
	case UIOT_ITEM_HEDGE:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_HEDGE();
	case UIOT_ITEM_OPENCLOSE:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_OPENCLOSE();
	case UIOT_ITEM_ACCOUNT:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_ACCOUNT();
	case UIOT_ITEM_CONDITION_LOCAL:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_CONDITION_LOCAL();
	case UIOT_ITEM_CONDITION_SERVER:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_CONDITION_SERVER();
	case UIOT_ITEM_PARKED_LOCAL:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_PARKED_LOCAL();
	case UIOT_ITEM_PARKED_SERVER:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_PARKED_SERVER();
	};
	return false;
}
//请求账号信息
int CPatsApiSvr::GetTrader(std::vector<PlatformStru_Trader> &outData) 
{
	return m_PlatFormService->GetTrader(outData);
}
///请求查询交易所信息
int CPatsApiSvr::GetExchange(std::vector<PlatformStru_Exchange> &outData) 
{
	return m_PlatFormService->GetExchange(outData);
}
///请求查询合约信息
int CPatsApiSvr::GetContract(std::vector<PlatformStru_Contract> &outData) 
{
	return m_PlatFormService->GetContract(outData);
}
///请求商品信息
int CPatsApiSvr::GetCommodity(std::vector<PlatformStru_Commodity> &outData) 
{
	return m_PlatFormService->GetCommodity(outData);
}
///获取报单类型信息
int CPatsApiSvr::GetOrderType(std::vector<PlatformStru_OrderType> &outData) 
{
	return m_PlatFormService->GetOrderType(outData);
}
bool CPatsApiSvr::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	return m_PlatFormService->GetTotalPosition(account,posi);
}
bool CPatsApiSvr::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	return m_PlatFormService->GetOpenPosition(InstrumentID,account,posi);
}
bool CPatsApiSvr::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	return m_PlatFormService->GetContractPosition(InstrumentID,account,posi);
}
std::string CPatsApiSvr::GetLogonReason()
{
	return m_PlatFormService->GetLogonReason();
}
void CPatsApiSvr::GetLinkStatus(int* pHost,int* pPrice)
{
	m_PlatFormService->GetLinkStatus(pHost,pPrice);
}
int CPatsApiSvr::GetReports( map<DWORD, Report>& mapReport )
{
	return m_PlatFormService->GetReports(mapReport);
}
double CPatsApiSvr::GetExchangeRate(char* currency)
{
	return m_PlatFormService->GetExchangeRate(currency);
}
void CPatsApiSvr::NotifyStartStopInitQry(bool bStart) 
{ 

}
int CPatsApiSvr::GetAllOrderFTIDList(std::vector<long>& vec)
{
	return m_PlatFormService->GetAllOrderFTIDList(vec);
}

int CPatsApiSvr::GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)
{
	return m_PlatFormService->GetAllOrderInfo(lFTID, outData);
}

int CPatsApiSvr::GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)
{
	return m_PlatFormService->GetAllOrderInfo(key, outData);
}

int CPatsApiSvr::GetPositionFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetPositionFTIDList(vec);
}

int CPatsApiSvr::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionInfo(lFTID, outData);
}

int CPatsApiSvr::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionInfo(key, outData);
}

int CPatsApiSvr::GetPositionCombFTIDList(std::vector<long>& vec)
{
	return m_PlatFormService->GetPositionCombFTIDList(vec);
}

int CPatsApiSvr::GetPositionCombInfo(long lFTID, PlatformStru_PositionCombDetail& outData)
{
	return m_PlatFormService->GetPositionCombInfo(lFTID, outData);
}
int CPatsApiSvr::GetGroupMaxContractNum()
{
	return GlobalConfigManager::MaxSubscribeCount;
}
bool CPatsApiSvr::ContractCountIsFit()
{
	return false;
}