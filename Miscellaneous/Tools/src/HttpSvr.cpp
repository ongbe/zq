/*
 * HttpSvr.cpp
 *
 *  http请求包：
    "POST /index.jsp HTTP/1.1\r\n"//注意\r\n为回车换行
    "Accept-Language: zh-cn\r\n"
    "Connection: Keep-Alive\r\n"
    "Host: 192.168.0.106\r\n"
    "Content-Length: 37\r\n"
    "\r\n"
    "userName=new_andy&password=new_andy\r\n"
    "\r\n");

 *  http应答包
    "HTTP/1.1 200 OK\r\n"
    "Date: Tue, 14 Sep 1999 02:19:57 GMT\r\n"
    "Server: Apache/1.2.6\r\n"
    "Connection: close\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "<html><head><title>解读HTTP包示例</title></head><body>test</body></html>\r\n"
 *
 *
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
#include <time.h>
#include <sys/timeb.h>
#include <sys/stat.h>

#include "HttpSvr.h"
#include "FileOpr.h"
#include "tools_ansi.h"
#include "tools_win32.h"
#include "myxml.h"

#ifdef WIN32
#pragma unmanaged
#endif

//****************************************************************
//  Realization of CHttpSvrObj
//****************************************************************

CHttpSvrObj::CHttpSvrObj(const char *pWebPath,const char *pDefWeb)
{
    CeasymutexGuard guard(m_mutex);

    memset(m_WebPath,0,sizeof(m_WebPath));
	CFileOpr::getcurrentexepath2(m_WebPath,sizeof(m_WebPath)-2,true);
    if(pWebPath)
    {
        strncpy(m_WebPath+strlen(m_WebPath),pWebPath,sizeof(m_WebPath)-strlen(m_WebPath)-2);
#ifdef WIN32
        if(strlen(m_WebPath)>0&&m_WebPath[strlen(m_WebPath)-1]!='\\') strcat(m_WebPath,"\\");
#else
        if(strlen(m_WebPath)>0&&m_WebPath[strlen(m_WebPath)-1]!='/') strcat(m_WebPath,"/");
#endif
    }

	memset(m_DefWeb,0,sizeof(m_DefWeb));
	if(pDefWeb) strncpy(m_DefWeb,pDefWeb,sizeof(m_DefWeb)-1);

    m_XhrReqTime=0;
    m_bXhrReqResponsed=false;

    m_pRcvData=(char*)malloc(RcvDataSize);
    m_RcvDataLen=0;
    m_pRspHttpHead=(char*)malloc(RspHttpHeadSize);
    m_RspHttpHeadLen=0;
    m_pRspHttpBody=(char*)malloc(RspHttpBodySize);
    m_RspHttpBodyLen=0;
}

CHttpSvrObj::CHttpSvrObj(const CHttpSvrObj& r) 
{
    m_pRcvData=(char*)malloc(RcvDataSize);
    m_RcvDataLen=0;
    m_pRspHttpHead=(char*)malloc(RspHttpHeadSize);
    m_RspHttpHeadLen=0;
    m_pRspHttpBody=(char*)malloc(RspHttpBodySize);
    m_RspHttpBodyLen=0;

    *this=r;
}

CHttpSvrObj::~CHttpSvrObj()
{
    if(m_pRcvData)
    {
        free(m_pRcvData);
        m_pRcvData=NULL;
    }
    if(m_pRspHttpHead)
    {
        free(m_pRspHttpHead);
        m_pRspHttpHead=NULL;
    }
    if(m_pRspHttpBody)
    {
        free(m_pRspHttpBody);
        m_pRspHttpBody=NULL;
    }
}

CHttpSvrObj& CHttpSvrObj::operator= (const CHttpSvrObj& r)
{
    memcpy(m_WebPath,r.m_WebPath,sizeof(m_WebPath));
    memcpy(m_DefWeb,r.m_DefWeb,sizeof(m_DefWeb));
    m_XhrReqHead=r.m_XhrReqHead;
    m_XhrReqTime=r.m_XhrReqTime;
    m_bXhrReqResponsed=r.m_bXhrReqResponsed;

    m_RcvDataLen=r.m_RcvDataLen;
    if(m_pRcvData&&r.m_pRcvData)
        memcpy(m_pRcvData,r.m_pRcvData,RcvDataSize);

    m_RspHttpHeadLen=r.m_RspHttpHeadLen;
    if(m_pRspHttpHead&&r.m_pRspHttpHead)
        memcpy(m_pRspHttpHead,r.m_pRspHttpHead,RspHttpHeadSize);

    m_RspHttpBodyLen=r.m_RspHttpBodyLen;
    if(m_pRspHttpBody&&r.m_pRspHttpBody)
        memcpy(m_pRspHttpBody,r.m_pRspHttpBody,RspHttpBodySize);

    return *this;
};

//process a request of http, refer to html. Don't response request that begin with CXhr_
bool CHttpSvrObj::ProcessReqPkg_Html(const Stru_HttpRequestPkg& ReqPkg,
                                     char* prlthead,int rltheadsize,int& rltheadlen,
                                     char* prltbody,int rltbodysize,int& rltbodylen)
{
    int				i;
	int				ResponseFileLen;
	char			Buf2[256];
	time_t			CurTime;
	struct tm		CurGMTTime,*pltime;
	bool			bRlt;

	//if it's a xhr request, return directly
	if(ReqPkg.Header.bXhr)
		return false;

	time(&CurTime);
	pltime=gmtime(&CurTime);
	memcpy((char*)&CurGMTTime,(char*)pltime,sizeof(struct tm));

	bRlt=false;
	FILE_UNITYPE Responsefd=FILE_INVALIDID;
    rltheadlen=rltbodylen=0;
    prlthead[0]=prlthead[rltheadsize-1]=prltbody[0]=prltbody[rltbodysize-1]=0;


    if(stricmp(ReqPkg.Header.Method.c_str(),"GET")==0)
	{
		//client request to get special document

        string			strReqFileName;
		if(ReqPkg.Header.Request_URL==string("/"))
        {
            strReqFileName=string(m_WebPath)+string(m_DefWeb);
        }
		else
		{
            for(i=0;i<(int)ReqPkg.Header.Request_URL.size()&&(ReqPkg.Header.Request_URL.c_str()[i]=='/'||ReqPkg.Header.Request_URL.c_str()[i]=='\\');i++);
            strReqFileName=string(m_WebPath)+ReqPkg.Header.Request_URL.substr(i);
		}

        FILE_OPEN_RDONLY(Responsefd,strReqFileName.c_str());

		if(!FILE_ISVALID(Responsefd))
		{
			//The file didn't exist, response 404
            sprintf_s(prlthead,rltheadsize-1,
                "%s 404 Not Found\r\n"
                "Date: %s\r\n"
                "Server: HttpSvr/0.2\r\n"
                "Connection: %s\r\n"
                "Content-Length: 0\r\n"
                "\r\n",
                ReqPkg.Header.HttpVersion.c_str(),
                CTools_Ansi::Get_strTime_from_utc3(CurTime).c_str(),
                ReqPkg.Header.Connection.c_str());
            rltheadlen=strlen(prlthead);
			bRlt=true;
		}
		else
		{
			//The file exist
			struct stat					FileStat;
			struct tm					FileGMTTime,IfModifiedSinceGMTTime;
			time_t						time1,time2;

			memset((char*)&FileStat,0,(int)sizeof(FileStat));
            stat(strReqFileName.c_str(),&FileStat);
			pltime=gmtime(&FileStat.st_mtime);
			memcpy((char*)&FileGMTTime,(char*)pltime,sizeof(struct tm));

			bool bNeedTransFile=true;
            if(!ReqPkg.Header.If_Modified_Since.empty())
			{
                ConvertStrToDate((char*)ReqPkg.Header.If_Modified_Since.c_str(),&IfModifiedSinceGMTTime);
				time1=mktime(&IfModifiedSinceGMTTime);
				time2=mktime(&FileGMTTime);
				if(time1>=time2)
					bNeedTransFile=false;
			}

			if(!bNeedTransFile)
			{
				//The file havn't refresh, response code 304
                sprintf_s(prlthead,rltheadsize-1,
                    "%s 304 Not Modified\r\n"
				    "Date: %s\r\n"
				    "Server: HttpSvr/0.2\r\n"
				    "Connection: %s\r\n"
				    "Content-Length: 0\r\n"
				    "\r\n",
                    ReqPkg.Header.HttpVersion,
                    CTools_Ansi::Get_strTime_from_utc3(CurTime).c_str(),
                    ReqPkg.Header.Connection);
                 rltheadlen=strlen(prlthead);
			     bRlt=true;
			}
			else
			{
				//The file will be transfer to client, response code 200
                FILE_SEEK(ResponseFileLen,Responsefd,0,2);

                sprintf_s(prlthead,rltheadsize-1,
                    "%s 200 OK\r\n"
				    "Date: %s\r\n"
				    "Server: HttpSvr/0.2\r\n"
				    "Connection: %s\r\n"
				    "Content-type: %s\r\n"
				    "Last-Modified: %s\r\n"
				    "Content-Length: %d\r\n"
				    "\r\n",
                    ReqPkg.Header.HttpVersion.c_str(),
                    CTools_Ansi::Get_strTime_from_utc3(CurTime).c_str(),
                    ReqPkg.Header.Connection.c_str(),
                    ConvertFileTypeToStr(strReqFileName.c_str(),Buf2),
                    CTools_Ansi::Get_strTime_from_utc3(CurTime).c_str(),
                    ResponseFileLen);
                rltheadlen=strlen(prlthead);

                rltbodylen = ResponseFileLen<rltbodysize? ResponseFileLen : rltbodysize-1;
                FILE_SEEK2(Responsefd,0,0);
                FILE_READ(Responsefd,prltbody,rltbodylen);
                bRlt=true;
			}
		}
	}
	else
	{
		//For other method, return ok directly, without detail entity contont
        sprintf_s(prlthead,rltheadsize-1,
		    "%s 200 OK\r\n"
		    "Date: %s\r\n"
		    "Server: HttpSvr/0.2\r\n"
		    "Connection: %s\r\n"
		    "Content-Length: 0\r\n"
		    "\r\n",
            ReqPkg.Header.HttpVersion.c_str(),
            CTools_Ansi::Get_strTime_from_utc3(CurTime).c_str(),
            ReqPkg.Header.Connection.c_str());
        rltheadlen=strlen(prlthead);
		bRlt=true;
	}

	if(FILE_ISVALID(Responsefd))
	{
		FILE_CLOSE(Responsefd);
		Responsefd=FILE_INVALIDID;
	}

    if(!bRlt) rltheadlen=rltbodylen=0;

	return bRlt;
}

//this routine was called to notify a xhr reach
bool CHttpSvrObj::ProcessReqPkg_XhrArrival(const Stru_HttpRequestPkgHeader& XhrHead)
{
    m_XhrReqHead=XhrHead;
    m_XhrReqTime=time(NULL);
    m_bXhrReqResponsed=false;
    return true;
}

//process a request of Xhr
bool CHttpSvrObj::ProcessReqPkg_XhrOntime(char* prlthead,int rltheadsize,int& rltheadlen,
        char* prltbody,int rltbodysize,int& rltbodylen)
{
	time_t			curtime=time(NULL);
	struct tm		CurGMTTime,*pltime;

	pltime=gmtime(&curtime);
	memcpy((char*)&CurGMTTime,(char*)pltime,sizeof(struct tm));

    rltheadlen=rltbodylen=0;
    prlthead[0]=prlthead[rltheadsize-1]=prltbody[0]=prltbody[rltbodysize-1]=0;
    bool brlt=false;

    //if a xhr havn't request or have been responded, ignore it; xhr will not response twice in 1 second
	if(m_XhrReqTime==0 || m_bXhrReqResponsed==true)
    {
        brlt=false;
    }
    else
    {
        struct timeb curtimeb;
        struct tm curtm;
        char szcurtime[32]={0};
        
        ftime(&curtimeb);
        memcpy((char*)&curtm,(char*)localtime(&(curtimeb.time)),sizeof(curtm));
        sprintf_s(szcurtime,sizeof(szcurtime)-1,"%02d:%02d:%02d.%03d",curtm.tm_hour,curtm.tm_min,curtm.tm_sec,curtimeb.millitm);

	    sprintf_s(prltbody,	rltbodysize-1, 
            "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"
		    "<root>\r\n"
		    "	<module>xhr</module>\r\n"
		    "	<datatype>xhr</datatype>\r\n"
		    "	<refreshtime>%s</refreshtime>\r\n"
		    "</root>\r\n",
            szcurtime);
        rltbodylen=strlen(prltbody);

	    int RltMode=2;
	    m_bXhrReqResponsed=true;

        sprintf_s(prlthead,rltheadsize-1,
            "%s 200 OK\r\n"
	        "Date: %s\r\n"
	        "Server: HttpSpace/0.2\r\n"
	        "Connection: %s\r\n"
	        "Content-type: %s\r\n"
	        "Content-Length: %d\r\n"
	        "Cache-Control: no-cache\r\n"
	        "Pragma: no-cache\r\n"
	        "\r\n",
            m_XhrReqHead.HttpVersion.c_str(),
            CTools_Ansi::Get_strTime_from_utc3(curtime).c_str(),
            m_XhrReqHead.Connection.c_str(),
            RltMode==2?"text/xml":"text/plain",
            rltbodylen);
        rltheadlen=strlen(prlthead);

        brlt=true;
    }

    if(!brlt) rltheadlen=rltbodylen=0;

    return brlt;
}


/*  http请求包：
    "POST /index.jsp HTTP/1.1\r\n"//注意\r\n为回车换行
    "Accept-Language: zh-cn\r\n"
    "Connection: Keep-Alive\r\n"
    "Host: 192.168.0.106\r\n"
    "Content-Length: 37\r\n"
    "\r\n"
    "userName=new_andy&password=new_andy\r\n"
    "\r\n");
*/
//Get http request package from tcp stream
//return 0: successful; -1: uncompleted; -2:error package
int CHttpSvrObj::GetRequestPkg(const char *pSour,int SourLen,Stru_HttpRequestPkg& RequestPkg,int& RltStartOff,int& RltLen)
{
	int HeadStart,HeadEnd,EntityStart,EntityEnd;
	int	i,Off,LineStart,LineEnd,LineLen;
	int WordCount,iStart1,iStart2,iStart3,iStart4,iEnd1,iEnd2,iEnd3,iEnd4;

    RltStartOff=0;
    RltLen=0;

    RequestPkg.clear();

	//parse head of http
	HeadStart=HeadEnd=EntityStart=EntityEnd=-1;
	Off=0;
	while(Off<SourLen)
	{
		CTools_Ansi::GetCurField(pSour,Off,SourLen,LineStart,LineEnd,' ');
		CTools_Ansi::GetCurField(pSour,LineStart,SourLen,LineStart,LineEnd,0x0d);
		if(LineStart>=SourLen||LineEnd>=SourLen)
		{
			//head of http should end with 0x0d, otherwise means uncompleted
			return -1;
		}

		//Parse Method, Request_URL, Request_Param, HttpVersion
		LineLen=LineEnd-LineStart;
		if(	HeadStart==-1 && IsBeginingOfHttp(pSour+LineStart,LineLen))
		{
			WordCount=CTools_Ansi::GetFourField(pSour,LineStart,LineEnd,iStart1,iEnd1,iStart2,iEnd2,iStart3,iEnd3,iStart4,iEnd4,' ');

			//get Method
            RequestPkg.Header.Method=string(pSour+iStart1,iEnd1-iStart1);

			if(WordCount>=2)
			{
				//get Request_URI, should be start with '?'
				for(i=iStart2;i<iEnd2&&pSour[i]!='?';i++);
				if(i==iStart2)
					return -2;
				RequestPkg.Header.Request_URL=string(pSour+iStart2,i-iStart2);

				//get Request_Param, like to xhr parameter
				if(i<iEnd2)
				{
					if(pSour[i]=='?') i++;

					RequestPkg.Header.Request_Param=string(pSour+i,iEnd2-i);

					//get bXhr, XhrModule, XhrCmd, XhrSeq, SessionID
					if(iEnd2-i>=(int)strlen(XHRID)&&memicmp(pSour+i,XHRID,strlen(XHRID))==0)
					{
                        RequestPkg.Header.XhrParam=string(pSour+i,iEnd2-i);
						RequestPkg.Header.bXhr=true;
                        RequestPkg.Header.XhrModule=CTools_Ansi::GetSubStr(RequestPkg.Header.XhrParam,string(XHRMODULE),string("&"));
                        RequestPkg.Header.XhrCmd=CTools_Ansi::GetSubStr(RequestPkg.Header.XhrParam,string(XHRCMD),string("&"));
                        RequestPkg.Header.XhrSeq=atoi(CTools_Ansi::GetSubStr(RequestPkg.Header.XhrParam,string(XHRSEQ),string("&")).c_str());
                        RequestPkg.Header.XhrSessionID=atoi(CTools_Ansi::GetSubStr(RequestPkg.Header.XhrParam,string(XHRSESSIONID),string("&")).c_str());
					}
				}
			}

			if(WordCount>=3)
				RequestPkg.Header.HttpVersion=string(pSour+iStart3,iEnd3-iStart3);

			HeadStart=LineStart;
		}
		else if(HeadStart!=-1)
		{
            char buf[1024];

			CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"Accept:",RequestPkg.Header.Accept);
			CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"Accept-Language:",RequestPkg.Header.Accept_Language);
			CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"Connection:",RequestPkg.Header.Connection);
			if(CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"Content-Length:",buf,sizeof(buf)))
                RequestPkg.Header.Content_Length=atoi(buf);
			CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"If-Modified-Since:",RequestPkg.Header.If_Modified_Since);
			CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"Referer:",RequestPkg.Header.Referer);
			CTools_Ansi::GetContentFollowPrefix(pSour,LineStart,LineEnd,"User-Agent:",RequestPkg.Header.User_Agent);
		}


		Off=LineEnd+1;
		if(pSour[Off]==0x0a) Off++;
		if(HeadStart!=-1&&pSour[Off]==0x0d)
		{
			//now the head have completed, following is entity
			Off++;
			if(pSour[Off]==0x0a) Off++;

			HeadEnd=Off;
			EntityStart=Off;
            if(RequestPkg.Header.Content_Length==0) EntityEnd=Off;
			else
			{
                if(SourLen-EntityStart>=RequestPkg.Header.Content_Length)
				{
                    EntityEnd=EntityStart+RequestPkg.Header.Content_Length;
				}
				else
				{
					return -1;				//uncompleted
				}
			}
			break;
		}
	}

	if(EntityEnd==-1||EntityStart==-1||HeadEnd==-1||HeadStart==-1) return -2;
	if(EntityEnd-EntityStart>=RequestPkg.ContentSize)
	{
		return -2;							//too long, error package
	}

	RequestPkg.setContent(pSour+EntityStart,EntityEnd-EntityStart);

	RltStartOff=HeadStart;
	RltLen=EntityEnd-HeadStart;

	return 0;
}
//tell you whether a PkgStream is a valid package, if successful, *pUsedLengthByPkg return length used by current package
bool CHttpSvrObj::IsValidPkg(const void* pData,int DataLen,int* pLengthUsedByPkg)
{
	if(DataLen<7) return false;
	if(!IsBeginingOfHttp((const char*)pData,DataLen)) return false;

	Stru_HttpRequestPkg RequestPkg;
	int RltStartOff,RltLen;
	int irlt=GetRequestPkg((const char*)pData,DataLen,RequestPkg,RltStartOff,RltLen);
	if(irlt==0&&RltStartOff==0)
	{
		if(pLengthUsedByPkg!=NULL) *pLengthUsedByPkg=RltLen;
		return true;
	}
	else return false;
}
//tell you whether a PkgStream is an invalid package. Typically, you can skip a character if the stream was start with an invalid package head
bool CHttpSvrObj::IsInvalidPkgHead(const void* pData,int DataLen)
{
	if(DataLen<7) return false;
	if(!IsBeginingOfHttp((const char*)pData,DataLen)) return true;

	Stru_HttpRequestPkg RequestPkg;
	int RltStartOff,RltLen;
	int irlt=GetRequestPkg((const char*)pData,DataLen,RequestPkg,RltStartOff,RltLen);
	if(irlt==-2||RltStartOff!=0) return true;
	else return false;
}




