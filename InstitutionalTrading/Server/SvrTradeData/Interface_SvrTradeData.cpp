// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrTradeData.h"
#include "Real_SvrTradeData.h"


CInterface_SvrTradeData& CInterface_SvrTradeData::getObj(void)
{
	return (CInterface_SvrTradeData&)CReal_SvrTradeData::getObj();
}
