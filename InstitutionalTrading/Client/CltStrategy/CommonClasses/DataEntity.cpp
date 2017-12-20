
#include "stdafx.h"
#include "DataEntity.h"
#include "Tools.h"

CDataEntity::CDataEntity()
{
	m_bSaveData = false;
	memset(&m_testIndicator,0,sizeof(m_testIndicator));
	m_testIndicator.TotalTradingTime =1;
}

CDataEntity::~CDataEntity()
{
}

bool CDataEntity::LoadAll()
{
	LoadStrategy();
	LoadInstrument();
	LoadMDQuotList();
	LoadMarginRate();
	LoadCommissionRate();
	LoadTradingAccount();
	LoadPosition();
	LoadOrder();
	LoadTrade();
	LoadTestIndicator();
	return true;
}

bool CDataEntity::SaveAll()
{
	SaveStrategy();
	SaveInstrument();
	SaveMDQuotList();
	SaveMarginRate();
	SaveCommissionRate();
	SaveTradingAccount();
	SavePosition();
	SaveOrder();
	SaveTrade();
	//计算指标
	CaculateIndicator();
	SaveTestIndicator();

	TestSimpleData data;
	memset(&data, 0, sizeof(TestSimpleData));
	LoadSimple(data);
	return true;
}

bool CDataEntity::LoadSimple(TestSimpleData& data)
{
	if(LoadStrategy() && LoadTestIndicator()) {
		int nstrlen = m_strSavePath.size();
		char strDateTime[16];
		char strDate[16];
		char strTime[16];
		memset(strDateTime, 0, sizeof(strDateTime));
		memset(strDate, 0, sizeof(strDate));
		memset(strTime, 0, sizeof(strTime));
		strncpy(strDateTime, m_strSavePath.c_str()+nstrlen-15-1, sizeof(strDateTime)-1);
		sscanf(strDateTime, "%[^\\]\\%s", strDate, strTime);
		strncpy(data.szName, m_strategy.GetStrategyNickName().c_str(), sizeof(data.szName)-1);

		strncpy(data.szDate, strDate, 4);
		strcat(data.szDate, "-");
		strncat(data.szDate, strDate+4, 2);
		strcat(data.szDate, "-");
		strncat(data.szDate, strDate+6, 2);

		strncpy(data.szTime, strTime, 2);
		strcat(data.szTime, ":");
		strncat(data.szTime, strTime+2, 2);
		strcat(data.szTime, ":");
		strncat(data.szTime, strTime+4, 2);

		data.NetProfit = m_testIndicator.NetProfit;
		data.TotalTradeCnt = m_testIndicator.TotalTradeCnt;
		return true;
	}
	else
		return false;
}

bool CDataEntity::SaveStrategy()
{
	std::string strFile = m_strSavePath + "strategy.xml";
	return m_strategy.MakeXmlToFile(Tools::string2String(strFile.c_str()));
}

bool CDataEntity::LoadStrategy()
{
	std::string strFile = m_strSavePath + "strategy.xml";
	return m_strategy.ImportFromXmlFile(strFile);
}



bool CDataEntity::AppendQuot(const PlatformStru_DepthMarketData& quot)
{
	if(m_bSaveData)
		m_lstQuot.push_back(quot);
	return true;
}

void CDataEntity::ClearQuotList()
{
}

bool CDataEntity::SaveMDQuotList()
{
	if(!m_bSaveData)
		return true;


	return true;
}

bool CDataEntity::LoadMDQuotList()
{
	return true;
}

bool CDataEntity::SaveMarginRate()
{
	return true;
}

bool CDataEntity::LoadMarginRate()
{
	return true;
}

bool CDataEntity::SaveCommissionRate()
{
	return true;
}

