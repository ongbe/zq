#pragma once
#include <windows.h>
#include "winbase.h"
#include "CommonPkg.h"
#include "fttcommdef.h"
#include "socketbase.h"
#define  MESSAGE_GETDATA    			WM_USER + 1018
#define  MESSAGE_GETSTATISITIC   		WM_USER + 1019
#define  MESSAGE_GETSTATISITIC_PUSH   	WM_USER + 1020
#define  MESSAGE_HEARTBEATRSP		    WM_USER + 1021



#define MONITOR_GETSTATISITIC			0x00001001  //发送统计数据请求
#define MONITOR_GETSTATISITIC_Rsp		0x00001002  //得到统计数据请求
#define MONITOR_GETSTATISITIC_Push		0x00001003  //得到统计数据请求(每分钟)


#define MONITOR_SENDCALCDATA_Rsp		0x00001100  //发送计算数据到监控终端（所有）

struct RecvData 
{
	Stru_UniPkgUDPHead head;	//包头
	void* pData;				//接收到的数据内容
};
struct RecvDataMsg//服务端消息
{
	Stru_UniPkgUDPHead head;	//包头
	void* pData;				//接收到的数据内容
	Stru_IPAddress RemoteAddr;
	SOCKET hSocket;
};

enum DataStatus
{
	UPRECEV		= 1,//1：上行接收
	UPSEND		= 2,//2：上行发送
	DOWNRECEV	= 3,//3：下行接收
	DOWNSEND    = 4,//4：下行发送
	
	UPSENDFAIL    = 5,// 5：上行发送失败
	DOWNSENDFAILE = 6,//6: 下行发送失败


};
//服务端的报单
struct ServerOrder
{//对应的这个结构Stru_UniPkgUDPHead
	unsigned int cmdid;		//FFT_API_START开始是上行；FFT_SPI_START开始是下行
	int			 len;		
	unsigned int seq;	
	unsigned int userdata1;	//业务请求ID
	unsigned int userdata2;	//1：上行接收；2：上行发送；3：下行接收；4：下行发送；5：上行发送失败6：下行发送失败
	unsigned int userdata3; //服务器端seq
	unsigned int userdata4; //客户端socket
	long		 nUTCTime;	//utc秒数
	long		 nMiliSecond;//毫秒数
};
/*
//客户端的报单
struct ClientOrder 
{
	int nCmdID;				//命令字
	int nSeq;				//报单唯一性
	char Direction;			// '0'上行；‘1’下行
	time_t tmSend;			//接收时间
	time_t tmRecv;			//接收时间
	time_t tmDay;			//日期
	ClientOrder()
	{
		memset(this,0,sizeof(*this));
	}
};*/

//
struct StatisticsData
{
	time_t tmTime; //每秒的时间

	DWORD dwUpRecv;//上行接收总数
	DWORD dwUpSend;//上行总数

	DWORD dwDownRecv;//下行接收总数
	DWORD dwDownSend;//下行总数
	
	time_t dwUpDelay;//上行平均延迟
	time_t dwDownDelay;//下行平均延迟

	StatisticsData()
	{
		memset(this,0,sizeof(*this));
	}
	bool operator< (const StatisticsData& r) const
	{
		if(tmTime<r.tmTime)
			return true;
		if(tmTime>r.tmTime)
			return false;

		if(dwUpRecv<r.dwUpRecv)
			return true;
		if(dwUpRecv>r.dwUpRecv)
			return false;
		
		if(dwUpSend<r.dwUpSend)
			return true;
		if(dwUpSend>r.dwUpSend)
			return false;

		if(dwDownRecv<r.dwDownRecv)
			return true;
		if(dwDownRecv>r.dwDownRecv)
			return false;

		if(dwDownSend<r.dwDownSend)
			return true;
		if(dwDownSend>r.dwDownSend)
			return false;

		if(dwUpDelay<r.dwUpDelay)
			return true;
		if(dwUpDelay>r.dwUpDelay)
			return false;

		if(dwDownDelay<r.dwDownDelay)
			return true;
		if(dwDownDelay>r.dwDownDelay)
			return false;

		return false;
	}
};
#pragma pack(push)
#pragma pack(1)

//typedef struct {
//	Stru_UniPkgHead base;
//	int nUtcTime;
//	int nMS;
//} Stru_UDPPkgHead;

typedef struct {
	Stru_UniPkgHead base;
	int nServSeq;
	int nSocket;
} Stru_ThreadData;

typedef struct {
	int CpuUsage;		// 当前cpu占用率，千分率
	INT64 Memory;		// 当前内存使用量
	INT64 DiskSpace;	// 服务程序所在硬盘当前可用空间
} Stru_Watch;

#pragma pack(pop)

struct StatData
{
	struct tm dwTime; //每秒的时间

	DWORD dwUpRecv;//上行接收总数
	DWORD dwUpSend;//上行总数

	DWORD dwDownRecv;//下行接收总数
	DWORD dwDownSend;//下行总数

	DWORD dwUpDelay;//上行总延迟
	DWORD dwDownDelay;//下行总延迟

	StatData()
	{
		memset(this,0,sizeof(*this));
	}
	bool operator< (const StatData& r) const
	{
		if(dwTime.tm_year > r.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year < r.dwTime.tm_year)
			return false;

		if(dwTime.tm_mon > r.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon < r.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday > r.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday < r.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour > r.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour < r.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min > r.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min < r.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec > r.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec < r.dwTime.tm_sec)
			return false;
		if(dwUpRecv<r.dwUpRecv)
			return true;
		if(dwUpRecv>r.dwUpRecv)
			return false;

		if(dwUpSend<r.dwUpSend)
			return true;
		if(dwUpSend>r.dwUpSend)
			return false;

		if(dwDownRecv<r.dwDownRecv)
			return true;
		if(dwDownRecv>r.dwDownRecv)
			return false;

		if(dwDownSend<r.dwDownSend)
			return true;
		if(dwDownSend>r.dwDownSend)
			return false;

		if(dwUpDelay<r.dwUpDelay)
			return true;
		if(dwUpDelay>r.dwUpDelay)
			return false;

		if(dwDownDelay<r.dwDownDelay)
			return true;
		if(dwDownDelay>r.dwDownDelay)
			return false;

		return false;
	}
};/*
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
  }
  };	// 分时图数据结构*/
struct STItem
{
	int nItem;
	//STLine* pItem;
	MSG* pMsg; 
	bool bIsMouse;   //鼠标消息
	bool bIsKeyBoard;//键盘消息
	//CPoint wxPt;    //鼠标当前位置，比如在k线内移动，需要告诉量能现在的鼠标移动位置（x轴是一样的）
	STItem()
	{
		nItem = -1;
		//pItem = NULL;
		pMsg  = NULL;
		bIsKeyBoard = false;
		bIsMouse    = false;	
	//	wxPt = CPoint(-1,-1);
	}
	STItem& operator = (STItem & other)
	{
		if(this == & other)
			return *this;

		nItem  = other.nItem;
		//	pItem     = other.pItem;
		pMsg      = other.pMsg;
		bIsKeyBoard = other.bIsMouse;
		bIsMouse    = other.bIsMouse;
	//	wxPt		= other.wxPt;
	}
};
#define MAX_VALUE_DEFAULT   100000000
#define MIN_VALUE_DEFAULT  -100000000
enum eMouseModes 
{ 
	MOUSE_NOTHING ,
	MOUSE_OVER_ITEM_DIVIDE,
	MOUSE_PREPARE_DRAGITEM, 	
};