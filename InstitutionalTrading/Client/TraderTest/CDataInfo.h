#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>

enum MODULE_NAME
{
    FINANCE_PRODUCT,
    TRADE_STRATEGY,
    TRADER_ACCOUNT
};

class CDataInfo
{
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
    void SetAeestMgmtOrg(std::map<int, Organization>&   mapAssetMgmtOrg);
    void GetAssetMgmtOrg(std::map<int, std::map<int, Organization>>&	mapAssetMgmtOrgTree);
    bool GetAssetMgmtOrgByID(int nAssetMgmtOrgID, Organization& sOrg);
    int  GetAssetMgmtOrgIDByName(std::string& sName);
    string  GetAssetMgmtOrgNameByID(int nOrgID);


    //用户
    void ClearUserInfo();
    void SetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void GetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    bool GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo);

    //角色
    void ClearRole();
    void SetRole(std::map<int, RoleInfo>& mapRole);
    void GetRole(std::map<int, RoleInfo>& mapRole);
    string GetRoleNameByID(int nRoleID);

    //用户与角色关系
    void ClearUserRoleRelation();
    void SetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetRoleByUserID(int nUserID,std::vector<UserRole>& vecRole);

    //用户与组织机构关系
    void ClearUserAndOrgRelation();
    void SetUserAndOrgRelation(std::map<int, UserAndOrgRelation>& mapUserAndOrgRelation);
    void GetUserAndOrgRelation(std::map<int, UserAndOrgRelation>& mapUserAndOrgRelation);


	//权限
	void SetPrivilegeNodes(std::map<int, PrivilegeType>& mapPrivilegeNode);
	void GetPrivilegeNodes(std::map<int, PrivilegeType>& mapPrivilegeNode);
	void SetPrivilegeActions(std::map<int, PrivilegeAction>& mapPrivilegeAction);
	void GetPrivilegeActions(std::map<int, PrivilegeAction>& mapPrivilegeAction);
	void SetPrivilegeNames(std::map<int, RoleInfo>& mapPrivilegeName);
	void GetPrivilegeNames(std::map<int, RoleInfo>& mapPrivilegeName);
	void SetAllPrivilegeValues(std::map<int, std::map<int, Privilege>>& allPrivilegeValue);
	void GetPrivilegeValues(int nPrivilegeID, std::map<int, Privilege>& mapValues);
	void SetPrivilegeValues(int nPrivilegeID, std::map<int, Privilege>& mapValues);
	void UpdatePrivilegeNameType(int nPrivilegeID, int nUserType);
	void SetCurPrivilegeValues(int nUserType, std::map<int, Privilege>& mapValues);
	void GetCurPrivilegeValues(int& nUserType, std::map<int, Privilege>& mapValues);
	void SetPrivilegeName(RoleInfo& name);
	void DelPrivilegeName(int nPrivilegeID);
	void SetCurPrivilegeName(RoleInfo& name);
	RoleInfo GetCurPrivilegeName();


	//经纪公司
	void SetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
	void SetBrokerInfo(BrokerInfo& brokerInfo);
	void DelBrokerInfo(int nBrokerID);    
	void GetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
    bool GetBrokerIDByName(std::string szName,int& brokerID);
    bool GetBrokerNameByID(int brokerID,std::string& szName);


    //服务器组
    void SetServerGroupInfo(std::map<int,ServerGroup>& mapServerGroup);
	void SetServerGroups(std::map<int,std::map<int,ServerGroup>>& mapServerGroup);
	void GetServerGroups(int nBrokerID, std::map<int,ServerGroup>& mapServerGroup);
    bool GetServerGroupNameByGroupID(int nServerGroupID,std::string& szName);
    bool GetServerGroupIDByBrokerIDAndServerName(int nBrokerID,std::string szName,int& nServerGroupID);
	void SetServerGroup(int nBrokerID, ServerGroup& serverGroup);
	void DelServerGroup(int nBrokerID, int nServerID);


    //服务器组地址
	void SetServerAddress(std::map<int, std::map<int, ServerAddress>>& mapServerAddr);
	void SetServerAddress(int nServerID, ServerAddress& addr);
	void DelServerAddress(int nServerID, int nAddrID);
	void GetServerAddress(int nServerID, std::map<int, ServerAddress>& mapServerAddr);


    //委托交易账号
    void SetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    void SetTradeAccount(const TradeAccount* pTradeAccount);
    void DelTradeAccount(int nTradeAccountID);
    void GetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    bool GetTradeAccountByAccount(string& sAccount,TradeAccount& oTradeAccount);
    string GetTradeAccountByID(int nTradeAccountID);
    bool IsValidOfTradeAccount(std::string strTradeAccount);

    //用户与委托交易账号
    void ClearUserAndTraderAccountRelation();
    void SetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);
    void GetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);

    //理财产品;
    void ClearDelFinanceProduct();    
    void SetDelFinancialProduct(int nProductID);
    void SetFinancialProductStatus();
    void GetDelFinanceProduct(std::set<int>& setDel); 

    void ClearFinanceProuct(); 
    void SetFinancialProduct(const FinancialProduct *pFP); 
    void GetFinancialProduct(std::map<int, FinancialProduct>& mapFinancialProduct);
    string GetFinancialProductNameByID(int nProductID);
    int  GetFinancialProductIDByName(string& sName); 
    void AddFinancialProduct(const FinancialProduct *pFP);
    void ModifyFinancialProduct(const FinancialProduct *pFP);

    bool IsValidOfProductName(const string& sName);  

    //理财产品与交易员的关系
    void ClearProductTraderRelation();
    void SetProductTraderRelation(const TraderProRelation *pRelate); 
    void GetProTraderRelation(std::vector<TraderProRelation>& vec);
    void AddProductTraderRelation(const TraderProRelation *pRelate);
    void DeleteProductTraderRelation(int nProductID,bool bReal);
    void DeleteProductTraderRelation(const TraderProRelation *pRelate,bool bReal);
    
    //交易策略
    void SetTradeStrategy(const TradeStrategy *pStrategy);
    void DeleteTradeStrategyByID(int& id);    
    void GetTradeStrategy(std::map<int, TradeStrategy>&	mapTradeStrategy);
    bool GetTradeStrategyByID(int nTradeStrategyID, TradeStrategy& oStrategy);
    bool GetTradeStrategyByName(TradeStrategy& oStrategy,string& sName); 
    void ClearTradeStrategy();
    bool IsValidOfStrategyName(const string& sName);

    //交易策略与组织机构的关系
    void ClearOrgStrategyRelation();
    void SetOrgStrategyRelation(const AssetOrgStrategyRelation *pRelate);
    void GetOrgStrategyRelation(std::vector<AssetOrgStrategyRelation>& vec);
    void DeleteOrgStrategyRelationByID(int nStrategyID);

