#include "StdAfx.h"
#include "COrderPositionPanel.h"


extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(COrderPositionPanel, wxPanel)
    EVT_SPIN_UP(ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET,OnPriceSpinDown)
    EVT_SPIN_UP(ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET,OnPriceSpinDown)
    EVT_SPIN_UP(ID_POSITION_SPINCTRLDOPRICEOFFSET,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_POSITION_SPINCTRLDOPRICEOFFSET,OnPriceSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderPositionPanel::COrderPositionPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_nViewMode(-1),
	m_fPriceTick(1.0f)
{

	wxPoint posInit(0, 0);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+8;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* pWin = NULL;

	
	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*24, nVH*10);
	
	posInit.x += nFW;
	posInit.y += nVH/2+2;
	
	
	// 这里不需要处理，
	// 语言问题会在ResetLanguage()里完成

	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*2, nFH);
	pWin = new wxStaticText( this, ID_POSITION_STATIC1, wxEmptyString, ctlPos, ctlSz);
	
	wxArrayString strArrCondi;
	strArrCondi.Add(wxEmptyString);
	strArrCondi.Add(wxEmptyString);
	ctlPos = wxPoint(posInit.x+nFW*2, posInit.y);
	ctlSz.Set(nFW*8, 20);
	wxComboBox* itemComboBox1 = new wxComboBox(this, ID_POSITION_COMBOPRICETYPE, wxEmptyString, 
			ctlPos, ctlSz, strArrCondi, wxCB_READONLY);
	itemComboBox1->SetSelection(0);

	ctlPos = wxPoint(posInit.x+nFW*2+nFW*8, posInit.y+2);
	ctlSz.Set(nFW*8, nFH);
	pWin = new wxStaticText( this, ID_POSITION_STATIC2, wxEmptyString, ctlPos, ctlSz);


	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*10, nFH);
	pWin = new wxCheckBox( this, ID_POSITION_CHECKSTOPLOSE, wxEmptyString, ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+nFW*10, posInit.y);
	ctlSz.Set(nFW*10, 20);
	pWin = new wxTextCtrl( this, ID_POSITION_TEXTSTOPLOSEPRICEOFFSET, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*20, posInit.y);
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton1 = new wxSpinButton( this, ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton1->SetRange(-1000000, 1000000);
    itemSpinButton1->SetValue(0);

	
	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*10, nFH);
	pWin = new wxCheckBox( this, ID_POSITION_CHECKSTOPGAIN, wxEmptyString, ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+nFW*10, posInit.y);
	ctlSz.Set(nFW*10, 20);
	pWin = new wxTextCtrl( this, ID_POSITION_TEXTSTOPGAINPRICEOFFSET, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*20, posInit.y);
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton2 = new wxSpinButton( this, ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton2->SetRange(-1000000, 1000000);
    itemSpinButton2->SetValue(0);


	//posInit.y += nVH+nVH/2;
	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x-nFW/2, posInit.y);
	ctlSz.Set(nFW*23, nVH*2);
	pWin = new wxStaticBox( this, ID_POSITION_STATIC3, wxEmptyString, ctlPos, ctlSz);

	
	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*10, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_POSITION_RADIONEWPRICE, wxEmptyString, ctlPos, ctlSz, wxRB_GROUP);
	itemRadioBox1->SetValue(true);

	ctlPos = wxPoint(posInit.x+nFW*10, posInit.y+2);
	ctlSz.Set(nFW*10, nFH);
	pWin = new wxRadioButton( this, ID_POSITION_RADIOBSPRICE, wxEmptyString, ctlPos, ctlSz);//, wxRB_GROUP);

	
