#include "StdAfx.h"
#include "PatsystemApi.h"
#include <string>
#include "../inc/Module-Misc/tools_util.h"
#include "ISvr/BusinessData.hpp"
#define SAFE_STR_COPY(a,b,c)	strncpy((a),(c),(b))
CPatsystemApi* g_PatsystemApi=NULL;
ApiFunc* g_coreapi=NULL;
CRITICAL_SECTION g_CS_AccountList;
int  OrderStatusCode[]=
{
	THOST_FTDC_OST_Unknown,
	THOST_FTDC_OST_Unknown,			//ptQueued
	THOST_FTDC_OST_Unknown,			//ptSent
	THOST_FTDC_OST_NoTradeQueueing,				//ptWorking
	THOST_FTDC_OST_Canceled,	//THOST_FTDC_OST_ERROE            ,			//ptRejected
	THOST_FTDC_OST_Canceled,					//ptCancelled
	THOST_FTDC_OST_Unknown,						//ptBalCancelled
	THOST_FTDC_OST_PartTradedQueueing,			//ptPartFilled
	THOST_FTDC_OST_AllTraded,					//ptFilled
	THOST_FTDC_OST_Unknown,						//ptCancelPending
	THOST_FTDC_OST_Unknown,						//ptAmendPending
	THOST_FTDC_OST_Unknown,						//ptUnconfirmedFilled
	THOST_FTDC_OST_Unknown,						//ptUnconfirmedPartFilled
	THOST_FTDC_OST_Unknown,						//ptHeldOrder
	THOST_FTDC_OST_Unknown,						//ptCancelHeldOrder
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,						//ptTransferred
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Unknown,//
	THOST_FTDC_OST_Canceled						//ptExternalCancelled
};
char OrderStatusMsg[][81]=
{
	"Unknow",
	"Submitted to PATSAPI",//ptQueued
	"Received by Patsystems server, order is in transit",//ptSent
	"Accepted by exchange as a valid order",//ptWorking
	"Rejected, either by Patsystems or by the exchange",//ptRejected
	"Order has been cancelled",//ptCancelled
	"The outstanding balance has been cancelled",//ptBalCancelled
	"Order has been partly filled",//ptPartFilled
	"Order has been completely filled",//ptFilled
	"The requested cancel received by Patsystems server, order is in transit",//ptCancelPending
	"The requested amend received by Patsystems server, order is in transit",//ptAmendPending
	"The order has filled but the fills have not yet reached PATS",//ptUnconfirmedFilled
	"The order has part filled but the fills have not reached PATS",//ptUnconfirmedPartFilled
	"Order is a synthetic order waiting for price to trigger",//ptHeldOrder
	"Synthetic order has been cancelled",//ptCancelHeldOrder
	"Unknow",//
	"Unknow",//
	"Unknow",//
	"Unknow",//
	"Unknow",//
	"Transferred the order to a trader account not in the user's Trader Account Group",//ptTransferred
	"Unknow",//
	"Unknow",//
	"Unknow",//
	"The order was cancelled because the exchange has closed"//ptExternalCancelled
};
char Logon_UserID[256],Logon_Password[256];
enum THREAD_STATUS
{
	THREAD_STATUS_EXIT=-1,
	THREAD_STATUS_NORMAL,
	THREAD_STATUS_ENTER_WAITING,
	THREAD_STATUS_WAITING,
	THREAD_STATUS_QRY_INST,
};
volatile long  g_ThreadStatus=THREAD_STATUS_NORMAL; 
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	DWORD starttime=GetTickCount();
	int CountContract=0,IndexContract=0;
	while(g_ThreadStatus!=THREAD_STATUS_EXIT)
	{
		switch(g_ThreadStatus)
		{
		case THREAD_STATUS_ENTER_WAITING:
			starttime=GetTickCount();
			InterlockedExchange(&g_ThreadStatus,THREAD_STATUS_WAITING);
			break;
		case THREAD_STATUS_WAITING:
			if(GetTickCount()-starttime>5000)
			{
				InterlockedExchange(&g_ThreadStatus,THREAD_STATUS_NORMAL);
				g_PatsystemApi->FireLogon();
			}
			break;
		case THREAD_STATUS_QRY_INST:
			{
				if(IndexContract==0)
					starttime=GetTickCount();
				if(GetTickCount()-starttime>1000)//一秒钟内
				{
					int Count=0;
					ApiFunc::ptCountContracts(&Count);
					if(Count>0&&Count==CountContract)
					{
						InterlockedExchange(&g_ThreadStatus,THREAD_STATUS_NORMAL);
						g_PatsystemApi->DoQryInstrument();
					}
                    CountContract=Count;
					starttime=GetTickCount();
				}
				IndexContract++;
			}
			break;
		}
		Sleep(50);
	}
	return NULL;
}

VOID   CALLBACK   OnTimerProc(HWND   hwnd,UINT   uMsg,UINT_PTR   idEvent,DWORD   dwTime) 
{
	CThostFtdcTradingNoticeField Field;
	memset(&Field,0,sizeof(CThostFtdcTradingNoticeField));
	CThostFtdcRspInfoField  RspInfoField;
	RspInfoField.ErrorID=0;
	SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
	g_PatsystemApi->m_pTraderSpi->OnRspQryTradingNotice(&Field, &RspInfoField, 0, true);
}
CPatsystemApi::CPatsystemApi(const char *pszFlowPath)
{
	if(stricmp(pszFlowPath,"Test")==0)
		m_ClientType=TestClient;
	else if(stricmp(pszFlowPath,"Real")==0)
		m_ClientType=RealClient;
	else
		m_ClientType=DemoClient;
	if(g_coreapi==NULL)g_coreapi=new ApiFunc(m_ClientType);

	InitializeCriticalSection(&g_CS_AccountList);
	m_HostLinkState=ptLinkInvalid;
	m_PriceLinkState=ptLinkInvalid;
	DWORD dwThreadID = 0;
	bFisrtReqUserLogin=true;
	bFirstLinkConnected=true;
	g_ThreadStatus=THREAD_STATUS_NORMAL;
	m_hThread= CreateThread(NULL,0,ThreadProc,this,0,&dwThreadID);
	initOrderTypeMap();
	iTimerID=SetTimer( NULL, 0, 4000, OnTimerProc ) ;
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	char strFileName[1024];
	ZeroMemory(strFileName, 1024);

	sprintf(strFileName, "PatsOrder%d%d%d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	m_pWriteLog = new zqWriteLog(LOGMODE_LOCALFILE, strFileName);

}

CPatsystemApi::~CPatsystemApi(void)
{
	CloseHandle(m_hThread);
	std::map<std::string, PlatformStru_DepthMarketData*>::iterator it
		= m_mapInstrumentPrices.begin();
	for (it; it != m_mapInstrumentPrices.end(); ++it)
	{
		if ( NULL != it->second )
		{
			delete it->second;
			it->second = NULL;
		}
	}
	m_mapInstrumentPrices.clear();
    g_PatsystemApi=NULL;
//	DeleteCriticalSection(&g_CS_AccountList);

}
CPatsystemApi *CPatsystemApi::CreateFtdcApi(const char *pszFlowPath, const bool bIsUsingUdp)
{
	g_PatsystemApi=new CPatsystemApi(pszFlowPath);
	return g_PatsystemApi;
}
///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void CPatsystemApi::Release()
{

	if(g_ThreadStatus!=THREAD_STATUS_EXIT)
	{
		InterlockedExchange(&g_ThreadStatus,THREAD_STATUS_EXIT);
		WaitForSingleObject(m_hThread,INFINITE);
	}
	RegisterCallback(false);
	//反注册行情
	std::set<std::string> tempSet(m_SubscribedInstrument);
	std::set<std::string>::iterator it;
	for(it=tempSet.begin();it!=tempSet.end();++it)
	{
		char *pInstrumentID = const_cast<char*>(it->c_str());
		UnSubscribeMarketData(&pInstrumentID,1);
	}
	LogonStatusStruct logonStatus;
	ApiFunc::ptGetLogonStatus(&logonStatus);
	if(ptLogonSucceeded==logonStatus.Status)
	{
		try{
			ApiFunc::ptLogOff();
		}catch(...){
		}

	}
}

///初始化
///@remark 初始化运行环境,只有调用后,接口才开始工作
void CPatsystemApi::Init()
{
}

///等待接口线程结束运行
///@return 线程退出代码
int CPatsystemApi::Join()
{
	return 0;
}

///获取当前交易日
///@retrun 获取到的交易日
///@remark 只有登录成功后,才能得到正确的交易日
const char *CPatsystemApi::GetTradingDay()
{
	return NULL;
}

///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void CPatsystemApi::RegisterCallback(bool reg)
{
	ApiFunc::ptRegisterLinkStateCallback(ptHostLinkStateChange,reg?onHostLinkStateChange:NULL);
	ApiFunc::ptRegisterLinkStateCallback(ptPriceLinkStateChange,reg?onPriceLinkStateChange:NULL);
	ApiFunc::ptRegisterCallback(ptLogonStatus,reg?onLogonStatus:NULL);
	ApiFunc::ptRegisterMsgCallback(ptMessage,reg?onMessage:NULL);
	ApiFunc::ptRegisterOrderCallback(ptOrder,reg?onOrder:NULL);	
	ApiFunc::ptRegisterCallback(ptForcedLogout,reg?onForcedLogout:NULL);
	ApiFunc::ptRegisterCallback(ptDataDLComplete,reg?onDataDLComplete:NULL);
	ApiFunc::ptRegisterPriceCallback(ptPriceUpdate,reg?onPriceUpdate:NULL);
	ApiFunc::ptRegisterFillCallback(ptFill,reg?onFill:NULL);
	ApiFunc::ptRegisterStatusCallback(ptStatusChange,reg?onStatusChange:NULL);
	ApiFunc::ptRegisterContractCallback(ptContractAdded,reg?onContractAdded:NULL);
	ApiFunc::ptRegisterContractCallback(ptContractDeleted,reg?onContractDeleted:NULL);
	ApiFunc::ptRegisterExchangeRateCallback(ptExchangeRate,reg?onExchangeRate:NULL);
	ApiFunc::ptRegisterConStatusCallback(ptConnectivityStatus,reg?onConnectivityStatus:NULL);
	ApiFunc::ptRegisterOrderCancelFailureCallback(ptOrderCancelFailure,reg?onOrderCancelFailure:NULL);
	ApiFunc::ptRegisterAtBestCallback(ptAtBestUpdate,reg?onAtBestUpdate:NULL);	
	ApiFunc::ptRegisterTickerCallback(ptTickerUpdate,reg?onTickerUpdate:NULL);
	ApiFunc::ptRegisterCallback(ptMemoryWarning,reg?onMemoryWarning:NULL);
	ApiFunc::ptRegisterSubscriberDepthCallback(ptSubscriberDepthUpdate,reg?onSubscriberDepthUpdate:NULL);
	ApiFunc::ptRegisterDOMCallback(ptDOMUpdate,reg?onDOMUpdate:NULL);
	ApiFunc::ptRegisterSettlementCallback(ptSettlementCallback,reg?onSettlementCallback:NULL);
	ApiFunc::ptRegisterStrategyCreateSuccess(ptStrategyCreateSuccess,reg?onStrategyCreateSuccess:NULL);
	ApiFunc::ptRegisterStrategyCreateFailure(ptStrategyCreateFailure,reg?onStrategyCreateFailure:NULL);
	ApiFunc::ptRegisterGenericPriceCallback(ptGenericPriceUpdate,reg?onGenericPriceUpdate:NULL);
	ApiFunc::ptRegisterBlankPriceCallback(ptBlankPrice,reg?onBlankPrice:NULL);
	ApiFunc::ptRegisterOrderSentFailureCallback(ptOrderSentFailure,reg?onOrderSentFailure:NULL);
	ApiFunc::ptRegisterOrderQueuedFailureCallback(ptOrderQueuedFailure,reg?onOrderQueuedFailure:NULL);
	ApiFunc::ptRegisterExchangeCallback(ptExchangeUpdate,reg?onExchangeUpdate:NULL);
	ApiFunc::ptRegisterCommodityCallback(ptCommodityUpdate,reg?onCommodityUpdate:NULL);
	ApiFunc::ptRegisterExchangeCallback(ptPurgeCompleted,reg?onPurgeCompleted:NULL);
	ApiFunc::ptRegisterOrderTypeUpdateCallback(ptOrderTypeUpdate,reg?onOrderTypeUpdate:NULL);
}
void CPatsystemApi::RegisterFront(char *pszFrontAddress)
{
	int nStatus;
	switch(m_ClientType)
	{
	case DemoClient:
		nStatus=ApiFunc::ptInitialise(ptDemoClient,ptAPIversion,"","","",true);
		break;
	case TestClient:
		nStatus=ApiFunc::ptInitialise(ptTestClient,ptAPIversion,"APPID_2139","v1.0.0.0","BETA",true);
		break;
	case RealClient:
		nStatus=ApiFunc::ptInitialise(ptClient,ptAPIversion,"FastTrader API 2139","v1.0.0.0","JBF2139R002P",true);
		break;
	}
	//register callback functions
    RegisterCallback(true);
	DLL_TRACE("ApiFunc::ptEnable\n");
	nStatus=ApiFunc::ptEnable(83);
	nStatus=ApiFunc::ptSetHostHandShake(30,600);
	nStatus=ApiFunc::ptSetHostReconnect(30);
	nStatus=ApiFunc::ptSetPriceReconnect(30);
	if(m_ClientType==DemoClient)
		nStatus=ApiFunc::ptSetInternetUser(FALSE);
	else
		nStatus=ApiFunc::ptSetInternetUser(TRUE);

	nStatus=ApiFunc::ptNotifyAllMessages('Y');
	if(m_ClientType==DemoClient)
	{
		nStatus=ApiFunc::ptSetHostAddress("","");
		nStatus=ApiFunc::ptSetPriceAddress("","");
	}
	else
	{
		string strHostAddr,strPriceAddr,strIp,strPort,strAddr(pszFrontAddress);
		int pos=strAddr.find(";");
		strHostAddr=strAddr.substr(0,pos);
		strPriceAddr=strAddr.substr(pos+1);
		pos=strHostAddr.rfind(":");
		strIp=strHostAddr.substr(0,pos);
		strPort=strHostAddr.substr(pos+1);
		nStatus=ApiFunc::ptSetHostAddress(strIp.c_str() ,strPort.c_str());
		pos=strPriceAddr.rfind(":");
		strIp=strPriceAddr.substr(0,pos);
		strPort=strPriceAddr.substr(pos+1);
		nStatus=ApiFunc::ptSetPriceAddress(strIp.c_str() ,strPort.c_str());
	}

	m_pMdSpi->OnFrontConnected();
	m_pTraderSpi->OnFrontConnected();
}

///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CPatsystemApi::RegisterSpi(CPatsQuotSpi *pSpi)
{
	m_pMdSpi= pSpi;
}
///注册回调接口
///@param pSpi 派生自回调接口类的实例
void CPatsystemApi::RegisterSpi(CPatsTraderSpi *pSpi)
{
	m_pTraderSpi=pSpi;
}
//---------------------------------------------------------------------------------------------
///订阅行情。
///@param ppInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int CPatsystemApi::SubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	for ( int i = 0; i < nCount; i++)
	{
		std::string strInstrumentID = ppInstrumentID[i];
		std::map<std::string, ContractStruct>::iterator it = m_mapInstrumentDetail.find( strInstrumentID );
		if ( it != m_mapInstrumentDetail.end())
		{
			ContractStruct contract = it->second;
			int nStatus = ApiFunc::ptSubscribePrice(&contract.ExchangeName, &contract.ContractName, &contract.ContractDate );
			m_SubscribedInstrument.insert(strInstrumentID);
		}
	}

	return 0;
}

