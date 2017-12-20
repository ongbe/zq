#include "stdafx.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/ISvr/PlatformStruct.h"
#include "QueryExchangeRate.h"

extern PlatformSvrMgr* g_pPlatformMgr;

static CQueryExchangeRateDlg* s_pQueryExchangeRateDlg = NULL;

BEGIN_EVENT_TABLE(CQueryExchangeRateDlg,wxDialog)
	EVT_SIZE(OnSize)
	EVT_COMMAND(wxID_ANY, wxEVT_EXCHANGERATE, OnExchangeRateChanged)
END_EVENT_TABLE()

CQueryExchangeRateDlg::CQueryExchangeRateDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(ID_QUERY_EXCHANGE_RATE),wxDefaultPosition, wxSize(600,400),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
    CreateGUIControls();

    s_pQueryExchangeRateDlg = this;
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ExchangeRate, GID_ANY, ExchangeRateCallBackFunc);
}

CQueryExchangeRateDlg::~CQueryExchangeRateDlg()
{
   s_pQueryExchangeRateDlg = NULL;
   if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ExchangeRate, GID_ANY);
}

void CQueryExchangeRateDlg::CreateGUIControls()
{
	m_pListCtrl = new wxExtListCtrl(this, ID_LIST, wxDefaultPosition, GetClientSize(), wxLCEXT_REPORT|wxLCEXT_VRULES);
	m_pListCtrl->InsertColumn(0, LOADSTRING(EXCHANGE_RATE_CURRENCY), wxLIST_FORMAT_LEFT, 100);
	m_pListCtrl->InsertColumn(1, LOADSTRING(EXCHANGE_RATE), wxLIST_FORMAT_RIGHT, 100);

	vector<PlatformStru_Commodity> vecCommodity;
	set<string> setCurrency;
	if(	DEFAULT_SVR()&&DEFAULT_SVR()->GetCommodity(vecCommodity)>0)
	{
		std::vector<PlatformStru_Commodity>::iterator it=vecCommodity.begin();
		for( ; it!=vecCommodity.end();it++)
		{
			setCurrency.insert(it->Currency);
		}
	}

	set<string>::iterator it = setCurrency.begin();
	int nIndex = 0;
	for ( ; it != setCurrency.end(); ++it )
	{
		m_pListCtrl->InsertItem(nIndex, *it );
		double dExchangeRate = DEFAULT_SVR()->GetExchangeRate((char*)(*it).c_str());
		m_pListCtrl->SetItem(nIndex, 1, GlobalFunc::ConvertToString(dExchangeRate, 4));
		nIndex++;
	}
}

void CQueryExchangeRateDlg::OnSize( wxSizeEvent& evt )
{
	wxRect rc = GetClientRect();
	m_pListCtrl->SetSize( GetClientSize() );
}

int CQueryExchangeRateDlg::ExchangeRateCallBackFunc(const GUIModuleID GID, const AbstractBusinessData &data )
{
	if(data.BID != BID_ExchangeRate || data.Size != sizeof(DataExchangeRate))
		return 0;	
	DataExchangeRate* pTmp=new DataExchangeRate();
	memcpy(pTmp, &data, sizeof(DataExchangeRate));
	wxCommandEvent evt(wxEVT_EXCHANGERATE);
	evt.SetClientData(pTmp);
	if( s_pQueryExchangeRateDlg )
		s_pQueryExchangeRateDlg->AddPendingEvent(evt);
	return 0;
}

void CQueryExchangeRateDlg::OnExchangeRateChanged( wxCommandEvent& event )
{
	DataExchangeRate* pTmp= (DataExchangeRate*)(event.GetClientData());
	if(!pTmp) return;

	if ( !DEFAULT_SVR() )
	{
		wxDELETE(pTmp);
		event.SetClientData(NULL);
		return;
	}

	double dExchangeRate = DEFAULT_SVR()->GetExchangeRate(pTmp->RateField);
	for ( int i = 0; i < m_pListCtrl->GetItemCount(); i++)
	{
		wxString strCurrency = m_pListCtrl->GetItemText( i );
		if( strCurrency == pTmp->RateField )
		{
			m_pListCtrl->SetItem(i, 1, GlobalFunc::ConvertToString(dExchangeRate, 4));
		}
	}

	wxDELETE(pTmp);
	event.SetClientData(NULL);
}

