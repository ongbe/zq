/**************************************************************************
*   @file         BankTransferDlg.cpp
*   @brief        银期转账对话框的实现金士达风格
*
*   @author    ying.qiu
*   @version    0.0.1
*   @date       created on 2010/12/17
********************************************************************************/
#include "StdAfx.h"
#include "BankTranferDlg.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-FundAccount/FundAccountModule.h"//查询期货资金账户
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
//#define MaxAmount 50000.00


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

#define  TIMER_CYCLE			5000 
static CBankTransferDlg* s_pCBankTransferDlg=NULL;
static int rspOnTransferCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(!s_pCBankTransferDlg)
        return 0;
    wxCommandEvent evt(wxEVT_TRANSFER_RSP, wxID_ANY);

    switch (data.BID)
    {
	case  BID_RspFromBankToFutureByFuture://期货发起银行资金转期货应答
		EVENT_SETCLIENTDATA(DataRspFromBankToFutureByFuture);
		break;
	case   BID_RspFromFutureToBankByFuture://期货发起期货资金转银行应答
		EVENT_SETCLIENTDATA(DataRspFromFutureToBankByFuture);
		break;
	case   BID_RspQueryBankAccountMoneyByFuture://期货发起查询银行余额应答
		EVENT_SETCLIENTDATA(DataRspQueryBankAccountMoneyByFuture);
		break;
	case   BID_RspQryTransferSerial://请求查询转帐流水响应
		EVENT_SETCLIENTDATA(DataRspQryTransferSerial);
		break;
	case  BID_RtnFromBankToFutureByFuture://期货发起银行资金转期货应答
		EVENT_SETCLIENTDATA(DataRtnFromBankToFutureByFuture);
		break;
	case   BID_RtnFromFutureToBankByFuture://期货发起期货资金转银行应答
		EVENT_SETCLIENTDATA(DataRtnFromFutureToBankByFuture);
		break;
	case   BID_RtnQueryBankAccountMoneyByFuture://期货发起查询银行余额应答
		EVENT_SETCLIENTDATA(DataRtnQueryBankAccountMoneyByFuture);
		break;
	default:
		return 0;
    }

    s_pCBankTransferDlg->AddPendingEvent(evt);

    return 0;

}


BEGIN_EVENT_TABLE(CBankTransferDlg,wxPanel)
//	EVT_CLOSE(CBankTransferDlg::OnClose)
EVT_BUTTON(ID_BTN_SEND,CBankTransferDlg::OnBtnSend)
EVT_BUTTON(ID_BTN_QRY_DETAIL,CBankTransferDlg::OnBtnQryDetail)
EVT_COMMAND(wxID_ANY, wxEVT_TRANSFER_RSP,  CBankTransferDlg::OnRcvRspTransfer)
EVT_LIST_EXT_ITEM_SELECTED(ID_WXLISTCTRL1, CBankTransferDlg::OnSelected)
EVT_TIMER(ID_DELAY_TIMER,OnDelayTimer)
EVT_SIZE( CBankTransferDlg::OnSize )
EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturn)
EVT_CHOICE(ID_CHOICEUSER,OnUserChoice)
END_EVENT_TABLE()


CBankTransferDlg::CBankTransferDlg(wxWindow *parent,
                                   wxWindowID winid ,
                                   const wxPoint& pos ,
                                   const wxSize& size ,
                                   long style,
                                   const wxString& name)
                                   :wxPanel(parent,winid,pos,size,style,name)
{
	m_pDelayTimer=new wxTimer(this,ID_DELAY_TIMER);
	m_bSungardStyle = false;
    CreateGUIControls();
    s_pCBankTransferDlg=this;
}

CBankTransferDlg::~CBankTransferDlg()
{

}


void CBankTransferDlg::SetSungardStyle( bool bFlag )
{
	m_bSungardStyle = bFlag;
}

wxString CBankTransferDlg::GetWindowCaption()
{
	return LOADSTRING(BankTransfer_DLGTitle);
}

