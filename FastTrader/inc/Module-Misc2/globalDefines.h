#pragma once
#ifndef _GLOBALDEFINES_H_
#define _GLOBALDEFINES_H_

//#define _USE_TOP_SPLITTER					//是否使用视频资讯
//#define _USE_MULTI_LANGUAGE				//是否使用双语版本
//#define _USE_GRID_LIST					//是否使用wxGrid代替wxExtListCtrl
//#define _USE_COMBOINSTRUMENT				//是否启用组合单
//#define _USE_TRACE_THREAD                 //是否使用TraceThread
#define WXLOG_INFO wxLogDebug
#define IS_MULTIACCOUNT_VERSION             (GlobalConfigManager::m_bNetVersion_MultiAccount||GlobalConfigManager::m_bLocalVersion_MultiAccount)
#define DEFAULT_SVR()					    (g_pPlatformMgr?(g_pPlatformMgr->GetDefaultPlatformSvr()):NULL)
#define CURRENT_SVR()					    (g_pPlatformMgr?(IS_MULTIACCOUNT_VERSION?g_pPlatformMgr->GetCurrentPlatformSvr():g_pPlatformMgr->GetDefaultPlatformSvr()):NULL)
#define SAFE_DELETE(p)                      if((p)!=NULL){delete p; p = NULL;}
//英语
#define LANGUAGE_ENGLISH					1
//大陆简体中文
#define LANGUAGE_CHINESE					2
//香港繁体中文
#define LANGUAGE_TRADITION                  3
//语言种类数量
#define LANGUAGE_MIN						1
#define LANGUAGE_MAX						3
//
#define SVR_LANGUAGE                        (DEFAULT_SVR()?DEFAULT_SVR()->GetServerParamCfg().LanguageId:2)
#define SVR_PLATFORM_ID                     (DEFAULT_SVR()?DEFAULT_SVR()->GetPlatformType():PLATFORM_DEFAULT)
//内部使用
#define LOADSTRING_TEXT(p)					CFG_MGR_DEFAULT()->LoadString(p).c_str()
#define LOADFORMATSTRING_TEXT(p,s)			CFG_MGR_DEFAULT()->LoadFormatString(p,s).c_str()
//外部统一使用LOADSTRING
#define LOADSTRING(p)						LOADSTRING_TEXT(#p)
#define LOADFORMATSTRING(p,s)				LOADFORMATSTRING_TEXT(#p,s)
#define LOADSTRING_LANGUAGE(p,id)			CFG_MGR_DEFAULT()->LoadString(#p,id).c_str()
#define REPLACEFMTSTRING(p)					CFG_MGR_DEFAULT()->ReplaceFmtString(p).c_str()
#define DIALOG_CREATE_LOG()\
do{\
	zqWriteLog zlog;\
	zlog.WriteLog_Fmt("wxDialog:Create", LOGLEVEL_DEBUGINFO, "[%s][%d]:HWND=[%08X],ID=[%d]", \
				  std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str(), __LINE__, \
				  GetHWND(),GetId());\
}while(0)

enum GUIModuleID
{
    GID_MIN = 8100,
    GID_QUOT_TABLE = 8100,
    GID_ORDER_INSERT_PANEL = 8101,
    GID_THRADE_RECORD = 8102,
    GID_THRADE_STATISTICS = 8103,
    GID_POSITION = 8104,
	GID_POSITIONDETAIL = 8105,
    GID_INSTRUMENT = 8106,
    GID_TEMP = 8107,
    GID_WAIT_TRADE = 8108,
	GID_POSITIONCOMB = 8109,
    GID_ACCOUNT = 8110,
    GID_TASK = 8111,
	GID_LOGIN_DLG = 8112,
    GID_OPEN_ORDER = 8113, 
	GID_ALL_ORDER = 8114,
	GID_MODIFYPWD = 8115,
	GID_SettlementInfo = 8116,
	GID_TRANSFER = 8117,
	GID_ORDER_SERVICETHREAD = 8118,
	GID_QUERY_ORDER_BATCH = 8119,
	GID_QUERY_ORDER_CONDITION = 8120,
	GID_QUERY_ORDER_POSITION = 8121,
    GID_QUERY_THRADE_RECORD = 8122,
    GID_CHURUJIN = 8123,
	GID_QUERY_ORDER_PARKED = 8124,
	GID_MASTERPIPE_SERVICETHREAD = 8125,
	GID_MULTI_ACCOUNT = 8126,
	GID_MOVESWAPPOSITION_SERVICETHREAD = 8127,
	GID_POSITIONALL=8128,
    GID_QUERYLOCAL_ORDER = 8129,
	GID_ANY = 8130,
	GID_MULTI_POSITION=8131,
	GID_PLUSIN = 8132,
	GID_QUERY_ORDER_PLDETAIL = 8133,
	GID_QUERY_ORDER_POSIPL = 8134,
	GID_DLG_AUTODERIVEORDERBYFIRSTACCOUNT = 8135,
    GID_DLG_MOVEPOSITION = 8136,
    GID_PlatformSvr = 8137,                         //代表底层，不是一个GUI模块
    GID_OptionTPrice = 8138,                        //期权T型报价模块
    GID_MAX = 8139,
};
//报价表
enum QuotFiled
{
	QUOT_FIELD_ID_InstrumentID,
	QUOT_FIELD_ID_LastPrice,
	QUOT_FIELD_ID_BidPrice1,
	QUOT_FIELD_ID_BidVolume1,
	QUOT_FIELD_ID_AskPrice1,
	QUOT_FIELD_ID_AskVolume1,
	QUOT_FIELD_ID_ChangeValue,
	QUOT_FIELD_ID_Volume,
	QUOT_FIELD_ID_OpenPrice,
	QUOT_FIELD_ID_OpenInterest,
	QUOT_FIELD_ID_UpperLimitPrice,
	QUOT_FIELD_ID_LowerLimitPrice,
	QUOT_FIELD_ID_PreSettlementPrice,
	QUOT_FIELD_ID_HighestPrice,
	QUOT_FIELD_ID_LowestPrice,
	QUOT_FIELD_ID_LastVolume,
	QUOT_FIELD_ID_InstrumentName,
	QUOT_FIELD_ID_UpdateTime,
	QUOT_FIELD_ID_ExchangeID,
	QUOT_FIELD_ID_ChangePercentage,
	QUOT_FIELD_ID_PreClosePrice,
	QUOT_FIELD_ID_PreOpenInterest,
	QUOT_FIELD_ID_ClosePrice,
	QUOT_FIELD_ID_SettlementPrice,
	QUOT_FIELD_ID_Turnover,
	
	//Pats fields
	QUOT_FIELD_ID_BidPrice2,
	QUOT_FIELD_ID_BidVolume2,
	QUOT_FIELD_ID_AskPrice2,
	QUOT_FIELD_ID_AskVolume2,
	QUOT_FIELD_ID_BidPrice3,
	QUOT_FIELD_ID_BidVolume3,
	QUOT_FIELD_ID_AskPrice3,
	QUOT_FIELD_ID_AskVolume3,
	QUOT_FIELD_ID_BidPrice4,
	QUOT_FIELD_ID_BidVolume4,
	QUOT_FIELD_ID_AskPrice4,
	QUOT_FIELD_ID_AskVolume4,
	QUOT_FIELD_ID_BidPrice5,
	QUOT_FIELD_ID_BidVolume5,
	QUOT_FIELD_ID_AskPrice5,
	QUOT_FIELD_ID_AskVolume5,
	QUOT_FIELD_ID_BidPrice6,
	QUOT_FIELD_ID_BidVolume6,
	QUOT_FIELD_ID_AskPrice6,
	QUOT_FIELD_ID_AskVolume6,
	QUOT_FIELD_ID_BidPrice7,
	QUOT_FIELD_ID_BidVolume7,
	QUOT_FIELD_ID_AskPrice7,
	QUOT_FIELD_ID_AskVolume7,
	QUOT_FIELD_ID_BidPrice8,
	QUOT_FIELD_ID_BidVolume8,
	QUOT_FIELD_ID_AskPrice8,
	QUOT_FIELD_ID_AskVolume8,
	QUOT_FIELD_ID_BidPrice9,
	QUOT_FIELD_ID_BidVolume9,
	QUOT_FIELD_ID_AskPrice9,
	QUOT_FIELD_ID_AskVolume9,
	QUOT_FIELD_ID_BidPrice10,
	QUOT_FIELD_ID_BidVolume10,
	QUOT_FIELD_ID_AskPrice10,
	QUOT_FIELD_ID_AskVolume10,
	QUOT_FIELD_ID_CURRENCY,
	QUOT_FIELD_ID_NewVolume,
	QUOT_FIELD_ID_MAX
};

//合约列表
enum InstrumentField
{
	INSTRUMENT_FIELD_ID_PRODUCTID,
	INSTRUMENT_FIELD_ID_INSTRUMENTID,
	INSTRUMENT_FIELD_ID_INSTRUMENTNAME,
	INSTRUMENT_FIELD_ID_EXCHANGE,
	INSTRUMENT_FIELD_ID_MULTIPLY,
	INSTRUMENT_FIELD_ID_PRICETICK,
	INSTRUMENT_FIELD_ID_PRODUCTCLASS,
	INSTRUMENT_FIELD_ID_EXPIREDATE,
	INSTRUMENT_FIELD_ID_LONGMARGINRATIO,
	INSTRUMENT_FIELD_ID_SHORTMARGINRATIO,
	INSTRUMENT_FIELD_ID_OPENRATIO,
	INSTRUMENT_FIELD_ID_CLOSERATIO,
	INSTRUMENT_FIELD_ID_CLOSETODAYRATIO,
	INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY,
	INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY,
	INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY,
	INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME,
	INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME,
	INSTRUMENT_FIELD_ID_CURRENCY,
	INSTRUMENT_FIELD_ID_TICKSPERPOINT,
	INSTRUMENT_FIELD_ID_TICKSIZE,
	INSTRUMENT_FIELD_ID_OptionStrikeRatioByMoney,       //期权行权手续费率
	INSTRUMENT_FIELD_ID_OptionStrikeRatioByVolume,      //期权行权手续费
	INSTRUMENT_FIELD_ID_OptionFixedMargin,              //期权保证金不变部分
	INSTRUMENT_FIELD_ID_OptionMiniMargin,               //期权最小保证金
};

