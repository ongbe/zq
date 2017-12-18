#pragma once
#include <math.h>
#include <algorithm> 
#include <vector>
#include <map>
#include <set>
using namespace std;

#include "KernelStruct.h"
#include "orderDefines.h"
#include "OrderFunc.h"
#include "LogFile.h"
#include "easymutex.h"
#include "easytime.h"
#include "easytimespan.hpp"


//#define _LOG
#define LogFileName_OptionStrategy "OptionStrategy.log"

#define  MAKE_CMDID(_base,_offset) \
    ((int)( (int)(_base) << 16 ) + (int)(_offset))

#define  CMDID_BASE_HIWORD_QUOT						        0x000F //行情高位CMDID
#define Cmd_QueryVolatilityData_Req                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0016)	//查询波动率请求
#define Cmd_QueryVolatilityData_Rsp                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0017)	//查询波动率返回

#pragma pack(push)
#pragma pack(1)
struct Volatility 
{
    ///交易日
    TThostFtdcDateType	TradingDay;
    ///合约代码
    InstrumentIDType	InstrumentID;
    ///20天波动率
    TThostFtdcPriceType	VolatilityTwenty;
    ///60天波动率
    TThostFtdcPriceType	VolatilitySixty;
};

#include "PLProbability.h"

//User message define
#define WM_USER_QUOT_UPDATE         WM_USER+600
#define WM_USER_VOLATILITY_UPDATE   WM_USER+700
#define WM_VOLATILITY_RESET         WM_USER+800
#define WM_USER_POSITION_UPDATE     WM_USER+900
#define WM_USER_EXPIREDATE_UPDATE   WM_USER+1000

enum eStrategyID
{
    StrategyID_Rose,					//大涨
    StrategyID_Crash,					//大跌
    StrategyID_Peakback,				//触顶回落
    StrategyID_Bottomout,				//触底回升
    StrategyID_Breakout_Straddle,			//突破-买入跨式策略
    StrategyID_Breakout_Strangle,			//突破-买入勒式策略
    StrategyID_Consolidation_Straddle,		//盘整-卖出跨式策略
    StrategyID_Consolidation_Strangle,		//盘整-卖出勒式策略
    StrategyID_ConsolidationUp_Call,		//盘涨Call
    StrategyID_ConsolidationUp_Put,		//盘涨Put
    StrategyID_ConsolidationDown_Call,		//盘跌Call
    StrategyID_ConsolidationDown_Put,		//盘跌Put
};
//定义更新CLineChart控件的数据结构
/*
    Buy         买进
    Sell        卖出
    Bearish     看跌
    Bullish     看涨
*/
enum eOrderStyle{eNone,BuyBullish,BuyBearish,SellBullish,SellBearish,eBullish,eBearish};
enum CHART_TYPE{eProbaChart,eProfitChart};

inline const int FormatStrategyID(const char *nStrategyName)
{
    int nStrategyID=-1;
    if(0==strcmp(nStrategyName,"大涨"))
        nStrategyID = StrategyID_Rose;
    else if(0==strcmp(nStrategyName,"大跌"))
        nStrategyID = StrategyID_Crash;
    else if(0==strcmp(nStrategyName,"触顶回落"))
        nStrategyID = StrategyID_Peakback;
    else if(0==strcmp(nStrategyName,"触底回升"))
        nStrategyID = StrategyID_Bottomout;
    else if(0==strcmp(nStrategyName,"突破-买入跨式"))
        nStrategyID = StrategyID_Breakout_Straddle;
    else if(0==strcmp(nStrategyName,"突破-买入勒式"))
        nStrategyID = StrategyID_Breakout_Strangle;
    else if(0==strcmp(nStrategyName,"盘整-卖出跨式"))
        nStrategyID = StrategyID_Consolidation_Straddle;
    else if(0==strcmp(nStrategyName,"盘整-卖出勒式"))
        nStrategyID = StrategyID_Consolidation_Strangle;
    else if(0==strcmp(nStrategyName,"盘涨-Call"))
        nStrategyID = StrategyID_ConsolidationUp_Call;
    else if(0==strcmp(nStrategyName,"盘涨-Put"))
        nStrategyID = StrategyID_ConsolidationUp_Put;
    else if(0==strcmp(nStrategyName,"盘跌-Call"))
        nStrategyID = StrategyID_ConsolidationDown_Call;
    else if(0==strcmp(nStrategyName,"盘跌-Put"))
        nStrategyID = StrategyID_ConsolidationDown_Put;

    return nStrategyID;
}

struct sStrategyData  
{
    sStrategyData()
    {
        eStyle = eNone;
        dOrderPrice           = 50.0;
        dUnderlyingMultiple   = 100.0;
    }

    eOrderStyle eStyle;                     //报单类型
    double      dUnderlyingMultiple;        //合约乘数
    int         nOrderCount;                //报单个数
    double      dOrderPrice;                //报单价格
    double      dStrikePrice;               //行权价
};

