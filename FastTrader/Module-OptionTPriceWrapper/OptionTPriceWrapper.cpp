/***************************************************************************
*   @file         OptionTPriceWrapper.cpp
*   @brief        合约列表板块,类的实现
*
*   @author     ying.qiu
*   @version    0.0.1
*   @date       created on 2010/09/26
********************************************************************************/

#include "OptionTPriceWrapper.h"
#include "cfgMgr/CfgMgr.h"
#include "cfgMgr/CfgGlobal.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "gui-common/FrameBase.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "../inc/Plusin/Plusin.h"

extern CPlusin g_Plusin;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace std;


COptionTPriceWrapper* COptionTPriceWrapper::m_pOptionTPriceWrapper=NULL;
bool COptionTPriceWrapper::m_bValid=false;
bool COptionTPriceWrapper::m_bValid_OptionQuation=false;

BEGIN_EVENT_TABLE(COptionTPriceWrapper,wxPanel)
	EVT_SIZE(OnSize)
    EVT_COMMAND(wxID_ANY, wxEVT_GRID_INSTRUMENTID_CHANGED,OnInstrumentIDChangeInQuotPanel)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnDepthMarketData,  COptionTPriceWrapper::OnRtnDepthMarketData)
	EVT_COMMAND(wxID_ANY, wxEVT_MENU_OPTIONUI_DarkBackground,  COptionTPriceWrapper::OnDarkBackground)
	EVT_COMMAND(wxID_ANY, wxEVT_MENU_OPTIONUI_LightBackground,  COptionTPriceWrapper::OnLightBackground)
    
END_EVENT_TABLE()

COptionTPriceWrapper::COptionTPriceWrapper(wxWindow *parent,
								 wxWindowID id,
								 const wxPoint& pos,
								 const wxSize& size,
								 long style,
								 const wxString& name)
:   wxPanel(parent, id, pos, size, style, name)
    ,m_hLib_TPriceList(NULL)
    ,m_Method_CreateWnd(NULL)
    ,m_Method_SetOptionInstruments(NULL)
    ,m_Method_NewMarketData(NULL)
    ,m_Method_Get_Del_ClickParam(NULL)
    ,m_Method_SetColorMode(NULL)
    ,m_hWnd_TPriceList(NULL)
    ,m_BackgroundMode(0)
{
	m_pOptionTPriceWrapper = this;

    //如果运行在期权模式，加载TPriceList.dll
    if(GlobalConfigManager::m_RunMode==2||GlobalConfigManager::m_RunMode==3)
    {
        m_hLib_TPriceList=LoadLibrary("TPriceList.dll");
        if(m_hLib_TPriceList)
        {
            m_Method_CreateWnd=(CreateWnd_TPriceListFunc)GetProcAddress(m_hLib_TPriceList,"CreateWnd");
            m_Method_SetOptionInstruments=(SetOptionInstruments_TPriceListFunc)GetProcAddress(m_hLib_TPriceList,"SetOptionInstruments");
            m_Method_NewMarketData=(NewMarketData_TPriceListFunc)GetProcAddress(m_hLib_TPriceList,"NewMarketData");
            m_Method_Get_Del_ClickParam=(Get_Del_ClickParam_TPriceListFunc)GetProcAddress(m_hLib_TPriceList,"Get_Del_ClickParam");
            m_Method_SetColorMode=(SetColorMode_TPriceListFunc)GetProcAddress(m_hLib_TPriceList,"SetColorMode");

            if(m_Method_CreateWnd&&
                m_Method_SetOptionInstruments&&
                m_Method_NewMarketData&&
                m_Method_Get_Del_ClickParam&&
                m_Method_SetColorMode)
                m_bValid=true;
        }


#if _DEBUG
        m_hLib_OptionQuation=LoadLibrary("OptionQuationD.dll");
#else
        m_hLib_OptionQuation=LoadLibrary("OptionQuation.dll");
#endif
        if(m_hLib_OptionQuation)
        {
            m_CreateQuationWnd_OptionQuationFunc	=(CreateQuationWnd_OptionQuationFunc)GetProcAddress(m_hLib_OptionQuation,"CreateQuationWnd");
            m_SetTradingDay_OptionQuationFunc		=(SetTradingDay_OptionQuationFunc)GetProcAddress(m_hLib_OptionQuation,"SetTradingDay");
            m_ShowInstrument_OptionQuationFunc		=(ShowInstrument_OptionQuationFunc)GetProcAddress(m_hLib_OptionQuation,"ShowInstrument");
            m_SetTradingScales_OptionQuationFunc	=(SetTradingScales_OptionQuationFunc)GetProcAddress(m_hLib_OptionQuation,"SetTradingScales");
            m_InitFinish_OptionQuationFunc			=(InitFinish_OptionQuationFunc)GetProcAddress(m_hLib_OptionQuation,"InitFinish");

            if(m_CreateQuationWnd_OptionQuationFunc&&
                m_SetTradingDay_OptionQuationFunc&&
                m_ShowInstrument_OptionQuationFunc&&
                m_SetTradingScales_OptionQuationFunc&&
                m_InitFinish_OptionQuationFunc)
                m_bValid_OptionQuation=true;
        }

    }

    if(m_bValid)
    {
        //创建窗口
        m_hWnd_TPriceList=m_Method_CreateWnd((HWND)(wxPanel::GetHandle()));
        if(m_BackgroundMode==0) m_Method_SetColorMode(0);
        else m_Method_SetColorMode(1);
        //::ShowWindow(m_hWnd_TPriceList,SW_SHOWNA);

        //订阅行情消息
        if(DEFAULT_SVR())
            DEFAULT_SVR()->SubscribeBusinessData(BID_RtnDepthMarketData, GID_OptionTPrice, CallBackFunc_BID_RtnDepthMarketData);

    }


}

