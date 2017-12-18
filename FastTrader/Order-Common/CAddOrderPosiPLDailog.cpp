#include "StdAfx.h"
#include "CAddOrderPosiPLDailog.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "wx\odcombo.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(CAddOrderPosiPLDailog, wxDialog)
    EVT_BUTTON(ID_ADDPOSIPL_OK,OnOk)
    EVT_BUTTON(ID_ADDPOSIPL_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
    EVT_TIMER(ID_QUOTTIMER, OnTimer)
	EVT_SPIN_UP(ID_ADDPOSIPL_NEWVOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_ADDPOSIPL_NEWVOLUME_SPINCTRL, OnVolumeSpinDown)
	EVT_SPIN_UP(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL,OnPriceSpinDown)
    EVT_SPIN_UP(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL,OnPriceSpinDown)
    EVT_SPIN_UP(ID_ADDPOSIPL_DOPRICEOFFSET_SPINCTRL,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_ADDPOSIPL_DOPRICEOFFSET_SPINCTRL,OnPriceSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CAddOrderPosiPLDailog::CAddOrderPosiPLDailog(
			 wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	m_poLocalOrderService(NULL),
    m_timer(this, ID_QUOTTIMER),
	m_bIsAbsCondPrice(FALSE), 
	m_bInitToday(FALSE), 
	m_fPriceTick(1.0),
	m_fUpperLimitPrice(0.0),
	m_fLowerLimitPrice(0.0)
{
	DIALOG_CREATE_LOG();

	m_nPosiCanCloseToday = 0;
	m_nPosiCanCloseYsday = 0;
	m_nPosiPLCloseToday = 0;
	m_nPosiPLCloseYsday = 0;
	m_nCondCloseToday = 0;
	m_nCondCloseYsday = 0;
	ZeroMemory(&m_tPLDetailOrder, sizeof(m_tPLDetailOrder));

	SetBackgroundColour(DEFAULT_COLOUR);
//	wxFont oldfont=GetFont();
//	oldfont.SetWeight(wxBOLD);
//	SetFont(oldfont);

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2+4+2;
	int nInputWidth = nFW*11;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL, *pWin = NULL;

	//posInit.x += nFW*3;
	//posInit.y += 24*8;

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*52, nVH*8);
	itemTemp = new wxStaticBox( this, ID_ADDPOSIPL_STATICBOX1, LOADSTRING(ID_ADDPOSIPL_STATICBOX1), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*1);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_POSITIONINFO_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//wxALIGN_CENTER_HORIZONTAL);//持仓基本信息

	{			//平今还是平昨
	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*2);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_CLOSEMODE_LABEL, LOADSTRING(ID_ADDPOSIPL_CLOSEMODE_LABEL), ctlPos, ctlSz, wxALIGN_RIGHT);//平仓模式

	wxArrayString strArrOC;
	strArrOC.Add(LOADSTRING(OLS_CLOSE_TODAY));
	strArrOC.Add(LOADSTRING(OLS_CLOSE));
	
	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*2);
	wxOwnerDrawnComboBox* pCmbOpenClose = new wxOwnerDrawnComboBox(this, ID_ADDPOSIPL_CLOSEMODE_COMBOBOX,LOADSTRING(OLS_OPEN), 
			ctlPos, wxSize(nInputWidth, 20), strArrOC, wxCB_READONLY);
	pCmbOpenClose->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CAddOrderPosiPLDailog::OnCmbOpenClose), NULL, this);
	pCmbOpenClose->SetSelection(1);
	}

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*3);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_QTY_LABEL, LOADSTRING(TITLE_QTY), ctlPos, ctlSz, wxALIGN_RIGHT);//数量: 

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*3);
	CreateFloatSpinCtrl_Volume(ctlPos);

	{			//基准价格
	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*4);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_PRICE_LABEL, LOADSTRING(ID_ADDPOSIPL_PRICE_LABEL), ctlPos, ctlSz, wxALIGN_RIGHT);//投保: 4

	wxArrayString strArrOC;
	strArrOC.Add(LOADSTRING(CFieldValueTool_LastPrice));
	strArrOC.Add(LOADSTRING(8104006));
	strArrOC.Add(LOADSTRING(8104007));
	
	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*4);
	wxOwnerDrawnComboBox* pCmbPriceType = new wxOwnerDrawnComboBox(this, ID_ADDPOSIPL_PRICE_COMBOBOX,LOADSTRING(OLS_OPEN), 
			ctlPos, wxSize(nInputWidth, 20), strArrOC, wxCB_READONLY);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth, posInit.y+nVH*4);
	ctlSz.Set(nFW*6, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_PRICE_LABEL2, LOADSTRING(ID_ADDPOSIPL_PRICE_LABEL2), ctlPos, ctlSz, wxALIGN_LEFT);//投保: 4
	}


	// 止盈价差
	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*5+2);
	ctlSz.Set(nFW*10, nFH);
	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_ADDPOSIPL_STOPGAIN_CHECK, LOADSTRING(OLS_STOP_P), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*5);
	ctlSz.Set(nInputWidth-18, 20);
	pWin = new wxTextCtrl( this, ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth-18, posInit.y+nVH*5);
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton1 = new wxSpinButton( this, ID_ADDPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton1->SetRange(-1000000, 1000000);
    itemSpinButton1->SetValue(0);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth, posInit.y+nVH*5);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_STOPGAINPRICE_LABEL, LOADSTRING(0.0), ctlPos, ctlSz, wxALIGN_LEFT|wxALIGN_BOTTOM);//平仓模式

	
	// 止损价差
	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*6+2);
	ctlSz.Set(nFW*10, nFH);
	wxCheckBox* itemCheckBox2 = new wxCheckBox( this, ID_ADDPOSIPL_STOPLOSE_CHECK, LOADSTRING(OLS_STOP_L), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*6);
	ctlSz.Set(nInputWidth-18, 20);
	pWin = new wxTextCtrl( this, ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth-18, posInit.y+nVH*6);
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton2 = new wxSpinButton( this, ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton2->SetRange(-1000000, 1000000);
    itemSpinButton2->SetValue(0);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth, posInit.y+nVH*6);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_STOPLOSEPRICE_LABEL, LOADSTRING(0.0), ctlPos, ctlSz, wxALIGN_LEFT|wxALIGN_BOTTOM);//平仓模式


	{			//触发条件价格
	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*7);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_CONDPRICE_LABEL, LOADSTRING(ID_ADDPOSIPL_CONDPRICE_LABEL), ctlPos, ctlSz, wxALIGN_RIGHT);

	wxArrayString strArrOC;
	//strArrOC.Add(LOADSTRING(CFieldValueTool_LastPrice));
	//strArrOC.Add(LOADSTRING(CFieldValueTool_BidPrice1));
	//strArrOC.Add(LOADSTRING(CFieldValueTool_AskPrice1));
	strArrOC.Add(LOADSTRING(CFieldValueTool_LastPrice));
	strArrOC.Add(LOADSTRING(OLS_B_S_PRICE3));
	
	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*7);
	wxOwnerDrawnComboBox* pCmbCondPrice = new wxOwnerDrawnComboBox(this, ID_ADDPOSIPL_CONDPRICETYPE_COMBOBOX, wxEmptyString, 
			ctlPos, wxSize(nInputWidth, 20), strArrOC, wxCB_READONLY);
	pCmbCondPrice->SetSelection(0);
	
	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth, posInit.y+nVH*7);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_TARGETPRICE_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);
	}

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*28, posInit.y+nVH*3);
	ctlSz.Set(nFW*20, nFH*4);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_POSIVOLUME_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//买卖: 3


	ctlPos = wxPoint(posInit.x, posInit.y+nVH*8+4);
	ctlSz.Set(nFW*52, nVH*4-4);
	pWin = new wxStaticBox( this, ID_ADDPOSIPL_STATICBOX2, LOADSTRING(OLS_CLOSE_MODE), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*9);
	ctlSz.Set(nFW*48, nFH);
	itemTemp = new wxStaticText(this, ID_ADDPOSIPL_DETAIL2_LABEL, LOADSTRING(ID_ADDPOSIPL_DETAIL2_LABEL), ctlPos, ctlSz, wxALIGN_LEFT|wxALIGN_BOTTOM);//平仓模式

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*10+2);
	ctlSz.Set(nFW*10, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_ADDPOSIPL_DOPRICEOFFSET_RADIO, LOADSTRING(OLS_REVERSE), ctlPos, ctlSz, wxRB_GROUP);
	itemRadioBox1->SetValue(true);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*10);
	ctlSz.Set(nInputWidth-18, 20);
	pWin = new wxTextCtrl( this, ID_ADDPOSIPL_DOPRICEOFFSET_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth-18, posInit.y+nVH*10);
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton3 = new wxSpinButton( this, ID_ADDPOSIPL_DOPRICEOFFSET_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton3->SetRange(0, 1000000);
    itemSpinButton3->SetValue(0);


	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*11+2);
	ctlSz.Set(nFW*20, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton( this, ID_ADDPOSIPL_DOPRICELIMIT_RADIO, LOADSTRING(OLS_PRICE_LIMIT), ctlPos, ctlSz);
	itemRadioBox2->SetValue(false);

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*12+6+2);
	ctlSz.Set(nFW*16, nFH);
	wxCheckBox* itemCheckBox3 = new wxCheckBox( this, ID_ADDPOSIPL_TOSERVER_CHECK, LOADSTRING(OLS_RUN_IN_SERVER), ctlPos, ctlSz);


	posInit.x += nFW*36;
	posInit.y += nVH*12+14;

	ctlPos = wxPoint(posInit.x, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ADDPOSIPL_OK, LOADSTRING(ASKNAMEDLG_OK), ctlPos, ctlSz, 0 );//确定

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ADDPOSIPL_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), ctlPos, ctlSz, 0 );//取消

	SetSize(wxSize(nFW*54, 24*15));

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_ADDPOSIPL_CLOSEMODE_COMBOBOX);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_NEWVOLUME_TEXT);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_PRICE_COMBOBOX);

	m_TabOrderVector.push_back(ID_ADDPOSIPL_STOPLOSE_CHECK);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_STOPGAIN_CHECK);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT);

	m_TabOrderVector.push_back(ID_ADDPOSIPL_CONDPRICETYPE_COMBOBOX);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_DOPRICEOFFSET_RADIO);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_DOPRICEOFFSET_TEXT);
	m_TabOrderVector.push_back(ID_ADDPOSIPL_DOPRICELIMIT_RADIO);

	m_TabOrderVector.push_back(ID_ADDPOSIPL_OK);
    m_TabOrderVector.push_back(ID_ADDPOSIPL_CANCEL);

	m_poLocalOrderService = CLocalOrderService::GetInstance();
	m_poLocalOrderService->RegisterMsgWindow(this);

}

