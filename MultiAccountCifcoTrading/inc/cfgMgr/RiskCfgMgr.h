#pragma once


#include "CfgGlobal.h"

class RiskCfgMgr: public BaseCfgMgr
{
public:
	RiskCfgMgr(void);
	~RiskCfgMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	int GetSingleMaxVolume(){return m_RiskMgrCfg.SingleMaxVolume;}
	int GetClearOrderPanelMode(){return m_RiskMgrCfg.ClearOrderPanelMode;}

	void SetSingleMaxVolume(int nValue){m_RiskMgrCfg.SingleMaxVolume = nValue;}
	void SetClearOrderPanelMode(int nValue){m_RiskMgrCfg.ClearOrderPanelMode = nValue;}

	LPRISK_MGR_CFG GetRiskMgrCfg(){return &m_RiskMgrCfg;}

private:
	RISK_MGR_CFG m_RiskMgrCfg;
};
