#include "StdAfx.h"
#include "OrderInsertPanel.h"
#include "../inc/Module-Misc/tools_util.h"
#include "COrderBatchDailog.h"
#include "CEditOrderChangeDailog.h"
#include "CEditOrderReplaceDailog.h"
#include "COrderConfirmDlg.h"
#include "COrderParkedSelectDlg.h"
#include "CMAOrderConfirmDlg.h"
#include "MultiAccountCore/MultiAccountCore.h"
#include "../inc/gui/EventParam.h"
#include "../Plusin/Plusin.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

OrderInsertPanel* s_pOrderInsertPanel=NULL;

int OrderInsertPanel::RspOrderInsertCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(!s_pOrderInsertPanel)
		return 0;

	wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, wxID_ANY);
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
	s_pOrderInsertPanel->AddPendingEvent(evt);

	return 0;
}

int OrderInsertPanel::TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  //成交回报
{
    if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
        return 0;

    if(!s_pOrderInsertPanel)
        return 0;

    DataRtnTrade* pTmp=new DataRtnTrade();
    memcpy(pTmp, &data, sizeof(DataRtnTrade));

    //post event to CAllOrdersPanel
    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, wxID_ANY);
    evt.SetClientData(pTmp);

    s_pOrderInsertPanel->AddPendingEvent(evt);
    return 0;
}

BEGIN_EVENT_TABLE(OrderInsertPanel,COrderInsertOperations)
    EVT_BUTTON(ID_BUTTON_OK,OnOk)
    EVT_BUTTON(ID_BUTTON_CANCEL,OnCancel)
    EVT_BUTTON(ID_BUTTON_PARKEDORDER,OnBtnParkedOrder)
    EVT_BUTTON(ID_BUTTON_MARKETPRICE,OnBtnMarketPrice)
    EVT_BUTTON(ID_BUTTON_BATCHORDER,OnBtnBatchOrder)
    EVT_BUTTON(ID_BUTTON_CONDITIONORDER,OnBtnConditionOrder)
    EVT_BUTTON(ID_BUTTON_POSITIONORDER,OnBtnPositionOrder)
    EVT_BUTTON(ID_BUTTON_STOPORDER,OnBtnStopOrder)
    EVT_BUTTON(ID_BUTTON_PATSORDER,OnBtnPatsOrder)
    EVT_CHECKBOX(ID_CHECK_WDSHOW,OnCheckWDShowClicked)
    EVT_CHECKBOX(ID_CHECK_WINDOWTOPALWAYS,OnCheckWinTopAlwaysClicked)
	EVT_CHECKBOX(ID_DOSTOPPOSITION_CHECK,OnCheckDoStopClicked)
    EVT_TIMER(ID_QUOTTIMER, OnTimer)
	EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
	EVT_PANEL_FOCUS(OnPanelFocus)
	EVT_PAINT(OnPaint)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, OrderInsertPanel::OnRcvOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_FIVEPRICE_PRICESELECTED, OnFivePriceSelected) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_INSTRUMENTID_CHANGED, OnInstrumentIdTextChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_AUTOOPENCLOSE_CHANGED, OnAutoOpenCloseChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_AUTOTRACKPRICE_CHANGED, OnAutoTrackPriceChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_BUYSELLSEL_CHANGED, OnBuySellSelChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_JSDPANEL_INSTRUMENTID_CHANGED, OnInstrumentIdTextChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_JSDPANEL_AUTOOPENCLOSE_CHANGED, OnAutoOpenCloseChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_JSDPANEL_AUTOTRACKPRICE_CHANGED, OnAutoTrackPriceChanged) 
    EVT_STYLE_CHANGE(OnCfgChanged)//风格改变时，重绘下单板
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_SUBSCRIBE(OnInitSubscribe)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD, OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
	EVT_COMMAND(wxID_ANY, wxEVT_GRID_INSTRUMENTID_CHANGED, OnCMDGridInstrumentIDChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_VIEWMODE_CHANGED, OnCMDViewModeChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_KEYORDER, OnCMDKeyOrder) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_MOUSEORDER, OnCMDMouseOrder) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_MOUSECLOSE, OnCMDMouseClose) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_ORDERCHANGE, OnCMDOrderChange) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_ORDERREPLACE, OnCMDOrderReplace) 
	EVT_COMMAND(wxID_ANY, wxEVT_MA_ORDERINSERT_MOUSECLOSE, OnCMDMouseMAClose) 
	//EVT_COMMAND(wxID_ANY, wxEVT_MA_ORDERINSERT_ORDERCHANGE, OnCMDMouseMACloseAll) 
	EVT_MOUSEWHEEL(OnMouseWheel)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


OrderInsertPanel::OrderInsertPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
	COrderInsertOperations(parent,winid,pos,size,style,name),
    m_timer(this,ID_QUOTTIMER),
	m_bUpdateRealMsg_Multiply(true),
	m_bUpdateRealMsg_Available(true),
	m_bUpdateRealMsg_OpenVolume(true),
	m_bUpdateRealMsg_PreClose(true)
{
    //行情更新定时器
    m_timer.Start(1000);
	s_pOrderInsertPanel=this;

	wxCommandEvent event(wxEVT_LANGUAGE_CHANGE,wxID_ANY);
	OnLanguageChanged(event);
    DoInitQryStep();
}

OrderInsertPanel::~OrderInsertPanel()
{
	s_pOrderInsertPanel=NULL;
}

OrderInsertPanel * OrderInsertPanel::CreatePane(wxWindow* parent)
{
    return new OrderInsertPanel(parent, GID_ORDER_INSERT_PANEL);
}

void OrderInsertPanel::SetViewStyle(int nViewStyle, int nViewSubStyle)
{
	
	__super::SetViewStyle(nViewStyle, nViewSubStyle);

	wxCheckBox* itemCheckWDShow = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW, this);
	if(itemCheckWDShow!=NULL) {
		itemCheckWDShow->SetValue(NeedFivePriceList()?true:false);
		wxCommandEvent evttemp(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_CHECK_WDSHOW);
		evttemp.SetInt(NeedFivePriceList());
		OnCheckWDShowClicked(evttemp);
	}

}

void OrderInsertPanel::SetViewSubStyle(int nViewSubStyle)
{
	
	__super::SetViewSubStyle(nViewSubStyle);

	wxCheckBox* itemCheckWDShow = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW, this);
	if(itemCheckWDShow!=NULL) {
		itemCheckWDShow->SetValue(NeedFivePriceList()?true:false);
        //wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,ID_BUTTON_REMOVE);
        //ProcessEvent(evt);
		wxCommandEvent evttemp(wxEVT_COMMAND_BUTTON_CLICKED, ID_CHECK_WDSHOW);
		evttemp.SetInt(NeedFivePriceList());
		OnCheckWDShowClicked(evttemp);
	}

}

void OrderInsertPanel::ResetMAOrderEntity(CMAOrderEntity* pOld, CMAOrderEntity* pNew)
{
	if(pOld==NULL || pNew==NULL)
		return;

	std::map<CMAOrderEntity*, CMAOrderConfirmDlg*>::iterator itMA = 
			m_mapMAOrderMonitoring.find(pOld);
	if(itMA==m_mapMAOrderMonitoring.end())
		return;

	CMAOrderConfirmDlg* pDlg = itMA->second;
	m_mapMAOrderMonitoring.erase(itMA);
	m_mapMAOrderMonitoring.insert(
			std::pair<CMAOrderEntity*, CMAOrderConfirmDlg*>(pNew, pDlg));
}

void OrderInsertPanel::NormalOrder()
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;

	wxString strName=GetInputInstrumentID();
	wxString strAccount=GetInputAccount();

		set<string> InstrumentIDArray;
		InstrumentIDArray.clear();
		IPlatformSingleSvr* pSvr = NULL;
		if(DEFAULT_SVR()->IsMultipleAccounts())
			pSvr = g_pPlatformMgr->GetCurrentPlatformSvr();
		else
			pSvr = DEFAULT_SVR();
		if(pSvr!=NULL) {
			pSvr->GetInstrumentList(InstrumentIDArray);
			if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
				ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
				return;
			}
		}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	wxString strMsg;

   //获得价格，和是否市价
	wxString strPriceText=GetInputPriceText();
	BOOL bIsMarketPrice = FALSE;
	fPrice = 0.0f;
	bIsMarketPrice = IsMarket(string(GetInputPriceText().c_str()));
	fPrice = atof(strPriceText.c_str());

	// 读取手数
	int nVolume = GetInputQty();
	if(nVolume<=0)
	{
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}

	// 读取方向
	BOOL bDirection = GetInputDirection();// GetButtonCheck(ID_RADIOBUTTON1);
	
	// 读取开平仓
	int nOCMode = GetInputOCMode();

	//获取是否自动
	BOOL bIsAuto = GetInputAutoOpenClose();

	//bool bHedge=!((wxCheckBox*)FindWindowById(ID_CHECKBOX1,this))->GetValue();
	BOOL bHedge = GetInputHedge();

	if(DEFAULT_SVR()->IsMultipleAccounts())
	{	// 多帐号下单过程

		OrderInsertUIItem orderUIItem;
		COrderAtomEntity::STDORDERREQ stdReq;
		CSAOrderEntity::OPERPARAM operParam;
		CMAOrderEntity::MAAUTOOPER maOper;

		ZeroMemory(&orderUIItem, sizeof(orderUIItem));
		ZeroMemory(&stdReq, sizeof(stdReq));
		ZeroMemory(&operParam, sizeof(operParam));
		ZeroMemory(&maOper, sizeof(maOper));

		maOper.nOperType = CSAOrderEntity::conOperNoneAuto;

		//operParam.nOperType = CSAOrderEntity::conOperNoneAuto;				// 执行类型，0 非自动单，1 自动单，2 非自动改单
		//operParam.nAutoMode = CMAOrderEntity::conOpenDefaultAndStrategy;		// 自动单模式，0 按持仓量全平，1 按默认手数平仓，不反向开仓
		//operParam.bNeedCancel = FALSE;										// 是否需要撤销相关报单
		//operParam.bIsCanClose = FALSE;										// 是否按可平仓数处理
		operParam.bSimulate = NeedSimulateMarketPrice();						// 是否进行市价模拟
		operParam.nDefaultVolume = nVolume;										// 默认手数
		operParam.bNoSendVolZoreOrder = TRUE;

		stdReq.nOrderType = OrderType_FT_Immediately;
		strcpy_s(stdReq.csInstrumentID, 
				sizeof(stdReq.csInstrumentID)-1, GetInputInstrumentID().c_str());		// 合约
		strcpy_s(stdReq.csUserID, sizeof(stdReq.csUserID)-1, 
				GetInputAccount().c_str());									// 投资者ID或Pats的用户名
		//strcpy_s(stdReq.csAccount, 
		//		sizeof(tOriReq.csAccount)-1, orderUIItem.Account);			// 帐号
		stdReq.bIsBuy = bDirection;
		stdReq.nOCMode = nOCMode;											// 开平仓模式, 0 开仓，1 平今，2 平仓
		stdReq.bIsMarket = bIsMarketPrice;	// 是否为市价
		stdReq.fPrice = stdReq.bIsMarket ? 0.0 : fPrice;
		stdReq.nVolume = nVolume;
		stdReq.bIsHedge = bHedge;

		BOOL bHasSelNoLogon = FALSE;
		map<string,bool> accsLogon;
		int nRet = GetLogonAccounts(accsLogon);
		if(accsLogon.size()==0) {
			wxMessageDialog dialog(NULL,LOADSTRING(请勾选已登录帐号进行下单),
					LOADSTRING(错误),wxOK|wxICON_ERROR);
			dialog.ShowModal();
			return;
		}
		bHasSelNoLogon = (nRet>0);
		if(bHasSelNoLogon) {
			wxMessageDialog dialog(NULL,LOADSTRING(勾选的账号中包含未登录的，\n未登录帐号不参与任何计算且不显示\n选择“是(Y)”继续下单，\n选择“否(N)”不下单),
					LOADSTRING(警告),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES)
				return;
		}
		DoMaOrderInsert(operParam, stdReq, maOper, accsLogon);
	}
	else
	{
		COrderInsertEntity::ORIORDERREQ tOriReq={0};
		strcpy(tOriReq.strInstrumentID, strName.c_str());
		strcpy(tOriReq.strAccount, strAccount.c_str());
		tOriReq.bIsParketOrder = FALSE;
		tOriReq.bDirection = bDirection;
		tOriReq.nOCMode = nOCMode;
		tOriReq.fPrice = fPrice;
		tOriReq.bIsMarketPrice = bIsMarketPrice;
		tOriReq.bSimulate = NeedSimulateMarketPrice();
		tOriReq.bIsHedge = bHedge;
		tOriReq.nVolume = nVolume;
		tOriReq.nDefaultVolume = nVolume;//GetDefaultVolume(strName);在OK按钮下，使用输入手数作为默认手数
		tOriReq.bIsAuto = FALSE; //bIsAuto;
		NeedAutoOpenClose(tOriReq.nAutoMode);

		BOOL bHasOrderInsert = FALSE;
		DoOrderInsert(tOriReq, 0, bHasOrderInsert, TRUE);
	}
}

void OrderInsertPanel::OnOk(wxCommandEvent& event)
{

	wxWindow* pWin = FindWindowById(ID_BUTTON_OK, this);
	if(pWin != NULL) {
		if(!pWin->IsShown() || !pWin->IsEnabled())
			return;
	}

	USERLOG_INFO("用户单击下单板[下单按钮]\n%s", m_poOrderInputPanel->GetUserInputText().c_str());

	NormalOrder();
	
}

void OrderInsertPanel::OnCancel(wxCommandEvent& event)
{
	USERLOG_INFO("用户单击下单板[取消按钮]\n");

    SetInputQty(0); 
}

void OrderInsertPanel::OnAutoOpenCloseChanged(wxCommandEvent& event)
{
	wxString strCode = GetInputInstrumentID();
	if(event.GetInt()) {
		int nSumVolume = 0, nSumTodayVolume=0;
		BOOL bIsBuy = GetInputDirection();
		BOOL bIsHedge = GetInputHedge();
		// 可平数 总仓/今仓
		COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
		if(nSumTodayVolume>0) {
			SetInputOCMode(1);
			//SetInputQty(nSumTodayVolume);
		}
		else if(nSumVolume>0){
			SetInputOCMode(2);
			//SetInputQty(nSumVolume-nSumTodayVolume);
		}
		else {
			SetInputOCMode(0);
			//SetInputQty(GetDefaultVolume(strCode));
		}
	}
	else {
		//SetInputQty(GetDefaultVolume(strCode));
	}

}

void OrderInsertPanel::OnAutoTrackPriceChanged(wxCommandEvent& event)
{
}

void OrderInsertPanel::OnBuySellSelChanged(wxCommandEvent& event)
{
	m_bUpdateRealMsg_OpenVolume = true;
	m_bUpdateRealMsg_PreClose = true;
}

