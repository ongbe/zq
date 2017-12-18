#include "stdafx.h"

#include <wx/fontdlg.h>
#include "Module-Misc2/globalDefines.h"
#include "cfgMgr/CfgGlobal.h"
#include "cfgMgr/ListCfg.h"
#include "cfgMgr/ListCfgMgr.h"

#include "CFontAndColorSetPanel.h"

BEGIN_EVENT_TABLE(CFontAndColorSetPanel,wxPanel)
	EVT_BUTTON(ID_BTN_QUOTEFONT,OnBtnQuoteFont)
	EVT_BUTTON(ID_BTN_OTHERLISTFONT,OnBtnOtherListFont)

	EVT_COMBOBOX(ID_CBX_COLORPLAN, OnChangeSel)  
	EVT_COLOURPICKER_CHANGED(ID_COLOR_QUOTE_TEXT,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_QUOTE_BACKGROUND,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_QUOTE_ODDBACK,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_QUOTE_EVENBACK,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_QUOTE_TITLETEXT,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_QUOTE_TITLEBACK,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_INSTRUMENT,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_OTHERLIST_TEXT,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_OTHERLIST_BACK,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_OTHERLIST_ODDBACK,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_OTHERLIST_EVENBACK,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_OTHERLIST_TITLETEXT,OnColorChange)
	EVT_COLOURPICKER_CHANGED(ID_COLOR_OTHERLIST_TITLEBACK,OnColorChange)
END_EVENT_TABLE()

