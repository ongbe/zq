// ProfitChartDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "ProfitChartDialog.h"
#include "TabProbaChartDialog.h"

CLineChartCtrl CProfitChartDialog::m_ProfitChartPlot;
// CProfitChartDialog dialog

IMPLEMENT_DYNAMIC(CProfitChartDialog, CDialog)

CProfitChartDialog::CProfitChartDialog(CWnd* pParent /*=NULL*/)
	: CBKDialog(CProfitChartDialog::IDD, pParent)
    , m_bInitDialog(FALSE)
    , m_strVolatilityValue(_T(""))
{
    m_pTabProbaChartDialog = NULL;
}

CProfitChartDialog::~CProfitChartDialog()
{
}

void CProfitChartDialog::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_PROFITCHART, m_ProfitChartPlot);
    DDX_Control(pDX, IDC_COMBO_STYLE1, m_comboStyle1Ctrl);
    DDX_Control(pDX, IDC_COMBO_STYLE2, m_comboStyle2Ctrl);
    DDX_Control(pDX, IDC_EDIT_PERCENT, m_editPercentCtrl);
    DDX_Control(pDX, IDC_SPIN_PERCENT, m_spinPercentCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER_DEADTIME, m_dataTimeCtrl);
    DDX_Control(pDX, IDC_EDIT_REST, m_editRestCtrl);
    DDX_Control(pDX, IDC_SPIN_REST, m_spinRestCtrl);
    DDX_Control(pDX, IDC_CHECK_UPDATETIME, m_checkUpdateTime);
    DDX_Control(pDX, IDC_COMBO_UPDATETIME, m_comboUpdateTime);
}


BEGIN_MESSAGE_MAP(CProfitChartDialog, CBKDialog)
    ON_MESSAGE(UPDATE_MESSAGE,UpdateMsgHandler)
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_CHECK_UPDATETIME, &CProfitChartDialog::OnBnClickedCheckUpdatetime)
    ON_CBN_SELCHANGE(IDC_COMBO_STYLE2, &CProfitChartDialog::OnCbnSelchangeComboStyle2)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PERCENT, &CProfitChartDialog::OnDeltaposSpinPercent)
END_MESSAGE_MAP()



// CProfitChartDialog message handlers