void OrderInsertPanel::OnInstrumentIdTextChanged(wxCommandEvent& event)
{

	double fPriceTick = 0.0;

    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

    wxString strName(EvtParamStr);
//	wxString strName=event.GetString();
	wxString strLowerName = strName;
	strLowerName.LowerCase();

	IPlatformSingleSvr* pSvr=NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetCurrentPlatformSvr();
	else
		pSvr = DEFAULT_SVR();

	// 对用户输入的合约代码进行容错性转换
	if(!m_bIsGridChangeInstrumentID) {
		map<string, string>::iterator it;
		map<string, string> mapConstractIDVariety;
		mapConstractIDVariety.clear();
		if(pSvr) pSvr->GetInstrumentVarietyMap(mapConstractIDVariety);
		it = mapConstractIDVariety.find(strLowerName.c_str());
		if(it != mapConstractIDVariety.end()) {
			if(strName.Cmp(it->second.c_str())!=0) {
				SetInputInstrumentID(wxString(it->second.c_str()));
				return;
			}
		}
	}
	
	// 调用行情面板的高亮函数
	//post event to COrderServiceThread
//	if(event.GetInt()==0) {
	if(!m_bIsGridChangeInstrumentID) {
		wxCommandEvent evtnew(wxEVT_ORDERINSERT_INSTRUMENTID_CHANGED, GetId());
	    if(1)
	    {
		    DWORD EvtParamID;
		    std::string EvtParamStr(strName.c_str());
		    if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
		    {
			    evtnew.SetInt(EvtParamID);
			    GETTOPWINDOW()->AddPendingEvent(evtnew);
		    }
	    }
//		evtnew.SetString(strName.c_str());
//		GETTOPWINDOW()->AddPendingEvent(evtnew);
	}

	m_bUpdateRealMsg_Multiply = true;
	m_bUpdateRealMsg_OpenVolume = true;
	m_bUpdateRealMsg_PreClose = true;

	if(GetInputAutoOpenClose()) {
		int nSumVolume = 0, nSumTodayVolume=0;
		BOOL bIsBuy = GetInputDirection();
		BOOL bIsHedge = GetInputHedge();
		// 可平数 总仓/今仓
		COrderInsertEntity::GetPreVolume(strName.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
		if(nSumTodayVolume>0) {
			SetInputOCMode(1);
			//SetInputQty(nSumTodayVolume);
		}
		else if(nSumVolume>0){
			SetInputOCMode(2);
			//SetInputQty(nSumVolume-nSumTodayVolume);
		}
		else {
			SetInputOCMode(0);
			//SetInputQty(GetDefaultVolume(strCode));
		}
	}
	else {
		//SetInputQty(GetDefaultVolume(strCode));
	}

//	if(m_poStaticFivePriceList!=NULL) {
//		m_poStaticFivePriceList->ResetDepthMarketData(strName);
//	}
	set<string> InstrumentIDArray;
	InstrumentIDArray.clear();
	if(pSvr) pSvr->GetInstrumentList(InstrumentIDArray);
    if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
        //InitDynamicUpdateItem();
		if(m_poStaticFivePriceList!=NULL) {
			m_poStaticFivePriceList->ResetDepthMarketData();
 			m_poStaticFivePriceList->Refresh();
		}
		return;
    }

    PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
    if(pSvr && pSvr->GetInstrumentInfo(strName.c_str(),insInfo)!=0)
    {
        //InitDynamicUpdateItem();
		if(m_poStaticFivePriceList!=NULL) {
			m_poStaticFivePriceList->ResetDepthMarketData();
			m_poStaticFivePriceList->Refresh();
		}
        return;
    }
 	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetPriceTick(insInfo.PriceTick);
	}
 	if(m_poPatsOrderInputPanel!=NULL) {
		m_poPatsOrderInputPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poOrderStopPanel!=NULL) {
		m_poOrderStopPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poStaticFivePriceList!=NULL) {
		m_poStaticFivePriceList->SetPriceTick(insInfo.PriceTick);
	}
	m_currPriceTick=insInfo.PriceTick;

	//实时订阅行情
	static wxString LastValidInstrumentID;
	BOOL bIsComboCode = FALSE;
	string strCmbSubCode1, strCmbSubCode2;
	if(LastValidInstrumentID!=strName && pSvr)
	{
		if(!LastValidInstrumentID.empty()) {
			bIsComboCode = pSvr->GetProductClassType(LastValidInstrumentID.c_str()) == THOST_FTDC_PC_Combination;
			if(bIsComboCode) {
				ConvertComboInstrumentID(string(LastValidInstrumentID.c_str()), strCmbSubCode1, strCmbSubCode2);
				pSvr->SetSubscribeStatus( strCmbSubCode1,S_STATUS_TEMP,S_STATUS_DEL);
				pSvr->SetSubscribeStatus( strCmbSubCode2,S_STATUS_TEMP,S_STATUS_DEL);
			}
			pSvr->SetSubscribeStatus( LastValidInstrumentID.c_str(),S_STATUS_TEMP,S_STATUS_DEL);
		}
		bIsComboCode = pSvr->GetProductClassType(strName.c_str()) == THOST_FTDC_PC_Combination;
		if(bIsComboCode) {
			ConvertComboInstrumentID(string(strName.c_str()), strCmbSubCode1, strCmbSubCode2);
			pSvr->SetSubscribeStatus( strCmbSubCode1,S_STATUS_TEMP);
			pSvr->SetSubscribeStatus( strCmbSubCode2,S_STATUS_TEMP);
		}
		pSvr->SetSubscribeStatus( strName.c_str(),S_STATUS_TEMP);
		LastValidInstrumentID=strName;
	}

	PlatformStru_DepthMarketData field;
	memset(&field,0,sizeof(field));
	m_NeedUpdatePrice = true;
	BOOL bHasQuot = FALSE;
    if(pSvr && pSvr->GetQuotInfo(strName.c_str(),field)==0) {
		bHasQuot = TRUE;
    }
 	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderConditionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderPositionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderStopPanel!=NULL) {
		m_poOrderStopPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderStopPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_currUpperLimitPrice=field.UpperLimitPrice;
	m_currLowerLimitPrice=field.LowerLimitPrice;

	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) {	
	}
    m_poStaticFivePriceList->Refresh();
	if(bHasQuot) {
		if(GetInputDirection()) {
			if(util::isInvalidValue(field.AskPrice1)) {
				if(field.Volume > 0)
					SetInputPrice(field.LastPrice);
				else 
					SetInputPrice(field.PreSettlementPrice);
			}
			else
				SetInputPrice(field.AskPrice1);
		}
		else {
			if(util::isInvalidValue(field.BidPrice1)) {
				if(field.Volume > 0)
					SetInputPrice(field.LastPrice);
				else 
					SetInputPrice(field.PreSettlementPrice);
			}
			else
				SetInputPrice(field.BidPrice1);
		}
		m_NeedUpdatePrice = false;
	}
	SetInputQty(GetDefaultVolume(strName));


	wxCheckBox* pitemCheck = (wxCheckBox*)FindWindowById(ID_DOSTOPPOSITION_CHECK, this);
	if(pitemCheck!=NULL) {
		if(pitemCheck->GetValue()!=0) {

			// 这里从xml中读是否要确认的配置
			CfgMgr* pMgr = CFG_MGR_DEFAULT();  
			if(pMgr != NULL) {

			vector<VarietyInfo>* parrVarietyInfo;
			parrVarietyInfo = pMgr->GetVarietyInfoVector();
			VarietyInfo tvInfo;
			wxString strCode = GetInputInstrumentID();
			double fGainOffset = 0.0;
			double fLoseOffset = 0.0;
			double fReverseOffset = 0.0;
			double fRetPrice = 0.0;
			BOOL bFound = FALSE;
			
			for(int i=0; i<(int)parrVarietyInfo->size(); i++) {
				tvInfo = (*parrVarietyInfo)[i];
				wxString strValue = tvInfo.InstruName;
				if(strCode.compare(strValue.c_str())==0 
						|| strnicmp(strValue.c_str(), strCode.c_str(), strValue.size())==0) {
					fLoseOffset = atof(tvInfo.loss);
					fGainOffset = atof(tvInfo.profit);
					fReverseOffset = atof(tvInfo.reverse);
					bFound = TRUE;
					break;
				}
			}
			if(bFound) {
				m_poOrderPositionPanel->SetLoseOffsetPrice(
						m_poOrderPositionPanel->GetLoseOffsetPrice(fRetPrice), 
						fLoseOffset*m_currPriceTick);
				m_poOrderPositionPanel->SetGainOffsetPrice(
						m_poOrderPositionPanel->GetGainOffsetPrice(fRetPrice), 
						fGainOffset*m_currPriceTick);
				m_poOrderPositionPanel->SetCloseOffsetPrice(
						fReverseOffset*m_currPriceTick);
			}
			else {
				m_poOrderPositionPanel->SetLoseOffsetPrice(
						m_poOrderPositionPanel->GetLoseOffsetPrice(fRetPrice), 0.0f);
				m_poOrderPositionPanel->SetGainOffsetPrice(
						m_poOrderPositionPanel->GetGainOffsetPrice(fRetPrice), 0.0f);
				m_poOrderPositionPanel->SetCloseOffsetPrice(0.0f);
			}
			}
		}
	}

}

void OrderInsertPanel::OnTimer(wxTimerEvent& event)
{
    wxString name=GetInputInstrumentID();
    PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	double fPriceTick = m_currPriceTick;
	
	BOOL bHasQuot = FALSE;
	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	IPlatformSingleSvr* pSvr=NULL;
	if(g_pPlatformMgr!=NULL && DEFAULT_SVR()!=NULL && DEFAULT_SVR()->IsMultipleAccounts())
	{
		pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
		if(pSvr && pSvr->GetQuotInfo(name.c_str(),field)==0)
			bHasQuot = TRUE;
	}
	else 
	{
		pSvr=DEFAULT_SVR();
		if(pSvr&&pSvr->GetQuotInfo(name.c_str(),field)==0)
			bHasQuot = TRUE;
	}
	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderConditionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderPositionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderStopPanel!=NULL) {
		m_poOrderStopPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderStopPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_currUpperLimitPrice=field.UpperLimitPrice;
	m_currLowerLimitPrice=field.LowerLimitPrice;

	m_currPriceTick = fPriceTick;

	if(bHasQuot) {
		//更新跟盘价
		if(GetInputAutoTrackPrice()) {
			if(GetInputDirection())
				SetInputPrice(field.AskPrice1);
			else
				SetInputPrice(field.BidPrice1);
		}
		else {
			if(m_NeedUpdatePrice) {
				m_NeedUpdatePrice=FALSE;
				if(GetInputDirection())
					SetInputPrice(field.AskPrice1);
				else
					SetInputPrice(field.BidPrice1);
			}
		}
	}
	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) {
		m_poStaticFivePriceList->Refresh();
	}

	static int cnt1=0,cnt2=0;

	if(m_bWaitingRsp_OrderInsert) {
		cnt1++;
		if(cnt1>6) {
			m_bWaitingRsp_OrderInsert=false;
			cnt1=0;
            //wxMessageBox("下单成功，但是综合交易平台没有返回!");
		}
	}
	else {
		cnt1=0;
	}

	if(m_bWaitingRsp_OrderDelete) {
		cnt2++;
		if(cnt2>6) {
			m_bWaitingRsp_OrderDelete=false;
			cnt2=0;
            //wxMessageBox("撤单成功，但是综合交易平台没有返回!");
		}
	}
	else {
		cnt2=0;
	}


	{
	// 处理那些下单前需要撤消前期相关挂单的原始单
	std::vector<COrderInsertEntity*> arrOverdue;
	std::map<COrderInsertEntity*, int>::iterator itOrder;
	itOrder = m_mapOrderInsertWaitForCancel.begin();

	while(itOrder!=m_mapOrderInsertWaitForCancel.end()) {
		if(itOrder->second>0) itOrder->second--;
		if(itOrder->second>0) {
			// 未过期
			m_mapOrderInsertWaitForCancel[itOrder->first] = itOrder->second;
		}
		else {
			// 已过期
			arrOverdue.push_back(itOrder->first);
		}
		itOrder++;
	}

	// 清除所有过期的订单
	for(int i=0; i<(int)arrOverdue.size(); i++) {
		COrderInsertEntity* pEntity = arrOverdue[i];
		m_mapOrderInsertWaitForCancel.erase(pEntity);
		wxString strMsg = pEntity->GetRevokeOrderDesciption();
		strMsg += pEntity->GetOrderDesciption();
		ShowTradeInfoDlg(LOADSTRING(CANCEL_OVER_TIME), strMsg.c_str(), TRUE);
		try {
			delete pEntity;
		}
		catch(...) {}
	}
	arrOverdue.clear();
	}
	

	{
	// 处理改单请求
	std::queue<COrderAtomEntity*> queueOverdue;
	std::map<COrderAtomEntity*, int>::iterator itOrder;
	itOrder = m_mapCancelThenOrder.begin();

	while(itOrder!=m_mapCancelThenOrder.end()) {
		break;
		if(itOrder->second>0) 
			itOrder->second--;
		if(itOrder->second<=0) {
			// 已过期
			queueOverdue.push(itOrder->first);
		}
		itOrder++;
	}

	// 清除所有过期的订单
	while(!queueOverdue.empty()) {
		COrderAtomEntity* pEntity = queueOverdue.front();
		queueOverdue.pop();
		m_mapCancelThenOrder.erase(pEntity);
		wxString strMsg = pEntity->GetRevokeDesciption();
		strMsg += pEntity->GetOrderDesciption();
		ShowTradeInfoDlg(LOADSTRING(CANCEL_OVER_TIME), strMsg.c_str(), TRUE);
		try {
			delete pEntity;
		} catch(...) {}
	}
	}


	{
	LockObject();
	DWORD dwCurrTickCount = ::GetTickCount();
	std::map<CMAOrderEntity*, CMAOrderConfirmDlg*>::iterator itMA = 
			m_mapMAOrderMonitoring.begin();	// 所有需要等待撤单完成后才能下单的实体的集合
	for(; itMA != m_mapMAOrderMonitoring.end(); itMA++) 
	{
		if(itMA->first==NULL)
			continue;
		if(!itMA->first->NeedCancelOrder())
			continue;
		if(itMA->first->m_bIsOverTime)
			continue;

		if(dwCurrTickCount - itMA->first->m_dwStartTick >= 2000)
			itMA->first->m_bIsOverTime=TRUE;
	}
	itMA = m_mapMAOrderMonitoring.begin();
	for(; itMA != m_mapMAOrderMonitoring.end(); itMA++) 
	{
		if(itMA->second!=NULL && !itMA->second->IsShown()) {
			// 处理过期
			delete itMA->second;
			itMA->second = NULL;
		}
	}

	std::vector<CMAOrderEntity*> arrMAOverdue;
	itMA = m_mapMAOrderMonitoring.begin();	// 所有需要等待撤单完成后才能下单的实体的集合
	for(; itMA != m_mapMAOrderMonitoring.end(); itMA++) 
	{
		if(itMA->second==NULL && itMA->first!=NULL 
				&& (itMA->first->Status_DoInsert()!=CMAOrderEntity::conWaiting
					|| (itMA->first->NeedCancelOrder() && itMA->first->m_bIsOverTime))) {
			// 已过期
			arrMAOverdue.push_back(itMA->first);
		}
	}
	// 清除所有过期的订单
	for(int i=0; i<(int)arrMAOverdue.size(); i++) {
		CMAOrderEntity* pEntity = arrMAOverdue[i];
		m_mapMAOrderMonitoring.erase(pEntity);
		//wxString strMsg = pEntity->GetRevokeOrderDesciption();
		//strMsg += pEntity->GetOrderDesciption();
		//ShowTradeInfoDlg(LOADSTRING(CANCEL_OVER_TIME), strMsg.c_str(), TRUE);
		try {
			delete pEntity;
		}
		catch(...) {}
	}
	UnlockObject();
	arrMAOverdue.clear();
	}

	wxString strCode = GetInputInstrumentID();
	double fPrice = GetInputPrice();
	BOOL bIsBuy = GetInputDirection();
	BOOL bIsHedge = GetInputHedge();
	int nDefaultVolume = 1, nMultiplyVolume = 1;
	GetDefaultVolume(strCode, nDefaultVolume, nMultiplyVolume);

    BOOL bIsMarketPrice = FALSE;
	wxString strPriceText = GetInputPriceText();
	if(strPriceText.compare(LOADSTRING(OLS_MARKET))==0) {
		bIsMarketPrice=TRUE;
	}
	else {
		bIsMarketPrice=FALSE;
	}

	m_poOrderRealMsgPanel->ResetMsg(strCode, bIsBuy, bIsHedge, fPrice, 
			nDefaultVolume, nMultiplyVolume, bIsMarketPrice?true:false, wxString(GetInputAccount().c_str()));
	m_bUpdateRealMsg_Multiply = m_bUpdateRealMsg_PreClose = false;
	//m_bUpdateRealMsg_Available = m_bUpdateRealMsg_OpenVolume = false;

}

void OrderInsertPanel::OnRcvOrder(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}

	switch (pAbstractBusinessData->BID)
	{
		case BID_RtnOrder:
		{
			DataRtnOrder& RawData= *(DataRtnOrder*)(pAbstractBusinessData);

			{
			// 处理那些下单前需要撤消前期相关挂单的原始单
			std::vector<COrderInsertEntity*> arrComplete;
			std::map<COrderInsertEntity*, int>::iterator itOrder;
			itOrder = m_mapOrderInsertWaitForCancel.begin();

			while(itOrder!=m_mapOrderInsertWaitForCancel.end()) {
				COrderInsertEntity* pEntity = itOrder->first;
				pEntity->MarkRevokeItemState(RawData);
				if(pEntity->IsRevokeAllOK()) {
					arrComplete.push_back(pEntity);
				}
				itOrder++;
			}

			// 清除所有过期的订单
			for(int i=0; i<(int)arrComplete.size(); i++) {
				COrderInsertEntity* pEntity = arrComplete[i];
				m_mapOrderInsertWaitForCancel.erase(pEntity);
				int nErrPos=-1;
				wxString strMsg;
				pEntity->DoOrder(nErrPos, strMsg);
				if(nErrPos==-1) {
					m_bWaitingRsp_OrderInsert=true;
				}
				else {
					//设置错误信息
					//strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret).c_str());
					//ShowTradeInfoDlg(wxString("下单失败"), tmpData);
					//if(NeedOrderFailConfirm())
					ShowTradeInfoDlg(LOADSTRING(OLS_SEND_ORDER_FAIL), pEntity->GetOrderReq(nErrPos),NeedOrderFailConfirm());
				}
				try {
					delete pEntity;
				}
				catch(...) {}
			}
			arrComplete.clear();
			}

			{
			// 处理改单
			LockObject();
			std::queue<COrderAtomEntity*> queueComplete;
			std::map<COrderAtomEntity*, int>::iterator itOrder;
			itOrder = m_mapCancelThenOrder.begin();

			while(itOrder!=m_mapCancelThenOrder.end()) {
				COrderAtomEntity* pEntity = itOrder->first;
				if(pEntity->ParseOrderStatus(RawData)) {
					if(pEntity->GetRunStatus()==COrderAtomEntity::conIsCancelFail
							|| pEntity->GetRunStatus()==COrderAtomEntity::conWaiting
							|| pEntity->GetRunStatus()==COrderAtomEntity::conIsSuccess
							|| pEntity->GetRunStatus()==COrderAtomEntity::conIsFail) {
						queueComplete.push(pEntity);
					}
				}
				itOrder++;
			}

			// 清除所有过期的订单
			while(!queueComplete.empty()) {
				COrderAtomEntity* pEntity = queueComplete.front();
				queueComplete.pop();
				m_mapCancelThenOrder.erase(pEntity);
				if(!(pEntity->GetRunStatus()==COrderAtomEntity::conIsFail || 
						pEntity->GetRunStatus()==COrderAtomEntity::conIsCancelFail)) {
					m_bWaitingRsp_OrderInsert=true;
				}
				else {
					//设置错误信息
					ShowTradeInfoDlg(LOADSTRING(OLS_SEND_ORDER_FAIL), 
							pEntity->GetLastRspInfo().ErrorMsg, NeedOrderFailConfirm());
				}
				try {
					delete pEntity;
				} catch(...) {}
			}
			UnlockObject();
			}

			{
			int nErrNum = 0;
			std::string strErrorMsg;
			// 这里需要加锁
			LockObject();
			std::map<CMAOrderEntity*, CMAOrderConfirmDlg*>::iterator itMA = 
					m_mapMAOrderMonitoring.begin();
			for(; itMA!=m_mapMAOrderMonitoring.end(); itMA++)
			{
				if(itMA->first!=NULL && itMA->first->IsActive())
				{
					itMA->first->ResetOrderStatus(*(DataRtnOrder*)pAbstractBusinessData);
					if(itMA->first->NeedCancelOrder()) {
						itMA->first->MarkOrderActionStatus(*(DataRtnOrder*)pAbstractBusinessData);
						if(itMA->first->IsRevokeAllOK() 
								&& itMA->first->Status_DoInsert()==CMAOrderEntity::conWaiting)
							itMA->first->DoOrderReq(nErrNum, strErrorMsg);
					}
				}
			}
			itMA = m_mapMAOrderMonitoring.begin();
			for(; itMA!=m_mapMAOrderMonitoring.end(); itMA++)
			{
				if(itMA->second!=NULL)
				{
					itMA->second->ResetOrderStatus((DataRtnOrder*)pAbstractBusinessData);
				}
			}
			UnlockObject();
			}

			m_bUpdateRealMsg_Available = true;
			m_bUpdateRealMsg_OpenVolume = true;
		}
			break;
		case BID_RspOrderInsert:
		{
			{
			LockObject();
			std::map<CMAOrderEntity*, CMAOrderConfirmDlg*>::iterator itMA = 
					m_mapMAOrderMonitoring.begin();
			for(; itMA!=m_mapMAOrderMonitoring.end(); itMA++)
			{
				if(!itMA->first->IsActive())
					continue;
				itMA->first->ResetOrderStatus(*(DataRspOrderInsert*)pAbstractBusinessData);
				if(itMA->second!=NULL)
				{
					itMA->second->ResetOrderStatus((DataRspOrderInsert*)pAbstractBusinessData);
				}
			}
			UnlockObject();
			}
		}
			break;
		case BID_RspOrderAction1:
		case BID_RspOrderAction2:
		{
			//此处可以处理撤单失败的情况
			//OrderActionField.OrderActionStatus == THOST_FTDC_OAS_Rejected // 被拒绝
			
			{
			// 处理改单
			std::queue<COrderAtomEntity*> queueComplete;
			std::map<COrderAtomEntity*, int>::iterator itOrder;

			LockObject();

			itOrder = m_mapCancelThenOrder.begin();
			while(itOrder!=m_mapCancelThenOrder.end()) {
				COrderAtomEntity* pEntity = itOrder->first;
				if(pAbstractBusinessData->BID==BID_RspOrderAction1) {
					DataRspOrderAction1& RawData= *(DataRspOrderAction1*)(pAbstractBusinessData);
					if(pEntity->ParseActionStatus(RawData)) {
						if(pEntity->GetRunStatus()==COrderAtomEntity::conIsCancelFail
								|| pEntity->GetRunStatus()==COrderAtomEntity::conWaiting
								|| pEntity->GetRunStatus()==COrderAtomEntity::conIsSuccess
								|| pEntity->GetRunStatus()==COrderAtomEntity::conIsFail) {
							queueComplete.push(pEntity);
						}
					}
				}
				else if(pAbstractBusinessData->BID==BID_RspOrderAction2) {
					DataRspOrderAction2& RawData= *(DataRspOrderAction2*)(pAbstractBusinessData);
					if(pEntity->ParseActionStatus(RawData)) {
						if(pEntity->GetRunStatus()==COrderAtomEntity::conIsCancelFail
								|| pEntity->GetRunStatus()==COrderAtomEntity::conWaiting
								|| pEntity->GetRunStatus()==COrderAtomEntity::conIsSuccess
								|| pEntity->GetRunStatus()==COrderAtomEntity::conIsFail) {
							queueComplete.push(pEntity);
						}
					}
				}
				itOrder++;
			}

			// 清除所有过期的订单
			while(!queueComplete.empty()) {
				COrderAtomEntity* pEntity = queueComplete.front();
				queueComplete.pop();
				m_mapCancelThenOrder.erase(pEntity);
				if(pEntity->GetRunStatus()==COrderAtomEntity::conIsCancelFail) {
					m_bWaitingRsp_OrderInsert=true;
				}
				else {
					//设置错误信息
					ShowTradeInfoDlg(LOADSTRING(OLS_SEND_ORDER_FAIL), 
							pEntity->GetLastRspInfo().ErrorMsg, NeedOrderFailConfirm());
				}
				try {
					delete pEntity;
				} catch(...) {}
			}
			UnlockObject();
			}

			{
			LockObject();
			std::map<CMAOrderEntity*, CMAOrderConfirmDlg*>::iterator itMA = 
					m_mapMAOrderMonitoring.begin();
			for(; itMA!=m_mapMAOrderMonitoring.begin(); itMA++)
			{
				if(!itMA->first->IsActive())
					continue;
				if(pAbstractBusinessData->BID==BID_RspOrderAction1) 
					itMA->first->ResetActionStatus(*(DataRspOrderAction1*)pAbstractBusinessData);
				else if(pAbstractBusinessData->BID==BID_RspOrderAction2) 
					itMA->first->ResetActionStatus(*(DataRspOrderAction2*)pAbstractBusinessData);
				if(itMA->second!=NULL)
				{
					if(pAbstractBusinessData->BID==BID_RspOrderAction1) 
						itMA->second->ResetOrderStatus((DataRspOrderAction1*)pAbstractBusinessData);
					else if(pAbstractBusinessData->BID==BID_RspOrderAction2) 
						itMA->second->ResetOrderStatus((DataRspOrderAction2*)pAbstractBusinessData);
				}
			}
			UnlockObject();
			}


			if(pAbstractBusinessData->BID==BID_RspOrderAction2) {
				DataRspOrderAction2& RawData= *(DataRspOrderAction2*)(pAbstractBusinessData);
				// 处理那些下单前需要撤消前期相关挂单的原始单
				std::vector<COrderInsertEntity*> arrComplete;
				std::map<COrderInsertEntity*, int>::iterator itOrder;
				itOrder = m_mapOrderInsertWaitForCancel.begin();

				while(itOrder!=m_mapOrderInsertWaitForCancel.end()) {
					COrderInsertEntity* pEntity = itOrder->first;
					if(pEntity->FindRspOrderAction(RawData)) {
						arrComplete.push_back(pEntity);
					}
					itOrder++;
				}

				// 清除所有过期的订单
				for(int i=0; i<(int)arrComplete.size(); i++) {
					COrderInsertEntity* pEntity = arrComplete[i];
					m_mapOrderInsertWaitForCancel.erase(pEntity);
					int nErrPos=-1;
					wxString strMsg;
					pEntity->DoOrder(nErrPos, strMsg);
					if(nErrPos==-1) {
						m_bWaitingRsp_OrderInsert=true;
					}
					else {
						//设置错误信息
						//strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret).c_str());
						//ShowTradeInfoDlg(wxString("下单失败"), tmpData);
						//if(NeedOrderFailConfirm())
						ShowTradeInfoDlg(LOADSTRING(OLS_CANCEL_ORDER_FAIL), pEntity->GetOrderReq(nErrPos),NeedOrderFailConfirm());
					}
					try {
						delete pEntity;
					}
					catch(...) {}
				}
				arrComplete.clear();

				if(!m_bWaitingRsp_OrderDelete)
					break;
				//if(RawData.OrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
				if(!DEFAULT_SVR()||RawData.OrderActionField.RequestID!=DEFAULT_SVR()->GetNextReqID())
					break;			

//				if(NeedOrderFailConfirm())
//					ShowTradeInfoDlg(wxString("操作失败"), RawData.OrderActionField);
//				m_bWaitingRsp_OrderDelete=false;
			}
		}
			break;
		case BID_RtnTrade:
		{
			DataRtnTrade& RawData= *(DataRtnTrade*)(pAbstractBusinessData);
			if(GetInputAutoOpenClose()) {
				wxString strInstrumentID = GetInputInstrumentID();
				BOOL bDirection = GetInputDirection();
				BOOL bIsBuy = RawData.TradeField.Direction == THOST_FTDC_D_Buy;
				if(strInstrumentID.Cmp(RawData.TradeField.InstrumentID)==0 && bDirection!=bIsBuy) {
					SetInputOCMode(1);
				}
			}
			wxString strInstrumentID = GetInputInstrumentID();
			if(strInstrumentID.CmpNoCase(RawData.TradeField.InstrumentID)==0)
			{
				m_bUpdateRealMsg_PreClose = true;
			}
		}
			break;
		default:
			break;
	}
	
	delete (void*)pAbstractBusinessData;
	pAbstractBusinessData=NULL;
	evt.SetClientData(NULL);
}

void OrderInsertPanel::OnPanelCharHook(wxCommandEvent& evt)
{
    wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();

	if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
		evt.Skip();
        return;
    }
	//pEvent->SetId(win->GetId());
	pEvent->SetId(CBaseInputPanel::ConvertOwnerDrawnComboBoxID(win->GetId(), this));


	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;
	
	// 默认下单倍率
    int keycode=pEvent->GetKeyCode();
	map<int,string>::iterator itKey;
	//获得键盘与字符串的映射表
	map<int,string> addr = pMgr->GetShortCutKeyNameMap();
	itKey = addr.find(keycode);
	if(itKey != addr.end()) {
		std::string strKeyName = itKey->second;
		std::map<std::string, int>::iterator itOrder;
		itOrder = m_KeyMultiplyVolumeMap.find(strKeyName);
		if(itOrder != m_KeyMultiplyVolumeMap.end()) {
			int nVolume = itOrder->second;
			SetMultiplyVolume(nVolume);
			m_bUpdateRealMsg_Multiply = true;
		}
	// 这里从xml中读是否要确认的配置
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// 添加下单按钮快捷键
		if(strKeyName.compare(p->szOpenOrderBtnHotKey)==0) {
			wxCommandEvent newEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_OK);
			this->AddPendingEvent(newEvent);
		}
		// 添加下预埋单按钮快捷键
		if(strKeyName.compare(p->szOrderParkedBtnHotKey)==0) {
			wxCommandEvent newEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_PARKEDORDER);
			this->AddPendingEvent(newEvent);
		}
	}

	//获得键盘与字符串的映射表
	addr = pMgr->GetShortCutKeyNameMap();
	std::vector<VarietyInfo>* pVInfo = pMgr->GetVarietyInfoVector();
	VarietyInfo tvInfo;

	if(!(pEvent->GetId()==ID_OISTD_PRICE_TEXT || pEvent->GetId()==ID_OISTD_VOLUME_TEXT) || 
			!(pEvent->GetId()==ID_OIPATS_PRICE_TEXT || pEvent->GetId()==ID_OIPATS_VOLUME_TEXT)) {
		itKey = addr.find(keycode);
		if(itKey != addr.end()) {
			std::string strKeyName = itKey->second;
			for(int i=0; i<(int)pVInfo->size(); i++) {
				tvInfo = (*pVInfo)[i];
				wxString strValue = tvInfo.ShortKey;
				if(strValue.compare(strKeyName.c_str()) == 0) {
					wxString strInstrumentID = tvInfo.InstruName;
					SetInputInstrumentID(strInstrumentID);
					SetInputQty(tvInfo.volume);
					//bFound = TRUE;
					if(pEvent->GetId()==ID_OISTD_INSTRUMENTID_TEXT || 
							pEvent->GetId()==ID_OIPATS_INSTRUMENTID_TEXT) {
						win->SetFocus();
						return;
					}
					else
						break;
				}
			}
		}
	}

	int thisID=CBaseInputPanel::ConvertOwnerDrawnComboBoxID(win->GetId(), this);
	wxWindow* pTmpWin = NULL;

    if(pEvent->GetKeyCode()==WXK_RETURN || pEvent->GetKeyCode()==VK_RETURN) {
		if(m_nViewStyle!=3) {
			if(m_poOrderInputPanel) {
				wxKeyEvent keyEvent(*pEvent);
				m_poOrderInputPanel->OnHookKeyPress(keyEvent);
			}
		}
        int id, count=m_TabOrderVector.size();
		if(thisID==ID_BUTTON_OK || thisID==ID_BUTTON_BATCHORDER 
				|| thisID==ID_BUTTON_CONDITIONORDER || thisID==ID_BUTTON_POSITIONORDER 
				|| thisID==ID_BUTTON_CANCEL || thisID==ID_BUTTON_MARKETPRICE 
				|| thisID==ID_BUTTON_PARKEDORDER) {
			evt.Skip();
			return;
		}
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
					//evt.Skip();
                    return;
                }
            }
        }
    }
	else if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB) {
        bool backward = pEvent->ShiftDown ();
        int id, count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(backward)
                    id=id>0?id-1:count-1;
                else
                    id=id<count-1?id+1:0;
                FindWindowById(m_TabOrderVector[id],this)->SetFocus();
				//evt.Skip();
                return;
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
    {
        int id, count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                int nIndex=id>0?id-1:count-1;
				FindWindowById(m_TabOrderVector[nIndex],this)->SetFocus();
				return;
            }
        }
 		//return;
	}
    else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
    {
		//if(m_poOrderInputPanel) {
		//	wxKeyEvent keyEvent(*pEvent);
		//	m_poOrderInputPanel->OnHookKeyPress(keyEvent);
		//	if(!keyEvent.GetSkipped()) {
		//		return;
		//	}
		//}

        int id, count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
				int nIndex=id<count-1?id+1:0;
				FindWindowById(m_TabOrderVector[nIndex],this)->SetFocus();
				//evt.Skip();
				return;
			}
        }
		//return;
    }
	else {
		if(DEFAULT_SVR()!=NULL) {
		if(DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			if(m_poOrderInputPanel) {
				wxKeyEvent keyEvent(*pEvent);
				m_poOrderInputPanel->OnHookKeyPress(keyEvent);
				if(!keyEvent.GetSkipped()) {
					return;
				}
			}
		}
		else {//PTYPE_PATSAPI
			if(m_poPatsOrderInputPanel) {
				wxKeyEvent keyEvent(*pEvent);
				m_poPatsOrderInputPanel->OnHookKeyPress(keyEvent);
				if(!keyEvent.GetSkipped()) {
					return;
				}
			}
		}
		}
//		if(m_poOrderBatchPanel) {
//			wxKeyEvent keyEvent(*pEvent);
//			m_poOrderBatchPanel->OnHookKeyPress(keyEvent);
//			if(!keyEvent.GetSkipped()) {
//				return;
//			}
//		}
		if(m_poOrderConditionPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderConditionPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		if(m_poOrderPositionPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderPositionPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		//evt.Skip();
	}
	evt.Skip();
}

void OrderInsertPanel::OnBtnParkedOrder(wxCommandEvent& event)
{
	USERLOG_INFO("用户单击下单板[预埋单按钮]\n%s", m_poOrderInputPanel->GetUserInputText().c_str());
	
	wxWindow* pWin = FindWindowById(ID_BUTTON_PARKEDORDER, this);
	if(pWin != NULL) {
		if(!pWin->IsShown() || !pWin->IsEnabled())
			return;
	}

	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

    if(GetInputQty()<=0)
    {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
        return;
    }

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY), LOADSTRING(OIP_FASTTRADER), 
			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
			wxMessageDialog dialog( NULL,LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
	}

	// 超过涨跌停价提示
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}
	
	wxString strMsg;

   //获得价格，和是否市价
	wxString strPriceText=GetInputPriceText();
    BOOL bIsMarketPrice = FALSE;
	fPrice = 0.0;
	if(strPriceText.compare(LOADSTRING(OLS_MARKET))==0) {
		bIsMarketPrice=TRUE;
	}
	else {
		bIsMarketPrice=FALSE;
		fPrice = atof(strPriceText.c_str());
		//fPrice = AnalysePrice(fPrice, strName);
		//SetInputPrice(fPrice);
	}

	// 读取手数
	int nVolume = GetInputQty();

	// 读取方向
	BOOL bDirection = GetInputDirection();// GetButtonCheck(ID_RADIOBUTTON1);
	
	// 读取开平仓
	int nOCMode = GetInputOCMode();

	//获取是否自动
	BOOL bIsAuto = GetInputAutoOpenClose();

	//bool bHedge=!((wxCheckBox*)FindWindowById(ID_CHECKBOX1,this))->GetValue();
	BOOL bHedge = GetInputHedge();

	COrderParkedSelectDlg dlg(NULL, bIsMarketPrice==TRUE);
	dlg.SetTitle(LOADSTRING(OIP_PARK_TRIGGER));
	if(dlg.ShowModal()!=wxID_YES) {
		USERLOG_INFO("预埋单提示选择，标题[%s]，\n用户选择[否]",dlg.GetTitle());
		return;
	}

	int nParkedMode = dlg.GetParkedMode();
	USERLOG_INFO("预埋单提示选择，标题[%s]，\n用户选择[是]，%s",dlg.GetTitle(),dlg.GetUserInputText().c_str());

	if(nParkedMode<2) {
		
		COrderServiceThread::PARKEDORDER tOrder = {0};

		PlatformStru_InputOrder req;
		ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
		FillInputOrderField(req);

		tOrder.nState = conCondActive;
		tOrder.bIsAuto = (nParkedMode==1);
		tOrder.nCreateTime = ::wxGetUTCTime();
		tOrder.req = req;

		PlatformStru_InstrumentInfo insInfo;
		memset(&insInfo,0,sizeof(insInfo));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(tOrder.req.InstrumentID, insInfo)==0) {
			strcpy(tOrder.ExchangeID, insInfo.ExchangeID);
			if(m_poOrderServiceThread) {
				m_poOrderServiceThread->OrderParkedAdd(tOrder);
			}
			
			wxCommandEvent evtnew(wxEVT_QUERY_PARKED_REQUERY, GetId());
			GETTOPWINDOW()->AddPendingEvent(evtnew);
		}
		else {
			// 下预埋单失败，原因是未能查多合约属于那个交易所
		}

	}
	else {
		COrderInsertEntity::ORIORDERREQ tOriReq={0};
		strcpy(tOriReq.strInstrumentID, strName.c_str());
		tOriReq.bIsParketOrder = TRUE;
		tOriReq.bDirection = bDirection;
		tOriReq.nOCMode = nOCMode;
		tOriReq.fPrice = fPrice;
		tOriReq.bIsMarketPrice = bIsMarketPrice;
		tOriReq.bSimulate = NeedSimulateMarketPrice();
		tOriReq.bIsHedge = bHedge;
		tOriReq.nVolume = nVolume;
		tOriReq.nDefaultVolume = nVolume;//GetDefaultVolume(strName);在OK按钮下，使用输入手数作为默认手数
		tOriReq.bIsAuto = FALSE; //bIsAuto;
		NeedAutoOpenClose(tOriReq.nAutoMode);

		BOOL bHasOrderInsert = FALSE;
		DoOrderInsert(tOriReq, 0, bHasOrderInsert, TRUE);
	}
}

void OrderInsertPanel::OnBtnMarketPrice(wxCommandEvent& event)
{
    wxString strText(LOADSTRING(OLS_MARKET));
	SetInputPriceText(strText);

	USERLOG_INFO("用户单击下单板[市价单按钮]\n%s", m_poOrderInputPanel->GetUserInputText().c_str());
	
	NormalOrder();
}

