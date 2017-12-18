#pragma once

// 停靠在Fasttrader窗口中的位置定义
#define NEED_NO_POS_NO_BIND		0	// 不需要停靠，不绑定窗口
#define POS_ANY			1	// 需要停靠，但对位置无要求
#define POS_LEFT		2	// 停靠在窗口左边
#define POS_RIGHT		3	// 停靠在窗口右边
#define POS_TOP			4	// 停靠在窗口顶端
#define POS_BOTTOM		5	// 停靠在窗口底端
#define POS_QUOTE		6	// 在报价表窗口位置显示
#define NEED_NO_POS_BUT_BIND	7	//不需要停靠，需要绑定窗口

#define DEBUG_ON	0	//调试开关
#define HEARTBEAT_RATE	60	//心跳时间间隔，单位为秒
// CMDID定义
// 控制消息命令字
#define CMDID_CONSOLE_MIN							0x1000
#define CMDID_CONSOLE_MAX							0x1FFF
#define CMDID_CONSOLE_HEARTBEAT						0x1001	//心跳命令
#define CMDID_CONSOLE_HIDE							0x1003	//隐藏
#define CMDID_CONSOLE_LOGON_REQUEST					0x1004	//登录请求
#define CMDID_CONSOLE_LOGON_RSP						0x1005	//登录响应
#define CMDID_CONSOLE_CLOSE							0x1006	//要求插件退出命令
#define CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW		0x1007	//FastTrader给插件发送消息，要求插件窗口显示
#define CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_HIDE		0x1008	//FastTrader给插件发送消息，要求插件窗口隐藏
#define CMDID_CONSOLE_CHANGE_SIZE					0x1009	//第三方插件向FastTrader发送消息，要求改变插件窗口大小状态
#define CMDID_CONSOLE_WND_SIZE_STATUS_QUERY			0x100A	//获取当前窗口大小状态
#define CMDID_CONSOLE_WND_SIZE_STATUS_RSP			0x100B	//返回当前窗口大小状态
#define CMDID_CONSOLE_WND_SIZE_STATUS_CHANGED		0x100C	//FastTrade向第三方插件推送窗口状态改变消息

// 账户信息命令字
#define CMDID_ACCOUNT_MIN							0x2000
#define CMDID_ACCOUNT_MAX							0x2FFF
#define CMDID_ACCOUNT_SUB_ACCOUNT_LIST_QUERY		0x2001	//查询所有子账户
#define CMDID_ACCOUNT_SUB_ACCOUNT_LIST_RSP			0x2002	//子账户信息返回
#define CMDID_ACCOUNT_SUB_ACCOUNT_INFO_QUERY		0x2003	//查询某个子账户资金信息
#define CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP			0x2004	//返回某个子账户资金信息
#define CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY			0x2005	//获取当前账号
#define CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP			0x2006	//返回当前账号
#define CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_QUERY		0x2007	//获取当前子账号
#define CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP		0x2008	//返回当前子账号
#define CMDID_ACCOUNT_REQUERY						0x2009	//FastTrader给第三方插件发送资金重新查询事件

