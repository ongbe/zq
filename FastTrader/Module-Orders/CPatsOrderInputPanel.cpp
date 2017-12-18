#include "StdAfx.h"
#include "CPatsOrderInputPanel.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Orders/COrderInsertEntity.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "wx\odcombo.h"


using namespace util;

const int nFW = 8, nFH = 20;
const int nVW = nFW+2, nVH = nFH+2;
const int nTitleWidth = nFW*7;
const int nInputWidth = nFW*14;

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(CPatsOrderInputPanel, CBaseInputPanel)
	EVT_PAINT(OnPaint)
	EVT_SHOW(OnShow)

//	EVT_COMBOX_DROPDOWN(ID_OISTD_INSTRUMENTID_COMBOCTRL, OnInstrumentIdListDropdown)

	EVT_TEXT(ID_OIPATS_INSTRUMENTID_TEXT, OnInstrumentIdTextChanged)
	EVT_TEXT_MAXLEN(ID_OIPATS_INSTRUMENTID_TEXT, OnTextMaxLen) 

	EVT_STATIC_LEFT_UP(OnStaticLeftUp)
	EVT_SPIN_UP(ID_OIPATS_PRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_OIPATS_PRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_OIPATS_LIMITPRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_OIPATS_LIMITPRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_OIPATS_VOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_OIPATS_VOLUME_SPINCTRL, OnVolumeSpinDown)

    EVT_STATIC_LEFT_UP(OnStaticLeftUp)

	//EVT_COMMAND(wxID_ANY, wxEVT_TEXT_FOCUS, OnInstrumentIDClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_CODECLICKED, OnNextCodeClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_PRICECLICKED, OnNextPriceClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_VOLUMECLICKED, OnNextVolumeClicked) 

	EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_CODE, OnMouseInputCode) 
	EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_DIGIT, OnMouseInputDigit) 
	EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_VOLUME, OnMouseInputVolume) 

	EVT_CFG_CHANGE(OnCfgChanged)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CPatsOrderInputPanel::CPatsOrderInputPanel(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_MouseHandyPopupPane(NULL),
	m_MouseHandyPopupPaneVolume(NULL),
	m_MouseHandyPopupPaneDigit(NULL),
	m_bIsLockForSetChange(FALSE),
	m_bNoKeyChangeText(TRUE),
	m_colorBak(0,0,0),
	m_bIsInitOK(FALSE),
	m_nViewStyle(0),
	m_fPriceTick(0.0)
{

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	//oldfont.SetPointSize(9);
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);


//	m_MouseHandyPopupPane = new CMouseHandyInputPane(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400), wxNO_BORDER|wxSTATIC_BORDER);	//
//	m_MouseHandyPopupPane->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel::OnCodePanelKeyAscii),NULL,this);
	m_MouseHandyPopupPaneVolume = new MouseHandyInputPaneVolume(this);
	m_MouseHandyPopupPaneVolume->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CPatsOrderInputPanel::OnVolumePanelKeyAscii),NULL,this);
	m_MouseHandyPopupPaneDigit = new CMouseHandyInputPaneDigit(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400), wxNO_BORDER|wxSTATIC_BORDER);
	m_MouseHandyPopupPaneDigit->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CPatsOrderInputPanel::OnPricePanelKeyAscii),NULL,this);

	
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;

	ctlPos.y-=nVH;

	
	{		// 合约
	ctlPos.y += nVH;
	ctlPos.x = 5;

	// label合约
	pWin = new wxStaticText( this, ID_OIPATS_INSTRUMENTID_STATIC, 
			LOADSTRING(STATIC_CONTRACT_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrCodeList;
	std::set<std::string> outData;
	// 获取合约列表
	if(DEFAULT_SVR()!=NULL)
		DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		strArrCodeList.Add(*it);
	}
	
	//ctlPos=wxPoint(5+nFW*6, 5);
	ctlPos.x += nFW*6;
	
	//ctlSz.Set(116, 20);
	// text合约
	wxTextCtrl* pText = new wxTextCtrl(this, ID_OIPATS_INSTRUMENTID_TEXT, 
			wxEmptyString, ctlPos, wxSize(nInputWidth, nFH), 0);
	pText->SetMaxLength(30);
	pText->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(CPatsOrderInputPanel::OnCodeClicked), NULL, this);
	
	//ctlSz.Set(16, 20);
	//WXLOG_INFO("wxOwnerDrawnComboBox--%d",GetTickCount());
	// combolist合约
	pWin = new wxOwnerDrawnComboBox(this, ID_OIPATS_INSTRUMENTID_COMBOCTRL, wxT(""), 
			wxPoint(-1000, ctlPos.y), wxSize(nTitleWidth, nFH), strArrCodeList, wxCB_DROPDOWN);//|wxCB_SORT);
	//WXLOG_INFO("wxOwnerDrawnComboBox--%d",GetTickCount());
	}


	{		// 下单类型
	ctlPos.y += nVH;
	ctlPos.x = 5;

	// label下单类型
	pWin = new wxStaticText( this, ID_OIPATS_ORDERTYPE_STATIC, 
			LOADSTRING(STATIC_ORDERTYPE_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrOrderType;
	strArrOrderType.Add(LOADSTRING(ORDERTYPE_LIMIT));
	strArrOrderType.Add(LOADSTRING(ORDERTYPE_MARKET));
	strArrOrderType.Add(LOADSTRING(ORDERTYPE_STOP));
	strArrOrderType.Add(LOADSTRING(ORDERTYPE_STOPLIMIT));
	
	//ctlPos=wxPoint(5+nFW*6, 5);
	ctlPos.x += nFW*6;
		
	//ctlSz.Set(16, 20);
	// combo下单类型
	wxOwnerDrawnComboBox* pCombo = new wxOwnerDrawnComboBox(this, ID_OIPATS_ORDERTYPE_COMBOBOX, wxT(""), 
			ctlPos, wxSize(nInputWidth, nFH), strArrOrderType, wxCB_READONLY);//|wxCB_SORT);
	pCombo->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CPatsOrderInputPanel::OnCmbOrderType), NULL, this);
	}


	{			//买卖方向
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OIPATS_BUYSELL_STATIC, LOADSTRING(STATIC_DIRECT_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrBS;
	strArrBS.Add(LOADSTRING(OLS_BUY));
	strArrBS.Add(LOADSTRING(OLS_SELL));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	//ctlSz.Set(nFW*8, 20);

	wxOwnerDrawnComboBox* pCmbBuySell = new wxOwnerDrawnComboBox(this, ID_OIPATS_BUYSELL_COMBOBOX, LOADSTRING(OLS_BUY), 
			ctlPos, wxSize(nInputWidth/2, nFH), strArrBS, wxCB_READONLY);
	pCmbBuySell->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CPatsOrderInputPanel::OnCmbBuySell), NULL, this);

	//ctlSz.Set(52, 20);
	pWin = new wxCheckBox( this, ID_OIPATS_HEDGE_CHECKBOX, LOADSTRING(OLS_HEDGE), 
			wxPoint(ctlPos.x+nInputWidth/2+6, ctlPos.y+4), wxSize(nInputWidth/2-6, nFH));
	if(DEFAULT_SVR()!=NULL) {
	if(!DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_HEDGE)) { 
		//ctlSz.Set(116, 20);
		pWin = FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX, this);
		if(pWin!=NULL) { 
			pWin->SetSize(wxSize(nInputWidth, nFH));
			pWin->Show(true);
		}

		pWin = FindWindowById(ID_OIPATS_HEDGE_CHECKBOX, this);
		if(pWin!=NULL) { 
			//pWin->SetPosition(wxPoint(ctlPos.x-1000, ctlPos.y+4));
			pWin->Show(false);
		}
	}
	}

	}


	{			//开平
	ctlPos.y += nVH;
	ctlPos.x = 5;

	ToggleStaticText* itemOpenClose = new ToggleStaticText( this, ID_OIPATS_AUTOOPENCLOSE_STATIC, 
			LOADSTRING(STATIC_OFFSETFLAG_TEXT), LOADSTRING(STATIC_OFFSETFLAG_AUTO));
	itemOpenClose->SetPosition(ctlPos);
	itemOpenClose->SetSize(wxSize(nTitleWidth, nFH));

	wxArrayString strArrOC;
	strArrOC.Add(LOADSTRING(OLS_OPEN));
	//strArrOC.Add(LOADSTRING(OLS_CLOSE_TODAY));
	strArrOC.Add(LOADSTRING(OLS_CLOSE));
	
	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	//ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pCmbOpenClose = new wxOwnerDrawnComboBox(this, ID_OIPATS_OPENCLOSE_COMBOBOX,LOADSTRING(OLS_OPEN), 
			ctlPos, wxSize(nInputWidth, nFH), strArrOC, wxCB_READONLY);
	pCmbOpenClose->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CPatsOrderInputPanel::OnCmbOpenClose), NULL, this);
	}


	{			//价格
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OIPATS_AUTOTRACKPRICE_STATIC, LOADSTRING(STATIC_PRICE_TEXT), ctlPos, wxSize(nTitleWidth, nFH));
	//ToggleStaticText* itemAutoTrackPrice = new ToggleStaticText( this, ID_OIPATS_AUTOTRACKPRICE_STATIC, 
	//		LOADSTRING(STATIC_LIMITPRICE_TEXT), LOADSTRING(STATIC_MARKETPRICE_TEXT));
	////ctlPos.y += nVH;
	//itemAutoTrackPrice->SetPosition(ctlPos);
	//itemAutoTrackPrice->SetSize(wxSize(nTitleWidth, nFH));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	CreateFloatSpinCtrl_Price(ctlPos, wxSize(nInputWidth, nFH), ID_OIPATS_PRICE_TEXT, ID_OIPATS_PRICE_SPINCTRL);
	}


	{			//限价
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OIPATS_AUTOTRACKLIMITPRICE_STATIC, LOADSTRING(STATIC_LIMITPRICE_TEXT), ctlPos, wxSize(nTitleWidth, nFH));
	//ToggleStaticText* itemAutoTrackPrice = new ToggleStaticText( this, ID_OIPATS_AUTOTRACKPRICE_STATIC, 
	//		LOADSTRING(STATIC_LIMITPRICE_TEXT), LOADSTRING(STATIC_MARKETPRICE_TEXT));
	////ctlPos.y += nVH;
	//itemAutoTrackPrice->SetPosition(ctlPos);
	//itemAutoTrackPrice->SetSize(wxSize(nTitleWidth, nFH));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	CreateFloatSpinCtrl_Price(ctlPos, wxSize(nInputWidth, nFH), ID_OIPATS_LIMITPRICE_TEXT, ID_OIPATS_LIMITPRICE_SPINCTRL);
	}

	{			//手数
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OIPATS_VOLUME_STATIC, LOADSTRING(STATIC_VOLUME_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	CreateFloatSpinCtrl_Volume(ctlPos, wxSize(nInputWidth, nFH));
	}


	// 这里不需要处理，
	// 语言问题会在ResetLanguage()里完成
	{			//交易账号
	ctlPos.y += nVH;
	ctlPos.x = 5;
	pWin = new wxStaticText( this, ID_OIPATS_ACCOUNT_STATIC, LOADSTRING(STATIC_ACCOUNT_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrAccount;
	string strInvestorID, strBrokerID;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID, strBrokerID);
		strArrAccount.Add(wxT(strInvestorID));
	}
	else
	{
		set<string> setAccount;
		if(DEFAULT_SVR()!=NULL)
			DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID, strBrokerID, &setAccount);
		if(setAccount.size()!=0) {
			for(set<string>::iterator it = setAccount.begin(); it != setAccount.end(); it++)
				strArrAccount.Add(wxT(*it));
		}
	}
	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pComboBox = new wxOwnerDrawnComboBox(this, ID_OIPATS_ACCOUNT_COMBOCTRL, 
		wxEmptyString, ctlPos, wxSize(nInputWidth, nFH), strArrAccount, wxCB_DROPDOWN);//|wxCB_SORT);
	pComboBox->GetTextCtrl()->SetMaxLength(20);
	if(pComboBox->GetCount()>0)
		pComboBox->Select(0);
	}

	InitOrderView();
	SetSellBuyColor();

	DynamicShowCtrl();

	m_bIsInitOK = TRUE;

	ResetLanguage();

}

