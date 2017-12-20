#ifndef THREADPOOLSELFDEFINE_H
#define THREADPOOLSELFDEFINE_H
#include "string"
//#include <afx.h>
#include "windows.h"
using namespace std;
typedef  DWORD  ( WINAPI * JOBCALLBACKE)(LPVOID lpParam);
typedef struct tagJobInfo{	//回调函数指针及参数
	JOBCALLBACKE		jobcallback;
	unsigned char		*lpParam;
	unsigned int		length;
}JobInfo;

typedef enum tagJobPriority //线程优先级
{
	HIGH_PRIORITY,
	NORMAL_PRIORITY
}JobPriority;
typedef struct tagTest{		//测试传递参数
	int a;
	int b;
}TestStruct;

class MyMutex{
	HANDLE	m_out;
public:
	MyMutex(){
		m_out = CreateMutex(NULL,FALSE,NULL);
	}
	~MyMutex(){
		ReleaseMutex(m_out);
		m_out = NULL;	// 没有关闭，会泄漏
	}
 void Lock(){ WaitForSingleObject(m_out, INFINITE); }
 void Unlock(){ ReleaseMutex(m_out); }
};
#endif	THREADPOOLSELFDEFINE_H
