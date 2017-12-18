// Tools_Ansi.cpp: implementation of the CTools_Ansi class.
//
//	Description	:1. Defines tool functions 
//	Version		:1.0
//	Date		:2008-4-30
//	Author		:lzq
//				 Copyright (C), 2008-2028
//	
//	History		:
//
//////////////////////////////////////////////////////////////////////

#include "Tools_Ansi.h"
#include <winsock2.h>       
#include <sys/timeb.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>											//header file for iostream
using namespace std;										//using of std namespace


//#include "commondef.h"

//#pragma comment(lib, "ws2_32.lib")											


#pragma unmanaged


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTools_Ansi::CTools_Ansi()
{

}

CTools_Ansi::~CTools_Ansi()
{

}




//function		:ReplaceStr
//description	:1. replace substring in string
//				 2. all other function can call this function
//				 3. It's possible that length of Str differ after replacement is finished, so caller must process this case, avoidance memory overflow
//parameters	:Str : pointer of string what is replace
//				 SubStr : pointer of substring that is replaced with
//				 NewSubStr : pointer of substring that is replaced to
void CTools_Ansi::ReplaceStr(char *Str,char *SubStr,char *NewSubStr)
{
	int				SubLen,NewSubLen,Len1,Len2;
	char			*pp;

	SubLen=(int)strlen(SubStr);
	NewSubLen=(int)strlen(NewSubStr);
	while(1)
	{
		pp=strstr(Str,SubStr);
		if(!pp) break;
		Len1=(int)strlen(Str);
		Len2=(int)strlen(pp);

		if(Len2>SubLen)
			memmove(pp+NewSubLen,pp+SubLen,Len2-SubLen);
		memcpy(pp,NewSubStr,NewSubLen);
		pp[NewSubLen+Len2-SubLen]=0;
	}
}


//function		:ReplaceStrOnce
//description	:1. replace substring in string
//				 2. only one substring will be replace if there are several substring match it
void CTools_Ansi::ReplaceStrOnce(char *Str,char *SubStr,char *NewSubStr)
{
	int				SubLen,NewSubLen,Len1,Len2;
	char			*pp;

	SubLen=(int)strlen(SubStr);
	NewSubLen=(int)strlen(NewSubStr);
	pp=strstr(Str,SubStr);
	if(pp)
	{
		Len1=(int)strlen(Str);
		Len2=(int)strlen(pp);

		if(Len2>SubLen)
			memmove(pp+NewSubLen,pp+SubLen,Len2-SubLen);
		memcpy(pp,NewSubStr,NewSubLen);
		pp[NewSubLen+Len2-SubLen]=0;
	}
}


//	删除串右边的空格
void CTools_Ansi::DeleteStrRightSpace(char *Str)
{
	int i;
	for(i=strlen(Str);i>0;i--)
	{
		if(Str[i-1]==' '||Str[i-1]=='\t') Str[i-1]=0;
		else break;
	}
}

//	删除串左边的空格
void CTools_Ansi::DeleteStrLeftSpace(char *Str)
{
	int			i,iLeft,Len;
	Len=strlen(Str);
	for(iLeft=0;iLeft<Len;iLeft++) if(Str[iLeft]!=' '&&Str[iLeft]!='\t') break;
	if(iLeft>0) 
	{
		for(i=iLeft;i<Len;i++) Str[i-iLeft]=Str[i];
		memset(Str+Len-iLeft,0,iLeft);
	}
}

//	删除串两边的空格
void CTools_Ansi::DeleteStrLeftRightSpace(char *Str)
{
	int			i,iLeft,iRight,Len;
	Len=strlen(Str);
	for(iLeft=0;iLeft<Len;iLeft++) if(Str[iLeft]!=' '&&Str[iLeft]!='\t') break;
	for(iRight=Len;iRight>iLeft;iRight--) if(Str[iRight-1]!=' '&&Str[iRight-1]!='\t') break;
	if(iLeft>0) for(i=iLeft;i<iRight;i++) Str[i-iLeft]=Str[i];
	if(Len>iRight-iLeft)
		memset(Str+iRight-iLeft,0,Len-(iRight-iLeft));
}

//	删除串两边的空格和回车换行符
void CTools_Ansi::DeleteStrLeftRightSpace_10_13(char *Str)
{
	int			i,iLeft,iRight,Len;
	Len=strlen(Str);
	for(iLeft=0;iLeft<Len;iLeft++) if(Str[iLeft]!=' '&&Str[iLeft]!='\t'&&Str[iLeft]!=10&&Str[iLeft]!=13) break;
	for(iRight=Len;iRight>iLeft;iRight--) if(Str[iRight-1]!=' '&&Str[iRight-1]!='\t'&&Str[iRight-1]!=10&&Str[iRight-1]!=13) break;
	if(iLeft>0) for(i=iLeft;i<iRight;i++) Str[i-iLeft]=Str[i];
	if(Len>iRight-iLeft)
		memset(Str+iRight-iLeft,0,Len-(iRight-iLeft));
}

//function		:GetNextDigitOff
//description	:look for offset of the next digit bytes，Len can be empty
int CTools_Ansi::GetNextDigitOff(char *pStr,int Len,int StartOff)
{
	int				i;
	if(!Len) Len=strlen(pStr);
	i=StartOff;
	while(i<Len&&pStr[i]>='0'&&pStr[i]<='9') i++;
	while(i<Len&&(pStr[i]<'0'||pStr[i]>'9')) i++;
	return i;
}

//function		:GetNextCharOff
//description	:look for offset of the next character bytes, Len can be empty
int CTools_Ansi::GetNextCharOff(char Ch,char *pStr,int Len,int StartOff)
{
	int				i;
	if(!Len) Len=strlen(pStr);
	for(i=StartOff;i<Len&&pStr[i]!=Ch;i++);
	return i;
}

//function		:GetNextCharOff2
//description	:look for offset of the next two character bytes, Len can be empty
int CTools_Ansi::GetNextCharOff2(char Ch1,char Ch2,char *pStr,int Len,int StartOff)
{
	int				i;
	if(!Len) Len=strlen(pStr);
	for(i=StartOff;i<Len&&pStr[i]!=Ch1&&pStr[i]!=Ch2;i++);
	return i;
}

//function		:GetNextValidOff
//description	:look for offset of the next char that is't space、tab、\r、\n, Len can be empty
int CTools_Ansi::GetNextValidOff(char *pStr,int Len,int StartOff)
{
	int				i;
	char			ich;
	if(!Len) Len=strlen(pStr);
	for(i=StartOff;i<Len;i++)
	{
		ich=pStr[i];
		if(ich!=' '&&ich!='\t'&&ich!=10&&ich!=13) return i;
	}
	return Len;
}


//function		:GetNextInvalidOff
//description	:look for offset of the next char that is space、tab、\r、\n, Len can be empty
int CTools_Ansi::GetNextInvalidOff(char *pStr,int Len,int StartOff)
{
	int				i;
	char			ich;
	if(!Len) Len=strlen(pStr);
	for(i=StartOff;i<Len;i++)
	{
		ich=pStr[i];
		if(ich==' '||ich=='\t'||ich==10||ich==13) return i;
	}
	return Len;
}


//function		:GetPrevInvalidOff
//description	:向前搜索，找第一个上一个字符是Valid的位置
int CTools_Ansi::GetPrevInvalidOff(char *pStr,int StartOff,int MinOff)
{
	int				i;
	char			ich;
	for(i=StartOff;i>0&&i>MinOff;i--)
	{
		ich=pStr[i-1];
		if(ich!=' '&&ich!='\t'&&ich!=10&&ich!=13) return i;
	}
	return i;
}


//function		:GetNextCharOrInvalidOff
//description	:look for offset of the next char that is special character, or space、tab、\r、\n, Len can be empty
int CTools_Ansi::GetNextCharOrInvalidOff(char Ch,char *pStr,int Len,int StartOff)
{
	int				i;
	char			ich;
	if(!Len) Len=strlen(pStr);
	for(i=StartOff;i<Len;i++)
	{
		ich=pStr[i];
		if(ich==Ch||ich==' '||ich=='\t'||ich==10||ich==13) return i;
	}
	return Len;
}


//function		:GetNextFieldOff
//description	:look for offset of the next field, seperated by Seperator，Len can be empty
int CTools_Ansi::GetNextFieldOff(char *pStr,int Len,int StartOff,char Seperator1,char Seperator2,char Seperator3,char Seperator4)
{
	int				i;
	if(!Len) Len=strlen(pStr);
	i=StartOff;
	while(i<Len&&pStr[i]!=Seperator1&&pStr[i]!=Seperator2&&pStr[i]!=Seperator3&&pStr[i]!=Seperator4) i++;
	while(i<Len&&(pStr[i]==Seperator1||pStr[i]==Seperator2||pStr[i]==Seperator3||pStr[i]==Seperator4)) i++;
	return i;
}

BOOL CTools_Ansi::GetCurField(char *pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2,char Seperator3,char Seperator4)
{
	int				i;
	*RltFieldStart=0;
	*RltFieldEnd=0;
	if(!Len) Len=strlen(pStr);
	i=StartOff;
	while(i<Len&&(pStr[i]==' '||pStr[i]=='\t'||pStr[i]==Seperator1||pStr[i]==Seperator2||pStr[i]==Seperator3||pStr[i]==Seperator4)) i++;
	*RltFieldStart=i;
	while(i<Len&&pStr[i]!=Seperator1&&pStr[i]!=Seperator2&&pStr[i]!=Seperator3&&pStr[i]!=Seperator4) i++;
	while(i>0&&(pStr[i-1]==' '||pStr[i-1]=='\t')) i--;
	if(i<(*RltFieldStart)) i=(*RltFieldStart);
	*RltFieldEnd=i;
	if((*RltFieldEnd)>(*RltFieldStart)) return TRUE;
	else return FALSE;
}

