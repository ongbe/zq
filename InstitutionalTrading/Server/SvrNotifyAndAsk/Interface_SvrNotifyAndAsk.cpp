// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrNotifyAndAsk.h"
#include "CustomInterfaceSvrNotifyAndAsk.h"

CInterface_SvrNotifyAndAsk& CInterface_SvrNotifyAndAsk::getObj(void)
{
    return (CInterface_SvrNotifyAndAsk&)CCustomInterfaceSvrNotifyAndAsk::getObj();
}

