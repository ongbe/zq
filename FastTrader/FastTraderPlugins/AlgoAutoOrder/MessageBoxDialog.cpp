// MessageBoxDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "MessageBoxDialog.h"


// CMessageBoxDialog dialog

IMPLEMENT_DYNAMIC(CMessageBoxDialog, CDialog)

CMessageBoxDialog::CMessageBoxDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageBoxDialog::IDD, pParent)
{

	EnableAutomation();

}

CMessageBoxDialog::CMessageBoxDialog(CString strText)
{
    m_Text = strText;
}

CMessageBoxDialog::~CMessageBoxDialog()
{
}

void CMessageBoxDialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CMessageBoxDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_SHOW, m_Show);
}


BEGIN_MESSAGE_MAP(CMessageBoxDialog, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMessageBoxDialog, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IMessageBoxDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {8DF26C84-C002-4379-9800-DDA31912580D}
static const IID IID_IMessageBoxDialog =
{ 0x8DF26C84, 0xC002, 0x4379, { 0x98, 0x0, 0xDD, 0xA3, 0x19, 0x12, 0x58, 0xD } };

BEGIN_INTERFACE_MAP(CMessageBoxDialog, CDialog)
	INTERFACE_PART(CMessageBoxDialog, IID_IMessageBoxDialog, Dispatch)
END_INTERFACE_MAP()


// CMessageBoxDialog message handlers

BOOL CMessageBoxDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    SetWindowText("");
    m_Show.SetWindowText(m_Text);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
