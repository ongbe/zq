/*
 * FileOpr.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: root
 */

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#pragma warning(disable : 4793)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include <fcntl.h>


#include "FileOpr.h"
#include "encrypt_charactermap.h"

#ifdef WIN32
#pragma unmanaged
#endif

auto_ptr<CFileOpr> CFileOpr::m_pObj;


//获取类的唯一实例指针。进程生命周期内只会创建一次。
CFileOpr& CFileOpr::getObj(void)
{
#ifdef WIN32

    //用running和InterlockedCompareExchange保证互斥，实现多处理器环境下的线程互斥。volatile避免编译器优化
    static volatile long running=0;
    while(InterlockedCompareExchange(&running,1,0)==1) Sleep(0);
    if(m_pObj.get()==NULL) 
        m_pObj.reset(new CFileOpr());
    InterlockedExchange(&running,0);

#else
#endif
    return *m_pObj;
}
CFileOpr::CFileOpr() 
{
}
CFileOpr::~CFileOpr()
{
    m_pObj.release();
}

//write log to local file.
//the log file exist in the exe file path,
//and date will be added to tail of logname
bool CFileOpr::writelocallog(const char* logname,const char* fmt,...)
{
	va_list ap;
	char logbuf[1024];

	logbuf[sizeof(logbuf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(logbuf,sizeof(logbuf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);
	return writelocallog_internal("log",logname,logbuf,false,false,true,true,false);
}

bool CFileOpr::writelocallog2(const char* subpathname,const char* logname,bool bAppendExtName,bool bInsertTime,bool bDelFileFirst,const char* fmt,...)
{
	va_list ap;
	char logbuf[1024];

	logbuf[sizeof(logbuf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(logbuf,sizeof(logbuf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);
	return writelocallog_internal(subpathname,logname,logbuf,false,false,bAppendExtName,bInsertTime,bDelFileFirst);
}

//append date to logname
bool CFileOpr::writelocallogDaily(const char* subpathname,const char* logname,const char* fmt,...)
{
	va_list ap;
	char logbuf[1024];

	logbuf[sizeof(logbuf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(logbuf,sizeof(logbuf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);
	return writelocallog_internal(subpathname,logname,logbuf,true,false,true,true,false);
}
bool CFileOpr::writelocallogDaily2(bool bEncrypt,const char* subpathname,const char* logname,const char* fmt,...)
{
	va_list ap;
	char logbuf[1024];

	logbuf[sizeof(logbuf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(logbuf,sizeof(logbuf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);
	return writelocallog_internal(subpathname,logname,logbuf,true,bEncrypt,true,true,false);
}
//printf to conio at the same time
bool CFileOpr::writelocallogDaily3(const char* subpathname,const char* logname,const char* fmt,...)
{
	va_list ap;
	char logbuf[1024];

	logbuf[sizeof(logbuf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(logbuf,sizeof(logbuf)-1,fmt,ap);
	va_end(ap);

	printf("%s\n",logbuf);

    CeasymutexGuard guard(m_mutex);
	return writelocallog_internal(subpathname,logname,logbuf,true,false,true,true,false);
}

//bDaily: 是否每天一个日志文件
bool CFileOpr::writelocallog_internal(const char* subpathname,const char* logname,const char* logbuf,bool bDaily,bool bEncrypt,bool bAppendExtName,bool bInsertTime,bool bDelFileFirst)
{
	char filename[512];
	char pathname[512];
	char strdate[64];
	int len;
	UNITYPE_FILE fd;
	struct timeb  curtimeb;
	//struct tm curtm;
#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif

	pathname[sizeof(pathname)-1]=0;
	filename[sizeof(filename)-1]=0;

	//get current working directory
#ifndef WIN32
	if(getcwd(pathname,sizeof(pathname)-1)==NULL)
		return false;
#else
	getcurrentexepath(pathname,sizeof(pathname)-1);
#endif

	len=strlen(pathname);
	if(len+32+strlen(logname)>=sizeof(filename))
		return false;
    if(!subpathname) sprintf(pathname+strlen(pathname),"%clog",seperator);
    else sprintf(pathname+strlen(pathname),"%c%s",seperator,subpathname);
	sprintf(filename,"%s%c%s",pathname,seperator,logname);

	ftime(&curtimeb);
	GetStrDate(curtimeb.time,strdate,sizeof(strdate));		//get string that formated yyyymmdd
	//memcpy((char*)&curtm,(char*)localtime(&(curtimeb.time)),sizeof(curtm));

	if(bDaily)
	{
		int len=strlen(filename);
		strncpy(filename+len,strdate,sizeof(filename)-1-len);
	}
	if(bAppendExtName&&(int)strlen(filename)<(int)sizeof(filename)-4) strcat(filename,".log");


	if(!bDelFileFirst)
	{
		FILE_OPEN_RW(fd,filename);
	}
	else
	{
		FILE_CREATE(fd,filename);
	}

	if(!FILE_ISVALID(fd))
	{
		FILE_CREATE(fd,filename);
	}
	if(!FILE_ISVALID(fd))
	{
#ifdef WIN32
		if(makemultidir(pathname)==true)
			FILE_CREATE(fd,filename);
#else
		if(access(filename,F_OK)==-1)
		{
			if(access(pathname,F_OK)==0||makemultidir(pathname)==true)
				fd=creat(filename,S_IRWXU);
		}
#endif
	}
	if(FILE_ISVALID(fd))
	{
		FILE_SEEK2(fd,0,2);

		if(bInsertTime)
		{
			if(bDaily) GetStrShortTime(curtimeb,strdate,sizeof(strdate));		//get string that formated hh:mm:ss.mmm
			else GetStrTime(curtimeb,strdate,sizeof(strdate));					//get string that formated yyyy-mm-dd hh:mm:ss.mmm
			strcat(strdate," : ");
			if(bEncrypt) CEncrypt_Charactermap::getObj().EncryptString(strdate);
			FILE_WRITE(fd,strdate,strlen(strdate));

			if(WriteLogMode==1)
			{
				GetStrShortTime(curtimeb,strdate,sizeof(strdate));
				strcat(strdate," : ");
				printf("%s",strdate);
			}
		}


        if(bEncrypt) CEncrypt_Charactermap::getObj().EncryptString(const_cast<char*>(logbuf));
		FILE_WRITE(fd,logbuf,strlen(logbuf));
        if(bEncrypt) CEncrypt_Charactermap::getObj().DecryptString(const_cast<char*>(logbuf));

        if(WriteLogMode==1)
			printf("%s",logbuf);

		FILE_WRITE(fd,"\r\n",2);
		if(WriteLogMode==1)
			printf("\n");

		FILE_CLOSE(fd);
	}


	if(!FILE_ISVALID(fd)) return false;
	else return true;
}

//output log to outputdebug
void CFileOpr::putlog_OutputDebugString(const char* fmt,...)
{
#ifndef WIN32
	return false;
#endif

	va_list ap;
	char logbuf[1024];
	int len;
	struct timeb  curtimeb;

	logbuf[sizeof(logbuf)-1]=0;
	ftime(&curtimeb);
	GetStrShortTime(curtimeb,logbuf,sizeof(logbuf));
	strcat(logbuf," : ");
	len=strlen(logbuf);

	va_start(ap,fmt);
	len=vsnprintf(logbuf+len,sizeof(logbuf)-len-1,fmt,ap);
	va_end(ap);

	strcat(logbuf,"\n");
	OutputDebugString(logbuf);
}


//get exe path of current process
bool CFileOpr::getcurrentexepath(char* buf,int bufsize)
{
	int len=0;
	memset(buf,0,bufsize);
#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif

#ifndef WIN32
	len=readlink("/proc/self/exe",buf,bufsize-1);
#else
	GetModuleFileName( NULL, buf, bufsize-1 );
	len=strlen(buf);
#endif

	if(len<=0)
	{
		buf[0]=0;
		return false;
	}
	else
	{
		while(len>0&&buf[len-1]!=seperator)
			len--;
		if(len>0&&buf[len-1]==seperator)
			len--;
		buf[len]=0;
		return  true;
	}
}

//create a root with multiple level.
//pathname like "/mnt/shared". you shouldn't use string like "/mnt/../shared"
bool CFileOpr::makemultidir(const char* pathname)
{
	int off,level,len;
	char buf[1024];

#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif

	if(!pathname) return false;

	len=strlen(pathname);
	if(len>(int)sizeof(buf)-2) return false;
	off=0;
	level=0;

	while(off<len&&level<20)
	{
		while(off<len&&(off==0||pathname[off]!=seperator)) off++;
		memcpy(buf,pathname,off);
		buf[off]=0;

#ifdef WIN32
		CreateDirectory(buf,NULL);
#else
		//check whether the dir exist
		if(access(buf,F_OK)==-1)
		{
			int ret=mkdir(buf,S_IRWXU | S_IRWXG | S_IRWXO);
			if(ret==-1) return false;
		}
#endif
		off++;
		level++;
	}
	return true;
}

//convert struct timeb to string formated to "yyyy-mm-dd hh:mm:ss.mmm"
char* CFileOpr::GetStrTime(struct timeb& sourtimeb,char* prlt,int bufsize)
{
	struct tm tmptm;
	memcpy((char*)&tmptm,(char*)localtime(&(sourtimeb.time)),sizeof(tmptm));
	_snprintf(prlt,bufsize-1,"%04d-%02d-%02d  %02d:%02d:%02d.%03d",
			tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday,tmptm.tm_hour,tmptm.tm_min,tmptm.tm_sec,sourtimeb.millitm);
    prlt[bufsize-1]=0;
	return prlt;
}

//convert time_t to string formated to "yyyymmdd"
char* CFileOpr::GetStrDate(time_t sourtime,char* prlt,int bufsize)
{
	struct tm tmptm;
	memcpy((char*)&tmptm,(char*)localtime(&sourtime),sizeof(tmptm));
	_snprintf(prlt,bufsize-1,"%04d%02d%02d",
			tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday);
    prlt[bufsize-1]=0;
	return prlt;
}

//convert time_t to string formated to "hh:mm:ss.mmm"
char* CFileOpr::GetStrShortTime(struct timeb& sourtimeb,char* prlt,int bufsize)
{
	if(sourtimeb.time==0)
	{
		strncpy(prlt,"00:00:00.000",bufsize-1);
        prlt[bufsize-1]=0;
		return prlt;
	}
	else
	{
		struct tm tmptm;
		memcpy((char*)&tmptm,(char*)localtime(&(sourtimeb.time)),sizeof(tmptm));
		_snprintf(prlt,bufsize-1,"%02d:%02d:%02d.%03d",
				tmptm.tm_hour,tmptm.tm_min,tmptm.tm_sec,sourtimeb.millitm);
        prlt[bufsize-1]=0;
		return prlt;
	}
}

