#include "FTTraderApi.h"
#include "SvrTradeFront.h"

#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","SvrTradefrontModule", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);



extern set<string>			  g_setTradeFrontAdds;



CFTTraderApi::CFTTraderApi(void)
:m_pTradeApi(NULL)
,m_TradeStatus(CTPCONNSTATUS_Disconnected)
{
}

CFTTraderApi::~CFTTraderApi(void)
{	
}

void CFTTraderApi::Init(int nSocket)
{
	m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	m_TradeSpi.SetSocket(nSocket);
	m_TradeSpi.SetCTPTraderApi(m_pTradeApi);
	m_pTradeApi->RegisterSpi(&m_TradeSpi);

	m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);

	string addrstr;
	for(std::set<std::string>::iterator it=g_setTradeFrontAdds.begin();it!=g_setTradeFrontAdds.end();++it)
	{		
		addrstr = "tcp://" + *it;//addrstr=string("tcp://180.168.102.193:41205");
		m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));// 连接前置机

	}
	m_pTradeApi->Init();
}

void CFTTraderApi::Init(LPVOID pLinkEntity)
{
	m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	m_TradeSpi.SetLinkEntity(pLinkEntity);
	m_TradeSpi.SetCTPTraderApi(m_pTradeApi);
	m_pTradeApi->RegisterSpi(&m_TradeSpi);

	m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);

	string addrstr;
	for(std::set<std::string>::iterator it=g_setTradeFrontAdds.begin();it!=g_setTradeFrontAdds.end();++it)
	{		
		addrstr = "tcp://" + *it;//addrstr=string("tcp://180.168.102.193:41205");
		m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));// 连接前置机

	}
	m_pTradeApi->Init();
}

void CFTTraderApi::Release()
{
	m_pTradeApi->RegisterSpi(NULL);
	m_pTradeApi->Release();
	m_pTradeApi=NULL;
}

//仅限于本模块使用
#define CHECK_TRADE_STATUS() \
	if(!m_pTradeApi || m_TradeStatus!=CTPCONNSTATUS_Connected)\
{\
	LOG_INFO("交易API还未准备就绪, 可能登入未成功或正在重连");\
	return ret;\
}
/***********************************************************
*   return  :   0:成功
*               -1:     网络连接失败
*               -2:     未处理请求超过许可数
*               -3:     每秒发送请求数超过许可数
*               -999:   其它原因失败
***********************************************************/
///用户登录请求
int CFTTraderApi::ReqUserLogin(CThostFtdcReqUserLoginField *pData, int nRequestID)
{	
	int ret = 0;	
	
	m_TradeSpi.LoginCtp(pData, nRequestID);
	ret = m_pTradeApi->ReqUserLogin(pData, nRequestID);
	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqUserLogin(用户登录请求):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t TradingDay=[%s],\t BrokerID=[%s],\t UserID=[%s],\t Password=[******],\t UserProductInfo=[%s],\n"
		"\t\t\t InterfaceProductInfo=[%s],\t ProtocolInfo=[%s]",
		ret, nRequestID,
		pData->TradingDay,
		pData->BrokerID,
		pData->UserID,
		/*pData->Password,*/
		pData->UserProductInfo,
		pData->InterfaceProductInfo,
		pData->ProtocolInfo);

	return ret;
}

///登出请求
int CFTTraderApi::ReqUserLogout(CThostFtdcUserLogoutField *pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqUserLogout(pData, nRequestID);	
	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqUserLogout(登出请求):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],UserID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->UserID);

	return ret;
}

///用户口令更新请求
int CFTTraderApi::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcUserPasswordUpdateField req={0};
	strncpy(req.BrokerID, pData->BrokerID, sizeof(req.BrokerID)-1);
	strncpy(req.UserID, pData->UserID, sizeof(req.UserID)-1);
	strncpy(req.NewPassword, pData->NewPassword, sizeof(req.NewPassword)-1);
	strncpy(req.OldPassword, pData->OldPassword, sizeof(req.OldPassword)-1);

	ret = m_pTradeApi->ReqUserPasswordUpdate(&req, nRequestID);
	if(ret==0)
		_GetForwarding().SendPackageForApi(&req, sizeof(req), nRequestID);

	LOG_INFO("ReqUserPasswordUpdate(用户口令更新请求):ret=[%d],nRequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],UserID=[%s],OldPassword=[******],NewPassword=[******]",
			ret, nRequestID,
			req.BrokerID,
			req.UserID);

	return ret;
}

