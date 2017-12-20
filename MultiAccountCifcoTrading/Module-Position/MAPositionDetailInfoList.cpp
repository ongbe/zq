//  Name: 持仓模块
//  Desc:
//          1. 处理持仓模块的显示
//          2. 将右键菜单的命令通过顶层窗口传递给下单模块
//  Interface:
//          底层模块platformSvr
//              1. 订阅BID_RspQryInvestorPositionDetail
//              2. 订阅BID_RspQryInstrumentMarginRate
//              3. 订阅BID_RtnTrade
//              4. 订阅BID_RtnOrder
//              5. 订阅BID_RtnDepthMarketData
//              6. g_pPlatformGetPositionDetails
//              7. g_pPlatformGetCurPrice
//              8. g_pPlatformGetProductClassType
//              9. g_pPlatformGetTradingDay
//              10.g_pPlatformGetInstrumentInfo
//              11.g_pPlatformGetQuotInfo
//              12.g_pPlatformGetMarginRate
//          配置模块CfgMgr
//              1. CfgMgr::GetInstance()->GetListCfg(GID_POSITIONDETAIL)
//              2. CfgMgr::GetInstance()->GetListColCount(GID_POSITIONDETAIL)
//              3. CfgMgr::GetInstance()->GetListColCfg(GID_POSITIONDETAIL, i);
//          配置程序ConfigApp
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_LOAD_CONFIG_APP) - 表格列设置
//************************************************************************************


#include "StdAfx.h"
#include "MAPositionDetailInfoList.h"

#include "time.h"
#include <fstream>
#include <algorithm>
#include "../inc/Module-Misc/tools_util.h"
#include "..\\inc\\Module-Orders\\OrderInsertPanel.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/constants.h"
#include "../ConfigApp/Const.h"
#include <fstream>
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/gui/MainFrame.h"
#include "PositionPanel.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
using namespace std;

#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("PositionDetailInfoList", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


CMAPositionDetailInfoList *CMAPositionDetailInfoList::m_PostionDetailList = NULL;


BEGIN_EVENT_TABLE(CMAPositionDetailInfoList, wxExtListCtrl)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryPositionDetailLast,  CMAPositionDetailInfoList::OnRspQryPositionDetailLast)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryMarginRate,  CMAPositionDetailInfoList::OnGetMarginRate)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnTrade,  CMAPositionDetailInfoList::OnRtnTrade)
//	EVT_COMMAND(wxID_ANY, wxEVT_RtnOrder,  CMAPositionDetailInfoList::OnRtnOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnDepthMarketData,  CMAPositionDetailInfoList::OnRtnDepthMarketData)
	EVT_LIST_EXT_CONTEXT_MENU(CMAPositionDetailInfoList::OnContext)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()







//----------下面是内部事件响应处理函数，在主线程中执行----------


void CMAPositionDetailInfoList::OnRspQryPositionDetailLast(wxCommandEvent& evt)
{
    DataRspQryInvestorPositionDetail RspQryData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspQryData,NULL,sizeof(RspQryData),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RspQryData.Head.pPlatformSvr);
	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

	std::string QryInstrumentID=std::string(RspQryData.QryInstrumentID);
	if(QryInstrumentID.empty())
	{
		ShowAll();
	}
	else
		UpdateInstrumentOfSvr(pSvr,QryInstrumentID,true);
}

//获得某些合约的保证金率，需要更新相应的数值
void CMAPositionDetailInfoList::OnGetMarginRate(wxCommandEvent& evt)
{
    DataRspQryInstrumentMarginRate MarginRate;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarginRate,NULL,sizeof(MarginRate),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
    if(MarginRate.RspInfoField.ErrorID!=0)
        return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(MarginRate.Head.pPlatformSvr);

	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

    std::string strInstru(MarginRate.InstrumentMarginRateField.InstrumentID);
    if(strInstru.empty())
        return;
	WXLOG_INFO("OnGetMarginRate(%s,%s)",pSvr->GetAccount().c_str(),strInstru.c_str());
	UpdateInstrumentOfSvr(pSvr,strInstru,false);
}

//成交回报
void CMAPositionDetailInfoList::OnRtnTrade(wxCommandEvent& evt)
{
    DataRtnTrade RtnTrade;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnTrade,NULL,sizeof(RtnTrade),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RtnTrade.Head.pPlatformSvr);

	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

    std::string strInstru(RtnTrade.TradeField.InstrumentID);
    if(strInstru.empty())
        return;
	UpdateInstrumentOfSvr(pSvr,strInstru,true);
}


