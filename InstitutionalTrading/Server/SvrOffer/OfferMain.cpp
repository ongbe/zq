#include "stdafx.h"
#include "CTPAccount.h"

#include "OfferMain.h"


COfferMain::COfferMain()
{


	m_bFinishedQueryInstrument = false;

	m_bNeedQueryInstrument = false;	
}

COfferMain::~COfferMain(void)
{
	Release();
}

void COfferMain::LogoffAccount(BrokerAccountKey BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();
	
		m_mutex_mapTimer2CTPAccount.write_lock();
		std::map<UINT_PTR, CCTPAccount*>::iterator itMap = m_mapTimer2CTPAccount.begin();
		for(; itMap != m_mapTimer2CTPAccount.end(); itMap++)
		{
			CCTPAccount* pClear = itMap->second;
			if(pClear == pCtpAccount)
			{
				m_mapTimer2CTPAccount.erase(itMap);
				break;
			}
		}
		m_mutex_mapTimer2CTPAccount.write_unlock();


		pCtpAccount->SetAccountStatus(ACCOUNT_STATUS_Disconnected);
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
		m_mapUserID2CTPAccount.erase(it);
	}
	
}
void COfferMain::Release()
{
	
	

	m_mutex_mapTimer2CTPAccount.write_lock();
	m_mapTimer2CTPAccount.clear();
	m_mutex_mapTimer2CTPAccount.write_unlock();

	//std::map<std::string, int>::iterator itQuto = m_mapInstrumentID.begin();
	//for(;itQuto != m_mapInstrumentID.end(); itQuto++)
	//{
	//	std::string strInstrument = itQuto->first;
	//	CTCPDataService::GetInstance()->SendData(Cmd_UnSubscribeQuotEvent_Req, (void *)strInstrument.c_str(), strlen(strInstrument.c_str())+1, 0);
	//}
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(; it != m_mapUserID2CTPAccount.end(); it++)
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();
		pCtpAccount->SetAccountStatus(ACCOUNT_STATUS_Disconnected);
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
	}	
	m_mapUserID2CTPAccount.clear();
	std::multimap<BrokerAccountKey, CCTPAccount*>::iterator itM = m_mapUserID2CTPAccount_Ratio.begin();
	for(; itM != m_mapUserID2CTPAccount_Ratio.end(); itM++)
	{		
		m_mutex_mapUserID2CTPAccount_Ratio.write_lock();
		CCTPAccount* pCtpAccount = itM->second;		
		m_mutex_mapUserID2CTPAccount_Ratio.write_unlock();
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
	}
	m_mapUserID2CTPAccount_Ratio.clear();
}
void COfferMain::Init(bool bSettlement, int nMorning)
{
	//if(!bTrade)
		InitDBData();

	std::vector<SOrderTransfer>				vecOrderTransfer;
	GetOrderTransfer(vecOrderTransfer);
	SetOrderTransferByAccount(vecOrderTransfer);
/*	for(int i=0; i< (int)vecOrderTransfer.size(); i++)
	{
		SOrderTransfer sTransfer = vecOrderTransfer[i];
		m_mutex_mapAccountID2OrderTransfer.write_lock();
		BrokerAccountKey BAKey(sTransfer.szBrokerID, sTransfer.szAccountID);
		m_mapAccountID2OrderTransfer.insert(make_pair(BAKey, sTransfer));
		m_mutex_mapAccountID2OrderTransfer.write_unlock();
	}*/

	std::vector<SLogin> vecLogin;
	std::vector<TradeAccount> vec;
	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(vec);	
	if(vec.size() ==0)
		return;
	for(int i=0; i < (int)vec.size(); i++)
	{
		TradeAccount& tradeAccount = vec[i];
		SLogin lsTemp;

		BrokerInfo brokeInfo;
		CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID, brokeInfo);
		lsTemp.strBrokerID = brokeInfo.szBrokerCode;	

		if(bSettlement)
		{//结算用的是经纪公司统一的服务器地址
			
			std::vector<ServerGroup> vecServerGroup;
			CInterface_SvrBrokerInfo::getObj().GetServerGroupsByBrokerID(tradeAccount.nBrokerID, vecServerGroup);
			lsTemp.vecServerGroup = vecServerGroup;
			for(int nGroup =0; nGroup < (int)vecServerGroup.size(); nGroup++)
			{
				ServerGroup& sGroup = vecServerGroup[nGroup];
				if(sGroup.nIsSettle)
				{//如果服务器组处于启动状态
					std::vector<ServerAddress> vecAddress;
					CInterface_SvrBrokerInfo::getObj().GetServerAddrByGroupID(sGroup.nServerID, vecAddress);
					copy(vecAddress.begin(), vecAddress.end(), back_inserter(lsTemp.vecServerAddress));
				}
			}
		}
		else
		{//交易用的是某个交易员特定的服务器地址
			 ServerGroup group;
			 CInterface_SvrBrokerInfo::getObj().GetServerGroupByID(tradeAccount.nServerGroupID, group);
			 std::vector<ServerAddress> vecAddress;
			 CInterface_SvrBrokerInfo::getObj().GetServerAddrByGroupID(group.nServerID, vecAddress);
			 copy(vecAddress.begin(), vecAddress.end(), back_inserter(lsTemp.vecServerAddress));
		}
		lsTemp.strUserID = tradeAccount.szTradeAccount;
		lsTemp.strPassword = tradeAccount.szPwd;

		vecLogin.push_back(lsTemp);		
	}
	for(int i=0; i< (int)vecLogin.size(); i++)
	{
		SLogin& login = vecLogin[i];
		BrokerAccountKey BAKey(login.strBrokerID.c_str(), login.strUserID.c_str());
		CCTPAccount* pCtpAccount = new CCTPAccount;
		pCtpAccount->Init(login, bSettlement, THOST_TERT_QUICK);
		InsertOrderRef(BAKey, pCtpAccount);
		pCtpAccount->InitQueryThread();		

		m_mutex_mapUserID2CTPAccount.write_lock();
		m_mapUserID2CTPAccount.insert(make_pair(BAKey, pCtpAccount)); 				
		m_mutex_mapUserID2CTPAccount.write_unlock();					
	}	
	
}
void COfferMain::RestartAccount(int nTradeAccount)
{
	TradeAccount tradeAccount;
	if(!CInterface_SvrBrokerInfo::getObj().GetTradeAccount(nTradeAccount, tradeAccount))
		return;
	BrokerInfo brokeInfo;
	CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID, brokeInfo);
	BrokerAccountKey BAKey;
	strcpy(BAKey.BrokerID, brokeInfo.szBrokerCode);
	strcpy(BAKey.AccountID, tradeAccount.szTradeAccount);

	m_mutex_mapUserID2CTPAccount.write_lock();
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{
		CCTPAccount* pCtpAccount = it->second;
		m_mapUserID2CTPAccount.erase(it);				
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
	}		
	m_mutex_mapUserID2CTPAccount.write_unlock();

	m_mutex_mapUserID2CTPAccount_Ratio.write_lock();

	int nCount = m_mapUserID2CTPAccount_Ratio.count(BAKey);
	std::multimap<BrokerAccountKey, CCTPAccount*>::iterator itRatio = m_mapUserID2CTPAccount_Ratio.find(BAKey);
	for(int i =0; i<nCount; i++)
	{
		CCTPAccount* pCtpAccount = itRatio->second;			
		pCtpAccount->Release();
		if(pCtpAccount)
		{
			delete pCtpAccount;
			pCtpAccount = NULL;
		}
		itRatio = m_mapUserID2CTPAccount_Ratio.erase(itRatio);			
	}	
	m_mutex_mapUserID2CTPAccount_Ratio.write_unlock();
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	std::vector<SLogin> vecLogin;

	SLogin lsTemp;	
	lsTemp.strBrokerID = brokeInfo.szBrokerCode;	
	//交易用的是某个交易员特定的服务器地址
	ServerGroup group;
	CInterface_SvrBrokerInfo::getObj().GetServerGroupByID(tradeAccount.nServerGroupID, group);
	std::vector<ServerAddress> vecAddress;
	CInterface_SvrBrokerInfo::getObj().GetServerAddrByGroupID(group.nServerID, vecAddress);
	copy(vecAddress.begin(), vecAddress.end(), back_inserter(lsTemp.vecServerAddress));		
	lsTemp.strUserID = tradeAccount.szTradeAccount;
	lsTemp.strPassword = tradeAccount.szPwd;
	vecLogin.push_back(lsTemp);		
	
	for(int i=0; i< (int)vecLogin.size(); i++)
	{
		SLogin& login = vecLogin[i];
		BrokerAccountKey BAKey(login.strBrokerID.c_str(), login.strUserID.c_str());
		CCTPAccount* pCtpAccount = new CCTPAccount;
		pCtpAccount->Init(login, 0, THOST_TERT_QUICK);
		InsertOrderRef(BAKey, pCtpAccount);
		pCtpAccount->InitQueryThread();		

		m_mutex_mapUserID2CTPAccount.write_lock();
		m_mapUserID2CTPAccount.insert(make_pair(BAKey, pCtpAccount)); 				
		m_mutex_mapUserID2CTPAccount.write_unlock();					
	}	

}
void COfferMain::CheckAccountsConnect()
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(; it != m_mapUserID2CTPAccount.end(); it++)
	{
		m_mutex_mapUserID2CTPAccount.write_lock();	
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();	

		EnumAccountStatus enumStatus = pCtpAccount->GetAccountStatus();
		if(enumStatus == ACCOUNT_STATUS_UnInit)
		{			
			SLogin login;
			pCtpAccount->GetLogin(login);
			CInterface_SvrTradeData::getObj().SetAccountStatus(login.strUserID, login.strBrokerID, ACCOUNT_STATUS_CannotConnect);
			OFFER_INFO("10秒钟连接不上ctp，主账号[%s]\n", login.strUserID.c_str());
			//PUSH_LOG_ERROR(FirstLevelError,true,true,"10秒钟连接不上ctp，主账号[%s]\n", login.strUserID.c_str());
		}
	}
}


