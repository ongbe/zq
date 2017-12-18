#include "StdAfx.h"
#include "FrameBase.h"
#include "MyProcess.h"
#include "gui/zq.h"


#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/ImportExport.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "../inc/Plusin/Plusin.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "Module-MainNoteBook/zqAuiNotebook.h" 
#include "Module-QuotFrame/QuotFrame.h"
#include "Module-Orders/OrderInsertPanel.h"
#include "Module-FundAccount/FundAccountModule.h"
#include "Module-AllOrderPanel/AllOrdersPanel.h" //所有委托单
#include "Module-Position/PositionPanel.h"
#include "Module-InstrumentList/InstrumentPanel.h"//合约列表
#include "Module-TradeRecord/TradeRecordPanel.h"
#include "Order-QueryPanel/CQueryLocalOrderPanel.h"
#include "Order-QueryPanel/CQueryOrderPositionPanel.h"
#include "Order-QueryPanel/CQueryOrderParkedPanel.h"
#include "Order-QueryPanel/CQueryOrderConditionPanel.h"
#include "Order-QueryPanel/CQueryOrderBatchPanel.h"
#include "Module-OptionTPriceWrapper/OptionTPriceWrapper.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
extern CPlusin g_Plusin;
wxArrayString g_HttpUrls;
extern DWORD g_StartIdleTime;


#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("FrameBase", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

BEGIN_EVENT_TABLE(FrameBase,wxFrame)
    EVT_SIZE(FrameBase::OnSize)
    EVT_BUTTON(wxID_ANY, FrameBase::OnButtonClick)
END_EVENT_TABLE()

FrameBase::FrameBase(void):
    m_notebook(NULL),
	m_configWizardProcessId(0),
	m_ReLoginSelfProcessId(0),
	m_bForceClose(false),
	m_bTop(false),
	m_curConfigPanel(NULL),
	m_pImportExport(NULL),
	m_StaticBlank(NULL)
#ifdef _USE_MULTI_LANGUAGE
,m_languageId(LANGUAGE_CHINESE)
#endif
{
    m_pWriteLog=new zqWriteLog();
#ifdef _USE_TOP_SPLITTER
	m_bShowVideoInfo=false;
	g_HttpUrls.Add(wxEmptyString,UrlIndex_Max);
	g_HttpUrls[UrlIndex_TodayHotService]="http://www.cifco.com.cn/ApiServices/TodayHotService";
	g_HttpUrls[UrlIndex_NameSpace]="http://service.api.app.cifco.hengtong.com/";
	g_HttpUrls[UrlIndex_ALL]="http://video.cifco.net/video/video!cxml.shtml?typeId=0" ;
	g_HttpUrls[UrlIndex_COPPER]="http://video.cifco.net/video/video!cxml.shtml?typeId=1" ;
	g_HttpUrls[UrlIndex_RUBBER]="http://video.cifco.net/video/video!cxml.shtml?typeId=2" ;
	g_HttpUrls[UrlIndex_COTTON]="http://video.cifco.net/video/video!cxml.shtml?typeId=3";
	g_HttpUrls[UrlIndex_ARTICLE]="http://www.cifco.net/ApiServices/article?wsdl";
	g_HttpUrls[UrlIndex_VIDEOPLAYER]="http://video.cifco.com.cn/player/videoAppPlayer.swf?shared=0&startTime=0&endTime=0&id=%d&xmlPath=http://video.cifco.com.cn/video/video!xml";
	g_HttpUrls[UrlIndex_GuideAction]="http://www.cifco.net/ApiServices/article";
	g_HttpUrls[UrlIndex_GuideNameSpace]="http://api.web.site.cifco.net/";
	LoadSaveVideoInfoProfile(true);
#endif 
}
void FrameBase::InitializeNCButton()
{
    if(!DEFAULT_SVR()) return;

	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if ( NULL == pMgr )
	{
		return;
	}
	m_CheckBitmap.LoadFile("zqBMP_CHECKBOX");
	m_UncheckBitmap.LoadFile("zqBMP_UNCHECKBOX");

#ifdef _USE_MULTI_LANGUAGE
	m_languageId=DEFAULT_SVR()->GetServerParamCfg().LanguageId;
	zqNCButton* pBtnEnglish =new zqNCButton(ID_NCBTNID_ENGLISH,wxString(""),m_languageId==LANGUAGE_ENGLISH,60,false);
	pBtnEnglish->SetBitmap(new wxBitmap(wxString("zqBMP_ENGLISH1")),new wxBitmap(wxString("zqBMP_ENGLISH2")));
	m_zqNCButtonArray.push_back( pBtnEnglish);
	zqNCButton* pBtnChinese =new zqNCButton(ID_NCBTNID_CHINESE,wxString(""),m_languageId==LANGUAGE_CHINESE,60,false);
	pBtnChinese->SetBitmap(new wxBitmap(wxString("zqBMP_CHINESE1")),new wxBitmap(wxString("zqBMP_CHINESE2")));
	m_zqNCButtonArray.push_back(pBtnChinese);
	zqNCButton* pBtnTradition =new zqNCButton(ID_NCBTNID_TRADITION,wxString(""),m_languageId==LANGUAGE_TRADITION,60,false);
	pBtnTradition->SetBitmap(new wxBitmap(wxString("zqBMP_TRADITION1")),new wxBitmap(wxString("zqBMP_TRADITION2")));
	m_zqNCButtonArray.push_back(pBtnTradition);
#endif
#ifdef _USE_TOP_SPLITTER
    m_zqNCButtonArray.push_back(new zqNCButton(ID_NCBTNID_VIDEO,LOADSTRING(VIDEOINFO),m_bShowVideoInfo,80));
#endif 
////////////////////////////////////////////////////////////////////////////
	zqNCButton* pBtnAutoLock = new zqNCButton(ID_NCBTNID_LOCK,LOADSTRING(DELAYLOCKED),false,100);
	m_zqNCButtonArray.push_back( pBtnAutoLock );
	zqNCButton* pBtnTop = new zqNCButton(ID_NCBTNID_TOP,LOADSTRING(TOPMOST),false,80);
    m_zqNCButtonArray.push_back( pBtnTop );
	CalcNCButtonOffset();
	m_pImportExport=new CImportExport(ID_BASE_LAYOUT,ID_BASE_LAYOUT+50);

	LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
	pBtnAutoLock->SetCheck( pMainWindowCfg->bAutoLock );
	pBtnTop->SetCheck( pMainWindowCfg->bIsTop );
	if ( pBtnAutoLock->GetChecked())
	{
		g_StartIdleTime = GetTickCount();
	}
}
void FrameBase::CalcNCButtonOffset()
{
	int offset=0;
	for(int index=m_zqNCButtonArray.size()-1;index>=0;index--)
	{
         offset+=m_zqNCButtonArray[index]->GetWidth();
		 m_zqNCButtonArray[index]->SetRightOffset(offset);
	}
}

