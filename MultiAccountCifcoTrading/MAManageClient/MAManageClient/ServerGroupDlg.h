#pragma once
#include "afxwin.h"
#include "MultiAccountStruct.h"


// CServerGroupDlg dialog
enum OperatorType;

class CServerGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerGroupDlg)

public:
	CServerGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerGroupDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERVER_GROUP };

	virtual BOOL OnInitDialog();

	void SetOperatorType(OperatorType eType);
	void SetServerGroup( const ServerGroup& severGroup);
	ServerGroup& GetServerGroup();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_edtGroup;
	CButton m_chkShare;

private:
	OperatorType m_eType;
	ServerGroup m_serverGroup;
public:
	afx_msg void OnBnClickedOk();
};
