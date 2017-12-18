// stdafx.cpp : 只包括标准包含文件的源文件
// OptionPriceListT.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

int g_nRunMode = 0;

void _SetRunMode(int nMode)
{
	g_nRunMode = nMode;
}

int  _GetRunMode()
{
	return g_nRunMode;
}

char g_cNewOptionType = '2';

void _SetNewOptionType(char cType)
{
	g_cNewOptionType = cType;
}

char _GetNewOptionType()
{
	return g_cNewOptionType;
}
