#include "StdAfx.h"
#include <fstream>
#include <algorithm>
#include "Module-Misc2/GlobalFunc.h"
#include "LanguageMgr.h"
#define	L_TRADITIONAL	(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL))
#define	L_SIMPLIFIED	(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED))
#define	L_HONGKONG		(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_HONGKONG))
#define	L_SINGAPORE		(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SINGAPORE))
#define	L_MACAU			(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_MACAU))
LanguageMgr::LanguageMgr(void)
{
	m_ENMap.clear();
	m_CNMap.clear();
	m_HKMap.clear();
	m_offset=0;
}

LanguageMgr::~LanguageMgr(void)
{
}

bool LanguageMgr::LoadConfig()
{
	std::string strLanguageFile = "language.csv";
	GlobalFunc::GetPath( strLanguageFile );
	char tempdir[MAX_PATH],tempfilename[MAX_PATH];
	::GetTempPath(MAX_PATH,tempdir);
	::GetTempFileName(tempdir,"~lc",0,tempfilename);
	if(::CopyFile(strLanguageFile.c_str(),tempfilename,false))
	{
		HANDLE hHandle=CreateFile(tempfilename,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
		if(hHandle!=INVALID_HANDLE_VALUE)
		{
			DWORD filesize=GetFileSize(hHandle,NULL); 
			char* buffer=new char[filesize+1]; 
			DWORD readsize;
			ReadFile(hHandle,buffer,filesize,&readsize,NULL);
			buffer[filesize]=0; 
			LCID lcid=GetSystemDefaultLCID();
			if(lcid==L_TRADITIONAL||lcid==L_HONGKONG||lcid==L_MACAU)//繁体中文
			{
				char* Langbuffer=ChangeLanguageCode(buffer);
				ReadConfig(Langbuffer);
				delete[] buffer;
				delete[] Langbuffer;
			}
			else//简体中文或者非中文
			{
				ReadConfig(buffer);
				delete[] buffer;
			}
			CloseHandle(hHandle);
			return true;
		}
	}
	return false;

}
//不是空白就是return true
bool IsNotSpace( const char c )
{
	if(c<0||c>128)
		return true;
	return !isspace( c );
}

std::string& LanguageMgr::Trim( std::string& buff )
{
	string::iterator itr = find_if(buff.begin(),buff.end(),IsNotSpace); 
	buff.erase( buff.begin(),itr );
	string::reverse_iterator ritr = find_if(buff.rbegin(),buff.rend(),IsNotSpace);  
	buff.erase( buff.length()-(ritr - buff.rbegin()) ); 
	return buff;
}
string& LanguageMgr::Replace(string& str)
{
	string::size_type pos=string::npos; 
	pos=str.rfind("\\n",pos); 
	while(pos!=string::npos)
	{
		str.replace(pos,2,"\n");
		pos=str.rfind("\\n",pos); 
	}
	return str;
}

void LanguageMgr::ReadConfig(char* buffer)
{
	char szBuf[4096];
	vector<string> field;
	std::string id;
	while (string_getline(buffer, szBuf,4096) )
	{
		field.clear();
		split(szBuf,field);
		if(field.size()>0) id=Trim(field[0]);
		if(field.size()>1)
		{
			//if(m_CNMap.find(id)!=m_CNMap.end())
			//{
			//	TRACE("LanguageMgr::ReadConfigCN(%s)\n",id.c_str());
			//}
			m_CNMap[id]=Replace(field[1]);
		}
		if(field.size()>2)
		{
			//if(m_ENMap.find(id)!=m_ENMap.end())
			//{
			//	TRACE("LanguageMgr::ReadConfigEN(%s)\n",id.c_str());
			//}
			m_ENMap[id]=Replace(field[2]);
		}
		if(field.size()>3)
		{
			//if(m_HKMap.find(id)!=m_HKMap.end())
			//{
			//	TRACE("LanguageMgr::ReadConfigHK(%s)\n",id.c_str());
			//}
			m_HKMap[id]=Replace(field[3]);
		}
	}
}
bool LanguageMgr::string_getline(char* pData,char* out,int len)
{
    if(m_offset>=(int)strlen(pData))
		return false;
	std::string src(pData+m_offset);
	int nPos = src.find("\r\n");
    if(nPos==-1) nPos=(int)strlen(pData)-m_offset;
	if(nPos>len) nPos=len;
	memset(out,0,len);
	memcpy(out,pData+m_offset,nPos);
	m_offset+=nPos+2;
	return true;
}
// split: split line into fields
int LanguageMgr::split(string line,vector<string>& field)
{
	string fld;
	int i, j;

	int nfield = 0;
	if (line.length() == 0)
		return 0;
	i = 0;

	do {
		if (i < (int)line.length() && line[i] == '"')
			j = advquoted(line, fld, ++i);    // skip quote
		else
			j = advplain(line, fld, i);
		if (nfield >= (int)field.size())
			field.push_back(fld);
		else
			field[nfield] = fld;
		nfield++;
		i = j + 1;
	} while (j < (int)line.length());

	return nfield;
}

// advquoted: quoted field; return index of next separator
int LanguageMgr::advquoted(const string& s, string& fld, int i)
{
	int j;
	fld = "";
	for (j = i; j < (int)s.length(); j++)
	{
		if (s[j] == '"' && s[++j] != '"')
		{
			int k = s.find_first_of(",", j);
			if (k <0)    // no separator found
				k = s.length();
			for (k -= j; k-- > 0; )
				fld += s[j++];
			break;
		}
		fld += s[j];
	}
	return j;
}

// advplain: unquoted field; return index of next separator
int LanguageMgr::advplain(const string& s, string& fld, int i)
{
	int j;
	j = s.find_first_of(",", i); // look for separator
	if (j <0)               // none found
		j = s.length();
	fld = string(s, i, j-i);
	return j;
}
std::string LanguageMgr::GetEN(std::string id)
{
    std::map<std::string,std::string>::iterator it;
    it=m_ENMap.find(Trim(id));
    if(it!=m_ENMap.end()) return it->second;
    else return id;
}
std::string LanguageMgr::GetCN(std::string id)
{
    std::map<std::string,std::string>::iterator it;
    it=m_CNMap.find(Trim(id));
    if(it!=m_CNMap.end()) return it->second;
    else return id;
}
std::string LanguageMgr::GetHK(std::string id)
{
    std::map<std::string,std::string>::iterator it;
    it=m_HKMap.find(Trim(id));
    if(it!=m_HKMap.end()) return it->second;
    else return id;
}

char* LanguageMgr::ChangeLanguageCode( const char* szGBString )
{
	LCID lcid = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);
	int nLength = LCMapString(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,NULL,0);
	char* pBuffer=new char[nLength+1];
	LCMapString(lcid,LCMAP_TRADITIONAL_CHINESE,szGBString,-1,pBuffer,nLength);
	pBuffer[nLength]=0;
	wchar_t* pUnicodeBuff = GBKToUnicode(pBuffer);
	char* pBIG5Buff = UnicodeToBIG5(pUnicodeBuff);
	delete[] pBuffer;
	delete[] pUnicodeBuff;
	return pBIG5Buff;
}

wchar_t* LanguageMgr::GBKToUnicode( const char* szGBString )
{
	UINT nCodePage = 936; //GB2312
	int nLength=MultiByteToWideChar(nCodePage,0,szGBString,-1,NULL,0);
	wchar_t* pBuffer = new wchar_t[nLength+1];
	MultiByteToWideChar(nCodePage,0,szGBString,-1,pBuffer,nLength);
	pBuffer[nLength]=0;
	return pBuffer;
}

char* LanguageMgr::UnicodeToBIG5( const wchar_t* szUnicodeString )
{
	UINT nCodePage = 950; //BIG5
	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);
	char* pBuffer=new char[nLength+1];
	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);
	pBuffer[nLength]=0;
	return pBuffer;
}

void LanguageMgr::ClearCfg()
{
	m_CNMap.clear();
	m_ENMap.clear();
	m_HKMap.clear();
	m_offset = 0;
}
