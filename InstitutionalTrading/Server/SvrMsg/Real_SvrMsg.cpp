// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#include "Real_SvrMsg.h"
#include "UserMsgCenter.h"

extern CUserMsgCenter g_userMsgCenter;


CReal_SvrMsg::CReal_SvrMsg()
{
}

CReal_SvrMsg::~CReal_SvrMsg()
{
}

bool CReal_SvrMsg::SendMsg(int nOwnerID, int nTargetUserID, int nMsgType, 
					 int nExpiredDate, const string& strTitle, const string& strMsg)
{
	return g_userMsgCenter.AppendOrigMsg(nOwnerID, nTargetUserID, nMsgType, 
										 nExpiredDate, strTitle, strMsg);
}

bool CReal_SvrMsg::SendMsg(int nOwnerID, const set<int>& setTargetUser, int nMsgType, 
					 int nExpiredDate, const string& strTitle, const string& strMsg)
{
	return g_userMsgCenter.AppendOrigMsg(nOwnerID, setTargetUser, nMsgType, 
										 nExpiredDate, strTitle, strMsg);
}
