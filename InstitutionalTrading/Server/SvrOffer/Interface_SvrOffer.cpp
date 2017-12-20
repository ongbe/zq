// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "stdafx.h"

#include "Interface_SvrOffer.h"
#include "Real_SvrOffer.h"


CInterface_SvrOffer* CInterface_SvrOffer::m_pObj=NULL;

extern COfferMainInterface*			g_pOfferMain;
CInterface_SvrOffer::CInterface_SvrOffer()
{
}

CInterface_SvrOffer::~CInterface_SvrOffer()
{
}

CInterface_SvrOffer& CInterface_SvrOffer::getObj(void)
{
  return (CInterface_SvrOffer&)CReal_SvrOffer::getObj();
}

