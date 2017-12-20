#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderPositionPanel.h"
#include "../inc/Order-Common/COrderServiceThread.h"
#include "../ConfigApp/Const.h"
#include "wx/file.h"
#include "../inc/Module-Misc/zqControl.h"

#include "CEditOrderPositionDailog.h"
#include "../inc/gui/MainFrame.h"


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
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_STOP_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPOSITION_STATUSCHANGED, OnStatusChanged)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderPositionPanel::CQueryOrderPositionPanel(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name):
	wxPanel(parent,winid, pos,size, style, name),
	m_poOrderServiceThread(NULL),
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
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY,//GID_QUERY_ORDER_POSITION, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	Init();

	m_poOrderServiceThread = COrderServiceThread::GetInstance();
	m_poOrderServiceThread->RegisterMsgWindow(this);
    InitializeCriticalSection(&m_CS_EvtCallback);

}

CQueryOrderPositionPanel::~CQueryOrderPositionPanel(void)
{
	m_poOrderServiceThread->UnregisterMsgWindow(this);

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
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_POSITION);//
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

	nColCount = pCfgMgr->GetListColCount(GID_QUERY_ORDER_POSITION);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0;  i < nColCount; i++) {
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_POSITION, i);
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
    return true;
}

void CQueryOrderPositionPanel::ShowOrderPositions(BOOL bIsShowDelete)
{
	int iRowNo = 0, iCol = 0, iSumCol = 0;

	m_pwxExtListCtrl->DeleteAllItems();
	m_OrderPositionKey2Row.clear();
	m_OrderPositionRow2Key.clear();

	i_CurOrderRows=-1;

	if(m_poOrderServiceThread==NULL) return;
	
	m_poOrderServiceThread->LockObject();
	COrderServiceThread::MAPPOSITIONORDER mapPosition = m_poOrderServiceThread->GetOrderPositionMap();
	m_poOrderServiceThread->UnlockObject();
	COrderServiceThread::MAPPOSITIONORDERITEM it = mapPosition.begin();

	iRowNo = -1;
	
	for(; it!=mapPosition.end(); ++it)	{
		COrderServiceThread::POSITIONORDER& tPositionOrder = it->second;

		if(!bIsShowDelete && tPositionOrder.nState==conPositionDelete)
			continue;

		++iRowNo;
		if(ShowOneOrder(iRowNo, tPositionOrder) < 0) {
			--iRowNo;
			continue;
		}
		else 
		{
			m_OrderPositionKey2Row[tPositionOrder.nNum] = iRowNo;
			m_OrderPositionRow2Key[iRowNo] = tPositionOrder.nNum;
		}
	}

	m_poOrderServiceThread->LockObject();
	COrderServiceThread::MAPPOSITIONORDER mapPositionOverdue = m_poOrderServiceThread->GetOrderPositionOverdueMap();
	m_poOrderServiceThread->UnlockObject();
	it = mapPositionOverdue.begin();

	for(; it!=mapPositionOverdue.end(); ++it)	{
		COrderServiceThread::POSITIONORDER& tPositionOrder = it->second;

		if(!bIsShowDelete && tPositionOrder.nState==conPositionDelete)
			continue;

		++iRowNo;
		if(ShowOneOrder(iRowNo, tPositionOrder) < 0) {
			--iRowNo;
			continue;
		}
		else 
		{
			m_OrderPositionKey2Row[tPositionOrder.nNum] = iRowNo;
			m_OrderPositionRow2Key[iRowNo] = tPositionOrder.nNum;
		}
	}

	i_CurOrderRows = iRowNo;

}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderPositionPanel::ShowOneOrder(int iRowNo, 
										   const COrderServiceThread::POSITIONORDER& tPositionOrder, 
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

	//增加新的一行，此时为空记录
	//COrderServiceThread::BATCHORDER *pRawData=new COrderServiceThread::BATCHORDER;
	//wxASSERT(pRawData);
	//ZeroMemory(pRawData, sizeof(COrderServiceThread::BATCHORDER));
	//*pRawData = tBatchOrder;

	if(bIsNew) {
		m_pwxExtListCtrl->InsertItem(iRowNo, item);
		m_pwxExtListCtrl->SetItemData(iRowNo, (long)tPositionOrder.nNum);
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
		newText.Printf("%d", tPositionOrder.nNum);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_OpenID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPositionOrder.nOpenNum);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_State);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		switch(tPositionOrder.nState) {
		case conPositionPause:
			newText = LOADSTRING(BTN_QP_BUTTONPAUSE);
			break;
		case conPositionActive:
			newText =LOADSTRING(MSG_POS_MAKECONDITIONORDER);
			break;
		case conPositionDelete:
			newText = LOADSTRING(BTN_QP_BUTTONDELETE);
			break;
		//case conPositionBaseCancel:
		//	newText = "基础单撤单";
		//	break;
		//case conPositionBaseTrade:
		//	newText = "基础单成交";
		//	break;
		case conPositionConfirm:
			newText = LOADSTRING(MSG_POS_PL_EXECUTE);
			break;
		case conPositionCancel:
			newText = LOADSTRING(MSG_POS_PL_CANCEL);
			break;
		case conPositionCompleted:
			newText = LOADSTRING(MSG_POS_PL_FINISH);
			break;
		default:
			newText = "";
			break;
		};
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_ConditionID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("zy:%d, zs:%d", tPositionOrder.nCondNum_StopGain, tPositionOrder.nCondNum_StopLose);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

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
		newText.Printf("%s", tPositionOrder.bIsBuy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_RegVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tPositionOrder.nRegVolume);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_RegPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		if(tPositionOrder.bIsMarket)
			newText = LOADSTRING(OLS_MARKET);
		else
			newText = Double2String(tPositionOrder.fRegPrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_OpenedPriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.fOpenedPriceOffset).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_StopGainPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.fStopGainPrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_StopLosePrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.fStopLosePrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_StopGainForecast);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.fStopGainForecast).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_StopLoseForecast);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.fStopLoseForecast).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_PriceType);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.nPriceType==1 ? LOADSTRING(OLS_LASTPRICE3) : LOADSTRING(OLS_B_S_PRICE3));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_CloseMode);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tPositionOrder.nCloseMode==1 ? LOADSTRING(OLS_UPLWPRICE): LOADSTRING(OLS_LIMITPRICE));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERPOSITION_ClosePriceOffset);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText = Double2String(tPositionOrder.fClosePriceOffset).c_str();
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
	for(int i=ID_QUERYPOSITION_BUTTONQUERY; i<ID_QUERYCONDITION_NUM; i++)
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
	
	//wxExtListItem item;
	//wxString strMsg, strInstrumentID;
	//item.SetId(SelectItemId);
	//item.SetColumn(0);
	//m_pwxExtListCtrl->GetItem(item);
	//wxString strPositionNum = item.GetText();
	//nPositionNum = atoi(strPositionNum.c_str());

	if(nPositionNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	m_poOrderServiceThread->LockObject();
	COrderServiceThread::MAPPOSITIONORDER& mapPosition = m_poOrderServiceThread->GetOrderPositionMap();
	COrderServiceThread::MAPPOSITIONORDERITEM it = mapPosition.find(nPositionNum);

	if(it == mapPosition.end()) {
		m_poOrderServiceThread->UnlockObject();
		wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_MESSAGE),LOADSTRING(PROFIT_ERROR_TITLE) ,wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	COrderServiceThread::POSITIONORDER tPosOrder={0};
	tPosOrder = it->second;
	m_poOrderServiceThread->UnlockObject();

	PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(tPosOrder.InstrumentID,insInfo)!=0) 
		insInfo.PriceTick = 1.0f;

	bool bFail=false;
	CEditOrderPositionDailog* pDlg = new CEditOrderPositionDailog(this, wxID_ANY, LOADSTRING(CONDITIONCURRENT_TITLE), wxDefaultPosition, wxSize(310, 250));
	pDlg->SetPriceTick(insInfo.PriceTick);
	pDlg->SetPositionOrder(tPosOrder);
	if(pDlg->ShowModal()==wxID_OK) {
		tPosOrder = pDlg->GetPositionOrder();

		// 同步加锁，保证数据的一致性
		m_poOrderServiceThread->LockObject();

		// 判断修改期间止盈止损单是否成交
		it = mapPosition.find(nPositionNum);
		if(it != mapPosition.end()) {

			PlatformStru_DepthMarketData tInstrfield;
            memset(&tInstrfield,0,sizeof(tInstrfield));
			// 从服务器更新价格
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(tPosOrder.InstrumentID, tInstrfield);

			COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
			COrderServiceThread::MAPCONDITIONORDERITEM itCond;
			
			int nCondLoseNum = tPosOrder.nCondNum_StopLose;

			// 修改止损条件单
			if(tPosOrder.bDoStopLose) {
				COrderServiceThread::CONDITIONORDER tOrderLose={0};

				itCond = mapCondition.find(nCondLoseNum);
				if(itCond != mapCondition.end()) {
					tOrderLose = itCond->second;
				}
				else {
					pDlg->ReadReq(tOrderLose.req);
					tOrderLose.req.Direction = 
							tOrderLose.req.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;
					tOrderLose.req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;
					tOrderLose.nPosNum = tPosOrder.nNum;
					tOrderLose.nCondPosRelation = conCPRCondFromPos;
					tOrderLose.bHedge = FALSE;
					tOrderLose.nState = tPosOrder.nRegState==conReqDealClose ? conCondActive : conCondPause;
				}
				tOrderLose.nPriceType = tPosOrder.nPriceType==1 ? tPosOrder.nPriceType : 
						(tOrderLose.req.Direction==THOST_FTDC_D_Buy ? 3:2);
				tOrderLose.nConditionType = (tOrderLose.req.Direction==THOST_FTDC_D_Buy ? 2:1);

				double fOffPrice = tPosOrder.nPriceOffsetType==1 ? tPosOrder.fOpenedPriceOffset : 0;
				if(tPosOrder.bIsBuy) {
					tOrderLose.fConditionPrice = 
							tPosOrder.fRegPrice + fOffPrice - tPosOrder.fStopLosePriceOffset;
					tOrderLose.nConditionType = 1;
				}
				else {
					tOrderLose.fConditionPrice = 
							tPosOrder.fRegPrice + fOffPrice + tPosOrder.fStopLosePriceOffset;
					tOrderLose.nConditionType = 2;
				}
				//tOrderLose.nPriceType = 1;

				if(tPosOrder.nCloseMode==1) 
					tOrderLose.req.LimitPrice = 
							(tOrderLose.req.Direction!=THOST_FTDC_D_Buy ? 
							tInstrfield.LowerLimitPrice : tInstrfield.UpperLimitPrice);
				else {
					tOrderLose.req.LimitPrice = tOrderLose.fConditionPrice;
					tOrderLose.req.LimitPrice += 
							(tOrderLose.req.Direction==THOST_FTDC_D_Buy ? 
							 tPosOrder.fClosePriceOffset : -tPosOrder.fClosePriceOffset);
				}
				if(itCond != mapCondition.end()) 
					m_poOrderServiceThread->OrderConditionSet(nCondLoseNum, tOrderLose);
					//mapCondition[nCondNum] = tOrderLose;
				else 
					nCondLoseNum = m_poOrderServiceThread->OrderConditionAdd(tOrderLose);
			}
			else {
				m_poOrderServiceThread->OrderConditionDelete(tPosOrder.nCondNum_StopLose);
				nCondLoseNum = 0;
			}

			int nCondGainNum = tPosOrder.nCondNum_StopGain;

			// 修改止盈条件单
			if(tPosOrder.bDoStopGain) {
				COrderServiceThread::CONDITIONORDER tOrderGain={0};

				itCond = mapCondition.find(nCondGainNum);
				if(itCond != mapCondition.end()) {
					tOrderGain = itCond->second;
				}
				else {
					pDlg->ReadReq(tOrderGain.req);
					tOrderGain.req.Direction = 
							tOrderGain.req.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;
					tOrderGain.req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;
					tOrderGain.nPosNum = tPosOrder.nNum;
					tOrderGain.nCondPosRelation = conCPRCondFromPos;
					tOrderGain.bHedge = FALSE;
					tOrderGain.nState = tPosOrder.nRegState==conReqDealClose ? conCondActive : conCondPause;
				}
				tOrderGain.nPriceType = tPosOrder.nPriceType==1 ? tPosOrder.nPriceType : 
						(tOrderGain.req.Direction==THOST_FTDC_D_Buy ? 3:2);
				tOrderGain.nConditionType = (tOrderGain.req.Direction==THOST_FTDC_D_Buy ? 1:2);

				double fOffPrice = tPosOrder.nPriceOffsetType==1 ? tPosOrder.fOpenedPriceOffset : 0;
				if(tPosOrder.bIsBuy) {
					tOrderGain.fConditionPrice = 
							tPosOrder.fRegPrice + fOffPrice + tPosOrder.fStopGainPriceOffset;
					tOrderGain.nConditionType = 2;
				}
				else {
					tOrderGain.fConditionPrice = 
							tPosOrder.fRegPrice + fOffPrice - tPosOrder.fStopGainPriceOffset;
					tOrderGain.nConditionType = 1;
				}
				//tOrderGain.nPriceType = 1;

				if(tPosOrder.nCloseMode==1) 
					tOrderGain.req.LimitPrice = 
							(tOrderGain.req.Direction!=THOST_FTDC_D_Buy ? 
                            tInstrfield.LowerLimitPrice : tInstrfield.UpperLimitPrice);
				else {
					tOrderGain.req.LimitPrice = tOrderGain.fConditionPrice;
					tOrderGain.req.LimitPrice += 
							(tOrderGain.req.Direction==THOST_FTDC_D_Buy ? 
							 tPosOrder.fClosePriceOffset : -tPosOrder.fClosePriceOffset);
				}
				if(itCond != mapCondition.end()) 
					m_poOrderServiceThread->OrderConditionSet(nCondGainNum, tOrderGain);
				else 
					nCondGainNum = m_poOrderServiceThread->OrderConditionAdd(tOrderGain);
				//mapCondition[nCondNum] = tOrderGain;

			}
			else {
				m_poOrderServiceThread->OrderConditionDelete(tPosOrder.nCondNum_StopGain);
				nCondGainNum = 0;
			}

			tPosOrder.nCondNum_StopLose = nCondLoseNum;
			tPosOrder.nCondNum_StopGain = nCondGainNum;

			m_poOrderServiceThread->OrderPositionSet(nPositionNum, tPosOrder);
			//mapPosition[nPositionNum] = tPosOrder;

		}
		else {
			bFail=true;
		}

		// 同步解锁
		m_poOrderServiceThread->UnlockObject();

		//ShowOneOrder(SelectItemId, tPosOrder, FALSE);

		if(bFail) {
			wxMessageDialog dialog( NULL,LOADSTRING(PROFIT_ERROR_TITLE) ,LOADSTRING(PROFIT_ERROR_MESSAGE),wxOK|wxICON_ERROR);
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
	//wxString strMsg,strInstrumentID;
	item.SetId(SelectItemId);
	//item.SetColumn(0);
	//m_pwxExtListCtrl->GetItem(item);
	//wxString strPositionNum = item.GetText();
	//nPositionNum = atoi(strPositionNum.c_str());

	if(m_poOrderServiceThread==NULL) return;

	USERLOG_INFO("用户单击盈损单模块按钮[撤单]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());
	
	m_poOrderServiceThread->LockObject();
	BOOL bRet = m_poOrderServiceThread->OrderPositionDelete(nPositionNum, conPositionDelete, FALSE, TRUE);
	m_poOrderServiceThread->UnlockObject();
	if(!bRet) 
	{
		wxMessageDialog dialog( NULL,LOADSTRING(MSG_POS_DELETEFIALED) ,LOADSTRING(OIP_ERROR),wxOK|wxICON_ERROR);
		dialog.ShowModal();
	}
	else {
		wxString strText;
		strText.Printf("%d (", nPositionNum);
		strText += LOADSTRING(OIP_DELETEED);
		strText += ")";
		item.SetText(strText);
		m_pwxExtListCtrl->SetItem(item);
	}
}

void CQueryOrderPositionPanel::OnButtonClear(wxCommandEvent& evt)
{

	if(m_poOrderServiceThread==NULL) return;
	
	COrderServiceThread::MAPPOSITIONORDER& mapPosition = m_poOrderServiceThread->GetOrderPositionMap();
//	COrderServiceThread::MAPPOSITIONORDER& mapPosition_Overdue = m_poOrderServiceThread->GetOrderPositionOverdueMap();
//	COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();

	int nCount = 0;

	nCount += mapPosition.size();
	//nCount += mapPosition_Overdue.size();
	if(nCount == 0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	COrderServiceThread::MAPPOSITIONORDERITEM it;
	std::vector<int> arrDelete;
	std::vector<int>::iterator itDel;


	wxMessageDialog dialog( NULL, LOADSTRING(MSG_POS_CLEARNOW), LOADSTRING(OIP_FASTTRADER), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES) return;


	m_poOrderServiceThread->LockObject();

	it = mapPosition.begin();
	while(it!=mapPosition.end()) {
		arrDelete.push_back(it->first);
		it++;
	}
	
	nCount = (int)arrDelete.size();
	for(int i=0; i<nCount; i++) {
		m_poOrderServiceThread->OrderPositionDelete(arrDelete[i], conPositionDelete, FALSE, i>=nCount-1);
	}
	arrDelete.clear();

	//it = mapPosition_Overdue.begin();
	//while(it!=mapPosition_Overdue.end()) {
	//	if(it->second.nState < conPositionDelete) {
	//		it->second.nState = conPositionDelete;
	//		//mapPosition_Overdue[it->first] = it->second;
	//	}
	//	it++;
	//}

	m_poOrderServiceThread->UnlockObject();

	//ShowOrderPositions(FALSE);

}

//配置文件发生变化
void CQueryOrderPositionPanel::OnCfgChanged(wxCommandEvent& evt)
{
	Init();
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPOSITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrderPositions(bIsShowDelete);
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
		menu.Append(ID_QUERYPOSITION_BUTTONCLEAR,LOADSTRING(CONTEXTMENU_QP_BUTTONCLEAR));
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
			COrderServiceThread::POSITIONORDER tOrder;
			for(int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++) {
				if(m_pwxExtListCtrl->IsSelected( i )) {
					item.SetId(i);
					item.SetColumn(0);
					m_pwxExtListCtrl->GetItem(item);
					strPositionNum = item.GetText();
					nPositionNum = atoi(strPositionNum.c_str());

					if(nPositionNum<=0) 
						continue;
					if(m_poOrderServiceThread==NULL) 
						continue;
					m_poOrderServiceThread->LockObject();
					{
					COrderServiceThread::MAPPOSITIONORDER& mapPosition = m_poOrderServiceThread->GetOrderPositionMap();
					COrderServiceThread::MAPPOSITIONORDERITEM it = mapPosition.find(nPositionNum);

					InstrumentId = "";
					if(it != mapPosition.end()) {
						tOrder = it->second;
						InstrumentId = tOrder.InstrumentID;
					}
					}
					{
					COrderServiceThread::MAPPOSITIONORDER& mapPositionOverdue = m_poOrderServiceThread->GetOrderPositionOverdueMap();
					COrderServiceThread::MAPPOSITIONORDERITEM itOverdue = mapPositionOverdue.find(nPositionNum);

					if(itOverdue != mapPositionOverdue.end()) {
						tOrder = itOverdue->second;
						InstrumentId = tOrder.InstrumentID;
					}
					}
					m_poOrderServiceThread->UnlockObject();

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
	                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
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
		case ID_MENU_EXPORT://导出列表
			{
                wxString cap=((MainFrame*)GETTOPWINDOW())->GetModuleCaption(GetId());
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

void CQueryOrderPositionPanel::OnStatusChanged(wxCommandEvent& evt)
{
	wxCommandEvent evt2(wxEVT_COMMAND_BUTTON_CLICKED);
	OnButtonQuery(evt2);
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
