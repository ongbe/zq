#pragma once

#include "BKDialog.h"
#include "StaticEx.h"
#include "BKButton.h"
#include "KernelStruct.h"
#include "XListCtrl.h"


#include "..\inc\OptionQuation\OptionQuationInterface.h"

#ifdef _DEBUG
#pragma comment (lib,"..\\..\\Lib\\OptionQuationD.lib")
#else
#pragma comment (lib,"..\\..\\Lib\\OptionQuation.lib")
#endif


// 标的物行情高度
#define MARK_QUOT_HIGHT  53       
// 标的物行情标题栏高度
#define MARK_QUOT_TITLE_HIGHT  25   



// CT_PriceModuleDialog dialog

class CT_PriceModuleDialog : public CBKDialog
{
	DECLARE_DYNAMIC(CT_PriceModuleDialog)

public:
	CT_PriceModuleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CT_PriceModuleDialog();

// Dialog Data
	enum { IDD = IDD_T_PIRCE_DIALOG };
    enum { conCreateTPriceList = 123456 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    BOOL CreateTPriceList(int nMode);

private:
    CXListCtrl m_listQuot;
    //梯形报价窗口句柄
    HWND	m_pTPriceListPanel;
    HINSTANCE m_HInstance;
    CString m_strInstrument;


protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
    BOOL m_bInitDialog;
    LRESULT OnTransaprentBk( WPARAM wParam, LPARAM lParam);


    //双缓存相关
    CDC* m_pMemDC;
    CBitmap* m_pOldBitmap;
    CBitmap* m_pBitmapMem;
public:
    afx_msg void OnPaint();
private:
    HICON hIcon;

private:
    int   m_Iconleft;
    CRect m_rectSave;
public:
    afx_msg LRESULT QuotUpdateMsgHandler(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnBtnKLineClick(WPARAM, LPARAM);
    void InitListStrategy();
    void FillListStrategy(const PlatformStru_DepthMarketData& data);
    bool  GenerageOptionStrategyOrders(const string& strInstrumentID,bool bDirection,PlatformStru_OrderInfo& outOrder);
    COLORREF GetTextRGB(const PlatformStru_DepthMarketData& data, int nSubItem, bool& bUpdate);
    void UpdataColorMode(BK_COLOR_MODE eMode);
    PlatformStru_DepthMarketData m_data;//保存最后一条，变更背景色时使用
    void InitColumn();
    void InsertData(const PlatformStru_DepthMarketData& data);
    void AutoAdjustColumnWidth();
};
