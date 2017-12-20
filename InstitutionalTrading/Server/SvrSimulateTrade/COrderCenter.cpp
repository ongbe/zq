#include "StdAfx.h"
#include "COrderCenter.h"
#include "tools_util.h"
#include "FileOpr.h"

#if 0
#define TRADE_LOG(fmt, ...)  ;
#else
#define TRADE_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","SimulateTrade", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

bool COrderCenter::IsInvalidValue(double fVal)
{
	return util::isInvalidValue(fVal);
}

void COrderCenter::split(const std::string& s, char c, std::vector<std::string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}
}

bool COrderCenter::ConvInput2Order(const CThostFtdcInputOrderField &input, 
								   const std::string& strExchangeID, 
								   CThostFtdcOrderField& order)
{
	strncpy(order.BrokerID, input.BrokerID,sizeof(order.BrokerID)-1);
	strncpy(order.InvestorID, input.InvestorID,sizeof(order.InvestorID)-1);
	strncpy(order.InstrumentID, input.InstrumentID,sizeof(order.InstrumentID)-1);
	strncpy(order.OrderRef, input.OrderRef,sizeof(order.OrderRef)-1);
	strncpy(order.UserID, input.InvestorID, sizeof(order.UserID)-1);
	order.OrderPriceType = input.OrderPriceType;
	order.Direction = input.Direction;
	order.CombOffsetFlag[0] = input.CombOffsetFlag[0];
	order.CombHedgeFlag[0] = input.CombHedgeFlag[0];
	order.LimitPrice = input.LimitPrice;
	order.VolumeTotalOriginal = input.VolumeTotalOriginal;
	order.TimeCondition = input.TimeCondition;
	strncpy(order.GTDDate, input.GTDDate, sizeof(order.GTDDate)-1);
	order.VolumeCondition = input.VolumeCondition;
	order.MinVolume = input.MinVolume;
	order.ContingentCondition = input.ContingentCondition;
	order.StopPrice = input.StopPrice;
	order.ForceCloseReason = input.ForceCloseReason;
	order.IsAutoSuspend = input.IsAutoSuspend;
	strncpy(order.BusinessUnit, input.BusinessUnit, sizeof(order.BusinessUnit)-1);
	order.RequestID = input.RequestID;
	//strncpy(order.OrderLocalID, input.OrderLocalID, sizeof(order.OrderLocalID)-1);
	strncpy(order.ExchangeID, strExchangeID.c_str(), sizeof(order.ExchangeID)-1);
	//strncpy(order.ParticipantID, input.ParticipantID, sizeof(order.ParticipantID)-1);
	//strncpy(order.ClientID, input.ClientID, sizeof(order.ClientID)-1);
	strncpy(order.ExchangeInstID, input.InstrumentID, sizeof(order.ExchangeInstID)-1);
	//strncpy(order.TraderID, input.TraderID, sizeof(order.TraderID)-1);
	//order.InstallID = input.InstallID;
	//order.OrderSubmitStatus = input.OrderSubmitStatus;
	//order.NotifySequence = input.NotifySequence;
	//strncpy(order.TradingDay, input.TradingDay, sizeof(order.TradingDay)-1);
	order.SettlementID = 1;//input.SettlementID;
	//strncpy(order.OrderSysID, input.OrderSysID, sizeof(order.OrderSysID)-1);
	//order.OrderSource = input.OrderSource;
	//order.OrderStatus = input.OrderStatus;
	//order.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	//order.OrderType = input.OrderType;
	order.VolumeTraded = 0;
	order.VolumeTotal = 0;
	//strncpy(order.InsertDate, input.InsertDate, sizeof(order.InsertDate)-1);
	//strncpy(order.InsertTime, input.InsertTime, sizeof(order.InsertTime)-1);
	//strncpy(order.ActiveTime, input.ActiveTime, sizeof(order.ActiveTime)-1);
	//strncpy(order.SuspendTime, input.SuspendTime, sizeof(order.SuspendTime)-1);
	//strncpy(order.UpdateTime, input.UpdateTime, sizeof(order.UpdateTime)-1);
	//strncpy(order.CancelTime, input.CancelTime, sizeof(order.CancelTime)-1);
	//strncpy(order.ActiveTraderID, input.ActiveTraderID, sizeof(order.ActiveTraderID)-1);
	//strncpy(order.ClearingPartID, input.ClearingPartID, sizeof(order.ClearingPartID)-1);
	//order.SequenceNo = input.SequenceNo;
	order.FrontID = 0;//input.FrontID;
	order.SessionID = 0;//input.SessionID;
	//strncpy(order.UserProductInfo, input.UserProductInfo, sizeof(order.UserProductInfo)-1);
	//strncpy(order.StatusMsg, input.StatusMsg, sizeof(order.StatusMsg)-1);
	order.UserForceClose = input.UserForceClose;
	//strncpy(order.ActiveUserID, input.ActiveUserID, sizeof(order.ActiveUserID)-1);
	//order.BrokerOrderSeq = input.BrokerOrderSeq;
	//strncpy(order.RelativeOrderSysID, input.RelativeOrderSysID, sizeof(order.RelativeOrderSysID)-1);

	return true;
}

