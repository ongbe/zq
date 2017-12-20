/***************************************************************************//**
*   @file         BankFutureTransferDlg.cpp
*   @brief        银期转账对话框的实现
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
 ********************************************************************************/
#include "stdafx.h"
#include "BankFutureTransferDlg.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-FundAccount/FundAccountModule.h"//查询期货资金账户
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../Module-Misc/LogDefine.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
//#define MaxAmount 50000.00
#define  TIMER_CYCLE			5000 
static CBankFutureTransferDlg* s_pCBankFutureTransferDlg=NULL;
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static int rspOnTransferCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(!s_pCBankFutureTransferDlg)
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

    s_pCBankFutureTransferDlg->AddPendingEvent(evt);

    return 0;

}


BEGIN_EVENT_TABLE(CBankFutureTransferDlg,wxDialog)
EVT_CLOSE(CBankFutureTransferDlg::OnClose)
EVT_BUTTON(ID_WXBUTTON1,CBankFutureTransferDlg::OnQueryBankAccount)
EVT_BUTTON(ID_WXBUTTON2,CBankFutureTransferDlg::WxButton2Click)
EVT_BUTTON(ID_WXBUTTON3,CBankFutureTransferDlg::OnQueryFromFutureToBank)
EVT_BUTTON(ID_WXBUTTON4,CBankFutureTransferDlg::OnQueryFromBankToFuture)
EVT_BUTTON(ID_WXBUTTON5,CBankFutureTransferDlg::OnBtnQryDetail)
EVT_BUTTON(wxID_CANCEL,CBankFutureTransferDlg::OnCancel)
EVT_BUTTON(ID_BTN_HIDE,CBankFutureTransferDlg::OnHide)
EVT_COMMAND(wxID_ANY, wxEVT_TRANSFER_RSP,  CBankFutureTransferDlg::OnRcvRspTransfer)
EVT_CHOICE(ID_CHOICEUSER,OnUserChoice)
END_EVENT_TABLE()

CBankFutureTransferDlg::CBankFutureTransferDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(BankTransfer_DLGTitle),wxDefaultPosition, wxSize(800,310),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX)
{
	AUTO_SETSIZE();
	CreateGUIControls();
	s_pCBankFutureTransferDlg=this;	
}
void CBankFutureTransferDlg::OnUserChoice(wxCommandEvent& event)
{
	UserChoice(event.GetInt());
	SubscribeBusinessData(rspOnTransferCallBackFunc);
	//获取签约银行信息
	m_cmbBankName->Clear();
	std::map<std::string, PlatformStru_ContractBank> ContractBankInfo;
	if(m_pSvr)
		m_pSvr->GetContractBank(ContractBankInfo);
	std::map<std::string,  PlatformStru_ContractBank>::iterator it=ContractBankInfo.begin();
	for(; it!=ContractBankInfo.end(); ++it)
		m_cmbBankName->Append(it->first);
	if(m_cmbBankName->GetCount()==1)
		m_cmbBankName->SetSelection(0);
}
CBankFutureTransferDlg::~CBankFutureTransferDlg()
{

}

