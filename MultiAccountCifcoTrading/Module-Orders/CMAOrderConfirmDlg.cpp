
#include "stdafx.h"
#include "CMAOrderConfirmDlg.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "..\inc\Module-Misc\GlobalConfigMgr.h"
#include "OrderInsertPanel.h"
#include "MultiAccountCore/MultiAccountCore.h"

using namespace util;

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(CMAOrderConfirmDlg, wxDialog)
	EVT_CLOSE(OnClose)
	EVT_CHAR_HOOK(OnHookKeyPress)
	EVT_BUTTON(ID_MAORDERCONFIRMDLG_YES, OnBtnYes)
	EVT_BUTTON(ID_MAORDERCONFIRMDLG_NO, OnBtnNo)
	EVT_BUTTON(ID_MAORDERCONFIRMDLG_CANCEL, OnBtnCancel)
    EVT_TIMER(ID_MAORDERCONFIRMDLG_TIMER, OnTimer)
	EVT_SPIN_UP(ID_MAORDERCONFIRMDLG_NEWPRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_MAORDERCONFIRMDLG_NEWPRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_MAORDERCONFIRMDLG_NEWVOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_MAORDERCONFIRMDLG_NEWVOLUME_SPINCTRL, OnVolumeSpinDown)
	EVT_TEXT(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT, OnPriceTextChanged)
	EVT_TEXT(ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT, OnVolumeTextChanged)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, OnItemClicked)
	EVT_LIST_EXT_COL_CLICK(ID_MAORDERCONFIRMDLG_ACCOUNT_LIST, OnColClick)	
END_EVENT_TABLE()

CMAOrderConfirmDlg::CMAOrderConfirmDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(320,255)),
m_timer(this,ID_MAORDERCONFIRMDLG_TIMER),
m_fPriceTick(0.0f)
{
	DIALOG_CREATE_LOG();
	m_fUpperLimitPrice = util::GetDoubleInvalidValue();
	m_fLowerLimitPrice = util::GetDoubleInvalidValue();
	m_fPriceTick=0.0;

	m_bIsLockForSetChange = FALSE;
	m_bHasSent = FALSE;
	m_bUserChgStraNum = FALSE;
	m_bUserChgAccounts = FALSE;

	m_pMessageText = NULL;
	m_pCmbStrategy = NULL;
	m_pwxExtListCtrl = NULL;
	m_pOwnerPanel = NULL;

	AUTO_SETSIZE();
	CreateGUIControls();
	Init();
	SetIcon(wxNullIcon);
	Center();
}

CMAOrderConfirmDlg::~CMAOrderConfirmDlg()
{
}

int CMAOrderConfirmDlg::GetRowFromList(wxExtListCtrl* pwxListCtrl, int nID)
{
	if(pwxListCtrl==NULL)
		return -1;

	for(int i=0; i<pwxListCtrl->GetItemCount(); i++) {
		if(pwxListCtrl->GetItemData(i)==nID)
			return i;
	}
	return -1;
}

int CMAOrderConfirmDlg::GetIDFromAccount(std::string strAccount)
{
	CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();
	if(pMACore==NULL) 
		return -1;

	INT nID=0;
	int nIndex=-1;
	if((nIndex = pMACore->FindTradeAccount(strAccount))>=0) {
		const CTPTradeAccount* pAccount=pMACore->GetTradeAccount(nIndex);
		if(pAccount!=NULL) {
			return pAccount->t.nID;//获取ID
		}
	}
	return -1;
}

void CMAOrderConfirmDlg::CreateGUIControls()
{

	SetBackgroundColour(DEFAULT_COLOUR);

	wxFont oldfont=GetFont();
	//oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	wxPoint posInit(10, 10);
	int nFW = 7, nFH = 14;
	int nVH = nFH+4+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;
	wxString strText;
	int nAccountLine = 0;

	ctlPos = posInit;
	
	ctlSz.Set(nFW*60, nFH*3+20);
	m_pMessageText = new wxTextCtrl(this, ID_MAORDERCONFIRMDLG_MESSAGE_TEXT, 
			wxEmptyString, posInit, ctlSz, wxTE_READONLY|wxTE_MULTILINE);
	
	ctlPos.y+=ctlSz.y+5;
	ctlSz.Set(nFW*8, nFH);
	wxStaticText* pStatic1 = new wxStaticText(this, ID_MAORDERCONFIRMDLG_NEWPRICE_STATIC, 
			LOADSTRING(修改价格), wxPoint(ctlPos.x, ctlPos.y+3), ctlSz, wxST_NO_AUTORESIZE);
	
	ctlPos.x += ctlSz.x;
	CreateFloatSpinCtrl_Price(ctlPos);

	ctlPos.x += (70+10);
    ctlSz.Set(nFW*12, nFH);
	wxStaticText* pStatic2 = new wxStaticText(this, ID_MAORDERCONFIRMDLG_NEWVOLUME_STATIC, 
			LOADSTRING(修改基数手数), wxPoint(ctlPos.x, ctlPos.y+3), ctlSz, wxST_NO_AUTORESIZE);
	
	ctlPos.x += ctlSz.x;
	CreateFloatSpinCtrl_Volume(ctlPos);
	
	ctlPos.x = posInit.x;
	ctlPos.y += nVH+5;
	
	ctlSz.Set(nFW*12, nFH);
	wxStaticText* pStatic3 = new wxStaticText(this, ID_MAORDERCONFIRMDLG_STRATEGY_STATIC, 
			LOADSTRING(临时策略修改), wxPoint(ctlPos.x, ctlPos.y+3), ctlSz, wxST_NO_AUTORESIZE);

	wxArrayString strArrStrategy;
	strArrStrategy.Add("下单板指定手数方式");
	strArrStrategy.Add("基数乘数方式");
	strArrStrategy.Add("总数分配");
	strArrStrategy.Add("按资金向首账户对齐");
	strArrStrategy.Add("按账户规模分配手数");
	strArrStrategy.Add("策略版自定义手数方式");
	//strArrStrategy.Add("多账户自动开平");
	
	ctlPos.x += ctlSz.x;
	ctlSz.Set(nFW*20, 20);
	m_pCmbStrategy = new wxOwnerDrawnComboBox(this, ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX, 
			LOADSTRING(下单板指定手数方式), ctlPos, ctlSz, strArrStrategy, wxCB_READONLY);
	m_pCmbStrategy->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CMAOrderConfirmDlg::OnCmbStrategy), NULL, this);

	ctlPos.x = posInit.x;
	ctlPos.y += nVH+5;

	ctlSz.Set(nFW*60, 150);
	m_pwxExtListCtrl = new wxExtListCtrl(this, ID_MAORDERCONFIRMDLG_ACCOUNT_LIST, 
			ctlPos, ctlSz, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxBORDER_SIMPLE|wxLCEXT_MASK_SORT, 
			wxDefaultValidator, wxEmptyString);
	m_pwxExtListCtrl->Connect(wxEVT_COMMAND_LIST_EXT_CHECKBOX_CHANGED, 
			wxExtListEventHandler(CMAOrderConfirmDlg::OnItemChanged), NULL, this); 

	SetSize(445, 350);

	wxSize dlgClientSize = GetClientSize();
	int nFirstLeft = dlgClientSize.x - 2*(65)-5;
	int nNextLeft = nFirstLeft;

	ctlPos = wxPoint(nNextLeft, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_MAORDERCONFIRMDLG_YES, LOADSTRING(下单), ctlPos, ctlSz, 0 );
	itemTemp->SetFocus();

	nNextLeft += 65;

	ctlPos = wxPoint(nNextLeft, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_MAORDERCONFIRMDLG_NO, LOADSTRING(取消), ctlPos, ctlSz, 0 );
	
	ctlPos = wxPoint(10, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_MAORDERCONFIRMDLG_CANCEL, LOADSTRING(全撤), ctlPos, ctlSz, 0 );
	itemTemp->Enable(false);

	//int accindex=0;
	//wxAcceleratorEntry entries[12];
	//entries[accindex++].Set(wxACCEL_NORMAL, (int)'Y', ID_ORDERCONFIRMDLG_YES);
	//entries[accindex++].Set(wxACCEL_NORMAL, (int)'N', ID_ORDERCONFIRMDLG_NO);
	//entries[accindex++].Set(wxACCEL_NORMAL, (int)'y', ID_ORDERCONFIRMDLG_YES);
	//entries[accindex++].Set(wxACCEL_NORMAL, (int)'n', ID_ORDERCONFIRMDLG_NO);

	//entries[accindex++].Set(wxACCEL_SHIFT, (int)'Y', ID_ORDERCONFIRMDLG_YES);
	//entries[accindex++].Set(wxACCEL_SHIFT, (int)'N', ID_ORDERCONFIRMDLG_NO);
	//entries[accindex++].Set(wxACCEL_SHIFT, (int)'y', ID_ORDERCONFIRMDLG_YES);
	//entries[accindex++].Set(wxACCEL_SHIFT, (int)'n', ID_ORDERCONFIRMDLG_NO);

	//entries[accindex++].Set(wxACCEL_CTRL, (int)'Y', ID_ORDERCONFIRMDLG_YES);
	//entries[accindex++].Set(wxACCEL_CTRL, (int)'N', ID_ORDERCONFIRMDLG_NO);
	//entries[accindex++].Set(wxACCEL_CTRL, (int)'y', ID_ORDERCONFIRMDLG_YES);
	//entries[accindex++].Set(wxACCEL_CTRL, (int)'n', ID_ORDERCONFIRMDLG_NO);
	//wxAcceleratorTable accel(12, entries);
	//this->SetAcceleratorTable(accel);

	//m_TabOrderVector.clear();
	////tab order
	//for(int i=0; i<m_nCmbOrderCount; i++) {
	//	m_TabOrderVector.push_back(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+i*2);
	//	m_TabOrderVector.push_back(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+i*2);
	//}

	//m_TabOrderVector.push_back(ID_ORDERCONFIRMDLG_YES);
	//m_TabOrderVector.push_back(ID_ORDERCONFIRMDLG_NO);

}

wxTextCtrl* CMAOrderConfirmDlg::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(54, 20);
	// wxCheckedListCtrl有相关的实现
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, 
			ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(CMAOrderConfirmDlg::OnPriceClicked), NULL, this);
    //itemTextCtrl3->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(COrderConfirmPanel::OnPriceTextFocus), NULL, this);

	ctlPos.x += 54;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, 
			ID_MAORDERCONFIRMDLG_NEWPRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* CMAOrderConfirmDlg::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(54, 20);
    wxTextCtrl* itemTextCtrl3 = new wxTextCtrl(this, 
				ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);
	itemTextCtrl3->SetMaxLength(4);

	itemTextCtrl3->Connect(wxEVT_LEFT_UP, 
			wxMouseEventHandler(CMAOrderConfirmDlg::OnVolumeClicked), NULL, this);

	ctlPos.x += 54;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_MAORDERCONFIRMDLG_NEWVOLUME_SPINCTRL, 
			ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 9999);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

bool CMAOrderConfirmDlg::Init()
{
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);
	
	//设置LIST界面属性
	m_HeadBackgroundColor = wxColor(RGB(128, 128, 128));
	m_HeadColor = wxColor(RGB(0, 0, 0));
	m_BackgroundColor = wxColor(RGB(255, 255, 255));
	m_TextColor = wxColor(RGB(0, 0, 0));
	m_Font.SetNativeFontInfoUserDesc("宋体"); 
	
	m_pwxExtListCtrl->SetHeaderBackgroundColour(m_HeadBackgroundColor);
	m_pwxExtListCtrl->SetHeaderForegroundColour(m_HeadColor);
	m_pwxExtListCtrl->SetBackgroundColour(m_BackgroundColor);
	m_pwxExtListCtrl->SetForegroundColour(m_TextColor);
	m_pwxExtListCtrl->SetFont(m_Font);
	m_pwxExtListCtrl->SetOddRowColour(RGB(224, 224, 224));
	m_pwxExtListCtrl->SetEvenRowColour(RGB(255, 255, 255));

	wxExtListItem col;
	int nColIndex = 0;

	col.SetText(LOADSTRING(有效));
	col.SetWidth(30);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;
	m_pwxExtListCtrl->SetColumnCtrlType(0, COLUMN_CTRL_TYPE_CHECKBOX);

	col.SetText(LOADSTRING(ID));
	col.SetWidth(30);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;

	col.SetText(LOADSTRING(投资者));
	col.SetWidth(70);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;

	col.SetText(LOADSTRING(撤原单));
	col.SetWidth(66);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;

	col.SetText(LOADSTRING(开仓));
	col.SetWidth(66);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;

	col.SetText(LOADSTRING(平今));
	col.SetWidth(66);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;

	col.SetText(LOADSTRING(平昨));
	col.SetWidth(66);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;

	col.SetText(LOADSTRING(计算参数));
	col.SetWidth(66);
	col.SetTextColour(RGB(0, 0, 0));
	m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
	nColIndex++;


	//m_iRowCnt=0;
	//CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	//wxASSERT(pCfgMgr);
	//m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_POSITION);
	//wxASSERT(m_pListCfg);

	////设置LIST界面属性
	//m_HeadBackgroundColor = wxColor(m_pListCfg->HeadBackgroundColor);
	//m_HeadColor = wxColor(m_pListCfg->HeadColor);
	//m_BackgroundColor = wxColor(m_pListCfg->BackgroundColor);
	//m_TextColor = wxColor(m_pListCfg->TextColor);
	//m_Font.SetNativeFontInfoUserDesc(m_pListCfg->szFont); 

	//m_pwxExtListCtrl->SetHeaderBackgroundColour(m_HeadBackgroundColor);
	//m_pwxExtListCtrl->SetHeaderForegroundColour(m_HeadColor);
	//m_pwxExtListCtrl->SetBackgroundColour(m_BackgroundColor);
	//m_pwxExtListCtrl->SetForegroundColour(m_TextColor);
	//m_pwxExtListCtrl->SetFont(m_Font);
	//m_pwxExtListCtrl->SetOddRowColour(m_pListCfg->OddLineBackgroundColor);
	//m_pwxExtListCtrl->SetEvenRowColour(m_pListCfg->EvenLineBackgroundColor);

	//wxExtListItem col;
	//int nColIndex = 0;

	//nColCount = pCfgMgr->GetListVisibleColCount(GID_QUERY_ORDER_POSITION);

	//m_FieldID2ColumnID.clear();
	//m_FieldAttr.clear();
	//for(int i = 0;  i < nColCount; i++) {
	//	//LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_POSITION, i);
	//	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSITION, i);
	//	wxASSERT(pColCfg);
	//	if(pColCfg->id<ORDERPOSITION_MIN || pColCfg->id>ORDERPOSITION_MAX)
	//		continue;

	//	//"合约" "报单编号"栏，左对齐
	//	if( (pColCfg->id == ORDERPOSITION_InstrumentID) || (pColCfg->id == ORDERPOSITION_ConditionID) )
	//		col.SetAlign(wxLISTEXT_FORMAT_LEFT);
	//	else//其余的，右对齐
	//		col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

 //       if(pColCfg->Visibility == 0)
 //       col.SetWidth(0);
 //       else
 //       col.SetWidth(pColCfg->Width);///* /
 //       if(pColCfg->Visibility != 0) {		
 //           col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_POSITION,pColCfg->id));
 //           col.SetWidth(pColCfg->Width);
	//		col.SetTextColour(pColCfg->TextColor);
 //           m_FieldID2ColumnID[pColCfg->id] = nColIndex;
 //           m_FieldAttr[pColCfg->id] = *pColCfg;

	//		col.SetColData(pColCfg->id);
 //           m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
 //           nColIndex++;
 //       }
 //   }
	PROCESS_LANGUAGE_EVENT();
    
	//wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_QUERYPOSITION_BUTTONQUERY);
	//OnButtonQuery(evt);

	return true;
}

void CMAOrderConfirmDlg::SetStrategyNumber(int straNum)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX,this);
	if(pComboBox->IsEnabled())
		pComboBox->SetSelection(straNum);
}

int CMAOrderConfirmDlg::GetStrategyNumber()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX,this);
	return (StrategyItem)(pComboBox->GetSelection());
}

wxString CMAOrderConfirmDlg::GetStrategyNumberText()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX,this);
	return wxString(pComboBox->GetValue());
}

void CMAOrderConfirmDlg::SetPriceText(wxString& strPrice)
{
	//m_bIsLockForSetChange = TRUE;
	wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT, this);
	window->SetLabel(strPrice);
	//m_bIsLockForSetChange = FALSE;
}

wxString CMAOrderConfirmDlg::GetPriceText()
{
	wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT, this);
	return window->GetLabel();
}

void CMAOrderConfirmDlg::SetPrice(double fPrice)
{
	//m_bIsLockForSetChange = TRUE;
	wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
	//m_bIsLockForSetChange = FALSE;
}

double CMAOrderConfirmDlg::GetPrice()
{
	wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void CMAOrderConfirmDlg::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT,this);
	window->SetLabel(strText);
}

int CMAOrderConfirmDlg::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

wxString CMAOrderConfirmDlg::GetQtyText()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT,this);
	return window->GetLabel();
}

void CMAOrderConfirmDlg::ResetID2Account(map<string,bool>& accsLogon)
{
	INT nID=0;
	map<string,bool>::iterator it = accsLogon.begin();

	m_mapID2Account.clear();
	for(; it!=accsLogon.end(); it++) {
		if((nID = GetIDFromAccount(std::string(it->first)))>=0) 
			m_mapID2Account.insert(std::pair<int, std::string>(
					nID, std::string(it->first)));
	}
}

void CMAOrderConfirmDlg::ResetMAOrderEntity(CMAOrderEntity* pMAOrderEntity, 
											BOOL bDeleteAll, BOOL bWriteToUI) 
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	if(pMAOrderEntity==NULL)
		return;

	m_pMAOrderEntity = pMAOrderEntity;

	COrderAtomEntity::STDORDERREQ stdOrderReq = m_pMAOrderEntity->GetStdOrderReq();

    PlatformStru_InstrumentInfo insInfo;
	ZeroMemory(&insInfo, sizeof(insInfo));

	IPlatformSingleSvr* pSvr=NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
	else
		pSvr=DEFAULT_SVR();
	if(pSvr==NULL)
		return;

	if(pSvr->GetInstrumentInfo(string(stdOrderReq.csInstrumentID), insInfo)==0)
		m_fPriceTick = insInfo.PriceTick;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
    if(pSvr->GetQuotInfo(string(stdOrderReq.csInstrumentID), field)==0)
	{
		m_fUpperLimitPrice = field.UpperLimitPrice;
		m_fLowerLimitPrice = field.LowerLimitPrice;
	}
	else 
	{
		m_fUpperLimitPrice = util::GetDoubleInvalidValue();
		m_fLowerLimitPrice = util::GetDoubleInvalidValue();
	}

	wxString strTitle;
	strTitle.Printf("确认 下单 %s; 价格: %s; 买卖: %s", 
			stdOrderReq.csInstrumentID, 
			stdOrderReq.bIsMarket ? LOADSTRING(市价) : 
			Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(),
			stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖));
	SetTitle(strTitle);
	//SetTitle("多帐号下单确认");

	if(bWriteToUI) {
		m_bIsLockForSetChange = TRUE;
		SetStrategyNumber(pMAOrderEntity->GetStrategyItem());
		if(stdOrderReq.bIsMarket)
			SetPriceText(wxString(LOADSTRING(市价)));
		else
			SetPrice(stdOrderReq.fPrice);
		SetQty(stdOrderReq.nVolume);
		m_bIsLockForSetChange = FALSE;
	}

	ShowMAOrderEntity(pMAOrderEntity, FALSE, bDeleteAll);

}

void CMAOrderConfirmDlg::ResetOrderStatus(DataRtnOrder* rtnOrderStatus)
{
	ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
}

void CMAOrderConfirmDlg::ResetOrderStatus(DataRspOrderInsert* rspOrderInsert)
{
	ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
}

void CMAOrderConfirmDlg::ResetOrderStatus(DataRspOrderAction1* rspOrderAction)
{
	ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
}

void CMAOrderConfirmDlg::ResetOrderStatus(DataRspOrderAction2* rspOrderAction)
{
	ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
}

