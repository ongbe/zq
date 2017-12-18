#pragma once

#include <map>
#include "RiskManageStruct.h"
#include "afxwin.h"
#include "AssetOrgComboTree.h"

// CTrustAccountOperatorDlg dialog

class CTraderOperatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CTraderOperatorDlg)

public:
	CTraderOperatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTraderOperatorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TRADER_OPERATOR };

	virtual BOOL OnInitDialog();

	void SetOperatorType( OperatorType eType);
	void SetTraderInfo(const TraderInfo& traderInfo );
	const TraderInfo& GetTraderInfo();
	void SetAssetMgmtOrgTree(std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree);
	void SetFinancialProduct(std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void SetModifyData();
	void FillFinancialProduct( int nAssetMgmtOrgID );

private:
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;
	std::map<int, std::map<int, FinancialProduct>> m_mapFinancialProductTree;
	OperatorType m_eType;
	TraderInfo m_curTraderInfo;

public:
	CString m_csAccount;
	CAssetOrgComboTree m_comboTree;
	CComboBox m_cbxFinancialProduct;
	CString m_csTel;
	CString m_csMobile;
	CString m_csOtherContactInfo;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboAccountOrg();
};
