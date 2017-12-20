#include "stdafx.h"
#include "CDataInfo.h"
#include "CommonDef.h"

CDataInfo* CDataInfo::m_pInstance = NULL;
int g_Count = 0;
const static char* __UNKOWN__="未知";

CDataInfo* CDataInfo::NewInstance()
{
	g_Count ++;
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CDataInfo();
	}

	return m_pInstance;
}

void CDataInfo::DestroyInstance()
{
	g_Count --;
	if(g_Count == 0 && m_pInstance != NULL) 
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void CDataInfo::Clear()
{
    m_bRoleLoad = false;
    m_bPrivilegeLoad = false;
    m_bRolePrivilegeRelationLoad = false;
    m_bUserRoleRelationLoad = false;

	m_mapAssetMgmtOrgTree.clear();
    m_mapOrganization.clear();
	m_mapSettlePrice.clear();
	m_mapCommissionMarginRate.clear();
	m_mapModifiedRate.clear();
	m_mapInstrumentInfo.clear();
	m_vecAllRate.clear();
	m_vecVacancyRate.clear();

    m_PopMsgWin = NULL;
}

CDataInfo::CDataInfo()
{
    Clear();
}

CDataInfo::~CDataInfo()
{
}

//资管组织
void CDataInfo::ClearOrganization()
{
    m_mapOrganization.clear();
}

void CDataInfo::SetOrganization(std::map<int, Organization>& mapAssetMgmtOrg)
{
    std::map<int, Organization>::iterator it_org = mapAssetMgmtOrg.begin();
    for ( ; it_org != mapAssetMgmtOrg.end(); it_org++ )
    {
        Organization info = it_org->second;
        if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
        {
            std::map<int, Organization> tempOrgMap;
            m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
        }

        std::map<int, map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
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

    m_mapOrganization = mapAssetMgmtOrg;
}

void CDataInfo::SetOrganization(const Organization* pOrganization)
{
    //将原来有的删掉
    std::map<int, map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.begin();
    for (;it!=m_mapAssetMgmtOrgTree.end();it++)
    {
        map<int, Organization>& mapInclude = it->second;
        map<int, Organization>::iterator it_org = mapInclude.find(pOrganization->nAssetMgmtOrgID);
        if(it_org!=mapInclude.end())
        {
            mapInclude.erase(it_org);
            break;
        }
    }

    if(m_mapAssetMgmtOrgTree.find(pOrganization->nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
    {
        std::map<int, Organization> tempOrgMap;
        m_mapAssetMgmtOrgTree[pOrganization->nAssetMgmtOrgID] = tempOrgMap;
    }

    it = m_mapAssetMgmtOrgTree.find(pOrganization->nUpperLevelOrgID);
    if ( it != m_mapAssetMgmtOrgTree.end() )
    {
        it->second[pOrganization->nAssetMgmtOrgID] = (*pOrganization);
    }
    else
    {
        std::map<int, Organization> tempOrgMap;
        tempOrgMap[pOrganization->nAssetMgmtOrgID] = *pOrganization;
        m_mapAssetMgmtOrgTree[pOrganization->nUpperLevelOrgID] = tempOrgMap;
    }

    m_mapOrganization[pOrganization->nAssetMgmtOrgID] = *pOrganization;
}

void CDataInfo::DelOrganization(int nOrgID)
{
    std::map<int,Organization>::iterator it = m_mapOrganization.find(nOrgID);
    if( it != m_mapOrganization.end() )
        m_mapOrganization.erase(it);

    std::map<int, std::map<int, Organization>>::iterator it1 = m_mapAssetMgmtOrgTree.begin();
    for ( ;it1!=m_mapAssetMgmtOrgTree.end();it1++)
    {
        std::map<int, Organization>& org = it1->second;
        std::map<int, Organization>::iterator it2 = org.begin();
        while (it2!=org.end())
        {
            if (it2->second.nAssetMgmtOrgID == nOrgID)
            {
                it2 = org.erase(it2);
            }
            else
                ++it2;
        }
    }

    std::map<int, std::map<int, Organization>>::iterator it3 = m_mapAssetMgmtOrgTree.find(nOrgID);
    if ( it3!=m_mapAssetMgmtOrgTree.end())
    {
        m_mapAssetMgmtOrgTree.erase(it3);
    }

    //关系操作
    //将所有是从属关系的交易员、风控员的部门设置为总部
    std::vector<UserAndOrgRelation>::iterator it_rela = m_vecUserAndOrgRelation.begin();
    for (;it_rela!=m_vecUserAndOrgRelation.end();it_rela++)
    {
        if(it_rela->nOrgID == nOrgID && it_rela->nRelationType == USER_ORG_RELATION_ATTACH)
            it_rela->nOrgID = ORGNIZATION_ROOT;
    }        
    

    //将所有是监控关系的风控员和部门删除
    it_rela = m_vecUserAndOrgRelation.begin();
    while (it_rela!=m_vecUserAndOrgRelation.end())
    {
        if(it_rela->nOrgID == nOrgID && it_rela->nRelationType == USER_ORG_RELATION_MONITOR)
            it_rela = m_vecUserAndOrgRelation.erase(it_rela);
        else
            it_rela++;
    }  
}

void CDataInfo::GetOrganization(std::map<int, std::map<int, Organization>>& mapAssetMgmtOrgTree)
{
	mapAssetMgmtOrgTree = m_mapAssetMgmtOrgTree;
}
bool CDataInfo::GetOrganizationByID(int nOrganizationID, Organization& sOrg)
{
	std::map<int, Organization>::iterator it = m_mapOrganization.find(nOrganizationID);
	if(it == m_mapOrganization.end())
		return false;

	sOrg = it->second;
	return true;
}
int CDataInfo::GetOrganizationIDByName(std::string& sName)
{
    std::map<int, Organization>::iterator it = m_mapOrganization.begin();
    for(;it != m_mapOrganization.end();it++)
    {
        if (strcmp(it->second.szName,sName.c_str()) == 0)
        {
            return it->first;
        }
    }

    return ORGNIZATION_ROOT;
}

string  CDataInfo::GetOrganizationNameByID(int nOrgID)
{
    string strName;
    std::map<int, Organization>::iterator it = m_mapOrganization.find(nOrgID);    
    if(it != m_mapOrganization.end())
        strName = (string)(it->second.szName);
    else 
        strName = " ";

    return strName;
}

int  CDataInfo::GetOrganizationScaleLevelByID(int nOrgID)
{
    Organization sOrg;
    GetOrganizationByID(nOrgID,sOrg);

    return sOrg.nScaleLevel;
}

bool CDataInfo::IsValidOfOrganizationName(const string& sName)
{
    std::map<int, Organization>::iterator it = m_mapOrganization.begin();
    for (;it!=m_mapOrganization.end();it++)
    {
        if (0 == strcmp(sName.c_str(),it->second.szName))
            return false;        
    }

    return true;
}   
//用户
void CDataInfo::ClearUserInfo()
{
    m_mapUserInfo.clear();
}

void CDataInfo::SetUserInfo(std::map<int, UserInfo>& mapUserInfo)
{
    m_mapUserInfo = mapUserInfo;
}

// void CDataInfo::SetUserInfo(int nUserID, bool bAssign)
// {
//     std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
//     if( it != m_mapUserInfo.end())
//         it->second.nAssign = bAssign;
// }
void CDataInfo::GetUserInfo(std::map<int, UserInfo>& mapUserInfo)
{
    mapUserInfo = m_mapUserInfo;
}

void CDataInfo::AddUserInfo(const UserInfo* pUserInfo)
{
    if(NULL == pUserInfo)
        return;

    m_mapUserInfo[pUserInfo->nUserID] = *pUserInfo;
}

void CDataInfo::DelUserInfo(int nUserID)
{
    std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if( it != m_mapUserInfo.end())
        m_mapUserInfo.erase(it);

    //删除用户与角色关系
    std::map<int, std::vector<UserRole>>::iterator it1 = m_mapUserRole.find(nUserID);
    if(it1!=m_mapUserRole.end())
    {
        m_mapUserRole.erase(it1);
    }

    //删除用户与组织机构关系
    std::vector<UserAndOrgRelation>::iterator it2 = m_vecUserAndOrgRelation.begin(); 
    for (;it2!=m_vecUserAndOrgRelation.end();it2++)
    {
        if(it2->nUserID == nUserID)
        {
            m_vecUserAndOrgRelation.erase(it2);
            break;
        }
    }

    //删除用户与委托交易账户关系
    std::map<int, UserAndTradeAccountRelation>::iterator it3 = m_mapUserAndTradeAccountRelation.find(nUserID);
    if(it3!=m_mapUserAndTradeAccountRelation.end())
    {
        m_mapUserAndTradeAccountRelation.erase(it3);
    }

    //删除用户与理财产品关系
    std::map<int, std::vector<TraderProRelation>>::iterator it4 = m_mapTraderProRelation.find(nUserID);
    if(it4!=m_mapTraderProRelation.end())
    {
        m_mapTraderProRelation.erase(it4);
    }
}

bool CDataInfo::GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if(it==m_mapUserInfo.end())
       return false;
    
    sUserInfo = it->second;
    return true;
}

int CDataInfo::GetUserIDByAccount(string szUserAccount)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.begin();
    for (; it != m_mapUserInfo.end(); it++)
    {
        if ( 0 == strcmp(it->second.szAccount,szUserAccount.c_str()))
        {            
            return it->first;  
        }
    }

    return -1;
}

bool CDataInfo::GetUserInfoByAccount(string szUserAccount,UserInfo& sUserInfo)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.begin();
    for (; it != m_mapUserInfo.end(); it++)
    {
        if ( 0 == strcmp(it->second.szAccount,szUserAccount.c_str()))
        {           
            memcpy(&sUserInfo,&(it->second),sizeof(UserInfo)); 
            return true;
        }
    }

    return false;
}

int CDataInfo::IsValidOfUserAccount(std::string strAccount, UserType nType)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.begin();
    for (; it != m_mapUserInfo.end(); it++)
    {
        if(nType == it->second.nUserType)
            if ( 0 == strcmp(it->second.szAccount,strAccount.c_str()) )
                return 0;
    }

    for (it = m_mapUserInfo.begin(); it != m_mapUserInfo.end(); it++)
    {
        if(nType != it->second.nUserType)
            if ( 0 == strcmp(it->second.szAccount,strAccount.c_str()) )
                return -1;
    }

    return 1;
}

