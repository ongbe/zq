#include "SvrTradingStrategy.h"
#include "SvrTradingStrategyBusiness.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "../SvrUserOrg/Interface_SvrUserOrg.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "SafeRwLock.h"
#include "CommonMacro.h"


CSvrTradingStrategyBusiness* CSvrTradingStrategyBusiness::m_pObj=NULL;

CSvrTradingStrategyBusiness::CSvrTradingStrategyBusiness(void)
:m_nNextTradeStrategyID(0)
{
    uv__rwlock_fallback_init(&m_Lock);
}

CSvrTradingStrategyBusiness::~CSvrTradingStrategyBusiness(void)
{
    uv__rwlock_fallback_destroy(&m_Lock);
}

CSvrTradingStrategyBusiness& CSvrTradingStrategyBusiness::getObj()
{
    g_mutex.lock();
    if (NULL == m_pObj)    
        m_pObj = new CSvrTradingStrategyBusiness();
    g_mutex.unlock();
    return *m_pObj;
}

void CSvrTradingStrategyBusiness::deleteObj()
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
CF_ERROR CSvrTradingStrategyBusiness::ResponseToAsk(const eAskType& neDataType,
                                                     const eAskOperationType& neChangeType,
                                                     const int nllDataID,
                                                     const char * npDataBuf)
{
    CSafeRwLockRead Lock(m_Lock);

    CF_ERROR lRet = CF_ERROR_SUCCESS;

    if(neDataType == AskType_Orgnazation)
    {
        //删除事件
        if(neChangeType == AskOperationTypeDelete)
        {  
            //必须是叶子节点
            if(!CInterface_SvrUserOrg::getObj().IsLeafNode(nllDataID))
            {
                return CF_ERROR_OPERATION_DEL_ORGANIZATION_NO_LEAFNODE;
            }

            std::map<int, std::vector<int>>::iterator it = m_mapStrategyID2OrgIDRelation.find(nllDataID);
            if(it!=m_mapStrategyID2OrgIDRelation.end())
            {
                return CF_ERROR_OPERATION_DEL_TRADESTRATEGY;
            }
        }
    }

    return lRet;
}

//从数据库加载数据
bool CSvrTradingStrategyBusiness::LoadDataFromDB()
{
    //读取交易策略
    LoadStrategyFromDB();

    //读取交易策略与组织机构的关系
    LoadOrgStrategyRelationFromDB();

    return true;
}

//读取交易策略
bool CSvrTradingStrategyBusiness::LoadStrategyFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapStrategy.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_TRADESTRATEGY f order by f.strategyid ASC");
    std::vector<std::vector<_variant_t>> vec;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vec, nErrorCode))
    {
        return false;
    }
    else
    {
        if(vec.size()<=0)
            return true;

        int nDataCount = vec.size()-1;

        //取数据库最后一条数据，判断是否是当天保存的，
        //如果是 m_nNextTradeStrategyID则从个数算起
        //如果否 m_nNextTradeStrategyID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nTradeStrategyID = vValue[0].intVal;

        if(((nTradeStrategyID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nTradeStrategyID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nTradeStrategyID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextTradeStrategyID = (nTradeStrategyID&0x00007FFF);

        for ( UINT i = 0; i < vec.size(); i++ )
        {
            TradeStrategy oStrategy;
            memset(&oStrategy, 0, sizeof(oStrategy));

            std::vector<_variant_t> vValue = vec[i];

            int nIndex = 0;
            oStrategy.nID = vValue[nIndex++].intVal;
            strcpy(oStrategy.szName, vValue[nIndex++].operator _bstr_t());
            oStrategy.nFrequency = vValue[nIndex++].intVal;
            strcpy(oStrategy.szAddr, vValue[nIndex++].operator _bstr_t());

            m_mapStrategy[oStrategy.nID] = oStrategy;
        }
    }

    return true;
}

//读取交易策略与组织机构的关系
bool CSvrTradingStrategyBusiness::LoadOrgStrategyRelationFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapStrategyID2OrgIDRelation.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from RELA_ORG_STRATEGY f");
    std::vector<std::vector<_variant_t>> vecRela;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vecRela, nErrorCode))
    {
        return false;
    }
    else
    {
        for ( UINT i = 0; i < vecRela.size(); i++ )
        {
            std::vector<_variant_t> vValue = vecRela[i];

            int nIndex = 0;
            int nStrategyID = vValue[nIndex++].intVal;
            int nAssetOrgID = vValue[nIndex++].intVal;

            m_mapStrategyID2OrgIDRelation[nStrategyID].push_back(nAssetOrgID);            
        }
    }

    return true;
}

