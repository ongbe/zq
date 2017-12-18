// CurveByTimeWin.h: interface for the CCurveByTimeWin class.
//
//	显示根据时间变化的曲线的窗口类
//		1. 这个类需要依赖于TimeLineWin类
//		2. 目前只支持int型的数据类型，即最小变动价位为整数的品种。au和IF的最小变动价位为小数，不支持
//		3. 绘图时，使用m_pTimeLineWin的m_WidPerWnd变量，决定每个窗口的宽度和总宽度
//		4. 窗口尺寸变化时，上层窗口应该保证先执行TimeLineWin的OnSize处理，再调用CurveByTimeWin的RefreshMemBmp()和RePaint()
//		5. m_hWnds是曲线窗口集
//		6. 上层窗口创建曲线的顺序为
//			a. 创建TimeLineWin
//			b. 创建CurveByTimeWin
//			c. 调用pCurveByTimeWin->SetTimeLineWin(pCurveByTimeWin)
//			d. pTimeLineWin->AddWnd，pCurveByTimeWin->AddWnd，轮流执行。一定是先添加TimeLineWin窗口，再添加CurveByTimeWin窗口
//			e. 调整曲线窗口和时间轴窗口的Z序AdjustZOrder()
//			f. 添加数据
//			g. 刷新显示refresh(true,true)
//
//////////////////////////////////////////////////////////////////////

#ifndef CURVEBYTIMEWIN_H_
#define CURVEBYTIMEWIN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include <time.h>
#include <map>
#include <set>
#include <vector>
#include <string>
using std::map;
using std::set;
using std::vector;
using std::string;


#include "Tools_Ansi.h"
#include "easytime.h"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"
#include "easymutex.h"
#include "TimeLineWin.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 


//基于时间的曲线窗口类
class CCurveByTimeWin
{
public:
	//定义曲线窗口的配置参数
	struct Stru_Cfg
	{
		Stru_Cfg(unsigned bkCol=0,
			unsigned lineCol=0xff,
			unsigned fontCol=0xff,
			int fontSize=12,
			const char* fontName="新宋体",
			int scaleHei=6,
			int tickSize=1,
			int markSpacing=50,
			bool bgrid=true,
			unsigned gridCol=0x1f,
			unsigned tipLineCol=0x7f0000,
			unsigned nameTextCol=0xffffff
			)
		: TickRatio2PowerN(0)
		{
			BkCol=bkCol;
			LineCol=lineCol;
			FontCol=fontCol;
			FontSize=fontSize;
			memset(FontName,0,sizeof(FontName));
			if(!fontName) strncpy(FontName,"宋体",sizeof(FontName)-1);
			else strncpy(FontName,fontName,sizeof(FontName)-1);
			ScaleHei=scaleHei;
			TickSize=tickSize;
			MarkSpacing=markSpacing;
			bGrid=bgrid;
			GridCol=gridCol;
			TipLineCol=tipLineCol;
			NameTextCol=nameTextCol;
			bViewName=false;
		}
		Stru_Cfg(const Stru_Cfg& r) { *this=r; }
		inline int getTickCntByPixelCnt(int PixelCnt)
		{
			if(TickRatio2PowerN>=0) return (PixelCnt>>TickRatio2PowerN);
			else return (PixelCnt<<(-TickRatio2PowerN));
		}
		inline int getPixelCntByTickCnt(int TickCnt)
		{
			if(TickRatio2PowerN>=0) return (TickCnt<<TickRatio2PowerN);
			else return (TickCnt>>(-TickRatio2PowerN));
		}
		inline int getPixelCntByValue(int Val)
		{
			if(!TickSize) return 0;
			if(Val<0) return 0;
			int ticks=Val/TickSize;
			int remainder=Val%TickSize;
			int Pixels=getPixelCntByTickCnt(ticks);
			if(remainder!=0)
			{
				//放大100倍取整来计算
				Pixels+=getPixelCntByTickCnt(remainder*100/TickSize)/100;
			}
			return Pixels;
		}

