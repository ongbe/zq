#pragma once

#ifndef SVRFUTURESPROCESS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRQUOTSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRFUTURESPROCESS_EXPORTS
#define SVRFUTURESPROCESS_API __declspec(dllexport)
#else
#define SVRFUTURESPROCESS_API __declspec(dllimport)
#endif
#endif

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#pragma warning(disable : 4101)


#include <queue>
#include <map>
#include <vector>
using namespace std;

#include "CommonPkg.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "MyXML.h"
#include "easymutex.h"
#include "Tools_Ansi.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")	
#pragma comment(lib, "SvrDBOpr.lib")										
#else 
#pragma comment(lib, "Tools4dll.lib")	
#pragma comment(lib, "SvrDBOpr.lib")
#endif

#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")

//行情结构体
struct STU
{
	char snum[10];
	char sname[10];
	char ssex[2];
	int  sage;
	char smajor[20];
};
//KLine结构体
struct KLine
{
	string		szExchange;
	string		szInstrumentID;
	EnumPhrase	ePhrase;
	KLineBase	sKLine;
};


void	LoadConfig(void);
bool	GetQuotFromQueue(PlatformStru_DepthMarketData& outData);
bool	GetKLineFromQueue(vector<KLine>& vData);
bool	RecalcWeekMonthKLine(void);
void	SetDataInTime(PlatformStru_DepthMarketData* pData);
bool	SetInTimeData_KLine(PlatformStru_DepthMarketData* pData);
bool	SetInTimeData_DiffSpan(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,string exchangeID);
bool	SetInTimeData_MINS(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine, int nTimeSpan ,vector<struct tm>& vecTimeScales);
bool    SetInTimeData_Day(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
bool	SetInTimeData_Week(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
bool	SetInTimeData_Month(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);

bool	SaveKline2Cache(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase);
bool	SaveKline2DB(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,string& exchangeID);

bool	UpdateKLineData(string& strInstrumentID, EnumPhrase phrase, KLineBase& pKLine, bool bInsert, int& nErrorCode);

string MakeTableName(string& strInstrumentID, EnumPhrase phrase);

struct	tm GetTimeInSpan(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales);
bool	IsOneWeek(struct tm tm1, struct tm tm2);
bool	IsOneMonth(struct tm tm1, struct tm tm2);
int		GetDayOfYear(struct tm tm1);


//读取SQLServer DB 线程 
HANDLE	hReadSQLDBThread=NULL;	
DWORD	idReadSQLDBThread=0;
DWORD	ReadSQLDBThreadProc(void);

//计算KLine线程
HANDLE	hCaculateKLineThread=NULL;	
DWORD	idCaculateKLineThread=0;
DWORD	CaculateKLineThreadProc(void);

//将K线数据本地库线程
HANDLE	hSaveDBThread=NULL;	
DWORD	idSaveDBThread=0;
DWORD	SaveDBThreadProc(void);

//三个线程退出标识
bool	bQuit = false;

//互斥锁 分别锁两个队列
Ceasymutex	mutexQuot;
Ceasymutex	mutexKLine;
Ceasymutex  m_mutexKLineM1,m_mutexKLineM5,m_mutexKLineM15,m_mutexKLineM30,m_mutexKLineM60,m_mutexKLineDay,m_mutexKLineWeek,m_mutexKLineMonth;

//两个队列分别存储查询的行情和计算后的K线
queue<PlatformStru_DepthMarketData>			queueQuot;
queue<KLine>								queueKLine;

//K线
string		m_strTradingDay;		//保存交易日
int			m_nTimeSpan;			//日，周、月K线存储间隔
struct		tm m_tmCloseTime;
struct		tm m_LastDBTimeDay;
struct		tm m_LastDBTimeWeek;
struct		tm m_LastDBTimeMonth;

set<string>							m_setTableName;		// 已创建KLine存储表的名称

map<string,vector<struct tm>>		m_mapExchangeTime;
map<string,KLineBase>				m_mapKLineLast;

map<string,KLineBase >				m_maptmLast1;	//上一个K线
map<string,KLineBase >				m_maptmLast5;
map<string,KLineBase >				m_maptmLast15;
map<string,KLineBase >				m_maptmLast30;
map<string,KLineBase >				m_maptmLast60;
map<string,KLineBase >				m_maptmLastDay;
map<string,KLineBase >				m_maptmLastWeek;
map<string,KLineBase >				m_maptmLastMonth;


map<string,vector<KLineBase>>		m_mapKLineMin1;		//所有合约1分钟K线
map<string,vector<KLineBase>>		m_mapKLineMin5;		//所有合约5分钟K线
map<string,vector<KLineBase>>		m_mapKLineMin15;	//所有合约15分钟K线
map<string,vector<KLineBase>>		m_mapKLineMin30;	//所有合约30分钟K线
map<string,vector<KLineBase>>		m_mapKLineMin60;	//所有合约60分钟K线
map<string,vector<KLineBase>>		m_mapKLineDay;		//所有合约日K线
map<string,vector<KLineBase>>		m_mapKLineWeek;		//所有合约周K线
map<string,vector<KLineBase>>		m_mapKLineMonth;	//所有合约月K线

map<string,map<time_t,KLineBase>>		m_vmapKLineMin1;	//所有合约1分钟K线
map<string,map<time_t,KLineBase>>		m_vmapKLineMin5;	//所有合约5分钟K线
map<string,map<time_t,KLineBase>>		m_vmapKLineMin15;	//所有合约15分钟K线
map<string,map<time_t,KLineBase>>		m_vmapKLineMin30;	//所有合约30分钟K线
map<string,map<time_t,KLineBase>>		m_vmapKLineMin60;	//所有合约60分钟K线
map<string,map<time_t,KLineBase>>		m_vmapKLineDay;		//所有合约日K线
map<string,map<time_t,KLineBase>>		m_vmapKLineWeek;	//所有合约周K线
map<string,map<time_t,KLineBase>>		m_vmapKLineMonth;	//所有合约月K线


