#include "stdafx.h"

#include "CDataInfo.h"

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
	m_bAeestMgmtOrganization	= false; 
	m_bFinancialProduct			= false;
	m_bRiskIndicatorModule		= false;
	m_bRiskIndicator			= false;
	m_bTrustTradeAccount		= false;
	m_bTraderInfo				= false;
	m_bMgmtOrg2Financial		= false;
	m_bInstrumentField			= false;
	m_bPermissionValue			= false;
	m_bRiskMgmtUserInfo			= false;
	m_bRiskWarning				= false;
	m_bProductRiskWarning		= false;

	m_strUserName				= "";					
	m_strPwd					="";						
	m_loginRsp.nAccountID		= 0;
	m_loginRsp.nAssetMgmtOrgID	= 0;
	m_loginRsp.nPermissionID	= 0;

	m_mapAssetMgmtOrgTree.clear();
	m_mapFinancialProduct.clear();
	m_mapRiskIndicatorModule.clear();
	m_mapRiskIndicator.clear();
	m_mapTrustTradeAccount.clear();
	m_mapTraderInfo.clear();
	m_mulmapMgmtOrg2Financial.clear();
	m_mapAssetMgmtOrg.clear();
	m_multimapOrgID2TrustTradeAccount.clear();
	m_multimapFinancialID2TrustTradeAccount.clear();
}
bool CDataInfo::IsInit()
{
	return m_bAeestMgmtOrganization && m_bFinancialProduct
		&& m_bRiskIndicatorModule && m_bRiskIndicator && m_bTrustTradeAccount
		&& m_bTraderInfo && m_bMgmtOrg2Financial && m_bInstrumentField 
		&& m_bPermissionValue && m_bRiskMgmtUserInfo 
		&& m_bRiskWarning && m_bProductRiskWarning;
}
void CDataInfo::SetLoginRsp(LoginRsp	loginRsp)
{
	m_loginRsp = loginRsp;
}
void CDataInfo::GetLoginRsp(LoginRsp&	loginRsp)
{
	loginRsp= m_loginRsp;
}
void CDataInfo::SetPwd(string strPwd)
{
	m_strPwd = strPwd;
}
void CDataInfo::GetPwd(string& strPwd)
{
	strPwd = m_strPwd;
}

void CDataInfo::SetUserName(string strUserName)
{
	m_strUserName = strUserName;
}
void CDataInfo::GetUserName(string& strUserName)
{
	strUserName = m_strUserName;
}
//得到资管组织
void CDataInfo::GetAeestMgmtOrg(MAPAeestMgmtOrganization&	mapAssetMgmtOrgTree)
{
	mapAssetMgmtOrgTree = m_mapAssetMgmtOrgTree;
}
bool CDataInfo::GetAssetMgmtOrg(int nAssetMgmtOrgID, AssetMgmtOrganization& sOrg)
{
	MAPAssetMgmtOrg::iterator it = m_mapAssetMgmtOrg.find(nAssetMgmtOrgID);
	if(it == m_mapAssetMgmtOrg.end())
		return false;

	sOrg = it->second;
	return true;
}
//理财产品
void CDataInfo::GetFinancialProduct(MAPFinancialProduct&	mapFinancialProduct)
{
	mapFinancialProduct = m_mapFinancialProduct;
}

//风险指标模型结构
void CDataInfo::GetRiskIndicatorModule(MAPRiskIndicatorModule&	mapRiskIndicatorModule)
{
	mapRiskIndicatorModule = m_mapRiskIndicatorModule;
}
bool CDataInfo::GetRiskIndicatorModule(int nRiskIndicatorModuleID, RiskIndicatorModule&	sRiskIndicatorModule)
{
	MAPRiskIndicatorModule::iterator it = m_mapRiskIndicatorModule.find(nRiskIndicatorModuleID);
	if(it == m_mapRiskIndicatorModule.end())
		return false;

	sRiskIndicatorModule = it->second;
	return true;
}
//风险指标
void CDataInfo::GetRiskIndicator(MAPRiskIndicator&	mapRiskIndicator)
{
	mapRiskIndicator = m_mapRiskIndicator;
}

