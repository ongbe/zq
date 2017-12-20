#pragma once
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include <map>
#include <set>
#include <vector>
#include <iostream>
#pragma pack(push)
#pragma pack(1)
typedef int RiskLevelType;
typedef unsigned long RSCOLORREF;
typedef char RiskProductID[300];
typedef int RiskEventIDType;
typedef int RiskEventSubIDType;
typedef __int64 RSTIME;
typedef int TradeInvestorIDType;
typedef int EventHandlingIDType;
typedef char USERNAMESTRING[21];
typedef char CommentType[201];
typedef  char MsgContent[1001];
typedef char ACCOUNTSTRING[21];
typedef	 char MsgTitle[41];
typedef char INDICATORNAMESTRING[41];
typedef char RESPONSENAMESTRING[41];
typedef  char RiskItemDesc[101];
typedef int   TradeAccountIDType;
typedef  char MsgContent[1001];
typedef char DateType[21];
typedef char ACCOUNTSTRING[21];
typedef int RiskMgmtUserIDType;
typedef char TInstrumentIDType[31];
typedef char BrokerIDType[11];
typedef char RiskTime[6];
typedef char TraderAccount[13];
///净
#define T_PD_Net '1'
///多头
#define T_PD_Long '2'
///空头
#define T_PD_Short '3'
//风险指标类型
enum RiskIndicatorType 
{
	RI_MaxPosition = 1,			//合约最大持仓;   合约持仓总数（买卖方向汇总）
	RI_MarketShock,				//市场冲击风险;   合约持仓总数（买卖方向汇总）占市场总持仓数百分比     
	RI_AccountLoss,				//账户限亏		  （今日动态权益 -- 账户初始权益）/账户初始权益
	RI_TodayLoss,				//今日限亏		  账户今日亏损占日初权益的百分比
	RI_GappedMarket,			//隔夜跳空        账户各合约持仓综合后（买卖方向中和），预测第二天最大跳空亏损占账户初始权益百分比
	RI_MarginRatio,				//保证金比例风险  保证金和动态权益的比值（风险度）
	RI_SingleHoldTime,          //单笔持仓时间    
	RI_FundNetValue,			//基金净值风险	
	RI_TRADE_FORBID,			//合同级风控，交易合约限制（也有可能限制的是品种）
	RI_MARGIN_FORBID,			//合同级风控，保证金使用限制
	RI_LOSS_FORBID,			    //合同级风控，亏损限制


	RI_ONEDAY_LARGESTLOSS,      //公司级风控，单日最大亏损
	RI_LOSSMAXVALUE,			//公司级风控，亏损最大值
	RI_MAXRETRACE,				//公司级风控，最大回撤值
	RI_CONTRACTS_VALUES,        //公司级风控，账户持仓合约价值
	RI_BULLBEAR_VALUES,         //公司级风控，多空轧差合约价值
	RI_LOSS_CONTINUEDAYS,       //公司级风控，连续亏损天数
	RI_LOSS_PERCENT,			//公司级风控，亏损比例线限制


	RI_Count,					//总共风险数目
};

enum RiskWarningType
{	
	WarningType_None                    = 0x0000000,
	WarningType_ForbidOpen_Single		= 0x0000001,			//限制开仓(单合约)
	WarningType_ForbidOpen				= 0x0000002,			//限制开仓
	WarningType_ForbidOrder_Single		= 0x0000004,			//限制下单(单合约)
	WarningType_ForbidOrder				= 0x0000010,			//限制下单
	WarningType_ForceClose_Single		= 0x0000020,			//市价强平（单合约）
	WarningType_ForceClose				= 0x0000040,			//市价强平
	
	WarningType_Beep					= 0x0000080,			//鸣笛
	WarningType_Vibrate					= 0x0000100,			//震动
	WarningType_Both					= 0x0000200,			//鸣笛加震动
    WarningType_ShowMsg					= 0x0000400,			//消息提示(弹出消息框)
	WarningType_ForceClose_SingleOpen	= 0x0000800,			//单笔持仓强平
};

