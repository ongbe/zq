// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "SvrBrokerInfoImpl.h"



CInterface_SvrBrokerInfo& CInterface_SvrBrokerInfo::getObj(void)
{
	return (CInterface_SvrBrokerInfo&)CSvrBrokerInfoImpl::getObj();
}