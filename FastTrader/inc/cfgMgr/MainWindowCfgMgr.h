#pragma once

#include "CfgGlobal.h"

class MainWindowCfgMgr: public BaseCfgMgr
{
public:
	MainWindowCfgMgr(void);
	virtual ~MainWindowCfgMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	LPMAIN_WINDOW_CFG GetMainWindowCfg(){return &m_MainWindowCfg;};
private:
	MAIN_WINDOW_CFG m_MainWindowCfg;
};
