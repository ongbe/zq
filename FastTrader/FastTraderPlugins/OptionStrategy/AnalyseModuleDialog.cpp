// AnalyseModuleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "AnalyseModuleDialog.h"
#include "OptionStrategyDlg.h"

#define RGB_AVERAGE(a, b)   ( ((((a) ^ (b)) & 0xfffefefeL) >> 1) + ((a) & (b)) )

// CAnalyseModuleDialog dialog

IMPLEMENT_DYNAMIC(CAnalyseModuleDialog, CDialog)

LRESULT CAnalyseModuleDialog::QuotUpdateMsgHandler(WPARAM wParam,LPARAM lParam)
{
    CDataInfo::NewInstance()->Calculate();
    UpdateLineChartCtrl();
    return 0;
}

LRESULT CAnalyseModuleDialog::OnVolatilityUpdate(WPARAM wParam,LPARAM lParam)
{
    CString strVolatility;
    strVolatility.Format("%0.1f",CDataInfo::NewInstance()->GetVolatility()*100);    
    m_ProfitChartDialog.UpdateVolatility(strVolatility);
    return 1;
}

LRESULT CAnalyseModuleDialog::OnExpireDateUpdate(WPARAM wParam,LPARAM lParam)
{
    CTime tExpireDate;
    string strRestDays;
    CDataInfo::NewInstance()->GetProfitDlgParams(tExpireDate,strRestDays);
    m_ProfitChartDialog.UpdateExpireDate(tExpireDate,(CString)strRestDays.data());
    return 1;
}

CAnalyseModuleDialog::CAnalyseModuleDialog(CWnd* pParent /*=NULL*/)
	: CBKDialog(CAnalyseModuleDialog::IDD, pParent)
{

}

CAnalyseModuleDialog::~CAnalyseModuleDialog()
{
}

void CAnalyseModuleDialog::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CUSTOM_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CAnalyseModuleDialog, CBKDialog)
    ON_WM_SIZE()
    ON_MESSAGE(WM_USER_QUOT_UPDATE,QuotUpdateMsgHandler)
    ON_MESSAGE(WM_USER_VOLATILITY_UPDATE,OnVolatilityUpdate)
    ON_MESSAGE(WM_USER_EXPIREDATE_UPDATE,OnExpireDateUpdate)
END_MESSAGE_MAP()


// CAnalyseModuleDialog message handlers

