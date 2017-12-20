/*
    定义接口数据结构
    added by l. 20110706
*/


#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../ctp/ThostFtdcUserApiStruct.h"
#include "wx/wxprec.h"
#include "KernelStruct.h" 

struct InstrumentInfo
{
	InstrumentInfo(std::string sid="",std::string sname=""):id(sid),name(sname){}
	std::string id;
	std::string name;
} ;

struct GroupInfo
{
	std::string GroupName;
	std::vector<InstrumentInfo> InstrVec;
};

struct Report
{
	std::string strReportType;
	int nSize;
	void* pReport;

	Report()
	{
		strReportType = "";
		nSize = 0;
		pReport = NULL;
	}

	DWORD GetDate()
	{
		int nPos = strReportType.find( ' ');
		if ( nPos == -1 )
		{
			return 0;
		}

		string strTemp = strReportType.substr( nPos+1, strReportType.length());
		nPos = strTemp.find( '-' );
		if ( nPos == -1 )
		{
			return 0;
		}

		string strDay = strTemp.substr( 0, nPos );
		strTemp = strTemp.substr( nPos+1, strTemp.length());
		nPos = strTemp.find( '-' );
		if ( nPos == -1 )
		{
			return 0;
		}

		string strMonth = strTemp.substr( 0, nPos );
		string strYear = strTemp.substr( nPos+1, strTemp.length());
		
		DWORD dwDate = atoi(strYear.c_str())*10000 + GetMonth(strMonth)*100 + atoi(strDay.c_str());
		return dwDate;
	}

	DWORD GetMonth( const string& month )
	{
		string strMonth = month;
		transform( strMonth.begin(), strMonth.end(), strMonth.begin(), toupper);
		if ( strMonth == "JAN" )
		{
			return 1;
		}
		else if ( strMonth == "FEB")
		{
			return 2;
		}
		else if ( strMonth == "MAR")
		{
			return 3;
		}
		else if ( strMonth == "APR")
		{
			return 4;
		}
		else if ( strMonth == "MAY")
		{
			return 5;
		}
		else if ( strMonth == "JUN")
		{
			return 6;
		}
		else if ( strMonth == "JUL")
		{
			return 7;
		}
		else if ( strMonth == "AUG")
		{
			return 8;
		}
		else if ( strMonth == "SEPT")
		{
			return 9;
		}
		else if ( strMonth == "OCT")
		{
			return 10;
		}
		else if ( strMonth == "NOV")
		{
			return 11;
		}
		else if ( strMonth == "DEC")
		{
			return 12;
		}
		else
		{
			return 0;
		}
	}
};