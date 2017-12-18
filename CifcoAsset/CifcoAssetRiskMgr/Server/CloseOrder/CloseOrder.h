#pragma once

#ifndef CLOSEORDER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CLOSEORDER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CLOSEORDER_EXPORTS
#define CLOSEORDER_API __declspec(dllexport)
#else
#define CLOSEORDER_API __declspec(dllimport)
#endif
#endif

#include "ThostFtdcUserApiStruct.h"
//模块初始化
CLOSEORDER_API void InitFunc(void);

//模块结束，释放资源，关闭工作线程
CLOSEORDER_API void ReleaseFunc(void);

CLOSEORDER_API void InsertOrder(std::string strUser, std::string strPwd, 
								std::vector<CThostFtdcInputOrderActionField>& vOrderAction, 
								std::vector<CThostFtdcInputOrderField>& vOrderField);

CLOSEORDER_API void SetFrontAddr(std::string strAddr);