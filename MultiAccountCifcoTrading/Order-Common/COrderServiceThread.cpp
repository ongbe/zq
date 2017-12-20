#include "StdAfx.h"
#include "COrderServiceThread.h"
#include "wx/file.h"
#include "wx/filename.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;



// 还缺一个响应综合交易平台状态改变的回调函数，在这个函数里发送一个事件，
// 在事件响应中到综合交易平台查询预埋单、预埋撤单的变化情况，因为没有发现综合交易平台会返回预埋发送事件



//COrderServiceThread* s_poOrderServiceThread = NULL;
COrderServiceThread* g_poOrderServiceThread = NULL;
COrderServiceThread g_oOrderServiceThread;

//合约交易状态通知回调函数
int COrderServiceThread::CallBack_InstrumentStatus(const GUIModuleID,const AbstractBusinessData &data)
{
 	if(g_poOrderServiceThread == NULL)
		return 0;
	
	if(data.BID!=BID_RtnInstrumentStatus || data.Size!=sizeof(DataInstrumentStatus))
		return 0;	

	wxCommandEvent evt(wxEVT_PLATFORMSVR_STATUSCHANGE, wxID_ANY);
	DataInstrumentStatus* pTmp=new DataInstrumentStatus();
	memcpy(pTmp, &data, sizeof(DataInstrumentStatus));
	evt.SetClientData(pTmp);
	g_poOrderServiceThread->AddPendingEvent(evt);


	return 0;
}

int funcDepthMarketCallBack(const GUIModuleID,const AbstractBusinessData& data)
{
//	WXLOG_INFO("COrderServiceThread::funcDepthMarketCallBack");
	if(data.BID!=BID_RtnDepthMarketData || data.Size!=sizeof(DataRtnDepthMarketData))
		return 0;

	if(!g_poOrderServiceThread)
		return 0;

	DataRtnDepthMarketData* pTmp=new DataRtnDepthMarketData();
	memcpy(pTmp, &data, sizeof(DataRtnDepthMarketData));

	//post event to CQuotPanel
	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_QUOT, wxID_ANY);
	evt.SetClientData(pTmp);
	g_poOrderServiceThread->AddPendingEvent(evt);

	return 0;
}

int RspOrderInsertCallBackFunc(const GUIModuleID,const AbstractBusinessData& data)
{
	if(!g_poOrderServiceThread)
		return 0;

	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_ORDER, wxID_ANY);
	if(data.BID==BID_RspOrderInsert && data.Size==sizeof(DataRspOrderInsert))
	{
		DataRspOrderInsert* pTmp=new DataRspOrderInsert();
		memcpy(pTmp, &data, sizeof(DataRspOrderInsert));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RtnOrder && data.Size==sizeof(DataRtnOrder))
	{
		DataRtnOrder* pTmp=new DataRtnOrder();
		memcpy(pTmp, &data, sizeof(DataRtnOrder));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction1 && data.Size==sizeof(DataRspOrderAction1))
	{
		DataRspOrderAction1* pTmp=new DataRspOrderAction1();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction1));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction2 && data.Size==sizeof(DataRspOrderAction2))
	{
		DataRspOrderAction2* pTmp=new DataRspOrderAction2();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction2));
		evt.SetClientData(pTmp);
	}
	else
	{
		return 0;
	}

	//post event to OrderInsertPanel
	g_poOrderServiceThread->AddPendingEvent(evt);

	return 0;
}

int TradeCallBackFunc(const GUIModuleID,const AbstractBusinessData &data)	  //成交回报
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
		return 0;

	if(!g_poOrderServiceThread)
		return 0;

	DataRtnTrade* pTmp=new DataRtnTrade();
	memcpy(pTmp, &data, sizeof(DataRtnTrade));

	//post event to COrderServiceThread
	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_TRADE, wxID_ANY);
	evt.SetClientData(pTmp);
	
	g_poOrderServiceThread->AddPendingEvent(evt);
	return 0;
}

//int	COrderServiceThread::sm_nReqNum=0;
int	COrderServiceThread::sm_nBatchOrderNum=0;
int	COrderServiceThread::sm_nBatchSubOrderNum=0;
int	COrderServiceThread::sm_nParkedOrderNum=0;
int	COrderServiceThread::sm_nConditionOrderNum=0;
int	COrderServiceThread::sm_nPositionOrderNum=0;

COrderServiceThread* COrderServiceThread::GetInstance()
{
	if(g_poOrderServiceThread == NULL) 
		g_poOrderServiceThread = &g_oOrderServiceThread;
	return g_poOrderServiceThread;
}


