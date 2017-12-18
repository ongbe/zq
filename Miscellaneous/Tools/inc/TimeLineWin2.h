// TimeLineWin.h: interface for the CTimeLineWin2 class.
//	为了与原来代码兼容。以后将废弃
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
#define AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include <time.h>
#include <map>
#include <set>


#include "Tools_Ansi.h"
#include "easytime.h"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"
#include "easymutex.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 


#define ClsName_TimeLineWin2 "cls_TimeLineWin2"


//定义时间刻度窗口的配置参数
struct Stru_TimeLineWin2_Cfg
{
    Stru_TimeLineWin2_Cfg(int markPos=0,
                            unsigned bkCol=0,
                            unsigned lineCol=0xff,
                            unsigned fontCol=0xff,
                            int fontSize=12,
                            const char* fontName="新宋体",
                            int longScaleHei=6,
                            int shortScaleHei=4,
                            int timespanstartScaleHei=2,
							int wndHei=16,
							int leftSpaceWid=0)
	{
		BkCol=bkCol;
		LineCol=lineCol;
		FontCol=fontCol;
		FontSize=fontSize;
		memset(FontName,0,sizeof(FontName));
		if(!fontName) strncpy(FontName,"宋体",sizeof(FontName)-1);
		else strncpy(FontName,fontName,sizeof(FontName)-1);
		LongScaleHei=longScaleHei;
		ShortScaleHei=shortScaleHei;
		TimespanStartScaleHei=timespanstartScaleHei;
		WndHei=wndHei;
        LeftSpaceWid=leftSpaceWid;
	}
    Stru_TimeLineWin2_Cfg(const Stru_TimeLineWin2_Cfg& r)
	{
		setCfg(r);
	}
    ~Stru_TimeLineWin2_Cfg()
	{
	}

	void setCfg(const Stru_TimeLineWin2_Cfg& r)
	{
		MarkPos=r.MarkPos;
		BkCol=r.BkCol;
		LineCol=r.LineCol;
		FontSize=r.FontSize;
		memcpy(FontName,r.FontName,sizeof(FontName));
		FontCol=r.FontCol;
		LongScaleHei=r.LongScaleHei;
		ShortScaleHei=r.ShortScaleHei;
		TimespanStartScaleHei=r.TimespanStartScaleHei;
		WndHei=r.WndHei;
        LeftSpaceWid=r.LeftSpaceWid;
	}
    const Stru_TimeLineWin2_Cfg& getCfg(void) { return *this; }


    unsigned	BkCol;					//背景颜色
    unsigned	LineCol;				//线条颜色
    unsigned	FontCol;				//字体颜色
    int         FontSize;				//字体大小，正数
    char        FontName[256];			//字体名称
    int         LongScaleHei;			//长刻度线的高度
    int         ShortScaleHei;			//短刻度线的高度
    int         TimespanStartScaleHei;	//时段开始时间刻度的高度
	int			WndHei;					//时间轴窗口高度
	int			LeftSpaceWid;			//时间轴左边空白区的宽度，单位为像素。这个区域画背景，不画时间轴

    int         MarkPos;				//刻度标注位置，0-数字标注在刻度中间；1-标注在右边
};


//定义时间刻度的范围
struct Stru_TimeLineWin2_Data
{
public:
    Stru_TimeLineWin2_Data(int TimeLineWid=0,
                            const CEasyTimespan<time_t>& WholePeriod=CEasyTimespan<time_t>(),
                            const CEasyTradingTimespan& TradingTimespan=CEasyTradingTimespan()
                            );

    //重新设置时间刻度的范围
    void setRange(time_t start,time_t end);
    //重新设置时间刻度的范围，指定起始时间，和有效秒数。非交易时段的时间不算有效秒数
    void setRange2(time_t StartUTC,int SpanSeconds);
    //重新设置窗口宽度
    void setTimeLineWid(int TimeLineWid) { m_TimeLineWid=TimeLineWid; }
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
	void getScale(std::set<time_t>& longScale,std::set<time_t>& shortScale) const;
    //获取窗口宽度
    int getTimeLineWid(void ) { return m_TimeLineWid; }

	//取每天合法的秒数
	int getSeconds_EachDay(void) const;

