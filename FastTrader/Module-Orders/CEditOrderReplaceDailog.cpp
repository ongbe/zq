#include "StdAfx.h"
#include "CEditOrderReplaceDailog.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "wx\odcombo.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


using namespace util;


BEGIN_EVENT_TABLE(CEditOrderReplaceDailog, wxDialog)
    EVT_BUTTON(ID_EDITORDERREPLACE_OK,OnOk)
    EVT_BUTTON(ID_EDITORDERREPLACE_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
	EVT_SPIN_UP(ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL, OnPrice2SpinUp)
	EVT_SPIN_DOWN(ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL, OnPrice2SpinDown)
	EVT_SPIN_UP(ID_EDITORDERREPLACE_NEWVOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_EDITORDERREPLACE_NEWVOLUME_SPINCTRL, OnVolumeSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CEditOrderReplaceDailog::CEditOrderReplaceDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	m_fPriceTick(1.0f)
{
	DIALOG_CREATE_LOG();
	ZeroMemory(&m_tftdcOrderField, sizeof(m_tftdcOrderField));
	ZeroMemory(&m_tftdcOrderInputField, sizeof(m_tftdcOrderInputField));

	SetBackgroundColour(DEFAULT_COLOUR);
	//wxFont oldfont=GetFont();
	//oldfont.SetWeight(wxBOLD);
	//SetFont(oldfont);

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2+4;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_ORDERTYPE_LABEL, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//下单类型: -

	ctlPos = wxPoint(posInit.x, posInit.y+nVH);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC1, wxEmptyString, ctlPos, ctlSz);//合约: 2

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC2, wxEmptyString, ctlPos, ctlSz);//合约: 2

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*3);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC3, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//买卖: 3

	ctlPos = wxPoint(posInit.x+nFW*11, posInit.y+nVH*3);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC4, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//投保: 4
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		itemTemp->Show(false);
	}

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*4);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC5, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//开平: 6
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		itemTemp->Show(false);
	}

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*5);
	ctlSz.Set(nFW*4+nFW/2, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC6, LOADSTRING(TITLE_PRICE), ctlPos, ctlSz, wxALIGN_LEFT);//价格: 

	ctlPos.x += (nFW*4)+nFW/2;
//	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Price(ctlPos);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*6);
	ctlSz.Set(nFW*4+nFW/2, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC7, LOADSTRING(TITLE_PRICE2), ctlPos, ctlSz, wxALIGN_LEFT);//价二: 

	ctlPos.x += (nFW*4)+nFW/2;
//	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Price2(ctlPos);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*7);
	ctlSz.Set(nFW*4+nFW/2, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC8, LOADSTRING(TITLE_QTY), ctlPos, ctlSz, wxALIGN_LEFT);//数量: 

	ctlPos.x += (nFW*4)+nFW/2;
//	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Volume(ctlPos);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*8);
	ctlSz.Set(nFW*4+nFW/2, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERREPLACE_STATIC8, LOADSTRING(TITLE_ACCOUNT), ctlPos, ctlSz, wxALIGN_LEFT);//帐号: 

	
	{
	wxArrayString strArrAccount;
	vector<string> accountList;
	accountList.clear();
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetAccountList(accountList)>0) {
		for(int i=0; i<(int)accountList.size(); i++) 
			strArrAccount.Add(wxT(accountList[i]));
	}
	ctlPos = wxPoint(posInit.x, posInit.y+nVH*8);
	ctlPos.x += (nFW*4)+nFW/2;
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pComboBox = new wxOwnerDrawnComboBox(this, ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO, 
		wxT(""), ctlPos, ctlSz, strArrAccount, wxCB_DROPDOWN);//|wxCB_SORT);
	pComboBox->GetTextCtrl()->SetMaxLength(20);
	}


	posInit.x += nFW*7;
	posInit.y += (24*9-5);

	ctlPos = wxPoint(posInit.x, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITORDERREPLACE_OK, LOADSTRING(ASKNAMEDLG_OK), ctlPos, ctlSz, 0 );//确定

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITORDERREPLACE_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), ctlPos, ctlSz, 0 );//取消

	SetSize(wxSize(180+80, 24*11+10));

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE_TEXT);
	m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE2_TEXT);
	m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWVOLUME_TEXT);
	m_TabOrderVector.push_back(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO);

	m_TabOrderVector.push_back(ID_EDITORDERREPLACE_OK);
    m_TabOrderVector.push_back(ID_EDITORDERREPLACE_CANCEL);

}

