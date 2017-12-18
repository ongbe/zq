#pragma once
#include "timelinewin2.h"

class CTimeLineWinEx :
    public CTimeLineWin2
{
public:
//     CTimeLineWinEx(void);
    CTimeLineWinEx(const Stru_TimeLineWin2_Cfg& cfg);
    ~CTimeLineWinEx(void);

    void SetPaintFlag(int nFlag) { m_nPaintFlag = nFlag; }
    void SetPaint(int bEnable) { m_bPaint = bEnable; }

private:
    void OnPaint(HWND hWnd);

    int m_nPaintFlag;                           //0:画成交量与时间柱状图 1:画成交量与次数柱状图 -1:默认
    BOOL m_bPaint;                              //是否开始画
};