CAddOrderPosiPLDailog::~CAddOrderPosiPLDailog()
{
	m_timer.Stop();
	m_poLocalOrderService->UnregisterMsgWindow(this);
	m_poLocalOrderService = NULL;
}

wxTextCtrl* CAddOrderPosiPLDailog::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(8*11-18, 20);
    wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_ADDPOSIPL_NEWVOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);

//	itemTextCtrl3->Connect(ID_OIJSD_VOLUME_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnVolumeClicked), NULL, this);

	ctlPos.x += 8*11-18;
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_ADDPOSIPL_NEWVOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

// 设置开平仓模式，0 开仓，1 平今，2 平仓
void CAddOrderPosiPLDailog::SetOpenClose(int nOCMode)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_CLOSEMODE_COMBOBOX,this);
	if(pComboBox->IsEnabled())
		pComboBox->SetSelection(nOCMode-1);
}

// 获得开平仓模式，0 开仓，1 平今，2 平仓
int CAddOrderPosiPLDailog::GetOpenClose()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_CLOSEMODE_COMBOBOX,this);
	return pComboBox->GetSelection()+1;
}

// 0 最新，1 持仓均价，2 开仓均价
void CAddOrderPosiPLDailog::SetPriceMode(int nPriceMode)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_PRICE_COMBOBOX,this);
	if(pComboBox->IsEnabled())
		pComboBox->SetSelection(nPriceMode);
}

