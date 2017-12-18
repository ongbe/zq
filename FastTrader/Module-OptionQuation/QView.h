#pragma once
#include "KLineView.h"
#include "TLineView.h"

// CQView dialog

class CQView : public CDialog
{
	DECLARE_DYNAMIC(CQView)

public:
	CQView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQView();

// Dialog Data
	enum { IDD = IDD_DIALOG_QView };
	CKLineView    m_KlineView;
	CTLineView    m_TlineView;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnMessageLDBClick(WPARAM wP,LPARAM lP);
	LRESULT OnMessageReturn(WPARAM wP,LPARAM lP);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnMessageChangeView(WPARAM wP,LPARAM lP);
	LRESULT OnMessageChangeTimeSpan(WPARAM wP,LPARAM lP);
	LRESULT OnMessageIndexExpress(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageIndexConfig(WPARAM wP,LPARAM lP);	
};
