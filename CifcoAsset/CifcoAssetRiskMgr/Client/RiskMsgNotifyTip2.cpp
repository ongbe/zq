#include "StdAfx.h"
#include "RiskMsgNotifyTip2.h"
#include "RiskManageCmd.h"
#include "TcpLayer.h"

CRiskMsgNotifyTip2::CRiskMsgNotifyTip2(void)
{
    m_MessageIndex = 0;
    m_CurMessageIndex = 0;
    m_mapMessageInfo.clear();
}

CRiskMsgNotifyTip2::~CRiskMsgNotifyTip2(void)
{
}

//退订全部事件
void CRiskMsgNotifyTip2::UnsubscribeAllEvt(unsigned int hWnd)
{
    CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}

//订阅消息推送响应
void CRiskMsgNotifyTip2::SubscribeAddMessagePush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SendUnReadMessage_Push,(int)hWnd);
}

void CRiskMsgNotifyTip2::SetMessageIndex(int nCount)
{
    m_CurMessageIndex = nCount;
}

void CRiskMsgNotifyTip2::SetMessageInfo(MessageInfo& oMessageInfo)
{
    m_mapMessageInfo.insert(std::pair<int,MessageInfo>(m_CurMessageIndex,oMessageInfo));
}

MessageInfo* CRiskMsgNotifyTip2::GetPreMessageInfo()
{    
    m_CurMessageIndex--;
    std::map<int,MessageInfo>::iterator it = m_mapMessageInfo.find(m_CurMessageIndex);
    if(it == m_mapMessageInfo.end())
        return NULL;
    else 
        return &(it->second);
}

MessageInfo* CRiskMsgNotifyTip2::GetNextmessageInfo()
{
    m_CurMessageIndex++;
    std::map<int,MessageInfo>::iterator it = m_mapMessageInfo.find(m_CurMessageIndex);
    if(it == m_mapMessageInfo.end())
        return NULL;
    else 
        return &(it->second);
}

int CRiskMsgNotifyTip2::GetMessageInfoCount()
{
    return m_CurMessageIndex;
}