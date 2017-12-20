#pragma once
#include "OfferMainInterface.h"
class CCTPAccount;
class CToolThread;

class COfferMain : public COfferMainInterface
{
public:
	COfferMain();
	virtual ~COfferMain(void);
	//初始化
	virtual  void Init(bool bSettlement, int nMorning);//0 为交易，1为结算

	//释放内存
	void Release();
	//判断所有账户是不是完成登录，三次登不上也算完成登录
	bool IsAllAcountFinishLoginIn();

    //查询所有合约
	void QueryInstruments();	
	void QueryCommission(BrokerAccountKey& BAKey);
	void QueryMargin(BrokerAccountKey& BAKey);
	void QueryOrder(BrokerAccountKey& BAKey);
	void QueryTrade(BrokerAccountKey& BAKey);
	void QueryFund(BrokerAccountKey& BAKey);
	void QueryPosition(BrokerAccountKey& BAKey);
	void QueryPositionDetail(BrokerAccountKey& BAKey);

	bool NeedQueryInstruments();
	void SetNeedQueryInstruments(bool bNeed);
	void StartQueryMargin(BrokerAccountKey& BAKey);
	void StartQueryCommission(BrokerAccountKey& BAKey);

	void RestartAccount(int nTradeAccount);

	//发送行情请求
	void QueryQuot(std::map<std::string, int>& mapInstrumentID);
	void QueryQuot();//给行情调用
	//查询回来的合约数组
	void AddInstrument(CThostFtdcInstrumentField *pInstrument, bool bIsLast);

	//从交易数据管理里面获取数据
	void AddInstrument();
	//得到下一个要查询合约保证金率的合约名称
	bool GetNextQueryInstrumentOfMargin(std::string strBrokerID, std::string strUser, std::string& strInstrument, std::string& strHedgeFlag);	

	//得到下一个要查询手续费率的合约名称	
	bool GetNextQueryInstrumentOfCommission(std::string strBrokerID, std::string strUser, std::string& strInstrument);

	//得到该品种下所有的合约
	void GetInstrumentsByProductID(std::string strProduct, std::vector<std::string>& vecInstruments);

	//得到该合约的品种
	bool GetProductIDByInstruments(std::string strInstrument, std::string& strProduct);
	
	//下单
	int ReqOrderInsert_Main(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);

	//撤单
	int ReqOrderAction_Main(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);	

	//得到交易账户指针
	bool GetCTPAccount(BrokerAccountKey BAKey, CCTPAccount*& pCtpAccount);

	//判断是不是合约已经查完
	bool GetFinishedQueryInstrument();



	//断开ctp连接
	void LogoffAccount(BrokerAccountKey BAKey);

	//定时检查账号ctp前端是否连接失败
	void CheckAccountsConnect();

	//如果连接上的断开
	bool IsCtpAccountTimerExit(CCTPAccount* pCtpAccount);
	void SetTimer2CTPAccount(UINT_PTR uPtr, CCTPAccount* pCtpAccount);
	bool GetCtpAccountByTime(UINT_PTR uPtr, CCTPAccount*& pCtpAccount);
	void RemoveCtpAccountByTime(UINT_PTR uPtr);
private:
	

	//查询数据库
	
	//void InsertOrderRef(BrokerAccountKey& BAKey, CCTPAccount*& pCtpAccount);

private:
//保存账号各种状态信息
	std::map<BrokerAccountKey, CCTPAccount*>		m_mapUserID2CTPAccount;
	CReadWriteLock							m_mutex_mapUserID2CTPAccount;

	std::multimap<BrokerAccountKey, CCTPAccount*>		m_mapUserID2CTPAccount_Ratio;
	CReadWriteLock									m_mutex_mapUserID2CTPAccount_Ratio;

	//所有合约的数组
	std::map<std::string, int>				m_mapInstrumentID;
	bool									m_bFinishedQueryInstrument;
	CReadWriteLock							m_mutex_mapInstrument;

	//产品到合约的映射，主要解决同品种费率不重复查询的问题
	std::multimap<std::string, std::string> m_mulmapProductID2InstrumentID;
	CReadWriteLock							m_mutex_mulmapProductID2InstrumentID;

	//合约到产品的映射
	std::map<std::string, std::string>		m_mapInstrumentID2ProductID;
	CReadWriteLock							m_mutex_mapInstrumentID2ProductID;

	//保存数据库里面所有交易账户的ID，转换关系
//	std::multimap<BrokerAccountKey, SOrderTransfer>	m_mapAccountID2OrderTransfer;
//	CReadWriteLock								m_mutex_mapAccountID2OrderTransfer;

	
	bool										m_bNeedQueryInstrument;
	CReadWriteLock								m_mutex_NeedQueryInstrument;

	

	std::map<UINT_PTR, CCTPAccount*>		   m_mapTimer2CTPAccount;
	CReadWriteLock							   m_mutex_mapTimer2CTPAccount; 



private:


};
