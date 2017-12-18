#pragma once
#include "stdafx.h"
//#include "../../inc/Quotation/TLine.h"
class MyApp :
	public wxApp
{
public:
	MyApp(void);
	~MyApp(void);
	virtual bool OnInit();

	virtual int OnExit();
	void Release();

//	void ReadFile(wxString strFileName, STLine *& pTLine, int& nRetCount);
//	void split(const string& s, char c, vector<string>& v);
//	time_t stingToTime(char * szTime);
protected:
	CTCPDataService*	m_poTCPDataService;
};
DECLARE_APP(MyApp)