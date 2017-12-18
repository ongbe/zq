#include "StdAfx.h"
#include "Strategy_SlowUp_PopularityDown_ArbitrageBearish.h"

CStrategy_SlowUp_PopularityDown_ArbitrageBearish::CStrategy_SlowUp_PopularityDown_ArbitrageBearish(void)
{
}

CStrategy_SlowUp_PopularityDown_ArbitrageBearish::~CStrategy_SlowUp_PopularityDown_ArbitrageBearish(void)
{
}

CStrategy_SlowUp_PopularityDown_ArbitrageBearish* CStrategy_SlowUp_PopularityDown_ArbitrageBearish::m_pInstance = NULL;
CStrategy_SlowUp_PopularityDown_ArbitrageBearish* CStrategy_SlowUp_PopularityDown_ArbitrageBearish::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_SlowUp_PopularityDown_ArbitrageBearish();

    return m_pInstance;

}
void CStrategy_SlowUp_PopularityDown_ArbitrageBearish::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_SlowUp_PopularityDown_ArbitrageBearish::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}