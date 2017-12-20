#pragma warning(disable : 4996)

#include <string>
#include "SvrUserOrg.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "SvrUserOrgImpl.h"
#include "../SvrBrokerInfo/Interface_SvrBrokerInfo.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "SafeRwLock.h"
#include "CommonMacro.h"

#include "..\\SvrTcp\\Interface_SvrTcp.h"
//-----------------------------------------------------------------------------------
//	import Tools4dll库
//-----------------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

//-----------------------------------------------------------------------------------
//	下面import本模块需要引用的其它模块
//-----------------------------------------------------------------------------------
#pragma comment(lib, "SvrTcp.lib")

CSvrUserOrgImpl* CSvrUserOrgImpl::m_pObj=NULL;


CSvrUserOrgImpl::CSvrUserOrgImpl()
: m_nNextUserID(0)
, m_nNextOrgID(0)
{
    uv__rwlock_fallback_init(&m_Lock);	
}

CSvrUserOrgImpl::~CSvrUserOrgImpl()
{
    uv__rwlock_fallback_destroy(&m_Lock);
}

CSvrUserOrgImpl& CSvrUserOrgImpl::getObj(void)
{
	g_mutex.lock();
	if(!m_pObj)
		m_pObj=new CSvrUserOrgImpl();
	g_mutex.unlock();
	return *m_pObj;
}

void CSvrUserOrgImpl::deleteObj()
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
CF_ERROR CSvrUserOrgImpl::ResponseToAsk(const eAskType& neDataType,
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
            bool bHave = false;
            std::map<int,Organization>::iterator it = m_mapOrg.begin();
            for (;it!=m_mapOrg.end();it++)
            {
                Organization& org = it->second;
                if(nllDataID == org.nUpperLevelOrgID)
                    bHave = true;
            }

            if(bHave)
            {
                return CF_ERROR_OPERATION_DEL_ORGANIZATION_NO_LEAFNODE;
            }

//             std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
//             for(;it!=m_vUserAndOrgRelation.end();it++)
//             {
//                 if(it->nOrgID == nllDataID)
//                     return CF_ERROR_OPERATION_DEL_ORGANIZATION_HAVE_TRDER;
//             }
        }
    }

    if(neDataType == AskType_Account)
    {
        //删除修改事件
        if(neChangeType == AskOperationTypeDelete)
        {   
            for(std::vector<UserAndTradeAccountRelation>::iterator it=m_vUserAndTradeAccountRelation.begin();
                it!=m_vUserAndTradeAccountRelation.end();
                ++it)
            {
                if (it->nTradeAccountID == nllDataID)
                    return CF_ERROR_OPERATION_DEL_TRADEACCOUNT;
            }
        }
    }

//     if(neDataType == AskType_Role)
//     {
//         //删除修改事件
//         if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
//         {   
//             for(std::vector<UserRole>::iterator it=m_vecUserRoleRelation.begin();
//                 it!=m_vecUserRoleRelation.end();
//                 ++it)
//             {
//                 if (it->nRoleID == nllDataID)
//                 {
//                     int nUserID = it->nUserID;
//                     return CF_ERROR_OPERATION_DEL_ROLE;
//                 }
//             }
//         }
//     }

    if(neDataType == AskType_QueryPrivilege)
    {
        int nPrivilegeID = (int)(*npDataBuf);

        if(!bUserHavePrivilege(nllDataID,nPrivilegeID))
            return CF_ERROR_COMMON_NO_PRIVILEGE;
    }
    
    return lRet;
}

void CSvrUserOrgImpl::SetServerRunMode(eServerRunMode nMode)
{
    CSafeRwLockWrite Lock(m_Lock);

    m_ServerRunMode = nMode;
    m_permissionMgr.SetServerRunMode(nMode);
}

char* CSvrUserOrgImpl::GetData(int& nDataLen)
{
    CSafeRwLockWrite Lock(m_Lock);

    char* p = NULL;
    int nLen = 0;

    std::map<int,RoleInfo> mapRoleInfo = m_permissionMgr.GetRoleInfo();
    std::vector<Privilege> vecPrivilege = m_permissionMgr.GetPrivilege();
    std::vector<RoleAndPrivilegeRelation> vecRoleAndPrivilegeRelation = m_permissionMgr.GetRoleAndPrivilegeRelation();

    int nOrgSize = m_mapOrg.size();   
    int nUserInfoSize = m_mapUserInfo.size();    
    int nRoleInfoSize = mapRoleInfo.size();    
    int nPrivilegeSize = vecPrivilege.size();    
    int nRoleAndPrivilegeRelationSize = vecRoleAndPrivilegeRelation.size();    
    int nUserAndTradeAccountRelationSize = m_vUserAndTradeAccountRelation.size();    
    int nUserAndOrgRelationSize = m_vUserAndOrgRelation.size();    
    int nUserRoleRelationSize = m_vecUserRoleRelation.size(); 

    nLen =  sizeof(int)+
            sizeof(int)+
            sizeof(int)+
            sizeof(int)+
            sizeof(int)+
            sizeof(int)+
            sizeof(int)+
            sizeof(int)+
            nOrgSize*sizeof(Organization)+
            nUserInfoSize*sizeof(UserInfo)+
            nRoleInfoSize*sizeof(RoleInfo)+
            nPrivilegeSize*sizeof(Privilege)+
            nRoleAndPrivilegeRelationSize*sizeof(RoleAndPrivilegeRelation)+
            nUserAndTradeAccountRelationSize*sizeof(UserAndTradeAccountRelation)+
            nUserAndOrgRelationSize*sizeof(UserAndOrgRelation)+
            nUserRoleRelationSize*sizeof(UserRole);


    char* pBuffer = new char[nLen];
    p = pBuffer;

    memset(pBuffer, 0, nDataLen);
    memcpy(pBuffer+0*sizeof(int), &nOrgSize, sizeof(int));
    memcpy(pBuffer+1*sizeof(int), &nUserInfoSize, sizeof(int));
    memcpy(pBuffer+2*sizeof(int), &nRoleInfoSize, sizeof(int));
    memcpy(pBuffer+3*sizeof(int), &nPrivilegeSize, sizeof(int));
    memcpy(pBuffer+4*sizeof(int), &nRoleAndPrivilegeRelationSize, sizeof(int));
    memcpy(pBuffer+5*sizeof(int), &nUserAndTradeAccountRelationSize, sizeof(int));
    memcpy(pBuffer+6*sizeof(int), &nUserAndOrgRelationSize, sizeof(int));
    memcpy(pBuffer+7*sizeof(int), &nUserRoleRelationSize, sizeof(int));

    int nSize = 0;
    Organization objOrganization;
    std::vector<Organization> vecOrganization;
    for (std::map<int,Organization>::iterator it = m_mapOrg.begin();it!=m_mapOrg.end();++it)
    {
        objOrganization = it->second;
        vecOrganization.push_back(objOrganization);
    }
    if(nOrgSize>0)
        memcpy(pBuffer+8*sizeof(int), &vecOrganization[0], nOrgSize*sizeof(Organization));
    nSize += nOrgSize*sizeof(Organization);

    UserInfo objUserInfo;
    std::vector<UserInfo> vecUserInfo;
    for (std::map<int,UserInfo>::iterator it = m_mapUserInfo.begin();it!=m_mapUserInfo.end();++it)
    {
        objUserInfo = it->second;
        vecUserInfo.push_back(objUserInfo);
    }
    if(nUserInfoSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &vecUserInfo[0], nUserInfoSize*sizeof(UserInfo));
    nSize += nUserInfoSize*sizeof(UserInfo);

    RoleInfo objRoleInfo;
    std::vector<RoleInfo> vecRoleInfo;
    for (std::map<int,RoleInfo>::iterator it = mapRoleInfo.begin();it!=mapRoleInfo.end();++it)
    {
        objRoleInfo = it->second;
        vecRoleInfo.push_back(objRoleInfo);
    }
    if(nRoleInfoSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &vecRoleInfo[0], nRoleInfoSize*sizeof(RoleInfo));
    nSize += nRoleInfoSize*sizeof(RoleInfo);

    if(nPrivilegeSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &vecPrivilege[0], nPrivilegeSize*sizeof(Privilege));
    nSize += nPrivilegeSize*sizeof(Privilege);

    if(nRoleAndPrivilegeRelationSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &vecRoleAndPrivilegeRelation[0], nRoleAndPrivilegeRelationSize*sizeof(RoleAndPrivilegeRelation));
    nSize += nRoleAndPrivilegeRelationSize*sizeof(RoleAndPrivilegeRelation);

    if(nUserAndTradeAccountRelationSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &m_vUserAndTradeAccountRelation[0], nUserAndTradeAccountRelationSize*sizeof(UserAndTradeAccountRelation));
    nSize += nUserAndTradeAccountRelationSize*sizeof(UserAndTradeAccountRelation);

    if(nUserAndOrgRelationSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &m_vUserAndOrgRelation[0], nUserAndOrgRelationSize*sizeof(UserAndOrgRelation));
    nSize += nUserAndOrgRelationSize*sizeof(UserAndOrgRelation);

    if(nUserRoleRelationSize>0)
        memcpy(pBuffer+8*sizeof(int)+nSize, &m_vecUserRoleRelation[0], nUserRoleRelationSize*sizeof(UserRole));
    
    nDataLen = nLen;
    return p;
}

