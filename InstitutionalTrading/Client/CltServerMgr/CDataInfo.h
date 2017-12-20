#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "TcpLayer.h"
#include "popmsgwin.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#define SELECT_ALL "全部"
#define STATUS_LOGIN "已登录"
#define STATUS_LOGINOUT "未登录"

enum MODULE_NAME
{
    FINANCE_PRODUCT,
    TRADE_STRATEGY,
    TRADER_ACCOUNT
};

enum eUserType
{
    eManagerType,   //管理员
    eRiskerType     //风控员
};

typedef std::pair<int, int> Key;

class CDataInfo
{
public:
	enum {	
		conInitQueryOrgan,				// 初始查询组织机构
		conInitQueryUser,				// 初始查询用户
		conInitQueryOrganUserRelate,	// 初始查询用户与组织关系
		conInitItemCount				// 查询项数量
	};

private:
	CDataInfo();
	~CDataInfo();

public:
	static CDataInfo* NewInstance();
	static void DestroyInstance();
	static CDataInfo* m_pInstance;

public:
	void Clear();

    //登录
    void SetLogonInfo(const sLoginRsp& loginRsp);
    void GetLogonInfo(sLoginRsp& loginRsp);


    //组织机构
    void ClearOrganization();
    void SetOrganization(std::map<int, Organization>& mapOrganization);
    void SetOrganization(const Organization* pOrganization);
    void DelOrganization(int nOrgID);
    void GetOrganization(std::map<int, std::map<int, Organization>>& mapAssetMgmtOrgTree);
    bool GetOrganizationByID(int nAssetMgmtOrgID, Organization& sOrg);
    int  GetOrganizationIDByName(std::string& sName);
    string  GetOrganizationNameByID(int nOrgID);
    int  GetOrganizationScaleLevelByID(int nOrgID);
    bool IsValidOfOrganizationName(const string& sName);

