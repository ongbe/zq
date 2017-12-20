// stdafx.cpp : source file that includes just the standard includes
// Module-PositionDetailInfoList.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "cfgMgr/CfgMgr.h"
#include "../Module-Misc/constants.h"
#include "../Module-Misc/globalDefines.h"

bool OpenPlusInProc( const wxString& strPath )
{
	if ( NULL == GETTOPWINDOW())
	{
		return false;
	}

	wxString strCmdLine;
	strCmdLine.Printf( wxT("\"%s\" %d"),strPath.c_str(),reinterpret_cast<long>(GETTOPWINDOW()->GetHWND()));
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi={0};

	if(!CreateProcess(NULL,(LPSTR)strCmdLine.c_str(),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		string str = LOADSTRING(GUI_LOAD_FAILED);
		wxString strMsg = wxString::Format( str.c_str(), strPath.c_str());
		wxMessageBox( strMsg,LOADSTRING(OIP_ERROR),wxOK|wxICON_WARNING);
		return false;
	}
	else
	{
		//提示用户插件已启动，等待登录
		return true;
	}
}

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
