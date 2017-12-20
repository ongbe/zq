#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryOrderParkedPanel.h"
#include "wx/file.h"
#include "wx/filename.h"

#include "../ConfigApp/Const.h"
#include "CEditOrderParkedDailog.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../Module-Misc/LogDefine.h"
#include "../inc/gui/MainFrame.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static CQueryOrderParkedPanel*	s_poQueryOrderParkedPanel = NULL;


// 还缺一个响应综合交易平台状态改变的回调函数，在这个函数里发送一个事件，
// 在事件响应中到综合交易平台查询预埋单、预埋撤单的变化情况，因为没有发现综合交易平台会返回预埋发送事件
#define TRANSLATE_MSG(dataid,datatype,msg)\
 	if(s_poQueryOrderParkedPanel&&data.BID==dataid&&data.Size==sizeof(datatype)){\
	wxCommandEvent evt(msg, wxID_ANY);\
	datatype* pTmp=new datatype();\
	memcpy(pTmp, &data, sizeof(datatype));\
	evt.SetClientData(pTmp);\
	s_poQueryOrderParkedPanel->AddPendingEvent(evt);}\
	return 0
//合约交易状态通知回调函数
int CQueryOrderParkedPanel::CallBack_InstrumentStatus(const GUIModuleID GID,const AbstractBusinessData &data)
{
	TRANSLATE_MSG(BID_RtnInstrumentStatus,DataInstrumentStatus,wxEVT_PLATFORMSVR_STATUSCHANGE);
}

int CQueryOrderParkedPanel::CallBack_ParkedOrderInsert(const GUIModuleID GID,const AbstractBusinessData &data)		//预埋单录入请求响应
{
	TRANSLATE_MSG(BID_RspParkedOrderInsert,DataRspParkedOrderInsert,wxEVT_QUERY_PARKED_ORDERINSERT); 
}

int CQueryOrderParkedPanel::CallBack_ParkedOrderAction(const GUIModuleID GID,const AbstractBusinessData &data)		//预埋撤单录入请求响应
{
	TRANSLATE_MSG(BID_RspParkedOrderAction,DataRspParkedOrderAction,wxEVT_QUERY_PARKED_ORDERACTION);
}

int CQueryOrderParkedPanel::CallBack_RemoveParkedOrder(const GUIModuleID GID,const AbstractBusinessData &data)		//删除预埋单响应
{
	TRANSLATE_MSG(BID_RspRemoveParkedOrder,DataRspRemoveParkedOrder,wxEVT_QUERY_PARKED_REMOVEPARKEDORDER); 
}

int CQueryOrderParkedPanel::CallBack_RemoveParkedOrderAction(const GUIModuleID GID,const AbstractBusinessData &data)	//删除预埋撤单响应
{
	TRANSLATE_MSG(BID_RspRemoveParkedOrderAction,DataRspRemoveParkedOrderAction,wxEVT_QUERY_PARKED_REMOVEPARKEDORDERACTION);
}

int CQueryOrderParkedPanel::CallBack_QryParkedOrder(const GUIModuleID GID,const AbstractBusinessData &data)			//请求查询预埋单响应
{
	TRANSLATE_MSG(BID_RspQryParkedOrder,DataRspQryParkedOrder,wxEVT_QUERY_PARKED_QUERYPARKED);
}

int CQueryOrderParkedPanel::CallBack_QryParkedOrderAction(const GUIModuleID GID,const AbstractBusinessData &data)		//请求请求查询预埋撤单响应
{
	TRANSLATE_MSG(BID_RspQryParkedOrderAction,DataRspQryParkedOrderAction,wxEVT_QUERY_PARKED_QUERYPARKEDACTION); 
}


CRITICAL_SECTION CQueryOrderParkedPanel::m_CS_EvtCallback;


BEGIN_EVENT_TABLE(CQueryOrderParkedPanel, wxPanel)
	EVT_SIZE(OnSize )
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, OnSelected)
	EVT_CHECKBOX(ID_QUERYPARKED_CHECKSHOWDELETE, OnButtonQuery)
	EVT_CHECKBOX(ID_QUERYPARKED_CHECKSHOWCONFIRM, OnButtonQuery)
	//EVT_RADIOBUTTON(ID_QUERYPARKED_RADIOSHOWALL, OnButtonQuery)
	//EVT_RADIOBUTTON(ID_QUERYPARKED_RADIOSHOWCONFIRM, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONQUERY, OnButtonQuery)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONEDIT, OnButtonEdit)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONDELETE, OnButtonDelete)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONPAUSE, OnButtonPause)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONACTIVE, OnButtonActive)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONDONOW, OnButtonDoNow)
	EVT_BUTTON(ID_QUERYPARKED_BUTTONCLEAR, OnButtonClear)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_ORDERINSERT, OnOrderInsert)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_ORDERACTION, OnOrderAction)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_REMOVEPARKEDORDER, OnRemoveOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_REMOVEPARKEDORDERACTION, OnRemoveOrderAction)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_QUERYPARKED, OnQueryParked)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_QUERYPARKEDACTION, OnQueryParkedAction)
	EVT_COMMAND(wxID_ANY, wxEVT_PLATFORMSVR_STATUSCHANGE, OnPlatSvrStatusChange)
	EVT_COMMAND(wxID_ANY, wxEVT_QUERY_PARKED_REQUERY, OnRequery)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
	EVT_COMMAND(wxID_ANY, wxEVT_LOCALPARKED_STATUSCHANGED, OnStatusChanged)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
END_EVENT_TABLE()

CQueryOrderParkedPanel::CQueryOrderParkedPanel(wxWindow *parent,
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

	wxCheckBox* itemCheckBox1 = new wxCheckBox( this, ID_QUERYPARKED_CHECKSHOWDELETE, 
			LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(5, 3), wxSize(14*8, 14));//, 0 );
	wxCheckBox* itemCheckBox2 = new wxCheckBox( this, ID_QUERYPARKED_CHECKSHOWCONFIRM, 
			LOADSTRING(OIP_SENDED), wxPoint(5+60, 3), wxDefaultSize);//, 0 );

	//wxRadioButton* itemRadioBox1 = new wxRadioButton(this, 
	//		ID_QUERYPARKED_RADIOSHOWALL, LOADSTRING(BTN_QP_ALL), wxPoint(5, 18), wxDefaultSize, wxRB_GROUP);
	//itemRadioBox1->SetValue(true);
	//wxRadioButton* itemRadioBox2 = new wxRadioButton(this, 
	//		ID_QUERYPARKED_RADIOSHOWCONFIRM, LOADSTRING(OIP_SENDED), wxPoint(5+60, 18), wxDefaultSize);//, wxRB_GROUP);

	m_pButtonQuery = new wxButton(this, ID_QUERYPARKED_BUTTONQUERY, LOADSTRING(BTN_QP_BUTTONQUERY), 
			wxPoint(150, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonEdit = new wxButton(this, ID_QUERYPARKED_BUTTONEDIT, LOADSTRING(BTN_QP_BUTTONEDIT), 
			wxPoint(150+55, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonDelete = new wxButton(this, ID_QUERYPARKED_BUTTONDELETE, LOADSTRING(BTN_QP_BUTTONDELETE), 
			wxPoint(150+110, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonPause = new wxButton(this, ID_QUERYPARKED_BUTTONPAUSE, LOADSTRING(BTN_QP_BUTTONPAUSE), 
			wxPoint(150+165, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonActive = new wxButton(this, ID_QUERYPARKED_BUTTONACTIVE, LOADSTRING(BTN_QP_BUTTONACTIVE), 
			wxPoint(150+220, 5), wxSize(53, 25), 0, wxDefaultValidator);
	m_pButtonDoNow = new wxButton(this, ID_QUERYPARKED_BUTTONDONOW, LOADSTRING(BTN_QP_BUTTONDONOW), 
			wxPoint(150+275, 5), wxSize(83, 25), 0, wxDefaultValidator);
	m_pButtonClear = new wxButton(this, ID_QUERYPARKED_BUTTONCLEAR,LOADSTRING(BTN_QP_BUTTONCLEAR), 
			wxPoint(150+360, 5), wxSize(83, 25), 0, wxDefaultValidator);
	m_pwxExtListCtrl = new wxExtListCtrl(this, wxID_ANY, //GID_QUERY_ORDER_CONDITION, 
			wxPoint(0, 35), wxDefaultSize, 
			wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES,wxDefaultValidator, wxEmptyString);

	Init();

	m_poOrderServiceThread = COrderServiceThread::GetInstance();
	m_poOrderServiceThread->RegisterMsgWindow(this);
    InitializeCriticalSection(&m_CS_EvtCallback);

	s_poQueryOrderParkedPanel = this;
    DoInitQryStep();
}

void CQueryOrderParkedPanel::SetColText(int nRow, int id,wxString intext)
{
	std::map<int, long> ::iterator it;
	it = m_FieldID2ColumnID.find(id);
	if(it!=m_FieldID2ColumnID.end()) 
	{
		m_pwxExtListCtrl->SetItem(nRow,it->second,intext);
	}
}

CQueryOrderParkedPanel::~CQueryOrderParkedPanel(void)
{
	m_poOrderServiceThread->UnregisterMsgWindow(this);

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnInstrumentStatus, GID_QUERY_ORDER_PARKED);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspParkedOrderInsert, GID_QUERY_ORDER_PARKED);		//退订预埋单录入请求响应
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspParkedOrderAction, GID_QUERY_ORDER_PARKED);		//退订预埋撤单录入请求响应
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspRemoveParkedOrder, GID_QUERY_ORDER_PARKED);		//退订删除预埋单响应
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspRemoveParkedOrderAction, GID_QUERY_ORDER_PARKED);	//退订删除预埋撤单响应
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryParkedOrder, GID_QUERY_ORDER_PARKED);			//退订请求查询预埋单响应
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryParkedOrderAction, GID_QUERY_ORDER_PARKED);	//退订请求查询预埋撤单响应

	SaveDataToFile();
	{
	std::map<string, DataRspParkedOrderInsert*>::iterator itInsert = m_ParkedInsert.begin();
	for(; itInsert!=m_ParkedInsert.end(); itInsert++)
	{
		if(itInsert->second!=NULL)
		{
			SAFE_DELETE(itInsert->second);
			itInsert->second = NULL;
		}
	}
	}
	{
	std::map<string, DataRspParkedOrderAction*>::iterator itAction = m_ParkedAction.begin();
	for(; itAction!=m_ParkedAction.end(); itAction++)
	{
		if(itAction->second!=NULL)
		{
			SAFE_DELETE(itAction->second);
			itAction->second = NULL;
		}
	}
	}

	DeleteCriticalSection(&m_CS_EvtCallback);
}

BOOL CQueryOrderParkedPanel::NeedConfirm()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderConfirm;
}

