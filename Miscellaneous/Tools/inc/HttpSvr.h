/*
 * HttpSvr.h
 *
 * Used for process Http requestion.
 * 1. it can deal with http request and xhr request
 *
 *  Created on: May 29, 2014
 *      Author: root
 */

#ifndef HTTPSVR_H_
#define HTTPSVR_H_

#ifndef WIN32
#include <pthread.h>
#endif

#include "TcpServerWorker.h"
#include "easymutex.h"
#include <string>
#include <map>
using std::string;
using std::map;


#ifdef WIN32
#pragma managed(push,off)
#endif 


#define XHRID			"XhrID=1"
#define XHRMODULE 		"XhrModule="
#define XHRCMD			"XhrCmd="
#define XHRSEQ			"XhrSeq="
#define XHRSESSIONID    "SessionID="

//****************************************************************
//  Next define CHttpSvrObj, who is used as single object in CHttpSvr
//  you can overload ProcessReqPkg_XhrOntime() to process xhr requestion
//****************************************************************
class CHttpSvrObj
{
public:


    //This structure define head of http request
    struct Stru_HttpRequestPkgHeader
    {
	    Stru_HttpRequestPkgHeader()
	    {
		    clear();
	    };
       	Stru_HttpRequestPkgHeader(const Stru_HttpRequestPkgHeader& r)
        {
            *this=r;
        };
	    Stru_HttpRequestPkgHeader& operator= (const Stru_HttpRequestPkgHeader& r)
	    {
	        Method=r.Method;
	        Request_URL=r.Request_URL;
	        Request_Param=r.Request_Param;
	        HttpVersion=r.HttpVersion;

	        bXhr=r.bXhr;
	        XhrModule=r.XhrModule;
	        XhrCmd=r.XhrCmd;
	        XhrSeq=r.XhrSeq;
            XhrSessionID=r.XhrSessionID;
	        XhrParam=r.XhrParam;

	        Accept=r.Accept;
	        Accept_Language=r.Accept_Language;
	        Connection=r.Connection;
	        Content_Length=r.Content_Length;
	        If_Modified_Since=r.If_Modified_Since;
	        Referer=r.Referer;
	        User_Agent=r.User_Agent;

            SvrAddr=r.SvrAddr;
            RemoteAddr=r.RemoteAddr;
            RecvTime=r.RecvTime;

		    return *this;
	    };

	    void clear(void)
	    {
            Method.clear();
	        Request_URL.clear();
	        Request_Param.clear();
	        HttpVersion.clear();

	        bXhr=false;
	        XhrModule.clear();
	        XhrCmd.clear();
	        XhrSeq=0;
            XhrSessionID=0;
	        XhrParam.clear();

	        Accept.clear();
	        Accept_Language.clear();
	        Connection.clear();
	        Content_Length=0;
	        If_Modified_Since.clear();
	        Referer.clear();
	        User_Agent.clear();

            SvrAddr.clear();
            RemoteAddr.clear();
            RecvTime=0;
	    };


        string			Method;
	    string			Request_URL;
	    string			Request_Param;
	    string			HttpVersion;

	    bool			bXhr;
	    string			XhrModule;
	    string			XhrCmd;
	    unsigned int	XhrSeq;
        int             XhrSessionID;
	    string			XhrParam;

	    string			Accept;
	    string			Accept_Language;
	    string			Connection;
	    int			    Content_Length;
	    string			If_Modified_Since;
	    string			Referer;
	    string			User_Agent;

        Stru_IPAddress  SvrAddr;
        Stru_IPAddress  RemoteAddr;
        time_t          RecvTime;
    };

    struct Stru_HttpRequestPkg
    {
        static const int ContentSize=1*1024*1024;

	    Stru_HttpRequestPkg()
	    {
            Content=(char*)malloc(ContentSize);
		    clear();
	    };
       	Stru_HttpRequestPkg(const Stru_HttpRequestPkg& r)
        {
            *this=r;
        };
        ~Stru_HttpRequestPkg()
        {
            if(Content) { free(Content);  Content=NULL; }
        };
	    Stru_HttpRequestPkg& operator= (const Stru_HttpRequestPkg& r)
        {
            Header=r.Header;
            if(Content&&r.Content&&Header.Content_Length>0&&Header.Content_Length<ContentSize) 
                memcpy(Content,r.Content,Header.Content_Length);
        };
	    void clear()
	    {
		    Header.clear();
		    memset(Content,0,sizeof(Content));
	    }
	    bool setContent(const char* pData,int DataLen) { if(DataLen>=ContentSize) return false; memcpy(Content,pData,DataLen); Content[DataLen]=0; return true; }

