/***************************************************************************
*   @file         OptionTPriceWrapper.h
*   @brief        期权T型报价包装类
*
********************************************************************************/

#ifndef _OPTIONTPRICEWRAPPER_H_
#define _OPTIONTPRICEWRAPPER_H_

#include <windows.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif

#include <vector>
#include <string>
using std::vector;
using std::pair;
using std::string;

#include "../inc/ISvr/PlatformSvrMgr.h"


//**********************定义TPriceList中的接口方法*************************
//创建新的T型报价窗口，返回窗口句柄
typedef HWND (*CreateWnd_TPriceListFunc)(HWND hParent);
//设置(期权合约-交易所)
typedef void (*SetOptionInstruments_TPriceListFunc)(const vector<PlatformStru_InstrumentInfo>& vecOptions);
//新行情到来
typedef void (*NewMarketData_TPriceListFunc)(const PlatformStru_DepthMarketData& NewMD);
//通过EventParamID获取鼠标点击参数，获取后删除。 ClickMode: 0 单击/ 1 双击; szInstrument: 点击期权合约的名称; bDirection: 买/卖
typedef bool (*Get_Del_ClickParam_TPriceListFunc)(int EventParamID, int& nClickMode, string& strInstrumentID, BOOL& bDirection);
//设置颜色模式，0为浅底色配色；1为深底色配色
typedef void (*SetColorMode_TPriceListFunc)(int colmode);


//**********************定义OptionQuation中的接口方法*************************
//创建行情窗口
typedef bool (*CreateQuationWnd_OptionQuationFunc)(HWND hParent);
//设置交易日
typedef void (*SetTradingDay_OptionQuationFunc)(std::string strTradingDay);
//设置显示的合约
typedef void (*ShowInstrument_OptionQuationFunc)(std::string strInstrument);
//设置合约交易时段
typedef bool (*SetTradingScales_OptionQuationFunc)(std::string strInstrument, const std::vector<std::pair<std::string,std::string>>& TradingTime);
//完成初始化合约
typedef void (*InitFinish_OptionQuationFunc)(std::string strInstrument);


class COptionTPriceWrapper : public wxPanel
{
public:
	COptionTPriceWrapper(wxWindow *parent,
		wxWindowID id = GID_OptionTPrice,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxEmptyString);

    virtual ~COptionTPriceWrapper();

    DECLARE_EVENT_TABLE()

public:
	void OnSize(wxSizeEvent& event);
    //报价表合约改变消息
    void OnInstrumentIDChangeInQuotPanel(wxCommandEvent& event);
    //行情回调
    static int CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data);
    //行情处理
    void OnRtnDepthMarketData(wxCommandEvent& evt);
    //设置深色背景
    void OnDarkBackground(wxCommandEvent& evt);
    //设置深色背景
    void OnLightBackground(wxCommandEvent& evt);

    //处理定制的窗口消息
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);

protected:
	void ShowKLine(std::string& strInstrument);

public:
    static COptionTPriceWrapper* m_pOptionTPriceWrapper;
    static bool m_bValid;
    static bool m_bValid_OptionQuation;

private:
    HINSTANCE m_hLib_TPriceList;
    HINSTANCE m_hLib_OptionQuation;

    CreateWnd_TPriceListFunc m_Method_CreateWnd;
    SetOptionInstruments_TPriceListFunc m_Method_SetOptionInstruments;
    NewMarketData_TPriceListFunc m_Method_NewMarketData;
    Get_Del_ClickParam_TPriceListFunc m_Method_Get_Del_ClickParam;
    SetColorMode_TPriceListFunc m_Method_SetColorMode;

	CreateQuationWnd_OptionQuationFunc	m_CreateQuationWnd_OptionQuationFunc;
	SetTradingDay_OptionQuationFunc		m_SetTradingDay_OptionQuationFunc;
	ShowInstrument_OptionQuationFunc	m_ShowInstrument_OptionQuationFunc;
	SetTradingScales_OptionQuationFunc	m_SetTradingScales_OptionQuationFunc;
	InitFinish_OptionQuationFunc		m_InitFinish_OptionQuationFunc;

    HWND m_hWnd_TPriceList;

    string  m_UnderlyingInstrumentID;

    int m_BackgroundMode;       //背景色方案。0:浅色背景；1:深色背景


};


#endif