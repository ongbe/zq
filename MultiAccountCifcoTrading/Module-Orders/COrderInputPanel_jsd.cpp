#include "StdAfx.h"
#include "COrderInputPanel_jsd.h"
#include "../inc/Module-Misc/zqControl.h"
#include "tools/util.h"
#include "wx\odcombo.h"


using namespace util;

#define ROW_HEIGHT  13
#define ROW_WIDTH   12
#define WDPANEL_WIDTH 146
#define ORDERPANEL_WIDTH 174


BEGIN_EVENT_TABLE(COrderInputPanel_jsd, CBaseInputPanel)
	EVT_PAINT(OnPaint)

	EVT_TEXT(ID_OIJSD_INSTRUMENTID_TEXT, OnInstrumentIdTextChanged)
	//EVT_STATIC_LEFT_UP(OnStaticLeftUp)
	EVT_SPIN_UP(ID_OIJSD_PRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_OIJSD_PRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_OIJSD_VOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_OIJSD_VOLUME_SPINCTRL, OnVolumeSpinDown)

    EVT_STATIC_LEFT_UP(OnStaticLeftUp)

	EVT_CFG_CHANGE(OnCfgChanged)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderInputPanel_jsd::COrderInputPanel_jsd(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_bIsLockForSetChange(FALSE),
	m_bNoKeyChangeText(TRUE),
	m_colorBak(0,0,0),
	m_bIsInitOK(FALSE)
{

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	
	int nFW = 8, nFH = 18;
	int nVW = nFW+2, nVH = nFH+4;
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;


	ctlPos.y+=3;
	pWin = new wxStaticText( this, ID_OIJSD_STATIC1, STATIC_CONTACT_TEXT, ctlPos);
//	ctlPos.y += nVH;
//	pWin = new wxStaticText( this, ID_OIJSD_STATIC2, STATIC_LONGCODE_TEXT, ctlPos);
	ctlPos.y += nVH;
	pWin = new wxStaticText( this, ID_OIJSD_STATIC3, STATIC_DIRECT_TEXT, ctlPos);
	//ctlPos.y += nVH;
	//pWin = new wxStaticText( this, ID_OIJSD_AUTOOPENCLOSE_STATIC, STATIC_OFFSETFLAG_TEXT, ctlPos);
	ctlPos.y += nVH;
	ToggleStaticText* itemOpenClose = new ToggleStaticText( this, ID_OIJSD_AUTOOPENCLOSE_STATIC, 
			STATIC_OFFSETFLAG_TEXT, STATIC_OFFSETFLAG_AUTO);
	itemOpenClose->SetPosition(ctlPos);
	ToggleStaticText* itemAutoTrackPrice = new ToggleStaticText( this, ID_OIJSD_AUTOTRACKPRICE_STATIC, 
			STATIC_LIMITPRICE_TEXT, STATIC_MARKETPRICE_TEXT);
	ctlPos.y += nVH;
	itemAutoTrackPrice->SetPosition(ctlPos);
	ctlPos.y += nVH;
	pWin = new wxStaticText( this, ID_OIJSD_STATIC4, STATIC_VOLUME_TEXT, ctlPos);
	ctlPos.y += nVH;
	pWin = new wxStaticText( this, ID_OIJSD_STATIC5, ("帐号:"), ctlPos);


	wxArrayString strArrCodeList;
	std::set<std::string> outData;
	// 获取合约列表
	outData.clear();
	if(g_pPlatform) g_pPlatform->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		strArrCodeList.Add(*it);
	}


	ctlPos=wxPoint(5+nFW*6, 5);
	

	//ctlSz.Set(116, 22);
	//pWin = new wxTextCtrl(this, ID_OIJSD_INSTRUMENTID_TEXT, wxEmptyString, ctlPos, ctlSz, 0);
	//pWin->Connect(ID_OIJSD_INSTRUMENTID_TEXT, wxEVT_LEFT_UP, 
	//		wxMouseEventHandler(COrderInputPanel_jsd::OnCodeClicked), NULL, this);
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pComboBox = new wxOwnerDrawnComboBox(this, ID_OIJSD_INSTRUMENTID_TEXT, //ID_OIJSD_INSTRUMENTID_COMBOCTRL
			wxT(""), ctlPos, ctlSz, strArrCodeList, wxCB_DROPDOWN);//|wxCB_SORT);
	pComboBox->GetTextCtrl()->SetMaxLength(30);

	//ctlSz.Set(116, 0);
	//wxOwnerDrawnComboBox* pComboCtrl = new wxOwnerDrawnComboBox();
	//pComboCtrl->Create(this, ID_OIJSD_INSTRUMENTID_COMBOCTRL, wxEmptyString,
	//			ctlPos, ctlSz, strArrCodeList, wxCB_DROPDOWN);

//	ctlPos.y += nVH;
//	ctlSz.Set(116, 20);
//	pWin = new wxComboBox(this, ID_OIJSD_LONGCODE_COMBOBOX, wxT(""), 
//			ctlPos, ctlSz, NULL, wxCB_DROPDOWN);

	wxArrayString strArrBS;
	strArrBS.Add(wxT("买入"));
	strArrBS.Add(wxT("卖出"));
	ctlPos.y += nVH;
	ctlSz.Set(nFW*8, 20);
	wxOwnerDrawnComboBox* pCmbBuySell = new wxOwnerDrawnComboBox(this, ID_OIJSD_BUYSELL_COMBOBOX, wxT("买入"), 
			ctlPos, ctlSz, strArrBS, wxCB_READONLY);
	pCmbBuySell->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(COrderInputPanel_jsd::OnCmbBuySell), NULL, this);

	ctlSz.Set(52, 20);
	pWin = new wxCheckBox( this, ID_OIJSD_HEDGE_CHECKBOX, _("保值"), wxPoint(ctlPos.x+nFW*9-4, ctlPos.y+4));

	wxArrayString strArrOC;
	strArrOC.Add(wxT("开仓"));
	strArrOC.Add(wxT("平今"));
	strArrOC.Add(wxT("平仓"));
	ctlPos.y += nVH;
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pCmbOpenClose = new wxOwnerDrawnComboBox(this, ID_OIJSD_OPENCLOSE_COMBOBOX, wxT("开仓"), 
			ctlPos, ctlSz, strArrOC, wxCB_READONLY);
	pCmbOpenClose->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(COrderInputPanel_jsd::OnCmbOpenClose), NULL, this);

	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Price(ctlPos);

	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Volume(ctlPos);

	wxArrayString strArrAccount;
	string strInvestorID="", strBrokerID="";
	if(g_pPlatform&&g_pPlatform->GetPlatformId()==PID_CTP) {
		if(g_pPlatform) g_pPlatform->GetBrokerIDUserID(strInvestorID, strBrokerID);
		strArrAccount.Add(wxT(strInvestorID));
	}
	else {
		set<string> setAccount;
		setAccount.clear();
		if(g_pPlatform) g_pPlatform->GetBrokerIDUserID(strInvestorID, strBrokerID, &setAccount);
		//setAccount.insert("QQ123456");
		//setAccount.insert("PATS9876543");
		//setAccount.insert("IB991753");
		if(setAccount.size()!=0) {
			for(set<string>::iterator it = setAccount.begin(); it != setAccount.end(); it++)
				strArrAccount.Add(wxT(*it));
		}
	}
	ctlPos.y += nVH;
	ctlSz.Set(116, 20);
	pComboBox = new wxOwnerDrawnComboBox(this, ID_OIJSD_ACCOUNT_TEXT, 
			wxT(""), ctlPos, ctlSz, strArrAccount, wxCB_DROPDOWN);//|wxCB_SORT);
	pComboBox->GetTextCtrl()->SetMaxLength(20);
	if(pComboBox->GetCount()>0)
		pComboBox->Select(0);


	InitOrderView();
	SetSellBuyColor();

	m_bIsInitOK = TRUE;

}

