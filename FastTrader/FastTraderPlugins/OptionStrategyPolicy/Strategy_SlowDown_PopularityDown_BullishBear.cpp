#include "StdAfx.h"
#include "Strategy_SlowDown_PopularityDown_BullishBear.h"

CStrategy_SlowDown_PopularityDown_BullishBear::CStrategy_SlowDown_PopularityDown_BullishBear(void)
{
}

CStrategy_SlowDown_PopularityDown_BullishBear::~CStrategy_SlowDown_PopularityDown_BullishBear(void)
{
}

CStrategy_SlowDown_PopularityDown_BullishBear* CStrategy_SlowDown_PopularityDown_BullishBear::m_pInstance = NULL;
CStrategy_SlowDown_PopularityDown_BullishBear* CStrategy_SlowDown_PopularityDown_BullishBear::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_SlowDown_PopularityDown_BullishBear();

    return m_pInstance;

}
void CStrategy_SlowDown_PopularityDown_BullishBear::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_SlowDown_PopularityDown_BullishBear::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}