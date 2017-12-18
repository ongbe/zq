// TimeLineWin.h: interface for the CTimeLineWin class.
//	时间轴处理类
//	1. 一般情况下，时间轴在一个窗口中显示。
//	2. 特殊情况下，需要显示很长的时间轴，可以在多个窗口里显示。在各个窗口里首尾相连。
//		这种情况下，一般各窗口宽度相同，上下摆放，以实现在一个屏幕上用多个窗口显示长时间轴
//		用AddPartWnd()和RemovePartWnd()可以增加和删除分窗口
//	3. 这个类并非线程安全
//	4. 可以画时间刻度，也可以画非时间刻度。用m_Cfg.bTimeBased
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
#include <vector>
using std::map;
using std::set;
using std::vector;


#include "Tools_Ansi.h"
#include "easytime.h"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"
#include "easymutex.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 





//时间刻度窗口类
class CTimeLineWin
{
public:
	//定义时间刻度窗口的配置参数
	struct Stru_Cfg
	{
		Stru_Cfg(unsigned bkCol=0,
			unsigned lineCol=0xff,
			unsigned fontCol=0xff,
			int fontSize=12,
			const char* fontName="新宋体",
			int longScaleHei=6,
			int shortScaleHei=3,
			int timespanstartScaleHei=1,
			int wndHei=16,
			int leftSpaceWid=40,
			unsigned tipLineCol=0x7f0000,
			int markPos=0)
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
			MarkPos=markPos;
			TipLineCol=tipLineCol;
		}

		unsigned	BkCol;						//背景颜色
		unsigned	LineCol;					//线条颜色
		unsigned	FontCol;					//字体颜色
		int         FontSize;					//字体大小，正数
		char        FontName[256];				//字体名称
		int         LongScaleHei;				//长刻度线的高度
		int         ShortScaleHei;				//短刻度线的高度
		int         TimespanStartScaleHei;		//时段开始时间刻度的高度
		int			WndHei;						//时间轴窗口高度
		int			LeftSpaceWid;				//时间轴左边空白区的宽度，单位为像素。这个区域画背景，不画时间轴
		int         MarkPos;					//刻度标注位置，0-数字标注在刻度中间；1-标注在右边
		unsigned	TipLineCol;					//Tip线的颜色
	};

	//定义时间刻度的范围等数据
	struct Stru_Data
	{
	public:
		//bTimeBased: 是否基于时间。可以基于普通整数。
		Stru_Data(bool bTimeBased=true);

		//清除时间刻度的范围，m_WholePeriod，注意：设置完后需要调用refresh才生效
		void clearRange(void);
		//重新设置时间刻度的范围，注意：设置完后需要调用refresh才生效
		void setRange(int start,int end);
		//重新设置时间刻度的范围，指定起始时间，和有效秒数。非交易时段的时间不算有效秒数，注意：设置完后需要调用refresh才生效
		void setRange2(int start,int SpanUnits);
		//重新计算，重新设置后，需要调用这个函数进行重新计算
		void recalcu(void) { calcuPeriods_internal(); calcuScalePos_internal(); }


		//X值是否已经超时
		bool isovertime(int iX) const;
		//根据秒数/单元数偏移量取X，相对于起始时间
		int getXbyEntireUnitOff(int UnitOff) const;
		//根据X取秒数/单元数偏移量，相对于起始时间，非法则返回-1
		int getEntireUnitOffbyX(int iX) const;
		//取每天合法的秒数，相对于交易时段，基于时间刻度有效
		int getSeconds_EachDay(void) const;

	private:
		//根据起始时间和窗口宽度，计算刻度的时间
		void calcuPeriods_internal(void);
		//计算刻度线位置
		void calcuScalePos_internal(void);


	public:
		//下面两个参数改变后，需要调用recalcu重新计算
		bool							m_bTimeBased;			//是否基于时间。如果基于时间，要考虑交易时段等和时间相关的问题
		CEasyTimespan<int>				m_WholePeriod;          //(时间)刻度起止时间
		CEasyTradingTimespan            m_WorkingTimespan;      //交易时段定义，对时间刻度有效

		//根据上面三个参数recalcu计算后得到的结果，外部可以使用，不要修改
		CEasyTimespan<int>				m_Spans;				//有效起止(时间)的集合
		int								m_WeekendDaysInclued;	//时间线包含的周末的天数，对时间刻度有效
		int								m_totalunits;			//时间线总秒数，指有效秒数，排除了非交易时段。不是时间跨度

		//下面参数对时间刻度有效
		int                             m_MarkUnit;             //标注单位，表示长刻度和短刻度的标注单位，0:按年长刻度，按月短刻度; 1:按月长刻度；按日短刻度; 2:按日长刻度，按小时短刻度; 3:按小时长刻度，按5分钟短刻度; 4:按5分钟长刻度，按1分钟短刻度
		set<time_t>						m_BreakScaleUTC;        //时间中断点的时间集
		set<int>						m_BreakScalePos;        //时间中断点的位置集合 
		set<time_t>						m_LongScaleUTC;         //长刻度的时间集   
		set<int>						m_LongScalePos;         //长刻度的位置集合 
		set<time_t>						m_ShortScaleUTC;        //短刻度的时间集
		set<int>						m_ShortScalePos;        //短刻度的位置集合

	private:
		//下面几个变量用于保存当前查询的时间段，在getEntireUnitOffbyX()中使用，目的是快速查找。针对m_Spans
		int								m_CacheTimespanOffset;	//上次查找的时间段的offset，-1表示无效
		int								m_CacheTimespanStart;	//上次查找的时间段的start，-1表示无效
		int								m_CacheTimespanEnd;		//上次查找的时间段的end，-1表示无效

	private:
		//下两个函数禁止使用
		Stru_Data(const Stru_Data& r);
		Stru_Data& operator=(const Stru_Data& r);
	};