void CSvrUserOrgImpl::SetData(	    std::map<int,Organization>& mapOrg,
                                    std::map<int,UserInfo>& mapUserInfo,
                                    std::map<int,RoleInfo>& mapRoleInfo,
                                    std::vector<Privilege>& vPrivilege,
                                    std::vector<RoleAndPrivilegeRelation>& vRoleAndPrivilegeRelation,
                                    std::vector<UserAndTradeAccountRelation>& vUserAndTradeAccountRelation,
                                    std::vector<UserAndOrgRelation>& vUserAndOrgRelation,
                                    std::vector<UserRole>& vUserRoleRelation)
{
    CSafeRwLockWrite Lock(m_Lock);

    m_mapOrg = mapOrg;
    m_mapUserInfo = mapUserInfo;
    m_permissionMgr.SetRoleInfo(mapRoleInfo);
    m_permissionMgr.SetPrivilege(vPrivilege);
    m_permissionMgr.SetRoleAndPrivilegeRelation(vRoleAndPrivilegeRelation);
    m_vUserAndTradeAccountRelation = vUserAndTradeAccountRelation;
    m_vUserAndOrgRelation = vUserAndOrgRelation;
    m_vecUserRoleRelation = vUserRoleRelation;

    
    for ( std::map<int, Organization>::iterator it_org = m_mapOrg.begin(); it_org != m_mapOrg.end(); it_org++ )
    {
        Organization info = it_org->second;
        if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
        {
            std::map<int, Organization> tempOrgMap;
            m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
        }

        std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
        if ( it != m_mapAssetMgmtOrgTree.end() )
        {
            it->second[info.nAssetMgmtOrgID] = info;
        }
        else
        {
            std::map<int, Organization> tempOrgMap;
            tempOrgMap[info.nAssetMgmtOrgID] = info;
            m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
        }	
    }


    UserRole info;
    for ( int i = 0; i < (int)m_vecUserRoleRelation.size(); i++ )
    {
        info = m_vecUserRoleRelation[i];
        m_mapUserRoleRelation[info.nUserID].push_back(info);
    }

    RoleAndPrivilegeRelation relation;
    std::map<int,std::vector<int>> mapRoleID2PrivilegeRelation;
    for ( int i = 0; i < (int)vRoleAndPrivilegeRelation.size(); i++ )
    {
        relation = vRoleAndPrivilegeRelation[i];
        mapRoleID2PrivilegeRelation[relation.nRoleID].push_back(relation.nPrivilegeID);
    }
    m_permissionMgr.SetRoleID2PrivilegeRelation(mapRoleID2PrivilegeRelation);
}


int CSvrUserOrgImpl::AddOrg( const int nUserID, Organization& org, int& nID )
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextOrgID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_ORGANIZATION values(%d, \
                          '%s', %d,%d,%.6f,%.6f,%d)",
                          nID, org.szName, org.nUpperLevelOrgID,
                          org.nScaleLevel, org.dLongitude, org.dLatitude,UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            org.nAssetMgmtOrgID = nID;
            m_mapOrg[org.nAssetMgmtOrgID] = org;


            if(m_mapAssetMgmtOrgTree.find(org.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
            {
                std::map<int, Organization> tempOrgMap;
                m_mapAssetMgmtOrgTree[org.nAssetMgmtOrgID] = tempOrgMap;
            }

            std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(org.nUpperLevelOrgID);
            if ( it != m_mapAssetMgmtOrgTree.end() )
            {
                it->second[org.nAssetMgmtOrgID] = org;
            }
            else
            {
                std::map<int, Organization> tempOrgMap;
                tempOrgMap[org.nAssetMgmtOrgID] = org;
                m_mapAssetMgmtOrgTree[org.nUpperLevelOrgID] = tempOrgMap;
            }	
        }
        else
        {
            m_nNextOrgID--;
        }


        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = org.nAssetMgmtOrgID;
        strcpy(logData.logTable,"LOG_ORGANIZATION");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"组织机构(ID:%d   Name:%s   UpperLevelID:%d   ScaleLevel:%d    Longitude:%.6f    Latitude:%.6f)",
            org.nAssetMgmtOrgID, org.szName, org.nUpperLevelOrgID,org.nScaleLevel, org.dLongitude, org.dLatitude);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        m_mapOrg[org.nAssetMgmtOrgID] = org;

        if(m_mapAssetMgmtOrgTree.find(org.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
        {
            std::map<int, Organization> tempOrgMap;
            m_mapAssetMgmtOrgTree[org.nAssetMgmtOrgID] = tempOrgMap;
        }

        std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(org.nUpperLevelOrgID);
        if ( it != m_mapAssetMgmtOrgTree.end() )
        {
            it->second[org.nAssetMgmtOrgID] = org;
        }
        else
        {
            std::map<int, Organization> tempOrgMap;
            tempOrgMap[org.nAssetMgmtOrgID] = org;
            m_mapAssetMgmtOrgTree[org.nUpperLevelOrgID] = tempOrgMap;
        }	
    }

	return nErrorCode;
}

int CSvrUserOrgImpl::ModifyOrg( const int nUserID, const Organization& org )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_ORGANIZATION t set t.name = '%s', t.upperlevelorgid = %d, t.longitude = %.6f, \
            t.latitude = %.6f where t.assetmgmtorgid = %d",
            org.szName, org.nUpperLevelOrgID, org.dLongitude, org.dLatitude, org.nAssetMgmtOrgID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
            {
                m_mapOrg[org.nAssetMgmtOrgID] = org;

                Organization info = org;
                if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
                {
                    std::map<int, Organization> tempOrgMap;
                    m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
                }

                std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
                if ( it != m_mapAssetMgmtOrgTree.end() )
                {
                    it->second[info.nAssetMgmtOrgID] = info;
                }
                else
                {
                    std::map<int, Organization> tempOrgMap;
                    tempOrgMap[info.nAssetMgmtOrgID] = info;
                    m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
                }                
            }
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = org.nAssetMgmtOrgID;
        strcpy(logData.logTable,"LOG_ORGANIZATION");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"组织机构(ID:%d   Name:%s   UpperLevelID:%d   ScaleLevel:%d    Longitude:%.6f    Latitude:%.6f)",
            org.nAssetMgmtOrgID, org.szName, org.nUpperLevelOrgID,org.nScaleLevel, org.dLongitude, org.dLatitude);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        m_mapOrg[org.nAssetMgmtOrgID] = org;

        Organization info = org;
        if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
        {
            std::map<int, Organization> tempOrgMap;
            m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
        }

        std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
        if ( it != m_mapAssetMgmtOrgTree.end() )
        {
            it->second[info.nAssetMgmtOrgID] = info;
        }
        else
        {
            std::map<int, Organization> tempOrgMap;
            tempOrgMap[info.nAssetMgmtOrgID] = info;
            m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
        }                
    }

	return nErrorCode;
}

