#pragma once
#include "stdafx.h"
#include "MsgQueue.h"
#include "CommonDefine.h"
#include "monitorCommonDefine.h"
class CCmdMsgQueue : public CMsgQueue<RecvData>
{
public:
	CCmdMsgQueue(void);
	~CCmdMsgQueue(void);
};
class CCmdClinetMsgQueue : public CMsgQueue<RecvDataMsg>
{
public:
	CCmdClinetMsgQueue(void);
	~CCmdClinetMsgQueue(void);
};
