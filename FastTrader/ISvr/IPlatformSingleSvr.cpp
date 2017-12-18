#include "StdAfx.h"
#include "IPlatformSingleSvr.h"

//#include "../inc/CmeApiSvr/CmeApiSvr.h"
#include "../inc/platformSvr/ServiceProxy.hpp"
//#include "../inc/patsapiSvr/PatsApiSvr.h"


IPlatformSingleSvr* IPlatformSingleSvr::Create(const string& strAccount,ePLATFORMTYPE type,int PlatformID,int MaxPlatformID)
{
	IPlatformSingleSvr* pSingleSvr = NULL;
	switch(type)
	{
	    case PTYPE_CTP: 
		    pSingleSvr=new CServiceProxy(strAccount,PlatformID,MaxPlatformID);
		    break;
	    //case PTYPE_PATSAPI:
	    //	pSingleSvr=new Pats::CPatsApiSvr();
	    //	break;
	}
	return pSingleSvr;
}


//int IPlatformSingleSvr::GetPlatformType()
//{
//	return PLATFORM_DEFAULT;
//}

void IPlatformSingleSvr::Release(IPlatformSingleSvr* pSingleSvr )
{
	if(pSingleSvr!=NULL)
	{
	    //delete (CCmeApiSvr*)pSingleSvr;
        delete (CServiceProxy*)pSingleSvr;
        //delete (CPatsApiSvr*)pSingleSvr;
	}

}
