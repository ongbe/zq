//*******************************************************************************************
//	作用：	定义弹出消息窗口类
//	描述：	1. 此类创建一个线程，线程中创建一个顶层edit窗口，应用程序可以向该窗口发送调试信息，显示在窗口里
//			2. 此类仅应用于Win32环境下
//			3. 注意，要彻底销毁这个窗口，使用者析构CPopMsgWin对象即可。切不可用DestroyWindow的方式销毁，否则会导致内存泄露
//			4. 这个类创建Edit窗口，Edit窗口过程由系统在WorkerThread中实现。
//			5. 基于“系统的Edit处理不会阻塞”的理由，所以在ShowWindow、AppendStr的操作时也不会阻塞。这个假设的前提不够严谨，但是正常情况下没问题。
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//*******************************************************************************************

#ifndef POPMSGWIN_H_
#define POPMSGWIN_H_

#ifdef WIN32
#pragma managed(push,off)
#endif 


typedef BOOL (*OPENSERVICEDESKTOP)(HANDLE &hOriDesk,HANDLE &hVisibleDesk);		//在服务中，打开桌面环境的回调函数
typedef BOOL (*CLOSESERVICEDESKTOP)(HANDLE &hOriDesk,HANDLE &hVisibleDesk);		//在服务中，关闭桌面环境的回调函数


#include "easymutex.h"
#include "Tools_Win32.h"
#include <string>
using std::string;


//	调试窗口类
class CPopMsgWin
{
public:
	enum eForceShow
	{
		ForceShow,		//添加内容时强制显示窗口
		NoForceShow		//不强制显示窗口
	};
	enum eDelOldContent
	{
		DelOldContent,	//添加内容时删除旧的内容
		NoDelOldContent	//不删除旧的内容
	};
	enum eAddCRLF
	{
		AddCRLF,		//添加内容时增加回车换行
		NoAddCRLF		//不强制显示窗口
	};
	enum eInsertTime
	{
		InsertTime,		//添加内容时插入时间
		NoInsertTime	//不插入时间
	};

    struct Stru_Param
    {
        Stru_Param()
        :   bInServiceID(false),
	        bCreateOKBtn(true),
	        bCreateCancelBtn(false),
	        bClearAsClose(true),
            bToolWindow(false),
            bTopMost(false),
            bMinMaxBox(true),
            bSysMenu(true),
			bDispMessageBorder(true),
            bInitVisible(true),
            InitPox(100),
            InitPoy(100),
            InitWid(320),
            InitHei(240),
            MaxMsgLen(0),
	        TextColor(0),
	        BackgroundColor(0xf8f8f8),
            bForceShow(ForceShow),
            bDelOldContent(NoDelOldContent),
            bAddCRLF(AddCRLF),
            bInsertTime(InsertTime),
			bDestroyWhenClose(false),
			hWnd_PostCloseMsg(0),
			wParam_PostCloseMsg(1000)
        {
            memset(szTitle,0,sizeof(szTitle));
            memset(szTxt_OKBtn,0,sizeof(szTxt_OKBtn));
            strncpy(szTxt_OKBtn,"确定",sizeof(szTxt_OKBtn)-1);
            strncpy(szTxt_CancelBtn,"取消",sizeof(szTxt_CancelBtn)-1);
        };
        void SetScreenCenter(void)
        {
            int ScnWid,ScnHei;
            CTools_Win32::GetWorkAreaSize(ScnWid,ScnHei);
            InitPox=(ScnWid-InitWid)/2;
            InitPoy=(ScnHei-InitHei)/2;
        };

	    bool			bInServiceID;					//是否在服务模式中运行
	    bool			bCreateOKBtn;
	    bool			bCreateCancelBtn;
	    bool			bClearAsClose;
        bool            bToolWindow;                    //是否是ToolWindow模式
        bool            bTopMost;                       //是否是TopMost模式
        bool            bMinMaxBox;                     //是否带最大最小化按钮
        bool            bSysMenu;                       //是否带SYSMENU
		bool			bDispMessageBorder;				//是否显示信息的边框

        bool            bInitVisible;
        int             InitPox;
        int             InitPoy;
        int             InitWid;
        int             InitHei;

