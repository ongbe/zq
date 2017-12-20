#pragma warning(disable : 4996)

#include "SvrBrokerInfo.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "SvrBrokerInfoImpl.h"
#include "SafeRwLock.h"
#include "CommonMacro.h"

CSvrBrokerInfoImpl* CSvrBrokerInfoImpl::m_pObj=NULL;


CSvrBrokerInfoImpl::CSvrBrokerInfoImpl()
: m_nNextTradeAccountID(0)
, m_nNextBrokerID(0)
, m_nNextServerGroupID(0)
, m_nNextAddrID(0)
, m_nNextAddrQuotID(0)
, m_ServerRunMode(ServerRunMode_Master)
{
    uv__rwlock_fallback_init(&m_Lock);	
}

CSvrBrokerInfoImpl::~CSvrBrokerInfoImpl()
{
    uv__rwlock_fallback_destroy(&m_Lock);
}

CSvrBrokerInfoImpl& CSvrBrokerInfoImpl::getObj(void)
{
	g_mutex.lock();

	if(!m_pObj)
		m_pObj=new CSvrBrokerInfoImpl();

    g_mutex.unlock();
	return *m_pObj;
}

void CSvrBrokerInfoImpl::deleteObj()
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
CF_ERROR CSvrBrokerInfoImpl::ResponseToAsk(const eAskType& neDataType,
                                          const eAskOperationType& neChangeType,
                                          const int nllDataID,
                                          const char * npDataBuf)
{
    CSafeRwLockRead Lock(m_Lock);

    CF_ERROR lRet = CF_ERROR_SUCCESS;

//     if(neDataType == DataType_Broker)
//     {
//         //删除事件
//         if(neChangeType == AskOperationTypeDelete)
//         {   
//             //有委托交易账户关联的不能删
//             for (std::map<int, TradeAccount>::iterator it=m_mapTradeAccount.begin();it!=m_mapTradeAccount.end();it++)
//             {
//                 if(it->second.nBrokerID == nllDataID)
//                     return CF_ERROR_OPERATION_DEL_BROKER;
//             }
//         }
//     }

//     if(neDataType == DataType_ServerGroup)
//     {
//         //删除事件
//         if(neChangeType == AskOperationTypeDelete)
//         {   
//             //有委托交易账户关联的不能删
//             for (std::map<int, TradeAccount>::iterator it=m_mapTradeAccount.begin();it!=m_mapTradeAccount.end();it++)
//             {
//                 if(it->second.nServerGroupID == nllDataID)
//                     return CF_ERROR_OPERATION_DEL_BROKER;
//             }
//         }
//     }

//     if(neDataType == DataType_ServerAddress)
//     {
//         //删除事件
//         if(neChangeType == AskOperationTypeDelete)
//         {   
//             std::map<int,ServerAddress>::iterator it_addr = m_mapServerAddr.find(nllDataID);
//             if(it_addr!=m_mapServerAddr.end())
//             {
//                 //有委托交易账户关联的不能删
//                 for (std::map<int, TradeAccount>::iterator it=m_mapTradeAccount.begin();it!=m_mapTradeAccount.end();it++)
//                 {
//                     if(it->second.nServerGroupID == it_addr->second.nServerID)
//                         return CF_ERROR_OPERATION_DEL_BROKER;
//                 }             
//             }
//         }
//     }

    return lRet;
}

void CSvrBrokerInfoImpl::SetServerRunMode(eServerRunMode nMode)
{
    CSafeRwLockWrite Lock(m_Lock);

    m_ServerRunMode = nMode;
}

