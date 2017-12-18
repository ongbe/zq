#pragma once
/*/////////////////////////////////////////////////////////////////////////
这个头文件里面主要用于进行IO操作时候用的数据结构，节省存储以及读取文件的时间

/////////////////////////////////////////////////////////////////////////*/

struct SKLine_IO   //k线数据结构，1分钟 到 月线都是这个数据结构
{
	DWORD  dwTime;                  //utc时间 包括日期时间。 这个时间与真实时间差1900；比如: dwTime.tm_year = 111;代表111+1900 = 2011年 (用这个结构，还不如用dword，后面开发麻烦了好多)
	double fOpenPrice;				// 开盘价
	double fHighPrice;				// 最高价
	double fLowPrice;				// 最低价
	double fClosePrice;				// 收盘价
	double fSumTradeVolume;			//成交金额
	DWORD dwVolume;					//成交量
	DWORD dwHoldVolume;				//持仓量
	double dJieSuan;                //结算价
	SKLine_IO& operator=(const SKLine_IO& other)
	{
		dwTime			 = other.dwTime;
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
	
} ;	

struct STLine_IO 
{
	DWORD dwTime;               //时间
	double fLastPrice;				 //价格
	DWORD dwVolume;					 //成交量
	double dwVolumeMoney;			 //成交金额
	DWORD dwHoldVolume;				 //持仓量
	
	STLine_IO& operator=(const STLine_IO& other)
	{
		dwTime			= other.dwTime;
		fLastPrice      = other.fLastPrice;
		dwVolume		= other.dwVolume;
		dwVolumeMoney   = other.dwVolumeMoney;
		dwHoldVolume	= other.dwHoldVolume;

		return *this;
	}
};	
struct SQIntime_IO                      //实时行情数据
{
	DWORD	dwTime;							//时间
	double	fLastPrice;						    //价格
	DWORD	dwVolume;						    //成交量
	DWORD	dwOpenVolume;					    //开量
	DWORD	dwCloseVolume;					    //平量
	int     nDesc;                              //0:空开; 1:多开; 2:多平; 3:空平; 4:多换; 5:空换;


	SQIntime_IO& operator=(const SQIntime_IO& other)
	{
		dwTime			= other.dwTime;
		fLastPrice      = other.fLastPrice;
		dwVolume		= other.dwVolume;
		dwOpenVolume	= other.dwOpenVolume;
		dwCloseVolume	= other.dwCloseVolume;
		nDesc         = other.nDesc;
		return *this;
	}
};