BEGIN_EVENT_TABLE(COrderServiceThread, wxEvtHandler)//wxPanel)
    EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_QUOT, OnRcvQuot)
    EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_ORDER, OnRcvOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_TRADE, OnRcvTrade)
	EVT_COMMAND(wxID_ANY, wxEVT_PLATFORMSVR_STATUSCHANGE, OnPlatSvrStatusChange)
	EVT_COMMAND(wxID_ANY, wxEVT_STOP_PLATFORM,OnStopPlatform)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderServiceThread::COrderServiceThread():
//wxThread(wxTHREAD_DETACHED),
wxEvtHandler()
{
}

void COrderServiceThread::OnStopPlatform(wxCommandEvent& evt)
{

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnInstrumentStatus, GID_ORDER_SERVICETHREAD);

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_ORDER_SERVICETHREAD);

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnTrade, GID_ORDER_SERVICETHREAD);	//订阅成交回报

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnOrder, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction1, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction2, GID_ORDER_SERVICETHREAD);

	m_setMsgWindow.clear();
	SaveDataToFile();

}

COrderServiceThread* COrderServiceThread::CreateThread()
{
    //wxThread* pThread = new COrderServiceThread();
	return NULL;//pThread;
}

BOOL COrderServiceThread::InitService()
{

	string strBrokerID="";
	m_strInvestorID="";
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(m_strInvestorID,strBrokerID);
	LoadDataFromFile();
	
	//s_poOrderServiceThread = this;

	//定制业务信息
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnInstrumentStatus, GID_ORDER_SERVICETHREAD, CallBack_InstrumentStatus);

	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnDepthMarketData, GID_ORDER_SERVICETHREAD, funcDepthMarketCallBack);

	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTrade, GID_ORDER_SERVICETHREAD, TradeCallBackFunc);	//订阅成交回报

	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_SERVICETHREAD, RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_ORDER_SERVICETHREAD, RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction1, GID_ORDER_SERVICETHREAD, RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction2, GID_ORDER_SERVICETHREAD, RspOrderInsertCallBackFunc);
	return TRUE;

}

void COrderServiceThread::OnInitSubscribe(wxCommandEvent& evt)
{
	InitService();
}

void COrderServiceThread::OnExit()
{
}

void* COrderServiceThread::Entry()
{
	return NULL;
}

