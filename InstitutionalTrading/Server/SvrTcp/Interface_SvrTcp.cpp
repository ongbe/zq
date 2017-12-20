// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)




#include "SvrTcpBusiness.h"
#include "Interface_SvrTcp.h"



CInterface_SvrTcp& CInterface_SvrTcp::getObj(void)
{
	return (CInterface_SvrTcp&)CSvrTcpBusiness::getObj();
}