bool CDataInfo::GetRiskIndicator(int nRiskIndicator, RiskIndicator&	sRiskIndicator)
{
	MAPRiskIndicator::iterator it = m_mapRiskIndicator.find(nRiskIndicator);
	if(it == m_mapRiskIndicator.end())
		return false;

	sRiskIndicator = it->second;
	return true;
}
//委托交易账户
void CDataInfo::GetTrustTradeAccount(MAPTrustTradeAccount&	mapTrustTradeAccount)
{
	mapTrustTradeAccount = m_mapTrustTradeAccount;
}

//交易员信息
void CDataInfo::GetTraderInfo(MAPTraderInfo&	mapTraderInfo)
{
	mapTraderInfo = m_mapTraderInfo;
}

bool CDataInfo::GetTraderInfo(int nID, TraderInfo&	 sTraderInfo)
{
	MAPTraderInfo::iterator it = m_mapTraderInfo.find(nID);
	if(it == m_mapTraderInfo.end())
		return false;

	sTraderInfo = it->second;
	return true;
}

void CDataInfo::SetAeestMgmtOrg(MAPAssetMgmtOrg&	mapAssetMgmtOrg)
{
	std::map<int, AssetMgmtOrganization>::iterator it_org = mapAssetMgmtOrg.begin();
	for ( ; it_org != mapAssetMgmtOrg.end(); it_org++ )
	{
		AssetMgmtOrganization info = it_org->second;
		//////////////////////////////////////////////////////////////////////////
		//自身要出现在树里面，没有子节点的叶节点也要出现在树中
		if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
		{
			std::map<int, AssetMgmtOrganization> tempOrgMap;
			tempOrgMap.clear();
			m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
		}
		//////////////////////////////////////////////////////////////////////////
		std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
			m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
		if ( it != m_mapAssetMgmtOrgTree.end() )
		{//找到父节点
			it->second.insert(make_pair(info.nAssetMgmtOrgID, info));
		}
		else
		{
			std::map<int, AssetMgmtOrganization> tempOrgMap;
			tempOrgMap.clear();
			tempOrgMap[info.nAssetMgmtOrgID] = info;
			m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
		}	
	}
	m_mapAssetMgmtOrg = mapAssetMgmtOrg;
}
void CDataInfo::SetFinancialProduct(FinancialProduct *pFP)
{
	if(pFP == NULL)
		return;
	m_mapFinancialProduct[pFP->nFinancialProductID] = *pFP;
}
void CDataInfo::SetRiskIndicatorModule(RiskIndicatorModule *pRIM)
{
	if(pRIM == NULL)
		return;
	m_mapRiskIndicatorModule[pRIM->nRiskIndModuleID] = *pRIM;
}
void CDataInfo::SetRiskIndicator(RiskIndicator *pRI)
{
	if(pRI == NULL)
		return;	
	m_mapRiskIndicator[pRI->nRiskIndicatorID] = *pRI;
}
void CDataInfo::SetTrustTradeAccount(TrustTradeAccount *pRI)
{
	if(pRI == NULL)
		return;	
	m_mapTrustTradeAccount[pRI->nTradeAccountID] = *pRI;
	m_mapTrustTradeAccountByName[pRI->szAccount] = *pRI;
}
void CDataInfo::SetOrgID2TrustTradeAccount(TrustTradeAccount *pRI)
{
	if(pRI == NULL)
		return;	
	m_multimapOrgID2TrustTradeAccount.insert(make_pair(pRI->nAssetMgmtOrgID, *pRI));
}
void CDataInfo::SetFinancialID2TrustTradeAccount(TrustTradeAccount *pRI)
{
	if(pRI == NULL)
		return;	
	m_multimapFinancialID2TrustTradeAccount.insert(make_pair(pRI->nFinancialProductID, *pRI));
}
void CDataInfo::SetTraderInfo(TraderInfo *pRI)
{
	if(pRI == NULL)
		return;	
	m_mapTraderInfo[pRI->nTraderID] = *pRI;
}
void CDataInfo::SetMulmapMgmtOrg2Financial(AssetOrgProRelation *pRI)
{
	if(pRI == NULL)
		return;	
	m_mulmapMgmtOrg2Financial.insert(make_pair(pRI->nAssetMgmtOrgID, pRI->nFinancialProductID));
}
int CDataInfo::GetFinancialProduct(int  nAssetMgmtOrg, bool bIncludeChildren, set<int>& setFPID)
{	
	int ncount = m_mulmapMgmtOrg2Financial.count(nAssetMgmtOrg);
	multimap<int, int>::iterator it2 = m_mulmapMgmtOrg2Financial.find(nAssetMgmtOrg);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		setFPID.insert(it2->second);
		it2++;
	}
	if(!bIncludeChildren)//不包含子节点
		return setFPID.size();

	std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return setFPID.size();
	
	map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
	for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetFinancialProduct(key,  bIncludeChildren, setFPID);//循环遍历子节点
	}
	return setFPID.size();
}
int CDataInfo::GetTrustTradeAccountByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, set<int>& setFPID)
{
	int ncount = m_multimapOrgID2TrustTradeAccount.count(nAssetMgmtOrg);
	multimap<int, TrustTradeAccount>::iterator it2 = m_multimapOrgID2TrustTradeAccount.find(nAssetMgmtOrg);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		setFPID.insert(it2->second.nTradeAccountID);
		it2++;
	}
	if(!bIncludeChildren)//不包含子节点
		return setFPID.size();

	std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return setFPID.size();

	map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
	for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetTrustTradeAccountByOrgID(key,  bIncludeChildren, setFPID);//循环遍历子节点
	}
	return setFPID.size();
}
int CDataInfo::GetTrustTradeAccountByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, set<TrustTradeAccount>& setFPID)
{
	int ncount = m_multimapOrgID2TrustTradeAccount.count(nAssetMgmtOrg);
	multimap<int, TrustTradeAccount>::iterator it2 = m_multimapOrgID2TrustTradeAccount.find(nAssetMgmtOrg);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		TrustTradeAccount tr = it2->second;
		setFPID.insert(tr);
		it2++;
	}
	if(!bIncludeChildren)//不包含子节点
		return setFPID.size();

	std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return setFPID.size();

	map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
	for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetTrustTradeAccountByOrgID(key,  bIncludeChildren, setFPID);//循环遍历子节点
	}
	return setFPID.size();
}
int CDataInfo::GetTrustTradeAccountByFinancialID(int  nFinancialID, set<TrustTradeAccount>& setFPID)
{
	int ncount = m_multimapFinancialID2TrustTradeAccount.count(nFinancialID);
	multimap<int, TrustTradeAccount>::iterator it2 = m_multimapFinancialID2TrustTradeAccount.find(nFinancialID);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		setFPID.insert(it2->second);
		it2++;
	}	
	return setFPID.size();
}
int CDataInfo::GetTrustTradeAccountByFinancialID(int  nFinancialID, set<int>& setFPID)
{
	int ncount = m_multimapFinancialID2TrustTradeAccount.count(nFinancialID);
	multimap<int, TrustTradeAccount>::iterator it2 = m_multimapFinancialID2TrustTradeAccount.find(nFinancialID);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		setFPID.insert(it2->second.nTradeAccountID);
		it2++;
	}	
	return setFPID.size();
}
int CDataInfo::GetTrustTradeAccountByOrgIDFinancialID(int  nAssetMgmtOrg, bool bIncludeChildren,int  nFinancialID, set<int>& setFPID)
{
	int ncount = m_multimapOrgID2TrustTradeAccount.count(nAssetMgmtOrg);
	multimap<int, TrustTradeAccount>::iterator it2 = m_multimapOrgID2TrustTradeAccount.find(nAssetMgmtOrg);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		TrustTradeAccount tAccount = it2->second;
		if(tAccount.nFinancialProductID == nFinancialID)
			setFPID.insert(it2->second.nTradeAccountID);
		it2++;
	}
	if(!bIncludeChildren)//不包含子节点
		return setFPID.size();

	std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return setFPID.size();

	map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
	for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetTrustTradeAccountByOrgIDFinancialID(key,  bIncludeChildren, nFinancialID, setFPID);//循环遍历子节点
	}
	return setFPID.size();
}
int CDataInfo::GetTrustTradeAccountByOrgIDFinancialID(int  nAssetMgmtOrg, bool bIncludeChildren,int  nFinancialID, set<TrustTradeAccount>& setFPID)
{
	int ncount = m_multimapOrgID2TrustTradeAccount.count(nAssetMgmtOrg);
	multimap<int, TrustTradeAccount>::iterator it2 = m_multimapOrgID2TrustTradeAccount.find(nAssetMgmtOrg);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		TrustTradeAccount tAccount = it2->second;
		if(tAccount.nFinancialProductID == nFinancialID)
			setFPID.insert(it2->second);		
		it2++;
	}
	if(!bIncludeChildren)//不包含子节点
		return setFPID.size();

	std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return setFPID.size();

	map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
	for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetTrustTradeAccountByOrgIDFinancialID(key,  bIncludeChildren, nFinancialID, setFPID);//循环遍历子节点
	}
	return setFPID.size();
}
bool CDataInfo::GetFinancialProduct(int nFinancialID, FinancialProduct& fP)
{
	MAPFinancialProduct::iterator it = m_mapFinancialProduct.find(nFinancialID);
	if(it == m_mapFinancialProduct.end())
		return false;

	fP = it->second;
	return true;
}

