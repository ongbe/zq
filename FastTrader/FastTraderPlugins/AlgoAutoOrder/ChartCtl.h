#pragma once

#include "TimeLineWinEx.h"
#include "Tools_AlgoTrading.h"
#include "AlgoAutoData.h"

// CChartCtl

#define ClassName_ChartCtl "ChartCtrlClass"

#define BkCol_ChartCtl RGB(204,232,207)
#define njCol_ChartCtl RGB(0,0,255)				//无趋势量能柱状图颜色
#define njTradedCol_ChartCtl RGB(0,0,192)		//无趋势已成交量能柱状图颜色
#define nj2Col_ChartCtl RGB(128,64,0)			//有趋势量能柱状图颜色
#define nj2TradedCol_ChartCtl RGB(96,48,0)		//有趋势已成交量能柱状图颜色
#define VCol_ChartCtl RGB(255,0,0)			    //纵坐标线段

class CChartCtl : public CWnd
{
	DECLARE_DYNAMIC(CChartCtl)

public:
	CChartCtl();
	virtual ~CChartCtl();
	BOOL RegisterWndClass();

	//创建子窗口。因为自定义控件无法响应OnCreate事件，所以只能显式的创建
	bool CreateChildren(int nFlag = -1);

    //时间轴子窗口
 	CTimeLineWinEx m_TimeLineWin;

protected:
	DECLARE_MESSAGE_MAP()

public:
	void setcontent(CAlgoAutoData *pAlgo,time_t StartTradingUTC);
	//刷新已成交量。只刷新指定节。
	void RefreshTradedVolume(int Type);
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

	CAlgoAutoData *m_pAlgoData;

public:
    // 0：成交量与时间关系 1：成交量与下单量关系
    int m_nType;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	bool m_bMouseInClient;
	POINT m_LastMousePt;

    bool m_bPaint;


};


