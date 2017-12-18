#include "StdAfx.h"
#include <string>

#include "Module-Misc2/globalDefines.h"
#include "Module-Misc2/GlobalFunc.h"
#include "cfgmgr/GUIShowSetMgr.h"

GUIShowSetMgr::GUIShowSetMgr(void)
{
	m_nPosX.xmlset(0);
	m_nPosY.xmlset(0);
	m_nWidth.xmlset(800);
	m_nHeight.xmlset(600);
	m_nStatus.xmlset(1);
}

GUIShowSetMgr::~GUIShowSetMgr(void)
{
}

bool GUIShowSetMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{   
    if(root==NULL)
        return false;

    TiXmlElement *pGUIShowSetMgrItem = root->FirstChildElement("GUIShowSet");
	if(pGUIShowSetMgrItem == NULL)
		return false;

	const char *pElementValue = NULL;
    TiXmlElement *pItem = pGUIShowSetMgrItem->FirstChildElement("status");
	if(pItem) 
    {
	    pElementValue = pItem->GetText();
	    if(pElementValue)
        {
	        m_nStatus.xmlset(atoi(pElementValue), bFromUserCfg);
        }
    }

	pItem = pGUIShowSetMgrItem->FirstChildElement("position");
	if(pItem)
    {
	    if(pItem->Attribute("x") && pItem->Attribute("y"))
        {
	        m_nPosX.xmlset(atoi(pItem->Attribute("x")), bFromUserCfg);
	        m_nPosY.xmlset(atoi(pItem->Attribute("y")), bFromUserCfg);
        }
    }

	pItem = pGUIShowSetMgrItem->FirstChildElement("size");
	if(pItem) 
    {
	    if(pItem->Attribute("width") && pItem->Attribute("height"))
        {
	        m_nWidth.xmlset(atoi(pItem->Attribute("width")), bFromUserCfg);
            m_nHeight.xmlset(atoi(pItem->Attribute("height")), bFromUserCfg);
        }
    }

	return true;

}

bool GUIShowSetMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pGUIShowSetMgrItem = root->FirstChildElement("GUIShowSet");
	if(pGUIShowSetMgrItem)
		root->RemoveChild(pGUIShowSetMgrItem);

	pGUIShowSetMgrItem = new TiXmlElement("GUIShowSet");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;
	if ( m_nStatus.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("status");
		pGUIShowSetMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_nStatus.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_nPosX.needWriteToUserCfg() || m_nPosY.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("position");
		pGUIShowSetMgrItem->LinkEndChild(pChildItem);
		pChildItem->SetAttribute("x", m_nPosX);
		pChildItem->SetAttribute("y", m_nPosY);
	}

	if ( m_nWidth.needWriteToUserCfg() || m_nHeight.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("size");
		pGUIShowSetMgrItem->LinkEndChild(pChildItem);
		pChildItem->SetAttribute("width", m_nWidth);
		pChildItem->SetAttribute("height", m_nHeight);
	}

	if ( NULL != pGUIShowSetMgrItem->FirstAttribute() || NULL != pGUIShowSetMgrItem->FirstChild())
	{
		root->LinkEndChild(pGUIShowSetMgrItem);
	}
	else
	{
		delete pGUIShowSetMgrItem;
		pGUIShowSetMgrItem = NULL;
	}

	return true;
}

void GUIShowSetMgr::ClearCfg()
{
}

void GUIShowSetMgr::SetPos( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void GUIShowSetMgr::SetSize( int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void GUIShowSetMgr::SetStatus( int nStatus )
{
	m_nStatus = nStatus;
}
