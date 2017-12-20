#pragma once
#define TIMER_CHECK_ACCOUNTSTATUS 12345
#include "CppSQLite3.h"
class CCTPAccount;
class CToolThread;
class COfferMainInterface
{
public:
	COfferMainInterface();
	virtual ~COfferMainInterface();

	virtual void QueryInstruments(){};	
	virtual void Init(bool bSettlement, int nMorning){};

public://实盘需要函数
	virtual bool GetCtpAccountByTime(UINT_PTR uPtr, CCTPAccount*& pCtpAccount){return true;};
	virtual void RemoveCtpAccountByTime(UINT_PTR uPtr){};
	virtual void CheckAccountsConnect(){};
	virtual void AddInstrument(){};
	virtual void AddInstrument(CThostFtdcInstrumentField *pInstrument, bool bIsLast){};
	virtual void RestartAccount(int nTradeAccount){};
	virtual void QueryCommission(BrokerAccountKey& BAKey){};
	virtual void QueryMargin(BrokerAccountKey& BAKey){};
	virtual void QueryOrder(BrokerAccountKey& BAKey){};
	virtual void QueryTrade(BrokerAccountKey& BAKey){};
	virtual void QueryFund(BrokerAccountKey& BAKey){};
	virtual void QueryPosition(BrokerAccountKey& BAKey){};
	virtual void QueryPositionDetail(BrokerAccountKey& BAKey){};
	virtual void LogoffAccount(BrokerAccountKey BAKey){};
	virtual bool GetCTPAccount(BrokerAccountKey BAKey, CCTPAccount*& pCtpAccount){ return true; };
	
	virtual bool GetProductIDByInstruments(std::string strInstrument, std::string& strProduct){return true;};
	virtual bool IsCtpAccountTimerExit(CCTPAccount* pCtpAccount){ return true;};
	virtual void SetTimer2CTPAccount(UINT_PTR uPtr, CCTPAccount* pCtpAccount){};
	virtual bool NeedQueryInstruments(){ return true;};
	virtual void SetNeedQueryInstruments(bool bNeed){};
	virtual void GetInstrumentsByProductID(std::string strProduct, std::vector<std::string>& vecInstruments){};

	
	virtual int ReqOrderInsert_Main(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID){return 0;};
	virtual int ReqOrderAction_Main(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID){return 0;};

public://报单存储
	virtual void QueryQuot(){};
	//根据交易员账号得到委托交易账号
	bool GetTradeAccountByUserID(int nTraderID, std::string& strBrokerID, std::string& strInvestID);

	void InitDBData();
	void GetFolderFileName(std::string& strFilePath,const std::string& folder);
	bool SaveOrderTransfer(SOrderTransfer& order,	int& nErrorCode);
	bool GetOrderTransfer(std::vector<SOrderTransfer>& vecOrderTransfer); 
	bool UpdateOrderTransfer(SOrderTransfer& sOrderTransfer);

	static DWORD WINAPI ThreadOrderTransfer(PVOID pParam);	
	void doOrderTransfer(COfferMainInterface* pMgr);
	CToolThread*		  m_Thread_OrderTransfer;
	const int			  m_IdleSleepTime;

	CppSQLite3DB		  m_db;

	//保存数据库里面所有交易账户的ID，转换关系
	std::multimap<BrokerAccountKey, SOrderTransfer>	m_mapAccountID2OrderTransfer;
	CReadWriteLock								m_mutex_mapAccountID2OrderTransfer;

	void SetOrderTransferByAccount(std::vector<SOrderTransfer>& vecOrderTransfer);
	void InsertOrderRef(BrokerAccountKey& BAKey, CCTPAccount*& pCtpAccount);


	virtual void QueryQuot(std::map<std::string, int>& mapInstrumentID);

};
