#include "stdafx.h"
#include "CDataInfo.h"
#include "CommonDef.h"

CDataInfo* CDataInfo::m_pInstance = NULL;
int g_Count = 0;
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
	m_mapAssetMgmtOrgTree.clear();
    m_mapAssetMgmtOrg.clear();
}

CDataInfo::CDataInfo()
{
    Clear();
}

CDataInfo::~CDataInfo()
{
}

//得到资管组织
void CDataInfo::GetAssetMgmtOrg(std::map<int, std::map<int, Organization>>&	mapAssetMgmtOrgTree)
{
	mapAssetMgmtOrgTree = m_mapAssetMgmtOrgTree;
}
bool CDataInfo::GetAssetMgmtOrgByID(int nAssetMgmtOrgID, Organization& sOrg)
{
	std::map<int, Organization>::iterator it = m_mapAssetMgmtOrg.find(nAssetMgmtOrgID);
	if(it == m_mapAssetMgmtOrg.end())
		return false;

	sOrg = it->second;
	return true;
}
int CDataInfo::GetAssetMgmtOrgIDByName(std::string& sName)
{
    std::map<int, Organization>::iterator it = m_mapAssetMgmtOrg.begin();
    for(;it != m_mapAssetMgmtOrg.end();it++)
    {
        if (strcmp(it->second.szName,sName.c_str()) == 0)
        {
            return it->first;
        }
    }

    return -1;
}

string  CDataInfo::GetAssetMgmtOrgNameByID(int nOrgID)
{
    string strName;
    std::map<int, Organization>::iterator it = m_mapAssetMgmtOrg.find(nOrgID);    
    if(it != m_mapAssetMgmtOrg.end())
        strName = (string)(it->second.szName);
    else 
        strName = " ";

    return strName;
}

void CDataInfo::GetUserInfo(std::map<int, UserInfo>& mapUserInfo)
{
    mapUserInfo = m_mapUserInfo;
}

bool CDataInfo::GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo)
{
    std::map<int, UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if(it==m_mapUserInfo.end())
       return false;
    
    sUserInfo = it->second;
    return true;
}

//角色
void CDataInfo::ClearRole()
{
    m_mapRoleInfo.clear();
}
void CDataInfo::SetRole(std::map<int, RoleInfo>& mapRole)
{
    m_mapRoleInfo = m_mapRoleInfo;
}

void CDataInfo::GetRole(std::map<int, RoleInfo>& mapRole)
{
    mapRole = m_mapRoleInfo;
}

string CDataInfo::GetRoleNameByID(int nRoleID)
{
    string strName(" ");
    std::map<int,RoleInfo>::iterator it = m_mapRoleInfo.find(nRoleID);
    if ( it != m_mapRoleInfo.end())
        strName = it->second.szName;
    
    return strName;
}

//用户与角色关系
void CDataInfo::ClearUserRoleRelation()
{
    m_mapUserRole.clear();
}
void CDataInfo::SetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole)
{
    m_mapUserRole = mapUserRole;
}

void CDataInfo::GetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole)
{
    mapUserRole = m_mapUserRole;
}

void CDataInfo::GetRoleByUserID(int nUserID,std::vector<UserRole>& vecRole)
{
    std::map<int ,std::vector<UserRole>>::iterator it = m_mapUserRole.find(nUserID);
    if (it != m_mapUserRole.end())    
        vecRole = it->second; 
}

void CDataInfo::SetAeestMgmtOrg(std::map<int, Organization>&	mapAssetMgmtOrg)
{
	std::map<int, Organization>::iterator it_org = mapAssetMgmtOrg.begin();
	for ( ; it_org != mapAssetMgmtOrg.end(); it_org++ )
	{
		Organization info = it_org->second;
		//////////////////////////////////////////////////////////////////////////
		//自身要出现在树里面，没有子节点的叶节点也要出现在树中
		if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
		{
			std::map<int, Organization> tempOrgMap;
			tempOrgMap.clear();
			m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
		}
		//////////////////////////////////////////////////////////////////////////
		std::map<int, map<int, Organization>>::iterator it = 
			m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
		if ( it != m_mapAssetMgmtOrgTree.end() )
		{//找到父节点
			it->second.insert(make_pair(info.nAssetMgmtOrgID, info));
		}
		else
		{
			std::map<int, Organization> tempOrgMap;
			tempOrgMap.clear();
			tempOrgMap[info.nAssetMgmtOrgID] = info;
			m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
		}	
	}

	m_mapAssetMgmtOrg = mapAssetMgmtOrg;
}

void CDataInfo::ClearUserInfo()
{
    m_mapUserInfo.clear();
}

void CDataInfo::SetUserInfo(std::map<int, UserInfo>& mapUserInfo)
{
    m_mapUserInfo = mapUserInfo;
}