COrderInputPanel_jsd::~COrderInputPanel_jsd()
{
}

COrderInputPanel_jsd* COrderInputPanel_jsd::CreatePane(wxWindow* parent)
{
    return new COrderInputPanel_jsd(parent, wxID_ANY);
}

wxTextCtrl* COrderInputPanel_jsd::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_OIJSD_PRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

//	itemTextCtrl3->Connect(ID_OIJSD_PRICE_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnPriceClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_OIJSD_PRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* COrderInputPanel_jsd::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
    wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_OIJSD_VOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);
	itemTextCtrl3->SetMaxLength(4);

//	itemTextCtrl3->Connect(ID_OIJSD_VOLUME_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnVolumeClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_OIJSD_VOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 9999);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void COrderInputPanel_jsd::SetViewMode(int nViewMode)
{
	wxWindow* window = NULL;
	for(int i=ID_OIJSD_FIRST+1; i<SUMNUM_OIJSD_ID; i++) {
		window = FindWindowById(i, this);
		if(window!=NULL) window->Enable(true);
	}
	switch(nViewMode) {
	case 0:		// 自由下单
		break;
	case 1:		// 修改条件下单
		window=FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this);
		window->Enable(false);
		//window=FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX, this);
		//window->Enable(false);
		break;
	case 2:		// 修改盈损下单
		window=FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this);
		window->Enable(false);
		//window=FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX, this);
		//window->Enable(false);
		window=FindWindowById(ID_OIJSD_BUYSELL_COMBOBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_OPENCLOSE_COMBOBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_PRICE_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_VOLUME_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_HEDGE_CHECKBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_PRICE_SPINCTRL, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_VOLUME_SPINCTRL, this);
		window->Enable(false);
		break;
	case 3:		// 下盈损单
		window=FindWindowById(ID_OIJSD_AUTOOPENCLOSE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OIJSD_OPENCLOSE_COMBOBOX, this);
		window->Enable(false);
		SetOpenClose(0);
		break;
	};
}

