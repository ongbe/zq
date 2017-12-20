#include "StdAfx.h"
#include "CMAOrderEntity.h"
#include "../inc/Module-Misc/tools_util.h"
//#include "PositionDetailInfoList.h"               //去掉对CPositionBaseData的引用

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

//////////////////////////////////////////////////////////////////////////////////////


CMAOrderEntity::CMAOrderEntity(StrategyItem enmStraNum, 
							   COrderAtomEntity::STDORDERREQ& stdReq, 
							   CSAOrderEntity::OPERPARAM& operParam, 
							   MAAUTOOPER& maOper, 
							   map<string,bool>& mapAccounts)
{
	m_bIsOverTime = FALSE;
	m_dwStartTick = 0;
	m_bIsActive = FALSE;
	m_bDisorder_Insert = FALSE;
	m_bNeedCancelOrder = FALSE;
	m_nDoCancelStatus = conWaiting;
	m_nDoInsertStatus = conWaiting;
	m_enmStraNum = enmStraNum;
	m_stdOrderReq = stdReq;
	m_operParam = operParam;
	m_maOper = maOper;
	m_mapAccounts = mapAccounts;

//	m_nReverseTimer = 3;
}

CMAOrderEntity::~CMAOrderEntity()
{
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it != m_mapSAOrder.end(); it++) {
		if(it->first!=NULL)
			delete it->first;
	}
	m_mapSAOrder.clear();
}

CMAOrderEntity* CMAOrderEntity::CreateMAOrderEntity(StrategyItem enmStraNum, 
													COrderAtomEntity::STDORDERREQ& stdReq, 
													CSAOrderEntity::OPERPARAM& operParam, 
													MAAUTOOPER& maOper, 
													map<string,bool>& mapAccounts)
{
	CMAOrderEntity* pEntity = new CMAOrderEntity(enmStraNum, 
			stdReq, operParam, maOper, mapAccounts);
	if(pEntity->MakeSAEntity(maOper))
		return pEntity;
	else {
		delete pEntity;
		return NULL;
	}
}

void CMAOrderEntity::ConvertToUIItem(COrderAtomEntity::STDORDERREQ& stdReq, 
									 OrderInsertUIItem& orderUIItem)
{
	strcpy_s(orderUIItem.Account, sizeof(orderUIItem.Account)-1, stdReq.csUserID);
	strcpy_s(orderUIItem.InstrumentID, sizeof(orderUIItem.InstrumentID)-1, stdReq.csInstrumentID);
	orderUIItem.IsBuy = stdReq.bIsBuy==TRUE;							//买卖
	orderUIItem.IsHedge = stdReq.bIsHedge==TRUE;						//投保
	orderUIItem.IsMarket = stdReq.bIsMarket==TRUE;						//是否为市价
	orderUIItem.IsMarket_Trigger = stdReq.bTrigger_IsMarket==TRUE;		//触发价是否为市价
	orderUIItem.Price = stdReq.fPrice;									//价格
	orderUIItem.TriggerPrice = stdReq.fTrigger_Price;					//触发价格
	orderUIItem.Volume = stdReq.nVolume;								//手数
	orderUIItem.OCMode = stdReq.nOCMode;								//开平, 0 开仓, 1 平今, 2 平仓
}

// 返回值: 0 无反向持仓，1 部分有反向持仓，2 全部有反向持仓
int CMAOrderEntity::AccountsPositionMode(map<string, bool>& mapAccounts, 
										 COrderAtomEntity::STDORDERREQ& stdReq, 
										 BOOL bNeedCancel)
{
	string strUserID;
	int nSumVolume = 0, nSumTodayVolume = 0;
	int nPosStatus = 0;
	BOOL bIsAll = TRUE;
	map<string, bool>::iterator itAccs = mapAccounts.begin();

	for(; itAccs!=mapAccounts.end(); itAccs++) {
		if(!itAccs->second)
			continue;
		strUserID = itAccs->first;
		if(bNeedCancel) 
			CSAOrderEntity::GetHoldingVolume(strUserID, 
					string(""), string(stdReq.csInstrumentID), 
					!stdReq.bIsBuy, stdReq.bIsHedge, 
					nSumVolume, nSumTodayVolume);
		else
			CSAOrderEntity::GetCanCloseVolume(strUserID, 
					string(""), string(stdReq.csInstrumentID), 
					!stdReq.bIsBuy, stdReq.bIsHedge, 
					nSumVolume, nSumTodayVolume);
		if(nSumVolume>0) 
			nPosStatus = 1;
		else 
			bIsAll = FALSE;
	}
	if(bIsAll && nPosStatus==1)
		nPosStatus = 2;

	return nPosStatus;
}

