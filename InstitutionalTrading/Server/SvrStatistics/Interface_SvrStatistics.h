#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRSTATISTICS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRSTATISTICS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRSTATISTICS_EXPORTS
#define SVRSTATISTICS_API __declspec(dllexport)
#else
#define SVRSTATISTICS_API __declspec(dllimport)
#endif
#endif


class SVRSTATISTICS_API CInterface_SvrStatistics : public CInterface_SvrCommon
{
public:
    static CInterface_SvrStatistics& getObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

private:
    CInterface_SvrStatistics();
    virtual ~ CInterface_SvrStatistics();
    static CInterface_SvrStatistics* m_pObj;
};







