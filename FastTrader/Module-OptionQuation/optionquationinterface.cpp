#pragma once
#include "stdafx.h"
#include "OptionQuationInterface.h"
#include "DlgQuation.h"
extern CDlgQuation *g_pDlgQuation;
extern HINSTANCE    m_ResHandle;
//创建新的T型报价窗口，返回窗口句柄
bool QUATION_API CreateQuationWnd(HWND hParent)
{
	AfxSetResourceHandle(m_ResHandle); 
	if(g_pDlgQuation == NULL)
	{
		CWnd *pWnd = CWnd::FromHandle(hParent);
		g_pDlgQuation = new CDlgQuation(pWnd);
		g_pDlgQuation->Create(IDD_DIALOG_QUATION, pWnd);			
	}	
	AfxSetResourceHandle(GetModuleHandle(NULL)); 
	g_pDlgQuation->ShowWindow(SW_SHOW);
	return true;
}
void QUATION_API SetTradingDay(std::string strTradingDay)
{
	if(g_pDlgQuation)
	{		
		g_pDlgQuation->SetTradingDay_Dlg(strTradingDay); 
	}
}
void QUATION_API ShowInstrument(std::string strInstrument)
{//显示合约
	if(g_pDlgQuation)
		g_pDlgQuation->ShowInstrument_Dlg(strInstrument);
}
void QUATION_API SetTradingScales(std::string strInstrument, const vector<std::pair<std::string,std::string>>& TradingTime)
{//交易刻度
	if(g_pDlgQuation)
		g_pDlgQuation->SetTradingScales_Dlg(strInstrument, TradingTime);
}
void QUATION_API InitFinish(std::string strInstrument)
{
	if(g_pDlgQuation)
		g_pDlgQuation->InitFinish_Dlg(strInstrument);	
}