BOOL CMAOrderEntity::FilterAccount(int nMaOper, BOOL bNeedCancel, 
								   COrderAtomEntity::STDORDERREQ& stdReq, 
								   map<string, bool>& mapAccounts)
{
	string strUserID;
	string strAccount;
	string strInstrumentID(stdReq.csInstrumentID);

	map<string, bool>::iterator it;
	int nSumVolume=0, nSumTodayVolume=0;

	switch(nMaOper) {
	case QOrder_CloseAll:					// 按持仓全平
		//// 按持仓全部，可以都设置为false，让一个都不参与多帐号计算
		//for(it=mapAccounts.begin(); it!=mapAccounts.end(); it++)
		//	it->second=false;
		//return TRUE;
		//break;
	case QOrder_AutoCloseDefaultAndNotOpen:	// 有仓账户按默认手数和策略平仓，超出不处理；无仓帐号不开仓
	case QOrder_AutoReverseOpenAndNotOpen:	// 有仓账户按默认手数和策略平仓，超出反向开仓；无仓帐号不开仓
	{
		for(it=mapAccounts.begin(); it!=mapAccounts.end(); it++) {
			nSumVolume = 0;
			nSumTodayVolume = 0;
			strUserID = it->first;
			if(it->second) {
				// 当且仅当原来为true被选择的才判断是否要参与多帐号计算
				// 原来为false就已经表明不参与多帐号计算了
				if(bNeedCancel) 
					CSAOrderEntity::GetHoldingVolume(strUserID, 
							string(""), string(stdReq.csInstrumentID), 
							!stdReq.bIsBuy, stdReq.bIsHedge, 
							nSumVolume, nSumTodayVolume);
				else
					CSAOrderEntity::GetCanCloseVolume(strUserID, 
							string(""), string(stdReq.csInstrumentID), 
							!stdReq.bIsBuy, stdReq.bIsHedge, 
							nSumVolume, nSumTodayVolume);
				it->second = nSumVolume>0;
			}
		}
	}
		return TRUE;
		break;
	case QOrder_OpenDefaultAndStrategy:		// 按默认手数和策略开仓
	case QOrder_AutoCloseDefaultAndOpen:	// 有仓账户按默认手数和策略平仓，超出不处理；无仓帐号开仓
	case QOrder_AutoReverseOpenAndOpen:		// 有仓账户按默认手数和策略平仓，超出反向开仓；无仓帐号开仓
	case QOrder_AutoCloseDefault:
	case QOrder_AutoReverseOpen:
	default:
		// 不用改变选项状态
		return TRUE;
		break;
	};
}

BOOL CMAOrderEntity::CalcMAStrategy(StrategyItem enmStraNum, 
									map<string, bool>& mapAccounts, 
									COrderAtomEntity::STDORDERREQ& stdReq, 
									map<string, OrderInsertUIItem>& mapAccntVolume, 
									vector<OrderInsertUIItem>& arrayAccountVolume)
{

	CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();
	if(pMACore==NULL)
		return FALSE;

	map<string, bool>::iterator it;
	map<string, OrderInsertUIItem>::iterator itVol;
	OrderInsertUIItem orderUIItem;

	ZeroMemory(&orderUIItem, sizeof(orderUIItem));
	ConvertToUIItem(stdReq, orderUIItem);

	pMACore->GetOrderInsertStrategy(arrayAccountVolume, orderUIItem, 
			enmStraNum, &mapAccounts);
	
	mapAccntVolume.clear();
	for(it = mapAccounts.begin(); it != mapAccounts.end(); it++) {
		strcpy_s(orderUIItem.Account, sizeof(orderUIItem.Account)-1, it->first.c_str());
		orderUIItem.Volume = 0;
		mapAccntVolume.insert(std::pair<string, OrderInsertUIItem>(
				it->first, orderUIItem));
	}
	for(UINT i=0; i<arrayAccountVolume.size(); i++) {
		itVol = mapAccntVolume.find(string(arrayAccountVolume[i].Account));
		if(itVol != mapAccntVolume.end()) 
			itVol->second = arrayAccountVolume[i];
	}

	return TRUE;
}

