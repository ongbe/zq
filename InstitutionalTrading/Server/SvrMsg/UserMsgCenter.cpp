
#include "stdafx.h"
#include "UserMsgCenter.h"
#include "CommonDef.h"
#include "..\SvrTcp\Interface_SvrTcp.h"
#include "..\SvrLogin\Interface_SvrLogin.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"

DWORD CUserMsgCenter::Proc_Send(LPVOID lpParameter)
{
	if(lpParameter == NULL)
		return 1;

	int nSendCount = 0;
	int nStartTargetMsgID = 0;
	CUserMsgCenter* pMsgCenter = (CUserMsgCenter*)lpParameter;
	//UserMsgMemStru userMsg;

	while(!pMsgCenter->IsExit()) {
		//pMsgCenter->LockObject();
		pMsgCenter->OnMsgSend();
		//memset(&userMsg, 0, sizeof(userMsg));
		//nSendCount = 0;
		//if(pMsgCenter->PopMsgForSend(userMsg)) {
		//	nStartTargetMsgID = userMsg.msg.TargetMsgID;
		//	do {
		//		pMsgCenter->SendMsg(userMsg);
		//		if((nStartTargetMsgID == userMsg.msg.TargetMsgID && nSendCount>0) || nSendCount>=conMaxSendNum)
		//			break;
		//		memset(&userMsg, 0, sizeof(userMsg));
		//		nSendCount++;
		//	}
		//	while(pMsgCenter->PopMsgForSend(userMsg));
		//}
		//pMsgCenter->UnlockObject();
		Sleep(1000);
	}
	return 0;
}

int CUserMsgCenter::GetLocalDate()
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	return sysTime.wYear*10000+sysTime.wMonth*100+sysTime.wDay;
}


CUserMsgCenter::CUserMsgCenter(void)
{
	m_bIsExit = false;
	m_bMaster = false;
	m_nLastMsgID = 0;
	m_nLastTargetMsgID = 0;
	m_hSendThread = NULL;
	m_dwSendThreadID = 0;
}

CUserMsgCenter::~CUserMsgCenter(void)
{
	ClearData();
}

bool CUserMsgCenter::Init()
{
	bool bRet = false;

	if(m_bMaster) {

		if(DB_GetLastMsgID(m_nLastMsgID)==CF_ERROR_SUCCESS && 
				DB_GetLastTargetMsgID(m_nLastTargetMsgID)==CF_ERROR_SUCCESS) {

			vector<UserMsgMemStru> vecRecvMsg;
			
			if(DB_LoadNoRecvMsgs(vecRecvMsg)==CF_ERROR_SUCCESS) {
				
				for(int i = 0; i < vecRecvMsg.size(); i++) {
					AppendTargetMsg(vecRecvMsg[i].msg);
				}

				//map<int, CUserRecvMsgCore*>::iterator it;
				//
				//LockObject();

				//for(int i = 0; i < vecRecvMsg.size(); i++) {
				//	it = m_mapUserRecvMsg.find(vecRecvMsg[i].msg.TargetUserID);
				//	if(it == m_mapUserRecvMsg.end()) {
				//		CUserRecvMsgCore* pNew = new CUserRecvMsgCore();
				//		pNew->SetMaster(m_bMaster);
				//		pNew->SetTargetUserID(vecRecvMsg[i].msg.TargetUserID);
				//		pNew->Reinit();
				//		pNew->AppendTargetMsg(
				//				vecRecvMsg[i].msg.TargetMsgID, vecRecvMsg[i]);
				//		pair<map<int, CUserRecvMsgCore*>::iterator, bool> ret = 
				//				m_mapUserRecvMsg.insert(make_pair(
				//						vecRecvMsg[i].msg.TargetUserID, pNew));
				//		if(ret.second)
				//			it = ret.first;
				//	}
				//	//if(it != m_mapUserRecvMsg.end()) {
				//	//	PushMsgForSend(vecRecvMsg[i]);
				//	//}
				//}

				//UnlockObject();

				bRet = true;
			}
		}
	}
	else {
		ReqSynchrQuery();
		//// 从master从请求所有未发送target的消息。
		//CInterface_SvrTcp::getObj().SendPkgData(_GetMasterSocket(),
		//		Cmd_UserMsg_QueryTargetMsgFromSlave_Req, NULL, 0, 0, 0,
		//		0, 0, 0, 0);
	}

	m_hSendThread = ::CreateThread(NULL, 0, Proc_Send, this, 0, &m_dwSendThreadID);

	return bRet;
}

