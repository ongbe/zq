///////////////////////////////////////////////////////////
//  PrivilegeManage.h
//  Implementation of the Class PrivilegeManage
//  Created on:      16-二月-2013 17:29:01
//  Original author: admin
///////////////////////////////////////////////////////////

#if !defined(EA_C0C5F35D_29B7_42c8_A5D1_8FEA4AB9D82E__INCLUDED_)
#define EA_C0C5F35D_29B7_42c8_A5D1_8FEA4AB9D82E__INCLUDED_

#include <vector>
#include <map>
#include "CommonStruct.h"

class PrivilegeManage
{
public:
	PrivilegeManage();
	virtual ~PrivilegeManage();

	bool LoadDB();

	void GetPrivileges(std::vector<Privilege>& vec);
	void GetPrivileges(int nRoleID, std::vector<Privilege>& vec);
	int  SaveRolePrivilegeRelation(const int nUserID, int nRoleID, std::vector<int>& vecPrivilegeID);
    int  GetRolePrivilegeRelation(std::vector<RoleAndPrivilegeRelation>& vec);
    int  GetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2PrivilegeRelation);

	int AddRole(const int nUserID, RoleInfo& role, int& nID);
	int ModifyRole(const int nUserID, const RoleInfo& role);
	int DelRole(const int nUserID, int nID);
	void GetRoles(std::vector<RoleInfo>& vec);

    //从服务器初始化数据
    void SetServerRunMode(eServerRunMode nMode);
    const std::map<int,RoleInfo>& GetRoleInfo();
    const std::vector<Privilege>& GetPrivilege();
    const std::vector<RoleAndPrivilegeRelation>& GetRoleAndPrivilegeRelation();

    void SetRoleInfo(std::map<int,RoleInfo>& mapRoleInfo);
    void SetPrivilege(std::vector<Privilege>& vecPrivilege);
    void SetRoleAndPrivilegeRelation(std::vector<RoleAndPrivilegeRelation>& vecRoleAndPrivilegeRelation);
    void SetRoleID2PrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2PrivilegeRelation);
	
private:
    bool LoadRoleInfo();
	bool LoadPrivileges();
    bool LoadRolePrivilegeRelation();

public:
	std::map<int,RoleInfo> m_mapRoleInfo;
	std::vector<Privilege> m_vecPrivilege;
    std::vector<RoleAndPrivilegeRelation> m_vecRoleAndPrivilegeRelation;
    std::map<int,std::vector<int>> m_mapRoleID2PrivilegeRelation;//角色与权限关系

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
	int m_nNextRoleID;

    eServerRunMode m_ServerRunMode;
};
#endif // !defined(EA_C0C5F35D_29B7_42c8_A5D1_8FEA4AB9D82E__INCLUDED_)