public:
	CTimeLineWin(bool bTimeBased=true);
	virtual ~CTimeLineWin();

	//下面这个函数在winform编程中需要使用
	void AddPartWnd(int hWnd,bool brefresh=true);				//设置时间轴窗口句柄。如果用winform创建时间轴窗口，创建后，用这个方法添加hWnd句柄
	void RemovePartWnd(int hWnd,bool brefresh=true);
	
    //下面函数可以用于在MFC/Winform创建的窗口中调用
	//针对指定分窗口
    void OnPaint(int hWnd,int hDC); 

	//刷新数据以及显示参数。
	//brecalcu:是否需要重新计算。设置时间区域、交易时段后需要重新计算。时间数据变化时，应该为true
	//bsetfullwnd:是否需要设置修改缩放因子使之全屏显示。时间数据变化时，窗口数量变化时，应该为true
	//bgetwndwid:是否需要重新获取分窗口宽度。窗口数量变化时，以及窗口尺寸变化时应该为true
	//brepaint:是否刷新全部分窗口
	void refresh(bool brecalcu,bool bsetfullwnd,bool bgetwndwid,bool brepaint);


	//刷新窗口显示
	//hWnd为NULL刷新全部分窗口显示，否则刷新指定分窗口显示
	void repaint(int hWnd);


	void SetTimeTip(int hWnd,int pox);          //显示鼠标所在位置对应时间tip，鼠标在父窗口客户区中移动时调用
	void ClearTimeTip(void);                    //清除鼠标所在位置对应时间tip，鼠标离开父窗口时调用
    //获取鼠标所在时间tip的当前位置
    void getTimeTipPos(int& hWnd,int& tippos);

	
	bool zoomin(void);							//时间轴窗口放大一倍
	bool zoomout(void);							//时间轴窗口缩小一倍
	bool movebyleftkey(int offset);				//按左键，时间轴右移一定偏移量
	bool movebyrightkey(int offset);			//按右键，时间轴左移一定偏移量


	//下面两个函数是Stru_TimeLineWin_Data的函数，但需要考虑LeftSpaceWid的影响，因此重载一下
    //根据x轴位置取相应时间。hWnd是分窗口句柄，xPos是分窗口上的位置
    bool getX(int hWnd,int xPos,int& rltX) const;
    //根据时间取x轴位置，如果utc不在有效时间段内，返回false
    bool getxPos(int iX,int& hWnd,int& rltPos) const;
	//根据时间在指定窗口内取x轴位置。返回的rltPos包含LeftSpaceWid。如果utc不在窗口有效时间段内，返回false
    bool getxPosInWnd(int iX,int WndOrd,int& rltPos) const;
	//由Unit数量计算显示宽度
	inline int getDispWidByUnitCnt(int UnitCnt) const
	{
		return m_ZoomFactor>=0? (UnitCnt<<m_ZoomFactor) : (UnitCnt>>(-m_ZoomFactor));
	}
	//由显示宽度计算Unit数量
	inline int getUnitCntByDispWid(int DispWid) const 
	{
		return m_ZoomFactor>=0? (DispWid>>m_ZoomFactor) : (DispWid<<(-m_ZoomFactor));
	}
	//取某个分窗口的序号，0开始，失败返回-1
	inline int getWndOrd(int hWnd) const
	{
		for(int WndOrd=0;WndOrd<(int)m_hWnds.size();WndOrd++) if(m_hWnds[WndOrd]==hWnd) return WndOrd;
		return -1;
	}
	//取某个分窗口的DispOff，不包含LeftSpaceWid。失败返回-1
	inline int getDispOff(int hWnd) const
	{
		int WndOrd=getWndOrd(hWnd);
		if(WndOrd==-1) return -1;
		return WndOrd*m_DispWidPerWnd;
	}
	//取某个分窗口的DispOff，不包含LeftSpaceWid。失败返回-1
	inline int getDispOff2(int wndord) const
	{
		if(wndord<0||wndord>=(int)m_hWnds.size()) return -1;
		return wndord*m_DispWidPerWnd;
	}
	//取某个分窗口在整个区域的UnitOff
	inline int getEntireUnitOff(int wndord) const
	{
		if(wndord<0||wndord>=(int)m_hWnds.size()) return -1;
		return wndord*m_MaxUnitPerWnd>=m_EntireUnitCnt4Disp ? -1 : m_EntireUnitOff4Disp+wndord*m_MaxUnitPerWnd;
	}
	//取某个分窗口的UnitCnt。对于最后一个分窗口，是实际UnitCnt; 对于之前的窗口，是m_MaxUnitPerWnd
	inline int getUnitCnt(int wndord) const
	{
		if(wndord<0||wndord>=(int)m_hWnds.size()) return 0;
		int unitoff=wndord*m_MaxUnitPerWnd;
		if(unitoff>=m_EntireUnitCnt4Disp) return 0;
		return  m_EntireUnitCnt4Disp-unitoff>=m_MaxUnitPerWnd ? m_MaxUnitPerWnd : m_EntireUnitCnt4Disp-unitoff;
	}

