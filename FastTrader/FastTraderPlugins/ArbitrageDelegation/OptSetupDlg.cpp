// OptSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArbitrageDelegation.h"
#include "ArbitrageDelegationDlg.h"
#include "OptSetupDlg.h"
#include "Tools.h"

#ifdef _DEBUG
#pragma comment(lib,"PlusinAPID.lib")
#else
#pragma comment(lib,"PlusinAPI.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib,"ToolsD.lib")
#else
#pragma comment(lib,"Tools.lib")
#endif

// COptSetupDlg dialog

IMPLEMENT_DYNAMIC(COptSetupDlg, CDialog)

COptSetupDlg::COptSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptSetupDlg::IDD, pParent)
{

	EnableAutomation();
    
    m_pParamData=NULL;

    m_bInst1InputError = false;
    m_bInst2InputError = false;
}

COptSetupDlg::COptSetupDlg(CParamData* pData,set<string>& setInstrumentID,ARBITRAGE_TYPE type,CWnd* pParent /*=NULL*/)
    : CDialog(COptSetupDlg::IDD, pParent)
{
    EnableAutomation();

    m_pParamData = pData;    
    m_nArbitrageType = type;
    m_setInstrumentID = setInstrumentID;
    if(!m_setInstrumentID.empty())
    {
        for(set<string>::iterator it=m_setInstrumentID.begin();it!=m_setInstrumentID.end();it++)
        {
            string szInst = *it;
            transform(szInst.begin(), szInst.end(), szInst.begin(), (int (*)(int))tolower); 
            m_mapInputKey2RightInst.insert(make_pair(szInst,*it));
        }
    }

    m_bInst1InputError = false;
    m_bInst2InputError = false;
}

COptSetupDlg::~COptSetupDlg()
{
}

void COptSetupDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void COptSetupDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_OFFSET, m_comboOffsetIDCtrl);
    DDX_Control(pDX, IDC_EDIT_TRIGGER, m_editTriggerCtrl);
    DDX_Control(pDX, IDC_EDIT_DEVIATE, m_editDeviateCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER_VALIDATE, m_datetimeValidateCtrl);
    DDX_Control(pDX, IDC_COMBO_DIRECTION1, m_comboDirection1Ctrl);
    DDX_Control(pDX, IDC_COMBO_DIRECTION2, m_comboDirection2Ctrl);
    DDX_Control(pDX, IDC_EDIT_COUNT1, m_editCount1Ctrl);
    DDX_Control(pDX, IDC_EDIT_COUNT2, m_editCount2Ctrl);
    DDX_Control(pDX, IDC_EDIT_DELEGATE1, m_editDelegate1Ctrl);
    DDX_Control(pDX, IDC_EDIT_DELEGATE2, m_editDelegate2Ctrl);
    DDX_Control(pDX, IDC_EDIT_UPPERPRICE1, m_editUpperPrice1Ctrl);
    DDX_Control(pDX, IDC_EDIT_UPPERPRICE2, m_editUpperPrice2Ctrl);
    DDX_Control(pDX, IDC_EDIT_LOWPRICE1, m_editLowestPrice1Ctrl);
    DDX_Control(pDX, IDC_EDIT_LOWPRICE2, m_editLowestPrice2Ctrl); 
    DDX_Control(pDX, IDC_COMBO_ARBITRAGETYPE, m_comboArbitrageTypeCtrl);
    DDX_Control(pDX, IDC_COMBO_INST1, m_combInstrument1Ctrl);
    DDX_Control(pDX, IDC_COMBO_INST2, m_combInstrument2Ctrl);
}


