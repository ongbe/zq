#pragma once

#include <map>
#include "MyEdit.h"
#include "FinancialEdit.h"
#include "RiskManageStruct.h"
#include "afxwin.h"
#include "AssetOrgComboTree.h"

// CTrustTradeOperatorDlg dialog

class CTrustTradeOperatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrustTradeOperatorDlg)

public:
	CTrustTradeOperatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrustTradeOperatorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TRUST_ACCOUNT_OPERATOR };

	virtual BOOL OnInitDialog();

	void SetOperatorType( OperatorType eType);
	void SetTrustTradeAccount(const TrustTradeAccount& account );
	const TrustTradeAccount& GetTrustTradeAccount();
	void SetAssetMgmtOrgTree( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree );
	void SetFinancialProduct(std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree);
	void SetTrader(std::map<int, TraderInfoEx>& mapTraderInfoEx);

	void SetIndicator( const std::vector<RiskIndicator>& vIndicator);
	std::map<int, RiskIndicatorModule> GetRiskIndModule();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void SetModifyData();

	void FillFinancialProduct( int nAssetMgmtOrgID );
	void FillTrader( int nAssetMgmtOrgID, int nFinancialProductID );

private:
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;
	std::map<int, std::map<int, FinancialProduct>> m_mapFinancialProductTree;
	std::map<int, RiskIndicatorModule> m_mapIndModule;
	std::map<int, TraderInfoEx> m_mapTraderInfoEx;
	OperatorType m_eType;
	TrustTradeAccount m_curAccount;

	std::vector<RiskIndicator> m_vIndicator;

public:
	CMyEdit m_edtAccount;
	CString m_csAccount;
	CComboBox m_cbxFinancialProduct;
	CComboBox m_cbxTrader;
	CComboBox m_cbxIndModule;
	CFinancialEdit m_edtProfit;
	CString m_csProfit;
	CEdit m_edtName;
	CString m_csName;
	CEdit m_edtContactInfo;
	CString m_csContactInfo;
	CComboBox m_cbxIDCardType;
	CEdit m_edtIDCardNo;
	CString m_csIDCardNo;
	CAssetOrgComboTree m_comboTree;

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboAccountOrg();
	afx_msg void OnCbnSelchangeComboAccountFin();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//afx_msg void OnEnKillfocusEditProfit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnRiskModuleSet();
	CMyEdit m_edtPwd;
	CString m_csPwd;
};
