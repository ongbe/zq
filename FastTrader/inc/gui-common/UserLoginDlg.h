#pragma once
#include "wx\msw\dialog.h"
#include "../inc/Module-Misc/tinyxml.h"
#include "../inc/Module-VerifyCode/VerifyCode.h"
#include "../inc/Module-VerifyCode/VirtualKeyboard.h"
#include "../inc/Module-Misc/zqControl.h"
#include "updatelib/doupdate.h"
#include <string>
#include <set>
#include <map>
#define INSTRUMENT_FILENAME "Instruments.xml"
//#define INSTRUMENT_FORMAT_STRING "%s,%s,%s,%s,%s,%c,%d,%d,%d,%d,%d,%d,%d,%f,%s,%s,%s,%s,%s,%c,%d,%c,%c,%f,%f,%d,%d,%d,%d,%d,%d"
#define SERVERPARAM_FILENAME "ServerParam.xml"
#define  TIMER_MILLISECONDS    100
#define  TIMER_OVERTIME        1200  
//#define TIMER_ERROR                             -1
#define TIMER_READY                             100
enum _tagInitQry
{
    TIMER_QRYSETTLEMENTINFO,
    TIMER_QRYINSTRUMENT, 
    //TIMER_QRYDEPTHMARKETDATA,
    TIMER_QRYTRADINGACCOUNT, 
    TIMER_QRYTRADE, 
    TIMER_QRYORDER,
    TIMER_QRYINVESTORPOSITIONDETAIL,
    TIMER_QRYINVESTORPOSITION,
    TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL,
	INITQRY_QRYCONTRACTBANK,
    TIMER_FINISH,
};
////////////////////////////////////////////////////////////////////////////
class wxSIPWin;
class zqCheckBox;
class UserLoginDlg :
    public wxDialog
{
public:
enum
{
    ID_GAUGE=100,
    ID_CHOICE,
    ID_BUTTON,
    ID_USERID,
    ID_CHECK1,
    ID_CHECK2,
	ID_CHECK3,
    ID_PROMPT,
    ID_PASSWORD,
	ID_SIPID,
    ID_BUTTON2,
    TIMER_ID,
	ID_VERIFY,
	ID_CHANGE,
	ID_VIRTUALKEYBOARD,
	ID_DYNAMICPASSWORD,
	ID_CHANGE_PWD
};

    UserLoginDlg(wxWindow *parent);
    ~UserLoginDlg(void);
	void ResetAutoLogin(wxString& strUserName, wxString& strPassword);
    void SetPromptText(wxString strText=wxEmptyString);
    void OnCharHook (wxKeyEvent& event);
    void OnOk(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnServerChoice(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnPaint(wxPaintEvent& event);
	void EnableUI(bool bEnable);
	void DoLoginFail(wxString strSrc=wxString(""));
	//void DoLoginSuccess();
    void PreLoginQuery(void);
    void StartLoginQuery(void);
    void EndLoginQuery(void);

    void SetProgressPosition(int TotalStep,int CurStep=-1,int pos=-1);
    static bool LoadSaveServerParam(bool bLoad);
    void SplitwxString(wxString& strLine,std::vector<std::string>& vec);

    //bool LoadTodayInstruments();
	//void SaveTodayInstruments();
    //bool LoadTodayProducts();
	//void SaveTodayProducts();

    //bool LoadCommissionRate(TiXmlElement* pRoot);
	//bool LoadMarginRate(TiXmlElement* pRoot);
	//bool LoadCommissionRateAndMarginRate(wxString strFileName);
	static BOOL MakeConstractIDVarietyMap(map<string, string>& mapConstractIDVariety);
	//void OnQryFinish(IPlatformSingleSvr* pSvr);
#ifdef LOGON_USE_ABOUTMENU
	bool MSWTranslateMessage(WXMSG* pMsg); 
#endif
protected:
	bool NeedVerifyCode();

	void OnChangeVerifyBitmap(wxCommandEvent& event);
	void ShowVirtualKeyboard();
	void OnChildFocus(wxChildFocusEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& evt);
	void OnWindowMove(wxMoveEvent& event);
	void OnChangePwd(wxCommandEvent& event);
	void OnWebCifco(wxCommandEvent& event);
	void OnHideAccount(wxCommandEvent& event);
	void OnSelSvr(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()

private:
	static int QrySettlementInfoCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int QryProductListCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryInstrumentListCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryTradingAccountCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int QryInvestorPositionDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryInvestorPositionCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryInvestorPositionCombineDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
private:
	BOOL m_bIsAutoLogin;
    static wxString m_ErrorMessage;
    wxString m_LastPromptText;
	std::string m_VersionText;
	wxFont   m_VersionFont;
    wxTimer m_timer;
    wxBitmap m_BackBitmap,m_VerifyBitmap;
	int m_cnt;   //登入次数
	//static std::map<std::string, std::set<std::string> > sm_TmpProductID;
	//static std::set<std::string> sm_TmpProductID;
	//static std::set<std::string> sm_TmpInstrumentID;
	VerifyCode m_objVerifyCode;
	VirtualKeyboard * m_pVirtualKeyboard;
	std::vector<zqStaticText *> m_vecStaticText;
protected:
	std::string m_strNewPwd;
	wxString m_strBrokerID,m_strUserID,m_strPassword,m_strDynamicPassword;
private:
    //是否正在进行初始化查询
    static bool m_bInitQry;
    //初始化查询的当前步骤，取值为_tagInitQry枚举值之一
    static int m_InitQryStep;

    //下面变量表示对应的查询是否已经完成
    static bool m_bRspQrySettlementInfo;
    static bool m_bRspQryInstrument;
    static bool m_bRspQryTradingAccount;
    static bool m_bRspQryTrade;
    static bool m_bRspQryOrder;
    static bool m_bRspQryInvestorPositionDetail;
    static bool m_bRspQryInvestorPosition;
    static bool m_bRspQryInvestorPositionCombDetail;

    static UserLoginDlg* m_pInst;
protected:
    Stru_ProxyServerParam m_ProxyParam;

    static bool m_bRlt_OnOK;            //处理OnOK的结果。在多账号里使用
};
