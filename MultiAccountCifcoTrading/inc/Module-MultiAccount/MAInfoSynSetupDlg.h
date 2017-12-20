#pragma once
#include "../MAServer/include/MAAccount/MultiAccountCmd.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
class CMAInfoSynSetupDlg :
	public wxDialog
{
public:
	enum
	{
		CMB_AUTO_SAVE=100,
		CMB_MAN_SAVE,
		CMB_AUTO_LOAD,
		CMB_MAN_LOAD,
		BTN_SAVE,
		BTN_LOAD,
		BTN_DEL_LOCAL,
		BTN_DEL_SERVER,
	};
	CMAInfoSynSetupDlg(wxWindow *parent=NULL);
	~CMAInfoSynSetupDlg(void);
	DECLARE_EVENT_TABLE()
	void OnBtnSave(wxCommandEvent& event);
	void OnBtnLoad(wxCommandEvent& event);
	void OnDelLocal(wxCommandEvent& event);
	void OnDelServer(wxCommandEvent& event);
	void OnMASocket(wxCommandEvent& event);
};
