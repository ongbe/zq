#pragma once
#include "afxcmn.h"

#include <string>

class CSortListCtrl : public CListCtrl
{
public:
	CSortListCtrl(void);
public:
	~CSortListCtrl(void);

public:
	enum DATA_TYPE{INT_TYPE = 0, STRING_TYPE, DOUBLE_TYPE };
public:
	//是否为升序
	bool m_bAsc;
	//当前排列的序
	int m_nSortedCol;
//public:
//	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
/*	std::string ws2s(const std::string& ws)
	{
		std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
		setlocale(LC_ALL, "chs");
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest,0,_Dsize);
		wcstombs(_Dest,_Source,_Dsize);
		std::string result = _Dest;
		delete []_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}*/

public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