CPatsOrderInputPanel::~CPatsOrderInputPanel()
{
	if(m_MouseHandyPopupPane)
	{
		delete m_MouseHandyPopupPane;
		m_MouseHandyPopupPane = NULL;
	}
	if(m_MouseHandyPopupPaneDigit)
	{
		delete m_MouseHandyPopupPaneDigit;
		m_MouseHandyPopupPaneDigit = NULL;
	}
	if(m_MouseHandyPopupPaneVolume)
	{
		delete m_MouseHandyPopupPaneVolume;
		m_MouseHandyPopupPaneVolume = NULL;
	}
}

CPatsOrderInputPanel* CPatsOrderInputPanel::CreatePane(wxWindow* parent)
{
    return new CPatsOrderInputPanel(parent, wxID_ANY);
}

wxTextCtrl* CPatsOrderInputPanel::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit, wxSize& ctrSize, int nTextID, int nSpinID)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(ctrSize.x, ctrSize.y);
	ctlSz.x-=16;
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, nTextID, wxEmptyString, ctlPos, ctlSz, 0 );//ID_OISTD_PRICE_TEXT

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(CPatsOrderInputPanel::OnPriceClicked), NULL, this);
	itemTextCtrl3->SetMaxLength(20);

	ctlPos.x += (ctrSize.x-16);
	ctlSz.Set(16, ctrSize.y);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, nSpinID, ctlPos, ctlSz, wxSP_VERTICAL );//ID_OISTD_PRICE_SPINCTRL
    itemSpinButton4->SetRange(-1000000, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

//wxTextCtrl* CPatsOrderInputPanel::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit, wxSize& ctrSize)
//{
//	wxPoint ctlPos(0, 0);
//	wxSize ctlSz(0, 0);
//
//	ctlPos=ctrPosInit;
//
//	ctlSz.Set(ctrSize.x, ctrSize.y);
//	ctlSz.x-=16;
//	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_OISTD_PRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );
//
//	itemTextCtrl3->Connect(ID_OISTD_PRICE_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(CPatsOrderInputPanel::OnPriceClicked), NULL, this);
//	itemTextCtrl3->SetMaxLength(20);
//
//	ctlPos.x += (ctrSize.x-16);
//	ctlSz.Set(16, ctrSize.y);
//    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_OISTD_PRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
//    itemSpinButton4->SetRange(-1000000, 1000000);
//    itemSpinButton4->SetValue(0);
//    return itemTextCtrl3;
//}

wxTextCtrl* CPatsOrderInputPanel::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit, wxSize& ctrSize)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(ctrSize.x, ctrSize.y);
	ctlSz.x-=16;
	wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_OIPATS_VOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(CPatsOrderInputPanel::OnVolumeClicked), NULL, this);
	itemTextCtrl3->SetMaxLength(4);

	ctlPos.x += (ctrSize.x-16);
	ctlSz.Set(16, ctrSize.y);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_OIPATS_VOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 9999);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void CPatsOrderInputPanel::InsertCtrlID2TabCtrl(vector<UINT>& tabCtrlList)
{
	for(int i=0; i<(int)m_arrTabCtrl.size(); i++) {
		tabCtrlList.push_back((UINT)m_arrTabCtrl[i]);
	}
}

