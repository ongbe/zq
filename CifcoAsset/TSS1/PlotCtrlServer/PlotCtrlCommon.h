
#pragma once

///什么都不能做
#define OPC_DirectMode_Nothing '0'
///只能开多
#define OPC_DirectMode_OnlyLong '1'
///只能开空
#define OPC_DirectMode_OnlyShort '2'
///可开多可开空
#define OPC_DirectMode_LongShort '3'

/////////////////////////////////////////////////////////////////////////
///TFtdcInstrumentIDType是一个合约代码类型
/////////////////////////////////////////////////////////////////////////
typedef char TInstrumentIDType[31];
/////////////////////////////////////////////////////////////////////////
///TPlotIDType是一个策略代码类型
/////////////////////////////////////////////////////////////////////////
typedef char TPlotIDType[31];


// 策略服务器订阅和推送下单控制命令
#define	Cmd_OPC_CtrlDirect_Min				0x200000			
#define	Cmd_OPC_CtrlDirect_MAX				0x2FFFFF			
#define	Cmd_OPC_SubscribeCtrlDirect_Req		0x200001	//订阅报单方向请求
#define	Cmd_OPC_SubscribeCtrlDirect_Rsp		0x200002	//订阅报单方向返回
#define	Cmd_OPC_CtrlDirect_Push				0x200003	//报单方向推送
#define	Cmd_OPC_UnSubscribeCtrlDirect_Req	0x200004	//退订报单方向请求		
#define	Cmd_OPC_UnSubscribeCtrlDirect_Rsp	0x200005	//退订报单方向返回

#define CONFIGFILENAME "plotCtrl.dat"

//风险事件数据结构
#pragma pack(push)
#pragma pack(1)

typedef struct {
	char				cDirectMode;
	TPlotIDType			plotID;
	TInstrumentIDType	inst;
} INSTRUMENTCTRLDIRECT, *PINSTRUMENTCTRLDIRECT;


// 策略合约配置结构
typedef struct {
	double				maxLoseForDay;			// 日内最大亏损额
	int					forceCloseTime;			// 强平倒计时
	int					cancelForceTime;		// 稳固时间
	int					fcPriceUp;				// 强平滑点
	DWORD				dwLongColor;
	DWORD				dwShortColor;
	DWORD				dwNoneColor;
	DWORD				dwLimitAllColor;
	int					plotItemCount;			// 附加的策略合约配置项
} PLOTCTRLHEADCFG, *PPLOTCTRLHEADCFG;


// 策略合约配置结构
typedef struct {
	TPlotIDType			plotID;
	TInstrumentIDType	inst;
	int					stopGainPoint;
	int					stopLosePoint;
	int					maxPosiVol;
	int					singlePosiTime;
} PLOTCTRLITEMCFG, *PPLOTCTRLITEMCFG;

#pragma pack(pop)
