// DlgOptionTTablePopup.cpp : implementation file
//

#include "stdafx.h"
#include "OptionPriceListT.h"
#include "DlgOptionTTablePopup.h"
#include "DlgSetOptionCols.h"


// CDlgOptionTTablePopup dialog

IMPLEMENT_DYNAMIC(CDlgOptionTTablePopup, CDialog)

CDlgOptionTTablePopup::CDlgOptionTTablePopup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionTTablePopup::IDD, pParent)
{
	m_bExiting = false;
	m_pCurrGrid = NULL;
	m_bUserChangeInstrumentID = false;
	m_nDlgMinWidth = 0;
	m_nDlgMinHeight = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgOptionTTablePopup::~CDlgOptionTTablePopup()
{
}

void CDlgOptionTTablePopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDINSTRUMENT, m_gridTarget);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_GRIDTTABLE, m_gridTTable);
}


BEGIN_MESSAGE_MAP(CDlgOptionTTablePopup, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDOK, &CDlgOptionTTablePopup::OnBnClickedOk)
	ON_COMMAND(ID_TARGETGRID_COLSETTING, &CDlgOptionTTablePopup::OnTargetGrid_ColSetting)
	ON_COMMAND(ID_TARGETGRID_AUTOSIZECOLS, &CDlgOptionTTablePopup::OnTargetGrid_AutoSizeCols)
	ON_NOTIFY(NM_RCLICK, IDC_GRIDINSTRUMENT, &CDlgOptionTTablePopup::OnTargetGrid_RClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRIDTTABLE, &CDlgOptionTTablePopup::OnOptionGrid_RClick)
END_MESSAGE_MAP()


// CDlgOptionTTablePopup message handlers

LRESULT CDlgOptionTTablePopup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgOptionTTablePopup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_gridTarget.SetDefCellHeight(20);
	m_gridTarget.SetDefCellMargin(3);
	m_gridTarget.SetEditable(FALSE);
	m_gridTarget.EnableSelection(TRUE);
	m_gridTarget.SetListMode(TRUE);
	m_gridTarget.SetSingleRowSelection(TRUE);
	m_gridTarget.SetFixedColumnSelection(FALSE);
	m_gridTarget.EnableDragAndDrop(FALSE);
	m_gridTarget.SetColumnResize(TRUE);
	m_gridTarget.SetHeaderSort(TRUE);
	m_gridTarget.SetTrackFocusCell(FALSE);
	m_gridTarget.SetShowFocusRect(FALSE);
	m_gridTarget.SetBkColor(RGB(255, 255, 255));
	m_gridTarget.SetHeaderSort(FALSE);

	m_gridTTable.SetDefCellHeight(20);
	m_gridTTable.SetDefCellMargin(3);
	m_gridTTable.SetEditable(FALSE);
	m_gridTTable.EnableSelection(TRUE);
	m_gridTTable.SetListMode(TRUE);
	m_gridTTable.SetSingleRowSelection(TRUE);
	m_gridTTable.SetFixedColumnSelection(FALSE);
	m_gridTTable.EnableDragAndDrop(FALSE);
	m_gridTTable.SetColumnResize(TRUE);
	m_gridTTable.SetHeaderSort(TRUE);
	m_gridTTable.SetTrackFocusCell(FALSE);
	m_gridTTable.SetShowFocusRect(FALSE);
	m_gridTTable.SetBkColor(RGB(255, 255, 255));
	m_gridTTable.SetHeaderSort(FALSE);

	{
	LockGlobalObject();
	std::map<int, std::string> mapCols = GetTargetColMap();
	std::vector<int> vecCol = GetTargetShowCol();
	UnlockGlobalObject();
	InitTargetHead(mapCols, vecCol);
	}

	{
	LockGlobalObject();
	std::map<int, std::string> mapCols = GetOptionColMap();
	std::vector<int> vecCol = GetOptionShowCol();
	UnlockGlobalObject();
	InitTTableHead(mapCols, vecCol);
	}

	CRect rect;
	
	GetWindowRect(&rect);
	m_nDlgMinWidth = rect.Width();
	m_nDlgMinHeight = rect.Height();
	GetClientRect(&rect);
	ResetSize(rect.Width(), rect.Height());

	//SetTimer(conTimerResize, 20, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptionTTablePopup::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	//KillTimer(conTimerResize);
}

