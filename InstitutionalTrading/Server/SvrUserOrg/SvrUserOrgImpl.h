#pragma once

#include <map>
#include <vector>
#include "PrivilegeManage.h"
#include "CommonStruct.h"
#include "Interface_SvrUserOrg.h"
#include "rwlock.h"

class PrivilegeManage;

class CSvrUserOrgImpl : public CInterface_SvrUserOrg
{
public:
	static CSvrUserOrgImpl& getObj(void);
	static void deleteObj();

    CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf);

    void SetServerRunMode(eServerRunMode nMode);
    char* GetData(int& nDataLen);
    void  SetData(	std::map<int,Organization>& mapOrg,
                    std::map<int,UserInfo>& mapUserInfo,
                    std::map<int,RoleInfo>& mapRoleInfo,
                    std::vector<Privilege>& vPrivilege,
                    std::vector<RoleAndPrivilegeRelation>& vRoleAndPrivilegeRelation,
                    std::vector<UserAndTradeAccountRelation>& vUserAndTradeAccountRelation,
                    std::vector<UserAndOrgRelation>& vUserAndOrgRelation,
                    std::vector<UserRole>& vUserRoleRelation);


	int AddOrg(const int nUserID, Organization& org, int& nID);
	int ModifyOrg(const int nUserID, const Organization& org);
	int DelOrg(const int nUserID, int nID);
	void GetOrgs(std::vector<Organization>& vec);
	void GetLowerLevelOrgs(int nOrgID, std::map<int, Organization>& mapOrganization);
	bool GetOrgByID(int nOrgID, Organization& org);
    bool IsLeafNode(int nOrgID);