//	posInit.y += nVH+nVH/2;
	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x-nFW/2, posInit.y);
	ctlSz.Set(nFW*23, nVH*3+4);
	pWin = new wxStaticBox( this, ID_POSITION_STATIC4, wxEmptyString, ctlPos, ctlSz);

	
	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*20, nFH);
	wxRadioButton* itemRadioBox3 = new wxRadioButton( this, ID_POSITION_RADIOTOPRICELIMIT, wxEmptyString, ctlPos, ctlSz, wxRB_GROUP);
	itemRadioBox3->SetValue(true);

	
	posInit.y += nVH;


	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*10, nFH);
	pWin = new wxRadioButton( this, ID_POSITION_RADIOTOPRICEOFFSET, wxEmptyString, ctlPos, ctlSz);//, wxRB_GROUP);

	ctlPos = wxPoint(posInit.x+nFW*10, posInit.y);
	ctlSz.Set(nFW*10, 20);
	pWin = new wxTextCtrl( this, ID_POSITION_TEXTDOPRICEOFFSET, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*20, posInit.y);
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton3 = new wxSpinButton( this, ID_POSITION_SPINCTRLDOPRICEOFFSET, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton3->SetRange(0, 1000000);
    itemSpinButton3->SetValue(0);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH+nVH/2);
	ctlSz.Set(nFW*16, nFH);
	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_POSITION_CHECKTOSERVER, wxEmptyString, ctlPos, ctlSz);
	if(DEFAULT_SVR()&&!DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_PROFIT_SERVER)) {
		itemCheckBox1->Show(false);
	}
	ResetLanguage();
}

COrderPositionPanel::~COrderPositionPanel()
{
}

COrderPositionPanel* COrderPositionPanel::CreatePane(wxWindow* parent)
{
    return new COrderPositionPanel(parent, GID_QUERY_ORDER_POSITION);
}

//
void COrderPositionPanel::SetViewMode(int nViewMode)
{
	m_nViewMode = nViewMode;
	wxWindow* window = NULL;
	for(int i=ID_OISTD_FIRST+1; i<SUMNUM_OISTD_ID; i++) {
		window = FindWindowById(i, this);
		if(window!=NULL) window->Enable(true);
	}
	switch(nViewMode) {
	case 0:		// 自由下单
		break;
	case 1:		// 盈损明细编辑模式
	{
		wxCheckBox* pCheck=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPLOSE, this);
		if(!pCheck->GetValue())
			pCheck->Enable(false);
		pCheck=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPGAIN, this);
		if(!pCheck->GetValue())
			pCheck->Enable(false);
		window=FindWindowById(ID_POSITION_CHECKTOSERVER, this);
		window->Hide();
	}
		break;
	};
}

void COrderPositionPanel::InitPositionPanel()
{
	SetOffsetPriceObject(1);
	SetLoseOffsetPrice(FALSE, 0.0f);
	SetGainOffsetPrice(FALSE, 0.0f);
	SetPriceType(1);
	SetCloseMode(1);
	SetCloseOffsetPrice(0.0f);
}

// 价格偏移对象，成交价/委托价
int COrderPositionPanel::GetOffsetPriceObject() 
{
	wxComboBox* window=NULL;
	window=(wxComboBox*)FindWindowById(ID_POSITION_COMBOPRICETYPE,this);
	return window->GetSelection()+1;
}

void COrderPositionPanel::SetOffsetPriceObject(int nPriceObject) 
{
	wxComboBox* window=NULL;
	window=(wxComboBox*)FindWindowById(ID_POSITION_COMBOPRICETYPE,this);
	return window->SetSelection(nPriceObject-1);
};

// 获得止损价格信息
BOOL COrderPositionPanel::GetLoseOffsetPrice(double& fOffsetPrice) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPLOSE,this);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPLOSEPRICEOFFSET,this);
	fOffsetPrice=atof(pText->GetLabel());
	return window->GetValue();
}

void COrderPositionPanel::SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice) 
{
	wxCheckBox* pCheck=NULL;
	pCheck=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPLOSE,this);
	pCheck->SetValue(bIsStopLose!=0);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPLOSEPRICEOFFSET,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	pText->SetLabel(strText);
}

// 获得止盈价格信息
BOOL COrderPositionPanel::GetGainOffsetPrice(double& fOffsetPrice) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPGAIN,this);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPGAINPRICEOFFSET,this);
	fOffsetPrice=atof(pText->GetLabel());
	return window->GetValue();
}

