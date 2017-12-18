#include "StdAfx.h"
#include "FundAccountPanel.h"
#include "FundAccountQryDlg.h"
#include <wx/statline.h> 

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

//#include "zqAuiNotebook.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-TradeRecord/TradeRecordPanel.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "gui-common/FrameBase.h"

#define COLUMN_SIZE 100
#define GETFUNDSTR(d,str,t)\
do{\
    if(util::isInvalidValue(t))\
		str.Printf("-");	\
	else\
	str = GlobalFunc::GetAccountFormatString(t, d).c_str();\
}while(0)
FundAccountPanel* m_pFundAccountPanel=NULL;
BEGIN_EVENT_TABLE(FundAccountPanel,wxScrolledWindow)

	EVT_BUTTON(ID_REFRESHBUTTON,BankFutureTransfer)
	EVT_BUTTON(wxID_OK,QryTradingAccount)
	EVT_BUTTON(ID_BTN_LEFT,OnBtnLeft)
	EVT_BUTTON(ID_BTN_RIGHT,OnBtnRight)
	EVT_CFG_CHANGE(OnCfgChanged)
	//EVT_STYLE_CHANGE(OnCfgChanged)
	EVT_SIZE( FundAccountPanel::OnSize )
	EVT_LIST_EXT_CONTEXT_MENU(FundAccountPanel::OnListConTextMenu)
	EVT_CONTEXT_MENU(FundAccountPanel::OnConTextMenu)
	EVT_MENU(ID_MENU_COLUMNCONFIG,FundAccountPanel::OnConfig)
	EVT_MENU(ID_MENU_EXPORT,FundAccountPanel::OnExportList)
	EVT_MENU(ID_MENU_FUNDSTYLECHANGE,FundAccountPanel::OnStyleChange)
	EVT_BUTTON(ID_ALLCALLBACK,FundAccountPanel::OnAllCallBack)
	EVT_BUTTON(ID_RspQryTradingAccount,FundAccountPanel::OnCallBack_RspQryTradingAccount)
	EVT_SUBSCRIBE(FundAccountPanel::OnSubscrible)
	EVT_COMMAND(wxID_ANY, wxEVT_STYLE_CHANGED,FundAccountPanel::OnCfgChanged)
	EVT_COMBOBOX(ID_CBX_ACCOUNT, FundAccountPanel::OnAccountChangeSel)
#ifdef _USE_MULTI_LANGUAGE
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE,  FundAccountPanel::OnLanguageChange)
#endif
END_EVENT_TABLE()
//构造函数
FundAccountPanel::FundAccountPanel(wxWindow *parent,
                                   wxWindowID winid ,
                                   const wxPoint& pos ,
                                   const wxSize& size ,
                                   long style,
                                   const wxString& name ):
