#include "stdafx.h"
#include "doupdate2.h"
#include "updateexe.h"
#include "tmpstackbuf.h"
#include "Tools_Win32.h"
#include "IsProcessExist.h"

char Cdoupdate2::FastTraderFileName[512];

Cdoupdate2::Cdoupdate2()
:   Cdoupdate(),
    m_bUpdateSucc(false)
{
    //取当前目录
    char mainpath[512]={0};
    CTools_Win32::GetMainPath(mainpath,sizeof(mainpath));

    char xmlfilename[512]={0};
    HFILE hF;

    //检查update下是否有FastTrader_Update.xml
    sprintf_s(xmlfilename,sizeof(xmlfilename)-1,"%s%s\\%s",mainpath,SubPathName_updatefiles,Cdoupdate::m_UpdateFileNameInSvr.c_str());
    hF=_lopen(xmlfilename,OF_READ);
    if(hF==HFILE_ERROR)
    {
        PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
        return;
    }
    _lclose(hF);

    //解析FastTrader_Update.xml
    m_UpdateXml.Parse(string(xmlfilename));
    if(!m_UpdateXml.m_bvalid)
    {
        PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
        return;
    }

    //检查全部升级文件是否存在
    map<string,bool>::iterator it;
    for(it=m_UpdateXml.m_replacefiles.begin();it!=m_UpdateXml.m_replacefiles.end();it++)
    {
        char sourfile[512]={0},purefilename[512]={0};

        sprintf_s(sourfile,sizeof(sourfile)-1,"%s%s\\%s",mainpath,SubPathName_updatefiles,it->first.c_str());

        //updateexe.exe在FastTrader里拷贝了
        CTools_Ansi::GetPureFileName(sourfile,purefilename,sizeof(purefilename)-1);
        if(string(purefilename)==string(UpdateExeName))
            continue;               

        hF=_lopen(sourfile,OF_READ);
        if(hF==HFILE_ERROR)
        {
            PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
            return;
        }
        _lclose(hF);
    }

    //每隔200ms判断FastTrader.exe进程是否存在，连续20次(4s)
    memset(FastTraderFileName,0,sizeof(FastTraderFileName));
    sprintf_s(FastTraderFileName,sizeof(FastTraderFileName)-1,"%sFastTrader.exe",mainpath);

    //定时检查就的交易终端进程是否已经停止
    SetTimer(NULL,1001,100,TimerCBFunc);

    //FastTrader.exe仍未退出，提示用户
    while(CIsProcessExist(string(FastTraderFileName)).m_bExist)
    {
        int irlt=MessageBox(
            NULL,
            "正在等待交易终端退出以完成升级，请稍候......\n\n"
            "点击[取消]将取消此次升级",
            MessageTitle,
            MB_OKCANCEL);
        if(irlt==IDCANCEL)
        {
            PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
            return;
        }
    }

    KillTimer(NULL,1001);

    //将update目录下的升级文件复制到当前目录下，并删除原有文件
    for(it=m_UpdateXml.m_replacefiles.begin();it!=m_UpdateXml.m_replacefiles.end();it++)
    {
        char sourfile[512]={0},destfile[512]={0},purefilename[512]={0};

        sprintf_s(sourfile,sizeof(sourfile)-1,"%s%s\\%s",mainpath,SubPathName_updatefiles,it->first.c_str());
        sprintf_s(destfile,sizeof(destfile)-1,"%s%s",mainpath,it->first.substr(m_UpdateXml.m_version_svrpath.size()+1).c_str());

        //updateexe.exe在FastTrader里拷贝了
        CTools_Ansi::GetPureFileName(sourfile,purefilename,sizeof(purefilename)-1);
        if(string(purefilename)==string(UpdateExeName))
            continue;               

        if(!CopyFile(sourfile,destfile,FALSE))
        {
            char buf[256]={0};
            sprintf_s(buf,sizeof(buf)-1,"文件拷贝不成功(%s)，升级失败。",purefilename);

            MessageBox(NULL,buf,MessageTitle,MB_OK);
            PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
            return;
        }
        unlink(sourfile);
    }

    m_bUpdateSucc=true;

    char buf[256]={0};
    sprintf_s(buf,sizeof(buf)-1,"FastTrader升级成功。当前版本号为%s。\n\n点击[确定]后将为您重启交易终端。",m_UpdateXml.m_version.c_str());
    MessageBox(NULL,buf,MessageTitle,MB_OK);

    //启动FastTrader.exe
    char pathfilename[512]={0};
    sprintf_s(pathfilename,sizeof(pathfilename)-1,"%s%s",mainpath,"FastTrader.exe");
    WinExec(pathfilename,SW_SHOW);

    //退出升级程序
    PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
}
Cdoupdate2::~Cdoupdate2()
{
}

//定时检查就的交易终端进程是否已经停止
void CALLBACK Cdoupdate2::TimerCBFunc( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    if(!CIsProcessExist(string(FastTraderFileName)).m_bExist)
    {
        HWND hWnd=FindWindow(NULL,MessageTitle);
        if(hWnd!=NULL)
            PostMessage(hWnd,WM_COMMAND,IDOK,0);
    }
}

