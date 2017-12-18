// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once


enum {	conMaxRltCodeNum = 32 };

typedef struct {
	int	 nCmbCodeCount;
	char cType;
	char strCodeName[24];	// 合约简称
	char strCodeNameJP[24];	// 合约简称的拼音首字母
	char strCodeHeads[conMaxRltCodeNum][24];	// 合约代码头
} CODEHEADDATA, *PCODEHEADDATA;


int GetNumDigits(double dbTick);
CString Number2String(int nData);
// 价格显示转换函数，返回一个价格字符串
// dbData, 价格
// dbTick，最小变动价位
CString Price2String(double dbData, double dbTick);
CString Double2String(double dbData);
CString Money2String(double dblAmount, int precision=0);

BOOL CheckSumPrice(double& fPrice, double fPriceTick, int nStep=0);
double AnalysePrice(double fPrice, double fUpperLimitPrice, double fLowerLimitPrice);

BOOL ConvertComboInstrumentID(const CString& strComboID, CString& strInstrumentID1, CString& strInstrumentID2);

BOOL MakeReq_Order(PlatformStru_InputOrder& req, CString& strInstrumentID, CString& strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder);

char* GetProductSNameItem(char* csProductSName, CODEHEADDATA& codeHead, BOOL& bRetHasData);
BOOL GetCodesItem(char* csCodes, CODEHEADDATA& codeHead);
