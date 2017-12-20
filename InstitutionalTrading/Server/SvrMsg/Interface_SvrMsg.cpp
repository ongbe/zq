// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrMsg.h"
#include "Real_SvrMsg.h"
#include "SvrMsg.h"
#include "easymutex.h"
#include "CommonPkg.h"

CInterface_SvrMsg* CInterface_SvrMsg::m_pObj=NULL;


CInterface_SvrMsg& CInterface_SvrMsg::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CReal_SvrMsg();
    g_mutex.unlock();
    return *m_pObj;
}
