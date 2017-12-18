#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderPositionPanel.h"
#include "../inc/Order-Common/CLocalOrderService.h"
#include "../ConfigApp/Const.h"
#include "wx/file.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/ZqMessageBoxEx.h"

#include "CEditOrderPositionDailog.h"
#include "gui-common/FrameBase.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


CRITICAL_SECTION CQueryOrderPositionPanel::m_CS_EvtCallback;


BEGIN_EVENT_TABLE(CQueryOrderPositionPanel, wxPanel)
	EVT_SIZE(OnSize )
	EVT_CHECKBOX(ID_QUERYPOSITION_CHECKSHOWDELETE, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPOSITION_BUTTONQUERY, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPOSITION_BUTTONEDIT, OnButtonEdit)
	EVT_BUTTON(ID_QUERYPOSITION_BUTTONDELETE, OnButtonDelete)
	EVT_BUTTON(ID_QUERYPOSITION_BUTTONCLEAR, OnButtonClear)
	EVT_BUTTON(ID_QUERYPOSITION_BUTTONPLDETAIL, OnButtonPLDetail)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_STOP_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPOSITION_STATUSCHANGED, OnPLStatusChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPLDETAIL_STATUSCHANGED, OnPLDetailStatusChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_SERVCOND_OVERBOUNDFAIL, OnServCondOverBoundFail)
	EVT_COMMAND(wxID_ANY, wxEVT_PLDETAILDLG_PRECLOSE, OnPLDetailDlgPreClose)
    EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, OnActivated)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderPositionPanel::CQueryOrderPositionPanel(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name):
	wxPanel(parent,winid, pos,size, style, name),
	m_poLocalOrderService(NULL),
	m_iColIndex(0)
{
    
	SetBackgroundColour(DEFAULT_COLOUR);

	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_QUERYPOSITION_CHECKSHOWDELETE, 
			LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(5, 3+8), wxSize(14*8, 14));//, 0 );

	m_pButtonQuery = new wxButton(this, ID_QUERYPOSITION_BUTTONQUERY, LOADSTRING(BTN_QP_BUTTONQUERY), 
			wxPoint(140, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonEdit = new wxButton(this, ID_QUERYPOSITION_BUTTONEDIT, LOADSTRING(BTN_QP_BUTTONEDIT), 
			wxPoint(140+55, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonDelete = new wxButton(this, ID_QUERYPOSITION_BUTTONDELETE, LOADSTRING(BTN_QP_BUTTONDELETE), 
			wxPoint(140+110, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonClear = new wxButton(this, ID_QUERYPOSITION_BUTTONCLEAR, LOADSTRING(BTN_QP_BUTTONCLEAR), 
			wxPoint(140+165, 5), wxSize(80, 25), 0, wxDefaultValidator);
	m_pButtonPLDetail = NULL;
	//m_pButtonPLDetail = new wxButton(this, ID_QUERYPOSITION_BUTTONPLDETAIL, LOADSTRING(BTN_QP_BUTTONPLDETAIL), 
	//		wxPoint(140+245, 5), wxSize(80, 25), 0, wxDefaultValidator);
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY,//GID_QUERY_ORDER_POSITION, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	Init();

	m_poLocalOrderService = CLocalOrderService::GetInstance();
	m_poLocalOrderService->RegisterMsgWindow(this);
    InitializeCriticalSection(&m_CS_EvtCallback);
	
	ShowOrderPositions(FALSE);
}

CQueryOrderPositionPanel::~CQueryOrderPositionPanel(void)
{
	m_poLocalOrderService->UnregisterMsgWindow(this);

	std::map<int, CQueryOrderPLDetailDialog*>::iterator it;
	::EnterCriticalSection(&m_CS_EvtCallback);
	it = m_mapDetailDlg.begin();
	for(;it != m_mapDetailDlg.end(); it++) 
	{
		if(it->second)
			it->second->Destroy();
	}
	m_mapDetailDlg.clear();
	::LeaveCriticalSection(&m_CS_EvtCallback);

    DeleteCriticalSection(&m_CS_EvtCallback);
}

bool CQueryOrderPositionPanel::Init()
{
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);

	m_iRowCnt=0;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_POSITION);
	wxASSERT(m_pListCfg);

	//设置LIST界面属性
	m_HeadBackgroundColor = wxColor(m_pListCfg->HeadBackgroundColor);
	m_HeadColor = wxColor(m_pListCfg->HeadColor);
	m_BackgroundColor = wxColor(m_pListCfg->BackgroundColor);
	m_TextColor = wxColor(m_pListCfg->TextColor);
	m_Font.SetNativeFontInfoUserDesc(m_pListCfg->szFont); 

	m_pwxExtListCtrl->SetHeaderBackgroundColour(m_HeadBackgroundColor);
	m_pwxExtListCtrl->SetHeaderForegroundColour(m_HeadColor);
	m_pwxExtListCtrl->SetBackgroundColour(m_BackgroundColor);
	m_pwxExtListCtrl->SetForegroundColour(m_TextColor);
	m_pwxExtListCtrl->SetFont(m_Font);
	m_pwxExtListCtrl->SetOddRowColour(m_pListCfg->OddLineBackgroundColor);
	m_pwxExtListCtrl->SetEvenRowColour(m_pListCfg->EvenLineBackgroundColor);

	wxExtListItem col;
	int nColIndex = 0;

	nColCount = pCfgMgr->GetListVisibleColCount(GID_QUERY_ORDER_POSITION);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0;  i < nColCount; i++) {
		//LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_POSITION, i);
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSITION, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<ORDERPOSITION_MIN || pColCfg->id>ORDERPOSITION_MAX)
			continue;

		//"合约" "报单编号"栏，左对齐
		if( (pColCfg->id == ORDERPOSITION_InstrumentID) || (pColCfg->id == ORDERPOSITION_ConditionID) )
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余的，右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        if(pColCfg->Visibility == 0)
        col.SetWidth(0);
        else
        col.SetWidth(pColCfg->Width);///* /
        if(pColCfg->Visibility != 0) {		
            col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_POSITION,pColCfg->id));
            col.SetWidth(pColCfg->Width);
			col.SetTextColour(pColCfg->TextColor);
            m_FieldID2ColumnID[pColCfg->id] = nColIndex;
            m_FieldAttr[pColCfg->id] = *pColCfg;

			col.SetColData(pColCfg->id);
            m_pwxExtListCtrl->InsertColumn(nColIndex, col); 
            nColIndex++;
        }
    }
	PROCESS_LANGUAGE_EVENT();
    
	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_QUERYPOSITION_BUTTONQUERY);
	OnButtonQuery(evt);

	return true;
}