BOOL CALLBACK EnumWinFunc(HWND hwnd,LPARAM lParam)
{
    FrameBase* pThis=(FrameBase*)lParam;
    if(pThis==NULL) return FALSE;
    return pThis->EnumTopLevelWin(hwnd);
}
BOOL FrameBase::EnumTopLevelWin(HWND hwnd)
{
    char caption[1024]={0},className[1024]={0};
    DWORD ProcessID,CurProcessID;
    GetWindowThreadProcessId(hwnd,&ProcessID);
    CurProcessID=GetCurrentProcessId();
    if(ProcessID==CurProcessID)
    {
        ::GetWindowText(hwnd,caption,1024);
        ::GetClassName(hwnd,className,1024);
        LOG_INFO("EnumTopLevelWin() Item:%08X %s(%s)",hwnd,caption,className);        
    }
    return TRUE;
}
FrameBase::~FrameBase(void)
{
#ifdef _USE_TOP_SPLITTER
	LoadSaveVideoInfoProfile(false);
#endif
    for(int i=m_zqNCButtonArray.size()-1;i>=0;i--)
    {
         wxDELETE(m_zqNCButtonArray[i]);
    }
    m_zqNCButtonArray.clear();
    wxToolBar *tbar = GetToolBar();
    if(tbar) tbar->ClearTools();
    SAFE_DELETE(tbar);
    SetToolBar(NULL);
	wxDELETE(m_pImportExport);
    LOG_INFO("FrameBase::~FrameBase()");
    /////////////////////////////////////////////////////////////////////
    //使用Win32 API获取此时属于本进程的所有顶级窗口
    //正常情况
    /////////////////////////////////////////////////////////////////////
    LOG_INFO("EnumTopLevelWin BEGIN"); 
    ::EnumWindows(EnumWinFunc,(LPARAM)this); 
    LOG_INFO("EnumTopLevelWin END"); 
    /////////////////////////////////////////////////////////////////////
    //使用Win32 API获取此时顶级窗口的子窗口信息
    //正常情况应该没有子窗口了
    /////////////////////////////////////////////////////////////////////
    HWND nHwnd=GetWindow((HWND)GetHWND(),GW_CHILD);
    char caption[1024]={0},className[1024]={0};
    LOG_INFO("GetChildWindow() BEGIN"); 
    while(nHwnd!=0)
    {
        ::GetWindowText(nHwnd,caption,1024);
        ::GetClassName(nHwnd,className,1024);
        LOG_INFO("GetChildWindow() Item:%08X %s(%s)",nHwnd,caption,className);        
        nHwnd=GetWindow(nHwnd,GW_HWNDNEXT);
    }
    LOG_INFO("GetChildWindow() END"); 
    SAFE_DELETE(m_pWriteLog);
}
void FrameBase::PaintMenuButton(bool bRefresh)
{
    int border,beginy,endy,textpixel_x,textpixel_y;
    border=wxSystemSettings::GetMetric(wxSYS_EDGE_Y);
    beginy=wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)+border*2;
    endy=wxSystemSettings::GetMetric(wxSYS_MENU_Y);
    wxWindowDC  dc(this);
    dc.SetFont(*wxNORMAL_FONT);
	wxColor base_colour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
	//刷新背景
	//int maxx=0;
	//for(int i=0;i<(int)m_zqNCButtonArray.size();i++)
	//{
	//	zqNCButton* pButton=m_zqNCButtonArray[i];
	//	if(pButton->GetRightOffset()>maxx)maxx=pButton->GetRightOffset();
	//}
	//wxRect AllRect(GetSize().x-maxx,beginy,maxx-border*2,endy);
	//dc.GradientFillLinear(AllRect,base_colour,base_colour);
	if(bRefresh)
		SendMessage(GetHwnd(),WM_NCPAINT,0,0);
	//绘制文本和图标
	for(int i=0;i<(int)m_zqNCButtonArray.size();i++)
	{
		zqNCButton* pButton=m_zqNCButtonArray[i];
		if(pButton->IsCheckbox())
		{
			wxString BtnText=pButton->GetText();
			dc.GetTextExtent(BtnText,&textpixel_x,&textpixel_y);
			wxRect startRect(GetSize().x-pButton->GetRightOffset(),beginy,endy,endy);
			DrawCheckBox(dc,startRect,pButton->GetChecked());
			pButton->SetRect(wxRect(GetSize().x-pButton->GetRightOffset(),beginy,endy+border+textpixel_x,endy));
			dc.DrawText(BtnText,startRect.GetRight()+border,beginy+border);
		}
		else
		{
			int xx=GetSize().x-pButton->GetRightOffset();
			pButton->SetRect(wxRect(xx,beginy+2,49,15));
			dc.DrawBitmap(*pButton->GetBitmap(),xx,beginy+2);

		}
	}
}
void FrameBase::DrawCheckBox(wxDC& dc,const wxRect& rect,bool bcheck)
{
	int x=rect.x+(rect.width-13)/2;
	int y=rect.y+(rect.height-13)/2;
	if(bcheck)
	{
		if(m_CheckBitmap.IsOk()) 
			dc.DrawBitmap(m_CheckBitmap,x,y);
	}
	else
	{
		if(m_UncheckBitmap.IsOk()) 
			dc.DrawBitmap(m_UncheckBitmap,x,y);
	}
}
#ifdef _USE_MULTI_LANGUAGE
void FrameBase::ChangeButtonCaption()
{
	for(int i=0;i<(int)m_zqNCButtonArray.size();i++)
	{
		zqNCButton* pButton=m_zqNCButtonArray[i];
		switch(pButton->GetId())
		{
		case ID_NCBTNID_VIDEO:
			pButton->SetText(LOADSTRING(VIDEOINFO));
			break;
		case ID_NCBTNID_LOCK:
			pButton->SetText(LOADSTRING(DELAYLOCKED));
			break;
		case ID_NCBTNID_TOP:
			pButton->SetText(LOADSTRING(TOPMOST));
			break;
		}
	}
}
#endif
void FrameBase::OnNcLeftUp(wxPoint pt)
{
    for(int i=0;i<(int)m_zqNCButtonArray.size();i++)
    {
        zqNCButton* pButton=m_zqNCButtonArray[i];
        wxRect rect=pButton->GetRect();
        rect.x+=GetPosition().x;
        rect.y+=GetPosition().y;
        if(rect.Inside(pt))
        {
			if(pButton->IsCheckbox())
				pButton->SetCheck(!pButton->GetChecked());
			switch(pButton->GetId())
			{
#ifdef _USE_TOP_SPLITTER
			case ID_NCBTNID_VIDEO:
				OnToggleViewVideoInfo(pButton->GetChecked());
				break;
#endif
#ifdef _USE_MULTI_LANGUAGE
			case ID_NCBTNID_ENGLISH:
				if(m_languageId!=LANGUAGE_ENGLISH)
				{
					ChangeLanguage(LANGUAGE_ENGLISH);
					ChangeButtonCaption();
				}
				break;
			case ID_NCBTNID_CHINESE:
				if(m_languageId!=LANGUAGE_CHINESE)
				{
       				ChangeLanguage(LANGUAGE_CHINESE); 
					ChangeButtonCaption();
				}
				break;
			case ID_NCBTNID_TRADITION:
				if(m_languageId!=LANGUAGE_TRADITION)
				{
       				ChangeLanguage(LANGUAGE_TRADITION); 
					ChangeButtonCaption();
				}
				break;
#endif
			case ID_NCBTNID_LOCK://300秒锁屏
				{
					if ( pButton->GetChecked())
					{
						g_StartIdleTime = GetTickCount();
					}
					
					CfgMgr* pMgr = CFG_MGR_DEFAULT();
					if ( pMgr )
					{	
						LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
						pMainWindowCfg->bAutoLock = pButton->GetChecked();
					}
				}
				break;
			case ID_NCBTNID_TOP://置顶
				{
					SetWindowStyle(pButton->GetChecked()?(GetWindowStyle()|wxSTAY_ON_TOP):(GetWindowStyle()&(~wxSTAY_ON_TOP)));
					CfgMgr* pMgr = CFG_MGR_DEFAULT();
					if (pMgr )
					{
						LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
						pMainWindowCfg->bIsTop = pButton->GetChecked();
					}
				}
				break;
			}
			PaintMenuButton(true);
        }

    }

}
//bool FrameBase::SaveCommissionRateAndMarginRate()
//{
//    if(!DEFAULT_SVR()) return false;
//    WXLOG_INFO("FrameBase::SaveCommissionRateAndMarginRate");
//    std::set<std::string> tmpInstrumentID;
//    DEFAULT_SVR()->GetInstrumentList(tmpInstrumentID);
//    wxString strTemp;
//    TiXmlDocument myDocument;
//    TiXmlElement *RootElement = new TiXmlElement("Rate");
//	string strTradeDay=DEFAULT_SVR()->GetTradingDay();
//    if(!strTradeDay.empty())
//    {
//        RootElement->SetAttribute("date",strTradeDay.c_str());
//    }
//	string strInvestorID,strBrokerID;
//	DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
//	RootElement->SetAttribute("id",strInvestorID.c_str());
//    myDocument.LinkEndChild(RootElement);
//    TiXmlElement *CommissionRateElement = new TiXmlElement("CommissionRate");
//    RootElement->LinkEndChild(CommissionRateElement); 
//    PlatformStru_InstrumentCommissionRate field1;
//    for (std::set<std::string>::iterator it = tmpInstrumentID.begin(); it != tmpInstrumentID.end(); ++it) 
//    {
//        if(DEFAULT_SVR()->GetCommissionRate(*it,field1)==0)
//        {
//            TiXmlElement *FieldElement = new TiXmlElement("Field");
//            TiXmlText *NameContent = new TiXmlText((*it).c_str());
//            FieldElement->LinkEndChild(NameContent);
//            strTemp.Printf("%0.12f",field1.OpenRatioByMoney);
//            FieldElement->SetAttribute("ORBM",strTemp);
//            strTemp.Printf("%0.12f",field1.OpenRatioByVolume);
//            FieldElement->SetAttribute("ORBV",strTemp);
//            strTemp.Printf("%0.12f",field1.CloseRatioByMoney);
//            FieldElement->SetAttribute("CRBM",strTemp);
//            strTemp.Printf("%0.12f",field1.CloseRatioByVolume);
//            FieldElement->SetAttribute("CRBV",strTemp);
//            strTemp.Printf("%0.12f",field1.CloseTodayRatioByMoney);
//            FieldElement->SetAttribute("CTRBM",strTemp);
//            strTemp.Printf("%0.12f",field1.CloseTodayRatioByVolume);
//            FieldElement->SetAttribute("CTRBV",strTemp);
//            CommissionRateElement->LinkEndChild(FieldElement);
//        }  
//    }
//    TiXmlElement *MarginRateElement = new TiXmlElement("MarginRate");
//    RootElement->LinkEndChild(MarginRateElement);
//    PlatformStru_InstrumentMarginRate field2;
//    for (std::set<std::string>::iterator it = tmpInstrumentID.begin(); it != tmpInstrumentID.end(); ++it) 
//    {
//        if(DEFAULT_SVR()->GetMarginRate(*it,field2)==0)
//        {
//            TiXmlElement *FieldElement = new TiXmlElement("Field");
//            TiXmlText *NameContent = new TiXmlText((*it).c_str());
//            FieldElement->LinkEndChild(NameContent);
//            strTemp.Printf("%0.12f",field2.LongMarginRatioByMoney);
//            FieldElement->SetAttribute("LMBM",strTemp);
//            strTemp.Printf("%0.12f",field2.LongMarginRatioByVolume);
//            FieldElement->SetAttribute("LMBV",strTemp);
//            strTemp.Printf("%0.12f",field2.ShortMarginRatioByMoney);
//            FieldElement->SetAttribute("SMBM",strTemp);
//            strTemp.Printf("%0.12f",field2.ShortMarginRatioByVolume);
//            FieldElement->SetAttribute("SMBV",strTemp);
//            MarginRateElement->LinkEndChild(FieldElement);
//        }     
//    }
//    std::string apppath;
//    GlobalFunc::GetPath(apppath);
//    wxString strFIleName=apppath+"cam.xml";
//    myDocument.SaveFile(strFIleName);
//	return true;
//}
WXLRESULT FrameBase::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{
    if(message==WM_CLOSE)
    {
        //重新登陆时，将m_ReLoginSelfProcessId设置为新进程ID，这样新进程关闭时，wParam等于进程ID，旧进程中就知道不用确认了。
        //升级时，升级程序将WM_CLOSE的wParam设置为0x9821，也表示不用进程关闭确认
        if(wParam>0&&wParam==m_ReLoginSelfProcessId||wParam==0x9821)
        {
            m_bForceClose=true;
        }
    }
    if(message==WM_NCLBUTTONUP) 
    {
        OnNcLeftUp(wxPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
    }
    WXLRESULT ret=wxFrame::MSWWindowProc(   message,   wParam,   lParam   ); 
	if(message==WM_NCPAINT||message==WM_NCACTIVATE)
	{
		PaintMenuButton(false);
	}
	return   ret;
}
void FrameBase::DoLoginAgain()
{
    wxString cmd;
    string filename;
    wxGetApp().GetFileName(filename);
    cmd.Printf("%s KillOldAfterStartup %d",filename.c_str(),(long)this->GetHandle());
    cProcess *process = new cProcess( this,m_ReLoginSelfProcessId);
    if(m_ReLoginSelfProcessId)//被调用程序是单实例程序
    {
        process->Execute(cmd, wxEXEC_ASYNC);
    }
    else
    {
        m_ReLoginSelfProcessId= process->Execute(cmd, wxEXEC_ASYNC,process); 
        if (!m_ReLoginSelfProcessId)  
        {  
            delete process;  
        } 
    }
}
bool FrameBase::CreateMenuBar_SunGard()
{
    if(!DEFAULT_SVR()) return false;
    WXLOG_INFO("FrameBase::CreateMenuBar_SunGard");
    wxMenuBar* mb = new wxMenuBar;
    //【系统】
    {
        wxMenu* menu = new wxMenu;
        { 
            menu->Append(ID_LOCK_FRAME, LOADSTRING(ID_SYSTEM_SUNGARD_LOCK));
            menu->AppendSeparator();
            if(IS_MULTIACCOUNT_VERSION)
            {
			    menu->Append(ID_MODIFY_MA_PASSWORD, "修改登录密码");
			    menu->AppendSeparator();
			    menu->Append(ID_READ_MSG, LOADSTRING(ID_SYSTEM_READ_MSG));
			    menu->AppendSeparator();
            }
            menu->Append(ID_LOGIN_AGAIN, LOADSTRING(ID_SYSTEM_SUNGARD_RELOGIN));
            menu->AppendSeparator();
            menu->Append(wxID_EXIT, LOADSTRING(ID_SYSTEM_SUNGARD_EXIT));
        }
        mb->Append(menu,LOADSTRING(ID_SYSTEM_SUNGARD));
    }
    //【设置】
    {
        wxMenu* menu = new wxMenu;
        { 
			menu->AppendCheckItem(ID_CONFIG_SELECT, LOADSTRING(ID_CONFIG_SUNGARD_SELECT));
            menu->AppendCheckItem(ID_CONFIG_ORDER, LOADSTRING(ID_CONFIG_SUNGARD_ORDER));
            menu->AppendCheckItem(ID_CONFIG_SYSTEM, LOADSTRING(ID_CONFIG_SUNGARD_SYSTEM));
			if(!IS_MULTIACCOUNT_VERSION&&DEFAULT_SVR()&&!DEFAULT_SVR()->HaveModifyPwdOnLongon())
			{
				menu->AppendCheckItem(ID_CONFIG_PASSWORD, LOADSTRING(ID_CONFIG_SUNGARD_PASSWORD));
			}
            menu->AppendCheckItem(ID_CONFIG_ACCE, LOADSTRING(ID_CONFIG_SUNGARD_ACCE));
        }
        mb->Append(menu,LOADSTRING(ID_CONFIG_SUNGARD));
    }
    //【下单】
    //对于专用期权终端，不显示此菜单
    if(GlobalConfigManager::m_RunMode!=3)
    {
        wxMenu* menu = new wxMenu;
        { 
            menu->AppendCheckItem(ID_ORDER_SMALLWIN, LOADSTRING(ID_ORDER_SUNGARD_SMALLWIN));
			if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_STANDARD)){menu->AppendCheckItem(ID_ORDER_STANDARD,LOADSTRING(ID_ORDER_SUNGARD_STANDARD));}
			if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_BATCH)){menu->AppendCheckItem(ID_ORDER_BATCH, LOADSTRING(ID_ORDER_SUNGARD_BATCH));}
			if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_CONDITION)){menu->AppendCheckItem(ID_ORDER_CONDITION, LOADSTRING(ID_ORDER_SUNGARD_CONDITION));}
			if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_PROFIT)){menu->AppendCheckItem(ID_ORDER_PROFIT, LOADSTRING(ID_ORDER_SUNGARD_STOP));}
 			if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_STOPLIMIT)){menu->AppendCheckItem(ID_ORDER_STOPLIMIT, LOADSTRING(ID_ORDER_STOPLIMIT_T));}	       
 			if(!IS_MULTIACCOUNT_VERSION&&DEFAULT_SVR()->HaveOrderType(UIOT_PANE_FAKFOK)){menu->AppendCheckItem(ID_ORDER_FAKFOK, LOADSTRING(ID_ORDER_FAKFOK_T));}	       
		}
        mb->Append(menu,LOADSTRING(ID_ORDER_SUNGARD));
    }
    //【查询】
    if(!IS_MULTIACCOUNT_VERSION)
    {
		wxMenu* menu = new wxMenu;
		{ 
			if(DEFAULT_SVR()->HaveBankFutureTransfer()){menu->Append(ID_QUERY_WITHDRAW, LOADSTRING(ID_QUERY_SUNGARD_WITHDRAW));}
			if(DEFAULT_SVR()->IsHistorySettlementSupported()){menu->Append(ID_QUERY_SETTLEMENT, LOADSTRING(ID_QUERY_SUNGARD_SETTLEMENT));}
			if(DEFAULT_SVR()->HaveGetTrader()){menu->Append(ID_QUERY_TRADER, LOADSTRING(ID_QUERY_TRADER));}
			if(DEFAULT_SVR()->HaveGetExchange()){menu->Append(ID_QUERY_EXCHANGE, LOADSTRING(ID_QUERY_EXCHANGE));}
			if(DEFAULT_SVR()->HaveGetContract()){menu->Append(ID_QUERY_CONTRACT, LOADSTRING(ID_QUERY_CONTRACT));}
			if(DEFAULT_SVR()->HaveGetCommodity()){menu->Append(ID_QUERY_COMMODITY, LOADSTRING(ID_QUERY_COMMODITY));}
			if(DEFAULT_SVR()->HaveGetOrderType()){menu->Append(ID_QUERY_ORDERTYPE, LOADSTRING(ID_QUERY_ORDERTYPE));}
			if(DEFAULT_SVR()->HaveGetPosition()){menu->Append(ID_QUERY_POSITION, LOADSTRING(ID_QUERY_POSITION));}
			if(DEFAULT_SVR()->HaveGetReport()){menu->Append(ID_QUERY_REPORT, LOADSTRING(ID_QUERY_REPORT));}
			if(DEFAULT_SVR()->HaveGetExchangeRate()){menu->Append(ID_QUERY_EXCHANGE_RATE, LOADSTRING(ID_QUERY_EXCHANGE_RATE));}
		}
        mb->Append(menu,LOADSTRING(ID_QUERY_SUNGARD));
    }
    //【转账】
    if(!IS_MULTIACCOUNT_VERSION&&DEFAULT_SVR()->HaveBankFutureTransfer())
    {
        wxMenu* menu = new wxMenu;
        { 
            menu->Append(ID_TRANSFER_BANK, LOADSTRING(ID_TRANSFER_SUNGARD_BANK));
        }
        mb->Append(menu,LOADSTRING(ID_TRANSFER_SUNGARD));
    }
    //【移换仓】，期权专业终端不支持
	if(DEFAULT_SVR()->SupportPlugins()&&GlobalConfigManager::m_RunMode!=3)
    {
        wxMenu* menu = new wxMenu;
        {
            menu->Append(ID_POS_MOVE, LOADSTRING(ID_POSITION_MOVE_CAPTION));
            menu->Append(ID_POS_SWAP, LOADSTRING(ID_POSITION_SWAP_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_POSITION_CAPTION));
    }
    //【期权下单】，只支持期权专业终端
	if(GlobalConfigManager::m_RunMode==2||GlobalConfigManager::m_RunMode==3)
    {
        wxMenu* menu = new wxMenu;
        {
            menu->Append(ID_OPTIONORDER_FORQUOTEINSERT, LOADSTRING(MENU_ID_OPTIONORDER_FORQUOTEINSERT));
            menu->Append(ID_OPTIONORDER_EXECORDER, LOADSTRING(MENU_ID_OPTIONORDER_EXECORDER));
			if(GlobalConfigManager::m_CanReceiveForQuote)
				menu->Append(ID_OPTIONORDER_SUBSUNSUBSFORQUOTE, LOADSTRING(MENU_ID_OPTIONORDER_SUBSUNSUBSFORQUOTE));
        }
        mb->Append(menu,LOADSTRING(MENU_ID_OPTIONORDER));
    }
    //【交易风格切换】
    {
        wxMenu* menu = new wxMenu;
        {
            //对于期权终端，不能改变界面风格
            if(GlobalConfigManager::m_RunMode!=2&&GlobalConfigManager::m_RunMode!=3)
            {
                menu->Append(ID_STYLE_SAVE, LOADSTRING(ID_CONFIG_SAVE_CAPTION));
                menu->AppendSubMenu(m_pImportExport->CreateLayoutMenu(),LOADSTRING(ID_CONFIG_LOAD_CAPTION));          
			    menu->AppendSeparator();
            }
            //对于期权终端，可以切换深色背景/浅色背景
            if(GlobalConfigManager::m_RunMode==2||GlobalConfigManager::m_RunMode==3)
            {
                menu->Append(ID_CONFIG_OPTIONUI_DarkBackground, LOADSTRING(ID_CONFIG_OPTIONUI_DarkBackground));
                menu->Append(ID_CONFIG_OPTIONUI_LightBackground, LOADSTRING(ID_CONFIG_OPTIONUI_LightBackground));
			    menu->AppendSeparator();
            }
			menu->AppendCheckItem(ID_CONFIG_OPTION,LOADSTRING(ID_CONFIG_OPTION_CAPTION));
            menu->Append(ID_DEFAULT_CONFIG,LOADSTRING(ID_DEFAULT_CONFIG_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_OPTION_CAPTION));
    }
    //【高级】
    CreateMenuBar_Plugins(mb);
    {
        wxMenu* menu = new wxMenu;
        { 
			//FASTTRADER-967 
              menu->Append(ID_HELP_DOC, LOADSTRING(ID_HELP_SUNGARD_DOC));
              menu->Append(wxID_ABOUT, LOADSTRING(ID_ABOUT_CAPTION));
           // menu->Append(ID_HELP_LOG, LOADSTRING(ID_HELP_SUNGARD_LOG));
        }
        mb->Append(menu,LOADSTRING(ID_HELP_CAPTION));
    }
    wxMenuBar* pMenuBar=GetMenuBar();
    SetMenuBar(mb);
    wxDELETE(pMenuBar);
    ToggleToolbar(true);
    return true;
}
void FrameBase::CreateMenuBar_Plugins(wxMenuBar* mb,int pos)
{
    if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		wxMenu* menu = new wxMenu;
		{
			menu->Append(ID_PLUSIN, LOADSTRING(ID_PLUSIN_MANAGER_CAPTION));
			vector<wxString> vAppID;
			g_Plusin.GetPlusinMenuInfo(MENU_POS_MAIN_MENU,vAppID);
			if(vAppID.size()>0)
				menu->AppendSeparator();
			wxString strMName;
			PLUGWIN2 plugwin;
			for (int i=0;i<(int)vAppID.size()&&i<MAX_PLUGIN_COUNT;++i)
			{
				if(g_Plusin.GetPlusinByAppID(vAppID[i],plugwin))
				{
					strMName=plugwin.strMenuName;
					if(strMName.IsEmpty()) 
						strMName=wxString::Format("Item %d",i+1);
					menu->Append(ID_PLUGIN_ITEM_BEGIN+i,strMName,vAppID[i]);
				}
			}
		}
		if(pos<0)
			mb->Append(menu,LOADSTRING(ID_PLUSIN_CAPTION));
		else
		{
			mb->Remove(pos);
			mb->Insert(pos,menu,LOADSTRING(ID_PLUSIN_CAPTION));
		}
	}
}
bool FrameBase::CreateMenuBar_Q7()
{
    if(!DEFAULT_SVR()) return false;
    WXLOG_INFO("FrameBase::CreateMenuBar_Q7");
    wxMenuBar* mb = new wxMenuBar;
    {
        wxMenu* menu = new wxMenu;
        {
            if(!IS_MULTIACCOUNT_VERSION)
            {
                if(DEFAULT_SVR()->HaveAccountText())
                    menu->Append(ID_QRY_ACCOUNT, LOADSTRING(ID_QRY_ACCOUNT_CAPTION));
                if(DEFAULT_SVR()->IsHistorySettlementSupported())
                    menu->Append(ID_QRY_SETTLE, LOADSTRING(ID_QRY_SETTLE_CAPTION));
			    if (!DEFAULT_SVR()->HaveModifyPwdOnLongon())
				    menu->Append(ID_MODIFY_PSW, LOADSTRING(ID_MODIFY_PSW_CAPTION));
            }
            else 
            {
			    menu->Append(ID_MODIFY_MA_PASSWORD, "修改登录密码");
            }
            menu->Append(ID_LOCK_FRAME, LOADSTRING(ID_LOCK_FRAME_CAPTION));
            menu->AppendSeparator();
            if(!IS_MULTIACCOUNT_VERSION)
            {
			    menu->Append(ID_READ_MSG, LOADSTRING(ID_SYSTEM_READ_MSG));
			    menu->AppendSeparator();
            }
            menu->Append(wxID_EXIT, LOADSTRING(ID_EXIT_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_SYSTEM_CAPTION));
    }
	if(DEFAULT_SVR()->HaveGetTrader()
		||DEFAULT_SVR()->HaveGetExchange()
		||DEFAULT_SVR()->HaveGetContract()
		||DEFAULT_SVR()->HaveGetCommodity()
		||DEFAULT_SVR()->HaveGetOrderType())
    {
		wxMenu* menu = new wxMenu;
		{ 
			if(DEFAULT_SVR()->HaveGetTrader()){menu->Append(ID_QUERY_TRADER, LOADSTRING(QUERY_TRADER));}
			if(DEFAULT_SVR()->HaveGetExchange()){menu->Append(ID_QUERY_EXCHANGE, LOADSTRING(ID_QUERY_EXCHANGE));}
			if(DEFAULT_SVR()->HaveGetContract()){menu->Append(ID_QUERY_CONTRACT, LOADSTRING(ID_QUERY_CONTRACT));}
			if(DEFAULT_SVR()->HaveGetCommodity()){menu->Append(ID_QUERY_COMMODITY, LOADSTRING(ID_QUERY_COMMODITY));}
			if(DEFAULT_SVR()->HaveGetOrderType()){menu->Append(ID_QUERY_ORDERTYPE, LOADSTRING(ID_QUERY_ORDERTYPE));}
			if(DEFAULT_SVR()->HaveGetPosition()){menu->Append(ID_QUERY_POSITION, LOADSTRING(ID_QUERY_POSITION));}		
			if(DEFAULT_SVR()->HaveGetReport()){menu->Append(ID_QUERY_REPORT, LOADSTRING(ID_QUERY_REPORT));}
			if(DEFAULT_SVR()->HaveGetExchangeRate()){menu->Append(ID_QUERY_EXCHANGE_RATE, LOADSTRING(ID_QUERY_EXCHANGE_RATE));}
		}
        mb->Append(menu,LOADSTRING(ID_QUERY_SUNGARD));
    }
    if(!IS_MULTIACCOUNT_VERSION&&DEFAULT_SVR()->HaveBankFutureTransfer())
    {
        wxMenu* menu = new wxMenu;
        {
            menu->Append(ID_TRANSFER, LOADSTRING(ID_TRANSFER_ITEM_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_TRANSFER_CAPTION));
    }
	if(DEFAULT_SVR()->SupportPlugins())
    {
        wxMenu* menu = new wxMenu;
        {
            menu->Append(ID_POS_MOVE, LOADSTRING(ID_POSITION_MOVE_CAPTION));
            menu->Append(ID_POS_SWAP, LOADSTRING(ID_POSITION_SWAP_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_POSITION_CAPTION));
    }
    if(0)
    {
        wxMenu* menu = new wxMenu;
        {
            menu->Append(ID_TRADE_REPORT, LOADSTRING(ID_TRADE_REPORT_CAPTION));
            menu->AppendSeparator();
            menu->Append(ID_TODAY_REPORT, LOADSTRING(ID_TODAY_REPORT_CAPTION));
            menu->Append(ID_WEEK_REPORT, LOADSTRING(ID_WEEK_REPORT_CAPTION));
            menu->Append(ID_MONTH_REPORT, LOADSTRING(ID_MONTH_REPORT_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_REPORT_CAPTION));
    }
    {
        wxMenu* menu = new wxMenu;
        {
            menu->Append(ID_STYLE_SAVE, LOADSTRING(ID_CONFIG_SAVE_CAPTION));
            menu->AppendSubMenu(m_pImportExport->CreateLayoutMenu(),LOADSTRING(ID_CONFIG_LOAD_CAPTION));  
			menu->AppendSeparator();
            menu->AppendCheckItem(ID_CONFIG_OPTION,LOADSTRING(ID_CONFIG_OPTION_CAPTION));
            menu->Append(ID_DEFAULT_CONFIG,LOADSTRING(ID_DEFAULT_CONFIG_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_OPTION_CAPTION));
    }
    CreateMenuBar_Plugins(mb);
    {
        wxMenu* menu = new wxMenu;
        {
			//FASTTRADER-967 
            menu->Append(ID_HELP_CONTENT,LOADSTRING(ID_HELP_CONTENT_CAPTION));
            menu->AppendSeparator();
            menu->Append(wxID_ABOUT, LOADSTRING(ID_ABOUT_CAPTION));
        }
        mb->Append(menu,LOADSTRING(ID_HELP_CAPTION));
    }
    wxMenuBar* pMenuBar=GetMenuBar();
    SetMenuBar(mb);
    wxDELETE(pMenuBar);
    ToggleToolbar(true);
    return true;
}
wxToolBar* FrameBase::CreateToolbar_Begin()
{
	static int s_toolcount=0;
	wxToolBar *toolBar = GetToolBar();
	if(toolBar)
	{
		for (int i=toolBar->GetToolsCount()-1;i>=0;--i)
		{
			if(i>=s_toolcount)
				toolBar->DeleteToolByPos(i);
		}
	}
	else
	{
		const long TOOLBAR_STYLE = wxTB_FLAT |wxTB_NOICONS | wxTB_TEXT;
		toolBar=CreateToolBar(TOOLBAR_STYLE,wxID_ANY); 

        //对于专用期权终端，不显示这些Tools
        if(GlobalConfigManager::m_RunMode!=3)
        {
		    toolBar->AddTool(ID_ORDER_SMALLWIN,LOADSTRING(ID_ORDER_SUNGARD_SMALLWIN_T), wxNullBitmap);
		    toolBar->AddSeparator();
		    if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_STANDARD)){toolBar->AddTool(ID_ORDER_STANDARD,LOADSTRING(ID_ORDER_SUNGARD_STANDARD_T), wxNullBitmap,wxNullBitmap,wxITEM_CHECK);}
		    if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_BATCH)){toolBar->AddTool(ID_ORDER_BATCH, LOADSTRING(ID_ORDER_SUNGARD_BATCH_T), wxNullBitmap,wxNullBitmap,wxITEM_CHECK);}
		    if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_CONDITION)){toolBar->AddTool(ID_ORDER_CONDITION, LOADSTRING(ID_ORDER_SUNGARD_CONDITION_T), wxNullBitmap,wxNullBitmap,wxITEM_CHECK);}
		    if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_PROFIT)){toolBar->AddTool(ID_ORDER_PROFIT, LOADSTRING(ID_ORDER_SUNGARD_STOP_T),wxNullBitmap,wxNullBitmap,wxITEM_CHECK);}
		    if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_STOPLIMIT)){toolBar->AddTool(ID_ORDER_STOPLIMIT, LOADSTRING(ID_ORDER_STOPLIMIT_T),wxNullBitmap,wxNullBitmap,wxITEM_CHECK);}			
		    if(!IS_MULTIACCOUNT_VERSION&&DEFAULT_SVR()->HaveOrderType(UIOT_PANE_FAKFOK)){toolBar->AddTool(ID_ORDER_FAKFOK, LOADSTRING(ID_ORDER_FAKFOK_T),wxNullBitmap,wxNullBitmap,wxITEM_CHECK);}			
        }
        else
        {
            //加一个空按钮，以免tools高度被压缩
		    toolBar->AddTool(wxID_STATIC,"　", wxNullBitmap);
        }
		toolBar->AddControl(m_StaticBlank=new zqStaticText(toolBar, wxID_STATIC,wxEmptyString, wxDefaultPosition, wxSize(100,-1) ));			
		s_toolcount=toolBar->GetToolsCount();
	}
	m_GroupButtons.clear();
	return toolBar;
}

