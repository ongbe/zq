#pragma  once
#include "string.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\tools_util.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#pragma warning(disable : 4996)
//请在这个头文件中定义服务器端和客户端共用的数据结构体
#pragma pack(push)
#pragma pack(1)


//存放各模块共有的数据结构体
typedef char _USERSTRING[21];
typedef char _PASSWORDSTRING[33];
typedef char _ExchangDateTimeType[18];
typedef char _ExchangTimeType[9];
typedef char _ExchangDateType[9];
typedef char _BrokerIDType[11];
typedef char _TraderAccount[13];
typedef char _SystemName[41];
typedef char _UserIDType[21];
typedef char _InstrumentID[31];
typedef char _SettlementDate[256];
typedef char _TradeIDType[21];

enum eBROKERTYPE 
{
    BROKER_TYPE_TRADE,//交易
    BROKER_TYPE_QUOT  //行情
};

//删除经纪公司时，服务器组、委托交易账号的brokerid为-1
#define TRADEACCOUNT_BROKERIDORSERVERID_NULL  -1 

//经纪公司
struct BrokerInfo 
{
	int nBrokerID;
	_BrokerIDType szBrokerCode;
	char szCompany[65];

    int nType;  //0 交易使用 1 行情使用
    char	szAccount[21];
    _PASSWORDSTRING	szPassword;  

    bool operator== (const BrokerInfo& r) const
    {
        int irlt;

        irlt=strncmp(szBrokerCode,r.szBrokerCode,sizeof(szBrokerCode));
        if(irlt!=0) return false;

        irlt=strncmp(szCompany,r.szCompany,sizeof(szCompany));
        if(irlt!=0) return false;

        irlt=strncmp(szAccount,r.szAccount,sizeof(szAccount));
        if(irlt!=0) return false;

        irlt=strncmp(szPassword,r.szPassword,sizeof(szPassword));
        if(irlt!=0) return false;

        if(nType!=r.nType)
            return false;

        return true;
    }
};

//服务器组
struct ServerGroup
{
	int nServerID;
	char szServerName[21];
	int nBrokerID;
	int nIsSettle;

    bool operator== (const ServerGroup& r) const
    {
        int irlt;

        irlt=strncmp(szServerName,r.szServerName,sizeof(szServerName));
        if(irlt!=0) return false;

        if(nBrokerID!=r.nBrokerID)
            return false;

        if(nIsSettle!=r.nIsSettle)
            return false;

        return true;
    }
};

//服务器地址
struct ServerAddress
{
    int nAddrID;	
    int nServerID;
	char szIP[128];
	int nPort;

    bool operator== (const ServerAddress& r) const
    {
        int irlt;

        irlt=strncmp(szIP,r.szIP,sizeof(szIP));
        if(irlt!=0) return false;

        if(nPort!=r.nPort)
            return false;

        if(nServerID!=r.nServerID)
            return false;

        return true;
    }
};

//行情服务器地址
struct ServerAddressQuot
{
    int nAddrID;
    _BrokerIDType szBrokerCode;
    char szIP[128];
    int nPort;

    bool operator== (const ServerAddressQuot& r) const
    {
        int irlt;

        irlt=strncmp(szBrokerCode,r.szBrokerCode,sizeof(szBrokerCode));
        if(irlt!=0) return false;

        irlt=strncmp(szIP,r.szIP,sizeof(szIP));
        if(irlt!=0) return false;

        if(nPort!=r.nPort)
            return false;

        return true;
    }
};


//委托交易账号
struct TradeAccount
{
	int nTradeAccountID;
	_TraderAccount szTradeAccount;
	_PASSWORDSTRING szPwd;
	int nBrokerID;
	int nServerGroupID;
    double dProfit;//没有
	char szAlias[21];
	char szRealName[21];
    char szContact[41]; 
	int  nIDCardType;
	char szIDCardNo[21];	

    bool operator== (const TradeAccount& r) const
    {
        int irlt;

        irlt=strncmp(szTradeAccount,r.szTradeAccount,sizeof(szTradeAccount));
        if(irlt!=0) return false;

        irlt=strncmp(szPwd,r.szPwd,sizeof(szPwd));
        if(irlt!=0) return false;

        irlt=strncmp(szAlias,r.szAlias,sizeof(szAlias));
        if(irlt!=0) return false;

        irlt=strncmp(szRealName,r.szRealName,sizeof(szRealName));
        if(irlt!=0) return false;

        irlt=strncmp(szContact,r.szContact,sizeof(szContact));
        if(irlt!=0) return false;

        irlt=strncmp(szIDCardNo,r.szIDCardNo,sizeof(szIDCardNo));
        if(irlt!=0) return false;

        if(nBrokerID!=r.nBrokerID)
            return false;

        if(nServerGroupID!=r.nServerGroupID)
            return false;

        if(dProfit!=r.dProfit)
            return false;

        if(nIDCardType!=r.nIDCardType)
            return false;

        return true;
    }
};

//委托交易账号与服务器组关系表
struct TradeAccountAndServerGroup
{
	int nTradeAccountID;		//账号ID
	int nServerGroupID;	        //服务器组ID
};

//委托交易账号与组织机构关系表
struct TradeAccountAndOrg
{
    int nTradeAccountID;		//账号ID
    int nOrgID;	                //组织机构ID
};

enum eUSERANDORGRELATIONTYPE
{
    USER_ORG_RELATION_ATTACH,   //从属
    USER_ORG_RELATION_MONITOR,  //监控
    USER_ORG_RELATION_ALL       //从属和监控

};

#define ORGNIZATION_ROOT 10846209
//组织机构
struct Organization
{
	int nAssetMgmtOrgID;
	char szName[21];
	int nUpperLevelOrgID;
	int nScaleLevel;
	double dLongitude;
	double dLatitude;

    bool operator== (const Organization& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        if(nUpperLevelOrgID!=r.nUpperLevelOrgID)
            return false;

        if(nScaleLevel!=r.nScaleLevel)
            return false;

        if(dLongitude!=r.dLongitude)
            return false;

        if(dLatitude!=r.dLatitude)
            return false;

        return true;
    }
};

enum UserType
{	
	USER_TYPE_MANAGE,		//管理员
	USER_TYPE_RISK_CONTROL,	//风控员
	USER_TYPE_TRADER,		//交易员
	USER_TYPE_VIEWER,		//展现员
	USER_TYPE_SLAVESERVER,  //从服务器用户
	USER_TYPE_MASTERSERVER,	//主服务器
	USER_TYPE_INVALID		//无效的用户类型
};
#define  SLAVE_USER_ID 1
// enum eUSERASSIGN
// {
//     UNASSIGN,  //分配组织机构
//     ASSIGN //未分配组织机构
// };

//基础数据表加入是否删除的列
enum eDELETE
{
    UNDELETE_FLAG, //未删除
    DELETE_FLAG    //删除
};

#define USER_ADMIN 10846209
//用户结构
struct UserInfo
{
	int		nUserID;
	char	szAccount[21];
	_PASSWORDSTRING	szPassword;    
	UserType nUserType;				//账户类型
	int		nStatus;				//启用状态
	char	szAlias[21];
	char	szContactInfo[101];
    int     nAssign;                //（组织机构中）0 未分配 1 分配


    bool operator== (const UserInfo& r) const
    {
        int irlt;

        irlt=strncmp(szAccount,r.szAccount,sizeof(szAccount));
        if(irlt!=0) return false;

        irlt=strncmp(szPassword,r.szPassword,sizeof(szPassword));
        if(irlt!=0) return false;

        irlt=strncmp(szAlias,r.szAlias,sizeof(szAlias));
        if(irlt!=0) return false;

        irlt=strncmp(szContactInfo,r.szContactInfo,sizeof(szContactInfo));
        if(irlt!=0) return false;

        if(nUserType!=r.nUserType)
            return false;

        if(nStatus!=r.nStatus)
            return false;

        if(nAssign!=r.nAssign)
            return false;

        return true;
    }
};

//用户修改密码
struct UserPassword
{
    int     nUserID;
    _PASSWORDSTRING szOldPassword; //旧密码 
    _PASSWORDSTRING	szNewPassword; //新密码 
};

//用户状态启用
struct UserStatus
{
    int nUserID;
    int nStatus; //0 停用 1 启用
};