bool CUserMsgCenter::ReqSynchrQuery()
{
	// 从master从请求所有未发送target的消息。
	CInterface_SvrTcp::getObj().SendPkgData(_GetMasterSocket(),
			Cmd_UserMsg_QueryTargetMsgFromSlave_Req, NULL, 0, 0, 0,
			0, 0, 0, 0);
	return true;
}

void CUserMsgCenter::Release()
{
	m_bIsExit = true;
	if(::WaitForSingleObject(m_hSendThread, 8000)==WAIT_TIMEOUT) {
		::TerminateThread(m_hSendThread, 0);
		::CloseHandle(m_hSendThread);
	}
}

bool CUserMsgCenter::SubsUser(int nUserID)
{
	map<int, int>::iterator it;
	LockObject();
	if((it = m_mapSubsUser.find(nUserID)) != m_mapSubsUser.end())
		it->second++;
	else
		m_mapSubsUser.insert(make_pair(nUserID, 1));
	UnlockObject();
	return true;
}

bool CUserMsgCenter::UnsubsUser(int nUserID)
{
	map<int, int>::iterator it;
	LockObject();
	if((it = m_mapSubsUser.find(nUserID)) != m_mapSubsUser.end()) {
		it->second--;
		if(it->second<=0)
			m_mapSubsUser.erase(nUserID);
	}
	UnlockObject();
	return true;
}

bool CUserMsgCenter::AppendOrigMsg(int nOwnerID, int nTargetUserID, int nMsgType, 
								   int nExpiredDate, const string& strTitle, const string& strMsg)
{
	set<int> setUser;

	setUser.insert(nTargetUserID);
	return AppendOrigMsg(nOwnerID, setUser, nMsgType, nExpiredDate, strTitle, strMsg);
}

