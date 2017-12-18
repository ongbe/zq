// Tools_Ansi.h: interface for the CTools_Ansi class.
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_)
#define AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include <time.h>
#include "public.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 


#define Tools_Ansi_GetBit(pMap_uint32,Offset) 		(((((uint32*)(pMap_uint32))[(Offset)/32]) & ((uint32)1<<(uint32)((Offset)%32)))==0?0:1)
#define Tools_Ansi_SetBit(pMap_uint32,Offset)		((((uint32*)(pMap_uint32))[(Offset)/32]) |= ((uint32)1<<(uint32)((Offset)%32)))
#define Tools_Ansi_ClrBit(pMap_uint32,Offset) 		((((uint32*)(pMap_uint32))[(Offset)/32]) &= (~((uint32)1<<(uint32)((Offset)%32))))
#define Tools_Ansi_CMPDATA(p1,p2,Type)				( (*(Type*)(p1)==*(Type*)(p2)) ? 0 : ( (*(Type*)(p1)) > (*(Type*)(p2)) ? 1 : -1))
#define Tools_Ansi_IsDialPadKey(Key) ((Key)>=(int)'0'&&(Key)<=(int)'9'||(Key)==(int)'*'||(Key)==(int)'#'?1:0)

class CTools_Ansi  
{
public:
	CTools_Ansi();
	virtual ~CTools_Ansi();


public:

	//替换串中的全部子串
	static void ReplaceStr(char *Str,char *SubStr,char *NewSubStr);	
	//替换串中的第一个子串
	static void ReplaceStrOnce(char *Str,char *SubStr,char *NewSubStr);

	static void DeleteStrRightSpace(char *Str);
	static void DeleteStrLeftRightSpace(char *Str);
	static void DeleteStrLeftRightSpace_10_13(char *Str);
	static void DeleteStrLeftSpace(char *Str);