void COrderServiceThread::OrderParked(PARKEDORDER& tOrder)
{
	
	//CThostFtdcInputOrderField req;
	//ZeroMemory(&req, sizeof(CThostFtdcInputOrderField));

	PlatformStru_InputOrder& req=tOrder.req;

/*
	//req.RequestID = ++sm_nReqNum;

	//组合投机套保标志，只有第一个字节有效
	if(!tOrder.bHedge)
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	else
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;
	
	strncpy(req.InstrumentID, tOrder.InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	req.Direction = tOrder.Direction;
	
	strncpy(req.CombOffsetFlag, tOrder.CombOffsetFlag, sizeof(TThostFtdcCombOffsetFlagType));
	req.ContingentCondition = THOST_FTDC_CC_Immediately; 
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	req.LimitPrice = tOrder.LimitPrice;
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	req.TimeCondition = THOST_FTDC_TC_GFD;
	req.VolumeTotalOriginal = tOrder.VolumeTotalOriginal;
	req.VolumeCondition = THOST_FTDC_VC_AV; 
	req.MinVolume = 1;
*/
	int ret = 0;

    ret=DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;

	tOrder.nRegNum=req.RequestID;//sm_nReqNum;
	tOrder.nState = conParkedConfirm;
	tOrder.nRegState=(ret==0 ? conReqSendOk : conReqSendFail);
	if(ret!=0) {
		//sm_nReqNum--;
		//设置错误信息
		strcpy(tOrder.strErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
	}
	else {
		strcpy(tOrder.strErrorMsg, LOADSTRING(SENT_SERVER_SUCCESS));
	}
	PostEvent_ParkedStatus(tOrder.nNum);
}

void COrderServiceThread::OrderCondition(CONDITIONORDER& tOrder)
{
	
	//CThostFtdcInputOrderField req;
	//ZeroMemory(&req, sizeof(CThostFtdcInputOrderField));

	PlatformStru_InputOrder& req=tOrder.req;

/*
	//req.RequestID = ++sm_nReqNum;

	//组合投机套保标志，只有第一个字节有效
	if(!tOrder.bHedge)
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	else
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;
	
	strncpy(req.InstrumentID, tOrder.InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	req.Direction = tOrder.Direction;
	
	strncpy(req.CombOffsetFlag, tOrder.CombOffsetFlag, sizeof(TThostFtdcCombOffsetFlagType));
	req.ContingentCondition = THOST_FTDC_CC_Immediately; 
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	req.LimitPrice = tOrder.LimitPrice;
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	req.TimeCondition = THOST_FTDC_TC_GFD;
	req.VolumeTotalOriginal = tOrder.VolumeTotalOriginal;
	req.VolumeCondition = THOST_FTDC_VC_AV; 
	req.MinVolume = 1;
*/
	int ret = 0;

    ret=DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;
	tOrder.nRegNum=req.RequestID;//sm_nReqNum;
	tOrder.nState = conCondConfirm;
	tOrder.nRegState=(ret==0 ? conReqSendOk : conReqSendFail);
	if(ret!=0) {
		//sm_nReqNum--;
		//设置错误信息
		strcpy(tOrder.strErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
	}
	else {
		strcpy(tOrder.strErrorMsg, LOADSTRING(SENT_SERVER_SUCCESS));
	}
	PostEvent_ConditionStatus(tOrder.nNum);

}

void COrderServiceThread::DoConditionOrder(PlatformStru_DepthMarketData& rawData)
{

	CONDITIONORDER tOrder;
	std::string strInputCode(rawData.InstrumentID);
	double fInputPrice = 0.0f;

	std::vector<int> listDelete;
	listDelete.clear();

	for(MAPCONDITIONORDERITEM it=m_mapConditionOrder.begin(); it!=m_mapConditionOrder.end(); ++it) {
		tOrder = it->second;
		if(!(tOrder.nState==conCondActive))
			continue;
		if(strInputCode.compare(tOrder.req.InstrumentID) == 0) {
			switch(tOrder.nPriceType) {
			case 1:
				fInputPrice = rawData.LastPrice;
				break;
			case 2:
				fInputPrice = rawData.BidPrice1;
				break;
			case 3:
				fInputPrice = rawData.AskPrice1;
				break;
			};
			if(!util::isInvalidValue(fInputPrice)) {
				if(tOrder.nConditionType==1) {
					if(fInputPrice <= tOrder.fConditionPrice) {
						OrderCondition(tOrder);
						m_mapConditionOrder[it->first] = tOrder;
						listDelete.push_back(it->first);
						//listDelete.push_back(tOrder.nRelateCondNum);
					}
				}
				else if(tOrder.nConditionType==2) {
					if(fInputPrice >= tOrder.fConditionPrice) {
						OrderCondition(tOrder);
						m_mapConditionOrder[it->first] = tOrder;
						listDelete.push_back(it->first);
						//listDelete.push_back(tOrder.nRelateCondNum);
					}
				}
				else if(tOrder.nConditionType==3) {
					if(fInputPrice < tOrder.fConditionPrice) {
						OrderCondition(tOrder);
						m_mapConditionOrder[it->first] = tOrder;
						listDelete.push_back(it->first);
						//listDelete.push_back(tOrder.nRelateCondNum);
					}
				}
				else if(tOrder.nConditionType==4) {
					if(fInputPrice > tOrder.fConditionPrice) {
						OrderCondition(tOrder);
						m_mapConditionOrder[it->first] = tOrder;
						listDelete.push_back(it->first);
						//listDelete.push_back(tOrder.nRelateCondNum);
					}
				}
			}
		}
    }

	int nNum = 0;
	POSITIONORDER tPosOrder={0};
	MAPPOSITIONORDERITEM itPos;
	
	for(int i = 0; i!=listDelete.size(); i++) {
		nNum = listDelete[i];
		tOrder = m_mapConditionOrder[nNum];
		OrderConditionDelete(nNum, conCondConfirm);
		PostEvent_ConditionStatus(nNum);
		if(tOrder.nCondPosRelation == conCPRCondFromPos) {
			itPos = m_mapPositionOrder.find(tOrder.nPosNum);
			if(itPos != m_mapPositionOrder.end()) {
				tPosOrder = itPos->second;
				//tPosOrder.nOpenNum = tOrder.nRegNum;
				tPosOrder.nState = conPositionConfirm;
				m_mapPositionOrder[tPosOrder.nNum]=tPosOrder;
				OrderPositionDelete(tOrder.nPosNum, conPositionConfirm);
				PostEvent_PositionStatus(tPosOrder.nNum);
			}
		}
	}
	listDelete.clear();

}

BOOL COrderServiceThread::OrderParkedDoNow(int nNum)
{
	PARKEDORDER tOrder={0};
	MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
	if(it != m_mapParkedOrder.end()) {
		tOrder = it->second;
		if(tOrder.nState != conCondActive) return FALSE;
		OrderParked(tOrder);
		m_mapParkedOrder[nNum]=tOrder;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL COrderServiceThread::OrderConditionDoNow(int nNum, BOOL bIsLock)
{
	CONDITIONORDER tOrder={0};
	MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
	if(it != m_mapConditionOrder.end()) {
		tOrder = it->second;
		if(tOrder.nState != conCondActive) return FALSE;
		OrderCondition(tOrder);
		m_mapConditionOrder[nNum]=tOrder;
		OrderConditionDelete(nNum, conCondConfirm);
		return TRUE;
	}
	else
		return FALSE;
}

//底层平台发过来一条新行情
void COrderServiceThread::OnRcvQuot(wxCommandEvent& evt)
{

    DataRtnDepthMarketData* pTmp = (DataRtnDepthMarketData*)(evt.GetClientData());
    if(!pTmp) {
        wxASSERT(pTmp);
        return;
    }

	LockObject();

//    const CThostFtdcDepthMarketDataField& rawData = pTmp->DepthMarketDataField;
	DoConditionOrder(pTmp->DepthMarketDataField);
	//DoPositionOrder(pTmp->DepthMarketDataField);

	UnlockObject();

    wxDELETE(pTmp)
    evt.SetClientData(NULL);

}

void COrderServiceThread::OnRcvOrder(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}
	
	LockObject();

	switch (pAbstractBusinessData->BID) {
		case BID_RtnOrder:
			{
				DataRtnOrder& RawData= *(DataRtnOrder*)(pAbstractBusinessData);

				MAPCONDITIONORDERITEM itCond;
				MAPPOSITIONORDERITEM itPos;
				CONDITIONORDER tCondOrder={0};
				POSITIONORDER tPosOrder={0};
				for(itCond=m_mapConditionOrder_Overdue.begin(); 
						itCond!=m_mapConditionOrder_Overdue.end(); itCond++) {
					tCondOrder = itCond->second;
					if(tCondOrder.nRegNum == RawData.OrderField.RequestID) {
						if(tCondOrder.nState==conCondPause) break;
						tCondOrder.nEntrustNum = atoi(RawData.OrderField.OrderSysID);
						switch(RawData.OrderField.OrderStatus) {
						case THOST_FTDC_OST_Canceled://下单后，CTP自动撤单
							tCondOrder.nRegState = conReqEntrustFail;
							tCondOrder.nState = conCondCancel;
							m_mapConditionOrder_Overdue[itCond->first]=tCondOrder;
							PostEvent_ConditionStatus(tCondOrder.nNum);
							break;
						case THOST_FTDC_OST_NoTradeQueueing://下单后，无法立即成交，处于委托状态
							tCondOrder.nRegState = conReqEntrustOk;
							tCondOrder.nState = conCondConfirm;
							m_mapConditionOrder_Overdue[itCond->first]=tCondOrder;
							PostEvent_ConditionStatus(tCondOrder.nNum);
							break;
						case THOST_FTDC_OST_AllTraded:      //下单后，立即成交
							tCondOrder.nRegState = conReqDealClose;
							tCondOrder.nState = conCondCompleted;
							m_mapConditionOrder_Overdue[itCond->first]=tCondOrder;
							PostEvent_ConditionStatus(tCondOrder.nNum);
							break;
						default:
							break;
						};
						if(tCondOrder.nCondPosRelation == conCPRCondFromPos) {
						}
						break;
					}
				}
				for(itPos=m_mapPositionOrder.begin(); itPos!=m_mapPositionOrder.end(); itPos++) {
					tPosOrder = itPos->second;
					if(tPosOrder.nRegNum == RawData.OrderField.RequestID) {
						if(tPosOrder.nState==conPositionPause) break;
						tPosOrder.nOpenNum = atoi(RawData.OrderField.OrderSysID);
						switch(RawData.OrderField.OrderStatus) {
						case THOST_FTDC_OST_Canceled://下单后，CTP自动撤单
							tPosOrder.nRegState = conReqEntrustFail;
							tPosOrder.nState = conPositionCancel;
							m_mapPositionOrder[itPos->first]=tPosOrder;
							OrderPositionDelete(itPos->first, conPositionCancel);
							PostEvent_PositionStatus(tPosOrder.nNum);
							PostEvent_ConditionStatus(tPosOrder.nCondNum_StopLose);
							//PostEvent_ConditionStatus(tPosOrder.nCondNum_StopGain);
							break;
						case THOST_FTDC_OST_NoTradeQueueing://下单后，无法立即成交，处于委托状态
							tPosOrder.nRegState = conReqEntrustOk;
							m_mapPositionOrder[itPos->first]=tPosOrder;
							PostEvent_PositionStatus(tPosOrder.nNum);
							break;
						case THOST_FTDC_OST_AllTraded:      //下单后，立即成交
							tPosOrder.nRegState = conReqDealClose;
							m_mapPositionOrder[itPos->first]=tPosOrder;
							PostEvent_PositionStatus(tPosOrder.nNum);
							//// 成交后激活对应的止盈止损条件单
							//if(tPosOrder.nCondNum_StopLose>0)
							//	OrderConditionActive(tPosOrder.nCondNum_StopLose, TRUE);
							//if(tPosOrder.nCondNum_StopGain>0)
							//	OrderConditionActive(tPosOrder.nCondNum_StopGain, TRUE);
							break;
						default:
							break;
						};
						break;
					}
				}
			}
			break;
		case BID_RspOrderInsert:
			{
				DataRspOrderInsert& RawData= *(DataRspOrderInsert*)(pAbstractBusinessData);
				MAPCONDITIONORDERITEM itCond;
				MAPPOSITIONORDERITEM itPos;
				CONDITIONORDER tCondOrder={0};
				POSITIONORDER tPosOrder={0};
				for(itCond=m_mapConditionOrder_Overdue.begin(); 
						itCond!=m_mapConditionOrder_Overdue.end(); itCond++) {
					tCondOrder = itCond->second;
					if(tCondOrder.nRegNum == RawData.InputOrderField.RequestID) {
						if(tCondOrder.nState==conCondPause) break;
						tCondOrder.nRegState = conReqEntrustFail;
						tCondOrder.nState = conCondCancel;
						m_mapConditionOrder_Overdue[itCond->first]=tCondOrder;
						PostEvent_ConditionStatus(tCondOrder.nNum);
						break;
					}
				}
				for(itPos=m_mapPositionOrder.begin(); itPos!=m_mapPositionOrder.end(); itPos++) {
					tPosOrder = itPos->second;
					if(tPosOrder.nRegNum == RawData.InputOrderField.RequestID) {
						if(tPosOrder.nState==conPositionPause) break;
						tPosOrder.nRegState = conReqEntrustFail;
						tPosOrder.nState = conPositionCancel;
						m_mapPositionOrder[itPos->first]=tPosOrder;
						OrderPositionDelete(itPos->first, conPositionCancel);
						PostEvent_PositionStatus(tPosOrder.nNum);
						break;
					}
				}
			}
		case BID_RspOrderAction1:
			{
				//if(!m_bWaitingRsp_OrderDelete)
				//	break;
				//DataRspOrderAction1& RawData= *(DataRspOrderAction1*)(pAbstractBusinessData);
				//if(RawData.InputOrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
				//	break;

				//wxASSERT(RawData.RspInfoField.ErrorID);//ErrorID肯定是非零

				//ShowTradeInfoDlg(wxString("操作失败"), RawData);
				//m_bWaitingRsp_OrderDelete=false;
			}
		case BID_RspOrderAction2:
			{
				//if(!m_bWaitingRsp_OrderDelete)
				//	break;
				//DataRspOrderAction2& RawData= *(DataRspOrderAction2*)(pAbstractBusinessData);
				//if(RawData.OrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
				//	break;			

				//ShowTradeInfoDlg(wxString("操作失败"), RawData.OrderActionField);
				//m_bWaitingRsp_OrderDelete=false;
			}
			break;
		default:
			break;
	};

	UnlockObject();
	
	wxDELETE(pAbstractBusinessData)
	evt.SetClientData(NULL);
}

void COrderServiceThread::OnRcvTrade(wxCommandEvent& evt)
{
//	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
	DataRtnTrade* pRawData= (DataRtnTrade*)(evt.GetClientData());
	if(!pRawData) {
		wxASSERT(pRawData);
		return;
	}

	MAPCONDITIONORDERITEM itCond;
	MAPPOSITIONORDERITEM itPos;
	CONDITIONORDER tCondOrder={0};
	POSITIONORDER tPosOrder={0};

	LockObject();

	for(itPos=m_mapPositionOrder.begin(); itPos!=m_mapPositionOrder.end(); itPos++) {
		tPosOrder = itPos->second;
		if(tPosOrder.nOpenNum == atoi(pRawData->TradeField.OrderSysID)) {		//Price // 成交价
			if(tPosOrder.nState==conPositionPause) break;	// 如果暂停，退出处理
			if(tPosOrder.fOpenedPriceOffset != 0.0f) break;	// 已处理过，退出处理
			//tPosOrder.nState = ;
			tPosOrder.nRegState = conReqDealClose;
			tPosOrder.fOpenedPriceOffset = pRawData->TradeField.Price - tPosOrder.fRegPrice;
			if(tPosOrder.nPriceOffsetType==1) {				// 如果比较价格为成交价
				if(tPosOrder.nCondNum_StopGain>0)
					tPosOrder.fStopGainPrice = tPosOrder.fStopGainPrice + tPosOrder.fOpenedPriceOffset;
				if(tPosOrder.nCondNum_StopLose>0)
					tPosOrder.fStopLosePrice = tPosOrder.fStopLosePrice + tPosOrder.fOpenedPriceOffset;
			}
			m_mapPositionOrder[itPos->first] = tPosOrder;
			PostEvent_PositionStatus(itPos->second.nNum);

			if(tPosOrder.nCondNum_StopGain>0) {
				itCond = m_mapConditionOrder.find(tPosOrder.nCondNum_StopGain);
				if(itCond != m_mapConditionOrder.end()) {
					itCond->second.nState = conCondActive;
					if(tPosOrder.nPriceOffsetType==1) {		// 如果比较价格为成交价
						itCond->second.fConditionPrice = 
								itCond->second.fConditionPrice + tPosOrder.fOpenedPriceOffset;
						if(tPosOrder.nCloseMode == 2)
							itCond->second.req.LimitPrice = 
									itCond->second.req.LimitPrice + tPosOrder.fOpenedPriceOffset;
					}
					m_mapConditionOrder[itCond->first] = itCond->second;
					PostEvent_ConditionStatus(itCond->second.nNum);
				}
			}

			if(tPosOrder.nCondNum_StopLose>0) {
				itCond = m_mapConditionOrder.find(tPosOrder.nCondNum_StopLose);
				if(itCond != m_mapConditionOrder.end()) {
					itCond->second.nState = conCondActive;
					if(tPosOrder.nPriceOffsetType==1) {		// 如果比较价格为成交价
						itCond->second.fConditionPrice = 
								itCond->second.fConditionPrice + tPosOrder.fOpenedPriceOffset;
						if(tPosOrder.nCloseMode == 2)
							itCond->second.req.LimitPrice = 
									itCond->second.req.LimitPrice + tPosOrder.fOpenedPriceOffset;
					}
					m_mapConditionOrder[itCond->first] = itCond->second;
					PostEvent_ConditionStatus(itCond->second.nNum);
				}
			}
			break;
		}
	}

	for(itCond=m_mapConditionOrder_Overdue.begin(); 
			itCond!=m_mapConditionOrder_Overdue.end(); itCond++) {
		tCondOrder = itCond->second;
		if(tCondOrder.nEntrustNum == atoi(pRawData->TradeField.OrderSysID)) {
			tCondOrder.nRegState = conReqDealClose;
			tCondOrder.nState = conCondCompleted;
			m_mapConditionOrder_Overdue[itCond->first]=tCondOrder;
			PostEvent_ConditionStatus(itCond->second.nNum);
			if(tCondOrder.nCondPosRelation == conCPRCondFromPos) {
				itPos = m_mapPositionOrder_Overdue.find(tCondOrder.nPosNum);
				if(itPos != m_mapPositionOrder_Overdue.end()) {
					itPos->second.nState = conPositionCompleted;
					m_mapPositionOrder_Overdue[itPos->first] = itPos->second;
					PostEvent_PositionStatus(itPos->second.nNum);
				}
			}
			else if(tCondOrder.nCondPosRelation == conCPRCondNextPos) {
			}
			break;
		}
	}

	UnlockObject();

	wxDELETE(pRawData)
	evt.SetClientData(NULL);
}

void COrderServiceThread::OnPlatSvrStatusChange(wxCommandEvent& evt)
{

/////开盘前
//#define THOST_FTDC_IS_BeforeTrading '0'
/////非交易
//#define THOST_FTDC_IS_NoTrading '1'
/////连续交易
//#define THOST_FTDC_IS_Continous '2'
/////集合竞价报单
//#define THOST_FTDC_IS_AuctionOrdering '3'
/////集合竞价价格平衡
//#define THOST_FTDC_IS_AuctionBalance '4'
/////集合竞价撮合
//#define THOST_FTDC_IS_AuctionMatch '5'
/////收盘
//#define THOST_FTDC_IS_Closed '6'

	DataInstrumentStatus* pStatus = (DataInstrumentStatus*)evt.GetClientData();
	if(pStatus == NULL)
		return;

	wxString strExchange = pStatus->InstrumentStatusField.ExchangeID;
	wxString strOrderExchange;

	char cOldStatus = 0;
	std::map<string, char>::iterator itStatus;

	LockObject();

	itStatus = m_mapExchangeStatus.find(strExchange.c_str());
	if(itStatus != m_mapExchangeStatus.end()) {
		if(itStatus->second == pStatus->InstrumentStatusField.InstrumentStatus) {
			UnlockObject();
			SAFE_DELETE(pStatus);
			return;
		}
	}
	string strExch = strExchange.c_str();
	m_mapExchangeStatus[strExch] = pStatus->InstrumentStatusField.InstrumentStatus;

	UnlockObject();


	if(!(pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_Continous ||
			pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_AuctionOrdering)) {
		SAFE_DELETE(pStatus);
		return;
	}

	MAPPARKEDORDERITEM it;
	PARKEDORDER tParkedOrder={0};

	LockObject();

	for(it=m_mapParkedOrder.begin(); it!=m_mapParkedOrder.end(); it++) {
		tParkedOrder = it->second;
		if(tParkedOrder.bIsAuto && tParkedOrder.nState == conParkedActive) {
			strOrderExchange = tParkedOrder.ExchangeID;
			if(strOrderExchange.compare(strExchange)==0) {
				OrderParked(tParkedOrder);
				m_mapParkedOrder[it->first] = tParkedOrder;
			}
		}
	}

	UnlockObject();
    SAFE_DELETE(pStatus);
	//SaveDataToFile();

}

BOOL COrderServiceThread::LoadDataFromFile()
{
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";

	FILEHEAD tFileHead={0};

	wxString strFileName;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".Dat");
	wxFileName filename(strFileName); 
	if(!filename.FileExists())
		return FALSE;

	wxDateTime dtCurr;
	dtCurr = wxDateTime::Today();
	wxDateTime dtFileModify = filename.GetModificationTime();
	
	DWORD dwCurrDate = dtCurr.GetYear()*10000+dtCurr.GetMonth()*100+dtCurr.GetDay();
	DWORD dwFileModifyDate = dtFileModify.GetYear()*10000+dtFileModify.GetMonth()*100+dtFileModify.GetDay();

	wxFile file;
	DWORD dwPos=0;

	// 如果是隔夜的条件单等数据，则清空文件
	if(dwCurrDate-dwFileModifyDate) {
//		file.Create(strFileName, true);
//		file.Close();
		return TRUE;
	}

	file.Open(strFileName);
	if(!file.IsOpened())
		return FALSE;

	file.Read(&tFileHead, sizeof(FILEHEAD));
	dwPos+=sizeof(FILEHEAD);

	tFileHead.dwDate;
	tFileHead.dwTime;
	if(DEFAULT_SVR()) DEFAULT_SVR()->SetReqID((int)tFileHead.dwOrderReqNum);
	sm_nBatchOrderNum = tFileHead.dwBatchGroupNum;
	sm_nBatchSubOrderNum = tFileHead.dwBatchOrderNum;
	sm_nParkedOrderNum = tFileHead.dwParkedOrderNum;
	sm_nConditionOrderNum = tFileHead.dwCondOrderNum;
	sm_nPositionOrderNum = tFileHead.dwPositionOrderNum;


	PARKEDORDER tParkedOrder={0};

	dwPos = tFileHead.dwParkedOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwParkedCount; i++) {
		file.Read(&tParkedOrder, sizeof(PARKEDORDER));
		m_mapParkedOrder[tParkedOrder.nNum] = tParkedOrder;
	}

	CONDITIONORDER tCondOrder={0};

	dwPos = tFileHead.dwCondActiveOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwCondActiveCount; i++) {
		file.Read(&tCondOrder, sizeof(CONDITIONORDER));
		m_mapConditionOrder[tCondOrder.nNum] = tCondOrder;
	}

	dwPos = tFileHead.dwCondOverdueOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwCondOverdueCount; i++) {
		file.Read(&tCondOrder, sizeof(CONDITIONORDER));
		m_mapConditionOrder_Overdue[tCondOrder.nNum] = tCondOrder;
	}

	POSITIONORDER tPosOrder={0};

	dwPos = tFileHead.dwPositionOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwPositionCount; i++) {
		file.Read(&tPosOrder, sizeof(POSITIONORDER));
		m_mapPositionOrder[tPosOrder.nNum] = tPosOrder;
	}

	dwPos = tFileHead.dwPosOverdueOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwPosOverdueCount; i++) {
		file.Read(&tPosOrder, sizeof(POSITIONORDER));
		m_mapPositionOrder_Overdue[tPosOrder.nNum] = tPosOrder;
	}

	BATCHSUBORDER tBatchSub={0};
	
	dwPos = tFileHead.dwBatchSubOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwBatchSubCount; i++) {
		file.Read(&tBatchSub, sizeof(BATCHSUBORDER));
		m_mapBatchSubOrder[tBatchSub.nNum] = tBatchSub;
	}

	file.Close();

	return TRUE;
}

