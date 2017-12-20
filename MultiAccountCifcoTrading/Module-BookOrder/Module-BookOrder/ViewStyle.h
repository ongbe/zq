// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once


// 单元格颜色结构体
typedef struct {
	DWORD dwBackColor;			// 背景色
	DWORD dwTextColor;			// 文本色
} CELLCOLOR, *PCELLCOLOR;

// 行结构颜色结构体
typedef struct {
	CELLCOLOR tFixedRow;		// 固定行
	CELLCOLOR tRow;				// 一般或奇数行
	CELLCOLOR tEvenRow;			// 偶数行
} ROWCOLOR, *PROWCOLOR;

// PriceGrid颜色结构体
typedef struct {
	ROWCOLOR	tWaitBuyCol;	// 申买挂单数列的行颜色配置
	ROWCOLOR	tAskCol;		// 申买量列的行颜色配置
	ROWCOLOR	tPriceCol;		// 申价格列的行颜色配置
	ROWCOLOR	tBidCol;		// 申卖量列的行颜色配置
	ROWCOLOR	tWaitSellCol;	// 申卖挂单数列的行颜色配置

	CELLCOLOR	tCellLastPrice;	// 最新价单元格颜色配置
	CELLCOLOR	tCellRisePrice;	// 上涨颜色配置
	CELLCOLOR	tCellFallPrice;	// 下跌颜色配置
} PRICEGRIDCOLOR, *PPRICEGRIDCOLOR;

// BookOrder对话框的颜色结构体
typedef struct {
	DWORD		dwDlgBackColor;	// 对话框背景色
	DWORD		dwDlgButtonBackColor;	// 对话框前景色，主要用于控件的背景色
	DWORD		dwDlgEditBackColor;	// 对话框前景色，主要用于控件的背景色
	ROWCOLOR	tAccount;
	ROWCOLOR	tPosition;
	PRICEGRIDCOLOR tPrice;
} BOOKORDERDLGCOLOR, *PBOOKORDERDLGCOLOR;

BOOL GetColorStyle(int nStyle, BOOKORDERDLGCOLOR& colorStyle);