///资金账户口令更新请求
int CFTTraderApi::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcTradingAccountPasswordUpdateField req={0};
	strncpy(req.BrokerID, pData->BrokerID, sizeof(req.BrokerID)-1); 
	strncpy(req.AccountID, pData->AccountID, sizeof(req.AccountID)-1);
	strncpy(req.OldPassword, pData->OldPassword, sizeof(req.OldPassword)-1);
	strncpy(req.NewPassword, pData->NewPassword, sizeof(req.NewPassword)-1);
	strncpy(req.CurrencyID, pData->CurrencyID, sizeof(req.CurrencyID)-1);

	ret = m_pTradeApi->ReqTradingAccountPasswordUpdate(&req, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&req, sizeof(req), nRequestID);

	LOG_INFO("ReqTradingAccountPasswordUpdate(资金账户口令更新请求):ret=[%d],nRequestID=[%d],\n"
				"\t\t\t BrokerID=[%s],AccountID=[%s],OldPassword=[******],NewPassword=[******],CurrencyID=[%s]",
				ret, nRequestID,
				req.BrokerID,
				req.AccountID,
				req.CurrencyID);

	return ret;
}

///报单录入请求
int CFTTraderApi::ReqOrderInsert(PlatformStru_InputOrder* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcInputOrderField tField = {0};
	strcpy(tField.BrokerID, pData->BrokerID);					///经纪公司代码
	strcpy(tField.InvestorID, pData->InvestorID);				///投资者代码
	strcpy(tField.InstrumentID, pData->InstrumentID);			///合约代码
	strcpy(tField.OrderRef, pData->OrderRef);					///报单引用
	strcpy(tField.UserID, pData->UserID);						///用户代码
	tField.OrderPriceType = pData->OrderPriceType;				///报单价格条件
	tField.Direction = pData->Direction;						///买卖方向
	strcpy(tField.CombOffsetFlag, pData->CombOffsetFlag);		///组合开平标志
	strcpy(tField.CombHedgeFlag, pData->CombHedgeFlag);			///组合投机套保标志
	tField.LimitPrice = pData->LimitPrice;						///价格
	tField.VolumeTotalOriginal = pData->VolumeTotalOriginal;	///数量
	tField.TimeCondition = pData->TimeCondition;				///有效期类型
	strcpy(tField.GTDDate, pData->GTDDate);						///GTD日期
	tField.VolumeCondition = pData->VolumeCondition;			///成交量类型
	tField.MinVolume = pData->MinVolume;						///最小成交量
	tField.ContingentCondition = pData->ContingentCondition;	///触发条件
	tField.StopPrice = pData->StopPrice;						///止损价
	tField.ForceCloseReason = pData->ForceCloseReason;			///强平原因
	tField.IsAutoSuspend = pData->IsAutoSuspend;				///自动挂起标志
	strcpy(tField.BusinessUnit, pData->BusinessUnit);			///业务单元
	tField.RequestID = pData->RequestID;						///请求编号
	tField.UserForceClose = pData->UserForceClose;				///用户强评标志

	ret = m_pTradeApi->ReqOrderInsert(&tField, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&tField, sizeof(tField), nRequestID);

	LOG_INFO("ReqOrderInsert(报单录入请求):ret=[%d],nRequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
			ret, nRequestID,
			pData->BrokerID, pData->InvestorID, pData->InstrumentID, pData->OrderRef,pData->UserID,
			pData->OrderPriceType, pData->Direction, pData->CombOffsetFlag, pData->CombHedgeFlag,pData->LimitPrice,
			pData->VolumeTotalOriginal, pData->TimeCondition, pData->GTDDate, pData->VolumeCondition,pData->MinVolume,
			pData->ContingentCondition, pData->StopPrice, pData->ForceCloseReason, pData->IsAutoSuspend,pData->BusinessUnit,
			pData->RequestID, pData->UserForceClose);

	return ret;

}

