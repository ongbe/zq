// TPriceListPanel.cpp : implementation file
//

#include "stdafx.h"
#include "TPriceList.h"
#include "TPriceListPanel.h"
#include "Global.h"
#include "DlgSetOptionCols.h"


// CTPriceListPanel dialog

IMPLEMENT_DYNAMIC(CTPriceListPanel, CDialog)

CTPriceListPanel::CTPriceListPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CTPriceListPanel::IDD, pParent)
{
	m_nColMode = 0;
	m_nLastEventID = 0;
	m_fontTitle.CreatePointFont(160, "微软雅黑");
	m_fontTarget.CreatePointFont(130, "新宋体");
	m_brushRed.CreateSolidBrush(RGB(255, 0, 0));
	m_brushGreen.CreateSolidBrush(GREEN);
	InitializeCriticalSection(&m_cs);
}

CTPriceListPanel::~CTPriceListPanel()
{
	m_fontTitle.DeleteObject();
	DeleteCriticalSection(&m_cs);
}

void CTPriceListPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDTTABLE, m_gridTTable);
}


BEGIN_MESSAGE_MAP(CTPriceListPanel, CDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDOK, &CTPriceListPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTPriceListPanel::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_GRIDTTABLE, &CTPriceListPanel::OnOptionGrid_Click)
	ON_NOTIFY(NM_RCLICK, IDC_GRIDTTABLE, &CTPriceListPanel::OnOptionGrid_RClick)
	ON_NOTIFY(NM_DBLCLK, IDC_GRIDTTABLE, &CTPriceListPanel::OnOptionGrid_DBClick)
	ON_COMMAND(ID_TARGETGRID_COLSETTING, &CTPriceListPanel::On_TargetGrid_ColSetting)
END_MESSAGE_MAP()


// CTPriceListPanel message handlers

LRESULT CTPriceListPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CTPriceListPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CStatic* pLabel = (CStatic*)GetDlgItem(IDC_LBL_TARGETID);
	if(pLabel != NULL) {
		pLabel->SetFont(&m_fontTarget);
	}
	pLabel = (CStatic*)GetDlgItem(IDC_STATIC_CALL);
	if(pLabel != NULL) {
		pLabel->SetFont(&m_fontTitle);
	}
	pLabel = (CStatic*)GetDlgItem(IDC_STATIC_PUT);
	if(pLabel != NULL) {
		pLabel->SetFont(&m_fontTitle);
	}

	BOOL bRet = FALSE;

	bRet = m_bmpKLineView.LoadBitmap(IDB_BMP_KLINEVIEW);
	//bRet = m_ImageList.Create(18, 16, ILC_COLOR24, 0, 0);
	//bRet = m_ImageList.Add(&m_bmpKLineView, RGB(255,255,255));

	bRet = m_ImageList.Create(MAKEINTRESOURCE(IDB_BMP_KLINEVIEW), 18, ILC_COLOR24, RGB(255,255,255));
	//bRet = m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP1), 48, ILC_COLOR, RGB(255,255,255));
	m_gridTTable.SetImageList(&m_ImageList);

	m_gridTTable.SetDefCellHeight(23);
	m_gridTTable.SetDefCellMargin(1);
	m_gridTTable.SetEditable(FALSE);
	m_gridTTable.EnableSelection(TRUE);
	m_gridTTable.SetListMode(TRUE);
	m_gridTTable.SetSingleRowSelection(TRUE);
	m_gridTTable.SetFixedColumnSelection(FALSE);
	m_gridTTable.EnableDragAndDrop(FALSE);
	m_gridTTable.SetColumnResize(TRUE);	//TRUE
	m_gridTTable.SetHeaderSort(FALSE);		//TRUE
	m_gridTTable.SetTrackFocusCell(FALSE);
	m_gridTTable.SetShowFocusRect(FALSE);
	m_gridTTable.SetBkColor(RGB(255, 255, 255));
	m_gridTTable.SetHeaderSort(FALSE);

	{
	LockGlobalObject();
	std::map<int, std::string> mapCols = GetOptionColMap();
	std::vector<int> vecCol = GetOptionShowCol();
	UnlockGlobalObject();
	InitTTableHead(mapCols, vecCol);
	}

	ResetColorSuite(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTPriceListPanel::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CTPriceListPanel::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

int CTPriceListPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CTPriceListPanel::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

HBRUSH CTPriceListPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Call the base class implementation first! Otherwise, it may
	// undo what we're trying to accomplish here.
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Are we painting the IDC_MYSTATIC control? We can use
	// CWnd::GetDlgCtrlID() to perform the most efficient test.
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CALL) {
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(255, 0, 0));
		hbr = m_brushRed;
	}
	else if(pWnd->GetDlgCtrlID() == IDC_STATIC_PUT) {
		// Set the text color to red
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(GREEN);
		// Return handle to our CBrush object
		hbr = m_brushGreen;

		// Set the background mode for text to transparent 
		// so background will show thru.
		//pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}

BOOL CTPriceListPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

void CTPriceListPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void CTPriceListPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CStatic* pLabel = (CStatic*)GetDlgItem(IDC_LBL_TARGETID);
	if(pLabel != NULL && pLabel->GetSafeHwnd() != NULL) {
		pLabel->MoveWindow(cx/2-35, 5, 70, 20);
	}
	pLabel = (CStatic*)GetDlgItem(IDC_STATIC_CALL);
	if(pLabel != NULL && pLabel->GetSafeHwnd() != NULL) {
		pLabel->MoveWindow(0, 0, cx/2, 30);
	}
	pLabel = (CStatic*)GetDlgItem(IDC_STATIC_PUT);
	if(pLabel != NULL) {
		pLabel->MoveWindow(cx/2, 0, cx/2+1, 30);
	}
	if(m_gridTTable.GetSafeHwnd()!=NULL) {
		m_gridTTable.MoveWindow(0, 30, cx, cy-30);
		for(int i=0; i<m_gridTTable.GetColumnCount(); i++) 
			m_gridTTable.SetColumnWidth(i, 72);
		int nCol = FindColIndex(m_gridTTable, QUOT_OptionPrice);
		if(nCol >= 0)
			m_gridTTable.SetColumnWidth(nCol, 92);
		m_gridTTable.ExpandColumnsToFit();
		m_gridTTable.ResetScrollBars();
		//ResetColorSuite(m_nColMode);
	}

}

void CTPriceListPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CTPriceListPanel::InitTTableHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol)
{
	m_gridTTable.SetRowCount(1);
	m_gridTTable.SetFixedRowCount(1);

	int i=0;
	std::map<int, std::string>::iterator it;

	m_gridTTable.SetColumnCount(vecCol.size());
	for(i=0; i<(int)vecCol.size(); i++) {
		if((it = mapCols.find(vecCol[i])) != mapCols.end()) {
			m_gridTTable.SetItemText(0, i, it->second.c_str());
			m_gridTTable.SetItemData(0, i, it->first);
			m_gridTTable.SetColumnWidth(i, 72);
		}
	}
	int nCol = FindColIndex(m_gridTTable, QUOT_OptionPrice);
	if(nCol >= 0)
		m_gridTTable.SetColumnWidth(nCol, 92);
	m_gridTTable.Refresh();
}

// 内部未加锁
void CTPriceListPanel::InitTTableGrid(const vector<PlatformStru_InstrumentInfo>& vecOptions)
{
	std::set<std::string>::iterator it;
	std::set<std::string> setNewID;
	std::vector<std::string> vecNewOptionID;
	CString strPrice, strExchangeID, strTargetID;
	BOOL bDirect = FALSE;
	std::map<std::string, std::map<bool, std::string>> mapPrice;
	std::map<std::string, std::map<bool, std::string>>::iterator itPrice;
	
	for(int i=0; i<(int)vecOptions.size(); i++) {
		strPrice.Format("%0.0f", vecOptions[i].StrikePrice);
		strTargetID = vecOptions[i].UnderlyingInstrID;
		itPrice = mapPrice.find(std::string((LPCSTR)strPrice));
		if(itPrice == mapPrice.end()) {
			std::map<bool, std::string> map;
			map.insert(std::make_pair(
					vecOptions[i].OptionsType==THOST_FTDC_CP_CallOptions, vecOptions[i].InstrumentID));
			mapPrice.insert(std::make_pair(std::string((LPCSTR)strPrice), map));
		}
		else {
			itPrice->second[vecOptions[i].OptionsType==THOST_FTDC_CP_CallOptions] 
					= vecOptions[i].InstrumentID;
		}
	}

	m_mapPrice2RowID.clear();
	m_mapRowID2OptionID.clear();

	m_gridTTable.SetRowCount(1+mapPrice.size());
	int nItemCount = m_gridTTable.GetRowCount();
	int nColCount = m_gridTTable.GetColumnCount();

	for(int i=1; i<nItemCount; i++) {
		for(int k=0; k<nColCount; k++) {
			m_gridTTable.SetItemText(i, k, "");
			m_gridTTable.SetItemBkColour(i, k, i%2 ? RGB(255, 255, 255) : RGB(235, 235, 235));

			GV_ITEM Item;

			Item.mask = GVIF_IMAGE;
			Item.row = i;

			Item.iImage = 0;
			Item.col = FindColIndex(m_gridTTable, QUOT_KLineView);
			m_gridTTable.SetItem(&Item);
			Item.col = FindColIndex(m_gridTTable, QUOTNEWHEAD + QUOT_KLineView);
			m_gridTTable.SetItem(&Item);
		}
	}
	int nCol = FindColIndex(m_gridTTable, QUOT_OptionPrice);
	//std::map<int, std::map<bool, std::string>>::iterator itRow;
	if(nCol >= 0) {
		int i=1;
		for(itPrice = mapPrice.begin(); itPrice != mapPrice.end(); itPrice++,i++) {
			m_gridTTable.SetItemText(i, nCol, itPrice->first.c_str());
			m_gridTTable.SetItemData(i, 0, i);
			m_mapPrice2RowID.insert(std::make_pair(itPrice->first, i));
			m_mapRowID2OptionID.insert(std::make_pair(i, itPrice->second));
		}
	}
	m_gridTTable.ResetScrollBars();
	m_gridTTable.Refresh();
}

