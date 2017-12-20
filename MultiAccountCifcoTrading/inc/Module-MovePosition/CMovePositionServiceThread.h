#pragma once
#include "stdafx.h"
#include <vector>
#include "CfgMgr/CfgGlobal.h"

#include <wx/listctrl.h>


class CMovePositionServiceThread : public wxEvtHandler//, wxThread
{
public:
	// 处理价格类型
	enum {	conPriceType_Market,		// 市价
			conPriceType_LastPrice,		// 最新价
			conPriceType_BuySellPrice,	// 买一卖一价
			conPriceType_Limit			// 指定价
	};

	// 仓位平开仓超时后的活动 
	enum {	conTOutAction_Nothing,		// 无动作
			conTOutAction_LastPrice,	// 按最新价浮动改单
			conTOutAction_BuySellPrice,	// 按买卖价浮动改单
			conTOutAction_Market,		// 按市价改单
			conTOutAction_CancelOrder	// 撤单
	};

	// 下撤单请求状态状态
	enum {	conPosRegWaitingSend,		// 未发送
			conPosRegOrderWaiting,		// 挂单等待应答
			conPosRegOrderHasResponsed,	// 报单已应答
			conPosRegOrderFail,			// 挂单失败
			conPosRegOrderCompleted,	// 全部成交
			conPosRegCancelWaiting,		// 撤单等待
			conPosRegCancelHasResponsed,	// 报单已应答
			conPosRegCancelFail,		// 撤单失败
			conPosRegCancel				// 撤销成功
	};

	// 仓位状态
	enum {	conPositionPause,			// 暂停
			conPositionActive,			// 激活
			//conPositionOrderWaiting,	// 挂单等待成交
			//conPositionOrderFail,		// 挂单失败
			//conPositionCancelWaiting,	// 撤单等待
			//conPositionCancelFail,	// 撤单失败
			conPositionCancel,			// 已撤销活动
			conPositionCompleted		// 全部成交
	};

	// 移仓状态
	enum {	conMovePause,				// 暂停
			conMoveActive,				// 激活
			conMoveCancel,				// 已撤销活动
			conMoveCompleted			// 完成
	};

	// 仓位互换状态
	enum {	conSwapPause,				// 暂停
			conSwapActive,				// 激活
			conSwapCancel,				// 已撤销活动
			conSwapCompleted			// 完成
	};

	enum {	conActionPause,
			conActionMoving,			//1 开始移仓或仓位互换
			conActionUserChanging,		//2 执行外围改单
			conActionAutoChanging,		//3 执行超时改单
			conActionChanged,			//4 改单完成
			conActionCompleted,			//5 移仓完成
			conActionCancel,			//6 用户撤销
			conActionCancel_TimeOut,	//7 超时撤销
			conActionCancel_Exciption,	//8 异常撤销
			conActionMoveCompleted,		//9 移仓单完成
			conActionSwapCompleted		//10仓位互换完成
	};

	typedef struct {
		DWORD	dwDate;
		DWORD	dwTime;
		DWORD	dwMovePosNum;
		DWORD	dwSwapPosNum;
		DWORD	dwMovePosCount;
		DWORD	dwMovePosOffset;
		DWORD	dwMovePosSize;
		DWORD	dwSwapPosCount;
		DWORD	dwSwapPosOffset;
		DWORD	dwSwapPosSize;
		DWORD	dwBak1[24];
	} FILEHEAD, *PFILEHEAD;		// sizeof()=64


