#include "StdAfx.h"
#include "zq.h"
#include "UserLoginDlg.h"
#include "MainFrame.h"
#include "AboutDlg.h"
#include "ChangePWDDlg.h"
#include "ProxySvrSetupDlg.h"
#include "..\inc\cfgMgr\CfgMgr.h"
#include "../../../miscellaneous/Tools/inc/Tools_Ansi.h"
#include "wx/file.h"
#include "wx/textfile.h"
#include <wx/settings.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/GlobalConfigMgr.h"
#include "../inc/Module-Misc/MD5.h"
#include "../inc/MultiAccountCore/MAConnSocket.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#include "../inc/Module-Misc/CryptoAPI.h"
#define  _USE_UGLY_SEL_SVR_TYPE
//错误信息
wxString UserLoginDlg::m_ErrorMessage;
UserLoginDlg* g_pUserLoginDlg=NULL;
UserLoginDlg::UserLoginDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(550,350),wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX),
m_cnt(0),m_pVirtualKeyboard(NULL), m_strNewPwd(""),m_VersionText(""),m_LoginCore(NULL)
{
	g_pUserLoginDlg=this;
	//解决FASTTRADER-1349，设置登录窗口和结算确认窗口置顶
	if(wxGetApp().argc>1)
	{
		HWND wHandle = (HWND)atoi(wxGetApp().argv[1]);
		if(::GetWindowLong(wHandle,GWL_EXSTYLE)&WS_EX_TOPMOST)
			SetWindowStyle(GetWindowStyle()|wxSTAY_ON_TOP);	
	}
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();
	GET_JPG_FILE_RESOURCE("IDR_XML_LOGIN",_T("XML"),m_BackBitmap);
	if(DEFAULT_SVR())
		m_VersionText=LOADSTRING_TEXT(DEFAULT_SVR()->GetVersionIconString());
	m_VersionFont.Create(14, wxDEFAULT, wxNORMAL, wxNORMAL, FALSE,LOADSTRING(USERLOGINDLG_VERSTR_FONT));
	m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();

	SetIcon(wxIcon(_T("zqICON_MAIN")));
#ifdef LOGON_USE_ABOUTMENU
	HMENU hSystemMenu = ::GetSystemMenu((HWND__ *) GetHWND(),FALSE); 
	::AppendMenu(hSystemMenu,MF_SEPARATOR,0,""); 
	::AppendMenu(hSystemMenu,MF_STRING,wxID_ABOUT,ID_ABOUT_CAPTION); 
	::DrawMenuBar((HWND__ *) GetHWND()); 
#endif
	/////////////////////////////////////////////////////////////////////////////////////////
	this->SetLabel(LOADSTRING(USERLOGINDLG_TITLE));
	int currentindex=DEFAULT_SVR()?DEFAULT_SVR()->GetServerParamCfg().current:0;
	if(currentindex<0||DEFAULT_SVR()&&currentindex>=(int)DEFAULT_SVR()->GetServerParamCfg().vec.size()) currentindex=0;
	wxArrayString itemChoiceStrings;
	if(DEFAULT_SVR())
	{
		for(int i=0;i<(int)DEFAULT_SVR()->GetServerParamCfg().vec.size();i++)
		{
			itemChoiceStrings.Add(LOADSTRING_TEXT(DEFAULT_SVR()->GetServerParamCfg().vec[i].ServerName));
		}
	}
	itemChoiceStrings.Add(LOADSTRING(PROXY_SVR_SETUP));

	std::string strAccount=DEFAULT_SVR()?DEFAULT_SVR()->GetServerParamCfg().vec[currentindex].InvestorID:"";
	wxSize centercolumnsize(160,20);
	wxChoice* itemChoice5 = new wxChoice( this, ID_CHOICE, wxPoint(245,133), centercolumnsize, itemChoiceStrings, 0 );
	itemChoice5->SetSelection(currentindex>=0?currentindex:0);
	//itemChoice5->SetClientObject(0,new wxStringClientData(CTPSERVER_FRONTADDR));

	m_vecStaticText.push_back( new zqStaticText( this, wxID_STATIC, LOADSTRING(USERLOGINDLG_SERVER),wxPoint(180,133+3), wxSize(60,18), 0 )); 
	m_vecStaticText.push_back( new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_ACCOUNT),wxPoint(180,160+3), wxSize(60,18), 0 ));
	zqStaticText *pstSaveAccount=NULL,*pstWebCifco=NULL;
	pstSaveAccount=  new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_SAVEACCOUNT), wxPoint(335+15,240), wxSize(56,18), 0 );
	m_vecStaticText.push_back(pstSaveAccount);
	if(GlobalConfigManager::IsCifcoServicesHall_ExtraFunction())
	{
		pstWebCifco =  new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_WEBCIFCO), wxPoint(410+15,240), wxSize(56,18), 0 );
		m_vecStaticText.push_back(pstWebCifco);
	}
	m_vecStaticText.push_back( new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_PASSWORD),wxPoint(180,185+3), wxSize(60,18), 0 ));
	m_vecStaticText.push_back( new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_VERIFICATION), wxPoint(180,210+3), wxSize(60,18), 0 ));
	m_vecStaticText.push_back( new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_DYNAMICPWD), wxPoint(180,235+3), wxSize(60,18), 0 ));
	m_vecStaticText.push_back( new zqStaticText( this, wxID_STATIC,LOADSTRING(USERLOGINDLG_PROGRESS), wxPoint(180,275), wxSize(60,18), 0 ));

	wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( this, ID_USERID,strAccount, wxPoint(245,160), centercolumnsize, 0 );
	wxCheckBox* cbSaveAccount= new wxCheckBox( this, ID_CHECK1, wxEmptyString, wxPoint(335,240), wxSize(14,14), 0 );
	cbSaveAccount->SetValue(strAccount.length()>0);
	pstSaveAccount->SetCheck(cbSaveAccount);
	if(GlobalConfigManager::IsCifcoServicesHall_ExtraFunction())
	{
		wxCheckBox* cbWebCifco= new wxCheckBox( this, ID_CHECK2, wxEmptyString, wxPoint(410,240), wxSize(14,14), 0 );
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
		if(pCfgMgr&&pCfgMgr->GetSystemParamInfo())
		{
			cbWebCifco->SetValue(pCfgMgr->GetSystemParamInfo()->bWebCifco);
		}
		pstWebCifco->SetCheck(cbWebCifco);
	}
	wxTextCtrl* itemTextCtrl11 = NULL;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->HaveModifyPwdOnLongon()) 
	{
		itemTextCtrl11 = new wxTextCtrl( this, ID_PASSWORD, wxEmptyString, wxPoint(245,185), wxSize(134,20), wxTE_PASSWORD );
		new wxButton( this, ID_CHANGE_PWD, "...", wxPoint(380, 185), wxSize(25, 20));
	}
	else
	{
		itemTextCtrl11 = new wxTextCtrl( this, ID_PASSWORD, wxEmptyString, wxPoint(245,185), centercolumnsize, wxTE_PASSWORD );
	}
	if(strAccount.empty())	
		itemTextCtrl8->SetFocus();
	else
		itemTextCtrl11->SetFocus();
	wxTextCtrl* itemTextCtrlVerify = new wxTextCtrl( this, ID_VERIFY, wxEmptyString, wxPoint(245,210), wxSize(80,20), 0 );
	itemTextCtrlVerify->SetMaxLength(4);
	wxTextCtrl* itemTextCtrlDynamicPassword  = new wxTextCtrl( this, ID_DYNAMICPASSWORD, wxEmptyString, wxPoint(245,235), wxSize(80,20),  wxTE_PASSWORD  );

	wxButton* itemButton14 = new wxButton( this, wxID_OK,LOADSTRING(USERLOGINDLG_LOGIN), wxPoint(420,160), wxSize(60,20), 0 );
	wxButton* itemButton15 = new wxButton( this, wxID_CANCEL,LOADSTRING(USERLOGINDLG_EXIT), wxPoint(420,185), wxSize(60,20), 0 );  
