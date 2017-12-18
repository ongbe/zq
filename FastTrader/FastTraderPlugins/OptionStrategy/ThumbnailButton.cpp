// ThumbnailButton.cpp : 
//

#include "stdafx.h"
#include "ThumbnailButton.h"
#include "ThumbNailControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThumbnailButton

CThumbnailButton::CThumbnailButton(CString strategyName)
: m_StrategyName(strategyName)
{
    int nValue = 4;
    m_TopHeight = nValue;
    m_BottomHeight = nValue;
    m_LeftWidth = nValue;
    m_RightWidth = nValue;
    m_State = notInited;
    m_BkColor = RGB(255,0,255);
    
    m_RcId       = 0;       // Resource ID
    m_NumofPics  = 0;  

    m_bClickSelected = false;

    //LOGFONT lFont; 
    //memset(&lFont, 0, sizeof(LOGFONT)); 
    //lFont.lfHeight = 14;
    //lFont.lfWeight = 14;
    //BOOL bRes = myFont.CreateFontIndirect(&lFont); 
    //ASSERT(bRes); 

    //myFont.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,//设置字体
    //    DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
}

CThumbnailButton::~CThumbnailButton()
{
    NormalBitmap.DeleteObject();
    SelectBitmap.DeleteObject();
    FocusBitmap.DeleteObject();
    DisableBitmap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CThumbnailButton, CButton)
	//{{AFX_MSG_MAP(CThumbnailButton)
    ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
    ON_WM_LBUTTONDOWN()
    ON_WM_CREATE()
END_MESSAGE_MAP()


void CThumbnailButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC xdc;
    xdc.Attach( lpDrawItemStruct->hDC );
    CRect rc;
    GetClientRect(rc);
    CBtnMemDC dc(&xdc,rc);
    
    UINT state = lpDrawItemStruct->itemState ;
    bool IsDisable = false;

	if (state & ODS_FOCUS)
    {
		DrawBitmap( &dc, focus );
	}
	else if (state & ODS_DISABLED)
    {
        IsDisable = true;
    	DrawBitmap( &dc, disable );

    }else if (state & ODS_SELECTED)
    { 
        DrawBitmap( &dc, select );
    }else if(m_bClickSelected)
    {
        DrawBitmap( &dc,focus);
    }
    else
    {
        DrawBitmap( &dc, normal );
    }



    int imode = dc.SetBkMode(TRANSPARENT);
    COLORREF oldColor;

    if(IsDisable)
      oldColor = dc.SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
    else
      oldColor = dc.SetTextColor( RGB(0,0,0) );


    dc.SetTextColor( oldColor );
    dc.SetBkMode(imode );
}


bool CThumbnailButton::LoadBitmaps(CString ResourceName ,int count,int TopHeight, int BottomHeight, int LeftWidth, int RightWidth)
{
    m_TopHeight = TopHeight;
    m_BottomHeight = BottomHeight;
    m_LeftWidth = LeftWidth;
    m_RightWidth = RightWidth;

    //m_RcId       = id;       // Resource ID
    m_NumofPics  = count;      




    HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,ResourceName,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE | LR_LOADFROMFILE);  
    


    CBitmap bmp;
    if( !bmp.Attach(hBitmap) ) return false;

//     if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
//     if( !InitBitmap( bmp, SelectBitmap, 1, count ) ) return false;
//     if( !InitBitmap( bmp, FocusBitmap,  2, count ) ) return false;
//     if( !InitBitmap( bmp, DisableBitmap,3, count ) ) return false;

    if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
    if( !InitBitmap( bmp, FocusBitmap,  1, count ) ) return false;

    bmp.DeleteObject();

    return true;
}