//交易策略
int CSvrTradingStrategyBusiness::AddStrategy(const int nUserID, TradeStrategy *pStrategy,int& nID)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    int nYear = sysTime.wYear-BASE_YEAR;
    int nMonth = sysTime.wMonth;
    int nDay = sysTime.wDay;

    nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextTradeStrategyID);

    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "insert into BASE_TRADESTRATEGY values(%d,'%s','%d','%s',%d)",
                      nID,pStrategy->szName,pStrategy->nFrequency,pStrategy->szAddr,UNDELETE_FLAG);
    if( CInterface_SvrDBOpr::getObj().Excute(szBuffer,nErrorCode))
    {
        pStrategy->nID = nID;
        m_mapStrategy[pStrategy->nID] = *pStrategy;
    }
    else
    {
        m_nNextTradeStrategyID--;
    }


    //log入库信息
    int nLogErrorCode; 
    LogBaseData logData;
    memset(&logData,0,sizeof(LogBaseData));

    logData.logTargetorID = pStrategy->nID;
    strcpy(logData.logTable,"LOG_TRADESTRATEGY");
    sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    logData.logOperatorID = nUserID;
    logData.logAction     = Oper_Add;
    sprintf(logData.logContent,"交易策略(ID:%d   Name:%s   Frequency:%d   Addr:%s)",pStrategy->nID,pStrategy->szName,pStrategy->nFrequency,pStrategy->szAddr);
    logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

    CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);


    return nErrorCode;
}

int CSvrTradingStrategyBusiness::DeleteStrategy(const int nUserID, int nID)
{
    CSafeRwLockRead Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));    
    sprintf(szBuffer, "delete BASE_TRADESTRATEGY t where t.StrategyID = %d", nID);
    if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
    {
        std::map<int, TradeStrategy> ::iterator it = m_mapStrategy.find(nID);
        if ( it != m_mapStrategy.end())
            m_mapStrategy.erase(it);
    }

    //关系操作
    memset(szBuffer, 0, sizeof(szBuffer));    
    sprintf(szBuffer, "delete RELA_ORG_STRATEGY t where t.StrategyID = %d", nID);
    if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
    {
        std::map<int, std::vector<int>> ::iterator it = m_mapStrategyID2OrgIDRelation.find(nID);
        if ( it != m_mapStrategyID2OrgIDRelation.end())
            m_mapStrategyID2OrgIDRelation.erase(it);
    }


    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    //log入库信息
    int nLogErrorCode; 
    LogBaseData logData;
    memset(&logData,0,sizeof(LogBaseData));

    logData.logTargetorID = nID;
    strcpy(logData.logTable,"LOG_TRADESTRATEGY");
    sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    logData.logOperatorID = nUserID;
    logData.logAction     = Oper_Delete;
    sprintf(logData.logContent,"交易策略(ID:%d)",nID);
    logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
    CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

    memset(&logData,0,sizeof(LogBaseData));

    logData.logTargetorID = nID;
    strcpy(logData.logTable,"LOG_RELA_ORG_STRATEGY");
    sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    logData.logOperatorID = nUserID;
    logData.logAction     = Oper_Delete;
    sprintf(logData.logContent,"交易策略(ID:%d):删除所有关系",nID);
    logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
    CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);


    return nErrorCode;
}

int CSvrTradingStrategyBusiness::ModifyStrategy(const int nUserID, const TradeStrategy *pStrategy)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, 
        "update BASE_TRADESTRATEGY t set t.Name = '%s',\
        t.Frequency = %d,\
        t.Addr = '%s' where t.StrategyID = %d",
        pStrategy->szName,   
        pStrategy->nFrequency,
        pStrategy->szAddr, pStrategy->nID);
    int nNum = 0;
    if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
    {
        if ( nNum != 0 )
            m_mapStrategy[pStrategy->nID] = *pStrategy;
    }

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    //log入库信息
    int nLogErrorCode; 
    LogBaseData logData;
    memset(&logData,0,sizeof(LogBaseData));

    logData.logTargetorID = pStrategy->nID;
    strcpy(logData.logTable,"LOG_TRADESTRATEGY");
    sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    logData.logOperatorID = nUserID;
    logData.logAction     = Oper_Modify;
    sprintf(logData.logContent,"交易策略(ID:%d   Name:%s   Frequency:%d   Addr:%s)",pStrategy->nID,pStrategy->szName,pStrategy->nFrequency,pStrategy->szAddr);
    logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

    CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);


    return nErrorCode;
}