BEGIN_MESSAGE_MAP(COptSetupDlg, CDialog)
    ON_BN_CLICKED(IDCANCEL, &COptSetupDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &COptSetupDlg::OnBnClickedOk)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TRIGGER, &COptSetupDlg::OnDeltaposSpinTrigger)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DEVIATE, &COptSetupDlg::OnDeltaposSpinDeviate)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELEGATE1, &COptSetupDlg::OnDeltaposSpinDelegate1)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELEGATE2, &COptSetupDlg::OnDeltaposSpinDelegate2)
    ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION1, &COptSetupDlg::OnCbnSelchangeComboDirection1)
    ON_EN_CHANGE(IDC_EDIT_COUNT1, &COptSetupDlg::OnEnChangeEditCount1)
    ON_CBN_SELCHANGE(IDC_COMBO_OFFSET, &COptSetupDlg::OnCbnSelchangeComboOffset)
    ON_CBN_SELCHANGE(IDC_COMBO_ARBITRAGETYPE, &COptSetupDlg::OnCbnSelchangeComboArbitragetype)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_CBN_SELCHANGE(IDC_COMBO_INST1, &COptSetupDlg::OnCbnSelchangeComboInst1)
    ON_CBN_EDITCHANGE(IDC_COMBO_INST1, &COptSetupDlg::OnCbnEditchangeComboInst1)
    ON_CBN_SELCHANGE(IDC_COMBO_INST2, &COptSetupDlg::OnCbnSelchangeComboInst2)
    ON_CBN_EDITCHANGE(IDC_COMBO_INST2, &COptSetupDlg::OnCbnEditchangeComboInst2)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COptSetupDlg, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_ICOptSetupDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {467D648C-6654-4B08-BA49-A7C483D99C83}
static const IID IID_ICOptSetupDlg =
{ 0x467D648C, 0x6654, 0x4B08, { 0xBA, 0x49, 0xA7, 0xC4, 0x83, 0xD9, 0x9C, 0x83 } };

BEGIN_INTERFACE_MAP(COptSetupDlg, CDialog)
	INTERFACE_PART(COptSetupDlg, IID_ICOptSetupDlg, Dispatch)
END_INTERFACE_MAP()


// COptSetupDlg message handlers
BOOL COptSetupDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_comboOffsetIDCtrl.AddString("开仓");
    m_comboOffsetIDCtrl.AddString("平仓");
    m_comboOffsetIDCtrl.AddString("平今");
    m_comboOffsetIDCtrl.SetCurSel(0);

    m_comboArbitrageTypeCtrl.AddString("价差缩小套利");
    m_comboArbitrageTypeCtrl.AddString("价差扩大套利");
    m_comboArbitrageTypeCtrl.SetCurSel(1);

    m_comboDirection1Ctrl.AddString("买");
    m_comboDirection1Ctrl.AddString("卖");

    m_comboDirection2Ctrl.AddString("买");
    m_comboDirection2Ctrl.AddString("卖");

    for (set<string>::iterator it=m_setInstrumentID.begin();it!=m_setInstrumentID.end();it++)
    {
        string szInst = *it;
        m_combInstrument1Ctrl.AddString(szInst.c_str());
        m_combInstrument2Ctrl.AddString(szInst.c_str());
    }
    m_combInstrument1Ctrl.SetCurSel(-1);
    m_combInstrument2Ctrl.SetCurSel(-1);
  
    SetSetupParamDataToWnd();

    if(m_nArbitrageType==ARBITRAGE_MODIFY)
        EnableCtrls(false);
    else if(m_nArbitrageType==ARBITRAGE_ADD)
        EnableCtrls(true);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void COptSetupDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    if(!Verify())
        return;

    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_SETUPDLG_CLOSE,0,0);

    CreateSetupParamData();

    OnOK();
}

void COptSetupDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here

    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_SETUPDLG_CLOSE,0,0);

    OnCancel();
}

