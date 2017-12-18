// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "IniFile.h"
#include <stdio.h>


#ifdef WIN32
#pragma unmanaged
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(const char *pIniFileName,int iUseImgID)
{
	memset(m_IniFileName,0,sizeof(m_IniFileName));
	memset((char*)&m_IniFileStat,0xff,sizeof(m_IniFileStat));
	m_pIniFileImg=NULL;
	m_IniFileLen=0;
	m_UseImgID=0;
	m_pSem=CTools_Win32::AllocWinSem();


	if(pIniFileName)
	{
		strncpy(m_IniFileName,pIniFileName,sizeof(m_IniFileName)-1);
		m_IniFileName[sizeof(m_IniFileName)-1]=0;
	}

	m_UseImgID=iUseImgID;

}

CIniFile::~CIniFile()
{
	if(m_pIniFileImg)
	{
		free(m_pIniFileImg);
		m_pIniFileImg=NULL;
	}
	if(m_IniFileLen)
		m_IniFileLen=0;

	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}
}




//function		:LoadIniFileImg(void)
//description	:Load inifile content to img
void CIniFile::LoadIniFileImg(void)
{
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	LoadIniFileImg_Internal();
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


//	取带路径的ini文件名
//	结果放在RltIniFileName里，尺寸必须够大。搜索时首先在当前目录，然后在下级的config目录，最后在上级的config目录
//	还返回ini文件所在的路径
//	如果找不到，RltIniFileName为空
void CIniFile::GetIniFileName(char *RltIniFileName,char *RltIniFilePath,const char *pMainPath,const char *pIniFileName)
{
	FILE			*fp;

	sprintf(RltIniFileName,"%s%s",pMainPath,pIniFileName);
	fp=fopen(RltIniFileName,"rb");
	if(fp) fclose(fp);
	else RltIniFileName[0]=0;

	if(!RltIniFileName[0])
	{
		sprintf(RltIniFileName,"%sconfig\\%s",pMainPath,pIniFileName);
		fp=fopen(RltIniFileName,"rb");
		if(fp) fclose(fp);
		else RltIniFileName[0]=0;
	}

	if(!RltIniFileName[0])
	{
		sprintf(RltIniFileName,"%s..\\config\\%s",pMainPath,pIniFileName);
		fp=fopen(RltIniFileName,"rb");
		if(fp) fclose(fp);
		else RltIniFileName[0]=0;
	}

	if(!RltIniFileName[0])
	{
		sprintf(RltIniFileName,"%s..\\..\\config\\%s",pMainPath,pIniFileName);
		fp=fopen(RltIniFileName,"rb");
		if(fp) fclose(fp);
		else RltIniFileName[0]=0;
	}

	if(!RltIniFileName[0])
	{
		sprintf(RltIniFileName,"%s..\\..\\..\\config\\%s",pMainPath,pIniFileName);
		fp=fopen(RltIniFileName,"rb");
		if(fp) fclose(fp);
		else RltIniFileName[0]=0;
	}

	if(RltIniFileName[0]&&RltIniFilePath)
	{
		int			Len1,Len2;
		Len1=strlen(RltIniFileName);
		Len2=strlen(pIniFileName);

		strcpy(RltIniFilePath,RltIniFileName);
		RltIniFilePath[Len1-Len2]=0;
	}

}



//function		:IsIniFileExist
//description	:Tell you whether a ini file is exist
//parameters	:filename:name of ini file
bool CIniFile::IsIniFileExist(const char *filename)
{
	FILE			*fp; 
	bool			bRlt;

	if(!filename||!filename[0]) return 0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if((fp = fopen( filename, "rb")) == NULL ) 
		bRlt=false;
	else 
	{
		fclose(fp);
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//取指定的key_val对的数量
int CIniFile::GetKeyValCount(const char* title,const char* key,int TitleOrd)
{
	
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,keyLen,BufLen,i,iOrd;
	int				InSessionID;
	int				FileOff,iTitleOrd;
	bool			bRlt;
	int				Count;


	if(!title||!title[0]||!key||!key[0]||!m_IniFileName||!m_IniFileName[0]) return 0;

	Count=0;
	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return 0;
	keyLen=(int)strlen(key);
	if(keyLen>=sizeof(Buf)) return 0;
	iOrd=0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	fp=NULL;
	InSessionID=0;
	FileOff=0;
	iTitleOrd=0;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;

			BufLen=(int)strlen(Buf);

			if(!InSessionID&&BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)								//先找指定节的位置
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else InSessionID=1;												
			}
			else if(InSessionID&&Buf[0]=='[')
				break;
			else if(InSessionID&&memicmp(Buf,key,keyLen)==0)
			{
				for(i=keyLen;i<BufLen&&(Buf[i]==' '||Buf[i]=='\t');i++);
				if(Buf[i]=='='&&Buf[i+1]!='=')
				{
					Count++;
				}
			}
		}
		if(fp) fclose(fp);
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return Count;
}


//get configurre data in IP value from ini file
DWORD CIniFile::GetIniKeyIP(const char *title,const char *key,const char *defstr,int SearchOrd,int TitleOrd)
{
	char			Buf[256];
	DWORD			RltIP;

	memset(Buf,0,sizeof(Buf));
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,defstr,Buf,sizeof(Buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	if(!Buf[0]||strcmp(Buf,"0")==0||stricmp(Buf,"NULL")==0) RltIP=0;
	else RltIP=CTools_Ansi::ConvertStrIP(Buf);
	return RltIP;
}


//function		:GetIniKeyVal
//description	:1. get configure data in integer value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defstr: default value of configure data
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
//return		:integer value of configure data
int CIniFile::GetIniKeyVal(const char *title,const char *key,const char *defstr,int SearchOrd,int TitleOrd)
{
	char Buf[256]="";
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,defstr,Buf,sizeof(Buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return atoi(Buf);
}



//function		:GetIniKeyVal
//description	:1. get configure data in integer value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defval: default value of configure data
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
//return		:integer value of configure data
int CIniFile::GetIniKeyVal(const char *title,const char *key,int defval,int SearchOrd,int TitleOrd)
{
	char Buf[256],DefBuf[64];
	sprintf(DefBuf,"%d",defval);
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,DefBuf,Buf,sizeof(Buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return atoi(Buf);
}

double CIniFile::GetIniKeyVal2(const char *title,const char *key,double defval,int SearchOrd,int TitleOrd)
{
	char buf[256],defbuf[256];
	sprintf_s(defbuf,sizeof(defbuf)-1,"%lf",defval);
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,defbuf,buf,sizeof(buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return atof(buf);
}

std::vector<double> CIniFile::GetIniKeyVal3(const char *title,const char *key,int SearchOrd,int TitleOrd)
{
	std::vector<double> vecrlt;
	char buf[10240];
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,"",buf,sizeof(buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	int i=0,len=strlen(buf);
	double tmpval;
	while(i<len)
	{
		if(sscanf_s(buf+i,"%lf",&tmpval,len-i)!=0)
			vecrlt.push_back(tmpval);

		for(;i<len&&buf[i]!=' '&&buf[i]!=','&&buf[i]!=';';i++);
		for(;i<len&&(buf[i]==' '||buf[i]==','||buf[i]==';');i++);
	}
	return vecrlt;
}

std::vector<int> CIniFile::GetIniKeyVal4(const char *title,const char *key,int SearchOrd,int TitleOrd)
{
	std::vector<int> vecrlt;
	char buf[10240];
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,"",buf,sizeof(buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	int i=0,len=strlen(buf);
	int tmpval;
	while(i<len)
	{
		if(sscanf_s(buf+i,"%d",&tmpval,len-i)!=0)
			vecrlt.push_back(tmpval);

		for(;i<len&&buf[i]!=' '&&buf[i]!=','&&buf[i]!=';';i++);
		for(;i<len&&(buf[i]==' '||buf[i]==','||buf[i]==';');i++);
	}
	return vecrlt;
}


//function		:GetIniKeyChar
//description	:1. get configure data in char value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defval: default value of configure data
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
//return		:integer value of configure data
char CIniFile::GetIniKeyChar(const char *title,const char *key,char defchar,int SearchOrd,int TitleOrd)
{
	char Buf[256],DefBuf[64];
	DefBuf[0]=defchar;
	DefBuf[1]=0;
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,DefBuf,Buf,sizeof(Buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return Buf[0];
}




//function		:GetIniKey_time_t
//description	:1. get configure data in time_t value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defval: default value of configure data
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
//return		:integer value of configure data
//	日期格式为：xxxx年xx月xx日； xxxx/xx/xx； xxxx-xx-xx
//	时间格式为：xx时xx分xx秒； xx:xx:xx； xx:xx
int CIniFile::GetIniKey_time_t(const char *title,const char *key,char *deftm,int SearchOrd,int TitleOrd)
{
	char Buf[256];

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,deftm,Buf,sizeof(Buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	if(Buf[0]==0) return 0;
	return (int)(CTools_Ansi::GetAnsiTimeFromString(Buf));
}





//function		:GetIniKeyColor
//description	:1. get configure data in color value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defcolor: default value of configure data
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
//return		:integer value of configure data
unsigned int CIniFile::GetIniKeyColor(const char *title,const char *key,unsigned int defcolor,int SearchOrd,int TitleOrd)
{
	char Buf[256],DefBuf[64];
	unsigned int iR,iG,iB;
	int  iOff,iLen;

	iR=(unsigned int)(defcolor&0xff);
	iG=(unsigned int)((defcolor&0xff00)>>8);
	iB=(unsigned int)((defcolor&0xff0000)>>16);

	memset(Buf,0,sizeof(Buf));
	sprintf(DefBuf,"%u,%u,%u",iR,iG,iB);
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,DefBuf,Buf,sizeof(Buf)-1,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	iR=iG=iB=0;
	iLen=strlen(Buf);
	iOff=0;
	while(iOff<iLen&&(Buf[iOff]<'0'||Buf[iOff]>'9')) iOff++;
	if(iOff>=0&&iOff<iLen) iR=(unsigned int)atoi(Buf+iOff);
	while(iOff<iLen&&Buf[iOff]>='0'&&Buf[iOff]<='9') iOff++;
	while(iOff<iLen&&(Buf[iOff]<'0'||Buf[iOff]>'9')) iOff++;
	if(iOff>=0&&iOff<iLen) iG=(unsigned int)atoi(Buf+iOff);
	while(iOff<iLen&&Buf[iOff]>='0'&&Buf[iOff]<='9') iOff++;
	while(iOff<iLen&&(Buf[iOff]<'0'||Buf[iOff]>'9')) iOff++;
	if(iOff>=0&&iOff<iLen) iB=(unsigned int)atoi(Buf+iOff);

	return (iR|(iG<<8)|(iB<<16));
}





//function		:GetIniKeyStr
//description	:1. get configure data in string value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defstr: default value of configure data
//				 RltBuf: prointer of buffer 
//				 RltSize: size of buffer
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
void CIniFile::GetIniKeyStr(const char *title,const char *key,const char *defstr,char *RltBuf,int RltSize,int SearchOrd,int TitleOrd) 
{ 
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	GetIniKeyStr_Internal(title,key,defstr,RltBuf,RltSize,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
} 


//	function		:GetKey_Val
//	找指定title指定SearchOrd的key
bool CIniFile::GetKey_Val(const char *title,char *RltKey,int RltKeySize,char *RltBuf,int RltBufSize,int SearchOrd,int TitleOrd) 
{ 
	
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,KeyLen,BufLen,RltPos,EndPos,RltLen,i;
	int				InSessionID,iOrd,iTitleOrd;
	int				FileOff;
	bool			bRlt;


	if(!RltKey||RltKeySize<0||!RltBuf||RltBufSize<=0) return false;

	iTitleOrd=0;
	iOrd=0;
	RltKey[0]=0;
	KeyLen=0;
	RltBuf[0]=0;
	RltLen=0;

	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return false;
	if(!m_IniFileName||!m_IniFileName[0]) return false;


	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	fp=NULL;
	InSessionID=0;
	FileOff=0;
	RltPos=-1;
	bRlt=false;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;

			BufLen=(int)strlen(Buf);

			if(!InSessionID&&BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)								//先找指定节的位置
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else InSessionID=1;												
			}
			else if(InSessionID&&Buf[0]=='[')
				break;
			else if(InSessionID)
			{
				for(i=0;i<BufLen;i++)
				{
					if(Buf[i]=='='&&Buf[i+1]!='=')
						break;
				}

				KeyLen=i;
				while(KeyLen>0&&(Buf[KeyLen-1]==' '||Buf[KeyLen-1]=='\t')) KeyLen--;

				if(KeyLen>0&&iOrd<SearchOrd)
					iOrd++;
				else if(KeyLen>0)
				{
					if(KeyLen>=RltKeySize) KeyLen=RltKeySize-1;
					memcpy(RltKey,Buf,KeyLen);
					RltKey[KeyLen]=0;
					CTools_Ansi::DeleteStrRightSpace(RltKey);

					RltPos=i+1;													//Rlt的起点是RltPos
					FilterContent_Internal(Buf,BufLen,RltPos,EndPos);
					RltLen=EndPos-RltPos;
					if(RltLen>0)
					{
						memcpy(RltBuf,Buf+RltPos,RltLen);							//将结果字符串放在RltBuf中
						RltBuf[RltLen]=0;
					}
					bRlt=true;
					break;
				}
			}
		}
		if(fp) fclose(fp);
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
} 

//function		:HasIniRootNode
//description	:1. Tell you whether a rootnode is in the inifile
//parameters	:title: title of configure data in ini file
bool CIniFile::HasIniRootNode(const char *title,int TitleOrd) 
{ 
	
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,BufLen;
	int				InSessionID;
	int				FileOff,iTitleOrd;
	bool			bRlt;


	if(!m_IniFileName||!m_IniFileName[0]||!title||!title[0]) return 0;


	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return 0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	iTitleOrd=0;
	fp=NULL;
	InSessionID=0;
	FileOff=0;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;

			BufLen=(int)strlen(Buf);

			if(BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else
				{
					InSessionID=1;
					break;
				}
			}
		}
		if(fp) fclose(fp);
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return InSessionID?true:false;
} 


//	function	:HasIniKey
//	description	:Tell you whether a title/key is in the inifile
//	return		:rtn 1:title/item is in the inifile; 0:isn't in
bool CIniFile::HasIniKey(const char *title,const char *key,int SearchOrd,int TitleOrd)
{
	
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,keyLen,BufLen,i,iOrd;
	int				InSessionID,MatchKeyID;
	int				FileOff,iTitleOrd;
	bool			bRlt;


	if(!title||!title[0]||!key||!key[0]||!m_IniFileName||!m_IniFileName[0]) return 0;

	MatchKeyID=0;
	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return 0;
	keyLen=(int)strlen(key);
	if(keyLen>=sizeof(Buf)) return 0;
	iOrd=0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	fp=NULL;
	InSessionID=0;
	FileOff=0;
	iTitleOrd=0;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;

			BufLen=(int)strlen(Buf);

			if(!InSessionID&&BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)								//先找指定节的位置
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else InSessionID=1;												
			}
			else if(InSessionID&&Buf[0]=='[')
				break;
			else if(InSessionID&&memicmp(Buf,key,keyLen)==0)
			{
				for(i=keyLen;i<BufLen&&(Buf[i]==' '||Buf[i]=='\t');i++);
				if(Buf[i]=='='&&Buf[i+1]!='=')
				{
					if(iOrd<SearchOrd)
						iOrd++;
					else 
					{
						MatchKeyID=1;
						break;
					}
				}
			}
		}
		if(fp) fclose(fp);
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return MatchKeyID?true:false;
}


//function		:IsIniFileUpdate
//description	:tell you whether the inifile was update 
bool CIniFile::IsIniFileUpdate(void)
{
	struct _stat	CurIniFileStat;
	bool			bRlt;

	memset((char*)&CurIniFileStat,0,sizeof(CurIniFileStat));
   _stat(m_IniFileName,&CurIniFileStat);
   if(	CurIniFileStat.st_ctime!=m_IniFileStat.st_ctime||
		CurIniFileStat.st_mtime!=m_IniFileStat.st_mtime||
		CurIniFileStat.st_size!=m_IniFileStat.st_size)
		bRlt=true;
   else bRlt=false;
   memcpy((char*)&m_IniFileStat,(char*)&CurIniFileStat,sizeof(m_IniFileStat));
   return bRlt;
}



//	取一节的首尾位置
//	title:节名
//	pNodeStartPos，pNodeEndPos：接收节的首尾位置的指针
bool CIniFile::GetNodePos(const char *title,int *pNodeStartPos,int *pNodeEndPos,int TitleOrd)
{
	bool			bRlt;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	bRlt=GetNodePos_Internal(title,pNodeStartPos,pNodeEndPos,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return bRlt;
}


//	function	:GetKeyPos
//	description	:Get position of a key
//	取指定key行的首尾位置
//	title:节名
//	key:键名
//	pKeyStartPos，pKeyEndPos：接收键的首尾位置的指针
bool CIniFile::GetKeyPos(const char *title,const char *key,int *pKeyStartPos,int *pKeyEndPos,int SearchOrd,int TitleOrd)
{
	bool			bRlt;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	bRlt=GetKeyPos_Internal(title,key,pKeyStartPos,pKeyEndPos,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return bRlt;
}




//	删除指定名称的节
bool CIniFile::DeleteNode(const char *title,int TitleOrd)
{
	FILE			*fp; 
	char			*pBuf;
	int				FileLen;
	int				DelStart,DelEnd;
	bool			bRlt;


	if(!m_IniFileName||!title||!title[0]) return false;


	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	DelStart=DelEnd=-1;
	bRlt=GetNodePos_Internal(title,&DelStart,&DelEnd,TitleOrd);
	if(bRlt)
	{
		if((fp=fopen(m_IniFileName,"rb"))==NULL) 
			bRlt=false;																//没有指定文件
		else
		{
			fseek(fp,0,2);
			FileLen=ftell(fp);
			fseek(fp,0,0);
			pBuf=(char*)malloc(FileLen);
			if(!pBuf)
			{
				fclose(fp);
				bRlt=false;
			}
			else
			{
				fread(pBuf,1,FileLen,fp);
				fclose(fp);
				if((fp=fopen(m_IniFileName,"wb"))==NULL)
				{
					free(pBuf);
					bRlt=false;
				}
				else
				{
					if(DelStart>0)
						fwrite(pBuf,1,DelStart,fp);
					if(DelEnd>=0&&DelEnd<FileLen)
						fwrite(pBuf+DelEnd,1,FileLen-DelEnd,fp);
					fclose(fp);
					free(pBuf);
				}
				bRlt=true;
			}
		}
	}

	if(bRlt&&m_UseImgID)
		LoadIniFileImg_Internal();

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);


	return bRlt;
}


//	删除指定名称的key
bool CIniFile::DeleteKey(const char *title,const char *key,int SearchOrd,int TitleOrd)
{
	bool			bRlt;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	bRlt=DeleteKey_Internal(title,key,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return bRlt;
}

bool CIniFile::AddKeyStr(const char *title,const char *key,char *keystr,int TitleOrd)
{
	bool bRlt;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	bRlt=AddKeyStr_Internal(title,key,keystr,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

bool CIniFile::AddKeydwordVal(const char *title,const char *key,unsigned int keyval,int TitleOrd)
{
	char buf[256];
	sprintf(buf,"%u",keyval);
	return AddKeyStr(title,key,buf,TitleOrd);
}

bool CIniFile::AddKeyintVal(const char *title,const char *key,int keyval,int TitleOrd)
{
	char buf[256];
	sprintf(buf,"%d",keyval);
	return AddKeyStr(title,key,buf,TitleOrd);
}



//	function	:UpdateKeyStr
//	description	:update string of a key
//	title:节名
//	key:键名
//	keystr：键值
bool CIniFile::UpdateKeyStr(const char *title,const char *key,const char *keystr,int SearchOrd,int TitleOrd)
{
	bool			bRlt;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	bRlt=UpdateKeyStr_Internal(title,key,keystr,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
	return bRlt;
}


//	function	:UpdateKeyVal
//	description	:update val of a key
//	title:节名
//	key:键名
//	keyval：键值
bool CIniFile::UpdateKeyVal(const char *title,const char *key,int val,int SearchOrd,int TitleOrd)
{
	char			Buf[256];
	bool bRlt;

	sprintf(Buf,"%d",val);
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	bRlt=UpdateKeyStr_Internal(title,key,Buf,SearchOrd,TitleOrd);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}
bool CIniFile::UpdateKeyVal2(const char *title,const char *key,double val,int SearchOrd,int TitleOrd)
{
	char			Buf[256];
	sprintf(Buf,"%lf",val);
	return UpdateKeyStr(title,key,Buf,SearchOrd,TitleOrd);
}
bool CIniFile::UpdateKeyVal3(const char *title,const char *key,std::vector<double>& vals,int SearchOrd,int TitleOrd)
{
	char Buf[10240],buf[256];
	int i,len,len2;
	for(i=0,len=0;i<(int)vals.size();i++)
	{
		sprintf(buf,"%lf",vals[i]);
		len2=strlen(buf);
		if(len+1+1+len2<(int)sizeof(Buf))
		{
			if(len>0) Buf[len++]=';';
			memcpy(Buf+len,buf,len2);
			len+=len2;
		}
		else break;
	}
	Buf[len]=0;

	return UpdateKeyStr(title,key,Buf,SearchOrd,TitleOrd);
}
bool CIniFile::UpdateKeyVal4(const char *title,const char *key,std::vector<int>& vals,int SearchOrd,int TitleOrd)
{
	char Buf[10240],buf[256];
	int i,len,len2;
	for(i=0,len=0;i<(int)vals.size();i++)
	{
		sprintf(buf,"%d",vals[i]);
		len2=strlen(buf);
		if(len+1+1+len2<(int)sizeof(Buf))
		{
			if(len>0) Buf[len++]=';';
			memcpy(Buf+len,buf,len2);
			len+=len2;
		}
		else break;
	}
	Buf[len]=0;

	return UpdateKeyStr(title,key,Buf,SearchOrd,TitleOrd);
}
bool CIniFile::UpdateKeyVal5(const char *title,const char *key,std::vector<std::string>& vals,int SearchOrd,int TitleOrd)
{
    char Buf[10240],buf[256];
    int i,len,len2;
    for(i=0,len=0;i<(int)vals.size();i++)
    {
        if(strlen(vals[i].c_str())==0)
            sprintf(buf,"%s"," ");
        else 
            sprintf(buf,"%s",vals[i].c_str());

		len2=strlen(buf);
		if(len+1+1+len2<(int)sizeof(Buf))
		{
			if(len>0) Buf[len++]=';';
			memcpy(Buf+len,buf,len2);
			len+=len2;
		}
		else break;

    }
    Buf[len]=0;

    return UpdateKeyStr(title,key,Buf,SearchOrd,TitleOrd);
}
bool CIniFile::UpdatedwordKeyVal(const char *title,const char *key,unsigned int val,int SearchOrd,int TitleOrd)
{
	char			Buf[256];
	sprintf(Buf,"%u",val);
	return UpdateKeyStr(title,key,Buf,SearchOrd,TitleOrd);
}


bool CIniFile::AddOneNode(const char *pNodeBuf)
{
	FILE			*fp=NULL; 
	int				FileLen;
	bool			bRlt;
	char			Buf[2];


	if(!m_IniFileName) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	//打开ini文件，如果文件不存在，就创建之
	fp=fopen(m_IniFileName,"rb+");
	if(fp==NULL) 
		fp=fopen(m_IniFileName,"wb");
	if(fp==NULL)
	{
		char	Path[256];
		memset(Path,0,sizeof(Path));
		if(CTools_Ansi::GetFilePathWithDiskSymbol(m_IniFileName,Path,sizeof(Path)))
		{
			CTools_Win32::CreateMultipleDirectory(Path);
			fp=fopen(m_IniFileName,"wb");
		}
	}

	if(fp==NULL)
	{
		bRlt=false;
	}
	else
	{
		fseek(fp,0,2);
		FileLen=ftell(fp);
		if(FileLen>1)
		{
			fseek(fp,FileLen-2,0);
			fread(Buf,1,2,fp);
		}
		if(FileLen<2||Buf[0]!='\r'||Buf[1]!='\n')
		{
			fseek(fp,0,2);
			fwrite("\r\n",1,2,fp);
		}

		fwrite(pNodeBuf,1,strlen(pNodeBuf),fp);
		if(strlen(pNodeBuf)<2||pNodeBuf[strlen(pNodeBuf)-1]!='\n'||pNodeBuf[strlen(pNodeBuf)-1]!='\r')
			fwrite("\r\n",1,2,fp);

		fclose(fp);
		bRlt=true;
	}

	if(bRlt&&m_UseImgID)
		LoadIniFileImg_Internal();

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



//function		:GetTitleOrdByKey
//description	:1. 取含有指定Key值的Title的序号，用于有多个同名Title的ini
//return		:TitleOrd,-1为找不到
int CIniFile::GetTitleOrdByKey_Str(const char *title,const char *key,const char *pKeystr) 
{ 
	int RltOrd;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	RltOrd=GetTitleOrdByKey_Str_Internal(title,key,pKeystr);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return RltOrd;
} 

//function		:GetTitleOrdByVal
//description	:1. 取含有指定Key值的Title的序号，用于有多个同名Title的ini
//return		:TitleOrd,-1为找不到
int CIniFile::GetTitleOrdByVal(const char *title,const char *key,const int KeyVal) 
{
	char			Buf[256];
	int RltOrd;
	sprintf(Buf,"%d",KeyVal);

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	RltOrd=GetTitleOrdByKey_Str_Internal(title,key,Buf);
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return RltOrd;
}




//从文件中取出一行内容，如果m_UseImgID,则从文件映像中取
bool CIniFile::GetLineContent_Internal(char* pRltBuf,int RltSize,char* fp,int&FileOff)
{
	if(!pRltBuf||!fp&&!m_UseImgID) return false;

	memset(pRltBuf,0,RltSize);
	if(!m_UseImgID)
	{
		if(fseek((FILE*)fp,FileOff,SEEK_SET)!=0) 
			return false;

		if(feof((FILE*)fp))
			return false;
		fgets(pRltBuf,RltSize-1,(FILE*)fp);
		int OldOff=FileOff;
		FileOff=ftell((FILE*)fp);
		if(OldOff==FileOff)
			return false;
	}
	else
	{
		int i,Len;
		if(!m_pIniFileImg) return false;
		if(FileOff<=m_IniFileLen) return false;
		for(i=FileOff;i<m_IniFileLen&&m_pIniFileImg[i]!=10&&m_pIniFileImg[i]!=13;i++);
		if(i<m_IniFileLen-1&&m_pIniFileImg[i]==10&&m_pIniFileImg[i]==13) i+=2;
		else if(i<m_IniFileLen-1&&m_pIniFileImg[i]==13&&m_pIniFileImg[i]==10) i+=2;
		else if(i<m_IniFileLen&&(m_pIniFileImg[i]==10||m_pIniFileImg[i]==13)) i++;

		Len=i-FileOff;
		if(Len>=RltSize) Len=RltSize-1;
		memcpy(pRltBuf,m_pIniFileImg+FileOff,Len);
		pRltBuf[Len]=0;
		FileOff+=Len;
	}
	CTools_Ansi::DeleteStrLeftRightSpace_10_13(pRltBuf);
	return true;
}


//过滤ini内容，主要把注释内容过滤掉，如果在双引号中，则从双引号中取出字符串，最后过滤头尾的空格
void CIniFile::FilterContent_Internal(const char* Buf,int BufLen,int& StartPos,int& EndPos)
{
	int j,j1;

	EndPos=-1;

																						//过滤掉了前面的空格和tab
	for(;StartPos<BufLen&&(Buf[StartPos]==' '||Buf[StartPos]=='\t');StartPos++);

	if(Buf[StartPos]=='"')
	{
		for(j=StartPos+1,j1=-1;j<BufLen;j++)
		{															//找双引号的位置
			if(Buf[j]=='"')
			{
				j1=j;
				break;
			}
		}

																	//第二个双引号后是行末或注释的情况
		if(j1!=-1)
		{															//只有两个双引号的情况
			for(j=j1+1;j<BufLen&&(Buf[j]==' '||Buf[j]=='\t');j++);
			if(j==BufLen||j<BufLen-1&&(Buf[j]=='\\'&&Buf[j+1]=='\\'||Buf[j]=='/'&&Buf[j+1]=='/'))
			{
				StartPos++;
				EndPos=j1;
			}
		}
	}

	if(EndPos==-1)
	{
																	//过滤掉//开始的注释
		for(j=StartPos;j<BufLen-1;j++)
			if(Buf[j]=='\\'&&Buf[j+1]=='\\'||Buf[j]=='/'&&Buf[j+1]=='/') break;
		if(j<BufLen-1)
			EndPos=j;
	}

	if(EndPos==-1)
		EndPos=BufLen;


																//过滤掉了前面的空格和tab
	while(StartPos<EndPos&&(Buf[StartPos]==' '||Buf[StartPos]=='\t')) 
		StartPos++;													

	while(EndPos>StartPos&&(Buf[EndPos-1]==' '||Buf[EndPos-1]=='\t'))
		EndPos--;												//过滤掉尾部的空格和tab
}

bool CIniFile::AddKeyStr_Internal(const char *title,const char *key,const char *keystr,int TitleOrd)
{
	int				NodeStartPos,NodeEndPos,FileLen,BufLen;
	FILE			*fp;
	char			Buf[10240];
	char			*pBuf;
	bool			bRlt;

	if(!keystr) return false;
	if(strlen(keystr)+strlen(key)>=sizeof(Buf)-100) return false;

	bRlt=false;
	NodeStartPos=NodeEndPos=-1;
	pBuf=NULL;


	//如果需要，创建ini文件
	if(!GetNodePos_Internal(title,&NodeStartPos,&NodeEndPos,TitleOrd))
	{
		fp=fopen(m_IniFileName,"rb+");
		if(fp==NULL) 
			fp=fopen(m_IniFileName,"wb");
		if(fp==NULL)
		{
			char	Path[256];
			memset(Path,0,sizeof(Path));
			if(CTools_Ansi::GetFilePathWithDiskSymbol(m_IniFileName,Path,sizeof(Path)))
			{
				CTools_Win32::CreateMultipleDirectory(Path);
				fp=fopen(m_IniFileName,"wb");
			}
		}

		if(fp)
		{
			//添加[...]
			fseek(fp,0,2);
			FileLen=ftell(fp);
			if(FileLen>1)
			{
				fseek(fp,FileLen-2,0);
				fread(Buf,1,2,fp);
			}
			if(FileLen==1||FileLen>1&&(Buf[0]!='\r'||Buf[1]!='\n'))
			{
				fseek(fp,0,2);
				fwrite("\r\n",1,2,fp);
			}
			sprintf(Buf,"[%s]\r\n",title);
			fseek(fp,0,2);
			fwrite(Buf,1,strlen(Buf),fp);
			fclose(fp);
		}

		GetNodePos_Internal(title,&NodeStartPos,&NodeEndPos,TitleOrd);
	}
	if(NodeStartPos==-1||NodeEndPos==-1) return false;

	memset(Buf,0,sizeof(Buf));

	//如果节尾不是\r\n结束，添加之
	char buf[256];
	if(NodeEndPos-NodeStartPos>=2)
	{
		fp=fopen(m_IniFileName,"rb+");
		fseek(fp,NodeEndPos-2,0);
		fread(buf,1,2,fp);
		fclose(fp);
	}
	if(NodeEndPos-NodeStartPos<2||buf[0]!='\r'||buf[1]!='\n')
		strcat(Buf,"\r\n");

	if(strlen(keystr)==0)
		sprintf(Buf+strlen(Buf),"%s=\"\"",key);
	else 
		sprintf(Buf+strlen(Buf),"%s=%s",key,keystr);
	BufLen=strlen(Buf);
	if(BufLen<40)
		memset(Buf+BufLen,' ',40-BufLen);
	strcat(Buf,"\r\n");
	BufLen=strlen(Buf);


	bRlt=false;
	fp=fopen(m_IniFileName,"rb");
	if(fp) 
	{
		fseek(fp,0,2);
		FileLen=ftell(fp);
		pBuf=(char*)malloc(FileLen);
		if(pBuf)
		{
			fseek(fp,0,0);
			fread(pBuf,1,FileLen,fp);
		}
		fclose(fp);

		if(pBuf)
		{
			fp=fopen(m_IniFileName,"wb");
			if(NodeEndPos>0&&NodeEndPos<=FileLen)
				fwrite(pBuf,1,NodeEndPos,fp);
			fwrite(Buf,1,BufLen,fp);
			if(NodeEndPos<FileLen)
				fwrite(pBuf+NodeEndPos,1,FileLen-NodeEndPos,fp);
			fclose(fp);

			free(pBuf);
			pBuf=NULL;
			bRlt=true;
		}
	}
	

	if(bRlt&&m_UseImgID)
		LoadIniFileImg_Internal();


	return bRlt;
}


//function		:LoadIniFileImg(void)
//description	:Load inifile content to img
void CIniFile::LoadIniFileImg_Internal(void)
{
	FILE			*fp;
	int				FileLen;

	if(!m_IniFileName[0]) return;
	if(!m_UseImgID) return;

	fp=fopen(m_IniFileName,"rb");
	if(fp)
	{
		fseek(fp,0,2);
		FileLen=ftell(fp);

		if(m_pIniFileImg)
		{
			free(m_pIniFileImg);
			m_pIniFileImg=NULL;
		}
		if(m_IniFileLen)
			m_IniFileLen=0;
			
		m_pIniFileImg=(char*)malloc(FileLen+1);
		if(m_pIniFileImg)
		{
			fseek(fp,0,0);
			fread(m_pIniFileImg,1,FileLen,fp);
			m_pIniFileImg[FileLen]=0;
			m_IniFileLen=FileLen;
		}
		fclose(fp);
	}
}


//function		:GetTitleOrdByKey
//description	:1. 取含有指定Key值的Title的序号，用于有多个同名Title的ini
//				 2. pKeystr可以为空
//return		:TitleOrd,-1为找不到
int CIniFile::GetTitleOrdByKey_Str_Internal(const char *title,const char *key,const char *pKeystr) 
{ 
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,keyLen,KeyStrLen,BufLen,RltPos,EndPos,RltLen,i;
	int				InSessionID,iOrd,iTitleOrd,RltTitleOrd;
	int				FileOff;
	bool			bRlt;


	if(!m_IniFileName||!m_IniFileName[0]||!title||!key||!title[0]||!key[0]) return -1;

	RltTitleOrd=-1;
	iTitleOrd=0;
	iOrd=0;


	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return -1;
	keyLen=(int)strlen(key);
	if(keyLen>=sizeof(Buf)) return -1;
	KeyStrLen=(int)strlen(pKeystr);
	if(KeyStrLen>=sizeof(Buf)) return -1;

	fp=NULL;
	InSessionID=0;
	FileOff=0;
	RltPos=-1;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;
																					//此时得到的Buf是一个过滤了首尾空格、\t、回车、换行的字符串
			BufLen=(int)strlen(Buf);


			if(Buf[0]=='[')
			{
				if(BufLen>=titleLen+2&&Buf[1+titleLen]==']'&&memicmp(Buf+1,title,titleLen)==0)
				{
					iTitleOrd++;
					InSessionID=1;
				}
				else
				{
					InSessionID=0;
				}
			}
			else if(InSessionID&&memicmp(Buf,key,keyLen)==0)					//好象符合要求的key
			{
				for(i=keyLen;i<BufLen&&(Buf[i]==' '||Buf[i]=='\t');i++);
				if(Buf[i]=='='&&Buf[i+1]!='=')
				{
					if(pKeystr==NULL)
					{
						RltTitleOrd=iTitleOrd-1;
						break;
					}
					else 
					{
						RltPos=i+1;													//Rlt的起点是RltPos
						FilterContent_Internal(Buf,BufLen,RltPos,EndPos);
						RltLen=EndPos-RltPos;
						if(RltLen==KeyStrLen&&memcmp(pKeystr,Buf+RltPos,RltLen)==0)
						{
							RltTitleOrd=iTitleOrd-1;
							break;
						}
					}
				}
			}
		}
		if(fp) fclose(fp);
	}


	return RltTitleOrd;
} 


//function		:GetIniKeyStr
//description	:1. get configure data in string value from ini file
//parameters	:title: title of configure data in ini file
//				 key: key of configure data in ini file
//				 defstr: default value of configure data
//				 RltBuf: prointer of buffer 
//				 RltSize: size of buffer
//				 SearchOrd:要找第几个符合条件的数据，同样title/key的数据可以出现多个
void CIniFile::GetIniKeyStr_Internal(const char *title,const char *key,const char *defstr,char *RltBuf,int RltSize,int SearchOrd,int TitleOrd) 
{ 
	
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,keyLen,BufLen,RltPos,EndPos,RltLen,i,defstrLen;
	int				InSessionID,iOrd,iTitleOrd;
	int				FileOff;
	bool			bRlt;

	if(!RltBuf||RltSize<=0) return;


	iTitleOrd=0;
	iOrd=0;
	RltBuf[0]=0;
	RltLen=0;
	if(defstr&&defstr[0])
	{
		defstrLen=(int)strlen(defstr);
		if(defstrLen>RltSize-1) defstrLen=RltSize-1;
		memcpy(RltBuf,defstr,defstrLen);
		RltBuf[defstrLen]=0;
		RltLen=defstrLen;
	}

	if(!m_IniFileName||!m_IniFileName[0]||!title||!key) return;

	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return;
	keyLen=(int)strlen(key);
	if(keyLen>=sizeof(Buf)) return;



	fp=NULL;
	InSessionID=0;
	FileOff=0;
	RltPos=-1;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;
																					//此时得到的Buf是一个过滤了首尾空格、\t、回车、换行的字符串
			BufLen=(int)strlen(Buf);

			if(!InSessionID&&BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)								//先找指定节的位置
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else InSessionID=1;												
			}
			else if(InSessionID&&Buf[0]=='[')
				break;
			else if(InSessionID&&memicmp(Buf,key,keyLen)==0)						//进入本节
			{
				for(i=keyLen;i<BufLen&&(Buf[i]==' '||Buf[i]=='\t');i++);
				if(Buf[i]=='='&&Buf[i+1]!='=')
				{																	//找到符合要求的key		
					if(iOrd<SearchOrd)
						iOrd++;
					else
					{
						RltPos=i+1;
						FilterContent_Internal(Buf,BufLen,RltPos,EndPos);
						RltLen=EndPos-RltPos;
						if(RltLen>0)
						{
							memcpy(RltBuf,Buf+RltPos,RltLen);							//将结果字符串放在RltBuf中
							RltBuf[RltLen]=0;
						}
						break;														//找到了
					}
				}
			}
		}
		if(fp) fclose(fp);
	}
} 


//	取一节的首尾位置
//	title:节名
//	pNodeStartPos，pNodeEndPos：接收节的首尾位置的指针
bool CIniFile::GetNodePos_Internal(const char *title,int *pNodeStartPos,int *pNodeEndPos,int TitleOrd)
{
	int				NodeStartPos,NodeEndPos;
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,BufLen;
	int				InSessionID;
	int				FileOff,LastFileOff,iTitleOrd;
	bool			bRlt;

	

	if(!m_IniFileName||!m_IniFileName[0]||!title||!title[0]) return false;


	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return false;

	fp=NULL;
	NodeStartPos=NodeEndPos=-1;
	InSessionID=0;
	FileOff=0;
	iTitleOrd=0;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			LastFileOff=FileOff;

			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;

			BufLen=(int)strlen(Buf);

			if(!InSessionID&&BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)								//先找指定节的位置
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else 
				{
					InSessionID=1;												
					NodeStartPos=LastFileOff;
				}
			}
			else if(InSessionID&&Buf[0]=='[')
			{
				NodeEndPos=LastFileOff;
				break;
			}
		}

		if(InSessionID&&NodeEndPos==-1)
			NodeEndPos=FileOff;

		if(fp) fclose(fp);
	}

	if(InSessionID)
	{
		if(pNodeStartPos) *pNodeStartPos=NodeStartPos;
		if(pNodeEndPos) *pNodeEndPos=NodeEndPos;
		return true;
	}
	else return false;
}


//	function	:GetKeyPos
//	description	:Get position of a key
//	取指定key行的首尾位置
//	title:节名
//	key:键名
//	pKeyStartPos，pKeyEndPos：接收键的首尾位置的指针
bool CIniFile::GetKeyPos_Internal(const char *title,const char *key,int *pKeyStartPos,int *pKeyEndPos,int SearchOrd,int TitleOrd)
{
	FILE			*fp; 
	char			Buf[10240];
	int				titleLen,keyLen,BufLen,i,iOrd;
	int				InSessionID,MatchKeyID;
	int				LastFileOff,FileOff;
	int				KeyStartPos,KeyEndPos,iTitleOrd;
	bool			bRlt;


	if(!title||!title[0]||!key||!key[0]||!m_IniFileName||!m_IniFileName[0]) return false;

	MatchKeyID=0;
	iTitleOrd=0;
	iOrd=0;
	titleLen=(int)strlen(title);
	if(titleLen>=sizeof(Buf)) return false;
	keyLen=(int)strlen(key);
	if(keyLen>=sizeof(Buf)) return false;


	fp=NULL;
	KeyStartPos=KeyEndPos=-1;
	InSessionID=0;
	FileOff=0;
	if(!m_UseImgID&&(fp=fopen(m_IniFileName,"rb"))!=NULL||m_UseImgID&&m_pIniFileImg)
	{
		while(1) 
		{ 
			LastFileOff=FileOff;

			bRlt=GetLineContent_Internal(Buf,sizeof(Buf),(char*)fp,FileOff);
			if(!bRlt) break;

			BufLen=(int)strlen(Buf);

			if(!InSessionID&&BufLen>=titleLen+2&&Buf[0]=='['&&Buf[1+titleLen]==']'&&
				   memicmp(Buf+1,title,titleLen)==0)								//先找指定节的位置
			{
				if(iTitleOrd<TitleOrd) iTitleOrd++;
				else InSessionID=1;											//进入本节
			}
			else if(InSessionID&&Buf[0]=='[')
				break;
			else if(InSessionID&&memicmp(Buf,key,keyLen)==0)			//有符合要求的key
			{
				if(iOrd<SearchOrd)
					iOrd++;
				else
				{
					for(i=keyLen;i<BufLen&&(Buf[i]==' '||Buf[i]=='\t');i++);
					if(Buf[i]=='='&&Buf[i+1]!='=')
					{															//找到符合要求的key		
						MatchKeyID=1;
						KeyStartPos=LastFileOff;
						KeyEndPos=FileOff;
						break;
					}
				}
			}
		}

		if(fp) fclose(fp);
	}


	if(MatchKeyID)
	{
		if(pKeyStartPos) *pKeyStartPos=KeyStartPos;
		if(pKeyEndPos) *pKeyEndPos=KeyEndPos;
		return true;
	}
	else return false;
}

//	删除指定名称的key
bool CIniFile::DeleteKey_Internal(const char *title,const char *key,int SearchOrd,int TitleOrd)
{
	FILE			*fp; 
	char			*pBuf;
	int				FileLen;
	int				DelStart,DelEnd;
	bool			bRlt;


	if(!m_IniFileName||!title||!title[0]||!key||!key[0]) return false;

	DelStart=DelEnd=-1;
	bRlt=GetKeyPos_Internal(title,key,&DelStart,&DelEnd,SearchOrd,TitleOrd);
	if(!bRlt) return false;														//没有指定key

	if((fp=fopen(m_IniFileName,"rb"))==NULL) 
		bRlt=false;																//没有指定文件
	else
	{
		fseek(fp,0,2);
		FileLen=ftell(fp);
		fseek(fp,0,0);
		pBuf=(char*)malloc(FileLen);
		if(!pBuf)
		{
			fclose(fp);
			bRlt=false;
		}
		else
		{
			fread(pBuf,1,FileLen,fp);
			fclose(fp);
			if((fp=fopen(m_IniFileName,"wb"))==NULL)
				bRlt=false;
			else
			{
				if(DelStart>0)
					fwrite(pBuf,1,DelStart,fp);
				if(DelEnd>=0&&DelEnd<FileLen)
					fwrite(pBuf+DelEnd,1,FileLen-DelEnd,fp);
				fclose(fp);
			}
			free(pBuf);
		}
		bRlt=true;
	}


	if(bRlt&&m_UseImgID)
		LoadIniFileImg_Internal();
	return bRlt;
}


//	function	:UpdateKeyStr_Internal
//	description	:update string of a key
//	title:节名
//	key:键名
//	keystr：键值
bool CIniFile::UpdateKeyStr_Internal(const char *title,const char *key,const char *keystr,int SearchOrd,int TitleOrd)
{
	int				KeyStartPos,KeyEndPos;

	if(!keystr) return false;

	KeyStartPos=KeyEndPos=-1;

	if(GetKeyPos_Internal(title,key,&KeyStartPos,&KeyEndPos,SearchOrd,TitleOrd)&&KeyStartPos!=-1&&KeyEndPos!=-1)
		DeleteKey_Internal(title,key,SearchOrd,TitleOrd);

	return AddKeyStr_Internal(title,key,keystr,TitleOrd);
}
