// OptionStrategyDlg.h : header file
//

#pragma once

#include "KernelStruct.h"
#include "PlusinAPI.h"
#include "CommPublic.h"

#include "SplitterControl.h"
#include "afxwin.h"
#include "StaticEx.h"
#include "T_PriceModuleDialog.h"
#include "QuickOrderModuleDialog.h"
#include "AnalyseModuleDialog.h"
#include "BKColor.h"
#include "BKDialog.h"
#include "BKButton.h"

#include <map>
#include <set>
#include <string>
using namespace std;

#define STD_GAP             5
#define SPLIT_WIDTH         4
#define STD_BUTTON_WIDTH    90
#define STD_TITLE_HEIGHT    55



// COptionStrategyDlg dialog
class COptionStrategyDlg : public CBKDialog
{
    // Construction
public:
    COptionStrategyDlg(int nWidth, int nHight, float fSplitterH, float fSplitterV, CWnd* pParent = NULL);	// standard constructor
    ~COptionStrategyDlg();

    // Dialog Data
    enum { IDD = IDD_OPTIONSTRATEGY_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:        

    string m_szCurrInstrumentID;//FastTrader 传来的合约
    static COptionStrategyDlg* m_pMainDlg;
    static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

    void UpdataColorMode(BK_COLOR_MODE eMode);
    void UpdateCombBoxCtrl();
    void UpdateLineChartCtrl();
    void UpdateXListCtrl(const PlatformStru_OrderInfo& outOrder);

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon(); 
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSplitterVDelta(NMHDR* pNMHDR, LRESULT* pResult); 
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnCbnSelchangeComboExchange();
    afx_msg void OnCbnSelchangeComboStyle();
    afx_msg void OnCbnSelchangeComboInstrument();
    afx_msg LRESULT QuotUpdateTimeSetMsgHandler(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT VolatilityReSetMsgHandler(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

private:
    void Resize();
    void MoveDlgItem(int nD, const CRect& rcPos, BOOL bRepaint); 

    LRESULT OnTransaprentBk( WPARAM wParam, LPARAM lParam);
    void BuildBkDC();
    void CreateSplitterWnd(void);

private:   

    //背景相关
    CDC* m_pMemDC;
    CBitmap* m_pOldBitmap;
    CBitmap* m_pBitmapMem;

    // 梯形报价窗口
    CT_PriceModuleDialog m_T_PriceModuleDialog;
    // 一键下单窗口
    CQuickOrderModuleDialog m_QuickOrderModuleDialog;
    // 分析策略模块窗口
    CAnalyseModuleDialog m_AnalyseModuleDialog;
    //分割窗口
    CSplitterControl    m_wndSplitterH;
    CSplitterControl    m_wndSplitterV;
    // 垂直分割条相对于父窗口的横坐标比例
    float m_SplitterVRate;
    // 水平分割条相对于父窗口的纵坐标比例
    float m_SplitterHRate;
    // 标题名称
    CStaticEx m_staticTitle;

    CComboBox m_combExchange;
    CComboBox m_combProduct;
    CComboBox m_combInstrument;

    CBKButton m_btnMin;
    CBKButton m_btnMax;
    CBKButton m_btnClose;

    //分辨率
    int m_nResolutionWidth;
    int m_nResolutionHight;



public:
    void SetSubscribedInstInfo(const PlatformStru_InstrumentInfo& sInstInfo);
    void DisplayWindow( BOOL bShow );

public:

    //报单相关
    void ReqPosition(const PositionKey& key);

    void NewOrder(vector<PlatformStru_OrderInfo>& vecOrders);
    BOOL MakeReq_Order(PlatformStru_InputOrder& req, string& strInstrumentID, string& strAccount, int nOrderType, 
        char cDirection, char cOffsetFlag, int nVolume, double fPrice, double fTriggerPrice, 
        BOOL bIsHedge,BOOL bIsMarketPrice, BOOL bIsReplaceOrder);

    void TradeInfoReach(PlatformStru_TradeInfo& TradeInfo);
    void OnOrderRtn(PlatformStru_OrderInfo& OrderInfo);

private:
    //先隐藏主窗口，待查询后合约，并初始化后才显示
    bool m_bVisible; 
    CString m_strNote;
public:
    afx_msg void OnBnClickedButtonMin();
    afx_msg void OnBnClickedButtonMax();
};
