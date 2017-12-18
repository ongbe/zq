#include "StdAfx.h"
#include "Strategy_SteepUp_PopularityUp_BuyBearish.h"

CStrategy_SteepUp_PopularityUp_BuyBearish::CStrategy_SteepUp_PopularityUp_BuyBearish(void)
{
}

CStrategy_SteepUp_PopularityUp_BuyBearish::~CStrategy_SteepUp_PopularityUp_BuyBearish(void)
{
}

CStrategy_SteepUp_PopularityUp_BuyBearish* CStrategy_SteepUp_PopularityUp_BuyBearish::m_pInstance = NULL;
CStrategy_SteepUp_PopularityUp_BuyBearish* CStrategy_SteepUp_PopularityUp_BuyBearish::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_SteepUp_PopularityUp_BuyBearish();

    return m_pInstance;

}
void CStrategy_SteepUp_PopularityUp_BuyBearish::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_SteepUp_PopularityUp_BuyBearish::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}