///退订行情。
///@param ppInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
int CPatsystemApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	for ( int i = 0; i < nCount; i++)
	{
		std::string strInstrumentID = ppInstrumentID[i];
		std::map<std::string, ContractStruct>::iterator it = m_mapInstrumentDetail.find( strInstrumentID );
		if ( it != m_mapInstrumentDetail.end())
		{
			ContractStruct contract = it->second;
			ApiFunc::ptUnsubscribePrice(&contract.ExchangeName, &contract.ContractName, &contract.ContractDate );
			m_SubscribedInstrument.erase(strInstrumentID);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------------------------
///订阅私有流。
///@param nResumeType 私有流重传方式  
///        THOST_TERT_RESTART:从本交易日开始重传
///        THOST_TERT_RESUME:从上次收到的续传
///        THOST_TERT_QUICK:只传送登录后私有流的内容
///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
void CPatsystemApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{

}

///订阅公共流。
///@param nResumeType 公共流重传方式  
///        THOST_TERT_RESTART:从本交易日开始重传
///        THOST_TERT_RESUME:从上次收到的续传
///        THOST_TERT_QUICK:只传送登录后公共流的内容
///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
void CPatsystemApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) 
{

}

///客户端认证请求
int CPatsystemApi::ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID)
{
	return 0;
}


///用户登录请求
int CPatsystemApi::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	if(bFisrtReqUserLogin)
	{
		if(m_ClientType!=DemoClient)
		{
			strcpy(Logon_UserID,pReqUserLoginField->UserID);
			strcpy(Logon_Password,pReqUserLoginField->Password);
		}
		else
		{
			strcpy(Logon_UserID,"DEMO1\0");
			strcpy(Logon_Password,"\0");
		}
		DLL_TRACE("ApiFunc::ptReady\n");
		int nStatus=ApiFunc::ptReady();
		nStatus=ApiFunc::ptSuperTASEnabled();
		DoLogon(-1,pReqUserLoginField,nRequestID);
		bFisrtReqUserLogin=false;
	}
	return 0;
}



///登出请求
int CPatsystemApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	return 0;
}


///用户口令更新请求
int CPatsystemApi::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID)
{
	return 0;
}


///资金账户口令更新请求
int CPatsystemApi::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID)
{
	return 0;
}

string Char2String(CHAR cData)
{
	char strText[32];
	ZeroMemory(strText, 32);
	sprintf(strText, "0x%02x(%c)", cData, cData>32&&cData<128?cData:' ');
	return strText;
}

///报单录入请求
int CPatsystemApi::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID)
{
	//只支持Market、Limit、Stop、Stop Limit、Replace

	//获得当前报单类型文本currOrderType
	OrderTypeStruct OrderType;
	AmendTypesArray strAmendTypesArray;
	int CountOrderTypes=0;
	Array10 currOrderType={0};
	ApiFunc::ptCountOrderTypes(&CountOrderTypes);
	for(int cLoop=0;cLoop<CountOrderTypes;cLoop++) {
		ApiFunc::ptGetOrderType(cLoop, &OrderType, strAmendTypesArray);
		//DLL_TRACE("%d = %s\n", OrderType.OrderTypeID, OrderType.OrderType);
		// 如果是市价单
		if(OrderType.OrderTypeID==ptOrderTypeMarket 
				&& pInputOrder->OrderPriceType==THOST_FTDC_OPT_AnyPrice 
				&& pInputOrder->ContingentCondition==THOST_FTDC_CC_Immediately) {
			strcpy(currOrderType, OrderType.OrderType);
			break;
		}
		// 如果是限价单
		else if(OrderType.OrderTypeID==ptOrderTypeLimit 
				&& pInputOrder->OrderPriceType==THOST_FTDC_OPT_LimitPrice
				&& pInputOrder->ContingentCondition==THOST_FTDC_CC_Immediately) {
			strcpy(currOrderType, OrderType.OrderType);
			break;
		}
		// 如果是Stop单
		else if(OrderType.OrderTypeID==ptOrderTypeStop 
				&& pInputOrder->OrderPriceType==THOST_FTDC_OPT_AnyPrice
				&& pInputOrder->ContingentCondition==THOST_FTDC_CC_Touch) {
			strcpy(currOrderType, OrderType.OrderType);
			break;
		}
		// 如果是Stop Limit单
		else if(OrderType.OrderTypeID==ptOrderTypeStopLoss 
				&& pInputOrder->OrderPriceType==THOST_FTDC_OPT_LimitPrice
				&& pInputOrder->ContingentCondition==THOST_FTDC_CC_Touch) {
			strcpy(currOrderType, OrderType.OrderType);
			break;
		}
	}
	if(strlen(currOrderType)==0)
		return -999;
	//获取合约结构体Contract
	std::map<std::string, ContractStruct>::iterator it = m_mapInstrumentDetail.find(pInputOrder->InstrumentID);
	if(it == m_mapInstrumentDetail.end())
		return -999;
	ContractStruct Contract = it->second;
    //组织下单结构体
	NewOrderStruct Order;
	ZeroMemory(&Order, sizeof(Order));
	strcpy(Order.TraderAccount, pInputOrder->strAccount);
	strcpy(Order.ExchangeName, Contract.ExchangeName);
	strcpy(Order.ContractName, Contract.ContractName);
	strcpy(Order.ContractDate, Contract.ContractDate);
	strcpy(Order.OrderType, currOrderType);
	Order.BuyOrSell = pInputOrder->Direction==THOST_FTDC_D_Buy ? ('B') : ('S');
	Order.OpenOrClose = pInputOrder->CombOffsetFlag[0]==THOST_FTDC_OF_Open ? ('O') : ('C');
	if(OrderType.OrderTypeID==ptOrderTypeStop) {
		sprintf_s(Order.Price, sizeof(Order.Price)-1,"%0.6f\0", pInputOrder->StopPrice);
	}
	else if(OrderType.OrderTypeID==ptOrderTypeStopLoss) {
		sprintf_s(Order.Price, sizeof(Order.Price)-1,"%0.6f\0", pInputOrder->StopPrice);
		sprintf_s(Order.Price2, sizeof(Order.Price2)-1,"%f\0", pInputOrder->LimitPrice);
	}
	else {
		sprintf_s(Order.Price, sizeof(Order.Price)-1,"%0.6f\0", pInputOrder->LimitPrice);
	}
	Order.Lots = pInputOrder->VolumeTotalOriginal;

	if(pInputOrder->OrderClass==ORDERCLASS_NewOrder || pInputOrder->OrderClass=='\000') {
		//执行下单
		OrderIDStr  SrcOrderID={0},DstOrderID={0};
		int ret=ApiFunc::ptAddOrder(&Order,(OrderIDPtr)SrcOrderID);
		LOG_INFO("ReqOrderInsert(报单录入请求) Insert:ret=[%d],nRequestID=[%s]\n"
				 "\t\t\t TraderAccount=[%s],\t OrderType=[%s],\t ExchangeName=[%s],\t ContractName=[%s],\t ContractDate=[%s],\n"
				 "\t\t\t BuyOrSell=[%c],\t Price=[%s],\t Price2=[%s],\t Lots=[%d],\n"
				 "\t\t\t LinkedOrder=[%s],\t OpenOrClose=[%s],\t Xref=[%d],\t XrefP=[%d],\t GoodTillDate=[%s],\n"
				 "\t\t\t TriggerNow=[%s],\t Reference=[%s],\t ESARef=[%d],\t Priority=[%d],\t TriggerDate=[%s],\n"
				 "\t\t\t TriggerTime=[%s],\t BatchID=[%s],\t BatchType=[%s],\t BatchCount=[%d],\t BatchStatus=[%s],\n"
				 "\t\t\t ParentID=[%s],\t DoneForDay=[%s],\t BigRefField=[%s],\t SenderLocationID=[%s],\t Rawprice=[%s],\n"
				 "\t\t\t Rawprice2=[%s],\t ExecutionID=[%s],\t ClientID=[%s],\t APIM=[%s],\t APIMUser=[%s],\n"
				 "\t\t\t YDSPAudit=[%s],\t ICSNearLegPrice=[%s],\t MinClipSize=[%d],\t MaxClipSize=[%d],\t Randomise=[%s],\n"
				 "\t\t\t TicketType=[%s],\t TicketVersion=[%s],\t ExchangeField=[%s],\t BOFID=[%s],\t Badge=[%s],\n"
				 "\t\t\t LocalUserName=[%s],\t LocalTrader=[%s],\t LocalBOF=[%s],\t LocalOrderID=[%s],\t LocalExAcct=[%s],\n"
				 "\t\t\t RoutingID1=[%s],\t RoutingID2=[%s],\t Inactive=[%s]",
				 ret, SrcOrderID,
				 Order.TraderAccount, Order.OrderType, Order.ExchangeName, Order.ContractName, Order.ContractDate, 
				 Order.BuyOrSell, Order.Price, Order.Price2, Order.Lots, 
				 Order.LinkedOrder, Char2String(Order.OpenOrClose).c_str(), Order.Xref, Order.XrefP, Order.GoodTillDate, 
				 Char2String(Order.TriggerNow).c_str(), Order.Reference, Order.ESARef, Order.Priority, Order.TriggerDate, 
				 Order.TriggerTime, Order.BatchID, Order.BatchType, Order.BatchCount, Order.BatchStatus, 
				 Order.ParentID, Char2String(Order.DoneForDay).c_str(), Order.BigRefField, Order.SenderLocationID, Order.Rawprice, 
				 Order.Rawprice2, Order.ExecutionID, Order.ClientID, Char2String(Order.APIM).c_str(), Order.APIMUser, 
				 Order.YDSPAudit, Order.ICSNearLegPrice, Order.ICSFarLegPrice, Order.MinClipSize, Order.MaxClipSize, Char2String(Order.Randomise).c_str(), 
				 Order.TicketType, Order.TicketVersion, Order.ExchangeField, Order.BOFID, Order.Badge, 
				 Order.LocalUserName, Order.LocalTrader, Order.LocalBOF, Order.LocalOrderID, Order.LocalExAcct, 
				 Order.RoutingID1, Order.RoutingID2, Char2String(Order.Inactive).c_str());

		//返回RequestID和错误码
		sscanf_s(SrcOrderID,"%[0-9]",DstOrderID,sizeof(DstOrderID)-1);
		pInputOrder->RequestID = atoi(DstOrderID);
		switch(ret) {
		case ptSuccess:					// Order has been sent to the host for processing (not the exchange).
			return 0;
			break;
		case ptErrNotInitialised:		// API has not been initialised.
		case ptErrNotLoggedOn:			// API has not logged on to the host.
		case ptErrPriceRequired:		// Order type requires a price and one was not provided.
		case ptErrInvalidPrice:			// Supplied price did not convert to a valid real number
			return -999;
			break;
		case ptErrNotTradable:
			return -98;
			break;
		case ptErrUnknownAccount:		// Trader account does not match a known trader.
			return -99;
			break;
		case ptErrUnknownOrderType:		// Order type is not known to PATS.
		case ptErrUnknownContract:		// Contract name / date does not refer to a valid contract.
			return -999;
			break;
		case ptErrTASUnavailable:		// Transaction Server is not connected.
		case ptErrMDSUnavailable:		// Market Data Server is not connected.	
			return -1;
			break;
		default:
			return -999;
			break;
		}
	}
	else if(pInputOrder->OrderClass==ORDERCLASS_ReplaceOrder) {
		///外盘改单
		//ptAmendOrder用于改单
		OrderIDStr SrcOrderID={0};
		AmendOrderStruct amendOrder={0};
		ZeroMemory(SrcOrderID, sizeof(OrderIDStr));
		ZeroMemory(&amendOrder, sizeof(AmendOrderStruct));
		memcpy(SrcOrderID, pInputOrder->strLocalRefID, 
			sizeof(OrderIDStr) < sizeof(pInputOrder->OrderRef) ? 
			sizeof(OrderIDStr) : sizeof(pInputOrder->OrderRef));

		strcpy(amendOrder.Trader, Order.TraderAccount);
		//strcpy(amendOrder.ExchangeName, Order.ExchangeName);
		//strcpy(amendOrder.ContractName, Order.ContractName);
		//strcpy(amendOrder.ContractDate, Order.ContractDate);
		strcpy(amendOrder.AmendOrderType, Order.OrderType);
		//amendOrder.BuyOrSell = Order.BuyOrSell;
		amendOrder.OpenOrClose = Order.OpenOrClose;
		strcpy(amendOrder.Price, Order.Price);
		strcpy(amendOrder.Price2, Order.Price2);
		amendOrder.Lots = Order.Lots;

		int ret=ApiFunc::ptAmendOrder((OrderIDPtr)SrcOrderID, &amendOrder);
		LOG_INFO("ReqOrderInsert(报单录入请求) Replace:ret=[%d],SrcOrderID=[%s]\n"
				 "\t\t\t Price=[%s],\t Price2=[%s],\t Lots=[%d],\t LinkedOrder=[%s],\t OpenOrClose=[%s],\n"
				 "\t\t\t Trader=[%s],\t Reference=[%s],\t Priority=[%d],\t TriggerDate=[%s],\t TriggerTime=[%s],\n"
				 "\t\t\t BatchID=[%s],\t BatchType=[%s],\t BatchCount=[%d],\t BatchStatus=[%s],\t ParentID=[%s],\n"
				 "\t\t\t DoneForDay=[%c],\t BigRefField=[%s],\t SenderLocationID=[%s],\t Rawprice=[%s],\t Rawprice2=[%s],\n"
				 "\t\t\t ExecutionID=[%s],\t ClientID=[%s],\t ESARef=[%s],\t YDSPAudit=[%s],\t ICSNearLegPrice=[%s],\n"
				 "\t\t\t ICSFarLegPrice=[%s],\t MaxClipSize=[%d],\t LocalUserName=[%s],\t LocalTrader=[%s],\t LocalBOF=[%s],\n"
				 "\t\t\t LocalOrderID=[%s],\t LocalExAcct=[%s],\t RoutingID1=[%s],\t RoutingID2=[%s],\t AmendOrderType=[%s],\n"
				 "\t\t\t TargetUserName=[%s]",
				 ret, SrcOrderID, 
				 amendOrder.Price, amendOrder.Price2, amendOrder.Lots, amendOrder.LinkedOrder, Char2String(amendOrder.OpenOrClose).c_str(), 
				 amendOrder.Trader, amendOrder.Reference, amendOrder.Priority, amendOrder.TriggerDate, amendOrder.TriggerTime, 
				 amendOrder.BatchID, amendOrder.BatchType, amendOrder.BatchCount, amendOrder.BatchStatus, amendOrder.ParentID, 
				 amendOrder.DoneForDay, amendOrder.BigRefField, amendOrder.SenderLocationID, amendOrder.Rawprice, amendOrder.Rawprice2, 
				 amendOrder.ExecutionID, amendOrder.ClientID, amendOrder.ESARef, amendOrder.YDSPAudit, amendOrder.ICSNearLegPrice, 
				 amendOrder.ICSFarLegPrice, amendOrder.MaxClipSize, amendOrder.LocalUserName, amendOrder.LocalTrader, amendOrder.LocalBOF, 
				 amendOrder.LocalOrderID, amendOrder.LocalExAcct, amendOrder.RoutingID1, amendOrder.RoutingID2, amendOrder.AmendOrderType,
				 amendOrder.TargetUserName);

		switch(ret) {
		case ptSuccess:					// Request to amend the order has been sent to the Host.
			return 0;
			break;
		case ptErrNotInitialised:		// API has not been initialised.
		case ptErrNotLoggedOn:			// API is not logged on to the Host.
			return -1;
			break;
		case ptErrNoData:				// API does not currently hold any order information.
		case ptErrUnknownOrder:			// Patsystems ID specified does not match a valid order.
		case ptErrAmendDisabled:		// Amend is not supported by the exchange. Use cancel/add.
		case ptErrInvalidState:			// Order may not be amended at this time.
		case ptErrInvalidPrice:			// New price is not valid.
		case ptErrInvalidVolume:		// New volume is not valid.
			return -999;
			break;
		case ptErrUnknownAccount:		// New trader account does not exist.
			return -99;
		case ptErrInvalidAmendOrderType:	// This Order cannot be amended to this Order Type
		default:
			return -999;
		}
	}
	else {
		return -999;
	}

}

