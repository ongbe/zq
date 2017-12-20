#pragma once
#include "../MAServer/include/MAAccount/MultiAccountCmd.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
class CMAUserEditDlg :
	public wxDialog
{
public:
	enum
	{
		ID_NAME=100,
		ID_USERID,
		ID_PASSWORD,
		ID_DYNAMICPASSWORD,
		ID_CMB_BROKER,
		ID_CMB_SVRGROUP,
	};
	CMAUserEditDlg(const CTPTradeAccount* acc,wxWindow *parent=NULL);
	~CMAUserEditDlg(void);
	void SelectSvrGroupOfBrokerID( int BrokerID ,int nSvrNo );
	void OnBrokerChoice(wxCommandEvent& event);
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
public:
    void GetTradeAccount(CTPTradeAccount& acc);
private:
	bool bIsEditDlg;
	CTPTradeAccount m_TradeAccount;
};