bool CDataInfo::IsTraderTypeByUserID(int nUserID)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if (it != m_mapUserInfo.end())
    {
        if(USER_TYPE_TRADER == it->second.nUserType)
            return true;
    }

    return false;
}

void CDataInfo::SetUserStatus(int nUserID)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if (it != m_mapUserInfo.end())
    {
        it->second.nStatus = (0==it->second.nStatus)?1:0;
    }

}
//角色
void CDataInfo::ClearRole()
{
    m_mapRoleInfo.clear();
}
void CDataInfo::SetRole(std::map<int, RoleInfo>& mapRole)
{
    m_mapRoleInfo = mapRole;
}

void CDataInfo::SetRole(const RoleInfo& oRole)
{
    m_mapRoleInfo[oRole.nRoleID] = oRole;
}

void CDataInfo::GetRole(std::map<int, RoleInfo>& mapRole)
{
    mapRole = m_mapRoleInfo;
}

bool CDataInfo::GetRoleInfoByID(int nRoleID,RoleInfo& oRoleInfo)
{
    std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.find(nRoleID);
    if( it != m_mapRoleInfo.end())
    {   
        oRoleInfo = it->second;
        return true;
    }

    return false;
}

string CDataInfo::GetRoleNameByID(int nRoleID)
{
    string strName("");
    std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.find(nRoleID);
    if ( it != m_mapRoleInfo.end())
        strName = it->second.szName;
    
    return strName;
}

int CDataInfo::GetRoleIDByName(string& szName)
{
    std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.begin();
    for (; it != m_mapRoleInfo.end(); it++)
    {
        if ( 0 == strcmp(it->second.szName,szName.c_str()) )
        {
            return it->first;
            break;
        }
    }

    return -1;
}

void CDataInfo::DelRole(int nRoleID)
{
    std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.find(nRoleID);
    if( it!=m_mapRoleInfo.end() )
        m_mapRoleInfo.erase(it);

    //删除与权限的关系
    std::map<int,std::vector<int>>::iterator it_privelige = m_mapRoleID2Privilege.find(nRoleID);
    if (it_privelige != m_mapRoleID2Privilege.end())
    {
        m_mapRoleID2Privilege.erase(it_privelige);
    }

    //删除与用户的关系
    std::map<int, std::vector<UserRole>>::iterator it_rela = m_mapUserRole.begin();
    for (;it_rela!=m_mapUserRole.end();it_rela++)
    {
        std::vector<UserRole>& vecUserRole = it_rela->second;
        std::vector<UserRole>::iterator it1 = vecUserRole.begin();
        while (it1 != vecUserRole.end())
        {
            if(it1->nRoleID == nRoleID)
                it1 = vecUserRole.erase(it1);
            else
                it1++;
        }
    }
}

bool CDataInfo::IsValidOfRoleName(std::string strName)
{
    std::map<int, RoleInfo>::iterator it = m_mapRoleInfo.begin();
    for (;it!=m_mapRoleInfo.end();it++)
    {
        if (0 == strcmp(strName.c_str(),it->second.szName))
            return false;        
    }

    return true;
}

//用户与角色关系
void CDataInfo::ClearUserRoleRelation()
{
    m_mapUserRole.clear();
}

void CDataInfo::ClearUserRoleRelationByUserID(int nUserID)
{
    std::map<int, std::vector<UserRole>>::iterator it = m_mapUserRole.find(nUserID);
    if(it!=m_mapUserRole.end())
        m_mapUserRole.erase(it);
}

void CDataInfo::SetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole)
{
    m_mapUserRole = mapUserRole;
}

void CDataInfo::GetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole)
{
    mapUserRole = m_mapUserRole;
}

void CDataInfo::GetUserType2RoleRelation(std::map<int,std::vector<RoleInfo>>& mapUserType2Role)
{
    mapUserType2Role.clear();

    std::map<int, RoleInfo>::iterator itRoleInfo = m_mapRoleInfo.begin();
    for (; itRoleInfo != m_mapRoleInfo.end(); itRoleInfo++)
    {
        mapUserType2Role[itRoleInfo->second.nUserTypeID].push_back(itRoleInfo->second);
    }
}

void CDataInfo::GetRoleByUserID(int nUserID,std::vector<UserRole>& vecRole)
{
    std::map<int ,std::vector<UserRole>>::iterator it = m_mapUserRole.find(nUserID);
    if (it != m_mapUserRole.end())    
        vecRole = it->second; 
}

string CDataInfo::GetRoleByUserID(int nUserID)
{
    string strRoleName = " ",strRole[30];
    std::vector<UserRole> vecUserRole;
    GetRoleByUserID(nUserID,vecUserRole);
    std::vector<UserRole>::iterator itRole = vecUserRole.begin();
    for(int k=0; itRole != vecUserRole.end(); itRole++,k++)
    {
        if(k!=0)
            strRoleName += ",";

        strRole[k] = GetRoleNameByID(itRole->nRoleID);

        strRoleName += strRole[k];
    }

    return strRoleName;
}

void CDataInfo::SetUserRoleRelation(const UserRole* pUserRole)
{
    if( NULL == pUserRole )
        return;

    m_mapUserRole[pUserRole->nUserID].push_back(*pUserRole);
}

void CDataInfo::ClearUserAndOrgRelation()
{
    m_vecUserAndOrgRelation.clear();
}

void CDataInfo::DelUserAndOrgRelation(int nID, int nUserType)
{
    std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();

    //将所有风控员的监控关系删除
    while (it!=m_vecUserAndOrgRelation.end())
    {
        if(nUserType == USER_TYPE_RISK_CONTROL && it->nUserID == nID && it->nUserType == USER_TYPE_RISK_CONTROL && it->nRelationType == USER_ORG_RELATION_MONITOR)
            it = m_vecUserAndOrgRelation.erase(it);
        else
            it++;
    }

    //将风控员的从属关系设为总部
    it = m_vecUserAndOrgRelation.begin();
    while (it!=m_vecUserAndOrgRelation.end())
    {
        if(nUserType == USER_TYPE_RISK_CONTROL && it->nUserID == nID && it->nUserType == USER_TYPE_RISK_CONTROL && it->nRelationType == USER_ORG_RELATION_ATTACH)
        {
            it->nOrgID = ORGNIZATION_ROOT;
            break;
        }
        else
            it++;
    }

    //将交易的从属关系设为总部
    it = m_vecUserAndOrgRelation.begin();
    while (it!=m_vecUserAndOrgRelation.end())
    {
        if(nUserType == USER_TYPE_TRADER && it->nUserID == nID && it->nUserType == USER_TYPE_RISK_CONTROL && it->nRelationType == USER_ORG_RELATION_ATTACH)
        {
            it->nOrgID = ORGNIZATION_ROOT;
            break;
        }
        else
            it++;
    }
}

// void CDataInfo::ModifyUserAndOrgRelation(const UserAndOrgRelation& oRelation)
// {
//     bool bFind = false;
//     std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
//     while (it!=m_vecUserAndOrgRelation.end())
//     {
//         if ( it->nUserID == oRelation.nUserID && 
//              it->nRelationType == oRelation.nRelationType && 
//              it->nUserType == oRelation.nUserType )
//         {
//             bFind = true;
//             it->nOrgID = oRelation.nOrgID;
//  			DelDealerOrganRelation(it->nUserID, it->nOrgID);
//             break;
//         }
//         else
//             ++it;
//     }
// 
// 	if(!bFind) {
//         m_vecUserAndOrgRelation.push_back(oRelation);
// 		ResetDealerOrganRelation(&oRelation);
// 	}
// 
//     //设置分配属性
// //     SetUserInfo(oRelation.nUserID, true);
// }
// 
void CDataInfo::SetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation)
{
    m_vecUserAndOrgRelation = vecUserAndOrgRelation;
	ResetDealerOrganRelationMap();
}
void CDataInfo::SetUserAndOrgListenRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgListenRelation)
{
    std::vector<UserAndOrgRelation>::iterator it = vecUserAndOrgListenRelation.begin();
    for (;it!=vecUserAndOrgListenRelation.end();it++)
    {
        m_vecUserAndOrgRelation.push_back(*it);
    }
}
void CDataInfo::InitUserAndOrgRelation(const UserAndOrgRelation* pRelation)
{
    std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    for (;it!=m_vecUserAndOrgRelation.end();it++)
    {
        if(it->nUserType == pRelation->nUserType && it->nRelationType == pRelation->nRelationType)
            it->nOrgID = pRelation->nOrgID;            ;
    }
}

void CDataInfo::AddUserAndOrgRelation(const UserAndOrgRelation* pRelation)
{
    m_vecUserAndOrgRelation.push_back(*pRelation); 
}

void CDataInfo::SetUserAndOrgRelation(const UserAndOrgRelation* pRelation)
{
//     SetUserInfo(pRelation->nUserID, true);
    std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    for (;it!=m_vecUserAndOrgRelation.end();it++)
    {
        if(it->nUserID == pRelation->nUserID && it->nUserType == pRelation->nUserType && it->nRelationType == pRelation->nRelationType)
        {
            it->nOrgID = pRelation->nOrgID;
            return;
        }
    }

//     if(pRelation->nUserType == USER_TYPE_RISK_CONTROL && pRelation->nRelationType == USER_ORG_RELATION_MONITOR)
        m_vecUserAndOrgRelation.push_back(*pRelation);

	ResetDealerOrganRelation(pRelation);
}

void CDataInfo::GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation)
{
	vecUserAndOrgRelation = m_vecUserAndOrgRelation;
}

void CDataInfo::ResetDealerOrganRelationMap()
{
	std::map<int, set<int>>::iterator it;
	m_mapDealerOrganRelation.clear();
	for(int i=0; i<(int)m_vecUserAndOrgRelation.size(); i++) {
		ResetDealerOrganRelation(&m_vecUserAndOrgRelation[i]);
		//UserAndOrgRelation& relation = m_vecUserAndOrgRelation[i];
		//if(relation.nRelationType != 0)
		//	continue;
		//it = m_mapDealerOrganRelation.find(relation.nOrgID);
		//if(it == m_mapDealerOrganRelation.end()) {
		//	std::set<int> setDealer;
		//	setDealer.insert(relation.nUserID);
		//	m_mapDealerOrganRelation.insert(makepair(relation.nOrgID, setDealer));
		//}
		//else {
		//	it->second.insert(relation.nUserID);
		//}
	}
}