void COrderPositionPanel::SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice) 
{
	wxCheckBox* pCheck=NULL;
	pCheck=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPGAIN,this);
	pCheck->SetValue(bIsStopGain!=0);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPGAINPRICEOFFSET,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	pText->SetLabel(strText);
}

// 触发价格类型，1 最新价，2 买卖价
int COrderPositionPanel::GetPriceType() 
{
	wxRadioButton* window=NULL;
	window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIONEWPRICE,this);
	if(window->GetValue()) return 1;
	window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOBSPRICE,this);
	if(window->GetValue()) return 2;
	return -1;
}

void COrderPositionPanel::SetPriceType(int nPriceType) 
{
	wxRadioButton* window=NULL;
	switch(nPriceType) {
	case 1:
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIONEWPRICE,this);
		window->SetValue(1);
		break;
	case 2:
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOBSPRICE,this);
		window->SetValue(1);
		break;
	};
}

// 平仓方式，1 反向涨跌停价，2 反向加减
int COrderPositionPanel::GetCloseMode() 
{
	wxRadioButton* window=NULL;
	window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOTOPRICELIMIT,this);
	if(window->GetValue()) return 1;
	window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOTOPRICEOFFSET,this);
	if(window->GetValue()) return 2;
	return -1;
}

void COrderPositionPanel::SetCloseMode(int nCloseMode) 
{
	wxRadioButton* window=NULL;
	switch(nCloseMode) {
	case 1:
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOTOPRICELIMIT,this);
		window->SetValue(1);
		break;
	case 2:
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOTOPRICEOFFSET,this);
		window->SetValue(1);
		break;
	};
}

double COrderPositionPanel::GetCloseOffsetPrice() 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTDOPRICEOFFSET,this);
	return atof(window->GetLabel());
}

void COrderPositionPanel::SetCloseOffsetPrice(double fOffsetPrice) 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTDOPRICEOFFSET,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	window->SetLabel(strText);
}

// 设置是在本地执行还是服务器上执行
void COrderPositionPanel::SetIsRunInServer(BOOL bIsRunInServer) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKTOSERVER,this);
	if(window) window->SetValue(bIsRunInServer==TRUE);
}

BOOL COrderPositionPanel::GetIsRunInServer() 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKTOSERVER,this);
	if(window) return window->GetValue()!=0;
	return FALSE;
}

