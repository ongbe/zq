#include "StdAfx.h"
#include "Strategy_SlowDown_PopularityUp_BearishBear.h"

CStrategy_SlowDown_PopularityUp_BearishBear::CStrategy_SlowDown_PopularityUp_BearishBear(void)
{
}

CStrategy_SlowDown_PopularityUp_BearishBear::~CStrategy_SlowDown_PopularityUp_BearishBear(void)
{
}

CStrategy_SlowDown_PopularityUp_BearishBear* CStrategy_SlowDown_PopularityUp_BearishBear::m_pInstance = NULL;
CStrategy_SlowDown_PopularityUp_BearishBear* CStrategy_SlowDown_PopularityUp_BearishBear::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_SlowDown_PopularityUp_BearishBear();

    return m_pInstance;

}
void CStrategy_SlowDown_PopularityUp_BearishBear::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_SlowDown_PopularityUp_BearishBear::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}