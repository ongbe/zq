#pragma once

#include <set>
#include <string>
using namespace std;

#include "UserApiStruct.h"

enum CLOSE_TYPE
{
    CLOSE_TYPE_TODAY,
    CLOSE_TYPE_YESTERDAY
};

enum RISK_STATUS
{
    RISK_STATUS_HANDLING
};

enum FORCE_OPERATER
{
    FORCE_OPERATER_MANUAL
};
struct ForceShortcutFormStruct
{
    int nRiskID; 
    int nAccountID;
    int nHandleRiskPersonID;
    int nHandledRiskPersonID;
    RISK_STATUS nRiskStatus;
    FORCE_OPERATER nForceOperater;
    string sTime;
    string sRemark;
};

class ForceShortcutForm2
{
public:
    ForceShortcutForm2(void);
    ~ForceShortcutForm2(void);
public:
    //订阅深度行情返回
    static void SubscribeQuotRsp(unsigned int hWnd);
    //深度行情推送
    static void SubscribeQuotPush(unsigned int hWnd);
    //退订深度行情返回
    static void UnsubscribeQuotRsp(unsigned int hWnd);

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

    //向服务器订阅深度行情请求
    static void ReqSubscribeQuot(TInstrumentIDType szInstrumentID); 
    //向服务器订阅资金请求
    static void ReqSubscribeFund(int nAccount);
    //向服务器订阅成交请求
    static void ReqSubscribeTrade(int nAccount);
    //向服务器请求强平报单录入
    static void ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField);
    

    //处理服务器推送的风险事件消息
    static bool ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt);
};
