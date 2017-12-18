// stdafx.cpp : source file that includes just the standard includes
// patsystem.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
#include <stdarg.h> 
#pragma warning( push )
#pragma warning( disable : 4996 )

void DLL_TRACE(const char* format,...)
{
#ifdef _DEBUG

        static const int BufferLen = 1024;        //注意此处大小根据需要修改
        va_list pNextArg;
        char szMessageBuffer[BufferLen];
		memset(szMessageBuffer,0,BufferLen);
        va_start(pNextArg,format);
        //_stprintf_s(szMessageBuffer,BufferLen-1,format,pNextArg);
        wvsprintf(szMessageBuffer,format,pNextArg);
        va_end(pNextArg);
        OutputDebugString(szMessageBuffer);

#endif        //#ifdef _DEBUG
}
#pragma warning( pop )