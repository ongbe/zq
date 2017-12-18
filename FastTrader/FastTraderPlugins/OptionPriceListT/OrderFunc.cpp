// Global.cpp : 只包括标准包含文件的源文件
// Module-BookOrder.pch 将作为预编译头
// Global.obj 将包含预编译类型信息

#include "stdafx.h"
#include "OrderFunc.h"
#include "Module-Misc2/tools_util.h"


int GetNumDigits(double dbTick)
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

CString Number2String(int nData)
{
	char str[512]="-";
	if(nData!=-1) sprintf(str, "%d", nData);
	return str;
}

CString Price2String(double dbData, double dbTick)
{
	char strPriceText[512]={0},strFormatText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	ZeroMemory(strFormatText, sizeof(strFormatText));
	strcpy(strPriceText, "-");
	if(util::isInvalidValue(dbData)||util::isInvalidValue(dbTick)) 
		return strPriceText;
	sprintf(strFormatText,"%%0.%df", GetNumDigits(dbTick));
	sprintf(strPriceText, strFormatText, dbData);
	return strPriceText;
}

CString Double2String(double dbData)
{
	char strPriceText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	strcpy(strPriceText, "-");
	if(util::isInvalidValue(dbData)) 
		return strPriceText;
	sprintf(strPriceText, "%f", dbData);
	for(int i=strlen(strPriceText)-1; i>=0; i--) 
	{
		if(strPriceText[i]=='0')
			strPriceText[i]=0;
		else
		{
			if(strPriceText[i]=='.')
				strPriceText[i]=0;
			break;
		}
	}
	return strPriceText;
}

//获取货币的分段显示格式
CString Money2String(double dblAmount, int precision)
{
	char szBuf[256];
    char szFormat[16];
    int i,Len,ipt;

    if(precision<0)
		precision=0;

    ZeroMemory(szBuf, sizeof(szBuf));
	sprintf(szFormat, "%%.%df", precision);
    sprintf(szBuf, szFormat, dblAmount);
    szBuf[sizeof(szBuf)-1]='\000';

    Len=strlen(szBuf);
    for(ipt=0;ipt<Len&&szBuf[ipt]!='.';ipt++);
    while(ipt>3&&
            szBuf[ipt-3-1]>='0'&&szBuf[ipt-3-1]<='9'&&
            Len<sizeof(szBuf)-10)
    {
        i=ipt-3;
        memmove(szBuf+i+1,szBuf+i,Len-i+1);
        szBuf[i]=',';
        Len++;
        ipt-=3;
    }
    return CString(szBuf);
}

BOOL CheckSumPrice(double& fPrice, double fPriceTick, int nStep)
{
	if(fPriceTick < 0.000001)
		return FALSE;

	double fNewPrice = fPrice;
	long nNewPrice = 0;

	if(nStep==0) {
		if(fNewPrice>0.0)
			nNewPrice=(long)((fPrice+fPriceTick/2.0+0.0000000009)/fPriceTick);
		else
			nNewPrice=(long)((fPrice-fPriceTick/2.0-0.0000000009)/fPriceTick);
		fNewPrice=(double)nNewPrice*fPriceTick;
	}
	else {
		if(fNewPrice>0.0)
			nNewPrice=(long)(fNewPrice/fPriceTick+0.0000000009);
		else
			nNewPrice=(long)(fNewPrice/fPriceTick-0.0000000009);
		fNewPrice=(double)nNewPrice*fPriceTick;
		if(abs(fPrice - fNewPrice)<0.0000000009) {
			if(nStep>0) {
				fNewPrice+=fPriceTick;
			}
			else {
				fNewPrice-=fPriceTick;
			}
		}
		else {
			if(nStep>0 && fPrice>0.0) {
				fNewPrice+=fPriceTick;
			}
			else if(nStep<0 && fPrice<0.0) {
				fNewPrice-=fPriceTick;
			}
		}
	}

	BOOL bIsEqual = abs(fPrice - fNewPrice)<0.0000000009;
	fPrice = fNewPrice;
	return !bIsEqual;
}

double AnalysePrice(double fPrice, double fUpperLimitPrice, double fLowerLimitPrice)
{
	if(util::isInvalidValue(fPrice))
		return 0.0;
	else if(fUpperLimitPrice==0.0 
			&& fLowerLimitPrice==0.0)
		return fPrice;
	else if(util::isInvalidValue(fUpperLimitPrice) 
			|| util::isInvalidValue(fLowerLimitPrice)) 
		return fPrice;
	else if(fPrice>fUpperLimitPrice) 
		return fUpperLimitPrice;
	else if(fPrice<fLowerLimitPrice)
		return fLowerLimitPrice;
	else 
		return fPrice;
}

BOOL ConvertComboInstrumentID(const CString& strComboID, CString& strInstrumentID1, CString& strInstrumentID2)
{
	char strTemp[128];
	char strCode1[128];
	char strCode2[128];
	ZeroMemory(strCode1, 128);
	ZeroMemory(strCode2, 128);
	sscanf_s(strComboID, "%s %[^&]&%s", strTemp, sizeof(strTemp)-1, 
			strCode1, sizeof(strCode1)-1, strCode2, sizeof(strCode2)-1);
	strInstrumentID1 = strCode1;
	strInstrumentID2 = strCode2;
	return TRUE;
}

BOOL ParseOptionFormat(const CString& strOptionInstr, const CString& strExchangeID, 
					   CString& strTargetID, BOOL& bDirect, CString& strPrice)
{
	if(strExchangeID.Compare("CZCE")==0) {
		int nFind = -1;
		if((nFind = strOptionInstr.Find('C', 5))!=-1) {
			bDirect = TRUE;
		}
		else {
			if((nFind = strOptionInstr.Find('P', 5))!=-1) {
				bDirect = FALSE;
			}
			else {
				return FALSE;
			}
		}
		strTargetID = strOptionInstr.Mid(0, nFind);
		strPrice = strOptionInstr.Mid(nFind+1, strOptionInstr.GetLength());
		return TRUE;
	}
	else {
		char csTarget[32];
		char csDirect[32];
		char csPrice[32];

		ZeroMemory(csTarget, sizeof(csTarget));
		ZeroMemory(csDirect, sizeof(csDirect));
		ZeroMemory(csPrice, sizeof(csPrice));
		int nRet = sscanf_s(strOptionInstr, "%[^-]-%[^-]-%s", csTarget, sizeof(csTarget)-1, 
				csDirect, sizeof(csDirect)-1, csPrice, sizeof(csPrice)-1);
		if(nRet!=3)
			return FALSE;

		strTargetID = csTarget;
		bDirect = csDirect[0]=='C';
		strPrice = csPrice;
		return TRUE;
	}
}

BOOL ConvOptionFormat(const CString& strOptionInstr, const CString& strExchangeID, 
					  CString& strNewOptionInstr)
{
	CString strTargetID;
	BOOL bDirect = FALSE;
	CString strPrice;

	if(ParseOptionFormat(strOptionInstr, strExchangeID, strTargetID, bDirect, strPrice)) {
		strNewOptionInstr = strTargetID;
		strNewOptionInstr += "-";
		strNewOptionInstr += strPrice;
		return TRUE;
	}
	else
		return FALSE;
}