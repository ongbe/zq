// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//
#include "Interface_SvrFinanceProduct.h"
#include "SvrFinanceProductBusiness.h"


CInterface_SvrFinanceProduct& CInterface_SvrFinanceProduct::getObj(void)
{
    return (CInterface_SvrFinanceProduct&)CSvrFinanceProductBusiness::getObj();
}