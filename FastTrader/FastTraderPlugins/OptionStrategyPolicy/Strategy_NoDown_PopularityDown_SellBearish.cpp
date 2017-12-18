#include "StdAfx.h"
#include "Strategy_NoDown_PopularityDown_SellBearish.h"

CStrategy_NoDown_PopularityDown_SellBearish::CStrategy_NoDown_PopularityDown_SellBearish(void)
{
}

CStrategy_NoDown_PopularityDown_SellBearish::~CStrategy_NoDown_PopularityDown_SellBearish(void)
{
}

CStrategy_NoDown_PopularityDown_SellBearish* CStrategy_NoDown_PopularityDown_SellBearish::m_pInstance = NULL;
CStrategy_NoDown_PopularityDown_SellBearish* CStrategy_NoDown_PopularityDown_SellBearish::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_NoDown_PopularityDown_SellBearish();

    return m_pInstance;

}
void CStrategy_NoDown_PopularityDown_SellBearish::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_NoDown_PopularityDown_SellBearish::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}