wxScrolledWindow(parent,winid,pos,size,style,name),
m_lineItem(100,20),
m_pFundAccountQryDlg(NULL),
m_TextColor(*wxWHITE),
m_BgColor(wxColour(128,128,128)),
m_offset(30)
{
    m_StartBlockOrd=0;
    m_BlockCount=0;
    m_mainfont=GetFont();
    m_pFundAccountPanel=this;
	m_strCurAccount = "";
    DoInitQryStep();
}
//订阅和取消订阅消息响应
void FundAccountPanel::OnSubscrible(wxCommandEvent& evt)
{
	if(evt.GetInt())
	{
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTrade, GID_ACCOUNT, TradeCallBackFunc);	
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_ACCOUNT, AllCallBackFunc);						
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderAction, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnDepthMarketData, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingAccount, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingNotice, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryDepthMarketData, GID_ACCOUNT, AllCallBackFunc);
	}
	else
	{
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnTrade, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnOrder, GID_ACCOUNT);						
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderAction, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderInsert, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingAccount, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingNotice, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryDepthMarketData, GID_ACCOUNT);
	}
}
//在切换风格时动态删除原来全部控件
void FundAccountPanel::RemoveAllControl()
{
    for(int i=ID_BASE_LINE;i<ID_BASE_ITEM+40;i++)
    {
        wxWindow* win=FindWindowById(i,this);
        if(win) delete win;
    }
    wxWindow* pButton=FindWindowById(wxID_OK,this);
    if(pButton) delete pButton;
    pButton=FindWindowById(ID_BTN_LEFT,this);
    if(pButton) delete pButton;
    pButton=FindWindowById(ID_BTN_RIGHT,this);
    if(pButton) delete pButton;
    wxWindow* pListBar=FindWindowById(ID_LISTCTRL,this);
    if(pListBar) delete pListBar;
}
//初始化网格资金栏
void FundAccountPanel::InitTabBar()
{
    RemoveAllControl();
   // SetFont(m_mainfont);
    SetForegroundColour(*wxBLACK);
    SetBackgroundColour(DEFAULT_COLOUR);
    wxExtListCtrl* pList=new wxExtListCtrl(this,ID_LISTCTRL,wxPoint(0,0),wxSize(520,180),
        wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_NO_HEADER|wxLCEXT_VRULES|wxLCEXT_HRULES|wxNO_BORDER);

	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return ;
    LPLIST_TABLE_CFG pTable = pMgr->GetListCfg(GID_ACCOUNT);
    if(pTable == NULL) return;
	
	pList->SetBackgroundColour(pTable->BackgroundColor);
	pList->SetHeaderBackgroundColour(pTable->HeadBackgroundColor);
	pList->SetHeaderForegroundColour(pTable->HeadColor);//白  16777215
	pList->SetForegroundColour(pTable->TextColor);
	pList->SetOddRowColour(pTable->OddLineBackgroundColor);
	pList->SetEvenRowColour(pTable->EvenLineBackgroundColor);

    wxExtListItem col;	
	int nColIndex=0,itemcount = pMgr->GetListColCount(GID_ACCOUNT );
	int VisibleCount=pMgr->GetListVisibleColCount(GID_ACCOUNT );
	m_row=(VisibleCount+1)/2;
    //一行显示4列
    for(int col=0;col<4;col++)
    {
        pList->InsertColumn(col,wxEmptyString,wxLISTEXT_ALIGN_LEFT,COLUMN_SIZE);

    }
	m_mapPosition.clear();
	for(int i=0;i<itemcount;i++)
	{
		LPLIST_COLUMN_CFG  pColCfg = pMgr->GetListColCfg(GID_ACCOUNT, i);
		if(pColCfg->Visibility)
		{
			
			if(nColIndex<m_row)
			{
				pList->InsertItem(nColIndex,pMgr->GetColCfgCaption(GID_ACCOUNT,pColCfg->id));
				pList->SetItem(nColIndex,1,"-");
				m_mapPosition[pColCfg->id]=nColIndex;
			}
			else
			{
				pList->SetItem((nColIndex-m_row),2,pMgr->GetColCfgCaption(GID_ACCOUNT,pColCfg->id));
			    pList->SetItem((nColIndex-m_row),3,"-");
				m_mapPosition[pColCfg->id]=nColIndex;
			}
			nColIndex++;
		}
    }
	wxRect rect;
    if(pList->GetItemCount()>0)
    {
        pList->GetItemRect(0, rect);
        pList->SetSize(COLUMN_SIZE*4,m_row*rect.height);
        if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI)
        {
            wxRect listRect = pList->GetRect();
            wxWindow* pStatic = FindWindowById(ID_SELECT_ACCOUNT, this);
            if ( NULL == pStatic )
            {
                new wxStaticText(this, ID_SELECT_ACCOUNT, LOADSTRING(ID_SELECT_ACCOUNT), wxPoint(40, listRect.height+5), wxSize(50, 20));
            }
            else
            {
                pStatic->SetPosition(wxPoint(40, listRect.height+5));
            }

            wxWindow* pCombox = FindWindowById(ID_CBX_ACCOUNT, this);
            if ( NULL == pCombox )
            {
                new wxComboBox(this, ID_CBX_ACCOUNT, "", wxPoint(90, listRect.height+5), wxSize(100, 20));
            }
            else
            {
                pCombox->SetPosition(wxPoint(90, listRect.height+5));
            }
        }
    }


    new wxButton(this, wxID_OK,LOADSTRING(ID_QUERY_ACCOUNTBTN), wxPoint(10,pList->GetSize().y+10), wxSize(m_lineItem.x,m_lineItem.y*2),0);
	SetScrollRate( 10, 10 );
	SetVirtualSize(pList->GetSize().GetX()+1,pList->GetSize().y+11+m_lineItem.y*2);
	wxFont font;
	font.SetNativeFontInfoUserDesc(pTable->szFont);
	pList->SetFont(font);
    PlatformStru_TradingAccountInfo data;
	memset(&data,0,sizeof(data));
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetTradingAccount(m_strCurAccount, data)==0)
		UpdateUIData(data);
	PROCESS_LANGUAGE_EVENT();
}
void FundAccountPanel::InitHorizontalBar()
{
    CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
    if( pMgr == NULL) return ;
    LPLIST_TABLE_CFG pTable   =pMgr->GetListCfg(GID_ACCOUNT);
    if(pTable == NULL) return;
    RemoveAllControl();
    SetForegroundColour(wxColour(255,255,255));     //pTable->HeadColor
    SetBackgroundColour(wxColour(112,112,112));     //pTable->HeadBackgroundColor
    wxFont PanelFont,tmpFont;
    PanelFont.SetNativeFontInfoUserDesc(pTable->szFont); 
    SetFont(PanelFont);// SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, FALSE));
    LPLIST_COLUMN_CFG pColCfg;
    int index=0,colnum = pMgr->GetListColCount(GID_ACCOUNT );
	int showCount=colnum;

    wxButton* shiftbtn=new wxButton(this, ID_BTN_LEFT,"<", wxPoint(0,0), wxSize(m_lineItem.y,m_lineItem.y-1),0);
    shiftbtn->SetForegroundColour(wxColour(77,97,133));
    shiftbtn->SetBackgroundColour(wxColour(177,203,250));
    tmpFont=PanelFont;
    tmpFont.SetWeight(wxFONTWEIGHT_BOLD);
    shiftbtn->SetFont(tmpFont);
    shiftbtn=new wxButton(this, ID_BTN_RIGHT,">", wxPoint(0,m_lineItem.y), wxSize(m_lineItem.y,m_lineItem.y-1),0);
    shiftbtn->SetForegroundColour(wxColour(77,97,133));
    shiftbtn->SetBackgroundColour(wxColour(177,203,250));
    shiftbtn->SetFont(tmpFont);

    for(int i=0;i<showCount;i++)
    {
        pColCfg = pMgr->GetListColCfg(GID_ACCOUNT, i);
        if(pColCfg->Visibility)
        {
            wxStaticLine* itemStaticLine2 = new wxStaticLine(this, ID_BASE_LINE+index, 
                wxPoint(index*(m_lineItem.x+2)-2+m_offset,3), wxSize(2,m_lineItem.y*2-4), wxLI_VERTICAL );
            wxStaticText* itemStaticText3 = new wxStaticText(this, ID_BASE_LABEL+index, 
                                                                pMgr->GetColCfgCaption(GID_ACCOUNT,pColCfg->id), 
                                                                wxPoint(0,0), 
                                                                wxSize(m_lineItem.x-2,m_lineItem.y),
                                                                wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
            wxStaticText* itemStaticText4 = new wxStaticText(this, ID_BASE_ITEM+index, 
                                                                wxEmptyString, 
                                                                wxPoint(0,0), 
                                                                wxSize(m_lineItem.x-2,m_lineItem.y),
                                                                wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
            tmpFont=PanelFont;
            tmpFont.SetWeight(wxFONTWEIGHT_BOLD);
            tmpFont.SetPointSize(PanelFont.GetPointSize()+1);
            itemStaticText4->SetFont(tmpFont);
			m_mapPosition[pColCfg->id]=index;
            index++;
        }
    }

    //根据配置，需要显示的项目总数
    m_BlockCount=index;       

	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI)
	{
		wxWindow* pStatic = FindWindowById(ID_SELECT_ACCOUNT, this);
		if ( NULL == pStatic )
		{
			new wxStaticText(this, ID_SELECT_ACCOUNT, LOADSTRING(ID_SELECT_ACCOUNT), wxPoint(index*(m_lineItem.x+2)+m_offset, 15), wxSize(50, 20));
		}
		else
		{
			pStatic->SetPosition(wxPoint(index*(m_lineItem.x+2)+m_offset, 15));
		}
		
		wxWindow* pCombox = FindWindowById(ID_CBX_ACCOUNT, this);
		if( NULL == pCombox )
		{
			new wxComboBox(this, ID_CBX_ACCOUNT, "", wxPoint(index*(m_lineItem.x+2)+m_offset+60, 15), wxSize(100, 20));
		}
		else
		{
			pCombox->SetPosition(wxPoint(index*(m_lineItem.x+2)+m_offset+60, 15));
		}
		
		wxButton* itemButton18 = new wxButton(this, wxID_OK,LOADSTRING(ID_QUERY_ACCOUNTBTN), wxPoint(index*(m_lineItem.x+2)+m_offset+170,5), wxSize(m_lineItem.x*2/3,m_lineItem.y*2-2),0);
	}
	else
	{
		 wxButton* itemButton18 = new wxButton(this, wxID_OK,LOADSTRING(ID_QUERY_ACCOUNTBTN), wxPoint(0,0), wxSize(m_lineItem.x*2/3,m_lineItem.y*2-2),0);
	}

    RePosition();
	SetVirtualSize(GetSize());
    PlatformStru_TradingAccountInfo data;
	memset(&data,0,sizeof(data));
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetTradingAccount(m_strCurAccount, data)==0)
		UpdateUIData(data);
	PROCESS_LANGUAGE_EVENT();
}

