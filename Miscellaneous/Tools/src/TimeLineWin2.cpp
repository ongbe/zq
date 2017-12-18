// TimeLineWin.cpp: implementation of the CTimeLineWin2 class.
//
//////////////////////////////////////////////////////////////////////


#include "TimeLineWin2.h"
#include <windows.h>
#include <map>
#include <set>
using namespace std;


#ifdef WIN32
#pragma unmanaged
#endif 

Ceasymutex				CTimeLineWin2::m_mutex;
map<int,CTimeLineWin2*>	CTimeLineWin2::m_mapTimeLineWins;

//窗口处理函数，用于win32 api创建的窗口。主要处理WM_SIZE和WM_PAINT
LRESULT WINAPI TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);


Stru_TimeLineWin2_Data::Stru_TimeLineWin2_Data(int TimeLineWid,
                                             const CEasyTimespan<time_t>& WholePeriod,
                                             const CEasyTradingTimespan& TradingTimespan)
{
    m_TimeLineWid=TimeLineWid;
    m_WholePeriod=WholePeriod;                  //时间刻度起止时间
    m_TradingTimespan=TradingTimespan;          //交易时段定义
    m_MarkUnit=0;
	m_WeekendDaysInclued=0;
    m_totalsecs=0;

	refresh();
}

//utc是否已经超时
bool Stru_TimeLineWin2_Data::isovertime(time_t utc) const
{
    time_t lastend=m_TimeSpans.getLastTimespan().end;
    return lastend==(time_t)0 || utc>=lastend ? true : false;
}
//根据x轴位置取相应时间
time_t Stru_TimeLineWin2_Data::getUTC(int xPos) const
{
    if(xPos<=0||m_TimeLineWid==0) return m_TimeSpans.getFirstTimespan().start;
    if(xPos>=m_TimeLineWid) return m_TimeSpans.getLastTimespan().end;

    int offset=xPos*m_totalsecs/m_TimeLineWid;
    return m_TimeSpans.getTimeByOffset(offset);
}
//根据时间取x轴位置
int Stru_TimeLineWin2_Data::getxPos(time_t utc) const
{
    if(m_totalsecs==0) return -1;

    int offset=m_TimeSpans.getOffsetByTime(utc);
    if(offset<0||offset>=m_totalsecs) return -1;

    return offset*m_TimeLineWid/m_totalsecs;
}

//获取刻度的时间集
void Stru_TimeLineWin2_Data::getScale(set<time_t>& longScale,set<time_t>& shortScale) const
{
    longScale=m_LongScaleUTC;
    shortScale=m_ShortScaleUTC;
}
//根据起始时间和窗口宽度，计算刻度的时间
void Stru_TimeLineWin2_Data::calcuPeriods_internal(void)
{
    m_TimeSpans.clear();
    m_WeekendDaysInclued=0;
    m_totalsecs=0;

    const map<time_t,Stru_TimespanInfo<time_t>>& wholeperiod=m_WholePeriod.getTimespans();

    if(wholeperiod.size()==1)
    {
        m_TimeSpans=m_TradingTimespan.getTradingTimespan_InPeriod(wholeperiod.begin()->first,
                                                                    wholeperiod.begin()->second.end,
                                                                    m_WeekendDaysInclued);
        m_totalsecs=(int)m_TimeSpans.getTimeDuration();
    }
}


