// MAManageClientView.cpp : CMAManageClientView 类的实现
//

#include "stdafx.h"
#include "MAManageClient.h"

#include "MAManageClientDoc.h"
#include "MAManageClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMAManageClientView

IMPLEMENT_DYNCREATE(CMAManageClientView, CFormView)

BEGIN_MESSAGE_MAP(CMAManageClientView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VIEW, &CMAManageClientView::OnTcnSelchangeTabView)
END_MESSAGE_MAP()

// CMAManageClientView 构造/析构

CMAManageClientView::CMAManageClientView()
	: CFormView(CMAManageClientView::IDD)
	, m_bInited(false)
	, m_bShowForeListData(false)
	, m_bShowBkgListData(false)
{
	// TODO: 在此处添加构造代码

}

CMAManageClientView::~CMAManageClientView()
{
}

void CMAManageClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_VIEW, m_mainTab);
}

void CMAManageClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_mainTab.InsertItem(0,"前台用户");
	m_mainTab.InsertItem(1,"管理者");

	m_bkgAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);
	m_foreAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);

	CRect rc;
	m_mainTab.GetClientRect(rc);
	CRect rcTabItem;
	m_mainTab.GetItemRect(0, rcTabItem);
	rc.top += rcTabItem.Height() + 2;
	m_bkgAccountDlg.MoveWindow(rc);
	m_foreAccountDlg.MoveWindow(rc);

	m_bkgAccountDlg.ShowWindow(SW_HIDE);
	m_foreAccountDlg.ShowWindow(SW_SHOW);
	m_mainTab.SetCurSel(0);

	m_bInited = true;
}

BOOL CMAManageClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMAManageClientView 绘制

void CMAManageClientView::OnDraw(CDC* /*pDC*/)
{
	CMAManageClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMAManageClientView 打印

BOOL CMAManageClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMAManageClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMAManageClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMAManageClientView 诊断

#ifdef _DEBUG
void CMAManageClientView::AssertValid() const
{
	CView::AssertValid();
}

void CMAManageClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMAManageClientDoc* CMAManageClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMAManageClientDoc)));
	return (CMAManageClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CMAManageClientView 消息处理程序

void CMAManageClientView::OnSize(UINT nType, int cx, int cy)
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
		m_bkgAccountDlg.MoveWindow(rc);
		m_foreAccountDlg.MoveWindow(rc);
	}
}

int CMAManageClientView::OnCreate( LPCREATESTRUCT lpcs )
{
	if (CFormView::OnCreate(lpcs) == -1)
		return -1;

	return 0;
}

void CMAManageClientView::OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nCurSel = m_mainTab.GetCurSel();
	int nAccountID = 0;
	switch(nCurSel)
	{
	case 0:
		m_bkgAccountDlg.ShowWindow(SW_HIDE);
		m_foreAccountDlg.ShowWindow(SW_SHOW);
		//nAccountID = m_foreAccountDlg.GetSelectAccountID();
		break;
	case 1:
		m_bkgAccountDlg.ShowWindow(SW_SHOW);
		m_foreAccountDlg.ShowWindow(SW_HIDE);
		//nAccountID = m_bkgAccountDlg.GetSelectAccountID();
		break;
	default:
		break;
	}

	AfxGetMainWnd()->SendMessage(MSG_MAIN_TAB_SEL_CHANGE, 0, 0);
	//AfxGetMainWnd()->SendMessage(MSG_LIST_SEL_CHANGE, 0, nAccountID);
}

int CMAManageClientView::GetCurPageIndex()
{
	return m_mainTab.GetCurSel();
}

AccountType CMAManageClientView::GetAccountType()
{
	if ( m_mainTab.GetCurSel() == 0 )
	{
		return ACCOUNT_TYPE_LOGON;
	}
	else
	{
		return ACCOUNT_TYPE_MANAGE;
	}
}

void CMAManageClientView::GetSelectAccountID(std::vector<int>& vec)
{
	int nCurSel = m_mainTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		m_foreAccountDlg.GetSelectAccountID(vec);
		break;
	case 1:
		m_bkgAccountDlg.GetSelectAccountID(vec);
		break;
	default:
		break;
	}

	return;
}

void CMAManageClientView::ClearAllData()
{
	m_bkgAccountDlg.ClearData();
	m_foreAccountDlg.ClearData();
}

