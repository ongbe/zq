#include "StdAfx.h"
#include "Strategy_NarrowShake_PopularityDown_SellStraddle.h"

CStrategy_NarrowShake_PopularityDown_SellStraddle::CStrategy_NarrowShake_PopularityDown_SellStraddle(void)
{
}

CStrategy_NarrowShake_PopularityDown_SellStraddle::~CStrategy_NarrowShake_PopularityDown_SellStraddle(void)
{
}

CStrategy_NarrowShake_PopularityDown_SellStraddle* CStrategy_NarrowShake_PopularityDown_SellStraddle::m_pInstance = NULL;
CStrategy_NarrowShake_PopularityDown_SellStraddle* CStrategy_NarrowShake_PopularityDown_SellStraddle::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_NarrowShake_PopularityDown_SellStraddle();

    return m_pInstance;

}
void CStrategy_NarrowShake_PopularityDown_SellStraddle::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_NarrowShake_PopularityDown_SellStraddle::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}