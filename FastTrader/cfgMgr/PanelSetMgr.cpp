#include "StdAfx.h"

#include <wx/defs.h>
#include "Module-Misc2/globalDefines.h"
#include "cfgmgr/PanelSetMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "cfgMgr/ShortCutKeysMgr.h"

extern PlatformSvrMgr* g_pPlatformMgr;

PanelSetMgr::PanelSetMgr(void)
{
	m_vecPanelInfo.clear();
}

PanelSetMgr::~PanelSetMgr(void)
{
	m_vecPanelInfo.clear();
}

bool PanelSetMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

	TiXmlElement* pPanelSet = root->FirstChildElement("PanelSet");
	if( NULL == pPanelSet )
	{
		return false;
	}

	PanelInfo oPanelInfo;
	const char* pValue = NULL;
	for(TiXmlElement *pItem = pPanelSet->FirstChildElement("item"); 
		pItem; 
		pItem = pItem->NextSiblingElement("item"))
	{
        ZeroMemory(&oPanelInfo, sizeof(oPanelInfo));

		pValue = pItem->Attribute("id");
		if( NULL == pValue )
			continue;
		else
			oPanelInfo.id = atoi( pValue );

		if ( DEFAULT_SVR() && !DEFAULT_SVR()->IsModuleValid( oPanelInfo.id ))
		{
			continue;
		}

		pValue = pItem->Attribute("AliasName");
		if(pValue)
			strncpy(oPanelInfo.AliasName, pValue, sizeof(oPanelInfo.AliasName)-1);

		pValue = pItem->Attribute("ShortKey") ;
		if(pValue)
			strncpy(oPanelInfo.ShortKey, pValue, sizeof(oPanelInfo.ShortKey)-1);

		pValue = pItem->Attribute("bVisiable");
		if(pValue)
			oPanelInfo.bVisiable =  atoi( pValue );

		if ( !bFromUserCfg )
		{
			m_vecPanelInfo.push_back( oPanelInfo );
		}
		else
		{
			for ( size_t i = 0; i < m_vecPanelInfo.size(); i++ )
			{
				if ( m_vecPanelInfo[i].id == oPanelInfo.id )
				{
					m_vecPanelInfo[i] = oPanelInfo;
				}
			}
		}
	}

	return true;
}

bool PanelSetMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement* pPanelSet = root->FirstChildElement("PanelSet");
	if( pPanelSet )
	{
		root->RemoveChild( pPanelSet );
	}

	pPanelSet = new TiXmlElement("PanelSet");

	for( unsigned int i=0; i< m_vecPanelInfo.size(); ++i )
	{
		TiXmlElement *pItem = new TiXmlElement("item");

		pItem ->SetAttribute( "id", m_vecPanelInfo[i].id);
		pItem ->SetAttribute( "ShortKey", m_vecPanelInfo[i].ShortKey);
		pItem ->SetAttribute( "AliasName", m_vecPanelInfo[i].AliasName);
		pItem->SetAttribute( "bVisiable",  m_vecPanelInfo[i].bVisiable );

		pPanelSet->LinkEndChild( pItem );

	}

	if ( NULL != pPanelSet->FirstAttribute() || NULL != pPanelSet->FirstChild())
	{
		root->LinkEndChild(pPanelSet);
	}
	else
	{
		delete pPanelSet;
		pPanelSet = NULL;
	}

	return true;
}

void PanelSetMgr::ClearCfg()
{
	m_vecPanelInfo.clear();
}

void PanelSetMgr::CalcUsableShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for ( int i = 0; i < (int)m_vecPanelInfo.size(); i++ )
	{
		pShortCutKeyMgr->DelUsableShortCutKey(m_vecPanelInfo[i].ShortKey);
	}
}

void PanelSetMgr::ProcessDulplicateShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr, bool bModify )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for (int i = 0; i < (int)m_vecPanelInfo.size(); i++)
	{
		if ( !bModify )
		{
			pShortCutKeyMgr->CalcDulplicateShortCutKey(m_vecPanelInfo[i].ShortKey);
		}
		else
		{
			if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_vecPanelInfo[i].ShortKey) )
			{
				strcpy(m_vecPanelInfo[i].ShortKey, ""); 
			}
		}
	}
}
