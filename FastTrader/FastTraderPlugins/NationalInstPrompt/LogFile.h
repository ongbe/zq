#pragma once

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>


class CLogFile
{
protected:

    CRITICAL_SECTION _csLock;
    char * _szFileName;
    HANDLE _hFile;

    bool OpenFile();
    DWORD Write(LPCVOID lpBuffer, DWORD dwLength);
    virtual void WriteLog( LPCVOID lpBuffer, DWORD dwLength);//写日志, 可以扩展修改

    void Lock();
    void Unlock();

public:

    CLogFile(const char *szFileName = "Log.log");   
    virtual ~CLogFile();

    const char * GetFileName();    
    void SetFileName(const char *szName);//修改文件名， 同时关闭上一个日志文件

    bool IsOpen();
    void Close();
    void Log(LPCVOID lpBuffer, DWORD dwLength);//追加日志内容
    void Log(const char *szText);
};