#ifdef _USE_UGLY_SEL_SVR_TYPE
	string strSelSvr=LOADSTRING(USERLOGINDLG_SERVER);
	EXCLUDE_AND_SIGNAL(strSelSvr,':');
	wxButton* btnSelSvr = new wxButton( this, ID_BUTTON2,strSelSvr, wxPoint(420,135), wxSize(60,20), 0 );
	itemButton14->SetPosition(wxPoint(420,185));
	itemButton15->SetPosition(wxPoint(420,210));
#endif
	itemButton14->SetDefault();
	wxString strBlank(' ',30);
	new zqStaticText( this, ID_PROMPT,strBlank, wxPoint(245,258), wxSize(243,14),wxALIGN_RIGHT|wxST_NO_AUTORESIZE);

	wxGauge* itemGauge = new wxGauge(this, ID_GAUGE, 1000, wxPoint(245,275), wxSize(243,16), wxGA_HORIZONTAL );
	itemGauge->SetValue(0);

	m_pVirtualKeyboard = new VirtualKeyboard(this,itemTextCtrl11);

	if(!NeedVerifyCode()) {
		itemTextCtrlVerify->Enable(false);
	}
}
#ifdef LOGON_USE_ABOUTMENU
bool UserLoginDlg::MSWTranslateMessage(WXMSG* pMsg) 
{ 
	if((pMsg->message == WM_SYSCOMMAND) && (pMsg->wParam == wxID_ABOUT)) 
	{ 
		AboutDlg dlg(this);
		dlg.ShowModal();
		return true; // Message processed 
	} 
	else 
		return wxDialog::MSWTranslateMessage(pMsg); 
} 
#endif
UserLoginDlg::~UserLoginDlg(void)
{ 


	if (m_pVirtualKeyboard != NULL) 
	{
		if (m_pVirtualKeyboard->IsShown())
		{
			m_pVirtualKeyboard->Hide();
		}
		m_pVirtualKeyboard->Destroy();
		m_pVirtualKeyboard = NULL;
	}
	CMAConnSocket::GetInstance()->UnRegisteWindow(this);
	//wxDELETE(m_zqCheckBox);
	//LoadSaveServerParam(false);
}
BEGIN_EVENT_TABLE(UserLoginDlg,wxDialog)
EVT_BUTTON(wxID_OK,UserLoginDlg::OnOk)
EVT_PAINT(UserLoginDlg::OnPaint)
EVT_BUTTON(wxID_CANCEL,UserLoginDlg::OnExit)
EVT_CHAR_HOOK (UserLoginDlg::OnCharHook)
EVT_CHOICE(ID_CHOICE,OnServerChoice)
EVT_LEFT_UP(UserLoginDlg::OnLeftUp)
//EVT_BUTTON(ID_CHANGE,UserLoginDlg::OnChangeVerifyBitmap)
EVT_LEFT_DOWN(UserLoginDlg::OnLeftDown)
EVT_MOTION(UserLoginDlg::OnMotion)
EVT_MOVE(UserLoginDlg::OnWindowMove)
//EVT_BUTTON(ID_VIRTUALKEYBOARD,UserLoginDlg::OnVirtualKeyboard)
EVT_CHILD_FOCUS(UserLoginDlg::OnChildFocus)
EVT_ERASE_BACKGROUND(UserLoginDlg::OnEraseBackground)
EVT_BUTTON(ID_CHANGE_PWD, OnChangePwd)
EVT_CHECKBOX(ID_CHECK2,OnWebCifco)
EVT_BUTTON(ID_BUTTON2,OnSelSvr)
EVT_COMMAND(wxID_ANY,wxEVT_MA_SOCKET,OnMASocket)
END_EVENT_TABLE()
void UserLoginDlg::SetPromptText(IPlatformSingleSvr* pSvr,wxString strText)
{
	wxWindow* window=FindWindowById(ID_PROMPT,this);
	//    window->SetLabel(wxString(""));
	window->Show(false);
	window->SetLabel(strText);
	window->Show(true);
	WXLOG_INFO(strText.c_str());
}
void UserLoginDlg::OnCharHook (wxKeyEvent& event)
{
	wxWindow *win = FindFocus ();
	if (win == NULL)
	{
		event.Skip ();
		return;
	}
	if (event.GetKeyCode () == WXK_TAB)
	{
		bool backward = event.ShiftDown ();

		wxWindowID id = win->GetId();
		switch (win->GetId ()) 
		{
		case ID_USERID:
			if (backward)
				FindWindowById(ID_CHOICE,this)->SetFocus();
			else
				FindWindowById(ID_PASSWORD,this)->SetFocus();			
			return;
		case ID_PASSWORD:
			if (backward)
				FindWindowById(ID_USERID,this)->SetFocus();
			else
				FindWindowById(ID_VERIFY,this)->SetFocus();			
			return;
		case ID_VERIFY:
			if (backward)
				FindWindowById(ID_PASSWORD,this)->SetFocus();
			else
				FindWindowById(ID_DYNAMICPASSWORD,this)->SetFocus();			
			return;
		case ID_DYNAMICPASSWORD:
			if (backward)
				FindWindowById(ID_VERIFY,this)->SetFocus();
			else
				FindWindowById(wxID_OK,this)->SetFocus();			
			return; 
		case wxID_OK:
			if (backward)
				FindWindowById(ID_DYNAMICPASSWORD,this)->SetFocus();
			else
				FindWindowById(wxID_CANCEL,this)->SetFocus();			
			return; 
		case wxID_CANCEL:
			if (backward)
				FindWindowById(wxID_OK,this)->SetFocus();
			else
				FindWindowById(ID_CHOICE,this)->SetFocus();			
			return; 
		case ID_CHOICE:
			if (backward)
				FindWindowById(wxID_CANCEL,this)->SetFocus();
			else
				FindWindowById(ID_USERID,this)->SetFocus();			
			return; 
		}
	}
	event.Skip ();
}
void UserLoginDlg::OnServerChoice(wxCommandEvent& event)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(event.GetId(),this);
	int currentselection=pChoice->GetCurrentSelection();
	bool bProxySvrSetup=(currentselection==pChoice->GetCount()-1);
	if(currentselection>=0&&!bProxySvrSetup)
	{
		wxString strInvestorID=DEFAULT_SVR()?DEFAULT_SVR()->GetServerParamCfg().vec[currentselection].InvestorID.c_str():"";
		((wxTextCtrl*)FindWindowById(ID_USERID,this))->SetValue(strInvestorID);
		((wxCheckBox*)FindWindowById(ID_CHECK1,this))->SetValue(!strInvestorID.IsEmpty());
	}
	else if(bProxySvrSetup)//代理及服务器设置
	{
		CProxySvrSetupDlg dlg;
		dlg.ShowModal();
		wxChoice* pChoice=(wxChoice*)FindWindowById(ID_CHOICE,this);
		pChoice->Clear();
		if(DEFAULT_SVR())
		{
			for(int i=0;i<(int)DEFAULT_SVR()->GetServerParamCfg().vec.size();i++)
			{
				pChoice->AppendString(LOADSTRING_TEXT(DEFAULT_SVR()->GetServerParamCfg().vec[i].ServerName));
			}
		}
		pChoice->AppendString(LOADSTRING(PROXY_SVR_SETUP));
		pChoice->SetSelection(0);
	}
}

