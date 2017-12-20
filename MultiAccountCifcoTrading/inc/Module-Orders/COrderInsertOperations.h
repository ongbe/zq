#pragma once
#include "stdafx.h"
#include <vector>
#include "../inc/Module-Misc/TradeInfoDlg.h" //交易信息提示对话框
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/COrderServiceThread.h"
#include "COrderInsertEntity.h"
#include "COrderChangeEntity.h"
#include "COrderInsertView.h"
#include "CMAOrderEntity.h"
#include "CMAOrderConfirmDlg.h"
#include "COrderAtomEntity.h"

#include "../inc/Module-Misc/SimpleWriteLog.h"

class CMAOrderConfirmDlg;

class COrderInsertOperations : public COrderInsertView
{
public:

	COrderInsertOperations(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style =wxScrolledWindowStyle | wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderInsertOperations();

	void SetMultiplyVolume(int nVolume) {
		m_nMultiplyVolume = nVolume;
	};

//DECLARE_EVENT_TABLE()

protected:

	void		FillInputOrderField(PlatformStru_InputOrder& req);
	
	int			GetLogonAccounts(map<string,bool>& accsLogon, BOOL bIsAll=FALSE);
	void		FilterNoPositionAccount(BOOL bNeedCancel, 
					COrderAtomEntity::STDORDERREQ& stdReq, map<string, bool>& mapAccounts);

	double		AnalysePrice(double fPrice, wxString& strInstrumentID);
	BOOL		DoMaOrderInsert(CSAOrderEntity::OPERPARAM& operParam, 
					COrderAtomEntity::STDORDERREQ& stdReq, 
					CMAOrderEntity::MAAUTOOPER& maOper, 
					map<string, bool>& accsLogon);
	BOOL		DoOrderInsert(COrderInsertEntity::ORIORDERREQ& tOriReq, int nAction, BOOL& bHasOrderInsert, BOOL bIsAsyncView=FALSE);
	BOOL		DoOrderChange(PlatformStru_OrderInfo& ftdcField, BOOL bIsAsyncView=FALSE);
	void		DoBackHand(wxString strAccount, wxString InstrumentID, char PosiDirection, char Hedge, int Volume, BOOL bOnlyOpen=FALSE);
	void		DoKeyOrder(wxString& strCode, KeyOrderInfo& keyOrder);
	void		AsyncViewData(BOOL bIsBuy, int nOCMode, int nVolume, double fPrice, int nAction);
	
	//根据报单回报显示 交易信息
	void		ShowTradeInfoDlg(const string& Title, const CThostFtdcOrderField& OrderInfo, BOOL bIsShow);
	//根据报单回执显示 交易信息
	void		ShowTradeInfoDlg(const string& Title, const DataRspOrderInsert& OrderInfo, BOOL bIsShow);
	//根据报单回执显示 交易信息
	void		ShowTradeInfoDlg(const string& Title, const COrderChangeEntity::CMBORDERINSERT& OrderInfo, BOOL bIsShow);
	//根据报单回执显示 交易信息
	void		ShowTradeInfoDlg(const string& Title, const COrderInsertEntity::CMBORDER& OrderInfo, BOOL bIsShow);
	//根据ErrRtnOrderAction显示 撤单失败信息
	void		ShowTradeInfoDlg(const string& Title, const CThostFtdcOrderActionField& OrderActionInfo, BOOL bIsShow);
	//根据OnRspOrderAction显示 撤单失败信息
	void		ShowTradeInfoDlg(const string& Title, const DataRspOrderAction1& InputOrderActionInfo, BOOL bIsShow);
    //显示错误信息
    void		ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow);

	BOOL		NeedConfirm();							// 判断下单前是否需要确认
	BOOL		NeedAutoOpenClose(int& nAutoMode);		// 判读是否需要自动开平
	BOOL		NeedAlwaysOpen();
	BOOL		NeedAutoOCHideRadio();					// 使用自动平仓时，取消开平仓选择
	BOOL		NeedContinueTrackPrice();				// 在跟盘状态 下单 后继续跟盘
	BOOL		NeedSimulateMarketPrice();
	BOOL		NeedReturnQuotList();
	BOOL		NeedOrderSuccessConfirm();
	BOOL		NeedOrderSuccessSound();
	BOOL		NeedOrderFailConfirm();
	BOOL		NeedOrderFailSound();
	BOOL		NeedOrderTradeConfirm();
	BOOL		NeedOrderTradeSound();
	void		SaveFivePriceList(BOOL bHas);

	int			GetAfterFocus();
	int			GetActionAfterOrder();
	int			GetRaskVolumeLimit();
	int			GetRaskDifferPrice();
	int			GetDefaultVolume(wxString& wxCode);
	int			GetDefaultVolume(wxString& wxCode, int& nDefaultVolume, int& nMultiplyVolume);

public:
	static BOOL		NeedFivePriceList();

	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

protected:

	double		m_currPriceTick;
	double		m_currUpperLimitPrice;
	double		m_currLowerLimitPrice;
	int			m_nMultiplyVolume;
    bool		m_NeedUpdatePrice;
	BOOL		m_bNoKeyChangeText;
	BOOL		m_bIsGridChangeInstrumentID;

	wxMutex		m_oMutex;

    std::set<std::string> m_InstrumentIDArray;
	COrderServiceThread* m_poOrderServiceThread;
	volatile bool m_bWaitingRsp_OrderInsert;//等待下单的响应
	volatile bool m_bWaitingRsp_OrderDelete;//等待撤单的响应
	//int m_ReqID;//最后下单的请求ID，
	bool m_bShowTradeInfo;//根据配置文件，是否显示交易信息
	std::map<std::string, int> m_KeyMultiplyVolumeMap;
	std::map<COrderInsertEntity*, int> m_mapOrderInsertWaitForCancel;	// 所有需要等待撤单完成后才能下单的实体的集合
	std::map<COrderAtomEntity*, int> m_mapCancelThenOrder;	// 所有需要等待撤单完成后才能下单的实体的集合
	std::map<CMAOrderEntity*, CMAOrderConfirmDlg*> m_mapMAOrderMonitoring;	// 所有需要监视的MAEntity的集合

	zqWriteLog* m_pWriteLog;

};
