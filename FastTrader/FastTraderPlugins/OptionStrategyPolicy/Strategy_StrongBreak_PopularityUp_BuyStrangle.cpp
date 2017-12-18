#include "StdAfx.h"
#include "Strategy_StrongBreak_PopularityUp_BuyStrangle.h"

CStrategy_StrongBreak_PopularityUp_BuyStrangle::CStrategy_StrongBreak_PopularityUp_BuyStrangle(void)
{
}

CStrategy_StrongBreak_PopularityUp_BuyStrangle::~CStrategy_StrongBreak_PopularityUp_BuyStrangle(void)
{
}

CStrategy_StrongBreak_PopularityUp_BuyStrangle* CStrategy_StrongBreak_PopularityUp_BuyStrangle::m_pInstance = NULL;
CStrategy_StrongBreak_PopularityUp_BuyStrangle* CStrategy_StrongBreak_PopularityUp_BuyStrangle::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_StrongBreak_PopularityUp_BuyStrangle();

    return m_pInstance;

}
void CStrategy_StrongBreak_PopularityUp_BuyStrangle::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_StrongBreak_PopularityUp_BuyStrangle::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}