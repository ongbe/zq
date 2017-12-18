#include "StdAfx.h"
#include "../Module-Misc2/globalDefines.h"
#include "../cfgMgr/CfgMgr.h"
#include "ConfigPanelBase.h"

BEGIN_EVENT_TABLE(ConfigPanelBase,wxPanel)
	//EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturn)
END_EVENT_TABLE()

ConfigPanelBase::ConfigPanelBase(wxWindow* pParent,
								 wxWindowID winid,
								 const wxPoint& pos,
								 const wxSize& size,
								 long style,
								 const wxString& name)
								 : wxPanel(pParent,winid,pos,size,style,name)
								 , m_strTitle("")
								 , m_bSungardStyle(false)
{
}

ConfigPanelBase::~ConfigPanelBase(void)
{
}

void ConfigPanelBase::CreateHeader( std::string strTitle )
{
	SetBackgroundColour(DEFAULT_COLOUR);

	m_strTitle = strTitle;
	int nTopOffset = 15;
	m_pStaticTitle = new wxStaticText(this, wxID_ANY, LOADSTRING_TEXT(strTitle), wxPoint(10, nTopOffset));
	m_pStaticTitle->SetFont(wxFont(16, wxSWISS, wxNORMAL, wxBOLD, false));
	m_pBtnReturn = new wxButton(this, ID_RETURN_NORMAL_UI, LOADSTRING(RETURN_NORMAL), wxPoint(234, nTopOffset), wxSize(75, 25));
	m_pBtnSaveAndReturn = new wxButton(this, ID_SAVE_AND_RETURN, LOADSTRING(SAVE_AND_RETURN), wxPoint(320, nTopOffset), wxSize(105, 25));

	new wxStaticLine(this, wxID_ANY, wxPoint(10, 45), wxSize(450, -1), wxLI_HORIZONTAL);
}

void ConfigPanelBase::OnLanguageChanged()
{
	m_pStaticTitle->SetLabel( LOADSTRING_TEXT(m_strTitle) );
	m_pBtnReturn->SetLabel( LOADSTRING(RETURN_NORMAL) );
	m_pBtnSaveAndReturn->SetLabel( LOADSTRING(SAVE_AND_RETURN) );
}

void ConfigPanelBase::EnableSaveAndReturn( bool bEnable )
{
	m_pBtnSaveAndReturn->Show( bEnable );	
}

void ConfigPanelBase::SetSungardStyle( bool bFlag )
{
	m_bSungardStyle = bFlag;
}

//void ConfigPanelBase::OnReturn( wxCommandEvent& event )
//{
//	if( m_bSungardStyle )
//	{
//		event.Skip();
//	}
//	else
//	{
//		this->GetParent()->Close();
//	}
//}

wxString ConfigPanelBase::GetWindowCaption()
{
	return LOADSTRING_TEXT(m_strTitle);
}
