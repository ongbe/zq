#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderConditionPanel.h"
#include "../ConfigApp/Const.h"
#include "wx/file.h"
#include "../inc/Module-Misc/zqControl.h"

#include "CEditOrderConditionDailog.h"
#include "../inc/gui/MainFrame.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;



CRITICAL_SECTION CQueryOrderConditionPanel::m_CS_EvtCallback;

static CQueryOrderConditionPanel *s_poQueryOrderConditionPanel = NULL;

int CQueryOrderConditionPanel::CallBack_OrderRsp(const GUIModuleID GID,const AbstractBusinessData &data)	  //报单回报
{ 
    if(!s_poQueryOrderConditionPanel)
        return 0;
    wxCommandEvent evt(wxEVT_ALL_ORDER_LIST_UPDATE, wxID_ANY);
    if(data.BID==BID_RtnOrder && data.Size==sizeof(DataRtnOrder)) {
		DataRtnOrder* pTmp=new DataRtnOrder();
		memcpy(pTmp, &data, sizeof(DataRtnOrder));
		evt.SetClientData(pTmp);
    }
	else if(data.BID==BID_RspOrderAction1 && data.Size==sizeof(DataRspOrderAction1)) {	//报单操作回执(错误回执1，综合交易平台认为撤单无效)
		DataRspOrderAction1* pTmp=new DataRspOrderAction1();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction1));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction2 && data.Size==sizeof(DataRspOrderAction2)) {	//报单操作回执(错误回执2，交易所认为撤单无效)
		DataRspOrderAction2* pTmp=new DataRspOrderAction2();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction2));
		evt.SetClientData(pTmp);
	}
    else {
		return 0;
	}
    s_poQueryOrderConditionPanel->AddPendingEvent(evt);
    return 0;
}

int CQueryOrderConditionPanel::CallBack_OrderInsert(const GUIModuleID GID,const AbstractBusinessData &data)	  //报单录入回报
{
    if(data.BID!=BID_RspOrderInsert || data.Size!=sizeof(DataRspOrderInsert))
        return 0;

    if(!s_poQueryOrderConditionPanel)
        return 0;

    DataRspOrderInsert* pTmp=new DataRspOrderInsert();
    memcpy(pTmp, &data, sizeof(DataRspOrderInsert));

    //post event to CAllOrdersPanel
    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, wxID_ANY);
    evt.SetClientData(pTmp);

    s_poQueryOrderConditionPanel->AddPendingEvent(evt);
    return 0;
}


BEGIN_EVENT_TABLE(CQueryOrderConditionPanel, wxPanel)
	EVT_SIZE(OnSize )
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, OnSelected)
	EVT_CHECKBOX(ID_QUERYCONDITION_CHECKSHOWDELETE, OnButtonQuery)
	EVT_CHECKBOX(ID_QUERYCONDITION_CHECKSHOWPOSITION, OnButtonQuery)
	EVT_CHECKBOX(ID_QUERYCONDITION_CHECKSHOWCONFIRM, OnButtonQuery)
	//EVT_RADIOBUTTON(ID_QUERYCONDITION_RADIOSHOWALL, OnButtonQuery)
	//EVT_RADIOBUTTON(ID_QUERYCONDITION_RADIOSHOWCONFIRM, OnButtonQuery)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONQUERY, OnButtonQuery)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONEDIT, OnButtonEdit)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONDELETE, OnButtonDelete)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONPAUSE, OnButtonPause)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONACTIVE, OnButtonActive)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONDONOW, OnButtonDoNow)
	EVT_BUTTON(ID_QUERYCONDITION_BUTTONCLEAR, OnButtonClear)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_ALL_ORDER_LIST_UPDATE, OnRcvOrderRsp)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, OnRcvOrderInsert)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_CONDITION_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALCONDITION_STATUSCHANGED, OnStatusChanged)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderConditionPanel::CQueryOrderConditionPanel(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name):
	wxPanel(parent,winid, pos,size, style, name),
	m_poOrderServiceThread(NULL),
	m_pwxExtListCtrl(NULL),
	m_iColIndex(0)
{
    
	SetBackgroundColour(DEFAULT_COLOUR);

	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_QUERYCONDITION_CHECKSHOWDELETE, 
			LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(5, 3), wxSize(14*8, 14));//, 0 );
	wxCheckBox* itemCheckBox2 = new wxCheckBox( this, ID_QUERYCONDITION_CHECKSHOWPOSITION, 
			LOADSTRING(BTN_QP_HIDESTEOPCONDITION), wxPoint(5, 18),  wxSize(14*8, 14));//, 0 );//18
	wxCheckBox* itemCheckBox3 = new wxCheckBox( this, ID_QUERYCONDITION_CHECKSHOWCONFIRM, 
			LOADSTRING(OIP_SENDED), wxPoint(5+120, 3), wxDefaultSize);//, 0 );//18

	//wxRadioButton* itemRadioBox1 = new wxRadioButton(this, 
	//		ID_QUERYCONDITION_RADIOSHOWALL,LOADSTRING(BTN_QP_ALL), wxPoint(5+120, 3), wxDefaultSize, wxRB_GROUP);
	//itemRadioBox1->SetValue(true);
	//wxRadioButton* itemRadioBox2 = new wxRadioButton(this, 
	//		ID_QUERYCONDITION_RADIOSHOWCONFIRM, LOADSTRING(OIP_SENDED), wxPoint(5+120, 18), wxDefaultSize);//, wxRB_GROUP);//60

	m_pButtonQuery = new wxButton(this, ID_QUERYCONDITION_BUTTONQUERY, LOADSTRING(BTN_QP_BUTTONQUERY), 
			wxPoint(180, 5), wxSize(53, 25), 0, wxDefaultValidator);//240
	m_pButtonEdit = new wxButton(this, ID_QUERYCONDITION_BUTTONEDIT, LOADSTRING(BTN_QP_BUTTONEDIT), 
			wxPoint(180+55, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonDelete = new wxButton(this, ID_QUERYCONDITION_BUTTONDELETE, LOADSTRING(BTN_QP_BUTTONDELETE), 
			wxPoint(180+110, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonPause = new wxButton(this, ID_QUERYCONDITION_BUTTONPAUSE, LOADSTRING(BTN_QP_BUTTONPAUSE), 
			wxPoint(180+165, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonActive = new wxButton(this, ID_QUERYCONDITION_BUTTONACTIVE, LOADSTRING(BTN_QP_BUTTONACTIVE), 
			wxPoint(180+220, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonDoNow = new wxButton(this, ID_QUERYCONDITION_BUTTONDONOW, LOADSTRING(BTN_QP_BUTTONDONOW), 
			wxPoint(180+275, 5), wxSize(80, 25), 0, wxDefaultValidator);
	m_pButtonClear = new wxButton(this, ID_QUERYCONDITION_BUTTONCLEAR, LOADSTRING(BTN_QP_BUTTONCLEAR), 
			wxPoint(180+360, 5), wxSize(80, 25), 0, wxDefaultValidator);
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY, //GID_QUERY_ORDER_CONDITION, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	Init();

	m_poOrderServiceThread = COrderServiceThread::GetInstance();
	m_poOrderServiceThread->RegisterMsgWindow(this);
    InitializeCriticalSection(&m_CS_EvtCallback);

	s_poQueryOrderConditionPanel = this;
    DoInitQryStep();
}

CQueryOrderConditionPanel::~CQueryOrderConditionPanel(void)
{
	m_poOrderServiceThread->UnregisterMsgWindow(this);

    DeleteCriticalSection(&m_CS_EvtCallback);
}

BOOL CQueryOrderConditionPanel::DoRemoveCondOrder(PlatformStru_OrderInfo& tCondOrder)
{
	if(tCondOrder.OrderStatus != THOST_FTDC_OST_NotTouched) 
		return FALSE;

	PlatformStru_InputOrderAction ReqData;
	memset(&ReqData, 0, sizeof(ReqData));
    ReqData.Thost.FrontID=tCondOrder.FrontID;
	ReqData.Thost.SessionID=tCondOrder.SessionID;
	memcpy(ReqData.Thost.OrderRef, tCondOrder.OrderRef, sizeof(tCondOrder.OrderRef));
	memcpy(ReqData.Thost.OrderSysID, tCondOrder.OrderSysID, sizeof(tCondOrder.OrderSysID));
	memcpy(ReqData.Thost.ExchangeID, tCondOrder.ExchangeID, sizeof(tCondOrder.ExchangeID));
	memcpy(ReqData.Thost.InstrumentID, tCondOrder.InstrumentID, sizeof(tCondOrder.InstrumentID));

	//// 把要撤单的保存到Map中
	//string strKey;
	//strKey = ReqData.Thost.ExchangeID;
	//strKey += ",";
	//strKey += ReqData.Thost.OrderSysID;
	//m_mapCancelWait[strKey] = GetTickCount();

    int ret=DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderAction(ReqData):-1;

	if(ret==0) {
		// m_bWaitingRsp_OrderDelete=true;
	}
	else {                       
		//设置错误信息

	}
	return !ret;
}

BOOL CQueryOrderConditionPanel::NeedConfirm()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderConfirm;
}

bool CQueryOrderConditionPanel::Init()
{
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);

	m_iRowCnt=0;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_CONDITION);//
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

	nColCount = pCfgMgr->GetListColCount(GID_QUERY_ORDER_CONDITION);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0;  i < nColCount; i++) {
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_CONDITION, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<ORDERCONDITION_MIN || pColCfg->id>ORDERCONDITION_MAX)
			continue;

		//"合约" "报单编号"栏，左对齐
		if( (pColCfg->id == ORDERCONDITION_InstrumentID) || (pColCfg->id == ORDERCONDITION_EntrustID) || (pColCfg->id == ORDERCONDITION_DoCondition))
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余的，右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        if(pColCfg->Visibility == 0)
        col.SetWidth(0);
        else
        col.SetWidth(pColCfg->Width);///* /
        if(pColCfg->Visibility != 0) {		
            col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_CONDITION,pColCfg->id));
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

void CQueryOrderConditionPanel::ShowOrders(int nShowMode, BOOL bIsShowDelete, BOOL bIsShowPosition)
{
	
	int nRowNo = -1, iCol = 0, iSumCol = 0;

	//删除当前的全部记录
	for(std::map<string, int>::iterator it=m_OrderKey2Row.begin(); it!=m_OrderKey2Row.end(); ++it) {
		LPVOID pVoid = (LPVOID)(m_pwxExtListCtrl->GetItemData(it->second));
		SAFE_DELETE(pVoid)
	}
    m_pwxExtListCtrl->DeleteAllItems();
	m_OrderRow2Key.clear();
    m_OrderKey2Row.clear();
    i_CurOrderRows=-1;

	ShowLocalConditions(nRowNo, nShowMode, bIsShowDelete, bIsShowPosition);
	ShowServerConditions(nRowNo, nShowMode, bIsShowDelete);

	i_CurOrderRows = nRowNo;

}

void CQueryOrderConditionPanel::ShowLocalConditions(int& nRowID, int nShowMode, BOOL bIsShowDelete, BOOL bIsShowPosition)
{

	if(m_poOrderServiceThread==NULL) return;
	
	m_poOrderServiceThread->LockObject();
	COrderServiceThread::MAPCONDITIONORDER mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
	m_poOrderServiceThread->UnlockObject();
	COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.begin();

	wxString strKey;
	string strkeytmp;
	for(; it!=mapCondition.end(); ++it)	{
		COrderServiceThread::CONDITIONORDER& tConditionOrder = it->second;

		if(tConditionOrder.nNum==0) continue;
		//if(nShowMode==1 && tConditionOrder.nState<=conCondDelete) continue;
		if(!bIsShowPosition && tConditionOrder.nCondPosRelation==conCPRCondFromPos) continue;

		++nRowID;
		if(ShowOneOrder(nRowID, tConditionOrder) < 0) {
			--nRowID;
		}
		else {
			strKey.Printf("%d,%d", conTypeCondLocal, tConditionOrder.nNum);
			strkeytmp = strKey;
			m_OrderKey2Row[strkeytmp] = nRowID;
			m_OrderRow2Key[nRowID] = strkeytmp;
		}
	}

	COrderServiceThread::MAPCONDITIONORDER& mapCondition_Overdue = 
			m_poOrderServiceThread->GetOrderConditionOverdueMap();
	COrderServiceThread::MAPCONDITIONORDERITEM itOverdue = mapCondition_Overdue.begin();

	for(; itOverdue!=mapCondition_Overdue.end(); ++itOverdue)	{
		COrderServiceThread::CONDITIONORDER& tConditionOrder = itOverdue->second;

		if(tConditionOrder.nNum==0) continue;
		if(nShowMode==0 && tConditionOrder.nState>conCondDelete) continue;
		if(!bIsShowDelete && tConditionOrder.nState==conCondDelete) continue;
		if(!bIsShowPosition && tConditionOrder.nCondPosRelation==conCPRCondFromPos) continue;

		++nRowID;
		if(ShowOneOrder(nRowID, tConditionOrder, TRUE, TRUE) < 0) {
			--nRowID;
		}
		else {
			strKey.Printf("%d,%d", conTypeCondLocal, tConditionOrder.nNum);
			strkeytmp = strKey;
			m_OrderKey2Row[strkeytmp] = nRowID;
			m_OrderRow2Key[nRowID] = strkeytmp;
		}
	}

}

void CQueryOrderConditionPanel::ShowServerConditions(int& nRowID, int nShowMode, BOOL bIsShowDelete)
{
    std::vector<PlatformStru_OrderInfo> outData;
	outData.clear();
	if( DEFAULT_SVR()&&DEFAULT_SVR()->GetAllOrders(outData) > 0) 
    {
        int i,count;
        count=outData.size();
		wxString strKey;
		string strkeytmp;
        for(i=0;i<count;i++)
        {			
            PlatformStru_OrderInfo& tOneOrder = outData[i];
			if(tOneOrder.ContingentCondition > THOST_FTDC_CC_ParkedOrder) {
				if(!bIsShowDelete && tOneOrder.OrderStatus == THOST_FTDC_OST_Canceled)
					continue;
				if(nShowMode == 0 && tOneOrder.OrderStatus == THOST_FTDC_OST_Touched)
					continue;
				++nRowID;
				if(ShowOneOrder(nRowID, tOneOrder) < 0) {
					--nRowID;
					continue;
				}
				else 
                {
					strKey.Printf("%d,%s", conTypeCondServer, tOneOrder.OrderSysID);
					strkeytmp = strKey;
					m_OrderKey2Row[strkeytmp] = nRowID;
					m_OrderRow2Key[nRowID] = strkeytmp;
				}
			}
		}
	}

}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderConditionPanel::ShowOneOrder(int nRowNo, 
											const COrderServiceThread::CONDITIONORDER& tConditionOrder, 
											BOOL bIsNew, BOOL bIsDeleted)
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
		m_pwxExtListCtrl->InsertItem(nRowNo, item);
		//m_pwxExtListCtrl->SetItemData(nRowNo, (long)tConditionOrder.nNum);
		m_pwxExtListCtrl->SetItemData(nRowNo, 0);
		item.SetId(nRowNo);	//设置行号
	}
	else {
		item.SetId(nRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}

	std::map<int, long> ::iterator it;
	it = m_FieldID2ColumnID.find(ORDERCONDITION_ConditionID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf(LOADFORMATSTRING(OIP_LOCALCONDORD,"%d"), tConditionOrder.nNum);//文本必须包含%d
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_State);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		switch(tConditionOrder.nState) {
		case conParkedPause:
			newText =LOADSTRING(OIP_PAUSE);
			break;
		case conParkedActive:
			newText =LOADSTRING(OIP_ACTIVE);
			break;
		case conParkedDelete:
			newText = LOADSTRING(OIP_DELETEED);
			break;
		case conCondConfirm:
			newText = LOADSTRING(OIP_SUBMITORDER);
			break;
		case conCondCancel:
			newText =LOADSTRING(OIP_SYSCANCEL);
			break;
		case conCondCompleted:
			newText = LOADSTRING(OIP_COMPLETE);
			break;
		default:
			newText = "";
			break;
		};
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_EntrustID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tConditionOrder.nEntrustNum);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_RequestState);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		// 请求状态
		switch(tConditionOrder.nRegState) {	
			case conReqWaiting:			// 等待处理
				newText =LOADSTRING(OIP_NOTOUCH);
				break;
			case conReqSendOk:			// 发送成功
				newText =LOADSTRING(OIP_SNEDOK);
				break;
			case conReqSendFail:		// 发送失败
				newText =LOADSTRING(OIP_SNEDFAIL);
				break;
			case conReqEntrustOk:		// 委托单成功
				newText =LOADSTRING(OIP_RESPOK);
				break;
			case conReqEntrustFail:		// 委托单失败
				newText = LOADSTRING(OIP_RESPFAIL);
				break;
			case conReqDealFail:		// 未成交
				newText =LOADSTRING(OIP_NOFILL);
				break;
			case conReqDealClose:		// 已成交
				newText =LOADSTRING(OIP_FILLED);
				break;
		};
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_InstrumentID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tConditionOrder.req.InstrumentID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_Direction);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tConditionOrder.req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_OCMode);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tConditionOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN):
					(tConditionOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_Hedge);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tConditionOrder.req.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_HEDGE));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_RegVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tConditionOrder.req.VolumeTotalOriginal);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_RegPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		if(tConditionOrder.req.OrderPriceType==THOST_FTDC_OPT_AnyPrice)
			newText = LOADSTRING(OLS_MARKET);
		else
			newText = Double2String(tConditionOrder.req.LimitPrice).c_str();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	//it = m_FieldID2ColumnID.find(ORDERCONDITION_ConditionPrice);
	//if(it!=m_FieldID2ColumnID.end()) {
	//	iCol=it->second;	
	//	newText.Printf("%s", tConditionOrder.nPriceType==1 ?LOADSTRING(OIP_LASTPRICE) : 
	//				(tConditionOrder.nPriceType==2 ? LOADSTRING(OLS_BIDPRICE) : LOADSTRING(OLS_ASKPRICE)));
	//	item.SetText(newText);
	//	item.SetColumn( iCol );
	//	m_pwxExtListCtrl->SetItem(item);
	//}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_DoCondition);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		wxString strPriceType;
		strPriceType.Printf("%s ", tConditionOrder.nPriceType==1 ?LOADSTRING(OIP_LASTPRICE) : 
					(tConditionOrder.nPriceType==2 ? LOADSTRING(OLS_BIDPRICE) : LOADSTRING(OLS_ASKPRICE)));
		wxString strCondType;
		switch(tConditionOrder.nConditionType) {
		case 1:
			strCondType.Printf("%s %s", "<=", 
					Double2String(tConditionOrder.fConditionPrice).c_str());
			break;
		case 2:
			strCondType.Printf("%s %s", ">=", 
					Double2String(tConditionOrder.fConditionPrice).c_str());
			break;
		case 3:
			strCondType.Printf("%s %s", "<", 
					Double2String(tConditionOrder.fConditionPrice).c_str());
			break;
		case 4:
			strCondType.Printf("%s %s", ">", 
					Double2String(tConditionOrder.fConditionPrice).c_str());
			break;
		};
		newText = strPriceType+strCondType;
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	// 实际为预埋时间
	it = m_FieldID2ColumnID.find(ORDERCONDITION_DeleteTime);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText="";
		if(tConditionOrder.nCreateTime>0)
			newText.Printf("%s", wxDateTime((time_t)tConditionOrder.nCreateTime).FormatISOTime());
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_ErrorMsg);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tConditionOrder.strErrorMsg);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	return 0;
}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderConditionPanel::ShowOneOrder(int nRowNo, 
											const PlatformStru_OrderInfo& tOneOrder, 
											BOOL bIsNew, BOOL bIsDeleted)
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

	PlatformStru_OrderInfo* pNewOrder = new PlatformStru_OrderInfo;
	*pNewOrder = tOneOrder;

	if(bIsNew) {
		m_pwxExtListCtrl->InsertItem(nRowNo, item);
		m_pwxExtListCtrl->SetItemData(nRowNo, (long)pNewOrder);
		item.SetId(nRowNo);	//设置行号
	}
	else {
		LPVOID pVoid = (LPVOID)(m_pwxExtListCtrl->GetItemData(nRowNo));
		SAFE_DELETE(pVoid)
		m_pwxExtListCtrl->SetItemData(nRowNo, (long)pNewOrder);
		item.SetId(nRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}

	std::map<int, long> ::iterator it;

	it = m_FieldID2ColumnID.find(ORDERCONDITION_ConditionID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf(LOADFORMATSTRING(OIP_SERVERCONDORD,"%s"), tOneOrder.OrderSysID);//文本必须包含%s
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_State);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		// 请求状态
		switch(tOneOrder.OrderStatus) {	
			case THOST_FTDC_OST_NotTouched:			// 等待处理
				newText =LOADSTRING(OIP_NOTOUCH);
				break;
			case THOST_FTDC_OST_Touched:			// 发送成功
				newText =LOADSTRING(OIP_TOUCHED);
				break;
			case THOST_FTDC_OST_AllTraded:			
				newText = LOADSTRING(OIP_ALLTRADED);
				break;
			case THOST_FTDC_OST_PartTradedQueueing:		// 委托单成功
				newText = LOADSTRING(OIP_PARTTRADEDQUEUEING);
				break;
			case THOST_FTDC_OST_PartTradedNotQueueing:		// 委托单失败
				newText = LOADSTRING(OIP_PARTTRADEDNOTQUEUEING);
				break;
			case THOST_FTDC_OST_NoTradeQueueing:		// 未成交
				newText = LOADSTRING(OIP_NOTRADEQUEUEING);
				break;
			case THOST_FTDC_OST_NoTradeNotQueueing:		// 已成交
					newText =LOADSTRING(OIP_NOTRADENOTQUEUEING);
				break;
			case THOST_FTDC_OST_Canceled:		// 已成交
				newText = LOADSTRING(IDT_GENERALCANCELORDER);
				break;
		};
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}
	it = m_FieldID2ColumnID.find(ORDERCONDITION_InstrumentID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tOneOrder.InstrumentID);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_Direction);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tOneOrder.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_OCMode);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tOneOrder.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN):
					(tOneOrder.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ?LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_Hedge);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tOneOrder.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_HEDGE));
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_RegVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tOneOrder.VolumeTotalOriginal);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_RegPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		if(tOneOrder.OrderPriceType==THOST_FTDC_OPT_AnyPrice) 
			newText = LOADSTRING(OLS_MARKET);
		else
			newText = Double2String(tOneOrder.LimitPrice).c_str();
		newText.Trim();
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_ConditionPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText="";
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_DoCondition);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
//		newText.Printf("%s%0.2f", tConditionOrder.nConditionType==1?"<=":">=", tConditionOrder.fConditionPrice);
		wxString strCondDir;
		switch(tOneOrder.ContingentCondition) {
		case THOST_FTDC_CC_LastPriceGreaterThanStopPrice:	///最新价大于条件价
			strCondDir = LOADSTRING(OIP_LASTPRICE);strCondDir+=" > ";
			break;
		case THOST_FTDC_CC_LastPriceGreaterEqualStopPrice:	///最新价大于等于条件价
			strCondDir = LOADSTRING(OIP_LASTPRICE);strCondDir+=" >= ";
			break;
		case THOST_FTDC_CC_LastPriceLesserThanStopPrice:	///最新价小于条件价
			strCondDir = LOADSTRING(OIP_LASTPRICE);strCondDir+=" < ";
			break;
		case THOST_FTDC_CC_LastPriceLesserEqualStopPrice:	///最新价小于等于条件价
			strCondDir = LOADSTRING(OIP_LASTPRICE);strCondDir+=" <= ";
			break;
		case THOST_FTDC_CC_AskPriceGreaterThanStopPrice:	///卖一价大于条件价
				strCondDir = LOADSTRING(OIP_ASKPRICE);strCondDir+=" > ";
			break;
		case THOST_FTDC_CC_AskPriceGreaterEqualStopPrice:	///卖一价大于等于条件价
				strCondDir = LOADSTRING(OIP_ASKPRICE);strCondDir+=" >= ";
			break;
		case THOST_FTDC_CC_AskPriceLesserThanStopPrice:		///卖一价小于条件价
				strCondDir = LOADSTRING(OIP_ASKPRICE);strCondDir+=" < ";
			break;
		case THOST_FTDC_CC_AskPriceLesserEqualStopPrice:	///卖一价小于等于条件价
				strCondDir = LOADSTRING(OIP_ASKPRICE);strCondDir+=" <= ";
			break;
		case THOST_FTDC_CC_BidPriceGreaterThanStopPrice:	///买一价大于条件价
			strCondDir = LOADSTRING(OIP_BIDPRICE);strCondDir+=" > ";
			break;
		case THOST_FTDC_CC_BidPriceGreaterEqualStopPrice:	///买一价大于等于条件价
			strCondDir = LOADSTRING(OIP_BIDPRICE);strCondDir+=" >= ";
			break;
		case THOST_FTDC_CC_BidPriceLesserThanStopPrice:		///买一价小于条件价
			strCondDir = LOADSTRING(OIP_BIDPRICE);strCondDir+=" < ";
			break;
		case THOST_FTDC_CC_BidPriceLesserEqualStopPrice:	///买一价小于等于条件价
			strCondDir = LOADSTRING(OIP_BIDPRICE);strCondDir+=" <= ";
			break;
		};
		wxString strPrice;
		strPrice = Double2String(tOneOrder.StopPrice).c_str();
		newText=strCondDir+strPrice;
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	// 实际为预埋时间
	it = m_FieldID2ColumnID.find(ORDERCONDITION_DeleteTime);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tOneOrder.InsertTime);
		//newText = "";
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	it = m_FieldID2ColumnID.find(ORDERCONDITION_ErrorMsg);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tOneOrder.StatusMsg);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}

	return 0;
}

