/***************************************************************************//**
*   @file         QuotPanel.h
*   @brief        行情板块,类的申明
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/20
********************************************************************************/

#ifndef _QUOTPANEL_H_
#define _QUOTPANEL_H_

#include "cfgMgr/CfgMgr.h"

#include "wx/button.h"
#include <map>
#include <string>

#include "../inc/ISvr/PlatformSvrMgr.h"
#include "MainGrid.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#define MAX_GROUP_COUNT 64
struct OneGroupInfo
{
	OneGroupInfo()
	{
		pWxButton=NULL;	
		table=NULL;
        pTextData=NULL;
	};
	DWORD	  GroupId;
	wxButton* pWxButton;
	QuotGridTable* table;
	map<int,vector<string>>* pTextData;
	std::string     name;
};

class CQuotPanel:public wxPanel
{
public:
	CQuotPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);

	virtual ~CQuotPanel(void);
	void SelectCurrentRow();
	void InitGridUI();
	void InitGridData();
    void DeleteRow(int nRow);
    void LoadUI(bool bFirst,bool bOnlyUI=false);
	void Release();
	int GetLineHeight(wxFont& cellfont);
	void OnRcvQuot(wxCommandEvent& evt);
	void OnSize( wxSizeEvent& event );
	void OnButtonClick(wxCommandEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);
	void OnSubscrible(wxCommandEvent& evt);
//	void OnGetInstrumentID(wxCommandEvent& evt);
	void InvalidRawQuot(PlatformStru_DepthMarketData& rawQuot,std::string& strInstrumentID);
	void OnPanelFocus(wxCommandEvent& evt);
	void OnSetGroup(wxCommandEvent& evt);
//	void OnGetGroup(wxCommandEvent& evt);
	void OnSetInstrumentID(wxCommandEvent& evt);
	void OnRangeSelected( wxGridRangeSelectEvent& evt );
	void OnCellLeftClick(wxGridEvent& evt);
	void OnCellLeftDoubleClick(wxGridEvent& evt);
	void OnRightClick(wxGridEvent& evt);
	void ContextMenuRespond(wxCommandEvent& event);
	void OnColumnResize(wxGridSizeEvent& event);
	void OnKeyDown( wxKeyEvent& event );
	void OnMAPlatformModify(wxCommandEvent& event);
	void LoadOldQuotData();
    bool GetCurInstrumentGrpInfo(std::string& GrpName,int& GrpID);
    void GetCurInstrumentID(std::string& strInstrument);
#ifdef _USE_MULTI_LANGUAGE
	void OnLanguageChanged(wxCommandEvent& event);
#endif
private:
	int UpdateCurrentInstrumentIDs(QuotGridTable *table);
	int GetIndexFromGroupId(int id,std::vector<OneGroupInfo>* allgroup=NULL);
	int GetButtonIdFromGroupId(int id,std::vector<OneGroupInfo>* allgroup=NULL);
	int GetIndexFromCtrlID(int id);
	void SaveColWidth();
private: 
	//合约组名称和合约组信息
	std::vector<OneGroupInfo> m_AllGroups;
	//当前合约组Index
	int m_nCurrentIndex;
	//网格控件
    MainGrid* m_grid;
	//当前右键点击的行号
	int m_RightClickRow;
    //合约组按钮是否显示到工具栏上面
    bool m_GroupButtonInToolBar;
	//
	wxMenu* m_pContextMenu;
	DECLARE_EVENT_TABLE()
private:
	IPlatformSingleSvr* m_pSvr;
    
    //初始化结束后一次加载全部合约行情
    bool m_bNeedLoadAllQuotData;

};


#endif