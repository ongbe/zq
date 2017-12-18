#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include <vector>
#include <map>
#include "RiskManageStruct.h"
#include "afxcmn.h"
#include "AssetOrgComboTree.h"

// CBatchAddAccountDlg dialog

class CBatchAddAccountDlg : public CDialog
{
	DECLARE_DYNAMIC(CBatchAddAccountDlg)

public:
	CBatchAddAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBatchAddAccountDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BATCH_ADD_ACCOUNT };

	void SetAssetMgmtOrgTree(std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree);
	void SetFinancialProduct(std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree);
	void SetIndicatorModule(std::map<int, RiskIndicatorModule>& mapIndModule);
	void SetTrader(std::map<int, TraderInfoEx>& mapTraderInfoEx);

	int GetImportedDataCount();

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedOk();

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	bool LoadImportFile(const CString& csFilePath);
	bool ParseLine( const CString& csLine, TrustTradeAccount& accountInfo);
	CString CombineLine( const TrustTradeAccount& accountInfo );
	bool IsValidString( const CString& cs);
	bool WriteErrorFile();
	CString ConventErrorMsg( const CString& csMsg );

	void FillFinancialProduct( int nAssetMgmtOrgID );
	void FillTrader( int nAssetMgmtOrgID, int nFinancialProductID );

	bool RemovePrefix( CString& cs);

public:
	CEdit m_edtPath;
	CAssetOrgComboTree m_comboTree;
	CComboBox m_cbxFinPro;
	CComboBox m_cbxTrader;
	CButton m_btnOK;
	CProgressCtrl m_progressCtrl;
	CComboBox m_cbxModule;

private:
	std::map<int, TrustTradeAccount> m_mapAccountInfo;
	std::map<int, TrustTradeAccount> m_tempAccountInfoMap;
	std::map<int, CString> m_mapErrorInfo;
	std::map<int, CString> m_mapFormatErrorLine;
	std::vector<int> m_vSuccessedID;

	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;
	std::map<int, std::map<int, FinancialProduct>> m_mapFinancialProductTree;
	std::map<int, RiskIndicatorModule> m_mapIndModule;
	std::map<int, TraderInfoEx> m_mapTraderInfoEx;
	
	CString m_csTitle;
public:
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboBatchOrg();
	afx_msg void OnCbnSelchangeComboBatchFinPro();
};