int CTools_Ansi::GetFourField(char *pStr,int Len,int StartOff,int *RltFieldStart1,int *RltFieldEnd1,int *RltFieldStart2,int *RltFieldEnd2,int *RltFieldStart3,int *RltFieldEnd3,int *RltFieldStart4,int *RltFieldEnd4,char Seperator1,char Seperator2,char Seperator3,char Seperator4)
{
	int				Count,iStart,iEnd;
	BOOL			bRlt;

	Count=0;
	if(!Len) Len=strlen(pStr);
	if(RltFieldStart1&&RltFieldEnd1) (*RltFieldStart1)=(*RltFieldEnd1)=0;
	if(RltFieldStart2&&RltFieldEnd2) (*RltFieldStart2)=(*RltFieldEnd2)=0;
	if(RltFieldStart3&&RltFieldEnd3) (*RltFieldStart3)=(*RltFieldEnd3)=0;
	if(RltFieldStart4&&RltFieldEnd4) (*RltFieldStart4)=(*RltFieldEnd4)=0;

	if(!RltFieldStart1||!RltFieldEnd1) return Count;
	bRlt=GetCurField(pStr,Len,StartOff,&iStart,&iEnd,Seperator1,Seperator2,Seperator3,Seperator4);
	if(!bRlt) return Count;
	*RltFieldStart1=iStart;
	*RltFieldEnd1=iEnd;
	Count++;

	if(!RltFieldStart2||!RltFieldEnd2) return Count;
	bRlt=GetCurField(pStr,Len,iEnd,&iStart,&iEnd,Seperator1,Seperator2,Seperator3,Seperator4);
	if(!bRlt) return Count;
	*RltFieldStart2=iStart;
	*RltFieldEnd2=iEnd;
	Count++;

	if(!RltFieldStart3||!RltFieldEnd3) return Count;
	bRlt=GetCurField(pStr,Len,iEnd,&iStart,&iEnd,Seperator1,Seperator2,Seperator3,Seperator4);
	if(!bRlt) return Count;
	*RltFieldStart3=iStart;
	*RltFieldEnd3=iEnd;
	Count++;

	if(!RltFieldStart4||!RltFieldEnd4) return Count;
	bRlt=GetCurField(pStr,Len,iEnd,&iStart,&iEnd,Seperator1,Seperator2,Seperator3,Seperator4);
	if(!bRlt) return Count;
	*RltFieldStart4=iStart;
	*RltFieldEnd4=iEnd;
	Count++;

	return Count;
}

//function		:GetSubStrInYuankuohao
//description	:get sub string in Yuankuohao
BOOL CTools_Ansi::GetSubStrInYuankuohao(const char *pSour,char *pRlt,int RltSize)
{
	int				i,j,Len;
	pRlt[0]=0;
	Len=strlen(pSour);
	for(i=0;i<Len&&pSour[i]!='(';i++);
	if(pSour[i]=='(') i++;
	for(j=Len;j>i&&pSour[j-1]!=')';j--);
	if(pSour[j-1]==')') j--;
	if(j-i>=RltSize) return FALSE;
	memcpy(pRlt,pSour+i,j-i);
	pRlt[j-i]=0;
	return TRUE;
}


//执行WSAStartup
void CTools_Ansi::PerformWSAStartup(void)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,0),&wsaData);
}

//执行WSACleanup
void CTools_Ansi::PerformWSACleanup(void)
{
    WSACleanup();
}


//function		:GetLocalHostName_IP
//description	:1. get local host name and local ip
BOOL CTools_Ansi::GetLocalHostName_IP(char *pHostName,int cbHostName,DWORD *pLocalIP,int MaxIPCount)
{

	HOSTENT			*pHost;
	DWORD			**ppAddress;
	int				i;
	DWORD			LocalIP;
	char			LocalHostName[HOSTNAMELEN];

																			//取本机地址
	if(pHostName) 
		memset(pHostName,0,cbHostName);
	if(pLocalIP!=NULL&&MaxIPCount>0)
		memset((char*)pLocalIP,0,sizeof(DWORD)*MaxIPCount);

	memset((char*)LocalHostName,0,sizeof(LocalHostName));
	LocalIP=0;

 	gethostname(LocalHostName,sizeof(LocalHostName)-1);
	if(!LocalHostName[0]) return FALSE;

	if(pHostName)
		strncpy(pHostName,LocalHostName,cbHostName-1);

	pHost=gethostbyname(LocalHostName);
	if(!pHost) return FALSE;
	if(pHost->h_length!=4||pHost->h_addrtype!=2) return FALSE;

	if(pLocalIP!=NULL&&MaxIPCount>0)
	{
		ppAddress=(DWORD**)(pHost->h_addr_list);
		for(i=0;i<MaxIPCount;i++)
		{
			if(!ppAddress[i]) 
				break;
			pLocalIP[i]=*(ppAddress[i]);
		}
	}
	return TRUE;
}


//function		:GetIPFromHostName
//description	:1. get local host name and local ip
//return		:IP number in result
int CTools_Ansi::GetIPFromHostName(char *pHostName,DWORD *pHostIP,int MaxIPCount)
{
	HOSTENT			*pHost;
	DWORD			**ppAddress;
	int				i,Len,RltNum,DotNum;

	if(MaxIPCount<=0) return 0;
	if(!pHostName) return 0;
	if(!pHostIP) return 0; 

	for(i=0,Len=strlen(pHostName),DotNum=0;i<Len;i++)
	{
		if(pHostName[i]!=' '&&pHostName[i]!='\t'&&pHostName[i]!='.'&&(pHostName[i]<'0'||pHostName[i]>'9'))
			break;
		if(pHostName[i]=='.') DotNum++;
	}
	if(i>=Len&&DotNum==3)									//pHostName是数字表示的IP地址，直接转换就行
	{
		pHostIP[0]=ConvertStrIP(pHostName);
		return 1;
	}

	pHost=gethostbyname(pHostName);
	if(!pHost) return 0;
	if(pHost->h_length!=4||pHost->h_addrtype!=2) return 0;

	memset(pHostIP,0,sizeof(DWORD)*MaxIPCount);

	RltNum=0;
	ppAddress=(DWORD**)(pHost->h_addr_list);
	for(i=0;i<MaxIPCount;i++)
	{
		if(!ppAddress[i]) 
			break;
		pHostIP[i]=*(ppAddress[i]);
		RltNum++;
	}
	return RltNum;
}



//	将字符串形式的IP地址转换成DWORD
DWORD CTools_Ansi::ConvertStrIP(const char *pIP)
{
	DWORD			dwIP;
	dwIP=inet_addr(pIP);
	return dwIP;
}

char* CTools_Ansi::ConvertdwIP(DWORD dwIP)
{
	struct in_addr in;
	in.S_un.S_addr=dwIP;
	return inet_ntoa(in);
}

//	判断是不是同一个主机
BOOL CTools_Ansi::IsSameHost(DWORD* iHostIP,int IPCount,DWORD TargetIP)
{
	int				i;
	for(i=0;i<IPCount;i++)
		if(iHostIP[i]!=0&&iHostIP[i]!=0xffffffff&&iHostIP[i]==TargetIP) return TRUE;
	return FALSE;
}



//	根据分隔符，从字符串中取出第N个字段
BOOL CTools_Ansi::GetFieldFromStr(const char *pData,int FieldOrd,const char *pSeperator,char *RltBuf,int RltSize)
{
	int				iOrd,Off,Len,SeperatorLen,j;
	char			*pp;

	memset(RltBuf,0,RltSize);

	iOrd=0;
	Off=0;
	Len=strlen(pData);
	SeperatorLen=strlen(pSeperator);
	while(Off<Len)
	{
		if(iOrd==FieldOrd) break;
		pp=(char*)strstr(pData+Off,pSeperator);
		if(!pp) break;
		j=pp-(pData+Off);
		Off+=j+SeperatorLen;
		iOrd++;
	}

	if(iOrd==FieldOrd)
	{
		pp=(char*)strstr(pData+Off,pSeperator);
		if(!pp) pp=(char*)pData+Len;
		j=pp-(pData+Off);
		if(j>RltSize) j=RltSize;
		memcpy(RltBuf,pData+Off,j);
		return TRUE;
	}


	return FALSE;
}



//	判断字符串里的时间是否包含当前时间
//	1. strDateTime可以包含多个具体时间，不同时间部分用分号或逗号分隔
//	2. 每个时间包含date部分和time部分
//	3. 如果每个时间里没有date，表示date无条件合法；如果没有time，表示time无条件合法
//	3. date部分可以包含多个时间，空格分隔，取值范围：
//		工作日、周末、周1、周2、周3、周4、周5、周6、周7、周0、周1-周3
//		1号、2号、......31号、1号-10号
//		1月1日、12月31日、1月1日-12月31日
//	4. time部分可包含多个时间，空格分隔，格式：
//		hh:mm:ss
//		hh:mm:ss-hh:mm:ss
//		hh:mm
//		hh:mm-hh:mm
//	5. 时间内不能有空格
BOOL CTools_Ansi::IsNowValidDateTimeStr(const char *strDateTime)
{
	time_t			itime;
	struct tm		*ltime;
	BOOL			bValidDate,bValidTime,bRlt;
	int				Off,i,Len;
	char			Buf[512];


	if(strstr(strDateTime,",")!=0||strstr(strDateTime,";")!=0)
	{																			//********串里包含多个时间的情况********
		Len=strlen(strDateTime);
		Off=0;
		while(Off<Len)
		{
			for(;Off<Len&&(strDateTime[Off]==','||strDateTime[Off]==';'||strDateTime[Off]==' '||strDateTime[Off]=='\t');Off++);
			for(i=Off;i<Len&&i-Off<sizeof(Buf)-1&&strDateTime[i]!=','&&strDateTime[i]!=';';i++);
			if(i>Off)
			{
				memcpy(Buf,strDateTime+Off,i-Off);
				Buf[i-Off]=0;
				bRlt=IsNowValidDateTimeStr(Buf);
				if(bRlt) return TRUE;
				Off=i+1;
			}
		}
		return FALSE;
	}

																				//********下面处理串里只包含一个时间的情况********
	time(&itime);
	ltime=localtime(&itime);
	bValidDate=FALSE;
	bValidTime=FALSE;

	if(	strstr(strDateTime,"工作日")==0&&strstr(strDateTime,"周")==0&&strstr(strDateTime,"月")==0&&
		strstr(strDateTime,"日")==0&&strstr(strDateTime,"号")==0&&
		strstr(strDateTime,":")==0) 
		return FALSE;															//既没有定义日期也没有定义时间，返回失败
	else if(strstr(strDateTime,"工作日")==0&&strstr(strDateTime,"周")==0&&strstr(strDateTime,"月")==0&&
			strstr(strDateTime,"日")==0&&strstr(strDateTime,"号")==0)
		bValidDate=TRUE;														//没有定义日期，认为日期有效
	else if(strstr(strDateTime,":")==0)
		bValidTime=TRUE;														//没有定义时间，认为时间有效

	Len=strlen(strDateTime);
	Off=0;
	while(Off<Len)
	{																			//一个个字段进行判断
		for(;Off<Len&&(strDateTime[Off]==' '||strDateTime[Off]=='\t');Off++);
		for(i=Off;i<Len&&i-Off<sizeof(Buf)-1&&strDateTime[i]!=' '&&strDateTime[i]!='\t';i++);
		memcpy(Buf,strDateTime+Off,i-Off);
		Buf[i-Off]=0;
		if(!bValidDate&&
			(strstr(Buf,"工作日")!=0||strstr(Buf,"周")!=0||strstr(Buf,"月")!=0||
			 strstr(strDateTime,"日")!=0||strstr(strDateTime,"号")!=0))
			bValidDate=IsNowValidDateStr(Buf);
		else if(!bValidTime&&strstr(Buf,":")!=0)
			bValidTime=IsNowValidTimeStr(Buf);

		if(bValidDate&&bValidTime) return TRUE;
		Off=i;
	}

	return FALSE;

}