bool CUserMsgCenter::AppendOrigMsg(int nOwnerID, const set<int>& setTargetUser, int nMsgType, 
								   int nExpiredDate, const string& strTitle, const string& strMsg)
{
	if(m_bMaster) {
		UserMsgMemStru userMsg;
		SYSTEMTIME sysTime;

		memset(&userMsg, 0, sizeof(userMsg));
		::GetLocalTime(&sysTime);
		userMsg.IsOrigMsg = true;
		userMsg.msg.MsgID = GetNextMsgID();
		userMsg.msg.OwnerUserID = nOwnerID;
		userMsg.msg.MsgType = nMsgType;
		sprintf_s(userMsg.msg.CreateDateTime, sizeof(userMsg.msg.CreateDateTime), 
				"%04d-%02d-%02d %02d:%02d:%02d", 
				sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		userMsg.msg.ExpiredDate = nExpiredDate;//sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;
		strncpy(userMsg.msg.Title, strTitle.c_str(), sizeof(userMsg.msg.Title)-1);
		strncpy(userMsg.msg.Msg, strMsg.c_str(), sizeof(userMsg.msg.Msg)-1);

		DB_SaveOrigMsg(userMsg);

		//map<int, CUserRecvMsgCore*>::iterator it;
		vector<UserMsgMemStru> vecRecvMsg;

		if(MakeRecvMsgsFromSendMsg(userMsg, setTargetUser, vecRecvMsg)>0) {

			for(int i = 0; i < vecRecvMsg.size(); i++) {
				AppendTargetMsg(vecRecvMsg[i].msg);
				//it = m_mapUserRecvMsg.find(vecRecvMsg[i].msg.TargetUserID);
				//if(it == m_mapUserRecvMsg.end()) {
				//	CUserRecvMsgCore* pNew = new CUserRecvMsgCore();
				//	pNew->SetMaster(m_bMaster);
				//	pNew->SetTargetUserID(vecRecvMsg[i].msg.TargetUserID);
				//	pNew->Reinit();
				//	pNew->AppendTargetMsg(
				//			vecRecvMsg[i].msg.TargetMsgID, vecRecvMsg[i]);
				//	pair<map<int, CUserRecvMsgCore*>::iterator, bool> ret = 
				//			m_mapUserRecvMsg.insert(make_pair(
				//					vecRecvMsg[i].msg.TargetUserID, pNew));
				//	if(ret.second)
				//		it = ret.first;
				//}
				//else {
				//	if(it->second != NULL)
				//		it->second->AppendTargetMsg(userMsg.TargetMsgID, vecRecvMsg[i]);
				//}
				//if(it != m_mapUserRecvMsg.end()) {
					DB_SaveTargetMsg(vecRecvMsg[i]);
					SynchrTargetMsgToSlave(vecRecvMsg[i]);
					//PushMsgForSend(vecRecvMsg[i]);
				//}
			}
		}
	}
	else {
		if(setTargetUser.size()==0)
			return false;

		int nLen = sizeof(UserSendMsgStru)+(setTargetUser.size()-1)*sizeof(int);
		char* pData = new char[nLen];
		UserSendMsgStru& tMsg = *(UserSendMsgStru*)pData;
		int i = 0;

		memset(pData, 0, nLen);
		tMsg.OwnerUserID = nOwnerID;
		tMsg.ExpiredDate = nExpiredDate;
		tMsg.TargetUserCount = setTargetUser.size();
		strncpy(tMsg.Title, strTitle.c_str(), sizeof(tMsg.Title)-1);
		strncpy(tMsg.Msg, strMsg.c_str(), sizeof(tMsg.Msg)-1);
		for(set<int>::const_iterator it = setTargetUser.begin(); it != setTargetUser.end(); it++) {
			tMsg.TargetUsers[i] = *it;
			i++;
		}
		if(_GetMasterSocket()!=0)
			CInterface_SvrTcp::getObj().SendPkgData(_GetMasterSocket(),
					Cmd_UserMsg_TurnSendReqToMaster_Req, (void*)pData, nLen, 0, 0, nOwnerID, 
					0, 0, nOwnerID);
		delete pData;
	}
	return true;
}

bool CUserMsgCenter::AppendTargetMsg(UserMsgStru& userMsg)
{
	map<int, CUserRecvMsgCore*>::iterator it;
	UserMsgMemStru memMsg;
	SYSTEMTIME sysTime;

	memset(&memMsg, 0, sizeof(memMsg));
	memMsg.IsOrigMsg = false;
	memMsg.msg = userMsg;

	LockObject();

	it = m_mapUserRecvMsg.find(userMsg.TargetUserID);
	if(it == m_mapUserRecvMsg.end()) {
		CUserRecvMsgCore* pNew = new CUserRecvMsgCore();
		pNew->SetMaster(m_bMaster);
		pNew->SetTargetUserID(userMsg.TargetUserID);
		pNew->Reinit();
		pNew->AppendTargetMsg(userMsg.TargetMsgID, memMsg);
		m_mapUserRecvMsg.insert(make_pair(userMsg.TargetUserID, pNew));
	}
	else {
		if(it->second != NULL)
			it->second->AppendTargetMsg(userMsg.TargetMsgID, memMsg);
	}

	UnlockObject();

	return true;
}

bool CUserMsgCenter::SynchrTargetMsgToSlave(UserMsgMemStru& sendMsg)
{
	std::vector<SOCKET> lvecSocket;
	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
	for(int i = 0; i < lvecSocket.size(); i++) {
		CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i], 
				Cmd_UserMsg_SynchrPushMsg, (void*)&sendMsg.msg, sizeof(sendMsg.msg), 
				0, 0, 0, 0, 0, 0);
	}
	return true;
}