///预埋单录入请求
int CFTTraderApi::ReqParkedOrderInsert(PlatformStru_ParkedOrder* pData, int nRequestID)
{
	int ret = -999;

	///预埋单
	CThostFtdcParkedOrderField req={0};
	strncpy(req.BrokerID, pData->BrokerID, sizeof(req.BrokerID));						///经纪公司代码
	strncpy(req.InvestorID, pData->InvestorID, sizeof(req.InvestorID));					///投资者代码
	strncpy(req.InstrumentID, pData->InstrumentID, sizeof(req.InstrumentID));			///合约代码
	strncpy(req.OrderRef, pData->OrderRef, sizeof(req.OrderRef));						///报单引用
	strncpy(req.UserID, pData->UserID, sizeof(req.UserID));								///用户代码
	req.OrderPriceType = pData->OrderPriceType;											///报单价格条件
	req.Direction = pData->Direction;													///买卖方向
	strncpy(req.CombOffsetFlag, pData->CombOffsetFlag, sizeof(req.CombOffsetFlag));		///组合开平标志
	strncpy(req.CombHedgeFlag, pData->CombHedgeFlag, sizeof(req.CombHedgeFlag));		///组合投机套保标志
	req.LimitPrice = pData->LimitPrice;													///价格
	req.VolumeTotalOriginal = pData->VolumeTotalOriginal;								///数量
	req.TimeCondition = pData->TimeCondition;											///有效期类型
	strncpy(req.GTDDate, pData->GTDDate, sizeof(req.GTDDate));							///GTD日期
	req.VolumeCondition = pData->VolumeCondition;										///成交量类型
	req.MinVolume = pData->MinVolume;													///最小成交量
	req.ContingentCondition = pData->ContingentCondition;								///触发条件
	req.StopPrice = pData->StopPrice;													///止损价
	req.ForceCloseReason = pData->ForceCloseReason;										///强平原因
	req.IsAutoSuspend = pData->IsAutoSuspend;											///自动挂起标志
	strncpy(req.BusinessUnit, pData->BusinessUnit, sizeof(req.BusinessUnit));			///业务单元
	req.RequestID = pData->RequestID;													///请求编号
	req.UserForceClose = pData->UserForceClose;											///用户强评标志
	strncpy(req.ExchangeID, pData->ExchangeID, sizeof(req.ExchangeID));					///交易所代码
	strncpy(req.ParkedOrderID, pData->ParkedOrderID, sizeof(req.ParkedOrderID));		///预埋报单编号
	req.UserType = pData->UserType;														///用户类型
	req.Status = pData->Status;															///预埋单状态
	req.ErrorID = pData->ErrorID;														///错误代码
	strncpy(req.ErrorMsg, pData->ErrorMsg, sizeof(req.ErrorMsg));						///错误信息	

	ret = m_pTradeApi->ReqParkedOrderInsert(&req, nRequestID); 
	if(ret==0)
		_GetForwarding().SendPackageForApi(&req, sizeof(req), nRequestID);

	LOG_INFO("ReqOrderInsert(预埋报单录入请求):ret=[%d],nRequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
			ret, nRequestID,
			pData->BrokerID, pData->InvestorID, pData->InstrumentID, pData->OrderRef,pData->UserID,
			pData->OrderPriceType, pData->Direction, pData->CombOffsetFlag, pData->CombHedgeFlag,pData->LimitPrice,
			pData->VolumeTotalOriginal, pData->TimeCondition, pData->GTDDate, pData->VolumeCondition,pData->MinVolume,
			pData->ContingentCondition, pData->StopPrice, pData->ForceCloseReason, pData->IsAutoSuspend,pData->BusinessUnit,
			pData->RequestID, pData->UserForceClose);

	return ret;
}

