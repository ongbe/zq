// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrTSS.h"
#include "SvrTSS.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrTSS* CInterface_SvrTSS::m_pObj=NULL;


CInterface_SvrTSS::CInterface_SvrTSS()
{
}

CInterface_SvrTSS::~CInterface_SvrTSS()
{
}

CInterface_SvrTSS& CInterface_SvrTSS::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CInterface_SvrTSS();
    g_mutex.unlock();
    return *m_pObj;
}


int CInterface_SvrTSS::getExampleValue(void)
{
	return 999;
}