#pragma once
#include "wx\msw\dialog.h"
#include <map>
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"

class MsgDlg :
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
	MsgDlg(wxWindow *parent);
	~MsgDlg(void);

	void AddMessage( const MessageInfo& msgInfo);

	void OnPrevious( wxCommandEvent& event);
	void OnNext( wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxStaticText* m_pStaticTitle;
	wxTextCtrl* m_pTextTitle;
	wxStaticText* m_pStaticContent;
	wxTextCtrl* m_pTextContent;
	wxButton* m_pBtnPrevious;
	wxButton* m_pBtnNext;
	wxButton* m_pBtnClose;

	std::map<int, MessageInfo> m_mapMsgInfo;
	int m_nCurMsgID;
};
