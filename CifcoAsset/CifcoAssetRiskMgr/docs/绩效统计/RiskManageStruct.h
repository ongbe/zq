#pragma once

#pragma pack(push)
#pragma pack(1)
#include <string>
typedef unsigned long RSDWORD;
typedef unsigned long RSCOLORREF;
typedef __int64       RSTIME;
typedef char ACCOUNTSTRING[21];
typedef char TRADEACCOUNTSTRING[32];
typedef char PASSWORDSTRING[33];
typedef char PERMISSIONNAMESTRING[21];
typedef char USERNAMESTRING[21];
typedef char CONTACTINFOSTRING[101];
typedef char IDCARDNOSTRING[21];
typedef char OTHERNAMESTRING[21];
typedef char INDICATORNAMESTRING[41];

typedef char DESCSTRING[101];

typedef  int  AssetMgmtOrgIDType;
typedef  int  RiskMgmtUserIDType;
typedef  int  FinancialProductIDType;
typedef  int  PermissionIDType;
typedef  int  StatusType;
typedef  int  AccountIDType;
typedef  int  TraderIDType;
typedef  int  RiskIndModuleType;
typedef  int  RiskLevelType;
typedef  int  PercentType;
typedef  char RiskItemDesc[101];
typedef	 char MsgTitle[41];
typedef  char MsgContent[1001];
typedef int	  RiskEventIDType;
typedef int	  RiskEventSubIDType;
typedef int   TradeAccountIDType;
typedef int   EventHandlingIDType;
typedef char InstrumentIDType[31];
typedef char CommentType[201];
typedef char DateType[11];

typedef double TFtdcMoneyType;
//////////////////////////////////////////////////////////////////////////
enum StatisticsIDType
{
	ST_PosProfit=1,//1.	持仓盈亏汇总
	ST_InitFund,//2.	账户初始权益汇总
	ST_Balance,//3.	账户当前总权益汇总
	ST_UseMargin,//4.	合约持仓保证金汇总
	ST_Position,//5.	合约的持仓量汇总
};
enum RiskIndicatorType
{
	RI_MaxPosition = 1,//市场冲击风险
	RI_FundLoss,//持仓限亏风险
	RI_GappedMarket,//隔夜跳空，风险敞口 
	RI_MarginRatio, //保证金比例风险
	RI_ProductMaxPositon, //品种最大持仓风险
	RI_FundNetValue,//基金净值风险

	RI_Count,//总共风险数目
};
//RiskIndicatorType 值对应的字符串称呼
/*这个数据从基础数据里面读取

#define STR_RI_MaxPosition   "单合约最大持仓"
#define STR_RI_FundLoss		 "持仓限亏风险"
#define STR_RI_GappedMarket	 "隔夜跳空，风险敞口"
#define STR_RI_MarginRatio	 "保证金比例风险"
#define STR_RI_ProductMaxPositon	 "品种最大持仓风险"
*/
enum MsgStatusType
{
	MsgStatus_New,      //服务器记录：发生
	MsgStatus_ChangeLevel,	//服务器记录：等级变更
	MsgStatus_Delete,	//服务器记录：消失

	MsgStatus_Appoint,//手动操作，委派给交易员
	MsgStatus_Finished,//手动操作，风险事件解决记录
	MsgStatus_ForceClose,//手动操作，执行手动强平记录
};
#define STR_MsgStatus_New			"新发生"
#define STR_MsgStatus_ChangeLevel	"等级变更"
#define STR_MsgStatus_Delete		"消失"
#define STR_MsgStatus_Appoint		"手动操作，委派给交易员"
#define STR_MsgStatus_Finished		"手动操作，风险事件解决记录"
#define STR_MsgStatus_ForceClose	"手动操作，执行手动强平记录"

enum RiskEventType
{
	RiskEvent_History,
	RiskEvent_Realtime,
};
enum EventHandlingActionType
{
	Action_ServerRecord,	//服务器记录
	Action_ClientRecord,	//手动记录
	Action_ForceClose,		//手动强平
};
#define STR_Action_ServerRecord    "服务器记录"
#define STR_Action_ClientRecord    "手动记录"
#define STR_Action_ForceClose	   "手动强平"
enum RoleType
{
	ROLE_TYPE_MANAGE,		//管理员
	ROLE_TYPE_RISK_CONTROL,	//风控员
};

enum IDCardType
{
	IDENTIFY_CARD,		//身份证
	MILITARY_ID,		//军官证
	OTHER_CARD			//其他
};

#define STR_T_FD_In    "入金"
#define STR_T_FD_Out    "出金"

#define STR_T_AVAF_Invalid    "未确认"
#define STR_T_AVAF_Valid      "有效"
#define STR_T_AVAF_Repeal     "冲正"

#define STR_T_FIOT_FundIO      "出入金"
#define STR_T_FIOT_Transfer     "银期转帐"
#define  NULL_QUERY_RESULE    "查询结果为空，请重新设置查询条件"
#define  NULL_QUERY_CONDITION    "查询条件为空，请设置查询条件"

//管理员/风控员账户结构
struct RiskMgmtUserInfo
{
	RiskMgmtUserIDType		nRiskMgmtUserID;
	ACCOUNTSTRING			szAccount;
	PASSWORDSTRING			szPassword;
	RoleType				nRoleType;				//账户类型
	AssetMgmtOrgIDType		nAssetMgmtOrgID;
	FinancialProductIDType	nFinancialProductID;
	StatusType				nStatus;				//启用状态
	USERNAMESTRING			szName;
	CONTACTINFOSTRING		szContactInfo;
	PermissionIDType		nPermissionID;
};

//管理员/风控员账户结构扩展
struct RiskMgmtUserInfoEx
{
	RiskMgmtUserInfo		userInfo;
	OTHERNAMESTRING			szAssetMgmtOrgName;	//资管区域
	OTHERNAMESTRING			szFinancialProductName;	//理财产品名
	PERMISSIONNAMESTRING	szPermissionName;	//权限名
};

struct LoginReq
{
	ACCOUNTSTRING	szAccount;
	PASSWORDSTRING	szPassword;
	RoleType		nRoleType;
};



struct LoginRsp
{
	AccountIDType	   nAccountID;			//账号ID
	PermissionIDType   nPermissionID;		//权限
	AssetMgmtOrgIDType nAssetMgmtOrgID;	//资管中心ID
	FinancialProductIDType	nFinancialProductID;	//理财产品ID
};

struct ModifyPwd
{
	RiskMgmtUserIDType	nRiskMgmtUserID;
	PASSWORDSTRING		szPassword;
};

//委托交易账户结构
struct TrustTradeAccount
{
	AccountIDType nTradeAccountID;
	ACCOUNTSTRING szAccount;
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	FinancialProductIDType nFinancialProductID;
	RiskIndModuleType	nRiskIndModuleID;
	TraderIDType		nTraderID;
	double				dProfit;
	char		szName[201];
	CONTACTINFOSTRING	szContactInfo;
	IDCardType			nIDCardType;
	IDCARDNOSTRING		szIDCardNo;
	PASSWORDSTRING	szPwd;
	bool operator<(const TrustTradeAccount item) const
	{ //STL 里面用到排序
		if(nTradeAccountID < item.nTradeAccountID)
			return true;
		else  if(nTradeAccountID > item.nTradeAccountID)
			return false;

		if(nAssetMgmtOrgID < item.nAssetMgmtOrgID)
			return true;
		else if(nAssetMgmtOrgID > item.nAssetMgmtOrgID)
			return false;

		if(nRiskIndModuleID < item.nRiskIndModuleID)
			return true;
		else if(nRiskIndModuleID > item.nRiskIndModuleID)
			return false;

		if(nTraderID < item.nTraderID)
			return true;
		else if(nTraderID > item.nTraderID)
			return false;

		if(nIDCardType < item.nIDCardType)
			return true;
		else if(nIDCardType > item.nIDCardType)
			return false;


		int i = strcmp(szAccount, item.szAccount) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		i = strcmp(szName, item.szName) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		i = strcmp(szContactInfo, item.szContactInfo) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		i = strcmp(szIDCardNo, item.szIDCardNo) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;

		i = strcmp(szPwd, item.szPwd) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		return false;
	}

};

