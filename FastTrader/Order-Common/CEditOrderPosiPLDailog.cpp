#include "StdAfx.h"
#include "CEditOrderPosiPLDailog.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "wx\odcombo.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(CEditOrderPosiPLDailog, wxDialog)
    EVT_BUTTON(ID_EDITPOSIPL_OK,OnOk)
    EVT_BUTTON(ID_EDITPOSIPL_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
	EVT_SPIN_UP(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL,OnPriceSpinDown)
    EVT_SPIN_UP(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL,OnPriceSpinDown)
    EVT_SPIN_UP(ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL,OnPriceSpinUp)
    EVT_SPIN_DOWN(ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL,OnPriceSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CEditOrderPosiPLDailog::CEditOrderPosiPLDailog(
			 wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	m_poLocalOrderService(NULL),
	m_bIsAbsCondPrice(FALSE), 
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

	wxPoint posInit(5, 15);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2+4+2;
	int nInputWidth = nFW*11;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL, *pWin = NULL;

	//posInit.x += nFW*3;
	//posInit.y += 24*8;

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y);
	ctlSz.Set(nFW*50, nFH);
	itemTemp = new wxStaticText(this, ID_EDITPOSIPL_POSITIONINFO_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//wxALIGN_CENTER_HORIZONTAL);//持仓基本信息

	//平今还是平昨
	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH);
	ctlSz.Set(nFW*12, nFH);
	itemTemp = new wxStaticText(this, ID_EDITPOSIPL_CLOSEMODE_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//平仓模式

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*2);
	ctlSz.Set(nFW*12, nFH);
	itemTemp = new wxStaticText(this, ID_EDITPOSIPL_QTY_LABEL, LOADSTRING(TITLE_QTY), ctlPos, ctlSz, wxALIGN_RIGHT);//数量: 


	// 止盈价差
	ctlPos = wxPoint(posInit.x+25*nFW, posInit.y+nVH+2);
	ctlSz.Set(nFW*10, nFH);
	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_EDITPOSIPL_STOPGAIN_CHECK, LOADSTRING(8121010), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+25*nFW+nFW*10, posInit.y+nVH);
	ctlSz.Set(nInputWidth-18, 20);
	pWin = new wxTextCtrl( this, ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+25*nFW+nFW*10+nInputWidth-18, posInit.y+nVH);
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton1 = new wxSpinButton( this, ID_EDITPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton1->SetRange(-1000000, 1000000);
    itemSpinButton1->SetValue(0);

	
	// 止损价差
	ctlPos = wxPoint(posInit.x+25*nFW, posInit.y+nVH*2+2);
	ctlSz.Set(nFW*10, nFH);
	wxCheckBox* itemCheckBox2 = new wxCheckBox( this, ID_EDITPOSIPL_STOPLOSE_CHECK, LOADSTRING(8121011), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+25*nFW+nFW*10, posInit.y+nVH*2);
	ctlSz.Set(nInputWidth-18, 20);
	pWin = new wxTextCtrl( this, ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+25*nFW+nFW*10+nInputWidth-18, posInit.y+nVH*2);
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton2 = new wxSpinButton( this, ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton2->SetRange(-1000000, 1000000);
    itemSpinButton2->SetValue(0);


	{			//触发条件价格
	ctlPos = wxPoint(posInit.x+25*nFW, posInit.y+nVH*3);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITPOSIPL_CONDPRICE_LABEL, LOADSTRING(ID_ADDPOSIPL_CONDPRICE_LABEL), ctlPos, ctlSz, wxALIGN_RIGHT);

	wxArrayString strArrOC;
	//strArrOC.Add(LOADSTRING(CFieldValueTool_LastPrice));
	//strArrOC.Add(LOADSTRING(CFieldValueTool_BidPrice1));
	//strArrOC.Add(LOADSTRING(CFieldValueTool_AskPrice1));
	strArrOC.Add(LOADSTRING(OLS_LASTPRICE3));
	strArrOC.Add(LOADSTRING(OLS_B_S_PRICE3));
	
	ctlPos = wxPoint(posInit.x+25*nFW+nFW*10, posInit.y+nVH*3);
	wxOwnerDrawnComboBox* pCmbCondPrice = new wxOwnerDrawnComboBox(this, ID_EDITPOSIPL_CONDPRICETYPE_COMBOBOX, wxEmptyString, 
			ctlPos, wxSize(nInputWidth, 20), strArrOC, wxCB_READONLY);
	pCmbCondPrice->SetSelection(0);
	}

//	ctlPos = wxPoint(posInit.x+2*nFW+nFW*28, posInit.y+nVH*3);
//	ctlSz.Set(nFW*20, nFH*4);
//	itemTemp = new wxStaticText(this, ID_EDITPOSIPL_POSIVOLUME_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_LEFT);//买卖: 3


	ctlPos = wxPoint(posInit.x, posInit.y+nVH*4);
	ctlSz.Set(nFW*52, nVH*4+4);
	pWin = new wxStaticBox( this, ID_EDITPOSIPL_STATICBOX2, LOADSTRING(OLS_CLOSE_MODE), ctlPos, ctlSz);

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*5);
	ctlSz.Set(nFW*48, nFH);
	itemTemp = new wxStaticText(this, ID_EDITPOSIPL_DETAIL2_LABEL, LOADSTRING(ID_EDITPOSIPL_DETAIL2_LABEL), ctlPos, ctlSz, wxALIGN_LEFT|wxALIGN_BOTTOM);//平仓模式

	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*6+2);
	ctlSz.Set(nFW*10, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_EDITPOSIPL_DOPRICEOFFSET_RADIO, LOADSTRING(OLS_REVERSE), ctlPos, ctlSz, wxRB_GROUP);
	itemRadioBox1->SetValue(true);

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10, posInit.y+nVH*6);
	ctlSz.Set(nInputWidth-18, 20);
	pWin = new wxTextCtrl( this, ID_EDITPOSIPL_DOPRICEOFFSET_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+2*nFW+nFW*10+nInputWidth-18, posInit.y+nVH*6);
	ctlSz.Set(18, 20);
    wxSpinButton* itemSpinButton3 = new wxSpinButton( this, ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton3->SetRange(0, 1000000);
    itemSpinButton3->SetValue(0);


	ctlPos = wxPoint(posInit.x+2*nFW, posInit.y+nVH*7+2);
	ctlSz.Set(nFW*20, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton( this, ID_EDITPOSIPL_DOPRICELIMIT_RADIO, LOADSTRING(OLS_PRICE_LIMIT), ctlPos, ctlSz);
	itemRadioBox2->SetValue(false);


	posInit.x += nFW*36;
	posInit.y += nVH*9;

	ctlPos = wxPoint(posInit.x, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITPOSIPL_OK, LOADSTRING(ASKNAMEDLG_OK), ctlPos, ctlSz, 0 );//确定

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITPOSIPL_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), ctlPos, ctlSz, 0 );//取消

	//SetSize(wxSize(180+10+195, 24*11+10+10));
	SetSize(wxSize(nFW*54, 24*12-4));

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_EDITPOSIPL_STOPLOSE_CHECK);
	m_TabOrderVector.push_back(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT);
	m_TabOrderVector.push_back(ID_EDITPOSIPL_STOPGAIN_CHECK);
	m_TabOrderVector.push_back(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT);

	m_TabOrderVector.push_back(ID_EDITPOSIPL_CONDPRICETYPE_COMBOBOX);
	m_TabOrderVector.push_back(ID_EDITPOSIPL_DOPRICEOFFSET_RADIO);
	m_TabOrderVector.push_back(ID_EDITPOSIPL_DOPRICEOFFSET_TEXT);
	m_TabOrderVector.push_back(ID_EDITPOSIPL_DOPRICELIMIT_RADIO);

	m_TabOrderVector.push_back(ID_EDITPOSIPL_OK);
    m_TabOrderVector.push_back(ID_EDITPOSIPL_CANCEL);

	m_poLocalOrderService = CLocalOrderService::GetInstance();
	m_poLocalOrderService->RegisterMsgWindow(this);

}

