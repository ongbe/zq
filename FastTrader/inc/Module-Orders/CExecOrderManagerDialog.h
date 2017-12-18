#pragma once

#include "../Module-Misc/DataListCtrl.hpp"
#include "../PlatformDataMgr/PlatformDataSetList.hpp"

class CExecOrderManagerDialog : public wxDialog
{
public:

	enum
	{
		ID_EXCHANGE_COMB=2001,
		ID_TARGETPRODUCT_COMB,
		ID_TARGETEXPAIREDATE_COMB,
		ID_OPTIONDIRECTION_COMB,
		ID_OPTIONINSTRUMENT_COMB,
		ID_QUERY,
		ID_CANCELEXECORDER,
		ID_STATIC,
		SUMNUM_ID	// 标志ID号的数量
	};


public:
	static CExecOrderManagerDialog* CreateDailog(wxWindow* parent);
    static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
	static int RspExecOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);	// 查询执行宣告应答

public:
    CExecOrderManagerDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	virtual ~CExecOrderManagerDialog(void);

//    //void ShowAll();
//
private:

	void Init();
	void OnRspQryExecOrder(wxCommandEvent& evt);

//	void OnSize( wxSizeEvent& event );
	void OnActivated(wxExtListEvent& event);
	void OnContext(wxContextMenuEvent& evt);
    void OnCmbExchange(wxCommandEvent& evt);
    void OnCmbTargetID(wxCommandEvent& evt);
    void OnCmbExpareDate(wxCommandEvent& evt);
    void OnCmbOptionDirection(wxCommandEvent& evt);
    void OnBtnQuery(wxCommandEvent& evt);
	void OnRemove(wxCommandEvent& evt);
    //void OnRemoveAll(wxCommandEvent& evt);
//	void OnColumnResize(wxExtListEvent& event);
//
//	void SaveColWidth();
//	void OnMAPlatformAdd(wxCommandEvent& event);
//	void OnMAPlatformDelete(wxCommandEvent& evt);
//
//private:
//	LPLIST_TABLE_CFG       m_pListCfg;
//	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示 

private:

	// 期权执行的Key借用OrderKey
	CDataListCtrl<ExecOrderKey,PlatformStru_ExecOrderField>*  m_pwxExtListCtrl;
	CPlatformDataSetList<ExecOrderKey,PlatformStru_ExecOrderField>*  m_pMapExecOrder;

	wxFont m_Font;
	wxColor m_HeadBackgroundColor;
	wxColor m_HeadColor;
	wxColor m_BackgroundColor;
	wxColor m_TextColor;
    wxColor m_EvenLineBgColor;
	wxColor m_OddLineBgColor;
//    unsigned long   m_TextColor2;  //字体颜色
	long m_baseFTID;
	long m_baseUpdateSeq;

DECLARE_EVENT_TABLE()
};


