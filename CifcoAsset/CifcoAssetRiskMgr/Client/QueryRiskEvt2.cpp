#include "StdAfx.h"
#include "QueryRiskEvt2.h"
#include <set>
#include <map>
#include <vector>

QueryRiskEvt2::QueryRiskEvt2(void)
{
	m_pdataInfo = CDataInfo::NewInstance();
	//dlgFinancialProductIDList = new CFinancialProductIDListDlg();
	//dlgMgmtOrganIDTreeList = new CAssetMgmtOrganIDTreeListDlg();
	//dlgOrganAndAccountIDTreeList = new COrganAndAccountIDTreeListDlg();
}

QueryRiskEvt2::~QueryRiskEvt2(void)
{
}
//订阅事件
void QueryRiskEvt2::SubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::SubscribePkg(Cmd_RM_QryRiskEvent_Rsp,(int)hWnd);
}
//退订事件
void QueryRiskEvt2::UnsubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}

//向服务器请求查询风险事件
void QueryRiskEvt2::ReqQueryRiskEvt( std::set<int> setProdAccntID,std::string strDateStart, std::string strDateEnd )
{
	
	//mapRiskEvent.clear();

	//bool bNoChecked = true;
	//std::set<int> setOrganID;
	//std::set<int> setFinanProdID;
	//dlgMgmtOrganIDTreeList->GetSelectID(setOrganID, bNoChecked);
	//if(!bNoChecked) {
	//	dlgFinancialProductIDList->GetSelectID(setFinanProdID);
	//}

	//std::set<int> setTemp;
	//std::set<int> setOrganAccntID;

	//std::set<int>::iterator it;
	//std::set<int>::iterator itTmp;
	//for(it = setOrganID.begin(); it != setOrganID.end(); it++) {
	//	setTemp.clear();
	//	m_pdataInfo->GetTrustTradeAccountByOrgID(*it, bNoChecked, setTemp);
	//	for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
	//		setOrganAccntID.insert(*itTmp);
	//	}
	//}

	//std::set<int> setProdAccntID;

	//for(it = setFinanProdID.begin(); it != setFinanProdID.end(); it++) {
	//	setTemp.clear();
	//	m_pdataInfo->GetTrustTradeAccountByFinancialID(*it, setTemp);
	//	for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
	//		setProdAccntID.insert(*itTmp);
	//	}
	//}

	//int nMax = max(setOrganAccntID.size(), setProdAccntID.size());
	//int* pTempID = new int[nMax];

	//std::set<int> setResult;
	//int* pEnd = std::set_intersection(
	//	setOrganAccntID.begin(), setOrganAccntID.end(), 
	//	setProdAccntID.begin(), setProdAccntID.end(), pTempID);
	//int* nPos = pTempID;
	//while(nPos < pEnd) {
	//	setResult.insert(*nPos);
	//	nPos++;
	//}

	RiskEventQueryKey Condition;
    memset(&Condition,0,sizeof(Condition));	
    strncpy(Condition.szStartDate, strDateStart.c_str(),sizeof(Condition.szStartDate)-1);
	strncpy(Condition.szEndDate, strDateEnd.c_str(),sizeof(Condition.szEndDate)-1);
	//strncpy(Condition.szEndDate, "2013-01-25",sizeof(Condition.szEndDate)-1);
	std::set<int>::iterator itstr=setProdAccntID.begin();
	for(itstr;itstr!=setProdAccntID.end();itstr++)
	{
		Condition.nTradeAccountID = *itstr;

	    CTcpLayer::SendData(Cmd_RM_QryRiskEvent_Req,(void *)&Condition, sizeof(Condition), 0);
	}
}

//处理风险事件消息
bool QueryRiskEvt2::ProcessQueryRiskEvtPkg(const char* pPkg,std::map<int, RiskEvent>& mapRiskEvent,int fieldcnt)
{
	for ( int i = 0; i < fieldcnt; i++ )
	{
		RiskEvent* pField = (RiskEvent*)((char*)(pPkg+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEvent));

		//RiskEventKey  RiskEvtKey;
		//RiskEvtKey.nRiskEventID = pField->nRiskEventID;
		//RiskEvtKey.nRiskEventSubID = pField->nRiskEventSubID;

			//mapRiskEvent.insert(make_pair(RiskEvtKey, *pField));
		mapRiskEvent[pField->nRiskEventID] =  *pField;

	}

	

	return true;
}