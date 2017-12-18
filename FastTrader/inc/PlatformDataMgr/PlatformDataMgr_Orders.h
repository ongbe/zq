/*
    用于保存和维护CTP的报单数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
using std::vector;
using std::map;
using std::multimap;
using std::set;
using std::string;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgr_Quots.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Orders
{
public:
    //DiscountDCEComb   :大商所组合单优惠策略，0-不优惠，1-第一腿优惠，2-第二腿优惠(缺省)
    //DiscountCZCEComb  :郑商所组合单优惠策略，0-不优惠，1-第一腿优惠，2-第二腿优惠(缺省)
    //bFreezeOrderUnknownStatus: 对状态为未知的报单(如手工审核单)，是否执行资金冻结
    CPlatformDataMgr_Orders(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates,
        CPlatformDataMgr_Quots& refQuots,
        int DiscountCZCE_Comb=3,
        int DiscountDCE_Comb=3,
        bool bFreezeOrderUnknownStatus=false);
    ~CPlatformDataMgr_Orders(void);


    //----------下面是修改报单接口----------
public:
    //查询的报单到来
    void QryOrderReach(PlatformStru_OrderInfo& OrderInfo,int ErrID,const bool bIsLast);

	//新报单到来
    void NewOrderReady(PlatformStru_OrderInfo& OrderInfo, PlatformStru_OrderInfo& OldOrderInfo, const PlatformStru_InstrumentInfo& InstrumentInfo);

    //更新所有报单中的冻结保证金，返回合约编号是否有报单，决定是否需要更新资金面板。查询保证金率响应时调用
    bool UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);

    //新所有报单中的冻结手续费，返回合约编号是否有报单，决定是否需要更新资金面板。查询手续费率响应时调用
    bool UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(const std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);

    void ClearMapQryRltOrders();
    //清除所有数据
    void clear(void);
    //判断报单是否在工作状态，含挂单状态和审核中(未知状态)
    static bool IsOrderWorking(const PlatformStru_OrderInfo& OrderInfo);
    //判断报单是否在挂单状态
    static bool IsOrderWaiting(const PlatformStru_OrderInfo& OrderInfo);
    //判断报单是否在触发状态
    bool IsOrderTrigger(const PlatformStru_OrderInfo& OrderInfo);
    //判断报单是否已成交，含部分成交和全部成交
    bool IsOrderTraded(const PlatformStru_OrderInfo& OrderInfo);
    //判断报单是否是撤单/错单
    bool IsOrderCanceled(const PlatformStru_OrderInfo& OrderInfo);


private:
    //计算报单信息中的冻结保证金。含未知状态的报单
    void CalcuOrderFreezeMargin_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //计算单合约期权报单信息中的冻结保证金. 含未知状态的报单。未考虑组合期权之情况
    //需要用到行情中的昨结算价
    //每手卖方交易保证金 = MAX(权利金 + 投资者期权合约保证金不变部分, 投资者期权合约最小保证金)
    //权利金 = 价格（冻结使用昨结算价）*期权合约乘数
    void CalcuOrderFreezeMargin_Option_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //计算组合合约报单中的冻结保证金。含未知状态的报单
    //由于报单中没有分腿报价，用昨结算价计算分腿冻结保证金
    void CalcuOrderFreezeMarginComb_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& Leg1InstrumentInfo,
        const PlatformStru_InstrumentInfo& Leg2InstrumentInfo);

    //计算报单信息中的冻结手续费。含未知状态的报单
    void CalcuOrderFreezeCommission_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //计算组合合约报单中的冻结手续费。需要按双边分别计算。含未知状态的报单
    //由于报单中没有分腿报价，用昨结算价计算分腿冻结手续费
    void CalcuOrderFreezeCommissionComb_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& Leg1InstrumentInfo,
        const PlatformStru_InstrumentInfo& Leg2InstrumentInfo);

    //----------下面是获取报单接口----------
public:
    ///获取指定报单
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    ///获取指定报单
    bool GetOrder2(const std::string& strOrderSysID,PlatformStru_OrderInfo& outData);
    ///获取全部报单
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取全部报单
    int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);

    ///获取全部已触发的报单
    int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取合约相关的已触发的报单。合约是单合约，报单是对应报单，或包含该合约的组合报单
    int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已触发的报单
    bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取已成交报单
    int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取已成交报单。合约是单合约，报单是对应报单，或包含该合约的组合报单
    int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已成交报单
    bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取已撤单和错误报单
    int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取已撤单和错误报单
    int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已撤单和错误报单
    bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取未成交报单, 不包含手工审核中的报单
    int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取未成交报单, 不包含手工审核中的报单
    int GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定合约相关的未成交报单, 不包含手工审核中的报单，合约是单合约，报单是对应报单，或包含该合约的组合报单
    int GetWaitOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData);
    ///获取指定未成交报单, 不包含手工审核中的报单
    bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取状态为未知的报单, 如手工审核中报单
    int GetUnknownStatusOrders(std::vector<PlatformStru_OrderInfo>& outData);

    //获取指定合约、指定方向、指定开平的挂单数量（手数）, 不包含手工审核中的报单
    int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, char Direction, char CloseType,char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, char Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。内部调用，不加锁。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
	int GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, char Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,char HedgeFlag=THOST_FTDC_HF_Speculation);
    //取全部挂单的冻结手续费和冻结保证金。如果没有费率，为无效值。含状态未知报单
    void GetFreezeMargin_Commission(double& FreezeMargin,double& FreezeCommission);
    //获取指定合约指定方向的的冻结金额，包括同方向的开仓单和反方向的平仓单。含状态未知报单。(考虑组合单对单腿合约的影响)
    //strInstrumentID为单腿合约
    void GetFreezeMargin_Commission2(const std::string& strAccount,const std::string &strInstrumentID, char Direction,char HedgeFlag,
        double& FreezeMargin,double& FreezeCommission);

	//为插件添加以下接口
	//获取委托单列表FTID
	int GetAllOrderFTIDList(std::vector<long>& vec);
	//获取委托单详情
	int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
	int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);

    //取所有挂单的合约列表
    void GetInstrumentIDs_Working(set<string>& setInstrumentID);
    //取等待行情进行冻结保证金和手续费计算的合约集
    void GetInstrumentIDs_WaitQuot(set<string>& setInstrumentID);

    //判断是否为空
    bool empty(void);

    ///获取大于等于MinFTID的全部数据，返回数据项的数量。结果放在数组中。m_bUpdateSeq2FTID为true时有效
    int GetAll_GE_UpdateSeq(long MinUpdateSeq,vector<PlatformStru_OrderInfo>& outDatas);

private:
    //获取和合约相关的报单FTID集合，包括单腿为该合约的组合报单
    void GetFTIDSetOfInstrument2_Internal(const string& strInstrument,set<long>&FTIDs);

private:

    Ceasymutex				        m_mutex;

    CPlatformDataMgr_Instruments    &m_RefInstruments;
    CPlatformDataMgr_Rates          &m_RefRates;
    CPlatformDataMgr_Quots          &m_RefQuots;

    //组合单优惠策略，0-不优惠，1-第一腿优惠，2-第二腿优惠，3-按保证金比较高的一边收取(缺省)
    int                             m_DiscountDCE_Comb;      
    int                             m_DiscountCZCE_Comb;

    static bool                     m_bFreezeOrderUnknownStatus;//对于状态为未知的报单，是否需要冻结

    //----------下面是报单数据集----------
    CPlatformDataSetList<OrderKey,PlatformStru_OrderInfo> m_OrderData;

	//OrderSysID 到 OrderKey 的映射关系。主要用于通过OrderSysID查找对应的OrderKey
    map<string, OrderKey> m_mapOrderSysID2OrderKey;

    multimap<string,long> m_mapLegID12CombOrderFTID;//第一腿合约到组合报单FTID的对应关系
    multimap<string,long> m_mapLegID22CombOrderFTID;//第二腿合约到组合报单FTID的对应关系

    //报单分类数据结构，通常用于计算保证金，考虑上期大额单边优惠
    struct Stru_OrderCategory
    {
        Stru_OrderCategory(){};
        Stru_OrderCategory(const string& account,const string& instrumentorproduct,TThostFtdcHedgeFlagType hedgeFlag,TThostFtdcDirectionType direction)
        {
            set(account,instrumentorproduct,hedgeFlag,direction);
        };
        Stru_OrderCategory& operator= (const Stru_OrderCategory& r)
        {
            Account=r.Account;
            InstrumentOrProduct=r.InstrumentOrProduct;
            HedgeFlag=r.HedgeFlag;
            Direction=r.Direction;
            return *this;
        };
        void set(const string& account,const string& instrumentorproduct,TThostFtdcHedgeFlagType hedgeFlag,TThostFtdcDirectionType direction)
        {
            Account=account;
            InstrumentOrProduct=instrumentorproduct;
            HedgeFlag=hedgeFlag;
            Direction=direction;
        };
        bool operator< (const Stru_OrderCategory& r) const
        {
            if(Account<r.Account) return true;
            else if(Account>r.Account) return false;
            if(InstrumentOrProduct<r.InstrumentOrProduct) return true;
            else if(InstrumentOrProduct>r.InstrumentOrProduct) return false;
            if(HedgeFlag<r.HedgeFlag) return true;
            else if(HedgeFlag>r.HedgeFlag) return false;
            if(Direction<r.Direction) return true;
            else if(Direction>r.Direction) return false;
            return false;
        }
        bool operator== (const Stru_OrderCategory& r) const
        {
            return Account==r.Account&&InstrumentOrProduct==r.InstrumentOrProduct&&HedgeFlag==r.HedgeFlag&&Direction==r.Direction ? true : false;
        }
        bool operator> (const Stru_OrderCategory& r) const
        {
            return !((*this) < r || (*this) == r);
        }


        string                  Account;            //投资者账号
        string                  InstrumentOrProduct;//按合约分类或按产品分类
	    TThostFtdcHedgeFlagType	HedgeFlag;          //投保
	    TThostFtdcDirectionType	Direction;          //方向
    };
    //按合约分类的保证金。m_TotalMargin有效则分类数据有效。m_mapInstrumentExchMargin是交易所保证金，用于在大额单边优惠时决定大边
    map<Stru_OrderCategory,double> m_mapInstrumentMargin;
    map<Stru_OrderCategory,double> m_mapInstrumentExchMargin;
    bool                        m_bmapInstrumentMarginOK;
    double                      m_TotalMargin;
    double                      m_TotalCommission;

    //更新全部冻结保证金
    void UpdateAllMargin_Internal(void);
    //更新全部冻结手续费
    void UpdateAllCommission_Internal(void);
    //根据报单变化，调整保证金
    void UpdateMarginByOrder_Internal(const PlatformStru_OrderInfo& Order,const PlatformStru_OrderInfo& OldOrder);
    //根据报单变化，调整手续费
    void UpdateCommissionByOrder_Internal(const PlatformStru_OrderInfo& Order,const PlatformStru_OrderInfo& OldOrder);

    //将所有冻结保证金按合约及方向分类统计
    void ClassifyAllMarginByInstrumentID_Internal(void);
    //根据保证金分类数据，计算总冻结保证金
    void CalcuTotalMargin_Internal(void);


};

#ifdef WIN32
#pragma managed(pop)
#endif 
