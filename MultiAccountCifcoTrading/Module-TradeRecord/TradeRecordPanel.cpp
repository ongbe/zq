#include "Stdafx.h"
#include "TradeRecordPanel.h"
#include <wx/gbsizer.h>
#include "../inc/Module-Misc/FieldValueTool.h"
#include "cfgMgr/CfgMgr.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "wx/file.h"
#include "../inc/gui/MainFrame.h"

void TRACE(const char* format,...);

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
static TradeRecordPanel* pTradeRecordPanel = NULL;

BEGIN_EVENT_TABLE(TradeRecordPanel,wxPanel)
EVT_COMMAND(wxID_ANY, wxEVT_RtnTradeRecord,  TradeRecordPanel::OnRcvTrade)
EVT_COMMAND(wxID_ANY, wxEVT_RspQryTradeLast,  TradeRecordPanel::OnRspQryTrade)
EVT_COMMAND(wxID_ANY, wxEVT_RspQryCommissionRate,  TradeRecordPanel::OnGetCommissionRate)

EVT_MENU_RANGE(ID_MENU_COLUMNCONFIG,ID_MENU_EXPORT,TradeRecordPanel::ContextMenuRespond)
EVT_MENU_RANGE(ID_MENU_ADDTOMYSELF,ID_MENU_ADDTOMYSELF+50,TradeRecordPanel::ContextMenuRespond)
EVT_MENU(ID_MENU_AUTO_ADJUST_COL_WIDTH, TradeRecordPanel::ContextMenuRespond)
EVT_LIST_EXT_CONTEXT_MENU(OnConTextMenu)
EVT_CONTEXT_MENU(OnConTextMenu)

EVT_SIZE( TradeRecordPanel::OnSize )
EVT_BUTTON(ID_BUTTON_REQRY,TradeRecordPanel::OnReQry)
EVT_COMMAND(wxID_ANY,wxEVT_REQ_REQRY,TradeRecordPanel::OnReConnectQry)
EVT_CFG_CHANGE(OnCfgChanged)
EVT_RADIOBUTTON(ID_RADIOBTDetail,OnRadioButton)
EVT_RADIOBUTTON(ID_RADIOBTTotal,OnRadioButton)
EVT_SUBSCRIBE(TradeRecordPanel::OnSubscrible)
EVT_LIST_EXT_COL_END_DRAG(ID_ListDetail, OnDetailColumnResize)
EVT_LIST_EXT_COL_END_DRAG(ID_ListTotal, OnTotalColumnResize)
#ifdef _USE_MULTI_LANGUAGE
EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
#endif
EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()

#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("TradeRecordPanel", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

//----------下面是订阅事件通知----------

void TradeRecordPanel::OnSubscrible(wxCommandEvent& evt)
{
	set<IPlatformSingleSvr*>::iterator it;
	if(evt.GetInt())
	{
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			(*it)->SubscribeBusinessData(BID_RspQryTrade, GID_THRADE_RECORD, RspQryTradeCallBackFunc, true);
			(*it)->SubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_THRADE_RECORD, GetCommissionRateCallBackFunc);
			(*it)->SubscribeBusinessData(BID_RtnTrade, GID_THRADE_RECORD, RtnTradeCallBackFunc, true);
		}
	}
	else
	{
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			(*it)->UnSubscribeBusinessData(BID_RspQryTrade, GID_THRADE_RECORD);
			(*it)->UnSubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_THRADE_RECORD);
			(*it)->UnSubscribeBusinessData(BID_RtnTrade, GID_THRADE_RECORD);
		}
	}
}


//----------下面是事件回调函数----------

//成交查询回报
int TradeRecordPanel::RspQryTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryTrade || data.Size!=sizeof(DataRspQryTrade) || !pTradeRecordPanel)
		return 0;	

	DataRspQryTrade& refData = *(DataRspQryTrade*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.bIsLast)
	{
        wxCommandEvent evt(wxEVT_RspQryTradeLast, wxID_ANY);
        pTradeRecordPanel->AddPendingEvent(evt);
	}
	return 0;
}


int TradeRecordPanel::GetCommissionRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  //手续费率回报
{ 
	if(data.BID!=BID_RspQryInstrumentCommissionRate || data.Size!=sizeof(DataRspQryInstrumentCommissionRate) || !pTradeRecordPanel)
		return 0;	

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInstrumentCommissionRate),0);

    wxCommandEvent evt(wxEVT_RspQryCommissionRate, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    pTradeRecordPanel->AddPendingEvent(evt);
	return 0;
}