    //获取配置参数
    const Stru_TimeLineWin2_Data& getData(void) { return *this; }

private:
    //根据起始时间和窗口宽度，计算刻度的时间
    void calcuPeriods_internal(void);
    //计算刻度线位置
    void calcuScalePos_internal(void);


public:

    int                             m_TimeLineWid;          //窗口宽度

    CEasyTradingTimespan            m_TradingTimespan;      //交易时段定义

    CEasyTimespan<time_t>           m_WholePeriod;          //时间刻度起止时间
    CEasyTimespan<time_t>           m_TimeSpans;            //有效起止时间的集合
	
	int								m_WeekendDaysInclued;	//包含的周末的天数

	int								m_totalsecs;			//总秒数，指有效秒数，不是时间跨度

    int                             m_MarkUnit;             //标注单位，0:按hh标注，1:按yyyy-mm-dd标注

	std::set<time_t>                m_BreakScaleUTC;        //时间中断点的时间集   
	std::set<int>                   m_BreakScalePos;        //时间中断点的位置集合 

    std::set<time_t>                m_LongScaleUTC;         //长刻度的时间集   
    std::set<int>                   m_LongScalePos;         //长刻度的位置集合 

    std::set<time_t>                m_ShortScaleUTC;        //短刻度的时间集
    std::set<int>                   m_ShortScalePos;        //短刻度的位置集合

private:
    //下两个函数禁止使用
    Stru_TimeLineWin2_Data(const Stru_TimeLineWin2_Data& r);
    Stru_TimeLineWin2_Data& operator=(const Stru_TimeLineWin2_Data& r);
};


//时间刻度窗口类
class CTimeLineWin2 : public Stru_TimeLineWin2_Cfg,public Stru_TimeLineWin2_Data
{
public:
	CTimeLineWin2(const Stru_TimeLineWin2_Cfg& cfg=Stru_TimeLineWin2_Cfg());
	virtual ~CTimeLineWin2();

	//下面三个函数在win32编程中需要使用
	static void RegistWndClass(void);			//注册窗口类。创建窗口前必须先调用此函数注册窗口类
	static void UnregistWndClass(void);			//注销窗口类
	int CreateWnd(int hParent);					//在父窗口中创建时间刻度窗口

	//下面这个函数在winform编程中需要使用
	void SetWnd(int hWnd);						//设置时间轴窗口句柄。如果用winform创建时间轴窗口，创建后，用这个方法设置hWnd句柄

    //下两个函数可以用于在MFC/Winform创建的窗口中调用
    virtual void OnSize(int iWnd);
    virtual void OnPaint(int iWnd); 
    virtual void OnPaint2(int iWnd,int hDC); 

    //下面两个函数是Stru_TimeLineWin2_Data的函数，但需要考虑LeftSpaceWid的影响，因此重载一下
    //根据x轴位置取相应时间
    time_t getUTC(int xPos) const { return Stru_TimeLineWin2_Data::getUTC(xPos-LeftSpaceWid); }
    //根据时间取x轴位置
    int getxPos(time_t utc) const { return Stru_TimeLineWin2_Data::getxPos(utc)+LeftSpaceWid; }


	void SetTimeTip(int pox);                   //显示鼠标所在位置对应时间tip，鼠标在父窗口客户区中移动时调用
	void ClearTimeTip(void);                    //清除鼠标所在位置对应时间tip，鼠标离开父窗口时调用


    //获取鼠标所在时间tip的当前位置
    int& getTimeTipPos(void) { return m_TimeTipPos; }

    int								m_hWnd;					//时间刻度窗口句柄
	static Ceasymutex				m_mutex;				//同步锁
    static CTimeLineWin2* getObj_withoutLocking(int hWnd);	//根据窗口句柄，获取对应时间轴对象的指针

private:

	int								m_TimeTipPos;			//鼠标所在时间tip的当前位置。在此位置显示鼠标所在时间

private:

	static std::map<int,CTimeLineWin2*>	m_mapTimeLineWins;	//考虑到可能有多个时间轴窗口存在，这里保存每个时间轴窗口句柄和对象指针的map。在OnSize/OnPaint时要知道是针对哪个窗口
	void AddWnd(int hWnd);									//将一个时间轴窗口句柄添加到map中去
    void RemoveWnd(int hWnd);								//将一个时间轴句柄从map中删除

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TIMELINE_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