int CSvrUserOrgImpl::DelOrg( const int nUserID, int nID )
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_ORGANIZATION t where t.assetmgmtorgid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,Organization>::iterator it = m_mapOrg.find(nID);
            if ( it != m_mapOrg.end())
                m_mapOrg.erase(it); 
        }

        //关系操作
        //将所有是从属关系的交易员、风控员的部门设置为总部
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "update RELA_USER_ORG t set t.orgid = %d where t.orgid = %d and t.relationtype=%d", ORGNIZATION_ROOT, nID, USER_ORG_RELATION_ATTACH);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
            for (;it!=m_vUserAndOrgRelation.end();it++)
            {
                if(it->nOrgID == nID && it->nRelationType == USER_ORG_RELATION_ATTACH)
                    it->nOrgID = ORGNIZATION_ROOT;
            }        
        }

        //将所有是监控关系的风控员和部门删除
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete RELA_USER_ORG t where t.orgid = %d and t.relationtype=%d", nID, USER_ORG_RELATION_MONITOR);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
            while (it!=m_vUserAndOrgRelation.end())
            {
                if(it->nOrgID == nID && it->nRelationType == USER_ORG_RELATION_MONITOR)
                    it = m_vUserAndOrgRelation.erase(it);
                else
                    it++;
            }        
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_ORGANIZATION");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"组织机构(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);


        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_RELA_USER_ORG");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"组织机构(ID:%d):将所有是从属关系的交易员、风控员的部门设置为总部,将所有是监控关系的风控员和部门删除",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        Organization org;
        std::map<int,Organization>::iterator it = m_mapOrg.find(nID);
        if ( it != m_mapOrg.end())
        {
            org = it->second;
            m_mapOrg.erase(it); 
        }
        
        std::map<int, std::map<int, Organization>>::iterator it_tree = m_mapAssetMgmtOrgTree.find(nID);
        if ( it_tree != m_mapAssetMgmtOrgTree.end() )
            it_tree=m_mapAssetMgmtOrgTree.erase(it_tree);

        it_tree = m_mapAssetMgmtOrgTree.find(org.nUpperLevelOrgID);
        if ( it_tree != m_mapAssetMgmtOrgTree.end() )
        {
            std::map<int, Organization>& subOrg = it_tree->second;
            std::map<int,Organization>::iterator it_org = subOrg.find(nID);
            if ( it_org != subOrg.end())
                subOrg.erase(it_org); 
        }
       

        for (std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();it!=m_vUserAndOrgRelation.end();it++)
        {
            if(it->nOrgID == nID && it->nRelationType == USER_ORG_RELATION_ATTACH)
                it->nOrgID = ORGNIZATION_ROOT;
        }

        std::vector<UserAndOrgRelation>::iterator it_rela = m_vUserAndOrgRelation.begin();
        while (it_rela!=m_vUserAndOrgRelation.end())
        {
            if(it_rela->nOrgID == nID && it_rela->nRelationType == USER_ORG_RELATION_MONITOR)
                it_rela = m_vUserAndOrgRelation.erase(it_rela);
            else
                it_rela++;
        }   
    }

	return nErrorCode;
}

void CSvrUserOrgImpl::GetOrgs( std::vector<Organization>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,Organization>::iterator it = m_mapOrg.begin();
	for ( ; it != m_mapOrg.end(); it++)
		vec.push_back(it->second);
}

bool CSvrUserOrgImpl::GetOrgByID(int nOrgID, Organization& org)
{
    CSafeRwLockRead Lock(m_Lock);

	std::map<int,Organization>::iterator it = m_mapOrg.find(nOrgID);
	if ( it != m_mapOrg.end())
	{
		org = it->second;
		return true;		
	}

    return false;
}

bool CSvrUserOrgImpl::IsLeafNode(int nOrgID)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,Organization>::iterator it = m_mapOrg.begin();
    for (;it!=m_mapOrg.end();it++)
    {
        Organization& org = it->second;
        if(nOrgID == org.nUpperLevelOrgID)
             return false;
    }

    return true;
}
// void CSvrUserOrgImpl::SetUserAssign(int nUserID, bool bAssign)
// {
//     std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
//     if( it != m_mapUserInfo.end())
//         it->second.nAssign = bAssign;
// }

int CSvrUserOrgImpl::AddUserInfo( const int nUserID, UserInfo& userInfo, int& nID )
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

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextUserID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_USER values(%d,'%s','%s',%d,%d,'%s','%s',%d,%d)",
            nID, userInfo.szAccount, userInfo.szPassword,
            userInfo.nUserType, userInfo.nStatus,
            userInfo.szAlias, userInfo.szContactInfo, userInfo.nAssign,UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            userInfo.nUserID = nID;
            m_mapUserInfo[userInfo.nUserID] = userInfo;
        }
        else
        {
            m_nNextUserID--;
        }


        //用户默认分配到总部下边
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into RELA_USER_ORG values(%d, %d, %d, %d, %d)", userInfo.nUserID, ORGNIZATION_ROOT, USER_ORG_RELATION_ATTACH, userInfo.nUserType, UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            UserAndOrgRelation relation;
            relation.nUserID = nID;
            relation.nOrgID = ORGNIZATION_ROOT;
            relation.nUserType = userInfo.nUserType;
            relation.nRelationType = USER_ORG_RELATION_ATTACH;

            m_vUserAndOrgRelation.push_back(relation); 
        }    


        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = userInfo.nUserID;
        strcpy(logData.logTable,"LOG_USER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"用户(ID:%d   Account:%s   Password:%s   Type:%d    Status:%d    Alias:%s    Contact:%s    Assign:%d)",
            userInfo.nUserID, userInfo.szAccount, userInfo.szPassword,userInfo.nUserType, userInfo.nStatus,userInfo.szAlias, userInfo.szContactInfo, userInfo.nAssign);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);



        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = userInfo.nUserID;
        strcpy(logData.logTable,"LOG_RELA_USER_ORG");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"用户(ID:%d):组织机构与用户的关系(UserType：%d   OrgID：%d   RelationType: %d)",userInfo.nUserID, userInfo.nUserType,ORGNIZATION_ROOT,USER_ORG_RELATION_ATTACH);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        m_mapUserInfo[userInfo.nUserID] = userInfo;

        UserAndOrgRelation relation;
        relation.nUserID = userInfo.nUserID;
        relation.nOrgID = ORGNIZATION_ROOT;
        relation.nUserType = userInfo.nUserType;
        relation.nRelationType = USER_ORG_RELATION_ATTACH;

        m_vUserAndOrgRelation.push_back(relation); 
    }

	return nErrorCode;
}

int CSvrUserOrgImpl::ModifyUserInfo( const int nUserID, const UserInfo& userInfo )
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_USER t set t.account = '%s',t.password = '%s',t.roletype = %d,\
            t.status = %d,t.name = '%s',t.contactinfo = '%s' where t.userid = %d",
            userInfo.szAccount, userInfo.szPassword, userInfo.nUserType,
            userInfo.nStatus, userInfo.szAlias, userInfo.szContactInfo, userInfo.nUserID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapUserInfo[userInfo.nUserID] = userInfo;
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = userInfo.nUserID;
        strcpy(logData.logTable,"LOG_USER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"用户(ID:%d   Account:%s   Password:%s   Type:%d    Status:%d    Alias:%s    Contact:%s    Assign:%d)",
            userInfo.nUserID, userInfo.szAccount, userInfo.szPassword,userInfo.nUserType, userInfo.nStatus,userInfo.szAlias, userInfo.szContactInfo, userInfo.nAssign);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapUserInfo[userInfo.nUserID] = userInfo;

	return nErrorCode;
}

int CSvrUserOrgImpl::ModifyUserPasswordFromRisk(UserPassword& userPassword, int& nID)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(userPassword.nUserID);
        if( it != m_mapUserInfo.end() )
            if( strcmp(userPassword.szOldPassword,it->second.szPassword))
            {
                nErrorCode = CF_ERROR_LOGIN_USER_PASSWORD_ERROR;
                return nErrorCode;
            }

            char szBuffer[MAX_SQL_LENGTH];
            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, 
                "update BASE_USER t set t.password = '%s' where t.userid = %d",
                userPassword.szNewPassword,userPassword.nUserID);
            int nNum = 0;
            if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
            {
                if ( nNum != 0 )
                {
                    nID = userPassword.nUserID;
                    memcpy(m_mapUserInfo[userPassword.nUserID].szPassword,userPassword.szNewPassword,sizeof(_PASSWORDSTRING));
                }
            }
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(userPassword.nUserID);
        if( it != m_mapUserInfo.end() )
            if( strcmp(userPassword.szOldPassword,it->second.szPassword) == 0)
                memcpy(m_mapUserInfo[userPassword.nUserID].szPassword,userPassword.szNewPassword,sizeof(_PASSWORDSTRING));
    }

    return nErrorCode;
}
int CSvrUserOrgImpl::UnLockPassword(UserPassword& userPassword, int& nID)
{
    int nErrorCode = CF_ERROR_SUCCESS;

    std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(userPassword.nUserID);
    if( it != m_mapUserInfo.end() )
        if( strcmp(userPassword.szNewPassword,it->second.szPassword))
            nErrorCode = CF_ERROR_LOGIN_USER_UNPASSWORD_ERROR;

    nID = userPassword.nUserID;
    return nErrorCode;
}
int CSvrUserOrgImpl::SetUserStatus(UserStatus& userStatus)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_USER t set t.status = %d where t.userid = %d",
            userStatus.nStatus, userStatus.nUserID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
                m_mapUserInfo[userStatus.nUserID].nStatus = userStatus.nStatus;
        }
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapUserInfo[userStatus.nUserID].nStatus = userStatus.nStatus;

    return nErrorCode;
}