void CDlgOptionTTablePopup::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	PostMessage(WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), NULL);
	
	//CDialog::OnClose();
}

void CDlgOptionTTablePopup::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(GetSafeHwnd()==NULL || m_gridTarget.GetSafeHwnd()==NULL || 
			m_gridTTable.GetSafeHwnd()==NULL)
		return;

	ResetSize(cx, cy);
}

void CDlgOptionTTablePopup::ResetSize(int cx, int cy)
{
	CRect rect;

	m_gridTarget.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right = cx - 7;
	m_gridTarget.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOMOVE);
	m_gridTarget.ResetScrollBars();
	m_gridTarget.Refresh();
	{
	int nMinPos = 0, nMaxPos = 0;
	CRect rect;
	m_gridTTable.GetWindowRect(&rect);
	m_gridTarget.GetScrollRange(SB_HORZ, &nMinPos, &nMaxPos);
	m_gridTarget.SetScrollPos32(SB_HORZ, (nMaxPos-nMinPos-rect.Width())/2);
	}

	m_gridTTable.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right = cx - 7;
	rect.bottom = cy - 37;
	m_gridTTable.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOMOVE);
	m_gridTTable.ResetScrollBars();
	m_gridTTable.Refresh();
	{
	int nMinPos = 0, nMaxPos = 0;
	CRect rect;
	m_gridTTable.GetWindowRect(&rect);
	m_gridTTable.GetScrollRange(SB_HORZ, &nMinPos, &nMaxPos);
	m_gridTTable.SetScrollPos32(SB_HORZ, (nMaxPos-nMinPos-rect.Width())/2);
	}

	CWnd* pWnd = GetDlgItem(IDOK);
	if(pWnd != NULL) {
		pWnd->GetWindowRect(&rect);
		int nWidth = rect.Width();
		int nHeight = rect.Height();
		rect.left = (cx-nWidth)/2;
		rect.top = cy - (nHeight + 7);
		rect.right = rect.left + nWidth;
		rect.bottom = rect.top + nHeight;
		pWnd->MoveWindow(rect);
	}

}

void CDlgOptionTTablePopup::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	if(GetSafeHwnd()!=NULL) {
		lpMMI->ptMinTrackSize.x = m_nDlgMinWidth;
		lpMMI->ptMinTrackSize.y = m_nDlgMinHeight;
	}

	CDialog::OnGetMinMaxInfo(lpMMI);
}

CString CDlgOptionTTablePopup::GetInstrumentID()
{
	return m_strInstrumentID;
}

void CDlgOptionTTablePopup::SetInstrumentID(CString& strInstrumentID)
{
	m_bUserChangeInstrumentID = false;
	m_strOldInstrumentID = m_strInstrumentID;
	m_strInstrumentID = strInstrumentID;
	m_bUserChangeInstrumentID = true;
	
	CString strTitle;
	std::map<std::string, std::set<std::string>> mapPrices;

	strTitle.Format("期权T型报价--%s", strInstrumentID);
	SetWindowText(strTitle);
	SubscribeRelateQuot(strInstrumentID);
	QueryRelateQuotSnapshot(strInstrumentID);
	
	LockGlobalObject();

	std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator it = 
			GetOptionTargetRelate().find(std::string(strInstrumentID));
	if(it != GetOptionTargetRelate().end())
		mapPrices = it->second;

	UnlockGlobalObject();

	InitTargetGrid(strInstrumentID);
	InitTTableGrid(mapPrices);
}

