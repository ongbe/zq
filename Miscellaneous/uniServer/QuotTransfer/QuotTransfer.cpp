// QuotTransfer.cpp : Defines the exported functions for the DLL application.
//



#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <windows.h>

#include "stdafx.h"
#include "QuotTransfer.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif


std::map<std::string,Stru_Quot2> g_Quots;
std::set<std::string> g_MainInstruments;	                //主力合约集
std::map<std::string,std::string> g_ValidUsersI;	        //合法的I类用户
std::map<std::string,std::string> g_ValidUsersII;	        //合法的II类用户
std::map<SOCKET,Stru_DataSocket> g_TcpUsers;				//已经建立tcp连接的用户
CWriteLog *g_pWriteLog=NULL;
char	*g_pSem=NULL;										//读写互斥锁
CFixedPackage *g_pQuotCache=NULL;
int g_HeartbeatTimeout=0;

char    *g_IBContractsInfo=NULL;							//IB合约信息内容
int		g_IBContractsLen=0;									//IB合约信息内容长度，字节数
std::map<std::string,std::string> g_IBContractsInfoReqUser;	//合法的请求IB合约信息的用户

char    *g_CTPMainContractsInfo=NULL;					    //CTP主力合约信息内容
int		g_CTPMainContractsLen=0;						    //CTP主力合约信息内容长度，字节数
std::map<std::string,std::string> g_CTPMainContractsInfoReqUser;	//合法的请求CTP主力合约信息的用户




