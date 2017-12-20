#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVROFFER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVROFFER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVROFFER_EXPORTS
#define SVROFFER_API __declspec(dllexport)
#else
#define SVROFFER_API __declspec(dllimport)
#endif
#endif

//
class SVROFFER_API CReal_SvrOffer : public CInterface_SvrCommon
{
public:
    static CReal_SvrOffer& getObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

	///报单录入请求
	virtual CF_ERROR ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);

	///报单操作请求
	virtual CF_ERROR ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);

private:
    CReal_SvrOffer();
    virtual ~ CReal_SvrOffer();
    static CReal_SvrOffer* m_pObj;
};







