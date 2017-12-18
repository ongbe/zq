
#include "stdafx.h"
#include "MyStatusBar.h"
#include "LogDlg.h"
#include <wx/tooltip.h> 
#include "gui/MainFrame.h"
#include "LinkStatusDlg.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

#pragma warning(disable:4996)
#define TIME_WIDTH 100
#define ITEM_WIDTH 40
#define ID_STATIC_QuotTrader 200
#define INVALID_TIME_TEXT "--:--:--"
#define CALC_STATUS_TEXT(exch)  \
  if(m_##exch##Diff)\
    {\
        wxDateTime _now((time_t)(GetTickCount()/1000+m_##exch##Diff)); \
        FindWindowById(ID_STATIC_##exch,this)->SetLabel(_now.FormatTime());\
    }\
    else\
    FindWindowById(ID_STATIC_##exch,this)->SetLabel(wxDateTime::Now().FormatTime())
BEGIN_EVENT_TABLE(MyStatusBar, wxStatusBar)
	EVT_TIMER(wxID_ANY, MyStatusBar::OnTimer)
	EVT_LEFT_DOWN(MyStatusBar::LBtnLeftDown)
	EVT_SIZE(OnSize)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()

MyStatusBar::MyStatusBar(wxWindow *parent)
: wxStatusBar(parent, wxID_ANY)
,m_timer(this)
, m_nStyle( -1 ),
m_LinkStatusDlg(NULL)
{
	vector<string> exchnames;
	int exchcount=0;
    exchcount=GetValidExchangeIDs(exchnames);//交易所数量
	int itemcount=Field_Exchange+exchcount;//列数
    //设置分割
	SetFieldsCount(itemcount);
	int* widths=new int[itemcount];
	widths[Field_LocalTime]=TIME_WIDTH;
	widths[Field_Msg]=-1;
	widths[Field_LayoutStyle]=ITEM_WIDTH*2;
	widths[Field_ReConnect]=ITEM_WIDTH*2;
	widths[Field_QuotTrader]=ITEM_WIDTH;
	for(int i=0;i<exchcount;i++)
		widths[Field_Exchange+i]=TIME_WIDTH;
	SetStatusWidths(itemcount, widths); 
	delete[] widths;
    //设置字体
    wxFont newfont; 
    newfont.SetWeight(wxBOLD);
    SetFont(newfont);
	//创建控件
    wxStaticText* pStatic;
    pStatic=new wxStaticText(this,ID_STATIC_QuotTrader,wxEmptyString,wxDefaultPosition, wxDefaultSize, 0);
	pStatic->Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(MyStatusBar::OnLinkStatus), NULL, this);
	pStatic->SetFont(newfont);

    wxColor ValidColor(128,128,255);
	string strTime;
    int CurTickCount=0;
	wxDateTime logintime;
	vector<string>::iterator it=exchnames.begin();
	for(it;it!=exchnames.end();++it)
	{
		StatusBarField field;
		pStatic=new wxStaticText(this,ID_STATIC_QuotTrader+m_ExchangeFields.size()+1,wxEmptyString,wxDefaultPosition, wxDefaultSize, 0);
		pStatic->SetFont(newfont);
		pStatic->SetForegroundColour(*wxRED);
        if(DEFAULT_SVR())
		    pStatic->SetToolTip( (DEFAULT_SVR()->ExchangeID2String(it->c_str())).c_str());
		field.m_static=pStatic;
		strTime="";
		CurTickCount=0;
        if(DEFAULT_SVR())
		    CurTickCount=DEFAULT_SVR()->GetExchangeTime(it->c_str(),strTime);
		if(CurTickCount>0&&strcmp(strTime.c_str(),INVALID_TIME_TEXT)!=0)
		{
			logintime=GetDateTimeFromString(strTime.c_str());
			field.m_TimeDiff=logintime.GetTicks()-CurTickCount/1000;
			pStatic->SetForegroundColour(ValidColor);
		}
		m_ExchangeFields.push_back(field);
	}
	SetStatusText(LOADSTRING(RECONNECT),Field_ReConnect);

    //
    m_timer.Start(1000);
    //创建log窗口
	LogDlg* pDlg =  LogDlg::GetInstance();
	if( pDlg == NULL )
	{
		pDlg = LogDlg::CreateLogDlg(this);
	}
	m_LinkStatusDlg=new LinkStatusDlg(this);
}
bool MyStatusBar::GetFieldRect(int i, wxRect& rect) const
{
	bool ret=false;
	if(i>=0&&i<GetFieldsCount())
		ret=wxStatusBar::GetFieldRect(i,rect);
	else
		rect=wxRect(0,0,0,0);
	return ret;
}
void MyStatusBar::OnSize(wxSizeEvent& evt)
{
    wxRect rt;
	vector<string> exchnames;
	int exchcount=0;
	exchnames.clear();
    exchcount=GetValidExchangeIDs(exchnames);//交易所数量
    for(int i=0;i<=exchcount;i++)
    {
        GetFieldRect(i+Field_QuotTrader,rt);
        wxWindow* pWin=FindWindowById(ID_STATIC_QuotTrader+i,this);
        if(pWin) 
		{
			int height=pWin->GetRect().GetHeight();
			pWin->SetPosition(wxPoint(rt.x+2,rt.y+(rt.height-height)/2));
		}
    }
}
wxDateTime MyStatusBar::GetDateTimeFromString(const char* pzStr)
{
    wxString strTime=pzStr;
    wxDateTime datetime=wxDateTime::Now();
    if(strTime.Length()>=8)
    {
        datetime.SetHour(atoi(strTime.substr(0,2).c_str()));
        datetime.SetMinute(atoi(strTime.substr(3,2).c_str()));
        datetime.SetSecond(atoi(strTime.substr(6,2).c_str()));
    }  
    return datetime;
}
MyStatusBar::~MyStatusBar()
{
    m_timer.Stop();
	LogDlg::DestroyInstance();
}
wxString MyStatusBar::GetStatusString(int status,bool bdetail)
{
    wxString str;
    if(bdetail)
    {
		switch(status)
		{
		case 0:           str=LOADSTRING(STATUS_DISCONNECTED);break;
		case 1:           str=LOADSTRING(STATUS_CONNECTING);break;
		case 2:           str=LOADSTRING(STATUS_LOGINING);break;
		case 3:           str=LOADSTRING(STATUS_LOGINFAILURE);break;
		case 4:           str=LOADSTRING(STATUS_TRADESETTLEMENTINFOCONFIRMING);break;
		case 5:           str=LOADSTRING(STATUS_TRADESETTLEMENTINFOCONFIRMFAILURE);break;
		case 6:           str=LOADSTRING(STATUS_CONNECTED);break;
		case 7:           str=LOADSTRING(STATUS_DISCONNECTING);break;
		}
    }
    else
    {
        str="☆☆☆☆☆☆★☆";
        str=str.Mid(status*2,2);
    }
    return str;
}
char StyleNamme[][64]={"STATUS_CLASSICS","STATUS_SIMPLE","STATUS_QUICK","STATUS_STANDARD"};
void MyStatusBar::OnTimer( wxTimerEvent& event)
{  
	for(int index=0;index<(int)m_ExchangeFields.size();index++)
	{
		StatusBarField& field=m_ExchangeFields[index];
		if(field.m_TimeDiff)
		{
			 wxDateTime _now((time_t)(GetTickCount()/1000+field.m_TimeDiff)); 
			 field.m_static->SetLabel(_now.FormatTime());
		}
		else
		{
			field.m_static->SetLabel(wxDateTime::Now().FormatTime());
		}
	}
	int hostlink=0,pricelink=0;
    //显示连接状态
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetLinkStatus(&hostlink,&pricelink);
	if(hostlink<0||pricelink<0)
	{
		static int old_quot_status=-1,old_trader_status=-1;
		int quot_status=SS_NOLOGIN,trader_status=SS_NOLOGIN;
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetCurrentStatus(&quot_status,&trader_status);
		wxString strQuotTrader=GetStatusString(quot_status);
		strQuotTrader+=" ";
		strQuotTrader+=GetStatusString(trader_status);
		FindWindowById(ID_STATIC_QuotTrader,this)->SetLabel(strQuotTrader);
		if(old_quot_status!=quot_status||old_trader_status!=trader_status)
		{
			strQuotTrader=LOADSTRING(GUI_QUOTE)+GetStatusString(quot_status,true);
			strQuotTrader+=LOADSTRING(GUI_TRADE);
			strQuotTrader+=GetStatusString(trader_status,true);
			wxWindow* pQTWin=FindWindowById(ID_STATIC_QuotTrader,this);
			pQTWin->SetToolTip(strQuotTrader);
			if(quot_status==6)
			{
				if(trader_status==6)
					pQTWin->SetForegroundColour(*wxGREEN);
				else
					pQTWin->SetForegroundColour(*wxBLUE);
			}
			else if(trader_status==6)
				pQTWin->SetForegroundColour(*wxBLUE);
			else
				pQTWin->SetForegroundColour(*wxBLACK);
			old_quot_status=quot_status;
			old_trader_status=trader_status;
		}
	}
	else
	{
		char statusstring[]=" OGDCI";
		char statuslabel[][32]={"","Opened","Connecting","Connected","Closed","Invalid"};
		wxString strQuotTrader=statusstring[hostlink];
		strQuotTrader+=" ";
        strQuotTrader+=statusstring[pricelink];
		wxWindow* pQTWin=FindWindowById(ID_STATIC_QuotTrader,this);
		if(pQTWin->GetLabel()!=strQuotTrader)
		{
			pQTWin->SetLabel(strQuotTrader);
			strQuotTrader="Host Link:";
			strQuotTrader+=statuslabel[hostlink];
			strQuotTrader+="\nPrice Link:";
			strQuotTrader+=statuslabel[hostlink];
			pQTWin->SetToolTip(strQuotTrader);
		}
	}
	wxCommandEvent MyEvent(wxEVT_GET_STYLE);
	((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
	int layout_style=MyEvent.GetInt();
	wxString *p=(wxString*)MyEvent.GetClientData();
    if(p&&!p->IsEmpty())
    {
        SetStatusText(*p,Field_LayoutStyle);
    }
    else/* if(layout_style!= m_nStyle)*/
	{
		if(layout_style>=0&&layout_style<4)
		{
			SetStatusText(LOADSTRING_TEXT(StyleNamme[layout_style]),Field_LayoutStyle);
		}
		m_nStyle = layout_style;
	}
    SAFE_DELETE(p);

}
void  MyStatusBar::OnLinkStatus( wxMouseEvent& event )
{
	if(m_LinkStatusDlg==NULL)
		m_LinkStatusDlg=new LinkStatusDlg(this);
	if(m_LinkStatusDlg->HasData())
		m_LinkStatusDlg->Show();
}
void MyStatusBar::LBtnLeftDown(wxMouseEvent& event)
{
	int pos_x=event.GetX();
    wxRect rt1,rt2;
    GetFieldRect(Field_Msg,rt1);
	if(pos_x>=0&&pos_x<rt1.GetRight())
	{
		LogDlg* pDlg =  LogDlg::GetInstance();
		if( pDlg == NULL )
		{
			pDlg = LogDlg::CreateLogDlg(this);
		}
		pDlg->Show();
		return;
	}  
	GetFieldRect(Field_ReConnect,rt2);
	if(pos_x>=rt2.GetLeft()&&pos_x<rt2.GetRight())
	{
		int iret=wxMessageBox(LOADSTRING(RECONNECT_SERVER),LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO);
		if(iret==wxYES)
		{
			if(DEFAULT_SVR()) DEFAULT_SVR()->ReConnetServer();
		}
		return;
	}
}

void MyStatusBar::OnLanguageChanged( wxCommandEvent& event )
{
	SetStatusText(LOADSTRING(RECONNECT),Field_ReConnect);
	SetStatusText(LOADSTRING_TEXT(StyleNamme[m_nStyle]),Field_LayoutStyle);
	if(m_LinkStatusDlg)m_LinkStatusDlg->ChangeLanguage();
}
int MyStatusBar::GetValidExchangeIDs(vector<string>& outData)
{
    outData.clear();
    if(!DEFAULT_SVR()) return 0;

	string strTime;
    DEFAULT_SVR()->GetExchangeIDs(outData);//交易所数量
    vector<string>::iterator it;
    for(it=outData.begin();it!=outData.end();)
    {
        if(DEFAULT_SVR()->GetExchangeTime(it->c_str(),strTime)==0)
            it=outData.erase(it);
        else
            ++it;
    }
    return outData.size();
}
