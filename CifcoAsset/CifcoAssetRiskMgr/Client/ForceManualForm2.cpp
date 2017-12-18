#include "StdAfx.h"
#include "ForceManualForm2.h"

#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"

ForceManualForm2::ForceManualForm2(void)
{
}

ForceManualForm2::~ForceManualForm2(void)
{
}
//订阅报单返回
void ForceManualForm2::SubscribeOrderRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeOrder_Rsp,(int)hWnd);
}
//报单推送
void ForceManualForm2::SubscribeOrderPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Order_Push,(int)hWnd);
}
//退订报单返回
void ForceManualForm2::UnsubscribeOrderRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeOrder_Rsp,(int)hWnd);
}
//订阅深度行情返回
void ForceManualForm2::SubscribeQuotRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeQuot_Rsp,(int)hWnd);
}
//深度行情推送
void ForceManualForm2::SubscribeQuotPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Quot_Push,(int)hWnd);
}
//退订深度行情返回
void ForceManualForm2::UnsubscribeQuotRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeQuot_Rsp,(int)hWnd);
}
//订阅持仓返回
void ForceManualForm2::SubscribePositionRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribePosition_Rsp,(int)hWnd);
}
//持仓推送
void ForceManualForm2::SubscribePositionPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Position_Push,(int)hWnd);
}
//退订持仓返回
void ForceManualForm2::UnsubscribePositionRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribePosition_Rsp,(int)hWnd);
}
//订阅资金返回
void ForceManualForm2::SubscribeFundRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp,(int)hWnd);
}
//资金推送
void ForceManualForm2::SubscribeFundPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push,(int)hWnd);
}
//退订资金返回
void ForceManualForm2::UnsubscribeFundRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeFund_Rsp,(int)hWnd);
}
//订阅强平报单录入返回
void ForceManualForm2::SubscribeForceCloseOrderInsertRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_ForceCloseOrderInsert_Rsp,(int)hWnd);
}
//订阅成交返回
void ForceManualForm2::SubscribeTradeRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeTrade_Rsp,(int)hWnd);
}
//成交推送
void ForceManualForm2::SubscribeTradePush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Trade_Push,(int)hWnd);
}
//退订成交返回
void ForceManualForm2::UnsubscribeTradeRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeTrade_Rsp,(int)hWnd);
}
//退订全部事件
void ForceManualForm2::UnsubscribeAllEvt(unsigned int hWnd)
{
    CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}
//向服务器订阅报单请求
void ForceManualForm2::ReqSubscribeOrder(int accountID)
{    
    CTcpLayer::SendData(Cmd_RM_SubscribeOrder_Req, (void*)&(accountID), sizeof(int), 0);
}
//向服务器订阅深度行情请求
void ForceManualForm2::ReqSubscribeQuot(TInstrumentIDType szInstrumentID)
{   
    CTcpLayer::SendData(Cmd_RM_SubscribeQuot_Req, (void*)szInstrumentID, sizeof(TInstrumentIDType), 0);
}
//向服务器退订深度行情请求
void ForceManualForm2::UnreqSubscribeQuot(TInstrumentIDType szInstrumentID)
{   
    CTcpLayer::SendData(Cmd_RM_UnSubscribeQuot_Req, (void*)szInstrumentID, sizeof(TInstrumentIDType), 0);
}
//向服务器订阅持仓请求
void ForceManualForm2::ReqSubscribePosition(int accountID)
{    
    CTcpLayer::SendData(Cmd_RM_SubscribePosition_Req, (void*)&(accountID), sizeof(int), 0);
}
//向服务器订阅资金请求
void ForceManualForm2::ReqSubscribeFund(int accountID)
{
    CTcpLayer::SendData(Cmd_RM_SubscribeFund_Req, (void*)&(accountID), sizeof(int), 0);
}
//向服务器请求强平报单录入
void ForceManualForm2::ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField)
{
    CTcpLayer::SendData(Cmd_RM_ForceCloseOrderInsert_Req, (void*)&oRiskForceCloseOrderField, sizeof(RiskForceCloseOrderField), 0);
}
//处理服务器推送的风险事件消息
bool ForceManualForm2::ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt)
{
    for(int i=0;i<fieldcnt;i++)
    {
        char buf[256];
        itoa(i,buf,10);
        pstr[i].assign(buf);
    }
    return true;
}

void ForceManualForm2::SetOrderRefToId(std::string szOrderRef,int nCount)
{
    m_mapOrderRefToId.insert(std::pair<std::string,int>(szOrderRef,nCount));
}
void ForceManualForm2::GetOrderRefToId(std::map<std::string,int>& mapOrderRefToId)
{
    mapOrderRefToId = m_mapOrderRefToId;
}
void ForceManualForm2::DeleteOrderRefToId(std::string orderRef)
{
     std::map<std::string,int>::iterator it = m_mapOrderRefToId.find(orderRef);
     if (it!=m_mapOrderRefToId.end())
     {
         m_mapOrderRefToId.erase(it);
     }
}
void ForceManualForm2::ResetOrderRefToId(int nId)
{
    std::map<std::string,int>::iterator it = m_mapOrderRefToId.begin();
    for(;it != m_mapOrderRefToId.end();it++)
    {
        if (it->second > nId)                                          
            it->second -= 1; 
    }
}