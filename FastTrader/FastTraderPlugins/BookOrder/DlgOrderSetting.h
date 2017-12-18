#pragma once
#include "afxwin.h"


// CDlgOrderSetting dialog

class CDlgOrderSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgOrderSetting)

public:
	CDlgOrderSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOrderSetting();

	void ResetOrderSetting(BOOL bHasLocal, const ORDERSETTING& setting) {
		m_bHasLocalSetting = bHasLocal;
		m_orderSetting = setting;
		m_nOrderOffset = setting.nCondOrderOffset;
	};
	BOOL GetHasLocalSetting() {
		return m_bHasLocalSetting;
	};
	const ORDERSETTING& GetOrderSetting() {
		return m_orderSetting;
	};

// Dialog Data
	enum { IDD = IDD_DLG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()

protected:

	CComboBox	m_cmbAuotOCKey;
	CComboBox	m_cmbOpenKey;
	CComboBox	m_cmbCloseKey;
	CComboBox	m_cmbCondOrderPriceMode;
	int			m_nOrderOffset;
	BOOL		m_bHasLocalSetting;

	ORDERSETTING	m_orderSetting;

};
