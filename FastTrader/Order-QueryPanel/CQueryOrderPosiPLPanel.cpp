#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderPosiPLPanel.h"
#include "../inc/Order-Common/CLocalOrderService.h"
#include "../ConfigApp/Const.h"
#include "wx/file.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/ZqMessageBoxEx.h"

//#include "CEditOrderPLDetailDailog.h"
#include "../inc/Order-Common/CEditOrderPosiPLDailog.h"
#include "gui-common/FrameBase.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


CRITICAL_SECTION CQueryOrderPosiPLPanel::m_CS_EvtCallback;


BEGIN_EVENT_TABLE(CQueryOrderPosiPLPanel, wxPanel)
	EVT_SIZE(OnSize )
	EVT_CHECKBOX(ID_QUERYPOSIPL_CHECKSHOWDELETE, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPOSIPL_BUTTONQUERY, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPOSIPL_BUTTONEDIT, OnButtonEdit)
	EVT_BUTTON(ID_QUERYPOSIPL_BUTTONDELETE, OnButtonDelete)
	EVT_BUTTON(ID_QUERYPOSIPL_BUTTONCLEAR, OnButtonClear)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_STOP_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
//	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPOSITION_STATUSCHANGED, OnPLStatusChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPLDETAIL_STATUSCHANGED, OnPLDetailStatusChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_SERVCOND_OVERBOUNDFAIL, OnServCondOverBoundFail)
//	EVT_COMMAND(wxID_ANY, wxEVT_PLDETAILDLG_PRECLOSE, OnPLDetailDlgPreClose)
    EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, OnActivated)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderPosiPLPanel::CQueryOrderPosiPLPanel(wxWindow *parent,
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

	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_QUERYPOSIPL_CHECKSHOWDELETE, 
			LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(5, 3+8), wxSize(14*8, 14));//, 0 );

	m_pButtonQuery = new wxButton(this, ID_QUERYPOSIPL_BUTTONQUERY, LOADSTRING(BTN_QP_BUTTONQUERY), 
			wxPoint(140, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonEdit = new wxButton(this, ID_QUERYPOSIPL_BUTTONEDIT, LOADSTRING(BTN_QP_BUTTONEDIT), 
			wxPoint(140+55, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonDelete = new wxButton(this, ID_QUERYPOSIPL_BUTTONDELETE, LOADSTRING(BTN_QP_BUTTONDELETE), 
			wxPoint(140+110, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonClear = new wxButton(this, ID_QUERYPOSIPL_BUTTONCLEAR, LOADSTRING(BTN_QP_BUTTONCLEAR), 
			wxPoint(140+165, 5), wxSize(80, 25), 0, wxDefaultValidator);
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY,//GID_QUERY_ORDER_POSITION, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	Init();

	m_poLocalOrderService = CLocalOrderService::GetInstance();
	m_poLocalOrderService->RegisterMsgWindow(this);
    InitializeCriticalSection(&m_CS_EvtCallback);
	
	ShowOrderPositions(TRUE);
}

CQueryOrderPosiPLPanel::~CQueryOrderPosiPLPanel(void)
{
	m_poLocalOrderService->UnregisterMsgWindow(this);

    DeleteCriticalSection(&m_CS_EvtCallback);
}

bool CQueryOrderPosiPLPanel::Init()
{
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);

	m_iRowCnt=0;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_POSIPL);
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

	nColCount = pCfgMgr->GetListVisibleColCount(GID_QUERY_ORDER_POSIPL);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0;  i < nColCount; i++) {
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSIPL, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<ORDERPOSIPL_MIN || pColCfg->id>ORDERPOSIPL_MAX)
			continue;

		//"合约" "报单编号"栏，左对齐
		if((pColCfg->id == ORDERPOSIPL_InstrumentID) || (pColCfg->id == ORDERPOSIPL_ConditionID))
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余的，右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        if(pColCfg->Visibility == 0)
			col.SetWidth(0);
        else
			col.SetWidth(pColCfg->Width);
        if(pColCfg->Visibility != 0) {		
            col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_POSIPL,pColCfg->id));
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
    
	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_QUERYPOSIPL_BUTTONQUERY);
	OnButtonQuery(evt);

	return true;
}

void CQueryOrderPosiPLPanel::ShowOrderPositions(BOOL bIsShowDelete)
{
	int iRowNo = 0, iCol = 0, iSumCol = 0;

	m_pwxExtListCtrl->DeleteAllItems();
	m_OrderPositionKey2Row.clear();
	m_OrderPositionRow2Key.clear();

	i_CurOrderRows=-1;

	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();
	CLocalOrderService::MAPPLDETAILORDER mapPosition = m_poLocalOrderService->GetOrderPLDetailMap();
	m_poLocalOrderService->UnlockObject();
	CLocalOrderService::MAPPLDETAILORDERITEM it = mapPosition.begin();

	iRowNo = -1;
	
	for(; it!=mapPosition.end(); ++it)	{
		CLocalOrderService::PLDETAILORDER& tPosiPL = it->second;

		if(!bIsShowDelete && tPosiPL.nState==conPosiPLDelete)
			continue;

		++iRowNo;
		if(ShowOneOrder(iRowNo, tPosiPL) < 0) {
			--iRowNo;
			continue;
		}
		else 
		{
			m_OrderPositionKey2Row[tPosiPL.FTID] = iRowNo;
			m_OrderPositionRow2Key[iRowNo] = tPosiPL.FTID;
		}
	}

	i_CurOrderRows = iRowNo;

}

