#pragma once
#include "string.h"
//请在这个头文件中定义服务器端数据变化的事件结构体和事件ID
#define  DATA_CHANGE_EVENT_INSTRUMENT_LENGTH 21

#define DATA_CHANGE_ENENT_DATA_LEN  1008
enum eAskType
{
    AskType_Broker,              //经纪公司
    AskType_ServerGroup,         //服务器组
    AskType_ServerAddress,       //服务器组地址 
	AskType_Account,             //委托交易帐号
	AskType_User,                //用户
    AskType_UserAndTradeAccount, //用户与委托交易账号关系
	AskType_Orgnazation,         //组织机构
	AskType_FinacialProduct,     //理财产品
    AskType_TradeStrategy,       //交易策略
	AskType_Login,				  //用户登录注销（ADD为登录，delete为注销）
    AskType_Role,                //角色
    AskType_QueryPrivilege,      //查询权限
    AskType_OrgAndStrategy,      //组织机构与交易策略的关系
    AskType_RoleAndPrivelege,    //角色与权限的关系
    AskType_TraderAndProduct,    //交易员与理财产品的关系
    AskType_UserAndRole,         //用户与角色的关系
    AskType_UserAndOrg,          //用户与组织机构的关系
    AskType_BrokerAndServerGroup,//经纪公司与服务器组的关系
    AskType_BrokerAndTradeAccount,//经纪公司与委托交易账号的关系
    AskType_BrokerAndServerGroupAddress,//经纪公司与服务器组地址的关系
	AskType_UserLoginCheck,  //用户是否在登陆中
};

enum eEventType
{
	EventTypeInvalideType = 0, //无效的事件类型
	
	EventTypeUserDataReady,    //通知用户数据准备好了
	EventTypeQuto,			   //通知新到一个行情
	EventTypeTrade,			   //成交变化 
	EventTypeFund,			   //资金变化 
	EventTypePosition,		   //持仓变化

	EventTypeNeedAllLoginTrade  ,        //通知报盘需要登录所有委托交易账号到交易IP
	EventTypeNeedSingleLoginTrade  ,     //通知报盘需要登录所有参数的委托交易账号
	EventTypeNeedAllLoginSettlement  ,   //通知报盘需要登录所有委托交易账号到结算IP
    EventTypeNeedGetInstrument,          //通知报盘从交易数据管理模块获取合约
	EventTypeNeedQryInstrument,          //通知报盘需要查询合约
	EventTypeNeedQryCommissionAndMargin, //通知报盘需查询手续费
	EventTypeNeedQryCommission,          //通知报盘需查询手续费
	EventTypeNeedQryMargin,				 //通知报盘需查询手续费
	EventTypeNeedQryOrder,				 //通知报盘需查询报单
	EventTypeNeedQryTrader,              //通知报盘需查询成交
	EventTypeNeedQryFund,				 //通知报盘需查询报单
	EventTypeNeedQryPosition,            //通知报盘需查询持仓
	EventTypeNeedQryPositionDetail,      //通知报盘需查询持仓明细
	EventTypeNeedLogoffSettlement,       //通知报盘把参数内的委托账号的ctp连接断开
	EventTypeOrderRtn,		    //通知交易执行,新到一个成交
	EventTypeTradeRtn,		    //通知交易执行,新到一个保单
	EventTypeRspOrderInsert,	//通知交易执行，报单录入应答
	EventTypeErrOrderInsert,	//通知交易执行，报单录入错误
	EventTypeRspOrderAction,	//通知交易执行，报单操作应答
	EventTypeErrOrderAction,	//通知交易执行，报单操作错误	
	
	EventTypeUserLogin,        //用户登陆事件 LPARAM为UserID
	EventTypeUserLogoff,       //用户注销事件 LPARAM为UserID
	EventTypeUserDiscon,       //用户断开事件 LPARAM为UserID

	EventTypeStartSettlement,  //开始结算
	EventTypeEndSettlement,    //结束结算
	EventTypeStartInit,        //开始初始化
	EventTypeInitFinish,        //初始化完成
	EventTypeUserStop,         //停用一个用户


