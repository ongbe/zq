#pragma once
#include "afxwin.h"


// CMessageBoxDialog dialog

class CMessageBoxDialog : public CDialog
{
	DECLARE_DYNAMIC(CMessageBoxDialog)

public:
	CMessageBoxDialog(CWnd* pParent = NULL);   // standard constructor
    CMessageBoxDialog(CString strText);
	virtual ~CMessageBoxDialog();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_MESSAGEBOXDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
    CStatic m_Show;
public:
    virtual BOOL OnInitDialog();
    CString m_Text;
};