void CMAOrderConfirmDlg::ShowMAOrderEntity(CMAOrderEntity* pMAOrderEntity, BOOL bHasSent, BOOL bDeleteAll)
{
	if(pMAOrderEntity==NULL)
		return;
	wxString strText, strItem;
	int nSumOpenVolume=0, nSumTodayVolume=0, nSumYdVolume=0;
	COrderAtomEntity::STDORDERREQ stdOrderReq = m_pMAOrderEntity->GetStdOrderReq();
	
	m_pMAOrderEntity->GetVolumeSum(nSumOpenVolume, nSumTodayVolume, nSumYdVolume);
	if(!bHasSent) {
		strItem.Printf("%s: 价格 %s, %s, %s%d手\r\n", stdOrderReq.csInstrumentID, 
				stdOrderReq.bIsMarket ? LOADSTRING(市价) : Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(), 
				stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖), LOADSTRING(开仓), nSumOpenVolume);
		strText += strItem;
		strItem.Printf("%s: 价格 %s, %s, %s%d手\r\n", stdOrderReq.csInstrumentID, 
				stdOrderReq.bIsMarket ? "市价" : Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(), 
				stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖), LOADSTRING(平昨), nSumYdVolume);
		strText += strItem;
		strItem.Printf("%s: 价格 %s, %s, %s%d手\r\n", stdOrderReq.csInstrumentID, 
				stdOrderReq.bIsMarket ? LOADSTRING(市价) : Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(), 
				stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖), LOADSTRING(平今), nSumTodayVolume);
		strText += strItem;
	}
	else {
		if(nSumOpenVolume>0) {
			strItem.Printf("下单 %s: 价格 %s, %s %s %s %d手\r\n", stdOrderReq.csInstrumentID, 
					stdOrderReq.bIsMarket ? LOADSTRING(市价) : Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(), 
					stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖), LOADSTRING(开仓), 
					stdOrderReq.csInstrumentID, nSumOpenVolume);
			strText += strItem;
		}
		if(nSumYdVolume>0) {
			strItem.Printf("下单 %s: 价格 %s, %s %s %s %d手\r\n", stdOrderReq.csInstrumentID, 
					stdOrderReq.bIsMarket ? "市价" : Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(), 
					stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖), LOADSTRING(平昨), 
					stdOrderReq.csInstrumentID, nSumYdVolume);
			strText += strItem;
		}
		if(nSumTodayVolume>0) {
			strItem.Printf("下单 %s: 价格 %s, %s %s %s %d手\r\n", stdOrderReq.csInstrumentID, 
					stdOrderReq.bIsMarket ? LOADSTRING(市价) : Price2String(stdOrderReq.fPrice, m_fPriceTick).c_str(), 
					stdOrderReq.bIsBuy ? LOADSTRING(买) : LOADSTRING(卖), LOADSTRING(平今), 
					stdOrderReq.csInstrumentID, nSumTodayVolume);
			strText += strItem;
		}
	}
	if(m_pMessageText!=NULL)
		m_pMessageText->SetValue(strText);

	ShowAllAccount(bHasSent, bDeleteAll);
}

void CMAOrderConfirmDlg::ShowAllAccount(BOOL bHasSent, BOOL bDeleteAll)
{
	if(m_pMAOrderEntity==NULL)
		return;
	
	int iRowNo = 0, iCol = 0, iSumCol = 0;

	if(bDeleteAll) {
		m_pwxExtListCtrl->DeleteAllItems();
		m_mapKey2Row.clear();
		m_mapRow2Key.clear();
	}

	map<CSAOrderEntity*, OrderInsertUIItem> mapSAOrder = m_pMAOrderEntity->GetSAOrderEntities();
	map<CSAOrderEntity*, OrderInsertUIItem>::iterator it = mapSAOrder.begin();
	std::map<std::string, int>::iterator itKey;

	iRowNo = -1;

	for(itKey = m_mapKey2Row.begin(); itKey!=m_mapKey2Row.end(); itKey++) {
		ShowZeroOneAccount(itKey->second);
	}
	
	for(; it!=mapSAOrder.end(); ++it) {
		if(!bDeleteAll) {
			iRowNo = GetRowFromList(m_pwxExtListCtrl, 
					GetIDFromAccount(std::string(it->second.Account)));
			if(iRowNo<0)
				continue;
		}
		else
			++iRowNo;
		if(bDeleteAll) {
			if(ShowOneAccount(iRowNo, it->first, it->second, TRUE, bHasSent) < 0) {
				--iRowNo;
				continue;
			}
			else {
				m_mapKey2Row[std::string(it->second.Account)] = iRowNo;
				m_mapRow2Key[iRowNo] = std::string(it->second.Account);
			}
		}
		else {
			ShowOneAccount(iRowNo, it->first, it->second, FALSE, bHasSent);
		}
	}

}

