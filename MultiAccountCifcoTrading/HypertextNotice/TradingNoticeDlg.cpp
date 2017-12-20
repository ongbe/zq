#include "StdAfx.h"
#include "TradingNoticeDlg.h"
#include "IEHtmlWin.h"
#define HTML_TEXT _T("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" /></head><body></body>")
#define HTML_POS _T("<body>")
BEGIN_EVENT_TABLE(CTradingNoticeDlg, wxDialog)
EVT_CLOSE(CTradingNoticeDlg::OnClose)
EVT_BUTTON(wxID_OK,CTradingNoticeDlg::OnExit)
EVT_COMMAND(wxID_ANY,wxEVT_GET_NEW_CONTENT,OnGetNewContent)
END_EVENT_TABLE()
CTradingNoticeDlg::CTradingNoticeDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(700,400)),
m_ie(NULL)
{
	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26);
	SetBackgroundColour(wxColour(255,255,240));
	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	SetSizer(itemBoxSizer2);
	m_ie = new wxIEHtmlWin(this);
	m_ie->SetPosition(wxDefaultPosition);
	m_ie->SetSize(wxSize(680,320));
	itemBoxSizer2->Add(m_ie, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxButton* itemButton4 = new wxButton( this, wxID_OK,_T("关闭"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemButton4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	itemButton4->SetFocus();
}

CTradingNoticeDlg::~CTradingNoticeDlg(void)
{
}
void CTradingNoticeDlg::OnClose(wxCloseEvent& event)
{
    Destroy();
}
void CTradingNoticeDlg::OnExit(wxCommandEvent& event)
{
    Destroy();
}
WXLRESULT CTradingNoticeDlg::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{
	if(message==WM_COPYDATA)
	{
		COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)lParam;
		if(pCopyDataStruct&&wParam==IE_MAGIC_NUMBER)
		{
			switch(pCopyDataStruct->dwData)
			{
			case IE_MSG_CONTENT://内容
				{
					wxCommandEvent evt(wxEVT_GET_NEW_CONTENT);
					char* pContent=new char[pCopyDataStruct->cbData+1];
					if(pContent)
					{
						memset(pContent,0,pCopyDataStruct->cbData+1);
						memcpy(pContent,pCopyDataStruct->lpData,pCopyDataStruct->cbData);
						evt.SetString(pContent);
						evt.SetInt(pCopyDataStruct->dwData);
						AddPendingEvent(evt);
						delete pContent;
					}
				}
			}
		}
	}
	return wxDialog::MSWWindowProc(message,wParam,lParam);
}
void CTradingNoticeDlg::OnGetNewContent(wxCommandEvent& event)
{
	//控制显示语言为gb2312
	wxString str=event.GetString();
	wxString strAll=HTML_TEXT;
	int pos=strAll.Find(HTML_POS);
	if(pos!=wxString::npos)
	{
		strAll.insert(pos+strlen(HTML_POS),str.c_str());
	}
	m_ie->LoadString(strAll);
}