		unsigned	BkCol;									//背景颜色
		unsigned	LineCol;								//线条颜色
		unsigned	FontCol;								//字体颜色
		int         FontSize;								//字体大小，正数
		char        FontName[256];							//字体名称
		int         ScaleHei;								//坐标轴刻度线的高度
		int			TickSize;								//数据的最小变动单位
		int			TickRatio2PowerN;						//每个tick的显示倍率，表示用多少个像素表示一个tick。用2的N次方表示(-8 - 8)。2^0=1; 2^1=2; 2^2=4;2^=8;...2^-1=0.5; 2^-2=0.25
		int			MarkSpacing;							//标注间距
		bool		bGrid;									//是否需要画网格线
		unsigned	GridCol;								//网格线颜色
		unsigned	TipLineCol;								//Tip线的颜色
		unsigned	NameTextCol;							//曲线名称文本颜色
		bool		bViewName;								//是否显示曲线名称。显示在每个分窗口的曲线开始处
	};

	//定义一条曲线的数据
	struct Stru_CurveData
	{
	public:
		//曲线类型枚举
		enum eDrawMode
		{
			DrawMode_connectbyline=0,		//用直线连接各个顶点
			DrawMode_pointonly,				//只画顶点
			DrawMode_crosspoint,			//画交叉点
			DrawMode_histogram				//画直方图
		};
		//曲线种类
		enum eCurveDataType
		{
			CurveDataType_Other=0,			//其它曲线数据
			CurveDataType_LastPrice,		//最新价曲线数据
			CurveDataType_BidPrice,			//买价曲线数据
			CurveDataType_AskPrice,			//卖价曲线数据
			CurveDataType_BuyPoint,			//买点曲线数据
			CurveDataType_SellPoint,		//卖点曲线数据
			CurveDataType_Average1,			//均线1
			CurveDataType_Average2,			//均线2
			CurveDataType_Average3,			//均线3
			CurveDataType_Average4,			//均线4
			CurveDataType_WeightedAverage1,	//加权均线1
			CurveDataType_WeightedAverage2,	//加权均线2
			CurveDataType_WeightedAverage3,	//加权均线3
			CurveDataType_WeightedAverage4,	//加权均线4
			CurveDataType_Performance,		//绩效曲线数据
			CurveDataType_Volume			//成交量曲线数据
		};

		//点数据结构，x一般是时间，y为对应的数据，如价格
		struct Stru_Data
		{
			Stru_Data() : x(0),y(0) {}
			Stru_Data(int ix,int iy) : x(ix),y(iy) {}

			int x,y;
		};

		//定义曲线数据块。一条曲线的数据由多个数据块组成。数据块之间按开始时间排序，相互不交叉
		//每个数据块由按时间排序的点数据组成
		struct Stru_DataBlock
		{
			Stru_DataBlock(): pData(NULL),MaxData(0),DataCnt(0),bFromDB(true) {}
			Stru_DataBlock(int maxData,bool bfromDB)
			{
				new(this) Stru_DataBlock(NULL,maxData,0,bfromDB);
			}
			Stru_DataBlock(const Stru_DataBlock& r)
			{
				new(this) Stru_DataBlock(r.pData,r.MaxData,r.DataCnt,r.bFromDB);
			}
			~Stru_DataBlock()
			{
				if(pData) { free(pData);  pData=NULL; }
			}
			Stru_DataBlock& operator= (const Stru_DataBlock& r)
			{
				if(pData) { free(pData);  pData=NULL; }

				new(this) Stru_DataBlock(r.pData,r.MaxData,r.DataCnt,r.bFromDB);
				return *this;
			}
			bool add(const Stru_Data& idata)
			{
				if(DataCnt>=MaxData) return false;
				pData[DataCnt++]=idata;
				return true;
			}
			void clear(void) { DataCnt=0; }

			Stru_Data*	pData;
			int			MaxData;
			int			DataCnt;
			bool		bFromDB;		//true: data from DB;  false: data from realtime quotation

