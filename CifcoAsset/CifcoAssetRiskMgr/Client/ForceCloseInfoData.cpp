#include "StdAfx.h"
#include "ForceCloseInfoData.h"

CForceCloseInfoData* CForceCloseInfoData::m_pInstance=NULL;
CForceCloseInfoData::CForceCloseInfoData(void)
{
}
CForceCloseInfoData::~CForceCloseInfoData(void)
{
    if(m_pInstance != NULL)
        delete m_pInstance;
}

CForceCloseInfoData* CForceCloseInfoData::GetInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new CForceCloseInfoData();
    }

    return m_pInstance;   
}
void CForceCloseInfoData::DestroyInstance()
{
    if(m_pInstance == NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    } 
}
void CForceCloseInfoData::LockObject() 
{
    m_oMutex.lock();
}
void CForceCloseInfoData::UnlockObject() 
{
    m_oMutex.unlock();
}

bool CForceCloseInfoData::GetPositionFieldByInstrumentIDandDirect(std::string strInstrumentID,std::string nDirect)
{
    char charDirection;

    if(nDirect == "¾»")
        charDirection = '1';
    else if(nDirect == "Âò")
        charDirection = '2';
    else if(nDirect == "Âô")
        charDirection = '3';

    std::vector<SequencialPositionField>::iterator it;
    LockObject();
    for (it=vecPositionInfo.begin();it!=vecPositionInfo.end();it++)
    {
        if (strInstrumentID == it->InstrumentID && charDirection == it->PosiDirection)
        {
            curPositionField = *it;
            return true;
        }
    }
    UnlockObject();

    return false;
}
bool CForceCloseInfoData::GetDepthMarketDataFieldByInstrumentID(std::string strInstrumentID)
{
    std::vector<DepthMarketDataField>::iterator it;
    LockObject();
    for (it=vecDepthMarketDataInfo.begin();it!=vecDepthMarketDataInfo.end();it++)
    {
        if (strInstrumentID == it->InstrumentID)
        {
            curDepthMarketDataField = *it;
            return true;
        }
    }
    UnlockObject();

    return false;
}
