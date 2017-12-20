#include "StdAfx.h"
#include "QuotFrame.h"
#include "QuotPanel.h"
#include "TimeChartContainer.h"

CQuotFrame::CQuotFrame(wxWindow *parent,
               wxWindowID id,
               const wxPoint& pos,
               const wxSize& size,
               long style,
			   const wxString& name)
			   :wxPanel(parent, id, pos, size, style, name),
			   m_bCanMoveNext(false)
{
	m_QuotPanel=new CQuotPanel(this,wxID_ANY);
	m_PluginPanel=new CTimeChartContainer(this,wxID_ANY);
	m_PluginPanel->Hide();

}

CQuotFrame::~CQuotFrame(void)
{
}
BEGIN_EVENT_TABLE(CQuotFrame, wxPanel)
	EVT_SIZE( CQuotFrame::OnSize)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_INSTRUMENTID_CHANGED,CQuotFrame::TranslateCommandEvent) 
//	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_GET_INSTRUMENTID,  CQuotFrame::TranslateCommandEvent)
	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_SET_GROUP,CQuotFrame::TranslateCommandEvent)
//	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_GET_GROUP,CQuotFrame::TranslateCommandEvent)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE,CQuotFrame::TranslateCommandEvent)
//	EVT_COMMAND(wxID_ANY, wxEVT_PLUGIN_NEWQUOTE,CQuotFrame::OnPluginNewQuote)
	EVT_SUBSCRIBE(CQuotFrame::TranslateCommandEvent)
	EVT_CFG_CHANGE(CQuotFrame::TranslateCommandEvent)
	EVT_PANEL_FOCUS(CQuotFrame::TranslateCommandEvent)
	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_VIEW_CHANGE, OnGridDblClk) 
    EVT_ERASE_BACKGROUND(OnEraseBackground)
	EVT_COMMAND(wxID_ANY, wxEVT_MAINFRAME_QUOT_SELGROUP,CQuotFrame::TranslateCommandEvent)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_MODIFY,CQuotFrame::TranslateCommandEvent)
END_EVENT_TABLE()
wxPanel* CQuotFrame::GetActivePanel()
{
	if(m_QuotPanel->IsShown())
		return m_QuotPanel;
	if(m_PluginPanel->IsShown())
		return m_PluginPanel;
	return NULL;

}
void CQuotFrame::MoveToQuotView()
{
	HWND nHwnd=NULL;
	m_PluginPanel->Hide();
	m_QuotPanel->Show();
	((CQuotPanel*)m_QuotPanel)->SelectCurrentRow();	
	m_QuotPanel->SetSize(GetSize());
	nHwnd=(HWND)GETTOPWINDOW()->GetHWND();
	if(nHwnd&&::IsWindow(nHwnd))
	{
		WINDOW_DO_ACTIVE(nHwnd);
	}
}
void CQuotFrame::MoveToPluginView()
{
	HWND nHwnd=NULL;
	m_QuotPanel->Hide();
	m_PluginPanel->SendInstrumentMsg(m_QuotPanel);
	m_PluginPanel->Show();
	m_PluginPanel->SetSize(GetSize());
	nHwnd=GetWindow((HWND)m_PluginPanel->GetHWND(),GW_CHILD);
	if(nHwnd&&::IsWindow(nHwnd))
	{
		WINDOW_DO_ACTIVE(nHwnd);
	}
}
void CQuotFrame::MoveNextView()
{
	if(m_QuotPanel->IsShown())
	{
		MoveToPluginView();

	}
	else if(m_PluginPanel->IsShown())
	{
		MoveToQuotView();
	}
}
void CQuotFrame::OnSize( wxSizeEvent& event )
{
        wxPanel* panel=GetActivePanel();
		if(panel)panel->SetSize(event.GetSize());
}
void CQuotFrame::TranslateCommandEvent(wxCommandEvent& event)
{
	int oldid=event.GetId();
	if(oldid!=GID_QUOT_TABLE)
	{
		event.SetId(GID_QUOT_TABLE); 
		if(wxEVT_MAINFRAME_QUOT_SELGROUP==event.GetEventType())
		{
			//确保回到报价表
			if(m_PluginPanel&&m_PluginPanel->IsShown())
			{
				MoveToQuotView();
			}
		}
		wxPanel* panel=m_QuotPanel;//GetActivePanel();
		if(panel)
		{
			panel->ProcessEvent(event);
		}
		event.SetId(oldid);
	}
}
void CQuotFrame::OnGridDblClk(wxCommandEvent& evt)
{
	if(!m_bCanMoveNext)//只有行情报表
		evt.Skip();
	else
		MoveNextView();
}
void CQuotFrame::OnPluginNewQuote(wxCommandEvent& event)
{

    int type=event.GetInt();
	if(type==1)//新建
	{
		m_bCanMoveNext=true;
		event.SetExtraLong((long)m_PluginPanel);
	}
	else if(type==2)//删除
	{
		if(GetActivePanel()==m_PluginPanel)
		{
			MoveNextView();
		}
		m_bCanMoveNext=false;
	}
}
 void CQuotFrame::OnEraseBackground(wxEraseEvent& event)
 {
 }

bool CQuotFrame::GetCurInstrumentGrpInfo(std::string& GrpName,int& GrpID)
{
    return m_QuotPanel?((CQuotPanel*)m_QuotPanel)->GetCurInstrumentGrpInfo(GrpName,GrpID):false;
}

void CQuotFrame::GetCurInstrumentID(std::string& strInstrument)
{
    strInstrument.clear();
    if(m_QuotPanel)
        ((CQuotPanel*)m_QuotPanel)->GetCurInstrumentID(strInstrument);
}

//插件要求新建或删除面板，type==1新建；2删除
wxWindow* CQuotFrame::Plugin_New_Del_Panel(int type)
{
	if(type==1)//新建
	{
		m_bCanMoveNext=true;
        return m_PluginPanel;
	}
	else if(type==2)//删除
	{
		if(GetActivePanel()==m_PluginPanel)
		{
			MoveNextView();
		}
		m_bCanMoveNext=false;
	}
    return NULL;
}