void CDataInfo::ResetDealerOrganRelation(const UserAndOrgRelation* pRelation)
{
	if(pRelation == NULL)
		return;
	std::map<int, set<int>>::iterator it;
	const UserAndOrgRelation& relation = (*pRelation);
	if(!(relation.nRelationType==0 && relation.nUserType==USER_TYPE_TRADER))
		return;

	it = m_mapDealerOrganRelation.find(relation.nOrgID);
	if(it == m_mapDealerOrganRelation.end()) {
		std::set<int> setDealer;
		setDealer.insert(relation.nUserID);
		m_mapDealerOrganRelation.insert(make_pair(relation.nOrgID, setDealer));
	}
	else {
		it->second.insert(relation.nUserID);
	}
}

void CDataInfo::DelDealerOrganRelation(int nDealerID, int nOrganID)
{
	std::map<int, set<int>>::iterator it;

	it = m_mapDealerOrganRelation.find(nOrganID);
	if(it != m_mapDealerOrganRelation.end()) 
    {
		it->second.erase(nDealerID);
	}
}

bool CDataInfo::GetDealerByOrganID(int nOrganID, set<int>& setDealerID)
{
	std::map<int, set<int>>::iterator it;

	it = m_mapDealerOrganRelation.find(nOrganID);
	if(it != m_mapDealerOrganRelation.end())
    {
		setDealerID = it->second;
		return true;
	}
	else
		return false;
}

string CDataInfo::GetOrgNameByUserID(int nUserID)
{
    std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    for ( ;it != m_vecUserAndOrgRelation.end(); it++ )
    {
        if( nUserID == it->nUserID )
            return GetOrganizationNameByID(it->nOrgID);
    }    

    return "";
}

void CDataInfo::SetLogonInfo( const sLoginRsp& loginRsp )
{
	m_loginRsp = loginRsp;
}

void CDataInfo::GetUserInfosByOrgID(int nOrgID, UserType nUserType, vector<UserInfo>& vecUserInfo, bool bSubOrg)
{
    vector<UserAndOrgRelation>::iterator it_Rela=m_vecUserAndOrgRelation.begin();
    for(;it_Rela!=m_vecUserAndOrgRelation.end();it_Rela++)
    {
        if(it_Rela->nOrgID == nOrgID)
        {
            map<int,UserInfo>::iterator itUserInfo=m_mapUserInfo.find(it_Rela->nUserID);
            if(itUserInfo!=m_mapUserInfo.end() && itUserInfo->second.nUserType==nUserType)
                vecUserInfo.push_back(itUserInfo->second);
        }
    }

    if(!bSubOrg)
        return;

    map<int, map<int, Organization>> mapAssetMgmtOrgTree;
    GetOrganization(mapAssetMgmtOrgTree);

    map<int, map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
    if(it != mapAssetMgmtOrgTree.end())
    {
        map<int, Organization>& mapSub = it->second;
        if(mapSub.begin()==mapSub.end())
            return;

        GetUserInfosBySubOrgID(nUserType, mapAssetMgmtOrgTree, mapSub, vecUserInfo, bSubOrg);
    }
}

void CDataInfo::GetUserInfosBySubOrgID(UserType nUserType, map<int, map<int, Organization>>& mapMain, map<int, Organization>& mapSub, vector<UserInfo>& vecUserInfo, bool bSubOrg)
{
    map<int, Organization>::iterator itSub = mapSub.begin();
    for(; itSub != mapSub.end(); itSub++) 
    {
        vector<UserInfo> vecUserInfo2;
        GetUserInfosByOrgID(itSub->first,nUserType,vecUserInfo2,bSubOrg);
        if(!vecUserInfo2.empty())
        {
            vector<UserInfo>::iterator itUserInfo = vecUserInfo2.begin();
            for (;itUserInfo!=vecUserInfo2.end();itUserInfo++)
                vecUserInfo.push_back(*itUserInfo);
        }

//         map<int, map<int, Organization>>::iterator it = mapMain.find(itSub->first);
//         if(it != mapMain.end())
//             GetUserInfosBySubOrgID(nUserType, mapMain, it->second, vecUserInfo, bSubOrg);
    }
}

void CDataInfo::GetLogonInfo(sLoginRsp& loginRsp)
{
    memcpy(&loginRsp,&m_loginRsp,sizeof(sLoginRsp));
}

void CDataInfo::ClearPrivilege()
{
    m_mapPrivilege.clear();
}

void CDataInfo::SetPrivilege(std::map<int,Privilege>& mapPrivilege)
{
    m_mapPrivilege = mapPrivilege;
}

void CDataInfo::GetPrivilege(std::map<int,Privilege>& mapPrivilege)
{
    mapPrivilege = m_mapPrivilege;
}

void CDataInfo::SetUserType2Privilege(std::map<int,std::vector<Privilege>>& mapUserType2Privilege)
{
    m_mapUserType2Privilege = mapUserType2Privilege;
}

void CDataInfo::SetParentID2Privilege(std::map<int,std::vector<Privilege>>& mapParentID2Privilege)
{
    m_mapParentID2Privilege = mapParentID2Privilege;
}

void CDataInfo::GetUserType2Privilege(std::map<int,std::vector<Privilege>>& mapUserType2Privilege)
{
    mapUserType2Privilege = m_mapUserType2Privilege;
}

void CDataInfo::GetParentID2Privilege(std::map<int,std::vector<Privilege>>& mapParentID2Privilege)
{
    mapParentID2Privilege = m_mapParentID2Privilege;
}

void CDataInfo::ClearRolePrivilegeRelation()
{
    m_mapRoleID2Privilege.clear();
}

void CDataInfo::SetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2Privilege)
{
    m_mapRoleID2Privilege = mapRoleID2Privilege;
}

void CDataInfo::SetRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID)
{
    std::map<int,std::vector<int>>::iterator it = m_mapRoleID2Privilege.find(nRoleID);
    if( it != m_mapRoleID2Privilege.end() )
    {
        m_mapRoleID2Privilege.erase(it);
    }

    m_mapRoleID2Privilege[nRoleID] = vecPrivilegeID;
}

void CDataInfo::GetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2Privilege)
{
    mapRoleID2Privilege = m_mapRoleID2Privilege;
}

void CDataInfo::ClearBrokerInfos()
{
    m_mapBrokerInfo.clear();
}

void CDataInfo::SetBrokerInfos( std::map<int,BrokerInfo>& mapBrokerInfo )
{
	m_mapBrokerInfo = mapBrokerInfo;
}

void CDataInfo::GetBrokerInfos( std::map<int,BrokerInfo>& mapBrokerInfo )
{
	mapBrokerInfo = m_mapBrokerInfo;
}

int CDataInfo::GetBrokerIDByName(std::string szName)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
    for(;it != m_mapBrokerInfo.end();it++)
    {
        if(0 == ::strcmp(szName.c_str(),it->second.szCompany))
        {
            return it->second.nBrokerID;
        }
    }

    return -1;
}

std::string CDataInfo::GetBrokerCodeByID(int brokerID)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(brokerID);
    if(it != m_mapBrokerInfo.end())
    {
        return it->second.szBrokerCode;
    }

    return "";  
}

std::string CDataInfo::GetBrokerNameByID(int brokerID)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(brokerID);
    if(it != m_mapBrokerInfo.end())
    {
        return it->second.szCompany;
    }

    return ""; 
}

int CDataInfo::GetBrokerIDByCode(const string& szCode)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
    for (;it!=m_mapBrokerInfo.end();it++)
    {
        if (0 == strcmp(szCode.c_str(),it->second.szBrokerCode))
            return it->second.nBrokerID;        
    }

    return -1;
}

bool CDataInfo::IsValidOfBrokerCode(std::string strBrokerCode)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
    for (;it!=m_mapBrokerInfo.end();it++)
    {
        if (0 == strcmp(strBrokerCode.c_str(),it->second.szBrokerCode))
            return false;        
    }

    return true;
}

bool CDataInfo::IsQuotBrokerByID(int nBrokerID)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(nBrokerID);
    if (it!=m_mapBrokerInfo.end())
    {
        return (it->second.nType)?true:false;        
    }

    return false;
}

void CDataInfo::ClearServerGroups()
{
    m_mapServerGroup.clear();
}   

void CDataInfo::SetServerGroups( std::map<int,std::map<int,ServerGroup>>& mapServerGroup )
{
	m_mapServerGroup = mapServerGroup;
}

void CDataInfo::GetServerGroups( int nBrokerID, std::map<int,ServerGroup>& mapServerGroup )
{
	mapServerGroup.clear();
	std::map<int, std::map<int, ServerGroup>>::iterator it_broker = m_mapServerGroup.find(nBrokerID);
	if ( it_broker != m_mapServerGroup.end())
	{
		mapServerGroup = it_broker->second;
	}
}

bool CDataInfo::GetServerGroupByID(int nBrokerID, int nServerID, ServerGroup& oServerGroup)
{
    std::map<int, std::map<int, ServerGroup>>::iterator it_broker = m_mapServerGroup.find(nBrokerID);
    if ( it_broker != m_mapServerGroup.end())
    {
        std::map<int, ServerGroup>& mapGroup = it_broker->second;
        std::map<int, ServerGroup>::iterator it_server = mapGroup.find(nServerID);
        if (it_server!=mapGroup.end())
        {
            oServerGroup = it_server->second;
            return true;
        }
    }

   return false;
}
std::string CDataInfo::GetServerGroupNameByID(int nBrokerID,int nServerGroupID)
{
    std::map<int,std::map<int,ServerGroup>>::iterator it = m_mapServerGroup.find(nBrokerID);
    if ( it != m_mapServerGroup.end() )
    {
        std::map<int,ServerGroup>::iterator it2 = it->second.find(nServerGroupID);
        if( it2 != it->second.end() )
            return it2->second.szServerName;
    }

    return "";
}

int CDataInfo::GetServerGroupID(int nBrokerID, std::string szServerName)
{
    std::map<int,ServerGroup> mapServerGroup;
    GetServerGroups(nBrokerID,mapServerGroup);
    if(mapServerGroup.empty())
        return false;

    std::map<int,ServerGroup>::iterator it = mapServerGroup.begin();
    for(;it != mapServerGroup.end();it++)
    {
        if(0 == ::strcmp(szServerName.c_str(),it->second.szServerName))
        {            
            return it->first;
        }
    }

    return -1;
}

void CDataInfo::SetServerAddress( std::map<int, std::map<int, ServerAddress>>& mapServerAddr )
{
	m_mapServerAddr = mapServerAddr;
}