void UserLoginDlg::OnChangeVerifyBitmap(wxCommandEvent& event)
{
	m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();
	Refresh();
}

void UserLoginDlg::ShowVirtualKeyboard()
{

	if (m_pVirtualKeyboard == NULL)
	{
		return;
	}

	wxPoint loginDlgPoint = GetPosition();
	wxSize loginDlgSize = GetSize();

	int vkXPoint = loginDlgPoint.x + 3*loginDlgSize.GetWidth()/4 - 5;
	int vkYPoint = loginDlgPoint.y + 3*loginDlgSize.GetHeight()/4 - 25;

	m_pVirtualKeyboard->Move(vkXPoint, vkYPoint);

	if (!m_pVirtualKeyboard->IsShown())
	{
		m_pVirtualKeyboard->Show();
		FindWindowById(ID_PASSWORD,this)->SetFocus();
	}


}

bool UserLoginDlg::NeedVerifyCode()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(NULL == pMgr) {
		return true;
	}
	LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
	if(NULL == pMainWindowCfg) {
		return true;
	}
	return (pMainWindowCfg->nLogonVerify)>0;
}

void UserLoginDlg::OnOk(wxCommandEvent& event)
{
	//获得m_strBrokerID
	wxChoice* pChoice=(wxChoice*)FindWindowById(ID_CHOICE,this);
	int currentselection=pChoice->GetCurrentSelection();
	if(currentselection<0||!DEFAULT_SVR()||
		currentselection>=(int)DEFAULT_SVR()->GetServerParamCfg().vec.size()) return ;
	DEFAULT_SVR()->GetServerParamCfg().current=currentselection;
	SERVER_PARAM_CFG& thisServerParamCfg=DEFAULT_SVR()->GetServerParamCfg();
	m_strBrokerID=thisServerParamCfg.vec[currentselection].BrokerID.c_str();
	m_strBrokerID.Trim(true).Trim(false); 
	//m_strUserID，m_strPassword，m_strDynamicPassword
	wxString strNewAccount,strVerifyCode;
	if(((wxCheckBox*)FindWindowById(ID_CHECK1,this))->GetValue())
	{
		strNewAccount=((wxTextCtrl*)FindWindowById(ID_USERID,this))->GetValue();
	}
	thisServerParamCfg.vec[currentselection].InvestorID=strNewAccount;
	m_strUserID=((wxTextCtrl*)FindWindowById(ID_USERID,this))->GetValue();
	m_strPassword=((wxTextCtrl*)FindWindowById(ID_PASSWORD,this))->GetValue();  
	strVerifyCode = ((wxTextCtrl*)FindWindowById(ID_VERIFY,this))->GetValue();
	m_strDynamicPassword=((wxTextCtrl*)FindWindowById(ID_DYNAMICPASSWORD,this))->GetValue();
	if(m_strUserID.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(USERLOGINDLG_INPUTACCOUNT) ,LOADSTRING(USERLOGINDLG_INFORMATION));
		m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();
		Refresh();
		return;
	}
	if(strVerifyCode.Trim().IsEmpty()&&NeedVerifyCode())
	{
		wxMessageBox(LOADSTRING(OLS_PLEASE_INPUT_VERIFICATION) ,LOADSTRING(USERLOGINDLG_INFORMATION));
		m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();
		Refresh();
		return;
	}
	if(!m_objVerifyCode.Verify(strVerifyCode)&&NeedVerifyCode())
	{
		wxMessageBox(LOADSTRING(USERLOGINDLG_VERIFYERROR),LOADSTRING(USERLOGINDLG_INFORMATION));
		m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();
		Refresh();
		return;
	}
	if(wxGetApp().argc>1)
	{
		HWND wHandle = (HWND)atoi(wxGetApp().argv[1]);
		::SendMessage(wHandle,WM_CLOSE,GetCurrentProcessId(),0);
	}
	EnableUI(false);
	if(!m_strBrokerID.empty()&&atoi(m_strBrokerID.c_str())>0)
	{
		SAFE_DELETE(m_LoginCore);
		m_LoginCore=new CTPLogin(DEFAULT_SVR(),this);
		m_LoginCore->SetParticipantInfo(m_strBrokerID.c_str(), m_strUserID.c_str(), 
			m_strPassword.c_str(),m_strDynamicPassword.c_str());
		m_LoginCore->SetTradeFrontAddrs(thisServerParamCfg.vec[currentselection].TradeAddr);
		m_LoginCore->SetQuotFrontAddrs(thisServerParamCfg.vec[currentselection].QuotAddr);
		m_LoginCore->SetProxyConnectStr(thisServerParamCfg.ProxyType,thisServerParamCfg.ProxyUser,
			thisServerParamCfg.ProxyPwd,thisServerParamCfg.ProxyAddr,thisServerParamCfg.ProxyPort);
		m_LoginCore->Login(m_strUserID.c_str(), m_strPassword.c_str(), m_strNewPwd);
		LoginCTP();
	}
	else
	{
		DEFAULT_SVR()->SetParticipantInfo(m_strBrokerID.c_str(), m_strUserID.c_str(), 
			m_strPassword.c_str(),m_strDynamicPassword.c_str());
		wxString strTradeFrontAddr;
		strTradeFrontAddr=thisServerParamCfg.vec[currentselection].TradeAddr[0].c_str();
		strTradeFrontAddr.Trim(true).Trim(false);
		LoginMultiAccount(strTradeFrontAddr);
	}
}
void UserLoginDlg::LoginCTP()
{
	/***********************************************************************************************/
	m_strNewPwd.clear();
	if(DEFAULT_SVR()&&DEFAULT_SVR()->ContractCountIsFit())
		SetPromptText(DEFAULT_SVR(),LOADSTRING(USERLOGINDLG_LOGINING));
	else
		SetPromptText(DEFAULT_SVR(),LOADSTRING(USERLOGINDLG_DLDOWNLOADING));
	//发起查询
	PlatformStru_QryNotice notice;
	memset(&notice,0,sizeof(notice));
	strncpy(notice.Thost.BrokerID,m_strBrokerID.c_str(),sizeof(notice.Thost.BrokerID)-1);
	if(DEFAULT_SVR()) DEFAULT_SVR()->ReqQryNotice(notice);

}
void UserLoginDlg::LoginMultiAccount(const wxString& strTradeFrontAddr)
{
	DWORD dwServAddr[2];
	CTools_Ansi::PerformWSAStartup();
	CTools_Ansi::GetIPFromHostName((char*)strTradeFrontAddr.BeforeLast(':').c_str(),dwServAddr,
		2);
	CTools_Ansi::PerformWSACleanup();
	int Port = atoi(strTradeFrontAddr.AfterLast(':').c_str());
	CMAConnSocket::GetInstance()->RegisteWindow(this);
	CMAConnSocket::GetInstance()->Connect(dwServAddr[0],Port);
}
void UserLoginDlg::EnableUI(bool bEnable)
{
	int ctrlID=0;
	wxWindowList& winlist=GetChildren();
	wxWindowList::compatibility_iterator node = winlist.GetFirst();
	while (node)
	{
		wxWindow *current = node->GetData();
		ctrlID=current->GetId();
		if(ctrlID!=wxID_CANCEL&&ctrlID!=ID_GAUGE&&!current->IsKindOf(CLASSINFO(zqStaticText)))
			current->Enable(bEnable);
		node = node->GetNext();
	}  
}
void UserLoginDlg::OnExit(wxCommandEvent& event)
{
	wxDialog::EndDialog(wxID_CANCEL);
}
void UserLoginDlg::DoLoginFail(IPlatformSingleSvr* pSvr,wxString strSrc)
{
	if(!strSrc.IsEmpty())	m_ErrorMessage=strSrc;
	m_cnt=0;
	if(DEFAULT_SVR()) DEFAULT_SVR()->Logout();
	SetPromptText(pSvr);
	((wxTextCtrl*)FindWindowById(ID_PASSWORD,this))->SetValue(""); 
	((wxTextCtrl*)FindWindowById(ID_VERIFY,this))->SetValue("");
	EnableUI(true);

	wxWindow* pWin = FindWindowById(ID_VERIFY);
	if(!NeedVerifyCode() && NULL != pWin) 
	{
		pWin->Enable(false);
	}

	int ErrorID;
	string strErrorMsg;
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetErrorField(ErrorID);
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetErrorField(strErrorMsg);
	if(m_ErrorMessage.empty()&&ErrorID!=0) 
	{
		m_ErrorMessage=strErrorMsg.c_str();
	}
	if(m_ErrorMessage.empty()) 
		m_ErrorMessage=LOADSTRING(USERLOGINDLG_FAILED);
	wxMessageBox(m_ErrorMessage,LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_ERROR);

	SetProgressPosition(pSvr,TIMER_FINISH);



	m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();
	Refresh();
	if(m_ErrorMessage.StartsWith("Password has expired"))
	{
		wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_CHANGE_PWD);
		AddPendingEvent(event);
	}
	m_ErrorMessage.Empty();

	if(DEFAULT_SVR())
		DEFAULT_SVR()->NotifyStartStopInitQry(false);      //通知底层，初始化查询结束
}

