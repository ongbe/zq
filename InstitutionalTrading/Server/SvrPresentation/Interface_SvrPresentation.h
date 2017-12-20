#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRPRESENTATION_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRPRESENTATION_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRPRESENTATION_EXPORTS
#define SVRPRESENTATION_API __declspec(dllexport)
#else
#define SVRPRESENTATION_API __declspec(dllimport)
#endif
#endif


class SVRPRESENTATION_API CInterface_SvrPresentation : public CInterface_SvrCommon
{
public:
    static CInterface_SvrPresentation& getObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------
	//int getExampleValue(void);

};







