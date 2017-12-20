#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MultiAccountStruct.h"

// CServerAddrDlg dialog

class CServerAddrDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerAddrDlg)

public:
	CServerAddrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerAddrDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERVER_ADDR };

	virtual BOOL OnInitDialog();

	void SetOperatorInfo(OperatorType eType, ServerType eServerType);
	void SetServerAddr( const ServerAddr& severAddr);
	ServerAddr& GetServerAddr();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedOk();

public:
	CEdit m_editIP;
	CEdit m_edtPort;

private:
	OperatorType m_eType;
	ServerType m_eServerType;
	ServerAddr m_addr;
};