		private:
			Stru_DataBlock(const Stru_Data* pdata,int maxData,int dataCnt,bool bfromDB)
			{
				new(this) Stru_DataBlock();

				MaxData=maxData;
				DataCnt=dataCnt;
				if(MaxData<DataCnt) MaxData=DataCnt;
				pData=(Stru_Data*)malloc(MaxData*sizeof(Stru_Data));
				if(pData&&pdata&&DataCnt>0) memcpy(pData,pdata,DataCnt*sizeof(Stru_Data));
				else DataCnt=0;
				bFromDB=bfromDB;
			}

		};


		Stru_CurveData(eCurveDataType dataType,eDrawMode drawMode=DrawMode_connectbyline,
			int curvePenWid=1,
			unsigned curveCol=0x7f,unsigned curvePtCol=0xff,
			unsigned curveCol2=0x7f,unsigned curvePtCol2=0xff,
			unsigned curveCol0=0x7f,unsigned curvePtCol0=0xff)
		:	bView(true),
			DataType(dataType),
			DrawMode(drawMode),
			CurvePenWid(curvePenWid),
			CurveCol(curveCol),
			CurvePtCol(curvePtCol),
			CurveCol2(curveCol2),
			CurvePtCol2(curvePtCol2),
			CurveCol0(curveCol0),
			CurvePtCol0(curvePtCol0)
		{
		}
		~Stru_CurveData()
		{
		}
		Stru_CurveData(const Stru_CurveData& r)
		{
			*this=r;
		}
		Stru_CurveData& operator= (const Stru_CurveData& r)
		{
			Name=r.Name;
			mapDataBlocks=r.mapDataBlocks;
			bView=r.bView;
			DataType=r.DataType;
			DrawMode=r.DrawMode;
			CurvePenWid=r.CurvePenWid;
			CurveCol=r.CurveCol;
			CurvePtCol=r.CurvePtCol;
			CurveCol2=r.CurveCol2;
			CurvePtCol2=r.CurvePtCol2;
			CurveCol0=r.CurveCol0;
			CurvePtCol0=r.CurvePtCol0;
			return *this;
		}
		int GetPointCnt(void)
		{
			int rlt=0;
			for(map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.begin();it!=mapDataBlocks.end();it++)
				rlt+=it->second.DataCnt;
			return rlt;
		}
		bool GetLastData(Stru_Data& rlt)
		{
			map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.end();
			while(it!=mapDataBlocks.begin())
			{
				it--;
				if(it->second.DataCnt>0) { rlt=it->second.pData[it->second.DataCnt-1];  return true; }
			}
			return false;
		}
		bool GetMinX(int& MinX)
		{
			if((int)mapDataBlocks.size()==0) return false;
			MinX=mapDataBlocks.begin()->first;
			return true;
		}
		bool GetMaxX(int& MaxX)
		{
			map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.end();
			while(it!=mapDataBlocks.begin())
			{
				it--;
				if(it->second.DataCnt>0) { MaxX=it->second.pData[it->second.DataCnt-1].x;  return true; }
			}
			return false;
		}
		void add(const Stru_Data& data,bool bForceNewBlock,bool bFromDB);
		void setName(const string& name) { Name=name; }
		string getName(void) { return Name; }


		//数据块的集合，firstTime -> DataBlock
		//1. 数据块内部数据按时间排序
		//2. 数据块按首条数据时间排序
		//3. 不同数据块的时间不交叉
		string						Name;
		map<int,Stru_DataBlock>		mapDataBlocks;			//曲线数据块, map startx to datablock
		bool						bView;					//是否显示
		eCurveDataType				DataType;				//曲线数据类型
		eDrawMode					DrawMode;				//曲线绘制模式
		int							CurvePenWid;			//曲线画笔宽度，画点模式无效
		unsigned					CurveCol;				//上行曲线颜色
		unsigned					CurvePtCol;				//上行曲线端点颜色
		unsigned					CurveCol2;				//下行曲线颜色
		unsigned					CurvePtCol2;			//下行曲线端点颜色
		unsigned					CurveCol0;				//平行曲线颜色
		unsigned					CurvePtCol0;			//平行曲线端点颜色

		static const int			blocksz=65536;
	};


	//描述每一条曲线在一个分窗口中的显示范围，对应于m_pTimeLineWin->m_StartUTC4Disp和m_pTimeLineWin->m_EndUTC4Disp
	struct Stru_DispRange
	{
		Stru_DispRange() { clear(); };
		void clear(void) { StartBlock=StartDataOrd=EndBlock=EndDataOrd=-1; };

		int StartBlock;										//m_StartUTC4Disp对应的数据块的起始x
		int StartDataOrd;									//m_StartUTC4Disp对应的数据块里的数据项序号
		int EndBlock;										//m_EndUTC4Disp对应的数据块的起始x，可以是-1
		int EndDataOrd;										//m_EndUTC4Disp对应的数据块里的数据项序号，可以是-1
	};

	//每个分窗口的显示参数。如果曲线窗口分N层，则每层窗口都有M个分窗口
	struct Stru_PartWndParam
	{
		Stru_PartWndParam();
		Stru_PartWndParam(int hwnd,bool bdrawTopBorder,unsigned topBorderCol);
		Stru_PartWndParam(const CCurveByTimeWin::Stru_PartWndParam& r);
		~Stru_PartWndParam();
		Stru_PartWndParam& operator= (const Stru_PartWndParam& r);
		void ReleaseResource(void);

		int							hWnd;					//曲线窗口句柄
		bool						bDrawTopBorder;			//是否要画顶上的边界。对于第二层曲线窗口，应该画一条顶上的边界线
		unsigned					TopBorderCol;			//顶上边界线的颜色
		int							TickMin;				//窗口的tickmin值
		int							hMemBmp;				//窗口的MemBmp，包含LeftSpaceWid
		int*						pValByPox;				//Pox对应的Y值。用pValByPox=(int*)malloc(CntOfPox*sizeof(int))方式申请。pValByPox!=0&&pPoyByPox!=0为有效
		int*						pPoyByPox;				//Pox对应的Poy值。用pPoyByPox=(int*)malloc(CntOfPox*sizeof(int))方式申请
		int							CntOfPox;				//Pox的数量，也就是窗口宽度，包括LeftSpaceWid
		int							StartXDisp;				//显示范围开始对应的utc，不考虑是否有数据，只考虑m_pTimeLineWin的范围
		int							EndXDisp;				//显示范围结束对应的utc，不考虑是否有数据，只考虑m_pTimeLineWin的范围
		vector<Stru_DispRange>		CurveDispRanges;		//描述每条曲线的显示范围；对应于m_pTimeLineWin分窗口的EntireUnitOff()和UnitCnt
	};

public:
	CCurveByTimeWin();
	virtual ~CCurveByTimeWin();

	//下面这个函数在winform编程中需要使用
	void SetTimeLineWin(CTimeLineWin* pTimeLineWin) { m_pTimeLineWin=pTimeLineWin; }
	void AddWnd(int hWnd,bool bdrawTopBorder,unsigned topBorderCol,bool brefresh=true);									//设置曲线窗口句柄。如果用winform创建曲线窗口，创建后，用这个方法设置hWnd句柄
	void RemoveWnd(int hWnd,bool brefresh=true);
	//调整曲线窗口和时间轴窗口的z序，保证pTimeLineWin先刷新
	void AdjustZOrder(void);

	//刷新窗口显示
	//hWnd为NULL刷新全部分窗口显示，否则刷新指定分窗口显示
	void repaint(int hWnd);
	
