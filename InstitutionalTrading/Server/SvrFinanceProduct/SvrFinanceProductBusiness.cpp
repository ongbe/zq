#include "SvrFinanceProduct.h"
#include "SvrFinanceProductBusiness.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "../SvrUserOrg/Interface_SvrUserOrg.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "SafeRwLock.h"
#include "CommonMacro.h"

CSvrFinanceProductBusiness* CSvrFinanceProductBusiness::m_pObj=NULL;

CSvrFinanceProductBusiness::CSvrFinanceProductBusiness(void)
:m_nNextFinancialProductID(0)
, m_ServerRunMode(ServerRunMode_Master)
{
    uv__rwlock_fallback_init(&m_Lock);
}

CSvrFinanceProductBusiness::~CSvrFinanceProductBusiness(void)
{
    uv__rwlock_fallback_destroy(&m_Lock);
}

CSvrFinanceProductBusiness& CSvrFinanceProductBusiness::getObj(void)
{
    g_mutex.lock();
    if (NULL == m_pObj)    
        m_pObj = new CSvrFinanceProductBusiness();

    g_mutex.unlock();
    return *m_pObj;
}

void CSvrFinanceProductBusiness::deleteObj()
{
    g_mutex.lock();
    if(NULL != m_pObj)
    {
        delete m_pObj;
        m_pObj = NULL;
    }	
    g_mutex.unlock();
}

//问询函数
CF_ERROR CSvrFinanceProductBusiness::ResponseToAsk(const eAskType& neDataType,
                                          const eAskOperationType& neChangeType,
                                          const int nllDataID,
                                          const char * npDataBuf)
{
    CSafeRwLockRead Lock(m_Lock);

    CF_ERROR lRet = CF_ERROR_SUCCESS;

    if(neDataType == AskType_FinacialProduct)
    {
        //删除事件
        if(neChangeType == AskOperationTypeDelete)
        {  
//             std::map<int, std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.find(nllDataID);
//             if(it!=m_mapProductID2TraderIDsRelation.end())
//                 return CF_ERROR_OPERATION_DEL_FINANCEPRODUCT;
        }
    }

    return lRet;
}

bool CSvrFinanceProductBusiness::LoadDataFromDB()
{
    //读取理财产品
    LoadFinancialProductFromDB();

    //读取理财产品与交易员的关系
    LoadTraderAndProRelationFromDB();

    return true;
}

bool CSvrFinanceProductBusiness::LoadFinancialProductFromDB()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapFinancialProduct.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_PRODUCT f order by f.financeproductid ASC");
    std::vector<std::vector<_variant_t>> vFP;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vFP, nErrorCode))
    {
        return false;
    }
    else
    {
        if(vFP.size()<=0)
            return true;

        int nDataCount = vFP.size()-1;
     
        //取数据库最后一条数据，判断是否是当天保存的，
        //如果是 m_nNextFinancialProductID则从个数算起
        //如果否 m_nNextFinancialProductID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vFP[nDataCount];
        int nFinancialProductID = vValue[0].intVal;
        
        if(((nFinancialProductID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nFinancialProductID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nFinancialProductID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextFinancialProductID = (nFinancialProductID&0x00007FFF);

        for ( UINT i = 0; i < vFP.size(); i++ )
        {
            FinancialProduct fp;
            memset(&fp, 0, sizeof(fp));

            std::vector<_variant_t> vValue = vFP[i];

            int nIndex = 0;
            fp.nID = vValue[nIndex++].intVal;
            strcpy(fp.szName, vValue[nIndex++].operator _bstr_t());
            strcpy(fp.szDesc, vValue[nIndex++].operator _bstr_t());

            m_mapFinancialProduct[fp.nID] = fp;
        }
    }

    return true;
}

bool CSvrFinanceProductBusiness::LoadTraderAndProRelationFromDB()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapProductID2TraderIDsRelation.clear();

    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from RELA_TRADER_PRODUCT f");
    std::vector<std::vector<_variant_t>> vFP;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vFP, nErrorCode))
    {
        return false;
    }
    else
    {
        for ( UINT i = 0; i < vFP.size(); i++ )
        {
            TraderProRelation oRelation;
            memset(&oRelation, 0, sizeof(oRelation));

            std::vector<_variant_t> vValue = vFP[i];

            int nIndex = 0;
            oRelation.nFinancialProductID = vValue[nIndex++].intVal;
            oRelation.nTraderID = vValue[nIndex++].intVal;

            m_mapProductID2TraderIDsRelation[oRelation.nFinancialProductID].push_back(oRelation.nTraderID);
        }
    }

    return true;
}

void CSvrFinanceProductBusiness::SetServerRunMode(eServerRunMode nMode)
{
    CSafeRwLockWrite Lock(m_Lock);

    m_ServerRunMode = nMode;
}

