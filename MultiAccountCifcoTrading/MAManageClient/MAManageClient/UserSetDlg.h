#pragma once
#include "afxwin.h"
#include <vector>
#include "MyEdit.h"
#include "MultiAccountStruct.h"

// CUserSetDlg dialog

class CUserSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserSetDlg)

public:
	CUserSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_SET };

	virtual BOOL OnInitDialog();
	void SetIDCardInfoData(std::vector<IDCardInfo>& vIDCardInfo );
	void SetAccountInfo(const AccountInfo& accountInfo );
	const AccountInfo& GetAccountInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtUserName;
	CEdit m_edtContactInfo;
	CComboBox m_cbxIDCardType;
	CEdit m_edtIDCardNo;
	CButton m_chkAttr1;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedOk();

private:
	std::vector<IDCardInfo> m_vIDCardInfo;
	AccountInfo m_curAccountInfo;
};