bool COrderCenter::ConvOrder2Trade(const CThostFtdcOrderField &order, 
				CThostFtdcTradeField& trade)
{
	strncpy(trade.BrokerID, order.BrokerID,sizeof(trade.BrokerID)-1);
	strncpy(trade.InvestorID, order.InvestorID,sizeof(trade.InvestorID)-1);
	strncpy(trade.InstrumentID, order.InstrumentID,sizeof(trade.InstrumentID)-1);
	strncpy(trade.OrderRef, order.OrderRef,sizeof(trade.OrderRef)-1);
	strncpy(trade.UserID, order.UserID,sizeof(trade.UserID)-1);
	strncpy(trade.ExchangeID, order.ExchangeID,sizeof(trade.ExchangeID)-1);
	//strncpy(trade.TradeID, order.TradeID,sizeof(trade.TradeID)-1);
	trade.Direction = order.Direction;
	strncpy(trade.OrderSysID, order.OrderSysID,sizeof(trade.OrderSysID)-1);
	strncpy(trade.ParticipantID, order.ParticipantID,sizeof(trade.ParticipantID)-1);
	strncpy(trade.ClientID, order.ClientID,sizeof(trade.ClientID)-1);
	//trade.TradingRole = order.TradingRole;
	strncpy(trade.ExchangeInstID, order.ExchangeInstID,sizeof(trade.ExchangeInstID)-1);
	trade.OffsetFlag = order.CombOffsetFlag[0];
	trade.HedgeFlag = order.CombHedgeFlag[0];
	//trade.Price = order.Price;
	trade.Volume = order.VolumeTotalOriginal;
	//strncpy(trade.TradeDate, order.InsertDate,sizeof(trade.TradeDate)-1);
	//strncpy(trade.TradeTime, order.TradeTime,sizeof(trade.TradeTime)-1);
	//trade.TradeType = order.TradeType;
	//trade.PriceSource = order.PriceSource;
	strncpy(trade.TraderID, order.TraderID,sizeof(trade.TraderID)-1);
	strncpy(trade.OrderLocalID, order.OrderLocalID,sizeof(trade.OrderLocalID)-1);
	strncpy(trade.ClearingPartID, order.ClearingPartID,sizeof(trade.ClearingPartID)-1);
	//strncpy(trade.BusinessUnit, BusinessUnit,sizeof(trade.BusinessUnit)-1);
	trade.SequenceNo = order.SequenceNo;
	strncpy(trade.TradingDay, order.TradingDay,sizeof(trade.TradingDay)-1);
	trade.SettlementID = order.SettlementID;
	trade.BrokerOrderSeq = order.BrokerOrderSeq;
	//trade.TradeSource = order.TradeSource;
	return true;
}