void OrderInsertPanel::OnCheckWDShowClicked(wxCommandEvent& event)
{

	Scroll(0, 0);

	UIENTITY* pUIArr = NULL;

	if(DEFAULT_SVR()==NULL)
		return;
	if(DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		switch(m_nViewSubStyle) {
		case conSubStyle_Std:
			pUIArr = s_tStdView;
			break;
		case conSubStyle_Batch:
			pUIArr = s_tBatchView;
			break;
		case conSubStyle_Condition:
			pUIArr = s_tConditionView;
			break;
		case conSubStyle_Stop:
			pUIArr = s_tStopView;
			m_poOrderInputPanel->SetViewMode(4);
			break;
		case conSubStyle_Position:
			pUIArr = s_tPositionView;
			break;
		default:
			return;
			break;
		};
	}
	else {
		pUIArr = s_tPatsView;
	}

	int nWidth=0;

	if(!event.IsChecked()) {
		nWidth=pUIArr[conUI_WDList].width+pUIArr[conUI_WDList].left;
	}

	for(int i=0; i<conUI_MAX; i++) {
		((wxWindow*)pUIArr[i].pWin)->SetPosition(wxPoint(pUIArr[i].left-nWidth, pUIArr[i].top));
	}

    //long nStyle; 
    //nStyle = GetParent()->GetWindowStyle();
    //if((nStyle&wxPOPUP_WINDOW)==wxPOPUP_WINDOW)
    //{
    //    int OriWid,OriHei;
    //    int OriPox,OriPoy;
    //    GetParent()->GetSize(&OriWid,&OriHei);
    //    GetParent()->GetPosition(&OriPox,&OriPoy);
    //    if(event.IsChecked())
    //    {
    //        OriWid+=160;
    //        OriPox-=160;
    //        if(OriPox<0) OriPox=0;
    //    }
    //    else
    //    {
    //        OriWid-=160;
    //        OriPox+=160;
    //        if(OriWid<180) OriWid=180;
    //        RECT ScnRect;
    //        ::GetWindowRect(::GetDesktopWindow(),&ScnRect);
    //        if(OriPox+OriWid>ScnRect.right)
    //            OriPox=ScnRect.right-OriWid;
    //    }
    //    GetParent()->SetSize(OriPox,OriPoy,OriWid,OriHei);

    //}

	int nVirWidth = 0;
	switch(m_nViewSubStyle) {
	case conSubStyle_Std:		// 嵌入面板时
	case conSubStyle_Batch:		// 批量下单时
	case conSubStyle_Condition:		// 条件单时
	case conSubStyle_Stop:		// 停止单时
		if(event.IsChecked())
			nVirWidth = pUIArr[conUI_WDList].width+pUIArr[conUI_StdInput].width+5;
		else
			nVirWidth = pUIArr[conUI_WDList].width+5;
		break;
	case conSubStyle_Position:		// 盈损单时
		if(event.IsChecked())
			nVirWidth = pUIArr[conUI_WDList].width+pUIArr[conUI_StdInput].width+pUIArr[conUI_Position].width+5;
		else
			nVirWidth = pUIArr[conUI_StdInput].width+pUIArr[conUI_Position].width+5;
	};
	SetVirtualSize(wxSize(nVirWidth, pUIArr[conUI_WDList].height));
	SaveFivePriceList(event.IsChecked());

	wxCommandEvent evtnew(wxEVT_BUYSELL5_SHOW_CHANGE);
	evtnew.SetInt(event.IsChecked());
	evtnew.SetExtraLong(nVirWidth);
	GETTOPWINDOW()->AddPendingEvent(evtnew);

	this->Refresh();
}

void OrderInsertPanel::OnCheckWinTopAlwaysClicked(wxCommandEvent& event)
{
	//SetWindowStyle(this->GetWindowStyle() | wxSTAY_ON_TOP);
	////this->SetWindowStyle( this->GetWindowStyle() & (~wxSTAY_ON_TOP) );

	wxWindow* pParentWin = GetParent();
	wxCheckBox* pCheckBox = (wxCheckBox*)FindWindowById(ID_CHECK_WINDOWTOPALWAYS,this);
	if(pParentWin) {
		if(pCheckBox->GetValue()) {
			SetWindowPos((HWND)pParentWin->GetHWND(), HWND_TOPMOST,
					0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 
		}
		else {
			SetWindowPos((HWND)pParentWin->GetHWND(), HWND_NOTOPMOST,
					0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 
		}
	}
}

void OrderInsertPanel::OnBtnBatchOrder(wxCommandEvent& event)
{
	USERLOG_INFO("用户单击下单板[批量单按钮]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderBatchPanel->GetUserInputText().c_str());
	
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}
	if(m_poOrderBatchPanel->GetPriceType()==-1) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_BATCH_MODE",TRUE);
		return;
	}
	if(m_poOrderBatchPanel->GetBatchEachNum()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","BATCH_QTY_BIG_ZERO",TRUE);
		return;
	}
	if(m_poOrderBatchPanel->GetTimerInterval()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","BATCH_TIME_BIG_ZERO",TRUE);
		return;
	}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY), LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	// 超过涨跌停价提示
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	PlatformStru_InputOrder req={0};
//	m_poOrderInputPanel->FillOrder(req);

	BOOL bIsMarketPrice = FALSE;
	fPrice = 0.0;
	string strPriceText = m_poOrderInputPanel->GetPriceText().c_str();
	string strstlName = strName.c_str();
	BOOL bIsBuy = m_poOrderInputPanel->GetDirection();

	//Macro
	AUTOMARKETPRICE(strstlName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
	
	MakeReq_Order(req, strstlName, 
			m_poOrderInputPanel->GetAccount().c_str(), OrderType_FT_Immediately, 
			bIsBuy, m_poOrderInputPanel->GetOpenClose()+1, 
			m_poOrderInputPanel->GetQty(), fPrice, 0, m_poOrderInputPanel->GetHedge(), 
			bIsMarketPrice, FALSE);
	
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	wxString strMsg;
	strMsg.Printf(LOADFORMATSTRING(BATCH_PRCQTY_FORMAT,"%s%s%s%s%d%s%d%d%s"),//文本必须包含%s %s %s %s %d %s %d %d  %s 
				Price2String(req.LimitPrice, m_currPriceTick).c_str(),
				req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL),
				req.InstrumentID, 
				req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): 
				(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
				req.VolumeTotalOriginal,
				req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE) : LOADSTRING(OLS_SPECULATE),
				m_poOrderBatchPanel->GetTimerInterval(), 
				m_poOrderBatchPanel->GetBatchEachNum(), 
				m_poOrderBatchPanel->GetPriceType()==0 ? LOADSTRING(OLS_INPUTPRICE) : 
				(m_poOrderBatchPanel->GetPriceType()==1 ? LOADSTRING(OLS_LASTPRICE3) : LOADSTRING(OLS_B_S_PRICE3)));
	BOOL bOK = TRUE;
	if(NeedConfirm()) {
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        if(bIsMarketPrice)
        {
            strText += strMsg;
            strMsg = strText;
        }
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_BATCHCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("批量单提示选择，标题[%s]，\n用户选择[%s]",dialog.GetTitle(),bOK?"是":"否");
	}
	if(bOK) {
		COrderBatchDailog* pDailog = NULL;
		pDailog = new COrderBatchDailog(this, wxID_ANY, LOADSTRING(SEND_BACTH_ORDER), wxDefaultPosition, wxSize(310, 250));
		pDailog->SetOrderServiceThread(m_poOrderServiceThread);
		pDailog->SetPriceType(m_poOrderBatchPanel->GetPriceType());
		pDailog->SetOrderReq(req);
		pDailog->SetHedge(GetInputHedge());
		pDailog->SetBatchEachNum(m_poOrderBatchPanel->GetBatchEachNum());
		pDailog->SetTimer(m_poOrderBatchPanel->GetTimerInterval());
		pDailog->Show();
		if(GetInputAutoTrackPrice()) {
			SetInputAutoTrackPrice(NeedContinueTrackPrice());
		}
	}
}

void OrderInsertPanel::OnBtnConditionOrder(wxCommandEvent& event)
{
	USERLOG_INFO("用户单击下单板[条件单按钮]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderConditionPanel->GetUserInputText().c_str());
	
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}
	if(m_poOrderConditionPanel->GetPriceType()==-1) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_CONDITION_MODE",TRUE);
		return;
	}
	if(m_poOrderConditionPanel->GetConditionType()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_CONDI_OPERATOR",TRUE);
		return;
	}
	//if(m_poOrderConditionPanel->GetConditionPrice()<=0) {
	//	ShowTradeInfoDlg("OIP_ERROR","OIP_CONDITIONPRCNOT0",TRUE);
	//	return;
	//}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	fPrice = m_poOrderConditionPanel->GetConditionPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_CONDITION),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	// 超过涨跌停价提示
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	COrderServiceThread::CONDITIONORDER tOrder={0};
	ZeroMemory(&tOrder, sizeof(COrderServiceThread::CONDITIONORDER));
	PlatformStru_InputOrder req;
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
	FillInputOrderField(req);
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	tOrder.bHedge = FALSE;
	tOrder.nState = conCondActive;
	tOrder.nPriceType = m_poOrderConditionPanel->GetPriceType();
	tOrder.nConditionType = m_poOrderConditionPanel->GetConditionType();
	tOrder.fConditionPrice = m_poOrderConditionPanel->GetConditionPrice();
	tOrder.nCreateTime = ::wxGetUTCTime();
	tOrder.req = req;

	wxString strMsg;
	double fInputPrice=0.0;
	BOOL bIsSuccess = FALSE;

	switch(tOrder.nPriceType) {
	case 1:
		fInputPrice = field.LastPrice;
		break;
	case 2:
		fInputPrice = field.BidPrice1;
		break;
	case 3:
		fInputPrice = field.AskPrice1;
		break;
	};
	if(!util::isInvalidValue(fInputPrice)) {
	if(tOrder.nConditionType==1) {
		if(fInputPrice <= tOrder.fConditionPrice) {
			bIsSuccess = TRUE;
		}
	}
	else if(tOrder.nConditionType==2) {
		if(fInputPrice >= tOrder.fConditionPrice) {
			bIsSuccess = TRUE;
		}
	}
	else if(tOrder.nConditionType==3) {
		if(fInputPrice < tOrder.fConditionPrice) {
			bIsSuccess = TRUE;
		}
	}
	else if(tOrder.nConditionType==4) {
		if(fInputPrice > tOrder.fConditionPrice) {
			bIsSuccess = TRUE;
		}
	}
	}

	BOOL bOK = TRUE;
	if(bIsSuccess) {
        wxString strText;
        if(GetInputPriceText().compare(LOADSTRING(OLS_MARKET))==0)
        {
            strText.Printf("%s\n%s", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE), LOADSTRING(OIP_CONDITIONMEETCONF));
        }
        else
        {
            strText.Printf("%s", LOADSTRING(OIP_CONDITIONMEETCONF));
        }
        wxMessageDialog dialog(NULL, strText, LOADSTRING(OIP_CONDITIONCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("条件单立即下单提示选择，标题[%s]，\n用户选择[%s]\n",LOADSTRING(OIP_CONDITIONCONF),bOK?"是":"否");
		if(bOK) {
            int ret=DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(tOrder.req, DEFAULT_SVR()->GetNextReqID()):-1;
			if(ret) {
				DataRspOrderInsert tRspReq;
				tRspReq.InputOrderField = *((CThostFtdcInputOrderField*)(&tOrder.req));
				strcpy(tRspReq.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
				ShowTradeInfoDlg("OIP_SOF", tRspReq, NeedOrderFailConfirm());
			}
		}
		return;
	}

	wxString strCondDir;
	switch(tOrder.nConditionType) {
	case 1:
		strCondDir = wxT("<=");
		break;
	case 2: 
		strCondDir = wxT(">=");
		break;
	case 3:
		strCondDir = wxT("<");
		break;
	case 4: 
		strCondDir = wxT(">");
		break;
	};
	strMsg.Printf(LOADFORMATSTRING(OIP_EXECONPRC_FORMAT,"%s%s%s%s%s%s%s%s%d"), //文本必须包含%s%s%s%s %s%s%s%s %d
				m_poOrderConditionPanel->GetIsRunInServer() ? LOADSTRING(OIP_SERVER) : LOADSTRING(OIP_LOCAL), 
				tOrder.req.InstrumentID, 
				tOrder.nPriceType==2 ? LOADSTRING(OIP_BIDPRICE) : (tOrder.nPriceType==3 ? LOADSTRING(OIP_ASKPRICE) : LOADSTRING(OIP_LASTPRICE)), 
				strCondDir, 
				Price2String(tOrder.fConditionPrice, m_currPriceTick).c_str(), 
				Price2String(tOrder.req.LimitPrice, m_currPriceTick).c_str(), 
				tOrder.req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
				tOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): (
				tOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
				tOrder.req.VolumeTotalOriginal);
	if(NeedConfirm()) {
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        if(GetInputPriceText().compare(LOADSTRING(OLS_MARKET))==0)
        {
            strText += strMsg;
            strMsg = strText;
        }
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_CONDITIONCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("条件单正常提示选择，标题[%s]，\n用户选择[%s]\n",LOADSTRING(OIP_CONDITIONCONF),bOK?"是":"否");
	}
	if(bOK) {
		if(m_poOrderConditionPanel->GetIsRunInServer()) {
			req.StopPrice = tOrder.fConditionPrice;
			switch(tOrder.nPriceType) {
			case 1:	// 1 最新
				switch(tOrder.nConditionType) {
				case 1:		// 1 小于等于指定价格
					req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
					break;
				case 2:		// 2 大于等于指定价格
					req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
					break;
				case 3:		// 3 小于
					req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserThanStopPrice;
					break;
				case 4:		// 4 大于
					req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
					break;
				};
				break;
			case 2:	// 2 买价
				switch(tOrder.nConditionType) {
				case 1:		// 1 小于等于指定价格
					req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice;
					break;
				case 2:		// 2 大于等于指定价格
					req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice;
					break;
				case 3:		// 3 小于
					req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserThanStopPrice;
					break;
				case 4:		// 4 大于
					req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterThanStopPrice;
					break;
				};
				break;
			case 3: // 3 卖价
				switch(tOrder.nConditionType) {
				case 1:		// 1 小于等于指定价格
					req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice;
					break;
				case 2:		// 2 大于等于指定价格
					req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice;
					break;
				case 3:		// 3 小于
					req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserThanStopPrice;
					break;
				case 4:		// 4 大于
					req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterThanStopPrice;
					break;
				};
				break;
			};

			
			int nRet = -1;
			wxString strRetMsg="";
            nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;
			if(GetInputAutoTrackPrice()) {
				SetInputAutoTrackPrice(NeedContinueTrackPrice());
			}
			if(nRet!=0) {
				//COrderServiceThread::sm_nReqNum--;
				DataRspOrderInsert tInsertOrder;
				ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
				//tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&req);
				strcpy(tInsertOrder.InputOrderField.InstrumentID, req.InstrumentID);
				tInsertOrder.InputOrderField.Direction = req.Direction;
				tInsertOrder.InputOrderField.CombOffsetFlag[0] = req.CombOffsetFlag[0];
				tInsertOrder.InputOrderField.CombHedgeFlag[0] = req.CombHedgeFlag[0];
				tInsertOrder.InputOrderField.LimitPrice = req.LimitPrice;
				tInsertOrder.InputOrderField.StopPrice = req.StopPrice;
				tInsertOrder.InputOrderField.VolumeTotalOriginal = req.VolumeTotalOriginal;				//设置错误信息
				strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
				ShowTradeInfoDlg("OIP_SOF", tInsertOrder, NeedOrderFailConfirm());
			}
		}
		else {
			if(m_poOrderServiceThread) {
				m_poOrderServiceThread->OrderConditionAdd(tOrder);

				if(GetInputAutoTrackPrice()) {
					SetInputAutoTrackPrice(NeedContinueTrackPrice());
				}
				wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
				GETTOPWINDOW()->AddPendingEvent(evtnew);
			}
		}
	}

}

void OrderInsertPanel::OnBtnPositionOrder(wxCommandEvent& event)
{
	USERLOG_INFO("用户单击下单板[盈损单按钮]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderPositionPanel->GetUserInputText().c_str());
	
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR", TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO", TRUE);
		return;
	}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}

	int nOffsetPriceObject = 0;

	nOffsetPriceObject = m_poOrderPositionPanel->GetOffsetPriceObject();

	BOOL bIsCheckLose = FALSE, bIsCheckGain = FALSE;
	double fLoseOffsetPrice = 0.0f, fGainOffsetPrice = 0.0f;

	bIsCheckLose = m_poOrderPositionPanel->GetLoseOffsetPrice(fLoseOffsetPrice);
	bIsCheckGain = m_poOrderPositionPanel->GetGainOffsetPrice(fGainOffsetPrice);

	if(bIsCheckLose && CheckSumPrice(fLoseOffsetPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_LOSE),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	if(bIsCheckGain && CheckSumPrice(fGainOffsetPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_GAIN),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	
	int nCloseMode = 0;
	double fCloseOffsetPrice = 0.0;

	nCloseMode = m_poOrderPositionPanel->GetCloseMode();
	fCloseOffsetPrice = m_poOrderPositionPanel->GetCloseOffsetPrice();
	if(CheckSumPrice(fCloseOffsetPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_CLOSEREVERSE),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}


	if(bIsCheckLose == FALSE && bIsCheckGain == FALSE) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_SOPPO", TRUE);
		return;
	}
	if(bIsCheckLose != FALSE && fLoseOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_STEOPPRCBIGZERO", TRUE);
		return;
	}
	if(bIsCheckGain != FALSE && fGainOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_PRCDELTABIGZERO", TRUE);
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY), LOADSTRING(OIP_FASTTRADER), 
			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;

	}

	// 超过涨跌停价提示
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	int nConditionPriceType = 0;
	nConditionPriceType = m_poOrderPositionPanel->GetPriceType();

	PlatformStru_InputOrder req={0};
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));

	int ret=0;
    
	// 首先把界面的数据填入，然后再进行自动平仓处理
	FillInputOrderField(req);
	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	wxString strText;
	strText.Printf(LOADFORMATSTRING(OIP_LRTSFC,"%f"), fCloseOffsetPrice);//文本必须包含%f

	wxString strMsg;
	BOOL bOK = TRUE;
	if(NeedConfirm()) {
		int nOpenClose = 0;
		if(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open)
			nOpenClose=0;
		else if(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday)
			nOpenClose=1;
		else
			nOpenClose=2;
		//	strAutoOpenClose,
		strMsg.Printf(LOADFORMATSTRING(COPQSOC_FOMRAT,"%s%s%s%s%s%d%f%f%s"),//文本必须包含%s %s、%s、%s %s %d %0.2f %0.2f %s
				strName.Trim(),
				GetInputHedge() ? LOADSTRING(OLS_HEDGE):LOADSTRING(OLS_SPECULATE),
				req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
				req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
				(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)),
				Price2String(req.LimitPrice, m_currPriceTick).c_str(), 
				req.VolumeTotalOriginal,
				fLoseOffsetPrice, 
				fGainOffsetPrice, 
				nCloseMode==1 ? LOADSTRING(OLS_PRICE_LIMIT) : strText);		//wxString strLose, strGain;
		//strLose.Printf("止损价差：%s", Price2String(fLoseOffsetPrice, m_currPriceTick).c_str());
		//strGain.Printf("止盈价差：%s", Price2String(fGainOffsetPrice, m_currPriceTick).c_str());
		//strMsg.Printf(LOADFORMATSTRING(COPQSOC_FOMRAT,"%s%s%s%s%s%d%f%f%s"),//文本必须包含%s %s、%s、%s %s %d %0.2f %0.2f %s
		//		strName.Trim(),
		//		GetInputHedge() ? LOADSTRING(OLS_HEDGE):LOADSTRING(OLS_SPECULATE),
		//		req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
		//		req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
		//		(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)),
		//		Price2String(req.LimitPrice, m_currPriceTick).c_str(), 
		//		req.VolumeTotalOriginal,
		//		bIsCheckLose?strLose.c_str():"", bIsCheckGain?strGain.c_str():"", 
		//		nCloseMode==1 ? LOADSTRING(OLS_PRICE_LIMIT) : strText);
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        if(GetInputPriceText().compare(LOADSTRING(OLS_MARKET))==0)
        {
            strText += strMsg;
            strMsg = strText;
        }
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_PLCONF),wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("盈损单提示选择，标题[%s]，\n用户选择[%s]\n",LOADSTRING(OIP_PLCONF),bOK?"是":"否");
	}
	if(bOK) {
        ret=DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;
		if(GetInputAutoTrackPrice()) {
			SetInputAutoTrackPrice(NeedContinueTrackPrice());
		}
		if(ret) {
			//COrderServiceThread::sm_nReqNum--;
			DataRspOrderInsert tmpData;
			ZeroMemory(&tmpData, sizeof(DataRspOrderInsert));
			tmpData.InputOrderField = *(CThostFtdcInputOrderField*)(&req);

			//设置错误信息
			strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
			//if(NeedOrderFailConfirm())
			ShowTradeInfoDlg("OIP_SOF", tmpData, NeedOrderFailConfirm());
			return;
		}
	}

	wxString strCaption;
	if(bOK && ret==0) {
		m_bWaitingRsp_OrderInsert=true;

		PlatformStru_DepthMarketData tInstrfield;
        memset(&tInstrfield,0,sizeof(tInstrfield));
		// 从服务器更新价格
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(), tInstrfield);

		int nLoseCondNum = 0, nGainCondNum = 0;

		COrderServiceThread::POSITIONORDER tPosOrder={0};
		ZeroMemory(&tPosOrder, sizeof(COrderServiceThread::POSITIONORDER));

		tPosOrder.bIsRunInServer = m_poOrderPositionPanel->GetIsRunInServer();
		tPosOrder.bHedge = FALSE;
		tPosOrder.nState = conPositionActive;
		m_poOrderServiceThread->OrderPositionAdd(tPosOrder);

		// 下止损条件单
		COrderServiceThread::CONDITIONORDER tOrderLose={0};
		ZeroMemory(&tOrderLose, sizeof(COrderServiceThread::CONDITIONORDER));
		PlatformStru_InputOrder reqLose;
		ZeroMemory(&reqLose, sizeof(PlatformStru_InputOrder));
		if(bIsCheckLose) {
			FillInputOrderField(reqLose);
			reqLose.Direction = reqLose.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;
			reqLose.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;
			if(reqLose.Direction!=THOST_FTDC_D_Buy) {
				tOrderLose.fConditionPrice = reqLose.LimitPrice-fLoseOffsetPrice;
			}
			else {
				tOrderLose.fConditionPrice = reqLose.LimitPrice+fLoseOffsetPrice;
			}
			if(nCloseMode==1) {
				reqLose.LimitPrice = 
						(reqLose.Direction==THOST_FTDC_D_Buy ? 
                        tInstrfield.UpperLimitPrice : tInstrfield.LowerLimitPrice);
			}
			else {
				reqLose.LimitPrice = tOrderLose.fConditionPrice;
				reqLose.LimitPrice += 
						(reqLose.Direction==THOST_FTDC_D_Buy ? fCloseOffsetPrice : -fCloseOffsetPrice);
			}
			tOrderLose.nPosNum = tPosOrder.nNum;
			tOrderLose.nCondPosRelation = conCPRCondFromPos;
			tOrderLose.bHedge = FALSE;
			tOrderLose.nState = conCondPause;//conCondActive;
			tOrderLose.nPriceType = nConditionPriceType==1 ? nConditionPriceType : 
					(reqLose.Direction==THOST_FTDC_D_Buy ? 3:2);
			tOrderLose.nConditionType = (reqLose.Direction==THOST_FTDC_D_Buy ? 2:1);
			tOrderLose.nCreateTime = ::wxGetUTCTime();
			tOrderLose.req = reqLose;

			if(m_poOrderPositionPanel->GetIsRunInServer()) {
				reqLose.StopPrice = tOrderLose.fConditionPrice;
				reqLose.ContingentCondition = THOST_FTDC_CC_Touch;
				
				int nRet = -1;
				wxString strRetMsg="";
                nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(tOrderLose.req, DEFAULT_SVR()->GetNextReqID()):-1;
				if(nRet!=0) {
					DataRspOrderInsert tInsertOrder;
					ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
					tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&reqLose);
					//设置错误信息
					strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
					ShowTradeInfoDlg("OIP_SOF", tInsertOrder, NeedOrderFailConfirm());
				}
			}
			else {
				if(m_poOrderServiceThread) {
					nLoseCondNum = m_poOrderServiceThread->OrderConditionAdd(tOrderLose);
				}
			}
		}

		// 下止盈条件单
		COrderServiceThread::CONDITIONORDER tOrderGain={0};
		ZeroMemory(&tOrderGain, sizeof(COrderServiceThread::CONDITIONORDER));
		PlatformStru_InputOrder reqGain;
		ZeroMemory(&reqGain, sizeof(PlatformStru_InputOrder));
		if(bIsCheckGain) {
			FillInputOrderField(reqGain);
			reqGain.Direction = (reqGain.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy);
			reqGain.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;
			if(reqGain.Direction!=THOST_FTDC_D_Buy) {
				tOrderGain.fConditionPrice = reqGain.LimitPrice + fGainOffsetPrice;
			}
			else {
				tOrderGain.fConditionPrice = reqGain.LimitPrice - fGainOffsetPrice;
			}
			if(nCloseMode==1) {
				reqGain.LimitPrice = 
						(reqGain.Direction==THOST_FTDC_D_Buy ? 
                        tInstrfield.UpperLimitPrice : tInstrfield.LowerLimitPrice);
			}
			else {
				reqGain.LimitPrice = tOrderGain.fConditionPrice;
				reqGain.LimitPrice += 
						(reqGain.Direction==THOST_FTDC_D_Buy ? fCloseOffsetPrice : -fCloseOffsetPrice);
			}
			tOrderGain.nPosNum = tPosOrder.nNum;
			tOrderGain.nCondPosRelation = conCPRCondFromPos;
			tOrderGain.bHedge = FALSE;
			tOrderGain.nState = conCondPause;//conCondActive;
			tOrderGain.nPriceType = nConditionPriceType==1 ? nConditionPriceType : 
					(reqGain.Direction==THOST_FTDC_D_Buy ? 3:2);
			tOrderGain.nConditionType = (reqGain.Direction==THOST_FTDC_D_Buy ? 1:2);
			tOrderGain.nCreateTime = ::wxGetUTCTime();
			tOrderGain.req = reqGain;

			if(m_poOrderPositionPanel->GetIsRunInServer()) {
				reqGain.StopPrice = tOrderLose.fConditionPrice;
				reqGain.ContingentCondition = THOST_FTDC_CC_TouchProfit;

				
				int nRet = -1;
				wxString strRetMsg="";
                nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(tOrderGain.req, DEFAULT_SVR()->GetNextReqID()):-1;
				if(nRet!=0) {
					DataRspOrderInsert tInsertOrder;
					ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
					tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&reqGain);
					//设置错误信息
					strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
					ShowTradeInfoDlg("OIP_SOF", tInsertOrder, NeedOrderFailConfirm());
				}
			}
			else {
				if(m_poOrderServiceThread) {
					nGainCondNum = m_poOrderServiceThread->OrderConditionAdd(tOrderGain);
				}
			}
		}
		
		//COrderServiceThread::POSITIONORDER tPosOrder;
		//ZeroMemory(&tPosOrder, sizeof(COrderServiceThread::POSITIONORDER));

		//tPosOrder.bHedge = FALSE;
		//tPosOrder.bActive = TRUE;
		tPosOrder.nRegNum = req.RequestID;
		//tPosOrder.nOpenNum = 0;
		//tPosOrder.nRegState = 0;
		tPosOrder.nCondNum_StopGain = nGainCondNum;
		tPosOrder.nCondNum_StopLose = nLoseCondNum;
		tPosOrder.nPriceOffsetType = nOffsetPriceObject;
		tPosOrder.bDoStopLose = bIsCheckLose;
		tPosOrder.fStopLosePriceOffset = bIsCheckLose ? fLoseOffsetPrice : 0.0;
		tPosOrder.bDoStopGain = bIsCheckGain;
		tPosOrder.fStopGainPriceOffset = bIsCheckGain ? fGainOffsetPrice : 0.0;
		tPosOrder.nPriceType = nConditionPriceType;
		tPosOrder.nCloseMode = nCloseMode;
		tPosOrder.fClosePriceOffset = fCloseOffsetPrice;
		tPosOrder.bIsBuy = req.Direction==THOST_FTDC_D_Buy;
		tPosOrder.bIsMarket = req.OrderPriceType==THOST_FTDC_OPT_AnyPrice;
		tPosOrder.fRegPrice = req.LimitPrice;					//委托价
		tPosOrder.nRegVolume = req.VolumeTotalOriginal;			//委托数量
		tPosOrder.fOpenedPriceOffset = 0.0f;					//成交价偏移
		tPosOrder.fStopLosePrice = bIsCheckLose ? tOrderLose.fConditionPrice : 0.0;	//止损价
		tPosOrder.fStopGainPrice = bIsCheckGain ? tOrderGain.fConditionPrice : 0.0;	//止盈价
		tPosOrder.fStopLoseForecast=0.0f;						//止损预测
		tPosOrder.fStopGainForecast=0.0f;						//止盈预测
		strcpy(tPosOrder.InstrumentID, req.InstrumentID);
		m_poOrderServiceThread->OrderPositionSet(tPosOrder.nNum, tPosOrder);

		wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
		GETTOPWINDOW()->AddPendingEvent(evtnew);

		wxCommandEvent evtnew2(wxEVT_QUERY_STOP_REQUERY, GetId());
		GETTOPWINDOW()->AddPendingEvent(evtnew2);

	}

	if(bOK)
	{
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();

		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		::wxPostEvent(this,evt);
	}

}

