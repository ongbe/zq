// ChartCtl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartCtl.h"


// CChartCtl

#ifdef _DEBUG
	#pragma comment(lib,"ToolsD.lib")
#else
	#pragma comment(lib,"Tools.lib")
#endif


IMPLEMENT_DYNAMIC(CChartCtl, CWnd)

CChartCtl::CChartCtl()
:m_TimeLineWin(Stru_TimeLineWin2_Cfg(0,BkCol_ChartCtl))
{
	RegisterWndClass();
	m_bMouseInClient=false;
	m_LastMousePt.x=m_LastMousePt.y=0;

}
CChartCtl::~CChartCtl()
{
}

BOOL CChartCtl::RegisterWndClass()
{
    WNDCLASS windowclass;
    HINSTANCE hInst = AfxGetInstanceHandle();

    //Check weather the class is registerd already
    if (!(::GetClassInfo(hInst, ClassName_ChartCtl, &windowclass)))
    {
        //If not then we have to register the new class
        windowclass.style = CS_DBLCLKS;// | CS_HREDRAW | CS_VREDRAW;
        windowclass.lpfnWndProc = ::DefWindowProc;
        windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
        windowclass.hInstance = hInst;
        windowclass.hIcon = NULL;
        windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
        windowclass.lpszMenuName = NULL;
        windowclass.lpszClassName = ClassName_ChartCtl;

        if (!AfxRegisterClass(&windowclass))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}



//创建子窗口。因为自定义控件无法响应OnCreate事件，所以只能显示的创建
bool CChartCtl::CreateChildren(void)
{
	HWND hWnd=GetSafeHwnd();
	if(hWnd==NULL) return false;

    CTimeLineWin2::RegistWndClass();
	m_TimeLineWin.CreateWnd((int)hWnd);

    CEasyTimespan<int> 		Timespan_IntraDay;
	//默认商品期货 9:00-10:15 10:30-11:30 13:00-15:00, total 4h15'(1h15'+1h+2h)
    Timespan_IntraDay.insert(9*3600,10*3600+15*60);
    Timespan_IntraDay.insert(10*3600+30*60,11*3600+30*60);
    Timespan_IntraDay.insert(13*3600+30*60,15*3600);
    CEasyTradingTimespan TradingTimespans(true,
                                            Timespan_IntraDay,
                                            CEasyTimespan<time_t>(),
											true,
                                            Stru_TimezoneDst());

    m_TimeLineWin.setTradingTimespan(TradingTimespans);

	time_t curtime;
	time(&curtime);
	m_TimeLineWin.setRange(curtime,curtime+24*3600);

    m_TimeLineWin.refresh();

	SetTimer(1001,100,NULL);
	return true;
}

void CChartCtl::setcontent(const CAlgo_No1& algo,time_t StartTradingUTC)
{
	m_algo=algo;

	m_TimeLineWin.setRange2(StartTradingUTC,(int)((double)m_TimeLineWin.getSeconds_EachDay()*m_algo.T));
	m_TimeLineWin.refresh();

	::InvalidateRect(m_hWnd,NULL,FALSE);
	::SendMessage(m_hWnd,WM_PAINT,0,0);
}
//刷新已成交量。只刷新指定节。
void CChartCtl::RefreshTradedVolume(int TradingSession,int TradedVolume)
{
	int cnt=m_algo.vecnj.size();
	if(TradingSession>=0&&TradingSession<cnt)
	{
		m_algo.vecTradedVolume[TradingSession]=TradedVolume;

		RECT rect;
		GetClientRect(&rect);
		int wid=rect.right-rect.left;
		rect.left=TradingSession*wid/cnt;
		rect.right=(TradingSession+1)*wid/cnt;

		::InvalidateRect(GetSafeHwnd(),&rect,false);
		::SendMessage(GetSafeHwnd(),WM_PAINT,0,0);
	}
}
//utc是否已经超时
bool CChartCtl::isovertime(time_t utc)
{
    time_t curtime;
    time(&curtime);
    return m_TimeLineWin.isovertime(curtime);
}
//根据时间取x轴位置
int CChartCtl::getxPos(time_t utc)
{
    return m_TimeLineWin.getxPos(utc);
}
//取时间轴窗口宽度
int CChartCtl::getTimeWndWid(void)
{
    return m_TimeLineWin.getTimeLineWid()+m_TimeLineWin.LeftSpaceWid;
}
//重置交易时间参数
void CChartCtl::setTradingTimespan(const CEasyTradingTimespan& TradingTimespan) 
{ 
    m_TimeLineWin.setTradingTimespan(TradingTimespan);
}
//刷新时间刻度窗口的参数
void CChartCtl::refreshTimeLineWin(void) 
{ 
    m_TimeLineWin.refresh();
}


BEGIN_MESSAGE_MAP(CChartCtl, CWnd)
	ON_WM_SIZE()
//	ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_TIMER()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChartCtl message handlers


void CChartCtl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	RECT rect;
	GetClientRect(&rect);
	CRect ClientRect=rect;
	
	if(m_TimeLineWin.m_hWnd!=NULL)
	{
		::MoveWindow((HWND)m_TimeLineWin.m_hWnd,0,rect.bottom-m_TimeLineWin.getCfg().WndHei,ClientRect.Width(),m_TimeLineWin.getCfg().WndHei,TRUE);
	}

}


