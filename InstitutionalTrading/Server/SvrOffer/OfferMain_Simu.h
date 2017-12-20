#pragma once

class COfferMain_Simu:public COfferMainInterface
{
public:
	COfferMain_Simu(void);
	~COfferMain_Simu(void);
	virtual  void Init(bool bSettlement, int nMorning);//0 为交易，1为结算
	virtual void QueryInstruments();
	virtual void QueryQuot();
	virtual void QueryQuot(std::map<std::string, int>& mapInstrumentID);
	//从交易数据管理里面获取数据
	virtual void AddInstrument();
	virtual void QueryCommission(BrokerAccountKey& BAKey);
	virtual void QueryMargin(BrokerAccountKey& BAKey);
	virtual void QueryOrder(BrokerAccountKey& BAKey);
	virtual void QueryTrade(BrokerAccountKey& BAKey);
	void RestartAccount(int nTradeAccount);
	void LogoffAccount(BrokerAccountKey BAKey);

	bool GetTadingDay(std::string& strTradeDay, int nMorning);
	bool GetTadingTime(std::string& strTradeTime);

private:
	//保存账号各种状态信息
	std::map<BrokerAccountKey, CCTPAccount*>		m_mapUserID2CTPAccount;
	CReadWriteLock							m_mutex_mapUserID2CTPAccount;

	std::map<std::string, int>				m_mapInstrumentID;	
	CReadWriteLock							m_mutex_mapInstrument;

public:
	void split(const string& s, char c, vector<string>& v);
	bool GetCTPAccount(BrokerAccountKey BAKey, CCTPAccount*& pCtpAccount);
	void SetOneInstrument(string& strLine);
	bool LoadCommissionRate(TiXmlElement* pRoot);
	bool LoadMarginRate(TiXmlElement* pRoot);


	virtual int ReqOrderInsert_Main(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);
	virtual int COfferMain_Simu::ReqOrderAction_Main(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);

	std::string m_strTradeDay;//交易日
};