CFontAndColorSetPanel::CFontAndColorSetPanel(wxWindow *parent,
								   wxWindowID winid ,
								   const wxPoint& pos ,
								   const wxSize& size ,
								   long style,
								   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	//列表配色方案
	new wxStaticText(this, wxID_ANY, LOADSTRING(COLOR_PLAN), wxPoint(12, 1), wxDefaultSize, 0);
	m_cbxColorPlan = new wxComboBox(this, ID_CBX_COLORPLAN, wxEmptyString, wxPoint(117, -1), wxSize(145, 20), 0, 0);
	m_cbxColorPlan->Append(LOADSTRING(CURRENT_PLAN));
	m_cbxColorPlan->Append(LOADSTRING(DEFAULT_PLAN));
	m_cbxColorPlan->Append(LOADSTRING(BLACK_QUOTE_1));
	m_cbxColorPlan->Append(LOADSTRING(BLACK_QUOTE_2));
	m_cbxColorPlan->Append(LOADSTRING(Q7_DEFAULT));
	m_cbxColorPlan->Select(0);
	m_strCurStyle = LOADSTRING(CURRENT_PLAN);

	new wxStaticBox(this, wxID_ANY, LOADSTRING(QUOTE_TABLE_FC), wxPoint(3, 30), wxSize(428, 138));
	m_btnQuoteFont = new wxButton(this, ID_BTN_QUOTEFONT, wxString(LOADSTRING(FONT_MAOHAO))+"Arial(12)", wxPoint(18, 53), wxSize(104, 25), 0);

	new wxStaticBox(this, wxID_ANY, LOADSTRING(OTHER_TABLE_FC), wxPoint(4, 198), wxSize(435, 130));
	m_btnOtherListFont = new wxButton(this, ID_BTN_OTHERLISTFONT,wxString(LOADSTRING(FONT_MAOHAO))+"Arial(11)", wxPoint(16, 221), wxSize(104, 25), 0);

	ReadCfg();

	//报价表字体颜色设置
	new wxStaticText(this, wxID_ANY, LOADSTRING(TEXT_COLOR), wxPoint(295, 63), wxDefaultSize, 0);
	m_quoteTextColor = new wxColourPickerCtrl(this, ID_COLOR_QUOTE_TEXT, m_defaultQuoteTextColor, 
		wxPoint(354, 58), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);
	
	new wxStaticText(this, wxID_ANY, LOADSTRING(BG_COLOR), wxPoint(18, 98), wxDefaultSize, 0);
	m_quoteBackgroungColor = new wxColourPickerCtrl(this, ID_COLOR_QUOTE_BACKGROUND, m_defaultQuoteBackColor, 
		wxPoint(77, 93), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ODD_BG), wxPoint(164, 98), wxDefaultSize, 0);
	m_quoteOddBackColor = new wxColourPickerCtrl(this, ID_COLOR_QUOTE_ODDBACK, m_defaultQuoteOddBackColor, 
		wxPoint(215, 93), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(EVEN_BG), wxPoint(295, 98), wxDefaultSize, 0);
	m_quoteEvenBackColor = new wxColourPickerCtrl(this, ID_COLOR_QUOTE_EVENBACK, m_defaultQuoteEvenBackColor, 
		wxPoint(354, 93), wxSize(80, 24),wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(TITLE_TEXT), wxPoint(18, 130), wxDefaultSize, 0);
	m_quoteTitleTextColor = new wxColourPickerCtrl(this, ID_COLOR_QUOTE_TITLETEXT, m_defaultQuoteTitleTextColor, 
		wxPoint(78, 125), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(TITLE_BG), wxPoint(165, 130), wxDefaultSize, 0);
	m_quoteTitleBackgroundColor = new wxColourPickerCtrl(this, ID_COLOR_QUOTE_TITLEBACK, m_defaultQuoteTitleBackColor, 
		wxPoint(215, 125), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(CONTRACTID2), wxPoint(295, 130), wxDefaultSize, 0);
	m_instrumentColor = new wxColourPickerCtrl(this, ID_COLOR_INSTRUMENT, m_defaultInstrumentColor, 
		wxPoint(354, 126), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	//其他列表字体颜色
	new wxStaticText(this, wxID_ANY, LOADSTRING(TEXT_COLOR), wxPoint(295, 226), wxDefaultSize, 0);
	m_otherListTextColor = new wxColourPickerCtrl(this, ID_COLOR_OTHERLIST_TEXT, m_defaultOtherListTextColor, 
		wxPoint(354, 221), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(BG_COLOR), wxPoint(18, 261), wxDefaultSize, 0);
	m_otherListBackColor = new wxColourPickerCtrl(this, ID_COLOR_OTHERLIST_BACK, m_defaultOtherListBackColor, 
		wxPoint(74, 256), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ODD_BG), wxPoint(160, 261), wxDefaultSize, 0);
	m_otherListOddBackColor = new wxColourPickerCtrl(this, ID_COLOR_OTHERLIST_ODDBACK, m_defaultOtherListOddBackColor, 
		wxPoint(215, 255), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(EVEN_BG), wxPoint(297, 261), wxDefaultSize, 0);
	m_otherListEvenBackColor = new wxColourPickerCtrl(this, ID_COLOR_OTHERLIST_EVENBACK, m_defaultOtherListEvenBackColor, 
		wxPoint(355, 254), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(TITLE_TEXT), wxPoint(18, 291), wxDefaultSize, 0);
	m_otherListTitleTextColor = new wxColourPickerCtrl(this, ID_COLOR_OTHERLIST_TITLETEXT, m_defaultOtherListTitleTextColor, 
		wxPoint(75, 286), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	new wxStaticText(this, wxID_ANY, LOADSTRING(TITLE_BG), wxPoint(160, 291), wxDefaultSize, 0);
	m_otherListTitleBackColor = new wxColourPickerCtrl(this, ID_COLOR_OTHERLIST_TITLEBACK, m_defaultOtherListTitleBackColor, 
		wxPoint(215, 285), wxSize(80, 24), wxCLRP_DEFAULT_STYLE|wxST_NO_AUTORESIZE|wxCLRP_SHOW_LABEL);

	m_curQuoteFont = m_defaultQuoteFont;
	m_curQuoteTextColor = m_defaultQuoteTextColor;
	m_curQuoteBackColor = m_defaultQuoteBackColor;
	m_curQuoteOddBackColor = m_defaultQuoteOddBackColor;
	m_curQuoteEvenBackColor = m_defaultQuoteEvenBackColor;
	m_curQuoteTitleTextColor = m_defaultQuoteTitleTextColor;
	m_curQuoteTitleBackColor = m_defaultQuoteTitleBackColor;
	m_curInstrumentColor = m_defaultInstrumentColor;

	m_curOtherListFont = m_defaultOtherListFont;
	m_curOtherListTextColor = m_defaultOtherListTextColor;
	m_curOtherListBackColor = m_defaultOtherListBackColor;
	m_curOtherListOddBackColor = m_defaultOtherListOddBackColor;
	m_curOtherListEvenBackColor = m_defaultOtherListEvenBackColor;
	m_curOtherListTitleTextColor = m_defaultOtherListTitleTextColor;
	m_curOtherListTitleBackColor = m_defaultOtherListTitleBackColor;
}

