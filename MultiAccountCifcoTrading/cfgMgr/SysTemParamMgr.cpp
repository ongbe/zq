#include "StdAfx.h"
#include <wx/defs.h>
#include "../inc/Module-Misc/GLobalFunc.h"
#include "cfgMgr/ShortCutKeysMgr.h"
#include "cfgmgr/SystemParamMgr.h"

SystemParamMgr::SystemParamMgr(void)
{
	ZeroMemory(&m_SystemParamInfo, sizeof(m_SystemParamInfo));
	m_SystemParamInfo.bWebCifco.xmlset(true);				//默认登录弹出网上中期客户服务平台
	m_SystemParamInfo.bUse.xmlset(false);               //使用隐身热键
	m_SystemParamInfo.prompt.xmlset(1);              //提示声 1.不用 2.pC喇叭 3.多媒体喇叭
	m_SystemParamInfo.nTradeTimeSoundPrompt.xmlset(0);	//开盘收盘及小结休息的声音提示
	m_SystemParamInfo.nTradeEndSoundPrompt.xmlset(0);	//有持仓的合约交易结束前的声音提示
	m_SystemParamInfo.nSoundPromptBeforeTradeEndSec.xmlset(180);	//有持仓的合约交易结束前声音提示提前秒数
	memcpy(&m_DefaultInfo,&m_SystemParamInfo,sizeof(m_SystemParamInfo));
}

SystemParamMgr::~SystemParamMgr(void)
{

}

bool SystemParamMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

    TiXmlElement* pElementParamInfo = root->FirstChildElement("SystemParamInfo");
	if( pElementParamInfo == NULL )
		return false;

	TiXmlElement *pElement = pElementParamInfo->FirstChildElement("SystemHide"); 
    if(pElement)
    {
	    char* pAttriUse = (char*)pElement->Attribute("bUse");
	    char* pAttriPrompt = (char*)pElement->Attribute("prompt");
		char* pAttriKey1 = (char*)pElement->Attribute("key1");
		char* pAttriKey2 = (char*)pElement->Attribute("key2");
		char* pAttriKey3 = (char*)pElement->Attribute("key3");
		char* pAttriKey4 = (char*)pElement->Attribute("key4");
		char* pAttriKey5 = (char*)pElement->Attribute("key5");
		if( pAttriUse && pAttriPrompt && pAttriKey1 && pAttriKey2 && pAttriKey3 
			&& pAttriKey4 && pAttriKey5 )
		{
			m_SystemParamInfo.bUse.xmlset(atoi(pAttriUse) > 0 ? true:false, bFromUserCfg);
		    m_SystemParamInfo.prompt.xmlset(atoi(pAttriPrompt), bFromUserCfg);
			m_SystemParamInfo.key1.xmlset(pAttriKey1, bFromUserCfg);
			m_SystemParamInfo.key2.xmlset(pAttriKey2, bFromUserCfg);
			m_SystemParamInfo.key3.xmlset(pAttriKey3, bFromUserCfg);
			m_SystemParamInfo.key4.xmlset(pAttriKey4, bFromUserCfg);
			m_SystemParamInfo.key5.xmlset(pAttriKey5, bFromUserCfg);
		}
    }

	pElement = pElementParamInfo->FirstChildElement("TradeTimeSoundPrompt");
	if ( pElement != NULL && pElement->GetText() != NULL )
		m_SystemParamInfo.nTradeTimeSoundPrompt.xmlset(atoi(pElement->GetText()), bFromUserCfg);

	pElement = pElementParamInfo->FirstChildElement("TradeEndSoundPrompt");
	if ( NULL != pElement && NULL != pElement->GetText() )
		m_SystemParamInfo.nTradeEndSoundPrompt.xmlset(atoi( pElement->GetText()), bFromUserCfg);

	pElement = pElementParamInfo->FirstChildElement("SoundPromptBeforeTradeEndSec");
	if ( NULL != pElement && NULL != pElement->GetText() )
		m_SystemParamInfo.nSoundPromptBeforeTradeEndSec.xmlset(atoi( pElement->GetText()), bFromUserCfg);

	pElement = pElementParamInfo->FirstChildElement("ShowWebCifco");
	if ( NULL != pElement && NULL != pElement->GetText() )
		m_SystemParamInfo.bWebCifco.xmlset(atoi(pElement->GetText())>0?true:false, bFromUserCfg);
	if(!bFromUserCfg)
		memcpy(&m_DefaultInfo,&m_SystemParamInfo,sizeof(m_SystemParamInfo));
	return true;
}

