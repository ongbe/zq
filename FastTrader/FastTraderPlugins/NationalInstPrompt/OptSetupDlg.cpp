// OptSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NationalInstPrompt.h"
#include "NationalInstPromptDlg.h"
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
}

COptSetupDlg::COptSetupDlg(CParamData* pData,CWnd* pParent /*=NULL*/)
    : CDialog(COptSetupDlg::IDD, pParent)
{
    EnableAutomation();

    m_pParamData = pData;
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
    DDX_Control(pDX, IDC_EDIT_BANDCODE, m_editBandCodeCtrl);
    DDX_Control(pDX, IDC_EDIT_BANDINSTRUMENT, m_editBandInstrumentCtrl);
    DDX_Control(pDX, IDC_EDIT_QUOTPRICE, m_editQuotPriceCtrl);
    DDX_Control(pDX, IDC_EDIT_IRR, m_editIRRCtrl);
    DDX_Control(pDX, IDC_EDIT_INTERESTRATES, m_editInterestRatesCtrl);
    DDX_Control(pDX, IDC_EDIT_BANDPRICE, m_editBandPriceCtrl);
    DDX_Control(pDX, IDC_EDIT_TRANSFERFACTOR, m_editTransferFactorCtrl);
    DDX_Control(pDX, IDC_EDIT_YEARINTERESTCOUNT, m_editYearInterestCountCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER_PREDELIVDATE, m_datetimePerDelivCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER_NEXTDELIVDATE, m_datetimeNextDelivCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER_DELIVDATE, m_datetimeDelivCtrl);
    DDX_Control(pDX, IDC_CHECK_ALARM, m_btnAlarmCtrl);
    DDX_Control(pDX, IDC_CHECK_AUTOORDER, m_btnAutoOrderCtrl);
    DDX_Control(pDX, IDC_EDIT_INSTRUMENT, m_editInstrumentCtrl);
    DDX_Control(pDX, IDC_EDIT_COUNT, m_editCountCtrl);
    DDX_Control(pDX, IDC_COMBO_PRICE, m_comboPriceCtrl);
    DDX_Control(pDX, IDC_COMBO_DIRECTION, m_comboDirectionCtrl);
    DDX_Control(pDX, IDC_COMBO_OFFSETID, m_comboOffsetIDCtrl);
}


BEGIN_MESSAGE_MAP(COptSetupDlg, CDialog)
    ON_BN_CLICKED(IDCANCEL, &COptSetupDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &COptSetupDlg::OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDIT_BANDINSTRUMENT, &COptSetupDlg::OnEnChangeEditBandinstrument)
    ON_BN_CLICKED(IDC_CHECK_AUTOORDER, &COptSetupDlg::OnBnClickedCheckAutoorder)
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
    m_comboDirectionCtrl.AddString("买");
    m_comboDirectionCtrl.AddString("卖");
    m_comboDirectionCtrl.SetCurSel(0);
    
    m_comboOffsetIDCtrl.AddString("开仓");
    m_comboOffsetIDCtrl.AddString("平仓");
    m_comboOffsetIDCtrl.AddString("平今");
    m_comboOffsetIDCtrl.SetCurSel(0);

    m_comboPriceCtrl.AddString("市价");
    m_comboPriceCtrl.SetCurSel(0);


    SetSetupParamDataToWnd();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void COptSetupDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    if(!Verify())
        return;

    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_CLOSE,0,0);

    CreateSetupParamData();

    OnOK();
}

void COptSetupDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here

    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_CLOSE,0,0);

    OnCancel();
}