//内容重新定位，在初始化后、OnSize后、左右移后进行
//会隐藏在显示区域以外的内容
void FundAccountPanel::RePosition()
{
    int i;
    wxWindow* tmpwin;
    wxRect Rect = GetClientRect();
    int ClientWid = Rect.GetWidth();
    int ClientHei = Rect.GetHeight();
    int QueryBtnWid=0,QueryBtnHei=0;
    int ShiftBtnWid=0,ShiftBtnHei=0;

    wxWindow *QueryBtn=FindWindowById(wxID_OK,this);
    wxWindow *LShiftBtn=FindWindowById(ID_BTN_LEFT,this);
    wxWindow *RShiftBtn=FindWindowById(ID_BTN_RIGHT,this);

    if(QueryBtn) QueryBtn->GetSize(&QueryBtnWid,&QueryBtnHei);
    if(LShiftBtn) LShiftBtn->GetSize(&ShiftBtnWid,&ShiftBtnHei);

    //根据配置，需要显示的项目总数
    if(m_BlockCount>40) m_BlockCount=40;

    //当前显示的起始块的序号
    if(m_StartBlockOrd>=m_BlockCount) m_StartBlockOrd=m_BlockCount-1;
    if(m_StartBlockOrd<0) m_StartBlockOrd=0;

    if(m_lineItem.x<40) m_lineItem.x=40;

    //最大可显示的内容块数量
    int MaxVisibleBlockCount=(ClientWid-QueryBtnWid-ShiftBtnWid)/m_lineItem.x;
    //处理临界条件
    if(MaxVisibleBlockCount==m_BlockCount-1&&(ClientWid-QueryBtnWid)/m_lineItem.x==m_BlockCount)
        MaxVisibleBlockCount=m_BlockCount;

    //实际显示块数量
    int VisibleBlockCount=MaxVisibleBlockCount;
    if(VisibleBlockCount>=m_BlockCount)
    {
        m_StartBlockOrd=0;
        VisibleBlockCount=m_BlockCount;
    }
    if(VisibleBlockCount>m_BlockCount-m_StartBlockOrd)
        VisibleBlockCount=m_BlockCount-m_StartBlockOrd;
    if(VisibleBlockCount<MaxVisibleBlockCount&&m_StartBlockOrd>0)
    {
        int Delta=MaxVisibleBlockCount-VisibleBlockCount;
        if(Delta>m_StartBlockOrd) Delta=m_StartBlockOrd;
        m_StartBlockOrd-=Delta;
        VisibleBlockCount+=Delta;
    }


    //隐藏不需要显示的内容块(超出可显示区域)
    //定位和显示需要显示的内容块
    for(i=0;i<m_BlockCount;i++)
    {
        bool bShow = i>=m_StartBlockOrd&&i<m_StartBlockOrd+VisibleBlockCount ? true : false;

        tmpwin = FindWindowById(ID_BASE_LINE+i,this);
        if(tmpwin!=NULL)
        {
            if(bShow)
            {
                tmpwin->SetPosition(wxPoint((i-m_StartBlockOrd)*m_lineItem.x+m_lineItem.x-2,3));
                if(!tmpwin->IsShown()) tmpwin->Show();
            }
            else if(tmpwin->IsShown()) tmpwin->Hide();
        }


        tmpwin = FindWindowById(ID_BASE_LABEL+i,this);
        if(tmpwin!=NULL)
        {
            if(bShow)
            {
                tmpwin->SetPosition(wxPoint((i-m_StartBlockOrd)*m_lineItem.x,5));
                if(!tmpwin->IsShown()) tmpwin->Show();
            }
            else if(tmpwin->IsShown()) tmpwin->Hide();
        }

        tmpwin = FindWindowById(ID_BASE_ITEM+i,this);
        if(tmpwin!=NULL)
        {
            if(bShow)
            {
                tmpwin->SetPosition(wxPoint((i-m_StartBlockOrd)*m_lineItem.x,m_lineItem.y+5));
                if(!tmpwin->IsShown()) tmpwin->Show();
            }
            else if(tmpwin->IsShown()) tmpwin->Hide();
        }
    }

    //处理ShiftBtn的定位和显示
    bool bShowShiftBtn = VisibleBlockCount<m_BlockCount ? true : false;
    if(bShowShiftBtn)
    {
        if(RShiftBtn)
        {
            RShiftBtn->SetPosition(wxPoint(ClientWid-QueryBtnWid-ShiftBtnWid,1));
            if(!RShiftBtn->IsShown()) 
                RShiftBtn->Show();
        }
        if(LShiftBtn)
        {
            LShiftBtn->SetPosition(wxPoint(ClientWid-QueryBtnWid-ShiftBtnWid,m_lineItem.y+1));
            if(!LShiftBtn->IsShown()) 
                LShiftBtn->Show();
        }
    }
    else
    {
        if(RShiftBtn && RShiftBtn->IsShown()) 
            RShiftBtn->Hide();
        if(LShiftBtn && LShiftBtn->IsShown()) 
            LShiftBtn->Hide();
    }

    //处理QueryBtn的定位和显示
    if(QueryBtn)
    {
        if(bShowShiftBtn)
        {
            QueryBtn->SetPosition(wxPoint(ClientWid-QueryBtnWid,2));
        }
        else 
        {
            int Pox = VisibleBlockCount*m_lineItem.x + (m_lineItem.x-QueryBtnWid)/2;
            if(Pox>ClientWid-QueryBtnWid) Pox=ClientWid-QueryBtnWid;
            QueryBtn->SetPosition(wxPoint(Pox,2));
        }
        if(!QueryBtn->IsShown()) 
            QueryBtn->Show();
    }
}

