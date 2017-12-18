#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COrderItemDlg dialog

class COrderItemDlg : public CDialog
{
	DECLARE_DYNAMIC(COrderItemDlg)

public:
	COrderItemDlg(CWnd* pParent = NULL);   // standard constructor
    COrderItemDlg(double dPrice, CString sDirect, CString sInstID, CString offSetFlag, int nCount, CWnd* pParent = NULL);   // standard constructor
	virtual ~COrderItemDlg();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_ORDERITEMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
    double m_dPrice;
    CString m_sDirect;
    CString m_sInstID;
    CString m_sOffsetFlag;
    int m_iCount;
    CSpinButtonCtrl m_spinPrice;
    CSpinButtonCtrl m_spinCount;
public:
    afx_msg void OnDeltaposSpinPrice(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinCount(NMHDR *pNMHDR, LRESULT *pResult);
private:
    CEdit m_editPrice;
    CEdit m_editCount;
};
