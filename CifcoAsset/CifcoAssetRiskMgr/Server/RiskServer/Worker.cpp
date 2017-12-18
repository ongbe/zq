#include "stdafx.h"
#include <iostream>
#include "Tools/WriteLog.h"
#include "Tools/TcpServerWorker.h"
#include "ThreadSharedData.h"
#include "SocketCommon.h"
#include "RiskMsgQueue.h"
#include "Worker.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "ToolThread.h"
using namespace std;

CWorker::CWorker(CThreadSharedData* pThreadSharedData)
: m_pWriteLog(NULL)
, m_hWorkerThread(NULL)
, m_pThreadSharedData(pThreadSharedData)
{
	m_backSysTime=GetTodayString();
	m_bFirstIdle = true;
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE, "Worker.log");
	m_hWorkerThread =new CToolThread(ThreadFunc, this);
}

CWorker::~CWorker(void)
{
	SAFE_DELETE(m_hWorkerThread);
	SAFE_DELETE ( m_pWriteLog);
}

DWORD CWorker::ThreadFunc( void* pData )
{
	CWorker* pWorker = (CWorker*)pData;
	if(pWorker)
		pWorker->Process();
	return 0;
}
void CWorker::Process()
{
	if (NULL==m_hWorkerThread||NULL == m_pThreadSharedData)
	{
		return;
	}
	while( !m_hWorkerThread->IsNeedExit() )
	{
		//OutputDebugString("worker thread running...\n");
		RequestData reqData;
		Risk2ClientData rcdata={0};
		if ( m_pThreadSharedData->GetMsg(reqData))
		{
			ProcessRequest(reqData);

			if ( reqData.pData != NULL )
			{
				delete [] reqData.pData;
				reqData.pData = NULL;
			}
		}
		else if(CRisk2ClientMsgQueue::Get()&&CRisk2ClientMsgQueue::Get()->GetMsg(rcdata))
		{
			ProcessSubscribedData(rcdata);
		}
		else
		{
			//空闲时自动执行动作
			OnIdle();

			Sleep(10);
			continue;
		}
	}
}

void CWorker::ProcessRequest( const RequestData& reqData )
{
	TRACE("Start process request: ModuleID: %d, CMDID: %d\n", 
		reqData.head.moduleid, reqData.head.cmdid  );

	if((!CDataCenter::Get())||!(CDataCenter::Get()->IsOK()))
	{
		string strError = "服务器未完成初始化或者CTP连接中断。";
		RspResultData(reqData.hSocket, OPERATOR_FAILED, reqData.head.cmdid+1, 
			(void*)strError.c_str(), strError.length() + 1, reqData.head.seq);
	}
	else
	{
		if ( reqData.head.cmdid == Cmd_RM_Login_Req )
		{
			ProcessLogonRequest(reqData);
		}
		else
		{
			//判断是否已登录，未登录的请求不予处理
			if ( !IsLogon(reqData.hSocket ))
			{
				string strError = "操作错误，账号未登录！";
				RspResultData(reqData.hSocket, OPERATOR_FAILED, reqData.head.cmdid+1, 
					(void*)strError.c_str(), strError.length() + 1, reqData.head.seq);
			}
			else
			{
				PROCESS_MAP_BEGIN(reqData.head.cmdid)	
					PROCESS_ITEM(Cmd_RM_Account,ProcessAccountRequest)
					PROCESS_ITEM(Cmd_RM_TradeAccount,ProcessTradeAccountRequest)
					PROCESS_ITEM(Cmd_RM_Trader,ProcessTraderRequest)
					PROCESS_ITEM(Cmd_RM_AssetMgmtOrg,ProcessAssetMgmtOrgRequest)
					PROCESS_ITEM(Cmd_RM_FinancialProduct,ProcessFinancialProductRequest)
					PROCESS_ITEM(Cmd_RM_RiskIndModule,ProcessRiskIndModuleRequest)
					PROCESS_ITEM(Cmd_RM_RiskWarning,ProcessRiskWarningRequest)
					PROCESS_ITEM(Cmd_RM_RiskIndicator,ProcessRiskIndicatorRequest)
					PROCESS_ITEM(Cmd_RM_EvtMsgTemplate,ProcessEvtMsgTemplateRequest)
					PROCESS_ITEM(Cmd_RM_RiskEvent,ProcessRiskEventRequest)
					PROCESS_ITEM(Cmd_RM_RiskEvtHandling,ProcessRiskEvtHandlingRequest)
					PROCESS_ITEM(Cmd_RM_Permission,ProcessPermissionRequest)
					PROCESS_ITEM(Cmd_RM_Message,ProcessMessageRequest)
					PROCESS_ITEM(Cmd_RM_Instrument,ProcessInstrumentRequest)
					PROCESS_ITEM(Cmd_RM_SubscribeDeposit,ProcessDepositRequest)
					PROCESS_ITEM(Cmd_RM_SubscribeTrade,ProcessTradeRequest)
					PROCESS_ITEM(Cmd_RM_SubscribeOrder,ProcessOrderRequest)			
					PROCESS_ITEM(Cmd_RM_SubscribeFund,ProcessFundRequest)	
					PROCESS_ITEM(Cmd_RM_SubscribePosition,ProcessPositionRequest)			
					PROCESS_ITEM(Cmd_RM_SubscribeQuot,ProcessQuotRequest)
					PROCESS_ITEM(Cmd_RM_ForceCloseDo,ProcessForceCloseRequest)
					PROCESS_ITEM(Cmd_RM_Statistics,ProcessStatisticsRequest)
					PROCESS_ITEM(Cmd_RM_Gather,ProcessGatherRequest)
				PROCESS_MAP_END()

			}
		}
	}

	TRACE("End process request: ModuleID: %d, CMDID: %d\n", reqData.head.moduleid, reqData.head.cmdid);


	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("业务处理完成：", WriteLogLevel_DEBUGINFO, 
			"ModuleId = %d, CmdId = %d",
			reqData.head.moduleid, reqData.head.cmdid);
	}
   
}

