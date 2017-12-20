#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRBROKERINFO_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRUSERORG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRBROKERINFO_EXPORTS
#define SVRBROKERINFO_API __declspec(dllexport)
#else
#define SVRBROKERINFO_API __declspec(dllimport)
#endif
#endif

#include <map>
#include <vector>
#include "CommonStruct.h"

class SVRBROKERINFO_API CInterface_SvrBrokerInfo : public CInterface_SvrCommon
{
public:

    //问询函数
    virtual CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf)=0;

    static CInterface_SvrBrokerInfo& getObj(void);

    //获取所有经纪公司信息
	virtual void GetBrokerInfos(std::vector<BrokerInfo>& vec) = 0;
    //通过经纪公司ID获取该信息
    virtual bool GetBrokerInfoByID(int brokerID,BrokerInfo& brokerInfo) = 0;
    //通过经纪公司代码获取该信息
    virtual bool GetBrokerInfoByCode(std::string strCode,BrokerInfo& brokerInfo)=0;

    //获取所有服务器组
	virtual void GetServerGroups(std::vector<ServerGroup>& vec) = 0;
    //获取一个经纪公司ID下的所有服务器组信息
	virtual void GetServerGroupsByBrokerID(int nBrokerID, std::vector<ServerGroup>& vec) = 0;
    //通过服务器组ID获取服务器组信息
	virtual bool GetServerGroupByID(int nGroupID, ServerGroup& group) = 0;

    //获取所有服务器组地址信息
	virtual void GetServerAddr(std::vector<ServerAddress>& vec) = 0;
    //获取一个服务器组ID下的所有服务器组地址信息
	virtual void GetServerAddrByGroupID(int nGroupID, std::vector<ServerAddress>& vec) = 0;

    //获取所有委托交易账号信息
	virtual void GetTradeAccounts(std::vector<TradeAccount>& vec) = 0;
    //获取一个经纪公司ID下的所有委托交易账号信息
	virtual void GetTradeAccountsByBrokerID(int nBrokerID, std::vector<TradeAccount>& vec) = 0;
    //获取一个服务器组ID下的所有委托交易账号信息
	virtual void GetTradeAccountsByGroupID(int nGroupID, std::vector<TradeAccount>& vec) = 0;
    //通过委托交易账号ID获取该信息
	virtual bool GetTradeAccount(int nTradeAccountID, TradeAccount& account) = 0;

    //判断是否有结算服务器组（结算服务器调用）
    virtual bool GetSettleServerGroup() = 0;


    //获取所有委托交易账号和组织机构的关系
    virtual void GetTradeAccountAndOrgRelation(std::vector<TradeAccountAndOrg>& vec) = 0;
    //通过委托交易账号ID获取组织机构ID
    virtual void GetOrgIDByTradeAccountID(const int& nTradeAccountID, int& nOrgID) = 0;
    //通过委托交易账号字符串获取组织机构ID
    virtual void GetOrgIDByBrokerCodeAndTradeAccount(const std::string& szBrokerCode, const std::string& szTradeAccount, int& nOrgID) = 0;
    //通过组织机构ID得到委托交易账户
    virtual void GetTradeAccountByOrgID(const int& nOrgID, std::vector<TradeAccount>& vec) = 0;

};







