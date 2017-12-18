// TPriceList.h : main header file for the TPriceList DLL
//

#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include "KernelStruct.h"

#define Msg_PriceListT_Click WM_USER+999

#ifndef TPRICELISTDLL

//创建新的T型报价窗口，返回窗口句柄
HWND __declspec(dllimport) CreateWnd(HWND hParent);

//设置期权合约
void __declspec(dllimport) SetOptionInstruments(const std::vector<PlatformStru_InstrumentInfo>& vecOptions);

//新行情到来
void __declspec(dllimport) NewMarketData(const PlatformStru_DepthMarketData& NewMD);

//通过EventParamID获取鼠标点击参数，获取后删除。
// nClickMode 鼠标点击模式，0 单击/ 1 双击
// strInstrumentID 合约ID
// bDirection 方向，TRUE 买入，FALSE 卖出
bool __declspec(dllimport) Get_Del_ClickParam(int EventParamID, int& nClickMode, 
											  std::string& strInstrumentID, BOOL& bDirection);

//设置颜色模式，0为浅底色配色；1为深底色配色
void __declspec(dllimport) SetColorMode(int colmode);

#endif