COptionTPriceWrapper::~COptionTPriceWrapper()
{
    if(m_bValid)
    {
        if(DEFAULT_SVR())
        {
            //退订合约行情
            DEFAULT_SVR()->UnsubscribeMarketDataOfGID(GID_OptionTPrice);

            //退订行情消息
	        DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_OptionTPrice);
        }



        if(m_hWnd_TPriceList)
        {
            DestroyWindow(m_hWnd_TPriceList);
            m_hWnd_TPriceList=NULL;
        }

        m_bValid=false;
    }


    m_Method_CreateWnd=NULL;
    m_Method_SetOptionInstruments=NULL;
    m_Method_NewMarketData=NULL;
    m_Method_Get_Del_ClickParam=NULL;
    m_Method_SetColorMode=NULL;

    if(m_hLib_TPriceList)
    {
        FreeLibrary(m_hLib_TPriceList);
        m_hLib_TPriceList=NULL;
    }
    m_pOptionTPriceWrapper=NULL;
}

//根据窗体大小调整控件的大小和位置
void COptionTPriceWrapper::OnSize( wxSizeEvent& event )
{
    if(m_bValid)
    {
        int wid,hei;
        GetClientSize(&wid,&hei);
        ::MoveWindow(m_hWnd_TPriceList,0,0,wid,hei,TRUE);
    }
}

//报价表合约改变消息
void COptionTPriceWrapper::OnInstrumentIDChangeInQuotPanel(wxCommandEvent& event)
{
	string strInstrumentID;
    DWORD EvtParamID=(DWORD)event.GetInt();
    int SenderGID=(int)event.GetExtraLong();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,&strInstrumentID,NULL,NULL,0,NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

    if(!DEFAULT_SVR()) return;
    if(!m_bValid) return;

    if(m_UnderlyingInstrumentID==strInstrumentID) return;

    vector<string> OptionInstrumentIDs;
    DEFAULT_SVR()->GetOptionInstrumentIDsByUnderlyingInstrumentID(strInstrumentID,OptionInstrumentIDs);
    vector<PlatformStru_InstrumentInfo> OptionInstrumentInfos;
    PlatformStru_InstrumentInfo InstrumentInfo;
    for(int i=0;i<(int)OptionInstrumentIDs.size();i++)
    {
        if(DEFAULT_SVR()->GetInstrumentInfo(OptionInstrumentIDs[i],InstrumentInfo)==0)
            OptionInstrumentInfos.push_back(InstrumentInfo);
    }

    m_Method_SetOptionInstruments(OptionInstrumentInfos);

    //退订原来的行情
    DEFAULT_SVR()->UnsubscribeMarketDataOfGID(GID_OptionTPrice);

    //订阅新的行情, 并获取/推送最近的行情
    for(int i=0;i<(int)OptionInstrumentIDs.size();i++)
    {
        DEFAULT_SVR()->SetSubscribeStatus(OptionInstrumentIDs[i],GID_OptionTPrice);

        PlatformStru_DepthMarketData lastquot;
        if(DEFAULT_SVR()->GetQuotInfo(OptionInstrumentIDs[i],lastquot)==0)
            m_Method_NewMarketData(lastquot);
    }
}

