#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "AllOrdersPanel.h"
#include "Module-Misc/GLobalFunc.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../Module-Misc/LogDefine.h"
#include "../inc/gui/MainFrame.h"
#include "../inc/Module-Misc/ZqMessageBox.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
BEGIN_EVENT_TABLE(CAllOrdersPanel, wxPanel)
EVT_SIZE( CAllOrdersPanel::OnSize )
EVT_RADIOBUTTON(wxID_ANY,CAllOrdersPanel::OnRadioButton)
EVT_COMMAND(wxID_ANY, wxEVT_ALL_ORDER_RspOrderAction1,  CAllOrdersPanel::OnRspOrderAction1)
EVT_COMMAND(wxID_ANY, wxEVT_ALL_ORDER_RspOrderAction2,  CAllOrdersPanel::OnRspOrderAction2)
EVT_COMMAND(wxID_ANY, wxEVT_RtnTrade,  CAllOrdersPanel::OnRcvTrade)
EVT_COMMAND(wxID_ANY, wxEVT_RspQryOrderLast,  CAllOrdersPanel::OnRspQryOrder)
EVT_COMMAND(wxID_ANY, wxEVT_RspQryCommissionRate,  CAllOrdersPanel::OnGetCommissionRate)
EVT_COMMAND(wxID_ANY, wxEVT_RspQryMarginRate,  CAllOrdersPanel::OnGetMarginRate)
EVT_COMMAND(wxID_ANY, wxEVT_RtnOrder,  CAllOrdersPanel::OnRtnOrder)

EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
EVT_BUTTON(ID_BUTTON_REMOVE,CAllOrdersPanel::OnRemove)
EVT_BUTTON(ID_BUTTON_REMOVEALL,CAllOrdersPanel::OnRemoveAll)
EVT_BUTTON(ID_BUTTON_REQRY,CAllOrdersPanel::OnReQry)
EVT_COMMAND(wxID_ANY,wxEVT_REQ_REQRY,CAllOrdersPanel::OnReConnectQry)
EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, CAllOrdersPanel::OnActivated)
EVT_CFG_CHANGE(OnCfgChanged)
EVT_LIST_EXT_CONTEXT_MENU(CAllOrdersPanel::OnContext)
EVT_COMMAND(wxID_ANY, wxEVT_REMOVE,  CAllOrdersPanel::OnRemove)
EVT_COMMAND(wxID_ANY, wxEVT_REMOVEALL,  CAllOrdersPanel::OnRemoveAll)
EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_NEW_ORDER,  CAllOrdersPanel::OnOrderInsert)
EVT_PANEL_FOCUS(OnPanelFocus)
EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
EVT_SUBSCRIBE(OnSubscrible)
#ifdef _USE_MULTI_LANGUAGE
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
#endif
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()

CAllOrdersPanel *s_pAllOrdersPanel = NULL;
CAllOrdersPanel *s_pOpenOrdersPanel = NULL;



//----------下面是订阅事件通知----------

void CAllOrdersPanel::OnSubscrible(wxCommandEvent& evt)
{
	set<IPlatformSingleSvr*>::iterator it;
	if(evt.GetInt())
	{
		switch(m_nViewStyle) 
		{ 
			case conAllOrderStyle:
			default:
				for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
				{
					(*it)->SubscribeBusinessData(BID_RspOrderInsert, m_nGID, OrderInsertBackFunc);	//订阅报单录入回执
					(*it)->SubscribeBusinessData(BID_RspOrderAction1, m_nGID, RspOrderAction1CallBackFunc);
					(*it)->SubscribeBusinessData(BID_RspOrderAction2, m_nGID, RspOrderAction2CallBackFunc);
					(*it)->SubscribeBusinessData(BID_RtnTrade, m_nGID, TradeCallBackFunc);	//订阅成交回报
				}
			case conOpenOrderStyle:
				for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
				{
					(*it)->SubscribeBusinessData(BID_RspQryOrder, m_nGID, RspQryOrderCallBackFunc);
					(*it)->SubscribeBusinessData(BID_RspQryInstrumentCommissionRate, m_nGID, GetCommissionRateCallBackFunc);
					(*it)->SubscribeBusinessData(BID_RspQryInstrumentMarginRate, m_nGID, GetMarginRateCallBackFunc);
					(*it)->SubscribeBusinessData(BID_RtnOrder, m_nGID, RtnOrderCallBackFunc);	//订阅报单回报
				}
				break;
		};
	}
	else
	{
		switch(m_nViewStyle) 
		{ 
			case conAllOrderStyle:
			default:				
				for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
				{
					(*it)->UnSubscribeBusinessData(BID_RspOrderInsert, m_nGID);	//订阅报单录入回执
					(*it)->UnSubscribeBusinessData(BID_RspOrderAction1, m_nGID);
					(*it)->UnSubscribeBusinessData(BID_RspOrderAction2, m_nGID);
					(*it)->UnSubscribeBusinessData(BID_RtnTrade, m_nGID);	//订阅成交回报
				}
			case conOpenOrderStyle:
				for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
				{
					(*it)->UnSubscribeBusinessData(BID_RspQryOrder, m_nGID);
					(*it)->UnSubscribeBusinessData(BID_RspQryInstrumentCommissionRate, m_nGID);
					(*it)->UnSubscribeBusinessData(BID_RspQryInstrumentMarginRate, m_nGID);
					(*it)->UnSubscribeBusinessData(BID_RtnOrder, m_nGID);	//报单回报(报单录入与报单操作都会产生该回报)	
				}
				break;
		};
	}
	
}

//----------下面是事件回调函数----------

//报单查询回报
int CAllOrdersPanel::RspQryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryOrder || data.Size!=sizeof(DataRspQryOrder))
		return 0;	

	DataRspQryOrder& refData = *(DataRspQryOrder*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.bIsLast)
	{
        wxCommandEvent evt(wxEVT_RspQryOrderLast, wxID_ANY);
        //s_pAllOrdersPanel->AddPendingEvent(evt);
	    switch(GID) 
	    { 
		    case GID_ALL_ORDER:
                if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
                break;
		    case GID_OPEN_ORDER: 
                if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
			    break;
	    };
	}
	return 0;
}

//手续费率回报
int CAllOrdersPanel::GetCommissionRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{ 
	if(data.BID!=BID_RspQryInstrumentCommissionRate || data.Size!=sizeof(DataRspQryInstrumentCommissionRate))
		return 0;	

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInstrumentCommissionRate),0);

    wxCommandEvent evt(wxEVT_RspQryCommissionRate, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
            if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
	return 0;
}



//保证金率查询回报
int CAllOrdersPanel::GetMarginRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{ 
	if(data.BID!=BID_RspQryInstrumentMarginRate || data.Size!=sizeof(DataRspQryInstrumentMarginRate))
		return 0;	

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInstrumentMarginRate),0);

    wxCommandEvent evt(wxEVT_RspQryMarginRate, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
            if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
	return 0;
}

//报单回报
int CAllOrdersPanel::RtnOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{ 
	if(data.BID!=BID_RtnOrder || data.Size!=sizeof(DataRtnOrder))
		return 0;	

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnOrder),0);

    wxCommandEvent evt(wxEVT_RtnOrder, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
            if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
    return 0;
}

//成交回报
int CAllOrdersPanel::TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
		return 0;	

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnTrade),0);

    wxCommandEvent evt(wxEVT_RtnTrade, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
             if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
    return 0;
}

//报单录入回报
int CAllOrdersPanel::OrderInsertBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{
	if(data.BID!=BID_RspOrderInsert || data.Size!=sizeof(DataRspOrderInsert))
		return 0;	

    if(((DataRspOrderInsert*)&data)->nRequestID<0)
        return 0;

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspOrderInsert),0);

    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
            if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
    return 0;
}

//撤单回执1(错误回执1，综合交易平台认为撤单无效)
int CAllOrdersPanel::RspOrderAction1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{ 
	if(data.BID!=BID_RspOrderAction1 || data.Size!=sizeof(DataRspOrderAction1))
		return 0;	

    wxCommandEvent evt(wxEVT_ALL_ORDER_RspOrderAction1, wxID_ANY);
    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspOrderAction1),0);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
            if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
    return 0;
}


//撤单回执2(错误回执2，交易所认为撤单无效)
int CAllOrdersPanel::RspOrderAction2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  
{ 
	if(data.BID!=BID_RspOrderAction2 || data.Size!=sizeof(DataRspOrderAction2))
		return 0;	

    wxCommandEvent evt(wxEVT_ALL_ORDER_RspOrderAction2, wxID_ANY);
    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspOrderAction2),0);
    evt.SetInt((int)EvtParamID);
    //s_pAllOrdersPanel->AddPendingEvent(evt);
    switch(GID) 
    { 
	    case GID_ALL_ORDER:
            if(s_pAllOrdersPanel) s_pAllOrdersPanel->AddPendingEvent(evt);
            break;
	    case GID_OPEN_ORDER: 
            if(s_pOpenOrdersPanel) s_pOpenOrdersPanel->AddPendingEvent(evt);
		    break;
    };
    return 0;
}


//----------下面是内部事件响应处理函数，在主线程中执行----------

//报单查询响应
void CAllOrdersPanel::OnRspQryOrder(wxCommandEvent& evt)
{
    ShowAll();
}

//获得某些合约的保证金率，需要更新相应的报单的冻结数值
void CAllOrdersPanel::OnGetCommissionRate(wxCommandEvent& evt)
{
    DataRspQryInstrumentCommissionRate CommissionRate;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&CommissionRate,NULL,sizeof(CommissionRate),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
    if(CommissionRate.RspInfoField.ErrorID!=0)
        return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(CommissionRate.Head.pPlatformSvr);
	
    if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end())
        return;

    std::set<std::string> setInstruments;
    std::set<std::string>::iterator it;
    std::vector<PlatformStru_OrderInfo> vecOrders;
	int iret;

    //有时候是合约ID,有时候是品种代码
    std::string strInstru(CommissionRate.InstrumentCommissionRateField.InstrumentID);
    if(strInstru.empty())
        return;

	iret=pSvr->GetInstrumentListByProductID(strInstru,setInstruments);

	if(iret>0)
	{//是品种代码
        for(it=setInstruments.begin();it!=setInstruments.end();it++)
        {
            vecOrders.clear();
            if(m_ShowType==ALLORDER) pSvr->GetTriggerOrders2(*it,vecOrders);
            else if(m_ShowType==OPENORDER) pSvr->GetWaitOrders2(*it,vecOrders);
            else if(m_ShowType==TRADEDORDER) pSvr->GetTradedOrders2(*it,vecOrders);
            else if(m_ShowType==CANCELORDER) pSvr->GetCanceledOrders2(*it,vecOrders);

            if(!vecOrders.empty())
                m_pwxExtListCtrl->UpdateInstrumentItems3(*it,vecOrders);
        }
	}
	else
	{//是合约ID
       vecOrders.clear();
        if(m_ShowType==ALLORDER) pSvr->GetTriggerOrders2(strInstru,vecOrders);
        else if(m_ShowType==OPENORDER) pSvr->GetWaitOrders2(strInstru,vecOrders);
        else if(m_ShowType==TRADEDORDER) pSvr->GetTradedOrders2(strInstru,vecOrders);
        else if(m_ShowType==CANCELORDER) pSvr->GetCanceledOrders2(strInstru,vecOrders);

        if(!vecOrders.empty())
            m_pwxExtListCtrl->UpdateInstrumentItems3(*it,vecOrders);
	}
}

