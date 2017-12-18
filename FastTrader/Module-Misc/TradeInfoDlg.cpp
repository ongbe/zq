/***************************************************************************//**
*   @file         TradeInfoDlg.cpp
*   @brief        交易信息提示对话框（非模式的对话框）
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
********************************************************************************/
#include "stdafx.h"
#include "TradeInfoDlg.h"
#include <wx/statline.h> 
#include "constants.h"
#include "EventSet.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "CfgMgr/CfgMgr.h"
#include "../Module-Misc/LogDefine.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
BEGIN_EVENT_TABLE(CTradeInfoDlg,wxDialog)
	EVT_CLOSE(CTradeInfoDlg::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,CTradeInfoDlg::WxButton1Click)
	EVT_BUTTON(ID_BTN_PREV,CTradeInfoDlg::OnBtnPrev)
	EVT_BUTTON(ID_BTN_NEXT,CTradeInfoDlg::OnBtnNext)
	EVT_CHECKBOX(ID_CHECK_CONFIG, CTradeInfoDlg::OnConfigCheck_Click)
	EVT_SIZE(CTradeInfoDlg::OnSize)
END_EVENT_TABLE()
CTradeInfoDlg* m_thisDlg=NULL;
CTradeInfoDlg *CTradeInfoDlg::GetInstance(wxWindow *parent)
{
	if(parent&&(!m_thisDlg))
	{
		m_thisDlg = new CTradeInfoDlg(parent);
		m_thisDlg->InitConfig();
	}
	return m_thisDlg;
}
void CTradeInfoDlg::Release()
{
	wxDELETE(m_thisDlg);
}
CTradeInfoDlg::CTradeInfoDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(350,250),wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)//交易信息提示
,m_bInitialized(false)
{
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();

	CreateGUIControls();
	SetIcon(wxNullIcon);
	SetMinSize(wxSize(350,250));
	SetMaxSize(wxSize(640,320));
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxSize dlgsz=wxSize(LOWORD(pCfgMgr->GetMainWindowCfg()->TradeInfoDlgSize),HIWORD(pCfgMgr->GetMainWindowCfg()->TradeInfoDlgSize));
	if(dlgsz.x>=350&&dlgsz.x<640&&dlgsz.y>=250&&dlgsz.y<=320)
		SetSize(dlgsz);
	Center();
	ResetLanguage();
	InitConfig();
}

void CTradeInfoDlg::InitConfig()
{
	m_bCancelSuccessDlg = true;
	m_bCancelFailDlg = true;
	m_bOrderFailDlg = true;
	m_bOrderSuccessDlg = true;
	m_bTradeDlg = true;
	m_bSCConvertSuccessDlg = true;
	m_bSCConvertFailDlg = true;

	m_bCancelSuccessDlgChg = false;
	m_bCancelFailDlgChg = false;
	m_bOrderFailDlgChg = false;
	m_bOrderSuccessDlgChg = false;
	m_bTradeDlgChg = false;
	m_bSCConvertSuccessDlgChg = false;
	m_bSCConvertFailDlgChg = false;

	
	//CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	//if(pMgr != NULL)
	//{
	//	CANCEL_ORDER_CFG cancelCfg = pMgr->GetCancelOrderCfg();
	//	m_bCancelSuccessDlg = cancelCfg.bCancelSuccessDlg;
	//	m_bCancelFailDlg = cancelCfg.bCancelFailDlg;
	//	LPORDER_CFG p = pMgr->GetOrderCfg();
	//	m_bOrderFailDlg = p->bOrderFailDlg;
	//	m_bOrderSuccessDlg = p->bOrderSuccessDlg;
	//	m_bTradeDlg = p->bTradeDlg;
	//}
}

CTradeInfoDlg::~CTradeInfoDlg()
{
}

