#include "StdAfx.h"
#include "Strategy_GeneralBreak_PopularityUp_BuyStraddle.h"

CStrategy_GeneralBreak_PopularityUp_BuyStraddle::CStrategy_GeneralBreak_PopularityUp_BuyStraddle(void)
{
}

CStrategy_GeneralBreak_PopularityUp_BuyStraddle::~CStrategy_GeneralBreak_PopularityUp_BuyStraddle(void)
{
}

CStrategy_GeneralBreak_PopularityUp_BuyStraddle* CStrategy_GeneralBreak_PopularityUp_BuyStraddle::m_pInstance = NULL;
CStrategy_GeneralBreak_PopularityUp_BuyStraddle* CStrategy_GeneralBreak_PopularityUp_BuyStraddle::GetInstance()
{
    if (NULL == m_pInstance)    
        m_pInstance = new CStrategy_GeneralBreak_PopularityUp_BuyStraddle();

    return m_pInstance;

}
void CStrategy_GeneralBreak_PopularityUp_BuyStraddle::DeleteInstance()
{
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
}

bool CStrategy_GeneralBreak_PopularityUp_BuyStraddle::GetOptionStrategyOrders(vector<sOrder>& outOrders)
{
    return false;
}
