// QuickOderModuleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "OptionStrategyDlg.h"
#include "QuickOrderModuleDialog.h"
#include "CommPublic.h"
#include "StrategyDecisionMaking.h"
#include "OrderFunc.h"


#include <gdiplus.h>
using namespace Gdiplus;


// CQuickOrderModuleDialog dialog

IMPLEMENT_DYNAMIC(CQuickOrderModuleDialog, CDialog)

CQuickOrderModuleDialog::CQuickOrderModuleDialog(CWnd* pParent /*=NULL*/)
	: CBKDialog(CQuickOrderModuleDialog::IDD, pParent)
    , m_bInitDialog(FALSE)
{
}

CQuickOrderModuleDialog::~CQuickOrderModuleDialog()
{
    PlatformStru_OrderInfo* pOrderInfo = NULL;
    for(set<PlatformStru_OrderInfo*>::iterator it = m_setOrderInfo.begin();it != m_setOrderInfo.end();it++)
    {
        pOrderInfo = *it;
        if(pOrderInfo)
        {
            delete pOrderInfo;
            pOrderInfo = NULL;
        }
    } 
    if(m_listStrategy.GetSafeHwnd())
        m_listStrategy.free();
}

void CQuickOrderModuleDialog::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_ORDER, m_listStrategy);
    DDX_Control(pDX, IDC_CUSTOM_THUMBNAIL, m_cThumbFrame);
    DDX_Control(pDX, IDC_BUTTON_ORDER, m_btnOrder);
}


BEGIN_MESSAGE_MAP(CQuickOrderModuleDialog, CBKDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_ORDER, &CQuickOrderModuleDialog::OnBnClickedButtonOrder)
    ON_REGISTERED_MESSAGE( UWM_ON_STRATEGY_BTN_LCLICKED, OnStrategyBtnLClicked )
    ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION, OnComboSelection)
    ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED, OnCheckbox)
    ON_REGISTERED_MESSAGE(WM_XLISTCTRL_DELETEIMG_CLICKED, OnDeleteImgClicked)
    ON_REGISTERED_MESSAGE(WM_XLISTCTRL_EDIT_END, OnEditEnd)
    ON_MESSAGE(WM_USER_VOLATILITY_UPDATE,OnVolatilityUpdate)
    ON_MESSAGE(WM_USER_POSITION_UPDATE,OnPositionUpdate)
END_MESSAGE_MAP()


// CQuickOrderModuleDialog message handlers

BOOL CQuickOrderModuleDialog::OnInitDialog()
{
    CBKDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    SetBitmap(IDB_BITMAPOptionStrategyBK);

    //the two param mean bmp's width and height
    m_cThumbFrame.InitializeVariables( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT );

    //init btn
    m_btnOrder.SetBitmaps(IDB_BITMAPBtnOrderBK, RGB(0, 255, 0), (int)BTNST_AUTO_DARKER);
    m_btnOrder.DrawTransparent();

    //////////////////////////////////////////////////////////////////////////
    m_listStrategy.ModifyStyle(0, LVS_REPORT/*|LVS_NOSCROLL*/);
    DWORD dwExStyle = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES/*|LVS_SHOWSELALWAYS*/;
    m_listStrategy.SetExtendedStyle(dwExStyle);
    m_listStrategy.SetBKColorMode(m_ColorMode); 
    m_listStrategy.SetHeaderTextColor(RGB(255,255,255));
    m_listStrategy.SetRowHeight(24);
    m_listStrategy.m_HeaderCtrl.SetListCtrl(&m_listStrategy);
    m_listStrategy.m_HeaderCtrl.SetBkColor((m_ColorMode==Color_Light)?RGB(72,129,174):RGB(38,73,104));
    m_listStrategy.m_HeaderCtrl.EnableDividerLines(FALSE);
    m_listStrategy.SetListStyle(eQuickOrderDialog);

    InitListStrategy();

    m_bInitDialog = TRUE;
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
void CQuickOrderModuleDialog::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if(m_bInitDialog)
    {
        CRect rect;
        GetClientRect(&rect);

        m_cThumbFrame.MoveWindow(0,0,cx,THUMBNAILWINDOW_HEIGHT); 
        m_btnOrder.MoveWindow(rect.Width()*3/4,rect.Height()-32,70,27);
        m_btnOrder.RedrawWindow();
        m_listStrategy.MoveWindow(0,THUMBNAILWINDOW_HEIGHT,cx,cy-THUMBNAILWINDOW_HEIGHT-40);
    }
}