///预埋单录入请求
int CPatsystemApi::ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID)
{
	return 0;
}


///预埋撤单录入请求
int CPatsystemApi::ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID)
{
	return 0;
}


///报单操作请求
int CPatsystemApi::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID)
{
	OrderIDStr OrderID={0};
	SAFE_STR_COPY(OrderID,sizeof(OrderIDStr), pInputOrderAction->OrderSysID);
	int ret = ApiFunc::ptCancelOrder((OrderIDPtr)OrderID);
	LOG_INFO("ReqOrderAction(报单撤单请求):ret=[%d],SrcOrderID=[%s]\n",
			 ret, OrderID);
	return ret;
}


///查询最大报单数量请求
int CPatsystemApi::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID)
{
	return 0;
}


///投资者结算结果确认
int CPatsystemApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID)
{
	return 0;
}


///请求删除预埋单
int CPatsystemApi::ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID)
{
	return 0;
}


///请求删除预埋撤单
int CPatsystemApi::ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID)
{
	return 0;
}


///请求查询报单
int CPatsystemApi::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID)
{
	int CountOrderHistory=0;
	int Count=0;

	int ret=ApiFunc::ptCountOrderHistory(0,&CountOrderHistory);
    ApiFunc::ptCountOrders(&Count);
	if(Count==0)
	{
        //PlatformStru_OrderInfo Field;
		//memset(&Field,0,sizeof(PlatformStru_OrderInfo));
		CThostFtdcRspInfoField  RspInfoField;
		RspInfoField.ErrorID=0;
		SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
		m_pTraderSpi->OnRspQryOrder(NULL, &RspInfoField, nRequestID, true);
	}
	for(int cLoop=0;cLoop<Count;cLoop++)
	{
        OrderDetailStruct OrderDetail;
		//int Position=1;
		ApiFunc::ptGetOrder(cLoop,&OrderDetail);
		m_OrderList.push_back(OrderDetail);

		PlatformStru_OrderInfo Field;
		//memset(&Field,0,sizeof(PlatformStru_OrderInfo));

		Field.BrokerOrderSeq=OrderDetail.Index;
		if(Field.BrokerOrderSeq<0)
			Field.BrokerOrderSeq=0;
		strcpy(Field.OrderSysID,OrderDetail.OrderID);
        strcpy(Field.RelativeOrderSysID,OrderDetail.ExchOrderID);
		strcpy(Field.ExchangeID,OrderDetail.ExchangeName);
		std::string strInstrumentID = MakeLongInstrumentID( OrderDetail.ExchangeName, OrderDetail.ContractName, OrderDetail.ContractDate);
		strcpy(Field.InstrumentID, strInstrumentID.c_str());
		if(OrderDetail.BuyOrSell=='B')
            Field.Direction=THOST_FTDC_D_Buy;
		else if(OrderDetail.BuyOrSell=='S')
            Field.Direction=THOST_FTDC_D_Sell;
		/*std::map<std::string, char>::iterator it = g_PatsystemApi->mapOrderType.find(OrderDetail.OrderType);
		if(it!=g_PatsystemApi->mapOrderType.end())
		{
			Field.OrderPriceType=it->second;
		}*/

		if(strcmp(OrderDetail.OrderType,"Market")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_AnyPrice; 
			//Field.LimitPrice=atof(OrderDetail.AveragePrice);
			Field.ContingentCondition=THOST_FTDC_CC_Immediately;
		}
		else if(strcmp(OrderDetail.OrderType,"Limit")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			Field.LimitPrice=atof(OrderDetail.Price);
			Field.ContingentCondition=THOST_FTDC_CC_Immediately;
		}
		else if(strcmp(OrderDetail.OrderType,"Stop")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
			//Field.LimitPrice=atof(OrderDetail.AveragePrice);
			Field.StopPrice=atof(OrderDetail.Price);
			if(OrderDetail.BuyOrSell=='B')
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
			else 
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
		}
		else if(strcmp(OrderDetail.OrderType,"Stop Limit")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			Field.StopPrice=atof(OrderDetail.Price);
			Field.LimitPrice=atof(OrderDetail.Price2);
			if(OrderDetail.BuyOrSell=='B')
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
			else 
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
		}
        Field.VolumeTotalOriginal=OrderDetail.Lots;
		Field.VolumeTraded=OrderDetail.AmountFilled;
		Field.VolumeTotal=Field.VolumeTotalOriginal-Field.VolumeTraded;
		Field.OrderStatus=OrderStatusCode[OrderDetail.Status];
		SAFE_STR_COPY(Field.StatusMsg,sizeof(Field.StatusMsg),OrderStatusMsg[OrderDetail.Status]);
		if(OrderDetail.OpenOrClose=='O')
            Field.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
		else if(OrderDetail.OpenOrClose=='C')
            Field.CombOffsetFlag[0]=THOST_FTDC_OF_Close;
		strcpy(Field.InsertDate,OrderDetail.DateSent);
        //strcpy(Field.InsertTime,OrderDetail.TimeSent);
		sprintf_s(Field.InsertTime, sizeof(Field.InsertTime),"%.2s:%.2s:%.2s",OrderDetail.TimeSent,OrderDetail.TimeSent+2,OrderDetail.TimeSent+4);
		strcpy(Field.ActiveTime,OrderDetail.TimeExchRecd);
        //strcpy(Field.UpdateTime,OrderDetail.TimeExchAckn);
		if(strlen(OrderDetail.TimeExchAckn)>1 && (OrderDetail.Status==ptFilled ||OrderDetail.Status==ptPartFilled))
		    sprintf_s(Field.UpdateTime,sizeof(Field.UpdateTime),"%.2s:%.2s:%.2s",OrderDetail.TimeExchAckn,OrderDetail.TimeExchAckn+2,OrderDetail.TimeExchAckn+4);
		if((OrderDetail.Status==ptFilled ||OrderDetail.Status==ptPartFilled))
            Field.AvgPrice = atof(OrderDetail.AveragePrice);
		Field.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
		strcpy(Field.OrderRef,OrderDetail.OrderID);
		strcpy(Field.Price2,OrderDetail.Price2);
		//sscanf(OrderDetail.ExchOrderID,"%d",&Field.FrontID);
		//sscanf(OrderDetail.ExecutionID,"%d",&Field.SessionID);
		//Field.SessionID=OrderDetail.NoOfFills;
		strcpy(Field.szAccount,OrderDetail.TraderAccount);

		CThostFtdcRspInfoField  RspInfoField;
		RspInfoField.ErrorID=0;
		SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
		bool bIsLast=false;
		if(cLoop==Count-1)
			bIsLast=true;
		m_pTraderSpi->OnRspQryOrder(&Field, &RspInfoField, nRequestID, bIsLast);
	}

	return 0;
}


///请求查询成交
int CPatsystemApi::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID)
{
	int Count=0;
	int ret=ApiFunc::ptCountFills(&Count);
	if(Count==0)
	{
		//PlatformStru_TradeInfo Field;
		//memset(&Field,0,sizeof(PlatformStru_TradeInfo));
		CThostFtdcRspInfoField  RspInfoField;
		RspInfoField.ErrorID= 0;
		SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
		m_pTraderSpi->OnRspQryTrade(NULL, &RspInfoField, nRequestID, true);
	}
    vector<FillStruct> FillStructArray;
	for(int cLoop=0;cLoop<Count;cLoop++)
	{
        FillStruct FillDetail={0};
		ApiFunc::ptGetFill(cLoop,&FillDetail);
        if(FillDetail.FillType!=ptNettedFill)//剔出隔夜仓位信息
            FillStructArray.push_back(FillDetail);
        m_PatsPositionItems.insert(PatsPositionItem(
            FillDetail.TraderAccount,
            FillDetail.ExchangeName,
            FillDetail.ContractName,
            FillDetail.ContractDate));
    }
    Count=(int)FillStructArray.size();
	if(Count>0)
	{
    for(int cLoop=0;cLoop<Count;cLoop++)
    {
        FillStruct& FillDetail=FillStructArray[cLoop];
        PlatformStru_TradeInfo Field;
        //memset(&Field,0,sizeof(PlatformStru_TradeInfo));
		SAFE_STR_COPY(Field.szAccount, sizeof(Field.szAccount), FillDetail.TraderAccount);
        sprintf_s(Field.OrderLocalID,sizeof(Field.OrderLocalID)-1,"%d",FillDetail.Index);
        SAFE_STR_COPY(Field.OrderSysID, sizeof(Field.OrderSysID)-1, FillDetail.OrderID);
        memcpy(Field.TradeID,FillDetail.ExchangeFillID, sizeof(Field.TradeID)-1);
 		//memcpy(Field.TradeID,FillDetail.FillId+strlen(FillDetail.FillId)-(sizeof(Field.TradeID)-1), sizeof(Field.TradeID)-1);
       strcpy(Field.ExchangeID,FillDetail.ExchangeName);
        std::string strInstrumentID = MakeLongInstrumentID( FillDetail.ExchangeName, FillDetail.ContractName, FillDetail.ContractDate);
        SAFE_STR_COPY(Field.InstrumentID, sizeof(Field.InstrumentID)-1, strInstrumentID.c_str());
        if(FillDetail.BuyOrSell=='B')
            Field.Direction=THOST_FTDC_D_Buy;
        else if(FillDetail.BuyOrSell=='S')
            Field.Direction=THOST_FTDC_D_Sell;

        Field.Price = atof( FillDetail.Price);
        Field.Volume=FillDetail.Lots;

        Field.OffsetFlag=THOST_FTDC_OF_Open;
        Field.HedgeFlag = THOST_FTDC_HF_Speculation;
		SAFE_STR_COPY(Field.TradeDate, sizeof(Field.TradeDate), FillDetail.DateFilled);
		SAFE_STR_COPY(Field.TradingDay, sizeof(Field.TradingDay), FillDetail.DateFilled);
		SAFE_STR_COPY(Field.TradeTime, sizeof(Field.TradeTime), FillDetail.TimeFilled);

        CThostFtdcRspInfoField  RspInfoField;
        RspInfoField.ErrorID=0;
        SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
        m_pTraderSpi->OnRspQryTrade(&Field, &RspInfoField, nRequestID, cLoop+1== Count);
    }
	}
	else 
	{
		//PlatformStru_TradeInfo Field;
		//memset(&Field,0,sizeof(PlatformStru_TradeInfo));
		CThostFtdcRspInfoField  RspInfoField;
		RspInfoField.ErrorID= 0;
		SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
		m_pTraderSpi->OnRspQryTrade(NULL, &RspInfoField, nRequestID, true);
	}
	return 0;
}

///请求查询资金账户
int CPatsystemApi::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID)
{
	//if ( m_AccountList.empty())
	//{
	//	return -1;
	//}
	//std::string ExchangeName=" ";
 //   std::string ContractName=" ";
	//std::string ContractDate=" ";
	//char MarginReqd[21];
	//char BPRemaining[21];
	//char BPUsed[21];
	//char Exposure[21];
	//char BurnRate[21];
	//std::list<TraderAcctStruct>::iterator it=m_AccountList.begin();
	//strcpy(m_accountName,(*it).TraderAccount);
	////int ret=ApiFunc::ptTotalMarginPaid((ExchNamePtr)&ExchangeName,(ConNamePtr)&ContractName,(ConDatePtr)&ContractDate,(TraderPtr)(*it).TraderAccount,(FloatPtr)&MarginReqd);
	//ApiFunc::ptTotalMarginPaid((ExchNamePtr)&ExchangeName,(ConNamePtr)&ContractName,(ConDatePtr)&ContractDate,(TraderPtr)m_accountName,(FloatPtr)MarginReqd);
	//ApiFunc::ptBuyingPowerRemaining((ExchNamePtr)&ExchangeName,(ConNamePtr)&ContractName,(ConDatePtr)&ContractDate,(TraderPtr)m_accountName,(FloatPtr)BPRemaining);
	//ApiFunc::ptBuyingPowerUsed((ExchNamePtr)&ExchangeName,(ConNamePtr)&ContractName,(ConDatePtr)&ContractDate,(TraderPtr)m_accountName,(FloatPtr)BPUsed);
	//ApiFunc::ptOpenPositionExposure((ExchNamePtr)&ExchangeName,(ConNamePtr)&ContractName,(ConDatePtr)&ContractDate,(TraderPtr)m_accountName,(FloatPtr)Exposure);
	//ApiFunc::ptPLBurnRate((ExchNamePtr)&ExchangeName,(ConNamePtr)&ContractName,(ConDatePtr)&ContractDate,(TraderPtr)m_accountName,(FloatPtr)BurnRate);

	CThostFtdcTradingAccountField Field;
	memset(&Field,0,sizeof(CThostFtdcTradingAccountField));
 //   strcpy(Field.AccountID,m_accountName);
	//double dBPRemaining=0.0;
	//sscanf(BPRemaining,"%lf",&dBPRemaining);
	//Field.CashIn=dBPRemaining;
	CThostFtdcRspInfoField  RspInfoField;
	RspInfoField.ErrorID=0;
	SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
	m_pTraderSpi->OnRspQryTradingAccount(&Field, &RspInfoField, nRequestID, true);
	return 0;
}
void CPatsystemApi::CalcPositionCloseProfit(const std::string strAccount,PlatformStru_TradingAccountInfo& info)
{
    double TotalPL=0.0;//总盈亏
	info.PositionProfit=0.0;//持仓盈亏
	PositionStruct posi;
	if(ApiFunc::ptGetTotalPosition((TraderPtr)info.Account,(PositionStructPtr)&posi)==ptSuccess)
		TotalPL=atof(posi.Profit);//使用本地货币
	std::set<PatsPositionItem>::iterator it;
	for(it=m_PatsPositionItems.begin();it!=m_PatsPositionItems.end();++it)
	{
        if(strAccount!=it->TraderAccount)
            continue;
		ContractStruct contract={0};
		PositionStruct openPosition;
		SafeFloatStr BurnRate={0};
		std::string strInstrumentID = MakeLongInstrumentID(it->ExchangeName,it->ContractName,it->ContractDate);
		std::map<std::string, ContractStruct>::iterator map_it = m_mapInstrumentDetail.find(strInstrumentID);
		if ( map_it != m_mapInstrumentDetail.end())
		{
			contract = map_it->second;
            ApiFunc::ptGetOpenPosition(&contract.ExchangeName, &contract.ContractName, 
					&contract.ContractDate,(TraderPtr)&(it->TraderAccount),&openPosition);
			CommodityStruct commodity={0};	
			SafeFloatStr ExchRate={0};
			ApiFunc::ptGetCommodityByName(&contract.ExchangeName, &contract.ContractName,&commodity);
			ApiFunc::ptGetExchangeRate((CurrNamePtr)commodity.Currency,(ExchRatePtr)ExchRate);
			info.PositionProfit+=atof(openPosition.Profit)/atof(ExchRate);//累积持仓盈亏，使用合约货币,转化货币
		}	
	}
	info.CloseProfit=TotalPL-info.PositionProfit;//平仓盈亏
}
///得到查询资金账户信息
int CPatsystemApi::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Field)
{
	if ( m_AccountList.empty())
	{
		return -1;
	}
	ExchNameStr ExchangeName={0};
	ConNameStr  ContractName={0};
	ConDateStr ContractDate ={0};
	/*清空结构体*/
	memset(&Field,0,sizeof(PlatformStru_TradingAccountInfo));
	/*设置用户名*/
	Array20 tradeAccount={0};
	strncpy(Field.Account,strAccount.c_str(),sizeof(Field.Account)-1);
	strncpy_s(tradeAccount,strAccount.c_str(), sizeof(tradeAccount)-1);
	/*静态权益通过ptGetTraderByName获得的LossLimit来计算*/
	TraderAcctStruct TraderAcct;
	ApiFunc::ptGetTraderByName((TraderPtr)strAccount.c_str(),&TraderAcct);
	Field.StaticProfit=TraderAcct.LossLimit;
	/*持仓盈亏*/    /*平仓盈亏*/
	CalcPositionCloseProfit(strAccount,Field);
	/*动态权益*/
	Field.DynamicProfit=Field.StaticProfit+Field.PositionProfit+Field.CloseProfit;
	/*缴付保证金通过ptTotalMarginPaid来计算*/
	SafeFloatStr MarginReqd={0};
	if(ApiFunc::ptTotalMarginPaid)
		ApiFunc::ptTotalMarginPaid(&ExchangeName,&ContractName,&ContractDate,&tradeAccount,(FloatPtr)&MarginReqd);
	sprintf_s(Field.MarginPaid,sizeof(Field.MarginPaid)-1,"%.2f",atof(MarginReqd));
	/*持仓占用*/
	SafeFloatStr tempExposure={0};
	if(ApiFunc::ptOpenPositionExposure)
		ApiFunc::ptOpenPositionExposure(&ExchangeName, &ContractName, &ContractDate, &tradeAccount,(FloatPtr)&tempExposure);
	sprintf_s(Field.Exposure,sizeof(Field.Exposure)-1,"%.2f",atof(tempExposure)*Field.DynamicProfit/100.0);
	/*剩余购买力通过ptBuyingPowerRemaining获取百分比数据*/
	SafeFloatStr BPRemaining={0};
	if(ApiFunc::ptBuyingPowerRemaining)
		ApiFunc::ptBuyingPowerRemaining(&ExchangeName,&ContractName,&ContractDate,&tradeAccount,(FloatPtr)&BPRemaining);
	sprintf_s(Field.BPRemaining,sizeof(Field.BPRemaining)-1,"%.2f",atof(BPRemaining)*Field.DynamicProfit/100.0);
    /*已占用购买力*/
	SafeFloatStr tempBPUsed={0};
	if(ApiFunc::ptBuyingPowerUsed)
		ApiFunc::ptBuyingPowerUsed(&ExchangeName, &ContractName, &ContractDate, &tradeAccount,(FloatPtr)&tempBPUsed);
	sprintf_s(Field.BPUsed,sizeof(Field.BPUsed)-1,"%.2f",atof(tempBPUsed)*Field.DynamicProfit/100.0);
	/*盈亏比例=(持仓盈亏+平仓盈亏)/静态权益*/
	double burnrate=(Field.PositionProfit+Field.CloseProfit)*100.0/Field.StaticProfit;
	sprintf_s(Field.BurnRate,sizeof(Field.BurnRate)-1,"%.2f%%",burnrate);
  
	/*赋值存储*/
	memcpy(&m_FundAccount,&Field,sizeof(PlatformStru_TradingAccountInfo));

	return 0;
}