void CWorker::RspResultData( SOCKET hSocket, int nOperateRet, int nCmdID, 
								void* pData, int nLen, int nSeq,
								int nCurPkgIndex, int nTotalPkg )
{
	if ( pData == NULL && nLen != 0)
	{
		return;
	}

	Stru_UniPkgHead head;
	head.moduleid = 0;
	head.cmdid = nCmdID;
	head.len = nLen;
	head.userdata1 = nOperateRet;
	head.userdata2 = nCurPkgIndex;
	head.userdata3 = nTotalPkg;
	head.seq = nSeq;

	int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+sizeof(TAIL);
	char* pBuf = new char[nTotalLen];
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return;
	}

	memcpy(pBuf, &head, sizeof(Stru_UniPkgHead));
	memcpy(pBuf+sizeof(Stru_UniPkgHead), pData, nLen);
	TAIL tail = TailID_UniPkg;
	memcpy(pBuf+sizeof(Stru_UniPkgHead)+head.len, &tail, sizeof(TAIL));


	TRACE("Start send response: OperateRet: %d, CMDID: %d\n", nOperateRet, nCmdID);


	WSASetLastError(0);
	int nSendLen = CSocketBase::SendDataTcp(hSocket, pBuf, nTotalLen);
	// 错误码10035表示缓存已满，稍微等待，继续发送
	while ( nSendLen == SOCKET_ERROR && WSAGetLastError() == 10035)
	{
		Sleep(10);
		nSendLen = CSocketBase::SendDataTcp(hSocket, pBuf, nTotalLen);
	}

	TRACE("End send response: OperateRet: %d, CMDID: %d,TotalLen: %d, SendLen: %d, TotalPkg: %d, CurPkg: %d\n", 
		nOperateRet, nCmdID, nTotalLen, nSendLen, nTotalPkg, nCurPkgIndex);
}

void CWorker::OutputUnkownReqLog(const RequestData& reqData)
{
	TRACE("Unkown request data, ModuleID: %d, CmdID: %d, data length: %d", 
		reqData.head.moduleid, reqData.head.cmdid,reqData.head.len);
}

void CWorker::ProcessLogonRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_Login_Req:
		{
			if ( reqData.head.len != sizeof(LoginReq) )
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			LoginReq* pLogon = (LoginReq*)reqData.pData;
			LoginRsp rsp;
			memset(&rsp, 0, sizeof(rsp));
			if ( !GET_DBACCESS().ExcuteLogon(pLogon->szAccount, pLogon->szPassword, pLogon->nRoleType, rsp))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Login_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				//更新在线用户
				if( NULL != m_pThreadSharedData )
				{
					m_pThreadSharedData->UpdateOnlineUser(reqData.hSocket, rsp.nAccountID);
				}

				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_Login_Rsp, 
					&rsp, sizeof(rsp), reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

bool CWorker::GetOnlineStatus( int nAccountID )
{
	return (m_pThreadSharedData&&m_pThreadSharedData->GetOnlineStatus(nAccountID));
}

bool CWorker::IsLogon( SOCKET hSocket )
{
	return (m_pThreadSharedData&&m_pThreadSharedData->FindUser(hSocket));
}

void CWorker::ForceCloseClient( int nAccountID )
{
	std::set<SOCKET> tempSet;
	std::set<SOCKET>::const_iterator it;
	if (m_pThreadSharedData&&m_pThreadSharedData->GetOnlineUserSocket(nAccountID, tempSet) )
	{
		for (it = tempSet.begin() ; it != tempSet.end(); ++it )
		{
			RspResultData(*it, OPERATOR_SUCCESS, Cmd_RM_ForceCloseClient_Push,NULL, 0);
		}
	}
}

void CWorker::OnIdle()
{
	if ( m_bFirstIdle )
	{
		DeleteExpiredWaitForSendAccounts();
		m_bFirstIdle = false;
	}
	else
	{
		//判断是否跨天
		int today=GetTodayString();
		if (today!= m_backSysTime)
		{
			DeleteExpiredWaitForSendAccounts();
			m_backSysTime = today;
		}
	}
}

void CWorker::SendMessage( std::vector<MsgSendStatus> &vForeAccount, MessageInfo msgInfo )
{
	//给在线用户发送消息
	if ( NULL == m_pThreadSharedData )
	{
		return;
	}

	std::vector<MsgSendStatus>::iterator it = vForeAccount.begin();
	for ( ; it != vForeAccount.end(); it++)
	{
		(*it).nSendStatus = 0;
		std::set<SOCKET> tempSet;
		if ( !m_pThreadSharedData->GetOnlineUserSocket((*it).nRiskMgmtUserID, tempSet) )
		{
			continue;
		}

		if ( tempSet.empty())
		{
			continue;
		}

		std::set<SOCKET>::iterator it_socket = tempSet.begin();
		for ( ; it_socket != tempSet.end(); it_socket++ )
		{
			RspResultData( *it_socket, OPERATOR_SUCCESS, Cmd_RM_SendUnReadMessage_Push, 
				&msgInfo, sizeof(msgInfo), 0);
		}

		(*it).nSendStatus = 1;
	}

	//未发送的信息保存，待下次用户登录时发送
	GET_DBACCESS().SaveMessageSendStatus(vForeAccount);
}

void CWorker::DeleteExpiredWaitForSendAccounts()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "delete from MESSAGEWAITFORSEND t1 where t1.messageid in \
		(select t2.messageid from MESSAGEINFO t2 where (select trunc(sysdate) from dual) > t2.expireddate)");
	GET_DBACCESS().Excute(szBuffer);
}