//Find the beginning of http package. It must be start with "GET " / "HEAD " / "POST " / "PUT " / "DELETE " / "LINK "
//return offset of begining(greater or equal than zero) if successful, -1 if failure
int CHttpSvrObj::FindBeginningOfHttpPkg(const char* pSour,int SourLen)
{
	int Off;

	for(Off=0;Off<SourLen;Off++)
	{
		if((SourLen-Off>=4&&memicmp(pSour+Off,"GET ",4)==0)||
				(SourLen-Off>=5&&memicmp(pSour+Off,"HEAD ",5)==0)||
				(SourLen-Off>=5&&memicmp(pSour+Off,"POST ",5)==0)||
				(SourLen-Off>=4&&memicmp(pSour+Off,"PUT ",4)==0)||
				(SourLen-Off>=7&&memicmp(pSour+Off,"DELETE ",7)==0)||
				(SourLen-Off>=5&&memicmp(pSour+Off,"LINK ",5)==0))
			return Off;
	}
	return -1;
}
bool CHttpSvrObj::IsBeginingOfHttp(const char* pData,int DataLen)
{
	if(!pData) return false;
	if((DataLen>=4&&memicmp(pData,"GET ",4)==0)||
	   (DataLen>=5&&memicmp(pData,"HEAD ",5)==0)||
	   (DataLen>=5&&memicmp(pData,"POST ",5)==0)||
	   (DataLen>=4&&memicmp(pData,"PUT ",4)==0)||
	   (DataLen>=7&&memicmp(pData,"DELETE ",7)==0)||
	   (DataLen>=5&&memicmp(pData,"LINK ",5)==0))
	   return true;
	else return false;
}

