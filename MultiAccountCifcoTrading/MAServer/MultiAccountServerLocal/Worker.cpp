#include "stdafx.h"
#include <queue>
#include <iostream>
#include "WriteLog.h"
#include "Worker.h"
#include "Tools_Win32.h"
#include "TcpServerWorker.h"
#include "MultiAccountServer.h"
#include "MultiAccountCmd.h"
#include "MultiAccountStruct.h"
#include "ConfigManager.h"

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
				//ProcessIDCardRequest(reqData);
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
				//ProcessPermissionModuleRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_Permission_Min && 
				reqData.head.cmdid <= Cmd_MA_Permission_Max)
			{
				//ProcessPermissionRequest(reqData);
			}
			else if ( reqData.head.cmdid >= Cmd_MA_Message_Min && 
				reqData.head.cmdid <= Cmd_MA_Message_Max)
			{
				//ProcessMessageRequest(reqData);
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
			if ( !ConfigManager::GetInstance()->Logon(*pLogon))
			{
				std::string strError = "用户名或密码错误";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_Login_Rsp, 
					(void*)strError.c_str(), 
					strError.length()+1, reqData.head.seq);
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
			ConfigManager::GetInstance()->ModifyPwd(pUserInfo->szPassword);

			RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_MA_ModifyPwd_Rsp, 
				NULL, 0, reqData.head.seq);
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
	case Cmd_MA_QryServerGroup_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nBrokerID = *(int*)reqData.pData;
			vector<ServerGroup> vec = ConfigManager::GetInstance()->GetServerGroup(nBrokerID);

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
			vector<ServerAddr> vec = ConfigManager::GetInstance()->GetServerAddr(nServerNo);
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
		break;
	default:
		break;
	}
}

void Worker::ProcessBrokerRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_QryBroker_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerAccountID = *(int*)reqData.pData;
			vector<BrokerInfo> vec = ConfigManager::GetInstance()->GetBrokerInfo();
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
		break;
	case Cmd_MA_QryUsableBroker_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerAccountID = *(int*)reqData.pData;
			vector<BrokerInfo> vec = ConfigManager::GetInstance()->GetBrokerInfo();
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
		break;
	default:
		break;
	}
}

void Worker::ProcessTradeAccountRequest( RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_MA_QryTradeAccount_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData.head.moduleid, reqData.head.cmdid,
					reqData.head.len);
				return;
			}

			int nOwnerID = *(int*)reqData.pData;
			vector<TradeAccount> vec = ConfigManager::GetInstance()->GetTradeAccount();
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

			ConfigManager::GetInstance()->SetTradeAccount(vTradeAccount);
			if ( !ConfigManager::GetInstance()->SaveConfig())
			{
				std::string strError = "保存配置文件出错";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_MA_SaveTradeAccount_Rsp, 
					(void*)strError.c_str(), 
					strError.length()+1, reqData.head.seq);
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