///预埋撤单录入请求
int CFTTraderApi::ReqParkedOrderAction(PlatformStru_ParkedOrderAction* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqParkedOrderAction(&pData->Thost, nRequestID); 
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqParkedOrderAction(预埋撤单录入请求):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.OrderActionRef, pData->Thost.OrderRef,pData->Thost.RequestID,
		pData->Thost.FrontID, pData->Thost.SessionID, pData->Thost.ExchangeID, pData->Thost.OrderSysID,pData->Thost.ActionFlag,
		pData->Thost.LimitPrice, pData->Thost.VolumeChange, pData->Thost.UserID, pData->Thost.InstrumentID);

	return ret;
}

///报单操作请求
int CFTTraderApi::ReqOrderAction(PlatformStru_InputOrderAction* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqOrderAction(&pData->Thost, nRequestID); 
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqOrderAction(报单操作请求):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.OrderActionRef, pData->Thost.OrderRef,pData->Thost.RequestID,
		pData->Thost.FrontID, pData->Thost.SessionID, pData->Thost.ExchangeID, pData->Thost.OrderSysID,pData->Thost.ActionFlag,
		pData->Thost.LimitPrice, pData->Thost.VolumeChange, pData->Thost.UserID, pData->Thost.InstrumentID);

	return ret;
}

///查询最大报单数量请求
int CFTTraderApi::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQueryMaxOrderVolume(&pData->Thost, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQueryMaxOrderVolume(查询最大报单数量请求):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t Direction=[%d],\n"
		"\t\t\t HedgeFlag=[%d],\t MaxVolume=[%d],\t OffsetFlag=[%d]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.InstrumentID, pData->Thost.Direction,
		pData->Thost.HedgeFlag, pData->Thost.MaxVolume, pData->Thost.OffsetFlag);

	return ret;
}

///投资者结算结果确认
int CFTTraderApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqSettlementInfoConfirm(pData, nRequestID);
	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqSettlementInfoConfirm(投资者结算结果确认):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
		ret, nRequestID, 
		pData->BrokerID,
		pData->InvestorID,
		pData->ConfirmDate,
		pData->ConfirmTime);

	return ret;
}

///请求删除预埋单
int CFTTraderApi::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqRemoveParkedOrder(&pData->Thost, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqRemoveParkedOrder(请求删除预埋单):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.ParkedOrderID);

	return ret;
}

///请求删除预埋撤单
int CFTTraderApi::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqRemoveParkedOrderAction(&pData->Thost, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqRemoveParkedOrderAction(请求删除预埋撤单):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, 
		pData->Thost.InvestorID, 
		pData->Thost.ParkedOrderActionID);

	return ret;
}

