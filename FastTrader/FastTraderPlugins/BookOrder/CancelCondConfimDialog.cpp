// CancelCondConfimDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "CancelCondConfimDialog.h"


#define BUYSTRING "买  "
#define SELLSTRING "  卖"


CListCtrl* CCancelCondConfimDialog::g_plistSort = NULL;
int CCancelCondConfimDialog::g_nSortColBak = -1;
BOOL CCancelCondConfimDialog::g_bSortAsc = FALSE;

int CALLBACK CCancelCondConfimDialog::CompareProc_CtrlList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNREFERENCED_PARAMETER(lParamSort);
	if(g_plistSort==NULL)
		return 0;

	CString strText1 = g_plistSort->GetItemText(lParam1, lParamSort);
	CString strText2 = g_plistSort->GetItemText(lParam2, lParamSort);
	
	if(g_bSortAsc)
		return strText1.CompareNoCase(strText2);
	else
		return -strText1.CompareNoCase(strText2);
}

// CCancelCondConfimDialog dialog

IMPLEMENT_DYNAMIC(CCancelCondConfimDialog, CDialog)

CCancelCondConfimDialog::CCancelCondConfimDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCancelCondConfimDialog::IDD, pParent)
{
	m_fPriceTick = 0.01;
	m_pDlgBrushBk = NULL;
}

CCancelCondConfimDialog::~CCancelCondConfimDialog()
{
	if(m_pDlgBrushBk!=NULL)
	{
		m_pDlgBrushBk->DeleteObject();
		m_pDlgBrushBk = NULL;
	}
}

void CCancelCondConfimDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTORDERS, m_listOrders);
}


BEGIN_MESSAGE_MAP(CCancelCondConfimDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTORDERS, &CCancelCondConfimDialog::OnLvnColumnclickListorders)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LISTORDERS, OnCustomDraw_OrderList)
	ON_NOTIFY(HDN_BEGINTRACK, 0, &CCancelCondConfimDialog::OnHdnBegintrackListorders)
	ON_NOTIFY(NM_CLICK, IDC_LISTORDERS, &CCancelCondConfimDialog::OnNMClickListorders)
	ON_BN_CLICKED(ID_CANCELSELECTALL, &CCancelCondConfimDialog::OnBnClickedCancelselectall)
	ON_BN_CLICKED(ID_SELECTALL, &CCancelCondConfimDialog::OnBnClickedSelectall)
END_MESSAGE_MAP()


// CCancelCondConfimDialog message handlers


HBRUSH CCancelCondConfimDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	// TODO:  Return a different brush if the default is not desired
	if(pWnd==this)
	{
		return *m_pDlgBrushBk;
	}
	else if(pWnd->GetDlgCtrlID()==IDC_STATIC)
	{
        pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_dwDlgBackColor);
        return m_brushNull;
	}
	return hbr;
}

