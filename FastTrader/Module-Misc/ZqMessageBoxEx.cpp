/***************************************************************************//**
*   @file       ZqMessageBoxEx.cpp
*   @brief      可以保存配置的显示提示信息的对话框（模式的对话框）
*
*   @author     杨桂明 
*   @version    0.0.1
*   @date       created on 2012/06/05
********************************************************************************/

#include "stdafx.h"
#include "ZqMessageBoxEx.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "CfgMgr/CfgMgr.h"
#include "Order-Common/CLocalOrderService.h"
#include "Module-Misc/LogDefine.h"
#include "Module-Misc/TradeInfoDlg.h"
#include "Module-Misc/constants.h"
#include "Module-Misc/EventSet.h"

BEGIN_EVENT_TABLE(CZqMessageBoxEx, wxDialog)
	EVT_CLOSE(CZqMessageBoxEx::OnClose)
    EVT_TIMER(ID_TIMETIMER, OnTimer)
    EVT_BUTTON(ID_AUTOCLOSE, OnOK)
END_EVENT_TABLE()

CZqMessageBoxEx::CZqMessageBoxEx(wxWindow *parent)
:wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, 50), 
			wxDEFAULT_DIALOG_STYLE),
    m_timer(this, ID_TIMETIMER)
{
	AUTO_SETSIZE();
	m_nBtnType = noButton;
	m_nConfigType = nothing;
	m_nRemianTime = 0;
	m_nPLDetailNum = 0;
	m_nMode = 0;
	ZeroMemory(&m_tInputOrder, sizeof(m_tInputOrder));
	SetIcon(wxNullIcon);
	//m_timer.Start(1000);
}

CZqMessageBoxEx::~CZqMessageBoxEx()
{
}

BOOL CZqMessageBoxEx::NeedServCondConvertSuccessConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bConventSuccessDlg;
}

BOOL CZqMessageBoxEx::NeedServCondConvertSuccessSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bConventSuccessSound;
}

BOOL CZqMessageBoxEx::NeedServCondConvertFailConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bConventFailDlg;
}

BOOL CZqMessageBoxEx::NeedServCondConvertFailSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bConventFailSound;
}

void CZqMessageBoxEx::ResetMessage(wxString& strTitle, wxString& strMessage, int nBtnType, int nConfigType)
{
	m_nBtnType = nBtnType;
	m_nConfigType = nConfigType;
	m_strTitle = strTitle;
	m_strMessage = strMessage;
}

void CZqMessageBoxEx::CreateGUIControls()
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
				wxCommandEventHandler(CZqMessageBoxEx::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'Y', wxID_YES);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'y', wxID_YES);
	}
	if(m_nBtnType&btnNo)
	{
		pBtn = new wxButton(this, wxID_NO, LOADSTRING(ID_BUTTON_NO), 
				wxPoint(nNextLeft, dlgClientSize.y-36), wxSize(60, 26));
		pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(CZqMessageBoxEx::OnBtnClick), NULL, this);
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
				wxCommandEventHandler(CZqMessageBoxEx::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'O', wxID_OK);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'o', wxID_OK);
	}
	if(m_nBtnType&btnCancel)
	{
		pBtn = new wxButton(this, wxID_CANCEL, LOADSTRING(ID_BUTTON_CANCEL), 
				wxPoint(nNextLeft, dlgClientSize.y-36), wxSize(60, 26));
		pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(CZqMessageBoxEx::OnBtnClick), NULL, this);
		nNextLeft += 65;
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'C', wxID_CANCEL);
		entries[accindex++].Set(wxACCEL_NORMAL, (int)'c', wxID_CANCEL);
	}
	wxAcceleratorTable accel(accindex, entries);
	SetAcceleratorTable(accel);

	wxCheckBox*	pCheckConfig = new wxCheckBox( this, ID_CHECK_CONFIG, 
				LOADSTRING(CONDS2L_OverBoundFail_DLG_CHECKMSG), 
				wxPoint(nFirstLeft-15*8-10, dlgClientSize.y-30), wxSize(15*8, 16));//, 0 );
	pCheckConfig->Show(true);
    
	wxString strText;
	strText.Printf(LOADFORMATSTRING(CONDS2L_OverBoundFail_DLG_TIMEMSG, "%u"), m_nRemianTime);
	wxStaticText* pTimeLabel = new wxStaticText(this, ID_TIMELABEL, 
			strText, wxPoint(10, dlgClientSize.y-30), wxSize(12*6, 16));
	pTimeLabel->Show(false);
	
	pStaticText->Show(true);
	pTimeLabel->Show(false);
}