CEditOrderReplaceDailog::~CEditOrderReplaceDailog()
{
}

wxTextCtrl* CEditOrderReplaceDailog::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_EDITORDERREPLACE_NEWPRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

//	itemTextCtrl3->Connect(ID_OIJSD_PRICE_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnPriceClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* CEditOrderReplaceDailog::CreateFloatSpinCtrl_Price2(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_EDITORDERREPLACE_NEWPRICE2_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

//	itemTextCtrl3->Connect(ID_OIJSD_PRICE_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnPriceClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* CEditOrderReplaceDailog::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
    wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_EDITORDERREPLACE_NEWVOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);

//	itemTextCtrl3->Connect(ID_OIJSD_VOLUME_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnVolumeClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_EDITORDERREPLACE_NEWVOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void CEditOrderReplaceDailog::Convert(PlatformStru_OrderInfo& tftdcOrderField, 
									  PlatformStru_InputOrder& tftdcOrderInputField)
{

	///经纪公司代码
	memcpy(tftdcOrderInputField.BrokerID, tftdcOrderField.BrokerID, sizeof(tftdcOrderInputField.BrokerID));
	///投资者代码
	memcpy(tftdcOrderInputField.InvestorID, tftdcOrderField.InvestorID, sizeof(tftdcOrderInputField.InvestorID));
	///合约代码
	memcpy(tftdcOrderInputField.InstrumentID, tftdcOrderField.InstrumentID, sizeof(tftdcOrderInputField.InstrumentID));
	///报单引用
	// 这个地方要特殊处理
	memcpy(tftdcOrderInputField.strLocalRefID, tftdcOrderField.OrderSysID, sizeof(tftdcOrderInputField.OrderRef));
	///用户代码
	memcpy(tftdcOrderInputField.UserID, tftdcOrderField.UserID, sizeof(tftdcOrderInputField.UserID));
	///报单价格条件
	tftdcOrderInputField.OrderPriceType = tftdcOrderField.OrderPriceType;
	///买卖方向
	tftdcOrderInputField.Direction = tftdcOrderField.Direction;
	///组合开平标志
	memcpy(tftdcOrderInputField.CombOffsetFlag, tftdcOrderField.CombOffsetFlag, sizeof(tftdcOrderInputField.CombOffsetFlag));
	///组合投机套保标志
	memcpy(tftdcOrderInputField.CombHedgeFlag, tftdcOrderField.CombHedgeFlag, sizeof(tftdcOrderInputField.CombHedgeFlag));
	///价格
	tftdcOrderInputField.LimitPrice = tftdcOrderField.LimitPrice;
	///数量
	tftdcOrderInputField.VolumeTotalOriginal = tftdcOrderField.VolumeTotal;
	///有效期类型
	tftdcOrderInputField.TimeCondition = tftdcOrderField.TimeCondition;
	///GTD日期
	memcpy(tftdcOrderInputField.GTDDate, tftdcOrderField.GTDDate, sizeof(tftdcOrderInputField.GTDDate));
	///成交量类型
	tftdcOrderInputField.VolumeCondition = tftdcOrderField.VolumeCondition;
	///最小成交量
	tftdcOrderInputField.MinVolume = tftdcOrderField.MinVolume;
	///触发条件
	tftdcOrderInputField.ContingentCondition = tftdcOrderField.ContingentCondition;
	///止损价
	tftdcOrderInputField.StopPrice = tftdcOrderField.StopPrice;
	///强平原因
	tftdcOrderInputField.ForceCloseReason = tftdcOrderField.ForceCloseReason;
	///自动挂起标志
	tftdcOrderInputField.IsAutoSuspend = tftdcOrderField.IsAutoSuspend;
	///业务单元
	memcpy(tftdcOrderInputField.BusinessUnit, tftdcOrderField.BusinessUnit, sizeof(tftdcOrderInputField.BusinessUnit));
	///请求编号
	tftdcOrderInputField.RequestID = tftdcOrderField.RequestID;
	///用户强评标志
	tftdcOrderInputField.UserForceClose = tftdcOrderField.UserForceClose;

	// 通用扩展域
	memcpy(tftdcOrderInputField.strLocalRequestID, tftdcOrderField.strLocalRequestID, sizeof(tftdcOrderInputField.strLocalRequestID));				// 本地的请求标号
	memcpy(tftdcOrderInputField.strAccount, tftdcOrderField.Account, sizeof(tftdcOrderInputField.strAccount));					// 交易账号
	memcpy(tftdcOrderInputField.strExchange, tftdcOrderField.strExchange, sizeof(tftdcOrderInputField.strExchange));				// 交易所代码

}

