/*
 * FileOpr.h
 *
 *  It can be used in windows and linux
 *
 *  Created on: Dec 1, 2011
 *      Author: root
 */

#ifndef FILEOPR_H_
#define FILEOPR_H_

#ifndef WIN32
#include <pthread.h>
#endif

#include "easymutex.h"

#define WriteLogMode 1					//write to local file and console


//define file operate method in windows and linux
#ifndef WIN32

#define UNITYPE_FILE int

#define FILE_OPEN_RDONLY(fd,filename) (fd)=open((filename),O_RDONLY);
#define FILE_OPEN_RW(fd,filename) (fd)=open((filename),O_RDWR);
#define FILE_OPEN_RW_CREAT(fd,filename) (fd)=open((filename),O_RDWR);  if((int)(fd)==-1) (fd)=creat((filename),S_IRWXU);
#define FILE_CREATE(fd,filename) (fd)=creat((filename),S_IRWXU);
#define FILE_READ(fd,buf,len) read((fd),(buf),(len));
#define FILE_WRITE(fd,buf,len) write((fd),(buf),(len));
#define FILE_SEEK(rltoffset,fd,offset,origin) (rltoffset)=lseek((fd),(offset),(origin));
#define FILE_SEEK2(fd,offset,origin) lseek((fd),(offset),(origin));
#define FILE_ISVALID(fd) ((fd)==-1?false:true)
#define FILE_CLOSE(fd) close((int)(fd));
#define FILE_CHMOD(filename,mode) chmod((filename),(mode));

#else

#define UNITYPE_FILE FILE*

#define FILE_OPEN_RDONLY(fd,filename) (fd)=fopen((filename),"rb")
#define FILE_OPEN_RW(fd,filename) (fd)=fopen((filename),"rb+");
#define FILE_OPEN_RW_CREAT(fd,filename) (fd)=fopen((filename),"rb+");  if((fd)==NULL) (fd)=fopen((filename),"wb");
#define FILE_CREATE(fd,filename) (fd)=fopen((filename),"wb");
#define FILE_READ(fd,buf,len) fread((buf),(len),1,(FILE*)(fd));
#define FILE_WRITE(fd,buf,len) fwrite((buf),(len),1,(FILE*)(fd));
#define FILE_SEEK(rltoffset,fd,offset,origin) fseek((FILE*)(fd),(offset),(origin));  rltoffset=ftell((FILE*)(fd));
#define FILE_SEEK2(fd,offset,origin) fseek((FILE*)(fd),(offset),(origin));
#define FILE_ISVALID(fd) ((fd)==0?false:true)
#define FILE_CLOSE(fd) fclose((FILE*)(fd));
#define FILE_CHMOD(filename,mode) ;

#endif

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CFileOpr
{
public:
	//create a root with multiple level.
	static bool makemultidir(const char* pathname);

	//write log to local file. the log file exist in the exe file path, and date will be added to tail of logname
	static bool writelocallog(const char* logname,const char* fmt,...);

	//get exe path of current process
	static bool getcurrentexepath(char* buf,int bufsize);

private:
	//convert struct timeb to string formated to "yyyy-mm-dd hh:mm:ss.mmm"
	static char* GetStrTime_internal(struct timeb& sourtimeb,char* prlt);
	//convert time_t to string formated to "yyyymmdd"
	static char* GetStrDate_internal(time_t sourtime,char* prlt);
	//convert time_t to string formated to "hh:mm:ss.mmm"
	static char* GetStrShortTime_internal(struct timeb& sourtimeb,char* prlt);

private:
	static Ceasymutex m_mutex;		//mutex for FileOpr


};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif /* FILEOPR_H_ */