//成交记录，成交统计
enum TradeRecordField
{
	TRADE_RECORD_FIELD_ID_INSTRUMENTID,
	TRADE_RECORD_FIELD_ID_DIRECTION,
	TRADE_RECORD_FIELD_ID_OPENCLOSE,
	TRADE_RECORD_FIELD_ID_TRADEPRICE,
	TRADE_RECORD_FIELD_ID_TRADEHAND,
	TRADE_RECORD_FIELD_ID_CHARGE,
	TRADE_RECORD_FIELD_ID_TRADETIME,
	TRADE_RECORD_FIELD_ID_TRADETYPE,
	TRADE_RECORD_FIELD_ID_INSURE,
	TRADE_RECORD_FIELD_ID_TRADENO,
	TRADE_RECORD_FIELD_ID_ORDERNO,
	TRADE_RECORD_FIELD_ID_EXCHANGE,
	TRADE_RECORD_FIELD_ID_TRADEAVGPRICE,
	TRADE_RECORD_FIELD_ID_ACCOUNT
};

//持仓
enum PositionField
{
	POSITION_FILED_ID_INSTRUMENTID,
	POSITION_FILED_ID_DIRECTION,
	POSITION_FIELD_ID_INSURE,
	POSITION_FILED_ID_TOTALPOSITION,
	POSITION_FIELD_ID_CLOSEVOLUMN,
	POSITION_FIELD_ID_LASTPRICE,
	POSITION_FIELD_ID_AVGPRICE,
	POSITION_FIELD_ID_AVGOPENPRICE,
	POSITION_FIELD_ID_PROFIT,
	POSITION_FIELD_ID_FLOATPROFIT,
	POSITION_FIELD_ID_TOTAL_PROFIT,
	POSITION_FIELD_ID_MARGIN,
	POSITION_FIELD_ID_PREPOSITION,
	POSITION_FIELD_ID_TODAYPOSITION,
	POSITION_FIELD_ID_BUYOPENPRICE,
	POSITION_FIELD_ID_SELLOPENPRICE,
	POSITION_FIELD_ID_TODAYOPENVOLUME,
	POSITION_FIELD_ID_TODAYCLOSEVOLUME,
	POSITION_FIELD_ID_TOTALBUYPOSITION,
	POSITION_FIELD_ID_TOTALSELLPOSITION,
	POSITION_FIELD_ID_TODAYBUYPOSITION,
	POSITION_FIELD_ID_TODAYSELLPOSITION,
	POSITION_FIELD_ID_TOTALTODAYCLOSEVOLUME,
	POSITION_FIELD_ID_TOTALPRECLOSEVOLUME,
	POSITION_FIELD_ID_TODAYWAITCLOSE,
	POSITION_FIELD_ID_WAITCLOSE,
	POSITION_FIELD_ID_TODAYENABLECLOSEVOLUME,
	POSITION_FIELD_ID_PREENABLECLOSEVOLUME,
	POSITION_FIELD_ID_EXCHANGE,

	POSITION_FIELD_ID_CURRENCY,
	POSITION_FIELD_ID_OPENPROFIT,
	POSITION_FIELD_ID_OPENAVGPRICE,
	POSITION_FIELD_ID_BUYVOLUMN,
	POSITION_FIELD_ID_SELVOLUMN,
	POSITION_FIELD_ID_TOTALPROFIT,
	POSITION_FIELD_ID_MARGINPERLOT,
	POSITION_FIELD_ID_PLBURNRATE,
	POSITION_FIELD_ID_OPENPOSITIONEXPOSURE,
	POSITION_FIELD_ID_BUYPOWERREMAIN,
	POSITION_FIELD_ID_BUYPOWERUSED,
	POSITION_FIELD_ID_TOTALMARGINPAID,
	POSITION_FIELD_ID_ACCOUNT,
	POSITION_FIELD_ID_CLOSEPROFIT
};

//组合持仓
enum ComboPositionField
{
	COMBO_POSITION_FIELD_ID_INSTRUMENT,
	COMBO_POSITION_FIELD_ID_DIRECTION,
	COMBO_POSITION_FIELD_ID_VOLUME,
	COMBO_POSITION_FIELD_ID_HEDGEFLAG,
	COMBO_POSITION_FIELD_ID_MARGIN,
	COMBO_POSITION_FIELD_ID_AVGPRICE,
	COMBO_POSITION_FIELD_ID_AVGOPENPRICE,
	COMBO_POSITION_FIELD_ID_ACCOUNT,
};

enum PositionDetailField
{
	POSITION_DETAIL_FIELD_ID_TRADEID,
	POSITION_DETAIL_FIELD_ID_INSTRUMENT,
	POSITION_DETAIL_FIELD_ID_DIRECTION,
	POSITION_DETAIL_FIELD_ID_VOLUME,
	POSITION_DETAIL_FIELD_ID_OPENPRICE,
	POSITION_DETAIL_FIELD_ID_MARGIN,
	POSITION_DETAIL_FIELD_ID_TYPE,
	POSITION_DETAIL_FIELD_ID_HEDGEFLAG,
	POSITION_DETAIL_FIELD_ID_EXCHANGE,
	POSITION_DETAIL_FILED_ID_OPENTIME,
	POSITION_DETAIL_FIELD_ID_LASTSETTLEMENTPRICE,
	POSITION_DETAIL_FIELD_ID_CLOSEVOLUME,
	POSITION_DETAIL_FIELD_ID_POSITIONPROFIT,
	POSITION_DETAIL_FIEL_ID_CLOSEPROFIT,
	POSITION_DETAIL_FIELD_FLUCTUATION,
	POSITION_DETAIL_FIELD_CURPRICE,
	POSITION_DETAIL_FIELD_ID_COMBSTATE,
	POSITION_DETAIL_FIELD_ID_COMBINSTRUMENT,
	POSITION_DETAIL_FIELD_ID_ACCOUNT,