// 0 最新，1 持仓均价，2 开仓均价
int CAddOrderPosiPLDailog::GetPriceMode()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_PRICE_COMBOBOX,this);
	return pComboBox->GetSelection();
}

void CAddOrderPosiPLDailog::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_ADDPOSIPL_NEWVOLUME_TEXT,this);
	window->SetLabel(strText);
}

int CAddOrderPosiPLDailog::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_ADDPOSIPL_NEWVOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

// 获得止损价格信息
BOOL CAddOrderPosiPLDailog::GetLoseOffsetPrice(double& fOffsetPrice) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_STOPLOSE_CHECK,this);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
	fOffsetPrice=atof(pText->GetLabel());
	return window->GetValue();
}

void CAddOrderPosiPLDailog::SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice) 
{
	wxCheckBox* pCheck=NULL;
	pCheck=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_STOPLOSE_CHECK,this);
	pCheck->SetValue(bIsStopLose!=0);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	pText->SetLabel(strText);
}

// 获得止盈价格信息
BOOL CAddOrderPosiPLDailog::GetGainOffsetPrice(double& fOffsetPrice) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_STOPGAIN_CHECK,this);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
	fOffsetPrice=atof(pText->GetLabel());
	return window->GetValue();
}

void CAddOrderPosiPLDailog::SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice) 
{
	wxCheckBox* pCheck=NULL;
	pCheck=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_STOPGAIN_CHECK,this);
	pCheck->SetValue(bIsStopGain!=0);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	pText->SetLabel(strText);
}

// 触发价格类型，1 最新价，2 买卖价，3 买一价，4 卖一价
int CAddOrderPosiPLDailog::GetCondPriceType() 
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_CONDPRICETYPE_COMBOBOX,this);
	//if(pComboBox->GetSelection()==0)
	//	return pComboBox->GetSelection()+1;
	//else
	//	return pComboBox->GetSelection()+2;
	return pComboBox->GetSelection()+1;
}

void CAddOrderPosiPLDailog::SetCondPriceType(int nPriceType) 
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_CONDPRICETYPE_COMBOBOX,this);
	//if(pComboBox->IsEnabled())
	//	if(nPriceType==1)
	//		pComboBox->SetSelection(nPriceType-1);
	//	else
	//		pComboBox->SetSelection(nPriceType-2);
	if(pComboBox->IsEnabled())
		pComboBox->SetSelection(nPriceType-1);
}

// 平仓方式，1 反向涨跌停价，2 反向加减
int CAddOrderPosiPLDailog::GetCloseMode() 
{
	wxRadioButton* window=NULL;
	window=(wxRadioButton*)FindWindowById(ID_ADDPOSIPL_DOPRICELIMIT_RADIO,this);
	if(window->GetValue()) return 1;
	window=(wxRadioButton*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_RADIO,this);
	if(window->GetValue()) return 2;
	return -1;
}

void CAddOrderPosiPLDailog::SetCloseMode(int nCloseMode) 
{
	wxRadioButton* window=NULL;
	switch(nCloseMode) {
	case 1:
		window=(wxRadioButton*)FindWindowById(ID_ADDPOSIPL_DOPRICELIMIT_RADIO,this);
		window->SetValue(1);
		break;
	case 2:
		window=(wxRadioButton*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_RADIO,this);
		window->SetValue(1);
		break;
	};
}

double CAddOrderPosiPLDailog::GetCloseOffsetPrice() 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_TEXT,this);
	return atof(window->GetLabel());
}

void CAddOrderPosiPLDailog::SetCloseOffsetPrice(double fOffsetPrice) 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_TEXT,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	window->SetLabel(strText);
}

// 设置是在本地执行还是服务器上执行
void CAddOrderPosiPLDailog::SetIsRunInServer(BOOL bIsRunInServer) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_TOSERVER_CHECK,this);
	if(window) window->SetValue(bIsRunInServer==TRUE);
}

BOOL CAddOrderPosiPLDailog::GetIsRunInServer() 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_TOSERVER_CHECK,this);
	if(window) return window->GetValue()!=0;
	return FALSE;
}

