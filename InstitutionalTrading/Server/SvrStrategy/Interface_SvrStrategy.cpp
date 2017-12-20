// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrStrategy.h"
#include "SvrStrategy.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrStrategy* CInterface_SvrStrategy::m_pObj=NULL;


CInterface_SvrStrategy::CInterface_SvrStrategy()
{
}

CInterface_SvrStrategy::~CInterface_SvrStrategy()
{
}

CInterface_SvrStrategy& CInterface_SvrStrategy::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CInterface_SvrStrategy();
    g_mutex.unlock();
    return *m_pObj;
}


int CInterface_SvrStrategy::getExampleValue(void)
{
	return 999;
}