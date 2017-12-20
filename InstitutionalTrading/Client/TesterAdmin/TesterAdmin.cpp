// TesterAdmin.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "TcpServer.h"
#ifdef _DEBUG
#pragma comment(lib, "Tools4clrD.lib")
#else
#pragma comment(lib, "Tools4clr.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
using namespace TesterAdmin;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	//创建server服务器
	CTcpServer lServer;
	lServer.CreateSvr();

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());

	lServer.CloseServer();
	return 0;
}
