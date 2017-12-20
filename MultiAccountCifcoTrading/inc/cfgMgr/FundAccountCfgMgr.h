#pragma once

#include "CfgGlobal.h"

class FundAccountCfgMgr: public BaseCfgMgr
{
public:
	FundAccountCfgMgr(void);
	virtual ~FundAccountCfgMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false);
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	FUND_ACCOUNT_CFG GetFundAccountCfg(){return m_fundAccountCfg;}
	void SetFundAccountCfg( const FUND_ACCOUNT_CFG& fundAccountCfg );

private:
	FUND_ACCOUNT_CFG m_fundAccountCfg;
};
