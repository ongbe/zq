// CfgMgr.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Module-Misc2/globalDefines.h"
#include "Module-Misc2/GlobalFunc.h"

#include "ShortCutKeysMgr.h"
#include "InstrumentGroupMgr.h"
#include "ListCfgMgr.h"
#include "RiskCfgMgr.h"
#include "OrderCfgMgr.h"
#include "CancelOrderCfgMgr.h"
#include "MainWindowCfgMgr.h"
#include "FundAccountCfgMgr.h"
#include "PanelSetMgr.h"
#include "VarietyMgr.h"
#include "NumberShortKeyMgr.h"
#include "SysTemParamMgr.h"
#include "MovePositionMgr.h"
#include "ReplacePositionMgr.h"
#include "LanguageMgr.h"
#include "GUIShowSetMgr.h"

#include "CfgMgr.h"
#include "../inc/Module-Misc/constants.h"
#include "ISvr/PlatformSvrMgr.h"

extern PlatformSvrMgr* g_pPlatformMgr;

CfgMgr* CfgMgr::m_pInstance = NULL;

CfgMgr::CfgMgr()
: m_bInit( false )
, m_pShortCutKeysMgr( NULL )
, m_pInstrumentGroupMgr( NULL )
, m_pListCfgMgr( NULL )
, m_pRiskCfgMgr( NULL )
, m_pOrderCfgMgr( NULL )
, m_pCancelOrderCfgMgr( NULL )
, m_pMainWindowCfgMgr( NULL )
, m_pFundAccountCfgMgr( NULL )
, m_pPanelSetMgr( NULL )
, m_pVarietyMgr( NULL )
, m_pNumberShortKeyMgr( NULL )
, m_pSystemParamMgr( NULL )
, m_pMovePositionMgr( NULL )
, m_pReplacePositionMgr( NULL )
, m_pLanguageMgr( NULL )
, m_pGUIShowSetMgr( NULL )
#ifdef _USE_MULTI_LANGUAGE
,m_languageId(LANGUAGE_CHINESE)
#endif
{
	CreateCfgManagers();
}

CfgMgr::~CfgMgr(void)
{
	ReleaseCfgManegers();
}

/**
 * @功能描述: 获取单实例
 * @返 回 值: 无
**/
CfgMgr * CfgMgr::GetInstance()
{
	if ( NULL == m_pInstance )
	{
		m_pInstance = new CfgMgr;
	}

	return m_pInstance;
}

/**
 * @功能描述: 销毁单实例
 * @返 回 值: 无
**/
void CfgMgr::DestroyInstance()
{
	SAFE_DELETE(m_pInstance)
}

void CfgMgr::CreateCfgManagers()
{
	m_pShortCutKeysMgr = new ShortCutKeysMgr;
	m_pInstrumentGroupMgr = new InstrumentGroupMgr;
	m_pRiskCfgMgr = new RiskCfgMgr;
	m_pListCfgMgr = new ListCfgMgr;
	m_pOrderCfgMgr = new OrderCfgMgr;
	m_pCancelOrderCfgMgr = new CancelOrderCfgMgr;
	m_pMainWindowCfgMgr = new MainWindowCfgMgr;
	m_pFundAccountCfgMgr= new FundAccountCfgMgr;
	m_pPanelSetMgr = new PanelSetMgr;
	m_pVarietyMgr = new VarietyMgr;
	m_pNumberShortKeyMgr = new NumberShortKeyMgr;
	m_pSystemParamMgr = new SystemParamMgr;
	m_pMovePositionMgr = new MovePositionMgr;
	m_pReplacePositionMgr = new ReplacePositionMgr;
	m_pLanguageMgr = new LanguageMgr;
	m_pGUIShowSetMgr = new GUIShowSetMgr;
}
//读取配置文件
//1表示成功
//0表示读取syscfg或usercfg失败
//-1表示读取language.csv失败
int CfgMgr::Init()
{
	if( m_bInit )
	{
		return 1;
	}

	if(!m_pLanguageMgr->LoadConfig())
		return -1;
	//读取合约组信息
	m_pInstrumentGroupMgr->InitCfg();

	//先读取系统配置文件syscfg.xml
	std::string strSysFileName =SYSCFG_FILENAME;
	GlobalFunc::GetPath( strSysFileName );
	if ( !GlobalFunc::IsFileExists(strSysFileName))
		return 0;
	
	if(!InitFromCFG( strSysFileName ))
		return 0;
	else
		m_bInit = 1;

	//读取用户配置文件usercfg.xml
	std::string strUserFileName = USERCFG_FILENAME;
	GlobalFunc::GetPath( strUserFileName );
	if ( GlobalFunc::IsFileExists(strUserFileName))
		InitFromCFG( strUserFileName, true );

	//统计重复的快捷键
	m_pShortCutKeysMgr->ClearUsedShortCutKeys();
	m_pVarietyMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, false);
	m_pPanelSetMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, false);
	m_pOrderCfgMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, false);
	m_pCancelOrderCfgMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, false);
	m_pSystemParamMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, false);

	//重复的用户快捷键都清空
	m_pVarietyMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, true);
	m_pPanelSetMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, true);
	m_pOrderCfgMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, true);
	m_pCancelOrderCfgMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, true);
	m_pSystemParamMgr->ProcessDulplicateShortCutKeys(m_pShortCutKeysMgr, true);

	return 1;
}

