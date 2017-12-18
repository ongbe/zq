#include "StdAfx.h"
#include "ParamData.h"
#include "LogFile.h"

CParamData::CParamData(void)
{
    Param_RevokeFlag = false;
    Param_TriggerFlag = false;

    Param_ID = 0;
    Param_OffsetID = 0;			    
    Param_ArbitrageType = 0;	   
    Param_TriggerPrice = 0.0f;     
    Param_DeviatePrice = 0.0f;      
    Param_ValidateDay = CTime::GetCurrentTime(); 
    Param_ValidateCreateDay = CTime::GetCurrentTime(); 

    //合约1
    Order_Instrument1 = "";	     
    Order_Direction1 = 0;		  
    Order_Count1 = 0;            
    Order_DelegatePrice1 = 0.0f; 
    Order_UpperPrice1 = 0.0f;   
    Order_LowestPrice1 = 0.0f;  

    //合约2
    Order_Instrument2 = "";	     
    Order_Direction2 = 1;		 
    Order_Count2 = 0;             
    Order_DelegatePrice2 = 0.0f; 
    Order_UpperPrice2 = 0.0f;     
    Order_LowestPrice2 = 0.0f;  

    //OrderKey1
    OrderKey_Account1="";
    OrderKey_Instrument1="";
    OrderKey_FrontID1=0;
    OrderKey_SessionID1=0;
    OrderKey_OrderRef1="";

    //OrderKey2
    OrderKey_Account2="";
    OrderKey_Instrument2="";
    OrderKey_FrontID2=0;
    OrderKey_SessionID2=0;
    OrderKey_OrderRef2="";
}

CParamData& CParamData::operator =(CParamData& other)
{   
    Param_RevokeFlag = other.Param_RevokeFlag;
    Param_TriggerFlag = other.Param_TriggerFlag;

    Param_ID = other.Param_ID;
    Param_OffsetID = other.Param_OffsetID;
    Param_ArbitrageType = other.Param_ArbitrageType;
    Param_TriggerPrice = other.Param_TriggerPrice;
    Param_DeviatePrice = other.Param_DeviatePrice;
    Param_ValidateDay = other.Param_ValidateDay;
    Param_ValidateCreateDay = other.Param_ValidateCreateDay;

    //合约1
    Order_Instrument1 = other.Order_Instrument1;
    Order_Direction1 = other.Order_Direction1;
    Order_Count1 = other.Order_Count1;
    Order_DelegatePrice1 = other.Order_DelegatePrice1;
    Order_UpperPrice1 = other.Order_UpperPrice1;
    Order_LowestPrice1 = other.Order_LowestPrice1;

    //合约2
    Order_Instrument2 = other.Order_Instrument2;
    Order_Direction2 = other.Order_Direction2;
    Order_Count2 = other.Order_Count2;
    Order_DelegatePrice2 = other.Order_DelegatePrice2;
    Order_UpperPrice2 = other.Order_UpperPrice2;
    Order_LowestPrice2 = other.Order_LowestPrice2;

    OrderKey_Account1=other.OrderKey_Account1;
    OrderKey_Instrument1=other.OrderKey_Instrument1;
    OrderKey_FrontID1=other.OrderKey_FrontID1;
    OrderKey_SessionID1=other.OrderKey_SessionID1;
    OrderKey_OrderRef1=other.OrderKey_OrderRef1;

    OrderKey_Account2=other.OrderKey_Account2;
    OrderKey_Instrument2=other.OrderKey_Instrument2;
    OrderKey_FrontID2=other.OrderKey_FrontID2;
    OrderKey_SessionID2=other.OrderKey_SessionID2;
    OrderKey_OrderRef2=other.OrderKey_OrderRef2;

    return *this;
}

CParamData::~CParamData(void)
{
}

/*测试*/
void CParamData::SetBandPrice(int& text)
{
    //Param_BandPrice += text;
    text = 0;
}

void CParamData::WriteLog()
{
    //获取程序路径
    //char PathFileName[512];
    //memset(PathFileName,0,sizeof(PathFileName));
    //CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(LogFileName_NationalInstPrompt));
    //strcat(PathFileName,LogFileName_NationalInstPrompt);    

    //char buf[256];
    //memset(buf,0,sizeof(buf));
    //sprintf(buf,"现券代码=%s, Interest=%.6f, 应计利息=%.6f, 发票价格=%.6f, 现券目标价格=%.6f, 行情价=%.6f, 现券价格=%.6f\r\n",
    //        Param_BandCode.c_str(),Interest,NeedInterest,InvoicePrice,Result_BandTargetPrice,Param_QuotPrice,Param_BandPrice);

    //CLogFile log(PathFileName);
    //log.Log(buf);
}