void CQueryOrderPositionPanel::ShowOrderPositions(BOOL bIsShowDelete)
{
	int iRowNo = 0, iCol = 0, iSumCol = 0;

	m_pwxExtListCtrl->DeleteAllItems();
	m_OrderPositionKey2Row.clear();
	m_OrderPositionRow2Key.clear();

	i_CurOrderRows=-1;

	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();
	CLocalOrderService::MAPPLORDER mapPosition = m_poLocalOrderService->GetOrderPLMap();
	m_poLocalOrderService->UnlockObject();
	CLocalOrderService::MAPPLORDERITEM it = mapPosition.begin();

	iRowNo = -1;
	
	for(; it!=mapPosition.end(); ++it)	{
		CLocalOrderService::PLORDER& tPositionOrder = it->second;

		if(!bIsShowDelete && tPositionOrder.nState==conPLDelete)
			continue;

		++iRowNo;
		if(ShowOneOrder(iRowNo, tPositionOrder) < 0) {
			--iRowNo;
			continue;
		}
		else 
		{
			m_OrderPositionKey2Row[tPositionOrder.FTID] = iRowNo;
			m_OrderPositionRow2Key[iRowNo] = tPositionOrder.FTID;
		}
	}

	i_CurOrderRows = iRowNo;

}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderPositionPanel::ShowOneOrder(int iRowNo, 
										   const CLocalOrderService::PLORDER& tPositionOrder, 
										   BOOL bIsNew)
{	

	if(m_pwxExtListCtrl->GetColumnCount() == 0)
		return -1;
	
	int iCol = 0;
	wxString newText;//单元格的文本内容
	wxString strValue;
	wxExtListItem item;	
	
	item.Clear();

	iCol = 0;

	if(bIsNew) {
		m_pwxExtListCtrl->InsertItem(iRowNo, item);
		m_pwxExtListCtrl->SetItemData(iRowNo, (long)tPositionOrder.FTID);
		item.SetId(iRowNo);	//设置行号
	}
	else {
		item.SetId(iRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}

	std::map<int, long> ::iterator it;
	it = m_FieldID2ColumnID.find(ORDERPOSITION_PositionID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPositionOrder.FTID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_OpenID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.ref.OrderSysID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_State);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		switch(tPositionOrder.nState) {
		case conPLPause:
			newText = LOADSTRING(PL_conPLPause);
			break;
		case conPLActive:
			newText = LOADSTRING(PL_conPLActive);
			break;
		case conPLDelete:
			newText = LOADSTRING(PL_conPLDelete);
			break;
		case conPLNoTrade:
			newText = LOADSTRING(PL_conPLNoTrade);
			break;
		case conPLPartTraded:
			newText = LOADSTRING(PL_conPLPartTraded);
			break;
		case conPLAllTraded:
			newText = LOADSTRING(PL_conPLAllTraded);
			break;
		case conPLCancelNoTrade:
			newText = LOADSTRING(PL_conPLCancelNoTrade);
			break;
		case conPLCancelPartTraded:
			newText = LOADSTRING(PL_conPLCancelPartTraded);
			break;
		case conPLFail:
			newText = LOADSTRING(PL_conPLFail);
			break;
		default:
			newText = "";
			break;
		};
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_ReqState);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		if(tPositionOrder.nState==conPLDelete)
			newText = LOADSTRING(BTN_QP_BUTTONDELETE);
		else
			newText = CFieldValueTool::OrderStatus2String(tPositionOrder.ref.OrderStatus,SVR_LANGUAGE);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_ConditionID);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;	
	//	newText.Printf("zy:%d, zs:%d", tPositionOrder.nCondNum_StopGain, tPositionOrder.nCondNum_StopLose);
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_InstrumentID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.InstrumentID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_Direction);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_RegVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPositionOrder.orderSrc.nVolume);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_RegPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		if(tPositionOrder.orderSrc.bIsMarket)
			newText = LOADSTRING(OLS_MARKET);
		else
			newText = Double2String(tPositionOrder.orderSrc.fPrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_OpenedPriceOffset);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;	
	//	newText = Double2String(tPositionOrder.fOpenedPriceOffset).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_StopGainPrice);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	newText = Double2String(tPositionOrder.fStopGainPrice).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_StopLosePrice);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	newText = Double2String(tPositionOrder.fStopLosePrice).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_StopGainPriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		if(util::equals(tPositionOrder.plparam.fStopGainPriceOffset, 0.0))
			newText = "-";
		else
			newText = Double2String(tPositionOrder.plparam.fStopGainPriceOffset).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_StopLosePriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		if(util::equals(tPositionOrder.plparam.fStopLosePriceOffset, 0.0))
			newText = "-";
		else
			newText = Double2String(tPositionOrder.plparam.fStopLosePriceOffset).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_StopGainForecast);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	newText = Double2String(tPositionOrder.fStopGainForecast).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPOSITION_StopLoseForecast);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	newText = Double2String(tPositionOrder.fStopLoseForecast).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_PriceType);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.plparam.nPriceType==1 ? LOADSTRING(OLS_LASTPRICE3) : LOADSTRING(OLS_B_S_PRICE3));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_CloseMode);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.plparam.nCloseMode==1 ? LOADSTRING(OLS_UPLWPRICE): LOADSTRING(OLS_LIMITPRICE));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_ClosePriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.plparam.fClosePriceOffset).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	return 0;
}

void CQueryOrderPositionPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
	//FindWindowById(GID_QUERY_ORDER_POSITION,this)->SetPosition(wxPoint(0,30));
	wxWindow* pWin = NULL;
	for(int i=ID_QUERYPOSITION_BUTTONQUERY; i<ID_QUERYPOSITION_NUM; i++)
	{
		pWin = FindWindowById(i, this);
		if(pWin)
		{
			wxPoint pos = pWin->GetPosition();
			pos.y=-30;
			pWin->SetPosition(pos);
		}
	}
	if(m_pwxExtListCtrl)
	{	
		m_pwxExtListCtrl->SetPosition(wxPoint(0, 0));
		m_pwxExtListCtrl->SetSize(rt.width,rt.height);
	}
	event.Skip();
}

/*撤单按钮事件响应*/
void CQueryOrderPositionPanel::OnButtonQuery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPositions(bIsShowDelete);
}

void CQueryOrderPositionPanel::OnButtonEdit(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, int>::iterator itr2k = m_OrderPositionRow2Key.find(SelectItemId);
	if(itr2k == m_OrderPositionRow2Key.end()) return;

	int nPositionNum=itr2k->second;
	
	if(nPositionNum<=0) return;
	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();
	CLocalOrderService::MAPPLORDER& mapPosition = m_poLocalOrderService->GetOrderPLMap();
	CLocalOrderService::MAPPLORDERITEM it = mapPosition.find(nPositionNum);

	if(it == mapPosition.end()) {
		m_poLocalOrderService->UnlockObject();
		wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	CLocalOrderService::PLORDER tPosOrder={0};
	tPosOrder = it->second;
	m_poLocalOrderService->UnlockObject();

	if(!(it->second.ref.OrderStatus==THOST_FTDC_OST_PartTradedQueueing 
			|| it->second.ref.OrderStatus==THOST_FTDC_OST_NoTradeQueueing 
			|| it->second.ref.OrderStatus==THOST_FTDC_OST_Unknown)) {
		wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(tPosOrder.InstrumentID,insInfo)!=0) 
		insInfo.PriceTick = 1.0f;

	bool bFail=false;
	CEditOrderPositionDailog* pDlg = new CEditOrderPositionDailog(this, wxID_ANY, LOADSTRING(CONDITIONCURRENT_TITLE), wxDefaultPosition, wxSize(310, 250));
	pDlg->SetPriceTick(insInfo.PriceTick);
	pDlg->SetPositionOrder(tPosOrder);
	if(pDlg->ShowModal()==wxID_OK) {
		tPosOrder = pDlg->GetPLOrder();

		// 同步加锁，保证数据的一致性
		m_poLocalOrderService->LockObject();

		// 判断修改期间止盈止损单是否成交
		it = mapPosition.find(nPositionNum);
		if(it!=mapPosition.end())
		{
			if(it->second.ref.OrderStatus==THOST_FTDC_OST_PartTradedQueueing 
					|| it->second.ref.OrderStatus==THOST_FTDC_OST_NoTradeQueueing 
					|| it->second.ref.OrderStatus==THOST_FTDC_OST_Unknown) {
				m_poLocalOrderService->OrderPLSet(nPositionNum, tPosOrder);
			}
			else
				bFail=true;
		}
		else 
			bFail=true;

		// 同步解锁
		m_poLocalOrderService->UnlockObject();

		//ShowOneOrder(SelectItemId, tPosOrder, FALSE);

		if(bFail) {
			wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
	pDlg->Destroy();
}

wxString CQueryOrderPositionPanel::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
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
		strItem.Printf("%s=[%s]%s", itemCol.GetText().c_str(), 
			pListCtrl->GetSubItemText(nRow, nCol).c_str(),
			(nCol<pListCtrl->GetColumnCount()-1?", ":""));
		strText+=strItem;
	}
	return strText;
}

wxString CQueryOrderPositionPanel::GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl)
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

