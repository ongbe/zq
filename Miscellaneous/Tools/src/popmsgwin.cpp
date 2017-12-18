//********************************************************************************************
//	
//	FileName	:popmsg.cpp
//	Target		:定义弹出消息窗口类
//	Description	:
//				 1. 此类创建一个线程，线程中创建一个顶层edit窗口，应用程序可以向该窗口发送调试信息，显示在窗口里
//				 2. 此类仅应用于Win32环境下
//	Version		:1.0
//	Date		:2008-4-30
//	Author		:lzq
//				 Copyright (C), 2008-2028
//	
//	History		:
//
//********************************************************************************************

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#ifdef WIN32
#pragma unmanaged
#endif


#include "popmsgwin.h"
#include "FileOpr.h"

#define ID_EditWnd	10001
#define ID_OK		10002
#define ID_Cancel	10003
#define BTNWID		100
#define BTNHEI		24
#define EDITBORDER	4

#define FontHei		12

LRESULT WINAPI WndProc_PopMsgWin(HWND  hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);


//	构造函数,这里会创建一个线程用于处理窗口消息
CPopMsgWin::CPopMsgWin(const Stru_Param& Param)
{
    m_Param=Param;

    m_hWnd=NULL;
	m_hEdit=NULL;
	m_hThread=NULL;
	m_hWinSta_Visible=0;
	m_hWinSta_Ori=0;
	m_hDesktop_Ori=0;
	m_hDesktop_Visible=0;
	m_hFont=NULL;
	m_hBkBrush=NULL;

	m_hFont=(int)CreateFont(-1*FontHei,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,"宋体");		//"Arial"
	m_hBkBrush=(int)CreateSolidBrush(m_Param.BackgroundColor);

    m_bClickOKBtn=false;

	WNDCLASSEX wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize			=sizeof(wc);
	wc.style			=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc		=WndProc_PopMsgWin;
	wc.cbClsExtra		=0;
	wc.cbWndExtra		=24;		//0:pObj; 8:保存是否显示滚动条bVScrollVisible; 12:TextColor; 16:BackgroundColor; 20:bClearAsClose
	wc.hInstance		=GetModuleHandle(NULL); 
	wc.hCursor			=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon			=LoadIcon(NULL,IDI_ASTERISK);
	wc.hbrBackground	=(HBRUSH)m_hBkBrush;
	wc.lpszMenuName		=NULL;
	wc.lpszClassName	="PopMsgWinClass"; 
	RegisterClassEx(&wc);

	m_hThread=(int)CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)WorkerThread,this,0,&m_hThreadID);
    CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCPopMsgWin::ThreadFunc",m_hThreadID);
}


//	析构函数
CPopMsgWin::~CPopMsgWin()
{
    CeasymutexGuard guard(m_mutex);

	DWORD			ExitCode;
	if(m_hThread&&m_hWnd&&IsWindow((HWND)m_hWnd))
	{
		//Post WM_QUIT让GetMessage退出
		PostMessage((HWND)m_hWnd,WM_QUIT,0,0);
	}
	if(m_hThread)
	{
		WaitForSingleObject((HANDLE)m_hThread,400000);
		if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hThread,0);
	}
	if(m_hFont)
	{
		DeleteObject((HFONT)m_hFont);
		m_hFont=NULL;
	}
	if(m_hBkBrush)
	{
		DeleteObject((HBRUSH)m_hBkBrush);
		m_hBkBrush=NULL;
	}
	UnregisterClass("PopMsgWinClass",GetModuleHandle(NULL));

	if(m_Param.hWnd_PostCloseMsg&&IsWindow((HWND)m_Param.hWnd_PostCloseMsg))
		PostMessage((HWND)m_Param.hWnd_PostCloseMsg,WM_COMMAND,m_Param.wParam_PostCloseMsg,m_bClickOKBtn?1:0);
}