/*
int CAllOrdersPanel::UpdateOrderList(const CThostFtdcOrderField& rawData)
{
	return 0;
}
*/
/*
//iRowNo肯定存在
int CAllOrdersPanel::DeleteListRow( int &iRowNo)
{	
	return 0;
}
*/
/*
int CAllOrdersPanel::UpdateListRow( int &iRowNo)
{
	int iCol=-1;
	wxExtListItem item;
	wxString newText;

	item.SetId(iRowNo);

	std::map<int, long> ::iterator it;
	it = m_FieldID2ColumnID.find( ALLORDER_OrderLocalID );
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText=m_OneOrderList.OrderSysID;//左边有空格
		newText.Trim(true).Trim(false);
		item.SetText(newText);
		item.SetColumn( iCol );
		m_pwxExtListCtrl->SetItem(item);
	}
	return 0;
}
*/

void CQueryOrderConditionPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
	//FindWindowById(GID_QUERY_ORDER_CONDITION,this)->SetPosition(wxPoint(0,30));
	wxWindow* pWin = NULL;
	for(int i=ID_QUERYCONDITION_BUTTONQUERY; i<ID_QUERYCONDITION_NUM; i++)
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

void CQueryOrderConditionPanel::OnButtonQuery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
	BOOL bIsShowPosition = !itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);
}

