#include "StdAfx.h"
#include "Strategy_SlowUp_PopularityUp_ArbitrageBullish.h"

CStrategy_SlowUp_PopularityUp_ArbitrageBullish::CStrategy_SlowUp_PopularityUp_ArbitrageBullish(void)
{
}

CStrategy_SlowUp_PopularityUp_ArbitrageBullish::~CStrategy_SlowUp_PopularityUp_ArbitrageBullish(void)
{
}

CStrategy_SlowUp_PopularityUp_ArbitrageBullish* CStrategy_SlowUp_PopularityUp_ArbitrageBullish::m_pInstance = NULL;
CStrategy_SlowUp_PopularityUp_ArbitrageBullish* CStrategy_SlowUp_PopularityUp_ArbitrageBullish::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_SlowUp_PopularityUp_ArbitrageBullish();

    return m_pInstance;

}
void CStrategy_SlowUp_PopularityUp_ArbitrageBullish::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_SlowUp_PopularityUp_ArbitrageBullish::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}