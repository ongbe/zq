#include "StdAfx.h"
#include "Strategy_NoUp_PopularityDown_SellBullish.h"

CStrategy_NoUp_PopularityDown_SellBullish::CStrategy_NoUp_PopularityDown_SellBullish(void)
{
}

CStrategy_NoUp_PopularityDown_SellBullish::~CStrategy_NoUp_PopularityDown_SellBullish(void)
{
}

CStrategy_NoUp_PopularityDown_SellBullish* CStrategy_NoUp_PopularityDown_SellBullish::m_pInstance = NULL;
CStrategy_NoUp_PopularityDown_SellBullish* CStrategy_NoUp_PopularityDown_SellBullish::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_NoUp_PopularityDown_SellBullish();

    return m_pInstance;

}
void CStrategy_NoUp_PopularityDown_SellBullish::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_NoUp_PopularityDown_SellBullish::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}