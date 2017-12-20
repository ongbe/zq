#pragma once  
#include <wx/process.h>
class cProcess : public wxProcess
{
public:
  cProcess(wxFrame *parent,long& pid);
  long Execute(const wxString& command, int flags = wxEXEC_ASYNC, wxProcess *process = (wxProcess *) NULL);
  void OnTerminate(int pid, int status);

protected:
  long&  m_pid;
  wxFrame *m_parent;
};
class nProcess : public wxProcess
{
public:
  nProcess(wxFrame *parent,long& pid);
  long Execute(const wxString& command, int flags = wxEXEC_ASYNC, wxProcess *process = (wxProcess *) NULL);
  //void OnTerminate(int pid, int status);

protected:
  long&  m_pid;
  wxFrame *m_parent;
};