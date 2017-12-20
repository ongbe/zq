#pragma once
#include "CommonPkg.h"
#include "commonstruct.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "CommonStructEx.h"

#define SELECT_ALL "全部"
enum MODULE_NAME
{
    FINANCE_PRODUCT,
    TRADE_STRATEGY,
    TRADER_ACCOUNT
};

typedef std::pair<int, int> Key;
typedef std::multimap<int, StatisticsEntry> RiskMapStatisticMap;
typedef std::map<int, FinancialProduct>		 MAPFinancialProduct;
typedef std::map<int, std::map<int, Organization>>   MAPAeestMgmtOrganization;
typedef std::map<string, PlatformStru_InstrumentInfo> MAPInstrumentID2InstrumentField;
typedef std::multimap<string, PlatformStru_InstrumentInfo> MAPProductID2InstrumentField;
class CDataInfo
{
public:
	enum {	
		conInitQueryOrgan,				// 初始查询组织机构
		conInitQueryUser,				// 初始查询用户
		conInitQueryOrganUserRelate,	// 初始查询用户与组织关系
		conInitQueryFinical,			// 初始查询用户与组织关系
		conInitQueryFinicalUser	,		// 初始查询用户与组织关系
		conInitQueryInstrument	,		// 初始合约
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


	//获得所有品种代码
	void SetInstrumentField(PlatformStru_InstrumentInfo *pField);
	void GetAllProducts(set<string>& setProductID);

    //组织机构
    void ClearOrganization();
    void SetOrganization(std::map<int, Organization>& mapOrganization);
    void SetOrganization(const Organization* pOrganization);
  
    void GetOrganization(std::map<int, std::map<int, Organization>>& mapAssetMgmtOrgTree);
    bool GetOrganizationByID(int nAssetMgmtOrgID, Organization& sOrg);
    int  GetOrganizationIDByName(std::string& sName);
    string  GetOrganizationNameByID(int nOrgID);
    int  GetOrganizationScaleLevelByID(int nOrgID);
    bool IsValidOfOrganizationName(const string& sName);

    //用户
    void ClearUserInfo();
    void SetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void SetUserInfo(int nUserID, bool bAssign);
    void GetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void AddUserInfo(const UserInfo* pUserInfo);
    
    bool GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo);
    bool GetUserInfoByAccount(string szUserAccount,UserInfo& sUserInfo);
    int GetUserIDByAccount(string szUserAccount);
    int IsValidOfUserAccount(std::string strAccount, UserType nType);
    bool IsTraderTypeByUserID(int nUserID);
    void SetUserStatus(int nUserID);

    //用户与组织机构关系
    void ClearUserAndOrgRelation();
    void DelUserAndOrgRelation(int nID, int nUserType, int nRelationType);
    void ModifyUserAndOrgRelation(const UserAndOrgRelation& oRelation);
    void SetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    void SetUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    string GetOrgNameByUserID(int nUserID);

	bool GetDealerByOrganID(int nOrganID, set<int>& setDealerID);
	int GetTraderUserIDsByOrgID(int  nOrganID, bool bIncludeChildren, std::set<int>& setFPID);



   


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
    void GetTraderSetByProductID(int nProductID,std::vector<int>& vec);  
  

protected:
	void ResetDealerOrganRelationMap();
	void ResetDealerOrganRelation(const UserAndOrgRelation* pRelation);
	void DelDealerOrganRelation(int nDealerID, int nOrganID);

private:	

    sLoginRsp m_loginRsp;	//账户信息

    std::map<int, std::map<int, Organization>>      m_mapAssetMgmtOrgTree;//一个组织机构ID下的所有组织机构信息
	std::map<int, Organization>	                    m_mapOrganization;//组织机构
    std::map<int, UserInfo>                         m_mapUserInfo;//用户   
    std::map<int, RoleInfo>                         m_mapRoleInfo;//角色   
    std::map<int, std::vector<UserRole>>            m_mapUserRole;//用户与角色关系
	std::vector<UserAndOrgRelation>	                m_vecUserAndOrgRelation; //用户与组织机构关系
    std::map<int, UserAndTradeAccountRelation>      m_mapUserAndTradeAccountRelation;//用户与委托交易账户关系

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
    
    std::map<int, FinancialProduct>	                m_mapFinancialProduct;//理财产品 
    std::map<int, std::vector<TraderProRelation>>   m_mapTraderProRelation;//理财产品与交易员关系 
    std::map<int, TradeStrategy>                    m_mapTradeStrategy; //交易策略              
    std::map<int, std::vector<int>>                 m_mapStrategyOrgRelation;//交易策略与组织机构的关系    


	
	//根据合约ID得到合约详情
	MAPInstrumentID2InstrumentField			m_mapInstrumentID2InstrumentField;

	//根据产品ID得到合约详情
	MAPProductID2InstrumentField			m_mapProductID2InstrumentField;


};
