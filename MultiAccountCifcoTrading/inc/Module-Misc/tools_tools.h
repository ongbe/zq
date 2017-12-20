// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TOOLS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TOOLS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

//#include "toolpublic.h"

// This class is exported from the tools.dll
class CTools 
{
public:
	CTools(void);

	//从全路径文件名中取出单纯的文件名
	static bool GetPureFileName(char* pFileNameWithPath,char* pRltFileName,int RltSize);

	//从全路径文件名中取出盘符，标志是:
	static bool GetDiskSymbol(char* pFileName,char* pRltDiskSymbol,int RltSize);

	//从全路径文件名种取出路径，不带盘符
	static bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);

	//从全路径文件名种取出路径，带盘符
	static bool GetFilePath(char* pFileName,char* pRltFilePath,int RltSize);

	//删除字符串头尾的空格和Tab和\n和\r
	static void Trim_Space(char* pString);

	//删除字符串中两个斜杠后面的注释内容
	static void Trim_Note(char* pString);

    //从Name=Value形式的字符串中取出Name和Value, 如果Value由双引号括起来的话，删除双引号
	static bool GetName_ValueFromString(char* pString,char* pRltName,char* pRltValue);

	//判断一个时间是否在指定的时间段内。时间用hhmm的整形表示
	static bool IsInTimePeriod(int iMins,int StartMins,int EndMins);

	//取当前的秒数和毫秒数
	static void GetTime(time_t *pSec,int *pMS);

	//	用二分法在一个排序的缓冲区里查找一个字符串
	//	如找到该字符串，返回其在缓冲区里的序号
	//	如找不到，返回-1，pLastHead和pLastTail是包含指定字符串的上下限值
	//	排序是从小到大
	//	大小写无关
	static int LookupStringInSequenceBuffer_memicmp(char *SeqBuf,int UnitNum,int UnitSize,char *pMem,int Len,int *pLastHead=NULL,int *pLastTail=NULL);

	//	用二分法在一个排序的缓冲区里查找一个字符串
	//	如找到该字符串，返回其在缓冲区里的序号
	//	如找不到，返回-1，pLastHead和pLastTail是包含指定字符串的上下限值
	//	排序是从小到大
	//	大小写相关
	static int LookupStringInSequenceBuffer_memcmp(char *SeqBuf,int UnitNum,int UnitSize,char *pMem,int Len,int *pLastHead=NULL,int *pLastTail=NULL);

	//	用冒泡法进行排序。排序是从小到大
	//	Buf是要排序的缓冲区，缓冲区由UnitNum个单元组成，每个单元的长度为UnitSize
	//	CmpOff为要比较的串在单元里的位置，CmpLen为比较的长度
	//	大小写无关
	static void SetSequenceInBubble_memicmp(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);

	//	用冒泡法进行排序。排序是从小到大
	//	Buf是要排序的缓冲区，缓冲区由UnitNum个单元组成，每个单元的长度为UnitSize
	//	CmpOff为要比较的串在单元里的位置，CmpLen为比较的长度
	//	大小写相关
	static void SetSequenceInBubble_memcmp(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);

	//	按4字节对齐的方式取一段内存的DWORD算术和
	static DWORD GetCRC_dw(DWORD* pdw,int dwCount);

    //  将字符串表示的时间转换成utc的time_t时间
    //  pDate:yyyymmdd
    //  pTime:hhmmss 或 hh:mm:ss
    //static time_t Get_time_t_utc_from_strTime(const char* pDate, const char* pTime);

	/// 将时间字符串转为秒数
	/// pTime:hhmmss 或 hh:mm:ss
	static unsigned int CalculateTotalSeconds(const char* pTime);

    //  将utc的time_t时间转换成字符串表示的时间
    //  pRltDate:yyyymmdd
    //  pRltTime:hh:mm:ss
    static void Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime);

    //  将utc的time_t时间转换成本地时间
    static void Get_DateTime_from_utc(time_t iTime,int* pRltYear=NULL,int* pRltMonth=NULL,int* pRltDay=NULL,int* pRltHour=NULL,int* pRltMinutes=NULL,int * pRltSecs=NULL);


    //  将字符串表示的时间转换成当天00:00:00开始的秒数
    //  pTime:hhmmss 或 hh:mm:ss
    static int Get_TodaySecs_from_strTime(char* pTime);


    //  将整数添加到串尾, 返回添加的长度
    static int AppendIntToString(int iVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  将double添加到串尾, 返回添加的长度
    static int AppendDoubleToString(double dVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  将char添加到串尾, 返回添加的长度
    static int AppendCharToString(char cVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  将string添加到串尾, 返回添加的长度
    static int AppendStringToString(char *pSourStr,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  在string的前面添加空格，是string的长度为指定长度
    static void AppendSpaceToString(char *pStr,int FinalLen);

	//  在string的前面添加'0'，使string的长度为指定长度
	static void AppendZeroToString(char *pStr,int FinalLen);

    //  将ip:port或ip port格式的地址串分隔为单独的IP串和Port串
    static bool SplitIPAddrStr(const char* pAddr,char* pRltIP,int RltIPSize,char* pRltPort,int RltPortSize);

    //  比较内存块，忽略后面的空格
    static int MyMemcmpIgnoreTailSpace(const char* pMem1,const char* pMem2,int MemLen,int* pRealCmpLen=NULL);

    static void mytrimleft(std::string& str);
    static void mytrimright(std::string& str);
    static void mytrim(std::string& str);

    //获取一个字符串trim后的有效起始位置和有效长度
    static void mygettrimpos(const char* pstr,int&off,int&len);

    static void mymemcpy_trim(char* prlt,int rltsize,const char* psrc);

    //比较两个串，返回1,0,-1
    static int mycompare_trynumeric(const wxString& str1,const wxString& str2);
    static int mycompare2_trynumeric(const char* str1,const char* str2);
    static int mycompare(const std::string& str1,const std::string& str2);
    static int mycompare2(const wxString& str1,const wxString& str2);

public:
	CTools(const CTools&);									
	CTools& operator= (const CTools&);				

};