wxButton* FrameBase::CreateToolbar_Item(wxToolBar *toolBar,wxWindowID id,
                                   const wxString& label,
                                   const wxPoint& pos,
                                   const wxSize& size,
                                   long style,
                                   const wxValidator& validator,
                                   const wxString& name)
{
    // wxID_ANY,p->GroupName, wxDefaultPosition, wxSize(72,-1), 0
    wxButton* pBtn=new wxButton(toolBar,id,label,pos,size,style,validator,name);
    m_GroupButtons.push_back(pBtn);
    toolBar->AddControl(pBtn);
    return pBtn;
}
void FrameBase::CreateToolbar_End(wxToolBar *toolBar)
{
    toolBar->Realize();
    if(GetToolBar()==NULL) 
        SetToolBar(toolBar);
}
void FrameBase::ToggleToolbar(bool bshow)
{
    if(!DEFAULT_SVR()) return;
    wxToolBar *tbar = GetToolBar();
    if(bshow)
    {
        if ( !tbar )
        {
            tbar=CreateToolbar_Begin();
            CreateToolbar_End(tbar);
        }
		else//
		{
            wxToolBarToolBase* ptool;
            if((ptool=tbar->FindById(ID_ORDER_SMALLWIN))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_SMALLWIN_T));
            if((ptool=tbar->FindById(ID_ORDER_STANDARD))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_STANDARD_T));
            if((ptool=tbar->FindById(ID_ORDER_BATCH))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_BATCH_T));
            if((ptool=tbar->FindById(ID_ORDER_CONDITION))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_CONDITION_T));
            if((ptool=tbar->FindById(ID_ORDER_PROFIT))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_STOP_T));
            if((ptool=tbar->FindById(ID_ORDER_STOPLIMIT))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_STOPLIMIT_T));
            if((ptool=tbar->FindById(ID_ORDER_FAKFOK))!=NULL) ptool->SetLabel(LOADSTRING(ID_ORDER_FAKFOK_T));

		    //tbar->FindById(ID_ORDER_SMALLWIN)->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_SMALLWIN_T));
		    //if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_STANDARD)){tbar->FindById(ID_ORDER_STANDARD)->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_STANDARD_T));}
		    //if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_BATCH)){tbar->FindById(ID_ORDER_BATCH)->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_BATCH_T));}
		    //if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_CONDITION)){tbar->FindById(ID_ORDER_CONDITION)->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_CONDITION_T));}
		    //if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_PROFIT)){tbar->FindById(ID_ORDER_PROFIT)->SetLabel(LOADSTRING(ID_ORDER_SUNGARD_STOP_T));}
		    //if(DEFAULT_SVR()->HaveOrderType(UIOT_PANE_STOPLIMIT)){tbar->FindById(ID_ORDER_STOPLIMIT)->SetLabel(LOADSTRING(ID_ORDER_STOPLIMIT_T));}						
		    //if(!IS_MULTIACCOUNT_VERSION&&DEFAULT_SVR()->HaveOrderType(UIOT_PANE_FAKFOK)){tbar->FindById(ID_ORDER_FAKFOK)->SetLabel(LOADSTRING(ID_ORDER_FAKFOK_T));}						
			tbar->Realize();
		}
    }
    else//隐藏
    {
        if(tbar)
        {
            delete tbar;
            SetToolBar(NULL);
        }
    }
}
void FrameBase::OnSize(wxSizeEvent& event)
{
    wxToolBar *tbar = GetToolBar();
    int staticBlankMinLength=80;
    if(tbar&&m_StaticBlank)
    {
        size_t btncount=m_GroupButtons.size();
        int length=event.GetSize().GetX()-2*GetSystemMetrics(SM_CXFRAME);
        int leftx=m_StaticBlank->GetPosition().x;
        wxSize sz=m_StaticBlank->GetSize();
        int realsize=length-btncount*80-leftx;
        if(realsize>=staticBlankMinLength)
        {
            m_StaticBlank->SetSize(length-btncount*80-leftx,-1);
            for(size_t i=0;i<btncount;++i)
            {
                m_GroupButtons[i]->SetPosition(wxPoint(length-(btncount-i)*80,-1));
            }
        }
        else
        {
            m_StaticBlank->SetSize(staticBlankMinLength,-1);
            for(size_t i=0;i<btncount;++i)
            {
                m_GroupButtons[i]->SetPosition(wxPoint(leftx+staticBlankMinLength+i*80,-1));
            }
        }
        tbar->Refresh();
    }
    event.Skip();
}
void FrameBase::OnButtonClick(wxCommandEvent& event)
{
    size_t btncount=m_GroupButtons.size();
    bool bGroupbutton=false;
    for(size_t i=0;i<btncount;++i)
    {
        if(event.GetId()==m_GroupButtons[i]->GetId())
        {
            bGroupbutton=true;
            break;
        }
    }
    if(bGroupbutton)
    {
        wxWindow* pWinQuot = FindWindowById(GID_QUOT_TABLE, this);
        wxCommandEvent evt(wxEVT_MAINFRAME_QUOT_SELGROUP);
        evt.SetInt(event.GetId());
        if(pWinQuot)
            pWinQuot->ProcessEvent(evt);
    }
    else
        event.Skip();


}
#ifdef _USE_MULTI_LANGUAGE
void FrameBase::ChangeLanguage(int langid)
{
    if(!DEFAULT_SVR()) return;
	m_languageId=langid;
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	DEFAULT_SVR()->GetServerParamCfg().LanguageId=m_languageId;
	if( pMgr != NULL) {pMgr->SetLanguageId(m_languageId);}
    for(int i=0;i<(int)m_zqNCButtonArray.size();i++)
    {
        zqNCButton* pButton=m_zqNCButtonArray[i];
		if(pButton->GetId()==ID_NCBTNID_ENGLISH)
			pButton->SetCheck(m_languageId==LANGUAGE_ENGLISH);
		if(pButton->GetId()==ID_NCBTNID_CHINESE)
			pButton->SetCheck(m_languageId==LANGUAGE_CHINESE);
		if(pButton->GetId()==ID_NCBTNID_TRADITION)
			pButton->SetCheck(m_languageId==LANGUAGE_TRADITION);
	}
	WXLOG_INFO("ChangeLanguage(%d)",langid);
}
#endif
#ifdef _USE_TOP_SPLITTER
void FrameBase::OnToggleViewVideoInfo(bool bCheck)
{
	m_bShowVideoInfo=bCheck;
}
void FrameBase::LoadSaveVideoInfoProfile(bool bload)
{
	std::string strAppPath;
	GlobalFunc::GetPath(strAppPath);
	strAppPath+="videoinfo_url.ini";
	char section[]="main";
	if(bload)
	{
		char retstring[1024]={0};
		GetPrivateProfileString(section,"TodayHotService",g_HttpUrls[UrlIndex_TodayHotService].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_TodayHotService]=retstring;
		GetPrivateProfileString(section,"NameSpace",g_HttpUrls[UrlIndex_NameSpace].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_NameSpace]=retstring;
		GetPrivateProfileString(section,"ALL",g_HttpUrls[UrlIndex_ALL].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_ALL]=retstring;
		GetPrivateProfileString(section,"COPPER",g_HttpUrls[UrlIndex_COPPER].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_COPPER]=retstring;
		GetPrivateProfileString(section,"RUBBER",g_HttpUrls[UrlIndex_RUBBER].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_RUBBER]=retstring;
		GetPrivateProfileString(section,"COTTON",g_HttpUrls[UrlIndex_COTTON].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_COTTON]=retstring;
		GetPrivateProfileString(section,"ARTICLE",g_HttpUrls[UrlIndex_ARTICLE].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_ARTICLE]=retstring;
		GetPrivateProfileString(section,"VIDEOPLAYER",g_HttpUrls[UrlIndex_VIDEOPLAYER].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_VIDEOPLAYER]=retstring;
		GetPrivateProfileString(section,"GuideAction",g_HttpUrls[UrlIndex_GuideAction].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_GuideAction]=retstring;
		GetPrivateProfileString(section,"GuideNameSpace",g_HttpUrls[UrlIndex_GuideNameSpace].c_str(),retstring,1024,strAppPath.c_str());
		g_HttpUrls[UrlIndex_GuideNameSpace]=retstring;
	}
	else
	{
		WritePrivateProfileString(section,"TodayHotService",g_HttpUrls[UrlIndex_TodayHotService].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"NameSpace",g_HttpUrls[UrlIndex_NameSpace].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"ALL",g_HttpUrls[UrlIndex_ALL].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"COPPER",g_HttpUrls[UrlIndex_COPPER].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"RUBBER",g_HttpUrls[UrlIndex_RUBBER].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"COTTON",g_HttpUrls[UrlIndex_COTTON].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"ARTICLE",g_HttpUrls[UrlIndex_ARTICLE].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"VIDEOPLAYER",g_HttpUrls[UrlIndex_VIDEOPLAYER].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"GuideAction",g_HttpUrls[UrlIndex_GuideAction].c_str(),strAppPath.c_str());
		WritePrivateProfileString(section,"GuideNameSpace",g_HttpUrls[UrlIndex_GuideNameSpace].c_str(),strAppPath.c_str());
	}

}
#endif