BOOL CCancelCondConfimDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_pDlgBrushBk = new CBrush(m_dwDlgBackColor);
    m_brushNull.CreateStockObject(NULL_BRUSH); 
	
	m_listOrders.SetExtendedStyle(LVS_EX_GRIDLINES | 
			LVS_EX_FULLROWSELECT | LVS_EX_SIMPLESELECT | LVS_EX_HIDELABELS 
			| LVS_EX_CHECKBOXES);// | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);

	m_listOrders.InsertColumn(0, "", 0, 20);
	m_listOrders.InsertColumn(1, "合约", 0, 54);
	m_listOrders.InsertColumn(2, "编号", 0, 68);
	m_listOrders.InsertColumn(3, "开平", 0, 40);
	m_listOrders.InsertColumn(4, "买卖", 0, 40);
	m_listOrders.InsertColumn(5, "数量", 0, 54);
	m_listOrders.InsertColumn(6, "价格", 0, 54);
	m_listOrders.InsertColumn(7, "偏移", 0, 54);

	ShowOrders();
	if(g_nSortColBak==-1)
		SortList(6, FALSE);
	else
		SortList(g_nSortColBak, FALSE);

	CPoint posItem;
	CRect rectWnd, rectClt, rectItem;
	m_listOrders.GetWindowRect(&rectWnd);
	m_listOrders.GetClientRect(&rectClt);
	m_listOrders.GetItemRect(0, &rectItem, LVIR_BOUNDS);
	m_listOrders.GetItemPosition(0, &posItem);

	int nRealItem = m_mapCanCancelCond.size()<5 ? 5 : 
			(m_mapCanCancelCond.size()>10 ? 10 : m_mapCanCancelCond.size());
	int nReserveHeight = rectWnd.Height()-rectClt.Height()+posItem.y;
	int nRealHeight = nReserveHeight+rectItem.Height()*nRealItem;
	m_listOrders.SetWindowPos(NULL, 0, 0, rectWnd.Width(), nRealHeight, SWP_NOMOVE);

	CRect rectDlg, rectDlgClt;
	GetWindowRect(&rectDlg);
	SetWindowPos(NULL, 0, 0, rectDlg.Width(), 
			rectDlg.Height()+nRealHeight-rectWnd.Height(), SWP_NOMOVE);
	GetClientRect(&rectDlgClt);

	CWnd* pWnd = GetDlgItem(IDOK);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(&rectItem);
		ScreenToClient(&rectItem);
		pWnd->SetWindowPos(NULL, rectItem.left, 
				rectDlgClt.Height()-rectItem.Height()-5, 0, 0, SWP_NOSIZE);
	}
	pWnd = GetDlgItem(IDCANCEL);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(&rectItem);
		ScreenToClient(&rectItem);
		pWnd->SetWindowPos(NULL, rectItem.left, 
				rectDlgClt.Height()-rectItem.Height()-5, 0, 0, SWP_NOSIZE);
	}
	pWnd = GetDlgItem(ID_SELECTALL);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(&rectItem);
		ScreenToClient(&rectItem);
		pWnd->SetWindowPos(NULL, rectItem.left, 
				rectDlgClt.Height()-rectItem.Height()-5, 0, 0, SWP_NOSIZE);
	}
	pWnd = GetDlgItem(ID_CANCELSELECTALL);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(&rectItem);
		ScreenToClient(&rectItem);
		pWnd->SetWindowPos(NULL, rectItem.left, 
				rectDlgClt.Height()-rectItem.Height()-5, 0, 0, SWP_NOSIZE);
	}

	CenterWindow();

	if(m_mapCanCancelCond.size()<=(UINT)nRealItem)
		m_listOrders.SetColumnWidth(6, m_listOrders.GetColumnWidth(6)+20);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCancelCondConfimDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	MAPPRICETRIGGERITEM::iterator it;
	int nItemCount = m_listOrders.GetItemCount();
	CString strText;
	std::string strInstrumentID;
	int nFTID = 0;
	double fPrice = 0.0;
	BOOL bDirection = FALSE;

	for(int i=0; i<nItemCount; i++) {
		if(m_listOrders.GetCheck(i)) {
			strText = m_listOrders.GetItemText(i, 0);
			nFTID = atoi(strText);
			strInstrumentID = (LPCSTR)m_listOrders.GetItemText(i, 1);
			strText = m_listOrders.GetItemText(i, 4);
			bDirection = strText.Compare(BUYSTRING)==0;
			strText = m_listOrders.GetItemText(i, 5);
			fPrice = atof(strText);
			DeleteLocalCondByID(strInstrumentID, nFTID);
		}
	}

	CDialog::OnOK();
}

void CCancelCondConfimDialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

void CCancelCondConfimDialog::CancelAllCond(MAPPRICETRIGGERITEM& mapCanCancelCond)
{
	MAPPRICETRIGGERITEM::iterator it;

	for(it = mapCanCancelCond.begin(); it != mapCanCancelCond.end(); it++) {
		DeleteLocalCondByID(std::string(it->second.InstrumentID), it->second.nID);
	}
}

void CCancelCondConfimDialog::UpdateItem_OrderList(LOCALTRIGGERORDER& tInfo)
{
	CString strText;

	int nRetItem = m_listOrders.InsertItem(m_listOrders.GetItemCount(), "");
	
	m_listOrders.SetCheck(nRetItem);
	strText.Format(TEXT("% 500d"), tInfo.nID);
	m_listOrders.SetItemText(nRetItem, 0, strText);
	strText.Format(TEXT("%s"), tInfo.InstrumentID);
	m_listOrders.SetItemText(nRetItem, 1, strText);
	strText.Format(TEXT("%d"), tInfo.nID);
	m_listOrders.SetItemText(nRetItem, 2, strText);
	strText.Format(TEXT("%s"), tInfo.OpenClose==THOST_FTDC_OF_Open ? "开仓" 
		: (tInfo.OpenClose==THOST_FTDC_OF_CloseToday ? "平今" : "平仓"));
	m_listOrders.SetItemText(nRetItem, 3, strText);
	strText.Format(TEXT("%s"), tInfo.Direction==THOST_FTDC_D_Buy ? BUYSTRING:SELLSTRING);
	m_listOrders.SetItemText(nRetItem, 4, strText);
	strText.Format(TEXT("%d"), tInfo.Qty);
	m_listOrders.SetItemText(nRetItem, 5, strText);
	strText.Format(TEXT("%s"), tInfo.OrderPriceMode==conOrderByMarket 
			? "市价" : Price2String(tInfo.TriggerPrice, m_fPriceTick));
	m_listOrders.SetItemText(nRetItem, 6, strText);
	strText.Format(TEXT("%s"), Price2String(tInfo.OrderOffset, m_fPriceTick));
	m_listOrders.SetItemText(nRetItem, 7, strText);
}

