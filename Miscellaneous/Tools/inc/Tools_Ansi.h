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
#include <map>
#include <set>
#include <vector>
#include <string>
using std::map;
using std::set;
using std::multimap;
using std::vector;
using std::pair;
using std::string;
#include "public.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 



#define Tools_Ansi_GetBit(pMap_uint32,Offset) 		(((((uint32*)(pMap_uint32))[(Offset)/32]) & ((uint32)1<<(uint32)((Offset)%32)))==0?0:1)
#define Tools_Ansi_SetBit(pMap_uint32,Offset)		((((uint32*)(pMap_uint32))[(Offset)/32]) |= ((uint32)1<<(uint32)((Offset)%32)))
#define Tools_Ansi_ClrBit(pMap_uint32,Offset) 		((((uint32*)(pMap_uint32))[(Offset)/32]) &= (~((uint32)1<<(uint32)((Offset)%32))))
#define Tools_Ansi_CMPDATA(p1,p2,Type)				( (*(Type*)(p1)==*(Type*)(p2)) ? 0 : ( (*(Type*)(p1)) > (*(Type*)(p2)) ? 1 : -1))
#define Tools_Ansi_IsDialPadKey(Key) ((Key)>=(int)'0'&&(Key)<=(int)'9'||(Key)==(int)'*'||(Key)==(int)'#'?1:0)

#ifndef Tools_Ansi_ArraySize
  #define Tools_Ansi_ArraySize(arrayvariable) ((int)sizeof(arrayvariable)/(int)sizeof(arrayvariable[0]))
#endif

#ifndef Tools_Ansi_OffsetOfStru
  #ifndef WIN32
    #define Tools_Ansi_OffsetOfStru(stru,membername) offsetof(stru,membername)
  #else
    #define Tools_Ansi_OffsetOfStru(stru,membername) ((int)(&(((stru*)0)->membername)))
  #endif
#endif

#ifndef Tools_Ansi_MemberArraySize
  #ifndef WIN32
    #define Tools_Ansi_MemberArraySize(stru,arraymember) ((int)sizeof(stru::arraymember)/(int)sizeof(stru::arraymember[0]))
  #else
    #define Tools_Ansi_MemberArraySize(stru,arraymember) ((int)sizeof(((stru*)0)->arraymember)/(int)sizeof(((stru*)0)->arraymember[0]))
  #endif
#endif

#ifndef Tools_Ansi_StructMemberSize
  #ifndef WIN32
    #define Tools_Ansi_StructMemberSize(stru,membername) ((int)sizeof(stru::membername))
  #else
    #define Tools_Ansi_StructMemberSize(stru,membername) ((int)sizeof(((stru*)0)->membername))
  #endif
#endif


class CTools_Ansi  
{
public:
	CTools_Ansi();
	virtual ~CTools_Ansi();


public:

	//替换串中的全部子串
	static void ReplaceStr(char *Str,const char *SubStr,const char *NewSubStr);	
	//替换串中的第一个子串
	static void ReplaceStrOnce(char *Str,const char *SubStr,const char *NewSubStr);

	static void DeleteStrRightSpace(char *Str);
	static void DeleteStrRightSpace_10_13(char *Str);
	static void DeleteStrLeftRightSpace(char *Str);
	static void DeleteStrLeftRightSpace_10_13(char *Str);
	static string DeleteStrLeftRightSpace_10_13(const string& Str);
	static void DeleteStrLeftSpace(char *Str);
	static void DeleteStrRightDigit(char *Str);

    //过滤掉双斜杠引导的行注释，并过滤掉首尾的空格、Tab、回车换行符
    static string FilterLineComment(const string& Str);

