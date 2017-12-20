// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#include "SvrDBOprImpl.h"
#include "Interface_SvrDBOpr.h"


CInterface_SvrDBOpr& CInterface_SvrDBOpr::getObj(void)
{
	return (CInterface_SvrDBOpr&)CSvrDBOprImpl::getObj();
}