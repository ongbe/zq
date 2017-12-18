#pragma once
#include "optionstrategybase.h"

class CStrategy_NoDown_PopularityDown_SellBearish :
    public COptionStrategyBase
{
public:
    CStrategy_NoDown_PopularityDown_SellBearish(void);
    ~CStrategy_NoDown_PopularityDown_SellBearish(void);

public:
    static CStrategy_NoDown_PopularityDown_SellBearish* m_pInstance;
    static CStrategy_NoDown_PopularityDown_SellBearish* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
