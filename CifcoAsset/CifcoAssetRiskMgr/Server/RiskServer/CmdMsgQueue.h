#pragma once
#include "MsgQueue.h"
#include "SocketCommon.h"

class CCmdMsgQueue : public CMsgQueue<RequestData>
{
public:
	CCmdMsgQueue(void);
	~CCmdMsgQueue(void);
};