///询价录入请求
int CFTTraderApi::ReqForQuoteInsert(PlatformStru_InputForQuoteField* pData, int nRequestID)
{
	int ret = -999;	

    CThostFtdcInputForQuoteField thost;
    pData->ToThost(thost);

    ret = m_pTradeApi->ReqForQuoteInsert(&thost, nRequestID);  
	if(ret==0)
 		_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);
    LOG_INFO("ReqForQuoteInsert(请求询价录入):ret=[%d],nRequestID=[%d]\n%s",
             ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///执行宣告录入请求
int CFTTraderApi::ReqExecOrderInsert(PlatformStru_InputExecOrderField* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcInputExecOrderField thost;
	pData->ToThost(thost);

	ret = m_pTradeApi->ReqExecOrderInsert(&thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);

	LOG_INFO("ReqExecOrderInsert(执行宣告录入请求):ret=[%d],nRequestID=[%d]\n%s",
		ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///执行宣告操作请求
int CFTTraderApi::ReqExecOrderAction(PlatformStru_InputExecOrderActionField* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcInputExecOrderActionField thost;
	pData->ToThost(thost);

	ret = m_pTradeApi->ReqExecOrderAction(&thost, nRequestID);  
	if(ret==0)
	 	_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);
	
	LOG_INFO("ReqExecOrderAction(执行宣告操作请求):ret=[%d],nRequestID=[%d]\n%s",
		ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///请求查询报单
int CFTTraderApi::ReqQryOrder(PlatformStru_QryOrder* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryOrder(&pData->Thost, nRequestID);
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryOrder(请求查询报单):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderSysID=[%s],\n"
		"\t\t\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID,
		pData->Thost.OrderSysID,
		pData->Thost.InsertTimeStart,
		pData->Thost.InsertTimeEnd);

	return ret;
}

///请求查询成交
int CFTTraderApi::ReqQryTrade(PlatformStru_QryTrade* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryTrade(&pData->Thost, nRequestID);
	if(ret==0)
 		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryTrade(请求查询成交):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t TradeID=[%s],\n"
		"\t\t\t TradeTimeStart=[%s],\t TradeTimeEnd=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID,
		pData->Thost.TradeID,
		pData->Thost.TradeTimeStart,
		pData->Thost.TradeTimeEnd);

	return ret;
}

///请求查询投资者持仓
int CFTTraderApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcQryInvestorPositionField field;
	memset( &field, 0, sizeof(field));
	strncpy(field.BrokerID, pData->BrokerID,sizeof(field.BrokerID)-1);
	strncpy(field.InstrumentID, pData->InstrumentID,sizeof(field.InstrumentID)-1);
	strncpy(field.InvestorID, pData->InvestorID,sizeof(field.InvestorID)-1);

	ret = m_pTradeApi->ReqQryInvestorPosition(&field, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&field, sizeof(field), nRequestID);

	LOG_INFO("ReqQryInvestorPosition(请求查询投资者持仓):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);

	return ret;
}

///请求查询资金账户
int CFTTraderApi::ReqQryTradingAccount(PlatformStru_QryTradingAccount* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcQryTradingAccountField QryTradingAccount;
	memset(&QryTradingAccount,0,sizeof(QryTradingAccount));
	strncpy(QryTradingAccount.BrokerID,pData->BrokerID,sizeof(QryTradingAccount.BrokerID)-1);
	strncpy(QryTradingAccount.InvestorID,pData->InvestorID,sizeof(QryTradingAccount.InvestorID)-1);

	ret = m_pTradeApi->ReqQryTradingAccount(&QryTradingAccount, nRequestID); 
	if(ret==0)
 		_GetForwarding().SendPackageForApi(&QryTradingAccount, sizeof(QryTradingAccount), nRequestID);

	LOG_INFO("ReqQryTradingAccount(请求查询资金账户):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///请求查询投资者
int CFTTraderApi::ReqQryInvestor(PlatformStru_QryInvestor* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryInvestor(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryInvestor(请求查询投资者):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID);

	return ret;
}

///请求查询合约保证金率
int CFTTraderApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryInstrumentMarginRate(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryInstrumentMarginRate(请求查询合约保证金率):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID,
		pData->HedgeFlag);

	return ret;
}

///请求查询合约手续费率
int CFTTraderApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryInstrumentCommissionRate(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryInstrumentCommissionRate(请求查询合约手续费率):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);

	return ret;
}

///请求查询交易所
int CFTTraderApi::ReqQryExchange(CThostFtdcQryExchangeField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryExchange(pData, nRequestID);
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryExchange(请求查询交易所):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t ExchangeID=[%s]",
		ret, nRequestID,
		pData->ExchangeID);

	return ret;
}

///请求查询产品
int CFTTraderApi::ReqQryProduct(PlatformStru_QryProduct* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryProduct(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryProduct(请求查询品种):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t ProductID=[%s],\t ProductClass=[%d]",
		ret, nRequestID, 
		pData->Thost.ProductID,
		pData->Thost.ProductClass);

	return ret;
}

///请求查询合约
int CFTTraderApi::ReqQryInstrument(PlatformStru_QryInstrument* pData, int nRequestID)
{
	int ret = -999;
	
	ret = m_pTradeApi->ReqQryInstrument(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryInstrument(请求查询合约):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\tProductID=[%s]",
		ret, nRequestID, 
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID,
		pData->Thost.ExchangeInstID,
		pData->Thost.ProductID);

	return ret;
}

///请求查询投资者结算结果
int CFTTraderApi::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQrySettlementInfo(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQrySettlementInfo(请求查询投资者结算结果):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t TradingDay=[%s]",
		ret, nRequestID, 
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.TradingDay);
	
	return ret;
}

