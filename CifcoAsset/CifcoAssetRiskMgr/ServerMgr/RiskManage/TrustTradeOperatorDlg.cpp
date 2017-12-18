// TrustTradeOperatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "TrustTradeOperatorDlg.h"
#include "TCPDataService.h"
#include "WarningSetDlg.h"
#include "CryptoAPI.h"
#include "RiskManageStruct.h"


// CTrustTradeOperatorDlg dialog

IMPLEMENT_DYNAMIC(CTrustTradeOperatorDlg, CDialog)

CTrustTradeOperatorDlg::CTrustTradeOperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrustTradeOperatorDlg::IDD, pParent)
	, m_csAccount(_T(""))
	, m_csProfit(_T(""))
	, m_csName(_T(""))
	, m_csContactInfo(_T(""))
	, m_csIDCardNo(_T(""))
	, m_csPwd(_T(""))
{
	memset(&m_curAccount, 0, sizeof(m_curAccount));
}

CTrustTradeOperatorDlg::~CTrustTradeOperatorDlg()
{
}

void CTrustTradeOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_edtAccount);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_csAccount);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_FIN, m_cbxFinancialProduct);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_TRADER, m_cbxTrader);
	DDX_Control(pDX, IDC_COMBO_IND_MODULE, m_cbxIndModule);
	DDX_Control(pDX, IDC_EDIT_PROFIT, m_edtProfit);
	DDX_Text(pDX, IDC_EDIT_PROFIT, m_csProfit);
	DDX_Control(pDX, IDC_EDIT_USER_NAME, m_edtName);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_csName);
	DDX_Control(pDX, IDC_EDIT_CONTACTINFO, m_edtContactInfo);
	DDX_Text(pDX, IDC_EDIT_CONTACTINFO, m_csContactInfo);
	DDX_Control(pDX, IDC_COMBO_IDCARDTYPE, m_cbxIDCardType);
	DDX_Control(pDX, IDC_EDIT_IDCARDNO, m_edtIDCardNo);
	DDX_Text(pDX, IDC_EDIT_IDCARDNO, m_csIDCardNo);
	DDX_Control(pDX, IDC_EDIT_PWD, m_edtPwd);
	DDX_Text(pDX, IDC_EDIT_PWD, m_csPwd);
}


BEGIN_MESSAGE_MAP(CTrustTradeOperatorDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTrustTradeOperatorDlg::OnBnClickedOk)
	ON_CBN_SELENDOK(IDC_COMBO_ACCOUNT_ORG, &CTrustTradeOperatorDlg::OnCbnSelchangeComboAccountOrg)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCOUNT_FIN, &CTrustTradeOperatorDlg::OnCbnSelchangeComboAccountFin)
	ON_MESSAGE(RECV_DATA_MSG, &CTrustTradeOperatorDlg::OnRecvMessage)
	ON_WM_DESTROY()
	//ON_EN_KILLFOCUS(IDC_EDIT_PROFIT, &CTrustTradeOperatorDlg::OnEnKillfocusEditProfit)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_RISK_MODULE_SET, &CTrustTradeOperatorDlg::OnBnClickedBtnRiskModuleSet)
END_MESSAGE_MAP()