    //基础数据改变后的广播事件	
    EventTypeAccountAdd ,      //增加一个委托交易账号  LPARAM为AccountID
	EventTypeAccountDelete,    //删除一个委托交易账号
	EventTypeAccountUpdate,    //更新一个委托交易账号
    EventTypeUserAdd ,         //增加一个用户
	EventTypeUserDelete,       //删除一个用户
	EventTypeUserUpdate,       //更新一个用户
    EventTypeUserLimitAndVerifyUpdate,       //更新一个用户的下单和手工审核权限
    EventTypeUserAndOrgRelation,        //用户和组织机构关系改变
    EventTypeUserAndRoleRelation,       //用户和角色关系改变
    EventTypeUserAndTradeAcctRelation,  //用户和委托交易账号关系改变
    EventTypeRoleAdd,                   //增加一个角色
    EventTypeRoleDelete,                //删除一个角色
    EventTypeRoleUpdate,                //更新一个角色
    EventTypeRoleAndPrivelegeRelation,  //角色和权限关系改变
    EventTypeOrgAdd,                   //增加一个组织机构
    EventTypeOrgDelete,                //删除一个组织机构
    EventTypeOrgUpdate,                //更新一个组织机构
    EventTypeTradeStrategyAdd,                   //增加一个交易策略
    EventTypeTradeStrategyDelete,                //删除一个交易策略
    EventTypeTradeStrategyUpdate,                //更新一个交易策略
    EventTypeTradeStrategyAndOrgRelation,  //交易策略和组织机构关系改变
    EventTypeFinicialProductAdd,                   //增加一个理财产品
    EventTypeFinicialProductDelete,                //删除一个理财产品
    EventTypeFinicialProductUpdate,                //更新一个理财产品
    EventTypeFinicialProductAndUserRelation,  //理财产品和用户关系改变
    EventTypeBrokerInfoAdd,                   //增加一个经纪公司
    EventTypeBrokerInfoDelete,                //删除一个经纪公司
    EventTypeBrokerInfoUpdate,                //更新一个经纪公司
    EventTypeServerGroupAdd,                   //增加一个服务器组
    EventTypeServerGroupDelete,                //删除一个服务器组
    EventTypeServerGroupUpdate,                //更新一个服务器组
    EventTypeServerAddressAdd,                   //增加一个服务器组地址
    EventTypeServerAddressDelete,                //删除一个服务器组地址
    EventTypeServerAddressUpdate,                //更新一个服务器组地址
    EventTypeBrokerInfoQuotUpdate,                //更新一个行情经纪公司
    EventTypeServerAddressQuotAdd,                   //增加一个行情服务器组地址
    EventTypeServerAddressQuotDelete,                //删除一个行情服务器组地址
    EventTypeServerAddressQuotUpdate,                //更新一个行情服务器组地址
	EventTypeErrorCome,                               //一个错误到达

	EventTypeSlaverLoginedMaster,                   //从服务器登录上主服务器
	
};


//数据操作问询时候，询问的数据操作类型
enum eAskOperationType
{
	AskOperationTypeAdd = 0, //增加一个基础数据
	AskOperationTypeDelete,  //删除一个基础数据
	AskOperationTypeUpdate,  //更新一个基础数据
	AskOperationTypeQuery	 //查询数据
};

typedef struct _sDataChangeEvent
{
	eEventType       meEventType;           //事件类型
	int              mhEvent;               //事件带的同步句柄
	int              mnDataID;             //事件所带的数据ID
	//char            msInstrumentName[DATA_CHANGE_EVENT_INSTRUMENT_LENGTH];  //可选项，合约名
	int             mnValidateLen;           //有效地数据长度
	unsigned char   marrDataByte[DATA_CHANGE_ENENT_DATA_LEN];       //有效地数据缓存
public:
	_sDataChangeEvent()
	{
		memset(this,0,sizeof(*this));
		meEventType = EventTypeInvalideType;
		mnValidateLen = 0;
	}
	_sDataChangeEvent(eEventType nType,int nId)
	{
		memset(this,0,sizeof(*this));
		mnValidateLen = 0;
		meEventType = nType;
		mnDataID = nId;
		mhEvent = 0;
	}
	virtual _sDataChangeEvent& operator=(const _sDataChangeEvent& t1) //函数重载，
	{ 	
		
		if(this == &t1)
			return *this;

		//this->meDataChangeType = t1.meDataChangeType;
		this->meEventType = t1.meEventType;
		this->mnDataID = t1.mnDataID;
		this->mhEvent = t1.mhEvent;
		//memcpy(msInstrumentName,t1.msInstrumentName,DATA_CHANGE_EVENT_INSTRUMENT_LENGTH);
		this->mnValidateLen = t1.mnValidateLen;
		memcpy(marrDataByte,t1.marrDataByte,DATA_CHANGE_ENENT_DATA_LEN);
		return *this;
	} 
	bool AddBufer(void* lpBuf,const int nLen)
	{
		if(DATA_CHANGE_ENENT_DATA_LEN - mnValidateLen < nLen || lpBuf == NULL || nLen == 0)
			return false;
		memcpy(marrDataByte+mnValidateLen,lpBuf,nLen);
		mnValidateLen += nLen;
		return true;

	}
}Stru_NotifyEvent;