void CAddOrderPosiPLDailog::CalcVolume()
{
	m_nPosiCanCloseToday = m_positionInfo.CanCloseTodayVolume;
	m_nPosiCanCloseYsday = m_positionInfo.CanCloseydVolume;
	
	CLocalOrderService::GetInstance()->LockObject();

	m_nPosiPLCloseToday = CLocalOrderService::GetInstance()->GetPosiPLVolume(
			std::string(m_positionInfo.InvestorID), 
			std::string(m_positionInfo.InstrumentID), 
			m_positionInfo.PosiDirection==THOST_FTDC_PD_Long?FALSE:TRUE,
			m_positionInfo.HedgeFlag==THOST_FTDC_HF_Hedge?TRUE:FALSE, 1);

	m_nPosiPLCloseYsday = CLocalOrderService::GetInstance()->GetPosiPLVolume(
				std::string(m_positionInfo.InvestorID), 
				std::string(m_positionInfo.InstrumentID), 
				m_positionInfo.PosiDirection==THOST_FTDC_PD_Long?FALSE:TRUE,
				m_positionInfo.HedgeFlag==THOST_FTDC_HF_Hedge?TRUE:FALSE, 2);

	CLocalOrderService::GetInstance()->UnlockObject();
	//m_nCondCloseToday;
	//m_nCondCloseYsday;
}

int CAddOrderPosiPLDailog::CalcCanPLVolume(BOOL bIsToday)
{
	int nVol = 0;
	if(bIsToday) {
		nVol = m_nPosiCanCloseToday - (m_nPosiPLCloseToday + m_nCondCloseToday);
	}
	else {
		nVol = m_nPosiCanCloseYsday - (m_nPosiPLCloseYsday + m_nCondCloseYsday);
	}
	if(nVol<0)
		nVol = 0;
	return nVol;
}

int CAddOrderPosiPLDailog::GetCondVolume(BOOL bIsServ, 
					CLocalOrderService::ORDERREF& condRef, string& strInstruemtID)
{
	if(m_poLocalOrderService==NULL || DEFAULT_SVR()==NULL) 
		return -1;
	
	int nCondVol = 0;
	if(bIsServ) {
		PlatformStru_OrderInfo outData;
		OrderKey orderkey(condRef.Account, strInstruemtID.c_str(), 
					condRef.FrontID, condRef.SessionID, condRef.OrderRef);
		if(!DEFAULT_SVR()->GetOrder(orderkey, outData)) 
			return -1;
		nCondVol = outData.VolumeTotalOriginal;
	}
	else {
		CLocalOrderService::MAPCONDITIONORDERITEM it;
		m_poLocalOrderService->LockObject();
		CLocalOrderService::MAPCONDITIONORDER& mapCond = 
				m_poLocalOrderService->GetOrderConditionMap();
		it = mapCond.find(condRef.FTID);
		if(it != mapCond.end()) {
			nCondVol = it->second.orderSrc.nVolume;
		}
		m_poLocalOrderService->UnlockObject();
	}

	return nCondVol;
}

int CAddOrderPosiPLDailog::CalcPLCondVolume(CLocalOrderService::PLDETAILORDER& posiPLOrder)
{
	if(m_poLocalOrderService==NULL) 
		return -1;
	
	int nVol = 0, nGainVol = 0, nLoseVol = 0;
	nGainVol = GetCondVolume(posiPLOrder.bGainIsServ, posiPLOrder.refGain, 
							std::string(posiPLOrder.orderSrc.InstrumentID));
	nLoseVol = GetCondVolume(posiPLOrder.bLoseIsServ, posiPLOrder.refLose, 
							std::string(posiPLOrder.orderSrc.InstrumentID));
	nVol = max(nGainVol, nLoseVol);
	if(nVol<0)
		nVol = 0;
	return nVol;
}

wxString CAddOrderPosiPLDailog::GetCanCloseMsg(BOOL bIsToday)
{
	wxString strMsg;

	//strMsg.Printf(LOADFORMATSTRING(PosiPLADD_CanCloseMsg, "%s%d%s%d%s%d%s%d"), 
	//		(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), (bIsToday?m_nPosiCanCloseToday:m_nPosiCanCloseYsday), 
	//		(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), (bIsToday?m_nPosiPLCloseToday:m_nPosiPLCloseYsday), 
	//		(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), (bIsToday?m_nCondCloseToday:m_nCondCloseYsday), 
	//		(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), CalcCanPLVolume(bIsToday));
	strMsg.Printf(LOADFORMATSTRING(PosiPLADD_CanCloseMsg, "%s%d%s%d%s%d"), 
			(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), (bIsToday?m_nPosiCanCloseToday:m_nPosiCanCloseYsday), 
			(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), (bIsToday?m_nPosiPLCloseToday:m_nPosiPLCloseYsday), 
			(bIsToday?LOADSTRING(CFieldValueTool_CloseToday):LOADSTRING(CFieldValueTool_CloseYesterday)), CalcCanPLVolume(bIsToday));

	return strMsg;
}

wxString CAddOrderPosiPLDailog::GetStaticText(UINT nUIID)
{
	wxString strText;
    wxWindow* pWin=FindWindowById(nUIID, this);
	if(pWin!=NULL)
		strText = pWin->GetLabel();
	return strText;
}

