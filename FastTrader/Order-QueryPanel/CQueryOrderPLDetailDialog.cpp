#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderPLDetailDialog.h"
#include "../inc/Order-Common/CLocalOrderService.h"
#include "../ConfigApp/Const.h"
#include "wx/file.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/ZqMessageBoxEx.h"

#include "CEditOrderPLDetailDailog.h"
#include "gui-common/FrameBase.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


CRITICAL_SECTION CQueryOrderPLDetailDialog::m_CS_EvtCallback;


BEGIN_EVENT_TABLE(CQueryOrderPLDetailDialog, wxDialog)//wxPanel)
	EVT_SIZE(OnSize )
	EVT_CLOSE(OnClose)
	EVT_CHECKBOX(ID_QUERYPLDETAIL_CHECKSHOWDELETE, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPLDETAIL_BUTTONQUERY, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPLDETAIL_BUTTONEDIT, OnButtonEdit)
	EVT_BUTTON(ID_QUERYPLDETAIL_BUTTONDELETE, OnButtonDelete)
	EVT_BUTTON(ID_QUERYPLDETAIL_BUTTONCLEAR, OnButtonClear)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_STOP_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPLDETAIL_STATUSCHANGED, OnPLDetailStatusChanged)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderPLDetailDialog::CQueryOrderPLDetailDialog(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name):
//	wxPanel(parent,winid, pos,size, style, name),
	wxDialog(parent,winid,wxEmptyString, pos,size, style, name),
	m_poLocalOrderService(NULL),
	m_poOwner(NULL)
{
    
	SetBackgroundColour(DEFAULT_COLOUR);
	m_nPLNum = -1;

	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_QUERYPLDETAIL_CHECKSHOWDELETE, 
			LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(5, 3+8), wxSize(14*8, 14));//, 0 );
	itemCheckBox1->Show(false);

	m_pButtonQuery = new wxButton(this, ID_QUERYPLDETAIL_BUTTONQUERY, LOADSTRING(BTN_QP_BUTTONQUERY), 
			wxPoint(0, 5), wxSize(53, 20), 0, wxDefaultValidator);//140
	m_pButtonEdit = new wxButton(this, ID_QUERYPLDETAIL_BUTTONEDIT, LOADSTRING(BTN_QP_BUTTONEDIT), 
			wxPoint(55, 5), wxSize(53, 20), 0, wxDefaultValidator);//140
	m_pButtonDelete = new wxButton(this, ID_QUERYPLDETAIL_BUTTONDELETE, LOADSTRING(BTN_QP_BUTTONDELETE), 
			wxPoint(110, 5), wxSize(53, 20), 0, wxDefaultValidator);//140
	m_pButtonClear = new wxButton(this, ID_QUERYPLDETAIL_BUTTONCLEAR, LOADSTRING(BTN_QP_BUTTONCLEAR), 
			wxPoint(165, 5), wxSize(80, 20), 0, wxDefaultValidator);//140
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY,//GID_QUERY_ORDER_POSITION, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	//Init();

	m_poLocalOrderService = CLocalOrderService::GetInstance();
	//m_poLocalOrderService->RegisterMsgWindow(this);
    InitializeCriticalSection(&m_CS_EvtCallback);

}

CQueryOrderPLDetailDialog::~CQueryOrderPLDetailDialog(void)
{
	//m_poLocalOrderService->UnregisterMsgWindow(this);

    DeleteCriticalSection(&m_CS_EvtCallback);
}

bool CQueryOrderPLDetailDialog::Reinit()
{
	return Init(m_nPLNum);
}