void UserLoginDlg::SetProgressPosition(IPlatformSingleSvr* pSvr,int TotalStep,int CurStep,int pos)
{
	wxGauge* itemGauge=(wxGauge*)FindWindowById(ID_GAUGE,this);	
	if(CurStep<0)
	{
		itemGauge->SetValue(CurStep);
		return;
	}
	if(DEFAULT_SVR()&&DEFAULT_SVR()->ContractCountIsFit())	
	{
		if(TotalStep==0) return;


		const int OldValue=itemGauge->GetValue();
		const int Range=itemGauge->GetRange();
		int MinValue=CurStep*Range/TotalStep;
		int MaxValue=(CurStep+1)*Range/TotalStep;
		int NewValue;
		if(MaxValue>Range) MaxValue=Range;

		if(pos>=0) NewValue=MinValue+pos;
		else NewValue=OldValue+2;
		if(NewValue<OldValue) NewValue=OldValue;
		if(NewValue<MinValue) NewValue=MinValue;
		if(NewValue>MaxValue) NewValue=MaxValue;

		if(NewValue!=OldValue) 
			itemGauge->SetValue(NewValue);
	}
	else
	{
		itemGauge->Pulse();
	}
}
void UserLoginDlg::OnPaint(wxPaintEvent& event)
{
	std::vector<zqStaticText *>::iterator iter = m_vecStaticText.begin();
	for (; iter != m_vecStaticText.end(); ++iter)
	{
		zqStaticText* pItem = *iter;
		if (pItem)
		{
			pItem->Refresh();
		}
	}

	if(m_BackBitmap.IsOk())
	{
		wxPaintDC dc(this);
		dc.DrawBitmap(m_BackBitmap,0,0);
		//dc.DrawBitmap(m_TypeBitmap,446,60);
		dc.SetTextForeground(*wxWHITE);
		dc.SetFont(m_VersionFont);
		dc.DrawLabel(m_VersionText,wxRect(442,52,80,17));
		dc.DrawBitmap(m_VerifyBitmap,335,210);		
	}
	else
		event.Skip();
}
void UserLoginDlg::OnLeftUp(wxMouseEvent& event)
{
	// _("保存账户")
	wxCheckBox* pCheckBox=wxDynamicCast(FindWindowById(ID_CHECK1,this),wxCheckBox);
	if(pCheckBox&&pCheckBox->IsEnabled())
	{
		wxRect textrt,checkrt=pCheckBox->GetRect();
		textrt.x=checkrt.GetRight();
		textrt.y=checkrt.y;
		textrt.width=50;
		textrt.height=checkrt.height;
		if(textrt.Contains(event.GetPosition()))
		{
			pCheckBox->SetValue(!pCheckBox->GetValue());
		}
	}

	//点击虚拟键盘外关闭虚拟键盘
	wxRect VKRect = m_pVirtualKeyboard->GetRect();
	if ( (!VKRect.Contains(event.GetPosition()) || m_pVirtualKeyboard != NULL) && m_pVirtualKeyboard->IsShown() )
	{
		m_pVirtualKeyboard->Show(false);
	}


}

