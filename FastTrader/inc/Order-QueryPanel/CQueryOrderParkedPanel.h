#pragma once

#include "../inc/Module-Misc2/tools_util.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/CLocalOrderService.h"
#include "../inc/Module-Misc/ExtListCtrl.h"

class CQueryOrderParkedPanel : public wxPanel
{
public:
	enum {	conTypeParkedLocal, conTypeParkedServer, conTypeParkedCancelServer };

	enum {	//GUI Enum Control ID Start
			ID_QUERYPARKED_BUTTONQUERY = 2100,
			ID_QUERYPARKED_BUTTONEDIT,
			ID_QUERYPARKED_BUTTONDELETE,
			ID_QUERYPARKED_BUTTONPAUSE,
			ID_QUERYPARKED_BUTTONACTIVE,
			ID_QUERYPARKED_BUTTONDONOW,
			ID_QUERYPARKED_BUTTONCLEAR,
			ID_QUERYPARKED_CHECKSHOWDELETE,
			ID_QUERYPARKED_CHECKSHOWCONFIRM,
			//ID_QUERYPARKED_RADIOSHOWALL,
			//ID_QUERYPARKED_RADIOSHOWCONFIRM,
			////GUI Enum Control ID End
			ID_QUERYPARKED_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	typedef struct {
		DWORD	nParkedType;			// 预埋单类型
		DWORD	bIsShowed;				// 是否已经显示过
		char	strParkedID[24];		// 带类型的服务器预埋单ID
	} PARKEDSHOWED, *PPARKEDSHOWED;

public:
    CQueryOrderParkedPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_PARKED,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderParkedPanel(void);

	void SetLocalOrderService(CLocalOrderService* poLocalOrderService) {
		m_poLocalOrderService = poLocalOrderService;
	};
	void SetColText(int nRow, int id,wxString intext);
	void OnColumnResize(wxExtListEvent& event);

	int i_CurOrderCnt;					//当前委托类型的记录数
	int i_CurOrderRows;					//当前委托类型的记录数

	void OnButtonQuery(wxCommandEvent& evt);
	void OnButtonEdit(wxCommandEvent& evt);
	void OnButtonDelete(wxCommandEvent& evt);
	void OnButtonPause(wxCommandEvent& evt);
	void OnButtonActive(wxCommandEvent& evt);
	void OnButtonDoNow(wxCommandEvent& evt);
	void OnButtonClear(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);////配置文件发生变化
	bool Init();

private:
	static int CallBack_InstrumentStatus(const GUIModuleID GID,const AbstractBusinessData &data);			// 合约交易状态改变
	static int CallBack_ParkedOrderInsert(const GUIModuleID GID,const AbstractBusinessData &data);		//预埋单录入请求响应
	static int CallBack_ParkedOrderAction(const GUIModuleID GID,const AbstractBusinessData &data);		//预埋撤单录入请求响应
	static int CallBack_RemoveParkedOrder(const GUIModuleID GID,const AbstractBusinessData &data);		//删除预埋单响应
	static int CallBack_RemoveParkedOrderAction(const GUIModuleID GID,const AbstractBusinessData &data);	//删除预埋撤单响应
	static int CallBack_QryParkedOrder(const GUIModuleID GID,const AbstractBusinessData &data);			//请求查询预埋单响应
	static int CallBack_QryParkedOrderAction(const GUIModuleID GID,const AbstractBusinessData &data);		//请求请求查询预埋撤单响应

	BOOL NeedConfirm();
	BOOL NeedOrderSuccessConfirm();
	BOOL NeedOrderSuccessSound();
	BOOL NeedOrderFailConfirm();
	BOOL NeedOrderFailSound();
	BOOL NeedOrderTradeConfirm();
	BOOL NeedOrderTradeSound();
	BOOL NeedCancelSuccessConfirm();
	BOOL NeedCancelFailConfirm();

	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