//	判断字符串里的时间是否包含当前时间
//	1. strDate取值范围：
//		工作日、周末、周1、周2、周3、周4、周5、周6、周7、周0、周1-周3，周一、周二......周日
//		1号、2号、......31号、1号-10号
//		1月1日、12月31日、1月1日-12月31日、一月1日......十二月1日
BOOL CTools_Ansi::IsNowValidDateStr(const char *strDate)
{
	time_t			itime;
	struct tm		*ltime;
	BOOL			bValidData;
	int				i,j,m1,d1,m2,d2;
	char			*pEndDate,*pDate1,*pDate2;;
	char			*pMonth,*pMon1,*pMon2;


	time(&itime);
	ltime=localtime(&itime);
	bValidData=FALSE;

	if(strstr(strDate,"工作日")!=0&&ltime->tm_wday>=1&&ltime->tm_wday<=5) return TRUE;
	if(strstr(strDate,"周末")!=0&&(ltime->tm_wday==0||ltime->tm_wday==6)) return TRUE;

	pEndDate=(char*)strstr(strDate,"-");
	if(!pEndDate)
	{																			//没有定义起始时间
		if(memcmp(strDate,"周",2)==0)
		{
			if(memcmp(strDate,"周一",4)==0) i=1;
			else if(memcmp(strDate,"周二",4)==0) i=2;
			else if(memcmp(strDate,"周三",4)==0) i=3;
			else if(memcmp(strDate,"周四",4)==0) i=4;
			else if(memcmp(strDate,"周五",4)==0) i=5;
			else if(memcmp(strDate,"周六",4)==0) i=6;
			else if(memcmp(strDate,"周日",4)==0) i=0;
			else i=atoi(strDate+2);

			if(i==7) i=0;
			if(ltime->tm_wday==i) return TRUE;									//符合星期几的定义
		}
		else if(strstr(strDate,"号")!=0)
		{
			i=atoi(strDate);
			if(ltime->tm_mday==i) return TRUE;									//符合几号的定义
		}
		else if((pMonth=(char*)strstr(strDate,"月"))!=0)
		{
			if(memcmp(strDate,"一月",4)==0) i=1;
			else if(memcmp(strDate,"二月",4)==0) i=2;
			else if(memcmp(strDate,"三月",4)==0) i=3;
			else if(memcmp(strDate,"四月",4)==0) i=4;
			else if(memcmp(strDate,"五月",4)==0) i=5;
			else if(memcmp(strDate,"六月",4)==0) i=6;
			else if(memcmp(strDate,"七月",4)==0) i=7;
			else if(memcmp(strDate,"八月",4)==0) i=8;
			else if(memcmp(strDate,"九月",4)==0) i=9;
			else if(memcmp(strDate,"十月",4)==0) i=10;
			else if(memcmp(strDate,"十一月",4)==0) i=11;
			else if(memcmp(strDate,"十二月",4)==0) i=12;
			else i=atoi(strDate);
			j=atoi(pMonth+2);
			if(ltime->tm_mon+1==i&&ltime->tm_mday==j) return TRUE;				//符合几月几日的定义
		}
	}
	else
	{																			//定义起始时间
		pEndDate++;
		if(memcmp(strDate,"周",2)==0&&memcmp(pEndDate,"周",2)==0)
		{
			if(memcmp(strDate,"周一",4)==0) i=1;
			else if(memcmp(strDate,"周二",4)==0) i=2;
			else if(memcmp(strDate,"周三",4)==0) i=3;
			else if(memcmp(strDate,"周四",4)==0) i=4;
			else if(memcmp(strDate,"周五",4)==0) i=5;
			else if(memcmp(strDate,"周六",4)==0) i=6;
			else if(memcmp(strDate,"周日",4)==0) i=0;
			else i=atoi(strDate+2);
			if(i==7) i=0;

			if(memcmp(pEndDate,"周一",4)==0) j=1;
			else if(memcmp(pEndDate,"周二",4)==0) j=2;
			else if(memcmp(pEndDate,"周三",4)==0) j=3;
			else if(memcmp(pEndDate,"周四",4)==0) j=4;
			else if(memcmp(pEndDate,"周五",4)==0) j=5;
			else if(memcmp(pEndDate,"周六",4)==0) j=6;
			else if(memcmp(pEndDate,"周日",4)==0) j=0;
			else j=atoi(pEndDate+2);
			if(j==7) j=0;

			if(	i<=j&&ltime->tm_wday>=i&&ltime->tm_wday<=j||
				i>j&&(ltime->tm_wday>=i&&ltime->tm_wday<=6||ltime->tm_wday<=j))
				return TRUE;													//符合周x-周y的定义
		}
		else if((pDate1=(char*)strstr(strDate,"号"))!=0&&pDate1<pEndDate&&(pDate2=(char*)strstr(pEndDate,"号"))!=0)
		{
			i=atoi(strDate);
			j=atoi(pEndDate);
			if(	i<=j&&ltime->tm_mday>=i&&ltime->tm_mday<=j||
				i>j&&(ltime->tm_mday>=i&&ltime->tm_mday<=31||ltime->tm_mday<=j))
				return TRUE;													//符合x号-y号的定义
		}
		else if((pMon1=(char*)strstr(strDate,"月"))!=0&&(pDate1=(char*)strstr(strDate,"日"))!=0&&pDate1>pMon1&&pDate1<pEndDate&&
				(pMon2=(char*)strstr(pEndDate,"月"))!=0&&(pDate2=(char*)strstr(pEndDate,"日"))!=0&&pDate2>pMon2)
		{
			if(memcmp(strDate,"一月",4)==0) m1=1;
			else if(memcmp(strDate,"二月",4)==0) m1=2;
			else if(memcmp(strDate,"三月",4)==0) m1=3;
			else if(memcmp(strDate,"四月",4)==0) m1=4;
			else if(memcmp(strDate,"五月",4)==0) m1=5;
			else if(memcmp(strDate,"六月",4)==0) m1=6;
			else if(memcmp(strDate,"七月",4)==0) m1=7;
			else if(memcmp(strDate,"八月",4)==0) m1=8;
			else if(memcmp(strDate,"九月",4)==0) m1=9;
			else if(memcmp(strDate,"十月",4)==0) m1=10;
			else if(memcmp(strDate,"十一月",4)==0) m1=11;
			else if(memcmp(strDate,"十二月",4)==0) m1=12;
			else m1=atoi(strDate);

			d1=atoi(pMon1+2);

			if(memcmp(pEndDate,"一月",4)==0) m2=1;
			else if(memcmp(pEndDate,"二月",4)==0) m2=2;
			else if(memcmp(pEndDate,"三月",4)==0) m2=3;
			else if(memcmp(pEndDate,"四月",4)==0) m2=4;
			else if(memcmp(pEndDate,"五月",4)==0) m2=5;
			else if(memcmp(pEndDate,"六月",4)==0) m2=6;
			else if(memcmp(pEndDate,"七月",4)==0) m2=7;
			else if(memcmp(pEndDate,"八月",4)==0) m2=8;
			else if(memcmp(pEndDate,"九月",4)==0) m2=9;
			else if(memcmp(pEndDate,"十月",4)==0) m2=10;
			else if(memcmp(pEndDate,"十一月",4)==0) m2=11;
			else if(memcmp(pEndDate,"十二月",4)==0) m2=12;
			else m2=atoi(pEndDate);

			d2=atoi(pMon2+2);

			if(	(ltime->tm_mon+1>m1||ltime->tm_mon+1==m1&&ltime->tm_mday>=d1)&&
				(ltime->tm_mon+1<m2||ltime->tm_mon+1==m2&&ltime->tm_mday<=d2))
				return TRUE;													//符合x月y日-a月b日的定义
		}

	}

	return FALSE;
}


//	判断字符串里的时间是否包含当前时间
//	1. strTime格式：
//		hh:mm
//		hh:mm:ss
//		hh:mm-hh:mm
//		hh:mm:ss-hh:mm:ss
//	2. 特殊用法：hh:02表示每小时的02分
BOOL CTools_Ansi::IsNowValidTimeStr(const char *strTime)
{
	time_t			itime;
	struct tm		*ltime;
	BOOL			bValidData;
	int				Len;
	char			*pEndTime,*pp;
	int				h1,m1,s1,h2,m2,s2;
	int				EveryHour1ID,EveryHour2ID;


	time(&itime);
	ltime=localtime(&itime);
	bValidData=FALSE;

	Len=strlen(strTime);
	pEndTime=(char*)strstr(strTime,"-");
	h1=m1=s1=h2=m2=s2=-1;
	EveryHour1ID=EveryHour2ID=0;
	if(!pEndTime)
	{																			//没有定义起始时间
		pp=(char*)strTime;
		while(pp[0]==' '||pp[0]=='\t') pp++;

		if(pp[0]=='h'&&pp[1]=='h')
		{
			EveryHour1ID=1;
			pp+=2;
		}
		else
		{
			h1=atoi(pp);
			while(pp[0]>='0'&&pp[0]<='9') pp++;
		}

		if(pp[0]==':')
		{
			pp++;
			while(pp[0]==' '||pp[0]=='\t') pp++;
			m1=atoi(pp);

			while(pp[0]>='0'&&pp[0]<='9') pp++;
			if(pp[0]==':')
			{
				pp++;
				while(pp[0]==' '||pp[0]=='\t') pp++;
				s1=atoi(pp);
			}
		}

		
		if(	h1>=0&&m1>=0&&s1<0&&ltime->tm_hour==h1&&ltime->tm_min==m1||
			EveryHour1ID&&m1>=0&&s1<0&&ltime->tm_min==m1||
			h1>=0&&m1>=0&&s1>=0&&ltime->tm_hour==h1&&ltime->tm_min==m1&&ltime->tm_sec==s1||
			EveryHour1ID&&m1>=0&&s1>=0&&ltime->tm_min==m1&&ltime->tm_sec==s1)
			return TRUE;														//符合hh:mm或hh:mm:ss的定义
	}
	else
	{																			//定义起始时间
		pEndTime++;

		pp=(char*)strTime;
		while(pp[0]==' '||pp[0]=='\t') pp++;

		if(pp[0]=='h'&&pp[1]=='h')
		{
			EveryHour1ID=1;
			pp+=2;
		}
		else
		{
			h1=atoi(pp);
			while(pp[0]>='0'&&pp[0]<='9') pp++;
		}

		if(pp[0]==':')
		{
			pp++;
			while(pp[0]==' '||pp[0]=='\t') pp++;
			m1=atoi(pp);

			while(pp[0]>='0'&&pp[0]<='9') pp++;
			if(pp[0]==':')
			{
				pp++;
				while(pp[0]==' '||pp[0]=='\t') pp++;
				s1=atoi(pp);
			}
		}



		pp=pEndTime;
		while(pp[0]==' '||pp[0]=='\t') pp++;

		if(pp[0]=='h'&&pp[1]=='h') 
		{
			EveryHour2ID=1;
			pp+=2;
		}
		else
		{
			h2=atoi(pp);
			while(pp[0]>='0'&&pp[0]<='9') pp++;
		}
		if(pp[0]==':')
		{
			pp++;
			while(pp[0]==' '||pp[0]=='\t') pp++;
			m2=atoi(pp);

			while(pp[0]>='0'&&pp[0]<='9') pp++;
			if(pp[0]==':')
			{
				pp++;
				while(pp[0]==' '||pp[0]=='\t') pp++;
				s2=atoi(pp);
			}
		}


		if(h1>=0&&m1>=0&&h2>=0&&m2>=0)
		{
			if(s1<0) s1=0;
			if(s2<0) s2=59;

			if(h2>h1||h2==h1&&m2>m1||h2==h1&&m2==m1&&s2>=s1)
			{
				if(	(ltime->tm_hour>h1||ltime->tm_hour==h1&&ltime->tm_min>m1||ltime->tm_hour==h1&&ltime->tm_min==m1&&ltime->tm_sec>=s1)&&
					(ltime->tm_hour<h2||ltime->tm_hour==h2&&ltime->tm_min<m2||ltime->tm_hour==h2&&ltime->tm_min==m2&&ltime->tm_sec<=s2))
					return TRUE;
			}
			else
			{
				if(	(ltime->tm_hour>h1||ltime->tm_hour==h1&&ltime->tm_min>m1||ltime->tm_hour==h1&&ltime->tm_min==m1&&ltime->tm_sec>=s1)||
					(ltime->tm_hour<h2||ltime->tm_hour==h2&&ltime->tm_min<m2||ltime->tm_hour==h2&&ltime->tm_min==m2&&ltime->tm_sec<=s2))
					return TRUE;
			}
		}
		else if(EveryHour1ID&&m1>=0&&EveryHour2ID&&m2>=0)
		{
			if(s1<0) s1=0;
			if(s2<0) s2=59;

			if(m2>m1||m2==m1&&s2>=s1)
			{
				if(	(ltime->tm_min>m1||ltime->tm_min==m1&&ltime->tm_sec>=s1)&&
					(ltime->tm_min<m2||ltime->tm_min==m2&&ltime->tm_sec<=s2))
					return TRUE;
			}
			else
			{
				if(	(ltime->tm_min>m1||ltime->tm_min==m1&&ltime->tm_sec>=s1)||
					(ltime->tm_min<m2||ltime->tm_min==m2&&ltime->tm_sec<=s2))
					return TRUE;
			}
		}
	}

	return FALSE;
}


