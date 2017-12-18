
#include "stdafx.h"
#include "COrderConfirmDlg.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"

using namespace util;

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(COrderConfirmDlg, wxDialog)
	EVT_CLOSE(OnClose)
	EVT_CHAR_HOOK(OnHookKeyPress)
	EVT_BUTTON(ID_ORDERCONFIRMDLG_YES, OnBtnYes)
	EVT_BUTTON(ID_ORDERCONFIRMDLG_NO, OnBtnNo)
    EVT_TIMER(ID_ORDERCONFIRMDLG_TIMER, OnTimer)
END_EVENT_TABLE()

COrderConfirmDlg::COrderConfirmDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,wxEmptyString, wxDefaultPosition, wxSize(320,255)),
m_timer(this,ID_ORDERCONFIRMDLG_TIMER),
m_bIsAuto(FALSE),
m_bIsMarketPrice(FALSE),
m_fPriceTick(0.0f)
{
	DIALOG_CREATE_LOG();
	m_fUpperLimitPrice=0.0;
	m_fLowerLimitPrice=0.0;	
	m_fUpperLimitPrice=0.0;
	m_fLowerLimitPrice=0.0;

	ClearFtdcOrderFields();

	AUTO_SETSIZE();
//	CreateGUIControls();
	SetIcon(wxNullIcon);
	Center();

    m_timer.Start(500);
}

COrderConfirmDlg::~COrderConfirmDlg()
{
}

int COrderConfirmDlg::ComputeLineNum(char* pStr)
{
	int nLen=strlen(pStr);
	if(nLen==0) return 0;

	int nNum=1;
	for(int i=0; i<nLen; i++) {
		if(pStr[i]=='\n') nNum++;
	}
	return nNum;
}

void COrderConfirmDlg::CreateGUIControls()
{

	SetBackgroundColour(DEFAULT_COLOUR);

	wxFont oldfont=GetFont();
	//oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	wxPoint posInit(10, 10);
	int nFW = 7, nFH = 14;
	int nVH = nFH+4+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;
	wxString strText;
	int nAccountLine = 0;

	ctlPos = posInit;
    ctlSz.Set(nFW*70, nFH);
	strText.Printf("%s", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
	m_pStaticConfirmMsg = new wxStaticText(this, ID_ORDERCONFIRMDLG_CONFIRMMSG_STATIC, 
			strText, ctlPos, ctlSz, wxST_NO_AUTORESIZE);
    strText="";
    if(!m_bIsMarketPrice)
    {
        m_pStaticConfirmMsg->Show(false);
    }

    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()!=PTYPE_CTP) {
		strText.Printf("%s: %s\n", LOADSTRING(Printf_AccountTitle), m_pCmbOrders[0]->order.insert.strAccount);
		nAccountLine=1;
	}
	strText+=m_strCancelInfo;

    ctlPos.y += nFH;
    ctlSz.Set(nFW*70, nFH*(nAccountLine));
	m_pStaticCancelInfo = new wxStaticText(this, ID_ORDERCONFIRMDLG_CANCELINFO_STATIC, 
			strText, ctlPos, wxDefaultSize);// wxALIGN_LEFT);
	wxSize textSize = m_pStaticCancelInfo->GetSize();
	int nTextRealHeight = textSize.y;
	if(strText.size()==0)
		nTextRealHeight=0;
	
//	posInit.x+=nFW*13;
	posInit.y+=nTextRealHeight+nVH;

	for(int i=0; i<m_nCmbOrderCount; i++) {
	    m_poOrderConfirmPanels[i] = new COrderConfirmPanel(
				this, wxID_ANY, wxPoint(posInit.x, posInit.y+22*i), wxSize(nFW*90, 22));
		m_poOrderConfirmPanels[i]->SetUpperLimitPrice(m_fUpperLimitPrice);
		m_poOrderConfirmPanels[i]->SetLowerLimitPrice(m_fLowerLimitPrice);
		m_poOrderConfirmPanels[i]->SetGroupIndex(i);
		m_poOrderConfirmPanels[i]->SetPriceTick(m_fPriceTick);
		m_poOrderConfirmPanels[i]->SetIsAuto(m_bIsAuto);
		m_poOrderConfirmPanels[i]->SetIsMarketPrice(m_bIsMarketPrice);
		m_poOrderConfirmPanels[i]->CreateGUIControls();
		if(m_pCmbOrders[i]->bIsParked)
			m_poOrderConfirmPanels[i]->ShowOrder(m_pCmbOrders[i]->order.parked);
		else
			m_poOrderConfirmPanels[i]->ShowOrder(m_pCmbOrders[i]->order.insert);
	}

	wxSize dlgSize, dlgClientSize;
	dlgSize = GetSize();
	dlgClientSize = GetClientSize();
	SetSize(580, posInit.y+m_nCmbOrderCount*22+50+dlgSize.y-dlgClientSize.y);
	dlgClientSize = GetClientSize();
	
	int nFirstLeft = dlgClientSize.x - 2*(65)-5;
	int nNextLeft = nFirstLeft;

	ctlPos = wxPoint(nNextLeft, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ORDERCONFIRMDLG_YES, LOADSTRING(BTN_YES_TEXT), ctlPos, ctlSz, 0 );
	itemTemp->SetFocus();
	wxString str = itemTemp->GetLabel();
	nNextLeft += 65;

	ctlPos = wxPoint(nNextLeft, dlgClientSize.y-36);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_ORDERCONFIRMDLG_NO, LOADSTRING(BTN_NO_TEXT), ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(nFirstLeft-15*6-10, dlgClientSize.y-30);
	wxCheckBox*	pCheckConfig = new wxCheckBox( this, ID_ORDERCONFIRMDLG_CHECK_CONFIG, 
				LOADSTRING(ID_CONFIRMDLG_CONFIG), ctlPos, wxSize(15*6, 16));//, 0 );

	int accindex=0;
	wxAcceleratorEntry entries[12];
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'Y', ID_ORDERCONFIRMDLG_YES);
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'N', ID_ORDERCONFIRMDLG_NO);
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'y', ID_ORDERCONFIRMDLG_YES);
	entries[accindex++].Set(wxACCEL_NORMAL, (int)'n', ID_ORDERCONFIRMDLG_NO);

	entries[accindex++].Set(wxACCEL_SHIFT, (int)'Y', ID_ORDERCONFIRMDLG_YES);
	entries[accindex++].Set(wxACCEL_SHIFT, (int)'N', ID_ORDERCONFIRMDLG_NO);
	entries[accindex++].Set(wxACCEL_SHIFT, (int)'y', ID_ORDERCONFIRMDLG_YES);
	entries[accindex++].Set(wxACCEL_SHIFT, (int)'n', ID_ORDERCONFIRMDLG_NO);

	entries[accindex++].Set(wxACCEL_CTRL, (int)'Y', ID_ORDERCONFIRMDLG_YES);
	entries[accindex++].Set(wxACCEL_CTRL, (int)'N', ID_ORDERCONFIRMDLG_NO);
	entries[accindex++].Set(wxACCEL_CTRL, (int)'y', ID_ORDERCONFIRMDLG_YES);
	entries[accindex++].Set(wxACCEL_CTRL, (int)'n', ID_ORDERCONFIRMDLG_NO);
	wxAcceleratorTable accel(12, entries);
	this->SetAcceleratorTable(accel);

	m_TabOrderVector.clear();
	//tab order
	for(int i=0; i<m_nCmbOrderCount; i++) {
		m_TabOrderVector.push_back(ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT+i*2);
		m_TabOrderVector.push_back(ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT+i*2);
	}

	m_TabOrderVector.push_back(ID_ORDERCONFIRMDLG_YES);
	m_TabOrderVector.push_back(ID_ORDERCONFIRMDLG_NO);

}

