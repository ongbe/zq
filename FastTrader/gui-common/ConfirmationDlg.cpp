#include "StdAfx.h"
#include <wx/image.h>
#include <wx/mstream.h>
#include "../Module-Misc2/GlobalFunc.h"
#include "ConfirmationDlg.h"
#include "gui/zq.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

ConfirmationDlg::ConfirmationDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,LOADSTRING(CONFIRMATIONDLG_TITLE),wxDefaultPosition, wxSize(900,500))
{
	//解决FASTTRADER-1349，设置登录窗口和结算确认窗口置顶
	if(wxGetApp().argc>2&&strcmp(wxGetApp().argv[1],"KillOldAfterStartup")==0)
	{
		HWND wHandle = (HWND)atoi(wxGetApp().argv[2]);
		if(::GetWindowLong(wHandle,GWL_EXSTYLE)&WS_EX_TOPMOST)
			SetWindowStyle(GetWindowStyle()|wxSTAY_ON_TOP);	
	}
	DIALOG_CREATE_LOG();
    AUTO_SETSIZE();
	GET_JPG_FILE_RESOURCE("IDR_XML_BG2",_T("XML"),m_BackBitmap);


    SetBackgroundColour(DEFAULT_COLOUR);
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxTextCtrl* m_pTextCtrl= new wxTextCtrl( this, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(880,400), wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL);
	m_pTextCtrl->SetMaxLength(0);
    itemBoxSizer3->Add(m_pTextCtrl, 0, wxGROW|wxALL, 5);
    m_pTextCtrl->SetFont(wxFont(10, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys"));
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton6 = new wxButton( this, wxID_OK,LOADSTRING(CONFIRMATIONLG_OK), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( this, wxID_CANCEL,LOADSTRING(CONFIRMATIONLG_CANCEL), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    std::string strContent="";
    if(DEFAULT_SVR()) DEFAULT_SVR()->GetLastSettlementInfo(strContent);
    m_pTextCtrl->SetLabel(strContent.c_str());
    itemButton6->SetDefault();
    itemButton6->SetFocus();
}

ConfirmationDlg::~ConfirmationDlg(void)
{

}
BEGIN_EVENT_TABLE(ConfirmationDlg,wxDialog)
    EVT_PAINT(ConfirmationDlg::OnPaint)
END_EVENT_TABLE()

void ConfirmationDlg::OnPaint(wxPaintEvent& event)
{
    if(m_BackBitmap.IsOk())
    {
        wxPaintDC dc(this);
        dc.DrawBitmap(m_BackBitmap,0,0);
    }
    else
        event.Skip();
}