int CMAOrderConfirmDlg::ShowOneAccount(int iRowNo, CSAOrderEntity* pSAEntity, 
									   OrderInsertUIItem& tOrderUIItem, 
									   BOOL bIsNew, BOOL bHasSent)
{
	if(pSAEntity==NULL)
		return -1;

	if(m_pwxExtListCtrl->GetColumnCount() == 0)
		return -1;

	int iCol = 0;
	wxString newText;//单元格的文本内容
	wxString strValue;
	wxExtListItem item;	
	
	INT nID=0;
	wxString strNickName;
	{
	int nIndex=-1;
	CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();

	if(pMACore!=NULL) {
		if((nIndex = pMACore->FindTradeAccount(tOrderUIItem.Account))>=0) {
			const CTPTradeAccount* pAccount=pMACore->GetTradeAccount(nIndex);
			if(pAccount!=NULL) {
				nID = pAccount->t.nID;//获取ID
				strNickName = pAccount->t.szNickName;//投资者名称
			}
		}
	}
	}

	item.Clear();

	iCol = 0;

	if(bIsNew) {
		m_pwxExtListCtrl->InsertItem(iRowNo, item);
		m_pwxExtListCtrl->SetItemData(iRowNo, (long)nID);
		item.SetId(iRowNo);	//设置行号
	}
	else {
		item.SetId(iRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}

	// 设置listCtrl的checkbox状态
		iCol = 0;//	iCol=it->second;	
		if(m_pwxExtListCtrl->GetColumnCtrlType(0)==COLUMN_CTRL_TYPE_NORMAL) 
			newText.Printf("√");
		else
			newText.Printf(" ");
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
		m_pwxExtListCtrl->SetItemTextColour(item, wxColour(RGB(0, 128, 64)));
	if(m_pwxExtListCtrl->GetColumnCtrlType(0)==COLUMN_CTRL_TYPE_CHECKBOX) {
		map<string, bool>::iterator it = m_mapAccounts.find(pSAEntity->GetUserID());
		if(it!=m_mapAccounts.end())
			m_pwxExtListCtrl->SetItemChecked(iRowNo, it->second);
	}

	//std::map<int, long> ::iterator it;
	//it = m_FieldID2ColumnID.find(ORDERPOSITION_PositionID);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 1;//	iCol=it->second;	
		newText.Printf("%d", nID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_OpenID);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 2;//	iCol=it->second;	
		newText.Printf("%s", strNickName.c_str());
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	int nWaitVolume=0, nRealVolume=0;
	pSAEntity->GetCancelVolumeSum(nWaitVolume, nRealVolume);
	//it = m_FieldID2ColumnID.find(ORDERPOSITION_State);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 3;//	iCol=it->second;
		newText = "-";
		if(nWaitVolume>0)
		{
			if(!bHasSent)
				newText.Printf("%d", nWaitVolume);
			else
				if(pSAEntity->Status_DoCancel()==CSAOrderEntity::conIsFail)
					newText.Printf("撤单失败");
				else
					newText.Printf("已撤%d/%d", nRealVolume, nWaitVolume);
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	int nSumOpenVolume = 0, nSumTodayVolume = 0, nSumYdVolume = 0;
	int nOpenVolume_Trade = 0, nTodayVolume_Trade = 0, nYdVolume_Trade = 0;
	pSAEntity->GetVolumeSum(nSumOpenVolume, nSumTodayVolume, nSumYdVolume);
	pSAEntity->GetVolumeSum_Trade(nOpenVolume_Trade, nTodayVolume_Trade, nYdVolume_Trade);

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 4;//	iCol=it->second;
		newText = "-";
		if(nSumOpenVolume>0) 
		{
			if(!bHasSent)
				newText.Printf("%d", nSumOpenVolume);
			else
				if(pSAEntity->GetRunStatus(conOCOpen)==CSAOrderEntity::conIsFail)
					//newText.Printf("失败%d", nSumOpenVolume);
					newText.Printf("失败");
				else if(pSAEntity->GetRunStatus(conOCOpen)==COrderAtomEntity::conIsCancelSuccess)
					if(nOpenVolume_Trade==0)
						newText.Printf("撤单%d", nSumOpenVolume);
					else
						newText.Printf("撤单%d成交%d", nSumOpenVolume-nOpenVolume_Trade, nOpenVolume_Trade);
				else 
				{
					if(nSumOpenVolume-nOpenVolume_Trade>0)
						newText.Printf("未成%d/%d", nSumOpenVolume-nOpenVolume_Trade, nSumOpenVolume);
					else
						newText.Printf("全成%d", nSumOpenVolume);
				}
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 5;//	iCol=it->second;
		newText = "-";
		if(nSumTodayVolume>0) 
		{
			if(!bHasSent)
				newText.Printf("%d", nSumTodayVolume);
			else
				if(pSAEntity->GetRunStatus(conOCClodeToday)==CSAOrderEntity::conIsFail)
					//newText.Printf("失败%d", nSumOpenVolume);
					newText.Printf("失败");
				else if(pSAEntity->GetRunStatus(conOCClodeToday)==COrderAtomEntity::conIsCancelSuccess)
					if(nTodayVolume_Trade==0)
						newText.Printf("撤单%d", nSumTodayVolume);
					else		
						newText.Printf("撤单%d成交%d", nSumTodayVolume-nTodayVolume_Trade, nTodayVolume_Trade);
				else 
				{
					if(nSumTodayVolume-nTodayVolume_Trade>0)
						newText.Printf("未成%d/%d", nSumTodayVolume-nTodayVolume_Trade, nSumTodayVolume);
					else
						newText.Printf("全成%d", nSumTodayVolume);
				}
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 6;//	iCol=it->second;
		newText = "-";
		if(nSumYdVolume>0) 
		{
			if(!bHasSent)
				newText.Printf("%d", nSumYdVolume);
			else
				if(pSAEntity->GetRunStatus(conOCClose)==COrderAtomEntity::conIsFail)
					//newText.Printf("失败%d", nSumOpenVolume);
					newText.Printf("失败");
				else if(pSAEntity->GetRunStatus(conOCClose)==COrderAtomEntity::conIsCancelSuccess)
					if(nYdVolume_Trade==0)
						newText.Printf("撤单%d", nSumYdVolume);
					else
						newText.Printf("撤单%d成交%d", nSumYdVolume-nYdVolume_Trade, nYdVolume_Trade);
				else 
				{
					if(nSumYdVolume-nYdVolume_Trade>0)
						newText.Printf("未成%d/%d", nSumYdVolume-nYdVolume_Trade, nSumYdVolume);
					else
						newText.Printf("全成%d", nSumYdVolume);
				}
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 7;//	iCol=it->second;
		if(util::isInvalidValue(tOrderUIItem.VolumeFlag))
			newText = "-";
		else
			newText.Printf("%s", Double2String(tOrderUIItem.VolumeFlag).c_str());
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	return 0;
}

int CMAOrderConfirmDlg::ShowZeroOneAccount(int iRowNo)
{

	if(m_pwxExtListCtrl->GetColumnCount() == 0)
		return -1;
	
	int iCol = 0;
	wxString newText;//单元格的文本内容
	wxString strValue;
	wxExtListItem item;	
	
	item.Clear();

	iCol = 0;

	item.SetId(iRowNo);	//设置行号
	m_pwxExtListCtrl->GetItem(item);

	// 设置listCtrl的checkbox状态
	//if(m_pwxExtListCtrl->GetColumnCtrlType(0)==COLUMN_CTRL_TYPE_NORMAL) {
		iCol = 0;//	iCol=it->second;	
		newText.Printf("");
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}
	//else 
	if(m_pwxExtListCtrl->GetColumnCtrlType(0)==COLUMN_CTRL_TYPE_CHECKBOX)
		m_pwxExtListCtrl->SetItemChecked(iRowNo, false);

	////std::map<int, long> ::iterator it;
	////it = m_FieldID2ColumnID.find(ORDERPOSITION_PositionID);
	////if(it!=m_FieldID2ColumnID.end()) {
	//	iCol = 1;//	iCol=it->second;	
	//	//newText.Printf("%d", tOrderUIItem.ID);
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	////}

	////it = m_FieldID2ColumnID.find(ORDERPOSITION_OpenID);
	////if(it!=m_FieldID2ColumnID.end()) {
	//	iCol = 2;//	iCol=it->second;	
	//	newText.Printf("%s", pSAEntity->GetUserID().c_str());
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	////}

	//int nWaitVolume=0, nRealVolume=0;
	//pSAEntity->GetCancelVolumeSum(nWaitVolume, nRealVolume);
	//it = m_FieldID2ColumnID.find(ORDERPOSITION_State);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 3;//	iCol=it->second;
		newText = "-";
		//if(nWaitVolume>0)
		//{
		//	if(!bHasSent)
		//		newText.Printf("%d", nWaitVolume);
		//	else
		//		newText.Printf("已撤%d/%d", nRealVolume, nWaitVolume);
		//}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//int nSumOpenVolume = 0, nSumTodayVolume = 0, nSumYdVolume = 0;
	//int nOpenVolume_Wait = 0, nTodayVolume_Wait = 0, nYdVolume_Wait = 0;
	//pSAEntity->GetVolumeSum(nSumOpenVolume, nSumTodayVolume, nSumYdVolume);
	//pSAEntity->GetVolumeSum_Wait(nOpenVolume_Wait, nTodayVolume_Wait, nYdVolume_Wait);

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 4;//	iCol=it->second;
		newText = "-";
		//if(nSumOpenVolume>0) 
		//{
		//	if(!bHasSent)
		//		newText.Printf("%d", nSumOpenVolume);
		//	else
		//		if(pSAEntity->Status_DoInsert()==CSAOrderEntity::conIsCancel)
		//			newText.Printf("撤单%d/%d", nOpenVolume_Wait, nSumOpenVolume);
		//		else
		//			newText.Printf("未成%d/%d", nOpenVolume_Wait, nSumOpenVolume);
		//}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 5;//	iCol=it->second;
		newText = "-";
		//if(nSumTodayVolume>0) 
		//{
		//	if(!bHasSent)
		//		newText.Printf("%d", nSumTodayVolume);
		//	else
		//		if(pSAEntity->Status_DoInsert()==CSAOrderEntity::conIsCancel)
		//			newText.Printf("撤单%d/%d", nTodayVolume_Wait, nSumTodayVolume);
		//		else
		//			newText.Printf("未成%d/%d", nTodayVolume_Wait, nSumTodayVolume);
		//}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	//if(it!=m_FieldID2ColumnID.end()) {
		iCol = 6;//	iCol=it->second;
		newText = "-";
		//if(nSumYdVolume>0) 
		//{
		//	if(!bHasSent)
		//		newText.Printf("%d", nSumYdVolume);
		//	else
		//		if(pSAEntity->Status_DoInsert()==CSAOrderEntity::conIsCancel)
		//			newText.Printf("撤单%d/%d", nYdVolume_Wait, nSumYdVolume);
		//		else
		//			newText.Printf("未成%d/%d", nYdVolume_Wait, nSumYdVolume);
		//}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	//}

	return 0;
}

wxString CMAOrderConfirmDlg::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
{
	wxString strText;
	if(pListCtrl==NULL)
		return strText;
	if(nRow<0 || nRow>=pListCtrl->GetItemCount())
		return strText;
	int nCol = 0;
	wxString strItem;
	wxExtListItem itemCol;
	for(; nCol<pListCtrl->GetColumnCount(); nCol++) {
		pListCtrl->GetColumn(nCol, itemCol);
		if(nCol==0) {
			if(m_pwxExtListCtrl->GetColumnCtrlType(0)==COLUMN_CTRL_TYPE_NORMAL)
				strItem.Printf("%s=[%s]%s", itemCol.GetText().c_str(), 
						pListCtrl->GetSubItemText(nRow, nCol).c_str(),
						(nCol<pListCtrl->GetColumnCount()-1?", ":""));
			else if(m_pwxExtListCtrl->GetColumnCtrlType(0)==COLUMN_CTRL_TYPE_CHECKBOX)
				strItem.Printf("%s=[%s]%s", itemCol.GetText().c_str(), 
						(pListCtrl->GetItemChecked(nRow)?"√":"　"),
						(nCol<pListCtrl->GetColumnCount()-1?", ":""));
		}
		else
			strItem.Printf("%s=[%s]%s", itemCol.GetText().c_str(), 
					pListCtrl->GetSubItemText(nRow, nCol).c_str(),
					(nCol<pListCtrl->GetColumnCount()-1?", ":""));
		strText+=strItem;
	}
	return strText;
}

wxString CMAOrderConfirmDlg::GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl)
{
	wxString strText;
	if(pListCtrl==NULL)
		return strText;
	int nRow = 0;
	for(; nRow<pListCtrl->GetItemCount(); nRow++) {
		strText+=GetListCtrlRowLog(pListCtrl, nRow);
		if(nRow<pListCtrl->GetItemCount()-1)
			strText+="\n";
	}
	return strText;
}

wxString CMAOrderConfirmDlg::GetConfirmStatusLog()
{
	wxString strText;
	strText += "多帐号策略=[";
	strText += GetStrategyNumberText();
	strText += "], ";
	strText += "价格=[";
	strText += GetPriceText();
	strText += "], ";
	strText += "默认手数=[";
	strText += GetQtyText();
	strText += "]\n";
	if(m_pMessageText) {
		strText+=m_pMessageText->GetValue();
		strText+="\n";
	}
	strText+=GetListCtrlAllRowLog(m_pwxExtListCtrl);
	return strText;
}

void CMAOrderConfirmDlg::OnBtnYes(wxCommandEvent& event)
{
	USERLOG_INFO("多帐号确认对话框，用户单击按钮[确认]确认\n\"\n%s\n\"", 
			GetConfirmStatusLog().c_str());

	if(m_pMAOrderEntity==NULL)
		return;
	if(g_pPlatformMgr==NULL)
		return;

	COrderAtomEntity::STDORDERREQ stdReq = m_pMAOrderEntity->GetStdOrderReq();

	double fPrice = stdReq.fPrice;
	// 判读是否符合最小变动价位
	if(CheckSumPrice(fPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetCurrentPlatformSvr();
	else
		pSvr = DEFAULT_SVR();
	if(pSvr!=NULL) {
		PlatformStru_DepthMarketData field;
		ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

		pSvr->GetQuotInfo(stdReq.csInstrumentID, field);

		// 超过涨跌停价提示
		if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
				&& !stdReq.bIsMarket) {
			bool bOverLimit = util::greater(stdReq.fPrice, field.UpperLimitPrice) 
				|| util::less(stdReq.fPrice, field.LowerLimitPrice);
			if(bOverLimit) {
				wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
						wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
				if(dialog.ShowModal()!=wxID_YES) 
					return;
			}
		}
	}

	int nErrPos = 0;
	std::string strErrMsg;

	m_bHasSent = TRUE;
	OrderInsertPanel* pPanel = (OrderInsertPanel*)m_pOwnerPanel;
	if(pPanel!=NULL)
		pPanel->LockObject();

	m_pMAOrderEntity->ResetActive(TRUE);
	if(m_pMAOrderEntity->NeedCancelOrder() && !m_pMAOrderEntity->IsRevokeAllOK()) {
		m_pMAOrderEntity->DoRevokeAction(nErrPos, strErrMsg);
		m_pMAOrderEntity->m_dwStartTick = ::GetTickCount();
	}
	else
		m_pMAOrderEntity->DoOrderReq(nErrPos, strErrMsg);

	if(pPanel!=NULL)
		pPanel->UnlockObject();
	
    wxWindow* pWin=NULL; 
	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT,this);
	if(pWin!=NULL)
		pWin->Enable(false);

	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_SPINCTRL,this);
	if(pWin!=NULL)
		pWin->Enable(false);

	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT,this);
	if(pWin!=NULL)
		pWin->Enable(false);

	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_SPINCTRL,this);
	if(pWin!=NULL)
		pWin->Enable(false);

	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX,this);
	if(pWin!=NULL)
		pWin->Enable(false);

	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_YES,this);
	if(pWin!=NULL)
		pWin->Enable(false);

	//pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NO,this);
	//if(pWin!=NULL)
	//	pWin->Enable(false);

	pWin=FindWindowById(ID_MAORDERCONFIRMDLG_CANCEL,this);
	if(pWin!=NULL)
		pWin->Enable(true);

	m_pwxExtListCtrl->SetColumnCtrlType(0, COLUMN_CTRL_TYPE_NORMAL);

	ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
	//m_pwxExtListCtrl->Disable();

	if(m_bUserChgStraNum) { 
		CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();
		if(pMACore!=NULL) {
			pMACore->SetStrategyNum(m_pMAOrderEntity->GetStrategyItem());
		}
		// 需要发一个event到多帐号板块，让其更新界面
	}

	if(m_bUserChgAccounts 
			&& m_pMAOrderEntity->GetMaAutoOper().nOperType==CSAOrderEntity::conOperAuto
			&& !m_pMAOrderEntity->GetMaAutoOper().bIsQuotOrder) {
		std::map<int, std::string>::iterator it;
		vector<string> arrValidAccounts;
		for(int i=0; i<m_pwxExtListCtrl->GetItemCount(); i++) {
			if(m_pwxExtListCtrl->GetItemChecked(i)) {
				it = m_mapID2Account.find(m_pwxExtListCtrl->GetItemData(i));
				if(it!=m_mapID2Account.end()) 
					arrValidAccounts.push_back(it->second);
			}
		}
		CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();
		if(pMACore!=NULL) {
			map<string, bool> accstatus;
			map<string, bool>::iterator it;
			pMACore->GetAccountStatus(&accstatus);
			for(it = accstatus.begin(); it != accstatus.end(); it++) {
				it->second = false;
			}
			for(int i=0; i<arrValidAccounts.size(); i++) {
				it = accstatus.find(arrValidAccounts[i]);
				if(it != accstatus.end()) 
					it->second = true;
			}
			pMACore->SetAccountStatus(&accstatus);
			// 需要发一个event到多帐号板块，让其更新界面
		}
	}
	return;

	//for(int i=0; i<m_nCmbOrderCount; i++) {
	//	if(m_pCmbOrders[i]->bIsParked)
	//		m_poOrderConfirmPanels[i]->FillOrder(m_pCmbOrders[i]->order.parked);
	//	else
	//		m_poOrderConfirmPanels[i]->FillOrder(m_pCmbOrders[i]->order.insert);
	//}

	//wxCheckBox* pWin = (wxCheckBox*)FindWindowById(ID_ORDERCONFIRMDLG_CHECK_CONFIG, this);
 //   CfgMgr* pMgr =  CFG_MGR_DEFAULT();  
 //   if(pMgr != NULL && pWin != NULL) 
	//{
	//	LPORDER_CFG p = pMgr->GetOrderCfg();
	//	p->bOrderConfirm = !pWin->GetValue();
	//}
	//EndModal(wxID_YES);
}

