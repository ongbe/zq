// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "SvrQuotServer.h"
#include "Interface_SvrQuotServer.h"
#include "SvrQuotServerImpl.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrQuotServer& CInterface_SvrQuotServer::getObj(void)
{
    return (CInterface_SvrQuotServer&)CSvrQuotServerImpl::getObj();
}