void UserLoginDlg::OnChildFocus(wxChildFocusEvent& event)
{
	wxWindow * pPassword = this->FindWindow(ID_PASSWORD);

	if (pPassword != NULL && event.GetWindow() == pPassword && m_pVirtualKeyboard != NULL  && !m_pVirtualKeyboard->IsShown())
	{
		ShowVirtualKeyboard();
		return;
	}

	wxWindow * pVerify = this->FindWindow(ID_VERIFY);

	if ( pVerify != NULL && event.GetWindow() == pVerify && m_pVirtualKeyboard != NULL && m_pVirtualKeyboard->IsShown() )
	{
		m_pVirtualKeyboard->Show(false);
		return;
	}

	wxWindow * pUser = this->FindWindow(ID_USERID);

	if (pUser != NULL && event.GetWindow() == pUser && m_pVirtualKeyboard != NULL  && m_pVirtualKeyboard->IsShown())
	{
		m_pVirtualKeyboard->Show(false);
		return;
	}

	wxWindow * pDynamicPassword = this->FindWindow(ID_DYNAMICPASSWORD);

	if (pDynamicPassword != NULL && event.GetWindow() == pDynamicPassword && m_pVirtualKeyboard != NULL  && m_pVirtualKeyboard->IsShown())
	{
		m_pVirtualKeyboard->Show(false);
		return;
	}

	wxWindow * pButtonOK = this->FindWindow(wxID_OK);

	if (pButtonOK != NULL && event.GetWindow() == pButtonOK && m_pVirtualKeyboard != NULL  && m_pVirtualKeyboard->IsShown())
	{
		m_pVirtualKeyboard->Show(false);
		return;
	}

}