bool CQueryOrderPLDetailDialog::Init(long nPLNum)
{
	m_nPLNum = nPLNum;
	
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_PLDETAIL);//
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

	//nColCount = pCfgMgr->GetListColCount(GID_QUERY_ORDER_PLDETAIL);
	nColCount = pCfgMgr->GetListVisibleColCount(GID_QUERY_ORDER_PLDETAIL);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0;  i < nColCount; i++) {
		//LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_PLDETAIL, i);
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_PLDETAIL, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<ORDERPLDETAIL_MIN || pColCfg->id>ORDERPLDETAIL_MAX)
			continue;

		//"合约" "报单编号"栏，左对齐
		if( (pColCfg->id == ORDERPLDETAIL_InstrumentID) || (pColCfg->id == ORDERPLDETAIL_ConditionID) )
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余的，右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        if(pColCfg->Visibility == 0)
			col.SetWidth(0);
        else
			col.SetWidth(pColCfg->Width);
        if(pColCfg->Visibility != 0) {		
            col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_PLDETAIL,pColCfg->id));
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

	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_QUERYPLDETAIL_BUTTONQUERY);
	OnButtonQuery(evt);
    return true;
}

BOOL CQueryOrderPLDetailDialog::CancelServCond(CLocalOrderService::PLDETAILORDER& plDetailOrder, 
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

void CQueryOrderPLDetailDialog::ShowOrderPLDetails(BOOL bIsShowDelete)
{
	int iRowNo = 0, iCol = 0, iSumCol = 0;

	m_pwxExtListCtrl->DeleteAllItems();
	m_OrderPLDetailKey2Row.clear();
	m_OrderPLDetailRow2Key.clear();

	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();
	CLocalOrderService::MAPPLDETAILORDER mapPLDetail = m_poLocalOrderService->GetOrderPLDetailMap();
	m_poLocalOrderService->UnlockObject();
	CLocalOrderService::MAPPLDETAILORDERITEM it = mapPLDetail.begin();

	iRowNo = -1;
	
	for(; it!=mapPLDetail.end(); ++it)	{
		CLocalOrderService::PLDETAILORDER& tPLDetailOrder = it->second;

		if(tPLDetailOrder.nPLNum != m_nPLNum)
			continue;
		//if(!bIsShowDelete && tPLDetailOrder.nState==conPLDetailDelete)
		//	continue;

		++iRowNo;
		if(ShowOneOrder(iRowNo, tPLDetailOrder) < 0) {
			--iRowNo;
			continue;
		}
		else 
		{
			m_OrderPLDetailKey2Row[tPLDetailOrder.FTID] = iRowNo;
			m_OrderPLDetailRow2Key[iRowNo] = tPLDetailOrder.FTID;
		}
	}
}

int CQueryOrderPLDetailDialog::ShowOneOrder(int iRowNo, 
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
	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_PLDetailID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPLDetailOrder.FTID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_PLID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPLDetailOrder.nPLNum);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_TraderID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%s", tPLDetailOrder.ref.TradeID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_ConditionID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = "";
		wxString strTmp;
		if(tPLDetailOrder.plparam.bDoStopGain)
		{
			if(!tPLDetailOrder.bGainIsServ)
				strTmp.Printf("zy:%d, ", tPLDetailOrder.refGain.FTID);
			else
				strTmp.Printf("zy:%s, ", tPLDetailOrder.refGain.OrderSysID);
			newText = strTmp;
		}
		if(tPLDetailOrder.plparam.bDoStopLose)
		{
			if(!tPLDetailOrder.bLoseIsServ)
				strTmp.Printf("zs:%d", tPLDetailOrder.refLose.FTID);
			else
				strTmp.Printf("zs:%s", tPLDetailOrder.refLose.OrderSysID);
			newText += strTmp;
		}
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopProfitState);
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

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopLoseState);
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

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_InstrumentID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%s", tPLDetailOrder.InstrumentID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_Direction);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	newText.Printf("%s", tPLDetailOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_CloseDirection);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%s", tPLDetailOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY) : LOADSTRING(OLS_SELL));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_RegVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPLDetailOrder.orderSrc.nVolume);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_RegPrice);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	if(tPLDetailOrder.orderSrc.bIsMarket)
	//		newText = LOADSTRING(OLS_MARKET);
	//	else
	//		newText = Double2String(tPLDetailOrder.orderSrc.fPrice).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_OpenedPriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPLDetailOrder.fOpenedPriceOffset).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopGainPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopGainPrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopLosePrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopLosePrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopGainPriceOffset);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	if(util::equals(tPLDetailOrder.plparam.fStopGainPriceOffset, 0.0))
	//		newText = "-";
	//	else
	//		newText = Double2String(tPLDetailOrder.plparam.fStopGainPriceOffset).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopLosePriceOffset);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;
	//	if(util::equals(tPLDetailOrder.plparam.fStopLosePriceOffset, 0.0))
	//		newText = "-";
	//	else
	//		newText = Double2String(tPLDetailOrder.plparam.fStopLosePriceOffset).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopGainForecast);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopGainForecast).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPLDETAIL_StopLoseForecast);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText = Double2String(tPLDetailOrder.fStopLoseForecast).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_PriceType);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;	
	//	newText.Printf("%s", tPLDetailOrder.plparam.nPriceType==1 ? 
	//			LOADSTRING(OLS_LASTPRICE3) : LOADSTRING(OLS_B_S_PRICE3));
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_CloseMode);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;	
	//	newText.Printf("%s", tPLDetailOrder.plparam.nCloseMode==1 ? 
	//			LOADSTRING(OLS_UPLWPRICE): LOADSTRING(OLS_LIMITPRICE));
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	//it = m_FieldID2ColumnID.find(ORDERPLDETAIL_ClosePriceOffset);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;	
	//	newText = Double2String(tPLDetailOrder.plparam.fClosePriceOffset).c_str();
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	return 0;
}

