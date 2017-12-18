// T_PriceModuleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "OptionStrategyDlg.h"
#include "T_PriceModuleDialog.h"
#include "TransparentHelper.h"
#include "CommPublic.h"
#include "OrderFunc.h"

#include "..\..\TPriceList\TPriceListDLL.h"


// CT_PriceModuleDialog dialog

IMPLEMENT_DYNAMIC(CT_PriceModuleDialog, CDialog)

CT_PriceModuleDialog::CT_PriceModuleDialog(CWnd* pParent /*=NULL*/)
	: CBKDialog(CT_PriceModuleDialog::IDD, pParent)
    , m_bInitDialog(FALSE)    
{
    m_pTPriceListPanel = NULL; 
    m_pMemDC = NULL;
    m_pOldBitmap = NULL;
    m_pBitmapMem = NULL;
}   

CT_PriceModuleDialog::~CT_PriceModuleDialog()
{
    if ( m_pMemDC )
    {
        delete m_pBitmapMem;
        m_pBitmapMem = NULL;
        delete m_pMemDC;
        m_pMemDC = NULL;
    }

}

void CT_PriceModuleDialog::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_QUOT, m_listQuot);
}


BEGIN_MESSAGE_MAP(CT_PriceModuleDialog, CBKDialog) 
    //ON_MESSAGE(WM_XLISTCTRL_KLINEIMG_CLICKED,OnBtnKLineClick)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_MESSAGE( WM_TRANSPARENT_BK, OnTransaprentBk)
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_USER_QUOT_UPDATE,QuotUpdateMsgHandler)
END_MESSAGE_MAP()


// CT_PriceModuleDialog message handlers

