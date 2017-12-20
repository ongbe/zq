
#include "stdafx.h"
#include "UserRecvMsgCore.h"
#include "CommonStruct.h"
#include "CommonDef.h"

CUserRecvMsgCore::CUserRecvMsgCore(void)
{
	m_bMaster = false;
	m_nTargetUserID = 0;
}

CUserRecvMsgCore::~CUserRecvMsgCore(void)
{
}

int CUserRecvMsgCore::GetLocalDate()
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	return sysTime.wYear*10000+sysTime.wMonth*100+sysTime.wDay;
}

bool CUserRecvMsgCore::Reinit()
{
	m_mapTargetMsg.clear();
	return true;
}

bool CUserRecvMsgCore::AppendTargetMsg(int nTargetMsgID, UserMsgMemStru& userMsg)
{
	map<int, UserMsgMemStru>::iterator it;
	pair<map<int, UserMsgMemStru>::iterator, bool> ret = 
		m_mapTargetMsg.insert(make_pair(nTargetMsgID, userMsg));
	return ret.second;
}

//bool CUserRecvMsgCore::AddNoSendTargetMsg(int nTargetMsgID, UserMsgMemStru& userMsg)
//{
//	if(userMsg.HasSend)
//		return false;
//	map<int, UserMsgMemStru>::iterator it;
//	pair<map<int, UserMsgMemStru>::iterator, bool> ret = 
//		m_mapNoSendMsg.insert(make_pair(nTargetMsgID, userMsg));
//	return ret.second;
//}

bool CUserRecvMsgCore::SendTargetMsg(int nSocket, bool bForSlaveSynchr)
{
	map<int, UserMsgMemStru>::iterator it;
	int nCount = 0;
	vector<SOCKET> vecSocket;

	if(!bForSlaveSynchr) {
		GetUserSocket(m_nTargetUserID, vecSocket);
	}

	it = m_mapTargetMsg.begin();
	while(it != m_mapTargetMsg.end()) {
		if(it->second.HasSend) {
			it++;
			continue;
		}

		// 需要处理targetmsg是否过期
		if(GetLocalDate() > it->second.msg.ExpiredDate) {
			it = m_mapTargetMsg.erase(it);
			continue;
		}

		if(bForSlaveSynchr) {
			// 发送给请求同步数据的Slave服务器
			CInterface_SvrTcp::getObj().SendPkgData(nSocket, 
					Cmd_UserMsg_SynchrPushMsg, 
					(void*)&it->second.msg, sizeof(it->second.msg), 
					0, 0, 0, 0, 0, 0);
		}
		else {
			nCount=0;
			// 正常发送给客户端
			for(int i=0; i<vecSocket.size(); i++) {
				if(CInterface_SvrTcp::getObj().SendPkgData(vecSocket[i],
						Cmd_UserMsg_PushMsg, &it->second.msg, sizeof(it->second.msg), 0, 0,
						CF_ERROR_SUCCESS, it->second.msg.TargetUserID, 0, 0))
					nCount++;
			}
			if(nCount>0) 
				UpdateTargetMsgToSent(it->second.msg.TargetMsgID, true, 0);
		}
		it++;
	}

	return true;
}
bool CUserRecvMsgCore::UpdateTargetMsgToSent(int nTargetMsgID, bool bSynchr, int nSrcSocket)
{
	bool bRet = false;
	map<int, UserMsgMemStru>::iterator it = m_mapTargetMsg.find(nTargetMsgID);

	if(it != m_mapTargetMsg.end()) {
		it->second.HasSend = true;
		if(bSynchr) {
			if(m_bMaster) {
				DB_UpdateMsgRecvStatus(nTargetMsgID, true);
			}
			UserMsgStatusStru msgStatus = {0};
			msgStatus.TargetUserID = m_nTargetUserID;
			msgStatus.TargetMsgID = nTargetMsgID;
			SynchroMsgSentStatus(msgStatus, nSrcSocket);
		}
		bRet = true;
	}
	return bRet;
}

bool CUserRecvMsgCore::GetUserSocket(int nUserID, vector<SOCKET>& vecSocket)
{
	return CInterface_SvrLogin::getObj().GetUserSockets(nUserID, vecSocket) == 0;
}

bool CUserRecvMsgCore::SynchroMsgSentStatus(UserMsgStatusStru& msgStatus, int nSrcSocket)
{
	if(m_bMaster) {
		std::vector<SOCKET> lvecSocket;
		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
		for(int i = 0; i < lvecSocket.size(); i++) {
			if(nSrcSocket==lvecSocket[i])
				continue;
			CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i], 
					Cmd_UserMsg_SynchroSentStatusToSlave_Req, (void*)&msgStatus, sizeof(msgStatus), 
					0, 0, 0, 0, 0, 0);
		}
	}
	else {
		if(_GetMasterSocket()!=0)
			CInterface_SvrTcp::getObj().SendPkgData(_GetMasterSocket(),
					Cmd_UserMsg_SynchroSentStatusToMaster_Req, (void*)&msgStatus, sizeof(msgStatus), 
					0, 0, 0, 0, 0, 0);
	}
	return true;
}

int CUserRecvMsgCore::DB_UpdateMsgRecvStatus(int nTargetMsgID, bool bHasSent)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	char szDateTime[24];
	SYSTEMTIME sysTime;

	memset(szDateTime, 0, sizeof(szDateTime));
	memset(szBuffer, 0, sizeof(szBuffer));
	::GetLocalTime(&sysTime);
	sprintf_s(szDateTime, sizeof(szDateTime)-1, "%04d-%02d-%02d %02d:%02d:%02d", 
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	sprintf(szBuffer, 
			"Update USERMSG_TARGET Set hassend = %d, senddatetime = '%s' WHERE targetmsgid = %d and senddatetime is null ", 
			(bHasSent ? 1 : 0), (bHasSent ? szDateTime : ""), 
			nTargetMsgID);
	
	int nRetCount = 0;
	CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nRetCount, nErrorCode);
	return nErrorCode;
}