void CQueryOrderPLDetailDialog::OnSize( wxSizeEvent& event )
{
	wxRect rt=this->GetClientRect();
	//FindWindowById(GID_QUERY_ORDER_POSITION,this)->SetPosition(wxPoint(0,30));
	wxWindow* pWin = NULL;
	for(int i=ID_QUERYPLDETAIL_BUTTONQUERY; i<ID_QUERYPLDETAIL_NUM; i++)
	{
		pWin = FindWindowById(i, this);
		if(pWin)
		{
			wxPoint pos = pWin->GetPosition();
			//pos.y=-30;
			pWin->SetPosition(pos);
		}
	}
	if(m_pwxExtListCtrl)
	{	
		m_pwxExtListCtrl->SetPosition(wxPoint(0, 30));
		m_pwxExtListCtrl->SetSize(rt.width,rt.height-30);
	}
	event.Skip();
}

/*撤单按钮事件响应*/
void CQueryOrderPLDetailDialog::OnButtonQuery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPLDETAIL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPLDetails(bIsShowDelete);
}

BOOL CQueryOrderPLDetailDialog::IsMayEdit(CLocalOrderService::PLDETAILORDER& tOrder, 
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
	BOOL bIsMay_Gain = FALSE, bIsMay_Lose = FALSE;
	//CLocalOrderService::CONDITIONORDER condOrder;
	//ZeroMemory(&condOrder, sizeof(condOrder));
	if(!tOrder.bGainIsServ)
	{
		m_poLocalOrderService->LockObject();
		itCond = m_poLocalOrderService->GetOrderConditionMap().find(tOrder.refGain.FTID);
		if(itCond!=m_poLocalOrderService->GetOrderConditionMap().end()) 
			bIsMay_Gain = itCond->second.nState<=conCondActive;
		m_poLocalOrderService->UnlockObject();
	}
	if(!tOrder.bLoseIsServ)
	{
		m_poLocalOrderService->LockObject();
		itCond = m_poLocalOrderService->GetOrderConditionMap().find(tOrder.refLose.FTID);
		if(itCond!=m_poLocalOrderService->GetOrderConditionMap().end()) 
			bIsMay_Lose = itCond->second.nState<=conCondActive;
		m_poLocalOrderService->UnlockObject();
	}
	return bIsMay_Gain && bIsMay_Lose;
}