public:
    int  GetNextSeqNum(MODULE_NAME eName);

private:	

    sLoginRsp m_loginRsp;	//账户信息

	std::map<int, std::map<int, Organization>>     m_mapAssetMgmtOrgTree;	//组织机构
	std::map<int, Organization>	m_mapAssetMgmtOrg;    
    std::map<int, UserInfo>   m_mapUserInfo;//用户   
    std::map<int, RoleInfo>   m_mapRoleInfo;//角色   
    std::map<int, std::vector<UserRole>>   m_mapUserRole;//用户与角色关系
	std::map<int, UserAndOrgRelation>	m_mapUserAndOrgRelation; //用户与组织机构关系
    std::map<int, UserAndTradeAccountRelation> m_mapUserAndTradeAccountRelation;//用户与委托交易账户关系

	std::map<int, PrivilegeType>   m_mapNode; //key为NodeID
	std::map<int, PrivilegeAction>     m_mapAction;	//key为ActionID
	std::map<int, RoleInfo>   m_mapPrivilegeName;	//key为PrivilegeID

	std::map<int, std::map<int, Privilege>>   m_mapAllPrivilegeValue; //外层key为PrivilegeID，里层key为ActionID
	std::map<int, Privilege>  m_curPrivilegeValue;
	RoleInfo  m_curPrivilegeName;	//当前选中的权限名
	int m_nCurUserType;
	std::map<int, Privilege>  m_mapCurPrivilegeValue;	

	
	std::map<int, BrokerInfo>   m_mapBrokerInfo;//经纪公司	
    std::map<int, ServerGroup>  m_mapServerGroupInfo;
	std::map<int, std::map<int, ServerGroup>>   m_mapServerGroup;//服务器组	
	std::map<int, std::map<int, ServerAddress>>     m_mapServerAddr;//交易地址    
    std::map<int, TradeAccount>     m_mapTradeAccount;//委托交易账户
    
    std::set<int>                       m_setDelFinanceProduct;//本次删除的产品ID
    std::map<int, FinancialProduct>	    m_mapFinancialProduct;   //理财产品 
    std::map<Key,TraderProRelation>     m_mapTraderProRelation; //理财产品与交易员关系 
    std::map<int, TradeStrategy>        m_mapTradeStrategy; //交易策略              
    std::vector<AssetOrgStrategyRelation>   m_vecOrgStrategyRelation;//交易策略与组织机构的关系     


};
