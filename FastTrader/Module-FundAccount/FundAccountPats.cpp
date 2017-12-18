#include "StdAfx.h"
#include "FundAccountPats.h"
#include "FundAccountQryDlg.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-TradeRecord/TradeRecordPanel.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "gui-common/FrameBase.h"
#include "wx/file.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
FundAccountPats* m_FundAccountPats=NULL;
#define GETFUNDSTR(str,t)\
	do{\
	if(util::isInvalidValue(t))\
	str.Printf("-");	\
	else\
	str = GlobalFunc::GetAccountFormatString(t, 2).c_str();\
	}while(0)

FundAccountPats * FundAccountPats::CreatePane(wxWindow* parent)
{
	if ( m_FundAccountPats == NULL )
	{
		return  new FundAccountPats( parent, GID_ACCOUNT );
	}
	else
	{
		if ( parent != m_FundAccountPats->GetParent())
		{
			m_FundAccountPats->Reparent(parent);
		}
		
		return m_FundAccountPats;
	}
}
BEGIN_EVENT_TABLE(FundAccountPats, wxPanel)
	EVT_SIZE( FundAccountPats::OnSize )
	EVT_SUBSCRIBE(FundAccountPats::OnSubscrible)
	EVT_COMMAND(wxID_ANY,wxEVT_CALC_FUNDACCOUNT,FundAccountPats::OnAllCallBack)
#ifdef _USE_MULTI_LANGUAGE
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE,  FundAccountPats::OnLanguageChange)
#endif
	EVT_MENU(ID_MENU_COLUMNCONFIG,FundAccountPats::OnConfig)
	EVT_MENU(ID_MENU_EXPORT,FundAccountPats::OnMenuExport)
	EVT_MENU(ID_MENU_AUTO_ADJUST_COL_WIDTH,FundAccountPats::OnAutoAdjustColWidth)
	EVT_LIST_EXT_CONTEXT_MENU(FundAccountPats::OnContext)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_EXCHANGERATE,FundAccountPats::OnExchangeRate)
	EVT_COMBOBOX(ID_CBX_CURRENCY, FundAccountPats::OnCurrencyChangeSel)
    EVT_BUTTON(wxID_OK,FundAccountPats::QryTradingAccount)
	EVT_BUTTON(ID_BUTTON_REQRY,FundAccountPats::OnReQry)
	EVT_COMMAND(wxID_ANY,wxEVT_REQ_REQRY,FundAccountPats::OnReQry)
END_EVENT_TABLE()
FundAccountPats::FundAccountPats(wxWindow *parent,
                                   wxWindowID winid ,
                                   const wxPoint& pos ,
                                   const wxSize& size ,
                                   long style,
                                   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name),
