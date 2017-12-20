//*********************************************************************
//	统一日志接口
//
//	1. 定义了统一的日志写入接口
//	2. DLL
//	3. 可以供C++/Java程序调用
//
//	history 
//	20100113	lzq		创建该文件
//*********************************************************************


#pragma once

//#include "toolpublic.h"


//#ifdef WRITELOG_EXPORTS
//#define WRITELOG_API __declspec(dllexport)
//#else
//#define WRITELOG_API __declspec(dllimport)
//#endif

//日志记录模式定义
#define LOGMODE_DEFAULT					0					//使用缺省日志记录模式
#define LOGMODE_LOCALFILE				1					//使用本地文件日志记录模式


//日志级别定义
#define LOGLEVEL_RUNNINGINFO			1					//运行信息
#define LOGLEVEL_DEBUGINFO				2					//调试信息
#define LOGLEVEL_ALERTINFO				-1					//一般告警信息
#define LOGLEVEL_CRITICALINFO			-2					//严重告警信息
#define LOGLEVEL_RISKINFO				-3					//致命告警信息


//同步操作的Mutex名
#define MUTEXNAME						"Mutex_CWriteLog"	//Mutex的名称


//日志写入类定义
class DllWriteLog
{
public:

	//构造函数，初始化日志模式为Default，这是推荐的构造函数
	DllWriteLog(void);

	//构造函数，指定日志模式
	//Param:iLogMode：	日志模式，可以是LOGMODE_xxxx
	//		pFileName:	如果是本地日志模式，指定本地文件名，带路径。
	DllWriteLog(int iLogMode,char* pFileName=NULL);

	//析构函数，释放资源
	~DllWriteLog(void);




	//写入一条日志
	//Param:pLogName:	日志名称，用于日志分类。每个模块可以定义自己的日志名称
	//		LogLevel:	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
	//		fmt及后续参数:类似于sprintf()的参数
	void WriteLog_Fmt(char* pLogName,int LogLevel,char* fmt,...);


	//是否能写log
	static void EnableWriteLog(bool bEnable);
private:
	static bool m_EnableLog;
	int m_LogMode;											//日志模式
	char m_pLocalFileName[MAX_PATH];						//Log文件带路径的文件名
	char m_pLocalFileDir[MAX_PATH];							//Log文件目录

	int m_DefaultLogMode;									//缺省的日志记录模式
	char m_pDefaultFileDir[MAX_PATH];						//缺省为本地文件模式时，Log文件目录



	//设置日志模式为本地文件日志
	//Param:pFileName:	本地日志文件名
	void SetLogMode_LocalFile(char *pFileName);
	//
	bool GetPureFileName(char* pFileNameWithPath,char* pRltFileName,int RltSize);
	//从全路径文件名中取出盘符，标志是:
	bool GetDiskSymbol(char* pFileName,char* pRltDiskSymbol,int RltSize);

	//从全路径文件名种取出路径，不带盘符
	bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);
	//写入一条日志
	//Param:pLogName:	日志名称，用于日志分类。每个模块可以定义自己的日志名称
	//		LogLevel:	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
	//		pLog:		日志字符串
	void WriteLog(char* pLogName,int LogLevel,char *pLog);


public:
	DllWriteLog(const DllWriteLog&);									
	DllWriteLog& operator= (const DllWriteLog&);				

};


