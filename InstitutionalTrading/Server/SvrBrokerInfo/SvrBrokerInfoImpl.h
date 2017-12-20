#pragma once

#include <map>
#include <vector>
#include "rwlock.h"
#include "CommonStruct.h"
#include "Interface_SvrBrokerInfo.h"

class CSvrBrokerInfoImpl : public CInterface_SvrBrokerInfo
{
public:
	static CSvrBrokerInfoImpl& getObj(void);
    static void deleteObj();

    CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf);

    void SetServerRunMode(eServerRunMode nMode);
    char* GetData(int& nDataLen);
    void  SetData(	std::map<int,TradeAccount>& mapTradeAccount,
                    std::map<int,BrokerInfo>& mapBrokerInfo,
                    std::map<int,ServerGroup>& mapServerGroup,
                    std::map<int,ServerAddress>& mapServerAddr,
                    std::map<int,ServerAddressQuot>& mapServerAddrQuot,
                    std::map<int,TradeAccountAndOrg>& mapTradeAccountAndOrg);

	int AddBrokerInfo(const int nUserID, BrokerInfo& brokerInfo, int& nID);
	int ModifyBrokerInfo(const int nUserID,const BrokerInfo& brokerInfo);
    int ModifyBrokerInfoQuot(const int nUserID,const BrokerInfo& brokerInfo);
	int DelBrokerInfo(const int nUserID,int nID);
	void GetBrokerInfos(std::vector<BrokerInfo>& vec);
    bool GetBrokerInfoByID(int brokerID,BrokerInfo& brokerInfo);
    bool GetBrokerInfoByCode(std::string strCode,BrokerInfo& brokerInfo);

	int AddServerGroup(const int nUserID,ServerGroup& group, int& nID);
	int ModifyServerGroup(const int nUserID,const ServerGroup& group);
	int DelServerGroup(const int nUserID,int nID);
	void GetServerGroups(std::vector<ServerGroup>& vec);
	void GetServerGroupsByBrokerID(int nBrokerID, std::vector<ServerGroup>& vec);
	bool GetServerGroupByID(int nGroupID, ServerGroup& group);

	int AddServerAddr(const int nUserID,ServerAddress& address, int& nID);
	int ModifyServerAddr(const int nUserID,const ServerAddress& address);
	int DelServerAddr(const int nUserID,int nID);
	void GetServerAddr(std::vector<ServerAddress>& vec);
	void GetServerAddrByGroupID(int nGroupID, std::vector<ServerAddress>& vec);

    int AddServerAddrQuot(const int nUserID,ServerAddressQuot& addressQuot, int& nID);
    int ModifyServerAddrQuot(const int nUserID,const ServerAddressQuot& addressQuot);
    int DelServerAddrQuot(const int nUserID,int nID);
    void GetServerAddrQuot(std::vector<ServerAddressQuot>& vec);

	int AddTradeAccount(const int nUserID,TradeAccount& account, int& nID);
	int ModifyTradeAccount(const int nUserID,const TradeAccount& account,bool& bPsw);
	int DelTradeAccount(const int nUserID,int nID);
	void GetTradeAccounts(std::vector<TradeAccount>& vec);
	void GetTradeAccountsByBrokerID(int nBrokerID, std::vector<TradeAccount>& vec);
	void GetTradeAccountsByGroupID(int nGroupID, std::vector<TradeAccount>& vec);
	bool GetTradeAccount(int nTradeAccountID, TradeAccount& account);

    //判断是否有结算服务器组（结算服务器调用）
    bool GetSettleServerGroup();

    //获取所有委托交易账号和组织机构的关系
    void GetTradeAccountAndOrgRelation(std::vector<TradeAccountAndOrg>& vec);
    //通过委托交易账号ID获取组织机构ID
    void GetOrgIDByTradeAccountID(const int& nTradeAccountID, int& nOrgID);
    //通过委托交易账号字符串获取组织机构ID
    void GetOrgIDByBrokerCodeAndTradeAccount(const std::string& szBrokerCode, const std::string& szTradeAccount, int& nOrgID);
    //通过组织机构ID得到委托交易账户
    void GetTradeAccountByOrgID(const int& nOrgID, std::vector<TradeAccount>& vec);

    int SaveTradeAccountAndOrgRelation(const int nUserID, const TradeAccountAndOrg& relation);
    bool IsUpdateTradeAccountAndOrgRelation(const TradeAccountAndOrg& relation);
    

public:
    //从数据库加载数据
    bool LoadDB();

private:
	bool LoadBrokerInfoFromDB();
	bool LoadServerGroupFromDB();
	bool LoadServerAddressFromDB();
    bool LoadServerAddressQuotFromDB();
	bool LoadTradeAccountFromDB();
	bool LoadTradeAccountAndOrgRelationFromDB();

private:
	CSvrBrokerInfoImpl();
	virtual ~CSvrBrokerInfoImpl();
	static CSvrBrokerInfoImpl* m_pObj;

	std::map<int,BrokerInfo> m_mapBrokerInfo;
	std::map<int,ServerGroup> m_mapServerGroup;
	std::map<int,ServerAddress> m_mapServerAddr;
    std::map<int,ServerAddressQuot> m_mapServerAddrQuot;

	std::map<int,TradeAccount> m_mapTradeAccount;
    std::map<int,TradeAccountAndOrg> m_mapTradeAccountAndOrgRelation;

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
	int m_nNextTradeAccountID;   
	int m_nNextBrokerID;         
	int m_nNextServerGroupID;      
	int m_nNextAddrID;           
    int m_nNextAddrQuotID;       

    rwlock_t    m_Lock;

    eServerRunMode m_ServerRunMode;
};