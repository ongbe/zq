#pragma once
#include "optionstrategybase.h"

class CStrategy_SlowUp_PopularityDown_ArbitrageBearish :
    public COptionStrategyBase
{
public:
    CStrategy_SlowUp_PopularityDown_ArbitrageBearish(void);
    ~CStrategy_SlowUp_PopularityDown_ArbitrageBearish(void);

public:
    static CStrategy_SlowUp_PopularityDown_ArbitrageBearish* m_pInstance;
    static CStrategy_SlowUp_PopularityDown_ArbitrageBearish* GetInstance();
    static void DeleteInstance();

public:
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