void UserLoginDlg::OnLeftDown(wxMouseEvent& event)
{
	if(FindWindowById(ID_VERIFY,this)->IsEnabled())
	{
		wxRect verifyRect(335,210,m_VerifyBitmap.GetWidth(),m_VerifyBitmap.GetHeight());
		if (verifyRect.Contains(event.GetPosition()))
		{
			m_VerifyBitmap = m_objVerifyCode.GetVerifyCodeBitmap();
			Refresh();
		}
	}
}

void UserLoginDlg::OnEraseBackground(wxEraseEvent& event)
{

}

void UserLoginDlg::OnMotion(wxMouseEvent& evt)
{
	evt.Skip();
}

void UserLoginDlg::OnWindowMove(wxMoveEvent& event)
{
	if (m_pVirtualKeyboard != NULL && m_pVirtualKeyboard->IsShown())
	{
		ShowVirtualKeyboard();
	}
}

void UserLoginDlg::OnChangePwd( wxCommandEvent& event )
{
	wxTextCtrl* pTextCtrl = (wxTextCtrl*)FindWindowById( ID_PASSWORD, this );
	ChangePWDDlg dlg( pTextCtrl->GetValue());
	if ( wxID_OK == dlg.ShowModal())
	{
		wxString strOldPwd = dlg.GetOldPwd();
		m_strNewPwd = dlg.GetNewPwd();
		pTextCtrl->SetValue( strOldPwd );
	}
}
void UserLoginDlg::OnWebCifco(wxCommandEvent& event)
{
	wxCheckBox* pCheck =(wxCheckBox*) FindWindowById(ID_CHECK2);
	bool bValue=pCheck->GetValue();
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
	if(pCheck&&pCfgMgr&&pCfgMgr->GetSystemParamInfo())
	{
		pCfgMgr->GetSystemParamInfo()->bWebCifco=bValue;
	}
}
void UserLoginDlg::OnSelSvr(wxCommandEvent& event)
{
	//简陋的实现方法
	string strSelSvr=LOADSTRING(USERLOGINDLG_SERVER);
	EXCLUDE_AND_SIGNAL(strSelSvr,':');
	class SelSvrDlg:public wxDialog
	{
	public:
		SelSvrDlg(wxWindow *parent,wxString cap,std::vector<SERVER_PARAM_CFG_ITEM>& item):
		  wxDialog(parent, wxID_ANY,cap,wxDefaultPosition, wxSize(200,item.size()*21+65)),
			  m_parent(parent)
		  {	
			  wxArrayString strArr;
			  for (int i=0;i<(int)item.size();++i)
			  {
				  strArr.Add(LOADSTRING_TEXT(item[i].ServerName));
			  }
			  wxRadioBox* pRadio=new wxRadioBox(this,wxID_ANY,cap,wxDefaultPosition,wxSize(180,item.size()*21+29),strArr,0,wxRA_SPECIFY_ROWS);
			  wxChoice* pChoice=(wxChoice*)m_parent->FindWindowById(ID_CHOICE);
			  if(pChoice)pRadio->SetSelection(pChoice->GetSelection());
			  Connect(wxEVT_COMMAND_RADIOBOX_SELECTED,wxCommandEventHandler(SelSvrDlg::OnServerChoice),NULL,this);
		  }
		  void OnServerChoice(wxCommandEvent& event)
		  {
			  wxChoice* pChoice=(wxChoice*)m_parent->FindWindowById(ID_CHOICE);
			  if(pChoice)pChoice->SetSelection(event.GetInt());
		  }
	private:
		wxWindow* m_parent;
	}dlg(this,strSelSvr.c_str(),DEFAULT_SVR()->GetServerParamCfg().vec);
	dlg.ShowModal();
}
void UserLoginDlg::OnMASocket(wxCommandEvent& event)
{
	RecvData* pRecvData=(RecvData*)event.GetClientData();
	if(pRecvData==NULL)return;
	wxGauge* itemGauge=(wxGauge*)FindWindowById(ID_GAUGE,this);	
	MA_CORE()->m_vecReqId.erase(pRecvData->head.seq);
	static int base_reqid=0;
	bool m_LoginFailed=false;
////////////////////////////////////////////////////////////////////////
	static set<int> cmdset;
	static int set_maxsize=6;
	if(cmdset.insert(pRecvData->head.cmdid).second)
	{
		itemGauge->SetValue(itemGauge->GetRange()*cmdset.size()/set_maxsize);
	}
////////////////////////////////////////////////////////////////////////////
	switch(pRecvData->head.cmdid)
	{
	case Cmd_MA_Connected_Rsp:
		{
			//连接成功后发起登录
			LoginReq req;
			memset(&req, 0, sizeof(req));
			strcpy(req.szAccount, m_strUserID.c_str());
			MD5 md5(m_strPassword.c_str());
			strcpy(req.szPassword, md5.toString().c_str());
			req.nAccountType = ACCOUNT_TYPE_LOGON;
			MA_CORE()->m_vecReqId.insert(++base_reqid);
			CMAConnSocket::GetInstance()->SendData(Cmd_MA_Login_Req, &req, sizeof(req),base_reqid);
		}
		break;
	case Cmd_MA_Login_Rsp:
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			MA_CORE()->m_pLoginRsp = *(LoginRsp*)pRecvData->pData;

			MA_CORE()->m_vecReqId.insert(++base_reqid);
			if(MA_CORE()->GetLoadTypeAuto()==2)
			{
				int Accid=MA_CORE()->m_pLoginRsp.nAccountID;
				CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryTradeAccount_Req, &Accid, sizeof(int),base_reqid);
			}
			else 
			{
				int AccPid= MA_CORE()->m_pLoginRsp.nAccountID;	
				CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryUsableBroker_Req, &AccPid, sizeof(int),base_reqid);
				if(MA_CORE()->GetLoadTypeAuto()==1)
					MA_CORE()->LoadFromLocal();
			}
		}
		else
		{
			m_LoginFailed=true;
			DoLoginFail(DEFAULT_SVR(),(char*)(pRecvData->pData));
		}
		break;
	case Cmd_MA_QryTradeAccount_Rsp:
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			//保存账号信息
			int count=pRecvData->head.len/sizeof(TradeAccount);
			for (int i=0;i<count;++i)
			{
				CTPTradeAccount info;
				ZeroMemory(&info,sizeof(info));
				info.t=*(TradeAccount*)((char*)pRecvData->pData+sizeof(TradeAccount)*i);
				CCryptoAPI::Decode3DES_ToString(info.t.szPassword);
				MA_CORE()->AddTradeAccount(info);
			}
			//请求经纪公司
			int AccPid=MA_CORE()->m_pLoginRsp.nAccountID;		
			MA_CORE()->m_vecReqId.insert(++base_reqid);
			CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryUsableBroker_Req, &AccPid, sizeof(int),base_reqid);
		}
		else
		{
			m_LoginFailed=true;
			DoLoginFail(DEFAULT_SVR(),(char*)(pRecvData->pData));
		}
		break;
	case Cmd_MA_QryUsableBroker_Rsp:
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int bid,count=pRecvData->head.len/sizeof(BrokerInfo);
			//保存经纪公司信息
			for (int i=0;i<count;++i)
			{
				BrokerInfo* info=(BrokerInfo*)((char*)pRecvData->pData+sizeof(BrokerInfo)*i);
				MA_CORE()->m_vecBrokerInfo.push_back(*info);
				bid=info->nBrokerID;
				MA_CORE()->m_vecReqId.insert(++base_reqid);
				CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryServerGroup_Req, &bid, sizeof(int),base_reqid);			
			}
		}
		else
		{
			m_LoginFailed=true;
			DoLoginFail(DEFAULT_SVR(),(char*)(pRecvData->pData));
		}
		break;
	case Cmd_MA_QryServerGroup_Rsp:
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int sid,count=pRecvData->head.len/sizeof(ServerGroup);
			for (int i=0;i<count;++i)
			{
				ServerGroup* info=(ServerGroup*)((char*)pRecvData->pData+sizeof(ServerGroup)*i);
				MA_CORE()->m_mapServerGroup.insert(make_pair(info->nBrokerID,*info));
				sid=info->nServerNo;
				MA_CORE()->m_vecReqId.insert(++base_reqid);
				CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryServerAddr_Req, &sid, sizeof(int),base_reqid);			
			}
		}
		else
		{
			m_LoginFailed=true;
			DoLoginFail(DEFAULT_SVR(),(char*)(pRecvData->pData));
		}
		break;
	case Cmd_MA_QryServerAddr_Rsp:
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int count=pRecvData->head.len/sizeof(ServerAddr);
			for (int i=0;i<count;++i)
			{
				ServerAddr* info=(ServerAddr*)((char*)pRecvData->pData+sizeof(ServerAddr)*i);
				MA_CORE()->m_mapServerAddr.insert(make_pair(info->nServerNo,*info));
			}
		}
		else
		{
			m_LoginFailed=true;
			DoLoginFail(DEFAULT_SVR(),(char*)(pRecvData->pData));
		}
		break;
	default:
		break;
	}
	if(!m_LoginFailed&&MA_CORE()->m_vecReqId.empty())
	{
		wxDialog::EndDialog(wxID_OK);
	}
}
void UserLoginDlg::OnQryFinish(IPlatformSingleSvr* pSvr)
{
	wxDialog::EndDialog(wxID_OK);
}