//成交记录回报
int TradeRecordPanel::RtnTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade) || !pTradeRecordPanel)
		return 0;

    DWORD EvtParamID;
    CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnTrade),0);

    wxCommandEvent evt(wxEVT_RtnTradeRecord, wxID_ANY);
    evt.SetInt((int)EvtParamID);
    pTradeRecordPanel->AddPendingEvent(evt);

	return 0;
}


//----------下面是内部事件响应处理函数，在主线程中执行----------

void TradeRecordPanel::OnRspQryTrade(wxCommandEvent& evt)
{
    ShowAll();
}

//获得某些合约的保证金率，需要更新相应的报单的冻结数值
void TradeRecordPanel::OnGetCommissionRate(wxCommandEvent& evt)
{
    DataRspQryInstrumentCommissionRate CommissionRate;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&CommissionRate,NULL,sizeof(CommissionRate),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
    if(CommissionRate.RspInfoField.ErrorID!=0)
        return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(CommissionRate.Head.pPlatformSvr);

    if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end())
        return;

    std::set<std::string> setInstruments;
    std::set<std::string>::iterator it;
    std::vector<PlatformStru_TradeTotalInfo> vecTradeTotal;
    std::vector<PlatformStru_TradeInfo> vecTrade;
	int iret;

    //有时候是合约ID,有时候是品种代码
    std::string strInstru(CommissionRate.InstrumentCommissionRateField.InstrumentID);
    if(strInstru.empty())
        return;

	iret=pSvr->GetInstrumentListByProductID(strInstru,setInstruments);

	if(iret>0)
	{//是品种代码
        for(it=setInstruments.begin();it!=setInstruments.end();it++)
        {
            if(pSvr->GetTradesOfInstrument(*it,vecTrade)>0)
                if(m_pListDetail&&!vecTrade.empty()) m_pListDetail->UpdateInstrumentItems3(*it,vecTrade);
            if(pSvr->GetTradeTotalOfInstrument(*it,vecTradeTotal)>0)
                if(m_pListTotal&&!vecTradeTotal.empty()) m_pListTotal->UpdateInstrumentItems3(*it,vecTradeTotal);
        }
	}
	else
	{//是合约ID
        if(pSvr->GetTradesOfInstrument(strInstru,vecTrade)>0)
            if(m_pListDetail&&!vecTrade.empty()) m_pListDetail->UpdateInstrumentItems3(strInstru,vecTrade);
        if(pSvr->GetTradeTotalOfInstrument(strInstru,vecTradeTotal)>0)
            if(m_pListTotal&&!vecTradeTotal.empty()) m_pListTotal->UpdateInstrumentItems3(strInstru,vecTradeTotal);
	}
}


void TradeRecordPanel::OnRcvTrade(wxCommandEvent& evt)
{
    DataRtnTrade RtnTrade;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnTrade,NULL,sizeof(RtnTrade),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RtnTrade.Head.pPlatformSvr);

	if(!pSvr||m_pSvrSet.find(pSvr)==m_pSvrSet.end())
		return;
    PlatformStru_TradeInfo& rawData= RtnTrade.TradeField;
    PlatformStru_TradeInfo TradeInfo;

    TradeKey tmpTradeKey(rawData);
    bool bGetTradeInfo=false;
    if(pSvr->GetTradeInfo(tmpTradeKey,TradeInfo))
    {
        if(m_pListDetail) m_pListDetail->UpdateOneItem(TradeInfo);
        bGetTradeInfo=true;
    }

    LOG_INFO("TradeRecordPanel::OnRcvTrade Instru=%s bGetTradeInfo=%d TradeID=%s",tmpTradeKey.InstrumentID,bGetTradeInfo,TradeInfo.TradeID);

    std::string strInstrument(rawData.InstrumentID);
    std::vector<PlatformStru_TradeTotalInfo> vecTradeTotal;

    if(pSvr->GetTradeTotalOfInstrument(strInstrument,vecTradeTotal)>0)
        if(m_pListTotal) m_pListTotal->UpdateInstrumentItems3(strInstrument,vecTradeTotal);
}





TradeRecordPanel::TradeRecordPanel(wxWindow *parent,
								   wxWindowID winid ,
								   const wxPoint& pos ,
								   const wxSize& size ,
								   long style,
								   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name),
