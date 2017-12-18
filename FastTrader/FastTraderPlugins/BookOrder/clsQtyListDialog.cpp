// clsQtyListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "clsQtyListDialog.h"


// CQtyListDialog dialog

IMPLEMENT_DYNAMIC(CQtyListDialog, CDialog)

CQtyListDialog::CQtyListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQtyListDialog::IDD, pParent)
{
	m_pButtonGroup = NULL;
	m_poOutputEditQty = NULL;
	m_nQtyGroup[0]=1;
	m_nQtyGroup[1]=2;
	m_nQtyGroup[2]=3;
	m_nQtyGroup[3]=4;
	m_nQtyGroup[4]=5;
	m_nQtyGroup[5]=10;
	m_nQtyGroup[6]=20;
	m_nQtyGroup[7]=30;
	m_nQtyGroup[8]=50;
}

CQtyListDialog::~CQtyListDialog()
{
	for(int i=0; i<9; i++) {
		if(m_pButtonGroup[i]!=NULL)
		{
			delete m_pButtonGroup[i];
			m_pButtonGroup[i] = NULL;
		}
	}
	if(m_pButtonGroup!=NULL)
	{
		delete m_pButtonGroup;
		m_pButtonGroup = NULL;
	}
}

void CQtyListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQtyListDialog, CDialog)
	ON_BN_CLICKED(ID_QTY_GROUP, &CQtyListDialog::OnBnClickedQtyGroup)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CQtyListDialog message handlers

void CQtyListDialog::ResetQtyList(int* pQty, int nCount)
{
	if(nCount!=9 || pQty==NULL)
		return;

	CString strText;
	for(int i=0; i<9; i++)
	{
		m_nQtyGroup[i]=pQty[i];
		strText.Format("%d", pQty[i]);
		m_pButtonGroup[i]->SetWindowText(strText);
	}
}

BOOL CQtyListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	int nBWidth = 36;//46;

	//if(g_pPlatform&&g_pPlatform->GetPlatformId()==PID_PATSAPI) {
	//	nBWidth = 72;
	//}

	CString strQty;
	m_pButtonGroup = new CButton*[conButtonMaxNum];
	ZeroMemory(m_pButtonGroup, sizeof(CButton*)*conButtonMaxNum);
	for(int i=0; i<conButtonMaxNum; i++) {
		strQty.Format("%u", m_nQtyGroup[i]);
		m_pButtonGroup[i] = new CButton();
		m_pButtonGroup[i]->Create(strQty, 
				BS_NOTIFY, 
				CRect((i%3)*nBWidth, (i/3)*32, (i%3+1)*nBWidth, (i/3+1)*32), 
				this, ID_QTY_GROUP);
		m_pButtonGroup[i]->SetFont(GetFont());
		m_pButtonGroup[i]->ShowWindow(SW_SHOW);
	}
	CRect rect;
	GetWindowRect(&rect);
	rect.right = rect.left + nBWidth*3 + 2;
	rect.bottom = rect.top + 32*(conButtonMaxNum/3) + 2;
	MoveWindow(&rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CQtyListDialog::OnBnClickedQtyGroup()
{
	// TODO: Add your control notification handler code here
	CWnd* pFocus = NULL;
	BOOL bFound = FALSE;
	int nCurrIndex = -1;
	for(int i=0; i<conButtonMaxNum; i++)
	{
		pFocus = GetFocus();
		if(m_pButtonGroup[i]==pFocus)
		{
			nCurrIndex = i;
			bFound = TRUE;
			break;
		}
	}
	if(!bFound)
		return;

	CString strText;
	m_pButtonGroup[nCurrIndex]->GetWindowText(strText);
	if(m_poOutputEditQty!=NULL)
	{
		m_poOutputEditQty->SetSel(0, -1);
		m_poOutputEditQty->ReplaceSel(strText);
	}
	ShowWindow(SW_HIDE);
}

void CQtyListDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(!bShow)
		m_poOutputEditQty = NULL;
}