//交易策略与组织机构的关系
int CSvrTradingStrategyBusiness::SaveStrategyOrganizationRelation(const int nUserID, int nStrategyID, std::vector<int> vecOrgID)
{
    CSafeRwLockWrite Lock(m_Lock);

    char tempBuffer[512];
    memset(tempBuffer,0,sizeof(tempBuffer));

    int nErrorCode = CF_ERROR_SUCCESS;
    if( CInterface_SvrDBOpr::getObj().SaveStrategyOrganizationRelation(nStrategyID, vecOrgID, nErrorCode))
    {
        std::map<int, std::vector<int>>::iterator it = m_mapStrategyID2OrgIDRelation.find(nStrategyID);
        if(it!=m_mapStrategyID2OrgIDRelation.end())
            m_mapStrategyID2OrgIDRelation.erase(it);

        if(!vecOrgID.empty())
            m_mapStrategyID2OrgIDRelation[nStrategyID] = vecOrgID;
    }

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    //log入库信息
    int nLogErrorCode; 
    LogBaseData logData;
    memset(&logData,0,sizeof(LogBaseData));

    logData.logTargetorID = nStrategyID;
    strcpy(logData.logTable,"LOG_RELA_ORG_STRATEGY");
    sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    logData.logOperatorID = nUserID;
    logData.logAction     = Oper_Save;
    sprintf(logData.logContent,"交易策略与组织机构关系(StategeID:%d)",nStrategyID);
    logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

    CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

    return nErrorCode;
}

 void CSvrTradingStrategyBusiness::GetStrategyOrganizationRelation(std::vector<AssetOrgStrategyRelation>& vecRelation)
 {
     CSafeRwLockRead Lock(m_Lock);

     vecRelation.clear();
     AssetOrgStrategyRelation oRelation;

     std::map<int, std::vector<int>>::iterator it = m_mapStrategyID2OrgIDRelation.begin();
     for (; it!=m_mapStrategyID2OrgIDRelation.end(); it++)
     {
         oRelation.nStrategyID = it->first;

         std::vector<int> vecOrgID = it->second;
         std::vector<int>::iterator itOrgID = vecOrgID.begin();
         for (; itOrgID!=vecOrgID.end(); itOrgID++)
         {
             oRelation.nAssetOrgID = (*itOrgID);
             vecRelation.push_back(oRelation);
         }
     }
 }

//模块间接口
void CSvrTradingStrategyBusiness::GetTradeStrategy(std::vector<TradeStrategy>& vecStrategy)
{
    CSafeRwLockRead Lock(m_Lock);

    vecStrategy.clear();
    std::map<int, TradeStrategy>::iterator it = m_mapStrategy.begin();
    for ( ; it != m_mapStrategy.end(); it++)
        vecStrategy.push_back(it->second);
}

void CSvrTradingStrategyBusiness::GetAssetOrgStrategyRelation(std::map<int,std::vector<int>>& mapRelation)
{
    CSafeRwLockRead Lock(m_Lock);
    mapRelation = m_mapStrategyID2OrgIDRelation;
}

void CSvrTradingStrategyBusiness::GetStrategyIDsByOrgID(int nOrgID,std::vector<int>& vecStrategyID )
{
    CSafeRwLockRead Lock(m_Lock);

    vecStrategyID.clear();
    std::map<int,std::vector<int>>::iterator it = m_mapStrategyID2OrgIDRelation.begin();
    for ( ;it!=m_mapStrategyID2OrgIDRelation.end(); it++)
    {
        std::vector<int> vec = it->second;
        std::vector<int>::iterator it1 = vec.begin();
        for (; it1 != vec.end(); it1++)
        {
            if ( nOrgID == (*it1))
            {
                vecStrategyID.push_back(it->first);
                break;
            }
        }
    }        
}

void CSvrTradingStrategyBusiness::GetOrgIDsByStrategyID (int nStrategyID,std::vector<int>& vecOrgID )
{
    CSafeRwLockRead Lock(m_Lock);

    vecOrgID.clear();
    std::map<int,std::vector<int>>::iterator it = m_mapStrategyID2OrgIDRelation.find(nStrategyID);
    if(it!=m_mapStrategyID2OrgIDRelation.end())
    {
         vecOrgID = it->second;
    }  
}

void CSvrTradingStrategyBusiness::GetInfoByStrategyID (int nStrategyID,TradeStrategy& oStrategy)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,TradeStrategy>::iterator it = m_mapStrategy.find(nStrategyID);
    if( it != m_mapStrategy.end())
        memcpy(&oStrategy,&(TradeStrategy)(it->second),sizeof(TradeStrategy));
}
