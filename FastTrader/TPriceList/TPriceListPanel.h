#pragma once

#include "GridCtrl\GridCtrl.h"
#include <map>
#include <vector>
#include <string>
#include "KernelStruct.h"
#include "TPriceListDLL.h"

// CTPriceListPanel dialog

class CTPriceListPanel : public CDialog
{
	DECLARE_DYNAMIC(CTPriceListPanel)

public:
	CTPriceListPanel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTPriceListPanel();

// Dialog Data
	enum { IDD = IDD_DLG_TPRICELIST };

public:
	void LockObject() {
		EnterCriticalSection(&m_cs);
	};
	void UnLockObject() {
		LeaveCriticalSection(&m_cs);
	};

	void ResetInstrumentList(const vector<PlatformStru_InstrumentInfo>& vecOptions);
	void ParseQuot(const PlatformStru_DepthMarketData* pDepthData);
	bool ReadClickParam(int EventParamID, Stru_PriceListT_ClickParam& retParam);
	void ResetColorSuite(int colmode);
	void RedrawDlgItem();
	void DrawRect(CDC* pDc, CRect& rect, CPen& pen);

protected:
	void ResetCols(int colmode);
	void ResetCols(int colmode, DWORD dwFgColor, DWORD dwBkColor);
	// 内部未加锁
	void InitTTableHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol);
	// 内部未加锁
	void InitTTableGrid(const vector<PlatformStru_InstrumentInfo>& vecOptions);

	void UpdateTTableQuot(const PlatformStru_DepthMarketData& quot, BOOL bHasOld, 
			PlatformStru_DepthMarketData* pQuotOld);

	int  FindColIndex(CGridCtrl& grid, int nColID);
	int  FindColID(CGridCtrl& grid, int nColIndex);
	int  FindRowIndex(CGridCtrl& grid, int nRowID);
	void UpdateGrid(CGridCtrl& grid, int nRow, const PlatformStru_DepthMarketData& quot, 
							BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
							double fPriceTick, BOOL bDirect);
	void SetItem_CenterAlign(CGridCtrl& gridPrice);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnOptionGrid_Click(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionGrid_DBClick(NMHDR * pNMHDR, LRESULT* pResult);

	afx_msg void On_TargetGrid_ColSetting();

	DECLARE_MESSAGE_MAP()

protected:
	CRITICAL_SECTION	m_cs;
	CBitmap		m_bmpKLineView;
	CImageList	m_ImageList;
	CGridCtrl	m_gridTTable;
	CFont		m_fontTitle;
	CFont		m_fontTarget;
	CBrush		m_brushRed;
	CBrush		m_brushGreen;
	std::vector<PlatformStru_InstrumentInfo>	m_vecOptions;
	std::map<std::string, PlatformStru_InstrumentInfo>	m_mapInstrument;
	std::map<std::string, int>	m_mapPrice2RowID;
	std::map<int, std::map<bool, std::string>>	m_mapRowID2OptionID;

	std::map<std::string, PlatformStru_DepthMarketData>	m_mapQuot;
	std::map<std::string, PlatformStru_DepthMarketData>	m_mapOldQuot;

	int			m_nColMode;
	int			m_nLastEventID;
	std::map<int, Stru_PriceListT_ClickParam>	m_mapEvent;

};
