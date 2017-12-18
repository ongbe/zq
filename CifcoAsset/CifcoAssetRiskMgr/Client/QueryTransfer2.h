#pragma once


#include <string>
#include "CDataInfo.h"
#include "UserApiStruct.h"


using namespace std;

#define       NoChoice           "" 
#define       FD_In              "入金" 
#define       FD_Out             "出金" 

class QueryTransfer2
{
public:
	//订阅事件
	static void SubscribeEvt(unsigned int hWnd);
	//退订事件
	static void UnsubscribeEvt(unsigned int hWnd);

	//向服务器请求投资者出入金
    void ReqQueryTransfer(std::set<int> setAccntID);
	//处理出入金消息
    bool ProcessTransferEvtPkg(const char* pPkg,std::map<int, SeqDepositField>& mapSeqDeposit,int fieldcnt,string& Direction);
	// 拆分字符串
	std::vector<std::string> split(const std::string &str);
	//double转string
	string Double2String(double dbData);


    QueryTransfer2(void);
    virtual ~QueryTransfer2(void);
public:
	//基础数据
	CDataInfo* m_pdataInfo;
	MAPTrustTradeAccount	m_mapTrustTradeAccount;

};

