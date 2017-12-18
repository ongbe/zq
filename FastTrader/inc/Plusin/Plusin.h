// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PLUSIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PLUSIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include <map>
#include <set>
#include <queue>
using std::string;
using std::map;

#include "../inc/PlusinCommon/CommonDef.h"
#include "../Module-Misc2/BusinessData.hpp"

typedef wxWindow* (*CreatePlugPane)(int nPos,wxString caption,wxString AppID);
typedef bool (*DeletePlugPane)(int npos, wxString AppID);

struct TargetMsg
{
	HWND hWnd;
	COPYDATASTRUCT msg;
};

struct PLUGWIN2
{
    //插件的运行实例信息
    struct Stru_Instance
    {
        Stru_Instance()
        {
            memset(this,0,sizeof(*this));
        };

	    HWND PluginMsgHWnd;	            //插件接收消息窗口句柄
	    HWND PlusinTopHWnd;	            //插件顶层窗口句柄
	    HWND ContainerHWnd;	            //容器窗口句柄
	    DWORD dwLastUpdateTime;         //最后一次心跳时间，单位为微秒
    };

	PLUGWIN2()
	{
		ShowPos = NEED_NO_POS_NO_BIND;

		strPath = "";

		unMenuPos = 0;
		strMenuName = "";
		strSummary = "";
		strDesc = "";
	};
    PLUGWIN2(const PLUGWIN2& r)
    {
        *this=r;
    };
    PLUGWIN2& operator= (const PLUGWIN2& r)
    {
        if (this == &r)
            return *this;

        ShowPos=r.ShowPos;
        strPath=r.strPath;
        bMultiInstance=r.bMultiInstance;
        bNeedCurrentInstrumentID=r.bNeedCurrentInstrumentID;
        unMenuPos=r.unMenuPos;
        strMenuName=r.strMenuName;
        strSummary=r.strSummary;
        strDesc=r.strDesc;
        m_Instance=r.m_Instance;

        return *this;
    };

	UINT ShowPos;

	string strPath;

    bool bMultiInstance;                //是否可以打开多个实例
    bool bNeedCurrentInstrumentID;      //启动时是否需要将当前合约ID作为参数

	UINT unMenuPos;			            //菜单显示位置
	string strMenuName;                 //菜单显示文字
	string strSummary;	                //概要说明
	string strDesc;	                    //详细说明

    map<HWND,Stru_Instance> m_Instance; //当前实例map, PluginMsgHWnd -> Instance

};

//插件菜单位置定义，按位运算
#define MENU_POS_NONE		0	//插件不需要菜单
#define MENU_POS_MAIN_MENU	1	//主菜单位置
#define MENU_POS_QUOTE		2	//报价表右键菜单位置

class PlusInMgrDlg;
// This class is exported from the Plusin.dll
class CPlusin 
{
public:
	//默认构造函数
	CPlusin(void);
	//默认析构函数
	~CPlusin(void);

public:
	//主界面初始化后加载系统插件
	void LoadPlusin();

	//响应主界面菜单“插件管理”显示对话框
	void ShowPlusinDlg(wxWindow* pParent);
	//初始化插件模块
	bool Init( wxWindow* pParent, CreatePlugPane createFunc, DeletePlugPane deleteFunc );

	void SubscribeBusinessData();

	//卸载插件模块
	void UnInit();

	void UnSubscribeBusinessData();

	//处理从主窗体获取的WM_COPYDATA消息
	BOOL ProcessMessage( WPARAM wParam, COPYDATASTRUCT* pCopyDataStruct );
	//循环检查并释放已不存在的插件
	void ReleasePlusInRC();
	//关闭由窗口句柄指定的插件
	void ClosePlusInProc( HWND hWnd );

	bool RegisterPlusin( const wxString& strPlusinPath, bool bSendMenuChangeMsg );
	//关闭由APPID指定的插件
	bool UnRegisterPlusin( const wxString& strAppID );