//重新读取配置文件
//1表示成功
//0表示读取syscfg或usercfg失败
//-1表示读取language.csv失败
int CfgMgr::ReInit()
{
	ClearCfg();

	return Init();
}

bool CfgMgr::InitFromCFG( std::string strFilePath, bool bFromUserCfg )
{
	TiXmlDocument* pXmlDocment = new TiXmlDocument( strFilePath.c_str());
	if( NULL == pXmlDocment)
	{
		return false;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return false;
	}

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return false;
	}

	if( std::string(root->Value()) != "GuiCfgInfo")
	{
		return false;
	}

	m_pFundAccountCfgMgr->InitCfg(root, bFromUserCfg);

	//读取快捷键信息
	m_pShortCutKeysMgr->InitCfg(root, bFromUserCfg);

	//读取列表信息
	m_pListCfgMgr->InitCfg(root, bFromUserCfg);

	//读取风险管理信息
	m_pRiskCfgMgr->InitCfg(root, bFromUserCfg);

	//读取下单配置信息
	m_pOrderCfgMgr->InitCfg(root, bFromUserCfg);

	//读取撤单配置信息
	m_pCancelOrderCfgMgr->InitCfg(root, bFromUserCfg);

	//读取主窗口配置
	m_pMainWindowCfgMgr->InitCfg(root, bFromUserCfg);

	//读取资金配置信息
	m_pFundAccountCfgMgr->InitCfg(root, bFromUserCfg);

	m_pPanelSetMgr->InitCfg(root, bFromUserCfg);

	m_pVarietyMgr->InitCfg(root, bFromUserCfg);

	m_pNumberShortKeyMgr->InitCfg(root, bFromUserCfg);

	m_pSystemParamMgr->InitCfg(root, bFromUserCfg);

	m_pGUIShowSetMgr->InitCfg(root, bFromUserCfg);

	m_pMovePositionMgr->InitCfg( root, bFromUserCfg );

	m_pReplacePositionMgr->InitCfg( root, bFromUserCfg );

	return true;
}

