#include "StdAfx.h"
#include "COrderConfirmPanel.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc2/tools_util.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


BEGIN_EVENT_TABLE(COrderConfirmPanel, CBaseInputPanel)

	EVT_KILL_FOCUS(OnPriceTextFocus)
    EVT_COMMAND(wxID_ANY, wxEVT_PRICECLICKED, OnNextPriceClicked) 

    EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_DIGIT, OnMouseInputDigit) 

    EVT_SPIN_UP(ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_ORDERCONFIRMPANEL_NEWVOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_ORDERCONFIRMPANEL_NEWVOLUME_SPINCTRL, OnVolumeSpinDown)
	EVT_CFG_CHANGE(OnCfgChanged)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderConfirmPanel::COrderConfirmPanel(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_bIsAuto(TRUE),
	m_bIsMarketPrice(FALSE),
	m_nGroupIndex(0),
	m_MouseHandyPopupPaneDigit(NULL),
	m_colorBak(0,0,0)
{

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	//oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);
	
	m_MouseHandyPopupPaneDigit = new CMouseHandyInputPaneDigit(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400), wxNO_BORDER|wxSTATIC_BORDER);
	m_MouseHandyPopupPaneDigit->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderConfirmPanel::OnPricePanelKeyAscii),NULL,this);

}

COrderConfirmPanel::~COrderConfirmPanel()
{
	if(m_MouseHandyPopupPaneDigit!=NULL)
	{
		delete m_MouseHandyPopupPaneDigit;
		m_MouseHandyPopupPaneDigit = NULL;
	}
}

void COrderConfirmPanel::CreateGUIControls()
{
	wxPoint posInit(4, 4);
	int nFW = 8, nFH = 16;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	// 这里不需要处理，
	// 语言问题会在ResetLanguage()里完成

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*6, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC1, wxEmptyString, ctlPos, ctlSz);//以价格

	ctlPos.x = posInit.x + (nFW*6);//+nFW/2;
	ctlPos.y = posInit.x - 2;
	CreateFloatSpinCtrl_Price(ctlPos);

	ctlPos = wxPoint(posInit.x+nFW*20, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC2, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//自动

	ctlPos = wxPoint(posInit.x+nFW*24, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC3, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//买卖

	ctlPos = wxPoint(posInit.x+nFW*28, posInit.y);
	ctlSz.Set(nFW*18, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC4, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//合约代码

	ctlPos = wxPoint(posInit.x+nFW*50, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC5, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//开平
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI)
	{
		itemTemp->Show(false);
	}

	ctlPos.x = posInit.x + (nFW*54);//+nFW/2;
	ctlPos.y = posInit.x - 2;
	CreateFloatSpinCtrl_Volume(ctlPos);

	ctlPos = wxPoint(posInit.x+nFW*63-4, posInit.y);
	ctlSz.Set(nFW*2, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC6, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//手

	ctlPos = wxPoint(posInit.x+nFW*65, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_ORDERCONFIRMPANEL_STATIC7, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//投保
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI)
	{
		itemTemp->Show(false);
	}

	InitOrderView();
}

COrderConfirmPanel* COrderConfirmPanel::CreatePane(wxWindow* parent)
{
    return new COrderConfirmPanel(parent, wxID_ANY);
}

wxTextCtrl* COrderConfirmPanel::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(80, 20);
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, 
			ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, wxEmptyString, ctlPos, ctlSz, 0 );

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderConfirmPanel::OnPriceClicked), NULL, this);
    //itemTextCtrl3->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(COrderConfirmPanel::OnPriceTextFocus), NULL, this);

	ctlPos.x += 80;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* COrderConfirmPanel::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(50, 20);
    wxTextCtrl* itemTextCtrl3 = new wxTextCtrl(this, 
				ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2, wxEmptyString, ctlPos, ctlSz, 0);
	itemTextCtrl3->SetMaxLength(4);

//	itemTextCtrl3->Connect(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(OnVolumeClicked), NULL, this);

	ctlPos.x += 50;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_ORDERCONFIRMPANEL_NEWVOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 9999);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void COrderConfirmPanel::SetPriceText(wxString& strPrice)
{
	wxWindow* window=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
	window->SetLabel(strPrice);
}

wxString COrderConfirmPanel::GetPriceText()
{
	wxWindow* window=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
	wxString strText=window->GetLabel();
	return strText.Trim();
}

void COrderConfirmPanel::SetPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
}

double COrderConfirmPanel::GetPrice()
{
	wxWindow* window=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void COrderConfirmPanel::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2,this);
	window->SetLabel(strText);
}

int COrderConfirmPanel::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

void COrderConfirmPanel::InitOrderView()
{
    wxString strText;
    SetPriceText(strText);
    SetQty(0);				// 设置手数
}

void COrderConfirmPanel::FillOrder(PlatformStru_InputOrder& req)
{
	
	BOOL bIsMarketPrice = m_bIsMarketPrice;
	double fPrice = 0.0f;
	string strPriceText = GetPriceText();
	string strName = req.InstrumentID;
	BOOL bIsBuy = req.Direction==THOST_FTDC_D_Buy;
	//Macro
	AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
	
	char cOldType = req.OrderPriceType;
	//报单价格条件
	if(bIsMarketPrice && cOldType!=THOST_FTDC_OPT_AnyPrice) {		//市价单
		req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		//价格
		req.LimitPrice=0.0f;
		//有效期类型
		req.TimeCondition=THOST_FTDC_TC_IOC;
	}
	else if(!bIsMarketPrice) {	//限价单
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//限价价格
		req.LimitPrice=fPrice;
		//有效期类型
		if(cOldType==THOST_FTDC_OPT_AnyPrice)	// 只有原来是市价才重置TimeCondition字段
			req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	//数量
	req.VolumeTotalOriginal=GetQty();

}

void COrderConfirmPanel::FillOrder(PlatformStru_ParkedOrder& req)
{
	
	BOOL bIsMarketPrice = FALSE;
	double fPrice = 0.0f;
	string strPriceText = GetPriceText();
	string strName = req.InstrumentID;
	BOOL bIsBuy = req.Direction==THOST_FTDC_D_Buy;

	//Macro
	AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
	
	//报单价格条件
	if(bIsMarketPrice) {		//市价单
		req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		//价格
		req.LimitPrice=0.0f;
		//有效期类型
		req.TimeCondition=THOST_FTDC_TC_IOC;
	}
	else {	//限价单
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//限价价格
		req.LimitPrice=fPrice;
		//有效期类型
		req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	//数量
	req.VolumeTotalOriginal=GetQty();

}

// 从数据结构中读数据并显示
void COrderConfirmPanel::ShowOrder(PlatformStru_InputOrder& ftdcField)
{

	wxWindow* pwinItem=NULL;
	wxString strValue;

    m_strInstrumentId = ftdcField.InstrumentID;

//	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
//	strValue.Printf("合约:  %s", ftdcField.InstrumentID);
//	if(pwinItem) pwinItem->SetLabel(strValue);

//	if(ftdcField.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
	if(m_bIsMarketPrice) {
		strValue.Printf(wxString(LOADSTRING(OLS_MARKET)));
//		strValue.Printf(wxString(LOADSTRING(OLS_MARKET))+" ");
//		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
//		if(pwinItem) pwinItem->Show(false);
//		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL, this);
//		if(pwinItem) pwinItem->Show(false);
//		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
//		if(pwinItem) pwinItem->SetLabel(LOADSTRING(USE_MKT));
	}
	else {
	    //pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
	    //if(pwinItem) pwinItem->SetLabel(LOADSTRING(USE_PRICE));
		strValue = Price2String(ftdcField.LimitPrice, m_fPriceTick).c_str();
	}
	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
	if(pwinItem) pwinItem->SetLabel(LOADSTRING(USE_PRICE));
	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC2, this);
	if(pwinItem) pwinItem->SetLabel(m_bIsAuto ? LOADSTRING(PARK_AUTO) :wxEmptyString);


	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC3, this);
	if(pwinItem) pwinItem->SetLabel(
				ftdcField.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY) : LOADSTRING(OLS_SELL));


	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC4, this);
	strValue.Printf("%s", ftdcField.InstrumentID);
	if(pwinItem) pwinItem->SetLabel(strValue);

	//if(PLATFORM_SVR().GetPlatformType()==PTYPE_CTP) {
	if(DEFAULT_SVR()&&DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_OPENCLOSE)) {
		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC5, this);
		if(pwinItem) 
			pwinItem->SetLabel(ftdcField.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN) : 
					(ftdcField.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)));
	}


	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2, this);
	strValue.Printf("%d", ftdcField.VolumeTotalOriginal);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC6, this);
	if(pwinItem) pwinItem->SetLabel(LOADSTRING(QTY_COUNT));
	strValue = Price2String(ftdcField.LimitPrice, m_fPriceTick).c_str();

	//if(PLATFORM_SVR().GetPlatformType()==PTYPE_CTP) {
	if(DEFAULT_SVR()&&DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_HEDGE)) {
		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC7, this);
		if(pwinItem) 
			pwinItem->SetLabel(
					ftdcField.CombHedgeFlag[0]==THOST_FTDC_BHF_Speculation ? LOADSTRING(OLS_SPECULATE) : LOADSTRING(OLS_HEDGE));
	}


}