private:
	//针对指定分窗口，在MemDC上Paint
	void PaintOnMemDC_Internal(int hWnd,int MemDC);

	//规范化m_Off4Disp和m_Wid4Disp，包括取整之类操作
	void RegularOff4Disp_Internal(void);					
	//获取指定分窗口的参数。WndWid:分窗口宽度；DispOff:显示的偏移；UnitOff:本窗口秒数/单元数偏移，UnitCnt是本窗口要显示的秒数/单元数
	bool GetDispOff_SecOff_SecCnt(int wndord,int& WndWid,int& DispOff,int& UnitOff,int& UnitCnt);


public:

	Stru_Cfg						m_Cfg;
	Stru_Data						m_Data;
	
	//下面几个变量，将各个窗口综合起来考虑
	int								m_WidPerWnd;			//每个窗口的宽度。所有窗口宽度一致
	int								m_MaxUnitPerWnd;		//每个窗口能显示的最大秒/单元数
	int								m_DispWidPerWnd;		//每个窗口能显示时间的宽度，=WndWid-LeftSpaceWid-不能整除的部分=(m_MaxUnitPerWnd>>m_ZoomFactor)
	int								m_EntireUnitOff4Disp;	//显示窗口起始位置在时间轴上的位置，秒为单位。
	int								m_EntireUnitCnt4Disp;	//当前显示的时间轴的宽度，秒为单位

	vector<int>						m_hWnds;				//时间刻度窗口句柄集。每个窗口的尺寸一定相同


public:
	int								m_ZoomFactor;			//缩放因子，2的N次方，正数放大，负数缩小

	int								m_TimeTipWnd;			//鼠标tip所在的分窗口句柄
	int								m_TimeTipPos;			//鼠标tip所在窗口的位置。在此位置显示鼠标所在时间。-1为不显示。相对于分窗口

	vector<int>						m_IntegralPointPos;		//显示范围内，整点线的位置。间隔大于32个像素时有效。相对于m_MemBmp，不包含LeftSpace
	vector<time_t>					m_IntegralPointX;		//显示范围内，整点线的X。
	int								m_IntegralPointUnitSpace;//整点线的间隔秒数，可以是1、5、15、30、60、300、900、1800、3600

private:
	int								m_MemBmp;				//组合窗口的membmp，注意，不包括LeftSpace

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TIMELINE_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
