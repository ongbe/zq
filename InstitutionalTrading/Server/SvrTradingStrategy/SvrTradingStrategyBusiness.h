#pragma once
#include "rwlock.h"
#include "interface_svrtradingstrategy.h"

class CSvrTradingStrategyBusiness : public CInterface_SvrTradingStrategy
{

public:
    static CSvrTradingStrategyBusiness& getObj();
    static void deleteObj();

    CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf);

    //交易策略
    int AddStrategy(const int nUserID, TradeStrategy *pStrategy,int& nID);
    int DeleteStrategy(const int nUserID, int nID);
    int ModifyStrategy(const int nUserID, const TradeStrategy *pStrategy);

    //交易策略与组织机构的关系
    int SaveStrategyOrganizationRelation(const int nUserID, int nStrategyID, std::vector<int> vecOrgID);
    void GetStrategyOrganizationRelation(std::vector<AssetOrgStrategyRelation>& vecRelation);

    //模块接口
    void GetTradeStrategy(std::vector<TradeStrategy>& vecStrategy ); 
    void GetAssetOrgStrategyRelation(std::map<int,std::vector<int>>& mapRelation);
    void GetStrategyIDsByOrgID(int nOrgID,std::vector<int>& vecStrategyID );
    void GetOrgIDsByStrategyID (int nStrategyID,std::vector<int>& vecOrgID );
    void GetInfoByStrategyID (int nStrategyID,TradeStrategy& oStrategy);

public:
    //从数据库加载数据
    bool LoadDataFromDB();
private:
    //读取交易策略
    bool LoadStrategyFromDB();
    //读取交易策略与组织机构的关系
    bool LoadOrgStrategyRelationFromDB();

private:
    CSvrTradingStrategyBusiness(void);
    virtual ~CSvrTradingStrategyBusiness(void);
    static CSvrTradingStrategyBusiness* m_pObj;

private:
    //保存以交易策略ID为索引的交易策略
    std::map<int, TradeStrategy>							m_mapStrategy;
    //保存以交易策略ID为索引的交易策略和组织机构关系
    std::map<int, std::vector<int>>	                        m_mapStrategyID2OrgIDRelation;

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
    int m_nNextTradeStrategyID; 

    rwlock_t    m_Lock;
};
