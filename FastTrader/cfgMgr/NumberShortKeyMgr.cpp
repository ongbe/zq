#include "StdAfx.h"
#include "NumberShortKeyMgr.h"

NumberShortKeyMgr::NumberShortKeyMgr(void)
{
	m_NumberShortKeyInfo.clear();
}

NumberShortKeyMgr::~NumberShortKeyMgr(void)
{
	m_NumberShortKeyInfo.clear();
}

bool NumberShortKeyMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
	if ( !bFromUserCfg )
	{
		return true;
	}

    if(root==NULL)
        return false;

	TiXmlElement* p = root->FirstChildElement("NumberShortKeyInfo");
	if( p == NULL )
		return false;

	NumberShortKey info;
	char* pItem = NULL;
	for(TiXmlElement *Item = p->FirstChildElement("item"); 
		Item; 
		Item = Item->NextSiblingElement("item"))
	{
        ZeroMemory(&info, sizeof(info));

		pItem = (char*)Item->Attribute("ShortKey");
		if( !pItem )
			continue;
		else
			strncpy(info.ShortKey, Item->Attribute("ShortKey"), sizeof(info.ShortKey)-1);

		pItem = (char*)Item->Attribute("Symbol");
		if( !pItem )
			continue;
		else
			strncpy(info.Symbol, Item->Attribute("Symbol"), sizeof(info.Symbol)-1);

		pItem = (char*)  ( Item->Attribute("Comment") );
		if(pItem)
			strncpy(info.Comment, Item->Attribute("Comment"), sizeof(info.Comment)-1 );

		m_NumberShortKeyInfo.push_back( info );
	}

	return true;
}

bool NumberShortKeyMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement* pNumberShortKey = root->FirstChildElement("NumberShortKeyInfo");
	if( pNumberShortKey )
		root->RemoveChild( pNumberShortKey );

	pNumberShortKey = new TiXmlElement("NumberShortKeyInfo");

	for( size_t i=0; i<m_NumberShortKeyInfo.size(); ++i )
	{
		TiXmlElement *pItem = new TiXmlElement("item");

		pItem->SetAttribute( "ShortKey", m_NumberShortKeyInfo[i].ShortKey);
		pItem->SetAttribute( "Symbol", m_NumberShortKeyInfo[i].Symbol);
		pItem->SetAttribute( "Comment",m_NumberShortKeyInfo[i].Comment);
		pNumberShortKey->LinkEndChild( pItem );
	}

	if ( NULL != pNumberShortKey->FirstAttribute() || NULL != pNumberShortKey->FirstChild())
	{
		root->LinkEndChild(pNumberShortKey);
	}
	else
	{
		delete pNumberShortKey;
		pNumberShortKey = NULL;
	}

	return true;
}

void NumberShortKeyMgr::ClearCfg()
{
	m_NumberShortKeyInfo.clear();
}
