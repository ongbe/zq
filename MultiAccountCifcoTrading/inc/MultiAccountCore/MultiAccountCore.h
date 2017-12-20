#pragma once
#include "../MAServer/include/MAAccount/MultiAccountCmd.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
#define MA_CORE()  (CMultiAccountCore::GetInstance())
typedef struct tagOrderInsertUIItem
{
	int						ID;
	CommonAccountType		Account;			//帐号
	CommonInstrumentIDType  InstrumentID;		//合约
	bool                    IsBuy;				//买卖
	bool                    IsHedge;			//投保
	bool					IsMarket;			//是否为市价
	bool					IsMarket_Trigger;	//触发价是否为市价
	double					Price;				//价格
	double					TriggerPrice;		//触发价格
	double					VolumeFlag;			//手数flag
	int						Volume;				//手数
	int						OCMode;				//开平, 0 开仓, 1 平今, 2 平仓
}OrderInsertUIItem;
typedef enum tagStrategyItem
{
	Strategy_None=-1,
	Strategy_Assign,//下单板指定手数
	Strategy_Base,//基数乘数方式
	Strategy_Total,//总数分配方式
	Strategy_FundDeviation,//按资金向首帐户对齐
	Strategy_AccountSize,//按帐户规模分配手数
	//Strategy_AccountAuto,//多帐户自动开平
	Strategy_UserDefine,//策略板自定义手数方式
}StrategyItem;
typedef void (*SETUICALLBACK)(bool bAcc);
class CMultiAccountCore
{
public:
	static CMultiAccountCore* GetInstance();
	static void DestroyInstance();	
	static CMultiAccountCore* m_pInstance;
public:
	//下单策略获取
	StrategyItem GetStrategyNum();
	void SetStrategyNum(StrategyItem it);
	bool GetOrderInsertStrategy(vector<OrderInsertUIItem>& dest,
								const OrderInsertUIItem& src,
								StrategyItem stra=Strategy_None,
								map<string,bool>* accstatus=NULL);
	int	GetAccountStatus(map<string,bool>* accstatus);
	int	SetAccountStatus(map<string,bool>* accstatus);
	wxArrayString GetAccountArray();
public:
	int GetOrderStrategyData(OrderStrategy eStrategy, const std::string strAccount,
		std::string strInstrument, std::string strProduct);
public:

	void AddTradeAccount(CTPTradeAccount& trader);
	void EraseTradeAccount(wxString acc,bool bOnlyContent);
	void ClearAllTradeAccount();
	void SetTradeAccount(vector<CTPTradeAccount>& vec);
	void SetTradeAccount(int index,const CTPTradeAccount& pAcc);
	void SetTradeAccountUseQuot(int index,bool UseQuot);
	void SetTradeAccountStrategyValid(int index,bool b);
	void SetTradeAccountStatusText(int index,wxString text);
	int FindTradeAccount(wxString acc) const;
	int GetTradeAccountCount() const;
	const CTPTradeAccount* GetTradeAccount(int index) const;
	string GetTradeAccountName(int index) const;
	int GetNextTradeAccountId() const;
	void SetLoadTypeAuto(int b);
	void SetSaveTypeAuto(int b);
	void SetLoadTypeManual(int b);
	void SetSaveTypeManual(int b);
	int GetLoadTypeAuto();
	int GetSaveTypeAuto();
	int GetLoadTypeManual();
	int GetSaveTypeManual();
public:
	void SaveDataToSvr(bool HasPwd,wxWindow* win);
	void SaveDataToLocal(bool HasPwd,map<int,vector<TradeAccount>>* const pContent=NULL);
	void LoadFromSvr(wxWindow* win);
	void LoadFromLocal(map<int,vector<TradeAccount>>* pRtn=NULL);
	void DelLocal();
	void DelServer();
public:
    void SetUICallback(SETUICALLBACK func){m_uicallback=func;}
	set<int>			m_vecReqId;
	LoginRsp  m_pLoginRsp;

	vector<BrokerInfo>   m_vecBrokerInfo;
	multimap<int,ServerGroup> m_mapServerGroup;
	multimap<int,ServerAddr>	m_mapServerAddr;
protected:
	MULTIACCOUNT*  m_pMACfg;
	SETUICALLBACK m_uicallback;	
	vector<CTPTradeAccount> m_vecTradeAccount;
private:
	CMultiAccountCore(void);
	virtual ~CMultiAccountCore(void);
	double GetAccountAlignOrAccountSizeData( IPlatformSingleSvr* pSvr, int nFirstAccountAlign );
};