CEditOrderPosiPLDailog::~CEditOrderPosiPLDailog()
{
	m_poLocalOrderService->UnregisterMsgWindow(this);
	m_poLocalOrderService = NULL;
}

// 获得止损价格信息
BOOL CEditOrderPosiPLDailog::GetLoseOffsetPrice(double& fOffsetPrice) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_EDITPOSIPL_STOPLOSE_CHECK,this);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
	fOffsetPrice=atof(pText->GetLabel());
	return window->GetValue();
}

void CEditOrderPosiPLDailog::SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice) 
{
	wxCheckBox* pCheck=NULL;
	pCheck=(wxCheckBox*)FindWindowById(ID_EDITPOSIPL_STOPLOSE_CHECK,this);
	pCheck->SetValue(bIsStopLose!=0);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	pText->SetLabel(strText);
}

// 获得止盈价格信息
BOOL CEditOrderPosiPLDailog::GetGainOffsetPrice(double& fOffsetPrice) 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_EDITPOSIPL_STOPGAIN_CHECK,this);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
	fOffsetPrice=atof(pText->GetLabel());
	return window->GetValue();
}

void CEditOrderPosiPLDailog::SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice) 
{
	wxCheckBox* pCheck=NULL;
	pCheck=(wxCheckBox*)FindWindowById(ID_EDITPOSIPL_STOPGAIN_CHECK,this);
	pCheck->SetValue(bIsStopGain!=0);
	wxTextCtrl* pText=NULL;
	pText=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	pText->SetLabel(strText);
}

