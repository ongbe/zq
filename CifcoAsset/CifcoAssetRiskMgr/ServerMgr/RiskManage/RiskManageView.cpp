// RiskManageView.cpp : CRiskManageView 类的实现
//

#include "stdafx.h"
#include "RiskManage.h"

#include "RiskManageDoc.h"
#include "RiskManageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRiskManageView

IMPLEMENT_DYNCREATE(CRiskManageView, CFormView)

BEGIN_MESSAGE_MAP(CRiskManageView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VIEW, &CRiskManageView::OnTcnSelchangeTabView)
END_MESSAGE_MAP()

// CRiskManageView 构造/析构

CRiskManageView::CRiskManageView()
	: CFormView(CRiskManageView::IDD)
	, m_bInited(false)
{
	// TODO: 在此处添加构造代码

}

CRiskManageView::~CRiskManageView()
{
}

void CRiskManageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_VIEW, m_mainTab);
}

void CRiskManageView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_mainTab.InsertItem(0,"委托交易账户");
	m_mainTab.InsertItem(1,"管理员/风控员");
	m_mainTab.InsertItem(2,"交易员");

	m_manageAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);
	m_trustTradeAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);
	m_traderAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);

	//创建列表
	std::vector<CString> vManageTitle;
	vManageTitle.push_back(_T("账户号"));
	vManageTitle.push_back(_T("角色"));
	vManageTitle.push_back(_T("权限"));
	vManageTitle.push_back(_T("所属资管"));
	vManageTitle.push_back(_T("理财产品"));
	vManageTitle.push_back(_T("账号状态"));
	vManageTitle.push_back(_T("用户名"));
	vManageTitle.push_back(_T("联系方式"));
	m_manageAccountDlg.CreateListColumn(vManageTitle);
	m_manageAccountDlg.SetAccountType(ACCOUNT_MANAGE);

	std::vector<CString> vTradeTitle;
	vTradeTitle.push_back(_T("账户号"));
	vTradeTitle.push_back(_T("所属资管"));
	vTradeTitle.push_back(_T("理财产品"));
	vTradeTitle.push_back(_T("风险模型"));
	vTradeTitle.push_back(_T("用户名"));
	vTradeTitle.push_back(_T("联系方式"));
	vTradeTitle.push_back(_T("证件类型"));
	vTradeTitle.push_back(_T("证件号码"));
	vTradeTitle.push_back(_T("交易员"));
	vTradeTitle.push_back(_T("初始权益"));
	m_trustTradeAccountDlg.CreateListColumn(vTradeTitle);
	m_trustTradeAccountDlg.SetAccountType(ACCOUNT_TRUST_TRADE);

	std::vector<CString> vTraderTitle;
	vTraderTitle.push_back(_T("姓名"));
	vTraderTitle.push_back(_T("所属资管"));
	vTraderTitle.push_back(_T("理财产品"));
	vTraderTitle.push_back(_T("座机"));
	vTraderTitle.push_back(_T("手机"));
	vTraderTitle.push_back(_T("其他联系方式"));
	m_traderAccountDlg.CreateListColumn(vTraderTitle);
	m_traderAccountDlg.SetAccountType(ACCOUNT_TRADER);

	CRect rc;
	m_mainTab.GetClientRect(rc);
	CRect rcTabItem;
	m_mainTab.GetItemRect(0, rcTabItem);
	rc.top += rcTabItem.Height() + 2;
	m_manageAccountDlg.MoveWindow(rc);
	m_trustTradeAccountDlg.MoveWindow(rc);
	m_traderAccountDlg.MoveWindow(rc);

	m_trustTradeAccountDlg.ShowWindow(SW_SHOW);
	m_manageAccountDlg.ShowWindow(SW_HIDE);
	m_traderAccountDlg.ShowWindow(SW_HIDE);
	m_mainTab.SetCurSel(0);

	m_bInited = true;
}

BOOL CRiskManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRiskManageView 绘制

void CRiskManageView::OnDraw(CDC* /*pDC*/)
{
	CRiskManageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CRiskManageView 打印

BOOL CRiskManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CRiskManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CRiskManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CRiskManageView 诊断

#ifdef _DEBUG
void CRiskManageView::AssertValid() const
{
	CView::AssertValid();
}

void CRiskManageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRiskManageDoc* CRiskManageView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRiskManageDoc)));
	return (CRiskManageDoc*)m_pDocument;
}
#endif //_DEBUG


// CRiskManageView 消息处理程序

void CRiskManageView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if ( m_bInited )
	{
		CRect rc;
		GetClientRect(rc);
		m_mainTab.MoveWindow(rc);

		m_mainTab.GetClientRect(rc);
		CRect rcTabItem;
		m_mainTab.GetItemRect(0, rcTabItem);
		rc.top += rcTabItem.Height() + 2;
		m_manageAccountDlg.MoveWindow(rc);
		m_trustTradeAccountDlg.MoveWindow(rc);
		m_traderAccountDlg.MoveWindow(rc);
	}
}

