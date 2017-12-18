// WriteLog.cpp: implementation of the CWriteLog class.
//
//	统一日志接口
//
//	1. 定义了统一的日志写入接口
//	2. DLL
//	3. 可以供C++/Java程序调用
//
//	history 
//	20100113	lzq		创建该文件
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "Tools_Ansi.h"
#include "Tools_Win32.h"
#include "WriteLog.h"

#ifdef WIN32
#pragma unmanaged
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteLog::CWriteLog(TYPE_WriteLogMode LogMode,char* pFileName)
{

	m_LogMode=LogMode;
	memset(m_pLocalFileName,0,sizeof(m_pLocalFileName));
	memset(m_pLocalFileDir,0,sizeof(m_pLocalFileDir));
	m_TmpBufSize=65536;
	m_pTmpBuf=(char*)malloc(m_TmpBufSize);

	if(m_LogMode==WriteLogMode_LOCALFILE)
		SetLogMode_LocalFile(pFileName);
}


CWriteLog::~CWriteLog()
{
	if(m_pTmpBuf)
	{
		free(m_pTmpBuf);
		m_pTmpBuf=NULL;
	}
}



/******************************************************************************************************
*   @brief					写入一条日志        
*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
*				fmt及其它	类似于sprintf()的参数
*******************************************************************************************************/
void CWriteLog::WriteLog_Fmt(char* pLogName,int LogLevel,char* fmt,...)
{
	va_list			argp;

	if(!m_pTmpBuf) return;

	m_Sem.lock();

	va_start(argp,fmt);

	m_pTmpBuf[0]=0;
	try
	{
		vsprintf(m_pTmpBuf,fmt,argp);
	}
	catch(...)
	{
		m_pTmpBuf[0]=0;
	}
	m_pTmpBuf[m_TmpBufSize-1]=0;

	va_end(argp);


	WriteLog_Internal(pLogName,LogLevel,m_pTmpBuf);


	m_Sem.unlock();
}


/******************************************************************************************************
*   @brief					将一个字符串写入一条日志，内部调用，不加锁
*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
*				pLog		日志字符串
*******************************************************************************************************/
void CWriteLog::WriteLog_Internal(char* pLogName,int LogLevel,char *pLog)
{
	char DateBuf[256];
	char LogLevelBuf[256];
	HFILE hF;
	SYSTEMTIME CurTime;


	GetLocalTime(&CurTime);
	sprintf(DateBuf,"%04d-%02d-%02d %02d:%02d:%02d.%03d : ",
			CurTime.wYear,CurTime.wMonth,CurTime.wDay,
			CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds);
	sprintf(LogLevelBuf,"%02d,",LogLevel);


	if(m_LogMode==WriteLogMode_LOCALFILE)
	{
		hF=_lopen(m_pLocalFileName,OF_READWRITE);
	    if(hF==HFILE_ERROR)
		    hF=_lcreat(m_pLocalFileName,0);
	    if(hF==HFILE_ERROR&&m_pLocalFileDir[0]!=0)
	    {
		    CreateDirectory(m_pLocalFileDir,NULL);
		    hF=_lcreat(m_pLocalFileName,0);
	    }
		if(hF!=HFILE_ERROR)
		{
			_llseek(hF,0,2);
			_lwrite(hF,DateBuf,strlen(DateBuf));
			if(pLogName!=NULL)
			{
				_lwrite(hF,pLogName,strlen(pLogName));
				_lwrite(hF,",",1);
			}
			_lwrite(hF,LogLevelBuf,strlen(LogLevelBuf));
			_lwrite(hF,pLog,strlen(pLog));
			_lwrite(hF,"\r\n",2);
			_lclose(hF);
		}
	}
}


