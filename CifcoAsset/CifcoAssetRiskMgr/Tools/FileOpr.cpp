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

#ifdef WIN32
#pragma unmanaged
#endif

Ceasymutex CFileOpr::m_mutex;		//mutex for FileOpr

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

//write log to local file.
//the log file exist in the exe file path,
//and date will be added to tail of logname
bool CFileOpr::writelocallog(const char* logname,const char* fmt,...)
{
	va_list ap;
	char logbuf[1024];
	char filename[512];
	char pathname[512];
	char strdate[64];
	int len;
	UNITYPE_FILE fd;
	struct timeb  curtimeb;
	struct tm curtm;
#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif

	logbuf[sizeof(logbuf)-1]=0;
	va_start(ap,fmt);
	len=vsnprintf(logbuf,sizeof(logbuf)-1,fmt,ap);
	va_end(ap);

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
	sprintf(pathname+strlen(pathname),"%clog",seperator);
	sprintf(filename,"%s%c%s.log",pathname,seperator,logname);

	ftime(&curtimeb);
	memcpy((char*)&curtm,(char*)localtime(&(curtimeb.time)),sizeof(curtm));
	GetStrDate_internal(curtimeb.time,strdate);		//get string that formated yyyymmdd

	m_mutex.lock();

	FILE_OPEN_RW(fd,filename);
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
		GetStrTime_internal(curtimeb,strdate);		//get string that formated yyyy-mm-dd hh:mm:ss.mmm
		strcat(strdate," : ");
		FILE_WRITE(fd,strdate,strlen(strdate));

		if(WriteLogMode==1)
		{
			GetStrShortTime_internal(curtimeb,strdate);
			strcat(strdate," : ");
			printf("%s",strdate);
		}

		FILE_WRITE(fd,logbuf,strlen(logbuf));
		if(WriteLogMode==1)
			printf("%s",logbuf);

		FILE_WRITE(fd,"\r\n",2);
		if(WriteLogMode==1)
			printf("\n");

		FILE_CLOSE(fd);
	}

	m_mutex.unlock();

	if(!FILE_ISVALID(fd)) return false;
	else return true;
}

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




//convert struct timeb to string formated to "yyyy-mm-dd hh:mm:ss.mmm"
char* CFileOpr::GetStrTime_internal(struct timeb& sourtimeb,char* prlt)
{
	struct tm tmptm;
	memcpy((char*)&tmptm,(char*)localtime(&(sourtimeb.time)),sizeof(tmptm));
	sprintf(prlt,"%04d-%02d-%02d  %02d:%02d:%02d.%03d",
			tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday,tmptm.tm_hour,tmptm.tm_min,tmptm.tm_sec,sourtimeb.millitm);
	return prlt;
}

//convert time_t to string formated to "yyyymmdd"
char* CFileOpr::GetStrDate_internal(time_t sourtime,char* prlt)
{
	struct tm tmptm;
	memcpy((char*)&tmptm,(char*)localtime(&sourtime),sizeof(tmptm));
	sprintf(prlt,"%04d%02d%02d",
			tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday);
	return prlt;
}

//convert time_t to string formated to "hh:mm:ss.mmm"
char* CFileOpr::GetStrShortTime_internal(struct timeb& sourtimeb,char* prlt)
{
	if(sourtimeb.time==0)
	{
		strcpy(prlt,"00:00:00.000");
		return prlt;
	}
	else
	{
		struct tm tmptm;
		memcpy((char*)&tmptm,(char*)localtime(&(sourtimeb.time)),sizeof(tmptm));
		sprintf(prlt,"%02d:%02d:%02d.%03d",
				tmptm.tm_hour,tmptm.tm_min,tmptm.tm_sec,sourtimeb.millitm);
		return prlt;
	}
}

