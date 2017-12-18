#pragma once
#pragma pack(push)
#pragma pack(1)
#include <windows.h>
#include <time.h>
#include <fstream>
#include <map>
#include <vector>
#define MAX_VALUE_DEFAULT   100000000
#define MIN_VALUE_DEFAULT  -100000000
#define COLOR_MAX   9
#define	 TIMER_2			1
#define	 TIMER_1			0

#define  MESSAGE_LEFTDBCLICK		WM_USER + 1011
#define  MESSAGE_RETURN     		WM_USER + 1012
#define  MESSAGE_QutoInTime    		WM_USER + 1018
#define  MESSAGE_CONNECTSUCCESS   	WM_USER + 1019
#define  MESSAGE_CONNECTFAIL   		WM_USER + 1020
#define  MESSAGE_QutoTLineHistory   WM_USER + 1021
#define  MESSAGE_QutoKLineHistory   WM_USER + 1023

#define  MESSAGE_CHANGEVIEW   		WM_USER + 1026
#define  MESSAGE_CHANGETimeSPAN		WM_USER + 1027

#define  MESSAGE_INDEX_EXPRESS		WM_USER + 1037
#define  MESSAGE_INDEX_CONFIG		WM_USER + 1038

#define  MESSAGE_CONFIG_CHANGE		WM_USER + 1039
enum eMouseModes 
{ 
	MOUSE_NOTHING ,
	MOUSE_OVER_ITEM_DIVIDE,
	MOUSE_PREPARE_DRAGITEM, 	
};
enum EnumPhrase
{
	PHRASE_1MIN		= 1,
	PHRASE_5MIN		= 5,
	PHRASE_15MIN	= 15,
	PHRASE_30MIN	= 30,
	PHRASE_60MIN	= 60,
	PHRASE_DAY		= 24*60,
	PHRASE_WEEK		= 7*24*60,
	PHRASE_MONTH	= 31*24*60,
	PHRASE_USERDEFINE = 0
}; 
struct SKLine   //k线数据结构，1分钟 到 月线都是这个数据结构
{
	struct tm  dwTime;              //utc时间 包括日期时间。 这个时间与真实时间差1900;比如: dwTime.tm_year = 111;代表111+1900 = 2011年 (用这个结构，还不如用dword，后面开发麻烦了好多)
	double fOpenPrice;				// 开盘价
	double fHighPrice;				// 最高价
	double fLowPrice;				// 最低价
	double fClosePrice;				// 收盘价
	double fSumTradeVolume;			//成交金额
	DWORD dwVolume;					//成交量
	DWORD dwHoldVolume;				//持仓量
	double dJieSuan;                //结算价
	SKLine& operator=(const SKLine& other)
	{
		dwTime.tm_hour = other.dwTime.tm_hour;
		dwTime.tm_isdst = other.dwTime.tm_isdst;
		dwTime.tm_mday  = other.dwTime.tm_mday;
		dwTime.tm_min   = other.dwTime.tm_min;
		dwTime.tm_mon   = other.dwTime.tm_mon;
		dwTime.tm_sec   = other.dwTime.tm_sec;
		dwTime.tm_wday  = other.dwTime.tm_wday;
		dwTime.tm_yday  = other.dwTime.tm_yday;
		dwTime.tm_year  = other.dwTime.tm_year;

		fOpenPrice      = other.fOpenPrice;
		fHighPrice		= other.fHighPrice;		
		fLowPrice		= other.fLowPrice;			
		fClosePrice		= other.fClosePrice;				
		fSumTradeVolume	= other.fSumTradeVolume;		
		dwVolume		= other.dwVolume;					
		dwHoldVolume	= other.dwHoldVolume;				
		dJieSuan		= other.dJieSuan;      
		return *this;
	}
	bool operator<(const SKLine&	src) const
	{//从大到小排序;
		if(dwTime.tm_year > src.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year < src.dwTime.tm_year)
			return false;

		if(dwTime.tm_mon > src.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon < src.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday > src.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday < src.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour > src.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour < src.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min > src.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min < src.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec > src.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec < src.dwTime.tm_sec)
			return false;

		return false;
	}
} ;	// K线图数据结构  


