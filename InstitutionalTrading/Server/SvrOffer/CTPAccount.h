#pragma once
#include "MsgQueue.h"
class CToolThread;
class CTradeDataMsgQueue;

struct SMarginKey
{
	std::string strInstrument;	//合约名称
	std::string strHedgeFlag;	//投机套保标志
	
	bool operator< (const SMarginKey& r) const
	{
		int irlt;
		irlt=strncmp(strInstrument.c_str(),r.strInstrument.c_str(),sizeof(strInstrument));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strHedgeFlag.c_str(),r.strHedgeFlag.c_str(),sizeof(strHedgeFlag));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}
};
struct STransfer//下单的时候用到
{
	std::string strBrokerID;//经纪公司，只对账户有效，对交易员无效
	std::string strID;//交易员ID或者用户ID
	InputOrderKey key;
	int						UserForceClose;	//用户强平标志 风控客户端强平时候为1，不然为0
	char			        ForceCloseReason; //报单类型   风控客户端强平时候为THOST_FTDC_FCC_ForceClose,不然为‘0’
	STransfer()
	{
		
		strBrokerID = "";
		strID = "";
		key.nFrontID = 0;
		key.nSessionID = 0;
		memset(key.szOrderRef, 0, sizeof(key.szOrderRef));
		ForceCloseReason = '0';
		UserForceClose   = 0;
	}
	bool operator< (const STransfer& r) const
	{
		int irlt;

		irlt=strncmp(strBrokerID.c_str(),r.strBrokerID.c_str(),sizeof(strBrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;


		irlt=strncmp(strID.c_str(),r.strID.c_str(),sizeof(strID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		if(UserForceClose < r.UserForceClose)
			return true;
		else if(UserForceClose > r.UserForceClose)
			return false;

		if(ForceCloseReason < r.ForceCloseReason)
			return true;
		else if(ForceCloseReason > r.ForceCloseReason)
			return false;

		if(key < r.key)
			return true;
		else return false;



		return false;
	}
};
struct SOrderRef//撤单的时候用到
{
	std::string strBrokerID;//经纪公司，只对账户有效，对交易员无效
	std::string strID;//交易员ID或者用户ID
	int  nOrderRef;	
	SOrderRef()
	{
		memset(this, 0, sizeof(SOrderRef));
	}
	bool operator< (const SOrderRef& r) const
	{
		int irlt;
		irlt=strncmp(strBrokerID.c_str(),r.strBrokerID.c_str(),sizeof(strBrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strID.c_str(),r.strID.c_str(),sizeof(strID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		if(nOrderRef < r.nOrderRef)
			return true;
		else if(nOrderRef > r.nOrderRef)
			return false;

	
		return false;
	}
};
struct SOrderSysIDRef//撤单的时候用到
{
	std::string strExchanggeID;//交易所ID
	std::string strOrderSysID;//报单编号	
	SOrderSysIDRef()
	{
		memset(this, 0, sizeof(SOrderSysIDRef));
	}
	bool operator< (const SOrderSysIDRef& r) const
	{
		int irlt;
		irlt=strncmp(strExchanggeID.c_str(),r.strExchanggeID.c_str(),sizeof(strExchanggeID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strOrderSysID.c_str(),r.strOrderSysID.c_str(),sizeof(strOrderSysID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}
};
class CCTPAccount
{
public:
	CCTPAccount(void);
	~CCTPAccount(void);
	void Init(SLogin& login, int nAddrType, THOST_TE_RESUME_TYPE restartType);//nAddrType: 0 普通地址1 结算地址
	void InitQueryThread();
	void QueryInstruments();
	void QueryCommission(std::string strInstrument);
	void QueryMargin(std::string strInstrument);
	void QueryOrder();
	void QueryTrade();
	void QueryFund();
	void QueryPosition();
	void QueryPositionDetail();

	bool ProcessResultFail(STradeQueryData& data, std::string strBrokerID, std::string strAccountID);
	bool ProcessResult(STradeQueryData& data, std::string strBrokerID, std::string strAccountID,bool bQueryCommission, bool bQueryMargin);
	
	void GetLogin(SLogin& login);
	void SetAccountBaseInfo(std::string	strBrokerID, std::string strUserID, std::string	 strPassword);
	void SetTrader(CThostFtdcTraderApi* pTraderApi, CUserTraderSpi* pTraderSpi);
	void SetCurrentOrderref(int nOrderRef);
	void SetFrontID(int nFrontID);
	void SetSessionID(int nSessionID);

	int GetCurrentOrderref();
	int GetFrontID();
	int GetSessionID();

	void GetAccountBaseInfo(std::string&	strBrokerID, std::string& strUserID, std::string& strPassword);
	CThostFtdcTraderApi* GetTraderApi();
	CUserTraderSpi*		 GetTraderSpi();

	void SetAccountStatus(EnumAccountStatus	enumAccountStatus);
	EnumAccountStatus GetAccountStatus();

	void Release();

	//设置登录次数
	void SetLoginCount(int nLoginCount);
	//得到登录次数
	int  GetLoginCount();

	//保证金率的查询
	void SetMarginQuery(std::map<std::string, int>& mapInstrument);
	//得到下一个要查询合约保证金率的合约名称
	bool GetNextInstrumentOfMargin(std::string& strInstrument, std::string& strHedgeFlag);
	//合约查到
	bool FinishedQueryInstrumentMargin(std::string strInstrument, std::string strHedgeFlag);

	//手续费率的查询所有合约map
	void SetCommissionQuery(std::map<std::string, int>& mapInstrument);
	
	//得到下一个要查询手续费率的合约名称	
	bool GetNextInstrumentOfCommission(std::string& strInstrument);

	//合约查到
	bool FinishedQueryInstrumentCommission(std::string strInstrument);

	//按品种返回手续费率，则一个品种查询一次就够了，多余的合约删除
	void DeleteInstrumrntOfComminssion(std::string& strInstrument);

	//增加一条客户端（FrontID,SessionID,OrderRef）到ctp（FrontID,SessionID,OrderRef）的转换关系
	void InsertCleint2CTPKeyTransfer(STransfer keyClient, STransfer keyCTP);
    //根据客户端的ID（FrontID,SessionID,OrderRef）找到对应的 CTP相关的（FrontID,SessionID,OrderRef）
	bool GetCleint2CTPKeyTransfer(STransfer keyClient, STransfer& keyCTP);

	//增加一条ctp（FrontID,SessionID,OrderRef）到客户端（FrontID,SessionID,OrderRef）的转换关系
	void InsertCTP2CleintKeyTransfer(STransfer keyCTP, STransfer keyClient);
	//根据 CTP（FrontID,SessionID,OrderRef）找到对应的 CTP客户端的ID相关的（FrontID,SessionID,OrderRef）
	bool GetCTP2CleintKeyTransfer(STransfer keyCTP, STransfer& keyClient);

	//插入一条客户端orderref和ctporderref之间的关系
	void InsertCtp2ClientOrderRef(SOrderRef CtpOrderRef, STransfer ClientOrderRef);

	bool GetClientOrderRef(SOrderRef CtpOrderRef, STransfer& ClientOrderRef);

	//插入一条ordersysid
	void Insertordersysid2ClientOrderRef(SOrderSysIDRef orderSysIDRef, STransfer ClientOrderRef);

	bool GetRefByOrderSysID(SOrderSysIDRef orderSysIDRef, STransfer& ClientOrderRef);

	//是否只负责查询费率
	void SetIsQueryRatio(bool bOnlyQueryRatio);

	//是否只负责查询费率
	bool GetIsQueryRatio();

	//下单
	int ReqOrderInsert_Account(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);
	//撤单
	int ReqOrderAction_Account(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);

	//根据交易员账号得到委托交易账号
	bool GetTradeAccountByUserID(int nTraderID, std::string& strBrokerID, std::string& strInvestID);



	//已经查到费率如果以品种返回的不查第二遍
	void InsertCommissionProductID(std::string strProductID);
	bool IsCommissionProductIDQuery(std::string strProductID);

	void InsertCommissionProductID_Main(BrokerAccountKey& BAKey, std::string strProductID);
	bool IsCommissionProductIDQuery_Main(BrokerAccountKey& BAKey, std::string strProductID);

public:
	#ifdef  SIMU//模拟交易相关的都在这个宏里面
		void Init_simu(SLogin& login);
		//模拟下单
		int ReqOrderInsert_SimuAccount(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);
		//模拟撤单
		int ReqOrderAction_SimuAccount(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);

		void ProcessOrder(CThostFtdcOrderField& order);
		void ProcessTrade(CThostFtdcTradeField& trade);
		void OnRspQryOrder(CThostFtdcOrderField *pOrder);
		void OnRspQryTrade(CThostFtdcTradeField *pTrade);
	#endif

private:
	SLogin									m_login;
	// 会员代码
	std::string								m_strBrokerID;
	// 交易用户代码		
	std::string								m_strUserID;
	// 交易用户代码
	std::string								m_strPassword;	

	CReadWriteLock							m_Mutex_BrokerID;

	//交易API
	CThostFtdcTraderApi *					m_pTraderApi;   
	CReadWriteLock							m_Mutex_TraderApi;

	//交易SPI
	CUserTraderSpi *						m_pTraderSpi;  
	CReadWriteLock							m_Mutex_TraderSpi;

	//登录的状态
	EnumAccountStatus						m_enumAccountStatus;
	CReadWriteLock							m_Mutex_AccountStatus;

	//登录次数
	int										m_nLoginInCount;
	CReadWriteLock							m_Mutex_loginInCount;

	//当前可用的OrderRef
	int										m_nCurrentOrderref;
	CReadWriteLock							m_Mutex_nCurrentOrderref;

	//CTP返回的FrontID
	int										m_nCTPFrontID;           
	CReadWriteLock							m_Mutex_nCTPFrontID;

	//CTP返回的SessionID
	int										m_nCTPSessionID;
	CReadWriteLock							m_Mutex_nCTPSessionID;
	
	//client 到 ctp Keytransfer
	std::map<STransfer, STransfer>			m_Cleint2CTPKeyTransfer;	
	CReadWriteLock							m_Mutex_Cleint2CTPKeyTransfer;

	//ctp 到 client  Keytransfer
	std::map<STransfer, STransfer>			m_CTP2ClientKeyTransfer;	
	CReadWriteLock							m_Mutex_CTP2ClientKeyTransfer;

	
	//ctp orderref to client orderref
	std::map<SOrderRef, STransfer>			m_CtpOrderref2Client;
	CReadWriteLock							m_Mutex_CtpOrderref2Client;

	//ctp ordersysid to client orderref
	std::map<SOrderSysIDRef, STransfer>			m_CtpOrderSysID2Client;
	CReadWriteLock							   m_Mutex_CtpOrderSysID2Client;


	//合约保证金率的查询记分板
	std::map<SMarginKey, int>				m_mapInstrumentMargin;
	CReadWriteLock							m_Mutex_mapInstrumentMargin;

	//手续费率的查询记分板
	std::map<std::string, int>				m_mapInstrumentCommission;
	CReadWriteLock							m_Mutex_mapInstrumentCommission;

	//合约以品种返回的，只查一次费率
	std::set<std::string>				    m_setCommissionProductID;
	CReadWriteLock							m_Mutex_setCommissionProductID;

	//
	CToolThread*		m_QueryMgrThread;
	static DWORD WINAPI QueryThread(PVOID pParam);
	

	CToolThread*		m_QueryRatioMgrThread;
	static DWORD WINAPI QueryRatioThread(PVOID pParam);

public:
	STradeQueryData		m_dataCurrent;     //最后一个查询的内容
	bool				m_bQueryCommission;//有查询费率
	bool				m_bQueryMargin;    //有查询保证金
	CCTPAccount*		m_LastAccount;     //最后一个查询的账户
	CReadWriteLock		m_Mutex_dataCurrent;
	void SetCurrentQueryData(STradeQueryData	dataCurrent, CCTPAccount* pCtpAccount);
	void GetCurrentQueryData(STradeQueryData&	dataCurrent, CCTPAccount*& pCtpAccount, bool&	bQueryCommission, bool& bQueryMargin);

	void SetQueryConnectFail();//断开后需要返回任务失败
public:		
	CTradeDataMsgQueue* m_pTradeDataMsgQueue;
	void				InitQueryRatioThread();
	DWORD				ProcessQueryCommission();
	DWORD				ProcessQueryMargin();
	bool				m_bOnlyQueryRatio;//该账户如果只是查费率用的，则查完后，退出ctp连接
	int					m_nQueryMargin;   // 查询保证金率 0， 初始化，1,开始查询，2：查完
	CReadWriteLock		m_Mutex_QueryMargin;
	int					m_nQueryCommission;// 查询合约手续费率 0， 初始化，1,开始查询，2：查完
	CReadWriteLock		m_Mutex_QueryCommission;
	int					GetQueryMargin();
	void				SetQueryMargin();
	int					GetQueryCommission();	
	void				SetQueryCommission();


	DWORD				m_LastQryTime;			//最后查询时间
	bool				m_bInQry;				//有没有在途查询
	bool				IsQryTime(void);		
	void				UpdateQryTime(void);
	void				ClearbInQry() ;
	bool				SendQryCmd(STradeQueryData& data, int& ReqRlt, CCTPAccount* pCtpMainAccount);


	
};