BOOL CPatsOrderInputPanel::CheckCtrlID(int nCtrlID)
{
	for(int i=0; i<(int)m_arrTabCtrl.size(); i++) {
		if(nCtrlID == m_arrTabCtrl[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

void CPatsOrderInputPanel::DynamicShowCtrl()
{
	int nPanelHeight = 22*7+10;
	//int nPanelHeight = 22*8+10;
	int nFW = 8, nFH = 20;
	int nVW = nFW+2, nVH = nFH+2;
	wxPoint ctlPos(5, 5);

	wxWindow* pWin = NULL;

	for(int i=ID_OIPATS_FIRST+1; i<SUMNUM_OIPATS_ID; i++) {
		pWin = FindWindowById(i, this);
		if(pWin != NULL)
			pWin->Show(false);
	}

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	int nRowCount = pCfgMgr->GetListColCount(GID_ORDER_INSERT_PANEL);
	//nRowCount = 8;

	if(nRowCount>1) {
		float fIntval = (float)(nPanelHeight-nFH*nRowCount-10)/(float)(nRowCount-1);
		fIntval < 0.0f ? fIntval=0.0f : 0;
		nVH = nFH+fIntval+0.3f;
	}

	int i=0;

	ctlPos.y-=nVH;
	m_arrTabCtrl.clear();
	for(i=0; i<nRowCount; i++) {

		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_ORDER_INSERT_PANEL, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<=ORDERNEWINPUT_MINNUM || pColCfg->id>=ORDERNEWINPUT_MAXNUM)
			continue;

		//合约ID
		if(pColCfg->id==ORDERNEWINPUT_INSTRUMENTID_ROW_ID) {
		//{	
			m_arrTabCtrl.push_back(ID_OIPATS_INSTRUMENTID_TEXT);

			ctlPos.y += nVH;
			ctlPos.x = 5;
			pWin = FindWindowById(ID_OIPATS_INSTRUMENTID_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}

//	ORDERNEWINPUT_ORDERTYPE_ROW_ID,		              				//Pats:下单类型
		//下单类型
		if(pColCfg->id==ORDERNEWINPUT_ORDERTYPE_ROW_ID) {
		//{	
			m_arrTabCtrl.push_back(ID_OIPATS_ORDERTYPE_COMBOBOX);

			ctlPos.y += nVH;
			ctlPos.x = 5;
			pWin = FindWindowById(ID_OIPATS_ORDERTYPE_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_ORDERTYPE_COMBOBOX, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}

		//买卖方向
		if(pColCfg->id==ORDERNEWINPUT_BUYSELL_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OIPATS_BUYSELL_COMBOBOX);
			/////////////m_setTabCtrl.insert(ID_OISTD_HEDGE_CHECKBOX);

			ctlPos.y += nVH;
			ctlPos.x = 5;

			pWin = FindWindowById(ID_OIPATS_BUYSELL_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			pWin = FindWindowById(ID_OIPATS_HEDGE_CHECKBOX, this);
			if(pWin!=NULL && DEFAULT_SVR()&&DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_HEDGE)) { 
				pWin->SetPosition(wxPoint(ctlPos.x+nInputWidth/2+6, ctlPos.y+1));
				pWin->Show(true);
			}
		}

		//开平
		if(pColCfg->id==ORDERNEWINPUT_OPENCLOSE_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OIPATS_OPENCLOSE_COMBOBOX);

			ctlPos.y += nVH;
			ctlPos.x = 5;

			pWin = FindWindowById(ID_OIPATS_AUTOOPENCLOSE_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}

		//价格
		if(pColCfg->id==ORDERNEWINPUT_PRICE_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OIPATS_PRICE_TEXT);

			ctlPos.y += nVH;
			ctlPos.x = 5;
			pWin = FindWindowById(ID_OIPATS_AUTOTRACKPRICE_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_PRICE_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += (nInputWidth-16);
			pWin = FindWindowById(ID_OIPATS_PRICE_SPINCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}

//	ORDERNEWINPUT_LIMITPRICE_ROW_ID,		           				//Pats:限价
		//限价
		if(pColCfg->id==ORDERNEWINPUT_LIMITPRICE_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OIPATS_LIMITPRICE_TEXT);

			ctlPos.y += nVH;
			ctlPos.x = 5;
			pWin = FindWindowById(ID_OIPATS_AUTOTRACKLIMITPRICE_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += (nInputWidth-16);
			pWin = FindWindowById(ID_OIPATS_LIMITPRICE_SPINCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}

		//手数
		if(pColCfg->id==ORDERNEWINPUT_VOLUME_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OIPATS_VOLUME_TEXT);

			ctlPos.y += nVH;
			ctlPos.x = 5;
			pWin = FindWindowById(ID_OIPATS_VOLUME_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_VOLUME_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += (nInputWidth-16);
			pWin = FindWindowById(ID_OIPATS_VOLUME_SPINCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}

		//交易账号
		if(pColCfg->id==ORDERNEWINPUT_TRADEACCOUNT_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OIPATS_ACCOUNT_COMBOCTRL);

			ctlPos.y += nVH;
			ctlPos.x = 5;
			pWin = FindWindowById(ID_OIPATS_ACCOUNT_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OIPATS_ACCOUNT_COMBOCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->Show(true);
			}
		}
    }

}

void CPatsOrderInputPanel::SetViewMode(int nViewMode)
{
/*
	wxWindow* window = NULL;
	for(int i=ID_OISTD_FIRST+1; i<SUMNUM_OISTD_ID; i++) {
		window = FindWindowById(i, this);
		if(window!=NULL) window->Enable(true);
	}
	switch(nViewMode) {
	case 0:		// 自由下单
		break;
	case 1:		// 修改条件下单
		window=FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
		window->Enable(false);
		break;
	case 2:		// 修改盈损下单
		window=FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_VOLUME_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_HEDGE_CHECKBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_PRICE_SPINCTRL, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_VOLUME_SPINCTRL, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_ACCOUNT_COMBOCTRL, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
		window->Enable(false);
		break;
	case 3:		// 下盈损单
		window=FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
		window->Enable(false);
		SetOpenClose(0);
		break;
	case 4:		// 下停止单
		window=FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OISTD_PRICE_SPINCTRL, this);
		window->Enable(false);
		SetOpenClose(2);
		break;
	};
*/
}

BOOL CPatsOrderInputPanel::NeedMouseHandyPane_Code()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputInstrumentId;
}

BOOL CPatsOrderInputPanel::NeedMouseHandyPane_Volume()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputVolume;
}

BOOL CPatsOrderInputPanel::NeedMouseHandyPane_Price()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputPrice;
}

// 可以输入缩写和拼音
BOOL CPatsOrderInputPanel::NeedInputAbbreviation()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bInputAbbreviation;
}

int CPatsOrderInputPanel::GetContractListStyle()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->nContractListStyle;
}

void CPatsOrderInputPanel::SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent)
{
	m_bNoKeyChangeText = FALSE;
	wxTextCtrl* window=(wxTextCtrl*)FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
	if(window!=NULL) {
		window->SetLabel(strInstrumentId);
		window->SetSelection(strInstrumentId.length(),strInstrumentId.length());
	}
	m_bNoKeyChangeText = TRUE;
}

wxString CPatsOrderInputPanel::GetInstrumentId()
{
	wxTextCtrl* window=(wxTextCtrl*)FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
	return window->GetValue();
}

void CPatsOrderInputPanel::SetAccount(wxString& strAccount)
{
    wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_ACCOUNT_COMBOCTRL,this);
	window->SetValue(strAccount);
}

wxString CPatsOrderInputPanel::GetAccount()
{
	wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_ACCOUNT_COMBOCTRL, this);
	return window->GetValue();
}

void CPatsOrderInputPanel::SetOrderType(int nOrderType)
{
    wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_ORDERTYPE_COMBOBOX,this);
	window->SetSelection(nOrderType);
	ResetOrderTypeView(nOrderType); 
}

int CPatsOrderInputPanel::GetOrderType()
{
	wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_ORDERTYPE_COMBOBOX, this);
	return window->GetSelection();
}

