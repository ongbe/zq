///////////////////////////////////////////////////////////
//  PrivilegeManage.cpp
//  Implementation of the Class PrivilegeManage
//  Created on:      16-二月-2013 17:29:01
//  Original author: admin
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PrivilegeManage.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "SvrUserOrg.h"
#include "CommonMacro.h"

PrivilegeManage::PrivilegeManage()
: m_nNextRoleID(0)
{

}

PrivilegeManage::~PrivilegeManage()
{

}

bool PrivilegeManage::LoadDB()
{
    LoadRoleInfo();
    LoadPrivileges();
    LoadRolePrivilegeRelation();

	return true;
}

int PrivilegeManage::AddRole(const int nUserID, RoleInfo& role, int& nID)
{
    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        int nYear = sysTime.wYear-BASE_YEAR;
        int nMonth = sysTime.wMonth;
        int nDay = sysTime.wDay;

        nID = MAKE_DATEBASE_ID(nYear,nMonth,nDay,++m_nNextRoleID);

        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "insert into BASE_ROLE values(%d,'%s',%d,%d)",nID, role.szName, role.nUserTypeID,UNDELETE_FLAG);
        if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            role.nRoleID = nID;
            m_mapRoleInfo[nID] = role;
        }
        else
        {
            m_nNextRoleID--;
        }

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = role.nRoleID;
        strcpy(logData.logTable,"LOG_ROLE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Add;
        sprintf(logData.logContent,"角色(ID:%d   Name:%s   UserType:%d)",role.nRoleID, role.szName, role.nUserTypeID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapRoleInfo[role.nRoleID] = role;

    return nErrorCode;
}

int PrivilegeManage::ModifyRole(const int nUserID, const RoleInfo& role)
{
    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, 
            "update BASE_ROLE t set t.name = '%s' where t.roleid = %d",
            role.szName, role.nRoleID);
        int nNum = 0;
        if ( CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode))
        {
            if ( nNum != 0 )
            {
                m_mapRoleInfo[role.nRoleID] = role;
            }
        }


        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = role.nRoleID;
        strcpy(logData.logTable,"LOG_ROLE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Modify;
        sprintf(logData.logContent,"角色(ID:%d   Name:%s   UserType:%d)",role.nRoleID, role.szName, role.nUserTypeID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
        m_mapRoleInfo[role.nRoleID] = role;

    return nErrorCode;
}

int PrivilegeManage::DelRole(const int nUserID, int nID)
{
    int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
        char szBuffer[MAX_SQL_LENGTH];
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete BASE_ROLE t where t.roleid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.find(nID);
            if ( it != m_mapRoleInfo.end())
                m_mapRoleInfo.erase(it);
        }

        //删除与权限的关系
        memset(szBuffer, 0, sizeof(szBuffer));
        sprintf(szBuffer, "delete RELA_ROLE_PRIVELEGE t where t.roleid = %d", nID);
        if ( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
        {
            std::vector<RoleAndPrivilegeRelation>::iterator it = m_vecRoleAndPrivilegeRelation.begin();
            while (it!=m_vecRoleAndPrivilegeRelation.end())
            {
                if(it->nRoleID == nID)
                    it = m_vecRoleAndPrivilegeRelation.erase(it);
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
        strcpy(logData.logTable,"LOG_ROLE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"角色(ID:%d)",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);


        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nID;
        strcpy(logData.logTable,"LOG_RELA_ROLE_PRIVELEGE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Delete;
        sprintf(logData.logContent,"角色(ID:%d):删除角色与权限的关系",nID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;
        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::map<int,RoleInfo>::iterator it_role = m_mapRoleInfo.find(nID);
        if ( it_role != m_mapRoleInfo.end())
            m_mapRoleInfo.erase(it_role);

        //删除与权限的关系
        std::vector<RoleAndPrivilegeRelation>::iterator it = m_vecRoleAndPrivilegeRelation.begin();
        while (it!=m_vecRoleAndPrivilegeRelation.end())
        {
            if(it->nRoleID == nID)
                it = m_vecRoleAndPrivilegeRelation.erase(it);
            else
                it++;
        }
    }

    return nErrorCode;
}

void PrivilegeManage::GetRoles(std::vector<RoleInfo>& vec)
{
    vec.clear();
    std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.begin();
    for ( ; it != m_mapRoleInfo.end(); it++)
    {
        vec.push_back(it->second);
    }
}

void PrivilegeManage::SetServerRunMode(eServerRunMode nMode)
{
    m_ServerRunMode = nMode;
}

const std::map<int,RoleInfo>& PrivilegeManage::GetRoleInfo()
{
    return m_mapRoleInfo;
}

const std::vector<Privilege>& PrivilegeManage::GetPrivilege()
{
    return m_vecPrivilege;
}

const std::vector<RoleAndPrivilegeRelation>& PrivilegeManage::GetRoleAndPrivilegeRelation()
{
    return m_vecRoleAndPrivilegeRelation;
}

void PrivilegeManage::SetRoleInfo(std::map<int,RoleInfo>& mapRoleInfo)
{
    m_mapRoleInfo = mapRoleInfo;
}

void PrivilegeManage::SetPrivilege(std::vector<Privilege>& vecPrivilege)
{
    m_vecPrivilege = vecPrivilege;
}

void PrivilegeManage::SetRoleAndPrivilegeRelation(std::vector<RoleAndPrivilegeRelation>& vecRoleAndPrivilegeRelation)
{
    m_vecRoleAndPrivilegeRelation = vecRoleAndPrivilegeRelation;
}

void PrivilegeManage::SetRoleID2PrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2PrivilegeRelation)
{
    m_mapRoleID2PrivilegeRelation = mapRoleID2PrivilegeRelation;
}

bool PrivilegeManage::LoadRoleInfo()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    m_mapRoleInfo.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_ROLE t order by t.roleid ASC");
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
        //如果是 m_nNextRoleID则从个数算起
        //如果否 m_nNextRoleID则从0算起

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::vector<_variant_t> vValue = vec[nDataCount];
        int nFinancialProductID = vValue[0].intVal;

        if(((nFinancialProductID>>25)&0x0000003F)+BASE_YEAR == st.wYear &&
            ((nFinancialProductID<<7>>27)&0x0000001F) == st.wMonth &&
            ((nFinancialProductID<<12>>27)&0x0000001F) == st.wDay )
            m_nNextRoleID = (nFinancialProductID&0x00007FFF);

        for ( UINT i = 0; i < vec.size(); i++ )
        {
            RoleInfo roleInfo;
            memset(&roleInfo, 0, sizeof(roleInfo));

            std::vector<_variant_t> vValue = vec[i];

            int index = 0;
            roleInfo.nRoleID = vValue[index++].intVal;
            strcpy(roleInfo.szName, vValue[index++].operator _bstr_t());
            roleInfo.nUserTypeID = vValue[index++].intVal;

            m_mapRoleInfo[roleInfo.nRoleID] = roleInfo;
        }
    }

    return true;
}

bool PrivilegeManage::LoadPrivileges()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    m_vecPrivilege.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_PRIVELEGES t");
    std::vector<std::vector<_variant_t>> vec;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vec, nErrorCode))
    {
        return false;
    }
    else
    {
        for ( UINT i = 0; i < vec.size(); i++ )
        {
            Privilege node;
            memset(&node, 0, sizeof(node));

            std::vector<_variant_t> vValue = vec[i];

            int index = 0;
            node.nPrivilegeID = vValue[index++].intVal;
            strcpy(node.szName, vValue[index++].operator _bstr_t());
            node.nUserTypeID = vValue[index++].intVal;
            node.nParentID = vValue[index++].intVal;
            node.nNodeLevel = vValue[index++].intVal;

            m_vecPrivilege.push_back(node);
        }
    }

    return true;
}