//获得某些合约的保证金率，需要更新相应的报单的冻结数值
void CAllOrdersPanel::OnGetMarginRate(wxCommandEvent& evt)
{
    DataRspQryInstrumentMarginRate MarginRate;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarginRate,NULL,sizeof(MarginRate),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
    if(MarginRate.RspInfoField.ErrorID!=0)
        return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(MarginRate.Head.pPlatformSvr);
    if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end())
        return;

    std::vector<PlatformStru_OrderInfo> vecOrders;
    std::string strInstru(MarginRate.InstrumentMarginRateField.InstrumentID);
    if(strInstru.empty())
        return;

    if(m_ShowType==ALLORDER) 
        pSvr->GetTriggerOrders2(strInstru,vecOrders);
    else if(m_ShowType==OPENORDER) 
        pSvr->GetWaitOrders2(strInstru,vecOrders);
    else if(m_ShowType==TRADEDORDER) 
        pSvr->GetTradedOrders2(strInstru,vecOrders);
    else if(m_ShowType==CANCELORDER) 
        pSvr->GetCanceledOrders2(strInstru,vecOrders);

    if(!vecOrders.empty())
        m_pwxExtListCtrl->UpdateInstrumentItems3(strInstru,vecOrders);
}

//报单回报
void CAllOrdersPanel::OnRtnOrder(wxCommandEvent& evt)
{
    DataRtnOrder RtnOrder;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnOrder,NULL,sizeof(RtnOrder),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;

    PlatformStru_OrderInfo& rawData= RtnOrder.OrderField;
    PlatformStru_OrderInfo OrderInfo;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RtnOrder.Head.pPlatformSvr);
    if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end()) 
        return;

    bool bvalue=false;
    OrderKey tmpOrderKey(rawData);

    if(m_nViewStyle==conOpenOrderStyle) 
        bvalue=pSvr->GetWaitOrder(tmpOrderKey,OrderInfo);
    else if(m_nViewStyle==conAllOrderStyle)
    {
        if(m_ShowType==ALLORDER)
            bvalue=pSvr->GetTriggerOrder(tmpOrderKey,OrderInfo);
        else if(m_ShowType==OPENORDER)
            bvalue=pSvr->GetWaitOrder(tmpOrderKey,OrderInfo);
        else if(m_ShowType==TRADEDORDER)
            bvalue=pSvr->GetTradedOrder(tmpOrderKey,OrderInfo);
        else if(m_ShowType==CANCELORDER)
            bvalue=pSvr->GetCanceledOrder(tmpOrderKey,OrderInfo);
    }

    if(bvalue)
        m_pwxExtListCtrl->UpdateOneItem(OrderInfo);
    else
        m_pwxExtListCtrl->DeleteOneItem(tmpOrderKey);

    if(m_nViewStyle==conAllOrderStyle)
        PopupCancel_InsertDlg(pSvr,rawData);
}

void CAllOrdersPanel::OnRcvTrade(wxCommandEvent& evt)
{
    DataRtnTrade RtnTrade;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnTrade,NULL,sizeof(RtnTrade),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RtnTrade.Head.pPlatformSvr);
	if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end()) 
		return;
    if(m_nViewStyle==conAllOrderStyle) 
    {
        if(NeedOrderTradeConfirm())
            PopupTradeDlg( pSvr,RtnTrade.TradeField );
		NoticeSound(conNoticeTrader);
    }
}

void CAllOrdersPanel::OnOrderInsert(wxCommandEvent& evt)
{
    DataRspOrderInsert RspOrderInsert;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspOrderInsert,NULL,sizeof(RspOrderInsert),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;

	if(m_nViewStyle==conAllOrderStyle) {
        ShowInsertDlg("OIP_ERROR", RspOrderInsert, NeedOrderFailConfirm());
		NoticeSound(conNoticeNewOrderFail);
	}

}

void CAllOrdersPanel::OnRspOrderAction1(wxCommandEvent& evt)
{
    DataRspOrderAction1 RspOrderAction1;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspOrderAction1,NULL,sizeof(RspOrderAction1),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RspOrderAction1.Head.pPlatformSvr);
	if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end()) return;
    if(m_nViewStyle==conAllOrderStyle) 
    {
	    m_pTradeInfoDlg=TRADEINFODLG(this);
	    if(m_pTradeInfoDlg)
	    {
	        wxString info;
	        info.reserve(512);
	        PlatformStru_InstrumentInfo InstrumentInfo;
	        memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
	        pSvr->GetInstrumentInfo(RspOrderAction1.InputOrderActionField.InstrumentID,InstrumentInfo);
	        //info.Printf(LOADFORMATSTRING(CVOLPRC_FORMAT,"%s%s%s"),//文本必须包含%s  %s  %s
		       // RspOrderAction1.InputOrderActionField.InstrumentID,
		       // Number2String(RspOrderAction1.InputOrderActionField.VolumeChange).c_str(),
		       // Price2String(RspOrderAction1.InputOrderActionField.LimitPrice,InstrumentInfo.PriceTick).c_str());
	        //m_pTradeInfoDlg->ShowTradeMsg(info, -1, LOADSTRING(OLS_CANCEL_ORDER_FAIL), -1, NeedCancelFailConfirm());

	        //LOG_DATA* pData = new LOG_DATA;
	        //pData->strTitle = "OLS_CANCEL_ORDER_FAIL";
	        //pData->strFormat = "CVOLPRC_FORMAT";
	        //pData->vParam.push_back(LOG_PARAM(RspOrderAction1.InputOrderActionField.InstrumentID));
	        //pData->vParam.push_back(LOG_PARAM(Number2String(RspOrderAction1.InputOrderActionField.VolumeChange)));
	        //pData->vParam.push_back(LOG_PARAM(Price2String(RspOrderAction1.InputOrderActionField.LimitPrice,InstrumentInfo.PriceTick)));
	        //m_pTradeInfoDlg->WriteLog( pData );

			OrderKey orderKey(RspOrderAction1.InputOrderActionField.InvestorID, 
						RspOrderAction1.InputOrderActionField.InstrumentID, 
						RspOrderAction1.InputOrderActionField.FrontID, 
						RspOrderAction1.InputOrderActionField.SessionID, 
						wxString(RspOrderAction1.InputOrderActionField.OrderRef).Trim(false).c_str());
			PlatformStru_OrderInfo rawData;
			pSvr->GetOrder(orderKey, rawData);
			
			std::string strStatusMsg=RspOrderAction1.RspInfoField.ErrorMsg;
			int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(strStatusMsg);
			if(valStatusMsg>0)  
				strStatusMsg=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
			info.Printf(LOADFORMATSTRING(CANCEL_ORDER_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
				RspOrderAction1.InputOrderActionField.InstrumentID,
				CFieldValueTool::Direction2String(rawData.Direction,SVR_LANGUAGE).c_str(),
				CFieldValueTool::OffsetFlag2String(rawData.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
				Number2String(rawData.VolumeTotalOriginal).c_str(),
				CFieldValueTool::OrderPriceType2String(rawData.OrderPriceType,SVR_LANGUAGE).c_str(),
				Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick).c_str(),
				strStatusMsg.c_str());
	        m_pTradeInfoDlg->ShowTradeMsg(info, -1, LOADSTRING(OLS_CANCEL_ORDER_FAIL), -1, NeedCancelFailConfirm());

			LOG_DATA* pData = new LOG_DATA;
	        pData->strTitle = "OLS_CANCEL_ORDER_FAIL";
	        pData->strFormat = "CVOLPRC_FORMAT";
			pData->vParam.push_back(LOG_PARAM(RspOrderAction1.InputOrderActionField.InstrumentID));
			pData->vParam.push_back(LOG_PARAM(rawData.Direction, DIRECTION_TYPE));
			pData->vParam.push_back(LOG_PARAM(rawData.CombOffsetFlag[0],  OFFSETFLAG_TYPE));
			pData->vParam.push_back(LOG_PARAM(Number2String(rawData.VolumeTotalOriginal)));
			pData->vParam.push_back(LOG_PARAM(rawData.OrderPriceType, PRICETYPE_MSG_TYPE));
			pData->vParam.push_back(LOG_PARAM(Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick)));
			pData->vParam.push_back(LOG_PARAM(RspOrderAction1.RspInfoField.ErrorMsg, ORDER_STATUS_MSG_TYPE));
			m_pTradeInfoDlg->WriteLog( pData );

			NoticeSound(conNoticeCancelFail);
		}
    }
}


