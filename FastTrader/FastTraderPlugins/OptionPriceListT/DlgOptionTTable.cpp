// DlgOptionTTable.cpp : implementation file
//

#include "stdafx.h"
#include "OptionPriceListT.h"
#include "DlgOptionTTable.h"

#include "DlgSetOptionCols.h"
#include <set>


// CDlgOptionTTable dialog

IMPLEMENT_DYNAMIC(CDlgOptionTTable, CDialog)

CDlgOptionTTable::CDlgOptionTTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionTTable::IDD, pParent)
{
	m_bExiting = false;
	m_pCurrGrid = NULL;
	ZeroMemory(&m_rectMe, sizeof(m_rectMe));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgOptionTTable::~CDlgOptionTTable()
{
}

void CDlgOptionTTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDINSTRUMENT, m_gridTarget);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_GRIDTTABLE, m_gridTTable);
}

//
BEGIN_MESSAGE_MAP(CDlgOptionTTable, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDOK, &CDlgOptionTTable::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgOptionTTable::OnBnClickedCancel)
	ON_COMMAND(ID_TARGETGRID_COLSETTING, &CDlgOptionTTable::OnTargetGrid_ColSetting)
	ON_COMMAND(ID_TARGETGRID_AUTOSIZECOLS, &CDlgOptionTTable::OnTargetGrid_AutoSizeCols)
	ON_NOTIFY(NM_RCLICK, IDC_GRIDINSTRUMENT, &CDlgOptionTTable::OnTargetGrid_RClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRIDTTABLE, &CDlgOptionTTable::OnOptionGrid_RClick)
	ON_NOTIFY(NM_DBLCLK, IDC_GRIDINSTRUMENT, &CDlgOptionTTable::OnTargetGrid_DBClick)
	ON_NOTIFY(NM_DBLCLK, IDC_GRIDTTABLE, &CDlgOptionTTable::OnOptionGrid_DBClick)
END_MESSAGE_MAP()


// CDlgOptionTTable message handlers

LRESULT CDlgOptionTTable::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgOptionTTable::OnInitDialog()
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

	SetTimer(conTimerResize, 20, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptionTTable::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(conTimerResize);
}

void CDlgOptionTTable::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//PostMessage(WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), NULL);
	
	//CDialog::OnClose();
}

void CDlgOptionTTable::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(GetSafeHwnd()==NULL || m_gridTarget.GetSafeHwnd()==NULL || 
			m_gridTTable.GetSafeHwnd()==NULL)
		return;

	CRect rect;

	m_gridTarget.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right = cx;// - 7;
	//m_gridTarget.MoveWindow(rect);
	m_gridTarget.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOMOVE);
	m_gridTarget.ResetScrollBars();
	//if(m_gridTarget.GetColumnCount()>0) {
	//	int nColWidth = m_gridTarget.GetColumnWidth(0);
	//	m_gridTarget.SetColumnWidth(0, 0);
	//	m_gridTarget.SetColumnWidth(0, nColWidth);
	//}
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
	rect.right = cx;// - 7;
	rect.bottom = cy;// - 37;
	//m_gridTTable.MoveWindow(rect);
	m_gridTTable.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOMOVE);
	m_gridTTable.ResetScrollBars();
	//if(m_gridTTable.GetColumnCount()>0) {
	//	int nColWidth = m_gridTTable.GetColumnWidth(0);
	//	m_gridTTable.SetColumnWidth(0, 0);
	//	m_gridTTable.Refresh();
	//	m_gridTTable.SetColumnWidth(0, nColWidth);
	//}
	m_gridTTable.Refresh();
	{
	int nMinPos = 0, nMaxPos = 0;
	CRect rect;
	m_gridTTable.GetWindowRect(&rect);
	m_gridTTable.GetScrollRange(SB_HORZ, &nMinPos, &nMaxPos);
	m_gridTTable.SetScrollPos32(SB_HORZ, (nMaxPos-nMinPos-rect.Width())/2);
	}

	//CWnd* pWnd = GetDlgItem(IDOK);
	//if(pWnd != NULL) {
	//	pWnd->GetWindowRect(&rect);
	//	int nWidth = rect.Width();
	//	int nHeight = rect.Height();
	//	rect.left = (cx-nWidth)/2;
	//	rect.top = cy - (nHeight + 7);
	//	rect.right = rect.left + nWidth;
	//	rect.bottom = rect.top + nHeight;
	//	pWnd->MoveWindow(rect);
	//}

}

