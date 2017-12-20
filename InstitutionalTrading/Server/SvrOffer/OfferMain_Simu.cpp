#include "stdafx.h"
#include "OfferMainInterface.h"
#include "OfferMain_Simu.h"
#include "textfile.h"

COfferMain_Simu::COfferMain_Simu(void)
{
	m_strTradeDay = "";
}

COfferMain_Simu::~COfferMain_Simu(void)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(; it != m_mapUserID2CTPAccount.end(); it++)
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();
		pCtpAccount->SetAccountStatus(ACCOUNT_STATUS_Disconnected);
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
	}	
	m_mapUserID2CTPAccount.clear();
}

void COfferMain_Simu::Init(bool bSettlement, int nMorning)
{
	//if(!bTrade)
	InitDBData();
	std::vector<SOrderTransfer>		vecOrderTransfer;
	GetOrderTransfer(vecOrderTransfer);
	SetOrderTransferByAccount(vecOrderTransfer);

	std::vector<SLogin> vecLogin;
	std::vector<TradeAccount> vec;
	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(vec);	
	if(vec.size() ==0)
		return;
	for(int i=0; i < (int)vec.size(); i++)
	{
		TradeAccount& tradeAccount = vec[i];
		SLogin lsTemp;
		BrokerInfo brokeInfo;
		CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID, brokeInfo);
		lsTemp.strBrokerID = brokeInfo.szBrokerCode;			
		lsTemp.strUserID = tradeAccount.szTradeAccount;
		lsTemp.strPassword = tradeAccount.szPwd;
		
		vecLogin.push_back(lsTemp);		
	}
	//std::string strTradeDay;	
	GetTadingDay(m_strTradeDay, nMorning);//把交易日设给交易数据管理	
	CInterface_SvrSimulateTrade::getObj().Reinit(m_strTradeDay);	
	int nFrontID =0, nSessionID =0;
	for(int i=0; i< (int)vecLogin.size(); i++)
	{
		SLogin& login = vecLogin[i];
		BrokerAccountKey BAKey(login.strBrokerID.c_str(), login.strUserID.c_str());
		CCTPAccount* pCtpAccount = new CCTPAccount;	
		InsertOrderRef(BAKey, pCtpAccount);
		pCtpAccount->Init_simu(login);

		m_mutex_mapUserID2CTPAccount.write_lock();
		m_mapUserID2CTPAccount.insert(make_pair(BAKey, pCtpAccount)); 				
		m_mutex_mapUserID2CTPAccount.write_unlock();

		CInterface_SvrTradeData::getObj().SetAccountStatus(login.strUserID, login.strBrokerID, ACCOUNT_STATUS_Login);
		pCtpAccount->SetAccountStatus(ACCOUNT_STATUS_Login);

		
		std::string strTime;
		GetTadingTime(strTime);
		CThostFtdcRspUserLoginField rspLogin;
		strcpy(rspLogin.TradingDay, m_strTradeDay.c_str());
		strcpy(rspLogin.CZCETime, strTime.c_str());
		strcpy(rspLogin.DCETime, strTime.c_str());
		strcpy(rspLogin.FFEXTime, strTime.c_str());
		strcpy(rspLogin.SHFETime, strTime.c_str());

		strcpy(rspLogin.BrokerID, login.strBrokerID.c_str());
		strcpy(rspLogin.UserID, login.strUserID.c_str());
		
		SYSTEMTIME st;
		GetLocalTime(&st);

		nFrontID = st.wHour*10000+ st.wMinute*100+ st.wSecond;
		rspLogin.FrontID   = nFrontID;
		nSessionID	= st.wHour*10000+ st.wMinute*100+ st.wSecond + 1;
		rspLogin.SessionID = nSessionID;

		CInterface_SvrSimulateTrade::getObj().Login(rspLogin);

		
		pCtpAccount->SetFrontID(rspLogin.FrontID);
		pCtpAccount->SetSessionID(rspLogin.SessionID);
//////////////////////////////////////////////////////////////////////////
		sExchangeTimeInfo sTime;
		strcpy(sTime.CZCETime, rspLogin.CZCETime);
		strcpy(sTime.DCETime, rspLogin.DCETime);
		strcpy(sTime.FFEXTime, rspLogin.FFEXTime);
		strcpy(sTime.SHFETime, rspLogin.SHFETime);
		strcpy(sTime.TradingDay, rspLogin.TradingDay);
		
		sprintf(sTime.GetTime, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);	
		CInterface_SvrLogin::getObj().SetTimeInfo(sTime);	
	}	

	
	CInterface_SvrTradeData::getObj().SetCurrentTradingDay(m_strTradeDay); 	
	CInterface_SvrSimulateTrade::getObj().ResertTradingDay(m_strTradeDay);//模拟成交模块
	
}
void COfferMain_Simu::split(const string& s, char c, vector<string>& v) 
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
void COfferMain_Simu::QueryInstruments()
{
	std::string strpath ="";
	std::string strfile = INSTRUMENT_FILENAME;
	GetFolderFileName(strfile, strpath);
	CTextFileRead myfile(strfile.c_str());

	if(!myfile.IsOpen())
	{
		OFFER_INFO("读取本地合约文件[%s]失败\n", strpath.c_str());
		PUSH_LOG_ERROR(Logger,false,true,"读取本地合约文件[%s]失败\n", strpath.c_str());	
		return;
	}
	std::string strtot;	
	myfile.Read(strtot);
	myfile.Close();
	
	std::vector<std::string> vec;	
	split(strtot, '\n', vec);	
	if(vec.size() == 0)
	{
		OFFER_INFO("本地合约文件[%s]里面没有合约信息\n", strpath.c_str());
		PUSH_LOG_ERROR(Logger,false,true,"本地合约文件[%s]里面没有合约信息\n", strpath.c_str());
	}
	for(int i =0; i< (int)vec.size(); i++)
	{
		std::string  strLine = vec[i];
		SetOneInstrument(strLine);
	}
	CInterface_SvrTradeData::getObj().EndQryInstrument();

	QueryQuot();
}
void COfferMain_Simu::SetOneInstrument(string& strLine)
{
	PlatformStru_InstrumentInfo Infofield;
	memset(&Infofield,0,sizeof(PlatformStru_InstrumentInfo));
	std::vector<std::string>  tokenvector; 
	split(strLine, ',',tokenvector);  
	if(tokenvector.size()<25) return;
	int index=0;
	strcpy(Infofield.InstrumentID,tokenvector[index++].c_str());
	strcpy(Infofield.ExchangeID,tokenvector[index++].c_str());
	strcpy(Infofield.InstrumentName,tokenvector[index++].c_str());
	strcpy(Infofield.ExchangeInstID,tokenvector[index++].c_str());
	strcpy(Infofield.ProductID,tokenvector[index++].c_str());
	Infofield.ProductClass=tokenvector[index++][0];
	Infofield.DeliveryYear=atoi(tokenvector[index++].c_str());
	Infofield.DeliveryMonth=atoi(tokenvector[index++].c_str());
	Infofield.MaxMarketOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.MinMarketOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.MaxLimitOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.MinLimitOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.VolumeMultiple=atoi(tokenvector[index++].c_str());
	Infofield.PriceTick=atof(tokenvector[index++].c_str());
	strcpy(Infofield.CreateDate,tokenvector[index++].c_str());
	strcpy(Infofield.OpenDate,tokenvector[index++].c_str());
	strcpy(Infofield.ExpireDate,tokenvector[index++].c_str());
	strcpy(Infofield.StartDelivDate,tokenvector[index++].c_str());
	strcpy(Infofield.EndDelivDate,tokenvector[index++].c_str());
	Infofield.InstLifePhase=tokenvector[index++][0];
	Infofield.IsTrading=atoi(tokenvector[index++].c_str());
	Infofield.PositionType=tokenvector[index++][0];
	Infofield.PositionDateType=tokenvector[index++][0];
	Infofield.LongMarginRatio=atof(tokenvector[index++].c_str());
	Infofield.ShortMarginRatio=atof(tokenvector[index++].c_str());

	m_mutex_mapInstrument.write_lock();
	m_mapInstrumentID[Infofield.InstrumentID] = 0;
	m_mutex_mapInstrument.write_unlock();

	CInterface_SvrTradeData::getObj().SetInstrumentInfo(Infofield);
}