bool CDataEntity::LoadCommissionRate()
{
	return true;
}
bool CDataEntity::SaveInstrument()
{
	std::string strFile = m_strSavePath + "Instrument.csv";

	int nLine=1;


	std::map<u32, std::map<u32, std::string> > MapFile;
	std::map<u32, std::string> StringMap;
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrument.begin();
	for ( int nline =1 ; it!=m_mapInstrument.end(); it++ ,nline++)
	{
		int nRow=0;
		PlatformStru_InstrumentInfo data = it->second;
		StringMap[nRow++] = data.InstrumentID;
		StringMap[nRow++] = data.ExchangeID;
		StringMap[nRow++] = data.InstrumentName;
		StringMap[nRow++] = data.ExchangeInstID;
		StringMap[nRow++] = data.ProductID;
		StringMap[nRow++] = data.ProductClass;
		StringMap[nRow++] = Int2String(data.DeliveryYear);
		StringMap[nRow++] = Int2String(data.DeliveryMonth);
		StringMap[nRow++] = Int2String(data.MaxMarketOrderVolume);
		StringMap[nRow++] = Int2String(data.MinMarketOrderVolume);
		StringMap[nRow++] = Int2String(data.MaxLimitOrderVolume);
		StringMap[nRow++] = Int2String(data.MinLimitOrderVolume);
		StringMap[nRow++] = Int2String(data.VolumeMultiple);
        StringMap[nRow++] = Double2String(data.PriceTick);
		StringMap[nRow++] = data.CreateDate;
		StringMap[nRow++] = data.OpenDate;
		StringMap[nRow++] = data.ExpireDate;
		StringMap[nRow++] = data.StartDelivDate;
		StringMap[nRow++] = data.EndDelivDate;
		StringMap[nRow++] = data.InstLifePhase;
		StringMap[nRow++] = Int2String(data.IsTrading);
		StringMap[nRow++] = data.PositionType;
		StringMap[nRow++] = data.PositionDateType;	
		StringMap[nRow++] = Double2String(data.LongMarginRatio);
		StringMap[nRow++] = Double2String(data.ShortMarginRatio);
		

		MapFile[nLine] = StringMap;
		nLine++;

	}
	CCSVOperator            csvOprater;
	csvOprater.SetStringKeyMap(MapFile);	
	if (csvOprater.SaveCSV(strFile.c_str()))
	{
		return true;
	}
	else
		return false;
}
bool CDataEntity::LoadInstrument()
{
	std::string strFile = m_strSavePath + "Instrument.csv";
	CCSVOperator   csvOprater;
	if(csvOprater.LoadCSV(strFile.c_str()))
	{
		std::map<u32, std::map<u32, std::string> > MapFile;
		MapFile =csvOprater.GetCSVMap();
		std::map<u32, std::map<u32, std::string> >::iterator it = MapFile.begin();
		for ( it; it!=MapFile.end(); it++)
		{
			PlatformStru_InstrumentInfo data;
			int i = 1;
			strncpy(data.InstrumentID, it->second[i++].c_str(),sizeof(data.InstrumentID));	
			strncpy(data.ExchangeID, it->second[i++].c_str(),sizeof(data.ExchangeID));	
			strncpy(data.InstrumentName, it->second[i++].c_str(),sizeof(data.InstrumentName));	
			strncpy(data.ExchangeInstID, it->second[i++].c_str(),sizeof(data.ExchangeInstID));	
			strncpy(data.ProductID, it->second[i++].c_str(),sizeof(data.ProductID));		
			//data.ProductClass= it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.ProductClass);
			data.DeliveryYear = atoi(it->second[i++].c_str());
			data.DeliveryMonth = atoi(it->second[i++].c_str());
			data.MaxMarketOrderVolume = atoi(it->second[i++].c_str());
			data.MinMarketOrderVolume = atoi(it->second[i++].c_str());
			data.MaxLimitOrderVolume = atoi(it->second[i++].c_str());
			data.MinLimitOrderVolume = atoi(it->second[i++].c_str());
			data.VolumeMultiple = atoi(it->second[i++].c_str());
			data.PriceTick = atof(it->second[i++].c_str());
			strncpy(data.CreateDate, it->second[i++].c_str(),sizeof(data.CreateDate));	
			strncpy(data.OpenDate, it->second[i++].c_str(),sizeof(data.OpenDate));	
			strncpy(data.ExpireDate, it->second[i++].c_str(),sizeof(data.ExpireDate));	
			strncpy(data.StartDelivDate, it->second[i++].c_str(),sizeof(data.StartDelivDate));	
			strncpy(data.EndDelivDate, it->second[i++].c_str(),sizeof(data.EndDelivDate));	
			//data.InstLifePhase= it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.InstLifePhase);
			data.IsTrading = atoi(it->second[i++].c_str());
			//data.PositionType= it->second[i++].at(0);
			//data.PositionDateType= it->second[i++].at(0);		
			sscanf(it->second[i++].c_str(),"%c",&data.PositionType);
			sscanf(it->second[i++].c_str(),"%c",&data.PositionDateType);
			data.LongMarginRatio = atof(it->second[i++].c_str());
			data.ShortMarginRatio = atof(it->second[i++].c_str());
		

			m_mapInstrument[data.InstrumentID]= data;

		}

		return true;

	}


	else
		return false;
}
bool CDataEntity::AppendTradingAccount(const PlatformStru_TradingAccountInfo& tradingAccnt)
{
	m_lstTradingAccnt.push_back(tradingAccnt);
	return true;
}

void CDataEntity::ClearTradingAccountList()
{
	m_lstTradingAccnt.clear();
}

bool CDataEntity::SaveTradingAccount()
{
	std::string strFile = m_strSavePath + "TradingAccount.csv";

	int nLine=1;


	std::map<u32, std::map<u32, std::string> > MapFile;
	std::map<u32, std::string> StringMap;
	std::list<PlatformStru_TradingAccountInfo>::iterator it = m_lstTradingAccnt.begin();
	for ( int nline =1 ; it!=m_lstTradingAccnt.end(); it++ ,nline++)
	{
		int nRow=0;
		PlatformStru_TradingAccountInfo data = *it;
		StringMap[nRow++] = data.BrokerID;
		StringMap[nRow++] = data.AccountID;
		StringMap[nRow++] = Double2String(data.PreMortgage);
		StringMap[nRow++] = Double2String(data.PreCredit);
		StringMap[nRow++] = Double2String(data.PreDeposit);
		StringMap[nRow++] = Double2String(data.PreBalance);
		StringMap[nRow++] = Double2String(data.PreMargin);
		StringMap[nRow++] = Double2String(data.InterestBase );
		StringMap[nRow++] = Double2String(data.Interest );
		StringMap[nRow++] = Double2String(data.Deposit);
		StringMap[nRow++] = Double2String(data.Withdraw);
		StringMap[nRow++] = Double2String(data.FrozenMargin);
		StringMap[nRow++] = Double2String(data.FrozenCash );
		StringMap[nRow++] = Double2String(data.FrozenCommission );
		StringMap[nRow++] = Double2String(data.CurrMargin);
		StringMap[nRow++] = Double2String(data.CashIn);
		StringMap[nRow++] = Double2String(data.Commission);
		StringMap[nRow++] = Double2String(data.CloseProfit );
		StringMap[nRow++] = Double2String(data.PositionProfit );
		StringMap[nRow++] = Double2String(data.Balance);
		StringMap[nRow++] = Double2String(data.Available);
		StringMap[nRow++] = Double2String(data.WithdrawQuota);
		StringMap[nRow++] = Double2String(data.Reserve );
		StringMap[nRow++] = data.TradingDay;
		StringMap[nRow++] = Int2String(data.SettlementID);
		StringMap[nRow++] = Double2String(data.Credit);
		StringMap[nRow++] = Double2String(data.Mortgage);
		StringMap[nRow++] = Double2String(data.ExchangeMargin );
		StringMap[nRow++] = Double2String(data.DeliveryMargin );
		StringMap[nRow++] = Double2String(data.ExchangeDeliveryMargin);
		StringMap[nRow++] = Double2String(data.StaticProfit);
		StringMap[nRow++] = Double2String(data.DynamicProfit);
		StringMap[nRow++] = Double2String(data.RiskDegree);
		StringMap[nRow++] = data.Account;//用来保存时间

		MapFile[nLine] = StringMap;
		nLine++;

	}
	CCSVOperator            csvOprater;
	csvOprater.SetStringKeyMap(MapFile);	
	if (csvOprater.SaveCSV(strFile.c_str()))
	{
		return true;
	}
	else
		return false;

}

