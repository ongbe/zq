#pragma once 

#include "CfgGlobal.h"
#include <vector>

class ShortCutKeysMgr;

class VarietyMgr: public BaseCfgMgr
{
public:
	VarietyMgr(void);
	virtual ~VarietyMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	void CalcUsableShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr);
	void ProcessDulplicateShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr, bool bModify);

	std::vector<VarietyInfo> m_VarietyInfo;
};