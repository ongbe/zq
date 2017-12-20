// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrStatistics.h"
#include "SvrStatistics.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrStatistics* CInterface_SvrStatistics::m_pObj=NULL;


CInterface_SvrStatistics::CInterface_SvrStatistics()
{
}

CInterface_SvrStatistics::~CInterface_SvrStatistics()
{
}

CInterface_SvrStatistics& CInterface_SvrStatistics::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CInterface_SvrStatistics();
    g_mutex.unlock();
    return *m_pObj;
}


int CInterface_SvrStatistics::getExampleValue(void)
{
	return 999;
}