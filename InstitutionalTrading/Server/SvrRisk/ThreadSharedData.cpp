#include "StdAfx.h"
#include "ThreadSharedData.h"

CThreadSharedData::CThreadSharedData(void)
{
	InitializeCriticalSection(&m_CS);
}

CThreadSharedData::~CThreadSharedData(void)
{
/*	RequestData reqData;
	while( m_cmdMsgQueue.GetMsg(reqData) )
	{
		if ( NULL != reqData.pData )
		{
			delete[] reqData.pData;
			reqData.pData = NULL;
		}
	}
*/
	DeleteCriticalSection(&m_CS);
}
/*
void CThreadSharedData::OffLineUser( SOCKET hSocket )
{
	EnterCriticalSection(&m_CS);

	std::map<int, std::set<SOCKET>>::iterator it_online = m_mapOnlineUser.begin();
	for ( ; it_online != m_mapOnlineUser.end(); it_online++ )
	{
		int nCount = it_online->second.erase(hSocket);
		if ( nCount != 0 )
		{
			if ( it_online->second.empty() )
			{
				m_mapOnlineUser.erase(it_online);
			}

			break;
		}
	}
	m_mapOnlineSocket.erase(hSocket);
	//¸üÐÂ¶©ÔÄ
	std::map<std::string,std::map<SubscribeID,std::set<SOCKET>>>::iterator it;
	for(it=m_SubscribeInfo.begin();it!=m_SubscribeInfo.end();++it)
	{
		std::map<SubscribeID,std::set<SOCKET>>& nestmap=it->second;
		std::map<SubscribeID,std::set<SOCKET>>::iterator nestit;
		for(nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
		{
			std::set<SOCKET>& sset=nestit->second;
			sset.erase(hSocket);
		}
	}   
	LeaveCriticalSection(&m_CS);
}*/
/*
void CThreadSharedData::AddMsg( const RequestData& data )
{
	EnterCriticalSection(&m_CS);
	m_cmdMsgQueue.AddMsg(data);
	LeaveCriticalSection(&m_CS);
}

bool CThreadSharedData::GetMsg( RequestData& data )
{
	bool bRet = false;
	EnterCriticalSection(&m_CS);
	bRet = m_cmdMsgQueue.GetMsg(data);
	LeaveCriticalSection(&m_CS);

	return bRet;
}
*//*
void CThreadSharedData::UpdateOnlineUser( SOCKET hSocket, int nAccountID )
{
	EnterCriticalSection(&m_CS);

	std::map<int, std::set<SOCKET>>::iterator it_online = m_mapOnlineUser.find(nAccountID);
	if ( it_online != m_mapOnlineUser.end() )
	{
		it_online->second.insert( hSocket );
	}
	else
	{
		std::set<SOCKET> socketSet;
		socketSet.insert( hSocket );
		m_mapOnlineUser[nAccountID] = socketSet;
	}
	m_mapOnlineSocket[hSocket]=nAccountID;

	LeaveCriticalSection(&m_CS);
}

bool CThreadSharedData::FindUser( SOCKET hSocket,int* pAccID )
{
	EnterCriticalSection(&m_CS);

	bool bFind = false;
	std::map<SOCKET,int>::iterator it = m_mapOnlineSocket.find(hSocket);
	if ( it != m_mapOnlineSocket.end() )
	{
		if(pAccID)
			*pAccID=it->second;
		bFind = true;
	}

	LeaveCriticalSection(&m_CS);

	return bFind;
}

bool CThreadSharedData::GetOnlineUserSocket( int nAccountID, std::set<SOCKET>& socketSet )
{
	socketSet.clear();
	bool bFind = false;
	EnterCriticalSection(&m_CS);
	std::map<int, std::set<SOCKET>>::iterator it_online = m_mapOnlineUser.find(nAccountID);
	if ( it_online != m_mapOnlineUser.end() )
	{
		socketSet = it_online->second;
		bFind = true;
	}
	LeaveCriticalSection(&m_CS);

	return bFind;
}

bool CThreadSharedData::GetOnlineStatus( int nAccountID )
{
	bool bOnline = false;
	EnterCriticalSection(&m_CS);
	std::map<int, std::set<SOCKET>>::iterator it_online = m_mapOnlineUser.find(nAccountID);
	if ( it_online != m_mapOnlineUser.end() )
	{
		if( !it_online->second.empty())
		{
			bOnline = true;
		}
		else
		{
			bOnline = false;
		}
	}
	LeaveCriticalSection(&m_CS);

	return bOnline;
}*/
void CThreadSharedData::Subscribe(SOCKET s,SubscribeID id,int AccID)
{
	char AccIDbuffer[256];
	sprintf_s(AccIDbuffer,256,"%d",AccID);
	Subscribe(s,id,AccIDbuffer);
}
void CThreadSharedData::UnSubscribe(SOCKET s,SubscribeID id,int AccID)
{
	char AccIDbuffer[256];
	sprintf_s(AccIDbuffer,256,"%d",AccID);
	UnSubscribe(s,id,AccIDbuffer);
}
void CThreadSharedData::Subscribe(SOCKET s,SubscribeID id,const std::string& Instrument)
{
	std::map<std::string,std::map<SubscribeID,std::set<SOCKET>>>::iterator it=m_SubscribeInfo.find(Instrument);
	if(it==m_SubscribeInfo.end())
	{
		std::map<SubscribeID,std::set<SOCKET>> nestmap;
		std::set<SOCKET> sset;
		sset.insert(s);
		nestmap[id]=sset;
		m_SubscribeInfo[Instrument]=nestmap;
	}
	else
	{
		std::map<SubscribeID,std::set<SOCKET>>& nestmap=it->second;
		std::map<SubscribeID,std::set<SOCKET>>::iterator nestit=nestmap.find(id);
		if(nestit==nestmap.end())
		{
			std::set<SOCKET> sset;
			sset.insert(s);
			nestmap[id]=sset;
		}
		else
		{
			std::set<SOCKET>& sset=nestit->second;
			sset.insert(s);
		}
	}
}
void CThreadSharedData::UnSubscribe(SOCKET s,SubscribeID id,const std::string& Instrument)
{
	std::map<std::string,std::map<SubscribeID,std::set<SOCKET>>>::iterator it=m_SubscribeInfo.find(Instrument);
	if(it!=m_SubscribeInfo.end())
	{
		std::map<SubscribeID,std::set<SOCKET>>& nestmap=it->second;
		std::map<SubscribeID,std::set<SOCKET>>::iterator nestit=nestmap.find(id);
		if(nestit!=nestmap.end())
		{
			std::set<SOCKET>& sset=nestit->second;
			sset.erase(s);
		}
	}
}
bool CThreadSharedData::GetSubscribeData(const std::string& AccIDOrInst,SubscribeID id,std::set<SOCKET>& socketset) const
{
	std::map<std::string,std::map<SubscribeID,std::set<SOCKET>>>::const_iterator it=m_SubscribeInfo.find(AccIDOrInst);
	if(it!=m_SubscribeInfo.end())
	{
		const std::map<SubscribeID,std::set<SOCKET>>& nestmap=it->second;
		std::map<SubscribeID,std::set<SOCKET>>::const_iterator nestit=nestmap.find(id);
		if(nestit!=nestmap.end())
		{
			socketset=nestit->second;
			return true;
		}
	}
	return false;
}/*
bool CThreadSharedData::GetSocketsFromUserID(std::set<SOCKET>& socketset) const
{
	
	std::map<SOCKET,int>::const_iterator it;
	for (it=m_mapOnlineSocket.begin();it!=m_mapOnlineSocket.end();++it)
	{
		socketset.insert(it->first);
	}
	return socketset.size()>0;
}
*/