void CAllOrdersPanel::OnRspOrderAction2(wxCommandEvent& evt)
{
    DataRspOrderAction2 RspOrderAction2;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspOrderAction2,NULL,sizeof(RspOrderAction2),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RspOrderAction2.Head.pPlatformSvr);
	if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end()) return;
    if(m_nViewStyle==conAllOrderStyle) 
    {
	    m_pTradeInfoDlg=TRADEINFODLG(this);
	    if(m_pTradeInfoDlg)
        {
	        wxString info;
	        PlatformStru_InstrumentInfo InstrumentInfo;
	        memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
	        pSvr->GetInstrumentInfo(RspOrderAction2.OrderActionField.InstrumentID,InstrumentInfo);
	        info.reserve(512);
	        //info.Printf(LOADFORMATSTRING(CVOLPRC_FORMAT,"%s%s%s"),//文本必须包含%s  %s  %s
		       // RspOrderAction2.OrderActionField.InstrumentID,
		       // Number2String(RspOrderAction2.OrderActionField.VolumeChange).c_str(),
		       // Price2String(RspOrderAction2.OrderActionField.LimitPrice,InstrumentInfo.PriceTick).c_str());
	        //m_pTradeInfoDlg->ShowTradeMsg(info, -1, LOADSTRING(OLS_CANCEL_ORDER_FAIL), -1, NeedCancelFailConfirm());

	        //LOG_DATA* pData = new LOG_DATA;
	        //pData->strTitle = "OLS_CANCEL_ORDER_FAIL";
	        //pData->strFormat = "CVOLPRC_FORMAT";
	        //pData->vParam.push_back(LOG_PARAM(RspOrderAction2.OrderActionField.InstrumentID));
	        //pData->vParam.push_back(LOG_PARAM(Number2String(RspOrderAction2.OrderActionField.VolumeChange)));
	        //pData->vParam.push_back(LOG_PARAM(Price2String(RspOrderAction2.OrderActionField.LimitPrice,InstrumentInfo.PriceTick)));
	        //m_pTradeInfoDlg->WriteLog( pData );
			
			OrderKey orderKey(RspOrderAction2.OrderActionField.InvestorID, 
						RspOrderAction2.OrderActionField.InstrumentID, 
						RspOrderAction2.OrderActionField.FrontID, 
						RspOrderAction2.OrderActionField.SessionID, 
						wxString(RspOrderAction2.OrderActionField.OrderRef).Trim(false).c_str());
			PlatformStru_OrderInfo rawData;
			pSvr->GetOrder(orderKey, rawData);

			if(strlen(RspOrderAction2.RspInfoField.ErrorMsg)==0)
				strncpy(RspOrderAction2.RspInfoField.ErrorMsg, 
						RspOrderAction2.OrderActionField.StatusMsg, 
						sizeof(RspOrderAction2.RspInfoField.ErrorMsg));
			
			std::string strStatusMsg=RspOrderAction2.RspInfoField.ErrorMsg;
			int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(strStatusMsg);
			if(valStatusMsg>0)  
				strStatusMsg=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
			info.Printf(LOADFORMATSTRING(CANCEL_ORDER_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
				RspOrderAction2.OrderActionField.InstrumentID,
				CFieldValueTool::Direction2String(rawData.Direction,SVR_LANGUAGE).c_str(),
				CFieldValueTool::OffsetFlag2String(rawData.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
				Number2String(rawData.VolumeTotalOriginal).c_str(),
				CFieldValueTool::OrderPriceType2String(rawData.OrderPriceType,SVR_LANGUAGE).c_str(),
				Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick).c_str(),
				strStatusMsg.c_str());
	        m_pTradeInfoDlg->ShowTradeMsg(info, -1, LOADSTRING(OLS_CANCEL_ORDER_FAIL), -1, NeedCancelFailConfirm());

			LOG_DATA* pData = new LOG_DATA;
	        pData->strTitle = "OLS_CANCEL_ORDER_FAIL";
	        pData->strFormat = "CVOLPRC_FORMAT";
			pData->vParam.push_back(LOG_PARAM(RspOrderAction2.OrderActionField.InstrumentID));
			pData->vParam.push_back(LOG_PARAM(rawData.Direction, DIRECTION_TYPE));
			pData->vParam.push_back(LOG_PARAM(rawData.CombOffsetFlag[0],  OFFSETFLAG_TYPE));
			pData->vParam.push_back(LOG_PARAM(Number2String(rawData.VolumeTotalOriginal)));
			pData->vParam.push_back(LOG_PARAM(rawData.OrderPriceType, PRICETYPE_MSG_TYPE));
			pData->vParam.push_back(LOG_PARAM(Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick)));
			pData->vParam.push_back(LOG_PARAM(RspOrderAction2.RspInfoField.ErrorMsg, ORDER_STATUS_MSG_TYPE));
			m_pTradeInfoDlg->WriteLog( pData );

			NoticeSound(conNoticeCancelFail);
	    }
    }
}

CAllOrdersPanel::CAllOrdersPanel(int nViewStyle, 
								 wxWindow *parent,
                                 wxWindowID winid,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxString& name)
:COrderListPanel(parent,winid, pos,size, style, name),m_pListCfg(NULL),
m_WxRadioButton1(NULL),
m_WxRadioButton2(NULL),
m_WxRadioButton3(NULL),
m_WxRadioButton4(NULL),
m_pButtonRemoveAll(NULL),
m_pButtonRemove(NULL),
m_pButtonReQry(NULL)
{

	SetBackgroundColour(DEFAULT_COLOUR);

    switch(nViewStyle) 
	{ 
		case conAllOrderStyle:
			m_nGID = GID_ALL_ORDER;
			m_ShowType = ALLORDER;
			m_nViewStyle = conAllOrderStyle;
			break;
		case conOpenOrderStyle:
			m_nGID = GID_OPEN_ORDER;
			m_ShowType = OPENORDER;
			m_nViewStyle = conOpenOrderStyle;
			break;
	};

	switch(nViewStyle) 
	{ 
		case conAllOrderStyle: 
		{
			m_WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, LOADSTRING(ALL_ORDER), wxPoint(), wxSize(60, CommonBtnHeight));//全部单
			m_WxRadioButton1->SetValue(true);
			m_WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, LOADSTRING(OPEN_ORDER), wxPoint(), wxSize(60, CommonBtnHeight));//挂单
			m_WxRadioButton3 = new wxRadioButton(this, ID_WXRADIOBUTTON3, LOADSTRING(OIP_FILLED), wxPoint(), wxSize(60, CommonBtnHeight));//已成交
			m_WxRadioButton4 = new wxRadioButton(this, ID_WXRADIOBUTTON4, LOADSTRING(CANCEL_FAIL_ORDER), wxPoint(), wxSize(90, CommonBtnHeight));//已撤单/错单
			break;
		}
		case conOpenOrderStyle:
            break;
	};

    m_pButtonReQry = new wxButton(this, ID_BUTTON_REQRY, LOADSTRING(BUTTON_REQRY_CAPTION), wxPoint(), wxSize(80,CommonBtnHeight));

	m_pButtonRemove = new wxButton(this, ID_BUTTON_REMOVE, LOADSTRING(CONTEXTMENU_REMOVE), wxPoint(), wxSize(60,CommonBtnHeight));             //撤单
	m_pButtonRemoveAll = new wxButton(this, ID_BUTTON_REMOVEALL, LOADSTRING(CONTEXTMENU_REMOVEALL), wxPoint(), wxSize(60,CommonBtnHeight));    //全撤
    
	switch(nViewStyle) 
	{ 
		case conAllOrderStyle:
			m_pwxExtListCtrl= new CDataListCtrl<OrderKey,PlatformStru_OrderInfo>(UpdateListItemCallBack, 
                                                                                 this, 
                                                                                 wxID_ANY,
                                                                                 wxDefaultPosition,
                                                                                 wxDefaultSize,
                                                                                 wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT,
                                                                                 wxDefaultValidator,
                                                                                 wxEmptyString);
			break;
		case conOpenOrderStyle: 
			m_pwxExtListCtrl= new CDataListCtrl<OrderKey,PlatformStru_OrderInfo>(UpdateOpenListItemCallBack, 
                                                                                 this, 
                                                                                 wxID_ANY,
                                                                                 wxDefaultPosition,
                                                                                 wxDefaultSize,
                                                                                 wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT,
                                                                                 wxDefaultValidator,
                                                                                 wxEmptyString);
			break;
	};

	switch(nViewStyle) 
	{ 
		case conAllOrderStyle:
            s_pAllOrdersPanel = this;
            break;
		case conOpenOrderStyle: 
            s_pOpenOrdersPanel = this;
			break;
	};

    Init();

}

CAllOrdersPanel::~CAllOrdersPanel(void)
{
    if(m_WxRadioButton1) { delete m_WxRadioButton1;  m_WxRadioButton1=NULL; }
    if(m_WxRadioButton2) { delete m_WxRadioButton2;  m_WxRadioButton2=NULL; }
    if(m_WxRadioButton3) { delete m_WxRadioButton3;  m_WxRadioButton3=NULL; }
    if(m_WxRadioButton4) { delete m_WxRadioButton4;  m_WxRadioButton4=NULL; }
    if(m_pButtonRemoveAll) { delete m_pButtonRemoveAll;  m_pButtonRemoveAll=NULL; }
    if(m_pButtonRemove) { delete m_pButtonRemove;  m_pButtonRemove=NULL; }
    if(m_pButtonReQry) { delete m_pButtonReQry;  m_pButtonReQry=NULL; }
}


void CAllOrdersPanel::ShowAll()
{
	set<IPlatformSingleSvr*>::iterator it;
	std::vector<PlatformStru_OrderInfo> vecAll,vecOrders;
	for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
	{
		if(m_ShowType==ALLORDER) 
			(*it)->GetTriggerOrders(vecOrders);
		else if(m_ShowType==OPENORDER) 
			(*it)->GetWaitOrders(vecOrders);
		else if(m_ShowType==TRADEDORDER) 
			(*it)->GetTradedOrders(vecOrders);
		else if(m_ShowType==CANCELORDER) 
			(*it)->GetCanceledOrders(vecOrders);
		vecAll.insert(vecAll.end(),vecOrders.begin(),vecOrders.end());
	}
    m_pwxExtListCtrl->UpdateAllItems(vecAll);
}


void CAllOrdersPanel::OnReQry(wxCommandEvent& evt)
{
    PlatformStru_QryOrder tmp;
    memset(&tmp, 0, sizeof(tmp));
	set<IPlatformSingleSvr*>::iterator it;
	for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
	{
		(*it)->ReqQryOrder(tmp);
	}       
}
void CAllOrdersPanel::OnReConnectQry(wxCommandEvent& evt)
{
	set<IPlatformSingleSvr*>::iterator it;
	if(evt.GetInt()==1)//获取需要刷新的总数
	{
		std::vector<PlatformStru_OrderInfo> vecOrders;
		int count=0;
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			(*it)->GetTriggerOrders(vecOrders);
			count+=vecOrders.size();
		}
		evt.SetExtraLong(count);
	}
	else
	{
		OnReQry(evt);
	}
}
// 下单成功 是否 弹框
BOOL CAllOrdersPanel::NeedOrderSuccessConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderSuccessDlg;
}

// 下单成功 是否 发声
BOOL CAllOrdersPanel::NeedOrderSuccessSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderSuccessSound;
}

// 下单成功 是否 弹框
BOOL CAllOrdersPanel::NeedOrderFailConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderFailDlg;
}

// 下单成功 是否 发声
BOOL CAllOrdersPanel::NeedOrderFailSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bOrderFailSound;
}

// 挂单成交 是否 弹框
BOOL CAllOrdersPanel::NeedOrderTradeConfirm()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bTradeDlg;
}

// 挂单成交 是否 发声
BOOL CAllOrdersPanel::NeedOrderTradeSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    LPORDER_CFG p = pMgr->GetOrderCfg();
    return p->bTradeSound;
}

// 撤单成交 是否 弹框
BOOL CAllOrdersPanel::NeedCancelSuccessConfirm()
{
	CANCEL_ORDER_CFG cancelOrderCfg = CFG_MGR_DEFAULT()->GetCancelOrderCfg();
	return cancelOrderCfg.bCancelSuccessDlg;
}

// 撤单成功 是否 发声
BOOL CAllOrdersPanel::NeedCancelSuccessSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    CANCEL_ORDER_CFG p = pMgr->GetCancelOrderCfg();
    return p.bCancelSuccessSound;
}

// 撤单失败 是否 弹框
BOOL CAllOrdersPanel::NeedCancelFailConfirm()
{
    CANCEL_ORDER_CFG cancelOrderCfg = CFG_MGR_DEFAULT()->GetCancelOrderCfg();
    return cancelOrderCfg.bCancelFailDlg;
}