m_pBTDetail(NULL),
m_pBTTotal(NULL),
m_pButtonReQry(NULL),
m_pListDetail(NULL),
m_pListTotal(NULL)
{
    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"TradeRecordPanel.log");

	SetBackgroundColour(DEFAULT_COLOUR);
	m_id = 0;
	//考虑只支持GID_THRADE_RECORD和GID_THRADE_STATISTICS中的一个模块的显示问题
	if (DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_THRADE_RECORD ) )
        m_pListDetail= new CDataListCtrl<TradeKey,PlatformStru_TradeInfo>(UpdateListItemCallBackDetail, this, ID_ListDetail, wxDefaultPosition, wxDefaultSize, wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT);
	if (DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_THRADE_STATISTICS ))
	    m_pListTotal= new CDataListCtrl<TradeTotalKey,PlatformStru_TradeTotalInfo>(UpdateListItemCallBackTotal, this, ID_ListTotal, wxDefaultPosition, wxDefaultSize, wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT);
	if(m_pListDetail&&m_pListTotal)
	{
		m_pBTDetail = new wxRadioButton( this, ID_RADIOBTDetail, LOADSTRING(DETAIL_TEXT),wxPoint(), wxSize(60, CommonBtnHeight));
		m_pBTTotal = new wxRadioButton( this, ID_RADIOBTTotal, LOADSTRING(SUM_TEXT),wxPoint(), wxSize(60, CommonBtnHeight));
	}
	if(m_pListDetail)
    {
	    if(m_pBTDetail)
			m_pBTDetail->SetValue(true);
        m_pListDetail->Show(true);
		if(m_pListTotal)
		{
  			m_pBTTotal->SetValue(false);
			m_pListTotal->Show(false);  
		}
    }
	else if(m_pListTotal)
	{
		m_pBTTotal->SetValue(true);
		m_pListTotal->Show(true);  
	}
    m_pButtonReQry = new wxButton(this, ID_BUTTON_REQRY, LOADSTRING(BUTTON_REQRY_CAPTION), wxPoint(), wxSize(80,CommonBtnHeight));


	pTradeRecordPanel = this ;

    Init();

}

TradeRecordPanel::~TradeRecordPanel()
{
    if(m_pListDetail) { delete m_pListDetail;  m_pListDetail=NULL; }
    if(m_pListTotal) { delete m_pListTotal;  m_pListTotal=NULL; }
    if(m_pBTDetail) { delete m_pBTDetail;  m_pBTDetail=NULL; }
    if(m_pBTTotal) { delete m_pBTTotal;  m_pBTTotal=NULL; }
    if(m_pButtonReQry) { delete m_pButtonReQry;  m_pButtonReQry=NULL; }

    pTradeRecordPanel = NULL;

    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }
}

void TradeRecordPanel::ShowAll()
{
    std::vector<PlatformStru_TradeInfo> aAllTrades,AllTrades;
    std::vector<PlatformStru_TradeTotalInfo> aAllTradeTotals,AllTradeTotals;
	set<IPlatformSingleSvr*>::iterator it;
	for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
	{
		if((*it)->GetAllTrades(AllTrades)>=0) 
			aAllTrades.insert(aAllTrades.end(),AllTrades.begin(),AllTrades.end());
		if((*it)->GetAllTradeTotals(AllTradeTotals)>=0)
			aAllTradeTotals.insert(aAllTradeTotals.end(),AllTradeTotals.begin(),AllTradeTotals.end());
	}
    if(m_pListDetail) 
		m_pListDetail->UpdateAllItems(aAllTrades);
    if(m_pListTotal) 
		m_pListTotal->UpdateAllItems(aAllTradeTotals);
}


void TradeRecordPanel::OnReQry(wxCommandEvent& evt)
{
	PlatformStru_QryTrade QryTrade;
	memset(&QryTrade, 0, sizeof(QryTrade));
	set<IPlatformSingleSvr*>::iterator it;
	for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
	{
		(*it)->ReqQryTrade(QryTrade);
	}
}
void TradeRecordPanel::OnReConnectQry(wxCommandEvent& evt)
{
	if(evt.GetInt()==1)//获取需要刷新的总数
	{
		std::vector<PlatformStru_TradeInfo> AllTrades;
		int count=0;
		set<IPlatformSingleSvr*>::iterator it;
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			(*it)->GetAllTrades(AllTrades);
			count+=AllTrades.size();
		}
		evt.SetExtraLong(count);
	}
	else
	{
		OnReQry(evt);
	}
}
void TradeRecordPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
    int Pox,Poy;

	if(m_pListDetail)
		m_pListDetail->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);
	if(m_pListTotal)
		m_pListTotal->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);



    Pox=8;
    Poy=rt.height-2-CommonBtnHeight;

    if(m_pBTDetail)
    {
        m_pBTDetail->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pBTDetail->GetRect().GetWidth()+8;
    }

    if(m_pBTTotal)
    {
        m_pBTTotal->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pBTTotal->GetRect().GetWidth()+8;
    }

    Pox+=8;

    if(m_pButtonReQry)
    {
        m_pButtonReQry->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonReQry->GetRect().GetWidth()+8;
    }
}