BOOL CMAOrderEntity::MakeSAEntity(MAAUTOOPER& maOper)
{
	string strAccount;
	int nMaOper = conNothing;
	map<string, bool> mapAccounts;
	map<string, OrderInsertUIItem> mapAccountsVolume;
	map<string, bool>::iterator itAccs;
	map<string, OrderInsertUIItem>::iterator itVol;
	vector<OrderInsertUIItem> arrayAccountVolume;
	CSAOrderEntity* pEntity = NULL;
	COrderAtomEntity::STDORDERREQ maStdReq;
	COrderAtomEntity::STDORDERREQ saStdReq;
	CSAOrderEntity::OPERPARAM saOperParam;

	// 1.  计算仓位状态，是无反向持仓，还是部分无反向持仓，还是全部有反向持仓
	//     并取得对应的快捷下单配置项
	
	mapAccounts = m_mapAccounts;

	if(maOper.nOperType == CSAOrderEntity::conOperAuto) {
		if(maOper.bIsQuotOrder) {
			int nPosStatus = AccountsPositionMode(mapAccounts, m_stdOrderReq, m_maOper.bNeedCancel);
			switch(nPosStatus) {
			case 0:
				nMaOper = maOper.nMouseNotReversed;
				break;
			case 1:
				nMaOper = maOper.nMousePartReversed;
				break;
			case 2:
				nMaOper = maOper.nMouseAllReversed;
				break;
			default:
				nMaOper = conNothing;
				return FALSE;
				break;
			};
		}
		else {
			nMaOper = maOper.nPositionClose;
		}
	}

	// 2.  过滤不需要参与多帐号策略计算的帐号
	FilterAccount(nMaOper, maOper.bNeedCancel, m_stdOrderReq, mapAccounts);
	
	// 3.  多帐号策略计算
	CalcMAStrategy(m_enmStraNum, mapAccounts, m_stdOrderReq, mapAccountsVolume, arrayAccountVolume);

	int nSumVolume=0, nTodayVolume=0;

	m_mapSAOrder.clear();

	//for(itVol = mapAccountsVolume.begin(); itVol != mapAccountsVolume.end(); itVol++) {
	for(UINT i=0; i<arrayAccountVolume.size(); i++) {
		itVol = mapAccountsVolume.find(string(arrayAccountVolume[i].Account));
		if(itVol == mapAccountsVolume.end()) 
			continue;

		ZeroMemory(&saStdReq, sizeof(saStdReq));
		maStdReq = m_stdOrderReq;
		maStdReq.nVolume = itVol->second.Volume;

		nSumVolume=0, nTodayVolume=0;
		if(maOper.bNeedCancel) 
			CSAOrderEntity::GetHoldingVolume(string(itVol->second.Account), 
					strAccount, string(maStdReq.csInstrumentID), 
					!maStdReq.bIsBuy, maStdReq.bIsHedge, nSumVolume, nTodayVolume);
		else
			CSAOrderEntity::GetCanCloseVolume(string(itVol->second.Account), 
					strAccount, string(maStdReq.csInstrumentID), 
					!maStdReq.bIsBuy, maStdReq.bIsHedge, nSumVolume, nTodayVolume);

		if(ConvertMA2SA(string(itVol->second.Account), strAccount, 
				itVol->second.Volume, maOper.bNeedCancel, maOper.nOperType, nMaOper, 
				nSumVolume>0, maStdReq, saStdReq, m_operParam, saOperParam)) {
			strcpy_s(saStdReq.csUserID, sizeof(saStdReq.csUserID)-1, 
					itVol->second.Account);
			//strcpy_s(saStdReq.csAccount, sizeof(saStdReq.csAccount)-1, 
			//	itVol->second.SubAccount);
			pEntity = CSAOrderEntity::CreateSAOrderEntity(saStdReq, saOperParam);
			if(pEntity!=NULL) {
				m_mapSAOrder.insert(
						std::pair<CSAOrderEntity*, OrderInsertUIItem>(
						pEntity, itVol->second));
				if(pEntity->NeedCancelOrder())
					m_bNeedCancelOrder = TRUE;
				m_vecAccountSeq.push_back(pEntity);
			}
		}
	}
	return TRUE;
}

