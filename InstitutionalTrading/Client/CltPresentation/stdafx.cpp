// stdafx.cpp : source file that includes just the standard includes
// CltPresentation.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
//
//CPopMsgWin::Stru_Param param;
//param.bInitVisible = false;
//CPopMsgWin g_oPopMsg(param);
//
//CPopMsgWin& _GetPopMsg()
//{
//	return g_oPopMsg;
//}


CPopMsgWin* g_poPopMsg = NULL;

CPopMsgWin* _GetPopMsg()
{
	if(g_poPopMsg == NULL) {
		CPopMsgWin::Stru_Param param;
        strncpy(param.szTitle,"展现系统消息",sizeof(param.szTitle)-1);
		param.bInitVisible = false;
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