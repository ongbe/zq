#include "StdAfx.h"
#include "CModifyOrderDlg.h"
#include "CMovePositionDlg.h"
#include "CSwapPositionDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

//////////////////////////////////////////////////////////////////////////
//CModifyOrderDlg

BEGIN_EVENT_TABLE(CModifyOrderDlg,wxDialog)
EVT_BUTTON(ID_WXBUTTON1,OnOK)
EVT_BUTTON(ID_WXBUTTON2,OnCancle)
EVT_CLOSE(OnClose)
END_EVENT_TABLE()


CModifyOrderDlg::CModifyOrderDlg(wxWindow *parent)
	: wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(212,411)), m_pListCtrl(NULL)
{
	DIALOG_CREATE_LOG();
	m_pDlg = NULL;
	m_bIsMove = TRUE;
	CreateGUIControls();
}

CModifyOrderDlg::~CModifyOrderDlg()
{
} 

void CModifyOrderDlg::InsertModify(char * pText)
{
	if (m_pListCtrl)
	{
		int nindex = m_pListCtrl->GetItemCount();
		//m_pListCtrl->SetItem(nindex,0, wxString(pText,wxConvUTF8));
		m_pListCtrl->InsertItem(nindex,wxString(pText,wxConvUTF8));
	}
}

void CModifyOrderDlg::Clear()
{
	if (m_pListCtrl)
	{
		m_pListCtrl->DeleteAllItems();
	}
}

void CModifyOrderDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	new wxButton(this, ID_WXBUTTON1, wxT("确定"), wxPoint(13, 343), wxSize(80, 20), 0, wxDefaultValidator, wxT("WxButton1"));
	new wxButton(this, ID_WXBUTTON2, wxT("取消"), wxPoint(110, 343), wxSize(80, 20), 0, wxDefaultValidator, wxT("WxButton2"));

	m_pListCtrl = new wxListCtrl(this, ID_WXLISTCTRL1, wxPoint(0, 0), wxSize(206, 334), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));
	m_pListCtrl->InsertColumn(0,wxT("修改记录"),wxLIST_FORMAT_CENTRE,204 );

	SetTitle(wxT("修改动作"));
	SetIcon(wxNullIcon);
	SetSize(9,32,212,411);
	Center();

	////GUI Items Creation End
}

void CModifyOrderDlg::OnOK(wxCommandEvent& event)
{

	if(m_pDlg != NULL)
	{


		if(m_bIsMove) 
		{

			wxCommandEvent myEvent(wxEVT_CHILD_FOCUS);
			((CMovePositionDlg*)m_pDlg)->ProcessEvent(myEvent);

			wxMessageDialog dialog( NULL, wxT("是否确认修改？"), _("提示"), wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES)
				return;

			CMovePositionDlg* pDlg = (CMovePositionDlg*)m_pDlg;
			pDlg->ReSendRequest();
		}
		else
		{
			wxCommandEvent myEvent(wxEVT_CHILD_FOCUS);
			((CSwapPositionDlg*)m_pDlg)->ProcessEvent(myEvent);

			wxMessageDialog dialog( NULL, wxT("是否确认修改？"), _("提示"), wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES)
				return;

			CSwapPositionDlg* pDlg = (CSwapPositionDlg*)m_pDlg;
			pDlg->ReSendRequest();
		}
	}


	Hide();

}

void CModifyOrderDlg::OnCancle(wxCommandEvent& event)
{

	if (m_pDlg != NULL)
	{
		if (m_bIsMove)
		{
			CMovePositionDlg* pDlg = (CMovePositionDlg*)m_pDlg;
			pDlg->RollBack();
		}
		else
		{
			CSwapPositionDlg* pDlg = (CSwapPositionDlg*)m_pDlg;
			pDlg->RollBack();
		}
		
	}


	Hide();
}

void CModifyOrderDlg::OnClose(wxCloseEvent& evt)
{
	wxCommandEvent event;
	OnCancle(event);
}