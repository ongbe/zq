#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRQUOTSERVER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRQUOTSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRQUOTSERVER_EXPORTS
#define SVRQUOTSERVER_API __declspec(dllexport)
#else
#define SVRQUOTSERVER_API __declspec(dllimport)
#endif
#endif

#include <string>
#include <map>
#include <set>
#include <vector>
#include "CommonStruct.h"


class SVRQUOTSERVER_API CInterface_SvrQuotServer : public CInterface_SvrCommon
{
public:
    static CInterface_SvrQuotServer& getObj(void);
};