	// 仓位描述
	typedef struct {
		InstrumentIDType	strInstrumentID;// 合约代码
		double		fPriceTick;
		BOOL 		bDirection;			// 买卖方向, TRUE 买入, FALSE 卖出
		BOOL		bHedge;				// 投机或保值
		BOOL		bIsClose;			// 是否为平仓
		int			nPriceType;			// 处理价格类型, 0 市价, 1 最新价, 2 买一卖一价, 3 指定价
		BOOL		bSimulate;			// 用涨跌停价模拟市价
		double		fPrice;				// 价格
		int			nPriceOffset;		// 滑动价差, 表示为价格变动单位的乘数
		int			nVolume;			// 持仓数量
		int			nTimeOutAction;		// 原始仓位平仓超时后的活动, 0 无动作, 1 按最新价浮动改单, 2 按买卖价浮动改单, 3 按市价改单, 4 撤单, 5 弹出价格修改对话框
		int			nTimeOutPriceOffset;// 原始仓位滑动价差

		int			nState;				// 活动状态, 见 仓位状态enum
		int			nRemainTime;		// 剩余时间，单位：秒
		int			nRetryCount;		// 已重试次数

		BOOL		bIsAuto;			// 执行自动单
		int			nRegState;			// 下撤单状态
		int			nRegNum;			// 请求号
		int			nCancelRegNum;		// 撤单请求号
		int			nRemainVolume;		// 剩余数量
		char		strOrderSysID[32];	// 委托单编号
		PlatformStru_InputOrder				req;			// 仓位平开仓请求
		PlatformStru_InputOrderAction		cancelAction;
		
		BOOL		bIsAuto_CloseToday;				// 执行自动单
		int			nRegState_CloseToday;			// 下撤单状态
		int			nRegNum_CloseToday;				// 请求号
		int			nCancelRegNum_CloseToday;		// 撤单请求号
		int			nRemainVolume_CloseToday;		// 剩余数量
		char		strOrderSysID_CloseToday[32];	// 委托单编号
		PlatformStru_InputOrder				req_CloseToday;	// 仓位平今请求
		PlatformStru_InputOrderAction		cancelAction_CloseToday;

	} POSITIONUNIT, *PPOSITIONUNIT;


	// 移仓结构体
	typedef struct {
		int			nNum;				// 序号
		int			nState;				// 活动状态, 见 移仓状态enum
		BOOL		bPauseTimer;		// 暂停超时计数(倒计时)
		void*		pWnd;
		POSITIONUNIT	tSrcPos;		// 原始仓位
		POSITIONUNIT	tDestPos;		// 目标仓位
		int			nActionMode;		// 操作顺序, 0 先平后开, 1 同时开平
		int			nTimeOutInterval;	// 超时间隔，单位：秒
		int			nRetryNum;			// 超时重试次数
		int			nRelateMode;		// 关联关系, 0 独立无关联, 1 是仓位互换的移仓单
		int			nRelateID;			// 关联的ID号, 如仓位互换的单号
	} MOVEPOSITION, *PMOVEPOSITION;

	// 仓位互换结构体
	typedef struct {
		int			nNum;				// 序号
		int			nState;				// 活动状态, 见 仓位互换状态enum
		int			nMovePos1Num;		// 仓位1编号
		int			nMovePos2Num;		// 仓位2编号
		int			nActionMode;		// 操作顺序, 0 同时进行, 1 等待仓位1先完成, 2 等待仓位2先完成
	} SWAPPOSITION, *PSWAPPOSITION;

	typedef std::map<int, MOVEPOSITION> MAPMOVEPOSITION;
	typedef std::map<int, MOVEPOSITION>::iterator MAPMOVEPOSITIONITEM;
	typedef std::map<int, SWAPPOSITION> MAPSWAPPOSITION;
	typedef std::map<int, SWAPPOSITION>::iterator MAPSWAPPOSITIONITEM;

public:
	
	static CMovePositionServiceThread* GetInstance();
	static void ReleaseInstance();


	CMovePositionServiceThread();//wxWindow *parent, wxWindowID winid);
	~CMovePositionServiceThread();

	BOOL InitService();

	void OnTimer();
	//void OnTimer(wxTimerEvent& evt);
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
	