	POSITION_DETAIL_FIELD_ID_MAX
};
//持仓汇总
enum MultiPositionField
{
	MULTIPOSITION_FIELD_ID_INSTRUMENTID,
	MULTIPOSITION_FIELD_ID_DIRECTION,
	MULTIPOSITION_FIELD_ID_HEDGEFLAG,
	MULTIPOSITION_FIELD_ID_TOTALPOSITION,
	MULTIPOSITION_FIELD_ID_CLOSEVOLUMN,
	MULTIPOSITION_FIELD_ID_LASTPRICE,
	MULTIPOSITION_FIELD_ID_AVGPRICE,
	MULTIPOSITION_FIELD_ID_AVGOPENPRICE,
	MULTIPOSITION_FIELD_ID_PROFIT,
	MULTIPOSITION_FIELD_ID_FLOATPROFIT,
	MULTIPOSITION_FIELD_ID_TOTAL_PROFIT,
	MULTIPOSITION_FIELD_ID_MARGIN,
	MULTIPOSITION_FIELD_ID_PREPOSITION,
	MULTIPOSITION_FIELD_ID_TODAYPOSITION,
	MULTIPOSITION_FIELD_ID_BUYOPENPRICE,
	MULTIPOSITION_FIELD_ID_SELLOPENPRICE,
	MULTIPOSITION_FIELD_ID_TODAYOPENVOLUME,
	MULTIPOSITION_FIELD_ID_TODAYCLOSEVOLUME,
	MULTIPOSITION_FIELD_ID_TOTALBUYPOSITION,
	MULTIPOSITION_FIELD_ID_TOTALSELLPOSITION,
	MULTIPOSITION_FIELD_ID_TODAYBUYPOSITION,
	MULTIPOSITION_FIELD_ID_TODAYSELLPOSITION,
	MULTIPOSITION_FIELD_ID_TOTALTODAYCLOSEVOLUME,
	MULTIPOSITION_FIELD_ID_TOTALPRECLOSEVOLUME,
	MULTIPOSITION_FIELD_ID_TODAYWAITCLOSE,
	MULTIPOSITION_FIELD_ID_WAITCLOSE,
	MULTIPOSITION_FIELD_ID_TODAYENABLECLOSEVOLUME,
	MULTIPOSITION_FIELD_ID_PREENABLECLOSEVOLUME,
	MULTIPOSITION_FIELD_ID_EXCHANGE,

	MULTIPOSITION_FIELD_ID_CURRENCY,
	MULTIPOSITION_FIELD_ID_OPENPROFIT,
	MULTIPOSITION_FIELD_ID_OPENAVGPRICE,
	MULTIPOSITION_FIELD_ID_BUYVOLUMN,
	MULTIPOSITION_FIELD_ID_SELVOLUMN,
	MULTIPOSITION_FIELD_ID_TOTALPROFIT,
	MULTIPOSITION_FIELD_ID_MARGINPERLOT,
	MULTIPOSITION_FIELD_ID_PLBURNRATE,
	MULTIPOSITION_FIELD_ID_OPENPOSITIONEXPOSURE,
	MULTIPOSITION_FIELD_ID_BUYPOWERREMAIN,
	MULTIPOSITION_FIELD_ID_BUYPOWERUSED,
	MULTIPOSITION_FIELD_ID_TOTALMARGINPAID,
	MULTIPOSITION_FIELD_ID_ACCOUNT,
	MULTIPOSITION_FIELD_ID_CLOSEPROFIT
};
//资金
enum FundAccountField
{
	FUNDACCOUNT_STATICPROFIT_COL_ID,				                //静态权益
	FUNDACCOUNT_POSITIONPROFIT_COL_ID,				                //持仓盈亏
	FUNDACCOUNT_CLOSEPROFIT_COL_ID,				                    //平仓盈亏
	FUNDACCOUNT_DYNAMICPROFIT_COL_ID,				                //动态权益
	FUNDACCOUNT_CURRMARGIN_COL_ID,                                  //占用保证金
	FUNDACCOUNT_COMMISSION_COL_ID,                      		    //手续费 
	FUNDACCOUNT_FROZEN_MARGIN_COL_ID,							    //冻结保证金
	FUNDACCOUNT_FROZEN_COMMISSION_COL_ID,					        //冻结手续费
	FUNDACCOUNT_PREBALANCE_COL_ID,							        //上次结算准备金
	FUNDACCOUNT_PRECREDIT_COL_ID,					   		        //上次信用额度
	FUNDACCOUNT_PREMORTGAGE_COL_ID,		                	        //上次质押金额
	FUNDACCOUNT_MORTGAGE_COL_ID,				         			//质押金额 
	FUNDACCOUNT_FROZENCASH_COL_ID,			        			    //冻结资金
	FUNDACCOUNT_AVALIABLE_COL_ID,					   	            //可用资金
	FUNDACCOUNT_WITHDRAWQUOTA_COL_ID,						        //可取资金	  
	FUNDACCOUNT_RISKDEGREE_COL_ID,			            	        //风险度
	FUNDACCOUNT_CREDIT_COL_ID,					        	        //信用金额 
	FUNDACCOUNT_DEPOSITE_COL_ID,				        	        //今日入金 
	FUNDACCOUNT_WITHDRAW_COL_ID,			                		//今日出金


