// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ConfigMgr.h"
#include "MyXML.h"

#ifdef _DEBUG
	#pragma comment(lib, "Tools4clrD.lib")
#else
	#pragma comment(lib, "Tools4clr.lib")
#endif



#pragma unmanaged


//是否已经加载
bool CConfigManager::m_bLoaded=false;

//服务器地址
string CConfigManager::m_SvrIP("");
unsigned short CConfigManager::m_SvrPort=0;
//本地地址
string CConfigManager::m_LocalIP("");
unsigned short CConfigManager::m_LocalPort=0;

//内部变量
Ceasymutex CConfigManager::m_mutex;
unsigned int CConfigManager::m_UpdateCounter=0;


void CConfigManager::Load()
{
//	char szLocalPath[256];
//	memset(szLocalPath, 0, 256);
//	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );
	
//	string strPathFileName( szLocalPath );
//	int nPos = strPathFileName.rfind( '\\' );
//	if ( nPos == -1) return;
//	strPathFileName = strPathFileName.substr( 0, nPos + 1 ) + string("cfg.xml");

	string strPathFileName( "cfg.xml" );

	m_mutex.lock();

	if(1)
	{
		//保证doc在unlock()之前析构掉
		CMyXML myxml(strPathFileName);
		if(myxml.m_bvalid)
		{
			if(!myxml.GetNodeContent(m_SvrIP,"root","Server","IP"))
				m_SvrIP="";
			int tmpport;
			if(!myxml.GetNodeContentInt(tmpport,"root","Server","Port"))
				tmpport=0;
			m_SvrPort=(unsigned short)tmpport;

			if(!myxml.GetNodeContent(m_LocalIP,"root","Local","IP"))
				m_LocalIP="";
			if(!myxml.GetNodeContentInt(tmpport,"root","Local","Port"))
				tmpport=0;
			m_LocalPort=(unsigned short)tmpport;

			m_UpdateCounter++;
			m_bLoaded=true;
		}
	}

	m_mutex.unlock();
}


bool CConfigManager::GetSvrAddr(string& SvrIP,unsigned short& SvrPort)
{
	if(!m_bLoaded) { SvrIP=string("");  SvrPort=0;  return false; }
	m_mutex.lock();
	SvrIP=m_SvrIP;
	SvrPort=m_SvrPort;
	m_mutex.unlock();
	return true;
}

bool CConfigManager::GetLocalAddr(string& LocalIP,unsigned short& LocalPort)
{
	if(!m_bLoaded) { LocalIP=string("");  LocalPort=0;  return false; }
	m_mutex.lock();
	LocalIP=m_LocalIP;
	LocalPort=m_LocalPort;
	m_mutex.unlock();
	return true;
}
