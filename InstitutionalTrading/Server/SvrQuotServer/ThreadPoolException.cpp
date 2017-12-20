// ThreadPoolException.cpp: implementation of the CThreadPoolException class.
//
//////////////////////////////////////////////////////////////////////

#include "ThreadPoolException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadPoolException::CThreadPoolException(string exceptionInfo)
{
m_exceptionInfo = exceptionInfo;
}

CThreadPoolException::~CThreadPoolException()
{

}

string CThreadPoolException::GetErrorMessage()
{
	return m_exceptionInfo;
}
