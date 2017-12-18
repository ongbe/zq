// updateexe.cpp : Defines the entry point for the application.
//  执行FastTrader的在线升级
//  程序逻辑参考readme.txt
//

#include "stdafx.h"
#include "updateexe.h"
#include "doupdate.h"
#include "doupdate2.h"
#include <string>
using std::string;



Cdoupdate2* g_pdoupdate2=NULL;
unsigned int g_idMainThread=0;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    g_idMainThread=GetCurrentThreadId();

    g_pdoupdate2=new Cdoupdate2();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    if(g_pdoupdate2) delete g_pdoupdate2;

	return (int) msg.wParam;
}

