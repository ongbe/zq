#include "stdafx.h"
#include "MyProcess.h"
#include "MainFrame.h"
extern MainFrame* s_pMainFrame;
cProcess::cProcess(wxFrame *parent,long& pid)
  : wxProcess(parent),
  m_pid(pid)
{
  WXLOG_INFO("cProcess::cProcess");
  m_parent = parent;

}
long cProcess::Execute(const wxString& command, int flags, wxProcess *process)
{
	m_pid=wxExecute(command, flags,process);
	//if(ret) s_pMainFrame->SetWindowStyle(s_pMainFrame->GetWindowStyle()&(~wxSTAY_ON_TOP));
	return m_pid; 
}
void cProcess::OnTerminate(int pid, int status)
{ 
    WXLOG_INFO("cProcess::OnTerminate");
	//s_pMainFrame->SetWindowStyle(s_pMainFrame->m_MainFrameCfg->IsTop!=0?
	//	(s_pMainFrame->GetWindowStyle()|wxSTAY_ON_TOP):
	//(s_pMainFrame->GetWindowStyle()&(~wxSTAY_ON_TOP)));
   // if(s_pMainFrame->GetFloatMainPane()&&s_pMainFrame->GetFloatMainPane()->IsShown())
    //    s_pMainFrame->GetFloatMainPane()->Hide();

    m_pid=0;
   //if( s_pMainFrame->m_bTop )
     // s_pMainFrame->GetBtArray()[1]->SetCheck(true);
	delete this; 
}
nProcess::nProcess(wxFrame *parent,long& pid)
  : wxProcess(parent),
  m_pid(pid)
{
  WXLOG_INFO("cProcess::cProcess");
  m_parent = parent;

}
long nProcess::Execute(const wxString& command, int flags, wxProcess *process)
{
	long ret=wxExecute(command, flags,process);
	//if(ret) s_pMainFrame->SetWindowStyle(s_pMainFrame->GetWindowStyle()&(~wxSTAY_ON_TOP));
	return ret; 
}