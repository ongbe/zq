#pragma once

#include <string>
#include <map>
#include <set>
#include "CommonStruct.h"
#include "..\SvrTcp\Interface_SvrTcp.h"
#include "..\SvrLogin\Interface_SvrLogin.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"

using namespace std;

class CUserRecvMsgCore
{
public:
	static int DB_UpdateMsgRecvStatus(int nTargetMsgID, bool bHasSent);

public:
	CUserRecvMsgCore(void);
	virtual ~CUserRecvMsgCore(void);

	void SetMaster(bool bHas) {
		m_bMaster = bHas;
	};
	bool GetMaster() {
		return m_bMaster;
	};
	void SetTargetUserID(int nUserID) {
		m_nTargetUserID = nUserID;
	};
	int  GetTargetUserID() {
		return m_nTargetUserID;
	};

	bool Reinit();
	bool AppendTargetMsg(int nTargetMsgID, UserMsgMemStru& userMsg);
	//bool AddNoSendTargetMsg(int nTargetMsgID, UserMsgMemStru& userMsg);
	bool SendTargetMsg(int nSocket, bool bForSlaveSynchr);
	bool UpdateTargetMsgToSent(int nTargetMsgID, 
								bool bSynchr, int nSrcSocket);

protected:
	int GetLocalDate();
	bool GetUserSocket(int nUserID, vector<SOCKET>& vecSocket);
	bool SynchroMsgSentStatus(UserMsgStatusStru& msgStatus, int nSrcSocket);

protected:
	bool						m_bMaster;
	int							m_nTargetUserID;
	map<int, UserMsgMemStru>	m_mapTargetMsg;		// key = nTargetMsgID, data = msg
};
