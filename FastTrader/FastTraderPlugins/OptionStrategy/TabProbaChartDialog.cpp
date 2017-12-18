// ProbaChartDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "TabProbaChartDialog.h"
#include "ProfitChartDialog.h"

CLineChartCtrl CTabProbaChartDialog::m_ProbaChartPlot;
// CTabProbaChartDialog dialog

IMPLEMENT_DYNAMIC(CTabProbaChartDialog, CDialog)

CTabProbaChartDialog::CTabProbaChartDialog(CWnd* pParent /*=NULL*/)
	: CBKDialog(CTabProbaChartDialog::IDD, pParent)
{
    m_pProfitChartDialog = NULL;
}

CTabProbaChartDialog::~CTabProbaChartDialog()
{
}

void CTabProbaChartDialog::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_PROBACHART, m_ProbaChartPlot);
}


BEGIN_MESSAGE_MAP(CTabProbaChartDialog, CBKDialog)
    ON_MESSAGE(UPDATE_MESSAGE,UpdateMsgHandler)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CTabProbaChartDialog message handlers

BOOL CTabProbaChartDialog::OnInitDialog()
{
    CBKDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_ProbaChartPlot.SetParent(this);

    m_ProbaChartPlot.SetBKColorMode(m_ColorMode);
    m_ProbaChartPlot.SetChartType(eProbaChart);
    m_ProbaChartPlot.AddLine(PS_SOLID,RGB(255,0,0));
    m_ProbaChartPlot.AddLine(PS_SOLID,RGB(0,255,0));

    m_ProbaChartPlot.GetAxisX().m_bDraw = FALSE;
    m_ProbaChartPlot.GetAxisY().m_color=RGB(255,255,255);
    m_ProbaChartPlot.GetAxisY().m_dMinValue=5000;
    m_ProbaChartPlot.GetAxisY().m_dMaxValue=6000;
    m_ProbaChartPlot.CalcLayout();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabProbaChartDialog::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    CRect rect;
    GetClientRect(&rect);
    //rect.DeflateRect(10,20);

    if(m_ProbaChartPlot.GetSafeHwnd())
        m_ProbaChartPlot.MoveWindow(rect.left,rect.top,cx,cy);
}

void CTabProbaChartDialog::Update()
{
    m_ProbaChartPlot.Update();
}

LRESULT CTabProbaChartDialog::UpdateMsgHandler(WPARAM wParam,LPARAM lParam)
{
    m_ProbaChartPlot.InvalidateCross();
    return 0;
}
void CTabProbaChartDialog::UpdataColorMode(BK_COLOR_MODE eMode)
{
    m_ProbaChartPlot.SetBKColorMode(m_ColorMode);
    m_ProbaChartPlot.Invalidate();
    m_ProbaChartPlot.UpdateWindow();
}