BOOL CTrustTradeOperatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//取消组合框风格
	SetWindowTheme( m_cbxFinancialProduct.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxTrader.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxIndModule.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxIDCardType.m_hWnd, L"", L"");

	m_edtAccount.SetLimitText(MAX_ACCOUNT_LENGTH);
	m_edtName.SetLimitText(MAX_TRUST_ACCOUNT_NAME_LENGTH);
	m_edtProfit.SetLimitText(20);
	m_edtIDCardNo.SetLimitText(MAX_IDCARD_NO_LENGTH);
	m_edtContactInfo.SetLimitText(MAX_CONTACTINFO_LENGTH);
	m_edtPwd.SetLimitText(MAX_PWD_LENGTH);

	//填充所属资管
	m_comboTree.SubclassDlgItem (IDC_COMBO_ACCOUNT_ORG, this);
	m_comboTree.SetAssetMgmtOrg(m_mapAssetMgmtOrgTree);

	//证件类型
	m_cbxIDCardType.ResetContent();
	m_cbxIDCardType.AddString(_T("身份证"));
	m_cbxIDCardType.SetItemData(0, IDENTIFY_CARD);
	m_cbxIDCardType.AddString(_T("军官证"));
	m_cbxIDCardType.SetItemData(1, MILITARY_ID);
	m_cbxIDCardType.AddString(_T("其他"));
	m_cbxIDCardType.SetItemData(2, OTHER_CARD);

	CString csTitle;
	if ( m_eType == OPERATOR_MODIFY )
	{
		SetModifyData();
		csTitle = _T("修改委托交易账户");
	}
	else
	{
		csTitle = _T("新增委托交易账户");

		m_cbxIDCardType.SetCurSel(0);
	}

	SetWindowText(csTitle);

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_QryValidRiskIndModule_Req, NULL, 0))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}

	return TRUE;
}

void CTrustTradeOperatorDlg::SetOperatorType( OperatorType eType )
{
	m_eType = eType;
}

void CTrustTradeOperatorDlg::SetTrustTradeAccount( const TrustTradeAccount& account )
{
	m_curAccount = account;
}

const TrustTradeAccount& CTrustTradeOperatorDlg::GetTrustTradeAccount()
{
	return m_curAccount;
}

void CTrustTradeOperatorDlg::SetAssetMgmtOrgTree( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree )
{
	m_mapAssetMgmtOrgTree = mapAssetMgmtOrgTree;
}

void CTrustTradeOperatorDlg::SetFinancialProduct( std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree )
{
	m_mapFinancialProductTree = mapFinancialProductTree;
}