//convert filename to string
char* CHttpSvrObj::ConvertFileTypeToStr(const char *FileName,char *pRltStr)
{
	int				FileNameLen;
	pRltStr[0]=0;
	FileNameLen=strlen(FileName);
	if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".doc",4)==0) strcpy(pRltStr,"application/msword");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".rtf",4)==0) strcpy(pRltStr,"application/rtf");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".zip",4)==0) strcpy(pRltStr,"application/zip");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".gif",4)==0) strcpy(pRltStr,"image/gif");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".jpg",4)==0) strcpy(pRltStr,"image/jpg");
	else if(FileNameLen>=5&&memicmp(FileName+FileNameLen-5,".jpeg",5)==0) strcpy(pRltStr,"image/jpeg");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".bmp",4)==0) strcpy(pRltStr,"image/x-xbitmap");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".ini",4)==0) strcpy(pRltStr,"text/plain");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".txt",4)==0) strcpy(pRltStr,"text/plain");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".htm",4)==0) strcpy(pRltStr,"text/html");
	else if(FileNameLen>=5&&memicmp(FileName+FileNameLen-5,".html",5)==0) strcpy(pRltStr,"text/html");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".avi",4)==0) strcpy(pRltStr,"video/avi");
	else if(FileNameLen>=4&&memicmp(FileName+FileNameLen-4,".wav",4)==0) strcpy(pRltStr,"audio/wav");
	return pRltStr;
}
//convert string to time value formated in tm, as "Tue, 14 Sep 1999"
void CHttpSvrObj::ConvertStrToDate(const char *pDateStr,struct tm *pltime)
{
	int				iStart1,iEnd1,iStart2,iEnd2,iStart3,iEnd3,iStart4,iEnd4;

	memset((char*)pltime,0,sizeof(struct tm));
	CTools_Ansi::GetFourField(pDateStr,0,strlen(pDateStr),iStart1,iEnd1,iStart2,iEnd2,iStart3,iEnd3,iStart4,iEnd4,',',' ','-');
	if(iEnd1-iStart1>=3)
	{
		if(memicmp(pDateStr+iStart1,"Sun",3)==0) pltime->tm_wday=0;
		else if(memicmp(pDateStr+iStart1,"Mon",3)==0) pltime->tm_wday=1;
		else if(memicmp(pDateStr+iStart1,"Tue",3)==0) pltime->tm_wday=2;
		else if(memicmp(pDateStr+iStart1,"Wed",3)==0) pltime->tm_wday=3;
		else if(memicmp(pDateStr+iStart1,"Thu",3)==0) pltime->tm_wday=4;
		else if(memicmp(pDateStr+iStart1,"Fri",3)==0) pltime->tm_wday=5;
		else if(memicmp(pDateStr+iStart1,"Sat",3)==0) pltime->tm_wday=6;
	}
	if(iEnd2>iStart2) pltime->tm_mday=atoi(pDateStr+iStart2);
	if(iEnd3-iStart3>=3)
	{
		if(memicmp(pDateStr+iStart3,"Jan",3)==0) pltime->tm_mon=0;
		else if(memicmp(pDateStr+iStart3,"Feb",3)==0) pltime->tm_mon=1;
		else if(memicmp(pDateStr+iStart3,"Mar",3)==0) pltime->tm_mon=2;
		else if(memicmp(pDateStr+iStart3,"Apr",3)==0) pltime->tm_mon=3;
		else if(memicmp(pDateStr+iStart3,"May",3)==0) pltime->tm_mon=4;
		else if(memicmp(pDateStr+iStart3,"Jun",3)==0) pltime->tm_mon=5;
		else if(memicmp(pDateStr+iStart3,"Jul",3)==0) pltime->tm_mon=6;
		else if(memicmp(pDateStr+iStart3,"Aug",3)==0) pltime->tm_mon=7;
		else if(memicmp(pDateStr+iStart3,"Seq",3)==0) pltime->tm_mon=8;
		else if(memicmp(pDateStr+iStart3,"Oct",3)==0) pltime->tm_mon=9;
		else if(memicmp(pDateStr+iStart3,"Nov",3)==0) pltime->tm_mon=10;
		else if(memicmp(pDateStr+iStart3,"Dec",3)==0) pltime->tm_mon=11;
	}
	if(iEnd4-iStart4==2)
	{
		if(pDateStr[iStart4]<'7') pltime->tm_year=atoi(pDateStr+iStart4)+100;	//17-Apr-06
		else pltime->tm_year=atoi(pDateStr+iStart4);							//17-Apr-96
	}
	else if(iEnd4-iStart4==1) pltime->tm_year=atoi(pDateStr+iStart4)+100;		//17-Apr-6
	else if(iEnd4-iStart4==4) pltime->tm_year=atoi(pDateStr+iStart4)-1900;		//17-Apr-2006

	CTools_Ansi::GetFourField(pDateStr,iEnd4,strlen(pDateStr),iStart1,iEnd1,iStart2,iEnd2,iStart3,iEnd3,iStart4,iEnd4,':',0,0,0);
	if(iEnd1>iStart1) pltime->tm_hour=atoi(pDateStr+iStart1);
	if(iEnd2>iStart2) pltime->tm_min=atoi(pDateStr+iStart2);
	if(iEnd3>iStart3) pltime->tm_sec=atoi(pDateStr+iStart3);
}