void CEditOrderReplaceDailog::ShowOrderReq()
{

	wxWindow* pwinItem=NULL;
	wxString strValue;
	int nOrderType = 0;

	// 如果是市价单
	if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_AnyPrice 
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Immediately) {
				strValue=LOADSTRING(OLS_ORDERTYPE_MARKET);
		nOrderType = 1;
	}
	// 如果是限价单
	else if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_LimitPrice
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Immediately) {
				strValue=LOADSTRING(OLS_ORDERTYPE_LIMIT);
		nOrderType = 2;
	}
	// 如果是Stop单
	else if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_AnyPrice
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Touch) {
		strValue=LOADSTRING(OLS_ORDERTYPE_STOP);
		nOrderType = 3;
	}
	// 如果是Stop Limit单
	else if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_LimitPrice
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Touch) {
				strValue=LOADSTRING(OLS_ORDERTYPE_STOP_LIMIT);
		nOrderType = 4;
	}
	pwinItem=FindWindowById(ID_EDITORDERREPLACE_ORDERTYPE_LABEL, this);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_EDITORDERREPLACE_STATIC1, this);
	strValue.Printf(LOADFORMATSTRING(EOCD_ORDERNO,"%s"), m_tftdcOrderField.OrderSysID);//文本必须包含%s
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_EDITORDERREPLACE_STATIC2, this);
	strValue.Printf(LOADFORMATSTRING(EOCD_CONTACT,"%s"), m_tftdcOrderField.InstrumentID);//文本必须包含%s
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_EDITORDERREPLACE_STATIC3, this);

	if(pwinItem) pwinItem->SetLabel(
		m_tftdcOrderField.Direction==THOST_FTDC_D_Buy ? LOADSTRING(EOCD_BUYSELL_B) :LOADSTRING(EOCD_BUYSELL_S));
	pwinItem=FindWindowById(ID_EDITORDERREPLACE_STATIC4, this);
	if(pwinItem) pwinItem->SetLabel(
			m_tftdcOrderField.CombHedgeFlag[0]==THOST_FTDC_BHF_Speculation ?LOADSTRING(EOCD_HEDG_S) :LOADSTRING(EOCD_HEDG_H));

	pwinItem=FindWindowById(ID_EDITORDERREPLACE_STATIC5, this);
	if(pwinItem) pwinItem->SetLabel(
			m_tftdcOrderField.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(EOCD_OCT_O)  : 
			(m_tftdcOrderField.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ?LOADSTRING(EOCD_OCT_T) : LOADSTRING(EOCD_OCT_C)));

	pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWVOLUME_TEXT, this);
	strValue.Printf("%d", m_tftdcOrderField.VolumeTotalOriginal);
	if(pwinItem) pwinItem->SetLabel(strValue);

	strValue.Printf("%s", m_tftdcOrderField.Account);
	SetTraderAccount(strValue);

	m_TabOrderVector.clear();
	switch(nOrderType) {
	case 1:
		strValue=LOADSTRING(OLS_MARKET);
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT, this);
		if(pwinItem) pwinItem->SetLabel(strValue);

		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT, this);
		if(pwinItem) pwinItem->Enable(false);
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL, this);
		if(pwinItem) pwinItem->Enable(false);
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_TEXT, this);
		if(pwinItem) pwinItem->Enable(false);
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL, this);
		if(pwinItem) pwinItem->Enable(false);
		//tab order
		//m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE_TEXT);
		//m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE2_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWVOLUME_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO);
		break;
	case 2:
		strValue = Price2String(m_tftdcOrderField.LimitPrice, m_fPriceTick).c_str();
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT, this);
		if(pwinItem) pwinItem->SetLabel(strValue);

		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_TEXT, this);
		if(pwinItem) pwinItem->Enable(false);
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL, this);
		if(pwinItem) pwinItem->Enable(false);
		//tab order
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE_TEXT);
		//m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE2_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWVOLUME_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO);
		break;
	case 3:
		strValue = Price2String(m_tftdcOrderField.StopPrice, m_fPriceTick).c_str();
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT, this);
		if(pwinItem) pwinItem->SetLabel(strValue);

		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_TEXT, this);
		if(pwinItem) pwinItem->Enable(false);
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL, this);
		if(pwinItem) pwinItem->Enable(false);
		//tab order
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE_TEXT);
		//m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE2_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWVOLUME_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO);
		break;
	case 4:
		strValue = Price2String(m_tftdcOrderField.StopPrice, m_fPriceTick).c_str();
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT, this);
		if(pwinItem) pwinItem->SetLabel(strValue);
		strValue = Price2String(m_tftdcOrderField.LimitPrice, m_fPriceTick).c_str();
		pwinItem=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_TEXT, this);
		if(pwinItem) pwinItem->SetLabel(strValue);

		//tab order
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWPRICE2_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_NEWVOLUME_TEXT);
		m_TabOrderVector.push_back(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO);
		break;
	};
	m_TabOrderVector.push_back(ID_EDITORDERREPLACE_OK);
    m_TabOrderVector.push_back(ID_EDITORDERREPLACE_CANCEL);

}