int CSvrUserOrgImpl::DelUserInfo( const int nUserID, int nID )
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
	    char szBuffer[MAX_SQL_LENGTH];
	    memset(szBuffer, 0, sizeof(szBuffer));
	    sprintf(szBuffer, "delete BASE_USER t where t.userid = %d", nID);
	    if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
	    {
		    std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nID);
		    if ( it != m_mapUserInfo.end())
			    m_mapUserInfo.erase(it);
	    }

        //删除用户与角色的关系
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete RELA_USER_ROLE t where t.userid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int, std::vector<UserRole>>::iterator it = m_mapUserRoleRelation.find(nID);
            if ( it != m_mapUserRoleRelation.end())
                m_mapUserRoleRelation.erase(it);

            std::vector<UserRole>::iterator it2 = m_vecUserRoleRelation.begin();
            while ( it2 != m_vecUserRoleRelation.end())
            {
                if(it2->nUserID == nID)
                    it2 = m_vecUserRoleRelation.erase(it2);
                else
                    it2++;
            }
        }



        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_USER");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"用户(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);

        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_RELA_USER_ROLE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"用户(ID:%d):删除用户和角色的关系",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int,UserInfo>::iterator it_userinfo = m_mapUserInfo.find(nID);
        if ( it_userinfo != m_mapUserInfo.end())
            m_mapUserInfo.erase(it_userinfo);

        std::map<int, std::vector<UserRole>>::iterator it = m_mapUserRoleRelation.find(nID);
        if ( it != m_mapUserRoleRelation.end())
            m_mapUserRoleRelation.erase(it);

        std::vector<UserRole>::iterator it2 = m_vecUserRoleRelation.begin();
        while ( it2 != m_vecUserRoleRelation.end())
        {
            if(it2->nUserID == nID)
                it2 = m_vecUserRoleRelation.erase(it2);
            else
                it2++;
        }
    }

	return nErrorCode;
}

void CSvrUserOrgImpl::GetUserInfos( std::vector<UserInfo>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec.clear();
	std::map<int,UserInfo>::iterator it = m_mapUserInfo.begin();
	for ( ; it != m_mapUserInfo.end(); it++)
		vec.push_back(it->second);
}

bool CSvrUserOrgImpl::GetUserByID( int nUserID, UserInfo& userInfo )
{
    CSafeRwLockRead Lock(m_Lock);

	std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
	if ( it != m_mapUserInfo.end())
	{
		userInfo = it->second;
        return true;
	}

	return false;
}
bool CSvrUserOrgImpl::GetUserByName(std::string szUserName, UserInfo& userInfo)
{
    CSafeRwLockRead Lock(m_Lock);

    for(std::map<int,UserInfo>::iterator it = m_mapUserInfo.begin(); it != m_mapUserInfo.end();it++)
    {
        UserInfo oUserInfo = it->second;
        if(szUserName == (string)(oUserInfo.szAccount))
        {
            memcpy(&userInfo,&oUserInfo,sizeof(UserInfo));
            return true;
        }
    }

    return false;
}
void CSvrUserOrgImpl::GetUserInfosByOrgID(int nOrgID, std::vector<UserInfo>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

	std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
	for ( ; it != m_vUserAndOrgRelation.end(); it++ )
	{
		if ( it->nOrgID == nOrgID && it->nRelationType == 0 )
		{
			std::map<int,UserInfo>::iterator it_user = m_mapUserInfo.find(it->nUserID);
			if ( it_user != m_mapUserInfo.end())
				vec.push_back(it_user->second);
		}
	}
}

void CSvrUserOrgImpl::GetUserInfosByOrgID(int nOrgID, UserType nUserType, std::vector<UserInfo>& vecUserInfo)
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<UserAndOrgRelation>::iterator it=m_vUserAndOrgRelation.begin();
    for(;it!=m_vUserAndOrgRelation.end();it++)
    {
        if(it->nOrgID == nOrgID)
        {
            std::map<int,UserInfo>::iterator itUserInfo=m_mapUserInfo.find(it->nUserID);
            if(itUserInfo!=m_mapUserInfo.end() && itUserInfo->second.nUserType==nUserType)
                vecUserInfo.push_back(itUserInfo->second);
        }
    }
}
void CSvrUserOrgImpl::GetUserInfosByOrgID(int nOrgID, UserType nUserType, int nRelationType, std::vector<UserInfo>& vecUserInfo)
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<UserAndOrgRelation>::iterator it=m_vUserAndOrgRelation.begin();
    for(;it!=m_vUserAndOrgRelation.end();it++)
    {
        if(it->nOrgID == nOrgID && it->nRelationType == nRelationType)
        {
            std::map<int,UserInfo>::iterator itUserInfo=m_mapUserInfo.find(it->nUserID);
            if(itUserInfo!=m_mapUserInfo.end() && itUserInfo->second.nUserType==nUserType)
                vecUserInfo.push_back(itUserInfo->second);
        }
    }
}

void CSvrUserOrgImpl::GetUsersIDByOrgID(int nOrgID,std::vector<int>& vecUserID)
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<UserInfo> vecUserInfo;
    GetUserInfosByOrgID(nOrgID,USER_TYPE_TRADER,vecUserInfo);
    if(!vecUserInfo.empty())
    {
        std::vector<UserInfo>::iterator itUserInfo = vecUserInfo.begin();
        for (;itUserInfo!=vecUserInfo.end();itUserInfo++)
            vecUserID.push_back(itUserInfo->nUserID);
    }
 
    std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(nOrgID);
    if(it == m_mapAssetMgmtOrgTree.end())
        return;

    std::map<int, Organization>& mapSub = it->second;
    if(mapSub.begin()==mapSub.end())
        return;

    GetUsersIDByOrgID(m_mapAssetMgmtOrgTree, mapSub, vecUserID); 
}

void CSvrUserOrgImpl::GetUsersIDByOrgID(std::map<int, std::map<int, Organization>>& mapMain,std::map<int, Organization>& mapSub, std::vector<int>& vecUserID) 
{
    std::map<int, std::map<int, Organization>>::iterator it;
    std::map<int, Organization>::iterator itSub;
  
    itSub = mapSub.begin();
    for(; itSub != mapSub.end(); itSub++) 
    {
        std::vector<UserInfo> vecUserInfo;       
        GetUserInfosByOrgID(itSub->first,USER_TYPE_TRADER,vecUserInfo);       
        if(!vecUserInfo.empty())
        {
            std::vector<UserInfo>::iterator itUserInfo = vecUserInfo.begin();
            for (;itUserInfo!=vecUserInfo.end();itUserInfo++)
                vecUserID.push_back(itUserInfo->nUserID);
        }

        it = mapMain.find(itSub->first);
        if(it != mapMain.end())
            GetUsersIDByOrgID(mapMain, it->second, vecUserID);       
    }
} 

int CSvrUserOrgImpl::IsValidUser(const std::string& strAccout, const std::string& strPwd,int nUserType, int& nUserID)
{
    CSafeRwLockRead Lock(m_Lock);

	bool bAccountFind = false;
	bool bPwdMatch = false;
    bool bStatus = false;
	std::map<int,UserInfo>::iterator it = m_mapUserInfo.begin();
	for ( ; it != m_mapUserInfo.end(); it++)
	{
		if ( strAccout == it->second.szAccount &&
			nUserType == it->second.nUserType )
		{
			nUserID = it->second.nUserID;
			bAccountFind = true;
			if ( strPwd == it->second.szPassword )
			{
				bPwdMatch = true;
                if(it->second.nStatus)
                    bStatus = true;
				break;
			}
		}
	}

	if (!bAccountFind)
		return CF_ERROR_LOGIN_USER_NOT_EXIST;
	else
	{
		if(!bPwdMatch)
			return CF_ERROR_LOGIN_PASS_INVALIDATE;
		else if(!bStatus)
            return CF_ERROR_LOGIN_USER_NOT_USE;
		else
			return CF_ERROR_SUCCESS;
	}
}

UserType CSvrUserOrgImpl::GetUserType(int nUserID)
{   
    CSafeRwLockRead Lock(m_Lock);
    
    std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if(it!=m_mapUserInfo.end())
        return it->second.nUserType;

    return USER_TYPE_INVALID;
}

bool CSvrUserOrgImpl::LoadDB()
{
	//读取组织架构
	LoadOrgFromDB();

	//读取用户表
	LoadUserFromDB();

	//读取用户与交易账号关系表
	LoadUserAndTradeAccountRelationFromDB();

	//读取用户与组织架构关系表
	LoadUserAndOrgRelationFromDB();

    //读取用户与角色关系表
    LoadUserAndRoleRelationFromDB();

	//读取权限表
	m_permissionMgr.LoadDB();

	return true;
}

