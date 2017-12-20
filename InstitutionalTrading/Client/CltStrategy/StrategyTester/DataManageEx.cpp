
#include "stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
#include "Windows.h"

CDataManageEx* CDataManageEx::m_pInstance = NULL;
int CDataManageEx::sm_nCount = 0;

CDataManageEx* CDataManageEx::NewInstance()
{
	sm_nCount++;
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CDataManageEx();
	}

	return m_pInstance;
}

void CDataManageEx::DestroyInstance()
{
	sm_nCount--;
	if(sm_nCount == 0 && m_pInstance != NULL) 
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CDataManageEx::CDataManageEx() : 
	CDataManage()
{
	//m_pMDApi = NULL;
	m_pTraderApi = NULL;

	m_bIsConnectedQuot = false;
	m_bIsConnectedTrader = false;

	m_nLoginQuotStatus = conLogout;
	m_nLoginTraderStatus = conLogout;

	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CDataManageEx::~CDataManageEx()
{
	CloseHandle(m_mutex);
}

bool CDataManageEx::Init()
{
	m_pTraderApi = _GetTraderApi();
	m_pTraderApi->RegisterFront((LPSTR)_GetStrategyServer().c_str());
	m_pTraderApi->SetBuzSpi(this);
   	m_pTraderApi->Init();

	//m_pMDApi = _GetMDApi();
	//m_pMDApi->RegisterFront((LPSTR)_GetQuotServer().c_str());
 //	m_pMDApi->SetBuzSpi(this);
 // 	m_pMDApi->Init();

	return true;
}

void CDataManageEx::Release()
{
	//m_pTraderApi->Release();
	////delete m_pTraderApi;
	m_pTraderApi = NULL;

	//m_pMDApi->Release();
	////delete m_pMDApi;
	//m_pMDApi = NULL;
}

void CDataManageEx::SubscribeEvent(UINT nEventID, HWND hWnd)
{
	std::map<UINT, set<HWND>>::iterator it;
	//LockMe();
	it = m_mapSubs.find(nEventID);
	if(it != m_mapSubs.end()) {
		it->second.insert(hWnd);
	}
	else {
		std::set<HWND> setHWnd;
		setHWnd.insert(hWnd);
		m_mapSubs.insert(std::make_pair(nEventID, setHWnd));
	}
	//UnLockMe();
}

void CDataManageEx::UnsubscribeEvent(UINT nEventID, HWND hWnd)
{
	std::map<UINT, set<HWND>>::iterator it;
	//LockMe();
	it = m_mapSubs.find(nEventID);
	if(it != m_mapSubs.end()) {
		it->second.erase(hWnd);
	}
	//UnLockMe();
}

void CDataManageEx::UnsubscribeAllEvent(HWND hWnd)
{
	std::map<UINT, set<HWND>>::iterator it;
	//LockMe();
	for(it = m_mapSubs.begin(); it != m_mapSubs.end(); it++) {
		it->second.erase(hWnd);
	}
	//UnLockMe();
}

bool CDataManageEx::PostEventToWnd(int nEventID)
{
	std::map<UINT, std::set<HWND>>::iterator it;
	std::set<HWND>::iterator itWnd;

	//LockMe();

	if((it = m_mapSubs.find(nEventID)) != m_mapSubs.end()) {
		for(itWnd = it->second.begin(); itWnd != it->second.end(); itWnd++) {
			if(IsWindow(*itWnd)) {
				PostMessage(*itWnd, _GetEventMsgID(), nEventID, NULL);
			}
		}
	}

	//UnLockMe();
	return true;
}

bool CDataManageEx::LoginQuot(const std::string& strUserID, const std::string& strPasswd)
{
	//if(m_pMDApi==NULL)
	//	return false;

	//m_nLoginQuotStatus = conLogon;
	//return m_pMDApi->ReqUserLogin(strUserID, strPasswd, 0)==0;
	return 0;
}

bool CDataManageEx::LoginTrader(const std::string& strUserID, const std::string& strPasswd)
{
	if(m_pTraderApi==NULL)
		return false;

	int nRet = m_pTraderApi->ReqUserLogin(strUserID, strPasswd, 0);
	if(nRet==0)
		m_nLoginTraderStatus = conLoginning;
	else
		m_nLoginTraderStatus = conLoginFail;
	return nRet==0;
}

bool CDataManageEx::LogoutQuot()
{
	return true;
}

bool CDataManageEx::LogoutTrader()
{
	m_nLoginTraderStatus = conLogout;
	return true;
}

bool CDataManage::GetExchangeID(const std::string& strInstrumentID, std::string& strExchangeID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrument.find(strInstrumentID);
	if(it != m_mapInstrument.end()) {
		strExchangeID = it->second.ExchangeID;
		return true;
	}
	else
		return false;
}

bool CDataManage::GetProductID(const std::string& strInstrumentID, std::string& strProductID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrument.find(strInstrumentID);
	if(it != m_mapInstrument.end()) {
		strProductID = it->second.ProductID;
		return true;
	}
	else
		return false;
}

void CDataManageEx::OnQuotConnected()
{
	m_bIsConnectedQuot = true;
	//if(m_pMDApi!=NULL)
	//	m_pMDApi->ReqUserLogin(_GetCurrUserID(), _GetPassword(), 0);
}

void CDataManageEx::OnQuotDisconnected()
{
	m_bIsConnectedQuot = false;
	m_nLoginQuotStatus = conLogout;
}

void CDataManageEx::OnTraderConnected()
{
	m_bIsConnectedTrader = true;
	if(m_pTraderApi!=NULL)
		m_pTraderApi->ReqUserLogin(_GetCurrUserID(), _GetPassword(), 0);
}

void CDataManageEx::OnTraderDisconnected()
{
	m_bIsConnectedTrader = false;
	m_nLoginTraderStatus = conLogout;
}

void CDataManageEx::OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)
{
}

void CDataManageEx::OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)
{
	m_strLoginTraderFailMsg = strErrorMsg;
	if(nErrorNum==0) {
		m_nLoginTraderStatus = conLogon;
		if(m_pTraderApi!=NULL) {
			if(m_mapInstrument.size()==0) {
				PlatformStru_QryInstrument qry;
				m_pTraderApi->ReqQryInstrument(&qry, 0);
			}
		}
	}
	else
		m_nLoginTraderStatus = conLoginFail;
	PostEventToWnd(conTraderLogin);
}

