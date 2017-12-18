#pragma once

#include "CfgGlobal.h"

#include <string>
#include <vector>

class CInstrumentGroup
{
public:
	CInstrumentGroup(void);
	virtual ~CInstrumentGroup(void);

	bool IsExitGroup(const char *pInstrumentId);

	//添加合约
	void AddInstrument(const char *pInstrumentId);

	//删除合约
	void DelInstrument(const char *pInstrumentId);

	void ClearInstrument();

	//获取成员个数
	int  GetGroupMemberCount();

	std::string GetInstrumentId(int nIndex);  

	//获取合约列表
	std::vector<std::string>* GetGroupMember();

	LPINSTRUMENT_GROUP_CFG GetGroupInfo();

	std::string GetGroupName();
	int GetGroupId();
	int IsVisible();

	void SetGroupName(const char *name);
	void SetGroupId(DWORD id);
	void SetVisible( bool bVisible);

private:
	INSTRUMENT_GROUP_CFG	  m_InstrumentGroupCfg;
	std::vector<std::string>  m_InstrumentVector;
};