// 合约信息命令字
#define CMDID_INSTRUMENT_MIN						0x3000
#define CMDID_INSTRUMENT_MAX						0x3FFF
#define CMDID_INSTRUMENT_LIST_QUERY					0x3001	//查询特定交易所的合约列表
#define CMDID_INSTRUMENT_LIST_RSP					0x3002	//查询特定交易所的合约列表响应
#define CMDID_INSTRUMENT_INFO_QUERY					0x3003	//查询单条合约详情
#define CMDID_INSTRUMENT_INFO_RSP					0x3004	//查询单条合约详情响应
#define CMDID_INSTRUMENT_SHORTNAME_QUERY			0x3005	//获取合约简称表，注意：此命令字已废弃，不再维护。
#define CMDID_INSTRUMENT_SHORTNAME_RSP				0x3006	//返回合约简称表，注意：此命令字已废弃，不再维护。
#define CMDID_PRODUCT_LIST_QUERY					0x3010	//查询指定交易所的品种列表。参数为ExchangeID
#define CMDID_PRODUCT_LIST_RSP					    0x3011	//查询指定交易所的品种列表响应。成功时lpData指向字符串数组，每个字符串32字节，第一个字符串是查询参数ExchangeID，后面是一系列的ProductID; cbData等于(1+品种数量)*32
#define CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY	0x3012	//查询指定交易所的期权标的物期货品种列表。参数为ExchangeID
#define CMDID_OPTIONUNDERLYINGPRODUCT_LIST_RSP	    0x3013	//查询特定交易所的期权标的物期货品种列表响应。成功时lpData指向字符串数组，每个字符串32字节，第一个字符串是查询参数ExchangeID，后面是一系列的ProductID; cbData等于(1+品种数量)*32
#define CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY	0x3014	//查询标的物期货合约对应的期权合约列表。参数为Underlying InstrumentID
#define CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_RSP	0x3015	//查询标的物期货合约对应的期权合约列表响应。成功时lpData指向字符串数组，每个字符串32字节，第一个字符串是查询参数UnderlyingInstrumentID(标的物合约)，后面是一系列的Option InstrumentID(期权合约); cbData等于(1+期权合约数量)*32
#define CMDID_PRODUCT_INFO_QUERY					0x3016	//查询单个品种详情。参数为ProductID，不能为空
#define CMDID_PRODUCT_INFO_RSP					    0x3017	//查询单个品种详情响应。成功时lpData指向PlatformStru_ProductInfo结构体
#define CMDID_PRODUCT_ALLINFO_QUERY					0x3018	//查询全部品种详情。参数为空
#define CMDID_PRODUCT_ALLINFO_RSP					0x3019	//查询全部品种详情响应。成功时lpData指向PlatformStru_ProductInfo数组，cbData=n*sizeof(PlatformStru_ProductInfo)
#define CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY	    0x3020	//查询指定品种对应的合约列表。参数为ProductID
#define CMDID_INSTRUMENT_LIST_BYPRODUCTID_RSP		0x3021	//查询指定品种对应的合约列表响应。成功时lpData指向字符串数组，每个字符串32字节，第一个字符串是查询参数ProductID，后面是对应的InstrumentID数组; cbData等于(1+合约数量)*32
#define CMDID_TRADINGTIMESPAN_QUERY				    0x3022	//查询合约的日内交易时段，参数为InstrumentID，不能为空
#define CMDID_TRADINGTIMESPAN_RSP				    0x3023	//返回合约的日内交易时段，成功时lpData为InstrumentID(32字节)+时间段数组，每个时间段为4+4字节，表示开始和结束时间，格式为hhmm，如0915113013001515表示两个时间段，9:15-11:30和13:00-15:15; cbData=32+时间段数量*8
#define CMDID_TRADINGDAY_QUERY				        0x3024	//查询交易日，参数为空
#define CMDID_TRADINGDAY_RSP				        0x3025	//返回交易日，成功时lpData为TradingDay(32字节)，如20141020; cbData=32

// 成交信息命令字
#define CMDID_TRADEINFOBRIEF_LIST_QUERY	            0x3500	//查询指定合约的成交信息(简要)列表。参数为InstrumentID，可以为空，表示查询全部的成交
#define CMDID_TRADEINFOBRIEF_LIST_RSP	            0x3501	//查询指定合约的成交信息(简要)列表响应。成功时lpData指向要查询的合约ID+结果PlatformStru_TradeInfoBrief数组，cbData等于32+sizeof(PlatformStru_TradeInfoBrief)*成交记录数量


// 委托单信息命令字
#define CMDID_ALL_ORDER_MIN							0x4000
#define CMDID_ALL_ORDER_MAX							0x4FFF
#define CMDID_ALL_ORDER_LIST_QUERY					0x4001	//查询委托单列表
#define CMDID_ALL_ORDER_LIST_RSP					0x4002	//查询委托单列表响应
#define CMDID_ALL_ORDER_INFO_QUERY1					0x4003	//查询单条委托单详情，参数为FTID
#define CMDID_ALL_ORDER_INFO_QUERY2					0x4004	//查询单条委托单详情，参数为Account、InstrumentID、SessionID、FrontID、OrderRef
#define CMDID_ALL_ORDER_INFO_RSP					0x4005	//查询单条委托单详情响应
#define CMDID_ALL_ORDER_CURRENT_SEL					0x4006	//FastTrader给第三方插件发送委托单模块当前选中的项
#define CMDID_ALL_ORDER_WAIT_ORDER_NUM_QUERY		0x4007	//查询未成交挂单数量，参数为用户名、子帐号、合约ID、LimitPrice
#define CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP			0x4008	//返回未成交挂单数量
#define CMDID_ALL_ORDER_REQUERY						0x4009	//FastTrader给第三方插件发送委托单重新查询事件
#define CMDID_ALL_ORDER_WAIT_ORDER_PRICE_QUERY		0x4010	//请求指定合约未成交挂单中最高挂单价格和最低挂单价格
#define CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP		0x4011	//返回指定合约未成交挂单中最高挂单价格和最低挂单价格
#define CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_QUERY	0x4012	//查询开仓未成交挂单数量，参数为用户名、子帐号、合约ID
#define CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP		0x4013	//返回开仓未成交挂单数量

