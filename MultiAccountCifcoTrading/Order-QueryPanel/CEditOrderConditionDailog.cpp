#include "StdAfx.h"
#include "CEditOrderConditionDailog.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(CEditOrderConditionDailog, wxDialog)
    EVT_BUTTON(ID_EDITCONDITION_OK,OnOk)
    EVT_BUTTON(ID_EDITCONDITION_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CEditOrderConditionDailog::CEditOrderConditionDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	//m_poOrderServiceThread(NULL),
	m_poOrderInputPanel(NULL),
	m_poOrderConditionPanel(NULL),
	m_refConditionOrder(m_tConditionOrder),
	m_fPriceTick(1.0f)
{
	DIALOG_CREATE_LOG();
    AUTO_SETSIZE();
	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	ZeroMemory(&m_tConditionOrder, sizeof(COrderServiceThread::CONDITIONORDER));

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	m_poOrderInputPanel = new COrderInputPanel(this);
	m_poOrderInputPanel->SetPosition(wxPoint(5, 5));
	m_poOrderInputPanel->SetSize(wxSize(174, 24*6+10));
	m_poOrderInputPanel->ResetLanguage();
	m_poOrderConditionPanel = new COrderConditionPanel(this);
	m_poOrderConditionPanel->SetPosition(wxPoint(5, 5+24*6+10));
	m_poOrderConditionPanel->SetSize(wxSize(174, 22*2+10));
	m_poOrderConditionPanel->ResetLanguage();

	posInit.x += nFW*3;
	posInit.y += 24*9;

	ctlPos = wxPoint(posInit.x, posInit.y-5);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITCONDITION_OK, LOADSTRING(OK), ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y-5);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITCONDITION_CANCEL,LOADSTRING(CANCEL), ctlPos, ctlSz, 0 );

	SetSize(wxSize(180+10, 24*11+20));

	m_TabOrderVector.clear();
	//tab order
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_INSTRUMENTID_TEXT))
//		m_TabOrderVector.push_back(ID_OISTD_INSTRUMENTID_TEXT);
	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_BUYSELL_COMBOBOX))
		m_TabOrderVector.push_back(ID_OISTD_BUYSELL_COMBOBOX);
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_HEDGE_CHECKBOX))
//		m_TabOrderVector.push_back(ID_OISTD_HEDGE_CHECKBOX);
	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_OPENCLOSE_COMBOBOX))
		m_TabOrderVector.push_back(ID_OISTD_OPENCLOSE_COMBOBOX);
	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_PRICE_TEXT))
		m_TabOrderVector.push_back(ID_OISTD_PRICE_TEXT);
	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_VOLUME_TEXT))
		m_TabOrderVector.push_back(ID_OISTD_VOLUME_TEXT);
	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_ACCOUNT_COMBOCTRL))
		m_TabOrderVector.push_back(ID_OISTD_ACCOUNT_COMBOCTRL);

	m_TabOrderVector.push_back(ID_CONDITION_COMBOCONDITION);
	m_TabOrderVector.push_back(ID_CONDITION_TEXTPRICE);

	m_TabOrderVector.push_back(ID_EDITCONDITION_OK);
    m_TabOrderVector.push_back(ID_EDITCONDITION_CANCEL);

}

CEditOrderConditionDailog::~CEditOrderConditionDailog()
{
}

void CEditOrderConditionDailog::ShowOrderReq()
{
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_refConditionOrder.req.InstrumentID,field)!=0) {
    }
	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_poOrderInputPanel->SetPriceTick(m_fPriceTick);
	m_poOrderInputPanel->ShowOrder(m_refConditionOrder.req);
}

void CEditOrderConditionDailog::ShowOrderCondition()
{
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_refConditionOrder.req.InstrumentID,field)!=0) {
    }
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderConditionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_poOrderConditionPanel->SetPriceTick(m_fPriceTick);
	m_poOrderConditionPanel->SetPriceType(m_refConditionOrder.nPriceType);
	m_poOrderConditionPanel->SetConditionType(m_refConditionOrder.nConditionType);
	m_poOrderConditionPanel->SetConditionPrice(m_refConditionOrder.fConditionPrice);
}

bool CEditOrderConditionDailog::Show( bool show )
{
	
	m_poOrderInputPanel->SetViewMode(1);
	ShowOrderReq();
	ShowOrderCondition();
	wxString strText;
	strText.Printf(LOADFORMATSTRING(EOCD_ECLCO,"%d"), m_refConditionOrder.nNum);//文本必须包含%d
	this->SetLabel(strText);
    return wxDialog::Show(show);
}

void CEditOrderConditionDailog::OnOk(wxCommandEvent& event)
{
	m_refConditionOrder.nPriceType = m_poOrderConditionPanel->GetPriceType();
	m_refConditionOrder.nConditionType = m_poOrderConditionPanel->GetConditionType();
	m_refConditionOrder.fConditionPrice = m_poOrderConditionPanel->GetConditionPrice();
	//m_poOrderInputPanel->FillOrder(m_refConditionOrder.req);

	BOOL bIsMarketPrice = FALSE;
	double fPrice = 0.0f;
	string strPriceText = m_poOrderInputPanel->GetPriceText().c_str();
	string strName = m_poOrderInputPanel->GetInstrumentId().c_str();
	BOOL bIsBuy = m_poOrderInputPanel->GetDirection();

	fPrice = m_poOrderInputPanel->GetPrice();
	if(CheckSumPrice(fPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	if(CheckSumPrice(m_refConditionOrder.fConditionPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_CONDITION),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}

	if(m_poOrderInputPanel->GetQty()<=0)
	{
		wxMessageDialog dialog(NULL,LOADSTRING(OIP_QTYBIGZERO),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	//Macro
	AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
	
	MakeReq_Order(m_refConditionOrder.req, strName, 
			m_poOrderInputPanel->GetAccount().c_str(), OrderType_FT_Immediately, 
			bIsBuy, m_poOrderInputPanel->GetOpenClose()+1, 
			m_poOrderInputPanel->GetQty(), fPrice, 0, m_poOrderInputPanel->GetHedge(), 
			bIsMarketPrice, FALSE);
	
	EndModal(wxID_OK);
}

void CEditOrderConditionDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CEditOrderConditionDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CEditOrderConditionDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CEditOrderConditionDailog::OnDialogCharHook(wxKeyEvent& evt)
{
    
	wxKeyEvent* pEvent=&evt;
    if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
//		evt.Skip();
        return;
    }
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
				//evt.Skip();
                return;
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_RETURN || pEvent->GetKeyCode()==VK_RETURN)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
					//evt.Skip();
                    return;
                }
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
 					//evt.Skip();
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
					//evt.Skip();
                    return;
                }
            }
        }
    }
	else {
		if(m_poOrderInputPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderInputPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		if(m_poOrderConditionPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderConditionPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		evt.Skip();
	}
}
