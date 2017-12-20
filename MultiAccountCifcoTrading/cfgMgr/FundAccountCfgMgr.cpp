#include "StdAfx.h"

#include "Module-Misc/globalDefines.h"
#include "Module-Misc/GLobalFunc.h"
#include "FundAccountCfgMgr.h"

extern long ColorStr2Long(const char* pColTextColor);

FundAccountCfgMgr::FundAccountCfgMgr(void)
{
	memset(&m_fundAccountCfg, 0, sizeof(FUND_ACCOUNT_CFG));

	m_fundAccountCfg.BgColor.xmlset(1120416);
	m_fundAccountCfg.TextColor.xmlset(16777215);
}

FundAccountCfgMgr::~FundAccountCfgMgr(void)
{
}

bool FundAccountCfgMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

    TiXmlElement *pFundAccountItem = root->FirstChildElement("FundAccount");

	if(pFundAccountItem == NULL)
		return false;

	TiXmlElement* pItem = pFundAccountItem->FirstChildElement("TextColor");
	if(pItem && pItem->GetText())
	{
		m_fundAccountCfg.TextColor.xmlset(ColorStr2Long(pItem->GetText()), bFromUserCfg);
	}

	pItem = pFundAccountItem->FirstChildElement("BgColor");
	if(pItem && pItem->GetText())
	{
		m_fundAccountCfg.BgColor.xmlset(ColorStr2Long(pItem->GetText()), bFromUserCfg);
	}

	return true;
}

bool FundAccountCfgMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pFundAccountItem = root->FirstChildElement("FundAccount");
	if(pFundAccountItem)
		root->RemoveChild(pFundAccountItem);

	pFundAccountItem = new TiXmlElement("FundAccount");

	TiXmlElement *pChildItem=NULL;
	std::string strContent;
	TiXmlText *pContent =NULL;

	if ( m_fundAccountCfg.TextColor.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("TextColor");
		pFundAccountItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_fundAccountCfg.TextColor.operator long());
		pContent = new TiXmlText(strContent.c_str());
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_fundAccountCfg.BgColor.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("BgColor");
		pFundAccountItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_fundAccountCfg.BgColor.operator long());
		pContent = new TiXmlText(strContent.c_str());
		pChildItem->LinkEndChild(pContent);
	}

	if ( NULL != pFundAccountItem->FirstAttribute() || NULL != pFundAccountItem->FirstChild())
	{
		root->LinkEndChild(pFundAccountItem);
	}
	else
	{
		delete pFundAccountItem;
		pFundAccountItem = NULL;
	}

	return true;
}

void FundAccountCfgMgr::ClearCfg()
{
	memset(&m_fundAccountCfg, 0, sizeof(FUND_ACCOUNT_CFG));
}

void FundAccountCfgMgr::SetFundAccountCfg( const FUND_ACCOUNT_CFG& fundAccountCfg )
{
	m_fundAccountCfg = fundAccountCfg;
}
