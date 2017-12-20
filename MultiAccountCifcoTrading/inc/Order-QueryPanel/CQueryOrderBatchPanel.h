#pragma once

#include "../inc/Module-Misc/tools_util.h"
#include "Module-Misc/ExtListCtrl.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/COrderServiceThread.h"

class CQueryOrderBatchPanel: public wxPanel
{
public:
	enum {	//GUI Enum Control ID Start
			ID_QUERYBATCH_BUTTONQUERY = 1700,
			////GUI Enum Control ID End
			ID_QUERYBATCH_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryOrderBatchPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_BATCH,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderBatchPanel(void);

	void SetOrderServiceThread(COrderServiceThread* poOrderServiceThread) {
		m_poOrderServiceThread = poOrderServiceThread;
	};

	void OnButtonQuery(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);////配置文件发生变化
	bool Init();

	int i_CurOrderCnt;					//当前委托类型的记录数
	int i_CurOrderRows;					//当前委托类型的记录数

private:

	void ShowOrderBatchs();
	bool GetOrderInfoByRowNo(int RowNo, DWORD& RowID, COrderServiceThread::BATCHSUBORDER& tBatchItem);
	int LoadOneOrderList(const COrderServiceThread::BATCHSUBORDER& tBatchOrder);
	void OnInitSubscribe(wxCommandEvent& evt);
	
	void OnSize( wxSizeEvent& event );
	void OnContext(wxContextMenuEvent& evt);
	void OnRequery(wxCommandEvent& evt);
	void DoInitQryStep();
	void SaveColWidth();

private:
    static  CRITICAL_SECTION m_CS_EvtCallback;

	COrderServiceThread*	m_poOrderServiceThread;
	LPLIST_TABLE_CFG		m_pListCfg;
	int m_iColIndex;
	int m_iRowCnt;

	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;//记录字段的显示属性
	//std::map<int, int> m_OrderBatchRow2Key;


	//下面数据保存列表中的报单记录的OrderKey和行ID
    //行ID是一个顺序递增的唯一值，标识一行，与行号无关
    //added by l. 20120222
    CRITICAL_SECTION m_CS;
    DWORD m_BaseRowID;      //从9999开始
    std::map<int,DWORD> m_MapOrderKey_RowID;    //BatchNum到RowID的对应Map
    std::map<DWORD,int> m_MapRowID_OrderKey;    //RowID到BatchNum的对应Map
    std::map<int, COrderServiceThread::BATCHSUBORDER> m_MapOrderKey_OrderInfo;    //BatchNum到BATCHSUBORDER的对应Map
    std::set<std::string> m_Set_Instruments_HaveRate;     //获得费率的合约集合，需要更新相应的报单的冻结数值

	
	wxButton*		m_pButtonQuery;
	wxExtListCtrl*	m_pwxExtListCtrl;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

DECLARE_EVENT_TABLE()
};
