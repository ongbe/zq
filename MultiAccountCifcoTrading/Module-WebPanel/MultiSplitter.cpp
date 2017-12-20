#include "StdAfx.h"
#ifdef _USE_TOP_SPLITTER
#include "MultiSplitter.h"
#include "VideoInfoPanel.h"
#include "NewsInfoPanel.h"
#include "GuideInfoPanel.h"
MultiSplitterWindow::MultiSplitterWindow(wxWindow *parent, wxWindowID id,
                                   const wxPoint& pos,
                                   const wxSize& size,
								   long style)
{
	m_fixed=(style&wxSP_NOSASH)>0;
	if(m_fixed)
	{
		style&=~wxSP_NOSASH;
	}
	Create(parent, id, pos, size, style);
	SetMinimumPaneSize(78);
}
bool MultiSplitterWindow::OnSashPositionChange(int WXUNUSED(newSashPosition))
{
	return !m_fixed;
}
BEGIN_EVENT_TABLE(MultiSplitter, wxPanel)
    EVT_SIZE(OnSize)
END_EVENT_TABLE()
MultiSplitter::MultiSplitter(wxWindow *parent, wxWindowID id,
                                   const wxPoint& pos,
                                   const wxSize& size,
                                   long style,
								   const wxString& name):
wxPanel(parent,id,pos,size,style,name),
m_pSplitterBase(NULL)
{
	m_pSplitterBase=new MultiSplitterWindow(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_NOSASH|wxSP_3D|wxNO_BORDER);
	wxSplitterWindow* pSplitter=new MultiSplitterWindow(m_pSplitterBase,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_NOSASH|wxSP_3D|wxNO_BORDER);
	//视频
	m_pVideoPanel=new CVideoInfoPanel(pSplitter);
	//资讯
	m_pNewsPanel=new CNewsInfoPanel(pSplitter);
	//交易指导
	m_pGuidePanel=new CGuideInfoPanel(m_pSplitterBase);
	//水平分割
	pSplitter->SetSashGravity(0);
	pSplitter->SplitHorizontally(m_pVideoPanel,m_pNewsPanel,180);
	//水平分割
	m_pSplitterBase->SetSashGravity(0);
	m_pSplitterBase->SplitHorizontally(pSplitter,m_pGuidePanel,600);	

}

MultiSplitter::~MultiSplitter(void)
{
}
void MultiSplitter::OnSize(wxSizeEvent& evt)
{
	if(m_pSplitterBase)
	{
		wxSize sz=GetClientSize();
		m_pSplitterBase->SetSize(0,0,sz.x,sz.y);
	}
	evt.Skip();
}
bool MultiSplitter::Show(bool show)
{
	return wxPanel::Show(show);
}

void MultiSplitter::StartTimer()
{
	m_pVideoPanel->StartTimer();
	m_pNewsPanel->StartTimer();
	m_pGuidePanel->StartTimer();
}

void MultiSplitter::StopTimer()
{
	m_pVideoPanel->StopTimer();
	m_pNewsPanel->StopTimer();
	m_pGuidePanel->StopTimer();
}

#endif