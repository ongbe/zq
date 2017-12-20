// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrTemplate.h"
#include "SvrTemplate.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrTemplate* CInterface_SvrTemplate::m_pObj=NULL;


CInterface_SvrTemplate::CInterface_SvrTemplate()
{
}

CInterface_SvrTemplate::~CInterface_SvrTemplate()
{
}

CInterface_SvrTemplate& CInterface_SvrTemplate::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CInterface_SvrTemplate();
    g_mutex.unlock();
    return *m_pObj;
}


int CInterface_SvrTemplate::getExampleValue(void)
{
	return 999;
}