// 撤单成功 是否 发声
BOOL CAllOrdersPanel::NeedCancelFailSound()
{
    // 这里从xml中读是否要确认的配置
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return TRUE;
    CANCEL_ORDER_CFG p = pMgr->GetCancelOrderCfg();
    return p.bCancelFailSound;
}

bool CAllOrdersPanel::NeedHideCancelButton()
{
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
    if(pCfgMgr) 
	{
		CANCEL_ORDER_CFG tCfg = pCfgMgr->GetCancelOrderCfg();
		return tCfg.bHideCancelButton;
	}
	return false;
}

void CAllOrdersPanel::NoticeSound(int nType)
{
	switch(nType)
	{
	case conNoticeNewOrderSuccess:
		if(NeedOrderSuccessSound()) 
		{
			PostBeepMessage(2);
		}
		break;
	case conNoticeNewOrderFail:
		if(NeedOrderFailSound())
		{
			PostBeepMessage(1);
		}
		break;
	case conNoticeTrader:
		if(NeedOrderTradeSound())
		{
			PostBeepMessage(2);
		}
		break;
	case conNoticeCancelSuccess:
		if(NeedCancelSuccessSound()) 
		{
			PostBeepMessage(2);
		}
		break;
	case conNoticeCancelFail:
		if(NeedCancelFailSound())
		{
			PostBeepMessage(1);
		}
		break;
	};
}

//根据报单回执显示 交易信息
void CAllOrdersPanel::ShowInsertDlg(const string& Title, const DataRspOrderInsert& OrderInfo, BOOL bIsShow)
{
    if(TRADEINFODLG(this)==NULL) return;
	wxString info;
    std::string stdstrprice=GlobalFunc::ConvertToString(OrderInfo.InputOrderField.LimitPrice,4);
	info.reserve(512);
	info.Printf(LOADFORMATSTRING(CDOCQPM_FORMAT,"%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s
					OrderInfo.InputOrderField.InstrumentID,
					CFieldValueTool::Direction2String(OrderInfo.InputOrderField.Direction,SVR_LANGUAGE).c_str(),
					CFieldValueTool::OffsetFlag2String(OrderInfo.InputOrderField.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
					Number2String(OrderInfo.InputOrderField.VolumeTotalOriginal).c_str(),
					stdstrprice.c_str(),
					OrderInfo.RspInfoField.ErrorMsg);
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CDOCQPM_FORMAT";
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InputOrderField.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InputOrderField.Direction, DIRECTION_TYPE));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InputOrderField.CombOffsetFlag[0], OFFSETFLAG_TYPE));
	pData->vParam.push_back(LOG_PARAM(Number2String(OrderInfo.InputOrderField.VolumeTotalOriginal)));
	pData->vParam.push_back(LOG_PARAM(stdstrprice));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.RspInfoField.ErrorMsg));
	TRADEINFODLG(this)->WriteLog( pData );
}


void  CAllOrdersPanel::PopupCancel_InsertDlg(IPlatformSingleSvr* pSvr,const PlatformStru_OrderInfo& rawData)
{
    int localid=atoi(rawData.OrderSysID);//atoi(rawData.OrderLocalID);
	if(rawData.OrderStatus == THOST_FTDC_OST_Canceled || rawData.OrderStatus>THOST_FTDC_OST_Touched || 
			rawData.OrderSubmitStatus==THOST_FTDC_OSS_InsertRejected) {
		unsigned int sysid=strlen(rawData.OrderSysID);//atoi(rawData.OrderSysID);//左边有空格
		BOOL bIsFail = FALSE;
            m_pTradeInfoDlg=TRADEINFODLG(this);
			if(m_pTradeInfoDlg){
            string title;
			wxString info;
			BOOL bIsShow = (NeedCancelSuccessConfirm() && sysid>0) || (NeedOrderFailConfirm() && sysid==0);
			if(sysid>0 && rawData.OrderSubmitStatus!=THOST_FTDC_OSS_InsertRejected) 
			{
				title = "CANCEL_ORDER_OK";
				bIsFail = FALSE;
			}
			else
			{
				title = "OLS_SEND_ORDER_FAIL";
				bIsFail = TRUE;
			}
            info.reserve(512);
			PlatformStru_InstrumentInfo InstrumentInfo;
			memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
		    pSvr->GetInstrumentInfo(rawData.InstrumentID,InstrumentInfo);
			std::string strStatusMsg=rawData.StatusMsg;
			int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(strStatusMsg);
			if(valStatusMsg>0)  
				strStatusMsg=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
			info.Printf(LOADFORMATSTRING(CANCEL_ORDER_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
				rawData.InstrumentID,
				CFieldValueTool::Direction2String(rawData.Direction,SVR_LANGUAGE).c_str(),
				CFieldValueTool::OffsetFlag2String(rawData.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
				Number2String(rawData.VolumeTotalOriginal).c_str(),
				CFieldValueTool::OrderPriceType2String(rawData.OrderPriceType,SVR_LANGUAGE).c_str(),
				Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick).c_str(),
				strStatusMsg.c_str());
			m_pTradeInfoDlg->ShowTradeMsg(info, -1, LOADSTRING_TEXT(title), -1, bIsShow);

			LOG_DATA* pData = new LOG_DATA;
			pData->strTitle = "ORDERSTATUS";//title;
			pData->strFormat = "CANCEL_ORDER_FORMAT";
			pData->vParam.push_back(LOG_PARAM(rawData.InstrumentID));
			pData->vParam.push_back(LOG_PARAM(rawData.Direction, DIRECTION_TYPE));
			pData->vParam.push_back(LOG_PARAM(rawData.CombOffsetFlag[0],  OFFSETFLAG_TYPE));
			pData->vParam.push_back(LOG_PARAM(Number2String(rawData.VolumeTotalOriginal)));
			pData->vParam.push_back(LOG_PARAM(rawData.OrderPriceType, PRICETYPE_MSG_TYPE));
			pData->vParam.push_back(LOG_PARAM(Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick)));
			pData->vParam.push_back(LOG_PARAM(rawData.StatusMsg, ORDER_STATUS_MSG_TYPE));
			m_pTradeInfoDlg->WriteLog( pData );

			NoticeSound(bIsFail ? conNoticeNewOrderFail : conNoticeCancelSuccess);
		}

    }
	if(rawData.OrderStatus == THOST_FTDC_OST_AllTraded 
			|| rawData.OrderStatus == THOST_FTDC_OST_PartTradedQueueing	//部分成交还在队列中 '1'
			|| rawData.OrderStatus == THOST_FTDC_OST_NoTradeQueueing) {
			BOOL bIsShow = TRUE;
			string strKey;
			DWORD dwCurrTime = ::GetTickCount();
			map<string, DWORD>::iterator it;
			strKey = rawData.ExchangeID;
			strKey += ",";
			strKey += rawData.OrderSysID;
			it = m_mapCancelWait.find(strKey);
			if(it != m_mapCancelWait.end()) {
				// 判断是否超时，未超时这可以不显示
				if(it->second+2000 > dwCurrTime) {
					bIsShow = FALSE;
				}
				m_mapCancelWait.erase(it);
			}
			if(bIsShow) {
				m_pTradeInfoDlg=TRADEINFODLG(this);
				if(m_pTradeInfoDlg){
				wxString info;
				info.reserve(512);
				PlatformStru_InstrumentInfo InstrumentInfo;
				memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
				pSvr->GetInstrumentInfo(rawData.InstrumentID,InstrumentInfo);
				std::string strStatusMsg=rawData.StatusMsg;
				int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(strStatusMsg);
				if(valStatusMsg>0)  strStatusMsg=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
				info.Printf(LOADFORMATSTRING(CANCEL_ORDER_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
					rawData.InstrumentID,
					CFieldValueTool::Direction2String(rawData.Direction,SVR_LANGUAGE).c_str(),
					CFieldValueTool::OffsetFlag2String(rawData.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
					Number2String(rawData.VolumeTotalOriginal).c_str(),
					CFieldValueTool::OrderPriceType2String(rawData.OrderPriceType,SVR_LANGUAGE).c_str(),
					Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick).c_str(),
					strStatusMsg.c_str());
				m_pTradeInfoDlg->ShowTradeMsg(info, localid, LOADSTRING(SEND_ORDER_OK), localid, NeedOrderSuccessConfirm());

				LOG_DATA* pData = new LOG_DATA;
				pData->strTitle = "ORDERSTATUS";//"SEND_ORDER_OK";
				pData->strFormat = "CANCEL_ORDER_FORMAT";
				pData->vParam.push_back(LOG_PARAM(rawData.InstrumentID));
				pData->vParam.push_back(LOG_PARAM(rawData.Direction, DIRECTION_TYPE));
				pData->vParam.push_back(LOG_PARAM(rawData.CombOffsetFlag[0], OFFSETFLAG_TYPE));
				pData->vParam.push_back(LOG_PARAM(Number2String(rawData.VolumeTotalOriginal)));
				pData->vParam.push_back(LOG_PARAM(rawData.OrderPriceType, PRICETYPE_MSG_TYPE));
				pData->vParam.push_back(LOG_PARAM(Price2String(rawData.LimitPrice,InstrumentInfo.PriceTick)));
				pData->vParam.push_back(LOG_PARAM(rawData.StatusMsg, ORDER_STATUS_MSG_TYPE));
				m_pTradeInfoDlg->WriteLog( pData );
				
				NoticeSound(conNoticeNewOrderSuccess);
			}
		}
    }
}

void  CAllOrdersPanel::PopupTradeDlg(IPlatformSingleSvr* pSvr,const PlatformStru_TradeInfo& rawData)
{
    m_pTradeInfoDlg=TRADEINFODLG(this);
	if(m_pTradeInfoDlg!=NULL){
    wxString info;

	PlatformStru_InstrumentInfo InstrumentInfo;
	memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
    pSvr->GetInstrumentInfo(rawData.InstrumentID,InstrumentInfo);

	info.reserve(512);
	info.Printf(LOADFORMATSTRING(CBSOCQP_FORMAT,"%s%s%s%s%s"),//文本必须包含%s  %s  %s %s %s
		rawData.InstrumentID,
		CFieldValueTool::Direction2String(rawData.Direction,SVR_LANGUAGE).c_str(),
		CFieldValueTool::OffsetFlag2String(rawData.OffsetFlag,SVR_LANGUAGE).c_str(),
		Number2String(rawData.Volume).c_str(),
		Price2String(rawData.Price,InstrumentInfo.PriceTick).c_str());
	info = REPLACEFMTSTRING(info.c_str());
	m_pTradeInfoDlg->ShowTradeMsg(info, -1, LOADSTRING(FILL_MESSAGE), -1, TRUE);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = "FILL_MESSAGE";
	pData->strFormat = "CBSOCQP_FORMAT";
	pData->vParam.push_back(LOG_PARAM(rawData.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(rawData.Direction, DIRECTION_TYPE));
	pData->vParam.push_back(LOG_PARAM(rawData.OffsetFlag, OFFSETFLAG_TYPE));
	pData->vParam.push_back(LOG_PARAM(Number2String(rawData.Volume)));
	pData->vParam.push_back(LOG_PARAM(Price2String(rawData.Price,InstrumentInfo.PriceTick).c_str()));
	m_pTradeInfoDlg->WriteLog( pData );
	}
}




//CommonBtnHeight

void CAllOrdersPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
    int Pox,Poy;

	if(m_pwxExtListCtrl)
		m_pwxExtListCtrl->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);

    Pox=8;
    Poy=rt.height-2-CommonBtnHeight;

    if(m_WxRadioButton1)
    {
        m_WxRadioButton1->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_WxRadioButton1->GetRect().GetWidth()+8;
    }
    if(m_WxRadioButton2)
    {
        m_WxRadioButton2->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_WxRadioButton2->GetRect().GetWidth()+8;
    }
    if(m_WxRadioButton3)
    {
        m_WxRadioButton3->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_WxRadioButton3->GetRect().GetWidth()+8;
    }
    if(m_WxRadioButton4)
    {
        m_WxRadioButton4->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_WxRadioButton4->GetRect().GetWidth()+8;
    }

    Pox+=8;

    if(m_pButtonReQry)
    {
        m_pButtonReQry->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonReQry->GetRect().GetWidth()+8;
    }

    Pox+=8;

    if(m_pButtonRemove)
    {
        m_pButtonRemove->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonRemove->GetRect().GetWidth()+8;
    }
    if(m_pButtonRemoveAll)
    {
        m_pButtonRemoveAll->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonRemoveAll->GetRect().GetWidth()+8;
    }
}

