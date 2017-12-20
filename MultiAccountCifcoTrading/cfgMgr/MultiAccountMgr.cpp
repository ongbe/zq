#include "StdAfx.h"
#include <string>

#include "Module-Misc/globalDefines.h"
#include "Module-Misc/GLobalFunc.h"
#include "cfgmgr/MultiAccountMgr.h"

MultiAccountMgr::MultiAccountMgr(void)
{
	memset(&m_multiAccount, 0, sizeof(m_multiAccount));

	m_multiAccount.nSaveType.xmlset(0);
	m_multiAccount.nLoadType.xmlset(0);
	m_multiAccount.nSaveTypeManual.xmlset(0);
	m_multiAccount.nLoadTypeManual.xmlset(0);
	m_multiAccount.nStrategyItem.xmlset(0);
	m_multiAccount.nFirstAccountAlign.xmlset(0);
	m_multiAccount.nAccountScaleDivided.xmlset(0);

	m_vBaseMultipliedWayContracts.clear();
	m_vBaseMultipliedWayData.clear();

	m_vUserDefinedVolumnWayContracts.clear();
	m_vUserDefinedVolumnWayData.clear();

	m_vSumDividedWayContracts.clear();
	m_vSumDividedWayData.clear();
}

MultiAccountMgr::~MultiAccountMgr(void)
{
}

bool MultiAccountMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{   
    if(root==NULL)
        return false;

    TiXmlElement *pMAMgrItem = root->FirstChildElement("MultiAccount");
	if(pMAMgrItem == NULL)
		return false;

	const char *pElementValue = NULL;
    TiXmlElement *pItem = pMAMgrItem->FirstChildElement("SaveType");
	if(pItem) 
    {
	    pElementValue = pItem->GetText();
	    if(pElementValue)
        {
	        m_multiAccount.nSaveType.xmlset(atoi(pElementValue), bFromUserCfg);
        }
    }

	pItem = pMAMgrItem->FirstChildElement("LoadType");
	if(pItem)
    {
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_multiAccount.nLoadType.xmlset(atoi(pElementValue), bFromUserCfg);
		}
    }

	pItem = pMAMgrItem->FirstChildElement("SaveTypeManual");
	if(pItem) 
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_multiAccount.nSaveTypeManual.xmlset(atoi(pElementValue), bFromUserCfg);
		}
	}

	pItem = pMAMgrItem->FirstChildElement("LoadTypeManual");
	if(pItem)
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_multiAccount.nLoadTypeManual.xmlset(atoi(pElementValue), bFromUserCfg);
		}
	}

	pItem = pMAMgrItem->FirstChildElement("StrategyItem");
	if(pItem)
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_multiAccount.nStrategyItem.xmlset(atoi(pElementValue), bFromUserCfg);
		}
	}

	pItem = pMAMgrItem->FirstChildElement("FirstAccountAlign");
	if(pItem)
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_multiAccount.nFirstAccountAlign.xmlset(atoi(pElementValue), bFromUserCfg);
		}
	}

	pItem = pMAMgrItem->FirstChildElement("AccountScaleDivided");
	if(pItem)
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_multiAccount.nAccountScaleDivided.xmlset(atoi(pElementValue), bFromUserCfg);
		}
	}

	InitOrderStrategy(BaseMultipliedWay, pMAMgrItem, bFromUserCfg);
	InitOrderStrategy(UserDefinedVolumnWay, pMAMgrItem, bFromUserCfg);
	InitOrderStrategy(SumDividedWay, pMAMgrItem, bFromUserCfg);

	return true;
}

bool MultiAccountMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pMAMgrItem = root->FirstChildElement("MultiAccount");
	if(pMAMgrItem)
		root->RemoveChild(pMAMgrItem);

	pMAMgrItem = new TiXmlElement("MultiAccount");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;
	if ( m_multiAccount.nSaveType.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("SaveType");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nSaveType.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_multiAccount.nLoadType.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("LoadType");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nLoadType.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_multiAccount.nSaveTypeManual.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("SaveTypeManual");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nSaveTypeManual.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_multiAccount.nLoadTypeManual.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("LoadTypeManual");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nLoadTypeManual.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_multiAccount.nStrategyItem.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("StrategyItem");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nStrategyItem.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_multiAccount.nFirstAccountAlign.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("FirstAccountAlign");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nFirstAccountAlign.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_multiAccount.nAccountScaleDivided.needWriteToUserCfg())
	{	
		pChildItem = new TiXmlElement("AccountScaleDivided");
		pMAMgrItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_multiAccount.nAccountScaleDivided.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	SaveOrderStrategy(BaseMultipliedWay, pMAMgrItem);
	SaveOrderStrategy(UserDefinedVolumnWay, pMAMgrItem);
	SaveOrderStrategy(SumDividedWay, pMAMgrItem);

	if ( NULL != pMAMgrItem->FirstAttribute() || NULL != pMAMgrItem->FirstChild())
	{
		root->LinkEndChild(pMAMgrItem);
	}
	else
	{
		delete pMAMgrItem;
		pMAMgrItem = NULL;
	}

	return true;
}