	    Stru_HttpRequestPkgHeader   Header;
	    char			            *Content;
    };

public:
    CHttpSvrObj(const char *pWebPath,const char *pDefWeb);
    CHttpSvrObj(const CHttpSvrObj& r);
    virtual ~CHttpSvrObj();
    CHttpSvrObj& operator= (const CHttpSvrObj& r);

public:
    //process a request of http, refer to html. Don't response request that begin with CXhr_
    virtual bool ProcessReqPkg_Html(const Stru_HttpRequestPkg& ReqPkg,
        char* prlthead,int rltheadsize,int& rltheadlen,
        char* prltbody,int rltbodysize,int& rltbodylen);

    //this routine was called to notify a xhr reach
	virtual bool ProcessReqPkg_XhrArrival(const Stru_HttpRequestPkgHeader& XhrHead);

    ////process a request of Xhr
    virtual bool ProcessReqPkg_XhrOntime(char* prlthead,int rltheadsize,int& rltheadlen,
        char* prltbody,int rltbodysize,int& rltbodylen);

	//网页端的js一般用encodeURI()对提交内容如用户名等进行UTF-8编码，这里需要解码并转换为GBK处理。参考baidu：ajax中文编码问题
	static string DecodeURI2GBK(const string& strURI);

private:
	//Get http request package from tcp stream
	//return 0: successful; -1: uncompleted; -2:error pkg
	int	GetRequestPkg(const char *pSour,int SourLen,Stru_HttpRequestPkg& RequestPkg,int& RltStartOff,int& RltLen);
    //tell you whether a PkgStream is a valid package, if successful, *pUsedLengthByPkg return length used by current package
    bool IsValidPkg(const void* pData,int DataLen,int* pLengthUsedByPkg);
    //tell you whether a PkgStream is an invalid package. Typically, you can skip a character if the stream was start with an invalid package head
    bool IsInvalidPkgHead(const void* pData,int DataLen);

private:
	//Find the beginning of http package. It must be start with "GET "/"HEAD "/"POST "/"PUT "/"DELETE "/"LINK "
	//return greater or equal than zero if successsul, -1 if failure
	int FindBeginningOfHttpPkg(const char* pSour,int SourLen);
    bool IsBeginingOfHttp(const char* pData,int DataLen);

    //convert filename to string
    char* ConvertFileTypeToStr(const char *FileName,char *pRltStr);
    //convert string to time value formated in tm, as "Tue, 14 Sep 1999"
    void ConvertStrToDate(const char *pDateStr,struct tm *pltime);


private:
    Ceasymutex m_mutex;


    //http member variables
    char                        m_WebPath[512];
    char                        m_DefWeb[512];

protected:
    //xhr member variables
    Stru_HttpRequestPkgHeader   m_XhrReqHead;           //head of current xhr request
    time_t                      m_XhrReqTime;           //time of receiving current xhr request
    bool                        m_bXhrReqResponsed;     //whether current xhr request has been responsed

public:
    void RecvData(const char* pData,int DataLen,char* pRlt,int RltSize,int& RltLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr);
    void ProcessOntime(char* pRlt,int RltSize,int& RltLen);

private:
    static const int            RcvDataSize=2*1024*1024;
    int                         m_RcvDataLen;
    char*                       m_pRcvData;

    static const int            RspHttpHeadSize=10*1024;
    int                         m_RspHttpHeadLen;
    char*                       m_pRspHttpHead;

    static const int            RspHttpBodySize=10*1024*1024;
    int                         m_RspHttpBodyLen;
    char*                       m_pRspHttpBody;


};

//****************************************************************
//  Next define CHttpSvr, who is used as http server.
//  CHttpSvrObj pointer is used in this class to process single http connetion
//****************************************************************
class CHttpSvr
{
public:
    CHttpSvr();
    ~CHttpSvr();

    void Start(const char *pWebPath,const char *pDefWeb,Stru_IPAddress BindAddr,int iMaxListen=16);
    void Stop();

    //Create a httpsvrobj. this routine can be overload in business layer to new a subclass of CHttpSvrObj
    virtual CHttpSvrObj* CreateHttpSvrObj();

public:
    //TcpSvr回调函数
    static void	AcceptCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);      
    static void	DisconnCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);		
    static void	RcvdataCB(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
    static void OntimeCB(void);

protected:
    static CHttpSvr*            m_pObj;

    CTcpServerWorker*           m_pTcpSvr;
    Ceasymutex                  m_mutex;

    string                      m_strWebPath;
    string                      m_strDefWeb;

    map<int,CHttpSvrObj*>       m_HttpSvrObjs;

    static const int            RspHttpBufSize=10*1024*1024;
    char*                       m_RspHttpBuf;

    bool                        m_bWriteHtmlLog;

private:
    CHttpSvr(const CHttpSvr& r);
    CHttpSvr& operator= (const CHttpSvr& r);
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif /* HTTPXHR_H_ */