/*撤单按钮事件响应*/
void CAllOrdersPanel::OnRemove(wxCommandEvent& evt)
{
    int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
    if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
        return;
    }

    wxExtListItem  item;
    wxString strMsg,strInstrumentID;
    item.SetId(SelectItemId);
    item.SetMask(wxLISTEXT_MASK_DATA);
    m_pwxExtListCtrl ->GetItem( item );

    PlatformStru_OrderInfo OrderInfo;

    if(!m_pwxExtListCtrl->GetValueByRow(SelectItemId,OrderInfo))
        return;
	IPlatformSingleSvr* pSvr=NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(OrderInfo.szAccount));
	else
		pSvr = DEFAULT_SVR();
    switch(OrderInfo.OrderStatus)
    {
        case THOST_FTDC_OST_PartTradedQueueing:
        case THOST_FTDC_OST_NoTradeQueueing:
        case THOST_FTDC_OST_NotTouched:
	    {
            wxString strmsg,strcaption;
		    PlatformStru_InstrumentInfo InstrumentInfo;
		    memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
            if(pSvr) pSvr->GetInstrumentInfo(OrderInfo.InstrumentID,InstrumentInfo);
		    std::string stdstrprice=Price2String(OrderInfo.LimitPrice,InstrumentInfo.PriceTick);
		    strmsg.Printf(LOADFORMATSTRING(CANCELQTYPRC_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
			    wxString(OrderInfo.OrderSysID).Trim(true).Trim(false).c_str(),
			    CFieldValueTool::OffsetFlag2String(OrderInfo.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
			    CFieldValueTool::Direction2String(OrderInfo.Direction,SVR_LANGUAGE).c_str(),
			    OrderInfo.InstrumentID,
			    Number2String(OrderInfo.VolumeTotalOriginal).c_str(),
			    stdstrprice.c_str(),
			    CFieldValueTool::HedgeFlag2String(OrderInfo.CombHedgeFlag[0],SVR_LANGUAGE).c_str()
			    );
		    strmsg = REPLACEFMTSTRING(strmsg.c_str());
		    strcaption=LOADSTRING(TITLE_CANCELORDER_CONFIRM);


		    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
            CANCEL_ORDER_CFG cancelOrderCfg;
            if( pCfgMgr) 
            {
                cancelOrderCfg = pCfgMgr->GetCancelOrderCfg(); 
            }
			int ret = wxID_YES;
            if( cancelOrderCfg.bCancelConfirm )
            {
				CZqMessageBox msgbox(this);
				msgbox.ResetMessage(strcaption, strmsg, 
						CZqMessageBox::btnYes|CZqMessageBox::btnNo, CZqMessageBox::cancelOrder);
				ret = msgbox.ShowModal();
				USERLOG_INFO("用户单击%s按钮[撤单]并且选择[%s]确认\n\"\n%s\n%s\n\"", 
						(m_nViewStyle==conOpenOrderStyle?"委托模块":"挂单模块"), 
						(ret==wxID_YES?"是":"否"), strcaption, strmsg);
			}
			else
				USERLOG_INFO("用户单击%s按钮[撤单]并且无需确认\n\"\n%s\n%s\n\"", 
						(m_nViewStyle==conOpenOrderStyle?"委托模块":"挂单模块"), 
						strcaption, strmsg);

			if(ret==wxID_YES)
			{
				PlatformStru_InputOrderAction ReqData;
                memset(&ReqData, 0, sizeof(ReqData));
                ReqData.Thost.FrontID=OrderInfo.FrontID;
                ReqData.Thost.SessionID=OrderInfo.SessionID;
                memcpy(ReqData.Thost.OrderRef, OrderInfo.OrderRef, sizeof(OrderInfo.OrderRef));
                memcpy(ReqData.Thost.OrderSysID, OrderInfo.OrderSysID, sizeof(OrderInfo.OrderSysID));
                memcpy(ReqData.Thost.ExchangeID, OrderInfo.ExchangeID, sizeof(OrderInfo.ExchangeID));
 				memcpy(ReqData.Thost.InstrumentID, OrderInfo.InstrumentID, sizeof(OrderInfo.InstrumentID));

			    // 把要撤单的保存到Map中
			    string strKey;
			    strKey = ReqData.Thost.ExchangeID;
			    strKey += ",";
			    strKey += ReqData.Thost.OrderSysID;
			    m_mapCancelWait[strKey] = GetTickCount();

                if(pSvr) pSvr->ReqOrderAction(ReqData);
            }
            break;
        }
        default:
	    {
		    ShowTradeInfoDlg("OIP_ERROR", "CANCELORDER_ERROE_SINGLE", NeedCancelFailConfirm());
    	    break;
	    }
    }
}


/*全撤按钮事件响应*/
void CAllOrdersPanel::OnRemoveAll(wxCommandEvent& evt)
{
    int SelectItemId = 0, iItemCount = 0;
    wxExtListItem  item;
    wxString strMsg,strInstrumentID;

    iItemCount = m_pwxExtListCtrl->GetItemCount();
    if (iItemCount <= 0) 
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
        return;
    }

    wxString strmsg,strmsgall,strcaption;
	strcaption=LOADSTRING(TITLE_CANCELORDER_CONFIRM);
    std::vector<PlatformStru_OrderInfo> selectedOrder;//选中的列表项，撤单或全撤

        for ( SelectItemId = 0; SelectItemId < iItemCount; SelectItemId++ )	
        {
            item.SetId(SelectItemId);
            item.SetMask(wxLISTEXT_MASK_DATA);
            m_pwxExtListCtrl ->GetItem( item );

            PlatformStru_OrderInfo OrderInfo;
            if(!m_pwxExtListCtrl->GetValueByRow(SelectItemId,OrderInfo))
                continue;
			IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(OrderInfo.szAccount);
            switch(OrderInfo.OrderStatus)
            {
                case THOST_FTDC_OST_PartTradedQueueing:
                case THOST_FTDC_OST_NoTradeQueueing:		
                {
					PlatformStru_InstrumentInfo InstrumentInfo;
					memset(&InstrumentInfo,0,sizeof(InstrumentInfo));
		            if(pSvr) pSvr->GetInstrumentInfo(OrderInfo.InstrumentID,InstrumentInfo);
					std::string stdstrprice=Price2String(OrderInfo.LimitPrice,InstrumentInfo.PriceTick);
					strmsg.Printf(LOADFORMATSTRING(CANCELQTYPRC_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
							wxString(OrderInfo.OrderSysID).Trim(true).Trim(false).c_str(),
							CFieldValueTool::OffsetFlag2String(OrderInfo.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
							CFieldValueTool::Direction2String(OrderInfo.Direction,SVR_LANGUAGE).c_str(),
							OrderInfo.InstrumentID,
							Number2String(OrderInfo.VolumeTotalOriginal).c_str(),
							stdstrprice.c_str(),
							CFieldValueTool::HedgeFlag2String(OrderInfo.CombHedgeFlag[0],SVR_LANGUAGE).c_str()
							); 
					strmsg = REPLACEFMTSTRING(strmsg.c_str());
                    strmsgall+=strmsg;
                    selectedOrder.push_back(OrderInfo);
                    break;
                }
                default:
                    break;
            }
        }
        if(selectedOrder.size()==0) 
        {
            wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
            return;
        }
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	CANCEL_ORDER_CFG cancelOrderCfg;
	if( pCfgMgr) 
	{
		cancelOrderCfg = pCfgMgr->GetCancelOrderCfg(); 
	}
	int ret = wxYES;
	if( cancelOrderCfg.bCancelConfirm )
	{ 
		COrderMessageDlg dlg(NULL);
		dlg.SetTitle(strcaption);
		strmsgall.Replace("\n", "\r\n");
		dlg.SetOrderInfo(strmsgall);
		ret=dlg.ShowModal();
		USERLOG_INFO("用户单击%s按钮[全撤]并且选择[%s]确认\n\"\n%s\n%s\n\"", 
				(m_nViewStyle==conOpenOrderStyle?"委托模块":"挂单模块"), 
				(ret==wxYES?"是":"否"), strcaption, strmsgall);
	}
	else
		USERLOG_INFO("用户单击%s按钮[全撤]并且不需要确认\n\"\n%s\n%s\n\"", 
				(m_nViewStyle==conOpenOrderStyle?"委托模块":"挂单模块"), 
				(ret==wxYES?"是":"否"), strcaption, strmsgall);

    if(ret==wxYES)
    {
        PlatformStru_InputOrderAction ReqData;
        memset(&ReqData, 0, sizeof(ReqData));
        vector<PlatformStru_OrderInfo>::iterator it=selectedOrder.begin();
		if(it!=selectedOrder.end()) {
			for(; it!=selectedOrder.end(); ++it)
			{

				ReqData.Thost.FrontID=it->FrontID;
				ReqData.Thost.SessionID=it->SessionID;
				memcpy(ReqData.Thost.OrderRef, it->OrderRef, sizeof(it->OrderRef));
				memcpy(ReqData.Thost.OrderSysID, it->OrderSysID, sizeof(it->OrderSysID));
				memcpy(ReqData.Thost.ExchangeID, it->ExchangeID, sizeof(it->ExchangeID));
				memcpy(ReqData.Thost.InstrumentID, it->InstrumentID, sizeof(it->InstrumentID));

				// 把要撤单的保存到Map中
				string strKey;
				strKey = ReqData.Thost.ExchangeID;
				strKey += ",";
				strKey += ReqData.Thost.OrderSysID;
				m_mapCancelWait[strKey] = GetTickCount();
				IPlatformSingleSvr* pSvr=NULL;
				if(DEFAULT_SVR()->IsMultipleAccounts())
					pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(it->szAccount));
				else
					pSvr = DEFAULT_SVR();
				if(pSvr) pSvr->ReqOrderAction(ReqData);
			}
			selectedOrder.clear();
		}
		else {
			ShowTradeInfoDlg("OIP_ERROR", "CANCELORDER_ERROE_MULTI", NeedCancelFailConfirm());
		}
    }
}



//配置文件发生变化
void CAllOrdersPanel::OnCfgChanged(wxCommandEvent& evt)
{
	WXLOG_INFO("CAllOrdersPanel::OnCfgChanged");

    Init();

	if(m_pButtonRemove)
		m_pButtonRemove->Show(!NeedHideCancelButton());
	if(m_pButtonRemoveAll)
		m_pButtonRemoveAll->Show(!NeedHideCancelButton());
}

void CAllOrdersPanel::Init()
{
	m_pwxExtListCtrl->Clear();
	if(m_pwxExtListCtrl->GetColumnCount()>0) m_pwxExtListCtrl->DeleteAllColumns();
    m_FieldID2ColumnID.clear();

    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
    if(!pCfgMgr) return;
    m_pListCfg = pCfgMgr->GetListCfg(m_nGID);
    if(!m_pListCfg) return;

    //设置LIST界面属性
    m_HeadBackgroundColor = wxColor(m_pListCfg->HeadBackgroundColor);
    m_HeadColor = wxColor(m_pListCfg->HeadColor);
    m_BackgroundColor = wxColor(m_pListCfg->BackgroundColor);
    m_TextColor = wxColor(m_pListCfg->TextColor);
    m_TextColor2=m_pListCfg->TextColor;
    m_Font.SetNativeFontInfoUserDesc(m_pListCfg->szFont); 
    m_EvenLineBgColor=wxColor(m_pListCfg->EvenLineBackgroundColor);
    m_OddLineBgColor= wxColor(m_pListCfg->OddLineBackgroundColor);

    m_pwxExtListCtrl->SetHeaderBackgroundColour(m_HeadBackgroundColor);
    m_pwxExtListCtrl->SetHeaderForegroundColour(m_HeadColor);
    m_pwxExtListCtrl->SetBackgroundColour(m_BackgroundColor);
    m_pwxExtListCtrl->SetForegroundColour(m_TextColor);
    m_pwxExtListCtrl->SetFont(m_Font);
    m_pwxExtListCtrl->SetOddRowColour(m_OddLineBgColor);
    m_pwxExtListCtrl->SetEvenRowColour(m_EvenLineBgColor);

    wxExtListItem col;	
    int nColCount = pCfgMgr->GetListVisibleColCount(m_nGID);
    for(int i = 0; i < nColCount; i++)
    {
        LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(m_nGID, i);
        if(!pColCfg) continue;

        //"合约" "报单编号"栏，左对齐
        if(pColCfg->id == OPENORDER_OrderLocalID || pColCfg->id == OPENORDER_InstrumentID || 
				(pColCfg->id == ALLORDER_InstrumentID) || (pColCfg->id == ALLORDER_OrderLocalID) )
            col.SetAlign(wxLISTEXT_FORMAT_LEFT);
        else//其余的，右对齐
            col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
	
        col.SetText(pCfgMgr->GetColCfgCaption(m_nGID,pColCfg->id));
        col.SetWidth(pColCfg->Width);
        col.SetTextColour(pColCfg->TextColor);
		col.SetColData(pColCfg->id);

        m_pwxExtListCtrl->InsertColumn(i, col); 

        m_FieldID2ColumnID[pColCfg->id] = i;
    }

    m_pwxExtListCtrl->SetCfg(m_TextColor2,m_FieldID2ColumnID);


    ShowAll();

    return ;
}




//双击撤单
void CAllOrdersPanel::OnActivated(wxExtListEvent& event)
{
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
    if( pCfgMgr == NULL) return;
    CANCEL_ORDER_CFG cancelOrderCfg = pCfgMgr->GetCancelOrderCfg();
    if(cancelOrderCfg.bDoubleClickMouseCancel)
    {
        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,ID_BUTTON_REMOVE);
        ProcessEvent(evt);
    }
    event.Skip();
}

