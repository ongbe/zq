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
,m_pAlgoData(NULL)
, m_nType(0)
, m_bPaint(FALSE)
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
bool CChartCtl::CreateChildren(int nFlag /*= -1*/)
{
	HWND hWnd=GetSafeHwnd();
	if(hWnd==NULL) return false;

    CTimeLineWin2::RegistWndClass();
	m_TimeLineWin.CreateWnd((int)hWnd);
    m_TimeLineWin.SetPaintFlag(nFlag);

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

void CChartCtl::setcontent(CAlgoAutoData *pAlgo,time_t StartTradingUTC)
{
	if(pAlgo==NULL)
        return;

    m_pAlgoData = pAlgo;
//    m_TimeLineWin.setRange2(StartTradingUTC,(int)((double)m_TimeLineWin.getSeconds_EachDay()*pAlgo->T));
	m_TimeLineWin.setRange(pAlgo->StartTime.GetTime(),pAlgo->EndTime.GetTime());
    m_TimeLineWin.refresh();

	::InvalidateRect(m_hWnd,NULL,FALSE);
	::SendMessage(m_hWnd,WM_PAINT,0,0);
}
//刷新已成交量。只刷新指定节。
void CChartCtl::RefreshTradedVolume(int Type)
{
    m_nType = Type;

    m_TimeLineWin.SetPaint(TRUE);
    m_bPaint = TRUE;

	RECT rect;
	GetClientRect(&rect);

	::InvalidateRect(GetSafeHwnd(),&rect,false);
	::SendMessage(GetSafeHwnd(),WM_PAINT,0,0);

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
        int xPos = 20;
        ::MoveWindow((HWND)m_TimeLineWin.m_hWnd,xPos,rect.bottom-m_TimeLineWin.getCfg().WndHei,ClientRect.Width()-xPos,m_TimeLineWin.getCfg().WndHei,TRUE);
	}

}


