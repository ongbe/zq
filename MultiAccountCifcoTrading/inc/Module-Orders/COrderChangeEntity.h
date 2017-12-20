#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

/*
1.保存原始报单信息
2.转换成实际报单信息
3.一个原始报单对应一个实例
*/
class COrderChangeEntity
{
public:
	enum {	conMaxRspOrderReqNum = 10, conMaxFtdcOrderActionNum = 1000 };

	typedef struct {
		TThostFtdcInstrumentIDType	strInstrumentID;
		char	strAccount[32];
		BOOL	bIsParketOrder;
		BOOL	bDirection;
		int		nOCMode;
		double	fPrice;
		int		nVolume;
		BOOL	bIsHedge;
		BOOL	bIsMarketPrice;
		BOOL	bSimulate;				// 是否用市价模拟
		BOOL	bIsAuto;
		int		nAutoMode;
		BOOL	bIsPreClose;			// 是否按可平仓数处理
		int		nDefaultVolume;
		//int		nPriceOffset;		// 价格变动价位，价格:=fPriceX+nPriceOffset*fPriceTick
		//double	fPriceTick;
		//double	fAskPrice1;			// 卖一价
		//double	fLastPrice;			// 最新价
		//double	fBidPrice1;			// 买一价
	} ORIORDERREQ, *PORIORDERREQ;

	typedef struct {
		PlatformStru_InputOrder		insert;		// 正常单域
		CThostFtdcRspInfoField		rspInfo;	// 应答结构
	} CMBORDERINSERT, *PCMBORDERINSERT;

public:
	// bPreClose: 是否按可平仓数处理
	static COrderChangeEntity* CreateOrderChange(ORIORDERREQ& tOriReq);

	~COrderChangeEntity();

protected:
	COrderChangeEntity();

	BOOL InsertOrder(wxString& strInstrumentID, wxString& strAccount, 
			BOOL bDirection, int nOCMode, double fPrice, int nVolume, BOOL bIsHedge, 
			BOOL bIsMarketPrice, BOOL bSimulate, BOOL bIsParketOrder);

public:

	int GetOrderReqCount() {
		return m_nRspOrderReqCount;
	};
	CMBORDERINSERT& GetOrderReq(int nIndex) {
		return m_tRspOrderReqs[nIndex];
	};
	int GetFtdcOrderActionCount() {
		return m_nFtdcOrderActionCount;
	};
	PlatformStru_InputOrderAction& GetFtdcOrderAction(int nIndex) {
		return m_tFtdcOrderActions[nIndex];
	};
	BOOL IsRevokeAllOK() {
		BOOL bIsOK = TRUE;
		for(int i=0; i<m_nFtdcOrderActionCount; i++) 
			bIsOK = bIsOK && m_bFtdcOrderActionsState[i];
		return bIsOK;
	};
	BOOL MarkRevokeItemState(DataRtnOrder& field);
	BOOL FindRspOrderAction(DataRspOrderAction2& act);

	//int GetOrderVolumeTotal() {
	//	int nVolume=0;
	//	for(int i=0; i<m_nRspOrderReqCount;i++) {
	//		nVolume += m_tRspOrderReqs[i].InputOrderField.VolumeTotalOriginal;
	//	}
	//	return nVolume;
	//};

	BOOL DoOrder(int& nErrPos, wxString& strRetMsg);
	BOOL RevokeOrder(PlatformStru_OrderInfo& ftdcOrderField, wxString& strRetMsg);
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
	BOOL				m_bIsLockForSetChange;

	//int					m_nReverseTimer;

	int					m_nRspOrderReqCount;
	CMBORDERINSERT		m_tRspOrderReqs[conMaxRspOrderReqNum];

	wxString			m_strRevokeMsg;
	int					m_nFtdcOrderActionCount;
	PlatformStru_InputOrderAction		m_tFtdcOrderActions[conMaxFtdcOrderActionNum];
	BOOL				m_bFtdcOrderActionsState[conMaxFtdcOrderActionNum];	// TRUE, 表示撤单成功

};