BOOL COrderInputPanel_jsd::NeedMouseHandyPane_Code()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputInstrumentId;
}

BOOL COrderInputPanel_jsd::NeedMouseHandyPane_Volume()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputVolume;
}

BOOL COrderInputPanel_jsd::NeedMouseHandyPane_Price()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputPrice;
}

// 可以输入缩写和拼音
BOOL COrderInputPanel_jsd::NeedInputAbbreviation()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bInputAbbreviation;
}

wxString COrderInputPanel_jsd::Number2String(int num)
{
    wxString str="-";
    if(num!=-1) 
       str.Printf("%d",num);
    return str;
}

wxString COrderInputPanel_jsd::Double2String(double num)
{
	wxString strPriceText="-";
	if(num!=-1.0f && (!util::isInvalidValue(num))) {
		//一位，二位，没有小数位
		int percentnum=(int)(m_fPriceTick*10000.0f);
		if(percentnum%10!=0) {			//四位小数位
			strPriceText.Printf("%.4f",num);
		}
		else if(percentnum%100!=0) {	//三位小数位
			strPriceText.Printf("%.3f",num);
		}
		else if(percentnum%1000!=0) {	//二位小数位
			strPriceText.Printf("%.2f",num);
		}
		else if(percentnum%10000!=0) {	//一位小数位
			strPriceText.Printf("%.1f",num);
		}
		else {							//没有小数位
			strPriceText.Printf("%.0f",num);
		}
	}
	return strPriceText;
}

