#ifndef __MYAPP_H__
#define __MYAPP_H__


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
class zqWriteLog;
class MainFrame;
class MyApp :	public wxApp
{
public:
	MyApp(void);
	~MyApp(void);
    /***********************************************************
    *   brief   :   默认程序初始化函数
    *   param   :   nothing
    *   return  :   成功 true 失败 false
    ***********************************************************/    
	virtual bool OnInit();
    virtual int OnExit();
    virtual void CleanUp();
    void Release(bool retval=true);
    bool StartMainFrame(bool bInit=false);
    void GetFileName(std::string &filename,bool bWithExt=true);
	bool UnZipXMLFile(string rcid,string strname);
    void StartLogSystem();
    void StopLogSystem();

	//去除当前目录的只读属性
	BOOL IsDirWriteable();

	BOOL IsFileWriteable( const std::string &strApiFile );
	void DeleteOldLogFile(int days);
private:
    //主窗口最小尺寸
    wxSize m_frame_minsize;
    MainFrame* m_frame;
    zqWriteLog*           m_pWriteLog;
};
// Implements MyApp& wxGetApp()
DECLARE_APP(MyApp)

#endif
