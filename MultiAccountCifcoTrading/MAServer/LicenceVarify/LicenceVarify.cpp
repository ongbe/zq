// LicenseVarify.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <fstream>
#include <vector>
#include <map>
#include "CommonPkg.h"
#include "LicenceVarify.h"
#include "MD5/MD5Checksum.h"
#include "IniFile.h"

using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

CWriteLog* g_pWriteLog = NULL;
char* g_pSem = NULL;
int g_SemCount = 0;
std::map<SOCKET,Stru_DataSocket> g_TcpUsers;					//已经建立tcp连接的用户
std::vector<Stru_Licence> g_vLicence;
std::map<std::string, Stru_ServerAddr> g_serverAddrMap;

// This is an example of an exported variable
LICENSEVARIFY_API int nLicenseVarify=0;

// This is an example of an exported function.
LICENSEVARIFY_API int fnLicenseVarify(void)
{
	return 42;
}

void LoadLicences();
void LoadServerInfo();
string GetSysPath();

LICENSEVARIFY_API void InitFunc( void )
{
	g_pSem = CTools_Win32::AllocWinSem();
	g_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"LicenceVarify.log");

	//获取服务地址列表
	//获取Licence列表
	LoadLicences();

	LoadServerInfo();

}

LICENSEVARIFY_API void ReleaseFunc( void )
{
	if(g_pWriteLog != NULL)
	{
		g_pWriteLog->WriteLog_Fmt("关闭服务", WriteLogLevel_DEBUGINFO, "Line=%d", __LINE__ );
		delete g_pWriteLog;
		g_pWriteLog = NULL;
	}

	if ( g_pSem != NULL )
	{
		CTools_Win32::ReleaseWinSem(g_pSem);
		g_pSem = NULL;
		g_SemCount=0;
	}
}

void LoadLicences()
{
	string strSysPath = GetSysPath();
	string strLicencePath = strSysPath;
	strLicencePath += "\\licence.txt";

	locale::global(locale(""));
	fstream file;
	file.open( strLicencePath.c_str(), ios_base::in );
	if ( !file.is_open())
	{
		locale::global(locale("C"));
		return;
	}

	char szTemp[100];
	memset(szTemp, 0, 100);
	while(file.getline(szTemp, sizeof(szTemp)))
	{
		Stru_Licence licence;
		memset( &licence, 0, sizeof(licence));

		string strLine(szTemp);
		int nTemp = strLine.find(',');
		if ( nTemp != -1 )
		{
			string strAppID = strLine.substr(0, nTemp);
			string strLicence = strLine.substr(nTemp + 1, strLine.length());
			string strMD5Licence = CMD5Checksum::GetMD5((BYTE*)strLicence.c_str(), strLicence.length());
			strcpy_s(licence.szAppID, sizeof(licence.szAppID) - 1, strAppID.c_str());
			strcpy_s(licence.szLicence, sizeof(licence.szLicence) - 1, strMD5Licence.c_str());
			
			g_vLicence.push_back(licence);
		}

		memset(szTemp, 0, sizeof(szTemp));
	}

	file.close();
	locale::global(locale("C"));
}

void LoadServerInfo()
{
	string strSysPath = GetSysPath();
	string strIniPath = strSysPath;
	strIniPath += "\\function.ini";
	CIniFile *pIniFile = new CIniFile(strIniPath.c_str());
	if(!pIniFile) return;

	//顺序读取配置的服务器功能
	int FuncOrd=0;
	while(true)
	{
		char NodeName[256]="Function";
		if(!pIniFile->HasIniRootNode(NodeName,FuncOrd))
			break;

		char serverName[256];
		memset(serverName, 0, sizeof(serverName));
		pIniFile->GetIniKeyStr(NodeName,"DllName","",serverName,sizeof(serverName),0,FuncOrd);

		Stru_ServerAddr addr;
		memset(&addr, 0, sizeof(addr));
		addr.TCPIP=pIniFile->GetIniKeyIP(NodeName,"ip_TcpSvr","0.0.0.0",0,FuncOrd);
		addr.TCPPort=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_TcpSvr",0,0,FuncOrd);
		addr.UDPIP=pIniFile->GetIniKeyIP(NodeName,"ip_UdpSvr","0.0.0.0",0,FuncOrd);
		addr.UDPPort=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_UdpSvr",0,0,FuncOrd);

		g_serverAddrMap[serverName] = addr;

		FuncOrd++;
	};
}

string GetSysPath()
{
	char szBuffer[MAX_PATH];
	memset( szBuffer, 0, sizeof(szBuffer));
	::GetModuleFileName(NULL, szBuffer, sizeof(szBuffer));
	string strSysPath(szBuffer);
	int nPos = strSysPath.rfind('\\');
	if ( -1 != nPos )
	{
		strSysPath = strSysPath.substr(0, nPos);
	}

	return strSysPath;
}

// This is the constructor of a class that has been exported.
// see LicenseVarify.h for the class definition
CLicenseVarify::CLicenseVarify()
{
	return;
}
