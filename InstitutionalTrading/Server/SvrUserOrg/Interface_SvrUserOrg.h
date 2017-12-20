#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRUSERORG_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRUSERORG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRUSERORG_EXPORTS
#define SVRUSERORG_API __declspec(dllexport)
#else
#define SVRUSERORG_API __declspec(dllimport)
#endif
#endif

#include <map>
#include <vector>
#include <set>
#include "CommonStruct.h"

class SVRUSERORG_API CInterface_SvrUserOrg : public CInterface_SvrCommon
{
public:
    static CInterface_SvrUserOrg& getObj(void);


    //问询函数
    virtual CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf)=0;

	virtual void GetOrgs(std::vector<Organization>& vec) = 0;
	virtual void GetLowerLevelOrgs(int nOrgID, std::map<int, Organization>& mapOrganization) = 0;
	virtual bool GetOrgByID(int nOrgID, Organization& org) = 0;
    virtual bool IsLeafNode(int nOrgID) = 0;


	virtual void GetUserInfos(std::vector<UserInfo>& vec) = 0;
	virtual bool GetUserByID(int nUserID, UserInfo& userInfo) = 0;
    virtual bool GetUserByName(std::string szUserName, UserInfo& userInfo) = 0;
	virtual void GetUserInfosByOrgID(int nOrgID, std::vector<UserInfo>& vec) = 0;
    virtual void GetUsersIDByOrgID(int nOrgID,std::vector<int>& vecUserID) = 0;//得到该组织机构下所有用户ID 
	virtual int IsValidUser(const std::string& strAccout, const std::string& strPwd,int nUserType, int& nUserID) = 0;

	virtual void GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vec) = 0;
	virtual bool GetOrgIDByUserID(int nUserID, int& nOrgID) = 0;//通过用户ID得到从属组织机构ID
    virtual bool GetOrgIDByAccount(std::string szAccount, int& nOrgID) = 0;
    virtual bool GetUserInfoByAccount(std::string szAccount, UserInfo& userInfo) = 0;
    virtual void GetMonitorUserIDByOrgID(int nOrgID, std::set<int>& vUserID) = 0;//通过组织机构ID得到监控该组织机构的所有风控员ID
    virtual bool GetMonitorOrgIDByUserID(int nUserID,std::vector<int> vecMonitorOrgID) = 0;//通过用户ID得到监控组织机构ID

	virtual void GetUserAndTradeAccountRelation(std::vector<UserAndTradeAccountRelation>& vec) = 0;
	virtual void GetUserAndTradeAccountRelationByUserID(int nUserID, std::vector<UserAndTradeAccountRelation>& vec) = 0;
	virtual bool GetUserAndTradeAccountRelation(int nTradeAccountID, std::vector<UserAndTradeAccountRelation>& vec) = 0;
	virtual void GetUserIDsByTradeAccount(std::string strAccount, std::string strBrokerCode, std::vector<UserInfo>& vec) = 0;
    virtual bool GetTradeAccountByUserID(int nUserID, TradeAccount& vec) = 0;

    virtual int  UpdateLimitTradeAndManualVerify(const int nLoginUserID, const UserAndTradeAccountRelation& info) = 0;
	//权限
	virtual void GetPrivileges(std::vector<Privilege>& vec) = 0;
	virtual void GetPrivileges(int nRoleID, std::vector<Privilege>& vec) = 0;
	virtual void GetPrivilegesByUserID(int nUserID, std::vector<Privilege>& vec) = 0;	
    virtual void GetUserRoles(int nUserID, std::vector<int>& vRoleID) = 0;

};