void CHttpSvrObj::RecvData(const char* pData,int DataLen,char* pRlt,int RltSize,int& RltLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr)
{
    CeasymutexGuard guard(m_mutex);

    RltLen=0;
    if(!pData||!m_pRcvData||m_RcvDataLen+DataLen>RcvDataSize) return;
    memcpy(m_pRcvData+m_RcvDataLen,pData,DataLen);
    m_RcvDataLen+=DataLen;

    int off=0;
    while(off<m_RcvDataLen)
    {
        Stru_HttpRequestPkg ReqPkg;
        int pkgoff,pkglen;
        int GetPkgRlt=GetRequestPkg(m_pRcvData+off,m_RcvDataLen-off,ReqPkg,pkgoff,pkglen);
        if(GetPkgRlt==-2) { off++;  continue;}
        if(GetPkgRlt==-1) break;

        ReqPkg.Header.SvrAddr=SvrAddr;
        ReqPkg.Header.RemoteAddr=RemoteAddr;
        ReqPkg.Header.RecvTime=time(NULL);

        if(!ReqPkg.Header.bXhr)
        {
            bool brlt=ProcessReqPkg_Html(ReqPkg,m_pRspHttpHead,RspHttpHeadSize,m_RspHttpHeadLen,m_pRspHttpBody,RspHttpBodySize,m_RspHttpBodyLen);
            if(brlt&&m_RspHttpHeadLen+m_RspHttpBodyLen>0)
            {
                if(m_RspHttpHeadLen+m_RspHttpBodyLen<=RltSize)
                {
                    if(m_RspHttpHeadLen>0) memcpy(pRlt,m_pRspHttpHead,m_RspHttpHeadLen);
                    if(m_RspHttpBodyLen>0) memcpy(pRlt+m_RspHttpHeadLen,m_pRspHttpBody,m_RspHttpBodyLen);
                    RltLen=m_RspHttpHeadLen+m_RspHttpBodyLen;
                    brlt=true;
                }
                m_RspHttpHeadLen=0;
                m_RspHttpBodyLen=0;
            }
        }
        else if(!ReqPkg.Header.XhrModule.empty())
        {
            ProcessReqPkg_XhrArrival(ReqPkg.Header);
        }

        off=pkgoff+pkglen;
    }
	if(off>0)
    {
		memmove(m_pRcvData,m_pRcvData+off,m_RcvDataLen-off);
	    m_RcvDataLen -= off;
    }
}

