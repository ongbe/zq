#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <map>
#include "MultiAccountStruct.h"

// CServerManageDlg dialog

class CServerManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerManageDlg)

public:
	CServerManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerManageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERVER_MANAGE };

	virtual BOOL OnInitDialog();

	void InitControls();

	void SetPermissionData( const PermissionData& data);

	virtual BOOL PreTranslateMessage( MSG* pMsg );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void EnableAllControls(BOOL bFlag);
	void EnableAddrControls(BOOL bFlag);
	void AddServerAddress(ServerType eType);
	void ClearAllData();
	CString ConventErrorMsg( const CString& csMsg );

private:
	std::vector<BrokerInfo> m_vBrokerInfo;
	std::map<int, ServerGroup> m_mapServerGroup;
	ServerGroup m_curServerGroup;
	int m_nBrokerID;
	int m_nCurAddrGroupID;
	std::map<int, ServerAddr> m_mapServerAddr;
	ServerAddr m_curAddr;

	bool m_bAddModifyPermission;
	bool m_bDelPermission;

public:
	CComboBox m_cmbBroker;
	CListCtrl m_groupList;
	CListCtrl m_tradeAddrList;
	CListCtrl m_quoteAddrList;

	CButton m_btnGroupAdd;
	CButton m_btnGroupMdf;
	CButton m_btnGroupDel;

	CButton m_btnTradeAddrAdd;
	CButton m_btnTradeAddrDel;
	CButton m_btnTradeAddrModify;
	CButton m_btnQuoteAddrAdd;
	CButton m_btnQuoteAddrDel;
	CButton m_btnQuoteAddrModify;

	afx_msg void OnBnClickedButtonGroupAdd();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);

	afx_msg void OnCbnSelchangeComboBroker();

	afx_msg void OnBnClickedButtonTradeAddrDel();
	afx_msg void OnBnClickedButtonQuoteAddrAdd();
	afx_msg void OnBnClickedButtonQuoteAddrDel();

	afx_msg void OnBnClickedButtonGroupModify();
	afx_msg void OnBnClickedButtonGroupDel();
	
	afx_msg void OnBnClickedButtonTradeAddrAdd();
	afx_msg void OnLvnItemchangedListGroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTradeAddrModify();
	afx_msg void OnBnClickedButtonQuoteAddrModify();
};
