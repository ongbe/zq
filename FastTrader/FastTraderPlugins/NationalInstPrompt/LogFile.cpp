#include "StdAfx.h"
#include "LogFile.h"


bool CLogFile::OpenFile()//打开文件，指针到文件尾
{
    if(IsOpen())
        return true;

    if(!_szFileName)
        return false;

    _hFile =  CreateFile(
        _szFileName, 
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL 
        );

    if(!IsOpen() && GetLastError() == 2)//打开不成功，且因为文件不存在， 创建文件
        _hFile =  CreateFile(
        _szFileName, 
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL 
        ); 

    if(IsOpen())
        SetFilePointer(_hFile, 0, NULL, FILE_END);

    return IsOpen();
}

DWORD CLogFile::Write(LPCVOID lpBuffer, DWORD dwLength)
{
    DWORD dwWriteLength = 0;

    if(IsOpen())
        WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);

    return dwWriteLength;
}

void CLogFile::WriteLog( LPCVOID lpBuffer, DWORD dwLength)//写日志, 可以扩展修改
{
    time_t now;
    char temp[21];
    DWORD dwWriteLength;

    if(IsOpen())
    {
        time(&now);
        strftime(temp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

        WriteFile(_hFile, temp, 19, &dwWriteLength, NULL);
        WriteFile(_hFile, ":" , 1 , &dwWriteLength, NULL);
        WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);

        FlushFileBuffers(_hFile);

    }
}

void CLogFile::Lock()  { ::EnterCriticalSection(&_csLock); }
void CLogFile::Unlock() { ::LeaveCriticalSection(&_csLock); }



CLogFile::CLogFile(const char *szFileName/* = "Log.log"*/)//设定日志文件名
{
    _szFileName = NULL;
    _hFile = INVALID_HANDLE_VALUE;
    ::InitializeCriticalSection(&_csLock);

    SetFileName(szFileName);
}

CLogFile:: ~CLogFile()
{
    ::DeleteCriticalSection(&_csLock);
    Close();

    if(_szFileName)
        delete []_szFileName;
}

const char * CLogFile::GetFileName()
{
    return _szFileName;
}

void CLogFile::SetFileName(const char *szName)//修改文件名， 同时关闭上一个日志文件
{
    assert(szName);

    if(_szFileName)
        delete []_szFileName;

    Close();

    _szFileName = new char[strlen(szName) + 1];
    assert(_szFileName);
    strcpy(_szFileName, szName);
}

bool CLogFile::IsOpen()
{
    return _hFile != INVALID_HANDLE_VALUE;
}

void CLogFile::Close()
{
    if(IsOpen())
    {
        CloseHandle(_hFile);
        _hFile = INVALID_HANDLE_VALUE;
    }
}

void CLogFile::Log(LPCVOID lpBuffer, DWORD dwLength)//追加日志内容
{
    assert(lpBuffer);
    __try
    {
        Lock();

        if(!OpenFile())
            return;

        WriteLog(lpBuffer, dwLength);
    }
    __finally
    {
        Unlock();
    } 
}

void CLogFile::Log(const char *szText)
{
    Log(szText, strlen(szText));
}
