#pragma once
#include "CommonDataChangeEvent.h"
#include "Interface_SvrCommon.h"
#include <map>
#include <queue>
using namespace std;


#include "easymutex.h"
#include "windows.h"


//互斥锁
extern Ceasymutex			g_mutex;


//互斥锁
extern Ceasymutex			g_mutexEvent;

extern DWORD				g_idThread;

//订阅内部事件通知的线程map, cmdid -> idThread。
//收到内部事件通知时，用PostThreadMessage()将事件包推送给订阅的线程。
//wParam=WndCmd_NotifyEventArrival
//lParam=EventParamID，EventParam的pEventData是包数据，Stru_UniPkgHead + 数据 + 包尾(4)，userint=hSocket
extern multimap<eEventType,unsigned int> g_mapDataType2Thread;

//问询接口map
extern multimap<eAskType, CInterface_SvrCommon * const> g_mapDataType2Interface;