///////////////////////////////////////////////////////////////////////////////
static int m_nColWidths[17] = { 6, 8, 10, 10, 8, 8, 12, 8, 12, 8, 8, 10, 10, 8, 9, 8, 8 };	
static int con_nColCount = 17;
static int con_nColWidths = 142;    
// InitListStrategy
void CQuickOrderModuleDialog::InitListStrategy()
{
    ASSERT(m_listStrategy);
    if (!m_listStrategy)
        return;

    // set column width according to window rect
    CRect rect;
    m_listStrategy.GetWindowRect(&rect);

    int w = rect.Width() - 2;

    TCHAR *	lpszHeaders[] = {   _T(""),                
                                _T("分析"),             
                                _T("行权价"),              
                                _T("合约别"),                 
                                _T("手数"),               
                                _T("持仓"),             
                                _T("价格条件"),               
                                _T("价格"),                
                                _T("持仓均价"),             
                                _T("买/卖"),    
                                _T("开平"),  
                                _T("有效期"),                 
                                _T("隐波率"),               
                                _T("Delta"),             
                                _T("Gamma"),               
                                _T("Theta"),                
                                _T("Vega"),             
                                NULL };


    int i;
    int total_cx = 0;
    LV_COLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));

    //w = w - ::GetSystemMetrics(SM_CXVSCROLL);	// width of vertical scroll bar

    // add columns
    for (i = 0; ; i++)
    {
        if (lpszHeaders[i] == NULL)
            break;

        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.fmt = LVCFMT_CENTER;
        lvcolumn.pszText = lpszHeaders[i];
        lvcolumn.iSubItem = i;
        lvcolumn.cx = (lpszHeaders[i] == NULL) ? w - total_cx/* - 2*/: (w * m_nColWidths[i]) / con_nColWidths;
        total_cx += lvcolumn.cx;
        m_listStrategy.InsertColumn(i, &lvcolumn);
    }

    // create the image list from bitmap resource
    VERIFY(m_listStrategy.m_cImageList.Create(15, 15, ILC_COLOR24, 6, 1));
    CBitmap bm;
    bm.LoadBitmap(IDB_BITMAP_XLISTIMG);
    m_listStrategy.m_cImageList.Add(&bm, RGB(255, 0, 255));

    // iterate through header items and attach the image list
    HDITEM hditem;

    for (i = 0; i < m_listStrategy.m_HeaderCtrl.GetItemCount(); i++)
    {
        //hditem.mask = HDI_IMAGE | HDI_FORMAT;
        m_listStrategy.m_HeaderCtrl.GetItem(i, &hditem);
        hditem.fmt |=  HDF_IMAGE;		
        if( i == 0)
            hditem.iImage = XHEADERCTRL_CUSTOM_IMAGE;        
        else
            hditem.iImage = XHEADERCTRL_NO_IMAGE;

        m_listStrategy.m_HeaderCtrl.SetItem(i, &hditem);
    }

    memset(&lvcolumn, 0, sizeof(lvcolumn));

    //set the format again - must do this twice or first column does not get set
    for (i = 0; ; i++)
    {
        if (lpszHeaders[i] == NULL)
            break;

        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
        lvcolumn.fmt = LVCFMT_CENTER;
        lvcolumn.iSubItem = i;
        m_listStrategy.SetColumn(i, &lvcolumn);
    }

}
void CQuickOrderModuleDialog::InitListStrategyData()
{
    // Note - the string arrays must persist for life of list control
    // a list of some of my favorite things - note that this list
    // is not sorted        

    m_arrayStrikePrice.RemoveAll();
    CString str;
    vector<PlatformStru_InstrumentInfo>::iterator it = m_vecOptionIns.begin();
    set<int> setOptionIns;
    while(it!=m_vecOptionIns.end())
    {
        setOptionIns.insert((int)(it->StrikePrice));
        it++;
    }

    set<int>::iterator it_set = setOptionIns.begin();     ;
    while(it_set!=setOptionIns.end())
    {
        str.Format("%d",*it_set);
        m_arrayStrikePrice.Add(str);
        it_set++;
    }


    //init
    m_arrayOptionStyle.RemoveAll();
    m_arrayOptionStyle.Add(_T("看涨期权"));
    m_arrayOptionStyle.Add(_T("看跌期权"));

    m_arrayPriceCondition.RemoveAll();
    m_arrayPriceCondition.Add(_T("市价"));
    m_arrayPriceCondition.Add(_T("限价"));

    m_arrayDirect.RemoveAll();
    m_arrayDirect.Add(_T("买"));
    m_arrayDirect.Add(_T("卖"));

    m_arrayOffset.RemoveAll();
    m_arrayOffset.Add(_T("开仓"));
    m_arrayOffset.Add(_T("平今"));
    m_arrayOffset.Add(_T("平仓"));

    m_arrayDate.RemoveAll();
    m_arrayDate.Add(_T("ROD"));
    m_arrayDate.Add(_T("IOC"));
}
// FillListStrategy
void CQuickOrderModuleDialog::FillListStrategy(const vector<PlatformStru_OrderInfo>&  outOrders,bool bAdd/*=false*/)
{
    //m_listStrategy.LockWindowUpdate();	// ***** lock window updates while filling list *****
    m_listStrategy.SetRedraw(FALSE);
    if(!bAdd)
        m_listStrategy.DeleteAllItems();

    CString str = _T("");
    int nItem=0, nSubItem;
    
    // insert the items and subitems into the list
    PlatformStru_OrderInfo sOrderInfo, *pOrderInfo=NULL;        
    int    nStrikePrice;
    double dPriceTick;
    double dUpperPrice;
    double dLowerPrice;
    double dVolatility;
    char   eTyple;
    int    nIndex = 0,nSize = 0;  
    char buf[13]={0};
    CString strStrikePrice,strOrderCount,strOrderPrice,strDirect,strVolatility;
    for (vector<PlatformStru_OrderInfo>::const_iterator it=outOrders.begin(); it!=outOrders.end(); ++it)
    {
        sOrderInfo = *it; 

        dPriceTick = CDataInfo::NewInstance()->GetInstPriceTick((string)sOrderInfo.InstrumentID);
        dUpperPrice = CDataInfo::NewInstance()->GetInstUpperPrice((string)sOrderInfo.InstrumentID);
        dLowerPrice = CDataInfo::NewInstance()->GetInstLowerPrice((string)sOrderInfo.InstrumentID);
        dVolatility = CDataInfo::NewInstance()->GetVolatility();

        pOrderInfo = new PlatformStru_OrderInfo(sOrderInfo);
        nStrikePrice = (int)GetStrikePrice(sOrderInfo.InstrumentID);
        eTyple = GetOrderStyle(sOrderInfo.InstrumentID);
        strStrikePrice.Format("%d",nStrikePrice);
        strOrderCount.Format("%d",sOrderInfo.VolumeTotalOriginal);
        strOrderPrice=Price2String(sOrderInfo.LimitPrice,dPriceTick);
        strVolatility.Format("%.1f%%",dVolatility*100);
        

        
        for (nSubItem = 0; nSubItem < con_nColCount; nSubItem++)
        {
            str = _T("");
            if (nSubItem == 0)				// custom bmp Image
                str = _T(" ");
            else if (nSubItem == 1)			// checkbox
                str = _T("");
            else if (nSubItem == 2)			// combo
                str = strStrikePrice;
            else if (nSubItem == 3)			// normal text
                str = (THOST_FTDC_CP_CallOptions == eTyple)?_T("看涨期权"):_T("看跌期权");
            else if (nSubItem == 4)			// spin
                str = strOrderCount;
            else if (nSubItem == 5)			// normal text
                str = _T("");
            else if (nSubItem == 6)			// combo
                str = _T("限价");
            else if (nSubItem == 7)			// spin
                str = strOrderPrice;
            else if (nSubItem == 8)			// normal text
                str = _T("");
            else if (nSubItem == 9)			// normal text
                str = (THOST_FTDC_D_Buy==sOrderInfo.Direction)?_T("买"):_T("卖");
            else if (nSubItem == 10)	    // combo
                str = _T("开仓");
            else if (nSubItem == 11)	    // combo
                str = _T("ROD");
            else if (nSubItem == 12)		// normal text
                str = strVolatility;
            else if (nSubItem == 13)		// normal text
                str = _T("0.00%");
            else if (nSubItem == 14)		// normal text
                str = _T("0.00%");
            else if (nSubItem == 15)		// normal text
                str = _T("0.00%");
            else if (nSubItem == 16)		// normal text
                str = _T("0.00%");

            // show background coloring
            if (nSubItem == 0)
            {
                m_listStrategy.InsertItem(nItem, str);
                m_listStrategy.SetItemData(nItem,(DWORD)pOrderInfo);
                m_listStrategy.SetItemImage(nItem, nSubItem,eDeleteImage);
                m_setOrderInfo.insert(pOrderInfo);
            }
            else if (nSubItem == 1)
                m_listStrategy.SetCheckbox(nItem, nSubItem, 1);			
            else if (nSubItem == 2)
            {
                nSize = m_arrayStrikePrice.GetSize();
                for(int   i=0;i<nSize;i++)  
                {   
                    CString strTmp = m_arrayStrikePrice.GetAt(i);  
                    if(strTmp == strStrikePrice)
                        nIndex = i;
                } 

                m_listStrategy.SetComboBox(nItem, nSubItem, 
                                    TRUE,		// enable combobox
                                    &m_arrayStrikePrice,		// pointer to persistent CStringArray
                                    16,			// size of dropdown
                                    nIndex,			// initial combo selectionf
                                    strStrikePrice,
                                    FALSE);		// sort CStringArray
            }
            else if (nSubItem == 3)
            {
                m_listStrategy.SetComboBox(nItem, nSubItem, 
                                    TRUE,		// enable combobox
                                    &m_arrayOptionStyle,  // pointer to persistent CStringArray
                                    16,			// size of dropdown
                                    (THOST_FTDC_CP_CallOptions == eTyple)?0:1,			// initial combo selection
                                    str,
                                    FALSE);		// sort CStringArray 
            }
            else if (nSubItem == 4)
            {
                m_listStrategy.SetInstrumentData(1,10000,1);
                m_listStrategy.SetNumSpin(nItem, nSubItem); 
                m_listStrategy.SetItemText(nItem, nSubItem, str/*, RGB(0,0,0), RGB(255,255,255)*/); 
            }
            else if (nSubItem == 5)
                m_listStrategy.SetItemText(nItem, nSubItem, str/*, RGB(0,0,0), RGB(255,255,255)*/); 
            else if (nSubItem == 6)
                m_listStrategy.SetComboBox(nItem, nSubItem, 
                                                    TRUE,		// enable combobox
                                                    &m_arrayPriceCondition,		// pointer to persistent CStringArray
                                                    16,			// size of dropdown
                                                    1,			// initial combo selection
                                                    str,
                                                    FALSE);		// sort CStringArray
            else if (nSubItem == 7)
            {
                m_listStrategy.SetInstrumentData(dLowerPrice,dUpperPrice,dPriceTick);
                m_listStrategy.SetNumSpin(nItem, nSubItem); 
                m_listStrategy.SetItemText(nItem, nSubItem, str/*, RGB(0,0,0), RGB(255,255,255)*/); 
            }
            else if (nSubItem == 9)
                m_listStrategy.SetComboBox(nItem, nSubItem, 
                                                TRUE,		// enable combobox
                                                &m_arrayDirect,// pointer to persistent CStringArray
                                                16,			// size of dropdown
                                                (THOST_FTDC_D_Buy==sOrderInfo.Direction)?0:1,			// initial combo selection
                                                str,
                                                FALSE);		// sort CStringArray
            else if (nSubItem == 10)
                m_listStrategy.SetComboBox(nItem, nSubItem, 
                                            TRUE,		// enable combobox
                                            &m_arrayOffset,		// pointer to persistent CStringArray
                                            16,			// size of dropdown
                                            0,			// initial combo selection
                                            str,
                                            FALSE);		// sort CStringArray
            else if (nSubItem == 11)
                m_listStrategy.SetComboBox(nItem, nSubItem, 
                                                TRUE,		// enable combobox
                                                &m_arrayDate,		// pointer to persistent CStringArray
                                                16,			// size of dropdown
                                                0,			// initial combo selection
                                                str,
                                                FALSE);		// sort CStringArray

           m_listStrategy.SetItemText(nItem, nSubItem, str);
           m_listStrategy.SetItemColors(nItem,nSubItem, GetTextRGB(str),
                                        (m_ColorMode == Color_Light)?RGB(255,255,255):RGB(9,58,117));
        }

        nItem++;

        PositionKey key(/*"18600099503"*/pOrderInfo->Account,
                        pOrderInfo->InstrumentID,
                        pOrderInfo->Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short,
                        pOrderInfo->CombHedgeFlag[0]);


        COptionStrategyDlg* pMain = (COptionStrategyDlg*)GetParent();
        pMain->ReqPosition(key);
    }

#ifndef NO_XLISTCTRL_TOOL_TIPS

    // set tool tips in second row
    nItem = 0;
    for (nSubItem = 0; nSubItem < con_nColCount; nSubItem++)
    {
        str.Format(_T("This is %d,%d"), nItem, nSubItem);
        m_listStrategy.SetItemToolTipText(nItem, nSubItem, str);
    }

#endif
    UpDataLineChart();
    m_listStrategy.Invalidate();
    m_listStrategy.UpdateWindow();  
    m_listStrategy.SetRedraw(TRUE);
    //m_listStrategy.UnlockWindowUpdate();	// ***** unlock window updates *****

}