char* CSvrBrokerInfoImpl::GetData(int& nDataLen)
{
    CSafeRwLockWrite Lock(m_Lock);

    char* p = NULL;
    int nLen = 0;

    int nTradeAccountSize = m_mapTradeAccount.size();
    int nBrokerInfoSize = m_mapBrokerInfo.size();
    int nServerGroupSize = m_mapServerGroup.size();
    int nServerAddrSize = m_mapServerAddr.size();
    int nServerAddrQuotSize = m_mapServerAddrQuot.size();
    int nTradeAccountAndOrgRelationSize = m_mapTradeAccountAndOrgRelation.size();

    nLen =     sizeof(int)+
               sizeof(int)+
               sizeof(int)+
               sizeof(int)+
               sizeof(int)+
               sizeof(int)+
               nTradeAccountSize*sizeof(TradeAccount)+
               nBrokerInfoSize*sizeof(BrokerInfo)+
               nServerGroupSize*sizeof(ServerGroup)+
               nServerAddrSize*sizeof(ServerAddress)+
               nServerAddrQuotSize*sizeof(ServerAddressQuot)+
               nTradeAccountAndOrgRelationSize*sizeof(TradeAccountAndOrg);

    char* pBuffer = new char[nLen];
    p = pBuffer;

    memset(pBuffer, 0, nDataLen);
    memcpy(pBuffer, &nTradeAccountSize, sizeof(int));
    memcpy(pBuffer+sizeof(int), &nBrokerInfoSize, sizeof(int));
    memcpy(pBuffer+2*sizeof(int), &nServerGroupSize, sizeof(int));
    memcpy(pBuffer+3*sizeof(int), &nServerAddrSize, sizeof(int));
    memcpy(pBuffer+4*sizeof(int), &nServerAddrQuotSize, sizeof(int));
    memcpy(pBuffer+5*sizeof(int), &nTradeAccountAndOrgRelationSize, sizeof(int));


    int nSize = 0;
    TradeAccount objTradeAccount;
    std::vector<TradeAccount> vecTradeAccount;
    for (std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();it!=m_mapTradeAccount.end();++it)
    {
        objTradeAccount = it->second;
        vecTradeAccount.push_back(objTradeAccount);
    }
    if(nTradeAccountSize>0)
        memcpy(pBuffer+6*sizeof(int), &vecTradeAccount[0], nTradeAccountSize*sizeof(TradeAccount));
    nSize += nTradeAccountSize*sizeof(TradeAccount);

    BrokerInfo objBrokerInfo;
    std::vector<BrokerInfo> vecBrokerInfo;
    for (std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();it!=m_mapBrokerInfo.end();++it)
    {
        objBrokerInfo = it->second;
        vecBrokerInfo.push_back(objBrokerInfo);
    }
    if(nBrokerInfoSize>0)
        memcpy(pBuffer+6*sizeof(int)+nSize, &vecBrokerInfo[0], nBrokerInfoSize*sizeof(BrokerInfo));
    nSize += nBrokerInfoSize*sizeof(BrokerInfo);

    ServerGroup objServerGroup;
    std::vector<ServerGroup> vecServerGroup;
    for (std::map<int,ServerGroup>::iterator it = m_mapServerGroup.begin();it!=m_mapServerGroup.end();++it)
    {
        objServerGroup = it->second;
        vecServerGroup.push_back(objServerGroup);
    }
    if(nServerGroupSize>0)
        memcpy(pBuffer+6*sizeof(int)+nSize, &vecServerGroup[0], nServerGroupSize*sizeof(ServerGroup));
    nSize += nServerGroupSize*sizeof(ServerGroup);

    ServerAddress objServerAddress;
    std::vector<ServerAddress> vecServerAddress;
    for (std::map<int,ServerAddress>::iterator it = m_mapServerAddr.begin();it!=m_mapServerAddr.end();++it)
    {
        objServerAddress = it->second;
        vecServerAddress.push_back(objServerAddress);
    }
    if(nServerAddrSize>0)
        memcpy(pBuffer+6*sizeof(int)+nSize, &vecServerAddress[0], nServerAddrSize*sizeof(ServerAddress));
    nSize += nServerAddrSize*sizeof(ServerAddress);

    ServerAddressQuot objServerAddressQuot;
    std::vector<ServerAddressQuot> vecServerAddressQuot;
    for (std::map<int,ServerAddressQuot>::iterator it = m_mapServerAddrQuot.begin();it!=m_mapServerAddrQuot.end();++it)
    {
        objServerAddressQuot = it->second;
        vecServerAddressQuot.push_back(objServerAddressQuot);
    }
    if(nServerAddrQuotSize>0)
        memcpy(pBuffer+6*sizeof(int)+nSize, &vecServerAddressQuot[0], nServerAddrQuotSize*sizeof(ServerAddressQuot));
    nSize += nServerAddrQuotSize*sizeof(ServerAddressQuot);

    TradeAccountAndOrg objTradeAccountAndOrg;
    std::vector<TradeAccountAndOrg> vecTradeAccountAndOrg;
    for (std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.begin();it!=m_mapTradeAccountAndOrgRelation.end();++it)
    {
        objTradeAccountAndOrg = it->second;
        vecTradeAccountAndOrg.push_back(objTradeAccountAndOrg);
    }
    if(nTradeAccountAndOrgRelationSize>0)
        memcpy(pBuffer+6*sizeof(int)+nSize, &vecTradeAccountAndOrg[0], nTradeAccountAndOrgRelationSize*sizeof(TradeAccountAndOrg));

    nDataLen = nLen;
    return p;

}

void CSvrBrokerInfoImpl::SetData(	  std::map<int,TradeAccount>& mapTradeAccount,
                                      std::map<int,BrokerInfo>& mapBrokerInfo,
                                      std::map<int,ServerGroup>& mapServerGroup,
                                      std::map<int,ServerAddress>& mapServerAddr,
                                      std::map<int,ServerAddressQuot>& mapServerAddrQuot,
                                      std::map<int,TradeAccountAndOrg>& mapTradeAccountAndOrg)
{
    CSafeRwLockWrite Lock(m_Lock);

    m_mapTradeAccount = mapTradeAccount;
    m_mapBrokerInfo = mapBrokerInfo;
    m_mapServerGroup = mapServerGroup;
    m_mapServerAddr = mapServerAddr;
    m_mapServerAddrQuot = mapServerAddrQuot;
    m_mapTradeAccountAndOrgRelation = mapTradeAccountAndOrg;
}

int CSvrBrokerInfoImpl::AddBrokerInfo( const int nUserID, BrokerInfo& brokerInfo, int& nID )
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextBrokerID);

	    memset(szBuffer, 0, sizeof(szBuffer));
	    sprintf(szBuffer, "insert into BASE_BROKER values(%d, '%s', '%s', %d, '%s', '%s', %d)",
					      nID, brokerInfo.szBrokerCode, brokerInfo.szCompany, brokerInfo.nType, brokerInfo.szAccount, brokerInfo.szPassword, UNDELETE_FLAG);
        
	    if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
	    {
		    brokerInfo.nBrokerID = nID;
		    m_mapBrokerInfo[nID] = brokerInfo;
	    }
	    else
	    {
		    m_nNextBrokerID--;
	    }

          
        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = brokerInfo.nBrokerID;
        strcpy(logData.logTable,"LOG_BROKER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
                                    sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"经纪公司(ID:%d   Code:%s   Company:%s    nType:%d    Account:%s    Password:%s)",
                                    brokerInfo.nBrokerID,brokerInfo.szBrokerCode,brokerInfo.szCompany,brokerInfo.nType,brokerInfo.szAccount,brokerInfo.szPassword);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapBrokerInfo[brokerInfo.nBrokerID] = brokerInfo;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::ModifyBrokerInfo( const int nUserID, const BrokerInfo& brokerInfo )
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_BROKER t set t.brokercode = '%s',t.company = '%s' where t.brokerid = %d",
            brokerInfo.szBrokerCode, brokerInfo.szCompany, brokerInfo.nBrokerID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapBrokerInfo[brokerInfo.nBrokerID] = brokerInfo;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = brokerInfo.nBrokerID;
        strcpy(logData.logTable,"LOG_BROKER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"经纪公司(ID:%d   Code:%s   Company:%s    nType:%d    Account:%s    Password:%s)",
            brokerInfo.nBrokerID,brokerInfo.szBrokerCode,brokerInfo.szCompany,brokerInfo.nType,brokerInfo.szAccount,brokerInfo.szPassword);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapBrokerInfo[brokerInfo.nBrokerID] = brokerInfo;

    return nErrorCode;
}