void COfferMain_Simu::AddInstrument()
{
	std::vector<PlatformStru_InstrumentInfo> lVecInstruments;
	std::set<std::string> lInstrumentStringSet;
	CInterface_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
	OFFER_INFO("Instruments size %d\n",lInstrumentStringSet.size());
	//PUSH_LOG_ERROR(Logger,false,true,"Instruments size %d",lInstrumentStringSet.size());
	std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
	while(lIterSet != lInstrumentStringSet.end())
	{
		PlatformStru_InstrumentInfo lTempInfo;
		if(0 == CInterface_SvrTradeData::getObj().GetInstrumentInfo(*lIterSet,lTempInfo))
		{
			m_mutex_mapInstrument.write_lock();
			m_mapInstrumentID[lTempInfo.InstrumentID] = 0;
			m_mutex_mapInstrument.write_unlock();
		}			
		lIterSet++;
	}

	QueryQuot();
}
void COfferMain_Simu::QueryQuot()
{
	m_mutex_mapInstrument.write_lock();
	QueryQuot(m_mapInstrumentID);	
	m_mutex_mapInstrument.write_unlock();
}
void COfferMain_Simu::QueryQuot(std::map<std::string, int>& mapInstrumentID)
{
	COfferMainInterface::QueryQuot(mapInstrumentID);
}
bool COfferMain_Simu::GetTadingDay(std::string& strTradeDay, int nMorning)
{
	if(nMorning != 0 && nMorning != 1)
		return true;

	bool bGet = false;
	std::string strpath ="";
	std::string strfile = COMMISSION_MARGIN_FILENAME;
	GetFolderFileName(strfile, strpath);
	TiXmlDocument* pDocument=new TiXmlDocument(strfile.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			std::string strDate,strRoot = RootElement->Value()?RootElement->Value():"";
			if(strcmp(strRoot.c_str(), "Rate")==0)
			{//日期不读配置文件，按照系统时间来
			//	const char* dateval=RootElement->Attribute("date");
			//	const char* idval=RootElement->Attribute("id");
			//	strTradeDay= dateval;
			//	if(dateval&& strlen(dateval)>=8)
			//	{//配置文件里面有日期，则按配置文件算当天日期（格式：20130731）
			//		bGet = true;
			//	}
			//	else
				{
					time_t tm(NULL);
					struct tm* target_time;
					time ( &tm ); 
					target_time = localtime (&tm); 
					//if(target_time->tm_hour > 21)//不是按照21点来了，按照交易数据管理发的早盘晚盘命令来判断是不是第二天
					if(nMorning)
					{
						tm += 86400;
						target_time = localtime (&tm); 
					}
					char strTime[64];
					memset(&strTime, 0, sizeof(strTime));					
					sprintf(strTime,"%4d%02d%02d",target_time->tm_year+1900, target_time->tm_mon +1, target_time->tm_mday);	
					strTradeDay= strTime;
					bGet = true;

				}
			}
		}
	}
	if(pDocument)
	{
		delete pDocument;
		pDocument = NULL;
	}
	return bGet;
}
bool COfferMain_Simu::GetTadingTime(std::string& strTradeTime)
{
	char szGet[9]; 
	SYSTEMTIME st;
	GetLocalTime(&st);				
	sprintf(szGet, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);	
	strTradeTime= szGet;
	return true;
}
void COfferMain_Simu::QueryCommission(BrokerAccountKey& BAKey)
{
	std::string strpath ="";
	std::string strfile = COMMISSION_MARGIN_FILENAME;
	GetFolderFileName(strfile, strpath);
	TiXmlDocument* pDocument=new TiXmlDocument(strfile.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			std::string strDate,strRoot = RootElement->Value()?RootElement->Value():"";
			TiXmlElement* pCommissionRateElement=RootElement->FirstChildElement();
			if(pCommissionRateElement)
			{
				if(LoadCommissionRate(pCommissionRateElement)) 
				{								
				}
			}  				
		}
	}
	if(pDocument)
	{
		delete pDocument;
		pDocument = NULL;
	}
	CInterface_SvrTradeData::getObj().EndUserQryCommission(BAKey.BrokerID, BAKey.AccountID);
}
bool COfferMain_Simu::LoadCommissionRate(TiXmlElement* pRoot)
{
	std::vector<TradeAccount> vec;
	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(vec);	
	if(vec.size() ==0)
		return true;


	TiXmlElement* pFieldElement=pRoot->FirstChildElement();
	PlatformStru_InstrumentCommissionRate field;
	memset(&field,0,sizeof(field));
	//获取BrokerID和InvestorID
	field.InvestorRange=THOST_FTDC_IR_Single;
//	string strInvestorID,strBrokerID;
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
//	strcpy(field.BrokerID,strBrokerID.c_str());
//	strcpy(field.InvestorID,strInvestorID.c_str());

	std::string tagname,tagvalue;
	while(pFieldElement)
	{
		if(pFieldElement->GetText())
			strcpy(field.InstrumentID,pFieldElement->GetText());
		TiXmlAttribute* attributeName=pFieldElement->FirstAttribute();
		while(attributeName)
		{
			tagname=attributeName->Name()?attributeName->Name():"";
			if(tagname=="ORBM")       field.OpenRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="ORBV")  field.OpenRatioByVolume=attributeName->DoubleValue();
			else if(tagname=="CRBM")  field.CloseRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="CRBV")  field.CloseRatioByVolume=attributeName->DoubleValue();
			else if(tagname=="CTRBM")  field.CloseTodayRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="CTRBV")  field.CloseTodayRatioByVolume=attributeName->DoubleValue();
			attributeName=attributeName->Next();
		}
		for(int i=0; i < (int)vec.size(); i++)
		{
			TradeAccount& tradeAccount = vec[i];			
			BrokerInfo brokeInfo;
			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID, brokeInfo);
			strcpy(field.BrokerID, brokeInfo.szBrokerCode);	
			strcpy(field.InvestorID, tradeAccount.szTradeAccount);
			CInterface_SvrTradeData::getObj().SetCommissionRate(field);
		}
	//	if(DEFAULT_SVR()) DEFAULT_SVR()->SetCommissionRate(field.InstrumentID,field);
		pFieldElement=pFieldElement->NextSiblingElement();
	}
	return true;
}
void COfferMain_Simu::QueryMargin(BrokerAccountKey& BAKey)
{
	std::string strpath ="";
	std::string strfile = COMMISSION_MARGIN_FILENAME;
	GetFolderFileName(strfile, strpath);
	TiXmlDocument* pDocument=new TiXmlDocument(strfile.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			std::string strDate,strRoot = RootElement->Value()?RootElement->Value():"";
			TiXmlElement* pCommissionRateElement=RootElement->FirstChildElement();
			if(pCommissionRateElement)
			{
				TiXmlElement* pMarginRateElement=pCommissionRateElement->NextSiblingElement();
				if(pMarginRateElement)
				{
					retval=LoadMarginRate(pMarginRateElement);
				}			
			}  				
		}
	}
	if(pDocument)
	{
		delete pDocument;
		pDocument = NULL;
	}
	CInterface_SvrTradeData::getObj().EndUserQryMargin(BAKey.BrokerID, BAKey.AccountID);
}
bool COfferMain_Simu::LoadMarginRate(TiXmlElement* pRoot)
{
	std::vector<TradeAccount> vec;
	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(vec);	
	if(vec.size() ==0)
		return true;

	TiXmlElement* pFieldElement=pRoot->FirstChildElement();
	PlatformStru_InstrumentMarginRate field;
	memset(&field,0,sizeof(field));
	//获取BrokerID和InvestorID
	string strInvestorID="",strBrokerID="";
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
	field.InvestorRange=THOST_FTDC_IR_Single;
//	strcpy(field.BrokerID,strBrokerID.c_str());
//	strcpy(field.InvestorID,strInvestorID.c_str());
	std::string tagname,tagvalue;
	while(pFieldElement)
	{
		if(pFieldElement->GetText())
			strcpy(field.InstrumentID,pFieldElement->GetText());
		TiXmlAttribute* attributeName=pFieldElement->FirstAttribute();
		while(attributeName)
		{
			tagname=attributeName->Name()?attributeName->Name():"";
			if(tagname=="LMBM")       field.LongMarginRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="LMBV")  field.LongMarginRatioByVolume=attributeName->DoubleValue();
			else if(tagname=="SMBM")  field.ShortMarginRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="SMBV")  field.ShortMarginRatioByVolume=attributeName->DoubleValue();
			attributeName=attributeName->Next();
		}
		for(int i=0; i < (int)vec.size(); i++)
		{
			TradeAccount& tradeAccount = vec[i];			
			BrokerInfo brokeInfo;
			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID, brokeInfo);
			strcpy(field.BrokerID, brokeInfo.szBrokerCode);
			strcpy(field.InvestorID, tradeAccount.szTradeAccount);
			CInterface_SvrTradeData::getObj().SetMarginRate(field);
		}
		//if(DEFAULT_SVR()) DEFAULT_SVR()->SetMarginRate(field.InstrumentID,field);
		pFieldElement=pFieldElement->NextSiblingElement();
	}
	return true;
}
void COfferMain_Simu::QueryOrder(BrokerAccountKey& BAKey)
{
	std::string strTradeDay = m_strTradeDay;
//	bool bGet = GetTadingDay(strTradeDay); 
//	if(bGet && strTradeDay.length()>=8)
	{		
		std::string lsTemp = strTradeDay.substr(0,4) + "-" + strTradeDay.substr(4,2) + "-" + strTradeDay.substr(6,2);
		vector<PlatformStru_OrderInfo> lOrderVec;
		CInterface_SvrTradeData::getObj().GetInterfaceDBOpr()->LoadTodayUserOrders("SIMU_CTP_ORDERS", BAKey.AccountID,lsTemp,lOrderVec);
		for(int i =0; i<(int)lOrderVec.size(); i++)
		{
			PlatformStru_OrderInfo& order = lOrderVec[i];
			BrokerAccountKey BAKey2;
			strcpy(BAKey2.BrokerID, order.BrokerID);
			strcpy(BAKey2.AccountID, order.InvestorID);		
			CCTPAccount* pCtpAccount = NULL;
			if(!GetCTPAccount(BAKey2,  pCtpAccount))
				continue;
			if(pCtpAccount)
			{
				CThostFtdcOrderField orderCtp;
				CopyCTPOrderField(orderCtp, order);
				pCtpAccount->OnRspQryOrder(&orderCtp);			
			}			
		/*	std::vector<UserInfo> vec;	
			CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(order.UserID, order.BrokerID, vec);
			for(int i =0; i< (int)vec.size(); i++)
			{
				UserInfo& user = vec[i];
				strcpy(order.InvestorID, user.szAccount);
				strcpy(order.szAccount, user.szAccount);
				CInterface_SvrTradeData::getObj().QryOrderReach(order, 0, false);
			}*/					
		}
	}	
	CInterface_SvrTradeData::getObj().EndUserQryOrder(BAKey.BrokerID, BAKey.AccountID);
}
void COfferMain_Simu::QueryTrade(BrokerAccountKey& BAKey)
{
	std::string strTradeDay = m_strTradeDay;
	//	bool bGet = GetTadingDay(strTradeDay); 
	//	if(bGet && strTradeDay.length()>=8)
	{		
		std::string lsTemp = strTradeDay.substr(0,4) + "-" + strTradeDay.substr(4,2) + "-" + strTradeDay.substr(6,2);
		vector<PlatformStru_TradeInfo> lTradeVec;
		CInterface_SvrTradeData::getObj().GetInterfaceDBOpr()->LoadTodayUserTrades("SIMU_CTP_TRADES", BAKey.AccountID,lsTemp,lTradeVec);
		for(int i =0; i<(int)lTradeVec.size(); i++)
		{
			PlatformStru_TradeInfo& trade = lTradeVec[i];
			BrokerAccountKey BAKey2;
			strcpy(BAKey2.BrokerID, trade.BrokerID);
			strcpy(BAKey2.AccountID, trade.InvestorID);			
		
			CCTPAccount* pCtpAccount = NULL;
			if(!GetCTPAccount(BAKey2,  pCtpAccount))
				continue;
			if(pCtpAccount)
			{
				CThostFtdcTradeField data;
				CopyCTPTradeRecordField(data, trade);
				pCtpAccount->OnRspQryTrade(&data);			
			}			
		}
	}	
	CInterface_SvrTradeData::getObj().EndUserQryTrade(BAKey.BrokerID, BAKey.AccountID);
}
int COfferMain_Simu::ReqOrderInsert_Main(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID)
{
	//去掉前面的空格
	int nOrderRef = atoi(key.szOrderRef);
	sprintf(key.szOrderRef, "%d", nOrderRef);

	int nRet = CF_ERROR_RISK_NOTVALIDORDER;
	if(pInputOrder == NULL)
		return nRet;	
	std::string strInvestor   = pInputOrder->InvestorID;	
	std::string strBrokerID;
	std::string strAccountID;//
	GetTradeAccountByUserID(nTraderID, strBrokerID, strAccountID);
	//strcpy(pInputOrder->InvestorID, strAccountID.c_str());

	BrokerAccountKey BAKey(strBrokerID.c_str(), strAccountID.c_str());
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		nRet = pCtpAccount->ReqOrderInsert_SimuAccount(pInputOrder, nTraderID, key, nRequestID);

	}	


	return nRet;
}
int COfferMain_Simu::ReqOrderAction_Main(CCifcoInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID)
{
	//去掉前面的空格
	int nOrderRef = atoi(key.szOrderRef);
	sprintf(key.szOrderRef, "%d", nOrderRef);

	int nRet = CF_ERROR_RISK_NOTVALIDORDER;
	if(pInputOrderAction == NULL)
		return nRet;	

	std::string strBrokerID;
	std::string strAccountID;
	GetTradeAccountByUserID(nTraderID, strBrokerID, strAccountID);
	//strcpy(pInputOrderAction->InvestorID, strAccountID.c_str());				///投资者代码

	BrokerAccountKey BAKey(strBrokerID.c_str(), strAccountID.c_str());
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;	
		m_mutex_mapUserID2CTPAccount.write_unlock();

		nRet = pCtpAccount->ReqOrderAction_SimuAccount(pInputOrderAction, nTraderID, key, nRequestID);

	}	

	return nRet;
}
bool COfferMain_Simu::GetCTPAccount(BrokerAccountKey BAKey, CCTPAccount*& pCtpAccount)
{
	bool bFind = false;	
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		pCtpAccount = it->second;	
		m_mutex_mapUserID2CTPAccount.write_unlock();
		bFind = true;
	}	
	return bFind;
}
void COfferMain_Simu::RestartAccount(int nTradeAccount)
{
	TradeAccount tradeAccount;
	if(!CInterface_SvrBrokerInfo::getObj().GetTradeAccount(nTradeAccount, tradeAccount))
		return;
	BrokerInfo brokeInfo;
	CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID, brokeInfo);
	BrokerAccountKey BAKey;
	strcpy(BAKey.BrokerID, brokeInfo.szBrokerCode);
	strcpy(BAKey.AccountID, tradeAccount.szTradeAccount);

	m_mutex_mapUserID2CTPAccount.write_lock();
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{
		CCTPAccount* pCtpAccount = it->second;
		m_mapUserID2CTPAccount.erase(it);				
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
	}		
	m_mutex_mapUserID2CTPAccount.write_unlock();


	//////////////////////////////////////////////////////////////////////////
	std::vector<SLogin> vecLogin;

	SLogin lsTemp;	
	lsTemp.strBrokerID = brokeInfo.szBrokerCode;	
	//交易用的是某个交易员特定的服务器地址
	ServerGroup group;
	CInterface_SvrBrokerInfo::getObj().GetServerGroupByID(tradeAccount.nServerGroupID, group);
	std::vector<ServerAddress> vecAddress;
	CInterface_SvrBrokerInfo::getObj().GetServerAddrByGroupID(group.nServerID, vecAddress);
	copy(vecAddress.begin(), vecAddress.end(), back_inserter(lsTemp.vecServerAddress));		
	lsTemp.strUserID = tradeAccount.szTradeAccount;
	lsTemp.strPassword = tradeAccount.szPwd;
	vecLogin.push_back(lsTemp);	
	std::string strTradeDay;
	int nFrontID =0, nSessionID =0;
	for(int i=0; i< (int)vecLogin.size(); i++)
	{
		SLogin& login = vecLogin[i];
		BrokerAccountKey BAKey(login.strBrokerID.c_str(), login.strUserID.c_str());
		CCTPAccount* pCtpAccount = new CCTPAccount;	
		InsertOrderRef(BAKey, pCtpAccount);
		pCtpAccount->Init_simu(login);

		m_mutex_mapUserID2CTPAccount.write_lock();
		m_mapUserID2CTPAccount.insert(make_pair(BAKey, pCtpAccount)); 				
		m_mutex_mapUserID2CTPAccount.write_unlock();

		CInterface_SvrTradeData::getObj().SetAccountStatus(login.strUserID, login.strBrokerID, ACCOUNT_STATUS_Login);
		pCtpAccount->SetAccountStatus(ACCOUNT_STATUS_Login);

		//GetTadingDay(strTradeDay);//把交易日设给交易数据管理	
		std::string strTime;
		GetTadingTime(strTime);
		CThostFtdcRspUserLoginField rspLogin;
		strcpy(rspLogin.TradingDay, m_strTradeDay.c_str());
		strcpy(rspLogin.CZCETime, strTime.c_str());
		strcpy(rspLogin.DCETime, strTime.c_str());
		strcpy(rspLogin.FFEXTime, strTime.c_str());
		strcpy(rspLogin.SHFETime, strTime.c_str());

		strcpy(rspLogin.BrokerID, login.strBrokerID.c_str());
		strcpy(rspLogin.UserID, login.strUserID.c_str());

		SYSTEMTIME st;
		GetLocalTime(&st);

		nFrontID = st.wHour*10000+ st.wMinute*100+ st.wSecond;
		rspLogin.FrontID   = nFrontID;
		nSessionID	= st.wHour*10000+ st.wMinute*100+ st.wSecond + 1;
		rspLogin.SessionID = nSessionID;

		CInterface_SvrSimulateTrade::getObj().Login(rspLogin);


		pCtpAccount->SetFrontID(rspLogin.FrontID);
		pCtpAccount->SetSessionID(rspLogin.SessionID);
		//////////////////////////////////////////////////////////////////////////
			
	}	

}
void COfferMain_Simu::LogoffAccount(BrokerAccountKey BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		pCtpAccount->SetAccountStatus(ACCOUNT_STATUS_Disconnected);
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
		m_mapUserID2CTPAccount.erase(it);
	}

}