//用户与角色关系
struct UserRole
{
    int nUserID;
    int nRoleID;
};

//与数据库ID对应
enum ePrivilege
{
    //管理员权限
	ACCOUNT                                             =1, //账户
        ACCOUNT_MANAGE                                  ,//委托交易账号管理
            ACCOUNT_ADD                                 ,//增加账号
            ACCOUNT_MODIFY                              ,//修改账号
            ACCOUNT_DEL                                 ,//删除账号
            ACCOUNT_LOGIN                               ,//登录
        USER_MANAGE                                     =7,//系统用户管理
            USER_ADD                                    ,//增加用户
            USER_MODIFY                                 ,//修改用户
            USER_DEL                                    ,//删除用户
            USER_START                                  ,//交易员启用
            USER_INIT                                   ,//交易员初始化
            USER_TRADER_ASSIGN_ORGANIZATION             ,//交易员分配组织架构
            USER_TRADER_ASSIGN_PRODUCT                  ,//交易员分配理财产品
            USER_TRADER_ASSIGN_ACCOUNT                  ,//交易员分配委托交易账号
            USER_RISKER_SETUP_ORGANIZATION              ,//风控员组织机构设置
        FUND_MANAGE                                     =17,//交易员出入金管理
            FUND_OUT                                    ,//出金
            FUND_IN                                     ,//入金
    MANAGE                                              =20, //管理
        ORGANIZATION_MANAGE                             ,//组织机构管理
            ORGANIZATION_ADD                            ,//增加组织架构
            ORGANIZATION_MODIFY                         ,//修改组织架构
            ORGANIZATION_DEL                            ,//删除组织架构
            ORGANIZATION_ASSIGN_TRADER                  ,//组织机构分配交易员
        FIN_PRODUCT_MANAGE                              =26,//理财产品管理
            FIN_PRODUCT_ADD                             ,//增加理财产品
            FIN_PRODUCT_MODIFY                          ,//修改理财产品
            FIN_PRODUCT_DEL                             ,//删除理财产品
            FIN_PRODUCT_ASSIGN_TRADER                   ,//理财产品分配交易员
        BROKER_SERVER_MANAGE                            =31,//经纪公司服务器组管理
            BROKER_ADD                                  ,//增加经纪公司
            BROKER_MODIFY                               ,//修改经纪公司
            BROKER_DEL                                  ,//删除经纪公司
            SERVER_ADD                                  ,//增加服务器组
            SERVER_MODIFY                               ,//修改服务器组
            SERVER_DEL                                  ,//删除服务器组
            SERVERADDR_ADD                              ,//增加服务器地址
            SERVERADDR_MODIFY                           ,//修改服务器地址
            SERVERADDR_DEL                              ,//删除服务器地址
            BROKER_QUOT_MODIFY                          ,//修改行情经纪公司
            SERVERADDR_QUOT_ADD                         ,//增加行情服务器地址
            SERVERADDR_QUOT_MODIFY                      ,//修改行情服务器地址
            SERVERADDR_QUOT_DEL                         ,//删除行情服务器地址
    STRATEGY                                            =45, //策略
        STRATEGY_ADD                                    ,//增加交易策略
        STRATEGY_MODIFY                                 ,//修改交易策略
        STRATEGY_DEL                                    ,//删除交易策略
        STRATEGY_ASSIGN_ORGANIZATION                    ,//交易策略分配组织机构
    PRIVILEGE                                           =50, //权限
        PRIVILEGE_ROLE_ADD                              ,//增加角色
        PRIVILEGE_ROLE_MODIFY                           ,//修改角色
        PRIVILEGE_ROLE_DEL                              ,//删除角色
        PRIVILEGE_ROLE_ASSIGN_PRIVILEGE                 ,//角色分配权限
    SETTLEMENT                                          =55, //结算
        SETTLEMENT_MANUAL                                ,//当日数据结算
        SETTLEMENT_HISTORY                               ,//历史结算
        SETTLEMENT_AUTO_SETUP                            ,//自动结算时间设置        
    QUERY                                               =59, //查询
        QUERY_HISTORY_FUND                              ,//查询历史基金
        QUERY_HISTORY_POSITION                          ,//查询历史持仓
        QUERY_HISTORY_POSITION_DETAIL                   ,//查询历史持仓明细
        QUERY_HISTORY_TRADE                             ,//查询历史成交
        QUERY_HISTORY_ORDER                             ,//查询历史报单  
    CONTROL                                             =65, //控制
        CONTROL_SERVER_RESET                            ,//服务器重启
        CONTROL_TIME_SETUP                              ,//定时重启时间设置
        CONTROL_SERVER_QUOT_RESET                       ,//行情服务器重启
        CONTROL_TIME_QUOT_SETUP                         ,//行情服务器定时重启时间设置

    //风控员权限
	RISK_RISK                                           =70,//风控
        RISK_MANUAL_VERIFY                              ,//手动审核
        RISK_LIMIT_TRADE                                ,//限制交易
        RISK_FORCE_CLOSE                                ,//强平
        RISK_ADD                                        ,//增加组织机构风控方案
        RISK_MODIFY                                     ,//修改组织机构风控方案
        RISK_DEL                                        ,//删除组织机构风控方案
	    RISK_SWITCH                                     ,//启用开关
    RISK_QUERY                                          =78,//查询
        RISK_HISTORY_RISK_EVENT_QUERY                   ,//历史风险事件查询
        RISK_HISTROY_FUND_SUTTLE_QUERY                  ,//历史基金净值查询
        RISK_ORDER_VERIFY_RECORD_QUERY                  ,//报单审核记录查询
        RISK_FORCE_CLOSE_RECORD_QUERY                   ,//强平记录查询
        RISK_TRADER_BASE_INFO_QUERY                     ,//交易员基础信息查询

    // 用户消息权限	
    MESSAGE                                             =84, //消息
        MESSAGE_QUERY                                   ,//消息查询
        MESSAGE_SEND                                    ,//消息发送

    //后期添加
    SETTLEMENT_AUTO_IMMEDIATE                        ,//立即自动结算
};

//角色
struct RoleInfo
{
    int nRoleID;
    char szName[21];
    int nUserTypeID;

    bool operator== (const RoleInfo& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        if(nUserTypeID!=r.nUserTypeID)
            return false;

        return true;
    }
};
//用户类型与角色关系
struct UserTypeAndRoleRelation
{
    int nUserTypeID;
    int nRoleID;
};

//权限
struct Privilege
{
	int nPrivilegeID;
	char szName[31];
    int nUserTypeID;    //用户类型
    int nParentID;      //父节点    
    int nNodeLevel;     //几级节点

    bool operator== (const Privilege& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        if(nUserTypeID!=r.nUserTypeID)
            return false;

        if(nParentID!=r.nParentID)
            return false;

        if(nNodeLevel!=r.nNodeLevel)
            return false;

        return true;
    }
};

//角色与权限的关系
struct RoleAndPrivilegeRelation
{
    int nRoleID;
    int nPrivilegeID;
};

struct UserAndTradeAccountRelation
{
	int nUserID;
	int nTradeAccountID;
	int nCanInputOrder;	    //是否允许下单
	int nNeedVerify;		//下单是否需要验证
};

struct UserAndOrgRelation
{
	int nUserID;
	int nOrgID;
	int nRelationType;	// 0 从属(组织架构) 1 监控管理(用户组)
    int nUserType;
};

//理财产品结构
struct 	FinancialProduct
{
    int			nID;			                //理财产品ID
    char		szName[20];					    //理财产品名称    
    char 		szDesc[128];				    //理财产品描述

    bool operator== (const FinancialProduct& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        irlt=strncmp(szDesc,r.szDesc,sizeof(szDesc));
        if(irlt!=0) return false;

        return true;
    }
};

enum eTraderProRelationType
{
    TRADER_ASSGIN_PRODUCT_TYPE,   //交易员指定理财产品
    PRODUCT_ASSGIN_TRADER_TYPE   //理财产品指定交易员
};
//理财产品与交易员关系结构
struct 	TraderProRelation
{
    int 			nFinancialProductID;	//理财产品ID
    int 			nTraderID;				//交易员ID 
};

//交易策略结构
struct TradeStrategy
{
    int 		nID;			    //策略ID
    char 	    szName[21];	        //策略名称
    int         nFrequency;         //策略读取频率
    char	    szAddr[256];	    //策略文件绝对路径

