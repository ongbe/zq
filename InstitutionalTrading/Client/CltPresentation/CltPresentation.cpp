// CltPresentation.cpp : main project file.

#include "stdafx.h"
#include "FormMain.h"
#include "LoginForm.h"
#include "ReportForm.h"
#include "ConfigMgr.h"
#ifdef _DEBUG
#pragma comment(lib, "Tools4clrD.lib")
#else
#pragma comment(lib, "Tools4clr.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")

using namespace CltPresentation;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	//Application::Run(gcnew FormMain());

	//连接服务器
	CConfigManager::Load();
	CTcpLayer::Init();

	//弹出登录对话框
	LoginForm^ loginForm = gcnew LoginForm();
	if( loginForm->ShowDialog() != ::DialogResult::OK)
	{
		return 0;
	}

	// Create the main window and run it
	Application::Run(gcnew FormMain());

	CTcpLayer::Stop();
	CTcpLayer::Release();
	_ReleasePopMsg();
	return 0;
}