//基金净值参数设置
struct NetFundParam
{
	int nOrgID;				//部门ID
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

struct AccountLossDay
{
	BrokerIDType	BrokerID;
	TraderAccount	AccountID;	
	double dynamicProfit;	
	int    nlossdaycount;
	DateType szLastDate;
	double maxdynamicProfit;//账户一直以来的最高动态市值
	AccountLossDay& operator=(const AccountLossDay& r) 
	{
		strcpy(BrokerID, r.BrokerID);
		strcpy(AccountID, r.AccountID);
		dynamicProfit = r.dynamicProfit;
		nlossdaycount = r.nlossdaycount;
		strcpy(szLastDate, r.szLastDate);
		maxdynamicProfit = r.maxdynamicProfit;
		return *this;
	}
};

//具体风险等级
struct RiskWaring
{
	int					nOrgID;				//组织机构ID	
	char				cTimeBegin[6];		//00:00
	char				cTimeEnd[6];		//24:00
	RiskIndicatorType	nRiskIndicatorID;	//风险指标类型

	RiskLevelType	    nRiskLevelID;			//风险等级
	float			    fThresholdValue;      //风险值 
	int				    nResponseType;        //动作

	RSCOLORREF		    nColor;               //颜色
	RiskWaring()
	{
		memset(this,0,sizeof(*this));
		strcpy(cTimeBegin, "00:00");
		strcpy(cTimeEnd, "24:00");
	}
};
//组织机构和风控方案的关系
struct RiskOrgIDPlanRelation
{
	int					nOrgID;				//组织机构ID
	int					nRiskType;			//0：合同风控 1：公司风控
	char				cTimeBegin[6];		//00:00
	char				cTimeEnd[6];		//24:00
	RiskIndicatorType	nRiskIndicatorID;	//风险指标类型
	
	RiskProductID		ProductID;			//品种名称
	bool				bUse;				//是否启用该风控方案	

	//合同级风控需要变量（需要下面的ProductID）
	char				cInstruments[300];  //交易合约限制；后面的品种名称可以用于赋值“品种”
	double				dbMarginDividDynamic;//保证金/动态权益
	double				dbMarginUse;		 //使用保证金	
	double				dbLossAmount;		//亏损总额
	double				dbLossPercent;      //亏损比例
	char				szPlan[32];			//风控方案名字


	//原来风控的变量，是全部品种还是 特定的几个品种（ProductID）
	bool				bAllProduct;		//全部品种
	RiskOrgIDPlanRelation()
	{
		memset(this,0,sizeof(*this));
		//默认的风险指标24小时有效
		strcpy(cTimeBegin, "00:00");
		strcpy(cTimeEnd, "24:00");
	}
};
//组织机构的某一级风险
struct RiskPlan
{
	RiskOrgIDPlanRelation		OrgIDPlanRelation;
	RiskWaring					WaringLevel;
	NetFundParam				netFundParam;//只给基金净值风险指标设置使用
	RiskPlan()
	{
		memset(this,0,sizeof(*this));
		strcpy(OrgIDPlanRelation.cTimeBegin, "00:00");
		strcpy(OrgIDPlanRelation.cTimeEnd, "24:00");
		strcpy(WaringLevel.cTimeBegin, "00:00");
		strcpy(WaringLevel.cTimeEnd, "24:00");
	}
	bool operator<(const RiskPlan&	r) const
	{		
		if(WaringLevel.nOrgID < r.WaringLevel.nOrgID)
			return true;
		else if(WaringLevel.nOrgID >r.WaringLevel.nOrgID)
			return false;

		if(WaringLevel.nRiskIndicatorID<r.WaringLevel.nRiskIndicatorID)
			return true;
		else if(WaringLevel.nRiskIndicatorID>r.WaringLevel.nRiskIndicatorID)
			return false;

		if(WaringLevel.nRiskLevelID<r.WaringLevel.nRiskLevelID)
			return true;
		else if(WaringLevel.nRiskLevelID>r.WaringLevel.nRiskLevelID)
			return false;

		return false;
	}
};
//风险指标启用，停用，或者删除用到的数据结构
struct RiskChange
{
	int					nOrgID;				//组织机构ID
	RiskIndicatorType	nRiskIndicatorID;	//风险指标类型
	bool				bUse;				//true:为启动，false为停用
};
struct SResponse
{	
	int nRiskEventID;						//风险事件ID
	std::map<int,RiskPlan>  mapResponse;   //风险等级
	std::set<std::string>   setProducts;  //主要针对最大持仓，市场冲击风险，限定的品种总数
	std::set<std::string>   setInstruments;//针对同时有品种和合约限制的情况，比如合同风控，限制交易里面有单独合约的情况
	SResponse()
	{
		nRiskEventID = 0;
	}
};
struct SRiskKey
{//某一账户某种风险的唯一性标识
	RiskIndicatorType	nRiskIndicatorID;	//风险指标类型
	std::string			strInstrument;      //合约名称，主要是针对最大持仓之类的
	PositionDetailKey   PostionDetail; //
	SRiskKey()
	{
		memset(this,0,sizeof(*this));
		nRiskIndicatorID = RI_Count;
		strInstrument.empty();		
	}
	~SRiskKey()
	{
		
	}
/**	SRiskKey(RiskIndicatorType	nID, std::string& str)
	{
		nRiskIndicatorID = nID;
		strInstrument    = str;		
	}*/
	SRiskKey(RiskIndicatorType	nID, std::string& str, PositionDetailKey& p)
	{
		nRiskIndicatorID = nID;
		strInstrument    = str;
		PostionDetail = p;			
	}
	bool operator< (const SRiskKey& r) const
	{
		int irlt;
		if(nRiskIndicatorID < r.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > r.nRiskIndicatorID)
			return false;

		irlt=strncmp(strInstrument.c_str(),r.strInstrument.c_str(),sizeof(strInstrument));
		if(irlt<0) return true;
		if(irlt>0) return false;

		if(PostionDetail < r.PostionDetail)
			return true;

		
		return false;
	}

};

struct SProduct
{
	InstrumentIDType ProductID;//品种ID
};
struct SLimitTrade
{
	int		nUserID;    //交易员ID
	bool	bLimitTrade;//是否限制交易
};
struct SManualVerify
{
	int		nUserID;    //交易员ID
	bool	bManualVerify;//是否手动审核
};
enum MsgStatusType
{
	MsgStatus_New,				//服务器记录：发生
	MsgStatus_ChangeLevel,		//服务器记录：等级变更
	MsgStatus_Delete,			//服务器记录：消失