bool CDataEntity::LoadTradingAccount()
{
	std::string strFile = m_strSavePath + "TradingAccount.csv";
	CCSVOperator   csvOprater;
	if(csvOprater.LoadCSV(strFile.c_str()))
	{
		std::map<u32, std::map<u32, std::string> > MapFile;
		MapFile =csvOprater.GetCSVMap();
		std::map<u32, std::map<u32, std::string> >::iterator it = MapFile.begin();
		for ( it; it!=MapFile.end(); it++)
		{
			PlatformStru_TradingAccountInfo data;
			int i = 1;
			strncpy(data.BrokerID, it->second[i++].c_str(),sizeof(data.BrokerID));	
			strncpy(data.AccountID, it->second[i++].c_str(),sizeof(data.AccountID));	
			
			data.PreMortgage = atof(it->second[i++].c_str());
			data.PreCredit = atof(it->second[i++].c_str());
			data.PreDeposit = atof(it->second[i++].c_str());
			data.PreBalance = atof(it->second[i++].c_str());
			data.PreMargin = atof(it->second[i++].c_str());
			data.InterestBase = atof(it->second[i++].c_str());
			data.Interest = atof(it->second[i++].c_str());
			data.Deposit = atof(it->second[i++].c_str());
			data.Withdraw = atof(it->second[i++].c_str());
			data.FrozenMargin = atof(it->second[i++].c_str());
			data.FrozenCash = atof(it->second[i++].c_str());
			data.FrozenCommission = atof(it->second[i++].c_str());
			data.CurrMargin = atof(it->second[i++].c_str());
			data.CashIn = atof(it->second[i++].c_str());
			data.Commission = atof(it->second[i++].c_str());
			data.CloseProfit = atof(it->second[i++].c_str());
			data.PositionProfit = atof(it->second[i++].c_str());
			data.Balance = atof(it->second[i++].c_str());
			data.Available = atof(it->second[i++].c_str());
			data.WithdrawQuota = atof(it->second[i++].c_str());
			data.Reserve = atof(it->second[i++].c_str());
			strncpy(data.TradingDay, it->second[i++].c_str(),sizeof(data.TradingDay));	
			data.SettlementID = atoi(it->second[i++].c_str());
			data.Credit = atof(it->second[i++].c_str());
			data.Mortgage = atof(it->second[i++].c_str());
			data.ExchangeMargin = atof(it->second[i++].c_str());
			data.DeliveryMargin = atof(it->second[i++].c_str());
			data.ExchangeDeliveryMargin = atof(it->second[i++].c_str());
			data.StaticProfit = atof(it->second[i++].c_str());
			data.DynamicProfit = atof(it->second[i++].c_str());
			data.RiskDegree = atof(it->second[i++].c_str());
			strncpy(data.Account, it->second[i++].c_str(),sizeof(data.Account));	//用来保存时间



			m_lstTradingAccnt.push_back(data);

		}

		return true;

	}


	else
		return false;
}

bool CDataEntity::AppendPosition(const PlatformStru_Position& position)
{
	return true;
}

void CDataEntity::ClearPositionList()
{
}

bool CDataEntity::SavePosition()
{
	return true;
}

bool CDataEntity::LoadPosition()
{
	return true;
}

bool CDataEntity::AppendOrder(const PlatformStru_OrderInfo& order)
{
	m_lstOrder.push_back(order);
	return true;
}

void CDataEntity::ClearOrderList()
{
	m_lstOrder.clear();
}

bool CDataEntity::SaveOrder()
{
	std::string strFile = m_strSavePath + "Order.csv";

	int nLine=1;


	std::map<u32, std::map<u32, std::string> > MapFile;
	std::map<u32, std::string> StringMap;
	std::list<PlatformStru_OrderInfo>::iterator it = m_lstOrder.begin();
	for ( int nline =1 ; it!=m_lstOrder.end(); it++ ,nline++)
	{
		int nRow=0;
		PlatformStru_OrderInfo data = *it;
		StringMap[nRow++] = data.BrokerID;
		StringMap[nRow++] = data.InvestorID;
		StringMap[nRow++] = data.InstrumentID;
		StringMap[nRow++] = data.OrderRef;
		StringMap[nRow++] = data.UserID;
		StringMap[nRow++] = data.OrderPriceType;
		StringMap[nRow++] = data.Direction;
		StringMap[nRow++] = data.CombOffsetFlag;
		StringMap[nRow++] = data.CombHedgeFlag;
		StringMap[nRow++] = Double2String(data.LimitPrice);
		StringMap[nRow++] = Int2String(data.VolumeTotalOriginal );
        StringMap[nRow++] = data.TimeCondition;
		StringMap[nRow++] = data.GTDDate ;
	    StringMap[nRow++] = data.VolumeCondition;
		StringMap[nRow++] = Int2String(data.MinVolume);
		StringMap[nRow++] = data.ContingentCondition;
		StringMap[nRow++] = Double2String(data.StopPrice);
        StringMap[nRow++] = data.ForceCloseReason;
		StringMap[nRow++] = Int2String(data.IsAutoSuspend);
        StringMap[nRow++] = data.BusinessUnit;
		StringMap[nRow++] = Int2String(data.RequestID);
		StringMap[nRow++] = data.OrderLocalID;
		StringMap[nRow++] = data.ExchangeID;
		StringMap[nRow++] = data.ParticipantID;
		StringMap[nRow++] = data.ClientID;
		StringMap[nRow++] = data.ExchangeInstID;
		StringMap[nRow++] = data.TraderID;
		StringMap[nRow++] = Int2String(data.InstallID);
		StringMap[nRow++] = data.OrderSubmitStatus;
		StringMap[nRow++] = Int2String(data.NotifySequence);
		StringMap[nRow++] = data.TradingDay;
		StringMap[nRow++] = Int2String(data.SettlementID);	
		StringMap[nRow++] = data.OrderSysID;
		StringMap[nRow++] = data.OrderSource;
		StringMap[nRow++] = data.OrderStatus;
		StringMap[nRow++] = data.OrderType;
		StringMap[nRow++] = Int2String(data.VolumeTraded);
		StringMap[nRow++] = Int2String(data.VolumeTotal);
		StringMap[nRow++] = data.InsertDate;
		StringMap[nRow++] = data.InsertTime;
		StringMap[nRow++] = data.ActiveTime;
		StringMap[nRow++] = data.SuspendTime;
		StringMap[nRow++] = data.UpdateTime;
		StringMap[nRow++] = data.CancelTime;
		StringMap[nRow++] = data.ActiveTraderID;
		StringMap[nRow++] = data.ClearingPartID;
		StringMap[nRow++] = Int2String(data.SequenceNo);
		StringMap[nRow++] = Int2String(data.FrontID);
		StringMap[nRow++] = Int2String(data.SessionID);
		StringMap[nRow++] = data.UserProductInfo;
		StringMap[nRow++] = data.StatusMsg;
		StringMap[nRow++] = Int2String(data.UserForceClose);
		StringMap[nRow++] = data.ActiveUserID;
		StringMap[nRow++] = Int2String(data.BrokerOrderSeq);
		StringMap[nRow++] = data.RelativeOrderSysID;
		StringMap[nRow++] = Double2String(data.AvgPrice);
		StringMap[nRow++] = long2String(data.FTID );
		StringMap[nRow++] = long2String(data.UpdateSeq );

		MapFile[nLine] = StringMap;
		nLine++;

	}
	CCSVOperator            csvOprater;
	csvOprater.SetStringKeyMap(MapFile);	
	if (csvOprater.SaveCSV(strFile.c_str()))
	{
		return true;
	}
	else
		return false;
}

