// TraderTest.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "TcpLayer.h"
#include "ConfigMgr.h"
#include "LoginForm.h"



#ifdef _DEBUG
#pragma comment(lib, "Tools4clrD.lib")
#else
#pragma comment(lib, "Tools4clr.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"PlatformDataMgr4dllD.lib")
#pragma comment(lib, "Module-Misc24dllD.lib") 
#pragma managed

using namespace TraderTest;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 
 //   //连接服务器
	CConfigManager::Load();
	CTcpLayer::Init();

	LoginForm^ loginForm = gcnew LoginForm();
	if( loginForm->ShowDialog() != ::DialogResult::OK)
	{
		return 0;
	}         
	// Create the main window and run it
	Application::Run(gcnew Form1());

	
	CTcpLayer::Stop();
	CTcpLayer::Release();
	return 0;
}
