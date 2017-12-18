#pragma once
#include "optionstrategybase.h"

class CStrategy_SlowDown_PopularityDown_BullishBear :
    public COptionStrategyBase
{
public:
    CStrategy_SlowDown_PopularityDown_BullishBear(void);
    ~CStrategy_SlowDown_PopularityDown_BullishBear(void);

public:
    static CStrategy_SlowDown_PopularityDown_BullishBear* m_pInstance;
    static CStrategy_SlowDown_PopularityDown_BullishBear* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
