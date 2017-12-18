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
		colorStyle.nViewMode = nStyle;
		GetColorStyle_1(colorStyle);
		return TRUE;
		break;
	case 2:
		colorStyle.nViewMode = nStyle;
		GetColorStyle_2(colorStyle);
		return TRUE;
		break;
	//case 3:
	//	colorStyle.nViewMode = nStyle;
	//	GetColorStyle_3(colorStyle);
	//	return TRUE;
	//	break;
	//case 4:
	//	colorStyle.nViewMode = nStyle;
	//	GetColorStyle_4(colorStyle);
	//	return TRUE;
	//	break;
	default:
		return FALSE;
		break;
	}
}

void GetColorStyle_1(BOOKORDERDLGCOLOR& colorStyle)
{
	colorStyle.dwDlgBackColor = RGB(7, 62, 130);//RGB(246, 246, 246);
	colorStyle.dwDlgButtonBackColor = RGB(238, 238, 238);
	colorStyle.dwDlgEditBackColor = RGB(255, 255, 255);

	colorStyle.tAccount.tFixedRow.dwBackColor = RGB(61, 119, 166);
	colorStyle.tAccount.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tAccount.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tAccount.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPosition.tFixedRow.dwBackColor = RGB(61, 119, 166);
	colorStyle.tPosition.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tBidCol.tFixedRow.dwBackColor = RGB(61, 119, 166);
	colorStyle.tPrice.tBidCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tBidCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 191);
	colorStyle.tPrice.tBidCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwBackColor = RGB(254, 246, 209);
	colorStyle.tPrice.tBidCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tEvenRow.dwBackColor = RGB(227, 220, 192);
	colorStyle.tPrice.tBidCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tPriceCol.tFixedRow.dwBackColor = RGB(61, 119, 166);
	colorStyle.tPrice.tPriceCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 191);
	colorStyle.tPrice.tPriceCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwTextColor = RGB(0, 0, 0);
	
	colorStyle.tPrice.tAskCol.tFixedRow.dwBackColor = RGB(61, 119, 166);
	colorStyle.tPrice.tAskCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tAskCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 191);
	colorStyle.tPrice.tAskCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwBackColor = RGB(213, 251, 226);
	colorStyle.tPrice.tAskCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tEvenRow.dwBackColor = RGB(197, 224, 205);
	colorStyle.tPrice.tAskCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tOtherCol.tFixedRow.dwBackColor = RGB(61, 119, 166);
	colorStyle.tPrice.tOtherCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tOtherCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 191);
	colorStyle.tPrice.tOtherCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tOtherCol.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tOtherCol.tRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tOtherCol.tEvenRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tOtherCol.tEvenRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tCellLastPrice.dwBackColor = RGB(255, 128, 128);
	colorStyle.tPrice.tCellLastPrice.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tCellRisePrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellRisePrice.dwTextColor = RGB(255, 0, 0);
	colorStyle.tPrice.tCellFallPrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellFallPrice.dwTextColor = RGB(0, 160, 0);
}

void GetColorStyle_2(BOOKORDERDLGCOLOR& colorStyle)
{
	colorStyle.dwDlgBackColor = RGB(23, 72, 123);//RGB(230, 241, 247);
	colorStyle.dwDlgButtonBackColor = RGB(182, 224, 248);
	colorStyle.dwDlgEditBackColor = RGB(255, 255, 255);

	colorStyle.tAccount.tFixedRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tAccount.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tAccount.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tAccount.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPosition.tFixedRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPosition.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPosition.tRow.dwTextColor = RGB(0, 0, 0);

	colorStyle.tPrice.tBidCol.tFixedRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tBidCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tBidCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 221);
	colorStyle.tPrice.tBidCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwBackColor = RGB(0, 0, 0);
	colorStyle.tPrice.tBidCol.tRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tBidCol.tEvenRow.dwBackColor = RGB(9, 9, 9);
	colorStyle.tPrice.tBidCol.tEvenRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPrice.tPriceCol.tFixedRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tPriceCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 221);
	colorStyle.tPrice.tPriceCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tPriceCol.tRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tPriceCol.tRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tPriceCol.tEvenRow.dwTextColor = RGB(255, 255, 255);
	
	colorStyle.tPrice.tAskCol.tFixedRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tAskCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tAskCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 221);
	colorStyle.tPrice.tAskCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwBackColor = RGB(0, 0, 0);
	colorStyle.tPrice.tAskCol.tRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tAskCol.tEvenRow.dwBackColor = RGB(9, 9, 9);
	colorStyle.tPrice.tAskCol.tEvenRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPrice.tOtherCol.tFixedRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tOtherCol.tFixedRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tOtherCol.tFixedEvenRow.dwBackColor = RGB(191, 191, 221);
	colorStyle.tPrice.tOtherCol.tFixedEvenRow.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tOtherCol.tRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tOtherCol.tRow.dwTextColor = RGB(255, 255, 255);
	colorStyle.tPrice.tOtherCol.tEvenRow.dwBackColor = RGB(10, 58, 118);
	colorStyle.tPrice.tOtherCol.tEvenRow.dwTextColor = RGB(255, 255, 255);

	colorStyle.tPrice.tCellLastPrice.dwBackColor = RGB(255, 128, 128);
	colorStyle.tPrice.tCellLastPrice.dwTextColor = RGB(0, 0, 0);
	colorStyle.tPrice.tCellRisePrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellRisePrice.dwTextColor = RGB(255, 0, 0);
	colorStyle.tPrice.tCellFallPrice.dwBackColor = RGB(255, 255, 255);
	colorStyle.tPrice.tCellFallPrice.dwTextColor = RGB(0, 160, 0);
}
