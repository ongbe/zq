#pragma once

#pragma pack(push)
#pragma pack(1)

typedef char ACCOUNTSTRING[21];
typedef char BROKERCODESTRING[21];
typedef char TRADEACCOUNTSTRING[32];
typedef char PASSWORDSTRING[33];
typedef char PERMISSIONNAMESTRING[21];
typedef char IDCARDNAMESTRING[21];

struct AccountInfo
{
	int nAccountID;
	ACCOUNTSTRING szAccount;
	PASSWORDSTRING szPassword;
	int nAccountType;
	int nOwnerAccountID;
	char szName[21];
	char szContactInfo[101];
	int nIDCardType;
	char szIDCardNo[21];
	int nAccountStatus;
	int nLogonStatus;
	int nViewPermission1;
	int nViewPermission2;
	int nPermissionID;
	char szDate[11];
};

struct AccountInfoEx
{
	AccountInfo accountInfo;
	//附加字段
	ACCOUNTSTRING szOwnerAccount;
	PERMISSIONNAMESTRING szPermissionName;
	IDCARDNAMESTRING szIDCardName;
};

struct LoginReq
{
	ACCOUNTSTRING szAccount;
	PASSWORDSTRING szPassword;
	int nAccountType;
};

enum AccountType
{
	ACCOUNT_TYPE_MANAGE,
	ACCOUNT_TYPE_LOGON,
};

struct LoginRsp
{
	int nAccountID;
	int nPermissionID;
};

struct ModifyPwd
{
	int nAccountID;
	PASSWORDSTRING szPassword;
};

struct BrokerInfo
{
	int nBrokerID;
	BROKERCODESTRING szCode;
	char szCompany[65];
	int nShareWithLowerLever;
	int nOwnerAccountID;
};

struct ServerGroup
{
	int nServerNo;
	char szServerName[21];
	int nBrokerID;
	int nShare;
};

struct ServerAddr
{
	int nAddrID;
	int nServerNo;
	char szServerAddress[129];
	int nPort;
	int nServerType;
};

struct IDCardInfo
{
	int nIDCardType;
	IDCARDNAMESTRING szIDCardName;
};

struct TradeAccount
{
	TRADEACCOUNTSTRING szTradeAccount;
	PASSWORDSTRING szPassword;
	int nOwnerAccountID;
	int nBrokerID;
	int nServerNo;
	char szNickName[21];
	int nID;
	int nRowNo;
};
struct CTPTradeAccount
{
	TradeAccount t;
	PASSWORDSTRING DynamicPassword;
	bool		UseQuot;
	bool		StrategyValid;
	char        StatusText[64]; 
};
struct TradeAccountKey
{
	TRADEACCOUNTSTRING szTradeAccount;
	int nOwnerAccountID;
};

struct PermissionModule
{
	int nModuleID;
	char szModuleName[21];
	int nCommonOperators;
	int nAdditionalOperators;
	int nModuleType;
};

struct Permission
{
	int nPermissionID;
	int nOwnerAccountID;
	PERMISSIONNAMESTRING szName;
	int nPermissionType;
	int nPermissionDataSize;
	char permissionData[120];
};

struct PermissionSimple
{
	int nPermissionID;
	int nOwnerAccountID;
	PERMISSIONNAMESTRING szName;
	int nPermissionType;
};

struct PermissionData
{
	int nModuleID;
	int nCommonOperators;
	int nAdditionalOperators;
};

struct MessageInfo
{
	int nMessageID;
	char szTitle[41];
	char szContent[2001];
	char szDate[11];
	int nOwner;
	int nType;
};

struct TargetAccount
{
	int nAccountID;
	ACCOUNTSTRING szAccount;
};

//消息类型定义
enum MessageType
{
	MESSAGE_COMMON,
	MESSAGE_PERSONAL,
};

//权限定义
enum PermissionType
{
	PERMISSION_BKG,
	PERMISSION_FORE,
};


//权限模块定义
enum PermissionModuleType
{
	MODULE_FORE_ACCOUNT_MANAGE = 1,	//前台账号管理
	MODULE_BKG_ACCOUNT_MANAGE,		//管理账号管理
	MODULE_BROKER_MANAGE,			//经纪公司管理
	MODULE_SERVER_MANAGE,			//服务器管理
	MODULE_PERMISSION_MANAGE,		//权限设置
	MODULE_FORE_CLIENT_MANAGE,		//前台客户端
};

//后台通用权限
#define PERMISSION_BKG_COMMON_ADD	0x1		//增加
#define PERMISSION_BKG_COMMON_MODIFY	0x2	//修改
#define PERMISSION_BKG_COMMON_DEL	0x4		//删除
#define PERMISSION_BKG_COMMON_QUERY	0x8		//查看
#define PERMISSION_BKG_COMMON_CHANGE_STATUS	0x10	//状态改变
#define PERMISSION_BKG_COMMON_ADD_MODIFY	0x20	//增加/修改

//前台权限
#define PERMISSION_FORE_LOGON	0x1	//登录
#define PERMISSION_FORE_ADD_TRADE_ACCOUNT	0x2	//添加交易账号

#define PERMISSION_STRING_ADD		"增加"
#define PERMISSION_STRING_MODIFY	"修改"	
#define PERMISSION_STRING_DEL		"删除"
#define PERMISSION_STRING_QUERY		"查看"
#define PERMISSION_STRING_CHANGE_STATUS	"状态改变"
#define PERMISSION_STRING_ADD_MODIFY		"增加/修改"
#define PERMISSION_STRING_LOGON		"登录"
#define PERMISSION_STRING_ADD_TRADE_ACCOUNT	"添加交易账号"

#pragma pack(pop)