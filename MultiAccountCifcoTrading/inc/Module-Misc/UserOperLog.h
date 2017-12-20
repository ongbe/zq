// UserOperLog.h: interface for the CUserOperLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSEROPERLOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
#define AFX_CUSEROPERLOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)


#include <windows.h>
#include <queue>
#include <string>


class CUserOperLog  
{
public:
	/******************************************************************************************************
	*   @brief					构造函数，指定日志模式        
	*				pFileName	如果是本地日志模式，指定本地文件名，带路径。
	*******************************************************************************************************/
	CUserOperLog(std::string& strFileName);

	/******************************************************************************************************
	*   @brief					析构函数，释放资源        
	*******************************************************************************************************/
	virtual ~CUserOperLog();

	/******************************************************************************************************
	*   @brief					写入一条日志        
	*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
	*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
	*				fmt及其它	类似于sprintf()的参数
	*******************************************************************************************************/
	void WriteLog_Fmt(char* fmt,...);

private:
	HANDLE			m_hMutex;
	std::string		m_strFileName;		//Log文件带路径的文件名
	char			*m_pTmpBuf;			//临时缓冲区
	int				m_TmpBufSize;		//临时缓冲区的尺寸

	/******************************************************************************************************
	*   @brief					讲一个字符串写入一条日志，内部调用，不加锁        
	*   @param[in]	pLogName	日志名称，用于日志分类。每个模块可以定义自己的日志名称
	*				LogLevel	日志级别。可以是LOGLEVEL_xxxx，在日志告警时，日志级别对应不同的告警方式
	*				pLog		日志字符串
	*******************************************************************************************************/
	void WriteLog_Internal(char *pLog);

	BOOL CryptString(char* pString);

	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		::WaitForSingleObject(m_hMutex, INFINITE);
	};
	void UnlockObject() {
		::ReleaseMutex(m_hMutex);
	};


public:
	//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CUserOperLog(const CUserOperLog&);									
	CUserOperLog& operator= (const CUserOperLog&);
};

#endif // !defined(AFX_CUSEROPERLOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