// 下单成功 是否 弹框
BOOL CQueryOrderParkedPanel::NeedOrderSuccessConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderSuccessDlg;
}

// 下单成功 是否 发声
BOOL CQueryOrderParkedPanel::NeedOrderSuccessSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderSuccessSound;
}

// 下单成功 是否 弹框
BOOL CQueryOrderParkedPanel::NeedOrderFailConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderFailDlg;
}

// 下单成功 是否 发声
BOOL CQueryOrderParkedPanel::NeedOrderFailSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderFailSound;
}

// 挂单成交 是否 弹框
BOOL CQueryOrderParkedPanel::NeedOrderTradeConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bTradeDlg;
}

// 挂单成交 是否 发声
BOOL CQueryOrderParkedPanel::NeedOrderTradeSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bTradeSound;
}

// 撤单成交 是否 弹框
BOOL CQueryOrderParkedPanel::NeedCancelSuccessConfirm()
{
    CANCEL_ORDER_CFG cancelOrderCfg = CFG_MGR_DEFAULT()->GetCancelOrderCfg();
    return cancelOrderCfg.bCancelSuccessDlg;
}

// 撤单失败 是否 弹框
BOOL CQueryOrderParkedPanel::NeedCancelFailConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    CANCEL_ORDER_CFG cancelOrderCfg = pMgr->GetCancelOrderCfg();
    return cancelOrderCfg.bCancelFailDlg;
}

BOOL CQueryOrderParkedPanel::LoadDataFromFile()
{
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";

	wxString strFileName;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".pks");
	wxFileName filename(strFileName); 
	if(!filename.FileExists())
		return FALSE;

	wxDateTime dtCurr;
	dtCurr = wxDateTime::Today();
	wxDateTime dtFileModify = filename.GetModificationTime();
	
	DWORD dwCurrDate = dtCurr.GetYear()*10000+dtCurr.GetMonth()*100+dtCurr.GetDay();
	DWORD dwFileModifyDate = dtFileModify.GetYear()*10000+dtFileModify.GetMonth()*100+dtFileModify.GetDay();

	wxFile file;
	DWORD dwPos=0;

	// 如果是隔夜的条件单等数据，则清空文件
	if(dwCurrDate-dwFileModifyDate) {
		file.Create(strFileName, true);
		file.Close();
		return TRUE;
	}

	file.Open(strFileName);
	if(!file.IsOpened())
		return FALSE;

	string strParkedID;
	PARKEDSHOWED tParkedShowed={0};

	while(file.Read(&tParkedShowed, sizeof(PARKEDSHOWED))==sizeof(PARKEDSHOWED)) {
		strParkedID = tParkedShowed.strParkedID;
		switch(tParkedShowed.nParkedType) {
		case conTypeParkedServer:
			m_ParkedMsgInsert[strParkedID] = tParkedShowed.bIsShowed;
			break;
		case conTypeParkedCancelServer:
			m_ParkedMsgAction[strParkedID] = tParkedShowed.bIsShowed;
			break;
		};
	}

	file.Close();

	return TRUE;
}

BOOL CQueryOrderParkedPanel::SaveDataToFile()
{
    // 创建目录
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";
    if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	wxFile file;
	DWORD dwPos=0;
	wxString strFileName;

	if(!(m_ParkedMsgInsert.size()>0 || m_ParkedMsgAction.size()>0))
		return FALSE;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".pks");
	file.Create(strFileName, true);
	if(!file.IsOpened()) {
		wxMessageDialog dialog( NULL,LOADSTRING(SaveFail_PKSFile_MESSAGE) ,LOADSTRING(SaveFail_Title),wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return FALSE;
	}

	std::map<string, BOOL>::iterator itParked;
	PARKEDSHOWED tParkedShowed={0};

	ZeroMemory(&tParkedShowed, sizeof(PARKEDSHOWED));

	for(itParked=m_ParkedMsgInsert.begin(); itParked!=m_ParkedMsgInsert.end(); itParked++) {
		tParkedShowed.nParkedType = conTypeParkedServer;
		tParkedShowed.bIsShowed = itParked->second;
		strcpy(tParkedShowed.strParkedID, itParked->first.c_str());
		file.Write(&tParkedShowed, sizeof(PARKEDSHOWED));
	}

	for(itParked=m_ParkedMsgAction.begin(); itParked!=m_ParkedMsgAction.end(); itParked++) {
		tParkedShowed.nParkedType = conTypeParkedCancelServer;
		tParkedShowed.bIsShowed = itParked->second;
		strcpy(tParkedShowed.strParkedID, itParked->first.c_str());
		file.Write(&tParkedShowed, sizeof(PARKEDSHOWED));
	}

	file.Close() ;

	return TRUE;
}