wxWindow* FrameBase::GetModuleWindow(int ModuleID)
{
    if(m_notebook==NULL) return NULL;
	wxWindow* pPanel=m_mapModuleWindow[ModuleID];
	if(pPanel!=NULL) return pPanel;
	
	switch(ModuleID)
	{
	case GID_QUOT_TABLE://报价表
		pPanel=new CQuotFrame(m_notebook);
		break;
	case GID_ORDER_INSERT_PANEL://下单面板
		pPanel=OrderInsertPanel::CreatePane(m_notebook);
		break;
	case GID_POSITIONALL://持仓
		pPanel=new PositionPanel(m_notebook);
		break;
	case GID_INSTRUMENT://合约列表
		pPanel=new InstrumentPanel(m_notebook,GID_INSTRUMENT);
		break;
	case GID_THRADE_RECORD://成交记录
		pPanel=new TradeRecordPanel(m_notebook,GID_THRADE_RECORD);
		break;
	case  GID_ACCOUNT://资金账户
		pPanel=FundAccountModule::CreatePane(m_notebook);
		break;
	case GID_OPEN_ORDER://未成交单
		//pPanel=new COpenOrder(m_notebook,GID_OPEN_ORDER);
		pPanel=new CAllOrdersPanel(CAllOrdersPanel::conOpenOrderStyle, m_notebook, GID_OPEN_ORDER);
		break;
	case GID_ALL_ORDER://委托单
		pPanel=new CAllOrdersPanel(CAllOrdersPanel::conAllOrderStyle, m_notebook, GID_ALL_ORDER);
		break;
	case GID_QUERY_ORDER_BATCH://批量单
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_BATCH, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_BATCH)\n");
	//	pPanel=new CQueryOrderBatchPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_POSITION://盈损单
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_POSITION, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_POSITION)\n");
	//	pPanel=new CQueryOrderPositionPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_POSIPL://持仓止盈止损
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_POSIPL, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_POSIPL)\n");
	//	pPanel=new CQueryOrderPositionPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_CONDITION://条件单
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_CONDITION, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_CONDITION)\n");
	//	pPanel=new CQueryOrderConditionPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_PARKED://预埋单
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_PARKED, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_PARKED)\n");
	//	pPanel=new CQueryOrderParkedPanel(m_notebook);
		break;
	case GID_QUERYLOCAL_ORDER:
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			pPanel=new CQueryLocalOrderPanel(m_notebook);
		TRACE("MainFrame::GetModuleWindow(GID_QUERYLOCAL_ORDER)\n");
		break;
	case GID_OptionTPrice://期权T型报价窗口
		pPanel=new COptionTPriceWrapper(m_notebook);
		break;
	default:
		break;
	}
	
	if ( NULL == pPanel )
	{
		return NULL;
	}
	m_mapModuleWindow[ModuleID]=pPanel;
	return pPanel;
}
bool FrameBase::GetCurInstrumentGrpInfo_QuotPanel(std::string& GrpName,int& GrpID)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel)
		return ((CQuotFrame*)poQuotPanel)->GetCurInstrumentGrpInfo(GrpName,GrpID);
    else return false;
}

