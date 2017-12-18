#include "StdAfx.h"
#include "ApiFunc.h"

#define LOADFUNCTION(funcname)\
	if(NULL==(funcname=(lp_##funcname)::GetProcAddress(m_hInst_PATSAPIDLL,#funcname))) bRet=false
HINSTANCE ApiFunc::m_hInst_PATSAPIDLL=NULL;
lp_ptRegisterCallback                                   ApiFunc::ptRegisterCallback=NULL;
lp_ptRegisterLinkStateCallback                          ApiFunc::ptRegisterLinkStateCallback=NULL;
lp_ptRegisterMsgCallback                                ApiFunc::ptRegisterMsgCallback=NULL;
lp_ptRegisterOrderCallback                              ApiFunc::ptRegisterOrderCallback=NULL;
lp_ptRegisterOrderTypeUpdateCallback                    ApiFunc::ptRegisterOrderTypeUpdateCallback=NULL;
lp_ptRegisterFillCallback                               ApiFunc::ptRegisterFillCallback=NULL;
lp_ptRegisterPriceCallback                              ApiFunc::ptRegisterPriceCallback=NULL;
lp_ptRegisterBlankPriceCallback                         ApiFunc::ptRegisterBlankPriceCallback=NULL;
lp_ptRegisterGenericPriceCallback                       ApiFunc::ptRegisterGenericPriceCallback=NULL;
lp_ptRegisterDOMCallback                                ApiFunc::ptRegisterDOMCallback=NULL;
lp_ptRegisterContractCallback                           ApiFunc::ptRegisterContractCallback=NULL;
lp_ptRegisterCommodityCallback                          ApiFunc::ptRegisterCommodityCallback=NULL;
lp_ptRegisterExchangeCallback                           ApiFunc::ptRegisterExchangeCallback=NULL;
lp_ptRegisterStatusCallback                             ApiFunc::ptRegisterStatusCallback=NULL;
lp_ptRegisterExchangeRateCallback                       ApiFunc::ptRegisterExchangeRateCallback=NULL;
lp_ptRegisterConStatusCallback                          ApiFunc::ptRegisterConStatusCallback=NULL;
lp_ptRegisterOrderCancelFailureCallback                 ApiFunc::ptRegisterOrderCancelFailureCallback=NULL;
lp_ptRegisterOrderSentFailureCallback                   ApiFunc::ptRegisterOrderSentFailureCallback=NULL;
lp_ptRegisterOrderQueuedFailureCallback                 ApiFunc::ptRegisterOrderQueuedFailureCallback=NULL;
lp_ptRegisterAtBestCallback                             ApiFunc::ptRegisterAtBestCallback=NULL;
lp_ptRegisterTickerCallback                             ApiFunc::ptRegisterTickerCallback=NULL;
lp_ptRegisterAmendFailureCallback                       ApiFunc::ptRegisterAmendFailureCallback=NULL;
lp_ptRegisterSubscriberDepthCallback                    ApiFunc::ptRegisterSubscriberDepthCallback=NULL;
lp_ptRegisterSettlementCallback                         ApiFunc::ptRegisterSettlementCallback=NULL;
lp_ptRegisterStrategyCreateSuccess                      ApiFunc::ptRegisterStrategyCreateSuccess=NULL;
lp_ptRegisterStrategyCreateFailure                      ApiFunc::ptRegisterStrategyCreateFailure=NULL;
lp_ptRegisterOrderBookReset                             ApiFunc::ptRegisterOrderBookReset=NULL;
lp_ptRegisterTraderAdded                                ApiFunc::ptRegisterTraderAdded=NULL;
lp_ptCountExchanges                                     ApiFunc::ptCountExchanges=NULL;
lp_ptGetExchange                                        ApiFunc::ptGetExchange=NULL;
lp_ptGetExchangeByName                                  ApiFunc::ptGetExchangeByName=NULL;
lp_ptExchangeExists                                     ApiFunc::ptExchangeExists=NULL;
lp_ptCountTraders                                       ApiFunc::ptCountTraders=NULL;
lp_ptGetTrader                                          ApiFunc::ptGetTrader=NULL;
lp_ptGetTraderByName                                    ApiFunc::ptGetTraderByName=NULL;
lp_ptTraderExists                                       ApiFunc::ptTraderExists=NULL;
lp_ptCountOrderTypes                                    ApiFunc::ptCountOrderTypes=NULL;
lp_ptGetOrderType                                       ApiFunc::ptGetOrderType=NULL;
lp_ptGetExchangeRate                                    ApiFunc::ptGetExchangeRate=NULL;
lp_ptCountReportTypes                                   ApiFunc::ptCountReportTypes=NULL;
lp_ptGetReportType                                      ApiFunc::ptGetReportType=NULL;
lp_ptReportTypeExists                                   ApiFunc::ptReportTypeExists=NULL;
lp_ptGetReportSize                                      ApiFunc::ptGetReportSize=NULL;
lp_ptGetReport                                          ApiFunc::ptGetReport=NULL;
lp_ptCountCommodities                                   ApiFunc::ptCountCommodities=NULL;
lp_ptGetCommodity                                       ApiFunc::ptGetCommodity=NULL;
lp_ptCommodityExists                                    ApiFunc::ptCommodityExists=NULL;
lp_ptGetCommodityByName                                 ApiFunc::ptGetCommodityByName=NULL;
lp_ptCountContracts                                     ApiFunc::ptCountContracts=NULL;
lp_ptGetContract                                        ApiFunc::ptGetContract=NULL;
lp_ptGetContractByName                                  ApiFunc::ptGetContractByName=NULL;
lp_ptGetContractByExternalID                            ApiFunc::ptGetContractByExternalID=NULL;
lp_ptGetExtendedContract                                ApiFunc::ptGetExtendedContract=NULL;
lp_ptGetExtendedContractByName                          ApiFunc::ptGetExtendedContractByName=NULL;
lp_ptContractExists                                     ApiFunc::ptContractExists=NULL;
lp_ptInitialise                                         ApiFunc::ptInitialise=NULL;
lp_ptGetAPIBuildVersion                                 ApiFunc::ptGetAPIBuildVersion=NULL;
lp_ptReady                                              ApiFunc::ptReady=NULL;
lp_ptPurge                                              ApiFunc::ptPurge=NULL;
lp_ptDisconnect                                         ApiFunc::ptDisconnect=NULL;
lp_ptSetHostAddress                                     ApiFunc::ptSetHostAddress=NULL;
lp_ptSetPriceAddress                                    ApiFunc::ptSetPriceAddress=NULL;
lp_ptEnable                                             ApiFunc::ptEnable=NULL;
lp_ptDisable                                            ApiFunc::ptDisable=NULL;
lp_ptLogString                                          ApiFunc::ptLogString=NULL;
lp_ptOMIEnabled                                         ApiFunc::ptOMIEnabled=NULL;
lp_ptNotifyAllMessages                                  ApiFunc::ptNotifyAllMessages=NULL;
lp_ptSetHostReconnect                                   ApiFunc::ptSetHostReconnect=NULL;
lp_ptSetPriceReconnect                                  ApiFunc::ptSetPriceReconnect=NULL;
lp_ptSetPriceAgeCounter                                 ApiFunc::ptSetPriceAgeCounter=NULL;
lp_ptSubscribePrice                                     ApiFunc::ptSubscribePrice=NULL;
lp_ptSubscribeToMarket                                  ApiFunc::ptSubscribeToMarket=NULL;
lp_ptUnsubscribePrice                                   ApiFunc::ptUnsubscribePrice=NULL;
lp_ptUnsubscribeToMarket                                ApiFunc::ptUnsubscribeToMarket=NULL;
lp_ptPriceSnapshot                                      ApiFunc::ptPriceSnapshot=NULL;
lp_ptSetEncryptionCode                                  ApiFunc::ptSetEncryptionCode=NULL;
lp_ptSetHandShakePeriod                                 ApiFunc::ptSetHandShakePeriod=NULL;
lp_ptSetHostHandShake                                   ApiFunc::ptSetHostHandShake=NULL;
lp_ptSetPriceHandShake                                  ApiFunc::ptSetPriceHandShake=NULL;
lp_ptSetInternetUser                                    ApiFunc::ptSetInternetUser=NULL;
lp_ptSetClientPath                                      ApiFunc::ptSetClientPath=NULL;
lp_ptGetErrorMessage          ApiFunc::ptGetErrorMessage=NULL;
lp_ptDumpLastError                                      ApiFunc::ptDumpLastError=NULL;
lp_ptSnapdragonEnabled                                  ApiFunc::ptSnapdragonEnabled=NULL;
lp_ptSubscribeBroadcast                                 ApiFunc::ptSubscribeBroadcast=NULL;
lp_ptUnsubscribeBroadcast                               ApiFunc::ptUnsubscribeBroadcast=NULL;
lp_ptCountFills                                         ApiFunc::ptCountFills=NULL;
lp_ptGetFill                                            ApiFunc::ptGetFill=NULL;
lp_ptGetFillByID                                        ApiFunc::ptGetFillByID=NULL;
lp_ptGetContractPosition                                ApiFunc::ptGetContractPosition=NULL;
lp_ptGetOpenPosition                                    ApiFunc::ptGetOpenPosition=NULL;
lp_ptGetTotalPosition                                   ApiFunc::ptGetTotalPosition=NULL;
lp_ptGetAveragePrice                                    ApiFunc::ptGetAveragePrice=NULL;
lp_ptCountOrders                                        ApiFunc::ptCountOrders=NULL;
lp_ptGetOrder                                           ApiFunc::ptGetOrder=NULL;
lp_ptGetOrderByID                                       ApiFunc::ptGetOrderByID=NULL;
lp_ptGetOrderIndex                                      ApiFunc::ptGetOrderIndex=NULL;
lp_ptBlankPrices                                        ApiFunc::ptBlankPrices=NULL;
lp_ptGetPrice                                           ApiFunc::ptGetPrice=NULL;
lp_ptGetPriceForContract                                ApiFunc::ptGetPriceForContract=NULL;
lp_ptGetGenericPrice                                    ApiFunc::ptGetGenericPrice=NULL;
lp_ptAddOrder                                           ApiFunc::ptAddOrder=NULL;
lp_ptAddAlgoOrder                                       ApiFunc::ptAddAlgoOrder=NULL;
lp_ptGetOrderEx                                         ApiFunc::ptGetOrderEx=NULL;
lp_ptGetOrderByIDEx                                     ApiFunc::ptGetOrderByIDEx=NULL;
lp_ptGetOrderHistoryEx                                  ApiFunc::ptGetOrderHistoryEx=NULL;
lp_ptAddProtectionOrder                                 ApiFunc::ptAddProtectionOrder=NULL;
lp_ptAddBatchOrder                                      ApiFunc::ptAddBatchOrder=NULL;
lp_ptAddAggregateOrder                                  ApiFunc::ptAddAggregateOrder=NULL;
lp_ptAddCustRequest                                     ApiFunc::ptAddCustRequest=NULL;
lp_ptReParent                                           ApiFunc::ptReParent=NULL;
lp_ptDoneForDay                                         ApiFunc::ptDoneForDay=NULL;
lp_ptAddCrossingOrder                                   ApiFunc::ptAddCrossingOrder=NULL;
lp_ptAddBlockOrder                                      ApiFunc::ptAddBlockOrder=NULL;
lp_ptAddBasisOrder                                      ApiFunc::ptAddBasisOrder=NULL;
lp_ptAddAAOrder                                         ApiFunc::ptAddAAOrder=NULL;
lp_ptGetConsolidatedPosition                            ApiFunc::ptGetConsolidatedPosition=NULL;
lp_ptAmendOrder                                         ApiFunc::ptAmendOrder=NULL;
lp_ptAmendAlgoOrder                                     ApiFunc::ptAmendAlgoOrder=NULL;
lp_ptCancelOrder                                        ApiFunc::ptCancelOrder=NULL;
lp_ptActivateOrder                                      ApiFunc::ptActivateOrder=NULL;
lp_ptDeactivateOrder                                    ApiFunc::ptDeactivateOrder=NULL;
lp_ptCancelBuys                                         ApiFunc::ptCancelBuys=NULL;
lp_ptCancelSells                                        ApiFunc::ptCancelSells=NULL;
lp_ptCancelOrders                                       ApiFunc::ptCancelOrders=NULL;
lp_ptCancelAll                                          ApiFunc::ptCancelAll=NULL;
lp_ptOrderChecked                                       ApiFunc::ptOrderChecked=NULL;
lp_ptQueryOrderStatus                                   ApiFunc::ptQueryOrderStatus=NULL;
lp_ptCountOrderHistory                                  ApiFunc::ptCountOrderHistory=NULL;
lp_ptGetOrderHistory                                    ApiFunc::ptGetOrderHistory=NULL;
lp_ptSetUserIDFilter                                    ApiFunc::ptSetUserIDFilter=NULL;
lp_ptNextOrderSequence                                  ApiFunc::ptNextOrderSequence=NULL;
lp_ptLogOn                                              ApiFunc::ptLogOn=NULL;
lp_ptLogOff                                             ApiFunc::ptLogOff=NULL;
lp_ptGetLogonStatus                                     ApiFunc::ptGetLogonStatus=NULL;
lp_ptDOMEnabled                                         ApiFunc::ptDOMEnabled=NULL;
lp_ptPostTradeAmendEnabled                              ApiFunc::ptPostTradeAmendEnabled=NULL;
lp_ptEnabledFunctionality                               ApiFunc::ptEnabledFunctionality=NULL;
lp_ptCountUsrMsgs                                       ApiFunc::ptCountUsrMsgs=NULL;
lp_ptGetUsrMsg                                          ApiFunc::ptGetUsrMsg=NULL;
lp_ptGetUsrMsgByID                                      ApiFunc::ptGetUsrMsgByID=NULL;
lp_ptAcknowledgeUsrMsg                                  ApiFunc::ptAcknowledgeUsrMsg=NULL;
lp_ptPriceStep                                          ApiFunc::ptPriceStep=NULL;
lp_ptPLBurnRate                                         ApiFunc::ptPLBurnRate=NULL;
lp_ptOpenPositionExposure                               ApiFunc::ptOpenPositionExposure=NULL;
lp_ptBuyingPowerRemaining                               ApiFunc::ptBuyingPowerRemaining=NULL;
lp_ptBuyingPowerUsed                                    ApiFunc::ptBuyingPowerUsed=NULL;
lp_ptMarginForTrade                                     ApiFunc::ptMarginForTrade=NULL;
lp_ptTotalMarginPaid                                    ApiFunc::ptTotalMarginPaid=NULL;
lp_ptGetMarginPerLot                                    ApiFunc::ptGetMarginPerLot=NULL;
lp_ptSetOrderCancelFailureDelay                         ApiFunc::ptSetOrderCancelFailureDelay=NULL;
lp_ptSetOrderSentFailureDelay                           ApiFunc::ptSetOrderSentFailureDelay=NULL;
lp_ptSetOrderQueuedFailureDelay                         ApiFunc::ptSetOrderQueuedFailureDelay=NULL;
lp_ptCountContractAtBest                                ApiFunc::ptCountContractAtBest=NULL;
lp_ptGetContractAtBest                                  ApiFunc::ptGetContractAtBest=NULL;
lp_ptGetContractAtBestPrices                            ApiFunc::ptGetContractAtBestPrices=NULL;
lp_ptCountContractSubscriberDepth                       ApiFunc::ptCountContractSubscriberDepth=NULL;
lp_ptGetContractSubscriberDepth                         ApiFunc::ptGetContractSubscriberDepth=NULL;
lp_ptSuperTASEnabled                                    ApiFunc::ptSuperTASEnabled=NULL;
lp_ptSetSSL                                             ApiFunc::ptSetSSL=NULL;
lp_ptSetPDDSSL                                          ApiFunc::ptSetPDDSSL=NULL;
lp_ptSetMDSToken                                        ApiFunc::ptSetMDSToken=NULL;
lp_ptSetSSLCertificateName                              ApiFunc::ptSetSSLCertificateName=NULL;
lp_ptSetPDDSSLCertificateName                           ApiFunc::ptSetPDDSSLCertificateName=NULL;
lp_ptSetSSLClientAuthName                               ApiFunc::ptSetSSLClientAuthName=NULL;
lp_ptSetPDDSSLClientAuthName                            ApiFunc::ptSetPDDSSLClientAuthName=NULL;
lp_ptSetSuperTAS                                        ApiFunc::ptSetSuperTAS=NULL;
lp_ptSetMemoryWarning                                   ApiFunc::ptSetMemoryWarning=NULL;
lp_ptCreateStrategy                                     ApiFunc::ptCreateStrategy=NULL;
lp_ptGetOptionPremium                                   ApiFunc::ptGetOptionPremium=NULL;
lp_ptLockUpdates                                        ApiFunc::ptLockUpdates=NULL;
lp_ptUnlockUpdates                                      ApiFunc::ptUnlockUpdates=NULL;
lp_ptAddOrderEx                                         ApiFunc::ptAddOrderEx=NULL;
lp_ptAmendOrderEx                                       ApiFunc::ptAmendOrderEx=NULL;
lp_ptCancelOrderEx                                      ApiFunc::ptCancelOrderEx=NULL;
lp_ptCancelBuysEx                                       ApiFunc::ptCancelBuysEx=NULL;
lp_ptCancelSellsEx                                      ApiFunc::ptCancelSellsEx=NULL;
ApiFunc::ApiFunc(ClientType  type)
{
if(type==DemoClient)
	m_hInst_PATSAPIDLL = ::LoadLibrary("DEMOAPI.dll");
else
	m_hInst_PATSAPIDLL = ::LoadLibrary("PATSAPI.dll");
	if(m_hInst_PATSAPIDLL)
		GetFunctionAddrOfPats();
}

ApiFunc::~ApiFunc(void)
{
	if(m_hInst_PATSAPIDLL)
	{
		::FreeLibrary(m_hInst_PATSAPIDLL);
        m_hInst_PATSAPIDLL=NULL;
	}
}
//获取patsapi中各个函数的入口地址
bool ApiFunc::GetFunctionAddrOfPats(void)
{
	bool bRet=true;
	LOADFUNCTION(ptRegisterCallback);
	LOADFUNCTION(ptRegisterLinkStateCallback);
	LOADFUNCTION(ptRegisterMsgCallback);
	LOADFUNCTION(ptRegisterOrderCallback);
	LOADFUNCTION(ptRegisterOrderTypeUpdateCallback);
	LOADFUNCTION(ptRegisterFillCallback);
	LOADFUNCTION(ptRegisterPriceCallback);
	LOADFUNCTION(ptRegisterBlankPriceCallback);
	LOADFUNCTION(ptRegisterGenericPriceCallback);
	LOADFUNCTION(ptRegisterDOMCallback);
	LOADFUNCTION(ptRegisterContractCallback);
	LOADFUNCTION(ptRegisterCommodityCallback);
	LOADFUNCTION(ptRegisterExchangeCallback);
	LOADFUNCTION(ptRegisterStatusCallback);
	LOADFUNCTION(ptRegisterExchangeRateCallback);
	LOADFUNCTION(ptRegisterConStatusCallback);
	LOADFUNCTION(ptRegisterOrderCancelFailureCallback);
	LOADFUNCTION(ptRegisterOrderSentFailureCallback);
	LOADFUNCTION(ptRegisterOrderQueuedFailureCallback);
	LOADFUNCTION(ptRegisterAtBestCallback);
	LOADFUNCTION(ptRegisterTickerCallback);
	LOADFUNCTION(ptRegisterAmendFailureCallback);
	LOADFUNCTION(ptRegisterSubscriberDepthCallback);
	LOADFUNCTION(ptRegisterSettlementCallback);
	LOADFUNCTION(ptRegisterStrategyCreateSuccess);
	LOADFUNCTION(ptRegisterStrategyCreateFailure);
	LOADFUNCTION(ptRegisterOrderBookReset);
	//LOADFUNCTION(ptRegisterTraderAdded);
	LOADFUNCTION(ptCountExchanges);
	LOADFUNCTION(ptGetExchange);
	LOADFUNCTION(ptGetExchangeByName);
	LOADFUNCTION(ptExchangeExists);
	LOADFUNCTION(ptCountTraders);
	LOADFUNCTION(ptGetTrader);
	LOADFUNCTION(ptGetTraderByName);
	LOADFUNCTION(ptTraderExists);
	LOADFUNCTION(ptCountOrderTypes);
	LOADFUNCTION(ptGetOrderType);
	LOADFUNCTION(ptGetExchangeRate);
	LOADFUNCTION(ptCountReportTypes);
	LOADFUNCTION(ptGetReportType);
	LOADFUNCTION(ptReportTypeExists);
	LOADFUNCTION(ptGetReportSize);
	LOADFUNCTION(ptGetReport);
	LOADFUNCTION(ptCountCommodities);
	LOADFUNCTION(ptGetCommodity);
	LOADFUNCTION(ptCommodityExists);
	LOADFUNCTION(ptGetCommodityByName);
	LOADFUNCTION(ptCountContracts);
	LOADFUNCTION(ptGetContract);
	LOADFUNCTION(ptGetContractByName);
	LOADFUNCTION(ptGetContractByExternalID);
	LOADFUNCTION(ptGetExtendedContract);
	LOADFUNCTION(ptGetExtendedContractByName);
	LOADFUNCTION(ptContractExists);
	LOADFUNCTION(ptInitialise);
	LOADFUNCTION(ptGetAPIBuildVersion);
	LOADFUNCTION(ptReady);
	LOADFUNCTION(ptPurge);
	LOADFUNCTION(ptDisconnect);
	LOADFUNCTION(ptSetHostAddress);
	LOADFUNCTION(ptSetPriceAddress);
	LOADFUNCTION(ptEnable);
	LOADFUNCTION(ptDisable);
	LOADFUNCTION(ptLogString);
	LOADFUNCTION(ptOMIEnabled);
	LOADFUNCTION(ptNotifyAllMessages);
	LOADFUNCTION(ptSetHostReconnect);
	LOADFUNCTION(ptSetPriceReconnect);
	LOADFUNCTION(ptSetPriceAgeCounter);
	//INT ptSubscribePrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptSubscribePrice订阅价格，要求价格服务器提供指定合约的价格，价格更新由ptPriceUpdate通知
	LOADFUNCTION(ptSubscribePrice);
    //INT  ptSubscribeToMarket(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptSubscribeToMarket订阅一个交易所全部合约价格，或者一个交易所下特定合约不同时间的全部价格
	LOADFUNCTION(ptSubscribeToMarket);
    //INT ptUnsubscribePrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptUnsubscribePrice函数取消订阅价格，要求价格服务器停止发送价格信息，与ptSubscribePrice对应
	LOADFUNCTION(ptUnsubscribePrice);
    //INT ptUnsubscribeToMarket(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptUnsubscribeToMarket函数取消订阅价格，要求价格服务器停止发送价格信息，与ptSubscribeToMarket对应
	LOADFUNCTION(ptUnsubscribeToMarket);
    //INT ptPriceSnapshot(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT Wait);
    //ptPriceSnapshot函数要求价格服务器提供给定合约的价格，可以直接等待返回和立即返回，如果后者，价格则通过ptPriceUpdate返回
	LOADFUNCTION(ptPriceSnapshot);
	LOADFUNCTION(ptSetEncryptionCode);
	LOADFUNCTION(ptSetHandShakePeriod);
	LOADFUNCTION(ptSetHostHandShake);
	LOADFUNCTION(ptSetPriceHandShake);
	LOADFUNCTION(ptSetInternetUser);
	LOADFUNCTION(ptSetClientPath);
	LOADFUNCTION(ptGetErrorMessage);
	LOADFUNCTION(ptDumpLastError);
	LOADFUNCTION(ptSnapdragonEnabled);
	LOADFUNCTION(ptSubscribeBroadcast);
	LOADFUNCTION(ptUnsubscribeBroadcast);
	LOADFUNCTION(ptCountFills);
	LOADFUNCTION(ptGetFill);
	LOADFUNCTION(ptGetFillByID);
	LOADFUNCTION(ptGetContractPosition);
	LOADFUNCTION(ptGetOpenPosition);
	LOADFUNCTION(ptGetTotalPosition);
    //INT ptGetAveragePrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, PricePtr Price);
	//ptGetAveragePrice函数返回交易用户一个合约下全部挂单的平均价格
	LOADFUNCTION(ptGetAveragePrice);
	LOADFUNCTION(ptCountOrders);
	LOADFUNCTION(ptGetOrder);
	LOADFUNCTION(ptGetOrderByID);
	LOADFUNCTION(ptGetOrderIndex);
	//IN ptBlankPrices()
	//ptBlankPrices函数通常用于客户端和价格服务器失去连接用时清空API存储的价格数据，使ptGetPrice全部返回0
	LOADFUNCTION(ptBlankPrices);
	//INT ptGetPrice(INT Index, PriceStructPtr CurrentPrice);
	//ptGetPrice函数返回一个合约的当前价格，index指定那个合约价格被返回，值从0到ptCountContracts – 1.
	LOADFUNCTION(ptGetPrice);
    //INT  ptGetPriceForContract(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, PriceStructPtr CurrentPrice);
	//ptGetPriceForContract函数返回一个合约的当前价格，采用ExchangeName，ContractName，ContractDate确定一个合约
	LOADFUNCTION(ptGetPriceForContract);
	//INT  ptGetGenericPrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT PriceType, INT Side, PriceStructPtr Price);
	//ptGetGenericPrice函数容许用户获取PriceStruct中特定的价格项
	LOADFUNCTION(ptGetGenericPrice);
	//INT  ptAddOrder(NewOrderStructPtr NewOrder, OrderIDPtr OrderID);
	//ptAddOrder函数提交一个新报单到交易服务器，报单状态通过ptOrder回调函数返回.根据返回的报单ID使用ptGetOrderByID获取详细信息
	LOADFUNCTION(ptAddOrder);
	//INT  ptAddAlgoOrder(NewOrderStructPtr NewOrder, INT BuffSize, AlgoBuff AlgoBuffer, OrderIDPtr OrderID);
	//ptAddAlgoOrder工作方式同ptAddOrder，只是附加了一个被ALGO使用的XML字符串
	LOADFUNCTION(ptAddAlgoOrder);
    //INT ptGetOrderEx(INT Index, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize);
	LOADFUNCTION(ptGetOrderEx);
	LOADFUNCTION(ptGetOrderByIDEx);
	LOADFUNCTION(ptAddProtectionOrder);
	LOADFUNCTION(ptAddBatchOrder);
	LOADFUNCTION(ptAddAggregateOrder);
	LOADFUNCTION(ptAddCustRequest);
	LOADFUNCTION(ptReParent);
	LOADFUNCTION(ptDoneForDay);
	LOADFUNCTION(ptAddCrossingOrder);
	LOADFUNCTION(ptAddBlockOrder);
	LOADFUNCTION(ptAddBasisOrder);
	LOADFUNCTION(ptAddAAOrder);
	//LOADFUNCTION(ptGetConsolidatedPosition);
	//改单
	LOADFUNCTION(ptAmendOrder);
	//改单，使用Algo XML
	LOADFUNCTION(ptAmendAlgoOrder);
	//撤销一个指定报单
	LOADFUNCTION(ptCancelOrder);
	LOADFUNCTION(ptActivateOrder);
	LOADFUNCTION(ptDeactivateOrder);
	//撤销特定交易账户下全部买单
	LOADFUNCTION(ptCancelBuys);
	//撤销特定交易账户下全部卖单
	LOADFUNCTION(ptCancelSells);
    //撤销一个特定账户下某个交易所某个合约下全部报单
	LOADFUNCTION(ptCancelOrders);
	//撤销特定交易账户下全部报单
	LOADFUNCTION(ptCancelAll);
	LOADFUNCTION(ptOrderChecked);
	LOADFUNCTION(ptQueryOrderStatus);
	//INT  ptCountOrderHistory(INT Index, PINT Count);
	//ptCountOrderHistory函数返回为给定报单持有的报单历史记录的数量
	LOADFUNCTION(ptCountOrderHistory);
	//INT  ptGetOrderHistory(INT  Index, INT Position, OrderDetailStructPtr OrderDetail);
	//ptGetOrderHistory函数返回报单明细，数据是按逆时间次序返回
	LOADFUNCTION(ptGetOrderHistory);
	//INT ptGetOrderHistoryEx(INT  Index, INT Position, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize);
	//ptGetOrderHistoryEx函数除了像ptGetOrderHistory返回报单明细外，还返回一个Algo XML数据和这个数据的大小
	LOADFUNCTION(ptGetOrderHistoryEx);
	LOADFUNCTION(ptSetUserIDFilter);
	LOADFUNCTION(ptNextOrderSequence);
	LOADFUNCTION(ptLogOn);
	LOADFUNCTION(ptLogOff);
	LOADFUNCTION(ptGetLogonStatus);
	LOADFUNCTION(ptDOMEnabled);
	LOADFUNCTION(ptPostTradeAmendEnabled);
	LOADFUNCTION(ptEnabledFunctionality);
	LOADFUNCTION(ptCountUsrMsgs);
	LOADFUNCTION(ptGetUsrMsg);
	LOADFUNCTION(ptGetUsrMsgByID);
	LOADFUNCTION(ptAcknowledgeUsrMsg);
	//INT ptPriceStep(double Price, double TickSize, INT NumSteps, INT TicksPerPoint);
	//ptPriceStep是价格计算函数，return Price+TickSize*NumSteps*TicksPerPoint
	LOADFUNCTION(ptPriceStep);
	LOADFUNCTION(ptPLBurnRate);
	LOADFUNCTION(ptOpenPositionExposure);
	LOADFUNCTION(ptBuyingPowerRemaining);
	LOADFUNCTION(ptBuyingPowerUsed);
	LOADFUNCTION(ptMarginForTrade);
	LOADFUNCTION(ptTotalMarginPaid);
	LOADFUNCTION(ptGetMarginPerLot);
	LOADFUNCTION(ptSetOrderCancelFailureDelay);
	LOADFUNCTION(ptSetOrderSentFailureDelay);
	LOADFUNCTION(ptSetOrderQueuedFailureDelay);
	LOADFUNCTION(ptCountContractAtBest);
	LOADFUNCTION(ptGetContractAtBest);
	LOADFUNCTION(ptGetContractAtBestPrices);
	LOADFUNCTION(ptCountContractSubscriberDepth);
	LOADFUNCTION(ptGetContractSubscriberDepth);
	LOADFUNCTION(ptSuperTASEnabled);
	LOADFUNCTION(ptSetSSL);
	LOADFUNCTION(ptSetPDDSSL);
	LOADFUNCTION(ptSetMDSToken);
	LOADFUNCTION(ptSetSSLCertificateName);
	LOADFUNCTION(ptSetPDDSSLCertificateName);
	LOADFUNCTION(ptSetSSLClientAuthName);
	LOADFUNCTION(ptSetPDDSSLClientAuthName);
	LOADFUNCTION(ptSetSuperTAS);
	LOADFUNCTION(ptSetMemoryWarning);
	LOADFUNCTION(ptCreateStrategy);
	LOADFUNCTION(ptGetOptionPremium);
	LOADFUNCTION(ptLockUpdates);
	LOADFUNCTION(ptUnlockUpdates);
/*交易函数***************************************************************************************/
    //INT ptAddOrderEx(NewOrderStructPtr NewOrder, OrderIDPtr OrderID, UserNameStrPtr UserName);
	//工作情况同ptAddOrder，只是附加了一个登录用户名，用于多用户网关程序
	LOADFUNCTION(ptAddOrderEx);
	//改单，附加用户名
	LOADFUNCTION(ptAmendOrderEx);
	//撤销一个指定报单，附加用户名
	LOADFUNCTION(ptCancelOrderEx);
	//撤销特定交易账户下全部买单，附加用户名
	LOADFUNCTION(ptCancelBuysEx);
	//撤销特定交易账户下全部卖单，附加用户名
	LOADFUNCTION(ptCancelSellsEx);
	return bRet; 
}