	//description:look for offset of the next digit bytes, current digit word will be ignore
	static int GetNextDigitOff(char *pStr,int Len=0,int StartOff=0);	
	//description	:look for offset of the next character bytes, Len can be empty
	static int GetNextCharOff(char Ch,char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next two character bytes, Len can be empty
	static int GetNextCharOff2(char Ch1,char Ch2,char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next char that is't space、tab、\r、\n, Len can be empty
	static int GetNextValidOff(char *pStr,int Len,int StartOff);		
	//description	:look for offset of the next char that is space、tab、\r、\n, Len can be empty
	static int GetNextInvalidOff(char *pStr,int Len,int StartOff);		
	//description	:向前搜索，找第一个上一个字符是Valid的位置
	static int GetPrevInvalidOff(char *pStr,int StartOff,int MinOff=0);
	//description	:look for offset of the next char that is special character, or space、tab、\r、\n, Len can be empty
	static int GetNextCharOrInvalidOff(char Ch,char *pStr,int Len,int StartOff);	
	//description	:look for offset of the next field, seperated by Seperator，Len can be empty
	static int GetNextFieldOff(char *pStr,int Len,int StartOff,char Seperator1,char Seperator2,char Seperator3,char Seperator4);	
	//取指定位置开始到指定间隔符之前的字符串，压缩头尾的空格和tab
	static BOOL GetCurField(char *pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static int GetFourField(char *pStr,int Len,int StartOff,int *RltFieldStart1,int *RltFieldEnd1,int *RltFieldStart2,int *RltFieldEnd2,int *RltFieldStart3,int *RltFieldEnd3,int *RltFieldStart4,int *RltFieldEnd4,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	//description	:get sub string in Yuankuohao
	static BOOL GetSubStrInYuankuohao(const char *pSour,char *pRlt,int RltSize);			


    //执行WSAStartup
    static void PerformWSAStartup(void);
    //执行WSACleanup
    static void PerformWSACleanup(void);
	//get local host name and local ip, 注意，调用此函数前要调用WSAStartup(). pHostName可以为NULL
	static BOOL GetLocalHostName_IP(char *pHostName,int cbHostName,DWORD *pLocalIP=NULL,int MaxIPCount=1);			

	//function		:GetIPFromHostName
	//description	:1. get local host name and local ip
    //               2. 可用于将域名解析为IP
	//return		:IP number in result
	static int GetIPFromHostName(char *pHostName,DWORD *pHostIP,int MaxIPCount);
	//将字符串形式的IP地址转换成DWORD
	static DWORD ConvertStrIP(const char *pIP);																			
	//将DWORD形式的IP地址转换成字符串
	static char* ConvertdwIP(DWORD dwIP);
	//判断是不是同一个主机
	static BOOL IsSameHost(DWORD* iHostIP,int IPCount,DWORD TargetIP);												

	//根据分隔符，从字符串中取出第N个字段
	static BOOL GetFieldFromStr(const char *pData,int FieldOrd,const char *pSeperator,char *RltBuf,int RltSize);	


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
	static BOOL IsNowValidDateTimeStr(const char *strDateTime);


	//	判断字符串里的时间是否包含当前时间
	//	1. strDate取值范围：
	//		工作日、周末、周1、周2、周3、周4、周5、周6、周7、周0、周1-周3
	//		1号、2号、......31号、1号-10号
	//		1月1日、12月31日、1月1日-12月31日
	//	2. 时间内不能有空格
	static BOOL IsNowValidDateStr(const char *strDate);


	//	判断字符串里的时间是否包含当前时间
	//	1. strTime格式：
	//		hh:mm:ss
	//		hh:mm:ss-hh:mm:ss
	//		hh:mm
	//		hh:mm-hh:mm
	//	2. 时间内不能有空格
	static BOOL IsNowValidTimeStr(const char *strTime);

	//	把NewFile指向的wave文件合并到TargetFile指向的wave文件中
	//	NewFile和TargetFile文件的格式必须一致
	static BOOL AppendWaveFileToAnother(const char *TargetFile,const char *NewFile);
	//	在Wave文件中，取音频内容
	//	返回长度。如果iSize不够大，或pContent为空，仅返回需要的长度
	static int GetContentFromWaveFile(const char *FileName,char *pContent,DWORD iSize);
	//	在Wave文件中，删掉指定内容
	//	pContent的长度应该大于16
	//	UnitBytes指一次跳过去几个字节，16bits编码就是2字节
	static BOOL RemoveContentFromWaveFile(const char *FileName,const char *pContent,int Len,int UnitBytes);

	//	从带路径的文件名中取出纯粹的文件名
	//	PathFileName:带路径的文件名
	//	RltFileName:分离出来的不带路径的文件名
	//	RltSize:最大尺寸
	static bool GetPureFileName(const char* pFileNameWithPath,char* pRltFileName,int RltSize);
	//	从全路径文件名中取出盘符，标志是:
	static bool GetDiskSymbol(const char* pFileName,char* pRltDiskSymbol,int RltSize);
	//	从全路径文件名中取出路径，不带盘符
	static bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);
	//	从全路径文件名中取出路径，带盘符
	static bool GetFilePathWithDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);


	static int GetDeltaMS(time_t Endt1,int Endms1,time_t Startt2,int Startms2);
	static void MyTrim(char*pStr);



	//	取当前的时间，包括time_t的秒数和int的毫秒数
	static void GetComplexAnsiTime(time_t *pRltSecs,int *pRltMS);


	//	从字符串中取出time_t时间
	//	日期格式为：xxxx年xx月xx日； xxxx/xx/xx； xxxx-xx-xx
	//	时间格式为：xx时xx分xx秒； xx:xx:xx； xx:xx
	static time_t	GetAnsiTimeFromString(char *pBuf);

	//	产生一个time_t时间
	static time_t MakeAnsiTime(int wYear,int wMon,int wDay,int wHour,int wMin,int wSec);

	//	将time_t和itimeMS表示的时间转换成yyyy-mm-dd hh:mm:ss.xxx的字符串
	//	GetPartMap:要取的时间部分，按位表示，从第0位开始表示：年、月、日、时、分、秒、毫秒
	static char *GetAnsiTimeString(char *pBuf,time_t itime,unsigned int itimeMS,unsigned int GetPartMap=0xffffffff);

    //  将字符串表示的时间转换成utc的time_t时间
    //  pDate:yyyymmdd
    //  pTime:hhmmss 或 hh:mm:ss
    static time_t Get_time_t_utc_from_strTime(const char* pDate, const char* pTime);

	/// 将时间字符串转为秒数
	/// pTime:hhmmss 或 hh:mm:ss
	static unsigned int CalculateTotalSeconds(const char* pTime);

    //  将utc的time_t时间转换成字符串表示的时间
    //  pRltDate:yyyymmdd
    //  pRltTime:hh:mm:ss
    static void Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime);

    //  将utc的time_t时间转换成本地时间
    static void Get_DateTime_from_utc(time_t iTime,int* pRltYear=NULL,int* pRltMonth=NULL,int* pRltDay=NULL,int* pRltHour=NULL,int* pRltMinutes=NULL,int * pRltSecs=NULL,int* pRltWeekDay=NULL);
    //  将本地时间转换成utc时间
    static time_t Get_utc_from_DateTime(int year,int month,int day,int hour=0,int minutes=0,int secs=0);

    //  将字符串表示的时间转换成当天00:00:00开始的秒数
    //  pTime:hhmmss 或 hh:mm:ss
    static int Get_TodaySecs_from_strTime(char* pTime);


	//	将一个数值转换成二进制表示的字符串，bit0在pRlt[BitNum-1]
	static char *GetBinFormatString(char *pRlt,unsigned int Val,unsigned int BitNum);	
	//	将一个数值转换成二进制表示的字符串bit0在pRlt[0]
	static char *GetBinFormatString2(char *pRlt,unsigned int Val,unsigned int BitNum);


	//用二分法在一个排序的缓冲区里查找一个字符串
	static int LookupInSequenceBuffer_Mem(char *SeqBuf,int UnitNum,int UnitSize,char *Str,int Len,int *pLastHead=NULL,int *pLastTail=NULL);
	//用二分法在一个排序的缓冲区里查找一个DWORD
	static int LookupInSequenceBuffer_DWORD(char *SeqBuf,int UnitNum,int UnitSize,unsigned int dwID,int *pLastHead=NULL,int *pLastTail=NULL);

    //在LookupInSequenceBuffer()中用到的比较函数
    typedef	int	(*COMPARE_LookupInSequenceBuffer)(const void* pdata1,const void* pdata2);
    //	用二分法在一个排序的缓冲区里查找内容
    //	排序是从小到大
    //  结果放在RltBegin和RltEnd中，表示第一个满足条件的位置和第一个不满足条件的位置。(二者相等，表示没找到)
    void LookupInSequenceBuffer(int& RltBegin,int& RltEnd,
                                 const void* pData,int UnitNum,int UnitSize,
                                 const void* pLookupContent,
                                 COMPARE_LookupInSequenceBuffer CompareMethod);

	//用冒泡法进行排序。排序是从小到大
	static void SetSequenceInBubble(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);
																
	//	取指定范围内的随机数
	static unsigned int GetRandomValue(unsigned int MinVal,unsigned int MaxVal);

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_)
