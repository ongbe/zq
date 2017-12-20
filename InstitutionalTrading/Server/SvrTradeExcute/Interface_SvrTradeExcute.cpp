// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "SvrTradeExcuteImpl.h"
#include "Interface_SvrTradeExcute.h"


CInterface_SvrTradeExcute& CInterface_SvrTradeExcute::getObj(void)
{
	return (CInterface_SvrTradeExcute&)CSvrTradeExcuteImpl::getObj();
}