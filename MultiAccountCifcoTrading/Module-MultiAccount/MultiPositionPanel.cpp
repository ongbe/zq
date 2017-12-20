#include "Stdafx.h"
#include "MultiPositionPanel.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../ConfigApp/Const.h"
#include "../inc/Plusin/Plusin.h"
#include "../Module-Misc/orderCommonFunc.h"
#include "wx/file.h"
#define SUM_TAG(tag)\
do{\
	if(!util::isInvalidValue(dest.tag)){\
		if(!util::isInvalidValue(src.tag))\
			dest.tag+=src.tag;\
		else\
			dest.tag=src.tag;\
	}\
}while(0)
#define WEIGHT_TAG(tag) do{dest.tag=(src.tag*src.Position+dest.tag*dest.Position)/(src.Position+dest.Position);}while(0)
extern PlatformSvrMgr* g_pPlatformMgr;
extern CPlusin g_Plusin;
MultiPositionPanel* g_MultiPositionPanel=NULL;
BEGIN_EVENT_TABLE(MultiPositionPanel,wxPanel) 
	EVT_SIZE(OnSize)
    EVT_SUBSCRIBE(OnSubscrible)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryPositionLast,  OnRspQryPositionLast)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryMarginRate,  OnGetMarginRate)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnTrade,  OnRtnTrade)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnOrder,  OnRtnOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnDepthMarketData,  OnRtnDepthMarketData)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, OnActivated)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, OnSelected)
	EVT_CFG_CHANGE(OnCfgChanged)
END_EVENT_TABLE()
MultiPositionPanel::MultiPositionPanel(wxWindow *parent,
									 wxWindowID winid,
									 const wxPoint& pos ,
									 const wxSize& size,
									 long style,
									 const wxString& name):