void CQueryOrderParkedPanel::PopupTradeDlg(const DataRspParkedOrderInsert& rawData, BOOL bIsQuery)
{
    CTradeInfoDlg* pInfoDlg=TRADEINFODLG(this);
	if(pInfoDlg==NULL) return;
	string title;
    wxString info;
	BOOL bShowInfo = FALSE;

	LOG_DATA* pData = new LOG_DATA;
	info.reserve(512);
	if(rawData.RspInfoField.ErrorID == 0) {
		if(!bIsQuery) {
			title = "OIP_SENDPARKORDER";
		}
		else {
			title = "OIP_CHGPARKORDER";
			//pInfoDlg->SetTitle(LOADSTRING(OIP_CHGPARKORDER), 0);
		}
		string strOffsetFlag = rawData.ParkedOrder.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? "OLS_OPEN" :
			(rawData.ParkedOrder.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? "OLS_CLOSE_TODAY": "OLS_CLOSE");
		info.Printf(LOADFORMATSTRING(TRADER_DLG_FORMAT,"%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s
				rawData.ParkedOrder.InstrumentID,
				CFieldValueTool::Direction2String(rawData.ParkedOrder.Direction,SVR_LANGUAGE).c_str(),
				LOADSTRING_TEXT(strOffsetFlag),
				Number2String(rawData.ParkedOrder.VolumeTotalOriginal).c_str(),
				Double2String(rawData.ParkedOrder.LimitPrice).c_str());
		//pInfoDlg->SetInfo(info,0);
		bShowInfo = NeedOrderSuccessConfirm(); 

		pData->strFormat = "TRADER_DLG_FORMAT";
		pData->vParam.push_back(LOG_PARAM(rawData.ParkedOrder.InstrumentID));
		pData->vParam.push_back(LOG_PARAM(rawData.ParkedOrder.Direction, DIRECTION_TYPE));
		pData->vParam.push_back(LOG_PARAM(strOffsetFlag, LOAD_STRING_TYPE));
		pData->vParam.push_back(LOG_PARAM(Number2String(rawData.ParkedOrder.VolumeTotalOriginal)));
		pData->vParam.push_back(LOG_PARAM(Double2String(rawData.ParkedOrder.LimitPrice)));
	}
	else {
		if(!bIsQuery) {
			title = "OIP_SENDPARKORDERFAIL";
		}
		else {
			title = "OIP_CHGPARKORDERFAIL";
		}
		info.Printf(LOADFORMATSTRING(TRADER_ERROR_FORMAT,"%s%s%s"),//文本必须包含%s,%s,%s
			rawData.ParkedOrder.ParkedOrderID, Number2String(rawData.RspInfoField.ErrorID).c_str(), rawData.RspInfoField.ErrorMsg);
		bShowInfo = NeedOrderFailConfirm(); 
		pData->strFormat = "TRADER_ERROR_FORMAT";
		pData->vParam.push_back(LOG_PARAM(rawData.ParkedOrder.ParkedOrderID));
		pData->vParam.push_back(LOG_PARAM(Number2String(rawData.RspInfoField.ErrorID)));
		pData->vParam.push_back(LOG_PARAM(rawData.RspInfoField.ErrorMsg));
	}
	pInfoDlg->ShowTradeMsg(info, -1, LOADSTRING_TEXT(title), -1, bShowInfo);

	pData->strTitle = title;
	pInfoDlg->WriteLog( pData );
}

void CQueryOrderParkedPanel::PopupTradeDlg(const DataRspParkedOrderAction& rawData, BOOL bIsQuery)
{
    CTradeInfoDlg* pInfoDlg=TRADEINFODLG(this);
	if(pInfoDlg==NULL) return;
	string title;
    wxString info;
	BOOL bShowInfo = FALSE;

	LOG_DATA* pData = new LOG_DATA;
    info.reserve(512);
	if(rawData.RspInfoField.ErrorID == 0) {
		if(!bIsQuery) 
			title = "PARK_SEND_CANCEL_OK";
		else 
			title = "PARK_CONVERT_CANCEL_OK";
		info.Printf(LOADFORMATSTRING(PARK_ORDER_FORMAT,"%s%s%s"),//文本必须包含%s,%s,%s
				rawData.ParkedOrderAction.OrderSysID, 
				rawData.ParkedOrderAction.InstrumentID,
				Number2String(rawData.ParkedOrderAction.VolumeChange).c_str());

		bShowInfo = NeedOrderSuccessConfirm(); 

		pData->strFormat = "PARK_ORDER_FORMAT";
		pData->vParam.push_back(LOG_PARAM(rawData.ParkedOrderAction.OrderSysID));
		pData->vParam.push_back(LOG_PARAM(rawData.ParkedOrderAction.OrderSysID));
		pData->vParam.push_back(LOG_PARAM(Number2String(rawData.ParkedOrderAction.VolumeChange)));
	}
	else {
		if(!bIsQuery) {
			title = "PARK_SEND_CANCEL_FAILED";
		}
		else {
			title = "PARK_CONVERT_CANCEL_FAILED";
		}
		info.Printf(LOADFORMATSTRING(TRADER_ERROR_FORMAT,"%s%s%s"),//文本必须包含%s,%s,%s
			rawData.ParkedOrderAction.ParkedOrderActionID, Number2String(rawData.RspInfoField.ErrorID).c_str(), rawData.RspInfoField.ErrorMsg);
		bShowInfo = NeedOrderFailConfirm();

		pData->strFormat = "TRADER_ERROR_FORMAT";
		pData->vParam.push_back(LOG_PARAM(rawData.ParkedOrderAction.ParkedOrderActionID));
		pData->vParam.push_back(LOG_PARAM(Number2String(rawData.RspInfoField.ErrorID)));
		pData->vParam.push_back(LOG_PARAM(rawData.RspInfoField.ErrorMsg));
	}
	pInfoDlg->ShowTradeMsg(info, -1, LOADSTRING_TEXT(title), -1, bShowInfo);

	pData->strTitle = title;
	pInfoDlg->WriteLog(pData);
}

void CQueryOrderParkedPanel::PopupTradeDlg(const DataRspRemoveParkedOrder& rawData)
{
    CTradeInfoDlg* pInfoDlg=TRADEINFODLG(this);
	if(pInfoDlg==NULL) return;
	string title;
    wxString info;
	BOOL bShowInfo = FALSE;

	LOG_DATA* pData = new LOG_DATA;
    info.reserve(512);
	if(rawData.RspInfoField.ErrorID == 0) {
		title = "PARK_DELETE_OK";
		info.Printf(LOADFORMATSTRING(PARK_DELETE_OK_TEXT,"%s"),rawData.RemoveParkedOrder.ParkedOrderID);//文本必须包含%s
		bShowInfo = NeedOrderSuccessConfirm(); 

		pData->strFormat = "PARK_DELETE_OK_TEXT";
		pData->vParam.push_back(LOG_PARAM(rawData.RemoveParkedOrder.ParkedOrderID));
	}
	else {
		title = "PARK_DELETE_FAIL";
		info.Printf(LOADFORMATSTRING(PARK_DELETE_FAIL_TEXT,"%s%s"),//文本必须包含%s,%s
			Number2String(rawData.RspInfoField.ErrorID).c_str(), rawData.RspInfoField.ErrorMsg);
		bShowInfo = NeedOrderFailConfirm(); 

		pData->strFormat = "PARK_DELETE_FAIL_TEXT";
		pData->vParam.push_back(LOG_PARAM(Number2String(rawData.RspInfoField.ErrorID)));
		pData->vParam.push_back(LOG_PARAM(rawData.RspInfoField.ErrorMsg));
	}

	pInfoDlg->ShowTradeMsg(info, -1, LOADSTRING_TEXT(title), -1, bShowInfo);

	pData->strTitle = title;
	pInfoDlg->WriteLog(pData);
}

void CQueryOrderParkedPanel::PopupTradeDlg(const DataRspRemoveParkedOrderAction& rawData)
{
	CTradeInfoDlg* pInfoDlg=TRADEINFODLG(this);
	if(pInfoDlg==NULL) return;
	//pInfoDlg->Hide();
	string title;
	wxString info;
	BOOL bShowInfo = FALSE;

	LOG_DATA* pData = new LOG_DATA;
	info.reserve(512);
	if(rawData.RspInfoField.ErrorID == 0) {
		title = "PARK_DELETE_OK";
		info.Printf(LOADFORMATSTRING(PARK_DELETE_OK_TEXT,"%s"), //文本必须包含%s
			rawData.RemoveParkedOrderAction.ParkedOrderActionID);
		bShowInfo = NeedOrderSuccessConfirm(); 

		pData->strFormat = "PARK_DELETE_OK_TEXT";
		pData->vParam.push_back(LOG_PARAM(rawData.RemoveParkedOrderAction.ParkedOrderActionID));
	}
	else {
		title = "PARK_DELETE_FAIL";
		info.Printf(LOADFORMATSTRING(PARK_DELETE_FAIL_TEXT,"%s%s"),//文本必须包含%s,%s
			Number2String(rawData.RspInfoField.ErrorID).c_str(), rawData.RspInfoField.ErrorMsg);
		bShowInfo = NeedOrderFailConfirm(); 

		pData->strFormat = "PARK_DELETE_FAIL_TEXT";
		pData->vParam.push_back(LOG_PARAM(Number2String(rawData.RspInfoField.ErrorID)));
		pData->vParam.push_back(LOG_PARAM(rawData.RspInfoField.ErrorMsg));
	}
	pInfoDlg->ShowTradeMsg(info, -1, title, -1, bShowInfo);

	pData->strTitle = title;
	pInfoDlg->WriteLog(pData);
}

bool CQueryOrderParkedPanel::Init()
{
	m_pwxExtListCtrl->DeleteAllColumns();

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if(nColCount!=0)
		for(int i=0; i<nColCount; i++)
			m_pwxExtListCtrl->DeleteColumn(0);

	m_iRowCnt=0;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	m_pListCfg = pCfgMgr->GetListCfg(GID_QUERY_ORDER_PARKED);//
	if ( NULL == m_pListCfg )
	{
		return false;
	}

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

	nColCount = pCfgMgr->GetListColCount(GID_QUERY_ORDER_PARKED);

	m_FieldID2ColumnID.clear();
	m_FieldAttr.clear();
	for(int i = 0;  i < nColCount; i++) {
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_PARKED, i);
		//LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_QUERY_ORDER_CONDITION, i);
		wxASSERT(pColCfg);
		if(pColCfg->id<ORDERPARKED_MIN || pColCfg->id>ORDERPARKED_MAX)
			continue;

		//"合约" "报单编号"栏，左对齐
		if( (pColCfg->id == ORDERPARKED_InstrumentID))// || (pColCfg->id == ORDERPARKED_EntrustID) )
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余的，右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        if(pColCfg->Visibility == 0)
			col.SetWidth(0);
        else
			col.SetWidth(pColCfg->Width);///* /
        if(pColCfg->Visibility != 0) {		
            col.SetText(pCfgMgr->GetColCfgCaption(GID_QUERY_ORDER_PARKED,pColCfg->id));
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

void CQueryOrderParkedPanel::ShowOrders(int nShowMode, BOOL bIsShowDelete)
{
	
	int nRowNo = -1, iCol = 0, iSumCol = 0;

	//删除当前的全部记录
	for(std::map<string, int>::iterator it=m_OrderKey2Row.begin(); it!=m_OrderKey2Row.end(); ++it) {
		LPVOID pVoid = (LPVOID)(m_pwxExtListCtrl->GetItemData(it->second));
		if(pVoid) {
			delete pVoid;
			pVoid=NULL;
		}
	}
    m_pwxExtListCtrl->DeleteAllItems();
	m_OrderRow2Key.clear();
    m_OrderKey2Row.clear();
    i_CurOrderRows=-1;

	ShowLocalParkeds(nRowNo, nShowMode, bIsShowDelete);
	ShowServerParkeds(nRowNo, nShowMode, bIsShowDelete);
	ShowServerParkedCancels(nRowNo, nShowMode, bIsShowDelete);

	i_CurOrderRows = nRowNo;

}

void CQueryOrderParkedPanel::ShowLocalParkeds(int& nRowID, int nShowMode, BOOL bIsShowDelete)
{

	if(m_poOrderServiceThread==NULL) return;
	
	m_poOrderServiceThread->LockObject();
	COrderServiceThread::MAPPARKEDORDER mapParked = m_poOrderServiceThread->GetOrderParkedMap();
	m_poOrderServiceThread->UnlockObject();
	COrderServiceThread::MAPPARKEDORDERITEM it = mapParked.begin();

	wxString strKey;
	string strkeytmp;
	for(; it!=mapParked.end(); ++it) {
		COrderServiceThread::PARKEDORDER& tParkedOrder = it->second;

		if(tParkedOrder.nNum==0) continue;
		if(!bIsShowDelete && tParkedOrder.nState==conParkedDelete) continue;
		if(nShowMode==0 && tParkedOrder.nState>conParkedDelete) continue;

		++nRowID;
		if(ShowOneOrder(nRowID, tParkedOrder) < 0) {
			--nRowID;
		}
		else {
			strKey.Printf("%d,%d", conTypeParkedLocal, tParkedOrder.nNum);
			strkeytmp = strKey;
			m_OrderKey2Row[strkeytmp] = nRowID;
			m_OrderRow2Key[nRowID] = strkeytmp;
		}
	}

}

void CQueryOrderParkedPanel::ShowServerParkeds(int& nRowID, int nShowMode, BOOL bIsShowDelete)
{

	std::map<string, DataRspParkedOrderInsert*>::iterator it;
	wxString strKey;
	string strkeytmp;
	for(it = m_ParkedInsert.begin(); it != m_ParkedInsert.end(); ++it) {			
		DataRspParkedOrderInsert* pOneParked = it->second;
		if(pOneParked != NULL) {
			
			if(!bIsShowDelete && pOneParked->ParkedOrder.Status==THOST_FTDC_PAOS_Deleted) continue;
			if(nShowMode==0 && pOneParked->ParkedOrder.Status!=THOST_FTDC_PAOS_NotSend) continue;
			
			++nRowID;
			if(ShowOneOrder(nRowID, pOneParked->ParkedOrder) < 0) {
				--nRowID;
				continue;
			}
			else {
				strKey.Printf("%d,%s", conTypeParkedServer, pOneParked->ParkedOrder.ParkedOrderID);
				strkeytmp = strKey;
				m_OrderKey2Row[strkeytmp] = nRowID;
				m_OrderRow2Key[nRowID] = strkeytmp;
			}
		}
	}

}

void CQueryOrderParkedPanel::ShowServerParkedCancels(int& nRowID, int nShowMode, BOOL bIsShowDelete)
{

	std::map<string, DataRspParkedOrderAction*>::iterator it;
	wxString strKey;
	string strkeytmp;
	for(it = m_ParkedAction.begin(); it != m_ParkedAction.end(); ++it) {			
		DataRspParkedOrderAction* pOneParked = it->second;
		if(pOneParked != NULL) {

			if(!bIsShowDelete && pOneParked->ParkedOrderAction.Status==THOST_FTDC_PAOS_Deleted) continue;
			if(nShowMode==0 && pOneParked->ParkedOrderAction.Status!=THOST_FTDC_PAOS_NotSend) continue;
			
			++nRowID;
			if(ShowOneOrder(nRowID, pOneParked->ParkedOrderAction) < 0) {
				--nRowID;
				continue;
			}
			else {
				strKey.Printf("%d,%s", conTypeParkedCancelServer, pOneParked->ParkedOrderAction.ParkedOrderActionID);
				strkeytmp = strKey;
				m_OrderKey2Row[strkeytmp] = nRowID;
				m_OrderRow2Key[nRowID] = strkeytmp;
			}
		}
	}
	//std::map<string, BOOL> m_ParkedMsgAction;

}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderParkedPanel::ShowOneOrder(int nRowNo, 
										 const COrderServiceThread::PARKEDORDER& tParkedOrder, 
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
		//m_pwxExtListCtrl->SetItemData(nRowNo, (long)tParkedOrder.nNum);
		m_pwxExtListCtrl->SetItemData(nRowNo, 0);
		item.SetId(nRowNo);	//设置行号
	}
	else {
		item.SetId(nRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}
	newText.Printf(LOADFORMATSTRING(PARK_LOCALID,"%s%d"), //文本必须包含%s,%d
		tParkedOrder.bIsAuto ? LOADSTRING(PARK_AUTO): LOADSTRING(PARK_MANUAL), tParkedOrder.nNum);
	SetColText(nRowNo,ORDERPARKED_ParkedID,newText);
	switch(tParkedOrder.nState) {
		case conParkedPause:
			newText =LOADSTRING(OIP_PAUSE);
			break;
		case conParkedActive:
			newText =LOADSTRING(OIP_ACTIVE);
			break;
		case conParkedDelete:
			newText = LOADSTRING(OIP_DELETEED);
			break;
		case conParkedConfirm:
			newText =LOADSTRING(OIP_SENDED);
			break;
		default:
			newText = "";
			break;
	};
	SetColText(nRowNo,ORDERPARKED_State,newText);

	newText.Printf("%s", tParkedOrder.req.InstrumentID);
	SetColText(nRowNo,ORDERPARKED_InstrumentID,newText);

	newText.Printf("%s", tParkedOrder.req.Direction==THOST_FTDC_D_Buy ?  LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));
	SetColText(nRowNo,ORDERPARKED_Direction,newText);

	newText.Printf("%s", tParkedOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN) :
		(tParkedOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)));

	SetColText(nRowNo,ORDERPARKED_OCMode,newText);

	newText.Printf("%s", tParkedOrder.req.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_HEDGE));

	SetColText(nRowNo,ORDERPARKED_Hedge,newText);

	newText.Printf("%d", tParkedOrder.req.VolumeTotalOriginal);

	SetColText(nRowNo,ORDERPARKED_RegVolume,newText);

	if(tParkedOrder.req.OrderPriceType==THOST_FTDC_OPT_AnyPrice)
		newText = LOADSTRING(OLS_MARKET);
	else
		newText = Double2String(tParkedOrder.req.LimitPrice).c_str();
	SetColText(nRowNo,ORDERPARKED_RegPrice,newText);

	newText="";
	if(tParkedOrder.nCreateTime>0)
		newText.Printf("%s", wxDateTime((time_t)tParkedOrder.nCreateTime).FormatISOTime());
	SetColText(nRowNo,ORDERPARKED_CreateTime,newText);

	//newText="";
	//if(tParkedOrder.nDeleteTime>0)
	//	newText.Printf("%s", wxDateTime((time_t)tParkedOrder.nDeleteTime).FormatISOTime());
	//SetColText(nRowNo,ORDERPARKED_DeleteTime,newText);

	newText="";//.Printf("%s", tParkedOrder.strErrorMsg);
	SetColText(nRowNo,ORDERPARKED_ErrorMsg,newText);

	return 0;
}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderParkedPanel::ShowOneOrder(int nRowNo, 
										 const CThostFtdcParkedOrderField& tOneOrder, 
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

	CThostFtdcParkedOrderField* pNewOrder = new CThostFtdcParkedOrderField;
	*pNewOrder = tOneOrder;

	if(bIsNew) {
		m_pwxExtListCtrl->InsertItem(nRowNo, item);
		m_pwxExtListCtrl->SetItemData(nRowNo, (long)pNewOrder);
		item.SetId(nRowNo);	//设置行号
	}
	else {
		LPVOID pVoid = (LPVOID)(m_pwxExtListCtrl->GetItemData(nRowNo));
		if(pVoid) {
			try {
			delete pVoid;
			pVoid=NULL;
			} catch(...) {}
		}
		m_pwxExtListCtrl->SetItemData(nRowNo, (long)pNewOrder);
		item.SetId(nRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}


		newText.Printf(LOADFORMATSTRING(PARK_SERVERID,"%s"), tOneOrder.ParkedOrderID);//文本必须包含%s
	SetColText(nRowNo,ORDERPARKED_ParkedID,newText);

		// 请求状态
		newText="";
		switch(tOneOrder.Status) {	
			case THOST_FTDC_PAOS_NotSend:			// 等待处理
				newText = LOADSTRING(OIP_NOSEND);
				break;
			case THOST_FTDC_PAOS_Send:			// 发送成功
				newText =LOADSTRING(OIP_SENDED);
				break;
			case THOST_FTDC_PAOS_Deleted:			// 发送失败
				newText = LOADSTRING(OIP_DELETEED);
				break;
		};
	SetColText(nRowNo,ORDERPARKED_State,newText);

	newText.Printf("%s", tOneOrder.InstrumentID);
	SetColText(nRowNo,ORDERPARKED_InstrumentID,newText);

		newText.Printf("%s", tOneOrder.Direction==THOST_FTDC_D_Buy ?  LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL));

	SetColText(nRowNo,ORDERPARKED_Direction,newText);

		newText.Printf("%s", tOneOrder.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN) :
					(tOneOrder.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)));

	SetColText(nRowNo,ORDERPARKED_OCMode,newText);

		newText.Printf("%s", tOneOrder.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_HEDGE));

	SetColText(nRowNo,ORDERPARKED_Hedge,newText);

		newText.Printf("%d", tOneOrder.VolumeTotalOriginal);

	SetColText(nRowNo,ORDERPARKED_RegVolume,newText);

	if(tOneOrder.OrderPriceType==THOST_FTDC_OPT_AnyPrice)
		newText = LOADSTRING(OLS_MARKET);
	else
		newText = Double2String(tOneOrder.LimitPrice).c_str();

	SetColText(nRowNo,ORDERPARKED_RegPrice,newText);

	newText.Printf("%s", tOneOrder.GTDDate);
		//newText = "";

	SetColText(nRowNo,ORDERPARKED_CreateTime,newText);

	newText.Printf("%s", tOneOrder.ErrorMsg);

	SetColText(nRowNo,ORDERPARKED_ErrorMsg,newText);

	return 0;
}