void CCancelCondConfimDialog::ShowOrders()
{
	MAPPRICETRIGGERITEM::iterator it = m_mapCanCancelCond.begin();

	m_listOrders.DeleteAllItems();

	for(; it!=m_mapCanCancelCond.end(); it++) {
		UpdateItem_OrderList(it->second);
	}
}

void CCancelCondConfimDialog::SortList(int nCol, BOOL bChange)
{
	if(g_nSortColBak>=0)
	{
		CString strText;
		LVCOLUMN Vol;
		TCHAR szCol[MAX_PATH];
		ZeroMemory(szCol, sizeof(szCol));
		Vol.iSubItem = g_nSortColBak;
		Vol.mask=LVCF_TEXT;
		Vol.pszText = szCol;
		Vol.cchTextMax=sizeof(szCol);
		m_listOrders.GetColumn(g_nSortColBak, &Vol);
		strText = szCol;
		strText.Replace("↓", "");
		strText.Replace("↑", "");
		strcpy(szCol, strText);
		m_listOrders.SetColumn(g_nSortColBak, &Vol);
	}

	int nItemCount = m_listOrders.GetItemCount();
	for(int i=0; i<nItemCount; i++)
		m_listOrders.SetItemData(i, i);
	//
	g_plistSort = &m_listOrders;
	if(bChange)
	{
		if(g_nSortColBak!=nCol)
			g_bSortAsc = FALSE;
		else
			g_bSortAsc = !g_bSortAsc;
	}
	m_listOrders.SortItems(CompareProc_CtrlList, nCol);
	g_nSortColBak = nCol;

	{
		CString strText;
		LVCOLUMN Vol;
		TCHAR szCol[MAX_PATH];
		ZeroMemory(szCol, sizeof(szCol));
		Vol.iSubItem = g_nSortColBak;
		Vol.mask=LVCF_TEXT;
		Vol.pszText = szCol;
		Vol.cchTextMax=sizeof(szCol);
		m_listOrders.GetColumn(g_nSortColBak, &Vol);
		if(g_bSortAsc)
			strText = "↑";
		else
			strText = "↓";
		strText += szCol;
		strcpy(szCol, strText);
		m_listOrders.SetColumn(g_nSortColBak, &Vol);
	}
}

void CCancelCondConfimDialog::OnLvnColumnclickListorders(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	SortList(pNMLV->iSubItem);
	*pResult = 0;
}

afx_msg void CCancelCondConfimDialog::OnCustomDraw_OrderList(NMHDR* pNMHDR, LRESULT* pResult)
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	if(pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if(pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		COLORREF clrItemBKColor = RGB(255,255,224);
		COLORREF clrItemEvenBKColor = RGB(224,255,224);
  		COLORREF clrBuyTextColor = RGB(255,0,0);
		COLORREF clrSellTextColor = RGB(0,128,0);
      
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		BOOL bIsEvenItem = nItem%2;

		pLVCD->clrTextBk = bIsEvenItem ? clrItemEvenBKColor : clrItemBKColor;
		pLVCD->clrText = RGB(0, 0, 0);

		if(pLVCD->iSubItem == 0)
		{
			pLVCD->clrText = bIsEvenItem ? clrItemEvenBKColor : clrItemBKColor;
		}
		else if(pLVCD->iSubItem == 4)
		{
			CString strTemp = m_listOrders.GetItemText(nItem, pLVCD->iSubItem);
			if(strTemp.CompareNoCase(BUYSTRING)==0)
			{
				pLVCD->clrText = clrBuyTextColor;
			}
			else
			{
				pLVCD->clrText = clrSellTextColor;
			}
		}
        
        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
        
	}
}

void CCancelCondConfimDialog::OnHdnBegintrackListorders(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	//if(phdr->iItem==0)
	//{
	//	if(phdr->pitem->cxy>23)
	//		*pResult = 0;
	//	else
	//		*pResult = -1;
	//}
	//else
		*pResult = 0;
}

void CCancelCondConfimDialog::OnNMClickListorders(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	static int nLastItem = -1;
	if(pNMItemActivate->iSubItem>0) {
		if(pNMItemActivate->iItem>=0 && pNMItemActivate->iItem!=nLastItem)
			m_listOrders.SetCheck(pNMItemActivate->iItem, !m_listOrders.GetCheck(pNMItemActivate->iItem));
		nLastItem = pNMItemActivate->iItem;
	}
	*pResult = 0;
}

void CCancelCondConfimDialog::OnBnClickedCancelselectall()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<m_listOrders.GetItemCount(); i++)
		m_listOrders.SetCheck(i, FALSE);
}

void CCancelCondConfimDialog::OnBnClickedSelectall()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<m_listOrders.GetItemCount(); i++)
		m_listOrders.SetCheck(i, TRUE);
}
