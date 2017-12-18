#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the QUOTTRANSFER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// QUOTTRANSFER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef QUOTTRANSFER_EXPORTS
#define QUOTTRANSFER_API __declspec(dllexport)
#else
#define QUOTTRANSFER_API __declspec(dllimport)
#endif

#include "MemoryCache.h"
#include "SocketBase.h"
#include <vector>
using namespace std;

QUOTTRANSFER_API void InitFunc(void);
QUOTTRANSFER_API void ReleaseFunc(void);
QUOTTRANSFER_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrOntimeFunc(void);


#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <set>
#include <map>
#include <string>
#include "Tools_Ansi.h"
#include "WinService.h"
#include "TcpServerWorker.h"
#include "UdpSimpleWorker.h"
#include "IniFile.h"
#include "StreamCache.h"
#include "WriteLog.h"
#include "FixedPackage.h"
#include "ThostFtdcUserApiStruct.h"


#define PACKAGEHEAD   0x7a8b0a6d
#define PACKAGETAIL   0xa38ca16a

#define CMD_RegQuotI	1			//注册I类行情数据，请求所有合约行情
#define CMD_RegQuotII	2			//注册II类行情数据，请求主力合约行情
#define CMD_RegQuotOKI	10			//注册成功回报
#define CMD_RegQuotOKII	20			//注册成功回报
#define CMD_Heartbeat	3			//心跳包
#define CMD_Quot1		4			//I类行情数据
#define CMD_Quot2		5			//II类行情数据

#define CMD_ReqIBContractsInfo 	31	//请求IB合约信息
#define CMD_IBContractsInfo		32	//发送IB合约信息

#define CMD_ReqCTPMainContractsInfo 	41	//请求CTP主力合约信息
#define CMD_CTPMainContractsInfo		42	//发送CTP主力合约信息

#define CMD_Quot3		 40			//II类行情数据
#define CMD_RegQuotIII	 41			//注册III类行情数据，请求含开盘价的所有合约行情
#define CMD_RegQuotOKIII 42			//注册成功回报




#pragma pack(push)
#pragma pack(4)
typedef struct
{
	DWORD	Head;					//包头
	int		PackageLen;				//包长度
	DWORD	CRCValue;				//CRC校验值
	DWORD	Cmd;					//命令字
	char	InstrumentID[32];		//合约名称
	double	LastPrice;				//最新价
	int 	Volume;					//成交量
	int		OpenInterest;			//持仓量
	double	PreSettlementPrice;		//昨结算价
	double  PreClosePrice;			//昨收盘价
	DWORD	UpdateTime_utc;			//更新时间,utc格式，单位为秒
	int		UpdateTimeMS;			//更新时间毫秒数
	DWORD	Tail;					//包尾
}Stru_Quot;

typedef struct
{
	DWORD	Head;					//包头
	int		PackageLen;				//包长度
	DWORD	CRCValue;				//CRC校验值
	DWORD	Cmd;					//命令字
	char	InstrumentID[32];		//合约名称
	double	OpenPrice;				//开盘价
	double	LastPrice;				//最新价
	int 	Volume;					//成交量
	int		OpenInterest;			//持仓量
	double	PreSettlementPrice;		//昨结算价
	double  PreClosePrice;			//昨收盘价
	DWORD	UpdateTime_utc;			//更新时间,utc格式，单位为秒
	int		UpdateTimeMS;			//更新时间毫秒数
	DWORD	Tail;					//包尾
}Stru_Quot2;

#pragma pack(pop)


//每个tcp数据连接的参数
struct Stru_DataSocket
{
	Stru_DataSocket()
	{
		hScoket=INVALID_SOCKET;
		memset(UserName,0,sizeof(UserName));
		bLogin=false;
		ReqType=2;
		time(&HeartbeatTime);
		pStreamCache=NULL;
		RemoteAddr.IP=0;
		RemoteAddr.Port=0;
	}

	SOCKET	hScoket;
	char	UserName[32];			//登录用户名
	bool	bLogin;					//是否已经登录
	int		ReqType;				//请求数据类型，1:I类(所有),2:II类(主力)
	time_t	HeartbeatTime;			//最近一次心跳时间
	CMemoryCache *pStreamCache;		//流Cache
	Stru_IPAddress RemoteAddr;
};




extern std::map<std::string,Stru_Quot2> g_Quots;
extern std::set<std::string> g_MainInstruments;				//主力合约集
extern std::map<std::string,std::string> g_ValidUsersI;		//合法的I类用户
extern std::map<std::string,std::string> g_ValidUsersII;	//合法的II类用户
extern std::map<SOCKET,Stru_DataSocket> g_TcpUsers;			//已经建立tcp连接的用户
extern CWriteLog *g_pWriteLog;
extern char	    *g_pSem;										//读写互斥锁
extern CFixedPackage *g_pQuotCache;
extern char    *g_IBContractsInfo;							//IB合约信息内容
extern int		g_IBContractsLen;									//IB合约信息内容长度，字节数
extern std::map<std::string,std::string> g_IBContractsInfoReqUser;	//合法的请求IB合约信息的用户
extern int g_HeartbeatTimeout;

extern char    *g_CTPMainContractsInfo;					    //CTP主力合约信息内容
extern int		g_CTPMainContractsLen;						//CTP主力合约信息内容长度，字节数
extern std::map<std::string,std::string> g_CTPMainContractsInfoReqUser;	//合法的请求CTP主力合约信息的用户
