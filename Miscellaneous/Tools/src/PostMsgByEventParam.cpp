/*
 * msg.cpp
 *
 *  定义通知消息，非托管
 *	只能应用于Windows编程
 *
 *  Created on: May 20, 2013
 *      Author: root
 */


#include "PostMsgByEventParam.h"

#include <windows.h>
#include "EventParam.h"

#pragma unmanaged


auto_ptr<CPostMsgByEventParam> CPostMsgByEventParam::m_pObj;


//获取类的唯一实例指针。进程生命周期内只会创建一次。
CPostMsgByEventParam& CPostMsgByEventParam::getObj(void)
{
    //用running和InterlockedCompareExchange保证互斥，volatile避免编译器优化
    static volatile long running=0;
    while(InterlockedCompareExchange(&running,1,0)==1) Sleep(0);
    if(m_pObj.get()==NULL)
        m_pObj.reset(new CPostMsgByEventParam());
    InterlockedExchange(&running,0);
    return *m_pObj;
}
CPostMsgByEventParam::CPostMsgByEventParam()
: m_hwnd_MsgReceiver(0) 
{
}
CPostMsgByEventParam::~CPostMsgByEventParam()
{
    m_pObj.release();
}

bool CPostMsgByEventParam::postMsgStr(int msgID,unsigned int evtID,const string& msgstring,const void* pdata,int datalen,int evtUserInt1,int evtUserInt2)
{
    if(!m_hwnd_MsgReceiver) return false;
	bool brlt=postMsgStr2(m_hwnd_MsgReceiver,msgID,evtID,msgstring,pdata,datalen,NULL,0,evtUserInt1,evtUserInt2);
	if(!brlt&&m_hwnd_MsgReceiver&&!IsWindow((HWND)m_hwnd_MsgReceiver))
		m_hwnd_MsgReceiver=NULL;
	return brlt;
}

bool CPostMsgByEventParam::postMsgStr2(int hwnd,int msgID,unsigned int evtID,const string& msgstring,const void* pdata,int datalen,const void* pdata2,int datalen2,int evtUserInt1,int evtUserInt2)
{
    if(!hwnd) return false;

	unsigned int EvtID;
	if(!CEventParam::getObj().CreateEventParam2(EvtID,evtID,msgstring.c_str(),pdata,datalen,pdata2,datalen2,evtUserInt1,evtUserInt2))
        return false;
	if(!PostMessage((HWND)hwnd,WM_COMMAND,(WPARAM)msgID,(LPARAM)EvtID))
	{
		CEventParam::getObj().DeleteEventParam(EvtID);
		return false;
	}
    return true;
}

bool CPostMsgByEventParam::postMsgStrThread(unsigned long idThread,int msgID,unsigned int evtID,const string& msgstring,const void* pdata,int datalen,const void* pdata2,int datalen2,int evtUserInt1,int evtUserInt2)
{
	if(!idThread) return false;

    unsigned int EvtID;
	if(!CEventParam::getObj().CreateEventParam2(EvtID,evtID,msgstring.c_str(),pdata,datalen,pdata2,datalen2,evtUserInt1,evtUserInt2))
        return false;
	if(!PostThreadMessage(idThread,WM_COMMAND,(WPARAM)msgID,(LPARAM)EvtID))
	{
		CEventParam::getObj().DeleteEventParam(EvtID);
		return false;
	}
	return true;
}