//插件要求新建或删除面板，type==1新建；2删除
wxWindow* FrameBase::Plugin_New_Del_Panel_QuotFrame(int type)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel)
		return ((CQuotFrame*)poQuotPanel)->Plugin_New_Del_Panel(type);
    else return NULL;
}
void FrameBase::GetCurInstrumentID_QuotPanel(std::string& strInstrument)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel)
		((CQuotFrame*)poQuotPanel)->GetCurInstrumentID(strInstrument);
    else strInstrument.clear();
}
wxString FrameBase::GetModuleCaption(int ModuleID)
{
#define CASE_RETURN(cap) case cap:return LOADSTRING(cap##_CAPTION)
	wxString str;
	switch(ModuleID)
	{
		CASE_RETURN(GID_QUOT_TABLE);
		CASE_RETURN(GID_ORDER_INSERT_PANEL);
		CASE_RETURN(GID_POSITIONALL);
		CASE_RETURN(GID_INSTRUMENT);
		CASE_RETURN(GID_THRADE_RECORD);
		CASE_RETURN(GID_ACCOUNT);
		CASE_RETURN(GID_OPEN_ORDER);
		CASE_RETURN(GID_ALL_ORDER);
		CASE_RETURN(GID_QUERYLOCAL_ORDER);
		CASE_RETURN(GID_QUERY_ORDER_BATCH);
		CASE_RETURN(GID_QUERY_ORDER_CONDITION);
		CASE_RETURN(GID_QUERY_ORDER_POSITION);
		CASE_RETURN(GID_QUERY_ORDER_POSIPL);
		CASE_RETURN(GID_QUERY_ORDER_PARKED);
		CASE_RETURN(GID_POSITIONCOMB);
		CASE_RETURN(GID_MULTI_ACCOUNT);
		CASE_RETURN(GID_MULTI_POSITION);
	}
	return str;
}
