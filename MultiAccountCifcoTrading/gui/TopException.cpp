#include "StdAfx.h"
#include "TopException.h"
//此文件只工作在Release模式
#include <dbghelp.h>
#pragma comment( lib, "dbghelp.lib" )
const int MAX_DebugBuffer = 4096;    // 应用程序和调试器之间传递数据是通过一个 4KB 大小的共享内存块完成的

typedef struct dbwin_buffer {
    DWORD   dwProcessId;
    char    szString[MAX_DebugBuffer-sizeof(DWORD)];
}DEBUGBUFFER,*PDEBUGBUFFER;
list<string> g_DebugBufferList;
void CreateMiniDump(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    std::string filename="Exception.dmp";
    GlobalFunc::GetPath(filename);
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
	wxFile file;
	std::string filename="Exception.log";
	GlobalFunc::GetPath(filename);
    if(file.Open(filename.c_str(),wxFile::write_append))
	{
        string AllLog;
        list<string>::const_iterator it=g_DebugBufferList.begin();
        for(;it!=g_DebugBufferList.end();++it)
        {
            AllLog+=it->c_str();
        }
        file.Write(AllLog.c_str(),AllLog.length());
        file.Close();

	}
    return EXCEPTION_CONTINUE_SEARCH;
}
CTrackThread::CTrackThread():wxThread()
{

}
void * CTrackThread::Entry()
{
    HANDLE hMapping = NULL;  
    HANDLE hAckEvent = NULL;
    HANDLE hReadyEvent = NULL;
    PDEBUGBUFFER pdbBuffer = NULL;  
    // 打开事件句柄    
    hAckEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("DBWIN_BUFFER_READY"));    
    if(hAckEvent == NULL)
    {
        CloseHandle(hAckEvent); 
        return NULL;
    }

    hReadyEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("DBWIN_DATA_READY"));  
    if(hReadyEvent == NULL)
    {
        CloseHandle(hReadyEvent); 
        return NULL;
    } 

    // 创建文件映射  
    hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_DebugBuffer, TEXT("DBWIN_BUFFER"));   
    if(hMapping == NULL)
    {
        CloseHandle(hMapping); 
        return NULL;
    } 

    // 映射调试缓冲区  
    pdbBuffer = (PDEBUGBUFFER) MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);  
    DWORD pid=::GetCurrentProcessId();
    char   TimeString[MAX_DebugBuffer+20]={0};
    // 循环  
    while( !TestDestroy() )
    {             
        // 激活事件
        SetEvent(hAckEvent);  
        // 等待缓冲区数据
        DWORD dwError=WaitForSingleObject(hReadyEvent, 1000);
        if(dwError==WAIT_TIMEOUT) 
            continue; 
        if(dwError==WAIT_OBJECT_0)   
        {              
            if(pid== pdbBuffer->dwProcessId)
            {
                time_t timer=time(NULL);
                struct tm* ptm=localtime(&timer);
                sprintf_s(TimeString,sizeof(TimeString)-1,"%02d:%02d:%02d  %s",
                    ptm->tm_hour,ptm->tm_min,ptm->tm_sec,pdbBuffer->szString);
                if(TimeString[strlen(TimeString)-1]!='\n') 
                    strcat(TimeString,"\r\n");
                g_DebugBufferList.push_back(TimeString); 
                if(g_DebugBufferList.size()>1000)
                    g_DebugBufferList.pop_front();
            }
        }   
    } 
    // 释放   
    if (pdbBuffer) 
    {      
        UnmapViewOfFile(pdbBuffer);   
    }   
    CloseHandle(hMapping);   
    CloseHandle(hReadyEvent);  
    CloseHandle(hAckEvent);
    return NULL;
}
void CTrackThread::OnExit()
{

}
CBeepThread::CBeepThread():wxThread()
{

}
void * CBeepThread::Entry()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	UINT count=0;
	while(!TestDestroy())
	{
		count=0;
		while(PeekMessage(&msg,(HWND)-1, 0, 0, PM_REMOVE))
			count++;
		if(count>0&&msg.message==WM_BEEPMSG)//有消息,MSG保存最后一个消息
		{
			switch(msg.wParam)
			{
			case 2:
				Beep(800, 300);
				Sleep(200);
				Beep(800, 300);
				break;
			case 1:
				Beep(800, 1200);
				break;
            default:
                Sleep(200);
                break;
			}
		}
        else Sleep(200);
	}
	return NULL;
}
void CBeepThread::OnExit()
{

}