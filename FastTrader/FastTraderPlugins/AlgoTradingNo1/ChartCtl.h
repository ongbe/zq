#pragma once

#include "TimeLineWin2.h"
#include "common-AlgoTradingNo1Dlg.h"
#include "Algo_No1.hpp"

// CChartCtl

#define ClassName_ChartCtl "ChartCtrlClass"

#define BkCol_ChartCtl RGB(204,232,207)
#define njCol_ChartCtl RGB(0,0,255)				//无趋势量能柱状图颜色
#define njTradedCol_ChartCtl RGB(0,0,192)		//无趋势已成交量能柱状图颜色
#define nj2Col_ChartCtl RGB(128,64,0)			//有趋势量能柱状图颜色
#define nj2TradedCol_ChartCtl RGB(96,48,0)		//有趋势已成交量能柱状图颜色

class CChartCtl : public CWnd
{
	DECLARE_DYNAMIC(CChartCtl)

public:
	CChartCtl();
	virtual ~CChartCtl();
	BOOL RegisterWndClass();

	//创建子窗口。因为自定义控件无法响应OnCreate事件，所以只能显式的创建
	bool CreateChildren(void);

    //时间轴子窗口
	CTimeLineWin2 m_TimeLineWin;

protected:
	DECLARE_MESSAGE_MAP()

public:
	void setcontent(const CAlgo_No1& algo,time_t StartTradingUTC);
	//刷新已成交量。只刷新指定节。
	void RefreshTradedVolume(int TradingSession,int TradedVolume);
	//utc是否已经超时
	bool isovertime(time_t utc);
    //根据时间取x轴位置
    int getxPos(time_t utc);
    //取时间轴窗口宽度
    int getTimeWndWid(void);
    //重置交易时间参数
    void setTradingTimespan(const CEasyTradingTimespan& TradingTimespan);
    //刷新时间刻度窗口的参数
    void refreshTimeLineWin(void);

	CAlgo_No1 m_algo;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	bool m_bMouseInClient;
	POINT m_LastMousePt;
};


