#pragma once


#include "stdafx.h"
#include "MyXML.h"
#include "TcpClientWorker.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "parseupdatexml.h"
#include "VersionInfo2.h"
#include "popmsgwin.h"

#include <string>
using std::string;

#define XMLName_AutoUpdate_InFastTrader     "AutoUpdate.xml"
#define SubPathName_updatefiles             "update"        //原始升级文件在FastTrader下的子目录名
#define UpdateExeName                       "updateexe.exe"
#define MessageTitle                        "FastTrader升级提示"



class Cdoupdate
{
public:
    //MainThreadID: 用于退出FastTrader
    Cdoupdate();
    ~Cdoupdate();
    void start(void);

public:
    //从tcp得到数据后，调用此回调函数进行处理
    static void CB_RcvData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //断开tcp连接后，调用此回调函数进行处理
    static void CB_Disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //连接成功后，调用此回调函数进行处理
    static void CB_ConnSucc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //连接失败后，调用此回调函数进行处理
    static void CB_ConnFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
    static void CB_OnTime(int UserData);


private:
    void stop_Internal(void);

    //从AutoUpdate.xml中取出升级服务器地址及升级定义文件名
    void GetParamInAutoUpdateFile_Internal();

    //发送请求文件的包, 对sendData的一个封装。
    bool SendQryFilePkg_Internal(unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
        unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);
    
    //将服务器传回来的内容写入文件。包体为文件名(\0结尾) + 文件内容。psubpath为写入的子目录，不带反斜杠。
    bool WritePkgToFile_Internal(const char* subpath,const void* pData,int len,string& Out_PathFileName);

    //处理一个有效数据包
    void ProcessPkg_Internal(const Stru_UniPkgHead& head,const void* pData,int len);

	static DWORD ThreadWorker(void *arg);

public:
    //FastTrader的主窗口句柄，用于退出
    HWND m_hMainWnd_FastTrader;
    //是否已经开始登陆。如果已经开始登陆，重启前提示用户确认
    bool m_bStartLogin;

    //doupdate的实例
    static Cdoupdate* m_pObj;


protected:
    Ceasymutex m_mutex;

    //升级服务器地址
    Stru_IPAddress m_SvrAddr;
    //(服务器上的)升级定义文件名，如FastTrader_Update.xml
    string m_UpdateFileNameInSvr;
    //要忽略的版本号
    string m_IgnoreVersion;

    CTcpClientWorker *m_pTcpClient;
    static const int m_RcvBufSize=50*1024*1024;
    int m_RcvDataLen;
    char* m_pRcvData;


    enum eupdatestep
    {
        Step_NotStart,
        Step_GetUpdateFile,
        Step_WaitUserConfirmUpdate,
        Step_DownloadFiles,
        Step_WaitUserConfirmCloseFastTrader,
        Step_Complete,
        Step_Stop
    };

    eupdatestep m_step;

    //服务器下载的update.xml的解析内容
    CParseUpdateXml m_UpdateXml;

    //显示下载内容的窗口
    CPopMsgWin *m_pmsgwin;

    //启动时间，超过3分钟认为超时，退出升级
    time_t      m_StartSeconds;

    static int             m_hThread;
    static unsigned long   m_hThreadID;

    //是否在MessageBox中。析构时用
    bool m_bInMessageBox;
    //是否在关闭中
    bool m_bClosing;

    //发送数据接收数据的时间。用于判断网络超时。超时时间暂设定为60s
    time_t SendTime;
    time_t RecvTime;


    //下面两个方法禁用
	Cdoupdate(const Cdoupdate&);
	Cdoupdate& operator= (const Cdoupdate&);
};