void CPatsOrderInputPanel::SetDirection(BOOL bIsBuy)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX,this);
	pComboBox->SetSelection(bIsBuy?0:1);
	SetSellBuyColor();
}

// 获得买卖方向，TRUE为买入
BOOL CPatsOrderInputPanel::GetDirection()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX,this);
	return pComboBox->GetSelection()==0;
}

// 设置开平仓模式，0开仓; 1平今; 2平仓; -1未知
void CPatsOrderInputPanel::SetOpenClose(int nOCMode)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX,this);
	if(pComboBox->IsEnabled())
    {
        if(nOCMode==0)
		    pComboBox->SetSelection(0);
        else if(nOCMode==2)
		    pComboBox->SetSelection(1);
        else 
		    pComboBox->SetSelection(-1);
    }
}

// 获得开平仓模式，0开仓; 1平今; 2平仓; -1未知
int CPatsOrderInputPanel::GetOpenClose()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX,this);
	int nsel=pComboBox->GetSelection();
    if(nsel==0) return 0;
    else if(nsel=1) return 2;
    else return -1;
}

void CPatsOrderInputPanel::SetPriceText(wxString& strPrice)
{
	wxWindow* window=FindWindowById(ID_OIPATS_PRICE_TEXT, this);
	if(window->IsEnabled()) {
		window->SetLabel(strPrice);
	}
	else {
		window->SetLabel(wxT(""));
	}
}

wxString CPatsOrderInputPanel::GetPriceText()
{
	wxWindow* window=FindWindowById(ID_OIPATS_PRICE_TEXT, this);
	return window->GetLabel();
}

void CPatsOrderInputPanel::SetPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_OIPATS_PRICE_TEXT,this);
	if(window->IsEnabled()) {
		window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
	}
	else {
		window->SetLabel(wxT(""));
	}
}

double CPatsOrderInputPanel::GetPrice()
{
	wxWindow* window=FindWindowById(ID_OIPATS_PRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void CPatsOrderInputPanel::SetLimitPriceText(wxString& strPrice)
{
	wxWindow* window=FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
	if(window->IsEnabled()) {
		window->SetLabel(strPrice);
	}
	else {
		window->SetLabel(wxT(""));
	}
}

wxString CPatsOrderInputPanel::GetLimitPriceText()
{
	wxWindow* window=FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
	return window->GetLabel();
}

void CPatsOrderInputPanel::SetLimitPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_OIPATS_LIMITPRICE_TEXT,this);
	if(window->IsEnabled()) {
		window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
	}
	else {
		window->SetLabel(wxT(""));
	}
}

double CPatsOrderInputPanel::GetLimitPrice()
{
	wxWindow* window=FindWindowById(ID_OIPATS_LIMITPRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void CPatsOrderInputPanel::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_OIPATS_VOLUME_TEXT,this);
	window->SetLabel(strText);
}

int CPatsOrderInputPanel::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_OIPATS_VOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

// 设置是否保值
void CPatsOrderInputPanel::SetHedge(BOOL bIsHedge)
{
   wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OIPATS_HEDGE_CHECKBOX,this);
   pCheckBox->SetValue(bIsHedge==TRUE);
}

BOOL CPatsOrderInputPanel::GetHedge()
{
   wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OIPATS_HEDGE_CHECKBOX,this);
   return pCheckBox->GetValue();
}

// 设置自动开平模式
void CPatsOrderInputPanel::SetAutoOpenClose(BOOL bIsAuto)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIPATS_AUTOOPENCLOSE_STATIC,this);
   pSText->SetNormal(!bIsAuto);
}

BOOL CPatsOrderInputPanel::GetAutoOpenClose()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIPATS_AUTOOPENCLOSE_STATIC,this);
   return !pSText->IsNormal();
}

// 设置价格跟盘模式
void CPatsOrderInputPanel::SetAutoTrackPrice(BOOL bIsTrack)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIPATS_AUTOTRACKPRICE_STATIC,this);
   pSText->SetNormal(!bIsTrack);
}

BOOL CPatsOrderInputPanel::GetAutoTrackPrice()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIPATS_AUTOTRACKPRICE_STATIC,this);
   return !pSText->IsNormal();
}

// 设置价格跟盘模式
void CPatsOrderInputPanel::SetAutoTrackLimitPrice(BOOL bIsTrack)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIPATS_AUTOTRACKLIMITPRICE_STATIC,this);
   pSText->SetNormal(!bIsTrack);
}

BOOL CPatsOrderInputPanel::GetAutoTrackLimitPrice()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIPATS_AUTOTRACKLIMITPRICE_STATIC,this);
   return !pSText->IsNormal();
}

void CPatsOrderInputPanel::InitOrderView()
{
	wxString strText(wxEmptyString);
	SetInstrumentId(strText);
	SetOrderType(OrderType_PATS_LIMIT);
    SetDirection(TRUE);
	SetOpenClose(0);		// 设置开平仓模式，0 开仓，1 平仓
    SetPriceText(strText);
    SetLimitPriceText(strText);
    SetQty(0);				// 设置手数
	SetHedge(FALSE);		// 设置是否保值
	SetAccount(strText);
}