	FUNDACCOUNT_BPREMAINING_COL_ID,			         			    //剩余购买力
	FUNDACCOUNT_BPUSED_COL_ID,				         			    //占用的资金	  
	FUNDACCOUNT_EXPOSURE_COL_ID,				        		    //合约占用资金比率
	FUNDACCOUNT_BURNRATE_COL_ID,				            	    //合约的盈亏比率 
	FUNDACCOUNT_ACCOUNTID_COL_ID,										//账号
	FUNDACCOUNT_MARGINPAID_COL_ID,	
	FUNDACCOUNT_TOTALPROFIT_COL_ID,	//总盈亏
	FUNDACCOUNT_MAX_COL_ID,

  
};
//所有委托单
enum AllOrderPanelField
{
	ALLORDER_OrderLocalID,                      //报单编号
	ALLORDER_InstrumentID,                     //合约
	ALLORDER_Direction,                        //买卖
	ALLORDER_CombOffsetFlag,                   //开平
	ALLORDER_OrderStatus,                      //挂单状态
	ALLORDER_LimitPrice,                       //报单价格
	ALLORDER_VolumeTotalOriginal,              //报单手数
	ALLORDER_VolumeTotal,                      //未成交手数
	ALLORDER_VolumeTraded,                     //成交手数
	ALLORDER_StatusMsg,                        //详细状态
	ALLORDER_InsertTime,                      //报单时间                
	ALLORDER_UpdateTime,                      //最后成交时间
	ALLORDER_d_avgPrice,                      //成交均价   --根据成交回报的成交价计算
	ALLORDER_d_freezeMargin,                  //冻结保证金 --多空头保证金率*委托价
	ALLORDER_d_troubleMoney,                  //冻结手续费 --根据手续费率计算
	ALLORDER_CombHedgeFlag,                   //投保
	ALLORDER_ExchangeID,                      //交易所
	ALLORDER_BrokerOrderSeq,                  //序列号
	ALLORDER_OrderPriceType,                  //报单价格条件
	ALLORDER_TimeCondition,                   //有效期类型
	ALLORDER_OrderType,                       //报单类型
	ALLORDER_UserForceClose,                  //是否强平
	ALLORDER_ForceCloseReason,               //强平信息
	ALLORDER_OrderSubmitStatus,               //指令结果
	ALLORDER_UserProductInfo,                 //客户端信息
	ALLORDER_Account,
	ALLORDER_MAX
};
//未成交单
enum OpenOrderField
{
	OPENORDER_OrderLocalID,                     //报单编号
	OPENORDER_InstrumentID,                     //合约
	OPENORDER_Direction,                        //买卖
	OPENORDER_CombOffsetFlag,                   //开平
	OPENORDER_VolumeTotalOriginal,              //报单手数
	OPENORDER_VolumeTotal,                      //未成交手数
	OPENORDER_LimitPrice,                       //报单价格
	OPENORDER_InsertTime,                      //报单时间  
	OPENORDER_d_freezeMargin,                  //冻结保证金 --多空头保证金率*委托价
	OPENORDER_d_troubleMoney,                  //冻结手续费 --根据手续费率计算
	OPENORDER_BrokerOrderSeq,                  //序列号
	OPENORDER_ExchangeID,                      //交易所 
	OPENORDER_CombHedgeFlag,                   //投保
	OPENORDER_VolumeTraded,                     //成交手数
	OPENORDER_OrderPriceType,                  //报单价格条件
	OPENORDER_TimeCondition,                   //有效期类型
	OPENORDER_OrderType,                       //报单类型
	OPENORDER_UpdateTime,                      //最后成交时间
	OPENORDER_d_avgPrice,                      //成交均价   --根据成交回报的成交价计算
	OPENORDER_UserForceClose,                  //是否强平
	OPENORDER_ForceCloseReason,               //强平信息 
	OPENORDER_OrderSubmitStatus,               //指令结果
	OPENORDER_OrderStatus,                      //挂单状态
	OPENORDER_StatusMsg,                        //详细状态
	OPENORDER_UserProductInfo,                 //客户端信息
	OPENORDER_Price2,                         //价格2
	OPENORDER_ACCOUNT
};

