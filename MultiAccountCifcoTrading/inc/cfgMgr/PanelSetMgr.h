
#pragma once 


#include "CfgGlobal.h"
#include <vector>

class TiXmlElement;
class ShortCutKeysMgr;

class PanelSetMgr: public BaseCfgMgr
{
public:
	PanelSetMgr(void);
	virtual ~PanelSetMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	void CalcUsableShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr);
	void ProcessDulplicateShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr, bool bModify);

public:
	std::vector<PanelInfo> m_vecPanelInfo;
};
