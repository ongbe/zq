#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

/*
1.保存原始报单信息
2.转换成实际报单信息
3.一个原始报单对应一个实例
*/
class COrderInsertEntity
{
public:
	enum {	conMaxRspOrderReqNum = 10, conMaxFtdcOrderActionNum = 1000 };

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

	typedef struct {
		int		nOrderType;
		TThostFtdcInstrumentIDType	strInstrumentID;
		char	strAccount[32];
		BOOL	bIsParketOrder;
		BOOL	bDirection;
		int		nOCMode;                //0开仓; 1平今; 2平仓; -1未知
		double	fPrice;
		int		nVolume;
		int		nHedgeStatus;
		BOOL	bIsMarketPrice;
		BOOL	bSimulate;				// 是否用市价模拟
		BOOL	bIsAuto;
		int		nAutoMode;
		BOOL	bIsPreClose;			// 是否按可平仓数处理
		int		nDefaultVolume;
		int		nMinVolume;
		//int		nPriceOffset;		// 价格变动价位，价格:=fPriceX+nPriceOffset*fPriceTick
		//double	fPriceTick;
		//double	fAskPrice1;			// 卖一价
		//double	fLastPrice;			// 最新价
		//double	fBidPrice1;			// 买一价
	} ORIORDERREQ, *PORIORDERREQ;

	typedef struct 
    {
		BOOL					bIsParked;	// 是否为预埋单或正常报单
		union 
        {
			PlatformStru_ParkedOrder	parked;	// 预埋单域
			PlatformStru_InputOrder		insert;	// 正常单域
		} order;
		CThostFtdcRspInfoField	rspInfo;	// 应答结构
	} CMBORDER, *PCMBORDER;

	typedef struct  {
		//CThostFtdcInputOrderActionField action;	// 报单活动域
		PlatformStru_InputOrderAction action;	// 报单活动域
		CThostFtdcRspInfoField	rspInfo;	// 应答结构
	} CMBORDERACTION, *PCMBORDERACTION;

public:
	// bPreClose: 是否按可平仓数处理
	static COrderInsertEntity* CreateOrderInsert(ORIORDERREQ& tOriReq);

	static BOOL GetHoldingVolume(string strCode, BOOL bIsBuy, int nHedgeStatus, int& nSumVolume, int& nSumTodayVolume, string strAccount);
	static BOOL GetAskCloseVolume(string strCode, BOOL bIsBuy, int nHedgeStatus, int& nAskVolume, int& nAskTodayVolume, string strAccount);
	static BOOL GetPreVolume(string strCode, BOOL bIsBuy, int nHedgeStatus, int& nAskVolume, int& nAskTodayVolume, string strAccount);//可平数量

	static BOOL AnalyseOrderReq(ORIORDERREQ& tOriReq, ORDERREQBREAK& tOrderReqBreak);

	~COrderInsertEntity();

protected:
	COrderInsertEntity();

	BOOL InsertOrder(wxString& strInstrumentID, wxString& strAccount, 
			BOOL bDirection, int nOCMode, double fPrice, int nVolume, int nHedgeStatus, 
			BOOL bIsMarketPrice, BOOL bSimulate, BOOL bIsParketOrder, 
			int nOrderType, int nMinVolume);

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
	
	int GetOrderReqCount() {
		return m_nRspOrderReqCount;
	};
	CMBORDER& GetOrderReq(int nIndex) {
		return m_tRspOrderReqs[nIndex];
	};
	int GetFtdcOrderActionCount() {
		return m_nFtdcOrderActionCount;
	};
//	CThostFtdcInputOrderActionField& GetFtdcOrderAction(int nIndex) {
//		return m_tFtdcOrderActions[nIndex];
//	};
	CMBORDERACTION& GetFtdcOrderAction(int nIndex) {
		return m_tFtdcOrderActions[nIndex];
	};
	BOOL IsRevokeAllOK() {
		BOOL bIsOK = TRUE;
		for(int i=0; i<m_nFtdcOrderActionCount; i++) 
			bIsOK = bIsOK && m_bFtdcOrderActionsState[i];
		return bIsOK;
	};
	BOOL MarkRevokeItemState(DataRtnOrder& field);
	BOOL FindRspOrderAction(DataErrRtnOrderAction& act);

	//int GetOrderVolumeTotal() {
	//	int nVolume=0;
	//	for(int i=0; i<m_nRspOrderReqCount;i++) {
	//		nVolume += m_tRspOrderReqs[i].InputOrderField.VolumeTotalOriginal;
	//	}
	//	return nVolume;
	//};

	BOOL DoOrder(int& nErrPos, wxString& strRetMsg);
	BOOL RevokeOrder(wxString& strInstrumentID, BOOL bDirection, int nOCMode, wxString& strRetMsg);
	BOOL DoRevokeOrder(int& nErrPos, wxString& strRetMsg);
	wxString GetOrderDesciption(int nViewStyle=3);
	wxString GetRevokeOrderDesciption(int nViewStyle=3) {
		return m_strRevokeMsg;
	};

	void LockForSetChange() {
		m_bIsLockForSetChange = TRUE;
	};
	void UnlockForSetChange() {
		m_bIsLockForSetChange = FALSE;
	};

private:

	BOOL				m_bIsAuto;
	BOOL				m_bIsParkedOrder;
	BOOL				m_bIsLockForSetChange;

	//int					m_nReverseTimer;

	int					m_nRspOrderReqCount;

	CMBORDER			m_tRspOrderReqs[conMaxRspOrderReqNum];

	wxString			m_strRevokeMsg;
	int					m_nFtdcOrderActionCount;
//	CThostFtdcInputOrderActionField		m_tFtdcOrderActions[conMaxFtdcOrderActionNum];
	CMBORDERACTION		m_tFtdcOrderActions[conMaxFtdcOrderActionNum];
	BOOL				m_bFtdcOrderActionsState[conMaxFtdcOrderActionNum];	// TRUE, 表示撤单成功

};