    bool operator== (const TradeStrategy& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        irlt=strncmp(szAddr,r.szAddr,sizeof(szAddr));
        if(irlt!=0) return false;

        if(nFrequency!=r.nFrequency)
            return false;

        return true;
    }
};

//交易策略与组织机构关系结构
struct AssetOrgStrategyRelation
{
    int 		nStrategyID;			//策略ID
    int		    nAssetOrgID;		    //机构节点ID

    bool operator<(const AssetOrgStrategyRelation&	src) const
    {
        if(nAssetOrgID<src.nAssetOrgID)
            return true;
        else if(nAssetOrgID==src.nAssetOrgID)
        {
            if(nStrategyID<src.nStrategyID)
                return true;
        }
        return false;
    }
};

// 用户消息类型
enum enmMsgType 
{
	conManualMsg, conAutoMsg, conStrategyMsg
};

// 用户Send消息tcp传输结构体
struct UserSendMsgStru
{
	int		OwnerUserID;			// 消息发送人
	int		ExpiredDate;			// 消息到期日
	int		TargetUserCount;			// 消息接收人
	char	Title[32];				// 消息标题
	char	Msg[512];				// 消息正文
	int		TargetUsers[1];			// 目标用户ID组
};

// 用户Recv消息tcp传输结构体
struct UserMsgStru
{
	int		MsgID;					// 消息ID
	int		TargetMsgID;			// 目标消息ID
	int		MsgType;				// 消息类型
	int		TargetUserID;			// 消息接收人
	int		OwnerUserID;			// 消息发送人，如果是系统自动消息，则为0
	int		ExpiredDate;			// 消息到期日
	char	CreateDateTime[20];		// 消息创建日期时间，格式"yyyy-mm-dd HH:MM:SS"
	char	SendDateTime[20];		// 消息发送日期时间，格式"yyyy-mm-dd HH:MM:SS"
	char	Title[32];				// 消息标题
	char	Msg[512];				// 消息正文
};

// 用户消息结构
struct UserMsgMemStru 
{
	bool	IsOrigMsg;				// 是原消息还是目标消息，用来决定保存在哪个表中
									// 原消息保存在原消息表中，目标消息保存在目标消息表中
	bool	HasSend;				// 是否已经发送
	bool	HasRead;				// 是否已经阅读，保留暂时不用
	UserMsgStru msg;				// 消息结构体
};

// 用户消息状态结构
struct UserMsgStatusStru 
{
	int		TargetUserID;			// 用户ID
	int		TargetMsgID;			// 目标消息ID
};


///设置登陆时所需其他信息
struct sExchangeTimeInfo
{
	///获取到该时间信息的时间
	_ExchangTimeType	GetTime;
	///交易日
	_ExchangDateType	TradingDay;	
	///上期所时间
	_ExchangTimeType	SHFETime;
	///大商所时间
	_ExchangTimeType	DCETime;
	///郑商所时间
	_ExchangTimeType	CZCETime;
	///中金所时间
	_ExchangTimeType	FFEXTime;
};
//登陆请求
enum eClientType
{
	CLIENT_TYPE_ADMIN = 0, //管理客户端
	CLIENT_TYPE_TRADE,     //交易客户端
	CLIENT_TYPE_RISK,      //风控客户端
	CLIENT_TYPE_SHOW       //展现客户端
};
struct sLoginReq
{
	_USERSTRING msUserName;         //用户名
	_PASSWORDSTRING msPassWord;     //用户密码
	UserType meUserType;            //用户类型
	eClientType meClientType;       //客户端类型
};



//交易客户端登陆返回
struct sTraderLoginRsp
{
	int		mnUserID;                //用户ID   
	int     mnFrontID;               //前置ID
	int     mnSessionID;             //会话ID
	int     mnMinOrderRef;           //最小的Orderref值		
	_SystemName	        mSystemName; ///交易系统名称
	_BrokerIDType       mBrokerID;   //保留使用，填空
	_ExchangTimeType	mLoginTime;  ///登录成功时间
	_ExchangDateType	mTradingDay; ///交易日	
	_ExchangTimeType	mSHFETime;   ///上期所时间
	_ExchangTimeType	mDCETime;    ///大商所时间
	_ExchangTimeType	mCZCETime;	 ///郑商所时间	
	_ExchangTimeType	mFFEXTime;   ///中金所时间

};
//登陆返回
struct sLoginRsp
{
	int		mnUserID;                //用户ID   
	int     mnFrontID;               //前置ID
	int     mnSessionID;             //会话ID
	int     mnMinOrderRef;           //最小的Orderref值
	int     mnPrivilegeLen;          //权限集合的长度
	int     mnPemission[1];          //权限集合

};
enum eSocketStatus
{
	SocketStatus_Validated = 0,     //有效的socket
	SocketStatus_Connected,         //建立连接，但没有通过验证
	SocketStatus_Exception,         //异常断开
	SocketStatus_Timeout,           //超时
	SocketStatus_Logoff             //注销

};

enum RiskLimitOpt
{
	NO_FORBID,		//不限制
	FORBID_ORDER,	//禁止交易
	FORBID_OPEN_ORDER,	//禁止开仓
};

enum RiskLimitDirection
{
	DIRECTION_NO_FORBID,		//无限制
	DIRECTION_FORBID_BUY,		//禁止买
	DIRECTION_FORBID_SELL,		//禁止卖
	DIRECTION_FORBID_BOTH_SIDE,	//双向都禁止
};

enum RiskLimitRange
{
	FORBID_NONE,				//无限制
	FORBID_ALL_PRODUCT,			//所有品种限制
	FORBID_SINGLE_PRODUCT,		//单品种限制
	FORBID_SINGLE_INSTRUMENT,	//单合约限制
};

//风险事件产生的交易限制
struct RiskLimit
{
	RiskLimitOpt opt;
	RiskLimitDirection direction;
	RiskLimitRange range;
};

enum StrategyOrderLimit
{
	STRATEGY_NO_FORBID,			//无限制
	STRATEGY_FORBID_BUY,		//禁止买
	STRATEGY_FORBID_SELL,		//禁止卖
	STRATEGY_FORBID_BOTH_SIDE,	//双向都禁止
};

struct InputOrderKey
{
	int nFrontID;
	int nSessionID;
	char szOrderRef[13];

	bool operator< (const InputOrderKey& r) const
	{
		int irlt;		
		if(nFrontID<r.nFrontID)
			return true;
		else if(nFrontID>r.nFrontID)
			return false;

		if(nSessionID<r.nSessionID)
			return true;
		else if(nSessionID>r.nSessionID)
			return false;

		irlt=strncmp(szOrderRef,r.szOrderRef,sizeof(szOrderRef));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}
};

//struct VerifyOrderRtn
//{
//	OrderKey orderKey;
//	bool bRet;
//};

//enum InputOrderStatus
//{
//	InputOrder_Receive,	//接收报单指令
//	InputOrder_Verifying,	//手动审核中
//	InputOrder_Verify_End,	//审核完毕
//	InputOrder_CTP_Received,	//ctp服务器已接收报单
//	InputOrder_Part_Transacted,	//部分成交
//	InputOrder_Complete_Transacted,	//全部成交
//};

struct SettleTradeKey
{
	SettleTradeKey()
	{
		memset(this,0,sizeof(*this));		
	};

	SettleTradeKey(const char* nInvestorID,const char* nTradeID)
	{
		memset(this,0,sizeof(*this));

		if(nInvestorID != NULL && nTradeID != NULL)
		{
			strcpy_s(InvestorID,sizeof(InvestorID),nInvestorID);
			strcpy_s(TradeID,sizeof(TradeID),nTradeID);

		}


	};


	//SettleTradeKey& operator= (const SettleTradeKey& other)
	//{
	//	if (this == &other)
	//		return *this;

	//	memcpy((char*)this,(char*)&other,sizeof(*this));
	//	return *this;
	//};