BOOL CMAOrderEntity::ConvertMA2SA(string& strUserID, string& strAccount, int nDefaultVolume, 
								  BOOL bNeedCancel, int nOperType, int nMaOper, BOOL bHoldingVolume, 
								  COrderAtomEntity::STDORDERREQ& maStdReq, 
								  COrderAtomEntity::STDORDERREQ& saStdReq,
								  CSAOrderEntity::OPERPARAM& maOperParam, 
								  CSAOrderEntity::OPERPARAM& saOperParam)
{
	string strInstrumentID(maStdReq.csInstrumentID);

	saStdReq = maStdReq;
	saOperParam = maOperParam;
	saOperParam.bNeedCancel = bNeedCancel;
	saOperParam.bIsCanClose = !bNeedCancel;
	if(nOperType==CSAOrderEntity::conOperNoneAuto) {
		saOperParam.nOperType=CSAOrderEntity::conOperNoneAuto;
		//saOperParam.nDefaultVolume = nDefaultVolume;
		//saStdReq.nVolume = nDefaultVolume;
	}
	else if(nOperType==CSAOrderEntity::conOperAuto) {
		switch(nMaOper) {
		case QOrder_CloseAll:					// 按持仓全平
			if(bHoldingVolume) {
				saOperParam.nOperType = CSAOrderEntity::conOperAuto;
				saOperParam.nAutoMode = CSAOrderEntity::conAutoCloseAll;
			}
			else {
				saOperParam.nOperType = CSAOrderEntity::conOperNoneAuto;
				saOperParam.nAutoMode = -1;
				saOperParam.nDefaultVolume = 0;
				saStdReq.nOCMode = conOCOpen;
				saStdReq.nVolume = 0;
			}
			break;
		case QOrder_OpenDefaultAndStrategy:		// 按默认手数和策略开仓
			saOperParam.nOperType = CSAOrderEntity::conOperNoneAuto;
			saOperParam.nAutoMode = -1;
			saOperParam.nDefaultVolume = nDefaultVolume;
			saStdReq.nOCMode = conOCUnknow;
			break;
		case QOrder_AutoCloseDefaultAndNotOpen:	// 有仓账户按默认手数和策略平仓，超出不处理；无仓帐号不开仓
		case QOrder_AutoReverseOpenAndNotOpen:	// 有仓账户按默认手数和策略平仓，超出反向开仓；无仓帐号不开仓
		case QOrder_AutoCloseDefault:
		{
			if(bHoldingVolume) {
				saOperParam.nOperType = CSAOrderEntity::conOperAuto;
				saOperParam.nAutoMode = (nMaOper==QOrder_AutoReverseOpenAndNotOpen
					? CSAOrderEntity::conAutoReverseOpen 
					: CSAOrderEntity::conAutoCloseDefault);
				saOperParam.nDefaultVolume = nDefaultVolume;
				saStdReq.nOCMode = conOCUnknow;
				saStdReq.nVolume = 0;
			}
			else {
				saOperParam.nOperType = CSAOrderEntity::conOperNoneAuto;
				saOperParam.nAutoMode = -1;
				saOperParam.nDefaultVolume = 0;
				saStdReq.nOCMode = conOCOpen;
				saStdReq.nVolume = 0;
			}
		}
			break;
		case QOrder_AutoCloseDefaultAndOpen:	// 有仓账户按默认手数和策略平仓，超出不处理；无仓帐号开仓
		case QOrder_AutoReverseOpenAndOpen:		// 有仓账户按默认手数和策略平仓，超出反向开仓；无仓帐号开仓
		case QOrder_AutoReverseOpen:
		{
			saOperParam.nDefaultVolume = nDefaultVolume;
			if(bHoldingVolume) {
				saOperParam.nOperType = CSAOrderEntity::conOperAuto;
				saOperParam.nAutoMode = (nMaOper==QOrder_AutoCloseDefaultAndOpen
						? CSAOrderEntity::conAutoCloseDefault 
						: CSAOrderEntity::conAutoReverseOpen);
				saStdReq.nVolume = 0;
			}
			else {
				saOperParam.nOperType = CSAOrderEntity::conOperNoneAuto;
				saOperParam.nAutoMode = -1;
				saStdReq.nOCMode = conOCOpen;
				saStdReq.nVolume = nDefaultVolume;
			}
		}
			break;
		default:
			return FALSE;
			break;
		};
	}
	return TRUE;
}

