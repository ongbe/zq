/*
    用于保存和维护CTP的费率数据
    added by l. 20130328
*/


#pragma once

#include <map>
#include <string>

#include "easymutex.h"
#include "../Module-Misc2/KernelStruct.h"

#include "PlatformDataSetList.hpp"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Rates
{
public:
    CPlatformDataMgr_Rates(void);
    ~CPlatformDataMgr_Rates(void);


    ///获取合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    bool IsCommissionRateExist(const string& InstrumentID);
	std::map<std::string, PlatformStru_InstrumentCommissionRate> GetCommissionRateAll(void) { CeasymutexGuard guard(m_mutex);  return m_InstrumentCommissionRate; }
    ///设置合约手续费率
    void SetCommissionRate(const std::string& InstrumentID, const PlatformStru_InstrumentCommissionRate& inData);
    ///设置正在查询手续费率的合约,用于查询返回时对应
    void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
    ///获取正在查询手续费率的合约
    void GetReqCommissionRateInstrumentID(std::string& outData);

    ///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    bool IsMarginRateExist(const string& InstrumentID);
	std::map<std::string, PlatformStru_InstrumentMarginRate> GetMarginRateAll(void) { CeasymutexGuard guard(m_mutex);  return m_InstrumentMarginRate; }
    ///获取合约保证金率，内部调用，不用加锁
    bool GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///设置合约保证金率
    void SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);

    void IncrementReqMarginRateError(const std::string& InstrumentID);
    void IncrementReqCommissionRateError(const std::string& InstrumentID);

    bool IsNeedCheckCommissionRateAndMarginRate(void) { return m_bNeedCheckCommissionRateAndMarginRate;}
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) { m_bNeedCheckCommissionRateAndMarginRate=bNeedCheck;}

    void GetAllCommissionRates(vector<PlatformStru_InstrumentCommissionRate>& Rates);
    void GetAllMarginRates(vector<PlatformStru_InstrumentMarginRate>& Rates);

    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);

private:
    ///获取合约手续费率，内部调用，不用加锁
    bool GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);

private:
    Ceasymutex				m_mutex;
    bool m_bNeedCheckCommissionRateAndMarginRate;                           //是否需要检查手续费率和保证金率，当更新合约列表时，此值为true，查询完后，此值为false

    std::map<std::string, PlatformStru_InstrumentCommissionRate> m_InstrumentCommissionRate;//手续费率
    std::map<std::string, PlatformStru_InstrumentMarginRate> m_InstrumentMarginRate;        //保证金率
    std::string m_ReqCommissionRateInstrumentID;                            //正在查询手续费率的合约

    std::map<std::string, int> m_reqMarginRateError;                        //请求保证金查询无记录次数
	std::map<std::string, int> m_reqCommissionRateError;                    //请求手续费查询无记录次数
};

#ifdef WIN32
#pragma managed(pop)
#endif 
