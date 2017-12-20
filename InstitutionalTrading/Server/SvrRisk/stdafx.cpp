// stdafx.cpp : source file that includes just the standard includes
// testtcpsvr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
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