bool COptSetupDlg::Verify()
{
    if (CTools::GetStringFromCEdit(m_editBandCodeCtrl).empty())
    {
        ::MessageBox(NULL, "现券代码不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editBandInstrumentCtrl).empty())
    {
        ::MessageBox(NULL, "策略合约不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editIRRCtrl).empty())
    {
        ::MessageBox(NULL, "IRR不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }
    else
    {
        if (!CTools::IsHaveDot(CTools::GetStringFromCEdit(m_editIRRCtrl)))
        {
            ::MessageBox(NULL, "IRR为0~1之间的小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return false;
        }
    } 

    if (CTools::GetStringFromCEdit(m_editInterestRatesCtrl).empty())
    {
        ::MessageBox(NULL, "票面利率不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }
    else
    {
        if (!CTools::IsHaveDot(CTools::GetStringFromCEdit(m_editInterestRatesCtrl)))
        {
            ::MessageBox(NULL, "票面利率为0~1之间的小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return false;
        }
    }

    if (CTools::GetStringFromCEdit(m_editBandPriceCtrl).empty())
    {
        ::MessageBox(NULL, "现券价格不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editTransferFactorCtrl).empty())
    {
        ::MessageBox(NULL, "转换因子不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editYearInterestCountCtrl).empty())
    {
        ::MessageBox(NULL, "年付息次数不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }
    else
    {
        if(CTools::IsHaveDot(CTools::GetStringFromCEdit(m_editYearInterestCountCtrl)))
        {
            ::MessageBox(NULL, "年付息次数不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    CTime time,time1,time2,time3;

    time = CTime::GetCurrentTime();  
    m_datetimePerDelivCtrl.GetTime(time1);
    m_datetimeNextDelivCtrl.GetTime(time2);
    m_datetimeDelivCtrl.GetTime(time3);

    if (time1 >= time)
    {
        ::MessageBox(NULL, "上一付息日必须小于今日，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (time >= time2)
    {
        ::MessageBox(NULL, "下一付息日必须大于今日，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (time3 < time)
    {
        ::MessageBox(NULL, "交割日必须大于等于今日，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if(BST_UNCHECKED == m_btnAutoOrderCtrl.GetCheck())
        goto END;

    if (CTools::GetStringFromCEdit(m_editInstrumentCtrl).empty())
    {
        ::MessageBox(NULL, "触发合约不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }

    if (CTools::GetStringFromCEdit(m_editCountCtrl).empty())
    {
        ::MessageBox(NULL, "触发手数不能为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }
    else if(CTools::IsHaveDot(CTools::GetStringFromCEdit(m_editCountCtrl)))
    {
        ::MessageBox(NULL, "触发手数不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }else if (0==(int)CTools::GetDoubleFromCEdit(m_editCountCtrl))
    {
        ::MessageBox(NULL, "触发手数能为0，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return false;
    }   
    

END:
    return true;
}

//将控件中的值放到m_algo中去
void COptSetupDlg::CreateSetupParamData()
{
    if(m_pParamData==NULL)
        return;

    //策略输入项   
    m_pParamData->Param_BandCode=CTools::GetStringFromCEdit(m_editBandCodeCtrl);
    m_pParamData->Param_BandInstrument=CTools::GetStringFromCEdit(m_editBandInstrumentCtrl);
    m_pParamData->Param_QuotPrice=CTools::GetDoubleFromCEdit(m_editQuotPriceCtrl);
    m_pParamData->Param_IRR=CTools::GetDoubleFromCEdit(m_editIRRCtrl);
    m_pParamData->Param_InterestRates=CTools::GetDoubleFromCEdit(m_editInterestRatesCtrl);
    m_pParamData->Param_BandPrice=CTools::GetDoubleFromCEdit(m_editBandPriceCtrl);
    m_pParamData->Param_TransferFactor=CTools::GetDoubleFromCEdit(m_editTransferFactorCtrl);
    m_pParamData->Param_YearInterestCount=(int)CTools::GetDoubleFromCEdit(m_editYearInterestCountCtrl);
    m_pParamData->Param_PreDelivDate=CTools::GetCTimeFromCDateTimeCtrl(m_datetimePerDelivCtrl);
    m_pParamData->Param_NextDelivDate=CTools::GetCTimeFromCDateTimeCtrl(m_datetimeNextDelivCtrl);
    m_pParamData->Param_DelivDate=CTools::GetCTimeFromCDateTimeCtrl(m_datetimeDelivCtrl);


    //触发输入项
    m_pParamData->Order_Alarm = m_btnAlarmCtrl.GetCheck()?true:false;
    m_pParamData->Order_AutoOrder = m_btnAutoOrderCtrl.GetCheck()?true:false;
    m_pParamData->Order_Instrument=CTools::GetStringFromCEdit(m_editInstrumentCtrl);
    m_pParamData->Order_Direction = CTools::GetStringFromCComboBox(m_comboDirectionCtrl)=="买"?0:1;
    if(CTools::GetStringFromCComboBox(m_comboOffsetIDCtrl)=="开仓") m_pParamData->Order_OffsetID=0;
    else if(CTools::GetStringFromCComboBox(m_comboOffsetIDCtrl)=="平仓") m_pParamData->Order_OffsetID=1;
    else m_pParamData->Order_OffsetID=2;	//平今
    m_pParamData->Order_Price = CTools::GetStringFromCComboBox(m_comboPriceCtrl)=="市价"?0:1;
    m_pParamData->Order_Count=(int)CTools::GetDoubleFromCEdit(m_editCountCtrl);
 }

CParamData* COptSetupDlg::GetSetupParamData()
{
    if(m_pParamData)
        return m_pParamData;
    else
        return NULL;
}

//将m_algo中的值设置到控件中去
void COptSetupDlg::SetSetupParamDataToWnd()
{
    //策略输入项  
    CTools::SetStringToCEdit(m_editBandCodeCtrl,m_pParamData->Param_BandCode.c_str());
    CTools::SetStringToCEdit(m_editBandInstrumentCtrl,m_pParamData->Param_BandInstrument.c_str());
    CTools::SetDoubleToCEdit(m_editQuotPriceCtrl,m_pParamData->Param_QuotPrice);
    CTools::SetDoubleToCEdit(m_editIRRCtrl,m_pParamData->Param_IRR);
    CTools::SetDoubleToCEdit(m_editInterestRatesCtrl,m_pParamData->Param_InterestRates);
    CTools::SetDoubleToCEdit(m_editBandPriceCtrl,m_pParamData->Param_BandPrice);
    CTools::SetDoubleToCEdit(m_editTransferFactorCtrl,m_pParamData->Param_TransferFactor);
    CTools::SetIntToCEdit(m_editYearInterestCountCtrl,m_pParamData->Param_YearInterestCount);
    CTools::SetCTimeToCDateTimeCtrl(m_datetimePerDelivCtrl,&(m_pParamData->Param_PreDelivDate));
    CTools::SetCTimeToCDateTimeCtrl(m_datetimeNextDelivCtrl,&(m_pParamData->Param_NextDelivDate));
    CTools::SetCTimeToCDateTimeCtrl(m_datetimeDelivCtrl,&(m_pParamData->Param_DelivDate));

    //触发输入项
    m_btnAlarmCtrl.SetCheck(m_pParamData->Order_Alarm?1:0);
    m_btnAutoOrderCtrl.SetCheck(m_pParamData->Order_AutoOrder?1:0);     
    m_comboDirectionCtrl.SetCurSel(m_pParamData->Order_Direction==0?0:1);  
    m_comboPriceCtrl.SetCurSel(m_pParamData->Order_Price);
    CTools::SetStringToCEdit(m_editInstrumentCtrl,m_pParamData->Order_Instrument.c_str());
    if(m_pParamData->Order_OffsetID==0) m_comboOffsetIDCtrl.SetCurSel(0);
    else if(m_pParamData->Order_OffsetID==1) m_comboOffsetIDCtrl.SetCurSel(1);
    else m_comboOffsetIDCtrl.SetCurSel(2);
    CTools::SetIntToCEdit(m_editCountCtrl,m_pParamData->Order_Count);

    EnableCtrls(m_pParamData->Order_AutoOrder);
}

void COptSetupDlg::OnEnChangeEditBandinstrument()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    InstrumentIDType InstName;
    memset(InstName,0,sizeof(InstName));
    memcpy(InstName,CTools::GetStringFromCEdit(m_editBandInstrumentCtrl).c_str(),sizeof(InstName));

    if(0==strlen(InstName))
        return;
    
    string szInstNameUpper(InstName);
    transform(szInstNameUpper.begin(), szInstNameUpper.end(), szInstNameUpper.begin(), (int (*)(int))toupper); 

    /*合约输入edit智能匹配的功能 预留（没完成）
    string szInstrument(InstName);
    transform(szInstrument.begin(), szInstrument.end(), szInstrument.begin(), (int (*)(int))tolower);

    CNationalInstPromptDlg* pMainDlg = (CNationalInstPromptDlg*)(AfxGetApp()->m_pMainWnd);
    //set<string>::iterator it_exchange = pMainDlg->m_setExchangeID.begin();
    string szExchange;
    if(pMainDlg->m_setExchangeID.size()>0)
        szExchange = *(pMainDlg->m_setExchangeID.begin());

    // 添加生成容错合约列表
    map<string, string> mapConstractIDVariety;
    MakeConstractIDVarietyMap(szExchange,pMainDlg->m_setInstrumentID, mapConstractIDVariety);

    map<string, string>::iterator it = mapConstractIDVariety.find(szInstrument.c_str());
    if(it != mapConstractIDVariety.end()) 
    {
        if(szInstrument.compare(it->second)!=0) 
        {            
            CTools::SetStringToCEdit(m_editBandInstrumentCtrl,(it->second).c_str());
        }
    }
    */
    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            if((string)szInstNameUpper != m_SubscribedInstrument)
            {
                //退订旧行情
				pAPI->Send(CMDID_QUOTE_UNSUBSCRIBE,(char*)(m_SubscribedInstrument.c_str()),sizeof(InstrumentIDType));

                //保存当前订阅行情的合约名称
                m_SubscribedInstrument=(string)szInstNameUpper;

                //订阅新行情
				pAPI->Send(CMDID_QUOTE_SUBSCRIBE,(char*)szInstNameUpper.c_str(),sizeof(InstrumentIDType));
            }
        }
    }
}

void COptSetupDlg::OnBnClickedCheckAutoorder()
{
    // TODO: Add your control notification handler code here
    (BST_CHECKED==m_btnAutoOrderCtrl.GetCheck())?EnableCtrls(true):EnableCtrls(false);
}

void COptSetupDlg::EnableCtrls(bool bEnable)
{
    m_editInstrumentCtrl.EnableWindow(bEnable);
    m_editCountCtrl.EnableWindow(bEnable);
    m_comboDirectionCtrl.EnableWindow(bEnable);
    m_comboOffsetIDCtrl.EnableWindow(bEnable);
    m_comboPriceCtrl.EnableWindow(bEnable);
}
/*合约输入edit智能匹配的功能 预留（没完成）
bool COptSetupDlg::MakeConstractIDVarietyMap(string szExchengeID,set<string> setInstrumentIDs, map<string, string>& mapConstractIDVariety)
{

    int nRet = 0;
    string strInstrumentID;
    string strNewID;
    string strExchangeID;
    int nCurrTenYear=0;
    int nCurrMonth=0;
    PlatformStru_InstrumentInfo tInstrumentField;
    SYSTEMTIME sysTime;

    GetLocalTime(&sysTime);
    nCurrTenYear = (sysTime.wYear%100)/10;
    nCurrMonth = sysTime.wMonth;

    // 生成郑商所4位日期及全小写容差合约
    for(set<string>::iterator it = setInstrumentIDs.begin();it != setInstrumentIDs.end(); ++it) 
    {
        strInstrumentID = *it;
        ///获取指定合约信息
        memset(&tInstrumentField,0,sizeof(tInstrumentField));
        strExchangeID = szExchengeID;
        if(strExchangeID.compare("CZCE")==0) 
        {
            int nPos = 0;
            BOOL bIsFound = FALSE;
            while(nPos < (int)strlen(tInstrumentField.InstrumentID)) 
            {
                if(tInstrumentField.InstrumentID[nPos]>='0'&& tInstrumentField.InstrumentID[nPos]<='9') 
                {
                        bIsFound = TRUE;
                        break;
                }
                nPos++;
            }

            if(bIsFound) 
            {
                int nMonth = 0;
                char strName[16];
                char strYear[16];
                char strMonth[16];
                ZeroMemory(strName, sizeof(strName));
                ZeroMemory(strYear, sizeof(strYear));
                ZeroMemory(strMonth, sizeof(strMonth));
                strncpy(strName, tInstrumentField.InstrumentID, nPos);
                strncpy(strYear+1, tInstrumentField.InstrumentID+nPos, 1);
                strncpy(strMonth, tInstrumentField.InstrumentID+nPos+1, 2);
                nMonth = atoi(strMonth);
                if(nCurrMonth < nMonth)
                {
                    strYear[0] = nCurrTenYear+'0';
                }
                else 
                {
                    if(strYear[1]=='9') 
                    {
                        strYear[0] = nCurrTenYear+1+'0';
                    }
                    else 
                    {
                        strYear[0] = nCurrTenYear+'0';
                    }
                }
                strNewID = strName;
                strNewID += strYear;
                strNewID += strMonth;
//                 strNewID.LowerCase();
                transform(strNewID.begin(), strNewID.end(), strNewID.begin(), (int (*)(int))tolower); 
                mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
            }
        }
    }

    // 生成全小写容差合约
    for(set<string>::iterator it = setInstrumentIDs.begin(); 
        it != setInstrumentIDs.end(); ++it) {
            strInstrumentID = *it;
            strNewID = strInstrumentID;
//             strNewID.LowerCase();
            transform(strNewID.begin(), strNewID.end(), strNewID.begin(), (int (*)(int))tolower); 
            mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
    }

    return TRUE;
}
*/