bool CThumbnailButton::InitBitmap( CBitmap & src, CBitmap & dist, int index, int count )
{
    CDC * pDC = GetDC();

    CDC memDC; 
    memDC.CreateCompatibleDC(pDC);

    CDC srcDC;
    srcDC.CreateCompatibleDC(pDC);

    CBitmap* pOldBitmap1;
    pOldBitmap1 = srcDC.SelectObject(&src);

    BITMAP bmpinfo;
    src.GetBitmap(&bmpinfo);
    int bmpWidth = bmpinfo.bmWidth / count;
    int bmpHeight = bmpinfo.bmHeight;
    int orix = bmpWidth * index;

    CRect rc;
    GetClientRect(rc);

    CBitmap* pOldBitmap2;
    dist.DeleteObject();
    dist.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
    pOldBitmap2 = memDC.SelectObject(&dist);

    // lefttop,leftbottom,righttop,rightbottom
    if( !memDC.BitBlt(0,0,m_LeftWidth, m_TopHeight, &srcDC,orix,0,SRCCOPY) ) return false;
    

    if( !memDC.BitBlt(0,rc.bottom - m_BottomHeight,m_LeftWidth, m_BottomHeight, 
                &srcDC,orix,bmpHeight - m_BottomHeight,SRCCOPY) ) return false;


    if( !memDC.BitBlt(rc.right - m_RightWidth, 0 ,m_RightWidth, m_TopHeight, 
                 &srcDC,orix + bmpWidth - m_RightWidth,0,SRCCOPY) ) return false;


    if( !memDC.BitBlt(rc.right - m_RightWidth,rc.bottom - m_BottomHeight,m_RightWidth,m_BottomHeight,
                 &srcDC,orix + bmpWidth - m_RightWidth,bmpHeight - m_BottomHeight,SRCCOPY) ) return false;

    // entireimage
    memDC.StretchBlt(m_LeftWidth,
                     m_TopHeight,
                     rc.Width()  - ( m_LeftWidth + m_RightWidth ) ,
                     rc.Height() -  ( m_TopHeight + m_BottomHeight) ,
                     &srcDC,
                     orix + m_LeftWidth,
                     m_TopHeight,
                     bmpWidth - ( m_LeftWidth + m_RightWidth ) ,
                     bmpHeight - ( m_TopHeight + m_BottomHeight ) ,SRCCOPY);

    
    // topbar,bottombar( Stretch )
    memDC.StretchBlt(m_LeftWidth,0, rc.Width() - (m_LeftWidth + m_RightWidth), m_TopHeight,
                     &srcDC,orix + m_LeftWidth, 0, bmpWidth - ( m_LeftWidth + m_RightWidth) , m_TopHeight,SRCCOPY);

    memDC.StretchBlt(m_LeftWidth, rc.bottom - m_BottomHeight, rc.Width() - ( m_LeftWidth + m_RightWidth), m_BottomHeight,
                     &srcDC,orix + m_LeftWidth,bmpHeight - m_BottomHeight,bmpWidth - ( m_LeftWidth + m_RightWidth) , m_BottomHeight,SRCCOPY );


    // sidebar ( STretch? )
    memDC.StretchBlt(0,m_TopHeight,m_LeftWidth,rc.bottom - m_TopHeight - m_BottomHeight ,
                     &srcDC, orix,m_TopHeight, m_LeftWidth, bmpHeight - ( m_TopHeight + m_BottomHeight ) ,SRCCOPY);

    memDC.StretchBlt(rc.right - m_RightWidth ,m_TopHeight,m_RightWidth,rc.bottom - m_TopHeight - m_BottomHeight ,
                     &srcDC, orix +  bmpWidth - m_RightWidth,m_TopHeight, m_RightWidth, bmpHeight - m_TopHeight - m_BottomHeight ,SRCCOPY);

    srcDC.SelectObject(pOldBitmap1);
    memDC.SelectObject(pOldBitmap2);

    ReleaseDC(pDC);
    ReleaseDC(&srcDC);
    ReleaseDC(&memDC);

     m_State = FileLoaded;
    
    return true;

}

void CThumbnailButton::DrawBitmap( CDC * pDC, int mode )
{
    if( m_State < FileLoaded ) return;

    CRect rc;
    GetClientRect(rc);
 
	COLORREF crOldBack = pDC->SetBkColor(RGB(255,255,255));
	COLORREF crOldText = pDC->SetTextColor(RGB(0,0,0));
	CDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage.CreateCompatibleDC(pDC);
	dcTrans.CreateCompatibleDC(pDC);

	// Select the image into the appropriate dc
    CBitmap* pOldBitmapImage;
    switch(mode)
    {
    case normal:
	   pOldBitmapImage  = dcImage.SelectObject(&NormalBitmap);
       break;
    case select:
	   pOldBitmapImage  = dcImage.SelectObject(&SelectBitmap);
       break;
    case focus:
	   pOldBitmapImage  = dcImage.SelectObject(&FocusBitmap);
       break;
    case disable:
	   pOldBitmapImage  = dcImage.SelectObject(&DisableBitmap);
       break;
    default:
        return;
    }
      
	// Create the mask bitmap
	CBitmap bitmapTrans;
	int nWidth = rc.Width();
	int nHeight = rc.Height();
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

    // Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// Build mask based on transparent colour
	dcImage.SetBkColor(m_BkColor);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);

	// Restore settings
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	pDC->SetBkColor(crOldBack);
	pDC->SetTextColor(crOldText);
}