	DECLARE_EVENT_TABLE()
public:
    static CMovePositionServiceThread* CreateThread();
	static BOOL MakeReq(wxString& strInstrumentID, BOOL bDirection, int nOCMode, 
					 double fPrice, int nVolume, BOOL bIsHedge, BOOL bIsMarketPrice, 
					 BOOL bSimulate, PlatformStru_InputOrder& req);
	//static BOOL MakeReq(wxString& strInstrumentID, BOOL bDirection, int nOCMode, 
	//				 double fPrice, int nVolume, BOOL bIsHedge, BOOL bIsMarketPrice, 
	//				 BOOL bSimulate, CThostFtdcInputOrderField& req);

	static int RspOrderInsertCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);

	static int	sm_nMovePosNum;
	static int	sm_nSwapPosNum;

public:
	MAPMOVEPOSITION& GetMovePostionMap() {
		return m_mapMovePosition;
	};
	MAPSWAPPOSITION& GetSwapPositionMap() {
		return m_mapSwapPosition;
	};

	// 新增一个移仓单，返回值: 这个移仓的底层标号
	int MovePositionAdd(MOVEPOSITION& tMove) {
		sm_nMovePosNum++;
		tMove.nNum = sm_nMovePosNum;
		tMove.nRelateMode = 0;
		tMove.nState = conMovePause;
		tMove.tSrcPos.nState = conPositionPause;
		tMove.tDestPos.nState = conPositionPause;
		m_mapMovePosition[tMove.nNum] = tMove;
		return tMove.nNum;
	};
	//// 重新给标号为nNum的移仓单赋值
	//BOOL MovePositionSet(int nNum, MOVEPOSITION& tMove) {
	//	tMove.nNum = nNum;
	//	m_mapMovePosition[nNum] = tMove;
	//	return TRUE;
	//};
	//// 仓位互换同时下两个移仓单
	//BOOL MovePositionAdd_SwapPosition(MOVEPOSITION& tMove1, MOVEPOSITION& tMove2) {
	//	sm_nMovePosNum++;
	//	tMove1.nNum = sm_nMovePosNum;
	//	//tMove1.nState = conPositionPause;

	//	sm_nMovePosNum++;
	//	tMove2.nNum = sm_nMovePosNum;
	//	//tMove2.nState = conPositionPause;
	//	
	//	m_mapMovePosition[tMove1.nNum] = tMove1;
	//	m_mapMovePosition[tMove2.nNum] = tMove2;
	//	return TRUE;
	//};
	// 撤销编号为nNum的移仓单，使其处于终止状态
	BOOL MovePositionCancel(int nNum, int nState=conMoveCancel) {
		MAPMOVEPOSITIONITEM it = m_mapMovePosition.find(nNum);
		if(it==m_mapMovePosition.end()) 
			return FALSE;
		//if(it->second.nState!=conCondDelete)
		it->second.nState = nState;
		//it->second.nDeleteTime = ::wxGetUTCTime();
		return TRUE;
	};
	// 撤销编号为nNum的移仓单，使其处于终止状态
	BOOL MovePositionPauseTimer(int nNum, BOOL nPauseTimer=TRUE) {
		MAPMOVEPOSITIONITEM it = m_mapMovePosition.find(nNum);
		if(it==m_mapMovePosition.end()) 
			return FALSE;
		//if(it->second.nState!=conCondDelete)
		it->second.bPauseTimer = nPauseTimer;
		//it->second.nDeleteTime = ::wxGetUTCTime();
		return TRUE;
	};
	// 清除所有的移仓单
	BOOL MovePositionClear() {
		m_mapMovePosition.clear();
	};
	// 执行编号为nNum的移仓单
	BOOL MovePositionDoNow(int nNum);
	// 重新执行编号为nNum的移仓单
	BOOL MovePositionRedo(int nNum, MOVEPOSITION& tTmpMove, BOOL& bIsEqual);
	// 某个移仓单是否有改单正在进行中	
	BOOL MovePositionCheckAction(int nNum, int& nAction);	

	// 新增一个仓位互换单，返回值: 这个仓位互换单的底层标号
	//int SwapPositionAdd(SWAPPOSITION& tSwap) {
	//	sm_nSwapPosNum++;
	//	tSwap.nNum = sm_nSwapPosNum;
	//	m_mapSwapPosition[tSwap.nNum] = tSwap;
	//	return sm_nSwapPosNum;
	//};
	// 新增一个仓位互换单，返回值: 这个仓位互换单的底层标号
	int SwapPositionAdd(SWAPPOSITION& tSwap, MOVEPOSITION& tMove1, MOVEPOSITION& tMove2) {
		sm_nSwapPosNum++;
		tSwap.nNum = sm_nSwapPosNum;
		tSwap.nState = conSwapPause;

		sm_nMovePosNum++;
		tMove1.nNum = sm_nMovePosNum;
		tMove1.nRelateMode = 1;
		tMove1.nRelateID = tSwap.nNum;
		tMove1.nState = conPositionPause;
		tMove1.tSrcPos.nState = conPositionPause;
		tMove1.tDestPos.nState = conPositionPause;
		tSwap.nMovePos1Num = tMove1.nNum;

		sm_nMovePosNum++;
		tMove2.nNum = sm_nMovePosNum;
		tMove2.nRelateMode = 1;
		tMove2.nRelateID = tSwap.nNum;
		tMove2.nState = conPositionPause;
		tMove2.tSrcPos.nState = conPositionPause;
		tMove2.tDestPos.nState = conPositionPause;
		tSwap.nMovePos2Num = tMove2.nNum;
		
		m_mapMovePosition[tMove1.nNum] = tMove1;
		m_mapMovePosition[tMove2.nNum] = tMove2;

		m_mapSwapPosition[tSwap.nNum] = tSwap;
		return sm_nSwapPosNum;
	};
	//// 重新给标号为nNum的移仓单赋值
	//BOOL SwapPositionSet(int nNum, SWAPPOSITION& tSwap) {
	//	tSwap.nNum = nNum;
	//	m_mapSwapPosition[nNum] = tSwap;
	//	return TRUE;
	//};
	// 撤销编号为nNum的移仓单，使其处于终止状态
	BOOL SwapPositionCancel(int nNum, int nState=conSwapCancel) {
		MAPSWAPPOSITIONITEM it = m_mapSwapPosition.find(nNum);
		if(it==m_mapSwapPosition.end()) 
			return FALSE;
		SWAPPOSITION& tSwap = it->second;
		tSwap.nState = nState;
		MovePositionCancel(tSwap.nMovePos1Num);
		MovePositionCancel(tSwap.nMovePos2Num);
		return TRUE;
	};
	// 清除所有的移仓单
	BOOL SwapPositionClear() {
		m_mapSwapPosition.clear();
	};
	// 执行编号为nNum的仓位互换单
	BOOL SwapPositionDoNow(int nNum);
	// 重新执行编号为nNum的仓位互换单
	BOOL SwapPositionRedo(int nNum, MOVEPOSITION& tMove1, MOVEPOSITION& tMove2, BOOL& bIsEqual);
	// 某个移仓单是否有改单正在进行中	
	BOOL SwapPositionCheckAction(int nNum, int& nAction);	


	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};


private:
	BOOL OrderInsert(POSITIONUNIT& tPosition, BOOL bIsCloseToday = FALSE);
	BOOL OrderCancel(POSITIONUNIT& tPosition, BOOL bIsCloseToday = FALSE);
	BOOL DoPositionOrder(POSITIONUNIT& tPosition, BOOL bIsTimeOut = FALSE);
	BOOL DoPositionOrder_ContinueTOut(POSITIONUNIT& tPosition, BOOL bIsCloseToday = FALSE);

	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

private:
	MAPMOVEPOSITION		m_mapMovePosition;			// 移仓单
	MAPSWAPPOSITION		m_mapSwapPosition;			// 仓位互换单

	wxMutex				m_oMutex;

	string				m_strInvestorID;

};
