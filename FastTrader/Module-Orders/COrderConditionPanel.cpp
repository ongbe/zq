#include "StdAfx.h"
#include "COrderConditionPanel.h"



extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(COrderConditionPanel, wxPanel)
    EVT_SPIN_UP(ID_CONDITION_SPINCTRLBUTTON,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_CONDITION_SPINCTRLBUTTON,OnPriceSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderConditionPanel::COrderConditionPanel(wxWindow *parent,
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
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_CONDITION_RADIONEWPRICE, wxEmptyString, ctlPos, ctlSz);//最新//, wxRB_GROUP);
	itemRadioBox1->SetValue(true);
	ctlPos = wxPoint(posInit.x+nFW*7+nFW/2, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton( this, ID_CONDITION_RADIOBUYPRICE, wxEmptyString, ctlPos, ctlSz);//买价//, wxRB_GROUP);
	ctlPos = wxPoint(posInit.x+nFW*7+nFW*7+nFW/2, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox3 = new wxRadioButton( this, ID_CONDITION_RADIOSELLPRICE, wxEmptyString, ctlPos, ctlSz);//卖价//, wxRB_GROUP);

	wxArrayString strArrCondi;
	strArrCondi.Add(wxEmptyString);//小于等于
	strArrCondi.Add(wxEmptyString);//大于等于
	strArrCondi.Add(wxEmptyString);//小于
	strArrCondi.Add(wxEmptyString);//大于
	ctlPos = wxPoint(posInit.x, posInit.y+nVH);
	ctlSz.Set(nFW*10, 20);
	wxComboBox* itemComboBox1 = new wxComboBox(this, ID_CONDITION_COMBOCONDITION, wxEmptyString, //小于等于
			ctlPos, ctlSz, strArrCondi, wxCB_READONLY);
	itemComboBox1->SetSelection(0);

	ctlPos = wxPoint(posInit.x+nFW*10+2, posInit.y+nVH);
	ctlSz.Set(nFW*8, 20);
	wxTextCtrl* itemTextCtrl1 = new wxTextCtrl( this, ID_CONDITION_TEXTPRICE, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*18+2, posInit.y+nVH);
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton1 = new wxSpinButton( this, ID_CONDITION_SPINCTRLBUTTON, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton1->SetRange(-1000000, 1000000);
    itemSpinButton1->SetValue(0);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2+4);
	ctlSz.Set(nFW*16, nFH);
	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_CONDITION_CHECKTOSERVER, wxEmptyString, ctlPos, ctlSz);//在服务器上运行
	if(DEFAULT_SVR()&&!DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_CONDITION_SERVER)) {
		itemCheckBox1->Show(false);
	}
	ResetLanguage();
}

COrderConditionPanel::~COrderConditionPanel()
{
}

COrderConditionPanel* COrderConditionPanel::CreatePane(wxWindow* parent)
{
    return  new COrderConditionPanel(parent, GID_QUERY_ORDER_CONDITION);
}

void COrderConditionPanel::SetPriceType(int nPriceType) 
{
	wxRadioButton* window=NULL;
	switch(nPriceType) {
	case 1:
		window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIONEWPRICE,this);
		if(window) window->SetValue(1);
		break;
	case 2:
		window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIOBUYPRICE,this);
		if(window) window->SetValue(1);
		break;
	case 3:
		window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIOSELLPRICE,this);
		if(window) window->SetValue(1);
		break;
	};
}

int COrderConditionPanel::GetPriceType() 
{
	wxRadioButton* window=NULL;
	window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIONEWPRICE,this);
	if(window->GetValue()) return 1;
	window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIOBUYPRICE,this);
	if(window->GetValue()) return 2;
	window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIOSELLPRICE,this);
	if(window->GetValue()) return 3;
	return -1;
}

void COrderConditionPanel::SetConditionType(int nConditionType) 
{
	wxComboBox* window=NULL;
	window=(wxComboBox*)FindWindowById(ID_CONDITION_COMBOCONDITION,this);
	if(window) window->SetSelection(nConditionType-1);
}

