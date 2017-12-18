// DlgOrderSetting.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "DlgOrderSetting.h"


// CDlgOrderSetting dialog

IMPLEMENT_DYNAMIC(CDlgOrderSetting, CDialog)

CDlgOrderSetting::CDlgOrderSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOrderSetting::IDD, pParent)
	, m_nOrderOffset(0)
	, m_bHasLocalSetting(FALSE)
{

}

CDlgOrderSetting::~CDlgOrderSetting()
{
}

void CDlgOrderSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AutoOCKey, m_cmbAuotOCKey);
	DDX_Control(pDX, IDC_COMBO_OpenKey, m_cmbOpenKey);
	DDX_Control(pDX, IDC_COMBO_CloseKey, m_cmbCloseKey);
	DDX_Control(pDX, IDC_COMBO_CondOrderPriceMode, m_cmbCondOrderPriceMode);
	DDX_Text(pDX, IDC_EDIT_ORDEROFFSET, m_nOrderOffset);
	DDV_MinMaxInt(pDX, m_nOrderOffset, -10000, 10000);
	DDX_Check(pDX, IDC_CHECK_HASLOCALSETTING, m_bHasLocalSetting);
}


BEGIN_MESSAGE_MAP(CDlgOrderSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgOrderSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgOrderSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgOrderSetting message handlers

BOOL CDlgOrderSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_cmbAuotOCKey.AddString("");
	m_cmbAuotOCKey.AddString("Shift");
	m_cmbAuotOCKey.AddString("Ctrl");
	m_cmbAuotOCKey.AddString("Alt");
	m_cmbAuotOCKey.SetCurSel(m_orderSetting.nAutoOrderKey);

	m_cmbOpenKey.AddString("");
	m_cmbOpenKey.AddString("Shift");
	m_cmbOpenKey.AddString("Ctrl");
	m_cmbOpenKey.AddString("Alt");
	m_cmbOpenKey.SetCurSel(m_orderSetting.nOpenOrderKey);

	m_cmbCloseKey.AddString("");
	m_cmbCloseKey.AddString("Shift");
	m_cmbCloseKey.AddString("Ctrl");
	m_cmbCloseKey.AddString("Alt");
	m_cmbCloseKey.SetCurSel(m_orderSetting.nCloseOrderKey);

	m_cmbCondOrderPriceMode.AddString("市价");
	m_cmbCondOrderPriceMode.AddString("限价");
	m_cmbCondOrderPriceMode.AddString("买卖价");
	m_cmbCondOrderPriceMode.SetCurSel(m_orderSetting.nCondOrderPriceMode);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOrderSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if(!UpdateData())
		return;

	if(m_cmbAuotOCKey.GetCurSel()==m_cmbOpenKey.GetCurSel() || 
			m_cmbOpenKey.GetCurSel()==m_cmbCloseKey.GetCurSel() ||
			m_cmbAuotOCKey.GetCurSel()==m_cmbCloseKey.GetCurSel()) {
		MessageBox("自动/开仓/平仓快捷键设置有重复，解决后再提交。");
		return;
	}

	if(m_cmbCondOrderPriceMode.GetCurSel()==conOrderByMarket) {
		MessageBox("不是所有交易所都支持市价单，请确认您要交易的合约是否支持市价单。");
	}

	m_orderSetting.nAutoOrderKey = m_cmbAuotOCKey.GetCurSel();
	m_orderSetting.nOpenOrderKey = m_cmbOpenKey.GetCurSel();
	m_orderSetting.nCloseOrderKey = m_cmbCloseKey.GetCurSel();
	m_orderSetting.nCondOrderPriceMode = m_cmbCondOrderPriceMode.GetCurSel();
	m_orderSetting.nCondOrderOffset = m_nOrderOffset;

	OnOK();
}

void CDlgOrderSetting::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
