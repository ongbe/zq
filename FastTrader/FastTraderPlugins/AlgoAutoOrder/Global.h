// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#include "PlusinCommon/PlusinAPI.h"
#include "SequenceManager.h"
#include "ChangeOrderManager.h"
#include "OrderFunc.h"
#include "ViewStyle.h"


typedef struct {
	int nSeqNum;
	int nCmd;
	int nErrorCode;
	int nDataLen;
	void* pData;
} ITFCMDATA, *PITFCMDATA;


typedef struct {
	BOOL	bHasData;
	BOOL	bFixedPrice;
	int		nGridRowCount;
	int		nLastPriceRowIndex;
	double	fTopPrice;
	double	fLastPrice;
	double	fPriceTick;
} PRICEGRIDINDEX, *PPRICEGRIDINDEX;


BOOL LockGlobalObject();
BOOL UnlockGlobalObject();
void ReleaseGlobalObject();

BOOL LockObjectOverTime();
BOOL UnLockObjectOverTime();

BOOL LockObjectSingle();
BOOL UnLockObjectSingle();

BOOL GetInitStatus();
void SetInitStatus(BOOL bIsSuccess);

void SetTargetHWnd(HWND hTargetWnd);
HWND GetTargetHWnd();

void SetPlusinAPI(PlusinAPI* poPlusinApi);
PlusinAPI* GetPlusinAPI();

UINT GetBookOrderCloseMSGID();

void SetIsConfirmRaskInfo(BOOL bConfirm);
BOOL GetIsConfirmRaskInfo();

void SetProductShortNames(CString& strProductShortNames);
CString& GetProductShortNames();

void SetExchangeIDList(std::set<std::string>& setExchangeID);
std::set<std::string>& GetExchangeIDList();

void SetInstrumentIDList(std::set<std::string>& setInstrumentID);
std::set<std::string>& GetInstrumentIDList();

std::set<CWnd*>& GetBookOrderDlgSet();
CSequenceManager& GetSequenceManager();
CChangeOrderManager& GetChangeOrderManager();

int GetColorStyle();
BOOKORDERDLGCOLOR& GetBookOrderDlgColor();
void SetBookOrderDlgColor(BOOKORDERDLGCOLOR& tDlgColor, int nColorStyle);

SIZE& GetBookDlgSize();
void SetBookDlgSize(SIZE& dlgSize);

CString GetIniFile();

void SaveConfig();
void LoadConfig();

