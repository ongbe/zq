#pragma once

class CBankCore
{
public:
	CBankCore(void);
	virtual ~CBankCore(void);
protected:
	bool GetBankInfo(const wxString& BankName,char* BankID,char* BankBranchID,char* BrokerID);
	wxString GetBankInfo(const char* BankID);
	wxString GetSafeAccout( const wxString strAcc );
	void PopupError( CThostFtdcRspInfoField* RspInfoField,const string strmsg );
	void OnRtnFutureBank(CThostFtdcRspTransferField* pRawData,const std::string prompt);
	void QueryAccountDetail();
	void QueryBankAccount();
	void QueryFromBankToFuture();
	void QueryFromFutureToBank();
	void ProcessRcvRspTransfer(wxCommandEvent& event);
    void SubscribeBusinessData(GuiCallBackFunc callback);
    void UnSubscribeBusinessData();
public:
	virtual void CreateGUIControls()=0;
	virtual wxString GetUIValue(const wxString flag)=0;	
	virtual void DetailListInsertData(int FutureSerial,const char* BankAccount,const char* TradeCode,
		double TradeAmount,const char* CurrencyID,const char* TradeTime,const char* ErrorMsg,const char* BankID)=0;
protected:
	wxTimer*  m_pDelayTimer;
	wxExtListCtrl *m_DetailList;
	std::set<int> m_TransferDetailIDSet;
};
