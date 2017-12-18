#include "StdAfx.h"
#include "Strategy_SteepDown_PopularityUp_SellBearish.h"

CStrategy_SteepDown_PopularityUp_SellBearish::CStrategy_SteepDown_PopularityUp_SellBearish(void)
{
}

CStrategy_SteepDown_PopularityUp_SellBearish::~CStrategy_SteepDown_PopularityUp_SellBearish(void)
{
}

CStrategy_SteepDown_PopularityUp_SellBearish* CStrategy_SteepDown_PopularityUp_SellBearish::m_pInstance = NULL;
CStrategy_SteepDown_PopularityUp_SellBearish* CStrategy_SteepDown_PopularityUp_SellBearish::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_SteepDown_PopularityUp_SellBearish();

    return m_pInstance;

}
void CStrategy_SteepDown_PopularityUp_SellBearish::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_SteepDown_PopularityUp_SellBearish::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}