#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRFINANCEPRODUCT_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRFINANCEPRODUCT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRFINANCEPRODUCT_EXPORTS
#define SVRFINANCEPRODUCT_API __declspec(dllexport)
#else
#define SVRFINANCEPRODUCT_API __declspec(dllimport)
#endif
#endif

#pragma warning( disable: 4251 )
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <string>
#include <map>
#include <set>
#include <vector>
#include "CommonStruct.h"
         


class SVRFINANCEPRODUCT_API CInterface_SvrFinanceProduct : public CInterface_SvrCommon
{
public:
    static CInterface_SvrFinanceProduct& getObj(void);

    //问询函数
    virtual CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf)=0;

    //查询所有的理财产品
    virtual void GetFinancialProduct(std::vector<FinancialProduct>& vecFP)=0;
    //查询某个理财产品的所有交易员ID
    virtual void GetTraderByFinancialProductID(int nFinancialProductID, std::vector<int>& vecTD)=0;
    //查询所有理财产品和交易员的关系
    virtual void GetTraderProRelation(std::vector<TraderProRelation>& vecRela)=0;
};







