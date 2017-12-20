#pragma once
#include "rwlock.h"
#include "interface_svrfinanceproduct.h"

class CSvrFinanceProductBusiness : public CInterface_SvrFinanceProduct
{

public:
    static CSvrFinanceProductBusiness& getObj(void);
    static void deleteObj();

    CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf);

    void SetServerRunMode(eServerRunMode nMode);
    char* GetData(int& nDataLen);
    void  SetData(	std::map<int,FinancialProduct>& mapFinancialProduct,
                    std::map<int,std::vector<int>>& mapProductID2TraderIDsRelation);

    //理财产品
    int AddFinancialProduct(const int nUserID, FinancialProduct *pFP, int& nID);
    int DeleteFinancialProduct(const int nUserID, int nID);
    int ModifyFinancialProduct(const int nUserID, FinancialProduct *pFP);    


    //理财产品与交易员的关系
    int SaveProductTraderRelation(const int nUserID, int nRelationType, int nID,std::vector<int> vecID);

    //模块间接口
    void GetFinancialProduct(std::vector<FinancialProduct>& vecFP);
    void GetTraderProRelation(std::vector<TraderProRelation>& vecRela);
    void GetTraderByFinancialProductID(int nFinancialProductID,std::vector<int>& vecTD);


public:
    //从数据库加载数据
    bool LoadDataFromDB();

private:   
    //读取理财产品
    bool LoadFinancialProductFromDB();
    //读取理财产品与交易员的关系
    bool LoadTraderAndProRelationFromDB();

private:
    CSvrFinanceProductBusiness(void);
    virtual ~CSvrFinanceProductBusiness(void);
    static CSvrFinanceProductBusiness* m_pObj;

private:

    //保存以理财产品ID为索引的理财产品
    std::map<int, FinancialProduct>		m_mapFinancialProduct;
    //保存以理财产品ID为索引的理财产品和交易员ID关系
    std::map<int, std::vector<int>>		m_mapProductID2TraderIDsRelation;

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
    int m_nNextFinancialProductID;   

    rwlock_t    m_Lock;

    eServerRunMode m_ServerRunMode;
};