int CTPriceListPanel::FindColIndex(CGridCtrl& grid, int nColID)
{
	for(int i=0; i<grid.GetColumnCount(); i++) {
		if(grid.GetItemData(0, i)==nColID)
			return i;
	}
	return -1;
}

int CTPriceListPanel::FindColID(CGridCtrl& grid, int nColIndex)
{
	if(nColIndex>=0 && nColIndex<grid.GetColumnCount())
		return grid.GetItemData(0, nColIndex);
	else
		return -1;
}

int CTPriceListPanel::FindRowIndex(CGridCtrl& grid, int nRowID)
{
	for(int i=1; i<grid.GetRowCount(); i++) {
		if(grid.GetItemData(i, 0)==nRowID)
			return i;
	}
	return -1;
}

void CTPriceListPanel::UpdateTTableQuot(const PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld)
{
	int nRow = -1;
	CString strText, strPrice, strExchangeID;
	BOOL bDirect = TRUE;
	double fPriceTick = 0.0;
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator itInstr;
	
	if((itInstr = m_mapInstrument.find(quot.InstrumentID)) == m_mapInstrument.end())
		return;
	strExchangeID = itInstr->second.ExchangeID;
	strPrice.Format("%0.0f", itInstr->second.StrikePrice);
	bDirect = itInstr->second.OptionsType==THOST_FTDC_CP_CallOptions;
	fPriceTick = itInstr->second.PriceTick;
	
	std::map<std::string, int>::iterator it = m_mapPrice2RowID.find((LPSTR)(LPCSTR)strPrice);
	if(it != m_mapPrice2RowID.end()) {
		CGridCtrl::CLockObject lock(&m_gridTTable.m_csLock);
		nRow = FindRowIndex(m_gridTTable, it->second);
		if(nRow!=-1)
			UpdateGrid(m_gridTTable, nRow, quot, bHasOld, pQuotOld, fPriceTick, bDirect);
	}
}

