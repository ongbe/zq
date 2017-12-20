#pragma once
#include "stdafx.h"
#include <wx/gbsizer.h>
#include <wx/spinctrl.h>
#include <wx/spinbutt.h>
#include <vector>
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/COrderServiceThread.h"


enum
{
	ID_OBATCH2_STATIC1=1550,
	ID_OBATCH2_STATIC2,
	ID_OBATCH2_STATIC3,
	ID_OBATCH2_STATIC4,
	ID_OBATCH2_STATIC5,
	ID_OBATCH2_STATIC6,
	ID_OBATCH2_STATIC7,
	ID_OBATCH2_STATICTOTAL,
	ID_OBATCH2_STATICSENDTOTAL,
	ID_OBATCH2_STATICSUCCESS,
	ID_OBATCH2_STATICINDEX,
	ID_OBATCH2_STATICTIMER,
	ID_OBATCH2_IDPAUSE,
	ID_OBATCH2_IDCANCEL,
	ID_OBATCH2_LISTMSG,
	ID_OBATCH2_TIMER,
	OBATCH2SUMNUM_ID	// 标志ID号的数量
};

class COrderBatchDailog :public wxDialog
{
public:
	COrderBatchDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
   ~COrderBatchDailog();

	void SetOrderServiceThread(COrderServiceThread* poOrderServiceThread) {
		m_poOrderServiceThread = poOrderServiceThread;
	};

    virtual bool Show( bool show = true );

    void OnPause(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnClose(wxCloseEvent& evt);
	void OnTimer(wxTimerEvent& event);
	DECLARE_EVENT_TABLE()

public:
    //static COrderBatchDailog* CreateDailog(wxWindow* parent);
	
	void SetPriceType(int nPriceType) {
		m_nPriceType = nPriceType;
	};
	void SetOrderReq(PlatformStru_InputOrder& tReq) {
		m_OrderReq = tReq;
	};
	void SetHedge(BOOL bHedge) {
		m_bHedge = bHedge;
	};
	void SetBatchEachNum(int nNum) {
		m_nBatchEachNum = nNum;
	};
	void SetTimer(int nVal) {
		m_nTimerInterval = nVal;
	};
	
private:
	void ShowOrderReq();

private:
	int		m_nGroupNum;
	COrderServiceThread* m_poOrderServiceThread;
	//int		m_ReqID;
    wxTimer m_timer;	//行情更新定时器
	(PlatformStru_InputOrder m_OrderReq;
	int		m_nPriceType;
	BOOL	m_bHedge;
	int		m_nBatchEachNum;
	int		m_nTimerInterval;

	int		m_nBatchCount;
	int		m_nBatchEndNum;
	int		m_nSendSum;
	int		m_nSuccessSum;
	int		m_nTimerOut;
	DWORD	m_dwTickCountBak;
	BOOL	m_bIsPause;

};