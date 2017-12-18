// stdafx.cpp : source file that includes just the standard includes
// zq.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


BOOL g_bHasConsoleStart = FALSE;

void SetConsoleStart(BOOL bHas)
{
	g_bHasConsoleStart = bHas;
}

BOOL HasConsoleStart()
{
	return g_bHasConsoleStart;
}

HWND g_hConsoleHWnd = NULL;

void SetConsoleHWnd(HWND hWnd)
{
	g_hConsoleHWnd = hWnd;
}

HWND GetConsoleHWnd()
{
	return g_hConsoleHWnd;
}
