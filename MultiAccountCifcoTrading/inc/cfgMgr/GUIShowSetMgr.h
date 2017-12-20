#pragma once

#include "CfgGlobal.h"

class GUIShowSetMgr: public BaseCfgMgr
{
public:
	GUIShowSetMgr(void);
	~GUIShowSetMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	int GetPosX(){return m_nPosX;}
	int GetPosY(){return m_nPosY;}
	void SetPos(int nPosX, int nPosY);
	int GetWidth(){return m_nWidth;}
	int GetHeight(){return m_nHeight;}
	void SetSize(int nWidth, int nHeight);
	int GetStatus(){return m_nStatus;}
	void SetStatus(int nStatus);

private:
	zq_int m_nPosX;
	zq_int m_nPosY;
	zq_int m_nWidth;
	zq_int m_nHeight;
	zq_int m_nStatus;
};
