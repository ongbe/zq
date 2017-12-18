#include "StdAfx.h"
#include "COrderStopPanel.h"




BEGIN_EVENT_TABLE(COrderStopPanel, wxPanel)
	EVT_RADIOBUTTON(ID_STOP_RADIOSTOP, OnRadioStopSel)
	EVT_RADIOBUTTON(ID_STOP_RADIOSTOPLIMIT, OnRadioStopSel)
    EVT_SPIN_UP(ID_STOP_SPINPRICE,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_STOP_SPINPRICE,OnPriceSpinDown)
	EVT_SPIN_UP(ID_STOP_SPINTRIGGERPRICE,OnTriggerPriceSpinUp)
	EVT_SPIN_DOWN(ID_STOP_SPINTRIGGERPRICE,OnTriggerPriceSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderStopPanel::COrderStopPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_fPriceTick(1.0f)
{

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+6;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	// 这里不需要处理，
	// 语言问题会在ResetLanguage()里完成

	ctlPos = wxPoint(posInit.x, posInit.y+4);
	ctlSz.Set(nFW*6, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_STOP_RADIOSTOP, wxEmptyString, ctlPos, ctlSz);//, wxRB_GROUP);
	itemRadioBox1->SetValue(true);
	ctlPos = wxPoint(posInit.x+nFW*7, posInit.y+4);
	ctlSz.Set(nFW*12, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton( this, ID_STOP_RADIOSTOPLIMIT, wxEmptyString, ctlPos, ctlSz);//, wxRB_GROUP);
	ctlPos = wxPoint(posInit.x+nFW*7+nFW*7, posInit.y+4);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH+4);
	new wxStaticText( this, ID_STOP_STATICTRIGGERPRICE, wxEmptyString, ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+nFW*8+2, posInit.y+nVH);
	ctlSz.Set(nFW*10, 20);
	wxTextCtrl* itemTextCtrl1 = new wxTextCtrl( this, ID_STOP_TEXTTRIGGERPRICE, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*18+2, posInit.y+nVH);
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton1 = new wxSpinButton( this, ID_STOP_SPINTRIGGERPRICE, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton1->SetRange(-1000000, 1000000);
    itemSpinButton1->SetValue(0);

	ctlPos = wxPoint(posInit.x, posInit.y+2*nVH+4);
	new wxStaticText( this, ID_STOP_STATICPRICE, wxEmptyString, ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+nFW*8+2, posInit.y+2*nVH);
	ctlSz.Set(nFW*10, 20);
	itemTextCtrl1 = new wxTextCtrl( this, ID_STOP_TEXTPRICE, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*18+2, posInit.y+2*nVH);
	ctlSz.Set(16, 20);
    itemSpinButton1 = new wxSpinButton( this, ID_STOP_SPINPRICE, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton1->SetRange(-1000000, 1000000);
    itemSpinButton1->SetValue(0);

	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_STOP_STATICPRICE, this);
	if(pWin!=NULL) pWin->Show(false);
	pWin = FindWindowById(ID_STOP_TEXTPRICE, this);
	if(pWin!=NULL) pWin->Show(false);
	pWin = FindWindowById(ID_STOP_SPINPRICE, this);
	if(pWin!=NULL) pWin->Show(false);

	ResetLanguage();

}

COrderStopPanel::~COrderStopPanel()
{
}

COrderStopPanel* COrderStopPanel::CreatePane(wxWindow* parent)
{
    return  new COrderStopPanel(parent, GID_QUERY_ORDER_CONDITION);
}

void COrderStopPanel::OnRadioStopSel(wxCommandEvent& event)
{
	wxWindow* pWin = NULL;
	if(event.GetId()==ID_STOP_RADIOSTOP) {
		pWin = FindWindowById(ID_STOP_STATICPRICE, this);
		if(pWin!=NULL) pWin->Show(false);
		pWin = FindWindowById(ID_STOP_TEXTPRICE, this);
		if(pWin!=NULL) pWin->Show(false);
		pWin = FindWindowById(ID_STOP_SPINPRICE, this);
		if(pWin!=NULL) pWin->Show(false);
	}
	else if(event.GetId()==ID_STOP_RADIOSTOPLIMIT) {
		pWin = FindWindowById(ID_STOP_STATICPRICE, this);
		if(pWin!=NULL) pWin->Show(true);
		pWin = FindWindowById(ID_STOP_TEXTPRICE, this);
		if(pWin!=NULL) pWin->Show(true);
		pWin = FindWindowById(ID_STOP_SPINPRICE, this);
		if(pWin!=NULL) pWin->Show(true);
	}
}

void COrderStopPanel::OnPriceSpinUp(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTPRICE,this);
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void COrderStopPanel::OnPriceSpinDown(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTPRICE,this);
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
	//if(price<0.0f)
	//	return;
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void COrderStopPanel::OnTriggerPriceSpinUp(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTTRIGGERPRICE,this);
	double price=atof(window->GetLabel())+m_fPriceTick;
	wxString strValue;
	strValue = Price2String(price, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void COrderStopPanel::OnTriggerPriceSpinDown(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTTRIGGERPRICE,this);
	double price=atof(window->GetLabel())-m_fPriceTick;
	if(price<0.0f)
		return;
	wxString strValue;
	strValue = Price2String(price, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void COrderStopPanel::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxComboBox* itemCombo = NULL;
		int thisID=evt.GetId();
		switch(thisID) {
		case ID_STOP_TEXTPRICE:
			evtSpin.SetId(ID_STOP_SPINPRICE);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_STOP_TEXTTRIGGERPRICE:
			evtSpin.SetId(ID_STOP_SPINTRIGGERPRICE);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxComboBox* itemCombo = NULL;
		int thisID=evt.GetId();
		switch(thisID) {
		case ID_STOP_TEXTPRICE:
			evtSpin.SetId(ID_STOP_SPINPRICE);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_STOP_TEXTTRIGGERPRICE:
			evtSpin.SetId(ID_STOP_SPINTRIGGERPRICE);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		};
	}
	evt.Skip();

}

void COrderStopPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_STOP_RADIOSTOP, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP));
	pWin = FindWindowById(ID_STOP_RADIOSTOPLIMIT, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP_LIMIT));
	pWin = FindWindowById(ID_STOP_STATICPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_PRICE));
	pWin = FindWindowById(ID_STOP_STATICTRIGGERPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_TRIGGER_PRICE));
}

// 仅此用于日志文件输出
wxString COrderStopPanel::GetUserInputText()
{
	wxString strRet;

	strRet = "停止单数据：\n";

	BOOL bIsStopLimit = FALSE;

	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_STOP_RADIOSTOP,this);
		if(window->GetValue()) {
			strRet+="下单类型=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
		window=(wxRadioButton*)FindWindowById(ID_STOP_RADIOSTOPLIMIT,this);
		if(window->GetValue()) {
			bIsStopLimit=TRUE;
			strRet+="下单类型=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}

	{
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTTRIGGERPRICE,this);
		strRet+="触发价格=[";
		strRet+=window->GetLabel();
		strRet+="],\t ";
	}

	if(bIsStopLimit) {
		wxComboBox* window=NULL;
		window=(wxComboBox*)FindWindowById(ID_STOP_TEXTPRICE,this);
		strRet+="价格=[";
		strRet+=window->GetLabel();
		strRet+="],\t ";
	}

	strRet+="\n";
	return strRet;

}