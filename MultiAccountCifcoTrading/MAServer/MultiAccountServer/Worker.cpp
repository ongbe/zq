#include "stdafx.h"
#include <queue>
#include <iostream>
#include "Tools_Win32.h"
#include "WriteLog.h"
#include "Worker.h"
#include "TcpServerWorker.h"
#include "MultiAccountServer.h"
#include "MultiAccountCmd.h"
#include "MultiAccountStruct.h"

using namespace std;

extern char* g_pMASem;
extern int g_nMASemCount;
extern queue<RequestData> g_msgQueue;						//消息队列
extern Stru_IPAddress g_svrAddr;	//服务器地址
extern std::map<int, std::set<SOCKET>> g_OnlineUser;			//在线用户
extern std::set<SOCKET> g_OnlineUserSet;			//在线用户，便于查找使用

#define MAX_PKG_RECORDNUM	1000

Worker::Worker(const char* pUser, const char* pPwd, const char* pSID)
: m_pWriteLog(NULL)
, m_hWorkerThread(NULL)
{
	GetLocalTime(&m_backSysTime);
	m_bFirstIdle = true;
	m_dbAccess.Init( pUser, pPwd, pSID);
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE, "Worker.log");
	m_bExit = false;
	DWORD dwThreadID;
	m_hWorkerThread = CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, &dwThreadID);
}

Worker::~Worker(void)
{
	if( m_hWorkerThread )
	{
		m_bExit = true;
		DWORD dwExitCode;
		WaitForSingleObject( m_hWorkerThread, 8000 );
		if( GetExitCodeThread(m_hWorkerThread, &dwExitCode) !=0 && dwExitCode == STILL_ACTIVE )
		{
			TerminateThread( m_hWorkerThread, 0 );
		}

		CloseHandle( m_hWorkerThread );
		m_hWorkerThread = NULL;
	}

	if ( m_pWriteLog != NULL )
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}
}

DWORD Worker::ThreadFunc( void* pData )
{
	Worker* pWorker = (Worker*)pData;
	if ( NULL == pWorker )
	{
		return 0;
	}

	while( !pWorker->m_bExit )
	{
		if(g_pMASem) 
		{
			CTools_Win32::TakeWinSem(g_pMASem);
			g_nMASemCount++;
		}

		if ( g_msgQueue.empty())
		{
			if(g_pMASem) 
			{
				CTools_Win32::GiveWinSem(g_pMASem);
				g_nMASemCount--;
			}
			
			//空闲时自动执行动作
			pWorker->OnIdle();

			Sleep(10);
			continue;
		}

		RequestData reqData = g_msgQueue.front();
		g_msgQueue.pop();

		if(g_pMASem) 
		{
			CTools_Win32::GiveWinSem(g_pMASem);
			g_nMASemCount--;
		}

		pWorker->ProcessRequest(reqData);

		if ( reqData.pData != NULL )
		{
			delete [] reqData.pData;
			reqData.pData = NULL;
		}
	}

	return 0;
}

void Worker::ProcessRequest( RequestData& reqData )
{
	char szBuff[LOG_BUFFER_SIZE];
	memset(szBuff, 0, sizeof(szBuff));
	sprintf_s(szBuff, LOG_BUFFER_USABLE_SIZE, "Start process request: ModuleID: %d, CMDID: %d", 
		reqData.head.moduleid, reqData.head.cmdid);
	OutputDebugString( szBuff );

	if ( reqData.head.cmdid == Cmd_MA_Login_Req )
	{
		ProcessLogonRequest(reqData);
	}
	else
	{
		//判断是否已登录，未登录的请求不予处理
		if ( !IsLogon(reqData.hSocket ))
		{
			string strError = "操作错误，账号未登录！";
			RspResultData(reqData.hSocket, OPERATOR_FAILED, Cmd_MA_UnLogon_Rsp, 
				(void*)strError.c_str(), strError.length() + 1, reqData.head.seq);
		}
		else
		{
			if ( reqData.head.cmdid >= Cmd_MA_Account_Min && 
				reqData.head.cmdid <= Cmd_MA_Account_Max)
			{
				ProcessAccountRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_IDCard_Min && 
				reqData.head.cmdid <= Cmd_MA_IDCard_Max)
			{
				ProcessIDCardRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_Server_Min && 
				reqData.head.cmdid <= Cmd_MA_Server_Max)
			{
				ProcessServerRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_Broker_Min && 
				reqData.head.cmdid <= Cmd_MA_Broker_Max)
			{
				ProcessBrokerRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_TradeAccount_Min && 
				reqData.head.cmdid <= Cmd_MA_TradeAccount_Max)
			{
				ProcessTradeAccountRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_PermissionModule_Min && 
				reqData.head.cmdid <= Cmd_MA_PermissionModule_Max)
			{
				ProcessPermissionModuleRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_Permission_Min && 
				reqData.head.cmdid <= Cmd_MA_Permission_Max)
			{
				ProcessPermissionRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_Message_Min && 
				reqData.head.cmdid <= Cmd_MA_Message_Max)
			{
				ProcessMessageRequest(reqData);
			}
			else
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid, reqData.head.len);
			}
		}
	}

	memset(szBuff, 0, sizeof(szBuff));
	sprintf_s(szBuff, LOG_BUFFER_USABLE_SIZE, "End process request: ModuleID: %d, CMDID: %d", 
		reqData.head.moduleid, reqData.head.cmdid);
	OutputDebugString( szBuff );

	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("业务处理完成：", WriteLogLevel_DEBUGINFO, 
			"ModuleId = %d, CmdId = %d",
			reqData.head.moduleid, reqData.head.cmdid);
	}
	return;
}