void CQueryOrderConditionPanel::OnButtonEdit(wxCommandEvent& evt)
{

	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(SelectItemId);
	if(itr2k == m_OrderRow2Key.end()) return;

	int nType = 0, nNum = 0;
	//char strTemp[1024];
	//ZeroMemory(strTemp, 1024);
	//sscanf_s(itr2k->second.c_str(), "%d", &nType);
	//switch(nType) {
	//case conTypeCondLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeCondServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//default:
	//	break;
	//};
	string strSysID;
	vector<string> outData;
	if(SplitString(itr2k->second, outData)<2) 
		return;
	nType = atoi(outData[0].c_str());
	switch(nType) {
	case conTypeCondLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeCondServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	BOOL bRet = FALSE;
	switch(nType) {
	case conTypeCondLocal:
	{

		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
		COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.find(nNum);
		COrderServiceThread::CONDITIONORDER tCondOrder={0};
		BOOL bFound = (it != mapCondition.end());
		if(bFound)
			tCondOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(!bFound) {
			wxMessageDialog dialog( NULL,LOADSTRING(CONDITION_ERROR_MESSAGE),LOADSTRING(CONDITION_ERROR_TITLE) ,wxOK|wxICON_ERROR);
			dialog.ShowModal();
			return;
		}

		if(tCondOrder.nState >= conCondDelete) {
			wxMessageDialog dialog( NULL,LOADSTRING(CONDITION_ERROR_MESSAGE) ,LOADSTRING(CONDITION_ERROR_TITLE),wxOK|wxICON_ERROR);
			dialog.ShowModal();
			return;
		}

		if(tCondOrder.nCondPosRelation==conCPRCondFromPos) {
				wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_CANNOTDELETE) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
				dialog.ShowModal();
			return;
		}

		PlatformStru_InstrumentInfo insInfo;
		memset(&insInfo,0,sizeof(insInfo));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(tCondOrder.req.InstrumentID,insInfo)!=0) 
			insInfo.PriceTick = 1.0f;

		bool bFail=false;
		CEditOrderConditionDailog* pDlg = new CEditOrderConditionDailog(this, wxID_ANY, 
				LOADSTRING(CONDITIONCURRENT_TITLE), wxDefaultPosition, wxSize(310, 250));
		pDlg->SetPriceTick(insInfo.PriceTick);
		pDlg->SetConditionOrder(tCondOrder);
		if(pDlg->ShowModal()==wxID_OK) {
			tCondOrder = pDlg->GetConditionOrder();

			// 同步加锁，保证数据的一致性
			m_poOrderServiceThread->LockObject();
			// 判断修改期间止盈止损单是否成交
			bFound = (mapCondition.find(nNum)!=mapCondition.end());
			if(bFound) 
				m_poOrderServiceThread->OrderConditionSet(nNum, tCondOrder);
				//mapCondition[nConditionNum] = tCondOrder;
			// 同步解锁
			m_poOrderServiceThread->UnlockObject();
			if(bFound) {
				ShowNormalInfoDlg(string("CONDITION_EDIT_TITLE"), string("CONDITION_EDIT_MESSAGE"), TRUE);
			}
			else {
				bFail = true;
			}

			ShowOneOrder(SelectItemId, tCondOrder, FALSE);

			if(bFail) {
				wxMessageDialog dialog( NULL,LOADSTRING(CONDITION_ERROR_MESSAGE) ,LOADSTRING(CONDITION_ERROR_TITLE),wxOK|wxICON_ERROR);
				dialog.ShowModal();
			}

		}
		pDlg->Destroy();
	}
		break;
	};

}

wxString CQueryOrderConditionPanel::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
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

wxString CQueryOrderConditionPanel::GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl)
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