void CHttpSvrObj::ProcessOntime(char* pRlt,int RltSize,int& RltLen)
{
    CeasymutexGuard guard(m_mutex);

	//static int cnt=0;
	//int cnt2=cnt;
	//cnt++;
	//CFileOpr::getObj().writelocallog("ontime","%s     : Start CHttpSvrObj::ProcessOntime cnt2=%d Tick=%d",CTools_Ansi::Get_strTime_from_utc2(time(NULL)).c_str(),cnt2,GetTickCount());

    RltLen=0;
    bool brlt=ProcessReqPkg_XhrOntime(m_pRspHttpHead,RspHttpHeadSize,m_RspHttpHeadLen,m_pRspHttpBody,RspHttpBodySize,m_RspHttpBodyLen);
    if(brlt&&m_RspHttpHeadLen+m_RspHttpBodyLen>0)
    {
        if(m_RspHttpHeadLen+m_RspHttpBodyLen<=RltSize)
        {
            if(m_RspHttpHeadLen>0) memcpy(pRlt,m_pRspHttpHead,m_RspHttpHeadLen);
            if(m_RspHttpBodyLen>0) memcpy(pRlt+m_RspHttpHeadLen,m_pRspHttpBody,m_RspHttpBodyLen);
            RltLen=m_RspHttpHeadLen+m_RspHttpBodyLen;
            brlt=true;
        }
        m_RspHttpHeadLen=0;
        m_RspHttpBodyLen=0;
    }
	//CFileOpr::getObj().writelocallog("ontime","%s     : End   CHttpSvrObj::ProcessOntime cnt2=%d Tick=%d",CTools_Ansi::Get_strTime_from_utc2(time(NULL)).c_str(),cnt2,GetTickCount());
}

