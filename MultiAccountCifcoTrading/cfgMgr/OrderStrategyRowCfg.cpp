#include "StdAfx.h"
#include "OrderStrategyRowCfg.h"

OrderStrategyRowCfg::OrderStrategyRowCfg(std::string strAccount, int nDefaultValue)
: m_strAccount(strAccount)
, m_nDefaultValue(nDefaultValue)
{
	m_mapContractVolume.clear();
}

OrderStrategyRowCfg::OrderStrategyRowCfg()
: m_strAccount("")
, m_nDefaultValue(1)
{
	m_mapContractVolume.clear();
}

OrderStrategyRowCfg::~OrderStrategyRowCfg(void)
{
}

void OrderStrategyRowCfg::AddContractVolumn( std::string strContract, int nVolumn )
{
	m_mapContractVolume[strContract] = nVolumn;
}

std::string OrderStrategyRowCfg::GetAccount()
{
	return m_strAccount;
}

int OrderStrategyRowCfg::GetDefaultValue()
{
	return m_nDefaultValue;
}

std::map<std::string, int> OrderStrategyRowCfg::GetContractVolumeMap()
{
	return m_mapContractVolume;
}

int OrderStrategyRowCfg::GetContractValue(std::string strContract)
{
	std::map<std::string, int>::iterator it = m_mapContractVolume.find(strContract);
	if ( it == m_mapContractVolume.end())
	{
		return m_nDefaultValue;
	}
	else
	{
		return it->second;
	}
}

int OrderStrategyRowCfg::GetContractValue( std::string strContract, std::string strContractType )
{
	std::map<std::string, int>::iterator it = m_mapContractVolume.find(strContract);
	if ( it == m_mapContractVolume.end())
	{
		it = m_mapContractVolume.find(strContractType);
		if ( it == m_mapContractVolume.end())
		{
			return m_nDefaultValue;
		}
		else
		{
			return it->second;
		}
	}
	else
	{
		return it->second;
	}
}
