#pragma once
#include <string>
#include "ThostFtdcTraderApi.h"
#include "KernelStruct.h"

#pragma pack(push)
#pragma pack(1)

//登入请求响应
struct DataRonUserLogin
{
	DataRonUserLogin(const CThostFtdcRspUserLoginField& userLoginField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		: UserLoginField(userLoginField),
		  RspInfoField(RspInfo),
		  nRequestID(requestID),
		  bIsLast(bisLast)
	{
	};


	CThostFtdcRspUserLoginField UserLoginField;
	CThostFtdcRspInfoField      RspInfoField; 
	int                         nRequestID;
	bool                        bIsLast;
};

//登出请求响应
struct DataRonUserLogout
{
	DataRonUserLogout(const CThostFtdcUserLogoutField& userLogoutField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:       UserLogoutField(userLogoutField),
				RspInfoField(RspInfo),
				nRequestID(requestID),
				bIsLast(bisLast)
	{
	};

    CThostFtdcUserLogoutField UserLogoutField;
	CThostFtdcRspInfoField    RspInfoField; 
	int                       nRequestID;
	bool                      bIsLast;
	
};

//用户口令更新请求响应
struct DataRonUserPasswordUpdate
{
	DataRonUserPasswordUpdate(const CThostFtdcUserPasswordUpdateField& userPasswordUpdateField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:       UserPasswordUpdateField(userPasswordUpdateField),
		        RspInfoField(RspInfo),
		        nRequestID(requestID),
		        bIsLast(bisLast)
	{
	};

	CThostFtdcUserPasswordUpdateField UserPasswordUpdateField;
	CThostFtdcRspInfoField            RspInfoField; 
	int                               nRequestID;
	bool                              bIsLast;

};

//资金账户口令更新请求响应
struct DataRonTradingAccountPasswordUpdate
{
	DataRonTradingAccountPasswordUpdate(const CThostFtdcTradingAccountPasswordUpdateField& tradingAccountPasswordUpdateField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:       TradingAccountPasswordUpdateField(tradingAccountPasswordUpdateField),
		        RspInfoField(RspInfo),
		        nRequestID(requestID),
		        bIsLast(bisLast)
	{
	};

	CThostFtdcTradingAccountPasswordUpdateField TradingAccountPasswordUpdateField;
	CThostFtdcRspInfoField                      RspInfoField; 
	int                                         nRequestID;
	bool                                        bIsLast;

};

//报单录入回执(分为成功回执与错误回执)
struct DataRonOrderInsert
{
	DataRonOrderInsert()
		: nRequestID(0),
		  bIsLast(true)
	{
		memset(&InputOrderField,0,sizeof(InputOrderField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonOrderInsert(const CThostFtdcInputOrderField& OrderField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InputOrderField(OrderField),
			RspInfoField(RspInfo),
			nRequestID(requestID),
			bIsLast(bisLast)
	{
	};


	CThostFtdcInputOrderField InputOrderField;
	CThostFtdcRspInfoField    RspInfoField;  
	int                       nRequestID;
	bool                      bIsLast;  
};

//预埋单录入回执(分为成功回执与错误回执)
struct DataRonParkedOrderInsert
{
	DataRonParkedOrderInsert()
		:  
		nRequestID(0),
		bIsLast(true)
	{
		memset(&ParkedOrder,0,sizeof(ParkedOrder));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonParkedOrderInsert( const CThostFtdcParkedOrderField& parkedOrder,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		 :     ParkedOrder(parkedOrder),
			   RspInfoField(RspInfo),
			   nRequestID(requestID),
			   bIsLast(bisLast)
	{
	};



	CThostFtdcParkedOrderField ParkedOrder;
	CThostFtdcRspInfoField     RspInfoField;  
	int                        nRequestID;
	bool                       bIsLast;
};

//预埋撤单录入请求响应
struct DataRonParkedOrderAction
{
	DataRonParkedOrderAction()
		:  
		nRequestID(0),
		bIsLast(true)
	{
		memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonParkedOrderAction(const CThostFtdcParkedOrderActionField& parkedOrderAction,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   ParkedOrderAction(parkedOrderAction),
			RspInfoField(RspInfo),
			nRequestID(requestID),
			bIsLast(bisLast)
	{
	};


	CThostFtdcParkedOrderActionField ParkedOrderAction;
	CThostFtdcRspInfoField           RspInfoField;  	
	int                              nRequestID;
	bool                             bIsLast;  
};

//报单操作回执(错误回执1，综合交易平台认为撤单无效)
struct DataRonOrderAction
{
	DataRonOrderAction()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&InputOrderActionField,0,sizeof(InputOrderActionField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonOrderAction(const PlatformStru_InputOrderAction& OrderActionField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InputOrderActionField(OrderActionField),
			RspInfoField(RspInfo),
			nRequestID(requestID),
			bIsLast(bisLast)
	{
	};

	
	PlatformStru_InputOrderAction InputOrderActionField;
	CThostFtdcRspInfoField          RspInfoField; 
	int                             nRequestID;
	bool                            bIsLast;  
};

//查询最大报单数量响应
struct DataRonQueryMaxOrderVolume
{
	DataRonQueryMaxOrderVolume()
		:  nRequestID(0),
		   bIsLast(true)

	{
		memset(&MaxOrderVolumeField,0,sizeof(MaxOrderVolumeField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQueryMaxOrderVolume(const CThostFtdcQueryMaxOrderVolumeField& maxOrderVolumeField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   MaxOrderVolumeField(maxOrderVolumeField),
		    RspInfoField(RspInfo),
	    	nRequestID(requestID),
		    bIsLast(bisLast)
	{
	};


	CThostFtdcQueryMaxOrderVolumeField MaxOrderVolumeField;
	CThostFtdcRspInfoField          RspInfoField; 
	int                             nRequestID;
	bool                            bIsLast;  
};

//投资者结算结果确认响应
struct DataRonSettlementInfoConfirm
{
	DataRonSettlementInfoConfirm()
		:  nRequestID(0),
		   bIsLast(true)

	{
		memset(&SettlementInfoConfirmField,0,sizeof(SettlementInfoConfirmField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonSettlementInfoConfirm(const CThostFtdcSettlementInfoConfirmField& settlementInfoConfirmField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   SettlementInfoConfirmField(settlementInfoConfirmField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)
	{
	};


	CThostFtdcSettlementInfoConfirmField SettlementInfoConfirmField;
	CThostFtdcRspInfoField               RspInfoField; 
	int                                  nRequestID;
	bool                                 bIsLast;  
};
//删除预埋单响应
struct DataRonRemoveParkedOrder
{
	DataRonRemoveParkedOrder()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&RemoveParkedOrder,0,sizeof(RemoveParkedOrder));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonRemoveParkedOrder(const CThostFtdcRemoveParkedOrderField& removeParkedOrder,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   RemoveParkedOrder(removeParkedOrder),
			RspInfoField(RspInfo),
			nRequestID(requestID),
			bIsLast(bisLast)
	{
	};

	CThostFtdcRemoveParkedOrderField RemoveParkedOrder;
	CThostFtdcRspInfoField     RspInfoField; 
	int                       nRequestID;
	bool                      bIsLast;   
};
//删除预埋撤单响应
struct DataRonRemoveParkedOrderAction
{
	DataRonRemoveParkedOrderAction()
		:   nRequestID(0),
		    bIsLast(true)
		
	{
		memset(&RemoveParkedOrderAction,0,sizeof(RemoveParkedOrderAction));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonRemoveParkedOrderAction(const CThostFtdcRemoveParkedOrderActionField& removeParkedOrderAction,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  RemoveParkedOrderAction(removeParkedOrderAction),
		   RspInfoField(RspInfo),
		   nRequestID(requestID),
		   bIsLast(bisLast)
		
		
	{
	};

	
	CThostFtdcRemoveParkedOrderActionField RemoveParkedOrderAction;
	CThostFtdcRspInfoField     RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;

};

//执行宣告录入请求响应
struct DataRonExecOrderInsert
{
	DataRonExecOrderInsert(void)
	{
			nRequestID = 0;
			bIsLast = false;
			memset(&InputExecOrderField, 0, sizeof(InputExecOrderField));
			memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
	DataRonExecOrderInsert(const PlatformStru_InputExecOrderField& inputExecOrderField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  InputExecOrderField(inputExecOrderField),
		   RspInfoField(RspInfo),
		   nRequestID(requestID),
		   bIsLast(bisLast)		
	{
	};
	
	PlatformStru_InputExecOrderField InputExecOrderField;
	CThostFtdcRspInfoField      RspInfoField;   
	int                         nRequestID;
	bool                        bIsLast;
};

//执行宣告操作请求响应
struct DataRonExecOrderAction
{
	DataRonExecOrderAction(void) 
		:   nRequestID(0),
		    bIsLast(false)
		
	{
		memset(&InputExecOrderActionField, 0, sizeof(InputExecOrderActionField));
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
	DataRonExecOrderAction(const PlatformStru_InputExecOrderActionField& inputExecOrderActionField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InputExecOrderActionField(inputExecOrderActionField),
		    RspInfoField(RspInfo),
		    nRequestID(requestID),
		    bIsLast(bisLast)		
	{
	};

	
	PlatformStru_InputExecOrderActionField  InputExecOrderActionField;
	CThostFtdcRspInfoField                  RspInfoField; 
	int                                     nRequestID;
	bool                                    bIsLast;  
};

//请求查询报单响应
struct DataRonQryOrder
{
	DataRonQryOrder(const PlatformStru_OrderInfo& orderField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   OrderField(orderField),
		    RspInfoField(RspInfo),
		    nRequestID(requestID),
		    bIsLast(bisLast)				
	{
	};

	
	PlatformStru_OrderInfo    OrderField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast; 
};
//请求查询成交响应
struct DataRonQryTrade
{
	DataRonQryTrade(const PlatformStru_TradeInfo& tradeField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		: TradeField(tradeField),
		  RspInfoField(RspInfo),
		  nRequestID(requestID),
		  bIsLast(bisLast)		
	{
	};
	
	PlatformStru_TradeInfo    TradeField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询投资者持仓响应
struct DataRonQryInvestorPosition
{
	DataRonQryInvestorPosition()
		:  nRequestID(0),
	       bIsLast(true)
		
	{
		memset(&InvestorPositionField,0,sizeof(InvestorPositionField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryInvestorPosition(const PlatformStru_Position& investorPositionField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InvestorPositionField(investorPositionField),
		    RspInfoField(RspInfo),
		    nRequestID(requestID),
		    bIsLast(bisLast)		
	{
	};


	
	PlatformStru_Position       InvestorPositionField;
	CThostFtdcRspInfoField      RspInfoField;  
	int                         nRequestID;
	bool                        bIsLast;  

};

//请求查询资金账户响应
struct DataRonQryTradingAccount
{
	DataRonQryTradingAccount()
		:  nRequestID(0),
		   bIsLast(true) 
		
	{
		memset(&TradingAccountField,0,sizeof(TradingAccountField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryTradingAccount(const PlatformStru_TradingAccountInfo& tradingAccountField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   TradingAccountField(tradingAccountField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)
		
		
	{
	};


	PlatformStru_TradingAccountInfo TradingAccountField;
	CThostFtdcRspInfoField          RspInfoField;    
	int                             nRequestID;
	bool                            bIsLast;
};

//请求查询投资者响应
struct DataRonQryInvestor
{
	DataRonQryInvestor()
		:  nRequestID(0),
	    	bIsLast(true)
		
	{
		memset(&InvestorField,0,sizeof(InvestorField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryInvestor(const CThostFtdcInvestorField& investorField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InvestorField(investorField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)
		
		
	{
	};
	
	CThostFtdcInvestorField   InvestorField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询交易编码响应
struct DataRonQryTradingCode
{
	DataRonQryTradingCode()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&TradingCodeField,0,sizeof(TradingCodeField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryTradingCode(const CThostFtdcTradingCodeField& tradingCodeField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  TradingCodeField(tradingCodeField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)
		
		
	{
	};
	
	CThostFtdcTradingCodeField   TradingCodeField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast; 
};

//请求查询合约保证金率响应
struct DataRonQryInstrumentMarginRate
{
	DataRonQryInstrumentMarginRate()
		:  nRequestID(0),
		   bIsLast(true)
	{
		memset(&InstrumentMarginRateField,0,sizeof(InstrumentMarginRateField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryInstrumentMarginRate(const PlatformStru_InstrumentMarginRate& instrumentMarginRateField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InstrumentMarginRateField(instrumentMarginRateField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	PlatformStru_InstrumentMarginRate   InstrumentMarginRateField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询合约手续费率响应
struct DataRonQryInstrumentCommissionRate
{
	DataRonQryInstrumentCommissionRate()
		:   nRequestID(0),
		    bIsLast(true)
		
	{
		memset(&InstrumentCommissionRateField,0,sizeof(InstrumentCommissionRateField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryInstrumentCommissionRate(const PlatformStru_InstrumentCommissionRate& instrumentCommissionRateField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InstrumentCommissionRateField(instrumentCommissionRateField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)
		
		
	{
	};

	
	PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询交易所响应
struct DataRonQryExchange
{
	DataRonQryExchange(const CThostFtdcExchangeField& exchangeField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   ExchangeField(exchangeField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)		
	{
	};

	
	CThostFtdcExchangeField   ExchangeField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast; 
};
//请求查询产品响应
struct DataRonQryProduct
{
	DataRonQryProduct(const PlatformStru_ProductInfo& productField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  ProductField(productField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast) 				
	{
	};

	
	PlatformStru_ProductInfo    ProductField;
	CThostFtdcRspInfoField      RspInfoField;    
	int                         nRequestID;
	bool                        bIsLast;
};

//请求查询合约响应
struct DataRonQryInstrument
{
	DataRonQryInstrument(const PlatformStru_InstrumentInfo& instrumentField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  InstrumentField(instrumentField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)
	{
	};

	
	PlatformStru_InstrumentInfo InstrumentField;
	CThostFtdcRspInfoField      RspInfoField;    
	int                         nRequestID;
	bool                        bIsLast;
};

//请求查询投资者结算结果响应
struct DataRonQrySettlementInfo
{
	DataRonQrySettlementInfo()
		:   nRequestID(0),
		    bIsLast(true)
		
	{
		memset(&SettlementInfoField,0,sizeof(SettlementInfoField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQrySettlementInfo(const CThostFtdcSettlementInfoField& settlementInfoField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   SettlementInfoField(settlementInfoField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)			
	{
	};


	CThostFtdcSettlementInfoField      SettlementInfoField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询投资者持仓明细响应
struct DataRonQryInvestorPositionDetail
{
	DataRonQryInvestorPositionDetail()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&InvestorPositionDetailField,0,sizeof(InvestorPositionDetailField));
		memset(&RspInfoField,0,sizeof(RspInfoField));

	};
	DataRonQryInvestorPositionDetail(const PlatformStru_PositionDetail& investorPositionDetailField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InvestorPositionDetailField(investorPositionDetailField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
		
	};
	
	PlatformStru_PositionDetail	InvestorPositionDetailField;
	CThostFtdcRspInfoField		RspInfoField;  
	int							nRequestID;
	bool						bIsLast;  

};

///查询用户通知响应
struct DataRonQryNotice
{
	DataRonQryNotice()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&NoticeField,0,sizeof(NoticeField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryNotice(const CThostFtdcNoticeField& noticeField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  NoticeField(noticeField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	
	
	CThostFtdcNoticeField     NoticeField; 
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};


struct DataRonQrySettlementInfoConfirm
{
	DataRonQrySettlementInfoConfirm(const CThostFtdcSettlementInfoConfirmField& settlementInfoConfirm,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  SettlementInfoConfirm(settlementInfoConfirm),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	
	
	CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirm;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询投资者持仓组合合约明细响应
struct DataRonQryInvestorPositionCombDetail
{
	DataRonQryInvestorPositionCombDetail()
		:   nRequestID(0),
		    bIsLast(true)		
	{
		memset(&InvestorPositionCombineDetailField,0,sizeof(InvestorPositionCombineDetailField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
		
	};
	DataRonQryInvestorPositionCombDetail(const PlatformStru_PositionDetailComb& investorPositionCombineDetailField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InvestorPositionCombineDetailField(investorPositionCombineDetailField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
		
	};

	
	PlatformStru_PositionDetailComb	InvestorPositionCombineDetailField;
	CThostFtdcRspInfoField		RspInfoField;    
	int							nRequestID;
	bool						bIsLast;

};

///查询保证金监管系统经纪公司资金账户密钥响应
struct DataRonTradingAccountKey
{
	DataRonTradingAccountKey(const CThostFtdcCFMMCTradingAccountKeyField& tradingAccountKeyField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  TradingAccountKeyField(tradingAccountKeyField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};



	CThostFtdcCFMMCTradingAccountKeyField      TradingAccountKeyField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};
//请求查询期权合约手续费率响应
struct DataRonQryOptionInstrCommRate
{
	DataRonQryOptionInstrCommRate()
		:  nRequestID(0),
		   bIsLast(true)		
	{
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryOptionInstrCommRate(const PlatformStru_InstrumentCommissionRate& instrumentCommissionRateField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   InstrumentCommissionRateField(instrumentCommissionRateField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};


	
	PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};
//请求查询期权交易成本响应
struct DataRonQryOptionInstrTradeCost
{
	DataRonQryOptionInstrTradeCost()
		:  nRequestID(0),
		bIsLast(true)
		
	{
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryOptionInstrTradeCost(const PlatformStru_InstrumentMarginRate& instrumentMarginRateField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		: InstrumentMarginRateField(instrumentMarginRateField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};



	PlatformStru_InstrumentMarginRate   InstrumentMarginRateField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};
//请求查询执行宣告响应
struct DataRonQryExecOrder
{
	DataRonQryExecOrder(void) 
	{
		nRequestID = 0;
		bIsLast = false;
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
	DataRonQryExecOrder(const PlatformStru_ExecOrderField& execOrderField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  ExecOrderField(execOrderField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)		
	{
	};


	
	PlatformStru_ExecOrderField ExecOrderField;
	CThostFtdcRspInfoField      RspInfoField;    
	int                         nRequestID;
	bool                        bIsLast;
};
//请求查询转帐流水响应
struct DataRonQryTransferSerial
{
	DataRonQryTransferSerial()
		:   nRequestID(0),
		    bIsLast(true)
		
	{
		memset(&TransferSerialField,0,sizeof(TransferSerialField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryTransferSerial(const CThostFtdcTransferSerialField& transferSerialField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   TransferSerialField(transferSerialField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	
	
	CThostFtdcTransferSerialField      TransferSerialField;
	CThostFtdcRspInfoField    RspInfoField;    
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询银期签约关系响应
struct DataRonQryAccountregister
{
	DataRonQryAccountregister(const CThostFtdcAccountregisterField& accountregister,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  Accountregister(accountregister),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	
	CThostFtdcAccountregisterField      Accountregister;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//错误应答
struct DataRonError
{
	DataRonError(const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		: RspInfoField(RspInfo),
		  nRequestID(requestID),
		  bIsLast(bisLast)	
					
	{
	};


	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};
struct DataErrRonOrderInsert
{
	DataErrRonOrderInsert()
	{
		memset(&InputOrderField,0,sizeof(InputOrderField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataErrRonOrderInsert(const CThostFtdcInputOrderField& OrderField,const CThostFtdcRspInfoField& RspInfo)
		: InputOrderField(OrderField),
		  RspInfoField(RspInfo)
	{
	};

	
	CThostFtdcInputOrderField InputOrderField;
	CThostFtdcRspInfoField    RspInfoField;    
};
//报单操作回执(错误回执2，交易所认为撤单无效)
struct DataErrRonOrderAction
{
	DataErrRonOrderAction()
	{
		memset(&OrderActionField,0,sizeof(OrderActionField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataErrRonOrderAction(const CThostFtdcOrderActionField& orderActionField,const CThostFtdcRspInfoField& RspInfo)
		:   OrderActionField(orderActionField),
		    RspInfoField(RspInfo)
		
	{
	};

	CThostFtdcOrderActionField OrderActionField;
	CThostFtdcRspInfoField     RspInfoField;    
};

//询价录入错误回报
struct DataErrRonForQuoteInsert
{
	DataErrRonForQuoteInsert(void) 
	{
		memset(this, 0, sizeof(*this));
	};
	DataErrRonForQuoteInsert(const PlatformStru_InputForQuoteField& inputForQuoteField,const CThostFtdcRspInfoField& RspInfo)
		:  InputForQuoteField(inputForQuoteField),
		   RspInfoField(RspInfo)
		
	{
	};

	PlatformStru_InputForQuoteField InputForQuoteField;
	CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告录入错误回报
struct DataErrRonExecOrderInsert
{
	DataErrRonExecOrderInsert(void) 
	{
		memset(this, 0, sizeof(*this));
	};
	DataErrRonExecOrderInsert(const PlatformStru_InputExecOrderField& inputExecOrderField,const CThostFtdcRspInfoField& RspInfo)
		:  InputExecOrderField(inputExecOrderField),
		   RspInfoField(RspInfo)		
	{
	};

	PlatformStru_InputExecOrderField InputExecOrderField;
	CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告操作错误回报
struct DataErrRonExecOrderAction
{
	DataErrRonExecOrderAction(void)
	{
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
	DataErrRonExecOrderAction(const PlatformStru_ExecOrderActionField& execOrderActionField,const CThostFtdcRspInfoField& RspInfo)
		:  ExecOrderActionField(execOrderActionField),
		   RspInfoField(RspInfo)		
	{
	};


	PlatformStru_ExecOrderActionField ExecOrderActionField;
	CThostFtdcRspInfoField      RspInfoField;    
};

//请求查询签约银行响应
struct DataRonQryContractBank
{
	DataRonQryContractBank(const CThostFtdcContractBankField& contractBankField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:   ContractBankField(contractBankField),
	    	RspInfoField(RspInfo),
	    	nRequestID(requestID),
		    bIsLast(bisLast)			
	{
	};


	
	CThostFtdcContractBankField      ContractBankField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast; 
};

//请求查询预埋单响应
struct DataRonQryParkedOrder
{
	DataRonQryParkedOrder()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&ParkedOrder,0,sizeof(ParkedOrder));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryParkedOrder(const CThostFtdcParkedOrderField& parkedOrder,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		: ParkedOrder(parkedOrder),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};


	
	CThostFtdcParkedOrderField      ParkedOrder;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询预埋撤单响应
struct DataRonQryParkedOrderAction
{
	DataRonQryParkedOrderAction()
		: nRequestID(0),
		  bIsLast(true)
		
	{
		memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryParkedOrderAction(const CThostFtdcParkedOrderActionField& parkedOrderAction,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  ParkedOrderAction(parkedOrderAction),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};


	
	CThostFtdcParkedOrderActionField      ParkedOrderAction;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询交易通知响应
struct DataRonQryTradingNotice
{
	DataRonQryTradingNotice()
		: nRequestID(0),
		bIsLast(true)

	{
		memset(&TradingNoticeField,0,sizeof(TradingNoticeField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQryTradingNotice(const CThostFtdcTradingNoticeField& tradingNoticeField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  TradingNoticeField(tradingNoticeField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};



	CThostFtdcTradingNoticeField      TradingNoticeField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//请求查询经纪公司交易参数响应
struct DataRonQryBrokerTradingParams
{
	DataRonQryBrokerTradingParams(const PlatformStru_BrokerTradingParams& brokerTradingParams,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  BrokerTradingParams(brokerTradingParams),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	
	
	PlatformStru_BrokerTradingParams    BrokerTradingParams;
	CThostFtdcRspInfoField              RspInfoField;    
	int                                 nRequestID;
	bool                                bIsLast;
};

//期货发起银行资金转期货应答
struct DataRonFromBankToFutureByFuture
{
	DataRonFromBankToFutureByFuture()
		:  nRequestID(0),
		   bIsLast(true)
		
	{
		memset(&TransferField,0,sizeof(TransferField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonFromBankToFutureByFuture(const CThostFtdcReqTransferField& transferField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  TransferField(transferField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};


	
	CThostFtdcReqTransferField      TransferField;
	CThostFtdcRspInfoField    RspInfoField;  
	int                       nRequestID;
	bool                      bIsLast; 
};

//期货发起期货资金转银行应答
struct DataRonFromFutureToBankByFuture
{
	DataRonFromFutureToBankByFuture()
		:   nRequestID(0),
		    bIsLast(true)
		
	{
		memset(&TransferField,0,sizeof(TransferField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonFromFutureToBankByFuture(const CThostFtdcReqTransferField& transferField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  TransferField(transferField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};

	
	
	CThostFtdcReqTransferField      TransferField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//期货发起查询银行余额应答
struct DataRonQueryBankAccountMoneyByFuture
{
	DataRonQueryBankAccountMoneyByFuture()
		:  nRequestID(0),
		   bIsLast(true)		
	{
		memset(&QueryAccountField,0,sizeof(QueryAccountField));
		memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataRonQueryBankAccountMoneyByFuture(const CThostFtdcReqQueryAccountField& queryAccountField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  QueryAccountField(queryAccountField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)			
	{
	};

	
	
	CThostFtdcReqQueryAccountField      QueryAccountField;
	CThostFtdcRspInfoField    RspInfoField;   
	int                       nRequestID;
	bool                      bIsLast;
};

//询价录入请求响应
struct DataRonForQuoteInsert
{
	DataRonForQuoteInsert(void) {
		memset(this, 0, sizeof(*this));
	};
	DataRonForQuoteInsert(const PlatformStru_InputForQuoteField& inputForQuoteField,const CThostFtdcRspInfoField& RspInfo,int requestID,bool bisLast)
		:  InputForQuoteField(inputForQuoteField),
		RspInfoField(RspInfo),
		nRequestID(requestID),
		bIsLast(bisLast)				
	{
	};


	
	PlatformStru_InputForQuoteField InputForQuoteField;
	CThostFtdcRspInfoField      RspInfoField;    
	int                         nRequestID;
	bool                        bIsLast;
};



#pragma pack(pop)