void COrderConfirmPanel::ShowOrder(PlatformStru_ParkedOrder& ftdcField)
{

	wxWindow* pwinItem=NULL;
	wxString strValue;

    m_strInstrumentId = ftdcField.InstrumentID;

//	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
//	strValue.Printf("合约:  %s", ftdcField.InstrumentID);
//	if(pwinItem) pwinItem->SetLabel(strValue);

//	if(ftdcField.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
	if(m_bIsMarketPrice) {
		strValue.Printf(wxString(LOADSTRING(OLS_MARKET))+" ");
		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
		if(pwinItem) pwinItem->Show(false);
		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL, this);
		if(pwinItem) pwinItem->Show(false);
		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
		if(pwinItem) pwinItem->SetLabel(LOADSTRING(USE_MKT));
	}
	else {
		pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC1, this);
		if(pwinItem) pwinItem->SetLabel(LOADSTRING(USE_PRICE));
		strValue = Price2String(ftdcField.LimitPrice, m_fPriceTick).c_str();
	}
	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC2, this);
	if(pwinItem) pwinItem->SetLabel(m_bIsAuto ? LOADSTRING(PARK_AUTO) :wxEmptyString);


	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC3, this);
	if(pwinItem) pwinItem->SetLabel(
				ftdcField.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL));


	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC4, this);
	strValue.Printf("%s", ftdcField.InstrumentID);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC5, this);
	if(pwinItem) pwinItem->SetLabel(ftdcField.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): 
				(ftdcField.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)));


	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2, this);
	strValue.Printf("%d", ftdcField.VolumeTotalOriginal);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC6, this);
	if(pwinItem) pwinItem->SetLabel(LOADSTRING(QTY_COUNT));
	strValue = Price2String(ftdcField.LimitPrice, m_fPriceTick).c_str();

	pwinItem=FindWindowById(ID_ORDERCONFIRMPANEL_STATIC7, this);
	if(pwinItem) pwinItem->SetLabel(
				ftdcField.CombHedgeFlag[0]==THOST_FTDC_BHF_Speculation ? LOADSTRING(OLS_SPECULATE) : LOADSTRING(OLS_HEDGE));

}

BOOL COrderConfirmPanel::NeedMouseHandyPane_Price()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputPrice;
}

void COrderConfirmPanel::OnPriceClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_PRICECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderConfirmPanel::OnNextPriceClicked(wxCommandEvent& evt)
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

void COrderConfirmPanel::OnPricePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderConfirmPanel::OnMouseInputDigit(wxCommandEvent& evt)
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
		FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this)->SetFocus();
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
        wxString name=m_strInstrumentId;
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
        wxString name=m_strInstrumentId;
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
        wxString name=m_strInstrumentId;
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

void COrderConfirmPanel::OnHookKeyPress(wxKeyEvent& evt)
{
	wxSpinEvent evtSpin;
	if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		int thisID=evt.GetId();
		if(thisID == ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2) {
			evtSpin.SetId(ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
		}
		else if(thisID == ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2) {
			evtSpin.SetId(ID_ORDERCONFIRMPANEL_NEWVOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
		}
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		int thisID=evt.GetId();
		if(thisID == ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2) {
			evtSpin.SetId(ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
		}
		else if(thisID == ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2) {
			evtSpin.SetId(ID_ORDERCONFIRMPANEL_NEWVOLUME_SPINCTRL);
			OnVolumeSpinUp(evtSpin);
			return;
		}
	}
	evt.Skip();
}

void COrderConfirmPanel::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
    SetPrice(fPrice);
}

void COrderConfirmPanel::OnPriceSpinDown(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
    SetPrice(fPrice);
}

void COrderConfirmPanel::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void COrderConfirmPanel::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void COrderConfirmPanel::OnCfgChanged(wxCommandEvent& evt)
{
}

void COrderConfirmPanel::OnPriceTextFocus(wxFocusEvent& evt)
{
    //if(m_MouseHandyPopupPaneDigit)
    //{
    //    wxWindow* pFocus = FindFocus();
    //    wxWindow* pPriceText = FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
    //    if(pFocus && pPriceText) 
    //    {
    //        if(pFocus->GetId()!=pPriceText->GetId())
    //            m_MouseHandyPopupPaneDigit->Show(false);
    //    }
    //    //if(evt.GetEventType()==wxEVT_KILL_FOCUS)
    //}
}

// 仅此用于日志文件输出
wxString COrderConfirmPanel::GetUserInputText()
{
	wxString strRet;
	
	wxWindow* pWin = NULL;

	pWin = FindWindowById(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+m_nGroupIndex*2, this);
	if(pWin!=NULL) {
		strRet+="价格[";
		strRet+=pWin->GetLabel();
		strRet+="],\t ";
	}

	pWin = FindWindowById(ID_ORDERCONFIRMPANEL_STATIC3, this);
	if(pWin!=NULL) {
		strRet+=pWin->GetLabel();
	}
	
	pWin = FindWindowById(ID_ORDERCONFIRMPANEL_STATIC4, this);
	if(pWin!=NULL) {
		strRet+="[";
		strRet+=pWin->GetLabel();
		strRet+="], \t";
	}

	pWin = FindWindowById(ID_ORDERCONFIRMPANEL_STATIC5, this);
	if(pWin!=NULL) {
		strRet+=pWin->GetLabel();
	}
	
	pWin = FindWindowById(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+m_nGroupIndex*2, this);
	if(pWin!=NULL) {
		strRet+="[";
		strRet+=pWin->GetLabel();
		strRet+="]";
	}

	strRet+="\r\n";
	return strRet;

}