void CBankFutureTransferDlg::CreateGUIControls()
{
    //左边部分
	m_strPromptText = new wxStaticText(this, ID_WXSTATICTEXT7,LOADSTRING(BT_NO_QUERY_BANK), wxPoint(125, 10));
	wxButton* pBtn=NULL;
	wxFont fnt(9, wxSWISS, wxNORMAL, wxBOLD, false);
	//左上
	{
		m_strPromptText->Hide();
		new wxStaticText(this, wxID_STATIC,"用户选择", wxPoint(20, 5), wxSize(100, 20));//wxT("用户选择")
		new wxChoice(this, ID_CHOICEUSER, wxPoint(125, 5), wxSize(145, 20), MA_CORE()->GetAccountArray());
		new wxStaticText(this, ID_WXSTATICTEXT1, LOADSTRING(BankTransfer_SelectBank), wxPoint(20, 30), wxSize(100, 20));//wxT("选择银行")
		m_cmbBankName = new wxComboBox(this, ID_WXCOMBOBOX1, wxEmptyString, wxPoint(125, 30), wxSize(145, 20), NULL,wxCB_READONLY);

		new wxStaticText(this, ID_WXSTATICTEXT2, LOADSTRING(BankTransfer_FundAccountPasswd), wxPoint(20, 55), wxSize(100, 20));//wxT("资金账户密码")
		m_AccountPassword = new wxTextCtrl(this, ID_WXEDIT1, wxEmptyString, wxPoint(125, 55), wxSize(145, 20),wxTE_PASSWORD);
		new wxStaticText(this, ID_WXSTATICTEXT6, LOADSTRING(BankTransfer_BankAccountPasswd), wxPoint(20, 80), wxSize(100, 20));//wxT("银行账户密码")
		m_BankPassword = new wxTextCtrl(this, ID_WXEDIT3, wxEmptyString, wxPoint(125, 80), wxSize(145, 20),wxTE_PASSWORD);
		new wxStaticText(this, ID_WXSTATICTEXT3, LOADSTRING(BankTransfer_TransferFunds), wxPoint(20, 105), wxSize(100, 20));//wxT("转账金额")
		m_Amount = new wxTextCtrl(this, ID_WXEDIT2, wxEmptyString, wxPoint(125, 105), wxSize(145, 20));
		new wxStaticText(this, ID_WXSTATICTEXT4, LOADSTRING(BankTransfer_CurrencySelect), wxPoint(20, 130), wxSize(100, 20));//wxT("币种选择")
		m_cmbCurrency = new wxComboBox(this, ID_WXCOMBOBOX2, wxT("RMB"), wxPoint(125, 130), wxSize(145, 20), NULL,wxCB_READONLY);
		m_cmbCurrency->Append("RMB");
		m_cmbCurrency->Append("USD");
		m_cmbCurrency->Append("HKD");
		m_cmbCurrency->SetSelection(0);
	}
    //左下
	{
		pBtn= new wxButton(this, ID_WXBUTTON1, LOADSTRING(BankTransfer_QueryBankAccount), wxPoint(10, 160), wxSize(140, 25));//wxT("查询银行账户")
		pBtn->SetFont(fnt);
		pBtn = new wxButton(this, ID_WXBUTTON2, LOADSTRING(BankTransfer_QueryFundAccount), wxPoint(165, 160), wxSize(140, 25));//wxT("查询期货账户")
		pBtn->SetFont(fnt);
		pBtn->Enable(!DEFAULT_SVR()->IsMultipleAccounts());
		pBtn = new wxButton(this, ID_WXBUTTON3, LOADSTRING(BankTransfer_FundToBank), wxPoint(10, 195), wxSize(140, 25));//wxT("期货账户资金转银行")
		pBtn->SetFont(fnt);
		pBtn = new wxButton(this, ID_WXBUTTON4, LOADSTRING(BankTransfer_BankToFund), wxPoint(165, 195), wxSize(140, 25));//wxT("银行资金转期货账户")
		pBtn->SetFont(fnt);
		pBtn = new wxButton(this, ID_WXBUTTON5, LOADSTRING(BankTransfer_QueryTransferDetail), wxPoint(10, 230), wxSize(140, 25));//wxT("查询转账明细")
		pBtn->SetFont(fnt);
		pBtn = new wxButton(this, wxID_CANCEL, LOADSTRING(BankTransfer_Cancel), wxPoint(165, 230), wxSize(140, 25));//wxT("取消")
		pBtn->SetFont(fnt);
	}
    //左右分隔线
    new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(320, 0), wxSize(-1, 260), wxLI_VERTICAL);
	//右边部分
	{
		new wxStaticText(this, ID_WXSTATICTEXT5, LOADSTRING(BankTransfer_BankDetail), wxPoint(330, 5), wxSize(100, 20));//wxT("银行交易明细:")
		pBtn = new wxButton(this, ID_BTN_HIDE, LOADSTRING(BankTransfer_Hide), wxPoint(740, 5), wxSize(50, 20));//wxT("隐藏<<")
		pBtn->SetFont(fnt);
		m_DetailList=new wxExtListCtrl(this, ID_LISTCTRL,wxPoint(330, 28) , wxSize(460, 212),wxLCEXT_REPORT|wxLCEXT_VRULES);
		int iIndex=0;
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BankTransfer_FuntureSeqNum),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BankTransfer_BankAccount),wxLISTEXT_FORMAT_LEFT,150);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BankTransfer_TradeType),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BankTransfer_AmountIncurred),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BankTransfer_TradingHours),wxLISTEXT_FORMAT_LEFT,100);
		m_DetailList->InsertColumn(iIndex++,LOADSTRING(BankTransfer_Information),wxLISTEXT_FORMAT_LEFT,100);
	}
    SetIcon(wxNullIcon);
    //改变大小，将右边部门隐藏
    SetSize(320,310);
    Center();
}

