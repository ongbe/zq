// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrInstruments.h"
#include "CustomInterfaceSvrInstruments.h"
#include "SvrInstruments.h"
#include "easymutex.h"
#include "CommonPkg.h"


CInterface_SvrInstruments& CInterface_SvrInstruments::getObj(void)
{
   
	return (CInterface_SvrInstruments&)CCustomInterfaceSvrInstruments::getObj();
}
