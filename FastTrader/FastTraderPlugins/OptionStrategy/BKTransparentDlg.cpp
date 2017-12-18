#include "stdafx.h"
#include "OptionStrategy.h"
#include "BKTransparentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBKTransparentDlg::CBKTransparentDlg(CWnd* pParent /*=NULL*/)
	: CBKDialog(CBKTransparentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBKTransparentDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CBKTransparentDlg::DoDataExchange(CDataExchange* pDX)
{
	CBKDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBKTransparentDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	// Make our buttons a CButtonST buttons
	DDX_Control(pDX, IDC_BTNLOGOFF, m_btnLogOff);
	DDX_Control(pDX, IDC_BTNWORKGROUP, m_btnWorkgroup);
	DDX_Control(pDX, IDC_BTNOPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTNSEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTNEXPLORER, m_btnExplorer);
	DDX_Control(pDX, IDC_BTNHELP, m_btnHelp);
	DDX_Control(pDX, IDC_BTNABOUT, m_btnAbout);
	DDX_Control(pDX, IDC_BTNCANNIBAL, m_btnCannibal);
	DDX_Control(pDX, IDC_CHECK, m_chkCheckbox);
}

BEGIN_MESSAGE_MAP(CBKTransparentDlg, CBKDialog)
	//{{AFX_MSG_MAP(CBKTransparentDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBKTransparentDlg::OnInitDialog() 
{
	CBKDialog::OnInitDialog();
	
	SetBitmap(IDB_SKY);

	// Log off button
	m_btnLogOff.SetIcon(IDI_BUTTERFLY, 48, 48);
	m_btnLogOff.DrawTransparent();
	// Workgroup button
	m_btnWorkgroup.SetIcon(IDI_WORKGROUP);
	m_btnWorkgroup.DrawTransparent();
	// Toolbar buttons
	m_btnOpen.SetIcon(IDI_OPEN, (int)BTNST_AUTO_DARKER);
	m_btnOpen.SetTooltipText(_T("Open"));
	m_btnOpen.DrawTransparent();
	m_btnSearch.SetIcon(IDI_SEARCH1, (int)BTNST_AUTO_DARKER);
	m_btnSearch.SetTooltipText(_T("Search"));
	m_btnSearch.DrawTransparent();
	m_btnExplorer.SetIcon(IDI_EXPLORER, (int)BTNST_AUTO_DARKER);
	m_btnExplorer.SetTooltipText(_T("Internet explorer"));
	m_btnExplorer.DrawTransparent();
	m_btnHelp.SetIcon(IDI_HELP, (int)BTNST_AUTO_DARKER);
	m_btnHelp.SetTooltipText(_T("Help"));
	m_btnHelp.DrawTransparent();
	m_btnAbout.SetIcon(IDI_ABOUT, (int)BTNST_AUTO_DARKER);
	m_btnAbout.SetTooltipText(_T("About"));
	m_btnAbout.DrawTransparent();
	// Cannibal button
	m_btnCannibal.SetBitmaps(IDB_CANNIBAL, RGB(0, 255, 0), (int)BTNST_AUTO_DARKER);
	m_btnCannibal.DrawTransparent();
	// Checkbox
	m_chkCheckbox.SetIcon(IDI_LEDON, IDI_LEDOFF);
	m_chkCheckbox.DrawTransparent();

	return TRUE;
} // End of OnInitDialog

void CBKTransparentDlg::OnOK()
{
} // End of OnOK

void CBKTransparentDlg::OnCancel()
{
} // End of OnCancel

void CBKTransparentDlg::OnPostEraseBkgnd(CDC* pDC)
{
	m_btnLogOff.SetBk(pDC);
	m_btnWorkgroup.SetBk(pDC);
	m_btnOpen.SetBk(pDC);
	m_btnSearch.SetBk(pDC);
	m_btnExplorer.SetBk(pDC);
	m_btnHelp.SetBk(pDC);
	m_btnAbout.SetBk(pDC);
	m_btnCannibal.SetBk(pDC);
	m_chkCheckbox.SetBk(pDC);
} // End of OnPostEraseBkgnd