        int				MaxMsgLen;						//消息文本的最大长度

	    unsigned int	TextColor;
	    unsigned int	BackgroundColor;
        char            szTitle[128];                   //消息标题
        char            szTxt_OKBtn[128];               //OK按钮的文本
        char            szTxt_CancelBtn[128];           //Cancel按钮的文本

        eForceShow      bForceShow;
        eDelOldContent  bDelOldContent;
        eAddCRLF        bAddCRLF;
        eInsertTime     bInsertTime;

		//关闭窗口即销毁对象的设置。点击OK、Cancel、右上角叉叉都可以关闭窗口
		bool			bDestroyWhenClose;				//关闭窗口时是否销毁对象。默认不销毁，只隐藏
		int				hWnd_PostCloseMsg;				//销毁对象时，是否向指定窗口Post WM_COMMAND消息。默认为0,不Post
		int				wParam_PostCloseMsg;			//销毁对象时，向指定窗口Post WM_COMMAND消息的wParam值。lParam为是否点击了OK按钮


	};

	CPopMsgWin(const Stru_Param& Param=Stru_Param());
	virtual ~CPopMsgWin();

	void Show(void);
	void Hide(void);

	//	清除窗口内容
	void Clear(void);
	//	将一段字符串添加到窗口中
	//	bForceShow: 如果窗口是隐藏的，则显示出来
	//	bAddCRLF: 添加回车换行到行尾
	//	bInsertTime: 添加时间到行首
	//	DelOldContent: 是否删除原来的内容
	void AppendStr(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *pStr);
	void AppendStr2(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char* fmt,...);
	void AppendStr3(const char *pStr);
	void AppendStr4(const char* fmt,...);

	//	设置窗口标题内容
	void SetTitle(const char *pStr);

    //  判断窗口是否已经创建
    bool IsWndCreated(void) { return m_hWnd!=NULL&&m_hEdit!=NULL; }

    //  等待窗口创建成功
    bool WaitWndCreate(int TimeoutMS);


private:
	int GethWnd(void) { return m_hWnd; }					//获取窗口句柄

	void CreateWindow_Internal(void);						//创建窗口

	void AppendStr_Internal(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *pStr);


	OPENSERVICEDESKTOP  m_OpenDesktopFunc;
	CLOSESERVICEDESKTOP m_CloseDesktopFunc;
	HANDLE			    m_hOriDesk;								//线程的原始桌面句柄
	HANDLE			    m_hVisibleDesk;							//可视桌面句柄

	//	工作线程
	static unsigned int WorkerThread(void *arg);

	void OpenDisplayInService_Internal(void);
	void CloseDisplayInService_Internal(void);

private:
    Ceasymutex      m_mutex;

    Stru_Param      m_Param;

	int				m_hFont;
	int				m_hBkBrush;
	int				m_hWnd;									//主窗口句柄
	int				m_hEdit;								//Edit窗口句柄

	int				m_hThread;								//线程
	unsigned long	m_hThreadID;

	int				m_hWinSta_Visible;						//可视的进程工作站句柄
	int				m_hWinSta_Ori;							//原始的进程工作站句柄
	int				m_hDesktop_Ori;
	int				m_hDesktop_Visible;

public:
    bool            m_bClickOKBtn;                          //是否点了OK按钮

public:
	//下面函数创建MessageBox。关闭即销毁。窗口居中。hWnd_PostCloseMsg;销毁对象时，是否向指定窗口Post WM_COMMAND消息。默认为0,不Post；wParam_PostCloseMsg;wParam值。lParam为是否点击了OK按钮
	static void	CreateMessageBox(const char* content,const char* caption,int wid=320,int hei=240,bool btopmost=true);
	static void	CreateMessageBoxOKCancel(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg=1000,int wid=320,int hei=240,bool btopmost=true);
	static void	CreateMessageBoxYESNO(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg=1000,int wid=320,int hei=240,bool btopmost=true);

private:
	string			m_TitleBeforeWndCreated;				//窗口创建以前用SetTitle设置的窗口标题
	string			m_ContentBeforeWndCreated;				//窗口创建以前设置的窗口内容
};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif