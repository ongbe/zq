#ifndef EVENTPARAM_H_
#define EVENTPARAM_H_

//定义并维护Event的参数
//1. 保证线程安全
//2. 每个EventParam有一个唯一的EventParamID，标志改EventParam
//3. 每个EventParam存续时间最大为8秒，超时删除
//4. 程序开始时Init，最后退出时Release，甚至不用Release也可
//5. 这是个单实例类

#include <time.h>
#include <map>
#include <string>
using namespace std;

#ifdef WIN32
#pragma managed(push,off)
#endif 


struct Stru_EventParam
{
public:
	Stru_EventParam(unsigned int eventParamID,unsigned int eventID,const char* peventString,const void* peventData,int eventDataLen,int eventUserInt)
	:EventParamID(eventParamID),
	 CreateTime(time(NULL)),
	 EvtID(eventID),
	 EvtString(peventString?peventString:""),
	 EvtDataLen(0),
	 pEvtData(NULL),
	 EvtUserInt(eventUserInt)
	{
		if(peventData&&eventDataLen>0)
		{
			if(eventDataLen<=(int)sizeof(LittleEvtBuf))
			{
				pEvtData=LittleEvtBuf;
				EvtDataLen=eventDataLen;
			}
			else
			{
				pEvtData=malloc(eventDataLen);
				if(pEvtData)
					EvtDataLen=eventDataLen;
			}

			if(pEvtData&&EvtDataLen>0)
				memcpy((char*)pEvtData,peventData,EvtDataLen);
		}
	}
	Stru_EventParam(const Stru_EventParam& r)
	:EventParamID(0),
	 CreateTime(0),
	 EvtID(0),
	 EvtString(),
	 EvtDataLen(0),
	 pEvtData(NULL),
	 EvtUserInt(0)
	{
		*this=r;
	}
	~Stru_EventParam()
	{
		if(pEvtData&&(char*)pEvtData!=(char*)LittleEvtBuf)
		{
			free(pEvtData);
			pEvtData=NULL;
		}
	}
	virtual Stru_EventParam& operator=(const Stru_EventParam& r)
	{
		EventParamID=r.EventParamID;
		CreateTime=r.CreateTime;
		EvtID=r.EvtID;
		EvtString=r.EvtString;
		EvtUserInt=r.EvtUserInt;
		if(r.pEvtData&&r.EvtDataLen>0)
		{
			if(r.EvtDataLen<=(int)sizeof(LittleEvtBuf))
			{
				pEvtData=LittleEvtBuf;
				EvtDataLen=r.EvtDataLen;
			}
			else
			{
				pEvtData=malloc(r.EvtDataLen);
				if(pEvtData)
					EvtDataLen=r.EvtDataLen;
			}

			if(pEvtData&&EvtDataLen>0)
				memcpy((char*)pEvtData,r.pEvtData,EvtDataLen);
		}
		return *this;
	}

    unsigned int EventParamID;
    time_t CreateTime;
    unsigned int EvtID;
    string EvtString;
    int EvtDataLen;
    void *pEvtData;
    int EvtUserInt;

private:
	char LittleEvtBuf[4096];
};

class CEventParam
{
public:
	//取对象，必要时创建对象
	static CEventParam& getObj(void);

    //创建一个EventParam，返回RltEventParamID
    bool CreateEventParam(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString=NULL,const void* peventData=NULL,int eventDataLen=0,int eventUserInt=0);
    //删除一个EventParma，应用层确保不再使用后可以删除
    bool DeleteEventParam(unsigned int EventParamID);
    //获取事件参数，如果pRltEventPtr为空，可以只获取pRltEventPtrLen值；pRltEventID、pRltEventString及pRltEventPtr为空时不返回相应值
    bool GetEventParam(unsigned int EventParamID,unsigned int* pRltEventID,string* pRltEventString,void* pRltEventData,int* pRltEventDataLen,int MaxRltEventDataLen,int* pRltEventUserInt);

private:
    map<unsigned int,Stru_EventParam> m_mapEvtParam;

	CEventParam();
	virtual ~CEventParam();
	static CEventParam* m_pObj;
};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif