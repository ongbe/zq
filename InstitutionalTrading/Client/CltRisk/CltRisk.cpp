// CltRisk.cpp : main project file.

#include "stdafx.h"
#include "ConfigMgr.h"
#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "FormMain.h"
#include "FormLogin.h"


//#pragma unmanaged

#ifdef _DEBUG
	#pragma comment(lib, "Tools4clrD.lib")
#else
	#pragma comment(lib, "Tools4clr.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")

#pragma managed


using namespace CltRisk;
using namespace System::Text;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
    System::Windows::Forms::Application::EnableVisualStyles();
    System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false); 

    //加载服务器地址
    CConfigManager::Load();
    //连接交易服务器
    CTcpLayer::Init();    

    //弹出登录对话框
    FormLogin^ loginForm = gcnew FormLogin();
    if( loginForm->ShowDialog() != ::DialogResult::OK)
    {
        return 0;
    }
		
    _GetPopMsg()->Hide();
    System::Windows::Forms::Application::Run(gcnew FormMain());
    _ReleasePopMsg();

    CTcpLayer::Stop();
    CTcpLayer::Release();
    CTcpDataConnect::Stop();
    CTcpDataConnect::Release();

	return 0;
}