//     void SetUserAssign(int nUserID, bool bAssign);
	int AddUserInfo(const int nUserID, UserInfo& userInfo, int& nID);
	int ModifyUserInfo(const int nUserID, const UserInfo& userInfo);
    int ModifyUserPasswordFromRisk(UserPassword& userPassword, int& nID);
    int UnLockPassword(UserPassword& userPassword, int& nID);
    int SetUserStatus(UserStatus& userStatus);
	int DelUserInfo(const int nUserID, int nID);
	void GetUserInfos(std::vector<UserInfo>& vec);
	bool GetUserByID(int nUserID, UserInfo& userInfo);
    bool GetUserByName(std::string szUserName, UserInfo& userInfo);
	void GetUserInfosByOrgID(int nOrgID, std::vector<UserInfo>& vec);
    void GetUserInfosByOrgID(int nOrgID, UserType nUserType, std::vector<UserInfo>& vecUserInfo);
    void GetUserInfosByOrgID(int nOrgID, UserType nUserType, int nRelationType, std::vector<UserInfo>& vecUserInfo);
    void GetUsersIDByOrgID(int nOrgID,std::vector<int>& vecUserID);
    void GetUsersIDByOrgID(std::map<int, std::map<int, Organization>>& mapMain,std::map<int, Organization>& mapSub, std::vector<int>& vecUserID); 
	int  IsValidUser(const std::string& strAccout, const std::string& strPwd, int nUserType, int& nUserID);
    UserType GetUserType(int nUserID);

    int SaveUserOrgRelation(const int nUserID, int nOrgID, int nUserType, int nTraderID, int nRelationType);
    int SaveUserOrgRelation(const int nUserID, int nOrgID, int nUserType, std::vector<int>& vecInID, std::vector<int>& vecOutID, int nRelationType);
    int SaveUserOrgRelation(const int nUserID, int nRiskID, int nAttachOrg, std::vector<int>& vecMonitorOrg);
	void GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vec);
	bool GetOrgIDByUserID(int nUserID, int& nOrgID);
    bool GetUserInfoByAccount(std::string szAccount, UserInfo& userInfo);
    bool GetOrgIDByAccount(std::string szAccount, int& nOrgID);
    int  GetAttachOrgIDByUserID(int nUserID);
	void GetMonitorUserIDByOrgID(int nOrgID, std::set<int>& vUserID);
    bool GetMonitorOrgIDByUserID(int nUserID,std::vector<int> vecMonitorOrgID);//通过用户ID得到监控组织机构ID

    bool IsAddUserTradeAcctRelation(int nUserID, int nTraderAcctID, int& bHaveRele);
	int AddUserAndTradeAccountRelation(const int nLoginUserID, const UserAndTradeAccountRelation& info);
	int DelUserAndTradeAccountRelation(const int nLoginUserID, int nUserID);
	void GetUserAndTradeAccountRelation(std::vector<UserAndTradeAccountRelation>& vec);
	void GetUserAndTradeAccountRelationByUserID(int nUserID, std::vector<UserAndTradeAccountRelation>& vec);
	bool GetUserAndTradeAccountRelation(int nTradeAccountID, std::vector<UserAndTradeAccountRelation>& vec);
	void GetUserIDsByTradeAccount(std::string strAccount, std::string strBrokerCode, std::vector<UserInfo>& vec);
    bool GetTradeAccountByUserID(int nUserID, TradeAccount& tradeAccount);

    CF_ERROR SetLimitTradeFromRisk(int nUserID, bool bTrue);
    CF_ERROR SetManualVerifyFromRisk(int nUserID, bool bTrue);
    int  UpdateLimitTradeAndManualVerify(const int nLoginUserID, const UserAndTradeAccountRelation& info);

	//权限
	void GetPrivileges(std::vector<Privilege>& vec);
	void GetPrivileges(int nRoleID, std::vector<Privilege>& vec);
    void GetPrivilegesByUserID(int nUserID, std::vector<Privilege>& vec);

	int  SaveRolePrivilegeRelation(const int nUserID, int nRoleID, std::vector<int>& vec);
    int  GetRolePrivilegeRelation(std::vector<RoleAndPrivilegeRelation>& vec);

	int AddRole(const int nUserID, RoleInfo& name, int& nID);
	int ModifyRole(const int nUserID, const RoleInfo& name);
	int DelRole(const int nUserID, int nID);
	void GetRole(std::vector<RoleInfo>& vec);

	int SaveUserRole(const int nLoginUserID, int nUserID, std::vector<int> vRoleID);
    void GetUserRoles(std::vector<UserRole>& vUserRole);
	void GetUserRoles(int nUserID, std::vector<int>& vRoleID);
    void GetUserIDByRoleID( int nRoleID, std::vector<int>& vUserID );

    //由角色和权限关系 用户(管理员)与角色关系获得用户(管理员)的权限关系
    bool bUserHavePrivilege(int nUserID, int nPrivilegeID);
    
public:
    bool LoadDB();

private:	
	bool LoadOrgFromDB();
	bool LoadUserFromDB();
	bool LoadUserAndTradeAccountRelationFromDB();
	bool LoadUserAndOrgRelationFromDB();
	bool LoadTradeAccountAndOrgRelationFromDB();
    bool LoadUserAndRoleRelationFromDB();

private:
	CSvrUserOrgImpl();
	virtual ~ CSvrUserOrgImpl();
	static CSvrUserOrgImpl* m_pObj;

	std::map<int,Organization> m_mapOrg;
    std::map<int, std::map<int, Organization>> m_mapAssetMgmtOrgTree;//一个组织机构ID下的所有组织机构信息
	std::map<int,UserInfo> m_mapUserInfo;
	PrivilegeManage m_permissionMgr;

	std::vector<UserAndTradeAccountRelation> m_vUserAndTradeAccountRelation;
	std::vector<UserAndOrgRelation> m_vUserAndOrgRelation;
    std::vector<UserRole> m_vecUserRoleRelation;
    std::map<int, std::vector<UserRole>> m_mapUserRoleRelation;//用户与角色关系

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
	int m_nNextUserID;
	int m_nNextOrgID;

    rwlock_t    m_Lock;

    eServerRunMode m_ServerRunMode;
};