BOOL CThumbnailButton::OnEraseBkgnd(CDC* pDC) 
{
    return FALSE;
}

void CThumbnailButton::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    ModifyStyle(0, BS_OWNERDRAW);//设置自绘
    CButton::PreSubclassWindow();
}

bool CThumbnailButton::SetClickSelected(bool bSelected)
{
    m_bClickSelected = bSelected;
    return false;
}

bool CThumbnailButton::GetClickSelected(void)
{
    return m_bClickSelected;
}

void CThumbnailButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    GetParent()->SendMessage(UWM_ON_TNB_LCLICKED,(WPARAM)this, (LPARAM)0);

    CButton::OnLButtonDown(nFlags, point);
}

int CThumbnailButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here 
    EnableToolTips(TRUE);
    m_toolTip.Create(this);
    m_toolTip.Activate(TRUE); 
    
    m_toolTip.SetTipBkColor(RGB(222,222,222));
    m_toolTip.SetTipTextColor(RGB(5,5,5));
    //m_toolTip.SetFont(&myFont);


    if(0==strcmp(lpCreateStruct->lpszName,"大涨"))
    {
        m_toolTip.SetTitle(NULL,"大涨");
        m_toolTip.AddTool(this,"市场看多,预期后市还有一波不小的涨幅。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"大跌"))
    {
        m_toolTip.SetTitle(NULL,"大跌");
        m_toolTip.AddTool(this,"市场看空，预期后市还有 不小的跌幅。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"触顶回落"))
    {
        m_toolTip.SetTitle(NULL,"触顶回落");
        m_toolTip.AddTool(this,"标的物价格将见顶回落，预期后市转空或者进行调整。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"触底回升"))
    {
        m_toolTip.SetTitle(NULL,"触底回升");
        m_toolTip.AddTool(this,"标的物价格触底回升，预期后市温和看多。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"突破-买入跨式"))
    {
        m_toolTip.SetTitle(NULL,"突破-买入跨式");
        m_toolTip.AddTool(this,"标的合约将有剧烈行情，但不确定是涨还是跌。波动率大有利。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"突破-买入勒式"))
    {
        m_toolTip.SetTitle(NULL,"突破-买入勒式");
        m_toolTip.AddTool(this,"标的合约将有剧烈行情，但不确定是涨还是跌。成本较低，收益相对也低，需要大涨跌才能获利。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"盘整-卖出跨式"))
    {
        m_toolTip.SetTitle(NULL,"盘整-卖出跨式");
        m_toolTip.AddTool(this,"判断标的物合约不会有太大变动。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"盘整-卖出勒式"))
    {
        m_toolTip.SetTitle(NULL,"盘整-卖出勒式");
        m_toolTip.AddTool(this,"判断标的物合约不会有太大变动。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"盘涨-Call"))
    {
        m_toolTip.SetTitle(NULL,"盘涨-Call");
        m_toolTip.AddTool(this,"标的物合约会涨但是不会涨太多。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"盘涨-Put"))
    {
        m_toolTip.SetTitle(NULL,"盘涨-Put");
        m_toolTip.AddTool(this,"标的物合约会涨但是不会涨太多。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"盘跌-Call"))
    {
        m_toolTip.SetTitle(NULL,"盘跌-Call");
        m_toolTip.AddTool(this,"标的物合约会跌但是不会跌太多。");
    }
    else if(0==strcmp(lpCreateStruct->lpszName,"盘跌-Put"))
    {
        m_toolTip.SetTitle(NULL,"盘跌-Put");
        m_toolTip.AddTool(this,"标的物合约会跌但是不会跌太多。");
    }

    return 0;
}

BOOL CThumbnailButton::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (NULL != m_toolTip.GetSafeHwnd())
    {
        m_toolTip.RelayEvent(pMsg);
    }

    return CButton::PreTranslateMessage(pMsg);
}