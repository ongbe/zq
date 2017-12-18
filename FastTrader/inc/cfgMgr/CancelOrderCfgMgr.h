#pragma once

#include "CfgGlobal.h"

class ShortCutKeysMgr;

class CancelOrderCfgMgr: public BaseCfgMgr
{
public:
	CancelOrderCfgMgr(void);
	virtual ~CancelOrderCfgMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	CANCEL_ORDER_CFG GetCancelOrderCfg(){return m_cancelOrderCfg;}
	void SetCancelOrderCfg( const CANCEL_ORDER_CFG& cancelOrderCfg );

	void CalcUsableShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr);
	void ProcessDulplicateShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr, bool bModify);

private:
	CANCEL_ORDER_CFG m_cancelOrderCfg;
};