bool CDataInfo::GetTrustTradeAccount(int nTradeAccountID, TrustTradeAccount& fP)
{
	MAPTrustTradeAccount::iterator it = m_mapTrustTradeAccount.find(nTradeAccountID);
	if(it == m_mapTrustTradeAccount.end())
		return false;

	fP = it->second;
	return true;
}
bool CDataInfo::GetTrustTradeAccount(string strTradeAccountID, TrustTradeAccount& fP)
{
	MAPTrustTradeAccountByName::iterator it = m_mapTrustTradeAccountByName.find(strTradeAccountID);
	if(it == m_mapTrustTradeAccountByName.end())
		return false;

	fP = it->second;
	return true;
}
bool CDataInfo::GetInstrumentFieldByInstrumentID(string strID, InstrumentField& instrumentField)
{
	MAPInstrumentID2InstrumentField::iterator it = m_mapInstrumentID2InstrumentField.find(strID);
	if(it == m_mapInstrumentID2InstrumentField.end())
		return false;

	instrumentField = it->second;
	return true;
}
bool CDataInfo::GetInstrumentFieldByProductID(string strID, vector<InstrumentField>& setInstrumentField)
{
	int ncount = m_mapProductID2InstrumentField.count(strID);
	MAPProductID2InstrumentField::iterator it2 = m_mapProductID2InstrumentField.find(strID);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{		
		setInstrumentField.push_back(it2->second);
		it2++;
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
void CDataInfo::GetAllProducts(MAPInstrumentID2InstrumentField& mapID2InstrumentField)
{
	mapID2InstrumentField = m_mapInstrumentID2InstrumentField;
}
void CDataInfo::SetInstrumentField(InstrumentField *pField)
{
	if(pField == NULL)
		return;
	m_mapInstrumentID2InstrumentField.insert(make_pair(pField->InstrumentID , *pField));
	m_mapProductID2InstrumentField.insert(make_pair(pField->ProductID , *pField));
}

void CDataInfo::FinishAeestMgmtOrganization(bool bFinish)
{
	m_bAeestMgmtOrganization	= bFinish;	
}
void CDataInfo::FinishFinancialProduct(bool bFinish)
{
	m_bFinancialProduct			= bFinish;
}
void CDataInfo::FinishRiskIndicatorModule(bool bFinish)
{
	m_bRiskIndicatorModule		= bFinish;
}
void CDataInfo::FinishRiskIndicator(bool bFinish)
{
	m_bRiskIndicator			= bFinish;
}
void CDataInfo::FinishTrustTradeAccount(bool bFinish)
{
	m_bTrustTradeAccount		= bFinish;
}
void CDataInfo::FinishTraderInfo(bool bFinish)
{
	m_bTraderInfo				= bFinish;
}
void CDataInfo::FinishMgmtOrg2Financial(bool bFinish)
{
	m_bMgmtOrg2Financial		= bFinish;
}
void CDataInfo::FinishInstrumentField(bool bFinish)
{
	m_bInstrumentField			= bFinish;
}
void CDataInfo::FinishPermissionValue(bool bFinish)
{
	m_bPermissionValue			= bFinish;
}
void CDataInfo::FinishRiskMgmtUserInfo(bool bFinish)
{
	m_bRiskMgmtUserInfo			= bFinish;
}
void CDataInfo::FinishProductRiskWarning(bool bFinish)
{
	m_bProductRiskWarning = bFinish;
}
void CDataInfo::FinishRiskWarning(bool bFinish)
{
	m_bRiskWarning = bFinish;
}

void CDataInfo::SetRiskEvent(RiskEvent *pRiskEvent)
{
	if(pRiskEvent == NULL)
		return;
/*
	m_mapCurRiskEvent.insert(make_pair(pRiskEvent->nRiskEventID, *pRiskEvent));	

	SRiskEventKey sKey;
	sKey.nTradeAccountID	= pRiskEvent->nTradeAccountID;
	sKey.strInstrumentID	= pRiskEvent->InstrumentID;
	sKey.nRiskIndicatorID	= pRiskEvent->nRiskIndicatorID;
	MAPKey2RiskEvent::iterator it = m_mapRiskEvtID2RiskEvent.find(sKey);
	if(it == m_mapRiskEvtID2RiskEvent.end())
	{
		m_mapRiskEvtID2RiskEvent.insert(make_pair(sKey, *pRiskEvent));	
		return;
	}	
	m_mapRiskEvtID2RiskEvent[sKey] = *pRiskEvent;*/
}
double CDataInfo::GetinitProgress()
{
	int i = m_bAeestMgmtOrganization + m_bFinancialProduct
		+m_bRiskIndicatorModule + m_bRiskIndicator + m_bTrustTradeAccount
		+ m_bTraderInfo + m_bMgmtOrg2Financial + m_bInstrumentField
		+ m_bPermissionValue + m_bRiskMgmtUserInfo + m_bRiskWarning
		+ m_bProductRiskWarning;
	double dbPercent = 12.0;

	return i/dbPercent;

}
void CDataInfo::SetPermissionValue(PermissionValue* pValue)
{
	if(pValue == NULL)
		return;
	m_mapID2PermissionValue.insert(make_pair(pValue->nActionID , *pValue));
}
void CDataInfo::GetPermission(MAPID2PermissionValue&	mapPermissionValue)
{
	mapPermissionValue = m_mapID2PermissionValue;
}
bool CDataInfo::HasPermission(PermissionActionType PAType)
{
	MAPID2PermissionValue::iterator it = m_mapID2PermissionValue.find((int)PAType);
	if(it == m_mapID2PermissionValue.end())
		return false;
	
	PermissionValue per = it->second;

	bool bValue = false;
	if(per.nValue >= 1)
		bValue = true;

	return bValue;
}
void CDataInfo::SetRiskEventHandling(RiskEventHandling* pValue)
{
	if(pValue == NULL)
		return;	
	m_mapAccountID2RiskEventHandling.insert(make_pair(pValue->nTradeAccountID, *pValue));
}
bool CDataInfo::GetRiskEvent(int nAccountID, int nRiskEventID, vector<RiskEventHandling>& vexRiskEventHandling)
{
	int ncount = m_mapAccountID2RiskEventHandling.count(nAccountID);
	MAPAccountID2RiskEventHandling::iterator it2 = m_mapAccountID2RiskEventHandling.find(nAccountID);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{
		RiskEventHandling& tr = it2->second;
		if(nRiskEventID == -1 
			|| tr.nRiskEventID == nRiskEventID)
		vexRiskEventHandling.push_back(tr);	
		it2 ++;
	}
	return true;
}
void CDataInfo::SetRiskMgmtUserInfo(RiskMgmtUserInfo *pValue)
{
	if(pValue == NULL)
		return;
	
	m_mapOrgID2RiskMgmtUserInfo.insert(make_pair(pValue->nAssetMgmtOrgID, *pValue));

	
}
int CDataInfo::GetRiskMgmtUserInfoByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, vector<RiskMgmtUserInfo>& vec)
{
	int ncount = m_mapOrgID2RiskMgmtUserInfo.count(nAssetMgmtOrg);
	MAPOrgID2RiskMgmtUserInfo::iterator it2 = m_mapOrgID2RiskMgmtUserInfo.find(nAssetMgmtOrg);
	for(int nIndex = 0; nIndex < ncount; nIndex ++)
	{		
		vec.push_back(it2->second);		
		it2++;
	}
	if(!bIncludeChildren)//不包含子节点
		return vec.size();

	std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
		m_mapAssetMgmtOrgTree.find(nAssetMgmtOrg);
	if ( it == m_mapAssetMgmtOrgTree.end() )
		return vec.size();

	map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
	for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
	{  
		int key = iter->first; 		
		GetRiskMgmtUserInfoByOrgID(key,  bIncludeChildren, vec);//循环遍历子节点
	}
	return vec.size();
}
void CDataInfo::GetmapOrgID2RiskMgmtUserInfo(MAPOrgID2RiskMgmtUserInfo& mapOrgID2RiskMgmtUserInfo)
{
	mapOrgID2RiskMgmtUserInfo = m_mapOrgID2RiskMgmtUserInfo;
}

void CDataInfo::SetProductRiskWarning(ProductRiskWarning* pValue)
{
	if(pValue == NULL)
		return;

	ProductRiskWarningKey pWKey;
	pWKey.nRiskIndicatorID = pValue->nRiskIndicatorID;
	pWKey.nRiskLevelID     = pValue->nRiskLevelID;
	pWKey.nTradeAccountID  = pValue->nTradeAccountID;
	pWKey.szProductID      = pValue->szProductID;

	m_mapProductRiskWarningKey.insert(make_pair(pWKey, *pValue));
}
void CDataInfo::SetRiskWarning(RiskWarning* pValue)
{
	if(pValue == NULL)
		return;
	RiskWarningKey pKey;
	pKey.nRiskIndModuleID	= pValue->nRiskIndModuleID; 
	pKey.nRiskIndicatorID	= pValue->nRiskIndicatorID; 
	pKey.nRiskLevelID		= pValue->nRiskLevelID;		
	//m_mapRiskWarning.insert(make_pair(pKey, *pValue));竟然这样有bug
	m_mapRiskWarning[pKey]= *pValue;
}
bool CDataInfo::GetRiskWarning(RiskWarningKey key, RiskWarning& rw)
{
	MAPRiskWarning::iterator it = m_mapRiskWarning.find(key);
	if(it == m_mapRiskWarning.end())
		return false;
	
	rw = it->second;
	return true;
}
bool CDataInfo::GetProductRiskWarning(ProductRiskWarningKey key, ProductRiskWarning& rw)
{
	MAPProductRiskWarningKey::iterator it = m_mapProductRiskWarningKey.find(key);
	if(it == m_mapProductRiskWarningKey.end())
		return false;

	rw = it->second;
	return true;
}