///请求查询投资者持仓明细
int CFTTraderApi::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail* pData, int nRequestID)
{
	int ret = -999;
	

	CThostFtdcQryInvestorPositionDetailField QryInvestorPositionDetail;
	memset(&QryInvestorPositionDetail,0,sizeof(QryInvestorPositionDetail));
	strncpy(QryInvestorPositionDetail.BrokerID,pData->BrokerID,sizeof(QryInvestorPositionDetail.BrokerID)-1);
	strncpy(QryInvestorPositionDetail.InvestorID,pData->InvestorID,sizeof(QryInvestorPositionDetail.InvestorID)-1);
	strncpy(QryInvestorPositionDetail.InstrumentID,pData->InstrumentID,sizeof(QryInvestorPositionDetail.InstrumentID)-1);

	ret = m_pTradeApi->ReqQryInvestorPositionDetail(&QryInvestorPositionDetail, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&QryInvestorPositionDetail, sizeof(QryInvestorPositionDetail), nRequestID);

	LOG_INFO("ReqQryInvestorPositionDetail(请求查询投资者持仓明细):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);

	return ret;
}

///请求查询客户通知
int CFTTraderApi::ReqQryNotice(PlatformStru_QryNotice* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryNotice(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryNotice(请求查询客户通知):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID);

	return ret;
}

///请求查询结算信息确认
int CFTTraderApi::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQrySettlementInfoConfirm(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQrySettlementInfoConfirm(请求查询结算信息确认):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID);

	return ret;
}

///请求查询投资者持仓明细
int CFTTraderApi::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail* pData, int nRequestID)
{
	int ret = -999;	

	CThostFtdcQryInvestorPositionCombineDetailField QryInvestorPositionCombineDetail;
	memset(&QryInvestorPositionCombineDetail,0,sizeof(QryInvestorPositionCombineDetail));
	strncpy(QryInvestorPositionCombineDetail.BrokerID,pData->BrokerID,sizeof(QryInvestorPositionCombineDetail.BrokerID)-1);
	strncpy(QryInvestorPositionCombineDetail.InvestorID,pData->InvestorID,sizeof(QryInvestorPositionCombineDetail.InvestorID)-1);
	strncpy(QryInvestorPositionCombineDetail.CombInstrumentID,pData->CombInstrumentID,sizeof(QryInvestorPositionCombineDetail.CombInstrumentID)-1);

	ret = m_pTradeApi->ReqQryInvestorPositionCombineDetail(&QryInvestorPositionCombineDetail, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&QryInvestorPositionCombineDetail, sizeof(QryInvestorPositionCombineDetail), nRequestID);

	LOG_INFO("ReqQryInvestorPositionCombineDetail(请求查询投资者组合持仓明细):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t CombInstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->CombInstrumentID);
	
	return ret;
}

///请求查询保证金监管系统经纪公司资金账户密钥
int CFTTraderApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryCFMMCTradingAccountKey(pData, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);
	
	LOG_INFO("ReqQryCFMMCTradingAccountKey(请求查询保证金监管系统经纪公司资金账户密钥):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID, 
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///请求查询期权交易成本
int CFTTraderApi::ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField* pData, int nRequestID)
{
	int ret = -999;
	
	ret = m_pTradeApi->ReqQryOptionInstrTradeCost(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryOptionInstrTradeCost(请求查询期权交易成本):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d],\t InputPrice=[%g],\t UnderlyingPrice=[%g]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID,
		pData->HedgeFlag,
		pData->InputPrice,
		pData->UnderlyingPrice
		);

	return ret;
}