void CQueryOrderPositionPanel::OnButtonDelete(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, int>::iterator itr2k = m_OrderPositionRow2Key.find(SelectItemId);
	if(itr2k == m_OrderPositionRow2Key.end()) return;

	int nPositionNum=itr2k->second;
	
	wxExtListItem item;
	item.SetId(SelectItemId);

	if(m_poLocalOrderService==NULL) return;

	CLocalOrderService::MAPPLORDERITEM it;
	BOOL bIsFound = FALSE;
	CLocalOrderService::PLORDER plOrder = {0};
	
	m_poLocalOrderService->LockObject();
	it = m_poLocalOrderService->GetOrderPLMap().find(nPositionNum);
	bIsFound = it!=m_poLocalOrderService->GetOrderPLMap().end();
	if(bIsFound)
		plOrder = it->second;
	m_poLocalOrderService->UnlockObject();
	if(!bIsFound)
	{
		wxMessageDialog dialog( NULL,LOADSTRING(MSG_POS_DELETEFIALED) ,LOADSTRING(OIP_ERROR),wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	int nRet = 0;
	//m_poLocalOrderService->LockObject();
	//BOOL bRet = m_poLocalOrderService->OrderPLDelete(nPositionNum, conPLDelete, TRUE);
	//m_poLocalOrderService->UnlockObject();

	USERLOG_INFO("用户单击盈损单模块按钮[撤单]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	if(plOrder.ref.OrderStatus==THOST_FTDC_OST_PartTradedQueueing
			|| plOrder.ref.OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
	{
        PlatformStru_InputOrderAction ReqData;
        memset(&ReqData, 0, sizeof(ReqData));
		ReqData.Thost.FrontID=plOrder.ref.FrontID;
        ReqData.Thost.SessionID=plOrder.ref.SessionID;
        memcpy(ReqData.Thost.OrderRef, plOrder.ref.OrderRef, sizeof(ReqData.Thost.OrderRef)-1);
        memcpy(ReqData.Thost.OrderSysID, plOrder.ref.OrderSysID, sizeof(ReqData.Thost.OrderSysID)-1);
        memcpy(ReqData.Thost.ExchangeID, plOrder.ref.ExchangeID, sizeof(ReqData.Thost.ExchangeID)-1);
		memcpy(ReqData.Thost.InstrumentID, plOrder.InstrumentID, sizeof(ReqData.Thost.InstrumentID)-1);
        nRet=DEFAULT_SVR()?
				DEFAULT_SVR()->ReqOrderAction(ReqData):-1;
	}
	else
		nRet=-1;

	if(nRet!=0) 
	{
		wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
	}
	//else {
	//	wxString strText;
	//	strText.Printf("%d (", nPositionNum);
	//	strText += LOADSTRING(OIP_DELETEED);
	//	strText += ")";
	//	item.SetText(strText);
	//	m_pwxExtListCtrl->SetItem(item);
	//}
}

void CQueryOrderPositionPanel::OnButtonClear(wxCommandEvent& evt)
{
#if 0
	if(m_poLocalOrderService==NULL) return;
	
	CLocalOrderService::MAPPLORDER& mapPosition = m_poLocalOrderService->GetOrderPLMap();

	int nCount = 0;

	nCount += mapPosition.size();
	if(nCount == 0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	CLocalOrderService::MAPPLORDERITEM it;
	std::vector<int> arrDelete;
	std::vector<int>::iterator itDel;


	wxMessageDialog dialog( NULL, LOADSTRING(MSG_POS_CLEARNOW), LOADSTRING(OIP_FASTTRADER), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES) return;


	m_poLocalOrderService->LockObject();

	it = mapPosition.begin();
	while(it!=mapPosition.end()) {
		arrDelete.push_back(it->first);
		it++;
	}
	
	nCount = (int)arrDelete.size();
	for(int i=0; i<nCount; i++) {
		m_poLocalOrderService->OrderPLDelete(arrDelete[i], conPLDelete, i>=nCount-1);
	}
	arrDelete.clear();

	m_poLocalOrderService->UnlockObject();

	//ShowOrderPositions(FALSE);
#endif
}

void CQueryOrderPositionPanel::OnActivated(wxExtListEvent& event)
{
	//wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, ID_QUERYPOSITION_BUTTONPLDETAIL);
	//OnButtonPLDetail(newEvt);
}

void CQueryOrderPositionPanel::OnButtonPLDetail(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, int>::iterator itr2k = m_OrderPositionRow2Key.find(SelectItemId);
	if(itr2k == m_OrderPositionRow2Key.end()) return;

	int nPositionNum=itr2k->second;

	std::map<int, CQueryOrderPLDetailDialog*>::iterator it;
	::EnterCriticalSection(&m_CS_EvtCallback);
	it = m_mapDetailDlg.find(nPositionNum);
	if(it != m_mapDetailDlg.end()) 
	{
		if(it->second)
		{
			it->second->Show();
			it->second->SetFocus();
		}
	}
	else 
	{
		wxString strTitle;
		strTitle.Printf(LOADFORMATSTRING(PLDETAILDLG_TITLE, "%d"), nPositionNum);
		CQueryOrderPLDetailDialog* pDlg = new CQueryOrderPLDetailDialog(GETTOPWINDOW());
		pDlg->SetOwner(this);
		pDlg->Init(nPositionNum);
		pDlg->SetTitle(strTitle);
		pDlg->SetSize(wxSize(600, 350));
		pDlg->Show();
		m_mapDetailDlg.insert(std::pair<int, CQueryOrderPLDetailDialog*>(nPositionNum, pDlg));
	}
	::LeaveCriticalSection(&m_CS_EvtCallback);
}
//配置文件发生变化
void CQueryOrderPositionPanel::OnCfgChanged(wxCommandEvent& evt)
{
	if(evt.GetInt()!=MSG_LIST_ORDER_PLDETAIL)
	{
		Init();
		wxCheckBox* itemCheck = NULL;
		itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSITION_CHECKSHOWDELETE, this);
		BOOL bIsShowDelete = itemCheck->GetValue();
		ShowOrderPositions(bIsShowDelete);
	}
	else 
	{
		wxCommandEvent myEvent(wxEVT_CFG_CHANGE);
		std::map<int, CQueryOrderPLDetailDialog*>::iterator it;
		::EnterCriticalSection(&m_CS_EvtCallback);
		it = m_mapDetailDlg.begin();
		for(; it != m_mapDetailDlg.end(); it++) 
		{
			if(it->second!=NULL)
				it->second->AddPendingEvent(myEvent);
		}
		::LeaveCriticalSection(&m_CS_EvtCallback);
	}
}

void CQueryOrderPositionPanel::OnInitSubscribe(wxCommandEvent& evt)
{
}

void CQueryOrderPositionPanel::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	BOOL bIsRowCLick=evt.GetId();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏

	menu.Append(ID_QUERYPOSITION_BUTTONQUERY,LOADSTRING(CONTEXTMENU_QP_BUTTONQUERY));
	menu.AppendSeparator();
	if(evt.GetId())
	{
		menu.Append(ID_QUERYPOSITION_BUTTONEDIT,LOADSTRING(CONTEXTMENU_QP_BUTTONEDIT));
		menu.Append(ID_QUERYPOSITION_BUTTONDELETE,LOADSTRING(CONTEXTMENU_QP_BUTTONDELETE));
		//menu.Append(ID_QUERYPOSITION_BUTTONCLEAR,LOADSTRING(CONTEXTMENU_QP_BUTTONCLEAR));
		//menu.Append(ID_QUERYPOSITION_BUTTONPLDETAIL,LOADSTRING(BTN_QP_BUTTONPLDETAIL));
		menu.AppendSeparator();
	}

	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	if(bIsRowCLick) {
		menu.AppendSeparator();
		INSERT_ADDTOMYSELF_ITEM(menu);
	}

	if(menu.GetMenuItemCount()>0)
	{
		POINT pt;
		::GetCursorPos(&pt);
		BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

		//加入自选
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
		if(retcmd - ID_MENU_ADDTOMYSELF >= 0 && retcmd - ID_MENU_ADDTOMYSELF < 50) {
			bool bAdded = false;
			int nPositionNum=0;
			string InstrumentId;
			wxExtListItem item;
			wxString strPositionNum;
			CLocalOrderService::PLORDER tOrder;
			for(int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++) {
				if(m_pwxExtListCtrl->IsSelected( i )) {
					item.SetId(i);
					item.SetColumn(0);
					m_pwxExtListCtrl->GetItem(item);
					strPositionNum = item.GetText();
					nPositionNum = atoi(strPositionNum.c_str());

					if(nPositionNum<=0) 
						continue;
					if(m_poLocalOrderService==NULL) 
						continue;
					m_poLocalOrderService->LockObject();
					{
					CLocalOrderService::MAPPLORDER& mapPosition = m_poLocalOrderService->GetOrderPLMap();
					CLocalOrderService::MAPPLORDERITEM it = mapPosition.find(nPositionNum);

					InstrumentId = "";
					if(it != mapPosition.end()) {
						tOrder = it->second;
						InstrumentId = tOrder.InstrumentID;
					}
					}
					m_poLocalOrderService->UnlockObject();

					if(InstrumentId.length()==0)
						continue;

					if(pCfgMgr->GetInstrumentGroupMemberCount(retcmd - ID_MENU_ADDTOMYSELF)>=DEFAULT_SVR()->GetGroupMaxContractNum()) {
						wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
						break;
					}
					else {
						pCfgMgr->AddInstrument( retcmd - ID_MENU_ADDTOMYSELF, InstrumentId.c_str()); 
						bAdded = true;					
					}
				}
			}

			if(bAdded) {
				wxCommandEvent myEvent(wxEVT_CONFIG_CHANGED);
				wxString strMsg;
				strMsg.sprintf( "%d", MSG_INSTRUMENT_GROUP_CHANGE );
//				myEvent.SetString( strMsg.c_str() );
//				GetParent()->AddPendingEvent(myEvent); 
                if(1)
                {
	                DWORD EvtParamID;
                    std::string EvtParamStr(strMsg.c_str());
	                if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
	                {
		                myEvent.SetInt((int)EvtParamID);
		                GetParent()->AddPendingEvent(myEvent);
	                }
                }
				return;
			}
		}

		EnterCriticalSection(&m_CS_EvtCallback);

		wxCommandEvent newevt;
		switch(retcmd)
		{
		case ID_MENU_AUTO_ADJUST_COL_WIDTH:
			m_pwxExtListCtrl->AutoAdjustColumnWidth();
			SaveColWidth();
			break;
		case ID_QUERYPOSITION_BUTTONQUERY://市价反手
			newevt.SetId(ID_QUERYPOSITION_BUTTONQUERY);
			OnButtonQuery(newevt);
			break;
		case ID_QUERYPOSITION_BUTTONEDIT://市价反手
			newevt.SetId(ID_QUERYPOSITION_BUTTONEDIT);
			OnButtonEdit(newevt);
			break;
		case ID_QUERYPOSITION_BUTTONDELETE://市价反手
			newevt.SetId(ID_QUERYPOSITION_BUTTONDELETE);
			OnButtonDelete(newevt);
			break;
		case ID_QUERYPOSITION_BUTTONCLEAR://市价反手
			newevt.SetId(ID_QUERYPOSITION_BUTTONCLEAR);
			OnButtonClear(newevt);
			break;
		case ID_QUERYPOSITION_BUTTONPLDETAIL://市价反手
			newevt.SetId(ID_QUERYPOSITION_BUTTONPLDETAIL);
			OnButtonPLDetail(newevt);
			break;
		case ID_MENU_EXPORT://导出列表
			{
                wxString cap=((FrameBase*)GETTOPWINDOW())->GetModuleCaption(GetId());
				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
			SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_QUERYORDERPOSITION);
			break;
		default:
			break;
		}

        LeaveCriticalSection(&m_CS_EvtCallback);
    }
    evt.Skip();
}

void CQueryOrderPositionPanel::OnRequery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPositions(bIsShowDelete);
}

void CQueryOrderPositionPanel::OnPLStatusChanged(wxCommandEvent& evt)
{
	wxCommandEvent evt2(wxEVT_COMMAND_BUTTON_CLICKED);
	OnButtonQuery(evt2);
}

void CQueryOrderPositionPanel::OnPLDetailStatusChanged(wxCommandEvent& evt)
{
	int nPLNum = evt.GetExtraLong();
	std::map<int, CQueryOrderPLDetailDialog*>::iterator it;
	::EnterCriticalSection(&m_CS_EvtCallback);
	it = m_mapDetailDlg.find(nPLNum);
	if(it != m_mapDetailDlg.end()) 
	{
		if(it->second)
			it->second->ShowOrderPLDetails(TRUE);
	}
	::LeaveCriticalSection(&m_CS_EvtCallback);
}

void CQueryOrderPositionPanel::OnPLDetailDlgPreClose(wxCommandEvent& evt)
{
	int nPLDetailNum = evt.GetInt();
	std::map<int, CQueryOrderPLDetailDialog*>::iterator it;
	::EnterCriticalSection(&m_CS_EvtCallback);
	it = m_mapDetailDlg.find(nPLDetailNum);
	if(it != m_mapDetailDlg.end()) 
	{
		if(it->second)
			it->second->Destroy();
		m_mapDetailDlg.erase(nPLDetailNum);
	}
	::LeaveCriticalSection(&m_CS_EvtCallback);
}

BOOL CQueryOrderPositionPanel::NeedOrderFailSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderFailSound;
}