void CfgMgr::ClearCfg()
{
	if ( NULL != m_pLanguageMgr )
	{
		m_pLanguageMgr->ClearCfg();
	}

	if ( NULL != m_pInstrumentGroupMgr )
	{
		m_pInstrumentGroupMgr->ClearCfg();
	}

	if( NULL != m_pShortCutKeysMgr )
	{
		m_pShortCutKeysMgr->ClearCfg();
	}

	if( NULL != m_pListCfgMgr )
	{
		m_pListCfgMgr->ClearCfg();
	}

	if ( NULL != m_pRiskCfgMgr )
	{
		m_pRiskCfgMgr->ClearCfg();
	}
	
	if ( NULL != m_pOrderCfgMgr )
	{
		m_pOrderCfgMgr->ClearCfg();
	}
	
	if ( NULL != m_pCancelOrderCfgMgr )
	{
		m_pCancelOrderCfgMgr->ClearCfg();
	}

	if ( NULL != m_pMainWindowCfgMgr)
	{
		m_pMainWindowCfgMgr->ClearCfg();
	}

	if ( NULL != m_pFundAccountCfgMgr )
	{
		m_pFundAccountCfgMgr->ClearCfg();
	}

	if ( NULL != m_pPanelSetMgr )
	{
		m_pPanelSetMgr->ClearCfg();
	}

	if ( NULL != m_pVarietyMgr )
	{
		m_pVarietyMgr->ClearCfg();
	}

	if ( NULL != m_pNumberShortKeyMgr )
	{
		m_pNumberShortKeyMgr->ClearCfg();
	}

	if ( NULL != m_pSystemParamMgr )
	{
		m_pSystemParamMgr->ClearCfg();
	}

	if ( NULL != m_pGUIShowSetMgr )
	{
		m_pGUIShowSetMgr->ClearCfg();
	}

	if ( NULL != m_pMovePositionMgr )
	{
		m_pMovePositionMgr->ClearCfg();
	}

	if ( NULL != m_pReplacePositionMgr )
	{
		m_pReplacePositionMgr->ClearCfg();
	}

	m_bInit = false;
}

/**
 * @功能描述: 保存配置文件
 * @返 回 值: true: 保存成功 false: 保存失败
**/
bool CfgMgr::Save()
{
	//保存合约组信息
	if(!m_pInstrumentGroupMgr->SaveCfg())
		return false;

    TiXmlDocument* pDoc = new TiXmlDocument;
    if ( NULL == pDoc )
	    return false;
    std::auto_ptr<TiXmlDocument> ptrDoc( pDoc );

	TiXmlElement *root = new TiXmlElement("GuiCfgInfo");
	if ( NULL == root )
	{
		return false;
	}
	pDoc->LinkEndChild( root );
  
	//保存快捷键信息
	if(!m_pShortCutKeysMgr->SaveCfg(root))
		return false;

	//保存列表信息
	if(!m_pListCfgMgr->SaveCfg(root))
		return false;
	//保存风险管理信息
	if(!m_pRiskCfgMgr->SaveCfg(root))
		return false;
	//保存下单配置信息
	if(!m_pOrderCfgMgr->SaveCfg(root))
		return false;
	//保存撤单配置信息
	if(!m_pCancelOrderCfgMgr->SaveCfg(root))
		return false;
	//保存主窗口配置
	if(!m_pMainWindowCfgMgr->SaveCfg(root))
		return false;
	//保存撤单配置信息
	if(!m_pFundAccountCfgMgr->SaveCfg(root))
		return false;

	if( !m_pPanelSetMgr->SaveCfg(root) )
		return false;

	if( !m_pVarietyMgr->SaveCfg(root) )
	    return false;
  
	if( !m_pNumberShortKeyMgr->SaveCfg(root) )
	    return false;
	
	if( !m_pSystemParamMgr->SaveCfg(root) )
		return false;

	if ( !m_pMovePositionMgr->SaveCfg( root ))
	{
		return false;
	}

	if ( !m_pReplacePositionMgr->SaveCfg( root ))
	{
		return false;
	}

	if ( !m_pGUIShowSetMgr->SaveCfg( root ))
	{
		return false;
	}

    std::string strFile =USERCFG_FILENAME;
    GlobalFunc::GetPath( strFile );
    if( !pDoc->SaveFile( strFile.c_str() ))
	    return false;

	return true;
}