//行情到来
void CMAPositionDetailInfoList::OnRtnDepthMarketData(wxCommandEvent& evt)
{
    DataRtnDepthMarketData MarketData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarketData,NULL,sizeof(MarketData),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;

	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(MarketData.Head.pPlatformSvr);

	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

    std::string strInstru(MarketData.DepthMarketDataField.InstrumentID);
    if(strInstru.empty())
        return;

	//set<IPlatformSingleSvr*>::iterator it=m_ppSvrSet->begin();

	//for (it;it!=m_ppSvrSet->end();++it)
	//{
		UpdateInstrumentOfSvr(pSvr,strInstru,false);
		//LOG_INFO("OnRtnDepthMarketData(%s-%s)",(*it)->GetAccount().c_str(),strInstru.c_str());
	//}
}






CMAPositionDetailInfoList::CMAPositionDetailInfoList(wxWindow *parent,
						   wxWindowID id,
						   const wxPoint& pos,
						   const wxSize& size,
						   long style,
						   const wxValidator& validator,
						   const wxString& name)
						   :wxExtListCtrl(parent, id, pos, size, style|wxLCEXT_MASK_SORT, validator, name)
{
    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PositionDetailInfoList.log");

    m_PostionDetailList=this;
    Init();
}



CMAPositionDetailInfoList::~CMAPositionDetailInfoList(void)
{
    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }
}

void CMAPositionDetailInfoList::ShowAll(IPlatformSingleSvr* pSvr)
{
	long curseq,RowNo;
	PlatformStru_PositionDetail* pDetail=NULL;
	std::vector<PlatformStru_PositionDetail> vecData;
	set<IPlatformSingleSvr*>::iterator it;
	for (it=PositionPanel::m_pSvrSet.begin();it!=PositionPanel::m_pSvrSet.end();++it)
	{
		if(pSvr&&pSvr!=*it)
			continue;
		(*it)->GetPositionDetails(vecData,curseq); 
		for (size_t i=0;i<vecData.size();++i)
		{
			RowNo=InsertItem(GetItemCount(),wxEmptyString);
			pDetail=new PlatformStru_PositionDetail();
			memcpy(pDetail,&(vecData[i]),sizeof(PlatformStru_PositionDetail));
			SetItemData(RowNo,(long)pDetail);
			UpdateCellFromItemData(RowNo);
		}
	}
}


//配置listctrl的参数
void CMAPositionDetailInfoList::Init(void)
{
	ClearAllItemsAndData();
	if(GetColumnCount()>0) DeleteAllColumns();
    m_ColumnID2FieldID.clear();

	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
    if(!pCfgMgr) return;

    LPLIST_TABLE_CFG pListCfg;
	pListCfg = pCfgMgr->GetListCfg(GID_POSITIONDETAIL);
    if(pListCfg!=NULL)
    {
	    //设置LIST界面属性
	    SetHeaderBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
	    SetHeaderForegroundColour(wxColor(pListCfg->HeadColor));
	    SetBackgroundColour(wxColor(pListCfg->BackgroundColor));
	    SetForegroundColour(wxColor(pListCfg->TextColor));
	    wxFont tmpfontLarge;
	    tmpfontLarge.SetNativeFontInfoUserDesc(pListCfg->szFont);
	    SetFont(tmpfontLarge);
    	
	    this->SetOddRowColour( wxColor(pListCfg->OddLineBackgroundColor));
	    this->SetEvenRowColour( wxColor(pListCfg->EvenLineBackgroundColor));
		m_TextColor=wxColor(pListCfg->TextColor);
    }

	wxExtListItem col;
    int i;
    wxExtListItem ColumnInfo; 

    //列的顺序按用户的配置顺序存储在配置信息中
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_POSITIONDETAIL);
    LPLIST_COLUMN_CFG pColCfg;
	for(i = 0; i < nColCount; i++)
	{
		pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITIONDETAIL, i);
        if(!pColCfg) continue;

		//"成交ID"栏，左对齐
		if(pColCfg->id == POSITION_DETAIL_FIELD_ID_TRADEID)   
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else if(pColCfg->id == POSITION_DETAIL_FIELD_ID_INSTRUMENT)
		{    //"合约ID"栏，左对齐                                               
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		}
		else //其余的，右对齐                                               
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

        col.SetText(pCfgMgr->GetColCfgCaption(GID_POSITIONDETAIL,pColCfg->id));
        col.SetWidth(pColCfg->Width);
        col.SetTextColour(pColCfg->TextColor);
		col.SetColData( pColCfg->id );

        InsertColumn(i, col); 

        m_ColumnID2FieldID[i] = pColCfg->id;

    }

    ShowAll();
}