bool CSvrUserOrgImpl::LoadOrgFromDB()
{
	int nErrorCode = CF_ERROR_SUCCESS;

	m_mapOrg.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from BASE_ORGANIZATION t");
	std::vector<std::vector<_variant_t>> vOrg;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vOrg, nErrorCode))
	{
		return false;
	}
	else
	{
        if(vOrg.size()<=0)
            return true;

        int nDataCount = vOrg.size()-1;

        //取数据库最后一条数据，判断是否是当天保存的，
        //如果是 m_nFinancialProductID则从个数算起
        //如果否 m_nFinancialProductID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vOrg[nDataCount];
        int nOrgID = vValue[0].intVal;

        if(((nOrgID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nOrgID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nOrgID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextOrgID = (nOrgID&0x00007FFF);

		for ( UINT i = 0; i < vOrg.size(); i++ )
		{
			Organization org;
			memset(&org, 0, sizeof(org));

			std::vector<_variant_t> vValue = vOrg[i];

            int nIndex = 0;
			org.nAssetMgmtOrgID = vValue[nIndex++].intVal;
			strcpy(org.szName, vValue[nIndex++].operator _bstr_t());
			org.nUpperLevelOrgID = vValue[nIndex++].intVal;
			org.nScaleLevel = vValue[nIndex++].intVal;
			org.dLongitude = vValue[nIndex++].dblVal;
			org.dLatitude = vValue[nIndex++].dblVal;

			m_mapOrg[org.nAssetMgmtOrgID] = org;
		}

        if(!m_mapOrg.empty())
        {
            std::map<int, Organization>::iterator it_org = m_mapOrg.begin();
            for ( ; it_org != m_mapOrg.end(); it_org++ )
            {
                Organization info = it_org->second;
                if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
                {
                    std::map<int, Organization> tempOrgMap;
                    m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
                }

                std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
                if ( it != m_mapAssetMgmtOrgTree.end() )
                {
                    it->second[info.nAssetMgmtOrgID] = info;
                }
                else
                {
                    std::map<int, Organization> tempOrgMap;
                    tempOrgMap[info.nAssetMgmtOrgID] = info;
                    m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
                }	
            }
        }
	}

	return true;
}

bool CSvrUserOrgImpl::LoadUserFromDB()
{
	int nErrorCode = CF_ERROR_SUCCESS;

	m_mapUserInfo.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from BASE_USER t order by t.userid ASC");
	std::vector<std::vector<_variant_t>> vUser;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vUser, nErrorCode))
	{
		return false;
	}
	else
	{
        if(vUser.size()<=0)
            return true;

        int nDataCount = vUser.size()-1;

        //取数据库最后一条数据，判断是否是当天保存的，
        //如果是 m_nFinancialProductID则从个数算起
        //如果否 m_nFinancialProductID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vUser[nDataCount];
        int nUsrID = vValue[0].intVal;

        if(((nUsrID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nUsrID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nUsrID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextUserID = (nUsrID&0x00007FFF);

        for ( UINT i = 0; i < vUser.size(); i++ )
        {
            UserInfo info;
            memset(&info, 0, sizeof(info));

            std::vector<_variant_t> vValue = vUser[i];

            int nIndex = 0;
            info.nUserID = vValue[nIndex++].intVal;
            strcpy(info.szAccount, vValue[nIndex++].operator _bstr_t());
            strcpy(info.szPassword, vValue[nIndex++].operator _bstr_t());
            info.nUserType = (UserType)(vValue[nIndex++].intVal);
            info.nStatus = vValue[nIndex++].intVal;
            strcpy(info.szAlias, vValue[nIndex++].operator _bstr_t());
            strcpy(info.szContactInfo, vValue[nIndex++].operator _bstr_t());
            info.nAssign = vValue[nIndex++].intVal;

            m_mapUserInfo[info.nUserID] = info;
        }
	}

	return true;
}

bool CSvrUserOrgImpl::LoadUserAndTradeAccountRelationFromDB()
{
	int nErrorCode = CF_ERROR_SUCCESS;

	m_vUserAndTradeAccountRelation.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from RELA_USER_TRADEACCOUNT t");
	std::vector<std::vector<_variant_t>> vUserAndTradeAccountRelation;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vUserAndTradeAccountRelation, nErrorCode))
	{
		return false;
	}
	else
	{
		for ( UINT i = 0; i < vUserAndTradeAccountRelation.size(); i++ )
		{
			UserAndTradeAccountRelation info;
			memset(&info, 0, sizeof(info));

			std::vector<_variant_t> vValue = vUserAndTradeAccountRelation[i];

			int nIndex = 0;
			info.nUserID = vValue[nIndex++].intVal;
			info.nTradeAccountID = vValue[nIndex++].intVal;
			info.nCanInputOrder = vValue[nIndex++].intVal;
			info.nNeedVerify = vValue[nIndex++].intVal;

			m_vUserAndTradeAccountRelation.push_back(info);
		}
	}

	return true;
}

bool CSvrUserOrgImpl::LoadUserAndRoleRelationFromDB()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    m_vecUserRoleRelation.clear();
    m_mapUserRoleRelation.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from RELA_USER_ROLE t");
    std::vector<std::vector<_variant_t>> vecUserRoleRelation;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vecUserRoleRelation, nErrorCode))
    {
        return false;
    }
    else
    {
        for ( UINT i = 0; i < vecUserRoleRelation.size(); i++ )
        {
            UserRole info;
            memset(&info, 0, sizeof(info));

            std::vector<_variant_t> vValue = vecUserRoleRelation[i];

            int nIndex = 0;
            info.nUserID = vValue[nIndex++].intVal;
            info.nRoleID = vValue[nIndex++].intVal;

            m_vecUserRoleRelation.push_back(info);
            m_mapUserRoleRelation[info.nUserID].push_back(info);
        }
    }

    return true;
}

bool CSvrUserOrgImpl::LoadUserAndOrgRelationFromDB()
{
	int nErrorCode = CF_ERROR_SUCCESS;

	m_vUserAndOrgRelation.clear();
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from RELA_USER_ORG t");
	std::vector<std::vector<_variant_t>> vUserAndOrgRelation;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vUserAndOrgRelation, nErrorCode))
	{
		return false;
	}
	else
	{
		for ( UINT i = 0; i < vUserAndOrgRelation.size(); i++ )
		{
			UserAndOrgRelation info;
			memset(&info, 0, sizeof(info));

			std::vector<_variant_t> vValue = vUserAndOrgRelation[i];

			int nIndex = 0;
			info.nUserID = vValue[nIndex++].intVal;
			info.nOrgID = vValue[nIndex++].intVal;
			info.nRelationType = vValue[nIndex++].intVal;
            info.nUserType = vValue[nIndex++].intVal;

            m_vUserAndOrgRelation.push_back(info);
		}
	}

	return true;
}

int CSvrUserOrgImpl::SaveUserOrgRelation(const int nUserID, int nOrgID, int nUserType, int nTraderID, int nRelationType)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        if( CInterface_SvrDBOpr::getObj().SaveUserOrganizationRelation(nOrgID, nUserType, nTraderID, nRelationType, nErrorCode))
        {   
            std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
            while (it!=m_vUserAndOrgRelation.end())
            {
                if(it->nUserID == nTraderID && it->nUserType == nUserType && it->nRelationType == nRelationType)
                {
                    it->nOrgID = nOrgID;
                    break;
                }
                else
                    ++it;
            }
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nOrgID;
        strcpy(logData.logTable,"LOG_RELA_USER_ORG");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"组织机构与用户的关系(UserType：%d   OrgID：%d   RelationType: %d)",nUserType,nOrgID,nRelationType);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
        while (it!=m_vUserAndOrgRelation.end())
        {
            if(it->nUserID == nTraderID && it->nUserType == nUserType && it->nRelationType == nRelationType)
            {
                it->nOrgID = nOrgID;
                break;
            }
            else
                ++it;
        }
    }

    return nErrorCode;
}

int CSvrUserOrgImpl::SaveUserOrgRelation(const int nUserID, int nOrgID, int nUserType, std::vector<int>& vecInID, std::vector<int>& vecOutID, int nRelationType)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        if( CInterface_SvrDBOpr::getObj().SaveUserOrganizationRelation(nOrgID, nUserType, vecInID, vecOutID, nRelationType, nErrorCode))
        {   
            //vecOut的用户默认分配到总部下边
            int nOutOrgID = ORGNIZATION_ROOT;
            std::vector<int>::iterator it_out = vecOutID.begin();
            for ( ; it_out != vecOutID.end(); it_out++ )
            {
                std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
                while (it!=m_vUserAndOrgRelation.end())
                {
                    if(it->nUserID == (*it_out) && it->nUserType == nUserType && it->nRelationType == nRelationType)
                    {
                        it->nOrgID = nOutOrgID;
                        break;
                    }
                    else
                        ++it;
                }
            }


            std::vector<int>::iterator it_InAttch = vecInID.begin();
            for ( ; it_InAttch != vecInID.end(); it_InAttch++ )
            {
                std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
                while (it!=m_vUserAndOrgRelation.end())
                {
                    if(it->nUserID == (*it_InAttch) && it->nUserType == nUserType && it->nRelationType == nRelationType)
                    {
                        it->nOrgID = nOrgID;
                        break;
                    }
                    else
                        ++it;
                }
            }
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nOrgID;
        strcpy(logData.logTable,"LOG_RELA_USER_ORG");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"组织机构与用户的关系(UserType：%d   OrgID：%d   RelationType: %d)",nUserType,nOrgID,nRelationType);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        //vecOut的用户默认分配到总部下边
        int nOutOrgID = ORGNIZATION_ROOT;
        std::vector<int>::iterator it_out = vecOutID.begin();
        for ( ; it_out != vecOutID.end(); it_out++ )
        {
            std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
            while (it!=m_vUserAndOrgRelation.end())
            {
                if(it->nUserID == (*it_out) && it->nUserType == nUserType && it->nRelationType == nRelationType)
                {
                    it->nOrgID = nOutOrgID;
                    break;
                }
                else
                    ++it;
            }
        }


        std::vector<int>::iterator it_InAttch = vecInID.begin();
        for ( ; it_InAttch != vecInID.end(); it_InAttch++ )
        {
            std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
            while (it!=m_vUserAndOrgRelation.end())
            {
                if(it->nUserID == (*it_InAttch) && it->nUserType == nUserType && it->nRelationType == nRelationType)
                {
                    it->nOrgID = nOrgID;
                    break;
                }
                else
                    ++it;
            }
        }
    }

    return nErrorCode;
}