	bool operator< (const SettleTradeKey& r) const
	{
		int irlt;
		irlt=strncmp(InvestorID,r.InvestorID,sizeof(InvestorID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const SettleTradeKey& r) const
	{
		int irlt;
		irlt=strncmp(InvestorID,r.InvestorID,sizeof(InvestorID));
		if(irlt!=0) return false;	

		irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
		if(irlt!=0) return false;

		return true;
	}

	bool operator> (const SettleTradeKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	///投资者代码
	_TraderAccount	InvestorID;
	///成交编号
	_TradeIDType	TradeID;
};

struct RateKey
{
	RateKey()
	{
		memset(this,0,sizeof(*this));		
	};

	RateKey(const char* nAccountID,const char* nInstrumentID,const char* nBrokerID)
	{
		memset(this,0,sizeof(*this));

		if(nAccountID != NULL && nInstrumentID != NULL && nBrokerID != NULL)
		{
			strcpy_s(AccountID,sizeof(AccountID),nAccountID);
			strcpy_s(InstrumentID,sizeof(InstrumentID),nInstrumentID);
			strcpy_s(BrokerID,sizeof(BrokerID),nBrokerID);
		}


	};


	RateKey& operator= (const RateKey& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const RateKey& r) const
	{
		int irlt;
		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const RateKey& r) const
	{
		int irlt;
		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt!=0) return false;	

		irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
		if(irlt!=0) return false;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt!=0) return false;

		return true;
	}

	bool operator> (const RateKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	///合约代码
	_InstrumentID	InstrumentID;
	///投资者代码
	_TraderAccount	AccountID;	
	///经纪公司代码
	_BrokerIDType	BrokerID;

};
struct QryHisRateCondition
{
	RateKey conditionKey;
	char loadDate[11];
};
struct CommissionMarginRate
{
	CommissionMarginRate()
	{
		memset(this,0,sizeof(*this));
		LongMarginRatioByMoney=util::GetDoubleInvalidValue();
		LongMarginRatioByVolume=util::GetDoubleInvalidValue();
		ShortMarginRatioByMoney=util::GetDoubleInvalidValue();
		ShortMarginRatioByVolume=util::GetDoubleInvalidValue();
		OpenRatioByMoney=util::GetDoubleInvalidValue();
		OpenRatioByVolume=util::GetDoubleInvalidValue();
		CloseRatioByMoney=util::GetDoubleInvalidValue();
		CloseRatioByVolume=util::GetDoubleInvalidValue();
		CloseTodayRatioByMoney=util::GetDoubleInvalidValue();
		CloseTodayRatioByVolume=util::GetDoubleInvalidValue();

	}
	///经纪公司代码
	_BrokerIDType	BrokerID;
	///投资者代码
	_TraderAccount	AccountID;	
	///合约代码
	_InstrumentID	InstrumentID;
	///多头保证金率
	double	LongMarginRatioByMoney;
	///多头保证金费
	double	LongMarginRatioByVolume;
	///空头保证金率
	double	ShortMarginRatioByMoney;
	///空头保证金费
	double	ShortMarginRatioByVolume;
	///开仓手续费率
	double	OpenRatioByMoney;
	///开仓手续费
	double	OpenRatioByVolume;
	///平仓手续费率
	double	CloseRatioByMoney;
	///平仓手续费
	double	CloseRatioByVolume;
	///平今手续费率
	double	CloseTodayRatioByMoney;
	///平今手续费
	double	CloseTodayRatioByVolume;
};
struct SettlementPriceField
{
	///合约代码
	_InstrumentID	InstrumentID;
	///交易所代码
	char	ExchangeID[9];
	///产品代码
	char	ProductID[64];
	///结算价
	double	SettlementPrice;
	///昨结算价
	double	LastSettlementPrice;
	//结算日期
	char	SettlementDate[9];
};

enum SettlementOperationActionType
{
	Action_AddRecord,	    //增加记录
	Action_ModifyRecord,	//修改记录
	Action_DeleteRecord,	//删除记录
};
enum SettlementBusinessType
{

	Business_SettlementPrice,
	Business_CommissionRate,
	Business_MarginRate,
	Business_Trade,
	Business_PositionDetail,
	Business_Position,
	Business_Fund,
};
//结算员操作动作记录
struct SettlementOperation
{
	int		                      nUserID;//结算员ID
	char 	                      szTraderID[13];//交易员ID	
	char	                      szTradingDay[11];//结算日期
	char                          szInstrumentID[31];//合约ID
	SettlementBusinessType        nBusinessType;//操作业务类型
	SettlementOperationActionType           nActionType;//动作类型
	char                          szOperationItem[21];//操作项
	double                        dOriginalContent;//原来的内容
	double                        dNewContent;//修改后的内容
};

enum eSettlementStatus
{
	No_Settlementing = 0, //非结算状态
	Manul_Settlementing,  //正在手动结算
	Auto_Settlementing    //正在自动结算
};

enum EnumAccountStatus
{    
	ACCOUNT_STATUS_UnInit							=      0,            //未初始化
	ACCOUNT_STATUS_CannotConnect					=      1,            //连接不上
	ACCOUNT_STATUS_Connected						=      2,            //已连接
	ACCOUNT_STATUS_Disconnected						=      3,            //断开连接	
	ACCOUNT_STATUS_Login							=      4,            //登录成功
	ACCOUNT_STATUS_LoginFailure						=      5,            //登录失败
	ACCOUNT_STATUS_TradeSettlementInfoConfirming    =	   6,			 //正在进行交易结算结果确认
	
};
enum eUserInitStatus
{
	USER_STATUS_UnInit=0,                                        //未初始化
	USER_STATUS_InitQryInstrumentsReady=1,					     //合约数据查询完成
	USER_STATUS_InitQryRatesReady =2,				        	 //费率和保证金率查询完成
	USER_STATUS_InitQryTradesReady =4,				        	 //成交查询完成	
	USER_STATUS_InitQryOrdersReady =8,				        	 //报单查询完成
	USER_STATUS_InitQryFundInfoReady =16,				         //日初的资金查询完成
	USER_STATUS_InitQryPositionReady =32,				         //日初的持仓查询完成
	USER_STATUS_InitQryPositionDetailReady =64,				     //日初的持仓明细查询完成
	USER_STATUS_Inited=127,				        	             //初始化完成
	USER_STATUS_InitFailed=128,			        	             //初始化失败

};

//交易所状态
enum eExchangeStatus
{
	ExchangeStatus_PreOpen = 0,  //开盘前
	ExchangeStatus_ContinueTrade,//连续交易
	ExchangeStatus_UnableTrade   //非交易

};
enum eTradeDataStatus
{
	TradeDataStatusInitialing = 0,      //模块初始化状态，所有对外接口均不可用
	TradeDataStatusLastDaySettlemtNotOK,//没有取到交易日，但是数据库中有待结算的交易日，可作该交易日的盘中结算和历史结算
    TradeDataStatusNoTradingDay,        //没有取到交易日，数据库中也没有待结算的交易日。只能做历史结算
	TradeDataStatusInitFail,            //初始化失败状态，当昨仓中有过期合约等原因导致的初始化失败
	TradeDataStatusTrading,             //交易数据管理模块正处于交易状态 程序默认进入交易状态
	TradeDataStatusSettlemtIniting,     //结算初始化状态
	TradeDataStatusSettlemting,         //结算初始化完成后，进入结算状态
	TradeDataStatusSettlemtEnd          //结算完成
};

enum eSettlementLoadOption
{
	SettlementLoadOption_TradingDataWithTodayDB = 0, //从盘中数据库中的原始数据开始结算
	SettlementLoadOption_TradingDataWithCTPSett,   	 //从盘中数据库中的原始合约费率 + CTP结算成交数据
	SettlementLoadOption_TradingDataWithHistory   	 //从数据库中读历史数据进行结算

};

enum eDataReady
{
	DataReday_NoReady = 0,
	DataReady_Ready
};

enum eCommissionMarginReady
{
	CommissionMargin_NoReady = 0,
	CommissionMargin_ComissionReady = 0x0001,
	CommissionMargin_MarginReady = 0x0002,
	CommissionMargin_ComissionAndMarinReady = 0x0003
};

//struct sBrokerAccount
//{
//	///经纪公司代码
//	_BrokerIDType	BrokerID;
//	///委托交易账号代码
//	_TraderAccount	AccountID;	
//};
///标示一个报单记录
struct BrokerAccountKey
{
	BrokerAccountKey()
	{
		memset(this,0,sizeof(*this));		
	};

	BrokerAccountKey(const char* npsBrokerID,const char* npsAccountID)
	{
		memset(this,0,sizeof(*this));

		if(npsAccountID != NULL && npsBrokerID != NULL)
		{
			//strcpy_s(BrokerID,sizeof(BrokerID),npsBrokerID);
			//strcpy_s(AccountID,sizeof(AccountID),npsAccountID);
			//长度超出，崩溃
			strncpy_s(BrokerID, sizeof(BrokerID),npsBrokerID, sizeof(BrokerID)-1);
			strncpy_s(AccountID, sizeof(AccountID),npsAccountID, sizeof(AccountID)-1);
		}
		

	};


	BrokerAccountKey& operator= (const BrokerAccountKey& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const BrokerAccountKey& r) const
	{
		int irlt;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt<0) return true;
		if(irlt>0) return false;


		return false;
	}

	bool operator== (const BrokerAccountKey& r) const
	{
		int irlt;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt!=0) return false;

		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt!=0) return false;		

