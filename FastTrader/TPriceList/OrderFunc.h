// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once


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

BOOL ConvertComboInstrumentID(const CString& strComboID, CString& strInstrumentID1, 
							  CString& strInstrumentID2);
BOOL ParseOptionFormat(const CString& strOptionInstr, const CString& strExchangeID, 
					   CString& strTargetID, BOOL& bDirect, CString& strPrice);
BOOL ConvOptionFormat(const CString& strOptionInstr, const CString& strExchangeID, 
					  CString& strNewOptionInstr);
