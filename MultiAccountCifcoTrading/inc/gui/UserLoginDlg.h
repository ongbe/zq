#pragma once
#include "wx\msw\dialog.h"
#include "../inc/Module-Misc/tinyxml.h"
#include "../inc/Module-VerifyCode/VerifyCode.h"
#include "../inc/Module-VerifyCode/VirtualKeyboard.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/MultiAccountCore/CTPLogin.h"
#include <string>
#include <set>
#include <map>
////////////////////////////////////////////////////////////////////////////
class wxSIPWin;
class zqCheckBox;
class CTPLogin;
class UserLoginDlg :
    public wxDialog,public LoginCallback
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
public:
    UserLoginDlg(wxWindow *parent);
    ~UserLoginDlg(void);
    void SetPromptText(IPlatformSingleSvr* pSvr,wxString strText=wxEmptyString);
    void OnCharHook (wxKeyEvent& event);
    void OnOk(wxCommandEvent& event);
	void LoginCTP();
    void LoginMultiAccount(const wxString& strTradeFrontAddr);
	void OnExit(wxCommandEvent& event);
    void OnServerChoice(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
	void EnableUI(bool bEnable);
	void DoLoginFail(IPlatformSingleSvr* pSvr,wxString strSrc=wxEmptyString);
	void DoLoginSuccess();

    void SetProgressPosition(IPlatformSingleSvr* pSvr,int TotalStep,int CurStep=-1,int pos=-1);
    void SplitwxString(wxString& strLine,std::vector<std::string>& vec);
	void SetOneInstrument(wxString& strLine);
	bool LoadTodayInstruments();

	void SaveTodayInstruments();
	bool LoadCommissionRate(TiXmlElement* pRoot);
	bool LoadMarginRate(TiXmlElement* pRoot);
	bool LoadCommissionRateAndMarginRate(wxString strFileName);
	BOOL MakeConstractIDVarietyMap(map<string, string>& mapConstractIDVariety);
	void OnQryFinish(IPlatformSingleSvr* pSvr);
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
	void OnSelSvr(wxCommandEvent& event);
	void OnMASocket(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()

private:
    static wxString m_ErrorMessage;
    wxString m_LastPromptText;
	std::string m_VersionText;
	wxFont   m_VersionFont;
    wxBitmap m_BackBitmap,m_VerifyBitmap;
	int m_cnt;   //µÇÈë´ÎÊý
	//static std::map<std::string, std::set<std::string> > sm_TmpProductID;
	//static std::set<std::string> sm_TmpProductID;
	//static std::set<std::string> sm_TmpInstrumentID;
	VerifyCode m_objVerifyCode;
	VirtualKeyboard * m_pVirtualKeyboard;
	std::vector<zqStaticText *> m_vecStaticText;
	std::string m_strNewPwd;
	wxString m_strBrokerID,m_strUserID,m_strPassword,m_strDynamicPassword;
private:
	CTPLogin* m_LoginCore;
};
