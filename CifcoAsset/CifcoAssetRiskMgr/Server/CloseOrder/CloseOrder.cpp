// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <queue>
#include "CloseOrder.h"
#include "TradeHandler.h"
#include "ThostFtdcTraderApi.h"
#include "Tools/easymutex.h"

struct ForceCloseOrderField
{
	///用户代码
	TThostFtdcUserIDType	UserID;
	///密码
	TThostFtdcPasswordType	Password;
	//下单结构体
	CThostFtdcInputOrderField order;
};

std::string g_strAddr;


//模块初始化
CLOSEORDER_API void InitFunc(void)
{

}

//模块结束，释放资源，关闭工作线程
CLOSEORDER_API void ReleaseFunc(void)
{
}

CLOSEORDER_API void InsertOrder( std::string strUser, std::string strPwd,  
								std::vector<CThostFtdcInputOrderActionField>& vOrderAction, 
								std::vector<CThostFtdcInputOrderField>& vOrderField)
{
	if ( vOrderAction.empty() && vOrderField.empty() )
	{
		return;
	}

	TThostFtdcBrokerIDType BrokerID;
	if ( !vOrderAction.empty())
	{
		strcpy(BrokerID, vOrderAction[0].BrokerID);
	}
	else
	{
		strcpy(BrokerID, vOrderField[0].BrokerID);
	}

	CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	HANDLE hConnEvent = CreateEvent(NULL, true, false, NULL);
	HANDLE hDoneEvent = CreateEvent(NULL, true, false, NULL);
	TThostFtdcUserIDType UserID;
	memset(UserID, 0, sizeof(UserID));
	strcpy_s(UserID, sizeof(UserID)-1, strUser.c_str());
	TThostFtdcPasswordType Pwd;
	memset(Pwd, 0, sizeof(Pwd));
	strcpy_s(Pwd, sizeof(Pwd)-1, strPwd.c_str());
	CTradeHandler handler(pUserApi, hConnEvent, hDoneEvent, UserID, Pwd, BrokerID, vOrderAction, vOrderField);
	pUserApi->RegisterSpi(&handler);
	char szAddr[256];
	memset(szAddr, 0, sizeof(szAddr));
	strcpy_s(szAddr, sizeof(szAddr)-1, g_strAddr.c_str());
	pUserApi->RegisterFront(szAddr);
	pUserApi->Init();

	DWORD dwRet = WaitForSingleObject(hConnEvent, 10*1000);
	if(WAIT_OBJECT_0 == dwRet)
	{
		WaitForSingleObject(hDoneEvent, 300*1000);
	}
	else if ( WAIT_TIMEOUT == dwRet )
	{
		printf("连接交易所超时.\n");
	}
	else
	{

	}

	pUserApi->Release();
}

CLOSEORDER_API void SetFrontAddr( std::string strAddr )
{
	g_strAddr = strAddr;
}