	MsgStatus_Appoint,			//手动操作，委派给交易员
	MsgStatus_Finished,			//手动操作，风险事件解决记录
	MsgStatus_ForceClose,		//手动操作，执行手动强平记录
};
enum RiskEventType
{
	RiskEvent_History,
	RiskEvent_Realtime,
};
struct RiskEventQueryKey
{
	TradeAccountIDType		nTradeAccountID;
	DateType szStartDate;	//起始时间
	DateType szEndDate;
};
//风险事件数据结构
struct RiskEvent
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	RiskTime				cTimeBegin;		//该风控事件有效的起止时间；风险事件以nRiskIndicatorID，cTimeBegin，cTimeEnd作为唯一标识
	RiskTime				cTimeEnd;		// 
	RSTIME					lEventTime;			
	BrokerIDType			BrokerID;			//对主账号风控时候，这个变量才有用
	TradeInvestorIDType		nTradeInvestorID;	
	RiskIndicatorType		nRiskIndicatorID;
	InstrumentIDType		InstrumentID;
	RiskLevelType			nRiskLevelID;
	double					dblIndicatorValue;			//触发值
	double					dblIndicatorCurrentValue;	//当前值
	MsgStatusType			nMsgSendStatus;
	RiskEventType			nIsValid;					//0 是历史风险事件， 1 实时风险事件
	int						nResponseType;				//风险动作，主要客户端用于震动鸣笛

	double					dblIndicatorValue2;		 // 有些指标需要两个触发值；(比如保证金限制,这个字段代表“实际保证金”;限制亏损里面"亏损总额"
	double					dblIndicatorCurrentValue2;//当前值

	bool operator<(const RiskEvent&	src) const
	{
		return lEventTime<src.lEventTime;
	}
};
//风险数值更新
struct RiskEventUpdate
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	double					dblIndicatorCurrentValue;//当前值
};

enum EventHandlingActionType
{
	Action_ServerRecord,	//服务器记录
	Action_ClientRecord,	//手动记录
	Action_ForceClose,		//手动强平
};
//风险数据处理表数据结构
struct RiskEventHandling
{
	EventHandlingIDType		nRiskEventHandlingID;
	RSTIME					lHandlingTime;
	TradeInvestorIDType		nTradeInvestorID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;			//客户端记录，处理者
	USERNAMESTRING			szAccepter;			//客户端记录，受理者
	EventHandlingActionType	nAction;			//服务器记录，手动记录，手动强平
	MsgStatusType			nProcessStatus;		//发生，处理中，消失
	CommentType				szContent;			//客户端记录，备注
	bool operator<(const RiskEventHandling&	src) const
	{
		return lHandlingTime<src.lHandlingTime;
	}
};