//start--以下注释由jacky.lee添加
//调用此函数之前，iRowNo所在的行肯定不存在
//增加一条记录
//end--
int CQueryOrderParkedPanel::ShowOneOrder(int nRowNo, 
										 const CThostFtdcParkedOrderActionField& tOneOrder, 
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

	CThostFtdcParkedOrderActionField* pNewOrder = new CThostFtdcParkedOrderActionField;
	*pNewOrder = tOneOrder;

	if(bIsNew) {
		m_pwxExtListCtrl->InsertItem(nRowNo, item);
		m_pwxExtListCtrl->SetItemData(nRowNo, (long)pNewOrder);
		item.SetId(nRowNo);	//设置行号
	}
	else {
		LPVOID pVoid = (LPVOID)(m_pwxExtListCtrl->GetItemData(nRowNo));
		if(pVoid) {
			delete pVoid;
			pVoid=NULL;
		}
		m_pwxExtListCtrl->SetItemData(nRowNo, (long)pNewOrder);
		item.SetId(nRowNo);	//设置行号
		m_pwxExtListCtrl->GetItem(item);
	}
	newText.Printf(LOADFORMATSTRING(OIP_SERVERCANCEL,"%s"), tOneOrder.ParkedOrderActionID);//文本必须包含%s
	SetColText(nRowNo,ORDERPARKED_ParkedID,newText);
	newText="";
	switch(tOneOrder.Status) {	
			case THOST_FTDC_PAOS_NotSend:			// 等待处理
				newText = LOADSTRING(OIP_NOSEND);
				break;
			case THOST_FTDC_PAOS_Send:			// 发送成功
				newText =LOADSTRING(OIP_SENDED);
				break;
			case THOST_FTDC_PAOS_Deleted:			// 发送失败
				newText = LOADSTRING(OIP_DELETEED);
				break;
	};
	SetColText(nRowNo,ORDERPARKED_State,newText);
	newText.Printf("%s", tOneOrder.InstrumentID);
	SetColText(nRowNo,ORDERPARKED_InstrumentID,newText);
	newText="";
	SetColText(nRowNo,ORDERPARKED_Direction,newText);
	newText="";
	SetColText(nRowNo,ORDERPARKED_OCMode,newText);
	newText="";
	SetColText(nRowNo,ORDERPARKED_Hedge,newText);
	newText="";
	SetColText(nRowNo,ORDERPARKED_RegVolume,newText);
	//newText="";
	newText = Double2String(tOneOrder.LimitPrice).c_str();
	SetColText(nRowNo,ORDERPARKED_RegPrice,newText);
	newText = "";
	//newText.Printf("%d", tOneOrder.nDeleteTime);
	SetColText(nRowNo,ORDERPARKED_CreateTime,newText);
	newText.Printf("%s", tOneOrder.ErrorMsg);
	SetColText(nRowNo,ORDERPARKED_ErrorMsg,newText);

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
	SetColText( ALLORDER_OrderLocalID );
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

void CQueryOrderParkedPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
	//FindWindowById(GID_QUERY_ORDER_CONDITION,this)->SetPosition(wxPoint(0,30));
	wxWindow* pWin = NULL;
	for(int i=ID_QUERYPARKED_BUTTONQUERY; i<ID_QUERYPARKED_NUM; i++)
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

void CQueryOrderParkedPanel::OnButtonQuery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPARKED_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete);

}