void CMAOrderConfirmDlg::OnBtnNo(wxCommandEvent& event)
{
	//EndModal(wxID_NO);
	Hide();
}

void CMAOrderConfirmDlg::OnBtnCancel(wxCommandEvent& event)
{
	if(m_pMAOrderEntity==NULL)
		return;

	if(m_pMAOrderEntity->SumCanCancel()==0) {
		wxMessageDialog dialog(NULL,LOADSTRING(已无可撤报单),
				LOADSTRING(提示),wxNO_DEFAULT|wxOK|wxICON_WARNING);
		dialog.ShowModal();
		return;
	}
	else {
		wxMessageDialog dialog(NULL,LOADSTRING(是否撤销相关挂单),
				LOADSTRING(提示),wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	int nErrPos = 0;
	std::string strErrMsg;

	//if(m_pMAOrderEntity->Status_DoInsert()==CMAOrderEntity::conHasDone) {
		m_pMAOrderEntity->Cancel(nErrPos, strErrMsg);
		//m_pMAOrderEntity->m_dwStartTick = ::GetTickCount();
	//}
	//else {
	//	wxMessageBox(LOADSTRING(已执行过撤单), LOADSTRING(错误));
	//}
	//wxWindow* pWin=NULL; 
	//pWin=FindWindowById(ID_MAORDERCONFIRMDLG_CANCEL,this);
	//if(pWin!=NULL)
	//	pWin->Enable(false);
	
	//EndModal(wxID_NO);
}

void CMAOrderConfirmDlg::OnClose(wxCloseEvent& event)
{
    m_timer.Stop();
    //EndModal(wxID_NO);
	Hide();
}

void CMAOrderConfirmDlg::OnTimer(wxTimerEvent& event)
{
}

bool CMAOrderConfirmDlg::Show(bool show)
{
	wxWindow* pWin=NULL; 
	if(show) {
		if(m_pMAOrderEntity!=NULL) {
			CMAOrderEntity::MAAUTOOPER& maAutoOper = m_pMAOrderEntity->GetMaAutoOper();
			if(maAutoOper.nOperType==CSAOrderEntity::conOperAuto 
					&& !maAutoOper.bIsQuotOrder) {
				pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT,this);
				if(pWin!=NULL)
					pWin->Enable(false);

				pWin=FindWindowById(ID_MAORDERCONFIRMDLG_NEWVOLUME_SPINCTRL,this);
				if(pWin!=NULL)
					pWin->Enable(false);

				pWin=FindWindowById(ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX,this);
				if(pWin!=NULL)
					pWin->Enable(false);
			}
		}
		m_pwxExtListCtrl->SortItems(CompareInt, 1, 0);
		Center();
	}
    return wxDialog::Show(show);
}

void CMAOrderConfirmDlg::OnHookKeyPress(wxKeyEvent& evt)
{
	evt.Skip();
	//wxSpinEvent evtSpin;
	//wxKeyEvent* pEvent=&evt;//(wxKeyEvent*)evt.GetClientData();
 //   if(!pEvent) return;
 //   wxWindow *win = FindFocus();
 //   if(win == NULL) {
	//	evt.Skip();
 //       return;
 //   }
	//pEvent->SetId(win->GetId());
 //   if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB)
 //   {
 //       bool backward = pEvent->ShiftDown();
 //       int id,thisID=win->GetId(),count=m_TabOrderVector.size();
 //       for(id=0;id<count;id++)
 //       {
 //           if(m_TabOrderVector[id]==thisID)
 //           {
 //               if(backward)
 //                   id=id>0?id-1:count-1;
 //               else
 //                   id=id<count-1?id+1:0;
 //               FindWindowById(m_TabOrderVector[id],this)->SetFocus();
 //               return;
 //           }
 //       }
 //   }
 //   else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
 //   {
 //       int id,thisID=win->GetId(),count=m_TabOrderVector.size();
 //       for(id=0;id<count;id++)
 //       {
 //           if(m_TabOrderVector[id]==thisID)
 //           {
 //               if(id>0)
 //               {
 //                   FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
 //                  return;
 //               }
 //           }
 //       }
 //   }
 //   else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
 //   {
 //       int id,thisID=win->GetId(),count=m_TabOrderVector.size();
 //       for(id=0;id<count;id++)
 //       {
 //           if(m_TabOrderVector[id]==thisID)
 //           {
 //               if(id<count-1)
 //               {
 //                   FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
 //                   return;
 //               }
 //           }
 //       }
 //   }
	//else {
	//	for(int nSel = 0; nSel < m_nCmbOrderCount; nSel++) {
	//		if(m_poOrderConfirmPanels[nSel]!=NULL)
	//			m_poOrderConfirmPanels[nSel]->OnHookKeyPress(evt);
	//	}
	//}
}

void CMAOrderConfirmDlg::OnCfgChanged(wxCommandEvent& evt)
{
}

void CMAOrderConfirmDlg::OnPriceClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_PRICECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void CMAOrderConfirmDlg::OnVolumeClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_VOLUMECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void CMAOrderConfirmDlg::OnCmbStrategy(wxCommandEvent& evt)
{
	if(m_bIsLockForSetChange)
		return;
	if(ChangeMaOrderEntity())	
		ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
	m_bUserChgStraNum = TRUE;
}

void CMAOrderConfirmDlg::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, m_fUpperLimitPrice, m_fLowerLimitPrice);
//    SetPrice(fPrice);
	wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());

}