void Worker::RspResultData( SOCKET hSocket, int nOperateRet, int nCmdID, 
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

	char szBuff[LOG_BUFFER_SIZE];
	memset(szBuff, 0, sizeof(szBuff));
	sprintf_s(szBuff, LOG_BUFFER_USABLE_SIZE, "Start send response: OperateRet: %d, CMDID: %d", 
		nOperateRet, nCmdID);
	OutputDebugString( szBuff );

	CSocketBase::SendDataTcp(hSocket, pBuf, nTotalLen);

	memset(szBuff, 0, sizeof(szBuff));
	sprintf_s(szBuff, LOG_BUFFER_USABLE_SIZE, "End send response: OperateRet: %d, CMDID: %d", 
		nOperateRet, nCmdID);
	OutputDebugString( szBuff );
}

template<typename T>
void Worker::RspResultDataAutoDividePkg( SOCKET hSocket, int nOperateRet, int nCmdID, 
										std::vector<T>& vData, int nSeq /* = 0 */ )
{
	if ( vData.empty())
	{
		RspResultData( hSocket, nOperateRet, nCmdID, 
			NULL, 0, nSeq);
	}
	else
	{
		int nTotalPkg = vData.size() / MAX_PKG_RECORDNUM;
		if ( vData.size() % MAX_PKG_RECORDNUM != 0 )
		{
			nTotalPkg += 1;
		}

		for ( int i = 0; i < nTotalPkg; i++ )
		{
			int nRecordNum = MAX_PKG_RECORDNUM;
			if ( i == nTotalPkg - 1 )
			{
				nRecordNum = vData.size() - MAX_PKG_RECORDNUM * i;
			}

			RspResultData( hSocket, nOperateRet, nCmdID, 
				&vData[i*MAX_PKG_RECORDNUM], sizeof(T)*nRecordNum, nSeq, i, nTotalPkg);
		}

	}
}

void Worker::OutputUnkownReqLog( int nModuleID, int nCmdID, int nLen )
{
	char szBuff[LOG_BUFFER_SIZE];
	memset(szBuff, 0, sizeof(szBuff));
	sprintf_s(szBuff, LOG_BUFFER_USABLE_SIZE, "Unkown request data, ModuleID: %d, CmdID: %d, data length: %d", 
		nModuleID, nCmdID, nLen);
	OutputDebugString( szBuff );
}