void CDlgOptionTTablePopup::InitTargetHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol)
{
	m_gridTarget.SetRowCount(1);
	m_gridTarget.SetFixedRowCount(1);

	int i=0;
	std::map<int, std::string>::iterator it;

	m_gridTarget.SetColumnCount(vecCol.size());
	for(i=0; i<(int)vecCol.size(); i++) {
		if((it = mapCols.find(vecCol[i])) != mapCols.end()) {
			m_gridTarget.SetItemText(0, i, it->second.c_str());
			m_gridTarget.SetItemData(0, i, it->first);
			m_gridTarget.SetColumnWidth(i, 72);
		}
	}
	int nCol = FindColIndex(m_gridTTable, QUOT_InstrumentID);
	if(nCol >= 0)
		m_gridTTable.SetColumnWidth(nCol, 88);
	m_gridTarget.Invalidate();
}

void CDlgOptionTTablePopup::InitTTableHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol)
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
	m_gridTTable.Invalidate();
}

void CDlgOptionTTablePopup::InitTargetGrid(CString& strInstrumentID)
{
	m_gridTarget.SetRowCount(2);
	int nItemCount = m_gridTarget.GetRowCount();
	int nColCount = m_gridTarget.GetColumnCount();

	for(int i=1; i<nItemCount; i++) {
		for(int k=0; k<nColCount; k++) {
			m_gridTarget.SetItemText(i, k, "");
		}
	}

	int nCol = FindColIndex(m_gridTarget, QUOT_InstrumentID);
	m_gridTarget.SetItemText(1, nCol, strInstrumentID);
	m_gridTarget.ResetScrollBars();
	m_gridTarget.Invalidate();
}

void CDlgOptionTTablePopup::InitTTableGrid(std::map<std::string, std::set<std::string>>& mapPrices)
{
	m_gridTTable.SetRowCount(mapPrices.size()+1);
	int nItemCount = m_gridTTable.GetRowCount();
	int nColCount = m_gridTTable.GetColumnCount();

	for(int i=1; i<nItemCount; i++) {
		for(int k=0; k<nColCount; k++) {
			m_gridTTable.SetItemText(i, k, "");
			m_gridTTable.SetItemBkColour(i, k, i%2 ? RGB(255, 255, 255) : RGB(235, 235, 235));
		}
	}

	std::map<std::string, std::set<std::string>>::iterator it;
	int nCol = FindColIndex(m_gridTTable, QUOT_OptionPrice);
	it = mapPrices.begin();
	for(int i=1; i<nItemCount && it != mapPrices.end(); i++, it++) {
		m_gridTTable.SetItemText(i, nCol, it->first.c_str());
		m_gridTTable.SetItemData(i, 0, atoi(it->first.c_str()));
	}
	m_gridTTable.ResetScrollBars();
	m_gridTTable.Invalidate();
}

int CDlgOptionTTablePopup::FindColIndex(CGridCtrl& grid, int nColID)
{
	for(int i=0; i<grid.GetColumnCount(); i++) {
		if(grid.GetItemData(0, i)==nColID)
			return i;
	}
	return -1;
}

int CDlgOptionTTablePopup::FindRowIndex(CGridCtrl& grid, int nRowID)
{
	for(int i=1; i<grid.GetRowCount(); i++) {
		if(grid.GetItemData(i, 0)==nRowID)
			return i;
	}
	return -1;
}