void CTPriceListPanel::UpdateGrid(CGridCtrl& grid, int nRow, const PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
										double fPriceTick, BOOL bDirect)
{
	BOOL bIsOption = TRUE;
	CString strText;
	int nHead = 0, nSymbol = 1, nCol = -1;
	DWORD dwColor = 0;
	const double fBasePrice = (util::isInvalidValue(quot.PreSettlementPrice) ? 
							quot.OpenPrice : quot.PreSettlementPrice);

	if(bIsOption && !bDirect) {
		nHead = QUOTNEWHEAD;
		//nSymbol = -1;
	}
	
	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_InstrumentID))!=-1) {
	//	grid.SetItemText(nRow, nCol, quot.InstrumentID);
	//}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LastPrice))!=-1) {
		strText.Format("%s", Price2String(quot.LastPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.LastPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_ChangeValue))!=-1) {
	//	double fSub = quot.LastPrice - fBasePrice;
	//	strText.Format("%s", Price2String(fSub, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = fSub < 0.0 ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}
	
	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_ChangePercentage))!=-1) {
	//	double fSub = (quot.LastPrice - fBasePrice)/fBasePrice*100.0;
	//	strText.Format("%0.2f", fSub);
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = fSub < 0.0 ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_BidPrice1))!=-1) {
		strText.Format("%s", Price2String(quot.BidPrice1, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.BidPrice1 < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_BidVolume1))!=-1) {
	//	strText.Format("%d", quot.BidVolume1);
	//	grid.SetItemText(nRow, nCol, strText);
	//	//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	//}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_AskPrice1))!=-1) {
		strText.Format("%s", Price2String(quot.AskPrice1, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.AskPrice1 < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_AskVolume1))!=-1) {
	//	strText.Format("%d", quot.AskVolume1);
	//	grid.SetItemText(nRow, nCol, strText);
	//	//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_OpenInterest))!=-1) {
	//	strText.Format("%0.0f", quot.OpenInterest);
	//	grid.SetItemText(nRow, nCol, strText);
	//	//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_OpenPrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.OpenPrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = quot.OpenPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	////if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_PreSettlementPrice))!=-1) {
	////	strText.Format("%s", Price2String(quot.PreSettlementPrice, fPriceTick));
	////	grid.SetItemText(nRow, nCol, strText);
	////	//dwColor = quot.AskPrice1 < fBasePrice ? GREEN : RGB(255, 0, 0);
	////	//grid.SetItemFgColour(nRow, nCol, dwColor);
	////}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LastVolume))!=-1) {
		int nNewVolume = 0;
		if(bHasOld)
			nNewVolume = quot.Volume - pQuotOld->Volume;
		strText.Format("%d", nNewVolume);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_HighestPrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.HighestPrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = quot.HighestPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LowestPrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.LowestPrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = quot.LowestPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_ClosePrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.ClosePrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = quot.ClosePrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_Volume))!=-1) {
		strText.Format("%d", quot.Volume);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_Turnover))!=-1) {
	//	strText.Format("%0.2f", quot.Turnover);
	//	grid.SetItemText(nRow, nCol, strText);
	//	//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_UpperLimitPrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.UpperLimitPrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = quot.UpperLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LowerLimitPrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.LowerLimitPrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	dwColor = quot.LowerLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_SettlementPrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.SettlementPrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	//dwColor = quot.LowerLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	//grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_PreClosePrice))!=-1) {
	//	strText.Format("%s", Price2String(quot.PreClosePrice, fPriceTick));
	//	grid.SetItemText(nRow, nCol, strText);
	//	//dwColor = quot.LowerLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	//grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_PreOpenInterest))!=-1) {
	//	strText.Format("%0.0f", quot.PreOpenInterest);
	//	grid.SetItemText(nRow, nCol, strText);
	//	//dwColor = quot.PreOpenInterest < fBasePrice ? GREEN : RGB(255, 0, 0);
	//	//grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_UpdateTime))!=-1) {
	//	grid.SetItemText(nRow, nCol, quot.UpdateTime);
	//	//grid.SetItemFgColour(nRow, nCol, dwColor);
	//}

	grid.Refresh();
}

void CTPriceListPanel::ResetInstrumentList(const vector<PlatformStru_InstrumentInfo>& vecOptions)
{
	std::vector<std::string> vecStdInstr;
	std::vector<std::string> vecOptionInstr;

	LockObject();

	m_mapInstrument.clear();
	for(int i=0; i<vecOptions.size(); i++) {
		m_mapInstrument[vecOptions[i].InstrumentID] = vecOptions[i];
	}
	m_vecOptions = vecOptions;

	InitTTableGrid(m_vecOptions);

	UnLockObject();

	ResetColorSuite(m_nColMode);

	CStatic* pLabel = (CStatic*)GetDlgItem(IDC_LBL_TARGETID);
	if(vecOptions.size()>0 && pLabel != NULL && pLabel->GetSafeHwnd() != NULL) 
		pLabel->SetWindowText(vecOptions[0].UnderlyingInstrID);
	else
		pLabel->SetWindowText("无合约");
}

void CTPriceListPanel::ResetCols(int colmode)
{
	LockGlobalObject();
	std::map<int, std::string> mapCols = GetOptionColMap();
	std::vector<int> vecCol = GetOptionShowCol();
	UnlockGlobalObject();
	InitTTableHead(mapCols, vecCol);
	InitTTableGrid(m_vecOptions);
}

