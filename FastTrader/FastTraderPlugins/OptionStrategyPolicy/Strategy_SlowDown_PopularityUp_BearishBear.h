#pragma once
#include "optionstrategybase.h"

class CStrategy_SlowDown_PopularityUp_BearishBear :
    public COptionStrategyBase
{
public:
    CStrategy_SlowDown_PopularityUp_BearishBear(void);
    ~CStrategy_SlowDown_PopularityUp_BearishBear(void);

public:
    static CStrategy_SlowDown_PopularityUp_BearishBear* m_pInstance;
    static CStrategy_SlowDown_PopularityUp_BearishBear* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
