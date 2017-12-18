// AlgoTradingNo1.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CAlgoTradingNo1App:
// 有关此类的实现，请参阅 AlgoTradingNo1.cpp
//

class CAlgoTradingNo1App : public CWinApp
{
public:
	CAlgoTradingNo1App();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAlgoTradingNo1App theApp;
extern HWND g_hWnd_FT;