COLORREF CQuickOrderModuleDialog::GetTextRGB(CString strName)
{
    if(strName.CompareNoCase("看涨期权")==0)
        return RGB(255,0,0);
    else if (strName.CompareNoCase("看跌期权")==0)
        return RGB(0,255,0);
    else if (strName.CompareNoCase("买")==0)
        return RGB(255,0,0);
    else if (strName.CompareNoCase("卖")==0)
        return RGB(0,255,0);


    return RGB(0,0,0);
}
void CQuickOrderModuleDialog::OnBnClickedButtonOrder()
{
    // TODO: Add your control notification handler code here

    //test
    //PlatformStru_OrderInfo order1, order2;
    //strcpy(order1.InstrumentID , "IF1412");
    //order1.Direction=THOST_FTDC_D_Buy;
    //order1.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
    //order1.LimitPrice = 2512.1;
    //order1.VolumeTotalOriginal=24;
    //m_vecOutOrders.push_back(order1);

    //strcpy(order2.InstrumentID , "IF1414");
    //order2.Direction=THOST_FTDC_D_Sell;
    //order2.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
    //order2.LimitPrice = 6552.1;
    //order2.VolumeTotalOriginal=2;
    //m_vecOutOrders.push_back(order2); 
    CString str;
    PlatformStru_OrderInfo *pOrder = NULL;
    vector<PlatformStru_OrderInfo> vecOrders;
    int nCount = m_listStrategy.GetItemCount();
    for (int i=0;i<nCount;i++)
    {
        pOrder = (PlatformStru_OrderInfo*)m_listStrategy.GetItemData(i);
        pOrder->Direction = (m_listStrategy.GetItemText(i,ColumnDirection).Compare(_T("买"))==0)?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;
        pOrder->CombOffsetFlag[0] = (m_listStrategy.GetItemText(i,ColumnOffset).Compare(_T("开仓"))==0)?THOST_FTDC_OF_Open:
                                 (m_listStrategy.GetItemText(i,ColumnOffset).Compare(_T("平今"))==0)?THOST_FTDC_OF_CloseToday:THOST_FTDC_OF_Close;
        pOrder->LimitPrice = atof(m_listStrategy.GetItemText(i,ColumnOrderPrice));
        pOrder->VolumeTotalOriginal = atof(m_listStrategy.GetItemText(i,ColumnOrderCount)); 
        
        vecOrders.push_back(*pOrder);
    }

    

    if(m_listStrategy.GetItemCount()<=0 || vecOrders.empty())
        return;
    
    m_OrderConfirmDlg.SetData(vecOrders);

    INT_PTR nResponse = m_OrderConfirmDlg.DoModal();
    if (nResponse == IDOK)
    {
        COptionStrategyDlg* pMain = (COptionStrategyDlg*)GetParent();
        pMain->NewOrder(vecOrders);

    }
    else if (nResponse == IDCANCEL)
    {

    }
}
LRESULT CQuickOrderModuleDialog::OnStrategyBtnLClicked( WPARAM wParam, LPARAM lParam )
{
    eStrategyID nStrategyID = (eStrategyID)wParam;

    m_vecOptionIns.clear();
    m_maplastQuot.clear();
    m_vecOutOrders.clear();

    ClearListCtrl();

    CDataInfo::NewInstance()->GetRelaInstrumentInfo(m_vecOptionIns);

    InitListStrategyData();  

    //加上标的物的合约信息
    m_vecOptionIns.push_back(m_sSubscribeInstInfo);
    CDataInfo::NewInstance()->GetLastQuotInfo(m_maplastQuot);
    
    StrategyDecisionMaking obj;    
    obj.GetOptionStrategyOrders(nStrategyID,m_sSubscribeInstInfo.InstrumentID,m_vecOptionIns,m_maplastQuot,m_vecOutOrders);

    FillListStrategy(m_vecOutOrders,true);

    return 0;
}
LRESULT CQuickOrderModuleDialog::OnVolatilityUpdate(WPARAM wParam,LPARAM lParam)
{
    CString str;
    str.Format("%0.1f",CDataInfo::NewInstance()->GetVolatility()*100);
    for (int nItem = 0;nItem<m_listStrategy.GetItemCount();nItem++)
        m_listStrategy.SetItemText(nItem,11,str);

    return 1;
}
//来自持仓查询
LRESULT CQuickOrderModuleDialog::OnPositionUpdate(WPARAM wParam,LPARAM lParam)
{
    PlatformStru_Position *pPosition = (PlatformStru_Position*)wParam;

    CString str;
    PlatformStru_OrderInfo sOrder;
    int nCount = m_listStrategy.GetItemCount();
    for (int i=0;i<nCount;i++)
    {
        sOrder = *(PlatformStru_OrderInfo*)m_listStrategy.GetItemData(i);

        if(strncmp(sOrder.InstrumentID,pPosition->InstrumentID,sizeof(pPosition->InstrumentID))==0 
            && (sOrder.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short) == pPosition->PosiDirection
            && pPosition->HedgeFlag== THOST_FTDC_HF_Speculation)
        {
            str.Format("%d",pPosition->Position);
            m_listStrategy.SetItemText(i,ColumnOrderPosion,str);
            str.Format("%.2f",pPosition->AveragePositionPrice);
            m_listStrategy.SetItemText(i,ColumnPosionAveragePrice,str);
        }
    }

    return 1;
}
//来自成交回报
void CQuickOrderModuleDialog::UpdatePositionVolume(PlatformStru_TradeInfo& TradeInfo)
{
    int nRestVolumn;
    CString strVolumn;
    PlatformStru_OrderInfo sOrder;
    int nCount = m_listStrategy.GetItemCount();
    for (int i=0;i<nCount;i++)
    {
        sOrder = *(PlatformStru_OrderInfo*)m_listStrategy.GetItemData(i);
        if(strncmp(sOrder.InstrumentID,TradeInfo.InstrumentID,sizeof(TradeInfo.InstrumentID))==0 
            && (sOrder.Direction == (TradeInfo.Direction==THOST_FTDC_D_Buy)?THOST_FTDC_D_Sell:THOST_FTDC_D_Buy)//持仓方向
            && sOrder.CombHedgeFlag[0]== THOST_FTDC_HF_Speculation)
        {
            if(THOST_FTDC_OF_Open == TradeInfo.OffsetFlag)
                nRestVolumn = atoi(m_listStrategy.GetItemText(i,ColumnOrderPosion))+TradeInfo.Volume;
            else
                nRestVolumn = atoi(m_listStrategy.GetItemText(i,ColumnOrderPosion))-TradeInfo.Volume;

            strVolumn.Format("%d",nRestVolumn);
            m_listStrategy.SetItemText(i,ColumnOrderPosion,strVolumn);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
// OnEditEnd
//
// This method shows how to handle WM_XLISTCTRL_EDIT_END messages 
// from XListCtrl
//
LRESULT CQuickOrderModuleDialog::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
    if (nItem >= 0 && nSubItem >= 0)
    {
        CString strText = m_listStrategy.GetItemText(nItem, nSubItem);

        COLORREF crText, crBackground;
        m_listStrategy.GetItemColors(nItem, nSubItem, crText, crBackground);
        if (m_listStrategy.GetModified(nItem, nSubItem))
        {
            // subitem was modified - color it red
            //m_listStrategy.SetItemText(nItem, nSubItem, strText, 
            //    RGB(255,0,0), crBackground);

            UpDataLineChart();
        }
        else
        {
            // subitem not modified - color it black -
            // note that once modified, a subitem will remain
            // marked as modified
            //m_listStrategy.SetItemText(nItem, nSubItem, strText, 
            //    RGB(0,0,0), crBackground);
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnComboSelection
//
// This method shows how to handle WM_XLISTCTRL_COMBO_SELECTION messages 
// from XListCtrl
//
LRESULT CQuickOrderModuleDialog::OnComboSelection(WPARAM nItem, LPARAM nSubItem)
{
    if (nItem >= 0 && nSubItem >= 0)
    {
        CString strText = m_listStrategy.GetItemText(nItem, nSubItem);

        COLORREF crText, crBackground;
        m_listStrategy.GetItemColors(nItem, nSubItem, crText, crBackground);
        if (m_listStrategy.GetModified(nItem, nSubItem))
        {
            // subitem was modified - color it red
            m_listStrategy.SetItemText(nItem, nSubItem, strText, 
                GetTextRGB(strText), crBackground);

            UpDataLineChart();
        }
        else
        {
            // subitem not modified - color it black -
            // note that once modified, a subitem will remain
            // marked as modified
            m_listStrategy.SetItemText(nItem, nSubItem, strText, 
                GetTextRGB(strText), crBackground);
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnCheckbox
//
// This method shows how to handle WM_XLISTCTRL_DELETEIMG_CLICKED messages 
// from XListCtrl
//
LRESULT CQuickOrderModuleDialog::OnDeleteImgClicked(WPARAM nItem, LPARAM nSubItem)
{
    int iItem = (int)nItem;
    m_listStrategy.DeleteItem(iItem);
    UpDataLineChart();

    //否则会有点击圈选现象
    m_listStrategy.PostMessage(WM_LBUTTONUP, 0, 0);

    return 0;
}
///////////////////////////////////////////////////////////////////////////////
// OnCheckbox
//
// This method shows how to handle WM_XLISTCTRL_CHECKBOX_CLICKED messages 
// from XListCtrl
//
LRESULT CQuickOrderModuleDialog::OnCheckbox(WPARAM nItem, LPARAM nSubItem)
{
    UpDataLineChart();
    return 0;
}

double  CQuickOrderModuleDialog::GetStrikePrice(string sInstID)
{
    string sInstrumentID;
    vector<PlatformStru_InstrumentInfo>::iterator it = m_vecOptionIns.begin();
    while(it!=m_vecOptionIns.end())
    {
        sInstrumentID = (string)(it->InstrumentID);
        if(0==strncmp(sInstrumentID.c_str(),sInstID.c_str(),strlen(sInstID.c_str())))
            return it->StrikePrice;
        else
            it++;
    }

    return 0.0;
}
char CQuickOrderModuleDialog::GetOrderStyle(string sInstID)
{
    string sInstrumentID;
    vector<PlatformStru_InstrumentInfo>::iterator it = m_vecOptionIns.begin();
    while(it!=m_vecOptionIns.end())
    {
        sInstrumentID = (string)(it->InstrumentID);
        if(sInstrumentID == sInstID)
            return it->OptionsType;
        else
            it++;
    }

    return '0';
}
void CQuickOrderModuleDialog::SetSubscribedInst(const PlatformStru_InstrumentInfo& sInstInfo)
{
    m_sSubscribeInstInfo = sInstInfo;
}

void CQuickOrderModuleDialog::UpDataLineChart(void)
{
    CString str;
    sStrategyData   sData;
    vector<sStrategyData> vData;

    bool bDirectBullish = false, bDirectBearish = false;
    for (int i = 0;i < m_listStrategy.GetItemCount();i++)
    {
        if (m_listStrategy.GetCheckbox(i, ColumnCheck))
        {
            str = m_listStrategy.GetItemText(i,ColumnPremiumStyle);
            if(0==str.Compare("看涨期权"))
            {
                bDirectBullish = true;
                str = m_listStrategy.GetItemText(i,ColumnDirection);
                 if(0==str.Compare("买"))
                     sData.eStyle = BuyBullish;
                 else 
                     sData.eStyle = SellBullish;                
            }
            else
            {
                bDirectBearish = true;
                str = m_listStrategy.GetItemText(i,ColumnDirection);
                 if(0==str.Compare("买"))
                     sData.eStyle = BuyBearish;
                 else 
                     sData.eStyle = SellBearish;                

            }
            string  sInstrumentID = m_sSubscribeInstInfo.InstrumentID;

            sData.dOrderPrice  = atof(m_listStrategy.GetItemText(i,ColumnOrderPrice));
            sData.dStrikePrice = atof(m_listStrategy.GetItemText(i,ColumnExecutivePrice));
            sData.nOrderCount  = atoi(m_listStrategy.GetItemText(i,ColumnOrderCount));
            sData.dUnderlyingMultiple = CDataInfo::NewInstance()->GetInstVolumeMultiple(sInstrumentID);

            vData.push_back(sData);
        }
    }

    bool bResult = vData.empty();
    CDataInfo::NewInstance()->SetInit(!bResult);
    
    if(!bResult)
    {
        CDataInfo::NewInstance()->SetStrategyData(vData);    
        CDataInfo::NewInstance()->Calculate();
    }

    COptionStrategyDlg* pMainWnd = (COptionStrategyDlg*)GetParent();
    if(pMainWnd && ::IsWindow(pMainWnd->m_hWnd))
        pMainWnd->UpdateLineChartCtrl();
}

void CQuickOrderModuleDialog::UpdateXListCtrl(const PlatformStru_OrderInfo& outOrder)
{
    PlatformStru_OrderInfo sOrder = NULL;
    char  cDirect;
    int     nCount,nOrderCount;
    CString sCount;

    nCount = m_listStrategy.GetItemCount();
    for (int i=0;i<nCount;i++)
    {
        sOrder = *(PlatformStru_OrderInfo*)m_listStrategy.GetItemData(i);
        cDirect = sOrder.Direction;
        nOrderCount = atoi(m_listStrategy.GetItemText(i,ColumnOrderCount))+1;
        sCount.Format("%d",nOrderCount);

        if(strncmp(sOrder.InstrumentID,outOrder.InstrumentID,sizeof(outOrder.InstrumentID))==0 && cDirect == outOrder.Direction)
        {
            m_listStrategy.SetItemText(i,ColumnOrderCount,sCount);
            return;
        }
    }

    vector<PlatformStru_OrderInfo> outOrders;
    outOrders.push_back(outOrder);

    CDataInfo::NewInstance()->GetRelaInstrumentInfo(m_vecOptionIns);
    InitListStrategyData();
    FillListStrategy(outOrders,true);
}

void CQuickOrderModuleDialog::ClearListCtrl(void)
{
    m_listStrategy.DeleteAllItems();

    CDataInfo::NewInstance()->SetInit(false);    
    COptionStrategyDlg* pMainWnd = (COptionStrategyDlg*)GetParent();
    if(pMainWnd && ::IsWindow(pMainWnd->m_hWnd))
        pMainWnd->UpdateLineChartCtrl();
}

void CQuickOrderModuleDialog::UpdataColorMode(BK_COLOR_MODE eMode)
{  
    m_listStrategy.SetBKColorMode(eMode); 

    int nCount = m_listStrategy.GetItemCount();//只有一行
    for (int nItem = 0; nItem < nCount; nItem++)
    {
        for (int nSubItem = 0; nSubItem < m_listStrategy.m_HeaderCtrl.GetItemCount(); nSubItem++)
        {
            COLORREF crText;
            COLORREF crBackground;
            m_listStrategy.GetItemColors(nItem, nSubItem, crText, crBackground);
            m_listStrategy.SetItemColors(nItem, nSubItem, crText, (eMode == Color_Light)?RGB(255,255,255):RGB(9,58,117));
        } 
    }


    m_listStrategy.RedrawWindow();
}