wxPanel(parent,winid,pos,size,style,name),
m_pList(NULL)
{
	g_MultiPositionPanel=this;
	SetBackgroundColour(DEFAULT_COLOUR);
	m_pList=new wxExtListCtrl(this,wxID_ANY,wxDefaultPosition, wxDefaultSize, 
		wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES);
	InitListCtrl();
	new wxButton(this, ID_BUTTON_REQRY, LOADSTRING(BUTTON_REQRY_CAPTION), wxPoint(), wxSize(80,CommonBtnHeight));

}
void MultiPositionPanel::OnSubscrible(wxCommandEvent& evt)
{
	set<IPlatformSingleSvr*>::iterator it;
	if(evt.GetInt())
	{
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			(*it)->SubscribeBusinessData(BID_RspQryInvestorPosition, GID_MULTI_POSITION, CallBackFunc);
			(*it)->SubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_MULTI_POSITION, CallBackFunc);
			(*it)->SubscribeBusinessData(BID_RtnTrade, GID_MULTI_POSITION, CallBackFunc);
			(*it)->SubscribeBusinessData(BID_RtnOrder, GID_MULTI_POSITION, CallBackFunc);
			(*it)->SubscribeBusinessData(BID_RtnDepthMarketData, GID_MULTI_POSITION, CallBackFunc);
		}
	}
	else
	{
		for (it=m_pSvrSet.begin();it!=m_pSvrSet.end();++it)
		{
			(*it)->UnSubscribeBusinessData(BID_RspQryInvestorPosition, GID_MULTI_POSITION);
			(*it)->UnSubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_MULTI_POSITION);
			(*it)->UnSubscribeBusinessData(BID_RtnTrade, GID_MULTI_POSITION);
			(*it)->UnSubscribeBusinessData(BID_RtnOrder, GID_MULTI_POSITION);
			(*it)->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_MULTI_POSITION);
		}
	}
}
void MultiPositionPanel::InitListCtrl()
{
	DeleteAllItemData();
	DeleteAllColumnData();
	LPLIST_TABLE_CFG pListCfg;
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if(!pCfgMgr) return;

	pListCfg = pCfgMgr->GetListCfg(GID_MULTI_POSITION);
	if(pListCfg)
	{
		//设置LIST界面属性
		m_pList->SetBackgroundColour(wxColor(pListCfg->BackgroundColor));
		m_pList->SetHeaderBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
		m_pList->SetHeaderForegroundColour(wxColor(pListCfg->HeadColor));//白  16777215
		m_pList->SetForegroundColour(wxColor(pListCfg->TextColor));
		wxFont tmpfontLarge;
		tmpfontLarge.SetNativeFontInfoUserDesc(pListCfg->szFont);
		m_pList->SetFont(tmpfontLarge);

		m_pList->SetOddRowColour( wxColor(pListCfg->OddLineBackgroundColor));
		m_pList->SetEvenRowColour( wxColor(pListCfg->EvenLineBackgroundColor));

		//m_TextColor=pListCfg->TextColor;

	}


	wxExtListItem col;
	int i;
	wxExtListItem ColumnInfo; 
	LPLIST_COLUMN_CFG pColCfg;

	//列的顺序按用户的配置顺序存储在配置信息中
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_MULTI_POSITION);
	for(i = 0;  i < nColCount; i++)
	{
		pColCfg = pCfgMgr->GetListVisibleColCfg(GID_MULTI_POSITION, i);
		if(!pColCfg) continue;

		if(pColCfg->id == POSITION_FILED_ID_INSTRUMENTID)       //合约字段 左对齐
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else                                                //其余右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);


		col.SetText(pCfgMgr->GetColCfgCaption(GID_MULTI_POSITION,pColCfg->id));
		col.SetWidth(pColCfg->Width);
		col.SetTextColour(pColCfg->TextColor);
		col.SetColData( pColCfg->id );

		m_pList->InsertColumn(i, col); 

		m_FieldID2ColumnID[pColCfg->id] = i;
		m_ColumnID2FieldID[i]=pColCfg->id;

	}
	ShowAll();	
}
void MultiPositionPanel::OnSize(wxSizeEvent& event)
{
	if(m_pList) 
	{
		wxSize sz=GetClientSize();
		m_pList->SetSize(0,0,sz.x,sz.y-26);
		wxWindow* pReQry = FindWindowById(ID_BUTTON_REQRY,this);
		if(pReQry) pReQry->SetSize(10,sz.y-24,80,22);
	}
}
void MultiPositionPanel::DeleteAllItemData()
{
	m_pList->DeleteAllItems();
	m_Row2Key.clear();
	m_Key2Row.clear();
}
void MultiPositionPanel::DeleteAllColumnData()
{
	m_pList->DeleteAllColumns();
	m_FieldID2ColumnID.clear();
	m_ColumnID2FieldID.clear();
}
void MultiPositionPanel::MergePosition(PlatformStru_Position& dest,const PlatformStru_Position& src)
{
	WEIGHT_TAG(AveragePositionPrice);
	WEIGHT_TAG(AverageOpenPrice);
	SUM_TAG(Position);
	SUM_TAG(CanCloseVolume);
	SUM_TAG(PositionProfit);
	SUM_TAG(PositionProfitByTrade);
	SUM_TAG(TotalPositionProfitByDate);
	SUM_TAG(UseMargin);
	SUM_TAG(TodayPosition);
	SUM_TAG(OpenVolume);
	SUM_TAG(CloseVolume);
	SUM_TAG(YdPositionRemain);
	SUM_TAG(CloseTodayOrderVolume);
	SUM_TAG(CloseOrderVolumeTdPart);
	SUM_TAG(CloseYesterdayOrderVolume);
	SUM_TAG(CloseOrderVolumeYdPart);
	SUM_TAG(CanCloseTodayVolume);
	SUM_TAG(CanCloseydVolume);
}
void MultiPositionPanel::ShowAll()
{
    DeleteAllItemData();
	std::map<PositionKey,PlatformStru_Position> mapContent;
	std::set<PositionKey> PositionKeySet;
	std::set<PositionKey>::iterator keyit;
	set<IPlatformSingleSvr*>::iterator svrit;
	for (svrit=m_pSvrSet.begin();svrit!=m_pSvrSet.end();++svrit)
	{
		(*svrit)->GetPositionKeySet(PositionKeySet);
		for (keyit=PositionKeySet.begin();keyit!=PositionKeySet.end();++keyit)
		{
			PlatformStru_Position pos;
			if(0==(*svrit)->GetPositionData(keyit->Account,keyit->InstrumentID,
				keyit->PosiDirection,keyit->HedgeFlag,pos)&&pos.Position>0)
			{
				PositionKey key("",keyit->InstrumentID,keyit->PosiDirection,keyit->HedgeFlag);
				if(mapContent.find(key)==mapContent.end())
					mapContent[key]=pos;
				else
					MergePosition(mapContent[key],pos);
			}
		}
	}
	std::map<PositionKey,PlatformStru_Position>::iterator mapit;
	for (mapit=mapContent.begin();mapit!=mapContent.end();++mapit)
	{
		if(mapit->second.Position>0)
		{
			int row=AddRow(mapit->first);
			SetOneRow(row,mapit->second);
		}
	}
}
void MultiPositionPanel::OnMAPlatformAdd(wxCommandEvent& event)
{
	if(event.GetId()!=GetId()) return;
	m_pSvrSet.insert((IPlatformSingleSvr*)event.GetInt());
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
	ShowAll();
}
void MultiPositionPanel::OnMAPlatformDelete(wxCommandEvent& event)
{
	if(event.GetId()!=GetId()) return;
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(0);
	ProcessEvent(myEvent);
	m_pSvrSet.erase((IPlatformSingleSvr*)event.GetInt());
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
	ShowAll();
}
int MultiPositionPanel::CallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(g_MultiPositionPanel==NULL) return 0;
	switch(data.BID)
	{
	case BID_RspQryInvestorPosition:
		if(data.Size==sizeof(DataRspQryInvestorPosition))
		{
			wxCommandEvent evt(wxEVT_RspQryPositionLast, wxID_ANY);
			DWORD EvtParamID;
			CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInvestorPosition),0);
			evt.SetInt((int)EvtParamID);
			g_MultiPositionPanel->AddPendingEvent(evt);
		}
		break;
	case BID_RspQryInstrumentMarginRate:
		if(data.Size==sizeof(DataRspQryInstrumentMarginRate))
		{
			DWORD EvtParamID;
			CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInstrumentMarginRate),0);

			wxCommandEvent evt(wxEVT_RspQryMarginRate, wxID_ANY);
			evt.SetInt((int)EvtParamID);
			g_MultiPositionPanel->AddPendingEvent(evt);
		}
		break;
	case BID_RtnTrade:
		if(data.Size==sizeof(DataRtnTrade))
		{
			DWORD EvtParamID;
			CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnTrade),0);

			wxCommandEvent evt(wxEVT_RtnTrade, wxID_ANY);
			evt.SetInt((int)EvtParamID);
			g_MultiPositionPanel->AddPendingEvent(evt);
		}
		break;
	case BID_RtnOrder:
		if(data.Size==sizeof(DataRtnOrder))
		{
			DWORD EvtParamID;
			CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnOrder),0);

			wxCommandEvent evt(wxEVT_RtnOrder, wxID_ANY);
			evt.SetInt((int)EvtParamID);
			g_MultiPositionPanel->AddPendingEvent(evt);
		}
		break;
	case BID_RtnDepthMarketData:
		if(data.Size==sizeof(DataRtnDepthMarketData))
		{
			DWORD EvtParamID;
			CEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnDepthMarketData),0);

			wxCommandEvent evt(wxEVT_RtnDepthMarketData, wxID_ANY);
			evt.SetInt((int)EvtParamID);
			g_MultiPositionPanel->AddPendingEvent(evt);
		}
		break;
	}
	return 0;
}
void MultiPositionPanel::DeleteRow(int row)
{
	if(m_pList->DeleteItem(row))
	{
		std::map<PositionKey,int>::iterator ip_it;
		for (ip_it=m_Key2Row.begin();ip_it!=m_Key2Row.end();)
		{
			if(ip_it->second==row)
				ip_it=m_Key2Row.erase(ip_it);
			else
			{
				if(ip_it->second>row)
					ip_it->second--;
				++ip_it;
			}
		}
		m_Row2Key.clear();
		for (ip_it=m_Key2Row.begin();ip_it!=m_Key2Row.end();++ip_it)
		{
			m_Row2Key.insert(make_pair(ip_it->second,ip_it->first));
		}
	}
}
#define UpdateColItem_Money(t)\
	(util::isInvalidValue(t)?"-":wxString::Format("%.02lf",t))