// 仅此用于日志文件输出
wxString CAddOrderPosiPLDailog::GetUserInputText()
{
	wxString strRet;

	strRet = "止盈止损参数：\n";

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_STOPLOSE_CHECK,this);
		if(window->GetValue()) {
			wxTextCtrl* pText=NULL;
			strRet+="选择止损，止损偏移=[";
			pText=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
			strRet+=pText->GetLabel();
			strRet+="],\t ";
		}
		else {
			strRet+="未选择止损,\t ";
		}
	}

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_STOPGAIN_CHECK,this);
		if(window->GetValue()) {
			wxTextCtrl* pText=NULL;
			strRet+="选择止盈，止盈偏移=[";
			pText=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
			strRet+=pText->GetLabel();
			strRet+="],\t ";
		}
		else {
			strRet+="未选择止盈,\t ";
		}
	}

	{
		wxOwnerDrawnComboBox* window=NULL;
		window=(wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_CONDPRICETYPE_COMBOBOX,this);
		if(window->GetValue()) {
			strRet+="价格类型=[";
			strRet+=window->GetString(window->GetSelection());
			strRet+="],\t ";
		}
	}
	
	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_ADDPOSIPL_DOPRICELIMIT_RADIO,this);
		if(window->GetValue()) {
			strRet+="平仓方式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
		window=(wxRadioButton*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_RADIO,this);
		if(window->GetValue()) {
			strRet+="平仓方式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}

	{
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_TEXT,this);
		strRet+="反向加减=[";
		strRet+=window->GetLabel();
		strRet+="],\t ";
	}

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_ADDPOSIPL_TOSERVER_CHECK,this);
		strRet+="是否在服务器上运行=[";
		if(window->GetValue()!=0)
			strRet+="是";
		else
			strRet+="否";
		strRet+="],\t ";
	}

	strRet+="\r\n";
	return strRet;

}

wxString CAddOrderPosiPLDailog::GetReqLog()
{
	wxString strText;
	strText = "[";
	strText += GetStaticText(ID_ADDPOSIPL_POSITIONINFO_LABEL);
	strText+="],\t ";
	{
		//wxWindow* window=NULL;
		//window=FindWindowById(ID_ADDPOSIPL_CLOSEMODE_COMBOBOX,this);
		//if(window) {
		//	strText+="平仓类型=[";
		//	strText+=window->GetLabel();
		//	strText+="],\t ";
		//}
		wxString strItem;
		strItem.Printf("平仓类型=[%d],\t ", GetOpenClose());
		strText+=strItem;
	}
	{
		wxWindow* window=NULL;
		window=FindWindowById(ID_ADDPOSIPL_NEWVOLUME_TEXT,this);
		if(window) {
			strText+="手数=[";
			strText+=window->GetLabel();
			strText+="],\t ";
		}
	}
	{
		//wxWindow* window=NULL;
		//window=FindWindowById(ID_ADDPOSIPL_PRICE_COMBOBOX,this);
		//if(window) {
		//	strText+="基准价格=[";
		//	strText+=window->GetLabel();
		//	strText+="],\t ";
		//}
		wxString strItem;
		strItem.Printf("基准价格=[%d],\t \r\n", GetPriceMode());
		strText+=strItem;
	}
	strText += GetUserInputText();
	return strText;
}

BOOL CAddOrderPosiPLDailog::Init(BOOL bIsAbsCond)
{
	m_bIsAbsCondPrice = bIsAbsCond;

    PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(
			std::string(m_positionInfo.InstrumentID),insInfo)!=0)
		return FALSE;
	
	SetPriceTick(insInfo.PriceTick);
	
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()
			&&DEFAULT_SVR()->GetQuotInfo(m_positionInfo.InstrumentID,field)!=0) {
    }
	SetUpperLimitPrice(field.UpperLimitPrice);
	SetLowerLimitPrice(field.LowerLimitPrice);
	
	CalcVolume();

	m_bInitToday = CalcCanPLVolume(TRUE)>0;

	ShowOrderReq(m_bInitToday);
	SetQty(CalcCanPLVolume(m_bInitToday));
	if(m_bInitToday)
		SetOpenClose(1);
	else
		SetOpenClose(2);

	SetGainOffsetPrice(TRUE, 0.0);
	SetLoseOffsetPrice(TRUE, 0.0);
	SetCloseOffsetPrice(0.0);

	return TRUE;
}

void CAddOrderPosiPLDailog::ShowOrderReq(BOOL bIsToday)
{
	wxWindow* pwinItem=NULL;
	wxString strValue;

	pwinItem=FindWindowById(ID_ADDPOSIPL_POSITIONINFO_LABEL, this);
	strValue.Printf(LOADFORMATSTRING(ID_ADDPOSIPL_POSITIONINFO_LABEL, "%s%s%s"), //文本必须包含%s
			m_positionInfo.InstrumentID, 
			m_positionInfo.PosiDirection==THOST_FTDC_PD_Long ? LOADSTRING(OLS_BUY) : LOADSTRING(OLS_SELL),
			m_positionInfo.HedgeFlag==THOST_FTDC_BHF_Speculation ? 
					LOADSTRING(CFieldValueTool_Speculation) : LOADSTRING(CFieldValueTool_Hedge));
	if(pwinItem) pwinItem->SetLabel(strValue);

	wxOwnerDrawnComboBox* pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_CLOSEMODE_COMBOBOX, this);
	if(pCmb!=NULL) 
		pCmb->SetSelection(bIsToday?0:1);

	pCmb = (wxOwnerDrawnComboBox*)FindWindowById(ID_ADDPOSIPL_PRICE_COMBOBOX, this);
	if(pCmb) 
		pCmb->SetSelection(0);

	pwinItem=FindWindowById(ID_ADDPOSIPL_POSIVOLUME_LABEL, this);
	strValue.Printf("%s", GetCanCloseMsg(bIsToday));
	if(pwinItem) pwinItem->SetLabel(strValue);

    wxTextCtrl* pTextCtrl = (wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_NEWVOLUME_TEXT, this);
	strValue.Printf("%d", CalcPLCondVolume(m_tPLDetailOrder));
	if(pTextCtrl) pTextCtrl->SetLabel(strValue);

}

