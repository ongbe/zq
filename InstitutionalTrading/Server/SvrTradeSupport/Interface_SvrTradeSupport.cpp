// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrTradeSupport.h"
#include "SvrTradeSupport.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrTradeSupport* CInterface_SvrTradeSupport::m_pObj=NULL;


CInterface_SvrTradeSupport::CInterface_SvrTradeSupport()
{
}

CInterface_SvrTradeSupport::~CInterface_SvrTradeSupport()
{
}

CInterface_SvrTradeSupport& CInterface_SvrTradeSupport::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CInterface_SvrTradeSupport();
    g_mutex.unlock();
    return *m_pObj;
}


int CInterface_SvrTradeSupport::getExampleValue(void)
{
	return 999;
}