#pragma once

#include "CmdMsgQueue.h"

class CThreadSharedData
{
public:
	CThreadSharedData(void);
	~CThreadSharedData(void);

	void OffLineUser( SOCKET hSocket );
	void UpdateOnlineUser(SOCKET hSocket, int nAccountID);
	bool FindUser( SOCKET hSocket,int* pAccID=NULL);
	bool GetOnlineUserSocket(int nAccountID, std::set<SOCKET>& socketSet);
	bool GetOnlineStatus(int nAccountID);

	void AddMsg(const RequestData& data);
	bool GetMsg(RequestData& data);
    void Subscribe(SOCKET s,SubscribeID id,int AccID);
    void UnSubscribe(SOCKET s,SubscribeID id,int AccID);
	void Subscribe(SOCKET s,SubscribeID id,const std::string& Instrument);
	void UnSubscribe(SOCKET s,SubscribeID id,const std::string& Instrument);
	bool GetSubscribeData(const std::string& AccIDOrInst,SubscribeID id,std::set<SOCKET>& socketset) const;
	bool GetSocketsFromUserID(std::set<SOCKET>& socketset) const;
private:
	CCmdMsgQueue m_cmdMsgQueue;

	std::map<int, std::set<SOCKET>> m_mapOnlineUser;
	std::map<SOCKET,int> m_mapOnlineSocket;
	CRITICAL_SECTION m_CS;
	//////////////////////////////////////////////////////////////////////////
	std::map<std::string,std::map<SubscribeID,std::set<SOCKET>>> m_SubscribeInfo;
	std::map<std::string,std::set<SOCKET>>	m_UserID2Socket;
};
