#include "StdAfx.h"
#include "EventParam.h"
#include "easymutex.h"
#include "FileOpr.h"
#include <windows.h>
#include "FileOpr.h"
#include "Tools_Win32.h"

using namespace std;

#ifdef WIN32
#pragma unmanaged
#endif 

auto_ptr<CEventParam> CEventParam::m_pObj;


//获取类的唯一实例指针。进程生命周期内只会创建一次。
CEventParam& CEventParam::getObj(void)
{
    //用running和InterlockedCompareExchange保证互斥，volatile避免编译器优化
    static volatile long running=0;
    while(InterlockedCompareExchange(&running,1,0)==1) Sleep(0);
    if(m_pObj.get()==NULL)
        m_pObj.reset(new CEventParam());
    InterlockedExchange(&running,0);

    return *m_pObj;
}


CEventParam::CEventParam()
:	m_FirstValidEvtParam(1),EventParamBaseID(1)
{
//#ifdef _DEBUG
//	CFileOpr::getObj().writelocallog("LogEventParam","CEventParam(): this=%x EventParamBaseID=%d/%x m_mutex=%x",this,EventParamBaseID,&EventParamBaseID,&m_mutex);
//#endif

	CPUTickCounter_CreateEventParam=0;
	CreateEventParamCnt=0;
	CPUTickCounter_GetEventParam=0;
	GetEventParamCnt=0;
	CPUTickCounter_DeleteEventParam=0;
	DeleteEventParamCnt=0;

    EvtCnt=0;
    MaxEvtCnt=0;
	CurMemUsage=0;
	MaxMemUsage=0;

	for(int i=0;i<MAXEVENTPARAM;i++)
		m_pEvtParams[i]=new Stru_EventParam(EvtCnt,MaxEvtCnt,CurMemUsage,MaxMemUsage);
}

CEventParam::~CEventParam()
{
	for(int i=0;i<MAXEVENTPARAM;i++)
	{
		if(m_pEvtParams[i])
		{
			delete m_pEvtParams[i];
			m_pEvtParams[i]=NULL;
		}
	}

    m_pObj.release();
}

//创建一个EventParam，返回RltEventParamID
bool CEventParam::CreateEventParam(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString,const void* peventData,int eventDataLen,int eventUserInt,int eventUserInt2)
{
	bool brlt=CreateEventParam2(RltEventParamID,eventID,peventString,peventData,eventDataLen,NULL,0,eventUserInt,eventUserInt2);
	return brlt;
}
bool CEventParam::CreateEventParam2(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString,const void* peventData,int eventDataLen,const void* peventData2,int eventDataLen2,int eventUserInt,int eventUserInt2)
{
#ifdef CALCUCPUTIME_EVENTPARAM
	LONGLONG t1=CTools_Win32::MyGetCpuTickCounter();
#endif

    m_mutex.lock();

    time_t curtime;
    time(&curtime);

    //检查有没有超时的时间参数
    //EventParamID是按时间排序的，如果前面的没有超时，后面的就不会超时
    int EventParamBaseOrd=EventParamBaseID%MAXEVENTPARAM;
    while(m_FirstValidEvtParam!=EventParamBaseOrd)
    {
		if(m_pEvtParams[m_FirstValidEvtParam]&&
			m_pEvtParams[m_FirstValidEvtParam]->EventParamID!=0&&
			curtime-m_pEvtParams[m_FirstValidEvtParam]->CreateTime<60)
		{
			break;
		}

		if(m_pEvtParams[m_FirstValidEvtParam]&&
			m_pEvtParams[m_FirstValidEvtParam]->EventParamID!=0&&
			curtime-m_pEvtParams[m_FirstValidEvtParam]->CreateTime>=60)
		{
            m_pEvtParams[m_FirstValidEvtParam]->clear();
		}

        m_FirstValidEvtParam++;
        if(m_FirstValidEvtParam==MAXEVENTPARAM) m_FirstValidEvtParam=0;
    }

    //创建事件参数,跳过NULL的m_pEvtParams
    EventParamBaseID++;
    while(!m_pEvtParams[EventParamBaseID%MAXEVENTPARAM]) EventParamBaseID++;
	RltEventParamID=EventParamBaseID;
    int EventParamOrd=RltEventParamID%MAXEVENTPARAM;
    int stringlen = peventString ? (int)strlen(peventString) : 0;
    m_pEvtParams[EventParamOrd]->set(RltEventParamID,eventID,stringlen>0?peventString:NULL,stringlen,peventData,eventDataLen,peventData2,eventDataLen2,eventUserInt,eventUserInt2);

	m_mutex.unlock();

#ifdef CALCUCPUTIME_EVENTPARAM
    LONGLONG t2=CTools_Win32::MyGetCpuTickCounter();
	CPUTickCounter_CreateEventParam+=(int)((t2-t1)/(LONGLONG)1000);
#endif
	CreateEventParamCnt++;

//#ifdef _DEBUG
//    CFileOpr::getObj().writelocallog("LogEventParam","CreateEventParam2(): this=%x EventParamBaseID=%d/%x m_mutex=%x,RltEventParamID=%d,eventID=%d",this,EventParamBaseID,&EventParamBaseID,&m_mutex,RltEventParamID,eventID);
//#endif

	return true;
}