void CfgMgr::ReleaseCfgManegers()
{
	SAFE_DELETE(m_pShortCutKeysMgr)
	SAFE_DELETE(m_pInstrumentGroupMgr)
	SAFE_DELETE(m_pListCfgMgr)
	SAFE_DELETE(m_pRiskCfgMgr)
	SAFE_DELETE(m_pOrderCfgMgr)
	SAFE_DELETE(m_pCancelOrderCfgMgr)
	SAFE_DELETE(m_pMainWindowCfgMgr)
    SAFE_DELETE(m_pFundAccountCfgMgr)
	SAFE_DELETE(m_pPanelSetMgr )
	SAFE_DELETE(m_pVarietyMgr )
	SAFE_DELETE(m_pNumberShortKeyMgr )	
	SAFE_DELETE(m_pSystemParamMgr )
	SAFE_DELETE(m_pMovePositionMgr )
	SAFE_DELETE(m_pReplacePositionMgr )
	SAFE_DELETE(m_pLanguageMgr )
	SAFE_DELETE(m_pGUIShowSetMgr )
    m_bInit = false;
}

std::map<int, std::string> CfgMgr::GetShortCutKeyNameMap()
{
	return m_pShortCutKeysMgr->GetShortCutKeyNameMap();
}

std::set<std::string> CfgMgr::GetUsableShortCutKeys()
{
	return m_pShortCutKeysMgr->GetUsableShortCutKeys();
}

void CfgMgr::AddUsableShortCutKey( const std::string& strKey )
{
	m_pShortCutKeysMgr->AddUsableShortCutKey(strKey);
}

void CfgMgr::DelUsableShortCutKey( const std::string& strKey )
{
	m_pShortCutKeysMgr->DelUsableShortCutKey(strKey);
}

bool CfgMgr::IsShortKeyUsable(const std::string& strKey )
{
	return m_pShortCutKeysMgr->IsShortKeyUsable(strKey);
}

void CfgMgr::CalcUsableShortCutKeys()
{
	if ( NULL == m_pShortCutKeysMgr )
	{
		return;
	}

	m_pShortCutKeysMgr->ClearUsableShortCutKeys();
	m_pShortCutKeysMgr->InitUsableShortCutKey();
	m_pVarietyMgr->CalcUsableShortCutKeys(m_pShortCutKeysMgr);
	m_pPanelSetMgr->CalcUsableShortCutKeys(m_pShortCutKeysMgr);
	m_pOrderCfgMgr->CalcUsableShortCutKeys(m_pShortCutKeysMgr);
	m_pCancelOrderCfgMgr->CalcUsableShortCutKeys(m_pShortCutKeysMgr);
	m_pSystemParamMgr->CalcUsableShortCutKeys(m_pShortCutKeysMgr);
}

void CfgMgr::ClearUsableShortCutKeys()
{
	if ( NULL == m_pShortCutKeysMgr )
	{
		return;
	}

	m_pShortCutKeysMgr->ClearUsableShortCutKeys();
}

//合约组信息
int	CfgMgr::GetInstrumentGroupCount()
{
    return m_pInstrumentGroupMgr->GetGroupCount();
}

int CfgMgr::GetNextGroupId()
{
	return m_pInstrumentGroupMgr->GetNextGroupId();
}

LPINSTRUMENT_GROUP_CFG  CfgMgr::GetInstrumentGroupInfo(int GroupIndex)
{
    return m_pInstrumentGroupMgr->GetGroupInfo(GroupIndex);
}

bool   CfgMgr::IsExitGroup(const char *pGroupName)
{
	return m_pInstrumentGroupMgr->IsExitGroup(pGroupName);
}

bool  CfgMgr::DelInstrumentGroup(const char *name)
{
   return m_pInstrumentGroupMgr->DelInstrumentGroup(name);
}

void CfgMgr::DelInstrument( int nGroupId, const char *pInstrumentId )
{
	int nIndex = m_pInstrumentGroupMgr->GetGroupIndexByGroupId(nGroupId);
	if ( nIndex == -1 )
	{
		return;
	}

	m_pInstrumentGroupMgr->DelInstrument(nIndex, pInstrumentId);
}

int	CfgMgr::GetInstrumentGroupMemberCount(int GroupId)
{
    return m_pInstrumentGroupMgr->GetGroupMemberCount(GroupId);
}

std::string	CfgMgr::GetInstrumentId(int GroupIndex, int InstrumentIndex)
{
    return m_pInstrumentGroupMgr->GetGroupInstrumentId(GroupIndex, InstrumentIndex);
}

void CfgMgr::ClearInstrumentGroupVector()
{
	return m_pInstrumentGroupMgr->ClearInstrumentGroupVector();
}
 
void CfgMgr::AddInstrumentGroup(std::string name, bool bVisible, DWORD id)
{
   m_pInstrumentGroupMgr->AddInstrumentGroup( name, bVisible, id);
}

void CfgMgr::AddInstrument(int GroupIndex, const char *pInstrumentId)
{
   m_pInstrumentGroupMgr->AddInstrument(GroupIndex,pInstrumentId);
}

std::vector<std::string> * CfgMgr::GetGroupMember(int GroupIndex)
{
   return m_pInstrumentGroupMgr->GetGroupMember( GroupIndex);		
}

std::vector<std::string> * CfgMgr::GetCurrentGroupMember()
{
	return m_pInstrumentGroupMgr->GetCurrentGroupMember();		
}

int CfgMgr::GetCurrentGruopId( )
{
	return m_pInstrumentGroupMgr->GetCurrentId();
}

std::string CfgMgr::GetCurrentGroupName()
{
	return m_pInstrumentGroupMgr->GetCurrentGroupName();
}

void CfgMgr::SetCurrentGruopId( int id )
{
  return m_pInstrumentGroupMgr->SetCurrentId(id);
}

std::string CfgMgr::GetGroupName(int GroupIndex)
{
	return m_pInstrumentGroupMgr->GetGroupName( GroupIndex);
}

/**
 * @功能描述: 获取指定列表整体风格配置信息
 * @参数列表: ModuleId in: 列表模块ID
 * @返 回 值: 列表风格配置指针
**/
LPLIST_TABLE_CFG CfgMgr::GetListCfg(int nModuleId)
{
    return m_pListCfgMgr->GetListCfg( nModuleId );
}

/**
 * @功能描述: 获取指定列表总列数
 * @参数列表: ModuleId in: 列表模块ID
 * @返 回 值: 指定列表总列数
**/
int	CfgMgr::GetListColCount( int nModuleId )
{
    return m_pListCfgMgr->GetListColCount( nModuleId );
}

/**
 * @功能描述: 获取列表列的配置信息
 * @参数列表: ModuleId in: 列表模块ID
 * @参数列表: index in: 列索引
 * @返 回 值: 列配置指针
**/
LPLIST_COLUMN_CFG CfgMgr::GetListColCfg(int nModuleId, int nIndex)		
{
    return m_pListCfgMgr->GetListColCfg(nModuleId, nIndex);
}

/**
 * @功能描述: 获取指定列表可见列总数
 * @参数列表: ModuleId in: 列表模块ID
 * @返 回 值: 可见列总数
**/
int	CfgMgr::GetListVisibleColCount(int nModuleId)
{
    return m_pListCfgMgr->GetListVisibleColCount(nModuleId);
}

/**
 * @功能描述: 获取指定列表可见列的配置信息
 * @参数列表: nModuleId in: 列表模块ID
 * @参数列表: nIndex in: 可见列索引
 * @返 回 值: 可见列配置指针
**/
LPLIST_COLUMN_CFG CfgMgr::GetListVisibleColCfg(int nModuleId, int nIndex)		
{
    return m_pListCfgMgr->GetListVisibleColCfg(nModuleId, nIndex);
}
std::string CfgMgr::GetColCfgCaption( int nModuleId, int ColumnId)
{
	char buf[1024]={0};
	sprintf(buf,"%d",nModuleId*1000+ColumnId);
	return LoadString(buf);
}
void CfgMgr::SetListColCfg(int ModuleId, LPLIST_TABLE_CFG pList,
							std::map<std::string ,LPLIST_COLUMN_CFG> NewColCfgMap,
							int ColCount)
{
    m_pListCfgMgr->SetListColCfg(ModuleId, pList,NewColCfgMap,ColCount);
}

