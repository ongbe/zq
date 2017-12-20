#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRTSS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTSS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTSS_EXPORTS
#define SVRTSS_API __declspec(dllexport)
#else
#define SVRTSS_API __declspec(dllimport)
#endif
#endif


class SVRTSS_API CInterface_SvrTSS : public CInterface_SvrCommon
{
public:
    static CInterface_SvrTSS& getObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

private:
    CInterface_SvrTSS();
    virtual ~ CInterface_SvrTSS();
    static CInterface_SvrTSS* m_pObj;
};







