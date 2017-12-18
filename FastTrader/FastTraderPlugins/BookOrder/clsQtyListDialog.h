#pragma once


// CQtyListDialog dialog

class CQtyListDialog : public CDialog
{
public:
	enum {	conButtonMaxNum = 9 };

	DECLARE_DYNAMIC(CQtyListDialog)

public:
	CQtyListDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQtyListDialog();

	void SetOutputEditQty(CEdit* poOutputEdit)
	{
		m_poOutputEditQty = poOutputEdit;
	}
	void ResetQtyList(int* pQty, int nCount);


// Dialog Data
	enum { IDD = IDD_QTYLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	CButton**		m_pButtonGroup;
	CEdit*			m_poOutputEditQty;
	int				m_nQtyGroup[conButtonMaxNum];

public:
	afx_msg void OnBnClickedQtyGroup();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