void CDlgOptionTTable::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	//if(GetSafeHwnd()!=NULL) {
	//	lpMMI->ptMinTrackSize.x = m_nDlgMinWidth;
	//	lpMMI->ptMinTrackSize.y = m_nDlgMinHeight;
	//}

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CDlgOptionTTable::ResetInstrumentList(std::vector<std::string>& vecInstrumentID)
{

	UnsubscribeQuot(m_vecInstrID);

	std::vector<std::string> vecStdInstr;
	std::vector<std::string> vecOptionInstr;
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;

	m_vecInstrID = vecInstrumentID;

	LockGlobalObject();

	for(int i=0; i<(int)vecInstrumentID.size(); i++) {
		it = GetInstrumentInfos().find(vecInstrumentID[i]);
		if(it != GetInstrumentInfos().end()) {
			if(it->second.ProductClass==THOST_FTDC_PC_Options) {
				vecOptionInstr.push_back(vecInstrumentID[i]);
			}
			else {
				vecStdInstr.push_back(vecInstrumentID[i]);
			}
		}
	}

	UnlockGlobalObject();

	m_vecTargetID = vecStdInstr;
	m_vecOptionID = vecOptionInstr;

	InitTargetGrid(vecStdInstr);
	InitTTableGrid(vecOptionInstr);
	SubscribeQuot(vecInstrumentID);
	QueryQuotSnapshot(vecInstrumentID);
}

void CDlgOptionTTable::InitTargetHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol)
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
	m_gridTarget.Refresh();
}

void CDlgOptionTTable::InitTTableHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol)
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
void CDlgOptionTTable::InitTargetGrid(const std::vector<std::string>& vecInstrID)
{
	m_gridTarget.SetRowCount(1+vecInstrID.size());
	int nItemCount = m_gridTarget.GetRowCount();
	int nColCount = m_gridTarget.GetColumnCount();

	m_mapTargetID2RowID.clear();
	m_mapRowID2TargetID.clear();

	for(int i=1; i<nItemCount; i++) {
		for(int k=0; k<nColCount; k++) {
			m_gridTarget.SetItemText(i, k, "");
			m_gridTarget.SetItemBkColour(i, k, i%2 ? RGB(255, 255, 255) : RGB(235, 235, 235));
		}
	}
	int nCol = FindColIndex(m_gridTarget, QUOT_InstrumentID);
	if(nCol >= 0) {
		for(int i=1; i<nItemCount; i++) {
			m_gridTarget.SetItemText(i, nCol, vecInstrID[i-1].c_str());
			m_gridTarget.SetItemData(i, 0, i);
			m_mapTargetID2RowID.insert(std::make_pair(vecInstrID[i-1], i));
			m_mapRowID2TargetID.insert(std::make_pair(i, vecInstrID[i-1]));
		}
	}
	m_gridTarget.ResetScrollBars();
	m_gridTarget.Refresh();
}