//网页端的js一般用encodeURI()对提交内容如用户名等进行UTF-8编码，这里需要解码并转换为GBK处理。参考baidu：ajax中文编码问题
string CHttpSvrObj::DecodeURI2GBK(const string& strURI)
{
	unsigned char* ptmp=(unsigned char*)malloc(strURI.length()+1);

	int cnt=0;
	for(int i=0;i<(int)strURI.length();)
	{
		unsigned char ch=strURI[i];
		if(ch!='%') 
		{
			ptmp[cnt++]=ch;
			i++;
		}
		else
		{
			ptmp[cnt++]=CTools_Ansi::GetByteFromHexString(strURI.substr(i+1,2));
			i+=3;
		}
	}
	ptmp[cnt]=0;
	return CTools_Win32::UTF8ToGBK(string((char*)ptmp));
}


//****************************************************************
//  Realization of CHttpSvr
//****************************************************************
CHttpSvr* CHttpSvr::m_pObj=NULL;

CHttpSvr::CHttpSvr()
{
    m_pTcpSvr=NULL;
    m_pObj=this;
    m_RspHttpBuf=(char*)malloc(RspHttpBufSize);
    
    m_bWriteHtmlLog=CMyXML("httpsvr.xml").GetNodeContentbool2(false,"root","httpsvr","writelog");
}
CHttpSvr::~CHttpSvr()
{
    Stop();
    m_pObj=NULL;
    if(m_RspHttpBuf)
    {
        free(m_RspHttpBuf);
        m_RspHttpBuf=NULL;
    }
}