char* CSvrFinanceProductBusiness::GetData(int& nDataLen)
{
    CSafeRwLockWrite Lock(m_Lock);

    char* p = NULL;
    int nLen = 0;

    int nFinancialProductSize = m_mapFinancialProduct.size();    
    if(nFinancialProductSize<=0)
    {
        nDataLen = nLen;
        return p;
    }

    int nProductID2TraderIDSize;  
    TraderProRelation oRelation; 
    std::vector<TraderProRelation> vecRela;    

    for (std::map<int, std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.begin(); it!=m_mapProductID2TraderIDsRelation.end(); it++)
    {
        oRelation.nFinancialProductID = it->first;

        std::vector<int> vecTraderID = it->second;        ;
        for (std::vector<int>::iterator itTraderID = vecTraderID.begin(); itTraderID!=vecTraderID.end(); itTraderID++)
        {
            oRelation.nTraderID = (*itTraderID);
            vecRela.push_back(oRelation);
        }
    }

    nProductID2TraderIDSize = vecRela.size();

    nLen = sizeof(int)+
           sizeof(int)+
           nFinancialProductSize*sizeof(FinancialProduct)+
           nProductID2TraderIDSize*sizeof(TraderProRelation);

    char* pBuffer = new char[nLen];
    p = pBuffer;

    memset(pBuffer, 0, nDataLen);
    memcpy(pBuffer, &nFinancialProductSize, sizeof(int));
    memcpy(pBuffer+sizeof(int), &nProductID2TraderIDSize, sizeof(int));

    int nSize = 0;
    std::vector<FinancialProduct> vecFinancialProduct;
    for (std::map<int,FinancialProduct>::iterator it = m_mapFinancialProduct.begin();it!=m_mapFinancialProduct.end();++it)
    {
        FinancialProduct obj = it->second;
        vecFinancialProduct.push_back(obj);
    }
    if(nFinancialProductSize>0)
        memcpy(pBuffer+2*sizeof(int), &vecFinancialProduct[0], nFinancialProductSize*sizeof(FinancialProduct));
    nSize += nFinancialProductSize*sizeof(FinancialProduct);

    if(nProductID2TraderIDSize>0)
        memcpy(pBuffer+2*sizeof(int)+nSize, &vecRela[0], nProductID2TraderIDSize*sizeof(TraderProRelation));
    
    nDataLen = nLen;
    return p;

}

void CSvrFinanceProductBusiness::SetData(	std::map<int,FinancialProduct>& mapFinancialProduct,
                                            std::map<int,std::vector<int>>& mapProductID2TraderIDsRelation)
{
    CSafeRwLockWrite Lock(m_Lock);

    m_mapFinancialProduct = mapFinancialProduct;
    m_mapProductID2TraderIDsRelation = mapProductID2TraderIDsRelation;
}

int CSvrFinanceProductBusiness::AddFinancialProduct(const int nUserID, FinancialProduct *pFP, int& nID)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        int nYear = sysTime.wYear-BASE_YEAR;
        int nMonth = sysTime.wMonth;
        int nDay = sysTime.wDay;

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextFinancialProductID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_PRODUCT values(%d,'%s','%s',%d)",nID,pFP->szName,pFP->szDesc,UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            pFP->nID = nID;
            m_mapFinancialProduct[pFP->nID] = *pFP;
        }
        else
        {
            m_nNextFinancialProductID--;
        }


        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = pFP->nID;
        strcpy(logData.logTable,"LOG_PRODUCT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"理财产品(ID:%d   Name:%s   Desc:%s)",pFP->nID,pFP->szName,pFP->szDesc);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapFinancialProduct[pFP->nID] = *pFP;

    return nErrorCode;
}

int CSvrFinanceProductBusiness::DeleteFinancialProduct(const int nUserID, int nID)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_PRODUCT t where t.FinanceProductID = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int, FinancialProduct> ::iterator it = m_mapFinancialProduct.find(nID);
            if ( it != m_mapFinancialProduct.end())
                m_mapFinancialProduct.erase(it);
        }

        //关系操作
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete RELA_TRADER_PRODUCT t where t.FinanceProductID = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int, std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.find(nID);
            if ( it != m_mapProductID2TraderIDsRelation.end())
                m_mapProductID2TraderIDsRelation.erase(it);
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_PRODUCT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"理财产品(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_RELA_TRADER_PRODUCT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"理财产品(ID:%d):删除所有关系",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int, FinancialProduct> ::iterator it_product = m_mapFinancialProduct.find(nID);
        if ( it_product != m_mapFinancialProduct.end())
            m_mapFinancialProduct.erase(it_product);

        std::map<int, std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.find(nID);
        if ( it != m_mapProductID2TraderIDsRelation.end())
            m_mapProductID2TraderIDsRelation.erase(it);
    }
    
    return nErrorCode;
}

int CSvrFinanceProductBusiness::ModifyFinancialProduct(const int nUserID, FinancialProduct *pFP)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_PRODUCT t set t.Name = '%s', t.Description = '%s' where t.FinanceProductID = %d",
            pFP->szName,pFP->szDesc, pFP->nID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapFinancialProduct[pFP->nID] = *pFP;
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = pFP->nID;
        strcpy(logData.logTable,"LOG_PRODUCT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"理财产品(ID:%d   Name:%s   Desc:%s)",pFP->nID,pFP->szName,pFP->szDesc);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapFinancialProduct[pFP->nID] = *pFP;

    return nErrorCode;
}

