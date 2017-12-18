#include "StdAfx.h"
#include "CEditOrderPLDetailDailog.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(CEditOrderPLDetailDailog, wxDialog)
    EVT_BUTTON(ID_EDITPLDETAIL_OK,OnOk)
    EVT_BUTTON(ID_EDITPLDETAIL_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CEditOrderPLDetailDailog::CEditOrderPLDetailDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	m_poOrderInputPanel(NULL),
	m_poOrderPositionPanel(NULL),
	m_fPriceTick(1.0f)
{
	DIALOG_CREATE_LOG();
    AUTO_SETSIZE();
	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	ZeroMemory(&m_tPLDetailOrder, sizeof(m_tPLDetailOrder));

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
	m_poOrderPositionPanel = new COrderPositionPanel(this);
	m_poOrderPositionPanel->SetPosition(wxPoint(5+174+3, -2));
	m_poOrderPositionPanel->SetSize(wxSize(210+10, 24*11));
	m_poOrderPositionPanel->ResetLanguage();

	posInit.x += nFW*3;
	posInit.y += 24*7+6;

	ctlPos = wxPoint(posInit.x, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITPLDETAIL_OK, LOADSTRING(OK), ctlPos, ctlSz, 0 );

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITPLDETAIL_CANCEL, LOADSTRING(CANCEL), ctlPos, ctlSz, 0 );

	SetSize(wxSize(180+10+195, 24*10));

	m_TabOrderVector.clear();
	//tab order
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_INSTRUMENTID_TEXT))
//		m_TabOrderVector.push_back(ID_OISTD_INSTRUMENTID_TEXT);
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_BUYSELL_COMBOBOX))
//		m_TabOrderVector.push_back(ID_OISTD_BUYSELL_COMBOBOX);
////	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_HEDGE_CHECKBOX))
////		m_TabOrderVector.push_back(ID_OISTD_HEDGE_CHECKBOX);
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_OPENCLOSE_COMBOBOX))
//		m_TabOrderVector.push_back(ID_OISTD_OPENCLOSE_COMBOBOX);
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_PRICE_TEXT))
//		m_TabOrderVector.push_back(ID_OISTD_PRICE_TEXT);
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_VOLUME_TEXT))
//		m_TabOrderVector.push_back(ID_OISTD_VOLUME_TEXT);
//	if(m_poOrderInputPanel->CheckCtrlID(ID_OISTD_ACCOUNT_COMBOCTRL))
//		m_TabOrderVector.push_back(ID_OISTD_ACCOUNT_COMBOCTRL);

	m_TabOrderVector.push_back(ID_POSITION_COMBOPRICETYPE);
	m_TabOrderVector.push_back(ID_POSITION_CHECKSTOPLOSE);
	m_TabOrderVector.push_back(ID_POSITION_TEXTSTOPLOSEPRICEOFFSET);
	m_TabOrderVector.push_back(ID_POSITION_CHECKSTOPGAIN);
	m_TabOrderVector.push_back(ID_POSITION_TEXTSTOPGAINPRICEOFFSET);
	m_TabOrderVector.push_back(ID_POSITION_TEXTDOPRICEOFFSET);

	m_TabOrderVector.push_back(ID_EDITPLDETAIL_OK);
    m_TabOrderVector.push_back(ID_EDITPLDETAIL_CANCEL);

}

CEditOrderPLDetailDailog::~CEditOrderPLDetailDailog()
{
}

void CEditOrderPLDetailDailog::ShowOrderReq()
{
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
    if(DEFAULT_SVR()
			&& DEFAULT_SVR()->GetQuotInfo(m_tPLDetailOrder.InstrumentID,field)!=0) {
    }
 	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}

	wxString strText;
	m_poOrderInputPanel->SetPriceTick(m_fPriceTick);
//	m_poOrderInputPanel->LockForSetChange();
	strText=m_tPLDetailOrder.InstrumentID;
	m_poOrderInputPanel->SetInstrumentId(strText, FALSE);
	m_poOrderInputPanel->SetDirection(m_tPLDetailOrder.orderSrc.bIsBuy);
	m_poOrderInputPanel->SetOpenClose(m_tPLDetailOrder.orderSrc.nOCMode);
	m_poOrderInputPanel->SetPrice(m_tPLDetailOrder.orderSrc.fPrice);
	m_poOrderInputPanel->SetQty(m_tPLDetailOrder.orderSrc.nVolume);
//	m_poOrderInputPanel_jsd->UnlockForSetChange();
}

void CEditOrderPLDetailDailog::ShowOrderPLDetail()
{
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()
			&&DEFAULT_SVR()->GetQuotInfo(m_tPLDetailOrder.InstrumentID,field)!=0) {
    }
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderPositionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}

	m_poOrderPositionPanel->SetIsRunInServer(m_tPLDetailOrder.bIsRunInServer);
	m_poOrderPositionPanel->SetPriceTick(m_fPriceTick);
	m_poOrderPositionPanel->SetOffsetPriceObject(m_tPLDetailOrder.plparam.nPriceOffsetType);
	m_poOrderPositionPanel->SetLoseOffsetPrice(
			m_tPLDetailOrder.plparam.bDoStopLose, m_tPLDetailOrder.plparam.fStopLosePriceOffset);
	m_poOrderPositionPanel->SetGainOffsetPrice(
			m_tPLDetailOrder.plparam.bDoStopGain, m_tPLDetailOrder.plparam.fStopGainPriceOffset);
	m_poOrderPositionPanel->SetPriceType(m_tPLDetailOrder.plparam.nPriceType);
	m_poOrderPositionPanel->SetCloseMode(m_tPLDetailOrder.plparam.nCloseMode);
	m_poOrderPositionPanel->SetCloseOffsetPrice(m_tPLDetailOrder.plparam.fClosePriceOffset);
}