void CEditOrderReplaceDailog::SetPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
}

double CEditOrderReplaceDailog::GetPrice()
{
	wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void CEditOrderReplaceDailog::SetPrice2(double fPrice)
{
	wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
}

double CEditOrderReplaceDailog::GetPrice2()
{
	wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE2_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

// 价格是否为市价
BOOL CEditOrderReplaceDailog::GetIsPriceMode()
{
	wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWPRICE_TEXT,this);
	wxString str=window->GetLabel();
	return str.compare(LOADSTRING(OLS_MARKET))==0;
}

void CEditOrderReplaceDailog::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWVOLUME_TEXT,this);
	window->SetLabel(strText);
}

int CEditOrderReplaceDailog::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_EDITORDERREPLACE_NEWVOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

void CEditOrderReplaceDailog::SetTraderAccount(wxString& strTraderAccount)
{
    wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)
			FindWindowById(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO,this);
	window->SetValue(strTraderAccount);
	//window->GetTextCtrl()->WriteText(strTraderAccount);
	window->GetTextCtrl()->SetSelection(0, 0);
}

wxString CEditOrderReplaceDailog::GetTraderAccount()
{
	wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)FindWindowById(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO, this);
	return window->GetValue();
}

bool CEditOrderReplaceDailog::Show( bool show )
{
	if(show) {
		ShowOrderReq();
		wxString strText;
		strText.Printf(LOADFORMATSTRING(EOCD_FCOSID,"%s"), m_tftdcOrderField.OrderSysID);//文本必须包含%s
		this->SetLabel(strText);
	}
    return wxDialog::Show(show);
}

wxString CEditOrderReplaceDailog::GetStaticText(UINT nUIID)
{
	wxString strText;
	if(nUIID!=ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO) {
		wxWindow* pWin=FindWindowById(nUIID, this);
		if(pWin!=NULL)
			strText = pWin->GetLabel();
	}
	else {
		strText = GetTraderAccount();
	}
	return strText;
}

