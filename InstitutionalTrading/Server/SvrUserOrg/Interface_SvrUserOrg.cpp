// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "SvrUserOrgImpl.h"
#include "Interface_SvrUserOrg.h"

CInterface_SvrUserOrg& CInterface_SvrUserOrg::getObj(void)
{
	return (CInterface_SvrUserOrg&)CSvrUserOrgImpl::getObj();
}
