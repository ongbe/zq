#pragma once
#include "afxwin.h"
#include "MultiAccountStruct.h"

// CMsgDetailDlg dialog

class CMsgDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgDetailDlg)

public:
	CMsgDetailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgDetailDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MSG_DETAIL };

	virtual BOOL OnInitDialog();

	void SetMessageInfo( const MessageInfo& msgInfo );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);

	CEdit m_edtTitle;
	CEdit m_edtContent;
	CEdit m_edtSendList;
	CEdit m_edtWaitForSendList;

private:
	MessageInfo m_msgInfo;
public:
	CStatic m_staticSendTotal;
	CStatic m_staticWaitTotal;
	afx_msg void OnDestroy();
};