void CMAOrderConfirmDlg::OnPriceSpinDown(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, m_fUpperLimitPrice, m_fLowerLimitPrice);
//    SetPrice(fPrice);
	wxWindow* window=FindWindowById(ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT,this);
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
}

void CMAOrderConfirmDlg::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void CMAOrderConfirmDlg::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void CMAOrderConfirmDlg::OnPriceTextChanged(wxCommandEvent& event)
{
	if(m_bIsLockForSetChange)
		return;
	if(m_pMAOrderEntity==NULL)
		return;

	BOOL bIsMarket = IsMarket(string(GetPriceText().c_str()));
	double fPrice = GetPrice();
	m_pMAOrderEntity->ChangePrice(bIsMarket, fPrice);
	ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
}

BOOL CMAOrderConfirmDlg::ChangeMaOrderEntity()
{
	if(m_pMAOrderEntity==NULL)
		return FALSE;
	
	CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();

	if(m_pOwnerPanel!=NULL && pMACore!=NULL) {

		map<string, bool> mapAccsStatus;
		map<string, bool>::iterator it;
		CMAOrderEntity* pMAEntity = NULL;
		COrderAtomEntity::STDORDERREQ stdReq = m_pMAOrderEntity->GetStdOrderReq(); 
		CSAOrderEntity::OPERPARAM operParam = m_pMAOrderEntity->GetOperParam(); 
		CMAOrderEntity::MAAUTOOPER maOper = m_pMAOrderEntity->GetMaAutoOper(); 

		for(it = m_mapAccounts.begin(); it != m_mapAccounts.end(); it++) {
			if(it->second)
				mapAccsStatus.insert(std::pair<string, bool>(it->first, true));
		}

		{
			// 这里从xml中读是否要确认的配置
			CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
			LPORDER_CFG pOrderCfg = NULL;
			if(pMgr != NULL) {
				pOrderCfg = pMgr->GetOrderCfg();
				maOper.bNeedCancel = pOrderCfg->nCancelSameDirOpenOrderBeforeOrder;
				maOper.nMouseNotReversed = pOrderCfg->nAllNotHaveOppositePosition;
				maOper.nMousePartReversed = pOrderCfg->nSomeHaveOppositePosition;
				maOper.nMouseAllReversed = pOrderCfg->nAllHaveOppositePosition;
				maOper.nPositionClose = pOrderCfg->nMouseClosePositionStrategy;
			}
		}

		StrategyItem straNum = (StrategyItem)GetStrategyNumber();
		stdReq.nVolume = GetQty();
	
		pMAEntity = CMAOrderEntity::CreateMAOrderEntity(straNum, 
				stdReq, operParam, maOper, mapAccsStatus);
		pMAEntity->SetDisorder_Insert(GlobalConfigManager::IsDisorderWhenInsertOrder_MultiAccount());
		OrderInsertPanel* pPanel = (OrderInsertPanel*)m_pOwnerPanel;
		pPanel->LockObject();
		pPanel->ResetMAOrderEntity(m_pMAOrderEntity, pMAEntity);
		delete m_pMAOrderEntity;
		ResetMAOrderEntity(pMAEntity, FALSE, FALSE);
		pPanel->UnlockObject();

		return TRUE;
	}
	return FALSE;
}