//委托交易账户扩展
struct TrustTradeAccountEx
{
	TrustTradeAccount trustAccount;
	OTHERNAMESTRING szAssetMgmtOrgName;	//资管区域
	OTHERNAMESTRING szFinancialProductName;	//理财产品名
	USERNAMESTRING szTraderName;	//交易员名
	OTHERNAMESTRING szRiskIncModuleName;	//风险模型名
};

//交易员信息结构
struct TraderInfo
{
	TraderIDType	nTraderID;
	USERNAMESTRING	szName;
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	FinancialProductIDType nFinancialProductID;
	char szTel[21];
	char szMobile[21];
	char szOtherContactInfo[21];
};

//交易员扩展结构
struct TraderInfoEx
{
	TraderInfo traderInfo;
	OTHERNAMESTRING szAssetMgmtOrgName;	//资管区域
	OTHERNAMESTRING szFinancialProductName;	//理财产品名
};

//资管组织结构
struct AssetMgmtOrganization
{
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	OTHERNAMESTRING		szName;
	AssetMgmtOrgIDType nUpperLevelOrgID;
	int nScaleLevel;
	double dLongitude;
	double dLatitude;
};

//理财产品结构
struct FinancialProduct
{
	FinancialProductIDType nFinancialProductID;
	OTHERNAMESTRING szName;
	DESCSTRING szScopeDesc;
};

//理财产品与资管组织关系
struct AssetOrgProRelation
{
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	FinancialProductIDType nFinancialProductID;

	bool operator<(const AssetOrgProRelation&	src) const
	{
		if(nAssetMgmtOrgID<src.nAssetMgmtOrgID)
			return true;
		else if(nAssetMgmtOrgID==src.nAssetMgmtOrgID)
		{
			if(nFinancialProductID<src.nFinancialProductID)
				return true;
		}
		return false;
	}

};
//
struct EventLevelID
{
	RiskEventIDType EventID;
	RiskLevelType	LevelID;
};
//
struct	StatisticsEntry
{
	StatisticsIDType		nStatisticsType;
	AssetMgmtOrgIDType		nAssetMgmtOrgID;
	FinancialProductIDType	nFinancialProductID;	
	InstrumentIDType		InstrumentID;
	double					dblValue;
};
//风险指标模型结构
struct RiskIndicatorModule
{
	RiskIndModuleType		nRiskIndModuleID;
	OTHERNAMESTRING			szName;
};

enum RiskWarningType
{
	WarningType_Beep,		//鸣笛
	WarningType_Vibrate,	//震动
	WarningType_Both,		//鸣笛加震动
};

//风险预警记录结构
struct RiskResponse
{
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;

	bool operator!=(const RiskResponse&	src) const
	{
		return (memcmp(this,&src,sizeof(*this))!=0);
	}
};
struct RiskWarning
{
	RiskIndModuleType nRiskIndModuleID;
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;
};
//风险级别设置结构
struct IndLevelWarning
{
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;
};

//品种风险级别设计结构
struct ProductRiskWarning
{
	TradeAccountIDType nTradeAccountID;
	InstrumentIDType  szProductID;	//品种名称
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;
};

struct ProducrRiskWarningKey
{
	InstrumentIDType  szProductID;	//品种名称
	TraderIDType nTraderID;
};

//风险指标定义结构
struct RiskIndicator
{
	RiskIndicatorType	nRiskIndicatorID;
	INDICATORNAMESTRING	szName;
	RiskItemDesc		szAlgorithmDesc;
};

//最大风险级别
struct MaxRiskLevel
{
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType nMaxLevel;
};

//事件消息模版结构
struct EventMessageTemplate
{
	RiskIndicatorType		nRiskIndicatorID;
	RiskLevelType			nRiskLevelID;
	MsgTitle				szTitle;
	MsgContent				szContent;
};

//风险事件数据结构
struct RiskEvent
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	RSTIME					lEventTime;			
	TradeAccountIDType		nTradeAccountID;
	RiskIndModuleType		nRiskIndModuleID;
	RiskIndicatorType		nRiskIndicatorID;
	InstrumentIDType		InstrumentID;
	RiskLevelType			nRiskLevelID;
	double					dblIndicatorValue;
	MsgStatusType			nMsgSendStatus;
	RiskEventType			nIsValid;		//0 是历史风险事件， 1 实时风险事件
	bool operator<(const RiskEvent&	src) const
	{
		return lEventTime<src.lEventTime;
	}
};

struct RiskEventQueryKey
{
	TradeAccountIDType		nTradeAccountID;
	DateType szStartDate;	//起始时间
	DateType szEndDate;
};
//风险数据处理用户请求结构
struct RiskEventHandlingUserReq
{
	TradeAccountIDType		nTradeAccountID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;//处理者
	USERNAMESTRING			szAccepter;//受理者
	MsgStatusType			nProcessStatus;//手动操作的三种情况
	CommentType				szContent;//备注
};
//风险数据处理表数据结构
struct RiskEventHandling
{
	EventHandlingIDType		nRiskEventHandlingID;
	RSTIME					lHandlingTime;
	TradeAccountIDType		nTradeAccountID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;//客户端记录，处理者
	USERNAMESTRING			szAccepter;//客户端记录，受理者
	EventHandlingActionType	nAction;//服务器记录，手动记录，手动强平
	MsgStatusType			nProcessStatus;//发生，处理中，消失
	CommentType				szContent;//客户端记录，备注
	bool operator<(const RiskEventHandling&	src) const
	{
		return lHandlingTime<src.lHandlingTime;
	}
};

//风险数据处理表数据结构扩展
struct RiskEventHandlingEx
{
	EventHandlingIDType		nRiskEventHandlingID;
	RSTIME					lHandlingTime;
	TradeAccountIDType		nTradeAccountID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;//客户端记录，处理者
	USERNAMESTRING			szAccepter;//客户端记录，受理者
	EventHandlingActionType	nAction;//服务器记录，手动记录，手动强平
	MsgStatusType			nProcessStatus;//发生，处理中，消失
	CommentType				szContent;//客户端记录，备注
	RiskIndicatorType		nRiskIndicatorID;
	RiskLevelType			nRiskLevelID;
	double					dblIndicatorValue;
	InstrumentIDType		InstrumentID;
};

struct RiskEventHandlingQueryKey
{
	RiskEventIDType			nRiskEventID;
};

//消息结构定义
struct MessageInfo
{
	int nMessageID;
	char szTitle[41];
	MsgContent szContent;
	DateType szExpiredDate;
	char szCreateDate[20];	//格式"yyyy-mm-dd HH:MM:SS"
	int nOwner;
	ACCOUNTSTRING szOwner;	//发送者用户名
};

struct MessageQryCondition
{
	RiskMgmtUserIDType nRiskMgmtUserID;	//消息创建者ID
	DateType szCreateDate;				//创建时间，格式为 YYYY-MM-DD
};

struct TargetAccount
{
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
};

struct MsgSendStatus
{
	int nMessageID;
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
	int nSendStatus;	// 消息发送状态 0 未发送 1 已发送
};

struct PermissionName
{
	PermissionIDType nPermissionID;
	OTHERNAMESTRING szName;
};

struct PermissionNameEx
{
	PermissionIDType nPermissionID;
	OTHERNAMESTRING szName;
	int nPermissionType;
};

struct PermissionValue
{
	PermissionIDType nPermissionID;
	int nActionID;//对应的是 PermissionActionType
	int nValue;
};

struct PermissionNode
{
	int nNodeID;
	OTHERNAMESTRING szName;
	int nNodeType;
};

struct PermissionAction
{
	int nActionID;
	int nNodeID;
	OTHERNAMESTRING szName;
};

struct HisTradeDataQueryKey
{
	char AccountID[13];
	DateType TradingDayFrom;	//起始时间 格式为"YYYY-MM-DD"
	DateType TradingDayTo;		//结束时间 格式为"YYYY-MM-DD"
};

//权限定义
enum PermissionType
{
	PERMISSION_MANAGE,			//管理员
	PERMISSION_RISK_CONTROL,	//风控员
};

