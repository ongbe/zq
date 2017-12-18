#include "StdAfx.h"
#include "BKColor.h"

CBKColor::CBKColor(void)
{
    //主窗口
    m_OptionStrategyTitleColorRef = RGB(12,70,144);         //标题
    m_OptionStrategyClientColorRef = RGB(226,226,226);      //客户区

    //T型报价窗口
    m_TPriceTitleColorRef = RGB(58,117,164);        //标题
    m_TPriceClientColorRef = RGB(246,245,245);      //客户区

    //快速下单窗口
    m_QuickOrderColorRef = RGB(226,226,226);                    //客户区
    m_QuickOrderThumbnailColorRef = RGB(156,187,212);           //策略缩略图背景
    m_QuickOrderListHeaderCtrlColorRef = RGB(48,107,156);       //列表头
    m_QuickOrderListCtrlColorRef = RGB(246,245,245);            //列表背景
    m_QuickOrderListItemSelectedColorRef = RGB(49,106,197);     //列表被选中背景

    //分析窗口
    m_AnalyseModuleColorRef = RGB(226,226,226);     //分析窗口

    m_CustomTabPageSheetNormalColorRef = RGB(31,119,217);    //tabCtrl窗口pageSheet正常背景
    m_CustomTabPageSheetSelectedColorRef = RGB(13,49,89);    //tabCtrl窗口pageSheet点选背景

    m_TabLineChartCtrlBottomColorRef = RGB(249,249,249);    //TabProbaCtrl窗口中LineChartCtrl底层背景
    m_TabLineChartCtrlTopColorRef = RGB(234,234,234);       //TabProbaCtrl窗口中LineChartCtrl顶层背景
    m_TabLineChartCtrlBorderColorRef = RGB(160,160,160);    //TabProbaCtrl窗口中LineChartCtrl边框背景
    m_TabLineChartCtrlLostColorRef = RGB(150,199,97);       //TabProbaCtrl窗口中LineChartCtrl亏损背景
    m_TabLineChartCtrlWinColorRef = RGB(229,181,169);       //TabProbaCtrl窗口中LineChartCtrl盈利背景
    m_TabLineChartCtrlAxisTextColorRef = RGB(0,0,0);        //TabProbaCtrl窗口中LineChartCtrl坐标刻度字体
}

CBKColor::~CBKColor(void)
{
}