int CSvrUserOrgImpl::SaveUserOrgRelation(const int nUserID, int nRiskID, int nAttachOrg, std::vector<int>& vecMonitorOrg)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        if( CInterface_SvrDBOpr::getObj().SaveUserOrganizationRelation(nRiskID, nAttachOrg, vecMonitorOrg, nErrorCode))
        {
            std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
            while (it!=m_vUserAndOrgRelation.end())
            {
                if ( it->nUserType == USER_TYPE_RISK_CONTROL && it->nUserID == nRiskID )
                    it = m_vUserAndOrgRelation.erase(it);
                else
                    ++it;
            }

            UserAndOrgRelation info;
            memset(&info, 0, sizeof(info));

            if(nAttachOrg>0)
            {
                info.nRelationType = USER_ORG_RELATION_ATTACH;
                info.nUserType = USER_TYPE_RISK_CONTROL;
                info.nUserID = nRiskID;
                info.nOrgID = nAttachOrg;
                m_vUserAndOrgRelation.push_back(info);
            }


            memset(&info, 0, sizeof(info));
            info.nRelationType = USER_ORG_RELATION_MONITOR;
            info.nUserType = USER_TYPE_RISK_CONTROL;
            info.nUserID = nRiskID;
            std::vector<int>::iterator itOrg = vecMonitorOrg.begin();
            for (; itOrg!=vecMonitorOrg.end(); itOrg++)
            {
                info.nOrgID = (*itOrg);
                m_vUserAndOrgRelation.push_back(info);
            }
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nRiskID;
        strcpy(logData.logTable,"LOG_RELA_USER_ORG");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"组织机构与风控员的关系(RiskerID：%d   attachOrgID：%d)",nRiskID,nAttachOrg);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
        while (it!=m_vUserAndOrgRelation.end())
        {
            if ( it->nUserType == USER_TYPE_RISK_CONTROL && it->nUserID == nRiskID )
                it = m_vUserAndOrgRelation.erase(it);
            else
                ++it;
        }

        UserAndOrgRelation info;
        memset(&info, 0, sizeof(info));

        if(nAttachOrg>0)
        {
            info.nRelationType = USER_ORG_RELATION_ATTACH;
            info.nUserType = USER_TYPE_RISK_CONTROL;
            info.nUserID = nRiskID;
            info.nOrgID = nAttachOrg;
            m_vUserAndOrgRelation.push_back(info);
        }


        memset(&info, 0, sizeof(info));
        info.nRelationType = USER_ORG_RELATION_MONITOR;
        info.nUserType = USER_TYPE_RISK_CONTROL;
        info.nUserID = nRiskID;
        std::vector<int>::iterator itOrg = vecMonitorOrg.begin();
        for (; itOrg!=vecMonitorOrg.end(); itOrg++)
        {
            info.nOrgID = (*itOrg);
            m_vUserAndOrgRelation.push_back(info);
        }
    }

    return nErrorCode;
}

void CSvrUserOrgImpl::GetUserAndOrgRelation( std::vector<UserAndOrgRelation>& vec )
{
	CSafeRwLockRead Lock(m_Lock);

	vec = m_vUserAndOrgRelation;
}

bool CSvrUserOrgImpl::GetOrgIDByUserID(int nUserID, int& nOrgID)
{
    CSafeRwLockRead Lock(m_Lock);

	std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
	for ( ; it != m_vUserAndOrgRelation.end(); it++ )
	{
		if ( nUserID == it->nUserID && it->nRelationType == 0)
		{
			nOrgID = it->nOrgID;
			return true;
		}
	}

	return false;
}

bool CSvrUserOrgImpl::GetOrgIDByAccount(std::string szAccount, int& nOrgID)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,UserInfo>::iterator it = m_mapUserInfo.begin();
    for(;it!=m_mapUserInfo.end();it++)
    {
        if(0 == strcmp(it->second.szAccount,szAccount.c_str()))
        {
            int nUserID = it->first; 
            return GetOrgIDByUserID(nUserID, nOrgID);
        }
    }

    return false;
}

int  CSvrUserOrgImpl::GetAttachOrgIDByUserID(int nUserID)
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
    for ( ; it != m_vUserAndOrgRelation.end(); it++ )
    {
        if ( nUserID == it->nUserID && it->nRelationType == 0)
            return it->nOrgID;                
    }

    return -1;
}

bool CSvrUserOrgImpl::GetUserInfoByAccount(std::string szAccount, UserInfo& userInfo)
{
    CSafeRwLockRead Lock(m_Lock);

    std::map<int,UserInfo>::iterator it = m_mapUserInfo.begin();
    for(;it!=m_mapUserInfo.end();it++)
    {
        if(0 == strcmp(it->second.szAccount,szAccount.c_str()))
        {
            userInfo = it->second; 
            return true;
        }
    }

    return false;
}

void CSvrUserOrgImpl::GetMonitorUserIDByOrgID(int nOrgID, std::set<int>& vUserID)
{
    CSafeRwLockRead Lock(m_Lock);

	std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
	for ( ; it != m_vUserAndOrgRelation.end(); it++ )
	{
		if ( nOrgID == it->nOrgID && it->nRelationType == 1)
		{
			vUserID.insert(it->nUserID);
		}
	}
}

bool CSvrUserOrgImpl::GetMonitorOrgIDByUserID(int nUserID,std::vector<int> vecMonitorOrgID)
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<UserAndOrgRelation>::iterator it = m_vUserAndOrgRelation.begin();
    for ( ; it != m_vUserAndOrgRelation.end(); it++ )
    {
        if ( nUserID == it->nUserID && it->nRelationType == 1)
            vecMonitorOrgID.push_back(it->nOrgID);
    }

    return vecMonitorOrgID.size()>0;
}

bool CSvrUserOrgImpl::IsAddUserTradeAcctRelation(int nUserID, int nTraderAcctID, int& bHaveRele)
{
    std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
    for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
    {
        UserAndTradeAccountRelation userAndTradeAccount = *it;
        if ( nUserID == userAndTradeAccount.nUserID )
        {
            bHaveRele = 1;
            if(nTraderAcctID == userAndTradeAccount.nTradeAccountID)
            {
                return false;
            }
        }
    }

    return true;    
}

int CSvrUserOrgImpl::AddUserAndTradeAccountRelation( const int nLoginUserID, const UserAndTradeAccountRelation& info )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
	    char szBuffer[MAX_SQL_LENGTH];
	    memset(szBuffer, 0, sizeof(szBuffer)); 
	    sprintf(szBuffer, "insert into RELA_USER_TRADEACCOUNT values(%d, %d, %d, %d, %d)",
		    info.nUserID, info.nTradeAccountID, info.nCanInputOrder,info.nNeedVerify,UNDELETE_FLAG);

        if(CF_ERROR_SUCCESS == DelUserAndTradeAccountRelation(nLoginUserID, info.nUserID))
        {
            if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
                m_vUserAndTradeAccountRelation.push_back(info);
        }


        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = info.nUserID;
        strcpy(logData.logTable,"LOG_RELA_USER_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nLoginUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"交易员与委托交易账号的关系（增加）(TraderID：%d   AccountID：%d    CanInputOrder：%d    NeedVerify：%d )",info.nUserID,info.nTradeAccountID, info.nCanInputOrder,info.nNeedVerify);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        if(CF_ERROR_SUCCESS == DelUserAndTradeAccountRelation(nLoginUserID, info.nUserID))
            m_vUserAndTradeAccountRelation.push_back(info);
    }

	return nErrorCode;
}