bool PrivilegeManage::LoadRolePrivilegeRelation()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    m_vecRoleAndPrivilegeRelation.clear();
    m_mapRoleID2PrivilegeRelation.clear();
    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from RELA_ROLE_PRIVELEGE t");
    std::vector<std::vector<_variant_t>> vec;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vec, nErrorCode))
    {
        return false;
    }
    else
    {
        for ( UINT i = 0; i < vec.size(); i++ )
        {
            RoleAndPrivilegeRelation relation;
            memset(&relation, 0, sizeof(relation));

            std::vector<_variant_t> vValue = vec[i];

            int nIndex = 0;
            relation.nRoleID = vValue[nIndex++].intVal;
            relation.nPrivilegeID = vValue[nIndex++].intVal;

            m_vecRoleAndPrivilegeRelation.push_back(relation);
            m_mapRoleID2PrivilegeRelation[relation.nRoleID].push_back(relation.nPrivilegeID);
        }
    }

    return true;
}

void PrivilegeManage::GetPrivileges(std::vector<Privilege>& vec)
{
	vec = m_vecPrivilege;
}

void PrivilegeManage::GetPrivileges(int nRoleID, std::vector<Privilege>& vec)
{
	vec.clear();
	std::vector<RoleAndPrivilegeRelation>::iterator it_per = m_vecRoleAndPrivilegeRelation.begin();
	for ( ; it_per != m_vecRoleAndPrivilegeRelation.end(); it_per++)
	{
        if (it_per->nRoleID == nRoleID)
        {
            std::vector<Privilege>::iterator it = m_vecPrivilege.begin();
            for (; it!=m_vecPrivilege.end(); it++)
            {
                if (it_per->nPrivilegeID == it->nPrivilegeID)
                {
                    vec.push_back(*it);
                    break;
                }
            }
        }

	}
}