void CChartCtl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	RECT rect;
	GetClientRect(&rect);

    CDC memdc;
    memdc.CreateCompatibleDC(&dc);
    CBitmap MemBmp;
    MemBmp.CreateCompatibleBitmap(&dc,rect.right-rect.left,rect.bottom-rect.top);
    CBitmap *pOldBmp=memdc.SelectObject(&MemBmp);
    memdc.FillSolidRect(0,0,rect.right-rect.left,rect.bottom-rect.top,BkCol_ChartCtl);


    rect.bottom-=m_TimeLineWin.getCfg().WndHei;
	int wid=rect.right-rect.left;
	int hei=rect.bottom-rect.top;
    
    if (m_pAlgoData == NULL || !m_bPaint) return;    

    char buf[256];
    int tmppox,tmpX,tmpY,gridwid;
    int tmpnjhei,tmpnj2hei;
    int nOrderCountMax1=-9999;
    int nOrderCountMax2=-9999;

    if(0 == m_nType)
    {        
        for(int i=0;i<m_pAlgoData->TradingMinute;i++)
        {
            if (nOrderCountMax1 < m_pAlgoData->vecTradedVolumePerMinute.at(i))
                nOrderCountMax1 = m_pAlgoData->vecTradedVolumePerMinute.at(i);
        }
        
        for(int i=0;i<m_pAlgoData->TradingMinute;i++)
        {
            tmppox=i*wid/m_pAlgoData->TradingMinute + 20;//与OnSize中的xPos相等
            gridwid=(0 == wid/m_pAlgoData->TradingMinute)?1:wid/m_pAlgoData->TradingMinute;
            if(m_pAlgoData->vecTradedVolumePerMinute.at(i) == 0)
                tmpnjhei=0;
            else
                tmpnjhei=(int)((m_pAlgoData->vecTradedVolumePerMinute[i]*hei)/nOrderCountMax1);
            
            if(i%2 == 0)
                memdc.FillSolidRect(tmppox,hei-tmpnjhei,gridwid,tmpnjhei,njCol_ChartCtl);
            else if(i%2 == 1)
                memdc.FillSolidRect(tmppox,hei-tmpnjhei,gridwid,tmpnjhei,nj2Col_ChartCtl);
        }

        if (m_bPaint)
        {          
            CPen lpen(PS_SOLID|PS_GEOMETRIC, 1, RGB(255,0,0));
            CPen* pOldPen = memdc.SelectObject(&lpen);

            int xPos = 20;
            int yPos = hei;

            memdc.MoveTo(xPos,0);
            memdc.LineTo(xPos,hei);

            //画横坐标线
            memdc.MoveTo(xPos,hei);
            memdc.LineTo(xPos+wid,hei);

            memdc.SelectObject(pOldPen);
            lpen.DeleteObject();

            CFont font;
            font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,"新宋体");
            CFont* poldFont=memdc.SelectObject(&font);
            memdc.SetTextColor(RGB(255,0,0));
            memdc.SetBkMode(TRANSPARENT);

            //画刻度
            xPos -= 4;

            int N = 2;//分2段
            int H = hei / N;//每段长度
            if(nOrderCountMax1%2 == 1)
				nOrderCountMax1 -= 1;
			int V = nOrderCountMax1 / N;//每段的下单量

            for (int i=0;i<N+1;i++)
            {
                yPos -= i*H;

                memdc.FillSolidRect(xPos,yPos,5,1,VCol_ChartCtl);
                yPos = hei;
			}

			if(nOrderCountMax1>0)
			{
				//画数值
				xPos = 0;
				yPos = hei;
				for (int j=0;j<N+1;j++)
				{
					yPos -= j*H +5;

					sprintf_s(buf,sizeof(buf)-1,"%d",0 + j*V);

					if(j == N)
						memdc.TextOutA(xPos,yPos+6,buf,strlen(buf));
					else
						memdc.TextOutA(xPos,yPos,buf,strlen(buf));

					yPos = hei;

					if(V == 0)
						break;
				}

				memdc.SelectObject(poldFont);
				font.DeleteObject();  
			}
	
        }
     }

    CFont font;
    font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,"新宋体");
    CFont* poldFont=memdc.SelectObject(&font);
    memdc.SetTextColor(RGB(255,0,0));
    memdc.SetBkMode(TRANSPARENT);      

    int realWid = wid-20;
    if(1 == m_nType)
    {
		int T = m_pAlgoData->TradingTime;
        for(int i=0;i<T;i++)
        {
            if (nOrderCountMax2 < m_pAlgoData->vecTradedVolumePerTime.at(i))
                nOrderCountMax2 = m_pAlgoData->vecTradedVolumePerTime.at(i);
        }

        if (m_bPaint)
        { 
            CPen lpen(PS_SOLID|PS_GEOMETRIC, 1, RGB(255,0,0));
            CPen* pOldPen = memdc.SelectObject(&lpen);

            int xPos = 20;
            int yPos = hei;

            memdc.MoveTo(xPos,0);
            memdc.LineTo(xPos,hei);

            //画横坐标线
            memdc.MoveTo(xPos,hei);
            memdc.LineTo(xPos+realWid,hei);

            memdc.SelectObject(pOldPen);
            lpen.DeleteObject();

            //画刻度
            xPos -= 4;

			int N = 2;//分5段
            int H = hei / N;//每段长度
            if((nOrderCountMax2)%2 == 1)
                nOrderCountMax2 += 1;
            int V = nOrderCountMax2 / N;//每段的下单量

            for (int i=0;i<N+1;i++)
            {
                yPos -= i*H;

                memdc.FillSolidRect(xPos,yPos,5,1,VCol_ChartCtl);
                yPos = hei;
            }

            //画数值
            xPos = 0;
            yPos = hei;
            for (int j=0;j<N+1;j++)
            {
                yPos -= j*H +5;

                sprintf_s(buf,sizeof(buf)-1,"%d",0 + j*V);

                if(j == N)
                    memdc.TextOutA(xPos,yPos+6,buf,strlen(buf));
                else
                    memdc.TextOutA(xPos,yPos,buf,strlen(buf));

                yPos = hei;

				if(0 == V)
					break;
            }
        }  

        for(int i=0;i<T;i++)
        {
            tmppox=i*realWid/T + 20;//与OnSize中的xPos相等
            gridwid=(0 == realWid/T)?1:realWid/T;
            if(m_pAlgoData->vecTradedVolumePerTime.at(i) == 0)
                tmpnj2hei=0;
            else
                tmpnj2hei=(int)((m_pAlgoData->vecTradedVolumePerTime[i]*hei)/nOrderCountMax2);

            if(i%2 == 0)
                memdc.FillSolidRect(tmppox,hei-tmpnj2hei,gridwid,tmpnj2hei,njCol_ChartCtl);
            else if(i%2 == 1)
                memdc.FillSolidRect(tmppox,hei-tmpnj2hei,gridwid,tmpnj2hei,nj2Col_ChartCtl);

            //画次数
            int tmp = tmppox + gridwid;
            if(i<9)
                tmpX = (tmppox+tmp)/2-2;
            else
                tmpX = (tmppox+tmp)/2 - 5;

            if(i == T-1)
                tmpX = (tmppox+tmp)/2 - 14;

            tmpY = hei + 2;
            sprintf_s(buf,sizeof(buf)-1,"%d",i+1);
            memdc.TextOutA(tmpX,tmpY,buf,strlen(buf));
        }

        memdc.SelectObject(poldFont);
        font.DeleteObject(); 
    }

    GetClientRect(&rect);
	dc.BitBlt(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,&memdc,0,0,SRCCOPY);
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
		pt.x += 20;
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
	point.x -= 20;
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