CFontAndColorSetPanel::~CFontAndColorSetPanel()
{ 
}

void CFontAndColorSetPanel::OnBtnQuoteFont(wxCommandEvent& event)
{
	wxFontData data;
	data.SetInitialFont(m_curQuoteFont);
	data.EnableEffects(false);
	wxFontDialog dlg(this,data);
	if( wxID_OK == dlg.ShowModal() )
	{
		wxFontData fontData = dlg.GetFontData();
		if ( m_curQuoteFont != fontData.GetChosenFont())
		{
			m_curQuoteFont = fontData.GetChosenFont();
			string strFontName = m_curQuoteFont.GetFaceName();
			int nFontSize = m_curQuoteFont.GetPointSize();
			char buf[100];
			memset(buf, 0, sizeof(buf));
			sprintf(buf,"%s(%d)",strFontName.c_str(),nFontSize);
			m_btnQuoteFont->SetLabel(buf);

			WriteCfg();

			wxCommandEvent myEvent( wxEVT_UPDATE_LIST_TABLE );
			myEvent.SetInt( MSG_LIST_BAOJIABIAO_CHANGE );
			GetParent()->GetParent()->AddPendingEvent(myEvent);
		}
	}
}

void CFontAndColorSetPanel::OnBtnOtherListFont(wxCommandEvent& event)
{
	wxFontData data;
	data.SetInitialFont(m_curOtherListFont);
	data.EnableEffects(false);
	wxFontDialog dlg(this,data);
	if( wxID_OK == dlg.ShowModal() )
	{
		wxFontData fontData = dlg.GetFontData();
		if ( m_curOtherListFont != fontData.GetChosenFont())
		{
			m_curOtherListFont = fontData.GetChosenFont();
			string strFontName = m_curOtherListFont.GetFaceName();
			int nFontSize = m_curOtherListFont.GetPointSize();
			char buf[100];
			memset(buf, 0, sizeof(buf));
			sprintf(buf,"%s(%d)",strFontName.c_str(),nFontSize);
			m_btnOtherListFont->SetLabel(buf);

			WriteCfg();

			wxCommandEvent myEvent( wxEVT_UPDATE_LIST_TABLE );
			myEvent.SetInt( MSG_ALL_LIST_CHANGE );
			GetParent()->GetParent()->AddPendingEvent(myEvent);
		}
	}
}

