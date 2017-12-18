#include "StdAfx.h"
#include "ForceShortcutForm2.h"

#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"


ForceShortcutForm2::ForceShortcutForm2(void)
{
}

ForceShortcutForm2::~ForceShortcutForm2(void)
{
}

//订阅深度行情返回
void ForceShortcutForm2::SubscribeQuotRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeQuot_Rsp,(int)hWnd);
}
//深度行情推送
void ForceShortcutForm2::SubscribeQuotPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Quot_Push,(int)hWnd);
}
//退订深度行情返回
void ForceShortcutForm2::UnsubscribeQuotRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeQuot_Rsp,(int)hWnd);
}
//订阅资金返回
void ForceShortcutForm2::SubscribeFundRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp,(int)hWnd);
}
//资金推送
void ForceShortcutForm2::SubscribeFundPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push,(int)hWnd);
}
//退订资金返回
void ForceShortcutForm2::UnsubscribeFundRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeFund_Rsp,(int)hWnd);
}
//订阅强平报单录入返回
void ForceShortcutForm2::SubscribeForceCloseOrderInsertRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_ForceCloseOrderInsert_Rsp,(int)hWnd);
}
//订阅成交返回
void ForceShortcutForm2::SubscribeTradeRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeTrade_Rsp,(int)hWnd);
}
//成交推送
void ForceShortcutForm2::SubscribeTradePush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Trade_Push,(int)hWnd);
}
//退订成交返回
void ForceShortcutForm2::UnsubscribeTradeRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeTrade_Rsp,(int)hWnd);
}
//退订全部事件
void ForceShortcutForm2::UnsubscribeAllEvt(unsigned int hWnd)
{
    CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}
//向服务器订阅深度行情请求
void ForceShortcutForm2::ReqSubscribeQuot(TInstrumentIDType szInstrumentID)
{   
   CTcpLayer::SendData(Cmd_RM_SubscribeQuot_Req, (void*)szInstrumentID, sizeof(TInstrumentIDType), 0);
}
//向服务器订阅资金请求
void ForceShortcutForm2::ReqSubscribeFund(int nAccount)
{
    CTcpLayer::SendData(Cmd_RM_SubscribeFund_Req, (void*)&nAccount, sizeof(int), 0);
}
//向服务器订阅成交请求
void ForceShortcutForm2::ReqSubscribeTrade(int nAccount)
{
    CTcpLayer::SendData(Cmd_RM_SubscribeTrade_Req, (void*)&nAccount, sizeof(int), 0);
}
//向服务器请求强平报单录入
void ForceShortcutForm2::ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField)
{
    CTcpLayer::SendData(Cmd_RM_ForceCloseOrderInsert_Req, (void*)&oRiskForceCloseOrderField, sizeof(RiskForceCloseOrderField), 0);
}
//处理服务器推送的风险事件消息
bool ForceShortcutForm2::ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt)
{
    for(int i=0;i<fieldcnt;i++)
    {
        char buf[256];
        itoa(i,buf,10);
        pstr[i].assign(buf);
    }
    return true;
}