void FundAccountPanel::OnBtnLeft(wxCommandEvent& event)
{
    if(m_StartBlockOrd>0)
    {
        m_StartBlockOrd--;
        RePosition();
    }
}
void FundAccountPanel::OnBtnRight(wxCommandEvent& event)
{
    if(m_StartBlockOrd<m_BlockCount-1)
    {
        m_StartBlockOrd++;
        RePosition();
    }
}
FundAccountPanel::~FundAccountPanel(void)
{

}


int FundAccountPanel::AllCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(!m_pFundAccountPanel)
        return 0;
    if(data.BID==BID_RtnDepthMarketData)
    {
        bool bDepthMarketChange=false;
        DataRtnDepthMarketData& refData = *(DataRtnDepthMarketData*)(const_cast<AbstractBusinessData*>(&data));	
        if(refData.bHavePosition)
        {
            wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_ALLCALLBACK);
            m_pFundAccountPanel->AddPendingEvent(evt);
        }
    }
    else if(data.BID==BID_RspQryTradingAccount)
    {
        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_RspQryTradingAccount);
        m_pFundAccountPanel->AddPendingEvent(evt);
    }
    else
    {
        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_ALLCALLBACK);
        m_pFundAccountPanel->AddPendingEvent(evt);
    }
	return 0;
}
int FundAccountPanel::TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    //if(DEFAULT_SVR()&&DEFAULT_SVR()->GetModuleDataQry())
    //{
    //    if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
    //        return 0;

    //    if(!m_pFundAccountPanel)
    //        return 0;
    //}
    wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_ALLCALLBACK);
    m_pFundAccountPanel->AddPendingEvent(evt);
    return 0;
}
//重新计算资金栏各项
void FundAccountPanel::OnAllCallBack(wxCommandEvent& event)
{
	PlatformStru_TradingAccountInfo NewAccountInfo;
	memset(&NewAccountInfo,0,sizeof(NewAccountInfo));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetTradingAccount(m_strCurAccount, NewAccountInfo)==0)
	{
		if(memcmp(&(NewAccountInfo),&(m_AccountField),sizeof(m_AccountField))!=0)
		{
			memcpy(&(m_AccountField),&(NewAccountInfo),sizeof(m_AccountField));
		    UpdateUIData();
        }
	}
}