void CAllOrdersPanel::OnPanelFocus(wxCommandEvent& evt)
{
	if(m_pwxExtListCtrl&&m_pwxExtListCtrl->GetItemCount())
	{
		m_pwxExtListCtrl->SetFocus();
		long   pos   =   m_pwxExtListCtrl->GetFirstSelected(); 
		if(pos<0)
		{
			m_pwxExtListCtrl->Select(0);
			m_pwxExtListCtrl->Focus(0);
		}
	}
}

void CAllOrdersPanel::OnContext(wxContextMenuEvent& evt)
{
    wxMenu menu;
	BOOL bIsRowCLick = evt.GetId();
    int colcount=m_pwxExtListCtrl->GetColumnCount();
    int rowcount=m_pwxExtListCtrl->GetItemCount();
    if(bIsRowCLick)
    {
		int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	    if(SelectItemId>=0)
		{
			wxExtListItem  item;
			item.SetId(SelectItemId);
			item.SetMask(wxLISTEXT_MASK_DATA);
			m_pwxExtListCtrl ->GetItem( item );

			PlatformStru_OrderInfo OrderInfo;
			if(m_pwxExtListCtrl->GetValueByRow(SelectItemId,OrderInfo))
			{
				switch(OrderInfo.OrderStatus)
				{
					case THOST_FTDC_OST_PartTradedQueueing:
					case THOST_FTDC_OST_NoTradeQueueing:
						{    
							menu.Append(ID_MENU_REMOVE,LOADSTRING(CONTEXTMENU_REMOVE));
							menu.Append(ID_MENU_REMOVEALL,LOADSTRING(CONTEXTMENU_REMOVEALL));
							menu.Append(ID_MENU_CHANGEORDER,LOADSTRING(CONTEXTMENU_CHANGEORDER));
							menu.AppendSeparator();
						}
						break;
					default:
						break;
				}
			}
        }
    }

	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));
	if ( bIsRowCLick )
	{
		menu.AppendSeparator();
		INSERT_ADDTOMYSELF_ITEM(menu);
	}

    if(menu.GetMenuItemCount()>0&&DEFAULT_SVR())
    {
        POINT pt;
        ::GetCursorPos(&pt);
        BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

		//加入自选
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
		if( retcmd - ID_MENU_ADDTOMYSELF >= 0 )
		{
			bool bAdded = false;
			for ( int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++)
			{
				if ( m_pwxExtListCtrl->IsSelected( i ))
				{			
                    PlatformStru_OrderInfo OrderInfo;
                    if(!m_pwxExtListCtrl->GetValueByRow(i,OrderInfo))
                        continue;

					string InstrumentId = OrderInfo.InstrumentID;
					if(pCfgMgr->GetInstrumentGroupMemberCount(retcmd - ID_MENU_ADDTOMYSELF)>=DEFAULT_SVR()->GetGroupMaxContractNum())
					{
						wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
						break;
					}
					else
					{
						pCfgMgr->AddInstrument( retcmd - ID_MENU_ADDTOMYSELF, InstrumentId.c_str()); 
						bAdded = true;					
					}
				}
			}

			if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);}
			return;
		}

        switch(retcmd)
        {
		case ID_MENU_AUTO_ADJUST_COL_WIDTH:
			m_pwxExtListCtrl->AutoAdjustColumnWidth();
			SaveColWidth();
			break;
        case ID_MENU_REMOVE://撤单
            {
                wxCommandEvent evt(wxEVT_REMOVE);	
                this->AddPendingEvent(evt);
            }
            break;
        case ID_MENU_REMOVEALL://全撤
            {
                wxCommandEvent evt(wxEVT_REMOVEALL);	
                this->AddPendingEvent(evt);
            }
            break;
        case ID_MENU_CHANGEORDER://快速改单
			{
				int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
				if(SelectItemId<0)
					return;

				PlatformStru_OrderInfo OrderInfo;

				if(m_pwxExtListCtrl->GetValueByRow(SelectItemId,OrderInfo)) 
				{
					wxCommandEvent request_event;
					if(DEFAULT_SVR()->HaveOrderType(UIOT_REPLACEORDER))
						request_event.SetEventType(wxEVT_ORDERINSERT_ORDERREPLACE);
					else
						request_event.SetEventType(wxEVT_ORDERINSERT_ORDERCHANGE);
					wxString strCmsStream;
					strCmsStream.Printf("%s,%s,%s",OrderInfo.InvestorID, 
							OrderInfo.ExchangeID, wxString(OrderInfo.OrderSysID));
					if(1)
					{
						DWORD EvtParamID;
						std::string EvtParamStr(strCmsStream.c_str());
						if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,2))
						{
							request_event.SetInt((int)EvtParamID);
							GETTOPWINDOW()->AddPendingEvent(request_event);
						}
					}
				}
            }
            break;  
        case ID_MENU_EXPORT://导出列表
            {
                wxString cap;
                switch(m_ShowType)
                {
                    case ALLORDER:
                        cap=LOADSTRING(ALL_ORDER);
                        break;
                    case OPENORDER:
                        cap=LOADSTRING(OPEN_ORDER);
                        break;
                    case TRADEDORDER:
                        cap=LOADSTRING(OIP_FILLED);
                        break;
                    case CANCELORDER:
                        cap=LOADSTRING(CANCEL_FAIL_ORDER);
                        break;
                }
   				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
            }
            break;
        case ID_MENU_COLUMNCONFIG://表格列设置
            switch(m_nViewStyle) 
            { 
            case conAllOrderStyle:
                SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_ALLORDER);
                break;
            case conOpenOrderStyle:
                SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_WAITTRADE);
                break;
            }
            break;
        default:
            break;
        }
    }
    evt.Skip();
}