//计算刻度线位置
void Stru_TimeLineWin2_Data::calcuScalePos_internal(void)
{
    m_MarkUnit=0;
	m_BreakScaleUTC.clear();
	m_BreakScalePos.clear();
    m_LongScaleUTC.clear();
    m_LongScalePos.clear();
    m_ShortScaleUTC.clear();
    m_ShortScalePos.clear();

    if((int)m_TimeSpans.getTimespans().size()==0) 
        return;

    const map<time_t,Stru_TimespanInfo<time_t>>& mapTimeSpans=m_TimeSpans.getTimespans();
    map<time_t,Stru_TimespanInfo<time_t>>::const_iterator it_TimeSpans;

    short syear,smon,sday,shour,smin,ssec;
	time_t starttime,endtime,tmputc;
	int SecondsIntraday;
	if(m_TradingTimespan.isLocalTzDST())
	{
		CEasyTime tt(m_TimeSpans.getFirstTimespan().start,0);
		starttime=tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		endtime=CEasyTime(m_TimeSpans.getLastTimespan().end,0).getutc();
	}
	else
	{
		CEasyTime2 tt(m_TimeSpans.getFirstTimespan().start,0,m_TradingTimespan.getTimezoneDst());
		starttime=tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		endtime=CEasyTime2(m_TimeSpans.getLastTimespan().end,0,m_TradingTimespan.getTimezoneDst()).getutc();
	}

    //计算时间跨度(日)
    int i=(int)(endtime-starttime+SecondsIntraday);
    int SpanDays=i/(24*3600)+(i%(24*3600)==0?0:1);
    SpanDays-=m_WeekendDaysInclued;
    if(SpanDays<0) SpanDays=0;
    
	//计算时间跨度(小时)
	int SpanHours=m_totalsecs/3600+(m_totalsecs%3600!=0?1:0);

    //确定标注单位，超过3天按日标注，否则按小时标注
    m_MarkUnit = SpanDays>3 ? 1 : 0;

	//所有时间段的开始都画长刻度
    for(it_TimeSpans=mapTimeSpans.begin();it_TimeSpans!=mapTimeSpans.end();it_TimeSpans++)
    {
		m_BreakScaleUTC.insert(it_TimeSpans->first);
    }

    //计算刻度值
    if(m_MarkUnit==1)		//SpanDays>3
    {
        if(m_TimeLineWid/SpanDays>=40)
        {
            //按日长刻度，没有短刻度
			if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
			else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
            while(tmputc<=endtime)
            {
                if(m_TimeSpans.isInTimespans(tmputc))
					m_LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    m_LongScaleUTC.insert(it_TimeSpans->first);
                }

                tmputc+=24*3600;
            }
        }
        else if(SpanDays>=30&&m_TimeLineWid/(SpanDays/30)>=40)
        {
            //按月长刻度，没有短刻度
            int year=syear;
            int mon=smon;
            while(true)
            {
				if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(year,mon,1).gettimeb().time;
				else tmputc=CEasyTime2(year,mon,1,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
				if(tmputc>endtime) break;

                if(m_TimeSpans.isInTimespans(tmputc))
					m_LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    m_LongScaleUTC.insert(it_TimeSpans->first);
                }

                mon++;
                if(mon>12) { year++;  mon=1; }
            }
        }
        else
        {
            //按年长刻度，没有短刻度
            int year=syear;
            while(true)
            {
				if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(year,1,1).gettimeb().time;
				else tmputc=CEasyTime2(year,1,1,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
				if(tmputc>endtime) break;

				if(m_TimeSpans.isInTimespans(tmputc))
					m_LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    m_LongScaleUTC.insert(it_TimeSpans->first);
                }

                year++;
            }
        }
    }
    else if(m_MarkUnit==0&&SpanHours>0)
    {
        int longhours;  //长刻度按几小时一格
        int shourmins;  //短刻度按几分钟一格
        if(m_TimeLineWid/SpanHours>=80)
        {
            //按小时为长刻度，按15/30/45分钟为短刻度
            longhours=1;
            shourmins=15;
        }
        else if(m_TimeLineWid/SpanHours*4>=100)
        {
            //按4小时为长刻度，按1小时为短刻度
            longhours=4;
            shourmins=60;
        }
        else 
        {
            //按12小时为长刻度，按1小时为短刻度
            longhours=12;
            shourmins=60;
        }

        //计算长刻度
		if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
		else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
        while(tmputc<=endtime)
        {
            if(m_TimeSpans.isInTimespans(tmputc))
				m_LongScaleUTC.insert(tmputc);
            tmputc+=longhours*3600;
        }
        //计算短刻度
		if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
		else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
        while(tmputc<=endtime)
        {
            if(m_TimeSpans.isInTimespans(tmputc))
				m_ShortScaleUTC.insert(tmputc);
            tmputc+=shourmins*60;
        }
    }

	//计算刻度位置
    set<time_t>::const_iterator it;
    for(it=m_BreakScaleUTC.begin();it!=m_BreakScaleUTC.end();it++)
        m_BreakScalePos.insert(getxPos(*it));
    for(it=m_LongScaleUTC.begin();it!=m_LongScaleUTC.end();it++)
        m_LongScalePos.insert(getxPos(*it));
    for(it=m_ShortScaleUTC.begin();it!=m_ShortScaleUTC.end();it++)
        m_ShortScalePos.insert(getxPos(*it));
}