void CQueryOrderParkedPanel::OnButtonEdit(wxCommandEvent& evt)
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
//	char strTemp[1024];
//	ZeroMemory(strTemp, 1024);
//	sscanf_s(itr2k->second.c_str(), "%d", &nType);
//	switch(nType) {
//	case conTypeParkedLocal:
//		sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
//		break;
//	case conTypeParkedServer:
//		sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
//		break;
//	case conTypeParkedCancelServer:
//		sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
//		break;
//	default:
//		break;
//	};
	string strSysID;
	vector<string> outData;
	if(SplitString(itr2k->second, outData)<2) 
		return;
	nType = atoi(outData[0].c_str());
	switch(nType) {
	case conTypeParkedLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeParkedServer:
	case conTypeParkedCancelServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	bool bFail = false;
	BOOL bRet = FALSE;
	switch(nType) {
	case conTypeParkedLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();
		COrderServiceThread::MAPPARKEDORDERITEM it = mapParked.find(nNum);

		BOOL bFound = (it != mapParked.end());
		COrderServiceThread::PARKEDORDER tParkedOrder={0};
		if(bFound) 
			tParkedOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) 	{
			if(tParkedOrder.nState < conParkedDelete) {

				PlatformStru_InstrumentInfo insInfo;
				memset(&insInfo,0,sizeof(insInfo));
				if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(tParkedOrder.req.InstrumentID,insInfo)!=0) 
					insInfo.PriceTick = 1.0f;

				CEditOrderParkedDailog* pDlg = new CEditOrderParkedDailog(this, wxID_ANY, 
						LOADSTRING(EOPD_ECLPO), wxDefaultPosition, wxSize(310, 250));
				pDlg->SetPriceTick(insInfo.PriceTick);
				pDlg->SetParkedOrder(tParkedOrder);
				if(pDlg->ShowModal()==wxID_OK) {
					tParkedOrder = pDlg->GetParkedOrder();

					// 同步加锁，保证数据的一致性
					m_poOrderServiceThread->LockObject();

					// 判断修改期间止盈止损单是否成交
					it = mapParked.find(nNum);
					if(it != mapParked.end()) {
						m_poOrderServiceThread->OrderParkedSet(nNum, tParkedOrder);
						//mapCondition[nConditionNum] = tCondOrder;
					}

					// 同步解锁
					m_poOrderServiceThread->UnlockObject();

					ShowOneOrder(SelectItemId, tParkedOrder, FALSE);
				}
				else {
					//bFail = true;
				}
				pDlg->Destroy();
			}
			else {
				bFail = true;
			}
		}
		else {
			bFail = true;
		}
		if(bFail) {
			wxMessageDialog dialog( NULL,LOADSTRING(PARKED_ERROR_MESSAGE),LOADSTRING(PARKED_ERROR_TITLE) ,wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	case conTypeParkedServer:
	{
	}
		break;
	};

}

wxString CQueryOrderParkedPanel::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
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

wxString CQueryOrderParkedPanel::GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl)
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

void CQueryOrderParkedPanel::OnButtonDelete(wxCommandEvent& evt)
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
	//case conTypeParkedLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeParkedServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//case conTypeParkedCancelServer:
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
	case conTypeParkedLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeParkedServer:
	case conTypeParkedCancelServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击预埋单模块按钮[撤单]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	bool bFail = false;
	BOOL bRet = FALSE;

	wxExtListItem item;
	item.SetId(SelectItemId);
	item.SetColumn(0);
	
	switch(nType) {
	case conTypeParkedLocal:
	{
		if(nNum<=0) return;
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();
		COrderServiceThread::MAPPARKEDORDERITEM it = mapParked.find(nNum);
		
		BOOL bFound = (it != mapParked.end());
		COrderServiceThread::PARKEDORDER tParkedOrder={0};
		if(bFound) 
			tParkedOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) {
			if(tParkedOrder.nState < conParkedDelete) {
				m_poOrderServiceThread->LockObject();
				BOOL bDelete = m_poOrderServiceThread->OrderParkedDelete(nNum);
				m_poOrderServiceThread->UnlockObject();
				if(bDelete) {
					item.SetText(LOADSTRING(OIP_DELETEED));
					m_pwxExtListCtrl->SetItem(item);
				}
				else {
					bFail = true;
				}
			}
			else {
				bFail = true;
			}
		}
		else {
			bFail = true;
		}
		if(bFail) {
			wxMessageDialog dialog( NULL,LOADSTRING(PARK_DELETENOTEXIST),LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	case conTypeParkedServer:
	{
		CThostFtdcParkedOrderField* pParked = 
				(CThostFtdcParkedOrderField*)(m_pwxExtListCtrl->GetItemData(SelectItemId));
		if(pParked==NULL) 
			return;

		PlatformStru_RemoveParkedOrder tRemoveParked;
        memset(&tRemoveParked,0,sizeof(tRemoveParked));
		
        strncpy(tRemoveParked.Thost.BrokerID, pParked->BrokerID, sizeof(tRemoveParked.Thost.BrokerID)-1);
		strncpy(tRemoveParked.Thost.InvestorID, pParked->InvestorID, sizeof(tRemoveParked.Thost.InvestorID)-1);
		strncpy(tRemoveParked.Thost.ParkedOrderID, pParked->ParkedOrderID, sizeof(tRemoveParked.Thost.ParkedOrderID)-1);
		///请求删除预埋单
        int nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqRemoveParkedOrder(tRemoveParked):-1;//, ++COrderServiceThread::sm_nReqNum);
		if(nRet != 0) {
			//if(NeedOrderFailConfirm())
			//	ShowTradeInfoDlg(wxString("删除预埋单失败"), poEntity->GetOrderReq(nErrPos)); // title: 中国国际期货网上交易终端
		}
	}
		break;
	case conTypeParkedCancelServer:
	{
		CThostFtdcParkedOrderActionField* pParked = 
				(CThostFtdcParkedOrderActionField*)(m_pwxExtListCtrl->GetItemData(SelectItemId));
		if(pParked==NULL) 
			return;

		PlatformStru_RemoveParkedOrderAction tRemoveParkedAction;
        memset(&tRemoveParkedAction,0,sizeof(tRemoveParkedAction));
		
        strncpy(tRemoveParkedAction.Thost.BrokerID, pParked->BrokerID, sizeof(tRemoveParkedAction.Thost.BrokerID)-1);
		strncpy(tRemoveParkedAction.Thost.InvestorID, pParked->InvestorID, sizeof(tRemoveParkedAction.Thost.InvestorID)-1);
		strncpy(tRemoveParkedAction.Thost.ParkedOrderActionID, pParked->ParkedOrderActionID, sizeof(tRemoveParkedAction.Thost.ParkedOrderActionID)-1);
		///请求删除预埋单
        int nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqRemoveParkedOrderAction(tRemoveParkedAction):-1;//, ++COrderServiceThread::sm_nReqNum);
		if(nRet != 0) {
			//if(NeedOrderFailConfirm())
			//	ShowTradeInfoDlg(wxString("删除预埋撤单失败"), poEntity->GetOrderReq(nErrPos)); // title: 中国国际期货网上交易终端
		}
	}
		break;
	};

}

void CQueryOrderParkedPanel::OnButtonPause(wxCommandEvent& evt)
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
	//case conTypeParkedLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeParkedServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//case conTypeParkedCancelServer:
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
	case conTypeParkedLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeParkedServer:
	case conTypeParkedCancelServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击预埋单模块按钮[暂停]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	bool bFail = false;
	BOOL bRet = FALSE;

	wxExtListItem item;
	item.SetId(SelectItemId);
	item.SetColumn(0);
	
	switch(nType) {
	case conTypeParkedLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();
		COrderServiceThread::MAPPARKEDORDERITEM it = mapParked.find(nNum);
		COrderServiceThread::PARKEDORDER tParkedOrder={0};
		BOOL bFound = (it!=mapParked.end());
		if(bFound)
			tParkedOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) {
			if(tParkedOrder.nState == conParkedActive) {
				tParkedOrder.nState = conParkedPause;
				m_poOrderServiceThread->LockObject();
				m_poOrderServiceThread->OrderParkedSet(nNum, tParkedOrder);
				m_poOrderServiceThread->UnlockObject();
				ShowOneOrder(SelectItemId, tParkedOrder, FALSE);
			}
			else {
				bFail = true;
			}
		}
		else {
			bFail = true;
		}
		if(bFail) {
			//wxMessageDialog dialog( NULL,LOADSTRING(PARK_PAUSEFIALED) ,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			wxMessageDialog dialog( NULL,LOADSTRING(PARKED_ERROR_MESSAGE) ,LOADSTRING(PARKED_ERROR_TITLE),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	};

}

void CQueryOrderParkedPanel::OnButtonActive(wxCommandEvent& evt)
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
	//case conTypeParkedLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeParkedServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//case conTypeParkedCancelServer:
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
	case conTypeParkedLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeParkedServer:
	case conTypeParkedCancelServer:
		strSysID = outData[1];
		break;
	default:
		break;
	};

	if(nNum<=0) return;
	if(m_poOrderServiceThread==NULL) return;
	
	USERLOG_INFO("用户单击预埋单模块按钮[激活]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	bool bFail = false;
	BOOL bRet = FALSE;

	wxExtListItem item;
	item.SetId(SelectItemId);
	item.SetColumn(0);
	
	switch(nType) {
	case conTypeParkedLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();
		COrderServiceThread::MAPPARKEDORDERITEM it = mapParked.find(nNum);
		COrderServiceThread::PARKEDORDER tParkedOrder={0};
		BOOL bFound = (it!=mapParked.end());
		if(bFound)
			tParkedOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) {
			if(tParkedOrder.nState == conParkedPause) {
				tParkedOrder.nState = conParkedActive;
				m_poOrderServiceThread->LockObject();
				m_poOrderServiceThread->OrderParkedSet(nNum, tParkedOrder);
				m_poOrderServiceThread->UnlockObject();
				ShowOneOrder(SelectItemId, tParkedOrder, FALSE);
			}
			else {
				bFail = true;
			}
		}
		else {
			bFail = true;
		}
		if(bFail) {
			wxMessageDialog dialog( NULL,LOADSTRING(PARKED_ERROR_MESSAGE) ,LOADSTRING(PARKED_ERROR_TITLE),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	};

}

void CQueryOrderParkedPanel::OnButtonDoNow(wxCommandEvent& evt)
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
	//case conTypeParkedLocal:
	//	sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
	//	break;
	//case conTypeParkedServer:
	//	sscanf_s(itr2k->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
	//	break;
	//case conTypeParkedCancelServer:
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
	case conTypeParkedLocal:
		nNum = atoi(outData[1].c_str());
		break;
	case conTypeParkedServer:
	case conTypeParkedCancelServer:
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
		case conTypeParkedLocal:
			strTitle = LOADSTRING(PARK_DONOW_LOCALINSERT);
			break;
		case conTypeParkedServer:
			strTitle = LOADSTRING(PARK_DONOW_SERVERINSERT);
			break;
		case conTypeParkedCancelServer:
			strTitle = LOADSTRING(PARK_DONOW_SERVERCANCEL);
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

	USERLOG_INFO("用户单击预埋单模块按钮[立即执行]\n\"\n%s\n\"", 
			GetListCtrlRowLog(m_pwxExtListCtrl, SelectItemId).c_str());

	BOOL bRet = FALSE;
	switch(nType) {
	case conTypeParkedLocal:
	{
		m_poOrderServiceThread->LockObject();
		COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();
		COrderServiceThread::MAPPARKEDORDERITEM it = mapParked.find(nNum);
		COrderServiceThread::PARKEDORDER tParkedOrder={0};
		BOOL bFound = (it!=mapParked.end());
		if(bFound)
			tParkedOrder = it->second;
		m_poOrderServiceThread->UnlockObject();

		if(bFound) {
			if(!(tParkedOrder.nState == conParkedActive && tParkedOrder.nRegState == conReqWaiting)) {
				wxMessageDialog dialog( NULL, LOADSTRING(PARK_DELETE_NOTACTIVE), LOADSTRING(USERLOGINDLG_INFORMATION), wxOK|wxICON_ERROR);
				bOK = dialog.ShowModal()==wxID_YES;
				return;
			}
			m_poOrderServiceThread->LockObject();
			bRet = m_poOrderServiceThread->OrderParkedDoNow(nNum);
			m_poOrderServiceThread->UnlockObject();
			//it = mapParked.find(nNum);
			//bFound = (it!=mapParked.end());
			//if(bFound)
			//	tParkedOrder = it->second;
			//if(bFound)
			//	ShowOneOrder(SelectItemId, it->second, FALSE);
		}

		if(!bRet) 
		{
			wxMessageDialog dialog( NULL,LOADSTRING(PARK_DELETE_NOTEXIST),LOADSTRING(USERLOGINDLG_INFORMATION),wxOK|wxICON_ERROR);
			dialog.ShowModal();
		}
	}
		break;
	default:
		break;
	};
}

