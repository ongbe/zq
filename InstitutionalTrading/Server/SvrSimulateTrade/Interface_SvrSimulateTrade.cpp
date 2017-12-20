// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrSimulateTrade.h"
#include "SvrSimulateTrade.h"
//#include "easymutex.h"
//#include "CommonPkg.h"
#include "Real_SvrSimulateTrade.h"


CReal_SvrSimulateTrade g_svrSimulateTrade;


CInterface_SvrSimulateTrade& CInterface_SvrSimulateTrade::getObj(void)
{
	return g_svrSimulateTrade;
}