int CSvrBrokerInfoImpl::ModifyBrokerInfoQuot( const int nUserID, const BrokerInfo& brokerInfo )
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        int nNum = 0;

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "update BASE_BROKER t set t.type = %d where t.type = %d", BROKER_TYPE_TRADE, BROKER_TYPE_QUOT);
        if (CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        { 
            if ( nNum != 0 )
            {
                for(std::map<int,BrokerInfo>::iterator it=m_mapBrokerInfo.begin();it!=m_mapBrokerInfo.end();it++)
                {
                    if(it->second.nType)
                        it->second.nType = BROKER_TYPE_TRADE;
                }
            }
        }

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "update BASE_BROKER t set t.type = %d,t.account = '%s',t.password = '%s' where t.brokerid = %d",
            brokerInfo.nType, brokerInfo.szAccount, brokerInfo.szPassword, brokerInfo.nBrokerID);

        if ( CF_ERROR_SUCCESS == nErrorCode && CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
            {
                std::map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.find(brokerInfo.nBrokerID);
                if(it!=m_mapBrokerInfo.end())
                {
                    it->second.nType = BROKER_TYPE_QUOT;
                    strcpy(it->second.szAccount,brokerInfo.szAccount);
                    strcpy(it->second.szPassword,brokerInfo.szPassword);
                }
            }
        }


        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = brokerInfo.nBrokerID;
        strcpy(logData.logTable,"LOG_BROKER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"经纪公司(ID:%d   Code:%s   Company:%s    nType:%d    Account:%s    Password:%s)",
            brokerInfo.nBrokerID,brokerInfo.szBrokerCode,brokerInfo.szCompany,brokerInfo.nType,brokerInfo.szAccount,brokerInfo.szPassword);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        for(std::map<int,BrokerInfo>::iterator it=m_mapBrokerInfo.begin();it!=m_mapBrokerInfo.end();it++)
        {
            if(it->second.nType)
                it->second.nType = BROKER_TYPE_TRADE;
        }

        std::map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.find(brokerInfo.nBrokerID);
        if(it!=m_mapBrokerInfo.end())
        {
            it->second.nType = BROKER_TYPE_QUOT;
            strcpy(it->second.szAccount,brokerInfo.szAccount);
            strcpy(it->second.szPassword,brokerInfo.szPassword);
        }
    }

	return nErrorCode;
}

int CSvrBrokerInfoImpl::DelBrokerInfo( const int nUserID, int nID )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_BROKER t where t.brokerid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(nID);
            if ( it != m_mapBrokerInfo.end())
                m_mapBrokerInfo.erase(it);
        }

        //删除与服务器组(地址)的关系
        std::map<int,ServerGroup>::iterator it_group = m_mapServerGroup.begin();
        while (it_group!=m_mapServerGroup.end())
        {
            if(it_group->second.nBrokerID == nID)
            {
                std::map<int,ServerAddress>::iterator it_addr = m_mapServerAddr.begin();
                while (it_addr!=m_mapServerAddr.end())
                {
                    if(it_addr->second.nServerID == it_group->second.nServerID)
                        it_addr = m_mapServerAddr.erase(it_addr);
                    else
                        it_addr++;
                }

                it_group = m_mapServerGroup.erase(it_group);
            }
            else
                it_group++;
        }

        //删除与委托交易账号的关系
        std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();
        for (;it!=m_mapTradeAccount.end();it++)
        {
            if(it->second.nBrokerID == nID)
            {
                it->second.nBrokerID = TRADEACCOUNT_BROKERIDORSERVERID_NULL;
                break;
            }
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_BROKER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"经纪公司(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_SERVERGROUP");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"经纪公司(ID:%d):删除服务器组",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

        strcpy(logData.logTable,"LOG_SERVERADDR");
        sprintf(logData.logContent,"经纪公司(ID:%d):删除服务器组地址",nID);
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);


        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"经纪公司(ID:%d):设置委托交易账号的BrokerID为-1",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        //删除
        std::map<int,BrokerInfo>::iterator it_broker = m_mapBrokerInfo.find(nID);
        if ( it_broker != m_mapBrokerInfo.end())
            m_mapBrokerInfo.erase(it_broker);

        //删除与服务器组(地址)的关系
        std::map<int,ServerGroup>::iterator it_group = m_mapServerGroup.begin();
        while (it_group!=m_mapServerGroup.end())
        {
            if(it_group->second.nBrokerID == nID)
            {
                std::map<int,ServerAddress>::iterator it_addr = m_mapServerAddr.begin();
                while (it_addr!=m_mapServerAddr.end())
                {
                    if(it_addr->second.nServerID == it_group->second.nServerID)
                        it_addr = m_mapServerAddr.erase(it_addr);
                    else
                        it_addr++;
                }

                it_group = m_mapServerGroup.erase(it_group);
            }
            else
                it_group++;
        }

        //删除与委托交易账号的关系
        std::map<int,TradeAccount>::iterator it_account = m_mapTradeAccount.begin();
        for (;it_account!=m_mapTradeAccount.end();it_account++)
        {
            if(it_account->second.nBrokerID == nID)
            {
                it_account->second.nBrokerID = TRADEACCOUNT_BROKERIDORSERVERID_NULL;
                break;
            }
        }
    }

	return nErrorCode;
}