void CDlgOptionTTablePopup::UpdateGrid(CGridCtrl& grid, int nRow, PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
										double fPriceTick, BOOL bIsOption, BOOL bDirect)
{
	CString strText;
	int nHead = 0, nSymbol = 1, nCol = -1;
	DWORD dwColor = 0;
	const double fBasePrice = (util::isInvalidValue(quot.PreSettlementPrice) ? 
							quot.OpenPrice : quot.PreSettlementPrice);

	if(bIsOption && !bDirect) {
		nHead = QUOTNEWHEAD;
		//nSymbol = -1;
	}
	
	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_InstrumentID))!=-1) {
		grid.SetItemText(nRow, nCol, quot.InstrumentID);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LastPrice))!=-1) {
		strText.Format("%s", Price2String(quot.LastPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.LastPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_ChangeValue))!=-1) {
		double fSub = quot.LastPrice - fBasePrice;
		strText.Format("%s", Price2String(fSub, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = fSub < 0.0 ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}
	
	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_ChangePercentage))!=-1) {
		double fSub = (quot.LastPrice - fBasePrice)/fBasePrice*100.0;
		strText.Format("%0.2f", fSub);
		grid.SetItemText(nRow, nCol, strText);
		dwColor = fSub < 0.0 ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_BidPrice1))!=-1) {
		strText.Format("%s", Price2String(quot.BidPrice1, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.BidPrice1 < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_BidVolume1))!=-1) {
		strText.Format("%d", quot.BidVolume1);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_AskPrice1))!=-1) {
		strText.Format("%s", Price2String(quot.AskPrice1, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.AskPrice1 < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_AskVolume1))!=-1) {
		strText.Format("%d", quot.AskVolume1);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_OpenInterest))!=-1) {
		strText.Format("%0.0f", quot.OpenInterest);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_OpenPrice))!=-1) {
		strText.Format("%s", Price2String(quot.OpenPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.OpenPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_PreSettlementPrice))!=-1) {
		strText.Format("%s", Price2String(quot.PreSettlementPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		//dwColor = quot.AskPrice1 < fBasePrice ? GREEN : RGB(255, 0, 0);
		//grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LastVolume))!=-1) {
		int nNewVolume = 0;
		if(bHasOld)
			nNewVolume = quot.Volume - pQuotOld->Volume;
		strText.Format("%d", nNewVolume);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_HighestPrice))!=-1) {
		strText.Format("%s", Price2String(quot.HighestPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.HighestPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LowestPrice))!=-1) {
		strText.Format("%s", Price2String(quot.LowestPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.LowestPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_ClosePrice))!=-1) {
		strText.Format("%s", Price2String(quot.ClosePrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.ClosePrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_Volume))!=-1) {
		strText.Format("%d", quot.Volume);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_Turnover))!=-1) {
		strText.Format("%0.2f", quot.Turnover);
		grid.SetItemText(nRow, nCol, strText);
		//grid.SetItemFgColour(nRow, nCol, RGB(0, 0, 255));
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_UpperLimitPrice))!=-1) {
		strText.Format("%s", Price2String(quot.UpperLimitPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.UpperLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_LowerLimitPrice))!=-1) {
		strText.Format("%s", Price2String(quot.LowerLimitPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		dwColor = quot.LowerLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_SettlementPrice))!=-1) {
		strText.Format("%s", Price2String(quot.SettlementPrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		//dwColor = quot.LowerLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		//grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_PreClosePrice))!=-1) {
		strText.Format("%s", Price2String(quot.PreClosePrice, fPriceTick));
		grid.SetItemText(nRow, nCol, strText);
		//dwColor = quot.LowerLimitPrice < fBasePrice ? GREEN : RGB(255, 0, 0);
		//grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_PreOpenInterest))!=-1) {
		strText.Format("%0.0f", quot.PreOpenInterest);
		grid.SetItemText(nRow, nCol, strText);
		//dwColor = quot.PreOpenInterest < fBasePrice ? GREEN : RGB(255, 0, 0);
		//grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_UpdateTime))!=-1) {
		grid.SetItemText(nRow, nCol, quot.UpdateTime);
		//grid.SetItemFgColour(nRow, nCol, dwColor);
	}

	grid.Invalidate();
}

void CDlgOptionTTablePopup::UpdateTargetQuot(PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
										BOOL bDirect, double fPriceTick)
{
	int nRow = 1;
	UpdateGrid(m_gridTarget, nRow, quot, bHasOld, pQuotOld, fPriceTick, FALSE, bDirect);
}

void CDlgOptionTTablePopup::UpdateTTableQuot(PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
										CString& strPrice, BOOL bDirect, double fPriceTick)
{
	int nRow = FindRowIndex(m_gridTTable, atoi(strPrice));
	if(nRow!=-1)
		UpdateGrid(m_gridTTable, nRow, quot, bHasOld, pQuotOld, fPriceTick, TRUE, bDirect);
}

// 内部未加锁
void CDlgOptionTTablePopup::ResetCols()
{
	std::map<std::string, std::set<std::string>> mapPrices;
	std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator itPrice = 
			GetOptionTargetRelate().find(std::string(m_strInstrumentID));
	
	if(itPrice == GetOptionTargetRelate().end())
		return;

	mapPrices = itPrice->second;


	{
	std::map<int, std::string> mapCols = GetTargetColMap();
	std::vector<int> vecCol = GetTargetShowCol();
	InitTargetHead(mapCols, vecCol);
	}

	{
	std::map<int, std::string> mapCols = GetOptionColMap();
	std::vector<int> vecCol = GetOptionShowCol();
	InitTTableHead(mapCols, vecCol);
	}

	InitTargetGrid(m_strInstrumentID);
	InitTTableGrid(mapPrices);

	std::map<std::string, PlatformStru_DepthMarketData>::iterator it, itOld;
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator itInst;
	CString strExchangeID, strTargetID, strPrice;
	BOOL bIsOption = FALSE, bDirect = FALSE;
	double fPriceTick = 0.001;
	BOOL bHasOld = FALSE;
	PlatformStru_DepthMarketData oldQuot;

	for(it = m_mapQuot.begin(); it != m_mapQuot.end(); it++) {
		if((itInst = GetInstrumentInfos().find(it->first))==GetInstrumentInfos().end())
			continue;
		strExchangeID = itInst->second.ExchangeID;
		fPriceTick = itInst->second.PriceTick;
		bIsOption = itInst->second.ProductClass==THOST_FTDC_PC_Options;
		if(bIsOption) {
			ParseOptionFormat(CString(it->first.c_str()), strExchangeID, strTargetID, bDirect, strPrice);
		}

		itOld = m_mapOldQuot.find(it->first);
		if(itOld != m_mapOldQuot.end()) {
			bHasOld = TRUE;
			oldQuot = itOld->second;
		}
		else {
			bHasOld = FALSE;
		}
		if(!bIsOption) {
			UpdateTargetQuot(it->second, bHasOld, &oldQuot, bDirect, fPriceTick);
		}
		else {
			UpdateTTableQuot(it->second, bHasOld, &oldQuot, strPrice, bDirect, fPriceTick);
		}
	}
	m_gridTarget.Invalidate();
	m_gridTTable.Invalidate();
}

void CDlgOptionTTablePopup::ParseQuot(PlatformStru_DepthMarketData* pDepthData, BOOL bIsOption, 
					CString& strTargetID, BOOL bDirect, CString& strPrice, double fPriceTick)
{
	if(pDepthData==NULL)
		return;

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

	if(!bIsOption) {
		UpdateTargetQuot(*pDepthData, bHasOld, &oldQuot, bDirect, fPriceTick);
	}
	else {
		UpdateTTableQuot(*pDepthData, bHasOld, &oldQuot, strPrice, bDirect, fPriceTick);
	}
}

void CDlgOptionTTablePopup::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
}

