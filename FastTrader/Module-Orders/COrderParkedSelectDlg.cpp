
#include "stdafx.h"
#include "COrderParkedSelectDlg.h"
#include "wx\statline.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"

extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(COrderParkedSelectDlg, wxDialog)
	EVT_CLOSE(OnClose)
	EVT_BUTTON(ID_ORDERPARKEDSELECTDLG_YES, OnBtnYes)
	EVT_BUTTON(ID_ORDERPARKEDSELECTDLG_NO, OnBtnNo)
END_EVENT_TABLE()

COrderParkedSelectDlg::COrderParkedSelectDlg(wxWindow *parent, bool bIsMarketPrice)
:wxDialog(parent, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(320,255))
{
	DIALOG_CREATE_LOG();
    m_bIsMarketPrice = bIsMarketPrice;
	AUTO_SETSIZE();
	CreateGUIControls();
	SetIcon(wxNullIcon);
	Center();
}

COrderParkedSelectDlg::~COrderParkedSelectDlg()
{
}

int COrderParkedSelectDlg::ComputeLineNum(char* pStr)
{
	int nLen=strlen(pStr);
	if(nLen==0) return 0;

	int nNum=1;
	for(int i=0; i<nLen; i++) {
		if(pStr[i]=='\n') nNum++;
	}
	return nNum;
}

void COrderParkedSelectDlg::CreateGUIControls()
{

	SetBackgroundColour(DEFAULT_COLOUR);

	wxFont oldfont=GetFont();
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	wxPoint posInit(5+10, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+4+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	ctlPos = wxPoint(posInit.x, posInit.y+2);
	ctlSz.Set(nFW*30, nFH);
    m_poMarketType_Message = new wxStaticText(this, ID_ORDERPARKEDSELECTDLG_MARKETMSG, 
			LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE_ENTER), ctlPos);

    int nHeight = m_poMarketType_Message->GetSize().y;
    nHeight += nVH/2;

    if(!m_bIsMarketPrice)
    {
        m_poMarketType_Message->Show(false);
        nHeight = 0;
    }

	ctlPos = wxPoint(posInit.x, posInit.y+nHeight+2);//nVH+2);
	ctlSz.Set(nFW*30, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton(this, 
			ID_ORDERPARKEDSELECTDLG_LOCAL_HANDY, LOADSTRING(ID_ORDERPARKEDSELECTDLG_LOCAL_HANDY), ctlPos, wxDefaultSize, wxRB_GROUP);//预埋(本地)，手动发出
	itemRadioBox1->SetValue(true);
	ctlPos = wxPoint(posInit.x, posInit.y+nHeight+nVH+2);
	ctlSz.Set(nFW*30, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton(this, 
			ID_ORDERPARKEDSELECTDLG_LOCAL_AUTO, LOADSTRING(ID_ORDERPARKEDSELECTDLG_LOCAL_AUTO), ctlPos, wxDefaultSize);//预埋(本地)，当重新进入交易状态时//, wxRB_GROUP);
	ctlPos = wxPoint(posInit.x, posInit.y+nHeight+nVH*2+2);
	ctlSz.Set(nFW*30, nFH);
	wxRadioButton* itemRadioBox3 = new wxRadioButton(this, 
			ID_ORDERPARKEDSELECTDLG_SERVER, LOADSTRING(ID_ORDERPARKEDSELECTDLG_SERVER), ctlPos, wxDefaultSize);//预埋(服务器)//, wxRB_GROUP);
	if(DEFAULT_SVR()&&!DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_CONDITION_SERVER)) {
		itemRadioBox3->Show(false);
	}
	wxStaticLine* line1 = new wxStaticLine(this, wxID_ANY, wxPoint(posInit.x, posInit.y+nHeight+nVH*3+2), wxSize(260-35, -1), wxLI_HORIZONTAL);
	
	posInit.x += nFW*12;
	posInit.y = 5+nVH*3+nHeight+nVH/2;

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ORDERPARKEDSELECTDLG_YES, LOADSTRING(BTN_YES_TEXT), ctlPos, ctlSz, 0 );//是(&Y)

	ctlPos = wxPoint(posInit.x+nFW*9, posInit.y);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ORDERPARKEDSELECTDLG_NO, LOADSTRING(BTN_NO_TEXT), ctlPos, ctlSz, 0 );//否(&N)

	SetSize(260, posInit.y+26*2+20);

	wxAcceleratorEntry entries[4];
	entries[0].Set(wxACCEL_NORMAL, (int)'Y', ID_ORDERPARKEDSELECTDLG_YES);
	entries[1].Set(wxACCEL_NORMAL, (int)'N', ID_ORDERPARKEDSELECTDLG_NO);
	entries[2].Set(wxACCEL_NORMAL, (int)'y', ID_ORDERPARKEDSELECTDLG_YES);
	entries[3].Set(wxACCEL_NORMAL, (int)'n', ID_ORDERPARKEDSELECTDLG_NO);
	wxAcceleratorTable accel(4, entries);
	this->SetAcceleratorTable(accel);

}

void COrderParkedSelectDlg::OnBtnYes(wxCommandEvent& event)
{
	EndModal(wxID_YES);
}

void COrderParkedSelectDlg::OnBtnNo(wxCommandEvent& event)
{
	EndModal(wxID_NO);
}

void COrderParkedSelectDlg::OnClose(wxCloseEvent& event)
{
	EndModal(wxID_NO);
}

bool COrderParkedSelectDlg::Show(bool show)
{
    return wxDialog::Show(show);
}

void COrderParkedSelectDlg::OnCfgChanged(wxCommandEvent& evt)
{
}

// 仅此用于日志文件输出
wxString COrderParkedSelectDlg::GetUserInputText()
{
	wxString strRet;

	strRet = "预埋单数据：\n";

	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_ORDERPARKEDSELECTDLG_LOCAL_HANDY,this);
		if(window->GetValue()) {
			strRet+="模式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}
	
	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_ORDERPARKEDSELECTDLG_LOCAL_AUTO,this);
		if(window->GetValue()) {
			strRet+="模式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}

	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_ORDERPARKEDSELECTDLG_SERVER,this);
		if(window->GetValue()) {
			strRet+="模式=[";
			strRet+=window->GetLabel();
			strRet+="],\t ";
		}
	}

	strRet+="\n";
	return strRet;

}