void CPatsOrderInputPanel::ClearViewData(int nMode)
{
	wxString strText;
	//1 下单后不清空下单板, 2 全部清空, 3 清空价格和手数, 4 只清空手数
	switch(nMode) {
	case 2:
		SetInstrumentId(strText);
		SetDirection(TRUE);
		SetOpenClose(0);
		SetPriceText(strText);
		SetLimitPriceText(strText);
		FindWindowById(ID_OIPATS_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 3:
		SetPriceText(strText);
		SetLimitPriceText(strText);
		FindWindowById(ID_OIPATS_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 4:
		FindWindowById(ID_OIPATS_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	};
}

// 从数据结构中读数据并显示
void CPatsOrderInputPanel::ShowOrder(PlatformStru_InputOrder& req)
{
	wxString strText;

	LockForSetChange();

	strText = req.strAccount;
	SetAccount(strText);

	strText=req.InstrumentID;
	SetInstrumentId(strText);
	SetDirection(req.Direction==THOST_FTDC_D_Buy? 1 : 0);
	SetOpenClose(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? 0 : 1);		// 设置开平仓模式，0 开仓，1 平仓
	if(req.ContingentCondition==THOST_FTDC_CC_Immediately || req.OrderPriceType==THOST_FTDC_OPT_LimitPrice) {
		SetOrderType(OrderType_PATS_LIMIT);
		SetPrice(req.LimitPrice);
		strText=wxEmptyString;//LOADSTRING(OLS_MARKET);
		SetLimitPriceText(strText);
	}
	else if(req.ContingentCondition==THOST_FTDC_CC_Immediately || req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		SetOrderType(OrderType_PATS_MARKET);
		strText=wxEmptyString;//LOADSTRING(OLS_MARKET);
		SetPriceText(strText);
		SetLimitPriceText(strText);
	}
	else if(req.ContingentCondition==THOST_FTDC_CC_Touch || req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		SetOrderType(OrderType_PATS_STOP);
		SetPrice(req.StopPrice);
		strText=wxEmptyString;//LOADSTRING(OLS_MARKET);
		SetLimitPriceText(strText);
	}
	else if(req.ContingentCondition==THOST_FTDC_CC_Touch || req.OrderPriceType==THOST_FTDC_OPT_LimitPrice) {
		SetOrderType(OrderType_PATS_STOPLIMIT);
		SetPrice(req.StopPrice);
		SetLimitPrice(req.LimitPrice);
	}
	else {
		//SetPrice(req.LimitPrice);
	}
	SetQty(req.VolumeTotalOriginal);							// 设置手数
	SetHedge(req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge);		// 设置是否保值

	UnlockForSetChange();
}

void CPatsOrderInputPanel::SetSellBuyColor()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxColor BuyColor=DEFAULT_COLOUR;
	wxColor SellColor=wxColor(0,128,255);
	if(pCfgMgr) {
		LPORDER_CFG	pOrder=pCfgMgr->GetOrderCfg();
		if(pOrder) {
			BuyColor=wxColor(pOrder->BuyColor);
			SellColor=wxColor(pOrder->SellColor);
		}
	}
	wxColor clr=(GetDirection())?BuyColor:SellColor;
	m_colorBak = clr;

	wxWindow* pWin = NULL;
	for(int i=ID_OIPATS_INSTRUMENTID_STATIC; i<SUMNUM_OIPATS_ID; i++) {
		pWin = FindWindowById(i, this);
		if(pWin) pWin->SetBackgroundColour(m_colorBak);
	}
	//pWin = FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX, this);
	//if(pWin) pWin->SetBackgroundColour(m_colorBak);

	//SetBackgroundColour(clr);
	Refresh();

}

void CPatsOrderInputPanel::ResetOrderTypeView(int nSel) 
{
		//ID_OIPATS_PRICE_TEXT,
		//ID_OIPATS_LIMITPRICE_TEXT,
	wxString strValue;
	wxWindow* pWin = NULL;
	switch(nSel) {
	case OrderType_PATS_LIMIT:							// 限价
		pWin = FindWindowById(ID_OIPATS_PRICE_TEXT, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_OIPATS_PRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(false);
		//SetPriceText(strValue);
		SetLimitPriceText(strValue);
		break;
	case OrderType_PATS_MARKET:							// 市价
		pWin = FindWindowById(ID_OIPATS_PRICE_TEXT, this);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_OIPATS_PRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(false);
		SetPriceText(strValue);
		SetLimitPriceText(strValue);
		break;
	case OrderType_PATS_STOP:							// 停止单
		pWin = FindWindowById(ID_OIPATS_PRICE_TEXT, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_OIPATS_PRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(false);
		//SetPriceText(strValue);
		SetLimitPriceText(strValue);
		break;
	case OrderType_PATS_STOPLIMIT:						// 停止限价单
		pWin = FindWindowById(ID_OIPATS_PRICE_TEXT, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_PRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_OIPATS_LIMITPRICE_SPINCTRL, this);
		if(pWin) pWin->Enable(true);
		//SetPriceText(strValue);
		//SetLimitPriceText(strValue);
		break;
	};
}

void CPatsOrderInputPanel::OnInstrumentIdTextChanged(wxCommandEvent& event)
{
	
	if(!m_bIsInitOK)
		return;

	if(m_bIsLockForSetChange)
		return;

    wxString strInputTxt;
	wxWindow* pWin = FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
	if(pWin != NULL) strInputTxt=pWin->GetLabel();

	pWin = FindWindowById(ID_OIPATS_INSTRUMENTID_COMBOCTRL, this);
	if(pWin != NULL) pWin->SetLabel(strInputTxt);
	//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIKQ_LONGCODE_COMBOBOX);
	//pCmbLongCode->SetValue(event.GetString());

	// 一般调用父窗体的OnInstrumentIdTextChanged()函数来处理合约代码改变事件

	if(m_bIsLockForSetChange)
		return;

	// 处理数字快捷键
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr != NULL) {
		std::vector<NumberShortKey>* pVec = pMgr->GetNumberShortKeyVector();

		wxString strInput = event.GetString();
		for(int i=0; i<(int)pVec->size(); i++) {
			NumberShortKey& tSKey = (*pVec)[i];
			if(strInput.compare(tSKey.ShortKey)==0) {
				//m_bNoKeyChangeText = FALSE;
				wxString strSymbol = tSKey.Symbol;
				SetInstrumentId(strSymbol);
                strInputTxt=strSymbol;
                //m_bNoKeyChangeText = TRUE;
				break;
			}
		}
	}


	// 向父窗体发送合约代码改变事件
	wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, event.GetId());
    if(1)
    {
        std::string EvtParamStr(strInputTxt.c_str());
        DWORD EvtParamID;
        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
        {
            evtOut.SetInt((int)EvtParamID);
            if(GetParent())
                GetParent()->ProcessEvent(evtOut);
        }
    }
//	evtOut.SetString(event.GetString().c_str());
//	if(GetParent())
//		GetParent()->ProcessEvent(evtOut);
	
	// 处理拼音转换
	if(NeedInputAbbreviation() && m_bNoKeyChangeText) {
//		wxString strText = event.GetString();
		if(m_MouseHandyPopupPane != NULL) {
			if(strInputTxt.Length()>0) {
				if(m_nViewStyle!=STYLE_SunGard || GetContractListStyle()==1) {
					wxWindow *objRect =FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
					wxPoint pos = objRect->GetPosition();
					pos = ClientToScreen(pos);
					wxSize sz = objRect->GetSize();
					pos.y+=sz.y;
					m_MouseHandyPopupPane->SetPosition(pos);
					if(!m_MouseHandyPopupPane->IsShown()) {
							m_MouseHandyPopupPane->Show();
					}
					m_MouseHandyPopupPane->ShowView(strInputTxt, FALSE);
					set<string> InstrumentIDArray;
					if(DEFAULT_SVR()!=NULL)
						DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
					if(InstrumentIDArray.find(strInputTxt.c_str()) != InstrumentIDArray.end()) {
						m_MouseHandyPopupPane->Show(false);
					}
				}
				else {
					m_MouseHandyPopupPane->Show(false);
				}
			}
			else {
				m_MouseHandyPopupPane->Show(false);
			}
			FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT)->SetFocus();
		}
	}

}

void CPatsOrderInputPanel::OnTextMaxLen(wxCommandEvent& event)
{
	int nLen = event.GetInt();
}

void CPatsOrderInputPanel::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	int thisID = evt.GetId();
	if(evt.GetKeyCode()==WXK_RETURN || evt.GetKeyCode()==VK_RETURN) {
		if(m_MouseHandyPopupPane != NULL) {
			if(thisID == ID_OIPATS_INSTRUMENTID_TEXT && m_MouseHandyPopupPane->IsShown()) {
				m_MouseHandyPopupPane->Show(false);
				evt.Skip();
				return;
			}
		}
		if(thisID == ID_OIPATS_PRICE_TEXT && m_MouseHandyPopupPaneDigit->IsShown()) {
			m_MouseHandyPopupPaneDigit->Show(false);
			evt.Skip();
			return;
		}
	}
	else if(evt.GetKeyCode()==WXK_DOWN || evt.GetKeyCode()==VK_DOWN) {
		if(m_MouseHandyPopupPane != NULL) {
			if(thisID == ID_OIPATS_INSTRUMENTID_TEXT && m_MouseHandyPopupPane->IsShown()) {
				m_MouseHandyPopupPane->SetFocus();
				return;
			}
		}
	}
	else if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		switch(thisID) {
		case ID_OIPATS_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OIPATS_INSTRUMENTID_COMBOCTRL, this),wxOwnerDrawnComboBox);
			if(itemCombo != NULL) {
				int CurSel=0;
				BOOL bIsFound=FALSE, bIsEequal=FALSE;
				wxString strValue=GetInstrumentId();
				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++) {
					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
						bIsFound = TRUE;
						bIsEequal = itemCombo->GetString(CurSel).compare(strValue)==0;
						break;
					}
				}
				if(strValue.Length()==0) {
					bIsFound=TRUE;
					CurSel=0;
				}
				if(bIsFound) {
					m_bIsLockForSetChange=TRUE;
					if(bIsEequal) {
						itemCombo->SetSelection(CurSel>0 ? CurSel-1 : 0);
					}
					else {
						itemCombo->SetSelection(CurSel);
					}
					SetInstrumentId(itemCombo->GetValue());
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIPATS_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// 需要调用合约代码改变事件
					// 向父窗体发送合约代码改变事件
					wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, ID_OIPATS_INSTRUMENTID_TEXT);
                    if(1)
                    {
                        std::string EvtParamStr(itemCombo->GetValue().c_str());
                        DWORD EvtParamID;
                        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                        {
                            evtOut.SetInt((int)EvtParamID);
                            if(GetParent())
                                GetParent()->ProcessEvent(evtOut);
                        }
                    }