//删除一个EventParma，应用层确保不再使用后可以删除
bool CEventParam::DeleteEventParam(unsigned int EventParamID)
{
//#ifdef _DEBUG
//    CFileOpr::getObj().writelocallog("LogEventParam","DeleteEventParam(): this=%x EventParamBaseID=%d/%x m_mutex-%x,EventParamID=%d",this,EventParamBaseID,&EventParamBaseID,&m_mutex,EventParamID);
//#endif
    bool brlt=false;

#ifdef CALCUCPUTIME_EVENTPARAM
	LONGLONG t1=CTools_Win32::MyGetCpuTickCounter();
#endif

	m_mutex.lock();

    int EventParamOrd=EventParamID%MAXEVENTPARAM;
    if(m_pEvtParams[EventParamOrd]&&
        m_pEvtParams[EventParamOrd]->EventParamID==EventParamID&&
        EventParamID!=0)
    {
        //清除EventParamOrd
        m_pEvtParams[EventParamOrd]->clear();

        //更新m_FirstValidEvtParam
        while(m_FirstValidEvtParam!=EventParamOrd)
        {
            if(m_pEvtParams[m_FirstValidEvtParam]->EventParamID==0)
            {
                m_FirstValidEvtParam++;
                if(m_FirstValidEvtParam==MAXEVENTPARAM) m_FirstValidEvtParam=0;
            }
            else break;
        }
    }

#ifdef CALCUCPUTIME_EVENTPARAM
	LONGLONG t2=CTools_Win32::MyGetCpuTickCounter();
	CPUTickCounter_DeleteEventParam+=(int)((t2-t1)/(LONGLONG)1000);
#endif

	DeleteEventParamCnt++;

	m_mutex.unlock();
    return brlt;
}