void CfgMgr::SetListColCfg(int ModuleId, LPLIST_TABLE_CFG pList,
							std::vector<LPLIST_COLUMN_CFG> NewColCfgVec)
{
    m_pListCfgMgr->SetListColCfg(ModuleId, pList,NewColCfgVec);
}

/**
 * @功能描述: 获取指定列表所有列配置信息
 * @参数列表: nModuleId in: 列表模块ID
 * @返 回 值: 列表所有列信息指针
**/
std::vector<LPLIST_COLUMN_CFG>* CfgMgr::GetListColCfg(int nModuleId)
{
    return m_pListCfgMgr->GetListColCfg(nModuleId);
}

//风险管理配置信息
LPRISK_MGR_CFG CfgMgr::GetRiskMgrCfg()
{
    return m_pRiskCfgMgr->GetRiskMgrCfg();
}

//下单配置信息
LPORDER_CFG	CfgMgr::GetOrderCfg()
{
    return m_pOrderCfgMgr->GetOrderCfg();
}

//撤单配置信息
CANCEL_ORDER_CFG CfgMgr::GetCancelOrderCfg()
{
    return m_pCancelOrderCfgMgr->GetCancelOrderCfg();
}

void CfgMgr::SetCancelOrderCfg( const CANCEL_ORDER_CFG& cancelOrderCfg )
{
	m_pCancelOrderCfgMgr->SetCancelOrderCfg( cancelOrderCfg );
}

/**
 * @功能描述: 获取主页面配置信息
 * @返 回 值: 主页面配置指针
**/
LPMAIN_WINDOW_CFG CfgMgr::GetMainWindowCfg()
{
    return m_pMainWindowCfgMgr->GetMainWindowCfg();
}

//资金配置信息
FUND_ACCOUNT_CFG CfgMgr::GetFundAccountCfg()
{
    return m_pFundAccountCfgMgr->GetFundAccountCfg();
}

void CfgMgr::SetFundAccountCfg( const FUND_ACCOUNT_CFG& fundAccountCfg )
{
	m_pFundAccountCfgMgr->SetFundAccountCfg( fundAccountCfg );
}

std::map<int, CListCfg*>* CfgMgr::GetListCfgMap()
{
   return m_pListCfgMgr->GetListCfgMap();   
}

std::vector< PanelInfo>* CfgMgr::GetPanelSetVector()
{
	return &( m_pPanelSetMgr->m_vecPanelInfo );
}
    void SetPanelCaption(int PanelID,const string& Caption);
    string GetPanelCaption(int PanelID);
    void CfgMgr::SetPanelCaption(int PanelID,const std::string& Caption)
{
    m_PanelCaption[PanelID]=Caption;
}
std::string CfgMgr::GetPanelCaption(int PanelID)
{
    if(m_PanelCaption.find(PanelID)!=m_PanelCaption.end()) return m_PanelCaption[PanelID];
    else return  string("");
}

std::vector< VarietyInfo>* CfgMgr::GetVarietyInfoVector()
{
	return &( m_pVarietyMgr->m_VarietyInfo );
}

std::vector< NumberShortKey>* CfgMgr::GetNumberShortKeyVector()
{
	return &( m_pNumberShortKeyMgr->m_NumberShortKeyInfo );
}

SystemParamInfo* CfgMgr::GetSystemParamInfo(bool bDefault)
{
	if(bDefault)
		return &( m_pSystemParamMgr->m_DefaultInfo);
	return &( m_pSystemParamMgr->m_SystemParamInfo );
}

MOVEPOSITIONCFG CfgMgr::GetMovePositionCfg()
{
	return m_pMovePositionMgr->GetMovePositionCfg();
}

void CfgMgr::SetMovePositionCfg( const MOVEPOSITIONCFG& movePosition )
{
	m_pMovePositionMgr->SetMovePositionCfg( movePosition );
}

REPLACEPOSITION CfgMgr::GetReplacePositionCfg()
{
	return m_pReplacePositionMgr->GetReplacePositionCfg();
}

