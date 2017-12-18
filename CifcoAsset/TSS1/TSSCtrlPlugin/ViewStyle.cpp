// Global.cpp : 只包括标准包含文件的源文件
// Module-BookOrder.pch 将作为预编译头
// Global.obj 将包含预编译类型信息

#include "stdafx.h"
#include "ViewStyle.h"

void GetColorStyle_1(BOOKORDERDLGCOLOR& colorStyle);
void GetColorStyle_2(BOOKORDERDLGCOLOR& colorStyle);
void GetColorStyle_3(BOOKORDERDLGCOLOR& colorStyle);
void GetColorStyle_4(BOOKORDERDLGCOLOR& colorStyle);

BOOL GetColorStyle(int nStyle, BOOKORDERDLGCOLOR& colorStyle)
{
	switch(nStyle)
	{
	case 1:
		GetColorStyle_1(colorStyle);
		return TRUE;
		break;
	case 2:
		GetColorStyle_2(colorStyle);
		return TRUE;
		break;
	case 3:
		GetColorStyle_3(colorStyle);
		return TRUE;
		break;
	case 4:
		GetColorStyle_4(colorStyle);
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

void GetColorStyle_1(BOOKORDERDLGCOLOR& colorStyle)
{
	colorStyle.dwDlgBackColor = RGB(246, 246, 246);
	colorStyle.dwDlgButtonBackColor = RGB(238, 238, 238);
	colorStyle.dwDlgEditBackColor = RGB(255, 255, 255);

	colorStyle.tAccount.tRow.dwBackColor = RGB(102, 100, 101);
	colorStyle.tAccount.tRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPosition.tFixedRow.dwBackColor = RGB(238, 238, 238);
	colorStyle.tPosition.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPosition.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwBackColor = RGB(238, 238, 238);
	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tBidCol.tFixedRow.dwBackColor = RGB(238, 238, 238);
	colorStyle.tPrice.tBidCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwBackColor = RGB(254, 246, 209);
	colorStyle.tPrice.tBidCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tEvenRow.dwBackColor = RGB(227, 220, 192);
	colorStyle.tPrice.tBidCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tPriceCol.tFixedRow.dwBackColor = RGB(238, 238, 238);
	colorStyle.tPrice.tPriceCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tAskCol.tFixedRow.dwBackColor = RGB(238, 238, 238);
	colorStyle.tPrice.tAskCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwBackColor = RGB(213, 251, 226);
	colorStyle.tPrice.tAskCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tEvenRow.dwBackColor = RGB(197, 224, 205);
	colorStyle.tPrice.tAskCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwBackColor = RGB(238, 238, 238);
	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitSellCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tCellLastPrice.dwBackColor = RGB(255, 128, 128);
	colorStyle.tPrice.tCellLastPrice.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tCellRisePrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellRisePrice.dwTextColor = RGB(255, 0, 0);
	colorStyle.tPrice.tCellFallPrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellFallPrice.dwTextColor = RGB(0, 160, 0);
}

void GetColorStyle_2(BOOKORDERDLGCOLOR& colorStyle)
{
	colorStyle.dwDlgBackColor = RGB(230, 241, 247);
	colorStyle.dwDlgButtonBackColor = RGB(182, 224, 248);
	colorStyle.dwDlgEditBackColor = RGB(255, 255, 255);

	colorStyle.tAccount.tRow.dwBackColor = RGB(102, 100, 101);
	colorStyle.tAccount.tRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPosition.tFixedRow.dwBackColor = RGB(182, 224, 248);
	colorStyle.tPosition.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPosition.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwBackColor = RGB(230, 240, 242);
	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tBidCol.tFixedRow.dwBackColor = RGB(230, 240, 242);
	colorStyle.tPrice.tBidCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwBackColor = RGB(254, 246, 209);
	colorStyle.tPrice.tBidCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tEvenRow.dwBackColor = RGB(227, 220, 192);
	colorStyle.tPrice.tBidCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tPriceCol.tFixedRow.dwBackColor = RGB(230, 240, 242);
	colorStyle.tPrice.tPriceCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tAskCol.tFixedRow.dwBackColor = RGB(230, 240, 242);
	colorStyle.tPrice.tAskCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwBackColor = RGB(213, 251, 226);
	colorStyle.tPrice.tAskCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tEvenRow.dwBackColor = RGB(197, 224, 205);
	colorStyle.tPrice.tAskCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwBackColor = RGB(230, 240, 242);
	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitSellCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tCellLastPrice.dwBackColor = RGB(255, 128, 128);
	colorStyle.tPrice.tCellLastPrice.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tCellRisePrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellRisePrice.dwTextColor = RGB(255, 0, 0);
	colorStyle.tPrice.tCellFallPrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellFallPrice.dwTextColor = RGB(0, 160, 0);
}

void GetColorStyle_3(BOOKORDERDLGCOLOR& colorStyle)
{
	colorStyle.dwDlgBackColor = RGB(253, 237, 237);
	colorStyle.dwDlgButtonBackColor = RGB(242, 230, 230);
	colorStyle.dwDlgEditBackColor = RGB(255, 255, 255);

	colorStyle.tAccount.tRow.dwBackColor = RGB(102, 100, 101);
	colorStyle.tAccount.tRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPosition.tFixedRow.dwBackColor = RGB(243, 191, 195);
	colorStyle.tPosition.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPosition.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwBackColor = RGB(242, 230, 230);
	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tBidCol.tFixedRow.dwBackColor = RGB(242, 230, 230);
	colorStyle.tPrice.tBidCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwBackColor = RGB(254, 246, 209);
	colorStyle.tPrice.tBidCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tEvenRow.dwBackColor = RGB(227, 220, 192);
	colorStyle.tPrice.tBidCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tPriceCol.tFixedRow.dwBackColor = RGB(242, 230, 230);
	colorStyle.tPrice.tPriceCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tAskCol.tFixedRow.dwBackColor = RGB(242, 230, 230);
	colorStyle.tPrice.tAskCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwBackColor = RGB(213, 251, 226);
	colorStyle.tPrice.tAskCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tEvenRow.dwBackColor = RGB(197, 224, 205);
	colorStyle.tPrice.tAskCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwBackColor = RGB(242, 230, 230);
	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitSellCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tCellLastPrice.dwBackColor = RGB(255, 128, 128);
	colorStyle.tPrice.tCellLastPrice.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tCellRisePrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellRisePrice.dwTextColor = RGB(255, 0, 0);
	colorStyle.tPrice.tCellFallPrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellFallPrice.dwTextColor = RGB(0, 160, 0);
}

void GetColorStyle_4(BOOKORDERDLGCOLOR& colorStyle)
{
	colorStyle.dwDlgBackColor = RGB(234, 253, 221);
	colorStyle.dwDlgButtonBackColor = RGB(178, 212, 102);
	colorStyle.dwDlgEditBackColor = RGB(255, 255, 255);

	colorStyle.tAccount.tRow.dwBackColor = RGB(102, 100, 101);
	colorStyle.tAccount.tRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPosition.tFixedRow.dwBackColor = RGB(182, 215, 108);
	colorStyle.tPosition.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPosition.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwBackColor = RGB(231, 241, 242);
	colorStyle.tPrice.tWaitBuyCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitBuyCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitBuyCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tBidCol.tFixedRow.dwBackColor = RGB(231, 241, 242);
	colorStyle.tPrice.tBidCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwBackColor = RGB(254, 246, 209);
	colorStyle.tPrice.tBidCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tEvenRow.dwBackColor = RGB(227, 220, 192);
	colorStyle.tPrice.tBidCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tPriceCol.tFixedRow.dwBackColor = RGB(231, 241, 242);
	colorStyle.tPrice.tPriceCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tAskCol.tFixedRow.dwBackColor = RGB(231, 241, 242);
	colorStyle.tPrice.tAskCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwBackColor = RGB(213, 251, 226);
	colorStyle.tPrice.tAskCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tEvenRow.dwBackColor = RGB(197, 224, 205);
	colorStyle.tPrice.tAskCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwBackColor = RGB(231, 241, 242);
	colorStyle.tPrice.tWaitSellCol.tFixedRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tWaitSellCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwBackColor = RGB(228, 226, 227);
	colorStyle.tPrice.tWaitSellCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tCellLastPrice.dwBackColor = RGB(255, 128, 128);
	colorStyle.tPrice.tCellLastPrice.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tCellRisePrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellRisePrice.dwTextColor = RGB(255, 0, 0);
	colorStyle.tPrice.tCellFallPrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellFallPrice.dwTextColor = RGB(0, 160, 0);
}
