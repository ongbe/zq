#include "StdAfx.h"
#include "COrderInputPanel.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Orders/COrderInsertEntity.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "wx\odcombo.h"
#include "../inc/Module-Misc2/EventParam.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

const int nFW = 8, nFH = 20;
const int nVW = nFW+2, nVH = nFH+2;
int nTitleWidth = nFW*7;
int nInputWidth = nFW*14;


BEGIN_EVENT_TABLE(COrderInputPanel, CBaseInputPanel)
	EVT_PAINT(OnPaint)
	EVT_SIZE(OnSize)
	EVT_SHOW(OnShow)

	EVT_TEXT(ID_OISTD_INSTRUMENTID_TEXT, OnInstrumentIdTextChanged)
	EVT_TEXT(ID_OISTD_PRICE_TEXT, OnPriceTextChanged)
	EVT_TEXT_MAXLEN(ID_OISTD_INSTRUMENTID_TEXT, OnTextMaxLen) 

	EVT_STATIC_LEFT_UP(OnStaticLeftUp)
	EVT_SPIN_UP(ID_OISTD_PRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_OISTD_PRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_OISTD_VOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_OISTD_VOLUME_SPINCTRL, OnVolumeSpinDown)

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


COrderInputPanel::COrderInputPanel(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_MouseHandyPopupPane(NULL),
	m_MouseHandyPopupPaneVolume(NULL),
	m_MouseHandyPopupPaneDigit(NULL),
	m_nHedgeStatus(0),
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
	m_MouseHandyPopupPaneVolume->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel::OnVolumePanelKeyAscii),NULL,this);
	m_MouseHandyPopupPaneDigit = new CMouseHandyInputPaneDigit(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400), wxNO_BORDER|wxSTATIC_BORDER);
	m_MouseHandyPopupPaneDigit->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel::OnPricePanelKeyAscii),NULL,this);

	
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;

	ctlPos.y-=nVH;

	{		// 合约
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OISTD_STATIC1, LOADSTRING(STATIC_CONTACT_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrCodeList;
	std::set<std::string> outData;
	// 获取合约列表
	outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		strArrCodeList.Add(*it);
	}
	
	//ctlPos=wxPoint(5+nFW*6, 5);
	ctlPos.x += nFW*6;
	
	//ctlSz.Set(116, 20);
	wxTextCtrl* pText = new wxTextCtrl(this, ID_OISTD_INSTRUMENTID_TEXT, wxEmptyString, ctlPos, wxSize(nInputWidth, nFH), 0);
	pText->SetMaxLength(30);
	pText->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel::OnCodeClicked), NULL, this);
	
	//ctlSz.Set(16, 20);
	//WXLOG_INFO("wxOwnerDrawnComboBox--%d",GetTickCount());
	pWin = new wxOwnerDrawnComboBox(this, ID_OISTD_INSTRUMENTID_COMBOCTRL, wxT(""), 
			wxPoint(-1000, ctlPos.y), wxSize(nTitleWidth, nFH), strArrCodeList, wxCB_DROPDOWN);//|wxCB_SORT);
	//WXLOG_INFO("wxOwnerDrawnComboBox--%d",GetTickCount());
	}

	{			//买卖方向
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OISTD_STATIC2, LOADSTRING(STATIC_DIRECT_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrBS;
	strArrBS.Add(LOADSTRING(OLS_BUY));
	strArrBS.Add(LOADSTRING(OLS_SELL));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	//ctlSz.Set(nFW*8, 20);

	wxOwnerDrawnComboBox* pCmbBuySell = new wxOwnerDrawnComboBox(this, ID_OISTD_BUYSELL_COMBOBOX, LOADSTRING(OLS_BUY), 
			ctlPos, wxSize(nInputWidth/2, nFH), strArrBS, wxCB_READONLY);
	pCmbBuySell->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(COrderInputPanel::OnCmbBuySell), NULL, this);

	//ctlSz.Set(52, 20);
	//pWin = new wxCheckBox( this, ID_OISTD_HEDGE_CHECKBOX, LOADSTRING(OLS_HEDGE), 
	//		wxPoint(ctlPos.x+nInputWidth/2+6, ctlPos.y+4), wxSize(nInputWidth/2-6, nFH));
	wxStaticText* pStaticText = new wxStaticText( this, ID_OISTD_HEDGE_CHECKBOX, LOADSTRING(OLS_SPECULATION), 
			wxPoint(ctlPos.x+nInputWidth/2+6+18, ctlPos.y+4), wxSize(nInputWidth/2-6, nFH), wxBORDER_STATIC);
	pStaticText->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel::OnHedgeClicked), NULL, this);
	if(DEFAULT_SVR()&&!DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_HEDGE)) { 

		//ctlSz.Set(116, 20);
		pWin = FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
		if(pWin!=NULL) { 
			pWin->SetSize(wxSize(nInputWidth, nFH));
			pWin->Show(true);
		}

		pWin = FindWindowById(ID_OISTD_HEDGE_CHECKBOX, this);
		if(pWin!=NULL) { 
			//pWin->SetPosition(wxPoint(ctlPos.x-1000, ctlPos.y+4));
			pWin->Show(false);
		}
	}

	}

	{			//开平
	ctlPos.y += nVH;
	ctlPos.x = 5;

	ToggleStaticText* itemOpenClose = new ToggleStaticText( this, ID_OISTD_AUTOOPENCLOSE_STATIC, 
			LOADSTRING(STATIC_OFFSETFLAG_TEXT), LOADSTRING(STATIC_OFFSETFLAG_AUTO));
	itemOpenClose->SetPosition(ctlPos);
	itemOpenClose->SetSize(wxSize(nTitleWidth, nFH));

	wxArrayString strArrOC;
	strArrOC.Add(LOADSTRING(OLS_OPEN));
	strArrOC.Add(LOADSTRING(OLS_CLOSE_TODAY));
	strArrOC.Add(LOADSTRING(OLS_CLOSE));
	
	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	//ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pCmbOpenClose = new wxOwnerDrawnComboBox(this, ID_OISTD_OPENCLOSE_COMBOBOX,LOADSTRING(OLS_OPEN), 
			ctlPos, wxSize(nInputWidth, nFH), strArrOC, wxCB_READONLY);
	pCmbOpenClose->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(COrderInputPanel::OnCmbOpenClose), NULL, this);
	}

	{		//价格
	ctlPos.y += nVH;
	ctlPos.x = 5;

	ToggleStaticText* itemAutoTrackPrice = new ToggleStaticText( this, ID_OISTD_AUTOTRACKPRICE_STATIC, 
			LOADSTRING(STATIC_AUTOLIMITPRICE_TEXT), LOADSTRING(STATIC_AUTOMARKETPRICE_TEXT));
	//ctlPos.y += nVH;
	itemAutoTrackPrice->SetPosition(ctlPos);
	itemAutoTrackPrice->SetSize(wxSize(nTitleWidth, nFH));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	CreateFloatSpinCtrl_Price(ctlPos, wxSize(nInputWidth, nFH));
	}

	{										//手数
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_OISTD_STATIC3, LOADSTRING(STATIC_VOLUME_TEXT), ctlPos, wxSize(nTitleWidth, nFH));

	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	CreateFloatSpinCtrl_Volume(ctlPos, wxSize(nInputWidth, nFH));
	}


	// 这里不需要处理，
	// 语言问题会在ResetLanguage()里完成
	{		              			//交易账号
	ctlPos.y += nVH;
	ctlPos.x = 5;
	pWin = new wxStaticText( this, ID_OISTD_STATIC4, wxEmptyString, ctlPos, wxSize(nTitleWidth, nFH));

	wxArrayString strArrAccount;
	string strInvestorID, strBrokerID;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID, strBrokerID);
		strArrAccount.Add(wxT(strInvestorID));
	}
	else
	{
		set<string> setAccount;
		setAccount.clear();
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID, strBrokerID, &setAccount);
        if(!setAccount.empty()) {
			for(set<string>::iterator it = setAccount.begin(); it != setAccount.end(); ++it)
				strArrAccount.Add(wxT(*it));
		}
	}
	//ctlPos.y += nVH;
	ctlPos.x += nFW*6;
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pComboBox = new wxOwnerDrawnComboBox(this, ID_OISTD_ACCOUNT_COMBOCTRL, 
		wxT(""), ctlPos, wxSize(nInputWidth, nFH), strArrAccount, wxCB_DROPDOWN);//|wxCB_SORT);
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