wxString CQueryOrderPLDetailDialog::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
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

wxString CQueryOrderPLDetailDialog::GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl)
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

void CQueryOrderPLDetailDialog::OnButtonEdit(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, int>::iterator itr2k = m_OrderPLDetailRow2Key.find(SelectItemId);
	if(itr2k == m_OrderPLDetailRow2Key.end()) return;

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
	CEditOrderPLDetailDailog* pDlg = new CEditOrderPLDetailDailog(this, wxID_ANY, LOADSTRING(CONDITIONCURRENT_TITLE), wxDefaultPosition, wxSize(310, 250));
	pDlg->SetPriceTick(insInfo.PriceTick);
	pDlg->SetPLDetailOrder(tPLDOrder);
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
					tOrderLose.cond.nPriceType = tPLDOrder.plparam.nPriceType==1 ? tPLDOrder.plparam.nPriceType : 
							(tOrderLose.orderSrc.bIsBuy ? 3:2);
					tOrderLose.cond.nConditionType = (tOrderLose.orderSrc.bIsBuy ? 2:1);

					double fOffPrice = tPLDOrder.plparam.nPriceOffsetType==1 ? tPLDOrder.fOpenedPriceOffset : 0.0;
					if(tPLDOrder.orderSrc.bIsBuy) {
						tOrderLose.cond.fConditionPrice = 
							tPLDOrder.orderSrc.fPrice + fOffPrice - tPLDOrder.plparam.fStopLosePriceOffset;
						tOrderLose.cond.nConditionType = 1;
					}
					else {
						tOrderLose.cond.fConditionPrice = 
								tPLDOrder.orderSrc.fPrice + fOffPrice + tPLDOrder.plparam.fStopLosePriceOffset;
						tOrderLose.cond.nConditionType = 2;
					}
					if(tPLDOrder.plparam.nCloseMode==conPLCMBound)
					{
						if(tPLDOrder.orderSrc.bIsBuy)
							tOrderLose.orderSrc.fPrice = tInstrfield.LowerLimitPrice;
						else
							tOrderLose.orderSrc.fPrice = tInstrfield.UpperLimitPrice;
					}
					else 
					{
						if(tPLDOrder.orderSrc.bIsBuy)
							tOrderLose.orderSrc.fPrice = tOrderLose.cond.fConditionPrice
									-tPLDOrder.plparam.fClosePriceOffset;
						else
							tOrderLose.orderSrc.fPrice = tOrderLose.cond.fConditionPrice
									+tPLDOrder.plparam.fClosePriceOffset;
					}
					//tOrderLose.nPriceType = 1;
					tPLDOrder.fStopLosePrice = tOrderLose.cond.fConditionPrice;
					tPLDOrder.fStopLoseForecast = tOrderLose.orderSrc.fPrice;

					m_poLocalOrderService->OrderConditionSet(nCondLoseNum, tOrderLose);
				}
			}
			else {
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
					tOrderGain.cond.nPriceType = tPLDOrder.plparam.nPriceType==1 ? tPLDOrder.plparam.nPriceType : 
							(tOrderGain.orderSrc.bIsBuy==THOST_FTDC_D_Buy ? 3:2);
					tOrderGain.cond.nConditionType = (tOrderGain.orderSrc.bIsBuy ? 1:2);

					double fOffPrice = tPLDOrder.plparam.nPriceOffsetType==1 ? tPLDOrder.fOpenedPriceOffset : 0.0;
					if(tPLDOrder.orderSrc.bIsBuy) {
						tOrderGain.cond.fConditionPrice = 
								tPLDOrder.orderSrc.fPrice + fOffPrice + tPLDOrder.plparam.fStopGainPriceOffset;
						tOrderGain.cond.nConditionType = 2;
					}
					else {
						tOrderGain.cond.fConditionPrice = 
								tPLDOrder.orderSrc.fPrice + fOffPrice - tPLDOrder.plparam.fStopGainPriceOffset;
						tOrderGain.cond.nConditionType = 1;
					}
					if(tPLDOrder.plparam.nCloseMode==conPLCMBound)
					{
						if(tPLDOrder.orderSrc.bIsBuy)
							tOrderGain.orderSrc.fPrice = tInstrfield.LowerLimitPrice;
						else
							tOrderGain.orderSrc.fPrice = tInstrfield.UpperLimitPrice;
					}
					else 
					{
						if(tPLDOrder.orderSrc.bIsBuy)
							tOrderGain.orderSrc.fPrice = tOrderGain.cond.fConditionPrice
									-tPLDOrder.plparam.fClosePriceOffset;
						else
							tOrderGain.orderSrc.fPrice = tOrderGain.cond.fConditionPrice
									+tPLDOrder.plparam.fClosePriceOffset;
					}
					//tOrderGain.nPriceType = 1;
					tPLDOrder.fStopGainPrice = tOrderGain.cond.fConditionPrice;
					tPLDOrder.fStopGainForecast = tOrderGain.orderSrc.fPrice;

					m_poLocalOrderService->OrderConditionSet(nCondGainNum, tOrderGain);
				}
			}
			else {
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
			ShowOrderPLDetails(TRUE);
	}
	pDlg->Destroy();
}

