#pragma once

#define DEFAULT_COLOUR                      wxColour(255,255,240)
#define WHITE_COLOUR						wxColour(255,255,255)
#define LAYOUT_FILENAME                     _("layout.xml")
#define SYSCFG_FILENAME                     ("syscfg.xml")
#define USERCFG_FILENAME                    ("usercfg.xml")
#define USERCFGMA_FILENAME                  ("usercfgma.xml")
#define CTPSERVER_FRONTADDR                 _("tcp://asp-sim2-front1.financial-trading-platform.com:26205\ntcp://asp-sim2-md1.financial-trading-platform.com:26213")
#define LOG_TIME_FORMAT_STRING           _("%04d/%02d/%02d %02d:%02d:%02d:%03d") 
#define DATE_FORMAT                      _T("%04d%02d%02d")
#define TIME_FORMAT                      _T("%02d:%02d:%02d")
#define  MAX_PLUGIN_COUNT				50
#define ID_ASKNAMECHOICE                 501
#define ID_ASKNAMEDEL                    502
#define ID_CMD_HIDE_FLOAT_MAIN_PANE  119
#define ID_BASE_CONTEXT_MENU         450
#define ID_MENU_ADDTOMYSELF          550
#define ID_MENU_COLUMNCONFIG         (ID_BASE_CONTEXT_MENU+1)
#define ID_MENU_EXPORT               (ID_BASE_CONTEXT_MENU+2)
#define ID_MENU_SELECTINSTRUMENT     (ID_BASE_CONTEXT_MENU+3)
#define	ID_MENU_PROFIT           	(ID_BASE_CONTEXT_MENU+4)
#define	ID_MENU_CONDITION        	(ID_BASE_CONTEXT_MENU+5)
#define	ID_MENU_DELSELECT        	(ID_BASE_CONTEXT_MENU+6)
#define	ID_MENU_CANCELOPDER      	(ID_BASE_CONTEXT_MENU+7)
#define	ID_MENU_CONDITION_DELETE 	(ID_BASE_CONTEXT_MENU+8)
#define	ID_MENU_CONDITION_PAUSE  	(ID_BASE_CONTEXT_MENU+9)
#define	ID_MENU_CONDITION_ACTIVE 	(ID_BASE_CONTEXT_MENU+10)
#define	ID_MENU_CONDITION_MODIFY 	(ID_BASE_CONTEXT_MENU+11)
#define	ID_MENU_CONFIGUI 	        (ID_BASE_CONTEXT_MENU+12)
#define	ID_MENU_PRICEBACKHAND       (ID_BASE_CONTEXT_MENU+13) 
#define ID_MENU_REMOVE              (ID_BASE_CONTEXT_MENU+14)
#define ID_MENU_REMOVEALL           (ID_BASE_CONTEXT_MENU+15)
#define ID_MENU_CHANGEORDER         (ID_BASE_CONTEXT_MENU+16)
#define ID_MENU_FASTCLOSE			(ID_BASE_CONTEXT_MENU+17)
#define ID_MENU_FASTLOCK			(ID_BASE_CONTEXT_MENU+18)
#define ID_MENU_FUNDSTYLECHANGE     (ID_BASE_CONTEXT_MENU+19)
#define ID_MENU_PATSCHANGEORDER     (ID_BASE_CONTEXT_MENU+20)
#define ID_MENU_AUTO_ADJUST_COL_WIDTH	(ID_BASE_CONTEXT_MENU+21)
// 下持仓止盈止损
#define ID_MENU_ADDNEWPOSIPL		(ID_BASE_CONTEXT_MENU+22)
// 下期权执行
#define ID_MENU_OPTIONEXECORDER		(ID_BASE_CONTEXT_MENU+23)

//数量统计
#define ID_MENU_Statisticsofnumber	(ID_BASE_CONTEXT_MENU+30) 

//多账号菜单项
#define ID_MENU_MULTIACCOUNT_LOGIN              (ID_BASE_CONTEXT_MENU+40)
#define ID_MENU_MULTIACCOUNT_LOGOUT             (ID_BASE_CONTEXT_MENU+41)
#define ID_MENU_MULTIACCOUNT_SETQUOT            (ID_BASE_CONTEXT_MENU+42)
#define ID_MENU_MULTIACCOUNT_SETFIRSTACCOUNT    (ID_BASE_CONTEXT_MENU+43)

#define ID_CONTEXT_PLUGINS_BEGIN	(ID_BASE_CONTEXT_MENU+31) 
#define ID_CONTEXT_PLUGINS_END		(ID_BASE_CONTEXT_MENU+81) 



//
#define CPFLAG_ACCOUNT						(1<<0)
#define CPFLAG_QUOT_TABLE					(1<<1)
#define CPFLAG_INSTRUMENT					(1<<2)
#define CPFLAG_OPEN_ORDER					(1<<3)
#define CPFLAG_ALL_TRUST_ORDER				(1<<4)
#define CPFLAG_ORDER_INSERT_PANEL			(1<<5)
#define CPFLAG_POSITION						(1<<6)
#define CPFLAG_POSITION_DETAIL				(1<<7)
#define CPFLAG_THRADE_RECORD				(1<<8)
#define CPFLAG_QUERY_ORDER_BATCH			(1<<9)
#define CPFLAG_QUERY_ORDER_CONDITION		(1<<10)
#define CPFLAG_QUERY_ORDER_POSITION			(1<<11)
#define CPFLAG_QUERY_ORDER_PARKED			(1<<12)