void CBankTransferDlg::CreateGUIControls()
{
    FLOAT_PANE_HEADER(LOADSTRING(RETURN_NORMAL),LOADSTRING(BankTransfer_DLGTitle)); 
	int iIndex=0;
    m_GroupBox = new wxStaticBox(this, ID_WXSTATICBOX1, wxEmptyString, wxPoint(10, 58), wxSize(901, 170));
	{//初始化左上
		new wxStaticText(this, ID_WXSTATICTEXT2, LOADSTRING(BT_DETAIL_BANKID), wxPoint(17, 93));
		m_BankId = new wxTextCtrl(this, ID_WXEDIT1, wxEmptyString, wxPoint(91, 91), wxSize(119, 19),wxTE_READONLY);
		m_BankId->SetBackgroundColour(*wxLIGHT_GREY);
		new wxStaticText(this, ID_WXSTATICTEXT3, LOADSTRING(BT_DETAIL_BANKNAME), wxPoint(250, 91));
		m_BankName = new wxTextCtrl(this, ID_WXEDIT3, wxEmptyString, wxPoint(303, 90), wxSize(132, 19), wxTE_READONLY);
		m_BankName->SetBackgroundColour(*wxLIGHT_GREY);
		new wxStaticText(this, ID_WXSTATICTEXT5, LOADSTRING(BT_STA_USEAMOUNT), wxPoint(17, 116));
		//期货账户可取资金
		double fWithdrawQuota=0;
		if(m_pSvr)m_pSvr->GetTradingAccountWithdrawQuota(fWithdrawQuota);	
		m_WithdrawQuota = new wxTextCtrl(this, ID_WXEDIT4, GlobalFunc::ConvertToString(fWithdrawQuota), wxPoint(91, 116), wxSize(121, 19), wxTE_READONLY);
		m_WithdrawQuota->SetBackgroundColour(*wxLIGHT_GREY);		
		new wxStaticText(this, ID_WXSTATICTEXT6, LOADSTRING(BT_STA_TANSTYPE), wxPoint(250, 116), wxDefaultSize, 0, wxT("WxStaticText6"));
		m_cmbType = new wxComboBox(this, ID_WXCOMBOBOX1,wxEmptyString, wxPoint(303, 116), wxSize(132, 16),NULL,wxCB_READONLY);
		m_cmbType->Append(LOADSTRING(BT_CMB_TYPE_QUERY));
		m_cmbType->Append(LOADSTRING(BT_CMB_TYPE_B2M));
		m_cmbType->Append(LOADSTRING(BT_CMB_TYPE_M2B));
		new wxStaticText(this, ID_WXSTATICTEXT7, LOADSTRING(BT_STA_ACCPSW), wxPoint(17, 142), wxDefaultSize );
		m_AccountPassword = new wxTextCtrl(this, ID_WXEDIT5,wxEmptyString, wxPoint(91, 142), wxSize(121, 19),wxTE_PASSWORD);
		new wxStaticText(this, ID_WXSTATICTEXT8, LOADSTRING(BT_STA_TANSAMNT), wxPoint(250, 142), wxDefaultSize);
		m_Amount = new wxTextCtrl(this, ID_WXEDIT6, wxEmptyString, wxPoint(303, 142), wxSize(132, 19));
		new wxStaticText(this, ID_WXSTATICTEXT9, LOADSTRING(BT_STA_BANKPSW), wxPoint(17, 165));
		m_BankPassword = new wxTextCtrl(this, ID_WXEDIT7,wxEmptyString, wxPoint(91, 165), wxSize(121, 19),wxTE_PASSWORD);
		new wxStaticText(this, ID_WXSTATICTEXT8, LOADSTRING(BT_STA_TANSAMNT), wxPoint(250, 142), wxDefaultSize);
		m_Amount = new wxTextCtrl(this, ID_WXEDIT6, wxEmptyString, wxPoint(303, 142), wxSize(132, 19));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->IsMultipleAccounts())
		{
			new wxStaticText(this, ID_WXSTATICTEXT9, "用户选择", wxPoint(250, 168));
			new wxChoice( this, ID_CHOICEUSER, wxPoint(303,168), wxSize(132, 19), MA_CORE()->GetAccountArray()  );
		}
		new wxButton(this, ID_BTN_QRY_DETAIL, LOADSTRING(BT_BTN_TRANS), wxPoint(250, 195), wxSize(86, 25));
		new wxButton(this, ID_BTN_SEND, LOADSTRING(BT_BTN_SEND), wxPoint(351, 195), wxSize(86, 25));
	}
	{//初始化右上
		m_strPromptText = new wxStaticText(this, ID_PROMPT_TEXT, LOADSTRING(BT_NO_QUERY_BANK), wxPoint(454, 67));
		m_BankList = new wxExtListCtrl(this, ID_WXLISTCTRL1, wxPoint(447, 84), wxSize(460, 130),  wxLCEXT_REPORT|wxLCEXT_VRULES);
		iIndex=0;
		m_BankList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_BANKID),wxLISTEXT_FORMAT_LEFT,100);
		m_BankList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_BANKNAME),wxLISTEXT_FORMAT_LEFT,100);
		m_BankList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_ACCOUNT),wxLISTEXT_FORMAT_LEFT,150);
		m_BankList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_CURENCY),wxLISTEXT_FORMAT_LEFT,100);
	}
	{//初始化下面
		m_DetailList = new wxExtListCtrl(this, ID_WXLISTCTRL2, wxPoint(10, 235), wxSize(900, 135), wxLCEXT_REPORT|wxLCEXT_VRULES);
		iIndex=0;
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_INDEX),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_ACCOUNT),wxLISTEXT_FORMAT_LEFT,150);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_TYPE),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_AMOUNT),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_RESULT),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_DATETIME),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_BANKNAME),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BT_DETAIL_BANKID),wxLISTEXT_FORMAT_LEFT,100);
	}

}
void CBankTransferDlg::SelectLisrRow(const int row )
{
	m_BankId->SetValue(m_BankList->GetSubItemText(row,0));
	m_BankName->SetValue(m_BankList->GetSubItemText(row,1));	
}
//发送按钮事件--转账与查询银行余额
void CBankTransferDlg::OnBtnSend(wxCommandEvent& event)
{
    int sel=m_cmbType->GetSelection();
	switch(sel)
	{
	case 0://查询银行余额
		QueryBankAccount();
		break;
	case 1://银行卡->保证金
		QueryFromBankToFuture();
		break;
	case 2://保证金->银行卡
		QueryFromFutureToBank();
		break;
	default:
		wxMessageBox(LOADSTRING(BankTransfer_NoType),LOADSTRING(USERLOGINDLG_INFORMATION));
		break;
	}
}
//查询转账流水
void CBankTransferDlg::OnBtnQryDetail(wxCommandEvent& event)
{
	QueryAccountDetail();
}
void CBankTransferDlg::OnDelayTimer(wxTimerEvent& event)
{
	//期货账户可取资金
	double fWithdrawQuota=0;
	if(m_pSvr) m_pSvr->GetTradingAccountWithdrawQuota(fWithdrawQuota);
	m_WithdrawQuota->SetValue(GlobalFunc::ConvertToString(fWithdrawQuota));
}