void CPopMsgWin::Show(void)
{
	ShowWindow((HWND)m_hWnd,SW_SHOW);
}
void CPopMsgWin::Hide(void)
{
	ShowWindow((HWND)m_hWnd,SW_HIDE);
}
void CPopMsgWin::Clear(void)
{
    CeasymutexGuard guard(m_mutex);

	if(m_hWnd&&m_hEdit)
	{
		SetWindowText((HWND)m_hEdit,"");
		ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
		SetWindowLong((HWND)m_hWnd,8,0);
	}
}
//	将一段字符串添加到窗口中
//	bAddCRLF: 添加回车换行到行尾
//	bInsertTime: 添加时间到行首
//	DelOldContent: 是否删除原来的内容
////	Mode=0:行模式，每次自动添加日志时间和行尾回车
////	Mode=1:流模式，不自动添加东西
void CPopMsgWin::AppendStr(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *Str)
{
    CeasymutexGuard guard(m_mutex);

    AppendStr_Internal(bForceShow,bDelOldContent,bAddCRLF,bInsertTime,Str);
}
void CPopMsgWin::AppendStr_Internal(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *Str)
{
	SYSTEMTIME			CurTime;
	int len;

	if(!m_hWnd||!m_hEdit||!Str) return;

	if(bDelOldContent==DelOldContent) 
	{
		if(m_hWnd)
		{
			SetWindowText((HWND)m_hEdit,"");
			ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
			SetWindowLong((HWND)m_hWnd,8,0);
		}
		else m_ContentBeforeWndCreated.clear();
	}
	else
	{
		if(m_hWnd&&m_Param.MaxMsgLen>0)
		{
			//内容超长，裁掉前一半
			len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
			if(len>m_Param.MaxMsgLen-(int)strlen(Str)-20)							
			{											
				SendMessage((HWND)m_hEdit,EM_SETSEL,0,len/2);
				SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)" ");
			}
		}
	}

	if(bInsertTime==InsertTime)
	{
		GetLocalTime(&CurTime);
		char buf[256];
		_snprintf(buf,sizeof(buf)-1,"%02d:%02d:%02d.%03d ",CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds);
		if(m_hWnd)
		{
			len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
			SendMessage((HWND)m_hEdit,EM_SETSEL,len,len);
			SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)buf);
		}
		else m_ContentBeforeWndCreated=string(buf)+m_ContentBeforeWndCreated;
	}

	if(m_hWnd)
	{
		len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
		SendMessage((HWND)m_hEdit,EM_SETSEL,len,len);
		SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)Str);
	}

	if(bAddCRLF==AddCRLF)
	{
		if(m_hWnd)
		{
			len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
			SendMessage((HWND)m_hEdit,EM_SETSEL,len,len);
			SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)"\r\n");
		}
		else m_ContentBeforeWndCreated=m_ContentBeforeWndCreated+string("\r\n");
	}

	if(m_hWnd)
	{
		SendMessage((HWND)m_hEdit,EM_SCROLLCARET,0,0);

		int FirstVisibleLine=SendMessage((HWND)m_hEdit,EM_GETFIRSTVISIBLELINE,0,0);
		int bVScrollVisible=(int)GetWindowLong((HWND)m_hWnd,8);
		if(FirstVisibleLine!=0&&!bVScrollVisible)
		{
			ShowScrollBar((HWND)m_hEdit,SB_VERT,TRUE);
			SetWindowLong((HWND)m_hWnd,8,1);
		}
		else if(FirstVisibleLine==0&&bVScrollVisible)
		{
			ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
			SetWindowLong((HWND)m_hWnd,8,0);
		}

		if(bForceShow==ForceShow&&m_hWnd&&!IsWindowVisible((HWND)m_hWnd))
			ShowWindow((HWND)m_hWnd,SW_SHOW);
	}
}
void CPopMsgWin::AppendStr2(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char* fmt,...)
{
	va_list ap;
	char buf[1024];

	buf[sizeof(buf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(buf,sizeof(buf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);

	AppendStr_Internal(bForceShow,bDelOldContent,bAddCRLF,bInsertTime,buf);
}
void CPopMsgWin::AppendStr3(const char *pStr)
{
    AppendStr(m_Param.bForceShow,m_Param.bDelOldContent,m_Param.bAddCRLF,m_Param.bInsertTime,pStr);
}
void CPopMsgWin::AppendStr4(const char* fmt,...)
{
	va_list ap;
	char buf[1024];

	buf[sizeof(buf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(buf,sizeof(buf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);

	AppendStr_Internal(m_Param.bForceShow,m_Param.bDelOldContent,m_Param.bAddCRLF,m_Param.bInsertTime,buf);
}

//	设置窗口标题内容
void CPopMsgWin::SetTitle(const char *pStr)
{
    CeasymutexGuard guard(m_mutex);

	if(m_hWnd) SetWindowText((HWND)m_hWnd,pStr);
	else m_TitleBeforeWndCreated=string(pStr);
}

//  等待窗口创建成功
bool CPopMsgWin::WaitWndCreate(int TimeoutMS)
{
    DWORD StartMS=GetTickCount();
    while(m_hWnd==NULL||m_hEdit==NULL)
    {
        Sleep(100);
        if(TimeoutMS>0&&(int)(GetTickCount()-StartMS)>=TimeoutMS)
            return false;
    }
    return true;
}

//	创建窗口
void CPopMsgWin::CreateWindow_Internal(void)
{
	HANDLE			hInst;

	hInst=GetModuleHandle(NULL);

    unsigned int dwExStyle;
    unsigned int dwStyle;

    dwExStyle=WS_EX_CLIENTEDGE|(m_Param.bToolWindow?WS_EX_TOOLWINDOW:0)|(m_Param.bTopMost?WS_EX_TOPMOST:0);
    dwStyle=WS_CAPTION|WS_THICKFRAME|WS_POPUP|WS_BORDER;
    if(m_Param.bInitVisible) dwStyle|=WS_VISIBLE;
    if(m_Param.bSysMenu) dwStyle|=WS_SYSMENU;
    if(m_Param.bMinMaxBox) dwStyle |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

	string strTitle=m_TitleBeforeWndCreated.empty()?string(m_Param.szTitle):m_TitleBeforeWndCreated;
	m_hWnd=(int)CreateWindowEx(
        dwExStyle,
		"PopMsgWinClass",strTitle.c_str(),
        dwStyle,
		m_Param.InitPox,m_Param.InitPoy,m_Param.InitWid,m_Param.InitHei,
		NULL,NULL,(HINSTANCE)hInst,NULL);
	if(m_hWnd==NULL) return;

	//将pObj放到WindowLong(0)上
	SetWindowLong((HWND)m_hWnd,0,(LONG)this);
	//将m_Param.bClearAsClose设置到WindowLong(12)上
	SetWindowLong((HWND)m_hWnd,20,(int)m_Param.bClearAsClose);

	RECT Rect;
	GetClientRect((HWND)m_hWnd,&Rect);
	int EditHei=Rect.bottom-Rect.top;

	if(m_Param.bCreateOKBtn||m_Param.bCreateCancelBtn)
	{
		//创建确定/取消按钮
        int pox=(Rect.right-Rect.left-BTNWID)/2;
        if(m_Param.bCreateOKBtn&&m_Param.bCreateCancelBtn)
            pox=(Rect.right-Rect.left-BTNWID*2-BTNWID/2)/2;

        if(m_Param.bCreateOKBtn)
        {
		    HWND hBtn=CreateWindow("BUTTON",m_Param.szTxt_OKBtn,
						    WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|BS_TEXT|BS_VCENTER|BS_CENTER|BS_FLAT,
						    pox,Rect.bottom-Rect.top-BTNHEI-BTNHEI/4,BTNWID,BTNHEI,
						    (HWND)m_hWnd,(HMENU)ID_OK,(HINSTANCE)hInst,NULL);
		    SendMessage(hBtn,WM_SETFONT,(WPARAM)m_hFont,0);
            pox+=BTNWID+BTNWID/2;
        }
        if(m_Param.bCreateCancelBtn)
        {
		    HWND hBtn=CreateWindow("BUTTON",m_Param.szTxt_CancelBtn,
						    WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|BS_TEXT|BS_VCENTER|BS_CENTER|BS_FLAT,
						    pox,Rect.bottom-Rect.top-BTNHEI-BTNHEI/4,BTNWID,BTNHEI,
						    (HWND)m_hWnd,(HMENU)ID_Cancel,(HINSTANCE)hInst,NULL);
		    SendMessage(hBtn,WM_SETFONT,(WPARAM)m_hFont,0);
            pox+=BTNWID+BTNWID/2;
        }
		EditHei-=BTNHEI/4+BTNHEI+BTNHEI/4;
		if(EditHei<0) EditHei=0;
	}

	if(m_Param.bDispMessageBorder)
		m_hEdit=(int)CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_WANTRETURN|ES_MULTILINE|ES_READONLY,
			EDITBORDER,EDITBORDER,Rect.right-Rect.left-EDITBORDER*2,EditHei-EDITBORDER*2,
			(HWND)m_hWnd,(HMENU)ID_EditWnd,(HINSTANCE)hInst,NULL);
	else
		m_hEdit=(int)CreateWindow("EDIT","",
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_WANTRETURN|ES_MULTILINE|ES_READONLY,
			EDITBORDER,EDITBORDER,Rect.right-Rect.left-EDITBORDER*2,EditHei-EDITBORDER*2,
			(HWND)m_hWnd,(HMENU)ID_EditWnd,(HINSTANCE)hInst,NULL);

	HideCaret((HWND)m_hEdit);
	ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
	SetWindowLong((HWND)m_hWnd,8,0);
	SetWindowLong((HWND)m_hWnd,12,m_Param.TextColor);
	SetWindowLong((HWND)m_hWnd,16,m_Param.BackgroundColor);

	SendMessage((HWND)m_hEdit,WM_SETFONT,(WPARAM)m_hFont,0);

	if(m_Param.MaxMsgLen>0)
		SendMessage((HWND)m_hEdit,EM_LIMITTEXT,m_Param.MaxMsgLen,0);

	if(!m_ContentBeforeWndCreated.empty())
		AppendStr_Internal(ForceShow,NoDelOldContent,NoAddCRLF,NoInsertTime,m_ContentBeforeWndCreated.c_str());
}





//	工作线程
unsigned int CPopMsgWin::WorkerThread(void *arg)						
{
	CPopMsgWin		*pMsgWin;
	MSG				msg;

	pMsgWin=(CPopMsgWin*)arg;
	if(!pMsgWin) return 0xffffffff;

	if(pMsgWin->m_Param.bInServiceID)
		pMsgWin->OpenDisplayInService_Internal();

	pMsgWin->m_mutex.lock();
	pMsgWin->CreateWindow_Internal();
	pMsgWin->m_mutex.unlock();

	//如果设置了bDestroyWhenClose, 当遇到WM_CLOSE时，结束线程前需要delete pobj。
	bool bNeedDeleteObject_if_bDestroyWhenClose=false;

	while(pMsgWin->m_hWnd&&
		IsWindow((HWND)pMsgWin->m_hWnd)&&
		GetMessage(&msg,(HWND)pMsgWin->m_hWnd,0,0))
	{
        //在WndProc_PopMsgWin里没有pMsgWin句柄，只好在这里做了
        if(msg.message==WM_COMMAND&&LOWORD(msg.wParam)==ID_OK)
            pMsgWin->m_bClickOKBtn=true;

		if(msg.message==WM_CLOSE&&pMsgWin->m_Param.bDestroyWhenClose)
		{
			bNeedDeleteObject_if_bDestroyWhenClose=true;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//析构时，发送WM_QUIT让GetMessage退出，但是窗口并未销毁，在此处销毁
	if(IsWindow((HWND)pMsgWin->m_hWnd))
		DestroyWindow((HWND)pMsgWin->m_hWnd);
	pMsgWin->m_hWnd=NULL;
	pMsgWin->m_hEdit=NULL;

	if(pMsgWin->m_Param.bInServiceID)
		pMsgWin->CloseDisplayInService_Internal();

	pMsgWin->m_hThread=NULL;

	if(bNeedDeleteObject_if_bDestroyWhenClose)
	{
		//由于设置了bDestroyWhenClose, 当遇到WM_CLOSE时，结束线程前需要delete pobj。此时m_hThread已经为NULL，不会引起死循环
		delete pMsgWin;
	}

	return 0; 
}

//******************************************************************
//  Name:					MainWndProc
//  Description:  主窗口的消息处理过程
//******************************************************************
static LRESULT WINAPI WndProc_PopMsgWin(HWND  hWnd,UINT Msg,WPARAM wParam,LPARAM lParam) 
{
  switch(Msg)
  {
	case WM_SIZE:
	{
		HWND hEdit=GetDlgItem(hWnd,ID_EditWnd);
		if(hEdit)
		{
			//调整Edit大小
			RECT Rect;
			GetClientRect(hWnd,&Rect);
			int EditHei=Rect.bottom-Rect.top;

			HWND hBtnOK=GetDlgItem(hWnd,ID_OK);
			if(hBtnOK)
			{
				MoveWindow(hBtnOK,(Rect.right-Rect.left-BTNWID)/2,Rect.bottom-Rect.top-BTNHEI/4-BTNHEI,BTNWID,BTNHEI,TRUE);
				EditHei-=BTNHEI/4+BTNHEI+BTNHEI/4;
				if(EditHei<0) EditHei=0;
			}

			MoveWindow(hEdit,EDITBORDER,EDITBORDER,Rect.right-Rect.left-EDITBORDER*2,EditHei-EDITBORDER*2,TRUE);


			//判断是否需要显示/隐藏滚动条
			bool bVScrollVisible=GetWindowLong(hWnd,8)?true:false;
			if(!bVScrollVisible)
			{
				//判断是否需要显示滚动条
				//注意，这里不能用WM_GETTEXTLENGTH配合EM_POSFROMCHAR。因为WM_GETTEXTLENGTH返回asc字符长度，EM_POSFROMCHAR使用的是UniCode索引
				int LineCnt=(int)SendMessage(hEdit,EM_GETLINECOUNT,0,0);		//取行数返回值总是>=1。没有内容也返回1
				int LineIndex=(int)SendMessage(hEdit,EM_LINEINDEX,LineCnt-1,0);	//取最后一行的第一个字符的索引。

				//有时，最后的LineIndex，取Pos失败，那么就往前找
				while(LineIndex>0)
				{
					LRESULT pos=SendMessage(hEdit,EM_POSFROMCHAR,(WPARAM)LineIndex,0);
					if(pos!=-1)
					{
						int poy=HIWORD(pos);
						RECT Rect;
						SendMessage(hEdit,EM_GETRECT,0,(LPARAM)&Rect);
						if(poy+FontHei>=Rect.bottom)
						{
							ShowScrollBar((HWND)hEdit,SB_VERT,TRUE);
							SetWindowLong(hWnd,8,1);
						}
						break;
					}
					else LineIndex--;
				}
			}
			else
			{
				//判断是否需要隐藏滚动条
				SCROLLINFO ScrollInfo;

				memset(&ScrollInfo,0,sizeof(ScrollInfo));
				ScrollInfo.cbSize=sizeof(SCROLLINFO);
				ScrollInfo.fMask=SIF_ALL;
				GetScrollInfo(hEdit,SB_VERT,&ScrollInfo);
				if(ScrollInfo.nMin+(int)ScrollInfo.nPage>ScrollInfo.nMax)
				{
					ShowScrollBar((HWND)hEdit,SB_VERT,FALSE);
					SetWindowLong(hWnd,8,0);
				}
			}
		}
		break;
	}
    case WM_CLOSE:	
	{
		//隐藏，不销毁
		ShowWindow(hWnd,SW_HIDE);

		int bClearAsClose=GetWindowLong(hWnd,20)==0?false:true;
		if(bClearAsClose)
		{
			HWND hEdit=GetDlgItem(hWnd,ID_EditWnd);
			SetWindowText((HWND)hEdit,"");
			ShowScrollBar((HWND)hEdit,SB_VERT,FALSE);
			SetWindowLong((HWND)hWnd,0,0);
		}
		return 0;
	}
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
	{
		SetBkColor((HDC)wParam,(COLORREF)GetWindowLong(hWnd,16));
		SetTextColor((HDC)wParam,(COLORREF)GetWindowLong(hWnd,12));
		return (LRESULT)GetClassLong(hWnd,GCL_HBRBACKGROUND);
	}
	case WM_COMMAND:
	{
		switch LOWORD(wParam)
		{
			case ID_OK:
			{
				SendMessage(hWnd,WM_CLOSE,0,0);
				return 0;
			}
		}
		break;
	}
  }
  return(DefWindowProc(hWnd,Msg,wParam,lParam));
}


//	名称 ： OpenDisplayInService
//	描述 ： 打开在服务中显示信息的环境
//	参数 ： 
//			hWinSta_Visible:可视的进程工作站句柄
//			hWinSta_Ori:原始的进程工作站句柄
//			hOriDesk:原始的线程桌面，恢复时使用
//	返回 ： 1. 返回时，hOriDesk保存先前的句柄指针
//			2. TRUE:成功，FALSE:失败
void CPopMsgWin::OpenDisplayInService_Internal(void)
{
	if(m_hWinSta_Visible) return;

	m_hWinSta_Visible=(int)OpenWindowStation("WinSta0",FALSE,MAXIMUM_ALLOWED); 
	m_hWinSta_Ori=(int)GetProcessWindowStation();
	if(m_hWinSta_Visible&&m_hWinSta_Ori&&m_hWinSta_Visible!=m_hWinSta_Ori)
		SetProcessWindowStation((HWINSTA)m_hWinSta_Visible); 

	m_hDesktop_Visible= (int)OpenDesktop("Default",0,FALSE,MAXIMUM_ALLOWED); 
    m_hDesktop_Ori= (int)GetThreadDesktop(m_hThreadID); 
	if(m_hDesktop_Visible&&m_hDesktop_Ori&&m_hDesktop_Visible!=m_hDesktop_Ori)
		SetThreadDesktop((HDESK)m_hDesktop_Visible); 
}



//	名称 ： CloseDisplayInService
//	描述 ： 关闭在服务中显示信息的环境
//	参数 ： 
//			hWinSta_Visible:可视的进程工作站句柄
//			hWinSta_Ori:原始的进程工作站句柄
//			hOriDesk:原始的线程桌面，恢复时使用
//	返回 ： TRUE:成功，FALSE:失败
void CPopMsgWin::CloseDisplayInService_Internal(void)
{
	if(!m_hWinSta_Visible) return;
																				//切换线程桌面
	if(m_hDesktop_Visible&&m_hDesktop_Ori&&m_hDesktop_Visible!=m_hDesktop_Ori)
		SetThreadDesktop((HDESK)m_hDesktop_Ori);
	if(m_hDesktop_Visible&&m_hDesktop_Visible!=m_hDesktop_Ori)
	{
		CloseDesktop((HDESK)m_hDesktop_Visible); 
		m_hDesktop_Visible=NULL;
	}
	m_hDesktop_Ori=NULL;

																				//切换进程工作站
	if(m_hWinSta_Visible&&m_hWinSta_Ori&&m_hWinSta_Visible!=m_hWinSta_Ori)
	{
		SetProcessWindowStation((HWINSTA)m_hWinSta_Ori);
		m_hWinSta_Ori=NULL;
	}
	if(m_hWinSta_Visible)
	{
		CloseWindowStation((HWINSTA)m_hWinSta_Visible);
		m_hWinSta_Visible=NULL;
	}
}

//下面函数创建MessageBox。关闭即销毁。窗口居中。hWnd_PostCloseMsg;销毁对象时，是否向指定窗口Post WM_COMMAND消息。默认为0,不Post；wParam_PostCloseMsg;wParam值。lParam为是否点击了OK按钮
void CPopMsgWin::CreateMessageBox(const char* content,const char* caption,int wid,int hei,bool btopmost)
{
	Stru_Param Param;
    Param.bTopMost=btopmost;
	Param.bDispMessageBorder=false;
	Param.InitWid=wid;
	Param.InitHei=hei;
	Param.InitPox=(GetSystemMetrics(SM_CXSCREEN)-wid)/2;
	Param.InitPoy=(GetSystemMetrics(SM_CYSCREEN)-hei)/2;
	Param.bInsertTime=NoInsertTime;
	Param.bDestroyWhenClose=true;
	if(caption) strncpy(Param.szTitle,caption,sizeof(Param.szTitle)-1);
	if(content) CPopMsgWin(Param).AppendStr3(content);
}
void CPopMsgWin::CreateMessageBoxOKCancel(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg,int wid,int hei,bool btopmost)
{
	Stru_Param Param;
	Param.bCreateCancelBtn=true;
    Param.bTopMost=btopmost;
	Param.bDispMessageBorder=false;
	Param.InitWid=wid;
	Param.InitHei=hei;
	Param.InitPox=(GetSystemMetrics(SM_CXSCREEN)-wid)/2;
	Param.InitPoy=(GetSystemMetrics(SM_CYSCREEN)-hei)/2;
	Param.bInsertTime=NoInsertTime;
	Param.bDestroyWhenClose=true;
	Param.hWnd_PostCloseMsg=hWnd_PostCloseMsg;
	Param.wParam_PostCloseMsg=wParam_PostCloseMsg;
	if(caption) strncpy(Param.szTitle,caption,sizeof(Param.szTitle)-1);
	if(content) CPopMsgWin(Param).AppendStr3(content);
}
void CPopMsgWin::CreateMessageBoxYESNO(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg,int wid,int hei,bool btopmost)
{
	Stru_Param Param;
	Param.bCreateCancelBtn=true;
    Param.bTopMost=btopmost;
	Param.bDispMessageBorder=false;
	Param.InitWid=wid;
	Param.InitHei=hei;
	Param.InitPox=(GetSystemMetrics(SM_CXSCREEN)-wid)/2;
	Param.InitPoy=(GetSystemMetrics(SM_CYSCREEN)-hei)/2;
	Param.bInsertTime=NoInsertTime;
	Param.bDestroyWhenClose=true;
	Param.hWnd_PostCloseMsg=hWnd_PostCloseMsg;
	Param.wParam_PostCloseMsg=wParam_PostCloseMsg;
	strncpy(Param.szTxt_OKBtn,"是",sizeof(Param.szTxt_OKBtn)-1);
	strncpy(Param.szTxt_CancelBtn,"否",sizeof(Param.szTxt_CancelBtn)-1);
	if(caption) strncpy(Param.szTitle,caption,sizeof(Param.szTitle)-1);
	if(content) CPopMsgWin(Param).AppendStr3(content);
}
