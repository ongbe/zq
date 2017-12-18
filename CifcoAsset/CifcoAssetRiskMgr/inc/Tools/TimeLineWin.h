// TimeLineWin.h: interface for the CTimeLineWin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
#define AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <time.h>
#include <map>
#include <set>
#include <vector>
using namespace std;

#include "Tools_Ansi.h"
#include "easytime.hpp"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"


struct Stru_TimeLineWin_Cfg;
class CPreTimeLineWin;


#define ClsName_TimeLineWin "cls_TimeLineWin"


//定义时间刻度窗口的配置参数
struct Stru_TimeLineWin_Cfg
{
    Stru_TimeLineWin_Cfg(int markPos=0,
                            COLORREF bkCol=RGB(0,0,0),
                            COLORREF lineCol=RGB(255,0,0),
                            COLORREF fontCol=RGB(255,0,0),
                            int fontSize=12,
                            const char* fontName="新宋体",
                            int longScaleHei=6,
                            int shortScaleHei=4,
                            int timespanstartScaleHei=2,
							int wndHei=16);
    Stru_TimeLineWin_Cfg(const Stru_TimeLineWin_Cfg& r);
    ~Stru_TimeLineWin_Cfg();
    Stru_TimeLineWin_Cfg& operator=(const Stru_TimeLineWin_Cfg& r);

    void setMarkPos(int markPos) { MarkPos=markPos; }


    COLORREF    BkCol;              //背景颜色
    COLORREF    LineCol;            //线条颜色
    COLORREF    FontCol;            //字体颜色
    int         FontSize;           //字体大小，正数
    char        FontName[256];      //字体名称
    int         LongScaleHei;       //长刻度线的高度
    int         ShortScaleHei;      //短刻度线的高度
    int         TimespanStartScaleHei;//时段开始时间刻度的高度
	int			WndHei;			    //时间轴窗口高度

    int         MarkPos;            //刻度标注位置，0-数字标注在刻度中间；1-标注在右边


    HBRUSH      hBrBk;              //时间轴窗口的背景刷
    HPEN        hPen;               //时间轴窗口的画笔
    HFONT       hFont;              //时间轴窗口的字体句柄
};
//定义时间刻度的范围
struct Stru_TimeLineWin_Data
{
    Stru_TimeLineWin_Data(int wndWide=0,
                            const CEasyTimespan<time_t>& WholePeriod=CEasyTimespan<time_t>(),
                            const CEasyTradingTimespan& TradingTimespan=CEasyTradingTimespan()
                            );

    //重新设置时间刻度的范围
    void setRange(time_t start,time_t end);
    //重新设置时间刻度的范围，指定起始时间，和有效秒数
    void setRange2(time_t StartUTC,int SpanSeconds);
    //重新设置窗口宽度
    void setWndWide(int wndWide) { m_WndWide=wndWide; }
    //重置交易时间参数
    void setTradingTimespan(const CEasyTradingTimespan& TradingTimespan) { m_TradingTimespan=TradingTimespan; }
    //重新计算，重新设置后，需要调用这个函数进行重新计算
    void refresh(void) { calcuPeriods_internal(); calcuScalePos_internal(); }


	//utc是否已经超时
	bool isovertime(time_t utc) const;
    //根据x轴位置取相应时间
    time_t getUTC(int xPos) const;
    //根据时间取x轴位置
    int getxPos(time_t utc) const;
    //获取刻度的时间集
    void getScale(set<time_t>& longScale,set<time_t>& shortScale) const;


    //根据起始时间和窗口宽度，计算刻度的时间
    void calcuPeriods_internal(void);
    //计算刻度线位置
    void calcuScalePos_internal(void);


	//取每天合法的秒数
	int getSeconds_EachDay(void);

    int                             m_WndWide;              //窗口宽度

    CEasyTradingTimespan            m_TradingTimespan;      //交易时段定义

    CEasyTimespan<time_t>           m_WholePeriod;          //时间刻度起止时间
    CEasyTimespan<time_t>           m_TimeSpans;            //有效起止时间的集合
	
	int								m_WeekendDaysInclued;	//包含的周末的天数

	int								m_totalsecs;			//总秒数，指有效秒数，不是时间跨度

    int                             m_MarkUnit;             //标注单位，0:按hh标注，1:按yyyy-mm-dd标注

