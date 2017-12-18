#pragma once 

#include "CfgGlobal.h"
#include <vector>

class NumberShortKeyMgr: public BaseCfgMgr
{
public:
	NumberShortKeyMgr(void);
	virtual ~NumberShortKeyMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	std::vector< NumberShortKey > m_NumberShortKeyInfo;
};