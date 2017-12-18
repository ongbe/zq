#include "StdAfx.h"
#include "DlgRealTimeOrderInfo.h"

#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"

bool Client::DlgRealTimeOrderInfoData::NewQuery()
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
	CTcpLayer::SendData(Cmd_RM_SubscribeOrder_Req, (void*)pnNewIDs, nIndex*sizeof(int), 0);
	delete pnNewIDs;
	pnNewIDs = NULL;
	UnlockObject();
	return true;

#else

	int nReqID = 0;
	int nAccountID = 0;

	std::set<int>::iterator it;

	LockObject();

	// 执行按帐号ID订阅
	for(it = setQueryAccountID.begin(); 
			it != setQueryAccountID.end(); it++) {
		nAccountID = *it;
		// 这里实际需要的是按帐号进行订阅退订
		CTcpLayer::SendData(Cmd_RM_SubscribeOrder_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
	}
	UnlockObject();
	return true;
#endif
}

void Client::DlgRealTimeOrderInfoData::ResetQuery()
{
	 LockObject();
	 mapKey2Index.clear();
	 mapIndex2Key.clear();
	 mapOrderInfo.clear();
	 UnlockObject();

	 std::set<int>::iterator it;
	 int nAccountID = 0;
	 
	 LockObject();
	 // 先对原来的订阅进行退订
	 for(it = setQueryAccountID.begin(); 
			it != setQueryAccountID.end(); it++) {
		 nAccountID = *it;
		 // 这里实际需要的是按帐号进行订阅退订
		 CTcpLayer::SendData(Cmd_RM_UnSubscribeOrder_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
	 }
	 setQueryAccountID.clear();
	 UnlockObject();
}

void Client::DlgRealTimeOrderInfoData::CheckNewData(CDataInfo* pdataInfo, 
								std::queue<SequencialOrderField>& queueRet)
{
	if(pdataInfo==NULL)
		return;

	std::map<string, int>::iterator it;
	int nIndex = -1;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);
	LockObject();
	while(!queueOrderInfo.empty()) {
		SequencialOrderField field = queueOrderInfo.front();
		queueOrderInfo.pop();
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
		sprintf(strTemp, "%s, %s, %x, %x", 
				field.InvestorID, field.OrderRef, field.FrontID, field.SessionID);
		strKey = strTemp;
		mapOrderInfo[strKey] = field;

		queueRet.push(field);

	}
	UnlockObject();
}