void MultiAccountMgr::ClearCfg()
{
	memset(&m_multiAccount, 0, sizeof(m_multiAccount));

	m_vBaseMultipliedWayContracts.clear();
	m_vBaseMultipliedWayData.clear();

	m_vUserDefinedVolumnWayContracts.clear();
	m_vUserDefinedVolumnWayData.clear();

	m_vSumDividedWayContracts.clear();
	m_vSumDividedWayData.clear();
}

MULTIACCOUNT* MultiAccountMgr::GetMultiAccountSet()
{
	return &m_multiAccount;
}

void MultiAccountMgr::GetContracts( OrderStrategy eStrategy, 
									std::vector<std::string>& vContracts)
{
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		vContracts = m_vBaseMultipliedWayContracts;
		break;
	case UserDefinedVolumnWay:
		vContracts = m_vUserDefinedVolumnWayContracts;
		break;
	case SumDividedWay:
		vContracts = m_vSumDividedWayContracts;
		break;
	default:
		break;
	}
}

bool MultiAccountMgr::GetOrderStrategyData( OrderStrategy eStrategy, 
										   const std::string strAccount, 
										   OrderStrategyRowCfg& rowCfg )
{
	std::vector<OrderStrategyRowCfg> vec;
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		vec = m_vBaseMultipliedWayData;
		break;
	case UserDefinedVolumnWay:
		vec = m_vUserDefinedVolumnWayData;
		break;
	case SumDividedWay:
		vec = m_vSumDividedWayData;
		break;
	default:
		return false;
	}

	for ( UINT i = 0; i < vec.size(); i++ )
	{
		if ( strAccount == vec[i].GetAccount())
		{
			rowCfg = vec[i];
			return true;
		}
	}

	return false;
}


void MultiAccountMgr::SetOrderStrategyData( OrderStrategy eStrategy, 
								   const std::vector<std::string>& vContracts,
								   const std::vector<OrderStrategyRowCfg>& vStrategyData )
{
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		m_vBaseMultipliedWayContracts = vContracts;
		m_vBaseMultipliedWayData = vStrategyData;
		break;
	case UserDefinedVolumnWay:
		m_vUserDefinedVolumnWayContracts = vContracts;
		m_vUserDefinedVolumnWayData = vStrategyData;
		break;
	case SumDividedWay:
		 m_vSumDividedWayContracts = vContracts;
		 m_vSumDividedWayData = vStrategyData;
		break;
	default:
		break;
	}
}

void MultiAccountMgr::InitOrderStrategy( OrderStrategy eStrategy, TiXmlElement* pMAMgrItem, bool /*bFromUserCfg*/ )
{
	if ( NULL == pMAMgrItem )
	{
		return;
	}

	std::string strLabel;
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		strLabel = "BaseMultipliedWay";
		break;
	case UserDefinedVolumnWay:
		strLabel = "UserDefinedVolumnWay";
		break;
	case SumDividedWay:
		strLabel = "SumDividedWay";
		break;
	default:
		return;
	}

	std::vector<std::string> vContracts;
	std::vector<OrderStrategyRowCfg> vData;

	TiXmlElement* pItem = pMAMgrItem->FirstChildElement(strLabel);
	if ( pItem )
	{
		TiXmlElement* pContractList = pItem->FirstChildElement("ContractList");
		if ( pContractList && pContractList->GetText())
		{
			std::string strContractList = pContractList->GetText();
			ParseContractList(strContractList, vContracts);
		}

		TiXmlElement* pAccountList = pItem->FirstChildElement("AccountList");
		if ( pAccountList )
		{
			for(TiXmlElement *pAccount = pAccountList->FirstChildElement("Account"); 
				pAccount; 
				pAccount = pAccount->NextSiblingElement("Account"))
			{
				if ( NULL == pAccount->Attribute("account") || NULL == pAccount->Attribute("defaultValue"))
				{
					continue;
				}

				std::string strAccount = pAccount->Attribute("account");
				int nDefaultValue = atoi(pAccount->Attribute("defaultValue"));
				OrderStrategyRowCfg rowCfg(strAccount, nDefaultValue);

				for(TiXmlElement *pContract = pAccount->FirstChildElement("Contract"); 
					pContract; 
					pContract = pContract->NextSiblingElement("Contract"))
				{

					if ( NULL == pContract->Attribute("name") || NULL == pContract->Attribute("volume"))
					{
						continue;
					}

					std::string strContractName = pContract->Attribute("name");
					int nContractVolumn = atoi(pContract->Attribute("volume"));
					rowCfg.AddContractVolumn(strContractName, nContractVolumn);
				}

				vData.push_back(rowCfg);
			}
		}

		switch(eStrategy)
		{
		case BaseMultipliedWay:
			m_vBaseMultipliedWayContracts = vContracts;
			m_vBaseMultipliedWayData = vData;
			break;
		case UserDefinedVolumnWay:
			m_vUserDefinedVolumnWayContracts = vContracts;
			m_vUserDefinedVolumnWayData = vData;
			break;
		case SumDividedWay:
			m_vSumDividedWayContracts = vContracts;
			m_vSumDividedWayData = vData;
			break;
		default:
			return;
		}
	}
}