void OrderInsertPanel::OnBtnStopOrder(wxCommandEvent& event)
{
	
	USERLOG_INFO("用户单击下单板[停止单按钮]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderStopPanel->GetUserInputText().c_str());

	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR", TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO", TRUE);
		return;
	}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	if(m_poOrderStopPanel->GetStopType()==-1) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_STOP_MODE", TRUE);
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	// 超过涨跌停价提示
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	COrderServiceThread::CONDITIONORDER tOrder={0};
	ZeroMemory(&tOrder, sizeof(COrderServiceThread::CONDITIONORDER));
	PlatformStru_InputOrder req;
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
	
	FillInputOrderField(req);
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	tOrder.bHedge = FALSE;
	tOrder.nState = conCondActive;
	//tOrder.nPriceType = m_poOrderStopPanel->GetPriceType();
	//tOrder.nConditionType = m_poOrderStopPanel->GetConditionType();
	tOrder.fConditionPrice = m_poOrderStopPanel->GetTriggerPrice();
	tOrder.nCreateTime = ::wxGetUTCTime();
	tOrder.req = req;

	wxString strMsg;
	double fInputPrice=0.0;
	BOOL bIsSuccess = FALSE;

	if(m_poOrderStopPanel->GetStopType()==1)
		strMsg.Printf(LOADFORMATSTRING(STOP_TYPE_FORMAT,"%s%s%s%s%s%d"), //文本必须包含%s %s %s %s %s %d 
		m_poOrderStopPanel->GetStopType()==1 ? LOADSTRING(ORDER_STOP_TYPE) : LOADSTRING(ORDER_STOPLIMIT_TYPE), 
		tOrder.req.InstrumentID, 
		Price2String(tOrder.fConditionPrice, m_currPriceTick).c_str(), 
		tOrder.req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
		tOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN) : (
		tOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
		tOrder.req.VolumeTotalOriginal);
	else
		strMsg.Printf(LOADFORMATSTRING(STOPLIMIT_TYPE_FORMAT,"%s%s%s%s%s%s%d"), //文本必须包含%s%s%s%s%s%s%d
		m_poOrderStopPanel->GetStopType()==1 ? LOADSTRING(ORDER_STOP_TYPE) : LOADSTRING(ORDER_STOPLIMIT_TYPE), 
		tOrder.req.InstrumentID, 
		Price2String(tOrder.fConditionPrice, m_currPriceTick).c_str(), 
		Price2String(m_poOrderStopPanel->GetPrice(), m_currPriceTick).c_str(), 
		tOrder.req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
		tOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN) : (
		tOrder.req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
		tOrder.req.VolumeTotalOriginal);


	BOOL bOK = TRUE;
	if(NeedConfirm()) {
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        strText += strMsg;
        strMsg = strText;
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_STOPCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("停止单提示选择，标题[%s]，\n用户选择[%s]\n",dialog.GetTitle().c_str(),bOK?"是":"否");
	}
	if(bOK) {
		//if(m_poOrderConditionPanel->GetIsRunInServer()) {
		if(1) {
			if(m_poOrderStopPanel->GetStopType()==1) {
				req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
				req.LimitPrice=0.0f;
				req.TimeCondition=THOST_FTDC_TC_IOC;

				req.StopPrice = tOrder.fConditionPrice;
				req.ContingentCondition = THOST_FTDC_CC_Touch;
			}
			else {
				req.LimitPrice = m_poOrderStopPanel->GetPrice();
				req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
				req.TimeCondition=THOST_FTDC_TC_GFD;

				req.StopPrice = tOrder.fConditionPrice;
				req.ContingentCondition = THOST_FTDC_CC_Touch;
			}
			
			int nRet = -1;
			wxString strRetMsg="";
            nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;
			if(GetInputAutoTrackPrice()) {
				SetInputAutoTrackPrice(NeedContinueTrackPrice());
			}
			if(nRet!=0) {
				//COrderServiceThread::sm_nReqNum--;
				DataRspOrderInsert tInsertOrder;
				ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
				//tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&req);
				strcpy(tInsertOrder.InputOrderField.InstrumentID, req.InstrumentID);
				tInsertOrder.InputOrderField.Direction = req.Direction;
				tInsertOrder.InputOrderField.CombOffsetFlag[0] = req.CombOffsetFlag[0];
				tInsertOrder.InputOrderField.CombHedgeFlag[0] = req.CombHedgeFlag[0];
				tInsertOrder.InputOrderField.LimitPrice = req.LimitPrice;
				tInsertOrder.InputOrderField.StopPrice = req.StopPrice;
				tInsertOrder.InputOrderField.VolumeTotalOriginal = req.VolumeTotalOriginal;				//设置错误信息
				strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
				ShowTradeInfoDlg("OIP_SOF", tInsertOrder, NeedOrderFailConfirm());
			}
		}
		else {
			if(m_poOrderServiceThread) {
				m_poOrderServiceThread->OrderConditionAdd(tOrder);

				wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
				GETTOPWINDOW()->AddPendingEvent(evtnew);
			}
		}
	}

}

void OrderInsertPanel::OnBtnPatsOrder(wxCommandEvent& event)
{
	
	USERLOG_INFO("用户单击下单板[Pats下单按钮]\n%s", 
			m_poPatsOrderInputPanel->GetUserInputText().c_str());

	if(DEFAULT_SVR()==NULL)
		return;

	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_CONTRERR), TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()!=NULL)
		DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	PlatformStru_InputOrder req;
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
	
	FillInputOrderField(req);
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	wxString strMsg;
	double fInputPrice=0.0;
	BOOL bIsSuccess = FALSE;

	strMsg = Printf_StdOrderReq(req, LOADSTRING(Printf_AccountTitle), LOADSTRING(Printf_OrderTypeTitle), 
			LOADSTRING(Printf_InstrumentIDTitle), LOADSTRING(Printf_DirectionTitle), 
			LOADSTRING(Printf_OpenCloseTitle), LOADSTRING(Printf_QtyTitle), 
			LOADSTRING(Printf_PriceTitle), LOADSTRING(Printf_TriggerPriceTitle), 
			LOADSTRING(Printf_HedgeTitle), m_currPriceTick, TRUE/*PLATFORM_SVR().HaveOrderType(UIOT_ACCOUNT)*/,
			DEFAULT_SVR()->HaveOrderType(UIOT_OPENCLOSE), DEFAULT_SVR()->HaveOrderType(UIOT_HEDGE));

	BOOL bOK = TRUE;
	if(NeedConfirm()) {
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_ORDERCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("Pats下单提示选择，标题[%s]，\n用户选择[%s]\n",dialog.GetTitle().c_str(),bOK?"是":"否");
	}
	if(bOK) {
		//if(m_poOrderConditionPanel->GetIsRunInServer()) {
		if(1) {			
			int nRet = -1;
			wxString strRetMsg="";
			nRet = DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID());
			if(nRet!=0) {
				DataRspOrderInsert tInsertOrder;
				ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
				tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&req);
				//设置错误信息
				strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,DEFAULT_SVR()->GetServerParamCfg().LanguageId).c_str());
				ShowTradeInfoDlg(LOADSTRING(OIP_SOF), tInsertOrder, NeedOrderFailConfirm());
			}
		}
		else {
			//if(m_poOrderServiceThread) {
			//	m_poOrderServiceThread->OrderConditionAdd(tOrder);

			//	wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
			//	GETTOPWINDOW()->AddPendingEvent(evtnew);
			//}
		}
	}

}

void OrderInsertPanel::OnFivePriceSelected(wxCommandEvent& event)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
    wxString strText(EvtParamStr.c_str());
	SetInputPriceText(strText);
}

void OrderInsertPanel::OnCheckDoStopClicked(wxCommandEvent& event)
{
	
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;

	vector<VarietyInfo>* parrVarietyInfo;
	parrVarietyInfo = pMgr->GetVarietyInfoVector();
	VarietyInfo tvInfo;
	wxString strCode = GetInputInstrumentID();
	double fGainOffset = 0.0;
	double fLoseOffset = 0.0;
	double fReverseOffset = 0.0;
	BOOL bFound = FALSE;

	if(event.GetInt()) {
		for(int i=0; i<(int)parrVarietyInfo->size(); i++) {
			tvInfo = (*parrVarietyInfo)[i];
			wxString strValue = tvInfo.InstruName;
			if(strCode.compare(strValue.c_str()) == 0 
						|| strnicmp(strValue.c_str(), strCode.c_str(), strValue.size())==0) {
				fLoseOffset = atof(tvInfo.loss);
				fGainOffset = atof(tvInfo.profit);
				fReverseOffset = atof(tvInfo.reverse);
				bFound = TRUE;
				break;
			}
		}
	}
	if(bFound) {
		m_poOrderPositionPanel->SetLoseOffsetPrice(event.GetInt(), fLoseOffset*m_currPriceTick);
		m_poOrderPositionPanel->SetGainOffsetPrice(event.GetInt(), fGainOffset*m_currPriceTick);
		m_poOrderPositionPanel->SetCloseOffsetPrice(fReverseOffset*m_currPriceTick);
	}
	else {
		BOOL bIsCheckLose = FALSE, bIsCheckGain = FALSE;
		double fLoseOffsetPrice = 0.0, fGainOffsetPrice = 0.0;

		bIsCheckLose = m_poOrderPositionPanel->GetLoseOffsetPrice(fLoseOffsetPrice);
		bIsCheckGain = m_poOrderPositionPanel->GetGainOffsetPrice(fGainOffsetPrice);

		m_poOrderPositionPanel->SetLoseOffsetPrice(event.GetInt(), fLoseOffsetPrice);
		m_poOrderPositionPanel->SetGainOffsetPrice(event.GetInt(), fGainOffsetPrice);
		//m_poOrderPositionPanel->SetCloseOffsetPrice(0.0);
	}
	
	if(event.GetInt()) {
		for(int i=0; i<(int)m_TabOrderVector.size(); i++) {
			if(m_TabOrderVector[i]==ID_BUTTON_OK) {
				m_TabOrderVector[i]=ID_BUTTON_POSITIONORDER;
				break;
			}
		}
	}
	else {
		for(int i=0; i<(int)m_TabOrderVector.size(); i++) {
			if(m_TabOrderVector[i]==ID_BUTTON_POSITIONORDER) {
				m_TabOrderVector[i]=ID_BUTTON_OK;
				break;
			}
		}
	}
	
	m_poOrderInsertCommandPanel->ViewStyleSwitch_DoStop(event.GetInt());
	if(event.GetInt()) {
		m_poOrderInputPanel->SetViewMode(3);
	}
	else {
		m_poOrderInputPanel->SetViewMode(0);
	}
}