void CEditOrderReplaceDailog::OnOk(wxCommandEvent& event)
{
	wxString strText;
	
	strText = GetTraderAccount();
	strncpy(m_tftdcOrderField.Account, strText.c_str(), strText.size()<20 ? strText.size() : 20);
	m_tftdcOrderField.VolumeTotal = GetQty();
	
	// 如果是市价单
	if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_AnyPrice 
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Immediately) {
	}
	// 如果是限价单
	else if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_LimitPrice
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Immediately) {
		m_tftdcOrderField.LimitPrice = GetPrice();
	}
	// 如果是Stop单
	else if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_AnyPrice
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Touch) {
		m_tftdcOrderField.StopPrice = GetPrice();
	}
	// 如果是Stop Limit单
	else if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_LimitPrice
			&& m_tftdcOrderField.ContingentCondition==THOST_FTDC_CC_Touch) {
		m_tftdcOrderField.StopPrice = GetPrice();
		m_tftdcOrderField.LimitPrice = GetPrice2();
	}

	Convert(m_tftdcOrderField, m_tftdcOrderInputField);
	// 设置为外盘改单类型
	m_tftdcOrderInputField.OrderClass=ORDERCLASS_ReplaceOrder;

	USERLOG_INFO("Pats快速改单，用户在对话框中单击按钮[确认]确认\n\"\n%s\n%s\n%s\n%s\n%s\n%s\n%s=[%s]\n%s=[%s]\n%s=[%s]\n\"", 
			GetTitle(), 
			GetStaticText(ID_EDITORDERREPLACE_STATIC1).c_str(), 
			GetStaticText(ID_EDITORDERREPLACE_STATIC2).c_str(), 
			GetStaticText(ID_EDITORDERREPLACE_STATIC3).c_str(), 
			GetStaticText(ID_EDITORDERREPLACE_STATIC4).c_str(), 
			GetStaticText(ID_EDITORDERREPLACE_STATIC5).c_str(),
			GetStaticText(ID_EDITORDERREPLACE_STATIC6).c_str(),
			GetStaticText(ID_EDITORDERREPLACE_NEWPRICE_TEXT).c_str(),
			GetStaticText(ID_EDITORDERREPLACE_STATIC7).c_str(),
			GetStaticText(ID_EDITORDERREPLACE_NEWVOLUME_TEXT).c_str(),
			GetStaticText(ID_EDITORDERREPLACE_STATIC8).c_str(),
			GetStaticText(ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO).c_str());

	EndModal(wxID_OK);
}

void CEditOrderReplaceDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CEditOrderReplaceDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CEditOrderReplaceDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CEditOrderReplaceDailog::OnDialogCharHook(wxKeyEvent& evt)
{
    
	wxSpinEvent evtSpin;
	wxKeyEvent* pEvent=&evt;//(wxKeyEvent*)evt.GetClientData();
    if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
		evt.Skip();
        return;
    }
	pEvent->SetId(win->GetId());
    if(pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB)
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
	else if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		switch(evt.GetId()) {
		case ID_EDITORDERREPLACE_NEWPRICE_TEXT:
			evtSpin.SetId(ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_EDITORDERREPLACE_NEWVOLUME_TEXT:
			evtSpin.SetId(ID_EDITORDERREPLACE_NEWVOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
			break;
		default:
			evt.Skip();
			return;
			break;
		}
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		switch(evt.GetId()) {
		case ID_EDITORDERREPLACE_NEWPRICE_TEXT:
			evtSpin.SetId(ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_EDITORDERREPLACE_NEWVOLUME_TEXT:
			evtSpin.SetId(ID_EDITORDERREPLACE_NEWVOLUME_SPINCTRL);
			OnVolumeSpinUp(evtSpin);
			return;
			break;
		default:
			evt.Skip();
			return;
			break;
		}
	}
	else {
		evt.Skip();
	}
}

void CEditOrderReplaceDailog::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
    SetPrice(fPrice+m_fPriceTick);
}

void CEditOrderReplaceDailog::OnPriceSpinDown(wxSpinEvent& event)
{
    double price=GetPrice();
    SetPrice(price-m_fPriceTick);
}

void CEditOrderReplaceDailog::OnPrice2SpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice2();
    SetPrice2(fPrice+m_fPriceTick);
}

void CEditOrderReplaceDailog::OnPrice2SpinDown(wxSpinEvent& event)
{
    double price=GetPrice2();
    SetPrice2(price-m_fPriceTick);
}

void CEditOrderReplaceDailog::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void CEditOrderReplaceDailog::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}
