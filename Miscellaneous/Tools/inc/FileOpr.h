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
#include <memory>
using std::auto_ptr;



//define file operate method in windows and linux
#ifndef WIN32

#define UNITYPE_FILE int
#define FILE_UNITYPE int

#define FILE_OPEN_RDONLY(fd,filename) (fd)=open((filename),O_RDONLY);
#define FILE_OPEN_RW(fd,filename) (fd)=open((filename),O_RDWR);
#define FILE_OPEN_RW_CREAT(fd,filename) (fd)=open((filename),O_RDWR);  if((int)(fd)==-1) (fd)=creat((filename),S_IRWXU);
#define FILE_CREATE(fd,filename) (fd)=creat((filename),S_IRWXU);
#define FILE_READ(fd,buf,len) read((fd),(buf),(len));
#define FILE_WRITE(fd,buf,len) write((fd),(buf),(len));
#define FILE_SEEK(rltoffset,fd,offset,origin) (rltoffset)=lseek((fd),(offset),(origin));
#define FILE_SEEK2(fd,offset,origin) lseek((fd),(offset),(origin));
#define FILE_ISVALID(fd) ((fd)==-1?false:true)
#define FILE_INVALIDID -1
#define FILE_CLOSE(fd) close((int)(fd));
#define FILE_CHMOD(filename,mode) chmod((filename),(mode));

#else

#define UNITYPE_FILE FILE*
#define FILE_UNITYPE FILE*

#define FILE_OPEN_RDONLY(fd,filename) (fd)=fopen((filename),"rb")
#define FILE_OPEN_RW(fd,filename) (fd)=fopen((filename),"rb+");
#define FILE_OPEN_RW_CREAT(fd,filename) (fd)=fopen((filename),"rb+");  if((fd)==NULL) (fd)=fopen((filename),"wb");
#define FILE_CREATE(fd,filename) (fd)=fopen((filename),"wb");
#define FILE_READ(fd,buf,len) fread((buf),(len),1,(FILE*)(fd));
#define FILE_WRITE(fd,buf,len) fwrite((buf),(len),1,(FILE*)(fd));
#define FILE_SEEK(rltoffset,fd,offset,origin) fseek((FILE*)(fd),(offset),(origin));  rltoffset=ftell((FILE*)(fd));
#define FILE_SEEK2(fd,offset,origin) fseek((FILE*)(fd),(offset),(origin));
#define FILE_ISVALID(fd) ((fd)==0?false:true)
#define FILE_INVALIDID 0
#define FILE_CLOSE(fd) fclose((FILE*)(fd));
#define FILE_CHMOD(filename,mode) ;

#endif

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CFileOpr
{
    //获取/创建Singleton类. 构造函数为private, 析构函数为public(为了让auto_ptr能析构对象)
    public: static CFileOpr& getObj(void);
    private:static auto_ptr<CFileOpr> m_pObj;
    private: CFileOpr();
    public: virtual ~CFileOpr();

public:
	//write log to local file. the log file exist in the exe file path, and date will be added to tail of logname
	bool writelocallog(const char* logname,const char* fmt,...);
	bool writelocallog2(const char* subpathname,const char* logname,bool bAppendExtName,bool bInsertTime,bool bDelFileFirst,const char* fmt,...);
	bool writelocallog3(const char* logfilenamewithpath,const char* fmt,...);
	//写日志同时printf输出
	bool writelocallog4(const char* logname,const char* fmt,...);
	//append date to logname
	bool writelocallogDaily(const char* subpathname,const char* logname,const char* fmt,...);
	bool writelocallogDaily2(bool bEncrypt,const char* subpathname,const char* logname,const char* fmt,...);
	//printf to conio at the same time
	bool writelocallogDaily3(const char* subpathname,const char* logname,const char* fmt,...);

	bool writelocallog4(const char* subpathname,const char* logname,bool bDaily,bool bEncrypt,bool bAppendExtName,bool bInsertTime,bool bDelFileFirst,const char* fmt,...);

public:

	//output log to outputdebug
	static void putlog_OutputDebugString(const char* fmt,...);

	//get exe path of current process
	static bool getcurrentexepath(char* buf,int bufsize);
    //get exe path of current process, include tail of '\' or '/'
    static bool getcurrentexepath2(char* buf,int bufsize,bool bTailIsSeperator);
	//create a root with multiple level.
	static bool makemultidir(const char* pathname);
	//convert struct timeb to string formated to "yyyy-mm-dd hh:mm:ss.mmm"
	static char* GetStrTime(struct timeb& sourtimeb,char* prlt,int bufsize);
	//convert time_t to string formated to "yyyymmdd"
	static char* GetStrDate(time_t sourtime,char* prlt,int bufsize);
	//convert time_t to string formated to "hh:mm:ss.mmm"
	static char* GetStrShortTime(struct timeb& sourtimeb,char* prlt,int bufsize);

private:

    //bDaily: 是否每天一个日志文件
	bool writelocallog_internal(const char* subpathname,const char* logname,const char* logbuf,bool bDaily,bool bEncrypt=false,bool bAppendExtName=true,bool bInsertTime=true,bool bDelFileFirst=false,bool bPrintf=false,bool blognameincludepath=false);


private:

    Ceasymutex m_mutex;


};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif /* FILEOPR_H_ */
