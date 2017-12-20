#pragma once

#include "CfgGlobal.h"

class MovePositionMgr : public BaseCfgMgr
{
public:
	MovePositionMgr(void);
	~MovePositionMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	MOVEPOSITIONCFG GetMovePositionCfg();
	void SetMovePositionCfg( const MOVEPOSITIONCFG& stMovePosition );

private:
	MOVEPOSITIONCFG m_stMovePosition;
};
