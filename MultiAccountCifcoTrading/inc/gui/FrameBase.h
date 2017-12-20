#pragma once
#include "stdafx.h"
class zqNCButton;
class CImportExport;
class zqWriteLog;
class FrameBase :
	public wxFrame
{
public:
    //菜单ID
    enum
    {
        ID_QRY_ACCOUNT=98,
		ID_NCBTNID_ENGLISH,
		ID_NCBTNID_CHINESE,
		ID_NCBTNID_TRADITION,
		ID_NCBTNID_VIDEO,
		ID_NCBTNID_LOCK,
		ID_NCBTNID_TOP,
        ID_QRY_SETTLE,
        ID_LOCK_MONITOR,
        ID_MODIFY_PSW,
		ID_MODIFY_MA_PASSWORD,
        ID_LOCK_FRAME,
		ID_READ_MSG,
        ID_TRANSFER,
        ID_TRADE_REPORT,
        ID_TODAY_REPORT,
        ID_WEEK_REPORT,
        ID_MONTH_REPORT,
        //ID_CONFIG_WIZARD,
        ID_STYLE_SAVE,
        ID_CONFIG_LOAD,
        ID_CONFIG_OPTION,
		ID_DEFAULT_CONFIG,
        ID_HELP_CONTENT,
		ID_TIMER,     //定时 查询合约手续费率与保证金率
		ID_PLUSIN,
        ID_LOGIN_AGAIN,
        ID_UPDATE_APP,
        ID_CONFIG_SELECT,  
        ID_CONFIG_ORDER,
        ID_CONFIG_SYSTEM,
        ID_CONFIG_PASSWORD,
        ID_CONFIG_ACCE,
		ID_CONFIG_Gaoji,
		ID_ORDER_STANDARD,
        ID_ORDER_SMALLWIN,
        ID_ORDER_BATCH,
        ID_ORDER_CONDITION,
        ID_ORDER_PROFIT,
        ID_ORDER_STOPLIMIT,
        ID_QUERY_WITHDRAW,
        ID_QUERY_SETTLEMENT,
		ID_QUERY_TRADER,
		ID_QUERY_EXCHANGE,
		ID_QUERY_CONTRACT,
		ID_QUERY_COMMODITY,
		ID_QUERY_ORDERTYPE,
		ID_QUERY_POSITION,
		ID_QUERY_REPORT,
		ID_QUERY_EXCHANGE_RATE,
        ID_TRANSFER_BANK,
        ID_TRANSFER_WITHDRAW,
        ID_HELP_DOC,
        ID_HELP_LOG,
		ID_POS_MOVE,
		ID_POS_SWAP,
		ID_PLUGIN_ITEM_BEGIN,
		ID_PLUGIN_ITEM_END=ID_PLUGIN_ITEM_BEGIN+MAX_PLUGIN_COUNT,
        ID_BASE_LAYOUT,//必须是最后一个
    };
public:
	FrameBase(void);
	~FrameBase(void);
    BOOL EnumTopLevelWin(HWND hwnd);
    void PaintMenuButton(bool bRefresh);
	void DrawCheckBox(wxDC& dc,const wxRect& rect,bool bcheck);
    void OnNcLeftUp(wxPoint pt);
    void OnSize(wxSizeEvent& event);
    void OnButtonClick(wxCommandEvent& event);
public:
	bool SaveCommissionRateAndMarginRate();
    WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	void DoLoginAgain();
    bool CreateMenuBar_Q7();
    bool CreateMenuBar_SunGard();
	void CreateMenuBar_Plugins(wxMenuBar* mb,int pos=-1);
    wxToolBar* CreateToolbar_Begin();
    wxButton* CreateToolbar_Item(wxToolBar *toolBar,wxWindowID id,
             const wxString& label = wxEmptyString,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxButtonNameStr);
    void CreateToolbar_End(wxToolBar *toolBar);
	void ToggleToolbar(bool bshow);
#ifdef _USE_MULTI_LANGUAGE
	void ChangeButtonCaption();
	virtual void ChangeLanguage(int langid);
#endif
#ifdef _USE_TOP_SPLITTER
	virtual void LoadSaveVideoInfoProfile(bool bload);
	virtual void OnToggleViewVideoInfo(bool bCheck);
	wxWindow*  m_right_splitter_wnd;
	bool       m_bShowVideoInfo;
#endif
	void InitializeNCButton();
	void CalcNCButtonOffset();
    std::vector<zqNCButton*> GetBtArray()
	{
		return m_zqNCButtonArray;
	}
	bool m_bTop;  //是否已经置顶
protected:
   //选项向导程序进程ID
    long m_configWizardProcessId;
    //重新登录的进程ID
    long m_ReLoginSelfProcessId;
    //是否强制关闭
    bool m_bForceClose;
    CImportExport* m_pImportExport;
    wxWindow* m_curConfigPanel;
	wxString  m_curConfigPanelClassName;
    zqWriteLog*           m_pWriteLog;
#ifdef _USE_MULTI_LANGUAGE
	int m_languageId;
#endif
private:
    wxStaticText* m_StaticBlank;
    std::vector<wxButton*> m_GroupButtons;
	wxBitmap m_CheckBitmap,m_UncheckBitmap;
	std::vector<zqNCButton*> m_zqNCButtonArray; 
    DECLARE_EVENT_TABLE()

};