void CDataInfo::ClearUserAndOrgRelation()
{
    m_mapUserAndOrgRelation.clear();
}

void CDataInfo::SetUserAndOrgRelation(std::map<int, UserAndOrgRelation>& mapUserAndOrgRelation)
{
    m_mapUserAndOrgRelation = mapUserAndOrgRelation;
}

void CDataInfo::GetUserAndOrgRelation(std::map<int, UserAndOrgRelation>& mapUserAndOrgRelation)
{
	mapUserAndOrgRelation = m_mapUserAndOrgRelation;
}

void CDataInfo::SetPrivilegeNodes( std::map<int, PrivilegeType>& mapPrivilegeNode )
{
	m_mapNode = mapPrivilegeNode;
}

void CDataInfo::GetPrivilegeNodes(std::map<int, PrivilegeType>& mapPrivilegeNode)
{
	mapPrivilegeNode = m_mapNode;
}

void CDataInfo::SetLogonInfo( const sLoginRsp& loginRsp )
{
	m_loginRsp = loginRsp;
}

void CDataInfo::GetLogonInfo(sLoginRsp& loginRsp)
{
    memcpy(&loginRsp,&m_loginRsp,sizeof(sLoginRsp));
}

void CDataInfo::SetPrivilegeActions( std::map<int, PrivilegeAction>& mapPrivilegeAction )
{
	m_mapAction = mapPrivilegeAction;
}

void CDataInfo::GetPrivilegeActions( std::map<int, PrivilegeAction>& mapPrivilegeAction )
{
	mapPrivilegeAction = m_mapAction;
}

void CDataInfo::SetPrivilegeNames( std::map<int, RoleInfo>& mapPrivilegeName )
{
	m_mapPrivilegeName = mapPrivilegeName;
}

void CDataInfo::GetPrivilegeNames( std::map<int, RoleInfo>& mapPrivilegeName )
{
	mapPrivilegeName = m_mapPrivilegeName;
}

void CDataInfo::SetAllPrivilegeValues( std::map<int, std::map<int, Privilege>>& allPrivilegeValue )
{
	m_mapAllPrivilegeValue = allPrivilegeValue;
}

void CDataInfo::GetPrivilegeValues(int nPrivilegeID, std::map<int, Privilege>& mapValues)
{
	std::map<int, std::map<int, Privilege>>::iterator it_permission = m_mapAllPrivilegeValue.find(nPrivilegeID);
	if ( it_permission != m_mapAllPrivilegeValue.end())
	{
		mapValues = it_permission->second;
	}
}

void CDataInfo::SetPrivilegeValues(int nPrivilegeID, std::map<int, Privilege>& mapValues)
{
	m_mapAllPrivilegeValue[nPrivilegeID] = mapValues;
}

void CDataInfo::SetPrivilegeName( RoleInfo& name )
{
	m_mapPrivilegeName[name.nRoleID] = name;
}

void CDataInfo::DelPrivilegeName(int nPrivilegeID)
{
	std::map<int, RoleInfo>::iterator it = m_mapPrivilegeName.find(nPrivilegeID);
	if ( it != m_mapPrivilegeName.end())
	{
		m_mapPrivilegeName.erase(it);
	}

	std::map<int, std::map<int, Privilege>>::iterator it_value
		= m_mapAllPrivilegeValue.find(nPrivilegeID);
	if ( it_value != m_mapAllPrivilegeValue.end() )
	{
		m_mapAllPrivilegeValue.erase(it_value);
	}
}

void CDataInfo::SetCurPrivilegeName( RoleInfo& name )
{
	m_curPrivilegeName = name;
}

RoleInfo CDataInfo::GetCurPrivilegeName()
{
	return m_curPrivilegeName;
}

void CDataInfo::SetCurPrivilegeValues( int nUserType, std::map<int, Privilege>& mapValues )
{
	m_nCurUserType = nUserType;
	m_mapCurPrivilegeValue = mapValues;
}

void CDataInfo::GetCurPrivilegeValues( int& nUserType, std::map<int, Privilege>& mapValues )
{
	nUserType = m_nCurUserType;
	mapValues = m_mapCurPrivilegeValue;
}

void CDataInfo::UpdatePrivilegeNameType( int nPrivilegeID, int nUserType )
{
	std::map<int, RoleInfo>::iterator it = m_mapPrivilegeName.find(nPrivilegeID);
	if ( it != m_mapPrivilegeName.end())
	{
		it->second.nUserType = nUserType;
	}
}

void CDataInfo::SetBrokerInfos( std::map<int,BrokerInfo>& mapBrokerInfo )
{
	m_mapBrokerInfo = mapBrokerInfo;
}

void CDataInfo::GetBrokerInfos( std::map<int,BrokerInfo>& mapBrokerInfo )
{
	mapBrokerInfo = m_mapBrokerInfo;
}

bool CDataInfo::GetBrokerIDByName(std::string szName,int& brokerID)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
    for(;it != m_mapBrokerInfo.end();it++)
    {
        if(0 == ::strcmp(szName.c_str(),it->second.szCompany))
        {
            brokerID = it->second.nBrokerID;
            return true;
        }
    }

    return false;
}

bool CDataInfo::GetBrokerNameByID(int brokerID,std::string& szName)
{
    std::map<int,BrokerInfo>::iterator it = m_mapBrokerInfo.find(brokerID);
    if(it != m_mapBrokerInfo.end())
    {
        szName = it->second.szCompany;
        return true;
    }

    return false;  
}
void CDataInfo::SetServerGroupInfo(std::map<int,ServerGroup>& mapServerGroupInfo)
{
    m_mapServerGroupInfo = mapServerGroupInfo;
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

bool CDataInfo::GetServerGroupNameByGroupID(int nServerGroupID,std::string& szName)
{
    std::map<int, ServerGroup>::iterator it = m_mapServerGroupInfo.find(nServerGroupID);
    if ( it != m_mapServerGroupInfo.end())
    {
        szName = it->second.szServerName;
        return true;
    }

    return false;
}

bool CDataInfo::GetServerGroupIDByBrokerIDAndServerName(int nBrokerID,std::string szName,int& nServerGroupID)
{
    std::map<int,ServerGroup> mapServerGroup;
    GetServerGroups(nBrokerID,mapServerGroup);
    if(mapServerGroup.empty())
        return false;

    std::map<int,ServerGroup>::iterator it = mapServerGroup.begin();
    for(;it != mapServerGroup.end();it++)
    {
        if(0 == ::strcmp(szName.c_str(),it->second.szServerName))
        {
            nServerGroupID = it->first;
            return true;
        }
    }

    return false;
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

void CDataInfo::SetBrokerInfo( BrokerInfo& brokerInfo )
{
	m_mapBrokerInfo[brokerInfo.nBrokerID] = brokerInfo;
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

void CDataInfo::GetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation)
{
    mapUserAndOrgRelation = m_mapUserAndTradeAccountRelation;
}


//理财产品
int CDataInfo::GetNextSeqNum(MODULE_NAME eName)
{
    int nSeqNum = 0;
    if(eName == FINANCE_PRODUCT && !m_mapFinancialProduct.empty())    
    {
        std::map<int,FinancialProduct>::iterator it = m_mapFinancialProduct.end();
        it--;

        nSeqNum = it->second.nID;
    }else if (eName == TRADE_STRATEGY && !m_mapTradeStrategy.empty())
    {
        std::map<int,TradeStrategy>::iterator it = m_mapTradeStrategy.end();
        it--;

        nSeqNum = it->second.nID;
    }else if (eName == TRADER_ACCOUNT && !m_mapTradeAccount.empty())
    {
        std::map<int,TradeAccount>::iterator it = m_mapTradeAccount.end();
        it--;

        nSeqNum = it->second.nTradeAccountID;
    }

    return ++nSeqNum;
}

void CDataInfo::ClearDelFinanceProduct()
{
    m_setDelFinanceProduct.clear();
}

void CDataInfo::GetDelFinanceProduct(std::set<int>& setDel)
{
    setDel = m_setDelFinanceProduct;
}

void CDataInfo::SetDelFinancialProduct(int nProductID)
{
    m_setDelFinanceProduct.insert(nProductID);
}

void CDataInfo::SetFinancialProductStatus()
{
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.begin();
    for (; it!=m_mapFinancialProduct.end(); it++)
        it->second.nDBOprType = 0;    

    std::map<Key,TraderProRelation>::iterator itRela = m_mapTraderProRelation.begin();
    for (; itRela != m_mapTraderProRelation.end(); itRela++)    
        itRela->second.nDBOprType = 0;    
}

void CDataInfo::ClearFinanceProuct()
{
    m_mapFinancialProduct.clear();
}

void CDataInfo::SetFinancialProduct(const FinancialProduct *pFP)
{
    if(pFP == NULL)
        return;
    m_mapFinancialProduct[pFP->nID] = *pFP;
}

void CDataInfo::AddFinancialProduct(const FinancialProduct *pFP)
{
    SetFinancialProduct(pFP);
}

void CDataInfo::ModifyFinancialProduct(const FinancialProduct *pFP)
{
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.find(pFP->nID);
    if (it!=m_mapFinancialProduct.end())
    {
        if (it->second.nDBOprType == 0)
            memcpy(&(it->second),pFP,sizeof(FinancialProduct));
        else
        {
            strcpy(it->second.szName,pFP->szName);
            strcpy(it->second.szDesc,pFP->szDesc);
        }
    }
}

void CDataInfo::ClearProductTraderRelation()
{
    m_mapTraderProRelation.clear();
}

void CDataInfo::SetProductTraderRelation(const TraderProRelation *pRelate)
{
    if(pRelate == NULL)
        return;
   
    m_mapTraderProRelation[Key(pRelate->nFinancialProductID,pRelate->nTraderID)] = *pRelate;
}

void CDataInfo::AddProductTraderRelation(const TraderProRelation *pRelate)
{
     SetProductTraderRelation(pRelate);
}

void CDataInfo::DeleteProductTraderRelation(int nProductID,bool bReal)
{
    std::map<Key,TraderProRelation>::iterator it = m_mapTraderProRelation.begin();
    for (; it != m_mapTraderProRelation.end(); )
    {
        Key key = it->first;
        if (key.first == nProductID)
        {
            if(bReal)
                it = m_mapTraderProRelation.erase(it);
            else
            {
                it->second.nDBOprType = Cmd_RM_DeleteProductTraderRelation_Req;
                it++;
            }
        }
        else
            it++;
    }

    /*
    std::vector<TraderProRelation>::iterator itRela = m_vecTraderProRelation.begin();
    for (; itRela != m_vecTraderProRelation.end();)
    {
        if (nProductID == itRela->nFinancialProductID)
        {
            if(bReal)
                itRela = m_vecTraderProRelation.erase(itRela);
            else
                itRela->nDBOprType = Cmd_RM_DeleteProductTraderRelation_Req;
        }
        else
            itRela++;
    }*/
}

void CDataInfo::DeleteProductTraderRelation(const TraderProRelation *pRelate,bool bReal)
{
    std::map<Key,TraderProRelation>::iterator itRela = m_mapTraderProRelation.begin();
    for (; itRela != m_mapTraderProRelation.end(); itRela++)
    {
        if (pRelate->nFinancialProductID == itRela->first.first &&
            pRelate->nTraderID == itRela->first.second )
        {      
            if(bReal || itRela->second.nDBOprType == Cmd_RM_AddProductTraderRelation_Req)
                itRela = m_mapTraderProRelation.erase(itRela);
            else            
                itRela->second.nDBOprType = Cmd_RM_DeleteProductTraderRelation_Req;

            break;
        }
    }
}

void CDataInfo::GetFinancialProduct(std::map<int, FinancialProduct>&	mapFinancialProduct)
{
    mapFinancialProduct = m_mapFinancialProduct;
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

    return false;
}

void CDataInfo::GetProTraderRelation(std::vector<TraderProRelation>& vec)
{
    vec.clear();

    std::map<Key,TraderProRelation>::iterator it = m_mapTraderProRelation.begin();
    for (; it != m_mapTraderProRelation.end(); it++)
    {
        Key key = it->first;
        TraderProRelation oRela;
        memcpy(&oRela,&(it->second),sizeof(TraderProRelation));

        vec.push_back(oRela);
    }
}

bool CDataInfo::IsValidOfProductName(const string& sName)
{
    std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.begin();
    for (;it!=m_mapFinancialProduct.end();it++)
    {
        if (0 == strcmp(sName.c_str(),it->second.szName) && it->second.nDBOprType != Cmd_RM_DeleteFinicialProduct_Req)
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

void CDataInfo::SetOrgStrategyRelation(const AssetOrgStrategyRelation *pRelate)
{
    if(pRelate == NULL)
        return;

    m_vecOrgStrategyRelation.push_back(*pRelate);
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

void CDataInfo::ClearTradeStrategy()
{
    m_mapTradeStrategy.clear();
}

void CDataInfo::DeleteTradeStrategyByID(int& id)
{
    std::map<int, TradeStrategy>::iterator it = m_mapTradeStrategy.find(id);
    if (it!=m_mapTradeStrategy.end())
    {
        it = m_mapTradeStrategy.erase(it);
    }
}

void CDataInfo::ClearOrgStrategyRelation()
{
    m_vecOrgStrategyRelation.clear();
}

void CDataInfo::GetOrgStrategyRelation(std::vector<AssetOrgStrategyRelation>& vec)
{
    vec = m_vecOrgStrategyRelation;
}

void CDataInfo::DeleteOrgStrategyRelationByID(int nStrategyID)
{    
    std::vector<AssetOrgStrategyRelation>::iterator it = m_vecOrgStrategyRelation.begin();
    for (;it!=m_vecOrgStrategyRelation.end();)
    {
        if (nStrategyID == it->nStrategyID)
        {
            it = m_vecOrgStrategyRelation.erase(it);
        }
        else
            it++;
    }    
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