void CTradeInfoDlg::CreateGUIControls()
{
    SetBackgroundColour(*wxWHITE);
    wxSize sz=GetClientSize();
    WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxEmptyString, wxPoint(30, 10), wxSize(sz.x-60, sz.y-10-50),wxST_NO_AUTORESIZE);
    wxStaticLine* line1 = new wxStaticLine(this, ID_ONLYLINE, wxPoint(30, sz.y-50), wxSize(sz.x-60, -1), wxLI_HORIZONTAL);
	//line1->Show(false);
    wxButton* pBtnPrev=new wxButton(this, ID_BTN_PREV, wxEmptyString, wxPoint(10, sz.y-30), wxSize(60, 26));//上一条
    pBtnPrev->Hide();
    wxButton* pBtnNext=new wxButton(this, ID_BTN_NEXT, wxEmptyString, wxPoint(80, sz.y-30), wxSize(60, 26));//下一条
    pBtnNext->Hide();
	wxCheckBox*	pCheckConfig = new wxCheckBox( this, ID_CHECK_CONFIG, 
				wxEmptyString, wxPoint(10, sz.y-48), wxSize(16*20, 16));//, 0 );
    m_current=0;
//	m_LocalTitleId=-1;
//	m_LocalInfoId=-1;
    WxButton1 = new wxButton(this, ID_WXBUTTON1, wxEmptyString, wxPoint(sz.x-70, sz.y-30), wxSize(60, 26));//确定
	m_bInitialized=true;
}
void CTradeInfoDlg::OnSize(wxSizeEvent& event)
{
	if(!m_bInitialized) return;
    wxSize sz=GetClientSize();
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	pCfgMgr->GetMainWindowCfg()->TradeInfoDlgSize=MAKELPARAM(event.GetSize().x,event.GetSize().y);
	FindWindowById(ID_WXSTATICTEXT1,this)->SetSize(20,20,sz.x-40, sz.y-10-60);
	FindWindowById(ID_ONLYLINE,this)->SetSize(10, sz.y-50, sz.x-20, -1);
	FindWindowById(ID_BTN_PREV,this)->SetSize(10, sz.y-36, 60, 26);
	FindWindowById(ID_BTN_NEXT,this)->SetSize(80, sz.y-36, 60, 26);
	FindWindowById(ID_WXBUTTON1,this)->SetSize(sz.x-70, sz.y-36, 60, 26);
	FindWindowById(ID_CHECK_CONFIG,this)->SetSize(150, sz.y-30, 15*8, 16);
}
void CTradeInfoDlg::OnClose(wxCloseEvent& /*event*/)
{
    CeasymutexGuard guard(m_mutex);

	InitConfig();

	m_TitleArray.Clear();
    m_InfoArray.Clear();
    m_setSysID.clear();
    m_current=0;

    wxDialog::SetTitle(wxString()); 
    WxStaticText1->SetLabel(wxString());
    FindWindowById(ID_BTN_PREV,this)->Show(false);
    FindWindowById(ID_BTN_NEXT,this)->Show(false);

    Hide();
}

