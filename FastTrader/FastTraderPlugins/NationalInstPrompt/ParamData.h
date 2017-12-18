#pragma once

#include "IniFile.h"
#include "Tools_Win32.h"

#include <string>
using namespace std;


//保存策略计算参数的ini文件名
#define LogFileName_NationalInstPrompt "NationalInstPrompt.log"
#define IniFileName_NationalInstPrompt "NationalInstPrompt.ini"
#define Node_NationalInstPrompt        "NationalInstPrompt"

class CParamData
{

public:
    CParamData(void);
    CParamData& operator=(CParamData& other);
    ~CParamData(void);

    //相关算法
    bool Calculate(double fLastQuotPrice);

    /*测试*/
    void SetBandPrice(int& text);
    void WriteLog();

public:

    bool            TurnOn;                   //触发是否启用

    //行情
    double          Param_QuotPrice;          //行情最新价

    //策略输入项
    string          Param_BandCode;           //现券代码
    string          Param_BandInstrument;     //现券合约
    double          Param_IRR;                //IRR
    double          Param_InterestRates;      //票面利率
    double          Param_BandPrice;          //现券价格
    double          Param_TransferFactor;     //转换因子
    int             Param_YearInterestCount;  //年付息次数
    CTime           Param_PreDelivDate;       //上一付息日
    CTime           Param_NextDelivDate;      //下一付息日
    CTime           Param_DelivDate;          //交割日
    string          Param_PreDelivDateText;   //上一付息日(回显时使用)
    string          Param_NextDelivDateText;  //下一付息日(回显时使用)
    string          Param_DelivDateText;      //交割日(回显时使用)


    //触发输入项
    bool            Order_Alarm;              //是否报警
    bool            Order_AutoOrder;          //是否自动下单
    string			Order_Instrument;	      //合约名称
    int				Order_Direction;		  //方向，0:买；1:卖
    int				Order_OffsetID;			  //开平，0:开仓；1:平仓；2:平今
    int             Order_Count;              //手数
    int             Order_Price;              //价格, 0:市价



    //结果项
    double          Interest;                 //Interest
    double          NeedInterest;             //应计利息
    double          InvoicePrice;             //发票价格
    double          Result_BandTargetPrice;   //现券目标价格

};
