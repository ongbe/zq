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
	m_mapAssetMgmtOrgTree.clear();
    m_mapOrganization.clear();	
	
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
    if(m_mapAssetMgmtOrgTree.find(pOrganization->nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
    {
        std::map<int, Organization> tempOrgMap;
        m_mapAssetMgmtOrgTree[pOrganization->nAssetMgmtOrgID] = tempOrgMap;
    }

    std::map<int, map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(pOrganization->nUpperLevelOrgID);
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

    return -1;
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

void CDataInfo::SetUserInfo(int nUserID, bool bAssign)
{
    std::map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if( it != m_mapUserInfo.end())
        it->second.nAssign = bAssign;
}
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


void CDataInfo::ClearUserAndOrgRelation()
{
    m_vecUserAndOrgRelation.clear();
}

void CDataInfo::DelUserAndOrgRelation(int nID, int nUserType, int nRelationType)
{
    std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    while (it!=m_vecUserAndOrgRelation.end())
    {
            if ( USER_TYPE_RISK_CONTROL == nUserType )//风控员
            {
				if ( 0 == nRelationType && it->nUserID == nID ) {
					DelDealerOrganRelation(it->nUserID, it->nOrgID);
                    it = m_vecUserAndOrgRelation.erase(it);
				}
                else
                    ++it;
            }
            else if ( USER_TYPE_TRADER == nUserType )
            {
				if ( 0 == nRelationType && it->nOrgID == nID ) {
					DelDealerOrganRelation(it->nUserID, it->nOrgID);
                    it = m_vecUserAndOrgRelation.erase(it);
				}
                else
                    ++it;
            }
            else
                ++it;
    }
}

void CDataInfo::ModifyUserAndOrgRelation(const UserAndOrgRelation& oRelation)
{
    bool bFind = false;
    std::vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    while (it!=m_vecUserAndOrgRelation.end())
    {
        if ( it->nUserID == oRelation.nUserID && 
             it->nRelationType == oRelation.nRelationType && 
             it->nUserType == oRelation.nUserType )
        {
            bFind = true;
            it->nOrgID = oRelation.nOrgID;
 			DelDealerOrganRelation(it->nUserID, it->nOrgID);
            break;
        }
        else
            ++it;
    }

	if(!bFind) {
        m_vecUserAndOrgRelation.push_back(oRelation);
		ResetDealerOrganRelation(&oRelation);
	}

    //设置分配属性
    SetUserInfo(oRelation.nUserID, true);
}

void CDataInfo::SetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation)
{
    m_vecUserAndOrgRelation = vecUserAndOrgRelation;
	ResetDealerOrganRelationMap();
}

void CDataInfo::SetUserAndOrgRelation(const UserAndOrgRelation* pRelation)
{
    SetUserInfo(pRelation->nUserID, true);
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
	for(int i=0; i<m_vecUserAndOrgRelation.size(); i++) {
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
	if(it == m_mapDealerOrganRelation.end()) {
		it->second.erase(nDealerID);
	}
}

bool CDataInfo::GetDealerByOrganID(int nOrganID, set<int>& setDealerID)
{
	std::map<int, set<int>>::iterator it;

	it = m_mapDealerOrganRelation.find(nOrganID);
	if(it != m_mapDealerOrganRelation.end()) {
		setDealerID = it->second;
		return true;
	}
	else
		return false;
}


int CDataInfo::GetTraderUserIDsByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, std::set<int>& setFPID)
{
	
	std::map<int, std::set<int>>::iterator it2 = m_mapDealerOrganRelation.find(nAssetMgmtOrg);
	if(it2 != m_mapDealerOrganRelation.end() )
	{
		std::set<int>::iterator lIter = it2->second.begin();
		for(;lIter != it2->second.end();lIter++)
			setFPID.insert(*lIter);
	}
	if(!bIncludeChildren)//不包含子节点
		return setFPID.size();

	std::map<int, map<int, Organization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return setFPID.size();

	map<int, Organization>& mapAMO  = it->second; 
	for(map<int, Organization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetTraderUserIDsByOrgID(key,  bIncludeChildren, setFPID);//循环遍历子节点
	}
	return setFPID.size();
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

void CDataInfo::GetLogonInfo(sLoginRsp& loginRsp)
{
    memcpy(&loginRsp,&m_loginRsp,sizeof(sLoginRsp));
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
void CDataInfo::GetTraderSetByProductID(int nProductID,std::vector<int>& vec)
{
	vec.clear();


	std::map<int,std::vector<TraderProRelation>>::iterator it = m_mapTraderProRelation.find(nProductID);
	if(it != m_mapTraderProRelation.end())
	{
		std::vector<TraderProRelation>::iterator lIter = it->second.begin();
		for(;lIter != it->second.end();lIter++)
			vec.push_back(lIter->nTraderID);

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

void CDataInfo::GetAllProducts(set<string>& setProductID)
{
	MAPProductID2InstrumentField::iterator it = m_mapProductID2InstrumentField.begin();
	for(;it!=m_mapProductID2InstrumentField.end();++it)
	{
		setProductID.insert(it->first);
	}
}

void CDataInfo::SetInstrumentField(PlatformStru_InstrumentInfo *pField)
{
	if(pField == NULL)
		return;
	m_mapInstrumentID2InstrumentField.insert(make_pair(pField->InstrumentID , *pField));
	m_mapProductID2InstrumentField.insert(make_pair(pField->ProductID , *pField));
}