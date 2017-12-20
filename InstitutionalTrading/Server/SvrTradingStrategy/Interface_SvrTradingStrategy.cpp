// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#include "Interface_SvrTradingStrategy.h"
#include "SvrTradingStrategyBusiness.h"


CInterface_SvrTradingStrategy& CInterface_SvrTradingStrategy::getObj(void)
{
    return (CInterface_SvrTradingStrategy&)CSvrTradingStrategyBusiness::getObj();
}