///执行宣告
enum ExecOrderPanelField {
	EXECORDER_BrokerID,						///经纪公司代码
	EXECORDER_InvestorID,					///投资者代码
	EXECORDER_InstrumentID,					///合约代码
	EXECORDER_ExecOrderRef,					///执行宣告引用
	EXECORDER_UserID,						///用户代码
	EXECORDER_Volume,						///数量
	EXECORDER_RequestID,					///请求编号
	EXECORDER_BusinessUnit,					///业务单元
	EXECORDER_OffsetFlag,					///开平标志
	EXECORDER_HedgeFlag,					///投机套保标志
	EXECORDER_ActionType,					///执行类型
	EXECORDER_PosiDirection,				///保留头寸申请的持仓方向
	EXECORDER_ReservePositionFlag,			///期权行权后是否保留期货头寸的标记
	EXECORDER_CloseFlag,					///期权行权后生成的头寸是否自动平仓
	EXECORDER_ExecOrderLocalID,				///本地执行宣告编号
	EXECORDER_ExchangeID,					///交易所代码
	EXECORDER_ParticipantID,				///会员代码
	EXECORDER_ClientID,						///客户代码
	EXECORDER_ExchangeInstID,				///合约在交易所的代码
	EXECORDER_TraderID,						///交易所交易员代码
	EXECORDER_InstallID,					///安装编号
	EXECORDER_OrderSubmitStatus,			///执行宣告提交状态
	EXECORDER_NotifySequence,				///报单提示序号
	EXECORDER_TradingDay,					///交易日
	EXECORDER_SettlementID,					///结算编号
	EXECORDER_ExecOrderSysID,				///执行宣告编号
	EXECORDER_InsertDate,					///报单日期
	EXECORDER_InsertTime,					///插入时间
	EXECORDER_CancelTime,					///撤销时间
	EXECORDER_ExecResult,					///执行结果
	EXECORDER_ClearingPartID,				///结算会员编号
	EXECORDER_SequenceNo,					///序号
	EXECORDER_FrontID,						///前置编号
	EXECORDER_SessionID,					///会话编号
	EXECORDER_UserProductInfo,				///用户端产品信息
	EXECORDER_StatusMsg,					///状态信息
	EXECORDER_ActiveUserID,					///操作用户代码
	EXECORDER_BrokerExecOrderSeq,			///经纪公司报单编号
};

// 新建下单面板
enum OrderNewInputField
{
	ORDERNEWINPUT_MINNUM,

	ORDERNEWINPUT_INSTRUMENTID_ROW_ID,								//合约ID
	ORDERNEWINPUT_BUYSELL_ROW_ID,									//买卖方向
	ORDERNEWINPUT_OPENCLOSE_ROW_ID,									//开平
	ORDERNEWINPUT_PRICE_ROW_ID,										//价格
	ORDERNEWINPUT_VOLUME_ROW_ID,									//手数

	ORDERNEWINPUT_TRADEACCOUNT_ROW_ID,		              			//交易账号

	ORDERNEWINPUT_ORDERTYPE_ROW_ID,		              				//Pats:下单类型
	ORDERNEWINPUT_LIMITPRICE_ROW_ID,		           				//Pats:限价

	ORDERNEWINPUT_MAXNUM,
};

//查批量
enum OrderBatchField
{
	ORDERBATCH_MIN,						//最小
	ORDERBATCH_GroupID,					//组号
	ORDERBATCH_SubID,					//组内号
	ORDERBATCH_RequestID,				//委托号
	ORDERBATCH_RequestState,			//委托状态
	ORDERBATCH_InstrumentID,			//合约代码
	ORDERBATCH_Direction,				//买卖
	ORDERBATCH_OCMode,					//开仓平仓
	ORDERBATCH_Hedge,					//投机、保值
	ORDERBATCH_RegVolume,				//委托手数
	ORDERBATCH_RegPrice,				//委托价格
	ORDERBATCH_MAX,						//最大
};

//查盈损单
enum OrderPositionField
{
	ORDERPOSITION_MIN,						//最小
	ORDERPOSITION_PositionID,				//止盈止损单号
	ORDERPOSITION_OpenID,					//开仓委托号
	ORDERPOSITION_State,					//状态
	ORDERPOSITION_ConditionID,				//条件单号
	ORDERPOSITION_InstrumentID,				//合约代码
	ORDERPOSITION_Direction,				//买卖
	ORDERPOSITION_RegVolume,				//委托手数
	ORDERPOSITION_RegPrice,					//委托价格
	ORDERPOSITION_OpenedPriceOffset,		//成交价偏移
	ORDERPOSITION_StopGainPrice,			//止盈价
	ORDERPOSITION_StopLosePrice,			//止损价
	ORDERPOSITION_StopGainForecast,			//止盈预测
	ORDERPOSITION_StopLoseForecast,			//止损预测
	ORDERPOSITION_PriceType,				//触发价格类型
	ORDERPOSITION_CloseMode,				//平仓方式
	ORDERPOSITION_ClosePriceOffset,			//平仓反向差
	ORDERPOSITION_ReqState,					//开仓报单状态
	ORDERPOSITION_StopGainPriceOffset,		//止盈价
	ORDERPOSITION_StopLosePriceOffset,		//止损价
	ORDERPOSITION_MAX,						//最大
};

//持仓止盈止损
enum OrderPositionPLField
{
	ORDERPOSIPL_MIN,						//最小
	ORDERPOSIPL_PosiPLID,					//持仓止盈止损编号
//	ORDERPOSIPL_PLID,						//止盈止损单号
//	ORDERPOSIPL_OpenID,						//开仓委托号
//	ORDERPOSIPL_TraderID,					//成交编号
	ORDERPOSIPL_State,						//状态
	ORDERPOSIPL_ConditionID,				//条件单号
	ORDERPOSIPL_StopProfitState,			//止盈状态
	ORDERPOSIPL_StopLoseState,				//止损状态
	ORDERPOSIPL_InstrumentID,				//合约代码
//	ORDERPOSIPL_Direction,					//买卖
	ORDERPOSIPL_CloseDirection,				//平仓方向
	ORDERPOSIPL_ReqVolume,					//委托手数
	ORDERPOSIPL_ReqPrice,					//委托价格
//	ORDERPOSIPL_OpenedPriceOffset,			//成交价偏移
	ORDERPOSIPL_StopGainPrice,				//止盈价
	ORDERPOSIPL_StopLosePrice,				//止损价
	ORDERPOSIPL_StopGainClosePrice,			//止盈平仓价
	ORDERPOSIPL_StopLoseClosePrice,			//止损平仓价
	ORDERPOSIPL_StopGainForecast,			//止盈预测
	ORDERPOSIPL_StopLoseForecast,			//止损预测
	ORDERPOSIPL_PriceType,					//触发价格类型
	ORDERPOSIPL_CloseMode,					//平仓方式
	ORDERPOSIPL_ClosePriceOffset,			//平仓反向差
	ORDERPOSIPL_MAX,						//最大
};

