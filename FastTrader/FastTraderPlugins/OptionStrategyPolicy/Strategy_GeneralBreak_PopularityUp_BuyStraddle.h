#pragma once
#include "optionstrategybase.h"

class CStrategy_GeneralBreak_PopularityUp_BuyStraddle :
    public COptionStrategyBase
{
public:
    CStrategy_GeneralBreak_PopularityUp_BuyStraddle(void);
    ~CStrategy_GeneralBreak_PopularityUp_BuyStraddle(void);

public:
    static CStrategy_GeneralBreak_PopularityUp_BuyStraddle* m_pInstance;
    static CStrategy_GeneralBreak_PopularityUp_BuyStraddle* GetInstance();
    static void DeleteInstance();

public:
    void DestroyOptionStrategy();
    bool GetOptionStrategyOrders(vector<sOrder>& outOrders);
};