int CQueryOrderPosiPLPanel::ShowOneOrder(int iRowNo, 
							const CLocalOrderService::PLDETAILORDER& tPLDetailOrder, 
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
		m_pwxExtListCtrl->SetItemData(iRowNo, tPLDetailOrder.FTID);
		item.SetId(iRowNo);	//设置行号
	}
	else {
		item.SetId(iRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}

	std::map<int, long> ::iterator it;
	it = m_FieldID2ColumnID.find(ORDERPOSIPL_PosiPLID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPLDetailOrder.FTID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_ConditionID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = "";
		wxString strTmp;
		if(tPLDetailOrder.plparam.bDoStopGain)
		{
			if(!tPLDetailOrder.bGainIsServ)
				strTmp.Printf("%s %d, ", LOADSTRING(POSIPL_COND_ZY_LOCAL), tPLDetailOrder.refGain.FTID);
			else
				strTmp.Printf("%s %s, ", LOADSTRING(POSIPL_COND_ZY_SERVER), tPLDetailOrder.refGain.OrderSysID);
			newText = strTmp;
		}
		if(tPLDetailOrder.plparam.bDoStopLose)
		{
			if(!tPLDetailOrder.bLoseIsServ)
				strTmp.Printf("%s %d", LOADSTRING(POSIPL_COND_ZS_LOCAL), tPLDetailOrder.refLose.FTID);
			else
				strTmp.Printf("%s %s", LOADSTRING(POSIPL_COND_ZS_SERVER), tPLDetailOrder.refLose.OrderSysID);
			newText += strTmp;
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_StopProfitState);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = "-";
		if(m_poLocalOrderService!=NULL)
		{
			if(tPLDetailOrder.plparam.bDoStopGain)
			{
				if(!tPLDetailOrder.bGainIsServ)
				{
					CLocalOrderService::MAPCONDITIONORDERITEM it 
							= m_poLocalOrderService->GetOrderConditionMap()
							.find(tPLDetailOrder.refGain.FTID);
					CLocalOrderService::CONDITIONORDER tCondOrder = {0};
					tCondOrder.nState = -1;
					if(it != m_poLocalOrderService->GetOrderConditionMap().end())
						tCondOrder = it->second;
					switch(tCondOrder.nState) {
					case conCondPause:
						newText = LOADSTRING(OIP_PAUSE);
						break;
					case conCondActive:
						newText = LOADSTRING(OIP_ACTIVE);
						break;
					case conCondDelete:
						newText = LOADSTRING(OIP_DELETEED);
						break;
					case conCondConfirm:
						newText = LOADSTRING(OIP_SUBMITORDER);
						break;
					case conCondCancel:
						newText = LOADSTRING(OIP_SYSCANCEL);
						break;
					case conCondCompleted:
						newText = LOADSTRING(OIP_COMPLETE);
						break;
					default:
						newText = "";
						break;
					};
				}
				else
				{
					newText = CFieldValueTool::OrderStatus2String(tPLDetailOrder.refGain.OrderStatus, SVR_LANGUAGE);
					//OrderKey orderKey(tPLDetailOrder.refGain.Account, tPLDetailOrder.InstrumentID, 
					//		tPLDetailOrder.refGain.FrontID, tPLDetailOrder.refGain.SessionID, 
					//		tPLDetailOrder.refGain.OrderRef);
					//PlatformStru_OrderInfo rawData;
					//if(DEFAULT_SVR()) 
					//{
					//	if(DEFAULT_SVR()->GetOrder(orderKey, rawData))
					//		newText = CFieldValueTool::OrderStatus2String(rawData.OrderStatus, SVR_LANGUAGE);
					//}
				}
			}
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_StopLoseState);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = "-";
		if(m_poLocalOrderService!=NULL)
		{
			if(tPLDetailOrder.plparam.bDoStopLose)
			{
				if(!tPLDetailOrder.bLoseIsServ)
				{
					CLocalOrderService::MAPCONDITIONORDERITEM it 
							= m_poLocalOrderService->GetOrderConditionMap()
							.find(tPLDetailOrder.refLose.FTID);
					CLocalOrderService::CONDITIONORDER tCondOrder = {0};
					tCondOrder.nState = -1;
					if(it != m_poLocalOrderService->GetOrderConditionMap().end())
						tCondOrder = it->second;
					switch(tCondOrder.nState) {
					case conCondPause:
						newText = LOADSTRING(OIP_PAUSE);
						break;
					case conCondActive:
						newText = LOADSTRING(OIP_ACTIVE);
						break;
					case conCondDelete:
						newText = LOADSTRING(OIP_DELETEED);
						break;
					case conCondConfirm:
						newText = LOADSTRING(OIP_SUBMITORDER);
						break;
					case conCondCancel:
						newText = LOADSTRING(OIP_SYSCANCEL);
						break;
					case conCondCompleted:
						newText = LOADSTRING(OIP_COMPLETE);
						break;
					default:
						newText = "";
						break;
					};
				}
				else
				{
					newText = CFieldValueTool::OrderStatus2String(tPLDetailOrder.refLose.OrderStatus, SVR_LANGUAGE);
					//OrderKey orderKey(tPLDetailOrder.refLose.Account, tPLDetailOrder.InstrumentID, 
					//		tPLDetailOrder.refLose.FrontID, tPLDetailOrder.refLose.SessionID, 
					//		tPLDetailOrder.refLose.OrderRef);
					//PlatformStru_OrderInfo rawData;
					//if(DEFAULT_SVR()) 
					//{
					//	if(DEFAULT_SVR()->GetOrder(orderKey, rawData))
					//		newText = CFieldValueTool::OrderStatus2String(rawData.OrderStatus, SVR_LANGUAGE);
					//}
				}
			}
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_InstrumentID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%s", tPLDetailOrder.InstrumentID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_CloseDirection);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%s", tPLDetailOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY) : LOADSTRING(OLS_SELL));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_ReqVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPLDetailOrder.orderSrc.nVolume);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_StopGainPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopGainPrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_StopLosePrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopLosePrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_StopGainClosePrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopGainForecast).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_StopLoseClosePrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopLoseForecast).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_PriceType);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", (tPLDetailOrder.plparam.nPriceType==1 ? 
				LOADSTRING(OLS_LASTPRICE3) : 
				(tPLDetailOrder.plparam.nPriceType==2 ? LOADSTRING(OLS_B_S_PRICE3) : 
					tPLDetailOrder.plparam.nPriceType==3 ? 
					LOADSTRING(OLS_BIDPRICE):LOADSTRING(OLS_ASKPRICE))));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_CloseMode);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPLDetailOrder.plparam.nCloseMode==1 ? 
				LOADSTRING(OLS_UPLWPRICE): LOADSTRING(OLS_LIMITPRICE));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSIPL_ClosePriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPLDetailOrder.plparam.fClosePriceOffset).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	return 0;
}

void CQueryOrderPosiPLPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
	//FindWindowById(GID_QUERY_ORDER_POSITION,this)->SetPosition(wxPoint(0,30));
	wxWindow* pWin = NULL;
	for(int i=ID_QUERYPOSIPL_BUTTONQUERY; i<ID_QUERYPOSIPL_NUM; i++)
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
void CQueryOrderPosiPLPanel::OnButtonQuery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSIPL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	//ShowOrderPositions(bIsShowDelete);
	ShowOrderPositions(TRUE);
}

BOOL CQueryOrderPosiPLPanel::IsMayEdit(CLocalOrderService::PLDETAILORDER& tOrder, 
										  BOOL& bHasServNoTouch)
{
	if(m_poLocalOrderService==NULL)
		return FALSE;
	if(tOrder.bGainIsServ || tOrder.bLoseIsServ) 
	{
		if(tOrder.bGainIsServ && tOrder.refGain.OrderStatus==THOST_FTDC_OST_NotTouched)
			bHasServNoTouch = TRUE;
		if(tOrder.bLoseIsServ && tOrder.refLose.OrderStatus==THOST_FTDC_OST_NotTouched)
			bHasServNoTouch = TRUE;
		return FALSE;
	}
	CLocalOrderService::MAPCONDITIONORDERITEM itCond;
	BOOL bIsMay_Gain = TRUE, bIsMay_Lose = TRUE;
	//CLocalOrderService::CONDITIONORDER condOrder;
	//ZeroMemory(&condOrder, sizeof(condOrder));
	if(!tOrder.bGainIsServ && tOrder.plparam.bDoStopGain)
	{
		m_poLocalOrderService->LockObject();
		itCond = m_poLocalOrderService->GetOrderConditionMap().find(tOrder.refGain.FTID);
		if(itCond!=m_poLocalOrderService->GetOrderConditionMap().end()) 
			bIsMay_Gain = itCond->second.nState<=conCondActive;
		m_poLocalOrderService->UnlockObject();
	}
	if(!tOrder.bLoseIsServ && tOrder.plparam.bDoStopLose)
	{
		m_poLocalOrderService->LockObject();
		itCond = m_poLocalOrderService->GetOrderConditionMap().find(tOrder.refLose.FTID);
		if(itCond!=m_poLocalOrderService->GetOrderConditionMap().end()) 
			bIsMay_Lose = itCond->second.nState<=conCondActive;
		m_poLocalOrderService->UnlockObject();
	}
	return bIsMay_Gain && bIsMay_Lose && (tOrder.plparam.bDoStopGain || tOrder.plparam.bDoStopLose);
}

