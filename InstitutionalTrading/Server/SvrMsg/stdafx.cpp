// stdafx.cpp : source file that includes just the standard includes
// testtcpsvr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

int g_nMasterSocket = 0;

void _SetMasterSocket(int nSocket)
{
	g_nMasterSocket = nSocket;
}

int  _GetMasterSocket()
{
	return g_nMasterSocket;
}
