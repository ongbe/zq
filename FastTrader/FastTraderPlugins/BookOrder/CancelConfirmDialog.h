#pragma once
#include "afxcmn.h"

#include "ColorButton.h"

// CCancelConfirmDialog dialog

class CCancelConfirmDialog : public CDialog
{
	DECLARE_DYNAMIC(CCancelConfirmDialog)

public:
	CCancelConfirmDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCancelConfirmDialog();

	void SetCanCancelOrder(std::map<int, PlatformStru_OrderInfo>& mapCanCancelOrder)
	{
		 m_mapCanCancelOrder = mapCanCancelOrder;
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

	void CancelAllOrder(std::map<int, PlatformStru_OrderInfo>& mapCanCancelOrder);

// Dialog Data
	enum { IDD = IDD_CANCELORDERS };

protected:
	void UpdateItem_OrderList(PlatformStru_OrderInfo& tInfo);
	void ShowOrders();
	void SortList(int nCol, BOOL bChange=TRUE);
	void CancelOrder(PlatformStru_OrderInfo& tInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

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
	CColorButton	m_btnOk;
	CColorButton	m_btnCancel;

	CBrush*			m_pDlgBrushBk;
	CBrush			m_brushNull;

	std::map<int, PlatformStru_OrderInfo> m_mapCanCancelOrder;
	double			m_fPriceTick;
	DWORD			m_dwDlgBackColor;
	DWORD			m_dwDlgButtonBackColor;
};