int CSvrUserOrgImpl::DelUserAndTradeAccountRelation( const int nLoginUserID, int nUserID )
{
    CSafeRwLockWrite Lock(m_Lock);

	int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
	    char szBuffer[MAX_SQL_LENGTH];
	    memset(szBuffer, 0, sizeof(szBuffer));
	    sprintf(szBuffer, "delete RELA_USER_TRADEACCOUNT t where t.userid = %d",nUserID);
	    if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
	    {
		    std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
		    for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
		    {
			    UserAndTradeAccountRelation userAndTradeAccount = *it;
			    if ( nUserID == userAndTradeAccount.nUserID )
			    {
				    m_vUserAndTradeAccountRelation.erase(it);
				    break;
			    }
		    }
	    }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nUserID;
        strcpy(logData.logTable,"LOG_RELA_USER_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nLoginUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"交易员与委托交易账号的关系（删除）(TraderID：%d)",nUserID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
        for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
        {
            UserAndTradeAccountRelation userAndTradeAccount = *it;
            if ( nUserID == userAndTradeAccount.nUserID )
            {
                m_vUserAndTradeAccountRelation.erase(it);
                break;
            }
        }
    }

	return nErrorCode;
}

void CSvrUserOrgImpl::GetUserAndTradeAccountRelation( std::vector<UserAndTradeAccountRelation>& vec )
{
    CSafeRwLockRead Lock(m_Lock);

	vec = m_vUserAndTradeAccountRelation;
}

void CSvrUserOrgImpl::GetUserAndTradeAccountRelationByUserID(int nUserID, std::vector<UserAndTradeAccountRelation>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

	std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
	for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
	{
		if ( it->nUserID == nUserID )
			vec.push_back(*it);
	}
}

bool CSvrUserOrgImpl::GetUserAndTradeAccountRelation(int nTradeAccountID, std::vector<UserAndTradeAccountRelation>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

    bool bFind = false;
    std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
    for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
    {
        if ( it->nTradeAccountID == nTradeAccountID )
        {
            bFind = true;
            vec.push_back(*it);
        }
    }

    return bFind;
}

void CSvrUserOrgImpl::GetPrivileges( std::vector<Privilege>& vec )
{
	CSafeRwLockRead Lock(m_Lock);

	m_permissionMgr.GetPrivileges(vec);
}

void CSvrUserOrgImpl::GetPrivileges( int nRoleID, std::vector<Privilege>& vec )
{
	CSafeRwLockRead Lock(m_Lock);

	m_permissionMgr.GetPrivileges(nRoleID, vec);
}

void CSvrUserOrgImpl::GetPrivilegesByUserID(int nUserID, std::vector<Privilege>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<int> vRoleID;
    GetUserRoles(nUserID, vRoleID);
    for ( UINT i = 0; i < vRoleID.size(); i++ )
        m_permissionMgr.GetPrivileges(vRoleID[i], vec);
}
int CSvrUserOrgImpl::SaveRolePrivilegeRelation( const int nUserID, int nRoleID, std::vector<int>& vec )
{
	CSafeRwLockRead Lock(m_Lock);

	int nRet = m_permissionMgr.SaveRolePrivilegeRelation(nUserID, nRoleID, vec);
	return nRet;
}

int CSvrUserOrgImpl::GetRolePrivilegeRelation(std::vector<RoleAndPrivilegeRelation>& vec)
{
    CSafeRwLockRead Lock(m_Lock);

    int nRet = m_permissionMgr.GetRolePrivilegeRelation(vec);
    return nRet;
}

int CSvrUserOrgImpl::AddRole( const int nUserID, RoleInfo& name, int& nID )
{
	CSafeRwLockRead Lock(m_Lock);

	int nRet = m_permissionMgr.AddRole(nUserID, name, nID);
	return nRet;
}

int CSvrUserOrgImpl::ModifyRole( const int nUserID, const RoleInfo& name )
{
	CSafeRwLockRead Lock(m_Lock);

	int nRet = m_permissionMgr.ModifyRole(nUserID, name);
	return nRet;
}