struct EventLevelID
{
	RiskEventIDType EventID;
	RiskLevelType	LevelID;
};
struct RiskEventLevelID
{
	RiskEventIDType EventID;
	RiskLevelType	LevelID;
	double			RiskVal;
};
//事件消息模版结构
struct EventMessageTemplate
{
	RiskIndicatorType		nRiskIndicatorID;
	RiskLevelType			nRiskLevelID;
	MsgTitle				szTitle;
	MsgContent				szContent;
};
//风险指标定义结构
struct RiskIndicator
{
	RiskIndicatorType	nRiskIndicatorID;
	INDICATORNAMESTRING	szName;
	RiskItemDesc		szAlgorithmDesc;
	char				szTypeName[40];//公司级风控，合同级风控类别名字
};

//动作定义结构
struct RiskResponse
{
	RiskWarningType		nResponseType;//某一个具体动作的ID，比如鸣笛；震动
	RESPONSENAMESTRING	szName;       //具体文字说明
	RiskItemDesc		szDesc;
};
////////////////////////////////////////////////////////////////////////////
//风控用到的结构
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
struct MsgSendStatus
{
	int nMessageID;
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
	int nSendStatus;	// 消息发送状态 0 未发送 1 已发送
};

struct TargetAccount
{
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
};
struct MessageQryCondition
{
	RiskMgmtUserIDType nRiskMgmtUserID;	//消息创建者ID
	DateType szCreateDate;				//创建时间，格式为 YYYY-MM-DD
};


//基金净值强平参数设置
struct ForceCloseParam
{
	int nTradeAccountID;		//交易员ID
	int nForceCloseInd;			//强平指标
	float fThreshold;			//触发阀值
	int nCloseStrategy;			//平仓策略 0，全部平仓
	int nForceClosePriceOption;	//强平价位 0，市价
};

struct SRiskControl
{
	RiskWarningType			nResponseType;//动作类型
	std::set<std::string>	setInstrument; //该动作限制的合约
	PositionDetailKey   PostionDetail;

