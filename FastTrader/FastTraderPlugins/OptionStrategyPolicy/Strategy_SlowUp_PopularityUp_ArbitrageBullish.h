#pragma once
#include "optionstrategybase.h"

class CStrategy_SlowUp_PopularityUp_ArbitrageBullish :
    public COptionStrategyBase
{
public:
    CStrategy_SlowUp_PopularityUp_ArbitrageBullish(void);
    ~CStrategy_SlowUp_PopularityUp_ArbitrageBullish(void);

public:
    static CStrategy_SlowUp_PopularityUp_ArbitrageBullish* m_pInstance;
    static CStrategy_SlowUp_PopularityUp_ArbitrageBullish* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