//行情回调
int COptionTPriceWrapper::CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RtnDepthMarketData || data.Size!=sizeof(DataRtnDepthMarketData))
		return 0;

    DataRtnDepthMarketData& NewBusiness= (DataRtnDepthMarketData&)data;  

    DWORD EvtParamID;
    CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnDepthMarketData),0);

    wxCommandEvent evt(wxEVT_RtnDepthMarketData, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    m_pOptionTPriceWrapper->AddPendingEvent(evt);
    return 0;
}
//行情处理
void COptionTPriceWrapper::OnRtnDepthMarketData(wxCommandEvent& evt)
{
    DataRtnDepthMarketData MarketData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarketData,NULL,sizeof(MarketData),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

    if(m_bValid)
    {
        m_Method_NewMarketData(MarketData.DepthMarketDataField);
    }
}
//设置深色背景
void COptionTPriceWrapper::OnDarkBackground(wxCommandEvent& evt)
{
    if(m_bValid)
    {
        m_BackgroundMode=1;
        m_Method_SetColorMode(1);
    }
}
//设置浅色背景
void COptionTPriceWrapper::OnLightBackground(wxCommandEvent& evt)
{
    if(m_bValid)
    {
        m_BackgroundMode=0;
        m_Method_SetColorMode(0);
    }
}

//处理定制的窗口消息
WXLRESULT COptionTPriceWrapper::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam)
{
    //杨桂明定义的TPrice窗口鼠标点击通知消息
    #define Msg_PriceListT_Click WM_USER+999        

    if(message==Msg_PriceListT_Click&&m_bValid)
    {
		int nEventID = wParam;
		int nClickMode = 0;
		string strInstrumentID;
		BOOL bDirection = TRUE;
		bool bRet = m_Method_Get_Del_ClickParam(nEventID, nClickMode, strInstrumentID, bDirection);
        if(bRet&&!strInstrumentID.empty()) 
        {
			//鼠标单击/双击
			if ( DEFAULT_SVR() && DEFAULT_SVR()->SupportPlugins() )
				g_Plusin.SetOrderModuleCurInstrumentID( strInstrumentID);

			DWORD EvtParamID;

			wxCommandEvent sendchangeevt(wxEVT_GRID_INSTRUMENTID_CHANGED);
			CFTEventParam::CreateEventParam(EvtParamID,NULL,&strInstrumentID,NULL,0,0);
			sendchangeevt.SetInt((int)EvtParamID);
			sendchangeevt.SetExtraLong((long)GID_OptionTPrice);
			GETTOPWINDOW()->AddPendingEvent(sendchangeevt);

			if(nClickMode==2) {
				ShowKLine(strInstrumentID);
			}
			else {
				wxCommandEvent sendevt(wxEVT_ORDERINSERT_MOUSEORDER);
				sendevt.SetExtraLong(bDirection?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell);
				CFTEventParam::CreateEventParam(EvtParamID,NULL,&strInstrumentID,NULL,0,nClickMode==0?1:2);
				sendevt.SetInt((int)EvtParamID);
				GETTOPWINDOW()->AddPendingEvent(sendevt);
			}

			wxString strInfo=wxString::Format(LOADFORMATSTRING(USEROPERATE_CLICK_PMT,"%s%c"), 
				strInstrumentID.c_str(), 
				bDirection?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell);
			USERLOG_INFO("%s",strInfo.c_str());
        }
    }
    return wxPanel::MSWWindowProc(message,wParam,lParam);
}

void COptionTPriceWrapper::ShowKLine(std::string& strInstrument)
{
	if(!m_bValid_OptionQuation)
		return;
	m_CreateQuationWnd_OptionQuationFunc((HWND)(wxPanel::GetHandle()));
	m_SetTradingDay_OptionQuationFunc(DEFAULT_SVR()->GetTradingDay());
	m_ShowInstrument_OptionQuationFunc(strInstrument);
	m_SetTradingScales_OptionQuationFunc(strInstrument, DEFAULT_SVR()->GetTradingTimespan2(strInstrument));
	m_InitFinish_OptionQuationFunc(strInstrument);
}