//	把NewFile指向的wave文件合并到TargetFile指向的wave文件中
//	NewFile和TargetFile文件的格式必须一致
BOOL CTools_Ansi::AppendWaveFileToAnother(const char *TargetFile,const char *NewFile)
{
	FILE			*fTarget,*fNew;
	BOOL			bRlt;
	struct RIFF_HEADER
	{
		char		szRiffID[4];												// 'R','I','F','F'
		DWORD		dwRiffSize;
		char		szRiffFormat[4];											// 'W','A','V','E'
	}RiffHead_Target,RiffHead_New;
	struct FMT_BLOCK
	{
		char		szFmtID[4];													// 'f','m','t',' '
		DWORD		dwFmtSize;
		struct WAVE_FORMAT
		{
			WORD	wFormatTag;
			WORD	wChannels;
			DWORD	dwSamplesPerSec;
			DWORD	dwAvgBytesPerSec;
			WORD	wBlockAlign;
			WORD	wBitsPerSample;
		}WavFormat;
	}FormatHead_Target,FormatHead_New;
	char			ExtFormatInfo_Target[2];
	char			ExtFormatInfo_New[2];
	struct FACT_BLOCK
	{
		char		szFactID[4];												// 'f','a','c','t'
		DWORD		dwFactSize;
	}FactHead_Target,FactHead_New;
	struct DATA_BLOCK
	{
		char		szDataID[4];
		DWORD		dwDataSize;
	}DataHead_Target,DataHead_New;
	DWORD			RIFFHeadOff_Target,DataHeadOff_Target;
	char			Tmp[32768];
	int				i,j,TmpSize;



	fTarget=fopen(TargetFile,"rb+");
	if(!fTarget) return FALSE;

	fNew=fopen(NewFile,"rb");
	if(!fNew)
	{
		fclose(fTarget);
		return FALSE;
	}


	memset((char*)&RiffHead_Target,0,sizeof(RiffHead_Target));
	memset((char*)&RiffHead_New,0,sizeof(RiffHead_New));
	memset((char*)&FormatHead_Target,0,sizeof(FormatHead_Target));
	memset((char*)&FormatHead_New,0,sizeof(FormatHead_New));
	memset((char*)ExtFormatInfo_Target,0,sizeof(ExtFormatInfo_Target));
	memset((char*)ExtFormatInfo_New,0,sizeof(ExtFormatInfo_New));
	memset((char*)&FactHead_Target,0,sizeof(FactHead_Target));
	memset((char*)&FactHead_New,0,sizeof(FactHead_New));
	memset((char*)&DataHead_Target,0,sizeof(DataHead_Target));
	memset((char*)&DataHead_New,0,sizeof(DataHead_New));

																				//读取TargetFile的文件头
	RIFFHeadOff_Target=0;
	fread((char*)&RiffHead_Target,1,sizeof(RiffHead_Target),fTarget);
	fread((char*)&FormatHead_Target,1,sizeof(FormatHead_Target),fTarget);
	if(FormatHead_Target.dwFmtSize==18)
		fread((char*)ExtFormatInfo_Target,1,sizeof(ExtFormatInfo_Target),fTarget);
	fread((char*)&FactHead_Target,1,sizeof(FactHead_Target),fTarget);
	if(memcmp(FactHead_Target.szFactID,"fact",4)==0)
	{
		fseek(fTarget,FactHead_Target.dwFactSize,1);
		fread((char*)&DataHead_Target,1,sizeof(DataHead_Target),fTarget);
	}
	else
	{
		memcpy((char*)&DataHead_Target,(char*)&FactHead_Target,sizeof(DataHead_Target));
		memset((char*)&FactHead_Target,0,sizeof(FactHead_Target));
	}
	DataHeadOff_Target=ftell(fTarget)-sizeof(DataHead_Target);

																				//读取NewFile的文件头
	fread((char*)&RiffHead_New,1,sizeof(RiffHead_New),fNew);
	fread((char*)&FormatHead_New,1,sizeof(FormatHead_New),fNew);
	if(FormatHead_New.dwFmtSize==18)
		fread((char*)ExtFormatInfo_New,1,sizeof(ExtFormatInfo_New),fNew);
	fread((char*)&FactHead_New,1,sizeof(FactHead_New),fNew);
	if(memcmp(FactHead_New.szFactID,"fact",4)==0)
	{
		fseek(fNew,FactHead_Target.dwFactSize,1);
		fread((char*)&DataHead_New,1,sizeof(DataHead_New),fNew);
	}
	else
	{
		memcpy((char*)&DataHead_New,(char*)&FactHead_New,sizeof(DataHead_New));
		memset((char*)&FactHead_New,0,sizeof(FactHead_New));
	}

	if(	memcmp((char*)&FormatHead_Target,(char*)&FormatHead_New,sizeof(FormatHead_Target))==0)
	{																			//wave格式一致
		fseek(fTarget,0,2);

		TmpSize=sizeof(Tmp);
		j=DataHead_New.dwDataSize/TmpSize;
		for(i=0;i<j;i++)
		{
			fread(Tmp,1,TmpSize,fNew);
			fwrite(Tmp,1,TmpSize,fTarget);
		}
		j=DataHead_New.dwDataSize%TmpSize;
		if(j>0)
		{
			fread(Tmp,1,j,fNew);
			fwrite(Tmp,1,j,fTarget);
		}

		RiffHead_Target.dwRiffSize+=DataHead_New.dwDataSize;
		DataHead_Target.dwDataSize+=DataHead_New.dwDataSize;
		fseek(fTarget,RIFFHeadOff_Target,0);
		fwrite((char*)&RiffHead_Target,1,sizeof(RiffHead_Target),fTarget);
		fseek(fTarget,DataHeadOff_Target,0);
		fwrite((char*)&DataHead_Target,1,sizeof(DataHead_Target),fTarget);
		
		bRlt=TRUE;
	}
	else 
		bRlt=FALSE;																//wave格式不一致



	fclose(fTarget);
	fclose(fNew);
	return bRlt;
}