void COrderInputPanel_jsd::SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent)
{
	
	m_bNoKeyChangeText = FALSE;

	wxOwnerDrawnComboBox* itemCombo = NULL;
	itemCombo = wxDynamicCast(FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this),wxOwnerDrawnComboBox);
	if(itemCombo!=NULL) {
		itemCombo->SetText(strInstrumentId);
		//itemCombo->GetTextCtrl()->SetSelection(0, 0);
		//发送textchange
		if(bSendChangeEvent) {
			wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, ID_OIJSD_INSTRUMENTID_TEXT);
			event.SetString(strInstrumentId);
			itemCombo->ProcessEvent(event);
		}
	}

	////wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
	////pCmbLongCode->SetValue(strInstrumentId);

	//if(m_bIsLockForSetChange) {
	//	m_bNoKeyChangeText = TRUE;
	//	return;
	//}

	m_bNoKeyChangeText = TRUE;
}

wxString COrderInputPanel_jsd::GetInstrumentId()
{
	wxOwnerDrawnComboBox* itemCombo = 
			wxDynamicCast(FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this),wxOwnerDrawnComboBox);
	return itemCombo->GetValue();
}

void COrderInputPanel_jsd::SetAccount(wxString& strAccount)
{
	wxOwnerDrawnComboBox* itemCombo = NULL;
	itemCombo = wxDynamicCast(FindWindowById(ID_OIJSD_ACCOUNT_TEXT, this),wxOwnerDrawnComboBox);
	if(itemCombo!=NULL) {
		itemCombo->SetText(strAccount);
	}
}

wxString COrderInputPanel_jsd::GetAccount()
{
	wxOwnerDrawnComboBox* itemCombo = 
			wxDynamicCast(FindWindowById(ID_OIJSD_ACCOUNT_TEXT, this),wxOwnerDrawnComboBox);
	return itemCombo->GetValue();
}

void COrderInputPanel_jsd::SetDirection(BOOL bIsBuy)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_BUYSELL_COMBOBOX,this);
	pComboBox->SetSelection(bIsBuy?0:1);
	SetSellBuyColor();
}

// 获得买卖方向，TRUE为买入
BOOL COrderInputPanel_jsd::GetDirection()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_BUYSELL_COMBOBOX,this);
	return pComboBox->GetSelection()==0;
}

// 设置开平仓模式，0 开仓，1 平今，2 平仓
void COrderInputPanel_jsd::SetOpenClose(int nOCMode)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_OPENCLOSE_COMBOBOX,this);
	pComboBox->SetSelection(nOCMode);
}

// 获得开平仓模式，0 开仓，1 平今，2 平仓
int COrderInputPanel_jsd::GetOpenClose()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_OPENCLOSE_COMBOBOX,this);
	return pComboBox->GetSelection();
}

void COrderInputPanel_jsd::SetPriceText(wxString& strPrice)
{
	wxWindow* window=FindWindowById(ID_OIJSD_PRICE_TEXT, this);
	window->SetLabel(strPrice);
}

wxString COrderInputPanel_jsd::GetPriceText()
{
	wxWindow* window=FindWindowById(ID_OIJSD_PRICE_TEXT, this);
	return window->GetLabel();
}

void COrderInputPanel_jsd::SetPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_OIJSD_PRICE_TEXT,this);
	wxString str;
	if(fPrice==-1||util::isInvalidValue(fPrice)) 
		str="-";
	window->SetLabel(Double2String(fPrice));
}

double COrderInputPanel_jsd::GetPrice()
{
	wxWindow* window=FindWindowById(ID_OIJSD_PRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void COrderInputPanel_jsd::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_OIJSD_VOLUME_TEXT,this);
	window->SetLabel(strText);
}

int COrderInputPanel_jsd::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_OIJSD_VOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

// 设置是否保值
void COrderInputPanel_jsd::SetHedge(BOOL bIsHedge)
{
   wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OIJSD_HEDGE_CHECKBOX,this);
   pCheckBox->SetValue(bIsHedge==TRUE);
}