//盈损单明细
enum OrderPLDetailField
{
	ORDERPLDETAIL_MIN,						//最小
	ORDERPLDETAIL_PLDetailID,				//盈损明细单号
	ORDERPLDETAIL_PLID,						//止盈止损单号
	ORDERPLDETAIL_OpenID,					//开仓委托号
	ORDERPLDETAIL_TraderID,					//成交编号
	ORDERPLDETAIL_State,					//状态
	ORDERPLDETAIL_ConditionID,				//条件单号
	ORDERPLDETAIL_StopProfitState,			//止盈状态
	ORDERPLDETAIL_StopLoseState,			//止损状态
	ORDERPLDETAIL_InstrumentID,				//合约代码
	ORDERPLDETAIL_Direction,				//买卖
	ORDERPLDETAIL_CloseDirection,			//平仓方向
	ORDERPLDETAIL_RegVolume,				//委托手数
	ORDERPLDETAIL_RegPrice,					//委托价格
	ORDERPLDETAIL_OpenedPriceOffset,		//成交价偏移
	ORDERPLDETAIL_StopGainPrice,			//止盈价
	ORDERPLDETAIL_StopLosePrice,			//止损价
	ORDERPLDETAIL_StopGainClosePrice,		//止盈平仓价
	ORDERPLDETAIL_StopLoseClosePrice,		//止损平仓价
	ORDERPLDETAIL_StopGainForecast,			//止盈预测
	ORDERPLDETAIL_StopLoseForecast,			//止损预测
	ORDERPLDETAIL_PriceType,				//触发价格类型
	ORDERPLDETAIL_CloseMode,				//平仓方式
	ORDERPLDETAIL_ClosePriceOffset,			//平仓反向差
	ORDERPLDETAIL_MAX,						//最大
};

//条件单
enum OrderConditionField
{
	ORDERCONDITION_MIN,						//最小
	ORDERCONDITION_ConditionID,				//条件单号
	ORDERCONDITION_State,					//状态
	ORDERCONDITION_EntrustID,				//委托号
	ORDERCONDITION_RequestState,			//委托状态
	ORDERCONDITION_InstrumentID,			//合约代码
	ORDERCONDITION_Direction,				//买卖
	ORDERCONDITION_OCMode,					//开仓平仓
	ORDERCONDITION_Hedge,					//投机、保值
	ORDERCONDITION_RegVolume,				//申报手数
	ORDERCONDITION_RegPrice,				//申报价格
	ORDERCONDITION_ConditionPrice,			//触发价格                
	ORDERCONDITION_DoCondition,				//触发条件
	ORDERCONDITION_DeleteTime,				//删除时间
	ORDERCONDITION_ErrorMsg,				//错误信息
	ORDERCONDITION_MAX,						//最大
};

//预埋单
enum
{
	ORDERPARKED_MIN,						//最小
	ORDERPARKED_ParkedID,					//预埋单号
	ORDERPARKED_State,						//状态
	ORDERPARKED_InstrumentID,				//合约代码
	ORDERPARKED_Direction,					//买卖
	ORDERPARKED_OCMode,						//开仓平仓
	ORDERPARKED_Hedge,						//投机、保值
	ORDERPARKED_RegVolume,					//申报手数
	ORDERPARKED_RegPrice,					//申报价格
	ORDERPARKED_CreateTime,					//预埋时间
	ORDERPARKED_DeleteTime,					//删除时间
	ORDERPARKED_ErrorMsg,					//错误信息
	ORDERPARKED_MAX	,						//最大
};

//多账号
enum MultiAccountField
{
	MULTIACCOUNT_FIELD_ID_MIN,
	MULTIACCOUNT_FIELD_ID_VILID,			//有效
	MULTIACCOUNT_FIELD_ID_ID,				//ID
	MULTIACCOUNT_FIELD_ID_INVESTOR,			//投资者
	MULTIACCOUNT_FIELD_ID_ACCOUNT,			//投资者账号
	MULTIACCOUNT_FIELD_ID_STATUS,			//登录状态
	MULTIACCOUNT_FIELD_ID_USEQUOT,			//行情
	MULTIACCOUNT_FIELD_ID_AVALIABLE,		//可用资金
	MULTIACCOUNT_FIELD_ID_PREBALANCE,		//上次结算准备金
	MULTIACCOUNT_FIELD_ID_STATICPROFIT,		//静态权益
	MULTIACCOUNT_FIELD_ID_DYNAMICPROFIT,	//动态权益
	MULTIACCOUNT_FIELD_ID_POSITIONPROFIT,	//持仓盈亏
	MULTIACCOUNT_FIELD_ID_CLOSEPROFIT,		//平仓盈亏
	MULTIACCOUNT_FIELD_ID_RISKDEGREE,		//风险度
	// 20131014新增字段
	MULTIACCOUNT_FIELD_ID_CURRMARGIN,			//占用保证金
	MULTIACCOUNT_FIELD_ID_COMMISSION,			//手续费 
	MULTIACCOUNT_FIELD_ID_FROZEN_MARGIN,		//冻结保证金
	MULTIACCOUNT_FIELD_ID_FROZEN_COMMISSION,	//冻结手续费
	MULTIACCOUNT_FIELD_ID_PRECREDIT,			//上次信用额度
	MULTIACCOUNT_FIELD_ID_PREMORTGAGE,			//上次质押金额
	MULTIACCOUNT_FIELD_ID_MORTGAGE,				//质押金额 
	MULTIACCOUNT_FIELD_ID_FROZENCASH,			//冻结资金
	MULTIACCOUNT_FIELD_ID_WITHDRAWQUOTA,		//可取资金	  
	MULTIACCOUNT_FIELD_ID_CREDIT,				//信用金额 
	MULTIACCOUNT_FIELD_ID_DEPOSITE,				//今日入金 
	MULTIACCOUNT_FIELD_ID_WITHDRAW,				//今日出金