/******************************************************************************************************
*   @brief					设置日志模式为本地文件日志
*   @param[in]	pFileName	本地日志文件名
*******************************************************************************************************/
void CWriteLog::SetLogMode_LocalFile(char *pFileName)
{
	char Buf[MAX_PATH_WriteLog];
	char ProgramFileName[MAX_PATH_WriteLog];
	char ProgramPath[MAX_PATH_WriteLog];							//程序所在的路径，带盘符
	char ProgramDiskSymbol[MAX_PATH_WriteLog];						//程序所在的盘符
	char ProgramName[MAX_PATH_WriteLog];
	char ProgramName_WithoutExt[MAX_PATH_WriteLog];
	char FilePath[MAX_PATH_WriteLog];
	char FileDiskSymbol[MAX_PATH_WriteLog];
	HFILE hF;
	int i;
	SYSTEMTIME CurTime;


	m_LogMode=WriteLogMode_LOCALFILE;

	memset(m_pLocalFileName,0,sizeof(m_pLocalFileName));
	memset(m_pLocalFileDir,0,sizeof(m_pLocalFileDir));


															//先取出程序所在的路径和盘符
	memset(ProgramFileName,0,sizeof(ProgramFileName));
	memset(ProgramPath,0,sizeof(ProgramPath));
	memset(ProgramName,0,sizeof(ProgramName));
	memset(ProgramDiskSymbol,0,sizeof(ProgramDiskSymbol));
	memset(ProgramName_WithoutExt,0,sizeof(ProgramName_WithoutExt));
	GetModuleFileName(NULL,ProgramFileName,sizeof(ProgramFileName)-1);
	CTools_Ansi::GetDiskSymbol(ProgramFileName,ProgramDiskSymbol,sizeof(ProgramDiskSymbol));
	CTools_Ansi::GetFilePathWithoutDiskSymbol(ProgramFileName,ProgramPath,sizeof(ProgramPath));
	CTools_Ansi::GetPureFileName(ProgramFileName,ProgramName,sizeof(ProgramName));
    for(i=strlen(ProgramName);i>0;i--)
        if(ProgramName[i-1]=='.') break;
    if(i>0&&i<sizeof(ProgramName_WithoutExt))
        memcpy(ProgramName_WithoutExt,ProgramName,i-1);
    else strncpy(ProgramName_WithoutExt,ProgramName,sizeof(ProgramName_WithoutExt)-1);


	if(pFileName)
	{
																//取指定的路径和盘符
		memset(FilePath,0,sizeof(FilePath));
		memset(FileDiskSymbol,0,sizeof(FileDiskSymbol));
		CTools_Ansi::GetDiskSymbol(pFileName,FileDiskSymbol,sizeof(FileDiskSymbol));
		CTools_Ansi::GetFilePathWithoutDiskSymbol(pFileName,FilePath,sizeof(FilePath));


		if(FileDiskSymbol[0]!=0)
		{														//指定了盘符，认为也就指定了路径
			strncpy(m_pLocalFileName,pFileName,sizeof(m_pLocalFileName)-1);
			sprintf(m_pLocalFileDir,"%s%s",FileDiskSymbol,FilePath);
		}
		else if(FilePath[0]=='\\')
		{														//未指定盘符，指定了绝对路径。使用程序所在的盘符和指定的绝对路径 
			strncpy(m_pLocalFileName,ProgramDiskSymbol,sizeof(m_pLocalFileName)-1);
			strncpy(m_pLocalFileName+strlen(m_pLocalFileName),pFileName,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
			strncpy(m_pLocalFileDir,ProgramDiskSymbol,sizeof(m_pLocalFileDir)-1);
			strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),FilePath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		}
		else
		{														//未指定盘符，未指定绝对路径。使用程序所在的盘符和路径的相对路径
			strncpy(m_pLocalFileName,ProgramDiskSymbol,sizeof(m_pLocalFileName)-1);
			strncpy(m_pLocalFileName+strlen(m_pLocalFileName),ProgramPath,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
			strncpy(m_pLocalFileName+strlen(m_pLocalFileName),pFileName,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
			strncpy(m_pLocalFileDir,ProgramDiskSymbol,sizeof(m_pLocalFileDir)-1);
			strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),ProgramPath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
			strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),FilePath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		}
	}
	else
	{
																//取缺省的日志文件名和文件目录
		strncpy(m_pLocalFileDir,ProgramDiskSymbol,sizeof(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),ProgramPath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),"log_",sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),ProgramName_WithoutExt,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),"\\",sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		
		strncpy(m_pLocalFileName,m_pLocalFileDir,sizeof(m_pLocalFileName)-1);
		GetLocalTime(&CurTime);
		sprintf(Buf,"%04d%02d%02d%02d.log",CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour);
		strncpy(m_pLocalFileName+strlen(m_pLocalFileName),Buf,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
	}



															//创建日志文件
	hF=_lopen(m_pLocalFileName,OF_READWRITE);
	if(hF==HFILE_ERROR)
		hF=_lcreat(m_pLocalFileName,0);
	if(hF==HFILE_ERROR&&m_pLocalFileDir[0]!=0)
	{
		CreateDirectory(m_pLocalFileDir,NULL);
		hF=_lcreat(m_pLocalFileName,0);
	}
	if(hF!=HFILE_ERROR)
		_lclose(hF);


}






