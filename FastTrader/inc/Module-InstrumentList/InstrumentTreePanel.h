#include "../inc/Module-Misc/TreeListCtrl.h"
class InstrumentItemData:public wxTreeItemData
{
public:	
	InstrumentItemData(const std::set<std::string>& datas){SetData(datas);}
	void SetData(const std::set<std::string>& datas){m_Instruments=datas;}
	void GetData(std::set<std::string>& datas){datas=m_Instruments;}
private:
	std::set<std::string> m_Instruments;

};
class InstrumentTreePanel:public wxPanel
{
public:
	InstrumentTreePanel(wxWindow *parent,
		wxWindowID id = GID_INSTRUMENT,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxEmptyString);
	~InstrumentTreePanel();
	void InitializeColumn(bool onlyText=false);
	void InitializeData();
	void InitializeItem();
	bool IsTypeNode(wxTreeItemId Id);
	wxString DisplayInstrument(wxString  strTypeName,wxString srcstring);
	void AddInstrumentInfo(string name,wxTreeItemId id);
	int GetColValue(int ColId, PlatformStru_InstrumentInfo* pField,  
		PlatformStru_InstrumentCommissionRate* commField,
		PlatformStru_InstrumentMarginRate* margField,
		std::string &strValue);
	//查询保证金率响应的回调函数，全部更新
	static int CallBackFunc_BID_RspQryInstrumentMarginRate(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询手续费率响应的回调函数，全部更新
	static int CallBackFunc_BID_RspQryInstrumentCommissionRate(const GUIModuleID GID,const AbstractBusinessData& data);
	void OnUpdateForInstrument(wxCommandEvent& evt);
	void OnContext(bool bIsRowCLick);
protected:
	void OnSize(wxSizeEvent& event);
	void OnTreeItemExpanding(wxTreeEvent& event);
	void OnTreeItemActived(wxTreeEvent& event);
	void OnTreeItemRightClick(wxTreeEvent& event);
	void OnTreeHeaderRightClick(wxListEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);
	void OnLanguageChanged(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
private:
	wxTreeListCtrl* m_treelist;
	wxTreeItemId    m_root;
	std::map<std::string,wxTreeItemId> m_Instrument2TreeItemMap;
};