void CSvrBrokerInfoImpl::GetBrokerInfos( std::vector<BrokerInfo>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
	for ( ; it != m_mapBrokerInfo.end(); it++)
		vec.push_back(it->second);
}

bool CSvrBrokerInfoImpl::GetBrokerInfoByID(int brokerID,BrokerInfo& brokerInfo)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(brokerID);
    if(it != m_mapBrokerInfo.end())
    {
        brokerInfo = it->second;
        return true;
    }

    return false;  
}

bool CSvrBrokerInfoImpl::GetBrokerInfoByCode(std::string strCode,BrokerInfo& brokerInfo)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
    for (;it!=m_mapBrokerInfo.end();it++)
    {
        if(it->second.szBrokerCode == strCode)
        {
            brokerInfo = it->second;
            return true;
        }        
    }

    return false;  
}

int CSvrBrokerInfoImpl::AddServerGroup( const int nUserID, ServerGroup& group, int& nID )
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextServerGroupID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_SERVERGROUP values(%d, '%s',%d, %d, %d)",
            nID, group.szServerName, group.nBrokerID, group.nIsSettle, UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            group.nServerID = nID;
            m_mapServerGroup[nID] = group;
        }
        else
        {
            m_nNextServerGroupID--;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = group.nServerID;
        strcpy(logData.logTable,"LOG_SERVERGROUP");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"服务器组(ID:%d   Name:%s   BrokerID:%d    IsSettle:%d)",
            group.nServerID,group.szServerName, group.nBrokerID, group.nIsSettle);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapServerGroup[group.nServerID] = group;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::ModifyServerGroup( const int nUserID, const ServerGroup& group )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_SERVERGROUP t set t.servername = '%s', t.issettle = %d where t.serverid = %d",
            group.szServerName, group.nIsSettle, group.nServerID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapServerGroup[group.nServerID] = group;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = group.nServerID;
        strcpy(logData.logTable,"LOG_SERVERGROUP");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"服务器组(ID:%d   Name:%s   BrokerID:%d    IsSettle:%d)",
            group.nServerID,group.szServerName, group.nBrokerID, group.nIsSettle);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapServerGroup[group.nServerID] = group;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::DelServerGroup( const int nUserID, int nID )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_SERVERGROUP t where t.serverid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,ServerGroup>::iterator it = m_mapServerGroup.find(nID);
            if ( it != m_mapServerGroup.end())
                m_mapServerGroup.erase(it);
        }

        //删除与服务器组地址的关系
        std::map<int,ServerAddress>::iterator it_addr = m_mapServerAddr.begin();
        while (it_addr!=m_mapServerAddr.end())
        {
            if(it_addr->second.nServerID == nID)
                it_addr = m_mapServerAddr.erase(it_addr);
            else
                it_addr++;
        }

        //删除与委托交易账号的关系
        std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();
        for (;it!=m_mapTradeAccount.end();it++)
        {
            if(it->second.nServerGroupID == nID)
            {
                it->second.nServerGroupID = TRADEACCOUNT_BROKERIDORSERVERID_NULL;
                break;
            }
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_SERVERGROUP");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"服务器组(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_SERVERADDR");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"服务器组(ID:%d):删除服务器组地址",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"服务器组(ID:%d):设置委托交易账号的BrokerID为-1",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        //删除
        std::map<int,ServerGroup>::iterator it = m_mapServerGroup.find(nID);
        if ( it != m_mapServerGroup.end())
            m_mapServerGroup.erase(it);

        //删除与服务器组地址的关系
        std::map<int,ServerAddress>::iterator it_addr = m_mapServerAddr.begin();
        while (it_addr!=m_mapServerAddr.end())
        {
            if(it_addr->second.nServerID == nID)
                it_addr = m_mapServerAddr.erase(it_addr);
            else
                it_addr++;
        }

        //删除与委托交易账号的关系
        std::map<int,TradeAccount>::iterator it_account = m_mapTradeAccount.begin();
        for (;it_account!=m_mapTradeAccount.end();it_account++)
        {
            if(it_account->second.nServerGroupID == nID)
            {
                it_account->second.nServerGroupID = TRADEACCOUNT_BROKERIDORSERVERID_NULL;
                break;
            }
        }
    }

	return nErrorCode;
}

void CSvrBrokerInfoImpl::GetServerGroups( std::vector<ServerGroup>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,ServerGroup>::iterator it = m_mapServerGroup.begin();
	for ( ; it != m_mapServerGroup.end(); it++)
	{
		vec.push_back(it->second);
	}
}

void CSvrBrokerInfoImpl::GetServerGroupsByBrokerID(int nBrokerID, std::vector<ServerGroup>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,ServerGroup>::iterator it = m_mapServerGroup.begin();
	for ( ; it != m_mapServerGroup.end(); it++)
	{
		if ( it->second.nBrokerID == nBrokerID )
			vec.push_back(it->second);
	}
}