enum _tagUrlIndex
{
	UrlIndex_TodayHotService,
	UrlIndex_NameSpace,
	UrlIndex_ALL,
	UrlIndex_COPPER,
	UrlIndex_RUBBER,
	UrlIndex_COTTON,
	UrlIndex_ARTICLE,
	UrlIndex_VIDEOPLAYER,
	UrlIndex_GuideAction,
	UrlIndex_GuideNameSpace,
	UrlIndex_Max

};
#define GETTOPWINDOW() ((wxApp*)wxApp::GetInstance())->GetTopWindow()
#define BEGIN_DO_EXPORT(gid)\
    wxString caption,m_timestring,wildcard =LOADSTRING(TEXTFILE_FILTER);\
    {CfgMgr * pMgr = CFG_MGR_DEFAULT();  caption = pMgr!=NULL ? (wxString)pMgr->GetPanelCaption(gid) : wxString("");}\
    wxDateTime t=wxDateTime::Now();\
    m_timestring.Printf(DATE_FORMAT,t.GetYear(),t.GetMonth(),t.GetDay());\
    wxFileDialog dlg(this,LOADSTRING(SAVE_AS),wxEmptyString,caption+m_timestring+".csv",wildcard,wxSAVE);\
    if(dlg.ShowModal()==wxID_OK){\
    wxString strline,path=dlg.GetPath();\
    wxFile file(path,wxFile::write);\
    if(file.IsOpened()){

#define END_DO_EXPORT()\
    file.Close();}}

//////////////////////////////////////////////////////////////////////////////////////////////
#define LIST_EXPORT_CSV_FILE(cap,plist)\
do{\
    wxString strcell,caption=cap,m_timestring,wildcard =LOADSTRING(TEXTFILE_FILTER);\
    caption=GlobalFunc::GetValidName(caption.c_str()).c_str();\
    wxDateTime t=wxDateTime::Now();\
    m_timestring.Printf(DATE_FORMAT,t.GetYear(),t.GetMonth()+1,t.GetDay());\
    int colcount=plist->GetColumnCount();\
    int rowcount=plist->GetItemCount();\
    wxFileDialog dlg(this,LOADSTRING(SAVE_AS),wxEmptyString,caption+m_timestring+".csv",wildcard,wxSAVE);\
    if(dlg.ShowModal()==wxID_OK)\
    {\
        wxString strline,path=dlg.GetPath();\
        wxFile file(path,wxFile::write);\
        if(file.IsOpened())\
        {\
            wxExtListItem item;\
            for(int col=0;col<colcount;col++)\
            {\
                if(plist->GetColumn(col,item))\
                {\
                    strline+=item.GetText();\
                }\
                if(col+1!=colcount) strline+=",";\
            }\
            strline+="\r\n";\
            for(int index=0;index<rowcount;index++)\
            {\
                for(int col=0;col<colcount;col++)\
                {\
					strcell=plist->GetSubItemText(index,col);\
					if(strcell.Find(',')==wxNOT_FOUND)\
						strline+=strcell;\
					else\
					{\
						strline+="\"";\
						strline+=strcell;\
						strline+="\"";\
					}\
                    if(col+1!=colcount) strline+=",";\
                }\
                strline+="\r\n";\
            } \
            file.Write(strline.c_str(),strline.size());\
            file.Close();\
        }\
    } \
}while(0)
////////////////////////////////////////////////////////////////////////////