BOOL CMAOrderEntity::GetVolumeSum(int& nSumOpenVolume, int& nSumTodayVolume, int& nSumYdVolume)
{
	if(m_mapSAOrder.size()==0)
		return FALSE;

	nSumOpenVolume = nSumTodayVolume = nSumYdVolume = 0;
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	int nSAReqCount = 0;
	COrderAtomEntity::STDORDERREQ stdReq = {0};
	for(; it!=m_mapSAOrder.end(); it++) {
		if(it->first!=NULL) {
			nSAReqCount = it->first->GetOrderReqCount();
			for(int i=0; i<nSAReqCount; i++) {
				stdReq = it->first->GetOrderReq(i)->GetStdOrderReq();
				switch(stdReq.nOCMode) {
				case conOCClodeToday:
					nSumTodayVolume+=stdReq.nVolume;
					break;
				case conOCClose:
					nSumYdVolume+=stdReq.nVolume;
					break;
				case conOCUnknow:
				case conOCOpen:
				default:
					nSumOpenVolume+=stdReq.nVolume;
					break;
				};
			}
		}
	}
	return TRUE;
}

int CMAOrderEntity::SumCanCancel()
{
	int nSum = 0;
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = 
			m_mapSAOrder.begin();
	for(; it != m_mapSAOrder.end(); it++)
		nSum += it->first->SumCanCancel();
	return nSum;
}

BOOL CMAOrderEntity::DoOrderReq(int& nErrPos, std::string& strRetMsg, BOOL bNoSendZero)
{
	if(m_nDoInsertStatus!=conWaiting)
		return FALSE;
	
	m_nDoInsertStatus = conHasDone;

	int nRet = -1;
	string strMsg;

	nErrPos = -1;
	strRetMsg="";
	
    //此处需要考虑是否需要乱序下单
    //GlobalConfigManager::IsDisorderWhenInsertOrder_MultiAccount()

	vector<CSAOrderEntity*> newSeq;
	
	newSeq = m_vecAccountSeq;
	if(m_bDisorder_Insert) {
		::srand(::GetTickCount());
		std::random_shuffle(newSeq.begin(), newSeq.end());
	}

	//map<CSAOrderEntity*, OrderInsertUIItem>::iterator it;
	//for(; it!=m_mapSAOrder.end(); ++it) {
	vector<CSAOrderEntity*>::iterator it;
	for(it = newSeq.begin(); it != newSeq.end(); it++) {
		if(m_mapSAOrder.find(*it)==m_mapSAOrder.end())
			continue;
		if(*it != NULL) {
			if(!(*it)->DoOrderReq(nErrPos, strMsg, bNoSendZero))
				m_nDoInsertStatus = conIsFail;
			strRetMsg+=strMsg;
		}
	}
	strRetMsg = LOADSTRING(ORDER_IE_CHANGEOK);//wxT("下单成功！\n");
	return TRUE;
}

