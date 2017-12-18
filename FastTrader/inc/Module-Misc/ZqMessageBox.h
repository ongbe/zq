/***************************************************************************//**
*   @file       ZqMessageBox.h
*   @brief      可以保存配置的显示提示信息的对话框（模式的对话框）
*
*   @author     杨桂明 
*   @version    0.0.1
*   @date       created on 2012/06/05
********************************************************************************/
#ifndef _ZQMESSAGEBOX_H_
#define _ZQMESSAGEBOX_H_

#include <wx/button.h>

class CZqMessageBox : public wxDialog
{
public:
	enum {	nothing, newOrder, cancelOrder, placeOrder, deleteInstrumentID };
	enum {	noButton, 
			btnOK = 0x01,
			btnCancel = 0x02,
			btnYes = 0x04,
			btnNo = 0x08 };

public:
	CZqMessageBox(wxWindow *parent=NULL);
	~CZqMessageBox();

	void ResetMessage(wxString& strTitle, wxString& strMessage, int nBtnType=btnOK, int nConfigType=nothing);

    virtual bool Show(bool show = true);
	void OnBtnClick(wxCommandEvent& event);
	void OnConfigCheck_Click(wxCommandEvent& event);

	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();

private:
	enum
		{
			ID_MESSAGE_TEXT = 1001,
			ID_CHECK_CONFIG,
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

private:
	int			m_nBtnType;
	int			m_nConfigType;
	wxString	m_strTitle;
	wxString	m_strMessage;

	DECLARE_EVENT_TABLE();

};

#endif