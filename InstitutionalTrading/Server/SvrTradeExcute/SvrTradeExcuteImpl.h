#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
#include "CommonStruct.h"
#include "Interface_SvrTradeExcute.h"
#include "KernelStruct.h"
#include "easymutex.h"




class CSvrTradeExcuteImpl : public CInterface_SvrTradeExcute
{
public:
	static CSvrTradeExcuteImpl& getObj(void);


	//风控下强平单
	virtual CF_ERROR RiskForceCloseOrder(PlatformStru_InputOrder & nInputOrder,
		const InputOrderKey& lKey,
		int nUserID,
		const std::string & nsForceReason) ;

	//风控撤挂单
	virtual CF_ERROR RiskForceActionOrder( PlatformStru_InputOrderAction & nInputOrderAction,
		int nUserID,
		int nRequestID,
		const std::string & nsActionReson) ;

	//风控模块调用，写入各种下单限制
	//风控调用
	virtual bool RiskControl(const std::string &nUserName, 
		std::multimap<RiskWarningType, SRiskControl>& vecRiskControl);


	//审核保单
	virtual  CF_ERROR SetVerifyOrder(OrderKey& orderKey, bool bPassVerify) ;
	////设置合约限制
	//void SetRiskInstrumentLimit(const std::map<std::string, std::set<std::string>>& mapRiskInstrumentLimit);

	////设置风险指标临时生成的下单限制
	//void AddRiskIndOrderLimit(const std::string &nUserName,
	//	RiskLimit limit, 
	//	std::set<std::string> setLimitInstruments);

	//void DelRiskIndOrderLimit(const std::string &nUserName);

	//交易策略模块调用，写入策略下单限制
	//void SetStrategyOrderLimit(const std::map<std::string, std::map<std::string, StrategyOrderLimit>>& mapStrategyOrderLimit);

	void SetOneStrategyOrderLimit(const std::string &nUserName, const std::string & nInstrumentID,StrategyOrderLimit nCtrl);	


    //预处理下单，判断用户启用，用户合约限制，用户是否需要手工审核 
	CF_ERROR PreFilterOrder(int nUserId,
		const PlatformStru_OrderInfo& order, 
		bool& bNeedVerify);

	//基础分控，
	CF_ERROR BaseFilterOrder(const std::string &nUserName,
		const PlatformStru_OrderInfo& order, 
		const bool nbNeedPushOrderInfoToTraderData);


	//分控和交易策略
	CF_ERROR AfterFilterOrder(const std::string &nUserName,
		const PlatformStru_OrderInfo& order, 
		const bool nbNeedPushOrderInfoToTraderData);



	//撤单相关处理
	int CancelOrder(InputOrderKey key);
	//撤单响应
	int CancelOrderReturn(InputOrderKey key);

	void AddVerifyingOrder( InputOrderKey key, PlatformStru_InputOrder order);
	bool GetInputOrder(InputOrderKey key, PlatformStru_InputOrder& order);
	void CancelOrderDeal(PlatformStru_OrderInfo & orderInfo,CF_ERROR nErrorCode,bool nbRiskEvent);
	void UpdateOrderDeal(PlatformStru_OrderInfo & orderInfo);
private:
	bool LoadDB();
	//判断交易员是否允许下单
	bool EnableOrder(int nTraderID, int nTradeAccountID);

	//判断是否需要手工审核
	bool NeedVerify(int nTraderID, int nTradeAccountID);
	//判断风险指标临时生成的下单限制
	bool RiskIndLimited(const std::string &nUserName,const PlatformStru_OrderInfo& order,CF_ERROR & nErrorCode);
	//判断下单策略限制
	bool StrategyLimited(const std::string &nUserName,std::string strInstrumentID, char Direction);

private:
	CSvrTradeExcuteImpl();
	virtual ~CSvrTradeExcuteImpl();
	static CSvrTradeExcuteImpl* m_pObj;

private:
	//下单结构
	
	//风控模块依据风险指标临时生成的下单限制	
	std::map<std::string, std::multimap<RiskWarningType, SRiskControl>> m_mapRiskLimited;
	Ceasymutex mMutexRisk;

	//交易策略下单限制
	std::map<std::string, std::map<std::string, StrategyOrderLimit>> m_mapStrategyOrderLimit;
	Ceasymutex mMutexStategy;
};
