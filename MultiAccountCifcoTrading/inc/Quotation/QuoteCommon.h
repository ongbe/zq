#pragma once

#include <windows.h>
#include <time.h>
#include <fstream>
#include <map>
#include <vector>
#include "TechDefine.h"
#include "..\\inc\\ISvr\\BusinessData.hpp"
//////////////////////////////////////////////////////////////////
#define MAX_VALUE_DEFAULT   100000000
#define MIN_VALUE_DEFAULT  -100000000
#define COLOR_MAX   9
enum KTYPE
{
	KTYPE_KLINE		= 0X001,// k线
	KTYPE_VOLUME	= 0X002,//量能
	KTYPE_TECHINDEX = 0X004,//技术指标
	KTYPE_SCALES	= 0X008,//坐标抽
};
enum TTYPE
{
	TTYPE_TLINE		= 0X001,// 分时线
	TTYPE_VOLUME	= 0X002,//分时量能
	TTYPE_TECHINDEX = 0X004,//分时技术指标
	TTYPE_SCALES	= 0X008,//分时坐标抽
};
enum EnumPhrase
{
	PHRASE_1MIN		= 1,
	PHRASE_5MIN		= 5,
	PHRASE_15MIN	= 15,
	PHRASE_30MIN	= 30,
	PHRASE_60MIN	= 60,
	PHRASE_DAY,
	PHRASE_WEEK,
	PHRASE_MONTH,
	PHRASE_USERDEFINE
}; 


struct SKLine   //k线数据结构，1分钟 到 月线都是这个数据结构
{
	struct tm  dwTime;                  //utc时间 包括日期时间。 这个时间与真实时间差1900；比如: dwTime.tm_year = 111;代表111+1900 = 2011年 (用这个结构，还不如用dword，后面开发麻烦了好多)
	double fOpenPrice;				// 开盘价
	double fHighPrice;				// 最高价
	double fLowPrice;				// 最低价
	double fClosePrice;				// 收盘价
	double fSumTradeVolume;			//成交金额
	DWORD dwVolume;					//成交量
	DWORD dwHoldVolume;				//持仓量
	double dJieSuan;                //结算价
	SKLine& operator=(SKLine& other)
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
	friend ofstream & operator<<(ofstream &os,  SKLine* kLine)
	{	
		os.write((char*)&kLine->dwTime,sizeof(kLine->dwTime));	
		os.write((char*)&kLine->fOpenPrice,sizeof(kLine->fOpenPrice));	
		os.write((char*)&kLine->fHighPrice,sizeof(kLine->fHighPrice));	
		os.write((char*)&kLine->fLowPrice,sizeof(kLine->fLowPrice));	
		os.write((char*)&kLine->fClosePrice,sizeof(kLine->fClosePrice));	
		os.write((char*)&kLine->fSumTradeVolume,sizeof(kLine->fSumTradeVolume));	
		os.write((char*)&kLine->dwVolume,sizeof(kLine->dwVolume));	
		os.write((char*)&kLine->dwHoldVolume,sizeof(kLine->dwHoldVolume));	
		os.write((char*)&kLine->dJieSuan,sizeof(kLine->dJieSuan));	

		return os;
	}

	friend ifstream & operator>>(ifstream &is,  SKLine*& kLine)
	{	
		is.read((char*)&kLine->dwTime,sizeof(kLine->dwTime));	
		is.read((char*)&kLine->fOpenPrice,sizeof(kLine->fOpenPrice));	
		is.read((char*)&kLine->fHighPrice,sizeof(kLine->fHighPrice));	
		is.read((char*)&kLine->fLowPrice,sizeof(kLine->fLowPrice));	
		is.read((char*)&kLine->fClosePrice,sizeof(kLine->fClosePrice));	
		is.read((char*)&kLine->fSumTradeVolume,sizeof(kLine->fSumTradeVolume));	
		is.read((char*)&kLine->dwVolume,sizeof(kLine->dwVolume));	
		is.read((char*)&kLine->dwHoldVolume,sizeof(kLine->dwHoldVolume));	
		is.read((char*)&kLine->dJieSuan,sizeof(kLine->dJieSuan));	
	
		return is;
	}
} ;	// K线图数据结构  