// 触发价格类型，1 最新价，2 买卖价，3 买一价，4 卖一价
int CEditOrderPosiPLDailog::GetCondPriceType() 
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_EDITPOSIPL_CONDPRICETYPE_COMBOBOX,this);
	//if(pComboBox->GetSelection()==0)
	//	return pComboBox->GetSelection()+1;
	//else
	//	return pComboBox->GetSelection()+2;
	return pComboBox->GetSelection()+1;
}

void CEditOrderPosiPLDailog::SetCondPriceType(int nPriceType) 
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_EDITPOSIPL_CONDPRICETYPE_COMBOBOX,this);
	//if(pComboBox->IsEnabled())
	//	if(nPriceType==1)
	//		pComboBox->SetSelection(nPriceType-1);
	//	else
	//		pComboBox->SetSelection(nPriceType-2);
	if(pComboBox->IsEnabled())
		pComboBox->SetSelection(nPriceType-1);
}

// 平仓方式，1 反向涨跌停价，2 反向加减
int CEditOrderPosiPLDailog::GetCloseMode() 
{
	wxRadioButton* window=NULL;
	window=(wxRadioButton*)FindWindowById(ID_EDITPOSIPL_DOPRICELIMIT_RADIO,this);
	if(window->GetValue()) return 1;
	window=(wxRadioButton*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_RADIO,this);
	if(window->GetValue()) return 2;
	return -1;
}

void CEditOrderPosiPLDailog::SetCloseMode(int nCloseMode) 
{
	wxRadioButton* window=NULL;
	switch(nCloseMode) {
	case 1:
		window=(wxRadioButton*)FindWindowById(ID_EDITPOSIPL_DOPRICELIMIT_RADIO,this);
		window->SetValue(1);
		break;
	case 2:
		window=(wxRadioButton*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_RADIO,this);
		window->SetValue(1);
		break;
	};
}

double CEditOrderPosiPLDailog::GetCloseOffsetPrice() 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_TEXT,this);
	return atof(window->GetLabel());
}

void CEditOrderPosiPLDailog::SetCloseOffsetPrice(double fOffsetPrice) 
{
	wxTextCtrl* window=NULL;
	window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_TEXT,this);
	wxString strText;
	strText = Price2String(fOffsetPrice, m_fPriceTick).c_str();
	window->SetLabel(strText);
}

wxString CEditOrderPosiPLDailog::GetStaticText(UINT nUIID)
{
	wxString strText;
    wxWindow* pWin=FindWindowById(nUIID, this);
	if(pWin!=NULL)
		strText = pWin->GetLabel();
	return strText;
}

