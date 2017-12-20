// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CFGMGR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CFGMGR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "CfgGlobal.h"

#include "cfgmgr/ListCfgMgr.h"
#include "cfgmgr/PanelSetMgr.h"
#include "cfgMgr/AccountGroup.h"
#include "cfgMgr/OrderStrategyRowCfg.h"

// This class is exported from the CfgMgr.dll


class ShortCutKeysMgr;
class InstrumentGroupMgr;
class ListCfgMgr;
class RiskCfgMgr;
class OrderCfgMgr;
class CancelOrderCfgMgr;
class MainWindowCfgMgr;
class FundAccountCfgMgr;
class PanelSetMgr;
class VarietyMgr;
class NumberShortKeyMgr;
class SystemParamMgr;
class MovePositionMgr;
class ReplacePositionMgr;
class TiXmlDocument;
class LanguageMgr;
class GUIShowSetMgr;
class AccountGroupMgr;
class MultiAccountMgr;
enum CFG_STYLE;

class CfgMgr 
{
public:
	//获取单实例
	static CfgMgr * GetInstance();

	//销毁单实例
	static void DestroyInstance();

	//保存配置文件
	bool Save();
	//读取配置文件
	//1表示成功
	//0表示读取syscfg或usercfg失败
	//-1表示读取language.csv失败
	int Init();

	//重新读取配置文件
	int ReInit();

public:
	//获取主页面配置信息
	LPMAIN_WINDOW_CFG GetMainWindowCfg();

	//获取指定列表整体风格配置信息
	LPLIST_TABLE_CFG GetListCfg( int nModuleId );

	//获取指定列表总列数
	int	GetListColCount( int nModuleId );

	//获取列表列的配置信息
	LPLIST_COLUMN_CFG GetListColCfg( int nModuleId, int nIndex );

	//获取指定列表可见列总数
	int	GetListVisibleColCount( int nModuleId );

	//获取指定列表可见列的配置信息
	LPLIST_COLUMN_CFG GetListVisibleColCfg( int nModuleId, int nIndex );
	//获取指定列表的文本
	std::string GetColCfgCaption( int nModuleId, int ColumnId);
	//获取指定列表所有列配置信息
	std::vector<LPLIST_COLUMN_CFG>* GetListColCfg( int nModuleId );

	//设置列表列的配置信息
	void SetListColCfg(int nModuleId, LPLIST_TABLE_CFG pList,
		std::map<std::string ,LPLIST_COLUMN_CFG> newColCfgMap,int nColCount);

	//设置列表列的配置信息
	void SetListColCfg(int nModuleId, LPLIST_TABLE_CFG pList,
		std::vector<LPLIST_COLUMN_CFG> newColCfgVec);

	//获取合约组个数
	int	GetInstrumentGroupCount();
	int GetNextGroupId();

	//获取合约组信息
	LPINSTRUMENT_GROUP_CFG GetInstrumentGroupInfo(int GroupIndex);
	//获取合约组成员个数
	int	GetInstrumentGroupMemberCount(int GroupIndex);
	bool IsExitGroup(const char *pGroupName);
	bool DelInstrumentGroup(const char *name);
	void DelInstrument( int nGroupId, const char *pInstrumentId );
	//获取合约组中合约的ID（按顺序获取）
	std::string	GetInstrumentId(int GroupIndex, int InstrumentIndex);
	void ClearInstrumentGroupVector();

	void AddInstrumentGroup(std::string name, bool bVisible, DWORD id);
	void AddInstrument(int GroupIndex, const char *pInstrumentId);

	int GetCurrentGruopId( );
	std::string GetCurrentGroupName();
	void SetCurrentGruopId( int id );

	std::vector<std::string> *  GetGroupMember(int GroupIndex);
	std::vector<std::string> *  GetCurrentGroupMember();
	std::string GetGroupName(int GroupIndex);

	//下单配置信息
	LPORDER_CFG	GetOrderCfg();

	//撤单配置信息
	CANCEL_ORDER_CFG  GetCancelOrderCfg();
	void SetCancelOrderCfg( const CANCEL_ORDER_CFG& cancelOrderCfg );

	//资金配置信息
	FUND_ACCOUNT_CFG GetFundAccountCfg();
	void SetFundAccountCfg( const FUND_ACCOUNT_CFG& fundAccountCfg );

	//风险管理配置信息
	LPRISK_MGR_CFG		GetRiskMgrCfg();
	
	//获取快捷键信息
	std::map<int, std::string> GetShortCutKeyNameMap();
	std::set<std::string> GetUsableShortCutKeys();
	void AddUsableShortCutKey( const std::string& strKey );
	void DelUsableShortCutKey( const std::string& strKey );
	bool IsShortKeyUsable( const std::string& strKey );
	//计算可用的快捷键
	void CalcUsableShortCutKeys();
	void ClearUsableShortCutKeys();
	std::string GetDulplicateKeyString();

