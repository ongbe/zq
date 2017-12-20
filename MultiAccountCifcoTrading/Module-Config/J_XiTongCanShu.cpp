#include "stdafx.h"

#include "cfgMgr/CfgMgr.h"
#include "J_XiTongCanShu.h"

BEGIN_EVENT_TABLE(J_XiTongCanShuDlg,ConfigPanelBase)

	EVT_BUTTON(ID_BTN_APPLY_DEFAULT,OnApplyDefault)
	EVT_BUTTON(ID_SAVE_AND_RETURN,OnSaveAndReturn)
	EVT_COMBOBOX(ID_COM, OnHotkeySelChanged) 
	EVT_CHECKBOX(wxID_ANY, OnCheck)
	EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturn)
	EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
	EVT_RADIOBUTTON(wxID_ANY,OnRadioButton)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()

void J_XiTongCanShuDlg::OnReturn(wxCommandEvent& event)
{
	if(m_bChanged)
	{
		if(wxMessageBox(LOADSTRING(CONFIG_IS_CHANGED),LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO)!=wxYES)
		{
			return;
		}
	}

	if( m_bSungardStyle )
	{
		event.Skip();
	}
	else
	{
		this->GetParent()->Close();
	}
}


J_XiTongCanShuDlg::J_XiTongCanShuDlg(wxWindow *parent,
									 wxWindowID winid ,
									 const wxPoint& pos ,
									 const wxSize& size ,
									 long style,
									 const wxString& name)
									 : ConfigPanelBase(parent,winid,pos,size,style,name)

{
	ConfigPanelBase::CreateHeader( "CONFIG_SYSTEM_PARAM" );

	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if( pMgr != NULL)
	{
		pMgr->CalcUsableShortCutKeys();
	}

	int offset = 30;
	m_pStaticBox = new wxStaticBox(this, wxID_ANY, LOADSTRING(CONFIG_HIDE_HOTKEY), wxPoint(19, 16+offset), wxSize(130, 164));
	m_pChkUseHideHotkey = new wxCheckBox(this, ID_CHECK1, LOADSTRING(CONFIG_USE_HIDE_HOTKEY), wxPoint(28, 32+offset), wxSize(115, 17), 0, wxDefaultValidator, wxT("WxCheckBox1"));

	wxArrayString arr;
	arr.Add("F1");
	arr.Add("F2");
	arr.Add("F3");
	arr.Add("F4");
	arr.Add("F5");
	arr.Add("F6");
	arr.Add("F7");
	arr.Add("F8");
	arr.Add("F9");
	arr.Add("F10");
	arr.Add("F11");
	arr.Add("F12");
	m_combox = new wxOwnerDrawnComboBox(this, ID_COM, wxT(""), wxPoint(29, 55+offset), wxSize(77, 20), arr, wxCB_READONLY, wxDefaultValidator, wxT("WxComboBox1"));

	m_chkAlt = new wxCheckBox(this, wxID_ANY, wxT("Alt"), wxPoint(28, 86+offset), wxSize(97, 17), 0, wxDefaultValidator, wxT("WxCheckBox2"));
	m_chkCtrl = new wxCheckBox(this, wxID_ANY, wxT("Ctrl"), wxPoint(28, 109+offset), wxSize(97, 17), 0, wxDefaultValidator, wxT("WxCheckBox3"));
	m_chkShift = new wxCheckBox(this, wxID_ANY, wxT("Shift"), wxPoint(28, 133+offset), wxSize(97, 17), 0, wxDefaultValidator, wxT("WxCheckBox4"));
	m_chkWindow = new wxCheckBox(this, wxID_ANY, wxT("Window"), wxPoint(28, 152+offset), wxSize(97, 17), 0, wxDefaultValidator, wxT("WxCheckBox5"));

	m_pBtnApplyDefault = new wxButton(this, ID_BTN_APPLY_DEFAULT, LOADSTRING(CONFIG_APPLY_DEFAULT_SETTING), wxPoint(175, 110+offset), wxSize(125, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	wxStaticBox* pp = new wxStaticBox(this, wxID_ANY, LOADSTRING(CONFIG_VOICE_PROMPT), wxPoint(160, 16+offset), wxSize(117, 162));
	pp->Show(false);

	Rb1 = new wxRadioButton(this, wxID_ANY, LOADSTRING(CONFIG_DONOT_USE), wxPoint(172, 32+offset), wxSize(85, 17), 0, wxDefaultValidator, wxT("WxRadioButton1"));
	Rb1->Show(false);
	Rb2 = new wxRadioButton(this, wxID_ANY, LOADSTRING(CONFIG_PC_LOUDSPEAKER), wxPoint(172, 55+offset), wxSize(80, 17), 0, wxDefaultValidator, wxT("WxRadioButton2"));
	Rb2->Show(false);

	Rb3= new wxRadioButton(this, wxID_ANY, LOADSTRING(CONFIG_MEDIA_LOUDSPEAKER), wxPoint(173, 79+offset), wxSize(87, 17), 0, wxDefaultValidator, wxT("WxRadioButton3"));
	Rb3->Show(false);

	m_bChanged = false;
	ReadCfg();
}

void J_XiTongCanShuDlg::OnCheck(wxCommandEvent& event)
{
	m_bChanged = true;
	if( m_pChkUseHideHotkey->GetValue() )
	{
		m_combox->Enable();
		m_chkAlt->Enable();
		m_chkCtrl->Enable();
		m_chkShift->Enable();
		m_chkWindow->Enable();
	}
	else
	{
		m_combox->Disable();
		m_chkAlt->Disable();
		m_chkCtrl->Disable();
		m_chkShift->Disable();
		m_chkWindow->Disable();
	}

}

void J_XiTongCanShuDlg::OnSaveAndReturn(wxCommandEvent& event)
{
	m_bChanged = false;  
	if( !m_chkAlt->GetValue()&&
		!m_chkCtrl->GetValue()&&
		!m_chkShift->GetValue()&&
		!m_chkWindow->GetValue())
	{
		wxString strValue = m_combox->GetValue();
		CfgMgr* pMgr = CFG_MGR_DEFAULT();  
		if( pMgr != NULL )
		{
			//没有改变时，不用保存
			if ( strlen(pMgr->GetSystemParamInfo()->key2) == 0
				&& strlen(pMgr->GetSystemParamInfo()->key3) == 0
				&& strlen(pMgr->GetSystemParamInfo()->key4) == 0
				&& strlen(pMgr->GetSystemParamInfo()->key5) == 0
				&& strValue == pMgr->GetSystemParamInfo()->key1)
			{
				CLOSE_FLOAT_PANEL();
				return;
			}
			else
			{
				if ( !pMgr->IsShortKeyUsable(strValue.c_str()) )
				{
					wxMessageBox(LOADSTRING(CONFIG_DUPLICATE_HOTKEYS), LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_ERROR);
					return;
				}
			}
		}

	}
	WriteCfg();

	CLOSE_FLOAT_PANEL();
}

void J_XiTongCanShuDlg:: OnHotkeySelChanged(wxCommandEvent& event)
{
	m_bChanged = true;
}

void J_XiTongCanShuDlg::OnApplyDefault(wxCommandEvent& event)
{
	ReadCfg(true);
	m_bChanged = true;

}

void  J_XiTongCanShuDlg::ReadCfg(bool bDefault)
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	SystemParamInfo*p = pMgr ->GetSystemParamInfo(bDefault);
	if(p->bUse)
	{ 
		m_pChkUseHideHotkey->SetValue( true);
		m_combox->Enable();
		m_chkAlt->Enable();
		m_chkCtrl->Enable();
		m_chkShift->Enable();
		m_chkWindow->Enable();
	}
	else 
	{
		m_pChkUseHideHotkey->SetValue( false);
		m_combox->Disable();
		m_chkAlt->Disable();
		m_chkCtrl->Disable();
		m_chkShift->Disable();
		m_chkWindow->Disable();
	}

	switch(p->prompt)
	{
	case 1:
		Rb1->SetValue(true);
		break;
	case 2:
		Rb2->SetValue(true);
		break;
	case 3:
		Rb3->SetValue( true);
		break;
	}

	string str = p->key1;
	m_combox->SetValue(str);

	str = p->key2;
	if( str!="")
		m_chkAlt->SetValue(true);

	str = p->key3;
	if( str!= "")
		m_chkCtrl->SetValue(true);

	str = p->key4;
	if( str!= "" )
		m_chkShift->SetValue(true);

	str = p->key5;
	if( str != "")
		m_chkWindow->SetValue(true);

}
void  J_XiTongCanShuDlg::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	SystemParamInfo*p = pMgr ->GetSystemParamInfo();
	if( Rb1->GetValue())
		p->prompt =1;
	else if( Rb2->GetValue())
		p->prompt =2;
	else
		p->prompt =3;

	m_pChkUseHideHotkey->GetValue()? p->bUse = true: p->bUse = false;

	string str = m_combox->GetValue();
	p->key1.copy(str.c_str());

	if( m_chkAlt->GetValue())
		p->key2.copy("Alt");
	else
		p->key2.copy("");

	if( m_chkCtrl->GetValue())
		p->key3.copy("Ctrl");
	else
		p->key3.copy("");

	if( m_chkShift->GetValue())
		p->key4.copy("Shift");
	else
		p->key4.copy("");

	if( m_chkWindow->GetValue()) 
		p->key5.copy("Window");
	else
		p->key5.copy("");
}