void CAllOrdersPanel::OnPanelCharHook(wxCommandEvent& evt)
{
    wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();

	if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
		evt.Skip();
        return;
    }
	pEvent->SetId(win->GetId());

	BOOL bFocusInPanel = IsFocusInPanel(this);

	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;
	
    int keycode=pEvent->GetKeyCode();
	map<int,string>::iterator itKey;
	//获得键盘与字符串的映射表
	map<int,string> addr = pMgr->GetShortCutKeyNameMap();
	itKey = addr.find(keycode);
	if(itKey != addr.end()) {
		std::string strKeyName = itKey->second;
		std::map<std::string, int>::iterator itOrder;
		// 这里从xml中读是否要确认的配置
		CANCEL_ORDER_CFG p = pMgr->GetCancelOrderCfg();
		// 添加撤单按钮快捷键
		if(strKeyName.compare(p.szCancelOrderBtnHotKey)==0 && bFocusInPanel 
				&& m_pButtonRemove && m_pButtonRemove->IsShown()) {
			wxCommandEvent newEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_REMOVE);
			this->AddPendingEvent(newEvent);
			return;
		}
		if(m_nViewStyle==conOpenOrderStyle)
		{
			if(strKeyName.compare(p.szCancelAllOrderBtnHotKey)==0) {
				wxCommandEvent newEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_REMOVEALL);
				this->AddPendingEvent(newEvent);
				return;
			}
		}
	}

	evt.Skip();
}
#ifdef _USE_MULTI_LANGUAGE
void CAllOrdersPanel::OnLanguageChanged(wxCommandEvent& evt)
{

	if(m_nViewStyle==conAllOrderStyle && m_pwxExtListCtrl) 
	{
		TRANSFORM_COLUMN_TEXT(GID_ALL_ORDER,m_pwxExtListCtrl);
	}
	else if(m_nViewStyle==conOpenOrderStyle && m_pwxExtListCtrl) 
	{
		TRANSFORM_COLUMN_TEXT(GID_OPEN_ORDER,m_pwxExtListCtrl);
	}
	else
		return;
    if(m_WxRadioButton1) m_WxRadioButton1->SetLabel(LOADSTRING(ALL_ORDER));
	if(m_WxRadioButton2) m_WxRadioButton2->SetLabel(LOADSTRING(OPEN_ORDER));
	if(m_WxRadioButton3) m_WxRadioButton3->SetLabel(LOADSTRING(OIP_FILLED));
	if(m_WxRadioButton4) m_WxRadioButton4->SetLabel(LOADSTRING(CANCEL_FAIL_ORDER));
	m_pButtonRemove->SetLabel(LOADSTRING(CONTEXTMENU_REMOVE));
	m_pButtonRemoveAll->SetLabel(LOADSTRING(CONTEXTMENU_REMOVEALL));
	m_pButtonReQry->SetLabel(LOADSTRING(BUTTON_REQRY_CAPTION));
	int TotalRow=m_pwxExtListCtrl->GetItemCount();
	if(TotalRow==0) return;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);
	switch(m_nViewStyle) 
	{ 
		case conOpenOrderStyle:
			for(int row=0;row<TotalRow;row++ )
			{
				std::string strValue;
				for(int i = 0; i < pCfgMgr->GetListVisibleColCount(GID_OPEN_ORDER); i++)
				{
					bool bReplace=true;
					LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_OPEN_ORDER, i);
					strValue=m_pwxExtListCtrl->GetSubItemText( row, i);
					if ( pColCfg->id == OPENORDER_Direction)
					{
						strValue=CFieldValueTool::Direction2String(CFieldValueTool::String2Direction(strValue),evt.GetInt());
					}
					else if(pColCfg->id == OPENORDER_StatusMsg)
					{
						int val=CFieldValueTool::String2OrderStatusMsg(strValue);
						if(val>0)
							strValue=CFieldValueTool::OrderStatusMsg2String(val,evt.GetInt());
						else
							bReplace=false;
					}
					else if(pColCfg->id == OPENORDER_OrderPriceType)
					{
						strValue = CFieldValueTool::OrderPriceType2String(CFieldValueTool::String2OrderPriceType(strValue),evt.GetInt());    //报单价格条件
					}
					else if(pColCfg->id == OPENORDER_CombOffsetFlag)
					{
						strValue = CFieldValueTool::OffsetFlag2String(CFieldValueTool::String2OffsetFlag(strValue),evt.GetInt()); 
					}
					else if(pColCfg->id == OPENORDER_OrderStatus)
					{
						strValue =CFieldValueTool::OrderStatus2String(CFieldValueTool::String2OrderStatus(strValue),evt.GetInt());
					}
					else if(pColCfg->id == OPENORDER_CombHedgeFlag)
					{
						strValue =  CFieldValueTool::HedgeFlag2String(CFieldValueTool::String2HedgeFlag(strValue),evt.GetInt());  
					}
					else if(pColCfg->id == OPENORDER_TimeCondition)
					{
						strValue = CFieldValueTool::TimeCondition2String(CFieldValueTool::String2TimeCondition(strValue),evt.GetInt());    //有效期类型
					}
					else if(pColCfg->id == OPENORDER_OrderType)
					{
						strValue = CFieldValueTool::OrderType2String(CFieldValueTool::String2OrderType(strValue),evt.GetInt());    //报单类型
					}
					else if(pColCfg->id == OPENORDER_UserForceClose)
					{
						strValue = CFieldValueTool::UserForceClose2String(CFieldValueTool::String2UserForceClose(strValue),evt.GetInt());
					}
					else if(pColCfg->id == OPENORDER_OrderSubmitStatus)
					{
						strValue = CFieldValueTool::OrderSubmitStatus2String(CFieldValueTool::String2OrderSubmitStatus(strValue),evt.GetInt()); 
					}
					else
					{
						bReplace=false;
					}
					if(bReplace) m_pwxExtListCtrl->SetItem( row, i, strValue );
				}
			}
			break;
		case conAllOrderStyle:
		default:
			for(int row=0;row<TotalRow;row++ )
			{
				string strValue;
				for(int i = 0; i < pCfgMgr->GetListVisibleColCount(m_nGID); i++)
				{
					bool bReplace=true;
					LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(m_nGID, i);
					strValue=m_pwxExtListCtrl->GetSubItemText( row, i);
					//买卖
					if ( pColCfg->id == ALLORDER_Direction)
					{
						strValue=CFieldValueTool::Direction2String(CFieldValueTool::String2Direction(strValue),evt.GetInt());
					}
					else if(pColCfg->id == ALLORDER_StatusMsg)
					{
						int val=CFieldValueTool::String2OrderStatusMsg(strValue);
						if(val>0)
							strValue=CFieldValueTool::OrderStatusMsg2String(val,evt.GetInt());
						else
							bReplace=false;
					}
					else if(pColCfg->id == ALLORDER_OrderPriceType)
					{
						strValue = CFieldValueTool::OrderPriceType2String(CFieldValueTool::String2OrderPriceType(strValue),evt.GetInt());
					}
					else if(pColCfg->id == ALLORDER_CombOffsetFlag)
					{
						strValue = CFieldValueTool::OffsetFlag2String(CFieldValueTool::String2OffsetFlag(strValue),evt.GetInt()); 
					}
					else if(pColCfg->id == ALLORDER_OrderStatus)
					{
						strValue =CFieldValueTool::OrderStatus2String(CFieldValueTool::String2OrderStatus(strValue),evt.GetInt());
					}
					else if(pColCfg->id == ALLORDER_CombHedgeFlag)
					{
						strValue =  CFieldValueTool::HedgeFlag2String(CFieldValueTool::String2HedgeFlag(strValue),evt.GetInt());  
					}
					else if(pColCfg->id == ALLORDER_TimeCondition)
					{
						strValue = CFieldValueTool::TimeCondition2String(CFieldValueTool::String2TimeCondition(strValue),evt.GetInt());   
					}
					else if(pColCfg->id == ALLORDER_OrderType)
					{
						strValue = CFieldValueTool::OrderType2String(CFieldValueTool::String2OrderType(strValue),evt.GetInt());
					}
					else if(pColCfg->id == ALLORDER_UserForceClose)
					{
						strValue = CFieldValueTool::UserForceClose2String(CFieldValueTool::String2UserForceClose(strValue),evt.GetInt());
					}
					else if(pColCfg->id == ALLORDER_ForceCloseReason)
					{
						strValue = CFieldValueTool::ForceCloseReason2String(CFieldValueTool::String2ForceCloseReason(strValue),evt.GetInt());
					}
					else if(pColCfg->id == ALLORDER_OrderSubmitStatus)
					{	
						strValue = CFieldValueTool::OrderSubmitStatus2String(CFieldValueTool::String2OrderSubmitStatus(strValue),evt.GetInt()); 
					}
					else
					{
						bReplace=false;
					}
					if(bReplace) m_pwxExtListCtrl->SetItem( row, i, strValue );

				}
			}	
			break;
	};
}
#endif
void CAllOrdersPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( m_nGID ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(m_nGID, nCol);
	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
}


void CAllOrdersPanel::OnRadioButton(wxCommandEvent& event)
{
	int id=event.GetId();
    char newShowType=m_ShowType;

	switch(id)
	{
	    case ID_WXRADIOBUTTON1:
            newShowType=ALLORDER;
		    break;
	    case ID_WXRADIOBUTTON2:
            newShowType=OPENORDER;
		    break;
	    case ID_WXRADIOBUTTON3:
            newShowType=TRADEDORDER;
		    break;
	    case ID_WXRADIOBUTTON4:
            newShowType=CANCELORDER;
		    break;
	    default:
		    break;
	}
    if(newShowType!=m_ShowType)
    {
        m_ShowType=newShowType;
        if(m_ShowType==ALLORDER||m_ShowType==OPENORDER)
        {
            if(m_pButtonRemove) m_pButtonRemove->Show(true && !NeedHideCancelButton());
            if(m_pButtonRemoveAll) m_pButtonRemoveAll->Show(true && !NeedHideCancelButton());
        }
        else
        {
            if(m_pButtonRemove) m_pButtonRemove->Show(false);
            if(m_pButtonRemoveAll) m_pButtonRemoveAll->Show(false);
        }
        ShowAll();
    }
}



#define UpdateCol_String(FieldName)  \
            if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
                return false;\
            outItem.SetText(wxString(pNew->FieldName));
#define UpdateCol_String3(FieldName,DispName)  \
            if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
                return false;\
            outItem.SetText(wxString(DispName));
#define UpdateCol_Number(FieldName,DispName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName)\
                return false;\
            outItem.SetText(wxString(DispName));\
            outItem.SetiColData((int)(pNew->FieldName));