bool CSvrBrokerInfoImpl::GetServerGroupByID(int nGroupID, ServerGroup& group)
{
    CSafeRwLockRead Lock(m_Lock);

	std::map<int,ServerGroup>::iterator it = m_mapServerGroup.find(nGroupID);
	if ( it != m_mapServerGroup.end())
	{
		group = it->second;
        return true;
	}

	return false;
}

int CSvrBrokerInfoImpl::AddServerAddr( const int nUserID, ServerAddress& address, int& nID )
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextAddrID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_SERVERADDR values(%d,%d,'%s',%d,%d)",
            nID, address.nServerID, address.szIP, address.nPort,UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            address.nAddrID = nID;
            m_mapServerAddr[nID] = address;
        }
        else
        {
            m_nNextAddrID--;
        }


        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_SERVERADDR");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"服务器组地址(ID:%d   ServerID:%d   IP:%s    Port:%d)",
            address.nAddrID, address.nServerID, address.szIP, address.nPort);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapServerAddr[address.nAddrID] = address;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::ModifyServerAddr( const int nUserID, const ServerAddress& address )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_SERVERADDR t set t.address = '%s',\
            t.port = %d where t.addrid = %d",
            address.szIP, address.nPort, address.nAddrID);

        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapServerAddr[address.nAddrID] = address;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = address.nAddrID;
        strcpy(logData.logTable,"LOG_SERVERADDR");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"服务器组地址(ID:%d   ServerID:%d   IP:%s    Port:%d)",
            address.nAddrID, address.nServerID, address.szIP, address.nPort);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapServerAddr[address.nAddrID] = address;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::DelServerAddr( const int nUserID, int nID )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_SERVERADDR t where t.addrid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,ServerAddress>::iterator it = m_mapServerAddr.find(nID);
            if ( it != m_mapServerAddr.end())
                m_mapServerAddr.erase(it);
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_SERVERADDR");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"服务器组地址(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int,ServerAddress>::iterator it = m_mapServerAddr.find(nID);
        if ( it != m_mapServerAddr.end())
            m_mapServerAddr.erase(it);
    }

	return nErrorCode;
}

void CSvrBrokerInfoImpl::GetServerAddr( std::vector<ServerAddress>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,ServerAddress>::iterator it = m_mapServerAddr.begin();
	for ( ; it != m_mapServerAddr.end(); it++)
	{
		vec.push_back(it->second);
	}
}

void CSvrBrokerInfoImpl::GetServerAddrByGroupID( int nGroupID, std::vector<ServerAddress>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,ServerAddress>::iterator it = m_mapServerAddr.begin();
	for ( ; it != m_mapServerAddr.end(); it++)
	{
		if ( it->second.nServerID == nGroupID )
			vec.push_back(it->second);
	}
}

