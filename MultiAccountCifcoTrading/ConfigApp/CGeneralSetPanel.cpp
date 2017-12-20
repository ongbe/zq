#include "stdafx.h"
#include <wx/valtext.h>
#include "CGeneralSetPanel.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(CGeneralSetPanel,wxPanel)
	EVT_COMBOBOX(ID_CBX_OPENORDERBTNHOTKEY, OnChangeSel)
	EVT_COMBOBOX(ID_CBX_ORDERPARKEDBTNHOTKEY, OnChangeSel)
END_EVENT_TABLE()

CGeneralSetPanel::CGeneralSetPanel(wxWindow *parent,
							wxWindowID winid ,
							const wxPoint& pos ,
							const wxSize& size ,
							long style,
							const wxString& name ):
							wxPanel(parent,winid,pos,size,style,name)
{
	const int nPartOneBoxHeight = 140;
	const int nPartTwoBoxHeight = 110;
	const int nPartThreeBoxHeight = 95;
	const int nDistOfBoxOne2Two = 5;
	const int nDistOfBoxTwo2Three = 5;
	const int nContractInputBoxHeight = 85;

	new wxStaticBox(this, wxID_ANY, LOADSTRING(CONSTRACT_INPUT), wxPoint(14, 3), wxSize(200, nContractInputBoxHeight));
	m_cbxMouseInputInstrumentId = new wxCheckBox(this, wxID_ANY, LOADSTRING(ENABLE_MOUSE_INPUT_CONSTRACT), 
		wxPoint(23, 23), wxSize(185, 15), 0);
	m_cbxInputSimpleSpell = new wxCheckBox(this, wxID_ANY, LOADSTRING(ENABLE_INPUT_SIMPLE_SPELL), 
		wxPoint(23, 44), wxSize(185, 15), 0);

	wxStaticText* staticStyle = new wxStaticText(this, wxID_ANY, LOADSTRING(POPUP_DIALOG_STYLE), wxPoint(23, 65), wxDefaultSize);
	m_cbxListStyle = new wxOwnerDrawnComboBox(this, ID_CBX_LISTSTYLE, wxEmptyString, 
		wxPoint(150, 65), wxSize(60, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticBox(this, wxID_ANY, LOADSTRING(GID_ORDER_INSERT_PANEL_CAPTION), wxPoint(221, 3), wxSize(257, nPartOneBoxHeight));
	m_cbxOrderConfirm = new wxCheckBox(this, wxID_ANY, LOADSTRING(CONFIRM_BEFORE_ORDER), 
		wxPoint(230, 23), wxSize(242, 15), 0);
	m_cbxAutoOpenClose = new wxCheckBox(this, wxID_ANY, LOADSTRING(AUTO_OPEN_CLOSE_AS_DEFAULT), 
		wxPoint(230, 44), wxSize(242, 15), 0);
    m_cbxRetrack = new wxCheckBox(this, wxID_ANY, LOADSTRING(KEEP_TRACK), 
		wxPoint(230, 65), wxSize(242, 15), 0);
	m_cbxSimulate = new wxCheckBox(this, wxID_ANY, LOADSTRING(LMT_PRICE_WHILE_NO_MKT), 
		wxPoint(230, 86), wxSize(242, 15), 0);
	m_cbxSimulate->Show(false);
	wxStaticText* staticMKTPriceUse = new wxStaticText(this, wxID_ANY, LOADSTRING(MKT_PRICE_USE), 
		wxPoint(230, 86), wxDefaultSize);
	m_cbxMKTPriceUse = new wxOwnerDrawnComboBox(this, ID_CBX_MKTPRICEUSE, wxEmptyString, 
		wxPoint(400, 86), wxSize(60, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	if (DEFAULT_SVR()&&!DEFAULT_SVR()->HaveOrderType( UIOT_SIMULATEMARKET ))
	{
		m_cbxSimulate->Hide();
	}

	const int nPartTwoTop = nPartOneBoxHeight+nDistOfBoxOne2Two;
	const int nInputPriceBoxTop = nContractInputBoxHeight + 5;
	new wxStaticBox(this, wxID_ANY, LOADSTRING(INPUT_PRICE_BOX), wxPoint(14, nInputPriceBoxTop), wxSize(200, nPartOneBoxHeight-nInputPriceBoxTop));
	m_cbxMouseInputPrice = new wxCheckBox(this, wxID_ANY, LOADSTRING(ENABLE_MOUSE_INPUT_PRICE), 
		wxPoint(22, nInputPriceBoxTop+20), wxSize(185, 17), 0);

	new wxStaticBox(this, wxID_ANY, LOADSTRING(VOLUME_INPUT_BOX), wxPoint(14, nPartTwoTop), wxSize(200, nPartTwoBoxHeight));
	m_cbxMouseInputHand = new wxCheckBox(this, wxID_ANY, LOADSTRING(ENABLE_MOUSE_INPUT_FREQUENTLY), wxPoint(21, nPartTwoTop+20), 
		wxSize(185, 17), 0);

	wxTextValidator validator( wxFILTER_NUMERIC );
	const int nInputVolTop = nPartTwoTop + 20;
	const int nRowDistance = 21;
	m_text1 = new wxTextCtrl(this,wxID_ANY, wxEmptyString, wxPoint(22, nInputVolTop+nRowDistance), wxSize(23, 19),  0);
	m_text2 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(55, nInputVolTop+nRowDistance), wxSize(23, 19), 0);
	m_text3 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(88, nInputVolTop+nRowDistance), wxSize(23, 19), 0);
	m_text4 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(22, nInputVolTop+nRowDistance*2), wxSize(23, 19), 0);
	m_text5 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(55, nInputVolTop+nRowDistance*2), wxSize(23, 19), 0);
	m_text6 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(88, nInputVolTop+nRowDistance*2), wxSize(23, 19), 0);
	m_text7 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(22, nInputVolTop+nRowDistance*3), wxSize(23, 19), 0);
	m_text8 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(55, nInputVolTop+nRowDistance*3), wxSize(23, 19), 0);
	m_text9 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(88, nInputVolTop+nRowDistance*3), wxSize(23, 19), 0);

	m_text1->SetMaxLength(2);
	m_text2->SetMaxLength(2);
	m_text3->SetMaxLength(2);
	m_text4->SetMaxLength(2);
	m_text5->SetMaxLength(2);
	m_text6->SetMaxLength(2);
	m_text7->SetMaxLength(2);
	m_text8->SetMaxLength(2);
	m_text9->SetMaxLength(2);

	//new wxStaticBox(this, wxID_ANY, LOADSTRING(CURSORPOS), wxPoint(221, nPartTwoTop), wxSize(257, nPartTwoBoxHeight));
	//new wxStaticText(this, wxID_ANY, LOADSTRING(CURSORPOS_TITLE), wxPoint(228, nPartTwoTop+20), wxDefaultSize);
	m_rbDefault = new wxRadioButton(this, wxID_ANY, LOADSTRING(CURSORPOS_DEFAULT), wxPoint(228, nPartTwoTop+40), wxSize(60, 17), 0 );
	m_rbContract = new wxRadioButton(this, wxID_ANY, LOADSTRING(Printf_InstrumentIDTitle), wxPoint(298, nPartTwoTop+40), wxSize(60, 17), 0 );
	m_rbDirection = new wxRadioButton(this, wxID_ANY, LOADSTRING(CURSORPOS_BUYSEL), wxPoint(368, nPartTwoTop+40), wxSize(60, 17), 0 );
	m_rbOpenClose = new wxRadioButton(this, wxID_ANY, LOADSTRING(Printf_OpenCloseTitle), wxPoint(228, nPartTwoTop+60), wxSize(60, 17), 0 );
	m_rbPrice = new wxRadioButton(this, wxID_ANY, LOADSTRING(Printf_PriceTitle), wxPoint(298, nPartTwoTop+60), wxSize(60, 17), 0 );
	m_rbVol = new wxRadioButton(this, wxID_ANY, LOADSTRING(Printf_QtyTitle), wxPoint(368, nPartTwoTop+60), wxSize(60, 17), 0 );
	m_rbDefault->Show(false);
	m_rbContract->Show(false);
	m_rbDirection->Show(false);
	m_rbOpenClose->Show(false);
	m_rbPrice->Show(false);
	m_rbVol->Show(false);

	const int nPartThreeTop = nPartTwoTop + nPartTwoBoxHeight + nDistOfBoxTwo2Three;
	const int nPartThreeLine1Top = nPartThreeTop + 25;
	const int nPartThreeLine2Top = nPartThreeLine1Top + 30;
	new wxStaticBox(this, wxID_ANY, LOADSTRING(ACCELERATOR), wxPoint(14, nPartThreeTop), wxSize(463, 85));
	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_BUY), wxPoint(23, nPartThreeLine1Top ), wxDefaultSize, 0);
	m_tcBuyHotKey = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(55, nPartThreeLine1Top), wxSize(30, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_SELL), wxPoint(115, nPartThreeLine1Top ), wxDefaultSize, 0, LOADSTRING(OLS_SELL));
	m_tcSelHotKey = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(147, nPartThreeLine1Top), wxSize(30, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_OPEN), wxPoint(207, nPartThreeLine1Top ), wxDefaultSize, 0, LOADSTRING(OLS_OPEN));
	m_tcOpenOrderHotKey = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(239, nPartThreeLine1Top), wxSize(30, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_CLOSE_TODAY), wxPoint(299, nPartThreeLine1Top ), wxDefaultSize, 0);
	m_tcCloseTodayVolumnHotKey = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(333,nPartThreeLine1Top), wxSize(30, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_CLOSE), wxPoint(384, nPartThreeLine1Top ), wxDefaultSize, 0, LOADSTRING(OLS_CLOSE));
	m_tcCloseOrderHotKey = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(425, nPartThreeLine1Top), wxSize(30, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(ORDER_BUTTON), wxPoint(23, nPartThreeLine2Top ), wxDefaultSize, 0);

	m_tcBuyHotKey->Connect( wxEVT_CHAR,wxCharEventHandler(CGeneralSetPanel::OnCharBuy),NULL,this);
	m_tcSelHotKey->Connect( wxEVT_CHAR,wxCharEventHandler(CGeneralSetPanel::OnCharSel),NULL,this);

	m_tcOpenOrderHotKey->Connect( wxEVT_CHAR,wxCharEventHandler(CGeneralSetPanel::OnCharOpen),NULL,this);
	m_tcCloseTodayVolumnHotKey->Connect( wxEVT_CHAR,wxCharEventHandler(CGeneralSetPanel::OnCharCloseToday),NULL,this);
	m_tcCloseOrderHotKey->Connect( wxEVT_CHAR,wxCharEventHandler(CGeneralSetPanel::OnCharClose),NULL,this);

	m_cbxOpenOrderBtnHotKey = new wxOwnerDrawnComboBox(this, ID_CBX_OPENORDERBTNHOTKEY, wxEmptyString, 
		wxPoint(97, nPartThreeLine2Top), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	wxStaticText* staticPark = new wxStaticText(this, wxID_ANY, LOADSTRING(PARK_BUTTON), wxPoint(192, nPartThreeLine2Top ), wxDefaultSize, 0);
	m_cbxOrderParkedBtnHotKey = new wxOwnerDrawnComboBox(this, ID_CBX_ORDERPARKEDBTNHOTKEY, wxEmptyString, 
		wxPoint(272, nPartThreeLine2Top), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	if ( DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		m_cbxMouseInputInstrumentId->Enable(false);
		m_cbxInputSimpleSpell->Enable(false);

		staticStyle->Show(false);
		m_cbxListStyle->Show(false);

		m_cbxAutoOpenClose->Hide();
		m_cbxRetrack->Hide();
		m_cbxMKTPriceUse->Hide();
		staticMKTPriceUse->Hide();

		staticPark->Show(false);
		m_cbxOrderParkedBtnHotKey->Show(false);
	}

	m_tcBuyHotKey->SetMaxLength(1);
	m_tcSelHotKey->SetMaxLength(1);
	m_tcOpenOrderHotKey->SetMaxLength(1);
	m_tcCloseTodayVolumnHotKey->SetMaxLength(1);
	m_tcCloseOrderHotKey->SetMaxLength(1);

	InitControls();
}

CGeneralSetPanel::~CGeneralSetPanel()
{

}

void CGeneralSetPanel::InitControls()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;
    
	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();

	m_cbxMouseInputInstrumentId->SetValue( pOrderCfg->bMouseInputInstrumentId );
	m_cbxInputSimpleSpell->SetValue( pOrderCfg->bInputAbbreviation );
	m_cbxOrderConfirm->SetValue( pOrderCfg->bOrderConfirm );
	m_cbxAutoOpenClose->SetValue( pOrderCfg->bAutoOpenClose );
	m_cbxRetrack->SetValue( pOrderCfg->bReTrack );
	m_cbxSimulate->SetValue( pOrderCfg->bSimulate );
	m_cbxMKTPriceUse->Append("---");
	m_cbxMKTPriceUse->Append(LOADSTRING(MKT_PRICE_LAST_PRICE));
	m_cbxMKTPriceUse->Append(LOADSTRING(MKT_PRICE_BUYSELL_PRICE));
	m_cbxMKTPriceUse->Select( pOrderCfg->nMKTPriceUse );

	m_cbxMouseInputPrice->SetValue( pOrderCfg->bMouseInputPrice );
	m_cbxMouseInputHand->SetValue(pOrderCfg->bMouseInputVolume);

	m_cbxListStyle->Append(LOADSTRING(PARK_AUTO));
	m_cbxListStyle->Append(LOADSTRING(Q7_NAME));
	m_cbxListStyle->Append(LOADSTRING(SUNGARD_NAME));
	m_cbxListStyle->Select( pOrderCfg->nContractListStyle );

	char buf[20];
	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[0]);
	m_text1->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[1]);
	m_text2->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[2]);
	m_text3->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[3]);
	m_text4->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[4]);
	m_text5->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[5]);
	m_text6->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[6]);
	m_text7->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[7]);
	m_text8->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%d",pOrderCfg->Volume[8]);
	m_text9->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%c",pOrderCfg->cBuyHotKey);
	m_tcBuyHotKey->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%c",pOrderCfg->cSelHotKey);
	m_tcSelHotKey->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%c",pOrderCfg->cOpenOrderHotKey);
	m_tcOpenOrderHotKey->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%c",pOrderCfg->cCloseTodayVolumnHotKey);
	m_tcCloseTodayVolumnHotKey->AppendText( buf );

	memset( buf, 0, sizeof(buf));
	sprintf(buf,"%c",pOrderCfg->cCloseOrderHotKey);
	m_tcCloseOrderHotKey->AppendText( buf );

	switch(pOrderCfg->nCursorPosAfterOrder)
	{
	case 0:
		m_rbDefault->SetValue(true);
		break;
	case 1:
		m_rbContract->SetValue(true);
		break;
	case 2:
		m_rbDirection->SetValue(true);
		break;
	case 3:
		m_rbOpenClose->SetValue(true);
		break;
	case 4:
		m_rbPrice->SetValue(true);
		break;
	case 5:
		m_rbVol->SetValue(true);
		break;
	default:
		m_rbDefault->SetValue(true);
		break;
	}

	map<int,string> addr = pMgr->GetShortCutKeyNameMap();
	map<int,string>::iterator it;
	wxArrayString arr;
	for( it= addr.begin(); it != addr.end(); ++it)
	{
		arr.Add(it->second);
	}

	m_cbxOpenOrderBtnHotKey->Append(arr);
	m_cbxOpenOrderBtnHotKey->SetValue( pOrderCfg->szOpenOrderBtnHotKey.operator const char*() );
	m_strOpenOrderBtnHotKey = pOrderCfg->szOpenOrderBtnHotKey;
	m_cbxOrderParkedBtnHotKey->Append(arr);
	m_cbxOrderParkedBtnHotKey->SetValue( pOrderCfg->szOrderParkedBtnHotKey.operator const char*() );
	m_strOrderParkedBtnHotKey = pOrderCfg->szOrderParkedBtnHotKey;
}