//取每天合法的秒数
int Stru_TimeLineWin2_Data::getSeconds_EachDay(void) const
{
    return m_TradingTimespan.getSeconds_EachDay();
}

void Stru_TimeLineWin2_Data::setRange(time_t start,time_t end) 
{ 
    m_WholePeriod.clear();
    m_WholePeriod.insert(start,end);
}

//重新设置时间刻度的范围，指定起始时间，和有效秒数
//目前采用一种比较低效的算法，先假设一天，看是否够，不够再增加一天，递增
void Stru_TimeLineWin2_Data::setRange2(time_t StartUTC,int SpanSeconds)
{
	if(SpanSeconds<=0)
	{
        m_WholePeriod.clear();
		return;
	}

	int ValidSecondsEachDay=getSeconds_EachDay();
    if(ValidSecondsEachDay==0)
    {
        m_WholePeriod.clear();
		return;
    }

	int days=(SpanSeconds/ValidSecondsEachDay)+1;
    if(m_TradingTimespan.isExcludeWeekend())
		days+=days/5*2;

	while(days<366)
	{
		setRange(StartUTC,StartUTC+days*24*3600);
		calcuPeriods_internal();
		if(m_totalsecs>=SpanSeconds)
			break;
		days+=2;
	}

	setRange(StartUTC,m_TimeSpans.getTimeByOffset(SpanSeconds));
}







CTimeLineWin2::CTimeLineWin2(const Stru_TimeLineWin2_Cfg& cfg)
:Stru_TimeLineWin2_Cfg(cfg),
 m_hWnd(NULL),
 m_TimeTipPos(-1)
{
}


CTimeLineWin2::~CTimeLineWin2()
{
	if(m_hWnd)
		RemoveWnd(m_hWnd);
}

//注册窗口类
void CTimeLineWin2::RegistWndClass(void)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc,sizeof(wc));
    wc.cbSize				=sizeof(wc);
    wc.style				=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wc.lpfnWndProc	        =TimeLineProc;
    wc.cbClsExtra		    =0;
    wc.cbWndExtra		    =0;
    wc.hInstance		    =GetModuleHandle(NULL); 
    wc.hCursor			    =LoadCursor(NULL,IDC_ARROW);
    wc.hIcon				=NULL;
    wc.hbrBackground        =NULL;
    wc.lpszMenuName         =NULL;
    wc.lpszClassName        =ClsName_TimeLineWin2; 
    wc.hIconSm			    =NULL;
    RegisterClassEx(&wc);
}
//注销窗口类
void CTimeLineWin2::UnregistWndClass(void)
{
    UnregisterClass(ClsName_TimeLineWin2,GetModuleHandle(NULL));
}

int CTimeLineWin2::CreateWnd(int hParent)
{
	if(m_hWnd!=NULL) return m_hWnd;

    DWORD dwStyle=WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE;
    if(hParent) dwStyle|=WS_CHILD;

    m_hWnd=(int)CreateWindow(ClsName_TimeLineWin2,"",dwStyle,0,0,0,WndHei,(HWND)hParent,NULL,GetModuleHandle(NULL),NULL);

	if(m_hWnd)
		AddWnd(m_hWnd);
	return m_hWnd;
}
//设置时间轴窗口句柄。如果用winform创建时间轴窗口，创建后，用这个方法设置hWnd句柄
void CTimeLineWin2::SetWnd(int hWnd)
{
	m_hWnd=hWnd;
	if(m_hWnd)
		AddWnd(m_hWnd);
}


void CTimeLineWin2::OnSize(int iWnd) 
{
    RECT		Rect;
	HWND		hWnd=(HWND)iWnd;
    GetClientRect(hWnd,&Rect);

    setTimeLineWid(Rect.right-Rect.left);
    refresh();

    InvalidateRect(hWnd,NULL,FALSE);
    SendMessage(hWnd,WM_PAINT,0,0L);
}

