// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#include "OrderFunc.h"
#include "KernelStruct.h"

//报价表
enum QuotTable
{
	QUOT_InstrumentID,
	QUOT_LastPrice,
	//QUOT_ChangeValue,
	//QUOT_ChangePercentage,
	QUOT_BidPrice1,
	//QUOT_BidVolume1,
	QUOT_AskPrice1,
	//QUOT_AskVolume1,
	//QUOT_OpenInterest,
	//QUOT_OpenPrice,
	//QUOT_PreSettlementPrice,
	QUOT_LastVolume,
	//QUOT_HighestPrice,
	//QUOT_LowestPrice,
	//QUOT_ClosePrice,
	QUOT_Volume,
	//QUOT_Turnover,
	//QUOT_UpperLimitPrice,
	//QUOT_LowerLimitPrice,
	//QUOT_SettlementPrice,
	//QUOT_PreClosePrice,
	//QUOT_PreOpenInterest,
	//QUOT_UpdateTime,
	QUOT_KLineView,
	QUOT_COUNT,						// 行情字段数量
	QUOT_OptionPrice,				// 期权执行价
	QUOT_HEAD_REVERSE = 100,		// 反向编号开始
};

#define QUOTNEWHEAD QUOT_HEAD_REVERSE


typedef struct {
	int nSeqNum;
	int nCmd;
	int nErrorCode;
	int nDataLen;
	void* pData;
} ITFCMDATA, *PITFCMDATA;


BOOL LockGlobalObject();
BOOL UnlockGlobalObject();
void ReleaseGlobalObject();

UINT GetColSetChangedMSGID();

std::map<std::string, PlatformStru_InstrumentInfo>& GetInstrumentInfos();

void ParseOptionInstrument(std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument);
std::map<std::string, std::map<std::string, std::set<std::string>>>& GetOptionTargetRelate();
BOOL FindTargetID(const CString& strOptionID, CString& strTargetID); 

void InitGridMap();
// 设置确实的显示列
void ResetDefaultShowCol();
const std::map<int, std::string>& GetTargetColMap();
const std::map<int, std::string>& GetOptionColMap();

const std::vector<int>& GetTargetShowCol();
void SetTargetShowCol(const std::vector<int>& vecCol);
const std::vector<int>& GetOptionShowCol();
void SetOptionShowCol(const std::vector<int>& vecCol);

CString GetIniFile();
CString GetXmlFile();

void SaveConfig();
void LoadConfig();
