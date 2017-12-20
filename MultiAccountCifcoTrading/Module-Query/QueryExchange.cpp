#include "stdafx.h"
#include "QueryExchange.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


static CQueryExchangeDlg* s_pCQueryExchangeDlg=NULL;


CQueryExchangeDlg::CQueryExchangeDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(QUERY_EXCHANGE),wxDefaultPosition, wxSize(800,280),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER),
m_pWxExtListCtrl(NULL)
{
	CreateGUIControls();

	s_pCQueryExchangeDlg=this;
}

CQueryExchangeDlg::~CQueryExchangeDlg()
{
	s_pCQueryExchangeDlg=NULL;
	m_vectorExchange.clear();
}
wxString CQueryExchangeDlg::OnGetItemText (long item, long column) const
{
	if(item>=0&&item<(long)m_vectorExchange.size())
	{
		switch (column)
		{
		case 0:
			return wxString(m_vectorExchange[item].Name);
		case 1:
			return wxString(m_vectorExchange[item].QueryEnabled);
		case 2:
			return wxString(m_vectorExchange[item].AmendEnabled);
		case 3:
			return wxString::Format( "%d",m_vectorExchange[item].Strategy);
		case 4:
			return wxString(m_vectorExchange[item].CustomDecs);
		case 5:
			return wxString::Format("%d",m_vectorExchange[item].Decimals);
		case 6:
			return wxString::Format("%x(%c)",m_vectorExchange[item].TicketType,m_vectorExchange[item].TicketType>32?m_vectorExchange[item].TicketType:32);
		case 7:
			return m_vectorExchange[item].RFQA;
		case 8:
			return m_vectorExchange[item].RFQT;
		case 9:
			return m_vectorExchange[item].EnableBlock;
		case 10:
			return m_vectorExchange[item].EnableBasis;
		case 11:
			return m_vectorExchange[item].EnableAA;
		case 12:
			return m_vectorExchange[item].EnableCross;
		case 13:
			return wxString::Format("%x",m_vectorExchange[item].GTStatus);	
		}
	}
	return wxEmptyString;
}
void CQueryExchangeDlg::CreateGUIControls()
{
    INSERT_QUERYLIST_HEADER(CQueryExchangeDlg,14,QUERYEXCHANGEDLG_COL);
	if(	DEFAULT_SVR())
		DEFAULT_SVR()->GetExchange(m_vectorExchange);
    m_pWxExtListCtrl->SetItemCount(m_vectorExchange.size());
}
