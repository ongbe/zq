// stdafx.cpp : source file that includes just the standard includes
// TPriceList.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

HWND g_hMainWnd = NULL;

void _SetMainWnd(HWND hMainWnd)
{
	g_hMainWnd = hMainWnd;
}

HWND _GetMainWnd()
{
	return g_hMainWnd;
}

CWnd* g_pPanel = NULL;

void _SetCurrPanel(CWnd* pPanel)
{
	g_pPanel = pPanel;
}

CWnd* _GetCurrPanel()
{
	return g_pPanel;
}
