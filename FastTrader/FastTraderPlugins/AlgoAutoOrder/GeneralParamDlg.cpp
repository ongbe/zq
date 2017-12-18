// GeneralParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "GeneralParamDlg.h"
#include "Tools_AlgoTrading.h"
#include "Module-Misc2/tools_util.h"
#include "AlgoAutoOrderDlg.h"
#include "MessageBoxDialog.h"

// CGeneralParamDlg dialog

IMPLEMENT_DYNAMIC(CGeneralParamDlg, CDialog)

CGeneralParamDlg::CGeneralParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog/*CBookOrderOperations*/(CGeneralParamDlg::IDD, pParent)
    , m_nCurVolume(0)
    , m_bOkClicked(FALSE)
    ,m_bCloseupComboBasicprice(TRUE)
{
}

CGeneralParamDlg::~CGeneralParamDlg()
{
}

void CGeneralParamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INSTRUMENTID, m_editInstrumentID);
    DDX_Control(pDX, IDC_CHECK_ACTIVEOPTIMIZE, m_checkActiveOptimize);
    DDX_Control(pDX, IDC_COMBO_BUYSELL, m_comboBuysell);
    DDX_Control(pDX, IDC_COMBO_OPENCLOSE, m_comboOpenclose);
    DDX_Control(pDX, IDC_EDIT_INSTRUMENTCOUNT, m_editInstrumentCount);
    DDX_Control(pDX, IDC_EDIT_PRICELIMIT, m_editPriceLimit);
    DDX_Control(pDX, IDC_SPIN_PRICELIMIT, m_spinPriceLimit);
    DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_editInstrumentStartTime);
    DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_editInstrumentEndTime);
    DDX_Control(pDX, IDC_SLID_MARKETDEGREED, m_slidMarketDegreed);
    DDX_Control(pDX, IDC_EDIT_MARKETDEGREED, m_editMarketDegreed);
    DDX_Control(pDX, IDC_SPIN_MARKETDEGREED, m_spinMarketDegreed);
    DDX_Control(pDX, IDC_EDIT_ORDERCOUNT, m_editOrderCount);
    DDX_Control(pDX, IDC_COMBO_ORDERCOUNT, m_comboOrderCount);
//     DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT, m_editResetOrderCount);
//     DDX_Control(pDX, IDC_COMBO_RESETORDERCOUNT1, m_comboResetOrderCount);
    DDX_Control(pDX, IDC_COMBO_BASICPRICE, m_comboBasicPrice);
    DDX_Control(pDX, IDC_EDIT_BASICPRICE, m_editBasicPrice);
//     DDX_Control(pDX, IDC_COMBO_SLIDPRICESPREAD, m_comboSlidPriceSpread);
//     DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD1, m_editSlidPriceSpread1);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD2, m_editSlidPriceSpread2);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD3, m_editSlidPriceSpread3);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD4, m_editSlidPriceSpread4);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD5, m_editSlidPriceSpread5);
//     DDX_Control(pDX, IDC_COMBO_RESETORDERCOUNT2, m_comboRelateOrderCount);
//     DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT1, m_editRelateOrderCount1);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT2, m_editRelateOrderCount2);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT3, m_editRelateOrderCount3);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT4, m_editRelateOrderCount4);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT5, m_editRelateOrderCount5);
    DDX_Control(pDX, IDC_STATIC_SELLPRICE, m_staticSellPrice);
    DDX_Control(pDX, IDC_STATIC_LATESTPRICE, m_staticLatestPrice);
    DDX_Control(pDX, IDC_STATIC_BUYPRICE, m_staticBuyPrice);
    DDX_Control(pDX, IDC_STATIC_MIDDLEPRICE, m_staticMiddlePrice);
    DDX_Control(pDX, IDC_STATIC_YESTERDAYPRICE, m_staticYesterdayPrice);
    DDX_Control(pDX, IDC_COMBO_ACTIVEOPTIMIZE, m_comboActiveOptimize);
    DDX_Control(pDX, IDC_COMBO_IDLETIME, m_comboIdleTime);
    DDX_Control(pDX, IDC_COMBO_EXECUTETIME, m_comboExecuteTime);
    DDX_Control(pDX, IDC_COMBO_OVERRETRYTOTAL, m_comboOverTimeTotal);
    DDX_Control(pDX, IDC_EDIT_IDLETIME, m_editIdleTime);
    DDX_Control(pDX, IDC_EDIT_EXECUTETIME, m_editExecuteTime);
    DDX_Control(pDX, IDC_CHECK_OVERRETRYCOUNT, m_checkOverTimeRetryCount);
    DDX_Control(pDX, IDC_EDIT_OVERRETRYCOUNT, m_editOverTimeRetryCount);
    DDX_Control(pDX, IDC_CHECK_OVERRETRYTOTAL, m_checkOverTimeTotal);
    DDX_Control(pDX, IDC_EDIT_OVERRETRYTOTAL, m_editOverTimeTotal);

    DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
    DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
    DDX_Control(pDX, IDC_BTN_PARAMSETUP, m_btnOptSetup);
    DDX_Control(pDX, IDC_STATIC_TITLE_SELLPRICE, m_staticTitleSellPrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_LATESTPRICE, m_staticTitleLatestPrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_BUYPRICE, m_staticTitleBuyPrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_MIDDLEPRICE, m_staticTitleMiddlePrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_YESTERDAYPRICE, m_staticTitleYesterdayPrice);
    DDX_Control(pDX, IDC_STATIC_MUST1, m_staticMust1);
    DDX_Control(pDX, IDC_STATIC_MUST2, m_staticMust2);
    DDX_Control(pDX, IDC_STATIC_MUST3, m_staticMust3);
    DDX_Control(pDX, IDC_STATIC_MUST4, m_staticMust4);
    DDX_Control(pDX, IDC_STATIC_SPLIT, m_staticSplit);
    DDX_Control(pDX, IDC_CHECK_MULTIPRICE, m_btnCheckMultiPrice);
    DDX_Control(pDX, IDC_COMBO_MULTIPRICE, m_comboMultiPrice);
}


BEGIN_MESSAGE_MAP(CGeneralParamDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_PARAMSETUP, &CGeneralParamDlg::OnBnClickedBtnParamsetup)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRICELIMIT, &CGeneralParamDlg::OnDeltaposSpinPricelimit)
    ON_WM_HSCROLL()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MARKETDEGREED, &CGeneralParamDlg::OnDeltaposSpinMarketdegreed)    
    ON_EN_CHANGE(IDC_EDIT_MARKETDEGREED, &CGeneralParamDlg::OnEnChangeEditMarketdegreed)
	ON_EN_CHANGE(IDC_EDIT_INSTRUMENTID, &CGeneralParamDlg::OnEnChangeEditInstrumentid)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CGeneralParamDlg::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CGeneralParamDlg::OnBnClickedBtnCancel)
	ON_WM_PAINT()
    ON_CBN_SELCHANGE(IDC_COMBO_ORDERCOUNT, &CGeneralParamDlg::OnCbnSelchangeComboOrdercount)
    ON_BN_CLICKED(IDC_CHECK_MULTIPRICE, &CGeneralParamDlg::OnBnClickedCheckMultiPrice)
    ON_CBN_CLOSEUP(IDC_COMBO_BASICPRICE, &CGeneralParamDlg::OnCbnCloseupComboBasicprice)
    ON_CBN_DROPDOWN(IDC_COMBO_BASICPRICE, &CGeneralParamDlg::OnCbnDropdownComboBasicprice)
    ON_CBN_SELCHANGE(IDC_COMBO_BASICPRICE, &CGeneralParamDlg::OnCbnSelchangeComboBasicprice)
END_MESSAGE_MAP()


// CGeneralParamDlg message handlers

void CGeneralParamDlg::OnBnClickedBtnParamsetup()
{
    // TODO: Add your control notification handler code here
    if(m_comboActiveOptimize.IsWindowEnabled())   
        m_OptimizeSetupDlg.SetSelectOptIndex(m_comboActiveOptimize.GetCurSel());
    
    m_OptimizeSetupDlg.SetParentWnd(this);
    m_OptimizeSetupDlg.DoModal();
}

BOOL CGeneralParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_staticMust1.Set_TextColor(RGB(255,0,0));;
    m_staticMust2.Set_TextColor(RGB(255,0,0));;
    m_staticMust3.Set_TextColor(RGB(255,0,0));;
    m_staticMust4.Set_TextColor(RGB(255,0,0));;

    m_staticTitleSellPrice.Set_TextColor(RGB(255,0,0));
    m_staticTitleLatestPrice.Set_TextColor(RGB(255,0,0));
    m_staticTitleBuyPrice.Set_TextColor(RGB(255,0,0)); 
    m_staticTitleMiddlePrice.Set_TextColor(RGB(255,0,0)); 
    m_staticTitleYesterdayPrice.Set_TextColor(RGB(255,0,0));

    m_staticSellPrice.Set_TextColor(RGB(255,0,0)); 
    m_staticLatestPrice.Set_TextColor(RGB(255,0,0));
    m_staticBuyPrice.Set_TextColor(RGB(255,0,0)); 
    m_staticMiddlePrice.Set_TextColor(RGB(255,0,0)); 
    m_staticYesterdayPrice.Set_TextColor(RGB(255,0,0)); 
    

    m_comboBuysell.InsertString(0, "买入");
    m_comboBuysell.InsertString(1, "卖出");
    m_comboBuysell.SetCurSel(0);

    m_comboOpenclose.InsertString(0, "开仓");
    m_comboOpenclose.InsertString(1, "平今");
    m_comboOpenclose.InsertString(2, "平仓");
    m_comboOpenclose.SetCurSel(0);

    m_comboOrderCount.InsertString(0, "百分比");
    m_comboOrderCount.InsertString(1, "手数");
    m_comboOrderCount.SetCurSel(1);

//     m_comboResetOrderCount.InsertString(0, "百分比");
//     m_comboResetOrderCount.InsertString(1, "手数");
//     m_comboResetOrderCount.SetCurSel(1);

//     m_comboRelateOrderCount.InsertString(0, "百分比");
//     m_comboRelateOrderCount.InsertString(1, "手数");
//     m_comboRelateOrderCount.SetCurSel(1);

    m_comboBasicPrice.InsertString(0, "买价");
    m_comboBasicPrice.InsertString(1, "最新价");
    m_comboBasicPrice.InsertString(2, "卖价");
    m_comboBasicPrice.InsertString(3, "中间价");
    m_comboBasicPrice.InsertString(4, "昨结算价");
    m_comboBasicPrice.InsertString(5, "自定义价格");
    m_comboBasicPrice.SetCurSel(1);
    m_editBasicPrice.EnableWindow(FALSE);

//     m_comboSlidPriceSpread.InsertString(0, "百分比");
//     m_comboSlidPriceSpread.InsertString(1, "点位");
//     m_comboSlidPriceSpread.SetCurSel(1);

    m_comboMultiPrice.InsertString(0, "-5");
    m_comboMultiPrice.InsertString(1, "-4");
    m_comboMultiPrice.InsertString(2, "-3");
    m_comboMultiPrice.InsertString(3, "-2");
    m_comboMultiPrice.InsertString(4, "-1");
    m_comboMultiPrice.InsertString(5, "0");
    m_comboMultiPrice.InsertString(6, "1");
    m_comboMultiPrice.InsertString(7, "2");
    m_comboMultiPrice.InsertString(8, "3");
    m_comboMultiPrice.InsertString(9, "4");
    m_comboMultiPrice.InsertString(10, "5");
    m_comboMultiPrice.SetCurSel(5);

    m_comboIdleTime.InsertString(0, "秒钟");
    m_comboIdleTime.InsertString(1, "分钟");
    m_comboIdleTime.InsertString(2, "时钟");
    m_comboIdleTime.SetCurSel(0);

    m_comboExecuteTime.InsertString(0, "秒钟");
    m_comboExecuteTime.InsertString(1, "分钟");
    m_comboExecuteTime.InsertString(2, "时钟");
    m_comboExecuteTime.SetCurSel(0);

    m_comboOverTimeTotal.InsertString(0, "时钟");
    m_comboOverTimeTotal.InsertString(1, "分钟");
    m_comboOverTimeTotal.InsertString(2, "秒钟");
    m_comboOverTimeTotal.SetCurSel(0);

    m_slidMarketDegreed.SetRange(0,100);
    m_slidMarketDegreed.SetTicFreq(10);
    m_slidMarketDegreed.SetPos(0);
    
    m_staticBuyPrice.SetWindowText("—");
    m_staticSellPrice.SetWindowText("—");
    m_staticMiddlePrice.SetWindowText("—");
    m_staticYesterdayPrice.SetWindowText("—");
    m_staticLatestPrice.SetWindowText("—");

    m_editMarketDegreed.SetWindowText("0");

	m_editInstrumentStartTime.SetFormat("HH:mm:ss tt");
	m_editInstrumentEndTime.SetFormat("HH:mm:ss tt");

    CTime t = CTime::GetCurrentTime(); //获取系统日期
    int h=t.GetHour();
	int mm=t.GetMinute(); 
	int s=t.GetSecond(); 

	int verH = 15-h;
    int verM = 30-mm;
	t = t + CTimeSpan(0,verH,verM,-s);
    m_editInstrumentEndTime.SetTime(&t);
     
    m_btnPlay.LoadBitmaps(IDB_BITMAP_PLAY,1,1,1,1,1); 
    m_btnStop.LoadBitmaps(IDB_BITMAP_STOP,1,1,1,1,1);
    m_btnOptSetup.LoadBitmaps(IDB_BITMAP_DEFAULT,1,1,1,1,1);

    AddToolTip();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralParamDlg::SetInstrumentID(CString& strInstrumentID)
{
    m_editInstrumentID.SetSel(0, -1);   
    m_editInstrumentID.ReplaceSel(strInstrumentID);
}

CString CGeneralParamDlg::GetInstrumentID()
{
    char strText[64];
    ZeroMemory(strText, sizeof(strText));
    m_editInstrumentID.GetLine(0, strText, 63);
    return strText;
}

