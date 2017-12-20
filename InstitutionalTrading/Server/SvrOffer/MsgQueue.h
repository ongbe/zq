#pragma once
#include "stdafx.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)
#endif
template<class T>
class CMsgQueue
{
public:
	CMsgQueue(void)
	{
		InitializeCriticalSection(&m_CS);
	}

	virtual ~CMsgQueue(void)
	{
		DeleteCriticalSection(&m_CS);
	}
	void DelMsg()
	{
		EnterCriticalSection(&m_CS);
		m_DataQueue.clear();
		LeaveCriticalSection(&m_CS);
	}
	void AddMsg(const T& pParam)
	{
		EnterCriticalSection(&m_CS);		
		m_DataQueue.push(pParam);
		LeaveCriticalSection(&m_CS);
	}
	bool GetMsg(T& pParam)
	{
		bool bRet=false;
		EnterCriticalSection(&m_CS);
		int count=m_DataQueue.size();		
		if(count>0)
		{			
			T& data=m_DataQueue.front();
			memcpy(&pParam,&data,sizeof(T));
			m_DataQueue.pop();			
			bRet=true;
		}
		LeaveCriticalSection(&m_CS);
		return bRet;
	}
	bool GetMsgVector(std::vector<T>& pParamVector)
	{
		bool bRet=false;
		EnterCriticalSection(&m_CS);
		int count=m_DataQueue.size();		
		while(count>0)
		{		
			T pParam;
			T& data=m_DataQueue.front();
			memcpy(&pParam,&data,sizeof(T));
			m_DataQueue.pop();			
			pParamVector.push_back(pParam);
			count=m_DataQueue.size();	
		}
		if(pParamVector.size() >0)
			bRet = true;
		LeaveCriticalSection(&m_CS);
		return bRet;
	}
protected:
	std::queue<T> m_DataQueue;
	CRITICAL_SECTION m_CS;
private:
	DISALLOW_COPY_AND_ASSIGN(CMsgQueue);
};
enum EnumQueryContent
{
	QUERYCOTENT_NULL = 1,
	QUERYCOTENT_INSTRUMENT,//合约
	QUERYCOTENT_ORDER,     //报单
	QUERYCOTENT_TRADE,     //成交	
	QUERYCOTENT_COMMISSION,//合约手续费率
	QUERYCOTENT_FUND,		//资金
	QUERYCOTENT_POSITION,   //持仓
	QUERYCOTENT_POSITIONDETAIL,//持仓明细

	QUERYCOTENT_MARGIN,    //保证金率
};
enum EnumQueryStatus
{
	QUERYSTATUS_INIT = 1,//初始化查询状态
	QUERYSTATUS_BEGIN,   //查询开始
	QUERYSTATUS_FAILURE, //查询失败
	QUERYSTATUS_SUCCESS, //查询成功

};
struct STradeQueryData 
{
	EnumQueryContent			enumQueryContent;	  //查询的类型
	std::string					strInstrument;		  //查询的合约
	std::string					strHedgeFlag;		 //投机套保标志；查询保证金的时候用到
	DWORD						dwBeginTickCount;    //开始时间 
	int							nRepeatCount;		 //重试的次数 目前重复3次
	EnumQueryStatus				enumStatus;			  //查询状态	
	STradeQueryData()
	{
		enumQueryContent = QUERYCOTENT_NULL;
		strInstrument    = "";
		strHedgeFlag	 = THOST_FTDC_HF_Speculation;
		dwBeginTickCount = GetTickCount();
		nRepeatCount     = 0;
		enumStatus		 = QUERYSTATUS_INIT;
	}
	STradeQueryData& operator= (const STradeQueryData& r)
	{
		enumQueryContent = r.enumQueryContent;
		strInstrument	 = r.strInstrument;
		strHedgeFlag	 = r.strHedgeFlag;
		dwBeginTickCount = r.dwBeginTickCount;
		nRepeatCount     = r.nRepeatCount;
		enumStatus		 = r.enumStatus;	

		return *this;
	}	
};
class  CTradeDataMsgQueue:
	public CMsgQueue<STradeQueryData>
{
public:
	CTradeDataMsgQueue(void){}
	virtual ~CTradeDataMsgQueue(void){}
	void AddMsg(STradeQueryData& sData)
	{
		EnterCriticalSection(&m_CS);
		m_DataQueue.push(sData);
		LeaveCriticalSection(&m_CS);
	}
	int  GetSize()
	{
		int nSize = 0;
		EnterCriticalSection(&m_CS);
		nSize = m_DataQueue.size();
		LeaveCriticalSection(&m_CS);

		return nSize;
	}	
};