void CDlgOptionTTablePopup::SetItem_CenterAlign(CGridCtrl& gridPrice)
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

void CDlgOptionTTablePopup::SubscribeRelateQuot(const CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {

		std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator it;
		std::map<std::string, std::set<std::string>>::iterator itPrice;
		std::set<std::string>::iterator itInst;
		std::map<std::string, std::set<std::string>> mapPrices;
		BOOL bFound = FALSE;

		LockGlobalObject();
		it = GetOptionTargetRelate().find(std::string(strInstrumentID));
		if(it != GetOptionTargetRelate().end()) {
			mapPrices = it->second;
			bFound = mapPrices.size()>0;
		}
		UnlockGlobalObject();
		
		char szInstrumentID[sizeof(InstrumentIDType)];

		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)-1);
		poPlusinApi->Send(CMDID_QUOTE_SUBSCRIBE, szInstrumentID, 
				sizeof(szInstrumentID), 0);

		for(itPrice = mapPrices.begin(); itPrice != mapPrices.end(); itPrice++) {
			for(itInst = itPrice->second.begin(); itInst != itPrice->second.end(); itInst++) {
				strncpy(szInstrumentID, (*itInst).c_str(), sizeof(szInstrumentID)-1);
				poPlusinApi->Send(CMDID_QUOTE_SUBSCRIBE, szInstrumentID, 
						sizeof(szInstrumentID), 0);
			}
		}
	}
}