void CEditOrderPLDetailDailog::ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow)
{
	TRADEINFODLG(this)->ShowTradeMsg(errormessage, -1, Title, -1, bIsShow);
}

bool CEditOrderPLDetailDailog::Show( bool show )
{
	if(show) {
		ShowOrderReq();
		ShowOrderPLDetail();
		m_poOrderInputPanel->SetViewMode(2);
		m_poOrderPositionPanel->SetViewMode(1);
		wxString strText;
		strText.Printf(LOADFORMATSTRING(EOPD_ECLSO,"%d"), m_tPLDetailOrder.FTID);//文本必须包含%d
		this->SetLabel(strText);
	}
    return wxDialog::Show(show);
}

void CEditOrderPLDetailDailog::OnOk(wxCommandEvent& event)
{
	BOOL bIsCheckLose = FALSE, bIsCheckGain = FALSE;
	double fLoseOffsetPrice = 0.0f, fGainOffsetPrice = 0.0f;
	int nCloseMode = 0;
	double fCloseOffsetPrice = 0.0;

	bIsCheckLose = m_poOrderPositionPanel->GetLoseOffsetPrice(fLoseOffsetPrice);
	bIsCheckGain = m_poOrderPositionPanel->GetGainOffsetPrice(fGainOffsetPrice);
	nCloseMode = m_poOrderPositionPanel->GetCloseMode();
	fCloseOffsetPrice = m_poOrderPositionPanel->GetCloseOffsetPrice();

	if(bIsCheckLose == FALSE && bIsCheckGain == FALSE) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_SOPPO), TRUE);
		return;
	}
	if(bIsCheckLose != FALSE && fLoseOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_STEOPPRCBIGZERO), TRUE);
		return;
	}
	if(bIsCheckGain != FALSE && fGainOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_PRCDELTABIGZERO), TRUE);
		return;
	}
	if(CheckSumPrice(fLoseOffsetPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_LOSE),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	if(CheckSumPrice(fGainOffsetPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_GAIN),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	if(CheckSumPrice(fCloseOffsetPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_CLOSEREVERSE),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	//m_tPLDetailOrder.bIsRunInServer = m_poOrderPositionPanel->GetIsRunInServer();

	m_tPLDetailOrder.plparam.nPriceOffsetType = m_poOrderPositionPanel->GetOffsetPriceObject();
	m_tPLDetailOrder.plparam.bDoStopLose = m_poOrderPositionPanel->GetLoseOffsetPrice(
			m_tPLDetailOrder.plparam.fStopLosePriceOffset);
	m_tPLDetailOrder.plparam.bDoStopGain = m_poOrderPositionPanel->GetGainOffsetPrice(
			m_tPLDetailOrder.plparam.fStopGainPriceOffset);
	m_tPLDetailOrder.plparam.nPriceType = m_poOrderPositionPanel->GetPriceType();
	m_tPLDetailOrder.plparam.nCloseMode = m_poOrderPositionPanel->GetCloseMode();
	m_tPLDetailOrder.plparam.fClosePriceOffset = m_poOrderPositionPanel->GetCloseOffsetPrice();

	//double fPriceOffset = m_tPLOrder.plparam.nPriceOffsetType==2 
	//		? 0.0 : m_tPLOrder.plparam.fOpenedPriceOffset;
	//if(m_tPLOrder.orderScr.bIsBuy)
	//	m_tPLOrder.fStopLosePrice = m_tPLOrder.orderScr.fPrice 
	//			+ fPriceOffset 
	//			- m_tPLOrder.plparam.fStopLosePriceOffset;	//止损价
	//else
	//	m_tPLOrder.fStopLosePrice = m_tPLOrder.orderScr.fPrice 
	//			+ fPriceOffset 
	//			+ m_tPLOrder.plparam.fStopLosePriceOffset;	//止损价

	//if(m_tPLOrder.orderScr.bIsBuy)
	//	m_tPLOrder.fStopGainPrice = m_tPLOrder.orderScr.fPrice 
	//			+ fPriceOffset 
	//			+ m_tPLOrder.plparam.fStopGainPriceOffset;	//止盈价
	//else
	//	m_tPLOrder.fStopGainPrice = m_tPLOrder.orderScr.fPrice 
	//			+ fPriceOffset 
	//			- m_tPLOrder.plparam.fStopGainPriceOffset;	//止盈价
	if(!m_tPLDetailOrder.plparam.bDoStopLose) {
		m_tPLDetailOrder.plparam.fStopLosePriceOffset = 0.0f;
		//m_tPLOrder.fStopLosePrice = 0.0f;
	}
	if(!m_tPLDetailOrder.plparam.bDoStopGain) {
		m_tPLDetailOrder.plparam.fStopGainPriceOffset = 0.0f;
		//m_tPLOrder.fStopGainPrice = 0.0f;
	}

	USERLOG_INFO("用户编辑盈损单明细，对话框中单击[确认]确认\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderPositionPanel->GetUserInputText().c_str());

	EndModal(wxID_OK);
}

void CEditOrderPLDetailDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CEditOrderPLDetailDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CEditOrderPLDetailDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CEditOrderPLDetailDailog::OnDialogCharHook(wxKeyEvent& evt)
{
    
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
		if(m_poOrderPositionPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderPositionPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		evt.Skip();
	}

}