    set<time_t>                     BreakScaleUTC;          //时间中断点的时间集   
	set<int>                        BreakScalePos;          //时间中断点的位置集合 

    set<time_t>                     LongScaleUTC;           //长刻度的时间集   
    set<int>                        LongScalePos;           //长刻度的位置集合 

    set<time_t>                     ShortScaleUTC;          //短刻度的时间集
    set<int>                        ShortScalePos;          //短刻度的位置集合

private:
    //下两个函数禁止使用
    Stru_TimeLineWin_Data(const Stru_TimeLineWin_Data& r);
    Stru_TimeLineWin_Data& operator=(const Stru_TimeLineWin_Data& r);
};


//用于为CTimeLineWin静态注册窗口类
class CPreTimeLineWin
{
public:
    CPreTimeLineWin();
    ~CPreTimeLineWin();
    void Lock(void) { EnterCriticalSection(&m_CS); };
    void Unlock(void) { LeaveCriticalSection(&m_CS); };

    void AddHWND(HWND hWnd,void* pobj);
    void RemoveHWND(HWND hWnd);
    void* getObj_withoutLocking(HWND hWnd);

private:

    map<HWND,void*> m_TimeLineWins;
    CRITICAL_SECTION m_CS;
};

//时间刻度窗口类
class CTimeLineWin
{
public:
	CTimeLineWin(const Stru_TimeLineWin_Cfg& cfg,bool bCreate=false,HWND hParent=NULL);
	virtual ~CTimeLineWin();

	HWND CreateWnd(HWND hParent);               //在父窗口中创建时间刻度窗口

    //下两个函数可以用于在MFC创建的窗口中调用
    virtual void OnSize(HWND hWnd);
    virtual void OnPaint(HWND hWnd); 

    //窗口处理函数，用于win32 api创建的窗口。主要处理WM_SIZE和WM_PAINT
    static LRESULT WINAPI TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);


	void SetTimeTip(int pox);                   //显示鼠标所在位置对应时间tip，鼠标在父窗口客户区中移动时调用
	void ClearTimeTip(void);                    //清除鼠标所在位置对应时间tip，鼠标离开父窗口时调用

    //重新设置时间刻度的范围
    void setRange(time_t start,time_t end) { m_data.setRange(start,end); }
    //重新设置时间刻度的范围，指定起始时间，和有效秒数
    void setRange2(time_t StartUTC,int SpanSeconds) { m_data.setRange2(StartUTC,SpanSeconds); }
    //重新设置窗口宽度
    void setWndWide(int wndWide) { m_data.setWndWide(wndWide); }
    //重置交易时间参数
    void setTradingTimespan(const CEasyTradingTimespan& TradingTimespan) { m_data.setTradingTimespan(TradingTimespan); }
    //重新计算，重新设置后，需要调用这个函数进行重新计算
    void refresh(void) { m_data.refresh(); }

	//utc是否已经超时
    bool isovertime(time_t utc) { return m_data.isovertime(utc); }
    //根据x轴位置取相应时间
    time_t getUTC(int xPos) { return m_data.getUTC(xPos); }
    //根据时间取x轴位置
    int getxPos(time_t utc) { return m_data.getxPos(utc); }
    //获取刻度的时间集
    void getScale(set<time_t>& longScale,set<time_t>& shortScale) { return m_data.getScale(longScale,shortScale); }
    //获取时间轴窗口宽度
    int getWndWide(void) { return m_data.m_WndWide; }
	//取每天合法的秒数
    int getSeconds_EachDay(void) { return m_data.getSeconds_EachDay(); }


    //获取配置参数
    const Stru_TimeLineWin_Cfg& getCfg(void) { return m_cfg; }
    //获取配置参数
    const Stru_TimeLineWin_Data& getData(void) { return m_data; }
    //获取鼠标所在时间tip的当前位置
    int& getTimeTipPos(void) { return m_TimeTipPos; }

    HWND                    m_hWnd;             //时间刻度窗口句柄

private:
    Stru_TimeLineWin_Cfg    m_cfg;              //时间刻度窗口的配置参数
    Stru_TimeLineWin_Data   m_data;

	int m_TimeTipPos;                           //鼠标所在时间tip的当前位置。在此位置显示鼠标所在时间

public:
    static CPreTimeLineWin  m_PreOperation;     //记录窗口句柄和窗口对象的对应关系，在窗口回调函数中使用
};


#endif // !defined(AFX_TIMELINE_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
