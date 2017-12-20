#include "StdAfx.h"
#include "CEditOrderParkedDailog.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(CEditOrderParkedDailog, wxDialog)
    EVT_BUTTON(ID_EDITPARKED_OK,OnOk)
    EVT_BUTTON(ID_EDITPARKED_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CEditOrderParkedDailog::CEditOrderParkedDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	//m_poOrderServiceThread(NULL),
	m_poOrderInputPanel(NULL),
	m_fPriceTick(1.0f)
{
	DIALOG_CREATE_LOG();
    AUTO_SETSIZE();
	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	ZeroMemory(&m_tParkedOrder, sizeof(COrderServiceThread::PARKEDORDER));

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	m_poOrderInputPanel = new COrderInputPanel(this);
	m_poOrderInputPanel->SetPosition(wxPoint(5, 5));
	m_poOrderInputPanel->SetSize(wxSize(180, 24*6+2));
	m_poOrderInputPanel->ResetLanguage();

	posInit.x += nFW/2;
	posInit.y += 24*6;

	ctlPos = wxPoint(posInit.x, posInit.y+6);
	ctlSz.Set(nFW*30, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton(this, 
			ID_EDITPARKED_RADIO_HANDY, LOADSTRING(BTN_MANSEND), ctlPos, wxDefaultSize, wxRB_GROUP);
	itemRadioBox1->SetValue(true);
	ctlPos = wxPoint(posInit.x, posInit.y+nVH+6);
	ctlSz.Set(nFW*30, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton(this, 
			ID_EDITPARKED_RADIO_AUTO, LOADSTRING(BTN_INTOTRADER), ctlPos, wxDefaultSize);//, wxRB_GROUP);

	posInit.x += nFW*2+6;
	posInit.y += 24*2;

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITPARKED_OK, LOADSTRING(ASKNAMEDLG_OK   ), ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITPARKED_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), ctlPos, ctlSz, 0 );

	SetSize(wxSize(180+20, 24*10+20+6));

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

	m_TabOrderVector.push_back(ID_EDITPARKED_OK);
    m_TabOrderVector.push_back(ID_EDITPARKED_CANCEL);

}

CEditOrderParkedDailog::~CEditOrderParkedDailog()
{
}

void CEditOrderParkedDailog::ShowOrderReq()
{
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tParkedOrder.req.InstrumentID,field)!=0) {
    }
	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_poOrderInputPanel->SetPriceTick(m_fPriceTick);
	m_poOrderInputPanel->ShowOrder(m_tParkedOrder.req);
}

void CEditOrderParkedDailog::ShowOrderParked()
{
	wxRadioButton* pRadio = NULL;
	if(m_tParkedOrder.bIsAuto) {
		pRadio = (wxRadioButton*)FindWindowById(ID_EDITPARKED_RADIO_AUTO, this);
		if(pRadio) pRadio->SetValue(true);
	}
	else {
		pRadio = (wxRadioButton*)FindWindowById(ID_EDITPARKED_RADIO_HANDY, this);
		if(pRadio) pRadio->SetValue(true);
	}
}

void CEditOrderParkedDailog::FillParked(COrderServiceThread::PARKEDORDER& tParked)
{
	wxRadioButton* pRadio = (wxRadioButton*)FindWindowById(ID_EDITPARKED_RADIO_AUTO, this);
	if(pRadio) {
		if(pRadio->GetValue())
			tParked.bIsAuto = TRUE;
		else
			tParked.bIsAuto = FALSE;
	}
	else {
		tParked.bIsAuto = FALSE;
	}
}

bool CEditOrderParkedDailog::Show( bool show )
{
	m_poOrderInputPanel->SetViewMode(1);
	ShowOrderReq();
	ShowOrderParked();
	wxString strText;
	strText.Printf(LOADFORMATSTRING(EOPD_ECLPO,"%d"), m_tParkedOrder.nNum);//文本必须包含%d
	this->SetLabel(strText);
    return wxDialog::Show(show);
}

void CEditOrderParkedDailog::OnOk(wxCommandEvent& event)
{
//	m_poOrderInputPanel->FillOrder(m_tParkedOrder.req);

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

	if(m_poOrderInputPanel->GetQty()<=0)
	{
		wxMessageDialog dialog(NULL,LOADSTRING(OIP_QTYBIGZERO),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	//Macro
	AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
	
	MakeReq_Order(m_tParkedOrder.req, strName, 
			m_poOrderInputPanel->GetAccount().c_str(), OrderType_FT_Immediately, 
			bIsBuy, m_poOrderInputPanel->GetOpenClose()+1, 
			m_poOrderInputPanel->GetQty(), fPrice, 0, m_poOrderInputPanel->GetHedge(), 
			bIsMarketPrice, FALSE);

	FillParked(m_tParkedOrder);
	EndModal(wxID_OK);
}

void CEditOrderParkedDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CEditOrderParkedDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CEditOrderParkedDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CEditOrderParkedDailog::OnDialogCharHook(wxKeyEvent& evt)
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
		evt.Skip();
	}
}