int PrivilegeManage::GetRolePrivilegeRelation(std::vector<RoleAndPrivilegeRelation>& vec)
{
    vec.clear();
    vec = m_vecRoleAndPrivilegeRelation;

    return vec.size();
}

int PrivilegeManage::GetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2PrivilegeRelation)
{
    mapRoleID2PrivilegeRelation.clear();
    mapRoleID2PrivilegeRelation = m_mapRoleID2PrivilegeRelation;

    return mapRoleID2PrivilegeRelation.size();
}
int PrivilegeManage::SaveRolePrivilegeRelation(const int nUserID, int nRoleID, std::vector<int>& vecPrivilegeID)
{
	int nErrorCode = CF_ERROR_SUCCESS;
    if(ServerRunMode_Master == m_ServerRunMode)
    {
	    if ( !CInterface_SvrDBOpr::getObj().SaveRolePrivilegeRelation(nRoleID, vecPrivilegeID, nErrorCode))
	    {
		    return false;
	    }
	    else
	    {
            std::vector<RoleAndPrivilegeRelation>::iterator it = m_vecRoleAndPrivilegeRelation.begin();
            while (it!=m_vecRoleAndPrivilegeRelation.end())
            {
                if ( nRoleID == it->nRoleID )
                {
                    it = m_vecRoleAndPrivilegeRelation.erase(it);
                }
                else
                    it++;
            }

            std::map<int,std::vector<int>>::iterator it_map = m_mapRoleID2PrivilegeRelation.find(nRoleID);
            if( it_map != m_mapRoleID2PrivilegeRelation.end() )
                m_mapRoleID2PrivilegeRelation.erase(it_map);


            RoleAndPrivilegeRelation oRelation;
            oRelation.nRoleID = nRoleID;
            std::vector<int>::iterator itPrivilegeID = vecPrivilegeID.begin();
            for (; itPrivilegeID!=vecPrivilegeID.end(); itPrivilegeID++)
            {
                oRelation.nPrivilegeID = *itPrivilegeID;
                m_vecRoleAndPrivilegeRelation.push_back(oRelation);
                m_mapRoleID2PrivilegeRelation[oRelation.nRoleID].push_back(oRelation.nPrivilegeID);
            }
	    }

        SYSTEMTIME sysTime;
        ::GetLocalTime(&sysTime);

        //log入库信息
        int nLogErrorCode; 
        LogBaseData logData;
        memset(&logData,0,sizeof(LogBaseData));

        logData.logTargetorID = nRoleID;
        strcpy(logData.logTable,"LOG_RELA_ROLE_PRIVELEGE");
        sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
            sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        logData.logOperatorID = nUserID;
        logData.logAction     = Oper_Save;
        sprintf(logData.logContent,"角色与权限的关系(RoleID：%d)",nRoleID);
        logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

        CInterface_SvrDBOpr::getObj().WriteLogToDB(logData, nLogErrorCode);
    }
    else if(ServerRunMode_Slave == m_ServerRunMode)
    {
        std::vector<RoleAndPrivilegeRelation>::iterator it = m_vecRoleAndPrivilegeRelation.begin();
        while (it!=m_vecRoleAndPrivilegeRelation.end())
        {
            if ( nRoleID == it->nRoleID )
            {
                it = m_vecRoleAndPrivilegeRelation.erase(it);
            }
            else
                it++;
        }

        std::map<int,std::vector<int>>::iterator it_map = m_mapRoleID2PrivilegeRelation.find(nRoleID);
        if( it_map != m_mapRoleID2PrivilegeRelation.end() )
            m_mapRoleID2PrivilegeRelation.erase(it_map);


        RoleAndPrivilegeRelation oRelation;
        oRelation.nRoleID = nRoleID;
        std::vector<int>::iterator itPrivilegeID = vecPrivilegeID.begin();
        for (; itPrivilegeID!=vecPrivilegeID.end(); itPrivilegeID++)
        {
            oRelation.nPrivilegeID = *itPrivilegeID;
            m_vecRoleAndPrivilegeRelation.push_back(oRelation);
            m_mapRoleID2PrivilegeRelation[oRelation.nRoleID].push_back(oRelation.nPrivilegeID);
        }
    }

	return nErrorCode;
}