void CQueryOrderConditionPanel::OnButtonDelete(wxCommandEvent& evt)
{

	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(SelectItemId);
	if(itr2k == m_OrderRow2Key.end()) return;

	int nType = 0, nNum = 0;
	//char strTemp[1024];
	//ZeroMemory(strTemp, 1024);
	//sscanf_s(itr2k->second.c_str(), "%d", &nType);
	//switch(nType) {
	//case conTypeCondLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeCondServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//default:
	//	break;
	//};
	string strSysID;
	vector<string> outData;
	if(SplitString(itr2k->second, outData)<2) 
		return;
	nType = atoi(outData[0].c_str());
	switch(nType) {
	case conTypeCondLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeCondServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击条件单模块按钮[撤单]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	BOOL bRet = FALSE;

	wxExtListItem item;
	item.SetId(SelectItemId);
	item.SetColumn(0);
	
	switch(nType) {
	case conTypeCondLocal:
	{
		if(nNum<=0) return;

		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
		COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.find(nNum);
		COrderServiceThread::CONDITIONORDER tCondOrder={0};
		BOOL bFound = (it != mapCondition.end());
		if(bFound)
			tCondOrder = it->second;
		m_poOrderServiceThread->UnlockObject();
		if(!bFound) {
			wxMessageDialog dialog( NULL,LOADSTRING(CONDITION_ERROR_MESSAGE) ,LOADSTRING(CONDITION_ERROR_TITLE),wxOK|wxICON_ERROR);
			dialog.ShowModal();
			return;
		}

		if(tCondOrder.nCondPosRelation==conCPRCondFromPos) 
		{
			wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_CANNOTDELETE) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
			return;
		}

		m_poOrderServiceThread->LockObject();
		BOOL bRet = m_poOrderServiceThread->OrderConditionDelete(nNum);
		m_poOrderServiceThread->UnlockObject();
		if(bRet) {
			item.SetText(LOADSTRING(OIP_DELETEED));
			m_pwxExtListCtrl->SetItem(item);
		}
		else {
			wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_DELETEFIALED) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	case conTypeCondServer:
	{
		wxString strOrderSysID = strSysID;

        std::vector<PlatformStru_OrderInfo> outData;
        outData.clear();
		if( DEFAULT_SVR()&&DEFAULT_SVR()->GetAllOrders(outData) == 0) 
			break;

        int i,count;
        count=outData.size();
		wxString strKey;
		string strkeytmp;
        for(i=0;i<count;i++)
        {
            PlatformStru_OrderInfo& tOneOrder = outData[i];
			if(tOneOrder.ContingentCondition > THOST_FTDC_CC_ParkedOrder) 
            {
				if(strOrderSysID.compare(tOneOrder.OrderSysID)==0) 
                {
					DoRemoveCondOrder(tOneOrder);
					break;
				}
			}
		}

	}
		break;
	};

}

void CQueryOrderConditionPanel::OnButtonPause(wxCommandEvent& evt)
{

	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(SelectItemId);
	if(itr2k == m_OrderRow2Key.end()) return;

	int nType = 0, nNum = 0;
	//char strTemp[1024];
	//ZeroMemory(strTemp, 1024);
	//sscanf_s(itr2k->second.c_str(), "%d", &nType);
	//switch(nType) {
	//case conTypeCondLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeCondServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//default:
	//	break;
	//};
	string strSysID;
	vector<string> outData;
	if(SplitString(itr2k->second, outData)<2) 
		return;
	nType = atoi(outData[0].c_str());
	switch(nType) {
	case conTypeCondLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeCondServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击条件单模块按钮[暂停]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	BOOL bRet = FALSE;

	wxExtListItem item;
	item.SetId(SelectItemId);
	item.SetColumn(0);
	
	switch(nType) {
	case conTypeCondLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
		COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.find(nNum);
		COrderServiceThread::CONDITIONORDER tCondOrder={0};
		BOOL bFound = (it != mapCondition.end());
		if(bFound)
			tCondOrder = it->second;
		m_poOrderServiceThread->UnlockObject();
		if(bFound) {
			if(tCondOrder.nCondPosRelation==conCPRCondFromPos) {
				wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_CANNOTDELETE) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
				dialog.ShowModal();
				return;
			}
			if(tCondOrder.nState == conCondActive)
			{
				tCondOrder.nState = conCondPause;
				m_poOrderServiceThread->LockObject();
				m_poOrderServiceThread->OrderConditionSet(nNum, tCondOrder);
				m_poOrderServiceThread->UnlockObject();
				ShowOneOrder(SelectItemId, tCondOrder, FALSE);
			}
			else {
				wxMessageDialog dialog( NULL,LOADSTRING(CONDITION_ERROR_MESSAGE) ,LOADSTRING(CONDITION_ERROR_TITLE),wxOK|wxICON_ERROR);
				dialog.ShowModal();
			}
		}
		else {
			wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_PAUSEFIALED) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	};

}

void CQueryOrderConditionPanel::OnButtonActive(wxCommandEvent& evt)
{

	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
    if(SelectItemId<0) 
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
        return;
    }

	std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(SelectItemId);
	if(itr2k == m_OrderRow2Key.end()) return;

	int nType = 0, nNum = 0;
	//char strTemp[1024];
	//ZeroMemory(strTemp, 1024);
	//sscanf_s(itr2k->second.c_str(), "%d", &nType);
	//switch(nType) {
	//case conTypeCondLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeCondServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//default:
	//	break;
	//};
	string strSysID;
	vector<string> outData;
	if(SplitString(itr2k->second, outData)<2) 
		return;
	nType = atoi(outData[0].c_str());
	switch(nType) {
	case conTypeCondLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeCondServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击条件单模块按钮[激活]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	BOOL bRet = FALSE;

	wxExtListItem item;
	item.SetId(SelectItemId);
	item.SetColumn(0);
	
	switch(nType) {
	case conTypeCondLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
		COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.find(nNum);
		COrderServiceThread::CONDITIONORDER tCondOrder={0};
		BOOL bFound = (it!=mapCondition.end());
		if(bFound)
			tCondOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) {
			if(tCondOrder.nCondPosRelation==conCPRCondFromPos) 
			{
				wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_CANNOTDELETE) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
				dialog.ShowModal();
				return;
			}
			if(tCondOrder.nState == conCondPause) {
				tCondOrder.nState = conCondActive;
				m_poOrderServiceThread->LockObject();
				m_poOrderServiceThread->OrderConditionSet(nNum, tCondOrder);
				m_poOrderServiceThread->UnlockObject();
				ShowOneOrder(SelectItemId, tCondOrder, FALSE);
			}
			else {
				wxMessageDialog dialog( NULL,LOADSTRING(CONDITION_ERROR_MESSAGE) ,LOADSTRING(CONDITION_ERROR_TITLE),wxOK|wxICON_ERROR);
				dialog.ShowModal();
			}
		}
		else {
			wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_CONFIDTIONDELETED) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	};

}