//配置文件改变

void CMAPositionDetailInfoList::OnCfgChanged()
{
    WXLOG_INFO("CMAPositionDetailInfoList::OnCfgChanged");

    Init();
}

void CMAPositionDetailInfoList::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.AppendSeparator();
	menu.Append(ID_MENU_Statisticsofnumber,LOADSTRING(CONTEXTMENU_Statisticsofnumber));

	BOOL bIsRowCLick = evt.GetId();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏
	if ( bIsRowCLick )
	{
    	menu.Append(wxID_SEPARATOR);
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
			for ( int i = 0; i < GetItemCount(); i++)
			{
				if ( IsSelected( i ))
				{
					PlatformStru_PositionDetail* pItemData=(PlatformStru_PositionDetail*)GetItemData(i);
                    if(pItemData==NULL)
                        continue;
							string InstrumentId = pItemData->InstrumentID;
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
			    AutoAdjustColumnWidth();
			    SaveColWidth();
			    break;
		    case ID_MENU_COLUMNCONFIG://表格列设置
		    {
			    wxCommandEvent request_event(wxEVT_LOAD_CONFIG_APP);
			    request_event.SetInt( ID_ITEM_POSITIONDETAIL );
			    GETTOPWINDOW()->ProcessEvent(request_event);
    		    break;
		    }
		    case ID_MENU_EXPORT://导出列表
		    {
                wxString cap=LOADSTRING(Name_PositionDetail);
		        LIST_EXPORT_CSV_FILE(cap, this);
    		    break;
		    }
            case ID_MENU_Statisticsofnumber://数量统计
            {
                if(m_PostionDetailList)
                {
                    int count=m_PostionDetailList->GetItemCount();
                    char buf[512];
                    memset(buf,0,sizeof(buf));
                    strncpy(buf,LOADSTRING(STATISTICSOFNUMBER_PositionDetail),sizeof(buf)-50);
                    sprintf(buf+strlen(buf),"%d",count);

				    wxMessageBox(buf,LOADSTRING(USERLOGINDLG_INFORMATION),wxOK);
                }
                break;
            }
		    default:
			    break;
		}


    }
    evt.Skip();
}

void CMAPositionDetailInfoList::OnLanguageChanged( int langid)
{
	TRANSFORM_COLUMN_TEXT(GID_POSITIONDETAIL,this);

	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nRowCount = GetItemCount();
	for ( int nRow = 0; nRow < nRowCount; nRow++ )
	{
		//重新设置买卖方向，投保列
		for(int i = 0; i < pCfgMgr->GetListVisibleColCount(GID_POSITIONDETAIL); i++)
		{
			bool bReplace=true;
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_POSITIONDETAIL, i);
			string strValue=GetSubItemText(nRow, i);
			//语言替换
			if ( pColCfg->id == POSITION_DETAIL_FIELD_ID_DIRECTION)
			{
				strValue=CFieldValueTool::Direction2String(CFieldValueTool::String2Direction(strValue),langid);
			}
			else if(pColCfg->id == POSITION_DETAIL_FIELD_ID_HEDGEFLAG)
			{
				strValue =  CFieldValueTool::HedgeFlag2String(CFieldValueTool::String2HedgeFlag(strValue),langid);  
			}
			else if(pColCfg->id == POSITION_DETAIL_FIELD_ID_COMBSTATE)//组合状态
			{
				strValue =  CFieldValueTool::TradeType2String(CFieldValueTool::String2TradeType(strValue),langid);  
			}
			else if(pColCfg->id == POSITION_DETAIL_FIELD_ID_TYPE )//今仓，昨仓
			{
				strValue =  CFieldValueTool::TodayPosition2String(CFieldValueTool::String2TodayPosition(strValue),langid); 
			}
			else
			{
				bReplace=false;
			}
			if(bReplace) SetItem( nRow, i, strValue );
		}
	}
}


void CMAPositionDetailInfoList::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_POSITIONDETAIL ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITIONDETAIL, nCol);
	pColCfg->Width = GetColumnWidth( nCol );
}