///请求查询期权合约手续费
int CFTTraderApi::ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryOptionInstrCommRate(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryOptionInstrCommRate(请求查询期权合约手续费率):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);
	
	return ret;
}

///请求查询执行宣告
int CFTTraderApi::ReqQryExecOrder(PlatformStru_QryExecOrderField* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcQryExecOrderField thost;
	pData->ToThost(thost);

	ret = m_pTradeApi->ReqQryExecOrder(&thost, nRequestID);
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);

	LOG_INFO("ReqQryExecOrder(请求查询执行宣告):ret=[%d],nRequestID=[%d]\n%s",
		ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///请求查询转帐流水
int CFTTraderApi::ReqQryTransferSerial(PlatformStru_QryTransferSerial* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryTransferSerial(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryTransferSerial(请求查询转帐流水) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
		ret,nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.AccountID,
		pData->Thost.BankID);

	return ret;
}

///请求查询银期签约关系
int CFTTraderApi::ReqQryAccountregister(PlatformStru_QryAccountRegister* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryAccountregister(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryAccountregister(请求查询银期签约关系) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
		ret,nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.AccountID,
		pData->Thost.BankID);
	
	return ret;
}

///请求查询签约银行
int CFTTraderApi::ReqQryContractBank(PlatformStru_QryContractBank* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryContractBank(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqQryContractBank(请求查询签约银行) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t BankID=[%s],\t BankBrchID=[%s]",
		ret,nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.BankID,
		pData->Thost.BankBrchID);
	
	return ret;
}

///请求查询预埋单
int CFTTraderApi::ReqQryParkedOrder(PlatformStru_QryParkedOrder* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryParkedOrder(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqQryParkedOrder(请求查询预埋单):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID);
	
	return ret;
}

///请求查询预埋撤单
int CFTTraderApi::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryParkedOrderAction(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqQryParkedOrderAction(请求查询预埋撤单):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID);
	
	return ret;
}

