#include "StdAfx.h"
#include "BankCore.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-FundAccount/FundAccountModule.h"//查询期货资金账户
#include "../inc/Module-Misc/TradeInfoDlg.h" //查询银行余额对话框
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
CBankCore::CBankCore(void):
m_pDelayTimer(NULL),
m_DetailList(NULL),
m_pSvr(NULL)
{
}

CBankCore::~CBankCore(void)
{
	SAFE_DELETE(m_pDelayTimer);
}
void CBankCore::SubscribeBusinessData(GuiCallBackFunc callback)
{
	//订阅银行资金转期货成功应答
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspFromBankToFutureByFuture, GID_TRANSFER, callback, false);
	//订阅期货发起期货资金转银行应答
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspFromFutureToBankByFuture, GID_TRANSFER, callback, false);
	//订阅期货发起查询银行余额应答
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspQueryBankAccountMoneyByFuture, GID_TRANSFER, callback, false);
	//订阅请求查询转帐流水响应
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RspQryTransferSerial, GID_TRANSFER, callback, false);

	//订阅银行资金转期货成功回报
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnFromBankToFutureByFuture, GID_TRANSFER, callback, false);
	//订阅期货发起期货资金转银行回报
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnFromFutureToBankByFuture, GID_TRANSFER, callback, false);
	//订阅期货发起查询银行余额回报
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnQueryBankAccountMoneyByFuture, GID_TRANSFER, callback, false);

}
void CBankCore::UnSubscribeBusinessData()
{
	//取消订阅银行资金转期货成功应答
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspFromBankToFutureByFuture, GID_TRANSFER);
	//取消订阅期货发起期货资金转银行应答
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspFromFutureToBankByFuture, GID_TRANSFER);
	//取消订阅期货发起查询银行余额应答
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspQueryBankAccountMoneyByFuture, GID_TRANSFER);
	//取消订阅请求查询转帐流水响应
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspQryTransferSerial, GID_TRANSFER);

	//取消订阅银行资金转期货成功回报
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnFromBankToFutureByFuture, GID_TRANSFER);
	//取消订阅期货发起期货资金转银行回报
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnFromFutureToBankByFuture, GID_TRANSFER);
	//取消订阅期货发起查询银行余额回报
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnQueryBankAccountMoneyByFuture, GID_TRANSFER);
}
bool CBankCore::GetBankInfo(const wxString& BankName,char* BankID,char* BankBranchID,char* BrokerID)
{
	std::map<std::string, PlatformStru_ContractBank> cbi;
	if(m_pSvr&&m_pSvr->GetContractBank(cbi)>0)
	{
		std::map<std::string, PlatformStru_ContractBank>::iterator it=cbi.find(BankName.c_str());
		if(it!=cbi.end())
		{
			strcpy(BankID,it->second.BankID);
			strcpy(BankBranchID,it->second.BankBranchID);
			strcpy(BrokerID,it->second.BrokerID);
			return true;
		}
	}
	return false;
}
wxString CBankCore::GetBankInfo(const char* BankID)
{
	wxString strReturn;
	std::map<std::string, PlatformStru_ContractBank> cbi;
	if(m_pSvr&&m_pSvr->GetContractBank(cbi)>0)
	{
		std::map<std::string, PlatformStru_ContractBank>::iterator it;
		for(it=cbi.begin();it!=cbi.end();++it)
		{
			if(strcmp(BankID,it->second.BankID)==0)
			{
				strReturn=it->second.BankName;
			}
		}
	}
	return strReturn;
}
wxString CBankCore::GetSafeAccout( const wxString strAcc )
{
	wxString str=strAcc;
	size_t digits=str.Length();
	for (size_t i=0;i<digits;++i)
	{
		if(i>4&&i<digits-4)
			str[i]='*';
	}
	return str;
}
void CBankCore::PopupError( CThostFtdcRspInfoField* RspInfoField,const string strmsg )
{
	if(RspInfoField&&RspInfoField->ErrorID!=0)
	{
		wxString strMsg;
		strMsg.Printf(_("%s"),
			RspInfoField->ErrorMsg
			);
		wxMessageBox(strMsg,LOADSTRING_TEXT(strmsg));
	}
}
void CBankCore::OnRtnFutureBank(CThostFtdcRspTransferField* pRawData,const std::string prompt)
{
	if(pRawData->ErrorID == 0)
	{
		DetailListInsertData(pRawData->FutureSerial,
			pRawData->BankAccount,
			pRawData->TradeCode,
			pRawData->TradeAmount,
			pRawData->TradeTime,
			pRawData->ErrorMsg,
			pRawData->BankID);
		wxCommandEvent evt(wxEVT_SUCESS_TRANSFER);	
		GETTOPWINDOW()->AddPendingEvent(evt);
		if(m_pDelayTimer)
			m_pDelayTimer->Start(1000,true);
	}
	//增加提示框
	CTradeInfoDlg*pBankAcctDlg=TRADEINFODLG(this);
	if(pBankAcctDlg!=NULL) 
	{
		pBankAcctDlg->Hide();
		wxString strMsg;
		if(pRawData->ErrorID == 0)
		{
			strMsg.Printf(LOADFORMATSTRING(BankTransfer_MSG6,"%f"),////文本必须包含%f
				pRawData->TradeAmount);
		}
		else
		{
			strMsg.Printf(pRawData->ErrorMsg);
		}
		pBankAcctDlg->ShowTradeMsg(strMsg, -1, LOADSTRING_TEXT(prompt), -1, TRUE);///"银行资金转期货"
	}
}
void CBankCore::QueryAccountDetail()
{
	m_TransferDetailIDSet.clear();
	m_DetailList->DeleteAllItems();//删除旧数据
	PlatformStru_QryTransferSerial QryTransferSerial;
	memset(&QryTransferSerial,0x00,sizeof(QryTransferSerial));

	if(m_pSvr) m_pSvr->ReqQryTransferSerial(QryTransferSerial);
}
void CBankCore::QueryBankAccount() 
{
	PlatformStru_ReqQueryAccount ReqQueryAccount;
	memset(&ReqQueryAccount,0x00,sizeof(ReqQueryAccount));
	///业务功能码
	strcpy(ReqQueryAccount.Thost.TradeCode,"204002");
	///银行代码
	wxString BankName=GetUIValue("BankName");
	if(BankName.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(BT_ERROR_SELECTBANK),LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(GetBankInfo(BankName,ReqQueryAccount.Thost.BankID,
		ReqQueryAccount.Thost.BankBranchID,
		ReqQueryAccount.Thost.BrokerID))
	{
		///期货资金密码核对标志
		ReqQueryAccount.Thost.SecuPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqQueryAccount.Thost.BankPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqQueryAccount.Thost.VerifyCertNoFlag=THOST_FTDC_YNI_No;
		//资金账号
		TThostFtdcAccountIDType data;
		memset(&data,0,sizeof(data));
		if(m_pSvr&&m_pSvr->GetTradingAccountID(data)==0)
			strcpy(ReqQueryAccount.Thost.AccountID,data);
		//资金账户密码
		wxString strPassword=GetUIValue("AccountPassword");
		if(strPassword.Trim().IsEmpty())
		{
			wxMessageBox(LOADSTRING(BankTransfer_MSG1),LOADSTRING(BankTransfer_MSGERROR),wxOK|wxICON_QUESTION);//"请输入资金账户密码!",LOADSTRING(USERLOGINDLG_ERROR)
			return;
		}
		else
		{
			strcpy(ReqQueryAccount.Thost.Password,strPassword);
		}
		//银行账户密码
		strPassword=GetUIValue("BankPassword");
		strcpy(ReqQueryAccount.Thost.BankPassWord,strPassword);
		if(m_pSvr) m_pSvr->ReqQueryBankAccountMoneyByFuture(ReqQueryAccount);
	}


}
void CBankCore::QueryFromBankToFuture(const wxString& strCurrencyID) 
{
	PlatformStru_ReqTransfer ReqTransfer;
	memset(&ReqTransfer,0x00,sizeof(ReqTransfer));
	///业务功能码
	strcpy(ReqTransfer.Thost.TradeCode,"202001");
	///银行代码
	wxString BankName=GetUIValue("BankName");
	if(BankName.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(BT_ERROR_SELECTBANK),LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(GetBankInfo(BankName,ReqTransfer.Thost.BankID,
		ReqTransfer.Thost.BankBranchID,
		ReqTransfer.Thost.BrokerID))
	{
		///期货资金密码核对标志
		ReqTransfer.Thost.SecuPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.BankPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.VerifyCertNoFlag=THOST_FTDC_YNI_No;
		//资金账号
		TThostFtdcAccountIDType data;
		memset(&data,0,sizeof(data));
		if(m_pSvr&&m_pSvr->GetTradingAccountID(data)==0)
			strcpy(ReqTransfer.Thost.AccountID,data);
		//资金账户密码
		wxString strPassword;
		strPassword=GetUIValue("AccountPassword");
		if(strPassword.Trim().IsEmpty())
		{
			wxMessageBox(LOADSTRING(BankTransfer_MSG1),LOADSTRING(USERLOGINDLG_INFORMATION));
			return;
		}
		else
		{
			strcpy(ReqTransfer.Thost.Password,strPassword);
		}
		//银行账户密码
		strPassword=GetUIValue("BankPassword");
		strcpy(ReqTransfer.Thost.BankPassWord,strPassword);

		double Amount=atof(GetUIValue("Amount").c_str());
		if(Amount>0)
			ReqTransfer.Thost.TradeAmount=Amount;
		else
		{
			wxMessageBox(LOADSTRING(BT_ERROR_AMOUNT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}
		strcpy(ReqTransfer.Thost.CurrencyID,strCurrencyID.c_str());
		if(m_pSvr) m_pSvr->ReqFromBankToFutureByFuture(ReqTransfer);
	}
}

void CBankCore::QueryFromFutureToBank(const wxString& strCurrencyID) 
{
	PlatformStru_ReqTransfer ReqTransfer;
	memset(&ReqTransfer,0x00,sizeof(ReqTransfer));
	///业务功能码
	strcpy(ReqTransfer.Thost.TradeCode,"202002");
	///银行代码
	wxString BankName=GetUIValue("BankName");
	if(BankName.Trim().IsEmpty())
	{
		wxMessageBox(LOADSTRING(BT_ERROR_SELECTBANK),LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(GetBankInfo(BankName,ReqTransfer.Thost.BankID,
		ReqTransfer.Thost.BankBranchID,
		ReqTransfer.Thost.BrokerID))
	{
		///期货资金密码核对标志
		ReqTransfer.Thost.SecuPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.BankPwdFlag=THOST_FTDC_BPWDF_NoCheck;
		ReqTransfer.Thost.VerifyCertNoFlag=THOST_FTDC_YNI_No;
		//资金账号
		TThostFtdcAccountIDType data;
		memset(&data,0,sizeof(data));
		if(m_pSvr&&m_pSvr->GetTradingAccountID(data)==0)
			strcpy(ReqTransfer.Thost.AccountID,data);
		//资金账户密码
		wxString strPassword;
		strPassword=GetUIValue("AccountPassword");
		if(strPassword.Trim().IsEmpty())
		{
			wxMessageBox(LOADSTRING(BankTransfer_MSG1),LOADSTRING(USERLOGINDLG_INFORMATION));
			return;
		}
		else
		{
			strcpy(ReqTransfer.Thost.Password,strPassword);
		}
		//银行账户密码
		strPassword=GetUIValue("BankPassword");
		strcpy(ReqTransfer.Thost.BankPassWord,strPassword);

		double Amount=atof(GetUIValue("Amount").c_str());
		if(Amount>0)
			ReqTransfer.Thost.TradeAmount=Amount;
		else
		{
			wxMessageBox(LOADSTRING(BT_ERROR_AMOUNT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}
		strcpy(ReqTransfer.Thost.CurrencyID,strCurrencyID.c_str());
		if(m_pSvr) m_pSvr->ReqFromFutureToBankByFuture(ReqTransfer);
	}
}
/*银行返回处理*/
void CBankCore::ProcessRcvRspTransfer(wxCommandEvent& event)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(event.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}
	CThostFtdcRspInfoField* RspInfoField=NULL;
	switch (pAbstractBusinessData->BID)
	{
	case  BID_RspFromBankToFutureByFuture://期货发起银行资金转期货应答
		RspInfoField= &(((DataRspFromBankToFutureByFuture*)(pAbstractBusinessData))->RspInfoField);
		PopupError(RspInfoField,"BankTransfer_MSG2");
		break;
	case   BID_RspFromFutureToBankByFuture://期货发起期货资金转银行应答
		RspInfoField= &(((DataRspFromFutureToBankByFuture*)(pAbstractBusinessData))->RspInfoField);
		PopupError(RspInfoField,"BankTransfer_MSG2");
		break;
	case   BID_RspQueryBankAccountMoneyByFuture://期货发起查询银行余额应答
		RspInfoField= &(((DataRspQueryBankAccountMoneyByFuture*)(pAbstractBusinessData))->RspInfoField);
		PopupError(RspInfoField,"BankTransfer_MSG2");
		break;
	case   BID_RspQryTransferSerial://请求查询转帐流水响应
		{
			DataRspQryTransferSerial* pTmp= (DataRspQryTransferSerial*)(pAbstractBusinessData);
			CThostFtdcTransferSerialField* pRawData= &pTmp->TransferSerialField;
			CThostFtdcRspInfoField  *RspInfoField=&pTmp->RspInfoField;
			wxString strMsg;
			strMsg.Printf(_(" %s"),RspInfoField->ErrorMsg);
			if(RspInfoField->ErrorID==0)//没有错误
			{
				DetailListInsertData(pRawData->FutureSerial,
					pRawData->BankAccount,
					pRawData->TradeCode,
					pRawData->TradeAmount,
					pRawData->TradeTime,
					pRawData->ErrorMsg,
					pRawData->BankID);
			}
			else if(RspInfoField->ErrorID == CustomErrorID)
				wxMessageBox(strMsg,LOADSTRING(BankTransfer_MSG4));//"查询转账明细"
			else
				wxMessageBox(strMsg,LOADSTRING(BankTransfer_MSG3));//"查询转帐流水操作失败"
		}           
		break;
	case  BID_RtnFromBankToFutureByFuture://期货发起银行资金转期货应答
		{
			DataRtnFromBankToFutureByFuture* pTmp= (DataRtnFromBankToFutureByFuture*)(pAbstractBusinessData);
			CThostFtdcRspTransferField* pRawData= &pTmp->RspTransferField;
			OnRtnFutureBank(pRawData,"BankTransfer_MSG7");
		}
		break;
	case   BID_RtnFromFutureToBankByFuture://期货发起期货资金转银行应答
		{
			DataRtnFromFutureToBankByFuture* pTmp= (DataRtnFromFutureToBankByFuture*)(pAbstractBusinessData);
			CThostFtdcRspTransferField* pRawData= &pTmp->RspTransferField;
			OnRtnFutureBank(pRawData,"BankTransfer_MSG5");

		}
		break;
	case BID_RtnQueryBankAccountMoneyByFuture://期货发起查询银行余额应答
		{
			DataRtnQueryBankAccountMoneyByFuture* pTmp= (DataRtnQueryBankAccountMoneyByFuture*)(pAbstractBusinessData);
			CThostFtdcNotifyQueryAccountField* pRawData= &pTmp->NotifyQueryAccountField;
			if(pRawData->ErrorID == 0)
			{
				DetailListInsertData(pRawData->FutureSerial,
					pRawData->BankAccount,
					pRawData->TradeCode,
					pRawData->BankFetchAmount,
					pRawData->TradeTime,
					pRawData->ErrorMsg,
					pRawData->BankID);
			}
			//提示框
			CTradeInfoDlg*pBankAcctDlg=TRADEINFODLG(this);
			if(pBankAcctDlg!=NULL) 
			{
				pBankAcctDlg->Hide();
				wxString info;
				if(pRawData->ErrorID == 0)
				{
					info.reserve(512);
					info.Printf(LOADFORMATSTRING(BankTransfer_BankRsp2,"%s%s"),//文本必须包含%s  %s  %s %s
						//"银行账号： %s\n"
						//"银行可用金额： %s\n"
						pRawData->BankAccount,
						wxString::Format("%0.2f",pRawData->BankFetchAmount).c_str());
					pBankAcctDlg->ShowTradeMsg(info, -1, LOADSTRING(BankTransfer_BankRsp1Title), -1, TRUE);//wxString("银行账户详情")
				}
				else
				{
					info.Printf(pRawData->ErrorMsg);
					pBankAcctDlg->ShowTradeMsg(info, -1, LOADSTRING(BankTransfer_MSG10), -1, TRUE);///"银行资金转期货"
				}
			}
		}
		break;
	default:
		break;
	}
	delete (void*)pAbstractBusinessData;
	pAbstractBusinessData=NULL;
	event.SetClientData(NULL);
}
void CBankCore::UserChoice(int sel)
{
	wxArrayString items=MA_CORE()->GetAccountArray();
	if(sel>=0&&sel<(int)items.size())
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(items[sel].c_str());
	}	
}