m_list(NULL),
m_curExchangeRate(1.0),
m_pFundAccountQryDlg(NULL)
{
	m_FundAccountPats=this;
    SetBackgroundColour(DEFAULT_COLOUR);
	InitComboBox();
	m_list=new wxExtListCtrl(this,wxID_ANY,wxDefaultPosition, wxDefaultSize, wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT); 
    InitListCtrl();
	new wxButton(this, ID_BUTTON_REQRY, LOADSTRING(BUTTON_REQRY_CAPTION), wxPoint(), wxSize(80,CommonBtnHeight));

}
int FundAccountPats::ExchangeRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_ExchangeRate || data.Size!=sizeof(DataExchangeRate))
		return 0;	
	DataExchangeRate* pTmp=new DataExchangeRate();
	memcpy(pTmp, &data, sizeof(DataExchangeRate));
	wxCommandEvent evt(wxEVT_EXCHANGERATE);
	evt.SetClientData(pTmp);
	if(m_FundAccountPats)
		m_FundAccountPats->AddPendingEvent(evt);
	return 0;
}
void FundAccountPats::OnExchangeRate(wxCommandEvent& event)
{
	DataExchangeRate* pTmp= (DataExchangeRate*)(event.GetClientData());
	if(!pTmp) return;
	if(DEFAULT_SVR())
	{
		wxComboBox* pCombox =(wxComboBox*)FindWindowById(ID_CBX_CURRENCY, this);
		if(pCombox)
		{
			wxString strSel=pCombox->GetStringSelection();
			if(strSel==pTmp->RateField)
			{
				m_curExchangeRate=DEFAULT_SVR()->GetExchangeRate((char*)strSel.c_str());
				for(int i=0;i<m_list->GetItemCount();i++)
				{
					SetColumnsText(i);
				}
			}
		}
	}
	wxDELETE(pTmp);
	event.SetClientData(NULL);
}
void FundAccountPats::InsertString(wxArrayString& strArr,wxString str)
{
	int size=strArr.GetCount();
	for(int i=0;i<size;++i)
	{
		if(strArr[i]==str)
			return;
	}
	strArr.Add(str);
}
void FundAccountPats::InitComboBox()
{
	new wxStaticText(this,ID_STATIC,LOADSTRING(LOCAL_CURRENCY));

	vector<PlatformStru_Commodity> vecCommodity;
	wxArrayString strArr;
	if(	DEFAULT_SVR()&&DEFAULT_SVR()->GetCommodity(vecCommodity)>0){
		std::vector<PlatformStru_Commodity>::iterator it=vecCommodity.begin();
		for( ; it!=vecCommodity.end();it++){
			InsertString(strArr,it->Currency);
		}
	}
	new wxComboBox(this,ID_CBX_CURRENCY,"",wxDefaultPosition, wxDefaultSize,strArr,wxCB_READONLY);
}
void FundAccountPats::OnSize( wxSizeEvent& event )
{
	if(m_list) {
		wxSize sz=GetClientSize();
		m_list->SetSize(0,0,sz.x,sz.y-26);
		wxWindow* pStatic = FindWindowById(ID_STATIC, this);
		if(pStatic) pStatic->SetSize(0,sz.y-22,120,20);
		wxWindow* pCombox = FindWindowById(ID_CBX_CURRENCY, this);
		if(pCombox) pCombox->SetSize(124,sz.y-25,80,22);
		wxWindow* pReQry = FindWindowById(ID_BUTTON_REQRY,this);
		if(pReQry) pReQry->SetSize(220,sz.y-24,80,22);

	}
}
void FundAccountPats::InitListCtrl()
{
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
    if( pCfgMgr == NULL) return;
	LPLIST_TABLE_CFG pTableCfg = pCfgMgr->GetListCfg(GID_ACCOUNT);
	if(pTableCfg==NULL) return;
	m_list->DeleteAllItems();
	m_list->DeleteAllColumns();
	//设置LIST界面属性
	m_list->SetHeaderBackgroundColour(wxColor(pTableCfg->HeadBackgroundColor));
	m_list->SetHeaderForegroundColour(wxColor(pTableCfg->HeadColor));
	m_list->SetBackgroundColour(wxColor(pTableCfg->BackgroundColor));
	m_list->SetForegroundColour( wxColor(pTableCfg->TextColor));
	wxFont m_Font;
	m_Font.SetNativeFontInfoUserDesc(pTableCfg->szFont); 
	m_list->SetFont(m_Font);
	m_list->SetOddRowColour(pTableCfg->OddLineBackgroundColor);
	m_list->SetEvenRowColour(pTableCfg->EvenLineBackgroundColor);
	int count=pCfgMgr->GetListVisibleColCount(GID_ACCOUNT);
	 wxExtListItem col;	
	for(int index=0;index<count;index++)
	{
		LPLIST_COLUMN_CFG pColCfg=pCfgMgr->GetListVisibleColCfg(GID_ACCOUNT,index);
		if(pColCfg)
		{
			col.SetText(pCfgMgr->GetColCfgCaption(GID_ACCOUNT,pColCfg->id));
			col.SetWidth(pColCfg->Width);
			m_list->InsertColumn(index,col); 
		}
	}
	wxCommandEvent evt(wxEVT_CALC_FUNDACCOUNT);
	AddPendingEvent(evt);
}
//订阅和取消订阅消息响应
void FundAccountPats::OnSubscrible(wxCommandEvent& evt)
{
	if(evt.GetInt())
	{
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTrade, GID_ACCOUNT, AllCallBackFunc);	
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_ACCOUNT, AllCallBackFunc);						
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderAction, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnDepthMarketData, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingAccount, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingNotice, GID_ACCOUNT, AllCallBackFunc);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ExchangeRate,GID_ANY,ExchangeRateCallBackFunc);
	}
	else
	{
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnTrade, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnOrder, GID_ACCOUNT);						
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderAction, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderInsert, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingAccount, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingNotice, GID_ACCOUNT);
		if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ExchangeRate, GID_ANY);
	}
}
int FundAccountPats::AllCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(data.BID==BID_RtnDepthMarketData)
    {
        DataRtnDepthMarketData& refData = *(DataRtnDepthMarketData*)(const_cast<AbstractBusinessData*>(&data));	

        if(DEFAULT_SVR()&&DEFAULT_SVR()->HavePositionDetail(std::string(refData.DepthMarketDataField.InstrumentID)))
        {
            wxCommandEvent evt(wxEVT_CALC_FUNDACCOUNT);
            if(m_FundAccountPats)
				m_FundAccountPats->AddPendingEvent(evt);
        }
    }
    else
    {
        wxCommandEvent evt(wxEVT_CALC_FUNDACCOUNT);
        if(m_FundAccountPats)
			m_FundAccountPats->AddPendingEvent(evt);
    }
	return 0;
}
//重新计算资金栏各项
void FundAccountPats::OnAllCallBack(wxCommandEvent& event)
{
	if(!DEFAULT_SVR())return;
	vector<string> accounts;
	DEFAULT_SVR()->GetAccountList(accounts);
	PlatformStru_TradingAccountInfo NewAccountInfo;
	for(int index=0;index<(int)accounts.size();index++)
	{
		if(DEFAULT_SVR()->GetTradingAccount(accounts[index], NewAccountInfo)==0)
		{
		    UpdateUIData(accounts[index],NewAccountInfo);
		}
	}
}

