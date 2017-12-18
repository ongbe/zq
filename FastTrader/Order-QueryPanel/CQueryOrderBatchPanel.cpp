#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderBatchPanel.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "wx/file.h"
#include "../inc/Module-Misc/zqControl.h"
//#include "../inc/gui/MainFrame.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


CRITICAL_SECTION CQueryOrderBatchPanel::m_CS_EvtCallback;


BEGIN_EVENT_TABLE(CQueryOrderBatchPanel, wxPanel)
	EVT_SIZE(OnSize )
	EVT_BUTTON(ID_QUERYBATCH_BUTTONQUERY, OnButtonQuery)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_BATCH_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderBatchPanel::CQueryOrderBatchPanel(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name):
	wxPanel(parent,winid, pos,size, style, name),
	m_poLocalOrderService(NULL),
	m_iColIndex(0)
{
    
    InitializeCriticalSection(&m_CS);
    m_BaseRowID=9999;      //从9999开始
    m_MapOrderKey_RowID.clear();
    m_MapRowID_OrderKey.clear();    //RowID到OrderKey的对应Map
    m_MapOrderKey_OrderInfo.clear();    //OrderKey到OrderInfo的对应Map
    m_Set_Instruments_HaveRate.clear();
		
	SetBackgroundColour(DEFAULT_COLOUR);

	m_pButtonQuery = new wxButton(this, ID_QUERYBATCH_BUTTONQUERY,LOADSTRING(BTN_QP_BUTTONQUERY), 
			wxPoint(5, 5), wxSize(55, 25), 0, wxDefaultValidator);
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY, //GID_QUERY_ORDER_BATCH, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	Init();

	m_poLocalOrderService = CLocalOrderService::GetInstance();
    InitializeCriticalSection(&m_CS_EvtCallback);

	// 取全部委托单
	//ShowOrders(ALLORDER);
    DoInitQryStep();
}

CQueryOrderBatchPanel::~CQueryOrderBatchPanel(void)
{
    DeleteCriticalSection(&m_CS_EvtCallback);
    DeleteCriticalSection(&m_CS);
}

bool CQueryOrderBatchPanel::Init()
{
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);

	m_iRowCnt=0;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_BATCH);//
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

	nColCount = pCfgMgr->GetListVisibleColCount(GID_QUERY_ORDER_BATCH);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0; i < nColCount; i++) {

		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_BATCH, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<ORDERBATCH_MIN || pColCfg->id>ORDERBATCH_MAX)
			continue;

		//"合约" "报单编号"栏，左对齐
		if((pColCfg->id == ORDERBATCH_InstrumentID) || (pColCfg->id == ORDERBATCH_RequestID))
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余的，右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        if(pColCfg->Visibility == 0)
        col.SetWidth(0);
        else
        col.SetWidth(pColCfg->Width);///* /
        if(pColCfg->Visibility != 0) {		
            col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_BATCH,pColCfg->id));
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

void CQueryOrderBatchPanel::ShowOrderBatchs()
{

	m_pwxExtListCtrl->DeleteAllItems();

	EnterCriticalSection(&m_CS);
	m_BaseRowID=9999;      
	m_MapOrderKey_RowID.clear();
	m_MapRowID_OrderKey.clear();
	m_MapOrderKey_OrderInfo.clear();
	LeaveCriticalSection(&m_CS);

	if(m_poLocalOrderService==NULL) return;

	m_poLocalOrderService->LockObject();

	CLocalOrderService::MAPBATCHSUBORDER mapBatchSub = m_poLocalOrderService->GetOrderBatchSubMap();

	m_poLocalOrderService->UnlockObject();

	CLocalOrderService::MAPBATCHSUBORDERITEM it = mapBatchSub.begin();

	this->SetCursor(*wxHOURGLASS_CURSOR);
	m_pwxExtListCtrl->Freeze();

	for(; it!=mapBatchSub.end(); ++it)	{
		LoadOneOrderList(it->second);
	}

	m_pwxExtListCtrl->Thaw();
	this->SetCursor(*wxSTANDARD_CURSOR);

}