enum EnumMark
{
	MARK_UNDEFINE,		//未定义
	MARK_TRIANGLE_UP,	//上三角形
	MARK_TRIANGLE_DOWN, //下三角形
	
};
struct SPoint
{
	int  enumType;
	//int nIndex;            //在k线数组中的序号，每次调整的时候都需要重新调整
	bool bSelected;        //是否被选中，删除用
	struct tm dwTime;
	double dwPrice;
	SPoint()
	{
		bSelected = false;
	}
	friend ofstream & operator<<(ofstream &os,  SPoint* sPoint)
	{	
		os.write((char*)&sPoint->enumType,sizeof(sPoint->enumType));	
	//	os.write((char*)&sPoint->nIndex,sizeof(sPoint->nIndex));	
		os.write((char*)&sPoint->bSelected,sizeof(sPoint->bSelected));	
		os.write((char*)&sPoint->dwTime,sizeof(sPoint->dwTime));	
		os.write((char*)&sPoint->dwPrice,sizeof(sPoint->dwPrice));	
		return os;
	}
	friend ifstream & operator>>(ifstream &is,  SPoint* sPoint)
	{	
		is.read((char*)&sPoint->enumType,sizeof(sPoint->enumType));	
	//	is.read((char*)&sPoint->nIndex,sizeof(sPoint->nIndex));	
		is.read((char*)&sPoint->bSelected,sizeof(sPoint->bSelected));	
		is.read((char*)&sPoint->dwTime,sizeof(sPoint->dwTime));	
		is.read((char*)&sPoint->dwPrice,sizeof(sPoint->dwPrice));	
		return is;
	}
};
struct SMark
{		
	vector<SPoint *> vecPt;//所有点的数组
	string strMark; //指标描述

	friend ofstream & operator<<(ofstream &os,  SMark* sMark)
	{			
		int nSize = sMark->vecPt.size();
		os.write((char*)&nSize,sizeof(nSize));	
		for(vector<SPoint *>::iterator it = sMark->vecPt.begin(); it != sMark->vecPt.end(); it++)
		{
			SPoint *p = *it;
			os<<p;
		}
		os.write((char*)&sMark->strMark,sizeof(sMark->strMark));	
		return os;
	}
	friend ifstream & operator>>(ifstream &is, SMark* sMark)
	{	
		int nCount =0;
		is.read((char*)&nCount,sizeof(nCount));	
		for(int i=0; i< nCount; i++)
		{
			SPoint *p = new SPoint;
			is>>p;
			sMark->vecPt.push_back(p);
		}
		is.read((char*)&sMark->strMark,sizeof(sMark->strMark));	
		return is;
	}
};
typedef map<int, SMark *>	MAPsmark;

struct SItem
{
	int nBegin;
	int nEnd;
	int nItem;
	int	nShowCount;
	int nDayWidth;
	int nDaySpan;
	SKLine* pItem;
	MSG* pMsg; 
	bool bIsMouse;   //鼠标消息
	bool bIsKeyBoard;//键盘消息
	wxPoint wxPt;    //鼠标当前位置，比如在k线内移动，需要告诉量能现在的鼠标移动位置（x轴是一样的）
	SItem()
	{
		nItem = -1;
		nShowCount = 100;
		nDayWidth = 0;
		nDaySpan = 0;
		pItem = NULL;
		pMsg  = NULL;
		bIsKeyBoard = false;
		bIsMouse    = false;	
		wxPt = wxPoint(-1,-1);
	}
	SItem& operator = (SItem & other)
	{
		if(this == & other)
			return *this;
		nBegin = other.nBegin;
		nEnd   = other.nEnd;
		nItem  = other.nItem;
		nShowCount = other.nShowCount;
		nDayWidth = other.nDayWidth;
		nDaySpan  = other.nDaySpan;
		pItem     = other.pItem;
		pMsg      = other.pMsg;
		bIsKeyBoard = other.bIsMouse;
		bIsMouse    = other.bIsMouse;
		wxPt		= other.wxPt;
	}
};

struct STLine //分时数据结构
{
	struct tm  dwTime;               //时间
	double fLastPrice;				 //价格
	DWORD dwVolume;					 //成交量
	double dwVolumeMoney;			 //成交金额
	DWORD dwHoldVolume;				 //持仓量
	
	friend ofstream & operator<<(ofstream &os,  STLine* tLine)
	{	
		os.write((char*)&tLine->dwTime,sizeof(tLine->dwTime));	
		os.write((char*)&tLine->fLastPrice,sizeof(tLine->fLastPrice));	
		os.write((char*)&tLine->dwVolume,sizeof(tLine->dwVolume));	
		os.write((char*)&tLine->dwVolumeMoney,sizeof(tLine->dwVolumeMoney));	
		os.write((char*)&tLine->dwHoldVolume,sizeof(tLine->dwHoldVolume));
		return os;
	}

