#pragma once

#include <windows.h>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include "easymutex.h"
#include "CommonStruct.h"
#include "UserRecvMsgCore.h"

using namespace std;

class CUserMsgCenter
{
public:
	enum {	conMaxSendNum = 500 };

public:
	static DWORD WINAPI Proc_Send(LPVOID lpParameter);
	static int GetLocalDate();

	static int DB_LoadNoRecvMsgs(vector<UserMsgMemStru>& vecRetMsg);
	static int DB_LoadNoRecvMsgs_UserID(int nUserID, vector<UserMsgMemStru>& vecRetMsg);
	//static int DB_UpdateMsgRecvStatus(int nTargetMsgID, bool bHasSent);
	static int DB_SaveOrigMsg(UserMsgMemStru& userMsg);
	static int DB_SaveTargetMsg(UserMsgMemStru& userMsg);
	static int DB_GetLastMsgID(int& nRetMsgID);
	static int DB_GetLastTargetMsgID(int& nRetMsgID);

public:
	CUserMsgCenter(void);
	virtual ~CUserMsgCenter(void);

	void LockObject() {
		m_mutex.lock();
	};
	void UnlockObject() {
		m_mutex.unlock();
	};

	bool IsExit() {
		return m_bIsExit;
	};

	void SetMaster(bool bHas) {
		m_bMaster = bHas;
	};
	bool GetMaster() {
		return m_bMaster;
	};

	bool Init();
	void Release();

	bool SubsUser(int nUserID);
	bool UnsubsUser(int nUserID);

	bool AppendOrigMsg(int nOwnerID, int nTargetUserID, int nMsgType, 
			int nExpiredDate, const string& strTitle, const string& strMsg);
	bool AppendOrigMsg(int nOwnerID, const set<int>& setTargetUser, int nMsgType, 
			int nExpiredDate, const string& strTitle, const string& strMsg);
	bool AppendTargetMsg(UserMsgStru& userMsg);

	//bool PushMsgForSend(UserMsgMemStru& userMsg);
	//bool PopMsgForSend(UserMsgMemStru& userMsg);

	//bool SendMsg(UserMsgMemStru& userMsg);
	void OnMsgSend();

	bool ReqSynchrQuery();
	bool SendTargetMsgToSlave(int hSlaveSocket);
	bool UpdateTargetMsgStatus(UserMsgStatusStru& msgStatus, bool bSynchrToSlave, int nSrcSocket);
	void ClearData();

protected:
	//bool CreateUserMsgCore(int nUserID);
	//bool GetUserSocket(int nUserID, vector<SOCKET>& vecSocket);
	int MakeRecvMsgsFromSendMsg(UserMsgMemStru& sendMsg, 
			const set<int>& setTargetUser, vector<UserMsgMemStru>& vecRecvMsg);
	bool SynchrTargetMsgToSlave(UserMsgMemStru& sendMsg);

	int GetNextMsgID() {
		m_nLastMsgID++;
		return m_nLastMsgID;
	};
	int GetNextTargetMsgID() {
		m_nLastTargetMsgID++;
		return m_nLastTargetMsgID;
	};
	bool HasSubsMsg(int nUserID) {
		return m_mapSubsUser.find(nUserID)!=m_mapSubsUser.end();
	};


protected:
	bool						m_bIsExit;
	bool						m_bMaster;
	int							m_nLastMsgID;
	int							m_nLastTargetMsgID;
	Ceasymutex					m_mutex;
	map<int, CUserRecvMsgCore*>	m_mapUserRecvMsg;
	map<int, int>				m_mapSubsUser;		// 记录订阅消息的用户及订阅次数
	//queue<UserMsgMemStru>		m_queueMsgWait;		// 等待发送的消息的队列
	HANDLE						m_hSendThread;
	DWORD						m_dwSendThreadID;

};