void CWorker::ProcessAccountRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_ModifyPwd_Req:
		{
			if ( reqData.head.len != sizeof(ModifyPwd))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			ModifyPwd* pUserInfo = (ModifyPwd*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "update RISKMGMTUSERINFO t set t.password = '%s' where t.riskmgmtuserid = %d",
				pUserInfo->szPassword, pUserInfo->nRiskMgmtUserID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyPwd_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该用户可能已被删除或者未被启用";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyPwd_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyPwd_Rsp, 
						NULL, 0, reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_AddAccount_Req:
		{
			if ( reqData.head.len != sizeof(RiskMgmtUserInfo))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			RiskMgmtUserInfo* pAccountInfo = (RiskMgmtUserInfo*)reqData.pData;
			char szFinField[20];
			memset(szFinField, 0, sizeof(szFinField));
			if ( pAccountInfo->nFinancialProductID > 0 )
			{
				sprintf(szFinField, "%d", pAccountInfo->nFinancialProductID);
			}
			else
			{
				strcpy(szFinField, "NULL");
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into RISKMGMTUSERINFO values(SEQ_RISKMGMTUSERID.Nextval, '%s','%s',%d,\
							  %d,%s,%d,'%s','%s',%d)",
							  pAccountInfo->szAccount, pAccountInfo->szPassword, pAccountInfo->nRoleType,
							  pAccountInfo->nAssetMgmtOrgID, szFinField, pAccountInfo->nStatus,
							  pAccountInfo->szName, pAccountInfo->szContactInfo, 
							  pAccountInfo->nPermissionID);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_RISKMGMTUSERID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddAccount_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddAccount_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyAccount_Req:
		{
			if ( reqData.head.len != sizeof(RiskMgmtUserInfo))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			RiskMgmtUserInfo* pAccountInfo = (RiskMgmtUserInfo*)reqData.pData;

			char szFinField[20];
			memset(szFinField, 0, sizeof(szFinField));
			if ( pAccountInfo->nFinancialProductID > 0 )
			{
				sprintf(szFinField, "%d", pAccountInfo->nFinancialProductID);
			}
			else
			{
				strcpy(szFinField, "NULL");
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update RISKMGMTUSERINFO t set t.password = '%s',t.assetmgmtorgid = %d,\
				t.financialproductid = %s,t.status = %d,t.name = '%s',t.contactinfo = '%s',t.permissionid = %d \
				where t.riskmgmtuserid = %d",
				pAccountInfo->szPassword,pAccountInfo->nAssetMgmtOrgID,
				szFinField, pAccountInfo->nStatus, 
				pAccountInfo->szName, pAccountInfo->szContactInfo,
				pAccountInfo->nPermissionID, pAccountInfo->nRiskMgmtUserID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyAccount_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该用户可能已被删除";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyAccount_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyAccount_Rsp, 
						&pAccountInfo->nRiskMgmtUserID, sizeof(int), reqData.head.seq);

					//账号被停用时，强制关闭客户端
					if ( pAccountInfo->nStatus == 0 )
					{
						ForceCloseClient(pAccountInfo->nRiskMgmtUserID);
					}
				}
			}
		}
		break;
	case Cmd_RM_DelAccount_Req:
		{
			if ( reqData.head.len % sizeof(int) != 0 ||
				reqData.head.len == 0 )
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nCount = reqData.head.len / sizeof(int);
			bool bHasError = false;
			std::vector<int> vDelAccount;
			char szBuffer[MAX_SQL_LENGTH];
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)reqData.pData + i*sizeof(int));
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "delete from RISKMGMTUSERINFO t where t.riskmgmtuserid = %d", nAccountID);
				if ( !GET_DBACCESS().Excute(szBuffer))
				{
					bHasError = true;
				}
				else
				{
					vDelAccount.push_back(nAccountID);
				}
			}

			if ( bHasError )
			{
				string strError = "部分账号删除失败！";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelAccount_Rsp, 
					(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelAccount_Rsp, 
					vDelAccount, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryAccountByOrgID_Req: 
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nOrgID = *(int*)reqData.pData;
			//char szBuffer[MAX_SQL_LENGTH];
			//memset(szBuffer, 0, sizeof(szBuffer));
			//sprintf(szBuffer, "select t.riskmgmtuserid,t.account,t.password,t.roletype,t.assetmgmtorgid,t.financialproductid,\
			//				  t.status,t.name,t.contactinfo,t.permissionid from RISKMGMTUSERINFO t \
			//				  where t.assetmgmtorgid = %d", nOrgID);
			//vector<RiskMgmtUserInfo> vec;
			//if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			//{
			//	RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryAccountByOrgID_Rsp, 
			//		(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
			//		GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			//	return;
			//}
			//else
			//{
			//	RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryAccountByOrgID_Rsp,
			//		vec, reqData.head.seq);
			//}

			std::vector<AssetMgmtOrganization> vOrg;
			if ( !GetLowerAssetOrg(nOrgID, vOrg))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryAccountByOrgID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
				return;
			}

			vector<RiskMgmtUserInfo> vec;
			char szBuffer[MAX_SQL_LENGTH];
			for( UINT i = 0; i < vOrg.size(); i++ )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select t.riskmgmtuserid,t.account,t.password,t.roletype,t.assetmgmtorgid,t.financialproductid,\
								  t.status,t.name,t.contactinfo,t.permissionid from RISKMGMTUSERINFO t \
								  where t.assetmgmtorgid = %d", vOrg[i]);

				if ( !GET_DBACCESS().QueryData(szBuffer, vec))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryAccountByOrgID_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}
			}

			RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryAccountByOrgID_Rsp,
				vec, reqData.head.seq);
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessTradeAccountRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(TrustTradeAccount))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			TrustTradeAccount* pAccountInfo = (TrustTradeAccount*)reqData.pData;
			char szFinField[20];
			memset(szFinField, 0, sizeof(szFinField));
			if ( pAccountInfo->nFinancialProductID > 0 )
			{
				sprintf(szFinField, "%d", pAccountInfo->nFinancialProductID);
			}
			else
			{
				strcpy(szFinField, "NULL");
			}

			char szTraderIDField[20];
			memset(szTraderIDField, 0, sizeof(szTraderIDField));
			if ( pAccountInfo->nTraderID > 0)
			{
				sprintf(szTraderIDField, "%d", pAccountInfo->nTraderID);
			}
			else
			{
				strcpy(szTraderIDField, "NULL");
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into TRUSTTRADEACCOUNT values(SEQ_TRADEACCOUNTID.Nextval, '%s',%d,\
							  %s,%d,%s,%f,'%s','%s',%d,'%s','%s')",
							  pAccountInfo->szAccount, pAccountInfo->nAssetMgmtOrgID,
							  szFinField, pAccountInfo->nRiskIndModuleID, szTraderIDField,
							  pAccountInfo->dProfit, pAccountInfo->szName, 
							  pAccountInfo->szContactInfo, pAccountInfo->nIDCardType,
							  pAccountInfo->szIDCardNo, pAccountInfo->szPwd);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_TRADEACCOUNTID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddTradeAccount_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddTradeAccount_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(TrustTradeAccount))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			TrustTradeAccount* pAccountInfo = (TrustTradeAccount*)reqData.pData;
			char szFinField[20];
			memset(szFinField, 0, sizeof(szFinField));
			if ( pAccountInfo->nFinancialProductID > 0 )
			{
				sprintf(szFinField, "%d", pAccountInfo->nFinancialProductID);
			}
			else
			{
				strcpy(szFinField, "NULL");
			}

			char szTraderIDField[20];
			memset(szTraderIDField, 0, sizeof(szTraderIDField));
			if ( pAccountInfo->nTraderID > 0)
			{
				sprintf(szTraderIDField, "%d", pAccountInfo->nTraderID);
			}
			else
			{
				strcpy(szTraderIDField, "NULL");
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update TRUSTTRADEACCOUNT t set t.assetmgmtorgid = %d,\
				t.financialproductid = %s,t.riskindmoduleid = %d,t.traderid = %s,\
				t.profit = %f,t.name = '%s',t.contactinfo = '%s',\
				t.idcardtype = %d,t.idcardno = '%s',t.password = '%s' \
				where t.tradeaccountid = %d",
				pAccountInfo->nAssetMgmtOrgID,szFinField,
				pAccountInfo->nRiskIndModuleID, szTraderIDField, 
				pAccountInfo->dProfit, pAccountInfo->szName,pAccountInfo->szContactInfo,
				pAccountInfo->nIDCardType,pAccountInfo->szIDCardNo,
				pAccountInfo->szPwd, pAccountInfo->nTradeAccountID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyTradeAccount_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该用户可能已被删除";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyTradeAccount_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyTradeAccount_Rsp, 
						&pAccountInfo->nTradeAccountID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_DelTradeAccount_Req:
		{
			if ( reqData.head.len % sizeof(int) != 0 ||
				reqData.head.len == 0 )
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nCount = reqData.head.len / sizeof(int);
			bool bHasError = false;
			std::vector<int> vDelAccount;
			char szBuffer[MAX_SQL_LENGTH];
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)reqData.pData + i*sizeof(int));
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "delete from TRUSTTRADEACCOUNT t where t.tradeaccountid = %d", nAccountID);
				if ( !GET_DBACCESS().Excute(szBuffer))
				{
					bHasError = true;
				}
				else
				{
					vDelAccount.push_back(nAccountID);
				}
			}

			if ( bHasError )
			{
				string strError = "部分账号删除失败！";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelTradeAccount_Rsp, 
					(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelTradeAccount_Rsp, 
					vDelAccount, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryTradeAccountByOrgID_Req: 
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nOrgID = *(int*)reqData.pData;
			std::vector<AssetMgmtOrganization> vOrg;
			if ( !GetLowerAssetOrg(nOrgID, vOrg))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryTradeAccountByOrgID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
				return;
			}

			vector<TrustTradeAccount> vec;
			char szBuffer[MAX_SQL_LENGTH];
			for( UINT i = 0; i < vOrg.size(); i++ )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select * from TRUSTTRADEACCOUNT t where t.assetmgmtorgid = %d", vOrg[i]);

				if ( !GET_DBACCESS().QueryData(szBuffer, vec))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryTradeAccountByOrgID_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}
			}

			RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryTradeAccountByOrgID_Rsp,
				vec, reqData.head.seq);
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessTraderRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddTrader_Req:
		{
			if ( reqData.head.len != sizeof(TraderInfo))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			TraderInfo* pTraderInfo = (TraderInfo*)reqData.pData;
			char szFinField[20];
			memset(szFinField, 0, sizeof(szFinField));
			if ( pTraderInfo->nFinancialProductID > 0 )
			{
				sprintf(szFinField, "%d", pTraderInfo->nFinancialProductID);
			}
			else
			{
				strcpy(szFinField, "NULL");
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into TRADERINFO values(SEQ_TRADERID.Nextval, \
							  '%s',%s,%d,'%s','%s','%s')",
							  pTraderInfo->szName, szFinField, 
							  pTraderInfo->nAssetMgmtOrgID, pTraderInfo->szTel,
							  pTraderInfo->szMobile, pTraderInfo->szOtherContactInfo);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_TRADERID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddTrader_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddTrader_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyTrader_Req:
		{
			if ( reqData.head.len != sizeof(TraderInfo))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			TraderInfo* pTraderInfo = (TraderInfo*)reqData.pData;
			char szFinField[20];
			memset(szFinField, 0, sizeof(szFinField));
			if ( pTraderInfo->nFinancialProductID > 0 )
			{
				sprintf(szFinField, "%d", pTraderInfo->nFinancialProductID);
			}
			else
			{
				strcpy(szFinField, "NULL");
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update TRADERINFO t set t.assetmgmtorgid = %d,\
				t.financialproductid = %s, t.tel = '%s', t.mobile = '%s', \
				t.othercontactinfo = '%s' where t.traderid = %d",
				pTraderInfo->nAssetMgmtOrgID, szFinField, 
				pTraderInfo->szTel, pTraderInfo->szMobile,
				pTraderInfo->szOtherContactInfo, pTraderInfo->nTraderID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyTrader_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该用户可能已被删除";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyTrader_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyTrader_Rsp, 
						&pTraderInfo->nTraderID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_DelTrader_Req:
		{
			if ( reqData.head.len % sizeof(int) != 0 ||
				reqData.head.len == 0 )
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nCount = reqData.head.len / sizeof(int);
			bool bHasError = false;
			std::vector<int> vDelAccount;
			char szBuffer[MAX_SQL_LENGTH];
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)reqData.pData + i*sizeof(int));
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "delete from TRADERINFO t where t.traderid = %d", nAccountID);
				if ( !GET_DBACCESS().Excute(szBuffer))
				{
					bHasError = true;
				}
				else
				{
					vDelAccount.push_back(nAccountID);
				}
			}

			if ( bHasError )
			{
				string strError = "部分账号删除失败！";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelTrader_Rsp, 
					(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelTrader_Rsp, 
					vDelAccount, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryTraderByOrgID_Req: 
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nOrgID = *(int*)reqData.pData;
			std::vector<AssetMgmtOrganization> vOrg;
			if ( !GetLowerAssetOrg(nOrgID, vOrg))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryTraderByOrgID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
				return;
			}

			vector<TraderInfo> vec;
			char szBuffer[MAX_SQL_LENGTH];
			for( UINT i = 0; i < vOrg.size(); i++ )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select * from TRADERINFO t where t.assetmgmtorgid = %d", vOrg[i]);

				if ( !GET_DBACCESS().QueryData(szBuffer, vec))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryTraderByOrgID_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}
			}

			RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryTraderByOrgID_Rsp,
				vec, reqData.head.seq);
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessAssetMgmtOrgRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddAssetMgmtOrg_Req:
		{
			if ( reqData.head.len != sizeof(AssetMgmtOrganization))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			AssetMgmtOrganization* pOrgInfo = (AssetMgmtOrganization*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into ASSETMGMTORGANIZATION values(SEQ_ASSETMGMTORGID.Nextval, \
							 '%s', %d,%d,%.6f,%.6f)",
							pOrgInfo->szName, pOrgInfo->nUpperLevelOrgID,
							pOrgInfo->nScaleLevel, pOrgInfo->dLongitude, pOrgInfo->dLatitude);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_ASSETMGMTORGID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddAssetMgmtOrg_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddAssetMgmtOrg_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyAssetMgmtOrg_Req:
		{
			if ( reqData.head.len != sizeof(AssetMgmtOrganization))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			AssetMgmtOrganization* pOrgInfo = (AssetMgmtOrganization*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update ASSETMGMTORGANIZATION t set t.name = '%s',t.longitude = %.6f, \
				t.latitude = %.6f where t.assetmgmtorgid = %d",
				pOrgInfo->szName, pOrgInfo->dLongitude, 
				pOrgInfo->dLatitude, pOrgInfo->nAssetMgmtOrgID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyAssetMgmtOrg_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyAssetMgmtOrg_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyAssetMgmtOrg_Rsp, 
						&pOrgInfo->nAssetMgmtOrgID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_DelAssetMgmtOrg_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nOrgID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete ASSETMGMTORGANIZATION t where t.assetmgmtorgid = %d", nOrgID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelAssetMgmtOrg_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelAssetMgmtOrg_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelAssetMgmtOrg_Rsp, 
						&nOrgID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_QryAssetMgmtOrgByOrgID_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nOrgID = *(int*)reqData.pData;
			vector<AssetMgmtOrganization> vecOut;
			if ( !GetLowerAssetOrg(nOrgID, vecOut))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryAssetMgmtOrgByOrgID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryAssetMgmtOrgByOrgID_Rsp,
					vecOut, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryAssetMgmtOrg_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from ASSETMGMTORGANIZATION t");
			vector<AssetMgmtOrganization> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryAssetMgmtOrg_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryAssetMgmtOrg_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessFinancialProductRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddFinancialProduct_Req:
		{
			if ( reqData.head.len != sizeof(FinancialProduct))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			FinancialProduct* pProductInfo = (FinancialProduct*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into FINANCIALPRODUCT values(SEQ_FINANCIALPRODUCTID.Nextval, \
							  '%s','%s')",
							  pProductInfo->szName,pProductInfo->szScopeDesc);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_FINANCIALPRODUCTID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddFinancialProduct_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddFinancialProduct_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyFinancialProduct_Req:
		{
			if ( reqData.head.len != sizeof(FinancialProduct))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			FinancialProduct* pProductInfo = (FinancialProduct*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update FINANCIALPRODUCT t set t.name = '%s', \
				t.scopedesc = '%s' where t.financialproductid = %d",
				pProductInfo->szName, pProductInfo->szScopeDesc, 
				pProductInfo->nFinancialProductID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyFinancialProduct_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyFinancialProduct_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyFinancialProduct_Rsp, 
						&pProductInfo->nFinancialProductID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_DelFinancialProduct_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nProductID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete FINANCIALPRODUCT t where t.financialproductid = %d", nProductID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelFinancialProduct_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelFinancialProduct_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelFinancialProduct_Rsp, 
						&nProductID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_QryFinancialProduct_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from FINANCIALPRODUCT t");
			vector<FinancialProduct> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryFinancialProduct_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryFinancialProduct_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryAssetOrgProRelation_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PRODUCTANDORGRELATION t");
			vector<AssetOrgProRelation> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryAssetOrgProRelation_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryAssetOrgProRelation_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_SaveAssetOrgProRelation_Req:
		{
			if ( !(reqData.head.len >= sizeof(int) && 
				reqData.head.len % sizeof(int) == 0))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nFinID = *(int*)reqData.pData;
			int nCount = (reqData.head.len - sizeof(int)) / sizeof(int);
			vector<int> vOrgID;
			for ( int i = 0; i < nCount; i++ )
			{
				int nOrgID = *(int*)((char*)reqData.pData + sizeof(int)*(i+1));
				vOrgID.push_back(nOrgID);
			}

			if ( !GET_DBACCESS().SaveFinOrgRelation(nFinID, vOrgID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_SaveAssetOrgProRelation_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SaveAssetOrgProRelation_Rsp, 
					&nFinID, sizeof(nFinID), reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessRiskIndModuleRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddRiskIndModule_Req:
		{
			if ( reqData.head.len != sizeof(RiskIndicatorModule))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			RiskIndicatorModule* pModule = (RiskIndicatorModule*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into RISKINDICATORMODULE values(SEQ_RISKINDMODULEID.Nextval, '%s')",
							  pModule->szName);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_RISKINDMODULEID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddRiskIndModule_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddRiskIndModule_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyRiskIndModule_Req:
		{
			if ( reqData.head.len != sizeof(RiskIndicatorModule))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			RiskIndicatorModule* pModule = (RiskIndicatorModule*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update RISKINDICATORMODULE t set t.name = '%s' where t.riskindmoduleid = %d",
				pModule->szName, pModule->nRiskIndModuleID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyRiskIndModule_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyRiskIndModule_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyRiskIndModule_Rsp, 
						&pModule->nRiskIndModuleID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_DelRiskIndModule_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nModuleID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete RISKINDICATORMODULE t where t.riskindmoduleid = %d", nModuleID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelRiskIndModule_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelRiskIndModule_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelRiskIndModule_Rsp, 
						&nModuleID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_QryRiskIndModule_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from RISKINDICATORMODULE t");
			vector<RiskIndicatorModule> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryRiskIndModule_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryRiskIndModule_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryValidRiskIndModule_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from RISKINDICATORMODULE t1 where t1.riskindmoduleid \
							  in(select t2.riskindmoduleid from RISKWARNING t2)");
			vector<RiskIndicatorModule> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryValidRiskIndModule_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryValidRiskIndModule_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessRiskWarningRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_QryRiskWarning_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from RISKWARNING t order by t.riskindmoduleid, t.riskindicatorid, t.risklevelid");
			vector<RiskWarning> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryRiskWarning_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryRiskWarning_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_SaveRiskWarning_Req:
		{
			if ( !(reqData.head.len >= sizeof(int) && 
				(reqData.head.len - sizeof(int))% sizeof(IndLevelWarning) == 0))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nModuleID = *(int*)reqData.pData;
			int nCount = (reqData.head.len - sizeof(int)) / sizeof(IndLevelWarning);
			vector<IndLevelWarning> vRiskWarning;
			for ( int i = 0; i < nCount; i++ )
			{
				IndLevelWarning levelWarning = *(IndLevelWarning*)((char*)reqData.pData + sizeof(int) + sizeof(IndLevelWarning)*i);
				vRiskWarning.push_back(levelWarning);
			}

			if ( !GET_DBACCESS().SaveRiskWarning(nModuleID, vRiskWarning))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_SaveRiskWarning_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SaveRiskWarning_Rsp, 
					&nModuleID, sizeof(nModuleID), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryMaxRiskLevel_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select riskindicatorid, max(risklevelid) from \
							  (select t1.riskindicatorid,t1.risklevelid from PRODUCTTHRESHOLD t1\
							  union select t2.riskindicatorid,t2.risklevelid from RISKWARNING t2) \
							  group by riskindicatorid order by riskindicatorid");
			vector<MaxRiskLevel> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryMaxRiskLevel_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryMaxRiskLevel_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryProductRiskWarning_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nTraderID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PRODUCTTHRESHOLD t\
							  where t.tradeaccountid = %d \
							  order by t.productid, t.riskindicatorid, t.risklevelid",
							  nTraderID);
			vector<ProductRiskWarning> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryProductRiskWarning_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryProductRiskWarning_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_SaveProductRiskWarning_Req:
		{
			if ( !(reqData.head.len >= sizeof(ProducrRiskWarningKey) && 
				(reqData.head.len - sizeof(ProducrRiskWarningKey))% sizeof(IndLevelWarning) == 0))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			ProducrRiskWarningKey key = *(ProducrRiskWarningKey*)reqData.pData;
			int nCount = (reqData.head.len - sizeof(ProducrRiskWarningKey)) / sizeof(IndLevelWarning);
			vector<IndLevelWarning> vRiskWarning;
			for ( int i = 0; i < nCount; i++ )
			{
				IndLevelWarning levelWarning = *(IndLevelWarning*)((char*)reqData.pData + sizeof(ProducrRiskWarningKey) + sizeof(IndLevelWarning)*i);
				vRiskWarning.push_back(levelWarning);
			}

			if ( !GET_DBACCESS().SaveProductRiskWarning(key, vRiskWarning))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_SaveProductRiskWarning_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SaveProductRiskWarning_Rsp, 
					&key, sizeof(key), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_DelProductRiskWarning_Req:
		{
			if ( reqData.head.len != sizeof(ProducrRiskWarningKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			ProducrRiskWarningKey key = *(ProducrRiskWarningKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete PRODUCTTHRESHOLD t where t.tradeaccountid = %d and t.productid = '%s'", 
				key.nTraderID, key.szProductID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelProductRiskWarning_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelProductRiskWarning_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelProductRiskWarning_Rsp, 
						&key, sizeof(ProducrRiskWarningKey), reqData.head.seq);
				}
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessRiskIndicatorRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_QryRiskIndicator_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from RISKINDICATOR t order by t.riskindicatorid");
			vector<RiskIndicator> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryRiskIndicator_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryRiskIndicator_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessEvtMsgTemplateRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_QryEvtMsgTemplate_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from EVENTMESSAGETEMPLATE t");
			vector<EventMessageTemplate> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryEvtMsgTemplate_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryEvtMsgTemplate_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_SaveEvtMsgTemplate_Req:
		{
			if ( !(reqData.head.len >= sizeof(int) && 
				(reqData.head.len-sizeof(int)) % sizeof(EventMessageTemplate) == 0))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nIndicatorID = *(int*)reqData.pData;
			int nCount = (reqData.head.len - sizeof(int)) / sizeof(EventMessageTemplate);
			vector<EventMessageTemplate> vMsgTemplate;
			for ( int i = 0; i < nCount; i++ )
			{
				EventMessageTemplate msg = *(EventMessageTemplate*)((char*)reqData.pData + sizeof(int) + sizeof(EventMessageTemplate)*i);
				vMsgTemplate.push_back(msg);
			}

			if ( !GET_DBACCESS().SaveMsgTemplate(nIndicatorID, vMsgTemplate))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_SaveEvtMsgTemplate_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SaveEvtMsgTemplate_Rsp, 
					&nIndicatorID, sizeof(nIndicatorID), reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}
void CWorker::ProcessPermissionRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddPermissionName_Req:
		{
			if ( reqData.head.len != sizeof(PermissionName))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			PermissionName* pModule = (PermissionName*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into PERMISSIONNAME values(SEQ_PERMISSIONID.Nextval, '%s')",
				pModule->szName);
			int nPKID = 0;
			if ( !GET_DBACCESS().InsertAndReturnID(szBuffer, 
				"select SEQ_PERMISSIONID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddPermissionName_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddPermissionName_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_ModifyPermissionName_Req:
		{
			if ( reqData.head.len != sizeof(PermissionName))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			PermissionName* pPermissionName = (PermissionName*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update PERMISSIONNAME t set t.name = '%s' where t.permissionid = %d",
				pPermissionName->szName, pPermissionName->nPermissionID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyPermissionName_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_ModifyPermissionName_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ModifyPermissionName_Rsp, 
						&pPermissionName->nPermissionID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_DelPermissionName_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nModuleID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete PERMISSIONNAME t where t.permissionid = %d", nModuleID);
			int nNum = 0;
			if ( !GET_DBACCESS().ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelPermissionName_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_DelPermissionName_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_DelPermissionName_Rsp, 
						&nModuleID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_QryPermissionName_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSIONNAME t");
			vector<PermissionName> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryPermissionName_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryPermissionName_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryPermissionNameEx_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select distinct(t1.permissionid),t4.name,t3.nodetype from PERMISSIONVALUE t1\
							  left join PERMISSIONACTION t2 on t1.actionid = t2.actionid \
							  left join PERMISSIONNODE t3 on t2.nodeid = t3.nodeid \
							  left join PERMISSIONNAME t4 on t1.permissionid = t4.permissionid \
							  order by t1.permissionid");
			vector<PermissionNameEx> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryPermissionNameEx_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryPermissionNameEx_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryPermissionNode_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSIONNODE t");
			vector<PermissionNode> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryPermissionNode_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryPermissionNode_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryPermissionAction_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSIONACTION t");
			vector<PermissionAction> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryPermissionAction_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryPermissionAction_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryPermissionValue_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSIONVALUE t order by t.actionid");
			vector<PermissionValue> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryPermissionValue_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryPermissionValue_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryPermissionValueByID_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nPermissionID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSIONVALUE t where t.permissionid = %d order by t.actionid",
				nPermissionID);
			vector<PermissionValue> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryPermissionValueByID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryPermissionValueByID_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_SavePermissionValue_Req:
		{
			if ( !(reqData.head.len >= sizeof(int) && 
				(reqData.head.len-sizeof(int)) % sizeof(PermissionValue) == 0))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nPermissionID = *(int*)reqData.pData;
			int nCount = (reqData.head.len - sizeof(int)) / sizeof(PermissionValue);
			vector<PermissionValue> vPermission;
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionValue permission = *(PermissionValue*)((char*)reqData.pData + sizeof(int) + sizeof(PermissionValue)*i);
				vPermission.push_back(permission);
			}

			if ( !GET_DBACCESS().SavePermissionValue(nPermissionID, vPermission))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_SavePermissionValue_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SavePermissionValue_Rsp, 
					&nPermissionID, sizeof(nPermissionID), reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessMessageRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_AddMessage_Req:
		{
			if ( !(reqData.head.len > sizeof(MessageInfo) &&
				(reqData.head.len-sizeof(MessageInfo)) % sizeof(TargetAccount) == 0))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			MessageInfo msgInfo = *(MessageInfo*)reqData.pData;
			vector<TargetAccount> vTargetAccountID;
			int nCount = (reqData.head.len - sizeof(MessageInfo)) / sizeof(TargetAccount);
			for ( int i = 0; i < nCount; i++ )
			{
				TargetAccount account = *(TargetAccount*)((char*)reqData.pData + sizeof(MessageInfo) + i*sizeof(TargetAccount));
				vTargetAccountID.push_back(account);
			}

			int nPKID = 0;
			if ( !GET_DBACCESS().AddMessage(msgInfo, vTargetAccountID, nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddMessage_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddMessage_Rsp, 
					&nPKID, sizeof(nPKID), reqData.head.seq);

				msgInfo.nMessageID = nPKID;
				std::vector<MsgSendStatus> vForeAccount;
				for ( UINT i = 0; i < vTargetAccountID.size(); i++ )
				{
					MsgSendStatus msgSendStatus;
					memset(&msgSendStatus, 0, sizeof(msgSendStatus));
					msgSendStatus.nMessageID = nPKID;
					msgSendStatus.nRiskMgmtUserID = vTargetAccountID[i].nRiskMgmtUserID;
					safestrcpy(msgSendStatus.szAccount, sizeof(msgSendStatus.szAccount), vTargetAccountID[i].szAccount);
					msgSendStatus.nSendStatus = 0;
					vForeAccount.push_back(msgSendStatus);
				}

				SendMessage(vForeAccount, msgInfo);
			}
		}
		break;
	case Cmd_RM_QrySendMessage_Req:
		{
			if ( reqData.head.len != sizeof(MessageQryCondition))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			MessageQryCondition key = *(MessageQryCondition*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t.messageid,t.title,t.content,\
							  to_char(t.expireddate,'YYYY-MM-DD'),t.ownerid,\
							  to_char(t.createdate,'YYYY-MM-DD HH24:MI:SS'),t.ownername \
							  from MESSAGEINFO t where t.ownerid = %d and \
							  to_char(t.createdate,'YYYY-MM-DD') = '%s' order by t.messageid",
							  key.nRiskMgmtUserID, key.szCreateDate);
			vector<MessageInfo> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QrySendMessage_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QrySendMessage_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryRecvMessage_Req:
		{
			if ( reqData.head.len != sizeof(MessageQryCondition))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			MessageQryCondition key = *(MessageQryCondition*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t2.messageid,t2.title,t2.content,\
							  to_char(t2.expireddate,'YYYY-MM-DD'),t2.ownerid,\
							  to_char(t2.createdate,'YYYY-MM-DD HH24:MI:SS'),t2.ownername from MESSAGETARGET t1 \
							  left join MESSAGEINFO t2 on t1.messageid = t2.messageid \
							  where t1.riskmgmtuserid = %d and \
							  to_char(t2.createdate,'YYYY-MM-DD') = '%s' \
							  order by t2.messageid",
							  key.nRiskMgmtUserID, key.szCreateDate);
			vector<MessageInfo> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryRecvMessage_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryRecvMessage_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryMsgSendStatus_Req:
		{
			if ( reqData.head.len != sizeof(MessageQryCondition))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			MessageQryCondition key = *(MessageQryCondition*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t1.* from MESSAGETARGET t1 \
					left join Messageinfo t2 on t1.messageid = t2.messageid \
					where t2.ownerid = %d and \
					to_char(t2.createdate,'YYYY-MM-DD') = '%s' order by t2.messageid",
					key.nRiskMgmtUserID, key.szCreateDate);
			vector<MsgSendStatus> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryMsgSendStatus_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryMsgSendStatus_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryUnReadMessage_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			int nOwnerID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t2.messageid,t2.title,t2.content,t2.expireddate,\
							  t2.ownerid,t2.createdate,t2.ownername from messagetarget t1 left join messageinfo t2\
							  on t1.messageid = t2.messageid where t1.riskmgmtuserid = %d and t1.sendstatus=0 \
							  and t2.expireddate > (select trunc(sysdate) from dual)",
							  nOwnerID);
			vector<MessageInfo> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_SendUnReadMessage_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SendUnReadMessage_Rsp, 
					vec, reqData.head.seq);

				//发送成功后，删除已发送的数据
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer,  "update MESSAGETARGET t set t.sendstatus = 1 where t.riskmgmtuserid = %d",
					nOwnerID);
				GET_DBACCESS().Excute(szBuffer);
			}

		}
		break;
	default:
		break;
	}
}

void CWorker::FilterAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vAssetOrg, 
							 std::vector<AssetMgmtOrganization>& vOut )
{
	vOut.clear();
	bool bFind = false;
	std::map<int, map<int, AssetMgmtOrganization>> mapAssetOrgTree;
	for ( UINT i = 0; i < vAssetOrg.size(); i++ )
	{
		AssetMgmtOrganization assetOrg = vAssetOrg[i];
		if ( assetOrg.nAssetMgmtOrgID == nCurOrgID )
		{
			vOut.push_back(assetOrg);
			bFind = true;
		}

		std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
			mapAssetOrgTree.find(assetOrg.nUpperLevelOrgID);
		if ( it != mapAssetOrgTree.end() )
		{
			it->second.insert(make_pair(assetOrg.nAssetMgmtOrgID, assetOrg));
		}
		else
		{
			std::map<int, AssetMgmtOrganization> tempOrgMap;
			tempOrgMap.clear();
			tempOrgMap[assetOrg.nAssetMgmtOrgID] = assetOrg;
			mapAssetOrgTree[assetOrg.nUpperLevelOrgID] = tempOrgMap;
		}	
	}

	if ( bFind )
	{
		CalcLowerData(mapAssetOrgTree, nCurOrgID, vOut);
	}
}

void CWorker::CalcLowerData( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetOrgTree, 
							int nOwnerID, std::vector<AssetMgmtOrganization>& vOutData )
{
	std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_owner = 
		mapAssetOrgTree.find(nOwnerID);
	if ( it_owner == mapAssetOrgTree.end() )
	{
		return;
	}
	else
	{
		std::map<int, AssetMgmtOrganization> tempAssetOrgMap = it_owner->second;
		if ( tempAssetOrgMap.empty())
		{
			return;
		}

		std::map<int, AssetMgmtOrganization>::iterator it_asset = tempAssetOrgMap.begin();
		for ( ; it_asset != tempAssetOrgMap.end(); it_asset++ )
		{
			AssetMgmtOrganization info = it_asset->second;
			vOutData.push_back(info);

			CalcLowerData(mapAssetOrgTree, info.nAssetMgmtOrgID, vOutData);
		}
	}
}

bool CWorker::GetLowerAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vOut )
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from ASSETMGMTORGANIZATION t where t.assetmgmtorgid >= %d", nCurOrgID);
	vector<AssetMgmtOrganization> vec;
	if ( !GET_DBACCESS().QueryData(szBuffer, vec))
	{
		return false;
	}
	else
	{
		FilterAssetOrg(nCurOrgID, vec, vOut);
		return true;
	}
}

void CWorker::SendEventMessage( MessageInfo& msgInfo, vector<TargetAccount>& vec )
{
	int nPKID = 0;
	if ( !GET_DBACCESS().AddMessage(msgInfo, vec, nPKID))
	{
		return;
	}
	else
	{
		msgInfo.nMessageID = nPKID;
		std::vector<MsgSendStatus> vMsgSendStatus;
		for ( UINT i = 0; i < vec.size(); i++ )
		{
			MsgSendStatus msgSendStatus;
			memset(&msgSendStatus, 0, sizeof(msgSendStatus));
			msgSendStatus.nMessageID = nPKID;
			msgSendStatus.nRiskMgmtUserID = vec[i].nRiskMgmtUserID;
			safestrcpy(msgSendStatus.szAccount, sizeof(msgSendStatus.szAccount), vec[i].szAccount);
			msgSendStatus.nSendStatus = 0;
			vMsgSendStatus.push_back(msgSendStatus);
		}

		SendMessage(vMsgSendStatus, msgInfo);
	}
}
