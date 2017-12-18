#include "stdafx.h"
#include "cfgMgr/CfgMgr.h"
#include "J_ShuZiKuaiJieJian.h"

BEGIN_EVENT_TABLE(J_ShuZiKuaiJieJianDlg,ConfigPanelBase)
	EVT_BUTTON( ID_ADD,OnAdd)
	EVT_TEXT(ID_Text1, TextChange)
	EVT_BUTTON( ID_DEL,OnDel)
	EVT_BUTTON(ID_SAVE_AND_RETURN,OnSaveAndReturn)
	EVT_SIZE( OnSize )
	EVT_LIST_EXT_COL_RIGHT_CLICK(wxID_ANY,ColClick)
	EVT_LIST_EXT_ITEM_RIGHT_CLICK( wxID_ANY, RightClick)
	EVT_CONTEXT_MENU(OnContextMenu)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY,Sel)
	EVT_LIST_EXT_END_LABEL_EDIT(wxID_ANY, OnLabelEndEdit)
	EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturn)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()


void J_ShuZiKuaiJieJianDlg::OnReturn(wxCommandEvent& event)
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

#define wxLIST_vv (wxLCEXT_REPORT|wxLCEXT_SINGLE_SEL|wxLCEXT_TOGGLE_COLOUR)

J_ShuZiKuaiJieJianDlg::J_ShuZiKuaiJieJianDlg(wxWindow *parent,
											 wxWindowID winid ,
											 const wxPoint& pos ,
											 const wxSize& size ,
											 long style,
											 const wxString& name)
											 : ConfigPanelBase(parent,winid,pos,size,style,name)
{	
	ConfigPanelBase::CreateHeader( "CONFIG_NUMBER_ACCE" );

	int offset = 50;
	m_pStaticBox = new wxStaticBox(this, wxID_ANY, LOADSTRING(SETUP), wxPoint(7, 5+offset), wxSize(253, 134));
	m_pStaticAccelerator = new wxStaticText(this, wxID_ANY, LOADSTRING(ACCELERATOR), wxPoint(15, 29+offset), wxDefaultSize, 0, wxT("WxStaticText1"));
	Text1 = new wxTextCtrl(this, ID_Text1, wxT(""), wxPoint(120, 24+offset), wxSize(121, 19), 0, wxDefaultValidator, wxT("WxEdit1"));

	m_pStaticAcceIndicate = new wxStaticText(this, wxID_ANY, LOADSTRING(CONFIG_ACCE_INDICATE), wxPoint(15, 57+offset), wxDefaultSize, 0, wxT("WxStaticText2"));
	Text2 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(120, 52+offset), wxSize(121, 19), 0, wxDefaultValidator, wxT("WxEdit2"));

	m_pStaticAcceIntro = new wxStaticText(this, wxID_ANY, LOADSTRING(CONFIG_ACCE_INTRODUCTIONS), wxPoint(15, 87+offset), wxDefaultSize, 0, wxT("WxStaticText3"));
	Text3 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(120, 83+offset), wxSize(121, 19), 0, wxDefaultValidator, wxT("WxEdit3"));

	m_pBtnAdd = new wxButton(this, ID_ADD, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(166, 111+offset), wxSize(75, 23), 0, wxDefaultValidator, wxT("WxButton1"));

	m_pBoxEgg = new wxStaticBox(this, wxID_ANY, LOADSTRING(CONFIG_ACCE_EG), wxPoint(9, 157+offset), wxSize(254, 120));
	m_pStaticEggAcce = new wxStaticText(this, wxID_ANY, LOADSTRING(ACCELERATOR), wxPoint(19, 175+offset), wxDefaultSize, 0, wxT("WxStaticText4"));
	m_pStaticEggAcceIndicate = new wxStaticText(this, wxID_ANY, LOADSTRING(CONFIG_ACCE_INDICATE), wxPoint(92, 174+offset), wxDefaultSize, 0, wxT("WxStaticText5"));
	new wxStaticText(this, wxID_ANY, wxT("a0101"), wxPoint(95, 253+offset), wxDefaultSize, 0, wxT("WxStaticText13"));
	new wxStaticText(this, wxID_ANY, wxT("01"), wxPoint(31, 256+offset), wxDefaultSize, 0, wxT("WxStaticText12"));
	new wxStaticText(this, wxID_ANY, wxT("WT7"), wxPoint(95, 232+offset), wxDefaultSize, 0, wxT("WxStaticText11"));
	new wxStaticText(this, wxID_ANY, wxT("IF0703"), wxPoint(95, 193+offset), wxDefaultSize, 0, wxT("WxStaticText10"));
	new wxStaticText(this, wxID_ANY, wxT("cu"), wxPoint(95, 212+offset), wxDefaultSize, 0, wxT("WxStaticText9"));
	new wxStaticText(this, wxID_ANY, wxT("3"), wxPoint(31, 235+offset), wxDefaultSize, 0, wxT("WxStaticText8"));
	new wxStaticText(this, wxID_ANY, wxT("2"), wxPoint(31, 216+offset), wxDefaultSize, 0, wxT("WxStaticText7"));
	new wxStaticText(this, wxID_ANY, wxT("1"), wxPoint(31, 198+offset), wxDefaultSize, 0, wxT("WxStaticText6"));

	m_pList = new wxExtListCtrl(this, ID_List, wxPoint(273, 10+offset), wxSize(267, 341), wxLIST_vv, wxDefaultValidator, wxT("WxListCtrl1"));

	Text1->Connect( wxEVT_CHAR,wxCharEventHandler(J_ShuZiKuaiJieJianDlg::OnKey),NULL,this);

	m_pBtnDel = new wxButton(this, ID_DEL, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(550, 20+offset), wxSize(75, 25), 0, wxDefaultValidator, wxT(""));

	m_pList->InsertColumn(0,LOADSTRING(ACCELERATOR));
	m_pList->InsertColumn(1,LOADSTRING(CONFIG_ACCE_INDICATE));
	m_pList->InsertColumn(2,LOADSTRING(CONFIG_ACCE_INTRODUCTIONS));
	m_pList->SetColumnCtrlType(1,COLUMN_CTRL_TYPE_EDIT);
	m_pList->SetColumnCtrlType(2,COLUMN_CTRL_TYPE_EDIT);
	m_pList->SetColumnWidth(0,80);
	m_pList->SetColumnWidth(1,80);
	m_pList->SetColumnWidth(2,105);

	ReadCfg();
	m_bChanged =false;
}

