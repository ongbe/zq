#pragma  once
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"

//请在这个头文件中定义服务器端和客户端共用的数据结构体
#pragma pack(push)
#pragma pack(1)

struct sFundInfoEx
{
	double mdbInitFund;
	PlatformStru_TradingAccountInfo mTradingAccountInfo;
};

#pragma pack(pop)