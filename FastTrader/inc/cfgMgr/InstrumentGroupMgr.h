#pragma once

class CInstrumentGroup;

class InstrumentGroupMgr
{
public:
	InstrumentGroupMgr(void);
	virtual ~InstrumentGroupMgr(void);

	//读取配置文件，根据LIST ID填充Map
	bool InitCfg();
	bool SaveCfg();
	void ClearCfg();

	//获取合约组个数
	int	 GetGroupCount();

	LPINSTRUMENT_GROUP_CFG GetGroupInfo(int GroupIndex);

	std::string GetGroupName(int GroupIndex);
	int  GetGroupMemberCount(int GroupIndex);

	//获取合约组成员个数
	std::string GetGroupInstrumentId(int GroupIndex, int InstrumentIndex);

	//获取合约组中合约的ID（按顺序获取）
	std::vector<std::string>* GetGroupMember(int GroupIndex);
	std::vector<std::string>* GetCurrentGroupMember();

	bool IsExitGroup(const std::string strGroupName);
	bool IsExitGroup( int nGroupId );

	int GetGroupIndexByGroupId( int nGroupId );

	//添加合约组，返回合约组index
	int AddInstrumentGroup(std::string strName, bool bVisible, DWORD id);

	int ReplaceInstrumentGroup( int nGroupIndex, const char * szName,
		bool bVisible, DWORD id = -1 );

	//同时删除组成员
	bool DelInstrumentGroup(const char *name); 
	bool DelInstrumentGroup( int nGroupId );

	//添加合约
	void AddInstrument(int GroupIndex, const char *pInstrumentId);					

	//删除合约组所有合约
	void DelInstrument(int GroupIndex);

	//删除合约
	void DelInstrument(int GroupIndex, const char *pInstrumentId);

	int GetCurrentId( );
	std::string GetCurrentGroupName();
	void SetCurrentId( int id );

	void ClearInstrumentGroupVector();

	int GetNextGroupId();

private:
	DWORD m_dwCurrentGroupId;
	DWORD m_nMaxGroupId;
	std::vector<CInstrumentGroup*> m_InstrumentGroupVector;
};
