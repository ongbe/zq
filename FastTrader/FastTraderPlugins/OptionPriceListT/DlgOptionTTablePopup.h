#pragma once

#include "GridCtrl\GridCtrl.h"

// CDlgOptionTTablePopup dialog

class CDlgOptionTTablePopup : public CDialog
{
	DECLARE_DYNAMIC(CDlgOptionTTablePopup)
	static void SetItem_CenterAlign(CGridCtrl& gridPrice);

public:
	enum {	conTimerRepaint = 123457, conTimerResize = 123458 };

public:
	CDlgOptionTTablePopup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOptionTTablePopup();

	CString GetInstrumentID();
	void SetInstrumentID(CString& strInstrumentID);

	bool IsExiting() {
		return m_bExiting;
	};

	// 内部未加锁
	void ResetCols();
	void ParseQuot(PlatformStru_DepthMarketData* pDepthData, BOOL bIsOption, 
					CString& strTargetID, BOOL bDirect, CString& strPrice, double fPriceTick);
	void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ResetDlgSize(int cx, int cy);
	void RedrawDlgItem();
	void DrawRect(CDC* pDc, CRect& rect, CPen& pen);
	void ResetSize(int cx, int cy);

// Dialog Data
	enum { IDD = IDD_DLG_OPTIONTPOPUP };

protected:

	// 内部未加锁
	void InitTargetHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol);
	// 内部未加锁
	void InitTTableHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol);
	// 内部未加锁
	void InitTargetGrid(CString& strInstrumentID);
	// 内部未加锁
	void InitTTableGrid(std::map<std::string, std::set<std::string>>& mapPrices);
	void UpdateTargetQuot(PlatformStru_DepthMarketData& quot, BOOL bHasOld, 
			PlatformStru_DepthMarketData* pQuotOld, BOOL bDirect, double fPriceTick);
	void UpdateTTableQuot(PlatformStru_DepthMarketData& quot, BOOL bHasOld, 
			PlatformStru_DepthMarketData* pQuotOld, CString& strPrice, BOOL bDirect, double fPriceTick);

	int  FindColIndex(CGridCtrl& grid, int nColID);
	int  FindRowIndex(CGridCtrl& grid, int nRowID);
	void UpdateGrid(CGridCtrl& grid, int nRow, PlatformStru_DepthMarketData& quot, 
							BOOL bHasOld, PlatformStru_DepthMarketData* pQuotOld, 
							double fPriceTick, BOOL bIsOption, BOOL bDirect);

	void SubscribeRelateQuot(const CString& strInstrumentID);
	void UnsubscribeRelateQuot(const CString& strInstrumentID);
	void QueryRelateQuotSnapshot(const CString& strInstrumentID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();

	afx_msg void OnTargetGrid_ColSetting();
	afx_msg void OnTargetGrid_AutoSizeCols();

	afx_msg void OnTargetGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	HICON		m_hIcon;
	CGridCtrl	m_gridTarget;
	CGridCtrl	m_gridTTable;
	CGridCtrl*	m_pCurrGrid;
	CFont		m_fontTitle;
	
	bool		m_bExiting;
	bool		m_bUserChangeInstrumentID;
	CString		m_strInstrumentID;
	CString		m_strOldInstrumentID;
	int			m_nDlgMinWidth;
	int			m_nDlgMinHeight;

	std::map<std::string, int>							m_mapTarget2Row;
	std::map<std::string, int>							m_mapPrice2Row;
	std::map<std::string, PlatformStru_DepthMarketData>	m_mapQuot;
	std::map<std::string, PlatformStru_DepthMarketData>	m_mapOldQuot;
};
