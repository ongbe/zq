#include "stdafx.h"
#include "QueryContract.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


static CQueryContractDlg* s_pCQueryContractDlg=NULL;


CQueryContractDlg::CQueryContractDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(QUERY_CONTRACT),wxDefaultPosition, wxSize(800,280),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER),
m_pWxExtListCtrl(NULL)
{
    CreateGUIControls();

    s_pCQueryContractDlg=this;
}

CQueryContractDlg::~CQueryContractDlg()
{
	s_pCQueryContractDlg=NULL;
    m_vectorContract.clear();
}
wxString CQueryContractDlg::OnGetItemText (long item, long column) const
{
	if(item>=0&&item<(long)m_vectorContract.size())
	{
		switch (column)
		{
		case 0:
			return wxString(m_vectorContract[item].ContractName);
		case 1:
			return wxString(m_vectorContract[item].ContractDate); 
		case 2:
			return wxString(m_vectorContract[item].ExchangeName); 
		case 3:
			return wxString(m_vectorContract[item].ExpiryDate);
		case 4:
			return wxString(m_vectorContract[item].LastTradeDate);
		case 5:
			return wxString::Format("%d",m_vectorContract[item].NumberOfLegs);
		case 6:
			return wxString::Format("%d",m_vectorContract[item].TicksPerPoint);
		case 7:
			return wxString(m_vectorContract[item].TickSize);
		case 8:
			return wxString::Format("%d",m_vectorContract[item].Tradable);
		case 9:
			return wxString::Format("%d",m_vectorContract[item].GTStatus);
		case 10:
			return wxString(m_vectorContract[item].Margin); 
		case 11:
			return wxString(m_vectorContract[item].ESATemplate);
		case 12:
			return wxString(m_vectorContract[item].MarketRef);
		case 13:
			return wxString(m_vectorContract[item].lnExchangeName); 
		case 14:
			return wxString(m_vectorContract[item].lnContractName);
		case 15:
			return wxString(m_vectorContract[item].lnContractDate);
		}
	}
	return wxEmptyString;
}
void CQueryContractDlg::CreateGUIControls()
{
    INSERT_QUERYLIST_HEADER(CQueryContractDlg,16,QUERYCONTRACTDLG_COL);
	if(	DEFAULT_SVR())
		DEFAULT_SVR()->GetContract(m_vectorContract);
	m_pWxExtListCtrl->SetItemCount(m_vectorContract.size());
}