void OrderInsertPanel::OnMAPlatformAdd(wxCommandEvent& evt)
{
	TRACE("OrderInsertPanel::OnMAPlatformAdd(wxCommandEvent& evt)\n");
	m_pSvrSet.insert((IPlatformSingleSvr*)evt.GetInt());
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
}
void OrderInsertPanel::OnMAPlatformDelete(wxCommandEvent& evt)
{
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(0);
	ProcessEvent(myEvent);
	m_pSvrSet.erase((IPlatformSingleSvr*)evt.GetInt());
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
}
void OrderInsertPanel::OnInitSubscribe(wxCommandEvent& evt)
{
	set<IPlatformSingleSvr*>::iterator it;
	if(evt.GetInt())
	{
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			//定制业务信息
			if(*it) (*it)->SubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
			if(*it) (*it)->SubscribeBusinessData(BID_RtnOrder, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
			if(*it) (*it)->SubscribeBusinessData(BID_RspOrderAction1, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
			if(*it) (*it)->SubscribeBusinessData(BID_RspOrderAction2, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
			if(*it) (*it)->SubscribeBusinessData(BID_RtnTrade, GID_ORDER_INSERT_PANEL, TradeCallBackFunc);	//订阅成交回报
		}
	}	
  	else {
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			//取消定制
			if(*it) (*it)->UnSubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_INSERT_PANEL);
			if(*it) (*it)->UnSubscribeBusinessData(BID_RtnOrder, GID_ORDER_INSERT_PANEL);
			if(*it) (*it)->UnSubscribeBusinessData(BID_RspOrderAction1, GID_ORDER_INSERT_PANEL);
			if(*it) (*it)->UnSubscribeBusinessData(BID_RspOrderAction2, GID_ORDER_INSERT_PANEL);
			if(*it) (*it)->UnSubscribeBusinessData(BID_RtnTrade, GID_ORDER_INSERT_PANEL);	//订阅成交回报
		}
	}
}

void OrderInsertPanel::OnCfgChanged(wxCommandEvent& evt)
{
	wxCommandEvent MyEvent(wxEVT_GET_STYLE);
	((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
    int nViewStyle=MyEvent.GetInt();
    void *p=MyEvent.GetClientData();
    SAFE_DELETE(p);
	m_poOrderInputPanel->DynamicShowCtrl();
	m_poOrderInputPanel->SetViewStyle(nViewStyle);
    if(nViewStyle!=m_nViewStyle) { 
        //SetViewStyle(-1, 0);
        LoadStyleUI(nViewStyle, 0);
        SetViewStyle(nViewStyle, 0);
		//SetSellBuyColor();
    }
	if(evt.GetInt()==TYPE_APPLYDEFAULT)//恢复默认设置时重新设置手数倍率
	{
		m_nMultiplyVolume = 1;
		m_bUpdateRealMsg_Multiply = true;
	}
	//ShowViewSubStyle(0);
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr != NULL) {
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// 设置界面上的自动开平按钮
		//待处理，默认自动开平
		SetInputAutoOpenClose(p->bAutoOpenClose);

		// 这里从xml中读下单倍率数据
		m_KeyMultiplyVolumeMap.clear();
		for(int i=0; i<p->OrderMultiDataNum; i++) {
			std::string strKey(p->OrderMultiData[i].ShortKey);
			m_KeyMultiplyVolumeMap[strKey] = p->OrderMultiData[i].Volume;
		}
	}

	wxCheckBox* itemCheckWDShow = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW, this);
	if(itemCheckWDShow!=NULL) {
		wxCommandEvent evttemp(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_CHECK_WDSHOW);
		if(evt.GetInt()!=TYPE_APPLYINIT &&  evt.GetEventType()==wxEVT_STYLE_CHANGED)//恢复默认设置时重新设置手数倍率
		{
			itemCheckWDShow->SetValue(false);
			evttemp.SetInt(0);
			OnCheckWDShowClicked(evttemp);
		}
		else {
			itemCheckWDShow->SetValue(NeedFivePriceList()?true:false);
			evttemp.SetInt(NeedFivePriceList());
			OnCheckWDShowClicked(evttemp);
		}
	}
	evt.Skip();
}

void OrderInsertPanel::OnPanelFocus(wxCommandEvent& evt)
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
	if(pWin!=NULL) pWin->SetFocus();
}

void OrderInsertPanel::OnCMDGridInstrumentIDChanged(wxCommandEvent& evt)
{
	TRACE("OrderInsertPanel::OnCMDGridInstrumentIDChanged(wxCommandEvent& evt)\n");

	std::string EvtParamStr;
    int EvtUserInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtUserInt))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

	// 合约不变，不往下走
	if(EvtParamStr==GetInputInstrumentID().c_str())
		return;

    if(EvtUserInt==0) {
		m_bNoKeyChangeText = FALSE;
		m_bIsGridChangeInstrumentID = TRUE;
        SetInputInstrumentID(wxString(EvtParamStr.c_str()));
		m_bIsGridChangeInstrumentID = FALSE;
		m_bNoKeyChangeText = TRUE;
	}
	else {
//		m_bNoKeyChangeText = FALSE;
//		SetInputInstrumentID(evt.GetString());
//		m_bNoKeyChangeText = TRUE;
	}
}

void OrderInsertPanel::OnCMDViewModeChanged(wxCommandEvent& evt)
{
	int nViewStyle = 0, nViewSubStyle = 0;
	nViewStyle = evt.GetInt()/256;
	nViewSubStyle = evt.GetInt()%256;
	ShowViewStyle(nViewStyle, nViewSubStyle);
}

void OrderInsertPanel::OnCMDKeyOrder(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;

	KeyOrderInfo keyOrder;
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        bool brlt=false;
        int EvtPtrLen;
        if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,NULL,&EvtPtrLen,0,NULL)&&
            EvtPtrLen==sizeof(KeyOrderInfo))
        {
            CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,&keyOrder,&EvtPtrLen,EvtPtrLen,NULL);
            CEventParam::DeleteEventParam(EvtParamID);
            brlt=true;
        }
        if(!brlt)
        {
            return;
        }
    }
	wxString strInstrumentID(EvtParamStr);
	USERLOG_INFO("快捷键快速下单，合约[%s]，快捷键[%s]，方向[%d]，价格[%d]，超价[%d]，活动[%d]，作用区域[%d]\n", 
			evt.GetString(), 
			keyOrder.ShortKey,
			keyOrder.nBuySell,
			keyOrder.nPrice,
			keyOrder.nOverPrice,
			keyOrder.nAction,
			keyOrder.nActionArea);


	if(DEFAULT_SVR()->IsMultipleAccounts())
	{	// 多帐号下单过程

		wxString strCode(EvtParamStr);
		if ( keyOrder.nAction == 0)
			return;

		int nAction = keyOrder.nAction;

		// 当键盘快速下单时，自动功能优先于盈损单状态
		if(m_nViewSubStyle==conSubStyle_Position) {
			SetStopPositionSelect(FALSE);
			m_poOrderPositionPanel->InitPositionPanel();
			wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
			evt.SetInt(0);
			ProcessEvent(evt);
		}

		// 这里从xml中读是否要确认的配置
		CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
		LPORDER_CFG pOrderCfg = NULL;
		if(pMgr == NULL) return;
		pOrderCfg = pMgr->GetOrderCfg();

		set<string> InstrumentIDArray;
		InstrumentIDArray.clear();
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
		if(pSvr!=NULL) pSvr->GetInstrumentList(InstrumentIDArray);
		//if(InstrumentIDArray.find(strCode.c_str())==InstrumentIDArray.end()) {
		//	ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
		//	return;
		//}

		// 首先把合约代码写到界面上
		m_bNoKeyChangeText = FALSE;
		m_bIsGridChangeInstrumentID = TRUE;
		SetInputInstrumentID(strCode);
		m_bIsGridChangeInstrumentID = FALSE;
		m_bNoKeyChangeText = TRUE;

		// 设置手数
		int nDefaultVolume = GetDefaultVolume(strCode);
		SetInputOrderType(OrderType_PATS_LIMIT);

		double fPrice = 0.0;
		int nOverPrice = 0;
		BOOL bIsBuy = FALSE;
		double fPriceTick = 0.0;

		PlatformStru_DepthMarketData field;
		ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

		// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
		if(pSvr) pSvr->GetQuotInfo(strCode.c_str(),field);

		// 获取市价
		nOverPrice = keyOrder.nOverPrice;
		if( keyOrder.nPrice == 0) {
			fPrice = field.LastPrice;
		}
		else if( keyOrder.nPrice == 1) {
			fPrice = field.BidPrice1;
		}
		else if( keyOrder.nPrice == 2) {
			fPrice = field.AskPrice1;
		}

		fPriceTick=m_currPriceTick;
		if(keyOrder.nBuySell==0) {
			bIsBuy = TRUE;
			if(!util::isInvalidValue(fPrice)) {
				fPrice += (nOverPrice*fPriceTick);
			}
		}
		else {
			bIsBuy = FALSE;
			if(!util::isInvalidValue(fPrice)) {
				fPrice -= (nOverPrice*fPriceTick);
			}
		}
		
		OrderInsertUIItem orderUIItem;
		COrderAtomEntity::STDORDERREQ stdReq;
		CSAOrderEntity::OPERPARAM operParam;
		CMAOrderEntity::MAAUTOOPER maOper;

		ZeroMemory(&orderUIItem, sizeof(orderUIItem));
		ZeroMemory(&stdReq, sizeof(stdReq));
		ZeroMemory(&operParam, sizeof(operParam));
		ZeroMemory(&maOper, sizeof(maOper));

		maOper.nOperType = NeedAlwaysOpen() ? CSAOrderEntity::conOperNoneAuto : CSAOrderEntity::conOperAuto;
		maOper.bIsQuotOrder = TRUE;
		maOper.bNeedCancel = pOrderCfg->nCancelSameDirOpenOrderBeforeOrder;
		maOper.nMouseNotReversed = pOrderCfg->nAllNotHaveOppositePosition;
		maOper.nMousePartReversed = pOrderCfg->nSomeHaveOppositePosition;
		maOper.nMouseAllReversed = pOrderCfg->nAllHaveOppositePosition;
		maOper.nPositionClose = pOrderCfg->nMouseClosePositionStrategy;
	
		//operParam.nOperType = CSAOrderEntity::conOperNoneAuto;				// 执行类型，0 非自动单，1 自动单，2 非自动改单
		//operParam.nAutoMode = CMAOrderEntity::conOpenDefaultAndStrategy;	// 自动单模式，0 按持仓量全平，1 按默认手数平仓，不反向开仓
		//operParam.bNeedCancel = FALSE;										// 是否需要撤销相关报单
		//operParam.bIsCanClose = FALSE;										// 是否按可平仓数处理
		operParam.bSimulate = FALSE;											// 是否进行市价模拟
		operParam.nDefaultVolume = nDefaultVolume;							// 默认手数
		operParam.bNoSendVolZoreOrder = TRUE;
		
		stdReq.nOrderType = OrderType_FT_Immediately;
		strcpy_s(stdReq.csInstrumentID, 
				sizeof(stdReq.csInstrumentID)-1, strCode.c_str());		// 合约
		strcpy_s(stdReq.csUserID, sizeof(stdReq.csUserID)-1, 
				GetInputAccount().c_str());									// 投资者ID或Pats的用户名
		//strcpy_s(stdReq.csAccount, 
		//		sizeof(stdReq.csAccount)-1, orderUIItem.Account);			// 帐号
		stdReq.bIsBuy = bIsBuy;
		stdReq.nOCMode = conOCUnknow;										// 开平仓模式, 0 开仓，1 平今，2 平仓
		stdReq.bIsMarket = FALSE;											// 是否为市价
		stdReq.fPrice = fPrice;
		stdReq.nVolume = nDefaultVolume;
		stdReq.bIsHedge = orderUIItem.IsHedge;

		if(nAction<=2) 
		{
			BOOL bHasSelNoLogon = FALSE;
			map<string,bool> accsLogon;
			int nRet = GetLogonAccounts(accsLogon);
			if(accsLogon.size()==0) {
				wxMessageDialog dialog(NULL,LOADSTRING(请勾选已登录帐号进行下单),
						LOADSTRING(错误),wxOK|wxICON_ERROR);
				dialog.ShowModal();
				return;
			}
			bHasSelNoLogon = (nRet>0);
			if(bHasSelNoLogon) {
				wxMessageDialog dialog(NULL,LOADSTRING(勾选的账号中包含未登录的，\n未登录帐号不参与任何计算且不显示\n选择“是(Y)”继续下单，\n选择“否(N)”不下单),
						LOADSTRING(警告),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
				if(dialog.ShowModal()!=wxID_YES)
					return;
			}
			DoMaOrderInsert(operParam, stdReq, maOper, accsLogon);
		}

		AsyncViewData(stdReq.bIsBuy, stdReq.nOCMode, nDefaultVolume, stdReq.fPrice, nAction);
	}
	else {
//	if(evt.GetClientData()==NULL) return;

//	KeyOrderInfo keyOrder = *((KeyOrderInfo*)evt.GetClientData());
//	try {
//	delete evt.GetClientData();
//	}
//	catch(...) {};
	//wxString strInstrumentID(EvtParamStr);
	//USERLOG_INFO("快捷键快速下单，合约[%s]，快捷键[%s]，方向[%d]，价格[%d]，超价[%d]，活动[%d]，作用区域[%d]\n", 
	//		evt.GetString(), 
	//		keyOrder.ShortKey,
	//		keyOrder.nBuySell,
	//		keyOrder.nPrice,
	//		keyOrder.nOverPrice,
	//		keyOrder.nAction,
	//		keyOrder.nActionArea);
		DoKeyOrder(strInstrumentID, keyOrder);
		m_bUpdateRealMsg_PreClose = true;
	}
}

