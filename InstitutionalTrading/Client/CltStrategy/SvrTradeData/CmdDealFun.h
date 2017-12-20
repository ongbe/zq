#pragma once
#include "CommonPkg.h"


//查询合约
bool QryInstrumentDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询费率
bool QryCommissionRateDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询保证金率
bool QryMarginRateDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询资金
bool QryUserAccountInfoDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询持仓
bool QryUserPositionDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询持仓明细
bool QryUserPositionDetailDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询组合持仓明细
bool QryUserComPositionDetailDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询报单
bool QryUserOrderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询成交
bool QryUserTraderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//出入金请求
bool ReqFundInOut(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询历史出入金请求
bool ReqQryFundInOut(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询结算确认请求
bool ReqQrySettlementConfirm(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询委托交易账号的状态
bool ReqQryTraderAccountStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//手动启动某一个委托交易帐号的初始化
bool ReqQryTraderAccountInit(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//结算确认请求
bool ReqSettlementConfirm(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询用户初始化状态
bool ReqQryUserInitStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//初始化用户请求
bool ReqInitNewUser(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//强制初始化模块
bool ReForceInitModule(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//初始化模块
bool ReInitModule(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询初始化模块时间
bool QryReinitTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//设置初始化模块时间
bool SetReinitTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//获取当前状态
bool QryCurrentStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询初始化模块时间
bool QryTradingTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//设置初始化模块时间
bool SetTradingTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//请求查询客户通知响应
bool QryNotice(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//期货发起查询银行余额通知
bool QryContractBank(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//请求查询预埋单
bool QryParkedOrder(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//请求查询预埋撤单
bool QryParkedOrderAction(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//请求查询深度行情
bool QryDepthMarket(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//////主服务响应的请求
//请求查询交易日
bool SlaverQryTradingDay(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//请求同步报单和成交
bool SlaverQrySyncReq(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//请求行情
bool SlaverReqMasterQuotSend(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


/////从服务响应的请求
//请求查询交易日响应
bool SlaverQryTradingDayRsp(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//响应报单push
bool SlaverRecMasterOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应成交push
bool SlaverRecMasterTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应主帐号报单push
bool SlaverRecMasterCTPOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应主帐号成交push
bool SlaverRecMasterCTPTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应行情push
bool SlaverRecMasterQuotPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//响应费率push
bool SlaverRecMasterQryCommissionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应保证金push
bool SlaverRecMasterQryMarginPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应资金push
bool SlaverRecMasterQryFundPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应持仓push
bool SlaverRecMasterQryPositionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应持仓明细push
bool SlaverRecMasterQryPositionDetailPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应用户初始化
bool SlaverRecUserInitPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应主帐号的查询报单
bool SlaverRecMasterQryOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应主帐号的查询成交
bool SlaverRecMasterQryTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//请求同步报单和成交
bool SlaverQrySyncRsp(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//响应下单错误
bool SlaverRecMasterErrOrderInsertPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//响应撤单错误
bool SlaverRecMasterErrOrderActionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);