bool CQueryOrderBatchPanel::GetOrderInfoByRowNo(int RowNo, DWORD& RowID, CLocalOrderService::BATCHSUBORDER& tBatchItem)
{
	bool bHave=false;
	std::map<DWORD,int>::iterator it_RowID_Orderkey;
	std::map<int,CLocalOrderService::BATCHSUBORDER>::iterator it_OrderKey_OrderInfo;

	RowID=m_pwxExtListCtrl->GetItemData(RowNo);

	EnterCriticalSection(&m_CS);
	it_RowID_Orderkey=m_MapRowID_OrderKey.find(RowID);
	if(it_RowID_Orderkey!=m_MapRowID_OrderKey.end())
	{
		it_OrderKey_OrderInfo=m_MapOrderKey_OrderInfo.find(it_RowID_Orderkey->second);
		if(it_OrderKey_OrderInfo!=m_MapOrderKey_OrderInfo.end())
		{
			tBatchItem=it_OrderKey_OrderInfo->second;
			bHave=true;
		}
	}
	LeaveCriticalSection(&m_CS);
	return bHave;
}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderBatchPanel::LoadOneOrderList(const CLocalOrderService::BATCHSUBORDER& tBatchOrder)
{
	if(m_pwxExtListCtrl->GetColumnCount()==0)
		return -1;

	int iCol=-1;
	int iRowNo = -1;
	wxExtListItem item;
	wxString newText;

	bool bHave=false;
	DWORD RowID;
	std::map<int, DWORD>::iterator it_Orderkey_RowID;
	EnterCriticalSection(&m_CS);
	it_Orderkey_RowID=m_MapOrderKey_RowID.find(tBatchOrder.nNum);
	if(it_Orderkey_RowID!=m_MapOrderKey_RowID.end()) {
		iRowNo=m_pwxExtListCtrl->FindItem(0,it_Orderkey_RowID->second);
	}
	LeaveCriticalSection(&m_CS);

	if(iRowNo<0) {	//一条新记录
		//下面处理将报单信息放到map中，并生成RowID
		EnterCriticalSection(&m_CS);
		RowID=m_BaseRowID++;
		m_MapOrderKey_RowID.insert(std::pair<int,DWORD>(tBatchOrder.nNum,RowID));
		m_MapRowID_OrderKey.insert(std::pair<DWORD,int>(RowID,tBatchOrder.nNum));
		m_MapOrderKey_OrderInfo.insert(std::pair<int,CLocalOrderService::BATCHSUBORDER>(tBatchOrder.nNum,tBatchOrder));
		LeaveCriticalSection(&m_CS);

		//增加新的一行，此时为空记录
		iRowNo = m_pwxExtListCtrl->InsertItem(m_pwxExtListCtrl->GetItemCount(), "");
		m_pwxExtListCtrl->SetItemData(iRowNo, (long)(RowID));
	}
	else {
		EnterCriticalSection(&m_CS);
		m_MapOrderKey_OrderInfo[tBatchOrder.nNum] = tBatchOrder;
		LeaveCriticalSection(&m_CS);
	}
	item.SetId(iRowNo);												//设置行号

	wxString strOldText;

	std::map<int, long> ::iterator it;
	it = m_FieldID2ColumnID.find(ORDERBATCH_GroupID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%d", tBatchOrder.nGroupNum);
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_SubID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		newText.Printf("%d", tBatchOrder.nSubNum);
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_RequestID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tBatchOrder.nRegNum);
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_RequestState);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		// 请求状态
		switch(tBatchOrder.nRegState) {	
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
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_InstrumentID);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tBatchOrder.req.InstrumentID);
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_Direction);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tBatchOrder.req.Direction==THOST_FTDC_D_Buy ?  LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_OCMode);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tBatchOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN) :
					(tBatchOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)));
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_Hedge);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%s", tBatchOrder.req.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_HEDGE));
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_RegVolume);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;	
		newText.Printf("%d", tBatchOrder.req.VolumeTotalOriginal);
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	it = m_FieldID2ColumnID.find(ORDERBATCH_RegPrice);
	if(it!=m_FieldID2ColumnID.end()) {
		iCol=it->second;
		if(tBatchOrder.req.OrderPriceType==THOST_FTDC_OPT_AnyPrice)
			newText = LOADSTRING(OLS_MARKET);
		else
			newText = Double2String(tBatchOrder.req.LimitPrice).c_str();
		strOldText = m_pwxExtListCtrl->GetSubItemText(iRowNo, iCol);
		if(strOldText!=newText) {
			item.SetText(newText);
			item.SetColumn( iCol );
			m_pwxExtListCtrl->SetItem(item);
		}
	}

	return 0;
}

void CQueryOrderBatchPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
	//FindWindowById(GID_QUERY_ORDER_BATCH,this)->SetPosition(wxPoint(0,35));
	wxWindow* pWin = NULL;
	for(int i=ID_QUERYBATCH_BUTTONQUERY; i<ID_QUERYBATCH_NUM; i++)
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
void CQueryOrderBatchPanel::OnButtonQuery(wxCommandEvent& evt)
{
	ShowOrderBatchs();
}

//配置文件发生变化
void CQueryOrderBatchPanel::OnCfgChanged(wxCommandEvent& evt)
{
	Init();
	ShowOrderBatchs();
}

void CQueryOrderBatchPanel::OnInitSubscribe(wxCommandEvent& evt)
{
	//ShowOrderBatchs();
}

void CQueryOrderBatchPanel::DoInitQryStep()
{
    ShowOrderBatchs();
}

void CQueryOrderBatchPanel::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	//BOOL bIsRowCLick=evt.GetInt();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏
	menu.Append(ID_QUERYBATCH_BUTTONQUERY,LOADSTRING(CONTEXTMENU_QP_BUTTONQUERY));
	menu.AppendSeparator();
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
		if(retcmd - ID_MENU_ADDTOMYSELF >= 0) {
			bool bAdded = false;
			int nGroupNum = 0, nSubNum = 0;
			CLocalOrderService::BATCHSUBORDER tOrder;
			for(int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++) {
				if(m_pwxExtListCtrl->IsSelected(i)) {
					nSubNum = m_pwxExtListCtrl->GetItemData(i);
					//nGroupNum = nSubNum/10000;
					//nSubNum = nSubNum%10000;

					DWORD RowID=0;
					bool bHave=GetOrderInfoByRowNo(i,RowID,tOrder);
					if(!bHave) {
						continue;
					}
					
					string InstrumentId = tOrder.req.InstrumentID;
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
		    case ID_QUERYBATCH_BUTTONQUERY://市价反手
			    newevt.SetId(ID_QUERYBATCH_BUTTONQUERY);
			    OnButtonQuery(newevt);
			    break;
		    case ID_MENU_EXPORT://导出列表
			{
	            CfgMgr * pMgr = CFG_MGR_DEFAULT();  
                wxString cap = pMgr!=NULL ? (wxString)pMgr->GetPanelCaption(GetId()) : wxString("");
			    LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
                break;
			}
		    case ID_MENU_COLUMNCONFIG://表格列设置
                SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_QUERYORDERBATCH);
			    break;
		    default:
			    break;
		}

        LeaveCriticalSection(&m_CS_EvtCallback);
    }
    evt.Skip();
}

void CQueryOrderBatchPanel::OnRequery(wxCommandEvent& evt)
{
	//ShowOrderBatchs();

	if(m_poLocalOrderService==NULL) return;
	
	m_poLocalOrderService->LockObject();
	
	CLocalOrderService::MAPBATCHSUBORDER mapBatchSub = m_poLocalOrderService->GetOrderBatchSubMap();

	m_poLocalOrderService->UnlockObject();

	CLocalOrderService::MAPBATCHSUBORDERITEM it = mapBatchSub.find((int)evt.GetExtraLong());
	if(it != mapBatchSub.end()) {
	
		this->SetCursor(*wxHOURGLASS_CURSOR);
		m_pwxExtListCtrl->Freeze();

		LoadOneOrderList(it->second);

		m_pwxExtListCtrl->Thaw();
		this->SetCursor(*wxSTANDARD_CURSOR);

	}
}

void CQueryOrderBatchPanel::OnLanguageChange(wxCommandEvent& evt)
{
	ShowOrderBatchs();
    TRANSFORM_COLUMN_TEXT(GID_QUERY_ORDER_BATCH,m_pwxExtListCtrl);
	m_pButtonQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
}

void CQueryOrderBatchPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_BATCH ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_BATCH, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}

void CQueryOrderBatchPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_BATCH ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_BATCH, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}
