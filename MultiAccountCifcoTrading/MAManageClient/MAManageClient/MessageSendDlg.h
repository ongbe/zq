#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "MultiAccountStruct.h"
#include "AbnormalTree.h"
#include <set>

// CMessageSendDlg dialog

class CMessageSendDlg : public CDialog
{
	DECLARE_DYNAMIC(CMessageSendDlg)

public:
	CMessageSendDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMessageSendDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MSG_SEND };

	virtual BOOL OnInitDialog();

	void SetAccountData(int nRootAccountID, 
		std::map<int, AccountInfoEx> mapAccount,
		std::map<int, std::map<int, AccountInfoEx>> mapAccountTree);
	void SetMessageType(MessageType eType);
	void SetSelectData(std::vector<int> vSelectAccount);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void ShowSmallWindow();
	void ShowLargeWindow();
	void MoveControls( CWnd* pWnd, BOOL bLeft = TRUE );
	bool IsExistInAccountList( int nAccountID );

public:
	CButton m_btnShowUserList;
	CButton m_checkBox;
	CAbnormalTree m_userTree;
	CButton m_btnSelectAll;
	CButton m_btnUnSelectAll;
	CButton m_btnOnlineUser;
	CStatic m_staticTitle;
	CEdit m_edtTitle;
	CStatic m_staticContent;
	CEdit m_edtContent;
	CStatic m_staticExpiredDate;
	CDateTimeCtrl m_dateCtrl;
	CButton m_btnSendMessage;
	CListCtrl m_msgList;
	CListCtrl m_userList;
	CStatic m_groupBox;
	CButton m_btnMsgSendStatus;

private:
	MessageType m_eType;
	int m_nRootAccountID;
	std::map<int, AccountInfoEx> m_mapAccount;
	std::map<int, std::map<int, AccountInfoEx>> m_mapAccountTree;
	std::set<int> m_targetAccountSet;
	std::set<int> m_setInitSelectAccount;

	MessageInfo m_msgInfo;
	std::map<int, MessageInfo> m_mapMsgInfo;

public:
	afx_msg void OnBnClickedRadioCommon();
	afx_msg void OnBnClickedRadioPersonal();
	afx_msg void OnBnClickedCheck1();
	afx_msg LRESULT OnAccountListChangeMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnSelectAll();
	afx_msg void OnBnClickedBtnUnselectAll();
	afx_msg void OnBnClickedBtnOnlineUser();
	afx_msg void OnBnClickedBtnSendMsg();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnShowUserList();
	afx_msg void OnBnClickedBtnMsgSendStatus();
};