///获取账户资金文本信息
int CPatsystemApi::GetAccountText(std::string& outData,int language)
{
	int ret=0;
	std::string strTemp,strBlank,strSeparator;
	strBlank.append(4,'  ');
	strSeparator.append(50,'=');
	strSeparator+="\r\n";
	outData+=strSeparator;
	char temp[100]; 
	switch(language)
	{
	case LANGUAGE_CHINESE:
		sprintf_s(temp,sizeof(temp)-1,"  当前保证金总额:                          %.2f\r\n",m_FundAccount.CurrMargin);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  剩余购买力:                              %s\r\n",m_FundAccount.BPRemaining);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  合约占用的资金:                          %s\r\n",m_FundAccount.BPUsed);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  合约占用资金比率                        %s\r\n",m_FundAccount.Exposure);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  合约的盈亏比率:                          %s\r\n",m_FundAccount.BurnRate);
		outData+=temp;
		break;
	case LANGUAGE_ENGLISH:
		sprintf_s(temp,sizeof(temp)-1,"  Current Margin:                          %.2f\r\n",m_FundAccount.CurrMargin);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  BPRemaining:                              %s\r\n",m_FundAccount.BPRemaining);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  BPUsed:                          %s\r\n",m_FundAccount.BPUsed);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  OpenPositionExposure                        %s\r\n",m_FundAccount.Exposure);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  PLBurnRate:                          %s\r\n",m_FundAccount.BurnRate);
		outData+=temp;
		break;
	case LANGUAGE_TRADITION:
		sprintf_s(temp,sizeof(temp)-1,"  當前保證金總額:                          %.2f\r\n",m_FundAccount.CurrMargin);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  剩餘購買力:                              %s\r\n",m_FundAccount.BPRemaining);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  合約佔用的資金:                          %s\r\n",m_FundAccount.BPUsed);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  合約佔用資金比率                       %s\r\n",m_FundAccount.Exposure);
		outData+=temp;
		sprintf_s(temp,sizeof(temp)-1,"  合約的盈虧比率:                          %s\r\n",m_FundAccount.BurnRate);
		outData+=temp;
		break;
	default:
		break;
	}
	outData+=strSeparator;


	return ret;   
}


///请求查询投资者
int CPatsystemApi::ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID)
{
	return 0;
}


///请求查询交易编码
int CPatsystemApi::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID)
{
	return 0;
}


///请求查询合约保证金率
int CPatsystemApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
{
	return 0;
}


///请求查询合约手续费率
int CPatsystemApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
	return 0;
}


///请求查询交易所
int CPatsystemApi::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID)
{
	return 0;
}
///请求查询合约
int CPatsystemApi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	if(g_ThreadStatus!=THREAD_STATUS_EXIT)
	{
		InterlockedExchange(&g_ThreadStatus,THREAD_STATUS_QRY_INST);
	}
	return 0;
}


///请求查询行情
int CPatsystemApi::ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID)
{
	return 0;
}


///请求查询投资者结算结果
int CPatsystemApi::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID)
{
	return 0;
}


///请求查询转帐银行
int CPatsystemApi::ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID)
{
	return 0;
}

///请求查询投资者持仓
int CPatsystemApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID)
{
	std::vector<PlatformStru_Position> vecPosition;
	GetAllContractPosition(vecPosition);

	if ( vecPosition.empty() )
	{
		PlatformStru_Position field;
		//添加参数
		CThostFtdcRspInfoField  RspInfoField;
		RspInfoField.ErrorID=0;
		strcpy(RspInfoField.ErrorMsg,"         ");
		m_pTraderSpi->OnRspQryInvestorPosition(NULL,&RspInfoField,nRequestID,true);
		return 0;
	}

	for ( size_t i = 0; i < vecPosition.size(); i++)
	{
		CThostFtdcRspInfoField  RspInfoField;
		RspInfoField.ErrorID=0;
		strcpy(RspInfoField.ErrorMsg,"         ");
		m_pTraderSpi->OnRspQryInvestorPosition(&(vecPosition[i]),&RspInfoField,nRequestID,i+1 == vecPosition.size());
	}
	return 0;
}

///请求查询投资者持仓明细
int CPatsystemApi::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID)
{
	PlatformStru_Position field;
	//添加参数
	CThostFtdcRspInfoField  RspInfoField;
	RspInfoField.ErrorID=0;
	strcpy(RspInfoField.ErrorMsg,"         ");
	m_pTraderSpi->OnRspQryInvestorPositionDetail(NULL,&RspInfoField,nRequestID,true);
	return 0;
}


///请求查询客户通知
int CPatsystemApi::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID)
{
	return 0;
}


///请求查询结算信息确认
int CPatsystemApi::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID)
{
	CThostFtdcSettlementInfoConfirmField field;
	memset(&field,0,sizeof(field));
	//添加参数
	CThostFtdcRspInfoField  RspInfoField;
	RspInfoField.ErrorID=0;
	SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");

	m_pTraderSpi->OnRspQrySettlementInfoConfirm(&field,&RspInfoField,nRequestID,true);
	return 0;
}


///请求查询投资者持仓明细
int CPatsystemApi::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID)
{
	//CThostFtdcInvestorPositionField field={0};
	//添加参数
	CThostFtdcRspInfoField  RspInfoField={0};
	RspInfoField.ErrorID=0;
	strcpy(RspInfoField.ErrorMsg,"         ");
	m_pTraderSpi->OnRspQryInvestorPosition(NULL,&RspInfoField,nRequestID,true);
	return 0;
}


///请求查询保证金监管系统经纪公司资金账户密钥
int CPatsystemApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID)
{
	return 0;
}


///请求查询仓单折抵信息
int CPatsystemApi::ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID)
{
	return 0;
}


///请求查询转帐流水
int CPatsystemApi::ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID)
{
	return 0;
}


///请求查询银期签约关系
int CPatsystemApi::ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID)
{
	return 0;
}


///请求查询签约银行
int CPatsystemApi::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID)
{
	return 0;
}


///请求查询预埋单
int CPatsystemApi::ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID)
{
	return 0;
}


///请求查询预埋撤单
int CPatsystemApi::ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID)
{
	return 0;
}


///请求查询交易通知
int CPatsystemApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID)
{
	return 0;
}


///请求查询经纪公司交易参数
int CPatsystemApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID)
{
	return 0;
}


///请求查询经纪公司交易算法
int CPatsystemApi::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID)
{
	return 0;
}


///期货发起银行资金转期货请求
int CPatsystemApi::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	return 0;
}


///期货发起期货资金转银行请求
int CPatsystemApi::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID)
{
	return 0;
}


///期货发起查询银行余额请求
int CPatsystemApi::ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID)
{
	return 0;
}
void CPatsystemApi::onHostLinkStateChange(LinkStateStructPtr data)
{
	DLL_TRACE("onHostLinkStateChange\nOldState=%d,NewState=%d\n",data->OldState,data->NewState);
	//解决交易所发回无效状态
	g_PatsystemApi->m_HostLinkState=data->NewState;
	if(g_PatsystemApi->m_HostLinkState>ptLinkInvalid
		||g_PatsystemApi->m_HostLinkState<ptLinkOpened)
		g_PatsystemApi->m_HostLinkState=ptLinkInvalid;
	if(data->NewState==ptLinkClosed||data->NewState==ptLinkInvalid)
	{
		g_PatsystemApi->m_pTraderSpi->OnFrontDisconnected(data->NewState);
		g_PatsystemApi->m_pMdSpi->OnFrontDisconnected(data->NewState);
	} 
	if(data->NewState==ptLinkConnected)
	{
		if(g_PatsystemApi->bFirstLinkConnected)
		{
			if(g_PatsystemApi->m_ClientType==DemoClient)
				g_PatsystemApi->FireLogon();
			else if(g_ThreadStatus!=THREAD_STATUS_EXIT)
			{
				InterlockedExchange(&g_ThreadStatus,THREAD_STATUS_ENTER_WAITING);
			}

			g_PatsystemApi->bFirstLinkConnected=false;
		}
	}
}
void CPatsystemApi::onPriceLinkStateChange(LinkStateStructPtr data)
{
	DLL_TRACE("onPriceLinkStateChange\nOldState=%d,NewState=%d\n",data->OldState,data->NewState);
	//解决交易所发回无效状态
	g_PatsystemApi->m_PriceLinkState=data->NewState;
	if(g_PatsystemApi->m_PriceLinkState>ptLinkInvalid
		||g_PatsystemApi->m_PriceLinkState<ptLinkOpened)
		g_PatsystemApi->m_PriceLinkState=ptLinkInvalid;
}
void CPatsystemApi::onLogonStatus()
{
	ApiFunc::ptGetLogonStatus(&g_PatsystemApi->m_logonStatus);	
	DLL_TRACE("ptGetLogonStatus()=%d\n",g_PatsystemApi->m_logonStatus.Status);
	if(ptLogonSucceeded!=g_PatsystemApi->m_logonStatus.Status)
		g_PatsystemApi->DoLogon(g_PatsystemApi->m_logonStatus.Status);

}

