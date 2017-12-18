// updatefasttrader.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "updatefasttrader.h"
#include "InternetGetFile.h"

#include "ProgressDlg.h"
#include   <fstream> 



#include "winsock2.h"
#include "iostream"
#pragma comment(lib,"ws2_32.lib")






using   namespace   std; 
#define MAX_LOADSTRING 100

IMPLEMENT_APP(CUpdateFasttraderApp)
bool LoadXmlConfig(std::string& strIP, int& nPort);
bool GetIPaddress(const char *pp, std::string& strIP);

CheckVersion g_check;
bool CUpdateFasttraderApp::OnInit()
{
	// 检查程序是否已经在运行，如是则直接退出
	wxString str;
	str.Printf(wxT("WPD800-50830-28132-57903-%d"), argc);
	HANDLE hSem = ::CreateSemaphore(NULL, 1, 1, str.c_str()); 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 		
		CloseHandle(hSem); 		
		return false; 
	} 
	g_check.ReadConfig();
	CProgressDlg* dlg=new CProgressDlg(NULL);		
	g_check.SetMsgWindow(dlg);	
	if(argc == 1)//不带参数，纯粹下载配置文件			
		g_check.m_bMainFileOut = true;		
	else
		dlg->SetMainProcExe(argv[1]);

	std::string strIP;
	int nPort;
	LoadXmlConfig(strIP, nPort);
	CTCPDataService::GetInstance()->Connect(strIP, nPort);		
	CTCPDataService::GetInstance()->RegisteHWnd((HWND)dlg->GetHWND());	


	if(argc == 1)
	{//不带参数，纯粹下载配置文件		
		 dlg->Show(false);
		return true;
	} 	
    dlg->Show();	
	return true;
}
bool LoadXmlConfig(std::string& strIP, int& nPort)
{
	string strClientName = "";
	std::string strPath;
	GetPath(strPath);
	strPath+=SERVERURL_FILENAME;
	TiXmlDocument myDocument(strPath.c_str()); 
	if(myDocument.LoadFile())
	{
		TiXmlElement *root = myDocument.RootElement();
		std::string rootVal = root->Value();
		if(rootVal != "update")
		{
			return false;
		}
		TiXmlNode* pchild =root->FirstChildElement("address");
		if(pchild)
		{
			const char *p =root->FirstChildElement("address")->GetText();
			if(p != NULL)
			{
				std::string str = p;
				int nPos = str.find(':');
				if ( nPos != -1 )
				{
					std::string strIPTemp = str.substr(0, nPos);
					GetIPaddress(strIPTemp.c_str(), strIP);
					nPort = atoi(str.substr(nPos+1, str.length()).c_str());					
				}
			}
		}
		else
		{
			OutputDebugString(_T("升级无法正常进行，AutoUpdate.xml文件里面没有IP地址信息"));
			return false;
		}
	}
	return true;
}
bool GetIPaddress(const char *pp, std::string& strIP)
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);
	hostent *phost=gethostbyname(pp);
	in_addr addr;
	for(int i=0;;i++)
	{
		char *p=phost->h_addr_list[i];
		if(p==NULL)
			break;
		memcpy(&addr.S_un.S_addr,p,phost->h_length); 
		char* ip=inet_ntoa(addr);
		strIP = ip;	
	}
	WSACleanup();
	return true;
}