void CDataInfo::GetServerAddress( int nServerID, std::map<int, ServerAddress>& mapServerAddr )
{
	mapServerAddr.clear();
	std::map<int, std::map<int, ServerAddress>>::iterator it_group = m_mapServerAddr.find(nServerID);
	if ( it_group != m_mapServerAddr.end())
	{
		mapServerAddr = it_group->second;
	}
}

bool CDataInfo::GetServerAddrByID(int nServerID, int nAddrID, ServerAddress& oServerAddr)
{
    std::map<int, std::map<int, ServerAddress>>::iterator it_server = m_mapServerAddr.find(nServerID);
    if ( it_server != m_mapServerAddr.end())
    {
        std::map<int, ServerAddress>& mapAddr = it_server->second;
        std::map<int, ServerAddress>::iterator it_addr = mapAddr.find(nAddrID);
        if (it_addr!=mapAddr.end())
        {
            oServerAddr = it_addr->second;
            return true;
        }
    }

    return false;
}

//行情服务器组地址
void CDataInfo::ClearServerAddressQuot()
{
    m_mapServerAddrQuot.clear();
}
void CDataInfo::SetServerAddressQuot(std::map<int, ServerAddressQuot>& mapServerAddr)
{
    m_mapServerAddrQuot = mapServerAddr;
}
void CDataInfo::SetServerAddressQuot(ServerAddressQuot& addr)
{
    m_mapServerAddrQuot[addr.nAddrID] = addr;
}
void CDataInfo::DelServerAddressQuot(int nAddrID)
{
    std::map<int, ServerAddressQuot>::iterator it = m_mapServerAddrQuot.find(nAddrID);
    if(it!=m_mapServerAddrQuot.end())
        m_mapServerAddrQuot.erase(it);
}
void CDataInfo::GetServerAddressQuot(std::map<int, ServerAddressQuot>& mapServerAddr)
{
    mapServerAddr = m_mapServerAddrQuot;
}
bool CDataInfo::GetServerAddrQuotByID(int nAddrID, ServerAddressQuot& oServerAddr)
{
    std::map<int, ServerAddressQuot>::iterator it_server = m_mapServerAddrQuot.find(nAddrID);
    if ( it_server != m_mapServerAddrQuot.end())
    {
        oServerAddr = it_server->second;
        return true;
    }

    return false;
}

void CDataInfo::SetBrokerInfo( BrokerInfo& brokerInfo )
{
	m_mapBrokerInfo[brokerInfo.nBrokerID] = brokerInfo;
}

void CDataInfo::SetBrokerInfoQuot(int nBrokerID, const string& szAccount, const string& szPWD)
{
    for(std::map<int,BrokerInfo>::iterator it=m_mapBrokerInfo.begin();it!=m_mapBrokerInfo.end();it++)
    {
        if(it->second.nType)
        {
            it->second.nType = 0;
            memset(it->second.szAccount,0,sizeof(it->second.szAccount));
            memset(it->second.szPassword,0,sizeof(it->second.szPassword));
        }
    }


    std::map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.find(nBrokerID);
    if(it!=m_mapBrokerInfo.end())
    {
        it->second.nType = 1;
        strcpy(it->second.szAccount,szAccount.c_str());
        strcpy(it->second.szPassword,szPWD.c_str());
    }
}

void CDataInfo::DelBrokerInfo( int nBrokerID )
{
	std::map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.find(nBrokerID);
	if ( it != m_mapBrokerInfo.end())
	{
		m_mapBrokerInfo.erase(it);
	}

	//服务器组
	std::map<int, std::map<int, ServerGroup>>::iterator it_broker = m_mapServerGroup.find(nBrokerID);
	if ( it_broker != m_mapServerGroup.end() )
	{
		std::map<int, ServerGroup>::iterator it_server = it_broker->second.begin();
		for ( ; it_server != it_broker->second.end(); it_server++ )
		{
			//服务器地址
			std::map<int, std::map<int, ServerAddress>>::iterator it_addr_serverID = m_mapServerAddr.find(it_server->second.nServerID);
			if ( it_addr_serverID != m_mapServerAddr.end())
			{
				m_mapServerAddr.erase(it_addr_serverID);
			}
		}

		m_mapServerGroup.erase(it_broker);
	}

    //删除与委托交易账号的关系
    std::map<int,TradeAccount>::iterator it_rela = m_mapTradeAccount.begin();
    for (;it_rela!=m_mapTradeAccount.end();it_rela++)
    {
        if(it_rela->second.nBrokerID == nBrokerID)
        {
            it_rela->second.nBrokerID = TRADEACCOUNT_BROKERIDORSERVERID_NULL;
            break;
        }
    }
}

void CDataInfo::SetServerGroup( int nBrokerID, ServerGroup& serverGroup )
{
	std::map<int, std::map<int, ServerGroup>>::iterator it = m_mapServerGroup.find(nBrokerID);
	if ( it != m_mapServerGroup.end())
	{
		it->second[serverGroup.nServerID] = serverGroup;
	}
	else
	{
		std::map<int, ServerGroup> mapServerGroup;
		mapServerGroup[serverGroup.nServerID] = serverGroup;
		m_mapServerGroup[nBrokerID] = mapServerGroup;
	}
}

void CDataInfo::DelServerGroup( int nBrokerID, int nServerID )
{
	std::map<int, std::map<int, ServerGroup>>::iterator it_broker = m_mapServerGroup.find(nBrokerID);
	if ( it_broker != m_mapServerGroup.end() )
	{
		std::map<int, ServerGroup>::iterator it_server = it_broker->second.find(nServerID);
		if ( it_server != it_broker->second.end())
		{
			it_broker->second.erase(it_server);
		}

		//服务器地址
		std::map<int, std::map<int, ServerAddress>>::iterator it_addr_serverID = m_mapServerAddr.find(nServerID);
		if ( it_addr_serverID != m_mapServerAddr.end())
		{
			m_mapServerAddr.erase(it_addr_serverID);
		}
	}

    //删除与委托交易账号的关系
    std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.begin();
    for (;it!=m_mapTradeAccount.end();it++)
    {
        if(it->second.nBrokerID == nBrokerID)
        {
            it->second.nServerGroupID = TRADEACCOUNT_BROKERIDORSERVERID_NULL;
            break;
        }
    }
}
bool CDataInfo::IsValidOfServerName(int nBrokerID, std::string strServerName)
{
    std::map<int, std::map<int, ServerGroup>>::iterator it_broker = m_mapServerGroup.find(nBrokerID);
    if ( it_broker != m_mapServerGroup.end() )
    {
        std::map<int, ServerGroup>::iterator it_server = it_broker->second.begin();
        for (;it_server!=it_broker->second.end();it_server++)
        {
            if (0 == strcmp(strServerName.c_str(),it_server->second.szServerName))
                return false;        
        }
    }

    return true;
}

bool CDataInfo::IsValidOfSettle(int nBrokerID,bool bChecked)
{
    std::map<int, std::map<int,ServerGroup>>::iterator it_all = m_mapServerGroup.find(nBrokerID);
    if(it_all!=m_mapServerGroup.end())
    {
        std::map<int,ServerGroup>& mapServerGroup=it_all->second;
        for (std::map<int,ServerGroup>::iterator it = mapServerGroup.begin();it!=mapServerGroup.end();it++)
        {
            if(it->second.nIsSettle && bChecked)
            {
                return false;
            }                    
        }
    }

    return true;
}

void CDataInfo::SetServerAddress( int nServerID, ServerAddress& addr )
{
	std::map<int, std::map<int, ServerAddress>>::iterator it_server = m_mapServerAddr.find(nServerID);
	if ( it_server != m_mapServerAddr.end())
	{
		it_server->second[addr.nAddrID] = addr;
	}
	else
	{
		std::map<int, ServerAddress> mapServerAddr;
		mapServerAddr[addr.nAddrID] = addr;
		m_mapServerAddr[nServerID] = mapServerAddr;
	}
}

void CDataInfo::DelServerAddress( int nServerID, int nAddrID )
{
	std::map<int, std::map<int, ServerAddress>>::iterator it_server = m_mapServerAddr.find(nServerID);
	if ( it_server != m_mapServerAddr.end())
	{
		std::map<int, ServerAddress>::iterator it_addr = it_server->second.find(nAddrID);
		if ( it_addr != it_server->second.end())
		{
			it_server->second.erase(it_addr);
			if ( it_server->second.empty())
			{
				m_mapServerAddr.erase(it_server);
			}
		}
	}
}

void CDataInfo::ClearTradeAccount()
{
    m_mapTradeAccount.clear();
}

void CDataInfo::SetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount)
{
    m_mapTradeAccount = mapTradeAccount;
}
void CDataInfo::SetTradeAccount(const TradeAccount* pTradeAccount)
{
    m_mapTradeAccount[pTradeAccount->nTradeAccountID] = *pTradeAccount;
}
void CDataInfo::DelTradeAccount(int nTradeAccountID)
{
	std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.find(nTradeAccountID);
	if ( it != m_mapTradeAccount.end())
	{
		m_mapTradeAccount.erase(it);
	}
}
void CDataInfo::GetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount)
{
    mapTradeAccount = m_mapTradeAccount;
}

bool CDataInfo::GetTradeAccountByAccount(string& sAccount,TradeAccount& oTradeAccount)
{
    std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.begin();
    for (;it!=m_mapTradeAccount.end();it++)
    {
        if (strcmp(it->second.szTradeAccount,sAccount.c_str()) == 0)        
        {
            memcpy(&oTradeAccount,&(it->second),sizeof(TradeAccount));     
            return true;
        }
    }

    return false;
}

string CDataInfo::GetTradeAccountByID(int nTradeAccountID)
{
    string strTradeAccount(" ");
    std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.find(nTradeAccountID);
    if (it!=m_mapTradeAccount.end())
        strTradeAccount = it->second.szTradeAccount;

    return strTradeAccount;
}

int CDataInfo::GetBrokerIDByAccountID(int nTradeAccountID)
{
    std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.find(nTradeAccountID);
    if (it!=m_mapTradeAccount.end())
        return it->second.nBrokerID;

    return -1;
}

int CDataInfo::GetTradeAccountIDByAccount(string szTradeAccount)
{
    std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.begin();
    for (; it!=m_mapTradeAccount.end(); it++)
    {
        if ( 0 == strcmp(it->second.szTradeAccount,szTradeAccount.c_str()) )
        {
            return it->first;
        }
    }

    return -1;
}

bool CDataInfo::IsValidOfTradeAccount(std::string strTradeAccount)
{
    std::map<int, TradeAccount>::iterator it = m_mapTradeAccount.begin();
    for (;it!=m_mapTradeAccount.end();it++)
    {
        if (0 == strcmp(strTradeAccount.c_str(),it->second.szTradeAccount))
            return false;        
    }

    return true;
}

//用户与委托交易账号
void CDataInfo::ClearUserAndTraderAccountRelation()
{
    m_mapUserAndTradeAccountRelation.clear();
}

void CDataInfo::SetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation)
{
    m_mapUserAndTradeAccountRelation = mapUserAndOrgRelation;
}

void CDataInfo::AddUserAndTraderAccountRelation(const UserAndTradeAccountRelation* pRelation)
{
    std::map<int,UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(pRelation->nUserID);
    if( it != m_mapUserAndTradeAccountRelation.end() )
        it->second = *pRelation;
    else
        m_mapUserAndTradeAccountRelation[pRelation->nUserID] = *pRelation;
}

void CDataInfo::GetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation)
{
    mapUserAndOrgRelation = m_mapUserAndTradeAccountRelation;
}

//组织机构和委托交易账号
void CDataInfo::ClearTradeAccountAndOrgRelation()
{
    m_mapTradeAccountAndOrgRelation.clear();
}
void CDataInfo::SetTradeAccountAndOrgRelation(const TradeAccountAndOrg& relation)
{
    std::map<int, TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.find(relation.nTradeAccountID);
    if(it!=m_mapTradeAccountAndOrgRelation.end())
        it->second.nOrgID = relation.nOrgID;
    else
        m_mapTradeAccountAndOrgRelation[relation.nTradeAccountID] = relation;
}
void CDataInfo::SetTradeAccountAndOrgRelation(std::map<int, TradeAccountAndOrg>& mapTradeAccountAndOrgRelation)
{
    m_mapTradeAccountAndOrgRelation = mapTradeAccountAndOrgRelation;
}
string CDataInfo::GetOrgByTradeAccountID(const int nTradeAccountID)
{
    std::map<int, TradeAccountAndOrg>::iterator it = m_mapTradeAccountAndOrgRelation.find(nTradeAccountID);
    if(it!=m_mapTradeAccountAndOrgRelation.end())
        return GetOrganizationNameByID(it->second.nOrgID);

    return "";
}

//理财产品
void CDataInfo::ClearFinanceProduct()
{
    m_mapFinancialProduct.clear();
}

void CDataInfo::SetFinancialProduct(const FinancialProduct *pFP)
{
    if(pFP == NULL)
        return;
    m_mapFinancialProduct[pFP->nID] = *pFP;
}

void CDataInfo::ClearProductTraderRelation()
{
    m_mapTraderProRelation.clear();
}

void CDataInfo::ClearProductTraderRelation(int nType, int nID)
{
    if (PRODUCT_ASSGIN_TRADER_TYPE == nType)
    {
        std::map<int,std::vector<TraderProRelation>>::iterator it = m_mapTraderProRelation.find(nID);
        if( it!=m_mapTraderProRelation.end() )
            m_mapTraderProRelation.erase(it);
    } 
    else if(TRADER_ASSGIN_PRODUCT_TYPE == nType)
    {
        std::map<int,std::vector<TraderProRelation>>::iterator it = m_mapTraderProRelation.begin();
        while( it!=m_mapTraderProRelation.end() )
        {
            std::vector<TraderProRelation>::iterator it1 = it->second.begin();
            while ( it1 != it->second.end() )
            {
                if ( it1->nTraderID == nID)
                {
                    it1 = it->second.erase(it1);
                }
                else
                    ++it1;
            }

            if (it->second.empty())
            {
                it = m_mapTraderProRelation.erase(it);
            }
            else
                ++it;
        }
    }
}

void CDataInfo::SetProductTraderRelation(const TraderProRelation *pRelate)
{
    if(pRelate == NULL)
        return;
   
    m_mapTraderProRelation[pRelate->nFinancialProductID].push_back(*pRelate);
}

void CDataInfo::GetFinancialProduct(std::map<int, FinancialProduct>&	mapFinancialProduct)
{
    mapFinancialProduct = m_mapFinancialProduct;
}

bool CDataInfo::GetFinancialProduct(int nFinanceProductID, FinancialProduct& oFinancialProduct)
{
    std::map<int,FinancialProduct>::iterator it = m_mapFinancialProduct.find(nFinanceProductID);
    if ( it!=m_mapFinancialProduct.end() )
    {
        oFinancialProduct = it->second;
        return true;
    }

    return false;
}

string CDataInfo::GetFinancialProductNameByID(int nProductID)
{
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.find(nProductID);
    string strName;
    if (it!=m_mapFinancialProduct.end())    
        strName = it->second.szName; 
    else 
        strName = " ";
    
    return strName; 
}

int CDataInfo::GetFinancialProductIDByName(string& sName)
{
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.begin();
    for (;it!=m_mapFinancialProduct.end();it++)
    {
        if (strcmp(it->second.szName,sName.c_str()) == 0)
            return it->second.nID; 
    }

    return -1;
}

void CDataInfo::DeleteFinanceProduct(int nProductID)
{   
    std::map<int,FinancialProduct>::iterator it = m_mapFinancialProduct.find(nProductID);
    if( it!=m_mapFinancialProduct.end())
    {
        m_mapFinancialProduct.erase(it); 
    }

    //关系操作
    std::map<int, std::vector<TraderProRelation>>::iterator it_rela = m_mapTraderProRelation.find(nProductID);
    if(it_rela!=m_mapTraderProRelation.end())
        m_mapTraderProRelation.erase(it_rela);
}

void CDataInfo::GetProTraderRelation(std::vector<TraderProRelation>& vec)
{
    vec.clear();

    
    std::map<int,std::vector<TraderProRelation>>::iterator it = m_mapTraderProRelation.begin();
    for (; it!=m_mapTraderProRelation.end(); it++)    
    {    
        std::vector<TraderProRelation> vecRelation = it->second;        
        for(int j=0; j<(int)vecRelation.size(); j++)
            vec.push_back(vecRelation[j]);    
    }
}

void CDataInfo::GetProductByTraderID(int nTraderID,std::set<int>& vec)
{
    vec.clear();

    std::vector<TraderProRelation> vecRelation;
    std::map<int,std::vector<TraderProRelation>>::iterator it = m_mapTraderProRelation.begin();
    for (; it != m_mapTraderProRelation.end(); it++)
    {        
        vecRelation = it->second;
        std::vector<TraderProRelation>::iterator it1 = vecRelation.begin();
        for( ; it1!=vecRelation.end(); it1++)
            if( it1->nTraderID == nTraderID)
                vec.insert(it->first);
    }   
}

string CDataInfo::GetProductByUserID(int nUserID)
{
    string strProductName = " ",strProduct[30];
    std::set<int> vecProduct;
    GetProductByTraderID(nUserID,vecProduct);

    std::set<int>::iterator it = vecProduct.begin();
    int nCount = vecProduct.size();
    for(int k=0; it != vecProduct.end(); it++,k++)
    {
        strProduct[k] = GetFinancialProductNameByID(*it);
        if(k==nCount-1)
            strProductName += strProduct[k];
        else
            strProductName += (strProduct[k]+",");
    }


    return strProductName;
}

bool CDataInfo::IsValidOfProductName(const string& sName)
{
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.begin();
    for (;it!=m_mapFinancialProduct.end();it++)
    {
        if (0 == strcmp(sName.c_str(),it->second.szName))
            return false;        
    }

    return true;
}

//交易策略
void CDataInfo::SetTradeStrategy(const TradeStrategy *pStrategy)
{
    if(pStrategy == NULL)
        return;
    m_mapTradeStrategy[pStrategy->nID] = *pStrategy;
}

void CDataInfo::GetTradeStrategy(std::map<int, TradeStrategy>&	mapTradeStrategy)
{
    mapTradeStrategy = m_mapTradeStrategy;
}

bool CDataInfo::GetTradeStrategyByID(int nTradeStrategyID, TradeStrategy& oStrategy)
{
    std::map<int, TradeStrategy>::iterator it = m_mapTradeStrategy.find(nTradeStrategyID);
    if(it == m_mapTradeStrategy.end())
        return false;

    oStrategy = it->second;
    return true;
}

bool CDataInfo::GetTradeStrategyByName(TradeStrategy& oStrategy,string& sName)
{
    std::map<int, TradeStrategy>::iterator it = m_mapTradeStrategy.begin();
    for (;it!=m_mapTradeStrategy.end();it++)
    {
        if (strcmp(it->second.szName,sName.c_str()) == 0)        
        {
            oStrategy = it->second;     
            return true;
        }
    }

    return false;
}

int CDataInfo::GetTradeStrategyIDByName(string& sName)
{
    std::map<int, TradeStrategy>::iterator it = m_mapTradeStrategy.begin();
    for (;it!=m_mapTradeStrategy.end();it++)
    {
        if (strcmp(it->second.szName,sName.c_str()) == 0)        
        {
            return it->second.nID;
        }
    }

    return -1;
}

void CDataInfo::ClearTradeStrategy()
{
    m_mapTradeStrategy.clear();
}

void CDataInfo::DeleteTradeStrategy(int nTradeStrategyID)
{
    std::map<int, TradeStrategy>::iterator it = m_mapTradeStrategy.find(nTradeStrategyID);
    if (it!=m_mapTradeStrategy.end())
    {
        it = m_mapTradeStrategy.erase(it);
    }

    //关系操作
    std::map<int, std::vector<int>>::iterator it_strategy = m_mapStrategyOrgRelation.find(nTradeStrategyID);
    if(it_strategy!=m_mapStrategyOrgRelation.end())
        m_mapStrategyOrgRelation.erase(it_strategy);
}

void CDataInfo::ClearStrategyOrgRelation()
{
    m_mapStrategyOrgRelation.clear();
}

void CDataInfo::SetStrategyOrgRelation(std::map<int, std::vector<int>>& mapRelate)
{
    m_mapStrategyOrgRelation = mapRelate;
}

void CDataInfo::SetStrategyOrgRelation(int nStrategyID, std::vector<int>& vecOrgID)
{
    m_mapStrategyOrgRelation[nStrategyID] = vecOrgID;
}

