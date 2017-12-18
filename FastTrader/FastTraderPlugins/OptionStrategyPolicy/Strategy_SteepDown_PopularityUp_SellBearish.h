#pragma once
#include "optionstrategybase.h"

class CStrategy_SteepDown_PopularityUp_SellBearish :
    public COptionStrategyBase
{
public:
    CStrategy_SteepDown_PopularityUp_SellBearish(void);
    ~CStrategy_SteepDown_PopularityUp_SellBearish(void);

public:
    static CStrategy_SteepDown_PopularityUp_SellBearish* m_pInstance;
    static CStrategy_SteepDown_PopularityUp_SellBearish* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