bool CGeneralSetPanel::WriteCfg()
{	
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	LPORDER_CFG  pOrderCFG = pMgr->GetOrderCfg();

	bool bChanged = false;
	if ( pOrderCFG->bMouseInputInstrumentId != m_cbxMouseInputInstrumentId->IsChecked() )
	{
		pOrderCFG->bMouseInputInstrumentId = m_cbxMouseInputInstrumentId->IsChecked();
		bChanged = true;
	}

	if( pOrderCFG->bInputAbbreviation != m_cbxInputSimpleSpell->IsChecked()) 
	{
		pOrderCFG->bInputAbbreviation = m_cbxInputSimpleSpell->IsChecked();
		bChanged = true;
	}

	if ( pOrderCFG->bOrderConfirm != m_cbxOrderConfirm->IsChecked())
	{
		pOrderCFG->bOrderConfirm = m_cbxOrderConfirm->IsChecked();
		bChanged = true;
	}

	if( pOrderCFG->bAutoOpenClose != m_cbxAutoOpenClose->IsChecked() )
	{
		pOrderCFG->bAutoOpenClose = m_cbxAutoOpenClose->IsChecked();
		bChanged = true;
	}

	if( pOrderCFG->bReTrack != m_cbxRetrack->IsChecked() )
	{
		pOrderCFG->bReTrack = m_cbxRetrack->IsChecked();
		bChanged = true;
	}

	if( pOrderCFG->bSimulate != m_cbxSimulate->IsChecked() )
	{
		pOrderCFG->bSimulate = m_cbxSimulate->IsChecked();
		bChanged = true;
	}

	if ( pOrderCFG->nMKTPriceUse != m_cbxMKTPriceUse->GetSelection())
	{
		pOrderCFG->nMKTPriceUse = m_cbxMKTPriceUse->GetSelection();
		bChanged = true;
	}

	if( pOrderCFG->bMouseInputPrice != m_cbxMouseInputPrice->IsChecked() )
	{
		pOrderCFG->bMouseInputPrice = m_cbxMouseInputPrice->IsChecked();
		bChanged = true;
	}

	if( pOrderCFG->bMouseInputVolume != m_cbxMouseInputHand->IsChecked() )
	{
		pOrderCFG->bMouseInputVolume = m_cbxMouseInputHand->IsChecked();
		bChanged = true;
	}

	if ( pOrderCFG->nContractListStyle != m_cbxListStyle->GetSelection())
	{
		pOrderCFG->nContractListStyle = m_cbxListStyle->GetSelection();
		bChanged = true;
	}

	int nCursorPosAfterOrder = 0;
	if ( m_rbDefault->GetValue() )
	{
		nCursorPosAfterOrder = 0;
	}
	else if ( m_rbContract->GetValue() )
	{
		nCursorPosAfterOrder = 1;
	}
	else if ( m_rbDirection->GetValue() )
	{
		nCursorPosAfterOrder = 2;
	}
	else if ( m_rbOpenClose->GetValue() )
	{
		nCursorPosAfterOrder = 3;
	}
	else if ( m_rbPrice->GetValue() )
	{
		nCursorPosAfterOrder = 4;
	}
	else if ( m_rbVol->GetValue() )
	{
		nCursorPosAfterOrder = 5;
	}
	else
	{

	}

	if ( nCursorPosAfterOrder != pOrderCFG->nCursorPosAfterOrder )
	{
		bChanged = true;
		pOrderCFG->nCursorPosAfterOrder = nCursorPosAfterOrder;
	}
	int theVolume[9]={0};
	string str ;
	int nValue = 0;
	str = m_text1->GetValue();
	theVolume[0] = atoi( str.c_str() );
	str = m_text2->GetValue();
	theVolume[1] = atoi( str.c_str() );
	str = m_text3->GetValue();
	theVolume[2] = atoi( str.c_str() );
	str = m_text4->GetValue();
	theVolume[3]= atoi( str.c_str() );
	str = m_text5->GetValue();
	theVolume[4] = atoi( str.c_str() );
	str = m_text6->GetValue();
	theVolume[5]= atoi( str.c_str() );
	str = m_text7->GetValue();
	theVolume[6] = atoi( str.c_str() );
	str = m_text8->GetValue();
	theVolume[7] = atoi( str.c_str() );
	str = m_text9->GetValue();
	theVolume[8] = atoi( str.c_str() );
	if(pOrderCFG->Volume.copy(theVolume))
		bChanged=true;



	str = m_tcBuyHotKey->GetValue();
	if ( !str.empty() && str[0] != pOrderCFG->cBuyHotKey )
	{
		pOrderCFG->cBuyHotKey = str[0];
		bChanged = true;
	}

	str = m_tcSelHotKey->GetValue();
	if ( !str.empty() && str[0] != pOrderCFG->cSelHotKey )
	{
		pOrderCFG->cSelHotKey = str[0];
		bChanged = true;
	}

	str = m_tcOpenOrderHotKey->GetValue();
	if ( !str.empty() && str[0] != pOrderCFG->cOpenOrderHotKey )
	{
		pOrderCFG->cOpenOrderHotKey = str[0];
		bChanged = true;
	}

	str = m_tcCloseTodayVolumnHotKey->GetValue();
	if ( !str.empty() && str[0] != pOrderCFG->cCloseTodayVolumnHotKey )
	{
		pOrderCFG->cCloseTodayVolumnHotKey = str[0];
		bChanged = true;
	}

	str = m_tcCloseOrderHotKey->GetValue();
	if ( !str.empty() && str[0] != pOrderCFG->cCloseOrderHotKey )
	{
		pOrderCFG->cCloseOrderHotKey = str[0];
		bChanged = true;
	}

	if ( !m_strOpenOrderBtnHotKey.IsEmpty() && 
		m_strOpenOrderBtnHotKey != wxString( pOrderCFG->szOpenOrderBtnHotKey ))
	{
		pOrderCFG->szOpenOrderBtnHotKey.copy(m_strOpenOrderBtnHotKey.c_str());
		bChanged = true;
	}

	if ( !m_strOrderParkedBtnHotKey.IsEmpty() && 
		m_strOrderParkedBtnHotKey != wxString( pOrderCFG->szOrderParkedBtnHotKey ))
	{
		pOrderCFG->szOrderParkedBtnHotKey.copy(m_strOrderParkedBtnHotKey.c_str());
		bChanged = true;
	}

	return bChanged;
} 

