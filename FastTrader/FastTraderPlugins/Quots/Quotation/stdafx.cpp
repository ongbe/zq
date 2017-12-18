
// stdafx.cpp : source file that includes just the standard includes
// Quotation.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#define rgb(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
static int ColorMap[]={
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,1,2,3,4,5,6,7,    8,9,0,0,0,0,0,0,

	0,10,11,12,13,14,15,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,10,11,12,13,14,15,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0
};

long ColorStr2Long(const char* pColTextColor)
{
	if ( NULL == pColTextColor )
	{
		return -1;
	}
	long ColTextColor=0;
	string str = pColTextColor;	
	if(str.find(',') != -1)
	{
		vector<string> vec;
		split(str, ',', vec);
		if(vec.size()!=3)
			return -1;
		int R = atoi(vec[0].c_str());
		int G = atoi(vec[1].c_str());
		int B = atoi(vec[2].c_str());

		ColTextColor = rgb(R, G, B);
	}
	else
		ColTextColor=atol(pColTextColor);
	/*
	long ColTextColor=0;
	if(pColTextColor[0]=='#')
	{
	ColTextColor = ( ColorMap[(pColTextColor[5])]<<20)+(ColorMap[(pColTextColor[6])]<<16)
	+(ColorMap[(pColTextColor[3])]<<12)+(ColorMap[(pColTextColor[4])]<<8)+
	(ColorMap[(pColTextColor[1])]<<4)+ColorMap[(pColTextColor[2])];
	}
	else
	ColTextColor=atol(pColTextColor);
	*/
	return ColTextColor;
}

wxString Double2String(double dbValue, int nNumDigits, bool bUseMax /*= false*/)
{
	wxString str;

	wxString strFormat;
	if(bUseMax )
	{
		if(dbValue>1000)
			str = wxString::Format(wxT("%.0f"), dbValue);
		else 
		{
			int n = 2;
			if(nNumDigits>2)
				n = nNumDigits;
			strFormat.Printf(wxT("%%.%df"), n);
			str.Printf(strFormat, dbValue);	
		}
	}
	else
	{
		strFormat.Printf(wxT("%%.%df"), nNumDigits);
		str.Printf(strFormat, dbValue);	
	}
	
	return str;
}
wxString Double2String_Tip(double dbValue)
{
	wxString str;
	if(dbValue>=10000)
		str = wxString::Format(wxT("%.0f"), dbValue);
	else if(dbValue>=1000)
		str = wxString::Format(wxT("%.2f"), dbValue);
	else
	{
		str = wxString::Format(wxT("%.3f"), dbValue);
	}
	return str;
}


wxString GetBNCMIniFile()
{
	DWORD dwRetLen = 0;
	char strTemp[1024];

	ZeroMemory(strTemp, sizeof(strTemp));
	dwRetLen = GetCurrentDirectory(sizeof(strTemp), strTemp);
	if(strTemp[dwRetLen-1]!='\\')
		strcat_s(strTemp, sizeof(strTemp)-1, "\\");
	strcat_s(strTemp, sizeof(strTemp)-1, "hqData\\");
	strcat_s(strTemp, sizeof(strTemp)-1, BNCMINIFILE);
	return strTemp;
}

string GetTechNamePhrase(string strTechIndexName, EnumPhrase enumPhrase)
{
	string strPhrase = strTechIndexName;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		strPhrase = strPhrase + "min1"; 
		break;
	case PHRASE_5MIN:
		strPhrase = strPhrase + "min5"; 
		break;
	case PHRASE_15MIN:
		strPhrase = strPhrase + "min15"; 
		break;
	case PHRASE_30MIN:
		strPhrase = strPhrase + "min30"; 
		break;
	case PHRASE_60MIN:
		strPhrase = strPhrase + "min60"; 
		break;
	case PHRASE_DAY:
		strPhrase = strPhrase +  "day";
		break;
	case PHRASE_WEEK:
		strPhrase = strPhrase + "week";
		break;
	case PHRASE_MONTH:
		strPhrase = strPhrase + "month";
		break;

	}
	return strPhrase;
}