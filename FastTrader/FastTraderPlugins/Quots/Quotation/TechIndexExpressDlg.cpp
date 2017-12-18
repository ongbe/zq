///-----------------------------------------------------------------
///
/// @file      CTechIndexExpressDlg.cpp
/// @author    Administrator
/// Created:   2012-8-23 13:20:57
/// @section   DESCRIPTION
///            CTechIndexExpressDlg class implementation
///
///------------------------------------------------------------------
#include "StdAfx.h"
#include "../../../inc/Quotation/TechIndexExpressDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// CTechIndexExpressDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(CTechIndexExpressDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(CTechIndexExpressDlg::OnClose)
	EVT_BUTTON(ID_WXBUTTON3,   OnCloseEvent)
	EVT_BUTTON(ID_WXBUTTON1,   OnCopy)
END_EVENT_TABLE()
////Event Table End

CTechIndexExpressDlg::CTechIndexExpressDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
	m_strTechIndex = "";
}

CTechIndexExpressDlg::~CTechIndexExpressDlg()
{
} 

void CTechIndexExpressDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start
	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT(""), wxPoint(2, 1), wxSize(369, 160),wxTE_MULTILINE, wxDefaultValidator, wxT("WxEdit1"));
	
	WxButton3 = new wxButton(this, ID_WXBUTTON3, wxT("关闭"), wxPoint(291, 164), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("WxButton2"), wxPoint(276, 182), wxSize(1, 2), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("拷贝"), wxPoint(197, 164), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	
	WxEdit1->SetEditable(false);

	wxString strTitle;
	strTitle.Printf(wxT("%s指标用法"), m_strTechIndex);
	SetTitle(strTitle);
	SetIcon(wxNullIcon);
	SetSize(8,8,379,221);
	Center();
	
	////GUI Items Creation End
}

void CTechIndexExpressDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
void CTechIndexExpressDlg::SetTechIndex(wxString strTechIndex)
{
	m_strTechIndex = strTechIndex;
	Init();
	wxString strTitle;
	strTitle.Printf(wxT("%s指标用法"), m_strTechIndex);
	SetTitle(strTitle);
}
void CTechIndexExpressDlg::Init()
{
	m_strExpress = "";
	wxString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath = filename.substr(0, splitpos+1);

	wxString strDir;
	strDir.Printf(_T("%shqData\\tech.xml"), strPath);

	TiXmlDocument* pXmlDocment = new TiXmlDocument( strDir.c_str());
	if( NULL == pXmlDocment)
	{
		return ;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return ;
	}

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return ;
	}

	if( std::string(root->Value()) != "tech")
	{
		return ;
	}

	TiXmlElement *tList2 = root->FirstChildElement(m_strTechIndex.c_str());
	if(tList2 == NULL)
		return ;
	const char *strValue = tList2->ToElement()->GetText();
	WxEdit1->SetValue(strValue);

	m_strExpress = strValue;

}
void CTechIndexExpressDlg::OnCloseEvent( wxCommandEvent &event )
{

	Close(true);
	//	event.Skip();
}
void CTechIndexExpressDlg::OnCopy( wxCommandEvent &event )
{
	
	HANDLE hClip;
	char *pBuf;
	char* str = (char *)m_strExpress.c_str();
	if(!OpenClipboard((HWND)m_hWnd))
		return;
	EmptyClipboard();
	hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(str)+1);
	pBuf=(char *)GlobalLock(hClip);
	strcpy(pBuf,str);
	GlobalUnlock(hClip);
	SetClipboardData(CF_TEXT,hClip);
	CloseClipboard();

}