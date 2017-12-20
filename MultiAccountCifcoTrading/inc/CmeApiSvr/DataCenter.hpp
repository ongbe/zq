/***************************************************************************//**
*   @file          DataCenter.hpp
*   @brief        数据中心，存储综合交易平台发过来的业务数据
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/

#ifndef _DATACENTER_HPP_
#define _DATACENTER_HPP_

#include <map>
#include <list>
#include <set>
#include <string>
#include "../inc/ISvr/BusinessData.hpp"
#include "wx/thread.h"
#include "PlatformDataMgr.h"


//#include "InvestorPositionDetailMgr.h"

class CPackageCache;
class zqWriteLog;
class CPlatFormService;
class CDataCenter
{
friend class CDataCenterWorkThread;
public:
    explicit CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost);
    ~CDataCenter();

    ///定制业务数据
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///取消业务数据的定制
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ///获取合约列表
    int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
	int GetInstrumentList(std::vector<GroupInfo> &outData);
    ///添加主力合约列表
    int AddMainInstrumentList(std::string instrument);
    ///获取主力合约列表
    int GetMainInstrumentList(std::set<std::string> &outData);
	///设置合约容差列表
	void SetInstrumentVarietyMap(map<string, string>& outData);
	// 获取合约容差列表
	int GetInstrumentVarietyMap(map<string, string>& outData);
    ///获取指定合约信息
    int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///设置指定合约信息
    void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///获取合约的产品类型，失败返回-1
    char GetProductClassType(const std::string& InstrumentID);

    ///获取合约品种代码
    int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

    ///获取合约手续费率
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///设置合约手续费率
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///设置正在查询手续费率的合约
    void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
    ///获取合约保证金率
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///设置合约保证金率
    int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);   
    ///获取合约乘数，成功返回合约乘数，失败返回-1
    int GetInstrumentMultiple(const std::string& InstrumentID);
	///获取指定品种的合约列表,返回合约个数
	int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData);

    ///获取指定合约行情
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///获取指定合约旧行情
    int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///获取指定合约的现价，失败返回0.0
    double GetCurPrice(const std::string& InstrumentID);

	string GetTradingDay();
    ///获取交易所时间
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///获取全部报单
    int GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData);
    //获取指定合约、指定方向、指定开平的挂单数量（手数）
	int GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    ///获取全部成交单
    int GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData);
    ///获取成交统计
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///在成交统计查找参数rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );

    ///获取交易资金账户信息
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);
    ///设置交易资金账户信息
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);

	///请求查询投资者结算结果响应
    int GetLastSettlementInfo(std::string& outData);
    
    ///启动
    int Start();

    ///停止,定制信息保留，再次启动之后，定制依然有效
    int Stop();

    ///获取所有委托单信息
    int GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap);

    ///获取指定合约的持仓明细
    int GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
    int GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
	int GetPositionDetails(std::list<PositionDetailStru> &pPositionDetailList);
    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeyList(std::list<PositionKey> &PositionKeyList);
    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData);

	//发送组合合约深度行情数据
	int MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index=0);
	int InitCombRelation();
    bool IsNeedCheckCommissionRateAndMarginRate(void);
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck);

	SERVER_PARAM_CFG&  GetServerParamCfg();
private:
    int doProcessBusinessData(AbstractBusinessData& data);
private:
    CPackageCache&     m_dataSource;
    zqWriteLog*         m_pWriteLog;
    CDataCenterWorkThread* m_pWorkThread;
    wxMutex            m_mutex;



    CPlatformDataMgr* m_pDataMgr;
	CPlatFormService* m_PlatFormService;



};



#endif