//// 内部不加锁
//bool CUserMsgCenter::PushMsgForSend(UserMsgMemStru& userMsg)
//{
//	m_queueMsgWait.push(userMsg);
//	return true;
//}

//// 内部未加锁
//bool CUserMsgCenter::PopMsgForSend(UserMsgMemStru& userMsg)
//{
//	bool bRet = false;
//	//LockObject();
//	if(!m_queueMsgWait.empty()) {
//		userMsg = m_queueMsgWait.front();
//		m_queueMsgWait.pop();
//		bRet = true;
//	}
//	//UnlockObject();
//	return bRet;
//}

void CUserMsgCenter::OnMsgSend()
{
	// 已在CUserRecvMsgCore中处理。
	//// 需要处理targetmsg是否过期
	////if(GetLocalDate() > userMsg.msg.ExpiredDate)
	////	return false;

	map<int, CUserRecvMsgCore*>::iterator it;

	LockObject();

	for(it = m_mapUserRecvMsg.begin(); it != m_mapUserRecvMsg.end(); it++) {
		if(HasSubsMsg(it->first) && it->second) {
			it->second->SendTargetMsg(0, false);
		}
	}

	UnlockObject();
}

bool CUserMsgCenter::SendTargetMsgToSlave(int hSlaveSocket)
{
	map<int, CUserRecvMsgCore*>::iterator it;

	LockObject();

	for(it = m_mapUserRecvMsg.begin(); it != m_mapUserRecvMsg.end(); it++) {
		if((HasSubsMsg(it->first) || hSlaveSocket>0) && it->second != NULL) {
			it->second->SendTargetMsg(hSlaveSocket, true);
		}
	}

	UnlockObject();

	return true;
}

bool CUserMsgCenter::UpdateTargetMsgStatus(UserMsgStatusStru& msgStatus, bool bSynchrToSlave, int nSrcSocket)
{
	map<int, CUserRecvMsgCore*>::iterator it;

	LockObject();

	it = m_mapUserRecvMsg.find(msgStatus.TargetUserID);
	if(it != m_mapUserRecvMsg.end()) {
		if(it->second != NULL) {
			it->second->UpdateTargetMsgToSent(msgStatus.TargetMsgID, bSynchrToSlave, nSrcSocket);
		}
	}

	UnlockObject();

	return true;
}

void CUserMsgCenter::ClearData()
{
	map<int, CUserRecvMsgCore*>::iterator it;

	LockObject();
	
	for(it = m_mapUserRecvMsg.begin(); it != m_mapUserRecvMsg.end(); it++) {
		delete it->second;
		it->second = NULL;
	}
	m_mapUserRecvMsg.clear();
	
	UnlockObject();
}

//// 内部未加锁
//bool CUserMsgCenter::SendMsg(UserMsgMemStru& userMsg)
//{
//	if(GetLocalDate() > userMsg.msg.ExpiredDate)
//		return false;
//
//	vector<SOCKET> vecSocket;
//	int nRetCount = 0;
//
//	if(HasSubsMsg(userMsg.msg.TargetUserID)) {
//		if(GetUserSocket(userMsg.msg.TargetUserID, vecSocket) && vecSocket.size()>0) {
//			for(int i=0; i<vecSocket.size(); i++) {
//				if(CInterface_SvrTcp::getObj().SendPkgData(vecSocket[i],
//						Cmd_UserMsg_PushMsg, &userMsg.msg, sizeof(userMsg.msg), 0, 0,
//						CF_ERROR_SUCCESS, userMsg.msg.TargetUserID, 0, 0))
//					nRetCount++;
//			}
//			if(nRetCount>0) {
//				map<int, CUserRecvMsgCore*>::iterator it = m_mapUserRecvMsg.find(userMsg.msg.TargetUserID);
//				if(it != m_mapUserRecvMsg.end()) {
//					it->second->ResetTargetMsgStatus(userMsg.msg.TargetUserID, true);
//				}
//				if(m_bNeedSave)
//					DB_UpdateMsgRecvStatus(userMsg.msg.TargetMsgID, true);
//			}
//			else {
//				PushMsgForSend(userMsg);
//			}
//		}
//		else {
//			m_mapSubsUser.erase(userMsg.msg.TargetUserID);
//			PushMsgForSend(userMsg);
//		}
//	}
//	else {
//		PushMsgForSend(userMsg);
//	}
//	return nRetCount>0;
//}