void CQueryOrderPosiPLPanel::OnButtonEdit(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, int>::iterator itr2k = m_OrderPositionRow2Key.find(SelectItemId);
	if(itr2k == m_OrderPositionRow2Key.end()) return;

	int nPLDetailNum=itr2k->second;
	
	if(nPLDetailNum<=0) return;
	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();
	CLocalOrderService::MAPPLDETAILORDER& mapPLDetail = m_poLocalOrderService->GetOrderPLDetailMap();
	CLocalOrderService::MAPPLDETAILORDERITEM it = mapPLDetail.find(nPLDetailNum);

	if(it == mapPLDetail.end()) {
		m_poLocalOrderService->UnlockObject();
		wxMessageDialog dialog( NULL, LOADSTRING(PROFIT_ERROR_MESSAGE), 
				LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	BOOL bHasServNoTouch = FALSE;
	if(!IsMayEdit(it->second, bHasServNoTouch))
	{
		m_poLocalOrderService->UnlockObject();
		if(bHasServNoTouch)
		{
			// 提示带服务器条件单的明细不能编辑
			wxMessageDialog dialog( NULL,LOADSTRING(PLDETAIL_ERROR_MESSAGE_SERVNOTOUCH), 
					LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
		else
		{
			// 提示带服务器条件单的明细不能编辑
			wxMessageDialog dialog( NULL,LOADSTRING(PLDETAIL_ERROR_MESSAGE), 
					LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
		return;
	}

	CLocalOrderService::PLDETAILORDER tPLDOrder={0};
	tPLDOrder = it->second;
	m_poLocalOrderService->UnlockObject();

	PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
	if(DEFAULT_SVR() 
			&& DEFAULT_SVR()->GetInstrumentInfo(tPLDOrder.InstrumentID,insInfo)!=0) 
		insInfo.PriceTick = 1.0;

	bool bFail=false;
	BOOL bIsAbsCondPrice = TRUE;
	//CEditOrderPLDetailDailog* pDlg = new CEditOrderPLDetailDailog(this, wxID_ANY, LOADSTRING(CONDITIONCURRENT_TITLE), wxDefaultPosition, wxSize(310, 250));
	CEditOrderPosiPLDailog* pDlg = new CEditOrderPosiPLDailog(this, wxID_ANY, LOADSTRING(CONDITIONCURRENT_TITLE), wxDefaultPosition, wxSize(310, 250));
	//pDlg->SetPriceTick(insInfo.PriceTick);
	pDlg->SetPLDetailOrder(tPLDOrder);
	pDlg->Init(bIsAbsCondPrice);
	if(pDlg->ShowModal()==wxID_OK) {
		tPLDOrder = pDlg->GetPLDetailOrder();

		// 同步加锁，保证数据的一致性
		m_poLocalOrderService->LockObject();

		// 判断修改期间止盈止损单是否成交
		it = mapPLDetail.find(nPLDetailNum);
		if(it!=mapPLDetail.end())
		{
			PlatformStru_DepthMarketData tInstrfield;
            ZeroMemory(&tInstrfield, sizeof(tInstrfield));
			// 从服务器更新价格
			if(DEFAULT_SVR()) 
				DEFAULT_SVR()->GetQuotInfo(tPLDOrder.InstrumentID, tInstrfield);

			CLocalOrderService::MAPCONDITIONORDER& mapCondition = m_poLocalOrderService->GetOrderConditionMap();
			CLocalOrderService::MAPCONDITIONORDERITEM itCond;
			
			int nCondLoseNum = tPLDOrder.refLose.FTID;

			// 修改止损条件单
			if(tPLDOrder.plparam.bDoStopLose) {
				CLocalOrderService::CONDITIONORDER tOrderLose={0};

				itCond = mapCondition.find(nCondLoseNum);
				if(itCond != mapCondition.end()) {
					tOrderLose = itCond->second;
				}
				else {
					tOrderLose.nPLDetailNum = tPLDOrder.FTID;
					tOrderLose.nCondPLRelation = conCPRCondFromPL;
					tOrderLose.nState = conCondActive;
					tOrderLose.orderSrc = tPLDOrder.orderSrc;
					strncpy(tOrderLose.InstrumentID, tPLDOrder.InstrumentID, sizeof(tOrderLose.InstrumentID)-1);
				}
				
				tOrderLose.cond.nPriceType = (tPLDOrder.plparam.nPriceType==1 ? tPLDOrder.plparam.nPriceType : 
						(tPLDOrder.plparam.nPriceType==2 ? (tOrderLose.orderSrc.bIsBuy ? 3:2) : tPLDOrder.plparam.nPriceType-1));
				//tOrderLose.cond.nConditionType = (tOrderLose.orderSrc.bIsBuy ? 2:1);

				double fOffPrice = (tPLDOrder.plparam.nPriceOffsetType==1 ? tPLDOrder.fOpenedPriceOffset : 0.0);
				if(!tPLDOrder.orderSrc.bIsBuy) {
					if(bIsAbsCondPrice) 
						tOrderLose.cond.fConditionPrice = tPLDOrder.fStopLosePrice;
					else 
						tOrderLose.cond.fConditionPrice = 
							tPLDOrder.orderSrc.fPrice + fOffPrice - tPLDOrder.plparam.fStopLosePriceOffset;
					tOrderLose.cond.nConditionType = 1;
				}
				else {
					if(bIsAbsCondPrice) 
						tOrderLose.cond.fConditionPrice = tPLDOrder.fStopLosePrice;
					else
						tOrderLose.cond.fConditionPrice = 
								tPLDOrder.orderSrc.fPrice + fOffPrice + tPLDOrder.plparam.fStopLosePriceOffset;
					tOrderLose.cond.nConditionType = 2;
				}
				if(tPLDOrder.plparam.nCloseMode==conPLCMBound)
				{
					if(tPLDOrder.orderSrc.bIsBuy)
						tOrderLose.orderSrc.fPrice = tInstrfield.UpperLimitPrice;
					else
						tOrderLose.orderSrc.fPrice = tInstrfield.LowerLimitPrice;
				}
				else 
				{
					if(tPLDOrder.orderSrc.bIsBuy)
						tOrderLose.orderSrc.fPrice = tOrderLose.cond.fConditionPrice
								+tPLDOrder.plparam.fClosePriceOffset;
					else
						tOrderLose.orderSrc.fPrice = tOrderLose.cond.fConditionPrice
								-tPLDOrder.plparam.fClosePriceOffset;
				}
				//tOrderLose.nPriceType = 1;
				tPLDOrder.fStopLosePrice = tOrderLose.cond.fConditionPrice;
				tPLDOrder.fStopLoseForecast = tOrderLose.orderSrc.fPrice;

				if(itCond != mapCondition.end()) 
					m_poLocalOrderService->OrderConditionSet(nCondLoseNum, tOrderLose);
				else 
					nCondLoseNum = m_poLocalOrderService->OrderConditionAdd(tOrderLose);
			}
			else {
				tPLDOrder.fStopLosePrice = util::GetDoubleInvalidValue();
				tPLDOrder.fStopLoseForecast = util::GetDoubleInvalidValue();
				m_poLocalOrderService->OrderConditionDelete(tPLDOrder.refLose.FTID);
				nCondLoseNum = 0;
			}

			int nCondGainNum = tPLDOrder.refGain.FTID;

			// 修改止盈条件单
			if(tPLDOrder.plparam.bDoStopGain) {
				CLocalOrderService::CONDITIONORDER tOrderGain={0};

				itCond = mapCondition.find(nCondGainNum);
				if(itCond != mapCondition.end()) {
					tOrderGain = itCond->second;
				}
				else {
					tOrderGain.nPLDetailNum = tPLDOrder.FTID;
					tOrderGain.nCondPLRelation = conCPRCondFromPL;
					tOrderGain.nState = conCondActive;
					tOrderGain.orderSrc = tPLDOrder.orderSrc;
					strncpy(tOrderGain.InstrumentID, tPLDOrder.InstrumentID, sizeof(tOrderGain.InstrumentID)-1);
				}

				tOrderGain.cond.nPriceType = (tPLDOrder.plparam.nPriceType==1 ? tPLDOrder.plparam.nPriceType : 
						(tPLDOrder.plparam.nPriceType==2 ? (tOrderGain.orderSrc.bIsBuy ? 3:2) : tPLDOrder.plparam.nPriceType-1));
				//tOrderGain.cond.nConditionType = (tOrderGain.orderSrc.bIsBuy ? 1:2);

				double fOffPrice = tPLDOrder.plparam.nPriceOffsetType==1 ? tPLDOrder.fOpenedPriceOffset : 0.0;
				if(!tPLDOrder.orderSrc.bIsBuy) {
					if(bIsAbsCondPrice) 
						tOrderGain.cond.fConditionPrice = tPLDOrder.fStopGainPrice;
					else 
						tOrderGain.cond.fConditionPrice = 
								tPLDOrder.orderSrc.fPrice + fOffPrice + tPLDOrder.plparam.fStopGainPriceOffset;
					tOrderGain.cond.nConditionType = 2;
				}
				else {
					if(bIsAbsCondPrice) 
						tOrderGain.cond.fConditionPrice = tPLDOrder.fStopGainPrice;
					else 
						tOrderGain.cond.fConditionPrice = 
								tPLDOrder.orderSrc.fPrice + fOffPrice - tPLDOrder.plparam.fStopGainPriceOffset;
					tOrderGain.cond.nConditionType = 1;
				}
				if(tPLDOrder.plparam.nCloseMode==conPLCMBound)
				{
					if(tPLDOrder.orderSrc.bIsBuy)
						tOrderGain.orderSrc.fPrice = tInstrfield.UpperLimitPrice;
					else
						tOrderGain.orderSrc.fPrice = tInstrfield.LowerLimitPrice;
				}
				else 
				{
					if(tPLDOrder.orderSrc.bIsBuy)
						tOrderGain.orderSrc.fPrice = tOrderGain.cond.fConditionPrice
								+tPLDOrder.plparam.fClosePriceOffset;
					else
						tOrderGain.orderSrc.fPrice = tOrderGain.cond.fConditionPrice
								-tPLDOrder.plparam.fClosePriceOffset;
				}
				//tOrderGain.nPriceType = 1;
				tPLDOrder.fStopGainPrice = tOrderGain.cond.fConditionPrice;
				tPLDOrder.fStopGainForecast = tOrderGain.orderSrc.fPrice;

				if(itCond != mapCondition.end()) 
					m_poLocalOrderService->OrderConditionSet(nCondGainNum, tOrderGain);
				else 
					nCondGainNum = m_poLocalOrderService->OrderConditionAdd(tOrderGain);
			}
			else {
				tPLDOrder.fStopGainPrice = util::GetDoubleInvalidValue();
				tPLDOrder.fStopGainForecast = util::GetDoubleInvalidValue();
				m_poLocalOrderService->OrderConditionDelete(tPLDOrder.refGain.FTID);
				nCondGainNum = 0;
			}

			tPLDOrder.refLose.FTID = nCondLoseNum;
			tPLDOrder.refGain.FTID = nCondGainNum;

			m_poLocalOrderService->OrderPLDetailSet(nPLDetailNum, tPLDOrder);

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
		else
			ShowOrderPositions(TRUE);
	}
	pDlg->Destroy();
}

wxString CQueryOrderPosiPLPanel::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
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

wxString CQueryOrderPosiPLPanel::GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl)
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

void CQueryOrderPosiPLPanel::OnButtonDelete(wxCommandEvent& evt)
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

	CLocalOrderService::MAPPLDETAILORDERITEM it;
	BOOL bIsFound = FALSE;
	CLocalOrderService::PLDETAILORDER plDetailOrder = {0};
	
	m_poLocalOrderService->LockObject();
	it = m_poLocalOrderService->GetOrderPLDetailMap().find(nPositionNum);
	bIsFound = it!=m_poLocalOrderService->GetOrderPLDetailMap().end();
	if(bIsFound)
		plDetailOrder = it->second;
	m_poLocalOrderService->UnlockObject();
	if(plDetailOrder.nState==conPosiPLDelete)
		bIsFound = FALSE;
	if(!bIsFound)
	{
		wxMessageDialog dialog( NULL,LOADSTRING(MSG_POS_DELETEFIALED) ,LOADSTRING(OIP_ERROR),wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	int nRetGain = 0, nRetLose = 0;
	//m_poLocalOrderService->LockObject();
	//BOOL bRet = m_poLocalOrderService->OrderPLDelete(nPositionNum, conPLDelete, TRUE);
	//m_poLocalOrderService->UnlockObject();

	USERLOG_INFO("用户单击持仓止盈止损模块按钮[撤单]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	CancelServCond(plDetailOrder, nRetGain, nRetLose);

	if(nRetGain!=0 || nRetLose!=0) 
	{
		wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
	}
	else {
		ShowOrderPositions(TRUE);
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

void CQueryOrderPosiPLPanel::OnButtonClear(wxCommandEvent& evt)
{
#if 1
	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();

	CLocalOrderService::MAPPLDETAILORDER& mapPLDetail = m_poLocalOrderService->GetOrderPLDetailMap();
	int nCount = 0;

	nCount += mapPLDetail.size();

	m_poLocalOrderService->UnlockObject();

	if(nCount == 0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	CLocalOrderService::MAPPLDETAILORDERITEM it;
	std::vector<int> arrDelete;
	std::vector<int>::iterator itDel;

	wxMessageDialog dialog( NULL, LOADSTRING(MSG_POS_CLEARNOW), LOADSTRING(OIP_FASTTRADER), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES) return;

	m_poLocalOrderService->LockObject();

	it = mapPLDetail.begin();
	while(it!=mapPLDetail.end()) {
		arrDelete.push_back(it->first);
		it++;
	}
	
	nCount = (int)arrDelete.size();
	for(int i=0; i<nCount; i++) {
		m_poLocalOrderService->OrderPLDetailDelete(arrDelete[i], conPosiPLDelete, i>=nCount-1);
	}
	arrDelete.clear();

	m_poLocalOrderService->UnlockObject();

	//ShowOrderPositions(FALSE);
#endif
}

void CQueryOrderPosiPLPanel::OnActivated(wxExtListEvent& event)
{
}

//配置文件发生变化
void CQueryOrderPosiPLPanel::OnCfgChanged(wxCommandEvent& evt)
{
	Init();
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSIPL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	//ShowOrderPositions(bIsShowDelete);
	ShowOrderPositions(TRUE);
}

void CQueryOrderPosiPLPanel::OnInitSubscribe(wxCommandEvent& evt)
{
}

void CQueryOrderPosiPLPanel::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	BOOL bIsRowCLick=evt.GetId();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏

	menu.Append(ID_QUERYPOSIPL_BUTTONQUERY,LOADSTRING(CONTEXTMENU_QP_BUTTONQUERY));
	menu.AppendSeparator();
	if(evt.GetId())
	{
		menu.Append(ID_QUERYPOSIPL_BUTTONEDIT,LOADSTRING(CONTEXTMENU_QP_BUTTONEDIT));
		menu.Append(ID_QUERYPOSIPL_BUTTONDELETE,LOADSTRING(CONTEXTMENU_QP_BUTTONDELETE));
		menu.Append(ID_QUERYPOSIPL_BUTTONCLEAR,LOADSTRING(CONTEXTMENU_QP_BUTTONCLEAR));
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
		case ID_QUERYPOSIPL_BUTTONQUERY://市价反手
			newevt.SetId(ID_QUERYPOSIPL_BUTTONQUERY);
			OnButtonQuery(newevt);
			break;
		case ID_QUERYPOSIPL_BUTTONEDIT://市价反手
			newevt.SetId(ID_QUERYPOSIPL_BUTTONEDIT);
			OnButtonEdit(newevt);
			break;
		case ID_QUERYPOSIPL_BUTTONDELETE://市价反手
			newevt.SetId(ID_QUERYPOSIPL_BUTTONDELETE);
			OnButtonDelete(newevt);
			break;
		case ID_QUERYPOSIPL_BUTTONCLEAR://市价反手
			newevt.SetId(ID_QUERYPOSIPL_BUTTONCLEAR);
			OnButtonClear(newevt);
			break;
		case ID_MENU_EXPORT://导出列表
			{
                wxString cap=((FrameBase*)GETTOPWINDOW())->GetModuleCaption(GetId());
				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
			SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_ORDER_POSIPL);
			break;
		default:
			break;
		}

        LeaveCriticalSection(&m_CS_EvtCallback);
    }
    evt.Skip();
}

void CQueryOrderPosiPLPanel::OnRequery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSIPL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	//ShowOrderPositions(bIsShowDelete);
	ShowOrderPositions(TRUE);
}

BOOL CQueryOrderPosiPLPanel::NeedOrderFailSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderFailSound;
}

BOOL CQueryOrderPosiPLPanel::NeedServCondOverBoundConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bReachUpperLimitDlg;
}

BOOL CQueryOrderPosiPLPanel::NeedServCondOverBoundSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bReachUpperLimitSound;
}

BOOL CQueryOrderPosiPLPanel::CancelServCond(CLocalOrderService::PLDETAILORDER& plDetailOrder, 
											   int nRetGain, int nRetLose)
{
	nRetGain=0;
	nRetLose=0;
	if(plDetailOrder.bGainIsServ)
	{
		if(plDetailOrder.refGain.OrderStatus==THOST_FTDC_OST_NotTouched)
		{
			OrderKey orderKey(plDetailOrder.refGain.Account, plDetailOrder.InstrumentID, 
					plDetailOrder.refGain.FrontID, plDetailOrder.refGain.SessionID, 
					plDetailOrder.refGain.OrderRef);
			PlatformStru_OrderInfo outData;
			if(DEFAULT_SVR()!=NULL &&
					DEFAULT_SVR()->GetOrder(orderKey, outData))
			{
				PlatformStru_InputOrderAction ReqData;
				memset(&ReqData, 0, sizeof(ReqData));
				ReqData.Thost.FrontID=outData.FrontID;
				ReqData.Thost.SessionID=outData.SessionID;
				memcpy(ReqData.Thost.OrderRef, outData.OrderRef, sizeof(ReqData.Thost.OrderRef)-1);
				memcpy(ReqData.Thost.OrderSysID, outData.OrderSysID, sizeof(ReqData.Thost.OrderSysID)-1);
				memcpy(ReqData.Thost.ExchangeID, outData.ExchangeID, sizeof(ReqData.Thost.ExchangeID)-1);
				memcpy(ReqData.Thost.InstrumentID, outData.InstrumentID, sizeof(ReqData.Thost.InstrumentID)-1);
				nRetGain=DEFAULT_SVR()?
						DEFAULT_SVR()->ReqOrderAction(ReqData):-1;
			}
			else
				nRetGain=-1;
		}
	}
	else
	{
		m_poLocalOrderService->OrderConditionDelete(plDetailOrder.refGain.FTID, 
				conCondDelete, TRUE);
	}
	if(plDetailOrder.bLoseIsServ)
	{
		if(plDetailOrder.refLose.OrderStatus==THOST_FTDC_OST_NotTouched)
		{
			OrderKey orderKey(plDetailOrder.refLose.Account, plDetailOrder.InstrumentID, 
					plDetailOrder.refLose.FrontID, plDetailOrder.refLose.SessionID, 
					plDetailOrder.refLose.OrderRef);
			PlatformStru_OrderInfo outData;
			if(DEFAULT_SVR()!=NULL &&
					DEFAULT_SVR()->GetOrder(orderKey, outData))
			{
				PlatformStru_InputOrderAction ReqData;
				memset(&ReqData, 0, sizeof(ReqData));
				ReqData.Thost.FrontID=outData.FrontID;
				ReqData.Thost.SessionID=outData.SessionID;
				memcpy(ReqData.Thost.OrderRef, outData.OrderRef, sizeof(ReqData.Thost.OrderRef)-1);
				memcpy(ReqData.Thost.OrderSysID, outData.OrderSysID, sizeof(ReqData.Thost.OrderSysID)-1);
				memcpy(ReqData.Thost.ExchangeID, outData.ExchangeID, sizeof(ReqData.Thost.ExchangeID)-1);
				memcpy(ReqData.Thost.InstrumentID, outData.InstrumentID, sizeof(ReqData.Thost.InstrumentID)-1);
				nRetLose=DEFAULT_SVR()?
						DEFAULT_SVR()->ReqOrderAction(ReqData):-1;
			}
			else
				nRetLose=-1;
		}
	}
	else
	{
		m_poLocalOrderService->OrderConditionDelete(plDetailOrder.refLose.FTID, 
				conCondDelete, TRUE);
	}
	return nRetGain==0 && nRetLose==0;
}

void CQueryOrderPosiPLPanel::OnPLDetailStatusChanged(wxCommandEvent& evt)
{
	wxCommandEvent evt2(wxEVT_COMMAND_BUTTON_CLICKED);
	OnButtonQuery(evt2);
}

void CQueryOrderPosiPLPanel::OnServCondOverBoundFail(wxCommandEvent& evt)
{
	long nNum = evt.GetInt();
	int nMode = evt.GetExtraLong();
	wxString strInstrumentID = evt.GetString();
	int nTimeOut = 10;
	wxString strText;
	strText.Printf(LOADFORMATSTRING(CONDS2L_OverBoundFailMSG, "%s%s"), 
			strInstrumentID.c_str(), (nMode==0 ? LOADSTRING(CONDS2L_OverBoundFail_COND) : 
			(nMode==1 ? LOADSTRING(CONDS2L_OverBoundFail_SP) : LOADSTRING(CONDS2L_OverBoundFail_SL))));

	{
		if(TRADEINFODLG(this)!=NULL)
		{
			LOG_DATA* pData = new LOG_DATA;
			pData->strTitle = std::string(LOADSTRING(CONDS2L_OverBoundFailTitle));
			pData->strFormat = std::string(strText.c_str());
			TRADEINFODLG(this)->WriteLog(pData);
		}
	}
//
	CZqMessageBoxEx* pmsgBox = new CZqMessageBoxEx(GETTOPWINDOW());
	pmsgBox->ResetMessage(wxString(LOADSTRING(CONDS2L_OverBoundFailTitle)), strText, 
			CZqMessageBoxEx::btnYes|CZqMessageBoxEx::btnNo, 
			CZqMessageBoxEx::condServOverBound);
	pmsgBox->SetCondInfo(NULL, nNum, nMode);
	if(!NeedServCondOverBoundConfirm()) // 如果不需要弹出对话框，就进行自动转换
	{
		pmsgBox->DoCond(TRUE);
		pmsgBox->Destroy();
	}
	else
		pmsgBox->Show();
	if(NeedServCondOverBoundSound())
		NoticeSound(conNoticeNewOrderFail);
}

void CQueryOrderPosiPLPanel::OnLanguageChange(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSIPL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	//ShowOrderPositions(bIsShowDelete);
	ShowOrderPositions(TRUE);

	TRANSFORM_COLUMN_TEXT(GID_QUERY_ORDER_POSIPL,m_pwxExtListCtrl);
	FindWindowById(ID_QUERYPOSIPL_CHECKSHOWDELETE,this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	m_pButtonQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	m_pButtonEdit->SetLabel(LOADSTRING(BTN_QP_BUTTONEDIT));
	m_pButtonDelete->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	m_pButtonClear->SetLabel(LOADSTRING(BTN_QP_BUTTONCLEAR));
}

void CQueryOrderPosiPLPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_POSIPL ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSIPL, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}

void CQueryOrderPosiPLPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_POSIPL ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_POSIPL, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}