void CFontAndColorSetPanel::OnColorChange( wxColourPickerEvent & event )
{
	MSG_TYPE eMsgType = INVALID_MSG;
	switch ( event.GetId() )
	{
	case ID_COLOR_QUOTE_TEXT:
		if ( m_curQuoteTextColor != m_quoteTextColor->GetColour())
		{
			m_curQuoteTextColor = m_quoteTextColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}	
		break;
	case ID_COLOR_QUOTE_BACKGROUND:
		if ( m_curQuoteBackColor != m_quoteBackgroungColor->GetColour())
		{
			m_curQuoteBackColor = m_quoteBackgroungColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}	
		break;
	case ID_COLOR_QUOTE_EVENBACK:
		if ( m_curQuoteEvenBackColor != m_quoteEvenBackColor->GetColour())
		{
			m_curQuoteEvenBackColor = m_quoteEvenBackColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}
		break;
	case ID_COLOR_QUOTE_ODDBACK:
		if ( m_curQuoteOddBackColor != m_quoteOddBackColor->GetColour())
		{
			m_curQuoteOddBackColor = m_quoteOddBackColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}
		break;
	case ID_COLOR_QUOTE_TITLETEXT:
		if ( m_curQuoteTitleTextColor != m_quoteTitleTextColor->GetColour())
		{
			m_curQuoteTitleTextColor = m_quoteTitleTextColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}
		break;
	case ID_COLOR_QUOTE_TITLEBACK:
		if ( m_curQuoteTitleBackColor != m_quoteTitleBackgroundColor->GetColour())
		{
			m_curQuoteTitleBackColor = m_quoteTitleBackgroundColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}
		break;
	case ID_COLOR_INSTRUMENT:
		if ( m_curInstrumentColor != m_instrumentColor->GetColour())
		{
			m_curInstrumentColor = m_instrumentColor->GetColour();
			eMsgType = MSG_LIST_BAOJIABIAO_CHANGE;
		}
		break;
	case ID_COLOR_OTHERLIST_TEXT:
		if ( m_curOtherListTextColor != m_otherListTextColor->GetColour())
		{
			m_curOtherListTextColor = m_otherListTextColor->GetColour();
			eMsgType = MSG_ALL_LIST_CHANGE;
		}
		break;
	case ID_COLOR_OTHERLIST_BACK:
		if ( m_curOtherListBackColor != m_otherListBackColor->GetColour())
		{
			m_curOtherListBackColor = m_otherListBackColor->GetColour();
			eMsgType = MSG_ALL_LIST_CHANGE;
		}
		break;
	case ID_COLOR_OTHERLIST_EVENBACK:
		if ( m_curOtherListEvenBackColor != m_otherListEvenBackColor->GetColour())
		{
			m_curOtherListEvenBackColor = m_otherListEvenBackColor->GetColour();
			eMsgType = MSG_ALL_LIST_CHANGE;
		}
		break;
	case ID_COLOR_OTHERLIST_ODDBACK:
		if ( m_curOtherListOddBackColor != m_otherListOddBackColor->GetColour())
		{
			m_curOtherListOddBackColor = m_otherListOddBackColor->GetColour();
			eMsgType = MSG_ALL_LIST_CHANGE;
		}
		break;
	case ID_COLOR_OTHERLIST_TITLETEXT:
		if ( m_curOtherListTitleTextColor != m_otherListTitleTextColor->GetColour())
		{
			m_curOtherListTitleTextColor = m_otherListTitleTextColor->GetColour();
			eMsgType = MSG_ALL_LIST_CHANGE;
		}
		break;
	case ID_COLOR_OTHERLIST_TITLEBACK:
		if ( m_curOtherListTitleBackColor != m_otherListTitleBackColor->GetColour())
		{
			m_curOtherListTitleBackColor = m_otherListTitleBackColor->GetColour();
			eMsgType = MSG_ALL_LIST_CHANGE;
		}
		break;
	}

	if ( eMsgType == INVALID_MSG )
	{
		return;
	}

	WriteCfg();
	wxCommandEvent myEvent( wxEVT_UPDATE_LIST_TABLE );
	myEvent.SetInt( static_cast<int>(eMsgType) );
	GetParent()->GetParent()->AddPendingEvent(myEvent);
}