	void ShowOrders(int nShowMode, BOOL bIsShowDelete=FALSE);
	void ShowLocalParkeds(int& nRowID, int nShowMode, BOOL bIsShowDelete);
	void ShowServerParkeds(int& nRowID, int nShowMode, BOOL bIsShowDelete);
	void ShowServerParkedCancels(int& nRowID, int nShowMode, BOOL bIsShowDelete);
	int ShowOneOrder(int nRowNo, 
					 const CLocalOrderService::PARKEDORDER& tParkedOrder, 
					 BOOL bIsNew=TRUE, BOOL bIsDeleted=FALSE);
	int ShowOneOrder(int nRowNo, 
					 const CThostFtdcParkedOrderField& tOneOrder, 
					 BOOL bIsNew=TRUE, BOOL bIsDeleted=FALSE); 
	int ShowOneOrder(int nRowNo, 
					 const CThostFtdcParkedOrderActionField& tOneOrder, 
					 BOOL bIsNew=TRUE, BOOL bIsDeleted=FALSE); 

	void PopupTradeDlg(const DataRspParkedOrderInsert& rawData, BOOL bIsQuery=FALSE);
	void PopupTradeDlg(const DataRspParkedOrderAction& rawData, BOOL bIsQuery=FALSE);
	void PopupTradeDlg(const DataRspRemoveParkedOrder& rawData);
	void PopupTradeDlg(const DataRspRemoveParkedOrderAction& rawData);

	int GetShowMode() {
		wxCheckBox* pCheck = NULL;
		pCheck = (wxCheckBox*)FindWindowById(ID_QUERYPARKED_CHECKSHOWCONFIRM, this);
		if(pCheck)
			return pCheck->GetValue()?1:0;
		//wxRadioButton* pRadio = NULL;
		//for(int i=0; i<2; i++) {
		//	pRadio = (wxRadioButton*)FindWindowById(ID_QUERYPARKED_RADIOSHOWALL+i, this);
		//	if(pRadio) 
		//		if(pRadio->GetValue()) return i;
		//}
		return -1;
	};

	void Lock() {
		m_oMutex.Lock();
	};
	void Unlock() {
		m_oMutex.Unlock();
	};
	
	void OnSize(wxSizeEvent& event );
	void OnSelected(wxExtListEvent& event);
	void OnRightClick(wxMouseEvent& evt);
	void OnColRightClick(wxExtListEvent& evt);
	void OnItemRightClick(wxExtListEvent& evt);

	void OnContext(wxContextMenuEvent& evt);
	void OnInitSubscribe(wxCommandEvent& evt);
	void DoInitQryStep();

	void OnOrderInsert(wxCommandEvent& evt);
	void OnOrderAction(wxCommandEvent& evt);
	void OnRemoveOrder(wxCommandEvent& evt);
	void OnRemoveOrderAction(wxCommandEvent& evt);
	void OnQueryParked(wxCommandEvent& evt);
	void OnQueryParkedAction(wxCommandEvent& evt);

	void OnPlatSvrStatusChange(wxCommandEvent& evt);

	void OnRequery(wxCommandEvent& evt);
	void OnStatusChanged(wxCommandEvent& evt);
	//定时器
	void OnTimer(wxTimerEvent& event);
	void SaveColWidth();
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
	wxString GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl);

private:
    static CRITICAL_SECTION	m_CS_EvtCallback;

	CLocalOrderService*		m_poLocalOrderService;
	LPLIST_TABLE_CFG		m_pListCfg;
	int m_iColIndex;
	int m_iRowCnt;

	bool	m_bNeedQueryParked;
	UINT	m_nLastStatusTime;


	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID;			//记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;		//记录字段的显示属性
	std::map<string, int> m_OrderKey2Row;
	std::map<int, string> m_OrderRow2Key;

	std::map<string, DataRspParkedOrderInsert*> m_ParkedInsert;
	std::map<string, DataRspParkedOrderAction*> m_ParkedAction;
	std::map<string, BOOL> m_ParkedMsgInsert;
	std::map<string, BOOL> m_ParkedMsgAction;
	
	wxButton*		m_pButtonQuery;
	wxButton*		m_pButtonEdit;
	wxButton*		m_pButtonDelete;
	wxButton*		m_pButtonPause;
	wxButton*		m_pButtonActive;
	wxButton*		m_pButtonDoNow;
	wxButton*		m_pButtonClear;
	wxExtListCtrl*	m_pwxExtListCtrl;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

	wxTimer			m_timer;
	wxMutex			m_oMutex;
	string			m_strInvestorID;

DECLARE_EVENT_TABLE()
};
