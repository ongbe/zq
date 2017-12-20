// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrSettlement.h"
#include "SvrSettlement.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "SvrSettlementBusiness.h"


CInterface_SvrSettlement::CInterface_SvrSettlement()
{
}

CInterface_SvrSettlement::~CInterface_SvrSettlement()
{
}

CInterface_SvrSettlement& CInterface_SvrSettlement::getObj(void)
{
    //g_mutex.lock();
    //if(!m_pObj)
    //    m_pObj=new CInterface_SvrSettlement();
    //g_mutex.unlock();
    //return *m_pObj;
	return (CInterface_SvrSettlement&)SvrSettlementBusiness::getObj();
}
void CInterface_SvrSettlement::deleteObj(void)
{
	SvrSettlementBusiness::deleteObj();
}


int CInterface_SvrSettlement::getExampleValue(void)
{
	return 999;
}