void OrderInsertPanel::OnCMDMouseOrder(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	//LOG_INFO("传入用户鼠标快速下单\n");
	//SetFocus();

	TRACE("OrderInsertPanel::OnCMDMouseOrder(wxCommandEvent& evt)\n");

    std::string EvtParamStr;
    int EvtUserInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtUserInt))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	LPORDER_CFG pOrderCfg = NULL;
	if(pMgr == NULL) return;
	pOrderCfg = pMgr->GetOrderCfg();
	int nAction = 0;

	switch(EvtUserInt) {
	case 1:
		nAction = pOrderCfg->ClickQuotTableAction;
		break;
	case 2:
		nAction = pOrderCfg->DoubleClickQuotTableAction;
		break;
	default:
		evt.Skip();
		return;
		break;
	}
	USERLOG_INFO("鼠标快速下单，合约[%s]，方向[%c]，默认手数[%d]，活动[%d]\n", 
            EvtParamStr.c_str(), 
			evt.GetExtraLong(),
            GetDefaultVolume(wxString(EvtParamStr.c_str())),
			nAction);
	if(nAction==0)	// 在鼠标快速开仓中，Action==0表示无活动，应立即返回
		return;

    wxString strCode(EvtParamStr.c_str());				// 获得转入的合约代码
	int nDefaultVolume = GetDefaultVolume(strCode);	// 获得合约的默认量
	double fPrice = 0.0;
	BOOL bIsBuy = FALSE;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	if(DEFAULT_SVR()->IsMultipleAccounts())
	{	
		// 多帐号下单过程

		// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
		IPlatformSingleSvr* pSvr=NULL;
			pSvr = g_pPlatformMgr->GetCurrentPlatformSvr();
		if(pSvr) pSvr->GetQuotInfo(strCode.c_str(),field);
		
		OrderInsertUIItem orderUIItem;
		COrderAtomEntity::STDORDERREQ stdReq;
		CSAOrderEntity::OPERPARAM operParam;
		CMAOrderEntity::MAAUTOOPER maOper;

		ZeroMemory(&orderUIItem, sizeof(orderUIItem));
		ZeroMemory(&stdReq, sizeof(stdReq));
		ZeroMemory(&operParam, sizeof(operParam));
		ZeroMemory(&maOper, sizeof(maOper));

		maOper.nOperType = NeedAlwaysOpen() ? CSAOrderEntity::conOperNoneAuto : CSAOrderEntity::conOperAuto;
		maOper.bIsQuotOrder = TRUE;
		maOper.bNeedCancel = pOrderCfg->nCancelSameDirOpenOrderBeforeOrder;
		maOper.nMouseNotReversed = pOrderCfg->nAllNotHaveOppositePosition;
		maOper.nMousePartReversed = pOrderCfg->nSomeHaveOppositePosition;
		maOper.nMouseAllReversed = pOrderCfg->nAllHaveOppositePosition;
		maOper.nPositionClose = pOrderCfg->nMouseClosePositionStrategy;
		
		if(evt.GetExtraLong()==THOST_FTDC_D_Buy) {
			fPrice = field.BidPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? TRUE : FALSE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
		}
		else {
			fPrice = field.AskPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? FALSE : TRUE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
		}

		//operParam.nOperType = CSAOrderEntity::conOperNoneAuto;				// 执行类型，0 非自动单，1 自动单，2 非自动改单
		//operParam.nAutoMode = CMAOrderEntity::conOpenDefaultAndStrategy;	// 自动单模式，0 按持仓量全平，1 按默认手数平仓，不反向开仓
		//operParam.bNeedCancel = FALSE;										// 是否需要撤销相关报单
		//operParam.bIsCanClose = FALSE;										// 是否按可平仓数处理
		operParam.bSimulate = FALSE;											// 是否进行市价模拟
		operParam.nDefaultVolume = nDefaultVolume;							// 默认手数
		operParam.bNoSendVolZoreOrder = TRUE;
		
		stdReq.nOrderType = OrderType_FT_Immediately;
		strcpy_s(stdReq.csInstrumentID, 
				sizeof(stdReq.csInstrumentID)-1, GetInputInstrumentID().c_str());		// 合约
		strcpy_s(stdReq.csUserID, sizeof(stdReq.csUserID)-1, 
				GetInputAccount().c_str());									// 投资者ID或Pats的用户名
		//strcpy_s(stdReq.csAccount, 
		//		sizeof(stdReq.csAccount)-1, orderUIItem.Account);			// 帐号
		stdReq.bIsBuy = bIsBuy;
		stdReq.nOCMode = conOCUnknow;												// 开平仓模式, 0 开仓，1 平今，2 平仓
		stdReq.bIsMarket = FALSE;											// 是否为市价
		stdReq.fPrice = fPrice;
		stdReq.nVolume = nDefaultVolume;
		stdReq.bIsHedge = orderUIItem.IsHedge;

		if(nAction<=2) 
		{
			BOOL bHasSelNoLogon = FALSE;
			map<string,bool> accsLogon;
			int nRet = GetLogonAccounts(accsLogon);
			if(accsLogon.size()==0) {
				wxMessageDialog dialog(NULL,LOADSTRING(请勾选已登录帐号进行下单),
						LOADSTRING(错误),wxOK|wxICON_ERROR);
				dialog.ShowModal();
				return;
			}
			bHasSelNoLogon = (nRet>0);
			if(bHasSelNoLogon) {
				wxMessageDialog dialog(NULL,LOADSTRING(勾选的账号中包含未登录的，\n未登录帐号不参与任何计算且不显示\n选择“是(Y)”继续下单，\n选择“否(N)”不下单),
						LOADSTRING(警告),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
				if(dialog.ShowModal()!=wxID_YES)
					return;
			}
			DoMaOrderInsert(operParam, stdReq, maOper, accsLogon);
		}

		AsyncViewData(stdReq.bIsBuy, stdReq.nOCMode, nDefaultVolume, stdReq.fPrice, nAction);
	}
	else
	{
		// 单帐号下单过程


		//// 这里从xml中读是否要确认的配置
		//CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
		//LPORDER_CFG pOrderCfg = NULL;
		//if(pMgr == NULL) return;
		//pOrderCfg = pMgr->GetOrderCfg();
		//
		//int nAction = 0;
		//switch(EvtUserInt) {
		//case 1:
		//	nAction = pOrderCfg->ClickQuotTableAction;
		//	break;
		//case 2:
		//	nAction = pOrderCfg->DoubleClickQuotTableAction;
		//	break;
		//default:
		//	evt.Skip();
		//	return;
		//	break;
		//}
		//USERLOG_INFO("鼠标快速下单，合约[%s]，方向[%c]，默认手数[%d]，活动[%d]\n", 
	 //           EvtParamStr.c_str(), 
		//		evt.GetExtraLong(),
	 //           GetDefaultVolume(wxString(EvtParamStr.c_str())),
		//		nAction);
		//if(nAction==0)	// 在鼠标快速开仓中，Action==0表示无活动，应立即返回
		//	return;
		////BOOL bIsAutoOpenClose = GetInputAutoOpenClose();
		////

		// 当鼠标快速平仓时，自动功能优先于盈损单状态
		if(m_nViewSubStyle==conSubStyle_Position) {
			SetStopPositionSelect(FALSE);
			m_poOrderPositionPanel->InitPositionPanel();
			wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
			evt.SetInt(0);
			ProcessEvent(evt);
		}

	 //   wxString strCode(EvtParamStr.c_str());				// 获得转入的合约代码
		//int nDefaultVolume = GetDefaultVolume(strCode);	// 获得合约的默认量
		//double fPrice = 0.0;
		//BOOL bDirect = FALSE;

		////m_bNoKeyChangeText = FALSE;
		////SetInputInstrumentID(strCode);
		////m_bNoKeyChangeText = TRUE;

		//PlatformStru_DepthMarketData field;
		//ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

		// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
		IPlatformSingleSvr* pSvr=DEFAULT_SVR();
		if(pSvr) pSvr->GetQuotInfo(strCode.c_str(),field);

		// 处理买卖方向及价格
		// pOrderCfg->MouseFastOrderDirection, 1 同向, 0 反向
		if(evt.GetExtraLong()==THOST_FTDC_D_Buy) {
			fPrice = field.BidPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? TRUE : FALSE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			SetInputDirection(bIsBuy);
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
			SetInputPrice(fPrice);
		}
		else if(evt.GetExtraLong()==THOST_FTDC_D_Sell) {
			fPrice = field.AskPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? FALSE : TRUE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			SetInputDirection(bIsBuy);
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
			SetInputPrice(fPrice);
		}
		else {
			if(GetInputDirection()) {
				fPrice = field.AskPrice1;// + pOrderCfg->PositionVolume*m_DynamicItem.PriceTick;
				//fPrice = AnalysePrice(fPrice);
				SetInputPrice(fPrice);
			}
			else {
				fPrice = field.BidPrice1;// - pOrderCfg->PositionVolume*m_DynamicItem.PriceTick;
				//fPrice = AnalysePrice(fPrice);
				SetInputPrice(fPrice);
			}
		}

		SetInputOrderType(OrderType_PATS_LIMIT);

		// 读取方向
		BOOL bDirection = GetInputDirection();

		COrderInsertEntity::ORIORDERREQ tOriReq={0};

		//获取是否自动
		BOOL bIsAuto = TRUE;
		NeedAutoOpenClose(tOriReq.nAutoMode);

		int nVolume = nDefaultVolume;
		int nSumVolume = 0, nSumTodayVolume=0;

	//	// 处理自动开平仓设置
		if(bIsAuto) 
		{
	//		SetInputAutoOpenClose(TRUE);
			BOOL bIsBuy = GetInputDirection();
			BOOL bIsHedge = GetInputHedge();
			// 可平数 总仓/今仓
			//COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
			if(COrderInsertEntity::GetHoldingVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()))) {
				nVolume = nSumVolume;
			}
			//if(nSumVolume-nSumTodayVolume>0) {
			//	SetInputOCMode(2);
			//	//SetInputQty(nSumTodayVolume);
			//}
			//else if(nSumTodayVolume>0){
			//	SetInputOCMode(1);
			//	//SetInputQty(nSumVolume-nSumTodayVolume);
			//}
			//else {
			//	SetInputOCMode(0);
			//	//SetInputQty(GetDefaultVolume(strCode));
			//}
		}

		if(NeedAlwaysOpen() || nSumVolume==0) 
		{
			nVolume = nDefaultVolume;
			//SetInputQty(nDefaultVolume);
			//SetInputOCMode(0);
			bIsAuto = FALSE;
		}
		else 
		{
			//if(tOriReq.nAutoMode==1)
			//	SetInputQty(nSumVolume);
			//else 
			//	SetInputQty(nDefaultVolume);
		}

		// 读取开平仓
	//	int nOCMode = GetInputOCMode();

	//	BOOL bHedge = GetInputHedge();

		wxString strAccount=GetInputAccount();

		strcpy(tOriReq.strInstrumentID, strCode.c_str());
		strcpy(tOriReq.strAccount, strAccount.c_str());
		tOriReq.bIsParketOrder = FALSE;
		tOriReq.bDirection = bDirection;
		tOriReq.nOCMode = 0;//nOCMode;
		tOriReq.fPrice = fPrice;
		tOriReq.bIsMarketPrice = FALSE;
		tOriReq.bSimulate = NeedSimulateMarketPrice();
		tOriReq.nVolume = nVolume;
		tOriReq.nDefaultVolume = nDefaultVolume;		// 在OK按钮下，使用输入手数作为默认手数
		tOriReq.bIsAuto = bIsAuto;
		NeedAutoOpenClose(tOriReq.nAutoMode);
		tOriReq.bIsHedge = FALSE;

		BOOL bHasOrderInsert = FALSE;
		DoOrderInsert(tOriReq, nAction, bHasOrderInsert, TRUE);
		m_bUpdateRealMsg_PreClose = true;
	}
}

void OrderInsertPanel::OnCMDMouseClose(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	//LOG_INFO("传入用户鼠标快速平仓\n");
	//SetFocus();

	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	LPORDER_CFG pOrderCfg = NULL;
	if(pMgr == NULL) return;
	pOrderCfg = pMgr->GetOrderCfg();

	PlatformStru_Position* pField=(PlatformStru_Position*)evt.GetClientData();
	
	int nAction = 0;
	switch(evt.GetInt()) {
	case 1:
		nAction = pOrderCfg->ClickPositionAction;
		break;
	case 2:
		nAction = pOrderCfg->DoubleClickPositionAction;
		break;
	case 3:
		nAction = NeedConfirm() ? 1 : 2;
		break;
	default:
		delete pField;
		return;
		break;
	}
	if(!nAction)	// 在鼠标快速平仓中，Action==0表示无活动，应立即返回
	{
		delete pField;
		return;
	}
	USERLOG_INFO("鼠标快速平仓，合约[%s]，方向[%c]，持仓量[%d]，活动[%d]\n", 
			pField->InstrumentID, 
			pField->PosiDirection,
			pField->Position,
			nAction);

	wxString strAccount;
	wxString strCode(pField->InstrumentID);				// 获得转入的合约代码
	int nDefaultVolume = GetDefaultVolume(strCode);		// 获得合约的默认量
	double fPrice = 0.0;
	BOOL bIsBuy = FALSE;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	if(DEFAULT_SVR()->IsMultipleAccounts())
	{	// 多帐号下单过程

		// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
		if(pSvr) pSvr->GetQuotInfo(strCode.c_str(),field);
	
		OrderInsertUIItem orderUIItem;
		COrderAtomEntity::STDORDERREQ stdReq;
		CSAOrderEntity::OPERPARAM operParam;
		CMAOrderEntity::MAAUTOOPER maOper;

		ZeroMemory(&orderUIItem, sizeof(orderUIItem));
		ZeroMemory(&stdReq, sizeof(stdReq));
		ZeroMemory(&operParam, sizeof(operParam));
		ZeroMemory(&maOper, sizeof(maOper));

		maOper.nOperType = CSAOrderEntity::conOperAuto;
		maOper.bIsQuotOrder = FALSE;
		maOper.bNeedCancel = pOrderCfg->nCancelSameDirOpenOrderBeforeOrder;
		maOper.nMouseNotReversed = pOrderCfg->nAllNotHaveOppositePosition;
		maOper.nMousePartReversed = pOrderCfg->nSomeHaveOppositePosition;
		maOper.nMouseAllReversed = pOrderCfg->nAllHaveOppositePosition;
		maOper.nPositionClose = pOrderCfg->nMouseClosePositionStrategy;
		
		bIsBuy = pField->PosiDirection==THOST_FTDC_PD_Long;
		SetInputInstrumentID(strCode);
		if(bIsBuy) {
			fPrice = field.BidPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? TRUE : FALSE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			SetInputDirection(bIsBuy);
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
			SetInputPrice(fPrice);
		}
		else {
			fPrice = field.AskPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? FALSE : TRUE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			SetInputDirection(bIsBuy);
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
			SetInputPrice(fPrice);
		}
		SetInputQty(pField->Position);

		//operParam.nOperType = CSAOrderEntity::conOperNoneAuto;				// 执行类型，0 非自动单，1 自动单，2 非自动改单
		//operParam.nAutoMode = CMAOrderEntity::conOpenDefaultAndStrategy;	// 自动单模式，0 按持仓量全平，1 按默认手数平仓，不反向开仓
		//operParam.bNeedCancel = FALSE;										// 是否需要撤销相关报单
		//operParam.bIsCanClose = FALSE;										// 是否按可平仓数处理
		operParam.bSimulate = FALSE;											// 是否进行市价模拟
		operParam.nDefaultVolume = nDefaultVolume;							// 默认手数
		operParam.bNoSendVolZoreOrder = TRUE;
		
		stdReq.nOrderType = OrderType_FT_Immediately;
		strcpy_s(stdReq.csInstrumentID, 
				sizeof(stdReq.csInstrumentID)-1, GetInputInstrumentID().c_str());		// 合约
		strcpy_s(stdReq.csUserID, sizeof(stdReq.csUserID)-1, 
				pField->InvestorID);									// 投资者ID或Pats的用户名
		//strcpy_s(stdReq.csAccount, 
		//		sizeof(stdReq.csAccount)-1, orderUIItem.Account);			// 帐号
		stdReq.bIsBuy = bIsBuy;
		stdReq.nOCMode = conOCUnknow;												// 开平仓模式, 0 开仓，1 平今，2 平仓
		stdReq.bIsMarket = FALSE;											// 是否为市价
		stdReq.fPrice = fPrice;
		stdReq.nVolume = nDefaultVolume;
		stdReq.bIsHedge = orderUIItem.IsHedge;

		if(nAction<=2)
		{
			map<string,bool> accsLogon;
			accsLogon.insert(std::pair<string,bool>(string(pField->InvestorID), true));
			DoMaOrderInsert(operParam, stdReq, maOper, accsLogon);
		}

		AsyncViewData(stdReq.bIsBuy, stdReq.nOCMode, nDefaultVolume, stdReq.fPrice, nAction);
	}
	else 
	{
		// 单帐号下单过程

	//// 这里从xml中读是否要确认的配置
	//CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	//LPORDER_CFG pOrderCfg = NULL;
	//if(pMgr == NULL) return;
	//pOrderCfg = pMgr->GetOrderCfg();
	//wxString strAccount;

	//PlatformStru_Position* pField=(PlatformStru_Position*)evt.GetClientData();
	//
	//int nAction = 0;
	//switch(evt.GetInt()) {
	//case 1:
	//	nAction = pOrderCfg->ClickPositionAction;
	//	break;
	//case 2:
	//	nAction = pOrderCfg->DoubleClickPositionAction;
	//	break;
	//case 3:
	//	nAction = NeedConfirm() ? 1 : 2;
	//	break;
	//default:
	//	delete pField;
	//	return;
	//	break;
	//}
	//if(!nAction)	// 在鼠标快速平仓中，Action==0表示无活动，应立即返回
	//	return;
	////BOOL bIsAutoOpenClose = GetInputAutoOpenClose();
	////

	// 当鼠标快速平仓时，自动功能优先于盈损单状态
	if(m_nViewSubStyle==conSubStyle_Position) {
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		ProcessEvent(evt);
	}

	//wxString strCode = pField->InstrumentID;				// 获得转入的合约代码
	//int nDefaultVolume = GetDefaultVolume(strCode);			// 获得合约的默认量
	//double fPrice = 0.0;

	//PlatformStru_DepthMarketData field;
	//ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strCode.c_str(),field);

	int nSumVolume=0, nSumTodayVolume=0;
	COrderInsertEntity::GetHoldingVolume(strCode.c_str(), 
			pField->PosiDirection==THOST_FTDC_PD_Long, pField->HedgeFlag==THOST_FTDC_HF_Hedge, 
			nSumVolume, nSumTodayVolume, string(pField->szAccount));

//	// 读取手数
	int nVolume = pField->Position;
//	// 读取方向
	BOOL bDirection = pField->PosiDirection!=THOST_FTDC_PD_Long;
//	// 读取开平仓
	int nOCMode = nSumVolume>nSumTodayVolume?2:1;
	BOOL bHedge = pField->HedgeFlag==THOST_FTDC_HF_Hedge;

	m_bNoKeyChangeText = FALSE;
	m_bIsGridChangeInstrumentID = TRUE;
	SetInputInstrumentID(strCode);
	m_bIsGridChangeInstrumentID = FALSE;
	m_bNoKeyChangeText = TRUE;
	strAccount = pField->szAccount;
	SetInputAccount(strAccount);
	//SetInputDirection(pField->PosiDirection!=THOST_FTDC_PD_Long);
	//SetInputOCMode(nSumVolume>nSumTodayVolume?2:1);
	//SetInputHedge(pField->HedgeFlag==THOST_FTDC_HF_Hedge);
	SetInputOrderType(OrderType_PATS_LIMIT);
	//SetInputAccount(wxString(pField->szAccount));
	if(pField->PosiDirection==THOST_FTDC_PD_Long)	{	// 持仓为买是，用当前买价执行平仓卖出
		fPrice = field.BidPrice1 - pOrderCfg->PositionVolume*m_currPriceTick;
		if(util::isInvalidValue(fPrice))
			fPrice = field.LastPrice - pOrderCfg->PositionVolume*m_currPriceTick;
		fPrice = AnalysePrice(fPrice, strCode);
		SetInputPrice(fPrice);
	}
	else {
		fPrice = field.AskPrice1 + pOrderCfg->PositionVolume*m_currPriceTick;
		if(util::isInvalidValue(fPrice))
			fPrice = field.LastPrice + pOrderCfg->PositionVolume*m_currPriceTick;
		fPrice = AnalysePrice(fPrice, strCode);
		SetInputPrice(fPrice);
	}
    ////SetInputQty(pField->CloseVolume);
	//SetInputQty(pField->Position);
    delete pField;

//	SetInputAutoOpenClose(TRUE);

//	// 读取手数
//	int nVolume = GetInputQty();
//	// 读取方向
//	BOOL bDirection = GetInputDirection();// GetButtonCheck(ID_RADIOBUTTON1);
//	// 读取开平仓
//	int nOCMode = GetInputOCMode();
	//获取是否自动
	BOOL bIsAuto = TRUE;

	//strAccount = GetInputAccount();

	COrderInsertEntity::ORIORDERREQ tOriReq={0};
	strcpy(tOriReq.strInstrumentID, strCode.c_str());
	strcpy(tOriReq.strAccount, strAccount.c_str());
	tOriReq.bIsParketOrder = FALSE;
	tOriReq.bDirection = bDirection;
	tOriReq.nOCMode = nOCMode;
	tOriReq.fPrice = fPrice;
	tOriReq.bIsMarketPrice = FALSE;
	tOriReq.bSimulate = NeedSimulateMarketPrice();
	tOriReq.nVolume = nVolume;
	tOriReq.nDefaultVolume = evt.GetInt()==3?nVolume:GetDefaultVolume(strCode);		// 在OK按钮下，使用输入手数作为默认手数
	tOriReq.bIsAuto = bIsAuto;
	NeedAutoOpenClose(tOriReq.nAutoMode);
	if(evt.GetInt()==3)
		tOriReq.nAutoMode=1;
	tOriReq.bIsHedge = bHedge;

	BOOL bHasOrderInsert = FALSE;
	DoOrderInsert(tOriReq, nAction, bHasOrderInsert, TRUE);
	m_bUpdateRealMsg_PreClose = true;
	}
}

