#ifndef __GLOBALFUNC_H__
#define __GLOBALFUNC_H__

#include <iostream>
#include <sstream>
#include <limits>
#include "GLobalDefines.h"
#include "tools_util.h"

#include "EventParam.h"
#include "WriteLog.h"
#include "FileOpr.h"






#ifdef WIN32
#pragma managed(push,off)
#endif 

class GlobalFunc
{
public:
	static void GetPath( std::string &strFilePath )
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
    };

	static void GetFileName(std::string& strFileName)
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
    };

	static bool IsFileExists( const std::string &strFilePath )
    {
        OFSTRUCT OfStruct;
        HFILE hF=OpenFile(strFilePath.c_str(),&OfStruct,OF_EXIST);
        return hF<0?false:true;
    };

	static std::string GetValidName(const std::string strInName)
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
    };

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

    static void* GetXmlFileResource(const char* lpName,const char*  lpType,unsigned long* lpSize=NULL)
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
    };

	//获取货币的分段显示格式
	static std::string GetAccountFormatString(double dblAmount,int precision=-1)
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

        if(strcmp(szBuf,"-0")==0) strcpy(szBuf,"0");
        else if(strcmp(szBuf,"-0.00")==0) strcpy(szBuf,"0.00");

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
        return std::string(szBuf);
    }
    ////获取货币的分段显示格式
    //std::string GetAccountFormatString2(double dblAmount,int precision=-1);

    //判断一个合约是否是组合单合约。如果是，返回单腿合约名称。
    //目前仅能判断大商和郑商的组合合约
    //判断依据是前有空格后有&，如SP c1305&c1309
    static bool IsCombInstrument(const std::string& strInstrument,std::string& LegInstrument1,std::string& LegInstrument2)
    {
        int len=strInstrument.length();
        int pos1=strInstrument.find(" ");
        if(pos1==-1) return false;
        int pos2=strInstrument.find("&",pos1);
        if(pos2==-1) return false;
        LegInstrument1=strInstrument.substr(pos1+1,pos2-pos1-1);
        LegInstrument2=strInstrument.substr(pos2+1,len-pos2-1);
        return true;
    }
    //判断一个合约是否是组合单合约。
    //目前仅能判断大商和郑商的组合合约
    //判断依据是前有空格后有&，如SP c1305&c1309
    static bool IsCombInstrument2(const std::string& strInstrument)
    {
        int len=strInstrument.length();
        int pos1=strInstrument.find(" ");
        if(pos1==-1) return false;
        int pos2=strInstrument.find("&",pos1);
        if(pos2==-1) return false;
        return true;
    }

};

#define USERLOG_INFO(fmt, ...) \
    CFileOpr::getObj().writelocallogDaily2(true,"UserOper","UserOperate",fmt,__VA_ARGS__);

#define FROMORDERLOG_INFO(fmt, ...) \
    CFileOpr::getObj().writelocallogDaily2(true,"FromOrder","UserOperate",fmt,__VA_ARGS__);


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif