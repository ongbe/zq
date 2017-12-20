// stdafx.cpp : source file that includes just the standard includes
// StrategyTester.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <windows.h>

UINT g_nDataRecvMsgID = ::RegisterWindowMessage("");

UINT _GetDataRecvMsgID()
{
	return g_nDataRecvMsgID;
}

UINT g_nEventMsgID = ::RegisterWindowMessage("");

UINT _GetEventMsgID()
{
	return g_nEventMsgID;
}

std::string g_strIniFile;

void _SetIniFile(const std::string& strFile)
{
	g_strIniFile = strFile;
}

std::string _GetIniFile()
{
	return g_strIniFile;
}

std::string g_strStategyPath;

void _SetStategyPath(const std::string& strPath)
{
	g_strStategyPath = strPath;
}

std::string _GetStategyPath()
{
	return g_strStategyPath;
}

std::string g_strRunPath;

void _SetRunPath(const std::string& strPath)
{
	g_strRunPath = strPath;
}

std::string _GetRunPath()
{
	return g_strRunPath;
}

std::string g_strHistoryDataPath;

void _SetHistoryDataPath(const std::string& strPath)
{
	g_strHistoryDataPath = strPath;
}

std::string _GetHistoryDataPath()
{
	return g_strHistoryDataPath;
}

std::string g_strStrategyServerAddr;

void _SetStrategyServerAddr(const std::string& strAddr)
{
	g_strStrategyServerAddr = strAddr;
}

std::string _GetStrategyServer()
{
	return g_strStrategyServerAddr;
}

std::string g_strQuotServerAddr;

void _SetQuotServerAddr(const std::string& strAddr)
{
	g_strQuotServerAddr = strAddr;
}

std::string _GetQuotServer()
{
	return g_strQuotServerAddr;
}

std::string g_strRedisServerAddr;

void _SetRedisServerAddr(const std::string& strAddr)
{
	g_strRedisServerAddr = strAddr;
}

std::string _GetRedisServer()
{
	return g_strRedisServerAddr;
}

DWORD g_dwStrategyTestDelay = 0;

void _SetStrategyTestDelay(DWORD dwDelay)
{
	g_dwStrategyTestDelay = dwDelay;
}

DWORD _GetStrategyTestDelay()
{
	return g_dwStrategyTestDelay;
}

std::string g_strStrategyID;

void _SetStrategyID(const std::string& strID)
{
	g_strStrategyID = strID;
}

std::string _GetStrategyID()
{
	return g_strStrategyID;
}

std::string g_strCurrUserID;

void _SetCurrUserID(const std::string& strID)
{
	g_strCurrUserID = strID;
}

std::string _GetCurrUserID()
{
	return g_strCurrUserID;
}

std::string g_strPassword;

void _SetPassword(const std::string& strPasswd)
{
	g_strPassword = strPasswd;
}

std::string _GetPassword()
{
	return g_strPassword;
}

CStrategyTraderApi* g_pTraderApi = CStrategyTraderApi::CreateStrategyTraderApi();

CStrategyTraderApi* _GetTraderApi() 
{
	return g_pTraderApi;
}

void _ReleaseTraderApi()
{
	g_pTraderApi->Release();
	//delete g_pTraderApi;
	g_pTraderApi = NULL;
}

CStrategyMdApi* g_pMDApi = CStrategyMdApi::CreateStrategyMdApi();

CStrategyMdApi* _GetMDApi() 
{
	return g_pMDApi;
}

void _ReleaseMDApi()
{
	g_pMDApi->Release();
	//delete g_pMDApi;
	g_pMDApi = NULL;
}

double _ConvDouble(const char* pText)
{
	if(pText==NULL)
		return util::GetDoubleInvalidValue();
	else
	return atof(pText) > 1E+100 ? util::GetDoubleInvalidValue() : atof(pText);
}

bool _ConvToQuot(const std::string& strQuot, PlatformStru_DepthMarketData& ret)
{
	if(strQuot.size()==0)
		return false;
	std::vector<std::string> vecItem;
    int startpos=0,endpos=0;
	std::string strTrueLine=strQuot+",";
    while(true) {
        endpos=strTrueLine.find(',',startpos);
        if(endpos<0) 
            break;
		vecItem.push_back(strTrueLine.substr(startpos,endpos-startpos));
        startpos=endpos+1;   
    }
	if(vecItem.size()<64) 
		return false;

    int index=0;

	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.TradingDay,vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.InstrumentID,vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.ExchangeID,vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.ExchangeInstID,vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.LastPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.PreSettlementPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.PreClosePrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.PreOpenInterest = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.OpenPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.HighestPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.LowestPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.Volume = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.Turnover = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.OpenInterest = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.ClosePrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.SettlementPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.UpperLimitPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.LowerLimitPrice = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.PreDelta = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.CurrDelta = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	strcpy(ret.UpdateTime,vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.UpdateMillisec = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidPrice1 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidVolume1 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskPrice1 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskVolume1 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidPrice2 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidVolume2 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskPrice2 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskVolume2 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidPrice3 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidVolume3 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskPrice3 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskVolume3 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidPrice4 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidVolume4 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskPrice4 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskVolume4 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidPrice5 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.BidVolume5 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskPrice5 = _ConvDouble(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AskVolume5 = atoi(vecItem[index++].c_str());
	//Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
	ret.AveragePrice = _ConvDouble(vecItem[index++].c_str());
//bidprice6,vecItem[index++].c_str());
//bidvolume6,vecItem[index++].c_str());
//askprice6,vecItem[index++].c_str());
//askvolume6,vecItem[index++].c_str());
//bidprice7,vecItem[index++].c_str());
//bidvolume7,vecItem[index++].c_str());
//askprice7,vecItem[index++].c_str());
//askvolume7,vecItem[index++].c_str());
//bidprice8,vecItem[index++].c_str());
//bidvolume8,vecItem[index++].c_str());
//askprice8,vecItem[index++].c_str());
//askvolume8,vecItem[index++].c_str());
//bidprice9,vecItem[index++].c_str());
//bidvolume9,vecItem[index++].c_str());
//askprice9,vecItem[index++].c_str());
//askvolume9,vecItem[index++].c_str());
//bidprice10,vecItem[index++].c_str());
//bidvolume10,vecItem[index++].c_str());
//askprice10,vecItem[index++].c_str());
//askvolume10,vecItem[index++].c_str());
//newvolume,vecItem[index++].c_str());
	//Debug::WriteLine("");
	return true;
}

std::string g_strQuotFile;

void _SetQuotFile(const std::string& strFile)
{
	g_strQuotFile = strFile;
}

const std::string& _GetQuotFile()
{
	return g_strQuotFile;
}

LPVOID g_lpMsChart = NULL;

void _SetMsChartFormHandle(LPVOID lpForm)
{
	g_lpMsChart = lpForm;
}

LPVOID _GetMsChartFormHandle()
{
	return g_lpMsChart;
}