//查询资金返回结果，重新计算资金栏各项
void FundAccountPanel::OnCallBack_RspQryTradingAccount(wxCommandEvent& event)
{
	PlatformStru_TradingAccountInfo NewAccountInfo;
	memset(&NewAccountInfo,0,sizeof(NewAccountInfo));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetTradingAccount(m_strCurAccount, NewAccountInfo)==0)
	{
		if(memcmp(&(NewAccountInfo),&(m_AccountField),sizeof(m_AccountField))!=0)
		{
			memcpy(&(m_AccountField),&(NewAccountInfo),sizeof(m_AccountField));
		    UpdateUIData();
        }
        if(m_pFundAccountQryDlg&&m_pFundAccountQryDlg->IsVisible())
	        m_pFundAccountQryDlg->SetContent();
	}
}
//void FundAccountPanel::OnTimer(wxTimerEvent& event)
//{
//
//}
void FundAccountPanel::BankFutureTransfer(wxCommandEvent& event)
{
    RefreshUI();
}
void FundAccountPanel::QryTradingAccount(wxCommandEvent& event)
{
    RefreshUI();
	if(!m_pFundAccountQryDlg)
		m_pFundAccountQryDlg=new FundAccountQryDlg(((wxApp*)wxApp::GetInstance())->GetTopWindow());

	m_pFundAccountQryDlg->Hide();
//	m_pFundAccountQryDlg->SetContent();
    //先显示“正在查询......”
    //注意：这里没有考虑pats的情况
	m_pFundAccountQryDlg->SetContent2(LOADSTRING(QueryInProgress));
	m_pFundAccountQryDlg->CenterOnScreen();
	m_pFundAccountQryDlg->Show();
}
int FundAccountPanel::GetIdFromText(std::string strText)
{
    std::map<std::string,int>::iterator it=m_Name2Id.find(strText);
    if(it!=m_Name2Id.end())
        return it->second;
    else
        return -1-ID_BASE_ITEM;
}
void FundAccountPanel::UpdateUIData(PlatformStru_TradingAccountInfo& field)
{
    memcpy(&m_AccountField,&field,sizeof(PlatformStru_TradingAccountInfo));
    UpdateUIData();
}
void FundAccountPanel::UpdateUIData()
{
    
	int digit=FindWindowById(ID_LISTCTRL,this)?2:0;
    wxString strTemp;
    GETFUNDSTR(digit,strTemp,m_AccountField.StaticProfit);
    SetItemValue(FUNDACCOUNT_STATICPROFIT_COL_ID,strTemp);//静态权益
    GETFUNDSTR(digit,strTemp,m_AccountField.FrozenMargin);
    SetItemValue(FUNDACCOUNT_FROZEN_MARGIN_COL_ID,strTemp);//冻结保证金
    GETFUNDSTR(digit,strTemp,m_AccountField.FrozenCommission);
    SetItemValue(FUNDACCOUNT_FROZEN_COMMISSION_COL_ID,strTemp);//冻结手续费
    GETFUNDSTR(digit,strTemp,m_AccountField.CurrMargin);
    SetItemValue(FUNDACCOUNT_CURRMARGIN_COL_ID,strTemp);//占用保证金
    GETFUNDSTR(digit,strTemp,m_AccountField.Commission);
    SetItemValue(FUNDACCOUNT_COMMISSION_COL_ID,strTemp);//手续费
    GETFUNDSTR(digit,strTemp,m_AccountField.PreBalance);
    SetItemValue(FUNDACCOUNT_PREBALANCE_COL_ID,strTemp);//上次结算准备金
    GETFUNDSTR(digit,strTemp,m_AccountField.PreCredit);
    SetItemValue(FUNDACCOUNT_PRECREDIT_COL_ID,strTemp);//上次信用额度
    GETFUNDSTR(digit,strTemp,m_AccountField.PreMortgage);
    SetItemValue(FUNDACCOUNT_PREMORTGAGE_COL_ID,strTemp);//上次质押金额
    GETFUNDSTR(digit,strTemp,m_AccountField.Mortgage);
    SetItemValue(FUNDACCOUNT_MORTGAGE_COL_ID,strTemp);//质押金额
    GETFUNDSTR(digit,strTemp,m_AccountField.FrozenCash);
    SetItemValue(FUNDACCOUNT_FROZENCASH_COL_ID,strTemp);//冻结资金
    GETFUNDSTR(digit,strTemp,m_AccountField.Available);
    SetItemValue(FUNDACCOUNT_AVALIABLE_COL_ID,strTemp);//可用资金
    GETFUNDSTR(digit,strTemp,m_AccountField.PositionProfit);
    SetItemValue(FUNDACCOUNT_POSITIONPROFIT_COL_ID,strTemp);//持仓盈亏
    GETFUNDSTR(digit,strTemp,m_AccountField.CloseProfit);
    SetItemValue(FUNDACCOUNT_CLOSEPROFIT_COL_ID,strTemp);//平仓盈亏
    GETFUNDSTR(digit,strTemp,m_AccountField.DynamicProfit);
    SetItemValue(FUNDACCOUNT_DYNAMICPROFIT_COL_ID,strTemp);//动态权益
    GETFUNDSTR(digit,strTemp,m_AccountField.WithdrawQuota);
    SetItemValue(FUNDACCOUNT_WITHDRAWQUOTA_COL_ID,strTemp);//可取资金
    if(util::isInvalidValue(m_AccountField.RiskDegree))
		strTemp.Printf("-");
	else
		strTemp.Printf("%.2f%%",m_AccountField.RiskDegree);
    SetItemValue(FUNDACCOUNT_RISKDEGREE_COL_ID,strTemp);//风险度=（占用保证金+交割保证金）/动态权益
    GETFUNDSTR(digit,strTemp,m_AccountField.Credit);
    SetItemValue(FUNDACCOUNT_CREDIT_COL_ID,strTemp);//信用金额
    GETFUNDSTR(digit,strTemp,m_AccountField.Deposit);
    SetItemValue(FUNDACCOUNT_DEPOSITE_COL_ID,strTemp);//今日入金
    GETFUNDSTR(digit,strTemp,m_AccountField.Withdraw);
    SetItemValue(FUNDACCOUNT_WITHDRAW_COL_ID,strTemp);//今日出金
 
	strTemp.Printf("%s",m_AccountField.BPRemaining);
	SetItemValue(FUNDACCOUNT_BPREMAINING_COL_ID,strTemp);//剩余购买力
    strTemp.Printf("%s",m_AccountField.BPUsed);
    SetItemValue(FUNDACCOUNT_BPUSED_COL_ID,strTemp);//占用的资金
    strTemp.Printf("%s",m_AccountField.Exposure);
    SetItemValue(FUNDACCOUNT_EXPOSURE_COL_ID,strTemp);//合约占用资金比率
    strTemp.Printf("%s",m_AccountField.BurnRate);
    SetItemValue(FUNDACCOUNT_BURNRATE_COL_ID,strTemp);//合约的盈亏比率
    strTemp.Printf("%s",m_AccountField.MarginPaid);
    SetItemValue(FUNDACCOUNT_MARGINPAID_COL_ID,strTemp);//合约的盈亏比率
}
void FundAccountPanel::SetItemValue(int listrow,wxString strTemp)
{
	std::map<int,int>::iterator it=m_mapPosition.find(listrow);
	if(it==m_mapPosition.end())
		return;
    wxExtListCtrl* pList=((wxExtListCtrl*)FindWindowById(ID_LISTCTRL,this));
    if(pList)
    {
        if(listrow>=0) pList->SetItem((it->second)%m_row,(it->second)<m_row?1:3,strTemp);
    }
    else
    {
        //int id=ID_BASE_ITEM+GetIdFromText(ItemLabel[listrow]);
		int id=ID_BASE_ITEM+it->second;
        if(id>0) 
        {
            wxStaticText* pText=(wxStaticText*)FindWindowById(id,this);
            if(pText) 
            {
                pText->SetLabel(strTemp);

                //根据内容长度调整字体
                wxFont PanelFont=GetFont();
                wxFont theFont=pText->GetFont();
                wxSize sz=pText->GetBestSize();
                if(PanelFont.GetPointSize()<theFont.GetPointSize())
                {
                    //如果显示不下，则把字号缩小一号
                    if(sz.GetWidth()>m_lineItem.GetWidth()-2)
                        pText->SetFont(PanelFont);
                }
            }
        }
    }
}

