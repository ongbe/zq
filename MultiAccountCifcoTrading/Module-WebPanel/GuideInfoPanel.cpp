#include "StdAfx.h"
#ifdef _USE_TOP_SPLITTER
#include "NewsInfoPanel.h"
#include "GuideInfoPanel.h"
#include <wx/aui/auibook.h> 
#include "SOAPTitle.h"
#include "Algorithm"

extern wxArrayString g_HttpUrls;
extern wxString g_Nameprev[];
extern wxString g_GuideFilter[];

BEGIN_EVENT_TABLE(MyTextCtrl,wxTextCtrl)
EVT_CONTEXT_MENU(OnContextMenu)
END_EVENT_TABLE()
MyTextCtrl::MyTextCtrl(wxWindow *parent, wxWindowID id,
               const wxString& value,
               const wxPoint& pos ,
               const wxSize& size ,
               long style ,
               const wxValidator& validator ,
			   const wxString& name):
wxTextCtrl(parent,id,value,pos,size,style,validator,name)
{
}
void MyTextCtrl::OnContextMenu(wxContextMenuEvent& event)
{
}
BEGIN_EVENT_TABLE(CGuideInfoPanel,wxPanel)
EVT_SIZE(CGuideInfoPanel::OnSize)
EVT_COMMAND(wxID_ANY, wxEVT_SOAP_MSG,OnSoapMsg)
EVT_TIMER(ID_TIMER_GUIDE_INFO_PANEL, CGuideInfoPanel::OnTimer)
EVT_CLOSE(OnClose)
END_EVENT_TABLE()
CGuideInfoPanel::CGuideInfoPanel(wxWindow *parent,
		wxWindowID winid ,
		const wxPoint& pos ,
		const wxSize& size ,
		long style,
		const wxString& name ):
wxPanel(parent,winid,pos,size,style,name),
m_timer(this,ID_TIMER_GUIDE_INFO_PANEL)
{
	m_InfoNotebook=new wxAuiNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
	m_TextCtrl = new MyTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, wxT("GuideTextCtrl"));
	m_InfoNotebook->AddPage(m_TextCtrl,g_Nameprev[VIDEOINFO_TYPE]+"时时指导");
    m_pThread=new SoapGuideThread(this,g_HttpUrls[UrlIndex_ARTICLE]);
	m_pThread->SetParam("20");
    if(m_pThread->Create()== wxTHREAD_NO_ERROR)
    {
        m_pThread->Run();
    }
	//m_timer.Start(TIMER_INTERVAL/60);
}
void CGuideInfoPanel::OnClose(wxCloseEvent& evt)
{
	//m_timer.Stop();
	StopTimer();
}
CGuideInfoPanel::~CGuideInfoPanel(void)
{
}

void CGuideInfoPanel::OnTimer( wxTimerEvent& event)
{
	SoapGuideThread* pThread=new SoapGuideThread(this,g_HttpUrls[UrlIndex_ARTICLE]);
	pThread->SetParam("20");
	if(pThread->Create()== wxTHREAD_NO_ERROR)
	{
		pThread->Run();
	}

	WXLOG_INFO("execute CGuideInfoPanel::OnTimer!");
}

void CGuideInfoPanel::OnSize(wxSizeEvent& evt)
{
	if(m_InfoNotebook&&m_TextCtrl)
	{
		wxSize sz=evt.GetSize();
		m_InfoNotebook->SetSize(0,0,sz.x,sz.y);
	}
}

void CGuideInfoPanel::OnSoapMsg(wxCommandEvent& evt)
{
	wxString strContent=evt.GetString().AfterFirst('\n');
	wxString strUserName=evt.GetString().BeforeFirst('\n');
	if(evt.GetExtraLong()>=0)
	{
		if(g_GuideFilter[VIDEOINFO_TYPE].IsEmpty()||g_GuideFilter[VIDEOINFO_TYPE]==strUserName)
		{
			std::vector<string>::iterator iter = find(m_vecLast.begin(),m_vecLast.end(),strContent.c_str());
			if (iter == m_vecLast.end())
			{
				m_vecLast.push_back(strContent.c_str());
				m_TextCtrl->AppendText(strContent+"\n");
			}
		}
	}
}
#endif