int CSvrBrokerInfoImpl::AddServerAddrQuot(const int nUserID, ServerAddressQuot& addressQuot, int& nID)
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextAddrQuotID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_SERVERADDRQUOT values(%d, '%s','%s',%d,%d)",
            nID, addressQuot.szBrokerCode, addressQuot.szIP, addressQuot.nPort,UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            addressQuot.nAddrID = nID;
            m_mapServerAddrQuot[nID] = addressQuot;
        }
        else
        {
            m_nNextAddrQuotID--;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = addressQuot.nAddrID;
        strcpy(logData.logTable,"LOG_SERVERADDRQUOT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"服务器组行情地址(ID:%d   BrokerCode:%s   IP:%s    Port:%d)",
            addressQuot.nAddrID,addressQuot.szBrokerCode,addressQuot.szIP,addressQuot.nPort);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapServerAddrQuot[addressQuot.nAddrID] = addressQuot;

    return nErrorCode;
}
int CSvrBrokerInfoImpl::ModifyServerAddrQuot(const int nUserID, const ServerAddressQuot& addressQuot)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_SERVERADDRQUOT t set t.brokercode = '%s', t.address = '%s',t.port = %d where t.addrid = %d",
            addressQuot.szBrokerCode, addressQuot.szIP, addressQuot.nPort, addressQuot.nAddrID);

        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapServerAddrQuot[addressQuot.nAddrID] = addressQuot;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = addressQuot.nAddrID;
        strcpy(logData.logTable,"LOG_SERVERADDRQUOT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"服务器组行情地址(ID:%d   BrokerCode:%s   IP:%s    Port:%d)",
            addressQuot.nAddrID,addressQuot.szBrokerCode,addressQuot.szIP,addressQuot.nPort);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapServerAddrQuot[addressQuot.nAddrID] = addressQuot;

    return nErrorCode;
}
int CSvrBrokerInfoImpl::DelServerAddrQuot(const int nUserID, int nID)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_SERVERADDRQUOT t where t.addrid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,ServerAddressQuot>::iterator it = m_mapServerAddrQuot.find(nID);
            if ( it != m_mapServerAddrQuot.end())
                m_mapServerAddrQuot.erase(it);
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_SERVERADDRQUOT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"服务器组行情地址(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int,ServerAddressQuot>::iterator it = m_mapServerAddrQuot.find(nID);
        if ( it != m_mapServerAddrQuot.end())
            m_mapServerAddrQuot.erase(it);
    }

    return nErrorCode;
}
void CSvrBrokerInfoImpl::GetServerAddrQuot(std::vector<ServerAddressQuot>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

    vec.clear();
    std::map<int,ServerAddressQuot>::iterator it = m_mapServerAddrQuot.begin();
    for ( ; it != m_mapServerAddrQuot.end(); it++)
        vec.push_back(it->second);
}
int CSvrBrokerInfoImpl::AddTradeAccount( const int nUserID, TradeAccount& account, int& nID )
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextTradeAccountID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_TRADEACCOUNT values(%d, \'%s\', \'%s\', %d, %d, \'%s\', \'%s\', \'%s\', %d, \'%s\', %d)",
            nID, account.szTradeAccount, account.szPwd, account.nBrokerID, 
            account.nServerGroupID, account.szAlias, account.szRealName,account.szContact,
            account.nIDCardType, account.szIDCardNo, UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            account.nTradeAccountID = nID;
            m_mapTradeAccount[nID] = account;
        }
        else
        {
            m_nNextTradeAccountID--;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = account.nTradeAccountID;
        strcpy(logData.logTable,"LOG_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"委托交易账号(ID:%d   Account:%s   Password:%s    BrokerID:%d    ServerID:%d   Alias:%s    RealName:%s   Contact:%s  IDCardType:%d   IDCardNo:%s)",
            account.nTradeAccountID, account.szTradeAccount, account.szPwd, account.nBrokerID,account.nServerGroupID, account.szAlias, account.szRealName,account.szContact,
            account.nIDCardType, account.szIDCardNo);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapTradeAccount[account.nTradeAccountID] = account;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::ModifyTradeAccount( const int nUserID, const TradeAccount& account,bool& bPsw)
{
    CSafeRwLockRead Lock(m_Lock);
    
    int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.find(account.nTradeAccountID);
        if(it!=m_mapTradeAccount.end())
        {
            if(0==strcmp(account.szPwd,it->second.szPwd))
                bPsw = false;
            else
                bPsw = true;
        }
        
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_TRADEACCOUNT t set t.password = \'%s\',t.brokerid = %d, t.servergroupid = %d, t.alias = \'%s\', t.realname = \'%s\', t.contact = \'%s\', t.idcardtype = %d, t.idcardno = \'%s\'where t.tradeaccountid = %d",
            account.szPwd, account.nBrokerID, account.nServerGroupID,
            account.szAlias, account.szRealName, account.szContact, account.nIDCardType,
            account.szIDCardNo, account.nTradeAccountID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapTradeAccount[account.nTradeAccountID] = account;
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = account.nTradeAccountID;
        strcpy(logData.logTable,"LOG_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"委托交易账号(ID:%d   Account:%s   Password:%s    BrokerID:%d    ServerID:%d   Alias:%s    RealName:%s   Contact:%s  IDCardType:%d   IDCardNo:%s)",
            account.nTradeAccountID, account.szTradeAccount, account.szPwd, account.nBrokerID,account.nServerGroupID, account.szAlias, account.szRealName,account.szContact,
            account.nIDCardType, account.szIDCardNo);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapTradeAccount[account.nTradeAccountID] = account;

	return nErrorCode;
}

int CSvrBrokerInfoImpl::DelTradeAccount( const int nUserID, int nID )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_TRADEACCOUNT t where t.tradeaccountid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.find(nID);
            if ( it != m_mapTradeAccount.end())
                m_mapTradeAccount.erase(it);
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"委托交易账号(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.find(nID);
        if ( it != m_mapTradeAccount.end())
            m_mapTradeAccount.erase(it);
    }

	return nErrorCode;
}

void CSvrBrokerInfoImpl::GetTradeAccounts( std::vector<TradeAccount>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();
	for ( ; it != m_mapTradeAccount.end(); it++)
		vec.push_back(it->second);
}

void CSvrBrokerInfoImpl::GetTradeAccountsByBrokerID( int nBrokerID, std::vector<TradeAccount>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();
	for ( ; it != m_mapTradeAccount.end(); it++)
		if ( it->second.nBrokerID == nBrokerID )
			vec.push_back(it->second);
}

void CSvrBrokerInfoImpl::GetTradeAccountsByGroupID( int nGroupID, std::vector<TradeAccount>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();
	for ( ; it != m_mapTradeAccount.end(); it++)
		if ( it->second.nServerGroupID == nGroupID )
			vec.push_back(it->second);
}

bool CSvrBrokerInfoImpl::GetTradeAccount( int nTradeAccountID, TradeAccount& account )
{
    CSafeRwLockRead Lock(m_Lock);

	std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.find(nTradeAccountID);
	if ( it != m_mapTradeAccount.end())
	{
        account = it->second;
        return true;
	}

	return false;
}

bool CSvrBrokerInfoImpl::GetSettleServerGroup()
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,ServerGroup>::iterator it = m_mapServerGroup.begin();
    for (;it!=m_mapServerGroup.end();it++)
    {
        if(it->second.nIsSettle)
            return true;
    }

    return false;
}

