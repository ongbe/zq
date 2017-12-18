// TimeLineWin.cpp: implementation of the CTimeLineWin class.
//
//////////////////////////////////////////////////////////////////////


#include "TimeLineWin.h"
#include <windows.h>
#include "Tools_Win32.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CTimeLineWin::Stru_Data::Stru_Data(bool bTimeBased)
:	m_bTimeBased(bTimeBased),
	m_WholePeriod(CEasyTimespan<int>()),
	m_WorkingTimespan(CEasyTradingTimespan()),
	m_CacheTimespanOffset(-1),
	m_CacheTimespanStart(-1),
	m_CacheTimespanEnd(-1)
{
	recalcu();
}

void CTimeLineWin::Stru_Data::clearRange(void)
{
    m_WholePeriod.clear();
}

void CTimeLineWin::Stru_Data::setRange(int start,int end) 
{ 
    m_WholePeriod.clear();
    m_WholePeriod.insert(start,end);
}

//重新设置时间刻度的范围，指定起始时间，和有效秒数
//目前采用一种比较低效的算法，先假设一天，看是否够，不够再增加一天，递增
void CTimeLineWin::Stru_Data::setRange2(int start,int SpanUnits)
{
	if(SpanUnits<=0)
	{
        m_WholePeriod.clear();
		return;
	}

	if(m_bTimeBased)
	{
		//对于时间刻度，SpanUnits是秒数
		int ValidSecondsEachDay=getSeconds_EachDay();
		if(ValidSecondsEachDay==0)
		{
			m_WholePeriod.clear();
			return;
		}

		int days=(SpanUnits/ValidSecondsEachDay)+1;
		if(m_WorkingTimespan.isExcludeWeekend())
			days+=days/5*2;

		while(true)
		{
			setRange(start,start+days*24*3600);
			calcuPeriods_internal();
			if(m_totalunits>=SpanUnits)
				break;
			days+=2;
		}

		setRange(start,m_Spans.getTimeByOffset(SpanUnits));
	}
	else
	{
		setRange(start,start+SpanUnits);
	}
}

//根据起始时间和窗口宽度，计算时间段和总秒数/单元数
void CTimeLineWin::Stru_Data::calcuPeriods_internal(void)
{
    m_Spans.clear();
    m_WeekendDaysInclued=0;
    m_totalunits=0;

	m_CacheTimespanOffset=-1;
	m_CacheTimespanStart=-1;
	m_CacheTimespanEnd=-1;

	if(m_bTimeBased)
	{
		if(!m_WholePeriod.isempty())
		{
			m_Spans=m_WorkingTimespan.getTradingTimespan_InPeriod(m_WholePeriod.getFirstTimespan().start,m_WholePeriod.getFirstTimespan().end,m_WeekendDaysInclued);
			m_totalunits=(int)m_Spans.getTimeDuration();
		}
	}
	else
	{
		m_Spans=m_WholePeriod;
		m_totalunits=(int)m_Spans.getTimeDuration();
	}
}

