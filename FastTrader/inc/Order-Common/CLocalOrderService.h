
#pragma once

#include "../Module-Misc2/BusinessData.hpp"

	///标示一个Local记录
#define _MakeKeyDefine(name, order) \
	struct name##Key \
	{ \
		name##Key() \
		{ \
			ZeroMemory(this, sizeof(*this)); \
		};\
\
		name##Key(const long	FTID) \
		{ \
			ZeroMemory(this, sizeof(*this)); \
			this->FTID = FTID; \
		}; \
\
		name##Key(const order& Order) \
		{ \
			ZeroMemory(this, sizeof(*this)); \
			FTID = Order.FTID; \
		}; \
\
		name##Key& operator= (const name##Key& other) \
		{ \
			if (this == &other) \
				return *this; \
\
			memcpy((char*)this,(char*)&other,sizeof(*this)); \
			return *this; \
		}; \
\
		bool operator< (const name##Key& r) const \
		{ \
			if(FTID<r.FTID) \
				return true; \
			if(FTID>r.FTID) \
				return false; \
\
			return false; \
		} \
\
		bool operator== (const name##Key& r) const \
		{ \
			if(FTID!=r.FTID) \
				return false; \
			return true; \
		}\
\
		bool operator> (const name##Key& r) const \
		{ \
			return !((*this) < r || (*this) == r); \
		} \
\
		char* tostring(char* prlt, int rltsize) const \
		{ \
			if(!prlt||!rltsize) return NULL; \
			int len=0,ilen; \
			prlt[len]=0; \
\
			char buf[256]; \
			sprintf(buf, "FTID[%d]", FTID); \
			ilen=strlen(buf); \
			if(len+ilen<rltsize) \
			{ \
				strcat(prlt, buf); \
				len=ilen; \
			} \
\
			return prlt; \
		}\
\
		long	FTID; \
	};


// 请求状态
enum {	conReqWaiting,			// 等待处理
		conReqSendOk,			// 发送成功
		conReqSendFail,			// 发送失败
		conReqEntrustOk,		// 委托单成功
		conReqEntrustFail,		// 委托单失败
		conReqDealFail,			// 未成交
		conReqDealClose			// 已成交
};

// 批量单状态
enum {	conBatchPause,			// 暂停
		conBatchActive,			// 激活
		conBatchDelete,			// 删除
		conBatchCompleted		// 全部成交
};

// 预埋单状态
enum {	conParkedPause,			// 暂停
		conParkedActive,		// 激活
		conParkedDelete,		// 删除
		conParkedConfirm		// 提交报单
};

// 条件单状态
enum {	conCondPause,			// 暂停
		conCondActive,			// 激活
		conCondDelete,			// 删除
		conCondConfirm,			// 提交报单
		conCondCancel,			// 撤单
		conCondCompleted		// 全部成交
};

//// 盈损单状态
//enum {	conPLPause,				// 暂停
//		conPLActive,			// 激活
//		conPLDelete,			// 删除
//		conPLConfirm,			// 执行止盈止损
//		conPLCancel,			// 撤销止盈止损
//		conPLCompleted			// 全部成交
//};
// 盈损单状态
enum {	conPLPause,				// 暂停
		conPLActive,			// 激活，下单等待
		conPLDelete,			// 删除
		conPLNoTrade,			// 未成交
		conPLPartTraded,		// 部分成交
		conPLAllTraded,			// 全部成交
		conPLCancelNoTrade,		// 未成交已撤销
		conPLCancelPartTraded,	// 部分成交已撤销
		conPLFail				// 下单失败
};

// 持仓止盈止损状态
enum {	conPosiPLPause,			// 暂停
		conPosiPLActive,		// 激活
		conPosiPLDelete,		// 删除
		conPosiPLConfirm,		// 执行止盈止损
		conPosiPLCancel,		// 撤销止盈止损
		conPosiPLCompleted		// 止盈止损完成
};

// 盈损单明细状态
enum {	conPLDetailPause,		// 暂停
		conPLDetailActive,		// 激活
		conPLDetailDelete,		// 删除
		conPLDetailConfirm,		// 执行止盈止损
		conPLDetailCancel,		// 撤销止盈止损
		conPLDetailCompleted	// 全部成交
};

// 条件单与盈损单的关联关系，conCondPosRelation
enum {	conCPRNon,				// 无关系
		conCPRCondFromPL,		// 条件单是盈损单明细的止盈止损条件
		conCPRCondNextPL		// 条件单完成后激活盈损单
};

// 条件单价格触发类型
enum {	conCndPTypeLast=1,		// 1 最新
		conCndPTypeBid,			// 2 买价
		conCndPTypeAsk			// 3 卖价
};

// 条件单触发条件类型
enum {  conCndTypeLessEqual=1, 	//1 小于等于指定价格
		conCndTypeGreaterEqual, //2 大于等于指定价格
		conCndTypeLess, 		//3 小于指定价格
		conCndTypeGreater		//4 大于指定价格
};

// 盈损单价格偏移设置类型
enum {	conPLPRTradePrice=1,	// 1 成交价
		conPLPRReqPrice			// 2 委托价
};

// 盈损单价格触发类型
enum {	conPLPTypeLast=1,		// 1 最新
		conPLPTypeBidAsk		// 2 买卖价
};

// 盈损单平仓方式
enum {	conPLCMBound=1,			// 1 反向涨跌停价
		conPLCMOffer,			// 2 反向加减价
		conPLCMMarket			// 3 市价
};


class CLocalOrderService : public wxEvtHandler//, wxThread
{
public:

	typedef struct {
		DWORD	dwDate;
		DWORD	dwTime;
		DWORD	dwOrderReqNum;
		DWORD	dwBatchGroupNum;		// 批量单号
		DWORD	dwBatchOrderNum;		// 批量单子单号
		DWORD	dwParkedOrderNum;
		DWORD	dwCondOrderNum;
		DWORD	dwPLOrderNum;
		DWORD	dwPLDetailOrderNum;
		DWORD	dwParkedCount;
		DWORD	dwParkedOffset;
		DWORD	dwParkedSize;
		DWORD	dwCondActiveCount;
		DWORD	dwCondActiveOffset;
		DWORD	dwCondActiveSize;
		DWORD	dwPLCount;
		DWORD	dwPLOffset;
		DWORD	dwPLSize;
		DWORD	dwPLDetailCount;
		DWORD	dwPLDetailOffset;
		DWORD	dwPLDetailSize;
		DWORD	dwBatchSubCount;
		DWORD	dwBatchSubOffset;
		DWORD	dwBatchSubSize;
	} FILEHEAD, *PFILEHEAD;		// sizeof()=64


	// 批量单
	typedef struct {
		int		nNum;				//序号
		int		nState;				//活动状态
		int		nOriVolume;			//原始提交数量
		int		nPriceType;			//价格类型，1 输入价，2 最新价，3 买一卖一价
		int		nEachNum;			//每批手数
		int		nTimeInterval;		//时间间隔，单位：秒
		int		nNextSubNum;		//下一个待触发组内号
		int		nTimePos;			//时间点，倒计时形式，初值为"nTimeInterval"
		PlatformStru_InputOrder req;
	} BATCHORDER, *PBATCHORDER;


	// 批量单子单
	typedef struct {
		int		nNum;				//序号
		int		nGroupNum;			//组号，或批量单号
		int		nSubNum;			//组内号
		int		nRegNum;			//请求号
		int		nEntrustNum;		//委托单号
		int		nDealNum;			//成交单号
		int		nRegState;			//委托状态
		PlatformStru_InputOrder req;
	} BATCHSUBORDER, *PBATCHSUBORDER;


	// 与报单相关需保留的字段
	// 报单关键数据
	typedef	struct {
		// 本地编号
		long						FTID;
		///前置编号
		TThostFtdcFrontIDType		FrontID;
		///会话编号
		TThostFtdcSessionIDType		SessionID;
		///请求编号
		TThostFtdcRequestIDType		RequestID;
		///报单提交状态
		TThostFtdcOrderSubmitStatusType	OrderSubmitStatus;
		///报单状态
		TThostFtdcOrderStatusType	OrderStatus;
		///报单引用
		TThostFtdcOrderRefType		OrderRef;
		///交易所代码
		TThostFtdcExchangeIDType	ExchangeID;
		///报单编号
		TThostFtdcOrderSysIDType	OrderSysID;
		///成交编号
		TThostFtdcTradeIDType		TradeID;
		// 交易账号
		CommonAccountType			Account;
	} ORDERREF, *PORDERREF;


	// 用户下单参数
	typedef struct {
		int		nOrderType;			//下单类型
		int		nOCMode;			//开平方式      //0开仓; 1平今; 2平仓; -1未知
		BOOL	bIsBuy;				//是否为买入
		BOOL	bIsMarket;			//是否为市价
		int	    nHedgeStatus;	    //0/1/2对应投机/套利/保值
		double	fPrice;				//委托价
		int		nVolume;			//委托数量
		InstrumentIDType	InstrumentID;	//合约代码
	} ORDERPARAM, *PORDERPARAM;


	// 条件设置参数
	typedef struct {
		int		nPriceType;			//价格触发类型，1 最新，2 买价，3 卖价
		int		nConditionType;		//触发条件类型，1 小于等于指定价格，2 大于等于指定价格
		double	fConditionPrice;	//指定的触发价格
	} CONDPARAM, *PCONDPARAM;


	// 盈损设置参数
	typedef struct {
		int		nPriceOffsetType;	//价格偏移设置类型，1 成交价，2 委托价
		BOOL	bDoStopLose;		//执行止损
		double	fStopLosePriceOffset;	//止损价差
		BOOL	bDoStopGain;		//执行止盈
		double	fStopGainPriceOffset;	//止盈价差
		int		nPriceType;			//触发价格类型，1 最新价，2 买卖价
		int		nCloseMode;			//平仓方式，1 反向涨跌停价，2 反向加减价
		double	fClosePriceOffset;	//反向加减价差
	} PLPARAM, *PPLPARAM;


	// 本地预埋单
	typedef struct {
		int		nNum;				//序号
		int		nState;				//活动状态
		int		nRegNum;			//请求号
		int		nRegState;			//委托状态
		BOOL	bIsAuto;			//是否为自动模式，FALSE 手动发出，TRUE 自动
		DWORD	nTradingDay;		//交易日
		DWORD	nCreateTime;		//预埋时间
		DWORD	nDeleteTime;		//删除时间
		PlatformStru_InputOrder req;
		TThostFtdcExchangeIDType	ExchangeID;	// 交易所代码
		char	strErrorMsg[256];	//错误信息
	} PARKEDORDER, *PPARKEDORDER;


	// 本地条件单
	typedef struct {
		//FasttraderID,对于每一条记录的Key，这是一个唯一编号
		long	FTID;                                                
		//每次数据内容发生变化，UpdateSeq加一
		long	UpdateSeq;
		InstrumentIDType	InstrumentID;	//合约代码

		//BOOL	bIsRunInServer;		//是否在综合交易平台上执行止盈止损
		int		nState;				//活动状态
		int		nCondPLRelation;	//与盈损单的关系
		int		nPLNum;				//盈损单号
		int		nPLDetailNum;		//盈损单号
		//int		nRelateCondNum;		//关联条件单号
		int		nRegState;			//委托状态
		ORDERREF	ref;
		ORDERPARAM	orderSrc;
		CONDPARAM	cond;
		DWORD	nTradingDay;		//交易日
		DWORD	nCreateTime;		//预埋时间
		DWORD	nDeleteTime;		//删除时间
		char	strErrorMsg[256];	//错误信息
	} CONDITIONORDER, *PCONDITIONORDER;


	// 2012.7.11，杨桂明
	// 盈损单的规则
	// 1.  根据指定的委托开仓
	// 2.  生成本地盈损单并进行监视
	// 3.  当有成交时，生成盈损单明细，并监视
	// 4.  盈损单明细生成止盈、止损条件单，下单数量与成交量相等
	// 5.  盈损单条件被编辑后，不修改已经下过的条件单，只对新的成交下的条件单使用
	// 6.  盈损单被删除后，只删除盈损单本身，主单未完成成交的进行撤单
	// 7.  盈损单明细条件被编辑后，只修改自身参数，及与之匹配的本地条件单
	// 8.  盈损单明细被删除后，删除盈损单明细本身及相关的条件单


	// 本地盈损单
	typedef struct {
		//FasttraderID,对于每一条记录的Key，这是一个唯一编号
		long	FTID;
		//每次数据内容发生变化，UpdateSeq加一
		long	UpdateSeq;
		InstrumentIDType	InstrumentID;	//合约代码

		BOOL	bIsRunInServer;		//是否在综合交易平台上执行止盈止损
		int		nState;				//活动状态

		int		nRegState;			//开仓委托状态
		ORDERREF	ref;

		PLPARAM		plparam;
		//int		nPriceOffsetType;	//价格偏移设置类型，1 成交价，2 委托价
		//BOOL	bDoStopLose;		//执行止损
		//double	fStopLosePriceOffset;	//止损价差
		//BOOL	bDoStopGain;		//执行止盈
		//double	fStopGainPriceOffset;	//止盈价差
		//int		nPriceType;			//触发价格类型，1 最新价，2 买卖价
		//int		nCloseMode;			//平仓方式，1 反向涨跌停价，2 反向加减价
		//double	fClosePriceOffset;	//反向加减价差
		ORDERPARAM	orderSrc;
		//int		nOCMode;			//开平方式
		//BOOL	bIsBuy;				//是否为买入
		//BOOL	bIsMarket;			//是否为市价
		//BOOL	bHedge;				//投机或保值
		//double	fRegPrice;			//委托价
		//int		nRegVolume;			//委托数量
		DWORD	nTradingDay;		//交易日
		DWORD	nCreateTime;		//预埋时间
		DWORD	nDeleteTime;		//删除时间
	} PLORDER, *PPLORDER;


	// 本地盈损单成交对应明细
	typedef struct {
		//FasttraderID,对于每一条记录的Key，这是一个唯一编号
		long	FTID;
		//每次数据内容发生变化，UpdateSeq加一
		long	UpdateSeq;
		InstrumentIDType	InstrumentID;	//合约代码

		BOOL	bIsRunInServer;		//是否在综合交易平台上执行止盈止损
		int		nState;				//活动状态
		long	nPLNum;				//盈损单编号

		ORDERREF ref;

		ORDERPARAM	orderSrc;
		PLPARAM		plparam;

		double	fOpenedPriceOffset;	//成交价偏移
		double	fStopLosePrice;		//止损价
		double	fStopGainPrice;		//止盈价
		double	fStopLoseForecast;	//止损预测
		double	fStopGainForecast;	//止盈预测

		BOOL	bGainIsServ;		//止盈条件单是否在服务器上运行
		BOOL	bLoseIsServ;		//止损条件单是否在服务器上运行
		ORDERREF	refGain;		//止盈条件单号
		ORDERREF	refLose;		//止损条件单号
		int		nGainRegState;		//止盈条件单委托状态
		int		nLoseRegState;		//止损条件单委托状态
		DWORD	nTradingDay;		//交易日
		DWORD	nCreateTime;		//预埋时间
		DWORD	nDeleteTime;		//删除时间
	} PLDETAILORDER, *PPLDETAILORDER;


	// 处理等待删除或新下应答的服务器条件单的结构体
	typedef struct {
		long		nPosiPLNum;		// 止盈止损编号
		BOOL		bIsLose;		// 是否为止损
		DWORD		dwStarTime;		// 执行删除操作的时间，用于超时处理
		ORDERREF	oldCondRef;		// 被删除或新下的服务器条件单的ref关键数据
		ORDERREF	newCondRef;		// 被删除或新下的服务器条件单的ref关键数据
		PLPARAM		plparam;		// 盈损设置
		ORDERPARAM	newOrder;		// 重下的下单结构
	} WaitDelOrNew_ServCond, *LPWaitDelOrNew_ServCond;


	_MakeKeyDefine(CondOrder, CONDITIONORDER)
	_MakeKeyDefine(PLOrder, PLORDER)
	_MakeKeyDefine(PLDetailOrder, PLDETAILORDER)


	typedef std::map<long, BATCHORDER> MAPBATCHORDER;
	typedef std::map<long, BATCHORDER>::iterator MAPBATCHORDERITEM;
	typedef std::map<long, BATCHSUBORDER> MAPBATCHSUBORDER;
	typedef std::map<long, BATCHSUBORDER>::iterator MAPBATCHSUBORDERITEM;
	typedef std::map<long, PARKEDORDER> MAPPARKEDORDER;
	typedef std::map<long, PARKEDORDER>::iterator MAPPARKEDORDERITEM;
	typedef std::map<long, CONDITIONORDER> MAPCONDITIONORDER;
	typedef std::map<long, CONDITIONORDER>::iterator MAPCONDITIONORDERITEM;
	typedef std::map<long, PLORDER> MAPPLORDER;
	typedef std::map<long, PLORDER>::iterator MAPPLORDERITEM;
	typedef std::map<long, PLDETAILORDER> MAPPLDETAILORDER;
	typedef std::map<long, PLDETAILORDER>::iterator MAPPLDETAILORDERITEM;

public:
	
	static CLocalOrderService* GetInstance();
    static CLocalOrderService* CreateThread();


	CLocalOrderService();//wxWindow *parent, wxWindowID winid);
	~CLocalOrderService();

	BOOL InitService();

	void OnRcvLogon(wxCommandEvent& evt);
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnRcvQuot(wxCommandEvent& evt);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
	
	void OnPlatSvrStatusChange(wxCommandEvent& evt);
	void OnStopPlatform(wxCommandEvent& evt);
    void OnTimer();
	DECLARE_EVENT_TABLE()
public:
	static int CallBack_UserLogin(const GUIModuleID,const AbstractBusinessData &data);
	static int CallBack_InstrumentStatus(const GUIModuleID,const AbstractBusinessData &data);
	static int CallBack_DepthMarketCallBack(const GUIModuleID,const AbstractBusinessData& data);
	static int CallBack_RspOrderInsertCallBackFunc(const GUIModuleID,const AbstractBusinessData& data);
	static int CallBack_TradeCallBackFunc(const GUIModuleID,const AbstractBusinessData &data);

	//static BOOL MakeLocalCondOrder_PLDetail(PLORDER& tPLOrder, PLDETAILORDER& tPLDetail, 
	//			CONDITIONORDER& tCondGain, CONDITIONORDER& tCondLose);
	static BOOL MakeLocalCondOrder_PLDetail(PLDETAILORDER& tPLDetail, 
				CONDITIONORDER& tCondGain, CONDITIONORDER& tCondLose);
	static BOOL ConvertPLD2Cond(ORDERPARAM& orderPL, BOOL bIsLose, 
								PLPARAM& plparam, CONDPARAM& cond, ORDERPARAM& orderNew);
	static BOOL ConvertLocal2ReqOrder(CONDITIONORDER& tCondOrder, PlatformStru_InputOrder& req);
	static BOOL ConvertLocal2ServCond(CONDITIONORDER& tCondOrder, PlatformStru_InputOrder& req);
	static BOOL ConvertServ2LocalCond(CThostFtdcInputOrderField& inputOrder, CONDITIONORDER& tCondOrder);
	static BOOL CopyOrderRef(ORDERREF& ref, PlatformStru_OrderInfo& order);

//	static int	sm_nReqNum;
	static int	sm_nBatchOrderNum;
	static int	sm_nBatchSubOrderNum;
	static int	sm_nParkedOrderNum;
	static int	sm_nConditionOrderNum;
	static int	sm_nPLOrderNum;
	static int	sm_nPLDetailOrderNum;

public:

	MAPBATCHORDER& GetOrderBatchMap() {
		return m_mapBatchOrder;
	};
	MAPBATCHSUBORDER& GetOrderBatchSubMap() {
		return m_mapBatchSubOrder;
	};
	MAPPARKEDORDER& GetOrderParkedMap() {
		return m_mapParkedOrder;
	};
	MAPCONDITIONORDER& GetOrderConditionMap() {
		return m_mapConditionOrder;
	};
	MAPPLORDER& GetOrderPLMap() {
		return m_mapPLOrder;
	};
	MAPPLDETAILORDER& GetOrderPLDetailMap() {
		return m_mapPLDetailOrder;
	};

	long OrderBatchAdd(BATCHORDER& tOrder);
	BOOL OrderBatchDelete(long nNum);
	void OrderBatchClear();
	BOOL OrderBatchActive(long nNum, BOOL bActive);

	long OrderBatchSubAdd(BATCHSUBORDER& tOrder);
	BOOL OrderBatchSubDelete(long nNum);
	void OrderBatchSubClear();

	long OrderParkedAdd(PARKEDORDER& tOrder);
	BOOL OrderParkedSet(long nNum, PARKEDORDER& tOrder);
	BOOL OrderParkedDelete(long nNum);
	void OrderParkedClear();
	BOOL OrderParkedActive(long nNum, BOOL bActive);
	BOOL OrderParkedDoNow(long nNum);

	long OrderConditionAdd(CONDITIONORDER& tOrder);
	BOOL OrderConditionAdd_PLDetail(CONDITIONORDER& tOrder1, CONDITIONORDER& tOrder2);
	BOOL OrderConditionSet(long nNum, CONDITIONORDER& tOrder);
	BOOL OrderConditionDelete(long nNum, int nState=conCondDelete, BOOL bIsPostEvent=FALSE);

	void OrderConditionClear();
	BOOL OrderConditionActive(long nNum, BOOL bActive);
	BOOL OrderConditionDoNow(long nNum);

	long OrderPLAdd(PLORDER& tOrder);
	BOOL OrderPLSet(long nNum, PLORDER& tOrder);
	BOOL OrderPLDelete(long nNum, int nState=conPLDelete, BOOL bIsPostEvent=FALSE);
	void OrderPLClear();

	long OrderPLDetailAdd(PLDETAILORDER& tOrder);
	BOOL OrderPLDetailSet(long nNum, PLDETAILORDER& tOrder);
	BOOL OrderPLDetailDelete(long nNum, int nState=conPLDetailDelete, BOOL bIsPostEvent=FALSE);
	BOOL OrderPLDetailDeleteAll(int nState=conPLDetailDelete, BOOL bIsPostEvent=FALSE);
	BOOL OrderPLDetailChange(int nNum, int nSubVolume);
	void OrderPLDetailClear();
	BOOL ConvertServ2Local_PLDetail(long nNum, BOOL bIsLose);
	BOOL AddPosiPLAndRun(PLDETAILORDER& tPLDetail);

	BOOL HasNoTouchLocalOrder();

	void CheckLocalOrder();
	int GetCondOrderVolume(BOOL bIsServ, ORDERREF& condRef, std::string& strInstruemtID);
	int	GetPosiPLVolume(PLDETAILORDER& posiPLOrder);
	// nMode取值, 0 求所有, 1 平今, 2 平昨
	int GetPosiPLVolume(std::string& strAccount, std::string& strInstrumentID, 
						BOOL bIsBuy, int nHedgeStatus, int nMode=0);
	BOOL AsyncVolumePosiAndPL(PlatformStru_Position& posiInfo);
	void AsyncPosiPLOrderVolume();

	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

	std::map<std::string, char>& GetExchangeStatusMap() {
		return m_mapExchangeStatus;
	};

	void RegisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.insert(pMsgWindow);
	};
	void UnregisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.erase(pMsgWindow);
	};

private:
	BOOL MakePLDetailAndRun(PLORDER& tPLOrder, PlatformStru_TradeInfo& traderInfo);
	void OrderCondition(CONDITIONORDER& tOrder, BOOL bIsServCond=FALSE);
	void OrderParked(PARKEDORDER& tOrder);
	void DoConditionOrder(PlatformStru_DepthMarketData& rawData);
	//void DoPositionOrder(CThostFtdcDepthMarketDataField& rawData);
	int  CancelCondServer(ORDERREF& ref, char* InstrumentID);
	BOOL OrderCondServDelete_PLDetail(PLDETAILORDER plDetail, BOOL bIsPostEvent=FALSE); 
	BOOL CheckOverTradingDay();

	void PostEvent_ParkedStatus(long nNum);
	void PostEvent_ConditionStatus(long nNum);
	// nMode, 0 条件单 pInputOrder有效，1 止盈, 2 止损 nNum有效 是PLDetail的编号
	void PostEvent_CondServFail(CThostFtdcInputOrderField* pInputOrder, long nNum, int nMode);
	void PostEvent_PLStatus(long nNum);
	void PostEvent_PLDetailStatus(long nNum, long nLPNum);
	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

private:
	MAPBATCHORDER		m_mapBatchOrder;			// 本地批量单
	MAPBATCHSUBORDER	m_mapBatchSubOrder;			// 本地批量单子单
	MAPPARKEDORDER		m_mapParkedOrder;			// 本地预埋单
	MAPCONDITIONORDER	m_mapConditionOrder;		// 本地条件单
    MAPPLORDER			m_mapPLOrder;				// 本地盈损单
    MAPPLDETAILORDER	m_mapPLDetailOrder;			// 本地盈损单明细

	// 等待删除应答的服务器条件单，仅用于止盈止损的改单
	std::map<long, WaitDelOrNew_ServCond>	m_mapWaitDelServCond;
	// 等待下单应答的服务器条件单，仅用于止盈止损的改单
	std::map<long, WaitDelOrNew_ServCond>	m_mapWaitNewServCond;

	std::map<std::string, char> m_mapExchangeStatus;		// 交易所交易状态
	std::set<void*>		m_setMsgWindow;				// 接收消息的窗口对象指针集合

	wxMutex				m_oMutex;
    //wxTimer				m_timer;

	std::string			m_strInvestorID;
	DWORD				m_nTradingDay;

};