void CFontAndColorSetPanel::ReadCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	std::map<int, CListCfg*>* pListCfgMap =pMgr->GetListCfgMap();
	if( pListCfgMap->empty() ) return;

	std::map<int, CListCfg*>::iterator it = pListCfgMap->begin();
	for( ; it != pListCfgMap->end(); ++it )
	{ 
		LPLIST_TABLE_CFG pListTableCfg = NULL;
		pListTableCfg = it->second->GetListCfg();
		if ( NULL == pListTableCfg )
		{
			continue;
		}

		if( pListTableCfg->Id == GID_QUOT_TABLE)
		{
			m_defaultQuoteFont.SetNativeFontInfoUserDesc(pListTableCfg->szFont);
			string strFontName = m_defaultQuoteFont.GetFaceName();
			int nFontSize = m_defaultQuoteFont.GetPointSize();
			char buf[100];
			memset(buf, 0, sizeof(buf));
			sprintf(buf,"%s(%d)",strFontName.c_str(),nFontSize);
			m_btnQuoteFont->SetLabel(buf);
			m_curQuoteFont = m_defaultQuoteFont;

			m_defaultQuoteTextColor.Set( pListTableCfg->TextColor );
			m_defaultQuoteBackColor.Set( pListTableCfg->BackgroundColor );
			m_defaultQuoteOddBackColor.Set( pListTableCfg->OddLineBackgroundColor );
			m_defaultQuoteEvenBackColor.Set( pListTableCfg->EvenLineBackgroundColor );
			m_defaultQuoteTitleTextColor.Set( pListTableCfg->HeadColor );
			m_defaultQuoteTitleBackColor.Set( pListTableCfg->HeadBackgroundColor );

			std::vector<LPLIST_COLUMN_CFG>* pTemp = it->second->GetListColCfg();
			for( unsigned int i=0; i<(*pTemp).size(); i++ )
			{
				if( (*pTemp)[i]->id ==0)
				{
					m_defaultInstrumentColor.Set( (*pTemp)[i]->TextColor );
					break;
				}	
			}
		}
		else 
		{
			m_defaultOtherListFont.SetNativeFontInfoUserDesc(pListTableCfg->szFont);
			string strFontName = m_defaultOtherListFont.GetFaceName();
			int nFontSize = m_defaultOtherListFont.GetPointSize();
			char buf[100];
			memset(buf, 0, sizeof(buf));
			sprintf(buf,"%s(%d)",strFontName.c_str(),nFontSize);
			m_btnOtherListFont->SetLabel(buf);
			m_curOtherListFont = m_defaultOtherListFont;

			m_defaultOtherListTextColor.Set( pListTableCfg->TextColor );
			m_defaultOtherListBackColor.Set( pListTableCfg->BackgroundColor );
			m_defaultOtherListOddBackColor.Set( pListTableCfg->OddLineBackgroundColor );
			m_defaultOtherListEvenBackColor.Set( pListTableCfg->EvenLineBackgroundColor );	
			m_defaultOtherListTitleTextColor.Set( pListTableCfg->HeadColor );
			m_defaultOtherListTitleBackColor.Set( pListTableCfg->HeadBackgroundColor );
			break;
		}
	}
}

void CFontAndColorSetPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;

	std::map<int, CListCfg*>* pListCfgMap = pMgr->GetListCfgMap();
	if ( NULL == pListCfgMap )
	{
		return;
	}

	if ( pListCfgMap->empty())
	{
		return;
	}

	std::map<int, CListCfg*>::iterator it = pListCfgMap->begin();
	for( ; it != pListCfgMap->end(); ++it )
	{ 
		LPLIST_TABLE_CFG pListTableCfg = NULL;	
		pListTableCfg = it->second->GetListCfg();
		if ( pListTableCfg == NULL )
		{
			continue;
		}

		if( pListTableCfg->Id == GID_QUOT_TABLE )
		{
			pListTableCfg->TextSize = m_curQuoteFont.GetPointSize();
			memset(pListTableCfg->szFont, 0 ,sizeof(pListTableCfg->szFont));
			strncpy(pListTableCfg->szFont, m_curQuoteFont.GetNativeFontInfoUserDesc().c_str(), sizeof(pListTableCfg->szFont)-1);

			pListTableCfg->TextColor = m_curQuoteTextColor.GetPixel();
			pListTableCfg->BackgroundColor = m_curQuoteBackColor.GetPixel();
			pListTableCfg->EvenLineBackgroundColor = m_curQuoteEvenBackColor.GetPixel();
			pListTableCfg->OddLineBackgroundColor = m_curQuoteOddBackColor.GetPixel();
			pListTableCfg->HeadColor = m_curQuoteTitleTextColor.GetPixel();
			pListTableCfg->HeadBackgroundColor = m_curQuoteTitleBackColor.GetPixel();
			std::vector<LPLIST_COLUMN_CFG> * temp = it->second->GetListColCfg();
			{
				for( unsigned int i=0; i<(*temp).size(); i++ )
				{
					if( (*temp)[i]->id ==0)
					{
						(*temp)[i]->TextColor = m_curInstrumentColor.GetPixel();
					}
					else 
					{
						(*temp)[i]->TextColor = m_curQuoteTextColor.GetPixel();
					}
				}
			}
		}
		else 
		{
			pListTableCfg->TextSize = m_curOtherListFont.GetPointSize();
			memset(pListTableCfg->szFont, 0 ,sizeof(pListTableCfg->szFont));
			strncpy(pListTableCfg->szFont, m_curOtherListFont.GetNativeFontInfoUserDesc(), sizeof(pListTableCfg->szFont)-1);

			pListTableCfg->TextColor = m_curOtherListTextColor.GetPixel();
			pListTableCfg->BackgroundColor = m_curOtherListBackColor.GetPixel();
			pListTableCfg->EvenLineBackgroundColor = m_curOtherListEvenBackColor.GetPixel();
			pListTableCfg->OddLineBackgroundColor  = m_curOtherListOddBackColor.GetPixel();
			pListTableCfg->HeadColor = m_curOtherListTitleTextColor.GetPixel();
			pListTableCfg->HeadBackgroundColor = m_curOtherListTitleBackColor.GetPixel();
			std::vector<LPLIST_COLUMN_CFG> * temp = it->second->GetListColCfg();
			{
				for( unsigned int i=0; i<(*temp).size(); i++ )
				{
					(*temp)[i]->TextColor = m_curOtherListTitleTextColor.GetPixel();
				}
			}
		}
	}
}