    //用户
    void ClearUserInfo();
    void SetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void GetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void AddUserInfo(const UserInfo* pUserInfo);
    void DelUserInfo(int nUserID);
    bool GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo);
    bool GetUserInfoByAccount(string szUserAccount,UserInfo& sUserInfo);
    int GetUserIDByAccount(string szUserAccount);
    int IsValidOfUserAccount(std::string strAccount, UserType nType);
    bool IsTraderTypeByUserID(int nUserID);
    void SetUserStatus(int nUserID);

    //角色
    void ClearRole();
    void SetRole(std::map<int, RoleInfo>& mapRole);
    void SetRole(const RoleInfo& oRole);
    void GetRole(std::map<int, RoleInfo>& mapRole);
    bool GetRoleInfoByID(int nRoleID,RoleInfo& oRoleInfo);
    string GetRoleNameByID(int nRoleID);
    int GetRoleIDByName(string& szName);
    void DelRole(int nRoleID);
    bool IsValidOfRoleName(std::string strName);

    //用户与角色关系
    void ClearUserRoleRelation();
    void ClearUserRoleRelationByUserID(int nUserID);
    void SetUserRoleRelation(const UserRole* pUserRole);
    void SetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetUserType2RoleRelation(std::map<int,std::vector<RoleInfo>>& mapUserType2Role);
    void GetRoleByUserID(int nUserID,std::vector<UserRole>& vecRole);
    string GetRoleByUserID(int nUserID);

    //用户与组织机构关系
    void ClearUserAndOrgRelation();
    void DelUserAndOrgRelation(int nID, int nUserType);
    void SetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    void SetUserAndOrgListenRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgListenRelation);
    void InitUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void AddUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void SetUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    string GetOrgNameByUserID(int nUserID);

    void GetUserInfosByOrgID(int nOrgID, UserType nUserType, vector<UserInfo>& vecUserInfo, bool bSubOrg);
	void GetUserInfosBySubOrgID(UserType nUserType, map<int, map<int, Organization>>& mapMain, map<int, Organization>& mapSub, vector<UserInfo>& vecUserInfo, bool bSubOrg);
	bool GetDealerByOrganID(int nOrganID, set<int>& setDealerID);

	//权限    
    void ClearPrivilege();
    void SetPrivilege(std::map<int,Privilege>& mapPrivilege);
    void GetPrivilege(std::map<int,Privilege>& mapPrivilege);
    void SetUserType2Privilege(std::map<int,std::vector<Privilege>>& mapUserType2Privilege);
    void SetParentID2Privilege(std::map<int,std::vector<Privilege>>& mapParentID2Privilege);
    void GetUserType2Privilege(std::map<int,std::vector<Privilege>>& mapUserType2Privilege);
    void GetParentID2Privilege(std::map<int,std::vector<Privilege>>& mapParentID2Privilege);

    //角色和权限关系
    void ClearRolePrivilegeRelation();
    void SetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2Privilege);
    void SetRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID);
    void GetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2Privilege);

	//经纪公司
    void ClearBrokerInfos();
	void SetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
	void SetBrokerInfo(BrokerInfo& brokerInfo);
    void SetBrokerInfoQuot(int nBrokerID, const string& szAccount, const string& szPWD);
	void DelBrokerInfo(int nBrokerID);    
	void GetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
    int  GetBrokerIDByName(std::string szName);
    std::string GetBrokerCodeByID(int brokerID);
    std::string GetBrokerNameByID(int brokerID);
    int  GetBrokerIDByCode(const string& szCode);
    bool IsValidOfBrokerCode(std::string strBrokerCode);
    bool IsQuotBrokerByID(int nBrokerID);


    //服务器组
    void ClearServerGroups();
	void SetServerGroups(std::map<int,std::map<int,ServerGroup>>& mapServerGroup);
	void GetServerGroups(int nBrokerID, std::map<int,ServerGroup>& mapServerGroup);
    bool GetServerGroupByID(int nBrokerID, int nServerID, ServerGroup& oServerGroup);
    std::string GetServerGroupNameByID(int nBrokerID,int nServerGroupID);
    int  GetServerGroupID(int nBrokerID, std::string szServerName);
	void SetServerGroup(int nBrokerID, ServerGroup& serverGroup);
	void DelServerGroup(int nBrokerID, int nServerID);
    bool IsValidOfServerName(int nBrokerID, std::string strServerGroup);
    bool IsValidOfSettle(int nBrokerID,bool bChecked);


    //服务器组地址
	void SetServerAddress(std::map<int, std::map<int, ServerAddress>>& mapServerAddr);
	void SetServerAddress(int nServerID, ServerAddress& addr);
	void DelServerAddress(int nServerID, int nAddrID);
	void GetServerAddress(int nServerID, std::map<int, ServerAddress>& mapServerAddr);
    bool GetServerAddrByID(int nServerID, int nAddrID, ServerAddress& oServerAddr);

    //行情服务器组地址
    void ClearServerAddressQuot();
    void SetServerAddressQuot(std::map<int, ServerAddressQuot>& mapServerAddr);
    void SetServerAddressQuot(ServerAddressQuot& addr);
    void DelServerAddressQuot(int nAddrID);
    void GetServerAddressQuot(std::map<int, ServerAddressQuot>& mapServerAddr);
    bool GetServerAddrQuotByID(int nAddrID, ServerAddressQuot& oServerAddr);


    //委托交易账号
    void ClearTradeAccount();
    void SetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    void SetTradeAccount(const TradeAccount* pTradeAccount);
    void DelTradeAccount(int nTradeAccountID);
    void GetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    bool GetTradeAccountByAccount(string& sAccount,TradeAccount& oTradeAccount);
    string GetTradeAccountByID(int nTradeAccountID);
    int  GetBrokerIDByAccountID(int nTradeAccountID);
    int GetTradeAccountIDByAccount(string szTradeAccount);
    bool IsValidOfTradeAccount(std::string strTradeAccount);



    //用户与委托交易账号
    void ClearUserAndTraderAccountRelation();
    void SetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);
    void AddUserAndTraderAccountRelation(const UserAndTradeAccountRelation* pRelation);
    void GetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);

    //组织机构和委托交易账号
    void ClearTradeAccountAndOrgRelation();
    void SetTradeAccountAndOrgRelation(const TradeAccountAndOrg& relation);
    void SetTradeAccountAndOrgRelation(std::map<int, TradeAccountAndOrg>& mapTradeAccountAndOrgRelation);
    string GetOrgByTradeAccountID(const int nTradeAccountID);


    //理财产品;
    void ClearFinanceProduct(); 
    void SetFinancialProduct(const FinancialProduct *pFP); 
    void GetFinancialProduct(std::map<int, FinancialProduct>& mapFinancialProduct);
    bool GetFinancialProduct(int nFinanceProductID, FinancialProduct& oFinancialProduct);
    string GetFinancialProductNameByID(int nProductID);
    int  GetFinancialProductIDByName(string& sName); 
    void DeleteFinanceProduct(int nProductID);
    bool IsValidOfProductName(const string& sName);  

    //理财产品与交易员的关系
    void ClearProductTraderRelation();
    void ClearProductTraderRelation(int nType, int nID);
    void SetProductTraderRelation(const TraderProRelation *pRelate); 
    void GetProTraderRelation(std::vector<TraderProRelation>& vec);
    void GetProductByTraderID(int nTraderID,std::set<int>& vec);
    string GetProductByUserID(int nUserID);
    
    //交易策略
    void ClearTradeStrategy();
    void SetTradeStrategy(const TradeStrategy *pStrategy);
    void DeleteTradeStrategy(int nTradeStrategyID);    
    void GetTradeStrategy(std::map<int, TradeStrategy>&	mapTradeStrategy);
    bool GetTradeStrategyByID(int nTradeStrategyID, TradeStrategy& oStrategy);
    bool GetTradeStrategyByName(TradeStrategy& oStrategy,string& sName); 
    int  GetTradeStrategyIDByName(string& sName); 
    bool IsValidOfStrategyName(const string& sName);

    void SetAccountStatus(const sAccountStatus& sAS);
    EnumAccountStatus GetAccountStatus(const string& sAccount);

    //交易策略与组织机构的关系
    void ClearStrategyOrgRelation();
    void SetStrategyOrgRelation(std::map<int, std::vector<int>>& mapRelate);
    void SetStrategyOrgRelation(int nStrategyID, std::vector<int>& vecOrgID);
    void GetStrategyOrgRelation(std::vector<AssetOrgStrategyRelation>& vec);

    //出入金
    void ClearFundInOut();
    void SetFundInOut(sFundInOut& oFundInOut);
    void GetStrategyOrgRelation(std::vector<sFundInOut>& vecFundInOut);

	//结算
	void SetOriginalSettlementPrice(std::map<std::string,SettlementPriceField>&  lMapFiled);
	void SetCommissionMarginRate(std::map<RateKey,CommissionMarginRate>& lMapFiled );
	void ClearCommissionMarginRate();
	void GetAllProducts(set<string>& setProductID, int exchange); //得到合约品种
    const string ExchangeID2String(const string& ExchangeID);
	const string ExchangeTabIndex2String(const int& ExchangeID);
	bool GetSettlementPrice(std::vector<SettlementPriceField>& data, const string& ProductID);
	double GetSettlementPrice(const string& InstrumentID);
	double GetLastSettlementPrice(const string& InstrumentID);
	bool ModifySettlementPrice(SettlementPriceField& Filed);
	bool ModifyLocalPrice(SettlementPriceField& Filed);

	bool CheckSettlePrice();
	void SetInstrumentInfo(std::map<std::string, PlatformStru_InstrumentInfo>& lMapFiled );
	bool GetCommissionMarginRate(std::vector<CommissionMarginRate>& vecRate ,int exchandeID,const std::string& AccountID,const std::string& BrokerID);
	bool GetCommissionMarginRate(std::vector<CommissionMarginRate>& vecRate);
    string GetExchangeID(string InstrumentID);
	bool GetBrokerIDByBrokerCode(std::string szName,int& brokerID);
	bool GetCommissionMarginRate(RateKey& key, CommissionMarginRate& Filed);
	void SetModifiedRate(const CommissionMarginRate *pFiled);
	void GetModifiedRate(std::map<RateKey,CommissionMarginRate>&  lMapFiled);
	void ModifyCommissionMarginRate(CommissionMarginRate& Field );
	void ModifyCommissionMarginRate();
	void CDataInfo::ClearModifiedRate();
	bool CheckRate();
	const string ExchangeName2ID( const string& name);
	void SetTrade(std::map<TradeKey,PlatformStru_TradeInfo>& lMapFiled );
	void ClearTrade();
	bool HaveModifiedRate();
	void ModifyTrade(PlatformStru_TradeInfo&  originaldata, PlatformStru_TradeInfo&  newdata);
	void AddTrade(PlatformStru_TradeInfo& field);
	void DeleteTrade(PlatformStru_TradeInfo& field);
	bool GetInstrumentFieldByProductID(string strID, vector<PlatformStru_InstrumentInfo>& setInstrumentField);
	void GetAllProducts(set<string>& setProductID ); //得到合约品种
	void GetTrade(std::vector<PlatformStru_TradeInfo> &vecTrade,const std::string& UserID);
	const char* Direction2String(char param);
	const char* OffsetFlag2String(char param);
	const char* HedgeFlag2String(char param);
	//const char* TradeType2String(char TradeType);
	void SetPositionDetail(std::multimap<std::string,PlatformStru_PositionDetail>& lMapFiled );
	void ClearPositionDetail();
	void SetPosition(std::multimap<std::string,PlatformStru_Position>& lMapFiled );
	void ClearPosition();
	void GetPositionDetail(std::vector<PlatformStru_PositionDetail>& vecField );
	void GetPosition(std::vector<PlatformStru_Position>& vecField );
	void SetFund(std::map<std::string,PlatformStru_TradingAccountInfo>& lMapFiled );
	void SetFund(PlatformStru_TradingAccountInfo& Filed );
	void ClearFund();
	void SetModifiedFund(PlatformStru_TradingAccountInfo& Field);
	void GteModifiedFund(std::map<std::string,PlatformStru_TradingAccountInfo>& lMapFiled );
	void ClearModifiedFund();
	void GetFund(std::vector<PlatformStru_TradingAccountInfo>& vecField ,const std::string& UserID );
	const char HedgeFlag2ID( const string& name);
	const char Direction2ID( const string& name);
	bool IsExitIns(const string& Ins);
	bool GetInstrunmentInfobyID(const string& Ins, PlatformStru_InstrumentInfo& field);
	bool GetTradeAccountByUserID(std::string& strUserID, std::string& strBrokerID, std::string& strInvestID);
	bool GetRateByKey(RateKey& key,CommissionMarginRate& rate);
	bool GetPositionDetail(const string& userID,const string& traderID,PlatformStru_PositionDetail& field);
	void SetPositionDetail(const PlatformStru_PositionDetail *pField);
	const char* TradeDirection2String(char param);
	const char TradeDirection2ID( const string& name);
	const char* TradeType2String(char TradeType);
	const char TradeType2ID(const string& name);
	const char OffsetFlag2ID(const string& name);
	bool IsExitTradeID(TradeKey& key);
	bool GetTradeInfo(TradeKey& key, PlatformStru_TradeInfo& field);
	void SetTradeInfo(const PlatformStru_TradeInfo *pField);
	bool GetFund( PlatformStru_TradingAccountInfo&Filed,const std::string& UserID);

    void SetTradeDataStatus(eTradeDataStatus tradeDataStatus);
    eTradeDataStatus GetTradeDataStatus();
	bool IsCombination(string InstrumentID);//结算费率确认事，如果组合合约费率可以为空
	void GetQryRate(std::vector<RateKey>& vec,bool IsAll);
	void SetQryRate();
	void SetCommissionMarginRate(RateKey& key,CommissionMarginRate& Filed );
	void SaveSortedTrade(std::vector<PlatformStru_TradeInfo>& vecTrade);
	void GetSortedTrade(std::vector<PlatformStru_TradeInfo>& vecTrade);
	void SaveSortedPositionDetail(std::vector<PlatformStru_PositionDetail>& vecPD);
	void GetSortedPositionDetail(std::vector<PlatformStru_PositionDetail>& vecPD);
	bool CompareSettlePrice(string & InsName);