void CDataInfo::GetStrategyOrgRelation(std::vector<AssetOrgStrategyRelation>& vec)
{
    vec.clear();
    
    AssetOrgStrategyRelation oRelation;
    std::map<int, std::vector<int>>::iterator it = m_mapStrategyOrgRelation.begin();
    for (; it!=m_mapStrategyOrgRelation.end(); it++)
    {
        oRelation.nStrategyID = it->first;

        std::vector<int> vecOrgID = it->second;
        std::vector<int>::iterator itOrg = vecOrgID.begin();
        for ( ;itOrg!=vecOrgID.end(); itOrg++ )
        {
            oRelation.nAssetOrgID = (*itOrg);
            vec.push_back(oRelation);
        }
    }
}

void CDataInfo::ClearFundInOut()
{
    m_vecFundInOut.clear();
}

void CDataInfo::SetFundInOut(sFundInOut& oFundInOut)
{
    m_vecFundInOut.push_back(oFundInOut);
}

void CDataInfo::GetStrategyOrgRelation(std::vector<sFundInOut>& vecFundInOut)
{
    vecFundInOut = m_vecFundInOut;
}

bool CDataInfo::IsValidOfStrategyName(const string& sName)
{
    std::map<int, TradeStrategy>::iterator it = m_mapTradeStrategy.begin();
    for (;it!=m_mapTradeStrategy.end();it++)
    {
        if (0 == strcmp(sName.c_str(),it->second.szName))
            return false;        
    }

    return true;
}

void CDataInfo::SetAccountStatus(const sAccountStatus& sAS)
{
    m_mapAccount2Status[sAS.mBrokerAccount.AccountID] = sAS;
}

EnumAccountStatus CDataInfo::GetAccountStatus(const string& sAccount)
{
     map<string,sAccountStatus>::iterator it=m_mapAccount2Status.find(sAccount);
     if(it!=m_mapAccount2Status.end())
     {
        return it->second.meStatus;
     }

     return ACCOUNT_STATUS_UnInit;
}

void CDataInfo::SetOriginalSettlementPrice(std::map<std::string,SettlementPriceField>&  lMapFiled)
{
	m_mapSettlePrice = lMapFiled;
	m_mapLocalPrice = lMapFiled;
	
}
void CDataInfo::GetAllProducts(set<string>& setProductID, int exchange )
{

	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.begin();
	for ( it; it != m_mapSettlePrice.end(); it++ )
	{
		if ( !strcmp(it->second.ExchangeID ,ExchangeTabIndex2String(exchange).c_str() ))
		{
			setProductID.insert(it->second.ProductID);
		}
		
	}
}


const string CDataInfo::ExchangeTabIndex2String(const int& ExchangeID)
{

	string strExchange;

	switch (ExchangeID)
	{
	case 0:
		strExchange= "SHFE";
		break;
	case 1:
		strExchange=  "CZCE";
		break;
	case 2:
		strExchange=  "DCE";
		break;
	case 3:
		strExchange=  "CFFEX";
		break;
	default:
		strExchange=  "SHFE";
		break;
	}
	return strExchange;
}
const string CDataInfo::ExchangeID2String(const string& ExchangeID)
{
	const string SHFE="上期所";
	const string CZCE="郑商所";
	const string DCE="大商所";
	const string CFFEX="中金所";

	if(ExchangeID=="SHFE")
		return SHFE;
	else if(ExchangeID=="CZCE")
		return CZCE;
	else if(ExchangeID=="DCE")
		return DCE;
	else if(ExchangeID=="CFFEX")
		return CFFEX;
	else
		return __UNKOWN__;
}
const string CDataInfo::ExchangeName2ID( const string& name)
{
	const static std::string strCZCE="郑商所";
	const static std::string strDCE="大商所";
	const static std::string strSHFE="上期所";
	const static std::string strCFFEX="中金所";


	if(name==strCZCE)
		return std::string("CZCE");
	if(name==strDCE)
		return std::string("DCE");
	if(name==strSHFE)
		return std::string("SHFE");
	if(name==strCFFEX)
		return std::string("CFFEX");
	return "";
}
bool CDataInfo::GetSettlementPrice(std::vector<SettlementPriceField>& data, const string& ProductID)
{
	if (m_mapSettlePrice.empty())
	{
		return false;
	}
	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.begin();
	for ( it; it != m_mapSettlePrice.end(); it++ )
	{
		if ( !strcmp(it->second.ProductID ,ProductID.c_str() ))
		{
			data.push_back(it->second);
		}

	}
    return true;
}

double CDataInfo::GetSettlementPrice(const string& InstrumentID)
{
	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.find(InstrumentID);
	if (  it != m_mapSettlePrice.end())
	{
		
		return it->second.SettlementPrice;

	}
   return 0;
}
double CDataInfo::GetLastSettlementPrice(const string& InstrumentID)
{
	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.find(InstrumentID);
	if (  it != m_mapSettlePrice.end())
	{

		return it->second.LastSettlementPrice;

	}
	return 0;
}
bool CDataInfo::ModifySettlementPrice(SettlementPriceField& Filed)
{

	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.find(Filed.InstrumentID);
	if( it != m_mapSettlePrice.end() )
	{
		m_mapSettlePrice[Filed.InstrumentID] = Filed;

	}
	return TRUE;
}
bool CDataInfo::ModifyLocalPrice(SettlementPriceField& Filed)
{

	std::map<std::string,SettlementPriceField>::iterator it = m_mapLocalPrice.find(Filed.InstrumentID);
	if( it != m_mapLocalPrice.end() )
	{
		m_mapLocalPrice[Filed.InstrumentID] = Filed;

	}
	return TRUE;
}
bool CDataInfo::CheckSettlePrice()
{
	double   InvalideValue = util::GetDoubleInvalidValue();
	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.begin();
	for ( it; it != m_mapSettlePrice.end(); it++ )
	{
		if ( it->second.SettlementPrice ==  InvalideValue )
		{
			string rrr=it->second.InstrumentID;
			return false;
		}

	}
	return true;
}
bool CDataInfo::CompareSettlePrice(string & InsName)
{
	double   InvalideValue = util::GetDoubleInvalidValue();
	std::map<std::string,SettlementPriceField>::iterator it = m_mapSettlePrice.begin();
	for ( it; it != m_mapSettlePrice.end(); it++ )
	{
		if ( it->second.SettlementPrice !=  m_mapLocalPrice[it->first].SettlementPrice )
		{
			InsName=it->second.InstrumentID;
			return false;
		}

	}
	return true;
}
void CDataInfo::SetInstrumentInfo(std::map<std::string, PlatformStru_InstrumentInfo>& lMapFiled )
{
	m_mapInstrumentInfo = lMapFiled;
}
void CDataInfo::SetCommissionMarginRate(std::map<RateKey,CommissionMarginRate>& lMapFiled )
{
	m_mapCommissionMarginRate = lMapFiled;
}
void CDataInfo::SetCommissionMarginRate(RateKey& key,CommissionMarginRate& Filed )
{
	m_mapCommissionMarginRate[key] = Filed;
}
void CDataInfo::ClearCommissionMarginRate( )
{
	m_mapCommissionMarginRate.clear();
}
string CDataInfo::GetExchangeID(string InstrumentID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrumentInfo.find(InstrumentID);
	if (it!=m_mapInstrumentInfo.end() && it->second.ExchangeID != NULL)
	{
		return it->second.ExchangeID;
	}
	return  __UNKOWN__;
}
bool CDataInfo::GetCommissionMarginRate(std::vector<CommissionMarginRate>& vecRate ,int exchandeID,const std::string& AccountID,const std::string& BrokerID)
{
	if (m_mapCommissionMarginRate.empty())
	{
		return false;
	}
	if ( BrokerID == SELECT_ALL && AccountID != SELECT_ALL )
	{
		std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
		for ( it; it!=m_mapCommissionMarginRate.end();it++ )
		{
			std::string strExchange = GetExchangeID(it->second.InstrumentID);
			std::string strExchangeID = ExchangeTabIndex2String(exchandeID);
			if ( it->second.AccountID == AccountID  && strExchange == strExchangeID)
			{
				vecRate.push_back(it->second);
			}
		}
	}
	else if(BrokerID == SELECT_ALL && AccountID == SELECT_ALL)
	{

		std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
		for ( it; it!=m_mapCommissionMarginRate.end();it++ )
		{
			std::string strExchange = GetExchangeID(it->second.InstrumentID);
			std::string strExchangeID = ExchangeTabIndex2String(exchandeID);
			if ( strExchange == strExchangeID)
			{
				vecRate.push_back(it->second);
			}
		}
	}
	else if(BrokerID != SELECT_ALL && AccountID == SELECT_ALL)
	{
		std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
		for ( it; it!=m_mapCommissionMarginRate.end();it++ )
		{
			std::string strExchange = GetExchangeID(it->second.InstrumentID);
			std::string strExchangeID = ExchangeTabIndex2String(exchandeID);
			if ( it->second.BrokerID == BrokerID && strExchange == strExchangeID)
			{
				vecRate.push_back(it->second);
			}
		}
	}
	else
	{
		std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
		for ( it; it!=m_mapCommissionMarginRate.end();it++ )
		{
			std::string strExchange = GetExchangeID(it->second.InstrumentID);
			std::string strExchangeID = ExchangeTabIndex2String(exchandeID);
			if ( it->second.AccountID == AccountID && it->second.BrokerID == BrokerID && strExchange == strExchangeID)
			{
				vecRate.push_back(it->second);
			}
		}
	}
	

	return true;
}
bool CDataInfo::GetCommissionMarginRate(std::vector<CommissionMarginRate>& vecRate)
{
	std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
	for ( it; it!=m_mapCommissionMarginRate.end();it++ )
	{
		
		vecRate.push_back(it->second);

	}
	return true;
}
bool CDataInfo::GetBrokerIDByBrokerCode(std::string szName,int& brokerID)
{
	std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
	for(;it != m_mapBrokerInfo.end();it++)
	{
		if(0 == ::strcmp(szName.c_str(),it->second.szBrokerCode))
		{
			brokerID = it->second.nBrokerID;
			return true;
		}
	}

	return false;
}