BOOL CT_PriceModuleDialog::OnInitDialog()
{
    CBKDialog::OnInitDialog();
    // TODO:  Add extra initialization here   
    SetTimer(conCreateTPriceList, 15, NULL);

    m_listQuot.ModifyStyle(0, LVS_REPORT|LVS_NOSCROLL);
    DWORD dwExStyle = LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS*/;
    m_listQuot.SetExtendedStyle(dwExStyle);
    m_listQuot.SetBKColorMode(m_ColorMode); 
    m_listQuot.SetHeaderTextColor(RGB(255,255,255));
    m_listQuot.SetRowHeight(24);
    m_listQuot.m_HeaderCtrl.SetBkColor((m_ColorMode==Color_Light)?RGB(72,129,174):RGB(38,73,104));
    m_listQuot.m_HeaderCtrl.EnableDividerLines(FALSE);
    m_listQuot.SetListStyle(eTPriceDialog);

    InitListStrategy();

    m_bInitDialog = TRUE;

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
void CT_PriceModuleDialog::InitColumn()
{
    //添加列
    m_listQuot.InsertColumn( 0, _T("标的期货")  ,LVCFMT_CENTER, 150);
    m_listQuot.InsertColumn( 1, _T(" ")         ,LVCFMT_CENTER, 50);        
    m_listQuot.InsertColumn( 2, _T("最新价")    ,LVCFMT_CENTER, 100);    
    m_listQuot.InsertColumn( 3, _T(" ")         ,LVCFMT_CENTER, 50);
    m_listQuot.InsertColumn( 4, _T("涨跌")      ,LVCFMT_CENTER, 150);
    m_listQuot.InsertColumn( 5, _T("涨跌幅")    ,LVCFMT_CENTER, 50);        
    m_listQuot.InsertColumn( 6, _T("买价")      ,LVCFMT_CENTER, 100);    
    m_listQuot.InsertColumn( 7, _T("卖价")      ,LVCFMT_CENTER, 50);
    m_listQuot.InsertColumn( 8, _T("最高价")    ,LVCFMT_CENTER, 150);
    m_listQuot.InsertColumn( 9, _T("最低价")    ,LVCFMT_CENTER, 50);        
    m_listQuot.InsertColumn(10, _T("成交量")    ,LVCFMT_CENTER, 100);    
    m_listQuot.InsertColumn(11, _T("到期日")    ,LVCFMT_CENTER, 50);
    m_listQuot.InsertColumn(12, _T("更新时间")  ,LVCFMT_CENTER, 150);

}

void CT_PriceModuleDialog::InsertData(const PlatformStru_DepthMarketData& data)
{
    CString str = _T("");
    double dPrice,dPriceTick = CDataInfo::NewInstance()->GetInstPriceTick();

    CString strTemp[13];
    strTemp[0].Format("%s",data.InstrumentID);

    dPrice = data.LastPrice;
    strTemp[2]=Price2String(dPrice, dPriceTick);

    dPrice = data.LastPrice-data.PreSettlementPrice;
    strTemp[4]=Price2String(dPrice, dPriceTick);

    dPrice = (data.LastPrice-data.PreSettlementPrice)*100/data.PreSettlementPrice;
    strTemp[5]=Price2String(dPrice, 0.01)+_T("%");

    dPrice = data.BidPrice1;
    strTemp[6]=Price2String(dPrice, dPriceTick);

    dPrice = data.AskPrice1;
    strTemp[7]=Price2String(dPrice, dPriceTick);

    dPrice = data.HighestPrice;
    strTemp[8]=Price2String(dPrice, dPriceTick);

    dPrice = data.LowestPrice;
    strTemp[9]=Price2String(dPrice, dPriceTick);

    strTemp[10].Format("%d",data.Volume);
    strTemp[11].Format("%s",CDataInfo::NewInstance()->GetExpireDate().c_str());
    strTemp[12].Format("%s",data.UpdateTime);

    int nCount = m_listQuot.GetItemCount();//获取当前已插入的行数
    m_listQuot.InsertItem(nCount, _T(""));//插入一行
    m_listQuot.SetItemText(nCount, 0, strTemp[0]);//第一列
    m_listQuot.SetItemImage(nCount, 1,0);
    m_listQuot.SetItemText(nCount, 2, strTemp[2]);//第三列
    if(data.LastPrice-data.PreSettlementPrice>0.001)
        m_listQuot.SetItemImage(nCount, 3,1);
    else
        m_listQuot.SetItemImage(nCount, 3,2);
    m_listQuot.SetItemText(nCount, 4, strTemp[4]);//第一列
    m_listQuot.SetItemText(nCount, 5, strTemp[5]);//第二列
    m_listQuot.SetItemText(nCount, 6, strTemp[6]);//第三列
    m_listQuot.SetItemText(nCount, 7, strTemp[7]);//第四列
    m_listQuot.SetItemText(nCount, 8, strTemp[8]);//第一列
    m_listQuot.SetItemText(nCount, 9, strTemp[9]);//第二列
    m_listQuot.SetItemText(nCount,10, strTemp[10]);//第三列
    m_listQuot.SetItemText(nCount,11, strTemp[11]);//第四列
    m_listQuot.SetItemText(nCount,12, strTemp[12]);//第四列
}
// 根据内容，自动调整CListCtrl的列宽  
void CT_PriceModuleDialog::AutoAdjustColumnWidth()  
{  
    m_listQuot.SetRedraw(FALSE);  
    CHeaderCtrl *pHeader = m_listQuot.GetHeaderCtrl();  
    int nColumnCount = pHeader->GetItemCount();  

    for(int i = 0; i < nColumnCount; i++)  
    {  
        m_listQuot.SetColumnWidth(i, LVSCW_AUTOSIZE);  
        int nColumnWidth = m_listQuot.GetColumnWidth(i);  
        m_listQuot.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
        int nHeaderWidth = m_listQuot.GetColumnWidth(i);  

        m_listQuot.SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));  
    }  
    m_listQuot.SetRedraw(TRUE);  
}  
void CT_PriceModuleDialog::InitListStrategy()
{
    static int con_nColCount = 13;  
    static int m_nColWidths[13] = { 10, 4, 8, 4, 8, 8, 8, 8, 8, 8, 10, 10, 15 };
    static int con_nColWidths = 110;  

    ASSERT(m_listQuot);
    if (!m_listQuot)
        return;

    // set column width according to window rect
    CRect rect;
    m_listQuot.GetWindowRect(&rect);

    int w = rect.Width()/* - 2*/;

    TCHAR *	lpszHeaders[] = {              
                            _T("标的期货"),   
                            _T(""), 
                            _T("最新价"),  
                            _T(""), 
                            _T("涨跌"),                 
                            _T("涨跌幅"),               
                            _T("买价"),             
                            _T("卖价"),               
                            _T("最高价"),                
                            _T("最低价"),             
                            _T("成交量"),              
                            _T("到期日"),                 
                            _T("更新时间"),     
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
        lvcolumn.cx = (lpszHeaders[i] == NULL) ? w - total_cx/* - 2*/ : (w * m_nColWidths[i]) / con_nColWidths;
        total_cx += lvcolumn.cx;
        m_listQuot.InsertColumn(i, &lvcolumn);
    }

	// create the image list from bitmap resource
	VERIFY(m_listQuot.m_cImageList.Create(15, 15, ILC_COLOR24, 5, 1));
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP_XLISTIMG);
	m_listQuot.m_cImageList.Add(&bm, RGB(255,0,255));

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < m_listQuot.m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		m_listQuot.m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		if (i == 1 || i == 3)
			hditem.iImage = XHEADERCTRL_CUSTOM_IMAGE;
		else
			hditem.iImage = XHEADERCTRL_NO_IMAGE;

		m_listQuot.m_HeaderCtrl.SetItem(i, &hditem);
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
        m_listQuot.SetColumn(i, &lvcolumn);
    }


    //先插入一行
    CString str = _T("");
    int nItem=0, nSubItem;
    m_listQuot.InsertItem(nItem, str);
    // insert the items and subitems into the list
    for (nSubItem = 0; nSubItem < 13; nSubItem++)
    {  
        if(nSubItem==1)
            m_listQuot.SetItemImage(nItem, nSubItem,eKLineImage);


        m_listQuot.SetItemText(nItem, nSubItem, str/*, RGB(0,0,0), RGB(255,255,255)*/);        
    } 
}
void CT_PriceModuleDialog::FillListStrategy(const PlatformStru_DepthMarketData& data)
{
    CString str = _T("");
    int nItem=0, nSubItem;
    double dPrice,dPriceTick = CDataInfo::NewInstance()->GetInstPriceTick();

    CString strTemp[13];
    strTemp[0].Format("%s",data.InstrumentID);

    dPrice = data.LastPrice;
    strTemp[2]=Price2String(dPrice, dPriceTick);

    dPrice = data.LastPrice-data.PreSettlementPrice;
    strTemp[4]=Price2String(dPrice, dPriceTick);

    dPrice = (data.LastPrice-data.PreSettlementPrice)*100/data.PreSettlementPrice;
    strTemp[5]=Price2String(dPrice, 0.01)+_T("%");

    dPrice = data.BidPrice1;
    strTemp[6]=Price2String(dPrice, dPriceTick);

    dPrice = data.AskPrice1;
    strTemp[7]=Price2String(dPrice, dPriceTick);

    dPrice = data.HighestPrice;
    strTemp[8]=Price2String(dPrice, dPriceTick);

    dPrice = data.LowestPrice;
    strTemp[9]=Price2String(dPrice, dPriceTick);

    strTemp[10].Format("%d",data.Volume);
    strTemp[11].Format("%s",CDataInfo::NewInstance()->GetExpireDate().c_str());
    strTemp[12].Format("%s",data.UpdateTime);


    // insert the items and subitems into the list
    int nCount = sizeof(strTemp);
    for (nSubItem = 0; nSubItem < 13; nSubItem++)
    {
        if(nSubItem==1)
            continue;

        if (nSubItem==3)
        {
            static int nsIndex = -1;
            if(data.LastPrice-data.PreSettlementPrice>0.001)
            {
                if(nsIndex != 1)
                {
                    nsIndex = 1;
                    m_listQuot.SetItemImage(nItem, nSubItem,m_ColorMode?eUpDarkImage:eUpLightImage);
                }
            }
            else
            {
                if(nsIndex != 2)
                {
                    nsIndex = 2;
                    m_listQuot.SetItemImage(nItem, nSubItem,m_ColorMode?eDownDarkImage:eDownLightImage);
                }                
            }

            continue;
        }

        str = strTemp[nSubItem];

        bool bUpdate = false;
        COLORREF crText = GetTextRGB(data,nSubItem,bUpdate);
        COLORREF crBackground = (m_ColorMode == Color_Light)?RGB(255,255,255):RGB(9,58,117);

        if(bUpdate)
            m_listQuot.SetItemText(nItem, nSubItem, str, crText, crBackground);
        else
            m_listQuot.SetItemText(nItem, nSubItem, str);
    } 
}
COLORREF CT_PriceModuleDialog::GetTextRGB(const PlatformStru_DepthMarketData& data, int nSubItem, bool& bUpdate)
{
    COLORREF rgb=-1;
    static map<int,string> mapClumn2Color;
    if((nSubItem==2)||(nSubItem==4)||(nSubItem==5)||(nSubItem==6)||(nSubItem==7)||(nSubItem==8)||(nSubItem==9))
    {        
        string sColor;
        if(data.LastPrice-data.PreSettlementPrice>0.001)  
            sColor = "red";
        else
            sColor = "green";

        map<int,string>::iterator it = mapClumn2Color.find(nSubItem);
        if(it!=mapClumn2Color.end())
        {     
            if(it->second == sColor)
                bUpdate = false;
            else
                bUpdate = true;
        }
        else
            bUpdate = true;  
        

        mapClumn2Color.insert(make_pair(nSubItem,sColor));
        rgb = (sColor == "red")?RGB(255,0,0):RGB(0,255,0);
    }    
        
    return rgb;
}
void CT_PriceModuleDialog::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if(m_pTPriceListPanel!=NULL)
        ::MoveWindow(m_pTPriceListPanel, 0, MARK_QUOT_HIGHT, cx, cy-MARK_QUOT_HIGHT, TRUE);

    if(m_bInitDialog)
    {        
        m_listQuot.MoveWindow(0, 0, cx, MARK_QUOT_HIGHT, TRUE);
        //AutoAdjustColumnWidth();对于自绘不起作用
    }
}

