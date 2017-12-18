#pragma once


// CCancelCondConfimDialog dialog

class CCancelCondConfimDialog : public CDialog
{
	DECLARE_DYNAMIC(CCancelCondConfimDialog)
public:
	static CListCtrl* g_plistSort;
	static int g_nSortColBak;
	static BOOL g_bSortAsc;
	static int CALLBACK CompareProc_CtrlList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public:
	CCancelCondConfimDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCancelCondConfimDialog();

	void SetCanCancelCond(MAPPRICETRIGGERITEM& mapCanCancelCond)
	{
		 m_mapCanCancelCond = mapCanCancelCond;
	};
	void SetPriceTick(double fPriceTick)
	{
		m_fPriceTick = fPriceTick;
	};
	void SetDlgBackColor(DWORD dwDlgBackColor)
	{
		m_dwDlgBackColor = dwDlgBackColor;
	};
	void SetDlgButtonBackColor(DWORD dwDlgButtonBackColor)
	{
		m_dwDlgButtonBackColor = dwDlgButtonBackColor;
	};

	void CancelAllCond(MAPPRICETRIGGERITEM& mapCanCancelCond);

// Dialog Data
	enum { IDD = IDD_CANCELCONDITIONS };

protected:
	void UpdateItem_OrderList(LOCALTRIGGERORDER& tInfo);
	void ShowOrders();
	void SortList(int nCol, BOOL bChange=TRUE);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnLvnColumnclickListorders(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomDraw_OrderList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHdnBegintrackListorders(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListorders(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancelselectall();
	afx_msg void OnBnClickedSelectall();

	DECLARE_MESSAGE_MAP()

protected:
	CListCtrl		m_listOrders;

	CBrush*			m_pDlgBrushBk;
	CBrush			m_brushNull;

	MAPPRICETRIGGERITEM m_mapCanCancelCond;
	double			m_fPriceTick;
	DWORD			m_dwDlgBackColor;
	DWORD			m_dwDlgButtonBackColor;
};