BOOL CMAOrderEntity::DoRevokeAction(int& nErrPos, string& strRetMsg)
{
	if(m_nDoCancelStatus!=conWaiting)
		return FALSE;
	
	m_nDoCancelStatus = conHasDone;

	int nRet = -1;
	string strMsg;

	nErrPos = -1;
	strRetMsg="";
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			if(!it->first->DoRevokeAction(nErrPos, strMsg))
				m_nDoCancelStatus = conIsFail;
			strRetMsg+=strMsg;
		}
	}
	strRetMsg = LOADSTRING(ORDER_IE_CHANGEOK);//wxT("下单成功！\n");
	return TRUE;
}

BOOL CMAOrderEntity::Cancel(int& nCancelSum, string& strRetMsg)
{
	if(m_nDoInsertStatus==conWaiting)
		return FALSE;
	
	m_nDoInsertStatus = conIsCancel;

	int nRet = -1;
	string strMsg;

	nCancelSum = 0;
	strRetMsg="";
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->Cancel(nCancelSum, strMsg);
			//if(!it->first->Cancel(nCancelSum, strMsg))
			//	m_nDoCancelStatus = m_nDoCancelStatus;
			//	//m_nDoCancelStatus = conIsFail;
			strRetMsg+=strMsg;
		}
	}
	strRetMsg = LOADSTRING(ORDER_IE_CHANGEOK);//wxT("下单成功！\n");
	return TRUE;
}

BOOL CMAOrderEntity::ResetParkedStatus(DataRspParkedOrderInsert& field)
{
	if(m_nDoInsertStatus!=conHasDone)
		return FALSE;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->ResetParkedStatus(field);
		}
	}
	return TRUE;
}

BOOL CMAOrderEntity::ResetOrderStatus(DataRtnOrder& field)
{
	if(m_nDoInsertStatus==conWaiting)
		return FALSE;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->ResetOrderStatus(field);
		}
	}
	return TRUE;
}

BOOL CMAOrderEntity::ResetOrderStatus(DataRspOrderInsert& field)
{
	if(m_nDoInsertStatus==conWaiting)
		return FALSE;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->ResetOrderStatus(field);
		}
	}
	return TRUE;
}

BOOL CMAOrderEntity::ResetActionStatus(DataRspOrderAction1& field)
{
	if(m_nDoInsertStatus==conWaiting)
		return FALSE;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->ResetActionStatus(field);
		}
	}
	return TRUE;
}

BOOL CMAOrderEntity::ResetActionStatus(DataRspOrderAction2& field)
{
	if(m_nDoInsertStatus==conWaiting)
		return FALSE;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->ResetActionStatus(field);
		}
	}
	return TRUE;
}

BOOL CMAOrderEntity::MarkOrderActionStatus(DataRtnOrder& field)
{
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			if(it->first->MarkOrderActionStatus(field))
				return TRUE;
		}
	}	
	return FALSE;
}

BOOL CMAOrderEntity::FindOrderAction(DataRspOrderAction2& act)
{
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			if(it->first->FindOrderAction(act))
				return TRUE;
		}
	}	
	return FALSE;
}

void CMAOrderEntity::ChangePrice(BOOL bIsMarket, double fPrice)
{
	if(m_nDoCancelStatus!=conWaiting)
		return;

	m_stdOrderReq.bIsMarket = bIsMarket;
	m_stdOrderReq.fPrice = fPrice;

	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			it->first->ChangePrice(bIsMarket, fPrice);
		}
	}	
}

std::string CMAOrderEntity::GetOrderDesciption()
{
	std::string strText;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			strText+=it->first->GetOrderDesciption();
			strText+="\n";
		}
	}
	return strText;
}

std::string CMAOrderEntity::GetRevokeOrderDesciption()
{
	std::string strText;
	
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
	for(; it!=m_mapSAOrder.end(); ++it) {
		if(it->first != NULL) {
			strText+=it->first->GetRevokeOrderDesciption();
			strText+="\n";
		}
	}
	return strText;
}