// 设置颜色选配模式
void CBKColor::SetBkColorMode(BK_COLOR_MODE mode)
{
    if(Color_Light == mode)
    {
        //主窗口
        m_OptionStrategyTitleColorRef = RGB(12,70,144);         //标题
        m_OptionStrategyClientColorRef = RGB(226,226,226);      //客户区

        //T型报价窗口
        m_TPriceTitleColorRef = RGB(58,117,164);        //标题
        m_TPriceClientColorRef = RGB(246,245,245);      //客户区

        //快速下单窗口
        m_QuickOrderColorRef = RGB(226,226,226);                    //客户区
        m_QuickOrderThumbnailColorRef = RGB(156,187,212);           //策略缩略图背景
        m_QuickOrderListHeaderCtrlColorRef = RGB(48,107,156);       //列表头
        m_QuickOrderListCtrlColorRef = RGB(246,245,245);            //列表背景
        m_QuickOrderListItemSelectedColorRef = RGB(49,106,197);     //列表被选中背景

        //分析窗口
        m_AnalyseModuleColorRef = RGB(226,226,226);     //分析窗口

        m_CustomTabPageSheetNormalColorRef = RGB(31,119,217);    //tabCtrl窗口pageSheet正常背景
        m_CustomTabPageSheetSelectedColorRef = RGB(13,49,89);    //tabCtrl窗口pageSheet点选背景

        m_TabListBoxCtrlIconColorRef = RGB(243,148,0);                  //TabTradeAnalyse窗口中ListBoxCtrl item前的图标背景
        m_TabListBoxCtrlIconSelectedColorRef = RGB(243,148,0);          //TabTradeAnalyse窗口中ListBoxCtrl item被选中前的图标背景
        m_TabListBoxCtrlBKColorRef = RGB(255,255,255);                  //TabTradeAnalyse窗口中ListBoxCtrl背景
        m_TabListBoxCtrlItemTextBKColorRef = RGB(255,255,255);          //TabTradeAnalyse窗口中ListBoxCtrl item text背景 
        m_TabListBoxCtrlItemSelectedColorRef = RGB(10,67,137);          //TabTradeAnalyse窗口中ListBoxCtrl item 被选中背景
        m_TabListBoxCtrlItemTextColorRef = RGB(0,0,0);                  //TabTradeAnalyse窗口中ListBoxCtrl item text本色
        m_TabListBoxCtrlItemTextSelectedBKColorRef= RGB(255,255,255);   //TabTradeAnalyse窗口中ListBoxCtrl item text被选中本色

        m_TabLineChartCtrlBottomColorRef = RGB(249,249,249);    //TabProbaChart窗口中LineChartCtrl底层背景
        m_TabLineChartCtrlTopColorRef = RGB(234,234,234);       //TabProbaChart窗口中LineChartCtrl顶层背景
        m_TabLineChartCtrlBorderColorRef = RGB(160,160,160);    //TabProbaChart窗口中LineChartCtrl边框背景
        m_TabLineChartCtrlLineColorRef = RGB(160,160,160);      //TabProbaChart窗口中LineChartCtrl线条背景
        m_TabLineChartCtrlLostColorRef = RGB(150,199,97);       //TabProbaChart窗口中LineChartCtrl亏损背景
        m_TabLineChartCtrlWinColorRef = RGB(229,181,169);       //TabProbaChart窗口中LineChartCtrl盈利背景
        m_TabLineChartCtrlAxisTextColorRef = RGB(0,0,0);        //TabProbaChart窗口中LineChartCtrl坐标刻度字体
    }
    else if(Color_Thick == mode)
    {
        //主窗口
        m_OptionStrategyTitleColorRef = RGB(9,58,117);          //标题
        m_OptionStrategyClientColorRef = RGB(226,226,226);      //客户区

        //T型报价窗口
        m_TPriceTitleColorRef = RGB(1,0,128);           //标题
        m_TPriceClientColorRef = RGB(5,34,69);          //客户区

        //快速下单窗口
        m_QuickOrderColorRef = RGB(226,226,226);                    //客户区
        m_QuickOrderThumbnailColorRef = RGB(239,239,239);           //策略缩略图背景
        m_QuickOrderListHeaderCtrlColorRef = RGB(38,73,103);        //列表头
        m_QuickOrderListCtrlColorRef = RGB(9,58,117);               //列表背景
        m_QuickOrderListItemSelectedColorRef = RGB(120,0,1);        //列表被选中背景

        //分析窗口
        m_AnalyseModuleColorRef = RGB(226,226,226);     //分析窗口

        m_CustomTabPageSheetNormalColorRef = RGB(31,119,217);    //tabCtrl窗口pageSheet正常背景
        m_CustomTabPageSheetSelectedColorRef = RGB(13,49,89);    //tabCtrl窗口pageSheet点选背景

        m_TabListBoxCtrlIconColorRef = RGB(120,0,1);                    //TabTradeAnalyse窗口中ListBoxCtrl item前的图标背景
        m_TabListBoxCtrlIconSelectedColorRef = RGB(0,132,0);            //TabTradeAnalyse窗口中ListBoxCtrl item被选中前的图标背景
        m_TabListBoxCtrlBKColorRef = RGB(9,58,117);                     //TabTradeAnalyse窗口中ListBoxCtrl背景
        m_TabListBoxCtrlItemTextBKColorRef = RGB(9,58,117);             //TabTradeAnalyse窗口中ListBoxCtrl item text背景
        m_TabListBoxCtrlItemSelectedColorRef = RGB(120,0,1);            //TabTradeAnalyse窗口中ListBoxCtrl item 被选中背景
        m_TabListBoxCtrlItemTextColorRef = RGB(255,255,255);                  //TabTradeAnalyse窗口中ListBoxCtrl item text本色
        m_TabListBoxCtrlItemTextSelectedBKColorRef= RGB(255,255,255);         //TabTradeAnalyse窗口中ListBoxCtrl item text被选中本色
        

        m_TabLineChartCtrlBottomColorRef = RGB(9,58,117);           //TabProbaCtrl窗口中LineChartCtrl底层背景
        m_TabLineChartCtrlTopColorRef = RGB(0,0,0);                 //TabProbaCtrl窗口中LineChartCtrl顶层背景
        m_TabLineChartCtrlBorderColorRef = RGB(154,156,158);        //TabProbaCtrl窗口中LineChartCtrl边框背景
        m_TabLineChartCtrlLineColorRef = RGB(160,160,160);          //TabProbaCtrl窗口中LineChartCtrl线条背景
        m_TabLineChartCtrlLostColorRef = RGB(0,132,0);              //TabProbaCtrl窗口中LineChartCtrl亏损背景
        m_TabLineChartCtrlWinColorRef = RGB(255,4,0);               //TabProbaCtrl窗口中LineChartCtrl盈利背景
        m_TabLineChartCtrlAxisTextColorRef = RGB(255,255,255);      //TabProbaCtrl窗口中LineChartCtrl坐标刻度字体
    }
}