LRESULT CT_PriceModuleDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    if(message==Msg_PriceListT_Click) 
    {
        int nEventID = wParam;
        int nClickMode = 0;
        std::string strInstrumentID;
        BOOL bDirection = TRUE;
        bool bRet = Get_Del_ClickParam(nEventID, nClickMode, strInstrumentID, bDirection);
        if(bRet) 
        {
            char csText[1024] = {0};
            sprintf(csText, "收到合约[%s]方向[%s]输入", strInstrumentID.c_str(), bDirection ? "buy" : "sell");

            if(nClickMode==2)
                OnBtnKLineClick(wParam,lParam);
            else if(nClickMode==0)//单击生成报单 
            {
                PlatformStru_OrderInfo  outOrder;
                if(GenerageOptionStrategyOrders(strInstrumentID,bDirection,outOrder))
                {
                    TRACE("pMainWnd->UpdateXListCtrl\n");
                    COptionStrategyDlg* pMainWnd = (COptionStrategyDlg*)GetParent();
                    if(pMainWnd && ::IsWindow(pMainWnd->m_hWnd))
                        pMainWnd->UpdateXListCtrl(outOrder);
                }
            }
        }
    }
    else if(WM_XLISTCTRL_KLINEIMG_CLICKED==message)
        OnBtnKLineClick(wParam,lParam);

    return CBKDialog::WindowProc(message, wParam, lParam);
}

