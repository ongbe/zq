#include "StdAfx.h"
#include "CDlgRealTimePositionInfoData.h"

#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "tools_util.h"

bool Client::CDlgRealTimePositionInfoData::NewQuery()
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
	CTcpLayer::SendData(Cmd_RM_SubscribePosition_Req, (void*)pnNewIDs, nIndex*sizeof(int), 0);
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
		CTcpLayer::SendData(Cmd_RM_SubscribePosition_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
	}
	UnlockObject();
	return true;
#endif
}

void Client::CDlgRealTimePositionInfoData::ResetQuery()
{
	 LockObject();
	 mapKey2Index.clear();
	 mapIndex2Key.clear();
	 mapPositionInfo.clear();
	 UnlockObject();

	 int nAccountID = 0;
	 
	 LockObject();
	 {
	 // 先对原来的订阅进行退订
	 std::set<int>::iterator it;
	 for(it = setQueryAccountID.begin(); 
			it != setQueryAccountID.end(); it++) {
		 nAccountID = *it;
		 // 这里实际需要的是按帐号进行订阅退订
		 CTcpLayer::SendData(Cmd_RM_UnSubscribePosition_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
	 }
	 setQueryAccountID.clear();
	 }
	 {
	 // 先对原来的订阅进行退订
	 std::set<std::string>::iterator it;
	 TInstrumentIDType instID;
	 for(it = setHasQuotInstrumentID.begin(); 
			it != setHasQuotInstrumentID.end(); it++) {
		memset(instID, 0, sizeof(instID));
		strncpy(instID, (*it).c_str(), sizeof(instID)-1);
		 // 这里实际需要的是按帐号进行订阅退订
		CTcpLayer::SendData(Cmd_RM_UnSubscribeQuot_Req, (void*)instID, sizeof(instID), 0);
	 }
	 setHasQuotInstrumentID.clear();
	 }
	 UnlockObject();
}

void Client::CDlgRealTimePositionInfoData::CheckNewData(CDataInfo* pdataInfo, 
								std::queue<SequencialPositionField>& queueRet)
{
	if(pdataInfo==NULL)
		return;

	std::map<string, int>::iterator it;
	int nIndex = -1;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);
	LockObject();
	while(!queuePositionInfo.empty()) {
		SequencialPositionField field = queuePositionInfo.front();
		queuePositionInfo.pop();
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
		sprintf(strTemp, "%s, %s, %c, %c", 
				field.InvestorID, field.InstrumentID, field.PosiDirection, field.HedgeFlag);
		strKey = strTemp;
		if(field.Position>0)
			mapPositionInfo[strKey] = field;
		else
			mapPositionInfo.erase(strKey);

		queueRet.push(field);

		bool bHasData = false;
		if(field.Position==0) {
			std::map<std::string, SequencialPositionField>::iterator itPos
					= mapPositionInfo.begin();
			for(; itPos!=mapPositionInfo.end(); itPos++) {
				if(strcmp(itPos->second.InstrumentID, field.InstrumentID)==0) {
					bHasData = true;
					break;
				}
			}
			if(!bHasData) {
				CTcpLayer::SendData(Cmd_RM_UnSubscribeQuot_Req, 
						(void*)field.InstrumentID, sizeof(field.InstrumentID), 0);
				setHasQuotInstrumentID.erase(std::string(field.InstrumentID));
			}
		}
		else {
			std::set<std::string>::iterator it = 
					setHasQuotInstrumentID.find(std::string(field.InstrumentID));
			if(it == setHasQuotInstrumentID.end()) {
				CTcpLayer::SendData(Cmd_RM_SubscribeQuot_Req, 
						(void*)field.InstrumentID, sizeof(field.InstrumentID), 0);
				setHasQuotInstrumentID.insert(std::string(field.InstrumentID));
			}
		}
	}
	UnlockObject();
}

void Client::CDlgRealTimePositionInfoData::CheckNewData_Quot(CDataInfo* pdataInfo, 
								std::queue<SequencialPositionField>& queueRet)
{
	if(pdataInfo==NULL)
		return;

	std::map<string, int>::iterator it;
	int nIndex = -1;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);
	LockObject();
	while(!queueQuot.empty()) {
		DepthMarketDataField field = queueQuot.front();
		queueQuot.pop();

		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%s", field.InstrumentID);
		strKey = strTemp;
		mapQuot[strKey] = field;

		RecalcPosition(pdataInfo, field, queueRet);
	}
	UnlockObject();
}

void Client::CDlgRealTimePositionInfoData::RecalcPosition(CDataInfo* pdataInfo, 
								DepthMarketDataField& quot, 
								std::queue<SequencialPositionField>& queueRet)
{
	if(pdataInfo==NULL)
		return;
	if(util::isInvalidValue(quot.LastPrice))
		return;

	std::map<std::string, SequencialPositionField>::iterator it;
	InstrumentField instField;
	memset(&instField, 0, sizeof(instField));
	if(!pdataInfo->GetInstrumentFieldByInstrumentID(std::string(quot.InstrumentID), instField))
		return;
	LockObject();
	for(it = mapPositionInfo.begin(); it != mapPositionInfo.end(); it++) {
		if(strcmp(it->second.InstrumentID, quot.InstrumentID)==0) {
			it->second.PositionProfit = (quot.LastPrice - it->second.PositionCost/
					(it->second.Position*instField.VolumeMultiple))
					*instField.VolumeMultiple*it->second.Position;
			it->second.PositionProfit *= (it->second.PosiDirection==T_PD_Long?1.0:-1.0);
#ifdef _DEBUG			
			// temp code
			it->second.PreSettlementPrice = quot.LastPrice;
#endif
			queueRet.push(it->second);
		}
	}
	UnlockObject();
}
