#pragma once


#include "afxcmn.h"
#include    <vector>
#include "afxwin.h"
#include "KernelStruct.h"
#include "XListCtrl.h"
#include "ThumbNailControl.h"
#include "BKDialog.h"
#include "BKButton.h"
#include "CommPublic.h"
#include "OrderConfirmDlg.h"

//THUMBNAILWINDOW_HEIGHT value must bigger than THUMBNAIL_WIDTH or THUMBNAIL_HEIGHT
#define	THUMBNAIL_WIDTH		35
#define	THUMBNAIL_HEIGHT	35
#define THUMBNAILWINDOW_HEIGHT  40

// CQuickOrderModuleDialog dialog

enum eHeaderColumn{ 
                    ColumnDelete,
                    ColumnCheck,
                    ColumnExecutivePrice,
                    ColumnPremiumStyle,
                    ColumnOrderCount,
                    ColumnOrderPosion,
                    ColumnConditionPrice,
                    ColumnOrderPrice,
                    ColumnPosionAveragePrice,
                    ColumnDirection,
                    ColumnOffset,
                    ColumnValidateDate,
                    ColumnHideWaveRate,
                    ColumnDelta,
                    ColumnGamma,
                    ColumnTheta,
                    ColumnVega
                    };
class CQuickOrderModuleDialog : public CBKDialog
{
	DECLARE_DYNAMIC(CQuickOrderModuleDialog)

public:
	CQuickOrderModuleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQuickOrderModuleDialog();

// Dialog Data
	enum { IDD = IDD_QUICKORDER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();

private:
    void InitListStrategy();
    void InitListStrategyData();
    void FillListStrategy(const vector<PlatformStru_OrderInfo>&  outOrders,bool bAdd=false);

private:
    CXListCtrl m_listStrategy;
    CStringArray m_arrayStrikePrice, m_arrayOptionStyle, m_arrayPriceCondition, m_arrayDate, m_arrayDirect, m_arrayOffset;

private:
    BOOL m_bInitDialog;
    // 策略缩略图窗口
    CThumbNailControl m_cThumbFrame;
    COrderConfirmDlg  m_OrderConfirmDlg;
    CBKButton m_btnOrder;
public:
    afx_msg void OnBnClickedButtonOrder();  
    afx_msg LRESULT OnStrategyBtnLClicked( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnComboSelection(WPARAM, LPARAM);
    afx_msg LRESULT OnCheckbox(WPARAM, LPARAM);
    afx_msg LRESULT OnDeleteImgClicked(WPARAM, LPARAM);
    afx_msg LRESULT OnEditEnd(WPARAM, LPARAM);
    afx_msg LRESULT OnVolatilityUpdate(WPARAM,LPARAM);
    afx_msg LRESULT OnPositionUpdate(WPARAM,LPARAM);
    

private:
    PlatformStru_InstrumentInfo m_sSubscribeInstInfo;//标的物合约
    set<PlatformStru_OrderInfo*> m_setOrderInfo;//XListCtrl绑定用的
public:
    vector<PlatformStru_InstrumentInfo>         m_vecOptionIns;     //当前所有期权期货合约
    map<string,PlatformStru_DepthMarketData>    m_maplastQuot;      //当前所有期权期货的行情
    vector<PlatformStru_OrderInfo>              m_vecOutOrders;     //策略报单

    double  GetStrikePrice(string sInstID);
    char    GetOrderStyle(string sInstID);
public:
    void SetSubscribedInst(const PlatformStru_InstrumentInfo& sInstInfo);//来自主窗口
    void UpDataLineChart(void);
    void UpdateXListCtrl(const PlatformStru_OrderInfo& outOrder);

    CStringArray m_sa1,m_sa2,m_sa3,m_sa4,m_sa5;
    void FillListStrategy2(CXListCtrl * pList);
    void ClearListCtrl(void);
    COLORREF GetTextRGB(CString strName);
    void UpdataColorMode(BK_COLOR_MODE eMode);
    void UpdatePositionVolume(PlatformStru_TradeInfo& TradeInfo);
};