bool SystemParamMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement* pSystemParamInfo = root->FirstChildElement("SystemParamInfo");
	if( pSystemParamInfo )
		root->RemoveChild( pSystemParamInfo );

	pSystemParamInfo = new TiXmlElement("SystemParamInfo");

	if ( m_SystemParamInfo.bUse.needWriteToUserCfg() ||
		m_SystemParamInfo.prompt.needWriteToUserCfg() ||
		m_SystemParamInfo.key1.needWriteToUserCfg() ||
		m_SystemParamInfo.key2.needWriteToUserCfg() ||
		m_SystemParamInfo.key3.needWriteToUserCfg() ||
		m_SystemParamInfo.key4.needWriteToUserCfg() ||
		m_SystemParamInfo.key5.needWriteToUserCfg())
	{
		TiXmlElement *pItem = new TiXmlElement("SystemHide");
		pItem->SetAttribute( "bUse", m_SystemParamInfo.bUse);
		pItem->SetAttribute( "prompt", m_SystemParamInfo.prompt);
		pItem->SetAttribute( "key1", m_SystemParamInfo.key1);
		pItem->SetAttribute( "key2", m_SystemParamInfo.key2);
		pItem->SetAttribute( "key3", m_SystemParamInfo.key3);
		pItem->SetAttribute( "key4", m_SystemParamInfo.key4);
		pItem->SetAttribute( "key5", m_SystemParamInfo.key5);
		pSystemParamInfo->LinkEndChild( pItem );
	}

	TiXmlElement *pElement = NULL;
	TiXmlText* pContext = NULL;

	if ( m_SystemParamInfo.nTradeTimeSoundPrompt.needWriteToUserCfg())
	{
		pElement = new TiXmlElement("TradeTimeSoundPrompt");
		pContext = new TiXmlText(GlobalFunc::ConvertToString(m_SystemParamInfo.nTradeTimeSoundPrompt.operator int()));
		pElement->LinkEndChild(pContext);
		pSystemParamInfo->LinkEndChild(pElement);
	}

	if ( m_SystemParamInfo.nTradeEndSoundPrompt.needWriteToUserCfg())
	{
		pElement = new TiXmlElement("TradeEndSoundPrompt");
		pContext = new TiXmlText(GlobalFunc::ConvertToString(m_SystemParamInfo.nTradeEndSoundPrompt.operator int()));
		pElement->LinkEndChild(pContext);
		pSystemParamInfo->LinkEndChild(pElement);
	}

	if ( m_SystemParamInfo.nSoundPromptBeforeTradeEndSec.needWriteToUserCfg())
	{
		pElement = new TiXmlElement("SoundPromptBeforeTradeEndSec");
		pContext = new TiXmlText(GlobalFunc::ConvertToString(m_SystemParamInfo.nSoundPromptBeforeTradeEndSec.operator int()));
		pElement->LinkEndChild(pContext);
		pSystemParamInfo->LinkEndChild(pElement);
	}

	if ( m_SystemParamInfo.bWebCifco.needWriteToUserCfg())
	{
		pElement = new TiXmlElement("ShowWebCifco");
		pContext = new TiXmlText(GlobalFunc::ConvertToString(m_SystemParamInfo.bWebCifco.operator bool()));
		pElement->LinkEndChild(pContext);
		pSystemParamInfo->LinkEndChild(pElement);
	}

	if ( NULL != pSystemParamInfo->FirstAttribute() || NULL != pSystemParamInfo->FirstChild())
	{
		root->LinkEndChild(pSystemParamInfo);
	}
	else
	{
		delete pSystemParamInfo;
		pSystemParamInfo = NULL;
	}

	return true;
}

void SystemParamMgr::ClearCfg()
{
	ZeroMemory(&m_SystemParamInfo, sizeof(m_SystemParamInfo));
}

void SystemParamMgr::CalcUsableShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	if ( strlen(m_SystemParamInfo.key2) == 0
		&& strlen(m_SystemParamInfo.key3) == 0
		&& strlen(m_SystemParamInfo.key4) == 0
		&& strlen(m_SystemParamInfo.key5) == 0
		&& strlen(m_SystemParamInfo.key1) != 0)
	{
		pShortCutKeyMgr->DelUsableShortCutKey(m_SystemParamInfo.key1.operator const char*());
	}
}

void SystemParamMgr::ProcessDulplicateShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr, bool bModify )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	if ( strlen(m_SystemParamInfo.key2) == 0
		&& strlen(m_SystemParamInfo.key3) == 0
		&& strlen(m_SystemParamInfo.key4) == 0
		&& strlen(m_SystemParamInfo.key5) == 0
		&& strlen(m_SystemParamInfo.key1) != 0)
	{
		if ( !bModify )
		{
			pShortCutKeyMgr->CalcDulplicateShortCutKey(m_SystemParamInfo.key1.operator const char*());
		}
		else
		{
			if( pShortCutKeyMgr->IsShortKeyDulplicate(m_SystemParamInfo.key1.operator const char*()))
			{
				m_SystemParamInfo.key1.xmlset("", true);
			}
		}
	}
}