// 仅此用于日志文件输出
wxString CEditOrderPosiPLDailog::GetUserInputText()
{
	wxString strRet;

	strRet = "止盈止损参数：\n";

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_EDITPOSIPL_STOPLOSE_CHECK,this);
		if(window->GetValue()) {
			wxTextCtrl* pText=NULL;
			strRet+="选择止损，止损偏移=[";
			pText=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
			strRet+=pText->GetLabel();
			strRet+="],\t ";
		}
		else {
			strRet+="未选择止损,\t ";
		}
	}

	{
		wxCheckBox* window=NULL;
		window=(wxCheckBox*)FindWindowById(ID_EDITPOSIPL_STOPGAIN_CHECK,this);
		if(window->GetValue()) {
			wxTextCtrl* pText=NULL;
			strRet+="选择止盈，止盈偏移=[";
			pText=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
			strRet+=pText->GetLabel();
			strRet+="],\t ";
		}
		else {
			strRet+="未选择止盈,\t ";
		}
	}

	{
		wxOwnerDrawnComboBox* window=NULL;
		window=(wxOwnerDrawnComboBox*)FindWindowById(ID_EDITPOSIPL_CONDPRICETYPE_COMBOBOX,this);
		if(window->GetValue()) {
			strRet+="价格类型=[";
			strRet+=window->GetString(window->GetSelection());
			strRet+="],\t ";
		}
	}
	
	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_EDITPOSIPL_DOPRICELIMIT_RADIO,this);
		if(window->GetValue()) {
			strRet+="平仓方式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
		window=(wxRadioButton*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_RADIO,this);
		if(window->GetValue()) {
			strRet+="平仓方式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}

	{
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_TEXT,this);
		strRet+="反向加减=[";
		strRet+=window->GetLabel();
		strRet+="],\t ";
	}

	strRet+="\r\n";
	return strRet;

}

wxString CEditOrderPosiPLDailog::GetReqLog()
{
	wxString strText;
	strText += GetStaticText(ID_EDITPOSIPL_POSITIONINFO_LABEL);
	strText += ",\t ";
	strText += GetStaticText(ID_EDITPOSIPL_CLOSEMODE_LABEL);
	strText += ",\t ";
	strText += GetStaticText(ID_EDITPOSIPL_QTY_LABEL);
	strText += ",\t \r\n";
	strText += GetUserInputText();
	return strText;
}

BOOL CEditOrderPosiPLDailog::Init(BOOL bIsAbsCond)
{
	m_bIsAbsCondPrice = bIsAbsCond;

    PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(
			std::string(m_tPLDetailOrder.InstrumentID),insInfo)!=0)
		return FALSE;
	
	SetPriceTick(insInfo.PriceTick);
	
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()
			&&DEFAULT_SVR()->GetQuotInfo(m_tPLDetailOrder.InstrumentID,field)!=0) {
    }
	SetUpperLimitPrice(field.UpperLimitPrice);
	SetLowerLimitPrice(field.LowerLimitPrice);
	
	ShowOrderPLDetail();

	return TRUE;
}

