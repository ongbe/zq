#pragma once
#include "CommonStruct.h"
#include "CommonPublicDef.h"
#include <vector>


#ifndef SVRDBFTRANSFER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRDBOPR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRDBFTRANSFER_EXPORTS
#define SVRDBFTRANSFER_API __declspec(dllexport)
#else
#define SVRDBFTRANSFER_API __declspec(dllimport)
#endif
#endif

class SVRDBFTRANSFER_API CInterface_SvrDBFTransfer
{
private:
    //static CInterface_SvrDBFTransfer* m_pObj;   
    //static CInterface_SvrDBFTransfer& getObj(void);  

public:
	static void regeditCallBackPtr(void *pThostFtdcMdSpi);
};