#pragma once

#include "CfgGlobal.h"

class ReplacePositionMgr : public BaseCfgMgr
{
public:
	ReplacePositionMgr(void);
	~ReplacePositionMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	REPLACEPOSITION GetReplacePositionCfg();
	void SetReplacePositionCfg( const REPLACEPOSITION& replacePosition );

private:
	REPLACEPOSITION m_stReplacePosition;
};