void CQueryOrderParkedPanel::OnButtonClear(wxCommandEvent& evt)
{

	if(m_poOrderServiceThread==NULL) return;
	
	COrderServiceThread::MAPPARKEDORDER& mapParked = m_poOrderServiceThread->GetOrderParkedMap();

	int nCount = 0;

	nCount += mapParked.size();
	nCount += m_ParkedInsert.size();
	nCount += m_ParkedAction.size();
	if(nCount == 0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }
	wxMessageDialog dialog( NULL, LOADSTRING(PARK_DELETE_ALL_ORDER), LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) return;

	USERLOG_INFO("用户单击预埋单模块按钮[全部撤单]并选择[是]确认\n\"\n%s\n\"", 
			GetListCtrlAllRowLog(m_pwxExtListCtrl).c_str());

	// 删除所有本地预埋单 
	{
		COrderServiceThread::MAPPARKEDORDERITEM it;
		std::vector<int> arrDelete;
		std::vector<int>::iterator itDel;

		m_poOrderServiceThread->LockObject();
		
		it = mapParked.begin();
		while(it!=mapParked.end()) {
			if(it->second.nState<conParkedDelete) {
				arrDelete.push_back(it->first);
			}
			it++;
		}

		for(int i=0; i<(int)arrDelete.size(); i++) {
			m_poOrderServiceThread->OrderParkedDelete(arrDelete[i]);
		}
		arrDelete.clear();

		m_poOrderServiceThread->UnlockObject();
	}

	// 删除所有服务器预埋单与预埋撤单 
	{
		
		std::map<int, string>::iterator it = m_OrderRow2Key.begin();

		CThostFtdcParkedOrderField* pParked = NULL;
		CThostFtdcParkedOrderActionField* pParkedAction = NULL;
		PlatformStru_RemoveParkedOrder tRemoveParked;

		int nType = 0, nNum = 0;
		string strSysID;
//		char strTemp[1024];
//		ZeroMemory(strTemp, 1024);

		for(; it != m_OrderRow2Key.end(); it++) {
//			sscanf_s(it->second.c_str(), "%d", &nType);
			vector<string> outData;
			if(SplitString(it->second, outData)<2) 
				continue;
			nType = atoi(outData[0].c_str());
			switch(nType) {
			case conTypeParkedServer:
			{
//				sscanf_s(it->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
				strSysID = outData[1];
				pParked = (CThostFtdcParkedOrderField*)(m_pwxExtListCtrl->GetItemData(it->first));
				if(pParked==NULL) 
					continue;

                memset(&tRemoveParked,0,sizeof(tRemoveParked));
                strncpy(tRemoveParked.Thost.BrokerID, pParked->BrokerID, sizeof(tRemoveParked.Thost.BrokerID)-1);
				strncpy(tRemoveParked.Thost.InvestorID, pParked->InvestorID, sizeof(tRemoveParked.Thost.InvestorID)-1);
				strncpy(tRemoveParked.Thost.ParkedOrderID, pParked->ParkedOrderID, sizeof(tRemoveParked.Thost.ParkedOrderID)-1);
				///请求删除预埋单
                int nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqRemoveParkedOrder(tRemoveParked):-1;//, ++COrderServiceThread::sm_nReqNum);
				if(nRet != 0) {
					//if(NeedOrderFailConfirm())
					//	ShowTradeInfoDlg(wxString("删除预埋单失败"), poEntity->GetOrderReq(nErrPos)); // title: 中国国际期货网上交易终端
				}

			}
				break;
			case conTypeParkedCancelServer:
			{
//				sscanf_s(it->second.c_str(), "%d,%s", &nType, strTemp, sizeof(strTemp)-1);
				strSysID = outData[1];
				pParkedAction = (CThostFtdcParkedOrderActionField*)(m_pwxExtListCtrl->GetItemData(it->first));
				if(pParkedAction==NULL) 
					continue;

                PlatformStru_RemoveParkedOrderAction tRemoveParkedAction;
                memset(&tRemoveParkedAction,0,sizeof(tRemoveParkedAction));

                strncpy(tRemoveParkedAction.Thost.BrokerID, pParkedAction->BrokerID, sizeof(tRemoveParkedAction.Thost.BrokerID)-1);
				strncpy(tRemoveParkedAction.Thost.InvestorID, pParkedAction->InvestorID, sizeof(tRemoveParkedAction.Thost.InvestorID)-1);
				strncpy(tRemoveParkedAction.Thost.ParkedOrderActionID, pParkedAction->ParkedOrderActionID, sizeof(tRemoveParkedAction.Thost.ParkedOrderActionID)-1);
				///请求删除预埋单
                int nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqRemoveParkedOrderAction(tRemoveParkedAction):-1;//, ++COrderServiceThread::sm_nReqNum);
				if(nRet != 0) {
					//if(NeedOrderFailConfirm())
					//	ShowTradeInfoDlg(wxString("删除预埋撤单失败"), poEntity->GetOrderReq(nErrPos)); // title: 中国国际期货网上交易终端
				}
			}
				break;
			case conTypeParkedLocal:
			default:
				continue;
				break;
			};
			
		}
	}
	
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPARKED_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete);

}

