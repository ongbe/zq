#include "StdAfx.h"
#include "../inc/Module-Misc/tinyxml.h"
#include "PlusinConfigHelper.h"
#include "Plusin.h"

PlusinConfigHelper::PlusinConfigHelper(void)
: m_strAppID("")
, m_bStartWhenRegist(false)
, m_bMultiInstance(false)
, m_bNeedCurrentInstrumentID(false)
, m_unMenuPos(0)
, m_strMenuName("")
, m_strSummary("")
, m_strDesc("")
{
}

PlusinConfigHelper::~PlusinConfigHelper(void)
{
}

bool PlusinConfigHelper::LoadConfig( const wxString& strPath )
{
	TiXmlDocument doc(strPath.c_str()); 
	if( doc.LoadFile())
	{
		TiXmlElement* root = doc.RootElement();
		if( root == NULL )
		{
			return false;
		}

		TiXmlElement *pElement = root->FirstChildElement("APPID");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_strAppID = pElement->GetText();
		}
		else
		{
			return false;
		}

		pElement = root->FirstChildElement("StartWhenRegist");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_bStartWhenRegist = atoi(pElement->GetText()) > 0 ? true : false;
		}
		else
		{
			return false;
		}

        m_bMultiInstance=false;
		pElement = root->FirstChildElement("MultiInstance");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_bMultiInstance = atoi(pElement->GetText()) == 1 ? true : false;
		}

        m_bNeedCurrentInstrumentID=false;
		pElement = root->FirstChildElement("NeedCurrentInstrumentID");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_bNeedCurrentInstrumentID = atoi(pElement->GetText()) == 1 ? true : false;
		}

		pElement = root->FirstChildElement("MenuPosition");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			std::string strMenuPos = pElement->GetText();
			ParseMenuPos(strMenuPos);
		}
		else
		{
			return false;
		}

		pElement = root->FirstChildElement("MenuName");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_strMenuName = pElement->GetText();
		}
		else
		{
			return false;
		}

		pElement = root->FirstChildElement("Summary");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_strSummary = pElement->GetText();
		}
		else
		{
			return false;
		}

		pElement = root->FirstChildElement("Description");
		if ( NULL != pElement && NULL != pElement->GetText())
		{
			m_strDesc = pElement->GetText();
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::string PlusinConfigHelper::GetAppID() const
{
	return m_strAppID;
}

UINT PlusinConfigHelper::GetMenuPos() const
{
	return m_unMenuPos;
}

std::string PlusinConfigHelper::GetMenuName() const
{
	return m_strMenuName;
}

std::string PlusinConfigHelper::GetSummary() const
{
	return m_strSummary;
}

std::string PlusinConfigHelper::GetDesc() const
{
	return m_strDesc;
}

void PlusinConfigHelper::ParseMenuPos( const std::string strMenuPos )
{
	if ( strMenuPos.empty())
	{
		return;
	}

	std::string strTemp = strMenuPos;
	int nPos = strTemp.find(",");
	while( nPos != -1 )
	{
		std::string strOneMenuPos = strTemp.substr(0, nPos);
		if ( strOneMenuPos == "None")
		{
			m_unMenuPos = MENU_POS_NONE;
			return;
		}
		else if ( strOneMenuPos == "MainMenu")
		{
			m_unMenuPos |= MENU_POS_MAIN_MENU;
		}
		else if ( strOneMenuPos == "Quote")
		{
			m_unMenuPos |= MENU_POS_QUOTE;
		}
		else
		{

		}

		strTemp = strTemp.substr(nPos+1, strTemp.length() - nPos);
		nPos = strTemp.find(",");
	}

	if ( strTemp == "None")
	{
		m_unMenuPos = MENU_POS_NONE;
		return;
	}
	else if ( strTemp == "MainMenu")
	{
		m_unMenuPos |= MENU_POS_MAIN_MENU;
	}
	else if ( strTemp == "Quote")
	{
		m_unMenuPos |= MENU_POS_QUOTE;
	}
	else
	{

	}
}

bool PlusinConfigHelper::NeedStartWhenRegist() const
{
	return m_bStartWhenRegist;
}
bool PlusinConfigHelper::CanMultiInstance() const
{
	return m_bMultiInstance;
}
bool PlusinConfigHelper::NeedCurrentInstrumentID() const
{
	return m_bNeedCurrentInstrumentID;
}