QUOTTRANSFER_API void InitFunc(void)
{
	char Buf[MAX_PATH*2];
	Stru_IPAddress UdpLocalAddr;
	Stru_IPAddress TcpSvrAddr;
	int  Count,i,UserNameLen,PWLen;
	char *pp;
	char UserName[32],PW[32];

	g_pQuotCache=new CFixedPackage(sizeof(Stru_Quot2),10000,0);
	g_Quots.clear();
	g_MainInstruments.clear();		//主力合约集
	g_ValidUsersI.clear();			//合法的I类用户
	g_ValidUsersII.clear();			//合法的II类用户
	g_IBContractsInfoReqUser.clear();//合法的IB合约信息请求用户
	g_CTPMainContractsInfoReqUser.clear();//合法的CTP主力合约信息请求用户
	g_TcpUsers.clear();
	g_pSem = CTools_Win32::AllocWinSem();


    char MainPath[MAX_PATH];

	//获取程序路径
	CTools_Win32::GetMainPath(MainPath,sizeof(MainPath)-1);

    CIniFile *pIniFile=NULL;

	//创建配置文件对象
	sprintf(Buf,"%squotationtransfer.ini",MainPath);
	pIniFile=new CIniFile(Buf);
	if(!pIniFile) return;

	g_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"quotationtransfer.log");

    g_HeartbeatTimeout=pIniFile->GetIniKeyVal("system","heartbeattimeout",0);

	//获取I类用户列表
	Count=pIniFile->GetKeyValCount("UsersI","user");
	for(i=0;i<Count;i++)
	{
		memset(Buf,0,sizeof(Buf));
		pIniFile->GetIniKeyStr("UsersI","user","",Buf,sizeof(Buf),i);
		pp=strstr(Buf,"/");
		if(strlen(Buf)>0&&pp!=NULL&&
			(UserNameLen=(pp-Buf))>0&&UserNameLen<sizeof(UserName)&&
			(PWLen=strlen(Buf)-UserNameLen-1)>0&&PWLen<sizeof(PW))
		{
			memcpy(UserName,Buf,UserNameLen);
			UserName[UserNameLen]=0;
			memcpy(PW,Buf+UserNameLen+1,PWLen);
			PW[PWLen]=0;
			CTools_Ansi::DeleteStrLeftRightSpace(UserName);
			CTools_Ansi::DeleteStrLeftRightSpace(PW);
			g_ValidUsersI[std::string(UserName)]=std::string(PW);
		}
	}
	//获取II类用户列表
	Count=pIniFile->GetKeyValCount("UsersII","user");
	for(i=0;i<Count;i++)
	{
		memset(Buf,0,sizeof(Buf));
		pIniFile->GetIniKeyStr("UsersII","user","",Buf,sizeof(Buf),i);
		pp=strstr(Buf,"/");
		if(strlen(Buf)>0&&pp!=NULL&&
			(UserNameLen=(pp-Buf))>0&&UserNameLen<sizeof(UserName)&&
			(PWLen=strlen(Buf)-UserNameLen-1)>0&&PWLen<sizeof(PW))
		{
			memcpy(UserName,Buf,UserNameLen);
			UserName[UserNameLen]=0;
			memcpy(PW,Buf+UserNameLen+1,PWLen);
			PW[PWLen]=0;
			CTools_Ansi::DeleteStrLeftRightSpace(UserName);
			CTools_Ansi::DeleteStrLeftRightSpace(PW);
			g_ValidUsersII[std::string(UserName)]=std::string(PW);
		}
	}
	//获取主力合约列表
	Count=pIniFile->GetKeyValCount("mainInstruments","Instrument");
	for(i=0;i<Count;i++)
	{
		memset(Buf,0,sizeof(Buf));
		pIniFile->GetIniKeyStr("mainInstruments","Instrument","",Buf,sizeof(Buf),i);
		if(strlen(Buf)>0&&strlen(Buf)<32)
		{
			g_MainInstruments.insert(std::string(Buf));
		}
	}


	//获取IB合约信息相关参数。注意，这里g_IBContractsInfo已经包含包头、Cmd、DataLen和包尾了
	g_IBContractsInfo=NULL;
	g_IBContractsLen=0;

	HFILE hFTmp;
	char FileName[MAX_PATH*2],Buf2[MAX_PATH];
	memset(Buf2,0,sizeof(Buf2));
	pIniFile->GetIniKeyStr("IBContractsInfo","file","",Buf2,sizeof(Buf2),0);
	sprintf(FileName,"%s%s",MainPath,Buf2);
	hFTmp=_lopen(FileName,OF_READ);
	if(hFTmp!=NULL)
	{
		int fileLen=_llseek(hFTmp,0,2);
		char *pp=(char*)malloc(20+fileLen+4);
		if(pp)
		{
			_llseek(hFTmp,0,0);
			g_IBContractsInfo=pp;
			g_IBContractsLen=20+fileLen+4;

			*(DWORD*)g_IBContractsInfo=(DWORD)PACKAGEHEAD;
			*(int*)(g_IBContractsInfo+4)=(int)(20+fileLen+4);
			*(DWORD*)(g_IBContractsInfo+8)=0;
			*(DWORD*)(g_IBContractsInfo+12)=(DWORD)CMD_IBContractsInfo;
			*(int*)(g_IBContractsInfo+16)=(int)fileLen;
			_lread(hFTmp,g_IBContractsInfo+20,fileLen);
			*(DWORD*)(g_IBContractsInfo+20+fileLen)=(DWORD)PACKAGETAIL;
		}
	}
	//获取IB合约信息用户列表
	Count=pIniFile->GetKeyValCount("IBContractsInfo","user");
	for(i=0;i<Count;i++)
	{
		memset(Buf,0,sizeof(Buf));
		pIniFile->GetIniKeyStr("IBContractsInfo","user","",Buf,sizeof(Buf),i);
		pp=strstr(Buf,"/");
		if(strlen(Buf)>0&&pp!=NULL&&
			(UserNameLen=(pp-Buf))>0&&UserNameLen<sizeof(UserName)&&
			(PWLen=strlen(Buf)-UserNameLen-1)>0&&PWLen<sizeof(PW))
		{
			memcpy(UserName,Buf,UserNameLen);
			UserName[UserNameLen]=0;
			memcpy(PW,Buf+UserNameLen+1,PWLen);
			PW[PWLen]=0;
			CTools_Ansi::DeleteStrLeftRightSpace(UserName);
			CTools_Ansi::DeleteStrLeftRightSpace(PW);
			g_IBContractsInfoReqUser[std::string(UserName)]=std::string(PW);
		}
	}


	//获取CTPMain合约信息相关参数。。注意，这里g_CTPMainContractsInfo已经包含包头、Cmd、DataLen和包尾了
	g_CTPMainContractsInfo=NULL;
	g_CTPMainContractsLen=0;
    //获取CTP主力合约列表
	memset(Buf2,0,sizeof(Buf2));
	pIniFile->GetIniKeyStr("CTPMainContractsInfo","file","",Buf2,sizeof(Buf2),0);
	sprintf(FileName,"%s%s",MainPath,Buf2);
	hFTmp=_lopen(FileName,OF_READ);
	if(hFTmp!=NULL)
	{
		int fileLen=_llseek(hFTmp,0,2);
		char *pp=(char*)malloc(20+fileLen+4);
		if(pp)
		{
			_llseek(hFTmp,0,0);
			g_CTPMainContractsInfo=pp;
			g_CTPMainContractsLen=20+fileLen+4;

			*(DWORD*)g_CTPMainContractsInfo=(DWORD)PACKAGEHEAD;
			*(int*)(g_CTPMainContractsInfo+4)=(int)(20+fileLen+4);
			*(DWORD*)(g_CTPMainContractsInfo+8)=0;
			*(DWORD*)(g_CTPMainContractsInfo+12)=(DWORD)CMD_CTPMainContractsInfo;
			*(int*)(g_CTPMainContractsInfo+16)=(int)fileLen;
			_lread(hFTmp,g_CTPMainContractsInfo+20,fileLen);
			*(DWORD*)(g_CTPMainContractsInfo+20+fileLen)=(DWORD)PACKAGETAIL;
		}
	}
	//获取CTP主力合约信息用户列表
	Count=pIniFile->GetKeyValCount("CTPMainContractsInfo","user");
	for(i=0;i<Count;i++)
	{
		memset(Buf,0,sizeof(Buf));
		pIniFile->GetIniKeyStr("CTPMainContractsInfo","user","",Buf,sizeof(Buf),i);
		pp=strstr(Buf,"/");
		if(strlen(Buf)>0&&pp!=NULL&&
			(UserNameLen=(pp-Buf))>0&&UserNameLen<sizeof(UserName)&&
			(PWLen=strlen(Buf)-UserNameLen-1)>0&&PWLen<sizeof(PW))
		{
			memcpy(UserName,Buf,UserNameLen);
			UserName[UserNameLen]=0;
			memcpy(PW,Buf+UserNameLen+1,PWLen);
			PW[PWLen]=0;
			CTools_Ansi::DeleteStrLeftRightSpace(UserName);
			CTools_Ansi::DeleteStrLeftRightSpace(PW);
			g_CTPMainContractsInfoReqUser[std::string(UserName)]=std::string(PW);
		}
	}

	if(pIniFile)
	{
		delete pIniFile;
		pIniFile=NULL;
	}

}

QUOTTRANSFER_API void ReleaseFunc(void)
{
	if(g_pWriteLog)
	{
		delete g_pWriteLog;
		g_pWriteLog=NULL;
	}
	if(g_pSem)
	{
		CTools_Win32::ReleaseWinSem(g_pSem);
		g_pSem = NULL;
	}
	if(g_pQuotCache)
	{
		delete g_pQuotCache;
		g_pQuotCache=NULL;
	}
	if(g_IBContractsInfo)
	{
		free(g_IBContractsInfo);
		g_IBContractsInfo=NULL;
	}
	if(g_CTPMainContractsInfo)
	{
		free(g_CTPMainContractsInfo);
		g_CTPMainContractsInfo=NULL;
	}

}