struct STLine //分时数据结构
{
	struct tm  dwTime;               //时间
	double fLastPrice;				 //价格
	DWORD dwVolume;					 //成交量
	double dwVolumeMoney;			 //成交金额
	DWORD dwHoldVolume;				 //持仓量

	STLine& operator=(const STLine& other)
	{
		dwTime.tm_hour = other.dwTime.tm_hour;
		dwTime.tm_isdst = other.dwTime.tm_isdst;
		dwTime.tm_mday  = other.dwTime.tm_mday;
		dwTime.tm_min   = other.dwTime.tm_min;
		dwTime.tm_mon   = other.dwTime.tm_mon;
		dwTime.tm_sec   = other.dwTime.tm_sec;
		dwTime.tm_wday  = other.dwTime.tm_wday;
		dwTime.tm_yday  = other.dwTime.tm_yday;
		dwTime.tm_year  = other.dwTime.tm_year;

		fLastPrice      = other.fLastPrice;
		dwVolume		= other.dwVolume;
		dwVolumeMoney   = other.dwVolumeMoney;
		dwHoldVolume	= other.dwHoldVolume;
		return *this;
	}
	bool operator<(const STLine&	src) const
	{//从大到小排序;
		if(dwTime.tm_year > src.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year < src.dwTime.tm_year)
			return false;

		if(dwTime.tm_mon > src.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon < src.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday > src.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday < src.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour > src.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour < src.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min > src.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min < src.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec > src.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec < src.dwTime.tm_sec)
			return false;

		return false;
	}/*
	bool operator<(const STLine&	src) const
	{
		if(dwTime.tm_year < src.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year > src.dwTime.tm_year)
			return false;
		
		if(dwTime.tm_mon < src.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon > src.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday < src.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday > src.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour < src.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour > src.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min < src.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min > src.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec < src.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec > src.dwTime.tm_sec)
			return false;

		return false;
	}*/
};	// 分时图数据结构

struct STItem
{
	int nItem;
	STLine* pItem;
	MSG* pMsg; 
	bool bIsMouse;   //鼠标消息
	bool bIsKeyBoard;//键盘消息
	CPoint wxPt;    //鼠标当前位置，比如在k线内移动，需要告诉量能现在的鼠标移动位置（x轴是一样的）
	STItem()
	{
		nItem = -1;
		pItem = NULL;
		pMsg  = NULL;
		bIsKeyBoard = false;
		bIsMouse    = false;	
		wxPt = CPoint(-1,-1);
	}
	STItem& operator = (STItem & other)
	{
		if(this == & other)
			return *this;

		nItem  = other.nItem;
		pItem     = other.pItem;
		pMsg      = other.pMsg;
		bIsKeyBoard = other.bIsMouse;
		bIsMouse    = other.bIsMouse;
		wxPt		= other.wxPt;
	}
};
struct STechCalcPara
{//计算技术指标时候用到的参数:时间周期用于读取本地存的不同周期的参数;合约乘数用于成交金额和量之间的关系
	EnumPhrase enumPhrase;		//时间周期
	int		m_VolumeMultiple;	//合约乘数
};
























//////////////////////////////////////////////////////////////////////////
//一下来自机构版里面的定义，不依赖那个目录的文件
//InstitutionalTrading\CommonDef\CommonStruct.h

