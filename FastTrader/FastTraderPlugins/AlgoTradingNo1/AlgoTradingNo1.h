// AlgoTradingNo1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAlgoTradingNo1App:
// �йش����ʵ�֣������ AlgoTradingNo1.cpp
//

class CAlgoTradingNo1App : public CWinApp
{
public:
	CAlgoTradingNo1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAlgoTradingNo1App theApp;
extern HWND g_hWnd_FT;