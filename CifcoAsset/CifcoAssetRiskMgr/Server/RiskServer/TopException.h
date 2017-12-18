#pragma once
#include <dbghelp.h>
#pragma comment( lib, "dbghelp.lib" )
void CreateMiniDump(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	std::string filename="Exception.dmp";
	GetFolderFileName(filename);
	HANDLE hFile = CreateFile(filename.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
	{
		return;
	}

	MINIDUMP_EXCEPTION_INFORMATION mei;
	mei.ThreadId = GetCurrentThreadId();
	mei.ClientPointers = TRUE;
	mei.ExceptionPointers = ExceptionInfo;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &mei, NULL, NULL);
	CloseHandle(hFile);
}

LONG WINAPI TopExceptionCallback(struct _EXCEPTION_POINTERS *pei)
{
	CreateMiniDump(pei);
	return EXCEPTION_CONTINUE_SEARCH;
}