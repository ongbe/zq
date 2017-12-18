/*
    用于保存和维护CTP的合约、交易所时间、交易所信息等数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
using std::string;
using std::map;
using std::multimap;
using std::set;

#include "easymutex.h"
#include "../Module-Misc2/KernelStruct.h"
#include "PlatformDataSetList.hpp"


#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_Instruments
{
public:
    CPlatformDataMgr_Instruments(bool bUseComb=false);
    ~CPlatformDataMgr_Instruments(void);



    ///ctp测试环境下，从合约名称解析出交易所ID
    const string Instrument2ExchangeID(const string& strInstrument);
    //设置指定合约信息
    void SetInstrumentInfo(const string& strInstrumentID, const PlatformStru_InstrumentInfo& inData, bool bCTPSimulate, bool bLast);
    ///获取指定合约信息
    bool GetInstrumentInfo(const string& strInstrumentID, PlatformStru_InstrumentInfo& outData);
    void GetInstrumentInfos(vector<PlatformStru_InstrumentInfo> &outData,const string ExchangeID="");

    ///指定合约是否合法(存在)
    bool InstrumentIsValid(const string& strInstrumentID);
    ///获取包含此单腿合约的组合合约列表
    bool GetCombInstrumentIDs_IncludeLeg(const string& LegInstrument,std::vector<string>& vecCombInstruments);
    ///获取合约的产品类型，失败返回-1
    char GetProductClassType(const string& InstrumentID);
    ///获取合约的ProductID
    string GetProductID(const string& strInstrumentID);
    ///获取合约的交易所
    bool GetExchangeID(const string& InstrumentID,string& outExchangeID);
    string GetExchangeID(const string& InstrumentID);
    ///获取合约列表
    void GetInstrumentList(std::vector<InstrumentGroupInfo> &outData);
    ///获取合约列表
    void GetInstrumentList(std::set<string> &outData,const string ExchangeID="");
    void GetInstrumentList(vector<string> &outData,const string ExchangeID="");
    ///获取合约列表
    int GetMainInstrumentList(std::set<string> &outData);
    //添加主力合约列表
    void AddMainInstrumentList(string instrument);
    ///设置合约容差列表
    void SetInstrumentVarietyMap(const std::map<string,string>& inData);
    // 获取合约容差列表
    void GetInstrumentVarietyMap(std::map<string,string>& outData);
    ///获取指定品种的合约列表
    void GetInstrumentListByProductID(const string& ProductID,set<string> &outData);
    void GetInstrumentListByProductID(const string& ProductID,vector<string> &outData);
    ///获取指定交易所的合约-品种
    void GetProductID_InstrumentIDsByExchangeID(std::map<string,std::set<string>>& outData,const string ExchangeID="");
    ///获取全部交易所的品种合约ID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
    void GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData);
    ///获取全部交易所的品种信息 vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
    void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData);
    ///获取合约乘数，成功返回合约乘数，失败返回-1
    int GetInstrumentMultiple(const string& InstrumentID);
    //将两个单腿合约合成组合合约，返回成功与否。合成时会检查合约列表里是否有该组合合约
	bool GetComboInstrumentID(  const string strInstrumentID1, 
                                const string strInstrumentID2, 
                                string& strComInstrumentID,
                                string& strLeg1InstrumentID,
                                string& strLeg2InstrumentID);
	bool GetLegInstrumentID(    const string& strComInstrumentID,
                                string& strLeg1InstrumentID,
                                string& strLeg2InstrumentID);

    //设置指定产品信息
    void SetProductInfo(const string& strProductID, const PlatformStru_ProductInfo& inData);
    //从产品列表中删除没有合约的产品. 查询合约结束后执行一次. 实盘里发现有这种情况，出现有过期的产品
    void DeleteEmptyProduct();
    ///获取品种列表
    int GetProductList(vector<string> &outData,const string ExchangeID="");
    bool GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData);
    bool GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData);
    void GetProductInfos(vector<PlatformStru_ProductInfo> &outData,const string ExchangeID="");


    //获取交易所的交易日
    string GetTradingDay(void);
    //判断一个字符串日期是否是当天
    bool IsToday(const char *OpenDate);
    //获取交易所时间信息
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    //获取交易所时间
    int GetExchangeTime(ExchangeTime& outData);
    //设置交易所时间信息
    void SetExchangeTime(const ExchangeTime& inData);
    //获取全部交易所
    int GetExchangeIDs(std::vector<string>& outData);

    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);

    //获取指定交易所的期权标的物期货品种，返回目标品种ID的数量
    int GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs);
    //获取指定标的物期货合约对应的期权合约ID，返回目标期权合约的数量。UnderlyingInstrumentID为空则返回全部期权合约
    int GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs);
    //获取指定标的物期货合约对应的看涨期权合约ID数组和看跌期权合约ID数组。UnderlyingInstrumentID为空则返回全部期权合约
    void GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs);
    //获取所有标的物期货合约
    int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs);
    //获取所有标的物期货合约
    int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs);
    //判断是否是期权标的物期货合约
    bool IsUnderlyingInstrumentID(const string& InstrumentID);

private:
    Ceasymutex				                    m_mutex;
    bool                                        m_bUseComb;                     //是否使用组合单功能

    //产品信息
    map<string, PlatformStru_ProductInfo>       m_allProducts;                  //ProductID -> ProductInfo

    //交易所ID->品种IDs
    map<string,set<string>>                     m_ProductIDsByExchangeID;       //ExchangeID -> set of ProductIDs

    //交易所ID->期权标的物品种ID
    multimap<string,string>                     m_OptionUnderlyingProductIDsByExchangeID;       //ExchangeID -> Underlying ProductIDs of option
    //标的物期货合约->期权合约
    multimap<string,string>                     m_OptionInstrumentIDsByUnderlyingInstrumentID;  //UnderlyingInstrumentID -> OptionInstrumentIDs
    multimap<string,string>                     m_CallOptionInstrumentIDsByUnderlyingInstrumentID;  //UnderlyingInstrumentID -> CallOptionInstrumentIDs
    multimap<string,string>                     m_PutOptionInstrumentIDsByUnderlyingInstrumentID;   //UnderlyingInstrumentID -> PutOptionInstrumentIDs

    //合约信息：下面三个数据集在查询合约信息结果中得到，或者由gui设置
    map<string, PlatformStru_InstrumentInfo>    m_allInstruments;               //当前所有合约信息，系统初始化时获取. InstrumentID -> InsrumentInfo
    set<string>                                 m_allInstrumentList;            //当前所有合约列表. set<InstrumentID>
    set<string>                                 m_allInstrumentListWithoutComb; //当前所有合约列表（排除组合合约）. set<InstrumentID>
    map<string, set<string>>                    m_allInstrumentIDsByProductID;  //所有产品代码. ProductID -> set<InstrumentID>
    multimap<string,string>                     m_Leg2Comb;                     //单腿合约到组合合约的对应表. LegInstrumentID -> CombInstrumentID
    map<string,string>                          m_Legs2Comb;                    //Leg1&Leg2 -> ComInstrumentID，如c1309&c1401 -> SP c1309&c1401

    set<string>                                 m_allMainInstrumentList;        //当前主力合约列表. set<InstrumentID>

	std::map<string, string>                    m_allInstrumentVarietyMap;      //用于对用户输入的合约代码进行容错性转换

    ExchangeTime                                m_exchangeTime;                 //交易所时间

};

#ifdef WIN32
#pragma managed(pop)
#endif 
