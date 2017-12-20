#pragma once

#include "Interface_SvrCommon.h"
#include "CommonStruct.h"
#include "socketBase.h"
#include <vector>

#ifndef SVRLOGIN_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRLOGIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRLOGIN_EXPORTS
#define SVRLOGIN_API __declspec(dllexport)
#else
#define SVRLOGIN_API __declspec(dllimport)
#endif
#endif


class SVRLOGIN_API CInterface_SvrLogin : public CInterface_SvrCommon
{
public:
    static CInterface_SvrLogin& getObj(void);

	//---------------------------------------------------------------------------------
	//生成唯一的FrontID和SessionID
	//---------------------------------------------------------------------------------
	virtual void GenerateUniqueFrontAndSessionID(int & nFrontID,int & nSessionID) = 0;


	////---------------------------------------------------------------------------------
	////  设置交易所时间
	////---------------------------------------------------------------------------------
	virtual void  SetTimeInfo(const sExchangeTimeInfo& nsTimeInfo) = 0;



	//---------------------------------------------------------------------------------
	//	获取socket的对应的UserID,FrontID,SessionID
	//  成功返回CF_ERROR_SUCCESS
	//  失败返回CF_ERROR_LOGIN_SOCKET_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetSocketUserIDAndFrontSessionID(const SOCKET&,int & nUsrID,int & nFrontID,int& nSessionID) = 0;

	//---------------------------------------------------------------------------------
	//	获取FrontID,SessionID的对应的socket
	//  成功返回CF_ERROR_SUCCESS
	//  成功返回CF_ERROR_SUCCESS
	//  失败返回CF_ERROR_LOGIN_SOCKET_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetSocketHandleWithFrontAndSessionID(const int & nFrontID,const int& nSessionID,SOCKET & nhShocketHandle) = 0;



	//---------------------------------------------------------------------------------
	//	获取UserID的对应的socket列表
	//  成功返回CF_ERROR_SUCCESS
	//  成功返回CF_ERROR_SUCCESS
	//  失败返回CF_ERROR_LOGIN_USER_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetUserSockets(const int & nUserID, std::vector<SOCKET> & nVecSocket) = 0;



	//---------------------------------------------------------------------------------
	//	获取UserName的对应的socket列表
	//  成功返回CF_ERROR_SUCCESS
	//  成功返回CF_ERROR_SUCCESS
	//  失败返回CF_ERROR_LOGIN_USER_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetUserSockets(const std::string & nUserName, std::vector<SOCKET> & nVecSocket,int & nUserID) = 0;


	//---------------------------------------------------------------------------------
	//	设置用户的交易数据是否准备好了	
	//---------------------------------------------------------------------------------
	virtual void SetUserDataReady(const std::string & nUserName,bool nbReady) = 0;



	//---------------------------------------------------------------------------------
	//	判断主从
	//---------------------------------------------------------------------------------
	virtual bool IsSlaver() = 0;



	//---------------------------------------------------------------------------------
	//	判断从服务用户数据的有效性
	//---------------------------------------------------------------------------------
	virtual CF_ERROR IsValidSlaveUser(const std::string& strUser, const std::string& strPwd,const std::string& strIP, int& nUserID) = 0;


	//---------------------------------------------------------------------------------
	//	获取某一类用户的socket
	//---------------------------------------------------------------------------------
	virtual bool GetUserTypeSocket(UserType  nUserType,std::vector<SOCKET> & nVecSocket) = 0;


	//---------------------------------------------------------------------------------
	//	获取某一类用户的在线情况
	//---------------------------------------------------------------------------------
	virtual void GetAllOnlineUserStatus(std::vector<sUserOnlineInfo>& nvecInfo)= 0;

	virtual void GetOnlineUserStatus(int nUserId ,sUserOnlineInfo& nInfo)= 0;

};







