#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <map>
#include "MultiAccountStruct.h"

// CBrokerManageDlg dialog

class CBrokerManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrokerManageDlg)

public:
	CBrokerManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBrokerManageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BROKER_MANAGE };

	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL OnInitDialog();

	void SetPermissionData( const PermissionData& data);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrokerAdd();
	afx_msg void OnBnClickedButtonBrokerMdf();
	afx_msg void OnBnClickedButtonBrokerDel();
	afx_msg void OnBnClickedButtonBrokerRefresh();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnClose();

private:
	void EnableControls(BOOL bFlag);
	void ShowBrokerInfo();
	CString ConventErrorMsg( const CString& csMsg );

private:
	CListCtrl m_listBroker;
	CButton m_btnAdd;
	CButton m_btnModify;
	CButton m_btnDel;
	CButton m_BtnRefresh;

private:
	std::map<int, BrokerInfo> m_mapBrokerInfo;
	BrokerInfo m_curBrokerInfo;
	
	bool m_bAddPermission;
	bool m_bModifyPermission;
	bool m_bDelPermission;
};