void CGeneralSetPanel::OnChangeSel( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nId = event.GetId();
	switch( nId )
	{
	case ID_CBX_OPENORDERBTNHOTKEY:
		{
			wxString strOpenOrderBtnHotKey = m_cbxOpenOrderBtnHotKey->GetValue();
			if ( strOpenOrderBtnHotKey == m_strOpenOrderBtnHotKey )
			{
				return;
			}

			if ( !pCfgMgr->IsShortKeyUsable(strOpenOrderBtnHotKey.c_str()) )
			{
				wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN) ,LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
				m_cbxOpenOrderBtnHotKey->SetValue( m_strOpenOrderBtnHotKey );
			}
			else
			{
				pCfgMgr->AddUsableShortCutKey(m_strOpenOrderBtnHotKey.c_str());
				m_strOpenOrderBtnHotKey = strOpenOrderBtnHotKey;	
				pCfgMgr->DelUsableShortCutKey(strOpenOrderBtnHotKey.c_str());
			}
		}
		break;
	case ID_CBX_ORDERPARKEDBTNHOTKEY:
		{
			wxString strOrderParkedBtnHotKey = m_cbxOrderParkedBtnHotKey->GetValue();
			if ( strOrderParkedBtnHotKey == m_strOrderParkedBtnHotKey )
			{
				return;
			}

			if ( !pCfgMgr->IsShortKeyUsable(strOrderParkedBtnHotKey.c_str()) )
			{
				wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
				m_cbxOrderParkedBtnHotKey->SetValue( m_strOrderParkedBtnHotKey );
			}
			else
			{
				pCfgMgr->AddUsableShortCutKey(m_strOrderParkedBtnHotKey.c_str());
				m_strOrderParkedBtnHotKey = strOrderParkedBtnHotKey;
				pCfgMgr->DelUsableShortCutKey(strOrderParkedBtnHotKey.c_str());
			}
		}
		break;
	default:
		break;
	}
}

