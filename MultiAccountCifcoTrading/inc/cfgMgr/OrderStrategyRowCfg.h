#pragma once

#include <map>

class OrderStrategyRowCfg
{
public:
	OrderStrategyRowCfg();
	OrderStrategyRowCfg(std::string strAccount, int nDefaultValue);
	~OrderStrategyRowCfg(void);

	void AddContractVolumn(std::string strContract, int nVolumn);
	std::string GetAccount();
	int GetDefaultValue();
	std::map<std::string, int> GetContractVolumeMap();

	int GetContractValue(std::string strContract);
	int GetContractValue(std::string strContract, std::string strContractType);

private:
	std::string m_strAccount;
	int m_nDefaultValue;
	std::map<std::string, int> m_mapContractVolume; 
};