// 持仓信息命令字
#define CMDID_POSITION_MIN							0x5000
#define CMDID_POSITION_MAX							0x5FFF
#define CMDID_POSITION_LIST_QUERY					0x5101	//查询持仓列表
#define CMDID_POSITION_LIST_RSP						0x5102	//查询持仓列表响应
#define CMDID_POSITION_INFO_QUERY1					0x5103	//查询单条持仓详情，参数为FTID
#define CMDID_POSITION_INFO_QUERY2					0x5104	//查询单条持仓详情，参数为PositionKey
#define CMDID_POSITION_INFO_RSP						0x5105	//查询单条持仓详情响应
#define CMDID_POSITION_INFO_LIST_QUERY				0x5106	//查询持仓详情，参数为账户，子账户，合约ID
#define CMDID_POSITION_INFO_LIST_RSP				0x5107	//查询单条持仓详情响应
#define CMDID_ACCT_POSITION_INFO_LIST_QUERY			0x5108	//查询某个账号的所有持仓详情，参数为账户ID
#define CMDID_ACCT_POSITION_INFO_LIST_RSP			0x5109	//返回某个账号的所有持仓详情
#define CMDID_ACCT_POSITION_DETAIL_LIST_QUERY		0x510A	//查询某个账号的所有持仓明细详情，参数为账户ID
#define CMDID_ACCT_POSITION_DETAIL_LIST_RSP			0x510B	//返回某个账号的所有持仓明细详情

#define CMDID_POSITION_COMBO_LIST_QUERY				0x5501	//查询组合持仓列表
#define CMDID_POSITION_COMBO_LIST_RSP				0x5502	//查询组合持仓列表响应
#define CMDID_POSITION_COMBO_INFO_QUERY				0x5503	//查询单条组合持仓详情
#define CMDID_POSITION_COMBO_INFO_RSP				0x5504	//查询单条组合持仓详情响应

#define CMDID_POSITION_CURRENT_SEL					0x5A01	//FastTrader给第三方插件发送持仓模块当前选中的项
#define CMDID_POSITION_DETAIL_CURRENT_SEL			0x5A02	//FastTrader给第三方插件发送持仓模块当前选中的项
#define CMDID_POSITION_COMBO_CURRENT_SEL			0x5A03	//FastTrader给第三方插件发送持仓模块当前选中的项
#define CMDID_POSITION_REQUERY						0x5A04	//FastTrader给第三方插件发送持仓重新查询事件
#define CMDID_POSITION_COMBO_REQUERY				0x5A05	//FastTrader给第三方插件发送组合持仓重新查询事件

// 交易所相关命令字
#define CMDID_EXCHANGE_MIN							0x6000
#define CMDID_EXCHANGE_MAX							0x6FFF
#define CMDID_EXCHANGE_LIST_QUERY					0x6001	//查询交易所列表
#define CMDID_EXCHANGE_LIST_RSP						0x6002	//查询交易所列表响应
#define CMDID_EXCHANGE_CURRENT_TIME_QUERY			0x6003	//查询交易所当前时间
#define CMDID_EXCHANGE_CURRENT_TIME_RSP				0x6004	//查询交易所当前时间响应
#define CMDID_EXCHANGE_TRADE_TIME_QUERY				0x6005	//查询交易所交易时间
#define CMDID_EXCHANGE_TRADE_TIME_RSP				0x6006	//查询交易所交易时间响应

// 行情相关命令字
#define CMDID_QUOTE_MIN								0x7000
#define CMDID_QUOTE_MAX								0x7FFF
#define CMDID_QUOTE_SUBSCRIBE						0x7001	//订阅行情
#define CMDID_QUOTE_SUBSCRIBE_RSP					0x7002	//订阅行情返回
#define CMDID_QUOTE_SEND_QUOTE						0x7003	//行情推送
#define CMDID_QUOTE_UNSUBSCRIBE						0x7004	//行情退订
#define CMDID_QUOTE_CURRENT_GROUP_INST_LIST			0x7006	//当前选中的合约组发生变化时,FastTrader向插件发送当前合约组的合约列表
#define CMDID_QUOTE_CURRENT_SEL_QUERY				0x7007	//请求报价表当前选中项
#define CMDID_QUOTE_CURRENT_SEL						0x7008	//FastTrader给第三方插件发送报价表当前选中的项
#define CMDID_QUOTE_SNAPSHOT_QUERY					0x7009	//获取行情快照
#define CMDID_QUOTE_SNAPSHOT_RSP					0x700A	//行情快照返回
#define CMDID_QUOTE_PLUSIN_CURRENT_SEL				0x700B	//插件改变当前显示的合约后，向FastTrader发送消息，要求报价表改变当前选中的合约
#define CMDID_QUOTE_IS_SIMULATE_QUERY				0x700C	//获取FastTrader当前登录的是模拟盘还是实盘
#define CMDID_QUOTE_IS_SIMULATE_RSP					0x700D	//返回FastTrader当前登录的是模拟盘还是实盘，1：模拟盘 0：实盘
#define CMDID_QUOTE_ADD_TO_SELFGROUP_REQ			0x700E	//请求将一个合约加入到FastTrader自选合约组中
#define CMDID_QUOTE_ADD_TO_SELFGROUP_RSP			0x700F	//返回将一个合约加入到FastTrader自选合约组中的操作结果
#define CMDID_QUOTE_GETCURRENTGROUPMEMBER_QUERY     0x7011  //查询当前合约组的合约列表，不带参数
#define CMDID_QUOTE_GETCURRENTGROUPMEMBER_RSP       0x7012  //返回当前合约组的合约列表。返回合约数组。每个合约名称为32字节长, 返回合约数量等于cbData/32

