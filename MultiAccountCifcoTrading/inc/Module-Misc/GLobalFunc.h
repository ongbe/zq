#ifndef __GLOBALFUNC_H__
#define __GLOBALFUNC_H__

#include <iostream>
#include <sstream>
#include <limits>
#include "GLobalDefines.h"

#include "../inc/gui/EventParam.h"
#include "SimpleWriteLog.h"
#include "UserOperLog.h"


#ifdef _USE_TRACE_THREAD
void RELEASE_TRACE(const char* format,...);
class CReleaseTrace
{
public:
    CReleaseTrace(DWORD id,const char* funcname,int line);
    ~CReleaseTrace();
private:
    DWORD m_id;
    std::string m_funcname;
    int m_line;
};
#endif


#define USEROPERLOGPATH "UserOper\\"


class GlobalFunc
{
public:
	static zqWriteLog& GetUserOperWriteLog();
	static void ReleaseUserOperWriteLog();
	static CUserOperLog& GetUserOperLog();
	static void ReleaseUserOperLog();
	static BOOL MakeUserOperDirection();
	static string GetUserOperFileName();

	static std::string GetSysPath();
	static void GetPath( std::string &strFilePath );
	static void GetFileName(std::string& strFileName);
	static bool IsFileExists( const std::string &strFilePath );
	static std::string GetValidName(const std::string strInName);
	template <class T> 
	static std::string ConvertToString( T value, int nDigit = 2 )
	{
		std::stringstream ss; 
		if(typeid(T)==typeid(float)||typeid(T)==typeid(double))
		{
			unsigned char invalid_dbl[8]={0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC};
			if(value!=std::numeric_limits<T>::quiet_NaN() 
				&&value!=std::numeric_limits<T>::signaling_NaN()
				&&value!=std::numeric_limits<T>::infinity()
				&&value!=*((T*)&invalid_dbl))
			{
				char szBuf[1024];
				memset( szBuf, 0, sizeof( szBuf ) );
				if(nDigit==3)
					sprintf_s(szBuf,"%.3f",value);
				else  if(nDigit==4)
					sprintf_s(szBuf,"%.4f",value);
				else
					sprintf_s(szBuf,"%.2f",value);
				for(int pos=strlen(szBuf)-1;pos>=0;pos--)
				{
					if(szBuf[pos]=='0')
					{
						szBuf[pos]=0;
					}
					else
					{
						if(szBuf[pos]=='.')
						{
							szBuf[pos]=0;
						}
						break;
					}
				}
				ss<<szBuf;
			}
			else
				ss<<0;
		}
		else 
			ss << value;
		return ss.str();
	}
	static void* GetXmlFileResource(const char* lpName,const char*  lpType,unsigned long* lpSize=NULL);
	//获取货币的分段显示格式
	static string GetAccountFormatString(double dblAmount,int precision=-1);
    //获取货币的分段显示格式
    string GetAccountFormatString2(double dblAmount,int precision=-1);


public:
	static zqWriteLog* sm_pUserOperWriteLog;
	static CUserOperLog* sm_pUserOperLog;

};

//#define USERLOG_INFO(fmt, ...) \
//		GlobalFunc::GetUserOperWriteLog().WriteLog_Fmt("UserOperate", LOGLEVEL_DEBUGINFO, fmt, __VA_ARGS__);
#define USERLOG_INFO(fmt, ...) \
		GlobalFunc::GetUserOperLog().WriteLog_Fmt(fmt, __VA_ARGS__);


#endif