bool  CT_PriceModuleDialog::GenerageOptionStrategyOrders(const string& strInstrumentID,bool bDirection,PlatformStru_OrderInfo& outOrder)
{
    memcpy(outOrder.InstrumentID,strInstrumentID.c_str(),sizeof(outOrder.InstrumentID)-1);
    outOrder.VolumeTotalOriginal = 1;
    outOrder.LimitPrice = CDataInfo::NewInstance()->GetLimitPrice(strInstrumentID,bDirection);
    outOrder.Direction = bDirection?THOST_FTDC_D_Sell:THOST_FTDC_D_Buy;
    outOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    outOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    outOrder.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

    return true;
}

void CT_PriceModuleDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    if(nIDEvent==conCreateTPriceList) {
        if(CreateTPriceList(m_ColorMode))
            KillTimer(conCreateTPriceList);
    }

    CBKDialog::OnTimer(nIDEvent);
}

LRESULT CT_PriceModuleDialog::OnTransaprentBk( WPARAM wParam, LPARAM lParam)
{
    HDC hdc = ( HDC)wParam;
    HWND hwnd = ( HWND)lParam;
    CTransparentHelper::OnTranparentControl( m_pMemDC->GetSafeHdc(), (WPARAM)hdc, (LPARAM)hwnd);
    return TRUE;
}
BOOL CT_PriceModuleDialog::CreateTPriceList(int nMode)
{
    HWND pPanel = CreateWnd(this->GetSafeHwnd());
    if(pPanel==NULL)
        return FALSE;

    ::SetParent(pPanel, GetSafeHwnd());
    m_pTPriceListPanel = pPanel;
    CRect rect;

    GetClientRect(&rect);
    ::ShowWindow(m_pTPriceListPanel, SW_SHOW);
    ::MoveWindow(m_pTPriceListPanel, 0, MARK_QUOT_HIGHT, rect.Width(), rect.Height()-MARK_QUOT_HIGHT, TRUE);


    vector<PlatformStru_InstrumentInfo> vecOptions;
    PlatformStru_InstrumentInfo instr;


    SetOptionInstruments(vecOptions);

    PlatformStru_DepthMarketData newMD;


    if(nMode) {
        SetColorMode(1);
    }
    else {
        SetColorMode(0);
    }

    return TRUE;
    
}
void CT_PriceModuleDialog::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CBKDialog::OnPaint() for painting messages
    CRect rtClient;
    GetClientRect(&rtClient);
    if ( m_pMemDC )
        dc.BitBlt( 0,0,rtClient.Width(), rtClient.Height(), m_pMemDC, 0,0, SRCCOPY);
}