		return true;
	}

	bool operator> (const BrokerAccountKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	///经纪公司代码
	_BrokerIDType	BrokerID;
	///投资者代码
	_TraderAccount	AccountID;	

};

struct sAccountStatus
{
	BrokerAccountKey mBrokerAccount;
	eCommissionMarginReady meCommissionAndMarginReady; //费率是否准备好
	EnumAccountStatus meStatus;                        //CTP连接状态
	double ldbDuration;                                //进入状态的时间长
};
struct sUserInitStatus
{
	_UserIDType	szUserAccount;              
	eUserInitStatus meStatus;                //用户的初始化状态
	EnumAccountStatus meCTPStatus;           //该用户关联的委托交易账号的ctp状态
	double ldbDuration;                      //进入状态的时间长
};



//报盘存取数据库用数据结构
typedef char OTACCOUNTTYPE[16];
typedef char DateType[21];
typedef char ORDERSYSID[21];
typedef char TExchangeIDType[9];
struct SOrderTransfer
{
	OTACCOUNTTYPE szInvestorID;//交易员ID
	int  nFrontID;
	int	 nSessionID;
	int  nOrderRef;
	
	OTACCOUNTTYPE szBrokerID;//账号经纪公司ID
	OTACCOUNTTYPE szAccountID;//账号ID
	int	 nCTPOrderRef;
	int  nCTPFrontID;
	int  nCTPSessionID;
	DateType                szUpdateDate;
	ORDERSYSID  orderSysID;   ///报单编号
	TExchangeIDType	ExchangeID; //交易所ID

