// stdafx.cpp : source file that includes just the standard includes
// CltRisk.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CPopMsgWin* g_poPopMsg = NULL;

CPopMsgWin* _GetPopMsg()
{
	if(g_poPopMsg == NULL) {
		CPopMsgWin::Stru_Param param;
		param.bInitVisible = false;
        strncpy(param.szTitle,"风控系统消息",sizeof(param.szTitle)-1);
		g_poPopMsg = new CPopMsgWin(param);        
	}
	return g_poPopMsg;
}

void _ReleasePopMsg()
{
	if(g_poPopMsg != NULL) {
		delete g_poPopMsg;
		g_poPopMsg = NULL;
	}
}

std::string Price2string(double dbData, double dbTick)
{
	char strPriceText[512]={0},strFormatText[512]={0};
	strcpy_s(strPriceText, "-");
	if(util::isInvalidValue(dbData)||util::isInvalidValue(dbTick)) 
		return strPriceText;
	sprintf_s(strFormatText,"%%0.%df", GetNumDigits(dbTick));
	sprintf_s(strPriceText, strFormatText, dbData);
	return std::string(strPriceText);
}
int GetNumDigits(double dbTick)
{
	int pos,nNumDigits=0;
	char strPriceText[512]={0};
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