void CMAManageClientView::ClearData(AccountType eType)
{
	if ( eType == ACCOUNT_TYPE_MANAGE )
	{
		m_bkgAccountDlg.ClearData();
	}
	else
	{
		m_foreAccountDlg.ClearData();
	}
}

void CMAManageClientView::SetData( std::map<int, AccountInfoEx>& accountMap )
{
	m_bkgAccountDlg.ClearData();
	m_foreAccountDlg.ClearData();

	std::map<int, AccountInfoEx> bkgAccountInfoMap;
	std::map<int, AccountInfoEx> foreAccountInfoMap;
	bkgAccountInfoMap.clear();
	foreAccountInfoMap.clear();

	std::map<int, AccountInfoEx>::iterator it = accountMap.begin();
	for ( ; it != accountMap.end(); ++it )
	{
		if ( it->second.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE )
		{
			bkgAccountInfoMap[it->second.accountInfo.nAccountID] = it->second;
		}
		else
		{
			foreAccountInfoMap[it->second.accountInfo.nAccountID] = it->second;
		}
	}

	if ( m_bShowBkgListData )
	{
		m_bkgAccountDlg.SetData(bkgAccountInfoMap);
	}
	
	if ( m_bShowForeListData )
	{
		m_foreAccountDlg.SetData(foreAccountInfoMap);
	}

	AfxGetMainWnd()->SendMessage(MSG_LIST_SEL_CHANGE, 0, -1);
}

void CMAManageClientView::SetData( AccountType eType, std::map<int, AccountInfoEx>& accountMap )
{
	if ( eType == ACCOUNT_TYPE_MANAGE )
	{
		m_bkgAccountDlg.ClearData();
		if ( m_bShowBkgListData )
		{
			m_bkgAccountDlg.SetData(accountMap);
		}
	}
	else
	{
		m_foreAccountDlg.ClearData();
		if ( m_bShowForeListData )
		{
			m_foreAccountDlg.SetData(accountMap);
		}
	}
}

void CMAManageClientView::AddAccount( const AccountInfoEx& accountInfoEx )
{
	if ( accountInfoEx.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE)
	{
		if ( m_bShowBkgListData )
		{
			m_bkgAccountDlg.AddAccount(accountInfoEx);
		}
	}
	else
	{
		if ( m_bShowForeListData )
		{
			m_foreAccountDlg.AddAccount(accountInfoEx);
		}
	}
}

void CMAManageClientView::AddAccount(AccountType eType, const AccountInfoEx& accountInfoEx)
{
	if ( eType == ACCOUNT_TYPE_MANAGE )
	{
		if ( m_bShowBkgListData )
		{
			m_bkgAccountDlg.AddAccount(accountInfoEx);
		}
	}
	else
	{
		if ( m_bShowForeListData )
		{
			m_foreAccountDlg.AddAccount(accountInfoEx);
		}
	}
}

void CMAManageClientView::ModifyAccount( const AccountInfoEx& accountInfoEx )
{
	if ( accountInfoEx.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE)
	{
		if ( m_bShowBkgListData )
		{
			m_bkgAccountDlg.ModifyAccount(accountInfoEx);
		}
	}
	else
	{
		if ( m_bShowForeListData )
		{
			m_foreAccountDlg.ModifyAccount(accountInfoEx);
		}
	}
}

void CMAManageClientView::DelAccount( int nAccountID, AccountType eType )
{
	if ( eType == ACCOUNT_TYPE_MANAGE)
	{
		if ( m_bShowBkgListData )
		{
			m_bkgAccountDlg.DelAccount(nAccountID);
		}
	}
	else
	{
		if ( m_bShowForeListData )
		{
			m_foreAccountDlg.DelAccount(nAccountID);
		}
	}
}

void CMAManageClientView::ShowForeListData( bool bShow )
{
	m_bShowForeListData = bShow;
}

void CMAManageClientView::ShowBkgListData( bool bShow )
{
	m_bShowBkgListData = bShow;
}

void CMAManageClientView::SetPermissionData( const PermissionData& foreData, 
											const PermissionData& bkgData )
{
	m_foreAccountDlg.SetPermissionData(foreData);
	m_bkgAccountDlg.SetPermissionData(bkgData);
}