///请求查询交易通知
int CFTTraderApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryTradingNotice(pData, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryTradingNotice(请求查询交易通知):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///请求查询经纪公司交易参数
int CFTTraderApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryBrokerTradingParams(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryBrokerTradingParams(请求查询经纪公司交易参数):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t CurrencyID=[%s],\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->CurrencyID,
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///期货发起银行资金转期货请求
int CFTTraderApi::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer* pData, int nRequestID)
{
	int ret = -999;
	
	ret = m_pTradeApi->ReqFromBankToFutureByFuture(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqFromBankToFutureByFuture(期货发起银行资金转期货请求) : ret=[%d],nRequestID=[%d],\n"
	                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
	                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
	                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
	                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[******],\n"
	                    "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
	                    "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
	                    "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
	                    "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
	                    "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
	                    ret,nRequestID,
	                    pData->Thost.TradeCode, pData->Thost.BankID,pData->Thost.BankBranchID,pData->Thost.BrokerID,pData->Thost.BrokerBranchID,
	                    pData->Thost.TradeDate, pData->Thost.TradeTime,pData->Thost.BankSerial,pData->Thost.TradingDay,pData->Thost.PlateSerial,
	                    pData->Thost.LastFragment, pData->Thost.SessionID,pData->Thost.CustomerName,pData->Thost.IdCardType,pData->Thost.IdentifiedCardNo,
	                    pData->Thost.CustType, pData->Thost.BankAccount,pData->Thost.BankPassWord,pData->Thost.AccountID,/*pData->Thost.Password,*/
	                    pData->Thost.InstallID,pData->Thost.FutureSerial, pData->Thost.UserID,pData->Thost.VerifyCertNoFlag,pData->Thost.CurrencyID,
	                    pData->Thost.TradeAmount,pData->Thost.FutureFetchAmount, pData->Thost.FeePayFlag,pData->Thost.CustFee,pData->Thost.BrokerFee,
	                    pData->Thost.Message, pData->Thost.Digest, pData->Thost.BankAccType,pData->Thost.DeviceID,pData->Thost.BankSecuAccType,
	                    pData->Thost.BrokerIDByBank, pData->Thost.BankSecuAcc, pData->Thost.BankPwdFlag,pData->Thost.SecuPwdFlag,pData->Thost.OperNo,
	                    pData->Thost.RequestID, pData->Thost.TID, pData->Thost.TransferStatus
	                    );

	return ret;
}

///期货发起期货资金转银行请求
int CFTTraderApi::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqFromFutureToBankByFuture(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqFromFutureToBankByFuture(期货发起期货资金转银行请求) : ret=[%d],nRequestID=[%d],\n"
	                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
	                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
	                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
	                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[******],\n"
	                    "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
	                    "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
	                    "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
	                    "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
	                    "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
	                    ret,nRequestID,
	                    pData->Thost.TradeCode, pData->Thost.BankID,pData->Thost.BankBranchID,pData->Thost.BrokerID,pData->Thost.BrokerBranchID,
	                    pData->Thost.TradeDate, pData->Thost.TradeTime,pData->Thost.BankSerial,pData->Thost.TradingDay,pData->Thost.PlateSerial,
	                    pData->Thost.LastFragment, pData->Thost.SessionID,pData->Thost.CustomerName,pData->Thost.IdCardType,pData->Thost.IdentifiedCardNo,
	                    pData->Thost.CustType, pData->Thost.BankAccount,pData->Thost.BankPassWord,pData->Thost.AccountID,/*pData->Thost.Password,*/
	                    pData->Thost.InstallID,pData->Thost.FutureSerial, pData->Thost.UserID,pData->Thost.VerifyCertNoFlag,pData->Thost.CurrencyID,
	                    pData->Thost.TradeAmount,pData->Thost.FutureFetchAmount, pData->Thost.FeePayFlag,pData->Thost.CustFee,pData->Thost.BrokerFee,
	                    pData->Thost.Message, pData->Thost.Digest, pData->Thost.BankAccType,pData->Thost.DeviceID,pData->Thost.BankSecuAccType,
	                    pData->Thost.BrokerIDByBank, pData->Thost.BankSecuAcc, pData->Thost.BankPwdFlag,pData->Thost.SecuPwdFlag,pData->Thost.OperNo,
	                    pData->Thost.RequestID, pData->Thost.TID, pData->Thost.TransferStatus
	                    );

	return ret;
}

///期货发起查询银行余额请求
int CFTTraderApi::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQueryBankAccountMoneyByFuture(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQueryBankAccountMoneyByFuture(期货发起查询银行余额请求) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
		"\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
		"\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
		"\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[******],\t AccountID=[%s],\t Password=[******],\n"
		"\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
		"\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
		"\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
		"\t\t\t TID=[%d]",
		ret,nRequestID,
		pData->Thost.TradeCode, pData->Thost.BankID,pData->Thost.BankBranchID,pData->Thost.BrokerID,pData->Thost.BrokerBranchID,
		pData->Thost.TradeDate, pData->Thost.TradeTime,pData->Thost.BankSerial,pData->Thost.TradingDay,pData->Thost.PlateSerial,
		pData->Thost.LastFragment, pData->Thost.SessionID,pData->Thost.CustomerName,pData->Thost.IdCardType,pData->Thost.IdentifiedCardNo,
		pData->Thost.CustType, pData->Thost.BankAccount,pData->Thost.AccountID,
		pData->Thost.FutureSerial, pData->Thost.InstallID,pData->Thost.UserID,pData->Thost.VerifyCertNoFlag,pData->Thost.CurrencyID,
		pData->Thost.Digest, pData->Thost.BankAccType,pData->Thost.DeviceID,pData->Thost.BankSecuAccType,pData->Thost.BrokerIDByBank,
		pData->Thost.BankSecuAcc, pData->Thost.BankPwdFlag,pData->Thost.SecuPwdFlag,pData->Thost.OperNo,pData->Thost.RequestID,
		pData->Thost.TID);		

	return ret;
}