void CTradeInfoDlg::WxButton1Click(wxCommandEvent& event)
{
    CeasymutexGuard guard(m_mutex);

	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if(pMgr != NULL)
	{
		CANCEL_ORDER_CFG& cancelCfg = pMgr->GetCancelOrderCfg();
		if(m_bCancelSuccessDlgChg)
			cancelCfg.bCancelSuccessDlg = m_bCancelSuccessDlg;
		if(m_bCancelFailDlgChg)
			cancelCfg.bCancelFailDlg = m_bCancelFailDlg;
		pMgr->SetCancelOrderCfg(cancelCfg);
		LPORDER_CFG p = pMgr->GetOrderCfg();
		if(m_bOrderFailDlgChg)
			p->bOrderFailDlg = m_bOrderFailDlg;
		if(m_bOrderSuccessDlgChg)
			p->bOrderSuccessDlg = m_bOrderSuccessDlg;
		if(m_bTradeDlgChg)
			p->bTradeDlg = m_bTradeDlg;
		if(m_bSCConvertSuccessDlgChg)
			p->bConventSuccessDlg = m_bSCConvertSuccessDlg;
		if(m_bSCConvertFailDlgChg)
			p->bConventFailDlg = m_bSCConvertFailDlg;
	}

	InitConfig();
    m_TitleArray.Clear();
    m_InfoArray.Clear();
    m_setSysID.clear();
    m_current=0;

    wxDialog::SetTitle(wxString()); 
    WxStaticText1->SetLabel(wxString());
    FindWindowById(ID_BTN_PREV,this)->Show(false);
    FindWindowById(ID_BTN_NEXT,this)->Show(false);
    Hide();
}
void CTradeInfoDlg::OnBtnNext(wxCommandEvent& event)
{
    m_current++;
    Reload();
	ShowCheckConfig();
}
static int ccc;
void CTradeInfoDlg::Reload()
{
    CeasymutexGuard guard(m_mutex);

    int count=m_TitleArray.Count();
    if(count==0)
    {
        wxDialog::SetTitle(wxString()); 
        WxStaticText1->SetLabel(wxString());
        FindWindowById(ID_BTN_PREV,this)->Show(false);
        FindWindowById(ID_BTN_NEXT,this)->Show(false);
        return;
    }

    if(m_current>=count) m_current=count-1;
    if(m_current<0) m_current=0;

    ccc=count;
    int count2=count;
    wxString strAllTitle;
    strAllTitle.Printf("(%d/%d) %s",m_current+1,count,m_TitleArray[m_current]);
    int count3=ccc;
    wxDialog::SetTitle(strAllTitle); 
    WxStaticText1->SetLabel(m_InfoArray[m_current]);
    FindWindowById(ID_BTN_PREV,this)->Show(m_current!=0);
    FindWindowById(ID_BTN_NEXT,this)->Show(m_current!=count-1);

}
void CTradeInfoDlg::OnBtnPrev(wxCommandEvent& event)
{
    m_current--;
    Reload();
	ShowCheckConfig();
}

void CTradeInfoDlg::SetInfo(const wxString& info,int id)
{
//	if(!(id>0&&id==m_LocalInfoId))
//		m_InfoArray.Add(info);
//    WxStaticText1->SetLabel(info);
//	if(id)m_LocalInfoId=id;
}

void CTradeInfoDlg::SetTitle( const wxString& title,int id)
{ 
//	if(!(id>0&&id==m_LocalTitleId))
//		m_TitleArray.Add(title);
//    wxString strAllTitle=title;
//    int count=m_TitleArray.Count();
//    m_current=count-1;//显示最后一个
//    if(count>1)
//    {
//        strAllTitle.Printf("(%d/%d) %s",count,count,title);
//        FindWindowById(ID_BTN_PREV,this)->Show();
//        FindWindowById(ID_BTN_NEXT,this)->Hide();
//    }
//    else
//    {
//        FindWindowById(ID_BTN_PREV,this)->Hide();
//        FindWindowById(ID_BTN_NEXT,this)->Hide();
//    }
//    wxDialog::SetTitle(strAllTitle); 
//    if(id)m_LocalTitleId=id;
}

void CTradeInfoDlg::SetMessage( const wxString& title, const wxString& info, const wxString& strID,bool bforce)
{
    CeasymutexGuard guard(m_mutex);

	if(bforce || m_setSysID.find(strID)==m_setSysID.end()) {
		wxString strNewInfo(info);
		strNewInfo.Replace("&", "&&");
		m_TitleArray.Add(title);
		m_InfoArray.Add(strNewInfo);
		WxStaticText1->SetLabel(strNewInfo);

		wxString strAllTitle = title;;
		int count=m_TitleArray.Count();
		m_current=count-1;//显示最后一个
		if(count>1)
		{
			strAllTitle.Printf("(%d/%d) %s",count,count,title.c_str());
			FindWindowById(ID_BTN_PREV,this)->Show();
			FindWindowById(ID_BTN_NEXT,this)->Hide();
		}
		else
		{
			FindWindowById(ID_BTN_PREV,this)->Hide();
			FindWindowById(ID_BTN_NEXT,this)->Hide();
		}
		wxDialog::SetTitle(strAllTitle); 
		m_setSysID.insert(strID);
	}

}


