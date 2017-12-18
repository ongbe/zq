#include "StdAfx.h"
#include "MyApp.h"
//#include "MyFrame.h"
//#include "TestDlg.h"

LPTOP_LEVEL_EXCEPTION_FILTER g_pFilterQuation=NULL;
IMPLEMENT_APP(MyApp)
MyApp::MyApp(void)
{
}

MyApp::~MyApp(void)
{
}
bool MyApp::OnInit()
{
	g_pFilterQuation=::SetUnhandledExceptionFilter(TopExceptionCallback);

	if(__argc != 2)
    {
        Release();
		return false;
    }

	wxString lpCmdLine = __argv[1];	
	long lHwnd = atol(lpCmdLine.c_str());
	if(lHwnd == 0||(!IsWindow((HWND)lHwnd)))
    {
        Release();
		return false;
    }

	WXHWND hWnd = (WXHWND)lHwnd;

	MyFrame* frame = new MyFrame(hWnd);
	frame->Show(true);
	
	// 初始化BNCM服务
/*	m_poTCPDataService = CTCPDataService::GetOnlyOneInstance();
	if(m_poTCPDataService==NULL) 
	{
		wxMessageBox(wxT("系统初始化错误，无法创建TCP相关应用"), wxT("提示"), MB_OK|MB_ICONSTOP);
		return FALSE;
	}*/
	return true;
}
void MyApp::Release()
{
	::SetUnhandledExceptionFilter(g_pFilterQuation);
//	CTCPDataService::ReleaseOnlyOneInstance();
}
int MyApp::OnExit()
{
	Release();
	return wxApp::OnExit();
}
//time_t MyApp::stingToTime(char * szTime)
//{
//	struct tm tm1;
//	time_t time1;
//	sscanf(szTime,"%4d-%2d-%2d %2d:%2d:%2d",
//		&tm1.tm_year,
//		&tm1.tm_mon,
//		&tm1.tm_mday,
//		&tm1.tm_hour,
//		&tm1.tm_min,
//		&tm1.tm_sec);             
//	tm1.tm_year -= 1900;
//	tm1.tm_mon --;
//	tm1.tm_isdst=-1;  
//	time1 = mktime(&tm1);
//	return time1;
//}
/*
void MyApp::split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}

*/