#pragma once


#include "GridCtrl\GridCtrl.h"
#include <vector>
#include <map>
#include <string>


// CDlgOptionTTable dialog

class CDlgOptionTTable : public CDialog
{
	DECLARE_DYNAMIC(CDlgOptionTTable)
	static void SetItem_CenterAlign(CGridCtrl& gridPrice);

public:
	enum {	conTimerRepaint = 123457, conTimerResize = 123458 };

public:
	CDlgOptionTTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOptionTTable();

	void ResetInstrumentList(std::vector<std::string>& vecInstrumentID);

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

// Dialog Data
	enum { IDD = IDD_DLG_OPTIONT };

protected:

	// 内部未加锁
	void InitTargetHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol);
	// 内部未加锁
	void InitTTableHead(std::map<int, std::string>& mapCols, std::vector<int>& vecCol);
	// 内部未加锁
	void InitTargetGrid(const std::vector<std::string>& vecInstrID);
	// 内部未加锁
	void InitTTableGrid(const std::vector<std::string>& vecInstrID);
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

	void SubscribeQuot(const std::vector<std::string>& vecInstrID);
	void UnsubscribeQuot(const std::vector<std::string>& vecInstrID);
	void QueryQuotSnapshot(const std::vector<std::string>& vecInstrID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnTargetGrid_ColSetting();
	afx_msg void OnTargetGrid_AutoSizeCols();

	afx_msg void OnTargetGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionGrid_RClick(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnTargetGrid_DBClick(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionGrid_DBClick(NMHDR * pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

protected:
	HICON						m_hIcon;
	CGridCtrl					m_gridTarget;
	CGridCtrl					m_gridTTable;
	CGridCtrl*					m_pCurrGrid;
	CRect						m_rectMe;
	CFont						m_fontTitle;
	
	bool						m_bExiting;
	std::vector<std::string>	m_vecInstrID;
	std::vector<std::string>	m_vecTargetID;
	std::vector<std::string>	m_vecOptionID;
	std::map<std::string, int>	m_mapTargetID2RowID;
	std::map<int, std::string>	m_mapRowID2TargetID;
	std::map<std::string, int>	m_mapOptionID2RowID;
	std::map<int, std::string>	m_mapRowID2OptionID;

	std::map<std::string, PlatformStru_DepthMarketData>	m_mapQuot;
	std::map<std::string, PlatformStru_DepthMarketData>	m_mapOldQuot;

public:
};