bool COfferMain::IsAllAcountFinishLoginIn()
{
	bool bFinished = true;
	
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(; it != m_mapUserID2CTPAccount.end(); it++)
	{
		m_mutex_mapUserID2CTPAccount.write_lock();	
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();	

		EnumAccountStatus enumStatus = pCtpAccount->GetAccountStatus();
		int nLoginCount = pCtpAccount->GetLoginCount();
		int nstatus = enumStatus == ACCOUNT_STATUS_Login || (enumStatus == ACCOUNT_STATUS_LoginFailure && nLoginCount == 3);
		if(nstatus == false)
		{
			bFinished = false;
			break;
		}
	}
	
	return bFinished;
}
void COfferMain::SetNeedQueryInstruments(bool bNeed)
{
	m_mutex_NeedQueryInstrument.write_lock();
	m_bNeedQueryInstrument = bNeed;
	m_mutex_NeedQueryInstrument.write_unlock();
}
bool COfferMain::NeedQueryInstruments()
{
	bool bNeed = false;
	m_mutex_NeedQueryInstrument.write_lock();
	bNeed = m_bNeedQueryInstrument;
	m_mutex_NeedQueryInstrument.write_unlock();

	return bNeed;
}
void COfferMain::QueryInstruments()
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(;it != m_mapUserID2CTPAccount.end(); it++)
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();
		if(pCtpAccount->GetAccountStatus() == ACCOUNT_STATUS_Login)
		{
			pCtpAccount->QueryInstruments();
			return;		
		}
	}	

	m_mutex_NeedQueryInstrument.write_lock();
	m_bNeedQueryInstrument = true;
	m_mutex_NeedQueryInstrument.write_unlock();	
}
void COfferMain::QueryCommission(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		std::map<std::string, int>::iterator itQuto = m_mapInstrumentID.begin();
		for(;itQuto != m_mapInstrumentID.end(); itQuto++)
		{
			std::string strInstrument = itQuto->first;
			pCtpAccount->QueryCommission(strInstrument);	
		}	
		
		SLogin login;
		pCtpAccount->GetLogin(login);
		BrokerAccountKey BAccountKey(login.strBrokerID.c_str(), login.strUserID.c_str());
		int nCount = m_mapUserID2CTPAccount_Ratio.count(BAccountKey);
		std::multimap<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount_Ratio.find(BAccountKey);
		if(it == m_mapUserID2CTPAccount_Ratio.end())
		{		
			for(int k =0; k<3; k++)
			{
				CCTPAccount* pCtp = new CCTPAccount;
				pCtp->Init(login, 0, THOST_TERT_QUICK);
				pCtp->SetIsQueryRatio(true);
				pCtp->InitQueryRatioThread();
		
				m_mutex_mapUserID2CTPAccount_Ratio.write_lock();
				m_mapUserID2CTPAccount_Ratio.insert(make_pair(BAccountKey, pCtp)); 	
				m_mutex_mapUserID2CTPAccount_Ratio.write_unlock();	
			}
		}	
	}
}
void COfferMain::QueryMargin(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;		
		m_mutex_mapUserID2CTPAccount.write_unlock();		

		std::map<std::string, int>::iterator itQuto = m_mapInstrumentID.begin();
		for(;itQuto != m_mapInstrumentID.end(); itQuto++)
		{
			std::string strInstrument = itQuto->first;
			pCtpAccount->QueryMargin(strInstrument);	
		}

	//////////////////////////////////////////////////////////////////////////		
		SLogin login;
		pCtpAccount->GetLogin(login);
		BrokerAccountKey BAccountKey(login.strBrokerID.c_str(), login.strUserID.c_str());
		int nCount = m_mapUserID2CTPAccount_Ratio.count(BAccountKey);
		std::multimap<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount_Ratio.find(BAccountKey);
		if(it == m_mapUserID2CTPAccount_Ratio.end())
		{
			for(int k =0; k<3; k++)
			{
				CCTPAccount* pCtp = new CCTPAccount;
				pCtp->Init(login, 0, THOST_TERT_QUICK);
				pCtp->SetIsQueryRatio(true);				
				pCtp->InitQueryRatioThread();
		
				m_mutex_mapUserID2CTPAccount_Ratio.write_lock();
				m_mapUserID2CTPAccount_Ratio.insert(make_pair(BAccountKey, pCtp)); 	
				m_mutex_mapUserID2CTPAccount_Ratio.write_unlock();	
			}
		}			
	}
}
void COfferMain::StartQueryMargin(BrokerAccountKey& BAKey)
{
	int nCount = m_mapUserID2CTPAccount_Ratio.count(BAKey);
	std::multimap<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount_Ratio.find(BAKey);
	for(int i =0; i<nCount; i++)
	{
		m_mutex_mapUserID2CTPAccount_Ratio.write_lock();
		CCTPAccount* pCtpAccount = it->second;		
		m_mutex_mapUserID2CTPAccount_Ratio.write_unlock();			
		pCtpAccount->SetQueryMargin();
		it++;
	}	
}
void COfferMain::StartQueryCommission(BrokerAccountKey& BAKey)
{
	int nCount = m_mapUserID2CTPAccount_Ratio.count(BAKey);
	std::multimap<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount_Ratio.find(BAKey);
	for(int i =0; i<nCount; i++)
	{
		m_mutex_mapUserID2CTPAccount_Ratio.write_lock();
		CCTPAccount* pCtpAccount = it->second;		
		m_mutex_mapUserID2CTPAccount_Ratio.write_unlock();			
		pCtpAccount->SetQueryCommission();
		it++;
	}	
}
void COfferMain::QueryOrder(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		pCtpAccount->QueryOrder();	
	}
}
void COfferMain::QueryTrade(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		pCtpAccount->QueryTrade();	
	}
}
void COfferMain::QueryFund(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		pCtpAccount->QueryFund();	
	}
}
void COfferMain::QueryPosition(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		pCtpAccount->QueryPosition();	
	}
}
void COfferMain::QueryPositionDetail(BrokerAccountKey& BAKey)
{
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		pCtpAccount->QueryPositionDetail();	
	}
}
bool COfferMain::GetFinishedQueryInstrument()
{
	bool bFinishedQueryInstrument = false;
	m_mutex_mapInstrument.write_lock();
	bFinishedQueryInstrument = m_bFinishedQueryInstrument;
	m_mutex_mapInstrument.write_unlock();
	return bFinishedQueryInstrument;
}