	//description:look for offset of the next digit bytes, current digit word will be ignore
	static int GetNextDigitOff(const char *pStr,int Len=0,int StartOff=0);	
	//description	:look for offset of the next character bytes, Len can be empty
	static int GetNextCharOff(char Ch,const char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next two character bytes, Len can be empty
	static int GetNextCharOff2(char Ch1,char Ch2,const char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next char that is't space、tab、\r、\n, Len can be empty
	static int GetNextValidOff(const char *pStr,int Len,int StartOff);		
	//description	:look for offset of the next char that is space、tab、\r、\n, Len can be empty
	static int GetNextInvalidOff(const char *pStr,int Len,int StartOff);		
	//description	:向前搜索，找第一个上一个字符是Valid的位置
	static int GetPrevInvalidOff(const char *pStr,int StartOff,int MinOff=0);
	//description	:look for offset of the next char that is special character, or space、tab、\r、\n, Len can be empty
	static int GetNextCharOrInvalidOff(char Ch,const char *pStr,int Len,int StartOff);	

    //description	:look for offset of the next field, seperated by Seperator，Len can be empty
	static int GetNextFieldOff(const char *pStr,int Len,int StartOff,char Seperator1,char Seperator2,char Seperator3,char Seperator4);	

    //取指定位置开始到指定间隔符之前的字符串，过滤当前的的空格、tab和间隔符
	static BOOL GetCurField(const char *pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static BOOL GetCurField_putDest(char *pDest,int MaxDestLen,const char* pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
    //取指定范围内用间隔符间隔出来的区域
    static bool GetCurField(const char *pStr,int StartOff,int EndOff,int& FieldStart,int& FieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	//取指定位置开始到指定间隔符之前的字符串，不过滤当前的的空格、tab和间隔符，返回长度
	static int GetCurField2(const char *pStr,int Len,int StartOff,int& RltFieldStart,int& RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static int GetCurField2_putDest(char *pDest,int MaxDestLen,const char* pStr,int Len,int StartOff,int& RltFieldStart,int& RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);

	static int GetFourField(const char *pStr,int Len,int StartOff,int *RltFieldStart1,int *RltFieldEnd1,int *RltFieldStart2,int *RltFieldEnd2,int *RltFieldStart3,int *RltFieldEnd3,int *RltFieldStart4,int *RltFieldEnd4,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static int GetFourFieldInt(const char *pStr,int Len,int StartOff,int *RltValue1,int *RltValue2,int *RltValue3,int *RltValue4,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
    static int GetFourField(const char *pStr,int StartOff,int EndOff,
        int& FieldStart1,int& FieldEnd1,int& FieldStart2,int& FieldEnd2,int& FieldStart3,int& FieldEnd3,int& FieldStart4,int& FieldEnd4,
		char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);

    //从Str中取出LeadingStr后面直到EndingStr之间的串
    static string GetSubStr(const string& Str,const string& LeadingStr,const string& EndingStr);

    //get content in a string that follow prefix.
    static bool GetContentFollowPrefix(const char* pStr,int StartOff,int EndOff,const char* pPrefix,char* pContent,int ContentSize);
    static bool GetContentFollowPrefix(const char* pStr,int StartOff,int EndOff,const char* pPrefix,string rltContent);

	//description	:get sub string in Yuankuohao
	static BOOL GetSubStrInYuankuohao(const char *pSour,char *pRlt,int RltSize);
	//计算一个串里指定字符的个数
	static int GetCountOfCharInStr(const char* pStr,char Ch);
	//取一个串里指定字符的位置
	static std::vector<int> GetCharPos(const char* pStr,char Ch);
	//将一个串按分隔符，分成多个子串
	static std::vector<std::string> SplitString(const char* pStr,char splitchar1,char splitchar2=0);
	static void SplitString(vector<int>& rltsubpos,vector<int>& rltsublen,const char* pStr,char splitchar1,char splitchar2=0);
	static bool SplitString(int* prltsubpos,int* prltsublen,int& rltsubcount,int maxsubcount,const char* pStr,char splitchar1,char splitchar2=0);
	//切分串。串格式为：str11-str12;str21-str22;str31-str32，其中[;]为splitchar1，[-]为splitchar2；过滤分隔符首尾的空格、tab、回车、换行
	static vector<pair<string,string>> SplitString2(const char* pStr,char splitchar1,char splitchar2);


    //执行WSAStartup
    static void PerformWSAStartup(void);
    //执行WSACleanup
    static void PerformWSACleanup(void);
	//get local host name and local ip, 注意，调用此函数前要调用WSAStartup(). pHostName可以为NULL
	static BOOL GetLocalHostName_IP(char *pHostName,int cbHostName,DWORD *pLocalIP=NULL,int MaxIPCount=1);			

	//function		:GetIPFromHostName
    //               GetIPFromHostName2(取第一个IP)
	//description	:1. get local host name and local ip
    //               2. 可用于将域名解析为IP
	//return		:IP number in result
	static int GetIPFromHostName(const char *pHostName,DWORD *pHostIP,int MaxIPCount);
	static bool GetIPFromHostName2(const char *pHostName,unsigned int& HostIP);
	//将字符串形式的IP地址转换成DWORD
	static DWORD ConvertStrIP(const char *pIP);																			
	//将DWORD形式的IP地址转换成字符串
	static char* ConvertdwIP(DWORD dwIP);
	static char* ConvertdwIP2(DWORD dwIP,char*buf,int bufsize);
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
	static string GetPureFileName(const string& FileNameWithPath);
	//	从全路径文件名中取出盘符，标志是:
	static bool GetDiskSymbol(const char* pFileName,char* pRltDiskSymbol,int RltSize);
	//	从全路径文件名中取出路径，不带盘符
	static bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);
	//	从全路径文件名中取出路径，带盘符
	static bool GetFilePathWithDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);


	static int GetDeltaMS(time_t Endt1,int Endms1,time_t Startt2,int Startms2);
	static void MyTrim(char*pStr);
	static void mytrimleft(std::string& str);
    static void mytrimright(std::string& str);	//删除串右边的空格
    static void mytrimright2(std::string& str);	//删除串右边的空格、tab、\r、\n
	static void mytrimrightdight(std::string& str);//删除串右边的空格、tab、\r、\n和数字
    static void mytrim(std::string& str);
    //获取一个字符串trim后的有效起始位置和有效长度
    static void mygettrimpos(const char* pstr,int&off,int&len);
    static void mymemcpy_trim(char* prlt,int rltsize,const char* psrc);
    //比较两个串，返回1,0,-1
    static int mycompare_trynumeric(const char* str1,const char* str2);
    static int mycompare(const std::string& str1,const std::string& str2);
    static int mycompare2(const char* str1,const char* str2);

	//判断串是否全部是数字
	static bool isdigitstring(const char* str,int len=0);



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
    static void Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime=NULL);
    //  utc转换为hh:mm:ss
    static string Get_strTime_from_utc2(time_t iTime);
    //  utc转换为Sun, 5 Jan 2014 10:22:33 GMT
    static string Get_strTime_from_utc3(time_t iTime);
    //  将utc的time_t时间转换成字符串表示的时间, 格式为yyyy-mm-dd
    static void Get_strDate_from_utc(time_t iTime,char* pRltDate);
	//	将utc的time_t时间的日期转换为整形数, 格式为yyyymmdd，如20140511
	static int Get_intDate_from_utc(time_t iTime);

    //  将utc的time_t时间转换成本地时间
    static void Get_DateTime_from_utc(time_t iTime,int* pRltYear=NULL,int* pRltMonth=NULL,int* pRltDay=NULL,int* pRltHour=NULL,int* pRltMinutes=NULL,int * pRltSecs=NULL,int* pRltWeekDay=NULL);
    //  将本地时间转换成utc时间. 失败返回-1
    static time_t Get_utc_from_DateTime(int year,int month,int day,int hour=0,int minutes=0,int secs=0);

    //  将字符串表示的时间转换成当天00:00:00开始的秒数
    //  pTime:hhmmss 或 hh:mm:ss
    static int Get_TodaySecs_from_strTime(char* pTime);

	//将指定位数的串转换为double
	inline static double myatod(const char*p,int len,char* buf,int bufsize)
	{
		memcpy(buf,p,len<bufsize?len:bufsize-1);
		buf[len<bufsize?len:bufsize-1]=0;
		return atof(buf);
	}
	//将指定位数的串转换为float
	inline static float myatof(const char*p,int len,char* buf,int bufsize)
	{
		memcpy(buf,p,len<bufsize?len:bufsize-1);
		buf[len<bufsize?len:bufsize-1]=0;
		return (float)atof(buf);
	}
	//将指定位数的串转换为int
	inline static int myatoi(const char*p,int len,char* buf,int bufsize)
	{
		memcpy(buf,p,len<bufsize?len:bufsize-1);
		buf[len<bufsize?len:bufsize-1]=0;
		return atoi(buf);
	}
	inline static string myitoa(int val,int radix=10)
	{
		char buf[64];
		itoa(val,buf,radix);
		return string(buf);
	}



	//	将一个数值转换成二进制表示的字符串，bit0在pRlt[BitNum-1]
	static char *GetBinFormatString(char *pRlt,unsigned int Val,unsigned int BitNum);	
	//	将一个数值转换成二进制表示的字符串bit0在pRlt[0]
	static char *GetBinFormatString2(char *pRlt,unsigned int Val,unsigned int BitNum);

    //  将一段内存转换成Hex字符串
    static string GetHexStringFromMem(const void* pMem,int MemLen);
    //  将Hex字符串转成内存。返回结果字节长度
    static int GetMemFromHexString(void* pMem,int MemSize,const string& strHex);
    static int GetMemFromHexString(void* pMem,int MemSize,const char* pszHex,int HexLen);
    //  将double转换成Hex字符串
    static string GetHexStringFromDouble(double Val);
    //  将Hex字符串转成double。返回结果字节长度
    static double GetDoubleFromHexString(const string& strHex);
    static double GetDoubleFromHexString(const char* pszHex,int HexLen);
    //  将整形转换成十进制字符串
    static string GetStringFromInteger(int Val);
    //  十进制字符串转换为整形
    static int GetIntegerFromString(const string& strVal);
    static int GetIntegerFromString(const char* pszVal);
    //  两位十六进制字符串转换为整形
    static unsigned char GetByteFromHexString(const string& strHex);

	//用二分法在一个排序的缓冲区里查找一个字符串
	static int LookupInSequenceBuffer_Mem(char *SeqBuf,int UnitNum,int UnitSize,char *Str,int Len,int *pLastHead=NULL,int *pLastTail=NULL);
	//用二分法在一个排序的缓冲区里查找一个DWORD
	static int LookupInSequenceBuffer_DWORD(char *SeqBuf,int UnitNum,int UnitSize,unsigned int dwID,int *pLastHead=NULL,int *pLastTail=NULL);

    //在LookupInSequenceBuffer()中用到的比较函数
    typedef	int	(*COMPARE_LookupInSequenceBuffer)(const void* pdata1,const void* pdata2);
    //	用二分法在一个排序的缓冲区里查找内容
    //	排序是从小到大
    //  结果放在RltBegin和RltEnd中，表示第一个满足条件的位置和第一个不满足条件的位置。(二者相等，表示没找到)
    static void LookupInSequenceBuffer(int& RltBegin,int& RltEnd,
		const void* pData,int UnitNum,int UnitSize,
		const void* pLookupContent,
		COMPARE_LookupInSequenceBuffer CompareMethod);

	//用冒泡法进行排序。排序是从小到大
	static void SetSequenceInBubble(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);
																
	//	取指定范围内的随机数
	static unsigned int GetRandomValue(unsigned int MinVal,unsigned int MaxVal);

    //  取汉字串里每个汉字的首字母组成的串。汉字必须是GB2312编码
    static string GetFirstAlphasOfChineseStrGBK(const string& GBKStr);

	template <typename KeyType,typename ValueType>
	static bool IsKeyValueInMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey,const ValueType& ivalue)
	{
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
			if(it_lower->second==ivalue)
				return true;
			it_lower++;
		}
		return false;
	};
	template <typename KeyType,typename ValueType>
    static bool GetValueInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey,ValueType& outvalue)
	{
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		if(it_lower!=it_upper)
		{
            outvalue=it_lower->second;
            return true;
		}
        else return false;
	};
	template <typename KeyType,typename ValueType>
    static void GetValuesInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey,vector<ValueType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
            outvalue.push_back(it_lower->second);
			it_lower++;
		}
	};
	template <typename KeyType,typename ValueType>
    static vector<ValueType> GetValuesInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey)
	{
		vector<ValueType> outvalue;
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
            outvalue.push_back(it_lower->second);
			it_lower++;
		}
		return outvalue;
	};
	template <typename KeyType,typename ValueType>
    static void GetValuesInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey,set<ValueType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
            if(outvalue.find(it_lower->second)==outvalue.end())
                outvalue.insert(it_lower->second);
			it_lower++;
		}
	};
	template <typename KeyType,typename ValueType>
    static void GetAllValuesInMultimap(const multimap<KeyType,ValueType>& mmap,set<ValueType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it;
		for(it=mmap.begin();it!=mmap.end();it++)
            if(outvalue.find(it->second)==outvalue.end()) outvalue.insert(it->second);
	};
	template <typename KeyType,typename ValueType>
    static void GetAllValuesInMultimap(const multimap<KeyType,ValueType>& mmap,vector<ValueType>& outvalue)
	{
        typename set<ValueType> setvalues;
        GetAllValuesInMultimap<KeyType,ValueType>(mmap,setvalues);
        outvalue.clear();
        typename set<ValueType>::const_iterator itset;
        for(itset=setvalues.begin();itset!=setvalues.end();itset++)
            outvalue.push_back(*itset);
	};
	template <typename KeyType,typename ValueType>
    static void GetAllKeysInMultimap(const multimap<KeyType,ValueType>& mmap,set<KeyType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it;
		for(it=mmap.begin();it!=mmap.end();it++)
            if(outvalue.find(it->first)==outvalue.end()) outvalue.insert(it->first);
	};
	template <typename KeyType,typename ValueType>
    static void GetAllKeysInMultimap(const multimap<KeyType,ValueType>& mmap,vector<KeyType>& outvalue)
	{
        typename set<ValueType> setvalues;
        GetAllKeysInMultimap<KeyType,ValueType>(mmap,setvalues);
        outvalue.clear();
        typename set<ValueType>::const_iterator itset;
        for(itset=setvalues.begin();itset!=setvalues.end();itset++)
            outvalue.push_back(*itset);
	};

	template <typename KeyType,typename ValueType>
    static void GetKeysInMultimap_MatchValue(const multimap<KeyType,ValueType>& mmap,const ValueType& ivalue,vector<KeyType>& outkeys)
	{
        outkeys.clear();
		for(typename multimap<KeyType,ValueType>::const_iterator it=mmap.begin();it!=mmap.end();it++)
		{
			if(it->second==ivalue)
                outkeys.push_back(it->first);
		}
	};

	template <typename KeyType,typename ValueType>
	static bool EraseKeyValueInMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey,const ValueType& ivalue)
	{
		bool brlt=false;
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
			if(it_lower->second==ivalue)
			{
				it_lower=mmap.erase(it_lower);
				if(!brlt) brlt=true;
			}
			else 
				it_lower++;
		}
		return brlt;
	};

	template <typename KeyType,typename ValueType>
	static bool EraseKeyInMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey)
	{
		bool brlt=false;
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		if(it_lower!=it_upper)
		{
			brlt=true;
			while(it_lower!=it_upper)
				it_lower=mmap.erase(it_lower);
		}
		return brlt;
	};

	template <typename KeyType,typename ValueType>
	static bool EraseValueInMultimap(multimap<KeyType,ValueType>& mmap,const ValueType& ivalue)
	{
		bool brlt=false;
		typename multimap<KeyType,ValueType>::iterator it;
		for(it=mmap.begin();it!=mmap.end();)
		{
			if(it->second==ivalue) 
			{
				it=mmap.erase(it);
				if(!brlt) brlt=true;
			}
			else it++;
		}
		return brlt;
	};
	template <typename KeyType,typename ValueType>
	static void AddKeyValueToMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey,const ValueType& ivalue)
	{
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper&&it_lower->second!=ivalue)
			it_lower++;
        if(it_lower==it_upper)
            mmap.insert(pair<KeyType,ValueType>(ikey,ivalue));
	};

	template <typename DataType>
	static bool IsSameVector(const vector<DataType>& vec1,const vector<DataType>& vec2)
	{
		if(vec1.size()!=vec2.size()) return false;
		for(int i=0;i<(int)vec1.size();i++)
			if(vec1[i]!=vec2[i]) return false;
		return true;
	}
	template <typename DataType>
	static bool IsValueInVector(const vector<DataType>& vec,const DataType& value)
	{
		for(int i=0;i<(int)vec.size();i++)
            if(vec[i]==value) return true;
		return false;
	}

	template <typename KeyType,typename ValueType>
	static bool EraseInMap(map<KeyType,ValueType>& imap,const KeyType& ikey)
    {
		typename map<KeyType,ValueType>::iterator it=imap.find(ikey);
        if(it!=imap.end())
        {
            imap.erase(it);
            return true;
        }
        return false;
    }

	template <typename KeyType,typename ValueType>
	static bool GetValueInMap(const map<KeyType,ValueType>& imap,const KeyType& ikey,ValueType& outValue)
    {
		typename map<KeyType,ValueType>::const_iterator it=imap.find(ikey);
        if(it!=imap.end())
        {
			outValue=it->second;
            return true;
        }
        return false;
    }
	template <typename KeyType,typename ValueType>
	static ValueType GetValueInMap(const map<KeyType,ValueType>& imap,const KeyType& ikey)
    {
		ValueType rlt;
		typename map<KeyType,ValueType>::const_iterator it=imap.find(ikey);
        if(it!=imap.end())
        {
			return it->second;
        }
        return rlt;
    }

	template <typename KeyType,typename ValueType>
	static bool GetKeyInMap(const map<KeyType,ValueType>& imap,const ValueType& iValue,KeyType& outkey)
    {
		typename map<KeyType,ValueType>::const_iterator it;
		for(it=imap.begin();it!=imap.end();it++)
		{
			if(it->second==iValue)
			{
				outkey=it->first;
				return true;
			}
		}
		return false;
    }

	template <typename KeyType,typename ValueType>
	static vector<ValueType> GetValuesInMap(const map<KeyType,ValueType>& imap)
    {
		vector<ValueType> outValues;
		typename map<KeyType,ValueType>::const_iterator it;
		for(it=imap.begin();it!=imap.end();it++)
			outValues.push_back(it->second);
        return outValues;
    }

	template <typename DataType>
	static void ConvertVector2Set(const vector<DataType>& sourvec,set<DataType>& rltset)
	{
		rltset.clear();
		for(vector<DataType>::const_iterator it=sourvec.begin();it!=sourvec.end();it++)
		{
			if(rltset.find(*it)==rltset.end())
				rltset.insert(*it);
		}
	}

	template <typename DataType>
	static void ConvertSet2Vector(const set<DataType>& sourset,vector<DataType>& rltvec)
	{
        rltvec.clear();
		for(set<DataType>::const_iterator it=sourset.begin();it!=sourset.end();it++)
            rltvec.push_back(*it);
	}

    template <typename DataType>
	static bool EraseInSet(set<DataType>& sset,const DataType& value)
	{
		set<DataType>::iterator it=sset.find(value);
		if(it==sset.end()) return false;
		sset.erase(it);
		return true;
	}

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_)
