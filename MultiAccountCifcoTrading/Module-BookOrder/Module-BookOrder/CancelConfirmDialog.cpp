// CancelConfirmDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "CancelConfirmDialog.h"


#define BUYSTRING "买  "
#define SELLSTRING "  卖"


CListCtrl* g_plistSort = NULL;
int g_nSortColBak = -1;
BOOL g_bSortAsc = FALSE;

int CALLBACK CompareProc_CtrlList(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

// CCancelConfirmDialog dialog

IMPLEMENT_DYNAMIC(CCancelConfirmDialog, CDialog)

CCancelConfirmDialog::CCancelConfirmDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCancelConfirmDialog::IDD, pParent)
{
	m_fPriceTick = 0.01;
	m_pDlgBrushBk = NULL;
}

CCancelConfirmDialog::~CCancelConfirmDialog()
{
	if(m_pDlgBrushBk!=NULL)
	{
		m_pDlgBrushBk->DeleteObject();
		m_pDlgBrushBk = NULL;
	}
}

void CCancelConfirmDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTORDERS, m_listOrders);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL CCancelConfirmDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	//m_btnOk.PreTranslateMessage(pMsg);
	//m_btnCancel.PreTranslateMessage(pMsg);
 
	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CCancelConfirmDialog, CDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTORDERS, &CCancelConfirmDialog::OnLvnColumnclickListorders)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LISTORDERS, OnCustomDraw_OrderList)
	ON_NOTIFY(HDN_BEGINTRACK, 0, &CCancelConfirmDialog::OnHdnBegintrackListorders)
	ON_NOTIFY(NM_CLICK, IDC_LISTORDERS, &CCancelConfirmDialog::OnNMClickListorders)
END_MESSAGE_MAP()


// CCancelConfirmDialog message handlers

HBRUSH CCancelConfirmDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CCancelConfirmDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_pDlgBrushBk = new CBrush(m_dwDlgBackColor);
    m_brushNull.CreateStockObject(NULL_BRUSH); 
	m_btnOk.SetBGColor(m_dwDlgButtonBackColor);
	m_btnCancel.SetBGColor(m_dwDlgButtonBackColor);
	
	m_listOrders.SetExtendedStyle(LVS_EX_GRIDLINES | 
			LVS_EX_FULLROWSELECT | LVS_EX_SIMPLESELECT | LVS_EX_HIDELABELS 
			| LVS_EX_CHECKBOXES);// | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);

	m_listOrders.InsertColumn(0, "", 0, 20);
	m_listOrders.InsertColumn(1, "合约", 0, 54);
	m_listOrders.InsertColumn(2, "报单编号", 0, 68);
	m_listOrders.InsertColumn(3, "开平", 0, 40);
	m_listOrders.InsertColumn(4, "买卖", 0, 40);
	m_listOrders.InsertColumn(5, "未成交量", 0, 66);
	m_listOrders.InsertColumn(6, "价格", 0, 54);

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

	int nRealItem = m_mapCanCancelOrder.size()<5 ? 5 : 
			(m_mapCanCancelOrder.size()>10 ? 10 : m_mapCanCancelOrder.size());
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

	CenterWindow();

	if(m_mapCanCancelOrder.size()<=nRealItem)
		m_listOrders.SetColumnWidth(6, m_listOrders.GetColumnWidth(6)+20);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCancelConfirmDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	std::map<int, PlatformStru_OrderInfo>::iterator it;
	int nItemCount = m_listOrders.GetItemCount();
	CString strText;
	int nFTID = 0;

	for(int i=0; i<nItemCount; i++)
	{
		if(m_listOrders.GetCheck(i))
		{
			strText = m_listOrders.GetItemText(i, 0);
			nFTID = atoi(strText);
			it = m_mapCanCancelOrder.find(nFTID);
			if(it != m_mapCanCancelOrder.end())
			{
				CancelOrder(it->second);
			}
		}
	}

	CDialog::OnOK();
}

void CCancelConfirmDialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

void CCancelConfirmDialog::UpdateItem_OrderList(PlatformStru_OrderInfo& tInfo)
{
	CString strText;

	int nRetItem = m_listOrders.InsertItem(m_listOrders.GetItemCount(), "");
	
	m_listOrders.SetCheck(nRetItem);
	strText.Format(TEXT("% 500d"), tInfo.FTID);
	m_listOrders.SetItemText(nRetItem, 0, strText);
	strText.Format(TEXT("%s"), tInfo.InstrumentID);
	m_listOrders.SetItemText(nRetItem, 1, strText);
	strText.Format(TEXT("%s"), tInfo.OrderSysID);
	strText.Trim();
	m_listOrders.SetItemText(nRetItem, 2, strText);
	strText.Format(TEXT("%s"), tInfo.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? "开仓" 
		: (tInfo.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? "平今" : "平仓"));
	m_listOrders.SetItemText(nRetItem, 3, strText);
	strText.Format(TEXT("%s"), tInfo.Direction==THOST_FTDC_D_Buy ? BUYSTRING:SELLSTRING);
	m_listOrders.SetItemText(nRetItem, 4, strText);
	strText.Format(TEXT("%d"), tInfo.VolumeTotal);
	m_listOrders.SetItemText(nRetItem, 5, strText);
	strText.Format(TEXT("%s"), tInfo.OrderPriceType==THOST_FTDC_OPT_AnyPrice 
			? "市价" : Price2String(tInfo.LimitPrice, m_fPriceTick));
	m_listOrders.SetItemText(nRetItem, 6, strText);
}

void CCancelConfirmDialog::ShowOrders()
{
	std::map<int, PlatformStru_OrderInfo>::iterator it = m_mapCanCancelOrder.begin();

	m_listOrders.DeleteAllItems();

	for(; it!=m_mapCanCancelOrder.end(); it++) {
		UpdateItem_OrderList(it->second);
	}
}

void CCancelConfirmDialog::CancelOrder(PlatformStru_OrderInfo& tInfo)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlatformStru_InputOrderAction tAction;
		ZeroMemory(&tAction, sizeof(tAction));
        tAction.Thost.FrontID=tInfo.FrontID;
        tAction.Thost.SessionID=tInfo.SessionID;
        memcpy(tAction.Thost.OrderRef, tInfo.OrderRef, sizeof(tInfo.OrderRef));
        memcpy(tAction.Thost.OrderSysID, tInfo.OrderSysID, sizeof(tInfo.OrderSysID));
        memcpy(tAction.Thost.ExchangeID, tInfo.ExchangeID, sizeof(tInfo.ExchangeID));
		memcpy(tAction.Thost.InstrumentID, tInfo.InstrumentID, sizeof(tInfo.InstrumentID));
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ORDER_CANCEL_ORDER, &tAction, sizeof(tAction), nSeqNum);
	}
}

void CCancelConfirmDialog::SortList(int nCol, BOOL bChange)
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

void CCancelConfirmDialog::OnLvnColumnclickListorders(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	SortList(pNMLV->iSubItem);
	*pResult = 0;
}

afx_msg void CCancelConfirmDialog::OnCustomDraw_OrderList(NMHDR* pNMHDR, LRESULT* pResult)
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

void CCancelConfirmDialog::OnHdnBegintrackListorders(NMHDR *pNMHDR, LRESULT *pResult)
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

void CCancelConfirmDialog::OnNMClickListorders(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	static int nLastItem = -1;
	if(pNMItemActivate->iItem>=0 && pNMItemActivate->iItem!=nLastItem)
		m_listOrders.SetCheck(pNMItemActivate->iItem, !m_listOrders.GetCheck(pNMItemActivate->iItem));
	nLastItem = pNMItemActivate->iItem;
	*pResult = 0;
}
