// stdafx.cpp : source file that includes just the standard includes
// OptionQuation.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}
bool IsDoubleSame(double db1, double db2)
{
	if(db1 - db2 > -0.00001 && db1 - db2 < 0.00001)
		return true;
	return false;
}
CString Double2String(double dbValue, int nNumDigits, bool bUseMax /*= false*/)
{
	CString str;

	CString strFormat;
	if(bUseMax )
	{
		if(dbValue>1000)
			str.Format(_T("%.0f"), dbValue);
		else 
		{
			int n = 2;
			if(nNumDigits>2)
				n = nNumDigits;
			strFormat.Format(_T("%%.%df"), n);
			str.Format(strFormat, dbValue);	
		}
	}
	else
	{
		strFormat.Format(_T("%%.%df"), nNumDigits);
		str.Format(strFormat, dbValue);	
	}

	return str;
}
CString Double2String_Tip(double dbValue)
{
	CString str;
	if(dbValue>=10000)
		str.Format(_T("%.0f"), dbValue);
	else if(dbValue>=1000)
		str.Format(_T("%.2f"), dbValue);
	else
	{
		str.Format(_T("%.3f"), dbValue);
	}
	return str;
}
  void  WriteLog_DepthMarketData(std::string szXns)
{
	long dwID = ::GetCurrentProcessId();
	CString strFile;
	strFile.Format(_T("c:\\HQTrace\\hq_%d_DepthMarketData.txt"), dwID);
	HFILE hF =_lopen(strFile.GetBuffer(strFile.GetLength()),OF_READWRITE);
	if(hF==HFILE_ERROR)
	{
		CString strFile2 = _T("c:\\HQTrace\\");
		CreateDirectory(strFile2.GetBuffer(strFile2.GetLength()), NULL);
		hF=_lcreat(strFile.GetBuffer(strFile.GetLength()),0);	
	}
	_llseek(hF,0,2);
	_lwrite(hF,szXns.c_str(),szXns.length());	
	_lwrite(hF,"\r\n",2);
	_lclose(hF);
}
 int getExpoBase2(const double& d)
{
	int i = 0;
	((short *)(&i))[0] = (((short *)(&d))[3] & (short)0x7ff0);
	return (i >> 4) - 1023;
}
 bool	equals(const double& d1, const double& d2)
{
	if (d1 == d2)
		return true;
	int e1 = getExpoBase2(d1);
	int e2 = getExpoBase2(d2);
	int e3 = getExpoBase2(d1 - d2);
	if ((e3 - e2 < -48) && (e3 - e1 < -48))
		return true;
	return false;
}
 bool isInvalidValue(const double& d)
{
	return equals(fabs(d), 1.7976931348623158e+308);
}
 void GetFullStr(CString& str, double fValue)
{
	CString strTemp = str;
	if(isInvalidValue(fValue))
	{
		str.Format(_T("%s - "), strTemp);
	}
	else
	{
		str.Format(_T("%s %.2f "), strTemp, fValue);
	}
}
 void GetFullStr(CString& str, int nValue, int n)
{
	CString strTemp = str;
	if(isInvalidValue(nValue))
	{
		str.Format(_T("%s - "), strTemp);
	}
	else
	{
		str.Format(_T("%s %7d "), strTemp, nValue);
	}
}
 void AppendLog_DepthMarketData(PlatformStru_DepthMarketData* pData)
{

	CString str;
	str.Format(_T("%s %s %s"), pData->TradingDay, 	pData->UpdateTime, 	pData->InstrumentID);

	GetFullStr(str, pData->PreSettlementPrice);
	GetFullStr(str, pData->LastPrice);
	GetFullStr(str, pData->OpenPrice);
	GetFullStr(str, pData->HighestPrice);
	GetFullStr(str, pData->LowestPrice);
	GetFullStr(str, pData->Volume, 0);
	GetFullStr(str, pData->Turnover);

	GetFullStr(str, pData->AskPrice1);
	GetFullStr(str, pData->AskVolume1, 0);
	GetFullStr(str, pData->BidPrice1);
	GetFullStr(str, pData->BidVolume1, 0);
	GetFullStr(str, pData->OpenInterest);

	WriteLog_DepthMarketData(str.GetBuffer(str.GetLength()));
	if(str.GetLength() > 200)
	{
		//	wxMessageBox(str,"ÌבÊ¾",wxYES_NO);		
	}
}
EnumPhrase GetEnumPhraseValue(KLineDataCycle eCycle)
{
	EnumPhrase enumPhrase = PHRASE_USERDEFINE;
	switch(eCycle)
	 {
	 case Minute_1:
		 enumPhrase = PHRASE_1MIN;
		 break;
	 case Minute_5:
		 enumPhrase = PHRASE_5MIN;
		 break;
	 case Minute_15:
		 enumPhrase = PHRASE_15MIN;
		 break;
	 case Minute_30:
		 enumPhrase = PHRASE_30MIN;
		 break;
	 case Minute_60:
		 enumPhrase = PHRASE_60MIN;
		 break;
	 case Day_1:
		 enumPhrase = PHRASE_DAY;
		 break;
	 case Week_1:
		 enumPhrase = PHRASE_WEEK;
		 break;
	 case Month_1:
		 enumPhrase = PHRASE_MONTH;
		 break;		
	 default:
		 {
			 break;
		 }
	 }
	return enumPhrase;
}
KLineDataCycle GetKLineDataCycleValue(EnumPhrase enumPhrase)
{
	KLineDataCycle eCycle;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		eCycle = Minute_1;
		break;
	case PHRASE_5MIN:
		eCycle = Minute_5;
		break;
	case PHRASE_15MIN:
		eCycle = Minute_15;
		break;
	case PHRASE_30MIN:
		eCycle = Minute_30;
		break;
	case PHRASE_60MIN:
		eCycle = Minute_60;
		break;
	case PHRASE_DAY:
		eCycle = Day_1;
		break;
	case PHRASE_WEEK:
		eCycle = Week_1;
		break;
	case PHRASE_MONTH:
		eCycle = Month_1;
		break;		
	}
	return eCycle;
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