bool COptSetupDlg::Verify()
{
    if (CTools::GetStringFromCEdit(m_editTriggerCtrl).empty() ||  CTools::compare_double(CTools::GetDoubleFromCEdit(m_editTriggerCtrl),0.000000f)<=0)
    {
        ::MessageBox(NULL, "触发价差不能为空或零或负值，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }   

    CTime time,time2;

    time = CTime::GetCurrentTime();  
    m_datetimeValidateCtrl.GetTime(time2);

    int nYear = time.GetYear();
    int nMonth = time.GetMonth();
    int nDay = time.GetDay();

    int nnYear = time2.GetYear();
    int nnMonth = time2.GetMonth();
    int nnDay = time2.GetDay();

    if(nnYear<nYear ||
        (nnYear == nYear && nnMonth<nMonth) ||
        (nnYear == nYear && nnMonth==nMonth && nnDay<nDay))
    {
        ::MessageBox(NULL, "有效期不能小于今日，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    CString str;
    m_combInstrument1Ctrl.GetWindowText(str);
    if (str.GetLength()==0)
    {
        ::MessageBox(NULL, "合约1不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    m_combInstrument2Ctrl.GetWindowText(str);
    if (str.GetLength()==0)
    {
        ::MessageBox(NULL, "合约2不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editCount1Ctrl).empty()||atoi(CTools::GetStringFromCEdit(m_editCount1Ctrl).c_str())<=0)
    {
        ::MessageBox(NULL, "委托数量1不能为空或负数，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }   

    if (CTools::GetStringFromCEdit(m_editCount2Ctrl).empty()||atoi(CTools::GetStringFromCEdit(m_editCount2Ctrl).c_str())<=0)
    {
        ::MessageBox(NULL, "委托数量2不能为空或负数，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editDelegate1Ctrl).empty())
    {
        ::MessageBox(NULL, "委托价差1不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editDelegate2Ctrl).empty())
    {
        ::MessageBox(NULL, "委托价差2不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    string szInstNameUpper1 = CTools::GetStringFromCComboBox(m_combInstrument1Ctrl);
    string szInstNameUpper2 = CTools::GetStringFromCComboBox(m_combInstrument2Ctrl);

    if(m_bInst1InputError)
    {
        ::MessageBox(NULL, "不是合法的合约1，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false; 
    }

    if(m_bInst2InputError)
    {
        ::MessageBox(NULL, "不是合法的合约2，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if(szInstNameUpper1==szInstNameUpper2)
    {
        ::MessageBox(NULL, "合约不能重复", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    return true;
}


void COptSetupDlg::CreateSetupParamData()
{
    if(m_pParamData==NULL)
        return;
   
    m_pParamData->Param_RevokeFlag = false;
    m_pParamData->Param_TriggerFlag = false;
    if(CTools::GetStringFromCComboBox(m_comboOffsetIDCtrl)=="开仓") m_pParamData->Param_OffsetID=0;
    else if(CTools::GetStringFromCComboBox(m_comboOffsetIDCtrl)=="平仓") m_pParamData->Param_OffsetID=1;
    else m_pParamData->Param_OffsetID=2;	//平今
    if(CTools::GetStringFromCComboBox(m_comboArbitrageTypeCtrl)=="价差缩小套利") m_pParamData->Param_ArbitrageType=0;
    else if(CTools::GetStringFromCComboBox(m_comboArbitrageTypeCtrl)=="价差扩大套利") m_pParamData->Param_ArbitrageType=1;
    m_pParamData->Param_TriggerPrice=CTools::GetDoubleFromCEdit(m_editTriggerCtrl);
    m_pParamData->Param_DeviatePrice=CTools::GetDoubleFromCEdit(m_editDeviateCtrl);
    m_pParamData->Param_ValidateDay=CTools::GetCTimeFromCDateTimeCtrl(m_datetimeValidateCtrl);
    m_pParamData->Param_ValidateDayText = (string)(m_pParamData->Param_ValidateDay.Format("%Y-%m-%d"));

    //合约1
    m_pParamData->Order_Instrument1=CTools::GetStringFromCComboBox(m_combInstrument1Ctrl);
    m_pParamData->Order_Direction1 = CTools::GetStringFromCComboBox(m_comboDirection1Ctrl)=="买"?0:1;
    m_pParamData->Order_Count1=(int)CTools::GetDoubleFromCEdit(m_editCount1Ctrl);
    m_pParamData->Order_Price1=0.0f;
    m_pParamData->Order_DelegatePrice1=CTools::GetDoubleFromCEdit(m_editDelegate1Ctrl);
    m_pParamData->Order_UpperPrice1=CTools::GetDoubleFromCEdit(m_editUpperPrice1Ctrl);
    m_pParamData->Order_LowestPrice1=CTools::GetDoubleFromCEdit(m_editLowestPrice1Ctrl);

    //合约2
    m_pParamData->Order_Instrument2=CTools::GetStringFromCComboBox(m_combInstrument2Ctrl);
    m_pParamData->Order_Direction2 = CTools::GetStringFromCComboBox(m_comboDirection2Ctrl)=="买"?0:1;
    m_pParamData->Order_Count2=(int)CTools::GetDoubleFromCEdit(m_editCount2Ctrl);
    m_pParamData->Order_Price2=0.0f;
    m_pParamData->Order_DelegatePrice2=CTools::GetDoubleFromCEdit(m_editDelegate2Ctrl);
    m_pParamData->Order_UpperPrice2=CTools::GetDoubleFromCEdit(m_editUpperPrice2Ctrl);
    m_pParamData->Order_LowestPrice2=CTools::GetDoubleFromCEdit(m_editLowestPrice2Ctrl);
 }

CParamData* COptSetupDlg::GetSetupParamData()
{
    if(m_pParamData)
        return m_pParamData;
    else
        return NULL;
}

void COptSetupDlg::Update(const PlatformStru_DepthMarketData* pMarketData)
{
    string szInstNameUpper1 = CTools::GetStringFromCComboBox(m_combInstrument1Ctrl);
    string szInstNameUpper2 = CTools::GetStringFromCComboBox(m_combInstrument2Ctrl);

    if( 0==strcmp(pMarketData->InstrumentID,szInstNameUpper1.c_str()) )
    {
        if(!CTools::isInvalidValue_CTPDouble(pMarketData->UpperLimitPrice))
            CTools::SetDoubleToCEdit(m_editUpperPrice1Ctrl,pMarketData->UpperLimitPrice);
        if(!CTools::isInvalidValue_CTPDouble(pMarketData->LowerLimitPrice))
            CTools::SetDoubleToCEdit(m_editLowestPrice1Ctrl,pMarketData->LowerLimitPrice);
    }
    if(0==strcmp(pMarketData->InstrumentID,szInstNameUpper2.c_str()))
    {
        if(!CTools::isInvalidValue_CTPDouble(pMarketData->UpperLimitPrice))
            CTools::SetDoubleToCEdit(m_editUpperPrice2Ctrl,pMarketData->UpperLimitPrice);
        if(!CTools::isInvalidValue_CTPDouble(pMarketData->LowerLimitPrice))
            CTools::SetDoubleToCEdit(m_editLowestPrice2Ctrl,pMarketData->LowerLimitPrice);
    }
}

void COptSetupDlg::SetSetupParamDataToWnd()
{
    m_comboOffsetIDCtrl.SetCurSel(m_pParamData->Param_OffsetID==0?0:1); 
    m_comboArbitrageTypeCtrl.SetCurSel(m_pParamData->Param_ArbitrageType==0?1:0);
    CTools::SetDoubleToCEdit(m_editTriggerCtrl,m_pParamData->Param_TriggerPrice);
    CTools::SetDoubleToCEdit(m_editDeviateCtrl,m_pParamData->Param_DeviatePrice);
    CTools::SetCTimeToCDateTimeCtrl(m_datetimeValidateCtrl,&(m_pParamData->Param_ValidateDay));

    //合约1
    CTools::SetStringToCComboBox(m_combInstrument1Ctrl,m_pParamData->Order_Instrument1.c_str());
    m_comboDirection1Ctrl.SetCurSel(m_pParamData->Order_Direction1);  
    CTools::SetIntToCEdit(m_editCount1Ctrl,m_pParamData->Order_Count1);
    CTools::SetDoubleToCEdit(m_editDelegate1Ctrl,m_pParamData->Order_DelegatePrice1);
    CTools::SetDoubleToCEdit(m_editUpperPrice1Ctrl,m_pParamData->Order_UpperPrice1);
    CTools::SetDoubleToCEdit(m_editLowestPrice1Ctrl,m_pParamData->Order_LowestPrice1);

    //合约2
    CTools::SetStringToCComboBox(m_combInstrument2Ctrl,m_pParamData->Order_Instrument2.c_str());
    m_comboDirection2Ctrl.SetCurSel(m_pParamData->Order_Direction2);  
    CTools::SetIntToCEdit(m_editCount2Ctrl,m_pParamData->Order_Count2);
    CTools::SetDoubleToCEdit(m_editDelegate2Ctrl,m_pParamData->Order_DelegatePrice2);
    CTools::SetDoubleToCEdit(m_editUpperPrice2Ctrl,m_pParamData->Order_UpperPrice2);
    CTools::SetDoubleToCEdit(m_editLowestPrice2Ctrl,m_pParamData->Order_LowestPrice2);

    if(m_comboOffsetIDCtrl.GetCurSel() == 0 && m_comboArbitrageTypeCtrl.GetCurSel()==1)//开仓 价差缩小套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText(">=");
    else if(m_comboOffsetIDCtrl.GetCurSel() == 0 && m_comboArbitrageTypeCtrl.GetCurSel() == 0)//开仓 价差扩大套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText("<=");
    else if((m_comboOffsetIDCtrl.GetCurSel() == 1 || m_comboOffsetIDCtrl.GetCurSel() == 2) && m_comboArbitrageTypeCtrl.GetCurSel() == 1)//平仓/今 价差缩小套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText("<=");
    else if((m_comboOffsetIDCtrl.GetCurSel() == 1 || m_comboOffsetIDCtrl.GetCurSel() == 2) && m_comboArbitrageTypeCtrl.GetCurSel() == 0)//平仓/今 价差扩大套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText(">=");
}

void COptSetupDlg::EnableCtrls(bool bEnable)
{
    m_comboOffsetIDCtrl.EnableWindow(bEnable);
    m_comboArbitrageTypeCtrl.EnableWindow(bEnable);
    m_combInstrument1Ctrl.EnableWindow(bEnable);    
    m_combInstrument2Ctrl.EnableWindow(bEnable);
    m_comboDirection1Ctrl.EnableWindow(bEnable);
}

void COptSetupDlg::OnDeltaposSpinTrigger(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
     int temp = 0;
     if(pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头
         temp -= 1;
     else if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
         temp += 1;
 
     double nValue = CTools::GetDoubleFromCEdit(m_editTriggerCtrl)+(double)temp;
     CTools::SetDoubleToCEdit(m_editTriggerCtrl,nValue);

    *pResult = 0;
}

void COptSetupDlg::OnDeltaposSpinDeviate(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int temp = 0;
    if(pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头
        temp -= 1;
    else if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
        temp += 1;

    double nValue = CTools::GetDoubleFromCEdit(m_editDeviateCtrl)+(double)temp;
    CTools::SetDoubleToCEdit(m_editDeviateCtrl,nValue);
    

    *pResult = 0;
}

void COptSetupDlg::OnDeltaposSpinDelegate1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int temp = 0;
    if(pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头
        temp -= 1;
    else if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
        temp += 1;

    double nValue = CTools::GetDoubleFromCEdit(m_editDelegate1Ctrl)+(double)temp;
    CTools::SetDoubleToCEdit(m_editDelegate1Ctrl,nValue);

    *pResult = 0;
}

void COptSetupDlg::OnDeltaposSpinDelegate2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int temp = 0;
    if(pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头
        temp -= 1;
    else if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
        temp += 1;

    double nValue = CTools::GetDoubleFromCEdit(m_editDelegate2Ctrl)+(double)temp;
    CTools::SetDoubleToCEdit(m_editDelegate2Ctrl,nValue);

    *pResult = 0;
}

void COptSetupDlg::OnCbnSelchangeComboDirection1()
{
    // TODO: Add your control notification handler code here
    m_comboDirection2Ctrl.SetCurSel((0==m_comboDirection1Ctrl.GetCurSel())?1:0);
}

void COptSetupDlg::OnEnChangeEditCount1()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    //m_editCount1Ctrl.GetWindowText(,)
    UINT nCount1 = GetDlgItemInt(IDC_EDIT_COUNT1);
    SetDlgItemInt(IDC_EDIT_COUNT2,nCount1);
}

void COptSetupDlg::OnCbnSelchangeComboOffset()
{
    // TODO: Add your control notification handler code here    
    if(m_comboOffsetIDCtrl.GetCurSel() == 0 && m_comboArbitrageTypeCtrl.GetCurSel()==1)//开仓 价差缩小套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText(">=");
    else if(m_comboOffsetIDCtrl.GetCurSel() == 0 && m_comboArbitrageTypeCtrl.GetCurSel() == 0)//开仓 价差扩大套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText("<=");
    else if((m_comboOffsetIDCtrl.GetCurSel() == 1 || m_comboOffsetIDCtrl.GetCurSel() == 2) && m_comboArbitrageTypeCtrl.GetCurSel() == 1)//平仓/今 价差缩小套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText("<=");
    else if((m_comboOffsetIDCtrl.GetCurSel() == 1 || m_comboOffsetIDCtrl.GetCurSel() == 2) && m_comboArbitrageTypeCtrl.GetCurSel() == 0)//平仓/今 价差扩大套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText(">=");
}

void COptSetupDlg::OnCbnSelchangeComboArbitragetype()
{
    // TODO: Add your control notification handler code here
    if(m_comboOffsetIDCtrl.GetCurSel() == 0 && m_comboArbitrageTypeCtrl.GetCurSel()==1)//开仓 价差缩小套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText(">=");
    else if(m_comboOffsetIDCtrl.GetCurSel() == 0 && m_comboArbitrageTypeCtrl.GetCurSel() == 0)//开仓 价差扩大套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText("<=");
    else if((m_comboOffsetIDCtrl.GetCurSel() == 1 || m_comboOffsetIDCtrl.GetCurSel() == 2) && m_comboArbitrageTypeCtrl.GetCurSel() == 1)//平仓/今 价差缩小套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText("<=");
    else if((m_comboOffsetIDCtrl.GetCurSel() == 1 || m_comboOffsetIDCtrl.GetCurSel() == 2) && m_comboArbitrageTypeCtrl.GetCurSel() == 0)//平仓/今 价差扩大套利
        GetDlgItem(IDC_STATIC_SIGN)->SetWindowText(">=");
}

HBRUSH COptSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here    
    if(nCtlColor == CTLCOLOR_STATIC && 
       (pWnd->GetDlgCtrlID()==IDC_STATIC_TEXT1)||
       (pWnd->GetDlgCtrlID()==IDC_STATIC_TEXT2)||
       (pWnd->GetDlgCtrlID()==IDC_STATIC_TEXT3))
    {
        pDC->SetBkMode(TRANSPARENT);//设置背景透明
        //pDC->SetTextColor(RGB(255,0,0));//设置字体为红
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

BOOL COptSetupDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnEraseBkgnd(pDC);
    HBITMAP   m_hBitmap;
    HDC           m_hBkDC;
    m_hBitmap   =   ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP_BKG));
    m_hBkDC     =   ::CreateCompatibleDC(pDC-> m_hDC);
    if(m_hBitmap   &&   m_hBkDC)
    {
        ::SelectObject(m_hBkDC,m_hBitmap);
        ::StretchBlt(pDC->m_hDC,0,0,1900,1900,m_hBkDC,0,0,100,800,SRCCOPY);
        ::DeleteObject(m_hBitmap);
        ::DeleteDC(m_hBkDC);
    }

    return TRUE;
}

void COptSetupDlg::OnCbnSelchangeComboInst1()
{
    // TODO: Add your control notification handler code here
    CString str;
    m_combInstrument1Ctrl.GetLBText(m_combInstrument1Ctrl.GetCurSel(),str);
    SubscribeMarketData((string)str);
    m_bInst1InputError = false;
}

void COptSetupDlg::OnCbnEditchangeComboInst1()
{
    // TODO: Add your control notification handler code here
    InstrumentIDType InstName;
    memset(InstName,0,sizeof(InstName));       
    memcpy(InstName,CTools::GetStringFromCComboBox(m_combInstrument1Ctrl).c_str(),sizeof(InstName));

    string szInst(InstName);
    set<string>::iterator it = m_setInstrumentID.find(szInst);
    if(it!=m_setInstrumentID.end())
        SubscribeMarketData(szInst);
    else
    {
        //转一下小写
        transform(szInst.begin(), szInst.end(), szInst.begin(), (int (*)(int))tolower); 
        map<string,string>::iterator _it = m_mapInputKey2RightInst.find(szInst);
        if(_it!=m_mapInputKey2RightInst.end())
        {
            szInst = _it->second;
            CTools::SetStringToCComboBox(m_combInstrument1Ctrl,szInst.c_str());
            SubscribeMarketData(szInst);
        }
        else
        {
            m_bInst1InputError = true;
            return;
        }
    }

    m_bInst1InputError = false;
}

void COptSetupDlg::OnCbnSelchangeComboInst2()
{
    // TODO: Add your control notification handler code here
    CString str;
    m_combInstrument2Ctrl.GetLBText(m_combInstrument2Ctrl.GetCurSel(),str);
    SubscribeMarketData((string)str);
    m_bInst2InputError = false;
}

void COptSetupDlg::OnCbnEditchangeComboInst2()
{
    // TODO: Add your control notification handler code here
    InstrumentIDType InstName;
    memset(InstName,0,sizeof(InstName));       
    memcpy(InstName,CTools::GetStringFromCComboBox(m_combInstrument2Ctrl).c_str(),sizeof(InstName));

    string szInst(InstName);
    set<string>::iterator it = m_setInstrumentID.find(szInst);
    if(it!=m_setInstrumentID.end())
        SubscribeMarketData(szInst);
    else
    {
        //转一下小写
        transform(szInst.begin(), szInst.end(), szInst.begin(), (int (*)(int))tolower); 
        map<string,string>::iterator _it = m_mapInputKey2RightInst.find(szInst);
        if(_it!=m_mapInputKey2RightInst.end())
        {
            szInst = _it->second;
            CTools::SetStringToCComboBox(m_combInstrument2Ctrl,szInst.c_str());
            SubscribeMarketData(szInst);
        }
        else
        {
            m_bInst2InputError = true;
            return;
        }
    }

    m_bInst2InputError = false;
}

void COptSetupDlg::SubscribeMarketData(string instrumentID)
{    
    if(instrumentID.length()==0)
        return;    

    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            if(instrumentID != m_SubscribedInstrument)
            {
                //订阅新行情
			    pAPI->Send(CMDID_QUOTE_SUBSCRIBE,(char*)instrumentID.c_str(),sizeof(InstrumentIDType));
                pAPI->Send(CMDID_QUOTE_SNAPSHOT_QUERY,(char*)instrumentID.c_str(),sizeof(InstrumentIDType));

                //保存当前订阅行情的合约名称
                m_SubscribedInstrument=instrumentID;                
            }
        }
    }
}
