#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"
#include "COrderAtomEntity.h"

/*
1.保存原始报单信息
2.转换成实际报单信息
3.一个原始报单对应一个实例
*/
// CSAOrderEntity不再支持replace和CancelThenOrder，这些可以用COrderAtomEntity
class CSAOrderEntity
{
public:
	enum {	conMaxOrderReqNum = 10, conMaxOrderActionNum = 200 };
	enum {	conOperNoneAuto, conOperAuto };//, conOperReplace }; 
	enum {	conAutoCloseAll=1, conAutoCloseDefault, conAutoReverseOpen };
	enum {	conWaiting, conHasDone, conIsSuccess, conIsFail, conIsCancel };

	typedef struct {
		BOOL	bIsOpen;				// 是否需要开仓
		DWORD	dwVolume_Open;			// 开仓量
		double	fPrice_Open;			// 开仓价
		BOOL	bIsCloseToday;			// 是否需要平今
		DWORD	dwVolume_CloseToday;	// 平今量
		double	fPrice_CloseToday;		// 平今价
		BOOL	bIsClose;				// 是否需要平仓
		DWORD	dwVolume_Close;			// 平仓量
		double	fPrice_Close;			// 平仓价
		BOOL	bIsReverseOpen;			// 是否需要反向开仓
		DWORD	dwVolume_ReverseOpen;	// 反向开仓量
		double	fPrice_ReverseOpen;		// 反向开仓价
		BOOL	bIsRevoke;				// 是否已有平仓/平今挂单
		DWORD	dwVolume_Revoke;		// 平仓/平今挂单
	} ORDERREQBREAK, *PORDERREQBREAK;

	// 这个其实是给CSAOrderEntity用的，Create时作为一个标准参数使用
	// 另外，如果是简单的改单，如挂单板块中的改单功能，不使用CSAOrderEntity，而直接用COrderAtomEntity处理
	typedef struct {
		int		nOperType;				// 执行类型，0 非自动单，1 自动单，2 非自动改单
		int		nAutoMode;				// 自动单模式，0 按持仓量全平，1 按默认手数平仓，不反向开仓
										// 2 按默认手数平仓，超出部分反向开仓
		int		nDefaultVolume;			// 默认手数
		BOOL	bNeedCancel;			// 是否需要撤销相关报单
		BOOL	bIsCanClose;			// 是否按可平仓数处理
		int		nCutMode;				// 0 不分拆报单，1 按指定数分拆，2 按交易所最大报单数分拆
		int		nCutVolume;				// 指定的分拆数量
		BOOL	bSimulate;				// 是否进行市价模拟
		BOOL	bSimulate_Trigger;		// 是否进行市价模拟
		BOOL	bNoSendVolZoreOrder;	// 不发送手数为0的报单
	} OPERPARAM, *LPOPERPARAM;

	typedef struct  {
		PlatformStru_InputOrderAction action;	// 报单活动域
		PlatformStru_OrderInfo	orderRsp;		// 正常单域
		CThostFtdcRspInfoField	rspInfo;		// 应答结构
	} CMBORDERACTION, *PCMBORDERACTION;

public:
	// bPreClose: 是否按可平仓数处理
	static CSAOrderEntity* CreateSAOrderEntity(
			COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam);

	static BOOL AnalyseOrderReq(
			COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam, 
			ORDERREQBREAK& tOrderReqBreak);

	// 求持仓量
	static BOOL GetHoldingVolume(string& strUserID, string& strAccount, string& strCode, 
			BOOL bIsBuy, BOOL bIsHedge, int& nSumVolume, int& nSumTodayVolume);
	// 求可平数量
	static BOOL GetCanCloseVolume(string& strUserID, string& strAccount, string& strCode, 
			BOOL bIsBuy, BOOL bIsHedge, int& nCanCloseVolume, int& nCanCloseTodayVolume);
	// 求平仓挂单数量
	static BOOL GetPreCloseVolume(string& strUserID, string& strAccount, string& strCode, 
			BOOL bIsBuy, BOOL bIsHedge, int& nPreCloseVolume, int& nPreCloseTodayVolume);

	static BOOL HasTrade(const string& strUserID);

	~CSAOrderEntity();

protected:
	CSAOrderEntity(COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam, 
			ORDERREQBREAK& orderReqBreak);

//	BOOL InsertOrder(string& strUserID, string& strAccount, string& strInstrumentID, 
//			BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, BOOL bIsHedge, 
//			BOOL bIsMarketPrice, BOOL bSimulate, BOOL bIsParketOrder);
	BOOL InsertOrder(COrderAtomEntity::STDORDERREQ stdReq, 
			BOOL bIsBuy, int nOCMode, int nVolume);

	BOOL MakeRevokeActions(string& strUserID, string& strAccount, 
			string& strInstrumentID, BOOL bIsBuy, int nOCMode);
//	BOOL MakeRevokeAction_Replace(OrderKey& orderKey);

public:

	//void SetReverseTimer(int nReverseTimer) {
	//	m_nReverseTimer = nReverseTimer;
	//};
	//int GetReverseTimer() {
	//	return m_nReverseTimer;
	//};
	//void ReverseTimerSubOne() {
	//	if(m_nReverseTimer>0)
	//		m_nReverseTimer--;
	//};
	//BOOL IsOverdue() {
	//	return m_nReverseTimer<=0;
	//};

