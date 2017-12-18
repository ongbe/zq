// Client.cpp : main project file.

#include "stdafx.h"
#include "ConfigMgr.h"
#include "TcpLayer.h"
#include "FormMain.h"

using namespace Client;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	//加载配置参数
	CConfigManager::Load();

	//初始化通信层
	CTcpLayer::Init();

	// Create the main window and run it
	FormMain^ form = gcnew FormMain();

	if(args->Length == 2)
	{
		String^ strName = args[0];
		String^ strPwd  = args[1];
		form->SetAutoStartPara(true, strName, strPwd);
	}
	Application::Run(form);

	//释放通信层
	CTcpLayer::Release();

	return 0;
}
