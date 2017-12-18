#pragma once
#include "optionstrategybase.h"

class CStrategy_WideShake_PopularityDown_SellStrangle :
    public COptionStrategyBase
{
public:
    CStrategy_WideShake_PopularityDown_SellStrangle(void);
    ~CStrategy_WideShake_PopularityDown_SellStrangle(void);

public:
    static CStrategy_WideShake_PopularityDown_SellStrangle* m_pInstance;
    static CStrategy_WideShake_PopularityDown_SellStrangle* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