void CTimeLineWin2::OnPaint(int iWnd)
{
	PAINTSTRUCT			Ps;
	HWND				hWnd=(HWND)iWnd;
	HDC					hDC=BeginPaint((HWND)hWnd,&Ps);
	OnPaint2(iWnd,(int)hDC);
	EndPaint(hWnd,&Ps);
}
void CTimeLineWin2::OnPaint2(int iWnd,int iDC) 
{
    RECT Rect;
    int Wid,Hei;
	HWND	hWnd=(HWND)iWnd;

	GetClientRect(hWnd,&Rect);
	Wid=Rect.right-Rect.left;
	Hei=Rect.bottom-Rect.top;

	HDC		hDC=(HDC)iDC;
	HDC		MemDC;
	HBITMAP	hBmp;
	hBmp=CreateCompatibleBitmap(hDC,Wid,Hei);
	MemDC=CreateCompatibleDC(hDC);
	SelectObject(MemDC,hBmp);

	//时间轴窗口的背景刷、画笔和字体
    HBRUSH		hBrBk=CreateSolidBrush(BkCol);
    HPEN		hPen=CreatePen(PS_SOLID,1,LineCol);
    HFONT       hFont=CreateFont(-1*FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,FontName);

    FillRect(MemDC,&Rect,hBrBk);
    SelectObject(MemDC,hPen);
    SelectObject(MemDC,hFont);
    SetTextColor(MemDC,FontCol);
    SetBkMode(MemDC,TRANSPARENT);

    //画顶层线
    MoveToEx(MemDC,LeftSpaceWid,0,NULL);
    LineTo(MemDC,Wid,0);

    set<int>::const_iterator it_pos;

    //时间段的开始画短刻度线
    for(it_pos=m_BreakScalePos.begin();it_pos!=m_BreakScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,TimespanStartScaleHei);
//		SetPixel(MemDC,(*it_pos)-1,1,LineCol);
//		SetPixel(MemDC,(*it_pos)+1,1,LineCol);
    }

    //画长刻度线
    for(it_pos=m_LongScalePos.begin();it_pos!=m_LongScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,LongScaleHei);
    }

    //画短刻度线
    for(it_pos=m_ShortScalePos.begin();it_pos!=m_ShortScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,ShortScaleHei);
    }

    //标注长刻度
    it_pos=m_LongScalePos.begin();
    set<time_t>::const_iterator it_utc=m_LongScaleUTC.begin();
    int lastlabelpos=-1;
    for(;it_pos!=m_LongScalePos.end()&&it_utc!=m_LongScaleUTC.end();it_pos++,it_utc++)
    {
        char buf[256];
        short year,month,day,hour,min,sec,wday;
		if(m_TradingTimespan.isLocalTzDST()) CEasyTime(*it_utc).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(*it_utc,0,m_TradingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        if(m_MarkUnit==1)
        {
            sprintf(buf,"%d/%d/%d/",year,month,day);
            switch(wday)
            {
                case 0: strcat(buf,"日");  break;
                case 1: strcat(buf,"一");  break;
                case 2: strcat(buf,"二");  break;
                case 3: strcat(buf,"三");  break;
                case 4: strcat(buf,"四");  break;
                case 5: strcat(buf,"五");  break;
                case 6: strcat(buf,"六");  break;
            }
        }
        else
        {
			if(min==0) sprintf(buf,"%d",hour);
			else sprintf(buf,"%d:%d",hour,min);
        }

        SIZE Sz;
        GetTextExtentPoint32(MemDC,buf,strlen(buf),&Sz);
        int pox,poy;
        if(MarkPos==1)
        {
            //刻度写在右边
            pox=(*it_pos)+2;
            poy=2;
        }
        else
        {
            //刻度写在中间
            pox=(*it_pos)-Sz.cx/2;
            poy=Hei-Sz.cy;
        }
        if(pox<0) pox=0;
        if(lastlabelpos==-1||pox>=lastlabelpos+4)
        {
            //避免和上一个刻度标注重合
            TextOut(MemDC,pox,poy,buf,strlen(buf));
            lastlabelpos=pox+Sz.cx;
        }
    }

	//标注TimeTip
	if(m_TimeTipPos>=0&&m_TimeTipPos<Wid)
	{
		MoveToEx(MemDC,m_TimeTipPos,1,NULL);
		LineTo(MemDC,m_TimeTipPos,Hei);

        char buf[256];
        short year,month,day,hour,min,sec,wday;
		time_t TimeTipUTC=getUTC(m_TimeTipPos);
		if(m_TradingTimespan.isLocalTzDST()) CEasyTime(TimeTipUTC).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(TimeTipUTC,0,m_TradingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		buf[0]=0;
		if(m_MarkUnit==1)
		{
			sprintf(buf,"%d/%d/%d/",year,month,day);
			switch(wday)
			{
				case 0: strcat(buf,"日");  break;
				case 1: strcat(buf,"一");  break;
				case 2: strcat(buf,"二");  break;
				case 3: strcat(buf,"三");  break;
				case 4: strcat(buf,"四");  break;
				case 5: strcat(buf,"五");  break;
				case 6: strcat(buf,"六");  break;
			}
			strcat(buf," ");
		}
		sprintf(buf+strlen(buf),"%d:%d:%d",hour,min,sec);

        SIZE Sz;
        GetTextExtentPoint32(MemDC,buf,strlen(buf),&Sz);
		int txtpox=m_TimeTipPos-Sz.cx/2;
		if(txtpox+Sz.cx>Wid) txtpox=Wid-Sz.cx;
		if(txtpox<0) txtpox=0;
		SetTextColor(MemDC,RGB(255,255,255));
		SetBkColor(MemDC,RGB(0,0,255));
		SetBkMode(MemDC,OPAQUE);
        TextOut(MemDC,txtpox,Hei-Sz.cy,buf,strlen(buf));
	}



	BitBlt(hDC,0,0,Wid,Hei,MemDC,0,0,SRCCOPY);

	DeleteDC(MemDC);
	DeleteObject(hBmp);

	if(hBrBk) DeleteObject(hBrBk);
	if(hPen) DeleteObject(hPen);
	if(hFont) DeleteObject(hFont);
}

LRESULT WINAPI TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam) 
{
    switch(Msg)
    {
        case WM_CLOSE:		
        {
			DestroyWindow(hWnd);
            return 0;
        }
        case WM_DESTROY:		
		{
			return 0;
		}
        case WM_SIZE:
        {
			CTimeLineWin2::m_mutex.lock();
			CTimeLineWin2* pObj=CTimeLineWin2::getObj_withoutLocking((int)hWnd);
            if(pObj!=NULL)
                pObj->OnSize((int)hWnd);
			CTimeLineWin2::m_mutex.unlock();
            break;
        }
        case WM_PAINT:
        {
			CTimeLineWin2::m_mutex.lock();
			CTimeLineWin2* pObj=CTimeLineWin2::getObj_withoutLocking((int)hWnd);
            if(pObj!=NULL)
                pObj->OnPaint((int)hWnd);
			CTimeLineWin2::m_mutex.unlock();
            break;
        }
    }
    return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

void CTimeLineWin2::SetTimeTip(int pox)
{
	m_TimeTipPos=pox;
	if(m_hWnd!=NULL)
	{
		::InvalidateRect((HWND)m_hWnd,NULL,FALSE);
		::SendMessage((HWND)m_hWnd,WM_PAINT,0,0);
	}
}

void CTimeLineWin2::ClearTimeTip(void)
{
	m_TimeTipPos=-1;
	if(m_hWnd!=NULL)
	{
		::InvalidateRect((HWND)m_hWnd,NULL,FALSE);
		::SendMessage((HWND)m_hWnd,WM_PAINT,0,0);
	}
}
CTimeLineWin2* CTimeLineWin2::getObj_withoutLocking(int hWnd)
{
    CTimeLineWin2* prlt=NULL;
    map<int,CTimeLineWin2*>::iterator it=m_mapTimeLineWins.find(hWnd);
    if(it!=m_mapTimeLineWins.end()) prlt=it->second;
    return prlt;
}
void CTimeLineWin2::AddWnd(int hWnd)
{
	m_mutex.lock();
	m_mapTimeLineWins.insert(pair<int,CTimeLineWin2*>(hWnd,this));
	m_mutex.unlock();
}
void CTimeLineWin2::RemoveWnd(int hWnd)
{
	m_mutex.lock();
    map<int,CTimeLineWin2*>::iterator it=m_mapTimeLineWins.find(hWnd);
    if(it!=m_mapTimeLineWins.end())
        m_mapTimeLineWins.erase(it);
	m_mutex.unlock();
}


