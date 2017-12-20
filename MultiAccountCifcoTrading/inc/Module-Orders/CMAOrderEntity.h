#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"
#include "CSAOrderEntity.h"
#include "..\inc\MultiAccountCore\MultiAccountCore.h"

/*
1.保存原始报单信息
2.转换成实际报单信息
3.一个原始报单对应一个实例
*/
class CMAOrderEntity
{
public:
	enum {	conNothing };
	//		conOpenDefaultAndStrategy=1,	// 按默认手数和策略开仓
	//		conCloseAll,					// 按持仓全平
	//		conAutoCloseDefaultAndNotOpen,	// 有仓账户按默认手数和策略平仓，超出不处理；无仓帐号不开仓
	//		conAutoReverseOpenAndNotOpen,	// 有仓账户按默认手数和策略平仓，超出反向开仓；无仓帐号不开仓
	//		conAutoCloseDefaultAndOpen,		// 有仓账户按默认手数和策略平仓，超出不处理；无仓帐号开仓
	//		conAutoReverseOpenAndOpen,		// 有仓账户按默认手数和策略平仓，超出反向开仓；无仓帐号开仓
	//		conAutoCloseDefault,			// 有仓账户按默认手数和策略平仓，超出不处理
	//		conAutoReverseOpen				// 有仓账户按默认手数和策略平仓，超出反向开仓
	//};
	enum {	conWaiting, conHasDone, conIsSuccess, conIsFail, conIsCancel };

	typedef struct {
		int nOperType;
		BOOL bNeedCancel;
		BOOL bIsQuotOrder;
		int nMouseNotReversed;
		int nMousePartReversed;
		int nMouseAllReversed;
		int nPositionClose;
	} MAAUTOOPER, *PMAAUTOOPER;

public:
	// bPreClose: 是否按可平仓数处理
	static CMAOrderEntity* CreateMAOrderEntity(StrategyItem enmStraNum, 
			COrderAtomEntity::STDORDERREQ& stdReq, CSAOrderEntity::OPERPARAM& operParam, 
			MAAUTOOPER& maOper, map<string,bool>& mapAccounts);

	static void ConvertToUIItem(COrderAtomEntity::STDORDERREQ& stdReq, 
								OrderInsertUIItem& orderUIItem);
	// 返回值: 0 无反向持仓，1 部分有反向持仓，2 全部有反向持仓
	static int AccountsPositionMode(map<string, bool>& mapAccounts, 
								COrderAtomEntity::STDORDERREQ& stdReq, 
								BOOL bNeedCancel);
	static BOOL FilterAccount(int nMaOper, BOOL bNeedCancel, 
								COrderAtomEntity::STDORDERREQ& stdReq, 
								map<string, bool>& mapAccounts);
	static BOOL CalcMAStrategy(StrategyItem enmStraNum, 
								map<string, bool>& mapAccounts, 
								COrderAtomEntity::STDORDERREQ& stdReq, 
								map<string, OrderInsertUIItem>& mapAccntVolume,
								vector<OrderInsertUIItem>& arrayAccountVolume);

	~CMAOrderEntity();

protected:
	CMAOrderEntity(StrategyItem enmStraNum, COrderAtomEntity::STDORDERREQ& stdReq, 
			CSAOrderEntity::OPERPARAM& operParam, 
			MAAUTOOPER& maOper, map<string,bool>& mapAccounts);

	BOOL MakeSAEntity(MAAUTOOPER& maOper);
	BOOL ConvertMA2SA(string& strUserID, string& strAccount, 
			int nDefaultVolume, BOOL bNeedCancel, int nOperType, int nMaOper, BOOL bHoldingVolume, 
			COrderAtomEntity::STDORDERREQ& maOriReq, COrderAtomEntity::STDORDERREQ& saOriReq, 
			CSAOrderEntity::OPERPARAM& maOperParam, CSAOrderEntity::OPERPARAM& saOperParam);

public:

	BOOL IsActive() {
		return m_bIsActive;
	};
	void ResetActive(BOOL bActive) {
		m_bIsActive = bActive;
	};
	BOOL IsDisorder_Insert() {
		return m_bDisorder_Insert;
	};
	void SetDisorder_Insert(BOOL bHas) {
		m_bDisorder_Insert = bHas;
	};
	BOOL NeedCancelOrder() {
		return m_bNeedCancelOrder;
		//return m_maOper.bNeedCancel;
	};
	int Status_DoCancel() {
		return m_nDoCancelStatus;
	};
	int Status_DoInsert() {
		return m_nDoInsertStatus;
	};
	StrategyItem GetStrategyItem() {
		return m_enmStraNum;
	};
	COrderAtomEntity::STDORDERREQ& GetStdOrderReq() {
		return m_stdOrderReq;
	};
	CSAOrderEntity::OPERPARAM& GetOperParam() {
		return m_operParam;
	};
	MAAUTOOPER& GetMaAutoOper() {
		return m_maOper;
	};
	int GetSAOrderEntityCount() {
		return m_mapSAOrder.size();
	};
	map<CSAOrderEntity*, OrderInsertUIItem>& GetSAOrderEntities() {
		return m_mapSAOrder;
	};
	BOOL IsRevokeAllOK() {
		BOOL bIsOK = TRUE;
		map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = m_mapSAOrder.begin();
		for(; it!=m_mapSAOrder.end(); it++) {
			if(it->first!=NULL)
				bIsOK = bIsOK && it->first->IsRevokeAllOK();
		}
		return bIsOK;
	};
	BOOL GetVolumeSum(int& nSumOpenVolume, int& nSumTodayVolume, int& nSumYdVolume);
	int SumCanCancel();

	BOOL MarkOrderActionStatus(DataRtnOrder& field);
	BOOL FindOrderAction(DataRspOrderAction2& act);

	BOOL DoOrderReq(int& nErrPos, string& strRetMsg, BOOL bNoSendZero=TRUE);
	BOOL DoRevokeAction(int& nErrPos, string& strRetMsg);
	BOOL Cancel(int& nCancelSum, string& strRetMsg);

	BOOL ResetParkedStatus(DataRspParkedOrderInsert& field);
	BOOL ResetOrderStatus(DataRtnOrder& field);
	BOOL ResetOrderStatus(DataRspOrderInsert& field);
	BOOL ResetActionStatus(DataRspOrderAction1& field);
	BOOL ResetActionStatus(DataRspOrderAction2& field);

	void ChangePrice(BOOL bIsMarket, double fPrice);

	string GetOrderDesciption();
	string GetRevokeOrderDesciption();

public:
	BOOL				m_bIsOverTime;
	DWORD				m_dwStartTick;

private:

	BOOL				m_bIsActive;
	BOOL				m_bNeedCancelOrder;
	int					m_nDoCancelStatus;
	int					m_nDoInsertStatus;

	BOOL				m_bDisorder_Insert;

	map<CSAOrderEntity*, OrderInsertUIItem> m_mapSAOrder;

	map<string, bool>				m_mapAccounts;
	StrategyItem					m_enmStraNum;
	COrderAtomEntity::STDORDERREQ	m_stdOrderReq;
	CSAOrderEntity::OPERPARAM		m_operParam;
	MAAUTOOPER						m_maOper;
	vector<CSAOrderEntity*>			m_vecAccountSeq;
};