//////////////////////////////////////////////////////////////////////////
/* 基本几何结构 */ 
struct _POINT 
{ 
    double x; 
    double y; 
    _POINT(double a=0, double b=0) { x=a; y=b;} //constructor 

    bool operator==(const _POINT& pt) const 
    { 
        bool bx = fabs(x-pt.x)<0.000001;
        bool by = fabs(y-pt.y)<0.000001;

        //TRACE("x = %f , pt.x = %f ,fabs(%f) = %f\n",x,pt.x,x-pt.x,fabs(x-pt.x));
        //TRACE("y = %f , pt.y = %f ,fabs(%f) = %f\n",y,pt.y,y-pt.y,fabs(y-pt.y));
        //TRACE("bx = %s, by = %s\n",bx?"true":"false",by?"true":"false");
        return (bx && by); 
    } 
}; 
struct _LINESEG 
{ 
    _POINT s; 
    _POINT e; 
    _LINESEG(_POINT a, _POINT b) { s=a; e=b;} 
    _LINESEG() { } 
}; 
struct _LINE           // 直线的解析方程 a*x+b*y+c=0  为统一表示，约定 a >= 0 
{ 
    double a; 
    double b; 
    double c; 
    _LINE(double d1=1, double d2=-1, double d3=0) {a=d1; b=d2; c=d3;} 
}; 


typedef _POINT sTurnPt;
typedef _POINT sBalancePt;


struct sPoint
{
    eOrderStyle eStyle;
    sTurnPt     ptTurn;
    sBalancePt  ptBalance;
};

class CDataInfo
{
private:
    CDataInfo();
    ~CDataInfo();

public:
    static CDataInfo* NewInstance();
    static void DestroyInstance();

private:
    static CDataInfo* m_pInstance;


public:
    void SetBalancePoint(const vector<sBalancePt>& vBalancePt);
    void GetBalancePoint(vector<sBalancePt>& vBalancePt);

    void SetMouseIn(bool bMouseIn);
    bool GetMouseIn();

    void    SetMousePt(CPoint sMousePt);
    CPoint  GetMousePt();


private:
    vector<sBalancePt> m_vBalancePos;       //所有损益平衡点逻辑坐标

    bool        m_bMouseIn;                 //是否在任何一个窗口内
    CPoint      m_ptMouse;                  //任何一个窗口中的鼠标坐标


public:

    void    SetTradingDayFlag(bool bTradingDay);
    void    SetExchangeFlag(bool bArrived);
    void    SetProductFlag(bool bArrived);
    void    SetInstrumentFlag(bool bArrived);

    bool    GetTradingDayFlag();
    bool    GetExchangeFlag();
    bool    GetProductFlag();
    bool    GetInstrumentFlag();
    
    void    SetVolatilityFlag(bool bArrived);
    void    SetLastPriceFlag(bool bArrived);

    bool    GetCaculateFlag();

    void    SetVolatilityTestFlag(bool bTest);
    bool    GetVolatilityTestFlag();

    void    SetInit(bool bInit);
    bool    GetInit();


    string  GetAccount();

    void    SetVolatility(const double& dVolatility);
    void    SetVolatilityTest(const double& dVolatilityTest);
    double  GetVolatility();

    void    GetProfitDlgParams(CTime& tExpireDate,string& strRestDays);

    void    SetStrategyData(const vector <sStrategyData>& vData);
    void    GetStrategyData(vector <sStrategyData>& vData);

    void    SetRelaInstCount(const int& nCount);
    int     GetRelaInstCount();

    void    SetInstrumentInfo(const PlatformStru_InstrumentInfo& info);
    double  GetInstPriceTick();                     //标的物最小波动价位
    double  GetInstPriceTick(const string& sInstID);//相关期权最小波动价位
    double  GetInstVolumeMultiple();                     //标的物合约乘数
    double  GetInstVolumeMultiple(const string& sInstID);//相关期权合约乘数
    string  GetExpireDate();
    

    void    SetInstrumentDepthData(const PlatformStru_DepthMarketData& depthData);  
    double  GetLastPrice();
    double  GetInstUpperPrice();
    double  GetInstLowerPrice();
    double  GetInstUpperPrice(const string& sInstID);
    double  GetInstLowerPrice(const string& sInstID);


    
private:

    CEasyTime *m_time1,*m_time2;
    /// 策略报单相关
    bool        m_bInit;
    double      m_dQuotLastPrice;                   //标的物行情价格
    double      m_dVolatility;                      //标的物价格年化波动率(20天) 来自标的物信息
    double      m_dVolatilityTest;                  //标的物价格年化波动率(20天) 来自盈亏窗口

    vector <sStrategyData> m_vStrategyData;         //策略报单

    
    //画图相关数据////////////////////////////////////////////////////////////////////////
public:  
    void    SetChartInfo(CHART_TYPE eType, const CRect& rc);   
    void    Calculate();
    void    CalculateTest();
    void    SetXAxisTick();
    void    GetXAxisInfo(vector<double>& vXAxisPos, double& dPriceMin, double& nSpaceValue,double& dXPosValueMin,double& dXValueMax);
    void    SetYAxisTick();
    