void CGeneralParamDlg::UpdateDepthMarketContent(PlatformStru_DepthMarketData& depthMarketDate)
{
	CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
	if(pAlgo == NULL) return;

    PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
    if(pInfo != NULL)
        pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;
	
//     if( !util::isInvalidValue(depthMarketDate.BidPrice1)&&
//         !util::isInvalidValue(depthMarketDate.AskPrice1)&&
//         !util::isInvalidValue(depthMarketDate.LastPrice)&&
//         !util::isInvalidValue(depthMarketDate.PreSettlementPrice))
    {
		if (pAlgo->GetOldInstrumentID().CompareNoCase(depthMarketDate.InstrumentID) != 0)
			CTools_AlgoTrading::SetStringToCEdit(m_editInstrumentID,depthMarketDate.InstrumentID);
        if(!util::isInvalidValue(depthMarketDate.BidPrice1))
            CTools_AlgoTrading::SetStringToCStatic(m_staticBuyPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.BidPrice1));
        if(!util::isInvalidValue(depthMarketDate.AskPrice1))
            CTools_AlgoTrading::SetStringToCStatic(m_staticSellPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.AskPrice1));
        if(!util::isInvalidValue(depthMarketDate.LastPrice))
            CTools_AlgoTrading::SetStringToCStatic(m_staticLatestPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.LastPrice));
    
        if (pAlgo->m_pAlgoAutoData->PriceTick != 0.0f)
        {
            double price = (depthMarketDate.BidPrice1+depthMarketDate.AskPrice1)/2;
            if ((int)(10000*price)%(int)(10000*(pAlgo->m_pAlgoAutoData->PriceTick)) != 0)
            {
                switch(m_comboBuysell.GetCurSel())
                {
                case 0://买入        
                    CheckSumPrice(price,pAlgo->m_pAlgoAutoData->PriceTick,-1);             
                    break;
                case 1://卖出
                    CheckSumPrice(price,pAlgo->m_pAlgoAutoData->PriceTick,1);    
                    break;
                }

                if(m_comboBasicPrice.GetCurSel() == 3 && m_bCloseupComboBasicprice)
                    CTools_AlgoTrading::SetDoubleToCEdit(m_editBasicPrice,price);

                if(!util::isInvalidValue(price))
                    CTools_AlgoTrading::SetdoubleToCStatic(m_staticMiddlePrice,price);
            }
        }

        if(!util::isInvalidValue(depthMarketDate.PreSettlementPrice))
            CTools_AlgoTrading::SetStringToCStatic(m_staticYesterdayPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.PreSettlementPrice));
    
        if(m_bCloseupComboBasicprice)
        {
            switch(m_comboBasicPrice.GetCurSel())
            {
            case 0://买价           
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticBuyPrice).c_str());
                break;
            case 1://最新价            
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticLatestPrice).c_str());
                break;
            case 2://卖价           
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticSellPrice).c_str());
                break;
            case 3://中间价            
                //CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticMiddlePrice).c_str());
                break;
            case 4://昨结算          
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticYesterdayPrice).c_str());
                break;
            } 
        }    

        pAlgo->m_pAlgoAutoData->InstrumentName = CTools_AlgoTrading::GetCStringFromCEdit(m_editInstrumentID);
        if (CTools_AlgoTrading::GetStringFromCComboBox(m_comboOpenclose)=="开仓")
            pAlgo->m_pAlgoAutoData->InstrumentOffsetID = 0;
        else if (CTools_AlgoTrading::GetStringFromCComboBox(m_comboOpenclose)=="平今")
            pAlgo->m_pAlgoAutoData->InstrumentOffsetID = 1;
        else 
            pAlgo->m_pAlgoAutoData->InstrumentOffsetID = 2;

        pAlgo->m_pAlgoAutoData->InstrumentDirection = CTools_AlgoTrading::GetStringFromCComboBox(m_comboBuysell)=="买入"?0:1;
        pAlgo->m_pAlgoAutoData->InstrumentCount = CTools_AlgoTrading::GetStringFromCEdit(m_editInstrumentCount);
        pAlgo->m_pAlgoAutoData->LimitPrice = CTools_AlgoTrading::GetStringFromCEdit(m_editPriceLimit);



        m_depthMarketDate = depthMarketDate;

        //行情相关
        PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
        if(pInfo != NULL)
            pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;

        pAlgo->m_pAlgoAutoData->BasicPrice.value = CTools_AlgoTrading::GetStringFromCEdit(m_editBasicPrice);

        pAlgo->m_pAlgoAutoData->NewVolume = depthMarketDate.Volume - m_nCurVolume;
        pAlgo->m_pAlgoAutoData->LatestPrice = depthMarketDate.LastPrice;

        m_nCurVolume = depthMarketDate.Volume;
        pAlgo->m_pAlgoAutoData->BidVolume = m_depthMarketDate.BidVolume1;
        pAlgo->m_pAlgoAutoData->AskVolume = m_depthMarketDate.AskVolume1;
    }
}

void CGeneralParamDlg::OnDeltaposSpinPricelimit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    double fPrice,dVolume = 0.0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editPriceLimit.GetLine(0, strQty, 63);
    dVolume = atof(strQty);

	CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
	if(pAlgo == NULL)
		return;

    PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
    if(pInfo != NULL)
        pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;

    CheckSumPrice(dVolume, pAlgo->m_pAlgoAutoData->PriceTick,0);
    dVolume -= pNMUpDown->iDelta * (pAlgo->m_pAlgoAutoData->PriceTick);

    fPrice = AnalysePrice(dVolume,pAlgo->GetUpperLimitPrice(),pAlgo->GetLowerLimitPrice());

    sprintf_s(strQty, "%.4f", fPrice);
    m_editPriceLimit.SetSel(0, -1);
    m_editPriceLimit.ReplaceSel(strQty);

    *pResult = 0;
}
void CGeneralParamDlg::OnDeltaposSpinMarketdegreed(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int nVolume = 0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editMarketDegreed.GetLine(0, strQty, 63);
    nVolume = atoi(strQty);

    nVolume -= pNMUpDown->iDelta;
    if(nVolume<=0)
        nVolume = 0;
    if(nVolume>100)
        nVolume = 100;
    sprintf_s(strQty, "%d", nVolume);
    m_editMarketDegreed.SetSel(0, -1);
    m_editMarketDegreed.ReplaceSel(strQty);

    m_slidMarketDegreed.SetPos(nVolume);

    *pResult = 0;
}

void CGeneralParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    int nCur=m_slidMarketDegreed.GetPos();
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    sprintf_s(strQty, "%d", nCur);
    m_editMarketDegreed.SetSel(0, -1);
    m_editMarketDegreed.ReplaceSel(strQty);

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGeneralParamDlg::OnCbnDropdownComboBasicprice()
{
    // TODO: Add your control notification handler code here
    m_bCloseupComboBasicprice = FALSE;
}

void CGeneralParamDlg::OnCbnCloseupComboBasicprice()
{
    // TODO: Add your control notification handler code here
    m_bCloseupComboBasicprice = TRUE;
}

void CGeneralParamDlg::OnCbnSelchangeComboBasicprice()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) return;

    if(5==m_comboBasicPrice.GetCurSel())
    {
        m_editBasicPrice.EnableWindow(TRUE);
        m_editBasicPrice.SetWindowText(_T(""));
    }else
        m_editBasicPrice.EnableWindow(FALSE);
}

