#pragma once

#include "CfgGlobal.h"
#include "OrderStrategyRowCfg.h"

class MultiAccountMgr: public BaseCfgMgr
{
public:
	MultiAccountMgr(void);
	~MultiAccountMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	MULTIACCOUNT* GetMultiAccountSet();

	void GetContracts( OrderStrategy eStrategy, std::vector<std::string>& vContracts);
	bool GetOrderStrategyData(OrderStrategy eStrategy, const std::string strAccount, OrderStrategyRowCfg& rowCfg );
	void SetOrderStrategyData(OrderStrategy eStrategy, const std::vector<std::string>& vContracts,
		const std::vector<OrderStrategyRowCfg>& vStrategyData);
	int GetOrderStrategyContractVolume(OrderStrategy eStrategy, const std::string strAccount,
		const std::string strContract, const std::string strContractType);

private:
	void InitOrderStrategy( OrderStrategy eStrategy, TiXmlElement* pMAMgrItem, bool bFromUserCfg );
	void SaveOrderStrategy( OrderStrategy eStrategy, TiXmlElement* pMAMgrItem);

	void ParseContractList( const std::string strContract, std::vector<std::string>& vec);
	std::string GetContractList( std::vector<std::string>& vec );

private:
	MULTIACCOUNT m_multiAccount;

	std::vector<std::string> m_vBaseMultipliedWayContracts;
	std::vector<OrderStrategyRowCfg> m_vBaseMultipliedWayData;

	std::vector<std::string> m_vUserDefinedVolumnWayContracts;
	std::vector<OrderStrategyRowCfg> m_vUserDefinedVolumnWayData;

	std::vector<std::string> m_vSumDividedWayContracts;
	std::vector<OrderStrategyRowCfg> m_vSumDividedWayData;
};
