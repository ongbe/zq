
#pragma once
#include "stdafx.h"
class LinkStatusDlg :
    public wxDialog
{
public:
    LinkStatusDlg(wxWindow *parent);
	void OnSize(wxSizeEvent& evt);
	void OnConnectivityStatus(wxCommandEvent& event);
	bool HasData(){return bHasData;}
	void ChangeLanguage();
	//合约交易状态通知回调函数
    static int ConnectivityStatusCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);  
private:
	vector<string> m_ListHeaders;
	bool bHasData;
	wxExtListCtrl* m_pList;
    DECLARE_EVENT_TABLE()
};
