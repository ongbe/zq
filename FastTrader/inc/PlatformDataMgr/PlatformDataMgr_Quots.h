/*
    用于保存和维护CTP的行情数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Quots
{
public:
    CPlatformDataMgr_Quots(CPlatformDataMgr_Instruments& refInstruments);
    ~CPlatformDataMgr_Quots(void);

    //获取指定合约行情
    bool GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    //设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
    bool SetQuotInfo(const PlatformStru_DepthMarketData& inData);
    //获取指定合约的最新价，失败返回无效值
    double GetCurPrice (const std::string& InstrumentID);
    //获取指定合约旧行情
    bool GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);

private:
    Ceasymutex				        m_mutex;

    CPlatformDataMgr_Instruments    &m_RefInstruments;

    //行情数据集
    std::map<std::string,PlatformStru_DepthMarketData> m_lastQuot;    //最新行情
    std::map<std::string,PlatformStru_DepthMarketData> m_OldQuot;     //上次老行情
};

#ifdef WIN32
#pragma managed(pop)
#endif 
