#include "StdAfx.h"
#include "CFormUserMsgQueryData.h"

#include "TcpLayer.h"

bool CltServerMgr::CFormUserMsgQueryData::NewQuery(int nUserID, std::string& strDate, bool bIsSend)
{
	int nDate = atoi(strDate.c_str());
	CTcpLayer::SendData(
			(bIsSend ? Cmd_UserMsg_Query_Send_Req : Cmd_UserMsg_Query_Recv_Req), 
			(void*)&nDate, sizeof(nDate), 0, nUserID, 0, 0, nUserID);

	return true;
}

void CltServerMgr::CFormUserMsgQueryData::ResetQuery_Send()
{
	LockObject();
	mapKey2Index_Send.clear();
	mapIndex2Key_Send.clear();
	//mapLastMsg_Send.clear();
	mapSendMsg_HasRecv.clear();
	mapSendMsg_NotRecv.clear();
	UnlockObject();
}

void CltServerMgr::CFormUserMsgQueryData::ResetQuery_Recv()
{
	LockObject();
	mapKey2Index_Recv.clear();
	mapIndex2Key_Recv.clear();
	//mapLastMsg_Recv.clear();
	UnlockObject();
}