void MultiAccountMgr::SaveOrderStrategy( OrderStrategy eStrategy, TiXmlElement* pMAMgrItem )
{
	if ( NULL == pMAMgrItem )
	{
		return;
	}

	std::string strLabel;
	std::vector<std::string> vContracts;
	std::vector<OrderStrategyRowCfg> vData;
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		strLabel = "BaseMultipliedWay";
		vContracts = m_vBaseMultipliedWayContracts;
		vData = m_vBaseMultipliedWayData;
		break;
	case UserDefinedVolumnWay:
		strLabel = "UserDefinedVolumnWay";
		vContracts = m_vUserDefinedVolumnWayContracts;
		vData = m_vUserDefinedVolumnWayData;
		break;
	case SumDividedWay:
		strLabel = "SumDividedWay";
		vContracts = m_vSumDividedWayContracts;
		vData = m_vSumDividedWayData;
		break;
	default:
		return;
	}

	TiXmlElement* pOrderStrategy = new TiXmlElement(strLabel);
	pMAMgrItem->LinkEndChild(pOrderStrategy);

	TiXmlElement* pContractList = new TiXmlElement("ContractList");
	pOrderStrategy->LinkEndChild(pContractList);
	pContractList->LinkEndChild(new TiXmlText(GetContractList(vContracts)));

	TiXmlElement* pAccountList = new TiXmlElement("AccountList");
	pOrderStrategy->LinkEndChild(pAccountList);

	for ( UINT i = 0; i < vData.size(); i++ )
	{
		OrderStrategyRowCfg rowCfg = vData[i];
		TiXmlElement* pAccount = new TiXmlElement("Account");
		pAccountList->LinkEndChild(pAccount);

		pAccount->SetAttribute("account", rowCfg.GetAccount());
		pAccount->SetAttribute("defaultValue", rowCfg.GetDefaultValue());

		std::map<std::string, int> mapContract = rowCfg.GetContractVolumeMap();
		std::map<std::string, int>::iterator it = mapContract.begin();
		for ( ; it != mapContract.end(); it++ )
		{
			TiXmlElement* pContract = new TiXmlElement("Contract");
			pAccount->LinkEndChild(pContract);

			pContract->SetAttribute("name", it->first);
			pContract->SetAttribute("volume", it->second);
		}
	}
}

void MultiAccountMgr::ParseContractList(const std::string strContract, std::vector<std::string>& vec)
{
	if ( strContract.empty() )
	{
		return;
	}

	std::string strTemp = strContract;
	int nPos = strTemp.find(',');
	while ( nPos != -1 )
	{
		vec.push_back(strTemp.substr(0, nPos));
		strTemp = strTemp.substr(nPos+1, strTemp.length()-nPos-1);
		nPos = strTemp.find(',');
	}

	vec.push_back(strTemp);
}

std::string MultiAccountMgr::GetContractList( std::vector<std::string>& vec )
{
	std::string strTemp;
	for ( UINT i = 0; i < vec.size(); i++ )
	{
		if ( i != 0 )
		{
			strTemp += ",";
		}

		strTemp += vec[i];
	}

	return strTemp;
}

int MultiAccountMgr::GetOrderStrategyContractVolume( OrderStrategy eStrategy, const std::string strAccount, 
													const std::string strContract, const std::string strContractType )
{
	std::vector<OrderStrategyRowCfg> vec;
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		vec = m_vBaseMultipliedWayData;
		break;
	case UserDefinedVolumnWay:
		vec = m_vUserDefinedVolumnWayData;
		break;
	case SumDividedWay:
		vec = m_vSumDividedWayData;
		break;
	default:
		break;
	}

	for ( UINT i = 0; i < vec.size(); i++ )
	{
		if ( vec[i].GetAccount() == strAccount )
		{
			return vec[i].GetContractValue(strContract, strContractType);
		}
	}

	return 1;
}
