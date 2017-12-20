#include "StdAfx.h"
#include "stdio.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#include <limits>
#include "GLobalFunc.h"
#ifdef _USE_TRACE_THREAD
void RELEASE_TRACE(const char* format,...)
{
	static const int BufferLen = 1024;        //注意此处大小根据需要修改
	va_list pNextArg;
	char szMessageBuffer[BufferLen];
	memset(szMessageBuffer,0,BufferLen);
	va_start(pNextArg,format);
	//_stprintf_s(szMessageBuffer,BufferLen-1,format,pNextArg);
	wvsprintf(szMessageBuffer,format,pNextArg);
	va_end(pNextArg);
#ifndef _DEBUG
	OutputDebugString(szMessageBuffer);
#endif
}

CReleaseTrace::CReleaseTrace(DWORD id,const char* funcname,int line)
:m_id(id),
m_funcname(funcname),
m_line(line)
{
    RELEASE_TRACE("BEGIN-TID(%u)-%s(%d)\n",m_id,m_funcname.c_str(),m_line);
}
CReleaseTrace::~CReleaseTrace()
{
    RELEASE_TRACE("END---TID(%u)-%s(%d)\n",m_id,m_funcname.c_str(),m_line);
}
#endif


zqWriteLog* GlobalFunc::sm_pUserOperWriteLog = NULL;
CUserOperLog* GlobalFunc::sm_pUserOperLog = NULL;


std::string GlobalFunc::GetSysPath()
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	strSystemPath = strSystemPath.substr( 0, nPos );
	return strSystemPath;
}

void GlobalFunc::GetPath(std::string& strFilePath)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr( 0, nPos + 1 );
		strFilePath = strSystemPath + strFilePath;
	}	
}
void GlobalFunc::GetFileName(std::string& strFileName)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr(nPos + 1 );
		nPos = strSystemPath.rfind( '.' );
		if(-1 != nPos)
		{
			strFileName = strSystemPath.substr(0,nPos);
		}
	}
}
bool GlobalFunc::IsFileExists( const std::string &strFilePath )
{
	return ( _access( strFilePath.c_str(), 0 ) == 0 );
}
std::string GlobalFunc::GetValidName(const std::string strInName)
{
    static const std::string IllegalChar="\\/*:?\"<>|";
    static size_t illcount=IllegalChar.length();
    std::string strName=strInName;
    size_t strcount=strName.length();
    for(size_t i=0;i<strcount;++i)
    {
        for(size_t j=0;j<illcount;++j)
        {
            if(strName[i]==IllegalChar[j])
            {
                strName[i]=' ';
                break;
            }
        }
    }
    return strName;
}
void* GlobalFunc::GetXmlFileResource(const char*  lpName,const char*  lpType,unsigned long* lpSize)
{
	HRSRC hRsrc = FindResource(NULL,lpName, lpType);
	if (NULL == hRsrc)
		return NULL;
	DWORD dwSize = SizeofResource(NULL, hRsrc); 
	if (0 == dwSize)
		return NULL;
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc); 
	if (NULL == hGlobal)
		return NULL;
	void* pBuffer = LockResource(hGlobal); 
	if (NULL == pBuffer)
		return NULL;
	if(lpSize)
		*lpSize=dwSize;
	return pBuffer;
}
class thousands_sep_facet:public std::numpunct<char> {
public:
	explicit  thousands_sep_facet(size_t r = 0):std::numpunct<char>(r) {}
protected:
	string do_grouping() const { return "\003";}

}; 
//获取货币的分段显示格式
//这个实现太慢了，改为GetAccountFormatString2，弃用
string GlobalFunc::GetAccountFormatString2(double dblAmount,int precision)
{
	string str;
	stringstream ss;
	locale  loc(locale(), new thousands_sep_facet); //这一句尤其慢
	ss.imbue(loc);
	ss.setf(ios::fixed);
	if(precision>=0)
	{
		ss<<setprecision(precision)<<dblAmount;
		str=ss.str();
	}
	else
	{
		ss<<dblAmount;
		str=ss.str();
		for(int i=str.length()-1; i>=0; i--) 
		{
			if(str[i]=='0')
				str[i]=0;
			else
			{
				if(str[i]=='.')
					str[i]=0;
				break;
			}
		}
	}
	ss.unsetf(ios::fixed);
	return str;
}

//获取货币的分段显示格式
string GlobalFunc::GetAccountFormatString(double dblAmount,int precision)
{
	char szBuf[256]={0};
	char szFormat[16]={0};
    int i,Len,ipt;

    if(precision<0&&precision!=-1) precision=2;

    if(precision==-1)
    {
        _snprintf(szBuf,255,"%f",dblAmount);
        szBuf[sizeof(szBuf)-1]=0;
        Len=strlen(szBuf);
        for(i=0;i<Len&&szBuf[i]!='.';i++);
        if(i<Len)
        {
            //过滤分数部分无效的零
            while(Len>0&&szBuf[Len-1]=='0') 
            {
                szBuf[Len-1]=0;
                Len--;
            }
		}
		Len=strlen(szBuf);
		if(Len>0&&szBuf[Len-1]=='.')
			szBuf[Len-1]=0;
    }
    else
    {
        _snprintf(szFormat,15,"%%.%df",precision);
        _snprintf(szBuf,255,szFormat,dblAmount);
        szBuf[sizeof(szBuf)-1]=0;
    }

    Len=strlen(szBuf);
    for(ipt=0;ipt<Len&&szBuf[ipt]!='.';ipt++);
    while(ipt>3&&
            szBuf[ipt-3-1]>='0'&&szBuf[ipt-3-1]<='9'&&
            Len<sizeof(szBuf)-10)
    {
        i=ipt-3;
        memmove(szBuf+i+1,szBuf+i,Len-i+1);
        szBuf[i]=',';
        Len++;
        ipt-=3;
    }
    return string(szBuf);
}

zqWriteLog& GlobalFunc::GetUserOperWriteLog() 
{
	if(sm_pUserOperWriteLog==NULL) {
		string strFileName = GetUserOperFileName();
		sm_pUserOperWriteLog = new zqWriteLog(LOGMODE_LOCALFILE, (char*)(strFileName.c_str()));
	}
	return *sm_pUserOperWriteLog;
}

void GlobalFunc::ReleaseUserOperWriteLog()
{
	if(sm_pUserOperWriteLog!=NULL) {
		delete sm_pUserOperWriteLog;
		sm_pUserOperWriteLog = NULL;
	}
}

CUserOperLog& GlobalFunc::GetUserOperLog() 
{
	if(sm_pUserOperLog==NULL) {
		string strFileName = GetUserOperFileName();
		sm_pUserOperLog = new CUserOperLog(strFileName);
	}
	return *sm_pUserOperLog;
}

void GlobalFunc::ReleaseUserOperLog()
{
	if(sm_pUserOperLog!=NULL) {
		delete sm_pUserOperLog;
		sm_pUserOperLog = NULL;
	}
}

BOOL GlobalFunc::MakeUserOperDirection()
{
    // 创建用户操作日志目录
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+=USEROPERLOGPATH;
    if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);
	return TRUE;
}

string GlobalFunc::GetUserOperFileName()
{
	SYSTEMTIME sysTime;
	char strFileName[1024];

	ZeroMemory(strFileName, 1024);
	::GetLocalTime(&sysTime);
	sprintf_s(strFileName, sizeof(strFileName)-1, 
			"%sUserOperate%04d%02d%02d.log", USEROPERLOGPATH, 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	return string(strFileName);
}
