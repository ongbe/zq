#pragma once

#include <string>
#include <set>
#include <map>
using namespace std;

#include "UserApiStruct.h"

class ForceManualForm2
{
public:
    ForceManualForm2(void);
    ~ForceManualForm2(void);

    static ForceManualForm2* NewInstance() {return new ForceManualForm2();}
    static void DestroyInstance(ForceManualForm2* instance) {if(instance!=NULL) delete instance;}
public:
    //订阅报单返回
    static void SubscribeOrderRsp(unsigned int hWnd);
    //报单推送
    static void SubscribeOrderPush(unsigned int hWnd);
    //退订报单返回
    static void UnsubscribeOrderRsp(unsigned int hWnd);

    //订阅深度行情返回
    static void SubscribeQuotRsp(unsigned int hWnd);
    //深度行情推送
    static void SubscribeQuotPush(unsigned int hWnd);
    //退订深度行情返回
    static void UnsubscribeQuotRsp(unsigned int hWnd);

    //订阅持仓返回
    static void SubscribePositionRsp(unsigned int hWnd);
    //持仓推送
    static void SubscribePositionPush(unsigned int hWnd);
    //退订持仓返回
    static void UnsubscribePositionRsp(unsigned int hWnd);

    //订阅资金返回
    static void SubscribeFundRsp(unsigned int hWnd);
    //资金推送
    static void SubscribeFundPush(unsigned int hWnd);
    //退订资金返回
    static void UnsubscribeFundRsp(unsigned int hWnd);

    //订阅强平报单录入返回
    static void SubscribeForceCloseOrderInsertRsp(unsigned int hWnd);

    //订阅成交返回
    static void SubscribeTradeRsp(unsigned int hWnd);
    //成交推送
    static void SubscribeTradePush(unsigned int hWnd);
    //退订成交返回
    static void UnsubscribeTradeRsp(unsigned int hWnd);

    //退订全部事件
    static void UnsubscribeAllEvt(unsigned int hWnd);

    //向服务器订阅报单请求
    static void ReqSubscribeOrder(int accountID); 
    //向服务器订阅深度行情请求
    static void ReqSubscribeQuot(TInstrumentIDType szInstrumentID); 
    //向服务器退订深度行情请求
    static void UnreqSubscribeQuot(TInstrumentIDType szInstrumentID); 
    //向服务器订阅持仓请求
    static void ReqSubscribePosition(int accountID);  
    //向服务器订阅资金请求
    static void ReqSubscribeFund(int accountID);
    //向服务器请求强平报单录入
    static void ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField);

    //处理服务器推送的风险事件消息
    static bool ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt);

public:
    void SetOrderRefToId(std::string szOrderRef,int nCount);
    void GetOrderRefToId(std::map<std::string,int>& mapOrderRefToId);
    void DeleteOrderRefToId(std::string orderRef);
    void ResetOrderRefToId(int nId);
private:
    std::map<std::string,int> m_mapOrderRefToId;//挂单Ref对应行号
};