/*
enum  KLineDataCycle
{
	Minute_1 = 1,
	Minute_3 =3,
	Minute_5=5,
	Minute_10=10,
	Minute_15=15,
	Minute_30=30,
	Minute_60=60,
	Day_1,
	Week_1,
	Month_1,
};*/
typedef char _InstrumentID[31];/*
//K线
struct KLineData
{
	KLineDataCycle	eCycle;           //K线周期
	char			szDate[16];			   // 日期yyyymmdd
	char			szTime[9];			  // 时间hh:mm:ss
	_InstrumentID	InstrumentID;     //合约代码
	double          OpenPrice;			  //开盘价	
	double	        HighestPrice;         ///最高价	
	double	        LowestPrice;          ///最低价
	double	        ClosePrice;           /// 收盘价
	int         	IncreaseVolume;              ///成交量增量
	int         	TotalVolume;              ///成交量总量
	double          Turnover;			//成交金额增量
	int            OpenInterest;				//持仓量增量
	double         SettlementPrice;                //结算价

};
///分时线
struct TimeLineData
{
	char			szDate[16];			   // 日期
	char			szTime[9];			  // 时间
	_InstrumentID	InstrumentID;     //合约代码
	double	        ClosePrice;           /// 收盘价
	int         	Volume;              ///成交量增量
	double          Turnover;			//成交金额增量
	int             OpenInterest;		//持仓量增量

};*/
//查询K线、分时线结构
struct KLineQuery
{	
	time_t			        szStartTime;		// 
	time_t			        szEndTime;		//
	_InstrumentID	        InstrumentID;			//合约代码
	EnumPhrase				nCycle;					//K线周期	
	int                     nDataType;				//1-K线数据，2-分钟线数据
};
struct KLineBase   //k线数据结构，1分钟 到 月线都是这个数据结构
{
	time_t dwTime;						//时间，从1900年1月1日开始的秒数
	double fOpenPrice;					// 开盘价
	double fHighPrice;					// 最高价
	double fLowPrice;					// 最低价
	double fClosePrice;					// 收盘价
	double fTurnover;					//成交金额
	DWORD dwVolume;						//成交量
	DWORD dwOpenInterest;				//持仓量
	KLineBase& operator=(const KLineBase& other)
	{
		dwTime			= other.dwTime;

		fOpenPrice      = other.fOpenPrice;
		fHighPrice		= other.fHighPrice;		
		fLowPrice		= other.fLowPrice;			
		fClosePrice		= other.fClosePrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		//dJieSuan		= other.dJieSuan;      
		return *this;
	}
	bool operator<(const KLineBase&	src) const
	{//从大到小排序;
		return dwTime > src.dwTime;
	}
} ; 

struct TLineBase //分时数据结构
{
	time_t dwTime;						//时间，从1900年1月1日开始的秒数
	double fLastPrice;					// 收盘价
	double fTurnover;					//成交金额
	DWORD dwVolume;						//成交量
	DWORD dwOpenInterest;				//持仓量
	TLineBase& operator=(const TLineBase& other)
	{
		dwTime			= other.dwTime;

		fLastPrice		= other.fLastPrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		return *this;
	}
	bool operator<(const TLineBase&	src) const
	{//从大到小排序;
		return dwTime > src.dwTime;
	}
};	
//////////////////////////////////////////////////////////////////////////
//行情接收ID，以后去掉
#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))
#define  CMDID_BASE_HIWORD_QUOT						0x000F //行情高位CMDID
#define Cmd_SubscribeQuotEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0001)	//订阅行情事件请求
#define Cmd_SubscribeQuotEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0002)	//订阅行情事件返回
#define Cmd_UnSubscribeQuotEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0003)	//退订行情事件请求
#define Cmd_UnSubscribeQuotEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0004)	//退订行情事件返回
#define Cmd_QuotEvent_Push								    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0005)	//行情事件推送

#define Cmd_QueryQuotKLineData_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0010)	//查询行情K线事件请求
#define Cmd_QueryQuotKLineData_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0011)	//查询行情K线事件返回
#define Cmd_QueryQuotTimeLineData_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0012)	//查询行情分时线事件请求
#define Cmd_QueryQuotTimeLineData_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0013)	//查询行情分时线事件返回

#define CF_ERROR_SUCCESS							0
