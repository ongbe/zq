// OptionPriceListT.h : OptionPriceListT 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// COptionPriceListTApp:
// 有关此类的实现，请参阅 OptionPriceListT.cpp
//

class COptionPriceListTApp : public CWinApp
{
public:
	COptionPriceListTApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern COptionPriceListTApp theApp;