	friend ifstream & operator>>(ifstream &is,  STLine*& tLine)
	{	
		is.read((char*)&tLine->dwTime,sizeof(tLine->dwTime));	
		is.read((char*)&tLine->fLastPrice,sizeof(tLine->fLastPrice));	
		is.read((char*)&tLine->dwVolume,sizeof(tLine->dwVolume));	
		is.read((char*)&tLine->dwVolumeMoney,sizeof(tLine->dwVolumeMoney));	
		is.read((char*)&tLine->dwHoldVolume,sizeof(tLine->dwHoldVolume));	
		return is;
	}
	STLine& operator=(STLine& other)
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
};	// 分时图数据结构
struct STItem
{
	int nItem;
	STLine* pItem;
	MSG* pMsg; 
	bool bIsMouse;   //鼠标消息
	bool bIsKeyBoard;//键盘消息
	wxPoint wxPt;    //鼠标当前位置，比如在k线内移动，需要告诉量能现在的鼠标移动位置（x轴是一样的）
	STItem()
	{
		nItem = -1;
		pItem = NULL;
		pMsg  = NULL;
		bIsKeyBoard = false;
		bIsMouse    = false;	
		wxPt = wxPoint(-1,-1);
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
struct SQIntime//实时行情数据
{
	struct tm	dwTime;							//时间
	double	fLastPrice;						    //价格
	DWORD	dwVolume;						    //成交量
	DWORD	dwOpenVolume;					    //开量
	DWORD	dwCloseVolume;					    //平量
	//string  strDesc;						    //多，空，平，开
	int     nDesc;                              //0:空开; 1:多开; 2:多平; 3:空平; 4:多换; 5:空换;6;双平；7双开

	friend ofstream & operator<<(ofstream &os,  SQIntime* tLine)
	{	
		os.write((char*)&tLine->dwTime,sizeof(tLine->dwTime));	
		os.write((char*)&tLine->fLastPrice,sizeof(tLine->fLastPrice));	
		os.write((char*)&tLine->dwVolume,sizeof(tLine->dwVolume));	
		os.write((char*)&tLine->dwOpenVolume,sizeof(tLine->dwOpenVolume));	
		os.write((char*)&tLine->dwCloseVolume,sizeof(tLine->dwCloseVolume));
		os.write((char*)&tLine->nDesc,sizeof(tLine->nDesc));
		return os;
	}

	friend ifstream & operator>>(ifstream &is,  SQIntime*& tLine)
	{	
		is.read((char*)&tLine->dwTime,sizeof(tLine->dwTime));	
		is.read((char*)&tLine->fLastPrice,sizeof(tLine->fLastPrice));	
		is.read((char*)&tLine->dwVolume,sizeof(tLine->dwVolume));	
		is.read((char*)&tLine->dwOpenVolume,sizeof(tLine->dwOpenVolume));	
		is.read((char*)&tLine->dwCloseVolume,sizeof(tLine->dwCloseVolume));	
		is.read((char*)&tLine->nDesc,sizeof(tLine->nDesc));	
		return is;
	}
};	
struct TLineDayInfo 
{//某一天是否显示
	wxString  strTime;
	bool bShow;
	TLineDayInfo()
	{
		bShow = false;
	}
};
struct SQuotation//实时行情数据
{
	DWORD	dwTime;							//时间
	double	fLastPrice;						//价格
	DWORD	dwVolume;						//成交量
	DWORD	dwHoldVolume;					//持仓量
};	// K线图数据结构

struct SSecondData //秒数据
{
	DWORD    dwMinute;		//几点几分几秒
	double    fVolume;		//成交量
	double    fPrice;		//成交价格
};


inline time_t stingToTime(char * szTime)
{
	struct tm tm1;
	time_t time1;
	sscanf(szTime,"%4d-%2d-%2d %2d:%2d:%2d",
		&tm1.tm_year,
		&tm1.tm_mon,
		&tm1.tm_mday,
		&tm1.tm_hour,
		&tm1.tm_min,
		&tm1.tm_sec);             
	tm1.tm_year -= 1900;
	tm1.tm_mon --;
	tm1.tm_isdst=-1;  
	time1 = mktime(&tm1);
	return time1;
}
inline void timeToSting(time_t time1, char *szTime)
{
	struct tm tm1;
#ifdef WIN32
	tm1 = *localtime(&time1);
#else
	localtime_r(time1, tm1 );
#endif
	sprintf( szTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
		tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
		tm1.tm_hour, tm1.tm_min,tm1.tm_sec);
}
inline int getExpoBase2(const double& d)
{
	int i = 0;
	((short *)(&i))[0] = (((short *)(&d))[3] & (short)0x7ff0);
	return (i >> 4) - 1023;
}

inline bool	equals(const double& d1, const double& d2)
{
	if (d1 == d2)
		return true;
	int e1 = getExpoBase2(d1);
	int e2 = getExpoBase2(d2);
	int e3 = getExpoBase2(d1 - d2);
	if ((e3 - e2 < -48) && (e3 - e1 < -48))
		return true;
	return false;
}
inline bool isInvalidValue(const double& d)
{
	return equals(fabs(d), 1.7976931348623158e+308);
}
inline bool isInvalidValue(const int& d)
{
	return d==0xfeeefeee || d==0xffffffff;
}
inline bool IsDoubleSame(double db1, double db2)
{
	if(db1 - db2 > -0.00001 && db1 - db2 < 0.00001)
		return true;
	return false;
}
inline void split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}
inline int GetNumDigits(double dbTick)
{
	int pos,nNumDigits=0;
	char strPriceText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	sprintf_s(strPriceText,512, "%f", dbTick);
	for(pos=strlen(strPriceText)-1;pos>=0;pos--)
	{
		if(strPriceText[pos]=='0')
			strPriceText[pos]=0;
		else
			break;
	}
	for(pos=strlen(strPriceText)-1;pos>=0;pos--)
	{
		if(strPriceText[pos]!='.')
			nNumDigits++;
		else
			break;
	}
	return nNumDigits;

}
#define WRITELOG  0

inline  void  WriteLog(std::string szXns)
{
	long dwID = ::GetCurrentProcessId();
	wxString strFile;
	strFile.Printf(wxT("c:\\HQTrace\\hq_%d.txt"), dwID);
	HFILE hF =_lopen(strFile.c_str(),OF_READWRITE);
	if(hF==HFILE_ERROR)
	{
		wxString strFile2 = wxT("c:\\HQTrace\\");
		CreateDirectory(strFile2.c_str(), NULL);
		hF=_lcreat(strFile.c_str(),0);	
	}
	_llseek(hF,0,2);
	_lwrite(hF,szXns.c_str(),szXns.length());	
	_lwrite(hF,"\r\n",2);
	_lclose(hF);
}


inline void AppendLog(SKLine *pKline, wxString strExplain)
{
	return;
	wxString str;
	str.Printf(wxT("SKLine:%d-%d-%d %d:%d:%d, %.2f, %.2f, %.2f, %.2f, %u, %u, %.2f, %.2f, %s"),
				
				pKline->dwTime.tm_year+1900,
				pKline->dwTime.tm_mon+1,
				pKline->dwTime.tm_mday,
				pKline->dwTime.tm_hour,
				pKline->dwTime.tm_min,
				pKline->dwTime.tm_sec,
				pKline->fOpenPrice,
				pKline->fHighPrice,
				pKline->fLowPrice,
				pKline->fClosePrice,
				pKline->dwVolume,
				pKline->dwHoldVolume,
				pKline->fSumTradeVolume,
				pKline->dJieSuan,
				strExplain
		);
	WriteLog(str.c_str());
}
inline void AppendLog(STLine *pTline, wxString strExplain)
{
	wxString str;
	str.Printf(wxT("STLine:%d-%d-%d %d:%d:%d, %.2f, %u, %.2f, %u, %s"),
		
		pTline->dwTime.tm_year+1900,
		pTline->dwTime.tm_mon+1,
		pTline->dwTime.tm_mday,
		pTline->dwTime.tm_hour,
		pTline->dwTime.tm_min,
		pTline->dwTime.tm_sec,
		pTline->fLastPrice,
		
		pTline->dwVolume,

		pTline->dwVolumeMoney,
		pTline->dwHoldVolume,
		strExplain
		);
	WriteLog(str.c_str());
}

inline  void  WriteLog_newT(std::string szXns)
{
	long dwID = ::GetCurrentProcessId();
	wxString strFile;
	strFile.Printf(wxT("c:\\HQTrace\\hq_%d_newT.txt"), dwID);
	HFILE hF =_lopen(strFile.c_str(),OF_READWRITE);
	if(hF==HFILE_ERROR)
	{
		wxString strFile2 = wxT("c:\\HQTrace\\");
		CreateDirectory(strFile2.c_str(), NULL);
		hF=_lcreat(strFile.c_str(),0);	
	}
	_llseek(hF,0,2);
	_lwrite(hF,szXns.c_str(),szXns.length());	
	_lwrite(hF,"\r\n",2);
	_lclose(hF);
}

inline  void  WriteLog_newK(std::string szXns)
{
	long dwID = ::GetCurrentProcessId();
	wxString strFile;
	strFile.Printf(wxT("c:\\HQTrace\\hq_%d_newK.txt"), dwID);
	HFILE hF =_lopen(strFile.c_str(),OF_READWRITE);
	if(hF==HFILE_ERROR)
	{
		wxString strFile2 = wxT("c:\\HQTrace\\");
		CreateDirectory(strFile2.c_str(), NULL);
		hF=_lcreat(strFile.c_str(),0);	
	}
	_llseek(hF,0,2);
	_lwrite(hF,szXns.c_str(),szXns.length());	
	_lwrite(hF,"\r\n",2);
	_lclose(hF);
}

inline void AppendLog_newK(SKLine *pKline)
{
	wxString str;
	str.Printf(wxT("SKLine %d %d %d %d %d %d %.2f %.2f %.2f %.2f %u %u %2f %.2f"),
		pKline->dwTime.tm_year+1900,
		pKline->dwTime.tm_mon+1,
		pKline->dwTime.tm_mday,
		pKline->dwTime.tm_hour,
		pKline->dwTime.tm_min,
		pKline->dwTime.tm_sec,
		pKline->fOpenPrice,
		pKline->fHighPrice,
		pKline->fLowPrice,
		pKline->fClosePrice,
		pKline->dwVolume,
		pKline->dwHoldVolume,
		pKline->fSumTradeVolume,
		pKline->dJieSuan
		);
	WriteLog_newK(str.c_str());
}
inline void AppendLog_newT(STLine *pTline)
{
	wxString str;
	str.Printf(wxT("STLine %d %d %d %d %d %d %.2f %u %u %u"),
		pTline->dwTime.tm_year+1900,
		pTline->dwTime.tm_mon+1,
		pTline->dwTime.tm_mday,
		pTline->dwTime.tm_hour,
		pTline->dwTime.tm_min,
		pTline->dwTime.tm_sec,
		pTline->fLastPrice,

		pTline->dwVolume,
		pTline->dwVolumeMoney,
		pTline->dwHoldVolume	
		);
	WriteLog_newT(str.c_str());
}

inline  void  WriteLog_newFutureID(std::string szXns)
{
	long dwID = ::GetCurrentProcessId();
	wxString strFile;
	strFile.Printf(wxT("c:\\HQTrace\\hq_%d_newFutureID.txt"), dwID);
	HFILE hF =_lopen(strFile.c_str(),OF_READWRITE);
	if(hF==HFILE_ERROR)
	{
		wxString strFile2 = wxT("c:\\HQTrace\\");
		CreateDirectory(strFile2.c_str(), NULL);
		hF=_lcreat(strFile.c_str(),0);	
	}
	_llseek(hF,0,2);
	_lwrite(hF,szXns.c_str(),szXns.length());	
	_lwrite(hF,"\r\n",2);
	_lclose(hF);
}

inline  void  WriteLog_DepthMarketData(std::string szXns)
{
	long dwID = ::GetCurrentProcessId();
	wxString strFile;
	strFile.Printf(wxT("c:\\HQTrace\\hq_%d_DepthMarketData.txt"), dwID);
	HFILE hF =_lopen(strFile.c_str(),OF_READWRITE);
	if(hF==HFILE_ERROR)
	{
		wxString strFile2 = wxT("c:\\HQTrace\\");
		CreateDirectory(strFile2.c_str(), NULL);
		hF=_lcreat(strFile.c_str(),0);	
	}
	_llseek(hF,0,2);
	_lwrite(hF,szXns.c_str(),szXns.length());	
	_lwrite(hF,"\r\n",2);
	_lclose(hF);
}
inline void AppendLog_DepthMarketData(PlatformStru_DepthMarketData* pData)
{
	
	wxString str;
	str.Printf(wxT("%s %s %s %.2f %.2f %.2f %.2f %.2f %8d %.2f %.2f %7d %.2f %7d %.0f"), 
		pData->TradingDay, 
		pData->UpdateTime, 
		pData->InstrumentID,
		pData->PreSettlementPrice,
		pData->LastPrice,
		pData->OpenPrice,
		pData->HighestPrice,
		pData->LowestPrice,	
		pData->Volume,
		pData->Turnover,
		pData->AskPrice1,
		pData->AskVolume1,
		pData->BidPrice1,
		pData->BidVolume1,
		pData->OpenInterest		
		);
	WriteLog_DepthMarketData(str.c_str());
	if(str.Length() > 200)
	{
		wxMessageBox(str,"提示",wxYES_NO);		
	}
}
