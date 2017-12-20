#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "PlatformDataMgr.h"



class CTraderDataInfo
{
private:
	CTraderDataInfo();
	~CTraderDataInfo();

public:
	static CTraderDataInfo* NewInstance();
	static void DestroyInstance();
	static CTraderDataInfo* m_pInstance;

public:
	virtual void SetInstrumentInfo(const PlatformStru_InstrumentInfo & nInfo);
	virtual int GetALlInstrumentInfo(vector<PlatformStru_InstrumentInfo> & nVecInfo);


public:
	CPlatformDataMgr_Instruments mpInstrument;
};