	int					nEventID;//风险事件ID
	RiskIndicatorType	nRiskIndicatorID;		//触发的风险指标类型
	RiskLevelType	    nRiskLevelID;			//触发的风险等级
	SRiskControl()
	{
		nResponseType = WarningType_None;
	}
};
//未审核单审核结构
struct  SVerisyOrder
{
	OrderKey					orderKey;							//未审核单
	int							nVerifyUser;						//审核员ID；（应该就是风控员）		
	int							nResult;							//审核结果 1通过；0没通过;-1未审核
};
//报单审核查询条件
struct SQueryOrder
{
	SQueryOrder()
	{
		memset(this, 0 ,sizeof(*this));
	}
	DateType				szVerifyDate;		//格式2013-08-06			如果空，则全部
	CommonAccountType       Account;			//交易员账户				如果空，则全部
	bool					bIsProduct;			//true:代表品种； false:代表合约
	CommonInstrumentIDType  InstrumentID;       //品种ID 或者 合约ID		如果空，则全部
	int						nResult;            //审核结果 1通过；0没通过；-1待审核;-2全部
};
//报单审核查询结果
struct SQueryOrderResult
{
	PlatformStru_OrderInfo order;				//报单结构
	int					   nVerifyUser;	        //审核员ID
	CommonAccountType	   VerifyUserName;		//审核员名字；（应该就是风控员）
	char			       szVerifyDate[25];    //审核日期 
	int					   nResult;             //审核结果
};
//强平记录查询条件
struct ForceCloseCondition
{
	ForceCloseCondition()
	{
		memset(this, 0 ,sizeof(*this));
	}
	DateType				szInsertDate;		//格式2013-08-06			如果空，则全部
	CommonAccountType       Account;			//交易员账户				如果空，则全部
	bool					bIsProduct;			//true:代表品种； false:代表合约
	CommonInstrumentIDType  InstrumentID;       //品种ID 或者 合约ID		如果空，则全部
	int						nForceCloseType;    //1为自动强平，0为手动  -2为全部
	CommonAccountType		TradeInvestorName; //风控员账户				如果空，则全部

};
//强平记录查询条件返回结构
struct ForceCloseResult
{
	ForceCloseResult()
	{
		memset(this, 0 ,sizeof(*this));
	}
	PlatformStru_OrderInfo  order;
//	PlatformStru_InputOrder order;            //强平下单结构
//	int						nFrontID;		  //
//	int						nSessionID;
//	char					szOrderRef[13];
	bool					bForceCloseType;  //true为自动强平，false为手动
	CommonAccountType		RiskerName;	  //风控员名字
	char					szInsertDate[22];		//格式2013-08-06 11:08:08	


};
/*示例代码
int nCount = 2;
SAdminQuery *sAdminQuery = (SAdminQuery*)malloc(sizeof(SAdminQuery)+ sizeof(CommonAccountType)*nCount) ;
strcpy(sAdminQuery->szDateBegin, "2011-01-01");
strcpy(sAdminQuery->szDateEnd, "2013-08-13");
sAdminQuery->nCount = nCount;
strcpy(sAdminQuery->strAccount[0], "0001");
strcpy(sAdminQuery->strAccount[1], "0002");	

if( !CTCPDataService::GetInstance()->SendData(Cmd_RM_QryHistoryFundInfo_Req, (void*)sAdminQuery, sizeof(SAdminQuery)+ sizeof(CommonAccountType)*2, 0, false))
{
std::string strError = CTCPDataService::GetInstance()->GetLastError();
MessageBox(strError.c_str(), "提示", MB_ICONERROR);
return;
}
*/
//管理终端查询（历史资金，历史持仓，历史报单，历史出入金）
struct SAdminQuery
{	
	DateType				szDateBegin;	  //格式2013-08-06	
	DateType				szDateEnd;			
	int						nCount;				 //交易员账户数目
	CommonAccountType		strAccount[1];       //交易员账号ID：以分号分割	
};
//手动强平结构
struct SManualForceClose
{
	PlatformStru_InputOrder  nInputOrder;//InvestorID 字段填写的交易员字符串名称
    int nFrontID;//风控员的Frontid（风控员的ID），session， orderref(自动递增，风控客户端维护)
    int nSessionID;
    char szOrderRef[13];
	char szReason[256];//强平的原因（哪个风控员下手动强平指令）
	char szRiskerName[256];//风控员的名字
};
//风控客户端撤单命令结构
struct SRiskInputOrderAction
{
	PlatformStru_InputOrderAction  nInputOrderAction; 
	int nRequestID;//暂时没有用到	
	char nsActionReson[256];//撤单原因
	char szRiskerName[256];//风控员的名字
};
//下单限制结构，用于通讯给交易执行
struct SRiskControl2Execute
{	
	CommonAccountType		Investor;	  //交易员ID
	RiskWarningType			nResponseType;//动作类型
	CommonInstrumentIDType	Instrument; //该动作限制的合约
	PositionDetailKey		PostionDetail;

	int						nEventID;//风险事件ID
	RiskIndicatorType		nRiskIndicatorID;		//触发的风险指标类型
	RiskLevelType			nRiskLevelID;			//触发的风险等级
};
//////////////////////////////////////////////////////////////////////////////
//账户交易合约申请
struct  STradeForbidRequest
{	
	RiskIndicatorType		rType;
	CommonAccountType       Account;	
	char					szRiskerName[256];//风控员的名字
	char					szInsertDate[22];		//格式2013-08-06 11:08:08	

	RiskProductID			ProductID;			//品种名称	
	char					cInstruments[300];  //交易合约限制；后面的品种名称可以用于赋值“品种”
	double					dbMarginDividDynamic;//保证金/动态权益
	double					dbMarginUse;		 //使用保证金
	double					dbLossAmount;		//亏损总额
	double					dbLossPercent;      //亏损比例

	STradeForbidRequest()
	{
		memset(this,0,sizeof(*this));
	}
};
//查询账户被限制情况
struct SQueryTradeRistrict
{	
	DateType				szDateBegin;	  //格式2013-08-06	
	DateType				szDateEnd;			
	CommonAccountType		strAccount;       //交易员账号ID
};
#pragma pack(pop)