    //  20140402新增字段
	MULTIACCOUNT_FIELD_ID_ISFIRSTACCOUNT		//是否是首账号
};


enum MSG_TYPE
{
	INVALID_MSG,
	MSG_INSTRUMENT_GROUP_CHANGE,
	MSG_DISPLAY_CHANGE,

	MSG_LIST_BAOJIABIAO_CHANGE,
	MSG_LIST_TRADE_RECORD_CHANGE,
	MSG_LIST_TRADE_STATIC_CHANGE,
	MSG_LIST_POSITION_CHANGE,
	MSG_LIST_POSITION_DETAIL_CHANGE,
	MSG_LIST_COMBO_POSITION_CHANGE,
	MSG_LIST_QUERY_ORDER_POSITION_CHANGE,
	MSG_LIST_QUERY_ORDER_BATCH_CHANGE,
	MSG_LIST_QUERY_ORDER_CONDITION_CHANGE,
	MSG_LIST_QUERY_ORDER_PARKED_CHANGE,
	MSG_LIST_ORDER_INSERT_CHANGE,
	MSG_LIST_ACCOUNT_CHANGE,
	MSG_LIST_ALL_ORDER_CHANGE,
	MSG_LIST_OPEN_ORDER_CHANGE,
	MSG_LIST_INSTRUMENT_CHANGE,
	MSG_LIST_MULTIACCOUNT_CHANGE,
	MSG_LIST_MULTIPOSITION_CHANGE,
	MSG_ALL_LIST_CHANGE,

	MSG_RISK_MGR_CHANGE,
	MSG_ORDER_CHANGE,
	MSG_CANCEL_ORDER_CHANGE,
	MSG_VARIETYINFO_CHANGE,

	MSG_LIST_ORDER_PLDETAIL,
	MSG_LIST_ORDER_POSIPL,

    MSG_USER_GROUP_CHANGE,
};

#define STRING_MSG_SEPERATOR _("-")	

#define COLOR_DEEP_GREEN	16777215
#define COLOR_DEEP_BLUE		1120416
#define COLOR_RED_BROWN		50331647
#define COLOR_GREEN			33554432
#define COLOR_PURPLE		46715092
#define COLOR_DEEP_PURPLE	33619967
#define COLOR_DARK			12910522
#define COLOR_BLUE			16765183
#define COLOR_BUY			16765183
#define COLOR_SELL			12910522


#define NEEDSIMULATEMARKET(str) (DEFAULT_SVR()&&DEFAULT_SVR()->HaveOrderType(UIOT_CMD_SIMULATEMARKET) ? stricmp(str, "CZCE")!=0 : false)

#ifdef _USE_MULTI_LANGUAGE

#define PROCESS_LANGUAGE_EVENT()\
	do{wxCommandEvent myEvent(wxEVT_LANGUAGE_CHANGE);\
	myEvent.SetInt(SVR_LANGUAGE);\
	ProcessEvent(myEvent);}while(0)	
#define TRANSFORM_COLUMN_TEXT(gid,p)\
	do{\
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); \
		wxExtListItem item;\
		int count=pCfgMgr->GetListVisibleColCount(gid);\
		for(int i=0;i<count; i++)\
		{\
			p->GetColumn(i,item);\
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(gid, i);\
			item.SetText(pCfgMgr->GetColCfgCaption(gid,pColCfg->id));\
			p->SetColumn(i,item);	\
		}\
	}while(0)

#else
#define PROCESS_LANGUAGE_EVENT() 
#define TRANSFORM_COLUMN_TEXT(gid,p)
#endif

#define GET_JPG_FILE_RESOURCE(lpname,lptype,retbmp)\
do{\
    wxImage::AddHandler(new wxJPEGHandler); \
    wxImage::AddHandler(new wxBMPHandler);\
    DWORD lpSize=0;\
    void* pFileBuffer=GlobalFunc::GetXmlFileResource(lpname,lptype,&lpSize);\
    if(pFileBuffer)\
    {\
        wxImage image;\
        wxMemoryInputStream stream(pFileBuffer, lpSize);\
        image.LoadFile(stream, wxBITMAP_TYPE_JPEG);\
        retbmp = wxBitmap(image);\
    }\
}while(0)

#endif