void CEditOrderPosiPLDailog::ShowOrderPLDetail()
{
	wxWindow* pwinItem=NULL;
	wxString strValue;

	pwinItem=FindWindowById(ID_EDITPOSIPL_POSITIONINFO_LABEL, this);
	strValue.Printf(LOADFORMATSTRING(ID_EDITPOSIPL_POSITIONINFO_LABEL, "%s%s%s"), //文本必须包含%s
			m_tPLDetailOrder.InstrumentID, 
			!m_tPLDetailOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY) : LOADSTRING(OLS_SELL),
            CFieldValueTool::HedgeFlag2String(m_tPLDetailOrder.orderSrc.nHedgeStatus+THOST_FTDC_HF_Speculation));
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem = FindWindowById(ID_EDITPOSIPL_CLOSEMODE_LABEL, this);
	if(pwinItem) {
		strValue.Printf(LOADFORMATSTRING(ID_EDITPOSIPL_CLOSEMODE_LABEL, "%s"), 
				m_tPLDetailOrder.orderSrc.nOCMode==1 ? 
						LOADSTRING(CFieldValueTool_CloseToday) : LOADSTRING(CFieldValueTool_CloseYesterday));
		pwinItem->SetLabel(strValue);
	}

	pwinItem = FindWindowById(ID_EDITPOSIPL_QTY_LABEL, this);
	if(pwinItem) {
		strValue.Printf(LOADFORMATSTRING(ID_EDITPOSIPL_QTY_LABEL, "%d"), 
				m_tPLDetailOrder.orderSrc.nVolume);
		pwinItem->SetLabel(strValue);
	}

	if(m_bIsAbsCondPrice) {
		SetLoseOffsetPrice(
				m_tPLDetailOrder.plparam.bDoStopLose, m_tPLDetailOrder.fStopLosePrice);
		SetGainOffsetPrice(
				m_tPLDetailOrder.plparam.bDoStopGain, m_tPLDetailOrder.fStopGainPrice);
	}
	else {
		SetLoseOffsetPrice(
				m_tPLDetailOrder.plparam.bDoStopLose, m_tPLDetailOrder.plparam.fStopLosePriceOffset);
		SetGainOffsetPrice(
				m_tPLDetailOrder.plparam.bDoStopGain, m_tPLDetailOrder.plparam.fStopGainPriceOffset);
	}
	SetCondPriceType(m_tPLDetailOrder.plparam.nPriceType);
	SetCloseMode(m_tPLDetailOrder.plparam.nCloseMode);
	SetCloseOffsetPrice(m_tPLDetailOrder.plparam.fClosePriceOffset);
}

void CEditOrderPosiPLDailog::ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow)
{
	TRADEINFODLG(this)->ShowTradeMsg(errormessage, -1, Title, -1, bIsShow);
}

bool CEditOrderPosiPLDailog::Show( bool show )
{
    return wxDialog::Show(show);
}

void CEditOrderPosiPLDailog::OnOk(wxCommandEvent& event)
{
	wxString name(m_tPLDetailOrder.InstrumentID);
	std::string strAccount;
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));
	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)!=0) {
		ShowTradeInfoDlg(LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),LOADSTRING(PosiPLADD_NoQuotNoBasisPrice), TRUE);
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

	double fPrice = m_tPLDetailOrder.orderSrc.fPrice;
	BOOL bIsBuy = !m_tPLDetailOrder.orderSrc.bIsBuy;

	if(m_bIsAbsCondPrice) {
		if(bIsCheckLose && (
				(bIsBuy && util::lessOrEqual(fLoseOffsetPrice, field.LowerLimitPrice))
				|| 
				(!bIsBuy && util::greaterOrEqual(fLoseOffsetPrice, field.UpperLimitPrice))
				)) {
			ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(PosiPLADD_HasLoseOverMsg), TRUE);
			return;
		}
		if(bIsCheckGain && (
				(bIsBuy && util::greaterOrEqual(fGainOffsetPrice, field.UpperLimitPrice))
				|| 
				(!bIsBuy && util::lessOrEqual(fGainOffsetPrice, field.LowerLimitPrice))
				)) {
			ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(PosiPLADD_HasGainOverMsg), TRUE);
			return;
		}
	}
	else {
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
		if(m_bIsAbsCondPrice) {
			if(bIsCheckLose && (
					(bIsBuy && util::greaterOrEqual(fLoseOffsetPrice, fBasePrice))
					|| 
					(!bIsBuy && util::lessOrEqual(fLoseOffsetPrice, fBasePrice))
					))
				bIsSuccess = TRUE;
			if(bIsCheckGain && (
					(bIsBuy && util::lessOrEqual(fGainOffsetPrice, fBasePrice))
					|| 
					(!bIsBuy && util::greaterOrEqual(fGainOffsetPrice, fBasePrice))
					))
				bIsSuccess = TRUE;
		}
		else {
			if(bIsCheckLose && (
					(bIsBuy && util::greaterOrEqual(fPrice-fLoseOffsetPrice, fBasePrice))
					|| 
					(!bIsBuy && util::lessOrEqual(fPrice+fLoseOffsetPrice, fBasePrice))
					))
				bIsSuccess = TRUE;
			if(bIsCheckGain && (
					(bIsBuy && util::lessOrEqual(fPrice+fGainOffsetPrice, fBasePrice))
					|| 
					(!bIsBuy && util::greaterOrEqual(fPrice-fGainOffsetPrice, fBasePrice))
					))
				bIsSuccess = TRUE;
		}
	}
	if(bIsSuccess) {
		wxString strText;
		//strText.Printf("当前止盈止损条件已满足，不能下持仓止盈止损。");
		strText = LOADSTRING(PosiPLADD_HasTriggerMsg);
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),strText, TRUE);
		return;
	}
	int nOCMode = m_tPLDetailOrder.orderSrc.nOCMode;
	int nVol = m_tPLDetailOrder.orderSrc.nVolume;

	MakePosiPL(strAccount, std::string(name.c_str()), bIsBuy, nOCMode, fPrice, nVol, 
			m_tPLDetailOrder);

	USERLOG_INFO("用户编辑持仓止盈止损，对话框中单击[确认]确认\n%s", 
			GetReqLog().c_str());

	EndModal(wxID_OK);
}

void CEditOrderPosiPLDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CEditOrderPosiPLDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CEditOrderPosiPLDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CEditOrderPosiPLDailog::OnDialogCharHook(wxKeyEvent& evt)
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
				//evt.Skip();
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
					//evt.Skip();
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
 					//evt.Skip();
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
					//evt.Skip();
                    return;
                }
            }
        }
    }
	else {
		evt.Skip();
	}

}

void CEditOrderPosiPLDailog::OnPriceSpinUp(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	switch(event.GetId()) {
	case ID_EDITPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
		break;
	case ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
		break;
	case ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_TEXT,this);
		break;
	default:
		return;
		break;
	};
	if(window==NULL) return;
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	if(m_bIsAbsCondPrice && event.GetId()!=ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL)
		fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
	else
		fPrice = AnalysePrice(fPrice, (GetUpperLimitPrice()-GetLowerLimitPrice()), 0.0);
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

void CEditOrderPosiPLDailog::OnPriceSpinDown(wxSpinEvent& event)
{
	wxTextCtrl* window=NULL;
	switch(event.GetId()) {
	case ID_EDITPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT,this);
		break;
	case ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT,this);
		break;
	case ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL:
		window=(wxTextCtrl*)FindWindowById(ID_EDITPOSIPL_DOPRICEOFFSET_TEXT,this);
		break;
	default:
		return;
		break;
	};
	if(window==NULL) return;
	double fPrice=atof(window->GetLabel());
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	if(m_bIsAbsCondPrice && event.GetId()!=ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL)
		fPrice = AnalysePrice(fPrice, GetUpperLimitPrice(), GetLowerLimitPrice());
	else
		fPrice = AnalysePrice(fPrice, (GetUpperLimitPrice()-GetLowerLimitPrice()), 0.0);
	wxString strValue;
	strValue = Price2String(fPrice, m_fPriceTick).c_str();
	window->SetLabel(strValue);
}

BOOL CEditOrderPosiPLDailog::MakePosiPL(std::string& strAccount, std::string& strInstrumentID,
										BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, 
										CLocalOrderService::PLDETAILORDER& tPLDetail)
{
	tPLDetail.plparam.nPriceOffsetType = 0;//GetOffsetPriceObject();
	tPLDetail.plparam.bDoStopLose = GetLoseOffsetPrice(
			tPLDetail.plparam.fStopLosePriceOffset);
	tPLDetail.fStopLosePrice = tPLDetail.plparam.fStopLosePriceOffset;
	tPLDetail.plparam.bDoStopGain = GetGainOffsetPrice(
			tPLDetail.plparam.fStopGainPriceOffset);
	tPLDetail.fStopGainPrice = tPLDetail.plparam.fStopGainPriceOffset;
	tPLDetail.plparam.nPriceType = GetCondPriceType();
	tPLDetail.plparam.nCloseMode = GetCloseMode();
	tPLDetail.plparam.fClosePriceOffset = GetCloseOffsetPrice();

	//tPLDetail.orderSrc.nVolume = nVolume;

	return TRUE;
}