void CPatsystemApi::onDataDLComplete()
{
	//登陆成功
	DLL_TRACE("onDataDLComplete()\n");
	g_PatsystemApi->m_AccountList.clear();
	EnterCriticalSection(&g_CS_AccountList);
	TraderAcctStruct TraderAcct;
	int CountTraders=0;
	ApiFunc::ptCountTraders(&CountTraders);
	for(int cLoop=0;cLoop<CountTraders;cLoop++)
	{
		ApiFunc::ptGetTrader(cLoop,&TraderAcct);
		g_PatsystemApi->m_AccountList.push_back(TraderAcct);	
	}
	LeaveCriticalSection(&g_CS_AccountList);
	g_PatsystemApi->DoLogon(ptLogonSucceeded);
}
int CPatsystemApi::GetAccountList(std::vector<string>& AcctList)
{
	EnterCriticalSection(&g_CS_AccountList);
	int iRet=0;
	std::list<TraderAcctStruct>::iterator it = m_AccountList.begin();   
	for(it;it!=m_AccountList.end();++it)
	{
		iRet++;
        AcctList.push_back(it->TraderAccount);
	}
	LeaveCriticalSection(&g_CS_AccountList);
	return iRet>0?iRet:-1;
}
int CPatsystemApi::GetExchangeIDs(vector<string>& outData)
{
	int count=0;
	outData.clear();
	if(ApiFunc::ptCountExchanges(&count)==ptSuccess)
	{
		ExchangeStruct exch;
		for(int i=0;i<count;i++)
		{
			if(ApiFunc::ptGetExchange(i,&exch)==ptSuccess)
			{
				outData.push_back(exch.Name);
			}
		}
	}
	return count;
}
void CPatsystemApi::onForcedLogout()
{
	DLL_TRACE("onForcedLogout\n");
}
void CPatsystemApi::onContractAdded(ContractUpdStructPtr data)
{
	DLL_TRACE("onContractAdded\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
}

void CPatsystemApi::onContractDeleted(ContractUpdStructPtr data)
{
	DLL_TRACE("onContractDeleted\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
}

void CPatsystemApi::onStatusChange(StatusUpdStructPtr data)
{
	DLL_TRACE("onStatusChange\nExchangeName=%s,ContractName=%s,ContractDate=%s,Status=%d\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate,
		data->Status
		);
}
void CPatsystemApi::onFill(FillUpdStructPtr data)
{
	DLL_TRACE("onFill\nOrderID=%s,FillID=%s\n",data->OrderID,data->FillID);
	FillStruct FillDetail;
	int OFSequence=1;
	if ( ptSuccess == ApiFunc::ptGetFillByID((FillIDPtr)data->FillID,&FillDetail))
	{
        g_PatsystemApi->m_PatsPositionItems.insert(PatsPositionItem(
            FillDetail.TraderAccount,
            FillDetail.ExchangeName,
            FillDetail.ContractName,
            FillDetail.ContractDate));
		PlatformStru_TradeInfo Field;
		//memset(&Field,0,sizeof(PlatformStru_TradeInfo));

		SAFE_STR_COPY(Field.szAccount, sizeof(Field.szAccount), FillDetail.TraderAccount);
        sprintf_s(Field.OrderLocalID,sizeof(Field.OrderLocalID),"%d",FillDetail.Index);
		strcpy(Field.OrderSysID,FillDetail.OrderID);
		memcpy(Field.TradeID,FillDetail.ExchangeFillID, sizeof(Field.TradeID)-1);
		//memcpy(Field.TradeID,FillDetail.FillId+strlen(FillDetail.FillId)-(sizeof(Field.TradeID)-1), sizeof(Field.TradeID)-1);
		SAFE_STR_COPY(Field.ExchangeID, sizeof(Field.ExchangeID),FillDetail.ExchangeName);
		std::string strInstrumentID = g_PatsystemApi->MakeLongInstrumentID( FillDetail.ExchangeName, FillDetail.ContractName, FillDetail.ContractDate);
		SAFE_STR_COPY(Field.InstrumentID, sizeof(Field.InstrumentID), strInstrumentID.c_str());
		if(FillDetail.BuyOrSell=='B')
			Field.Direction=THOST_FTDC_D_Buy;
		else if(FillDetail.BuyOrSell=='S')
			Field.Direction=THOST_FTDC_D_Sell;

		Field.Price = atof( FillDetail.Price);
		Field.Volume=FillDetail.Lots;

		Field.OffsetFlag=THOST_FTDC_OF_Open;//默认为开仓
		Field.HedgeFlag = THOST_FTDC_HF_Speculation;
		SAFE_STR_COPY(Field.TradeDate, sizeof(Field.TradeDate), FillDetail.DateFilled);
		SAFE_STR_COPY(Field.TradingDay, sizeof(Field.TradingDay), FillDetail.DateFilled);
		SAFE_STR_COPY(Field.TradeTime, sizeof(Field.TradeTime), FillDetail.TimeFilled);

		//strcpy(Field.OrderRef,FillDetail.LocalOrderID);
		//Field.FrontID=FillDetail.Xref;
		//Field.SessionID=FillDetail.OFSeqNumber;

		g_PatsystemApi->m_pTraderSpi->OnRtnTrade(&Field);
	}
}
void CPatsystemApi::onMessage(MsgIDPtr MsgID)
{
	MessageStruct msg;
	ApiFunc::ptGetUsrMsgByID(MsgID,&msg);
    ApiFunc::ptAcknowledgeUsrMsg(MsgID);
	DLL_TRACE("onMessage\nMsgID=%s,MsgText=%s,IsAlert=%c,Status=%c\n",msg.MsgID,msg.MsgText,msg.IsAlert,msg.Status);
	g_PatsystemApi->m_pTraderSpi->OnRtnAlertMessage((PlatformStru_MessageStruct*)&msg);
}

void CPatsystemApi::onOrder(OrderUpdStructPtr data)
{
	DLL_TRACE("onOrder\nOrderID=%s,OldOrderID=%s,OrderStatus=%d,OFSeqNumber=%d,OrderTypeId=%d\n",
		data->OrderID,
		data->OldOrderID,
		data->OrderStatus,
		data->OFSeqNumber,
		data->OrderTypeId
		);
	OrderDetailStruct OrderDetail;
	ZeroMemory(&OrderDetail, sizeof(OrderDetail));
	int OFSequence=0;
	if ( ptSuccess == ApiFunc::ptGetOrderByID((OrderIDPtr)data->OrderID,&OrderDetail,OFSequence))
	{
		PlatformStru_OrderInfo Field;
		//ZeroMemory(&Field, sizeof(PlatformStru_OrderInfo));

		Field.BrokerOrderSeq=OrderDetail.Index;
		strcpy(Field.OrderSysID,OrderDetail.OrderID);
        strcpy(Field.RelativeOrderSysID,OrderDetail.ExchOrderID);
		strcpy(Field.ExchangeID,OrderDetail.ExchangeName);
		std::string strInstrumentID = g_PatsystemApi->MakeLongInstrumentID( OrderDetail.ExchangeName, OrderDetail.ContractName, OrderDetail.ContractDate);
		strcpy(Field.InstrumentID, strInstrumentID.c_str());
		if(OrderDetail.BuyOrSell=='B')
            Field.Direction=THOST_FTDC_D_Buy;
		else if(OrderDetail.BuyOrSell=='S')
            Field.Direction=THOST_FTDC_D_Sell;
		if(strcmp(OrderDetail.OrderType,"Market")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_AnyPrice; 
			//Field.LimitPrice=atof(OrderDetail.AveragePrice);
			Field.ContingentCondition=THOST_FTDC_CC_Immediately;
		}
		else if(strcmp(OrderDetail.OrderType,"Limit")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			Field.LimitPrice=atof(OrderDetail.Price);
			Field.ContingentCondition=THOST_FTDC_CC_Immediately;
		}
		else if(strcmp(OrderDetail.OrderType,"Stop")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
			//Field.LimitPrice=atof(OrderDetail.AveragePrice);
			Field.StopPrice=atof(OrderDetail.Price);
			if(OrderDetail.BuyOrSell=='B')
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
			else 
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
		}
		else if(strcmp(OrderDetail.OrderType,"Stop Limit")==0)
		{
			Field.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			Field.StopPrice=atof(OrderDetail.Price);
			Field.LimitPrice=atof(OrderDetail.Price2);
			if(OrderDetail.BuyOrSell=='B')
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
			else 
				Field.ContingentCondition=THOST_FTDC_CC_Touch;
		}
        Field.VolumeTotalOriginal=OrderDetail.Lots;//下单手数
		Field.VolumeTraded=OrderDetail.AmountFilled;//成交量
		Field.VolumeTotal=Field.VolumeTotalOriginal-Field.VolumeTraded;//未成交量
		Field.OrderStatus=OrderStatusCode[OrderDetail.Status];
		///报单提交状态---指令结果
		if(OrderDetail.Status==ptRejected) {
			Field.OrderSubmitStatus=THOST_FTDC_OSS_InsertRejected;
		}
		else if(OrderDetail.Status==ptSent || OrderDetail.Status==ptWorking) {
			Field.OrderSubmitStatus=THOST_FTDC_OSS_InsertSubmitted;
		}
		else {
			Field.OrderSubmitStatus=THOST_FTDC_OSS_Accepted;
		}

		SAFE_STR_COPY(Field.StatusMsg,sizeof(Field.StatusMsg),OrderStatusMsg[OrderDetail.Status]);
		if(OrderDetail.OpenOrClose=='O')
            Field.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
		else if(OrderDetail.OpenOrClose=='C')
            Field.CombOffsetFlag[0]=THOST_FTDC_OF_Close;
		strcpy(Field.InsertDate,OrderDetail.DateSent);
        //strcpy(Field.InsertTime,OrderDetail.TimeSent);
		sprintf_s(Field.InsertTime,sizeof(Field.InsertTime),"%.2s:%.2s:%.2s",OrderDetail.TimeSent,OrderDetail.TimeSent+2,OrderDetail.TimeSent+4);
		strcpy(Field.ActiveTime,OrderDetail.TimeExchRecd);
        //strcpy(Field.UpdateTime,OrderDetail.TimeExchAckn);
		if(strlen(OrderDetail.TimeExchAckn)>1 && (OrderDetail.Status==ptFilled ||OrderDetail.Status==ptPartFilled))
		    sprintf_s(Field.UpdateTime,sizeof(Field.UpdateTime),"%.2s:%.2s:%.2s",OrderDetail.TimeExchAckn,OrderDetail.TimeExchAckn+2,OrderDetail.TimeExchAckn+4);
		Field.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
        if((OrderDetail.Status==ptFilled ||OrderDetail.Status==ptPartFilled))
            Field.AvgPrice = atof( OrderDetail.AveragePrice );
		else
			Field.AvgPrice = 0;
		strcpy(Field.OrderRef,OrderDetail.OrderID);
		strcpy(Field.Price2,OrderDetail.Price2);
		//sscanf(OrderDetail.ExchOrderID,"%d",&Field.FrontID);
		//sscanf(OrderDetail.ExecutionID,"%d",&Field.SessionID);
		//Field.SessionID=OrderDetail.NoOfFills;
		//Field.SequenceNo=OrderDetail.Index;
		// 复制交易帐号
		strcpy(Field.szAccount, OrderDetail.TraderAccount);

		g_PatsystemApi->m_pTraderSpi->OnRtnOrder(&Field);
	}
}
void CPatsystemApi::onPriceUpdate(PriceUpdStructPtr data)
{
#ifdef _USE_MD_TRACE
	DLL_TRACE("onPriceUpdate\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
#endif
	PriceStruct newPrice;
	if ( ptSuccess == ApiFunc::ptGetPriceForContract(&data->ExchangeName, &data->ContractName, &data->ContractDate, &newPrice))
	{
		std::string strInstrumentID = g_PatsystemApi->MakeLongInstrumentID( data->ExchangeName, data->ContractName, data->ContractDate );
		PlatformStru_DepthMarketData* pDepthMarketData = NULL;
		std::map<std::string, PlatformStru_DepthMarketData*>::iterator it = g_PatsystemApi->m_mapInstrumentPrices.find( strInstrumentID );
		if ( it != g_PatsystemApi->m_mapInstrumentPrices.end() && it->second != NULL )
		{
			pDepthMarketData = it->second;
		}
		else
		{
			pDepthMarketData = new PlatformStru_DepthMarketData();	
			InvalidateDepthMarketData(pDepthMarketData);
			g_PatsystemApi->m_mapInstrumentPrices[strInstrumentID] = pDepthMarketData;
			//合约代码
			strcpy(pDepthMarketData->InstrumentID, strInstrumentID.c_str());
			//交易所
			strcpy(pDepthMarketData->ExchangeID, data->ExchangeName);
			strcpy(pDepthMarketData->ExchangeInstID, data->ExchangeName);
		}

		//价量更新
		int nChangeMask = newPrice.ChangeMask;
		if ( nChangeMask&ptChangeBid )
		{
			pDepthMarketData->BidPrice1 = atof( newPrice.Bid.Price);
			pDepthMarketData->BidVolume1 = newPrice.Bid.Volume;
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), newPrice.Bid );
		}
		if ( nChangeMask&ptChangeOffer)
		{
			pDepthMarketData->AskPrice1 = atof( newPrice.Offer.Price );
			pDepthMarketData->AskVolume1 = newPrice.Offer.Volume;
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), newPrice.Offer );
		}
		if ( nChangeMask&ptChangeImpliedBid)
		{
		}
		if ( nChangeMask&ptChangeImpliedOffer)
		{
		}
		if ( nChangeMask&ptChangeRFQ)
		{
		}
		if ( nChangeMask&ptChangeLast)
		{
			PriceDetailStruct last0 = newPrice.Last0;
			pDepthMarketData->LastPrice = atof(last0.Price);
			pDepthMarketData->NewVolume=last0.Volume;
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), last0 );
		}
		if ( nChangeMask&ptChangeTotal)
		{
			PriceDetailStruct total = newPrice.Total;
			pDepthMarketData->Turnover = atof( total.Price);
			pDepthMarketData->Volume = total.Volume;
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), total );
		}
		if ( nChangeMask&ptChangeHigh)
		{
			PriceDetailStruct high = newPrice.High;
			pDepthMarketData->HighestPrice = atof( high.Price );
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), high );
		}
		if ( nChangeMask&ptChangeLow)
		{
			PriceDetailStruct low = newPrice.Low;
			pDepthMarketData->LowestPrice = atof( low.Price );
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), low );
		}
		if ( nChangeMask&ptChangeOpening)
		{
			PriceDetailStruct opening = newPrice.Opening;
			pDepthMarketData->OpenPrice = atof( opening.Price );
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), opening );
		}
		if ( nChangeMask&ptChangeClosing)
		{
			PriceDetailStruct closing = newPrice.Closing;
			pDepthMarketData->PreClosePrice = atof( closing.Price );
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), closing );
		}
		if ( nChangeMask&ptChangeBidDOM)
		{
			pDepthMarketData->BidPrice1 = atof( newPrice.BidDOM0.Price );
			pDepthMarketData->BidVolume1 = newPrice.BidDOM0.Volume;
			pDepthMarketData->BidPrice2 = atof( newPrice.BidDOM1.Price );
			pDepthMarketData->BidVolume2 = newPrice.BidDOM1.Volume;
			pDepthMarketData->BidPrice3 = atof( newPrice.BidDOM2.Price );
			pDepthMarketData->BidVolume3 = newPrice.BidDOM2.Volume;
			pDepthMarketData->BidPrice4 = atof( newPrice.BidDOM3.Price );
			pDepthMarketData->BidVolume4 = newPrice.BidDOM3.Volume;
			pDepthMarketData->BidPrice5 = atof( newPrice.BidDOM4.Price );
			pDepthMarketData->BidVolume5 = newPrice.BidDOM4.Volume;

			pDepthMarketData->BidPrice6 = atof( newPrice.BidDOM5.Price );
			pDepthMarketData->BidVolume6 = newPrice.BidDOM5.Volume;
			pDepthMarketData->BidPrice7 = atof( newPrice.BidDOM6.Price );
			pDepthMarketData->BidVolume7 = newPrice.BidDOM6.Volume;
			pDepthMarketData->BidPrice8 = atof( newPrice.BidDOM7.Price );
			pDepthMarketData->BidVolume8 = newPrice.BidDOM7.Volume;
			pDepthMarketData->BidPrice9 = atof( newPrice.BidDOM8.Price );
			pDepthMarketData->BidVolume9 = newPrice.BidDOM8.Volume;
			pDepthMarketData->BidPrice10 = atof( newPrice.BidDOM9.Price );
			pDepthMarketData->BidVolume10 = newPrice.BidDOM9.Volume;

			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), newPrice.BidDOM0 );
		}
		if ( nChangeMask&ptChangeOfferDOM)
		{
			pDepthMarketData->AskPrice1= atof( newPrice.OfferDOM0.Price );
			pDepthMarketData->AskVolume1 = newPrice.OfferDOM0.Volume;
			pDepthMarketData->AskPrice2 = atof( newPrice.OfferDOM1.Price );
			pDepthMarketData->AskVolume2 = newPrice.OfferDOM1.Volume;
			pDepthMarketData->AskPrice3 = atof( newPrice.OfferDOM2.Price );
			pDepthMarketData->AskVolume3 = newPrice.OfferDOM2.Volume;
			pDepthMarketData->AskPrice4 = atof( newPrice.OfferDOM3.Price );
			pDepthMarketData->AskVolume4 = newPrice.OfferDOM3.Volume;
			pDepthMarketData->AskPrice5 = atof( newPrice.OfferDOM4.Price );
			pDepthMarketData->AskVolume5 = newPrice.OfferDOM4.Volume;

			pDepthMarketData->AskPrice6= atof( newPrice.OfferDOM5.Price );
			pDepthMarketData->AskVolume6 = newPrice.OfferDOM5.Volume;
			pDepthMarketData->AskPrice7 = atof( newPrice.OfferDOM6.Price );
			pDepthMarketData->AskVolume7 = newPrice.OfferDOM6.Volume;
			pDepthMarketData->AskPrice8 = atof( newPrice.OfferDOM7.Price );
			pDepthMarketData->AskVolume8 = newPrice.OfferDOM7.Volume;
			pDepthMarketData->AskPrice9 = atof( newPrice.OfferDOM8.Price );
			pDepthMarketData->AskVolume9 = newPrice.OfferDOM8.Volume;
			pDepthMarketData->AskPrice10 = atof( newPrice.OfferDOM9.Price );
			pDepthMarketData->AskVolume10 = newPrice.OfferDOM9.Volume;

			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), newPrice.OfferDOM0 );
		}
		if ( nChangeMask&ptChangeTGE)
		{

		}
		if ( nChangeMask&ptChangeSettlement)
		{
			pDepthMarketData->PreSettlementPrice = atof( newPrice.pvCurrStl.Price );
			pDepthMarketData->SettlementPrice = atof( newPrice.pvNewStl.Price );
			g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->UpdateTime, 
				sizeof(pDepthMarketData->UpdateTime), newPrice.pvCurrStl );
		}
		if ( nChangeMask&ptChangeIndic)
		{

		}
		if ( nChangeMask&ptChangeClear)
		{

		}
#ifdef _USE_MD_TRACE
		DLL_TRACE("OnRtnDepthMarketData\r\n");
#endif
		g_PatsystemApi->m_pMdSpi->OnRtnDepthMarketData( pDepthMarketData );
	}
}