	int						UserForceClose;	//用户强平标志 风控客户端强平时候为1，不然为0
	char			        ForceCloseReason; //报单类型   风控客户端强平时候为THOST_FTDC_FCC_ForceClose,不然为‘0’
	SOrderTransfer()
	{
		memset(this,0,sizeof(*this));		
	}
	SOrderTransfer& operator= (const SOrderTransfer& order)
	{
		strcpy(szInvestorID, order.szInvestorID);
		nFrontID = order.nFrontID;
		nSessionID = order.nSessionID;
		nOrderRef = order.nOrderRef;
		strcpy(szBrokerID, order.szBrokerID);
		strcpy(szAccountID, order.szAccountID);
		nCTPOrderRef = order.nCTPOrderRef;
		nCTPFrontID = order.nCTPFrontID;
		nCTPSessionID = order.nCTPSessionID;
		strcpy(szUpdateDate, order.szUpdateDate);
		strcpy(orderSysID, order.orderSysID);
		strcpy(ExchangeID, order.ExchangeID);
		UserForceClose		= order.UserForceClose;
		ForceCloseReason	= order.ForceCloseReason;

		return *this;
	}
	bool operator<(const SOrderTransfer item) const
	{

		int i = strcmp(szInvestorID, item.szInvestorID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(nFrontID < item.nFrontID)
			return true;
		else if(nFrontID > item.nFrontID)
			return false;

		if(nSessionID < item.nSessionID)
			return true;
		else if(nSessionID > item.nSessionID)
			return false;

		if(nOrderRef < item.nOrderRef)
			return true;
		else if(nOrderRef > item.nOrderRef)
			return false;

		i = strcmp(szBrokerID, item.szBrokerID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		i = strcmp(szAccountID, item.szAccountID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(nCTPOrderRef < item.nCTPOrderRef)
			return true;
		else if(nCTPOrderRef > item.nCTPOrderRef)
			return false;

		if(nCTPFrontID < item.nCTPFrontID)
			return true;
		else if(nCTPFrontID > item.nCTPFrontID)
			return false;

		if(nCTPSessionID < item.nCTPSessionID)
			return true;
		else if(nCTPSessionID > item.nCTPSessionID)
			return false;

		i = strcmp(szUpdateDate, item.szUpdateDate);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		i = strcmp(orderSysID, item.orderSysID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		i = strcmp(ExchangeID, item.ExchangeID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(UserForceClose < item.UserForceClose)
			return true;
		else if(UserForceClose > item.UserForceClose)
			return false;

		if(ForceCloseReason < item.ForceCloseReason)
			return true;
		else if(ForceCloseReason > item.ForceCloseReason)
			return false;
		return false;
	}
};

enum eInOut
{
	FundIN = 0, //入金
	FundOut     //出金
};
//出入金
struct sFundInOut
{
	_UserIDType  mUserID;     //交易员账户
	eInOut       meInOut;     //出或入
	double       mdbVolume;   //金额
	char         msDesc[200]; //备注
	_UserIDType  mOpAdminID;  //操作的管理员账户
	char		 msDay[11];   //操作日期
	char		 msTime[9];   //操作时间
};

struct sQryFundInOut
{
	_UserIDType  mUserID;           //交易员账户 如果是所有的请填入*
	char msStartDay[11];    //开始操作日期
	char msEndDay[11];      //开始操作日期
};


struct sReinitTime
{
	_ExchangTimeType mNightTime; //夜盘启动时间
	_ExchangTimeType mDayTime;  //日盘启动时间
};
struct sSaveTime
{
	_ExchangTimeType mSaveTime1; //盘中保存数据
	_ExchangTimeType mSaveTime2;   //盘尾启动时间
};
struct sTradingTime
{
	_ExchangTimeType mTradingDayStart; //日盘开始时间
	_ExchangTimeType mTradingDayEnd ;  //日盘结束时间
	_ExchangTimeType mTradingNightStart; //夜盘开始时间
	_ExchangTimeType mTradingNightEnd ;  //夜盘结束时间
};

///////////////////////////风控///////////////////////////////////////////////
//修改密码
struct sModifyPassword
{
    char szOldPassword[21];
    char szNewPassword1[21];
    char szNewPassword2[21];
};

//解锁密码
struct sUnLockPassword
{
    int nRiskerID;
    char szUnLockPassword[21];
};


//展现统计使用的结构体
//理财产品与资管组织关系
struct AssetOrgProRelation
{
	int mnAssetMgmtOrgID;     //组织机构ID
	int mnFinancialProductID; //理财产品ID

	bool operator<(const AssetOrgProRelation&	src) const
	{
		if(mnAssetMgmtOrgID<src.mnAssetMgmtOrgID)
			return true;
		else if(mnAssetMgmtOrgID==src.mnAssetMgmtOrgID)
		{
			if(mnFinancialProductID<src.mnFinancialProductID)
				return true;
		}
		return false;
	}

};
enum eStatisticsIDType
{
	ST_PosProfit=1,//1.	持仓盈亏汇总
	ST_InitFund,//2.	账户初始权益汇总
	ST_Balance,//3.	账户当前总权益汇总
	ST_UseMargin,//4.	合约持仓保证金汇总
	ST_Position,//5.	合约的持仓量汇总
};
struct	StatisticsEntry
{
	eStatisticsIDType		mnStatisticsType;
	int						mnAssetMgmtOrgID;
	int						mnFinancialProductID;	
	_InstrumentID			mInstrumentID;
	double					mdblValue;
};

//////////////////////////////////////////////////////////////////////////
//基础数据日志结构

enum OperType
{
    Oper_Add,
    Oper_Delete,
    Oper_Modify,
    Oper_Save,
    Oper_Add_Trade,
    Oper_Add_Quot,
    Oper_Modify_Trade,
    Oper_Modify_Quot    
};

struct LogBaseData
{
    char     logTable[41];   // 要存入数据库的表
    char	 logDateTime[20];// 操作时间，格式"yyyy-mm-dd HH:MM:SS"
    int      logOperatorID;  // 操作者ID
    int      logTargetorID;  // 操作对象ID
    OperType logAction;      // 操作动作
    char     logContent[256];// 操作内容
    bool     logResult;      // 操作结果
};

//系统错误日志定义
enum eErrorType
{
	FirstLevelError,  //1级错误，影响系统运行的错误，需推送
	SecondLevelError, //2级错误，不影响系统运行的错误，需推送
	ThirdLevelError,  //3级错误，不影响系统运行，不推送
	Logger,			  //系统正常日志 ，可选推送
    OperationLogNeedSend,			  //系统操作日志，推送到其他的客户端正常日志
};
enum eSendBit
{
    eSendManager = 1, //推送给管理终端
    eSendRisk = 2,    //推送给所有风控终端
    eSendPresentation = 4, //推送给展现终端
    eSendTrader = 8,       //推送给交易终端
    eSendSpecialUserClient = 16,//推送给特定的用户ID
};
#define  MAX_SEND_SPECIAL_USER_SIZE 50
struct sErrorInfo
{
	sErrorInfo()
	{
		memset(msError,0,512);
        mhSocket = 0;
        mnSendBit = 0;
		ClearArray();
	
	}
	sErrorInfo(eErrorType nErrorType,char* npMsg)
	{
		meErrorType = nErrorType;
		memset(msError,0,512);
	    ClearArray();
		if(npMsg)
			strcpy(msError,npMsg);
	}

    sErrorInfo(eErrorType nErrorType,char* npMsg,int nSocket,int nSendBit)
    {
        meErrorType = nErrorType;
        memset(msError,0,512);
        if(npMsg)
            strcpy(msError,npMsg);
        mnSendBit = nSendBit;
        mhSocket = nSocket;
		ClearArray();
    }
	void ClearArray()
	{
		for(int i = 0; i < MAX_SEND_SPECIAL_USER_SIZE; i++)
			mnSpecialUserIDArray[i] = 0;
	}
	eErrorType meErrorType;  //错误类型
	char       msError[512];   // 错误日志
    int        mhSocket;      //错误socket
    int        mnSendBit;     //推送的客户端组合位 、
	int	       mnSpecialUserIDArray[MAX_SEND_SPECIAL_USER_SIZE]; //推送的特定用户ID数组
};


struct sUserOnlineInfo
{
	int mUserID;           //用户iD
	int mUserOnlineCount;  //用户在线数
};

//////////////////////////////////////////////////////////////////////////
enum eServerRunMode 
{
    ServerRunMode_Master,//主服务
    ServerRunMode_Slave  //从服务
};


//#ifdef _SIMULATE_
//模拟行情数据类型
enum eSimulateDayType
{
    SimulateDayType_Special, //最近交易日
    SimulateDayType_Nearest //指定交易日
};
//行情回放类型
enum eSimulatePlayMode
{
    SimulatePlayMode_Normal, //实盘正常回放
    SimulatePlayMode_NoDelay //无时间延迟回放
};
struct sSimulateDay
{
    eSimulateDayType mType;     //模拟行情数据选择类型 1：最近一个交易日（默认项，下面无效）0：指定交易日（下面两个有效）
    _ExchangDateType mStartDay; //开始交易日
    _ExchangDateType mEndDay;   //结束交易日
};
struct sSimulateTime
{
    _ExchangTimeType mStartTime; //启动时间点
    _ExchangTimeType mEndTime;  //停止时间点
};
//#endif

enum QryPropertyAccount
{
	QryIsAccount = 0,
	QryIsUser
};
enum QryPropertyTrader
{
	QryIsTrader = 0,
	QryIsOrder
};
enum QryPropertyEnd
{
	QryIsNotEnd = 0,
	QryIsEnd
};


//以下为策略平台定义


// 指标类型
enum IndicatorType
{
    BarIndicator = 0,   //K线指标
	MDIndicator         //行情指标

};

//指标文件夹
enum IndicatorFile
{
	PriceIndicatorFile = 0,    //价格指标
	TradingVolumeFile,          //交易量指标
	IndicatorSampleFile         //指标样例


};
//指标
struct IndicatorInfo
{
	//int              nIndicatorID;       //指标ID
	char             szName[32];         //指标名称
	IndicatorType    eIndiType;          // 指标类型
	IndicatorFile    eIndiFile;          //指标文件夹
	char             szDescribe[256];     //指标描述

	bool operator== (const IndicatorInfo& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;
		irlt=strncmp(szDescribe,r.szDescribe,sizeof(szDescribe));
		if(irlt!=0) return false;

		if(eIndiType!=r.eIndiType)
			return false;
		if(eIndiFile!=r.eIndiFile)
			return false;

		return true;
	}
};

//指标参数类型
enum  IndicatorParaType
{
	IntegerType = 0,
	DoubleType,
	BooleanType      

};
//指标参数
struct  IndicatorPara
{
	char               szName[32];                //参数名称
	IndicatorParaType  eType;                     //指标参数类型
	char               szDefault[128];             // 默认值

	bool operator== (const IndicatorPara& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;

		irlt=strncmp(szDefault,r.szDefault,sizeof(szDefault));
		if(irlt!=0) return false;

		if(eType!=r.eType)
			return false;

		return true;
	}

};
//

typedef char StrategyName[21]; 
typedef char ACCOUNTSTRING[21];
typedef char VERSIONSTRING[11];
typedef char TInstrumentID[255];
typedef char TParameterType[2550];
typedef char TCommentType[255];

typedef char INDEXSTRING[21];

typedef char UPLOADSTRING[2000];
typedef char UPLOADEND[50];
//策略
struct SStrategy
{
	StrategyName		strategyName;		//策略名
	StrategyName		strategyNickName;	//策略中文名
	VERSIONSTRING		Version;			//策略版本号
	ACCOUNTSTRING		strTraderName;		//上传人
	int					nPrivate;			//1:私有；2共享
	TParameterType      strPara;			//配置参数字符串
	TCommentType		strComment;			//备注

	int					nUse;				//1:启用； 
};

//策略方案
struct SStrategyInstance
{
	StrategyName		strategyName;			//策略名
	ACCOUNTSTRING		strTraderName;			//上传人
	TInstrumentID		Instruments;			//合约	
	int					nRunAfterLoad;			//1:加载后立即运行；  0：加载后不运行
	int					nRunAfterOffline;		//1:离线后依然运行；  0：离线后不运行
	int					nOrderActionBeforeStop; //1:停止前撤单；      0：停止前不撤单
	TParameterType      strPara;				//配置参数字符串
	TCommentType		strComment;				//备注
	int					nStart;					//1:运行；（对应的就是SUseStrategy的1:启用；0:停用）

	bool				bStartTimer;			//是否启动定时器
	int					nTimerSpan;				//定时器时间间隔
};
#define UPLOAD_STRATEGY_DLL 2
#define UPLOAD_STRATEGY_ZIP 3
#define UPLOAD_INDEX_DLL 2
#define UPLOAD_INDEX_ZIP 3
struct SIndex
{
	INDEXSTRING		IndexName;				//指标名
	int				nType;
	ACCOUNTSTRING		strTraderName;		//交易员
	int					nPrivate;			//1:私有；2共享
	TParameterType      strPara;				//配置参数字符串
	TCommentType		strComment;			//备注
};
struct SStrategy2Index//一个策略对应几个指标名
{
	StrategyName		strategyName;			//策略名
	INDEXSTRING			IndexName;				//指标名
};
struct SUseStrategy //停用启用策略;策略方案
{
	int					nUserType;				//1:管理终端；2：为策略终端
	ACCOUNTSTRING		strTraderName;			//交易员
	StrategyName		strategyName;			//策略名
	int					nStatus;				//0:停用；1:启用
};
struct SDelStrategy //删除策略;策略方案
{
	int					nUserType;				//1:管理终端；2：为策略终端
	ACCOUNTSTRING		strTraderName;			//交易员
	StrategyName		strategyName;			//策略名	
};
struct LogStrategy
{
	char     logTable[41];   // 要存入数据库的表
	char	 logDateTime[20];// 操作时间，格式"yyyy-mm-dd HH:MM:SS"
	char      logOperatorID[25];  // 操作者ID
	char      logTargetorID[25];  // 操作对象ID
	OperType logAction;      // 操作动作
	char     logContent[256];// 操作内容
	bool     logResult;      // 操作结果
};

struct UploadStart 
{
	UPLOADSTRING uploadstring;//这个字符串是一个XML格式
};
/*XML格式
	<uploadid>ID</uploadid>						//上传唯一性ID		
	<traderid>taderid</traderid>				//交易员ID
	<stratregyname>test1;test2</stratregyname>  //所有策略名，用分号隔开
	<indexname>in1;in2</indexname>              //所有指标名，用分号隔开

*/
struct UploadEnd 
{
	UPLOADEND uploadend;//这个字符串是uploadstring里面的uploadid
	ACCOUNTSTRING		strTraderName;			//交易员
};
struct QueryCommission
{	
	///账号
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};
struct QueryMarginRate
{
	///账号
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};
//////////////////////////////////////////////////////////////////////////

//策略参数类型
enum  StrategyParaType
{
	StrategyIntegerType = 0,
	StrategyDoubleType =1,
	StrategyBooleanType,
	StrategyStringType,
	StrategyDateTimeType,
	StrategyCycleType,
	SubStrategyType
};
enum  StrategyDirection
{
	DirectionIN = 0,
	DirectionOUT =1,
	DirectionINOUT      

};
//显示方式
enum  DisplayType
{
	EditBox,//文本框
	RadioBox,//单选框
	DropDownBox,//单选框
	BOOLBox,//布尔
	CYCLEBox,//周期
	DirectionBox,//买卖方向 
	OffsetFlagBox,//开平方向
	TimeBox,//时间输入框
	//DateBox//日期输入框
};
//策略参数
struct StrategyPara 
{
	char               szTitle[32];                //参数标题
	char               szName[32];                //参数名称
	StrategyParaType   eType;                     //策略参数类型
	StrategyDirection  eDirection;                 //策略参数方向
	char               szDefault[128];             // 默认值
	DisplayType        eDisplayType;                //显示方式
	bool               bReadOnly;                    //只读


	bool operator== (const StrategyPara& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;
		irlt=strncmp(szTitle,r.szTitle,sizeof(szTitle));
		if(irlt!=0) return false;

		irlt=strncmp(szDefault,r.szDefault,sizeof(szDefault));
		if(irlt!=0) return false;

		if(eDirection!=r.eDirection)
			return false;
		if(eType!=r.eType)
			return false;
		if(eDisplayType!=r.eDisplayType)
			return false;
		if(bReadOnly!=r.bReadOnly)
			return false;

		return true;
	}
};
//策略指标配置
struct StrategyIndicator 
{
	char               szTitle[32];                //策略指标，为key值
	char               szName[32];                //指标名称
	char               szPara[256];                //指标参数
	bool               bOutput;                    //是否输出
	int                nInstrument;           //所选合约


	bool operator== (const StrategyIndicator& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;
		irlt=strncmp(szTitle,r.szTitle,sizeof(szTitle));
		if(irlt!=0) return false;

		irlt=strncmp(szPara,r.szPara,sizeof(szPara));
		if(irlt!=0) return false;
		
		if(nInstrument!=r.nInstrument)
			return false;
		if(bOutput!=r.bOutput)
			return false;

		return true;
	}
};
struct StrategyPrarSubItem 
{
	char               szName[32];                 //参数选项名称
	char               szValue[32];                //参数选项值
	bool               bDefault;                   //是否是默认值
};

//策略测试结果指标
struct TestIndicator 
{
	//报表输出用到的字段
	double         NetProfit;            //净利润
	double         AvgProfit;            //平均盈利
	//double         PositionTimeRatio;    //持仓时间比率
	double         TotalProfit;          //总盈利
	double         TotalLoss;            //总亏损
	double         AvgLoss;              //平均亏损
	double         InitialProfit;;       //初始权益
	double         MiniDynamicProfit;    //最小权益
	double         MaxDynamicProfit;     //最大权益
	double         GrossProfit;          //毛利润
	double         AvgNetProfit;         //平均利润
	double         TotalCommission;      //总手续费
	double         MaxUsedFund;          //最大使用资金
	double         FinishProfit;         //结束总权益
	int            OpenOrderCnt;         //未成交手数
	double         WinRate;              //胜率
	double         YieldRate;            //收益率
	double         EffectiveYield;       //有效收益率
	double         Annualyield;          //年收益率
	double         MonthlyAvgProfit;     //月平均利润
	double         AnnualProfit;         //年化收益
	int            MaxContinuousProfit;  //最大连续盈利次数
	int            MaxContinuousLoss;    //最大连续亏损次数
	int            TotalTradeCnt;        //总交易次数
	int            MaxPosition;          //最大持仓量
	double         ConfidenceLevel;      //置信度
	int            TotalOrderCnt;        //总交易委托数
	//int            TotalPositionTime;    //总持仓时间
	double         MaxProfit;            //最大盈利
	double         MaxLoss;              //最大亏损
	char		   szStartTime[24];		// 策略开始时间yyyy-mm-dd hh:mm:ss
	char		   szEndTime[24];		// 策略结束时间yyyy-mm-dd hh:mm:ss
	char		   szCreatTime[24];		// 策略创建时间yyyy-mm-dd hh:mm:ss
	char		   szModifyTime[24];	// 策略修改时间yyyy-mm-dd hh:mm:ss
	char		   szQuoteSource[_MAX_PATH];	// 行情来源

	//计算过程中使用到的字段

	int           TotalTradingTime;       //总交易天数
	int           TotalProfitCnt;         //总盈利次数
	int           TotalLossCnt;           //总亏损次数
	bool          bIsProfit;              //是否盈利









};

struct TestSimpleData
{
	char			szName[64];				// 策略中文名
	char			szDate[16];				// 测试日期
	char			szTime[16];				// 测试时间
	double         NetProfit;				//净利润
	int            TotalTradeCnt;			//总交易次数
};



//redis 通道名称
#define ORDERINPUT_CHANNEL_NAME "OrderInput"
#define ORDERACTION_CHANNEL_NAME "OrderAction"
#define HEARTBEAT_CHANNEL_NAME "HeartBeat"


///行情信息扩展 只为本地策略调试使用
struct PlatformStru_DepthMarketData_EX
{
     PlatformStru_DepthMarketData_EX()
     {
         clear();
     }
     PlatformStru_DepthMarketData_EX(const CThostFtdcDepthMarketDataField& r)
     {
         clear();
 
         strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
         strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
         strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);
         LastPrice = r.LastPrice;
         PreSettlementPrice = r.PreSettlementPrice;
         PreClosePrice = r.PreClosePrice;
         PreOpenInterest = r.PreOpenInterest;
         OpenPrice = r.OpenPrice;
         HighestPrice = r.HighestPrice;
         LowestPrice = r.LowestPrice;
         Volume = r.Volume;
         Turnover = r.Turnover;
         OpenInterest = r.OpenInterest;
         ClosePrice = r.ClosePrice;
         SettlementPrice = r.SettlementPrice;
         UpperLimitPrice = r.UpperLimitPrice;
         LowerLimitPrice = r.LowerLimitPrice;
         strcpy(UpdateTime, r.UpdateTime);
         UpdateMillisec = r.UpdateMillisec;
         BidPrice1 = r.BidPrice1;
         BidVolume1 = r.BidVolume1;
         AskPrice1 = r.AskPrice1;
         AskVolume1 = r.AskVolume1;
         AveragePrice = r.AveragePrice;
     }
     void clear(void)
     {
         memset(this,0,sizeof(*this));
 
         LastPrice=util::GetDoubleInvalidValue();
         PreSettlementPrice=util::GetDoubleInvalidValue();
         PreClosePrice=util::GetDoubleInvalidValue();
         PreOpenInterest=util::GetDoubleInvalidValue();
         OpenPrice=util::GetDoubleInvalidValue();
         HighestPrice=util::GetDoubleInvalidValue();
         LowestPrice=util::GetDoubleInvalidValue();
         Volume=util::GetIntInvalidValue();
         Turnover=util::GetDoubleInvalidValue();
         OpenInterest=util::GetDoubleInvalidValue();
         ClosePrice=util::GetDoubleInvalidValue();
         SettlementPrice=util::GetDoubleInvalidValue();
         UpperLimitPrice=util::GetDoubleInvalidValue();
         LowerLimitPrice=util::GetDoubleInvalidValue();
         BidPrice1=util::GetDoubleInvalidValue();
         BidVolume1=util::GetIntInvalidValue();
         AskPrice1=util::GetDoubleInvalidValue();
         AskVolume1=util::GetIntInvalidValue();
         AveragePrice=util::GetDoubleInvalidValue();
     }
 
    ///交易日
    TThostFtdcDateType	TradingDay;
    ///合约代码
    InstrumentIDType	InstrumentID;
    ///交易所代码
    TThostFtdcExchangeIDType	ExchangeID;
    ///最新价
    TThostFtdcPriceType	LastPrice;
    ///上次结算价
    TThostFtdcPriceType	PreSettlementPrice;
    ///昨收盘
    TThostFtdcPriceType	PreClosePrice;
    ///昨持仓量
    TThostFtdcLargeVolumeType	PreOpenInterest;
    ///今开盘
    TThostFtdcPriceType	OpenPrice;
    ///最高价
    TThostFtdcPriceType	HighestPrice;
    ///最低价
    TThostFtdcPriceType	LowestPrice;
    ///数量
    TThostFtdcVolumeType	Volume;
    ///成交金额
    TThostFtdcMoneyType	Turnover;
    ///持仓量
    TThostFtdcLargeVolumeType	OpenInterest;
    ///今收盘
    TThostFtdcPriceType	ClosePrice;
    ///本次结算价
    TThostFtdcPriceType	SettlementPrice;
    ///涨停板价
    TThostFtdcPriceType	UpperLimitPrice;
    ///跌停板价
    TThostFtdcPriceType	LowerLimitPrice;
    ///最后修改时间
    TThostFtdcTimeType	UpdateTime;
    ///最后修改毫秒
    TThostFtdcMillisecType	UpdateMillisec;
    ///申买价一
    TThostFtdcPriceType	BidPrice1;
    ///申买量一
    TThostFtdcVolumeType	BidVolume1;
    ///申卖价一
    TThostFtdcPriceType	AskPrice1;
    ///申卖量一
    TThostFtdcVolumeType	AskVolume1;
    ///当日均价
    TThostFtdcPriceType	AveragePrice;
};

enum  KLineDataCycle
{
	Minute_1 = 1,
	Minute_3 =3,
	Minute_5=5,
	Minute_10=10,
	Minute_15=15,
	Minute_30=30,
	Minute_60=60,
	Day_1,
	Week_1,
	Month_1,
};

//K线
struct KLineData
{
	KLineDataCycle	eCycle;           //K线周期
	char			szDate[16];			   // 日期yyyymmdd
	char			szUpdateTime[9];			  // 最后更新时间分钟K线hh:mm:ss，日k 线 yyyymm  周K线 月K线yyyy
	_InstrumentID	InstrumentID;     //合约代码
	double          OpenPrice;			  //开盘价	
	double	        HighestPrice;         ///最高价	
	double	        LowestPrice;          ///最低价
    double	        ClosePrice;           /// 收盘价
	int         	IncreaseVolume;              ///成交量增量
	int         	TotalVolume;              ///成交量总量
	double          Turnover;			//成交金额增量
	int            OpenInterest;				//持仓量增量
	double         SettlementPrice;                //结算价

};
///分时线
struct TimeLineData
{
	char			szDate[16];			   // 日期
	char			szTime[9];			  // 时间
	_InstrumentID	InstrumentID;     //合约代码
	double	        ClosePrice;           /// 收盘价
	int         	Volume;              ///成交量增量
	double          Turnover;			//成交金额增量
	int             OpenInterest;		//持仓量增量

};
enum EnumPhrase
{
	PHRASE_1MIN		= 1,
	PHRASE_5MIN		= 5,
	PHRASE_15MIN	= 15,
	PHRASE_30MIN	= 30,
	PHRASE_60MIN	= 60,
	PHRASE_DAY		= 24*60,
	PHRASE_WEEK		= 7*24*60,
	PHRASE_MONTH	= 31*24*60,
	PHRASE_USERDEFINE = 0
};
struct KLineBase   //k线数据结构，1分钟 到 月线都是这个数据结构
{
	time_t dwTime;						//时间，从1900年1月1日开始的秒数
	double fOpenPrice;					// 开盘价
	double fHighPrice;					// 最高价
	double fLowPrice;					// 最低价
	double fClosePrice;					// 收盘价
	double fTurnover;					//成交金额
	DWORD dwVolume;						//成交量
	DWORD dwOpenInterest;				//持仓量
	KLineBase& operator=(const KLineBase& other)
	{
		dwTime			= other.dwTime;

		fOpenPrice      = other.fOpenPrice;
		fHighPrice		= other.fHighPrice;		
		fLowPrice		= other.fLowPrice;			
		fClosePrice		= other.fClosePrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		//dJieSuan		= other.dJieSuan;      
		return *this;
	}
	bool operator<(const KLineBase&	src) const
	{//从大到小排序；
		return dwTime > src.dwTime;
	}
} ; 

struct TLineBase //分时数据结构
{
	time_t dwTime;						//时间，从1900年1月1日开始的秒数
	double fLastPrice;					// 收盘价
	double fTurnover;					//成交金额
	DWORD dwVolume;						//成交量
	DWORD dwOpenInterest;				//持仓量
	TLineBase& operator=(const TLineBase& other)
	{
		dwTime			= other.dwTime;

		fLastPrice		= other.fLastPrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		return *this;
	}
	bool operator<(const TLineBase&	src) const
	{//从大到小排序；
		return dwTime > src.dwTime;
	}
};	
//查询K线、分时线结构
struct KLineQuery
{	
	time_t			        szStartTime;		// 
	time_t			        szEndTime;		//
	_InstrumentID	        InstrumentID;     //合约代码
	EnumPhrase				nCycle;           //K线周期	
	int                     nDataType;         //1-K线数据，2-分钟线数据, 3-查询结算价
};

struct QueryPara 
{
	char szBuffer[4000]; 
	//std::vector<std::string> vecSql;
	//KLineQuery query;
	int  nSocket;
	int  nSeq;
};
struct SimpleDepthMarketData
{

	///交易日
	TThostFtdcDateType	TradingDay;
	///合约代码
	InstrumentIDType	InstrumentID;
	///最新价
	TThostFtdcPriceType	LastPrice;
	///今开盘
	TThostFtdcPriceType	OpenPrice;
	///最高价
	TThostFtdcPriceType	HighestPrice;
	///最低价
	TThostFtdcPriceType	LowestPrice;
	///今收盘
	TThostFtdcPriceType	ClosePrice;
	///成交金额
	TThostFtdcMoneyType	Turnover;
	///持仓量
	TThostFtdcLargeVolumeType	OpenInterest;
	///上次结算价
	TThostFtdcPriceType	PreSettlementPrice;
	///数量
	TThostFtdcVolumeType	Volume;
	///最后修改时间
	TThostFtdcTimeType	UpdateTime;
	///申买价一
	TThostFtdcPriceType	BidPrice1;
	///申买量一
	TThostFtdcVolumeType	BidVolume1;
	///申卖价一
	TThostFtdcPriceType	AskPrice1;
	///申卖量一
	TThostFtdcVolumeType	AskVolume1;

};
struct SettlementPriceCalc
{
	///交易日
	TThostFtdcDateType	TradingDay;
	///合约代码
	InstrumentIDType	InstrumentID;
	///结算价
	TThostFtdcPriceType	SettlementPrice;
	///价格变化
	TThostFtdcPriceType	PriceChange;
	///标准差
	TThostFtdcPriceType	StandardDeviation;

};
struct Volatility 
{
	///交易日
	TThostFtdcDateType	TradingDay;
	///合约代码
	InstrumentIDType	InstrumentID;
	///20天波动率
	TThostFtdcPriceType	VolatilityTwenty;
	///60天波动率
	TThostFtdcPriceType	VolatilitySixty;
};

struct SettlementPrice
{
	///交易日
	int	TradingDay;
	///结算价
	double	Price;
};

#pragma pack(pop)