enum PermissionActionType
{
	//管理终端权限
	ACCOUNT_ADD = 1,		//增加账户
	ACCOUNT_MODIFY,			//修改账户
	ACCOUNT_DEL,			//删除账户
	ACCOUNT_STOP_ACTIVE,	//启用/停用账户
	ACCOUNT_BATCH_IMPORT,	//批量导入账户
	ORGANIZATION_MANAGE,	//组织架构管理
	FIN_PRODUCT_MANAGE,		//产品管理
	PERMISSION_SET,			//权限设置
	RISK_MANAGE,			//预警模型设置
	RISK_EVENT_MSG_MANAGE,	//风险消息设置

	//风控终端权限
	FORMS_STATISTICS,		//报表统计
	MESSAGE_SEND,			//发送消息
	TODAY_OPEN_ORDER_QUERY,	//当日报单查询
	TODAY_TRADE_QUERY,		//当日成交查询
	TODAY_POSITION_QUERY,	//当日持仓查询
	TODAY_ACCOUNT_QUERY,	//当日资金查询
	TODAY_DEPOSIT_QUERY,	//当日出入金查询
	INVESTOR_INFO_QUERY,	//投资者信息查询
	UNREAD_MSG_QUERY,		//消息通知查询
	RISK_EVENT_QUERY,		//用户事件查询
	FORCE_CLOSE_ORDER,		//强平报单
};

//基金净值参数设置
struct NetFundParam
{
	int nTradeAccountID;	//委托交易账号ID
	double dInnerVolumn;	//场内份额
	double dOuterVolumn;	//场外份额
	double dOuterNetAsset;	//场外资产净值
	int nInnerNetAssetOption;	//场内资产净值选项
};
//基金净值每日净值结果
struct NetFundCalcResult
{
	int nTradeAccountID;
	double dInnerVolumn;
	double dOuterVolumn;
	double dInnerNetAsset;
	double dOuterNetAsset;
	double dInnerPerNet;
	double dOuterPerNet;
	double dTotalNetAsset;
	DateType szUpdateDate;
};
//基金净值强平参数设置
struct ForceCloseParam
{
	int nTradeAccountID;		//委托交易账户ID
	int nForceCloseInd;			//强平指标
	float fThreshold;			//触发阀值
	int nCloseStrategy;			//平仓策略 0，全部平仓
	int nForceClosePriceOption;	//强平价位 0，市价
};



//汇总数据
/*
组织机构：
szName:		 组织机构名
PreBalance:	 该组织机构下所有账户初始权益和 (SeqPreRiskAccountField::PreBalance)
Balance:	 该组织机构下所有账户初始权益和 (RiskSyncAccountField::Balance)	
OffBalance:  Balance - PreBalance
YearEarnRate:


MarginRate:  该组织机构下所有账户保证金和和动态权益和的比率(RiskSyncAccountField::CurrMargin/ RiskSyncAccountField::Balance)
*/
struct SGather
{
	int						nID;	         //组织机构ID,理财产品ID,账户ID
	int						nIDType;		 //0 按部门查询,1 按理财产品查询,2 按某一账户查询
	TFtdcMoneyType			SumPreBalance;		 //初始权益和
	TFtdcMoneyType			SumBalance;		 //当前权益和
	TFtdcMoneyType			YearEarnRate;	 //年化收益率
	TFtdcMoneyType		    MarginRate;	     //保证金比率
};
struct SGatherData
{
	int						nIDType;		 //0 按部门查询,1 按理财产品查询,2 按某一账户查询
	int						nID;	         //组织机构ID,理财产品ID,账户ID	
	DateType                szUpdateDate;    //查询条件如果-1：则为最后一个今日之前有效交易日存储的数据
	double					SumPreBalance;    //初始权益和
	double					SumBalance;		 //当前权益和
	double					OffBalance;		 //今日权益变化]
	double					YearEarnRate;	 //年化收益率
	double				    MarginRate;	     //保证金比率

	double					dbValue;		 //指标值
	int						nDayCount;		 //总共多少天
};

#pragma pack(pop)