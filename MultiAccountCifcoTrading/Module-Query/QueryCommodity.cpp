#include "stdafx.h"
#include "QueryCommodity.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static CQueryCommodityDlg* s_pCQueryCommodityDlg=NULL;


CQueryCommodityDlg::CQueryCommodityDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(QUERY_COMMODITY),wxDefaultPosition, wxSize(800,280),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER),
m_pWxExtListCtrl(NULL)
{
    CreateGUIControls();

    s_pCQueryCommodityDlg=this;
   
}

CQueryCommodityDlg::~CQueryCommodityDlg()
{
	s_pCQueryCommodityDlg=NULL;
   m_vectorCommodity.clear();
}
wxString CQueryCommodityDlg::OnGetItemText (long item, long column) const
{
	if(item>=0&&item<(long)m_vectorCommodity.size())
	{
		switch (column)
		{
		case 0:
			return wxString(m_vectorCommodity[item].ExchangeName);
		case 1:
			return wxString(m_vectorCommodity[item].ContractName); 
		case 2:
			return wxString(m_vectorCommodity[item].Currency); 
		case 3:
			return wxString(m_vectorCommodity[item].Group);
		case 4:
			return wxString(m_vectorCommodity[item].OnePoint);
		case 5:
			return wxString::Format("%d",m_vectorCommodity[item].TicksPerPoint);
		case 6:
			return wxString(m_vectorCommodity[item].TickSize);
		case 7:
			return wxString::Format("%d",m_vectorCommodity[item].GTStatus);
		}
	}
	return wxEmptyString;
}
void CQueryCommodityDlg::CreateGUIControls()
{
    INSERT_QUERYLIST_HEADER(CQueryCommodityDlg,8,QUERYCOMMODITYDLG_COL);
	if(	DEFAULT_SVR())
		DEFAULT_SVR()->GetCommodity(m_vectorCommodity);
    m_pWxExtListCtrl->SetItemCount(m_vectorCommodity.size());
}
