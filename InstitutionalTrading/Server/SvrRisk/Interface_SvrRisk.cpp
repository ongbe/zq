// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrRisk.h"
#include "SvrRisk.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrRisk* CInterface_SvrRisk::m_pObj=NULL;


CInterface_SvrRisk::CInterface_SvrRisk()
{
}

CInterface_SvrRisk::~CInterface_SvrRisk()
{
}

CInterface_SvrRisk& CInterface_SvrRisk::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CInterface_SvrRisk();
    g_mutex.unlock();
    return *m_pObj;
}


int CInterface_SvrRisk::getExampleValue(void)
{
	return 999;
}