BOOL COrderServiceThread::SaveDataToFile()
{
    // 创建目录
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";
    if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	FILEHEAD tFileHead={0};

	wxFile file;
	DWORD dwPos=0;

    wxString strFileName;

	if(!(m_mapParkedOrder.size()>0 || m_mapConditionOrder.size()>0 || m_mapConditionOrder_Overdue.size()>0 
			|| m_mapPositionOrder.size()>0 || m_mapPositionOrder_Overdue.size() || m_mapBatchSubOrder.size()>0))
		return FALSE;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".Dat");
	file.Create(strFileName, true);
	if(!file.IsOpened()) {
		wxMessageDialog dialog( NULL,LOADSTRING(SaveFail_DataFile_MESSAGE) ,LOADSTRING(SaveFail_Title),wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return FALSE;
	}

	tFileHead.dwDate;
	tFileHead.dwTime;
    tFileHead.dwOrderReqNum = DEFAULT_SVR()?DEFAULT_SVR()->GetReqID():0;//sm_nReqNum;
	tFileHead.dwBatchGroupNum = sm_nBatchOrderNum;
	tFileHead.dwBatchOrderNum = sm_nBatchSubOrderNum;
	tFileHead.dwParkedOrderNum = sm_nParkedOrderNum;
	tFileHead.dwCondOrderNum = sm_nConditionOrderNum;
	tFileHead.dwPositionOrderNum = sm_nPositionOrderNum;

	file.Write(&tFileHead, sizeof(FILEHEAD));
	dwPos+=sizeof(FILEHEAD);

	LockObject();

	tFileHead.dwParkedOffset = dwPos;
	for(MAPPARKEDORDERITEM itParked=m_mapParkedOrder.begin(); 
			itParked!=m_mapParkedOrder.end(); ++itParked) {
		file.Write(&(itParked->second), sizeof(PARKEDORDER));
		tFileHead.dwParkedCount++;
		tFileHead.dwParkedSize+=sizeof(PARKEDORDER);
		dwPos+=sizeof(PARKEDORDER);
	}

	tFileHead.dwCondActiveOffset = dwPos;
	for(MAPCONDITIONORDERITEM itCond=m_mapConditionOrder.begin(); 
			itCond!=m_mapConditionOrder.end(); ++itCond) {
		file.Write(&(itCond->second), sizeof(CONDITIONORDER));
		tFileHead.dwCondActiveCount++;
		tFileHead.dwCondActiveSize+=sizeof(CONDITIONORDER);
		dwPos+=sizeof(CONDITIONORDER);
	}

	tFileHead.dwCondOverdueOffset = dwPos;
	for(MAPCONDITIONORDERITEM itCondOver=m_mapConditionOrder_Overdue.begin(); 
			itCondOver!=m_mapConditionOrder_Overdue.end(); ++itCondOver) {
		file.Write(&(itCondOver->second), sizeof(CONDITIONORDER));
		tFileHead.dwCondOverdueCount++;
		tFileHead.dwCondOverdueSize+=sizeof(CONDITIONORDER);
		dwPos+=sizeof(CONDITIONORDER);
	}

	tFileHead.dwPositionOffset = dwPos;
	for(MAPPOSITIONORDERITEM itPos=m_mapPositionOrder.begin(); 
			itPos!=m_mapPositionOrder.end(); ++itPos) {
		file.Write(&(itPos->second), sizeof(POSITIONORDER));
		tFileHead.dwPositionCount++;
		tFileHead.dwPositionSize+=sizeof(POSITIONORDER);
		dwPos+=sizeof(POSITIONORDER);
	}

	tFileHead.dwPosOverdueOffset = dwPos;
	for(MAPPOSITIONORDERITEM itPos=m_mapPositionOrder_Overdue.begin(); 
			itPos!=m_mapPositionOrder_Overdue.end(); ++itPos) {
		file.Write(&(itPos->second), sizeof(POSITIONORDER));
		tFileHead.dwPosOverdueCount++;
		tFileHead.dwPosOverdueSize+=sizeof(POSITIONORDER);
		dwPos+=sizeof(POSITIONORDER);
	}
	
	tFileHead.dwBatchSubOffset = dwPos;
	for(MAPBATCHSUBORDERITEM itSub=m_mapBatchSubOrder.begin(); 
			itSub!=m_mapBatchSubOrder.end(); ++itSub) {
		file.Write(&(itSub->second), sizeof(BATCHSUBORDER));
		tFileHead.dwBatchSubCount++;
		tFileHead.dwBatchSubSize+=sizeof(BATCHSUBORDER);
		dwPos+=sizeof(BATCHSUBORDER);
	}

	UnlockObject();

	file.Seek(0);
	file.Write(&tFileHead, sizeof(FILEHEAD));

	file.Close() ;

	return TRUE;
}

void COrderServiceThread::PostEvent_ParkedStatus(int nNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALPARKED_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

void COrderServiceThread::PostEvent_ConditionStatus(int nNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALCONDITION_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

void COrderServiceThread::PostEvent_PositionStatus(int nNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALPOSITION_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

BOOL COrderServiceThread::HasNoTouchLocalOrder()
{
	for(MAPPARKEDORDERITEM itParked=m_mapParkedOrder.begin(); 
			itParked!=m_mapParkedOrder.end(); ++itParked) 
	{
		if(itParked->second.nState<=conParkedActive)
		{
			return TRUE;
		}
	}

	for(MAPCONDITIONORDERITEM itCond=m_mapConditionOrder.begin(); 
			itCond!=m_mapConditionOrder.end(); ++itCond) {
		if(itCond->second.nState<=conCondActive)
		{
			return TRUE;
		}
	}

	return FALSE;
}