bool CDataEntity::LoadOrder()
{
	std::string strFile = m_strSavePath + "Order.csv";
	CCSVOperator   csvOprater;
	if(csvOprater.LoadCSV(strFile.c_str()))
	{
		std::map<u32, std::map<u32, std::string> > MapFile;
		MapFile =csvOprater.GetCSVMap();
		std::map<u32, std::map<u32, std::string> >::iterator it = MapFile.begin();
		for ( it; it!=MapFile.end(); it++)
		{
			PlatformStru_OrderInfo data;
			int i = 1;
			strncpy(data.BrokerID, it->second[i++].c_str(),sizeof(data.BrokerID));	
			strncpy(data.InvestorID, it->second[i++].c_str(),sizeof(data.InvestorID));	
			strncpy(data.InstrumentID, it->second[i++].c_str(),sizeof(data.InstrumentID));	
			strncpy(data.OrderRef, it->second[i++].c_str(),sizeof(data.OrderRef));	
			strncpy(data.UserID, it->second[i++].c_str(),sizeof(data.UserID)); 
			sscanf(it->second[i++].c_str(),"%c",&data.OrderPriceType);
			sscanf(it->second[i++].c_str(),"%c",&data.Direction);
			//*\\*/data.OrderPriceType = it->second[i++].at(0);
			//*\\*/data.Direction = it->second[i++].at(0);
			strncpy(data.CombOffsetFlag, it->second[i++].c_str(),sizeof(data.CombOffsetFlag)); 	
			strncpy(data.CombHedgeFlag, it->second[i++].c_str(),sizeof(data.CombHedgeFlag)); 
			data.LimitPrice = atof(it->second[i++].c_str());
			data.VolumeTotalOriginal = atoi(it->second[i++].c_str());
			//data.TimeCondition = it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.TimeCondition);
			strncpy(data.GTDDate, it->second[i++].c_str(),sizeof(data.GTDDate)); 
			//data.VolumeCondition = it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.VolumeCondition);
			data.MinVolume = atoi(it->second[i++].c_str());
			//data.ContingentCondition = it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.ContingentCondition);
			data.StopPrice = atof(it->second[i++].c_str());
			//data.ForceCloseReason = it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.ForceCloseReason);
			data.IsAutoSuspend = atoi(it->second[i++].c_str());
			strncpy(data.BusinessUnit, it->second[i++].c_str(),sizeof(data.BusinessUnit)); 
			data.RequestID = atoi(it->second[i++].c_str());
			strncpy(data.OrderLocalID, it->second[i++].c_str(),sizeof(data.OrderLocalID)); 
			strncpy(data.ExchangeID, it->second[i++].c_str(),sizeof(data.ExchangeID)); 
			strncpy(data.ParticipantID, it->second[i++].c_str(),sizeof(data.ParticipantID)); 
			strncpy(data.ClientID, it->second[i++].c_str(),sizeof(data.ClientID)); 
			strncpy(data.ExchangeInstID, it->second[i++].c_str(),sizeof(data.ExchangeInstID)); 
			strncpy(data.TraderID, it->second[i++].c_str(),sizeof(data.TraderID)); 
			data.InstallID = atoi(it->second[i++].c_str());
			//data.OrderSubmitStatus = it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.OrderSubmitStatus);
			data.NotifySequence = atoi(it->second[i++].c_str());
			strncpy(data.TradingDay, it->second[i++].c_str(),sizeof(data.TradingDay)); 
			data.SettlementID = atoi(it->second[i++].c_str());
			strncpy(data.OrderSysID, it->second[i++].c_str(),sizeof(data.OrderSysID)); 
			/*data.OrderSource = it->second[i++].at(0);
			data.OrderStatus = it->second[i++].at(0);
			data.OrderType = it->second[i++].at(0);*/
			sscanf(it->second[i++].c_str(),"%c",&data.OrderSource);
			sscanf(it->second[i++].c_str(),"%c",&data.OrderStatus);
			sscanf(it->second[i++].c_str(),"%c",&data.OrderType);
			data.VolumeTraded = atoi(it->second[i++].c_str());
			data.VolumeTotal = atoi(it->second[i++].c_str());
			strncpy(data.InsertDate, it->second[i++].c_str(),sizeof(data.InsertDate)); 
			strncpy(data.InsertTime, it->second[i++].c_str(),sizeof(data.InsertTime)); 
			strncpy(data.ActiveTime, it->second[i++].c_str(),sizeof(data.ActiveTime)); 
			strncpy(data.SuspendTime, it->second[i++].c_str(),sizeof(data.SuspendTime)); 
			strncpy(data.UpdateTime, it->second[i++].c_str(),sizeof(data.UpdateTime)); 
			strncpy(data.CancelTime, it->second[i++].c_str(),sizeof(data.CancelTime)); 
			strncpy(data.ActiveTraderID, it->second[i++].c_str(),sizeof(data.ActiveTraderID)); 
			strncpy(data.ClearingPartID, it->second[i++].c_str(),sizeof(data.ClearingPartID)); 
			data.SequenceNo = atoi(it->second[i++].c_str());
			data.FrontID = atoi(it->second[i++].c_str());
			data.SessionID = atoi(it->second[i++].c_str());
			strncpy(data.UserProductInfo, it->second[i++].c_str(),sizeof(data.UserProductInfo)); 
			strncpy(data.StatusMsg, it->second[i++].c_str(),sizeof(data.StatusMsg)); 
			data.UserForceClose = atoi(it->second[i++].c_str());
			strncpy(data.ActiveUserID, it->second[i++].c_str(),sizeof(data.ActiveUserID)); 
			data.BrokerOrderSeq = atoi(it->second[i++].c_str());
			strncpy(data.RelativeOrderSysID, it->second[i++].c_str(),sizeof(data.RelativeOrderSysID)); 
			data.AvgPrice = atof(it->second[i++].c_str());
			data.FTID = atol(it->second[i++].c_str());	
			data.UpdateSeq = atol(it->second[i++].c_str());	

			m_lstOrder.push_back(data);

		}

		return true;

	}


	else
		return false;
}