	//GUI各模块向插件发送指令消息，hPlusinTopWnd为NULL时，表示给所有插件群发消息，不为NULL时，表示给指定插件发送消息
	bool Send(COPYDATASTRUCT msg, HWND hPlusinTopWnd = NULL);

	void SetOrderModuleCurInstrumentID( const wxString& strInstrumentID);

	bool GetPlusinByAppID( const wxString& strAppID, PLUGWIN2& plusin);

	//获取插件菜单信息
    void GetPlusinMenuInfo( UINT nMenuPos,std::vector<wxString>& AppIDs );
	//启动插件
	void ShowPlusin( const wxString& strAppID );

	bool GetOrderVerifyStatus();
	bool SendNeedVerifyOrder(PlatformStru_InputOrder& Order);

protected:
	//行情到来时的回调函数
	static int CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data);
	//委托单状态到来的回调函数
	static int CallBackFunc_BID_RtnOrder(const GUIModuleID GID,const AbstractBusinessData& data);
	//成交回报到来的回调函数
	static int CallBackFunc_BID_RtnTrade(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询持仓响应的回调函数
	static int CallBackFunc_BID_RspQryInvestorPosition(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询组合持仓响应的回调函数
	static int CallBackFunc_BID_RspQryInvestorComboPositionDetail(const GUIModuleID GID,const AbstractBusinessData& data);
	//报单查询回报
	static int CallBackFunc_BID_RspQryOrder(const GUIModuleID GID,const AbstractBusinessData& data);
	//资金账户查询回报
	static int CallBackFunc_BID_RspQryTradingAccount(const GUIModuleID GID,const AbstractBusinessData& data);
	//撤单回执1
	static int RspOrderAction1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
	//撤单回执2
	static int RspOrderAction2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
	//下单错误回执1
	static int RspOrderInsert1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
	//下单错误回执2
	static int RspOrderInsert2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
    //发送线程
	static DWORD SendThreadFunc(void* pData);

	bool PushIntoMsgQueue(const TargetMsg& targetMsg, int nRequestId);
	void SaveUserPlusIn();
	void CloseAllPlusin();
	// TODO: add your methods here.
	void ReleasePlusInRC(int nPos, const wxString& strAppID );

	BOOL ProcessConsoleMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessConsoleLogon( HWND hWnd, void* pData, int nDataLen, int nRequestId );

	BOOL ProcessAccountMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessInstrumentMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessAllOrderMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessPositionMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessExchangeMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessQuoteMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessOrderMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessConfigMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessInsertOrderVerifyMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessOtherMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );

private:
    //打开一个进程，返回ProcessID。失败时返回0
    //命令行参数为：FastTrader的主窗口句柄 Param
    DWORD OpenPlusInProc( const wxString& strPath, const string& Param );

private:
	CreatePlugPane m_pFnCreatePlusPanel;
	DeletePlugPane m_pFnDeletePlusPanel;

	std::map<wxString, PLUGWIN2> m_mapAppID2Plusin;
	PlusInMgrDlg* m_pPlusInDlg;
	std::set<std::string> m_setSysPlusIn;
	HWND m_pParentHWnd;
	bool m_bInit;
	HANDLE m_hSendThreadHandle;
	bool m_bSendThreadExit;

	std::queue<TargetMsg> m_msgQueue;
	CRITICAL_SECTION m_cs;
	//订阅合约数据集
	typedef std::map<std::string, std::set<HWND>> QUOTE_SUBSCRIBE_MAP;
	QUOTE_SUBSCRIBE_MAP m_quoteSubscribe;

	wxString m_strOrderModuleCurInstrumentID;
	std::set<DWORD> m_setProcessIDWaitForShow;

	bool m_bOrderVerify;	//下单是否需要审核
	HWND m_hMsgWnd_OrderVerify;	//审核插件的消息窗口句柄
};

