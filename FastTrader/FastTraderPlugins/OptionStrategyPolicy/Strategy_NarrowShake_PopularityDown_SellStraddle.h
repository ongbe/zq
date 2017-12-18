#pragma once
#include "optionstrategybase.h"

class CStrategy_NarrowShake_PopularityDown_SellStraddle :
    public COptionStrategyBase
{
public:
    CStrategy_NarrowShake_PopularityDown_SellStraddle(void);
    ~CStrategy_NarrowShake_PopularityDown_SellStraddle(void);

public:
    static CStrategy_NarrowShake_PopularityDown_SellStraddle* m_pInstance;
    static CStrategy_NarrowShake_PopularityDown_SellStraddle* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