void CGeneralSetPanel::OnCharBuy( wxKeyEvent& event )
{
	char ch = event.GetKeyCode();
	string str = m_tcSelHotKey->GetValue().c_str();
	if ( !str.empty() && str[0] == ch )
	{
		m_tcBuyHotKey->SetValue("");
		m_tcBuyHotKey->SetFocus();
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
	}
	else
	{
		event.Skip();
	}
}

void CGeneralSetPanel::OnCharSel( wxKeyEvent& event )
{
	char ch = event.GetKeyCode();
	string str = m_tcBuyHotKey->GetValue().c_str();
	if ( !str.empty() && str[0] == ch )
	{
		m_tcSelHotKey->SetValue("");
		m_tcSelHotKey->SetFocus();
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
	}
	else
	{
		event.Skip();
	}
}

void CGeneralSetPanel::OnCharOpen( wxKeyEvent& event )
{
	char ch = event.GetKeyCode();
	string str1 = m_tcCloseTodayVolumnHotKey->GetValue().c_str();
	string str2 = m_tcCloseOrderHotKey->GetValue().c_str();
	if ( (!str1.empty() && str1[0] == ch) 
		||( !str2.empty() && str2[0] == ch ))
	{
		m_tcOpenOrderHotKey->SetValue("");
		m_tcOpenOrderHotKey->SetFocus();
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
	}
	else
	{
		event.Skip();
	}
}

void CGeneralSetPanel::OnCharCloseToday( wxKeyEvent& event )
{
	char ch = event.GetKeyCode();
	string str1 = m_tcOpenOrderHotKey->GetValue().c_str();
	string str2 = m_tcCloseOrderHotKey->GetValue().c_str();
	if ( (!str1.empty() && str1[0] == ch) 
		||( !str2.empty() && str2[0] == ch ))
	{
		m_tcCloseTodayVolumnHotKey->SetValue("");
		m_tcCloseTodayVolumnHotKey->SetFocus();
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
	}
	else
	{
		event.Skip();
	}
}

void CGeneralSetPanel::OnCharClose( wxKeyEvent& event )
{
	char ch = event.GetKeyCode();
	string str1 = m_tcOpenOrderHotKey->GetValue().c_str();
	string str2 = m_tcCloseTodayVolumnHotKey->GetValue().c_str();
	if ( (!str1.empty() && str1[0] == ch) 
		||( !str2.empty() && str2[0] == ch ))
	{
		m_tcCloseOrderHotKey->SetValue("");
		m_tcCloseOrderHotKey->SetFocus();
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
	}
	else
	{
		event.Skip();
	}
}