	BOOL NeedCancelOrder() {
		return m_nOrderActionCount>0;
		//return m_operParam.bNeedCancel;
	};
	int Status_DoCancel() {
		return m_nDoCancelStatus;
	};
	int Status_DoInsert() {
		return m_nDoInsertStatus;
	};
	int GetRunStatus(int nOCMode);

	string GetUserID() {
		return string(m_stdOrderReq.csUserID);
	};
	
	OPERPARAM& GetOperParam() {
		return m_operParam;
	};
	int GetOrderReqCount() {
		return m_arrOrderEntity.size();
	};
	COrderAtomEntity* GetOrderReq(int nIndex) {
		if(nIndex<0 || nIndex>=GetOrderReqCount())
			return NULL;
		else
			return m_arrOrderEntity[nIndex];
	};
	int GetNeedCancelVolume() {
		return m_nNeedCancelVolume;
	};
	int GetOrderActionCount() {
		return m_nOrderActionCount;
	};
	CMBORDERACTION* GetOrderAction(int nIndex) {
		if(nIndex<0 || nIndex>=m_nOrderActionCount)
			return NULL;
		else
			return m_tOrderActions+nIndex;
	};
	BOOL IsRevokeAllOK() {
		BOOL bIsOK = TRUE;
		for(int i=0; i<m_nOrderActionCount; i++) 
			bIsOK = bIsOK && m_bOrderActionsState[i];
		return bIsOK;
	};
	UINT GetVolumeSum() {
		UINT nCount = 0;
		if(m_orderReqBreak.bIsOpen)
			nCount += m_orderReqBreak.dwVolume_Open;
		if(m_orderReqBreak.bIsReverseOpen)
			nCount += m_orderReqBreak.dwVolume_ReverseOpen;
		if(m_orderReqBreak.bIsCloseToday)
			nCount += m_orderReqBreak.dwVolume_CloseToday;
		if(m_orderReqBreak.bIsClose)
			nCount += m_orderReqBreak.dwVolume_Close;
		return nCount;
	};
	BOOL GetVolumeSum(int& nSumOpenVolume, int& nSumTodayVolume, int& nSumYdVolume) {
		if(m_orderReqBreak.bIsOpen)
			nSumOpenVolume += m_orderReqBreak.dwVolume_Open;
		if(m_orderReqBreak.bIsReverseOpen)
			nSumOpenVolume += m_orderReqBreak.dwVolume_ReverseOpen;
		if(m_orderReqBreak.bIsCloseToday)
			nSumTodayVolume += m_orderReqBreak.dwVolume_CloseToday;
		if(m_orderReqBreak.bIsClose)
			nSumYdVolume += m_orderReqBreak.dwVolume_Close;
		return TRUE;
	};
	BOOL GetVolumeSum_Trade(int& nOpenVolume, int& nTodayVolume, int& nYdVolume);
	BOOL GetCancelVolumeSum(int& nWaitVolume, int& nRealVolume);

	int SumCanCancel();

	BOOL DoOrderReq(int& nErrPos, string& strRetMsg, BOOL bNoSendZero=FALSE);
	BOOL DoRevokeAction(int& nErrPos, string& strRetMsg);
	BOOL Cancel(int& nCancelSum, string& strRetMsg);

	void ChangePrice(BOOL bIsMarket, double fPrice);
	void ChangeTriggerCond(int nPriceType, int nCondType, 
			BOOL bIsMarket, double fPrice, BOOL bNeedChangeOrderType=TRUE);


	BOOL ResetParkedStatus(DataRspParkedOrderInsert& field);
	BOOL ResetOrderStatus(DataRtnOrder& field);
	BOOL ResetOrderStatus(DataRspOrderInsert& field);
	BOOL ResetActionStatus(DataRspOrderAction1& field);
	BOOL ResetActionStatus(DataRspOrderAction2& field);


	BOOL MarkOrderActionStatus(DataRtnOrder& field);
	BOOL FindOrderAction(DataRspOrderAction2& act);

	std::string GetOrderDesciption();
	std::string GetRevokeOrderDesciption() {
		return m_strRevokeMsg;
	};

private:

	int					m_nDoCancelStatus;
	int					m_nDoInsertStatus;
	string				m_strRevokeMsg;

//	std::vector<COrderAtomEntity*>	m_arrCancelEntity;
	std::vector<COrderAtomEntity*>	m_arrOrderEntity;

	COrderAtomEntity::STDORDERREQ	m_stdOrderReq;
	OPERPARAM						m_operParam;
	ORDERREQBREAK					m_orderReqBreak;


	//int					m_nOrderReqCount;
	//CMBORDER			m_tOrderReqs[conMaxOrderReqNum];

	int					m_nNeedCancelVolume;
	int					m_nOrderActionCount;
	CMBORDERACTION		m_tOrderActions[conMaxOrderActionNum];
	BOOL				m_bOrderActionsState[conMaxOrderActionNum];	// TRUE, 表示撤单成功

};