//获取事件参数，如果pRltEventPtr为空，可以只获取pRltEventPtrLen值；pRltEventID、pRltEventString及pRltEventPtr为空时不返回相应值
bool CEventParam::GetEventParam(unsigned int EventParamID,unsigned int* pRltEventID,string* pRltEventString,void* pRltEventData,int* pRltEventDataLen,int MaxRltEventDataLen,int* pRltEventUserInt,int* pRltEventUserInt2)
{
	bool brlt=GetEventParam2(false,EventParamID,pRltEventID,pRltEventString,pRltEventData,pRltEventDataLen,MaxRltEventDataLen,NULL,NULL,0,pRltEventUserInt,pRltEventUserInt2);
	return brlt;
}
bool CEventParam::GetEventParam2(bool bDel,
                                 unsigned int EventParamID,
                                 unsigned int* pRltEventID,
                                 string* pRltEventString,
                                 void* pRltEventData,int* pRltEventDataLen,int MaxRltEventDataLen,
                                 void* pRltEventData2,int* pRltEventDataLen2,int MaxRltEventDataLen2,
                                 int* pRltEventUserInt,
                                 int* pRltEventUserInt2)
{
    bool brlt=false;

#ifdef CALCUCPUTIME_EVENTPARAM
	LONGLONG t1=CTools_Win32::MyGetCpuTickCounter();
#endif

	m_mutex.lock();

    int EventParamOrd=EventParamID%MAXEVENTPARAM;
    if(m_pEvtParams[EventParamOrd]&&
        m_pEvtParams[EventParamOrd]->EventParamID==EventParamID&&
        EventParamID!=0)
    {
        if(m_pEvtParams[EventParamOrd]->pdata&&pRltEventData&&m_pEvtParams[EventParamOrd]->EvtDataLen>MaxRltEventDataLen||
            m_pEvtParams[EventParamOrd]->pdata&&pRltEventData2&&m_pEvtParams[EventParamOrd]->EvtDataLen2>MaxRltEventDataLen2)
        {
            brlt=false;
        }
        else
        {
            if(pRltEventID!=NULL)
                *pRltEventID=m_pEvtParams[EventParamOrd]->EvtID;
            if(pRltEventString!=NULL)
            {
                if(m_pEvtParams[EventParamOrd]->pdata&&m_pEvtParams[EventParamOrd]->EvtStringLen>0)
                    pRltEventString->assign(m_pEvtParams[EventParamOrd]->pdata,m_pEvtParams[EventParamOrd]->EvtStringLen);
                else pRltEventString->clear();
            }
            if(pRltEventDataLen!=NULL)
                *pRltEventDataLen=m_pEvtParams[EventParamOrd]->EvtDataLen;
            if(m_pEvtParams[EventParamOrd]->pdata&&pRltEventData&&m_pEvtParams[EventParamOrd]->EvtDataLen<=MaxRltEventDataLen)
                memcpy((char*)pRltEventData,m_pEvtParams[EventParamOrd]->pdata+m_pEvtParams[EventParamOrd]->EvtStringLen,m_pEvtParams[EventParamOrd]->EvtDataLen);
            if(pRltEventDataLen2!=NULL)
                *pRltEventDataLen2=m_pEvtParams[EventParamOrd]->EvtDataLen2;
            if(m_pEvtParams[EventParamOrd]->pdata&&pRltEventData2&&m_pEvtParams[EventParamOrd]->EvtDataLen2<=MaxRltEventDataLen2)
                memcpy((char*)pRltEventData2,m_pEvtParams[EventParamOrd]->pdata+m_pEvtParams[EventParamOrd]->EvtStringLen+m_pEvtParams[EventParamOrd]->EvtDataLen,m_pEvtParams[EventParamOrd]->EvtDataLen2);
            if(pRltEventUserInt!=NULL)
                *pRltEventUserInt=m_pEvtParams[EventParamOrd]->EvtUserInt;
            if(pRltEventUserInt2!=NULL)
                *pRltEventUserInt2=m_pEvtParams[EventParamOrd]->EvtUserInt2;

			if(bDel) 
            {
                //清除EventParamOrd
                m_pEvtParams[EventParamOrd]->clear();

                //更新m_FirstValidEvtParam
                while(m_FirstValidEvtParam!=EventParamOrd)
                {
                    if(m_pEvtParams[m_FirstValidEvtParam]->EventParamID==0)
                    {
                        m_FirstValidEvtParam++;
                        if(m_FirstValidEvtParam==MAXEVENTPARAM) m_FirstValidEvtParam=0;
                    }
                    else break;
                }
            }

            brlt=true;
        }
    }

#ifdef CALCUCPUTIME_EVENTPARAM
	LONGLONG t2=CTools_Win32::MyGetCpuTickCounter();
	CPUTickCounter_GetEventParam+=(int)((t2-t1)/(LONGLONG)1000);
#endif

	GetEventParamCnt++;

	m_mutex.unlock();
    return brlt;
}

bool CEventParam::GetEventParam3(unsigned int EventParamID,
                                 unsigned int* pRltEventID,
                                 string* pRltEventString,
                                 int* pRltEventDataLen,
                                 int* pRltEventDataLen2,
                                 int* pRltEventUserInt,
                                 int* pRltEventUserInt2)
{
	bool brlt=GetEventParam2(
        false,
        EventParamID,
        pRltEventID,
        pRltEventString,
        NULL,pRltEventDataLen,0,
        NULL,pRltEventDataLen,0,
        pRltEventUserInt,
        pRltEventUserInt2);
	return brlt;
}