void CMAPositionDetailInfoList::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nColCount = GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_POSITIONDETAIL ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITIONDETAIL, i);
		pColCfg->Width = GetColumnWidth( i );
	}
}
void CMAPositionDetailInfoList::OnMAPlatformAdd(wxCommandEvent& event)
{
	//ClearAllItemsAndData();
	ShowAll((IPlatformSingleSvr*)event.GetInt());
}
void CMAPositionDetailInfoList::OnMAPlatformDelete(wxCommandEvent& event)
{
	ClearAllItemsAndData();
	ShowAll();
}

void CMAPositionDetailInfoList::ClearAllItemsAndData()
{
	for (int row=0;row<GetItemCount();++row)
	{
		PlatformStru_PositionDetail* pItemData=(PlatformStru_PositionDetail*)GetItemData(row);
		SAFE_DELETE(pItemData);
	}
	DeleteAllItems();
}

void CMAPositionDetailInfoList::UpdateInstrumentOfSvr( IPlatformSingleSvr* pSvr, std::string strInstru, bool bRowsChanged )
{
	LOG_INFO("UpdateInstrumentOfSvr(%s-%s)",pSvr->GetAccount().c_str(),strInstru.c_str());
	std::set<long> setFTID;
	std::vector<PlatformStru_PositionDetail> vecValue;
	long lastseq;
	int vecsize=pSvr->GetPositionDetails(vecValue,lastseq);
	map<PositionDetailKey,PlatformStru_PositionDetail> key2index;
	map<PositionDetailKey,PlatformStru_PositionDetail>::iterator key2index_it;
	for (size_t i=0;i<vecsize;++i)
	{
		if(strInstru==vecValue[i].InstrumentID)
		{
			key2index.insert(make_pair(PositionDetailKey(vecValue[i]),vecValue[i]));
		}
	}
	PlatformStru_PositionDetail* pItemData=NULL;
	for (int row=GetItemCount()-1;row>=0;--row)
	{
		pItemData=(PlatformStru_PositionDetail*)GetItemData(row);
		if(pItemData && pSvr->GetAccount()==pItemData->szAccount && strInstru==pItemData->InstrumentID)
		{
			key2index_it=key2index.find(PositionDetailKey(*pItemData));
			if(key2index_it!=key2index.end())
			{
				memcpy(pItemData,&(key2index_it->second),sizeof(PlatformStru_PositionDetail));
				UpdateCellFromItemData(row);
				LOG_INFO("UpdateInstrumentOfSvr(%d)",row);
				key2index.erase(key2index_it);
			}
			else if(bRowsChanged)
			{
				SAFE_DELETE(pItemData);
				DeleteItem(row);
			}
		}
	}
	if(bRowsChanged)
	{
		int RowNo=0;
		for (key2index_it=key2index.begin();key2index_it!=key2index.end();++key2index_it)
		{
			RowNo=InsertItem(GetItemCount(),wxEmptyString);
			pItemData=new PlatformStru_PositionDetail();
			memcpy(pItemData,&(key2index_it->second),sizeof(PlatformStru_PositionDetail));
			SetItemData(RowNo,(long)pItemData);
			UpdateCellFromItemData(RowNo);
		}
	}
}
#undef	UpdateCol_String
#define UpdateCol_String(FieldName)  outItem.SetText(wxString(pNew->FieldName))
#undef	UpdateCol_Money
#define UpdateCol_Money(FieldName)  \
	if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
			else tmpstr.Printf("%.02f", pNew->FieldName ); \
			outItem.SetText(tmpstr)
#undef	UpdateCol_Int
#define UpdateCol_Int(FieldName)  \
	if(pNew->FieldName==util::GetIntInvalidValue()) tmpstr.Printf("-");\
			else tmpstr.Printf("%d",pNew->FieldName);\
			outItem.SetText(tmpstr)
#undef	UpdateCol_Fund
#define UpdateCol_Fund(FieldName)  \
	if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
			else tmpstr = GlobalFunc::GetAccountFormatString(pNew->FieldName, 2); \
			outItem.SetText(tmpstr)
#undef UpdateCol_IntInt
#define UpdateCol_IntInt(FieldName1,FieldName2)  \
	if(pNew->FieldName1==util::GetIntInvalidValue()||pNew->FieldName2==util::GetIntInvalidValue()) tmpstr.Printf("-");\
			else tmpstr.Printf("%d",(pNew->FieldName1+pNew->FieldName2));\
			outItem.SetText(tmpstr)