void CFontAndColorSetPanel::OnChangeSel(wxCommandEvent& event)
{
	wxString strStyle = event.GetString();
	if ( strStyle == m_strCurStyle )
	{
		return;
	}

	DWORD t1 = GetTickCount();
	if( strStyle == m_cbxColorPlan->GetString(0))
	{
		m_curQuoteFont = m_defaultQuoteFont;
		m_curQuoteTextColor = m_defaultQuoteTextColor;
		m_curQuoteBackColor = m_defaultQuoteBackColor;
		m_curQuoteOddBackColor = m_defaultQuoteOddBackColor;
		m_curQuoteEvenBackColor = m_defaultQuoteEvenBackColor;
		m_curQuoteTitleTextColor = m_defaultQuoteTitleTextColor;
		m_curQuoteTitleBackColor = m_defaultQuoteTitleBackColor;
		m_curInstrumentColor = m_defaultInstrumentColor;

		m_curOtherListFont = m_defaultOtherListFont;
		m_curOtherListTextColor = m_defaultOtherListTextColor;
		m_curOtherListBackColor = m_defaultOtherListBackColor;
		m_curOtherListOddBackColor = m_defaultOtherListOddBackColor;
		m_curOtherListEvenBackColor = m_defaultOtherListEvenBackColor;
		m_curOtherListTitleTextColor = m_defaultOtherListTitleTextColor;
		m_curOtherListTitleBackColor = m_defaultOtherListTitleBackColor;
	}
	else if( strStyle == m_cbxColorPlan->GetString(1))
	{
		m_curQuoteFont.SetNativeFontInfoUserDesc(LOADSTRING(DEFAULT_FONT));
		m_curQuoteFont.SetPointSize(10);
		m_curQuoteTextColor = wxColor(255,255,255);
		m_curQuoteBackColor = *wxBLACK;
		m_curQuoteOddBackColor = *wxBLACK;
		m_curQuoteEvenBackColor = *wxBLACK;
		m_curQuoteTitleTextColor = wxColor(0,0,0);
		m_curQuoteTitleBackColor = wxColor(0xD4,0xD0,0xC8);
		m_curInstrumentColor = wxColor(0xFF,0xFF,0x00);

		m_curOtherListFont.SetNativeFontInfoUserDesc(LOADSTRING(DEFAULT_FONT));
		m_curOtherListFont.SetPointSize(9);
		m_curOtherListTextColor = *wxBLACK;
		m_curOtherListBackColor = *wxWHITE;
		m_curOtherListOddBackColor = wxColor(0xFF,0xFF,0xE0);
		m_curOtherListEvenBackColor = wxColor(0xE0,0xFF,0xE0);
		m_curOtherListTitleTextColor = *wxBLACK;
		m_curOtherListTitleBackColor = wxColor(0xF0,0xF3,0xF5);
	}
	else if( strStyle == m_cbxColorPlan->GetString(2))
	{
		m_curQuoteFont.SetNativeFontInfoUserDesc(LOADSTRING(DEFAULT_FONT));
		m_curQuoteFont.SetPointSize(9);
		m_curQuoteTextColor = wxColor(230,255,243);
		m_curQuoteBackColor = *wxBLACK;
		m_curQuoteOddBackColor = *wxBLACK;
		m_curQuoteEvenBackColor = *wxBLACK;
		m_curQuoteTitleTextColor = wxColor(230,255,243);
		m_curQuoteTitleBackColor = *wxBLACK;
		m_curInstrumentColor = wxColor(240,240,0);

		m_curOtherListFont = m_curQuoteFont;
		m_curOtherListTextColor = *wxBLACK;
		m_curOtherListBackColor = *wxWHITE;
		m_curOtherListOddBackColor = wxColor(224,255,224);
		m_curOtherListEvenBackColor = *wxWHITE;
		m_curOtherListTitleTextColor = *wxBLACK;
		m_curOtherListTitleBackColor = wxColor(240,243,245);
	}
	else if( strStyle == m_cbxColorPlan->GetString(3) )
	{
		m_curQuoteFont.SetNativeFontInfoUserDesc(LOADSTRING(DEFAULT_FONT));
		m_curQuoteFont.SetPointSize(9);
		m_curQuoteTextColor = wxColor(230,255,243);
		m_curQuoteBackColor = *wxBLACK;
		m_curQuoteOddBackColor = *wxBLACK;
		m_curQuoteEvenBackColor = *wxBLACK;
		m_curQuoteTitleTextColor = wxColor(230,255,243);
		m_curQuoteTitleBackColor = *wxBLACK;
		m_curInstrumentColor = wxColor(203,172,0);

		m_curOtherListFont = m_curQuoteFont;
		m_curOtherListTextColor = *wxBLACK;
		m_curOtherListBackColor = *wxWHITE;
		m_curOtherListOddBackColor = wxColor(240,240,240);
		m_curOtherListEvenBackColor = *wxWHITE;
		m_curOtherListTitleTextColor = *wxBLACK;
		m_curOtherListTitleBackColor = *wxWHITE;
	}
	else
	{
		m_curQuoteFont.SetNativeFontInfoUserDesc(LOADSTRING(DEFAULT_FONT));
		m_curQuoteFont.SetPointSize(9);
		m_curQuoteTextColor = *wxBLACK;
		m_curQuoteBackColor = *wxWHITE;
		m_curQuoteOddBackColor = *wxWHITE;
		m_curQuoteEvenBackColor = *wxWHITE;
		m_curQuoteTitleTextColor = *wxBLACK;
		m_curQuoteTitleBackColor = wxColor(128,128,128);
		m_curInstrumentColor = wxColor(112,96,16);

		m_curOtherListFont = m_curQuoteFont;
		m_curOtherListTextColor = *wxBLACK;
		m_curOtherListBackColor = *wxWHITE;
		m_curOtherListOddBackColor = *wxWHITE;
		m_curOtherListEvenBackColor = *wxWHITE;
		m_curOtherListTitleTextColor = *wxBLACK;
		m_curOtherListTitleBackColor = *wxWHITE;
	}

	DWORD t2 = GetTickCount() - t1;
	WriteCfg();

	wxCommandEvent myEvent( wxEVT_UPDATE_LIST_TABLE );
	myEvent.SetInt( MSG_ALL_LIST_CHANGE );
	GetParent()->GetParent()->AddPendingEvent(myEvent);

	ApplyColorsToUI();
	m_strCurStyle = strStyle;
}