//	在Wave文件中，删掉指定内容
//	pContent的长度应该大于16
//	UnitBytes指一次跳过去几个字节，16bits编码就是2字节
BOOL CTools_Ansi::RemoveContentFromWaveFile(const char *FileName,const char *pContent,int Len,int UnitBytes)
{
	FILE			*fp,*fpNew;
	char			NewFileName[256];
	char			*pHead;
	struct RIFF_HEADER
	{
		char		szRiffID[4];												// 'R','I','F','F'
		DWORD		dwRiffSize;
		char		szRiffFormat[4];											// 'W','A','V','E'
	}RiffHead;
	struct FMT_BLOCK
	{
		char		szFmtID[4];													// 'f','m','t',' '
		DWORD		dwFmtSize;
		struct WAVE_FORMAT
		{
			WORD	wFormatTag;
			WORD	wChannels;
			DWORD	dwSamplesPerSec;
			DWORD	dwAvgBytesPerSec;
			WORD	wBlockAlign;
			WORD	wBitsPerSample;
		}WavFormat;
	}FormatHead;
	char			ExtFormatInfo[2];
	struct FACT_BLOCK
	{
		char		szFactID[4];												// 'f','a','c','t'
		DWORD		dwFactSize;
	}FactHead;
	struct DATA_BLOCK
	{
		char		szDataID[4];
		DWORD		dwDataSize;
	}DataHead;
	DWORD			RIFFHeadOff,DataHeadOff,DataOff;
	char			*pTmp;
	DWORD			Off,LastOff,OldLen,NewLen;
	DWORD			CmpVal1,CmpVal2,CmpVal3,CmpVal4;


	if(Len<8) return FALSE;
	CmpVal1=*(DWORD*)pContent;
	CmpVal2=*(DWORD*)(pContent+4);
	CmpVal3=*(DWORD*)(pContent+8);
	CmpVal4=*(DWORD*)(pContent+12);

	fp=fopen(FileName,"rb+");
	if(!fp) return FALSE;

	sprintf(NewFileName,"%s_~",FileName);
	fpNew=fopen(NewFileName,"wb");
	if(!fpNew)
	{
		fclose(fp);
		return FALSE;
	}

	memset((char*)&RiffHead,0,sizeof(RiffHead));
	memset((char*)&FormatHead,0,sizeof(FormatHead));
	memset((char*)ExtFormatInfo,0,sizeof(ExtFormatInfo));
	memset((char*)&FactHead,0,sizeof(FactHead));
	memset((char*)&DataHead,0,sizeof(DataHead));

																				//读取Wave的文件头
	RIFFHeadOff=0;
	fread((char*)&RiffHead,1,sizeof(RiffHead),fp);
	fread((char*)&FormatHead,1,sizeof(FormatHead),fp);
	if(FormatHead.dwFmtSize==18)
		fread((char*)ExtFormatInfo,1,sizeof(ExtFormatInfo),fp);
	fread((char*)&FactHead,1,sizeof(FactHead),fp);
	if(memcmp(FactHead.szFactID,"fact",4)==0)
	{
		fseek(fp,FactHead.dwFactSize,1);
		fread((char*)&DataHead,1,sizeof(DataHead),fp);
	}
	else
	{
		memcpy((char*)&DataHead,(char*)&FactHead,sizeof(DataHead));
		memset((char*)&FactHead,0,sizeof(FactHead));
	}
	DataOff=ftell(fp);
	DataHeadOff=DataOff-sizeof(DataHead);



	pHead=(char*)malloc(DataOff);
	if(!pHead)
	{
		fclose(fp);
		fclose(fpNew);
		unlink(NewFileName);
		return FALSE;
	}
	fseek(fp,0,0);
	fread(pHead,1,DataOff,fp);
	fwrite(pHead,1,DataOff,fpNew);
	free(pHead);



	OldLen=DataHead.dwDataSize;
	pTmp=(char*)malloc(OldLen);
	if(!pTmp)
	{
		fclose(fp);
		fclose(fpNew);
		unlink(NewFileName);
		return FALSE;
	}
	fread(pTmp,1,OldLen,fp);
	fclose(fp);



	Off=0;
	LastOff=0;
	NewLen=OldLen;
	while((int)OldLen>=Len&&Off<=OldLen-Len)
	{
		if(	CmpVal1==*(DWORD*)(pTmp+Off)&&
			CmpVal2==*(DWORD*)(pTmp+Off+4)&&
			CmpVal3==*(DWORD*)(pTmp+Off+8)&&
			CmpVal4==*(DWORD*)(pTmp+Off+12)&&
			memcmp(pContent+16,pTmp+Off+16,Len-16)==0)
		{
			NewLen-=Len;
			if(Off>LastOff)
				fwrite(pTmp+LastOff,1,Off-LastOff,fpNew);
			Off+=Len;
			LastOff=Off;
			RiffHead.dwRiffSize-=Len;
			DataHead.dwDataSize-=Len;
			continue;
		}
		Off+=UnitBytes;
	}
	if(OldLen>LastOff)
		fwrite(pTmp+LastOff,1,OldLen-LastOff,fpNew);

	free(pTmp);

	if(NewLen!=OldLen)
	{
		fseek(fpNew,0,0);
		fwrite((char*)&RiffHead,1,sizeof(RiffHead),fpNew);
		fseek(fpNew,DataHeadOff,0);
		fwrite((char*)&DataHead,1,sizeof(DataHead),fpNew);
		fclose(fpNew);
		unlink(FileName);
		rename(NewFileName,FileName);
	}
	else 
	{
		fclose(fpNew);
		unlink(NewFileName);
	}

	return TRUE;
}


//	在Wave文件中，取音频内容
//	返回长度。如果iSize不够大，或pContent为空，仅返回需要的长度
int CTools_Ansi::GetContentFromWaveFile(const char *FileName,char *pContent,DWORD iSize)
{
	FILE			*fp;
	struct RIFF_HEADER
	{
		char		szRiffID[4];												// 'R','I','F','F'
		DWORD		dwRiffSize;
		char		szRiffFormat[4];											// 'W','A','V','E'
	}RiffHead;
	struct FMT_BLOCK
	{
		char		szFmtID[4];													// 'f','m','t',' '
		DWORD		dwFmtSize;
		struct WAVE_FORMAT
		{
			WORD	wFormatTag;
			WORD	wChannels;
			DWORD	dwSamplesPerSec;
			DWORD	dwAvgBytesPerSec;
			WORD	wBlockAlign;
			WORD	wBitsPerSample;
		}WavFormat;
	}FormatHead;
	char			ExtFormatInfo[2];
	struct FACT_BLOCK
	{
		char		szFactID[4];												// 'f','a','c','t'
		DWORD		dwFactSize;
	}FactHead;
	struct DATA_BLOCK
	{
		char		szDataID[4];
		DWORD		dwDataSize;
	}DataHead;
	DWORD			RIFFHeadOff,DataHeadOff;


	fp=fopen(FileName,"rb");
	if(!fp) return 0;

	memset((char*)&RiffHead,0,sizeof(RiffHead));
	memset((char*)&FormatHead,0,sizeof(FormatHead));
	memset((char*)ExtFormatInfo,0,sizeof(ExtFormatInfo));
	memset((char*)&FactHead,0,sizeof(FactHead));
	memset((char*)&DataHead,0,sizeof(DataHead));

																				//读取Wave的文件头
	RIFFHeadOff=0;
	fread((char*)&RiffHead,1,sizeof(RiffHead),fp);
	fread((char*)&FormatHead,1,sizeof(FormatHead),fp);
	if(FormatHead.dwFmtSize==18)
		fread((char*)ExtFormatInfo,1,sizeof(ExtFormatInfo),fp);
	fread((char*)&FactHead,1,sizeof(FactHead),fp);
	if(memcmp(FactHead.szFactID,"fact",4)==0)
	{
		fseek(fp,FactHead.dwFactSize,1);
		fread((char*)&DataHead,1,sizeof(DataHead),fp);
	}
	else
	{
		memcpy((char*)&DataHead,(char*)&FactHead,sizeof(DataHead));
		memset((char*)&FactHead,0,sizeof(FactHead));
	}
	DataHeadOff=ftell(fp)-sizeof(DataHead);

	if(iSize>=DataHead.dwDataSize&&pContent)
		fread(pContent,1,DataHead.dwDataSize,fp);
	fclose(fp);
	return DataHead.dwDataSize;
}


//	从带路径的文件名中取出纯粹的文件名
//	PathFileName:带路径的文件名
//	RltFileName:分离出来的不带路径的文件名
//	RltSize:最大尺寸
bool CTools_Ansi::GetPureFileName(const char* pFileNameWithPath,char* pRltFileName,int RltSize)
{
	int Len,Off;

	if(!pFileNameWithPath||!pRltFileName) return false;

	pRltFileName[0]=0;

	Len=strlen(pFileNameWithPath);
	Off=Len;
	while(Off>0&&pFileNameWithPath[Off-1]!='\\'&&pFileNameWithPath[Off-1]!=':') 
		Off--;

	if(RltSize>0&&Len-Off>=RltSize)
		return false;

	memcpy(pRltFileName,pFileNameWithPath+Off,Len-Off);
	pRltFileName[Len-Off]=0;

	return true;
}

//	从全路径文件名中取出盘符，标志是:
bool CTools_Ansi::GetDiskSymbol(const char* pFileName,char* pRltDiskSymbol,int RltSize)
{
	char *pTemp;
	int Len;

	if(!pFileName||!pRltDiskSymbol) return false;

	pRltDiskSymbol[0]=0;

	pTemp=(char*)strstr(pFileName,":");
	if(!pTemp) return false;

	Len=pTemp-pFileName+1;
	if(RltSize>0&&Len>=RltSize) return false;
	
	memcpy(pRltDiskSymbol,pFileName,Len);
	pRltDiskSymbol[Len]=0;

	return true;
}

//	从全路径文件名中取出路径，不带盘符
bool CTools_Ansi::GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize)
{
	char *pTempHead;
	int OffHead;
	int Len;

	if(!pFileName||!pRltFilePath) return false;

	pRltFilePath[0]=0;

	pTempHead=strstr(pFileName,":");
	OffHead=0;
	if(pTempHead!=NULL)
		OffHead=pTempHead-pFileName+1;
	

	Len=strlen(pFileName);
	while(Len>0&&pFileName[Len-1]!='\\'&&pFileName[Len-1]!=':') 
		Len--;

	if(RltSize>0&&Len-OffHead>=RltSize)
		return false;

	memcpy(pRltFilePath,pFileName+OffHead,Len-OffHead);
	pRltFilePath[Len-OffHead]=0;

	if(Len-OffHead>0&&pRltFilePath[Len-OffHead-1]!='\\'&&(RltSize<=0||Len-OffHead<RltSize-1))
	{
		pRltFilePath[Len-OffHead]='\\';
		pRltFilePath[Len-OffHead+1]=0;
	}

	return true;
}

//	从全路径文件名中取出路径，带盘符
bool CTools_Ansi::GetFilePathWithDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize)
{
	int Len;

	if(!pFileName||!pRltFilePath) return false;

	pRltFilePath[0]=0;

	Len=strlen(pFileName);
	while(Len>0&&pFileName[Len-1]!='\\'&&pFileName[Len-1]!=':') 
		Len--;

	if(RltSize>0&&Len>=RltSize)
		return false;

	memcpy(pRltFilePath,pFileName,Len);
	pRltFilePath[Len]=0;

	if(pRltFilePath[Len-1]!='\\'&&(RltSize<=0||Len<RltSize-1))
	{
		pRltFilePath[Len]='\\';
		pRltFilePath[Len+1]=0;
	}

	return true;
}



//	计算Startt2到Endms1的间隔时间
int CTools_Ansi::GetDeltaMS(time_t Endt1,int Endms1,time_t Startt2,int Startms2)
{
	DWORD			DeltaSec;
	int				iRlt;

	if(Endt1>Startt2||Endt1==Startt2&&Endms1>=Startms2)
	{
		DeltaSec=(DWORD)(Endt1-Startt2);
		if(DeltaSec>(0x7fffffff-1000)/1000) iRlt=0x7fffffff;
		else iRlt=(int)DeltaSec*1000+(Endms1-Startms2);
		return iRlt;
	}
	else
	{
		DeltaSec=(DWORD)(Startt2-Endt1);
		if(DeltaSec>(0x7fffffff-1000)/1000) iRlt=0x7fffffff;
		else iRlt=(int)DeltaSec*1000+(Startms2-Endms1);
		return (-1)*iRlt;
	}
}


void CTools_Ansi::MyTrim(char*pStr)
{
	int				i,j,Len;

	for(i=0,Len=strlen(pStr);i<Len&&pStr[i]==' ';i++);
	for(j=Len;j>i&&pStr[j-1]==' ';j--);

	if(i>0)
	{
		memmove(pStr,pStr+i,j-i);
		memset(pStr+j-i,0,Len-(j-i));
	}
	else if(j<Len)
	{
		memset(pStr+j,0,Len-j);
	}
}


