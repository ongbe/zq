#pragma once
#include "FrameBase.h"
class zqAuiNotebook;
class wxAuiNotebookEvent;
class wxAuiManagerEvent;
class CBankFutureTransferDlg;
class DisConnPromptDlg;
class TradingNoticeDlg;
class UserNoticeDlg;
class CLocalOrderService;
class MainTaskBarIcon;
class CTrackThread;
class CBeepThread;
class PlusInMgrDlg;
class NoticeContent;
class LockFrameDlg;
struct LOG_DATA;
struct MessageInfo;
class MsgDlg;

struct USER_LOG
{
	USER_LOG()
	{
		strLogType = "";
		pData = NULL;
	}
	std::string strLogType;
	LOG_DATA* pData;
};
class MainFrame:
    public FrameBase
{
public:
    //标准构造函数,并没有创建窗口
    MainFrame();
	wxString GetMainFrameCaption();
    //初始化界面
    bool Initialize(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);
    //析构界面
    void Uninitialize();

	void RestorePosition();

	//标准析构函数
    ~MainFrame(void);

	//虚函数：禁止状态栏显示菜单提示
     virtual void DoGiveHelp(const wxString& text, bool show);
	 //虚函数：用于布局
	 virtual bool Layout();
	//快期菜单响应:锁定软件
    void OnLockFrame(wxCommandEvent& evt);
	//消息查看
	void OnReadMsg(wxCommandEvent& evt);
	//快期菜单响应:退出
    void OnExit(wxCommandEvent& evt);
	//系统按钮“关闭”的响应
    void OnMainClose(wxCloseEvent& evt);

    //void OnTradeReport(wxCommandEvent& event);
    //void OnTradeReport_Today(wxCommandEvent& event); 
    //void OnTradeReport_Week(wxCommandEvent& event); 
    //void OnTradeReport_Month(wxCommandEvent& event); 
    void ShowConfigAppDlg(int id=-1);
	//快期菜单响应:选项设置
    void OnConfigOption(wxCommandEvent& event);
	//改变配置
	void DoChangeStyle(int cmdid);
	//恢复默认配置
	void OnApplyDefaultConfig(wxCommandEvent& event);
	//发送订阅业务事件
	void SendSubscribeEventToChildren(bool IsSubscribe);
	//发送配置文件改变事件
	void SendCfgXmlChangeEventToChildren();
	//发送风格改变事件
	void SendStyleChangeEventToChildren(int nApplyType);
    //金仕达界面收回下方的配置面板，回到正常页面
	void OnReturnNormalUI(wxCommandEvent& evt);
	void DoReturnNormalUI();
	//传递CHAR_HOOK到指定窗口
	bool TransmitCharHook(wxWindow *panel,wxKeyEvent& event);
	//处理键盘功能键输入
    void OnCharHook (wxKeyEvent& event);
    //浮动面板弹出响应
    void OnFloatPaneOpen(wxAuiNotebookEvent& event);
    //浮动面板收回响应
    void OnFloatPaneClose(wxAuiManagerEvent& event);
	//弹出银期转账对话框
	void OnBankFutureTransfer(wxCommandEvent& event);
	//弹出查询账户对话框
	void OnQueryTrader(wxCommandEvent& event);
	//弹出查询合约对话框
	void OnQueryContract(wxCommandEvent& event);
	//弹出查询交易所对话框
	void OnQueryExchange(wxCommandEvent& event);
	//弹出查询报单类型对话框
	void OnQueryOrderType(wxCommandEvent& event);
	//弹出查询商品对话框
	void OnQueryCommodity(wxCommandEvent& event);
	//弹出查询持仓对话框
	void OnQueryPosition(wxCommandEvent& event);
	//弹出查询报告对话框
	void OnQueryReport(wxCommandEvent& event);
	//弹出查询汇率对话框
	void OnQueryExchangeRate(wxCommandEvent& event);
	//配置文件的保存
    void OnStyleSave(wxCommandEvent& event);
	//界面风格改变
	void OnUpdateUIOnStyleChanged(wxUpdateUIEvent& event);
    void OnStyleChanged(wxCommandEvent& event);
	//快期菜单响应：修改密码
	void ModifyPwd(wxCommandEvent& event);
	//快期菜单响应：查历史结算单
	void HisCal(wxCommandEvent& event);
	//快期菜单响应：使用说明
    void OnHelpContent(wxCommandEvent& event);
	//快期菜单响应：关于
    void OnAbout(wxCommandEvent& event);
	//快期菜单响应：查期货账户资金
    void OnQryAccount(wxCommandEvent& evt);
	//快期菜单响应:设置向导
    //void OnConfigWizard(wxCommandEvent& evt);
	//菜单响应：移仓
    void OnPositionMove(wxCommandEvent& event);
	//菜单响应：换仓
    void OnPositionSwap(wxCommandEvent& event);
	//菜单响应：插件管理
	void OnShowPlusInDlg(wxCommandEvent& event );
  	//合约交易状态通知响应 
	void OnInstrumentStatus(wxCommandEvent& evt);
	//与交易后台通信连接断开事件响应
    void OnFrontDisconnected(wxCommandEvent& evt);
	//与交易所连接，断开和登录响应
	void OnConnectLogin(wxCommandEvent& evt);
	//交易通知事件响应
    void OnTradingNotice(wxCommandEvent& evt);
	//最小化到托盘响应
	void OnIconize(wxIconizeEvent& evt);
	//空闲消息响应
	void OnIdle(wxIdleEvent& event);
    //金仕达菜单响应：重新登录
    void OnLoginAgain(wxCommandEvent& event);
	//金仕达菜单响应：自动升级
    void OnUpdateApp(wxCommandEvent& event);
    //金仕达菜单响应：自选合约
    void OnConfigSelect(wxCommandEvent& event); 
	//金仕达菜单响应：委托参数
    void OnConfigOrder(wxCommandEvent& event);
	//金仕达菜单响应：系统参数
    void OnConfigSystem(wxCommandEvent& event);
	//金仕达菜单响应：修改密码
    void OnConfigPassword(wxCommandEvent& event);
	//金仕达菜单响应：数字快捷键
    void OnConfigAcce(wxCommandEvent& event);
    //金仕达菜单响应：自选合约
    void OnUpdateUIConfigSelect(wxUpdateUIEvent& event); 
	//金仕达菜单响应：委托参数
    void OnUpdateUIConfigOrder(wxUpdateUIEvent& event);
	//金仕达菜单响应：系统参数
    void OnUpdateUIConfigSystem(wxUpdateUIEvent& event);
	//金仕达菜单响应：修改密码
    void OnUpdateUIConfigPassword(wxUpdateUIEvent& event);
	//金仕达菜单响应：数字快捷键
    void OnUpdateUIConfigAcce(wxUpdateUIEvent& event);
	//金仕达菜单响应：高级
	void OnConfigGaoji(wxCommandEvent& event);
	//金仕达菜单响应：标准下单板
	void OnOrderStandard(wxCommandEvent& event);
	//金仕达菜单响应：小窗委托
    void OnOrderSmallWin(wxCommandEvent& event);
	//金仕达菜单响应：批量下单
    void OnOrderBatch(wxCommandEvent& event);
	//金仕达菜单响应：条件单
    void OnOrderCondition(wxCommandEvent& event);
	//金仕达菜单响应：盈损单
    void OnOrderProfit(wxCommandEvent& event);
	//外盘菜单响应：Stop or Stop Limit Order
	void OnOrderStopLimit(wxCommandEvent& event);
	//上期所FAKFOK菜单响应：FAKFOK
	void OnOrderFAKFOK(wxCommandEvent& event);
	//金仕达菜单响应：查询出入金
    void OnQueryWithdraw(wxCommandEvent& event);
	//金仕达菜单响应：查询结算单
    void OnQuerySettlement(wxCommandEvent& event);
	//金仕达菜单响应：银期转账
    void OnTransferBank(wxCommandEvent& event);
	//金仕达菜单响应：预约出金
    void OnTransferWithdraw(wxCommandEvent& event);
    //金仕达右键菜单响应：所有委托单模块
    void CancerlOrder(wxCommandEvent& event);
	//金仕达右键菜单响应：查条件单模块
    void ConditionPanelButton(wxCommandEvent& event);
	//金仕达菜单响应：帮助文档
    void OnHelpDoc(wxCommandEvent& event);
	//金仕达菜单响应：打开日志
    void OnHelpLog(wxCommandEvent& event);
	//获得窗口风格
	//int GetStyle();
	//下单板合约改变消息
	void OnInstrumentIdCHange(wxCommandEvent& event);
    //报价表合约改变消息
    void OnInstrumentIDChangeInQuotPanel(wxCommandEvent& event);
	//发给下单板消息
	void OnPane2OrderPane(wxCommandEvent& event);
	//发给报价表消息
	void OnPane2QuotPane(wxCommandEvent& event);
	//更新与下单板相关的菜单或工具栏
	void OnUpdateUI_Order(wxUpdateUIEvent& event);
	//更新菜单或工具栏
	void OnUpdateUI_PosMove(wxUpdateUIEvent& event);
	//更新菜单或工具栏
	void OnUpdateUI_PosSwap(wxUpdateUIEvent& event);
	//获取配置面板显示状态
	void OnGetConfigPaneStatus(wxCommandEvent& event);
	//载入configapp程序
	void OnLoadConfigApp(wxCommandEvent& event);
//	//获取子面板的显示名称
//	void OnGetPaneCaption(wxCommandEvent& event);
//	//获取全部手续费之和
//	void OnGetAllCommissionRate(wxCommandEvent& event);
	//银期模块转账成功
	void OnSucessTransfer(wxCommandEvent& event);
	//初始化后台查询进度消息响应
	//void OnInitQryStepMain(wxCommandEvent& event);
	//委托模块写状态栏log的响应函数
	void OnWriteUserLog(wxCommandEvent& event);
    //其他模块获取风格类型响应函数
	void OnGetStyle(wxCommandEvent& event);
	//转发单板消息到批量、预埋、条件、盈损单查询面板执行查询
	void OnOrderPanel2BatchQuery(wxCommandEvent& event);
	void OnOrderPanel2ParkedQuery(wxCommandEvent& event);
	void OnOrderPanel2ConditionQuery(wxCommandEvent& event);
	void OnOrderPanel2StopQuery(wxCommandEvent& event);
	//
	void OnSendIEContent(wxCommandEvent& event);
	//配置文件改变,刷新界面
	void OnCfgChanged(wxCommandEvent& event);
	//列表颜色字体改变，刷新界面
	void OnUpdateListTable(wxCommandEvent& event);
	//重新连接到来消息
	void OnRtnReconnected(wxCommandEvent& event);
	//插件要求菜单改变
	void OnPluginMenuChange(wxCommandEvent& event);
	//定时器
	void OnTimer(wxTimerEvent& event);
    //
    void OnUserNotice(wxCommandEvent& event);
	//
	void LoadHypertextNotice(NoticeContent* pContent,bool waitFinished);
	//下单板中买卖五档勾选或者取消勾选消息响应
	void OnBuySell5ShowChanged(wxCommandEvent& evt);
	void OnMassageNotice(wxCommandEvent& evt);
	void OnQuotSetFocus(wxCommandEvent& evt);
	//插件自定义菜单响应
	void OnClickPluginMenu(wxCommandEvent& evt);
	void OnNoticeSound(wxCommandEvent& evt);

    //期权UI：改变底色配色
    void OnOPTIONUI_DarkBackground(wxCommandEvent& event);
    void OnOPTIONUI_LightBackground(wxCommandEvent& event);
	//期权下单相关
	void OnOption_ForQuoteInsert(wxCommandEvent& evt);
	void OnOption_ExecOrder(wxCommandEvent& evt);
	void OnOption_SubsUnsubsForQuote(wxCommandEvent& evt);

#ifdef _USE_MULTI_LANGUAGE
	//语言改变
	void ChangeLanguage(int langid);
	//改变主框架语言
	void ChangeFrameLanguage(int langid);
#endif


protected: 
	//由快捷键选择一个页面
	bool SelectPageFromAccel(int keycode);
    //创建主控件
    bool CreateNotebook(wxWindow * pParent);
#ifdef _USE_TOP_SPLITTER
	//显示隐藏视频资讯
	void OnToggleViewVideoInfo(bool bCheck); 
	//创建主控件右边分割视图
    bool CreateRightSplitter(wxWindow * pParent);
#endif
	//改变配置后重新定义界面属性
	void CustomDefineUI();
	//创建状态栏
	bool CreateMyStatusBar();
    //创建子面板
    //bool  CreateChildPane();
	////设置指定窗口右键菜单类型
 //   void ShowExtListContextMenuStyle(wxWindow* pWin,int style);
	////设置全部窗口右键菜单类型为当前
 //   void ShowExtListContextMenuStyle();
	void FloatPaneOpen();
	//弹出下单板
    void PopupOrderInsertPanel();
    //板块标题显示快捷键
	void SetAliasNameShortCutKey(int id,std::string alias,std::string keycode);
	//重新载入键盘快速下单设置数据和下单倍率数据
	void AsyncHandyOrderMapAndKeyMultiplyVolumeMap();
	//注册热键
	void MyRegisterHotKey();
	//获取报价表当前选择合约
	string  GetQuotCurInstrumentID();

	// 解析控制台传过来的CopyData协议
	void MainFrame::ParseConsoleCopyData(COPYDATASTRUCT* pCopyData);
	void RspDoOrderToConsole(HWND hWnd, int nSeqNum, int nResult);

public:
	//接收windows杂项事件WM_COPYDATA和WM_HOTKEY
    WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	//写状态栏log记录
    void WriteUserLog(string strType, LOG_DATA* pData, string strTime="");
	//获取下单服务线程
	CLocalOrderService* GetLocalOrderService() {
		return m_poLocalOrderService;
	};

	//回调函数
	static bool DeletePlugPane(int npos, wxString AppID);
	static wxWindow* CreatePlugPane(int nPos,wxString caption,wxString AppID);
	void OnLoadPlusIn(wxCommandEvent& event);
	void OnDulplicateKeyPrompt(wxCommandEvent& event);

	UINT GetBeepThreadId();
private:
	CBankFutureTransferDlg* m_pBankFutureTransferDlg;//银期转账对话框
    DisConnPromptDlg*     m_pDisConnPromptDlg;//网络断开提示对话框
    TradingNoticeDlg*     m_pTradingNoticeDlg;//交易通知对话框
	HWND m_hypertextNoticeWnd;//
    DECLARE_EVENT_TABLE()
    //快捷键<窗口ID，按键keycode>    
    std::map<int,int> m_ID2AccelMap;
    //浮动和缩回下单板
    wxString m_LayoutPerspective;
    int      m_FloatPageIndex;
	CLocalOrderService*	m_poLocalOrderService;

	int			m_nDeltaOrderPanelSashPos;
	BOOL		m_bMaxBak_MainFrame;
	wxPoint		m_posBak_MainFrame;
    bool        m_RegisterHotKey;
	std::map<std::string, KeyOrderInfo> m_KeyFuncMap;
//	std::map<std::string, int> m_KeyMultiplyVolumeMap;
	MainTaskBarIcon* m_MainTaskBarIcon;
    //std::map<int,std::string> m_TradingNoticeArray;
	std::vector<std::string> m_vecTradingNotice;
    NoticeContent* m_NoticeContent;
#ifdef _USE_TRACE_THREAD
    CTrackThread*  m_pTrackThread;
#endif
	CBeepThread*  m_pBeepThread;

	wxTimer m_timer;
	LockFrameDlg* m_LockFrameDlg;
	std::string m_OptionCaption;
	std::vector<USER_LOG> m_vLogData;
	//
	long m_CifcoServicesHallProcessId;	
public:	
	LPMAIN_WINDOW_CFG m_MainFrameCfg;
	//合约交易状态通知回调函数
    static int InstrumentStatusCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);  
	//与交易后台通信连接断开回调函数
    static int FrontDisconnectedCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data); 
    //与交易后台通信连接成功回调函数
	static int FrontConnectedCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//用户登录
	static int RspUserLoginCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//交易通知回调函数
    static int TradingNoticeCallFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//
	static int AlertMessageCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
    //
	static int RspQryNoticeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//
	static int RspQrySettlementInfoConfirmCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);

	//回调函数
	static bool NotebookAddPage(int id,wxWindow** win,wxString& caption);

	static void DoStyleChanged(int nApplyType);

public:
    //获取子面板的显示名称
    string GetPanelCaption(int PanelID,int type);
	string GetCifcoServicesHallLocation();
	void LoadCifcoServicesHallApp();
	bool LoadExDll( std::string strpath );
};
class ShowMgr
{
public:
    ShowMgr(MainFrame* pParent):m_pParent(pParent)
    {
       if(m_pParent&&m_pParent->IsShown()) 
       {
			m_pParent->Hide();
       }
    }
    ~ShowMgr()
    {
       if(m_pParent&&(!m_pParent->IsShown()))
       {
           m_pParent->Show();
       }
    }
private:
    MainFrame* m_pParent;
};