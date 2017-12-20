#ifndef GUI_CFG_GLOBAL_H
#define GUI_CFG_GLOBAL_H

#include <string>
#include <vector>
#include "../inc/Module-Misc/tinyxml.h"
#include "../ISvr/PlatformStruct.h"
#include "zqType.h"

typedef char ZQFontType[32];
typedef char ZQShortKeyType[32];

enum _style
{
	STYLE_Q7=0,
	STYLE_SIMPLE,
	STYLE_MOUSE,
	STYLE_SunGard
};

class BaseCfgMgr
{
public:
	virtual bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false )=0;
	virtual bool SaveCfg(TiXmlElement *root)=0;
	virtual void ClearCfg() = 0;
};

//显示单独一个配置文件（包括多个方案（默认，当前）分多个文件存储）
/************主窗体**begin********************************************************************************************************/

typedef struct tagMainWindowCfg
{
	zq_bool	bIsTop;					//是否置顶
	zq_bool	bIsMinTray;				//是否最小化到托盘
	zq_bool	bAutoLock;				//自动锁定时间（单位：秒）
	zq_int	nColorPlan;				//配色方案
	zq_bool	bFixPanel;               //固定板块
	zq_bool bShowPanelShortKey;      //板块标题显示快捷键  
	zq_int	nLogonVerify;			//登录验证 0:不需要验证 1:数字字母组合验证 2:数字验证
	zq_bool	bDelInstrumentWarning;	//删除自选合约是否弹出提示框
	zq_int	TradeInfoDlgSize;		//TradeInfoDlg的大小
	zq_bool	bModuleIsQry;	        //1-各个模块使用成交回报触发查询的方式，0表示不使用
}MAIN_WINDOW_CFG,*LPMAIN_WINDOW_CFG;

/************主窗体**end********************************************************************************************************/

enum GUI_MODULE_ID
{
	QUOT_TABLE_MODULE_ID = 1001,		//报价表
	ORDER_PANEL_MODULE_ID,				//标准下单板
	THRADE_RECORD_MODULE_ID,			//成交记录模块
	THRADE_STATISTICS_MODULE_ID,		//成交统计
	POSITION_MODULE_ID,					//持仓
	POSITION_DETAIL_MODULE_ID,			//持仓明细
	INSTRUMENT_MODULE_ID,				//合约列表
	ALL_TRUST_ORDER_MODULE_ID,          //所有委托单
	WAIT_TRADE_MODULE_ID,               //未成交
	COMBINATION_POSITION_MODULE_ID,     //组合持仓
	ACCOUNT_MODULE_ID,					//资金帐户
	TASK_MODULE_ID,						//任务

	QUOT_PANEL_MODULE_ID				//报价块
};

typedef struct
{
	DWORD GroupId;
	std::string GroupName;
	std::vector<std::string>  InstrVec;
}GroupInfo2;

//list配置界面信息配置
/************list**begin********************************************************************************************************/

typedef struct tagListTableCfg
{	
	int				Id;							
	ZQFontType		szFont;							//字体设置，如“underlined bold 14”
	int				TextSize;						//字体大小
	unsigned long	TextColor;						//文字颜色
	unsigned long	HeadColor;						//标题文字颜色
	unsigned long	HeadBackgroundColor;			//标题底色

	unsigned long	BackgroundColor;				//底色
	unsigned long	EvenLineBackgroundColor;        //偶行底色
	unsigned long	OddLineBackgroundColor;         //奇行底色

	int				HeadVisibility;                 //标题是否隐藏
	int				Visibility;						//是否可见

	tagListTableCfg()
	{
		memset(this, 0, sizeof(tagListTableCfg));
	}
} LIST_TABLE_CFG, *LPLIST_TABLE_CFG;

//list列信息配置
typedef struct tagListColumnCfg
{
	int				id;								//列ID
	int				Width;                          //显示宽度，默认为100
	unsigned long	TextColor;					    //列文字颜色
	int				Visibility;                     //是否可见

	tagListColumnCfg()
	{
		memset(this, 0, sizeof(tagListColumnCfg));
	}
} LIST_COLUMN_CFG, *LPLIST_COLUMN_CFG;

/************list**end********************************************************************************************************/
/************合约组**begin********************************************************************************************************/
#define GROUPNAME_LENGTH 63
typedef struct tagInstrumentGroupCfg
{
    tagInstrumentGroupCfg()
    {
        memset(this, 0, sizeof(tagInstrumentGroupCfg));
    }
	int			GroupId;			//组ID
	char		GroupName[GROUPNAME_LENGTH+1];			//组名称
	bool		IsVisibility;		//是否显示
}INSTRUMENT_GROUP_CFG, *LPINSTRUMENT_GROUP_CFG;

//合约组合约成员信息
typedef struct tagInstrumentGroupMemberInfoCfg
{
	InstrumentIDType	szInstrumentID;		//合约ID
	int		    PositionIndex;      //在合约组中显示的排序位置

	tagInstrumentGroupMemberInfoCfg()
	{
		memset(this, 0, sizeof(tagInstrumentGroupMemberInfoCfg));
	}
}INSTRUMENT_GROUP_MEMBER_INFO_CFG, *LPINSTRUMENT_GROUP_MEMBER_INFO_CFG;

/************合约组**end********************************************************************************************************/
/************下单**begin********************************************************************************************************/

//自动开平配置
enum AUTO_CLOSE_MODE
{
	ALL_CLOSE = 1001,                                    //按持仓量全平。
	DEFAUlT_VOLUME_CLOSE_ABANDON,						 //按默认手数平仓，超出持仓量部分不处理。
	DEFAUlT_VOLUME_CLOSE_OPEN,							 //按默认手数平仓，超出持仓量部分自动反向开仓。
};

enum AUTO_CLOSE_PRE_ORDER_MODE
{
	IGNORE_PRE_ORDER = 1001,							 //不处理
	AUTO_CANCEL_PRE_ORDER,								 //每次平仓时自动撤销原平仓单
	INSUFFICIENT_AUTO_CANCEL_PRE_ORDER,					 //可平不足时自动撤销原平仓单
};

//下单倍率
typedef struct tagOrderMultiply
{
	ZQShortKeyType ShortKey;
	int Volume;

	tagOrderMultiply()
	{
		memset(this, 0, sizeof(tagOrderMultiply));
	}
}OrderMultiply;


typedef struct tagKeyOrderInfo
{
	ZQShortKeyType ShortKey;
	int nBuySell;	//0 买入 1 卖出
	int nPrice;
	int nOverPrice;
	int nAction;
	int nActionArea;

	tagKeyOrderInfo()
	{
		memset(this, 0, sizeof(tagKeyOrderInfo));
	}
}KeyOrderInfo;

typedef struct tagOrderCfg
{
	zq_bool		bBuy5Sell5;							// 是否显示五档行情
	zq_bool		bMouseInputInstrumentId;			// 是否可以用鼠标输入合约代码
	zq_bool		bInputAbbreviation;					// 可以输入缩写和拼音
	zq_int		nContractListStyle;					//合约弹出框风格
	zq_bool		bOrderConfirm;						//下单前确认
	zq_bool     bAutoOpenClose;						//固定使用自动开平
	zq_bool     b13Change;							//13切换
	zq_bool     bReTrack;							//继续跟盘
	zq_bool     bSimulate;							//是否模拟
	zq_int		nMKTPriceUse;						// 0 不计算 1 最新价 2 买卖价
	zq_bool		bMouseInputPrice;					// 是否可以用鼠标输入价格
	zq_bool		bKeyBoardFastPrice;					// 键盘快速输入价格
	zq_bool		bMouseInputVolume;					// 是否可以用鼠标输入常用手数
	zq_int9		Volume;							// 常用手数的量
	InstrumentIDType Name[5];							//下单项目顺序

	//自动开平
	zq_int		ProcessPositionType;				//如果有持仓处理方式 1全平 2超出不处理 3超出方向开仓

	//键盘快速下单
	zq_bool		bOpenQuotKeyBoardOrder;				//是否启用键盘下单
	zq_bool		bSyncQuotPanel;						//发出报单是否填充下单块
	zq_bool		bReturnQuotTable;					//下单后光标是否回报价表
	int			KeyOrderDataNum;
	KeyOrderInfo KeyOrderData[100];					//键盘快速下单设置数据

	//单击报价 ,双击报价， 单击持仓，双击持仓
	//1发出报单(带确认)   2 发出报单(不带确认)  3 填单后光标不动
	//4 填单后光标到合约代码  5 填单后光标到价格 6  填单后光标开平
	//7 填单后光标到买卖   8 填单后光标到数量 9 填单后光标到下单按钮
	//10 ""
	zq_int 		ClickQuotTableAction;                                                       
	zq_int		DoubleClickQuotTableAction;			//双击报价
	zq_int		MouseFastOrderDirection;			//下单方向
	zq_bool		bCtrlReverse;						//是否允许Ctrl点击临时反转方向
	zq_bool		bAlwaysOpen;                        //是否鼠标点击报价开仓
	zq_int		ClickPositionAction;				//单击持仓
	zq_int		DoubleClickPositionAction;			//双击持仓
	zq_int		PositionVolume;                     //调整仓位数量

	//高级
	int			OrderMultiDataNum;
	OrderMultiply OrderMultiData[100];				//下单倍率数据

	//风险管理
	zq_int		VolumeLimit;							//手数限制
	zq_int		DifferPrice;							//偏离价位 
	zq_int		ActionAfterOrder;						//1,下单后不清空下单板2,全部清空3,清空价格和手数4只清空手数  

	//提示及反馈
	zq_bool		bOrderSuccessDlg;					// 下单成功 是否 弹框
	zq_bool		bOrderSuccessSound;					// 下单成功 是否 发声
	zq_bool		bOrderFailDlg;						// 下单失败 是否 弹框
	zq_bool		bOrderFailSound;					// 下单失败 是否 发声
	zq_bool		bTradeDlg;							// 挂单成交 是否 弹框
	zq_bool		bTradeSound;						// 挂单成交 是否 发声
	zq_long		SellColor;							//卖色
	zq_long		BuyColor;							//买色

	//快捷键
	zq_char		cBuyHotKey;							//买入快捷键
	zq_char		cSelHotKey;							//卖出快捷键
	zq_char		cOpenOrderHotKey;					//开仓快捷键
	zq_char		cCloseTodayVolumnHotKey;			//平今快捷键
	zq_char		cCloseOrderHotKey;					//平仓快捷键
	zq_char20	szOpenOrderBtnHotKey;			//下单按钮快捷键
	zq_char20	szOrderParkedBtnHotKey;			//预埋单按钮快捷键
	zq_int		nCursorPosAfterOrder;				//下单后光标定位位置 0,默认 1,合约 2,买卖 3,开平 4,价格 5,手数

	//快速下单设置
	zq_int		nAllNotHaveOppositePosition;		//都无反向持仓
	zq_int		nSomeHaveOppositePosition;			//部分有反向持仓
	zq_int		nAllHaveOppositePosition;			//全部有反向持仓
	zq_int		nMouseClosePositionStrategy;		//鼠标点持仓或持仓汇总
	zq_int		nCancelSameDirOpenOrderBeforeOrder;	//下单前先撤同方向挂单

	tagOrderCfg()
	{
		memset(this, 0, sizeof(tagOrderCfg));
	}
}ORDER_CFG, *LPORDER_CFG;

/************下单**end********************************************************************************************************/

/************撤单**begin********************************************************************************************************/
typedef struct tagCancelOrderCfg
{
	zq_bool		bDoubleClickMouseCancel;				//鼠标双击撤单
	zq_bool		bCancelConfirm;							//撤单前确认
	zq_bool		bHideCancelButton;			            //隐藏撤单按钮
	zq_bool		bHideCancelAllButton;			        //隐藏全撤按钮

	zq_bool		bOpenModifyOrder;				        //启用改单
	zq_bool		bModifyOrderConfirm;			    	//改单前确认

	zq_bool		bCancelSuccessDlg;						// 下单成功 是否 弹框
	zq_bool		bCancelSuccessSound;					// 下单成功 是否 发声

	zq_bool		bCancelFailDlg;							// 下单成功 是否 弹框
	zq_bool		bCancelFailSound;						// 下单成功 是否 发声

	zq_bool		bOpenKeyBoardCancelOrder;				//是否启用报价表键盘下单
	zq_bool	    bReturnQuotTable;						//下单后光标是否回报价表
	zq_char20	szCancelOrderBtnHotKey;				//撤单按钮快捷键
	zq_char20	szCancelAllOrderBtnHotKey;			//全部撤单按钮快捷键

	tagCancelOrderCfg()
	{
		memset(this, 0, sizeof(tagCancelOrderCfg));
	}
}CANCEL_ORDER_CFG, *LPCANCEL_ORDER_CFG;
/************撤单**end********************************************************************************************************/

/************资金账户**begin********************************************************************************************************/
typedef struct tagFundAccountCfg
{
	zq_long	TextColor;			//文字颜色
	zq_long	BgColor;			//底色	
}FUND_ACCOUNT_CFG, *LPFUND_ACCOUNT_CFG;
/************资金账户**end********************************************************************************************************/


/************风险管理**begin********************************************************************************************************/

enum ORDER_PANEL_CLEAR_MODE
{
	CLEAR_ORDER_PANEL_NO = 2001,
	CLEAR_ORDER_PANEL_ALL,
	CLEAR_ORDER_PANEL_PRICE_VOLUME,
	CLEAR_ORDER_PANEL_VOLUME
};

typedef struct tagRiskMgrCfg
{
	zq_int SingleMaxVolume;
	zq_int ClearOrderPanelMode;
}RISK_MGR_CFG, *LPRISK_MGR_CFG;

/************风险管理**end********************************************************************************************************/

/***********快捷键**begin********************************************************************************************************/

//快捷键类型
enum SHORTCUTKEYS_TYPE
{
	SHORTCUTKEYS_UNDEFINE_TYPE = 0,					//无定义
	SHORTCUTKEYS_MAIN_CONTROL_TYPE = 3001,			//主窗口控件 切换快捷键
	SHORTCUTKEYS_INSTRUMENT_GROUP_TYPE,				//合约组     切换快捷键
	SHORTCUTKEYS_ORDER_TYPE,						//下单类型   快捷键
	SHORTCUTKEYS_CANCEL_ORDER_TYPE,					//撤单类型   
	SHORTCUTKEYS_ORDER_TIMES_TYPE					//下单倍率
};


//下单动作
enum ORDER_ACTION
{
	ORDER_UNDEFINE = 0,						//无动作
	SEND_ORDER_CONFIRM = 4001,              //发出报单（带确认）
	SEND_ORDER_NOT_CONFIRM,					//发出报单（不带确认）
	FILL_ORDER,								//填单后光标不动
	FILL_ORDER_FOCUS_INSTRUMENT_ID,         //填单后光标到合约代码
	FILL_ORDER_FOCUS_DIRECTION,				//填单后光标到买卖
	FILL_ORDER_FOCUS_OPEN_CLOSE,			//填单后光标到开平
	FILL_ORDER_FOCUS_VOLUME,				//填单后光标到数量
	FILL_ORDER_FOCUS_PRICE,					//填单后光标到价格
	FILL_ORDER_FOCUS_ORDER_BUTTON			//填单后光标到下单按钮
};

//撤单动作
enum CANCEL_ORDER_ACTION
{
	CANCEL_ORDER_UNDEFINE = 0,              //无动作
	CANCEL_ORDER_SELECT = 5001,             //撤销选中单
	CANCEL_ORDER_LAST,						//撤销最后一个单
	CANCEL_ORDER_FIRST,						//撤销最早一个单
	CANCEL_ORDER_INSTRUMENT,				//按合约撤单
	CANCEL_ORDER_DIRECTION,					//按方向撤单
	CANCEL_ORDER_INSTRUMENT_DIRECTION,		//按合约及方向撤单
	CANCEL_ORDER_ALL,						//全部撤单
	MODIFY_ORDER_PRICE,						//改价格
	MODIFY_ORDER_VOLUME						//改手数
};

typedef struct tagPanelInfo
{
	int   id;     
	ZQShortKeyType    ShortKey;
	int   bVisiable;
	char AliasName[32];

	tagPanelInfo()
	{
		memset(this, 0, sizeof(tagPanelInfo));
	}
} PanelInfo;

typedef struct tagVarietyInfo
{
	int    volume;
	InstrumentIDType  InstruName;
	ZQShortKeyType  ShortKey;
	char  profit[20];
	char  loss[20];
	char  reverse[20];

	tagVarietyInfo()
	{
		memset(this, 0, sizeof(tagVarietyInfo));
	}
} VarietyInfo;

typedef struct tagNumberShortKey
{
	ZQShortKeyType ShortKey;
	char Symbol[100];
	char Comment[100];

	tagNumberShortKey()
	{
		memset(this, 0, sizeof(tagNumberShortKey));
	}
}NumberShortKey;

typedef struct tagSystemParamInfo
{
	zq_bool bWebCifco;			  //登录是否弹出网上中期客户服务平台
	zq_bool bUse;               //使用隐身热键
	zq_int prompt;              //提示声 1.不用 2.pC喇叭 3.多媒体喇叭
	zq_char32  key1;       //F1~F12
	zq_char32  key2;       //Alt键     空，不选此键
	zq_char32  key3;       //Ctrl键    空，不选此键
	zq_char32  key4;       //Shift键   空，不选此键
	zq_char32  key5;       //Window键  空，不选此键

	zq_int nTradeTimeSoundPrompt;	//开盘收盘及小结休息的声音提示
	zq_int nTradeEndSoundPrompt;	//有持仓的合约交易结束前的声音提示
	zq_int nSoundPromptBeforeTradeEndSec;	//有持仓的合约交易结束前声音提示提前秒数

	tagSystemParamInfo()
	{
		memset(this, 0, sizeof(tagSystemParamInfo));
	}
}SystemParamInfo;