	//刷新窗口显示参数
	//breset: 获取显示范围，缩放倍数复原到0，重新计算各个窗口的TickMin(使分窗口第一个数据点在窗口中间)
	//brepaint: 刷新全部分窗口
	void refresh(bool bresetRatio,bool bresetTickMin,bool brefreshmembmp,bool brepaint);

private:
	//在指定分窗口的MemBmp上画背景、坐标轴、网格线
	void DrawFrameInMemBmp_Internal(int MemDC,int WndOrd,int WndWid,int WndHei,int DispOff,int SecOff,int SecCnt,int LeftSpaceWid);
	//在指定分窗口的MemBmp上画曲线
	void DrawCurveInMemBmp_Internal(int MemDC,int WndOrd,int WndWid,int WndHei,int DispOff,int LeftSpaceWid,int StartX=-1,int EndX=-1,int*pMinPox=NULL,int* pMaxPox=NULL);

public:
	//刷新指定分窗口的MemBmp
	void RefreshMemBmp(int hWnd);
	//在指定分窗口的MemBmp上刷新指定时间范围内的曲线
	void RefreshMemBmp_Part(int startX,int endX);
    void OnPaint(int hWnd,int hDC); 

	void zoomin(void);										//曲线纵向放大一倍
	void zoomout(void);										//曲线纵向缩小一倍
	void moveup(int hWnd,int offset);						//曲线上移一定偏移量，如果hWnd==NULL，所有分窗口都上移
	void movedown(int hWnd,int offset);						//曲线下移一定偏移量，如果hWnd==NULL，所有分窗口都下移

	//取某个分窗口的序号，0开始，失败返回-1
	inline int getWndOrd(int hWnd) const
	{
		int WndOrd;
		for(WndOrd=0;WndOrd<(int)m_WndParams.size()&&m_WndParams[WndOrd].hWnd!=hWnd;WndOrd++);
		if(WndOrd<(int)m_WndParams.size()) return WndOrd;
		else return -1;
	}
	//取X所在分窗口的序号，0开始，失败返回-1
	inline int getWndOrdByX(int X) const
	{
		int WndOrd;
		if(m_WndParams.size()==0||m_WndParams[0].CurveDispRanges.size()==0) return -1;
		for(WndOrd=0;WndOrd<(int)m_WndParams.size();WndOrd++)
			if(m_WndParams[WndOrd].StartXDisp<=X&&m_WndParams[WndOrd].EndXDisp>X) return WndOrd;
		return -1;
	}

	//将所有曲线的最后一个数据相比较，取最小的x，失败返回-1
	int GetMinLastX_AllCurve(void)
	{
		int minx=-1;
		Stru_CurveData::Stru_Data LastData;
		for(int i=0;i<(int)m_Curves.size();i++)
		{
			if(m_Curves[i].GetLastData(LastData)&&(minx==-1||minx>LastData.x))
				minx=LastData.x;
		}
		return minx;
	}

	int GetFirstVisibleCurveOrd(void);

private:

	//获取每条曲线的显示范围,m_CurveDispRanges
	void GetCurveDispRange(void);
	//按初始条件计算各分窗口的TickMin(使分窗口第一个数据点在窗口中间)
	void ResetTickMins(void);
	//获取第一个大于等于指定UTC的数据位置，失败时二者都返回-1
	void GetFirstDataPositionByX(int iX,int curveord,int& blockord,int& dataord);
	//获取第一个大于等于指定UTC的数据
	bool GetFirstDataByX(int iX,int curveord,Stru_CurveData::Stru_Data& data);

public:
	Stru_Cfg						m_Cfg;					//曲线的配置参数

	CTimeLineWin*					m_pTimeLineWin;			//对应的时间轴窗口的指针。要保证在对象生命期内，m_pTimeLineWin是有效的

	vector<Stru_CurveData>			m_Curves;				//曲线数据指针。有可能有多条曲线。对每条曲线，先push_back一个空的曲线，再设置该曲线数据

	vector<Stru_PartWndParam>		m_WndParams;

	Ceasymutex				        m_mutex;

private:
    //下两个函数禁止使用
    CCurveByTimeWin(const CCurveByTimeWin& r);
    CCurveByTimeWin& operator=(const CCurveByTimeWin& r);
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(CURVEBYTIMEWIN_H_)
