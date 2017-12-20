#pragma once

//定义并维护Event的参数
//1. 保证线程安全
//2. 每个EventParam有一个唯一的EventParamID，标志改EventParam
//3. 每个EventParam存续时间最大为8秒，超时删除
//4. 程序开始时Init，最后退出时Release，甚至不用Release也可

#include <windows.h>
#include <time.h>
#include <string>
#include <map>
using namespace std;

struct Stru_EventParam
{
    DWORD BaseID;
    time_t CreateTime;
    DWORD EvtID;
    string EvtString;
    int EvtPtrLen;
    char *EvtPtr;
    int EvtUserInt;
};

class CEventParam
{
public:
    //程序开始时Init
    static void Init(void);         

    //程序退出时Release，也可以不Release，让操作系统自动释放资源
    static void Release(void);             

    //创建一个EventParam，返回RltEventParamID
    static bool CreateEventParam(DWORD& RltEventParamID,DWORD* pEventID,string* pEventString,const void* pEventPtr,int EventPtrLen,int EventUserInt);
    //删除一个EventParma，应用层确保不再使用后可以删除
    static bool DeleteEventParam(DWORD EventParamID);
    //获取事件参数，如果pRltEventPtr为空，可以只获取pRltEventPtrLen值；pRltEventID、pRltEventString及pRltEventPtr为空时不返回相应值
    static bool GetEventParam(DWORD EventParamID,DWORD* pRltEventID,string* pRltEventString,void* pRltEventPtr,int* pRltEventPtrLen,int MaxRltEventPtrLen,int* pRltEventUserInt);

private:
    static CRITICAL_SECTION m_CS;
    static DWORD m_BaseID;
    static map<DWORD,Stru_EventParam> m_mapEvtParam;

};