//bool CUserMsgCenter::CreateUserMsgCore(int nUserID)
//{
//	return false;
//}

//bool CUserMsgCenter::GetUserSocket(int nUserID, vector<SOCKET>& vecSocket)
//{
//	return CInterface_SvrLogin::getObj().GetUserSockets(nUserID, vecSocket) == 0;
//}

int CUserMsgCenter::MakeRecvMsgsFromSendMsg(UserMsgMemStru& sendMsg, const set<int>& setTargetUser, 
											vector<UserMsgMemStru>& vecRecvMsg)
{
	set<int>::const_iterator it;
	UserMsgMemStru tempMsg;

	vecRecvMsg.clear();
	for(it = setTargetUser.begin(); it != setTargetUser.end(); it++) {
		memset(&tempMsg, 0, sizeof(tempMsg));
		tempMsg.msg = sendMsg.msg;
		tempMsg.msg.TargetMsgID = GetNextTargetMsgID();
		tempMsg.msg.TargetUserID = *it;
		vecRecvMsg.push_back(tempMsg);
	}
	return vecRecvMsg.size();
}

int CUserMsgCenter::DB_LoadNoRecvMsgs(vector<UserMsgMemStru>& vecRetMsg)
{
	return DB_LoadNoRecvMsgs_UserID(0, vecRetMsg);
}

int CUserMsgCenter::DB_LoadNoRecvMsgs_UserID(int nUserID, vector<UserMsgMemStru>& vecRetMsg)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	SYSTEMTIME sysTime;
	char szBuffer[MAX_SQL_LENGTH];

	memset(szBuffer, 0, sizeof(szBuffer));
	::GetLocalTime(&sysTime);
	//select * from (select tta.msgid, tta.targetmsgid, tor.ownerid, tta.userid as targetuserid, tor.expireddate, tor.createdatetime, tta.hassend, tta.senddatetime, tor.title, tor.message from USERMSG_TARGET tta, USERMSG_ORIG tor where tta.msgid = tor.msgid) t
	//where expireddate>=20130820 and targetuserid = 1
	if(nUserID==0)
		sprintf(szBuffer, 
				"select * from (select tta.msgid, tta.targetmsgid, tor.ownerid, tta.userid as targetuserid, tor.expireddate, tor.createdatetime, tta.hassend, tta.senddatetime, tor.title, tor.message from USERMSG_TARGET tta, USERMSG_ORIG tor where tta.msgid = tor.msgid) t "
				"where t.HASSEND = 0 AND "
				"t.EXPIREDDATE >= %04d%02d%02d", 
				sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	else
		sprintf(szBuffer, 
				"select * from (select tta.msgid, tta.targetmsgid, tor.ownerid, tta.userid as targetuserid, tor.expireddate, tor.createdatetime, tta.hassend, tta.senddatetime, tor.title, tor.message from USERMSG_TARGET tta, USERMSG_ORIG tor where tta.msgid = tor.msgid) t "
				"where t.HASSEND = 0 AND "
				"t.targetuserid = %d AND t.EXPIREDDATE >= %04d%02d%02d", 
				nUserID, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	
	std::vector<std::vector<_variant_t>> vNode;
	if(!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode)/* || vNode.size()==0*/) {
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else {
		UserMsgMemStru tempMsg;
		for(UINT j = 0; j < vNode.size(); j++) {
			memset(&tempMsg, 0, sizeof(tempMsg));

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 10)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				int i = 0;
				tempMsg.msg.MsgID = vValue[i++];
				tempMsg.msg.TargetMsgID = vValue[i++];
				tempMsg.msg.OwnerUserID = vValue[i++];
				tempMsg.msg.TargetUserID = vValue[i++];
				tempMsg.msg.ExpiredDate = vValue[i++];
				strncpy(tempMsg.msg.CreateDateTime, (_bstr_t)(vValue[i++]), sizeof(tempMsg.msg.CreateDateTime)-1);
				tempMsg.HasSend = (int)(vValue[i++])==0;
				tempMsg.HasSend = false;
				strncpy(tempMsg.msg.SendDateTime, (_bstr_t)(vValue[i++]), sizeof(tempMsg.msg.SendDateTime)-1);
				strncpy(tempMsg.msg.Title, (_bstr_t)(vValue[i++]), sizeof(tempMsg.msg.Title)-1);
				strncpy(tempMsg.msg.Msg, (_bstr_t)(vValue[i++]), sizeof(tempMsg.msg.Msg)-1);

				vecRetMsg.push_back(tempMsg);
			}
		}
	}

	return CF_ERROR_SUCCESS;
}

