// SocketBase.h: interface for the CSocketBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETBASE_H__7B3C46D4_52DB_4B35_9F44_66F35B8EF1E4__INCLUDED_)
#define AFX_SOCKETBASE_H__7B3C46D4_52DB_4B35_9F44_66F35B8EF1E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <time.h>
#include <set>
#include <map>
#include <string>
using std::string;
#include "public.h"
#include "writelog.h"
#include "easymutex.h"
#include "tools_ansi.h"

typedef unsigned int          SOCKET;

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET  (SOCKET)(~0)
#endif 

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR            (-1)
#endif

#ifdef WIN32
#pragma managed(push,off)
#endif 


//IP address
struct Stru_IPAddress
{
	Stru_IPAddress(void)
	{
		IP=Port=0;
	};
	Stru_IPAddress(unsigned int iIP,unsigned short iPort)
	{
		IP=iIP;
		Port=iPort;
	};
    //从"www.sina.com:9999"或"www.sina.com"构造地址
    Stru_IPAddress(const string& straddr)
    {
        if(straddr.empty())
        {
            IP=Port=0;
        }
        else
        {
            int pos=straddr.find(":");
            if(pos==string::npos)
            {
                CTools_Ansi::GetIPFromHostName2(straddr.c_str(),IP);
                Port=0;
            }
            else
            {
                CTools_Ansi::GetIPFromHostName2(straddr.substr(0,pos).c_str(),IP);
                Port=atoi(straddr.substr(pos+1).c_str());
            }
        }
    };
    Stru_IPAddress& operator= (const Stru_IPAddress& other)
    {
        if (this == &other)
            return *this;

        IP=other.IP;
        Port=other.Port;
        return *this;
    };

    bool operator< (const Stru_IPAddress& r) const
    {
        if(IP<r.IP)
            return true;
        else if(Port<r.Port)
            return true;
        else
			return false;
    }

    bool operator== (const Stru_IPAddress& r) const
    {
        if(IP!=r.IP)
            return false;
        else if(Port!=r.Port)
            return false;
		else 
			return true;
    }

    bool operator!= (const Stru_IPAddress& r) const
    {
		return !((*this)==r);
    }

    bool operator> (const Stru_IPAddress& r) const
    {
        return !((*this) < r || (*this) == r);
    }
    //获取ip:port的串
    char* getstring(char* pbuf,int bufsize) const
    {
        if(bufsize>0)
        {
            _snprintf(pbuf,bufsize,"%s:%d",CTools_Ansi::ConvertdwIP(IP),Port);
            pbuf[bufsize-1]=0;
        }
        return pbuf;
    }
    string tostring(void) const
    {
        char buf[64]={0};
        return string(getstring(buf,sizeof(buf)-1));
    }
    void clear(void)
    {
        IP=0;
        Port=0;
    }


	unsigned int	IP;
	unsigned short	Port;
};

struct Stru_SocketRecvDataCallBackParam
{
    Stru_SocketRecvDataCallBackParam()
    {
        reset();
    }
    void reset()
    {
        memset(this,0,sizeof(*this));
    }
    void set(char *pdata,int dataLen,const Stru_IPAddress& localAddr,const Stru_IPAddress& remoteAddr,SOCKET hsocket)
    {
        pData=pdata;
        DataLen=dataLen;
        LocalAddr=localAddr;
        RemoteAddr=remoteAddr;
        hSocket=hsocket;
        bValid=true;
    }

    char *pData;
    int DataLen;

    Stru_IPAddress LocalAddr;
    Stru_IPAddress RemoteAddr;
    SOCKET hSocket;
    bool bValid;
};

struct Stru_SocketCallBackParam
{
    Stru_SocketCallBackParam()
    {
        reset();
    }
    void reset()
    {
        memset(this,0,sizeof(*this));
    }
    void set(const Stru_IPAddress& localAddr,const Stru_IPAddress& remoteAddr,SOCKET hsocket)
    {
        LocalAddr=localAddr;
        RemoteAddr=remoteAddr;
        hSocket=hsocket;
        bValid=true;
    }
    Stru_IPAddress LocalAddr;
    Stru_IPAddress RemoteAddr;
    SOCKET hSocket;
    bool bValid;
};

struct Stru_SocketCallBackParam2
{
    Stru_SocketCallBackParam2()
    {
        memset(this,0,sizeof(*this));
    };
    Stru_SocketCallBackParam2(int dataLen,const Stru_IPAddress& localAddr,const Stru_IPAddress& remoteAddr,SOCKET hsocket,int userData)
    {
        DataLen=dataLen;
        LocalAddr=localAddr;
        RemoteAddr=remoteAddr;
        hSocket=hsocket;
        UserData=userData;
    };

    int DataLen;
    Stru_IPAddress LocalAddr;
    Stru_IPAddress RemoteAddr;
    SOCKET hSocket;
    int UserData;
};


class CSocketBase  
{
public:
	CSocketBase(int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
	virtual ~CSocketBase();

    static int SendDataTcp(SOCKET hSocket,const void* pData,int DataLen);
    static int SendDataUdp(SOCKET hSocket,Stru_IPAddress RemoteAddr,const void* pData,int DataLen);
    static void CloseSocket(SOCKET hSocket);


protected:
	bool GetLocalMachineIP(void);

	SOCKET CreateUdpSocket(void);

	SOCKET CreateTcpSocket(void);

	SOCKET AcceptTcpSocket(SOCKET ListenSocket,unsigned int& RemoteIP,unsigned short& RemotePort);

protected:
	DWORD							m_LocalIP[5];
	time_t							m_GetLocalIPTime;

	CWriteLog						*m_pWriteLog;							//日志接口
	Ceasymutex						m_Sem;									//读写互斥锁

	const int						m_SOSNDBUFSize;
	const int						m_SORCVBUFSize;

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_SOCKETBASE_H__7B3C46D4_52DB_4B35_9F44_66F35B8EF1E4__INCLUDED_)