// 行情到来更新T型窗口界面元素
LRESULT CT_PriceModuleDialog::QuotUpdateMsgHandler(WPARAM wParam,LPARAM lParam)
{
    if(wParam == lParam && wParam == 0)
    {
        CString str;
        str.Format("%s",CDataInfo::NewInstance()->GetExpireDate().c_str());
        m_listQuot.SetItemText(0, 11, str);
        return 1;
    }

    PlatformStru_DepthMarketData* data = (PlatformStru_DepthMarketData*)wParam;
    m_data = *data;
    FillListStrategy(*data);
    //InsertData(*data);

    return 0;
}
LRESULT CT_PriceModuleDialog::OnBtnKLineClick(WPARAM nItem, LPARAM nSubItem)
{
    // TODO: Add your control notification handler code here
    CString strPath;
    char localPath[256];
    memset(localPath, 0, 256);
    GetModuleFileName( NULL, localPath, 256);
    std::string filename=localPath;
    size_t splitpos=filename.find_last_of('\\');
    strPath.Format(_T("%s"), filename.substr(0, splitpos+1).c_str()) ;

    CString strDir;

#ifdef _DEBUG
    strDir.Format(_T("%sOptionQuationD.dll"), strPath);
#else
    strDir.Format(_T("%sOptionQuation.dll"), strPath);

#endif	
    m_HInstance = LoadLibrary(strDir.GetBuffer(strDir.GetLength()));
    Func_CreateQuationWnd  funcCreate=(Func_CreateQuationWnd)GetProcAddress(m_HInstance,"CreateQuationWnd");	
    funcCreate(m_hWnd);

    Func_SetTradingDay funcSetTrayDay = (Func_SetTradingDay)GetProcAddress(m_HInstance,"SetTradingDay");	
    std::string strTradingday = "20141017";
    funcSetTrayDay(strTradingday.c_str());

    Func_ShowInstrument funcShowInstrument = (Func_ShowInstrument)GetProcAddress(m_HInstance,"ShowInstrument");
    std::string strInstrument = m_strInstrument.GetBuffer(m_strInstrument.GetLength());
    funcShowInstrument(strInstrument);

    //交易时段
    std::string strInstrument2 = m_strInstrument.GetBuffer(m_strInstrument.GetLength());
    std::vector<std::pair<std::string,std::string>> TradingTime;
    TradingTime.push_back(std::make_pair("09:15", "11:30"));//第一交易时段的起止时间
    TradingTime.push_back(std::make_pair("13:00", "15:15"));//第二交易时段的起止时间
    Func_SetTradingScales funcSetTradingScales = (Func_SetTradingScales)GetProcAddress(m_HInstance,"SetTradingScales");
    funcSetTradingScales(strInstrument2, TradingTime);

    Func_InitFinish funcInitFinish = (Func_InitFinish)GetProcAddress(m_HInstance,"InitFinish");
    funcInitFinish(strInstrument);

    return 1;
}

void CT_PriceModuleDialog::UpdataColorMode(BK_COLOR_MODE eMode)
{
    m_listQuot.SetBKColorMode(eMode); 
    SetColorMode(eMode);

    int nItem = 0;//只有一行
    for (int nSubItem = 0; nSubItem < 13; nSubItem++)
    {
        COLORREF crText;
        COLORREF crBackground;
        m_listQuot.GetItemColors(nItem, nSubItem, crText, crBackground);
        m_listQuot.SetItemColors(nItem, nSubItem, crText, (eMode == Color_Light)?RGB(255,255,255):RGB(9,58,117));
    } 

    m_listQuot.SetItemImage(0, 3,m_ColorMode?eUpDarkImage:eUpLightImage);

    m_listQuot.RedrawWindow();
}