	std::map<int, CListCfg*>* GetListCfgMap();
	std::vector< PanelInfo>* GetPanelSetVector();
	std::vector< VarietyInfo>* GetVarietyInfoVector();
	std::vector< NumberShortKey>* GetNumberShortKeyVector();
	SystemParamInfo* GetSystemParamInfo(bool bDefault=false);

	MOVEPOSITIONCFG GetMovePositionCfg();
	void SetMovePositionCfg( const MOVEPOSITIONCFG& movePosition );

	REPLACEPOSITION GetReplacePositionCfg();
	void SetReplacePositionCfg( const REPLACEPOSITION& replacePosition );

	const char* GetGroupText();
	void ApplyGroupText( const char* szBuffer );
	std::string LoadString(std::string id,int langid=-1);
	std::string GetFmtString(std::string pStr);
	std::string LoadFormatString(std::string id,std::string fmt,int langid=-1);
	std::string ReplaceFmtString(std::string str);

	int GetPosX();
	int GetPosY();
	void SetPos(int nPosX, int nPosY);
	int GetWidth();
	int GetHeight();
	void SetSize(int nWidth, int nHeight);
	int GetStatus();
	void SetStatus(int nStatus);

	//用户组设置
	void ClearAccoutGroup();
	bool IsAccountGroupExist( int nGroupID );
	int GetNextAccountGroupID();
	bool AddAccountGroup( int nGroupID, std::string strGroupName );
	bool AddAccount( int nGroupID, int nAccountID, std::string strAccountName );
	bool DelAccount( int nAccountID );
	void GetAccountGroup(std::vector<AccountGroup>& accountGroup);
	void SetAccountGroup( const std::vector<AccountGroup>& accountGroup);
	void SetCurAccountGroupID( int nGroupID );
	int GetCurAccountGroupID();

	MULTIACCOUNT* GetMultiAccountCfg();
	void GetContracts( OrderStrategy eStrategy, std::vector<std::string>& vContracts);
	bool GetOrderStrategyData(OrderStrategy eStrategy, const std::string strAccount, OrderStrategyRowCfg& rowCfg );
	void SetOrderStrategyData(OrderStrategy eStrategy, const std::vector<std::string>& vContracts,
		const std::vector<OrderStrategyRowCfg>& vStrategyData);
	int GetOrderStrategyContractVolume(OrderStrategy eStrategy, const std::string strAccount,
		const std::string strContract, const std::string strContractType);

#ifdef _USE_MULTI_LANGUAGE
public:
	void SetLanguageId(int id){m_languageId=id;}
private:
	int m_languageId;
#endif
private:
	std::string GetEN(std::string id);
	std::string GetCN(std::string id);
	std::string GetHK(std::string id);

	CfgMgr(void);
	~CfgMgr(void);
	void CreateCfgManagers();
	void ReleaseCfgManegers();
	bool InitFromCFG( std::string strFilePath, bool bFromUserCfg = false );
	void ClearCfg();

private:
	static CfgMgr*		m_pInstance;
	ShortCutKeysMgr*	m_pShortCutKeysMgr;
	InstrumentGroupMgr*	m_pInstrumentGroupMgr;
	ListCfgMgr*			m_pListCfgMgr;
	RiskCfgMgr*			m_pRiskCfgMgr;
	OrderCfgMgr*		m_pOrderCfgMgr;
	CancelOrderCfgMgr*	m_pCancelOrderCfgMgr;
	MainWindowCfgMgr*	m_pMainWindowCfgMgr;
	FundAccountCfgMgr*	m_pFundAccountCfgMgr;
	PanelSetMgr*		m_pPanelSetMgr;
	VarietyMgr*			m_pVarietyMgr;
	NumberShortKeyMgr*	m_pNumberShortKeyMgr;
	SystemParamMgr*		m_pSystemParamMgr;
	MovePositionMgr*	m_pMovePositionMgr;
	ReplacePositionMgr* m_pReplacePositionMgr;
	LanguageMgr*		m_pLanguageMgr;
	GUIShowSetMgr*		m_pGUIShowSetMgr;
	AccountGroupMgr*	m_pAccountGroupMgr;
	MultiAccountMgr*	m_pMultiAccountMgr;

	bool				m_bInit;
};

#define CFG_MGR_DEFAULT() (CfgMgr::GetInstance())
#define CFG_MGR_DEFAULT_REINITCFG()  (CfgMgr::GetInstance()->ReInit())