void CHttpSvr::Start(const char *pWebPath,const char *pDefWeb,Stru_IPAddress BindAddr,int iMaxListen)
{
    CeasymutexGuard guard(m_mutex);

    if(pWebPath) m_strWebPath=string(pWebPath);
    if(pDefWeb) m_strDefWeb=string(pDefWeb);
    if(BindAddr.IP!=0&&BindAddr.Port!=0)
        m_pTcpSvr=new CTcpServerWorker(BindAddr,iMaxListen,RcvdataCB,AcceptCB,DisconnCB,OntimeCB);
}

void CHttpSvr::Stop()
{
    CeasymutexGuard guard(m_mutex);

    if(m_pTcpSvr) 
    {
        delete m_pTcpSvr;
        m_pTcpSvr=NULL;
    }

    for(map<int,CHttpSvrObj*>::iterator it=m_HttpSvrObjs.begin();it!=m_HttpSvrObjs.end();it++)
    {
        if(it->second!=NULL)
        {
            delete it->second;
            it->second=NULL;
        }
    }
    m_HttpSvrObjs.clear();
}

//Create a httpsvrobj. this routine can be overload in business layer to new a subclass of CHttpSvrObj
CHttpSvrObj* CHttpSvr::CreateHttpSvrObj(void)
{
    return new CHttpSvrObj(m_strWebPath.c_str(),m_strDefWeb.c_str());
}