// 内部未加锁
void CDlgOptionTTable::InitTTableGrid(const std::vector<std::string>& vecInstrID)
{
	std::set<std::string>::iterator it;
	std::set<std::string> setNewID;
	std::vector<std::string> vecNewOptionID;
	CString strText, strExchangeID;
	std::map<std::string, PlatformStru_InstrumentInfo> mapInstrInfo;
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator itInstr;
	
	mapInstrInfo = GetInstrumentInfos();
	for(int i=0; i<(int)vecInstrID.size(); i++) {
		strExchangeID = "";
		if((itInstr = mapInstrInfo.find(vecInstrID[i])) != mapInstrInfo.end())
			strExchangeID = itInstr->second.ExchangeID;
		if(ConvOptionFormat(CString(vecInstrID[i].c_str()), strExchangeID, strText)) {
			if(setNewID.find((LPSTR)(LPCSTR)strText)==setNewID.end()) {
				setNewID.insert((LPSTR)(LPCSTR)strText);
				vecNewOptionID.push_back((LPSTR)(LPCSTR)strText);
			}
		}
	}

	m_mapOptionID2RowID.clear();
	m_mapRowID2OptionID.clear();

	m_gridTTable.SetRowCount(1+vecNewOptionID.size());
	int nItemCount = m_gridTTable.GetRowCount();
	int nColCount = m_gridTTable.GetColumnCount();

	for(int i=1; i<nItemCount; i++) {
		for(int k=0; k<nColCount; k++) {
			m_gridTTable.SetItemText(i, k, "");
			m_gridTTable.SetItemBkColour(i, k, i%2 ? RGB(255, 255, 255) : RGB(235, 235, 235));
		}
	}
	int nCol = FindColIndex(m_gridTTable, QUOT_OptionPrice);
	if(nCol >= 0) {
		for(int i=1; i<nItemCount; i++) {
			m_gridTTable.SetItemText(i, nCol, vecNewOptionID[i-1].c_str());
			m_gridTTable.SetItemData(i, 0, i);
			m_mapOptionID2RowID.insert(std::make_pair(vecNewOptionID[i-1], i));
			m_mapRowID2OptionID.insert(std::make_pair(i, vecNewOptionID[i-1]));
		}
	}
	m_gridTTable.ResetScrollBars();
	m_gridTTable.Refresh();
}

int CDlgOptionTTable::FindColIndex(CGridCtrl& grid, int nColID)
{
	for(int i=0; i<grid.GetColumnCount(); i++) {
		if(grid.GetItemData(0, i)==nColID)
			return i;
	}
	return -1;
}

int CDlgOptionTTable::FindRowIndex(CGridCtrl& grid, int nRowID)
{
	for(int i=1; i<grid.GetRowCount(); i++) {
		if(grid.GetItemData(i, 0)==nRowID)
			return i;
	}
	return -1;
}

void CDlgOptionTTable::UpdateGrid(CGridCtrl& grid, int nRow, PlatformStru_DepthMarketData& quot, 
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
	
	//if((nCol=FindColIndex(grid, nHead+nSymbol*QUOT_InstrumentID))!=-1) {
	//	grid.SetItemText(nRow, nCol, quot.InstrumentID);
	//}

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

	grid.Refresh();
}

void CDlgOptionTTable::UpdateTargetQuot(PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
										BOOL bDirect, double fPriceTick)
{
	int nRow = -1;
	std::map<std::string, int>::iterator it = m_mapTargetID2RowID.find(quot.InstrumentID);
	if(it != m_mapTargetID2RowID.end()) {
		CGridCtrl::CLockObject lock(&m_gridTarget.m_csLock);
		nRow = FindRowIndex(m_gridTarget, it->second);
		if(nRow!=-1)
			UpdateGrid(m_gridTarget, nRow, quot, bHasOld, pQuotOld, fPriceTick, FALSE, bDirect);
	}
}

void CDlgOptionTTable::UpdateTTableQuot(PlatformStru_DepthMarketData& quot, 
										BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
										CString& strPrice, BOOL bDirect, double fPriceTick)
{
	int nRow = -1;
	CString strText, strExchangeID;
	std::map<std::string, PlatformStru_InstrumentInfo> mapInstrInfo;
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator itInstr;
	
	mapInstrInfo = GetInstrumentInfos();
	if((itInstr = mapInstrInfo.find(quot.InstrumentID)) != mapInstrInfo.end())
		strExchangeID = itInstr->second.ExchangeID;
	if(!ConvOptionFormat(CString(quot.InstrumentID), strExchangeID, strText))
		return;
	
	std::map<std::string, int>::iterator it = m_mapOptionID2RowID.find((LPSTR)(LPCSTR)strText);
	if(it != m_mapOptionID2RowID.end()) {
		CGridCtrl::CLockObject lock(&m_gridTTable.m_csLock);
		nRow = FindRowIndex(m_gridTTable, it->second);
		if(nRow!=-1)
			UpdateGrid(m_gridTTable, nRow, quot, bHasOld, pQuotOld, fPriceTick, TRUE, bDirect);
	}
}

// 内部未加锁
void CDlgOptionTTable::ResetCols()
{
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

	InitTargetGrid(m_vecTargetID);
	InitTTableGrid(m_vecOptionID);

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
	m_gridTarget.Refresh();
	m_gridTTable.Refresh();
}

