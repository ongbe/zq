#pragma once

//配色
enum BK_COLOR_MODE
{
    Color_Light,    //浅色
    Color_Thick     //深色
};


class CBKColor
{
public:
    CBKColor(void);
    virtual ~CBKColor(void);

private:
    //主窗口
    COLORREF    m_OptionStrategyTitleColorRef;       //标题
    COLORREF    m_OptionStrategyClientColorRef;      //客户区

    //T型报价窗口
    COLORREF    m_TPriceTitleColorRef;       //标题
    COLORREF    m_TPriceClientColorRef;      //客户区

    //快速下单窗口
    COLORREF    m_QuickOrderColorRef;                   //客户区
    COLORREF    m_QuickOrderThumbnailColorRef;          //策略缩略图背景
    COLORREF    m_QuickOrderListHeaderCtrlColorRef;     //列表头
    COLORREF    m_QuickOrderListCtrlColorRef;           //列表背景
    COLORREF    m_QuickOrderListItemSelectedColorRef;   //列表被选中背景

    //分析窗口
    COLORREF    m_AnalyseModuleColorRef;    //分析窗口

    COLORREF    m_CustomTabPageSheetNormalColorRef;    //tabCtrl窗口pageSheet正常背景
    COLORREF    m_CustomTabPageSheetSelectedColorRef;  //tabCtrl窗口pageSheet点选背景

    COLORREF    m_TabListBoxCtrlIconColorRef;               //TabTradeAnalyse窗口中ListBoxCtrl item前的图标背景
    COLORREF    m_TabListBoxCtrlIconSelectedColorRef;       //TabTradeAnalyse窗口中ListBoxCtrl item被选中前的图标背景
    COLORREF    m_TabListBoxCtrlBKColorRef;                 //TabTradeAnalyse窗口中ListBoxCtrl背景
    COLORREF    m_TabListBoxCtrlItemTextBKColorRef;         //TabTradeAnalyse窗口中ListBoxCtrl item text背景
    COLORREF    m_TabListBoxCtrlItemSelectedColorRef;       //TabTradeAnalyse窗口中ListBoxCtrl item 被选中背景
    COLORREF    m_TabListBoxCtrlItemTextColorRef;           //TabTradeAnalyse窗口中ListBoxCtrl item text本色
    COLORREF    m_TabListBoxCtrlItemTextSelectedBKColorRef; //TabTradeAnalyse窗口中ListBoxCtrl item text被选中本色
    

    COLORREF    m_TabLineChartCtrlBottomColorRef;  //TabProbaCtrl窗口中LineChartCtrl底层背景
    COLORREF    m_TabLineChartCtrlTopColorRef;     //TabProbaCtrl窗口中LineChartCtrl顶层背景
    COLORREF    m_TabLineChartCtrlBorderColorRef;  //TabProbaCtrl窗口中LineChartCtrl边框背景
    COLORREF    m_TabLineChartCtrlLineColorRef;    //TabProbaCtrl窗口中LineChartCtrl线条背景    
    COLORREF    m_TabLineChartCtrlLostColorRef;    //TabProbaCtrl窗口中LineChartCtrl亏损背景
    COLORREF    m_TabLineChartCtrlWinColorRef;     //TabProbaCtrl窗口中LineChartCtrl盈利背景
    COLORREF    m_TabLineChartCtrlAxisTextColorRef;    //TabProbaCtrl窗口中LineChartCtrl坐标刻度字体
public:
    // 设置颜色选配模式
    void SetBkColorMode(BK_COLOR_MODE mode);
};
