
#pragma once

#include "stdafx.h"
#include <vector>
#include "CfgMgr/CfgGlobal.h"


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

// 盈损单状态
enum {	conPositionPause,		// 暂停
		conPositionActive,		// 激活
		conPositionDelete,		// 删除
		//conPositionBaseCancel,	// 基础单撤单
		//conPositionBaseTrade,	// 基础单成交
		conPositionConfirm,		// 执行止盈止损
		conPositionCancel,		// 撤销止盈止损
		conPositionCompleted	// 全部成交
};

// 条件单与盈损单的关联关系，conCondPosRelation
enum {	conCPRNon,				// 无关系
		conCPRCondFromPos,		// 条件单是盈损单的止盈止损条件
		conCPRCondNextPos		// 条件单完成后激活盈损单
};

class COrderServiceThread : public wxEvtHandler//, wxThread
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
		DWORD	dwPositionOrderNum;
		DWORD	dwParkedCount;
		DWORD	dwParkedOffset;
		DWORD	dwParkedSize;
		DWORD	dwCondActiveCount;
		DWORD	dwCondActiveOffset;
		DWORD	dwCondActiveSize;
		DWORD	dwPositionCount;
		DWORD	dwPositionOffset;
		DWORD	dwPositionSize;
		DWORD	dwBatchSubCount;
		DWORD	dwBatchSubOffset;
		DWORD	dwBatchSubSize;
		DWORD	dwCondOverdueCount;
		DWORD	dwCondOverdueOffset;
		DWORD	dwCondOverdueSize;
		DWORD	dwPosOverdueCount;
		DWORD	dwPosOverdueOffset;
		DWORD	dwPosOverdueSize;
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

	// 本地预埋单
	typedef struct {
		int		nNum;				//序号
		int		nState;				//活动状态
		int		nRegNum;			//请求号
		int		nRegState;			//委托状态
		BOOL	bIsAuto;			//是否为自动模式，FALSE 手动发出，TRUE 自动
		DWORD	nCreateTime;		//预埋时间
		DWORD	nDeleteTime;		//删除时间
		PlatformStru_InputOrder req;
		TThostFtdcExchangeIDType	ExchangeID;	// 交易所代码
		char	strErrorMsg[256];	//错误信息
	} PARKEDORDER, *PPARKEDORDER;

	// 本地条件单
	typedef struct {
		int		nNum;				//序号
		int		nState;				//活动状态
		int		nCondPosRelation;	//与盈损单的关系
		int		nPosNum;			//盈损单号
		//int		nRelateCondNum;		//关联条件单号
		int		nRegNum;			//请求号
		int		nEntrustNum;		//委托单号
		int		nDealNum;			//成交单号
		int		nRegState;			//委托状态
		BOOL	bHedge;				//投机或保值
		int		nPriceType;			//价格触发类型，1 最新，2 买价，3 卖价
		int		nConditionType;		//触发条件类型，1 小于等于指定价格，2 大于等于指定价格
		double	fConditionPrice;	//指定的触发价格
		DWORD	nCreateTime;		//预埋时间
		DWORD	nDeleteTime;		//删除时间
		PlatformStru_InputOrder req;
		char	strErrorMsg[256];	//错误信息
	} CONDITIONORDER, *PCONDITIONORDER;

	// 盈损单的规则
	// 1.  根据指定的委托开仓
	// 2.  同时开一个止盈条件单
	// 3.  同时再开一个止损条件单

	// 本地盈损单
	typedef struct {
		int		nNum;				//序号
		BOOL	bIsRunInServer;		//是否在综合交易平台上执行止盈止损
		int		nState;				//活动状态
		int		nRegNum;			//请求号
		int		nOpenNum;			//开仓委托号
		int		nDealNum;			//成交单号
		int		nRegState;			//开仓委托状态
		int		nCondNum_StopGain;	//止盈条件单号
		int		nCondNum_StopLose;	//止损条件单号
		BOOL	bHedge;				//投机或保值
		int		nPriceOffsetType;	//价格偏移设置类型，1 成交价，2 委托价
		BOOL	bDoStopLose;		//执行止损
		double	fStopLosePriceOffset;	//止损价差
		BOOL	bDoStopGain;		//执行止盈
		double	fStopGainPriceOffset;	//止盈价差
		int		nPriceType;			//触发价格类型，1 最新价，2 买卖价
		int		nCloseMode;			//平仓方式，1 反向涨跌停价，2 反向加减价
		double	fClosePriceOffset;	//反向加减价差
		BOOL	bIsBuy;				//是否为买入
		BOOL	bIsMarket;			//是否为市价
		double	fRegPrice;			//委托价
		int		nRegVolume;			//委托数量
		double	fOpenedPriceOffset;	//成交价偏移
		double	fStopLosePrice;		//止损价
		double	fStopGainPrice;		//止盈价
		double	fStopLoseForecast;	//止损预测
		double	fStopGainForecast;	//止盈预测
		char	InstrumentID[32];	//合约代码
		//CThostFtdcInputOrderField req;
	} POSITIONORDER, *PPOSITIONORDER;
	// 必要时把类名由POSITIONORDER改为STOPORDER，这才是个合理的止盈止损的概念


	typedef std::map<int, BATCHORDER> MAPBATCHORDER;
	typedef std::map<int, BATCHORDER>::iterator MAPBATCHORDERITEM;
	typedef std::map<int, BATCHSUBORDER> MAPBATCHSUBORDER;
	typedef std::map<int, BATCHSUBORDER>::iterator MAPBATCHSUBORDERITEM;
	typedef std::map<int, PARKEDORDER> MAPPARKEDORDER;
	typedef std::map<int, PARKEDORDER>::iterator MAPPARKEDORDERITEM;
	typedef std::map<int, CONDITIONORDER> MAPCONDITIONORDER;
	typedef std::map<int, CONDITIONORDER>::iterator MAPCONDITIONORDERITEM;
	typedef std::map<int, POSITIONORDER> MAPPOSITIONORDER;
	typedef std::map<int, POSITIONORDER>::iterator MAPPOSITIONORDERITEM;

