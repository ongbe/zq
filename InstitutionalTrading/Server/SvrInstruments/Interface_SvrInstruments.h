#pragma once

#include "Interface_SvrCommon.h"
#include "UserApiStruct.h"
#include <map>
#include <string>
#ifndef SVRINSTRUMENTS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTEMPLATE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRINSTRUMENTS_EXPORTS
#define SVRINSTRUMENTS_API __declspec(dllexport)
#else
#define SVRINSTRUMENTS_API __declspec(dllimport)
#endif
#endif


typedef std::map<std::string,InstrumentField> _MapInstrumentType;

class SVRINSTRUMENTS_API CInterface_SvrInstruments : public CInterface_SvrCommon
{
public:
    static CInterface_SvrInstruments& getObj(void);


	//---------------------------------------------------------------------------------
	//	重置所有合约
	//  返回： CF_ERROR_SUCCESS 代表成功
	//         CF_ERROR_COMMON_INPUT_PARAM 代表参数错误
	//---------------------------------------------------------------------------------
	virtual CF_ERROR WriteAllInstruments(const _MapInstrumentType & nMapInstruments) = 0;


	//---------------------------------------------------------------------------------
	//	获取所有合约
	//  返回： CF_ERROR_SUCCESS 代表成功
	//         CF_ERROR_NO_INSTRUMENTS 代表没有合约可取
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetAllInstruments( _MapInstrumentType & nMapInstruments) = 0;

	//---------------------------------------------------------------------------------
	//	获取单个合约的具体信息
	//  返回： CF_ERROR_SUCCESS 代表成功
	//         CF_ERROR_NO_INSTRUMENTS 代表没有对应合约
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetInstrumentField( const std::string& nInstrumentID,
		InstrumentField & nVecInstruments) = 0;


	//---------------------------------------------------------------------------------
	//	追加或更新单条合约
	//  返回： CF_ERROR_SUCCESS 代表成功
	//         CF_ERROR_COMMON_MEMEORY 代表内存错误，没有添加成功
	//---------------------------------------------------------------------------------
	virtual CF_ERROR AppendInstrument( const InstrumentField & nVecInstruments) = 0;

};