void CMAOrderConfirmDlg::OnVolumeTextChanged(wxCommandEvent& event)
{
	if(m_bIsLockForSetChange)
		return;
	if(ChangeMaOrderEntity())
		ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
}

void CMAOrderConfirmDlg::OnItemClicked(wxExtListEvent& evt)
{
	TRACE("OnItemClicked() ID := %u, EventType := %u, col := %d\n", 
			evt.GetId(), evt.GetEventType(), evt.GetColumn());
	evt.Skip();
}

void CMAOrderConfirmDlg::OnItemChanged(wxExtListEvent& evt)
{
	TRACE("OnItemClicked() ID := %u, EventType := %u, col := %d\n", 
			evt.GetId(), evt.GetEventType(), evt.GetColumn());

	if(m_bHasSent) {
		//evt.SetEditCanceled(true);
		return;
	}
	if(evt.GetItem()>=0 && evt.GetColumn()==0) {

		int nCount = 0;
		for(int i=0; i<m_pwxExtListCtrl->GetItemCount(); i++) {
			if(m_pwxExtListCtrl->GetItemChecked(i))
				nCount++;
		}
		if(nCount>0) {
			wxWindow* pWin=FindWindowById(ID_MAORDERCONFIRMDLG_YES,this);
			if(pWin!=NULL)
				pWin->Enable(true);
		}
		else {
			wxWindow* pWin=FindWindowById(ID_MAORDERCONFIRMDLG_YES,this);
			if(pWin!=NULL)
				pWin->Enable(false);
		}

		map<string, bool> accstatus;
		map<string,bool>::iterator itAccs = m_mapAccounts.begin();
		std::map<std::string, int>::iterator it;

		for(; itAccs != m_mapAccounts.end(); itAccs++) {
			it = m_mapKey2Row.find(itAccs->first);
			if(it!=m_mapKey2Row.end()) 
				itAccs->second = m_pwxExtListCtrl->GetItemChecked(
						GetRowFromList(m_pwxExtListCtrl, 
						GetIDFromAccount(std::string(it->first))));
			else
				itAccs->second = false;
		}
		if(ChangeMaOrderEntity())	
			ShowMAOrderEntity(m_pMAOrderEntity, m_bHasSent);
		m_bUserChgAccounts = TRUE;
	}
}

int wxCALLBACK CMAOrderConfirmDlg::CompareInt(const wxExtListItem& item1, 
											  const wxExtListItem& item2,long sortData)
{
	int nNum1 = atoi(item1.GetText());
	int nNum2 = atoi(item2.GetText());

	if(nNum1==nNum2)
		return 0;
	else if(sortData==0) //降序
		return (nNum1>nNum2?1:-1);
	else
		return (nNum1<nNum2?1:-1);
}

int wxCALLBACK CMAOrderConfirmDlg::CompareDouble(const wxExtListItem& item1, 
												 const wxExtListItem& item2,long sortData)
{
	double fDbl1 = atof(item1.GetText());
	double fDbl2 = atof(item2.GetText());

	if(fDbl1==fDbl2)
		return 0;
	else if(sortData==0) //降序
		return (fDbl1>fDbl2?1:-1);
	else
		return (fDbl1<fDbl2?1:-1);
}

int wxCALLBACK CMAOrderConfirmDlg::CompareCheckBox(const wxExtListItem& item1, 
												   const wxExtListItem& item2,long sortData)
{
	wxClassInfo* pInfo = item1.GetClassInfo();
	int nNum1 = atoi(item1.GetText());
	int nNum2 = atoi(item2.GetText());

	if(nNum1==nNum2)
		return 0;
	else if(sortData==0) //降序
		return (nNum1>nNum2?1:-1);
	else
		return (nNum1<nNum2?1:-1);
}

void CMAOrderConfirmDlg::OnColClick(wxExtListEvent& evt)
{
	int col = evt.GetColumn();
	long iasc=evt.GetSortAsc()?1:0;

	//m_pwxExtListCtrl->SetColumnImage(col,iasc? 0:1);
	if(col==1)
		m_pwxExtListCtrl->SortItems(CompareInt, evt.GetColumn(), (long)iasc);
	else if(col==7)
		m_pwxExtListCtrl->SortItems(CompareDouble, evt.GetColumn(), (long)iasc);
	else
		evt.Skip();
}