void J_ShuZiKuaiJieJianDlg::OnKey(wxKeyEvent& evt)
{
	int code  = evt.GetKeyCode();
	bool a = false;

	if( code<48 ||code >57 )
	{
		if( code<WXK_NUMPAD0 || code>WXK_NUMPAD9 )
			if( code!=8)
				a =true;
	}

	if(!a)
	{
		evt.Skip();
	}

	m_bChanged = true;

}

void J_ShuZiKuaiJieJianDlg::TextChange(wxCommandEvent& event)
{
}

void J_ShuZiKuaiJieJianDlg::OnDel(wxCommandEvent& event)
{
	int rownum = m_pList->GetItemCount();
	for( int i=0; i< rownum; i++ )
	{
		if( m_pList->IsSelectedItem( i) )
		{
			m_pList->DeleteItem( i );
			rownum--;
			i--;
			m_bChanged = true;
		}
	}  
}

void J_ShuZiKuaiJieJianDlg::OnSaveAndReturn(wxCommandEvent& event)
{
	WriteCfg();

	CLOSE_FLOAT_PANEL();
}

bool J_ShuZiKuaiJieJianDlg::Legal( string str )
{
	int count = m_pList->GetItemCount();
	string ListText;
	for( int i=0; i<count; i++)
	{
		ListText = m_pList->GetSubItemText(i,0);
		if( ListText.substr(0,1)== str.substr(0,1))
		{  
			return false;
		}
	}

	return true;
}

void J_ShuZiKuaiJieJianDlg::OnAdd(wxCommandEvent& event)
{
	string str1 = Text1->GetValue();
	if( str1 =="")
	{
		wxMessageBox(LOADSTRING(CONFIG_ACCE_EMPTY), LOADSTRING(MAINFRAME_TITLE));
		return;
	}

	string str2 = Text2->GetValue();
	if( str2.empty())
	{
		wxMessageBox(LOADSTRING(CONFIG_ACCE_EMPTY_INDICATE), LOADSTRING(MAINFRAME_TITLE));
		return;
	}

	string str3 = Text3->GetValue();

	int count = m_pList->GetItemCount();

	int row;

	string str ;
	bool find = false;
	for( int i=0; i<count ;i++)
	{
		str = m_pList->GetSubItemText(i,0);
		if( str == str1)
		{ 
			find = true;		
			row = i;
			break;
		}
		row = i;
	}

	if(find)
	{
		m_pList->SetItem(row,0,str1);
		m_pList->SetItem(row,1,str2);
		m_pList->SetItem(row,2,str3);       
	}

	else
	{
		if( !Legal( str1 ) )
		{

			wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(ABOUTDLG_TITLE));
			return;
		}
		m_pList->InsertItem(count,str1);
		m_pList->SetItem(count,1,str2);
		m_pList->SetItem(count,2,str3);
	}

	m_bChanged = true;
}