COrderInputPanel::~COrderInputPanel()
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

COrderInputPanel* COrderInputPanel::CreatePane(wxWindow* parent)
{
    return new COrderInputPanel(parent, wxID_ANY);
}

wxTextCtrl* COrderInputPanel::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit, wxSize& ctrSize)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(ctrSize.x, ctrSize.y);
	ctlSz.x-=16;
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_OISTD_PRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel::OnPriceClicked), NULL, this);
	itemTextCtrl3->SetMaxLength(20);

	ctlPos.x += (ctrSize.x-16);
	ctlSz.Set(16, ctrSize.y);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_OISTD_PRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(-1000000, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* COrderInputPanel::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit, wxSize& ctrSize)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(ctrSize.x, ctrSize.y);
	ctlSz.x-=16;
	wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_OISTD_VOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel::OnVolumeClicked), NULL, this);
	itemTextCtrl3->SetMaxLength(4);

	ctlPos.x += (ctrSize.x-16);
	ctlSz.Set(16, ctrSize.y);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_OISTD_VOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 9999);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void COrderInputPanel::InsertCtrlID2TabCtrl(vector<UINT>& tabCtrlList)
{
	for(int i=0; i<(int)m_arrTabCtrl.size(); i++) {
		tabCtrlList.push_back((UINT)m_arrTabCtrl[i]);
	}
}