void CQueryOrderConditionPanel::OnButtonDoNow(wxCommandEvent& evt)
{
	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

	std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(SelectItemId);
	if(itr2k == m_OrderRow2Key.end()) return;

	int nType = 0, nNum = 0;
	//char strTemp[1024];
	//ZeroMemory(strTemp, 1024);
	//sscanf_s(itr2k->second.c_str(), "%d", &nType);
	//switch(nType) {
	//case conTypeCondLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeCondServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//default:
	//	break;
	//};
	string strSysID;
	vector<string> outData;
	if(SplitString(itr2k->second, outData)<2) 
		return;
	nType = atoi(outData[0].c_str());
	switch(nType) {
	case conTypeCondLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeCondServer:
		strSysID = outData[1];
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
		break;
	default:
		break;
	};

	BOOL bOK = TRUE;
	if(NeedConfirm()) {
		wxString strTitle;
		switch(nType) {
		case conTypeCondLocal:
			strTitle =LOADSTRING(MSG_PRO_DONOWLOCAL);
			break;
		case conTypeCondServer:
			strTitle = LOADSTRING(MSG_PRO_DONOWSERVER);
			break;
		default:
			break;
		};
		wxMessageDialog dialog( NULL, strTitle, LOADSTRING(OIP_ORDERCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
	}
	if(!bOK) return;

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击条件单模块按钮[立即执行]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	BOOL bRet = FALSE;
	switch(nType) {
	case conTypeCondLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
		COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.find(nNum);
		COrderServiceThread::CONDITIONORDER tCondOrder={0};
		BOOL bFound = (it!=mapCondition.end());
		if(bFound)
			tCondOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) {
			if(tCondOrder.nRegState != conReqWaiting) {
				wxMessageDialog dialog( NULL, LOADSTRING(MSG_PRO_ORDERFINAL),LOADSTRING(OIP_ERROR), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
				bOK = dialog.ShowModal()==wxID_YES;
				return;

			}
// 所有的未执行的激活的条件都可以立即执行，包括盈损单的条件单
//			if(it->second.nCondPosRelation==conCPRCondFromPos) {
//				wxString strMsg;
//				strMsg.Printf(_("这是属于止盈止损单的条件单，不能直接修改。\n请到止盈止损单板块中修改。"));
//				wxMessageDialog dialog( NULL,strMsg ,_("提示"),wxOK|wxICON_ERROR);
//				dialog.ShowModal();
//				return;
//			}
			m_poOrderServiceThread->LockObject();
			bRet = m_poOrderServiceThread->OrderConditionDoNow(nNum);
			m_poOrderServiceThread->UnlockObject();
			//it = mapCondition.find(nNum);
			//ShowOneOrder(SelectItemId, it->second, FALSE);
			{
			wxCheckBox* itemCheck = NULL;
			itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
			BOOL bIsShowDelete = itemCheck->GetValue();
			itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
			BOOL bIsShowPosition = !itemCheck->GetValue();
			ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);
			}
		}

		if(!bRet) {
			wxMessageDialog dialog( NULL,LOADSTRING(MSG_PRO_CANNOTDONOW) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	case conTypeCondServer:
		break;
	default:
		break;
	};
/*
	COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
	COrderServiceThread::MAPCONDITIONORDERITEM it = mapCondition.find(nConditionNum);

	if(it!=mapCondition.end()) {
// 所有的未执行的激活的条件都可以立即执行，包括盈损单的条件单
//		if(it->second.nCondPosRelation==conCPRCondFromPos) {
//			wxString strMsg;
//			strMsg.Printf(_("这是属于止盈止损单的条件单，不能直接修改。\n请到止盈止损单板块中修改。"));
//			wxMessageDialog dialog( NULL,strMsg ,_("提示"),wxOK|wxICON_ERROR);
//			dialog.ShowModal();
//			return;
//		}
		bRet = m_poOrderServiceThread->OrderConditionDoNow(nConditionNum);
		//ShowOneOrder(SelectItemId, tCondOrder, FALSE);
	}
	m_poOrderServiceThread->UnlockObject();

	if(!bRet) {
		wxString strMsg;
		strMsg.Printf(_("条件单未找到或非激活状态，无法立即执行。"));
		wxMessageDialog dialog( NULL,strMsg ,_("提示"),wxOK|wxICON_ERROR);
		dialog.ShowModal();
	}*/
}

void CQueryOrderConditionPanel::OnButtonClear(wxCommandEvent& evt)
{

	if(m_poOrderServiceThread==NULL) return;
	
	COrderServiceThread::MAPCONDITIONORDER& mapCondition = m_poOrderServiceThread->GetOrderConditionMap();
	COrderServiceThread::MAPCONDITIONORDER& mapCondition_Overdue = m_poOrderServiceThread->GetOrderConditionOverdueMap();
	
	std::vector<PlatformStru_OrderInfo> outData;
    outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetAllOrders(outData);

	int nCount = 0;

	nCount += mapCondition.size();
	//nCount += mapCondition_Overdue.size();
	int nServCondiCount = 0;
	std::map<OrderKey, PlatformStru_OrderInfo>::iterator itsearch;
    int i,count;
    count=outData.size();
    for(i=0;i<count;i++)
    {
		if(outData[i].ContingentCondition > THOST_FTDC_CC_ParkedOrder) 
			nServCondiCount++;
	}
	nCount += nServCondiCount;
	//nCount += outData.size();
	if(nCount == 0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }
	wxMessageDialog dialog( NULL, LOADSTRING(MSG_PRO_CLEARCONDITION),LOADSTRING(OIP_FASTTRADER), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES) return;

	USERLOG_INFO("用户单击条件单模块按钮[全部撤单]并选择[是]确认\n\"\n%s\n\"", 
			GetListCtrlAllRowLog(m_pwxExtListCtrl).c_str());

	// 删除所有本地条件单
	{
		COrderServiceThread::MAPCONDITIONORDERITEM it;
		std::vector<int> arrDelete;
		std::vector<int>::iterator itDel;

		m_poOrderServiceThread->LockObject();
		
		it = mapCondition.begin();
		while(it!=mapCondition.end()) {
			if(it->second.nCondPosRelation==conCPRNon && it->second.nState<conCondDelete) {
				arrDelete.push_back(it->first);
			}
			it++;
		}

		for(int i=0; i<(int)arrDelete.size(); i++) {
			m_poOrderServiceThread->OrderConditionDelete(arrDelete[i]);
		}
		arrDelete.clear();

		//it = mapCondition_Overdue.begin();
		//while(it!=mapCondition_Overdue.end()) {
		//	if(it->second.nCondPosRelation==conCPRNon) {
		//		it->second.nState = conCondDelete;
		//		mapCondition_Overdue[it->first] = it->second;
		//	}
		//	it++;
		//}

		m_poOrderServiceThread->UnlockObject();
	}

	// 删除所有服务器条件单
	{
        count=outData.size();
		if(count > 0) 
        {
            for(i=0;i<count;i++)
            {
                PlatformStru_OrderInfo& tOneOrder = outData[i];
				if(tOneOrder.ContingentCondition > THOST_FTDC_CC_ParkedOrder) 
                {
					DoRemoveCondOrder(tOneOrder);
				}
			}
		}
	}

	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
	BOOL bIsShowPosition = !itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);

	wxMessageDialog dialog2( NULL,LOADSTRING(MSG_PRO_CANNOTDELETE) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_INFORMATION);
	dialog2.ShowModal();
}

//配置文件发生变化
void CQueryOrderConditionPanel::OnCfgChanged(wxCommandEvent& evt)
{
	Init();

	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
	BOOL bIsShowPosition = !itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);

}

void CQueryOrderConditionPanel::OnInitSubscribe(wxCommandEvent& evt)
{
}

void CQueryOrderConditionPanel::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;

	int nType = -1;
	int nRow = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	std::map<int, string>::iterator it = m_OrderRow2Key.find(nRow);
	if(it != m_OrderRow2Key.end()) 
	{
		//sscanf_s(it->second.c_str(), "%d", &nType);
		vector<string> outData;
		if(SplitString(it->second, outData)>1) 
		{
			nType = atoi(outData[0].c_str());
		}
	}

	//BOOL bIsRowCLick=evt.GetInt();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏
	menu.Append(ID_QUERYCONDITION_BUTTONQUERY,LOADSTRING(CONTEXTMENU_QP_BUTTONQUERY));
	menu.AppendSeparator();
	if(evt.GetId())
	{
		if(nType==conTypeCondLocal)
			menu.Append(	ID_QUERYCONDITION_BUTTONEDIT,	LOADSTRING(CONTEXTMENU_QP_BUTTONEDIT));
		menu.Append(	ID_QUERYCONDITION_BUTTONDELETE,	LOADSTRING(CONTEXTMENU_QP_BUTTONDELETE));
		if(nType==conTypeCondLocal)
			menu.Append(	ID_QUERYCONDITION_BUTTONPAUSE,	LOADSTRING(CONTEXTMENU_QP_BUTTONPAUSE));
		if(nType==conTypeCondLocal)
			menu.Append(	ID_QUERYCONDITION_BUTTONACTIVE,	LOADSTRING(CONTEXTMENU_QP_BUTTONACTIVE));
		if(nType==conTypeCondLocal)
			menu.Append(	ID_QUERYCONDITION_BUTTONDONOW,	LOADSTRING(CONTEXTMENU_QP_BUTTONDONOW));
		menu.Append(	ID_QUERYCONDITION_BUTTONCLEAR,	LOADSTRING(CONTEXTMENU_QP_BUTTONCLEAR));
		menu.AppendSeparator();
	}
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	BOOL bIsRowCLick = evt.GetId();
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
			int nType = 0, nNum = 0;
			string InstrumentId;
            std::vector<PlatformStru_OrderInfo> outData;
			//char strTemp[1024];
			string strSysID;
			COrderServiceThread::CONDITIONORDER tOrder;
            outData.clear();
			if(DEFAULT_SVR()) 
				DEFAULT_SVR()->GetAllOrders(outData);
			for(int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++) 
            {
				if(m_pwxExtListCtrl->IsSelected(i)) {

					std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(i);
					if(itr2k == m_OrderRow2Key.end()) 
						continue;
					//ZeroMemory(strTemp, 1024);
					//sscanf_s(itr2k->second.c_str(), "%d", &nType);
					vector<string> outDataStr;
					if(SplitString(itr2k->second, outDataStr)<2)
						continue; 
					nType = atoi(outDataStr[0].c_str());
					switch(nType) {
					case conTypeCondLocal:
						{
							//sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
							m_poOrderServiceThread->LockObject();
							nNum = atoi(outDataStr[1].c_str());
							{
							InstrumentId = "";
							COrderServiceThread::MAPCONDITIONORDER& mapConditionSub = m_poOrderServiceThread->GetOrderConditionMap();
							COrderServiceThread::MAPCONDITIONORDERITEM it=mapConditionSub.find(nNum);
							if(it!=mapConditionSub.end()) {
								tOrder = it->second;
								InstrumentId = tOrder.req.InstrumentID;
							}
							}
							{
							COrderServiceThread::MAPCONDITIONORDER& mapConditionOverdue = m_poOrderServiceThread->GetOrderConditionOverdueMap();
							COrderServiceThread::MAPCONDITIONORDERITEM itOverdue=mapConditionOverdue.find(nNum);
							if(itOverdue!=mapConditionOverdue.end()) {
								tOrder = itOverdue->second;
								InstrumentId = tOrder.req.InstrumentID;
							}
							}
							m_poOrderServiceThread->UnlockObject();
						}
						break;
					case conTypeCondServer:
						{
							//sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
							strSysID = outDataStr[1];
							std::map<OrderKey, PlatformStru_OrderInfo>::iterator it;
                            int count=outData.size();
							if(count > 0) 
                            {
                                for(int i=0;i<count;i++)
                                {
									PlatformStru_OrderInfo& tOneOrder = outData[i];
									if(tOneOrder.ContingentCondition > THOST_FTDC_CC_ParkedOrder && 
										strSysID.compare(tOneOrder.OrderSysID)==0) 
                                    {
										InstrumentId = tOneOrder.InstrumentID;
									}
								}
							}
						}
						break;
					default:
						continue;
						break;
					};

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
			}
			return;
		}

        EnterCriticalSection(&m_CS_EvtCallback);

		wxCommandEvent newevt;
		switch(retcmd)
		{
		case ID_MENU_AUTO_ADJUST_COL_WIDTH:
			m_pwxExtListCtrl->AutoAdjustColumnWidth();
			SaveColWidth();
			break;
		case ID_QUERYCONDITION_BUTTONQUERY://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONQUERY);
			OnButtonQuery(newevt);
			break;
		case ID_QUERYCONDITION_BUTTONEDIT://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONEDIT);
			OnButtonEdit(newevt);
			break;
		case ID_QUERYCONDITION_BUTTONDELETE://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONDELETE);
			OnButtonDelete(newevt);
			break;
		case ID_QUERYCONDITION_BUTTONPAUSE://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONPAUSE);
			OnButtonPause(newevt);
			break;
		case ID_QUERYCONDITION_BUTTONACTIVE://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONACTIVE);
			OnButtonActive(newevt);
			break;
		case ID_QUERYCONDITION_BUTTONDONOW://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONDONOW);
			OnButtonDoNow(newevt);
			break;
		case ID_QUERYCONDITION_BUTTONCLEAR://市价反手
			newevt.SetId(ID_QUERYCONDITION_BUTTONCLEAR);
			OnButtonClear(newevt);
			break;
		case ID_MENU_EXPORT://导出列表
			{
                wxString cap=((MainFrame*)GETTOPWINDOW())->GetModuleCaption(GetId());
				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
            SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_QUERYORDERCONDITION);
			break;
		default:
			break;
		}

        LeaveCriticalSection(&m_CS_EvtCallback);
    }
    evt.Skip();
}