BOOL CQueryOrderPositionPanel::NeedServCondOverBoundConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bReachUpperLimitDlg;
}

BOOL CQueryOrderPositionPanel::NeedServCondOverBoundSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bReachUpperLimitSound;
}

void CQueryOrderPositionPanel::OnServCondOverBoundFail(wxCommandEvent& evt)
{
//	long nNum = evt.GetInt();
//	int nMode = evt.GetExtraLong();
//	wxString strInstrumentID = evt.GetString();
//	int nTimeOut = 10;
//	wxString strText;
//	strText.Printf(LOADFORMATSTRING(CONDS2L_OverBoundFailMSG, "%s%s"), 
//			strInstrumentID.c_str(), (nMode==0 ? LOADSTRING(CONDS2L_OverBoundFail_COND) : 
//			(nMode==1 ? LOADSTRING(CONDS2L_OverBoundFail_SP) : LOADSTRING(CONDS2L_OverBoundFail_SL))));
//
//	{
//		if(TRADEINFODLG(this)!=NULL)
//		{
//			LOG_DATA* pData = new LOG_DATA;
//			pData->strTitle = std::string(LOADSTRING(CONDS2L_OverBoundFailTitle));
//			pData->strFormat = std::string(strText.c_str());
//			TRADEINFODLG(this)->WriteLog(pData);
//		}
//	}
////
//	CZqMessageBoxEx* pmsgBox = new CZqMessageBoxEx(GETTOPWINDOW());
//	pmsgBox->ResetMessage(wxString(LOADSTRING(CONDS2L_OverBoundFailTitle)), strText, 
//			CZqMessageBoxEx::btnYes|CZqMessageBoxEx::btnNo, 
//			CZqMessageBoxEx::condServOverBound);
//	pmsgBox->SetCondInfo(NULL, nNum, nMode);
//	if(!NeedServCondOverBoundConfirm()) // 如果不需要弹出对话框，就进行自动转换
//	{
//		pmsgBox->DoCond(TRUE);
//		pmsgBox->Destroy();
//	}
//	else
//		pmsgBox->Show();
//	if(NeedServCondOverBoundSound())
//		NoticeSound(conNoticeNewOrderFail);
}

void CQueryOrderPositionPanel::OnLanguageChange(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPositions(bIsShowDelete);

	TRANSFORM_COLUMN_TEXT(GID_QUERY_ORDER_POSITION,m_pwxExtListCtrl);
	FindWindowById(ID_QUERYPOSITION_CHECKSHOWDELETE,this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	m_pButtonQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	m_pButtonEdit->SetLabel(LOADSTRING(BTN_QP_BUTTONEDIT));
	m_pButtonDelete->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	m_pButtonClear->SetLabel(LOADSTRING(BTN_QP_BUTTONCLEAR));
	m_pButtonPLDetail->SetLabel(LOADSTRING(BTN_QP_BUTTONPLDETAIL));
}

void CQueryOrderPositionPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_POSITION ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSITION, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}

void CQueryOrderPositionPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_POSITION ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSITION, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}
