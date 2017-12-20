// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "stdafx.h"
#include "CommonStruct.h"
#include "Real_SvrOffer.h"
#include "SvrOffer.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "OfferMainInterface.h"

CReal_SvrOffer* CReal_SvrOffer::m_pObj=NULL;

extern COfferMainInterface*			g_pOfferMain;
CReal_SvrOffer::CReal_SvrOffer()
{
}

CReal_SvrOffer::~CReal_SvrOffer()
{
}

CReal_SvrOffer& CReal_SvrOffer::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CReal_SvrOffer();
    g_mutex.unlock();
    return *m_pObj;
}


int CReal_SvrOffer::getExampleValue(void)
{
	return 999;
}
CF_ERROR CReal_SvrOffer::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID)
{	
	if(pInputOrder)
	{
		OFFER_INFO("下单： %s, %s \n",pInputOrder->InvestorID, pInputOrder->OrderRef);
		//PUSH_LOG_ERROR(Logger,false,true,"下单： %s, %s \n",pInputOrder->InvestorID, pInputOrder->OrderRef);
	}
	
	int nRet = 0;
	if(g_pOfferMain)
		nRet = g_pOfferMain->ReqOrderInsert_Main(pInputOrder, nTraderID, key, nRequestID);	
/*
	switch(nRet)
	{
		case -1:  cfErr = CF_ERROR_RISK_NETWORKCONNECTFAIL;			break;			
		case -2:  cfErr = CF_ERROR_RISK_NOTPROCESSFULL;				break;		
		case -3:  cfErr = CF_ERROR_RISK_NOTPROCESSFULLEACHSECOND;	break;
		case -4:  cfErr = CF_ERROR_RISK_NOTVALIDORDER ;				break;
		case -10: cfErr = CF_ERROR_RISK_ACCOUNTCONNECTFAIL;			break;
			
	}*/
	return nRet;
}
CF_ERROR CReal_SvrOffer::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID)
{
	if(pInputOrderAction == NULL)
		return 0;
	
	OFFER_INFO("撤单： %s, %s ",pInputOrderAction->InvestorID, pInputOrderAction->OrderRef);
	//PUSH_LOG_ERROR(Logger,false,true,"撤单： %s, %s \n",pInputOrderAction->InvestorID, pInputOrderAction->OrderRef);
	int nRet = 0;
	if(g_pOfferMain)
		nRet = g_pOfferMain->ReqOrderAction_Main(pInputOrderAction, nTraderID, key, nRequestID);	
/*
	CF_ERROR cfErr = CF_ERROR_SUCCESS;
	switch(nRet)
	{
		case -1:  cfErr = CF_ERROR_RISK_NETWORKCONNECTFAIL;			break;			
		case -2:  cfErr = CF_ERROR_RISK_NOTPROCESSFULL;				break;		
		case -3:  cfErr = CF_ERROR_RISK_NOTPROCESSFULLEACHSECOND;	break;
		case -4:  cfErr = CF_ERROR_RISK_NOTVALIDORDER ;				break;
		case -5:  cfErr = CF_ERROR_RISK_NOTVALIDACTION ;			break;
		case -10: cfErr = CF_ERROR_RISK_ACCOUNTCONNECTFAIL;			break;
	}*/
	return nRet;	
}