#define UpdateColItem_Int(t)\
	(util::isInvalidValue(t)?"-":wxString::Format("%d",t))
#define UpdateColItem_Fund(t)\
	(util::isInvalidValue(t)?"-":GlobalFunc::GetAccountFormatString(t,2))
#define UpdateColItem_Price(t,tick)\
	(util::isInvalidValue(t)?"-":Price2String(t,tick))
#define UpdateColItem_IntInt(a,b)\
	((util::isInvalidValue(a)||util::isInvalidValue(b))?"-":wxString::Format("%d",a+b))
void MultiPositionPanel::SetOneRow(int row,const PlatformStru_Position& pos)
{
	wxExtListItem outItem;
	outItem.SetId(row);

	for (int col=0;col<m_pList->GetColumnCount();++col)
	{
		if(m_ColumnID2FieldID.find(col)==m_ColumnID2FieldID.end())
			continue;
		outItem.SetText(wxEmptyString);
		outItem.SetTextColour(wxColor(0,0,0));
		outItem.SetColumn(col);
		switch(m_ColumnID2FieldID[col])
		{
			///合约代码	
		case MULTIPOSITION_FIELD_ID_INSTRUMENTID:
			outItem.SetText(pos.InstrumentID);
			break;
			///持仓多空方向	
		case MULTIPOSITION_FIELD_ID_DIRECTION:
			outItem.SetText(CFieldValueTool::PosiDirection2String(pos.PosiDirection));
			outItem.SetTextColour(wxColor(pos.PosiDirection==THOST_FTDC_PD_Long?RGB(255,0,0):RGB(0,128,0)));
			break;
			///投机套保标志	
		case MULTIPOSITION_FIELD_ID_HEDGEFLAG:
			outItem.SetText(CFieldValueTool::HedgeFlag2String(pos.HedgeFlag));
			break;
			//交易所
		case MULTIPOSITION_FIELD_ID_EXCHANGE:
			outItem.SetText(CFieldValueTool::ExchangeID2Name((char*)pos.ExchangeID));
			break;
			//货币
		case MULTIPOSITION_FIELD_ID_CURRENCY:
			outItem.SetText(pos.Currency);
			break;
		case MULTIPOSITION_FIELD_ID_OPENAVGPRICE:
			outItem.SetText(UpdateColItem_Money(pos.AveragePositionPrice));
			break;
		case MULTIPOSITION_FIELD_ID_OPENPROFIT:
			outItem.SetText(UpdateColItem_Money(pos.PositionProfit));
			break;
        case MULTIPOSITION_FIELD_ID_BUYVOLUMN:
			if(pos.PosiDirection==THOST_FTDC_PD_Long)
			{
				outItem.SetText(UpdateColItem_Int(pos.Position));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
        case MULTIPOSITION_FIELD_ID_SELVOLUMN:
            if(pos.PosiDirection==THOST_FTDC_PD_Short)
			{
				outItem.SetText(UpdateColItem_Int(pos.Position));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_TOTALPROFIT:
			outItem.SetText(UpdateColItem_Money(pos.TotalPositionProfitByDate));
			break;
		case MULTIPOSITION_FIELD_ID_CLOSEPROFIT:
			outItem.SetText(UpdateColItem_Money(pos.CloseProfit));
			break;
		case MULTIPOSITION_FIELD_ID_CLOSEVOLUMN:
			outItem.SetText(UpdateColItem_Int(pos.CanCloseVolume));
			break;
		case MULTIPOSITION_FIELD_ID_PREPOSITION:
			outItem.SetText(UpdateColItem_Int(pos.YdPositionRemain));
			break;
		case MULTIPOSITION_FIELD_ID_TOTALTODAYCLOSEVOLUME:
			outItem.SetText(UpdateColItem_Int(pos.TodayPosition));
			break;
		case MULTIPOSITION_FIELD_ID_TODAYENABLECLOSEVOLUME:
			outItem.SetText(UpdateColItem_Int(pos.CanCloseTodayVolume));
			break;
		case MULTIPOSITION_FIELD_ID_TOTALPRECLOSEVOLUME:
			outItem.SetText(UpdateColItem_Int(pos.YdPositionRemain));
			break;
		case MULTIPOSITION_FIELD_ID_PREENABLECLOSEVOLUME:
			outItem.SetText(UpdateColItem_Int(pos.CanCloseydVolume));
			break;
		case MULTIPOSITION_FIELD_ID_TOTAL_PROFIT:
			outItem.SetText(UpdateColItem_Fund(pos.TotalPositionProfitByDate));
			if(pos.TotalPositionProfitByDate>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else if(pos.TotalPositionProfitByDate<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
			break;
		case POSITION_FILED_ID_TOTALPOSITION:
			outItem.SetText(UpdateColItem_Int(pos.Position));
			break;
		case MULTIPOSITION_FIELD_ID_TOTALBUYPOSITION:
			if(pos.PosiDirection==THOST_FTDC_PD_Long)
			{
				outItem.SetText(UpdateColItem_Int(pos.Position));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_TOTALSELLPOSITION:
			if(pos.PosiDirection==THOST_FTDC_PD_Short)
			{
				outItem.SetText(UpdateColItem_Int(pos.Position));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_TODAYPOSITION:
			outItem.SetText(UpdateColItem_Int(pos.TodayPosition));
			break;
		case MULTIPOSITION_FIELD_ID_TODAYBUYPOSITION:
			if(pos.PosiDirection==THOST_FTDC_PD_Long)
			{
				outItem.SetText(UpdateColItem_Int(pos.TodayPosition));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_TODAYSELLPOSITION:
			if(pos.PosiDirection==THOST_FTDC_PD_Short)
			{
				outItem.SetText(UpdateColItem_Int(pos.TodayPosition));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_TODAYOPENVOLUME:
			outItem.SetText(UpdateColItem_Int(pos.OpenVolume));
			break;
		case MULTIPOSITION_FIELD_ID_TODAYCLOSEVOLUME:
			outItem.SetText(UpdateColItem_Int(pos.CloseVolume));
			break;
		case MULTIPOSITION_FIELD_ID_WAITCLOSE:       //平仓挂单量，指平昨
			outItem.SetText(UpdateColItem_IntInt(pos.CloseYesterdayOrderVolume,pos.CloseOrderVolumeYdPart));
			break;
		case MULTIPOSITION_FIELD_ID_TODAYWAITCLOSE:  //平今挂单量
			outItem.SetText(UpdateColItem_IntInt(pos.CloseTodayOrderVolume,pos.CloseOrderVolumeTdPart));
			break;
		case MULTIPOSITION_FIELD_ID_AVGPRICE:
			outItem.SetText(UpdateColItem_Money(pos.AveragePositionPrice));
			break;
		case MULTIPOSITION_FIELD_ID_AVGOPENPRICE:
			outItem.SetText(UpdateColItem_Money(pos.AverageOpenPrice));
			break;
		case MULTIPOSITION_FIELD_ID_BUYOPENPRICE:
			if(pos.PosiDirection==THOST_FTDC_PD_Long)
			{
				outItem.SetText(UpdateColItem_Money(pos.AverageOpenPrice));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_SELLOPENPRICE:
			if(pos.PosiDirection==THOST_FTDC_PD_Short)
			{
				outItem.SetText(UpdateColItem_Money(pos.AverageOpenPrice));
			}
			else 
			{
				outItem.SetText(wxString("-"));
			}
			break;
		case MULTIPOSITION_FIELD_ID_PROFIT:
			outItem.SetText(UpdateColItem_Fund(pos.PositionProfit));
			if(pos.PositionProfit>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else if(pos.PositionProfit<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
			break;
		case MULTIPOSITION_FIELD_ID_FLOATPROFIT:
			outItem.SetText(UpdateColItem_Fund(pos.PositionProfitByTrade));
			if(pos.PositionProfitByTrade>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else if(pos.PositionProfitByTrade<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
			break;
		case MULTIPOSITION_FIELD_ID_MARGIN:
			outItem.SetText(UpdateColItem_Fund(pos.UseMargin));
			break;
		case MULTIPOSITION_FIELD_ID_LASTPRICE:
			outItem.SetText(UpdateColItem_Price(pos.CurPrice,pos.PriceTick));
			break;
		case MULTIPOSITION_FIELD_ID_MARGINPERLOT:
			outItem.SetText(UpdateColItem_Money(pos.pats_MarginPerLot));
			break;
		case MULTIPOSITION_FIELD_ID_PLBURNRATE:
			outItem.SetText(UpdateColItem_Money(pos.pats_PLBurnRate));
			break;
		case MULTIPOSITION_FIELD_ID_OPENPOSITIONEXPOSURE:
			outItem.SetText(UpdateColItem_Money(pos.pats_OpenPositionExposure));
			break;
		case MULTIPOSITION_FIELD_ID_BUYPOWERREMAIN:
			outItem.SetText(UpdateColItem_Money(pos.pats_BuyPowerRemain));
			break;
		case MULTIPOSITION_FIELD_ID_BUYPOWERUSED:
			outItem.SetText(UpdateColItem_Money(pos.pats_BuyPowerUsed));
			break;
		case MULTIPOSITION_FIELD_ID_TOTALMARGINPAID:
			outItem.SetText(UpdateColItem_Money(pos.pats_TotalMarginPaid));
			break;
		}
		m_pList->SetItem(outItem);
	}
}
void MultiPositionPanel::UpdateSameInstrumentIDRows(wxString strInstrumentID)
{
	set<PositionKey> AllKeys;
	AllKeys.insert(PositionKey("",strInstrumentID.c_str(),THOST_FTDC_PD_Long,THOST_FTDC_HF_Speculation));
	AllKeys.insert(PositionKey("",strInstrumentID.c_str(),THOST_FTDC_PD_Long,THOST_FTDC_HF_Hedge));
	AllKeys.insert(PositionKey("",strInstrumentID.c_str(),THOST_FTDC_PD_Short,THOST_FTDC_HF_Speculation));
	AllKeys.insert(PositionKey("",strInstrumentID.c_str(),THOST_FTDC_PD_Short,THOST_FTDC_HF_Hedge));
	set<PositionKey>::iterator set_it;
	std::map<PositionKey,int>::iterator  map_it;
	vector<int> waitDeleteRow;
	for (set_it=AllKeys.begin();set_it!=AllKeys.end();++set_it)
	{
		PlatformStru_Position all;
		if(GetPositionFromKey(*set_it,all))
		{
			int row;
			map_it=m_Key2Row.find(*set_it);
			if(map_it==m_Key2Row.end())//添加
			{
				row=AddRow(*set_it);
			}
			else//修改
				row=map_it->second;
			SetOneRow(row,all);
		}
		else
		{
			map_it=m_Key2Row.find(*set_it);
			if(map_it==m_Key2Row.end())
			{
			}
			else//删除
			{
				DeleteRow(map_it->second);
			}
		}
	}
}
bool MultiPositionPanel::GetPositionFromKey(const PositionKey key,PlatformStru_Position& all)
{
	bool bFirst=true;
	set<IPlatformSingleSvr*>::iterator svrit;
	for (svrit=m_pSvrSet.begin();svrit!=m_pSvrSet.end();++svrit)
	{
		if(bFirst)
		{
			if((*svrit)->GetPositionData((*svrit)->GetAccount(),key.InstrumentID,
				key.PosiDirection,key.HedgeFlag,all)==0&&all.Position>0)
			{
				bFirst=false;
			}
		}
		else
		{
			PlatformStru_Position tmppos;
			if((*svrit)->GetPositionData((*svrit)->GetAccount(),key.InstrumentID,
				key.PosiDirection,key.HedgeFlag,tmppos)==0&&tmppos.Position>0)
			{
				MergePosition(all,tmppos);
			}
		}
	}
	return !bFirst;
}

wxString MultiPositionPanel::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
{
	wxString strText;
	if(pListCtrl==NULL)
		return strText;
	if(nRow<0 || nRow>=pListCtrl->GetItemCount())
		return strText;
	int nCol = 0;
	wxString strItem;
	wxExtListItem itemCol;
	for(; nCol<pListCtrl->GetColumnCount(); nCol++) {
		pListCtrl->GetColumn(nCol, itemCol);
		strItem.Printf("%s=[%s]%s", itemCol.GetText().c_str(), 
			pListCtrl->GetSubItemText(nRow, nCol).c_str(),
			(nCol<pListCtrl->GetColumnCount()-1?", ":""));
		strText+=strItem;
	}
	return strText;
}

void MultiPositionPanel::OnRspQryPositionLast(wxCommandEvent& evt)
{
	DataRspQryInvestorPosition RspQryData;
	DWORD EvtParamID=(DWORD)evt.GetInt();
	if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspQryData,NULL,sizeof(RspQryData),NULL))
		CEventParam::DeleteEventParam(EvtParamID);
	else return;

	std::string QryInstrumentID=std::string(RspQryData.QryInstrumentID);
	if(QryInstrumentID.empty())
	{
		ShowAll();
	}
	else
		UpdateSameInstrumentIDRows(QryInstrumentID.c_str());
}
void MultiPositionPanel::OnGetMarginRate(wxCommandEvent& evt)
{
	DataRspQryInstrumentMarginRate MarginRate;
	DWORD EvtParamID=(DWORD)evt.GetInt();
	if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarginRate,NULL,sizeof(MarginRate),NULL))
		CEventParam::DeleteEventParam(EvtParamID);
	else return;
	UpdateSameInstrumentIDRows(MarginRate.InstrumentMarginRateField.InstrumentID);
}
void MultiPositionPanel::OnRtnTrade(wxCommandEvent& evt)
{
	DataRtnTrade RtnTrade;
	DWORD EvtParamID=(DWORD)evt.GetInt();
	if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnTrade,NULL,sizeof(RtnTrade),NULL))
		CEventParam::DeleteEventParam(EvtParamID);
	else return;
	UpdateSameInstrumentIDRows(RtnTrade.TradeField.InstrumentID);
}
void MultiPositionPanel::OnRtnOrder(wxCommandEvent& evt)
{
	DataRtnOrder RtnOrder;
	DWORD EvtParamID=(DWORD)evt.GetInt();
	if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnOrder,NULL,sizeof(RtnOrder),NULL))
		CEventParam::DeleteEventParam(EvtParamID);
	else return;
	UpdateSameInstrumentIDRows(RtnOrder.OrderField.InstrumentID);
}
void MultiPositionPanel::OnRtnDepthMarketData(wxCommandEvent& evt)
{
	DataRtnDepthMarketData MarketData;
	DWORD EvtParamID=(DWORD)evt.GetInt();
	if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarketData,NULL,sizeof(MarketData),NULL))
		CEventParam::DeleteEventParam(EvtParamID);
	else return;
	UpdateSameInstrumentIDRows(MarketData.DepthMarketDataField.InstrumentID);

}
void MultiPositionPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_POSITION ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITION, nCol);
	pColCfg->Width = m_pList->GetColumnWidth( nCol );
}
void MultiPositionPanel::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {	
		menu.Append(ID_MENU_PRICEBACKHAND,LOADSTRING(CONTEXTMENU_PRICEBACKHAND));
	}
	menu.Append(ID_MENU_FASTCLOSE,LOADSTRING(CONTEXTMENU_FASTCLOSE));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {	
		menu.Append(ID_MENU_FASTLOCK,LOADSTRING(CONTEXTMENU_FASTLOCK));
	}

	menu.AppendSeparator();
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	BOOL bIsRowCLick = evt.GetId();
	if ( bIsRowCLick )
	{
		menu.AppendSeparator();
		INSERT_ADDTOMYSELF_ITEM(menu);
	}

	if(menu.GetMenuItemCount()>0)
	{
		POINT pt;
		::GetCursorPos(&pt);
		BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

		//加入自选
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
		if( retcmd - ID_MENU_ADDTOMYSELF >= 0 )
		{
			bool bAdded = false;
			for ( int i = 0; i <m_pList->GetItemCount(); i++)
			{
				if ( m_pList->IsSelected( i ))
				{
					std::map<int,PositionKey>::iterator rkit=m_Row2Key.find(i);
					if(rkit==m_Row2Key.end())
						continue;
					string InstrumentId = rkit->second.InstrumentID;
					if(pCfgMgr->GetInstrumentGroupMemberCount(retcmd - ID_MENU_ADDTOMYSELF)>=DEFAULT_SVR()->GetGroupMaxContractNum())
					{
						wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
						break;
					}
					else
					{
						pCfgMgr->AddInstrument( retcmd - ID_MENU_ADDTOMYSELF, InstrumentId.c_str()); 
						bAdded = true;					
					}
				}
			}

			if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);}
			return;
		}


		switch(retcmd)
		{
		case ID_MENU_AUTO_ADJUST_COL_WIDTH:
			m_pList->AutoAdjustColumnWidth();
			SaveColWidth();
			break;
		case ID_MENU_PRICEBACKHAND://市价反手，和快捷锁仓操作一样
		case ID_MENU_FASTLOCK://快捷锁仓
			for(int row=0;row<m_pList->GetItemCount();row++)
			{
				if(m_pList->IsSelected(row))
				{
					std::map<int,PositionKey>::iterator rkit=m_Row2Key.find(row);
					if(rkit!=m_Row2Key.end())
					{
						wxCommandEvent request_event(wxEVT_MA_ORDERINSERT_ORDERCHANGE);
						wxString strCmsStream;
						strCmsStream.Printf("%s,%c,%c",
							rkit->second.InstrumentID,rkit->second.PosiDirection,rkit->second.HedgeFlag);

						DWORD EvtParamID;
						std::string EvtParamStr(strCmsStream.c_str());
						int UserInt;
						if(retcmd==ID_MENU_PRICEBACKHAND)
							UserInt=1;
						else if(retcmd==ID_MENU_FASTLOCK)
							UserInt=3;
						if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,UserInt))
						{
							request_event.SetInt((int)EvtParamID);
							GETTOPWINDOW()->ProcessEvent(request_event);
						}
					}
					else
					{
						WXLOG_INFO("Can't find Position by row(%d)",row);
					}
					break;
				}
			}
			break;
		case ID_MENU_EXPORT://导出列表
			{
				wxString cap=LOADSTRING(Name_MultiPosition);
				LIST_EXPORT_CSV_FILE(cap, m_pList);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
			{
				wxCommandEvent request_event(wxEVT_LOAD_CONFIG_APP);
				request_event.SetInt( ID_ITEM_MULTIPOSITION );
				GETTOPWINDOW()->ProcessEvent(request_event);
			}
			break;
		case ID_MENU_FASTCLOSE: // 快捷平仓
			{
				for(int row=0;row<m_pList->GetItemCount();row++)
				{
					if(m_pList->IsSelected(row))
					{
						std::map<int,PositionKey>::iterator rkit=m_Row2Key.find(row);
						if(rkit!=m_Row2Key.end())
						{
							PlatformStru_Position* pStru=new PlatformStru_Position();
							strcpy(pStru->InstrumentID,rkit->second.InstrumentID);
							pStru->PosiDirection=rkit->second.PosiDirection;
							pStru->HedgeFlag=rkit->second.HedgeFlag;
							wxCommandEvent sendchangeevt(wxEVT_GRID_INSTRUMENTID_CHANGED);

							DWORD EvtParamID;
							std::string EvtParamStr(pStru->InstrumentID);
							CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,1);
							sendchangeevt.SetInt((int)EvtParamID);

							GETTOPWINDOW()->ProcessEvent(sendchangeevt);

							wxCommandEvent evt(wxEVT_MA_ORDERINSERT_MOUSECLOSE);
							evt.SetInt(3);
							evt.SetClientData(pStru);
							GETTOPWINDOW()->ProcessEvent(evt);
						}
						else
						{
							WXLOG_INFO("Can't find position by row(%d) is NULL",row);
						}
						break;
					}
				}
			}
			break;
		default:
			break;
		}

	}
	evt.Skip();
}
void MultiPositionPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nColCount = m_pList->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_MULTI_POSITION ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_MULTI_POSITION, i);
		pColCfg->Width = m_pList->GetColumnWidth( i );
	}
}
void MultiPositionPanel::OnActivated(wxExtListEvent& event)
{
	int row=event.GetIndex();
	std::map<int,PositionKey>::iterator rkit=m_Row2Key.find(row);
	if(rkit!=m_Row2Key.end())
	{
		PlatformStru_Position* pStru=new PlatformStru_Position();
		strcpy(pStru->InstrumentID,rkit->second.InstrumentID);
		pStru->PosiDirection=rkit->second.PosiDirection;
		pStru->HedgeFlag=rkit->second.HedgeFlag;
		wxString strLog;
		if(m_pList!=NULL)
			strLog = GetListCtrlRowLog(m_pList, row);
		wxCommandEvent evt(wxEVT_MA_ORDERINSERT_MOUSECLOSE);
		evt.SetInt(2);
		evt.SetClientData(pStru);
		evt.SetString(strLog);
		GETTOPWINDOW()->ProcessEvent(evt);
	}
	else
	{
		WXLOG_INFO("Can't find position by row(%d) is NULL",row);
	}
}
void MultiPositionPanel::OnSelected(wxExtListEvent& event)
{
	int row=event.GetIndex();
	std::map<int,PositionKey>::iterator rkit=m_Row2Key.find(row);
	if(rkit!=m_Row2Key.end())
	{
		PlatformStru_Position* pStru=new PlatformStru_Position();
		strcpy(pStru->InstrumentID,rkit->second.InstrumentID);
		pStru->PosiDirection=rkit->second.PosiDirection;
		pStru->HedgeFlag=rkit->second.HedgeFlag;

		if ( DEFAULT_SVR() && DEFAULT_SVR()->SupportPlugins() )
		{
			g_Plusin.SetOrderModuleCurInstrumentID(pStru->InstrumentID);
		}

		wxString strLog;
		if(m_pList!=NULL)
			strLog = GetListCtrlRowLog(m_pList, row);
		wxCommandEvent evt(wxEVT_MA_ORDERINSERT_MOUSECLOSE);
		evt.SetInt(1);
		evt.SetClientData(pStru);
		evt.SetString(strLog);
		GETTOPWINDOW()->ProcessEvent(evt);
	}
	else
	{
		WXLOG_INFO("Can't find position by row(%d) is NULL",row);
	}
}

void MultiPositionPanel::OnCfgChanged(wxCommandEvent& evt)
{
	InitListCtrl();
}

int MultiPositionPanel::AddRow(const PositionKey& key)
{
	int row=m_pList->InsertItem(m_pList->GetItemCount(),"");
	m_Row2Key.insert(make_pair(row,key));
	m_Key2Row.insert(make_pair(key,row));
	return row;
}
