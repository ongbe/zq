
#pragma once


#define CONSOLE_MAGIC_NUMBER  0x4946


#define CONSOLE_INPUT_ORDER 1		// 下单
#define CONSOLE_INPUT_ACTION 2		// 撤单
#define CONSOLE_INPUT_RANDAM 3		// 随机


enum {
	CONSOLE_CMD_INPUTSIMPLE,		// 简易下单指令
	CONSOLE_CMD_EXITFASTTRADER,		// 退出FastTrader进程
};

enum {
	CONSOLE_RSP_HWND,		//返回交易客户端的主窗口句柄
	CONSOLE_RSP_DOORDER,	//返回交易客户端的下单/撤单执行结果
};


// console通知的下单
typedef struct {
	int		nSeqNum;			// 命令对应的SeqNum
	int		nInputType;			// 下单类型，下单、撤单、随机
	int		nQty;				// 下单数量，-1 随机，>=0 指定数量
	int		nMaxQty;			// 下单数量上限，-1 随机，>=0 指定数量
	char 	OpenClose;			// ctp的开平仓，0 随机，'0' 开仓，'1' 平仓
	char 	Direction;			// ctp的方向，0 随机，'0' 买，'1' 卖
	char	OrderPrice;			// 挂单模式，0 随机，'0' 要求成交，'1' 要求挂单
	char	CancelMode;			// 撤单模式，0 随机，'0' 撤一笔挂单，'1' 撤所有挂单
	char	InstrumentID[32];	// ctp合约ID
} CMD_INPUTSIMPLE;


// 由FastTrader返回给Console的通讯句柄
typedef struct {
	DWORD	dwProcID;	// FastTrader进程ID
	HWND	hWnd;		// FastTrader主窗口句柄
} RSP_FASTTRADERHWND;


// 由FastTrader返回给Console的下单撤单结果
typedef struct {
	DWORD	nSeqNum;	// 命令对应的SeqNum
	int		nResult;	// FastTrader处理结果，0 接收，1 下发api，2 拒绝下发api
} RSP_DOORDER;

