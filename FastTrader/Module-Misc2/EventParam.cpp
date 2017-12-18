#include "StdAfx.h"
#include "EventParam.h"

using namespace std;

#ifdef WIN32
#pragma unmanaged
#endif 


CRITICAL_SECTION CFTEventParam::m_CS;
DWORD CFTEventParam::m_BaseID=1;
map<DWORD,Stru_EventParam> CFTEventParam::m_mapEvtParam;


//程序开始时Init
void CFTEventParam::Init(void)
{
    InitializeCriticalSection(&m_CS);
    m_BaseID=1;
    m_mapEvtParam.clear();
}

//程序退出时Release，也可以不Release，让操作系统自动释放资源
void CFTEventParam::Release(void)
{
    EnterCriticalSection(&m_CS);
    while(m_mapEvtParam.begin()!=m_mapEvtParam.end())
    {
        char* ptmp=m_mapEvtParam.begin()->second.EvtPtr;
        m_mapEvtParam.erase(m_mapEvtParam.begin());
        if(ptmp!=NULL) free(ptmp);
    }
    LeaveCriticalSection(&m_CS);

    DeleteCriticalSection(&m_CS);
}

//创建一个EventParam，返回RltEventParamID
bool CFTEventParam::CreateEventParam(DWORD& RltEventParamID,DWORD* pEventID,string* pEventString,const void* pEventPtr,int EventPtrLen,int EventUserInt)
{
    EnterCriticalSection(&m_CS);

    time_t curtime;
    time(&curtime);

    //检查有没有超时的时间参数
    map<DWORD,Stru_EventParam>::iterator it=m_mapEvtParam.begin();
    while(it!=m_mapEvtParam.end())
    {
        if(curtime<it->second.CreateTime||
            curtime>it->second.CreateTime&&curtime-it->second.CreateTime>=60)
        {
            it=m_mapEvtParam.erase(it);
        }
        else 
        {
            it++;
        }
    }

    //创建事件参数
    Stru_EventParam EvtParam;
    EvtParam.BaseID=m_BaseID++;
    time(&EvtParam.CreateTime);
    EvtParam.EvtID = (pEventID!=NULL ? (*pEventID) : 0);
    EvtParam.EvtString.clear();
    if(pEventString) EvtParam.EvtString=*pEventString;
    EvtParam.EvtPtr=NULL;
    EvtParam.EvtPtrLen=0;
    if(pEventPtr&&EventPtrLen>0)
    {
        EvtParam.EvtPtr=(char*)malloc(EventPtrLen);
        if(EvtParam.EvtPtr!=NULL)
        {
            memcpy((char*)EvtParam.EvtPtr,(char*)pEventPtr,EventPtrLen);
            EvtParam.EvtPtrLen=EventPtrLen;
        }
    }
    EvtParam.EvtUserInt=EventUserInt;

    RltEventParamID=EvtParam.BaseID;

    m_mapEvtParam.insert(pair<DWORD,Stru_EventParam>(EvtParam.BaseID,EvtParam));

    LeaveCriticalSection(&m_CS);

    return true;
}

//删除一个EventParma，应用层确保不再使用后可以删除
bool CFTEventParam::DeleteEventParam(DWORD EventParamID)
{
    bool brlt=false;

    EnterCriticalSection(&m_CS);

    map<DWORD,Stru_EventParam>::iterator it=m_mapEvtParam.find(EventParamID);
    if(it!=m_mapEvtParam.end())
    {
        char *ptmp=it->second.EvtPtr;
        m_mapEvtParam.erase(it);
        if(ptmp) free(ptmp);
        brlt=true;
    }

    LeaveCriticalSection(&m_CS);

    return brlt;
}

//获取事件参数，如果pRltEventPtr为空，可以只获取pRltEventPtrLen值；pRltEventID、pRltEventString及pRltEventPtr为空时不返回相应值
bool CFTEventParam::GetEventParam(DWORD EventParamID,DWORD* pRltEventID,string* pRltEventString,void* pRltEventPtr,int* pRltEventPtrLen,int MaxRltEventPtrLen,int* pRltEventUserInt)
{
    bool brlt=false;

    EnterCriticalSection(&m_CS);

    map<DWORD,Stru_EventParam>::iterator it=m_mapEvtParam.find(EventParamID);
    if(it!=m_mapEvtParam.end())
    {
        if(it->second.EvtPtr!=NULL&&pRltEventPtr!=NULL&&it->second.EvtPtrLen>MaxRltEventPtrLen)
        {
            brlt=false;
        }
        else
        {
            if(pRltEventID!=NULL)
                *pRltEventID=it->second.EvtID;
            if(pRltEventString!=NULL)
                *pRltEventString=it->second.EvtString;
            if(pRltEventPtrLen!=NULL)
                *pRltEventPtrLen=it->second.EvtPtrLen;
            if(it->second.EvtPtr!=NULL&&pRltEventPtr!=NULL&&it->second.EvtPtrLen<=MaxRltEventPtrLen)
                memcpy((char*)pRltEventPtr,it->second.EvtPtr,it->second.EvtPtrLen);
            if(pRltEventUserInt!=NULL)
                *pRltEventUserInt=it->second.EvtUserInt;
            brlt=true;
        }
    }
    else 
    {
        brlt=false;
    }

    LeaveCriticalSection(&m_CS);
    return brlt;
}
