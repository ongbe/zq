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
#include <set>
#include <string>
#include <vector>
using std::map;
using std::set;
using std::string;
using std::vector;

#include "../Module-Misc2/BusinessData.hpp"
#include "wx/thread.h"
#include "PlatformDataMgr.h"
#include "easymutex.h"
#include "myxml.h"



class CPackageCache;
class zqWriteLog;
class CPlatFormService;
class CDataCenter
{
friend class CDataCenterWorkThread;
public:
    explicit CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost,int PlatformID,int MaxPlatformID);
    ~CDataCenter();

    ///定制业务数据
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///取消业务数据的定制
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ////从xml节中取出经纪公司交易参数
    //void GetBrokerTradingParamsFromXML(CMyXML& pnodexml);

    ///获取合约列表
    int GetInstrumentList(set<string> &outData,string ExchangeID="");
    int GetInstrumentList(vector<string> &outData,string ExchangeID="");
	int GetInstrumentList(std::vector<InstrumentGroupInfo> &outData);
    ///添加主力合约列表
    void AddMainInstrumentList(string instrument);
    ///获取主力合约列表
    int GetMainInstrumentList(set<string> &outData);
	///设置合约容差列表
	void SetInstrumentVarietyMap(map<string, string>& outData);
	// 获取合约容差列表
	int GetInstrumentVarietyMap(map<string, string>& outData);
    ///获取指定合约信息
    int GetInstrumentInfo(const string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    bool GetLegInstrumentID(const string& strComInstrumentID,
        string& strLeg1InstrumentID,
        string& strLeg2InstrumentID);
    ///设置指定合约信息
    void SetInstrumentInfo(const string& InstrumentID, PlatformStru_InstrumentInfo& outData,bool bLast);
    ///获取合约的产品类型，失败返回-1
    char GetProductClassType(const string& InstrumentID);
    ///获取合约的ProductID
    string GetProductID(const string& strInstrumentID);
    string GetExchangeID(const string& strInstrumentID);

    ///设置指定品种信息
    void SetProductInfo(const string& ProductID, PlatformStru_ProductInfo& outData);
    ///获取品种列表
    int GetProductList(vector<string> &outData,const string& ExchangeID="");

    ///获取包含此单腿合约的组合合约列表
    bool GetCombInstrumentIDs_IncludeLeg(const string& LegInstrument,std::vector<string>& vecCombInstruments);

    ///获取指定交易所的所有品种-合约
    int GetProductID_InstrumentIDsByExchangeID(map<string, set<string> >& outData,string ExchangeID="");    
    ///获取全部交易所的品种合约ID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
    void GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData);
    ///获取全部交易所的品种信息 vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
    void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData);
    bool GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData);
    bool GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData);
    //获取指定交易所的期权标的物期货品种
    int GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs);
    //获取指定标的物期货合约对应的期权合约ID，返回目标期权合约的数量。UnderlyingInstrumentID为空则返回全部期权合约
    int GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs);
    //获取指定标的物期货合约对应的看涨期权合约ID数组和看跌期权合约ID数组。UnderlyingInstrumentID为空则返回全部期权合约
    void GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs);
    //获取所有标的物期货合约
    int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs);
    int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs);
    //判断是否是期权标的物期货合约
    bool IsUnderlyingInstrumentID(const string& InstrumentID);

    ///获取合约手续费率
    int GetCommissionRate(const string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    bool IsCommissionRateExist(const string& InstrumentID);
    ///设置合约手续费率
    int SetCommissionRate(const string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///设置正在查询手续费率的合约
    void SetReqCommissionRateInstrumentID(const string& InstrumentID);
    ///获取合约保证金率
    int GetMarginRate(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    bool IsMarginRateExist(const string& InstrumentID);
    ///设置合约保证金率
    int SetMarginRate(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);   
    ///获取合约乘数，成功返回合约乘数，失败返回-1
    int GetInstrumentMultiple(const string& InstrumentID);
	///获取指定品种的合约列表,返回合约个数
	int GetInstrumentListByProductID(const string& ProductID,set<string> &outData);
	int GetInstrumentListByProductID(const string& ProductID,vector<string> &outData);

    ///获取指定合约行情
    int GetQuotInfo(const string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///获取指定合约旧行情
    int GetOldQuotInfo(const string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///获取指定合约的现价，失败返回无效值
    double GetCurPrice(const string& InstrumentID);

    ///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
    void SetCurrentPositionContent(int PositionContentMode);

	string GetTradingDay();
	bool IsTodayPosition( const char* pOpenData );
    ///获取交易所时间
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///获取交易所日内交易时段, 返回交易时段起始时间对的数组，如{pair("0915","1130"),pair("1300","1515")}等
    vector<pair<string,string>> GetTradingTimespan(const string& ExchangeID);
	//获取全部交易所
	int GetExchangeIDs(vector<string>& outData);
    ///获取全部报单
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取全部报单
    int GetAllOrders2(const string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定报单
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    bool GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData);

    ///获取全部已触发的报单
    int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取合约相关的已触发的报单
    int GetTriggerOrders2(const string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已触发的报单
    bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取已成交报单
    int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取已成交报单
    int GetTradedOrders2(const string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已成交报单
    bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取已撤单和错误报单
    int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取已撤单和错误报单
    int GetCanceledOrders2(const string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已撤单和错误报单
    bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取未成交报单
    int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取未成交报单
    int GetWaitOrders3(const string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定合约相关的未成交报单, 不包含手工审核中的报单，合约是单合约，报单是对应报单，或包含该合约的组合报单
    int GetWaitOrders2(const string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定未成交报单
    bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


    //获取指定合约、指定方向、指定开平的挂单数量（手数）
	int GetWaitOrderVolume(const string& strAccount, const string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const string& strAccount,const string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    ///获取全部成交单
    int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
    ///获取指定成交信息
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    ///获取指定成交信息
    int GetTradesOfInstrument(const string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);
    int GetTradeBriefsOfInstrument(const string& strInstrument,std::vector<PlatformStru_TradeInfoBrief>& outData);
    ///获取全部成交统计记录
    int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///获取指定合约的成交统计记录成功返回0，失败返回-1
    int GetTradeTotalOfInstrument(const string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///获取成交统计
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///在成交统计查找参数rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );

    ///获取交易资金账户信息
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID,int rltsize);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);
	///获取账户资金文本信息
    int GetAccountText(string& outData,int language);

	///请求查询投资者结算结果响应
    int GetLastSettlementInfo(string& outData);
	 //获取客户签约银行信息
    int GetContractBank(map<string,  PlatformStru_ContractBank>& outData);
    
    ///启动
    int Start();

    ///停止,定制信息保留，再次启动之后，定制依然有效
    int Stop();


    ///获取指定合约的持仓明细
    bool HavePositionDetail(const string& strInstrumentID);
    bool HaveCombPositionDetail();
    void GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs);

    int GetPositions2(const string& strInstrument,
                        set<long>& setFTID,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    int GetPositions3(const string& strInstrument,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    int GetPositionDetails3(const string& strInstrument,
                              set<long>& setFTID,
                              std::vector<PlatformStru_PositionDetail>& vecValue,
                              long& lastseq);
    int GetPositionCombs2(const string& strInstrument,std::vector<PlatformStru_Position>& outData);

    int GetPositions(std::vector<PlatformStru_Position>& outData);
    int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    int GetPositionCombs(std::vector<PlatformStru_Position>& outData);


    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(set<PositionKey> &PositionKeySet);
    //获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
    int GetPositionData(const string& strAccount, const string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);


    bool IsNeedCheckCommissionRateAndMarginRate(void) { return m_pDataMgr->m_Rates.IsNeedCheckCommissionRateAndMarginRate();}
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) { m_pDataMgr->m_Rates.SetNeedCheckCommissionRateAndMarginRate(bNeedCheck);}
	SERVER_PARAM_CFG&  GetServerParamCfg();
    //void SetModuleDataQry(bool bQry);
    //bool GetModuleDataQry();

    void ClearMapQryRltOrders();
    void ClearMapQryRltTrades();
    void ClearMapQryRltPositionDetails(string& strQryInstrument);
    void ClearMapQryRltPositions(string& strQryInstrument);
    void ClearMapQryRltPositionDetailComb(string& strQryInstrument);

    //UI层通知底层，初始化查询开始或完成
    void NotifyInitQryStart();
    void NotifyInitQrySucc();

	//为插件添加以下接口
	//获取委托单列表FTID
	int GetAllOrderFTIDList(std::vector<long>& vec);
	//获取委托单详情
	int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
	int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);

	//获取持仓列表FTID
	int GetPositionFTIDList(std::vector<long>& vec);
	//获取持仓详情
	int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
	int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);

	//获取组合持仓列表FTID
	int GetPositionCombFTIDList(std::vector<long>& vec);
	//获取组合持仓详情
	int GetPositionCombInfo(long lFTID, PlatformStru_Position& outData);

    //获取交易连接的FrontID和SessionID。这两个值在交易登录时由ctp返回
    const PlatformStru_LoginInfo& GetTradingLoginInfo();

    //获取币种信息
    vector<string> GetCurrencys(const string& BankName,bool bAddUSD=true,bool bAddCNY=true);
    vector<string> GetCurrencys(bool bAddUSD=true,bool bAddCNY=true);

public:
    //是否已经得到BID_RspQrySettlementInfoConfirm，表示DataCenter已经和底层同步上，可以开始初始化查询了。
    bool bRspQrySettlementInfoConfirm;


private:
    int doProcessBusinessData(AbstractBusinessData& data);

private:
    CPackageCache&          m_dataSource;
    zqWriteLog*             m_pWriteLog;
    CDataCenterWorkThread*  m_pWorkThread;

    CPlatformDataMgr*       m_pDataMgr;
	CPlatFormService*       m_PlatFormService;


    //业务定制信息
    map<BusinessID, map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;

    Ceasymutex              m_mutex;

public:
    string                  m_LoginInvestorID;


};



#endif

