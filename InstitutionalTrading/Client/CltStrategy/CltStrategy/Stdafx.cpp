// stdafx.cpp : source file that includes just the standard includes
// AddIn.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <windows.h>

UINT g_nDataRecvMsgID = ::RegisterWindowMessage("");

UINT _GetDataRecvMsgID()
{
	return g_nDataRecvMsgID;
}

UINT g_nEventMsgID = ::RegisterWindowMessage("");

UINT _GetEventMsgID()
{
	return g_nEventMsgID;
}

UINT g_nCurrRequestID = 0;

UINT _GetNextRequestID()
{
	g_nCurrRequestID++;
	return g_nCurrRequestID;
}

std::string g_strAddinDllFileName;

void _SetAddinDllFileName(const std::string& strFile)
{
	g_strAddinDllFileName = strFile;
}

const std::string& _GetAddinDllFileName()
{
	return g_strAddinDllFileName;
}

std::string g_strWorkPath;

void _SetWorkPath(const std::string& strPath)
{
	g_strWorkPath = strPath;
	::CreateDirectory(strPath.c_str(), NULL);
	::CreateDirectory(_GetProjectsPath().c_str(), NULL);
	::CreateDirectory(_GetShareDllsPath().c_str(), NULL);
	::CreateDirectory(_GetTempPath().c_str(), NULL);
	::CreateDirectory(_GetHistoryDataPath().c_str(), NULL);
}

std::string _GetWorkPath()
{
	return g_strWorkPath;
}

std::string _GetProjectsPath()
{
	return g_strWorkPath+"Projects\\";
}

std::string _GetShareDllsPath()
{
	return g_strWorkPath+"ShareDlls\\";
}

std::string _GetTempPath()
{
	return g_strWorkPath+"Temp\\";
}

std::string _GetHistoryDataPath()
{
	return g_strWorkPath+"HistoryDatas\\";
}

std::string g_strSupportFilesPath;

void _SetSupportFilesPath(const std::string& strPath)
{
	g_strSupportFilesPath = strPath;
}

std::string _GetSupportFilesPath()
{
	return g_strSupportFilesPath;
}

std::string g_strCurrUserID;

void _SetCurrUserID(const std::string& strID)
{
	g_strCurrUserID = strID;
}

std::string _GetCurrUserID()
{
	return g_strCurrUserID;
}

std::string g_strPassword;

void _SetPassword(const std::string& strPasswd)
{
	g_strPassword = strPasswd;
}

std::string _GetPassword()
{
	return g_strPassword;
}

CPopMsgWin* g_poPopMsg = NULL;

CPopMsgWin* _GetPopMsg()
{
	if(g_poPopMsg == NULL) {
		CPopMsgWin::Stru_Param param;
		param.bInitVisible = false;
        strncpy(param.szTitle,"策略开发系统消息",sizeof(param.szTitle)-1);
		g_poPopMsg = new CPopMsgWin(param);
	}
	return g_poPopMsg;
}

void _ReleasePopMsg()
{
	if(g_poPopMsg != NULL) {
		//::PostMessage(g_poPopMsg->GetSafeWnd(), WM_QUIT, 0, 0);
		// delete可能导致死锁
		//delete g_poPopMsg;
		g_poPopMsg = NULL;
	}
}