void J_XiTongCanShuDlg::OnRadioButton(wxCommandEvent& event)
{
	m_bChanged = true;
}

void J_XiTongCanShuDlg::OnPanelCharHook(wxCommandEvent& evt)
{
	wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();
	if (pEvent->GetKeyCode () == WXK_TAB)
	{
		bool backward = pEvent->ShiftDown ();
		wxWindow *win = FindFocus ();
		if (win == NULL) return;
		if(backward)
		{
			if(win==Rb3)	{Rb2->SetFocus();evt.Skip();}	
			else if(win==Rb2)	{Rb1->SetFocus();evt.Skip();}	
			else if(win==Rb1)	{m_pBtnApplyDefault->SetFocus();evt.Skip();}	
			else{}
		}
		else
		{
			if(win==m_pBtnApplyDefault)			{Rb1->SetFocus();evt.Skip();}
			else if(win==Rb1)		{Rb2->SetFocus();evt.Skip();}
			else if(win==Rb2)		{Rb3->SetFocus();evt.Skip();}
			else{}
		}
	}
}

void J_XiTongCanShuDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pStaticBox->SetLabel(LOADSTRING(CONFIG_HIDE_HOTKEY));
	m_pChkUseHideHotkey->SetLabel(LOADSTRING(CONFIG_USE_HIDE_HOTKEY));
	m_pBtnApplyDefault->SetLabel(LOADSTRING(CONFIG_APPLY_DEFAULT_SETTING));
}

J_XiTongCanShuDlg::~J_XiTongCanShuDlg()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if( pMgr != NULL)
	{
		pMgr->ClearUsableShortCutKeys();
	}
}