void OrderInsertPanel::OnCMDMouseMAClose(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	//LOG_INFO("传入用户鼠标快速平仓\n");
	//SetFocus();

	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	LPORDER_CFG pOrderCfg = NULL;
	if(pMgr == NULL) return;
	pOrderCfg = pMgr->GetOrderCfg();

	PlatformStru_Position* pField=(PlatformStru_Position*)evt.GetClientData();
	wxString strParam=evt.GetString();
	char cDirection = 0;
	char cHedge = 0;
	char strTemp[128];
	ZeroMemory(strTemp, sizeof(strTemp));
	strcpy_s(strTemp, sizeof(pField->InstrumentID)-1, pField->InstrumentID);
	cDirection = pField->PosiDirection;
	cHedge = pField->HedgeFlag;
	
	int nAction = 0;
	switch(evt.GetInt()) {
	case 1:
		nAction = pOrderCfg->ClickPositionAction;
		break;
	case 2:
		nAction = pOrderCfg->DoubleClickPositionAction;
		break;
	case 3:
		nAction = NeedConfirm() ? 1 : 2;
		break;
	default:
		delete pField;
		return;
		break;
	}
	if(!nAction)	// 在鼠标快速平仓中，Action==0表示无活动，应立即返回
	{
		delete pField;
		return;
	}
	USERLOG_INFO("持仓汇总鼠标快速平仓\n活动[%d]\n持仓汇总行：\n%s\n", 
			nAction, evt.GetString());

	wxString strCode(strTemp);				// 获得转入的合约代码
	int nDefaultVolume = 0;//GetDefaultVolume(strCode);	// 获得合约的默认量
	double fPrice = 0.0;
	BOOL bIsBuy = FALSE;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	if(DEFAULT_SVR()->IsMultipleAccounts())
	{	// 多帐号下单过程

		// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
		if(pSvr) pSvr->GetQuotInfo(strCode.c_str(),field);
	
		CMultiAccountCore* pMACore = CMultiAccountCore::GetInstance();
		if(pMACore==NULL)
			return;

		CMAOrderEntity* pMAEntity = NULL;
		vector<OrderInsertUIItem> arrayOrderUIItem;
		OrderInsertUIItem orderUIItem;
		COrderAtomEntity::STDORDERREQ stdReq;
		CSAOrderEntity::OPERPARAM operParam;
		CMAOrderEntity::MAAUTOOPER maOper;

		ZeroMemory(&orderUIItem, sizeof(orderUIItem));
		ZeroMemory(&stdReq, sizeof(stdReq));
		ZeroMemory(&operParam, sizeof(operParam));
		ZeroMemory(&maOper, sizeof(maOper));

		maOper.nOperType = CSAOrderEntity::conOperAuto;
		maOper.bIsQuotOrder = FALSE;
		maOper.bNeedCancel = pOrderCfg->nCancelSameDirOpenOrderBeforeOrder;
		maOper.nMouseNotReversed = pOrderCfg->nAllNotHaveOppositePosition;
		maOper.nMousePartReversed = pOrderCfg->nSomeHaveOppositePosition;
		maOper.nMouseAllReversed = pOrderCfg->nAllHaveOppositePosition;
		maOper.nPositionClose = pOrderCfg->nMouseClosePositionStrategy;
		
		bIsBuy = cDirection==THOST_FTDC_PD_Long;
		SetInputInstrumentID(strCode);
		if(bIsBuy) {
			fPrice = field.BidPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? TRUE : FALSE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			SetInputDirection(bIsBuy);
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
			SetInputPrice(fPrice);
		}
		else {
			fPrice = field.AskPrice1;
			bIsBuy = pOrderCfg->MouseFastOrderDirection ? FALSE : TRUE;
			if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
				bIsBuy = !bIsBuy;
			SetInputDirection(bIsBuy);
			if(!util::isInvalidValue(fPrice)) {
				if(bIsBuy)
					fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
				else
					fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
				fPrice = AnalysePrice(fPrice, strCode);
			}
			SetInputPrice(fPrice);
		}
		SetInputQty(nDefaultVolume);

		//operParam.nOperType = CSAOrderEntity::conOperNoneAuto;				// 执行类型，0 非自动单，1 自动单，2 非自动改单
		//operParam.nAutoMode = CMAOrderEntity::conOpenDefaultAndStrategy;	// 自动单模式，0 按持仓量全平，1 按默认手数平仓，不反向开仓
		//operParam.bNeedCancel = FALSE;										// 是否需要撤销相关报单
		//operParam.bIsCanClose = FALSE;										// 是否按可平仓数处理
		operParam.bSimulate = FALSE;											// 是否进行市价模拟
		operParam.nDefaultVolume = nDefaultVolume;							// 默认手数
		operParam.bNoSendVolZoreOrder = TRUE;
		
		stdReq.nOrderType = OrderType_FT_Immediately;
		strcpy_s(stdReq.csInstrumentID, 
				sizeof(stdReq.csInstrumentID)-1, GetInputInstrumentID().c_str());		// 合约
		strcpy_s(stdReq.csUserID, sizeof(stdReq.csUserID)-1, 
				GetInputAccount().c_str());									// 投资者ID或Pats的用户名
		//strcpy_s(stdReq.csAccount, 
		//		sizeof(stdReq.csAccount)-1, orderUIItem.Account);			// 帐号
		stdReq.bIsBuy = bIsBuy;
		stdReq.nOCMode = conOCUnknow;												// 开平仓模式, 0 开仓，1 平今，2 平仓
		stdReq.bIsMarket = FALSE;											// 是否为市价
		stdReq.fPrice = fPrice;
		stdReq.nVolume = nDefaultVolume;
		stdReq.bIsHedge = orderUIItem.IsHedge;

		if(nAction<=2)
		{
			map<string,bool> accsLogon;
			int nRet = GetLogonAccounts(accsLogon, TRUE);
			FilterNoPositionAccount(maOper.bNeedCancel, stdReq, accsLogon);
			DoMaOrderInsert(operParam, stdReq, maOper, accsLogon);
		}

		AsyncViewData(stdReq.bIsBuy, stdReq.nOCMode, nDefaultVolume, stdReq.fPrice, nAction);
	}
	delete pField;
}

void OrderInsertPanel::OnCMDOrderChange(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	//LOG_INFO("传入用户快捷改单\n");

//	wxString strParam(evt.GetString());
    std::string EvtParamStr;
    int EvtParamInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtParamInt))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
	wxString strParam(EvtParamStr);


	switch(EvtParamInt) {
	case 1:		// 市价反手
	{
		string strAccount;
		string strID;
//		char strTmpAccount[1024];
//		char strTmpID[1024];
		char cDirection=0;
		char cHedge=0;
		PlatformStru_Position PositionData;
//		ZeroMemory(strTmpAccount, sizeof(strTmpAccount));
//		ZeroMemory(strTmpID, sizeof(strTmpID));
		//ZeroMemory(&PositionData, sizeof(PositionData));
//		if(strParam[0]==',') {
//			strParam=strParam.substr(1, strParam.size()-1);
//			sscanf_s(strParam.c_str(), "%[^,],%c,%c", strTmpID, sizeof(strTmpID)-1, 
//					&cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		}
//		else
//			sscanf_s(strParam.c_str(), "%[^,],%[^,],%c,%c", strTmpAccount, sizeof(strTmpAccount)-1, 
//					strTmpID, sizeof(strTmpID)-1, &cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		strAccount = strTmpAccount;
//		strID = strTmpID;
		vector<string> outData;
		if(SplitString(strParam.c_str(), outData)<4) 
			return;
		strAccount = outData[0];
		strID = outData[1];
		if(outData[2].size()>0)
			cDirection = outData[2][0];
		if(outData[3].size()>0)
			cHedge = outData[3][0];
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPositionData(strAccount, strID, cDirection, cHedge, PositionData)==0) {
			DoBackHand(strAccount, strID, cDirection, cHedge, PositionData.Position);
			m_bUpdateRealMsg_PreClose = true;
		}
	}
		break;
	case 2:		// 快速改单
	{
		BOOL bIsFound = FALSE;
//		int nOrderSysID = 0, nTmpID = 0;
		PlatformStru_OrderInfo ftdcField;
		vector<PlatformStru_OrderInfo> outData;
//		char strExchangeID[128];
//		char strOrderSysID[128];

//		ZeroMemory(strExchangeID, 128);
//		ZeroMemory(strOrderSysID, 128);
//		sscanf_s(strParam.c_str(), "%[^,],%s", strExchangeID, sizeof(strExchangeID)-1, strOrderSysID, sizeof(strOrderSysID)-1);
		
		string strUserID;
		string strExchangeID;
		string strOrderSysID;
		vector<string> outDataStr;
		if(SplitString(strParam.c_str(), outDataStr)<2) 
			return;
		strUserID = outDataStr[0];
		strExchangeID = outDataStr[1];
		strOrderSysID = outDataStr[2];

		outData.clear();
		IPlatformSingleSvr* pSvr = NULL;
		if(DEFAULT_SVR()->IsMultipleAccounts())
			pSvr = g_pPlatformMgr->GetPlatformSvr(strUserID);
		else
			pSvr = DEFAULT_SVR();
		if(pSvr==NULL) 
			return;
		if(pSvr->GetAllOrders(outData)>0) 
        {
            int i,count;
            count=outData.size();
            for(i=0;i<count;i++)
            {
				if(strOrderSysID.compare(outData[i].OrderSysID)==0 && strExchangeID.compare(outData[i].ExchangeID)==0) 
                {
                    ftdcField = outData[i];
					bIsFound = TRUE;
					break;
				}
			}
			if(bIsFound)
			{
				DoOrderChange(ftdcField, FALSE);
				m_bUpdateRealMsg_PreClose = true;
			}
		}
		break;
	}
	case 3:		// 快捷锁仓
	{
		string strAccount;
		string strID;
//		char strTmpAccount[1024];
//		char strTmpID[1024];
		char cDirection=0;
		char cHedge=0;
		PlatformStru_Position PositionData;
//		ZeroMemory(strTmpAccount, sizeof(strTmpAccount));
//		ZeroMemory(strTmpID, sizeof(strTmpID));
		//ZeroMemory(&PositionData, sizeof(PositionData));
//		if(strParam[0]==',') {
//			strParam=strParam.substr(1, strParam.size()-1);
//			sscanf_s(strParam.c_str(), "%[^,],%c,%c", strTmpID, sizeof(strTmpID)-1, 
//					&cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		}
//		else
//			sscanf_s(strParam.c_str(), "%[^,],%[^,],%c,%c", strTmpAccount, sizeof(strTmpAccount)-1, 
//					strTmpID, sizeof(strTmpID)-1, &cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		strAccount = strTmpAccount;
//		strID = strTmpID;
		vector<string> outData;
		if(SplitString(strParam.c_str(), outData)<4) 
			return;
		strAccount = outData[0];
		strID = outData[1];
		if(outData[2].size()>0)
			cDirection = outData[2][0];
		if(outData[3].size()>0)
			cHedge = outData[3][0];
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPositionData(strAccount, strID, cDirection, cHedge, PositionData)==0) {
			DoBackHand(strAccount, strID, cDirection, cHedge, PositionData.Position, TRUE);
			m_bUpdateRealMsg_PreClose = true;
		}
	}
		break;
	};
}

void OrderInsertPanel::OnCMDOrderReplace(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	//LOG_INFO("传入用户外盘改单\n");

//	wxString strParam(evt.GetString());
    std::string EvtParamStr;
    int EvtParamInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtParamInt))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
	wxString strParam(EvtParamStr);

	BOOL bIsFound = FALSE;
//	int nOrderSysID = 0, nTmpID = 0;
	PlatformStru_OrderInfo ftdcField;
	vector<PlatformStru_OrderInfo> outData;
//	char strExchangeID[128];
//	char strOrderSysID[128];

//	ZeroMemory(strExchangeID, 128);
//	ZeroMemory(strOrderSysID, 128);
//	sscanf_s(strParam.c_str(), "%[^,],%s", strExchangeID, sizeof(strExchangeID)-1, strOrderSysID, sizeof(strOrderSysID)-1);
//	nOrderSysID = atoi(strOrderSysID);
	string strExchangeID;
	string strOrderSysID;
	vector<string> outDataStr;
	if(SplitString(strParam.c_str(), outDataStr)<2) 
		return;
	strExchangeID = outDataStr[0];
	strOrderSysID = outDataStr[1];
	outData.clear();
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetAllOrders(outData)>0) 
    {
        int i,count;
        count=outData.size();
        for(i=0;i<count;i++)
        {
//            nTmpID = atoi(it->second.OrderSysID);
//            if(nTmpID==nOrderSysID && strncmp(strExchangeID, it->second.ExchangeID, strlen(strExchangeID))==0) {
			if(strOrderSysID.compare(outData[i].OrderSysID)==0 && strExchangeID.compare(outData[i].ExchangeID)==0) 
            {
                ftdcField = outData[i];
				bIsFound = TRUE;
				break;
			}
		}
	}

	if(!bIsFound)
		return;

	BOOL bIsAsyncView = FALSE;

	int nSourceVolume = ftdcField.VolumeTotal;
	double fSourcePrice = ftdcField.LimitPrice;

	// 读取方向
	BOOL bDirection = (ftdcField.Direction==THOST_FTDC_D_Buy ? 1 : 0);

	if(bIsAsyncView) {
	//	m_bNoKeyChangeText = FALSE;
	//	SetInputInstrumentID(strInstrumentID);
	//	m_bNoKeyChangeText = TRUE;

	//	SetInputDirection(bDirection);
	//	SetInputQty(nVolume);
	//	wxString strPriceText(wxT("市价"));
	//	SetInputPriceText(strPriceText);
	}

//	// 读取开平仓
//	int nOCMode = GetInputOCMode();
//	//获取是否自动
//	BOOL bIsAuto = TRUE;
	wxString strName = ftdcField.InstrumentID;
    PlatformStru_DepthMarketData field;
    memset(&field,0,sizeof(field));
    if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);
    PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(strName.c_str(),insInfo)==0) {
		m_currPriceTick=insInfo.PriceTick;
    }
	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) {
		m_poStaticFivePriceList->Refresh();
	}

	int nNewVolume = 0;
	double fNewPrice = 0.0;
	BOOL bNewIsMarketPrice = FALSE;

	
	// 这里一个对话框，用于显示带输入框的窗体，方便用户修改价格和数量
	// ...
	PlatformStru_InputOrder tFtdcOrderInputField = {0};
	CEditOrderReplaceDailog* pDlg = new CEditOrderReplaceDailog(this, wxID_ANY, 
			LOADSTRING(OLS_REPLACE_CURRENT_ORDER), wxDefaultPosition, wxSize(310, 250));
	pDlg->SetPriceTick(m_currPriceTick);
	pDlg->SetFtdcOrderField(ftdcField);
	if(pDlg->ShowModal()==wxID_OK) {
		tFtdcOrderInputField = pDlg->GetFtdcOrderInputField();

		pDlg->Destroy();
	}
	else {
		pDlg->Destroy();
		return;
	}

	// 处理风险管理，手数
	if(tFtdcOrderInputField.VolumeTotalOriginal > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
			wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES)
				return;
	}

	// 处理风险管理，价格
	BOOL bIsPriceLimit = tFtdcOrderInputField.LimitPrice>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
		|| tFtdcOrderInputField.LimitPrice<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL,LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	BOOL bOK = TRUE;
	int nRet = 0;
	wxString strErrMsg;
	wxString strMsg;
	if(NeedConfirm()) {
		PlatformStru_InputOrder& req = tFtdcOrderInputField;
		strMsg.Printf(LOADFORMATSTRING(PRCQTY_FORMAT,"%s%s%s%s%s%d%s"),//文本必须包含%s %s %s %s %s %d %s
					Price2String(req.LimitPrice, m_currPriceTick).c_str(),
					wxEmptyString,
					req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL),
					req.InstrumentID, 
					req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): 
					(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
					req.VolumeTotalOriginal,
					req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE): LOADSTRING(OLS_SPECULATE));
		wxMessageDialog dialog(NULL, strMsg, LOADSTRING(OLS_ORDER_CONFIRM), 
				wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
	}
	if(bOK) {
        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(tFtdcOrderInputField, DEFAULT_SVR()->GetNextReqID()):-1;
	}
	wxString strCaption;
	if(nRet==0) {
		m_bWaitingRsp_OrderInsert=true;
	}
	else {
		//设置错误信息
		strErrMsg = CFieldValueTool::ReturnCode2String(nRet, SVR_LANGUAGE).c_str();
		//if(NeedOrderFailConfirm())
		ShowTradeInfoDlg("OLS_SEND_ORDER_FAIL", strErrMsg.c_str(), NeedOrderFailConfirm());
	}

	if(bOK) {
		if(bIsAsyncView) {
			//1 下单后不清空下单板, 2 全部清空, 3 清空价格和手数, 4 只清空手数
			m_poOrderInputPanel->ClearViewData(GetActionAfterOrder());
		}
	}

	return;

}

void OrderInsertPanel::OnMouseWheel(wxMouseEvent& evt)
{
	if(FindFocus()!=FindWindowById(ID_BUTTON_OK,this))
		FindWindowById(ID_BUTTON_OK, this)->SetFocus();
	evt.Skip();
}

void OrderInsertPanel::DoInitQryStep()
{
    vector<string> accountList;
    accountList.clear();
    //if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
        if(DEFAULT_SVR()&&DEFAULT_SVR()->GetAccountList(accountList)>0) {
            m_poOrderInputPanel->ResetAccountList(accountList);
            m_poPatsOrderInputPanel->ResetAccountList(accountList);
        }
    //}
    m_poOrderInputPanel->ResetContractList();
    m_poPatsOrderInputPanel->ResetContractList();
}

void OrderInsertPanel::OnLanguageChanged( wxCommandEvent& event )
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_CHECK_WDSHOW, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_FIVE_PRICE_LIST));
	pWin = FindWindowById(ID_CHECK_WINDOWTOPALWAYS, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_MOST_TOP_WINDOW));
	pWin = FindWindowById(ID_DOSTOPPOSITION_CHECK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP_P_L));
	pWin = FindWindowById(ID_DOCONDITION_CHECK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_CONDITION_SEND_ORDER));

	m_poOrderInputPanel->ResetLanguage();
	m_poPatsOrderInputPanel->ResetLanguage();
	m_poOrderBatchPanel->ResetLanguage();
	m_poOrderConditionPanel->ResetLanguage();
	m_poOrderPositionPanel->ResetLanguage();
	m_poOrderStopPanel->ResetLanguage();
	m_poOrderInsertCommandPanel->ResetLanguage();
	m_poStaticFivePriceList->Refresh();
}

void OrderInsertPanel::OnInfoListFocus(wxFocusEvent& evt)
{
	wxWindow* pWin = FindWindowById(ID_BUTTON_MARKETPRICE, this);
	if(pWin) pWin->SetFocus();
}
