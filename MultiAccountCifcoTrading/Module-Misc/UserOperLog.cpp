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

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "UserOperLog.h"


// 字符映射表
const int g_AscConv[] = {
	228, 	178, 	68, 	155, 	118, 	177, 	35, 	221, 	126, 	231, 	
	180, 	46, 	161, 	208, 	63, 	249, 	47, 	116, 	182, 	210, 	
	36, 	235, 	176, 	166, 	100, 	58, 	150, 	223, 	237, 	198, 	
	137, 	169, 	81, 	69, 	119, 	234, 	52, 	111, 	127, 	164, 	
	195, 	64, 	136, 	60, 	142, 	101, 	70, 	191, 	192, 	190, 	
	255, 	165, 	230, 	132, 	160, 	187, 	139, 	43, 	92, 	114, 	
	250, 	107, 	88, 	33, 	84, 	233, 	51, 	91, 	144, 	44, 	
	207, 	181, 	93, 	90, 	40, 	227, 	185, 	152, 	240, 	121, 	
	163, 	123, 	143, 	246, 	239, 	124, 	244, 	154, 	77, 	94, 	
	175, 	254, 	85, 	215, 	162, 	170, 	134, 	86, 	98, 	211, 	
	61, 	48, 	120, 	217, 	138, 	95, 	146, 	99, 	213, 	75, 	
	65, 	87, 	197, 	125, 	106, 	140, 	149, 	115, 	96, 	199, 	
	194, 	39, 	97, 	252, 	103, 	109, 	37, 	209, 	245, 	128, 	
	117, 	49, 	67, 	214, 	129, 	151, 	148, 	45, 	41, 	205, 	
	80, 	225, 	168, 	108, 	79, 	174, 	171, 	236, 	38, 	202, 	
	32, 	220, 	229, 	72, 	184, 	110, 	159, 	59, 	226, 	156, 	
	183, 	242, 	189, 	206, 	57, 	248, 	56, 	172, 	73, 	83, 	
	200, 	74, 	53, 	157, 	167, 	89, 	147, 	201, 	42, 	82, 	
	112, 	179, 	131, 	186, 	50, 	158, 	130, 	62, 	141, 	54, 	
	102, 	204, 	105, 	153, 	122, 	145, 	238, 	251, 	203, 	241, 	
	218, 	188, 	212, 	193, 	113, 	135, 	133, 	76, 	104, 	196, 	
	55, 	232, 	66, 	243, 	173, 	216, 	247, 	253, 	78, 	34, 	
	219, 	222, 	224, 	71 };


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserOperLog::CUserOperLog(std::string& strFileName)
{
	m_hMutex = ::CreateMutex(NULL, FALSE, "");
	m_strFileName = strFileName;

	m_TmpBufSize = 65536;
	m_pTmpBuf = (char*)malloc(m_TmpBufSize);
	ZeroMemory(m_pTmpBuf, m_TmpBufSize);
}

CUserOperLog::~CUserOperLog()
{
	::CloseHandle(m_hMutex);

	if(m_pTmpBuf != NULL) {
		free(m_pTmpBuf);
		m_pTmpBuf = NULL;
	}
}

BOOL CUserOperLog::CryptString(char* pString)
{
	if(pString==NULL)
		return FALSE;

	int nIndex = 0;
	BYTE cAsc = 0;
	while(pString[nIndex]!=0) {
		cAsc = pString[nIndex];
		if(cAsc>=32) {
			cAsc = (BYTE)g_AscConv[cAsc-32];
			pString[nIndex] = (char)cAsc;
		}
		nIndex++;
	}
	return TRUE;
}

/******************************************************************************************************
*   @brief					写入一条日志        
*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
*				fmt及其它	类似于sprintf()的参数
*******************************************************************************************************/
void CUserOperLog::WriteLog_Fmt(char* fmt,...)
{
	LockObject();

	va_list argp;
	va_start(argp, fmt);

	ZeroMemory(m_pTmpBuf, sizeof(m_TmpBufSize));
	try
	{
		vsprintf(m_pTmpBuf, fmt, argp);
	}
	catch(...)
	{
		m_pTmpBuf[0]=0;
	}
	m_pTmpBuf[m_TmpBufSize-1]=0;

	va_end(argp);

	CryptString(m_pTmpBuf);

	WriteLog_Internal(m_pTmpBuf);

	UnlockObject();
}


/******************************************************************************************************
*   @brief					将一个字符串写入一条日志，内部调用，不加锁
*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
*				pLog		日志字符串
*******************************************************************************************************/
void CUserOperLog::WriteLog_Internal(char *pLog)
{
	char DateBuf[256];
	HANDLE hFile = NULL;
	SYSTEMTIME CurTime;

	GetLocalTime(&CurTime);
	sprintf(DateBuf,"\r\n\r\n%04d-%02d-%02d %02d:%02d:%02d.%03d : \r\n",
			CurTime.wYear,CurTime.wMonth,CurTime.wDay,
			CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds);
	CryptString(DateBuf);

	hFile = ::CreateFile(m_strFileName.c_str(), 
                       GENERIC_WRITE,			// open for reading
                       FILE_SHARE_READ,			// share for reading
                       NULL,					// default security
                       OPEN_ALWAYS,				// existing file only
                       FILE_ATTRIBUTE_NORMAL,	// normal file
                       NULL);					// no attr. template
	if(hFile == INVALID_HANDLE_VALUE) 
		return;

	DWORD dwHighWrite = 0;
	::SetFilePointer(hFile, 0, NULL, FILE_END);
	::WriteFile(hFile, DateBuf, strlen(DateBuf), &dwHighWrite, NULL);
	::WriteFile(hFile, pLog, strlen(pLog), &dwHighWrite, NULL);
	::CloseHandle(hFile);
}
