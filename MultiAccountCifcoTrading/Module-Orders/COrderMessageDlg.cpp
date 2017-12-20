
#include "stdafx.h"
#include "COrderMessageDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

using namespace util;


BEGIN_EVENT_TABLE(COrderMessageDlg, wxDialog)
	EVT_CLOSE(OnClose)
	EVT_CHAR_HOOK(OnHookKeyPress)
	EVT_BUTTON(ID_ORDERMESSAGEDLG_YES, OnBtnYes)
	EVT_BUTTON(ID_ORDERMESSAGEDLG_NO, OnBtnNo)
END_EVENT_TABLE()

COrderMessageDlg::COrderMessageDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(320,255))
{
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();
	CreateGUIControls();
	SetIcon(wxNullIcon);
	Center();
}

COrderMessageDlg::~COrderMessageDlg()
{
}

int COrderMessageDlg::ComputeLineNum(char* pStr)
{
	int nLen=strlen(pStr);
	if(nLen==0) return 0;

	int nNum=1;
	for(int i=0; i<nLen; i++) {
		if(pStr[i]=='\n') nNum++;
	}
	return nNum;
}

void COrderMessageDlg::CreateGUIControls()
{

	SetBackgroundColour(DEFAULT_COLOUR);

	wxFont oldfont=GetFont();
	//oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);
	
	wxPoint posInit(10, 10);
	int nFW = 8, nFH = 16;
	int nVH = nFH+4+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxSize dlgClientSize = GetClientSize();
	wxSize dlgSize = GetSize();
	SetSize(500, dlgSize.y-dlgClientSize.y+nFH*16+4+60);
	dlgClientSize = GetClientSize();

	int nFirstLeft = dlgClientSize.x - 2*(65)-5;
	int nNextLeft = nFirstLeft;
	
	wxWindow* itemTemp = NULL;
	wxString strText;

	ctlSz.Set(nFW*60-5, nFH*16+4);
	wxTextCtrl* pText = new wxTextCtrl(this, ID_ORDERMESSAGEDLG_MESSAGETEXT, 
			strText, posInit, ctlSz, wxTE_READONLY|wxTE_MULTILINE);

	ctlPos = wxPoint(nNextLeft, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ORDERMESSAGEDLG_YES, LOADSTRING(BTN_YES_TEXT), ctlPos, ctlSz, 0 );
	itemTemp->SetFocus();
	wxString str = itemTemp->GetLabel();
	nNextLeft+=65;

	ctlPos = wxPoint(nNextLeft, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ORDERMESSAGEDLG_NO, LOADSTRING(BTN_NO_TEXT), ctlPos, ctlSz, 0 );

	wxCheckBox*	pCheckConfig = new wxCheckBox( this, ID_ORDERMESSAGEDLG_CHECK_CONFIG, 
				LOADSTRING(ID_CONFIRMDLG_CONFIG), 
				wxPoint(nFirstLeft-15*6-10, dlgClientSize.y-30), wxSize(15*6, 16));//, 0 );

	int accindex=0;
	wxAcceleratorEntry entries[12];
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'Y', ID_ORDERMESSAGEDLG_YES);
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'N', ID_ORDERMESSAGEDLG_NO);
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'y', ID_ORDERMESSAGEDLG_YES);
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'n', ID_ORDERMESSAGEDLG_NO);

	entries[accindex++].Set(wxACCEL_SHIFT, (int)'Y', ID_ORDERMESSAGEDLG_YES);
	entries[accindex++].Set(wxACCEL_SHIFT, (int)'N', ID_ORDERMESSAGEDLG_NO);
	entries[accindex++].Set(wxACCEL_SHIFT, (int)'y', ID_ORDERMESSAGEDLG_YES);
	entries[accindex++].Set(wxACCEL_SHIFT, (int)'n', ID_ORDERMESSAGEDLG_NO);

	entries[accindex++].Set(wxACCEL_CTRL, (int)'Y', ID_ORDERMESSAGEDLG_YES);
	entries[accindex++].Set(wxACCEL_CTRL, (int)'N', ID_ORDERMESSAGEDLG_NO);
	entries[accindex++].Set(wxACCEL_CTRL, (int)'y', ID_ORDERMESSAGEDLG_YES);
	entries[accindex++].Set(wxACCEL_CTRL, (int)'n', ID_ORDERMESSAGEDLG_NO);
	wxAcceleratorTable accel(12, entries);
	this->SetAcceleratorTable(accel);

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_ORDERMESSAGEDLG_YES);
	m_TabOrderVector.push_back(ID_ORDERMESSAGEDLG_NO);

}

