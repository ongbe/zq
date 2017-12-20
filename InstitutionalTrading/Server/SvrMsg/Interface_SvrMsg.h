#pragma once

#include "Interface_SvrCommon.h"
#include <map>
#include <set>


using namespace std;

#ifndef SVRMSG_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRMSG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRMSG_EXPORTS
#define SVRMSG_API __declspec(dllexport)
#else
#define SVRMSG_API __declspec(dllimport)
#endif
#endif


class SVRMSG_API CInterface_SvrMsg : public CInterface_SvrCommon
{
public:
    static CInterface_SvrMsg& getObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------

	// 向单个目标用户发送消息
	// nOwnerID，消息发起人
	// nTargetUserID，目标用户
	// nMsgType，消息类型
	// nExpiredDate，有效日期
	// strTitle，消息标题
	// strMsg，消息正文
	// 返回值，true，消息被保存；false，消息无法保存发送。
	virtual bool SendMsg(int nOwnerID, int nTargetUserID, int nMsgType, 
						 int nExpiredDate, const string& strTitle, const string& strMsg) = 0;
	
	// 向多个目标用户发送消息
	// nOwnerID，消息发起人
	// setTargetUser，目标用户的集合
	// nMsgType，消息类型
	// nExpiredDate，有效日期
	// strTitle，消息标题
	// strMsg，消息正文
	// 返回值，true，消息被保存；false，消息无法保存发送。
	virtual bool SendMsg(int nOwnerID, const set<int>& setTargetUser, int nMsgType, 
						 int nExpiredDate, const string& strTitle, const string& strMsg) = 0;
private:
    static CInterface_SvrMsg* m_pObj;
};