//TcpSvr callback routine
void CHttpSvr::AcceptCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    if(!m_pObj) return;
    m_pObj->m_mutex.lock();

    map<int,CHttpSvrObj*>::iterator it=m_pObj->m_HttpSvrObjs.find((int)hSocket);
    if(it!=m_pObj->m_HttpSvrObjs.end()) 
    {
        if(it->second!=NULL)
        {
            delete it->second;
            it->second=NULL;
        }
        m_pObj->m_HttpSvrObjs.erase(it);
    }
    m_pObj->m_HttpSvrObjs[(int)hSocket]=m_pObj->CreateHttpSvrObj();

    m_pObj->m_mutex.unlock();
}
void CHttpSvr::DisconnCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    if(!m_pObj) return;
    m_pObj->m_mutex.lock();

    map<int,CHttpSvrObj*>::iterator it=m_pObj->m_HttpSvrObjs.find((int)hSocket);
    if(it!=m_pObj->m_HttpSvrObjs.end()) 
    {
        if(it->second!=NULL)
        {
            delete it->second;
            it->second=NULL;
        }
        m_pObj->m_HttpSvrObjs.erase(it);
    }

    m_pObj->m_mutex.unlock();
}
void CHttpSvr::RcvdataCB(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    if(!m_pObj) return;
    m_pObj->m_mutex.lock();

    string log(pData,DataLen);
    if(m_pObj->m_bWriteHtmlLog)
        CFileOpr::getObj().writelocallog("html","\r\n**********recv**********, datalen=%d hSocket=%d/%s\r\n%s",DataLen,(int)hSocket,RemoteAddr.tostring().c_str(),log.c_str());

    map<int,CHttpSvrObj*>::iterator it=m_pObj->m_HttpSvrObjs.find((int)hSocket);
    if(it!=m_pObj->m_HttpSvrObjs.end()&&m_pObj->m_RspHttpBuf)
    {
        int RspHttpBufLen=0;
        it->second->RecvData(pData,DataLen,m_pObj->m_RspHttpBuf,RspHttpBufSize,RspHttpBufLen,SvrAddr,RemoteAddr);
        if(RspHttpBufLen>0)
        {
            int len=CSocketBase::SendDataTcp(hSocket,m_pObj->m_RspHttpBuf,RspHttpBufLen);

            if(m_pObj->m_bWriteHtmlLog)
            {
                string log(m_pObj->m_RspHttpBuf,RspHttpBufLen);
                CFileOpr::getObj().writelocallog("html","\r\n----------send----------, sentlen=%d/%d\r\n%s",len,RspHttpBufLen,log.c_str());
            }
        }
    }

    m_pObj->m_mutex.unlock();
}
void CHttpSvr::OntimeCB(void)
{
    if(!m_pObj) return;
    m_pObj->m_mutex.lock();

	//static int cnt=0;
	//int cnt2=cnt;
	//cnt++;
	//CFileOpr::getObj().writelocallog("ontime","%s     : Start CHttpSvr::OntimeCB m_HttpSvrObjs.size()=%d cnt2=%d Tick=%d",CTools_Ansi::Get_strTime_from_utc2(time(NULL)).c_str(),m_pObj->m_HttpSvrObjs.size(),cnt2,GetTickCount());

    if(m_pObj->m_RspHttpBuf)
    {
        for(map<int,CHttpSvrObj*>::iterator it=m_pObj->m_HttpSvrObjs.begin();it!=m_pObj->m_HttpSvrObjs.end();it++)
        {
            int RspHttpBufLen=0;
            it->second->ProcessOntime(m_pObj->m_RspHttpBuf,RspHttpBufSize,RspHttpBufLen);
            if(RspHttpBufLen>0)
            {
                int len=CSocketBase::SendDataTcp((SOCKET)(it->first),m_pObj->m_RspHttpBuf,RspHttpBufLen);

                if(m_pObj->m_bWriteHtmlLog)
                {
                    string log(m_pObj->m_RspHttpBuf,RspHttpBufLen);
                    CFileOpr::getObj().writelocallog("html","\r\n==========sendontime==========, sentlen=%d/%d\r\n%s",len,RspHttpBufLen,log.c_str());
                }
            }
        }
    }
	//CFileOpr::getObj().writelocallog("ontime","%s     : End   CHttpSvr::OntimeCB cnt2=%d Tick=%d",CTools_Ansi::Get_strTime_from_utc2(time(NULL)).c_str(),cnt2,GetTickCount());

	m_pObj->m_mutex.unlock();
}

