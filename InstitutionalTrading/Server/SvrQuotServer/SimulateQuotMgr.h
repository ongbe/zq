#pragma once

#include "PlatformCTPQuotImpl.h"

#define QUOTTABLENAME   "AQUOTINFO"
enum WEEKDAY{Sunday=0, Monday,Tuesday,Wednesday,Thursday,Friday,Saturday};


class CSimulateQuotMgr
{
public:
    static CSimulateQuotMgr& getObj(void);
    static void deleteObj();   

public:
    CSimulateQuotMgr(void);
    ~CSimulateQuotMgr(void);

    void SetSimulateQuit(int nVal);
    bool GetSimulateQuit();

    void SetSimulatePause(int nVal);
    bool GetSimulatePause();

    bool GetSimulateData(eSimulateDayType& nType,vector<string>& vDayTime);
    int  GetSimulateQuotSize();

    void SetSimulateQuot(const PlatformStru_DepthMarketData& pDepthMarketData);
    void GetSimulateQuot(std::vector<PlatformStru_DepthMarketData>& vecdepthMarketData);

    //一旦手动开始回放操作时，自动的就失效，直到手动停止回放操作后自动才生效
    //手动相关
    //模拟行情时间交易日
    void LoadSimulateDay();
    CF_ERROR GetSimulateDay(sSimulateDay& sDay);
    CF_ERROR SetSimulateDay(const sSimulateDay& sDay);

    //模拟行情回放方式
    void LoadSimulatePlayMode();
    CF_ERROR GetSimulatePlayMode(eSimulatePlayMode& eMode);
    CF_ERROR SetSimulatePlayMode(const eSimulatePlayMode& sMode);

    //模拟控制
    bool SimulateQuotPlay();
    bool SimulateQuotPause();
    bool SimulateQuotStop();

    //自动相关
    //模拟时间点（自动模拟时间 按星期+时间段）
    void LoadSimulateTime();
    CF_ERROR GetSimulateTime(sSimulateTime& sTime);

public:
    static CSimulateQuotMgr* m_pObj;
    static Ceasymutex	m_mutexSimulate;

    HANDLE  m_hReadStopEvent;
    HANDLE  m_hSendStopEvent;
private: 
    sSimulateDay    m_sSimulateDay;   
    sSimulateTime   m_sSimulateTime;
    eSimulatePlayMode m_eSimulatePlayMode;

    volatile int m_bSimulateQuit;        //
    volatile int m_bSimulatePause;       //

    //手动
    HANDLE m_hReadThread_manual;
    HANDLE m_hSendThread_manual;

    DWORD m_idReadThread_manual;
    DWORD m_idSendThread_manual;

    queue<PlatformStru_DepthMarketData>  m_queueSimulateQuot; //保存从数据库中读取的每条深度行情数据
};