bool CDataEntity::AppendTrade(const PlatformStru_TradeInfo& trade)
{
	m_lstTrade.push_back(trade);
	return true;
}

void CDataEntity::ClearTradeList()
{
	m_lstTrade.clear();
}

bool CDataEntity::SaveTrade()
{
	std::string strFile = m_strSavePath + "Trade.csv";

	int nLine=1;


	std::map<u32, std::map<u32, std::string> > MapFile;
	std::map<u32, std::string> StringMap;
	std::list<PlatformStru_TradeInfo>::iterator it = m_lstTrade.begin();
	for ( int nline =1 ; it!=m_lstTrade.end(); it++ ,nline++)
	{
		int nRow=0;
		PlatformStru_TradeInfo data = *it;
		StringMap[nRow++] = data.BrokerID;
		StringMap[nRow++] = data.InvestorID;
		StringMap[nRow++] = data.InstrumentID;
		StringMap[nRow++] = data.OrderRef;
		StringMap[nRow++] = data.UserID;
		StringMap[nRow++] = data.ExchangeID;
		StringMap[nRow++] = data.TradeID;
		StringMap[nRow++] = data.Direction;
		StringMap[nRow++] = data.OrderSysID;
		StringMap[nRow++] = data.ParticipantID;
		StringMap[nRow++] = data.ClientID;
		StringMap[nRow++] = data.TradingRole;
		StringMap[nRow++] = data.ExchangeInstID;
		StringMap[nRow++] = data.OffsetFlag;
		StringMap[nRow++] = data.HedgeFlag;
		StringMap[nRow++] = Double2String(data.Price) ;
		StringMap[nRow++] = Int2String(data.Volume);
		StringMap[nRow++] = data.TradeDate;
		StringMap[nRow++] = data.TradeTime;
		StringMap[nRow++] = data.TradeType;
		StringMap[nRow++] = data.PriceSource;
		StringMap[nRow++] = data.TraderID;
		StringMap[nRow++] = data.OrderLocalID;
		StringMap[nRow++] = data.ClearingPartID;
		StringMap[nRow++] = data.BusinessUnit;
		StringMap[nRow++] = Int2String(data.SequenceNo);
		StringMap[nRow++] = data.TradingDay;
		StringMap[nRow++] = Int2String(data.SettlementID);
		StringMap[nRow++] = Int2String(data.BrokerOrderSeq);
		StringMap[nRow++] = data.TradeSource;
		StringMap[nRow++] = Double2String(data.CloseProfitByDate);
		StringMap[nRow++] = Double2String(data.CloseProfitByTrade );
		StringMap[nRow++] = Double2String(data.TradeCommission );
		StringMap[nRow++] = long2String(data.FTID );
		StringMap[nRow++] = long2String(data.UpdateSeq );

		MapFile[nLine] = StringMap;
		nLine++;

	}
	CCSVOperator            csvOprater;
    csvOprater.SetStringKeyMap(MapFile);	
	if (csvOprater.SaveCSV(strFile.c_str()))
	{
		return true;
	}
	else
		return false;
	

	
}

bool CDataEntity::LoadTrade()
{
	std::string strFile = m_strSavePath + "Trade.csv";
	CCSVOperator   csvOprater;
	if(csvOprater.LoadCSV(strFile.c_str()))
	{
		std::map<u32, std::map<u32, std::string> > MapFile;
		MapFile =csvOprater.GetCSVMap();
		std::map<u32, std::map<u32, std::string> >::iterator it = MapFile.begin();
		for ( it; it!=MapFile.end(); it++)
		{
			PlatformStru_TradeInfo data;
			int i = 1;
			strncpy(data.BrokerID, it->second[i++].c_str(),sizeof(data.BrokerID));	
			strncpy(data.InvestorID, it->second[i++].c_str(),sizeof(data.InvestorID));	
			strncpy(data.InstrumentID, it->second[i++].c_str(),sizeof(data.InstrumentID));	
			strncpy(data.OrderRef, it->second[i++].c_str(),sizeof(data.OrderRef));	
			strncpy(data.UserID, it->second[i++].c_str(),sizeof(data.UserID)); 				
			strncpy(data.ExchangeID, it->second[i++].c_str(),sizeof(data.ExchangeID));	
			strncpy(data.TradeID, it->second[i++].c_str(),sizeof(data.TradeID));
			//data.Direction = it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.Direction);
			strncpy(data.OrderSysID, it->second[i++].c_str(),sizeof(data.OrderSysID));
			strncpy(data.ParticipantID, it->second[i++].c_str(),sizeof(data.ParticipantID));
			strncpy(data.ClientID, it->second[i++].c_str(),sizeof(data.ClientID));
			//data.TradingRole= it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.TradingRole);
			strncpy(data.ExchangeInstID, it->second[i++].c_str(),sizeof(data.ExchangeInstID));
			//data.OffsetFlag= it->second[i++].at(0);
			//data.HedgeFlag= it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.OffsetFlag);
			sscanf(it->second[i++].c_str(),"%c",&data.HedgeFlag);
			data.Price = atof(it->second[i++].c_str());
			data.Volume = atoi(it->second[i++].c_str());
			strncpy(data.TradeDate, it->second[i++].c_str(),sizeof(data.TradeDate));
			strncpy(data.TradeTime, it->second[i++].c_str(),sizeof(data.TradeTime));
			//data.TradeType= it->second[i++].at(0);
			//data.PriceSource= it->second[i++].at(0);
			sscanf(it->second[i++].c_str(),"%c",&data.TradeType);
			sscanf(it->second[i++].c_str(),"%c",&data.PriceSource);
			strncpy(data.TraderID, it->second[i++].c_str(),sizeof(data.TraderID));
			strncpy(data.OrderLocalID, it->second[i++].c_str(),sizeof(data.OrderLocalID));
			strncpy(data.ClearingPartID, it->second[i++].c_str(),sizeof(data.ClearingPartID));
			strncpy(data.BusinessUnit, it->second[i++].c_str(),sizeof(data.BusinessUnit));
			data.SequenceNo = atoi(it->second[i++].c_str());
			strncpy(data.TradingDay, it->second[i++].c_str(),sizeof(data.TradingDay));
			data.SettlementID = atoi(it->second[i++].c_str());
			data.BrokerOrderSeq = atoi(it->second[i++].c_str());
			//data.TradeSource= it->second[i++].at(0);	
			sscanf(it->second[i++].c_str(),"%c",&data.TradeSource);
			data.CloseProfitByDate = atof(it->second[i++].c_str());
			data.CloseProfitByTrade = atof(it->second[i++].c_str());
			data.TradeCommission = atof(it->second[i++].c_str());
			data.FTID = atol(it->second[i++].c_str());	
			data.UpdateSeq =atol( it->second[i++].c_str());	

			m_lstTrade.push_back(data);

		}

		return true;

	}


	else
		return false;
}