void CDataManageEx::OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum)
{
	char csText[256] = {0};
	char* ppInstrumentID[] = { (char*)&csText };
	for(int i=0; i<vec.size(); i++) {
		m_mapInstrument[vec[i].InstrumentID] = vec[i];
		if(m_pTraderApi != NULL) {
			{
			CThostFtdcQryInstrumentMarginRateField qry = {0};
			strncpy(qry.InstrumentID, vec[i].InstrumentID, sizeof(qry.InstrumentID)-1);
			qry.HedgeFlag=THOST_FTDC_HF_Speculation;
			strncpy(qry.BrokerID, "", sizeof(qry.BrokerID)-1);								///经纪公司代码    
			strncpy(qry.InvestorID, _GetCurrUserID().c_str(), sizeof(qry.InvestorID)-1);	///投资者代码
			m_pTraderApi->ReqQryInstrumentMarginRate(&qry, 0);
			}
			{
			CThostFtdcQryInstrumentCommissionRateField qry = {0};
			strncpy(qry.InstrumentID, vec[i].InstrumentID, sizeof(qry.InstrumentID)-1);
			strncpy(qry.BrokerID, "", sizeof(qry.BrokerID)-1);								///经纪公司代码    
			strncpy(qry.InvestorID, _GetCurrUserID().c_str(), sizeof(qry.InvestorID)-1);	///投资者代码
			m_pTraderApi->ReqQryInstrumentCommissionRate(&qry, 0);
			}
		}
	}
}

void CDataManageEx::OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum)
{
	////m_mapMDQuot[rsp.InstrumentID] = rsp;
	//m_lstQuots.push_back(rsp);
	//PostEventToWnd(conDepthMarketData);
}

void CDataManageEx::OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum)
{
	m_mapCommissionRate[rsp.InstrumentID] = rsp;
	PostEventToWnd(conQryCommissionRate);
}

void CDataManageEx::OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum)
{
	m_mapMarginRate[rsp.InstrumentID] = rsp;
	PostEventToWnd(conQryMarginRate);
}
