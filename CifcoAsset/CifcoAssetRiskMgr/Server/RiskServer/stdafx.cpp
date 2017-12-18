// stdafx.cpp : source file that includes just the standard includes
// RiskServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

//#define NO_USE_TRACE	
#pragma warning( disable : 4996 )
#include <stdarg.h> 
void TRACE(const char* format,...)
{
#ifdef _DEBUG 
	#ifndef NO_USE_TRACE
	static const int BufferLen = 1024;        //注意此处大小根据需要修改
	va_list pNextArg;
	char szMessageBuffer[BufferLen];
	memset(szMessageBuffer,0,BufferLen);
	va_start(pNextArg,format);
	//_stprintf_s(szMessageBuffer,BufferLen-1,format,pNextArg);
	wvsprintf(szMessageBuffer,format,pNextArg);
	va_end(pNextArg);
	OutputDebugString(szMessageBuffer);
	#endif
#endif        //#ifdef _DEBUG
}

void GetFolderFileName(std::string& strFilePath,const std::string& folder)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr( 0, nPos + 1 );
		if(!folder.empty())
		{
			if(std::string::npos==folder.rfind("\\"))
				strSystemPath+=folder+"\\";
			else
				strSystemPath+=folder;
			CreateDirectory(strSystemPath.c_str(),NULL);
		}
		strFilePath = strSystemPath + strFilePath;
	}	
}
int GetTodayString(char* datestring)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int idays=st.wYear*10000+st.wMonth*100+st.wDay;
	if(datestring)
	{
		sprintf(datestring,"%d",idays);
	}
	return idays;
}
int GetIDPre()
{
	struct tm *ptr;
	time_t lt;
	lt =time(NULL);
	ptr=localtime(&lt);
	int idays=(ptr->tm_year-113)*1000+ptr->tm_yday;
	return idays*100000;
}