public:
	
	static COrderServiceThread* GetInstance();


	COrderServiceThread();//wxWindow *parent, wxWindowID winid);
	~COrderServiceThread(){}

	BOOL InitService();

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnRcvQuot(wxCommandEvent& evt);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
	
	void OnPlatSvrStatusChange(wxCommandEvent& evt);
	void OnStopPlatform(wxCommandEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderServiceThread* CreateThread();
	static int CallBack_InstrumentStatus(const GUIModuleID,const AbstractBusinessData &data);

//	static int	sm_nReqNum;
	static int	sm_nBatchOrderNum;
	static int	sm_nBatchSubOrderNum;
	static int	sm_nParkedOrderNum;
	static int	sm_nConditionOrderNum;
	static int	sm_nPositionOrderNum;

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
	MAPCONDITIONORDER& GetOrderConditionOverdueMap() {
		return m_mapConditionOrder_Overdue;
	};
	MAPPOSITIONORDER& GetOrderPositionMap() {
		return m_mapPositionOrder;
	};
	MAPPOSITIONORDER& GetOrderPositionOverdueMap() {
		return m_mapPositionOrder_Overdue;
	};

	int OrderBatchAdd(BATCHORDER& tOrder, BOOL bIsLock=FALSE) {
		sm_nBatchOrderNum++;
		tOrder.nNum = sm_nBatchOrderNum;
		tOrder.nState = conBatchActive;
		m_mapBatchOrder[tOrder.nNum] = tOrder;
		//m_mapConditionOrder.insert(makepair(tOrder.nNum, tOrder));
		return sm_nBatchOrderNum;
	};
	BOOL OrderBatchDelete(int nNum, BOOL bIsLock=FALSE) {
		return m_mapBatchOrder.erase(nNum);
	};
	BOOL OrderBatchClear(BOOL bIsLock=FALSE) {
		m_mapBatchOrder.clear();
	};
	BOOL OrderBatchActive(int nNum, BOOL bActive, BOOL bIsLock=FALSE) {
		BATCHORDER tOrder={0};
		MAPBATCHORDERITEM it = m_mapBatchOrder.find(nNum);
		if(it != m_mapBatchOrder.end()) {
			tOrder = it->second;
			if(tOrder.nState >= conCondDelete) return FALSE;
			tOrder.nState = bActive ? conBatchActive : conBatchPause;
			m_mapBatchOrder[nNum] = tOrder;
			return TRUE;
		}
		else
			return FALSE;
	};


	int OrderBatchSubAdd(BATCHSUBORDER& tOrder) {
		sm_nBatchSubOrderNum++;
		tOrder.nNum = sm_nBatchSubOrderNum;
		m_mapBatchSubOrder[tOrder.nNum] = tOrder;
		return sm_nBatchSubOrderNum;
	};
	BOOL OrderBatchSubDelete(int nNum) {
		return m_mapBatchSubOrder.erase(nNum);
	};
	BOOL OrderBatchSubClear() {
		m_mapBatchSubOrder.clear();
	};

	int OrderParkedAdd(PARKEDORDER& tOrder) {
		sm_nParkedOrderNum++;
		tOrder.nNum = sm_nParkedOrderNum;
		tOrder.nCreateTime = ::wxGetUTCTime();
		m_mapParkedOrder[tOrder.nNum] = tOrder;
		return sm_nParkedOrderNum;
	};
	BOOL OrderParkedSet(int nNum, PARKEDORDER& tOrder) {
		m_mapParkedOrder[nNum] = tOrder;
		return TRUE;
	};
	BOOL OrderParkedDelete(int nNum) {
		MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
		if(it==m_mapParkedOrder.end()) return FALSE;
		PARKEDORDER tOrder = m_mapParkedOrder[nNum];
		//if(tOrder.nState!=conCondDelete)
		tOrder.nState = conParkedDelete;
		tOrder.nDeleteTime = ::wxGetUTCTime();
		m_mapParkedOrder[nNum] = tOrder;
		return TRUE;
	};
	BOOL OrderParkedClear(BOOL bIsLock=FALSE) {
		m_mapParkedOrder.clear();
	};
	BOOL OrderParkedActive(int nNum, BOOL bActive) {
		PARKEDORDER tOrder={0};
		MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
		if(it != m_mapParkedOrder.end()) {
			tOrder = it->second;
			if(tOrder.nState >= conParkedDelete) return FALSE;
			tOrder.nState = bActive ? conParkedActive : conParkedPause;
			m_mapParkedOrder[nNum] = tOrder;
			return TRUE;
		}
		else
			return FALSE;
	};
	BOOL OrderParkedDoNow(int nNum);

	int OrderConditionAdd(CONDITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		sm_nConditionOrderNum++;
		tOrder.nNum = sm_nConditionOrderNum;
		tOrder.nCreateTime = ::wxGetUTCTime();
		m_mapConditionOrder[tOrder.nNum] = tOrder;
		PostEvent_ConditionStatus(sm_nConditionOrderNum);
		return sm_nConditionOrderNum;
	};
	BOOL OrderConditionSet(int nNum, CONDITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		m_mapConditionOrder[nNum] = tOrder;
		PostEvent_ConditionStatus(nNum);
		return TRUE;
	};
	BOOL OrderConditionAdd_Position(CONDITIONORDER& tOrder1, CONDITIONORDER& tOrder2, BOOL bIsLock=FALSE) {
		sm_nConditionOrderNum++;
		tOrder1.nNum = sm_nConditionOrderNum;
		tOrder1.nState = conCondPause;

		sm_nConditionOrderNum++;
		tOrder2.nNum = sm_nConditionOrderNum;
		tOrder2.nState = conCondPause;
		
		//tOrder1.nRelateCondNum = tOrder2.nNum;
		//tOrder2.nRelateCondNum = tOrder1.nNum;
		
		m_mapConditionOrder[tOrder1.nNum] = tOrder1;
		m_mapConditionOrder[tOrder2.nNum] = tOrder2;
		return TRUE;
	};
	BOOL OrderConditionDelete(int nNum, int nState=conCondDelete, BOOL bIsLock=FALSE, BOOL bIsPostEvent=FALSE) {
		MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
		if(it==m_mapConditionOrder.end()) return FALSE;
		CONDITIONORDER tOrder = m_mapConditionOrder[nNum];
		if(nState==conCondDelete && !(tOrder.nState==conCondPause || tOrder.nState==conCondActive))
			return FALSE;
		tOrder.nState = nState;
		tOrder.nDeleteTime = ::wxGetUTCTime();
		m_mapConditionOrder_Overdue[nNum] = tOrder;
		m_mapConditionOrder.erase(nNum);
		if(bIsPostEvent)
			PostEvent_ConditionStatus(nNum);
		return TRUE;
	};
	BOOL OrderConditionClear(BOOL bIsLock=FALSE) {
		m_mapConditionOrder.clear();
	};
	BOOL OrderConditionActive(int nNum, BOOL bActive, BOOL bIsLock=FALSE) {
		CONDITIONORDER tOrder={0};
		MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
		if(it != m_mapConditionOrder.end()) {
			tOrder = it->second;
			if(tOrder.nState >= conCondDelete) return FALSE;
			tOrder.nState = bActive ? conCondActive : conCondPause;
			m_mapConditionOrder[nNum] = tOrder;
			return TRUE;
		}
		else
			return FALSE;
	};
	BOOL OrderConditionDoNow(int nNum, BOOL bIsLock=FALSE);

	int OrderPositionAdd(POSITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		sm_nPositionOrderNum++;
		tOrder.nNum = sm_nPositionOrderNum;
		m_mapPositionOrder[tOrder.nNum] = tOrder;
		PostEvent_PositionStatus(sm_nPositionOrderNum);
		return sm_nPositionOrderNum;
	};
	BOOL OrderPositionSet(int nNum, POSITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		m_mapPositionOrder[nNum] = tOrder;
		PostEvent_PositionStatus(nNum);
		return TRUE;
	};
	BOOL OrderPositionDelete(int nNum, int nState=conPositionDelete, BOOL bIsLock=FALSE, BOOL bIsPostEvent=FALSE) {
		MAPPOSITIONORDERITEM it = m_mapPositionOrder.find(nNum);
		if(it==m_mapPositionOrder.end()) return FALSE;

		COrderServiceThread::POSITIONORDER tPosOrder={0};
		tPosOrder = it->second;
		if(nState==conPositionDelete && !(tPosOrder.nState==conPositionPause || tPosOrder.nState==conPositionActive))
			return FALSE;
		tPosOrder.nState = nState;
		m_mapPositionOrder.erase(nNum);
		OrderConditionDelete(tPosOrder.nCondNum_StopLose);
		OrderConditionDelete(tPosOrder.nCondNum_StopGain);
		m_mapPositionOrder_Overdue[nNum]=tPosOrder;
		if(bIsPostEvent)
		{
			PostEvent_PositionStatus(nNum);
			// 临时的用法，当界面改为收到事件后只改变相关的记录时，删除这个用法
			PostEvent_ConditionStatus(tPosOrder.nCondNum_StopLose);
		}
		return TRUE;
	};
	BOOL OrderPositionClear(BOOL bIsLock=FALSE) {
		m_mapPositionOrder.clear();
	};

	BOOL HasNoTouchLocalOrder();

	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

	std::map<string, char>& GetExchangeStatusMap() {
		return m_mapExchangeStatus;
	};

	void RegisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.insert(pMsgWindow);
	};
	void UnregisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.erase(pMsgWindow);
	};