//配置文件发生变化
void CQueryOrderParkedPanel::OnCfgChanged(wxCommandEvent& evt)
{
	Init();

	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPARKED_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete);

}

void CQueryOrderParkedPanel::DoInitQryStep()
{
		ShowOrders(GetShowMode());

		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnInstrumentStatus, GID_QUERY_ORDER_PARKED, CallBack_InstrumentStatus);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspParkedOrderInsert, GID_QUERY_ORDER_PARKED, CallBack_ParkedOrderInsert);			//订阅预埋单录入请求响应
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspParkedOrderAction, GID_QUERY_ORDER_PARKED, CallBack_ParkedOrderAction);			//订阅预埋撤单录入请求响应
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspRemoveParkedOrder, GID_QUERY_ORDER_PARKED, CallBack_RemoveParkedOrder);			//订阅删除预埋单响应
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspRemoveParkedOrderAction, GID_QUERY_ORDER_PARKED, CallBack_RemoveParkedOrderAction);//订阅删除预埋撤单响应
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryParkedOrder, GID_QUERY_ORDER_PARKED, CallBack_QryParkedOrder);				//订阅请求查询预埋单响应
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryParkedOrderAction, GID_QUERY_ORDER_PARKED, CallBack_QryParkedOrderAction);	//订阅请求查询预埋撤单响应

		string strBrokerID;

		if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(m_strInvestorID,strBrokerID);
		LoadDataFromFile();

		int nRet = 0;
		PlatformStru_QryParkedOrder tQryParked;
		PlatformStru_QryParkedOrderAction tQryParkedAction;
        memset(&tQryParked,0,sizeof(tQryParked));
        memset(&tQryParkedAction,0,sizeof(tQryParkedAction));
        if(DEFAULT_SVR())
        {
		    DEFAULT_SVR()->ReqQryParkedOrder(tQryParked);
		    DEFAULT_SVR()->ReqQryParkedOrderAction(tQryParkedAction);
        }
}

void CQueryOrderParkedPanel::OnInitSubscribe(wxCommandEvent& evt)
{
	string strBrokerID="";

	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(m_strInvestorID,strBrokerID);
}

void CQueryOrderParkedPanel::OnContext(wxContextMenuEvent& evt)
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

	BOOL bIsRowCLick=evt.GetId();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏
	menu.Append(ID_QUERYPARKED_BUTTONQUERY,	LOADSTRING(CONTEXTMENU_QP_BUTTONQUERY));
	menu.AppendSeparator();	
	if(evt.GetId())
	{
		if(nType==conTypeParkedLocal)
			menu.Append(ID_QUERYPARKED_BUTTONEDIT, LOADSTRING(CONTEXTMENU_QP_BUTTONEDIT));
		menu.Append(ID_QUERYPARKED_BUTTONDELETE, LOADSTRING(CONTEXTMENU_QP_BUTTONDELETE));
		if(nType==conTypeParkedLocal)
			menu.Append(ID_QUERYPARKED_BUTTONPAUSE,	LOADSTRING(CONTEXTMENU_QP_BUTTONPAUSE));
		if(nType==conTypeParkedLocal)
			menu.Append(ID_QUERYPARKED_BUTTONACTIVE, LOADSTRING(CONTEXTMENU_QP_BUTTONACTIVE));
		if(nType==conTypeParkedLocal)
			menu.Append(ID_QUERYPARKED_BUTTONDONOW,	LOADSTRING(CONTEXTMENU_QP_BUTTONDONOW));
		menu.Append(ID_QUERYPARKED_BUTTONCLEAR,	LOADSTRING(CONTEXTMENU_QP_BUTTONCLEAR));
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
			int nType = 0, nNum = 0;
			string InstrumentId;
			CThostFtdcParkedOrderField* pParked = NULL;
			CThostFtdcParkedOrderActionField* pParkedAction = NULL;
			//char strTemp[1024];
			string strSysID;
			COrderServiceThread::PARKEDORDER tOrder;
			for(int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++) {
				if(m_pwxExtListCtrl->IsSelected( i )) {
					std::map<int, string>::iterator itr2k = m_OrderRow2Key.find(i);
					if(itr2k == m_OrderRow2Key.end()) {
						continue;
					}

					//ZeroMemory(strTemp, 1024);
					//sscanf_s(itr2k->second.c_str(), "%d", &nType);
					vector<string> outDataStr;
					if(SplitString(itr2k->second, outDataStr)<2) 
						continue;
					nType = atoi(outDataStr[0].c_str());
					switch(nType) {
					case conTypeParkedLocal:
						{
							//sscanf_s(itr2k->second.c_str(), "%d,%d", &nType, &nNum);
							m_poOrderServiceThread->LockObject();
							nNum = atoi(outDataStr[1].c_str());
							COrderServiceThread::MAPPARKEDORDER& mapParkedSub = m_poOrderServiceThread->GetOrderParkedMap();
							COrderServiceThread::MAPPARKEDORDERITEM it=mapParkedSub.find(nNum);
							if(it==mapParkedSub.end()) {
								m_poOrderServiceThread->UnlockObject();
								continue;
							}
							tOrder = it->second;
							m_poOrderServiceThread->UnlockObject();
							InstrumentId = tOrder.req.InstrumentID;
						}
						break;
					case conTypeParkedServer:
						pParked = (CThostFtdcParkedOrderField*)(m_pwxExtListCtrl->GetItemData(i));
						if(pParked==NULL) 
							continue;
						InstrumentId = pParked->InstrumentID;
						break;
					case conTypeParkedCancelServer:
						pParkedAction = (CThostFtdcParkedOrderActionField*)(m_pwxExtListCtrl->GetItemData(i));
						if(pParkedAction==NULL) 
							continue;
						InstrumentId = pParkedAction->InstrumentID;
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
			if ( bAdded ){
				SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);
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
		case ID_QUERYPARKED_BUTTONQUERY://
			newevt.SetId(ID_QUERYPARKED_BUTTONQUERY);
			OnButtonQuery(newevt);
			break;
		case ID_QUERYPARKED_BUTTONEDIT://
			newevt.SetId(ID_QUERYPARKED_BUTTONEDIT);
			OnButtonEdit(newevt);
			break;
		case ID_QUERYPARKED_BUTTONDELETE://
			newevt.SetId(ID_QUERYPARKED_BUTTONDELETE);
			OnButtonDelete(newevt);
			break;
		case ID_QUERYPARKED_BUTTONPAUSE://
			newevt.SetId(ID_QUERYPARKED_BUTTONPAUSE);
			OnButtonPause(newevt);
			break;
		case ID_QUERYPARKED_BUTTONACTIVE://
			newevt.SetId(ID_QUERYPARKED_BUTTONACTIVE);
			OnButtonActive(newevt);
			break;
		case ID_QUERYPARKED_BUTTONDONOW://
			newevt.SetId(ID_QUERYPARKED_BUTTONDONOW);
			OnButtonDoNow(newevt);
			break;
		case ID_QUERYPARKED_BUTTONCLEAR://
			newevt.SetId(ID_QUERYPARKED_BUTTONCLEAR);
			OnButtonClear(newevt);
			break;
		case ID_MENU_EXPORT://导出列表
			{
                wxString cap=((MainFrame*)GETTOPWINDOW())->GetModuleCaption(GetId());
				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
            SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_PARKEDORDER);
			break;
		default:
			break;
		}

        LeaveCriticalSection(&m_CS_EvtCallback);
    }
    evt.Skip();
}

void CQueryOrderParkedPanel::OnSelected(wxExtListEvent& event)
{
	int nRow = event.GetIndex();
	std::map<int, string>::iterator it = m_OrderRow2Key.find(nRow);
	if(it == m_OrderRow2Key.end()) return;

	int nType = 0;
//	sscanf_s(it->second.c_str(), "%d", &nType);
	vector<string> outData;
	if(SplitString(it->second, outData)<1) 
		return;
	nType = atoi(outData[0].c_str());
	wxWindow* pWin = NULL;
	switch(nType) {
	case conTypeParkedLocal:
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONEDIT);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONPAUSE);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONACTIVE);
		if(pWin) pWin->Enable(true);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONDONOW);
		if(pWin) pWin->Enable(true);
		break;
	case conTypeParkedServer:
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONEDIT);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONPAUSE);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONACTIVE);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONDONOW);
		if(pWin) pWin->Enable(false);
		break;
	case conTypeParkedCancelServer:
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONEDIT);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONPAUSE);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONACTIVE);
		if(pWin) pWin->Enable(false);
		pWin = FindWindowById(ID_QUERYPARKED_BUTTONDONOW);
		if(pWin) pWin->Enable(false);
		break;
	default:
		break;
	};
}

void CQueryOrderParkedPanel::OnOrderInsert(wxCommandEvent& evt)
{
	DataRspParkedOrderInsert* pParked = (DataRspParkedOrderInsert*)evt.GetClientData();
	if(pParked == NULL)
		return;

	if(pParked->RspInfoField.ErrorID == 0) {
		Lock();
		string strKey(pParked->ParkedOrder.ParkedOrderID);
		m_ParkedInsert[strKey]=pParked;
		m_ParkedMsgInsert[strKey]=FALSE;
		Unlock();
		ShowOrders(0);
	}
	PopupTradeDlg(*pParked);
	//这里不能delete
	//delete pParked;
}

