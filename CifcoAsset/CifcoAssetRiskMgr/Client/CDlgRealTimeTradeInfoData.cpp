#include "StdAfx.h"
#include "CDlgRealTimeTradeInfoData.h"

#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"

bool Client::CDlgRealTimeTradeInfoData::NewQuery()
{
#if 1
	int nReqID = 0;
	int nIndex = 0;

	std::set<int>::iterator it;
	int* pnNewIDs = NULL;

	LockObject();

	pnNewIDs = new int[setQueryAccountID.size()+1];
	memset(pnNewIDs, 0, (setQueryAccountID.size()+1)*sizeof(int));

	// 执行按帐号ID订阅
	for(it = setQueryAccountID.begin(); 
			it != setQueryAccountID.end(); it++) {
		pnNewIDs[nIndex] = *it;
		nIndex++;
		// 这里实际需要的是按帐号进行订阅退订
	}
	CTcpLayer::SendData(Cmd_RM_SubscribeTrade_Req, (void*)pnNewIDs, nIndex*sizeof(int), 0);
	delete pnNewIDs;
	pnNewIDs = NULL;
	UnlockObject();
	return true;

#else

	int nReqID = 0;
	int nIndex = 0;

	std::set<int>::iterator it;
	int* pnNewIDs = NULL;

	LockObject();

	pnNewIDs = new int[setQueryAccountID.size()+1];
	memset(pnNewIDs, 0, (setQueryAccountID.size()+1)*sizeof(int));

	// 执行按帐号ID订阅
	for(it = setQueryAccountID.begin(); 
			it != setQueryAccountID.end(); it++) {
		pnNewIDs[nIndex] = *it;
		nIndex++;
		// 这里实际需要的是按帐号进行订阅退订
	}
	CTcpLayer::SendData(Cmd_RM_SubscribeTrade_Req, (void*)pnNewIDs, nIndex*sizeof(int), 0);
	UnlockObject();
	return true;
#endif
}

void Client::CDlgRealTimeTradeInfoData::ResetQuery()
{
	 LockObject();
	 mapKey2Index.clear();
	 mapIndex2Key.clear();
	 mapTradeInfo.clear();
	 UnlockObject();

	 std::set<int>::iterator it;
	 int nAccountID = 0;
	 
	 LockObject();
	 // 先对原来的订阅进行退订
	 for(it = setQueryAccountID.begin(); 
			it != setQueryAccountID.end(); it++) {
		 nAccountID = *it;
		 // 这里实际需要的是按帐号进行订阅退订
		 CTcpLayer::SendData(Cmd_RM_UnSubscribeTrade_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
	 }
	 setQueryAccountID.clear();
	 UnlockObject();
}

void Client::CDlgRealTimeTradeInfoData::CheckNewData(CDataInfo* pdataInfo, 
								std::queue<SequencialTradeField>& queueRet)
{
	if(pdataInfo==NULL)
		return;

	int nIndex = -1;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);
	LockObject();
	while(!queueTradeInfo.empty()) {
		SequencialTradeField field = queueTradeInfo.front();
		queueTradeInfo.pop();
		if(bIsGroupQuery) {
			if(setFilterInstrumentID.size()>0) {
				bool bFound = false;
				InstrumentField instrument;
				memset(&instrument, 0, sizeof(instrument));
				bFound = pdataInfo->GetInstrumentFieldByInstrumentID(
						std::string(field.InstrumentID), instrument);
				if(bFound) {
					std::string strProductID(instrument.ProductID);
					if(setFilterInstrumentID.find(field.InstrumentID)==setFilterInstrumentID.end()
							&&setFilterInstrumentID.find(strProductID)==setFilterInstrumentID.end()) {
						continue;
					}
				}
			}

			if(setFilterFinanProductID.size()>0) {
				TrustTradeAccount traderAccount;
				memset(&traderAccount, 0, sizeof(traderAccount));
				if(pdataInfo->GetTrustTradeAccount(std::string(field.InvestorID), traderAccount)) {
					if(setFilterFinanProductID.find(traderAccount.nFinancialProductID)==setFilterFinanProductID.end())
						continue;
				}
			}
		}

		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%s, %s, %c", 
			field.ExchangeID, field.TradeID, field.Direction);
		strKey = strTemp;
		mapTradeInfo[strKey] = field;

		queueRet.push(field);

	}
	UnlockObject();
}