void COrderPositionPanel::OnPriceSpinUp(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	switch(event.GetId()) {
	case ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET:
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPLOSEPRICEOFFSET,this);
		break;
	case ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET:
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPGAINPRICEOFFSET,this);
		break;
	case ID_POSITION_SPINCTRLDOPRICEOFFSET:
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTDOPRICEOFFSET,this);
		break;
	default:
		return;
		break;
	};
	if(window==NULL) return;
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, (GetUpperLimitPrice()-GetLowerLimitPrice()), 0.0);
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void COrderPositionPanel::OnPriceSpinDown(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	switch(event.GetId()) {
	case ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET:
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPLOSEPRICEOFFSET,this);
		break;
	case ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET:
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPGAINPRICEOFFSET,this);
		break;
	case ID_POSITION_SPINCTRLDOPRICEOFFSET:
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTDOPRICEOFFSET,this);
		break;
	default:
		return;
		break;
	};
	if(window==NULL) return;
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, (GetUpperLimitPrice()-GetLowerLimitPrice()), 0.0);
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void COrderPositionPanel::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxComboBox* itemCombo = NULL;
		int thisID=evt.GetId();
		switch(thisID) {
		case ID_POSITION_TEXTSTOPLOSEPRICEOFFSET:
			evtSpin.SetId(ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_POSITION_TEXTSTOPGAINPRICEOFFSET:
			evtSpin.SetId(ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_POSITION_TEXTDOPRICEOFFSET:
			evtSpin.SetId(ID_POSITION_SPINCTRLDOPRICEOFFSET);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxComboBox* itemCombo = NULL;
		int thisID=evt.GetId();
		switch(thisID) {
		case ID_POSITION_TEXTSTOPLOSEPRICEOFFSET:
			evtSpin.SetId(ID_POSITION_SPINCTRLSTOPLOSEPRICEOFFSET);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_POSITION_TEXTSTOPGAINPRICEOFFSET:
			evtSpin.SetId(ID_POSITION_SPINCTRLSTOPGAINPRICEOFFSET);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_POSITION_TEXTDOPRICEOFFSET:
			evtSpin.SetId(ID_POSITION_SPINCTRLDOPRICEOFFSET);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		};
	}
	evt.Skip();

}

void COrderPositionPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_POSITION_STATIC1, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_ON));
	pWin = FindWindowById(ID_POSITION_STATIC2, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_SET_OFFSET));
	pWin = FindWindowById(ID_POSITION_STATIC3, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_TRIGGER_CONDITION_TYPE));
	pWin = FindWindowById(ID_POSITION_STATIC4, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_CLOSE_MODE));

	pWin = FindWindowById(ID_POSITION_CHECKSTOPLOSE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP_L));
	pWin = FindWindowById(ID_POSITION_CHECKSTOPGAIN, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP_P));

	pWin = FindWindowById(ID_POSITION_RADIONEWPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_LASTPRICE2));
	pWin = FindWindowById(ID_POSITION_RADIOBSPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_B_S_PRICE3));
	pWin = FindWindowById(ID_POSITION_RADIOTOPRICELIMIT, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_PRICE_LIMIT));
	pWin = FindWindowById(ID_POSITION_RADIOTOPRICEOFFSET, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_REVERSE));

	pWin = FindWindowById(ID_POSITION_CHECKTOSERVER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_RUN_IN_SERVER));

	wxComboBox* itemComboBox1 = (wxComboBox*)FindWindowById(ID_POSITION_COMBOPRICETYPE, this);
	itemComboBox1->SetString(0, LOADSTRING(OLS_TRADE_PRICE));
	itemComboBox1->SetString(1, LOADSTRING(OLS_ORDER_PRICE));
}

// 仅此用于日志文件输出
wxString COrderPositionPanel::GetUserInputText()
{
	wxString strRet;

	strRet = "盈损单数据：\n";

	// 价格偏移对象，成交价/委托价
	{
		wxComboBox* window=NULL;
		window=(wxComboBox*)FindWindowById(ID_POSITION_COMBOPRICETYPE,this);
		strRet+="价格类型=[";
		strRet+=window->GetString(window->GetSelection());
		strRet+="],\t ";
	}
	
	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPLOSE,this);
		if(window->GetValue()) {
			wxTextCtrl* pText=NULL;
			strRet+="选择止损，止损偏移=[";
			pText=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPLOSEPRICEOFFSET,this);
			strRet+=pText->GetLabel();
			strRet+="],\t ";
		}
		else {
			strRet+="未选择止损,\t ";
		}
	}

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKSTOPGAIN,this);
		if(window->GetValue()) {
			wxTextCtrl* pText=NULL;
			strRet+="选择止盈，止盈偏移=[";
			pText=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTSTOPGAINPRICEOFFSET,this);
			strRet+=pText->GetLabel();
			strRet+="],\t ";
		}
		else {
			strRet+="未选择止盈,\t ";
		}
	}

	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIONEWPRICE,this);
		if(window->GetValue()) {
			strRet+="价格类型=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOBSPRICE,this);
		if(window->GetValue()) {
			strRet+="价格类型=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}
	
	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOTOPRICELIMIT,this);
		if(window->GetValue()) {
			strRet+="平仓方式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
		window=(wxRadioButton*)FindWindowById(ID_POSITION_RADIOTOPRICEOFFSET,this);
		if(window->GetValue()) {
			strRet+="平仓方式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}

	{
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_POSITION_TEXTDOPRICEOFFSET,this);
		strRet+="反向加减=[";
		strRet+=window->GetLabel();
		strRet+="],\t ";
	}

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_POSITION_CHECKTOSERVER,this);
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