void FundAccountPanel::OnListConTextMenu(wxContextMenuEvent& event)
{
	wxContextMenuEvent evt(wxEVT_CONTEXT_MENU);
	AddPendingEvent(evt);
}
void FundAccountPanel::OnConTextMenu(wxContextMenuEvent& event)
{
    wxMenu menu;
	if ( m_layoutstyle== STYLE_SIMPLE||m_layoutstyle==STYLE_SunGard )
	{
		menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	}
	
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

    POINT pt;
    ::GetCursorPos(&pt);
    wxPoint point(pt.x,pt.y);
    point = ScreenToClient(point);
    PopupMenu(&menu,point.x,point.y);
}
void FundAccountPanel::OnConfig(wxCommandEvent& event)
{
	SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_ACCOUNT);
}

void FundAccountPanel::OnExportList(wxCommandEvent& event)
{
    wxExtListCtrl* pList = (wxExtListCtrl*)FindWindowById(ID_LISTCTRL,this);
	if ( NULL != pList )
	{
	    CfgMgr * pMgr = CFG_MGR_DEFAULT();  
        wxString cap = pMgr!=NULL ? (wxString)pMgr->GetPanelCaption(GetId()) : wxString("FundPanel");
		LIST_EXPORT_CSV_FILE(cap, pList);
	}
}