int COrderConditionPanel::GetConditionType() 
{
	wxComboBox* window=NULL;
	window=(wxComboBox*)FindWindowById(ID_CONDITION_COMBOCONDITION,this);
	return window->GetSelection()+1;
}

void COrderConditionPanel::SetConditionPrice(double fPrice) 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_CONDITION_TEXTPRICE,this);
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick);
	if(window) window->SetLabel(strValue);
}

double COrderConditionPanel::GetConditionPrice() 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_CONDITION_TEXTPRICE,this);
	return atof(window->GetLabel());
}

// 设置是在本地执行还是服务器上执行
void COrderConditionPanel::SetIsRunInServer(BOOL bIsRunInServer) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_CONDITION_CHECKTOSERVER,this);
	if(window) window->SetValue(bIsRunInServer==TRUE);
}

BOOL COrderConditionPanel::GetIsRunInServer() 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_CONDITION_CHECKTOSERVER,this);
	if(window) return window->GetValue()!=0;
	return FALSE;
}

void COrderConditionPanel::OnPriceSpinUp(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_CONDITION_TEXTPRICE,this);
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick);
	window->SetLabel(strValue);
}

void COrderConditionPanel::OnPriceSpinDown(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_CONDITION_TEXTPRICE,this);
	double fPrice=atof(window->GetLabel());
	//if(price<0.0f)
	//	return;
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick);
	window->SetLabel(strValue);
}

void COrderConditionPanel::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxComboBox* itemCombo = NULL;
		int thisID=evt.GetId();
		switch(thisID) {
		case ID_CONDITION_TEXTPRICE:
			evtSpin.SetId(ID_CONDITION_SPINCTRLBUTTON);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxComboBox* itemCombo = NULL;
		int thisID=evt.GetId();
		switch(thisID) {
		case ID_CONDITION_TEXTPRICE:
			evtSpin.SetId(ID_CONDITION_SPINCTRLBUTTON);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		};
	}
	evt.Skip();

}

void COrderConditionPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_CONDITION_RADIONEWPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_LASTPRICE2));
	pWin = FindWindowById(ID_CONDITION_RADIOBUYPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_BIDPRICE));
	pWin = FindWindowById(ID_CONDITION_RADIOSELLPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_ASKPRICE));
	pWin = FindWindowById(ID_CONDITION_CHECKTOSERVER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_RUN_IN_SERVER));
	wxComboBox* itemComboBox1 = (wxComboBox*)FindWindowById(ID_CONDITION_COMBOCONDITION, this);
	itemComboBox1->SetString(0, LOADSTRING(OLS_OP_SMEQU));
	itemComboBox1->SetString(1, LOADSTRING(OLS_OP_LARGEEQU));
	itemComboBox1->SetString(2, LOADSTRING(OLS_OP_SM));
	itemComboBox1->SetString(3, LOADSTRING(OLS_OP_LARGE));
}

// 仅此用于日志文件输出
wxString COrderConditionPanel::GetUserInputText()
{
	wxString strRet;

	strRet = LOADSTRING(OBP_INPUT_BEGIN1);

	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIONEWPRICE,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
		window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIOBUYPRICE,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
		window=(wxRadioButton*)FindWindowById(ID_CONDITION_RADIOSELLPRICE,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
	}

	{
		wxComboBox* window=NULL;
		window=(wxComboBox*)FindWindowById(ID_CONDITION_COMBOCONDITION,this);
		strRet+="条件类型=[";
		strRet+=window->GetString(window->GetSelection());
		strRet+="],\t ";
	}

	{
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_CONDITION_TEXTPRICE,this);
		strRet+="触发价格=[";
		strRet+=window->GetLabel();
		strRet+="],\t ";
	}

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_CONDITION_CHECKTOSERVER,this);
		strRet+="是否在服务器上运行=[";
		if(window->GetValue()!=0)
			strRet+="是";
		else
			strRet+="否";
		strRet+="],\t ";
	}
	strRet+="\n";
	return strRet;

}