#include "stdafx.h"
#include "CTraderDataInfo.h"
#include "CommonDef.h"

CTraderDataInfo* CTraderDataInfo::m_pInstance = NULL;
int gTraderInstanceCount = 0;
CTraderDataInfo* CTraderDataInfo::NewInstance()
{
	gTraderInstanceCount ++;
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CTraderDataInfo();
	}

	return m_pInstance;
}

void CTraderDataInfo::DestroyInstance()
{
	gTraderInstanceCount --;
	if(gTraderInstanceCount == 0 && m_pInstance != NULL) 
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CTraderDataInfo::CTraderDataInfo()
{
	
}

CTraderDataInfo::~CTraderDataInfo()
{
}

void CTraderDataInfo::SetInstrumentInfo(const PlatformStru_InstrumentInfo & nInfo)
{
	mpInstrument.SetInstrumentInfo(nInfo.InstrumentID,nInfo,false);
}
int CTraderDataInfo::GetALlInstrumentInfo(vector<PlatformStru_InstrumentInfo> & nVecInfo)
{
	std::set<string> lsSetString;
	mpInstrument.GetInstrumentList(lsSetString);
	std::set<string> ::iterator lIter = lsSetString.begin();
	for(;lIter != lsSetString.end();lIter++)
	{
		PlatformStru_InstrumentInfo ltemp;
		mpInstrument.GetInstrumentInfo(*lIter,ltemp);
		nVecInfo.push_back(ltemp);
		
	}

	return nVecInfo.size();

}