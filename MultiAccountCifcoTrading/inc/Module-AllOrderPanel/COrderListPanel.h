#pragma once
#include "stdafx.h"

typedef std::map <OrderKey,  PlatformStru_OrderInfo> OrdersPanelMAP;
typedef std::map <OrderKey,  PlatformStru_OrderInfo> OrdersPanelMAP2;
class COrderListPanel : public wxPanel
{
public:
	COrderListPanel(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	virtual ~COrderListPanel();

	static BOOL IsFocusInPanel(wxWindow* pCurrWnd);
	void ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow);
    
	DECLARE_EVENT_TABLE()

protected:
	static map<string, DWORD> m_mapCancelWait;	// 所有等待的撤单的集合
};