//	取当前的时间，包括time_t的秒数和int的毫秒数
void CTools_Ansi::GetComplexAnsiTime(time_t *pRltSecs,int *pRltMS)
{
	if(pRltSecs&&!pRltMS)
		time(pRltSecs);
	else
	{
		struct _timeb	itime;
		_ftime(&itime);
		if(pRltSecs) *pRltSecs=itime.time;
		if(pRltMS) *pRltMS=itime.millitm;
	}
}





//	从字符串中取出time_t时间
//	日期格式为：xxxx年xx月xx日； xxxx/xx/xx； xxxx-xx-xx
//	时间格式为：xx时xx分xx秒； xx:xx:xx； xx:xx
time_t	CTools_Ansi::GetAnsiTimeFromString(char *pBuf)
{
	struct tm		*ptm;
	time_t			itime;
	int				iYear,iMon,iDay,iHour,iMin,iSec;
	char			*pYear,*pMon,*pDay,*pHour,*pSec,*pp;

	time(&itime);
	ptm=localtime(&itime);

	iYear=ptm->tm_year+1900;
	iMon=ptm->tm_mon+1;
	iDay=ptm->tm_mday;
	iHour=ptm->tm_hour;
	iMin=ptm->tm_min;
	iSec=ptm->tm_sec;

	pYear=strstr(pBuf,"年");
	if(!pYear) pYear=strstr(pBuf,"/");
	if(!pYear) pYear=strstr(pBuf,"-");
	if(pYear)
	{
		pp=pYear;
		while(pp>pBuf&&*(pp-1)==' ') pp--;
		while(pp>pBuf&&*(pp-1)>='0'&&*(pp-1)<='9') pp--;
		iYear=atoi(pp);
	}

	pMon=strstr(pBuf,"月");
	if(!pMon&&pYear)
	{
		if(*pYear=='/') pMon=strstr(pYear+1,"/");
		else if(*pYear=='-') pMon=strstr(pYear+1,"-");
	}
	if(pMon)
	{
		pp=pMon;
		while(pp>pBuf&&*(pp-1)==' ') pp--;
		while(pp>pBuf&&*(pp-1)>='0'&&*(pp-1)<='9') pp--;
		iMon=atoi(pp);
	}

	pDay=strstr(pBuf,"日");
	if(pDay)
	{
		pp=pDay;
		while(pp>pBuf&&*(pp-1)==' ') pp--;
		while(pp>pBuf&&*(pp-1)>='0'&&*(pp-1)<='9') pp--;
		iDay=atoi(pp);
	}
	else if(pMon)
	{
		if(*pMon=='/') pp=pMon+1;
		if(*pMon=='-') pp=pMon+1;
		while(pp[0]!=0&&pp[0]==' ') pp++;
		iDay=atoi(pp);
	}

	pHour=strstr(pBuf,":");
	if(pHour)
	{
		pp=pHour;
		while(pp>pBuf&&*(pp-1)==' ') pp--;
		while(pp>pBuf&&*(pp-1)>='0'&&*(pp-1)<='9') pp--;
		iHour=atoi(pp);

		pp=pHour+1;
		while(pp[0]!=0&&pp[0]==' ') pp++;
		iMin=atoi(pp);

		pSec=strstr(pHour+1,":");
		if(pSec)
		{
			pp=pSec+1;
			while(pp[0]!=0&&pp[0]==' ') pp++;
			iSec=atoi(pp);
		}
	}

	return MakeAnsiTime(iYear,iMon,iDay,iHour,iMin,iSec);

	
}

//	产生一个time_t时间
time_t CTools_Ansi::MakeAnsiTime(int wYear,int wMon,int wDay,int wHour,int wMin,int wSec)
{
	struct tm		itm;

	if(wYear<1970) wYear=1970;
	if(wYear>2038) wYear=2038;
	if(wMon<1) wMon=1;
	if(wMon>12) wMon=12;
	if(wDay<1) wDay=1;
	if(wDay>31) wDay=31;
	if(wHour<0) wHour=0;
	if(wHour>23) wHour=23;
	if(wMin<0) wMin=0;
	if(wMin>59) wMin=59;
	if(wSec<0) wSec=0;
	if(wSec>59) wSec=59;

	memset(&itm,0,sizeof(itm));
	itm.tm_year=wYear-1900;
	itm.tm_mon=wMon-1;
	itm.tm_mday=wDay;
	itm.tm_hour=wHour;
	itm.tm_min=wMin;
	itm.tm_sec=wSec;

	return mktime(&itm);
}

//	将time_t和itimeMS表示的时间转换成yyyy-mm-dd hh:mm:ss.xxx的字符串
//	GetPartMap:要取的时间部分，按位表示，从第右边0位开始表示：年、月、日、时、分、秒、毫秒
char* CTools_Ansi::GetAnsiTimeString(char *pBuf,time_t itime,unsigned int itimeMS,unsigned int GetPartMap)
{
	struct tm		*ptm;
	int				Len;

	if(!pBuf) return NULL;
	pBuf[0]=0;

	if(itime==0)
		return pBuf;

	ptm=localtime(&itime);
	Len=0;
	if(ptm&&itime) 
	{
		if((GetPartMap&1)==1) { sprintf(pBuf+Len,"%04d-",ptm->tm_year+1900);  Len=strlen(pBuf); }
		if((GetPartMap&2)==2) { sprintf(pBuf+Len,"%02d-",ptm->tm_mon+1);  Len=strlen(pBuf); }
		if((GetPartMap&4)==4) { sprintf(pBuf+Len,"%02d",ptm->tm_mday);  Len=strlen(pBuf); }
		if(Len>0&&pBuf[Len-1]=='-') { pBuf[Len-1]=0; Len--; }
		if(Len>0) { strcat(pBuf," ");  Len++; }

		if((GetPartMap&8)==8) { sprintf(pBuf+Len,"%02d:",ptm->tm_hour);  Len=strlen(pBuf); }
		if((GetPartMap&16)==16) { sprintf(pBuf+Len,"%02d:",ptm->tm_min);  Len=strlen(pBuf); }
		if((GetPartMap&32)==32) { sprintf(pBuf+Len,"%02d",ptm->tm_sec);  Len=strlen(pBuf); }
		if((GetPartMap&64)==64) { sprintf(pBuf+Len,".%03d",itimeMS);  Len=strlen(pBuf); }
		if(Len>0&&pBuf[Len-1]==':') { pBuf[Len-1]=0; Len--; }
	}
	return pBuf;
}



//  将字符串表示的时间转换成utc的time_t时间
//  pDate:yyyymmdd
//  pTime:hh:mm:ss 或 hhmmss
time_t CTools_Ansi::Get_time_t_utc_from_strTime(const char* pDate, const char* pTime)
{
    int iYear,iMon,iDay,iHour,iMin,iSec;
    struct tm itm;
    time_t itime;

    if(!pDate&&!pTime) return 0;

    if(!pDate||!pDate[0]) 
    {
        SYSTEMTIME CurTime;
        GetLocalTime(&CurTime);
        iYear=CurTime.wYear;
        iMon=CurTime.wMonth;
        iDay=CurTime.wDay;
    }
    else 
    {
        sscanf(pDate,"%04d%02d%02d",&iYear,&iMon,&iDay);
    }

    if(!pTime||!pTime[0]) 
        iHour=iMin=iSec=0;
    else if(pTime[2]==':'&&pTime[5]==':')
        sscanf(pTime,"%02d:%02d:%02d",&iHour,&iMin,&iSec);
    else
        sscanf(pTime,"%02d%02d%02d",&iHour,&iMin,&iSec);

    if(iYear<1970||iYear>2030||iMon<1||iMon>12||iDay<1||iDay>31||iHour<0||iHour>23||iMin<0||iMin>59||iSec<0||iSec>59) 
        return 0;

    memset(&itm,0,sizeof(itm));
    itm.tm_year=iYear-1900;
    itm.tm_mon=iMon-1;
    itm.tm_mday=iDay;
    itm.tm_hour=iHour;
    itm.tm_min=iMin;
    itm.tm_sec=iSec;

    itime=mktime(&itm);                                     //得到utc时间

    return itime;
}

/// 将时间字符串转为秒数
/// pTime:hhmmss 或 hh:mm:ss
unsigned int CTools_Ansi::CalculateTotalSeconds(const char* pTime)
{
	if(pTime==NULL || pTime[0]=='\0')
		return 0;

	int iHour=0;
	int iMin=0;
	int iSec=0;
	unsigned int len=strlen(pTime);
	if(len==6)
	{
		sscanf(pTime,"%02d%02d%02d",&iHour,&iMin,&iSec);
	}
	else if(len==8 && pTime[2]==':'&&pTime[5]==':')
	{
		sscanf(pTime,"%02d:%02d:%02d",&iHour,&iMin,&iSec);
	}

	if(iHour<0||iHour>23||iMin<0||iMin>59||iSec<0||iSec>59)
		return 0;

	return (iHour*3600+iMin*60+iSec);

}