void CChartCtl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	RECT rect;
	GetClientRect(&rect);
	rect.bottom-=m_TimeLineWin.getCfg().WndHei;
	int wid=rect.right-rect.left;
	int hei=rect.bottom-rect.top;

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(&dc,wid,hei);
	CBitmap *pOldBmp=memdc.SelectObject(&MemBmp);
	memdc.FillSolidRect(0,0,wid,hei,BkCol_ChartCtl);

	if(m_algo.N>0&&(int)m_algo.vecnj.size()==m_algo.N&&(int)m_algo.vecnj2.size()==m_algo.N&&m_algo.vecTradedVolume.size()==m_algo.N)
	{
		double dmaxvolume=m_algo.maxnj>m_algo.maxnj2?m_algo.maxnj:m_algo.maxnj2;
		double dfactor=(double)hei*3.0f/4.0f/dmaxvolume;
		int gridwid=wid/m_algo.N;

		int tmpwid=gridwid-(gridwid>=4?2:0);
		if(tmpwid%2==1) tmpwid--;
		if(tmpwid<2) tmpwid=2;

		int tmppox;
		int tmpnjhei,tmpnj2hei,tmptradedhei;

		for(int i=0;i<m_algo.N;i++)
		{
			tmppox=i*wid/m_algo.N+(gridwid>=4?1:0);

			tmpnjhei=(int)(m_algo.vecnj[i]*dfactor);
			if(tmpnjhei<0) tmpnjhei*=-1;
			tmpnj2hei=(int)(m_algo.vecnj2[i]*dfactor);
			if(tmpnj2hei<0) tmpnj2hei*=-1;
			tmptradedhei=(int)((double)m_algo.vecTradedVolume[i]*dfactor);

			memdc.FillSolidRect(tmppox,hei-tmpnjhei,tmpwid/2,tmpnjhei,njCol_ChartCtl);
			if(tmptradedhei>0&&!m_algo.bTradeTrend&&m_algo.vecTradedVolume[i]>0&&tmpwid/2>2)
				memdc.FillSolidRect(tmppox,hei-tmptradedhei,tmpwid/2,tmptradedhei,njTradedCol_ChartCtl);

			memdc.FillSolidRect(tmppox+tmpwid/2,hei-tmpnj2hei,tmpwid/2,tmpnj2hei,nj2Col_ChartCtl);
			if(tmptradedhei>0&&m_algo.bTradeTrend&&m_algo.vecTradedVolume[i]>0&&tmpwid/2>2)
				memdc.FillSolidRect(tmppox+tmpwid/2,hei-tmptradedhei,tmpwid/2,tmptradedhei,nj2TradedCol_ChartCtl);
		}
	}

	CFont font;
	font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,"新宋体");
	CFont* poldFont=memdc.SelectObject(&font);
	memdc.SetTextColor(RGB(0,0,0));
	memdc.SetBkMode(TRANSPARENT);
	char buf[256];
	strcpy_s(buf,sizeof(buf)-1,"nj无趋势：");
	memdc.TextOutA(wid-150,10,buf,strlen(buf));
	memdc.FillSolidRect(wid-90,10,60,12,njCol_ChartCtl);

	strcpy_s(buf,sizeof(buf)-1,"nj有趋势：");
	memdc.TextOutA(wid-150,26,buf,strlen(buf));
	memdc.FillSolidRect(wid-90,26,60,12,nj2Col_ChartCtl);
	memdc.SelectObject(poldFont);
	font.DeleteObject();

	dc.BitBlt(rect.left,rect.top,wid,hei,&memdc,0,0,SRCCOPY);
	memdc.DeleteDC();
	MemBmp.DeleteObject();
}

void CChartCtl::OnTimer(UINT_PTR nIDEvent)
{
	HWND hWnd;
	if(m_bMouseInClient&&(hWnd=GetSafeHwnd())!=NULL)
	{
		POINT pt;
		GetCursorPos(&pt);
		::ScreenToClient(hWnd,&pt);
		RECT rect;
		::GetClientRect(hWnd,&rect);
		if(!::PtInRect(&rect,pt))
		{
			m_bMouseInClient=false;
			m_TimeLineWin.ClearTimeTip();
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CChartCtl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(point.x!=m_LastMousePt.x||point.y!=m_LastMousePt.y)
	{
		m_LastMousePt.x=point.x;
		m_LastMousePt.y=point.y;

		RECT rect;
		::GetClientRect(GetSafeHwnd(),&rect);

		bool bInClient=point.y>=rect.bottom-m_TimeLineWin.getCfg().WndHei?false:true;
		if(m_bMouseInClient&&!bInClient)
			m_TimeLineWin.ClearTimeTip();
		else
			m_TimeLineWin.SetTimeTip(point.x);

		m_bMouseInClient=bInClient;
	}

	CWnd::OnMouseMove(nFlags, point);
}