BOOL COrderInputPanel_jsd::GetHedge()
{
   wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OIJSD_HEDGE_CHECKBOX,this);
   return pCheckBox->GetValue();
}

// 设置自动开平模式
void COrderInputPanel_jsd::SetAutoOpenClose(BOOL bIsAuto)
{
	ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIJSD_AUTOOPENCLOSE_STATIC,this);
	pSText->SetNormal(!bIsAuto);
}

BOOL COrderInputPanel_jsd::GetAutoOpenClose()
{
	ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIJSD_AUTOOPENCLOSE_STATIC,this);
	return !pSText->IsNormal();
}

// 设置价格跟盘模式
void COrderInputPanel_jsd::SetAutoTrackPrice(BOOL bIsTrack)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIJSD_AUTOTRACKPRICE_STATIC,this);
   pSText->SetNormal(!bIsTrack);
}

BOOL COrderInputPanel_jsd::GetAutoTrackPrice()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIJSD_AUTOTRACKPRICE_STATIC,this);
   return !pSText->IsNormal();
}

void COrderInputPanel_jsd::InitOrderView()
{
    wxString strText;
	SetInstrumentId(strText);
    SetDirection(TRUE);
	SetOpenClose(0);		// 设置开平仓模式，0 开仓，1 平今，2 平仓
    SetPriceText(strText);
    SetQty(0);				// 设置手数
	SetHedge(FALSE);		// 设置是否保值
}

