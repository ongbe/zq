#include "stdafx.h"
#include "QueryTrader.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

CQueryTraderDlg::CQueryTraderDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(QUERY_TRADER),wxDefaultPosition, wxSize(800,280),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER),
m_pWxExtListCtrl(NULL)
{
    CreateGUIControls();  
}

CQueryTraderDlg::~CQueryTraderDlg()
{
   m_vectorTrader.clear();
}
wxString CQueryTraderDlg::OnGetItemText (long item, long column) const
{
	if(item>=0&&item<(long)m_vectorTrader.size())
	{
		switch (column)
		{
		case 0:
			return wxString(m_vectorTrader[item].TraderAccount);//ук╨е
		case 1:
			return wxString::Format("%c",m_vectorTrader[item].Tradable);
		case 2:
			return wxString::Format("%d",m_vectorTrader[item].LossLimit);
		}
	}
	return wxEmptyString;
}
void CQueryTraderDlg::CreateGUIControls()
{
    INSERT_QUERYLIST_HEADER(CQueryTraderDlg,3,QUERYTRADERDLG_COL);
	if(	DEFAULT_SVR())
		DEFAULT_SVR()->GetTrader(m_vectorTrader);
    m_pWxExtListCtrl->SetItemCount(m_vectorTrader.size());
}

