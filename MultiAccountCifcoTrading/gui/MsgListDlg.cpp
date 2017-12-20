#include "StdAfx.h"
#include "MsgListDlg.h"

BEGIN_EVENT_TABLE(MsgListDlg,wxDialog)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, OnItemSelected)
END_EVENT_TABLE()

MsgListDlg::MsgListDlg(wxWindow *parent)
		: wxDialog(parent, wxID_ANY, "当日接收消息查看", wxDefaultPosition, wxSize(600,400))
		, m_nCurMsgID(-1)
{
	m_mapMsgInfo.clear();

	m_pMsgList = new wxExtListCtrl( this, wxID_ANY, wxPoint(10, 20), 
		wxSize(150, 335), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE|wxLCEXT_SINGLE_SEL);
	m_pStaticContent = new wxStaticText(this, ID_STATIC_CONTENT, "内容", wxPoint(170, 20), wxDefaultSize);
	m_pTextContent = new wxTextCtrl(this,ID_TEXT_CONTENT, wxEmptyString, wxPoint(170, 45), wxSize(410, 310),  wxTE_READONLY );

	m_pMsgList->InsertColumn(0, "标题", wxALIGN_LEFT, 145);
}

MsgListDlg::~MsgListDlg(void)
{
}

void MsgListDlg::SetMessageInfo( std::map<int, MessageInfo> msgMap )
{
	m_mapMsgInfo = msgMap;

	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.begin();
	for ( ; it != m_mapMsgInfo.end(); it++ )
	{
		m_pMsgList->InsertItem(0, it->second.szTitle);
		m_pMsgList->SetItemData(0, it->first);
	}
}

void MsgListDlg::OnItemSelected( wxExtListEvent& event )
{
	int nIndex = event.GetIndex();
	int nMsgId = m_pMsgList->GetItemData(nIndex);
	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.find(nMsgId);
	if ( it != m_mapMsgInfo.end() )
	{
		m_pTextContent->SetValue(it->second.szContent);
	}
}
