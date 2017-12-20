#pragma once

#include <vector>
#include "../Module-Misc/DataListCtrl.hpp"
#include "../MAServer/include/MAAccount/MultiAccountCmd.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
#include "../inc/MultiAccountCore/CTPLogin.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
using namespace std;

class zqWriteLog;
class MultiAccountPanel :public wxPanel,public LoginCallback
{
public:
	enum
	{
		ID_LISTCTRL=100,
		ID_CHOICE,
		ID_USERGROUP,
		ID_ALGO_SETUP,
		ID_ADD,
		ID_EDIT,
		ID_DEL,
		ID_QUOT_SETUP,
		ID_LOGOUT_SEL,
		ID_LOGIN_CHECKED,
		ID_LOGIN_ALL,
		ID_REQUERY,
		ID_SYNTYPE,
	};
	enum
	{
		MA_COL_VILID,
		MA_COL_ID,
		MA_COL_NAME,
		MA_COL_ACCOUNT,
		MA_COL_STATUS,
		MA_COL_USEQUOT,
		MA_COL_AVALIABLE,		
		MA_COL_PREBALANCE,		
		MA_COL_STATICPROFIT,	
		MA_COL_DYNAMICPROFIT,	
		MA_COL_POSITIONPROFIT,	
		MA_COL_CLOSEPROFIT,		
		MA_COL_RISKDEGREE,	
	};
	MultiAccountPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);
	~MultiAccountPanel();
	void OnSize(wxSizeEvent& event);
	void OnSynType(wxCommandEvent& evt);
	void OnAlgoSetup(wxCommandEvent& evt);
	void OnAdd(wxCommandEvent& evt);		
	void OnEdit(wxCommandEvent& evt);	
	void OnDel(wxCommandEvent& evt);	
	void OnQuotSetup(wxCommandEvent& evt);
	void OnLogoutSel(wxCommandEvent& evt);
	void OnLoginChecked(wxCommandEvent& evt);
	void OnLoginAll(wxCommandEvent& evt);
	void OnReQuery(wxCommandEvent& evt);
	void InsertOneRow(int index,const CTPTradeAccount* acc);
	void ModifyOneRow(int index, const CTPTradeAccount* acc );
	void SetLoginStatus(int row,wxString strText);
	void MAUpdateOneAccount(int sel,string strCurAccount,bool bRefresh);
	void OnMaFundUpdate( wxCommandEvent& event);
	void OnUICallBack( wxCommandEvent& event);
	void OnStrategyChoice(wxCommandEvent& event);
	void OnUserChoice(wxCommandEvent& event);
	void OnMaListUpdate(wxCommandEvent& event);
	void OnMaUserChange(int sel);
	void OnListCheckbox(wxExtListEvent& event);
	void OnCfgChanged(wxCommandEvent& event);
	void OnPanelCharHook(wxCommandEvent& evt);
	void OnContext(wxContextMenuEvent& evt);
	void OnListColClick(wxExtListEvent& event);
	void OnSubscrible(wxCommandEvent& evt);
	int SetAccountComboBox(wxChoice* combMAUser);
	void DeleteItem(vector<int> vecRows);
	void SetQuotUse(int row);
	static void SetUICallback(bool bAcc);
	static int AllCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static MultiAccountPanel* m_MultiAccountPanel;
	DECLARE_EVENT_TABLE()
private:
	int GetRow(IPlatformSingleSvr* pSvr);
	int LoginSpecialAccount( int sel );
	void SetProgressPosition(IPlatformSingleSvr* pSvr,int TotalStep,int CurStep=-1,int pos=-1);
	void DoLoginFail(IPlatformSingleSvr* pSvr,wxString strSrc=wxEmptyString);
	void SetPromptText(IPlatformSingleSvr* pSvr,wxString strText=wxEmptyString);
	void OnQryFinish(IPlatformSingleSvr* pSvr);
	void CopySvr(IPlatformSingleSvr* pSrcSvr, IPlatformSingleSvr* pDestSvr );
	void UpdateUIData(string acc,int row);
	void InitListCtrl();
	void SaveColWidth();
	void ResetValidColText(int nRow, BOOL bIsCheck);

private:
	MULTIACCOUNT*  m_pMACfg;
	wxExtListCtrl* m_pList;
	bool m_bFirstAccountLogin;
	map<string,PlatformStru_TradingAccountInfo> m_mapAccountInfo;
	map<int,int> m_mapCfgItem;

	std::map<int, long> m_FieldID2ColumnID;
	std::map<int, long> m_ColumnID2FieldID;

};