//					evtOut.SetString(itemCombo->GetValue().c_str());
//					if(GetParent())
//						GetParent()->ProcessEvent(evtOut);
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OIPATS_PRICE_TEXT:
			evtSpin.SetId(ID_OIPATS_PRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_OIPATS_LIMITPRICE_TEXT:
			evtSpin.SetId(ID_OIPATS_LIMITPRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_OIPATS_VOLUME_TEXT:
			evtSpin.SetId(ID_OIPATS_VOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		int thisID = evt.GetId();
		switch(thisID) {
		case ID_OIPATS_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OIPATS_INSTRUMENTID_COMBOCTRL, this),wxOwnerDrawnComboBox);
			if(itemCombo != NULL) {
				int CurSel=0;
				BOOL bIsFound=FALSE, bIsEequal=FALSE;
				wxString strValue=GetInstrumentId();
				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++) {
					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
						bIsFound = TRUE;
						bIsEequal = itemCombo->GetString(CurSel).compare(strValue)==0;
						break;
					}
				}
				if(strValue.Length()==0) {
					bIsFound=TRUE;
					CurSel=0;
				}
				if(bIsFound) {
					m_bIsLockForSetChange=TRUE;
					if(bIsEequal) {
						itemCombo->SetSelection(
								CurSel<(int)itemCombo->GetCount()-1 ? CurSel+1 : (int)itemCombo->GetCount()-1);
					}
					else {
						itemCombo->SetSelection(CurSel);
					}
					SetInstrumentId(itemCombo->GetValue());
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIPATS_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// 需要调用合约代码改变事件
					// 向父窗体发送合约代码改变事件
					wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, ID_OIPATS_INSTRUMENTID_TEXT);
                    if(1)
                    {
                        std::string EvtParamStr(itemCombo->GetValue().c_str());
                        DWORD EvtParamID;
                        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                        {
                            evtOut.SetInt((int)EvtParamID);
                            if(GetParent())
                                GetParent()->ProcessEvent(evtOut);
                        }
                    }
//					evtOut.SetString(itemCombo->GetValue().c_str());
//					if(GetParent())
//						GetParent()->ProcessEvent(evtOut);
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OIPATS_PRICE_TEXT:
			evtSpin.SetId(ID_OIPATS_PRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_OIPATS_LIMITPRICE_TEXT:
			evtSpin.SetId(ID_OIPATS_LIMITPRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_OIPATS_VOLUME_TEXT:
			evtSpin.SetId(ID_OIPATS_VOLUME_SPINCTRL);
			OnVolumeSpinUp(evtSpin);
			return;
			break;
		};
	}
	else {
		int thisID = evt.GetId();

		// 这里从xml中读是否要确认的配置
		CfgMgr* pMgr = CFG_MGR_DEFAULT();  
		if(pMgr != NULL) {
			LPORDER_CFG p = pMgr->GetOrderCfg();
			int nSrcChar = evt.GetKeyCode();
			int nChar = nSrcChar;
			if(nSrcChar >= VK_NUMPAD0 && nSrcChar <= VK_NUMPAD9)
				nChar = nSrcChar - VK_NUMPAD0 + '0';
			if(nSrcChar >= WXK_NUMPAD0 && nSrcChar <= WXK_NUMPAD9)
				nChar = nSrcChar - WXK_NUMPAD0 + '0';

			if(thisID == ID_OIPATS_BUYSELL_COMBOBOX) {
				if(nChar==p->cBuyHotKey) {
					SetDirection(TRUE);
					return;
				}
				else if(nChar==p->cSelHotKey) {
					SetDirection(FALSE);
					return;
				}
			}
			else if(thisID == ID_OIPATS_OPENCLOSE_COMBOBOX) {
				if(nChar==p->cOpenOrderHotKey) {
					SetOpenClose(0);
					return;
				}
				else if(nChar==p->cCloseTodayVolumnHotKey) {
					SetOpenClose(1);
					return;
				}
				else if(nChar==p->cCloseOrderHotKey) {
					SetOpenClose(2);
					return;
				}
			}
		}
	}
	evt.Skip();

}

void CPatsOrderInputPanel::OnStaticLeftUp(wxCommandEvent& event)
{
	ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(event.GetId(), this);
	if(event.GetId()==ID_OIPATS_AUTOOPENCLOSE_STATIC) {
		// 向父窗体发送自动开平仓改变事件
		wxCommandEvent evtOut(wxEVT_KQPANEL_AUTOOPENCLOSE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoOpenClose());
		if(GetParent())
			GetParent()->AddPendingEvent(evtOut);
	}
	else if(event.GetId()==ID_OIPATS_AUTOTRACKPRICE_STATIC) {
		// 向父窗体发送自动跟踪价格改变事件
		wxCommandEvent evtOut(wxEVT_KQPANEL_AUTOTRACKPRICE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoTrackPrice());
		if(GetParent())
			GetParent()->AddPendingEvent(evtOut);
	}

}

void CPatsOrderInputPanel::OnPriceSpinUp(wxSpinEvent& event)
{
	if(event.GetId()==ID_OIPATS_PRICE_SPINCTRL) {
		double fPrice=GetPrice();
		SetPrice(fPrice+m_fPriceTick);
	}
	else if(event.GetId()==ID_OIPATS_LIMITPRICE_SPINCTRL) {
		double fPrice=GetLimitPrice();
		SetLimitPrice(fPrice+m_fPriceTick);
	}
}

void CPatsOrderInputPanel::OnPriceSpinDown(wxSpinEvent& event)
{
	if(event.GetId()==ID_OIPATS_PRICE_SPINCTRL) {
		double price=GetPrice();
		SetPrice(price-m_fPriceTick);
	}
	else if(event.GetId()==ID_OIPATS_LIMITPRICE_SPINCTRL) {
		double price=GetLimitPrice();
		SetLimitPrice(price-m_fPriceTick);
	}
}

void CPatsOrderInputPanel::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void CPatsOrderInputPanel::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void CPatsOrderInputPanel::OnCmbOrderType(wxCommandEvent& evt)
{
	int nSel = evt.GetSelection();
	// 向父窗体发送下单类型改变事件
	wxCommandEvent evtOut(wxEVT_PATSPANEL_ORDERTYPE_CHANGED, evt.GetId());
	if(GetParent())
		GetParent()->AddPendingEvent(evtOut);
	
	ResetOrderTypeView(nSel); 
}

void CPatsOrderInputPanel::OnCmbBuySell(wxCommandEvent& evt)
{
//	int nSel = evt.GetSelection();
//	// 向父窗体发送自动跟踪价格改变事件
//	wxCommandEvent evtOut(wxEVT_KQPANEL_BUYSELLSEL_CHANGED, evt.GetId());
//	evtOut.SetInt(GetDirection());
//	if(GetParent())
//		GetParent()->AddPendingEvent(evtOut);

	if(GetAutoOpenClose()) {
		wxString strCode = GetInstrumentId();
		int nSumVolume = 0, nSumTodayVolume=0;
		BOOL bIsBuy = GetDirection();
		BOOL bIsHedge = GetHedge();
		// 可平数 总仓/今仓
		COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetAccount().c_str()));
		//if(nSumTodayVolume>0) {
		//	SetOpenClose(1);
		//	//SetInputQty(nSumTodayVolume);
		//}
		//else 
		if(nSumVolume>0){
			SetOpenClose(1);
			//SetOpenClose(2);
			//SetInputQty(nSumVolume-nSumTodayVolume);
		}
		else {
			SetOpenClose(0);
			//SetInputQty(GetDefaultVolume(strCode));
		}
	}
	
	SetSellBuyColor();

}

void CPatsOrderInputPanel::OnCmbOpenClose(wxCommandEvent& evt)
{
	SetAutoOpenClose(FALSE);
}