void CAddOrderPosiPLDailog::ShowOrderPLDetail()
{
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()
			&&DEFAULT_SVR()->GetQuotInfo(m_tPLDetailOrder.InstrumentID,field)!=0) {
    }
	SetUpperLimitPrice(field.UpperLimitPrice);
	SetLowerLimitPrice(field.LowerLimitPrice);

	SetIsRunInServer(m_tPLDetailOrder.bIsRunInServer);
	SetPriceTick(m_fPriceTick);
	SetLoseOffsetPrice(
			m_tPLDetailOrder.plparam.bDoStopLose, m_tPLDetailOrder.plparam.fStopLosePriceOffset);
	SetGainOffsetPrice(
			m_tPLDetailOrder.plparam.bDoStopGain, m_tPLDetailOrder.plparam.fStopGainPriceOffset);
	SetCondPriceType(m_tPLDetailOrder.plparam.nPriceType);
	SetCloseMode(m_tPLDetailOrder.plparam.nCloseMode);
	SetCloseOffsetPrice(m_tPLDetailOrder.plparam.fClosePriceOffset);
}

void CAddOrderPosiPLDailog::ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow)
{
	TRADEINFODLG(this)->ShowTradeMsg(errormessage, -1, Title, -1, bIsShow);
}

bool CAddOrderPosiPLDailog::Show( bool show )
{
	if(show) {
		m_timer.Start(500);
	}
	else {
		m_timer.Stop();
	}
    return wxDialog::Show(show);
}

void CAddOrderPosiPLDailog::OnOk(wxCommandEvent& event)
{
    if(GetQty()<=0)
    {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR), LOADSTRING(OIP_QTYBIGZERO),TRUE);
        return;
    }
    if(GetQty()>CalcCanPLVolume(GetOpenClose()==1))
    {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR), LOADSTRING(PosiPLADD_QtyOverMsg),TRUE);
        return;
    }

	wxString name;
	std::string strAccount;
	name = m_positionInfo.InstrumentID;
	strAccount = m_positionInfo.InvestorID;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)!=0) {
		ShowTradeInfoDlg(LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),LOADSTRING(PosiPLADD_NoQuotNoBasisPrice), TRUE);
		return;
    }
	double fPrice = util::GetDoubleInvalidValue();
	int nPriceMode = GetPriceMode();
	switch(nPriceMode) {
	case 0:
		fPrice = (util::isInvalidValue(field.LastPrice)?field.PreSettlementPrice:field.LastPrice);
		break;
	case 1:
		fPrice = m_positionInfo.AveragePositionPrice;
		break;
	case 2:
		fPrice = m_positionInfo.AverageOpenPrice;
		break;
	}
	CheckSumPrice(fPrice, m_fPriceTick);
	if(util::isInvalidValue(fPrice)) {
		ShowTradeInfoDlg(LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),LOADSTRING(PosiPLADD_PriceIsInvilad), TRUE);
		return;
	}

	BOOL bIsCheckLose = FALSE, bIsCheckGain = FALSE;
	double fLoseOffsetPrice = 0.0f, fGainOffsetPrice = 0.0f;
	int nCloseMode = 0;
	double fCloseOffsetPrice = 0.0;

	bIsCheckLose = GetLoseOffsetPrice(fLoseOffsetPrice);
	bIsCheckGain = GetGainOffsetPrice(fGainOffsetPrice);
	nCloseMode = GetCloseMode();
	fCloseOffsetPrice = GetCloseOffsetPrice();

	if(bIsCheckLose == FALSE && bIsCheckGain == FALSE) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_SOPPO), TRUE);
		return;
	}
	if(bIsCheckLose != FALSE && fLoseOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_STEOPPRCBIGZERO), TRUE);
		return;
	}
	if(bIsCheckGain != FALSE && fGainOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_PRCDELTABIGZERO), TRUE);
		return;
	}
	if(CheckSumPrice(fLoseOffsetPrice, m_fPriceTick)) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(CHECKSUM_PRICE_LOSE), TRUE);
		return;
	}
	if(CheckSumPrice(fGainOffsetPrice, m_fPriceTick)) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(CHECKSUM_PRICE_GAIN), TRUE);
		return;
	}
	if(CheckSumPrice(fCloseOffsetPrice, m_fPriceTick)) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(CHECKSUM_PRICE_CLOSEREVERSE), TRUE);
		return;
	}
	
	BOOL bIsBuy = (m_positionInfo.PosiDirection==THOST_FTDC_PD_Long ? TRUE : FALSE);

	if(bIsCheckLose && (
			(bIsBuy && util::lessOrEqual(fPrice-fLoseOffsetPrice, field.LowerLimitPrice))
			|| 
			(!bIsBuy && util::greaterOrEqual(fPrice+fLoseOffsetPrice, field.UpperLimitPrice))
			)
			) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(PosiPLADD_HasLoseOverMsg), TRUE);
		return;
	}
	if(bIsCheckGain && (
			(bIsBuy && util::greaterOrEqual(fPrice+fGainOffsetPrice, field.UpperLimitPrice))
			|| 
			(!bIsBuy && util::lessOrEqual(fPrice-fGainOffsetPrice, field.LowerLimitPrice))
			)
			) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(PosiPLADD_HasGainOverMsg), TRUE);
		return;
	}

	BOOL bIsSuccess = FALSE;

	double fBasePrice = 0.0;
	switch(GetCondPriceType()) {
	case 1:
		fBasePrice = field.LastPrice;
		break;
	case 2:
		if(!bIsBuy)
			fBasePrice = field.AskPrice1;
		else
			fBasePrice = field.BidPrice1;
		break;
	case 3:
		fBasePrice = field.BidPrice1;
		break;
	case 4:
		fBasePrice = field.AskPrice1;
		break;
	};
	if(!util::isInvalidValue(fBasePrice)) {
		if(bIsCheckLose && (
				(bIsBuy && util::greaterOrEqual(fPrice-fLoseOffsetPrice, fBasePrice))
				|| 
				(!bIsBuy && util::lessOrEqual(fPrice+fLoseOffsetPrice, fBasePrice))
				)
				)
			bIsSuccess = TRUE;
		if(bIsCheckGain && (
				(bIsBuy && util::lessOrEqual(fPrice+fGainOffsetPrice, fBasePrice))
				|| 
				(!bIsBuy && util::greaterOrEqual(fPrice-fGainOffsetPrice, fBasePrice))
				)
				)
			bIsSuccess = TRUE;
	}
	if(bIsSuccess) {
		wxString strText;
		strText = LOADSTRING(PosiPLADD_HasTriggerMsg);
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),strText, TRUE);
		return;
	}
	int nOCMode = GetOpenClose();
	int nVol = GetQty();

	MakePosiPL(strAccount, std::string(name.c_str()), !bIsBuy, nOCMode, fPrice, nVol, 
			m_tPLDetailOrder);

	USERLOG_INFO("用户新增持仓止盈止损，对话框中单击[确认]确认\n%s", 
			GetReqLog().c_str());

	EndModal(wxID_OK);
}

void CAddOrderPosiPLDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CAddOrderPosiPLDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CAddOrderPosiPLDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CAddOrderPosiPLDailog::OnDialogCharHook(wxKeyEvent& evt)
{
    
	wxKeyEvent* pEvent=&evt;//(wxKeyEvent*)evt.GetClientData();
    if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
		evt.Skip();
        return;
    }
	pEvent->SetId(win->GetId());
    if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB)
    {
        bool backward = pEvent->ShiftDown();
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(backward)
                    id=id>0?id-1:count-1;
                else
                    id=id<count-1?id+1:0;
                FindWindowById(m_TabOrderVector[id],this)->SetFocus();
                return;
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_RETURN || pEvent->GetKeyCode()==VK_RETURN)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
                    return;
                }
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id>0)
                {
                    FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
                   return;
                }
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
                    return;
                }
            }
        }
    }
	else {
		evt.Skip();
	}

}

void CAddOrderPosiPLDailog::OnTimer(wxTimerEvent& event)
{
	wxString name(m_positionInfo.InstrumentID);
	double fPriceTick = m_fPriceTick;
    PlatformStru_DepthMarketData field;

	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	BOOL bIsBuy = (m_positionInfo.PosiDirection==THOST_FTDC_PD_Long ? TRUE : FALSE);

	BOOL bHasQuot = FALSE;
	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)==0) {
		bHasQuot = TRUE;
    }

	wxString strText;
	double fPrice = 0.0, fOffsetPrice = 0.0;
	wxWindow* pWin = FindWindowById(ID_ADDPOSIPL_STOPGAINPRICE_LABEL, this);
	if(pWin != NULL) {
		strText = "-";
		if(bHasQuot && GetGainOffsetPrice(fOffsetPrice)) {
			switch(GetPriceMode()) {
			case 0:
				if(!util::isInvalidValue(field.LastPrice)) {
					if(bIsBuy)
						fPrice = field.LastPrice+fOffsetPrice;
					else
						fPrice = field.LastPrice-fOffsetPrice;
					strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
				}
				break;
			case 1:
				if(!util::isInvalidValue(m_positionInfo.AveragePositionPrice)) {
					if(bIsBuy)
						fPrice = m_positionInfo.AveragePositionPrice+fOffsetPrice;
					else
						fPrice = m_positionInfo.AveragePositionPrice-fOffsetPrice;
					CheckSumPrice(fPrice, m_fPriceTick);
					strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
				}
				break;
			case 2:
				if(!util::isInvalidValue(m_positionInfo.AverageOpenPrice)) {
					if(bIsBuy)
						fPrice = m_positionInfo.AverageOpenPrice+fOffsetPrice;
					else
						fPrice = m_positionInfo.AverageOpenPrice-fOffsetPrice;
					CheckSumPrice(fPrice, m_fPriceTick);
					strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
				}
				break;
			};
		}
		pWin->SetLabel(strText);
	}
	pWin = FindWindowById(ID_ADDPOSIPL_STOPLOSEPRICE_LABEL, this);
	if(pWin != NULL) {
		strText = "-";
		if(bHasQuot && GetLoseOffsetPrice(fOffsetPrice)) {
			switch(GetPriceMode()) {
			case 0:
				if(!util::isInvalidValue(field.LastPrice)) {
					if(!bIsBuy)
						fPrice = field.LastPrice+fOffsetPrice;
					else
						fPrice = field.LastPrice-fOffsetPrice;
					strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
				}
				break;
			case 1:
				if(!util::isInvalidValue(m_positionInfo.AveragePositionPrice)) {
					if(!bIsBuy)
						fPrice = m_positionInfo.AveragePositionPrice+fOffsetPrice;
					else
						fPrice = m_positionInfo.AveragePositionPrice-fOffsetPrice;
					CheckSumPrice(fPrice, m_fPriceTick);
					strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
				}
				break;
			case 2:
				if(!util::isInvalidValue(m_positionInfo.AverageOpenPrice)) {
					if(!bIsBuy)
						fPrice = m_positionInfo.AverageOpenPrice+fOffsetPrice;
					else
						fPrice = m_positionInfo.AverageOpenPrice-fOffsetPrice;
					CheckSumPrice(fPrice, m_fPriceTick);
					strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
				}
				break;
			};
		}
		pWin->SetLabel(strText);
	}
	
	pWin = FindWindowById(ID_ADDPOSIPL_TARGETPRICE_LABEL, this);
	if(pWin != NULL) {
		strText = "-";
		if(bHasQuot) {
			switch(GetCondPriceType()) {
				case 1:
					fPrice = field.LastPrice;
					break;
				case 2:
					if(!bIsBuy)
						fPrice = field.AskPrice1;
					else
						fPrice = field.BidPrice1;
					break;
				case 3:
					fPrice = field.BidPrice1;
					break;
				case 4:
					fPrice = field.AskPrice1;
					break;
			};
			if(!util::isInvalidValue(fPrice)) 
				strText.Printf("%s", Price2String(fPrice, m_fPriceTick).c_str());
		}
		pWin->SetLabel(strText);
	}
}

