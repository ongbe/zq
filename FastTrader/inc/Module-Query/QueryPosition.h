#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/notebook.h>
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
//#include "../Module-Misc2/PlatformStruct.h"
class CQueryPositionChild:public wxPanel
{
public:
	CQueryPositionChild(wxWindow *parent,int type);
	~CQueryPositionChild();

	void OnSize(wxSizeEvent& evt);
private:	
	int m_type;
	std::vector<PlatformStru_Contract> m_vectorContract;
	wxExtListCtrl *m_pWxExtListCtrl;
	DECLARE_EVENT_TABLE()
private:
	void CreateGUIControls_Total();
	void CreateGUIControls_Open();
	void CreateGUIControls_Contract();
};
class CQueryPositionDlg:public wxDialog
{
public:
	CQueryPositionDlg(wxWindow *parent);
	~CQueryPositionDlg();

	void OnSize(wxSizeEvent& evt);
private:	
	wxNotebook* m_notebook;
	DECLARE_EVENT_TABLE()
private:
	void CreateGUIControls();
};