void COrderConfirmDlg::OnBtnYes(wxCommandEvent& event)
{
	for(int i=0; i<m_nCmbOrderCount; i++) {
		if(m_pCmbOrders[i]->bIsParked)
			m_poOrderConfirmPanels[i]->FillOrder(m_pCmbOrders[i]->order.parked);
		else
			m_poOrderConfirmPanels[i]->FillOrder(m_pCmbOrders[i]->order.insert);
	}

	wxCheckBox* pWin = (wxCheckBox*)FindWindowById(ID_ORDERCONFIRMDLG_CHECK_CONFIG, this);
	if(pWin!=NULL && pWin->GetValue())
		m_confirmChg.ChgToNotConfirm();
	EndModal(wxID_YES);
}

void COrderConfirmDlg::OnBtnNo(wxCommandEvent& event)
{
	EndModal(wxID_NO);
}

void COrderConfirmDlg::OnClose(wxCloseEvent& event)
{
    m_timer.Stop();
    EndModal(wxID_NO);
}

void COrderConfirmDlg::OnTimer(wxTimerEvent& event)
{
    bool bIsMarket = false;
    for(int i=0; i<m_nCmbOrderCount; i++) 
    {
        if(m_poOrderConfirmPanels[i]) 
        {
            if(m_poOrderConfirmPanels[i]->GetIsMarketPrice())
                bIsMarket = true;
        }
	}
    if(m_pStaticConfirmMsg)
        m_pStaticConfirmMsg->Show(bIsMarket);
}

bool COrderConfirmDlg::Show(bool show)
{
	if(show) {
		if(m_nCmbOrderCount>0) {
			CreateGUIControls();
		}
		
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

void COrderConfirmDlg::OnHookKeyPress(wxKeyEvent& evt)
{
	wxSpinEvent evtSpin;
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
	else {
		for(int nSel = 0; nSel < m_nCmbOrderCount; nSel++) {
			if(m_poOrderConfirmPanels[nSel]!=NULL)
				m_poOrderConfirmPanels[nSel]->OnHookKeyPress(evt);
		}
	}
}

void COrderConfirmDlg::OnCfgChanged(wxCommandEvent& evt)
{
}

// 仅此用于日志文件输出
wxString COrderConfirmDlg::GetUserInputText()
{
	wxString strRet;
	
    for(int i=0; i<m_nCmbOrderCount; i++) {
        if(m_poOrderConfirmPanels[i]) {
            strRet += m_poOrderConfirmPanels[i]->GetUserInputText();
        }
	}
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_ORDERCONFIRMDLG_CHECK_CONFIG,this);
	if(window) {
		strRet+=LOADSTRING(ID_CONFIRMDLG_CONFIG);
		strRet+="=[";
		if(window->GetValue()!=0)
			strRet+="是";
		else
			strRet+="否";
		strRet+="]";
	}
	return strRet;
}