void CBankFutureTransferDlg::OnClose(wxCloseEvent& /*event*/)
{
   Hide();
}

//查询银行账户
void CBankFutureTransferDlg::OnQueryBankAccount(wxCommandEvent& event)
{
	QueryBankAccount();
}

//查询期货账户
void CBankFutureTransferDlg::WxButton2Click(wxCommandEvent& event)
{
       wxCommandEvent evt(wxEVT_QRY_ACCOUNT_DLG);	
       GETTOPWINDOW()->AddPendingEvent(evt);
}

//期货账户资金转银行
void CBankFutureTransferDlg::OnQueryFromFutureToBank(wxCommandEvent& event)
{
	QueryFromFutureToBank(m_cmbCurrency->GetValue());
}

//银行资金转期货账户
void CBankFutureTransferDlg::OnQueryFromBankToFuture(wxCommandEvent& event)
{
	QueryFromBankToFuture(m_cmbCurrency->GetValue());
}

//查询转账明细
void CBankFutureTransferDlg::OnBtnQryDetail(wxCommandEvent& event)
{
    //改变大小，将右边部门显示
    SetSize(800,310);
    QueryAccountDetail();
}

//取消按钮
void CBankFutureTransferDlg::OnCancel(wxCommandEvent& event)
{
    Hide();
}

//隐藏按钮
void CBankFutureTransferDlg::OnHide(wxCommandEvent& event)
{
    //改变大小，将右边部门隐藏
    SetSize(320,310);
}
void CBankFutureTransferDlg::DetailListInsertData(int FutureSerial,const char* BankAccount,const char* TradeCode,
						  double TradeAmount,const char* TradeTime,const char* ErrorMsg,const char* BankID)
{
	if(m_TransferDetailIDSet.insert(FutureSerial).second)
	{
		int RowId=m_DetailList->InsertItem(0,wxString::Format("%d", FutureSerial));
		m_DetailList->SetItem(RowId,1,GetSafeAccout(BankAccount)); 
		if((!strcmp(TradeCode,"102001")) || (!strcmp(TradeCode,"202001")))
			m_DetailList->SetItem(RowId,2,LOADSTRING(BankTransfer_MSG7)); 
		else if(!strcmp(TradeCode,"102002") || !strcmp(TradeCode,"202002"))
			m_DetailList->SetItem(RowId,2,LOADSTRING(BankTransfer_MSG5)); 
		else if(!strcmp(TradeCode,"204002"))
			m_DetailList->SetItem(RowId,2,LOADSTRING(BTN_QP_BUTTONQUERY));
		m_DetailList->SetItem(RowId,3,GlobalFunc::GetAccountFormatString(TradeAmount,2));
		m_DetailList->SetItem(RowId,4,wxString(TradeTime));
		m_DetailList->SetItem(RowId,5,wxString(ErrorMsg));
	}
}
void CBankFutureTransferDlg::OnRcvRspTransfer(wxCommandEvent& event)
{
	ProcessRcvRspTransfer(event);
}
wxString CBankFutureTransferDlg::GetUIValue(const wxString flag)
{
	wxString str;
	if(flag=="BankName")
		str=m_cmbBankName->GetValue();
	else if(flag=="AccountPassword")
		str=m_AccountPassword->GetValue();
	else if(flag=="BankPassword")
		str=m_BankPassword->GetValue();
	else if(flag=="Amount")
		str=m_Amount->GetValue();
	return str;
}