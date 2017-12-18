// TradeAnalyseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "TabTradeAnalyseDialog.h"


// CTabTradeAnalyseDialog dialog

IMPLEMENT_DYNAMIC(CTabTradeAnalyseDialog, CDialog)

CTabTradeAnalyseDialog::CTabTradeAnalyseDialog(CWnd* pParent /*=NULL*/)
	: CBKDialog(CTabTradeAnalyseDialog::IDD, pParent)
{
    m_style=StyleStretch;
}

CTabTradeAnalyseDialog::~CTabTradeAnalyseDialog()
{
}

void CTabTradeAnalyseDialog::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_BOX, m_listBoxEx);
}


BEGIN_MESSAGE_MAP(CTabTradeAnalyseDialog, CBKDialog)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTabTradeAnalyseDialog message handlers

BOOL CTabTradeAnalyseDialog::OnInitDialog()
{
    CBKDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_listBoxEx.SetBkColor((m_ColorMode==Color_Light)?RGB(255,255,255):RGB(9,58,117),(m_ColorMode==Color_Light)?RGB(10,67,137):RGB(120,0,1));
    m_listBoxEx.SetBMPSize(18,18);
    m_listBoxEx.SetTextColor((m_ColorMode==Color_Light)?RGB(0,0,0):RGB(255,255,255),(m_ColorMode==Color_Light)?RGB(255,255,255):RGB(255,255,255));
    m_listBoxEx.SetItemHeight(18);

    for (int i=0;i<=1;i++){
        //m_DemoList.AddString("Hey World");
        m_listBoxEx.AddItem ((m_ColorMode==Color_Light)?IDB_BITMAPTabListBoxIcon:IDB_BITMAPTabListBoxIcon_Deep,"Hello World!");
        //m_DemoList.AddItem (NO_BMP_ITEM,"Hi World!");
        //m_DemoList.InsertItem(2,BLANK_BMP, "Greetings");
        //m_DemoList.AddItem (IDB_COOL,"Vacation's Great!");
        //m_listBoxEx.InsertItem(1,IDB_COOL, "Greetings");
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabTradeAnalyseDialog::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if(m_listBoxEx.GetSafeHwnd())
        m_listBoxEx.MoveWindow(0,0,cx,cy);
}

BOOL CTabTradeAnalyseDialog::OnEraseBkgnd(CDC* pDC) 
{
    CBKDialog::OnEraseBkgnd(pDC);	
    if(!m_bitmap.m_hObject)
        return true;

    CRect rect;
    GetClientRect(&rect);
    CDC dc;
    dc.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = dc.SelectObject(&m_bitmap);
    int bmw, bmh ;
    BITMAP bmap;
    m_bitmap.GetBitmap(&bmap);
    bmw = bmap.bmWidth;
    bmh = bmap.bmHeight;
    int xo=0, yo=0;

    if(m_style == StyleTile)
    {
        for (yo = 0; yo < rect.Height(); yo += bmh)
        {
            for (xo = 0; xo < rect.Width(); xo += bmw)
            {
                pDC->BitBlt (xo, yo, rect.Width(),
                    rect.Height(), &dc,
                    0, 0, SRCCOPY);
            }
        }

    }

    if(m_style == StyleCenter)
    {
        if(bmw < rect.Width())
            xo = (rect.Width() - bmw)/2;
        else 
            xo=0;
        if(bmh < rect.Height())
            yo = (rect.Height()-bmh)/2;
        else
            yo=0;
        pDC->BitBlt (xo, yo, rect.Width(),
            rect.Height(), &dc,
            0, 0, SRCCOPY);
    }

    if(m_style == StyleStretch)
    {
        pDC->StretchBlt(xo, yo, rect.Width(),
            rect.Height(), &dc,
            0, 0,bmw,bmh, SRCCOPY);
    }


    dc.SelectObject(pOldBitmap);


    return true;
}

void CTabTradeAnalyseDialog::SetBitmapStyle(int style)
{
    if((style==StyleTile)||
        (style==StyleCenter)||
        (style==StyleStretch))
    {	
        m_style = style;
    }

}

int CTabTradeAnalyseDialog::SetBitmap(UINT nIDResource)
{
    if(m_bitmap.LoadBitmap(nIDResource))
        return 0;
    else
        return 1;//error
}