void J_ShuZiKuaiJieJianDlg::ReadCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	// std::vector< NumberShortKey>* GetNumberShortKeyVector();
	//SystemParamInfo*p = pMgr ->GetSystemParamInfo();

	std::vector< NumberShortKey>* p = pMgr->GetNumberShortKeyVector();
	int count = p->size();

	for( int i=0; i<count; i++)
	{
		m_pList->InsertItem(i, (*p)[i].ShortKey);
		m_pList->SetItem(i,1,(*p)[i].Symbol);
		m_pList->SetItem(i,2,(*p)[i].Comment);
	}

}

void J_ShuZiKuaiJieJianDlg::WriteCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	//fSystemParamInfo*p = pMgr ->GetSystemParamInfo();
	vector< NumberShortKey>* p = pMgr->GetNumberShortKeyVector();
	p->clear();

	int count = m_pList->GetItemCount();

	NumberShortKey info;
	for( int i=0; i<count; i++)
	{
		strncpy(info.ShortKey, m_pList->GetSubItemText(i,0).c_str(), sizeof(info.ShortKey)-1);
		strncpy(info.Symbol, m_pList->GetSubItemText(i,1).c_str(), sizeof(info.Symbol)-1);
		strncpy(info.Comment, m_pList->GetSubItemText(i,2).c_str(), sizeof(info.Comment)-1);
		p->push_back(info);
	}
	//pMgr->Save();

}

void J_ShuZiKuaiJieJianDlg::ColClick(wxExtListEvent& event)
{
	event.Veto();
}

void J_ShuZiKuaiJieJianDlg::RightClick(wxExtListEvent& event)
{
	event.Veto();
}

void J_ShuZiKuaiJieJianDlg::OnContextMenu(wxContextMenuEvent& event)
{
	//event.Skip();
}
void J_ShuZiKuaiJieJianDlg::Sel(wxExtListEvent& event)
{
}

void J_ShuZiKuaiJieJianDlg::OnSize( wxSizeEvent& event )
{
	wxRect rt = this->GetRect();
	m_pList->SetSize( 267,rt.height-90);
}

void J_ShuZiKuaiJieJianDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pStaticBox->SetLabel(LOADSTRING(SETUP));
	m_pStaticAccelerator->SetLabel(LOADSTRING(ACCELERATOR));
	m_pStaticAcceIndicate->SetLabel(LOADSTRING(CONFIG_ACCE_INDICATE));
	m_pStaticAcceIntro->SetLabel(LOADSTRING(CONFIG_ACCE_INTRODUCTIONS));
	m_pBoxEgg->SetLabel(LOADSTRING(CONFIG_ACCE_EG));
	m_pStaticEggAcce->SetLabel(LOADSTRING(ACCELERATOR));
	m_pStaticEggAcceIndicate->SetLabel(LOADSTRING(CONFIG_ACCE_INDICATE));
	m_pBtnAdd->SetLabel(LOADSTRING(ASKNAMEDLG_ADD));
	m_pBtnDel->SetLabel(LOADSTRING(ASKNAMEDLG_DEL));

	m_pList->SetColumn(0,LOADSTRING(ACCELERATOR));
	m_pList->SetColumn(1,LOADSTRING(CONFIG_ACCE_INDICATE));
	m_pList->SetColumn(2,LOADSTRING(CONFIG_ACCE_INTRODUCTIONS));
}

void J_ShuZiKuaiJieJianDlg::OnLabelEndEdit( wxExtListEvent& event )
{
	wxString strText = event.GetLabel();
	if( event.GetColumn() == 1 && strText.empty() )
	{
		wxMessageBox(LOADSTRING(CONFIG_ACCE_EMPTY_INDICATE), LOADSTRING(MAINFRAME_TITLE));
		event.Veto();
		return;
	}
}
