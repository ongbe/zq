#pragma once

class CMyApp :	public wxApp
{
public:
	CMyApp(void);
	~CMyApp(void);
public:
	virtual bool OnInit();
	virtual int OnExit();
private:
	HWND m_DestHWnd;
};
DECLARE_APP(CMyApp)