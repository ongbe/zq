#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
class LanguageMgr
{
public:
	LanguageMgr(void);
	~LanguageMgr(void);

	bool LoadConfig();
	std::string GetEN(std::string id);
	std::string GetCN(std::string id);
	std::string GetHK(std::string id);
	void ClearCfg();
private:
	string& Replace(string& str);
    string& Trim(string& buff );
	void ReadConfig(char* buffer);
	bool string_getline(char* pData,char* out,int len);
	int split(string line,vector<string>& field);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);
	char* ChangeLanguageCode( const char* szGBString );
	wchar_t* GBKToUnicode( const char* szGBString );
	char* UnicodeToBIG5( const wchar_t* szUnicodeString );
private:
	int m_offset;
	std::map<std::string, std::string> m_ENMap;
	std::map<std::string, std::string> m_CNMap;
	std::map<std::string, std::string> m_HKMap;
};