bool CDataInfo::GetCommissionMarginRate(RateKey& key, CommissionMarginRate& Filed)
{
	std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.find(key);
	if(it == m_mapCommissionMarginRate.end())
		return false;

	Filed = it->second;
	return true;

}
void CDataInfo::SetModifiedRate(const CommissionMarginRate *pFiled)
{
	if(pFiled == NULL)
		return;
	RateKey key(pFiled->AccountID,pFiled->InstrumentID,pFiled->BrokerID);
	m_mapModifiedRate[key] = *pFiled;
}
void CDataInfo::GetModifiedRate(std::map<RateKey,CommissionMarginRate>&  lMapFiled)
{
    lMapFiled = m_mapModifiedRate;
}
void CDataInfo::ModifyCommissionMarginRate(CommissionMarginRate& Field )
{
	RateKey key(Field.AccountID,Field.InstrumentID,Field.BrokerID);
	m_mapCommissionMarginRate[key] = Field;
}
void CDataInfo::ModifyCommissionMarginRate()
{
	if (m_mapModifiedRate.empty())
	return;
	std::map<RateKey,CommissionMarginRate>::iterator it = m_mapModifiedRate.begin();
	for ( it; it!=m_mapModifiedRate.end();it++ )
	{
		m_mapCommissionMarginRate[it->first] = it->second;
	}
	
}
void CDataInfo::ClearModifiedRate()
{
    m_mapModifiedRate.clear();
}
bool CDataInfo::IsCombination(string InstrumentID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrumentInfo.find(InstrumentID);
	if (it!=m_mapInstrumentInfo.end() && it->second.ProductClass == THOST_FTDC_PC_Combination)
	{
		return true;
	}
	else
		return false;
}
bool CDataInfo::CheckRate()
{
	/*double   InvalideValue = util::GetDoubleInvalidValue();;
	std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
	for( ; it!= m_mapCommissionMarginRate.end();it++ )
	{
		if ( IsCombination(it->second.InstrumentID))
		continue;

		if ( it->second.CloseRatioByMoney == InvalideValue || it->second.CloseRatioByMoney < 0 )
		{
			return false;
		}
		if ( it->second.CloseRatioByVolume == InvalideValue || it->second.CloseRatioByVolume < 0 )
		{
			return false;
		}
		if ( it->second.CloseTodayRatioByMoney == InvalideValue || it->second.CloseTodayRatioByMoney < 0 )
		{
			return false;
		}
		if ( it->second.CloseTodayRatioByVolume == InvalideValue || it->second.CloseTodayRatioByVolume < 0 )
		{
			return false;
		}
		if ( it->second.LongMarginRatioByMoney == InvalideValue || it->second.LongMarginRatioByMoney < 0 )
		{
			return false;
		}
		if ( it->second.LongMarginRatioByVolume == InvalideValue || it->second.LongMarginRatioByVolume < 0 )
		{
			return false;
		}
		if ( it->second.ShortMarginRatioByMoney == InvalideValue || it->second.ShortMarginRatioByMoney < 0 )
		{
			return false;
		}
		if ( it->second.ShortMarginRatioByVolume == InvalideValue || it->second.ShortMarginRatioByVolume < 0 )
		{
			return false;
		}
		if ( it->second.OpenRatioByMoney == InvalideValue || it->second.OpenRatioByMoney < 0 )
		{
			return false;
		}
		if ( it->second.OpenRatioByVolume == InvalideValue || it->second.OpenRatioByVolume < 0 )
		{
			return false;
		}

	
	}*/
	return true;
}
bool CDataInfo::HaveModifiedRate()
{
	if (m_mapModifiedRate.empty())
	{
		return false;
	}
	return true;
}
void CDataInfo::SetQryRate()
{
	m_vecAllRate.clear();
	m_vecVacancyRate.clear();
	double   InvalideValue = util::GetDoubleInvalidValue();
	std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.begin();
	for( ; it!= m_mapCommissionMarginRate.end();it++ )
	{
		

		if ( IsCombination(it->second.InstrumentID))
			continue;

        m_vecAllRate.push_back(it->first);

		if ( it->second.OpenRatioByMoney == InvalideValue || it->second.OpenRatioByMoney < 0 
			|| it->second.OpenRatioByVolume == InvalideValue || it->second.OpenRatioByVolume < 0 
			|| it->second.CloseRatioByMoney == InvalideValue || it->second.CloseRatioByMoney < 0 
			|| it->second.CloseRatioByVolume == InvalideValue || it->second.CloseRatioByVolume < 0 
			|| it->second.CloseTodayRatioByMoney == InvalideValue || it->second.CloseTodayRatioByMoney < 0 
			|| it->second.CloseTodayRatioByVolume == InvalideValue || it->second.CloseTodayRatioByVolume < 0 

			|| it->second.LongMarginRatioByMoney == InvalideValue || it->second.LongMarginRatioByMoney < 0
			|| it->second.LongMarginRatioByVolume == InvalideValue || it->second.LongMarginRatioByVolume < 0
			|| it->second.ShortMarginRatioByMoney == InvalideValue || it->second.ShortMarginRatioByMoney < 0
			|| it->second.ShortMarginRatioByVolume == InvalideValue || it->second.ShortMarginRatioByVolume < 0)
		{
			m_vecVacancyRate.push_back(it->first);
		}

	}

}
void CDataInfo::GetQryRate(std::vector<RateKey>& vec,bool IsAll)
{
	if(IsAll)
		vec = m_vecAllRate;
	else
		vec = m_vecVacancyRate;

}
void CDataInfo::SetTrade(std::map<TradeKey,PlatformStru_TradeInfo>& lMapFiled )
{
	m_mapTrade = lMapFiled;
}
void CDataInfo::ClearTrade()
{
	m_mapTrade.clear();
}
void CDataInfo::ModifyTrade(PlatformStru_TradeInfo&  originaldata, PlatformStru_TradeInfo&  newdata)
{

	TradeKey keyoriginal(originaldata);
	TradeKey keynew(newdata);
	m_mapTrade.erase(keyoriginal);
	m_mapTrade[keynew] = newdata;
}
void CDataInfo::AddTrade(PlatformStru_TradeInfo& field)
{
	TradeKey key(field);
	m_mapTrade[key] = field;
}
void CDataInfo::DeleteTrade(PlatformStru_TradeInfo& field)
{
	TradeKey key(field);
	m_mapTrade.erase(key);
}
void CDataInfo::SaveSortedTrade(std::vector<PlatformStru_TradeInfo>& vecTrade)
{

	m_vecSortedTrade.clear();
	m_vecSortedTrade=vecTrade;
}
void CDataInfo::GetSortedTrade(std::vector<PlatformStru_TradeInfo>& vecTrade)
{


	vecTrade=m_vecSortedTrade;
}
void CDataInfo::SaveSortedPositionDetail(std::vector<PlatformStru_PositionDetail>& vecPD)
{
	m_vecSortedPositionDetail.clear();
	m_vecSortedPositionDetail=vecPD;
}
void CDataInfo::GetSortedPositionDetail(std::vector<PlatformStru_PositionDetail>& vecPD)
{


	vecPD=m_vecSortedPositionDetail;
}
void CDataInfo::GetAllProducts(set<string>& setProductID )
{

	std::map<std::string,PlatformStru_InstrumentInfo>::iterator it = m_mapInstrumentInfo.begin();
	for ( it; it != m_mapInstrumentInfo.end(); it++ )
	{		
		setProductID.insert(it->second.ProductID);

	}
}
bool CDataInfo::GetInstrumentFieldByProductID(string strID, vector<PlatformStru_InstrumentInfo>& setInstrumentField)
{
	std::map<std::string,PlatformStru_InstrumentInfo>::iterator it = m_mapInstrumentInfo.begin();
	for ( it; it != m_mapInstrumentInfo.end(); it++ )
	{		
		if (strcmp(it->second.ProductID,strID.c_str()) == 0)
		{
			setInstrumentField.push_back(it->second);
		}
	}
	return true;
}

