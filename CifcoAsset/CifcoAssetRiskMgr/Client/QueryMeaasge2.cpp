#include "StdAfx.h"
#include "QueryMeaasge2.h"

//#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include "UserApiStruct.h"

QueryMeaasge2::QueryMeaasge2(void)
{
	m_pdataInfo = CDataInfo::NewInstance();
}

QueryMeaasge2::~QueryMeaasge2(void)
{
}

//订阅事件
void QueryMeaasge2::SubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::SubscribePkg(Cmd_RM_QrySendMessage_Rsp,(int)hWnd);
	CTcpLayer::SubscribePkg(Cmd_RM_QryMsgSendStatus_Rsp,(int)hWnd);
	CTcpLayer::SubscribePkg(Cmd_RM_QryRecvMessage_Rsp,(int)hWnd);
}
//退订事件
void QueryMeaasge2::UnsubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}


//向服务器请求风控员消息通知
void QueryMeaasge2::ReqQueryMessage(std::string strDate,int flag)
{
	 char szDate[11];
	 memcpy(szDate,(const void *)(strDate.c_str()),sizeof(szDate)-1);
	 TrustTradeAccount  account;
	 memset(&account,0,sizeof(account));
	 /*std::string userName = "";
	 m_pdataInfo->GetUserName(userName);
	 m_pdataInfo->GetTrustTradeAccount(userName, account);*/
	 LoginRsp loginRsp;
	 memset(&loginRsp, 0, sizeof(loginRsp));
	 m_pdataInfo->GetLoginRsp(loginRsp);
	 MessageQryCondition Condition;
    memset(&Condition,0,sizeof(Condition));
	Condition.nRiskMgmtUserID = loginRsp.nAccountID;
	//Condition.nRiskMgmtUserID = 43;//测试代码

	//sprintf(pszTime, "%4d-%02d-%02d", strDate,
	strncpy(Condition.szCreateDate, strDate.c_str(),sizeof(Condition.szCreateDate)-1);

	if(flag == QUERY_SEND)
	{
		CTcpLayer::SendData(Cmd_RM_QrySendMessage_Req,(void *)&Condition, sizeof(Condition), 0);
		CTcpLayer::SendData(Cmd_RM_QryMsgSendStatus_Req,(void *)&Condition, sizeof(Condition), 0);
	}
	else if(flag == QUERY_RECV)
	{

		CTcpLayer::SendData(Cmd_RM_QryRecvMessage_Req,(void *)&Condition, sizeof(Condition), 0);
	}
}

//处理消息
bool QueryMeaasge2::ProcessMessageInfoEvtPkg(const char* pPkg,std::map<int, MessageInfo>& mapMessageInfo,int fieldcnt)
{
	for ( int i = 0; i < fieldcnt; i++ )
	{
		MessageInfo* pField = (MessageInfo*)((char*)(pPkg+sizeof(Stru_UniPkgHead)) + i*sizeof(MessageInfo));
		mapMessageInfo[pField->nMessageID]  =	*pField;
	}
		



	return true;
}

bool QueryMeaasge2::ProcessMessageStatusEvtPkg(const char* pPkg,std::multimap<int, MsgSendStatus>& mapMsgSendStatus,int fieldcnt)
{
		for ( int i = 0; i < fieldcnt; i++ )
	{
		MsgSendStatus* pField = (MsgSendStatus*)((char*)(pPkg+sizeof(Stru_UniPkgHead)) + i*sizeof(MsgSendStatus));
		mapMsgSendStatus.insert(make_pair(pField->nRiskMgmtUserID,  *pField));
	}
		



	return true;

}