void COfferMain::QueryQuot()
{
	m_mutex_mapInstrument.write_lock();
	QueryQuot(m_mapInstrumentID);	
	m_mutex_mapInstrument.write_unlock();
}
void COfferMain::QueryQuot(std::map<std::string, int>& mapInstrumentID)
{
	COfferMainInterface::QueryQuot(mapInstrumentID);
}
void COfferMain::AddInstrument(CThostFtdcInstrumentField *pInstrument, bool bIsLast)
{
	if(pInstrument == NULL)
		return;
	m_mutex_mapInstrument.write_lock();
	m_mapInstrumentID[pInstrument->InstrumentID] = 0;
	m_mutex_mapInstrument.write_unlock();

	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(;it != m_mapUserID2CTPAccount.end(); it++)
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;		
		pCtpAccount->SetCommissionQuery(m_mapInstrumentID);
		pCtpAccount->SetMarginQuery(m_mapInstrumentID);
		m_mutex_mapUserID2CTPAccount.write_unlock();
	}
	
	m_mutex_mulmapProductID2InstrumentID.write_lock();
	m_mulmapProductID2InstrumentID.insert(std::make_pair(pInstrument->ProductID, pInstrument->InstrumentID));
	m_mutex_mulmapProductID2InstrumentID.write_unlock();

	m_mutex_mapInstrumentID2ProductID.write_lock();
	m_mapInstrumentID2ProductID.insert(std::make_pair(pInstrument->InstrumentID, pInstrument->ProductID));
	m_mutex_mapInstrumentID2ProductID.write_unlock();

	m_mutex_mapInstrument.write_lock();
	if(bIsLast)
	{
		m_bFinishedQueryInstrument = true;
		QueryQuot(m_mapInstrumentID);
	}
	m_mutex_mapInstrument.write_unlock();
}
void COfferMain::AddInstrument()
{
	std::vector<PlatformStru_InstrumentInfo> lVecInstruments;
	std::set<std::string> lInstrumentStringSet;
	CInterface_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
	OFFER_INFO("Instruments size %d\n",lInstrumentStringSet.size());
	//PUSH_LOG_ERROR(Logger,false,true,"Instruments size %d",lInstrumentStringSet.size());
	std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
	while(lIterSet != lInstrumentStringSet.end())
	{
		PlatformStru_InstrumentInfo lTempInfo;
		if(0 == CInterface_SvrTradeData::getObj().GetInstrumentInfo(*lIterSet,lTempInfo))
		{
			m_mutex_mapInstrument.write_lock();
			m_mapInstrumentID[lTempInfo.InstrumentID] = 0;
			m_mutex_mapInstrument.write_unlock();

			m_mutex_mulmapProductID2InstrumentID.write_lock();
			m_mulmapProductID2InstrumentID.insert(std::make_pair(lTempInfo.ProductID, lTempInfo.InstrumentID));
			m_mutex_mulmapProductID2InstrumentID.write_unlock();

			m_mutex_mapInstrumentID2ProductID.write_lock();
			m_mapInstrumentID2ProductID.insert(std::make_pair(lTempInfo.InstrumentID, lTempInfo.ProductID));
			m_mutex_mapInstrumentID2ProductID.write_unlock();
		}			
		lIterSet++;
	}

	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.begin();
	for(;it != m_mapUserID2CTPAccount.end(); it++)
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;		
		pCtpAccount->SetCommissionQuery(m_mapInstrumentID);
		pCtpAccount->SetMarginQuery(m_mapInstrumentID);
		m_mutex_mapUserID2CTPAccount.write_unlock();
	}	

	m_mutex_mapInstrument.write_lock();
	m_bFinishedQueryInstrument = true;
	m_mutex_mapInstrument.write_unlock();

	QueryQuot(m_mapInstrumentID);
}
bool COfferMain::GetNextQueryInstrumentOfMargin(std::string strBrokerID, std::string strUser, std::string& strInstrument, std::string& strHedgeFlag)
{
	bool bGetNext = false;
	
	BrokerAccountKey BAKey(strBrokerID.c_str(), strUser.c_str());
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		bGetNext = pCtpAccount->GetNextInstrumentOfMargin(strInstrument, strHedgeFlag);		
	}	
	return bGetNext;
}
void COfferMain::GetInstrumentsByProductID(std::string strProduct, std::vector<std::string>& vecInstruments)
{	
	m_mutex_mulmapProductID2InstrumentID.write_lock();
	int nCount = m_mulmapProductID2InstrumentID.count(strProduct);
	std::multimap<std::string, std::string>::iterator it = m_mulmapProductID2InstrumentID.find(strProduct);
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		vecInstruments.push_back(it->second);
		it++;
	}	
	m_mutex_mulmapProductID2InstrumentID.write_unlock();
  
}
bool COfferMain::GetProductIDByInstruments(std::string strInstrument, std::string& strProduct)
{
	bool bFind = false;
	m_mutex_mapInstrumentID2ProductID.write_lock();
	std::map<std::string, std::string>::iterator it = m_mapInstrumentID2ProductID.find(strInstrument);
	if(it !=  m_mapInstrumentID2ProductID.end())
	{
		strProduct = it->second;
		bFind = true;
	}
	m_mutex_mapInstrumentID2ProductID.write_unlock();

	return bFind;
}
bool COfferMain::GetNextQueryInstrumentOfCommission(std::string strBrokerID, std::string strUser, std::string& strInstrument)
{
	bool bGetNext = false;	
	BrokerAccountKey BAKey(strBrokerID.c_str(), strUser.c_str());
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		bGetNext = pCtpAccount->GetNextInstrumentOfCommission(strInstrument);		
	}		

	return bGetNext;
}

