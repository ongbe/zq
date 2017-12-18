#pragma once
#include "optionstrategybase.h"

class CStrategy_NoUp_PopularityDown_SellBullish :
    public COptionStrategyBase
{
public:
    CStrategy_NoUp_PopularityDown_SellBullish(void);
    ~CStrategy_NoUp_PopularityDown_SellBullish(void);

public:
    static CStrategy_NoUp_PopularityDown_SellBullish* m_pInstance;
    static CStrategy_NoUp_PopularityDown_SellBullish* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