#undef UpdateCol_Price
#define UpdateCol_Price(FieldName)  \
	if(pNew->FieldName==util::GetIntInvalidValue()) outItem.SetText("-");\
			else outItem.SetText(Price2String(pNew->FieldName,pNew->PriceTick))

void CMAPositionDetailInfoList::UpdateCellFromItemData(int row)
{
	PlatformStru_PositionDetail* pNew=(PlatformStru_PositionDetail*)GetItemData(row);
	if(pNew==NULL) return;
	int FieldID=0;
	wxExtListItem outItem;
	wxString tmpstr;
	for (int ColID=0;ColID<GetColumnCount();++ColID)
	{
		outItem.SetColumn(ColID);
		outItem.SetId(row);
		outItem.SetTextColour(m_TextColor);
		FieldID=m_ColumnID2FieldID[ColID];
		switch(FieldID)
		{ 
		case POSITION_DETAIL_FIELD_ID_INSTRUMENT:
			UpdateCol_String(InstrumentID);
			break;
		case POSITION_DETAIL_FIELD_ID_HEDGEFLAG:
			outItem.SetText(CFieldValueTool::HedgeFlag2String(pNew->HedgeFlag,SVR_LANGUAGE));
			break;
		case POSITION_DETAIL_FIELD_ID_DIRECTION:
			outItem.SetText(CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
			outItem.SetTextColour(wxColor(pNew->Direction==THOST_FTDC_D_Buy?RGB(255,0,0):RGB(0,128,0)));
			break;
		case POSITION_DETAIL_FILED_ID_OPENTIME:
			UpdateCol_String(OpenDate);
			break;
		case POSITION_DETAIL_FIELD_ID_TRADEID:
			outItem.SetText(wxString(pNew->TradeID).Trim(true).Trim(false));
			break;
		case POSITION_DETAIL_FIELD_ID_VOLUME:
			UpdateCol_Int(Volume);
			break;
		case POSITION_DETAIL_FIELD_ID_OPENPRICE:
			UpdateCol_Price(OpenPrice);
			break;
		case POSITION_DETAIL_FIELD_ID_COMBINSTRUMENT:
			UpdateCol_String(CombInstrumentID);
			break;
		case POSITION_DETAIL_FIELD_ID_ACCOUNT:
			UpdateCol_String(szAccount);
			break;
		case POSITION_DETAIL_FIELD_ID_EXCHANGE:
			outItem.SetText(CFieldValueTool::ExchangeID2Name((char*)(pNew->ExchangeID)));
			break;
		case POSITION_DETAIL_FIEL_ID_CLOSEPROFIT:
			UpdateCol_Fund(CloseProfitByDate);
			if(pNew->CloseProfitByDate>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else if(pNew->CloseProfitByDate<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
			break;
		case POSITION_DETAIL_FIELD_ID_POSITIONPROFIT:
			UpdateCol_Fund(PositionProfitByDate);
			if(pNew->PositionProfitByDate>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else if(pNew->PositionProfitByDate<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
			break;
		case POSITION_DETAIL_FIELD_FLUCTUATION:
			UpdateCol_Fund(PositionProfitByTrade);
			if(pNew->PositionProfitByTrade>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
			else if(pNew->PositionProfitByTrade<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
			break;
		case POSITION_DETAIL_FIELD_ID_MARGIN:
			UpdateCol_Fund(Margin);
			break;
		case POSITION_DETAIL_FIELD_ID_LASTSETTLEMENTPRICE:
			UpdateCol_Price(LastSettlementPrice);
			break;
		case POSITION_DETAIL_FIELD_CURPRICE:
			UpdateCol_Price(CurPrice);
			break;
		case POSITION_DETAIL_FIELD_ID_CLOSEVOLUME:
			UpdateCol_Int(CloseVolume);
			break;
		case POSITION_DETAIL_FIELD_ID_TYPE:
			{
				outItem.SetText(((pNew->bToday)?LOADSTRING(TODAY_POSITION):LOADSTRING(PRE_POSITION)));
				break;
			}
		case POSITION_DETAIL_FIELD_ID_COMBSTATE:
			outItem.SetText(CFieldValueTool::TradeType2String(pNew->TradeType,SVR_LANGUAGE));
			break;
		}
		SetItem(outItem);
	}
}

bool CMAPositionDetailInfoList::ReQry()
{
	ClearAllItemsAndData();
	ShowAll();
	return false;
}