protected:
	void ResetDealerOrganRelationMap();
	void ResetDealerOrganRelation(const UserAndOrgRelation* pRelation);
	void DelDealerOrganRelation(int nDealerID, int nOrganID);

public:

    bool m_bRoleLoad;
    bool m_bPrivilegeLoad;
    bool m_bRolePrivilegeRelationLoad;
    bool m_bUserRoleRelationLoad;

private:	

    CPopMsgWin  *m_PopMsgWin;//消息反馈信息窗口

    sLoginRsp m_loginRsp;	//账户信息

    std::map<int, std::map<int, Organization>>      m_mapAssetMgmtOrgTree;//一个组织机构ID下的所有组织机构信息
	std::map<int, Organization>	                    m_mapOrganization;//组织机构
    std::map<int, UserInfo>                         m_mapUserInfo;//用户   
    std::map<int, RoleInfo>                         m_mapRoleInfo;//角色   
    std::map<int, std::vector<UserRole>>            m_mapUserRole;//用户与角色关系
	std::vector<UserAndOrgRelation>	                m_vecUserAndOrgRelation; //用户与组织机构关系
    std::map<int, UserAndTradeAccountRelation>      m_mapUserAndTradeAccountRelation;//用户与委托交易账户关系
    std::map<int, TradeAccountAndOrg>               m_mapTradeAccountAndOrgRelation;//组织机构与委托交易账户关系

    std::map<int, set<int>>							m_mapDealerOrganRelation;	//用户与组织机构的关系map


    std::map<int, Privilege>                        m_mapPrivilege;//权限
    std::map<int,std::vector<int>>                  m_mapRoleID2Privilege;//角色与权限关系
    std::map<int, std::vector<Privilege>>           m_mapUserType2Privilege;//用户类型与权限关系
    std::map<int, std::vector<Privilege>>           m_mapParentID2Privilege;//权限节点关系
	
	std::map<int, BrokerInfo>                       m_mapBrokerInfo;//经纪公司	
	std::map<int, std::map<int, ServerGroup>>       m_mapServerGroup;//服务器组	
	std::map<int, std::map<int, ServerAddress>>     m_mapServerAddr;//交易地址   
    std::map<int, ServerAddressQuot>                m_mapServerAddrQuot;//行情地址   
    std::map<int, TradeAccount>                     m_mapTradeAccount;//委托交易账户
    std::map<std::string,sAccountStatus>            m_mapAccount2Status;//委托交易账号模块保存账号实时登录状态

    
    std::map<int, FinancialProduct>	                m_mapFinancialProduct;//理财产品 
    std::map<int, std::vector<TraderProRelation>>   m_mapTraderProRelation;//理财产品与交易员关系 
    std::map<int, TradeStrategy>                    m_mapTradeStrategy; //交易策略              
    std::map<int, std::vector<int>>                 m_mapStrategyOrgRelation;//交易策略与组织机构的关系    

    std::vector<sFundInOut>                         m_vecFundInOut;//出入金

	std::map<std::string,SettlementPriceField>      m_mapLocalPrice;
	std::map<std::string,SettlementPriceField>      m_mapSettlePrice;
	std::map<RateKey,CommissionMarginRate>          m_mapCommissionMarginRate;
	std::map<std::string, PlatformStru_InstrumentInfo> m_mapInstrumentInfo;
	std::map<RateKey,CommissionMarginRate>          m_mapModifiedRate;
	std::map<TradeKey,PlatformStru_TradeInfo>       m_mapTrade;
	std::multimap<std::string,PlatformStru_PositionDetail>  m_mapPositionDetail;
	std::multimap<std::string,PlatformStru_Position>  m_mapPosition;
	std::map<std::string,PlatformStru_TradingAccountInfo>  m_mapFund;
	std::map<std::string,PlatformStru_TradingAccountInfo> m_mapModifyFund;
	std::vector<RateKey>  m_vecAllRate;
	std::vector<RateKey>  m_vecVacancyRate;
	std::vector<PlatformStru_TradeInfo>  m_vecSortedTrade;
	std::vector<PlatformStru_PositionDetail> m_vecSortedPositionDetail;

    eTradeDataStatus m_tradeDataStatus; //交易状态
	

};
