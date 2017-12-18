#include "StdAfx.h"
#include "Strategy_WideShake_PopularityDown_SellStrangle.h"

CStrategy_WideShake_PopularityDown_SellStrangle::CStrategy_WideShake_PopularityDown_SellStrangle(void)
{
}

CStrategy_WideShake_PopularityDown_SellStrangle::~CStrategy_WideShake_PopularityDown_SellStrangle(void)
{
}

CStrategy_WideShake_PopularityDown_SellStrangle* CStrategy_WideShake_PopularityDown_SellStrangle::m_pInstance = NULL;
CStrategy_WideShake_PopularityDown_SellStrangle* CStrategy_WideShake_PopularityDown_SellStrangle::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_WideShake_PopularityDown_SellStrangle();

    return m_pInstance;

}
void CStrategy_WideShake_PopularityDown_SellStrangle::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_WideShake_PopularityDown_SellStrangle::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}