// 下单相关命令字
#define CMDID_ORDER_INSERT_MIN						0x8000
#define CMDID_ORDER_INSERT_MAX						0x8FFF
#define CMDID_ORDER_INSERT_ORDER					0x8001	//下单请求,需要审核
#define CMDID_ORDER_INSERT_ORDER_RSP				0x8002	//下单请求响应
#define CMDID_ORDER_CANCEL_ORDER					0x8004	//撤单请求
#define CMDID_ORDER_CANCEL_ORDER_RSP				0x8005	//撤单请求响应
#define CMDID_ORDER_ORDER_STATUS_CHANGE				0x8007	//委托单状态改变时推送消息
#define CMDID_ORDER_TRADE_RTN						0x8008	//成交回报到来时推送
#define CMDID_ORDER_CANCEL_ORDER_BY_INST			0x8009	//指定合约撤单请求
#define CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTPRICE	0x8010	//指定合约、价格撤单请求
#define CMDID_ORDER_CURRENT_INSTRUENT_QUERY			0x8011	//查询下单板当前合约
#define CMDID_ORDER_CURRENT_INSTRUENT_RSP			0x8012	//返回下单板当前合约
#define CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTDIRPRICE	0x8013	//指定合约、方向、价格撤单请求
#define CMDID_ORDER_RSP_ORDER_ACTION1				0x8014	//撤单错误返回1
#define CMDID_ORDER_RSP_ORDER_ACTION2				0x8015	//撤单错误返回2
#define CMDID_ORDER_RSP_ORDER_INSERT1				0x8016	//下单错误返回1
#define CMDID_ORDER_RSP_ORDER_INSERT2				0x8017	//下单错误返回2
#define CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY		0x8018	//下单请求,不需要审核
#define CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY_RSP		0x8019	//下单请求,不需要审核

#define CMDID_CONFIG_MIN							0x9000
#define CMDID_CONFIG_MAX							0x9FFF
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_QUERY	0x9001	//查询“可以用鼠标输入合约代码”
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP	0x9002	//返回“可以用鼠标输入合约代码”
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_QUERY	0x9003	//查询“可以用鼠标输入常用手数”
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP		0x9004	//返回“可以用鼠标输入常用手数”
#define CMDID_CONFIG_INPUT_VOLUMES_QUERY			0x9005	//查询“手数输入框的9个常用配置项值”
#define CMDID_CONFIG_INPUT_VOLUMES_RSP				0x9006	//返回“手数输入框的9个常用配置项值”
#define CMDID_CONFIG_CHANGE							0x9007	//FastTrader给第三方插件发送配置改变消息

#define CMDID_INSERT_ORDER_VERIFY_MIN				0xA000
#define CMDID_INSERT_ORDER_VERIFY_MAX				0xAFFF
#define CMDID_INSERT_ORDER_VERIFY_OPEN_QUERY		0xA001	//开启下单审核请求
#define CMDID_INSERT_ORDER_VERIFY_OPEN_RSP			0xA002	//开启下单审核返回
#define CMDID_INSERT_ORDER_VERIFY_CLOSE_QUERY		0xA003	//关闭下单审核请求
#define CMDID_INSERT_ORDER_VERIFY_CLOSE_RSP			0xA004	//关闭下单审核返回
#define CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH		0xA005	//推送要审核的下单指令


//UI相关命令字
#define CMDID_UI_Change_ColorScheme                 0xB000  //FastTrader通知插件更改配色方案 lpData指向一个Int, 0:浅底色方案；1:深底色方案
#define CMDID_UI_ColorScheme_QUERY				    0xB001	//查询UI配色方案，参数为空
#define CMDID_UI_ColorScheme_RSP				    0xB002	//返回UI配色方案，成功时lpData指向一个Int, 0:浅底色方案；1:深底色方案; cbData=4


//错误码定义
#define SUCCESS			0		//查询成功
#define ERROR_ONE		1		//错误情况1
#define UNKOWN_ERROR	999		//未知错误

struct LogonStu
{
	char szAppID[100];
	char szLicense[100];
	char szAppPath[256];
	HWND hPlusinTopWnd;
	int nPos;
	DWORD dwVersion;
};