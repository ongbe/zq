#include "StdAfx.h"
#include <wx/defs.h>
#include "cfgMgr/ShortCutKeysMgr.h"
#include "cfgmgr/VarietyMgr.h"

VarietyMgr::VarietyMgr(void)
{
	m_VarietyInfo.clear();
}

VarietyMgr::~VarietyMgr(void)
{
	m_VarietyInfo.clear();
}

bool VarietyMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{   
	if ( !bFromUserCfg )
	{
		return true;
	}

    if(root==NULL)
        return false;

    TiXmlElement* p = root->FirstChildElement("VarietyInfo");
	if( p == NULL )
		return false;

	const char* pp = NULL;
	VarietyInfo info;
	for(TiXmlElement *Item = p->FirstChildElement("item"); 
		Item; 
		Item = Item->NextSiblingElement("item"))
	{
		ZeroMemory(&info, sizeof(info));
		pp = Item->Attribute("Name");
		if( !pp)
			continue;
		else
			strncpy(info.InstruName, Item->Attribute("Name"),sizeof(info.InstruName)-1 );

		pp = Item->Attribute("Volume");
		if( !pp )
			info.volume = 1;
		else
			info.volume = atoi( Item->Attribute("Volume"));

		pp = Item->Attribute("ShortKey");
		if(pp)
			strncpy(info.ShortKey, Item->Attribute("ShortKey"), sizeof(info.ShortKey)-1);
     
		pp = Item->Attribute("profit");
		if(pp)
			strncpy(info.profit, Item->Attribute("profit"), sizeof(info.profit)-1);

		pp = Item->Attribute("loss");
		if(pp)
            strncpy(info.loss, Item->Attribute("loss"), sizeof(info.loss)-1);

		pp = Item->Attribute("reverse");
		if(pp)
			strncpy(info.reverse, Item->Attribute("reverse"), sizeof(info.reverse)-1 );
  
		m_VarietyInfo.push_back( info );
	}
	
	return true;
}

bool VarietyMgr::SaveCfg(TiXmlElement *root)
{ 
	TiXmlElement* pVarietyInfo = root->FirstChildElement("VarietyInfo");
	if( pVarietyInfo )
		root->RemoveChild( pVarietyInfo );

	pVarietyInfo = new TiXmlElement("VarietyInfo");

	for( int unsigned i=0; i<m_VarietyInfo.size(); ++i )
	{
		TiXmlElement *Item = new TiXmlElement("item");

		Item->SetAttribute( "Name", m_VarietyInfo[i].InstruName);
		Item->SetAttribute( "Volume", m_VarietyInfo[i].volume);
		Item->SetAttribute( "ShortKey", m_VarietyInfo[i].ShortKey);
		Item->SetAttribute("profit",m_VarietyInfo[i].profit);
		Item->SetAttribute("loss",m_VarietyInfo[i].loss);
		Item->SetAttribute("reverse",m_VarietyInfo[i].reverse);
     	pVarietyInfo->LinkEndChild( Item );
	}

	if ( NULL != pVarietyInfo->FirstAttribute() || NULL != pVarietyInfo->FirstChild())
	{
		root->LinkEndChild(pVarietyInfo);
	}
	else
	{
		delete pVarietyInfo;
		pVarietyInfo = NULL;
	}

	return true;
}

void VarietyMgr::ClearCfg()
{
	m_VarietyInfo.clear();
}

void VarietyMgr::CalcUsableShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for (int i = 0; i < (int)m_VarietyInfo.size(); i++)
	{
		pShortCutKeyMgr->DelUsableShortCutKey(m_VarietyInfo[i].ShortKey);
	}
}

void VarietyMgr::ProcessDulplicateShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr, bool bModify )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for (int i = 0; i < (int)m_VarietyInfo.size(); i++)
	{
		if ( !bModify )
		{
			pShortCutKeyMgr->CalcDulplicateShortCutKey(m_VarietyInfo[i].ShortKey);
		}
		else
		{
			if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_VarietyInfo[i].ShortKey) )
			{
				strcpy(m_VarietyInfo[i].ShortKey, ""); 
			}
		}
	}
}