COrderCenter::COrderCenter(void)
{
	n_nSessionID = 0;
	m_nSeqID = 0;
	m_nOrderID = 0;
	m_nTradeID = 0;
}

COrderCenter::~COrderCenter(void)
{
}

void COrderCenter::LoadExchangeTime()
{
	int nPathLen = 0;
	char localPath[256];
	memset(localPath, 0, 256);
	nPathLen = GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	string strPath(filename.substr(0, splitpos+1).c_str());
	strPath += "SvrSimulateTrade.xml";

	TiXmlDocument* pXmlDocment = new TiXmlDocument(strPath.c_str());
	if(pXmlDocment == NULL) 
		return;

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() ) 
		return;

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( root == NULL ) 
		return;

	if( std::string(root->Value()) != "root")
		return;

	//各交易所交易时间配置文件

	TiXmlElement *tList = root->FirstChildElement("tradingtime");
	if(tList == NULL)
		return;

	int nStartTime = 0, nEndTime = 0, nSubNum = 0;
	int nHour = 0, nMinite = 0;
	EXCHANGETIME subTime;

	for(TiXmlElement *GroupItem = tList->FirstChildElement("group");
			GroupItem; 
			GroupItem = GroupItem->NextSiblingElement("group")) {

		const char *pGroupid = GroupItem->Attribute("id");
		if(!pGroupid) 
			continue;

		string strID = pGroupid;
		const char *pGrouptime = GroupItem->Attribute("time");
		if(!pGrouptime)
			continue;

		vector<EXCHANGETIME> vecRestTime;
		string str = pGrouptime;
		vector<string> vecTime;
		split(str, ' ', vecTime);
		for(int i = 0; i < (int)vecTime.size(); i++) {
			string strTime = vecTime[i];
			vector<string> vecOne;
			split(strTime, '-', vecOne);
			if(vecOne.size() != 2)
				break;

			nHour = 0, nMinite = 0;
			sscanf(vecOne[0].c_str(), "%d:%d", &nHour, &nMinite);
			nStartTime = nHour*3600 + nMinite*60;
			nHour = 0, nMinite = 0;
			sscanf(vecOne[1].c_str(), "%d:%d", &nHour, &nMinite);
			nEndTime = nHour*3600 + nMinite*60;

			nSubNum = nEndTime - nStartTime;

			if(nEndTime<=nStartTime)
				break;

			ZeroMemory(&subTime, sizeof(subTime));
			subTime.nStart = nStartTime;
			subTime.nEnd = nEndTime;
			subTime.nSubNum = nSubNum;

			vecRestTime.push_back(subTime);
		}
		m_mapTradingTime.insert(std::make_pair(strID,  vecRestTime));			
	}
	//delete pXmlDocment;
}

bool COrderCenter::IsInTradingTime(const std::string& strExchangeID, int nTime)
{
	char strExchID[128] = {0};
	
	memset(strExchID, 0, sizeof(strExchID));
	strncpy(strExchID, strExchangeID.c_str(), sizeof(strExchID)-1);
	_strlwr(strExchID);
	MAPEXCHANGETIME::iterator it = m_mapTradingTime.find(strExchID);
	if(it != m_mapTradingTime.end()) {
		int nSubTime = 0;
		for(int i=0; i < it->second.size(); i++) {
			if(nTime >= it->second[i].nStart && nTime <= it->second[i].nEnd)
				return true;
		}
	}
	return false;
}

// 重置模拟成交功能
bool COrderCenter::Reinit(const std::string& strTradingDay)
{

	int nPathLen = 0, nRet = 0;
	char localPath[256];
	char csText[1024] = {0};
	memset(localPath, 0, 256);
	nPathLen = GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	string strPath(filename.substr(0, splitpos+1).c_str());
	strPath += "SvrSimulateTrade.ini";

	m_nSeqID = ::GetPrivateProfileInt("save", "seqid", 0, strPath.c_str());
	m_nOrderID = ::GetPrivateProfileInt("save", "orderid", 0, strPath.c_str());
	m_nTradeID = ::GetPrivateProfileInt("save", "tradeid", 0, strPath.c_str());

	nRet = ::GetPrivateProfileString("save", "tradingday", "", csText, sizeof(csText)-1, strPath.c_str());
	if(strTradingDay.compare(csText)==0)
		return false;

	LockObject();

	m_strTradingDay = strTradingDay;

	m_mapTradingTime.clear();
	LoadExchangeTime();

	m_mapUserSession.clear();
	m_mapQuot.clear();
	m_mapOrder.clear();
	m_mapInst2Order.clear();
	n_nSessionID = 0;
	m_nSeqID = 0;
	m_nOrderID = 0;
	m_nTradeID = 0;

	UnlockObject();
	return true;
}

//推送报单
bool COrderCenter::PushOrder(const CThostFtdcOrderField& order)
{
	bool bRet = FALSE;
	char strOrderKey[256] = {0};
	sprintf_s(strOrderKey, sizeof(strOrderKey)-1, 
			"%s,%s,%s,%d,%d", order.BrokerID, order.InvestorID, 
			order.OrderRef, order.FrontID, order.SessionID); 
	LockObject();
	if(order.OrderStatus==THOST_FTDC_OST_PartTradedQueueing || 
			order.OrderStatus==THOST_FTDC_OST_NoTradeQueueing) {
		bRet = m_mapOrder.insert(std::make_pair(strOrderKey, order)).second;
	}
	UnlockObject();
	return bRet;
}

//帐号登录
bool COrderCenter::Login(const CThostFtdcRspUserLoginField& rspLogin)
{
	std::string strUserKey = rspLogin.BrokerID;
	MAPUSERSESSION::iterator it;
	int nSesssionID = GetNextSessionID();

	strUserKey += ",";
	strUserKey += rspLogin.UserID;

	LockObject();

	m_mapUserSession[strUserKey] = rspLogin;

	//it = m_mapUserSession.find(strUserKey);
	//if(it != m_mapUserSession.end()) {
	//	UnlockObject();
	//	return false;
	//}
	//strncpy(rspLogin.BrokerID, reqUserLoginField.BrokerID, sizeof(rspLogin.BrokerID)-1);
	//strncpy(rspLogin.UserID, reqUserLoginField.UserID, sizeof(rspLogin.UserID)-1);
	//rspLogin.FrontID = 1;
	//rspLogin.SessionID = nSesssionID;
	//strncpy(rspLogin.MaxOrderRef, "1", sizeof(rspLogin.MaxOrderRef)-1);

	//m_mapUserSession.insert(std::make_pair(strUserKey, rspLogin));

	UnlockObject();

	return true;
}

//帐号登出
bool COrderCenter::Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID)
{
	std::string strUserKey = userLogout.BrokerID;
	MAPUSERSESSION::iterator it;

	strUserKey += ",";
	strUserKey += userLogout.UserID;

	LockObject();

	it = m_mapUserSession.find(strUserKey);
	if(it != m_mapUserSession.end()) {
		m_mapUserSession.erase(it);
	}

	UnlockObject();

	return true;
}

//重置交易日，并把所有挂单也重置
bool COrderCenter::ResertTradingDay(const std::string& strTradingDay)
{
	MAPORDER::iterator it;

	LockObject();
	
	m_strTradingDay = strTradingDay;
	for(it = m_mapOrder.begin(); it != m_mapOrder.end(); it++) {
		strncpy(it->second.TradingDay, strTradingDay.c_str(), sizeof(it->second.TradingDay)-1);
	}
	
	UnlockObject();

	return true;
}

