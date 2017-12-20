#pragma once
#include <wx/treectrl.h>
class CConfigAppDlg : public wxDialog
{
public:
	enum
	{
		ID_TREE,
	};

public:
	CConfigAppDlg( wxWindow *parent, const int defaultItemId=-1,IPlatformSingleSvr* pSvr=NULL);
	virtual ~CConfigAppDlg(void);

	void InitializeTree();
	void SetDefaultShowPanel( const int defaultItemId);
	void OnSelChanged( wxTreeEvent& event );
	void OnShowPanel( wxCommandEvent& event );
	void SwitchShow( wxPanel* pPanel );
	void OnOK( wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnGroupChange(wxCommandEvent& event);
	class wxTreeItemIntData : public wxTreeItemData
	{
	public:
		wxTreeItemIntData() : m_data(-1) { }
		wxTreeItemIntData(int data ) : m_data(data) { }
		void SetData( int data ) { m_data = data; }
		int GetData() const { return m_data; }

	private:
		int  m_data;
	};

	DECLARE_EVENT_TABLE()
private:
	wxTreeCtrl*		m_treeCtrl;
	std::map<int, wxPanel*> m_panelMap;
	std::map<int, wxTreeItemId> m_treeItemMap;
	IPlatformSingleSvr* m_pSvr;
};
