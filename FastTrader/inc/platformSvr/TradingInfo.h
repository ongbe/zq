/***************************************************************************//**
*   @file         TradingInfo.h
*   @brief        操作当日缓存的交易参数，包括品种、合约、手续费率、保证金率
*                   这些数据缓存在tradinginfo.xml/product.xml/instrument.xml/commissionrate.xml/marginrate.xml里
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/

#ifndef _TRADINGPARAM_H_
#define _TRADINGPARAM_H_

//#include <map>
//#include <set>
#include <string>
#include <vector>
//using std::map;
//using std::set;
using std::string;
using std::vector;

//#include "../Module-Misc2/BusinessData.hpp"
#include "easymutex.h"
#include "myxml.h"
#include "../Module-Misc2/KernelStruct.h"



//********************************************************************************************
//  读取xml信息的类
//
//  从xml节中取出参数。可以取品种参数、合约参数、手续费参数、保证金参数
//  <Instruments>
//      <colname>InstrumentID,InvestorRange,BrokerID,InvestorID,OpenRatioByMoney,OpenRatioByVolume,CloseRatioByMoney,CloseRatioByVolume,CloseTodayRatioByMoney,CloseTodayRatioByVolume,StrikeRatioByMoney,StrikeRatioByVolume</colname>
//      <lines>
//          <line>...</line>
//          <line>...</line>
//      </lines>
//  </Instruments>
//
//这里使用malloc的方法申请内存，并自行管理内存，而不用stl来管理。
//原因是在debug模式下，如果合约数量在1400条左右，用stl会耗时5-6秒
//*********************************************************************************

class CTradingInfo
{
public:
    enum eDataStructType
    {
        eTradingParams=1,
        eProductInfos,
        eInstrumentInfos,
        eCommissionRates,
        eMarginRates,
    };

public:
    CTradingInfo(eDataStructType datatype,int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo();

    //从xml中加载数据，分行放到m_pLineContent中，m_psubposs和m_psublens记录每行每个字段的位置和长度
    virtual void Load(void);

public:


    //获取数据类型名称。该名称既用作xml文件名称，又用作节名，可以是tradingparam、product、instrument、commissionrate、marginrate
    static string GetDataTypeName(eDataStructType datatype);
    //获取xml文件名称，带路径
    static string GetXMLFileName(eDataStructType datatype);
    //获取[SvrGrpIDxxx-Accountyyy]节名称
    static string GetAccountNodeName(int SvrGrpID,const string& strAccount);

private:
    //Load时，先检查xml是否合法。1. 如果不是当前交易日，则清空配置文件; 2. 如果没有<SvrGrpIDxxx-Accountyyy>节，则创建之
    void CheckAfterLogin_Internal(CMyXML& myxml);


public:
    eDataStructType m_datatype;
    int             m_SvrGrpID;
    string          m_strAccount;
    string          m_strTradingDay;

    bool            m_bValid;
    vector<string>  m_vecColName;   //列头名称
    int             m_ColCnt;       //列数
    int             m_LineCnt;      //行数
    char**          m_pLineContent; //每一行原始数据
    int*            m_psubposs;     //每行每字段在行原始数据里的位置
    int*            m_psublens;     //每行每字段的长度

    static Ceasymutex      m_mutex;
private:
    //禁止拷贝构造和赋值
    CTradingInfo(const CTradingInfo&);
    CTradingInfo& operator= (const CTradingInfo&);
};

//********************************************************************************************
//  保存xml信息的类
//  使用CTradingInfo::m_mutex
//  节名称为: [root]-[SvrGrpIDxxx-Accountyyy]-[NodeName]
//  列名放在<colname>里
//  数据放在<lines>
//  bEraseOldInfo表示是否删除原有数据。品种、合约、交易参数时一次性保存，可以删除原有数据。费率是收到一条存一条，不能删除原有数据(在登录后的验证中已经把往日数据删掉了)。
//  例如：
//      <Instruments>
//          <colname>InstrumentID,InvestorRange,BrokerID,InvestorID,OpenRatioByMoney,OpenRatioByVolume,CloseRatioByMoney,CloseRatioByVolume,CloseTodayRatioByMoney,CloseTodayRatioByVolume,StrikeRatioByMoney,StrikeRatioByVolume</colname>
//          <lines>
//              <line>...</line>
//              <line>...</line>
//          </lines>
//      </Instruments>
//********************************************************************************************

class CSaveTradingInfo
{
public:
    CSaveTradingInfo(CTradingInfo::eDataStructType datatype,int SvrGrpID,const string& strAccount);
    ~CSaveTradingInfo();

    void OpenXML4Save(bool bEraseOldInfo);

protected:
    CTradingInfo::eDataStructType m_datatype;
    int                 m_SvrGrpID;
    string              m_strAccount;

    const static int    m_linebufsize=10240;
    char*               m_plinecontent;
    char*               m_pcolname;
    int                 m_linebuflen;
    int                 m_colnamelen;
    CMyXML*             m_pxml;
    CMyXML*             m_pnodexml;
    CMyXML*             m_pcolnamexml;
    CMyXML*             m_plinesxml;


private:
    //禁止拷贝构造和赋值
    CSaveTradingInfo(const CSaveTradingInfo&);
    CSaveTradingInfo& operator= (const CSaveTradingInfo&);
};


//********************************************************************************************
//  下面是派生类，负责实现xml数据的加载和保存
//********************************************************************************************
//品种信息
class CTradingInfo_PlatformStru_ProductInfo : public CTradingInfo,CSaveTradingInfo
{
public:
    //加载product.xml中的品种信息，结果放在m_pLineData中
    CTradingInfo_PlatformStru_ProductInfo(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_ProductInfo();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_ProductInfo>& Infos);

public:
    PlatformStru_ProductInfo*    m_pLineData;
};

//合约信息
class CTradingInfo_PlatformStru_InstrumentInfo : public CTradingInfo,CSaveTradingInfo
{
public:
    //加载instrument.xml中的合约信息，结果放在m_pLineData中
    CTradingInfo_PlatformStru_InstrumentInfo(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_InstrumentInfo();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_InstrumentInfo>& Infos);

public:
    PlatformStru_InstrumentInfo*    m_pLineData;
};

//手续费率信息
class CTradingInfo_PlatformStru_InstrumentCommissionRate : public CTradingInfo,CSaveTradingInfo
{
public:
    //加载commissionrate.xml中的手续费率信息，结果放在m_pLineData中
    CTradingInfo_PlatformStru_InstrumentCommissionRate(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_InstrumentCommissionRate();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_InstrumentCommissionRate>& Infos);

public:
    PlatformStru_InstrumentCommissionRate*    m_pLineData;
};

//保证金率信息
class CTradingInfo_PlatformStru_InstrumentMarginRate : public CTradingInfo,CSaveTradingInfo
{
public:
    //加载marginrate.xml中的保证金率信息，结果放在m_pLineData中
    CTradingInfo_PlatformStru_InstrumentMarginRate(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_InstrumentMarginRate();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_InstrumentMarginRate>& Infos);

public:
    PlatformStru_InstrumentMarginRate*    m_pLineData;
};

//经纪公司交易参数信息
class CTradingInfo_PlatformStru_BrokerTradingParams : public CTradingInfo,CSaveTradingInfo
{
public:
    //加载tradingparam.xml中的交易参数信息，结果放在m_pLineData中
    CTradingInfo_PlatformStru_BrokerTradingParams(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_BrokerTradingParams();
    virtual void Load(void);
    void SaveXML(const PlatformStru_BrokerTradingParams& Info);

public:
    PlatformStru_BrokerTradingParams*    m_pLineData;
};

#endif