void CQueryOrderPLDetailDialog::OnButtonDelete(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, int>::iterator itr2k = m_OrderPLDetailRow2Key.find(SelectItemId);
	if(itr2k == m_OrderPLDetailRow2Key.end()) return;

	int nPLDetailNum=itr2k->second;
	
	wxExtListItem item;
	item.SetId(SelectItemId);

	if(m_poLocalOrderService==NULL) return;

	USERLOG_INFO("用户单击盈损单明细模块按钮[撤单]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	CLocalOrderService::MAPPLDETAILORDERITEM it;
	BOOL bIsFound = FALSE;
	CLocalOrderService::PLDETAILORDER plDetailOrder = {0};
	
	m_poLocalOrderService->LockObject();
	it = m_poLocalOrderService->GetOrderPLDetailMap().find(nPLDetailNum);
	bIsFound = it!=m_poLocalOrderService->GetOrderPLDetailMap().end();
	if(bIsFound)
		plDetailOrder = it->second;
	m_poLocalOrderService->UnlockObject();
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

	CancelServCond(plDetailOrder, nRetGain, nRetLose);

	if(nRetGain!=0 || nRetLose!=0) 
	{
		wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
	}
	else {
		ShowOrderPLDetails(TRUE);
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

void CQueryOrderPLDetailDialog::OnButtonClear(wxCommandEvent& evt)
{
	if(m_poLocalOrderService==NULL) return;
	
	CLocalOrderService::MAPPLDETAILORDER& mapPLDetail 
			= m_poLocalOrderService->GetOrderPLDetailMap();

	int nCount = 0;

	nCount += mapPLDetail.size();
	if(nCount == 0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	CLocalOrderService::MAPPLDETAILORDERITEM it;

	wxMessageDialog dialog( NULL, LOADSTRING(MSG_POS_CLEARNOW), LOADSTRING(OIP_FASTTRADER), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES) return;

	USERLOG_INFO("用户单击盈损单明细模块按钮[全部撤单]并选择[是]确认\n\"\n%s\n\"", 
			GetListCtrlAllRowLog(m_pwxExtListCtrl).c_str());

	m_poLocalOrderService->LockObject();

	int nRetGain = 0, nRetLose = 0;
	it = mapPLDetail.begin();
	for(; it != mapPLDetail.end(); it++) {
		if(it->second.nPLNum==m_nPLNum)
			CancelServCond(it->second, nRetGain, nRetLose);
	}

	m_poLocalOrderService->UnlockObject();
	
	ShowOrderPLDetails(TRUE);

	//ShowOrderPositions(FALSE);
}

//配置文件发生变化
void CQueryOrderPLDetailDialog::OnCfgChanged(wxCommandEvent& evt)
{
	Reinit();
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPLDETAIL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPLDetails(bIsShowDelete);
}

void CQueryOrderPLDetailDialog::OnInitSubscribe(wxCommandEvent& evt)
{
}

void CQueryOrderPLDetailDialog::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	BOOL bIsRowCLick=evt.GetId();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏

	menu.Append(ID_QUERYPLDETAIL_BUTTONQUERY,LOADSTRING(CONTEXTMENU_QP_BUTTONQUERY));
	menu.AppendSeparator();
	if(evt.GetId())
	{
		menu.Append(ID_QUERYPLDETAIL_BUTTONEDIT,LOADSTRING(CONTEXTMENU_QP_BUTTONEDIT));
		menu.Append(ID_QUERYPLDETAIL_BUTTONDELETE,LOADSTRING(CONTEXTMENU_QP_BUTTONDELETE));
		menu.Append(ID_QUERYPLDETAIL_BUTTONCLEAR,LOADSTRING(CONTEXTMENU_QP_BUTTONCLEAR));
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
		case ID_QUERYPLDETAIL_BUTTONQUERY:
			newevt.SetId(ID_QUERYPLDETAIL_BUTTONQUERY);
			OnButtonQuery(newevt);
			break;
		case ID_QUERYPLDETAIL_BUTTONEDIT:
			newevt.SetId(ID_QUERYPLDETAIL_BUTTONEDIT);
			OnButtonEdit(newevt);
			break;
		case ID_QUERYPLDETAIL_BUTTONDELETE:
			newevt.SetId(ID_QUERYPLDETAIL_BUTTONDELETE);
			OnButtonDelete(newevt);
			break;
		case ID_QUERYPLDETAIL_BUTTONCLEAR:
			newevt.SetId(ID_QUERYPLDETAIL_BUTTONCLEAR);
			OnButtonClear(newevt);
			break;
		case ID_MENU_EXPORT://导出列表
			{
                wxString cap=((FrameBase*)GETTOPWINDOW())->GetModuleCaption(GetId());
				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
			SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_ORDER_PLDETAIL);
			break;
		default:
			break;
		}

        LeaveCriticalSection(&m_CS_EvtCallback);
    }
    evt.Skip();
}

void CQueryOrderPLDetailDialog::OnRequery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPLDETAIL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPLDetails(bIsShowDelete);
}

void CQueryOrderPLDetailDialog::OnPLDetailStatusChanged(wxCommandEvent& evt)
{
	//wxCommandEvent evt2(wxEVT_COMMAND_BUTTON_CLICKED);
	//OnButtonQuery(evt2);
}

void CQueryOrderPLDetailDialog::OnLanguageChange(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPLDETAIL_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPLDetails(bIsShowDelete);

	TRANSFORM_COLUMN_TEXT(GID_QUERY_ORDER_PLDETAIL,m_pwxExtListCtrl);
	FindWindowById(ID_QUERYPLDETAIL_CHECKSHOWDELETE,this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	m_pButtonQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	m_pButtonEdit->SetLabel(LOADSTRING(BTN_QP_BUTTONEDIT));
	m_pButtonDelete->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	m_pButtonClear->SetLabel(LOADSTRING(BTN_QP_BUTTONCLEAR));
}

void CQueryOrderPLDetailDialog::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_PLDETAIL ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_PLDETAIL, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}

void CQueryOrderPLDetailDialog::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_PLDETAIL ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_PLDETAIL, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}

void CQueryOrderPLDetailDialog::OnClose(wxCloseEvent& evt)
{
	if(m_poOwner!=NULL)
	{
		wxCommandEvent newEvt(wxEVT_PLDETAILDLG_PRECLOSE);
		newEvt.SetInt(m_nPLNum);
		m_poOwner->AddPendingEvent(newEvt);
	}
}