//下单接口
int COrderCenter::InsertOrder(const CThostFtdcInputOrderField &input, 
							  int nRequestID, const std::string& strExchangeID, 
							  std::vector<CThostFtdcOrderField>& vecOrder, 
							  std::vector<CThostFtdcTradeField>& vecTrade)
{
	MAPORDER::iterator it;
	MAPQUOT::iterator itQuot;
	MAPINSTID2ORDER::iterator itIID;
	int nFrontID = 0, nSessionID = 0;
	char strOrderKey[256] = {0};
	CThostFtdcOrderField order = {0};
	SYSTEMTIME sysTime;
	
	LockObject();
	if(!GetUserSession(std::string(input.BrokerID), 
			std::string(input.InvestorID), nFrontID, nSessionID)) {
		UnlockObject();
		return 1;
	}

	sprintf_s(strOrderKey, sizeof(strOrderKey)-1, 
			"%s,%s,%s,%d,%d", input.BrokerID, input.InvestorID, 
			input.OrderRef, nFrontID, nSessionID); 

	it = m_mapOrder.find(strOrderKey);
	if(it != m_mapOrder.end()) {
		UnlockObject();
		return 2;
	}

	::GetLocalTime(&sysTime);

	ConvInput2Order(input, strExchangeID, order);
	order.FrontID = nFrontID;
	order.SessionID = nSessionID;
	MarkOrderBase(order);
	m_mapOrder.insert(std::make_pair(strOrderKey, order));
	itIID = m_mapInst2Order.find(order.InstrumentID);
	if(itIID != m_mapInst2Order.end()) {
		itIID->second.insert(strOrderKey);
	}
	else {
		std::set<std::string> setKey;
		setKey.insert(strOrderKey);
		m_mapInst2Order.insert(std::make_pair(order.InstrumentID, setKey));
	}
	vecOrder.push_back(order);

	if(!IsInTradingTime(strExchangeID, sysTime.wHour*3600+sysTime.wMinute*60+sysTime.wSecond)) {
		order.OrderStatus = THOST_FTDC_OST_Canceled;
		strncpy(order.StatusMsg, "已撤单报单被拒绝当前状态禁止此项操作", sizeof(order.StatusMsg)-1);
		sprintf_s(order.CancelTime, sizeof(order.CancelTime), "%02d:%02d:%02d", 
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		strncpy(order.UpdateTime, order.CancelTime, sizeof(order.UpdateTime));
		m_mapOrder.erase(strOrderKey);

		vecOrder.push_back(order);
		return 0;
	}

	MarkOrderSys(order);

	itQuot = m_mapQuot.find(order.InstrumentID);
	if(order.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		// 处理市价的情况
		if(itQuot != m_mapQuot.end() && !IsInvalidValue(itQuot->second.LastPrice)) {
			CThostFtdcTradeField trade = {0};
			ConvOrder2Trade(order, trade);
			MarkTrade(trade, itQuot->second.LastPrice);
			order.VolumeTotal = 0;
			order.VolumeTraded = order.VolumeTotalOriginal;
			order.OrderStatus = THOST_FTDC_OST_AllTraded;
			strncpy(order.StatusMsg, "全部成交报单已提交", sizeof(order.StatusMsg)-1);
			sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			m_mapOrder.erase(strOrderKey);
			itIID = m_mapInst2Order.find(order.InstrumentID);
			if(itIID != m_mapInst2Order.end()) {
				itIID->second.erase(strOrderKey);
			}
			vecOrder.push_back(order);
			vecTrade.push_back(trade);
		}
		else {
			// 无行情，市价不能成交
			order.OrderStatus = THOST_FTDC_OST_Canceled;
			strncpy(order.StatusMsg, "无行情市价撤单", sizeof(order.StatusMsg)-1);
			sprintf_s(order.CancelTime, sizeof(order.CancelTime), "%02d:%02d:%02d", 
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			strncpy(order.UpdateTime, order.CancelTime, sizeof(order.UpdateTime));
			m_mapOrder.erase(strOrderKey);
			itIID = m_mapInst2Order.find(order.InstrumentID);
			if(itIID != m_mapInst2Order.end()) {
				itIID->second.erase(strOrderKey);
			}
			vecOrder.push_back(order);
		}
	}
	else {
		if(itQuot!=m_mapQuot.end()) {
			TRADE_LOG("OrderInsert [%s] Direction==[%c] OpenClose==[%c] LimitPrice==[%f], quot.InstrumentID==[%s], Quot.LastPrice==[%f], Quot.UpdateTime==[%s.%03d]\r\n", 
				order.InstrumentID, order.Direction, order.CombOffsetFlag[0], 
				order.LimitPrice, itQuot->second.InstrumentID, itQuot->second.LastPrice, 
				itQuot->second.UpdateTime, itQuot->second.UpdateMillisec);
		}
		else {
			TRADE_LOG("OrderInsert [%s] Direction==[%c] OpenClose==[%c] LimitPrice==[%f], 无行情!\r\n", 
				order.InstrumentID, order.Direction, order.CombOffsetFlag[0], 
				order.LimitPrice);
		}
		// 非市价一律当作限价处理
		if(itQuot != m_mapQuot.end() && !IsInvalidValue(itQuot->second.LastPrice) && 
				((order.Direction==THOST_FTDC_D_Buy && itQuot->second.LastPrice<=order.LimitPrice)
				|| (order.Direction==THOST_FTDC_D_Sell && itQuot->second.LastPrice>=order.LimitPrice))) {
			CThostFtdcTradeField trade = {0};
			ConvOrder2Trade(order, trade);
			MarkTrade(trade, itQuot->second.LastPrice);
			order.VolumeTotal = 0;
			order.VolumeTraded = order.VolumeTotalOriginal;
			order.OrderStatus = THOST_FTDC_OST_AllTraded;
			strncpy(order.StatusMsg, "全部成交报单已提交", sizeof(order.StatusMsg)-1);
			sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			m_mapOrder.erase(strOrderKey);
			itIID = m_mapInst2Order.find(order.InstrumentID);
			if(itIID != m_mapInst2Order.end()) {
				itIID->second.erase(strOrderKey);
			}
			vecOrder.push_back(order);
			vecTrade.push_back(trade);
		}
		else {
			// 无行情，设置为未成交单
			order.VolumeTotal = order.VolumeTotalOriginal;
			order.VolumeTraded = 0;
			order.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
				strncpy(order.StatusMsg, "报单已提交", sizeof(order.StatusMsg)-1);
			sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			m_mapOrder[strOrderKey] = order;
			vecOrder.push_back(order);
		}
	}

	UnlockObject();

	SaveDataToIni();
	
	return 0;
}

//撤单接口
int COrderCenter::CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
							  std::vector<CThostFtdcOrderField>& vecOrder)
{
	MAPORDER::iterator it;
	MAPINSTID2ORDER::iterator itIID;
	char strOrderKey[256] = {0};

	sprintf_s(strOrderKey, sizeof(strOrderKey)-1, 
			"%s,%s,%s,%d,%d", action.BrokerID, action.InvestorID, 
			action.OrderRef, action.FrontID, action.SessionID); 

	LockObject();

	it = m_mapOrder.find(strOrderKey);
	if(it == m_mapOrder.end()) {
		UnlockObject();
		return 1;
	}

	CThostFtdcOrderField& order = it->second;
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);

	vecOrder.push_back(order);
	order.OrderStatus = THOST_FTDC_OST_Canceled;
	strncpy(order.StatusMsg, "撤单", sizeof(order.StatusMsg)-1);
	sprintf_s(order.CancelTime, sizeof(order.CancelTime), "%02d:%02d:%02d", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(order.UpdateTime, order.CancelTime, sizeof(order.UpdateTime));
	vecOrder.push_back(order);
	m_mapOrder.erase(strOrderKey);
	itIID = m_mapInst2Order.find(order.InstrumentID);
	if(itIID != m_mapInst2Order.end()) {
		itIID->second.erase(strOrderKey);
	}

	UnlockObject();

	return 0;
}

//行情接口
int COrderCenter::Quot(const PlatformStru_DepthMarketData& quot, 
					   std::vector<CThostFtdcOrderField>& vecOrder, 
					   std::vector<CThostFtdcTradeField>& vecTrade)
{
	if(IsInvalidValue(quot.LastPrice))
		return 2;

	MAPINSTID2ORDER::iterator it;
	MAPINSTID2ORDER::iterator itIID;
	MAPORDER::iterator itOrder;
	std::set<std::string>::iterator itKey;
	std::map<std::string, std::string> mapForDelete;
	std::map<std::string, std::string>::iterator itDel;
	BOOL bHasTrade = FALSE;
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);

	LockObject();

	m_mapQuot[quot.InstrumentID] = quot;

	it = m_mapInst2Order.find(quot.InstrumentID);
	if(it == m_mapInst2Order.end()) {
		UnlockObject();
		return 0;
	}

	for(itKey = it->second.begin(); itKey != it->second.end(); itKey++) {
		itOrder = m_mapOrder.find(*itKey);
		if(itOrder != m_mapOrder.end()) {
			CThostFtdcOrderField& order = itOrder->second;
			// 非市价一律当作限价处理
			if((order.Direction==THOST_FTDC_D_Buy && quot.LastPrice<=order.LimitPrice)
					|| (order.Direction==THOST_FTDC_D_Sell && quot.LastPrice>=order.LimitPrice)) {
				CThostFtdcTradeField trade = {0};
				ConvOrder2Trade(order, trade);
				MarkTrade(trade, quot.LastPrice);
				order.VolumeTotal = 0;
				order.VolumeTraded = order.VolumeTotalOriginal;
				order.OrderStatus = THOST_FTDC_OST_AllTraded;
				strncpy(order.StatusMsg, "全部成交报单已提交", sizeof(order.StatusMsg)-1);
				sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
						sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				vecOrder.push_back(order);
				mapForDelete.insert(std::make_pair(*itKey, order.InstrumentID));
				m_mapOrder.erase(itOrder);
				vecTrade.push_back(trade);
				bHasTrade = TRUE;
			}
		}
	}
	for(itDel = mapForDelete.begin(); itDel != mapForDelete.end(); itDel++) {
		itIID = m_mapInst2Order.find(itDel->second);
		if(itIID != m_mapInst2Order.end()) {
			itIID->second.erase(itDel->first);
		}
	}
	UnlockObject();

	if(bHasTrade)
		SaveDataToIni();

	return 0;
}

