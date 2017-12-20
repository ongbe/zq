// MYDEBUG.h: interface for the MYDEBUG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDEBUG_H__AA9439F3_22C7_41A2_97FB_4D6FCB779247__INCLUDED_)
#define AFX_MYDEBUG_H__AA9439F3_22C7_41A2_97FB_4D6FCB779247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "string"
#include "iostream"
#include "ThreadPoolSelfDefine.h"
using namespace std;
class MYDEBUG{
public:
	static void OUTPUT(char *format,...)
	 {
		m_mymutex.Lock();
		va_list ap;
		va_start(ap,format);
		vfprintf(stdout,format,ap);
		va_end(ap);
		fprintf(stdout,"\n");
		m_mymutex.Unlock();
	}
	static	MyMutex  m_mymutex;
};
#endif // !defined(AFX_MYDEBUG_H__AA9439F3_22C7_41A2_97FB_4D6FCB779247__INCLUDED_)
