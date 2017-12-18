

#pragma once

#ifndef LogLock1
#if 1
#define LogLock1 
#else
#define LogLock1 \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,"..lock1: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif
#endif

#ifndef LogLock2
#if 1
#define LogLock2 
#else
#define LogLock2 \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,"..lock2: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif
#endif

#ifndef LogUnlock
#if 1
#define LogUnlock 
#else
#define LogUnlock \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,".unlock: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif
#endif

#ifndef Lock_CPlatformDataMgr
#define Lock_CPlatformDataMgr \
                { \
                    LogLock1  \
                    m_mutex.lock();\
                    LogLock2 \
                }
#endif

#ifndef Unlock_CPlatformDataMgr
#define Unlock_CPlatformDataMgr \
                { \
                    LogUnlock  \
                    m_mutex.unlock();\
                }
#endif


#ifndef LOG_INFO

#if 1

#define LOG_INFO(fmt, ...) 

#else

#ifndef __CUSTOM_FILE__
#define __CUSTOM_FILE__    (string(__FILE__).substr(string(__FILE__).rfind('\\')+1).c_str())
#endif

#define LOG_INFO(fmt, ...) \
    {\
        char logbuf[512]={0},logbuf2[512]={0};\
        CFileOpr::getObj().writelocallog(__CUSTOM_FILE__, "[%d]:"fmt, __LINE__, __VA_ARGS__);\
	}

#endif

#endif
