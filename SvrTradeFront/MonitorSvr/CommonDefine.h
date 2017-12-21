#pragma once

#include "stdafx.h"
#include "SocketBase.h"

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)
#endif


struct SClientKey
{
	Stru_IPAddress RemoteAddr;
	SOCKET hSocket;
	SClientKey()
	{
		memset(this,0,sizeof(*this));
	}
	SClientKey(Stru_IPAddress RemoteAddr1, SOCKET hSocket1)
	{
		RemoteAddr = RemoteAddr1;
		hSocket    = hSocket1;
	}
	bool operator< (const SClientKey& r) const
	{		
		if(RemoteAddr.IP < r.RemoteAddr.IP)
			return true;
		else if(RemoteAddr.IP > r.RemoteAddr.IP)
			return false;

		if(RemoteAddr.Port < r.RemoteAddr.Port)
			return true;
		else if(RemoteAddr.Port > r.RemoteAddr.Port)
			return false;

		if(hSocket < r.hSocket)
			return true;
		else if(hSocket > r.hSocket)
			return false;
		return false;
	}
};