void CQueryOrderConditionPanel::OnSelected(wxExtListEvent& event)
{
	int nRow = event.GetIndex();
	std::map<int, string>::iterator it = m_OrderRow2Key.find(nRow);
	if(it == m_OrderRow2Key.end()) return;

	int nType = 0;
	//sscanf_s(it->second.c_str(), "%d", &nType);
	vector<string> outData;
	if(SplitString(it->second, outData)<1) 
		return;
	nType = atoi(outData[0].c_str());
	wxWindow* pWin = NULL;
	switch(nType) {
	case conTypeCondLocal:
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONEDIT);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONPAUSE);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONACTIVE);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONDONOW);
		if(pWin) pWin->Enable(true);
		break;
	case conTypeCondServer:
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONEDIT);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONPAUSE);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONACTIVE);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYCONDITION_BUTTONDONOW);
		if(pWin) pWin->Enable(false);
		break;
	default:
		break;
	};
}

void CQueryOrderConditionPanel::DoInitQryStep()
{
		wxCheckBox* itemCheck = NULL;
		itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
		BOOL bIsShowDelete = itemCheck->GetValue();
		itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
		BOOL bIsShowPosition = !itemCheck->GetValue();
		ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);

		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_QUERY_ORDER_CONDITION, CallBack_OrderRsp);	//订阅报单回报
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_QUERY_ORDER_CONDITION, CallBack_OrderInsert);	//订阅报单录入回执
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction1, GID_QUERY_ORDER_CONDITION, CallBack_OrderRsp);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction2, GID_QUERY_ORDER_CONDITION, CallBack_OrderRsp);
}

