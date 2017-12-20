// TestFastTraderPipeView.h : interface of the CTestFastTraderPipeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTFASTTRADERPIPEVIEW_H__75765DB1_11C6_4944_B869_9DC65FCB5DE0__INCLUDED_)
#define AFX_TESTFASTTRADERPIPEVIEW_H__75765DB1_11C6_4944_B869_9DC65FCB5DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestFastTraderPipeView : public CFormView
{
protected: // create from serialization only
	CTestFastTraderPipeView();
	DECLARE_DYNCREATE(CTestFastTraderPipeView)

public:
	//{{AFX_DATA(CTestFastTraderPipeView)
	enum { IDD = IDD_TESTFASTTRADERPIPE_FORM };
	CRichEditCtrl	m_ctlRichTextMsg;
	CString	m_strMasterHost;
	CString	m_strMasterPID;
	//}}AFX_DATA

// Attributes
public:
	CTestFastTraderPipeDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestFastTraderPipeView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestFastTraderPipeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ShowMsg(CString& strMsg, CRichEditCtrl& ctlRichText); 

// Generated message map functions
protected:
	//{{AFX_MSG(CTestFastTraderPipeView)
	afx_msg void On_Btn_LoginFastTrader();
	afx_msg void On_Btn_HelloFastTrader();
	afx_msg void On_Btn_HeartBeat();
	afx_msg void On_Btn_AutoHeartBeat();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void On_Btn_LogoutFastTrader();
	afx_msg void On_Btn_ExchangeDateTime();
	afx_msg void On_Btn_QryAllOrder();
	afx_msg void On_Btn_QryAllPosition();
	afx_msg void On_Btn_QryDepthMarket();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestFastTraderPipeView.cpp
inline CTestFastTraderPipeDoc* CTestFastTraderPipeView::GetDocument()
   { return (CTestFastTraderPipeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTFASTTRADERPIPEVIEW_H__75765DB1_11C6_4944_B869_9DC65FCB5DE0__INCLUDED_)
