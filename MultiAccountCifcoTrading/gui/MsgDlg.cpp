#include "StdAfx.h"
#include "MsgDlg.h"

BEGIN_EVENT_TABLE(MsgDlg,wxDialog)
	EVT_BUTTON(ID_PREVIOUS, OnPrevious)
	EVT_BUTTON(ID_NEXT, OnNext)
END_EVENT_TABLE()

MsgDlg::MsgDlg(wxWindow *parent)
		: wxDialog(parent, wxID_ANY, "消息", wxDefaultPosition, wxSize(400,280))
		, m_nCurMsgID(-1)
{
	m_pStaticTitle = new wxStaticText(this, ID_STATIC_TITLE, "标题", wxPoint(20, 20), wxDefaultSize);
	m_pTextTitle = new wxTextCtrl(this,ID_TEXT_TITLE, wxEmptyString, wxPoint(70, 20), wxSize(300, 20),  wxTE_READONLY );
	m_pStaticContent = new wxStaticText(this, ID_STATIC_CONTENT, "内容", wxPoint(20, 45), wxDefaultSize);
	m_pTextContent = new wxTextCtrl(this,ID_TEXT_CONTENT, wxEmptyString, wxPoint(70, 45), wxSize(300, 150),  wxTE_READONLY );

	m_pBtnPrevious = new wxButton(this, ID_PREVIOUS, "上一条", wxPoint(20, 210), wxSize(60, 20));
	m_pBtnNext = new wxButton(this, ID_NEXT, "下一条", wxPoint(95, 210), wxSize(60, 20));
	m_pBtnClose = new wxButton(this, wxID_CANCEL, "关闭", wxPoint(300, 210), wxSize(60, 20));

	m_pBtnPrevious->Enable(false);
	m_pBtnNext->Enable(false);
}

MsgDlg::~MsgDlg(void)
{
}

void MsgDlg::AddMessage( const MessageInfo& msgInfo )
{
	m_mapMsgInfo[msgInfo.nMessageID] = msgInfo;
	m_nCurMsgID = msgInfo.nMessageID;
	m_pTextTitle->SetValue(msgInfo.szTitle);
	m_pTextContent->SetValue(msgInfo.szContent);

	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.find(m_nCurMsgID);
	if ( it != m_mapMsgInfo.end() )
	{
		if ( std::distance(m_mapMsgInfo.begin(), it) > 0 )
		{
			m_pBtnPrevious->Enable(true);
		}
		else
		{
			m_pBtnPrevious->Enable(false);
		}

		if ( std::distance(it, m_mapMsgInfo.end()) > 1 )
		{
			m_pBtnNext->Enable(true);
		}
		else
		{
			m_pBtnNext->Enable(false);
		}
	}
}

void MsgDlg::OnPrevious( wxCommandEvent& event )
{
	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.find(m_nCurMsgID);
	if ( it != m_mapMsgInfo.end() && it != m_mapMsgInfo.begin())
	{
		it--;
		m_nCurMsgID = it->second.nMessageID;
		m_pTextTitle->SetValue(it->second.szTitle);
		m_pTextContent->SetValue(it->second.szContent);

		if ( std::distance(m_mapMsgInfo.begin(), it) > 0 )
		{
			m_pBtnPrevious->Enable(true);
		}
		else
		{
			m_pBtnPrevious->Enable(false);
		}

		if ( std::distance(it, m_mapMsgInfo.end()) > 1 )
		{
			m_pBtnNext->Enable(true);
		}
		else
		{
			m_pBtnNext->Enable(false);
		}
	}

}

void MsgDlg::OnNext( wxCommandEvent& event )
{
	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.find(m_nCurMsgID);
	if ( it != m_mapMsgInfo.end() && (++it) != m_mapMsgInfo.end())
	{
		m_nCurMsgID = it->second.nMessageID;
		m_pTextTitle->SetValue(it->second.szTitle);
		m_pTextContent->SetValue(it->second.szContent);

		if ( std::distance(m_mapMsgInfo.begin(), it) > 0 )
		{
			m_pBtnPrevious->Enable(true);
		}
		else
		{
			m_pBtnPrevious->Enable(false);
		}

		if ( std::distance(it, m_mapMsgInfo.end()) > 1 )
		{
			m_pBtnNext->Enable(true);
		}
		else
		{
			m_pBtnNext->Enable(false);
		}
	}
}