// 内部未加锁
void CTPriceListPanel::ResetCols(int colmode, DWORD dwFgColor, DWORD dwBkColor)
{
	LockObject();

	int nItemCount = m_gridTTable.GetRowCount();
	int nColCount = m_gridTTable.GetColumnCount();
	int nPriceCol = FindColIndex(m_gridTTable, QUOT_OptionPrice);

	for(int i=1; i<nItemCount; i++) {
		for(int k=0; k<nColCount; k++) {
			m_gridTTable.SetItemFgColour(i, k, dwFgColor);
			if(k==nPriceCol) {
				if(colmode==0) 
					m_gridTTable.SetItemBkColour(i, k, dwBkColor);
				else 
					m_gridTTable.SetItemBkColour(i, k, RGB(31,66,201));
			}
			else 
				m_gridTTable.SetItemBkColour(i, k, dwBkColor);
		}
	}

	std::map<std::string, PlatformStru_DepthMarketData>::iterator it;
	for(it = m_mapQuot.begin(); it != m_mapQuot.end(); it++) {
		ParseQuot(&it->second);
	}
	m_gridTTable.SetBkColor(dwBkColor);

	m_gridTTable.SetFixedTextColor(dwFgColor);
	if(colmode==0) 
		m_gridTTable.SetFixedBkColor(RGB(151,151,151));
	else 
		m_gridTTable.SetFixedBkColor(RGB(31,66,201));
	m_gridTTable.Refresh();
	m_gridTTable.ExpandColumnsToFit();

	UnLockObject();
}

void CTPriceListPanel::ParseQuot(const PlatformStru_DepthMarketData* pDepthData)
{
	if(pDepthData==NULL)
		return;

	LockObject();

	std::map<std::string, PlatformStru_DepthMarketData>::iterator it;
	BOOL bHasOld = FALSE;
	std::string strInstrumentID;
	int nRow = -1;
	PlatformStru_DepthMarketData oldQuot;

	strInstrumentID = pDepthData->InstrumentID;
	it = m_mapQuot.find(strInstrumentID);
	if(it != m_mapQuot.end()) {
		oldQuot = it->second;
		m_mapOldQuot[strInstrumentID] = oldQuot;
		bHasOld = TRUE;
	}
	
	if(it != m_mapQuot.end()) {
		it->second = *pDepthData;
	}
	else {
		m_mapQuot[strInstrumentID] = *pDepthData;
	}

	UpdateTTableQuot(*pDepthData, bHasOld, &oldQuot);

	UnLockObject();
}

bool CTPriceListPanel::ReadClickParam(int EventParamID, Stru_PriceListT_ClickParam& retParam)
{
	std::map<int, Stru_PriceListT_ClickParam>::iterator it;
	bool bRet = false;

	LockObject();

	it = m_mapEvent.find(EventParamID);
	if(it != m_mapEvent.end()) {
		bRet = true;
		retParam = it->second;
		m_mapEvent.erase(it);
	}

	UnLockObject();

	return bRet;
}

void CTPriceListPanel::ResetColorSuite(int colmode)
{
	LockObject();

	m_nColMode = colmode;
	ResetCols(colmode, colmode==0 ? RGB(0,0,0) : RGB(255, 255, 255), colmode==0 ? RGB(255,255,255) : RGB(31, 31, 31));

	UnLockObject();
}

void CTPriceListPanel::SetItem_CenterAlign(CGridCtrl& gridPrice)
{
	CString strText;
	int nFixedCount = gridPrice.GetFixedRowCount();
	int nItemCount = gridPrice.GetRowCount() - nFixedCount;
	for(int i=0; i<nItemCount; i++)
	{
		for(int k=0; k<5; k++)
			gridPrice.SetItemFormat(i+nFixedCount, k, DT_CENTER);
	}
}