void CDataInfo::GetTrade(std::vector<PlatformStru_TradeInfo> &vecTrade, const std::string& UserID)
{
	if (m_mapTrade.empty())
	{
		return;
	}
	if ( UserID != SELECT_ALL )
	{
		
		std::map<TradeKey,PlatformStru_TradeInfo>::iterator it = m_mapTrade.begin();
		for(it; it!=m_mapTrade.end(); it ++)
		{
			if (strcmp(it->second.InvestorID,UserID.c_str()) == 0)
			{
				vecTrade.push_back(it->second);
			}
			
			
		}	

	}
	else if( UserID == SELECT_ALL)
	{

		std::map<TradeKey,PlatformStru_TradeInfo>::iterator it = m_mapTrade.begin();
		for ( it; it!=m_mapTrade.end();it++ )
		{

			vecTrade.push_back(it->second);

		}
	}
	
}
const char* CDataInfo::TradeDirection2String(char param)
{

	const static char* Buy="买";
	const static char* Sell="卖";
	switch(param)
	{
	case THOST_FTDC_D_Buy:return Buy;
	case THOST_FTDC_D_Sell:return Sell;
	default: return __UNKOWN__;
	}
}
const char CDataInfo::TradeDirection2ID( const string& name)
{

	const static std::string Buy="买";
	const static std::string Sell="卖";


	if(name==Buy)
		return THOST_FTDC_D_Buy;
	if(name==Sell)
		return THOST_FTDC_D_Sell;
	return ' ';
}
/////成交类型
//const char* CDataInfo::TradeType2String(char TradeType)
//{
//	const static char* Common="普通成交";
//	const static char* OptionsExecution="期权执行";
//	const static char* OTC="OTC成交";
//	const static char* EFPDerived="期转现衍生成交";
//	const static char* CombinationDerived="组合衍生成交";
//
//	switch(TradeType)
//	{
//	case THOST_FTDC_TRDT_Common:			return Common;
//	case THOST_FTDC_TRDT_OptionsExecution:	return OptionsExecution;
//	case THOST_FTDC_TRDT_OTC:  				return OTC;
//	case THOST_FTDC_TRDT_EFPDerived:		return EFPDerived;
//	case THOST_FTDC_TRDT_CombinationDerived:return CombinationDerived;
//	case 0x00:								return Common;
//	default: return __UNKOWN__;
//	}
//}
const char CDataInfo::TradeType2ID(const string& name)
{
	const static char* Common="普通成交";
	const static char* OptionsExecution="期权执行";
	const static char* OTC="OTC成交";
	const static char* EFPDerived="期转现衍生成交";
	const static char* CombinationDerived="组合衍生成交";

	if(name==Common)
		return THOST_FTDC_TRDT_Common;
	if(name==OptionsExecution)
		return THOST_FTDC_TRDT_OptionsExecution;
	if(name==OTC)
		return THOST_FTDC_TRDT_OTC;
	if(name==EFPDerived)
		return THOST_FTDC_TRDT_EFPDerived;
	if(name==CombinationDerived)
		return THOST_FTDC_TRDT_CombinationDerived;
	return ' ';

	
}
const char* CDataInfo::Direction2String(char param)
{
	const static char* Net="净";
	const static char* Buy="多头";
	const static char* Sell="空头";
	switch(param)
	{
	case THOST_FTDC_PD_Net:return Net;
	case THOST_FTDC_PD_Long:return Buy;
	case THOST_FTDC_PD_Short:return Sell;
	default: return __UNKOWN__;
	}
}
const char CDataInfo::Direction2ID( const string& name)
{
	const static std::string Net="净";
	const static std::string Buy="多头";
	const static std::string Sell="空头";


	if(name==Net)
		return THOST_FTDC_PD_Net;
	if(name==Buy)
		return THOST_FTDC_PD_Long;
	if(name==Sell)
		return THOST_FTDC_PD_Short;
	return ' ';
}
///开平标志
const char* CDataInfo::OffsetFlag2String(char param)
{
	const static char* Open="开仓";
	const static char* Close="平仓";
	const static char* ForceClose="强平";
	const static char* CloseToday="平今";
	const static char* CloseYesterday="平昨";
	switch(param)
	{
	case THOST_FTDC_OF_Open:		return Open;
	case THOST_FTDC_OF_Close:		return Close;
	case THOST_FTDC_OF_ForceClose:	return ForceClose;
	case THOST_FTDC_OF_CloseToday:	return CloseToday;
	case THOST_FTDC_OF_CloseYesterday:return CloseYesterday;
	default: return __UNKOWN__;
	}
}
const char CDataInfo::OffsetFlag2ID(const string& name)
{
	const static char* Open="开仓";
	const static char* Close="平仓";
	const static char* ForceClose="强平";
	const static char* CloseToday="平今";
	const static char* CloseYesterday="平昨";
	if(name==Open)
		return THOST_FTDC_OF_Open;
	if(name==Close)
		return THOST_FTDC_OF_Close;
	if(name==ForceClose)
		return THOST_FTDC_OF_ForceClose;
	if(name==CloseToday)
		return THOST_FTDC_OF_CloseToday;
	if(name==CloseYesterday)
		return THOST_FTDC_OF_CloseYesterday;
	return ' ';

	
}
///投机套保标志
const char* CDataInfo::HedgeFlag2String(char param)
{
	const static char* Speculation="投机";
	const static char* Arbitrage="套利";
	const static char* Hedge="套保";
	switch(param)
	{
	case THOST_FTDC_HF_Speculation:return Speculation;
	case THOST_FTDC_HF_Arbitrage:return Arbitrage;
	case THOST_FTDC_HF_Hedge:return Hedge;
	default: return __UNKOWN__;
	}
}
const char CDataInfo::HedgeFlag2ID( const string& name)
{
	const static std::string Speculation="投机";
	const static std::string Arbitrage="套利";
	const static std::string Hedge="套保";


	if(name==Speculation)
		return THOST_FTDC_HF_Speculation;
	if(name==Arbitrage)
		return THOST_FTDC_HF_Arbitrage;
	if(name==Hedge)
		return THOST_FTDC_HF_Hedge;
	return ' ';
}
///成交类型
const char* CDataInfo::TradeType2String(char TradeType)
{
	const static char* Common="普通成交";
	const static char* OptionsExecution="期权执行";
	const static char* OTC="OTC成交";
	const static char* EFPDerived="期转现衍生成交";
	const static char* CombinationDerived="组合衍生成交";

	switch(TradeType)
	{
	case THOST_FTDC_TRDT_Common:			return Common;
	case THOST_FTDC_TRDT_OptionsExecution:	return OptionsExecution;
	case THOST_FTDC_TRDT_OTC:  				return OTC;
	case THOST_FTDC_TRDT_EFPDerived:		return EFPDerived;
	case THOST_FTDC_TRDT_CombinationDerived:return CombinationDerived;
	case 0x00:								return Common;
	default: return __UNKOWN__;
	}
}

void CDataInfo::SetPositionDetail(std::multimap<std::string,PlatformStru_PositionDetail>& lMapFiled )
{
	m_mapPositionDetail = lMapFiled;
}
void CDataInfo::ClearPositionDetail()
{
	m_mapPositionDetail.clear();
}
void CDataInfo::SetPosition(std::multimap<std::string,PlatformStru_Position>& lMapFiled )
{
	m_mapPosition = lMapFiled;
}
void CDataInfo::ClearPosition()
{
	m_mapPosition.clear();
}
void CDataInfo::GetPositionDetail(std::vector<PlatformStru_PositionDetail>& vecField )
{
	std::multimap<std::string,PlatformStru_PositionDetail>::iterator it = m_mapPositionDetail.begin();
	for ( it; it!=m_mapPositionDetail.end();it++ )
	{
		vecField.push_back(it->second);
	}

}
void CDataInfo::GetPosition(std::vector<PlatformStru_Position>& vecField )
{
	std::multimap<std::string,PlatformStru_Position>::iterator it = m_mapPosition.begin();
	for ( it; it!=m_mapPosition.end();it++ )
	{
		vecField.push_back(it->second);
	}
}
void CDataInfo::SetFund(std::map<std::string,PlatformStru_TradingAccountInfo>& lMapFiled )
{
	m_mapFund = lMapFiled;
}
void CDataInfo::SetFund(PlatformStru_TradingAccountInfo& Filed )
{
	m_mapFund[Filed.AccountID] =Filed;
}
void CDataInfo::ClearFund()
{
	m_mapFund.clear();
}
void CDataInfo::GetFund(std::vector<PlatformStru_TradingAccountInfo>& vecField ,const std::string& UserID )
{
	if (m_mapFund.empty())
	{
		return;
	}
	if ( UserID != SELECT_ALL )
	{

		std::map<std::string,PlatformStru_TradingAccountInfo>::iterator it = m_mapFund.find(UserID);
		if( it!=m_mapFund.end())
		{
			
			vecField.push_back(it->second);
			
		}
	}
	else
	{
		std::map<std::string,PlatformStru_TradingAccountInfo>::iterator it = m_mapFund.begin();
		for ( it; it!=m_mapFund.end();it++ )
		{
			vecField.push_back(it->second);
		}
	}
	
}
bool CDataInfo::GetFund( PlatformStru_TradingAccountInfo& Filed,const std::string& UserID)
{
	std::map<std::string,PlatformStru_TradingAccountInfo>::iterator it = m_mapFund.find(UserID);
	if( it!=m_mapFund.end())
	{

		Filed=it->second;
		return true;

	}
	return false;

}

void CDataInfo::SetTradeDataStatus(eTradeDataStatus tradeDataStatus)
{
    m_tradeDataStatus = tradeDataStatus;
}
eTradeDataStatus CDataInfo::GetTradeDataStatus()
{
    return m_tradeDataStatus;
}

void CDataInfo::SetModifiedFund(PlatformStru_TradingAccountInfo& Field)
{

	m_mapModifyFund[Field.AccountID]= Field;
}
void CDataInfo::GteModifiedFund(std::map<std::string,PlatformStru_TradingAccountInfo>& lMapFiled )
{
    lMapFiled=m_mapModifyFund;
}
void CDataInfo::ClearModifiedFund()
{
	m_mapModifyFund.clear();
}
bool CDataInfo::IsExitIns(const string& Ins)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrumentInfo.find(Ins);
	if (it != m_mapInstrumentInfo.end())
	{
		return true;
	}
	else
		return false;
}
bool CDataInfo::IsExitTradeID(TradeKey& key)
{
	std::map<TradeKey,PlatformStru_TradeInfo>::iterator it = m_mapTrade.find(key);
	if (it != m_mapTrade.end())
	{
		return true;
	}
	else
		return false;
}
bool CDataInfo::GetInstrunmentInfobyID(const string& Ins, PlatformStru_InstrumentInfo& field)
{

	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrumentInfo.find(Ins);
	if (it != m_mapInstrumentInfo.end())
	{
		field = it->second;
		return true;
	}
	else
		return false;
}

bool CDataInfo::GetTradeAccountByUserID(std::string& strUserID, std::string& strBrokerID, std::string& strInvestID)
{
	//根据交易员账号得到委托交易账号
	UserInfo sUserInfo;

	GetUserInfoByAccount(strUserID,sUserInfo);
	int nUserID = sUserInfo.nUserID;
	TradeAccount account;
	std::map<int, UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(nUserID);
	if (it!=m_mapUserAndTradeAccountRelation.end())
	{
		strInvestID = GetTradeAccountByID( it->second.nTradeAccountID);
	}

	GetTradeAccountByAccount(strInvestID,account);
	strBrokerID = GetBrokerCodeByID(account.nBrokerID);
		
	return true;
}
bool CDataInfo::GetRateByKey(RateKey& key,CommissionMarginRate& rate)
{
	std::map<RateKey,CommissionMarginRate>::iterator it= m_mapCommissionMarginRate.find(key);
		if (it!= m_mapCommissionMarginRate.end())
		{
            rate=it->second;
			return true;
		}
		return false;
}
bool CDataInfo::GetPositionDetail(const string& userID,const string& traderID,PlatformStru_PositionDetail& field)
{

	int ncount = m_mapPositionDetail.count(userID);
	multimap<string, PlatformStru_PositionDetail>::iterator it = m_mapPositionDetail.find(userID);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{		
		if (it->second.TradeID == traderID)
		{
			field = it->second;
			return true;
		}
	}
	return false;
}
void CDataInfo::SetPositionDetail(const PlatformStru_PositionDetail *pField)
{
	if(pField == NULL)
		return;
	m_mapPositionDetail.insert(make_pair(pField->InvestorID,*pField));
}
//void CDataInfo::ModifyPositionDetail(const PlatformStru_PositionDetail *pField)
//{
//	if(pField == NULL)
//		return;
//	m_mapPositionDetail.insert(make_pair(pField->InvestorID,*pField));;
//}
bool CDataInfo::GetTradeInfo(TradeKey& key, PlatformStru_TradeInfo& field)
{

	std::map<TradeKey,PlatformStru_TradeInfo>::iterator it = m_mapTrade.find(key);
	if (it != m_mapTrade.end())
	{
		field = it->second;
		return true;
	}
	else
		return false;
}
void CDataInfo::SetTradeInfo(const PlatformStru_TradeInfo *pField)
{
	if(pField == NULL)
		return;
	TradeKey key(*pField);
	m_mapTrade.insert(make_pair(key,*pField));
}