void COrderMessageDlg::OnBtnYes(wxCommandEvent& event)
{
	BOOL bIsCheck = FALSE;
	wxCheckBox* pCheck = (wxCheckBox*)FindWindowById(ID_ORDERMESSAGEDLG_CHECK_CONFIG, this);
	if(pCheck!=NULL)
		bIsCheck = pCheck->GetValue();

	if(bIsCheck)
	{
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
		if(pCfgMgr != NULL)
		{
			CANCEL_ORDER_CFG cancelOrderCfg = pCfgMgr->GetCancelOrderCfg(); 
			cancelOrderCfg.bCancelConfirm = false;
			pCfgMgr->SetCancelOrderCfg(cancelOrderCfg);
		}
	}
	EndModal(wxYES);
}

void COrderMessageDlg::OnBtnNo(wxCommandEvent& event)
{
	EndModal(wxNO);
}

void COrderMessageDlg::OnClose(wxCloseEvent& event)
{
	EndModal(wxNO);
}

bool COrderMessageDlg::Show(bool show)
{
	if(show) {
		if(!IsShown()) {	//由隐藏变成显示
			wxDialog::CentreOnScreen();
		}
	}
    return wxDialog::Show(show);
}


#define VK_TAB		0x09  //TAB key  
#define VK_RETURN	0x0D  //ENTER key  
#define VK_LEFT		0x25  //LEFT ARROW key  
#define VK_UP		0x26  //UP ARROW key  
#define VK_RIGHT	0x27  //RIGHT ARROW key  
#define VK_DOWN		0x28  //DOWN ARROW key  

void COrderMessageDlg::OnHookKeyPress(wxKeyEvent& evt)
{
//	wxSpinEvent evtSpin;
	wxKeyEvent* pEvent=&evt;//(wxKeyEvent*)evt.GetClientData();
	if(!pEvent) return;
	wxWindow *win = FindFocus();
	if(win == NULL) {
		evt.Skip();
		return;
	}
	pEvent->SetId(win->GetId());
	if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB)
	{
		bool backward = pEvent->ShiftDown();
		int id,thisID=win->GetId(),count=m_TabOrderVector.size();
		for(id=0;id<count;id++)
		{
			if(m_TabOrderVector[id]==thisID)
			{
				if(backward)
					id=id>0?id-1:count-1;
				else
					id=id<count-1?id+1:0;
				FindWindowById(m_TabOrderVector[id],this)->SetFocus();
				return;
			}
		}
	}
	else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
	{
		int id,thisID=win->GetId(),count=m_TabOrderVector.size();
		for(id=0;id<count;id++)
		{
			if(m_TabOrderVector[id]==thisID)
			{
				if(id>0)
				{
					FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
					return;
				}
			}
		}
	}
	else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
	{
		int id,thisID=win->GetId(),count=m_TabOrderVector.size();
		for(id=0;id<count;id++)
		{
			if(m_TabOrderVector[id]==thisID)
			{
				if(id<count-1)
				{
					FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
					return;
				}
			}
		}
	}
}

void COrderMessageDlg::OnCfgChanged(wxCommandEvent& evt)
{
}