BOOL CProfitChartDialog::OnInitDialog()
{
    CBKDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_ProfitChartPlot.SetParent(this);

    SetBitmap((m_ColorMode==Color_Light)?IDB_BITMAPTabProfitBK:IDB_BITMAPTabProfitBK_Deep);

    m_ProfitChartPlot.SetBKColorMode(m_ColorMode);
    m_ProfitChartPlot.SetChartType(eProfitChart);
    m_ProfitChartPlot.AddLine(PS_SOLID,RGB(255,0,0));//x axis
    m_ProfitChartPlot.AddLine(PS_SOLID,RGB(0,255,0));//y axis

    m_ProfitChartPlot.GetAxisX().m_bDraw = FALSE;
    m_ProfitChartPlot.GetAxisY().m_bDraw = FALSE;
    m_ProfitChartPlot.GetAxisY().m_color=RGB(255,255,255);//change y axis color
    m_ProfitChartPlot.GetAxisY().m_dMinValue=5000;
    m_ProfitChartPlot.GetAxisY().m_dMaxValue=6000;
    m_ProfitChartPlot.CalcLayout();


    m_comboUpdateTime.AddString("10");
    m_comboUpdateTime.AddString("15");
    m_comboUpdateTime.AddString("20");
    m_comboUpdateTime.AddString("25");
    m_comboUpdateTime.AddString("30");
    m_comboUpdateTime.SetCurSel(4);

    //
    m_comboStyle1Ctrl.AddString("到期盈亏分析");
    //m_comboStyle1Ctrl.AddString("未到期盈亏分析");
    //m_comboStyle1Ctrl.AddString("波动率分析");
    //m_comboStyle1Ctrl.AddString("到期日分析");
    m_comboStyle1Ctrl.SetCurSel(0);
    
    m_comboStyle2Ctrl.AddString("波动率");
    //m_comboStyle2Ctrl.AddString("隐波率");
    m_comboStyle2Ctrl.AddString("自订");
    m_comboStyle2Ctrl.SetCurSel(0);

    GetDlgItem(IDC_EDIT_PERCENT)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPIN_PERCENT)->EnableWindow(FALSE);

    m_bInitDialog = TRUE;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CProfitChartDialog::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    if(!m_bInitDialog)
        return;

    // TODO: Add your message handler code here
    CRect rect;
    GetClientRect(&rect);

    if(m_ProfitChartPlot.GetSafeHwnd())
        m_ProfitChartPlot.MoveWindow(rect.left,TOP_HIGHT,cx,cy - TOP_HIGHT - BOTTOM_HIGHT);

    int nTop,nLeft = rect.left + 10;    
    GetDlgItem(IDC_STATIC_NAME)->MoveWindow(nLeft, 8, 30, 20);
    nLeft = rect.Width() - 116;
    GetDlgItem(IDC_CHECK_UPDATETIME)->MoveWindow(nLeft, 6, 30, 20);
    nLeft += 33; 
    GetDlgItem(IDC_COMBO_UPDATETIME)->MoveWindow(nLeft, 6, 40, 20);
    nLeft += 43;
    GetDlgItem(IDC_STATIC_UPDATETIME)->MoveWindow(nLeft, 8, 40, 20);

    nLeft = rect.left+2;
    nTop = TOP_HIGHT + (cy - TOP_HIGHT - BOTTOM_HIGHT);
    GetDlgItem(IDC_COMBO_STYLE1)->MoveWindow(nLeft, nTop, 110, 20);
    nLeft = rect.right - 10;
    GetDlgItem(IDC_STATIC_PERCENT)->MoveWindow(nLeft, nTop + 2, 10, 20);
    nLeft -= 23;
    GetDlgItem(IDC_SPIN_PERCENT)->MoveWindow(nLeft, nTop, 20, 20);
    nLeft -= 35;
    GetDlgItem(IDC_EDIT_PERCENT)->MoveWindow(nLeft, nTop, 34, 20);
    nLeft -= 70;
    GetDlgItem(IDC_COMBO_STYLE2)->MoveWindow(nLeft, nTop, 70, 20);


    nLeft = rect.left+2;
    nTop += 8 + 20;
    GetDlgItem(IDC_STATIC_DEADTIME)->MoveWindow(nLeft, nTop + 2, 40, 20);
    nLeft += 40;
    GetDlgItem(IDC_DATETIMEPICKER_DEADTIME)->MoveWindow(nLeft, nTop, 120, 20);

    nLeft = rect.right - 33;
    GetDlgItem(IDC_SPIN_REST)->MoveWindow(nLeft, nTop, 20, 20);
    nLeft -= 35;
    GetDlgItem(IDC_EDIT_REST)->MoveWindow(nLeft, nTop, 34, 20);

    nLeft -= 15;
    GetDlgItem(IDC_STATIC_REST)->MoveWindow(nLeft, nTop + 2, 18, 20);
}

HBRUSH CProfitChartDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CBKDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here
    pDC->SetBkMode(TRANSPARENT); 

    switch(pWnd->GetDlgCtrlID()) 
    {      
    case IDC_STATIC_NAME:
    case IDC_CHECK_UPDATETIME:
    case IDC_STATIC_UPDATETIME:
    case IDC_STATIC_PERCENT:
    case IDC_STATIC_DEADTIME:
    case IDC_STATIC_REST:
        pDC->SetTextColor((m_ColorMode==Color_Light)?RGB(0,0,0):RGB(255,255,255)); 
        return (HBRUSH)GetStockObject(HOLLOW_BRUSH); 
    //case IDC_STATIC_VOLUME2:
    //    pDC->SetTextColor(RGB(0,155,0)); 
    //    return (HBRUSH)GetStockObject(HOLLOW_BRUSH); 
    default: 
        break; 
    } 
    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CProfitChartDialog::Update()
{
    m_ProfitChartPlot.Update();
}