private:
	void OrderCondition(CONDITIONORDER& tOrder);
	void OrderParked(PARKEDORDER& tOrder);
	void DoConditionOrder(PlatformStru_DepthMarketData& rawData);
	//void DoPositionOrder(CThostFtdcDepthMarketDataField& rawData);

	void PostEvent_ParkedStatus(int nNum);
	void PostEvent_ConditionStatus(int nNum);
	void PostEvent_PositionStatus(int nNum);
	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

private:
	MAPBATCHORDER		m_mapBatchOrder;			// 本地批量单
	MAPBATCHSUBORDER	m_mapBatchSubOrder;			// 本地批量单子单
	MAPPARKEDORDER		m_mapParkedOrder;			// 本地预埋单
	MAPCONDITIONORDER	m_mapConditionOrder;		// 本地条件单
	MAPCONDITIONORDER	m_mapConditionOrder_Overdue;	// 本地条件单
    MAPPOSITIONORDER	m_mapPositionOrder;			// 本地盈损单
    MAPPOSITIONORDER	m_mapPositionOrder_Overdue;		// 本地盈损单

	std::map<string, char> m_mapExchangeStatus;		// 交易所交易状态
	std::set<void*>		m_setMsgWindow;				// 接收消息的窗口对象指针集合

	wxMutex				m_oMutex;

	string				m_strInvestorID;

};