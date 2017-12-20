// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrPresentation.h"
#include "SvrPresentation.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "RealSvrPresentation.h"
CInterface_SvrPresentation& CInterface_SvrPresentation::getObj(void)
{
  return (CInterface_SvrPresentation&)CRealSvrPresentation::getObj();
}