BOOL COrderInputPanel::CheckCtrlID(int nCtrlID)
{
	for(int i=0; i<(int)m_arrTabCtrl.size(); i++) {
		if(nCtrlID == m_arrTabCtrl[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

void COrderInputPanel::DynamicShowCtrl()
{
	int nFW = 8, nFH = 20;
	int nVH = nFH+2;
	int nPanelHeight = this->GetClientSize().GetHeight();//(nFH+6)*5+10;
    int StartPox=5;     //开始的x坐标

	wxPoint ctlPos(5, 5);

	wxWindow* pWin = NULL;

	for(int i=ID_OISTD_FIRST+1; i<SUMNUM_OISTD_ID; i++) {
		pWin = FindWindowById(i, this);
		if(pWin != NULL)
			pWin->Show(false);
	}

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	int nRowCount = pCfgMgr->GetListColCount(GID_ORDER_INSERT_PANEL);
	//nRowCount = 6;
    int Intval;

	if(nRowCount>1) 
    {
        Intval = (nPanelHeight-nFH*nRowCount)/(float)(nRowCount+1);
        if(Intval<1) Intval=1;
        nVH = nFH + Intval;
//		float fIntval = (float)(nPanelHeight-nFH*nRowCount-10)/(float)(nRowCount-1);
//		fIntval < 0.0f ? fIntval=0.0f : 0;
//		nVH = nFH+fIntval+0.3f;
	}

	int i=0;

//	ctlPos.y-=nVH;
	m_arrTabCtrl.clear();
	for(i=0; i<nRowCount; i++) {

		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_ORDER_INSERT_PANEL, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<=ORDERNEWINPUT_MINNUM || pColCfg->id>=ORDERNEWINPUT_MAXNUM)
			continue;

        ctlPos.y = Intval + i * nVH;
		ctlPos.x = StartPox;

		//合约ID
		if(pColCfg->id==ORDERNEWINPUT_INSTRUMENTID_ROW_ID) {
		//{	
			m_arrTabCtrl.push_back(ID_OISTD_INSTRUMENTID_TEXT);

//			ctlPos.y += nVH;
//			ctlPos.x = 5;
			pWin = FindWindowById(ID_OISTD_STATIC1, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nTitleWidth, nFH));
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nInputWidth, nFH));
				pWin->Show(true);
			}
		}

		//买卖方向
		if(pColCfg->id==ORDERNEWINPUT_BUYSELL_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OISTD_BUYSELL_COMBOBOX);
			/////////////m_setTabCtrl.insert(ID_OISTD_HEDGE_CHECKBOX);

//			ctlPos.y += nVH;
//			ctlPos.x = 5;

			pWin = FindWindowById(ID_OISTD_STATIC2, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nTitleWidth, nFH));
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nInputWidth/2-3, nFH));
				pWin->Show(true);
			}

			pWin = FindWindowById(ID_OISTD_HEDGE_CHECKBOX, this);
			if(pWin!=NULL && DEFAULT_SVR()&&DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_HEDGE)) { 
				pWin->SetPosition(wxPoint(ctlPos.x+nInputWidth/2+6+18, ctlPos.y+1));
				pWin->Show(true);
			}
		}

		//开平
		if(pColCfg->id==ORDERNEWINPUT_OPENCLOSE_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OISTD_OPENCLOSE_COMBOBOX);

//			ctlPos.y += nVH;
//			ctlPos.x = 5;

			pWin = FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				//pWin->SetSize(wxSize(nTitleWidth, nFH));
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nInputWidth, nFH));
				pWin->Show(true);
			}
		}

		//价格
		if(pColCfg->id==ORDERNEWINPUT_PRICE_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OISTD_PRICE_TEXT);

//			ctlPos.y += nVH;
//			ctlPos.x = 5;
			pWin = FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				//pWin->SetSize(wxSize(nTitleWidth, nFH));
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OISTD_PRICE_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nInputWidth-16, nFH));
				pWin->Show(true);
			}

			ctlPos.x += (nInputWidth-16);
			pWin = FindWindowById(ID_OISTD_PRICE_SPINCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(16, nFH));
				pWin->Show(true);
			}
		}

		//手数
		if(pColCfg->id==ORDERNEWINPUT_VOLUME_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OISTD_VOLUME_TEXT);

//			ctlPos.y += nVH;
//			ctlPos.x = 5;
			pWin = FindWindowById(ID_OISTD_STATIC3, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nTitleWidth, nFH));
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OISTD_VOLUME_TEXT, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nInputWidth-16, nFH));
				pWin->Show(true);
			}

			ctlPos.x += (nInputWidth-16);
			pWin = FindWindowById(ID_OISTD_VOLUME_SPINCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(16, nFH));
				pWin->Show(true);
			}
		}

		//交易账号
		if(pColCfg->id==ORDERNEWINPUT_TRADEACCOUNT_ROW_ID) {
		//{
			m_arrTabCtrl.push_back(ID_OISTD_ACCOUNT_COMBOCTRL);

//			ctlPos.y += nVH;
//			ctlPos.x = 5;
			pWin = FindWindowById(ID_OISTD_STATIC4, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nTitleWidth, nFH));
				pWin->Show(true);
			}

			ctlPos.x += nTitleWidth;
			pWin = FindWindowById(ID_OISTD_ACCOUNT_COMBOCTRL, this);
			if(pWin!=NULL) { 
				pWin->SetPosition(ctlPos);
				pWin->SetSize(wxSize(nInputWidth, nFH));
				pWin->Show(true);
			}
		}
    }

}

void COrderInputPanel::SetViewMode(int nViewMode)
{
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
		//SetOpenClose(0);
		{
		wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX,this);
		pComboBox->SetSelection(0);
		}
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
}

BOOL COrderInputPanel::NeedMouseHandyPane_Code()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputInstrumentId;
}

BOOL COrderInputPanel::NeedMouseHandyPane_Volume()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputVolume;
}

BOOL COrderInputPanel::NeedMouseHandyPane_Price()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputPrice;
}

// 可以输入缩写和拼音
BOOL COrderInputPanel::NeedInputAbbreviation()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bInputAbbreviation;
}

int COrderInputPanel::GetContractListStyle()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->nContractListStyle;
}

void COrderInputPanel::SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent)
{
	m_bNoKeyChangeText = FALSE;
	wxTextCtrl* window=(wxTextCtrl*)FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
	if(window!=NULL) {
		window->SetLabel(strInstrumentId);
		window->SetSelection(strInstrumentId.length(),strInstrumentId.length());
		
	}
	m_bNoKeyChangeText = TRUE;
}

wxString COrderInputPanel::GetInstrumentId()
{
	wxTextCtrl* window=(wxTextCtrl*)FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
	return window->GetValue();
}

