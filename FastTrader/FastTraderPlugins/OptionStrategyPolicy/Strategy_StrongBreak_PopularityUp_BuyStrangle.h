#pragma once
#include "optionstrategybase.h"

class CStrategy_StrongBreak_PopularityUp_BuyStrangle :
    public COptionStrategyBase
{
public:
    CStrategy_StrongBreak_PopularityUp_BuyStrangle(void);
    ~CStrategy_StrongBreak_PopularityUp_BuyStrangle(void);

public:
    static CStrategy_StrongBreak_PopularityUp_BuyStrangle* m_pInstance;
    static CStrategy_StrongBreak_PopularityUp_BuyStrangle* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