void CFontAndColorSetPanel::ApplyColorsToUI()
{
	string strFontName = m_curQuoteFont.GetFaceName();
	int nFontSize = m_curQuoteFont.GetPointSize();
	char buf[100];
	memset(buf, 0, sizeof(buf));
	sprintf(buf,"%s(%d)",strFontName.c_str(),nFontSize);
	m_btnQuoteFont->SetLabel(buf);

	m_quoteTextColor->SetColour( m_curQuoteTextColor );
	m_quoteBackgroungColor->SetColour( m_curQuoteBackColor );
	m_quoteOddBackColor->SetColour( m_curQuoteOddBackColor );
	m_quoteEvenBackColor->SetColour( m_curQuoteEvenBackColor );
	m_quoteTitleTextColor->SetColour( m_curQuoteTitleTextColor );
	m_quoteTitleBackgroundColor->SetColour( m_curQuoteTitleBackColor );
	m_instrumentColor->SetColour( m_curInstrumentColor );

	strFontName = m_curOtherListFont.GetFaceName();
	nFontSize = m_curOtherListFont.GetPointSize();
	memset(buf, 0, sizeof(buf));
	sprintf(buf,"%s(%d)",strFontName.c_str(),nFontSize);
	m_btnOtherListFont->SetLabel(buf);

	m_otherListTextColor->SetColour( m_curOtherListTextColor );
	m_otherListBackColor->SetColour( m_curOtherListBackColor );
	m_otherListOddBackColor->SetColour( m_curOtherListOddBackColor );
	m_otherListEvenBackColor->SetColour( m_curOtherListEvenBackColor );
	m_otherListTitleTextColor->SetColour( m_curOtherListTitleTextColor );
	m_otherListTitleBackColor->SetColour( m_curOtherListTitleBackColor );
}

