#pragma once

#include "../inc/Module-Misc/tools_util.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/COrderServiceThread.h"
#include "../inc/Module-Misc/ExtListCtrl.h"

class CQueryOrderConditionPanel: public wxPanel
{
public:
	enum {	conTypeCondLocal, conTypeCondServer };

	enum {	//GUI Enum Control ID Start
			ID_QUERYCONDITION_BUTTONQUERY = 1750,
			ID_QUERYCONDITION_BUTTONEDIT,
			ID_QUERYCONDITION_BUTTONDELETE,
			ID_QUERYCONDITION_BUTTONPAUSE,
			ID_QUERYCONDITION_BUTTONACTIVE,
			ID_QUERYCONDITION_BUTTONDONOW,
			ID_QUERYCONDITION_BUTTONCLEAR,
			ID_QUERYCONDITION_CHECKSHOWDELETE,
			ID_QUERYCONDITION_CHECKSHOWPOSITION,
			ID_QUERYCONDITION_CHECKSHOWCONFIRM,
			//ID_QUERYCONDITION_RADIOSHOWALL,
			//ID_QUERYCONDITION_RADIOSHOWCONFIRM,
			////GUI Enum Control ID End
			ID_QUERYCONDITION_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryOrderConditionPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_CONDITION,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderConditionPanel(void);

	void SetOrderServiceThread(COrderServiceThread* poOrderServiceThread) {
		m_poOrderServiceThread = poOrderServiceThread;
	};

	BOOL NeedConfirm();

	void OnSelected(wxExtListEvent& evt);
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

	int i_CurOrderCnt;					//当前委托类型的记录数
	int i_CurOrderRows;					//当前委托类型的记录数

private:
	static int CallBack_OrderRsp(const GUIModuleID GID,const AbstractBusinessData &data);			//报单回报
	static int CallBack_OrderInsert(const GUIModuleID GID,const AbstractBusinessData &data);		//报单录入回报

	BOOL DoRemoveCondOrder(PlatformStru_OrderInfo& tCondOrder);

	void ShowOrders(int nShowMode, BOOL bIsShowDelete=FALSE, BOOL bIsShowPosition=TRUE);
	void ShowLocalConditions(int& nRowID, int nShowMode, BOOL bIsShowDelete, BOOL bIsShowPosition);
	void ShowServerConditions(int& nRowID, int nShowMode, BOOL bIsShowDelete);
	int ShowOneOrder(int nRowNo, 
					 const COrderServiceThread::CONDITIONORDER& tConditionOrder, 
					 BOOL bIsNew=TRUE, BOOL bIsDeleted=FALSE);
	int ShowOneOrder(int nRowNo, 
					 const PlatformStru_OrderInfo& tOneOrder, 
					 BOOL bIsNew=TRUE, BOOL bIsDeleted=FALSE); 
	void OnInitSubscribe(wxCommandEvent& evt);

	void ShowNormalInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow);

	int GetShowMode() {
		wxCheckBox* pCheck = NULL;
		pCheck = (wxCheckBox*)FindWindowById(ID_QUERYCONDITION_CHECKSHOWCONFIRM, this);
		if(pCheck)
			return pCheck->GetValue()?1:0;
		//wxRadioButton* pRadio = NULL;
		//for(int i=0; i<4; i++) {
		//	pRadio = (wxRadioButton*)FindWindowById(ID_QUERYCONDITION_RADIOSHOWALL+i, this);
		//	if(pRadio) 
		//		if(pRadio->GetValue()) return i;
		//}
		return -1;
	};
	
	void OnSize(wxSizeEvent& event );
	void OnRightClick(wxMouseEvent& evt);
	void OnColRightClick(wxExtListEvent& evt);
	void OnItemRightClick(wxExtListEvent& evt);
	void OnContext(wxContextMenuEvent& evt);
	void DoInitQryStep();
	void OnRcvOrderRsp(wxCommandEvent& evt);
	void OnRcvOrderInsert(wxCommandEvent& evt);
	void OnRequery(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnStatusChanged(wxCommandEvent& evt);

	void SaveColWidth();
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
	wxString GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl);

private:
    static  CRITICAL_SECTION m_CS_EvtCallback;

	COrderServiceThread*	m_poOrderServiceThread;
	LPLIST_TABLE_CFG		m_pListCfg;
	int m_iColIndex;
	int m_iRowCnt;

	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID;			//记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;		//记录字段的显示属性
	std::map<string, int> m_OrderKey2Row;
	std::map<int, string> m_OrderRow2Key;
	
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

DECLARE_EVENT_TABLE()
};