BOOL CAnalyseModuleDialog::OnInitDialog()
{
    CBKDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    //set background
    SetBitmap(IDB_BITMAPOptionStrategyBK);

    m_TabCtrl.ModifyStyle(0, WS_VISIBLE | CTCS_TOP | CTCS_AUTOHIDEBUTTONS | CTCS_CONTAINER);	

    //setup tab control colors
    TabItemColors ticColors;

    //get colors..
    m_TabCtrl.GetColors(&ticColors);

    //modify them..
    ticColors.crBkgActive=(m_ColorMode==Color_Light)?RGB(13,49,89):RGB(13,49,89);//RGB_AVERAGE(GetSysColor(COLOR_ACTIVECAPTION), RGB(255, 255, 255));
    ticColors.crTxtActive=RGB(255, 255, 255);
    ticColors.crBkgInactive=(m_ColorMode==Color_Light)?RGB(31,119,217):RGB(31,119,217);//GetSysColor(COLOR_ACTIVECAPTION);
    ticColors.crTxtInactive=RGB(255, 255, 255);//RGB_AVERAGE(GetSysColor(COLOR_ACTIVECAPTION), GetSysColor(COLOR_CAPTIONTEXT));
    ticColors.crWndBkg=RGB(226,226,226);//GetSysColor(COLOR_ACTIVECAPTION);

    //..and write back
    m_TabCtrl.SetColors(&ticColors);

    //create tabs and setup page colors

    //create the ProbaChart dlg
    m_TabProbaChartDialog=new CTabProbaChartDialog();
    m_TabProbaChartDialog->Create(CTabProbaChartDialog::IDD, this);
    m_TabProbaChartDialog->SetBitmap(IDB_BITMAPOptionStrategyBK);
    //..setup colors
    //m_pTabDialog1->SetBkgColor(RGB_AVERAGE(GetSysColor(COLOR_ACTIVECAPTION), RGB(255, 255, 255)));
    //m_pTabDialog1->SetFrgColor(RGB(0, 0, 0));
    //..add tab to tab control & dialog to container with the same call
    m_TabCtrl.AddDialog(0, _T("概率图"), m_TabProbaChartDialog);
    m_TabProbaChartDialog->SetParent(this);
    

    //create the TradeAnalyse dlg
    m_TabTradeAnalyseDialog=new CTabTradeAnalyseDialog();
    m_TabTradeAnalyseDialog->Create(CTabTradeAnalyseDialog::IDD, this);
    m_TabTradeAnalyseDialog->SetBitmap(IDB_BITMAPOptionStrategyBK);
    //..setup colors
    //m_pTabDialog2->SetBkgColor(RGB_AVERAGE(GetSysColor(COLOR_ACTIVECAPTION), RGB(255, 255, 255)));
    //m_pTabDialog2->SetFrgColor(RGB(0, 0, 0));
    //..add tab to tab control & dialog to container with the same call
    //m_TabCtrl.AddDialog(1, _T("交易情景分析"), m_TabTradeAnalyseDialog);


    m_ProfitChartDialog.Create(IDD_PROFIT_DIALOG,this);
    m_ProfitChartDialog.ShowWindow(SW_SHOW);
    m_ProfitChartDialog.SetParent(this);


    //保留对方窗口
    m_TabProbaChartDialog->m_pProfitChartDialog = &m_ProfitChartDialog;
    m_ProfitChartDialog.m_pTabProbaChartDialog = m_TabProbaChartDialog;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnalyseModuleDialog::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    CRect rc;
    GetClientRect(&rc);

    if(m_TabCtrl.GetSafeHwnd())
        m_TabCtrl.MoveWindow(rc.left,rc.top,rc.Width(),rc.Height()/2-2);

    if(m_ProfitChartDialog.GetSafeHwnd())
        m_ProfitChartDialog.MoveWindow(rc.left,rc.Height()/2+2,rc.Width(),rc.Height()/2);
}

void CAnalyseModuleDialog::UpdateLineChartCtrl()
{    
    m_ProfitChartDialog.Update();
    m_TabProbaChartDialog->Update();
}

void CAnalyseModuleDialog::UpdataColorMode(BK_COLOR_MODE eMode)
{
    TabItemColors ticColors;

    //get colors..
    m_TabCtrl.GetColors(&ticColors);

    //modify them..
    ticColors.crBkgActive=(eMode==Color_Light)?RGB(13,49,89):RGB(13,49,89);//RGB_AVERAGE(GetSysColor(COLOR_ACTIVECAPTION), RGB(255, 255, 255));
    ticColors.crTxtActive=RGB(255, 255, 255);
    ticColors.crBkgInactive=(eMode==Color_Light)?RGB(31,119,217):RGB(31,119,217);//GetSysColor(COLOR_ACTIVECAPTION);
    ticColors.crTxtInactive=RGB(255, 255, 255);//RGB_AVERAGE(GetSysColor(COLOR_ACTIVECAPTION), GetSysColor(COLOR_CAPTIONTEXT));
    ticColors.crWndBkg=RGB(226,226,226);//GetSysColor(COLOR_ACTIVECAPTION);

    //..and write back
    m_TabCtrl.SetColors(&ticColors);

    m_TabProbaChartDialog->UpdataColorMode(eMode);
    m_ProfitChartDialog.UpdataColorMode(eMode);

    m_TabCtrl.Invalidate();
    m_TabCtrl.UpdateWindow();
}
