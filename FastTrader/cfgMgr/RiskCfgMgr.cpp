#include "StdAfx.h"
#include <string>

#include "Module-Misc2/globalDefines.h"
#include "Module-Misc2/GlobalFunc.h"
#include "cfgmgr/RiskCfgMgr.h"

RiskCfgMgr::RiskCfgMgr(void)
{
	memset(&m_RiskMgrCfg, 0, sizeof(m_RiskMgrCfg));
	m_RiskMgrCfg.ClearOrderPanelMode.xmlset(0);
	m_RiskMgrCfg.SingleMaxVolume.xmlset(0);
}

RiskCfgMgr::~RiskCfgMgr(void)
{
}

bool RiskCfgMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{    
    if(root==NULL)
        return false;

    TiXmlElement *pRiskMgrItem = root->FirstChildElement("RiskMgr");
	if(pRiskMgrItem == NULL)
		return false;

	const char *pElementValue = NULL;
	TiXmlElement *pItem = pRiskMgrItem->FirstChildElement("SingleMaxVolume");
	if(pItem && pItem->GetText()) 
	{
		m_RiskMgrCfg.SingleMaxVolume.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pRiskMgrItem->FirstChildElement("ClearOrderPanelMode");
	if(pItem && pItem->GetText()) 
	{
		m_RiskMgrCfg.ClearOrderPanelMode.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	return true;

}

bool RiskCfgMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pRiskMgrItem = root->FirstChildElement("RiskMgr");
	if(pRiskMgrItem)
		root->RemoveChild(pRiskMgrItem);

	pRiskMgrItem = new TiXmlElement("RiskMgr");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;
	
	if ( m_RiskMgrCfg.SingleMaxVolume.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SingleMaxVolume");
		pRiskMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_RiskMgrCfg.SingleMaxVolume.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_RiskMgrCfg.ClearOrderPanelMode.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClearOrderPanelMode");
		pRiskMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_RiskMgrCfg.ClearOrderPanelMode.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( NULL != pRiskMgrItem->FirstAttribute() || NULL != pRiskMgrItem->FirstChild())
	{
		root->LinkEndChild(pRiskMgrItem);
	}
	else
	{
		delete pRiskMgrItem;
		pRiskMgrItem = NULL;
	}

	return true;
}

void RiskCfgMgr::ClearCfg()
{
	ZeroMemory(&m_RiskMgrCfg, sizeof(m_RiskMgrCfg));
}