int CSvrUserOrgImpl::DelRole( const int nUserID, int nID )
{
	CSafeRwLockRead Lock(m_Lock);

	int nRet = m_permissionMgr.DelRole(nUserID, nID);

    //删除与用户的关系
    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete RELA_USER_ROLE t where t.roleid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::vector<UserRole>::iterator it = m_vecUserRoleRelation.begin();
            while (it!=m_vecUserRoleRelation.end())
            {
                if(it->nRoleID == nID)
                    it = m_vecUserRoleRelation.erase(it);
                else
                    it++;
            }

            std::map<int, std::vector<UserRole>>::iterator it_userRole = m_mapUserRoleRelation.begin();
            for (;it_userRole!=m_mapUserRoleRelation.end();it_userRole++)
            {
                std::vector<UserRole>& vec = it_userRole->second;
                std::vector<UserRole>::iterator it_role = vec.begin();
                while(it_role!=vec.end())
                {
                    if(it_role->nRoleID == nID)
                        it_role = vec.erase(it_role);
                    else
                        it_role++;
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
        strcpy(logData.logTable,"LOG_ROLE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"角色(ID:%d):删除角色与用户的关系",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        m_permissionMgr.DelRole(nUserID, nID);

        std::vector<UserRole>::iterator it = m_vecUserRoleRelation.begin();
        while (it!=m_vecUserRoleRelation.end())
        {
            if(it->nRoleID == nID)
                it = m_vecUserRoleRelation.erase(it);
            else
                it++;
        }

        std::map<int, std::vector<UserRole>>::iterator it_userRole = m_mapUserRoleRelation.begin();
        for (;it_userRole!=m_mapUserRoleRelation.end();it_userRole++)
        {
            std::vector<UserRole>& vec = it_userRole->second;
            std::vector<UserRole>::iterator it_role = vec.begin();
            while(it_role!=vec.end())
            {
                if(it_role->nRoleID == nID)
                    it_role = vec.erase(it_role);
                else
                    it_role++;
            }
        }
    }

	return nRet;
}

void CSvrUserOrgImpl::GetRole( std::vector<RoleInfo>& vec )
{
	CSafeRwLockRead Lock(m_Lock);

	m_permissionMgr.GetRoles(vec);
}

void CSvrUserOrgImpl::GetLowerLevelOrgs( int nOrgID, std::map<int, Organization>& mapOrganization )
{
    CSafeRwLockRead Lock(m_Lock);

	std::map<int, std::map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(nOrgID);
	if (it != m_mapAssetMgmtOrgTree.end())
	{
		mapOrganization = it->second;
	}
}

int CSvrUserOrgImpl::SaveUserRole( const int nLoginUserID, int nUserID, std::vector<int> vRoleID )
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        if( CInterface_SvrDBOpr::getObj().SaveUserRole(nUserID, vRoleID, nErrorCode))
        {
            std::vector<UserRole>::iterator it = m_vecUserRoleRelation.begin();
            while(it!=m_vecUserRoleRelation.end())
            {
                if(it->nUserID == nUserID)
                    it=m_vecUserRoleRelation.erase(it);
                else
                    ++it;
            }

            std::map<int, std::vector<UserRole>>::iterator it_userRole = m_mapUserRoleRelation.find(nUserID);
            if(it_userRole!=m_mapUserRoleRelation.end())
                m_mapUserRoleRelation.erase(it_userRole);

            UserRole info;
            info.nUserID = nUserID;
            std::vector<int>::iterator it_Role = vRoleID.begin();
            while(it_Role!=vRoleID.end())
            {
                info.nRoleID = (*it_Role);
                m_vecUserRoleRelation.push_back(info);
                m_mapUserRoleRelation[info.nUserID].push_back(info);
                ++it_Role;
            }
        }


        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nUserID;
        strcpy(logData.logTable,"LOG_RELA_USER_ROLE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nLoginUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"用户与角色的关系(UserID：%d)",nUserID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::vector<UserRole>::iterator it = m_vecUserRoleRelation.begin();
        while(it!=m_vecUserRoleRelation.end())
        {
            if(it->nUserID == nUserID)
                it=m_vecUserRoleRelation.erase(it);
            else
                ++it;
        }

        std::map<int, std::vector<UserRole>>::iterator it_userRole = m_mapUserRoleRelation.find(nUserID);
        if(it_userRole!=m_mapUserRoleRelation.end())
            m_mapUserRoleRelation.erase(it_userRole);

        UserRole info;
        info.nUserID = nUserID;
        std::vector<int>::iterator it_Role = vRoleID.begin();
        while(it_Role!=vRoleID.end())
        {
            info.nRoleID = (*it_Role);
            m_vecUserRoleRelation.push_back(info);
            m_mapUserRoleRelation[info.nUserID].push_back(info);
            ++it_Role;
        }
    }

    return nErrorCode;
}

void CSvrUserOrgImpl::GetUserRoles(std::vector<UserRole>& vUserRole)
{
    CSafeRwLockRead Lock(m_Lock);

    vUserRole = m_vecUserRoleRelation;
}

void CSvrUserOrgImpl::GetUserRoles( int nUserID, std::vector<int>& vRoleID )
{
	CSafeRwLockRead Lock(m_Lock);

	std::vector<UserRole>::iterator it = m_vecUserRoleRelation.begin();
	for (; it != m_vecUserRoleRelation.end(); it++)
	{
		if(nUserID = it->nUserID)
            vRoleID.push_back(it->nRoleID);
	}
}
void CSvrUserOrgImpl::GetUserIDByRoleID( int nRoleID, std::vector<int>& vUserID )
{
    CSafeRwLockRead Lock(m_Lock);

    std::vector<UserRole>::iterator it = m_vecUserRoleRelation.begin();
    for (; it != m_vecUserRoleRelation.end(); it++)
    {
        if(nRoleID == it->nRoleID)
            vUserID.push_back(it->nUserID);
    }
}

//由角色和权限关系 用户(管理员)与角色关系获得用户(管理员)的权限关系
bool CSvrUserOrgImpl::bUserHavePrivilege(int nUserID, int nPrivilegeID)
{
    if(USER_ADMIN==nUserID)//admin
    {
        return true;
    }
	UserInfo lUserInfo;
	if(false == GetUserByID(nUserID,lUserInfo))
	{
		return false;
	}

	if(lUserInfo.nUserType == USER_TYPE_VIEWER && nPrivilegeID > QUERY && nPrivilegeID < CONTROL)
	{
		return true;
	}

    std::map<int, std::vector<UserRole>>::iterator it_userRole = m_mapUserRoleRelation.find(nUserID);
    if(it_userRole==m_mapUserRoleRelation.end())
    {
        return false;
    }

    std::vector<UserRole> vecUserRole = it_userRole->second;
    for (std::vector<UserRole>::iterator it_vec = vecUserRole.begin();it_vec!=vecUserRole.end();it_vec++)
    {
        int nRoleID = it_vec->nRoleID;

        std::map<int,std::vector<int>> mapRoleID2Privilege;
        if(m_permissionMgr.GetRolePrivilegeRelation(mapRoleID2Privilege))
        {
            std::map<int,std::vector<int>>::iterator it_role2pri = mapRoleID2Privilege.find(nRoleID);
            if(it_role2pri==mapRoleID2Privilege.end())
                continue;
            else
            {
                std::vector<int> vecPrivilege = it_role2pri->second;
                for (std::vector<int>::iterator it=vecPrivilege.begin();it!=vecPrivilege.end();it++)
                {
                    if(nPrivilegeID == (int)(*it))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void CSvrUserOrgImpl::GetUserIDsByTradeAccount(std::string strAccount, std::string strBrokerCode, std::vector<UserInfo>& vec)
{
	CSafeRwLockRead Lock(m_Lock);

	std::vector<TradeAccount> vAccount;
	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(vAccount);
	int nTradeAccountID = 0;
	for ( int i = 0; i < (int)vAccount.size(); i++ )
	{
		TradeAccount account = vAccount[i];
		BrokerInfo brokerInfo;
		if ( CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(account.nBrokerID, brokerInfo))
		{
			if ( strAccount == account.szTradeAccount && strBrokerCode == brokerInfo.szBrokerCode)
			{
				nTradeAccountID = account.nTradeAccountID;
                for ( int j = 0; j < (int)m_vUserAndTradeAccountRelation.size(); j++ )
                {
                    if ( m_vUserAndTradeAccountRelation[j].nTradeAccountID == nTradeAccountID )
                    {
                        std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(m_vUserAndTradeAccountRelation[j].nUserID);
                        if ( it != m_mapUserInfo.end() )
                            vec.push_back(it->second);
                    }
                }

				break;
			}
		}
	}
}

bool CSvrUserOrgImpl::GetTradeAccountByUserID(int nUserID, TradeAccount& tradeAccount)
{
    CSafeRwLockRead Lock(m_Lock);

    bool bFind = false;
    std::vector<UserAndTradeAccountRelation> vecRelation;

    std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
    for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
    {
        if ( it->nUserID == nUserID )
        {
            TradeAccount account;
            if(CInterface_SvrBrokerInfo::getObj().GetTradeAccount(it->nTradeAccountID,account))
            {
                bFind = true;
                tradeAccount = account;
                break;
            }
        }
    }
   
    return bFind;
}

CF_ERROR CSvrUserOrgImpl::SetLimitTradeFromRisk(int nUserID, bool bTrue)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH]; 
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "update RELA_USER_TRADEACCOUNT t set t.caninputorder = %d where t.userid = %d", (bTrue)?0:1, nUserID);
        CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
    }

    TradeAccount tradeAccount;
    bool bFind = false;
    std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
    for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
    {
        if ( it->nUserID == nUserID )
        {
            TradeAccount account;
            if(CInterface_SvrBrokerInfo::getObj().GetTradeAccount(it->nTradeAccountID,account))
            {
                bFind = true;
                tradeAccount = account;
                break;
            }
        }
    }


    if(bFind)
    {
        std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
        for (; it != m_vUserAndTradeAccountRelation.end(); it++)
        {
            if(it->nUserID == nUserID && it->nTradeAccountID == tradeAccount.nTradeAccountID)
            {
                it->nCanInputOrder = (bTrue)?0:1;
                break;
            }
        }  
    }  

    return nErrorCode;
}
CF_ERROR CSvrUserOrgImpl::SetManualVerifyFromRisk(int nUserID, bool bTrue)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH]; 
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "update RELA_USER_TRADEACCOUNT t set t.needverify = %d where t.userid = %d", int(bTrue), nUserID);
        CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
    }

    
    TradeAccount tradeAccount;

    bool bFind = false;
    std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
    for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
    {
        if ( it->nUserID == nUserID )
        {
            TradeAccount account;
            if(CInterface_SvrBrokerInfo::getObj().GetTradeAccount(it->nTradeAccountID,account))
            {
                bFind = true;
                tradeAccount = account;
                break;
            }
        }
    }


    if(bFind)
    {
        std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
        for (; it != m_vUserAndTradeAccountRelation.end(); it++)
        {
            if(it->nUserID == nUserID && it->nTradeAccountID == tradeAccount.nTradeAccountID)
            {
                it->nNeedVerify = (bTrue)?1:0;
                break;
            }
        }
    } 
    

    return nErrorCode;
}

int CSvrUserOrgImpl::UpdateLimitTradeAndManualVerify(const int nLoginUserID, const UserAndTradeAccountRelation& info)
{
    CSafeRwLockWrite Lock(m_Lock);

    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH]; 
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "update RELA_USER_TRADEACCOUNT t set t.needverify = %d , t.caninputorder = %d where t.userid = %d", info.nNeedVerify, info.nCanInputOrder, info.nUserID);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            TradeAccount tradeAccount;

            bool bFind = false;
            std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
            for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
            {
                if ( it->nUserID == info.nUserID )
                {
                    TradeAccount account;
                    if(CInterface_SvrBrokerInfo::getObj().GetTradeAccount(it->nTradeAccountID,account))
                    {
                        bFind = true;
                        tradeAccount = account;
                        break;
                    }
                }
            }

            if(bFind)
            {
                std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
                for (; it != m_vUserAndTradeAccountRelation.end(); it++)
                {
                    if(it->nUserID == info.nUserID && it->nTradeAccountID == tradeAccount.nTradeAccountID)
                    {
                        it->nNeedVerify = info.nNeedVerify;
                        it->nCanInputOrder = info.nCanInputOrder;
                        break;
                    }
                }
            } 
        }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = info.nUserID;
        strcpy(logData.logTable,"LOG_RELA_USER_TRADEACCOUNT");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nLoginUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"交易员与委托交易账号的关系（属性）(TraderID：%d     AccountID：%d     CanInputOrder：%d     NeedVerify：%d)",info.nUserID,info.nTradeAccountID,info.nCanInputOrder,info.nNeedVerify);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        TradeAccount tradeAccount;

        bool bFind = false;
        std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
        for ( ; it != m_vUserAndTradeAccountRelation.end(); it++ )
        {
            if ( it->nUserID == info.nUserID )
            {
                TradeAccount account;
                if(CInterface_SvrBrokerInfo::getObj().GetTradeAccount(it->nTradeAccountID,account))
                {
                    bFind = true;
                    tradeAccount = account;
                    break;
                }
            }
        }

        if(bFind)
        {
            std::vector<UserAndTradeAccountRelation>::iterator it = m_vUserAndTradeAccountRelation.begin();
            for (; it != m_vUserAndTradeAccountRelation.end(); it++)
            {
                if(it->nUserID == info.nUserID && it->nTradeAccountID == tradeAccount.nTradeAccountID)
                {
                    it->nNeedVerify = info.nNeedVerify;
                    it->nCanInputOrder = info.nCanInputOrder;
                    break;
                }
            }
        } 
    }

    return nErrorCode;
}