void CSvrFinanceProductBusiness::GetFinancialProduct(std::vector<FinancialProduct>& vecFP)
{
    CSafeRwLockRead Lock(m_Lock);

    vecFP.clear();
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.begin();
    for ( ; it != m_mapFinancialProduct.end(); it++)
        vecFP.push_back(it->second);
}

int CSvrFinanceProductBusiness::SaveProductTraderRelation(const int nUserID,int nRelationType, int nID,std::vector<int> vecID)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        if( CInterface_SvrDBOpr::getObj().SaveProductTraderRelation(nRelationType, nID, vecID, nErrorCode))
        {
            if (PRODUCT_ASSGIN_TRADER_TYPE == nRelationType)
            {
                std::map<int,std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.find(nID);
                if(it!=m_mapProductID2TraderIDsRelation.end())
                    m_mapProductID2TraderIDsRelation.erase(it);

                if(!vecID.empty())
                    m_mapProductID2TraderIDsRelation[nID] = vecID;                
            }
            else if (TRADER_ASSGIN_PRODUCT_TYPE == nRelationType)
            {
                std::map<int,std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.begin();
                while (it!=m_mapProductID2TraderIDsRelation.end())
                {
                    std::vector<int>& vecTraderID = (it->second);
                    std::vector<int>::iterator it2 = vecTraderID.begin();
                    while (it2!=vecTraderID.end())
                    {
                        if (*it2 == nID)
                        {
                            it2 = vecTraderID.erase(it2);
                        }
                        else
                            ++it2;
                    }

                    if (vecTraderID.empty())
                    {
                        it = m_mapProductID2TraderIDsRelation.erase(it);
                    }
                    else
                        ++it;
                }


                std::vector<int>::iterator it3 = vecID.begin();
                for (; it3!=vecID.end(); it3++)
                {
                    std::map<int,std::vector<int>>::iterator it4 = m_mapProductID2TraderIDsRelation.find(*it3);
                    if (it4 == m_mapProductID2TraderIDsRelation.end())                
                        m_mapProductID2TraderIDsRelation[*it3].push_back(nID);
                    else
                        it4->second.push_back(nID);             
                }
            }
        }


        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_RELA_TRADER_PRODUCT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"理财产品与交易员关系(RelationType：%d     ProductID:%d)",nRelationType,nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        if (PRODUCT_ASSGIN_TRADER_TYPE == nRelationType)
        {
            std::map<int,std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.find(nID);
            if(it!=m_mapProductID2TraderIDsRelation.end())
                m_mapProductID2TraderIDsRelation.erase(it);

            if(!vecID.empty())
                m_mapProductID2TraderIDsRelation[nID] = vecID;                
        }
        else if (TRADER_ASSGIN_PRODUCT_TYPE == nRelationType)
        {
            std::map<int,std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.begin();
            while (it!=m_mapProductID2TraderIDsRelation.end())
            {
                std::vector<int>& vecTraderID = (it->second);
                std::vector<int>::iterator it2 = vecTraderID.begin();
                while (it2!=vecTraderID.end())
                {
                    if (*it2 == nID)
                    {
                        it2 = vecTraderID.erase(it2);
                    }
                    else
                        ++it2;
                }

                if (vecTraderID.empty())
                {
                    it = m_mapProductID2TraderIDsRelation.erase(it);
                }
                else
                    ++it;
            }


            std::vector<int>::iterator it3 = vecID.begin();
            for (; it3!=vecID.end(); it3++)
            {
                std::map<int,std::vector<int>>::iterator it4 = m_mapProductID2TraderIDsRelation.find(*it3);
                if (it4 == m_mapProductID2TraderIDsRelation.end())                
                    m_mapProductID2TraderIDsRelation[*it3].push_back(nID);
                else
                    it4->second.push_back(nID);             
            }
        }
    }
    return nErrorCode;
}

void CSvrFinanceProductBusiness::GetTraderProRelation(std::vector<TraderProRelation>& vecRela)
{		
    CSafeRwLockRead Lock(m_Lock);

    vecRela.clear();
    TraderProRelation oRelation;

    std::map<int, std::vector<int>>::iterator it = m_mapProductID2TraderIDsRelation.begin();
    for (; it!=m_mapProductID2TraderIDsRelation.end(); it++)
    {
        oRelation.nFinancialProductID = it->first;
        
        std::vector<int> vecTraderID = it->second;
        std::vector<int>::iterator itTraderID = vecTraderID.begin();
        for (; itTraderID!=vecTraderID.end(); itTraderID++)
        {
            oRelation.nTraderID = (*itTraderID);

            vecRela.push_back(oRelation);
        }
    }
}

void CSvrFinanceProductBusiness::GetTraderByFinancialProductID(int nFinancialProductID,std::vector<int>& vecTD)
{
    CSafeRwLockRead Lock(m_Lock);

    vecTD.clear();
    std::map<int, std::vector<int>> ::iterator itRelation = m_mapProductID2TraderIDsRelation.find(nFinancialProductID);
    if( itRelation != m_mapProductID2TraderIDsRelation.end())                    
        vecTD = itRelation->second;    
}