void CQueryOrderParkedPanel::OnOrderAction(wxCommandEvent& evt)
{
	DataRspParkedOrderAction* pParked = (DataRspParkedOrderAction*)evt.GetClientData();
	if(pParked == NULL)
		return;

	if(pParked->RspInfoField.ErrorID == 0) {
		Lock();
		string strKey(pParked->ParkedOrderAction.ParkedOrderActionID);
		m_ParkedAction[strKey]=pParked;
		m_ParkedMsgAction[strKey]=FALSE;
		Unlock();
		ShowOrders(0);
	}
	PopupTradeDlg(*pParked);
	//这里不能delete
	//delete pParked;
}

void CQueryOrderParkedPanel::OnRemoveOrder(wxCommandEvent& evt)
{
	DataRspRemoveParkedOrder* pParked = (DataRspRemoveParkedOrder*)evt.GetClientData();
	if(pParked == NULL)
		return;

	if(pParked->RspInfoField.ErrorID == 0) {
		Lock();
		string strKey(pParked->RemoveParkedOrder.ParkedOrderID);
		std::map<string, DataRspParkedOrderInsert*>::iterator it;
		it = m_ParkedInsert.find(strKey);
		if(it != m_ParkedInsert.end()) {
			delete it->second;
			m_ParkedInsert.erase(strKey);
		}
		Unlock();
		ShowOrders(0);
	}
	PopupTradeDlg(*pParked);
	delete pParked;
}

void CQueryOrderParkedPanel::OnRemoveOrderAction(wxCommandEvent& evt)
{
	DataRspRemoveParkedOrderAction* pParked = (DataRspRemoveParkedOrderAction*)evt.GetClientData();
	if(pParked == NULL)
		return;

	if(pParked->RspInfoField.ErrorID == 0) {
		Lock();
		string strKey(pParked->RemoveParkedOrderAction.ParkedOrderActionID);
		std::map<string, DataRspParkedOrderAction*>::iterator it;
		it = m_ParkedAction.find(strKey);
		if(it != m_ParkedAction.end()) {
			delete it->second;
			m_ParkedAction.erase(strKey);
		}
		Unlock();
		ShowOrders(0);
	}
	PopupTradeDlg(*pParked);
	delete pParked;
}

void CQueryOrderParkedPanel::OnQueryParked(wxCommandEvent& evt)
{
	DataRspQryParkedOrder* pParked = (DataRspQryParkedOrder*)evt.GetClientData();
	if(pParked == NULL)
		return;

	if(pParked->RspInfoField.ErrorID == 0) {
		Lock();
		string strKey(pParked->ParkedOrder.ParkedOrderID);
		std::map<string, DataRspParkedOrderInsert*>::iterator it;
		it = m_ParkedInsert.find(strKey);
		if(it != m_ParkedInsert.end()) {
			it->second->ParkedOrder = pParked->ParkedOrder;
		}
		else {
			DataRspParkedOrderInsert* pNewParked = new DataRspParkedOrderInsert;
			ZeroMemory(pNewParked, sizeof(DataRspParkedOrderInsert));
			//pNewParked->BID==BID_RspParkedOrderInsert;
			pNewParked->ParkedOrder = pParked->ParkedOrder;
			m_ParkedInsert[strKey] = pNewParked;
		}

		std::map<string, BOOL>::iterator itMsg;
		itMsg = m_ParkedMsgInsert.find(strKey);
		if(itMsg != m_ParkedMsgInsert.end()) {
			if(itMsg->second == FALSE && 
					pParked->ParkedOrder.Status == THOST_FTDC_PAOS_Send) {
				m_ParkedMsgInsert[itMsg->first] = TRUE;
				DataRspParkedOrderInsert tOneParked;
				tOneParked.ParkedOrder = pParked->ParkedOrder;
				tOneParked.RspInfoField.ErrorID = pParked->ParkedOrder.ErrorID;
				strcpy(tOneParked.RspInfoField.ErrorMsg, pParked->ParkedOrder.ErrorMsg);
				PopupTradeDlg(tOneParked, TRUE);
			}
		}


		Unlock();
		ShowOrders(GetShowMode());
	}
	delete pParked;
}

void CQueryOrderParkedPanel::OnQueryParkedAction(wxCommandEvent& evt)
{
	DataRspQryParkedOrderAction* pParked = (DataRspQryParkedOrderAction*)evt.GetClientData();
	if(pParked == NULL)
		return;

	if(pParked->RspInfoField.ErrorID == 0) {
		Lock();
		string strKey(pParked->ParkedOrderAction.ParkedOrderActionID);
		std::map<string, DataRspParkedOrderAction*>::iterator it;
		it = m_ParkedAction.find(strKey);
		if(it != m_ParkedAction.end()) {
			it->second->ParkedOrderAction = pParked->ParkedOrderAction;
		}
		else {
			DataRspParkedOrderAction* pNewParked = new DataRspParkedOrderAction;
			ZeroMemory(pNewParked, sizeof(DataRspParkedOrderAction));
			//pNewParked->BID==BID_RspParkedOrderInsert;
			pNewParked->ParkedOrderAction = pParked->ParkedOrderAction;
			m_ParkedAction[strKey] = pNewParked;
		}

		std::map<string, BOOL>::iterator itMsg;
		itMsg = m_ParkedMsgAction.find(strKey);
		if(itMsg != m_ParkedMsgAction.end()) {
			if(itMsg->second == FALSE && 
					pParked->ParkedOrderAction.Status == THOST_FTDC_PAOS_Send) {
				m_ParkedMsgAction[itMsg->first] = TRUE;
				DataRspParkedOrderAction tOneParked;
				tOneParked.ParkedOrderAction = pParked->ParkedOrderAction;
				tOneParked.RspInfoField.ErrorID = pParked->ParkedOrderAction.ErrorID;
				strcpy(tOneParked.RspInfoField.ErrorMsg, pParked->ParkedOrderAction.ErrorMsg);
				PopupTradeDlg(tOneParked, TRUE);
			}
		}
		
		
		Unlock();
		ShowOrders(GetShowMode());
	}
	delete pParked;
}

void CQueryOrderParkedPanel::OnPlatSvrStatusChange(wxCommandEvent& evt)
{
/////开盘前
//#define THOST_FTDC_IS_BeforeTrading '0'
/////非交易
//#define THOST_FTDC_IS_NoTrading '1'
/////连续交易
//#define THOST_FTDC_IS_Continous '2'
/////集合竞价报单
//#define THOST_FTDC_IS_AuctionOrdering '3'
/////集合竞价价格平衡
//#define THOST_FTDC_IS_AuctionBalance '4'
/////集合竞价撮合
//#define THOST_FTDC_IS_AuctionMatch '5'
/////收盘
//#define THOST_FTDC_IS_Closed '6'

	DataInstrumentStatus* pStatus = (DataInstrumentStatus*)evt.GetClientData();
	if(pStatus == NULL)
		return;

	if(!(pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_BeforeTrading || 
			pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_NoTrading || 
			pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_Closed)) {
		Sleep(200);
		int nRet = 0;
		PlatformStru_QryParkedOrder tQryParked;
        memset(&tQryParked,0,sizeof(tQryParked));
		PlatformStru_QryParkedOrderAction tQryParkedAction;
        memset(&tQryParkedAction,0,sizeof(tQryParkedAction));
		strcpy(tQryParked.Thost.ExchangeID, pStatus->InstrumentStatusField.ExchangeID);
        strcpy(tQryParkedAction.Thost.ExchangeID, pStatus->InstrumentStatusField.ExchangeID);
        if(DEFAULT_SVR())
        {
		    DEFAULT_SVR()->ReqQryParkedOrder(tQryParked);
		    DEFAULT_SVR()->ReqQryParkedOrderAction(tQryParkedAction);
        }
	}
	delete pStatus;

}

void CQueryOrderParkedPanel::OnRequery(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPARKED_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete);
}

void CQueryOrderParkedPanel::OnLanguageChange(wxCommandEvent& evt)
{
	wxCheckBox* itemCheck = NULL;
	itemCheck = (wxCheckBox*)FindWindowById(ID_QUERYPARKED_CHECKSHOWDELETE, this);
	BOOL bIsShowDelete = itemCheck->GetValue();
	ShowOrders(GetShowMode(), bIsShowDelete);
	TRANSFORM_COLUMN_TEXT(GID_QUERY_ORDER_PARKED,m_pwxExtListCtrl);
	FindWindowById(ID_QUERYPARKED_CHECKSHOWDELETE,this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	FindWindowById(ID_QUERYPARKED_CHECKSHOWCONFIRM,this)->SetLabel(LOADSTRING(OIP_SENDED));
	//FindWindowById(ID_QUERYPARKED_RADIOSHOWALL,this)->SetLabel(LOADSTRING(BTN_QP_ALL));
	//FindWindowById(ID_QUERYPARKED_RADIOSHOWCONFIRM,this)->SetLabel(LOADSTRING(OIP_SENDED));
	m_pButtonQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	m_pButtonEdit->SetLabel(LOADSTRING(BTN_QP_BUTTONEDIT));
	m_pButtonDelete->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	m_pButtonPause->SetLabel(LOADSTRING(BTN_QP_BUTTONPAUSE));
    m_pButtonActive->SetLabel(LOADSTRING(BTN_QP_BUTTONACTIVE));
	m_pButtonDoNow->SetLabel(LOADSTRING(BTN_QP_BUTTONDONOW));
	m_pButtonClear->SetLabel(LOADSTRING(BTN_QP_BUTTONCLEAR));
}

void CQueryOrderParkedPanel::OnStatusChanged(wxCommandEvent& evt)
{
	wxCommandEvent evt2(wxEVT_COMMAND_BUTTON_CLICKED);
	OnButtonQuery(evt2);
}

void CQueryOrderParkedPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_PARKED ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_PARKED, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}

void CQueryOrderParkedPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUERY_ORDER_PARKED ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUERY_ORDER_PARKED, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}