void CDlgOptionTTable::ParseQuot(PlatformStru_DepthMarketData* pDepthData, BOOL bIsOption, 
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

void CDlgOptionTTable::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
}

void CDlgOptionTTable::SetItem_CenterAlign(CGridCtrl& gridPrice)
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

void CDlgOptionTTable::SubscribeQuot(const std::vector<std::string>& vecInstrID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {
		
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));

		for(int i = 0; i<(int)vecInstrID.size(); i++) {
			strncpy(szInstrumentID, vecInstrID[i].c_str(), sizeof(szInstrumentID)-1);
			poPlusinApi->Send(CMDID_QUOTE_SUBSCRIBE, szInstrumentID, 
					sizeof(szInstrumentID), 0);
		}
	}
}

void CDlgOptionTTable::UnsubscribeQuot(const std::vector<std::string>& vecInstrID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {
		
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));

		for(int i = 0; i<(int)vecInstrID.size(); i++) {
			strncpy(szInstrumentID, vecInstrID[i].c_str(), sizeof(szInstrumentID)-1);
			poPlusinApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szInstrumentID, 
					sizeof(szInstrumentID), 0);
		}
	}
}

void CDlgOptionTTable::QueryQuotSnapshot(const std::vector<std::string>& vecInstrID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {

		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));

		for(int i = 0; i<(int)vecInstrID.size(); i++) {
			strncpy(szInstrumentID, vecInstrID[i].c_str(), sizeof(szInstrumentID)-1);
			poPlusinApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY, szInstrumentID, 
					sizeof(szInstrumentID), 0);
		}
	}
}

void CDlgOptionTTable::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//PlusinAPI* poPlusinApi = GetPlusinAPI();
	//if(poPlusinApi!=NULL) {
	//	poPlusinApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, 0);
	//}

	//OnOK();
}

void CDlgOptionTTable::OnBnClickedCancel()
{
	//PlusinAPI* poPlusinApi = GetPlusinAPI();
	//if(poPlusinApi!=NULL) {
	//	poPlusinApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, 0);
	//}

	//OnCancel();
}

void CDlgOptionTTable::OnTargetGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult)
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

void CDlgOptionTTable::OnOptionGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult)
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

void CDlgOptionTTable::OnTargetGrid_DBClick(NMHDR * pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGrid = (NM_GRIDVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
   
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {
		poPlusinApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, 0);
	}

	*pResult = 0;
}

void CDlgOptionTTable::OnOptionGrid_DBClick(NMHDR * pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pNMGrid = (NM_GRIDVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
   
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {
		poPlusinApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, 0);
	}

	*pResult = 0;
}

void CDlgOptionTTable::OnTargetGrid_ColSetting()
{
	// TODO: Add your command handler code here
	CDlgSetOptionCols dlg(this);
	dlg.DoModal();
}

void CDlgOptionTTable::OnTargetGrid_AutoSizeCols()
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

void CDlgOptionTTable::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==conTimerResize) {
		CRect rectCur;
		GetWindowRect(&rectCur);
		if(!m_rectMe.EqualRect(&rectCur)) {
			if(GetSafeHwnd()==NULL || m_gridTarget.GetSafeHwnd()==NULL 
					|| m_gridTTable.GetSafeHwnd()==NULL)
				return;

			//m_gridTarget.PostMessage(WM_MOVE, 0, MAKELONG(0, -1));
			//m_gridTTable.PostMessage(WM_MOVE, 0, MAKELONG(0, -1));
			//m_gridTarget.PostMessage(WM_MOVE, 0, MAKELONG(0, 0));
			//m_gridTTable.PostMessage(WM_MOVE, 0, MAKELONG(0, 0));
			CRect rect;
			m_gridTarget.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_gridTarget.SetWindowPos(NULL, rect.left, rect.top-1, rect.Width(), rect.Height(), SWP_NOZORDER);
			m_gridTarget.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
			m_gridTTable.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_gridTTable.SetWindowPos(NULL, rect.left, rect.top-1, rect.Width(), rect.Height(), SWP_NOZORDER);
			m_gridTTable.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
			//m_dlgOptionTTable.SetWindowPos(NULL, -1, -1, rect.Width(), rect.Height(), SWP_NOZORDER);
			//m_dlgOptionTTable.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
			m_rectMe = rectCur;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