//获取所有委托交易账号和组织机构的关系
void CSvrBrokerInfoImpl::GetTradeAccountAndOrgRelation(std::vector<TradeAccountAndOrg>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

    vec.clear();
    for(std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.begin();it!=m_mapTradeAccountAndOrgRelation.end();it++)
        vec.push_back(it->second);
}
//通过委托交易账号ID获取组织机构ID
void CSvrBrokerInfoImpl::GetOrgIDByTradeAccountID(const int& nTradeAccountID, int& nOrgID)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.find(nTradeAccountID);
    if(it!=m_mapTradeAccountAndOrgRelation.end())
        nOrgID = it->second.nOrgID;
}
//通过委托交易账号字符串获取组织机构ID
void CSvrBrokerInfoImpl::GetOrgIDByBrokerCodeAndTradeAccount(const std::string& szBrokerCode, const std::string& szTradeAccount, int& nOrgID)
{
    CSafeRwLockRead Lock(m_Lock);

    int nTradeAccountID;
    for(std::map<int,TradeAccount>::iterator it_account = m_mapTradeAccount.begin();it_account!=m_mapTradeAccount.end();it_account++)
    {
        int nBrokerID = it_account->second.nBrokerID;
        
        BrokerInfo oBrokerInfo;
        memset(&oBrokerInfo,0,sizeof(BrokerInfo));

        std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(nBrokerID);
        if(it != m_mapBrokerInfo.end())
            oBrokerInfo = it->second;
        else 
            continue;

        if(0==strcmp(szTradeAccount.c_str(),it_account->second.szTradeAccount) &&
           0==strcmp(szBrokerCode.c_str(),oBrokerInfo.szBrokerCode))
        {
            nTradeAccountID = it_account->first;
            break;
        }
    }

    std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.find(nTradeAccountID);
    if(it!=m_mapTradeAccountAndOrgRelation.end())
        nOrgID = it->second.nOrgID;    
}
//通过组织机构ID得到委托交易账户
void CSvrBrokerInfoImpl::GetTradeAccountByOrgID(const int& nOrgID, std::vector<TradeAccount>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

    int nTradeAccountID;
    for(std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.begin();it!=m_mapTradeAccountAndOrgRelation.end();it++)
    {
        if(nOrgID == it->second.nOrgID)
        {
            nTradeAccountID = it->first;

            std::map<int,TradeAccount>::iterator it_account = m_mapTradeAccount.find(nTradeAccountID);
            if(it_account!=m_mapTradeAccount.end())
                vec.push_back(it_account->second);
        }
    }
}

int CSvrBrokerInfoImpl::SaveTradeAccountAndOrgRelation(const int nUserID, const TradeAccountAndOrg& relation)
{
    CSafeRwLockRead Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {    
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));

        std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.find(relation.nTradeAccountID);
        if(it!=m_mapTradeAccountAndOrgRelation.end())
            sprintf(szBuffer, "update RELA_TRADEACCOUNT_ORG t set t.orgid = %d where t.tradeaccountid = %d",relation.nOrgID,relation.nTradeAccountID);
        else
            sprintf(szBuffer, "insert into RELA_TRADEACCOUNT_ORG values(%d, %d)",relation.nTradeAccountID,relation.nOrgID);

        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            m_mapTradeAccountAndOrgRelation[relation.nTradeAccountID] = relation;

            SYSTEMTIME sysTime;
            ::GetLocalTime(&sysTime);

            //log入库信息
            int nLogErrorCode; 
            LogBaseData logData;
            memset(&logData,0,sizeof(LogBaseData));

            logData.logTargetorID = relation.nTradeAccountID;
            strcpy(logData.logTable,"LOG_RELA_TRADEACCOUNT_ORG");
            sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
                sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
            logData.logOperatorID = nUserID;
            logData.logAction     = Oper_Modify;
            sprintf(logData.logContent,"委托交易账号与组织机构关系(AccountID:%d  OrgID:%d)",relation.nOrgID,relation.nTradeAccountID);
            logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

            CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
        }
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapTradeAccountAndOrgRelation[relation.nTradeAccountID] = relation;

    return nErrorCode;
}

bool CSvrBrokerInfoImpl::IsUpdateTradeAccountAndOrgRelation(const TradeAccountAndOrg& relation)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.find(relation.nTradeAccountID);
    if(it!=m_mapTradeAccountAndOrgRelation.end())
        return true;

    return false;
}

bool CSvrBrokerInfoImpl::LoadDB()
{
	//读取经纪公司
	LoadBrokerInfoFromDB();

	//读取服务器组
	LoadServerGroupFromDB();

	//读取服务器地址
	LoadServerAddressFromDB();

    //读取行情服务器地址
    LoadServerAddressQuotFromDB();

	//读取委托交易账户
	LoadTradeAccountFromDB();

    //读取委托交易账户与组织机构关系
    LoadTradeAccountAndOrgRelationFromDB();

	return true;
}

