#pragma once

#include "GridCtrl\GridCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "BookOrderOperations.h"
#include "clsInstrumentIDListDialog.h"
#include "clsQtyListDialog.h"

#include "NCButton\NCButton.h"
#include "NCButton\NCButtonManager.h"
#include "ColorButton.h"

#include "CancelConfirmDialog.h"
#include "afxext.h"

// CBookOrderDialog dialog

class CBookOrderDialog : public CBookOrderOperations
{
public:

	DECLARE_DYNAMIC(CBookOrderDialog)

public:
	CBookOrderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBookOrderDialog();

	CString GetInstrumentID() {
		char strText[64];
		ZeroMemory(strText, sizeof(strText));
		m_EditInstrumentID.GetLine(0, strText, 63);
		return strText;
	};
	void SetInstrumentID(CString& strInstrumentID) {
		m_EditInstrumentID.SetSel(0, -1);
		m_bUserChangeInstrumentID = false;
		m_EditInstrumentID.ReplaceSel(strInstrumentID);
		m_bUserChangeInstrumentID = true;
		SetTilte(CString("Ò»¼üÏÂµ¥ -- "+strInstrumentID));
		QueryInstrumentInfo(strInstrumentID);
		SubscribeQuot(strInstrumentID);
		QueryQuotSnapshot(strInstrumentID);
		m_strOldInstrumentID = strInstrumentID;
	};
	void SetTilte(CString& strTitle) 
	{
		CWnd* pItem = GetDlgItem(IDC_LABEL_TITLE);
		if(pItem!=NULL)
			pItem->SetWindowText(strTitle);
		SetWindowText(strTitle);
	};
	CString SetTilte() 
	{
		CString strTitle;
		CWnd* pItem = GetDlgItem(IDC_LABEL_TITLE);
		if(pItem!=NULL)
			pItem->GetWindowText(strTitle);
		return strTitle;
	};
	void ResetViewMode(int nViewMode);

	void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ResetDlgSize(int cx, int cy);
	void RedrawDlgItem();
	void DrawRect(CDC* pDc, CRect& rect, CPen& pen);

// Dialog Data
	enum { IDD = IDD_BOOKORDER };

protected:

	void ParseCmd_Base(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Config(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ShowCurrAccountInfo();
	void ShowPositionInfos();
	void InitPriceGrid(BOOL bFixedPrice, BOOL bNeedColorGradient=FALSE);
	void ShowPriceGrid(BOOL bFixedPrice, BOOL bReflash=TRUE);
	void ShowPriceGrid_GradientColor();
	void ShowLastPrice2GridCenter();
	void ShowPriceGridCondVolume();
	//void ShowPriceGridPositionVolumeToAvgPrice();
	void ShowTradeVolume();
	void ShowPriceGridSumRow();
	void SetItem_AskBid(CGridCtrl& gridPrice, 
			double fPrice, int nVolume, double fPriceTick, 
			double fTopPrice, BOOL bIsAsk);
	void SetItem_WaitVolume(CGridCtrl& gridPrice, 
			double fPrice, int nVolume, double fPriceTick, 
			double fTopPrice, BOOL bIsAsk);
	void ShowOrderWaitVolume(double fPrice, BIDASKVOLUME& tVol);
	void ResetDlgColor(int nViewMode, DWORD dwBackColor, DWORD dwButtonBackColor, 
			DWORD dwEditBackColor);
	void ResetAccountGridColor(ROWCOLOR& rowColor);
	void ResetPositionGridColor(ROWCOLOR& rowColor);
	void ResetPriceGridColor(PRICEGRIDCOLOR& rowColor);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	afx_msg void OnBnClickedCheckFixprice();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedReflashprice();

    afx_msg void OnGridLeftClick(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridRightClick(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridLeftDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnDeltaposSpinQtyLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinQtyRight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTextLeftClicked_InstrmentID();
	afx_msg void OnTextLeftClicked_QtyLeft();
	afx_msg void OnTextKeyDown_InstrmentID(UINT nKeyNum);
	afx_msg void OnEnChangeEditInstrumentid();
	afx_msg void OnBnClickedCheckTomosttop();
	afx_msg void OnBnClickedCheckAllowOrder();

	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrl	m_gridPrice;
	CGridCtrl	m_gridAccountInfo;
	CGridCtrl	m_gridPositionInfo;
	CImageList	m_ImageList;

	CEdit			m_EditInstrumentID;
	CEdit			m_editQtyLeft;
	CBitmapButton	m_CheckAllowOrder;
	CBitmapButton	m_CheckToMostTop;
	CSpinButtonCtrl m_SpinQtyLeft;
	CBitmapButton	m_btnClose;
	CBitmapButton	m_btnSetting;

	CInstrumentIDListDialog*	m_pdlgInstrumentIDList;
	CQtyListDialog*				m_pdlgQtyList;
	CCancelConfirmDialog*		m_pdlgCancelConfirm;

	CBrush*				m_pDlgBrushBk;
	CBrush*				m_pButtonBrushBk;
	CBrush*				m_pEditBrushBk;
	CBrush				m_brushNull;
	CBrush				m_brushWhite;
	CFont				m_fontTitle;

	int					m_nViewMode;
	BOOL				m_bDlgTopMost;
	BOOL				m_bHasOrder;
	BOOL				m_bHasFixPriceGrid;

	BOOL				m_bCancelAllOrder;

	BOOL				m_bHasLocalSetting;
	ORDERSETTING		m_tOrderSetting;
	int					m_nPositionCellInitWidth[conPositionColCount];
	int					m_nPriceCellInitWidth[conPriceColCount];

protected:
	HICON m_hIcon;

public:
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnMenuPlhAutofitcolwidth();
	afx_msg void OnMenuPlhCancelallorder();
	afx_msg void OnMenuPlhCancelallcond();
	afx_msg void OnMenuPlhBatchcancelorder();
	afx_msg void OnMenuPlhBatchcancelcond();
};
