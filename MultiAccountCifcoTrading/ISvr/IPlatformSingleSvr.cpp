#include "StdAfx.h"
#include "IPlatformSingleSvr.h"

//#include "../inc/CmeApiSvr/CmeApiSvr.h"
#include "../inc/platformSvr/ServiceProxy.hpp"
//#include "../inc/patsapiSvr/PatsApiSvr.h"



IPlatformSingleSvr* IPlatformSingleSvr::Create(PTYPE type)
{
	IPlatformSingleSvr* pSingleSvr = NULL;
	switch(type)
	{
	case PTYPE_CTP: 
		pSingleSvr=new CServiceProxy();
		break;
	//case PTYPE_PATSAPI:
	//	pSingleSvr=new Pats::CPatsApiSvr();
	//	break;
	}
	return pSingleSvr;
}
void IPlatformSingleSvr::Release(IPlatformSingleSvr* pSingleSvr )
{
	if(pSingleSvr!=NULL)
	{
		delete pSingleSvr;
	}

}
