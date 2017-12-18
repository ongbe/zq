#pragma once
#include "CommPublic.h"

// COrderConfirmDlg dialog

class COrderConfirmDlg : public CDialog
{
	DECLARE_DYNAMIC(COrderConfirmDlg)

public:
	COrderConfirmDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COrderConfirmDlg();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_ORDERCONFIRMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    void SetData(vector<PlatformStru_OrderInfo>& vData);

private:
    vector<PlatformStru_OrderInfo> m_vOrderData;
public:
    afx_msg void OnBnClickedOk();
};