void CPatsystemApi::onTickerUpdate(TickerUpdStructPtr data)
{
	//std::string strInstrumentID = g_PatsystemApi->CombineInstrumentID( data->ExchangeName, data->ContractName, data->ContractDate );
	//PlatformStru_DepthMarketData* pDepthMarketData = NULL;
	//std::map<std::string, PlatformStru_DepthMarketData*>::iterator it = g_PatsystemApi->m_mapInstrumentPrices.find( strInstrumentID );
	//if ( it != g_PatsystemApi->m_mapInstrumentPrices.end() && it->second != NULL )
	//{
	//	pDepthMarketData = it->second;
	//}
	//else
	//{
	//	pDepthMarketData = new PlatformStru_DepthMarketData();
	//	g_PatsystemApi->m_mapInstrumentPrices[strInstrumentID] = pDepthMarketData;
	//	ZeroMemory(pDepthMarketData, sizeof(PlatformStru_DepthMarketData));
	//	//合约代码
	//	strcpy(pDepthMarketData->Thost.InstrumentID, strInstrumentID.c_str());
	//	//交易所
	//	strcpy(pDepthMarketData->Thost.ExchangeID, data->ExchangeName);
	//	strcpy(pDepthMarketData->Thost.ExchangeInstID, data->ExchangeName);
	//}

	//if ( data->Bid == 'Y')
	//{
	//	pDepthMarketData->Thost.BidPrice1 = atof( data->BidPrice);
	//	pDepthMarketData->Thost.BidVolume1 = data->BidVolume; 
	//}

	//if ( data->Offer == 'Y')
	//{
	//	pDepthMarketData->Thost.AskPrice1 = atof( data->OfferPrice);
	//	pDepthMarketData->Thost.AskVolume1 = data->OfferVolume;
	//}

	//if ( data->Last == 'Y')
	//{
	//	pDepthMarketData->Thost.LastPrice = atof( data->LastPrice );
	//}
	//
	//g_PatsystemApi->m_pMdSpi->OnRtnDepthMarketData( &pDepthMarketData->Thost );
#ifdef _USE_MD_TRACE
	DLL_TRACE("onTickerUpdata\nExchangeName=%s,ContractName=%s,ContractDate=%s,BidPrice=%s,"
		"BidVolume=%d,OfferPrice=%s,OfferVolume=%d,LastPrice=%s,LastVolume=%d,Bid=%c,Offer=%c,Last=%c\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate,
		data->BidPrice,
		data->BidVolume,
		data->OfferPrice,
		data->OfferVolume,
		data->LastPrice,
		data->LastVolume,
		data->Bid,
		data->Offer,
		data->Last
		);
#endif
}
void CPatsystemApi::onExchangeRate(ExchangeRateUpdStructPtr data)
{
	SafeFloatStr ExchRate;
	ApiFunc::ptGetExchangeRate((CurrNamePtr)data->Currency,(ExchRatePtr)ExchRate);
	DLL_TRACE("onExchangeRate\nCurrency=%s,ExchRate=%s\n",data->Currency,ExchRate);
	g_PatsystemApi->m_pTraderSpi->OnRtnExchangeRate(data->Currency);
}
vector<ConnectivityStatusUpdStruct> ConnectivityStatusUpdStructSet;
void CPatsystemApi::onConnectivityStatus(ConnectivityStatusUpdStructPtr data)
{
	ConnectivityStatus status;
	memset(&status,0,sizeof(ConnectivityStatus));
	SAFE_STR_COPY(status.DeviceType,sizeof(status.DeviceType)-1,data->DeviceType);
	SAFE_STR_COPY(status.DeviceName,sizeof(status.DeviceName)-1,data->DeviceName);
	SAFE_STR_COPY(status.Commentary,sizeof(status.Commentary)-1,data->Commentary);
	SAFE_STR_COPY(status.Status,sizeof(status.Status)-1,data->Status);
	SAFE_STR_COPY(status.Severity,sizeof(status.Severity)-1,data->Severity);
	SAFE_STR_COPY(status.ExchangeID,sizeof(status.ExchangeID)-1,data->ExchangeID);
	g_PatsystemApi->m_pTraderSpi->OnRtnConnectivityStatus(&status);
}
void CPatsystemApi::onOrderCancelFailure(OrderUpdStructPtr data)
{
	DLL_TRACE("onOrderCancelFailure\nOrderID=%s,OldOrderID=%s,OrderStatus=%d,OFSeqNumber=%d,OrderTypeId=%d\n",
		data->OrderID,
		data->OldOrderID,
		data->OrderStatus,
		data->OFSeqNumber,
		data->OrderTypeId
		);
}
void CPatsystemApi::onAtBestUpdate(AtBestUpdStructPtr data)
{
	DLL_TRACE("onAtBestUpdate\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
}
void CPatsystemApi::onMemoryWarning()
{
	DLL_TRACE("onMemoryWarning\n");
}
void CPatsystemApi::onSubscriberDepthUpdate(SubscriberDepthUpdStructPtr data)
{
	DLL_TRACE("onSubscriberDepthUpdate\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
}
void CPatsystemApi::onDOMUpdate(DOMUpdStructPtr data)
{
#ifdef _USE_MD_TRACE
	DLL_TRACE("onDOMUpdate\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
#endif
}
void CPatsystemApi::onSettlementCallback(SettlementPriceStructPtr data)
{
#ifdef _USE_MD_TRACE
	DLL_TRACE("onSettlementCallback\nExchangeName=%s,ContractName=%s,"
		"ContractDate=%s,SettlementType=%d,Price=%s,Time=%s,Date=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate,
		data->SettlementType,
		data->Price,
		data->Time,
		data->Date
		);
#endif
	//std::string strInstrumentID = g_PatsystemApi->CombineInstrumentID( data->ExchangeName, data->ContractName, data->ContractDate );
	//PlatformStru_DepthMarketData* pDepthMarketData = NULL;
	//std::map<std::string, PlatformStru_DepthMarketData*>::iterator it = g_PatsystemApi->m_mapInstrumentPrices.find( strInstrumentID );
	//if ( it != g_PatsystemApi->m_mapInstrumentPrices.end() && it->second != NULL )
	//{
	//	pDepthMarketData = it->second;
	//}
	//else
	//{
	//	pDepthMarketData = new PlatformStru_DepthMarketData();
	//	g_PatsystemApi->m_mapInstrumentPrices[strInstrumentID] = pDepthMarketData;
	//	ZeroMemory(pDepthMarketData, sizeof(PlatformStru_DepthMarketData));
	//	//合约代码
	//	strcpy(pDepthMarketData->Thost.InstrumentID, strInstrumentID.c_str());
	//	//交易所
	//	strcpy(pDepthMarketData->Thost.ExchangeID, data->ExchangeName);
	//	strcpy(pDepthMarketData->Thost.ExchangeInstID, data->ExchangeName);
	//}

	//bool bChanged = false;
	//if ( data->SettlementType == ptStlCurPrice )
	//{
	//	pDepthMarketData->Thost.PreSettlementPrice = atof( data->Price );
	//	bChanged = true;
	//}
	//else if ( data->SettlementType == ptStlNewPrice )
	//{
	//	pDepthMarketData->Thost.SettlementPrice = atof( data->Price );
	//}

	//if ( bChanged )
	//{
	//	//g_PatsystemApi->GetLastUpdateTime( pDepthMarketData->Thost.UpdateTime, 
	//	//	sizeof(pDepthMarketData->Thost.UpdateTime), newPrice.Bid );
	//	g_PatsystemApi->m_pMdSpi->OnRtnDepthMarketData( &pDepthMarketData->Thost );
	//}	
}
void CPatsystemApi::onStrategyCreateSuccess(StrategyCreateSuccessStructPtr data)
{
	DLL_TRACE("onStrategyCreateSuccess\nUserName=%s,ExchangeName=%s,ContractName=%s,ReqContractDate=%s,GenContractDate=%s\n",
		data->UserName,
		data->ExchangeName,
		data->ContractName,
		data->ReqContractDate,
		data->GenContractDate
		);
}
void CPatsystemApi::onStrategyCreateFailure(StrategyCreateFailureStructPtr data)
{
	DLL_TRACE("onStrategyCreateFailure\nUserName=%s,ExchangeName=%s,ContractName=%s,ContractDate=%s,Text=%s\n",
		data->UserName,
		data->ExchangeName,
		data->ContractName,
		data->ContractDate,
		data->Text
		);
}
void CPatsystemApi::onGenericPriceUpdate(GenericPriceStructPtr data)
{
	DLL_TRACE("onGenericPriceUpdate\nExchangeName=%s,ContractName=%s,ContractDate=%s,PriceType=%d,BuyOrSell=%d\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate,
		data->PriceType,
		data->BuyOrSell
		);
}
void CPatsystemApi::onBlankPrice(BlankPriceStructPtr data)
{
	DLL_TRACE("onBlankPrice\nExchangeName=%s,ContractName=%s,ContractDate=%s\n",
		data->ExchangeName,
		data->ContractName,
		data->ContractDate
		);
}
void CPatsystemApi::onOrderSentFailure(OrderUpdStructPtr data)
{
	DLL_TRACE("onOrderSentFailure\nOrderID=%s,OldOrderID=%s,OrderStatus=%d,OFSeqNumber=%d,OrderTypeId=%d\n",
		data->OrderID,
		data->OldOrderID,
		data->OrderStatus,
		data->OFSeqNumber,
		data->OrderTypeId
		);
}
void CPatsystemApi::onOrderQueuedFailure(OrderUpdStructPtr data)
{
	DLL_TRACE("onOrderQueuedFailure\nOrderID=%s,OldOrderID=%s,OrderStatus=%d,OFSeqNumber=%d,OrderTypeId=%d\n",
		data->OrderID,
		data->OldOrderID,
		data->OrderStatus,
		data->OFSeqNumber,
		data->OrderTypeId
		);
}
void CPatsystemApi::onExchangeUpdate( ExchangeUpdStructPtr data)
{
	DLL_TRACE("onExchangeUpdate\nExchangeName=%s\n",
		data->ExchangeName
		);
}
void CPatsystemApi::onCommodityUpdate(CommodityUpdStructPtr data)
{
	DLL_TRACE("onCommodityUpdate\nExchangeName=%s,ContractName=%s\n",
		data->ExchangeName,
		data->ContractName
		);
}
void CPatsystemApi::onPurgeCompleted( ExchangeUpdStructPtr data)
{
	DLL_TRACE("onPurgeCompleted\nExchangeName=%s\n",
		data->ExchangeName
		);
}
void CPatsystemApi::onOrderTypeUpdate(OrderTypeStructPtr data)
{
	DLL_TRACE("onOrderTypeUpdate()\nOrderType=%s,ExchangeName=%s,OrderTypeID=%d,"
		"NumPricesReqd=%d,NumVolumesReqd=%d,NumDatesReqd=%d,AutoCreated=%c,"
		"TimeTriggered=%c,RealSynthetic=%c,GTCFlag=%c,TicketType=%s,"
		"PatsOrderType=%c,AmendOTCount=%d,AlgoXML=%s\n",
		data->OrderType,
		data->ExchangeName,
		data->OrderTypeID,
		data->NumPricesReqd,
		data->NumVolumesReqd,
		data->NumDatesReqd,
		data->AutoCreated,
		data->TimeTriggered,
		data->RealSynthetic,
		data->GTCFlag,
		data->TicketType,
		data->PatsOrderType,
		data->AmendOTCount,
		data->AlgoXML
		);
}
//
void CPatsystemApi::DoLogon(BOOL bOk,CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	static int Logon_RequestID=nRequestID;
	if(bOk>=0)
	{
		CThostFtdcRspUserLoginField field;
		memset(&field,0,sizeof(field));
		CThostFtdcRspInfoField pRspInfo;
		if(bOk==ptLogonSucceeded)
		{
			pRspInfo.ErrorID=0;
			SAFE_STR_COPY(pRspInfo.ErrorMsg,sizeof(pRspInfo.ErrorMsg)-1,"         ");
		}
		else
		{
			pRspInfo.ErrorID=0x12340000+bOk;
			SAFE_STR_COPY(pRspInfo.ErrorMsg,sizeof(pRspInfo.ErrorMsg)-1,m_logonStatus.Reason);
		}
		m_pTraderSpi->OnRspUserLogin(&field,&pRspInfo,Logon_RequestID,true);
		m_pMdSpi->OnRspUserLogin(&field,&pRspInfo,Logon_RequestID,true);
	}
}
void CPatsystemApi::FireLogon()
{
	LogonStruct logonDetails;
	memset(&logonDetails,0,sizeof(logonDetails));
	if(m_ClientType==DemoClient)
	{
		SAFE_STR_COPY(logonDetails.UserID,sizeof(logonDetails.UserID)-1,Logon_UserID);//DEMO1
		SAFE_STR_COPY(logonDetails.Password,sizeof(logonDetails.Password)-1,Logon_Password);
		//SAFE_STR_COPY(logonDetails.NewPassword,sizeof(logonDetails.NewPassword)-1,Logon_Password); 
		logonDetails.Reset='Y';
	}
	else
	{
		SAFE_STR_COPY(logonDetails.UserID,sizeof(logonDetails.UserID)-1,Logon_UserID);//APPID_2139
		SAFE_STR_COPY(logonDetails.Password,sizeof(logonDetails.Password)-1,Logon_Password);//PASSWORD1
		if(!m_strNewPwd.empty())
			SAFE_STR_COPY(logonDetails.NewPassword, sizeof(logonDetails.NewPassword)-1, m_strNewPwd.c_str());
		logonDetails.Reset='Y';
		logonDetails.Reports='Y';
	}
	DLL_TRACE("ApiFunc::ptLogOn\n");
	int nStatus=ApiFunc::ptLogOn(&logonDetails);
}
void CPatsystemApi::DoQryInstrument()
{
	ContractStruct contract;
	int cLoop=0,CountContract=0;
	int nRequestID=0;
	ApiFunc::ptCountContracts(&CountContract);
	//CountContract=500;//控制总数
	DLL_TRACE("ApiFunc::ptCountContracts()=%d\n",CountContract);
	PlatformStru_InstrumentInfo field;
	CThostFtdcRspInfoField  RspInfoField;
	RspInfoField.ErrorID=0;
	SAFE_STR_COPY(RspInfoField.ErrorMsg,sizeof(RspInfoField.ErrorMsg)-1,"         ");
	int countindex=0;

	
	//TraderAcctStruct TraderAcct;
	//int CountTraders=0;
	//ApiFunc::ptCountTraders (&CountTraders);
	//for(int cLoop=0;cLoop<CountTraders;cLoop++)
	//{
	//	ApiFunc::ptGetTrader(cLoop,&TraderAcct);
	//	m_AccountList.push_back(TraderAcct);	
	//}

	std::list<TraderAcctStruct>::iterator it=m_AccountList.begin();
	strcpy(m_accountName,(*it).TraderAccount);
	for(cLoop=0;cLoop<CountContract;cLoop++)
	{
		ApiFunc::ptGetContract(cLoop,&contract);
		memset(&field,0,sizeof(PlatformStru_InstrumentInfo));
		std::string ExchangeName=contract.ExchangeName;
		std::string strInstrumentID = MakeLongInstrumentID(contract.ExchangeName,contract.ContractName,contract.ContractDate);
		SAFE_STR_COPY(field.InstrumentID,sizeof(field.InstrumentID),strInstrumentID.c_str());
		SAFE_STR_COPY(field.InstrumentName,sizeof(field.InstrumentName),strInstrumentID.c_str());
		m_mapInstrumentDetail[strInstrumentID] = contract;
		SAFE_STR_COPY(field.ExchangeID,sizeof(field.ExchangeID),contract.ExchangeName);
		SAFE_STR_COPY(field.ExchangeInstID,sizeof(field.ExchangeInstID),contract.ExchangeName);
		SAFE_STR_COPY(field.ExpireDate,sizeof(field.ExpireDate),contract.ExpiryDate);
		sscanf_s(contract.TickSize,"%lf",&field.PriceTick);
		SAFE_STR_COPY(field.TickSize,sizeof(field.TickSize),contract.TickSize);
		field.TicksPerPoint=contract.TicksPerPoint;
		//field.ThostInstrument.VolumeMultiple=contract.TicksPerPoint;
		field.ProductClass=THOST_FTDC_PC_Futures;
		SAFE_STR_COPY(field.ProductID,sizeof(field.ProductID),contract.ContractName);
		if(m_ClientType!=DemoClient)
		{
			char MarginReqd[1024];
			memset(MarginReqd,0,sizeof(MarginReqd));
			int lots=1;
			//char orderId[11];
			//char price[21];
			int ret;
			/*	ret=ApiFunc::ptMarginForTrade ((ExchNamePtr)contract->ExchangeName,(ConNamePtr)contract->ContractName,(ConDatePtr)contract->ContractDate,
			(TraderPtr)m_accountName,(PINT)lots,(OrderIDPtr)orderId,(PricePtr)price,(FloatPtr)MarginReqd);*/
			ret=ApiFunc::ptGetMarginPerLot ((ExchNamePtr)contract.ExchangeName,(ConNamePtr)contract.ContractName,(ConDatePtr)contract.ContractDate,(TraderPtr)m_accountName,(FloatPtr)MarginReqd);
			//ret=ApiFunc::ptTotalMarginPaid ((ExchNamePtr)contract->ExchangeName,(ConNamePtr)contract->ContractName,(ConDatePtr)contract->ContractDate,(TraderPtr)m_accountName,(FloatPtr)MarginReqd);
			sscanf_s(MarginReqd,"%g",&field.LongMarginRatio);
			sscanf_s(MarginReqd,"%g",&field.ShortMarginRatio);
			//field.PriceTick=1.0f;
			//field.PriceTick/=100.0f;
		}
		CommodityStruct commodity;
		if( ptSuccess == ApiFunc::ptGetCommodityByName(&contract.ExchangeName, &contract.ContractName,&commodity))
		{
			strcpy(field.Currency, commodity.Currency);
			field.VolumeMultiple = atoi( commodity.OnePoint );
		}

		m_pTraderSpi->OnRspQryInstrument(&field,&RspInfoField,nRequestID,cLoop+1==CountContract);
	}
}
std::string CPatsystemApi::MakeLongInstrumentID(std::string exchange, std::string name, std::string date)
{
	std::string strInstrumentID;
	if(!name.empty())
	{
		strInstrumentID = name + "-" + date + "@" + exchange;
		for(int pos=strInstrumentID.length()-1;pos>=0;pos--) 
			if(strInstrumentID[pos]==' ')strInstrumentID[pos]='-';
	}
	return strInstrumentID;
}

void CPatsystemApi::GetLastUpdateTime( char* pBuf, int nLen, const PriceDetailStruct& priceDetail )
{
	if (pBuf )
	{
		sprintf_s(pBuf,nLen,"%02d:%02d:%02d", 
			priceDetail.Hour, 
			priceDetail.Minute, 
			priceDetail.Second );
	}
}
std::string CPatsystemApi::GetLogonStatusString(BYTE LogonStatus)
{
	if(LogonStatus==ptLogonFailed) return "ptLogonFailed";
	if(LogonStatus==ptLogonSucceeded) return "ptLogonSucceeded";
	if(LogonStatus==ptForcedOut) return "ptForcedOut";
	if(LogonStatus==ptObsoleteVers) return "ptObsoleteVers";
	if(LogonStatus==ptWrongEnv) return "ptWrongEnv";
	if(LogonStatus==ptDatabaseErr) return "ptDatabaseErr";
	if(LogonStatus==ptInvalidUser) return "ptInvalidUser";
	if(LogonStatus==ptLogonRejected) return "ptLogonRejected";
	if(LogonStatus==ptInvalidAppl) return "ptInvalidAppl";
	if(LogonStatus==ptLoggedOn) return "ptLoggedOn";
	if(LogonStatus==ptInvalidLogonState) return "ptInvalidLogonState";
	return "";
}
void CPatsystemApi::initOrderTypeMap()
{
	mapOrderType["Market"]=PATSYSTEM_FTDC_OPT_Market;
	mapOrderType["Limit"]=PATSYSTEM_FTDC_OPT_Limit;
	mapOrderType["Limit Fill and Kill"]=PATSYSTEM_FTDC_OPT_LimitFAK;
	mapOrderType["Limit Fill or Kill"]=PATSYSTEM_FTDC_OPT_LimitFOK;
	mapOrderType["Stop"]=PATSYSTEM_FTDC_OPT_Stop;
	mapOrderType["Synthetic Stop"]=PATSYSTEM_FTDC_OPT_SynthStop;
	mapOrderType["Synthetic Stop Limit"]=PATSYSTEM_FTDC_OPT_SynthStopLimit;
	mapOrderType["Market if Touched"]=PATSYSTEM_FTDC_OPT_MIT;
	mapOrderType["Synthetic Market if Touched"]=PATSYSTEM_FTDC_OPT_SynthMIT;
	mapOrderType["Market Fill or Kill"]=PATSYSTEM_FTDC_OPT_MarketFOK;
	mapOrderType["Market on Open"]=PATSYSTEM_FTDC_OPT_MOO;
	mapOrderType["Crossing Order"]=PATSYSTEM_FTDC_OPT_CrossingBatchType;
	mapOrderType["Basis Batch Order"]=PATSYSTEM_FTDC_OPT_BasisBatchType;
	mapOrderType["Block wholesale trade order"]=PATSYSTEM_FTDC_OPT_BlockBatchType;
	mapOrderType["Against Actual trade order"]=PATSYSTEM_FTDC_OPT_AABatchType;
	mapOrderType["Immediate or Cancel"]=PATSYSTEM_FTDC_OPT_IOC;
	mapOrderType["Stop Rise"]=PATSYSTEM_FTDC_OPT_StopRise;
	mapOrderType["Stop Fall"]=PATSYSTEM_FTDC_OPT_StopFall;
	mapOrderType["Request for Quote"]=PATSYSTEM_FTDC_OPT_RFQ;
	mapOrderType["Stop Loss"]=PATSYSTEM_FTDC_OPT_StopLoss;

}

bool CPatsystemApi::GetPositionInfo( const std::string& strAccount, const std::string& strInstrumentID, PlatformStru_Position& field )
{
	ContractStruct contract;
	std::map<std::string, ContractStruct>::iterator it = m_mapInstrumentDetail.find( strInstrumentID );
	if ( it == m_mapInstrumentDetail.end())
	{
		return false;
	}
	else
	{
		contract = it->second;
	}

	PositionStruct position;
	TraderStr tradeAccount;
	memset(tradeAccount, 0, sizeof(tradeAccount));
	strcpy(tradeAccount, strAccount.c_str());
	int nStatus = ApiFunc::ptGetContractPosition(&contract.ExchangeName, &contract.ContractName, &contract.ContractDate, &tradeAccount,&position);
	if( ptSuccess != nStatus )
	{
		return false;
	}

	if ( util::equals( position.Buys, 0) && util::equals( position.Sells, 0))
	{
		return false;
	}

	PositionStruct openPosition;
	ApiFunc::ptGetOpenPosition(&contract.ExchangeName, &contract.ContractName, &contract.ContractDate, &tradeAccount,&openPosition);

	PriceStruct newPrice;
	if( ptSuccess != ApiFunc::ptGetPriceForContract(&contract.ExchangeName, &contract.ContractName, &contract.ContractDate, &newPrice))
	{
		return false;
	}

	float fAvgPrice;
	PriceStr avgPrice;
	memset(avgPrice, 0, sizeof(avgPrice));
	if( ptSuccess != ApiFunc::ptGetAveragePrice(&contract.ExchangeName, &contract.ContractName, &contract.ContractDate, &tradeAccount, &avgPrice))
	{
		return false;
	}
	else
	{
		fAvgPrice = atof( avgPrice );
	}

	int nMultiply = 0;
	CommodityStruct commodity;
	if( ptSuccess != ApiFunc::ptGetCommodityByName(&contract.ExchangeName, &contract.ContractName,&commodity))
	{
		return false;
	}
	else
	{
		nMultiply = atoi( commodity.OnePoint );
	}

	Array20 marginLot;
	memset( marginLot, 0, sizeof(marginLot));
	if(ApiFunc::ptGetMarginPerLot!=NULL) {
		if ( ptSuccess != ApiFunc::ptGetMarginPerLot(&contract.ExchangeName, &contract.ContractName, 
			&contract.ContractDate, &tradeAccount, &marginLot))
		{
			return false;
		}
	}
	else {
		return false;
	}

	//资金消耗率
	char burnRate[100];
	memset( burnRate, 0, sizeof(burnRate));
	if ( ptSuccess != ApiFunc::ptPLBurnRate(&contract.ExchangeName, &contract.ContractName, 
		&contract.ContractDate, &tradeAccount, (FloatPtr)burnRate))
	{
		return false;
	}

	//未平仓合约风险
	char openPositionExposure[100];
	memset( openPositionExposure, 0, sizeof(openPositionExposure));
	if ( ptSuccess != ApiFunc::ptOpenPositionExposure(&contract.ExchangeName, &contract.ContractName, 
		&contract.ContractDate, &tradeAccount, (FloatPtr)openPositionExposure))
	{
		return false;
	}

	//剩余购买力
	char BRRemain[100];
	memset( BRRemain, 0, sizeof(BRRemain));
	if ( ptSuccess != ApiFunc::ptBuyingPowerRemaining(&contract.ExchangeName, &contract.ContractName, 
		&contract.ContractDate, &tradeAccount, (FloatPtr)BRRemain))
	{
		return false;
	}

	//占用资金
	char BRUsed[100];
	memset( BRUsed, 0, sizeof(BRUsed));
	if ( ptSuccess != ApiFunc::ptBuyingPowerUsed(&contract.ExchangeName, &contract.ContractName, 
		&contract.ContractDate, &tradeAccount, (FloatPtr)BRUsed))
	{
		return false;
	}

	//支付保证金
	char MarginReqd[100];
	memset( MarginReqd, 0, sizeof(MarginReqd));
	if ( ptSuccess != ApiFunc::ptTotalMarginPaid(&contract.ExchangeName, &contract.ContractName, 
		&contract.ContractDate, &tradeAccount, (FloatPtr)MarginReqd))
	{
		return false;
	}

	strcpy( field.InstrumentID, strInstrumentID.c_str());
	strcpy( field.ExchangeID, contract.ExchangeName);
	strcpy( field.InvestorID, tradeAccount);
	field.pats_BuyVolumn = position.Buys;
	field.pats_SellVolumn = position.Sells;
	field.pats_TotalProfit = atof( position.Profit );
	field.PreSettlementPrice = atof( newPrice.pvCurrStl.Price);
	field.HedgeFlag = THOST_FTDC_HF_Speculation;//默认投机
	field.pats_MarginPerLot = atof( marginLot ) ;
	field.pats_OpenAvgPrice = fAvgPrice;
	field.pats_OpenProfit = atof( openPosition.Profit );
	field.pats_PLBurnRate = atof( burnRate );
	field.pats_OpenPositionExposure = atof( openPositionExposure );
	field.pats_BuyPowerRemain = atof( BRRemain );
	field.pats_BuyPowerUsed = atof( BRUsed );
	field.pats_TotalMarginPaid = atof( MarginReqd );
	strcpy(field.szAccount, strAccount.c_str());

	//净仓位
	if( !util::equals( openPosition.Buys, 0) )
	{
		field.PosiDirection = THOST_FTDC_D_Buy;
		field.Position = openPosition.Buys;	
	}	
	else if ( !util::equals( openPosition.Sells, 0) )
	{
		field.PosiDirection = THOST_FTDC_D_Sell;
		field.Position = openPosition.Sells;
	}
	else
	{
		field.PosiDirection = THOST_FTDC_D_Buy;
		field.Position = 0;
	}

	return true;
}

void CPatsystemApi::SetNewPwd( const std::string& strNewPwd )
{
	m_strNewPwd = strNewPwd;
}

int CPatsystemApi::GetExchange(vector<PlatformStru_Exchange>& outData)
{
	int count=0;
	outData.clear();
	if(ApiFunc::ptCountExchanges(&count)==ptSuccess)
	{
		ExchangeStruct exch;
		memset(&exch,0,sizeof(ExchangeStruct));
		PlatformStru_Exchange Exchange;
		for(int i=0;i<count;i++)
		{
			if(ApiFunc::ptGetExchange(i,&exch)==ptSuccess)
			{
				strcpy(Exchange.Name,exch.Name);
				Exchange.QueryEnabled=exch.QueryEnabled;
				Exchange.QueryEnabled=exch.QueryEnabled;
				Exchange.AmendEnabled=exch.AmendEnabled;
				Exchange.Strategy=exch.Strategy;
				Exchange.CustomDecs=exch.CustomDecs;
				Exchange.Decimals=exch.Decimals;
				Exchange.TicketType=exch.TicketType;
				Exchange.RFQA=exch.RFQA;
				Exchange.RFQT=exch.RFQT;
				Exchange.EnableBlock=exch.EnableBlock;
				Exchange.EnableBasis=exch.EnableBasis;
				Exchange.EnableAA=exch.EnableAA;
				Exchange.EnableCross=exch.EnableCross;
				Exchange.GTStatus=exch.GTStatus;

				outData.push_back((PlatformStru_Exchange)Exchange);
			}
		}
	}
	return count;
}
int CPatsystemApi::GetTrader(vector<PlatformStru_Trader>& outData)
{
	int count=0;
	outData.clear();

	if(ApiFunc::ptCountTraders(&count)==ptSuccess)
	{
		TraderAcctStruct TraderAcct;
		memset(&TraderAcct,0,sizeof(TraderAcctStruct));
		PlatformStru_Trader  Trader;
		for(int cLoop=0;cLoop<count;cLoop++)
		{
			ApiFunc::ptGetTrader(cLoop,&TraderAcct);
			strcpy(Trader.TraderAccount,TraderAcct.TraderAccount);
			strcpy(Trader.BackOfficeID,TraderAcct.BackOfficeID);
			Trader.Tradable=TraderAcct.Tradable;
			Trader.LossLimit=TraderAcct.LossLimit;
			outData.push_back(Trader);	
		}
	}

	return count;
}
int CPatsystemApi::GetContract(vector<PlatformStru_Contract>& outData)
{
	int count=0;
	outData.clear();

	if(ApiFunc:: ptCountContracts(&count)==ptSuccess)
	{
		ContractStruct Contractpats;
		memset(&Contractpats,0,sizeof(ContractStruct));
		PlatformStru_Contract Contract;
		for(int cLoop=0;cLoop<count;cLoop++)
		{
			ApiFunc::ptGetContract(cLoop,&Contractpats);
			strcpy(Contract.ContractName,Contractpats.ContractName);
			strcpy(Contract.ContractDate,Contractpats.ContractDate);
			strcpy(Contract.ExchangeName,Contractpats.ExchangeName);
			strcpy(Contract.ExpiryDate,Contractpats.ExpiryDate);
			strcpy(Contract.LastTradeDate,Contractpats.LastTradeDate);
			Contract.NumberOfLegs=Contractpats.NumberOfLegs;
            Contract.TicksPerPoint=Contractpats.TicksPerPoint;
			strcpy(Contract.TickSize,Contractpats.TickSize);
            Contract.Tradable=Contractpats.Tradable;
            Contract.GTStatus=Contractpats.GTStatus;
			strcpy(Contract.Margin,Contractpats.Margin);
			Contract.ESATemplate=Contractpats.ESATemplate;
			strcpy(Contract.MarketRef,Contractpats.MarketRef);
			strcpy(Contract.lnExchangeName,Contractpats.lnExchangeName);
			strcpy(Contract.lnContractName,Contractpats.lnContractName);
			strcpy(Contract.lnContractDate,Contractpats.lnContractDate);
			outData.push_back(Contract);	
		}
	}

	return count;
}
int CPatsystemApi::GetCommodity(vector<PlatformStru_Commodity>& outData)
{
	int count=0;
	outData.clear();

	if(ApiFunc::  ptCountCommodities(&count)==ptSuccess)
	{
		CommodityStruct Commoditypats;
		memset(&Commoditypats,0,sizeof(CommodityStruct));
		PlatformStru_Commodity Commodity;
		for(int cLoop=0;cLoop<count;cLoop++)
		{
			ApiFunc::ptGetCommodity(cLoop,&Commoditypats);
			strcpy(Commodity.ExchangeName,Commoditypats.ExchangeName);
			strcpy(Commodity.ContractName,Commoditypats.ContractName);
			strcpy(Commodity.Currency,Commoditypats.Currency);
			strcpy(Commodity.Group,Commoditypats.Group);
			strcpy(Commodity.OnePoint,Commoditypats.OnePoint);
            Commodity.TicksPerPoint=Commoditypats.TicksPerPoint;
			strcpy(Commodity.TickSize,Commoditypats.TickSize);
            Commodity.GTStatus=Commoditypats.GTStatus;
			outData.push_back(Commodity);	
		}
	}

	return count;
}
int CPatsystemApi::GetOrderType(vector<PlatformStru_OrderType>& outData)
{
	int count=0;
	outData.clear();

	if(ApiFunc::ptCountOrderTypes (&count)==ptSuccess)
	{
		OrderTypeStruct OrderTypepats;
		memset(&OrderTypepats,0,sizeof(OrderTypeStruct));
		AmendTypesArray mendOrderTypepats;
		PlatformStru_OrderType OrderType;
		for(int cLoop=0;cLoop<count;cLoop++)
		{
			ApiFunc::ptGetOrderType(cLoop,&OrderTypepats,mendOrderTypepats);
			strcpy(OrderType.OrderType,OrderTypepats.OrderType);
			strcpy(OrderType.ExchangeName,OrderTypepats.ExchangeName);
			OrderType.OrderTypeID=OrderTypepats.OrderTypeID;
			OrderType.NumPricesReqd=OrderTypepats.NumPricesReqd;
			OrderType.NumVolumesReqd=OrderTypepats.NumVolumesReqd;
			OrderType.NumDatesReqd=OrderTypepats.NumDatesReqd;
			OrderType.AutoCreated=OrderTypepats.AutoCreated;
			OrderType.TimeTriggered=OrderTypepats.TimeTriggered;
			OrderType.RealSynthetic=OrderTypepats.RealSynthetic;
			OrderType.GTCFlag=OrderTypepats.GTCFlag;
			strcpy(OrderType.TicketType,OrderTypepats.TicketType);
			OrderType.PatsOrderType=OrderTypepats.PatsOrderType;
			OrderType.AmendOTCount=OrderTypepats.AmendOTCount;
			strcpy(OrderType.AlgoXML,OrderTypepats.AlgoXML);
			strcpy(OrderType.AmendTypesArray,mendOrderTypepats);
            
			outData.push_back(OrderType);	
		}
	}

	return count;
}
bool CPatsystemApi::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	return ApiFunc::ptGetTotalPosition((TraderPtr)account.c_str(),(PositionStructPtr)posi)==ptSuccess;

}
bool CPatsystemApi::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
		return ApiFunc::ptGetOpenPosition(
			(ExchNamePtr)InstrumentID.ExchangeName,
			(ConNamePtr)InstrumentID.ContractName,
			(ConDatePtr)InstrumentID.ContractDate,
			(TraderPtr)account.c_str(),
			(PositionStructPtr)posi)==ptSuccess&&(posi->Sells>0||posi->Buys>0);
}
bool CPatsystemApi::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
		return ApiFunc::ptGetContractPosition(
			(ExchNamePtr)InstrumentID.ExchangeName,
			(ConNamePtr)InstrumentID.ContractName,
			(ConDatePtr)InstrumentID.ContractDate,
			(TraderPtr)account.c_str(),
			(PositionStructPtr)posi)==ptSuccess&&(posi->Sells>0||posi->Buys>0);

}
//void WINAPI CPatsystemApi::onRecvMessage( MsgIDPtr data )
//{
//	DLL_TRACE( (char*)data );
//}