void COrderInputPanel::SetAccount(wxString& strAccount)
{
    wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)
			FindWindowById(ID_OISTD_ACCOUNT_COMBOCTRL,this);
	window->SetValue(strAccount);
}

wxString COrderInputPanel::GetAccount()
{
	wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_ACCOUNT_COMBOCTRL, this);
	return window->GetValue();
}

void COrderInputPanel::SetDirection(BOOL bIsBuy)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_BUYSELL_COMBOBOX,this);
	pComboBox->SetSelection(bIsBuy?0:1);
	SetSellBuyColor();
}

// 获得买卖方向，TRUE为买入
BOOL COrderInputPanel::GetDirection()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_BUYSELL_COMBOBOX,this);
	return pComboBox->GetSelection()==0;
}

// 设置开平仓模式，0 开仓，1 平今，2 平仓
void COrderInputPanel::SetOpenClose(int nOCMode)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX,this);
	if(pComboBox->IsEnabled())
		pComboBox->SetSelection(nOCMode);
}

// 获得开平仓模式，0 开仓，1 平今，2 平仓
int COrderInputPanel::GetOpenClose()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX,this);
	return pComboBox->GetSelection();
}

void COrderInputPanel::SetPriceText(wxString& strPrice)
{
	m_bIsLockForSetChange = TRUE;
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
	window->SetLabel(strPrice);
	m_bIsLockForSetChange = FALSE;
}

wxString COrderInputPanel::GetPriceText()
{
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
	return window->GetLabel();
}

void COrderInputPanel::SetPrice(double fPrice)
{
	m_bIsLockForSetChange = TRUE;
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
	m_bIsLockForSetChange = FALSE;
}

double COrderInputPanel::GetPrice()
{
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void COrderInputPanel::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_OISTD_VOLUME_TEXT,this);
	window->SetLabel(strText);
}

int COrderInputPanel::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_OISTD_VOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

// 设置是否保值
void COrderInputPanel::SetHedge(int nHedgeStatus)
{
	//wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OISTD_HEDGE_CHECKBOX,this);
	//pCheckBox->SetValue(bIsHedge==TRUE);
	m_nHedgeStatus = nHedgeStatus;
	wxStaticText* pText=(wxStaticText*)FindWindowById(ID_OISTD_HEDGE_CHECKBOX,this);
	if(pText != NULL) {
		switch(m_nHedgeStatus) {
		case 0:
			pText->SetLabel(LOADSTRING(OLS_SPECULATION));
			break;
		case 1:
			pText->SetLabel(LOADSTRING(OLS_ARBITRAGE));
			break;
		case 2:
			pText->SetLabel(LOADSTRING(OLS_HEDGE));
			break;
		};
	}
}

int COrderInputPanel::GetHedge()
{
   //wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OISTD_HEDGE_CHECKBOX,this);
   //return pCheckBox->GetValue();
   return m_nHedgeStatus;
}

// 设置自动开平模式
void COrderInputPanel::SetAutoOpenClose(BOOL bIsAuto)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC,this);
   pSText->SetNormal(!bIsAuto);
}

BOOL COrderInputPanel::GetAutoOpenClose()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC,this);
   return !pSText->IsNormal();
}

// 设置价格跟盘模式
void COrderInputPanel::SetAutoTrackPrice(BOOL bIsTrack)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC,this);
   pSText->SetNormal(!bIsTrack);
}

BOOL COrderInputPanel::GetAutoTrackPrice()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC,this);
   return !pSText->IsNormal();
}

void COrderInputPanel::InitOrderView()
{
    wxString strText;
	SetInstrumentId(strText);
    SetDirection(TRUE);
	SetOpenClose(0);		// 设置开平仓模式，0 开仓，1 平今，2 平仓
    SetPriceText(strText);
    SetQty(0);				// 设置手数
	SetHedge(FALSE);		// 设置是否保值
	SetAccount(strText);
}

