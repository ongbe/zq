// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__FC455F2F_486A_4A75_861C_E25A1262AC67__INCLUDED_)
#define AFX_INIFILE_H__FC455F2F_486A_4A75_861C_E25A1262AC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Tools_Ansi.h"
#include "Tools_Win32.h"
#include <sys/types.h>										//must include TYPES.H before STAT.H
#include <sys/stat.h>
#include <vector>

class CIniFile  
{
public:
	CIniFile(const char *pIniFileName=NULL,int iUseImgID=0);
	virtual ~CIniFile();


	//取带路径的ini文件名，结果放在RltIniFileName里，尺寸必须够大。搜索时首先在当前目录，然后在下级的config目录，最后在上级的config目录。返回ini文件所在的路径，如果找不到，RltIniFileName为空
	void GetIniFileName(char *RltIniFileName,char *RltIniFilePath,const char *pMainPath,const char *pIniFileName);
	//Tell you whether a ini file is exist
	bool IsIniFileExist(const char *filename);


	//取指定的key_val对的数量
	int GetKeyValCount(const char* title,const char* key,int TitleOrd=0);
	//get configure data in string value from ini file
	void GetIniKeyStr(const char *title,const char *key,const char *defstr,char *RltBuf,int RltSize,int SearchOrd=0,int TitleOrd=0);
	//get configure data in integer value from ini file 
	int GetIniKeyVal(const char *title,const char *key,const char *defstr,int SearchOrd=0,int TitleOrd=0); 
	int GetIniKeyVal(const char *title,const char *key,int defval,int SearchOrd=0,int TitleOrd=0);
	double GetIniKeyVal2(const char *title,const char *key,double defval,int SearchOrd=0,int TitleOrd=0);
	std::vector<double> GetIniKeyVal3(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);
	std::vector<int> GetIniKeyVal4(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);
	char GetIniKeyChar(const char *title,const char *key,char defchar,int SearchOrd=0,int TitleOrd=0);

	//get configurre data in IP value from ini file
	DWORD GetIniKeyIP(const char *title,const char *key,const char *defstr=NULL,int SearchOrd=0,int TitleOrd=0);
	int GetIniKey_time_t(const char *title,const char *key,char *deftm,int SearchOrd=0,int TitleOrd=0);
	unsigned int GetIniKeyColor(const char *title,const char *key,unsigned int defcolor,int SearchOrd=0,int TitleOrd=0);

	bool GetKey_Val(const char *title,char *RltKey,int RltKeySize,char *RltBuf,int RltBufSize,int SearchOrd,int TitleOrd=0);//找指定title的下一个key

	//Tell you whether a rootnode is in the inifile
	bool HasIniRootNode(const char *title,int TitleOrd=0);
	//Tell you whether a rootnode/key is in the inifile, rtn 1:rootnode/key is in the inifile; 0:isn't in
	bool HasIniKey(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);
	//tell you whether the inifile was update 
	bool IsIniFileUpdate(void);
	//Load inifile content to img
	void LoadIniFileImg(void);


	//取一节的首尾位置。title:节名；pNodeStartPos，pNodeEndPos：接收节的首尾位置的指针
	bool GetNodePos(const char *title,int *pNodeStartPos,int *pNodeEndPos,int TitleOrd=0);		
	//取指定key行的首尾位置；title:节名；key:键名；pKeyStartPos，pKeyEndPos：接收键的首尾位置的指针
	bool GetKeyPos(const char *title,const char *key,int *pKeyStartPos,int *pKeyEndPos,int SearchOrd=0,int TitleOrd=0);


	//删除指定名称的节
	bool DeleteNode(const char *title,int TitleOrd=0);											
	//删除指定名称的key
	bool DeleteKey(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);							
	bool AddKeyStr(const char *title,const char *key,char *keystr,int TitleOrd=0);
	bool AddKeydwordVal(const char *title,const char *key,unsigned intkeyval,int TitleOrd=0);
	bool AddKeyintVal(const char *title,const char *key,int keyval,int TitleOrd=0);
	//改变一个键值的内容
	bool UpdateKeyStr(const char *title,const char *key,const char *keystr,int SearchOrd=0,int TitleOrd=0);			
	//改变一个键值的内容
	bool UpdateKeyVal(const char *title,const char *key,int val,int SearchOrd=0,int TitleOrd=0);				
	bool UpdateKeyVal2(const char *title,const char *key,double val,int SearchOrd=0,int TitleOrd=0);				
	bool UpdateKeyVal3(const char *title,const char *key,std::vector<double>& vals,int SearchOrd=0,int TitleOrd=0);				
	bool UpdateKeyVal4(const char *title,const char *key,std::vector<int>& vals,int SearchOrd=0,int TitleOrd=0);	
    bool UpdateKeyVal5(const char *title,const char *key,std::vector<std::string>& vals,int SearchOrd=0,int TitleOrd=0);	
	bool UpdatedwordKeyVal(const char *title,const char *key,unsigned int val,int SearchOrd=0,int TitleOrd=0);				
	bool AddOneNode(const char *pNodeBuf);


	//取含有指定Key值的Title的序号，用于有多个同名Title的ini,return TitleOrd,-1为找不到
	int GetTitleOrdByKey_Str(const char *title,const char *key,const char *pKeystr);
	int GetTitleOrdByVal(const char *title,const char *key,const int KeyVal);

	char			m_IniFileName[256];
	int				m_UseImgID;

private:
	struct _stat	m_IniFileStat;

	char			*m_pIniFileImg;
	int				m_IniFileLen;

	//同步对象
	char			*m_pSem;												


	//从文件中取出一行内容，如果m_UseImgID,则从文件映像中取
	bool GetLineContent_Internal(char* pRltBuf,int RltSize,char* fp,int&FileOff);
	//过滤ini内容，主要把注释内容过滤掉，如果在双引号中，则从双引号中取出字符串，最后过滤头尾的空格
	void FilterContent_Internal(const char* pBuf,int BufLen,int& StartPos,int& EndPos);

	bool AddKeyStr_Internal(const char *title,const char *key,const char *keystr,int TitleOrd);
	void LoadIniFileImg_Internal(void);
	//function		:GetTitleOrdByKey
	//description	:1. 取含有指定Key值的Title的序号，用于有多个同名Title的ini
	//				 2. pKeystr可以为空
	//return		:TitleOrd,-1为找不到
	int GetTitleOrdByKey_Str_Internal(const char *title,const char *key,const char *pKeystr);
	void GetIniKeyStr_Internal(const char *title,const char *key,const char *defstr,char *RltBuf,int RltSize,int SearchOrd,int TitleOrd);
	bool GetNodePos_Internal(const char *title,int *pNodeStartPos,int *pNodeEndPos,int TitleOrd);
	bool GetKeyPos_Internal(const char *title,const char *key,int *pKeyStartPos,int *pKeyEndPos,int SearchOrd,int TitleOrd);
	bool DeleteKey_Internal(const char *title,const char *key,int SearchOrd,int TitleOrd);
	bool UpdateKeyStr_Internal(const char *title,const char *key,const char *keystr,int SearchOrd,int TitleOrd);

};

#endif // !defined(AFX_INIFILE_H__FC455F2F_486A_4A75_861C_E25A1262AC67__INCLUDED_)
