#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

/*
1. 保存一个原子操作的原始报单信息（即转换后也只对应一条报单请求，不会在分解了）
2. 转换成实际报单信息
3. 一个原子报单对应一个实例
*/

// 提供一下基本功能
// 1.  撤指定报单，由OrderKey指定相关的被撤挂单
// 2.  撤指定报单后下单，由OrderKey指定相关的被撤挂单
// 3.  下单
// 4.  撤已下报单
// 5.  撤已下报单后修改价格下单
// 6.  撤已下报单后修改数量下单
// 7.  撤已下报单后修改价格和数量下单
// 8.  撤单完成后自动下指定报单

class COrderAtomEntity
{
public:
	enum {	conModeNewOrder, conModeCancelOnly, conModeReplace, conModeCancelThenOrder };
	enum {	conPriceNone, conPriceMarket, conPriceLimit };//, conPriceUpperLimit, conPriceLowerLimit, 
			//conPriceLast, conPriceAsk1, conPriceBid1 };
	enum {	conVolumeNone, conVolumeCustom, conVolumeRemain, conVolumeRemainIncrement };
	enum {	conNothing, conWaiting, conIsSuccess, conIsFail, 
					conCancelling, conIsCancelSuccess, conIsCancelFail };
	enum {	conStepNothing, conStepOrderSource, conStepOrderChange };


	// 这个是标准的下单结构体，无平台差异的
	typedef struct {
		//BOOL	bIsParketOrder;			// 是否为预埋单
		int		nOrderType;				// 报单类型
		BOOL	bIsBuy;
		int		nOCMode;				// 开平仓模式, 0 未知，1 开仓，2 平今，3 平仓(平昨)
		BOOL	bIsHedge;				// FALSE 投机，TRUE 套保
		BOOL	bIsMarket;				// 是否为市价
		double	fPrice;
		int		nVolume;
		int		nTrigger_PriceType;		// 触发价格类型，已包含在nOrderType中了，这里只是用来做备份用的
		int		nTrigger_Condition;		// 触发条件，已包含在nOrderType中了，这里只是用来做备份用的
		BOOL	bTrigger_IsMarket;		// 是否为市价
		double	fTrigger_Price;
		InstrumentIDType		csInstrumentID;		// 合约
		CommonExchangeIDType	csExchangeID;		// 交易所
		CommonUserIDType		csUserID;			// 投资者ID或Pats的用户名
		CommonAccountType		csAccount;			// 子帐号
	} STDORDERREQ, *LPSTDORDERREQ;

public:
	// nMode，0 普通下单，1 改单(直接使用命令对挂单进行修改)，2 撤指定单后下单
	static COrderAtomEntity* CreateOrderAtomEntity(STDORDERREQ& tOriReq, 
								int nMode, LPVOID pOrderInfo=NULL, 
								BOOL bNoSendVolZoreOrder=FALSE);
	static BOOL ConvertCondToOrderType(int nPriceType, int nCondType, int& nOrderType);

	static BOOL ConvertToAction(PlatformStru_InputOrderAction& actionReq, 
								PlatformStru_OrderInfo& orderInfo);
	static BOOL ConvertToAction(PlatformStru_RemoveParkedOrder& actionReq, 
								CThostFtdcParkedOrderField& orderInfo);
	static wxString MakeRevokeDesciption(PlatformStru_OrderInfo& field);
	
	static BOOL IsParketOrder(STDORDERREQ& stdReq) {
		return stdReq.nOrderType==OrderType_FT_ParkedOrder;
	};
	
	~COrderAtomEntity();

protected:
	COrderAtomEntity(STDORDERREQ& stdReq, int nMode, LPVOID pOrderInfo=NULL, 
			BOOL bNoSendVolZoreOrder=FALSE);

	BOOL SendSrcOrder(BOOL bIsReplace);
	BOOL ChangeOrder(int nPriceMode, double fPrice, 
					int nPriceTriggerMode, double fPriceTrigger, 
					int nVolumeMode, int nVolume, BOOL bIsReplaceOrder);
	BOOL CancelOrder(int nNextStep=conStepNothing);

public:

	void SetStartTime(DWORD dwStartTime) {
		m_dwStartTime = dwStartTime;
	};
	int GetStartTime() {
		return m_dwStartTime;
	};
	BOOL IsOverdue(DWORD dwCurrTime, DWORD dwTimeoutNum=2000) {
		return (dwCurrTime-m_dwStartTime)>=dwTimeoutNum;
	};

	int GetRunStatus() {
		return m_nRunStatus;
	};
	int GetNextStep() {
		return m_nNextStep;
	}

	STDORDERREQ& GetStdOrderReq() {
		return m_stdReq;
	};
	string GetUserID() {
		return string(m_stdReq.csUserID);
	};

	CThostFtdcRspInfoField&	GetLastRspInfo() {
		return m_lastRspInfo;
	};

	BOOL IsCanCancel() {
		if(m_nArrayIndex<0)
			return FALSE;
		return m_arrayOrderRsp[m_nArrayIndex].OrderStatus==THOST_FTDC_OST_PartTradedQueueing ||
				m_arrayOrderRsp[m_nArrayIndex].OrderStatus==THOST_FTDC_OST_NoTradeQueueing;
	};

	int GetTradeVolume() {
		if(IsParketOrder(m_stdReq))
			return 0;
		if(m_nRunStatus == conWaiting)
			return 0;
		if(m_nArrayIndex<0)
			return 0;
		return m_arrayOrderRsp[m_nArrayIndex].VolumeTraded;
	};
	
	// 第一次执行下单是使用，只用状态为conNothing有效
	BOOL Launch(BOOL bNothingOrder=FALSE);
	// 撤销由此对象下的报单，当状态为conIsSuccess有效
	BOOL Cancel();
	// 改单，直接使用改单命令字，当状态为conIsSuccess有效
	// conVolumeNone不改数据
	BOOL Replace(int nPriceMode, double fPrice, 
			int nPriceTriggerMode, double fPriceTrigger, int nVolumeMode, int nVolume);
	// 撤销并重新下单，当状态为conIsSuccess有效
	// conVolumeNone使用剩余数量下单，同conVolumeRemain
	BOOL CancelThenOrder(int nPriceMode, double fPrice, 
			int nPriceTriggerMode, double fPriceTrigger, int nVolumeMode, int nVolume);

	void ChangePrice(BOOL bIsMarket, double fPrice);
	void ChangeTriggerCond(int nPriceType, int nCondType, 
			BOOL bIsMarket, double fPrice, BOOL bNeedChangeOrderType=TRUE);

	BOOL ParseParkedStatus(DataRspParkedOrderInsert& field);
	BOOL ParseOrderStatus(DataRtnOrder& field);
	BOOL ParseOrderStatus(DataRspOrderInsert& field);
	BOOL ParseActionStatus(DataRspOrderAction1& field);
	BOOL ParseActionStatus(DataRspOrderAction2& field);

	wxString GetOrderDesciption();
	wxString GetRevokeDesciption() {
		return m_strRevokeMsg;
	};

private:

	DWORD				m_dwStartTime;//初值=0xFFFFFFFF;

	// 每个时刻只能有一个状态，并根据不同的状态进行不同的活动
	int					m_nRunStatus;

	BOOL				m_bNeedNewOrderStatus;
	UINT				m_nLastOrderReqID;
	UINT				m_nLastCancelReqID;
	int					m_nArrayIndex;

	std::vector<CThostFtdcParkedOrderField>
						m_arrayParkedRsp;
	std::vector<PlatformStru_OrderInfo>
						m_arrayOrderRsp;

	int					m_nMode;
	BOOL				m_bNoSendVolZoreOrder;
	STDORDERREQ			m_stdReq;

	int					m_nNextStep;
	int					m_nPriceMode;
	double				m_fPrice;
	int					m_nPriceTriggerMode;
	double				m_fPriceTrigger;
	int					m_nVolumeMode;
	int					m_nVolume;

	wxString			m_strRevokeMsg;

	CThostFtdcRspInfoField	
						m_lastRspInfo;	// 应答结构

};