//计算刻度线位置
void CTimeLineWin::Stru_Data::calcuScalePos_internal(void)
{
    m_MarkUnit=0;
	m_BreakScaleUTC.clear();
	m_BreakScalePos.clear();
    m_LongScaleUTC.clear();
    m_LongScalePos.clear();
    m_ShortScaleUTC.clear();
    m_ShortScalePos.clear();

	if(!m_bTimeBased) 
	{
		//非时间刻度窗口，不画时间刻度
		return;
	}

	if(m_Spans.isempty()) 
        return;

    short syear,smon,sday,shour,smin,ssec;
	int starttime,endtime,tmputc;
	int SecondsIntraday,SecondsIntraday2;						//日内秒数。从零时到该时间的秒数
	if(m_WorkingTimespan.isLocalTzDST())
	{
		CEasyTime tt(m_Spans.getFirstTimespan().start,0);
		starttime=(int)tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();		
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		CEasyTime tt2(m_Spans.getLastTimespan().end,0);
		endtime=(int)tt2.getutc();
		SecondsIntraday2=tt2.getSecondsIntraday();
	}
	else
	{
		CEasyTime2 tt(m_Spans.getFirstTimespan().start,0,m_WorkingTimespan.getTimezoneDst());
		starttime=(int)tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		CEasyTime2 tt2(m_Spans.getLastTimespan().end,0,m_WorkingTimespan.getTimezoneDst());
		endtime=(int)tt2.getutc();
		SecondsIntraday2=tt2.getSecondsIntraday();
	}

    //计算时间跨度(日)
	int SpanDays=(int)(endtime-SecondsIntraday2-(starttime-SecondsIntraday))/(24*3600) + (SecondsIntraday2>0 ? 1 : 0) - m_WeekendDaysInclued;
    if(SpanDays<0) SpanDays=0;
    
	//计算时间跨度(小时)
	int SpanHours=m_totalunits/3600+(m_totalunits%3600!=0?1:0);

    const map<int,Stru_TimespanInfo<int>>& mapTimeSpans=m_Spans.getTimespans();
    map<int,Stru_TimespanInfo<int>>::const_iterator it_TimeSpans;

	//所有时间段的开始都画长刻度
    for(it_TimeSpans=mapTimeSpans.begin();it_TimeSpans!=mapTimeSpans.end();it_TimeSpans++)
		m_BreakScaleUTC.insert(it_TimeSpans->first);
	//结束时间也画长刻度
	if((int)mapTimeSpans.size()>0)
		m_BreakScaleUTC.insert(mapTimeSpans.rbegin()->second.end);

    //计算刻度值
    if(SpanDays>=365)
    {
        //按年长刻度，按月短刻度
		m_MarkUnit=0;
		for(int year=syear;;year++)
        {
			if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,1,1).gettimeb().time;
			else tmputc=(int)CEasyTime2(year,1,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
			if(tmputc>=endtime) break;

			if(m_Spans.isInTimespans(tmputc)) m_LongScaleUTC.insert(tmputc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int mon=1;mon<=12;mon++)
            {
				if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,mon,1).gettimeb().time;
				else tmputc=(int)CEasyTime2(year,mon,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
				if(tmputc>=endtime) break;
                if(m_Spans.isInTimespans(tmputc)) m_ShortScaleUTC.insert(tmputc);
            }
        }
    }
	else  if(SpanDays>=30)
	{
		//按月长刻度；按日短刻度
		m_MarkUnit=1;
        int year=syear;
        int mon=smon;
		for(int year=syear,mon=smon;;year=mon==12?year+1:year,mon=mon==12?1:mon+1)
        {
			if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,mon,1).gettimeb().time;
			else tmputc=(int)CEasyTime2(year,mon,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
			if(tmputc>=endtime) break;

            if(m_Spans.isInTimespans(tmputc)) m_LongScaleUTC.insert(tmputc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			int day=(year==syear&&mon==smon?sday:1);
			if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,mon,day).gettimeb().time;
			else tmputc=(int)CEasyTime2(year,mon,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
			for(;day<=31&&tmputc<endtime;day++,tmputc+=24*3600)
                if(m_Spans.isInTimespans(tmputc)) m_ShortScaleUTC.insert(tmputc);
        }
	}
	else if(SpanDays>3)
	{
		//按日长刻度，按小时短刻度
		m_MarkUnit=2;
		for(int dayutc=(int)(starttime-SecondsIntraday);dayutc<endtime;dayutc+=24*3600)
        {
            if(m_Spans.isInTimespans(dayutc)) m_LongScaleUTC.insert(dayutc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(dayutc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int hhutc=dayutc;hhutc<dayutc+24*3600;hhutc+=3600)
				if(m_Spans.isInTimespans(hhutc)) m_ShortScaleUTC.insert(hhutc);
        }
	}
    else if(m_totalunits>=3600)
    {
		//按小时长刻度，按5分钟短刻度
		m_MarkUnit=3;
		for(int hhutc=(int)(starttime-SecondsIntraday+shour*3600);hhutc<endtime;hhutc+=3600)
		{
            if(m_Spans.isInTimespans(hhutc)) m_LongScaleUTC.insert(hhutc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(hhutc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int mmutc=hhutc;mmutc<hhutc+3600;mmutc+=300)
				if(m_Spans.isInTimespans(mmutc)) m_ShortScaleUTC.insert(mmutc);
		}
    }
	else
	{
		//按5分钟长刻度，按1分钟短刻度
		m_MarkUnit=4;
		for(int mm5utc=(int)(starttime-SecondsIntraday+shour*3600+(smin-smin%5)*60);mm5utc<endtime;mm5utc+=300)
		{
            if(m_Spans.isInTimespans(mm5utc)) m_LongScaleUTC.insert(mm5utc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(mm5utc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int mm1utc=mm5utc;mm1utc<mm5utc+300;mm1utc+=60)
				if(m_Spans.isInTimespans(mm1utc)) m_ShortScaleUTC.insert(mm1utc);
		}
	}

	//计算刻度位置
    set<time_t>::const_iterator it;
    for(it=m_BreakScaleUTC.begin();it!=m_BreakScaleUTC.end();it++)
        m_BreakScalePos.insert(getEntireUnitOffbyX((int)(*it)));
    for(it=m_LongScaleUTC.begin();it!=m_LongScaleUTC.end();it++)
        m_LongScalePos.insert(getEntireUnitOffbyX((int)(*it)));
    for(it=m_ShortScaleUTC.begin();it!=m_ShortScaleUTC.end();it++)
        m_ShortScalePos.insert(getEntireUnitOffbyX((int)(*it)));
}

//X是否已经超时/超限
bool CTimeLineWin::Stru_Data::isovertime(int iX) const
{
    return iX<m_Spans.getFirstTimespan().start||iX>=m_Spans.getLastTimespan().end ? true : false;
}
//根据秒数/单元数偏移量取X
int CTimeLineWin::Stru_Data::getXbyEntireUnitOff(int UnitOff) const
{
    if(UnitOff<0) return m_Spans.getFirstTimespan().start;
    if(UnitOff>=m_totalunits) return m_Spans.getLastTimespan().end;
    return m_Spans.getTimeByOffset(UnitOff);
}
//根据X取秒数/单元数偏移量, 非法则返回-1
int CTimeLineWin::Stru_Data::getEntireUnitOffbyX(int iX) const
{
	if(m_totalunits==0) return 0;

	//为快速查找，使用缓冲
	if(m_CacheTimespanOffset!=-1&&m_CacheTimespanStart!=-1&&m_CacheTimespanEnd!=-1&&
		iX>=m_CacheTimespanStart&&iX<m_CacheTimespanEnd)
		return (int)(iX-m_CacheTimespanStart+m_CacheTimespanOffset);

    return m_Spans.getOffsetByTime2(iX,
		*(const_cast<int*>(&m_CacheTimespanOffset)),
		*(const_cast<int*>(&m_CacheTimespanStart)),
		*(const_cast<int*>(&m_CacheTimespanEnd)));
}

//取每天合法的秒数
int CTimeLineWin::Stru_Data::getSeconds_EachDay(void) const
{
	return m_totalunits&&m_bTimeBased ? m_WorkingTimespan.getSeconds_EachDay() : 0;
}









CTimeLineWin::CTimeLineWin(bool bTimeBased)
:	m_MaxUnitPerWnd(0),
	m_DispWidPerWnd(0),	
	m_ZoomFactor(0),
	m_EntireUnitOff4Disp(0),
	m_EntireUnitCnt4Disp(0),
	m_TimeTipWnd(0),
	m_TimeTipPos(-1),
	m_IntegralPointUnitSpace(-1),
	m_MemBmp(0)
{
	m_Data.m_bTimeBased=bTimeBased;
}


CTimeLineWin::~CTimeLineWin()
{
	if(m_MemBmp)
	{
		DeleteObject((HBITMAP)m_MemBmp);
		m_MemBmp=0;
	}
}

//设置时间轴窗口句柄。如果用winform创建时间轴窗口，创建后，用这个方法设置hWnd句柄
void CTimeLineWin::AddPartWnd(int hWnd,bool brefresh)
{
	if(!hWnd) return;
	m_hWnds.push_back(hWnd);
	if(brefresh) refresh(false,true,true,true);
}
//从窗口集中删除一个窗口
void CTimeLineWin::RemovePartWnd(int hWnd,bool brefresh)
{
	if(!hWnd) return;
	int wndord=getWndOrd(hWnd);
	if(wndord==-1) return;
	m_hWnds.erase(m_hWnds.begin()+wndord);
	if(brefresh) refresh(false,true,true,true);
}

//刷新数据以及显示参数。
//brecalcu:是否需要重新计算。设置时间区域、交易时段后需要重新计算。时间数据变化时，应该为true
//bsetfullwnd:是否需要设置修改缩放因子使之全屏显示。时间数据变化时，窗口数量变化时，应该为true
//bgetwndwid:是否需要重新获取分窗口宽度。窗口数量变化时，以及窗口尺寸变化时应该为true
//brepaint:是否刷新全部分窗口
void CTimeLineWin::refresh(bool brecalcu,bool bsetfullwnd,bool bgetwndwid,bool brepaint) 
{ 
	//重新计算数据
	if(brecalcu)
		m_Data.recalcu();

	//计算各个分窗口宽度和总宽度
	if(bgetwndwid)
	{
		m_WidPerWnd = m_hWnds.size()>0 ? CTools_Win32::GetClientWid((HWND)m_hWnds[0]) : 0;
	}

	//需要重置缩放因子，使完整时间轴能全屏显示
	if(bsetfullwnd&&(int)m_hWnds.size()>0)
	{
		m_ZoomFactor=0;
		if((m_WidPerWnd-m_Cfg.LeftSpaceWid)*(int)m_hWnds.size()<=m_Data.m_totalunits)
		{
			for( ; m_ZoomFactor>=-16&&getUnitCntByDispWid(m_WidPerWnd-m_Cfg.LeftSpaceWid)*(int)m_hWnds.size()<m_Data.m_totalunits ; m_ZoomFactor--);
		}
		else
		{
			for( ; m_ZoomFactor<16&&getUnitCntByDispWid(m_WidPerWnd-m_Cfg.LeftSpaceWid)*(int)m_hWnds.size()>m_Data.m_totalunits ; m_ZoomFactor++);
			if(m_ZoomFactor<16&&m_ZoomFactor>0) 
			{
				//过头了，缩小一个因子
				m_ZoomFactor--;
			}
		}

		m_EntireUnitOff4Disp=0;
		m_EntireUnitCnt4Disp=m_Data.m_totalunits;
	}

	m_MaxUnitPerWnd=(int)m_hWnds.size()>0?getUnitCntByDispWid(m_WidPerWnd-m_Cfg.LeftSpaceWid):0;
	m_DispWidPerWnd=(int)m_hWnds.size()>0?getDispWidByUnitCnt(m_MaxUnitPerWnd):0;

	//规整时间轴的显示范围
	RegularOff4Disp_Internal(); 

	//刷新全部分窗口的显示
	if(brepaint)
		repaint(NULL);
}

//规范化m_Off4Disp和m_Wid4Disp
void CTimeLineWin::RegularOff4Disp_Internal(void)
{
	int DispWid=m_DispWidPerWnd*m_hWnds.size();
	int MaxDispUnits = getUnitCntByDispWid(DispWid);

	//先调整m_EntireUnitCnt4Disp，尽量的充满窗口
	m_EntireUnitCnt4Disp=MaxDispUnits;
	if(m_EntireUnitCnt4Disp>m_Data.m_totalunits) m_EntireUnitCnt4Disp=m_Data.m_totalunits;

	//看m_EntireUnitOff4Disp是否越界
	if(m_EntireUnitOff4Disp>m_Data.m_totalunits) m_EntireUnitOff4Disp=m_Data.m_totalunits;
	if(m_EntireUnitOff4Disp<0) m_EntireUnitOff4Disp=0;

	//判断m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp是否越界
	if(m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp>m_Data.m_totalunits) m_EntireUnitOff4Disp=m_Data.m_totalunits-m_EntireUnitCnt4Disp;


	//显示的开始位置和结束位置的UTC
	int StartX4Disp=m_Data.getXbyEntireUnitOff(m_EntireUnitOff4Disp);					
	int EndX4Disp=m_Data.getXbyEntireUnitOff(m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp);			


	//计算显示范围内，整点线的位置。间隔大于32个像素时有效。整点线可能是小时、30分、15分、5分、1分、30秒、15秒、5秒、1秒线
	m_IntegralPointPos.clear();
	m_IntegralPointX.clear();
	m_IntegralPointUnitSpace=-1;
	const int pixels=32;														//整点线最小间距

	if(m_Data.m_bTimeBased)
	{
		int HourSpace=getDispWidByUnitCnt(3600);								//3600秒的显示宽度(点距)
		if(HourSpace/3600>=pixels) m_IntegralPointUnitSpace=1;					//每秒画一根整点线
		else if(HourSpace/(3600/   5)>=pixels) m_IntegralPointUnitSpace=5;		//每5秒画一根整点线
		else if(HourSpace/(3600/  15)>=pixels) m_IntegralPointUnitSpace=15;		//每15秒画一根整点线
		else if(HourSpace/(3600/  30)>=pixels) m_IntegralPointUnitSpace=30;		//每30秒画一根整点线
		else if(HourSpace/(3600/  60)>=pixels) m_IntegralPointUnitSpace=60;		//每分钟画一根整点线
		else if(HourSpace/(3600/ 300)>=pixels) m_IntegralPointUnitSpace=300;	//每5分钟画一根整点线
		else if(HourSpace/(3600/ 900)>=pixels) m_IntegralPointUnitSpace=900;	//每15分钟画一根整点线
		else if(HourSpace/(3600/1800)>=pixels) m_IntegralPointUnitSpace=1800;	//每30分钟画一根整点线
		else if(HourSpace/(3600/3600)>=pixels) m_IntegralPointUnitSpace=3600;	//每小时画一根整点线
		else m_IntegralPointUnitSpace=-1;
	}
	else
	{
		for(m_IntegralPointUnitSpace=1;getDispWidByUnitCnt(m_IntegralPointUnitSpace)<pixels&&m_IntegralPointUnitSpace<100000000;m_IntegralPointUnitSpace*=5);
		if(m_IntegralPointUnitSpace>=100000000) m_IntegralPointUnitSpace=-1;
	}

	if(m_IntegralPointUnitSpace!=-1)
	{
		int iX = StartX4Disp%m_IntegralPointUnitSpace==0 ? StartX4Disp : StartX4Disp-StartX4Disp%m_IntegralPointUnitSpace+m_IntegralPointUnitSpace;
		for(;iX<EndX4Disp;iX+=m_IntegralPointUnitSpace)
		{
			int unitoff=m_Data.getEntireUnitOffbyX(iX);  
			if(unitoff>=m_EntireUnitOff4Disp)
			{
				m_IntegralPointPos.push_back(getDispWidByUnitCnt(unitoff-m_EntireUnitOff4Disp));
				m_IntegralPointX.push_back(iX);
			}
		}
	}
}

//刷新窗口显示
//hWnd为NULL刷新全部分窗口显示，否则刷新指定分窗口显示
void CTimeLineWin::repaint(int hWnd)
{
	if(!hWnd)
	{
		for(int i=0;i<(int)m_hWnds.size();i++)
		{
			::InvalidateRect((HWND)m_hWnds[i],NULL,FALSE);
			::SendMessage((HWND)m_hWnds[i],WM_PAINT,0,0);
		}
	}
	else
	{
		::InvalidateRect((HWND)hWnd,NULL,FALSE);
		::SendMessage((HWND)hWnd,WM_PAINT,0,0);
	}
}

void CTimeLineWin::SetTimeTip(int hWnd,int pox)
{
	int oldtipwnd=m_TimeTipWnd;

	m_TimeTipWnd=hWnd;
	m_TimeTipPos=pox;

	if(oldtipwnd!=NULL)
		repaint(oldtipwnd);
	if(hWnd!=NULL&&hWnd!=oldtipwnd)
		repaint(hWnd);
}

void CTimeLineWin::ClearTimeTip(void)
{
	int oldtipwnd=m_TimeTipWnd;
	m_TimeTipWnd=NULL;
	m_TimeTipPos=-1;
	if(oldtipwnd!=NULL)
		repaint(oldtipwnd);
}
//获取鼠标所在时间tip的当前位置
void CTimeLineWin::getTimeTipPos(int& hWnd,int& tippos)
{
	hWnd=m_TimeTipWnd;
	tippos=m_TimeTipPos;
}

//时间轴放大一倍。(本质上是m_ZoomFactor加一，以使显示内容变大)。注意：内部已经refresh了
bool CTimeLineWin::zoomin(void) 
{ 
	if(m_ZoomFactor>=16) return false;
	if(m_MaxUnitPerWnd<4) return false;
	m_ZoomFactor++;
	m_EntireUnitOff4Disp+=m_EntireUnitCnt4Disp/4;
	m_EntireUnitCnt4Disp/=2;
	refresh(false,false,false,true); 
	return true;
}
//时间轴窗口缩小一倍。(本质上是m_ZoomFactor减一，以使显示内容变小)。注意：内部已经refresh了
bool CTimeLineWin::zoomout(void) 
{ 
	if(m_ZoomFactor<=-16) return false;
	if(getUnitCntByDispWid(m_DispWidPerWnd*m_hWnds.size())>m_Data.m_totalunits) return false;

	m_ZoomFactor--;
	m_EntireUnitOff4Disp-=m_EntireUnitCnt4Disp/2;
	m_EntireUnitCnt4Disp*=2;
	refresh(false,false,false,true); 
	return true;
}
//时间轴左移一定偏移量。(本质上是按左箭头键，m_EntireUnitOff4Disp左移，以使显示左边的内容)。注意：内部已经refresh了
bool CTimeLineWin::movebyleftkey(int offset)
{
	if(m_EntireUnitOff4Disp>0)
	{
		int unitcnt=getUnitCntByDispWid(offset);
		if(unitcnt<=0) unitcnt=1;
		m_EntireUnitOff4Disp-=unitcnt;
		refresh(false,false,false,true); 
		return true;
	}
	else return false;
}
//时间轴右移一定偏移量。(本质上是按右箭头键，m_EntireUnitOff4Disp右移，以使显示右边的内容)。注意：内部已经refresh了
bool CTimeLineWin::movebyrightkey(int offset)
{
	if(m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp<m_Data.m_totalunits)
	{
		int unitcnt=getUnitCntByDispWid(offset);
		if(unitcnt<=0) unitcnt=1;
		m_EntireUnitOff4Disp+=unitcnt;
		refresh(false,false,false,true); 
		return true;
	}
	else return false;
}



//根据x轴位置取相应时间。xPos包含LeftSpaceWid
bool CTimeLineWin::getX(int hWnd,int xPos,int& rltX) const 
{ 
	if(xPos<m_Cfg.LeftSpaceWid||xPos>m_Cfg.LeftSpaceWid+m_DispWidPerWnd) return false;
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return false;

	int unitoff=getUnitCntByDispWid(WndOrd*m_DispWidPerWnd+xPos-m_Cfg.LeftSpaceWid);
	if(unitoff>=m_EntireUnitCnt4Disp) return false;
	rltX=(int)m_Data.getXbyEntireUnitOff(unitoff+m_EntireUnitOff4Disp); 
	return true;
}
//根据时间取x轴位置。返回的rltPos包含LeftSpaceWid。如果utc不在有效时间段内，返回false
bool CTimeLineWin::getxPos(int iX,int& hWnd,int& rltPos) const 
{ 
	int unitoff=m_Data.getEntireUnitOffbyX(iX);  
	if(unitoff<0) return false; 
	int totalpos=getDispWidByUnitCnt(unitoff-m_EntireUnitOff4Disp);
	int wndord=totalpos/m_DispWidPerWnd;
	if(wndord>=(int)m_hWnds.size()) return false;
	hWnd=m_hWnds[wndord];
	rltPos=totalpos%m_DispWidPerWnd+m_Cfg.LeftSpaceWid;
	return true;
}
//根据时间在指定窗口内取x轴位置。返回的rltPos包含LeftSpaceWid。如果utc不在窗口有效时间段内，返回false
bool CTimeLineWin::getxPosInWnd(int iX,int WndOrd,int& rltPos) const
{
	if(WndOrd<0||WndOrd>=(int)m_hWnds.size()) return false;
	int unitoff=m_Data.getEntireUnitOffbyX(iX);  
	if(unitoff<0) return false; 
	int totalpos=getDispWidByUnitCnt(unitoff-m_EntireUnitOff4Disp);
	if(WndOrd!=totalpos/m_DispWidPerWnd) return false;
	rltPos=totalpos%m_DispWidPerWnd+m_Cfg.LeftSpaceWid;
	return true;
}


//获取指定分窗口的参数。SecOff是窗口在m_DispWids上的秒数偏移,-1为无效，SecCnt是本窗口要显示的秒数
bool CTimeLineWin::GetDispOff_SecOff_SecCnt(int wndord,int& WndWid,int& DispOff,int& UnitOff,int& UnitCnt)
{
	if(wndord<0||wndord>=(int)m_hWnds.size()) return false;
	DispOff=wndord*m_DispWidPerWnd;
	WndWid=m_WidPerWnd;
	UnitOff=wndord*m_MaxUnitPerWnd;
	if(UnitOff>=m_EntireUnitCnt4Disp) { UnitOff=-1;  UnitCnt=0; }
	else { UnitCnt=m_EntireUnitCnt4Disp-UnitOff;  if(UnitCnt>m_MaxUnitPerWnd) UnitCnt=m_MaxUnitPerWnd; }
	return true;
}


//针对指定分窗口，在MemDC上Paint
void CTimeLineWin::PaintOnMemDC_Internal(int hWnd,int MemDC)
{
	//WndOrd是窗口的序号
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return;

	//WndWid为此窗口的宽度
	int DispOff=getDispOff2(WndOrd);
	int WndWid=m_WidPerWnd;

	//时间轴窗口的背景刷、画笔和字体
    HBRUSH	hBrBk=CreateSolidBrush(m_Cfg.BkCol);
	RECT Rect={0,0,WndWid,m_Cfg.WndHei};
    FillRect((HDC)MemDC,&Rect,hBrBk);
	DeleteObject(hBrBk);

    HPEN	hPen=CreatePen(PS_SOLID,1,m_Cfg.LineCol);
    HPEN	hOldPen=(HPEN)SelectObject((HDC)MemDC,hPen);

	HFONT   hFont=CreateFont(-1*m_Cfg.FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,m_Cfg.FontName);
    HFONT	hOldFont=(HFONT)SelectObject((HDC)MemDC,hFont);

    SetTextColor((HDC)MemDC,m_Cfg.FontCol);
    SetBkMode((HDC)MemDC,TRANSPARENT);

    //画顶层线
	int pox=m_Cfg.LeftSpaceWid>0?m_Cfg.LeftSpaceWid-1:0;
	MoveToEx((HDC)MemDC,pox,0,NULL);
    LineTo((HDC)MemDC,WndWid,0);

    set<int>::const_iterator it_pos,it_pos2,it_postmp;

    //根据每个时间段的开始，画短刻度线
	it_pos=m_Data.m_BreakScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_BreakScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
    for(;it_pos!=m_Data.m_BreakScalePos.end();it_pos++)
    {
		pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);
		if(pox>=DispOff&&pox<=DispOff+m_DispWidPerWnd)
		{
			pox=pox-DispOff+m_Cfg.LeftSpaceWid;
			if(pox!=m_Cfg.LeftSpaceWid)
			{
				MoveToEx((HDC)MemDC,pox,m_Cfg.TimespanStartScaleHei,NULL);
				LineTo((HDC)MemDC,pox,0);
			}
		}
		if(it_pos==it_pos2) break;
    }

    //画长刻度线
	it_pos=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
    for(;it_pos!=it_pos2;it_pos++)
    {
		int pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);
		if(pox>=DispOff&&pox<=DispOff+m_DispWidPerWnd)
		{
			pox=pox-DispOff+m_Cfg.LeftSpaceWid;
			if(pox!=m_Cfg.LeftSpaceWid)
			{
				MoveToEx((HDC)MemDC,pox,m_Cfg.LongScaleHei,NULL);
				LineTo((HDC)MemDC,pox,0);
			}
		}
    }

    //画短刻度线
	int lastpox=-1;		//上一次的刻度线，短刻度线之间要间隔8以上，否则跳过不画
	it_pos=m_Data.m_ShortScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_ShortScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
    for(;it_pos!=it_pos2;it_pos++)
    {
		int pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);
		if(pox>=DispOff&&pox<=DispOff+m_DispWidPerWnd)
		{
			pox=pox-DispOff+m_Cfg.LeftSpaceWid;
			if(pox!=m_Cfg.LeftSpaceWid&&(lastpox==-1||pox-lastpox>=8))
			{
				MoveToEx((HDC)MemDC,pox,m_Cfg.ShortScaleHei,NULL);
				LineTo((HDC)MemDC,pox,0);
				lastpox=pox;
			}
		}
    }

    //标注长刻度
	vector<int> scaletxtpox;
	vector<int> scaletxtwid;
    lastpox=-1;
	it_pos=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
	set<time_t>::const_iterator it_utc=m_Data.m_LongScaleUTC.begin();
	for(it_postmp=m_Data.m_LongScalePos.begin();it_postmp!=it_pos&&it_utc!=m_Data.m_LongScaleUTC.end();it_postmp++,it_utc++);
	//此时it_pos和it_utc是对应的迭代器
	for(;it_pos!=it_pos2&&it_utc!=m_Data.m_LongScaleUTC.end();it_pos++,it_utc++)
    {
		int pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);

		//避免越界情况
		if(pox<DispOff||pox>=DispOff+m_DispWidPerWnd) continue;

		pox=pox-DispOff+m_Cfg.LeftSpaceWid;

		//避免和上一个刻度标注重合
		if(lastpox!=-1&&pox<lastpox+8) continue;

		char buf[256]={0,};
		memset(buf,0,sizeof(buf));
        short year,month,day,hour,min,sec,wday;
		if(m_Data.m_WorkingTimespan.isLocalTzDST()) CEasyTime(*it_utc).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(*it_utc,0,m_Data.m_WorkingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		switch(m_Data.m_MarkUnit)
		{
			case 0:						//按年长刻度，按月短刻度
			case 1:						//按月长刻度；按日短刻度
			case 2:						//按日长刻度，按小时短刻度
			{
				if(m_Data.m_MarkUnit==0) sprintf(buf,"%04d/",year);
				sprintf(buf+strlen(buf),"%02d/%02d/",month,day);
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
				break;
			}
			case 3:						//按小时长刻度，按5分钟短刻度
			{
				sprintf(buf,"%02d:%02d",hour,min);
				break;
			}
			case 4:						//按5分钟长刻度，按1分钟短刻度
			{
				sprintf(buf,"%02d",min);
				break;
			}
		}

        SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
        int ipox;
        if(m_Cfg.MarkPos==1) ipox=pox+2;		//刻度写在右边
        else ipox=pox-Sz.cx/2;					//刻度写在中间
		if(ipox<0&&ipox+Sz.cx>0) ipox=0;

		//再次避免和上一个刻度标注重合
		if(lastpox!=-1&&ipox<lastpox+8) continue;

		TextOut((HDC)MemDC,ipox,m_Cfg.WndHei-Sz.cy,buf,strlen(buf));
        lastpox=ipox+Sz.cx;
		scaletxtpox.push_back(ipox);
		scaletxtwid.push_back(Sz.cx);
    }

	//标注整点线的时间，要避开长刻度标注的范围
    SetTextColor((HDC)MemDC,m_Cfg.FontCol/2);
	lastpox=-1;
	for(int i=0,ilongscale=0;i<(int)m_IntegralPointPos.size();i++)
	{
		int pox=m_IntegralPointPos[i];

		//避免越界情况
		if(pox<DispOff) continue;
		if(pox>=DispOff+m_DispWidPerWnd) break;

		pox=pox-DispOff+m_Cfg.LeftSpaceWid;

		char buf[256];
		short hour,min,sec;
		if(!m_Data.m_bTimeBased)
		{
			sprintf(buf,"%d",m_IntegralPointX[i]);
		}
		else
		{
			CEasyTime(m_IntegralPointX[i]).convert2fileds(NULL,NULL,NULL,&hour,&min,&sec);
			if(m_IntegralPointUnitSpace>=3600) 
			{
				sprintf(buf,"%d",hour);
			}
			else if(m_IntegralPointUnitSpace>=60) 
			{
				if(min==0) sprintf(buf,"%02d:%02d",hour,min);
				else sprintf(buf,"%d'",min);
			}
			else 
			{
				if(sec==0) sprintf(buf,"%02d:%02d",hour,min);
				else sprintf(buf,"%d\"",sec);
			}
		}

		SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
		int ipox=pox-Sz.cx/2;

		if(lastpox!=-1&&ipox<=lastpox+4) 
			continue;

		int boverlap=false;		//是否重叠
		while(ilongscale<(int)scaletxtpox.size())
		{
			//需要判断是否和长刻度标注重叠
			int scaletxtleft=scaletxtpox[ilongscale];
			int scaletxtright=scaletxtpox[ilongscale]+scaletxtwid[ilongscale];
			if(scaletxtleft<=ipox)
			{
				if(scaletxtright+8<=ipox)
				{
					//检查下一个长刻度标注
					ilongscale++;
					continue;
				}
				else
				{
					//有重叠
					boverlap=true;
					break;
				}
			}
			else
			{
				if(scaletxtleft>=ipox+Sz.cx+8)
				{
					//没有重叠，可以退出了
					break;
				}
				else
				{
					//有重叠
					boverlap=true;
					break;
				}
			}
		}

		if(!boverlap)
		{
			TextOut((HDC)MemDC,ipox,m_Cfg.WndHei-Sz.cy,buf,strlen(buf));
			lastpox=ipox+Sz.cx;
		}
	}
    SetTextColor((HDC)MemDC,m_Cfg.FontCol);

	//标注TimeTip
	if(m_TimeTipWnd==hWnd&&
		m_TimeTipPos>=m_Cfg.LeftSpaceWid&&m_TimeTipPos<m_DispWidPerWnd+m_Cfg.LeftSpaceWid)
	{
        char buf[256];
		int TimeTipX;
		if(getX(hWnd,m_TimeTipPos,TimeTipX))
		{
			HPEN TipLinePen=CreatePen(PS_SOLID,1,m_Cfg.TipLineCol);
			HPEN hOldPen=(HPEN)SelectObject((HDC)MemDC,TipLinePen);
			MoveToEx((HDC)MemDC,m_TimeTipPos,1,NULL);
			LineTo((HDC)MemDC,m_TimeTipPos,m_Cfg.WndHei);
			SelectObject((HDC)MemDC,hOldPen);
			DeleteObject(TipLinePen);

			memset(buf,0,sizeof(buf));
			if(m_Data.m_bTimeBased)
			{
				short year,month,day,hour,min,sec,wday;

				if(m_Data.m_WorkingTimespan.isLocalTzDST()) CEasyTime((time_t)TimeTipX).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
				else CEasyTime2((time_t)TimeTipX,0,m_Data.m_WorkingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);

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
				sprintf(buf+strlen(buf),"%d:%d:%d",hour,min,sec);
			}
			else sprintf(buf,"%d",TimeTipX);

			SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
			int txtpox=m_TimeTipPos-Sz.cx/2;
			if(txtpox+Sz.cx>WndWid) txtpox=WndWid-Sz.cx;
			if(txtpox<0) txtpox=0;
			SetTextColor((HDC)MemDC,RGB(255,255,255));
			SetBkColor((HDC)MemDC,RGB(0,0,255));
			SetBkMode((HDC)MemDC,OPAQUE);
			TextOut((HDC)MemDC,txtpox,m_Cfg.WndHei-1-Sz.cy,buf,strlen(buf));
		}
	}


	if(hOldPen) SelectObject((HDC)MemDC,hOldPen);
	if(hOldFont) SelectObject((HDC)MemDC,hOldFont);

	if(hPen) DeleteObject(hPen);
	if(hFont) DeleteObject(hFont);
}

void CTimeLineWin::OnPaint(int hWnd,int iDC) 
{
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return;
	int WndWid=m_WidPerWnd;

	HBITMAP hMemBmp=CreateCompatibleBitmap((HDC)iDC,WndWid,m_Cfg.WndHei);
	if(!hMemBmp) return;

	HDC		MemDC=CreateCompatibleDC((HDC)iDC);
	HBITMAP hOldBmp=(HBITMAP)SelectObject((HDC)MemDC,hMemBmp);

	PaintOnMemDC_Internal(hWnd,(int)MemDC);
	BitBlt((HDC)iDC,0,0,WndWid,m_Cfg.WndHei,MemDC,0,0,SRCCOPY);

	if(hOldBmp) SelectObject((HDC)MemDC,hOldBmp);
	if(hMemBmp) DeleteObject(hMemBmp);
	DeleteDC(MemDC);
}