bool CSvrBrokerInfoImpl::LoadBrokerInfoFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

	m_mapBrokerInfo.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from BASE_BROKER t order by t.brokerid ASC");
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
        //如果是 m_nNextBrokerID则从个数算起
        //如果否 m_nNextBrokerID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nBrokerID = vValue[0].intVal;

        if(((nBrokerID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nBrokerID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nBrokerID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextBrokerID = (nBrokerID&0x00007FFF);

		for ( UINT i = 0; i < vec.size(); i++ )
		{
			BrokerInfo brokerInfo;
			memset(&brokerInfo, 0, sizeof(brokerInfo));

			std::vector<_variant_t> vValue = vec[i];

			int nIndex = 0;
			brokerInfo.nBrokerID = vValue[nIndex++].intVal;
			strcpy(brokerInfo.szBrokerCode, vValue[nIndex++].operator _bstr_t());
			strcpy(brokerInfo.szCompany, vValue[nIndex++].operator _bstr_t());
            brokerInfo.nType = vValue[nIndex++].intVal;
            strcpy(brokerInfo.szAccount, vValue[nIndex++].operator _bstr_t());
            strcpy(brokerInfo.szPassword, vValue[nIndex++].operator _bstr_t());

			m_mapBrokerInfo[brokerInfo.nBrokerID] = brokerInfo;		
    	}
	}

	return true;
}

bool CSvrBrokerInfoImpl::LoadServerGroupFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

	m_mapServerGroup.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from BASE_SERVERGROUP t order by t.serverid ASC");
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
        //如果是 m_nNextServerGroupID则从个数算起
        //如果否 m_nNextServerGroupID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nServerGroupID = vValue[0].intVal;

        if(((nServerGroupID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nServerGroupID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nServerGroupID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextServerGroupID = (nServerGroupID&0x00007FFF);

		for ( UINT i = 0; i < vec.size(); i++ )
		{
			ServerGroup info;
			memset(&info, 0, sizeof(info));

			std::vector<_variant_t> vValue = vec[i];

			int nIndex = 0;
			info.nServerID = vValue[nIndex++].intVal;
            strcpy(info.szServerName, vValue[nIndex++].operator _bstr_t());
			info.nBrokerID = vValue[nIndex++].intVal;
			info.nIsSettle = vValue[nIndex++].intVal;

			m_mapServerGroup[info.nServerID] = info;
		}
	}

	return true;
}

bool CSvrBrokerInfoImpl::LoadServerAddressFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

	m_mapServerAddr.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from BASE_SERVERADDR t order by t.addrid ASC");
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
        //如果是 m_nNextAddrID则从个数算起
        //如果否 m_nNextAddrID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nAddrID = vValue[0].intVal;

        if(((nAddrID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nAddrID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nAddrID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextAddrID = (nAddrID&0x00007FFF);

		for ( UINT i = 0; i < vec.size(); i++ )
		{
			ServerAddress info;
			memset(&info, 0, sizeof(info));

			std::vector<_variant_t> vValue = vec[i];

			int nIndex = 0;
			info.nAddrID = vValue[nIndex++].intVal;
			info.nServerID = vValue[nIndex++].intVal;
			strcpy(info.szIP, vValue[nIndex++].operator _bstr_t());
			info.nPort = vValue[nIndex++].intVal;

			m_mapServerAddr[info.nAddrID] = info;
		}
	}

	return true;
}

bool CSvrBrokerInfoImpl::LoadServerAddressQuotFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapServerAddrQuot.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_SERVERADDRQUOT t order by t.addrid ASC");
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
        //如果是 m_nNextAddrQuotID则从个数算起
        //如果否 m_nNextAddrQuotID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nAddrQuotID = vValue[0].intVal;

        if(((nAddrQuotID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nAddrQuotID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nAddrQuotID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextAddrQuotID = (nAddrQuotID&0x00007FFF);

        for ( UINT i = 0; i < vec.size(); i++ )
        {
            ServerAddressQuot info;
            memset(&info, 0, sizeof(info));

            std::vector<_variant_t> vValue = vec[i];

            int nIndex = 0;
            info.nAddrID = vValue[nIndex++].intVal;
            strcpy(info.szBrokerCode, vValue[nIndex++].operator _bstr_t());
            strcpy(info.szIP, vValue[nIndex++].operator _bstr_t());
            info.nPort = vValue[nIndex++].intVal;

            m_mapServerAddrQuot[info.nAddrID] = info;
        }
    }

    return true;
}

bool CSvrBrokerInfoImpl::LoadTradeAccountFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;

	m_mapTradeAccount.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from BASE_TRADEACCOUNT t order by t.tradeaccountid ASC");
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
        //如果是 m_nNextTradeAccountID则从个数算起
        //如果否 m_nNextTradeAccountID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nTradeAccountID = vValue[0].intVal;

        if(((nTradeAccountID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nTradeAccountID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nTradeAccountID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextTradeAccountID = (nTradeAccountID&0x00007FFF);

		for ( UINT i = 0; i < vec.size(); i++ )
		{
			TradeAccount info;
			memset(&info, 0, sizeof(info));

			std::vector<_variant_t> vValue = vec[i];

			int nIndex = 0;
			info.nTradeAccountID = vValue[nIndex++].intVal;
			strcpy(info.szTradeAccount, vValue[nIndex++].operator _bstr_t());
			strcpy(info.szPwd, vValue[nIndex++].operator _bstr_t());
			info.nBrokerID = vValue[nIndex++].intVal;
			info.nServerGroupID = vValue[nIndex++].intVal;
			strcpy(info.szAlias, vValue[nIndex++].operator _bstr_t());
			strcpy(info.szRealName, vValue[nIndex++].operator _bstr_t());
			strcpy(info.szContact, vValue[nIndex++].operator _bstr_t());
			info.nIDCardType = vValue[nIndex++].intVal;
			strcpy(info.szIDCardNo, vValue[nIndex++].operator _bstr_t());
// 			info.dProfit = vValue[nIndex++].dblVal;

			m_mapTradeAccount[info.nTradeAccountID] = info;
		}
	}

	return true;
}
bool CSvrBrokerInfoImpl::LoadTradeAccountAndOrgRelationFromDB()
{
    CSafeRwLockRead Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapTradeAccountAndOrgRelation.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from RELA_TRADEACCOUNT_ORG t order by t.tradeaccountid ASC");
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
        //如果是 m_nNextTradeAccountID则从个数算起
        //如果否 m_nNextTradeAccountID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nTradeAccountID = vValue[0].intVal;

        if(((nTradeAccountID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nTradeAccountID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nTradeAccountID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextTradeAccountID = (nTradeAccountID&0x00007FFF);

        for ( UINT i = 0; i < vec.size(); i++ )
        {
            TradeAccountAndOrg relation;
            memset(&relation, 0, sizeof(relation));

            std::vector<_variant_t> vValue = vec[i];

            int nIndex = 0;
            relation.nTradeAccountID = vValue[nIndex++].intVal;
            relation.nOrgID = vValue[nIndex++].intVal;

            m_mapTradeAccountAndOrgRelation[relation.nTradeAccountID] = relation;
        }
    }

    return true;
}