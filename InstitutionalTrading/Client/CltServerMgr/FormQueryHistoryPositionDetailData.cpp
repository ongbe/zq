#include "StdAfx.h"
#include "FormQueryHistoryPositionDetailData.h"

#include "RiskManageStruct.h"
#include "TcpLayer.h"

bool CltServerMgr::FormQueryHistoryPositionDetailData::NewQuery(std::string& strStartDate, 
						std::string& strEndDate, int nUserID, int& nRetCount)
{
	if(setQueryAccountID.size()==0)
		return false;

	this->nUserID = nUserID;
	while(!queueDate.empty()) 
		queueDate.pop();
	nRetCount = _MakeDateSeq(strStartDate, strEndDate, queueDate);
	return NextQuery();
}

bool CltServerMgr::FormQueryHistoryPositionDetailData::NextQuery()
{
	if(setQueryAccountID.size()==0 || queueDate.empty())
		return false;

	std::set<std::string>::iterator it;
	SAdminQuery* pQuery = NULL;

	int nDataLen = sizeof(SAdminQuery)+(setQueryAccountID.size()-1)*sizeof(CommonAccountType);
	std::string strDate;
	char* pBuff = new char[nDataLen];
	pQuery = (SAdminQuery*)pBuff;
	memset(pBuff, 0, nDataLen);

	strDate = queueDate.front();
	queueDate.pop();

	pQuery->nCount = setQueryAccountID.size();
	strncpy(pQuery->szDateBegin, strDate.c_str(), sizeof(pQuery->szDateBegin)-1);
	strncpy(pQuery->szDateEnd, strDate.c_str(), sizeof(pQuery->szDateEnd)-1);

	int i=0;

	LockObject();
	// 执行按帐号ID订阅
	for(it = setQueryAccountID.begin(); it != setQueryAccountID.end(); it++, i++) {
		strncpy(pQuery->strAccount[i], (*it).c_str(), sizeof(pQuery->strAccount[i])-1);
	}	
	UnlockObject();

	CTcpLayer::SendData(Cmd_RM_QryHistoryPositionDetail_Req, (void*)pBuff, nDataLen, 0, nUserID);
	delete pBuff;
	
	return true;
}

bool CltServerMgr::FormQueryHistoryPositionDetailData::StopQuery()
{
	while(!queueDate.empty()) 
		queueDate.pop();
	return true;
}

void CltServerMgr::FormQueryHistoryPositionDetailData::ResetQuery()
{
	LockObject();
	//mapKey2Index.clear();
	//mapIndex2Key.clear();
	//mapPosiDetailInfo.clear();
	while(!queueDate.empty()) 
		queueDate.pop();
	UnlockObject();
//
//	std::set<int>::iterator it;
//	int nAccountID = 0;
//
	LockObject();
//	// 先对原来的订阅进行退订
//	for(it = setQueryAccountID.begin(); 
//		it != setQueryAccountID.end(); it++) {
//			nAccountID = *it;
//			// 这里实际需要的是按帐号进行订阅退订
//			CTcpLayer::SendData(Cmd_RM_UnSubscribeFund_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
//	}
	//setQueryAccountID.clear();
	UnlockObject();
}

//void CltServerMgr::FormQueryHistoryPositionDetailData::CheckNewData(CDataInfo* pdataInfo, 
//								std::queue<PlatformStru_TradingAccountInfo>& queueRet)
//{
//	if(pdataInfo==NULL)
//		return;
//
//	std::map<string, int>::iterator it;
//	int nIndex = -1;
//	char strTemp[1024];
//	std::string strKey;
//	strKey.reserve(1024);
//
//	LockObject();
//	while(!queueLastFundInfo.empty()) {
//		RiskSyncAccountField field = queueLastFundInfo.front();
//		queueLastFundInfo.pop();
//		//if(bIsGroupQuery) {
//		//	if(setFilterFinanProductID.size()>0) {
//		//		TrustTradeAccount traderAccount;
//		//		memset(&traderAccount, 0, sizeof(traderAccount));
//		//		if(pdataInfo->GetTrustTradeAccount(std::string(field.InvestorID), traderAccount)) {
//		//			if(setFilterFinanProductID.find(traderAccount.nFinancialProductID)==setFilterFinanProductID.end())
//		//				continue;
//		//		}
//		//	}
//		//}
//		memset(strTemp, 0, sizeof(strTemp));
//		sprintf(strTemp, "%s,%s", field.InvestorID);
//		strKey = strTemp;
//		mapLastFundInfo[strKey] = field;
//
//		queueRet.push(field);
//	}
//	UnlockObject();
//
//}