void COrderInputPanel::ClearViewData(int nMode)
{
	wxString strText;
	//1 下单后不清空下单板, 2 全部清空, 3 清空价格和手数, 4 只清空手数
	switch(nMode) {
	case 2:
		SetInstrumentId(strText);
		SetDirection(TRUE);
		SetOpenClose(0);
		FindWindowById(ID_OISTD_PRICE_TEXT, this)->SetLabel(wxT(""));
		FindWindowById(ID_OISTD_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 3:
		FindWindowById(ID_OISTD_PRICE_TEXT, this)->SetLabel(wxT(""));
		FindWindowById(ID_OISTD_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 4:
		FindWindowById(ID_OISTD_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	};
}

// 从数据结构中读数据并显示
void COrderInputPanel::ShowOrder(PlatformStru_InputOrder& req)
{
	wxString strText;

	LockForSetChange();

	strText = req.strAccount;
	SetAccount(strText);

	strText=req.InstrumentID;
	SetInstrumentId(strText);
	SetDirection(req.Direction==THOST_FTDC_D_Buy? 1 : 0);
	SetOpenClose(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? 0 :
		(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? 1 : 2));		// 设置开平仓模式，0 开仓，1 平今，2 平仓
	if(req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		strText=LOADSTRING(OLS_MARKET);
		SetPriceText(strText);
	}
	else
		SetPrice(req.LimitPrice);
	SetQty(req.VolumeTotalOriginal);							// 设置手数
	SetHedge(req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge);		// 设置是否保值

	UnlockForSetChange();
}

void COrderInputPanel::SetSellBuyColor()
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
	for(int i=ID_OISTD_STATIC1; i<SUMNUM_OISTD_ID; i++) {
		pWin = FindWindowById(i, this);
		if(pWin) pWin->SetBackgroundColour(m_colorBak);
	}
	//pWin = FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX, this);
	//if(pWin) pWin->SetBackgroundColour(m_colorBak);

	//SetBackgroundColour(clr);
	Refresh();

}

void COrderInputPanel::OnHedgeClicked(wxMouseEvent& evt)
{
	m_nHedgeStatus++;
	if(m_nHedgeStatus>2)
		m_nHedgeStatus=0;
	
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_OISTD_HEDGE_CHECKBOX, this);
	switch(m_nHedgeStatus) {
	case 0:
		pWin->SetLabel(LOADSTRING(OLS_SPECULATION));
		break;
	case 1:
		pWin->SetLabel(LOADSTRING(OLS_ARBITRAGE));
		break;
	case 2:
		pWin->SetLabel(LOADSTRING(OLS_HEDGE));
		break;
	}
}

void COrderInputPanel::OnInstrumentIdTextChanged(wxCommandEvent& event)
{
	
	if(!m_bIsInitOK)
		return;

	if(m_bIsLockForSetChange)
		return;

    wxString strInputTxt;
	wxWindow* pWin = FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
	if(pWin != NULL) strInputTxt=pWin->GetLabel();


	pWin = FindWindowById(ID_OISTD_INSTRUMENTID_COMBOCTRL, this);
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

		for(int i=0; i<(int)pVec->size(); i++) {
			NumberShortKey& tSKey = (*pVec)[i];
			if(strInputTxt.compare(tSKey.ShortKey)==0) {
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
        DWORD EvtParamID;
        std::string EvtParamStr(strInputTxt.c_str());
        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
        {
            evtOut.SetInt((int)EvtParamID);
	        if(GetParent())
		        GetParent()->ProcessEvent(evtOut);
        }
    }
	
	// 处理拼音转换
	if(NeedInputAbbreviation() && m_bNoKeyChangeText) {
		if(m_MouseHandyPopupPane != NULL) {
			if(strInputTxt.Length()>0) {
				if(m_nViewStyle!=STYLE_SunGard || GetContractListStyle()==1) {
					wxWindow *objRect =FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
					wxPoint pos = objRect->GetPosition();
					pos = ClientToScreen(pos);
					wxSize sz = objRect->GetSize();
					wxSize szPane = m_MouseHandyPopupPane->GetSize();
					RECT rectDesk;
					::GetWindowRect(::GetDesktopWindow(), &rectDesk);
					pos.y+=sz.y;
					if(pos.x+szPane.x > rectDesk.right-4)
						pos.x = rectDesk.right - szPane.x - 4;
					m_MouseHandyPopupPane->SetPosition(pos);
					if(!m_MouseHandyPopupPane->IsShown()) {
							m_MouseHandyPopupPane->Show();
					}
					m_MouseHandyPopupPane->ShowView(strInputTxt, FALSE);
					set<string> InstrumentIDArray;
					InstrumentIDArray.clear();
					if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
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
			FindWindowById(ID_OISTD_INSTRUMENTID_TEXT)->SetFocus();
		}
	}

}

void COrderInputPanel::OnPriceTextChanged(wxCommandEvent& event)
{
	
	if(!m_bIsInitOK)
		return;

	if(m_bIsLockForSetChange)
		return;

	if(GetAutoTrackPrice())
		SetAutoTrackPrice(FALSE);
}

void COrderInputPanel::OnTextMaxLen(wxCommandEvent& event)
{
	//int nLen = event.GetInt();
}

void COrderInputPanel::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	int thisID = evt.GetId();
	if(evt.GetKeyCode()==WXK_RETURN || evt.GetKeyCode()==VK_RETURN) {
		if(m_MouseHandyPopupPane != NULL) {
			if(thisID == ID_OISTD_INSTRUMENTID_TEXT && m_MouseHandyPopupPane->IsShown()) {
				m_MouseHandyPopupPane->Show(false);
				evt.Skip();
				return;
			}
		}
		if(thisID == ID_OISTD_PRICE_TEXT && m_MouseHandyPopupPaneDigit->IsShown()) {
			m_MouseHandyPopupPaneDigit->Show(false);
			evt.Skip();
			return;
		}
	}
	else if(evt.GetKeyCode()==WXK_DOWN || evt.GetKeyCode()==VK_DOWN) {
		if(m_MouseHandyPopupPane != NULL) {
			if(thisID == ID_OISTD_INSTRUMENTID_TEXT && m_MouseHandyPopupPane->IsShown()) {
				m_MouseHandyPopupPane->SetFocus();
				return;
			}
		}
	}
	else if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		switch(thisID) {
		case ID_OISTD_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OISTD_INSTRUMENTID_COMBOCTRL, this),wxOwnerDrawnComboBox);
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
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// 需要调用合约代码改变事件
					// 向父窗体发送合约代码改变事件
					wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, ID_OISTD_INSTRUMENTID_TEXT);
                    std::string EvtParamStr(itemCombo->GetValue().c_str());
                    DWORD EvtParamID;
                    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                    {
                        evtOut.SetInt((int)EvtParamID);
	                    if(GetParent())
		                    GetParent()->ProcessEvent(evtOut);
                    }
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OISTD_PRICE_TEXT:
			evtSpin.SetId(ID_OISTD_PRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_OISTD_VOLUME_TEXT:
			evtSpin.SetId(ID_OISTD_VOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		int thisID = evt.GetId();
		switch(thisID) {
		case ID_OISTD_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OISTD_INSTRUMENTID_COMBOCTRL, this),wxOwnerDrawnComboBox);
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
								(CurSel<(int)itemCombo->GetCount()-1) ? (CurSel+1) : ((int)itemCombo->GetCount()-1));
					}
					else {
						itemCombo->SetSelection(CurSel);
					}
					SetInstrumentId(itemCombo->GetValue());
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// 需要调用合约代码改变事件
					// 向父窗体发送合约代码改变事件
					wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, ID_OISTD_INSTRUMENTID_TEXT);
                    std::string EvtParamStr(itemCombo->GetValue().c_str());
                    DWORD EvtParamID;
                    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                    {
                        evtOut.SetInt((int)EvtParamID);
	                    if(GetParent())
		                    GetParent()->ProcessEvent(evtOut);
                    }
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OISTD_PRICE_TEXT:
			evtSpin.SetId(ID_OISTD_PRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_OISTD_VOLUME_TEXT:
			evtSpin.SetId(ID_OISTD_VOLUME_SPINCTRL);
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

			if(thisID == ID_OISTD_BUYSELL_COMBOBOX) {
				if(nChar==p->cBuyHotKey) {
					SetDirection(TRUE);
					return;
				}
				else if(nChar==p->cSelHotKey) {
					SetDirection(FALSE);
					return;
				}
			}
			else if(thisID == ID_OISTD_OPENCLOSE_COMBOBOX) {
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

void COrderInputPanel::OnStaticLeftUp(wxCommandEvent& event)
{
	//ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(event.GetId(), this);
	if(event.GetId()==ID_OISTD_AUTOOPENCLOSE_STATIC) {
		// 向父窗体发送自动开平仓改变事件
		wxCommandEvent evtOut(wxEVT_KQPANEL_AUTOOPENCLOSE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoOpenClose());
		if(GetParent())
			GetParent()->AddPendingEvent(evtOut);
	}
	else if(event.GetId()==ID_OISTD_AUTOTRACKPRICE_STATIC) {
		// 向父窗体发送自动跟踪价格改变事件
		wxCommandEvent evtOut(wxEVT_KQPANEL_AUTOTRACKPRICE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoTrackPrice());
		if(GetParent())
			GetParent()->AddPendingEvent(evtOut);
	}

}

void COrderInputPanel::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
//    SetPrice(fPrice);
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());

}

