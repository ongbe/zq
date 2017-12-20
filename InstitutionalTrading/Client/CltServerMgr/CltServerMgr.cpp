// CltServerMgr.cpp : main project file.

#include "stdafx.h"
#include "FormMain.h"
#include "TcpLayer.h"
#include "LoginForm.h"
#include "ConfigMgr.h"

#pragma unmanaged

#ifdef _DEBUG
	#pragma comment(lib, "Tools4clrD.lib")
#else
	#pragma comment(lib, "Tools4clr.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")

#pragma managed

using namespace CltServerMgr;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	//连接服务器
	CConfigManager::Load();
	CTcpLayer::Init();
	
	//弹出登录对话框
	LoginForm^ loginForm = gcnew LoginForm();
	if( loginForm->ShowDialog() != ::DialogResult::OK)
	{
		return 0;
	}

    _GetPopMsg()->Hide();
	// Create the main window and run it
	Application::Run(gcnew FormMain());
    _ReleasePopMsg();

    LOG_INFO("Enter CTcpLayer::Stop()");
    CTcpLayer::Stop();
    LOG_INFO("Enter CTcpLayer::Release()");
    CTcpLayer::Release();
    LOG_INFO("main Quit Finished");

	return 0;
}
