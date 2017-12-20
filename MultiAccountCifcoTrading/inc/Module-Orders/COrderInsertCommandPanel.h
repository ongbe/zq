#pragma once
#include "stdafx.h"
#include "../inc/Module-Misc/orderDefines.h"


class COrderInsertCommandPanel : public wxPanel
{
public:
	COrderInsertCommandPanel(wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxEmptyString);
	~COrderInsertCommandPanel();
    	
//	void OnSize(wxSizeEvent& event);
//	void OnPaint(wxPaintEvent& event);
    DECLARE_CLASS(COrderRealMsgPanel)
	DECLARE_EVENT_TABLE()

public:
	void ResetViewStyle(int nViewStyle);
	void ViewStyleSwitch_DoStop(BOOL bIsStop);
	void ResetLanguage();
	void InsertCtrlID2TabCtrl(vector<UINT>& tabCtrlList);

private:
    std::vector<int>			m_arrTabCtrl;

};