void COrderInputPanel::OnPriceSpinDown(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
//    SetPrice(fPrice);
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
}

void COrderInputPanel::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void COrderInputPanel::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void COrderInputPanel::OnCmbBuySell(wxCommandEvent& evt)
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
		if(nSumTodayVolume>0) {
			SetOpenClose(1);
			//SetInputQty(nSumTodayVolume);
		}
		else if(nSumVolume>0){
			SetOpenClose(2);
			//SetInputQty(nSumVolume-nSumTodayVolume);
		}
		else {
			SetOpenClose(0);
			//SetInputQty(GetDefaultVolume(strCode));
		}
	}
	
	SetSellBuyColor();

}

void COrderInputPanel::OnCmbOpenClose(wxCommandEvent& evt)
{
	SetAutoOpenClose(FALSE);
}

void COrderInputPanel::OnCodeClicked(wxMouseEvent& evt)//wxMouseEvent
{
	wxCommandEvent evttmp(wxEVT_CODECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderInputPanel::OnPriceClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_PRICECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderInputPanel::OnVolumeClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_VOLUMECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderInputPanel::OnNextCodeClicked(wxCommandEvent& evt)
{
	wxString strText;
	if(NeedMouseHandyPane_Code()) {
		try {
			if(DEFAULT_SVR()&&DEFAULT_SVR()->HaveShowInstumentComboList() && m_MouseHandyPopupPane == NULL) {
				std::map<string, set<string>> outData;
				outData.clear();
				if(DEFAULT_SVR()->GetProductID_InstrumentIDsByExchangeID(outData) > 0) {
					m_MouseHandyPopupPane = new CMouseHandyInputPane(NULL, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400), wxNO_BORDER|wxSTATIC_BORDER);	//
					m_MouseHandyPopupPane->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel::OnCodePanelKeyAscii),NULL,this);
					m_MouseHandyPopupPane->SetViewStyle(m_nViewStyle);
				}
			}
			if(m_MouseHandyPopupPane != NULL) {
				m_MouseHandyPopupPane->SetParent(this);
				wxWindow *objRect =FindWindowById(evt.GetId(), this);
				wxRect rect = objRect->GetRect();
				wxPoint pos = ClientToScreen(wxPoint(rect.x,rect.y));
				//wxPoint pos = ClientToScreen(objRect->GetPosition());
				wxSize sz = objRect->GetSize();
				wxSize szPane = m_MouseHandyPopupPane->GetSize();
				RECT rectDesk;
				//::GetWindowRect(::GetDesktopWindow(), &rectDesk);
				rectDesk.top = 0;
				rectDesk.left = 0;
				rectDesk.right=::GetSystemMetrics(SM_CXVIRTUALSCREEN);
				rectDesk.bottom=::GetSystemMetrics(SM_CYVIRTUALSCREEN);

				pos.y+=sz.y;
				if(pos.y+szPane.y > rectDesk.bottom-32)
					pos.y = rectDesk.bottom - szPane.y - 32;
				if(pos.x+szPane.x > rectDesk.right-4)
					pos.x = rectDesk.right - szPane.x - 4;

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

void COrderInputPanel::OnNextPriceClicked(wxCommandEvent& evt)
{

	if(NeedMouseHandyPane_Price() && m_MouseHandyPopupPaneDigit!=NULL) {
		try {
			wxWindow *objRect =FindWindowById(evt.GetId(), this);
			wxRect rect = objRect->GetRect();
			wxPoint pos = ClientToScreen(wxPoint(rect.x,rect.y));
			wxSize sz = objRect->GetSize();
			pos.y+=sz.y;
			m_MouseHandyPopupPaneDigit->SetPosition(pos);
			m_MouseHandyPopupPaneDigit->Show(true);
			SetWindowPos((HWND)m_MouseHandyPopupPaneDigit->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
			objRect->SetFocus();
		}
		catch(...) {}
	}
	evt.Skip();
}

void COrderInputPanel::OnNextVolumeClicked(wxCommandEvent& evt)
{
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

void COrderInputPanel::OnMouseInputCode(wxCommandEvent& evt)
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
	m_bNoKeyChangeText = FALSE;
	SetInstrumentId(strText, TRUE);
	m_bNoKeyChangeText = TRUE;
	FindWindowById(ID_OISTD_INSTRUMENTID_TEXT)->SetFocus();
	evt.Skip();
}

void COrderInputPanel::OnMouseInputDigit(wxCommandEvent& evt)
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
		FindWindowById(ID_OISTD_PRICE_TEXT, this)->SetFocus();
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
		SetPriceText(strText);
	}
		break;
	case 2:
	{
		wxString strText(LOADSTRING(OLS_MARKET));
		SetPriceText(strText);
	}
		break;
	case 3:
	{
        wxString name=GetInstrumentId();
        wxString strPrice("-");
        PlatformStru_DepthMarketData field;
	    ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

        if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)==0) 
        {
            if(::util::isInvalidValue(field.BidPrice1))
                SetPriceText(strPrice);
            else
                SetPrice(field.BidPrice1);
        }
	}
		break;
	case 4:
	{
        wxString name=GetInstrumentId();
        wxString strPrice("-");
        PlatformStru_DepthMarketData field;
	    ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

        if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)==0) 
        {
            if(::util::isInvalidValue(field.AskPrice1))
                SetPriceText(strPrice);
            else
                SetPrice(field.AskPrice1);
        }
	}
		break;
	case 5:
	{
        wxString name=GetInstrumentId();
        wxString strPrice("-");
        PlatformStru_DepthMarketData field;
	    ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

        if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)==0) 
        {
            if(::util::isInvalidValue(field.LastPrice))
                SetPriceText(strPrice);
            else
                SetPrice(field.LastPrice);
        }
	}
		break;
	};

}

void COrderInputPanel::OnMouseInputVolume(wxCommandEvent& evt)
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
	SetQty(atoi(strText.c_str()));
	evt.Skip();
}

void COrderInputPanel::OnCodeTextKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel::OnCodePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel::OnVolumePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel::OnPricePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
	wxRect rectTemp = this->GetRect();
	wxRect rect(0, 0, rectTemp.width, rectTemp.height);
	dc.SetPen(wxPen(wxColour(172,168,153)));
	dc.SetBrush(wxBrush(m_colorBak));
	dc.DrawRectangle(rect);
	dc.SetPen(wxPen(wxColour(255,255,255)));
	dc.DrawLine(1,1,1,26*5+10-1);
	//dc.GradientFillLinear(rect,m_colorBak,m_colorBak);
}

void COrderInputPanel::OnCfgChanged(wxCommandEvent& evt)
{
	SetSellBuyColor();

	ToggleStaticText* itemText = (ToggleStaticText*)
			FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC);
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr =  CFG_MGR_DEFAULT();  
	if(pMgr != NULL) {
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// 设置界面上的自动开平按钮
		// 待处理，默认自动开平
		itemText->SetNormal(!p->bAutoOpenClose);
	}
	evt.Skip();
}

void COrderInputPanel::ResetAccountList(vector<string>& accountList)
{
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_ACCOUNT_COMBOCTRL, this);
	if(pComboBox != NULL) {
		pComboBox->Clear();
		for(int i=0; i<(int)accountList.size(); i++) {
			pComboBox->Insert(accountList[i], i);
		}
		pComboBox->Select(0);
	}
}

void COrderInputPanel::ResetContractList()
{
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OISTD_INSTRUMENTID_COMBOCTRL, this);
	if(pComboBox != NULL) {
		pComboBox->Clear();
		std::set<std::string> outData;
		// 获取合约列表
		outData.clear();
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
		std::set<std::string>::iterator it=outData.begin();
		for(int i=0; it!=outData.end(); ++it,i++) {
			pComboBox->Insert(*it, i);
		}
	}
	if(m_MouseHandyPopupPane != NULL)
		m_MouseHandyPopupPane->ResetContractList();
}

void COrderInputPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_OISTD_STATIC1, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_CONTRACT));
	pWin = FindWindowById(ID_OISTD_STATIC2, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_BUY_SELL));
	pWin = FindWindowById(ID_OISTD_STATIC3, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_QTY));
	pWin = FindWindowById(ID_OISTD_STATIC4, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_ACCOUNT));
	pWin = FindWindowById(ID_OISTD_HEDGE_CHECKBOX, this);
	//if(pWin) pWin->SetLabel(LOADSTRING(OLS_HEDGE));
	if(pWin) {
		switch(m_nHedgeStatus) {
		case 0:
			pWin->SetLabel(LOADSTRING(OLS_SPECULATION));
			break;
		case 1:
			pWin->SetLabel(LOADSTRING(OLS_ARBITRAGE));
			break;
		case 2:
			pWin->SetLabel(LOADSTRING(OLS_HEDGE));
			break;
		}
	}

	ToggleStaticText* pTStatic = NULL;
	
	pTStatic = (ToggleStaticText*)FindWindowById(ID_OISTD_AUTOOPENCLOSE_STATIC, this);
	pTStatic->SetLabel2(LOADSTRING(OLS_OPENCLOSE), 
			LOADSTRING(OLS_AUTO));
	pTStatic->SetNormal(pTStatic->IsNormal());
	pTStatic = (ToggleStaticText*)FindWindowById(ID_OISTD_AUTOTRACKPRICE_STATIC, this);
	pTStatic->SetLabel2(LOADSTRING(OLS_PRICE3), 
			LOADSTRING(OLS_T_PRICE));
	pTStatic->SetNormal(pTStatic->IsNormal());

	wxOwnerDrawnComboBox* itemComboBox1 = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
	itemComboBox1->SetString(0, LOADSTRING(OLS_BUY));
	itemComboBox1->SetString(1, LOADSTRING(OLS_SELL));
	itemComboBox1->SetSelection(itemComboBox1->GetSelection());
	itemComboBox1 = (wxOwnerDrawnComboBox*)
			FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
	itemComboBox1->SetString(0, LOADSTRING(OLS_OPEN));
	itemComboBox1->SetString(1, LOADSTRING(OLS_CLOSE_TODAY));
	itemComboBox1->SetString(2, LOADSTRING(OLS_CLOSE));
	itemComboBox1->SetSelection(itemComboBox1->GetSelection());
}

// 仅此用于日志文件输出
wxString COrderInputPanel::GetUserInputText()
{
	wxString strRet;
	
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	int nRowCount = pCfgMgr->GetListColCount(GID_ORDER_INSERT_PANEL);

	wxWindow* pWin = NULL;

	strRet = "标准下单数据：\n";

	for(int i=0; i<nRowCount; i++) {

		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_ORDER_INSERT_PANEL, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<=ORDERNEWINPUT_MINNUM || pColCfg->id>=ORDERNEWINPUT_MAXNUM)
			continue;

		//合约ID
		if(pColCfg->id==ORDERNEWINPUT_INSTRUMENTID_ROW_ID) {
			pWin = FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="合约ID=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		{
		//买卖方向
		if(pColCfg->id==ORDERNEWINPUT_BUYSELL_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
			if(pCmb!=NULL) { 
				strRet+="买卖方向=[";
				strRet+=pCmb->GetString(pCmb->GetSelection());
				strRet+="],\t ";
			}
		}
		}

		//开平
		if(pColCfg->id==ORDERNEWINPUT_OPENCLOSE_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
			if(pWin!=NULL) { 
				strRet+="开平=[";
				strRet+=pCmb->GetString(pCmb->GetSelection());
				strRet+="],\t ";
			}
		}

		//价格
		if(pColCfg->id==ORDERNEWINPUT_PRICE_ROW_ID) {
			pWin = FindWindowById(ID_OISTD_PRICE_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="价格=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		//手数
		if(pColCfg->id==ORDERNEWINPUT_VOLUME_ROW_ID) {
			pWin = FindWindowById(ID_OISTD_VOLUME_TEXT, this);
			if(pWin!=NULL) { 
				strRet+="手数=[";
				strRet+=pWin->GetLabel();
				strRet+="],\t ";
			}
		}

		//交易账号
		if(pColCfg->id==ORDERNEWINPUT_TRADEACCOUNT_ROW_ID) {
			wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_OISTD_ACCOUNT_COMBOCTRL, this);
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

void COrderInputPanel::EnablePriceInputCtl(bool bEnabled)
{
	wxWindow* window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
    window->Enable(bEnabled);
}


void COrderInputPanel::OnSize( wxSizeEvent& event )
{

	int nWidth = this->GetClientSize().GetWidth();

	nWidth -= 5;
	nTitleWidth = nWidth/3;
	nInputWidth = nWidth-nTitleWidth-5;

	DynamicShowCtrl();
	event.Skip();
}

void COrderInputPanel::OnShow(wxShowEvent& evt)
{
	if(m_MouseHandyPopupPane && ::IsWindow((HWND)m_MouseHandyPopupPane->GetHWND()))
		m_MouseHandyPopupPane->Show(false);
	if(m_MouseHandyPopupPaneVolume && ::IsWindow((HWND)m_MouseHandyPopupPaneVolume->GetHWND()))
		m_MouseHandyPopupPaneVolume->Show(false);
	if(m_MouseHandyPopupPaneDigit && ::IsWindow((HWND)m_MouseHandyPopupPaneDigit->GetHWND()))
		m_MouseHandyPopupPaneDigit->Show(false);
}

bool COrderInputPanel::Show(bool show)
{
	if(!show) {
		if(m_MouseHandyPopupPane && ::IsWindow((HWND)m_MouseHandyPopupPane->GetHWND()))
			m_MouseHandyPopupPane->Show(false);
		if(m_MouseHandyPopupPaneVolume && ::IsWindow((HWND)m_MouseHandyPopupPaneVolume->GetHWND()))
			m_MouseHandyPopupPaneVolume->Show(false);
		if(m_MouseHandyPopupPaneDigit && ::IsWindow((HWND)m_MouseHandyPopupPaneDigit->GetHWND()))
			m_MouseHandyPopupPaneDigit->Show(false);
	}
	return CBaseInputPanel::Show(show);
}