void TradeRecordPanel::OnRadioButton(wxCommandEvent& event)
{
	int id=event.GetId();

	switch(id)
	{
	case ID_RADIOBTDetail:
		if(m_pListTotal) m_pListTotal->Show(false);
		if(m_pListDetail) m_pListDetail->Show(true);
		m_id = 0;
		break;

	case ID_RADIOBTTotal:
		if(m_pListDetail) m_pListDetail->Show(false);
		if(m_pListTotal) m_pListTotal->Show(true);
		m_id = 1;
		break;

	default:
		break;
	}
}
//配置文件改变响应函数
void TradeRecordPanel::OnCfgChanged(wxCommandEvent& evt)
{
	WXLOG_INFO("TradeRecordPanel::OnCfgChanged");

    Init();
}


void TradeRecordPanel::Init()
{
	if(m_pListDetail) 
    {
        m_pListDetail->Clear();
	    if(m_pListDetail->GetColumnCount()>0) m_pListDetail->DeleteAllColumns();
    }

	if(m_pListTotal) 
    {
        m_pListTotal->Clear();
	    if(m_pListTotal->GetColumnCount()>0) m_pListTotal->DeleteAllColumns();
    }

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
    if(!pCfgMgr) return;

    int ColNum;
	LPLIST_COLUMN_CFG Colcfg;
    LPLIST_TABLE_CFG cfg;


    if(m_pListDetail)
    {
        cfg = pCfgMgr->GetListCfg( GID_THRADE_RECORD );
	    if( cfg != NULL)
	    { 
		    m_pListDetail->SetHeaderBackgroundColour( wxColor(cfg->HeadBackgroundColor) ) ;
		    m_pListDetail->SetHeaderForegroundColour( wxColor(cfg->HeadColor));
		    wxFont font;
		    font.SetNativeFontInfoUserDesc(cfg->szFont); 
		    m_pListDetail->SetFont( font );
		    m_pListDetail->SetBackgroundColour( wxColor(cfg->BackgroundColor));
		    m_pListDetail->SetForegroundColour( wxColor(cfg->TextColor) );
		    m_pListDetail->SetOddRowColour(wxColor(cfg->OddLineBackgroundColor));
		    m_pListDetail->SetEvenRowColour(wxColor(cfg->EvenLineBackgroundColor));
            m_TextColorDetail=cfg->TextColor;
	    }

	    ColNum = pCfgMgr->GetListVisibleColCount(GID_THRADE_RECORD);
        m_FieldID2ColumnIDDetail.clear();
	    for( int i=0; i< ColNum; i++)
	    {
		    Colcfg  = pCfgMgr->GetListVisibleColCfg(GID_THRADE_RECORD, i);
            if(!Colcfg) continue;

		    wxExtListItem col;
		    //"成交ID"栏，左对齐
		    if(Colcfg->id == TRADE_RECORD_FIELD_ID_TRADENO)   
			    col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		    else if(Colcfg->id == TRADE_RECORD_FIELD_ID_INSTRUMENTID)
		    {    //"合约ID"栏，左对齐                                               
			    col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		    }
		    else if(Colcfg->id == TRADE_RECORD_FIELD_ID_ORDERNO)
		    {
			    col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		    }
		    else //其余的，右对齐                                               
			    col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

		    col.SetText(pCfgMgr->GetColCfgCaption(GID_THRADE_RECORD,Colcfg->id));
		    col.SetWidth( Colcfg->Width);
		    col.SetTextColour( Colcfg->TextColor);
            col.SetColData(Colcfg->id);

		    m_pListDetail->InsertColumn(i, col); 

            m_FieldID2ColumnIDDetail[Colcfg->id] = i;
	    }

        m_pListDetail->SetCfg(m_TextColorDetail,m_FieldID2ColumnIDDetail);
    }

    if(m_pListTotal) 
    {
	    cfg = pCfgMgr->GetListCfg( GID_THRADE_STATISTICS );
	    if( cfg!=NULL )
	    {
		    m_pListTotal->SetHeaderBackgroundColour( wxColor(cfg->HeadBackgroundColor) ) ;
		    m_pListTotal->SetHeaderForegroundColour( wxColor(cfg->HeadColor));
		    wxFont font;
		    font.SetNativeFontInfoUserDesc(cfg->szFont); 
		    m_pListTotal->SetFont( font );
		    m_pListTotal->SetBackgroundColour( wxColor(cfg->BackgroundColor));
		    m_pListTotal->SetForegroundColour( wxColor(cfg->TextColor) );
		    m_pListTotal->SetOddRowColour(wxColor(cfg->OddLineBackgroundColor));
		    m_pListTotal->SetEvenRowColour(wxColor(cfg->EvenLineBackgroundColor));
            m_TextColorTotal=cfg->TextColor;
	    }

	    ColNum = pCfgMgr->GetListVisibleColCount(GID_THRADE_STATISTICS);
        m_FieldID2ColumnIDTotal.clear();
	    for( int i=0; i<ColNum; i++)
	    {
		    Colcfg = pCfgMgr->GetListVisibleColCfg(GID_THRADE_STATISTICS, i);
		    if(!Colcfg) continue;

		    wxExtListItem col;
		    if(Colcfg->id == TRADE_RECORD_FIELD_ID_INSTRUMENTID)
		    {    //"合约ID"栏，左对齐                                               
			    col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		    }
		    else //其余的，右对齐                                               
			    col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

            col.SetText(pCfgMgr->GetColCfgCaption(GID_THRADE_STATISTICS,Colcfg->id));
		    col.SetWidth( Colcfg->Width); 
		    col.SetTextColour( Colcfg->TextColor);
		    col.SetColData(Colcfg->id);
            col.SetColData(Colcfg->id);

		    m_pListTotal->InsertColumn(i, col); 

            m_FieldID2ColumnIDTotal[Colcfg->id] = i;
	    }

        m_pListTotal->SetCfg(m_TextColorTotal,m_FieldID2ColumnIDTotal);
    }


    ShowAll();
}