void FundAccountPats::UpdateUIData(string strAccount,PlatformStru_TradingAccountInfo& AccountInfo)
{
	PlatformStru_TradingAccountInfo* pFundRowData=NULL;
	int row_index=-1,count=m_list->GetItemCount();
	for(int i=0;i<count;i++)
	{
		pFundRowData=(PlatformStru_TradingAccountInfo*)m_list->GetItemData(i);
		if(pFundRowData&&strAccount==pFundRowData->Account)
		{
			row_index=i;
			if(memcmp(&AccountInfo,pFundRowData,sizeof(AccountInfo))==0)
			{
				//相同不需要改变
				return;
			}
			memcpy(pFundRowData,&AccountInfo,sizeof(AccountInfo));
			break;
		}
	}
	if(row_index<0)//插入一行
	{
		row_index=m_list->InsertItem(m_list->GetItemCount(),wxEmptyString);
        pFundRowData=new PlatformStru_TradingAccountInfo();
		memcpy(pFundRowData,&AccountInfo,sizeof(PlatformStru_TradingAccountInfo));
		m_list->SetItemData(row_index,(long)pFundRowData);
	}
	SetColumnsText(row_index);
}
void FundAccountPats::SetColumnsText(int row)
{
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
    if( pCfgMgr == NULL) return;
	if(row<0||row>=m_list->GetItemCount())return;
	PlatformStru_TradingAccountInfo* pFundRowData=(PlatformStru_TradingAccountInfo*)m_list->GetItemData(row);
	if(pFundRowData==NULL) return;
	int count=pCfgMgr->GetListVisibleColCount(GID_ACCOUNT);
	for(int index=0;index<count;index++)
	{
		LPLIST_COLUMN_CFG pColCfg=pCfgMgr->GetListVisibleColCfg(GID_ACCOUNT,index);
		if(pColCfg)
		{
			m_list->SetItem(row,index,GetColValue(pColCfg->id,pFundRowData));
		}
	}
}
wxString FundAccountPats::GetColValue(int id,PlatformStru_TradingAccountInfo* info)
{
	wxString strText;
	if(info)
	{
		switch(id)
		{
		case FUNDACCOUNT_STATICPROFIT_COL_ID:				                //静态权益
			GETFUNDSTR(strText, info->StaticProfit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_POSITIONPROFIT_COL_ID:				                //持仓盈亏
			GETFUNDSTR(strText, info->PositionProfit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_CLOSEPROFIT_COL_ID:				                    //平仓盈亏
			GETFUNDSTR(strText, info->CloseProfit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_DYNAMICPROFIT_COL_ID:				                //动态权益
			GETFUNDSTR(strText, info->DynamicProfit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_CURRMARGIN_COL_ID:                                  //占用保证金
			GETFUNDSTR(strText, info->CurrMargin*m_curExchangeRate);
			break;
		case FUNDACCOUNT_COMMISSION_COL_ID:                      		    //手续费 
			GETFUNDSTR(strText, info->Commission*m_curExchangeRate);
			break;
		case FUNDACCOUNT_FROZEN_MARGIN_COL_ID:							    //冻结保证金
			GETFUNDSTR(strText, info->FrozenMargin*m_curExchangeRate);
			break;
		case FUNDACCOUNT_FROZEN_COMMISSION_COL_ID:					        //冻结手续费
			GETFUNDSTR(strText, info->FrozenCommission*m_curExchangeRate);
			break;
		case FUNDACCOUNT_PREBALANCE_COL_ID:							        //上次结算准备金
			GETFUNDSTR(strText, info->PreBalance*m_curExchangeRate);
			break;
		case FUNDACCOUNT_PRECREDIT_COL_ID:					   		        //上次信用额度
			GETFUNDSTR(strText, info->PreCredit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_PREMORTGAGE_COL_ID:		                	        //上次质押金额
			GETFUNDSTR(strText, info->PreMortgage*m_curExchangeRate);
			break;
		case FUNDACCOUNT_MORTGAGE_COL_ID:				         			//质押金额 
			GETFUNDSTR(strText, info->Mortgage*m_curExchangeRate);
			break;
		case FUNDACCOUNT_FROZENCASH_COL_ID:			        			    //冻结资金
			GETFUNDSTR(strText, info->FrozenCash*m_curExchangeRate);
			break;
		case FUNDACCOUNT_AVALIABLE_COL_ID:					   	            //可用资金
			GETFUNDSTR(strText, info->Available*m_curExchangeRate);
			break;
		case FUNDACCOUNT_WITHDRAWQUOTA_COL_ID:						        //可取资金	 
			GETFUNDSTR(strText, info->WithdrawQuota*m_curExchangeRate);
			break;
		case FUNDACCOUNT_RISKDEGREE_COL_ID:			            	        //风险度
			strText.Printf("%.2f%%",info->RiskDegree);
			break;
		case FUNDACCOUNT_CREDIT_COL_ID:					        	        //信用金额 
			GETFUNDSTR(strText, info->Credit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_DEPOSITE_COL_ID:				        	        //今日入金 
			GETFUNDSTR(strText, info->Deposit*m_curExchangeRate);
			break;
		case FUNDACCOUNT_WITHDRAW_COL_ID:			                		//今日出金
			GETFUNDSTR(strText, info->Withdraw*m_curExchangeRate);
			break;
		case FUNDACCOUNT_BPREMAINING_COL_ID:			         			    //剩余购买力
			GETFUNDSTR(strText, atof(info->BPRemaining)*m_curExchangeRate);
			break;
		case FUNDACCOUNT_BPUSED_COL_ID:				         			    //占用的资金
			GETFUNDSTR(strText, atof(info->BPUsed)*m_curExchangeRate);
			break;
		case FUNDACCOUNT_EXPOSURE_COL_ID:				        		    //合约占用资金比率
			GETFUNDSTR(strText, atof(info->Exposure)*m_curExchangeRate);
			break;
		case FUNDACCOUNT_ACCOUNTID_COL_ID:				        		        //账号
			strText.Printf("%s",info->Account);
			break;
		case FUNDACCOUNT_BURNRATE_COL_ID:				            	    //合约的盈亏比率 
			strText.Printf("%s",info->BurnRate);
			break;
		case FUNDACCOUNT_MARGINPAID_COL_ID:                                  //占用保证金
			GETFUNDSTR(strText, atof(info->MarginPaid)*m_curExchangeRate);
			break;
        case FUNDACCOUNT_TOTALPROFIT_COL_ID:
			GETFUNDSTR(strText, (info->PositionProfit+info->CloseProfit)*m_curExchangeRate);
            break;
		}
	}
	return strText;
}
void FundAccountPats::OnCfgChanged(wxCommandEvent& evt)
{
	InitListCtrl();
}
#ifdef _USE_MULTI_LANGUAGE
void FundAccountPats::OnLanguageChange(wxCommandEvent& evt)
{
	TRANSFORM_COLUMN_TEXT(GID_ACCOUNT,m_list);
	wxWindow* pStatic = FindWindowById(ID_STATIC, this);
	if(pStatic) pStatic->SetLabel(LOADSTRING(LOCAL_CURRENCY));
	wxWindow* pBtnReQry=FindWindowById(ID_BUTTON_REQRY,this);
	if(pBtnReQry) pBtnReQry->SetLabel(LOADSTRING(BUTTON_REQRY_CAPTION));
}
#endif
void FundAccountPats::OnContext(wxContextMenuEvent& evt)
{
    wxMenu menu;    
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
    menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

    POINT pt;
    ::GetCursorPos(&pt);
    wxPoint point(pt.x,pt.y);
    point = ScreenToClient(point);
    PopupMenu(&menu,point.x,point.y);
}
void FundAccountPats::OnConfig(wxCommandEvent& event)
{
    SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_ACCOUNT);
}
void FundAccountPats::OnMenuExport(wxCommandEvent& event)
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
    wxString cap = pMgr!=NULL ? (wxString)pMgr->GetPanelCaption(GetId()) : wxString("FundPanel");

    LIST_EXPORT_CSV_FILE(cap,m_list);
}
void FundAccountPats::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nColCount = m_list->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_ACCOUNT ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_ACCOUNT, i);
		pColCfg->Width = m_list->GetColumnWidth( i );
	}
}
void FundAccountPats::OnAutoAdjustColWidth(wxCommandEvent& event)
{
	m_list->AutoAdjustColumnWidth();
	SaveColWidth();
}
void FundAccountPats::OnCurrencyChangeSel( wxCommandEvent& event )
{
	if(!DEFAULT_SVR()) return;
	wxComboBox* pCombox =(wxComboBox*)FindWindowById(ID_CBX_CURRENCY, this);
	if(!pCombox) return;
	wxString strSel=pCombox->GetStringSelection();
	if(strSel.IsEmpty()) return;
	m_curExchangeRate=DEFAULT_SVR()->GetExchangeRate((char*)strSel.c_str());
	for(int i=0;i<m_list->GetItemCount();i++)
	{
		SetColumnsText(i);
	}

}
void FundAccountPats::QryTradingAccount(wxCommandEvent& event)
{
	if(!m_pFundAccountQryDlg)
		m_pFundAccountQryDlg=new FundAccountQryDlg(((wxApp*)wxApp::GetInstance())->GetTopWindow());

	m_pFundAccountQryDlg->Hide();
	m_pFundAccountQryDlg->SetContent();
	m_pFundAccountQryDlg->CenterOnScreen();
	m_pFundAccountQryDlg->Show();
}
void FundAccountPats::OnReQry(wxCommandEvent& event)
{
	wxCommandEvent evt(wxEVT_CALC_FUNDACCOUNT);
	AddPendingEvent(evt);
}