std::string CPatsystemApi::GetLogonReason()
{
	return m_logonStatus.Reason;
}
void CPatsystemApi::GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition)
{
	vecPosition.clear();
    std::set<PatsPositionItem>::iterator it;
    for(it=m_PatsPositionItems.begin();it!=m_PatsPositionItems.end();++it)
	{
        PatsPositionItem& FillDetail=*it;
		PositionStruct position;
		if(ptSuccess!=ApiFunc::ptGetContractPosition(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate, &FillDetail.TraderAccount,&position))
			continue;
		if ( util::equals( position.Buys, 0) && util::equals( position.Sells, 0))
			continue;
		PositionStruct openPosition;
		if(ptSuccess!=ApiFunc::ptGetOpenPosition(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate,&FillDetail.TraderAccount,&openPosition))
			continue;
		if(openPosition.Buys==0&&openPosition.Sells==0)
			continue;
		PriceStruct newPrice;
		if(ptSuccess!=ApiFunc::ptGetPriceForContract(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate, &newPrice))
			continue;
		float fAvgPrice;
		SafeFloatStr avgPrice={0};
		if(ptSuccess!=ApiFunc::ptGetAveragePrice(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate,&FillDetail.TraderAccount,(PricePtr)&avgPrice))
			continue;
		fAvgPrice = atof( avgPrice );
		int nMultiply = 0;
		CommodityStruct commodity;
		if( ptSuccess != ApiFunc::ptGetCommodityByName(&FillDetail.ExchangeName, &FillDetail.ContractName,&commodity))
			continue;
		nMultiply = atoi( commodity.OnePoint );
		Array20 marginLot={0};
		if ( ptSuccess != ApiFunc::ptGetMarginPerLot(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate, &FillDetail.TraderAccount, &marginLot))
			continue;
		//资金消耗率
		SafeFloatStr burnRate;
		memset( burnRate, 0, sizeof(burnRate));
		if ( ptSuccess != ApiFunc::ptPLBurnRate(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate,&FillDetail.TraderAccount, (FloatPtr)&burnRate))
			continue;
		//未平仓合约风险
		SafeFloatStr openPositionExposure;
		memset( openPositionExposure, 0, sizeof(openPositionExposure));
		if ( ptSuccess != ApiFunc::ptOpenPositionExposure(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate, &FillDetail.TraderAccount, (FloatPtr)&openPositionExposure))
			continue;
		//剩余购买力
		SafeFloatStr BRRemain;
		memset( BRRemain, 0, sizeof(BRRemain));
		if ( ptSuccess != ApiFunc::ptBuyingPowerRemaining(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate, &FillDetail.TraderAccount, (FloatPtr)&BRRemain))
			continue;
		//占用资金
		SafeFloatStr BRUsed;
		memset( BRUsed, 0, sizeof(BRUsed));
		if ( ptSuccess != ApiFunc::ptBuyingPowerUsed(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate,&FillDetail.TraderAccount, (FloatPtr)&BRUsed))
			continue;
		//支付保证金
		SafeFloatStr MarginReqd;
		memset( MarginReqd, 0, sizeof(MarginReqd));
		if ( ptSuccess != ApiFunc::ptTotalMarginPaid(&FillDetail.ExchangeName, &FillDetail.ContractName, 
			&FillDetail.ContractDate,&FillDetail.TraderAccount, (FloatPtr)&MarginReqd))
			continue;

		PlatformStru_Position field;
		std::string strInstrumentID = MakeLongInstrumentID( FillDetail.ExchangeName, FillDetail.ContractName, FillDetail.ContractDate);
		strcpy( field.InstrumentID, strInstrumentID.c_str());
		strcpy( field.ExchangeID, FillDetail.ExchangeName);
		//strcpy( field.InvestorID, FillDetail.TraderAccount);
		////净仓位
		if(openPosition.Buys>0)
		{
			field.PosiDirection = THOST_FTDC_PD_Long;
			field.Position = openPosition.Buys;
		}
		else
		{
			field.PosiDirection = THOST_FTDC_PD_Short;
			field.Position = openPosition.Sells;
		}
		field.pats_BuyVolumn = position.Buys;
		field.pats_SellVolumn = position.Sells;
		field.PositionDate = THOST_FTDC_PSD_Today;
		field.SettlementPrice = atof( newPrice.pvCurrStl.Price);
		field.HedgeFlag = THOST_FTDC_HF_Speculation;//默认投机
		field.CurPrice = atof(newPrice.Last0.Price);
		field.CurPriceUsedByCalcu = atof(newPrice.Last0.Price);
		field.AverageOpenPrice = fAvgPrice;
		field.AveragePositionPrice = fAvgPrice;
		field.TotalPositionProfitByDate = atof( position.Profit );//使用合约货币
		field.PositionProfit = atof( openPosition.Profit );//使用合约货币
		field.CloseProfit = field.TotalPositionProfitByDate-field.PositionProfit;
		field.UseMargin = atof( MarginReqd );//atof( BRUsed );
		field.pats_MarginPerLot = atof( marginLot ) ;
		field.pats_PLBurnRate = atof( burnRate );
		field.pats_OpenPositionExposure = atof( openPositionExposure );
		field.pats_BuyPowerRemain = atof( BRRemain );
		field.pats_BuyPowerUsed = atof( BRUsed );
		field.pats_TotalMarginPaid = atof( MarginReqd );
		strcpy(field.szAccount, FillDetail.TraderAccount);

		if(field.Position>0)
			vecPosition.push_back( field );
	}
}
double CPatsystemApi::GetExchangeRate(char* currency)
{
	SafeFloatStr ExchRate={0};
	ApiFunc::ptGetExchangeRate((CurrNamePtr)currency,(ExchRatePtr)ExchRate);
	return atof(ExchRate);
}
int CPatsystemApi::GetReports( map<DWORD, Report>& mapReport )
{
	mapReport.clear();

	int nReportCount = 0;
	if ( ptSuccess != ApiFunc::ptCountReportTypes(&nReportCount))
	{
		return -1;
	}

	for ( int i = 0; i < nReportCount; i++)
	{
		ReportTypeStr reportType;
		memset(reportType, 0, sizeof(reportType));
		if ( ptSuccess != ApiFunc::ptGetReportType(i, &reportType))
		{
			continue;
		}

		if ( ptSuccess != ApiFunc::ptReportTypeExists(&reportType))
		{
			continue;
		}

		int nSize = 0;
		if ( ptSuccess != ApiFunc::ptGetReportSize(&reportType, &nSize))
		{
			continue;
		}

		if ( nSize <= 0 )
		{
			continue;
		}

		char* pBuffer = new char[nSize+1];
		memset(pBuffer, 0, nSize + 1);
		if ( ptSuccess != ApiFunc::ptGetReport(&reportType, nSize, pBuffer))
		{
			delete [] pBuffer;
			pBuffer = NULL;
			continue;
		}

		Report report;
		report.strReportType = reportType;
		report.nSize = nSize;
		report.pReport = pBuffer;
		mapReport[report.GetDate()] = report;
	}

	return 0;
}