typedef struct tagMovePosition
{
	zq_int nSourcePriceLimit;		//原始合约限价价格：0,市价 1,最新价 2买卖价
	zq_int nTargetPriceLimit;		//目标合约限价价格
	zq_int nSourcePriceOffset;		//原始合约滑动价差
	zq_int nTargetPriceOffset;		//目标合约滑动价差

	zq_int nOperateOrder;			//操作顺序
	zq_int nOutTimeSec;			//超时时间
	zq_int nRetryTimes;			//重试次数

	zq_int nSourceNotDealAction;	//原始合约未成交处理动作：0,无动作 1,按最新价浮动改单 2,按买卖价浮动改单 3,按市价改单 4,撤单
	zq_int nTargetNotDealAction;	//目标合约未成交处理动作
	zq_int nSourceNotDealPriceOffset;	//原始合约未成交处理滑动价差
	zq_int nTargetNotDealPriceOffset;	//目标合约未成交处理滑动价差
}MOVEPOSITIONCFG;

typedef struct tagReplacePosition
{
	zq_int nClosePositionPriceLimit1;		//仓位1平仓限价
	zq_int nClosePositionPriceOffset1;		//仓位1平仓滑动价差
	zq_int nOpenPositionPriceLimit1;		//仓位1开仓限价
	zq_int nOpenPositionPriceOffset1;		//仓位1开仓滑动价差

	zq_int nClosePositionPriceLimit2;		//仓位2平仓限价
	zq_int nClosePositionPriceOffset2;		//仓位2平仓滑动价差
	zq_int nOpenPositionPriceLimit2;		//仓位2开仓限价
	zq_int nOpenPositionPriceOffset2;		//仓位2开仓滑动价差

	zq_int nOperateOrder;					//操作顺序
	zq_int nOutTimeSec;					//超时时间
	zq_int nRetryTimes;					//重试次数

	zq_int nOpenPositionNotDealAction1;	//仓位1开仓未成交处理动作
	zq_int nOpenPositionNotDealPriceOffset1;	//仓位1开仓未成交处理滑动价差
	zq_int nClosePositionNotDealAction1;	//仓位1平仓未成交处理动作
	zq_int nClosePositionNotDealPriceOffset1;	//仓位1平仓未成交处理滑动价差

	zq_int nOpenPositionNotDealAction2;	//仓位2开仓未成交处理动作
	zq_int nOpenPositionNotDealPriceOffset2;	//仓位2开仓未成交处理滑动价差
	zq_int nClosePositionNotDealAction2;	//仓位2平仓未成交处理动作
	zq_int nClosePositionNotDealPriceOffset2;	//仓位2平仓未成交处理滑动价差
}REPLACEPOSITION;

struct AccountStru
{
	int nID;
	char szAccount[64];
};

enum OrderStrategy
{
	BaseMultipliedWay,
	UserDefinedVolumnWay,
	SumDividedWay,
	FirstAccountAlign,
	AccountScaleDivided,
};

enum QuickOrderStrategy
{
	QOrder_OpenDefaultAndStrategy = 1,		//按默认手数和策略开仓
	QOrder_CloseAll,						//按持仓量全平
	QOrder_AutoCloseDefaultAndNotOpen,		//有仓账户按默认手数和策略平仓，超出不处理；无仓账户不开仓
	QOrder_AutoReverseOpenAndNotOpen,		//有仓账户按默认手数和策略平仓，超出反向开仓；无仓账户不开仓
	QOrder_AutoCloseDefaultAndOpen,			//按默认手数和策略：有仓账户平仓，超出不处理；无仓账户开仓
	QOrder_AutoReverseOpenAndOpen,			//按默认手数和策略：有仓账户平仓，超出反向开仓；无仓账户开仓
	QOrder_AutoCloseDefault,				//按默认手数和策略平仓，超出不处理
	QOrder_AutoReverseOpen,					//按默认手数和策略平仓，超出反向开仓
};

typedef struct tagMULTIACCOUNT
{
	zq_int nSaveType;		//自动保存账号设置
	zq_int nLoadType;		//自动读取账号设置
	zq_int nSaveTypeManual;		//自动保存账号设置
	zq_int nLoadTypeManual;		//自动读取账号设置
	zq_int nStrategyItem;	//下单策略设置
	zq_int nFirstAccountAlign;	//按资金向首账户对齐
	zq_int nAccountScaleDivided;	//按账户规模分配手数
}MULTIACCOUNT;

#include <iostream>
#include <sstream>
#include <limits>
#pragma warning( disable : 4996 )



#endif