void CTradeInfoDlg::ShowTradeMsg(const wxString& strInfo, int nInfoId, const wxString& strTitle, int nTitleId, BOOL bIsShowDlg)
{
    assert(nInfoId==-1);
	wxString strTempInfo = REPLACEFMTSTRING(strInfo.c_str());
    wxString strMsg=strTitle+":";
    strMsg+=strTempInfo;
    strMsg.Replace(wxT("\n"), wxT(" "));

	if(bIsShowDlg) 
    {
		SetMessage(strTitle, strTempInfo, wxString(""),true);
		ShowCheckConfig();
		Show(true);
	}
}
void CTradeInfoDlg::ShowTradeMsg2(const wxString& strInfo, const wxString& strTitle, BOOL bIsShowDlg, const wxString& strID, bool bForce)
{
	wxString strTempInfo = REPLACEFMTSTRING(strInfo.c_str());
    wxString strMsg=strTitle+":";
    strMsg+=strTempInfo;
    strMsg.Replace(wxT("\n"), wxT(" "));

	if(bIsShowDlg) 
    {
		SetMessage(strTitle, strTempInfo, strID, bForce);
		ShowCheckConfig();
		Show(true);
	}
}

bool CTradeInfoDlg::Show(bool show)
{
    if(show)
    {
		//wxString strMsg=GetTitle()+":";
		//strMsg+=WxStaticText1->GetLabel();
		//strMsg.Replace(wxT("\n"),wxT(" "));
		//wxCommandEvent myEvent(wxEVT_WRITE_USER_LOG);
		//myEvent.SetString(strMsg.c_str());
		//wxWindow* pWin = ((wxApp*)wxApp::GetInstance())->GetTopWindow();
		//pWin->ProcessEvent(myEvent);
		if(!IsShown())
		{//由隐藏变成显示
			FindWindowById(ID_WXBUTTON1,this)->SetFocus();
			wxDialog::CentreOnScreen();
		}
    }
	//else
	//{
	//	//选择下单面板
	//	wxCommandEvent myEvent(wxEVT_PANEL_FOCUS);
	//	wxWindow* pWin=FindWindowById(GID_ORDER_INSERT_PANEL);
	//	if(pWin)pWin->AddPendingEvent(myEvent);
	//}
    return wxDialog::Show(show);
}

void CTradeInfoDlg::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_WXBUTTON1, this);
	if(pWin) 
		pWin->SetLabel(LOADSTRING(ASKNAMEDLG_OK));
	pWin = FindWindowById(ID_BTN_PREV, this);
	if(pWin) 
		pWin->SetLabel(LOADSTRING(ID_INFODLG_PREV));
	pWin = FindWindowById(ID_BTN_NEXT, this);
	if(pWin) 
		pWin->SetLabel(LOADSTRING(ID_INFODLG_NEXT));
	pWin = FindWindowById(ID_CHECK_CONFIG, this);
	if(pWin) 
	{
		wxString strText = GetTitle();
		pWin->SetLabel(LOADSTRING(ID_INFODLG_CONFIG)+strText);
	}
}

void CTradeInfoDlg::WriteLog( LOG_DATA* pData )
{
	wxCommandEvent myEvent(wxEVT_WRITE_USER_LOG);
	myEvent.SetClientData( (void*)pData);
	wxWindow* pWin = ((wxApp*)wxApp::GetInstance())->GetTopWindow();
	pWin->AddPendingEvent(myEvent);
}

void CTradeInfoDlg::ShowCheckConfig()
{
	wxString strText = GetTitle();
	char strTitle[1024], strTemp[1024];
	ZeroMemory(strTitle, sizeof(strTitle));
	ZeroMemory(strTemp, sizeof(strTemp));
	sscanf_s(strText.c_str(), "%[^ ]%s", strTemp, sizeof(strTemp)-1, strTitle, sizeof(strTitle)-1);
	if(strlen(strTitle)>0)
		strText = strTitle;
	else
		strText = strTemp;
	wxCheckBox* pWin = (wxCheckBox*)FindWindowById(ID_CHECK_CONFIG, this);
	if(pWin) 
	{
		pWin->SetLabel(LOADSTRING(ID_INFODLG_CONFIG)+strText);
		if(strText.compare(LOADSTRING(CANCEL_ORDER_OK))==0)
		{
			pWin->SetValue(!m_bCancelSuccessDlg);
			pWin->Show();
		}
		else if(strText.compare(LOADSTRING(OLS_SEND_ORDER_FAIL))==0)
		{
			pWin->SetValue(!m_bOrderFailDlg);
			pWin->Show();
		}
		else if(strText.compare(LOADSTRING(SEND_ORDER_OK))==0)
		{
			pWin->SetValue(!m_bOrderSuccessDlg);
			pWin->Show();
		}
		else if(strText.compare(LOADSTRING(FILL_MESSAGE))==0)
		{
			pWin->SetValue(!m_bTradeDlg);
			pWin->Show();
		}
		else if(strText.compare(LOADSTRING(OLS_CANCEL_ORDER_FAIL))==0)
		{
			pWin->SetValue(!m_bCancelFailDlg);
			pWin->Show();
		}
		else if(strText.compare(LOADSTRING(CONDITION_ORDER_CONVENT_SUCCESS))==0)
		{
			pWin->SetValue(!m_bSCConvertSuccessDlg);
			pWin->Show();
		}
		else if(strText.compare(LOADSTRING(CONDITION_ORDER_CONVENT_FAILED))==0)
		{
			pWin->SetValue(!m_bSCConvertFailDlg);
			pWin->Show();
		}
		else
		{
			pWin->Show(false);
		}
	}
}

void CTradeInfoDlg::OnConfigCheck_Click(wxCommandEvent& event)
{
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return;
    LPORDER_CFG p = pMgr->GetOrderCfg();

	wxString strText = GetTitle();
	char strTitle[1024], strTemp[1024];
	ZeroMemory(strTitle, sizeof(strTitle));
	ZeroMemory(strTemp, sizeof(strTemp));
	sscanf_s(strText.c_str(), "%[^ ]%s", strTemp, sizeof(strTemp)-1, strTitle, sizeof(strTitle)-1);
	if(strlen(strTitle)>0)
		strText = strTitle;
	else
		strText = strTemp;
	wxWindow* pWin = FindWindowById(ID_CHECK_CONFIG, this);
	if(pWin) 
	{
		pWin->SetLabel(LOADSTRING(ID_INFODLG_CONFIG)+strText);
		if(strText.compare(LOADSTRING(CANCEL_ORDER_OK))==0)
		{
			m_bCancelSuccessDlgChg = true;
			m_bCancelSuccessDlg = !event.GetInt();
		}
		else if(strText.compare(LOADSTRING(OLS_SEND_ORDER_FAIL))==0)
		{
			m_bOrderFailDlgChg = true;
			m_bOrderFailDlg = !event.GetInt();
		}
		else if(strText.compare(LOADSTRING(SEND_ORDER_OK))==0)
		{
			m_bOrderSuccessDlgChg = true;
			m_bOrderSuccessDlg = !event.GetInt();
		}
		else if(strText.compare(LOADSTRING(FILL_MESSAGE))==0)
		{
			m_bTradeDlgChg = true;
			m_bTradeDlg = !event.GetInt();
		}
		else if(strText.compare(LOADSTRING(OLS_CANCEL_ORDER_FAIL))==0)
		{
			m_bCancelFailDlgChg = true;
			m_bCancelFailDlg = !event.GetInt();
		}
		else if(strText.compare(LOADSTRING(CONDITION_ORDER_CONVENT_SUCCESS))==0)
		{
			m_bSCConvertSuccessDlgChg = true;
			m_bSCConvertSuccessDlg = !event.GetInt();
		}
		else if(strText.compare(LOADSTRING(CONDITION_ORDER_CONVENT_FAILED))==0)
		{
			m_bSCConvertFailDlgChg = true;
			m_bSCConvertFailDlg = !event.GetInt();
		}
	}
}