void CFontAndColorSetPanel::Recovery()
{
	bool bChange = false;
	if ( m_curQuoteTextColor != m_defaultQuoteTextColor)
	{
		m_curQuoteTextColor = m_defaultQuoteTextColor;
		bChange = true;
	}
	
	if ( m_curQuoteBackColor != m_defaultQuoteBackColor)
	{
		m_curQuoteBackColor = m_defaultQuoteBackColor;
		bChange = true;
	}
	
	if ( m_curQuoteOddBackColor != m_defaultQuoteOddBackColor)
	{
		m_curQuoteOddBackColor = m_defaultQuoteOddBackColor;
		bChange = true;
	}

	if ( m_curQuoteEvenBackColor != m_defaultQuoteEvenBackColor)
	{
		m_curQuoteEvenBackColor = m_defaultQuoteEvenBackColor;
		bChange = true;
	}
	
	if ( m_curQuoteTitleTextColor != m_defaultQuoteTitleTextColor)
	{
		m_curQuoteTitleTextColor = m_defaultQuoteTitleTextColor;
		bChange = true;
	}
	
	if ( m_curQuoteTitleBackColor != m_defaultQuoteTitleBackColor)
	{
		m_curQuoteTitleBackColor = m_defaultQuoteTitleBackColor;
		bChange = true;
	}
	
	if ( m_curInstrumentColor != m_defaultInstrumentColor)
	{
		m_curInstrumentColor = m_defaultInstrumentColor;
		bChange = true;
	}
	
	if ( m_curOtherListTextColor != m_defaultOtherListTextColor)
	{
		m_curOtherListTextColor = m_defaultOtherListTextColor;
		bChange = true;
	}
	
	if ( m_curOtherListBackColor != m_defaultOtherListBackColor)
	{
		m_curOtherListBackColor = m_defaultOtherListBackColor;
		bChange = true;
	}
	
	if ( m_curOtherListOddBackColor != m_defaultOtherListOddBackColor)
	{
		m_curOtherListOddBackColor = m_defaultOtherListOddBackColor;
		bChange = true;
	}
	
	if ( m_curOtherListEvenBackColor != m_defaultOtherListEvenBackColor)
	{
		m_curOtherListEvenBackColor = m_defaultOtherListEvenBackColor;
		bChange = true;
	}
	
	if ( m_curOtherListTitleTextColor != m_defaultOtherListTitleTextColor)
	{
		m_curOtherListTitleTextColor = m_defaultOtherListTitleTextColor;
		bChange = true;
	}
	
	if ( m_curOtherListTitleBackColor != m_defaultOtherListTitleBackColor)
	{
		m_curOtherListTitleBackColor = m_defaultOtherListTitleBackColor;
		bChange = true;
	}
	
	if ( bChange )
	{
		WriteCfg();
		wxCommandEvent myEvent( wxEVT_UPDATE_LIST_TABLE );
		myEvent.SetInt( MSG_ALL_LIST_CHANGE );
		GetParent()->GetParent()->AddPendingEvent(myEvent);
	}
}
