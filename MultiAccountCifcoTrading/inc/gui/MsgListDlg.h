#pragma once
#include "wx\msw\dialog.h"
#include <map>
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"

class MsgListDlg :
	public wxDialog
{
public:
	enum
	{
		ID_STATIC_TITLE,
		ID_TEXT_TITLE,
		ID_STATIC_CONTENT,
		ID_TEXT_CONTENT,
		ID_PREVIOUS,
		ID_NEXT,
		ID_CLOSE,
	};

public:
	MsgListDlg(wxWindow *parent);
	~MsgListDlg(void);

	void SetMessageInfo( std::map<int, MessageInfo> msgMap );

	void OnItemSelected(wxExtListEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxExtListCtrl* m_pMsgList;
	wxStaticText* m_pStaticContent;
	wxTextCtrl* m_pTextContent;

	std::map<int, MessageInfo> m_mapMsgInfo;
	int m_nCurMsgID;
};
