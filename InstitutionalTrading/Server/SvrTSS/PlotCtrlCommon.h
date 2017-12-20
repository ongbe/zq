
#pragma once

/////什么都不能做
//#define OPC_DirectMode_Nothing '0'
/////只能开多
//#define OPC_DirectMode_OnlyLong '1'
/////只能开空
//#define OPC_DirectMode_OnlyShort '2'
/////可开多可开空
//#define OPC_DirectMode_LongShort '3'

/////////////////////////////////////////////////////////////////////////
///TFtdcInstrumentIDType是一个合约代码类型
/////////////////////////////////////////////////////////////////////////
typedef char TInstrumentIDType[31];
/////////////////////////////////////////////////////////////////////////
///TPlotIDType是一个策略代码类型
/////////////////////////////////////////////////////////////////////////
typedef char TPlotIDType[31];
/////////////////////////////////////////////////////////////////////////
///TPLOTDIRECTType是一个策略控制方向类型
/////////////////////////////////////////////////////////////////////////
typedef char TPlotDirectType[31];


// 策略内部计算结构
typedef struct {
	int					nDirectMode;		// 0 双边，1 多，-1 空
	TPlotIDType			plotID;
	TInstrumentIDType	instID;
} INSTPLOTDIRECT, *PINSTPLOTDIRECT;

// 策略的导入结构
typedef struct {
	TPlotIDType			plotID;
	TInstrumentIDType	instID;
	TPlotDirectType		direct;				// Sell，ForbidOpenSell，Donotcare，ForbidOpenBuy，Buy
} IMPORTPLOTDIRECT, *PIMPORTPLOTDIRECT;

static int g_nPlotDirectCount = 5;
static char* g_pcsPlotDirect[] = { "sell", "forbidopensell", "donotcare", "forbidopenbuy", "buy" };