//int CUserMsgCenter::DB_UpdateMsgRecvStatus(int nTargetMsgID, bool bHasSent)
//{
//	int nErrorCode = CF_ERROR_SUCCESS;
//
//	char szBuffer[MAX_SQL_LENGTH];
//	char szDateTime[24];
//	SYSTEMTIME sysTime;
//
//	memset(szDateTime, 0, sizeof(szDateTime));
//	memset(szBuffer, 0, sizeof(szBuffer));
//	::GetLocalTime(&sysTime);
//	sprintf_s(szDateTime, sizeof(szDateTime)-1, "%04d-%02d-%02d %02d:%02d:%02d", 
//		sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
//		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
//	sprintf(szBuffer, 
//			"Update USERMSG_TARGET Set hassend = %d, senddatetime = '%s' WHERE targetmsgid = %d and senddatetime is null ", 
//			(bHasSent ? 1 : 0), (bHasSent ? szDateTime : ""), 
//			nTargetMsgID);
//	
//	int nRetCount = 0;
//	CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nRetCount, nErrorCode);
//	return nErrorCode;
//}

int CUserMsgCenter::DB_SaveOrigMsg(UserMsgMemStru& userMsg)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, 
			"Insert into USERMSG_ORIG values(%d, %d, %d, %d, '%s', '%s', '%s')", 
			userMsg.msg.MsgID, userMsg.msg.MsgType, userMsg.msg.OwnerUserID, userMsg.msg.ExpiredDate,
			userMsg.msg.CreateDateTime, userMsg.msg.Title, userMsg.msg.Msg);
	
	CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
	return nErrorCode;
}

int CUserMsgCenter::DB_SaveTargetMsg(UserMsgMemStru& userMsg)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, 
			"Insert into USERMSG_TARGET(TARGETMSGID, MSGID, USERID) values(%d, %d, %d)", 
			userMsg.msg.TargetMsgID, userMsg.msg.MsgID, userMsg.msg.TargetUserID);
	
	CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
	return nErrorCode;
}

int CUserMsgCenter::DB_GetLastMsgID(int& nRetMsgID)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select max(msgid) from USERMSG_ORIG");
	
	std::vector<std::vector<_variant_t>> vNode;
	if(!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode)) {
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else {
		for(UINT j = 0; j < vNode.size() && j < 1; j++) {

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 10)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				nRetMsgID = vValue[0];
			}
		}
	}

	return CF_ERROR_SUCCESS;
}

int CUserMsgCenter::DB_GetLastTargetMsgID(int& nRetMsgID)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select max(targetmsgid) from USERMSG_TARGET");
	
	std::vector<std::vector<_variant_t>> vNode;
	if(!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode)) {
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else {
		for(UINT j = 0; j < vNode.size() && j < 1; j++) {

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 10)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				nRetMsgID = vValue[0];
			}
		}
	}

	return CF_ERROR_SUCCESS;
}