void CDlgOptionTTablePopup::UnsubscribeRelateQuot(const CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {

		std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator it;
		std::map<std::string, std::set<std::string>>::iterator itPrice;
		std::set<std::string>::iterator itInst;
		std::map<std::string, std::set<std::string>> mapPrices;
		BOOL bFound = FALSE;

		LockGlobalObject();
		it = GetOptionTargetRelate().find(std::string(strInstrumentID));
		if(it != GetOptionTargetRelate().end()) {
			mapPrices = it->second;
			bFound = mapPrices.size()>0;
		}
		UnlockGlobalObject();
		
		char szInstrumentID[sizeof(InstrumentIDType)];

		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)-1);
		poPlusinApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szInstrumentID, 
				sizeof(szInstrumentID), 0);

		for(itPrice = mapPrices.begin(); itPrice != mapPrices.end(); itPrice++) {
			for(itInst = itPrice->second.begin(); itInst != itPrice->second.end(); itInst++) {
				strncpy(szInstrumentID, (*itInst).c_str(), sizeof(szInstrumentID)-1);
				poPlusinApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szInstrumentID, 
						sizeof(szInstrumentID), 0);
			}
		}
	}
}

void CDlgOptionTTablePopup::QueryRelateQuotSnapshot(const CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {

		std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator it;
		std::map<std::string, std::set<std::string>>::iterator itPrice;
		std::set<std::string>::iterator itInst;
		std::map<std::string, std::set<std::string>> mapPrices;
		BOOL bFound = FALSE;

		LockGlobalObject();
		it = GetOptionTargetRelate().find(std::string(strInstrumentID));
		if(it != GetOptionTargetRelate().end()) {
			mapPrices = it->second;
			bFound = mapPrices.size()>0;
		}
		UnlockGlobalObject();
		
		char szInstrumentID[sizeof(InstrumentIDType)];

		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)-1);
		poPlusinApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY, szInstrumentID, 
				sizeof(szInstrumentID), 0);

		for(itPrice = mapPrices.begin(); itPrice != mapPrices.end(); itPrice++) {
			for(itInst = itPrice->second.begin(); itInst != itPrice->second.end(); itInst++) {
				strncpy(szInstrumentID, (*itInst).c_str(), sizeof(szInstrumentID)-1);
				poPlusinApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY, szInstrumentID, 
						sizeof(szInstrumentID), 0);
			}
		}
	}
}

void CDlgOptionTTablePopup::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	UnsubscribeRelateQuot(GetInstrumentID());

	m_bExiting = true;

	//if(m_pdlgInstrumentIDList!=NULL)
	//{
	//	m_pdlgInstrumentIDList->ShowWindow(SW_HIDE);
	//}
	//if(m_pdlgQtyList!=NULL)
	//{
	//	m_pdlgQtyList->ShowWindow(SW_HIDE);
	//}

	::PostMessage(::AfxGetMainWnd()->GetSafeHwnd(), 
			GetTTableCloseMSGID(), 0, (LPARAM)this);

	//OnOK();
}

void CDlgOptionTTablePopup::OnTargetGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult)
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

	m_pCurrGrid = &m_gridTarget;

	*pResult = 0;
}

void CDlgOptionTTablePopup::OnOptionGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult)
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

	m_pCurrGrid = &m_gridTTable;

	*pResult = 0;
}

void CDlgOptionTTablePopup::OnTargetGrid_ColSetting()
{
	// TODO: Add your command handler code here
	CDlgSetOptionCols dlg(this);
	dlg.DoModal();
}

void CDlgOptionTTablePopup::OnTargetGrid_AutoSizeCols()
{
	// TODO: Add your command handler code here
	if(m_pCurrGrid==&m_gridTarget) {
		m_gridTarget.AutoSizeColumns();
		m_gridTarget.Refresh();
	}
	else if(m_pCurrGrid==&m_gridTTable) {
		m_gridTTable.AutoSizeColumns();
		m_gridTTable.Refresh();
	}
}