#define SHOW_FLOAT_PANEL(classname)\
if(m_curConfigPanel)DoReturnNormalUI();\
if(m_notebook->LayoutStyleIsSunGard()){\
	wxWindow* pOrderPanel = GetModuleWindow(GID_ORDER_INSERT_PANEL);\
	if (pOrderPanel&&m_notebook&&m_notebook->IsFloatPaneOpen(pOrderPanel) )\
		m_notebook->DoFloatPaneClose(pOrderPanel);\
    classname* p##classname= new classname(m_notebook,wxID_ANY,wxDefaultPosition,wxSize(0,0));\
	p##classname->SetSungardStyle(true);\
    m_notebook->DoConfigPaneOpen(p##classname);\
    m_notebook->FixedPane(true);\
    m_curConfigPanel=p##classname;\
    m_curConfigPanelClassName=#classname;\
}else{\
	CFloatPanelDlg dlg(this);\
    classname* p##classname= new classname(&dlg,wxID_ANY,wxDefaultPosition,wxSize(0,0));\
	p##classname->SetSungardStyle(false);\
    dlg.SetPanel(p##classname);\
	dlg.SetTitle(p##classname->GetWindowCaption());\
    dlg.ShowModal();\
}

#define CLOSE_FLOAT_PANEL()\
do{\
	if( m_bSungardStyle )\
	{\
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_RETURN_NORMAL_UI);	\
		GETTOPWINDOW()->AddPendingEvent(evt);	\
	}\
	else\
	{\
		this->GetParent()->Close();\
	}\
}while(0)

#define ID_RETURN_NORMAL_UI		4180
#define ID_SAVE_AND_RETURN		4181
#define FLOAT_PANE_HEADER(caption,tname)\
    SetBackgroundColour(DEFAULT_COLOUR);\
    int height_offset=60;\
	wxStaticText* WxStaticText_M1 = new wxStaticText(this, wxID_ANY,tname, wxPoint(10, 18));\
        WxStaticText_M1->SetFont(wxFont(16, wxSWISS, wxNORMAL, wxBOLD, false));\
    wxButton* ppBtnOK=new wxButton(this, ID_RETURN_NORMAL_UI, caption, wxPoint(234, 18), wxSize(75, 25));\
	new wxStaticLine(this, wxID_ANY, wxPoint(10, 45), wxSize(300, -1), wxLI_HORIZONTAL)
#define INSERT_ADDTOMYSELF_ITEM(menu)\
do{\
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();\
    wxMenu* pAddtoSubMenu = NULL;\
    for(int i = 0; i < pCfgMgr->GetInstrumentGroupCount();++i){\
        LPINSTRUMENT_GROUP_CFG pGroup = pCfgMgr->GetInstrumentGroupInfo(i);\
        if( pGroup->IsVisibility == 1){\
            if( pAddtoSubMenu == NULL) pAddtoSubMenu=new wxMenu;\
            pAddtoSubMenu->Append(ID_MENU_ADDTOMYSELF+i, pGroup->GroupName);\
        }\
    }\
    if( pAddtoSubMenu )menu.AppendSubMenu(pAddtoSubMenu,LOADSTRING(CONTEXTMENU_ADDTOMYSELF));\
}while(0)
#define SEND_CONFIG_CHANGED_EVENT(id)\
do{	\
	wxCommandEvent myEvent(wxEVT_CONFIG_CHANGED);\
	wxString strMsg=wxString::Format( "%d", id);\
    DWORD EvtParamID;\
    std::string EvtParamStr(strMsg.c_str());\
    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))\
    {\
        myEvent.SetInt((int)EvtParamID);\
        GETTOPWINDOW()->AddPendingEvent(myEvent);\
    }\
}while(0)
#define SEND_LOAD_CONFIG_APP_EVENT(id)\
do{\
	wxCommandEvent request_event(wxEVT_LOAD_CONFIG_APP);\
	request_event.SetInt( id );\
	GETTOPWINDOW()->ProcessEvent(request_event);\
}while(0)
#define WINDOW_DO_ACTIVE(hWnd)\
do{\
    HWND hForeWnd=::GetForegroundWindow();\
    DWORD dwCurID=::GetCurrentThreadId();\
    DWORD dwForeID=::GetWindowThreadProcessId(hForeWnd,NULL);\
    ::AttachThreadInput(dwCurID,dwForeID,TRUE);\
    ::ShowWindow(hWnd,SW_SHOW);\
    ::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);\
    ::SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);\
    ::SetForegroundWindow(hWnd);\
    ::AttachThreadInput(dwCurID,dwForeID,FALSE);\
}while(0)
#define WM_BEEPMSG				(WM_USER+0x500)
#define PostBeepMessage(iNum)\
do{\
	MainFrame* pFrame=(MainFrame*)GETTOPWINDOW();\
	if(pFrame&&pFrame->GetBeepThreadId())\
		PostThreadMessage(pFrame->GetBeepThreadId(),WM_BEEPMSG,(WPARAM)iNum,(LPARAM)0);\
}while(0)
#define TYPE_APPLYDEFAULT     0x123
#define TYPE_APPLYINIT		  0x124
#define TYPE_APPLYCHANGED     0x125
#define EXCLUDE_AND_SIGNAL(strDisp,alpha)\
do{	 \
for(string::iterator iter = strDisp.begin(); iter != strDisp.end(); )\
{\
	if(*iter==alpha) \
	    strDisp.erase(iter);\
	else\
		++iter;\
}\
}while(0)

enum { conNoticeNewOrderSuccess, conNoticeNewOrderFail, conNoticeTrader, conNoticeCancelSuccess, conNoticeCancelFail };

#define NoticeSound(nType)\
	{\
		wxFrame* pFrame=(wxFrame*)GETTOPWINDOW();\
		if(pFrame)\
		{\
			wxCommandEvent evtBeep(wxEVT_MAIN_NOTICESOUND);\
			evtBeep.SetInt(nType);\
			pFrame->AddPendingEvent(evtBeep);\
		}\
	}

// 是否对下单进行强制审核的宏，0 无强制审核，直接通过接口下单，1 强制审核，通过插件接口下单
#define _FORCEVERIFYORDER 0
// 一个临时进行限多的代码的开关，0 正常下单，1 启用禁止下开仓买入单
#define _TEMPNONEOPENLONG 0