int CRiskManageView::OnCreate( LPCREATESTRUCT lpcs )
{
	if (CFormView::OnCreate(lpcs) == -1)
		return -1;

	return 0;
}

void CRiskManageView::OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nCurSel = m_mainTab.GetCurSel();
	int nAccountID = 0;
	switch(nCurSel)
	{
	case ACCOUNT_TRUST_TRADE:
		m_manageAccountDlg.ShowWindow(SW_HIDE);
		m_trustTradeAccountDlg.ShowWindow(SW_SHOW);
		m_traderAccountDlg.ShowWindow(SW_HIDE);
		break;
	case ACCOUNT_MANAGE:
		m_trustTradeAccountDlg.ShowWindow(SW_HIDE);
		m_manageAccountDlg.ShowWindow(SW_SHOW);
		m_traderAccountDlg.ShowWindow(SW_HIDE);
		break;
	case ACCOUNT_TRADER:
		m_manageAccountDlg.ShowWindow(SW_HIDE);
		m_trustTradeAccountDlg.ShowWindow(SW_HIDE);
		m_traderAccountDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	AfxGetMainWnd()->SendMessage(MSG_MAIN_TAB_SEL_CHANGE, 0, 0);
}

int CRiskManageView::GetCurPageIndex()
{
	return m_mainTab.GetCurSel();
}

AccountType CRiskManageView::GetAccountType()
{
	return static_cast<AccountType>(m_mainTab.GetCurSel());
}

void CRiskManageView::GetSelectAccountID(std::vector<int>& vec)
{
	int nCurSel = m_mainTab.GetCurSel();
	switch(nCurSel)
	{
	case ACCOUNT_TRUST_TRADE:
		m_trustTradeAccountDlg.GetSelectAccountID(vec);
		break;
	case ACCOUNT_MANAGE:
		m_manageAccountDlg.GetSelectAccountID(vec);
		break;
	case ACCOUNT_TRADER:
		m_traderAccountDlg.GetSelectAccountID(vec);
		break;
	default:
		break;
	}

	return;
}

void CRiskManageView::ClearAllData()
{
	m_trustTradeAccountDlg.ClearData();
	m_manageAccountDlg.ClearData();
	m_traderAccountDlg.ClearData();
}

void CRiskManageView::ClearData(AccountType eType)
{
	if ( eType == ACCOUNT_MANAGE )
	{
		m_manageAccountDlg.ClearData();
	}
	else if( eType == ACCOUNT_TRUST_TRADE )
	{
		m_trustTradeAccountDlg.ClearData();
	}
	else
	{
		m_traderAccountDlg.ClearData();
	}
}

void CRiskManageView::SetData( std::map<int, RiskMgmtUserInfoEx>& accountMap )
{
	m_manageAccountDlg.ClearData();
	std::vector<CAccountListData> vListData;

	std::map<int, RiskMgmtUserInfoEx>::iterator it = accountMap.begin();
	for ( ; it != accountMap.end(); ++it )
	{
		CAccountListData listData;
		GenerateListData(it->second, listData);
		vListData.push_back(listData);
	}

	m_manageAccountDlg.SetData(vListData);
}

void CRiskManageView::SetData( std::map<int, TrustTradeAccountEx>& trustTradeMap )
{
	m_trustTradeAccountDlg.ClearData();
	std::vector<CAccountListData> vListData;

	DWORD t1 = GetTickCount();
	std::map<int, TrustTradeAccountEx>::iterator it = trustTradeMap.begin();
	for ( ; it != trustTradeMap.end(); ++it )
	{
		CAccountListData listData;
		GenerateListData(it->second, listData);
		vListData.push_back(listData);
	}

	DWORD t2 = GetTickCount();
	char szBuffer[100];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "step 1:%d\n", t2-t1);
	OutputDebugString(szBuffer);

	m_trustTradeAccountDlg.SetData(vListData);

	DWORD t3 = GetTickCount();
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "step 2:%d\n", t3-t2);
	OutputDebugString(szBuffer);
}

void CRiskManageView::SetData( std::map<int, TraderInfoEx>& traderMap )
{
	m_traderAccountDlg.ClearData();
	std::vector<CAccountListData> vListData;

	std::map<int, TraderInfoEx>::iterator it = traderMap.begin();
	for ( ; it != traderMap.end(); ++it )
	{
		CAccountListData listData;
		GenerateListData(it->second, listData);
		vListData.push_back(listData);
	}

	m_traderAccountDlg.SetData(vListData);
}

void CRiskManageView::AddAccount( const RiskMgmtUserInfoEx& userInfoEx )
{
	CAccountListData listData;
	GenerateListData(userInfoEx, listData);
	m_manageAccountDlg.AddAccount(listData);
}

