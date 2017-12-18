// Tabpage2_Chart.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoTradingNo1.h"
#include "Tabpage2_Chart.h"


// CTabpage2_Chart dialog

IMPLEMENT_DYNAMIC(CTabpage2_Chart, CDialog)

CTabpage2_Chart::CTabpage2_Chart(CWnd* pParent /*=NULL*/)
	: CDialog(CTabpage2_Chart::IDD, pParent)
{

}

CTabpage2_Chart::~CTabpage2_Chart()
{
}

void CTabpage2_Chart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_ChartCtl);
}


BEGIN_MESSAGE_MAP(CTabpage2_Chart, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTabpage2_Chart message handlers

void CTabpage2_Chart::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_ChartCtl.GetSafeHwnd()!=NULL)
		m_ChartCtl.MoveWindow(0,0,cx,cy);
}

BOOL CTabpage2_Chart::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ChartCtl.CreateChildren();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