void CBankTransferDlg::OnSelected(wxExtListEvent& event)
{
	SelectLisrRow(event.GetIndex());
	OnDelayTimer(wxTimerEvent());
}
void CBankTransferDlg::OnSize( wxSizeEvent& event )
{
	wxRect rt = GetRect();
	if ( rt.width > 927 )
	{
		rt.width = 927;
	}
	else if ( rt.width < 600 )
	{
		rt.width = 600;
	}

	if ( m_GroupBox )
	{
		wxRect rcStaticBox = m_GroupBox->GetRect();
		m_GroupBox->SetSize(rt.width-30, rcStaticBox.height);
	}

	if ( m_BankList )
	{
		wxRect rcList1 = m_BankList->GetRect();
		m_BankList->SetSize(rt.width-467, rcList1.height);
	}

	if(m_DetailList)
	{
		m_DetailList->SetSize(rt.width-30, rt.height - 225);
	}
}
void CBankTransferDlg::OnReturn( wxCommandEvent& event )
{
	if( m_bSungardStyle )
	{
		event.Skip();
	}
	else
	{
		this->GetParent()->Close();
	}
}
void CBankTransferDlg::OnUserChoice(wxCommandEvent& event)
{
	UserChoice(event.GetInt());
	SubscribeBusinessData(rspOnTransferCallBackFunc);
	//获取签约银行信息
	std::map<std::string,  PlatformStru_ContractBank> ContractBankInfo;
	if(m_pSvr&&m_pSvr->GetContractBank(ContractBankInfo)>0)
	{
		m_BankList->DeleteAllItems();
		m_strPromptText->SetLabel(LOADSTRING(BT_SELECT_BANK));
		std::map<std::string,  PlatformStru_ContractBank>::iterator it=ContractBankInfo.begin();
		for(; it!=ContractBankInfo.end(); ++it)
		{
			int iIndex=0;
			int bankRow=m_BankList->InsertItem(m_BankList->GetItemCount(),it->second.BankID);  
			m_BankList->SetItem(bankRow,++iIndex,it->second.BankName);
			m_BankList->SetItem(bankRow,++iIndex,GetSafeAccout(it->second.BankAccount));
			m_BankList->SetItem(bankRow,++iIndex,it->second.CurrencyID);
			bankRow++;               

		}
		if(m_BankList->GetItemCount() == 1)
		{
			SelectLisrRow(0);
		}
	}
}
void CBankTransferDlg::DetailListInsertData(int FutureSerial,const char* BankAccount,const char* TradeCode,
						  double TradeAmount,const char* TradeTime,const char* ErrorMsg,const char* BankID)
{
	if(m_TransferDetailIDSet.insert(FutureSerial).second)
	{
		int RowId=m_DetailList->InsertItem(0,wxString::Format("%d", FutureSerial));
		m_DetailList->SetItem(RowId,1,GetSafeAccout(BankAccount)); 
		if((!strcmp(TradeCode,"102001")) || (!strcmp(TradeCode,"202001")))
			m_DetailList->SetItem(RowId,2,LOADSTRING(IMPORT_FUNDS)); 
		else if(!strcmp(TradeCode,"102002") || !strcmp(TradeCode,"202002"))
			m_DetailList->SetItem(RowId,2,LOADSTRING(OUTPORT_FUNDS)); 
		else if(!strcmp(TradeCode,"204002"))
			m_DetailList->SetItem(RowId,2,LOADSTRING(BTN_QP_BUTTONQUERY));
		m_DetailList->SetItem(RowId,3,GlobalFunc::GetAccountFormatString(TradeAmount,2));
		m_DetailList->SetItem(RowId,4,wxString(ErrorMsg));
		m_DetailList->SetItem(RowId,5,wxString(TradeTime));
		wxString BankName=GetBankInfo(BankID);
		m_DetailList->SetItem(RowId,6,wxString(BankName)); 
		m_DetailList->SetItem(RowId,7,wxString(BankID)); 
	}
}
void CBankTransferDlg::OnRcvRspTransfer(wxCommandEvent& event)
{
	ProcessRcvRspTransfer(event);
}
wxString CBankTransferDlg::GetUIValue(const wxString flag)
{
	wxString str;
	if(flag=="BankName")
		str=m_BankName->GetValue();
	else if(flag=="AccountPassword")
		str=m_AccountPassword->GetValue();
	else if(flag=="BankPassword")
		str=m_BankPassword->GetValue();
	else if(flag=="Amount")
		str=m_Amount->GetValue();
	return str;
}