bool COrderCenter::GetUserSession(const std::string& strBrokerID, 
								  const std::string& strDealerID, 
								  int& nFrontID, int& nSessionID)
{
	std::string strUserKey = strBrokerID;
	MAPUSERSESSION::iterator it;

	strUserKey += ",";
	strUserKey += strDealerID;
	it = m_mapUserSession.find(strUserKey);
	if(it == m_mapUserSession.end())
		return false;

	nFrontID = it->second.FrontID;
	nSessionID = it->second.SessionID;

	return true;
}

bool COrderCenter::MarkOrderBase(CThostFtdcOrderField& order)
{
	int nSeqID = GetNextSeqID();
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	//sprintf_s(order.OrderLocalID, sizeof(order.OrderLocalID)-1, "%d", nOrderID);
	order.OrderSubmitStatus = THOST_FTDC_OSS_InsertSubmitted;
	order.NotifySequence = nSeqID;
	strncpy(order.TradingDay, m_strTradingDay.c_str(), sizeof(order.TradingDay)-1);
	//sprintf_s(order.OrderSysID, sizeof(order.OrderSysID)-1, "%d", nOrderID);
	order.OrderSource = THOST_FTDC_OSRC_Participant;
	order.OrderStatus = THOST_FTDC_OST_Unknown;
	order.OrderType = THOST_FTDC_ORDT_Normal;
	sprintf_s(order.InsertDate, sizeof(order.InsertDate), "%04d%02d%02d", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	sprintf_s(order.InsertTime, sizeof(order.InsertTime), "%02d:%02d:%02d", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	order.SequenceNo = nSeqID;
	strncpy(order.UserProductInfo, "cifco-simu", sizeof(order.UserProductInfo)-1);
	order.BrokerOrderSeq = nSeqID;

	return true;
}

bool COrderCenter::MarkOrderSys(CThostFtdcOrderField& order)
{
	int nOrderID = GetNextOrderID();
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	sprintf_s(order.OrderLocalID, sizeof(order.OrderLocalID)-1, "%d", nOrderID);
	//order.OrderSubmitStatus = THOST_FTDC_OSS_InsertSubmitted;
	//order.NotifySequence = nOrderID;
	//strncpy(order.TradingDay, m_strTradingDay.c_str(), sizeof(order.TradingDay)-1);
	sprintf_s(order.OrderSysID, sizeof(order.OrderSysID)-1, "%d", nOrderID);
	//order.OrderSource = THOST_FTDC_OSRC_Participant;
	//order.OrderStatus = THOST_FTDC_OST_Unknown;
	//order.OrderType = THOST_FTDC_ORDT_Normal;
	//sprintf_s(order.InsertDate, sizeof(order.InsertDate), "%04d%02d%02d", 
	//		sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	//sprintf_s(order.InsertTime, sizeof(order.InsertTime), "%02d:%02d:%02d", 
	//		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	//order.SequenceNo = nOrderID;
	//strncpy(order.UserProductInfo, "cifco-simu", sizeof(order.UserProductInfo)-1);
	//order.BrokerOrderSeq = nOrderID;

	return true;
}

bool COrderCenter::MarkTrade(CThostFtdcTradeField& trade, double fPrice)
{
	int nTradeID = GetNextTradeID();
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	sprintf_s(trade.TradeID, sizeof(trade.TradeID)-1, "%d", nTradeID);
	trade.TradingRole = THOST_FTDC_ER_Broker;
	trade.Price = fPrice;
	sprintf_s(trade.TradeDate, sizeof(trade.TradeDate), "%04d%02d%02d", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	sprintf_s(trade.TradeTime, sizeof(trade.TradeTime), "%02d:%02d:%02d", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	trade.TradeType = THOST_FTDC_TRDT_Common;
	trade.PriceSource = THOST_FTDC_PSRC_LastPrice;
	strncpy(trade.BusinessUnit, "1", sizeof(trade.BusinessUnit)-1);
	trade.TradeSource = THOST_FTDC_TSRC_NORMAL;

	return true;
}

void COrderCenter::SaveDataToIni()
{
	int nPathLen = 0, nRet = 0;
	char localPath[256];
	char csText[1024] = {0};
	memset(localPath, 0, 256);
	nPathLen = GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	string strPath(filename.substr(0, splitpos+1).c_str());
	strPath += "SvrSimulateTrade.ini";

	sprintf_s(csText, sizeof(csText)-1, "%s", m_strTradingDay.c_str());
	::WritePrivateProfileString("save", "tradingday", csText, strPath.c_str());
	sprintf_s(csText, sizeof(csText)-1, "%d", m_nSeqID);
	::WritePrivateProfileString("save", "seqid", csText, strPath.c_str());
	sprintf_s(csText, sizeof(csText)-1, "%d", m_nOrderID);
	::WritePrivateProfileString("save", "orderid", csText, strPath.c_str());
	sprintf_s(csText, sizeof(csText)-1, "%d", m_nTradeID);
	::WritePrivateProfileString("save", "tradeid", csText, strPath.c_str());
}
