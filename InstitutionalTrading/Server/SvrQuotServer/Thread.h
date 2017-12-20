// Thread.h: interface for the CThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "cstring"
#include "iostream"
#include "windows.h"
//#include <afx.h>
class CThread  
{

private:
	int			m_ErrCode;
	char*		m_ThreadName;
protected:
	void		SetErrcode(int errcode){m_ErrCode = errcode;}
	static DWORD WINAPI _ThreadFunction(LPVOID lpParam);
	HANDLE		m_ThreadHandle;
public:
	DWORD GetThreadIdReal();
	BOOL ThreadRun(LPVOID lpParam);
	CThread();
	virtual ~CThread();
	virtual void	Run()=0;
	HANDLE			Start(CThread* cthread);
	int				GetLastError(void){return m_ErrCode;}
	void			SetThreadName(char* thrname){strcpy(m_ThreadName,thrname);}
	char*			GetThreadName(void){return m_ThreadName;}
	HANDLE	GetThreadHandle(void){return m_ThreadHandle;}
	DWORD			m_ThreadIDReal;
};