//  将utc的time_t时间转换成字符串表示的时间
//  pRltDate:yyyymmdd，要保证足够的空间
//  pRltTime:hh:mm:ss，要保证足够的空间
void CTools_Ansi::Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime)
{
    struct tm *ptm;
    ptm=localtime(&iTime);
    if(!ptm) return;

    if(pRltDate)
        sprintf(pRltDate,"%04d%02d%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);
    if(pRltTime)
        sprintf(pRltTime,"%02d:%02d:%02d",ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
}


//  将utc的time_t时间转换成本地时间
void CTools_Ansi::Get_DateTime_from_utc(time_t iTime,int* pRltYear,int* pRltMonth,int* pRltDay,int* pRltHour,int* pRltMinutes,int * pRltSecs,int* pRltWeekDay)
{
    struct tm *ptm;

    if(pRltYear) *pRltYear=0;
    if(pRltMonth) *pRltMonth=0;
    if(pRltDay) *pRltDay=0;
    if(pRltHour) *pRltHour=0;
    if(pRltMinutes) *pRltMinutes=0;
    if(pRltSecs) *pRltSecs=0;
    if(pRltWeekDay) *pRltWeekDay=0;

    ptm=localtime(&iTime);
    if(!ptm) return;

    if(pRltYear) *pRltYear=ptm->tm_year+1900;
    if(pRltMonth) *pRltMonth=ptm->tm_mon+1;
    if(pRltDay) *pRltDay=ptm->tm_mday;
    if(pRltHour) *pRltHour=ptm->tm_hour;
    if(pRltMinutes) *pRltMinutes=ptm->tm_min;
    if(pRltSecs) *pRltSecs=ptm->tm_sec;
    if(pRltWeekDay) *pRltWeekDay=ptm->tm_wday;
}
//  将本地时间转换成utc时间
time_t CTools_Ansi::Get_utc_from_DateTime(int year,int month,int day,int hour,int minutes,int secs)
{
    struct tm itm;

    memset(&itm,0,sizeof(itm));
    itm.tm_year=year-1900;
    itm.tm_mon=month-1;
    itm.tm_mday=day;
    itm.tm_hour=hour;
    itm.tm_min=minutes;
    itm.tm_sec=secs;

    return mktime(&itm);                                     //得到utc时间
}


//  将字符串表示的时间转换成当天00:00:00开始的秒数
//  pTime:hhmmss 或 hh:mm:ss 或hh:mm 或 h:mm 或 h:m 或 hh:m
int CTools_Ansi::Get_TodaySecs_from_strTime(char* pTime)
{
    int iHour=0,iMin=0,iSec=0;

    if(!pTime) return 0;

    if(pTime[2]==':'&&pTime[5]==':')
        sscanf(pTime,"%02d:%02d:%02d",&iHour,&iMin,&iSec);
    else if(strlen(pTime)==6)
        sscanf(pTime,"%02d%02d%02d",&iHour,&iMin,&iSec);
    else if(strlen(pTime)==5&&pTime[2]==':')
        sscanf(pTime,"%02d:%02d",&iHour,&iMin);
    else if(strlen(pTime)==4&&pTime[1]==':')
        sscanf(pTime,"%01d:%02d",&iHour,&iMin);
    else if(strlen(pTime)==3&&pTime[1]==':')
        sscanf(pTime,"%01d:%01d",&iHour,&iMin);
    else if(strlen(pTime)==4&&pTime[2]==':')
        sscanf(pTime,"%02d:%01d",&iHour,&iMin);

    if(iHour<0||iHour>23||iMin<0||iMin>59||iSec<0||iSec>59) 
        return 0;

    return iHour*3600+iMin*60+iSec;
}


//	将一个数值转换成二进制表示的字符串
//	转换结果按从右到左的顺序排列，bit0在pRlt[BitNum-1]
char* CTools_Ansi::GetBinFormatString(char *pRlt,unsigned int Val,unsigned int BitNum)
{
	unsigned int	i;

	if(!pRlt) return NULL;

	if(BitNum>32) BitNum=32;
	memset(pRlt,0,BitNum+1);
	for(i=0;i<BitNum;i++)
	{
		if( ( Val & ((unsigned int)1<<i) ) != 0 )
			pRlt[BitNum-1-i]='1';
		else 
			pRlt[BitNum-1-i]='0';
	}
	return pRlt;
}

//	将一个数值转换成二进制表示的字符串
//	转换结果按从左到右的顺序排列，bit0在pRlt[0]
char* CTools_Ansi::GetBinFormatString2(char *pRlt,unsigned int Val,unsigned int BitNum)
{
	unsigned int	i;

	if(!pRlt) return NULL;

	if(BitNum>32) BitNum=32;
	memset(pRlt,0,BitNum+1);
	for(i=0;i<BitNum;i++)
	{
		if( ( Val & ((unsigned int)1<<i) ) != 0 )
			pRlt[i]='1';
		else 
			pRlt[i]='0';
	}
	return pRlt;
}



//	下面的宏在InternalLookupInFieldIndex()中使用，用于计算iMid、iHead、iTail
//	使用到如下标志:
//	LookupMidCompleteID;					//二分法找Mid完成
//	LookupHeadID;							//二分法找头
//	LookupHeadCompleteID;					//找头完成
//	LookupHeadBackwardOneByOneID;			//向前一个一个的精确查找第一个不等于Mid的mid
//	LookupHeadForwardOneByOneID;			//向后一个一个的精确查找第一个等于Mid的mid
//	LookupTailID;							//二分法找尾
//	LookupTailCompleteID;					//找尾完成
//	LookupTailBackwardOneByOneID;			//向前一个一个的精确查找第一个等于Mid的mid
//	LookupTailForwardOneByOneID;			//向后一个一个的精确查找第一个不等于Mid的mid
//	下面的宏在InternalLookupInFieldIndex()中使用，用于计算Mid、Head、Tail，结果放在CmpRlt、iHead、iTail、iMid中
#define CalculateMid_LookupInSequenceBuffer															\
{																									\
	if(LookupMidCompleteID==0)																		\
	{																								\
		Calculate_GetMid_LookupInSequenceBuffer;													\
	}																								\
	else																							\
	{																								\
		if(LookupHeadID==1&&LookupHeadCompleteID==0)												\
		{																							\
			Calculate_GetHead_LookupInSequenceBuffer;												\
		}																							\
		else if(LookupTailID==1&&LookupTailCompleteID==0)											\
		{																							\
			Calculate_GetTail_LookupInSequenceBuffer;												\
		}																							\
		if((LookupHeadID==0||LookupHeadCompleteID==1)&&(LookupTailID==0||LookupTailCompleteID==1))	\
		{																							\
			CmpRlt=0;																				\
			break;																					\
		}																							\
	}																								\
}



//	下面的宏在InternalLookupInFieldIndex()中使用，用于计算Mid、Head、Tail，结果放在CmpRlt、iHead、iTail、iMid中
//	此宏用于计算二分法的Mid，在获取f(Mid)=pSour的Mid时调用
#define Calculate_GetMid_LookupInSequenceBuffer														\
{																									\
	if(CmpRlt>0&&Head!=Mid) { Head=Mid;  Mid=(Head+Tail)/2; }										\
	else if(CmpRlt>0&&Head==Mid)																	\
	{																								\
		CmpRlt=-1;  LookupMidCompleteID=1;															\
		iHead=Head;  iTail=Tail;  iMid=Mid;															\
		break;																						\
	}																								\
	else if(CmpRlt<0&&Tail!=Mid) { Tail=Mid;  Mid=(Head+Tail)/2; }									\
	else if(CmpRlt<0&&Tail==Mid)																	\
	{																								\
		Head--; CmpRlt=-1; LookupMidCompleteID=1;													\
		iHead=Head;  iTail=Tail;  iMid=Mid;															\
		break;																						\
	}																								\
	else																							\
	{																								\
		iHead=Head;																					\
		iTail=Tail;																					\
		iMid=Mid;																					\
		LookupMidCompleteID=1;																		\
		if(pLastHead) { LookupHeadID=1;  Head=iHead;  Tail=iMid;  Mid=(Head+Tail)/2; }				\
		else if(pLastTail) { LookupTailID=1;  Head=iMid;  Tail=iTail;  Mid=(Head+Tail)/2; }			\
		else break; 																				\
	}																								\
}



//	下面的宏在InternalLookupInFieldIndex()中使用，用于计算Mid、Head、Tail，结果放在CmpRlt、iHead、iTail、iMid中
//	此宏用于精确计算f(iMid)==pSour时的Head，在获取f(iMid)=pSour的iMid后调用
#define Calculate_GetHead_LookupInSequenceBuffer													\
{																									\
	if(LookupHeadBackwardOneByOneID==0&&LookupHeadForwardOneByOneID==0)								\
	{																								\
		if(CmpRlt==0&&Tail!=Mid) { Tail=Mid;  Mid=(Head+Tail)/2;  }									\
		else if(CmpRlt==0&&Tail==Mid) { LookupHeadBackwardOneByOneID=1; }							\
		else if(CmpRlt!=0&&Head!=Mid) { Head=Mid;  Mid=(Head+Tail)/2; }								\
		else if(CmpRlt!=0&&Head==Mid) { LookupHeadBackwardOneByOneID=1; }							\
	}																								\
	if(LookupHeadBackwardOneByOneID==1&&LookupHeadForwardOneByOneID==0)								\
	{																								\
		if(Mid<=0) { Mid=0;  LookupHeadForwardOneByOneID=1; }										\
		else if(CmpRlt!=0) { LookupHeadForwardOneByOneID=1; }										\
		else																						\
		{																							\
			Mid--;																					\
			if(Mid<=0) { Mid=0;  LookupHeadForwardOneByOneID=1; }									\
		}																							\
	}																								\
	if(LookupHeadBackwardOneByOneID==1&&LookupHeadForwardOneByOneID==1)								\
	{																								\
		if(Mid>=iMid) { Mid=iMid;  LookupHeadCompleteID=1; }										\
		else if(CmpRlt==0) LookupHeadCompleteID=1;													\
		else																						\
		{																							\
			Mid++;																					\
			if(Mid>=iMid) { Mid=iMid;  LookupHeadCompleteID=1; }									\
		}																							\
																									\
		if(LookupHeadCompleteID)																	\
		{																							\
			if(Mid<0) Mid=0;																		\
			if(Mid>iMid) Mid=iMid;																	\
			iHead=Mid;																				\
																									\
			if(pLastTail) { LookupTailID=1;  Head=iMid;  Tail=iTail;  Mid=(Head+Tail)/2; }			\
			else { CmpRlt=0;  break; }																\
		}																							\
	}																								\
}


//	下面的宏在InternalLookupInFieldIndex()中使用，用于计算Mid、Head、Tail，结果放在CmpRlt、iHead、iTail、iMid中
//	此宏用于精确计算f(iMid)==pSour时的Tail，在获取f(iMid)=pSour的iMid后调用
#define Calculate_GetTail_LookupInSequenceBuffer													\
{																									\
	if(LookupTailBackwardOneByOneID==0&&LookupTailForwardOneByOneID==0)								\
	{																								\
		if(CmpRlt==0&&Head!=Mid) { Head=Mid;  Mid=(Head+Tail)/2;  }									\
		else if(CmpRlt==0&&Head==Mid) { LookupTailBackwardOneByOneID=1; }							\
		else if(CmpRlt!=0&&Tail!=Mid) { Tail=Mid;  Mid=(Head+Tail)/2; }								\
		else if(CmpRlt!=0&&Tail==Mid) { LookupTailBackwardOneByOneID=1; }							\
	}																								\
	if(LookupTailBackwardOneByOneID==1&&LookupTailForwardOneByOneID==0)								\
	{																								\
		if(Mid<=iMid) { Mid=iMid;  LookupTailForwardOneByOneID=1; }									\
		else if(CmpRlt==0) { LookupTailForwardOneByOneID=1; }										\
		else																						\
		{																							\
			Mid--;																					\
			if(Mid<=iMid) { Mid=iMid;  LookupTailForwardOneByOneID=1; }								\
		}																							\
	}																								\
	if(LookupTailBackwardOneByOneID==1&&LookupTailForwardOneByOneID==1)								\
	{																								\
		if(Mid>=UnitNum) { Mid=UnitNum;  LookupTailCompleteID=1; }					\
		else if(CmpRlt!=0) LookupTailCompleteID=1;													\
		else																						\
		{																							\
			Mid++;																					\
			if(Mid>=UnitNum) { Mid=UnitNum;  LookupTailCompleteID=1; }				\
		}																							\
																									\
		if(LookupTailCompleteID)																	\
		{																							\
			if(Mid<=iMid) Mid=iMid+1;																\
			if(Mid>UnitNum) Mid=UnitNum;												\
			iTail=Mid;																				\
			CmpRlt=0;																				\
			break;																					\
		}																							\
	}																								\
}




//	用二分法在一个排序的缓冲区里查找一个字符串
//	排序是从小到大
//	大小写无关
//	1. 如找到该数值，返回其在索引表中的序号，pLastHead是第一个满足条件的位置，pLastTail是第一个不满足条件的位置，pLastHead<=x<pLastTail均满足条件
//	2. 如找不到，返回-1，pLastHead和pLastTail是包含指定字符串的上下限值 *pLastHead=(-1 ~ Max-1) *pLastTail=(0 ~ Max)
int CTools_Ansi::LookupInSequenceBuffer_Mem(char *SeqBuf,int UnitNum,int UnitSize,char *Str,int Len,int *pLastHead,int *pLastTail)
{
	int				Head,Tail,Mid,iHead,iTail,iMid,CmpRlt;
	int				LookupMidCompleteID;					//二分法找Mid完成
	int				LookupHeadID;							//二分法找头
	int				LookupHeadCompleteID;					//找头完成
	int				LookupHeadBackwardOneByOneID;			//向前一个一个的精确查找第一个不等于Mid的mid
	int				LookupHeadForwardOneByOneID;			//向后一个一个的精确查找第一个等于Mid的mid
	int				LookupTailID;							//二分法找尾
	int				LookupTailCompleteID;					//找尾完成
	int				LookupTailBackwardOneByOneID;			//向前一个一个的精确查找第一个等于Mid的mid
	int				LookupTailForwardOneByOneID;			//向后一个一个的精确查找第一个不等于Mid的mid

	if(Len>UnitSize) return -1;

	Head=0;
	Tail=UnitNum;
	Mid=(Head+Tail)/2;

	LookupMidCompleteID=0;									//二分法找Mid完成
	LookupHeadID=0;											//二分法找头
	LookupHeadCompleteID=0;									//找头完成
	LookupHeadBackwardOneByOneID=0;							//向前一个一个的精确查找第一个不等于Mid的mid
	LookupHeadForwardOneByOneID=0;							//向后一个一个的精确查找第一个等于Mid的mid
	LookupTailID=0;											//二分法找尾
	LookupTailCompleteID=0;									//找尾完成
	LookupTailBackwardOneByOneID=0;							//向前一个一个的精确查找第一个等于Mid的mid
	LookupTailForwardOneByOneID=0;							//向后一个一个的精确查找第一个不等于Mid的mid
	
	while(1)
	{
		CmpRlt=_memicmp(Str,SeqBuf+Mid*UnitSize,Len);
		CalculateMid_LookupInSequenceBuffer;
	}
	if(pLastHead) *pLastHead=Head;
	if(pLastTail) *pLastTail=Tail;

	if(CmpRlt==0) return iMid;
	else return -1;
}

//	用二分法在一个排序的缓冲区里查找一个DWORD
//	如找到该DWORD，返回其在缓冲区里的序号，否则返回-1，pLastHead和pLastTail是包含指定字符串的上下限值
//	排序是从小到大
//	大小写无关
int CTools_Ansi::LookupInSequenceBuffer_DWORD(char *SeqBuf,int UnitNum,int UnitSize,unsigned int dwID,int *pLastHead,int *pLastTail)
{
	int				Head,Tail,Mid,iHead,iTail,iMid,CmpRlt;
	int				LookupMidCompleteID;					//二分法找Mid完成
	int				LookupHeadID;							//二分法找头
	int				LookupHeadCompleteID;					//找头完成
	int				LookupHeadBackwardOneByOneID;			//向前一个一个的精确查找第一个不等于Mid的mid
	int				LookupHeadForwardOneByOneID;			//向后一个一个的精确查找第一个等于Mid的mid
	int				LookupTailID;							//二分法找尾
	int				LookupTailCompleteID;					//找尾完成
	int				LookupTailBackwardOneByOneID;			//向前一个一个的精确查找第一个等于Mid的mid
	int				LookupTailForwardOneByOneID;			//向后一个一个的精确查找第一个不等于Mid的mid

	Head=0;
	Tail=UnitNum;
	Mid=(Head+Tail)/2;

	LookupMidCompleteID=0;									//二分法找Mid完成
	LookupHeadID=0;											//二分法找头
	LookupHeadCompleteID=0;									//找头完成
	LookupHeadBackwardOneByOneID=0;							//向前一个一个的精确查找第一个不等于Mid的mid
	LookupHeadForwardOneByOneID=0;							//向后一个一个的精确查找第一个等于Mid的mid
	LookupTailID=0;											//二分法找尾
	LookupTailCompleteID=0;									//找尾完成
	LookupTailBackwardOneByOneID=0;							//向前一个一个的精确查找第一个等于Mid的mid
	LookupTailForwardOneByOneID=0;							//向后一个一个的精确查找第一个不等于Mid的mid
	
	while(1)
	{
		if(dwID>(*(unsigned int*)(SeqBuf+Mid*UnitSize))) CmpRlt=1;
		else if(dwID<(*(unsigned int*)(SeqBuf+Mid*UnitSize))) CmpRlt=-1;
		else CmpRlt=0;
		
		CalculateMid_LookupInSequenceBuffer;
	}
	if(pLastHead) *pLastHead=Head;
	if(pLastTail) *pLastTail=Tail;

	if(CmpRlt==0) return iMid;
	else return -1;
}
#undef	CalculateMid_LookupInSequenceBuffer
#undef	Calculate_GetMid_LookupInSequenceBuffer
#undef	Calculate_GetTail_LookupInSequenceBuffer
#undef	Calculate_GetHead_LookupInSequenceBuffer



//	用二分法在一个排序的缓冲区里查找内容
//	排序是从小到大
//  结果放在RltBegin和RltEnd中，表示第一个满足条件的位置和第一个不满足条件的位置。(二者相等，表示没找到)
void CTools_Ansi::LookupInSequenceBuffer(int& RltBegin,int& RltEnd,
                                         const void* pData,int UnitNum,int UnitSize,
                                         const void* pLookupContent,
                                         COMPARE_LookupInSequenceBuffer CompareMethod)
{
	int				Head,Tail,Mid,iHead,iTail,iMid,CmpRlt;

    if(UnitNum<=0)
    {
        RltBegin=RltEnd=0;
        return;
    }

    Head=0;
	Tail=UnitNum+1;         //查找时不包含Tail
	Mid=(Head+Tail)/2;

    //先查找指定内容。查找过程中，Head<=LookupContent<Tail
    while(1)
    {
        CmpRlt=CompareMethod(pLookupContent,(const char*)pData+Mid*UnitSize);

	    if(CmpRlt>0&&Mid!=Head) { Head=Mid;  Mid=(Head+Tail)/2; }
	    else if(CmpRlt<0&&Mid!=Tail) { Tail=Mid;  Mid=(Head+Tail)/2; }
	    else if(CmpRlt!=0)
	    {   
            //表示要查找的内容在N和N+1之间(Tail=Head+1)，或小于第一个数据(Tail==Head==0)，没有结果
            RltBegin=RltEnd=Tail;       
            return;
	    }
	    else 
	    {
            //找到了结果, CmpRlt==0，要查找的内容在Head和Tail之间
            break;
	    }
    }

    //这是的Head和Tail是可能的上限和下限
    iHead=Head;
    iTail=Tail;
    iMid=Mid;

    //用二分法向前查找第一个符合条件的值(上限)，在Head-Mid之间。查找过程中，Head!=LookupContent && Tail==LookupContent
    Head=iHead;
    Tail=iMid;
    Mid=(Head+Tail)/2;
    while(1)
    {
        CmpRlt=CompareMethod(pLookupContent,(const char*)pData+Mid*UnitSize);

	    if(CmpRlt==0&&Mid!=Tail) { Tail=Mid;  Mid=(Head+Tail)/2; }
	    else if(CmpRlt!=0&&Mid!=Head) { Head=Mid;  Mid=(Head+Tail)/2; }
	    else 
	    {   
            //这时的Tail就是第一个符合条件的值(上限)
            RltBegin=Tail;       
            break;
	    }
    }

    //用二分法向后查找第一个不符合条件的值(下限)，在Mid-Tail之间。查找过程中，Head==LookupContent && Tail!=LookupContent
    Head=iMid;
    Tail=iTail;
    Mid=(Head+Tail)/2;
    while(1)
    {
        CmpRlt=CompareMethod(pLookupContent,(const char*)pData+Mid*UnitSize);

	    if(CmpRlt!=0&&Mid!=Tail) { Tail=Mid;  Mid=(Head+Tail)/2; }
	    else if(CmpRlt==0&&Mid!=Head) { Head=Mid;  Mid=(Head+Tail)/2; }
	    else 
	    {   
            //这时的Tail就是第一个不符合条件的值(下限)
            RltEnd=Tail;       
            break;
	    }
    }

    return;
}



//	用冒泡法进行排序。排序是从小到大
//	Buf是要排序的缓冲区，缓冲区由UnitNum个单元组成，每个单元的长度为UnitSize
//	CmpOff为要比较的串在单元里的位置，CmpLen为比较的长度
//	大小写无关
void CTools_Ansi::SetSequenceInBubble(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen)
{
	int			i,j,AdjustNum,Rlt;
	char		Tmp[1024];

	if(UnitSize>1024) return;
	for(i=0;i<UnitNum-1;i++)
	{
		AdjustNum=0;
		for(j=UnitNum-1;j>i;j--)
		{
			Rlt=_memicmp(Buf+CmpOff+(j-1)*UnitSize,Buf+CmpOff+j*UnitSize,CmpLen);
			if(Rlt>0)
			{
				memcpy(Tmp,Buf+(j-1)*UnitSize,UnitSize);
				memcpy(Buf+(j-1)*UnitSize,Buf+j*UnitSize,UnitSize);
				memcpy(Buf+j*UnitSize,Tmp,UnitSize);
				AdjustNum++;
			}
		}
		if(!AdjustNum) break;								//没有要交换的项，都已经排好序了
	}
}


//	取指定范围内的随机数
unsigned int CTools_Ansi::GetRandomValue(unsigned int MinVal,unsigned int MaxVal)
{
	unsigned int Val1,Val2,Val;
	struct _timeb	itime;

	_ftime(&itime);

	srand((unsigned int)itime.millitm);
	Val1=rand();
	if(MaxVal-MinVal>0&&MaxVal-MinVal<RAND_MAX)
		Val=Val1%(MaxVal-MinVal)+MinVal;
	else if(MaxVal-MinVal>0&&RAND_MAX>0&&MaxVal-MinVal>RAND_MAX)
	{
		Val2=rand();
		Val=(Val1*((MaxVal-MinVal)/RAND_MAX)+Val2)%(MaxVal-MinVal)+MinVal;
	}
	else Val=MinVal;
	return Val;
}