void COrderInputPanel_jsd::ClearViewData(int nMode)
{
	//1,下单后不清空下单板2,全部清空3,清空价格和手数4只清空手数
	wxString strText;
	switch(nMode) {
	case 2:
		SetInstrumentId(strText);
		//if(sm_nViewStyle==3)		// 如果是金仕达风格
		//	SetPanelItemText(ID_CMBLONGCODE, strText);
		SetDirection(TRUE);
		SetOpenClose(0);
		FindWindowById(ID_OIJSD_PRICE_TEXT, this)->SetLabel(wxT(""));
		FindWindowById(ID_OIJSD_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 3:
		FindWindowById(ID_OIJSD_PRICE_TEXT, this)->SetLabel(wxT(""));
		FindWindowById(ID_OIJSD_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 4:
		FindWindowById(ID_OIJSD_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	};
}

void COrderInputPanel_jsd::FillOrder(PlatformStru_InputOrder& req)
{
     
	memset(&req, 0, sizeof(req));

	//交易账号
	wxString strAccount = GetAccount();
	strncpy(req.strAccount, strAccount.c_str(), sizeof(req.strAccount)-1);

	//合约代码
	wxString strName = GetInstrumentId();
	strncpy(req.InstrumentID, strName.c_str(), sizeof(req.InstrumentID)-1);

	//买卖方向
	wxOwnerDrawnComboBox* pCmbBuySell = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_BUYSELL_COMBOBOX, this);
	req.Direction = (pCmbBuySell->GetSelection()==0 ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

	wxTextCtrl* pTxtPrice = (wxTextCtrl*)FindWindowById(ID_OIJSD_PRICE_TEXT, this);
	wxString strPriceText = pTxtPrice->GetLabel();

	double fPrice = 0.0f;
	BOOL bIsMarketPrice = FALSE;

	if(strPriceText.compare("市价")==0) {
		PlatformStru_DepthMarketData field;
        memset(&field,0,sizeof(field));
		if(g_pPlatform) g_pPlatform->GetQuotInfo(strName.c_str(), field);
		bIsMarketPrice=TRUE;
		if(req.Direction==THOST_FTDC_D_Buy) {
            fPrice = field.UpperLimitPrice;
		}
		else {
            fPrice = field.LowerLimitPrice;
		}
	}
	else {
		bIsMarketPrice=FALSE;
		fPrice=atof(strPriceText.c_str());
	}

	//报单价格条件
	if(bIsMarketPrice) {		/*市价单*/
		PlatformStru_InstrumentInfo outData;
		memset(&outData,0,sizeof(outData));
		CfgMgr* pMgr = CFG_MGR_DEFAULT();
		if(pMgr == NULL) return;
		LPORDER_CFG p = pMgr->GetOrderCfg();
		
		if(g_pPlatform) g_pPlatform->GetInstrumentInfo(req.InstrumentID, outData);

		if((!strcmp(outData.ExchangeID,"SHFE")) && (p->bSimulate)) {	//上期所不支持市价单，用限价单模拟
			
			req.LimitPrice = fPrice;

			req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
			//有效期类型
			req.TimeCondition=THOST_FTDC_TC_GFD;
		}
		else {
			req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
			//价格
			req.LimitPrice=0.0f;
			//有效期类型
			req.TimeCondition=THOST_FTDC_TC_IOC;
		}
	}
	else {	/*限价单*/
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//限价价格
		req.LimitPrice=fPrice;
		//有效期类型
		req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	wxOwnerDrawnComboBox* pCmbOC = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_OPENCLOSE_COMBOBOX, this);
	//开平标志
	switch(pCmbOC->GetSelection()) {
	case 0:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
		break;
	case 1: 
		req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;//平今
		break;
	case 2:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Close;//平仓
		break;
	};

	wxCheckBox* pChkHedge = (wxCheckBox*)FindWindowById(ID_OIJSD_HEDGE_CHECKBOX, this);
	//组合投机套保标志，只有第一个字节有效
	if(!pChkHedge->GetValue())
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	else
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;

	////限价价格
	//req.LimitPrice=GetContractPrice();
	//数量
	req.VolumeTotalOriginal=GetQty();
	//成交量类型
	req.VolumeCondition = THOST_FTDC_VC_AV; 
	//最小成交量
	req.MinVolume=1;
	//触发条件
	req.ContingentCondition=THOST_FTDC_CC_Immediately; 
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

}

// 从数据结构中读数据并显示
void COrderInputPanel_jsd::ShowOrder(PlatformStru_InputOrder& req)
{
	wxString strText;

	LockForSetChange();

	strText=req.InstrumentID;
	SetInstrumentId(strText);
	SetDirection(req.Direction==THOST_FTDC_D_Buy? 1 : 0);
	SetOpenClose(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? 0 :
		(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? 1 : 2));		// 设置开平仓模式，0 开仓，1 平今，2 平仓
	if(req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		strText=_("市价");
		SetPriceText(strText);
	}
	else
		SetPrice(req.LimitPrice);
	SetQty(req.VolumeTotalOriginal);							// 设置手数
	SetHedge(req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge);		// 设置是否保值

	UnlockForSetChange();
}

void COrderInputPanel_jsd::SetSellBuyColor()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxColor BuyColor=DEFAULT_COLOUR;
	wxColor SellColor=wxColor(0,128,255);
	if(pCfgMgr) {
		LPORDER_CFG	pOrder=pCfgMgr->GetOrderCfg();
		if(pOrder) {
			BuyColor=pOrder->BuyColor;
			SellColor=pOrder->SellColor;
		}
	}
	wxColor clr=(GetDirection())?BuyColor:SellColor;
	m_colorBak = clr;

	wxWindow* pWin = NULL;
	for(int i=ID_OIJSD_STATIC1; i<SUMNUM_OIJSD_ID; i++) {
		pWin = FindWindowById(i, this);
		if(pWin) pWin->SetBackgroundColour(m_colorBak);
	}
	//pWin = FindWindowById(ID_OIJSD_BUYSELL_COMBOBOX, this);
	//if(pWin) pWin->SetBackgroundColour(m_colorBak);

	//SetBackgroundColour(clr);
	Refresh();

}

void COrderInputPanel_jsd::OnInstrumentIdTextChanged(wxCommandEvent& event)
{
	
	if(!m_bIsInitOK)
		return;

	//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
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
			if(strInput.compare(tSKey.ShortKey.c_str())==0) {
				//m_bNoKeyChangeText = FALSE;
				wxString strSymbol = tSKey.Symbol.c_str();
				SetInstrumentId(strSymbol);
				event.SetString(strSymbol);
				//m_bNoKeyChangeText = TRUE;
				break;
			}
		}
	}

	// 向父窗体发送合约代码改变事件
	wxCommandEvent evtOut(wxEVT_JSDPANEL_INSTRUMENTID_CHANGED, event.GetId());
	evtOut.SetString(event.GetString());
	if(GetParent())
		GetParent()->ProcessEvent(evtOut);
	
}

void COrderInputPanel_jsd::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		int thisID = ConvertOwnerDrawnComboBoxID(evt.GetId());
		switch(thisID) {
		case ID_OIJSD_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this),wxOwnerDrawnComboBox);
			if(itemCombo != NULL) {
				int CurSel=0;
				BOOL bIsFound=FALSE, bIsEequal=FALSE;
				wxString strValue=itemCombo->GetValue();
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
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// 需要调用合约代码改变事件
					// 向父窗体发送合约代码改变事件
					wxCommandEvent evtOut(wxEVT_JSDPANEL_INSTRUMENTID_CHANGED, ID_OIJSD_INSTRUMENTID_TEXT);
					evtOut.SetString(itemCombo->GetValue());
					if(GetParent())
						GetParent()->ProcessEvent(evtOut);
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OIJSD_PRICE_TEXT:
			evtSpin.SetId(ID_OIJSD_PRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_OIJSD_VOLUME_TEXT:
			evtSpin.SetId(ID_OIJSD_VOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		int thisID = ConvertOwnerDrawnComboBoxID(evt.GetId());
		switch(thisID) {
		case ID_OIJSD_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this),wxOwnerDrawnComboBox);
			if(itemCombo != NULL) {
				int CurSel=0;
				BOOL bIsFound=FALSE, bIsEequal=FALSE;
				wxString strValue=itemCombo->GetValue();
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
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// 需要调用合约代码改变事件
					// 向父窗体发送合约代码改变事件
					wxCommandEvent evtOut(wxEVT_JSDPANEL_INSTRUMENTID_CHANGED, ID_OIJSD_INSTRUMENTID_TEXT);
					evtOut.SetString(itemCombo->GetValue());
					if(GetParent())
						GetParent()->ProcessEvent(evtOut);
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OIJSD_PRICE_TEXT:
			evtSpin.SetId(ID_OIJSD_PRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_OIJSD_VOLUME_TEXT:
			evtSpin.SetId(ID_OIJSD_VOLUME_SPINCTRL);
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

			if(thisID == ID_OIJSD_BUYSELL_COMBOBOX) {
				if(nChar==p->cBuyHotKey) {
					SetDirection(TRUE);
					return;
				}
				else if(nChar==p->cSelHotKey) {
					SetDirection(FALSE);
					return;
				}
			}
			else if(thisID == ID_OIJSD_OPENCLOSE_COMBOBOX) {
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

void COrderInputPanel_jsd::OnStaticLeftUp(wxCommandEvent& event)
{
	if(event.GetId()==ID_OIJSD_AUTOOPENCLOSE_STATIC) {
		// 向父窗体发送自动开平仓改变事件
		wxCommandEvent evtOut(wxEVT_JSDPANEL_AUTOOPENCLOSE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoOpenClose());
		if(GetParent())
			GetParent()->ProcessEvent(evtOut);
		//if(!pSText->IsNormal()) {
		//wxString strName=GetContractName();
		//if(m_InstrumentIDArray.find(strName.c_str())!=m_InstrumentIDArray.end())
		//{
		////if(
		//wxExtListCtrl *p=(wxExtListCtrl*)FindWindowById(GID_POSITION);
		//int checkid=ID_RADIOBUTTON3;
		//for(int row=0;row<p->GetItemCount();row++)
		//{
		//LPPOSITION_INFO pInfo=(LPPOSITION_INFO)p->GetItemData(row);
		//if(strName==pInfo->InstrumentID&&m_DirectionSellBuy!=pInfo->Direction)
		//{
		//if(pInfo->YdVolume>0)//有昨仓
		//checkid=ID_RADIOBUTTON5;
		//else
		//checkid=ID_RADIOBUTTON4;
		//break;
		//}
		//}
		//SetButtonCheck(checkid);
		//}
		//}
	}
	else if(event.GetId()==ID_OIJSD_AUTOTRACKPRICE_STATIC) {
		// 向父窗体发送自动跟踪价格改变事件
		wxCommandEvent evtOut(wxEVT_JSDPANEL_AUTOTRACKPRICE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoTrackPrice());
		if(GetParent())
			GetParent()->ProcessEvent(evtOut);
		//if(!pSText->IsNormal())
		//{
		//if(GetButtonCheck(ID_RADIOBUTTON1))
		//SetContractPrice(m_DynamicItem.SellPrice);
		//else
		//SetContractPrice(m_DynamicItem.BuyPrice);
		//} 
	}

}

void COrderInputPanel_jsd::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
    SetPrice(fPrice+m_fPriceTick);
}

void COrderInputPanel_jsd::OnPriceSpinDown(wxSpinEvent& event)
{
    double price=GetPrice();
    SetPrice(price-m_fPriceTick);
}

void COrderInputPanel_jsd::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void COrderInputPanel_jsd::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void COrderInputPanel_jsd::OnCmbBuySell(wxCommandEvent& evt)
{
	int nSel = evt.GetSelection();
	SetSellBuyColor();
	// 向父窗体发送自动跟踪价格改变事件
	wxCommandEvent evtOut(wxEVT_KQPANEL_BUYSELLSEL_CHANGED, evt.GetId());
	evtOut.SetInt(GetDirection());
	if(GetParent())
		GetParent()->AddPendingEvent(evtOut);
}

void COrderInputPanel_jsd::OnCmbOpenClose(wxCommandEvent& evt)
{
	SetAutoOpenClose(FALSE);
}

void COrderInputPanel_jsd::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
	wxRect rect(0,0,ORDERPANEL_WIDTH,26*5+10);
	dc.SetPen(wxPen(wxColour(172,168,153)));
	dc.SetBrush(wxBrush(m_colorBak));
	dc.DrawRectangle(rect);
	dc.SetPen(wxPen(wxColour(255,255,255)));
	dc.DrawLine(1,1,1,26*5+10-1);
	//dc.GradientFillLinear(rect,m_colorBak,m_colorBak);
}

void COrderInputPanel_jsd::OnCfgChanged(wxCommandEvent& evt)
{
	SetSellBuyColor();

	//ToggleStaticText* itemText = (ToggleStaticText*)
	//		FindWindowById(ID_OIJSD_AUTOOPENCLOSE_STATIC);
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

void COrderInputPanel_jsd::ReloadInstrumentIDList()
{
	wxArrayString strArrCodeList;
	std::set<std::string> outData;
	// 获取合约列表
	outData.clear();
	if(g_pPlatform) g_pPlatform->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		strArrCodeList.Add(*it);
	}
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_INSTRUMENTID_TEXT, this);
	if(pComboBox) {
		pComboBox->Clear();
		for(int i=0; i<(int)strArrCodeList.size(); i++) {
			pComboBox->Insert(wxString(strArrCodeList[i]), i);
		}
	}

}

void COrderInputPanel_jsd::ResetAccountList(vector<string>& accountList)
{
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIJSD_ACCOUNT_TEXT, this);
	if(pComboBox != NULL) {
		pComboBox->Clear();
		for(int i=0; i<(int)accountList.size(); i++) {
			pComboBox->Insert(accountList[i], i);
			pComboBox->Select(0);
		}
	}
}
