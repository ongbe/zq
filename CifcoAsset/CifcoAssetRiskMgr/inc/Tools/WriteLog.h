// WriteLog.h: interface for the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITELOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
#define AFX_WRITELOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)
#pragma warning(disable : 4793)


#include "easymutex.h"


//日志记录模式定义
enum TYPE_WriteLogMode
{
	WriteLogMode_LOCALFILE				= 1					//使用本地文件日志记录模式
};



//日志级别定义
enum TYPE_WriteLogLevel
{
	WriteLogLevel_RUNNINGINFO			= 1,				//运行信息
	WriteLogLevel_DEBUGINFO				= 2,				//调试信息
	WriteLogLevel_ALERTINFO				=-1,				//一般告警信息
	WriteLogLevel_CRITICALINFO			=-2,				//严重告警信息
	WriteLogLevel_RISKINFO				=-3					//致命告警信息
};

#define MAX_PATH_WriteLog 1024
class CWriteLog  
{
public:
	/******************************************************************************************************
	*   @brief					构造函数，指定日志模式        
	*   @param[in]	LogMode		日志模式，可以是LOGMODE_xxxx
	*				pFileName	如果是本地日志模式，指定本地文件名，带路径。
	*******************************************************************************************************/
	CWriteLog(TYPE_WriteLogMode LogMode=WriteLogMode_LOCALFILE,char* pFileName=NULL);

	/******************************************************************************************************
	*   @brief					析构函数，释放资源        
	*******************************************************************************************************/
	virtual ~CWriteLog();


	/******************************************************************************************************
	*   @brief					写入一条日志        
	*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
	*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
	*				fmt及其它	类似于sprintf()的参数
	*******************************************************************************************************/
	void WriteLog_Fmt(char* pLogName,int LogLevel,char* fmt,...);




private:
	TYPE_WriteLogMode	m_LogMode;							//日志模式
	char	m_pLocalFileName[MAX_PATH_WriteLog];			//Log文件带路径的文件名
	char	m_pLocalFileDir[MAX_PATH_WriteLog];				//Log文件目录
	char	*m_pTmpBuf;										//临时缓冲区
	int		m_TmpBufSize;									//临时缓冲区的尺寸

	Ceasymutex	m_Sem;										//读写互斥锁



	/******************************************************************************************************
	*   @brief					设置日志模式为本地文件日志
	*   @param[in]	pFileName	本地日志文件名
	*******************************************************************************************************/
	void SetLogMode_LocalFile(char *pFileName=NULL);


	/******************************************************************************************************
	*   @brief					讲一个字符串写入一条日志，内部调用，不加锁        
	*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
	*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
	*				pLog		日志字符串
	*******************************************************************************************************/
	void WriteLog_Internal(char* pLogName,int LogLevel,char *pLog);




public:
															//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CWriteLog(const CWriteLog&);									
	CWriteLog& operator= (const CWriteLog&);				


};

#endif // !defined(AFX_WRITELOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
