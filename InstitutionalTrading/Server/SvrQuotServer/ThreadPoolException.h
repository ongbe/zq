// ThreadPoolException.h: interface for the CThreadPoolException class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "string"
using namespace std;
class CThreadPoolException : public exception 
{
public:
	string GetErrorMessage();
	CThreadPoolException(string exceptionInfo);
	virtual ~CThreadPoolException();
private:
	string m_exceptionInfo;
};


