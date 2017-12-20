// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// TODO: reference additional headers your program requires here


#include <string>
#include <vector>
#include <map>
#include <set>
#include <winsock2.h>
#include <Windows.h>
#include "Tools.h"
#include "StrategyMDApi.h"
#include "StrategyTraderApi.h"

#define WndUserCmd_YourPkgArrival 0x3bcb
#define TradeRule_LastPrice    1
#define TradeRule_AskBidPrice  2
#define TradeRule_AskBidVolume 3

//事件到来后，通知本身线程的消息ID 定义
#define TMESSAGE_START_SUBCRIBE_CACHE_CHANNEL 0x0408

//事件到来后，通知本身线程的消息ID 定义
#define TMESSAGE_START_DEAL_CACHE_CHANNEL_COMMAND 0x0409

extern DWORD				g_idThreadSubcribe;

typedef struct 
{
	bool				bLocalOutput;			//本地导出
	bool				bSaveLog;				//记录日志
	bool				bSaveHistory;			//记录历史

	double				fInitFee;				//初始资金
	int					nTradeRule;				// 0 最新价见价成交, 1 买卖价见价成交, 2 买卖价按买卖量成交

	bool				bStartTimer;			//是否启动定时器
	int					nTimerSpan;				//定时器时间间隔
} StrategyTestParams;

UINT _GetDataRecvMsgID();
UINT _GetEventMsgID();

void _SetStrategyServerAddr(const std::string& strAddr);
std::string _GetStrategyServer();
void _SetQuotServerAddr(const std::string& strAddr);
std::string _GetQuotServer();
void _SetRedisServerAddr(const std::string& strAddr);
std::string _GetRedisServer();

void _SetStrategyTestDelay(DWORD dwDelay);
DWORD _GetStrategyTestDelay();

void _SetIniFile(const std::string& strFile);
std::string _GetIniFile();

void _SetStategyPath(const std::string& strPath);
std::string _GetStategyPath();
void _SetRunPath(const std::string& strPath);
std::string _GetRunPath();
void _SetHistoryDataPath(const std::string& strPath);
std::string _GetHistoryDataPath();

void _SetStrategyID(const std::string& strID);
std::string _GetStrategyID();
void _SetCurrUserID(const std::string& strID);
std::string _GetCurrUserID();
void _SetPassword(const std::string& strPasswd);
std::string _GetPassword();

CStrategyTraderApi* _GetTraderApi();
void _ReleaseTraderApi();
CStrategyMdApi* _GetMDApi();
void _ReleaseMDApi();

bool _ConvToQuot(const std::string& strQuot, PlatformStru_DepthMarketData& ret);

void _SetQuotFile(const std::string& strFile);
const std::string& _GetQuotFile();

void _SetMsChartFormHandle(LPVOID lpForm);
LPVOID _GetMsChartFormHandle();