void CfgMgr::SetReplacePositionCfg( const REPLACEPOSITION& replacePosition )
{
	m_pReplacePositionMgr->SetReplacePositionCfg( replacePosition );
}
std::string CfgMgr::LoadString(string id,int langid)
{
#ifdef _USE_MULTI_LANGUAGE
	if(langid<LANGUAGE_MIN||langid>LANGUAGE_MAX)
		langid=m_languageId;
	if(langid==LANGUAGE_ENGLISH)
		return GetEN(id);
	if(langid==LANGUAGE_TRADITION)
		return GetHK(id);
	if(langid==LANGUAGE_CHINESE)
#endif
	return GetCN(id);
}
string CfgMgr::GetFmtString(string pStr)
{
	string substr;
	size_t index=0;
	bool waitalpha=false;
	while(index<pStr.length())
	{
		if(waitalpha){
			if((pStr[index]>='A'&&pStr[index]<='Z')||
				(pStr[index]>='a'&&pStr[index]<='z')){
				substr+=pStr[index];
				waitalpha=false;
			}
			else if(pStr[index]=='%'){
				waitalpha=false;
			}
		}
		else{
			if(pStr[index]=='%')
				waitalpha=true;
		}
		index++;
	}
	return substr;
}
std::string CfgMgr::LoadFormatString(string id,string fmt,int langid)
{
	string src=GetFmtString(LoadString(id,langid));
	string stock=GetFmtString(fmt);
	if(stricmp(src.c_str(),stock.c_str())!=0)
	{
		TRACE("%s format string is mismatch fmt(%s)\n",id.c_str(),fmt.c_str());
		return fmt;
	}
	return LoadString(id,langid);
}
std::string CfgMgr::GetEN(string id)
{
	return m_pLanguageMgr->GetEN(id);
}
std::string CfgMgr::GetCN(string id)
{
	return m_pLanguageMgr->GetCN(id);
}
std::string CfgMgr::GetHK(string id)
{
	return m_pLanguageMgr->GetHK(id);
}

std::string CfgMgr::ReplaceFmtString( std::string str )
{
	char szStart[3] = "[[";
	char szEnd[3] = "]]";
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI)
	{
		int nStartPos = str.find( szStart );
		int nEndPos = str.find( szEnd );
		while ( nStartPos != -1 && nEndPos != -1 && nEndPos > nStartPos )
		{
			str.replace( nStartPos, nEndPos+strlen(szEnd)-nStartPos, "");
			nStartPos = str.find( szStart );
			nEndPos = str.find( szEnd );
		}
	}
	else
	{
		int nStartPos = str.find( szStart );
		while( nStartPos != -1 )
		{
			str.replace( nStartPos, strlen(szStart), "");
			nStartPos = str.find( szStart );
		}

		int nEndPos = str.find( szEnd );
		while( nEndPos != -1 )
		{
			str.replace( nEndPos, strlen(szEnd), "");
			nEndPos = str.find( szEnd );
		}
	}

	return str;
}

int CfgMgr::GetPosX()
{
	return m_pGUIShowSetMgr->GetPosX();
}

int CfgMgr::GetPosY()
{
	return m_pGUIShowSetMgr->GetPosY();
}

void CfgMgr::SetPos( int nPosX, int nPosY )
{
	m_pGUIShowSetMgr->SetPos(nPosX, nPosY);
}

int CfgMgr::GetWidth()
{
	return m_pGUIShowSetMgr->GetWidth();
}

int CfgMgr::GetHeight()
{
	return m_pGUIShowSetMgr->GetHeight();
}

void CfgMgr::SetSize( int nWidth, int nHeight )
{
	m_pGUIShowSetMgr->SetSize(nWidth, nHeight);
}

int CfgMgr::GetStatus()
{
	return m_pGUIShowSetMgr->GetStatus();
}

void CfgMgr::SetStatus( int nStatus )
{
	m_pGUIShowSetMgr->SetStatus(nStatus);
}

std::string CfgMgr::GetDulplicateKeyString()
{
	if ( NULL == m_pShortCutKeysMgr)
	{
		return "";
	}

	return m_pShortCutKeysMgr->GetDulplicateKeyString();
}