LRESULT CProfitChartDialog::UpdateMsgHandler(WPARAM wParam,LPARAM lParam)
{
    m_ProfitChartPlot.InvalidateCross();
    return 0;
}
void CProfitChartDialog::OnBnClickedCheckUpdatetime()
{
    // TODO: Add your control notification handler code here
    HWND hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();  
    
    CString str;
    m_comboUpdateTime.GetLBText(m_comboUpdateTime.GetCurSel(),str);
    int nValue = _ttoi(str);

    int nState = m_checkUpdateTime.GetCheck()?1:0;
    ::SendMessage(hWnd,WM_USER_QUOT_UPDATE, (WPARAM)&nState,(LPARAM)&nValue);
}

void CProfitChartDialog::UpdateVolatility(CString strValue)
{
    m_strVolatilityValue = strValue;
    GetDlgItem(IDC_EDIT_PERCENT)->SetWindowText(m_strVolatilityValue);
}
void CProfitChartDialog::UpdateExpireDate(CTime tExpireDate,CString strRestDays)
{
    m_dataTimeCtrl.SetTime(&tExpireDate);
    m_editRestCtrl.SetWindowText(strRestDays);
}

void CProfitChartDialog::OnCbnSelchangeComboStyle2()
{
    // TODO: Add your control notification handler code here
    int nSel = m_comboStyle2Ctrl.GetCurSel();
    if(nSel==1)
    {
        GetDlgItem(IDC_SPIN_PERCENT)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_PERCENT)->EnableWindow(TRUE);

        CString sValue;
        m_editPercentCtrl.GetWindowText(sValue);
        CDataInfo::NewInstance()->SetVolatilityTestFlag(true); 
        CDataInfo::NewInstance()->SetVolatilityTest(atof(sValue)/100.0);
    }
    else
    {     
        GetDlgItem(IDC_EDIT_PERCENT)->SetWindowText(m_strVolatilityValue);
        GetDlgItem(IDC_EDIT_PERCENT)->EnableWindow(FALSE);
        GetDlgItem(IDC_SPIN_PERCENT)->EnableWindow(FALSE);

        CDataInfo::NewInstance()->SetVolatilityTestFlag(false);
    }

    HWND hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();

    CDataInfo::NewInstance()->Calculate();
    ::SendMessage(hwnd,WM_VOLATILITY_RESET,0,0);

    GetDlgItem(IDC_STATIC_REST)->SetFocus();
}

void CProfitChartDialog::OnDeltaposSpinPercent(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    double dVolume = 0.0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editPercentCtrl.GetLine(0, strQty, 63);
    dVolume = atof(strQty);

    dVolume -= pNMUpDown->iDelta * 0.1;

    sprintf_s(strQty, "%.1f", (dVolume<0.0)?0:dVolume);
    m_editPercentCtrl.SetSel(0, -1);
    m_editPercentCtrl.ReplaceSel(strQty);

    Sleep(20);
    //recaculate
    CDataInfo::NewInstance()->SetVolatilityTest(dVolume/100.0);
    CDataInfo::NewInstance()->Calculate();
    HWND hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();  
    ::SendMessage(hwnd,WM_VOLATILITY_RESET,0,0);
    
    *pResult = 0;
}
BOOL CProfitChartDialog::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if( pMsg->message ==WM_KEYDOWN && pMsg->wParam==VK_RETURN  )
    {
        CString sValue;
        m_editPercentCtrl.GetWindowText(sValue);

        //recaculate
        CDataInfo::NewInstance()->SetVolatilityTest(atof(sValue.GetBuffer(0))/100.0);
        CDataInfo::NewInstance()->Calculate();
        HWND hwnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();  
        ::SendMessage(hwnd,WM_VOLATILITY_RESET,0,0);


        m_spinPercentCtrl.SetFocus();
        return true;
    } 

    return CBKDialog::PreTranslateMessage(pMsg);
}
void CProfitChartDialog::UpdataColorMode(BK_COLOR_MODE eMode)
{
    SetBitmap((eMode==Color_Light)?IDB_BITMAPTabProfitBK:IDB_BITMAPTabProfitBK_Deep);
    m_ProfitChartPlot.SetBKColorMode(eMode);   
    m_ColorMode = eMode;

    m_ProfitChartPlot.Invalidate();
    m_ProfitChartPlot.UpdateWindow();

}
