#include "stdafx.h"
#include "QueryOrderType.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static CQueryOrderTypeDlg* s_pCQueryOrderTypeDlg=NULL;


CQueryOrderTypeDlg::CQueryOrderTypeDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(QUERY_ORDER_TYPE),wxDefaultPosition, wxSize(800,280),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER),
m_pWxExtListCtrl(NULL)
{
    CreateGUIControls();

    s_pCQueryOrderTypeDlg=this;
   
}

CQueryOrderTypeDlg::~CQueryOrderTypeDlg()
{
	s_pCQueryOrderTypeDlg=NULL;
   m_vectorOrderType.clear();
}
wxString CQueryOrderTypeDlg::OnGetItemText (long item, long column) const
{
	if(item>=0&&item<(long)m_vectorOrderType.size())
	{
		switch (column)
		{
		case 0:
			return wxString(m_vectorOrderType[item].OrderType);
		case 1:
			return wxString(m_vectorOrderType[item].ExchangeName); 
		case 2:
			return wxString::Format("%d",m_vectorOrderType[item].OrderTypeID); 
		case 3:
			return wxString::Format("%d",m_vectorOrderType[item].NumPricesReqd);
		case 4:
			return wxString::Format("%d",m_vectorOrderType[item].NumVolumesReqd);
		case 5:
			return wxString::Format("%d",m_vectorOrderType[item].NumDatesReqd);
		case 6:
			return wxString(m_vectorOrderType[item].AutoCreated);
		case 7:
			return wxString(m_vectorOrderType[item].TimeTriggered);
		case 8:
			return wxString(m_vectorOrderType[item].RealSynthetic);
		case 9:
			return wxString(m_vectorOrderType[item].GTCFlag);
		case 10:
			return wxString(m_vectorOrderType[item].TicketType);
		case 11:
			return wxString(m_vectorOrderType[item].PatsOrderType);
		case 12:
			return wxString::Format("%d",m_vectorOrderType[item].AmendOTCount); 
		case 13:
			return wxString(m_vectorOrderType[item].AlgoXML);
		case 14:
			return wxString(m_vectorOrderType[item].AmendTypesArray);
		}
	}

	return wxEmptyString;
}
void CQueryOrderTypeDlg::CreateGUIControls()
{
    INSERT_QUERYLIST_HEADER(CQueryOrderTypeDlg,15,QUERYORDERTYPEDLG_COL);
	if(	DEFAULT_SVR())
		DEFAULT_SVR()->GetOrderType(m_vectorOrderType);
    m_pWxExtListCtrl->SetItemCount(m_vectorOrderType.size());
}