BOOL CZqMessageBoxEx::DoCond(BOOL bIsAuto)
{
	BOOL bConvSuccess = FALSE;
	CLocalOrderService* poLOrderService = CLocalOrderService::GetInstance();
	if(poLOrderService!=NULL)
	{
		if(m_nMode==0) 
		{
			CLocalOrderService::CONDITIONORDER tOrder;
			ZeroMemory(&tOrder, sizeof(tOrder));
			if(CLocalOrderService::ConvertServ2LocalCond(m_tInputOrder, tOrder))
			{
				poLOrderService->LockObject();
				poLOrderService->OrderConditionAdd(tOrder);
				poLOrderService->UnlockObject();
				bConvSuccess = TRUE;
			}
		}
		else
		{
			poLOrderService->LockObject();
			bConvSuccess = poLOrderService->ConvertServ2Local_PLDetail(m_nPLDetailNum, m_nMode-1);
			poLOrderService->UnlockObject();
		}
		{
			wxString strTitle;
			wxString strText;
			if(bConvSuccess)
			{
				strTitle = LOADSTRING(CONDITION_ORDER_CONVENT_SUCCESS);
				if(bIsAuto)
					strText=LOADSTRING(CONDS2L_OverBoundFail_LOG_AUTOCONV);
				else
					strText=LOADSTRING(CONDS2L_OverBoundFail_LOG_MANUALCONV);
				if(NeedServCondConvertSuccessSound()) 
					NoticeSound(conNoticeNewOrderSuccess);
			}
			else
			{
				strTitle = LOADSTRING(CONDITION_ORDER_CONVENT_FAILED);
				strText=LOADSTRING(CONDS2L_OverBoundFail_LOG_NOTCONV);
				if(NeedServCondConvertFailSound()) 
					NoticeSound(conNoticeNewOrderFail);
			}
			if(TRADEINFODLG(this)!=NULL)
			{
				if((NeedServCondConvertSuccessConfirm() && bConvSuccess)
						|| (NeedServCondConvertFailConfirm() && !bConvSuccess))
					//TRADEINFODLG(this)->ShowTradeMsg(strText, -1, 
					//		LOADSTRING(CONDS2L_OverBoundFail_LOG_TITLE), -1, TRUE);//NeedCancelFailConfirm()
					TRADEINFODLG(this)->ShowTradeMsg(strText, -1, 
							strTitle, -1, TRUE);//NeedCancelFailConfirm()
				LOG_DATA* pData = new LOG_DATA();
				pData->strTitle = std::string(LOADSTRING(CONDS2L_OverBoundFail_LOG_TITLE));
				pData->strFormat = std::string(strText.c_str());
				TRADEINFODLG(this)->WriteLog(pData);
			}
		}
	}
	return TRUE;
}

void CZqMessageBoxEx::OnClose(wxCloseEvent& event)
{
	{
		wxString strText(LOADSTRING(CONDS2L_OverBoundFail_LOG_MANUALCONV));
		if(TRADEINFODLG(this)!=NULL)
		{
			//TRADEINFODLG(this)->ShowTradeMsg(strText, -1, 
			//		LOADSTRING("取消转换"), -1, TRUE);//NeedCancelFailConfirm()
			LOG_DATA* pData = new LOG_DATA;
			pData->strTitle = std::string(LOADSTRING(CONDS2L_OverBoundFail_LOGCANCEL_TITLE));
			pData->strFormat = std::string(strText.c_str());
			TRADEINFODLG(this)->WriteLog(pData);
		}
	}
	m_timer.Stop();
	if(IsModal())
		EndModal(wxID_CANCEL);
	else
		Destroy();
}

void CZqMessageBoxEx::OnOK(wxCommandEvent& event)
{
	m_timer.Stop();

	DoCond(FALSE);

	if(IsModal())
		EndModal(wxID_YES);
	else
		Destroy();
}

void CZqMessageBoxEx::OnBtnClick(wxCommandEvent& event)
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
		case condServOverBound:
			{
				CfgMgr * pMgr = CFG_MGR_DEFAULT();  
				if(pMgr != NULL)
				{
					LPORDER_CFG p = pMgr->GetOrderCfg();
					p->bReachUpperLimitDlg = false;
				}
			}
			break;
		}
	}
	m_timer.Stop();

	if(event.GetId()==wxID_YES || event.GetId()==wxID_OK)
		DoCond(FALSE);
	else
	{
		wxString strText;
		strText.Printf(LOADSTRING(CONDS2L_OverBoundFail_LOG_MANUALCANCEL));
		if(TRADEINFODLG(this)!=NULL)
		{
			//TRADEINFODLG(this)->ShowTradeMsg(strText, -1, 
			//		LOADSTRING("取消转换"), -1, TRUE);//NeedCancelFailConfirm()
			LOG_DATA* pData = new LOG_DATA;
			pData->strTitle = std::string(LOADSTRING(CONDS2L_OverBoundFail_LOGCANCEL_TITLE));
			pData->strFormat = std::string(strText.c_str());
			TRADEINFODLG(this)->WriteLog(pData);
		}
	}
	
	if(IsModal())
		EndModal(event.GetId());
	else
		Destroy();
}

bool CZqMessageBoxEx::Show(bool show)
{
    if(show)
    {
		CreateGUIControls();
		static int nShowPos = 0;
		nShowPos+=32;
		if(nShowPos>10*32)
			nShowPos = 0;
		SetPosition(wxPoint(nShowPos, nShowPos));
		//wxDialog::CentreOnScreen();
    }

	return wxDialog::Show(show);
}

void CZqMessageBoxEx::OnTimer(wxTimerEvent& event)
{
	if(m_nRemianTime>0)
		m_nRemianTime--;
	wxWindow* pWnd = FindWindowById(ID_TIMELABEL, this);
	if(pWnd!=NULL) 
	{
		wxString strText;
		strText.Printf(LOADFORMATSTRING(CONDS2L_OverBoundFail_DLG_TIMEMSG, "%u"), m_nRemianTime);
		pWnd->SetLabel(strText);
	}
	if(m_nRemianTime==0)
	{
		m_timer.Stop();
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_AUTOCLOSE);	
		AddPendingEvent(evt);
	}
}