    static bool CompareOrderByStrikePriceASC(const sStrategyData& l, const sStrategyData& r){return l.dStrikePrice<r.dStrikePrice;}
    static bool ComparePointByXASC(const _POINT& l, const _POINT& r)
    {
        if(fabs(l.x-r.x)<=0.000001)
            return l.y < r.y;

        return l.x<r.x;
    }

    void    SetPos();
    bool    GetPos(vector<_POINT>& vPos);

    void    GetStrategyPos(vector<_POINT>& _vPoint);
    void    MergeStrategyPos(vector<_POINT>& _vPoint);
    double  GetYPosValue(const double& dXPosValue);
    void    GetYPosValue(CHART_TYPE  eType, double& dYPosValueMin, double& dYPosValueMax);
    bool    GetBalancePos(vector<_POINT>& _vPoints, vector<_LINESEG>& _vLineSeg);

    double  GetProbaYPosValue(double dXPosValue);
    double  GetArea(double dXPosValue);            

private:
    CPLProbability*     m_PLProbability;
    CHART_TYPE          m_ChartType;                                //窗口标识
    CRect               m_ProbaRect, m_ProfitRect;                  //所画区域
    int                 m_nRelaInstCount;                           //相关标的物个数,用于x坐标轴划分
    double              m_dValueMin, m_dValueMax;                   //确定所画X坐标逻辑最值
    double              m_dXAxisPosValueMin, m_dXAxisPosValueMax;   //x逻辑坐标最大值和最小值(行权价) 

    double              m_dYAxisPosValueMin1, m_dYAxisPosValueMax1;   //Proba窗口的Y逻辑坐标最大值和最小值(最大损益权利金)
    double              m_dYAxisPosValueMin2, m_dYAxisPosValueMax2;   //Profit窗口的Y逻辑坐标最大值和最小值(最大损益权利金)

    vector<double>      m_vecXAxisPos;                              //X轴坐标刻度点

    double              m_dPriceMin;
    double              m_dPriceSpace;


    bool                m_bBalance;                                 //是否有平衡点标识
    vector<_POINT>      m_vPoint;                                   //每个策略报单的转折和平衡点

public:    
    void    GetExchangeID2ProductID(map<string,set<string>>& mapData);
    void    GetProductID2InstID(map<string,set<string>>& mapData);

    string GetProductByInstID(const string& szCurrInstrumentID);
    string GetExchangeByProductID(const string& sProductName);

public:
    Ceasymutex      m_mutexLastQuot;
    Ceasymutex      m_mutexRelaInst;
    Ceasymutex      m_mutexCaculate;

    CLogFile*   m_pLog;                                     //log
    bool m_bLogon;                                          // 是否登录上CTP
    string m_szAccount;                                     //当前的账号
    set<std::string> m_setExchangeID;                       //交易所
    string      m_TradingDay;                               //交易日
    map<string,set<string>> m_mapExchangeID2ProductID;      //交易所下的产品品种
    map<string,set<string>> m_mapProductID2InstID;          //产品品种下的合约

    string  m_SubscribedInstrument;                         //当前标的物合约
    set<string> m_setRelaInstrumentInfoReq;                 //相关标的物合约请求 ：T型报价需一次全部给它初始化，因为异步请求，所以要记录请求的相关合约是否都到达
    static bool upSort(const PlatformStru_InstrumentInfo& l, const PlatformStru_InstrumentInfo& r){return l.StrikePrice < r.StrikePrice;}

    void    SetLastQuotInfo(const PlatformStru_DepthMarketData& data);
    void    GetLastQuotInfo(map<string,PlatformStru_DepthMarketData>& outData);
    double  GetLimitPrice(const string& sInstID, bool bDirect);
    void    GetRelaInstrumentInfo(vector<PlatformStru_InstrumentInfo>& vData);

    map<string,PlatformStru_DepthMarketData> m_mapLastQuot;             //保存每个合约的最新行情
    vector<PlatformStru_InstrumentInfo> m_vecRelaInstrumentInfoReq;     //保存相关期权合约信息

    //合约信息相关
    bool m_bTradingDayFlag;         
    bool m_bExchangeFlag;
    bool m_bProductFlag;    
    bool m_bInstrumentFlag; 
    bool m_bCaculateFlag;
    bool m_LastPriceFlag;                                           //最新价是否查询到
    bool m_VolatilityFlag;                                          //波动率是否查询到
    bool m_bVolatilityTestFlag;                                      //是否是自订波动率设置状态
    PlatformStru_InstrumentInfo     m_InstrumentInfo;                 //当前标的物信息
    PlatformStru_DepthMarketData    m_InstrumentDepthData;            //当前标的物行情

    CString m_strNote;//错误信息
    CString GetNoteText();
    void    SetNoteText(CString strNote);
};
