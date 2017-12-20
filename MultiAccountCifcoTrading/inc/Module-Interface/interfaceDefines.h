#pragma once

// 第三方接口使用的数据结构

typedef char IAppIdType[64];
typedef char ILicenseType[64];
typedef char IHostNameType[64];
typedef char IUserNameType[32];
typedef char IPasswordType[32];
typedef char ITAType[32];
typedef DWORD ISessionIDType;
typedef char INamedPipeType[128];
typedef char IMessageType[256];

#define ISPLITCHAR			'\001'
#define ISPLITSTRING		"\001"
#define IPROTOCOLTITLE		"FastTraderV0.1"

#define IBUFSIZE			8192
#define IBUFMAXSIZE			16*1024*1024
#define IMAXSESSIOMNUM		20
#define IMAXREMAINTIME		180

#define IMASTERPIPENAME		"\\\\.\\pipe\\FastTrader\\v0_1\\Master"
#define IAPPPIPENAMEBODY	"\\pipe\\FastTrader\\v0_1\\App\\"

#define IYOUWELCOME			"You welcome to FastTrader, Protocol Version 0.1"

#define ISynIDNum			0xac34cd12
#define ITailNum			0x12342321

#define IMSGTYPE_A	1
#define IMSGTYPE_B	2
#define IMSGTYPE_C	3
#define IMSGTYPE_D	4
#define IMSGTYPE_E	5
#define IMSGTYPE_F	6
#define IMSGTYPE_G	7
#define IMSGTYPE_H	8

// MsgType A类型命令字
#define ICMD_A_Login			1	//"Login"
#define ICMD_A_LoginReturn		2	//"LoginReturn"
#define ICMD_A_Logout			3	//"Logout"
#define ICMD_A_Hello			4	//"Hello"
#define ICMD_A_HeartBeat		6	//"HeartBeat"

// MsgType B类型命令字
#define ICMD_B_IamFine			5	//"IamFine"
#define ICMD_B_rspHeartBeat		7	//"rspHeartBeat"

// MsgType C类型命令字
#define ICMD_C_QueryAccountInfo				100	//"QueryAccountInfo"
#define ICMD_C_QueryContractIDInfo			102	//"QueryContractIDInfo"
#define ICMD_C_QueryOrderInfo				104	//"QueryOrderInfo"
#define ICMD_C_QueryPositionInfo			106	//"QueryPositionInfo"
#define ICMD_C_QueryExchangeDateTime		108	//"QueryExchangeDateTime"
#define ICMD_C_QueryDepthMarket				110	//"QueryDepthMarket"

// MsgType D类型命令字
#define ICMD_D_ReturnAccountInfo			101	//"ReturnAccountInfo"
#define ICMD_D_ReturnContractIDInfo			103	//"ReturnContractIDInfo"
#define ICMD_D_ReturnOrderInfo				105	//"ReturnOrderInfo"
#define ICMD_D_ReturnPositionInfo			107	//"ReturnPositionInfo"
#define ICMD_D_ReturnExchangeDateTime		109	//"ReturnExchangeDateTime"
#define ICMD_D_ReturnDepthMarket			111	//"ReturnDepthMarket"

// MsgType E类型命令字
#define ICMD_E_SubscribeDepthMarket			200	//"SubscribeDepthMarket"

// MsgType F类型命令字
#define ICMD_F_ReturnDepthMarket			201	//"ReturnDepthMarket"
#define ICMD_F_rspDepthMarket				202	//"rspDepthMarket"

// MsgType G类型命令字
#define ICMD_G_NewOrder						300	//"NewOrder"
#define ICMD_G_CancelOrder					303	//"CancelOrder"

// MsgType H类型命令字
#define ICMD_H_rspNewOrderApiFail			301	//"rspNewOrderApiFail"
#define ICMD_H_rspNewOrderError				302	//"rspNewOrderError"
#define ICMD_H_rspCancelOrderApiFail		304	//"rspCancelOrderApiFail"
#define ICMD_H_rspCancelOrderError			305	//"rspCancelOrderError"
#define ICMD_H_rspOrderStatus				306	//"rspOrderStatus"
#define ICMD_H_rspTrade						307	//"rspTrade"


// 包头数据结构
typedef struct
{
	DWORD	SyncID;				//0xac34cd12
	ISessionIDType SessionID;	//sessionID
	DWORD	MsgType;			//消息类型
	DWORD	CMDID;				//命令ID
	DWORD	dwRspMsgNum;		//返回的消息编号
	DWORD	Length;				//包长度，包含的协议头和协议尾，即sizeof(PackageHead + Data + PackageTail)
	DWORD	CRC;				//校验，=0
} IPackageHeadStru;


// 包尾数据类型
typedef DWORD PackageTailType;


// 登录结构体
typedef struct {
	IAppIdType		strAppID;		// AppID
	ILicenseType	strLicense;		// License
	IHostNameType 	strHostName;	// 客户端计算机名
	IUserNameType 	strUserName;	// 用户名
	IPasswordType 	strPassword;	// 密码
	ITAType			strTraderAccount;	// 交易账号
	char 			chUseQuot;		// 使用行情，'1'表示使用，'0'表示不用
	char 			chUseOrder;		// 使用下单交易，'1'表示使用，'0'表示不用
} ILOGINDATA, *PILOGINDATA;


// Session管理结构体
typedef struct {
	ILOGINDATA		tLoginData;
	ISessionIDType	SessionID;
	INamedPipeType	strConnectAppName;
	INamedPipeType	strCreateAppName;
	DWORD			dwRemainTime;
} ISESSION, *PISESSION;


// 登录成功后的返回数据结构体
typedef struct {
	IMessageType	strMessage;		// 返回消息字符串
	INamedPipeType	strAppPipeName;	// AppPipe，一个names pipe
} ILOGINRETURN, *PILOGINRETURN;
