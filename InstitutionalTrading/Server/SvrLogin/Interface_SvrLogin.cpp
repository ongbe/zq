// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrLogin.h"
#include "CustomSvrLoginInterface.h"


CInterface_SvrLogin& CInterface_SvrLogin::getObj(void)
{
   return (CInterface_SvrLogin&)CCunstomSvrLoginInterface::getObj();
}