void TradeRecordPanel::OnConTextMenu(wxContextMenuEvent& event)
{
	wxMenu menu;
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	BOOL bIsRowCLick = event.GetId();
	if ( bIsRowCLick )
	{
		menu.AppendSeparator();
		INSERT_ADDTOMYSELF_ITEM(menu);
	}

	if(menu.GetMenuItemCount()>0)
	{
		POINT pt;
		::GetCursorPos(&pt);
		wxPoint point(pt.x,pt.y);
		point = ScreenToClient(point);
		PopupMenu(&menu,point.x,point.y);
	}
	event.Skip();
}
void TradeRecordPanel::ContextMenuRespond(wxCommandEvent& event)
{
	int id = event.GetId();
	//加入自选
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if( id - ID_MENU_ADDTOMYSELF >= 0 )
	{
		wxExtListCtrl* pListCtrl = NULL;
		if ( m_id == 0 ) pListCtrl = m_pListDetail;
		else pListCtrl = m_pListTotal;

        if(!pListCtrl) 
            return;


		bool bAdded = false;
        PlatformStru_TradeInfo TradeInfo;
        PlatformStru_TradeTotalInfo TradeTotalInfo;
		for ( int i = 0; i < pListCtrl->GetItemCount(); i++)
		{
			if ( pListCtrl->IsSelected( i ))
			{
				string InstrumentId;
				if ( m_id == 0 )
				{
                    if(m_pListDetail && m_pListDetail->GetValueByRow(i,TradeInfo))
                    {
                        InstrumentId=std::string(TradeInfo.InstrumentID);
                    }
				}
				else
				{
                    if(m_pListTotal && m_pListTotal->GetValueByRow(i,TradeTotalInfo))
                    {
                        InstrumentId=std::string(TradeTotalInfo.InstrumentID);
                    }
				}
                if(InstrumentId.empty()) continue;

				if(pCfgMgr->GetInstrumentGroupMemberCount(id - ID_MENU_ADDTOMYSELF)>=DEFAULT_SVR()->GetGroupMaxContractNum())
				{
					wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
					break;
				}
				else
				{
					pCfgMgr->AddInstrument( id - ID_MENU_ADDTOMYSELF, InstrumentId.c_str()); 
					bAdded = true;
				}
			}
		}

		if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);}
		return;
	}

	switch(event.GetId())
	{
	case ID_MENU_AUTO_ADJUST_COL_WIDTH:
		{
			if( m_id==0 )
			{
				if( NULL != m_pListDetail)
				{
					m_pListDetail->AutoAdjustColumnWidth();
				}
			}
			else
			{
				if( NULL != m_pListTotal)
				{
					m_pListTotal->AutoAdjustColumnWidth();
				}
			}

			SaveColWidth();
		}
		break;
	case ID_MENU_COLUMNCONFIG:
		SEND_LOAD_CONFIG_APP_EVENT(m_id==0?ID_ITEM_TRADERECORD:ID_ITEM_TRADESTATISTIC);
		break;
	case ID_MENU_EXPORT:
		{
			wxExtListCtrl* p = NULL;
            wxString cap;
			if( m_id==0 )
            {
				p = m_pListDetail;
                cap=LOADSTRING(DETAIL_TEXT);
            }
			else
            {
				p= m_pListTotal;
                cap=LOADSTRING(SUM_TEXT);
            }
            if(!p) break;
			LIST_EXPORT_CSV_FILE(cap, p);
		}
		break;
	default:
		break;
	}
}
#ifdef _USE_MULTI_LANGUAGE
void TradeRecordPanel::OnLanguageChanged( wxCommandEvent& event )
{
	if(m_pListDetail)
		TRANSFORM_COLUMN_TEXT(GID_THRADE_RECORD,m_pListDetail);
	if(m_pListTotal)
		TRANSFORM_COLUMN_TEXT(GID_THRADE_STATISTICS,m_pListTotal);
	if(FindWindowById(ID_RADIOBTDetail, this))
		FindWindowById(ID_RADIOBTDetail, this)->SetLabel(LOADSTRING(DETAIL_TEXT));
	if(FindWindowById(ID_RADIOBTTotal, this))
		FindWindowById(ID_RADIOBTTotal, this)->SetLabel(LOADSTRING(SUM_TEXT));
	if(FindWindowById(ID_BUTTON_REQRY, this))
		FindWindowById(ID_BUTTON_REQRY, this)->SetLabel(LOADSTRING(BUTTON_REQRY_CAPTION));
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	
	int nRowCount = 0;
	if(m_pListDetail)
	{
	nRowCount = m_pListDetail->GetItemCount();
	for ( int nRow = 0; nRow < nRowCount; nRow++ )
	{
		//重新设置买卖方向，投保列
		for(int i = 0; i < pCfgMgr->GetListVisibleColCount(GID_THRADE_RECORD); i++)
		{
			bool bReplace=true;
			string strValue=m_pListDetail->GetSubItemText( nRow, i);
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_THRADE_RECORD, i);
			//语言替换
			if ( pColCfg->id == TRADE_RECORD_FIELD_ID_DIRECTION)
			{
				strValue=CFieldValueTool::Direction2String(CFieldValueTool::String2Direction(strValue),event.GetInt());
			}
			else if(pColCfg->id == TRADE_RECORD_FIELD_ID_OPENCLOSE )
			{
				strValue=CFieldValueTool::OffsetFlag2String(CFieldValueTool::String2OffsetFlag(strValue),event.GetInt());
			}
			else if (pColCfg->id == TRADE_RECORD_FIELD_ID_TRADETYPE)
			{
				strValue=CFieldValueTool::TradeType2String(CFieldValueTool::String2TradeType(strValue),event.GetInt());
			}
			else
			{
				bReplace=false;
			}
			if(bReplace) m_pListDetail->SetItem( nRow, i, strValue );
		}
	}
	}
	if(m_pListTotal)
	{
	nRowCount = m_pListTotal->GetItemCount();
	for ( int nRow = 0; nRow < nRowCount; nRow++ )
	{
		//重新设置买卖方向，投保列
		for(int i = 0; i < pCfgMgr->GetListVisibleColCount(GID_THRADE_STATISTICS); i++)
		{
			bool bReplace=true;
			string strValue=m_pListTotal->GetSubItemText( nRow, i);
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_THRADE_STATISTICS, i);
			//语言替换
			if ( pColCfg->id == TRADE_RECORD_FIELD_ID_DIRECTION)
			{
				strValue=CFieldValueTool::Direction2String(CFieldValueTool::String2Direction(strValue),event.GetInt());
			}
			else if(pColCfg->id == TRADE_RECORD_FIELD_ID_OPENCLOSE )
			{
				strValue=CFieldValueTool::OffsetFlag2String(CFieldValueTool::String2OffsetFlag(strValue),event.GetInt());
			}
			else
			{
				bReplace=false;
			}
			if(bReplace) m_pListTotal->SetItem( nRow, i, strValue );
		}
	}
	}
}
#endif
void TradeRecordPanel::OnDetailColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_THRADE_RECORD ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_THRADE_RECORD, nCol);
    pColCfg->Width = m_pListDetail ? m_pListDetail->GetColumnWidth( nCol ) : 0;
}