void CPatsOrderInputPanel::OnCodeClicked(wxMouseEvent& evt)//wxMouseEvent
{
	wxCommandEvent evttmp(wxEVT_CODECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void CPatsOrderInputPanel::OnPriceClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_PRICECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void CPatsOrderInputPanel::OnVolumeClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_VOLUMECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void CPatsOrderInputPanel::OnNextCodeClicked(wxCommandEvent& evt)
{
	wxString strText;
	if(NeedMouseHandyPane_Code()) {
		try {
			if(DEFAULT_SVR()!=NULL) {
			if(DEFAULT_SVR()->HaveShowInstumentComboList() && m_MouseHandyPopupPane == NULL) {
				std::map<string, set<string>> outData;
				if(DEFAULT_SVR()->GetProductID_InstrumentIDsByExchangeID(outData) > 0) {
					m_MouseHandyPopupPane = new CMouseHandyInputPane(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400), wxNO_BORDER|wxSTATIC_BORDER);	//
					m_MouseHandyPopupPane->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CPatsOrderInputPanel::OnCodePanelKeyAscii),NULL,this);
					m_MouseHandyPopupPane->SetViewStyle(m_nViewStyle);
				}
			}
			}
			if(m_MouseHandyPopupPane != NULL) {
				wxWindow *objRect =FindWindowById(evt.GetId(), this);
				wxPoint pos = ClientToScreen(objRect->GetPosition());
				wxSize sz = objRect->GetSize();
				pos.y+=sz.y;
				m_MouseHandyPopupPane->SetPosition(pos);
				m_MouseHandyPopupPane->Show(true);
				SetWindowPos((HWND)m_MouseHandyPopupPane->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
				m_MouseHandyPopupPane->ShowView(strText, TRUE);
				objRect->SetFocus();
			}
		}
		catch(...) {}
	}
	evt.Skip();
}

void CPatsOrderInputPanel::OnNextPriceClicked(wxCommandEvent& evt)
{
	return;
	if(NeedMouseHandyPane_Price() && m_MouseHandyPopupPaneDigit!=NULL) {
		try {
			wxWindow *objRect =FindWindowById(evt.GetId(), this);
			wxRect rect = objRect->GetRect();
			wxPoint pos = ClientToScreen(wxPoint(rect.x,rect.y));
			wxSize sz = objRect->GetSize();
			pos.y+=sz.y;
			m_MouseHandyPopupPaneDigit->SetPosition(pos);
			m_MouseHandyPopupPaneDigit->Show(true);
			m_MouseHandyPopupPaneDigit->SetOwnerUIID(evt.GetId());
			SetWindowPos((HWND)m_MouseHandyPopupPaneDigit->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
			objRect->SetFocus();
		}
		catch(...) {}
	}
	evt.Skip();
}

void CPatsOrderInputPanel::OnNextVolumeClicked(wxCommandEvent& evt)
{
	return;
	if(NeedMouseHandyPane_Volume() && m_MouseHandyPopupPaneVolume!=NULL) {
		try {
			wxWindow *objRect =FindWindowById(evt.GetId(), this);
			wxRect rect = objRect->GetRect();
			wxPoint pos = ClientToScreen(wxPoint(rect.x,rect.y));
			wxSize sz = objRect->GetSize();
			m_MouseHandyPopupPaneVolume->Position(pos, wxSize(0,sz.y) );
			m_MouseHandyPopupPaneVolume->Popup();
		}
		catch(...) {}
	}
	evt.Skip();
}

void CPatsOrderInputPanel::OnMouseInputCode(wxCommandEvent& evt)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
	if(m_MouseHandyPopupPane)
	{
		m_MouseHandyPopupPane->Show(false);
	}
    wxString strText(EvtParamStr.c_str());
//	wxString strText = evt.GetString();
	m_bNoKeyChangeText = FALSE;
	SetInstrumentId(strText, TRUE);
	m_bNoKeyChangeText = TRUE;
	FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT)->SetFocus();
	evt.Skip();
}

void CPatsOrderInputPanel::OnMouseInputDigit(wxCommandEvent& evt)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
    wxString strText(EvtParamStr.c_str());

	int nMode = 0;
	char cKeyCode = 0;
//	wxString strText = evt.GetString();
	//sscanf(strText.c_str(), "%d,%c", &nMode, &cKeyCode);
	vector<string> outData;
	if(SplitString(strText.c_str(), outData)==0) 
		return;
	nMode = atoi(outData[0].c_str());
	if(outData.size()>=2) {
		if(outData[1].size()>0) 
			cKeyCode = outData[1][0];
	}

	switch(nMode) {
	case 0:
	{
		//FindWindowById(ID_OIPATS_PRICE_TEXT, this)->SetFocus();
		wxWindow* pWin = FindWindowById(evt.GetId(), this);
		if(pWin!=NULL)
			pWin->SetFocus();
		INPUT tInput={0};
		tInput.type = INPUT_KEYBOARD;
		tInput.ki.wVk = cKeyCode;//VK_RETURN;
		tInput.ki.wScan = 0;
		tInput.ki.dwFlags = 0;//KEYEVENTF_KEYUP;
		//tInput.ki.time = ;
		//tInput.ki.dwExtraInfo = 0;
		// 发送虚拟键盘鼠标按键事件
		SendInput(1, &tInput, sizeof(INPUT));
	}
		break;
	case 1:
	{
		wxString strText(wxT(""));
		//SetPriceText(strText);
		wxWindow* pWin = FindWindowById(evt.GetId(), this);
		if(pWin!=NULL)
			pWin->SetLabel(strText);
	}
		break;
	case 2:
	{
		wxString strText(LOADSTRING(OLS_MARKET));
		//SetPriceText(strText);
		wxWindow* pWin = FindWindowById(evt.GetId(), this);
		if(pWin!=NULL)
			pWin->SetLabel(strText);
	}
		break;
	};

}

void CPatsOrderInputPanel::OnMouseInputVolume(wxCommandEvent& evt)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
    wxString strText(EvtParamStr.c_str());
//	wxString strText = evt.GetString();
	SetQty(atoi(strText.c_str()));
	evt.Skip();
}

void CPatsOrderInputPanel::OnCodeTextKeyAscii(wxKeyEvent& evt)
{
////	if(NeedMouseHandyPane_Volume()) {
//		wxString strText = GetPanelItemText(ID_CONTRANCTNAME);
//		if(evt.GetKeyCode() != '\b') {
//			strText += (char)(evt.GetKeyCode());
//		}
//		try {
//			wxWindow *objRect =FindWindowById(evt.GetId());
//			wxPoint pos = ClientToScreen(wxPoint(60, 3));
//			wxSize sz = objRect->GetSize();
//			m_MouseHandyPopupPane->Position(pos, wxSize(0,sz.y) );
//			m_MouseHandyPopupPane->ShowKeyInput(strText);
//			if(strText.length()>0) {
//				m_MouseHandyPopupPane->Show(true);
//			}
//			else {
//				wxWindow* window=FindWindowById(ID_CONTRANCTNAME,this);
//				m_MouseHandyPopupPane->Popup(window);
//			}
//		}
//		catch(...) {}
////	}
	evt.Skip();
}

void CPatsOrderInputPanel::OnCodePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void CPatsOrderInputPanel::OnVolumePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void CPatsOrderInputPanel::OnPricePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void CPatsOrderInputPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
	wxRect rectTemp = this->GetRect();
	wxRect rect(0, 0, rectTemp.width, rectTemp.height);
	//wxRect rect(0,0,ORDERPANEL_WIDTH,26*5+10);
	dc.SetPen(wxPen(wxColour(172,168,153)));
	dc.SetBrush(wxBrush(m_colorBak));
	dc.DrawRectangle(rect);
	dc.SetPen(wxPen(wxColour(255,255,255)));
	dc.DrawLine(1,1,1,26*5+10-1);
	//dc.GradientFillLinear(rect,m_colorBak,m_colorBak);
}