void CAddOrderPosiPLDailog::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999 && nVolume<CalcCanPLVolume(GetOpenClose()==1))
		nVolume++;
	SetQty(nVolume);
}

void CAddOrderPosiPLDailog::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void CAddOrderPosiPLDailog::OnPriceSpinUp(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	switch(event.GetId()) {
	case ID_ADDPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
		break;
	case ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
		break;
	case ID_ADDPOSIPL_DOPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_TEXT,this);
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

void CAddOrderPosiPLDailog::OnPriceSpinDown(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	switch(event.GetId()) {
	case ID_ADDPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
		break;
	case ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
		break;
	case ID_ADDPOSIPL_DOPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_ADDPOSIPL_DOPRICEOFFSET_TEXT,this);
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

void CAddOrderPosiPLDailog::OnCmbOpenClose(wxCommandEvent& evt)
{
	wxString strValue;
	wxWindow* pwinItem=FindWindowById(ID_ADDPOSIPL_POSIVOLUME_LABEL, this);
	strValue.Printf("%s", GetCanCloseMsg(GetOpenClose()==1));
	if(pwinItem) pwinItem->SetLabel(strValue);
	SetQty(CalcCanPLVolume(GetOpenClose()==1));
}

BOOL CAddOrderPosiPLDailog::MakePosiPL(std::string& strAccount, std::string& strInstrumentID,
										BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, 
										CLocalOrderService::PLDETAILORDER& tPLDetail)
{
	ZeroMemory(&tPLDetail, sizeof(tPLDetail));
	
	tPLDetail.bIsRunInServer = GetIsRunInServer();
	tPLDetail.bGainIsServ = tPLDetail.bIsRunInServer;
	tPLDetail.bLoseIsServ = tPLDetail.bIsRunInServer;
	tPLDetail.nState = conPLDetailActive;
	strcpy(tPLDetail.InstrumentID, strInstrumentID.c_str());

	tPLDetail.plparam.nPriceOffsetType = 0;//GetOffsetPriceObject();
	tPLDetail.plparam.bDoStopLose = GetLoseOffsetPrice(
			tPLDetail.plparam.fStopLosePriceOffset);
	tPLDetail.plparam.bDoStopGain = GetGainOffsetPrice(
			tPLDetail.plparam.fStopGainPriceOffset);
	tPLDetail.plparam.nPriceType = GetCondPriceType();
	tPLDetail.plparam.nCloseMode = GetCloseMode();
	tPLDetail.plparam.fClosePriceOffset = GetCloseOffsetPrice();

	strncpy(tPLDetail.orderSrc.InstrumentID, strInstrumentID.c_str(), 
			sizeof(tPLDetail.orderSrc.InstrumentID)-1);
	tPLDetail.orderSrc.nOrderType = OrderType_FT_Immediately;
	tPLDetail.orderSrc.nOCMode = nOCMode;
	tPLDetail.orderSrc.bIsBuy = bIsBuy;
	tPLDetail.orderSrc.bIsMarket = FALSE;
	tPLDetail.orderSrc.fPrice = fPrice;
	tPLDetail.orderSrc.nVolume = nVolume;
	tPLDetail.orderSrc.nHedgeStatus = 0;

	if(tPLDetail.plparam.bDoStopGain) {
		if(tPLDetail.orderSrc.bIsBuy)
			tPLDetail.fStopGainPrice = fPrice - 
					(tPLDetail.orderSrc.bIsBuy ? tPLDetail.plparam.fStopGainPriceOffset
					: -tPLDetail.plparam.fStopGainPriceOffset);
		else
			tPLDetail.fStopGainPrice = fPrice + 
					(tPLDetail.orderSrc.bIsBuy ? -tPLDetail.plparam.fStopGainPriceOffset
					: tPLDetail.plparam.fStopGainPriceOffset);
	}
	if(tPLDetail.plparam.bDoStopLose) {
		if(tPLDetail.orderSrc.bIsBuy)
			tPLDetail.fStopLosePrice = fPrice + 
					(tPLDetail.orderSrc.bIsBuy ? tPLDetail.plparam.fStopLosePriceOffset
					: -tPLDetail.plparam.fStopLosePriceOffset);
		else
			tPLDetail.fStopLosePrice = fPrice - 
					(tPLDetail.orderSrc.bIsBuy ? -tPLDetail.plparam.fStopLosePriceOffset
					: tPLDetail.plparam.fStopLosePriceOffset);
	}
	return TRUE;
}
