/***************************************************************************//**
*   @file       ZqMessageBox.cpp
*   @brief      可以保存配置的显示提示信息的对话框（模式的对话框）
*
*   @author     杨桂明 
*   @version    0.0.1
*   @date       created on 2012/06/05
********************************************************************************/

#include "stdafx.h"
#include "ZqMessageBox.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "CfgMgr/CfgMgr.h"

BEGIN_EVENT_TABLE(CZqMessageBox, wxDialog)
	EVT_CLOSE(CZqMessageBox::OnClose)
END_EVENT_TABLE()

CZqMessageBox::CZqMessageBox(wxWindow *parent)
:wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, 50), 
			wxDEFAULT_DIALOG_STYLE)
{
	AUTO_SETSIZE();
	m_nBtnType = noButton;
	m_nConfigType = nothing;
	SetIcon(wxNullIcon);
}

CZqMessageBox::~CZqMessageBox()
{
}

void CZqMessageBox::ResetMessage(wxString& strTitle, wxString& strMessage, int nBtnType, int nConfigType)
{
	m_nBtnType = nBtnType;
	m_nConfigType = nConfigType;
	m_strTitle = strTitle;
	m_strMessage = strMessage;
}

void CZqMessageBox::CreateGUIControls()
{
    SetBackgroundColour(*wxWHITE);
    wxSize sz=GetClientSize();

	SetTitle(m_strTitle);

    wxStaticText* pStaticText = new wxStaticText(this, ID_MESSAGE_TEXT, 
			m_strMessage, wxPoint(20, 20), wxDefaultSize);	
	wxSize textSize = pStaticText->GetSize();
	pStaticText->Show(false);
	
	wxSize dlgClientSize = GetClientSize();
	wxSize dlgSize = GetSize();
	int nSize = dlgSize.x-dlgClientSize.x + textSize.x+40;
	if(nSize<250)
		nSize = 250;
	SetSize(nSize, dlgSize.y-dlgClientSize.y + textSize.y+40+40);
	dlgClientSize = GetClientSize();
	pStaticText->SetPosition(wxPoint(20, 20));

	int nBtnCount = 0;
	nBtnCount += m_nBtnType&btnOK ? 1:0;
	nBtnCount += m_nBtnType&btnCancel ? 1:0;
	nBtnCount += m_nBtnType&btnYes ? 1:0;
	nBtnCount += m_nBtnType&btnNo ? 1:0;

	int nFirstLeft = dlgClientSize.x - nBtnCount*(65)-5;
	int nNextLeft = nFirstLeft;

	int accindex=0;
	wxAcceleratorEntry entries[12];
	wxButton* pBtn = NULL;
	if(m_nBtnType&btnYes)
	{
		pBtn = new wxButton(this, wxID_YES, LOADSTRING(ID_BUTTON_YES), 
				wxPoint(nNextLeft, dlgClientSize.y-36), wxSize(60, 26));
		pBtn->SetFocus();
		pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(CZqMessageBox::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'Y', wxID_YES);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'y', wxID_YES);
	}
	if(m_nBtnType&btnNo)
	{
		pBtn = new wxButton(this, wxID_NO, LOADSTRING(ID_BUTTON_NO), 
				wxPoint(nNextLeft, dlgClientSize.y-36), wxSize(60, 26));
		pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(CZqMessageBox::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'N', wxID_NO);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'n', wxID_NO);
	}
	if(m_nBtnType&btnOK)
	{
		pBtn = new wxButton(this, wxID_OK, LOADSTRING(ID_BUTTON_OK), 
				wxPoint(nNextLeft, dlgClientSize.y-36), wxSize(60, 26));
		pBtn->SetFocus();
		pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(CZqMessageBox::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'O', wxID_OK);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'o', wxID_OK);
	}
	if(m_nBtnType&btnCancel)
	{
		pBtn = new wxButton(this, wxID_CANCEL, LOADSTRING(ID_BUTTON_CANCEL), 
				wxPoint(nNextLeft, dlgClientSize.y-36), wxSize(60, 26));
		pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(CZqMessageBox::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'C', wxID_CANCEL);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'c', wxID_CANCEL);
	}
	wxAcceleratorTable accel(accindex, entries);
	SetAcceleratorTable(accel);

	wxCheckBox*	pCheckConfig = new wxCheckBox( this, ID_CHECK_CONFIG, 
				LOADSTRING(ID_CONFIRMDLG_CONFIG), 
				wxPoint(nFirstLeft-15*6-10, dlgClientSize.y-30), wxSize(15*6, 16));//, 0 );
	pStaticText->Show(true);
}

void CZqMessageBox::OnClose(wxCloseEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CZqMessageBox::OnBtnClick(wxCommandEvent& event)
{
	BOOL bIsCheck = FALSE;
	wxCheckBox* pCheck = (wxCheckBox*)FindWindowById(ID_CHECK_CONFIG, this);
	if(pCheck!=NULL)
		bIsCheck = pCheck->GetValue();

	if(bIsCheck && (event.GetId()==wxID_OK || event.GetId()==wxID_YES))
	{
		switch(m_nConfigType)
		{
		case newOrder:
			{
				CfgMgr * pMgr = CFG_MGR_DEFAULT();  
				if(pMgr != NULL)
				{
					LPORDER_CFG p = pMgr->GetOrderCfg();
					p->bOrderConfirm = false;
				}
			}
			break;
		case cancelOrder:
			{
				CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
				if(pCfgMgr != NULL)
				{
					CANCEL_ORDER_CFG& cancelOrderCfg = pCfgMgr->GetCancelOrderCfg(); 
					cancelOrderCfg.bCancelConfirm = false;
					pCfgMgr->SetCancelOrderCfg(cancelOrderCfg);
				}
			}
			break;
		case placeOrder:
			break;
		case deleteInstrumentID:
			{
				CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
				if(pCfgMgr != NULL)
				{
					pCfgMgr->GetMainWindowCfg()->bDelInstrumentWarning = false;
				}
			}
			break;
		}
	}
	EndModal(event.GetId());

}

bool CZqMessageBox::Show(bool show)
{
    if(show)
    {
		CreateGUIControls();
		wxDialog::CentreOnScreen();
    }

	return wxDialog::Show(show);
}
