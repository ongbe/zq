#pragma once

#include "KernelStruct.h"
#include "IniFile.h"
#include "Tools_Win32.h"

#include <string>
using namespace std;


//保存策略计算参数的ini文件名
#define LogFileName_ArbitrageDelegation "ArbitrageDelegation.log"
#define IniFileName_ArbitrageDelegation "ArbitrageDelegation.ini"
#define Node_ArbitrageDelegation        "ArbitrageDelegation"

class CParamData
{

public:
    CParamData(void);
    CParamData& operator =(CParamData& other);
    ~CParamData(void);


    /*测试*/
    void SetBandPrice(int& text);
    void WriteLog();

public:    

    bool            Param_RevokeFlag;         //是否撤单
    bool            Param_TriggerFlag;        //是否触发

    int             Param_ID;                 //流水号
    int				Param_OffsetID;			  //开平，0:开仓；1:平仓；2:平
    int				Param_ArbitrageType;	  //套利类型，0:价差缩小套利1:价差扩大套利
    double          Param_TriggerPrice;       //触发价差
    double          Param_DeviatePrice;       //偏离板价
    CTime           Param_ValidateDay;        //有效期
    string          Param_ValidateDayText;    //有效期(用于转换)
    CTime           Param_ValidateCreateDay;  //创建套利的日期

    //合约1
    string			Order_Instrument1;	      //合约名称
    int				Order_Direction1;		  //方向，0:买；1:卖
    int             Order_Count1;             //手数
    double          Order_Price1;             //下单委托价格
    double          Order_DelegatePrice1;     //委托价差
    double          Order_UpperPrice1;        //涨停价
    double          Order_LowestPrice1;       //跌停价

    //合约2
    string			Order_Instrument2;	      //合约名称
    int				Order_Direction2;		  //方向，0:买；1:卖
    int             Order_Count2;             //手数
    double          Order_Price2;             //下单委托价格
    double          Order_DelegatePrice2;     //委托价差
    double          Order_UpperPrice2;        //涨停价
    double          Order_LowestPrice2;       //跌停价

    //Orderkey1
    string          OrderKey_Account1;
    string          OrderKey_Instrument1;
    int             OrderKey_FrontID1;
    int             OrderKey_SessionID1;
    string          OrderKey_OrderRef1;

    //Orderkey2
    string          OrderKey_Account2;
    string          OrderKey_Instrument2;
    int             OrderKey_FrontID2;
    int             OrderKey_SessionID2;
    string          OrderKey_OrderRef2;

};