void Worker::ProcessLogonRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_Login_Req:
		{
			if ( reqData.head.len != sizeof(LoginReq) )
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			LoginReq* pLogon = (LoginReq*)reqData.pData;
			LoginRsp rsp;
			memset(&rsp, 0, sizeof(rsp));
			if ( !m_dbAccess.ExcuteLogon(pLogon->szAccount, pLogon->szPassword, pLogon->nAccountType, rsp))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_Login_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				//更新在线用户
				if(g_pMASem) 
				{
					CTools_Win32::TakeWinSem(g_pMASem);
					g_nMASemCount++;
				}

				std::map<int, std::set<SOCKET>>::iterator it_online = g_OnlineUser.find(rsp.nAccountID);
				if ( it_online != g_OnlineUser.end() )
				{
					it_online->second.insert( reqData.hSocket );
				}
				else
				{
					std::set<SOCKET> socketSet;
					socketSet.insert( reqData.hSocket );
					g_OnlineUser[rsp.nAccountID] = socketSet;
				}

				g_OnlineUserSet.insert(reqData.hSocket);

				if(g_pMASem) 
				{
					CTools_Win32::GiveWinSem(g_pMASem);
					g_nMASemCount--;
				}

				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_Login_Rsp, 
					&rsp, sizeof(rsp), reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessAccountRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_ModifyPwd_Req:
		{
			if ( reqData.head.len != sizeof(ModifyPwd))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			ModifyPwd* pUserInfo = (ModifyPwd*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "update ACCOUNT t set t.password = '%s' where t.accountid = %d",
				pUserInfo->szPassword, pUserInfo->nAccountID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyPwd_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该用户可能已被删除或者未被启用";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyPwd_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyPwd_Rsp, 
						NULL, 0, reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_AddAccount_Req:
		{
			if ( reqData.head.len != sizeof(AccountInfo))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			AccountInfo* pAccountInfo = (AccountInfo*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into ACCOUNT values(SEQ_ACCOUNTID.Nextval, '%s','%s',%d,\
				%d,'%s','%s',%d,'%s',%d,0,%d,%d,%d,to_date('%s','YYYY-MM-DD'))",
				pAccountInfo->szAccount, pAccountInfo->szPassword, pAccountInfo->nAccountType,
				pAccountInfo->nOwnerAccountID, pAccountInfo->szName, pAccountInfo->szContactInfo,
				pAccountInfo->nIDCardType, pAccountInfo->szIDCardNo, 
				pAccountInfo->nAccountStatus, pAccountInfo->nViewPermission1, 
				pAccountInfo->nViewPermission2, pAccountInfo->nPermissionID,
				pAccountInfo->szDate);
			int nPKID = 0;
			if ( !m_dbAccess.InsertAndReturnID(szBuffer, 
				"select SEQ_ACCOUNTID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddAccount_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyAccount_Req:
		{
			if ( reqData.head.len != sizeof(AccountInfo))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			AccountInfo* pAccountInfo = (AccountInfo*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update account t set t.password = '%s',t.permissionid = %d,\
				t.name = '%s',t.contactinfo = '%s',\
				t.Idcardtype = %d,t.idcardno = '%s',t.accountstatus = %d,\
				t.viewpermission1 = %d,t.viewpermission2 = %d, t.enddate = to_date('%s','YYYY-MM-DD') where t.accountid = %d",
				pAccountInfo->szPassword,pAccountInfo->nPermissionID,
				pAccountInfo->szName, pAccountInfo->szContactInfo, 
				pAccountInfo->nIDCardType, pAccountInfo->szIDCardNo,
				pAccountInfo->nAccountStatus, pAccountInfo->nViewPermission1,
				pAccountInfo->nViewPermission2, pAccountInfo->szDate,
				pAccountInfo->nAccountID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该用户可能已被删除";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyAccount_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyAccount_Rsp, 
						&pAccountInfo->nAccountID, sizeof(int), reqData.head.seq);

					//账号被停用时，强制关闭客户端
					if ( pAccountInfo->nAccountStatus == 0 )
					{
						ForceCloseClient(pAccountInfo->nAccountID);
					}
				}
			}
		}
		break;
	case Cmd_MA_DelAccount_Req:
		{
			if ( reqData.head.len % sizeof(int) != 0 ||
				reqData.head.len == 0 )
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nCount = reqData.head.len / sizeof(int);
			bool bHasError = false;
			bool bCaseCade = false;
			std::vector<int> vDelAccount;
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)reqData.pData + i*sizeof(int));
				bool bCaseCadeDeleted = false;
				bool bRet = DelAccount(nAccountID, bCaseCadeDeleted);
				if ( !bRet )
				{
					bHasError = true;
				}
				else
				{
					vDelAccount.push_back(nAccountID);
				}

				if ( bCaseCadeDeleted )
				{
					bCaseCade = true;
				}
			}

			if ( bHasError )
			{
				string strError = "部分账号删除失败！";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelAccount_Rsp, 
					(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
			}
			else
			{
				if ( bCaseCade )
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelAccountCaseCade_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelAccount_Rsp, 
						&vDelAccount[0], vDelAccount.size()*sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryALLAccount_Req:
		//{
		//	if ( reqData.head.len != 0)
		//	{
		//		OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
		//			reqData.head.len);
		//		return;
		//	}

		//	char szBuffer[MAX_SQL_LENGTH];
		//	memset(szBuffer, 0, sizeof(szBuffer));
		//	sprintf(szBuffer, "select * from ACCOUNT t order by t.accountid");
		//	vector<AccountInfo> vec;
		//	if ( !m_dbAccess.QueryData(szBuffer, vec))
		//	{
		//		RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryAccount_Rsp, 
		//			(void*)m_dbAccess.GetLastErrorString().c_str(), 
		//			m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
		//	}
		//	else
		//	{
		//		RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryAccount_Rsp,
		//			vec, reqData.head.seq);
		//	}
		//}
		break;
	case Cmd_MA_QryAccountByOwner_Req: //返回该账户有查看权限的数据
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerAccountID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t1.accountid, t1.account, t1.password, t1.accounttype, \
				t1.owner, t1.name, t1.contactinfo, t1.idcardtype, t1.idcardno, \
				t1.accountstatus, t1.logonstatus, t1.viewpermission1, \
				t1.viewpermission2, t1.permissionid, to_char(t1.enddate, 'YYYY-MM-DD'), \
				t2.account, t3.name, t4.idcardname from ACCOUNT t1 \
				left join ACCOUNT t2 on t1.owner = t2.accountid \
				left join PERMISSION t3 on t1.permissionid = t3.permissionid \
				left join IDCARD t4 on t1.idcardtype = t4.idcardtype \
				where t1.accountid >= %d order by t1.accountid", nOwnerAccountID);
			vector<AccountInfoEx> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				//筛选数据
				vector<AccountInfoEx> vecOut;
				FilterAccount(nOwnerAccountID, vec, vecOut);

				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryAccount_Rsp,
					vecOut, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessIDCardRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddIDCard_Req:
		{
			if ( reqData.head.len != sizeof(IDCardInfo))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			IDCardInfo* pIDCardInfo = (IDCardInfo*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into IDCARD values(SEQ_IDCARDTYPE.Nextval, '%s')",
							  pIDCardInfo->szIDCardName);
			int nPKID = 0;
			if ( !m_dbAccess.InsertAndReturnID(szBuffer, 
				"select SEQ_IDCARDTYPE.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddIDCard_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddIDCard_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyIDCard_Req:
		{
			if ( reqData.head.len != sizeof(IDCardInfo))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			IDCardInfo* pIDCardInfo = (IDCardInfo*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update IDCARD t set t.idcardname = '%s' where t.idcardtype = %d",
				pIDCardInfo->szIDCardName, pIDCardInfo->nIDCardType);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyIDCard_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyIDCard_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyIDCard_Rsp, 
						&pIDCardInfo->nIDCardType, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelIDCard_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nIDCardType = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete IDCARD t where t.idcardtype = %d", nIDCardType);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelIDCard_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelIDCard_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelIDCard_Rsp, 
						&nIDCardType, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryIDCard_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from IDCARD t order by t.idcardtype");
			vector<IDCardInfo> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryIDCard_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryIDCard_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryIDCard_Rsp, 
						&vec[0], sizeof(IDCardInfo)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessServerRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddServerGroup_Req:
		{
			if ( reqData.head.len != sizeof(ServerGroup))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			ServerGroup* pServerInfo = (ServerGroup*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into SERVERGROUP values(SEQ_SERVERNO.Nextval,'%s',%d, %d)",
				pServerInfo->szServerName, pServerInfo->nBrokerID, pServerInfo->nShare);
			int nPKID = 0;
			if ( !m_dbAccess.InsertAndReturnID(szBuffer, 
				"select SEQ_SERVERNO.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddServerGroup_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddServerGroup_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyServerGroup_Req:
		{
			if ( reqData.head.len != sizeof(ServerGroup))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			ServerGroup* pServerGroup = (ServerGroup*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update SERVERGROUP t set t.servername = '%s', t.SHAREWITHLOWER = %d where t.serverno = %d",
				pServerGroup->szServerName, pServerGroup->nShare, pServerGroup->nServerNo);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyServerGroup_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyServerGroup_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyServerGroup_Rsp, 
						&pServerGroup->nServerNo, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelServerGroup_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nServerNo = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete SERVERGROUP t where t.serverno = %d", nServerNo);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelServerGroup_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelServerGroup_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelServerGroup_Rsp, 
						&nServerNo, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryServerGroup_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nBrokerID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from SERVERGROUP t where t.brokerid = %d order by t.serverno",
				nBrokerID);
			vector<ServerGroup> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryServerGroup_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryServerGroup_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryServerGroup_Rsp, 
						&vec[0], sizeof(ServerGroup)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_AddServerAddr_Req:
		{
			if ( reqData.head.len != sizeof(ServerAddr))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			ServerAddr* pServerAddr = (ServerAddr*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into SERVERADDR values(SEQ_ADDRID.Nextval,%d,'%s',%d, %d)",
				pServerAddr->nServerNo, pServerAddr->szServerAddress, 
				pServerAddr->nPort, pServerAddr->nServerType);
			int nPKID = 0;
			if ( !m_dbAccess.InsertAndReturnID(szBuffer, 
				"select SEQ_ADDRID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddServerAddr_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddServerAddr_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyServerAddr_Req:
		{
			if ( reqData.head.len != sizeof(ServerAddr))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			ServerAddr* pServerAddr = (ServerAddr*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update SERVERADDR t set t.address = '%s',t.port = %d where t.addrid = %d",
				pServerAddr->szServerAddress, pServerAddr->nPort, pServerAddr->nAddrID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyServerAddr_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyServerAddr_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyServerAddr_Rsp, 
						&pServerAddr->nServerNo, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelServerAddr_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nAddrNo = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete SERVERADDR t where t.addrid = %d", nAddrNo);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelServerAddr_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelServerAddr_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelServerAddr_Rsp, 
						&nAddrNo, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryServerAddr_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nServerNo = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from SERVERADDR t where t.serverno = %d order by t.addrid", nServerNo);
			vector<ServerAddr> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryServerAddr_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryServerAddr_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryServerAddr_Rsp, 
						&vec[0], sizeof(ServerAddr)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessBrokerRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddBroker_Req:
		{
			if ( reqData.head.len != sizeof(BrokerInfo))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			BrokerInfo* pBrokerInfo = (BrokerInfo*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into BROKERINFO values(SEQ_BROKERID.Nextval,'%s', '%s', %d, %d)",
				pBrokerInfo->szCode, pBrokerInfo->szCompany, 
				pBrokerInfo->nShareWithLowerLever, pBrokerInfo->nOwnerAccountID);
			int nPKID;
			if ( !m_dbAccess.InsertAndReturnID(szBuffer, 
				"select SEQ_BROKERID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddBroker_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddBroker_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyBroker_Req:
		{
			if ( reqData.head.len != sizeof(BrokerInfo))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			BrokerInfo* pBrokerInfo = (BrokerInfo*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update BROKERINFO t set t.company = '%s',t.sharewithlowerlever = %d where t.brokerid = %d",
				pBrokerInfo->szCompany, pBrokerInfo->nShareWithLowerLever, pBrokerInfo->nBrokerID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyBroker_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyBroker_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyBroker_Rsp, 
						&(pBrokerInfo->nBrokerID), sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelBroker_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nBrokerID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete BROKERINFO t where t.brokerid = %d", nBrokerID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelBroker_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelBroker_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelBroker_Rsp, 
						&nBrokerID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryBroker_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerAccountID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from BROKERINFO t where t.owner = %d order by t.brokerid",
				nOwnerAccountID);
			vector<BrokerInfo> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryBroker_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryBroker_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryBroker_Rsp, 
						&vec[0], sizeof(BrokerInfo)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryUsableBroker_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nForeAccountID = *(int*)reqData.pData;
			vector<BrokerInfo> vec;
			if ( !m_dbAccess.QuerySharedBrokerInfo(nForeAccountID, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryUsableBroker_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryUsableBroker_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryUsableBroker_Rsp, 
						&vec[0], sizeof(BrokerInfo)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessTradeAccountRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(TradeAccount))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			TradeAccount* pTradeAccount = (TradeAccount*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into TRADEACCOUNT values('%s', '%s', %d, %d,%d, '%s',%d,%d)",
				pTradeAccount->szTradeAccount, pTradeAccount->szPassword,
				pTradeAccount->nOwnerAccountID, pTradeAccount->nBrokerID,
				pTradeAccount->nServerNo, pTradeAccount->szNickName,
				pTradeAccount->nID, pTradeAccount->nRowNo);
			if ( !m_dbAccess.Excute(szBuffer))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddTradeAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddTradeAccount_Rsp, 
					NULL, 0, reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(TradeAccount))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			TradeAccount* pTradeAccount = (TradeAccount*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update TRADEACCOUNT t set t.password = '%s',t.brokerid = %d,\
				t.serverno = %d,t.nickname = '%s',t.id = %d, t.rowno = %d \
				where t.tradeaccount = '%s' and t.owner = %d",
				pTradeAccount->szPassword, pTradeAccount->nBrokerID,
				pTradeAccount->nServerNo, pTradeAccount->szNickName,
				pTradeAccount->nID, pTradeAccount->nRowNo,
				pTradeAccount->szTradeAccount, pTradeAccount->nOwnerAccountID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyTradeAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyTradeAccount_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyTradeAccount_Rsp, 
						NULL, 0, reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(TradeAccountKey))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			TradeAccountKey key = *(TradeAccountKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete TRADEACCOUNT t where t.tradeaccount = '%s' and t.owner = %d", 
				key.szTradeAccount, key.nOwnerAccountID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelTradeAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelTradeAccount_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelTradeAccount_Rsp, 
						NULL, 0, reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from TRADEACCOUNT t where t.owner = %d", nOwnerID);
			vector<TradeAccount> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryTradeAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryTradeAccount_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryTradeAccount_Rsp, 
						&vec[0], sizeof(TradeAccount)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_SaveTradeAccount_Req:
		{
			if ( !(reqData.head.len >= sizeof(int) && 
				(reqData.head.len-sizeof(int)) % sizeof(TradeAccount) == 0))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerAccount = *(int*)reqData.pData;
			int nCount = reqData.head.len / sizeof(TradeAccount);
			vector<TradeAccount> vTradeAccount;
			for ( int i = 0; i < nCount; i++ )
			{
				TradeAccount tradeAccount = *(TradeAccount*)((char*)reqData.pData + sizeof(int) + sizeof(TradeAccount)*i);
				vTradeAccount.push_back(tradeAccount);
			}

			if ( !m_dbAccess.SaveTradeAccount(nOwnerAccount, vTradeAccount))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_SaveTradeAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_SaveTradeAccount_Rsp, 
						NULL, 0, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessPermissionModuleRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddPermissionModule_Req:
		{
			if ( reqData.head.len != sizeof(PermissionModule))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			PermissionModule* pPermissionModule = (PermissionModule*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into PERMISSIONMODULE values(SEQ_MODULEID.Nextval,\
							  '%s',%d,%d,%d)",
							  pPermissionModule->szModuleName, pPermissionModule->nCommonOperators,
							  pPermissionModule->nAdditionalOperators, pPermissionModule->nModuleType);
			int nPKID = 0;
			if ( !m_dbAccess.InsertAndReturnID(szBuffer, 
				"select SEQ_MODULEID.CURRVAL from dual", nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddPermissionModule_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddPermissionModule_Rsp, 
					&nPKID, sizeof(int), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyPermissionModule_Req:
		{
			if ( reqData.head.len != sizeof(PermissionModule))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			PermissionModule* pPermissionModule = (PermissionModule*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, 
				"update PERMISSIONMODULE t set t.modulename = '%s',t.commonoperators = %d,\
				t.additionaloperators = %d,t.moduletype = %d where t.moduleid = %d",
				pPermissionModule->szModuleName, pPermissionModule->nCommonOperators,
				pPermissionModule->nAdditionalOperators, pPermissionModule->nModuleType, 
				pPermissionModule->nModuleID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyPermissionModule_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyPermissionModule_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyPermissionModule_Rsp, 
						&pPermissionModule->nModuleID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelPermissionModule_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nModuleID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete PERMISSIONMODULE t where t.moduleid = %d", nModuleID);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelPermissionModule_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelPermissionModule_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelPermissionModule_Rsp, 
						&nModuleID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryPermissionModule_Req:
		{
			if ( reqData.head.len != 0)
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSIONMODULE t order by t.moduleid");
			vector<PermissionModule> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryPermissionModule_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryPermissionModule_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryPermissionModule_Rsp, 
						&vec[0], sizeof(PermissionModule)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessPermissionRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddPermission_Req:
		{
			if ( reqData.head.len != sizeof(Permission))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			Permission* pPermission = (Permission*)reqData.pData;
			int nPKID = 0;
			if ( !m_dbAccess.AddPermissionData(pPermission, nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddPermission_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddPermission_Rsp, 
					&nPKID, sizeof(nPKID), reqData.head.seq);
			}
		}
		break;
	case Cmd_MA_ModifyPermission_Req:
		{
			if ( reqData.head.len != sizeof(Permission))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			Permission* pPermission = (Permission*)reqData.pData;
			int nNum = 0;
			if ( !m_dbAccess.UpdatePermissionData(pPermission, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyPermission_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_ModifyPermission_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyPermission_Rsp, 
						&pPermission->nPermissionID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_DelPermission_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nKey = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete PERMISSION t where t.permissionid = %d", nKey);
			int nNum = 0;
			if ( !m_dbAccess.ExcuteUpdate(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelPermission_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( nNum == 0 )
				{
					string strError = "该记录不存在";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_DelPermission_Rsp, 
						(void*)strError.c_str(), strError.length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_DelPermission_Rsp, 
						&nKey, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryPermissionByOwner_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nAccountID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSION t where t.owner = %d order by t.permissionid", nAccountID);
			vector<Permission> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryPermissionByOwner_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryPermissionByOwner_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryPermissionByOwner_Rsp, 
						&vec[0], sizeof(Permission)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryPermissionByID_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nPermissionID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from PERMISSION t where t.permissionid = %d",
				nPermissionID);
			vector<Permission> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryPermissionByID_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					string strError = "权限信息不存在！";
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryPermissionByID_Rsp, 
						(void*)m_dbAccess.GetLastErrorString().c_str(), 
						m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryPermissionByID_Rsp, 
						&vec[0], sizeof(Permission), reqData.head.seq);
				}
			}
		}
		break;
	default:
		break;
	}
}

void Worker::ProcessMessageRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_AddMessage_Req:
		{
			if ( !(reqData.head.len > sizeof(MessageInfo) &&
				(reqData.head.len-sizeof(MessageInfo)) % sizeof(TargetAccount) == 0))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
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
			std::vector<TargetAccount> vForeAccount;
			if ( !m_dbAccess.AddMessage(msgInfo, vTargetAccountID, vForeAccount, nPKID))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_AddMessage_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_AddMessage_Rsp, 
					&nPKID, sizeof(nPKID), reqData.head.seq);

				msgInfo.nMessageID = nPKID;
				SendMessage(vForeAccount, msgInfo);
			}
		}
		break;
	case Cmd_MA_QryMessage_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t.messageid,t.title,t.content,\
				to_char(t.expireddate,'YYYY-MM-DD'),t.owner,t.type\
				from messageinfo t where t.owner = 1 and \
				t.expireddate >= trunc(sysdate) order by t.messageid",
				nOwnerID);
			vector<MessageInfo> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryMessage_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryMessage_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryMessage_Rsp, 
						&vec[0], sizeof(MessageInfo)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryTargetAccount_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nMessageID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t.accountid, t.account from MESSAGETARGET t where t.messageid = %d",
						nMessageID);
			vector<TargetAccount> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryTargetAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryTargetAccount_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryTargetAccount_Rsp, 
						&vec[0], sizeof(TargetAccount)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryWaitForSendAccount_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nMessageID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t.accountid, t.account from MESSAGEWAITFORSEND t where t.messageid = %d",
				nMessageID);
			vector<TargetAccount> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_QryWaitForSendAccount_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryWaitForSendAccount_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_QryWaitForSendAccount_Rsp, 
						&vec[0], sizeof(TargetAccount)*vec.size(), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_MA_QryUnReadMessage_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t2.messageid,t2.title,t2.content, \
					to_char(t2.expireddate,'YYYY-MM-DD'),t2.owner,t2.type \
					from MESSAGEWAITFORSEND t1 left join MESSAGEINFO t2 \
					on t1.messageid = t2.messageid where t1.accountid = %d \
					and t2.expireddate >= trunc(sysdate) order by t2.messageid",
					nOwnerID);
			vector<MessageInfo> vec;
			if ( !m_dbAccess.QueryData(szBuffer, vec) )
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_SendUnReadMessage_Rsp, 
					(void*)m_dbAccess.GetLastErrorString().c_str(), 
					m_dbAccess.GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				if ( vec.empty())
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_SendUnReadMessage_Rsp, 
						NULL, 0, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_SendUnReadMessage_Rsp, 
						&vec[0], sizeof(MessageInfo)*vec.size(), reqData.head.seq);
				}

				//发送成功后，删除已发送的数据
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer,  "delete from MESSAGEWAITFORSEND t where t.accountid =%d",
					nOwnerID);
				m_dbAccess.Excute(szBuffer);
			}

		}
		break;
	default:
		break;
	}
}

void Worker::FilterAccount(  int nCurAccountID, std::vector<AccountInfoEx>& vAccount, 
								  std::vector<AccountInfoEx>& vOut )
{
	vOut.clear();
	AccountInfoEx curAccountInfoEx;
	bool bFind = false;
	std::map<int, map<int, AccountInfoEx>> mapAccountTree;
	for ( UINT i = 0; i < vAccount.size(); i++ )
	{
		AccountInfoEx accountInfoEx = vAccount[i];
		if ( accountInfoEx.accountInfo.nAccountID == nCurAccountID )
		{
			curAccountInfoEx = accountInfoEx;
			vOut.push_back(curAccountInfoEx);
			bFind = true;
		}

		std::map<int, map<int, AccountInfoEx>>::iterator it = 
			mapAccountTree.find(accountInfoEx.accountInfo.nOwnerAccountID);
		if ( it != mapAccountTree.end() )
		{
			it->second.insert(make_pair(accountInfoEx.accountInfo.nAccountID, accountInfoEx));
		}
		else
		{
			std::map<int, AccountInfoEx> tempAccountMap;
			tempAccountMap.clear();
			tempAccountMap[accountInfoEx.accountInfo.nAccountID] = accountInfoEx;
			mapAccountTree[accountInfoEx.accountInfo.nOwnerAccountID] = tempAccountMap;
		}	
	}

	if ( bFind )
	{
		CalcShowData(mapAccountTree, curAccountInfoEx.accountInfo.nAccountID, 
			curAccountInfoEx.accountInfo.nViewPermission2, 1, vOut);
	}
}

void Worker::CalcShowData( std::map<int, std::map<int, AccountInfoEx>>& mapAccountTree,
						  int nOwnerID, int nRootViewPermission2, int nParentViewPermission1,
						  std::vector<AccountInfoEx>& vOutData )
{
	std::map<int, std::map<int, AccountInfoEx>>::iterator it_owner = 
		mapAccountTree.find(nOwnerID);
	if ( it_owner == mapAccountTree.end() )
	{
		return;
	}
	else
	{
		std::map<int, AccountInfoEx> tempAccountMap = it_owner->second;
		if ( tempAccountMap.empty())
		{
			return;
		}

		std::map<int, AccountInfoEx>::iterator it_account = tempAccountMap.begin();
		for ( ; it_account != tempAccountMap.end(); it_account++ )
		{
			if ( nRootViewPermission2 == 1 )
			{
				AccountInfoEx info = it_account->second;
				info.accountInfo.nLogonStatus = GetOnlineStatus(info.accountInfo.nAccountID);
				vOutData.push_back(info);

				CalcShowData(mapAccountTree, info.accountInfo.nAccountID, 
					nRootViewPermission2, info.accountInfo.nViewPermission1, vOutData);
			}
			else
			{
				if ( nParentViewPermission1 )
				{
					AccountInfoEx info = it_account->second;
					info.accountInfo.nLogonStatus = GetOnlineStatus(info.accountInfo.nAccountID);
					vOutData.push_back(info);

					CalcShowData(mapAccountTree, info.accountInfo.nAccountID, 
						nRootViewPermission2, info.accountInfo.nViewPermission1, vOutData);
				}
			}
		}
	}
}

int Worker::GetOnlineStatus( int nAccountID )
{
	if(g_pMASem) 
	{
		CTools_Win32::TakeWinSem(g_pMASem);
		g_nMASemCount++;
	}

	int nOnlineStatus = 0;
	std::map<int, std::set<SOCKET>>::iterator it_online = g_OnlineUser.find(nAccountID);
	if ( it_online != g_OnlineUser.end() )
	{
		if ( it_online->second.empty() )
		{
			nOnlineStatus = 0;
		}
		else
		{
			nOnlineStatus = 1;
		}
	}
	else
	{
		nOnlineStatus = 0;
	}

	if(g_pMASem) 
	{
		CTools_Win32::GiveWinSem(g_pMASem);
		g_nMASemCount--;
	}

	return nOnlineStatus;
}

bool Worker::IsLogon( SOCKET hSocket )
{
	if(g_pMASem) 
	{
		CTools_Win32::TakeWinSem(g_pMASem);
		g_nMASemCount++;
	}

	bool bLogon = false;
	std::set<SOCKET>::iterator it = g_OnlineUserSet.find(hSocket);
	if ( it != g_OnlineUserSet.end() )
	{
		bLogon = true;
	}

	if(g_pMASem) 
	{
		CTools_Win32::GiveWinSem(g_pMASem);
		g_nMASemCount--;
	}

	return bLogon;
}

void Worker::ForceCloseClient( int nAccountID )
{
	if(g_pMASem) 
	{
		CTools_Win32::TakeWinSem(g_pMASem);
		g_nMASemCount++;
	}

	std::set<SOCKET> tempSet;
	std::map<int, std::set<SOCKET>>::iterator it_online = g_OnlineUser.find(nAccountID);
	if ( it_online != g_OnlineUser.end() )
	{
		tempSet = it_online->second;
	}

	if(g_pMASem) 
	{
		CTools_Win32::GiveWinSem(g_pMASem);
		g_nMASemCount--;
	}

	std::set<SOCKET>::iterator it = tempSet.begin();
	for ( ; it != tempSet.end(); it++ )
	{
		RspResultData(*it, OPERATOR_SUCCESS, Cmd_MA_ForceCloseClient_Rsp,
			NULL, 0);
	}
}

void Worker::GetDelAccounts( int nCurAccountID, std::vector<AccountInfo>& vAccount, 
							std::set<int>& setOut )
{
	setOut.clear();
	std::map<int, map<int, AccountInfo>> mapAccountTree;
	for ( UINT i = 0; i < vAccount.size(); i++ )
	{
		AccountInfo accountInfo = vAccount[i];

		std::map<int, map<int, AccountInfo>>::iterator it = 
			mapAccountTree.find(accountInfo.nOwnerAccountID);
		if ( it != mapAccountTree.end() )
		{
			it->second.insert(make_pair(accountInfo.nAccountID, accountInfo));
		}
		else
		{
			std::map<int, AccountInfo> tempAccountMap;
			tempAccountMap.clear();
			tempAccountMap[accountInfo.nAccountID] = accountInfo;
			mapAccountTree[accountInfo.nOwnerAccountID] = tempAccountMap;
		}	
	}

	GetChildAccounts(nCurAccountID, mapAccountTree, setOut);
}

void Worker::GetChildAccounts( int nCurAccountID, 
							  std::map<int, map<int, AccountInfo>>& mapAccountTree, 
							  std::set<int>& setOut )
{
	std::map<int, std::map<int, AccountInfo>>::iterator it_owner = mapAccountTree.find(nCurAccountID);
	if ( it_owner != mapAccountTree.end() )
	{
		setOut.insert(nCurAccountID);
		std::map<int, AccountInfo>::iterator it_account = it_owner->second.begin();
		for ( ; it_account != it_owner->second.end(); it_account++ )
		{
			GetChildAccounts(it_account->first, mapAccountTree, setOut);
		}
	}
	else
	{
		//setOut.insert(nCurAccountID);
	}
}

bool Worker::DelAccountCaseCade( int nAccountID )
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from ACCOUNT t where t.accountid >= %d", nAccountID);
	vector<AccountInfo> vec;
	if ( !m_dbAccess.QueryData(szBuffer, vec))
	{
		return false;
	}
	else
	{
		//筛选数据
		std::set<int> setOut;
		GetDelAccounts(nAccountID, vec, setOut);
		if ( !m_dbAccess.DelAccountCaseCade(setOut))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool Worker::DelAccount( int nAccountID, bool& bCaseCadeDeleted )
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	//先检查在account表中是否有关联记录，有则删除单条记录
	sprintf(szBuffer, "select * from ACCOUNT t where t.owner = %d", nAccountID);
	int nNum = 0;
	if ( !m_dbAccess.ExcuteSelect(szBuffer, nNum))
	{
		return false;
	}
	else
	{
		if ( nNum == 0 )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "delete from ACCOUNT t where t.accountid = %d", nAccountID);
			if ( !m_dbAccess.Excute(szBuffer))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			//如果有关联子记录，则执行级联删除
			bCaseCadeDeleted = DelAccountCaseCade(nAccountID);
			return bCaseCadeDeleted;
		}
	}
}

void Worker::OnIdle()
{
	if ( m_bFirstIdle )
	{
		DisableExpiredAccounts();
		DeleteExpiredWaitForSendAccounts();
		m_bFirstIdle = false;
	}
	else
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		//判断是否跨天
		if ( st.wYear != m_backSysTime.wYear ||
			st.wMonth != m_backSysTime.wMonth ||
			st.wDay != m_backSysTime.wDay )
		{
			DisableExpiredAccounts();
			DeleteExpiredWaitForSendAccounts();
			m_backSysTime = st;
		}
	}
}

void Worker::DisableExpiredAccounts()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "update account t set t.accountstatus = 0 where (select trunc(sysdate) from dual) > t.enddate");
	m_dbAccess.Excute(szBuffer);
}

void Worker::SendMessage( std::vector<TargetAccount> &vForeAccount, MessageInfo msgInfo )
{
	//给在线用户发送消息
	if(g_pMASem) 
	{
		CTools_Win32::TakeWinSem(g_pMASem);
		g_nMASemCount++;
	}

	std::vector<TargetAccount>::iterator it = vForeAccount.begin();
	for ( ; it != vForeAccount.end(); )
	{
		std::map<int, std::set<SOCKET>>::iterator it_onlineUser = 
			g_OnlineUser.find((*it).nAccountID);
		if ( it_onlineUser == g_OnlineUser.end() )
		{
			it++;
			continue;
		}

		if ( it_onlineUser->second.empty())
		{
			it++;
			continue;
		}

		std::set<SOCKET>::iterator it_socket = it_onlineUser->second.begin();
		for ( ; it_socket != it_onlineUser->second.end(); it_socket++ )
		{
			RspResultData( *it_socket, OPERATOR_SUCCESS, Cmd_MA_SendUnReadMessage_Rsp, 
				&msgInfo, sizeof(msgInfo), 0);
		}

		it = vForeAccount.erase(it);
	}

	if(g_pMASem) 
	{
		CTools_Win32::GiveWinSem(g_pMASem);
		g_nMASemCount--;
	}

	//未发送的信息保存，待下次用户登录时发送
	m_dbAccess.SaveWaitForSendMessage(msgInfo.nMessageID, vForeAccount);
}

void Worker::DeleteExpiredWaitForSendAccounts()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "delete from MESSAGEWAITFORSEND t1 where t1.messageid in \
		(select t2.messageid from MESSAGEINFO t2 where (select trunc(sysdate) from dual) > t2.expireddate)");
	m_dbAccess.Excute(szBuffer);
}
