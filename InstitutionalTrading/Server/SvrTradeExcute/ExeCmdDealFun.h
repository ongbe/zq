
#pragma once
#include "CommonPkg.h"
//
//void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo);
//void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder);

//广播
bool BroadcastBufToClient(const int nRspID,const char* nUserId,PVOID buf,int nBufLen,int nRequestID = 0);
//下单
bool InputOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//撤单
bool ActionOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//处理报单的审核结果
bool VerifyedOrderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//查询该风控员所属的交易员的待审核报单
bool QryApprovingOrderInfo(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//下单
bool StragegyInputOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//撤单
bool StragegyActionOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);