int COfferMain::ReqOrderInsert_Main(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID)
{
	//去掉前面的空格
	int nOrderRef = atoi(key.szOrderRef);
	sprintf(key.szOrderRef, "%d", nOrderRef);

	int nRet = CF_ERROR_RISK_NOTVALIDORDER;
	if(pInputOrder == NULL)
		return nRet;	
	std::string strInvestor   = pInputOrder->InvestorID;	
	std::string strBrokerID;
	std::string strAccountID;//
	GetTradeAccountByUserID(nTraderID, strBrokerID, strAccountID);
	//strcpy(pInputOrder->InvestorID, strAccountID.c_str());

	BrokerAccountKey BAKey(strBrokerID.c_str(), strAccountID.c_str());
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;
		m_mutex_mapUserID2CTPAccount.write_unlock();

		nRet = pCtpAccount->ReqOrderInsert_Account(pInputOrder, nTraderID, key, nRequestID);

	}	
	

	return nRet;
}
int COfferMain::ReqOrderAction_Main(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID)
{
	//去掉前面的空格
	int nOrderRef = atoi(key.szOrderRef);
	sprintf(key.szOrderRef, "%d", nOrderRef);

	int nRet = CF_ERROR_RISK_NOTVALIDORDER;
	if(pInputOrderAction == NULL)
		return nRet;	

	std::string strBrokerID;
	std::string strAccountID;
	GetTradeAccountByUserID(nTraderID, strBrokerID, strAccountID);
	//strcpy(pInputOrderAction->InvestorID, strAccountID.c_str());				///投资者代码
	
	BrokerAccountKey BAKey(strBrokerID.c_str(), strAccountID.c_str());
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		CCTPAccount* pCtpAccount = it->second;	
		m_mutex_mapUserID2CTPAccount.write_unlock();

		nRet = pCtpAccount->ReqOrderAction_Account(pInputOrderAction, nTraderID, key, nRequestID);		
	}	
	
	return nRet;
}
bool COfferMain::GetCTPAccount(BrokerAccountKey BAKey, CCTPAccount*& pCtpAccount)
{
	bool bFind = false;	
	std::map<BrokerAccountKey, CCTPAccount*>::iterator it = m_mapUserID2CTPAccount.find(BAKey);
	if(it != m_mapUserID2CTPAccount.end())
	{		
		m_mutex_mapUserID2CTPAccount.write_lock();
		pCtpAccount = it->second;	
		m_mutex_mapUserID2CTPAccount.write_unlock();
		bFind = true;
	}	
	return bFind;
}
bool COfferMain::IsCtpAccountTimerExit(CCTPAccount* pCtpAccount)
{
	bool bExit = false;
	m_mutex_mapTimer2CTPAccount.write_lock();
	std::map<UINT_PTR, CCTPAccount*>::iterator itMap = m_mapTimer2CTPAccount.begin();
	for(; itMap != m_mapTimer2CTPAccount.end(); itMap++)
	{
		CCTPAccount* pClear = itMap->second;
		if(pClear == pCtpAccount)
		{
			bExit = true;
			break;
		}
	}
	m_mutex_mapTimer2CTPAccount.write_unlock();
	return bExit;
}
void COfferMain::SetTimer2CTPAccount(UINT_PTR uPtr, CCTPAccount* pCtpAccount)
{
	m_mutex_mapTimer2CTPAccount.write_lock();
	m_mapTimer2CTPAccount[uPtr] = pCtpAccount;
	m_mutex_mapTimer2CTPAccount.write_unlock();
}
bool COfferMain::GetCtpAccountByTime(UINT_PTR uPtr, CCTPAccount*& pCtpAccount)
{
	bool bGet = false;
	m_mutex_mapTimer2CTPAccount.write_lock();
	
	std::map<UINT_PTR, CCTPAccount*>::iterator it = m_mapTimer2CTPAccount.find(uPtr);
	if(it != m_mapTimer2CTPAccount.end())
	{
		pCtpAccount = it->second;
		bGet = true;
	}
	m_mutex_mapTimer2CTPAccount.write_unlock();

	return bGet;
}
void COfferMain::RemoveCtpAccountByTime(UINT_PTR uPtr)
{
	m_mutex_mapTimer2CTPAccount.write_lock();
	m_mapTimer2CTPAccount.erase(uPtr);
	m_mutex_mapTimer2CTPAccount.write_unlock();
}
