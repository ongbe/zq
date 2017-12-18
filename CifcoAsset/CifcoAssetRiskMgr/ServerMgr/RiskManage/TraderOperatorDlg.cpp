// TrustAccountOperatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "TraderOperatorDlg.h"


// CTrustAccountOperatorDlg dialog

IMPLEMENT_DYNAMIC(CTraderOperatorDlg, CDialog)

CTraderOperatorDlg::CTraderOperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraderOperatorDlg::IDD, pParent)
	, m_csAccount(_T(""))
	, m_csTel(_T(""))
	, m_csMobile(_T(""))
	, m_csOtherContactInfo(_T(""))
{
	memset(&m_curTraderInfo, 0, sizeof(m_curTraderInfo));
}

CTraderOperatorDlg::~CTraderOperatorDlg()
{
}

void CTraderOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_csAccount);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_FIN, m_cbxFinancialProduct);
	DDX_Text(pDX, IDC_EDIT_TEL, m_csTel);
	DDX_Text(pDX, IDC_EDIT_MOBILE, m_csMobile);
	DDX_Text(pDX, IDC_EDIT_OTHER_CONTACTINFO, m_csOtherContactInfo);
}


BEGIN_MESSAGE_MAP(CTraderOperatorDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTraderOperatorDlg::OnBnClickedOk)
	ON_CBN_SELENDOK(IDC_COMBO_ACCOUNT_ORG, &CTraderOperatorDlg::OnCbnSelchangeComboAccountOrg)
END_MESSAGE_MAP()

BOOL CTraderOperatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT))->SetLimitText(MAX_ACCOUNT_LENGTH);
	((CEdit*)GetDlgItem(IDC_EDIT_TEL))->SetLimitText(MAX_PHONE_NO_LENGTH);
	((CEdit*)GetDlgItem(IDC_EDIT_MOBILE))->SetLimitText(MAX_PHONE_NO_LENGTH);
	((CEdit*)GetDlgItem(IDC_EDIT_OTHER_CONTACTINFO))->SetLimitText(MAX_PHONE_NO_LENGTH);

	//取消组合框风格
	SetWindowTheme( m_cbxFinancialProduct.m_hWnd, L"", L"");

	//填充所属资管
	m_comboTree.SubclassDlgItem (IDC_COMBO_ACCOUNT_ORG, this);
	m_comboTree.SetAssetMgmtOrg(m_mapAssetMgmtOrgTree);

	CString csTitle;
	if ( m_eType == OPERATOR_MODIFY )
	{
		SetModifyData();
		csTitle = _T("修改交易员信息");
	}
	else
	{
		csTitle = _T("新增交易员信息");
	}

	SetWindowText(csTitle);

	return TRUE;
}

void CTraderOperatorDlg::SetOperatorType( OperatorType eType )
{
	m_eType = eType;
}

void CTraderOperatorDlg::SetTraderInfo( const TraderInfo& traderInfo )
{
	m_curTraderInfo = traderInfo;
}

const TraderInfo& CTraderOperatorDlg::GetTraderInfo()
{
	return m_curTraderInfo;
}

void CTraderOperatorDlg::SetAssetMgmtOrgTree( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree )
{
	m_mapAssetMgmtOrgTree = mapAssetMgmtOrgTree;
}

void CTraderOperatorDlg::SetFinancialProduct( std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree )
{
	m_mapFinancialProductTree = mapFinancialProductTree;
}

void CTraderOperatorDlg::FillFinancialProduct( int nAssetMgmtOrgID )
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

void CTraderOperatorDlg::SetModifyData()
{
	m_csAccount = m_curTraderInfo.szName;
	m_csTel = m_curTraderInfo.szTel;
	m_csMobile = m_curTraderInfo.szMobile;
	m_csOtherContactInfo = m_curTraderInfo.szOtherContactInfo;

	//所属资管
	m_comboTree.SelectItem(m_curTraderInfo.nAssetMgmtOrgID);

	//填充理财产品列表
	FillFinancialProduct(m_curTraderInfo.nAssetMgmtOrgID);
	for ( int i = 0; i < m_cbxFinancialProduct.GetCount(); i++ )
	{
		if ( m_curTraderInfo.nFinancialProductID == m_cbxFinancialProduct.GetItemData(i))
		{
			m_cbxFinancialProduct.SetCurSel(i);
		}
	}

	UpdateData(FALSE);

	((CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT))->EnableWindow(FALSE);
}


// CTrustAccountOperatorDlg message handlers

void CTraderOperatorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if ( m_csAccount.IsEmpty() )
	{
		MessageBox(_T("账号不能为空！"), lpszPrompt, MB_ICONERROR);
		((CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT))->SetFocus();
		return;
	}

	if ( m_comboTree.GetCurSelItem() == NULL )
	{
		MessageBox(lpszEmptyOrg, lpszPrompt, MB_ICONERROR);
		m_comboTree.SetFocus();
		return;
	}

	strcpy(m_curTraderInfo.szName,m_csAccount.GetBuffer(0));
	m_csAccount.ReleaseBuffer();

	m_curTraderInfo.nAssetMgmtOrgID = m_comboTree.GetSelectItemData();
	int nFinancialProduct = m_cbxFinancialProduct.GetCurSel();
	if ( nFinancialProduct != -1 )
	{
		m_curTraderInfo.nFinancialProductID = m_cbxFinancialProduct.GetItemData(nFinancialProduct);
	}
	else
	{
		m_curTraderInfo.nFinancialProductID = 0;
	}

	strcpy(m_curTraderInfo.szTel,m_csTel.GetBuffer(0));
	m_csTel.ReleaseBuffer();
	strcpy(m_curTraderInfo.szMobile,m_csMobile.GetBuffer(0));
	m_csMobile.ReleaseBuffer();
	strcpy(m_curTraderInfo.szOtherContactInfo,m_csOtherContactInfo.GetBuffer(0));
	m_csOtherContactInfo.ReleaseBuffer();

	OnOK();
}

void CTraderOperatorDlg::OnCbnSelchangeComboAccountOrg()
{
	// TODO: Add your control notification handler code here
	FillFinancialProduct(m_comboTree.GetSelectItemData());
}