void CQueryOrderConditionPanel::OnRcvOrderRsp(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData) {
		wxASSERT(pAbstractBusinessData);
		return;
	}

	switch(pAbstractBusinessData->BID) {
    case BID_RtnOrder:
	{
		DataRtnOrder* pRtnOrder= (DataRtnOrder*)(evt.GetClientData());
		if(!pRtnOrder) {
			wxASSERT(pRtnOrder);
			return;
		}
		if(pRtnOrder->OrderField.ContingentCondition >= THOST_FTDC_CC_LastPriceGreaterThanStopPrice) { 
			wxCheckBox* itemCheck = NULL;
			itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
			BOOL bIsShowDelete = itemCheck->GetValue();
			itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
			BOOL bIsShowPosition = !itemCheck->GetValue();
			ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);
		}
	}
        break;

  //  case BID_RspOrderAction1://综合交易平台认为撤单无效           处理撤单失败的情况
  //      {
  //          DataRspOrderAction1& RawData= *(DataRspOrderAction1*)(pAbstractBusinessData);
  //          //if(RawData.InputOrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
  //            //  break;

  //          wxASSERT(RawData.RspInfoField.ErrorID);//ErrorID肯定是非零

  //          if(NeedCancelFailConfirm())
  //          {
  //              m_pTradeInfoDlg=TRADEINFODLG(this);

  //              wxString info;
  //                  m_pTradeInfoDlg->SetTitle("撤单失败");

  //              info.reserve(512);
  //              info.Printf("合约： %s\n"
  //                  "委托量： %d\n"
  //                  "委托价： %s\n",
  //                  RawData.InputOrderActionField.InstrumentID,
  //                  RawData.InputOrderActionField.VolumeChange,
  //                  ConvertToString(RawData.InputOrderActionField.LimitPrice).c_str());

  //              m_pTradeInfoDlg->SetInfo(info);
  //              m_pTradeInfoDlg->Show();
  //          }
  //      }
		//break;
  //  case BID_RspOrderAction2://交易所认为撤单无效
  //      {
  //          DataRspOrderAction2& RawData= *(DataRspOrderAction2*)(pAbstractBusinessData);

 
  //          wxASSERT(RawData.RspInfoField.ErrorID);//ErrorID肯定是非零
  //          if(NeedCancelFailConfirm())
  //          {
  //              m_pTradeInfoDlg=TRADEINFODLG(this);

  //              wxString info;
  //                  m_pTradeInfoDlg->SetTitle("撤单失败");

  //              info.reserve(512);
  //              info.Printf("合约： %s\n"
  //                  "委托量： %d\n"
  //                  "委托价： %s\n",
  //                  RawData.OrderActionField.InstrumentID,
  //                  RawData.OrderActionField.VolumeChange,
  //                  ConvertToString(RawData.OrderActionField.LimitPrice).c_str());

  //              m_pTradeInfoDlg->SetInfo(info);
  //              m_pTradeInfoDlg->Show();
  //          }
  //
  //      }
  //      break;
    default:
        break;
	}
	SAFE_DELETE(pAbstractBusinessData)
	evt.SetClientData(NULL);
}

void CQueryOrderConditionPanel::OnRcvOrderInsert(wxCommandEvent& evt)
{

 //   DataRspOrderInsert* pTmp= (DataRspOrderInsert*)(evt.GetClientData());
 //   if(!pTmp) {
 //       wxASSERT(pTmp);
 //       return;
 //   }
 //   if(pTmp->RspInfoField.ErrorID==0)
 //       return;

 //    //CThostFtdcInputOrderField& rawData= pTmp->InputOrderField;
 //    //memset(&m_OneOrderList,0,sizeof(m_OneOrderList));
 //    //memcpy(m_OneOrderList.BrokerID,rawData.BrokerID,sizeof(m_OneOrderList.BrokerID));
 //    //memcpy( m_OneOrderList.InvestorID,rawData.InvestorID,sizeof(m_OneOrderList.InvestorID));
 //    //memcpy( m_OneOrderList.InstrumentID,rawData.InstrumentID,sizeof(m_OneOrderList.InstrumentID));
 //    //memcpy( m_OneOrderList.OrderRef,rawData.OrderRef,sizeof(m_OneOrderList.OrderRef));
 //    //memcpy( m_OneOrderList.UserID,rawData.UserID,sizeof(m_OneOrderList.UserID));
 //    //m_OneOrderList.OrderPriceType=rawData.OrderPriceType;
 //    //m_OneOrderList.Direction=rawData.Direction;
 //    //memcpy( m_OneOrderList.CombOffsetFlag,rawData.CombOffsetFlag,sizeof(m_OneOrderList.CombOffsetFlag));
 //    //memcpy(  m_OneOrderList.CombHedgeFlag,rawData.CombHedgeFlag,sizeof(m_OneOrderList.CombHedgeFlag));
 //    //m_OneOrderList.LimitPrice=rawData.LimitPrice;
 //    //m_OneOrderList.VolumeTotalOriginal=rawData.VolumeTotalOriginal;
 //    //m_OneOrderList.TimeCondition=rawData.TimeCondition;
 //    //memcpy(  m_OneOrderList.GTDDate,rawData.GTDDate,sizeof(m_OneOrderList.GTDDate));
 //    //m_OneOrderList.VolumeCondition=rawData.VolumeCondition;
 //    //m_OneOrderList.MinVolume=rawData.MinVolume;
 //    //m_OneOrderList.ContingentCondition=rawData.ContingentCondition;
 //    //m_OneOrderList.StopPrice=rawData.StopPrice;
 //    //m_OneOrderList.ForceCloseReason=rawData.ForceCloseReason;
 //    //m_OneOrderList.IsAutoSuspend=rawData.IsAutoSuspend;
 //    //memcpy(  m_OneOrderList.BusinessUnit,rawData.BusinessUnit,sizeof(m_OneOrderList.BusinessUnit));
 //    //m_OneOrderList.RequestID=rawData.RequestID;
 //    //m_OneOrderList.OrderStatus=THOST_FTDC_OST_ERROE;
 //    //m_OneOrderList.VolumeTotal = rawData.VolumeTotalOriginal;
 //    //memcpy(  m_OneOrderList.StatusMsg, pTmp->RspInfoField.ErrorMsg, sizeof(m_OneOrderList.StatusMsg));
 //    //PopupCancel_InsertDlg(m_OneOrderList);
	//if(pTmp->InputOrderField.OrderStatus>THOST_FTDC_OST_Unknown)
	//	UpdateOrderList( m_OneOrderList );

 //   delete pTmp;
 //   pTmp=NULL;
 //   evt.SetClientData(NULL);
}

void CQueryOrderConditionPanel::OnRequery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
	BOOL bIsShowPosition = !itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);
}

void CQueryOrderConditionPanel::OnLanguageChange(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
	BOOL bIsShowPosition = !itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete, bIsShowPosition);
	TRANSFORM_COLUMN_TEXT(GID_QUERY_ORDER_CONDITION,m_pwxExtListCtrl);
	FindWindowById(ID_QUERYCONDITION_CHECKSHOWDELETE, this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	FindWindowById(ID_QUERYCONDITION_CHECKSHOWPOSITION,this)->SetLabel(LOADSTRING(BTN_QP_HIDESTEOPCONDITION));
	FindWindowById(ID_QUERYCONDITION_CHECKSHOWCONFIRM,this)->SetLabel(LOADSTRING(OIP_SENDED));
	//FindWindowById(ID_QUERYCONDITION_RADIOSHOWALL,this)->SetLabel(LOADSTRING(BTN_QP_ALL));
	//FindWindowById(ID_QUERYCONDITION_RADIOSHOWCONFIRM,this)->SetLabel(LOADSTRING(OIP_SENDED));
	m_pButtonQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	m_pButtonEdit->SetLabel(LOADSTRING(BTN_QP_BUTTONEDIT));
	m_pButtonDelete->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	m_pButtonPause->SetLabel(LOADSTRING(BTN_QP_BUTTONPAUSE));
    m_pButtonActive->SetLabel(LOADSTRING(BTN_QP_BUTTONACTIVE));
	m_pButtonDoNow->SetLabel(LOADSTRING(BTN_QP_BUTTONDONOW));
	m_pButtonClear->SetLabel(LOADSTRING(BTN_QP_BUTTONCLEAR));
}

void CQueryOrderConditionPanel::OnStatusChanged(wxCommandEvent& evt)
{
	wxCommandEvent evt2(wxEVT_COMMAND_BUTTON_CLICKED);
	OnButtonQuery(evt2);
}

void CQueryOrderConditionPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_CONDITION ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_CONDITION, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}

//显示一般提示信息
void CQueryOrderConditionPanel::ShowNormalInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow)
{
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();
	TRADEINFODLG(this)->ShowTradeMsg(LOADSTRING_TEXT(errormessage), -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = errormessage;
	TRADEINFODLG(this)->WriteLog( pData );
}

void CQueryOrderConditionPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_CONDITION ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_CONDITION, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}
