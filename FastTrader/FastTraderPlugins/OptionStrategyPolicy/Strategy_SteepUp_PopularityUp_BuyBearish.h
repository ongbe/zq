#pragma once
#include "optionstrategybase.h"

class CStrategy_SteepUp_PopularityUp_BuyBearish :
    public COptionStrategyBase
{
public:
    CStrategy_SteepUp_PopularityUp_BuyBearish(void);
    ~CStrategy_SteepUp_PopularityUp_BuyBearish(void);

public:
    static CStrategy_SteepUp_PopularityUp_BuyBearish* m_pInstance;
    static CStrategy_SteepUp_PopularityUp_BuyBearish* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