void FundAccountPanel::OnStyleChange(wxCommandEvent& event)
{
	if(FindWindowById(ID_LISTCTRL,this))
        InitHorizontalBar(); 
	else
		InitTabBar();
    Refresh();
}
//重新从交易所查询资金栏各项
void FundAccountPanel::RefreshUI()
{
    //发起查询
	PlatformStru_QryTradingAccount QryTradingAccount;
	if(DEFAULT_SVR()) DEFAULT_SVR()->ReqQryTradingAccount(QryTradingAccount);
}
void FundAccountPanel::OnSize( wxSizeEvent& event )
{
	wxWindow* pListBar=FindWindowById(ID_LISTCTRL,this);
	if(pListBar) 
	{
		pListBar->SetPosition(wxPoint(0,0));
		wxWindow* pButton=FindWindowById(wxID_OK,this);
		if(pButton) pButton->SetPosition(wxPoint(10,pListBar->GetSize().y+10));
	}
	else
	{
		RePosition();
		SetVirtualSize(GetSize());
	}
	event.Skip();
}
void FundAccountPanel::OnCfgChanged(wxCommandEvent& evt)
{
	wxCommandEvent MyEvent(wxEVT_GET_STYLE);
	((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
	m_layoutstyle=MyEvent.GetInt();
    void *p=MyEvent.GetClientData();
    SAFE_DELETE(p);
    if(m_layoutstyle== STYLE_SIMPLE||m_layoutstyle==STYLE_SunGard)
        InitTabBar();
    else
        InitHorizontalBar(); 
    //Refresh();
}
FundAccountPanel * FundAccountPanel::CreatePane(wxWindow* parent)
{
	if ( m_pFundAccountPanel == NULL )
	{
		return  new FundAccountPanel( parent, GID_ACCOUNT );
	}
	else
	{
		if ( parent != m_pFundAccountPanel->GetParent())
		{
			m_pFundAccountPanel->Reparent(parent);
		}

		return m_pFundAccountPanel;
	}
}
#ifdef _USE_MULTI_LANGUAGE
void FundAccountPanel::OnLanguageChange(wxCommandEvent& evt)
{
	wxExtListCtrl* pList=((wxExtListCtrl*)FindWindowById(ID_LISTCTRL,this));
	CfgMgr * pMgr =  CFG_MGR_DEFAULT(); 
	int count = pMgr->GetListColCount(GID_ACCOUNT );
	std::string strNewContent;
	LPLIST_COLUMN_CFG pColCfg=NULL;
	if(pList)
	{
		for(int col=0;col<count;col++)
		{
			pColCfg = pMgr->GetListColCfg(GID_ACCOUNT, col);
			if(pColCfg->Visibility)
			{
				strNewContent=pMgr->GetColCfgCaption(GID_ACCOUNT,pColCfg->id);
				pList->SetItem(col%m_row,col<m_row?0:2,strNewContent);
			}
		}
	}
	else
	{
		wxWindow* pLabel=NULL;
		int index=0;
		for(int col=0;col<count;col++)
		{
			pColCfg = pMgr->GetListColCfg(GID_ACCOUNT, col);
			if(pColCfg->Visibility)
			{
				pLabel=FindWindowById(ID_BASE_LABEL+index,this);
				strNewContent=pMgr->GetColCfgCaption(GID_ACCOUNT,pColCfg->id);
				if(pLabel)pLabel->SetLabel(strNewContent);
				index++;
			}
		}
	}
	wxWindow* pButton=FindWindowById(wxID_OK,this);
	if(pButton) pButton->SetLabel(LOADSTRING(ID_QUERY_ACCOUNTBTN));

	wxWindow* pStatic = FindWindowById(ID_SELECT_ACCOUNT, this);
	if(pStatic) pStatic->SetLabel(LOADSTRING(ID_SELECT_ACCOUNT));
}
#endif

void FundAccountPanel::DoInitQryStep()
{
    vector<string> accountList;
    accountList.clear();
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) 
    {
        if(DEFAULT_SVR()&&DEFAULT_SVR()->GetAccountList(accountList)>0) 
        {
            InitAccountCtrl(accountList);
        }
    }
}

void FundAccountPanel::InitAccountCtrl(const vector<string>& vAccount)
{
	wxComboBox* pCombox = (wxComboBox*)(FindWindowById(ID_CBX_ACCOUNT, this));
	if ( NULL == pCombox || vAccount.empty())
	{
		return;
	}

	pCombox->Clear();
	for ( size_t i = 0; i < vAccount.size(); i++)
	{
		pCombox->AppendString( vAccount.at(i) );
	}

	pCombox->SetSelection(0);
	m_strCurAccount = vAccount.at(0);

	RefreshUI();
}

void FundAccountPanel::OnAccountChangeSel( wxCommandEvent& event )
{
	wxComboBox* pCombox = (wxComboBox*)(FindWindowById(ID_CBX_ACCOUNT, this));
	if ( NULL == pCombox )
	{
		return;
	}

	m_strCurAccount = pCombox->GetValue().c_str();

	RefreshUI();
}

