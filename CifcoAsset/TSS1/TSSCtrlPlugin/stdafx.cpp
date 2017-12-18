// stdafx.cpp : 只包括标准包含文件的源文件
// TSSCtrlPlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


UINT nDataPkgArriveMSGID = ::RegisterWindowMessage("TSSCtrlPlugin Data Package Arrive Message ID");
UINT nDataPkgArriveNextMSGID = ::RegisterWindowMessage("TSSCtrlPlugin Data Package Arrive Next Message ID");


CString _GetIniFile()
{
	DWORD dwRetLen = 0;
	char strTemp[1024];

	ZeroMemory(strTemp, sizeof(strTemp));
	dwRetLen = GetCurrentDirectory(sizeof(strTemp), strTemp);
	if(strTemp[dwRetLen-1]!='\\')
		strcat_s(strTemp, sizeof(strTemp)-1, "\\");
	strcat_s(strTemp, sizeof(strTemp)-1, INIFILE);
	return strTemp;
}

UINT _GetDataPkgArriveMSGID()
{
	return nDataPkgArriveMSGID;
}
