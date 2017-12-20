#pragma once
#include "afxwin.h"
#include "MultiAccountStruct.h"

// CBrokerOperatorDlg dialog

class CBrokerOperatorDlg : public CDialog
{

	DECLARE_DYNAMIC(CBrokerOperatorDlg)

public:
	CBrokerOperatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBrokerOperatorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BROKER_OPERATOR };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetOperatorType( OperatorType eType);
	void SetBrokerInfo( const BrokerInfo& info);
	const BrokerInfo& GetBrokerInfo();

private:
	OperatorType m_eOperatorType;
	BrokerInfo m_brokerInfo;

public:
	CEdit m_edtCode;
	CEdit m_edtCompany;
	CButton m_cbxShare;
	afx_msg void OnBnClickedOk();
};