#define UpdateCol_Price(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetIntInvalidValue()) outItem.SetText("-");\
            else outItem.SetText(Price2String(pNew->FieldName,pNew->PriceTick));\
            outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Int(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetIntInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%d",pNew->FieldName);\
            outItem.SetText(tmpstr);\
            outItem.SetiColData(pNew->FieldName);
#define UpdateCol_Money(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%.02lf", pNew->FieldName ); \
            outItem.SetText(tmpstr);\
            outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Fund(FieldName)  \
			if(!bnewline&&pOld->FieldName==pNew->FieldName) \
				return false;\
			if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
			else tmpstr = GlobalFunc::GetAccountFormatString(pNew->FieldName, 2); \
			outItem.SetText(tmpstr);\
            outItem.SetdColData(pNew->FieldName);


bool CAllOrdersPanel::UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_OrderInfo* pNew=(const PlatformStru_OrderInfo*)pNewData;
    PlatformStru_OrderInfo* pOld=(PlatformStru_OrderInfo*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    outItem.SetTextColour(wxColor(textcol));

    switch(FieldID)
    {
        case ALLORDER_OrderLocalID:
            UpdateCol_String3(OrderSysID,wxString(pNew->OrderSysID).Trim(true).Trim(false));
            outItem.SetbtrynumericAsComparing(true);
            return true;
        case ALLORDER_InstrumentID:
            if(!bnewline) return false;
            UpdateCol_String(InstrumentID);
            return true;
        case ALLORDER_Direction:
            UpdateCol_Number(Direction,CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
            if ( pNew->Direction == THOST_FTDC_D_Buy )
                outItem.SetTextColour(wxColor(RGB(255,0,0)));//红色
            else if ( pNew->Direction == THOST_FTDC_D_Sell )
                outItem.SetTextColour(wxColor(RGB(0,128,0)));//绿色
            return true;
        case ALLORDER_CombOffsetFlag:
            UpdateCol_Number(CombOffsetFlag[0],CFieldValueTool::OffsetFlag2String(pNew->CombOffsetFlag[0],SVR_LANGUAGE));
            return true;
        case ALLORDER_OrderStatus:
            UpdateCol_Number(OrderStatus,CFieldValueTool::OrderStatus2String(pNew->OrderStatus,SVR_LANGUAGE));
            return true;
        case ALLORDER_LimitPrice:
            UpdateCol_Price(LimitPrice);
            return true;
        case ALLORDER_VolumeTotalOriginal:
            UpdateCol_Int(VolumeTotalOriginal);
            return true;
        case ALLORDER_VolumeTotal:
            UpdateCol_Int(VolumeTotal);
            return true;
        case ALLORDER_VolumeTraded:
            UpdateCol_Int(VolumeTraded);
            return true;
        case ALLORDER_StatusMsg:
        {
            if(!bnewline&&memcmp(pOld->StatusMsg,pNew->StatusMsg,sizeof(pOld->StatusMsg))==0) 
                return false;
            int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(pNew->StatusMsg);
            if(valStatusMsg>0)  tmpstr=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
            else tmpstr=wxString(pNew->StatusMsg);
            outItem.SetText(tmpstr);
            return true;
        }
        case ALLORDER_InsertTime:
			// InsertTime可能会改变的
            //if(!bnewline) return false;
            UpdateCol_String(InsertTime);
            return true;
        case ALLORDER_d_freezeMargin:
            UpdateCol_Fund(freezeMargin);
            return true;
        case ALLORDER_d_troubleMoney:
            UpdateCol_Fund(troubleMoney);
            return true;
        case ALLORDER_CombHedgeFlag:
            UpdateCol_Number(CombHedgeFlag[0],CFieldValueTool::HedgeFlag2String(pNew->CombHedgeFlag[0],SVR_LANGUAGE));
            return true;
        case ALLORDER_ExchangeID:
            if(!bnewline) return false;
            UpdateCol_String3(ExchangeID,CFieldValueTool::ExchangeID2Name((char*)pNew->ExchangeID));
            return true;
        case ALLORDER_BrokerOrderSeq:
            UpdateCol_Int(BrokerOrderSeq);
            return true;
        case ALLORDER_OrderPriceType:
            UpdateCol_Number(OrderPriceType,CFieldValueTool::OrderPriceType2String(pNew->OrderPriceType,SVR_LANGUAGE));
            return true;
        case ALLORDER_TimeCondition:
            UpdateCol_Number(TimeCondition,CFieldValueTool::TimeCondition2String(pNew->TimeCondition,SVR_LANGUAGE));
            return true;
        case ALLORDER_OrderType:
            UpdateCol_Number(OrderType,CFieldValueTool::OrderType2String(pNew->OrderType,SVR_LANGUAGE));
            return true;
        case ALLORDER_UserForceClose:
            UpdateCol_Number(UserForceClose,CFieldValueTool::UserForceClose2String(pNew->UserForceClose,SVR_LANGUAGE));
            return true;
        case ALLORDER_ForceCloseReason:
            UpdateCol_Number(ForceCloseReason,CFieldValueTool::ForceCloseReason2String(pNew->ForceCloseReason,SVR_LANGUAGE));
            return true;
        case ALLORDER_OrderSubmitStatus:
            UpdateCol_Number(OrderSubmitStatus,CFieldValueTool::OrderSubmitStatus2String(pNew->OrderSubmitStatus,SVR_LANGUAGE));
            return true;
        case ALLORDER_UserProductInfo:
            if(!bnewline) return false;
            UpdateCol_String(UserProductInfo);
            return true;
        case ALLORDER_UpdateTime:
            UpdateCol_String(UpdateTime);
            return true;
        case ALLORDER_d_avgPrice:
            UpdateCol_Price(AvgPrice);
            return true;
        case ALLORDER_Account:
            if(!bnewline) return false;
            UpdateCol_String(szAccount);
            return true;
	};
	return false;
}

bool CAllOrdersPanel::UpdateOpenListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_OrderInfo* pNew=(const PlatformStru_OrderInfo*)pNewData;
    PlatformStru_OrderInfo* pOld=(PlatformStru_OrderInfo*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    outItem.SetTextColour(wxColor(textcol));

	// 这里开始为未成交单
	switch(FieldID)
	{
		case OPENORDER_OrderLocalID:
            if(!bnewline) return false;
            UpdateCol_String(OrderSysID);
            outItem.SetbtrynumericAsComparing(true);
			return true;
		case OPENORDER_InstrumentID:
            if(!bnewline) return false;
            UpdateCol_String(InstrumentID);
			return true;
		case OPENORDER_Direction:
            UpdateCol_Number(Direction,CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
			if(pNew->Direction==THOST_FTDC_D_Buy)
				outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else
				outItem.SetTextColour(wxColor(RGB(0,128,0)));
			return true;
		case OPENORDER_CombOffsetFlag:
            UpdateCol_Number(CombOffsetFlag[0],CFieldValueTool::OffsetFlag2String(pNew->CombOffsetFlag[0],SVR_LANGUAGE));
			return true;
		case OPENORDER_VolumeTotalOriginal:
            UpdateCol_Int(VolumeTotalOriginal);
			return true;
		case OPENORDER_VolumeTotal:
            UpdateCol_Int(VolumeTotal);
			return true;
		case OPENORDER_LimitPrice:
            UpdateCol_Price(LimitPrice);
			return true;
		case OPENORDER_InsertTime:
			// InsertTime可能会改变的
            //if(!bnewline) return false;
            UpdateCol_String(InsertTime);
			return true;
		case OPENORDER_d_freezeMargin:																		//冻结保证金
            UpdateCol_Fund(freezeMargin);
			return true;
		case OPENORDER_d_troubleMoney:																		//冻结手续费
            UpdateCol_Fund(troubleMoney);
			return true;
		case OPENORDER_BrokerOrderSeq:																		//序列号
            UpdateCol_Int(BrokerOrderSeq);
			return true;
		case OPENORDER_ExchangeID:
            if(!bnewline) return false;
            UpdateCol_String3(ExchangeID,CFieldValueTool::ExchangeID2Name((char*)pNew->ExchangeID));
			return true;
		case OPENORDER_CombHedgeFlag:																		//投保
            UpdateCol_Number(CombHedgeFlag[0],CFieldValueTool::HedgeFlag2String(pNew->CombHedgeFlag[0],SVR_LANGUAGE));
			return true;
		case OPENORDER_VolumeTraded:
            UpdateCol_Int(VolumeTraded);
			return true;
		case OPENORDER_OrderPriceType:
            UpdateCol_Number(OrderPriceType,CFieldValueTool::OrderPriceType2String(pNew->OrderPriceType,SVR_LANGUAGE));
			return true;
		case OPENORDER_TimeCondition:
            UpdateCol_Number(TimeCondition,CFieldValueTool::TimeCondition2String(pNew->TimeCondition,SVR_LANGUAGE));
			return true;
		case OPENORDER_OrderType:
            UpdateCol_Number(OrderType,CFieldValueTool::OrderType2String(pNew->OrderType,SVR_LANGUAGE));
			return true;
		case OPENORDER_UpdateTime:
            UpdateCol_String(UpdateTime);
			return true;
		case OPENORDER_d_avgPrice:
			outItem.SetText("");
			return true;
		case OPENORDER_UserForceClose:
            UpdateCol_Number(UserForceClose,CFieldValueTool::UserForceClose2String(pNew->UserForceClose,SVR_LANGUAGE));
			return true;
		case OPENORDER_ForceCloseReason:
            UpdateCol_Number(ForceCloseReason,CFieldValueTool::ForceCloseReason2String(pNew->ForceCloseReason,SVR_LANGUAGE));
			return true;
		case OPENORDER_OrderSubmitStatus:
            UpdateCol_Number(OrderSubmitStatus,CFieldValueTool::OrderSubmitStatus2String(pNew->OrderSubmitStatus,SVR_LANGUAGE));
			return true;
		case OPENORDER_OrderStatus:
            UpdateCol_Number(OrderStatus,CFieldValueTool::OrderStatus2String(pNew->OrderStatus,SVR_LANGUAGE));
			return true;
		case OPENORDER_StatusMsg:																			//详细状态
		{					
            if(!bnewline&&memcmp(pOld->StatusMsg,pNew->StatusMsg,sizeof(pOld->StatusMsg))==0) 
                return false;
            int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(pNew->StatusMsg);
            if(valStatusMsg>0)  tmpstr=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
            else tmpstr=wxString(pNew->StatusMsg);
            outItem.SetText(tmpstr);
            return true;
		}
		case OPENORDER_UserProductInfo:
            if(!bnewline) return false;
            UpdateCol_String(UserProductInfo);
			return true;
		case OPENORDER_Price2:
            UpdateCol_String(Price2);
			return true;
		case OPENORDER_ACCOUNT:
            if(!bnewline) return false;
            UpdateCol_String(szAccount);
			return true;
	};
	return false;
}

void CAllOrdersPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
	{
		return;
	}

	int nColCount = m_pwxExtListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( m_nGID ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(m_nGID, i);
		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
	}
}
void CAllOrdersPanel::OnMAPlatformAdd(wxCommandEvent& event)
{
	m_pSvrSet.insert((IPlatformSingleSvr*)event.GetInt());
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
	m_pwxExtListCtrl->Clear();
	ShowAll();

}
void CAllOrdersPanel::OnMAPlatformDelete(wxCommandEvent& evt)
{
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(0);
	ProcessEvent(myEvent);
	m_pSvrSet.erase((IPlatformSingleSvr*)evt.GetInt());
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
	m_pwxExtListCtrl->Clear();
	ShowAll();
}