void CTPriceListPanel::OnOptionGrid_Click(NMHDR * pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGrid = (NM_GRIDVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	BOOL bHasEvent = FALSE;

	std::map<int, std::map<bool, std::string>>::iterator itRow = 
			m_mapRowID2OptionID.find(pNMGrid->iRow);
	if(itRow==m_mapRowID2OptionID.end()) {
		*pResult = 0;
		return;
	}

	int nNewEventID = 0;
	std::map<bool, std::string>::iterator itOpt;
	Stru_PriceListT_ClickParam param = {0};

	switch(FindColID(m_gridTTable, pNMGrid->iColumn))
	{
	case QUOT_BidPrice1:
		itOpt = itRow->second.find(true);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "buy");
			param.bDirection = TRUE;
			param.nClickMode = 0;
		}
		break;
	case QUOT_AskPrice1:
		itOpt = itRow->second.find(true);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "sell");
			param.bDirection = FALSE;
			param.nClickMode = 0;
		}
		break;
	case QUOT_BidPrice1+QUOTNEWHEAD:
		itOpt = itRow->second.find(false);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "buy");
			param.bDirection = TRUE;
			param.nClickMode = 0;
		}
		break;
	case QUOT_AskPrice1+QUOTNEWHEAD:
		itOpt = itRow->second.find(false);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "sell");
			param.bDirection = FALSE;
			param.nClickMode = 0;
		}
		break;
	case QUOT_KLineView:
		itOpt = itRow->second.find(true);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "sell");
			param.bDirection = FALSE;
			param.nClickMode = 2;
			//char szText[1024] = {0};
			//sprintf(szText, "点击合约[%s]的K线图", itOpt->second.c_str());
			//::AfxMessageBox(szText);
		}
		break;
	case QUOT_KLineView+QUOTNEWHEAD:
		itOpt = itRow->second.find(false);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "sell");
			param.bDirection = FALSE;
			param.nClickMode = 2;
			//char szText[1024] = {0};
			//sprintf(szText, "点击合约[%s]的K线图", itOpt->second.c_str());
			//::AfxMessageBox(szText);
		}
		break;
	};

	if(bHasEvent) {
		//param.nClickMode = 0;
		m_nLastEventID++;
		nNewEventID = m_nLastEventID;

		m_mapEvent.insert(std::make_pair(nNewEventID, param));
		::PostMessage(_GetMainWnd(), Msg_PriceListT_Click, nNewEventID, NULL);
	}

	*pResult = 0;
}

void CTPriceListPanel::OnOptionGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGrid = (NM_GRIDVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	CMenu menu;
	CMenu* pPopMenu = NULL;
	CPoint point(0, 0);

	menu.LoadMenu(IDR_MENU_TARGETGRID);
	pPopMenu = menu.GetSubMenu(0);
	GetCursorPos(&point);
	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}

void CTPriceListPanel::OnOptionGrid_DBClick(NMHDR * pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGrid = (NM_GRIDVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	BOOL bHasEvent = FALSE;

	std::map<int, std::map<bool, std::string>>::iterator itRow = 
			m_mapRowID2OptionID.find(pNMGrid->iRow);
	if(itRow==m_mapRowID2OptionID.end()) {
		*pResult = 0;
		return;
	}

	int nNewEventID = 0;
	std::map<bool, std::string>::iterator itOpt;
	Stru_PriceListT_ClickParam param = {0};

	switch(FindColID(m_gridTTable, pNMGrid->iColumn))
	{
	case QUOT_BidPrice1:
		itOpt = itRow->second.find(true);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "buy");
			param.bDirection = TRUE;
		}
		break;
	case QUOT_AskPrice1:
		itOpt = itRow->second.find(true);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "sell");
			param.bDirection = FALSE;
		}
		break;
	case QUOT_BidPrice1+QUOTNEWHEAD:
		itOpt = itRow->second.find(false);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "buy");
			param.bDirection = TRUE;
		}
		break;
	case QUOT_AskPrice1+QUOTNEWHEAD:
		itOpt = itRow->second.find(false);
		if(itOpt != itRow->second.end()) {
			bHasEvent = TRUE;
			strncpy(param.szInstrument, itOpt->second.c_str(), sizeof(param.szInstrument)-1);
			//strcpy(param.szColName, "sell");
			param.bDirection = FALSE;
		}
		break;
	};

	if(bHasEvent) {
		param.nClickMode = 1;
		m_nLastEventID++;
		nNewEventID = m_nLastEventID;

		m_mapEvent.insert(std::make_pair(nNewEventID, param));
		::PostMessage(_GetMainWnd(), Msg_PriceListT_Click, nNewEventID, NULL);
	}
	
	*pResult = 0;
}

void CTPriceListPanel::On_TargetGrid_ColSetting()
{
	// TODO: Add your command handler code here
	CDlgSetOptionCols dlg(this);
	if(dlg.DoModal()==IDOK) {
		//ResetCols(m_nColMode);
		ResetColorSuite(m_nColMode);
	}
}