void CTrustTradeOperatorDlg::SetModifyData()
{
	m_csAccount = m_curAccount.szAccount;
	char szPwd[33];
	memset(szPwd, 0, sizeof(szPwd));
	strcpy(szPwd, m_curAccount.szPwd);
	if ( strlen(szPwd) == 0 )
	{
		m_csPwd.Empty();
	}
	else
	{
		CCryptoAPI::Decode3DES_ToString(szPwd);
		m_csPwd = szPwd;
	}

	//所属资管
	m_comboTree.SelectItem(m_curAccount.nAssetMgmtOrgID);

	//填充理财产品列表
	FillFinancialProduct(m_curAccount.nAssetMgmtOrgID);
	for ( int i = 0; i < m_cbxFinancialProduct.GetCount(); i++ )
	{
		if ( m_curAccount.nFinancialProductID == m_cbxFinancialProduct.GetItemData(i))
		{
			m_cbxFinancialProduct.SetCurSel(i);
		}
	}

	//填充交易员列表
	FillTrader(m_curAccount.nAssetMgmtOrgID, m_curAccount.nFinancialProductID);
	for ( int i = 0; i < m_cbxTrader.GetCount(); i++ )
	{
		if ( m_curAccount.nTraderID == m_cbxTrader.GetItemData(i))
		{
			m_cbxTrader.SetCurSel(i);
		}
	}

	//初始权益
	m_csProfit = GetAccountFormatString(m_curAccount.dProfit, 2);
	m_csName = m_curAccount.szName;
	m_csContactInfo = m_curAccount.szContactInfo;
	
	//证件类型
	for ( int i = 0; i < m_cbxIDCardType.GetCount(); i++ )
	{
		if ( m_curAccount.nIDCardType == m_cbxIDCardType.GetItemData(i))
		{
			m_cbxIDCardType.SetCurSel(i);
		}
	}

	m_csIDCardNo = m_curAccount.szIDCardNo;

	m_edtAccount.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CTrustTradeOperatorDlg::FillFinancialProduct( int nAssetMgmtOrgID )
{
	m_cbxFinancialProduct.ResetContent();
	std::map<int, std::map<int, FinancialProduct>>::iterator it =
		m_mapFinancialProductTree.find(nAssetMgmtOrgID);
	if ( it == m_mapFinancialProductTree.end())
	{
		return;
	}

	std::map<int, FinancialProduct>::iterator it_fin = it->second.begin();
	int i = 0;
	for ( ; it_fin != it->second.end(); ++it_fin )
	{
		m_cbxFinancialProduct.AddString(it_fin->second.szName);
		m_cbxFinancialProduct.SetItemData(i, it_fin->second.nFinancialProductID);
		i++;
	}
}


// CTrustTradeOperatorDlg message handlers

void CTrustTradeOperatorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if ( m_csAccount.IsEmpty() )
	{
		MessageBox(_T("账号不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtAccount.SetFocus();
		return;
	}

	char szPwd[33];
	memset(szPwd, 0, sizeof(szPwd));
	if ( !m_csPwd.IsEmpty())
	{
		strcpy(szPwd, m_csPwd.GetBuffer(0));
		m_csPwd.ReleaseBuffer();
		CCryptoAPI::Encode3DES_ToString(szPwd);
	}

	strcpy(m_curAccount.szPwd, szPwd);

	if ( m_comboTree.GetCurSelItem() == NULL )
	{
		MessageBox(lpszEmptyOrg, lpszPrompt, MB_ICONERROR);
		m_comboTree.SetFocus();
		return;
	}

	if ( m_cbxIndModule.GetCurSel() == -1 )
	{
		MessageBox(_T("风险模型不能为空！"), lpszPrompt, MB_ICONERROR);
		m_cbxIndModule.SetFocus();
		return;
	}

	if ( m_csProfit.IsEmpty() )
	{
		MessageBox(_T("初始权益不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtProfit.SetFocus();
		return;
	}

	strcpy(m_curAccount.szAccount,m_csAccount.GetBuffer(0));
	m_csAccount.ReleaseBuffer();

	m_curAccount.nAssetMgmtOrgID = m_comboTree.GetSelectItemData();
	int nFinancialProduct = m_cbxFinancialProduct.GetCurSel();
	if ( nFinancialProduct != -1 )
	{
		m_curAccount.nFinancialProductID = m_cbxFinancialProduct.GetItemData(nFinancialProduct);
	}
	else
	{
		m_curAccount.nFinancialProductID = 0;
	}

	int nTrader = m_cbxTrader.GetCurSel();
	if ( nTrader != -1 )
	{
		m_curAccount.nTraderID = m_cbxTrader.GetItemData(nTrader);
	}
	else
	{
		m_curAccount.nTraderID = 0;
	}

	int nIndModuleID = m_cbxIndModule.GetCurSel();
	m_curAccount.nRiskIndModuleID = m_cbxIndModule.GetItemData(nIndModuleID);
	CString csProfit = m_csProfit;
	csProfit.Remove(',');
	m_curAccount.dProfit = atof(csProfit.GetBuffer(0));
	csProfit.ReleaseBuffer();

	strcpy(m_curAccount.szName, m_csName.GetBuffer(0));
	m_csName.ReleaseBuffer();
	strcpy(m_curAccount.szContactInfo, m_csContactInfo.GetBuffer(0));
	m_csContactInfo.ReleaseBuffer();

	m_curAccount.nIDCardType = static_cast<IDCardType>(m_cbxIDCardType.GetCurSel());
	strcpy(m_curAccount.szIDCardNo, m_csIDCardNo.GetBuffer(0));
	m_csIDCardNo.ReleaseBuffer();

	OnOK();
}

void CTrustTradeOperatorDlg::SetTrader( std::map<int, TraderInfoEx>& mapTraderInfoEx )
{
	m_mapTraderInfoEx = mapTraderInfoEx;
}

void CTrustTradeOperatorDlg::SetIndicator( const std::vector<RiskIndicator>& vIndicator )
{
	m_vIndicator = vIndicator;
}

std::map<int, RiskIndicatorModule> CTrustTradeOperatorDlg::GetRiskIndModule()
{
	return m_mapIndModule;
}

void CTrustTradeOperatorDlg::FillTrader( int nAssetMgmtOrgID, int nFinancialProductID )
{
	m_cbxTrader.ResetContent();

	int i = 0;
	std::map<int, TraderInfoEx>::iterator it = m_mapTraderInfoEx.begin();
	for ( ; it != m_mapTraderInfoEx.end(); ++it )
	{
		if ( it->second.traderInfo.nAssetMgmtOrgID == nAssetMgmtOrgID &&
			it->second.traderInfo.nFinancialProductID == nFinancialProductID )
		{
			m_cbxTrader.AddString(it->second.traderInfo.szName);
			m_cbxTrader.SetItemData(i, it->second.traderInfo.nTraderID);
			i++;
		}
	}
}

void CTrustTradeOperatorDlg::OnCbnSelchangeComboAccountOrg()
{
	// TODO: Add your control notification handler code here
	int nOrgID = m_comboTree.GetSelectItemData();
	FillFinancialProduct(nOrgID);

	FillTrader(nOrgID, 0);
}

void CTrustTradeOperatorDlg::OnCbnSelchangeComboAccountFin()
{
	// TODO: Add your control notification handler code here
	int nOrgID = m_comboTree.GetSelectItemData();
	int nFinSel = m_cbxFinancialProduct.GetCurSel();
	int nFinProductID = m_cbxFinancialProduct.GetItemData(nFinSel);
	FillTrader(nOrgID, nFinProductID);
}

LRESULT CTrustTradeOperatorDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QryValidRiskIndModule_Rsp )
	{
		m_mapIndModule.clear();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(RiskIndicatorModule);
			for ( int i = 0; i < nCount; i++ )
			{
				RiskIndicatorModule* pRiskModule = (RiskIndicatorModule*)((char*)pRecvData->pData + i*sizeof(RiskIndicatorModule));
				m_mapIndModule[pRiskModule->nRiskIndModuleID] = *pRiskModule;
			}

			//填充风险模型列表
			m_cbxIndModule.ResetContent();
			int i = 0;
			std::map<int, RiskIndicatorModule>::iterator it_module = m_mapIndModule.begin();
			for ( ; it_module != m_mapIndModule.end(); ++it_module )
			{
				m_cbxIndModule.AddString(it_module->second.szName);
				m_cbxIndModule.SetItemData(i, it_module->second.nRiskIndModuleID);
				i++;
			}

			if ( m_eType == OPERATOR_MODIFY )
			{
				//风险模型
				for ( int i = 0; i < m_cbxIndModule.GetCount(); i++ )
				{
					if ( m_curAccount.nRiskIndModuleID == m_cbxIndModule.GetItemData(i))
					{
						m_cbxIndModule.SetCurSel(i);
					}
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}

void CTrustTradeOperatorDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

//void CTrustTradeOperatorDlg::OnEnKillfocusEditProfit()
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//
//	m_csProfit.Remove(',');
//	double dProfit = atof(m_csProfit.GetBuffer(0));
//	m_csProfit.ReleaseBuffer();
//	m_csProfit = GetAccountFormatString(dProfit, 2);
//
//	UpdateData(FALSE);
//}

void CTrustTradeOperatorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ( ::GetFocus() == m_edtProfit.GetSafeHwnd() )
	{
		//OnEnKillfocusEditProfit();
		m_edtProfit.OnEnKillfocus();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CTrustTradeOperatorDlg::OnBnClickedBtnRiskModuleSet()
{
	// TODO: Add your control notification handler code here
	CWarningSetDlg dlg;
	dlg.SetIndicator(m_vIndicator);
	dlg.DoModal();

	m_mapIndModule = dlg.GetRiskIndModule();
	m_cbxIndModule.ResetContent();
	int i = 0;
	std::map<int, RiskIndicatorModule>::iterator it_module = m_mapIndModule.begin();
	for ( ; it_module != m_mapIndModule.end(); ++it_module )
	{
		m_cbxIndModule.AddString(it_module->second.szName);
		m_cbxIndModule.SetItemData(i, it_module->second.nRiskIndModuleID);
		if ( m_curAccount.nRiskIndModuleID == it_module->second.nRiskIndModuleID)
		{
			m_cbxIndModule.SetCurSel(i);
		}
		i++;
	}
}
