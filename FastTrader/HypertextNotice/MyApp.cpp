#include "StdAfx.h"
#include "MyApp.h"
#include "TradingNoticeDlg.h"
IMPLEMENT_APP(CMyApp)
CMyApp::CMyApp(void):
m_DestHWnd(NULL)
{
}

CMyApp::~CMyApp(void)
{
}
bool CMyApp::OnInit()
{
	if(argc==2)
	{
		m_DestHWnd=(HWND)atoi(argv[1]);
		if(m_DestHWnd&&::IsWindow(m_DestHWnd))
		{
			//显示主窗口
			CTradingNoticeDlg* dlg=new CTradingNoticeDlg(NULL);	
			dlg->Show();
			//修改属性
			HWND hThis=(HWND)dlg->GetHWND();
			::SetWindowLong(hThis,GWL_HWNDPARENT,(long)m_DestHWnd);
			//发送握手消息
			COPYDATASTRUCT data;
			ZeroMemory( &data, sizeof(COPYDATASTRUCT));
			data.dwData=IE_MSG_LOGIN;//握手
			data.cbData =sizeof(HWND);
			data.lpData =(void*)&hThis;//句柄
			::SendMessage(m_DestHWnd, WM_COPYDATA,IE_MAGIC_NUMBER, (LPARAM)&data);
			return true;
		}
	}
	return false;
}
int CMyApp::OnExit()
{
	//发送握手消息
	COPYDATASTRUCT data;
	ZeroMemory( &data, sizeof(COPYDATASTRUCT));
	data.dwData=IE_MSG_EXIT;//退出
	::SendMessage(m_DestHWnd, WM_COPYDATA,IE_MAGIC_NUMBER, (LPARAM)&data);
	return wxApp::OnExit();
}