void CGeneralParamDlg::OnEnChangeEditMarketdegreed()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    int nVolume = 0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editMarketDegreed.GetLine(0, strQty, 63);
    nVolume = atoi(strQty);

    m_slidMarketDegreed.SetPos(nVolume);
}

void CGeneralParamDlg::OnBnClickedBtnOk()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL)
        return;

    if(!CheckParam()) 
        return; 

    m_bOkClicked = TRUE;
    pAlgo->QueryPositionInfo(GetInstrumentID());
    //pAlgo->StartTrading();    
}

BOOL CGeneralParamDlg::CheckParam(void)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) 
        return FALSE;

    CString strTemp,strTemp1,strTemp2,strTemp3,strTemp4,strTemp5,Temp1,Temp2,Temp3,Temp4,Temp5;  
    CTime startTime,endTime;

    m_editInstrumentID.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "交易合约为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (CTools_AlgoTrading::IsHaveDot(strTemp))
    {
        ::MessageBox(NULL, "交易合约非法字符，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    m_editInstrumentCount.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "交易数量为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (CTools_AlgoTrading::IsHaveDot(strTemp))
    {
        ::MessageBox(NULL, "交易数量不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    m_editPriceLimit.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "成本限价为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }
    
    double fPrice=CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceLimit);
    if(CheckSumPrice(fPrice, pAlgo->m_pAlgoAutoData->PriceTick))
    {
        ::MessageBox(NULL, "成本限价不是最小变动价格整数倍，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }
    //fPrice = AnalysePrice(fPrice,pAlgo->m_tDepthMarketData.UpperLimitPrice,pAlgo->m_tDepthMarketData.LowerLimitPrice);
    

	m_editInstrumentStartTime.GetTime(startTime);
	m_editInstrumentEndTime.GetTime(endTime);
	if (endTime <= startTime)
	{
 		::MessageBox(NULL, "交易时间设置有误，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
 		return FALSE;
	}

    m_editMarketDegreed.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "市场参与度为空，请设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }else
    {
        if (CTools_AlgoTrading::IsHaveDot(strTemp))
        {
            ::MessageBox(NULL, "市场参与度不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    m_editIdleTime.GetWindowText(Temp1); 
    m_editExecuteTime.GetWindowText(Temp2);
    m_editOverTimeRetryCount.GetWindowText(Temp3);
    m_editOverTimeTotal.GetWindowText(Temp4);

    if (!Temp1.IsEmpty())
    {
        if(CTools_AlgoTrading::IsHaveDot(Temp1))
        {
            ::MessageBox(NULL, "空闲时间不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    if (!Temp2.IsEmpty())
    {
        if(CTools_AlgoTrading::IsHaveDot(Temp2))
        {
            ::MessageBox(NULL, "执行时间不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    if((BST_UNCHECKED == m_checkOverTimeRetryCount.GetCheck())?FALSE:TRUE)
    {
        if (Temp3.IsEmpty())
        {
            ::MessageBox(NULL, "选择超时重试次数后，必须设置具体的数值", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if(CTools_AlgoTrading::IsHaveDot(Temp3))
        {
            ::MessageBox(NULL, "超时重试次数不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if (Temp1.IsEmpty() && Temp2.IsEmpty())
        {
            ::MessageBox(NULL, "选择超时重试次数后，必须设置空闲或执行时间", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    if((BST_UNCHECKED == m_checkOverTimeTotal.GetCheck())?FALSE:TRUE )
    {
        if (Temp4.IsEmpty())
        {
            ::MessageBox(NULL, "选择超时重试总时长后，必须设置具体的数值", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if(CTools_AlgoTrading::IsHaveDot(Temp4))
        {
            ::MessageBox(NULL, "超时重试总时长不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if (Temp1.IsEmpty() && Temp2.IsEmpty() )
        {
            ::MessageBox(NULL, "选择超时重试总时长后，必须设置空闲或执行时间", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    m_editOrderCount.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "下单数量为空，请输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }else
    {
        if(CTools_AlgoTrading::IsHaveDot(strTemp))
        {
            ::MessageBox(NULL, "下单数量不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

//     m_editResetOrderCount.GetWindowText(strTemp);
//     if(CTools_AlgoTrading::IsHaveDot(strTemp))
//     {
//         ::MessageBox(NULL, "重置数量不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//         return FALSE;
//     }

    int nOrderCount;
    if(1 == m_comboOrderCount.GetCurSel())//手数
        nOrderCount = CTools_AlgoTrading::GetIntFromCEdit(m_editOrderCount);
    else if(0 == m_comboOrderCount.GetCurSel())//百分比
    {
        if(0 == m_comboBuysell.GetCurSel())//买入
        {
            nOrderCount = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * pAlgo->m_pAlgoAutoData->AskVolume);
        }else if(1 == m_comboBuysell.GetCurSel())//卖入        
        {
            nOrderCount = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * pAlgo->m_pAlgoAutoData->BidVolume);
        }
    }    

//     int nResetOrderCount;
// 	if(1 == m_comboResetOrderCount.GetCurSel())//手数
// 	{    
// 		nResetOrderCount = CTools_AlgoTrading::GetIntFromCEdit(m_editResetOrderCount);
// 
//         if (nResetOrderCount > nOrderCount)
//         {
//             ::MessageBox(NULL, "重置数量不应该超过下单数量，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//             return FALSE;
//         }
// 	}else if(0 == m_comboResetOrderCount.GetCurSel())//百分比
// 	{
// 		nResetOrderCount = (int)(CTools_AlgoTrading::GetDoubleFromCEdit(m_editResetOrderCount)/100.0f * nOrderCount);
// 	}  

    int nTotalCount = CTools_AlgoTrading::GetIntFromCEdit(m_editInstrumentCount);
    if (nOrderCount > nTotalCount)
    {
        ::MessageBox(NULL, "下单数量不能超过总数量，请重新设置市场参与度", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

//     if(1 == m_comboResetOrderCount.GetCurSel())//手数
//     {
//         m_editResetOrderCount.GetWindowText(strTemp);
//         if (CTools_AlgoTrading::IsHaveDot(strTemp))
//         {
//             ::MessageBox(NULL, "重置数量在点位下不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//             return FALSE;
//         }
//     }

//     m_editSlidPriceSpread1.GetWindowText(Temp1);
    m_comboMultiPrice.GetLBText(m_comboMultiPrice.GetCurSel(),Temp1);
//     m_editSlidPriceSpread2.GetWindowText(Temp2);
//     m_editSlidPriceSpread3.GetWindowText(Temp3);
//     m_editSlidPriceSpread4.GetWindowText(Temp4);
//     m_editSlidPriceSpread5.GetWindowText(Temp5);
    
//     if(1 == m_comboSlidPriceSpread.GetCurSel())//点位
//     {
        if (CTools_AlgoTrading::IsHaveDot(Temp1)/*||
            CTools_AlgoTrading::IsHaveDot(Temp2)||
            CTools_AlgoTrading::IsHaveDot(Temp3)||
            CTools_AlgoTrading::IsHaveDot(Temp4)||
            CTools_AlgoTrading::IsHaveDot(Temp5)*/)
        {
            ::MessageBox(NULL, "对应下单量档位在点位下不支持小数，请重新输入", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }         

//     }


//     double value1 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount1);
//     double value2 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount2);
//     double value3 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount3);
//     double value4 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount4);
//     double value5 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount5);

//     switch(m_comboRelateOrderCount.GetCurSel())
//     {
//     case 0://百分比
//         if ((value1+value2+value3+value4+value5) != 100.0f)
//         {
//             ::MessageBox(NULL, "对应下单量百分比总数不等于100%，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//             return FALSE;
//         }
//         break;
//     case 1://手数
//         if (1 == m_comboOrderCount.GetCurSel())//手数
//         {
//             if((int)(value1+value2+value3+value4+value5) != CTools_AlgoTrading::GetIntFromCEdit(m_editOrderCount))
//             {
//                 ::MessageBox(NULL, "对应下单量手数总数不等于下单数量，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//                 return FALSE;
//             }
//         }else if(0 == m_comboOrderCount.GetCurSel())//百分比
//         {
//             int nOrder = 0;
//             switch(m_comboBuysell.GetCurSel())
//             {
//             case 0://买入
//                 nOrder = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * m_depthMarketDate.AskVolume1);
//                 break;
//             case 1://卖出
//                 nOrder = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * m_depthMarketDate.BidVolume1);
//             }
// 
//             if((int)(value1+value2+value3+value4+value5) > nOrder)
//             {
//                 ::MessageBox(NULL, "对应下单量百分比总数应该等于100%，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//                 return FALSE;
//             }
//         }
//         break;
//     }

// 	m_editRelateOrderCount1.GetWindowText(strTemp1);
// 	m_editRelateOrderCount2.GetWindowText(strTemp2);
// 	m_editRelateOrderCount3.GetWindowText(strTemp3);
// 	m_editRelateOrderCount4.GetWindowText(strTemp4);
// 	m_editRelateOrderCount5.GetWindowText(strTemp5);

// 	if (Temp1.IsEmpty() && !strTemp1.IsEmpty() || !Temp1.IsEmpty() && strTemp1.IsEmpty())
// 	{
// 		::MessageBox(NULL, "滑动价差与下单量要成对出现，请填写完整", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}
//     else if (Temp2.IsEmpty() && !strTemp2.IsEmpty() || !Temp2.IsEmpty() && strTemp2.IsEmpty())
// 	{
// 		::MessageBox(NULL, "滑动价差与下单量要成对出现，请填写完整", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}else if (Temp3.IsEmpty() && !strTemp3.IsEmpty() || !Temp3.IsEmpty() && strTemp3.IsEmpty())
// 	{
// 		::MessageBox(NULL, "滑动价差与下单量要成对出现，请填写完整", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}else if (Temp4.IsEmpty() && !strTemp4.IsEmpty() || !Temp4.IsEmpty() && strTemp4.IsEmpty())
// 	{
// 		::MessageBox(NULL, "滑动价差与下单量要成对出现，请填写完整", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}else if (Temp5.IsEmpty() && !strTemp5.IsEmpty() || !Temp5.IsEmpty() && strTemp5.IsEmpty())
// 	{
// 		::MessageBox(NULL, "滑动价差与下单量要成对出现，请填写完整", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}

    return TRUE;
}

void CGeneralParamDlg::AddToolTip()
{
    EnableToolTips(TRUE);
    m_toolTip.Create(this);
    m_toolTip.Activate(TRUE);
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_INSTRUMENTID),"交易合约，合约输入框，可以修改合约。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_INSTRUMENTID),"交易合约。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_BUYSELL),"交易方向。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_BUYSELL),"交易方向：买入或卖出。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_OPENCLOSE),"开平：开仓、平仓或平今。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_INSTRUMENTCOUNT),"目标值：系统运行期望达到的成交总数。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_INSTRUMENTCOUNT),"目标值：系统运行期望达到的成交总数。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_PRICELIMIT),"对资金成本进行限制，买入方向就是最高值，卖出方向就是最低值。如果超出则调整为此值下单");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_PRICELIMIT),"对资金成本进行限制，买入方向就是最高值，卖出方向就是最低值。如果超出则调整为此值下单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_START),"交易开始时间。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_DATETIMEPICKER_START),"交易开始时间。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_END),"交易结束时间。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_DATETIMEPICKER_END),"交易结束时间。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_MARKETDEGREED),"单次下单量不能超过市场参与度与现量的乘积，如果设置为0则代表不限制。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_MARKETDEGREED),"单次下单量不能超过市场参与度与现量的乘积，如果设置为0则代表不限制。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_ORDERCOUNT),"每次下单数量。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_ORDERCOUNT),"每次下单数量。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_RESETORDERCOUNT),"当已成交数量达到重置数量时，未成交单数被撤销，重新按当前盘口情况挂单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_RESETORDERCOUNT),"当已成交数量达到重置数量时，未成交单数被撤销，重新按当前盘口情况挂单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_BASICPRICE),"表示每次下单的价格的基数值。所有最终下单价格均以此为基数调整滑动价差的数值后，再判断是否有超过限价，如未超过则以此价格下单。如果超过限价则以限价价格进行下单操作。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_IDLETIME),"若在指定的时间内无成交，则撤销挂单重新下单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_IDLETIME),"若在指定的时间内无成交，则撤销挂单重新下单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_EXECUTETIME),"如果每笔挂单执行时间超过指定的时间，则撤销当前挂单，开始新一轮下单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_EXECUTETIME),"如果每笔挂单执行时间超过指定的时间，则撤销当前挂单，开始新一轮下单。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_OVERRETRYCOUNT),"如果勾选此项，判断超时重试次数累加值，如超过本设定值，则系统运行停止，未成交单撤单。未达到交易总量，交易失败");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_OVERRETRYTOTAL),"如果勾选此项，判断由于空闲时间或执行时间而导致的撤单并重新下单的总时长，如超过本设定值，则系统运行停止，未成交单撤单。未达到交易总量，交易失败");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_SLIDPRICESPREAD),"滑动价差下拉菜单分为 百分比/点位 选择为点位时，则具体下单价格为 盯盘价格+价差*最小变动价位;如果选择为百分比，则下单价格为 盯盘价格*（1+滑动价差）根据合约的最小变动价位整数倍取最接近的值。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_SLIDPRICESPREAD),"滑动价差下拉菜单分为 百分比/点位 选择为点位时，则具体下单价格为 盯盘价格+价差*最小变动价位;如果选择为百分比，则下单价格为 盯盘价格*（1+滑动价差）根据合约的最小变动价位整数倍取最接近的值。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_RESETORDERCOUNT2),"对应下单量分为“百分比”“手数”两种,如果选择是百分比,则需要保证所有数字和为100;如果选择的是手数，则只能填写正整数。");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_RESETORDERCOUNT2),"对应下单量分为“百分比”“手数”两种,如果选择是百分比,则需要保证所有数字和为100;如果选择的是手数，则只能填写正整数。");
}

void CGeneralParamDlg::GetAlgoParamFromWnd(void)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) 
        return;

    //界面相关
    CString strTemp;
    pAlgo->m_pAlgoAutoData->InstrumentName = CTools_AlgoTrading::GetStringFromCEdit(m_editInstrumentID);    
    pAlgo->m_pAlgoAutoData->InstrumentDirection = m_comboBuysell.GetCurSel();
    pAlgo->m_pAlgoAutoData->InstrumentOffsetID = m_comboOpenclose.GetCurSel();
    pAlgo->m_pAlgoAutoData->InstrumentCount = CTools_AlgoTrading::GetStringFromCEdit(m_editInstrumentCount);
    pAlgo->m_pAlgoAutoData->LimitPrice = CTools_AlgoTrading::GetStringFromCEdit(m_editPriceLimit);
    m_editInstrumentStartTime.GetTime(pAlgo->m_pAlgoAutoData->StartTime); 
    m_editInstrumentEndTime.GetTime(pAlgo->m_pAlgoAutoData->EndTime); 
    pAlgo->m_pAlgoAutoData->MarketDegreed = CTools_AlgoTrading::GetIntFromCEdit(m_editMarketDegreed);
    pAlgo->m_pAlgoAutoData->OrderCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_editOrderCount);
    pAlgo->m_pAlgoAutoData->OrderCount.type = m_comboOrderCount.GetCurSel();
//     pAlgo->m_pAlgoAutoData->ResetCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_editResetOrderCount);
//     pAlgo->m_pAlgoAutoData->ResetCount.type = m_comboResetOrderCount.GetCurSel();
    pAlgo->m_pAlgoAutoData->BasicPrice.value = CTools_AlgoTrading::GetStringFromCEdit(m_editBasicPrice);
    pAlgo->m_pAlgoAutoData->BasicPrice.type = m_comboBasicPrice.GetCurSel();
    pAlgo->m_pAlgoAutoData->IdleTime.value = CTools_AlgoTrading::GetStringFromCEdit(m_editIdleTime);
    pAlgo->m_pAlgoAutoData->IdleTime.type = m_comboIdleTime.GetCurSel();
    pAlgo->m_pAlgoAutoData->ExecuteTime.value = CTools_AlgoTrading::GetStringFromCEdit(m_editExecuteTime);
    pAlgo->m_pAlgoAutoData->ExecuteTime.type = m_comboExecuteTime.GetCurSel();
    pAlgo->m_pAlgoAutoData->RetryCount = CTools_AlgoTrading::GetStringFromCEdit(m_editOverTimeRetryCount);
    pAlgo->m_pAlgoAutoData->RetryTotalTime.value = CTools_AlgoTrading::GetStringFromCEdit(m_editOverTimeTotal);
    pAlgo->m_pAlgoAutoData->RetryTotalTime.type = m_comboOverTimeTotal.GetCurSel();
    pAlgo->m_pAlgoAutoData->bActiceOpt = (BST_CHECKED == m_checkActiveOptimize.GetCheck())?true:false;
    pAlgo->m_pAlgoAutoData->OptimizeIndex = m_comboActiveOptimize.GetCurSel();
//     pAlgo->m_pAlgoAutoData->bMultiPrice = (BST_CHECKED == m_btnCheckMultiPrice.GetCheck())?true:false;

    m_editIdleTime.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
        pAlgo->m_pAlgoAutoData->bIdleTime = FALSE;
    else
        pAlgo->m_pAlgoAutoData->bIdleTime = TRUE;

    m_editExecuteTime.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
        pAlgo->m_pAlgoAutoData->bExecuteTime = FALSE;
    else
        pAlgo->m_pAlgoAutoData->bExecuteTime = TRUE;

//     pAlgo->m_pAlgoAutoData->bMultiPrice = (BST_CHECKED == m_btnCheckMultiPrice.GetCheck())?true:false;

    //五档
    pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.clear();
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.clear();
    
    CString str0,str1,str2,str3,str4;
//     m_editSlidPriceSpread1.GetWindowText(str0);
    int nIndex = m_comboMultiPrice.GetCurSel();
    if(nIndex>0)
        m_comboMultiPrice.GetLBText(nIndex,str0);
//     m_editSlidPriceSpread2.GetWindowText(str1);
//     m_editSlidPriceSpread3.GetWindowText(str2);
//     m_editSlidPriceSpread4.GetWindowText(str3);
//     m_editSlidPriceSpread5.GetWindowText(str4);
    

    pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.index = 1;//m_comboSlidPriceSpread.GetCurSel();
    pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str0.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str1.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str2.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str3.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str4.GetBuffer(0));

//     str0.ReleaseBuffer();
//     str1.ReleaseBuffer();
//     str2.ReleaseBuffer();
//     str3.ReleaseBuffer();
//     str4.ReleaseBuffer();

//     m_editRelateOrderCount1.GetWindowText(str0);
//     m_editRelateOrderCount2.GetWindowText(str1);
//     m_editRelateOrderCount3.GetWindowText(str2);
//     m_editRelateOrderCount4.GetWindowText(str3);
//     m_editRelateOrderCount5.GetWindowText(str4);
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.index = m_comboRelateOrderCount.GetCurSel();//0;
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str0.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str1.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str2.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str3.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str4.GetBuffer(0));

    str0.ReleaseBuffer();
//     str1.ReleaseBuffer();
//     str2.ReleaseBuffer();
//     str3.ReleaseBuffer();
//     str4.ReleaseBuffer();

    pAlgo->m_pAlgoAutoData->bActiceOverTime = (BST_UNCHECKED == m_checkOverTimeRetryCount.GetCheck())?FALSE:TRUE;
    pAlgo->m_pAlgoAutoData->bActiceTotalTime = (BST_UNCHECKED == m_checkOverTimeTotal.GetCheck())?FALSE:TRUE;

    //行情相关
    PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
    if(pInfo != NULL)
        pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;
}


void CGeneralParamDlg::SetAlgoParamToWnd(BOOL bEditChanged/* = FALSE*/)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) 
        return;

    if(!bEditChanged)
        CTools_AlgoTrading::SetStringToCEdit(m_editInstrumentID,pAlgo->m_pAlgoAutoData->InstrumentName.c_str());
    m_comboBuysell.SetCurSel(pAlgo->m_pAlgoAutoData->InstrumentDirection);
    m_comboOpenclose.SetCurSel(pAlgo->m_pAlgoAutoData->InstrumentOffsetID);
    CTools_AlgoTrading::SetStringToCEdit(m_editInstrumentCount,pAlgo->m_pAlgoAutoData->InstrumentCount.c_str());
    CTools_AlgoTrading::SetStringToCEdit(m_editPriceLimit,pAlgo->m_pAlgoAutoData->LimitPrice.c_str());
    m_spinMarketDegreed.SetPos(pAlgo->m_pAlgoAutoData->MarketDegreed);
    CTools_AlgoTrading::SetIntToCEdit(m_editMarketDegreed,pAlgo->m_pAlgoAutoData->MarketDegreed);
    CTools_AlgoTrading::SetStringToCEdit(m_editOrderCount,pAlgo->m_pAlgoAutoData->OrderCount.value.c_str());
    m_comboOrderCount.SetCurSel(pAlgo->m_pAlgoAutoData->OrderCount.type);
//     CTools_AlgoTrading::SetStringToCEdit(m_editResetOrderCount,pAlgo->m_pAlgoAutoData->ResetCount.value.c_str());
//     m_comboResetOrderCount.SetCurSel(pAlgo->m_pAlgoAutoData->ResetCount.type);
//     m_comboResetOrderCount.EnableWindow(pAlgo->m_pAlgoAutoData->OrderCount.type);
    m_comboBasicPrice.SetCurSel(pAlgo->m_pAlgoAutoData->BasicPrice.type);
    m_editBasicPrice.EnableWindow((5==pAlgo->m_pAlgoAutoData->BasicPrice.type)?TRUE:FALSE);
    //填入数据
    BOOL bHaveData = FALSE;
    vector<OPTPARAM*>::iterator iter = pAlgo->m_pAlgoAutoData->vecOptInfo.begin();
    for (;iter!=pAlgo->m_pAlgoAutoData->vecOptInfo.end();iter++)
    {
        m_comboActiveOptimize.AddString((*iter)->Name.c_str());
        bHaveData = TRUE;
    }
    m_checkActiveOptimize.EnableWindow(bHaveData);  
    m_comboActiveOptimize.EnableWindow(bHaveData); 

    if (pAlgo->m_pAlgoAutoData->bActiceOpt)    
        m_checkActiveOptimize.SetCheck(BST_CHECKED);        
    else
        m_checkActiveOptimize.SetCheck(BST_UNCHECKED); 

    m_comboActiveOptimize.SetCurSel(pAlgo->m_pAlgoAutoData->OptimizeIndex);

    CTools_AlgoTrading::SetStringToCEdit(m_editIdleTime,pAlgo->m_pAlgoAutoData->IdleTime.value.c_str());
    m_comboIdleTime.SetCurSel(pAlgo->m_pAlgoAutoData->IdleTime.type);
    CTools_AlgoTrading::SetStringToCEdit(m_editExecuteTime,pAlgo->m_pAlgoAutoData->ExecuteTime.value.c_str());
    m_comboExecuteTime.SetCurSel(pAlgo->m_pAlgoAutoData->ExecuteTime.type);    
    
    if(pAlgo->m_pAlgoAutoData->bActiceOverTime)
    {
        m_checkOverTimeRetryCount.SetCheck(BST_CHECKED);
    }else
    {
        m_checkOverTimeRetryCount.SetCheck(BST_UNCHECKED);
    }
    
    CTools_AlgoTrading::SetStringToCEdit(m_editOverTimeRetryCount,pAlgo->m_pAlgoAutoData->RetryCount.c_str());
    CTools_AlgoTrading::SetStringToCEdit(m_editOverTimeTotal,pAlgo->m_pAlgoAutoData->RetryTotalTime.value.c_str());
    m_comboOverTimeTotal.SetCurSel(pAlgo->m_pAlgoAutoData->RetryTotalTime.type);

    if(pAlgo->m_pAlgoAutoData->bActiceTotalTime)
    {
        m_checkOverTimeTotal.SetCheck(BST_CHECKED);
    }else
    {
        m_checkOverTimeTotal.SetCheck(BST_UNCHECKED);
    }

//     if (pAlgo->m_pAlgoAutoData->bMultiPrice)
//         m_btnCheckMultiPrice.SetCheck(BST_CHECKED);
//     else
//         m_btnCheckMultiPrice.SetCheck(BST_UNCHECKED);   

//     EnableMultiPriceControls(pAlgo->m_pAlgoAutoData->bMultiPrice);


    //五档
    vector<string> vecPriceSpread = pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data;
//     vector<string> vecOrderCount = pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data;

//     m_comboSlidPriceSpread.SetCurSel(pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.index);
//     for (int i=0;i<(int)vecPriceSpread.size();i++)
//     {
//         if(i == 0)
//             m_editSlidPriceSpread1.SetWindowText(vecPriceSpread[i].c_str()); 
            int nIndex = m_comboMultiPrice.FindStringExact( 0, vecPriceSpread[0].c_str());
            m_comboMultiPrice.SetCurSel((nIndex<0)?5:nIndex);
//         else if(i == 1)
//             m_editSlidPriceSpread2.SetWindowText(vecPriceSpread[i].c_str());
//         else if(i == 2)
//             m_editSlidPriceSpread3.SetWindowText(vecPriceSpread[i].c_str());
//         else if(i == 3)
//             m_editSlidPriceSpread4.SetWindowText(vecPriceSpread[i].c_str());
//         else if(i == 4)
//             m_editSlidPriceSpread5.SetWindowText(vecPriceSpread[i].c_str());
// 
//     }

//     m_comboRelateOrderCount.SetCurSel(pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.index);
//     m_comboRelateOrderCount.EnableWindow(pAlgo->m_pAlgoAutoData->OrderCount.type);    
//     for (int i=0;i<(int)vecOrderCount.size();i++)
//     {
//         if(i == 0)
//             m_editRelateOrderCount1.SetWindowText(vecOrderCount[0].c_str());
//         else if(i == 1)
//             m_editRelateOrderCount2.SetWindowText(vecOrderCount[i].c_str());
//         else if(i == 2)
//             m_editRelateOrderCount3.SetWindowText(vecOrderCount[i].c_str());
//         else if(i == 3)
//             m_editRelateOrderCount4.SetWindowText(vecOrderCount[i].c_str());
//         else if(i == 4)
//             m_editRelateOrderCount5.SetWindowText(vecOrderCount[i].c_str());
//     }
}


void CGeneralParamDlg::UpdataOptCombo(int optIndex,BOOL bHaveItem)
{
    if (bHaveItem)
    {
        CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
        if(pAlgo == NULL) return;

        SINGLEOPT *pSingle = NULL;

        m_comboActiveOptimize.ResetContent();
        vector<OPTPARAM*>::iterator iter = pAlgo->m_pAlgoAutoData->vecOptInfo.begin();
        for (;iter!=pAlgo->m_pAlgoAutoData->vecOptInfo.end();iter++)
        {
            m_comboActiveOptimize.AddString((*iter)->Name.c_str());
        }

        int nIndex = (optIndex == -1)?0:optIndex;
        m_comboActiveOptimize.SetCurSel(nIndex);//选择优化参数
        pAlgo->m_pAlgoAutoData->OptimizeIndex = nIndex;

        m_comboActiveOptimize.EnableWindow(TRUE);
        m_checkActiveOptimize.EnableWindow(TRUE);
    }else
    {
        m_comboActiveOptimize.ResetContent();
        m_comboActiveOptimize.EnableWindow(FALSE);
        m_checkActiveOptimize.EnableWindow(FALSE);
        m_checkActiveOptimize.SetCheck(BST_UNCHECKED);
    }
}

void CGeneralParamDlg::OnEnChangeEditInstrumentid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
    m_staticBuyPrice.SetWindowText("—");
    m_staticSellPrice.SetWindowText("—");
    m_staticMiddlePrice.SetWindowText("—");
    m_staticYesterdayPrice.SetWindowText("—");
    m_staticLatestPrice.SetWindowText("—");

	CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
	if(pAlgo == NULL) return;

	char strInstrumentID[64];
	ZeroMemory(strInstrumentID, sizeof(strInstrumentID));
	m_editInstrumentID.GetLine(0, strInstrumentID, sizeof(strInstrumentID)-1);


    pAlgo->UnsubscribeQuot(pAlgo->GetOldInstrumentID());
	pAlgo->QueryInstrumentInfo(CString(strInstrumentID));
	pAlgo->SubscribeQuot(CString(strInstrumentID));
	pAlgo->QueryQuotSnapshot(CString(strInstrumentID));	
	pAlgo->SetOldInstrumentID(CString(strInstrumentID));

    if(pAlgo->m_pAlgoAutoData->ReadIni((string)strInstrumentID))
        SetAlgoParamToWnd(TRUE);
}

void CGeneralParamDlg::OnBnClickedBtnCancel()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo != NULL)
    {
        CMessageBoxDialog dlg(this);
        if(!pAlgo->IsTradeTime())
            dlg.m_Text = "交易未完成，非交易时间段停止交易，撤单将不被受理！";
        else
            dlg.m_Text = "交易未完成，所有挂单将撤销！";
        if(dlg.DoModal() == IDOK)  
        {
            pAlgo->StopTrading();
        }
        else
            return;    
    }    
}

void CGeneralParamDlg::EnableCtrls(bool bEnable)
{
    m_btnPlay.EnableWindow(bEnable);
    m_btnOptSetup.EnableWindow(bEnable);
    m_editInstrumentID.EnableWindow(bEnable);
    m_comboBuysell.EnableWindow(bEnable);
    m_comboOpenclose.EnableWindow(bEnable);
    m_editInstrumentCount.EnableWindow(bEnable);
    m_editPriceLimit.EnableWindow(bEnable);
    m_spinPriceLimit.EnableWindow(bEnable);
    m_editInstrumentStartTime.EnableWindow(bEnable);
    m_editInstrumentEndTime.EnableWindow(bEnable);
    m_slidMarketDegreed.EnableWindow(bEnable);
    m_editMarketDegreed.EnableWindow(bEnable);
    m_spinMarketDegreed.EnableWindow(bEnable);
    m_editOrderCount.EnableWindow(bEnable);
    m_comboOrderCount.EnableWindow(bEnable);
//     m_editResetOrderCount.EnableWindow(bEnable);
//     m_comboResetOrderCount.EnableWindow(bEnable);
    m_comboBasicPrice.EnableWindow(bEnable);
    m_editBasicPrice.EnableWindow(bEnable);
//     m_comboSlidPriceSpread.EnableWindow(bEnable);
    m_comboMultiPrice.EnableWindow(bEnable);
//     m_editSlidPriceSpread1.EnableWindow(bEnable);
    m_editSlidPriceSpread2.EnableWindow(bEnable);
    m_editSlidPriceSpread3.EnableWindow(bEnable);
    m_editSlidPriceSpread4.EnableWindow(bEnable);
    m_editSlidPriceSpread5.EnableWindow(bEnable);
//     m_comboRelateOrderCount.EnableWindow(bEnable);
//     m_editRelateOrderCount1.EnableWindow(bEnable);
    m_editRelateOrderCount2.EnableWindow(bEnable);
    m_editRelateOrderCount3.EnableWindow(bEnable);
    m_editRelateOrderCount4.EnableWindow(bEnable);
    m_editRelateOrderCount5.EnableWindow(bEnable);    
    m_comboIdleTime.EnableWindow(bEnable);
    m_comboExecuteTime.EnableWindow(bEnable);
    m_comboOverTimeTotal.EnableWindow(bEnable);
    m_editIdleTime.EnableWindow(bEnable);
    m_editExecuteTime.EnableWindow(bEnable);
    m_checkOverTimeRetryCount.EnableWindow(bEnable);
    m_editOverTimeRetryCount.EnableWindow(bEnable);
    m_checkOverTimeTotal.EnableWindow(bEnable);
    m_editOverTimeTotal.EnableWindow(bEnable);
    m_btnCheckMultiPrice.EnableWindow(bEnable);

    if(!m_btnCheckMultiPrice.GetCheck())
        EnableMultiPriceControls(false);

//     if (0 == m_comboOrderCount.GetCurSel())//百分比
//     {
//         m_comboResetOrderCount.SetCurSel(0);
//         m_comboResetOrderCount.EnableWindow(FALSE);

//         m_comboRelateOrderCount.SetCurSel(0);
//         m_comboRelateOrderCount.EnableWindow(FALSE);
//     }else
//     {
//         if (bEnable)
//         {
//             m_comboResetOrderCount.EnableWindow(TRUE);
//             m_comboRelateOrderCount.EnableWindow(TRUE);
//         } 
//     }

    if (bEnable)
    {
        if (m_comboActiveOptimize.GetCount() > 0)
        { 
            m_checkActiveOptimize.EnableWindow(bEnable);
            m_comboActiveOptimize.EnableWindow(bEnable);
        }

        m_btnPlay.SetWindowText("重新开始");
    }else
    {
        m_checkActiveOptimize.EnableWindow(bEnable);
        m_comboActiveOptimize.EnableWindow(bEnable);
        m_btnPlay.SetWindowText("开始执行");
    }
}

void CGeneralParamDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	if (m_staticSplit.GetSafeHwnd() != NULL)
	{
		CPen pen(PS_SOLID,1,RGB(192,192,192));
		CPen *oldpen=dc.SelectObject(&pen);

		RECT rect;
		m_staticSplit.GetWindowRect(&rect);
		ScreenToClient(&rect);
		int wid = rect.right - rect.left;

		dc.MoveTo(rect.left,rect.top+18);
		dc.LineTo(rect.left+wid,rect.top+18);
		dc.SelectObject(oldpen);
	}
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CGeneralParamDlg::OnCbnSelchangeComboOrdercount()
{
    // TODO: Add your control notification handler code here
//     if (0 == m_comboOrderCount.GetCurSel())//百分比
//     {
//         m_comboResetOrderCount.SetCurSel(0);
//         m_comboResetOrderCount.EnableWindow(FALSE);

//         m_comboRelateOrderCount.SetCurSel(0);
//         m_comboRelateOrderCount.EnableWindow(FALSE);
//     }else
//     {
//         m_comboResetOrderCount.EnableWindow(TRUE);
//         m_comboRelateOrderCount.EnableWindow(TRUE);
//     }
}

BOOL CGeneralParamDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (NULL != m_toolTip.GetSafeHwnd())
    {
        m_toolTip.RelayEvent(pMsg);
    }

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)  
        return TRUE;
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
        return TRUE;

    return CDialog::PreTranslateMessage(pMsg);
}
void CGeneralParamDlg::OnBnClickedCheckMultiPrice()
{
    // TODO: Add your control notification handler code here
    EnableMultiPriceControls(m_btnCheckMultiPrice.GetCheck());
}

void CGeneralParamDlg::EnableMultiPriceControls(BOOL bEnable)
{
    m_editSlidPriceSpread2.EnableWindow(bEnable);
    m_editSlidPriceSpread3.EnableWindow(bEnable);
    m_editSlidPriceSpread4.EnableWindow(bEnable);
    m_editSlidPriceSpread5.EnableWindow(bEnable);
    m_editRelateOrderCount2.EnableWindow(bEnable);
    m_editRelateOrderCount3.EnableWindow(bEnable);
    m_editRelateOrderCount4.EnableWindow(bEnable);
    m_editRelateOrderCount5.EnableWindow(bEnable);
}