void CPatsOrderInputPanel::OnCfgChanged(wxCommandEvent& evt)
{
	SetSellBuyColor();

	//ToggleStaticText* itemText = (ToggleStaticText*)
	//		FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC);
	//// 这里从xml中读是否要确认的配置
	//CfgMgr* pMgr =  CFG_MGR_DEFAULT();  
	//if(pMgr != NULL) {
	//	LPORDER_CFG p = pMgr->GetOrderCfg();
	//	// 设置界面上的自动开平按钮
	//	// 待处理，默认自动开平
	//	itemText->SetNormal(!p->bAutoOpenClose);
	//}
	evt.Skip();
}

void CPatsOrderInputPanel::ResetAccountList(vector<string>& accountList)
{
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_ACCOUNT_COMBOCTRL, this);
	if(pComboBox != NULL) {
		pComboBox->Clear();
		for(int i=0; i<(int)accountList.size(); i++) {
			pComboBox->Insert(accountList[i], i);
		}
		pComboBox->Select(0);
	}
}

void CPatsOrderInputPanel::ResetContractList()
{
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_INSTRUMENTID_COMBOCTRL, this);
	if(pComboBox != NULL) {
		pComboBox->Clear();
		std::set<std::string> outData;
		// 获取合约列表
		if(DEFAULT_SVR()!=NULL)
			DEFAULT_SVR()->GetInstrumentList(outData);
		std::set<std::string>::iterator it=outData.begin();
		for(int i=0; it!=outData.end(); ++it,i++) {
			pComboBox->Insert(*it, i);
		}
	}
	if(m_MouseHandyPopupPane != NULL)
		m_MouseHandyPopupPane->ResetContractList();
}

void CPatsOrderInputPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_OIPATS_INSTRUMENTID_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_CONTRACT_TEXT));
	pWin = FindWindowById(ID_OIPATS_ORDERTYPE_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_ORDERTYPE_TEXT));
	pWin = FindWindowById(ID_OIPATS_BUYSELL_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_DIRECT_TEXT));
	pWin = FindWindowById(ID_OIPATS_VOLUME_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_VOLUME_TEXT));
	pWin = FindWindowById(ID_OIPATS_ACCOUNT_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_ACCOUNT_TEXT));
	pWin = FindWindowById(ID_OIPATS_HEDGE_CHECKBOX, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_HEDGE));

	pWin = FindWindowById(ID_OIPATS_AUTOTRACKPRICE_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_PRICE_TEXT));
	pWin = FindWindowById(ID_OIPATS_AUTOTRACKLIMITPRICE_STATIC, this);
	if(pWin) pWin->SetLabel(LOADSTRING(STATIC_LIMITPRICE_TEXT));
	//ToggleStaticText* pTStatic = NULL;
	//
	//pTStatic = (ToggleStaticText*)FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC, this);
	//pTStatic->SetLabel2(LOADSTRING(OLS_OPENCLOSE), 
	//		LOADSTRING(OLS_AUTO));
	//pTStatic->SetNormal(pTStatic->IsNormal());
	//pTStatic = (ToggleStaticText*)FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
	//pTStatic->SetLabel2(LOADSTRING(OLS_PRICE3), 
	//		LOADSTRING(OLS_T_PRICE));
	//pTStatic->SetNormal(pTStatic->IsNormal());

	wxOwnerDrawnComboBox* itemComboBox = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX, this);
	itemComboBox->SetString(0, LOADSTRING(OLS_BUY));
	itemComboBox->SetString(1, LOADSTRING(OLS_SELL));
	itemComboBox->SetSelection(itemComboBox->GetSelection());
	itemComboBox = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX, this);
	itemComboBox->SetString(0, LOADSTRING(OLS_OPEN));
	//itemComboBox->SetString(1, LOADSTRING(OLS_CLOSE_TODAY));
	itemComboBox->SetString(1, LOADSTRING(OLS_CLOSE));
	itemComboBox->SetSelection(itemComboBox->GetSelection());
	itemComboBox = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OIPATS_ORDERTYPE_COMBOBOX, this);
	itemComboBox->SetString(0, LOADSTRING(ORDERTYPE_LIMIT));
	itemComboBox->SetString(1, LOADSTRING(ORDERTYPE_MARKET));
	itemComboBox->SetString(2, LOADSTRING(ORDERTYPE_STOP));
	itemComboBox->SetString(3, LOADSTRING(ORDERTYPE_STOPLIMIT));
	itemComboBox->SetSelection(itemComboBox->GetSelection());
}

// 仅此用于日志文件输出
wxString CPatsOrderInputPanel::GetUserInputText()
{
	wxString strRet;
	
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	int nRowCount = pCfgMgr->GetListColCount(GID_ORDER_INSERT_PANEL);

	wxWindow* pWin = NULL;

	strRet = "Pats标准下单数据：\n";

	for(int i=0; i<nRowCount; i++) {

		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_ORDER_INSERT_PANEL, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<=ORDERNEWINPUT_MINNUM || pColCfg->id>=ORDERNEWINPUT_MAXNUM)
			continue;

		//合约ID
		if(pColCfg->id==ORDERNEWINPUT_INSTRUMENTID_ROW_ID) {
			pWin = FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="合约ID=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		//下单类型
		if(pColCfg->id==ORDERNEWINPUT_ORDERTYPE_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_ORDERTYPE_COMBOBOX, this);
			if(pWin!=NULL) { 
				strRet+="下单类型=[";
				strRet+=pCmb->GetString(pCmb->GetSelection());
				strRet+="],\t ";
			}
		}

		{
		//买卖方向
		if(pColCfg->id==ORDERNEWINPUT_BUYSELL_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX, this);
			if(pCmb!=NULL) { 
				strRet+="买卖方向=[";
				strRet+=pCmb->GetString(pCmb->GetSelection());
				strRet+="],\t ";
			}
		}
		}

		//开平
		if(pColCfg->id==ORDERNEWINPUT_OPENCLOSE_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX, this);
			if(pWin!=NULL) { 
				strRet+="开平=[";
				strRet+=pCmb->GetString(pCmb->GetSelection());
				strRet+="],\t ";
			}
		}

		//价格
		if(pColCfg->id==ORDERNEWINPUT_PRICE_ROW_ID) {
			pWin = FindWindowById(ID_OIPATS_PRICE_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="价格=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		//限价
		if(pColCfg->id==ORDERNEWINPUT_LIMITPRICE_ROW_ID) {
			pWin = FindWindowById(ID_OIPATS_LIMITPRICE_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="限价=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		//手数
		if(pColCfg->id==ORDERNEWINPUT_VOLUME_ROW_ID) {
			pWin = FindWindowById(ID_OIPATS_VOLUME_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="手数=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		//交易账号
		if(pColCfg->id==ORDERNEWINPUT_TRADEACCOUNT_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIPATS_ACCOUNT_COMBOCTRL, this);
			if(pWin!=NULL) { 
				strRet+="交易账号=[";
				strRet+=pCmb->GetTextCtrl()->GetLabel();
				strRet+="],\t ";
			}
		}
    }
	strRet+="\n";
	return strRet;

}

void CPatsOrderInputPanel::OnShow(wxShowEvent& evt)
{
	if(m_MouseHandyPopupPane && ::IsWindow((HWND)m_MouseHandyPopupPane->GetHWND()))
		m_MouseHandyPopupPane->Show(false);
	if(m_MouseHandyPopupPaneVolume && ::IsWindow((HWND)m_MouseHandyPopupPaneVolume->GetHWND()))
		m_MouseHandyPopupPaneVolume->Show(false);
	if(m_MouseHandyPopupPaneDigit && ::IsWindow((HWND)m_MouseHandyPopupPaneDigit->GetHWND()))
		m_MouseHandyPopupPaneDigit->Show(false);
}