void CPatsystemApi::InvalidateDepthMarketData( PlatformStru_DepthMarketData* pDepthMarketData )
{
	if(pDepthMarketData==NULL) return;
	ZeroMemory(pDepthMarketData,sizeof(PlatformStru_DepthMarketData));
	///交易日
	///合约代码
	///交易所代码
	///合约在交易所的代码
	///最新价
	pDepthMarketData->LastPrice=util::GetDoubleInvalidValue();
	///上次结算价
	pDepthMarketData->PreSettlementPrice=util::GetDoubleInvalidValue();
	///昨收盘
	pDepthMarketData->PreClosePrice=util::GetDoubleInvalidValue();
	///昨持仓量
	pDepthMarketData->PreOpenInterest=util::GetDoubleInvalidValue();
	///今开盘
	pDepthMarketData->OpenPrice=util::GetDoubleInvalidValue();
	///最高价
	pDepthMarketData->HighestPrice=util::GetDoubleInvalidValue();
	///最低价
	pDepthMarketData->LowestPrice=util::GetDoubleInvalidValue();
	///数量
	pDepthMarketData->Volume=util::GetIntInvalidValue();
	///成交金额
	pDepthMarketData->Turnover=util::GetDoubleInvalidValue();
	///持仓量
	pDepthMarketData->OpenInterest=util::GetDoubleInvalidValue();
	///今收盘
	pDepthMarketData->ClosePrice=util::GetDoubleInvalidValue();
	///本次结算价
	pDepthMarketData->SettlementPrice=util::GetDoubleInvalidValue();
	///涨停板价
	pDepthMarketData->UpperLimitPrice=util::GetDoubleInvalidValue();
	///跌停板价
	pDepthMarketData->LowerLimitPrice=util::GetDoubleInvalidValue();
	///昨虚实度
	pDepthMarketData->PreDelta=util::GetDoubleInvalidValue();
	///今虚实度
	pDepthMarketData->CurrDelta=util::GetDoubleInvalidValue();
	///最后修改时间
	///最后修改毫秒
	pDepthMarketData->UpdateMillisec=util::GetIntInvalidValue();
	///申买价一
	pDepthMarketData->BidPrice1=util::GetDoubleInvalidValue();
	///申买量一
	pDepthMarketData->BidVolume1=util::GetIntInvalidValue();
	///申卖价一
	pDepthMarketData->AskPrice1=util::GetDoubleInvalidValue();
	///申卖量一
	pDepthMarketData->AskVolume1=util::GetIntInvalidValue();
	///申买价二
	pDepthMarketData->BidPrice2=util::GetDoubleInvalidValue();
	///申买量二
	pDepthMarketData->BidVolume2=util::GetIntInvalidValue();
	///申卖价二
	pDepthMarketData->AskPrice2=util::GetDoubleInvalidValue();
	///申卖量二
	pDepthMarketData->AskVolume2=util::GetIntInvalidValue();
	///申买价三
	pDepthMarketData->BidPrice3=util::GetDoubleInvalidValue();
	///申买量三
	pDepthMarketData->BidVolume3=util::GetIntInvalidValue();
	///申卖价三
	pDepthMarketData->AskPrice3=util::GetDoubleInvalidValue();
	///申卖量三
	pDepthMarketData->AskVolume3=util::GetIntInvalidValue();
	///申买价四
	pDepthMarketData->BidPrice4=util::GetDoubleInvalidValue();
	///申买量四
	pDepthMarketData->BidVolume4=util::GetIntInvalidValue();
	///申卖价四
	pDepthMarketData->AskPrice4=util::GetDoubleInvalidValue();
	///申卖量四
	pDepthMarketData->AskVolume4=util::GetIntInvalidValue();
	///申买价五
	pDepthMarketData->BidPrice5=util::GetDoubleInvalidValue();
	///申买量五
	pDepthMarketData->BidVolume5=util::GetIntInvalidValue();
	///申卖价五
	pDepthMarketData->AskPrice5=util::GetDoubleInvalidValue();
	///申卖量五
	pDepthMarketData->AskVolume5=util::GetIntInvalidValue();
	///当日均价
	pDepthMarketData->AveragePrice=util::GetDoubleInvalidValue();

	///申买价六
	pDepthMarketData->BidPrice6=util::GetDoubleInvalidValue();
	///申买量六
	pDepthMarketData->BidVolume6=util::GetIntInvalidValue();
	///申卖价六
	pDepthMarketData->AskPrice6=util::GetDoubleInvalidValue();
	///申卖量六
	pDepthMarketData->AskVolume6=util::GetIntInvalidValue();
	///申买价七
	pDepthMarketData->BidPrice7=util::GetDoubleInvalidValue();
	///申买量七
	pDepthMarketData->BidVolume7=util::GetIntInvalidValue();
	///申卖价七
	pDepthMarketData->AskPrice7=util::GetDoubleInvalidValue();
	///申卖量七
	pDepthMarketData->AskVolume7=util::GetIntInvalidValue();
	///申买价八
	pDepthMarketData->BidPrice8=util::GetDoubleInvalidValue();
	///申买量八
	pDepthMarketData->BidVolume8=util::GetIntInvalidValue();
	///申卖价八
	pDepthMarketData->AskPrice8=util::GetDoubleInvalidValue();
	///申卖量八
	pDepthMarketData->AskVolume8=util::GetIntInvalidValue();
	///申买价九
	pDepthMarketData->BidPrice9=util::GetDoubleInvalidValue();
	///申买量九
	pDepthMarketData->BidVolume9=util::GetIntInvalidValue();
	///申卖价九
	pDepthMarketData->AskPrice9=util::GetDoubleInvalidValue();
	///申卖量九
	pDepthMarketData->AskVolume9=util::GetIntInvalidValue();
	///申买价十
	pDepthMarketData->BidPrice10=util::GetDoubleInvalidValue();
	///申买量十
	pDepthMarketData->BidVolume10=util::GetIntInvalidValue();
	///申卖价十
	pDepthMarketData->AskPrice10=util::GetDoubleInvalidValue();
	///申卖量十
	pDepthMarketData->AskVolume10=util::GetIntInvalidValue();
	//最新量
	pDepthMarketData->NewVolume=util::GetIntInvalidValue();
}