void CRiskManageView::AddAccount( const TrustTradeAccountEx& trustTradeEx )
{
	CAccountListData listData;
	GenerateListData(trustTradeEx, listData);
	m_trustTradeAccountDlg.AddAccount(listData);
}

void CRiskManageView::AddAccount( const TraderInfoEx& traderInfoEx )
{
	CAccountListData listData;
	GenerateListData(traderInfoEx, listData);
	m_traderAccountDlg.AddAccount(listData);
}

void CRiskManageView::ModifyAccount( const TraderInfoEx& traderEx )
{
	CAccountListData listData;
	GenerateListData(traderEx, listData);
	m_traderAccountDlg.ModifyAccount(listData);
}

void CRiskManageView::ModifyAccount( const RiskMgmtUserInfoEx& userInfoEx )
{
	CAccountListData listData;
	GenerateListData(userInfoEx, listData);
	m_manageAccountDlg.ModifyAccount(listData);
}

void CRiskManageView::ModifyAccount( const TrustTradeAccountEx& trustTradeEx )
{
	CAccountListData listData;
	GenerateListData(trustTradeEx, listData);
	m_trustTradeAccountDlg.ModifyAccount(listData);
}

void CRiskManageView::DelAccount( int nAccountID, AccountType eType )
{
	if ( eType == ACCOUNT_TRUST_TRADE)
	{
		m_trustTradeAccountDlg.DelAccount(nAccountID);
	}
	else if ( eType == ACCOUNT_MANAGE )
	{
		m_manageAccountDlg.DelAccount(nAccountID);
	}
	else
	{
		m_traderAccountDlg.DelAccount(nAccountID);
	}
}

void CRiskManageView::ShowListData( AccountType eType, bool bShow )
{
	
}

void CRiskManageView::GenerateListData( const RiskMgmtUserInfoEx &userInfoEx, CAccountListData &listData )
{
	listData.SetKey(userInfoEx.userInfo.nRiskMgmtUserID);
	listData.AddValue(userInfoEx.userInfo.szAccount);
	if ( userInfoEx.userInfo.nRoleType == ROLE_TYPE_MANAGE )
	{
		listData.AddValue(_T("管理员"));
	}
	else
	{
		listData.AddValue(_T("风控员"));
	}
	listData.AddValue(userInfoEx.szPermissionName);
	listData.AddValue(userInfoEx.szAssetMgmtOrgName);
	listData.AddValue(userInfoEx.szFinancialProductName);
	if ( userInfoEx.userInfo.nStatus == 0 )
	{
		listData.AddValue(_T("未启用"));
	}
	else
	{
		listData.AddValue(_T("启用"));
	}
	listData.AddValue(userInfoEx.userInfo.szName);
	listData.AddValue(userInfoEx.userInfo.szContactInfo);
}

void CRiskManageView::GenerateListData( const TrustTradeAccountEx &trustTradeEx, CAccountListData &listData )
{
	listData.SetKey(trustTradeEx.trustAccount.nTradeAccountID);
	listData.AddValue(trustTradeEx.trustAccount.szAccount);
	listData.AddValue(trustTradeEx.szAssetMgmtOrgName);
	listData.AddValue(trustTradeEx.szFinancialProductName);
	listData.AddValue(trustTradeEx.szRiskIncModuleName);
	listData.AddValue(trustTradeEx.trustAccount.szName);
	listData.AddValue(trustTradeEx.trustAccount.szContactInfo);
	if ( trustTradeEx.trustAccount.nIDCardType == IDENTIFY_CARD )
	{
		listData.AddValue(_T("身份证"));
	}
	else if( trustTradeEx.trustAccount.nIDCardType == MILITARY_ID )
	{
		listData.AddValue(_T("军官证"));
	}
	else
	{
		listData.AddValue(_T("其他"));
	}
	listData.AddValue(trustTradeEx.trustAccount.szIDCardNo);
	listData.AddValue(trustTradeEx.szTraderName);

	CString csTemp = GetAccountFormatString(trustTradeEx.trustAccount.dProfit, 2);
	listData.AddValue(csTemp);
}

void CRiskManageView::GenerateListData( const TraderInfoEx &traderInfoEx, CAccountListData &listData )
{
	listData.SetKey(traderInfoEx.traderInfo.nTraderID);
	listData.AddValue(traderInfoEx.traderInfo.szName);
	listData.AddValue(traderInfoEx.szAssetMgmtOrgName);
	listData.AddValue(traderInfoEx.szFinancialProductName);
	listData.AddValue(traderInfoEx.traderInfo.szTel);
	listData.AddValue(traderInfoEx.traderInfo.szMobile);
	listData.AddValue(traderInfoEx.traderInfo.szOtherContactInfo);
}

//void CRiskManageView::SetPermissionData( const PermissionData& foreData, 
//											const PermissionData& bkgData )
//{
//	m_tradeAccountDlg.SetPermissionData(foreData);
//	m_manageAccountDlg.SetPermissionData(bkgData);
//}