void TradeRecordPanel::OnTotalColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_THRADE_STATISTICS ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_THRADE_STATISTICS, nCol);
    pColCfg->Width = m_pListTotal? m_pListTotal->GetColumnWidth( nCol ) : 0;
}



#define UpdateCol_String(FieldName)  \
            if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
                return false;\
            outItem.SetText(wxString(pNew->FieldName));
#define UpdateCol_String3(FieldName,DispName)  \
            if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
                return false;\
            outItem.SetText(wxString(DispName));
#define UpdateCol_Number(FieldName,DispName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName)\
                return false;\
            outItem.SetText(wxString(DispName));\
            outItem.SetiColData((int)(pNew->FieldName));
#define UpdateCol_Price(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetIntInvalidValue()) outItem.SetText("-");\
            else outItem.SetText(Price2String(pNew->FieldName,pNew->PriceTick));\
            outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Int(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetIntInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%d",pNew->FieldName);\
            outItem.SetText(tmpstr);\
            outItem.SetiColData(pNew->FieldName);
#define UpdateCol_Money(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%.02lf", pNew->FieldName ); \
            outItem.SetText(tmpstr);\
            outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Fund(FieldName)  \
			if(!bnewline&&pOld->FieldName==pNew->FieldName) \
				return false;\
			if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
			else tmpstr = GlobalFunc::GetAccountFormatString(pNew->FieldName, 2); \
			outItem.SetText(tmpstr);\
            outItem.SetdColData(pNew->FieldName);

bool TradeRecordPanel::UpdateListItemCallBackDetail(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_TradeInfo* pNew=(const PlatformStru_TradeInfo*)pNewData;
    PlatformStru_TradeInfo* pOld=(PlatformStru_TradeInfo*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    outItem.SetTextColour(wxColor(textcol));

    switch(FieldID)
    {
        case TRADE_RECORD_FIELD_ID_INSTRUMENTID:
            UpdateCol_String(InstrumentID);
            return true;
        case TRADE_RECORD_FIELD_ID_EXCHANGE:
            UpdateCol_String3(ExchangeID,CFieldValueTool::ExchangeID2Name((char*)pNew->ExchangeID));
            return true;
        case TRADE_RECORD_FIELD_ID_TRADENO:
            UpdateCol_String3(TradeID,wxString(pNew->TradeID).Trim(true).Trim(false));
            outItem.SetbtrynumericAsComparing(true);
            return true;
        case TRADE_RECORD_FIELD_ID_ORDERNO:
            UpdateCol_String3(OrderSysID,wxString(pNew->OrderSysID).Trim(true).Trim(false));
            outItem.SetbtrynumericAsComparing(true);
            return true;
        case TRADE_RECORD_FIELD_ID_DIRECTION:
            UpdateCol_Number(Direction,CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
            if ( pNew->Direction == THOST_FTDC_D_Buy )
                outItem.SetTextColour(wxColor(RGB(255,0,0)));//红色
            else if ( pNew->Direction == THOST_FTDC_D_Sell )
                outItem.SetTextColour(wxColor(RGB(0,128,0)));//绿色
            return true;
        case TRADE_RECORD_FIELD_ID_OPENCLOSE:
            UpdateCol_Number(OffsetFlag,CFieldValueTool::OffsetFlag2String(pNew->OffsetFlag,SVR_LANGUAGE));
            return true;
        case TRADE_RECORD_FIELD_ID_TRADEPRICE:
            UpdateCol_Price(Price);
            return true;
        case TRADE_RECORD_FIELD_ID_TRADEHAND:
            UpdateCol_Int(Volume);
            return true;
        case TRADE_RECORD_FIELD_ID_CHARGE:
            UpdateCol_Fund(TradeCommission);
            return true;
        case TRADE_RECORD_FIELD_ID_TRADETIME:
            if(!bnewline&&
                memcmp(pOld->TradeDate,pNew->TradeDate,sizeof(pOld->TradeDate))==0&&
                memcmp(pOld->TradeTime,pNew->TradeTime,sizeof(pOld->TradeTime))==0) 
                return false;
            if(memcmp(pNew->TradeDate,pNew->TradingDay,sizeof(pNew->TradeDate))==0)
                tmpstr.Printf("%s", pNew->TradeTime);
            else
                tmpstr.Printf("%s %s", pNew->TradeDate,pNew->TradeTime);
            outItem.SetText(tmpstr);
            return true;
        case TRADE_RECORD_FIELD_ID_TRADETYPE:
            UpdateCol_Number(TradeType,CFieldValueTool::TradeType2String(pNew->TradeType,SVR_LANGUAGE));
            return true;
        case TRADE_RECORD_FIELD_ID_INSURE:
            UpdateCol_Number(HedgeFlag,CFieldValueTool::HedgeFlag2String(pNew->HedgeFlag,SVR_LANGUAGE));
            return true;
        case TRADE_RECORD_FIELD_ID_ACCOUNT:
            UpdateCol_String(szAccount);
            return true;
    }
    return false;
}


bool TradeRecordPanel::UpdateListItemCallBackTotal(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_TradeTotalInfo* pNew=(const PlatformStru_TradeTotalInfo*)pNewData;
    PlatformStru_TradeTotalInfo* pOld=(PlatformStru_TradeTotalInfo*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    outItem.SetTextColour(wxColor(textcol));

    switch(FieldID)
    {
        case TRADE_RECORD_FIELD_ID_INSTRUMENTID:
            UpdateCol_String(InstrumentID);
            return true;
        case TRADE_RECORD_FIELD_ID_EXCHANGE:
            UpdateCol_String3(ExchangeID,CFieldValueTool::ExchangeID2Name((char*)pNew->ExchangeID));
            return true;
		case TRADE_RECORD_FIELD_ID_DIRECTION:
            UpdateCol_Number(Direction,CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
            if ( pNew->Direction == THOST_FTDC_D_Buy )
                outItem.SetTextColour(wxColor(RGB(255,0,0)));//红色
            else if ( pNew->Direction == THOST_FTDC_D_Sell )
                outItem.SetTextColour(wxColor(RGB(0,128,0)));//绿色
			return true;
		case TRADE_RECORD_FIELD_ID_OPENCLOSE:
            UpdateCol_Number(OffsetFlag,CFieldValueTool::OffsetFlag2String(pNew->OffsetFlag,SVR_LANGUAGE));
			return true;
        case TRADE_RECORD_FIELD_ID_TRADEAVGPRICE:
            UpdateCol_Money(price);
            return true;
        case TRADE_RECORD_FIELD_ID_TRADEHAND:
            UpdateCol_Int(volume);
            return true;
        case TRADE_RECORD_FIELD_ID_CHARGE:
            UpdateCol_Fund(TradeCommission);
            return true;
    }
    return false;
}

void TradeRecordPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	wxExtListCtrl* pCtrl = NULL;
	int nGid = 0;
	if ( m_id == 0 )
	{
		pCtrl = m_pListDetail;
		nGid = GID_THRADE_RECORD;
	}
	else
	{
		pCtrl = m_pListTotal;
		nGid = GID_THRADE_STATISTICS;
	}

	if ( NULL == pCtrl )
	{
		return;
	}

	int nColCount = pCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( nGid ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(nGid, i);
		pColCfg->Width = pCtrl->GetColumnWidth( i );
	}

}
void TradeRecordPanel::OnMAPlatformAdd(wxCommandEvent& event)
{
	m_pSvrSet.insert((IPlatformSingleSvr*)event.GetInt());
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
	if(m_pListDetail) 
		m_pListDetail->Clear();
	if(m_pListTotal) 
		m_pListTotal->Clear();
	ShowAll();

}
void TradeRecordPanel::OnMAPlatformDelete(wxCommandEvent& event)
{
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(0);
	ProcessEvent(myEvent);
	m_pSvrSet.erase((IPlatformSingleSvr*)event.GetInt());
	myEvent.SetInt(1);
	ProcessEvent(myEvent);

	if(m_pListDetail) 
		m_pListDetail->Clear();
	if(m_pListTotal) 
		m_pListTotal->Clear();
	ShowAll();

}