void CDataEntity::CaculateIndicator()
{
	int nMaxContinuousLoss = 0;
	int nMaxContinuousProfit = 0;
	
	std::list<PlatformStru_TradeInfo>::iterator it =m_lstTrade.begin();
	for (  ; it!=m_lstTrade.end(); it++ )
	{
		PlatformStru_TradeInfo trade = *it;
		m_testIndicator.TotalOrderCnt++;

		if ( trade.OffsetFlag ==THOST_FTDC_OF_Close || trade.OffsetFlag ==THOST_FTDC_OF_CloseToday
			|| trade.OffsetFlag ==THOST_FTDC_OF_CloseYesterday )
		{
			m_testIndicator.TotalTradeCnt++;
			m_testIndicator.TotalCommission  += trade.TradeCommission;
			if ( trade.CloseProfitByTrade >=0 )
			{
				m_testIndicator.TotalProfit += trade.CloseProfitByTrade;
				m_testIndicator.TotalProfitCnt++;
				if (trade.CloseProfitByTrade > m_testIndicator.MaxProfit)
				{
					m_testIndicator.MaxProfit = trade.CloseProfitByTrade;
				}

				nMaxContinuousProfit++;
				if (!m_testIndicator.bIsProfit)
				{
					if (m_testIndicator.MaxContinuousLoss< nMaxContinuousLoss)
					{
						m_testIndicator.MaxContinuousLoss =nMaxContinuousLoss;
					}

					nMaxContinuousLoss = 0;
				}

				m_testIndicator.bIsProfit = true;


			}
			else
			{
				m_testIndicator.TotalLoss += trade.CloseProfitByTrade;
				m_testIndicator.TotalLossCnt++;
				if (trade.CloseProfitByTrade < m_testIndicator.MaxLoss)
				{
					m_testIndicator.MaxLoss = trade.CloseProfitByTrade;
				}
				nMaxContinuousLoss++;
				if ( m_testIndicator.bIsProfit )
				{
					if (m_testIndicator.MaxContinuousProfit< nMaxContinuousProfit)
					{
						m_testIndicator.MaxContinuousProfit =nMaxContinuousProfit;
					}

					nMaxContinuousProfit = 0;

				}

				m_testIndicator.bIsProfit = false;


			}
		}


	}
	m_testIndicator.NetProfit = m_testIndicator.TotalProfit + m_testIndicator.TotalLoss;//净利润：总盈利+总亏损
	if (m_testIndicator.TotalProfitCnt!=0)
	{
		m_testIndicator.AvgProfit = m_testIndicator.TotalProfit / m_testIndicator.TotalProfitCnt;//2.	平均盈利：总盈利/总盈利次数 （平仓盈亏）
	}
	if ( m_testIndicator.TotalLossCnt!=0)
	{
		m_testIndicator.AvgLoss = m_testIndicator.TotalLoss / m_testIndicator.TotalLossCnt;//8.	平均亏损：总亏损/亏损次数
	}

	m_testIndicator.GrossProfit = m_testIndicator.NetProfit + m_testIndicator.TotalCommission;//12.	毛利润：净利润+手续费
	if (m_testIndicator.TotalTradeCnt!=0)
	{
		m_testIndicator.AvgNetProfit = m_testIndicator.NetProfit /m_testIndicator.TotalTradeCnt;//13.	平均利润：净利润/总交易次数
		m_testIndicator.WinRate = (double)m_testIndicator.TotalProfitCnt / m_testIndicator.TotalTradeCnt;//19.	胜率：盈利次数/总交易次数
		m_testIndicator.ConfidenceLevel = 1-1/sqrt((double)m_testIndicator.TotalTradeCnt);//38.	置信度：1—1/sqrt（交易次数）
	}

	if (m_testIndicator.InitialProfit != 0)
	{
		m_testIndicator.YieldRate = m_testIndicator.NetProfit / m_testIndicator.InitialProfit;//20.	收益率%：净利润/初始资金

	}




	//std::list<PlatformStru_OrderInfo>::iterator it =m_lstOrder.begin();
	//for (  ; it!=m_lstOrder.end(); it++ )
	//{
	//}

	std::list<PlatformStru_TradingAccountInfo>::iterator itFund =m_lstTradingAccnt.begin();
	for (  ; itFund!=m_lstTradingAccnt.end(); itFund++ )
	{
		PlatformStru_TradingAccountInfo data = *itFund;

		if ( m_testIndicator.MiniDynamicProfit > data.DynamicProfit)
		{
			m_testIndicator.MiniDynamicProfit =data.DynamicProfit;
		}
		if ( m_testIndicator.MaxDynamicProfit < data.DynamicProfit)
		{
			m_testIndicator.MaxDynamicProfit =data.DynamicProfit;
		}
		if ( m_testIndicator.MaxUsedFund < data.CurrMargin)
		{
			m_testIndicator.MaxUsedFund =data.CurrMargin;
		}
		m_testIndicator.FinishProfit = data.DynamicProfit;
	}

	if (m_testIndicator.MaxUsedFund!=0)
	{
		//21.	有效收益率%：净利润/最大使用资金
		m_testIndicator.EffectiveYield = m_testIndicator.NetProfit/m_testIndicator.MaxUsedFund;
		//23.	年度收益率%：（净利润/最大使用资金）/（总交易的天数/365）
		m_testIndicator.Annualyield = (m_testIndicator.EffectiveYield/m_testIndicator.TotalTradingTime)*365;
	}

	//25.	月度平均盈利：净利润/总交易的天数*30.5
	m_testIndicator.MonthlyAvgProfit = m_testIndicator.NetProfit/m_testIndicator.TotalTradingTime*30.5;
	//27.	年化收益：净利润/总交易的天数*365
	m_testIndicator.AnnualProfit = m_testIndicator.NetProfit/m_testIndicator.TotalTradingTime*365;

}
void  CDataEntity::SetTradingTime(int date)
{
	m_testIndicator.TotalTradingTime = date;
}
void CDataEntity::SetIndiOpenOrder( int Cnt)
{
	m_testIndicator.OpenOrderCnt = Cnt;
}
int  CDataEntity::GetMaxPosition()
{
    return m_testIndicator.MaxPosition;
}
void  CDataEntity::SetMaxPosition( int Cnt)
{
	 m_testIndicator.MaxPosition = Cnt;
}
bool CDataEntity::SaveTestIndicator()
{
	std::string strFile = m_strSavePath + "TestIndicator.csv";

	int nLine=1;

	std::map<u32, std::map<u32, std::string> > MapFile;
	std::map<u32, std::string> StringMap;

	int nRow=0;
	StringMap[nRow++] = Double2String(m_testIndicator.NetProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.AvgProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.TotalProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.TotalLoss);
	StringMap[nRow++] = Double2String(m_testIndicator.AvgLoss);
	StringMap[nRow++] = Double2String(m_testIndicator.InitialProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.MiniDynamicProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.MaxDynamicProfit );
	StringMap[nRow++] = Double2String(m_testIndicator.GrossProfit );
	StringMap[nRow++] = Double2String(m_testIndicator.AvgNetProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.TotalCommission);
	StringMap[nRow++] = Double2String(m_testIndicator.MaxUsedFund);
	StringMap[nRow++] = Double2String(m_testIndicator.FinishProfit );
	StringMap[nRow++] = Int2String(m_testIndicator.OpenOrderCnt );
	StringMap[nRow++] = Double2String(m_testIndicator.WinRate);
	StringMap[nRow++] = Double2String(m_testIndicator.YieldRate);
	StringMap[nRow++] = Double2String(m_testIndicator.EffectiveYield);
	StringMap[nRow++] = Double2String(m_testIndicator.Annualyield );
	StringMap[nRow++] = Double2String(m_testIndicator.MonthlyAvgProfit );
	StringMap[nRow++] = Double2String(m_testIndicator.AnnualProfit);
	StringMap[nRow++] = Int2String(m_testIndicator.MaxContinuousProfit);
	StringMap[nRow++] = Int2String(m_testIndicator.MaxContinuousLoss);
	StringMap[nRow++] = Int2String(m_testIndicator.TotalTradeCnt);
	StringMap[nRow++] = Int2String(m_testIndicator.MaxPosition);
	StringMap[nRow++] = Double2String(m_testIndicator.ConfidenceLevel);
	StringMap[nRow++] = Int2String(m_testIndicator.TotalOrderCnt);
	StringMap[nRow++] = Double2String(m_testIndicator.MaxProfit);
	StringMap[nRow++] = Double2String(m_testIndicator.MaxLoss);

	StringMap[nRow++] = m_testIndicator.szStartTime;
	StringMap[nRow++] = m_testIndicator.szEndTime;
	StringMap[nRow++] = m_testIndicator.szCreatTime;
	StringMap[nRow++] = m_testIndicator.szModifyTime;
	StringMap[nRow++] = m_testIndicator.szQuoteSource;

	MapFile[nLine] = StringMap;


	CCSVOperator            csvOprater;
	csvOprater.SetStringKeyMap(MapFile);	
	if (csvOprater.SaveCSV(strFile.c_str()))
	{
		return true;
	}
	else
		return false;

}
bool CDataEntity::LoadTestIndicator()
{

	std::string strFile = m_strSavePath + "TestIndicator.csv";
	CCSVOperator   csvOprater;
	if(csvOprater.LoadCSV(strFile.c_str()))
	{
		std::map<u32, std::map<u32, std::string> > MapFile;
		MapFile =csvOprater.GetCSVMap();
		std::map<u32, std::map<u32, std::string> >::iterator it = MapFile.begin();
		int i = 1;

		m_testIndicator.NetProfit = atof(it->second[i++].c_str());
		m_testIndicator.AvgProfit = atof(it->second[i++].c_str());
		m_testIndicator.TotalProfit = atof(it->second[i++].c_str());
		m_testIndicator.TotalLoss = atof(it->second[i++].c_str());
		m_testIndicator.AvgLoss = atof(it->second[i++].c_str());
		m_testIndicator.InitialProfit = atof(it->second[i++].c_str());
		m_testIndicator.MiniDynamicProfit = atof(it->second[i++].c_str());
		m_testIndicator.MaxDynamicProfit = atof(it->second[i++].c_str());
		m_testIndicator.GrossProfit = atof(it->second[i++].c_str());
		m_testIndicator.AvgNetProfit = atof(it->second[i++].c_str());
		m_testIndicator.TotalCommission = atof(it->second[i++].c_str());
		m_testIndicator.MaxUsedFund = atof(it->second[i++].c_str());
		m_testIndicator.FinishProfit = atof(it->second[i++].c_str());
		m_testIndicator.OpenOrderCnt = atoi(it->second[i++].c_str());
		m_testIndicator.WinRate = atof(it->second[i++].c_str());
		m_testIndicator.YieldRate = atof(it->second[i++].c_str());
		m_testIndicator.EffectiveYield = atof(it->second[i++].c_str());
		m_testIndicator.Annualyield = atof(it->second[i++].c_str());
		m_testIndicator.MonthlyAvgProfit = atof(it->second[i++].c_str());
		m_testIndicator.AnnualProfit = atof(it->second[i++].c_str());
		m_testIndicator.MaxContinuousProfit = atoi(it->second[i++].c_str());
		m_testIndicator.MaxContinuousLoss = atoi(it->second[i++].c_str());
		m_testIndicator.TotalTradeCnt = atoi(it->second[i++].c_str());
		m_testIndicator.MaxPosition = atoi(it->second[i++].c_str());
		m_testIndicator.ConfidenceLevel = atof(it->second[i++].c_str());
		m_testIndicator.TotalOrderCnt = atoi(it->second[i++].c_str());
		m_testIndicator.MaxProfit = atof(it->second[i++].c_str());
		m_testIndicator.MaxLoss = atof(it->second[i++].c_str());
		strncpy(m_testIndicator.szStartTime, it->second[i++].c_str(),sizeof(m_testIndicator.szStartTime));	
		strncpy(m_testIndicator.szEndTime, it->second[i++].c_str(),sizeof(m_testIndicator.szEndTime));	
		strncpy(m_testIndicator.szCreatTime, it->second[i++].c_str(),sizeof(m_testIndicator.szCreatTime));	
		strncpy(m_testIndicator.szModifyTime, it->second[i++].c_str(),sizeof(m_testIndicator.szModifyTime));	
		strncpy(m_testIndicator.szQuoteSource, it->second[i++].c_str(),sizeof(m_testIndicator.szQuoteSource));	

		return true;

	}


	else
		return false;
}
void CDataEntity::SetInitFund( double fund)
{
    m_testIndicator.InitialProfit = fund;
	 m_testIndicator.MiniDynamicProfit = fund;
}
void CDataEntity::GetQuot(std::list<PlatformStru_DepthMarketData>& outData)
{
	outData = m_lstQuot;
}
void CDataEntity::GetInstrumentInfo(std::map<std::string, PlatformStru_InstrumentInfo>& outData)
{
	outData = m_mapInstrument;
}
void CDataEntity::GetMarginRate(std::map<std::string, PlatformStru_InstrumentMarginRate>& outData)
{
	outData = m_mapMarginRate;
}
void CDataEntity::GetCommissionRate(std::map<std::string, PlatformStru_InstrumentCommissionRate>& outData)
{
	outData = m_mapCommissionRate;
}
void CDataEntity::GetTradingAccountInfo(std::list<PlatformStru_TradingAccountInfo>& outData)
{
	outData = m_lstTradingAccnt;
}
void CDataEntity::GetPosition(std::list<PlatformStru_Position>& outData)
{
	outData = m_lstPosition;
}
void CDataEntity::GetOrderInfo(std::list<PlatformStru_OrderInfo>& outData)
{
	outData = m_lstOrder;
}
void CDataEntity::GetTradeInfo(std::list<PlatformStru_TradeInfo>& outData)
{
	outData = m_lstTrade;
}
void CDataEntity::GetTestIndicator(TestIndicator& outData)
{
	outData = m_testIndicator;
}
//bool CDataManage::ConvToQuot(const std::string& strQuot, PlatformStru_DepthMarketData& ret)
//{
//	std::vector<std::string> vecItem;
//    int startpos=0,endpos=0;
//	std::string strTrueLine=strQuot+",";
//    while(true) {
//        endpos=strTrueLine.find(',',startpos);
//        if(endpos<0) 
//            break;
//		vecItem.push_back(strTrueLine.substr(startpos,endpos-startpos));
//        startpos=endpos+1;   
//    }
//	if(vecItem.size()<64) 
//		return false;
//
//    int index=0;
//
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.TradingDay,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.InstrumentID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.ExchangeID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.ExchangeInstID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.LastPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.PreSettlementPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.PreClosePrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.PreOpenInterest = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.OpenPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.HighestPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.LowestPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.Volume = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.Turnover = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.OpenInterest = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.ClosePrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.SettlementPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.UpperLimitPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.LowerLimitPrice = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.PreDelta = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.CurrDelta = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	strcpy(ret.UpdateTime,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.UpdateMillisec = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidPrice1 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidVolume1 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskPrice1 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskVolume1 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidPrice2 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidVolume2 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskPrice2 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskVolume2 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidPrice3 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidVolume3 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskPrice3 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskVolume3 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidPrice4 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidVolume4 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskPrice4 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskVolume4 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidPrice5 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.BidVolume5 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskPrice5 = atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AskVolume5 = atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//	ret.AveragePrice = atof(vecItem[index++].c_str());
////bidprice6,vecItem[index++].c_str());
////bidvolume6,vecItem[index++].c_str());
////askprice6,vecItem[index++].c_str());
////askvolume6,vecItem[index++].c_str());
////bidprice7,vecItem[index++].c_str());
////bidvolume7,vecItem[index++].c_str());
////askprice7,vecItem[index++].c_str());
////askvolume7,vecItem[index++].c_str());
////bidprice8,vecItem[index++].c_str());
////bidvolume8,vecItem[index++].c_str());
////askprice8,vecItem[index++].c_str());
////askvolume8,vecItem[index++].c_str());
////bidprice9,vecItem[index++].c_str());
////bidvolume9,vecItem[index++].c_str());
////askprice9,vecItem[index++].c_str());
////askvolume9,vecItem[index++].c_str());
////bidprice10,vecItem[index++].c_str());
////bidvolume10,vecItem[index++].c_str());
////askprice10,vecItem[index++].c_str());
////askvolume10,vecItem[index++].c_str());
////newvolume,vecItem[index++].c_str());
//	Debug::WriteLine("");
//	return true;
//}
//
//bool CDataManage::ConvToInstrument(const std::string& strInstrument, PlatformStru_InstrumentInfo& ret)
//{
//	std::vector<std::string> vecItem;
//    int startpos=0,endpos=0;
//	std::string strTrueLine=strInstrument+",";
//    while(true) {
//        endpos=strTrueLine.find(',',startpos);
//        if(endpos<0) 
//            break;
//		vecItem.push_back(strTrueLine.substr(startpos,endpos-startpos));
//        startpos=endpos+1;   
//    }
//	if(vecItem.size()<25) 
//		return false;
//
//    int index=0;
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.InstrumentID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.ExchangeID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.InstrumentName,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.ExchangeInstID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.ProductID,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.ProductClass=vecItem[index++][0];
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.DeliveryYear=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.DeliveryMonth=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.MaxMarketOrderVolume=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.MinMarketOrderVolume=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.MaxLimitOrderVolume=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.MinLimitOrderVolume=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.VolumeMultiple=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.PriceTick=atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.CreateDate,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.OpenDate,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.ExpireDate,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.StartDelivDate,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    strcpy(ret.EndDelivDate,vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.InstLifePhase=vecItem[index++][0];
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.IsTrading=atoi(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.PositionType=vecItem[index++][0];
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.PositionDateType=vecItem[index++][0];
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.LongMarginRatio=atof(vecItem[index++].c_str());
//	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
//    ret.ShortMarginRatio=atof(vecItem[index++].c_str());
//	Debug::WriteLine("");
//	return true;
//}
