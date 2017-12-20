//  Name: 持仓模块
//  Desc:
//          1. 处理持仓模块的显示
//          2. 将右键菜单的命令通过顶层窗口传递给下单模块
//  Interface:
//          底层模块platformSvr
//              1. 订阅BID_RspQryInvestorPosition
//              2. 订阅BID_RspQryInstrumentMarginRate
//              3. 订阅BID_RtnTrade
//              4. 订阅BID_RtnOrder
//              5. 订阅BID_RtnDepthMarketData
//              6. DEFAULT_SVR()->GetPositionKeySet()-获取持仓记录键值列表,返回持仓记录的数量
//              7. DEFAULT_SVR()->GetPositionData()-获取指定合约、方向、投保的持仓数据
//              8. DEFAULT_SVR()->GetCurPrice()-获取指定合约的现价
//          配置模块CfgMgr
//              1. CfgMgr::GetInstance()->GetListCfg(GID_POSITION)
//              2. CfgMgr::GetInstance()->GetListColCount(GID_POSITION)
//              3. CfgMgr::GetInstance()->GetListColCfg(GID_POSITION, i);
//          配置程序ConfigApp
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_LOAD_CONFIG_APP) - 表格列设置
//          报价表模块
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_GRID_INSTRUMENTID_CHANGED) - 当前合约设置
//          下单板模块
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_ORDERINSERT_ORDERCHANGE) - 市价反手
//              2. GETTOPWINDOW()->ProcessEvent(wxEVT_ORDERINSERT_MOUSECLOSE) - 鼠标快速平仓
//           移仓模块
//				1.接收CPositionInfoPanel的移仓命令，调用CMovePositionDlg
//				2.接收CPositionInfoPanel的换仓命令，调用CSwapPositionDlg
//************************************************************************************
#include "StdAfx.h"
#include "PositionInfoList.h"

#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Orders/OrderInsertPanel.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-MovePosition/CMovePositionDlg.h"
#include "../inc/Module-MovePosition/CSwapPositionDlg.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/globalDefines.h"
#include "../ConfigApp/Const.h"
#include "../Module-Misc/orderCommonFunc.h"
#include "../inc/gui/MainFrame.h"
#include "../inc/Plusin/Plusin.h"
#include "PositionPanel.h"
#include <fstream>

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
extern CPlusin g_Plusin;
using namespace std;

BEGIN_EVENT_TABLE(CPositionInfoList, wxExtListCtrl)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryPositionLast,  CPositionInfoList::OnRspQryPositionLast)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryMarginRate,  CPositionInfoList::OnGetMarginRate)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnTrade,  CPositionInfoList::OnRtnTrade)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnOrder,  CPositionInfoList::OnRtnOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnDepthMarketData,  CPositionInfoList::OnRtnDepthMarketData)
	EVT_LIST_EXT_CONTEXT_MENU(CPositionInfoList::OnContext)
    EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, CPositionInfoList::OnActivated)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, CPositionInfoList::OnSelected)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()

CPositionInfoList *CPositionInfoList::m_PostionList = NULL;


#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPositionInfoList", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif



//----------下面是内部事件响应处理函数，在主线程中执行----------


void CPositionInfoList::OnRspQryPositionLast(wxCommandEvent& evt)
{
    DataRspQryInvestorPosition RspQryData;
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
    {
        std::set<long> setFTID;
        std::vector<PlatformStru_Position> vecData;

        std::map<std::string,long>::iterator it_lastseq;
        it_lastseq=m_PositionDataLastUpdateSeq.find(QryInstrumentID);
        if(it_lastseq==m_PositionDataLastUpdateSeq.end())
        {
            m_PositionDataLastUpdateSeq.insert(std::make_pair(QryInstrumentID,0));
            it_lastseq=m_PositionDataLastUpdateSeq.find(QryInstrumentID);
        }
        long lastseq=it_lastseq->second;
        
        //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
        int irlt=pSvr->GetPositions2(QryInstrumentID,setFTID,vecData,lastseq);

        if(irlt==0)
        {
            it_lastseq->second=lastseq;
            UpdateInstrumentItems2(QryInstrumentID,setFTID,vecData);
        }
    }
}

//获得某些合约的保证金率，需要更新相应的数值
void CPositionInfoList::OnGetMarginRate(wxCommandEvent& evt)
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

    std::set<long> setFTID;
    std::vector<PlatformStru_Position> vecData;

    std::map<std::string,long>::iterator it_lastseq;
    it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    if(it_lastseq==m_PositionDataLastUpdateSeq.end())
    {
        m_PositionDataLastUpdateSeq.insert(std::make_pair(strInstru,0));
        it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    }
    long lastseq=it_lastseq->second;
    
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
    int irlt=pSvr->GetPositions2(strInstru,setFTID,vecData,lastseq);

    if(irlt==0)
    {
        it_lastseq->second=lastseq;
        UpdateInstrumentItems2(strInstru,setFTID,vecData);
    }
}

//成交回报
void CPositionInfoList::OnRtnTrade(wxCommandEvent& evt)
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

    std::set<long> setFTID;
    std::vector<PlatformStru_Position> vecData;

    std::map<std::string,long>::iterator it_lastseq;
    it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    if(it_lastseq==m_PositionDataLastUpdateSeq.end())
    {
        m_PositionDataLastUpdateSeq.insert(std::make_pair(strInstru,0));
        it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    }
    long lastseq=it_lastseq->second;
    
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
    int irlt=pSvr->GetPositions2(strInstru,setFTID,vecData,lastseq);
    LOG_INFO("CPositionInfoList::OnRtnTrade strInstru=%s irlt=%d lastseq=%u curseq=%u rltcount=%d",strInstru.c_str(),irlt,it_lastseq->second,lastseq,vecData.size());

    if(irlt==0)
    {
        it_lastseq->second=lastseq;
        UpdateInstrumentItems2(strInstru,setFTID,vecData);
    }
}

//报单回报
void CPositionInfoList::OnRtnOrder(wxCommandEvent& evt)
{
    DataRtnOrder RtnOrder;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnOrder,NULL,sizeof(RtnOrder),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RtnOrder.Head.pPlatformSvr);

	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

    std::string strInstru(RtnOrder.OrderField.InstrumentID);
    if(strInstru.empty())
        return;

    std::set<long> setFTID;
    std::vector<PlatformStru_Position> vecData;

    std::map<std::string,long>::iterator it_lastseq;
    it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    if(it_lastseq==m_PositionDataLastUpdateSeq.end())
    {
        m_PositionDataLastUpdateSeq.insert(std::make_pair(strInstru,0));
        it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    }
    long lastseq=it_lastseq->second;
    
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
    int irlt=pSvr->GetPositions2(strInstru,setFTID,vecData,lastseq);

    if(irlt==0)
    {
        it_lastseq->second=lastseq;
        UpdateInstrumentItems2(strInstru,setFTID,vecData);
    }
}


//行情到来
void CPositionInfoList::OnRtnDepthMarketData(wxCommandEvent& evt)
{
    DataRtnDepthMarketData MarketData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarketData,NULL,sizeof(MarketData),NULL))
        CEventParam::DeleteEventParam(EvtParamID);
    else 
        return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(MarketData.Head.pPlatformSvr);

	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

    std::string strInstru(MarketData.DepthMarketDataField.InstrumentID);
    if(strInstru.empty())
        return;

    static bool bgo=true;
    if(!bgo) return;

    std::set<long> setFTID;
    std::vector<PlatformStru_Position> vecData;

    std::map<std::string,long>::iterator it_lastseq;
    it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    if(it_lastseq==m_PositionDataLastUpdateSeq.end())
    {
        m_PositionDataLastUpdateSeq.insert(std::make_pair(strInstru,0));
        it_lastseq=m_PositionDataLastUpdateSeq.find(strInstru);
    }
    long lastseq=it_lastseq->second;
    
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
    int irlt=pSvr->GetPositions2(strInstru,setFTID,vecData,lastseq);

    if(irlt==0)
    {
        it_lastseq->second=lastseq;
        UpdateInstrumentItems2(strInstru,setFTID,vecData);
    }
}





CPositionInfoList::CPositionInfoList(wxWindow *parent,
									   wxWindowID id,
									   const wxPoint& pos,
									   const wxSize& size,
									   long style,
									   const wxValidator& validator,
									   const wxString& name)
                                       :CDataListCtrl<PositionKey,PlatformStru_Position>(UpdateListItemCallBack,parent, id, pos, size, style|wxLCEXT_MASK_SORT, validator, name)
{
    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PositionInfoList.log");

	m_pMovePositionDlg=NULL;
	m_pSwapPositionDlg=NULL;

    m_PostionList=this;
    Init();

    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }
}

CPositionInfoList::~CPositionInfoList(void)
{
	SAFE_DELETE(m_pMovePositionDlg)
	SAFE_DELETE(m_pSwapPositionDlg)
	CMovePositionServiceThread::ReleaseInstance();

}

void CPositionInfoList::ShowAll()
{
	std::vector<PlatformStru_Position> vecData,all;
	set<IPlatformSingleSvr*>::iterator it;
	for (it=PositionPanel::m_pSvrSet.begin();it!=PositionPanel::m_pSvrSet.end();++it)
	{
		(*it)->GetPositions(vecData);
		all.insert(all.end(),vecData.begin(),vecData.end());
	}
    UpdateAllItems(all);
}


void CPositionInfoList::Init()
{
	ClearAll();
	if(GetColumnCount()>0) DeleteAllColumns();
	m_FieldID2ColumnID.clear();

    LPLIST_TABLE_CFG pListCfg;
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
    if(!pCfgMgr) return;

	pListCfg = pCfgMgr->GetListCfg(GID_POSITION);
    if(pListCfg)
    {
	    //设置LIST界面属性
	    SetBackgroundColour(wxColor(pListCfg->BackgroundColor));
	    SetHeaderBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
	    SetHeaderForegroundColour(wxColor(pListCfg->HeadColor));//白  16777215
	    SetForegroundColour(wxColor(pListCfg->TextColor));
	    wxFont tmpfontLarge;
	    tmpfontLarge.SetNativeFontInfoUserDesc(pListCfg->szFont);
	    SetFont(tmpfontLarge);
    	
	    this->SetOddRowColour( wxColor(pListCfg->OddLineBackgroundColor));
	    this->SetEvenRowColour( wxColor(pListCfg->EvenLineBackgroundColor));

        m_TextColor=pListCfg->TextColor;

    }


	wxExtListItem col;
    int i;
    wxExtListItem ColumnInfo; 
    PositionMAP::iterator it;
    LPLIST_COLUMN_CFG pColCfg;

    //列的顺序按用户的配置顺序存储在配置信息中
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_POSITION);
	for(i = 0;  i < nColCount; i++)
	{
		pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITION, i);
        if(!pColCfg) continue;

		if(pColCfg->id == POSITION_FILED_ID_INSTRUMENTID)       //合约字段 左对齐
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else                                                //其余右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
		

        col.SetText(pCfgMgr->GetColCfgCaption(GID_POSITION,pColCfg->id));
        col.SetWidth(pColCfg->Width);
        col.SetTextColour(pColCfg->TextColor);
		col.SetColData( pColCfg->id );

        InsertColumn(i, col); 

        m_FieldID2ColumnID[pColCfg->id] = i;

    }

    SetCfg(m_TextColor,m_FieldID2ColumnID);

    ShowAll();
}







std::string CPositionInfoList::GetItemText(int row, int col)
{
	wxExtListItem item;
	item.m_itemId = row;
	item.m_col = col;
	if(GetItem(item))
	{
		return std::string(item.m_text);	
	}
	return "";
}

//配置文件改变
void CPositionInfoList::OnCfgChanged()
{
    Init();
}

void CPositionInfoList::OnContext(wxContextMenuEvent& evt)
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
			for ( int i = 0; i < GetItemCount(); i++)
			{
				if ( IsSelected( i ))
				{
					PlatformStru_Position position;
                    if(!GetValueByRow(i,position))
                        continue;

					string InstrumentId = position.InstrumentID;
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
		case ID_MENU_PRICEBACKHAND://市价反手，和快捷锁仓操作一样
		case ID_MENU_FASTLOCK://快捷锁仓
			for(int row=0;row<GetItemCount();row++)
			{
				if(IsSelected(row))
				{
					USERLOG_INFO("用户选择持仓列表菜单项[%s]\n\"\n%s\n\"", 
							(retcmd==ID_MENU_PRICEBACKHAND?"市价反手":"快捷锁仓"), 
							GetListCtrlRowLog(this, row).c_str());

					PlatformStru_Position PositionData;
                    if(GetValueByRow(row,PositionData))
					{
						wxCommandEvent request_event(wxEVT_ORDERINSERT_ORDERCHANGE);
						wxString strCmsStream;
						strCmsStream.Printf("%s,%s,%c,%c",PositionData.szAccount,PositionData.InstrumentID,PositionData.PosiDirection,PositionData.HedgeFlag);

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
                wxString cap=LOADSTRING(Name_Position);
			    LIST_EXPORT_CSV_FILE(cap, this);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
			{
				wxCommandEvent request_event(wxEVT_LOAD_CONFIG_APP);
				request_event.SetInt( ID_ITEM_POSITION );
				GETTOPWINDOW()->ProcessEvent(request_event);
			}
			break;
		case ID_MENU_FASTCLOSE: // 快捷平仓
			{
				for(int row=0;row<GetItemCount();row++)
				{
					if(IsSelected(row))
					{
						USERLOG_INFO("用户选择持仓列表菜单项[快捷平仓]\n\"\n%s\n\"", 
								GetListCtrlRowLog(this, row).c_str());

						PlatformStru_Position OrgStru;

                        if(GetValueByRow(row,OrgStru))
						{
							PlatformStru_Position* pStru=new PlatformStru_Position();
							memcpy(pStru,&OrgStru,sizeof(PlatformStru_Position));

							wxCommandEvent sendchangeevt(wxEVT_GRID_INSTRUMENTID_CHANGED);

                            DWORD EvtParamID;
                            std::string EvtParamStr(pStru->InstrumentID);
                            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,1);
                            sendchangeevt.SetInt((int)EvtParamID);

                            GETTOPWINDOW()->ProcessEvent(sendchangeevt);

							wxCommandEvent evt(wxEVT_ORDERINSERT_MOUSECLOSE);
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

wxString CPositionInfoList::GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow)
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

void CPositionInfoList::OnActivated(wxExtListEvent& event)
{
	int row=event.GetIndex();
    PlatformStru_Position OrgStru;

    if(GetValueByRow(row,OrgStru))
	{
		USERLOG_INFO("用户双击持仓列表\n\"\n%s\n\"", 
				GetListCtrlRowLog(this, row).c_str());

		PlatformStru_Position* pStru=new PlatformStru_Position();
		memcpy(pStru,&OrgStru,sizeof(PlatformStru_Position));

		wxCommandEvent evt(wxEVT_ORDERINSERT_MOUSECLOSE);
		evt.SetInt(2);
		evt.SetClientData(pStru);
		GETTOPWINDOW()->ProcessEvent(evt);
	}
	else
	{
		WXLOG_INFO("Can't find position by row(%d) is NULL",row);
	}
}

void CPositionInfoList::OnSelected(wxExtListEvent& event)
{
	int row=event.GetIndex();
    PlatformStru_Position OrgStru;

    if(GetValueByRow(row,OrgStru))
    {
		USERLOG_INFO("用户单击持仓列表\n\"\n%s\n\"", 
				GetListCtrlRowLog(this, row).c_str());

		PlatformStru_Position* pStru=new PlatformStru_Position();
		memcpy(pStru,&OrgStru,sizeof(PlatformStru_Position));

		if ( DEFAULT_SVR() && DEFAULT_SVR()->SupportPlugins() )
		{
			g_Plusin.SetOrderModuleCurInstrumentID(pStru->InstrumentID);
		}

		wxCommandEvent evt(wxEVT_ORDERINSERT_MOUSECLOSE);
		evt.SetInt(1);
		evt.SetClientData(pStru);
		GETTOPWINDOW()->ProcessEvent(evt);
	}
	else
	{
		WXLOG_INFO("Can't find position by row(%d) is NULL",row);
	}
}
//移仓
void CPositionInfoList::OnMovePosition()
{
    PlatformStru_Position tPositionData;

	int SelectItemId = GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

    if(!GetValueByRow(SelectItemId, tPositionData))
        return;

    if(m_pMovePositionDlg==NULL) 
    {
		m_pMovePositionDlg=new CMovePositionDlg(GETTOPWINDOW());
	}
	
	m_pMovePositionDlg->SetPosition(tPositionData);
	m_pMovePositionDlg->Init(CMovePositionServiceThread::GetInstance());
	m_pMovePositionDlg->Show();

}
//仓位互换
void CPositionInfoList::OnSwapPosition()
{
	PlatformStru_Position tPositionData1;
	PlatformStru_Position tPositionData2;

	int SelectItemId = GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
		return;
    }

    if(!GetValueByRow(SelectItemId, tPositionData1))
        return; 

    if(m_pSwapPositionDlg==NULL) 
    {
		m_pSwapPositionDlg=new CSwapPositionDlg(GETTOPWINDOW());
	}

	m_pSwapPositionDlg->SetPosition(tPositionData1);

    if(!DEFAULT_SVR())
        return;
    std::vector<PlatformStru_Position> vecPositions;
    DEFAULT_SVR()->GetPositions(vecPositions);
	m_pSwapPositionDlg->Init(CMovePositionServiceThread::GetInstance(), vecPositions);
	m_pSwapPositionDlg->Show();
}
void CPositionInfoList::OnUpdatePosMoveSwapSwap(wxCommandEvent& evt)
{
	int SelectItemId = GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	if(SelectItemId<0)//没有选中
		return;
	PlatformStru_Position tPositionData1;

    if(!GetValueByRow(SelectItemId, tPositionData1))
        return;

    if(m_pMovePositionDlg&&m_pMovePositionDlg->IsShown())
		return;
    if(m_pSwapPositionDlg&&m_pSwapPositionDlg->IsShown())
		return;
	evt.SetInt(1);
}

void CPositionInfoList::OnLanguageChanged(int langid)
{
	TRANSFORM_COLUMN_TEXT(GID_POSITION,this);

	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nRowCount = GetItemCount();
	for ( int nRow = 0; nRow < nRowCount; nRow++ )
	{
		//重新设置买卖方向，投保列
		for(int i = 0; i < pCfgMgr->GetListVisibleColCount(GID_POSITION); i++)
		{
			bool bReplace=true;
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_POSITION, i);
			std::string strValue=GetSubItemText( nRow, i);
			//语言替换
			if ( pColCfg->id == POSITION_FILED_ID_DIRECTION)
			{
				strValue=CFieldValueTool::PosiDirection2String(CFieldValueTool::String2PosiDirection(strValue),langid);
			}
			else if(pColCfg->id == POSITION_FIELD_ID_INSURE )
			{
				strValue =  CFieldValueTool::HedgeFlag2String(CFieldValueTool::String2HedgeFlag(strValue),langid);  
			}
			else
			{
				bReplace=false;
			}
			if(bReplace) SetItem( nRow, i, strValue );

		}
	}
}

void CPositionInfoList::OnColumnResize( wxExtListEvent& event )
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
	pColCfg->Width = GetColumnWidth( nCol );
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
            outItem.SetText(wxString(DispName));
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
#define UpdateCol_IntInt(FieldName1,FieldName2)  \
            if(!bnewline&&pOld->FieldName1==pNew->FieldName1&&pOld->FieldName2==pNew->FieldName2) \
                return false;\
            if(pNew->FieldName1==util::GetIntInvalidValue()||pNew->FieldName2==util::GetIntInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%d",(pNew->FieldName1+pNew->FieldName2));\
            outItem.SetText(tmpstr);\
            outItem.SetiColData(pNew->FieldName1+pNew->FieldName2);
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

bool CPositionInfoList::UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_Position* pNew=(const PlatformStru_Position*)pNewData;
    PlatformStru_Position* pOld=(PlatformStru_Position*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    outItem.SetTextColour(wxColor(textcol));

    switch(FieldID)
    {
        case POSITION_FILED_ID_INSTRUMENTID:
            if(!bnewline) return false;
            UpdateCol_String(InstrumentID);
            return true;
        case POSITION_FILED_ID_DIRECTION:
            UpdateCol_Number(PosiDirection,CFieldValueTool::PosiDirection2String(pNew->PosiDirection,SVR_LANGUAGE));
            outItem.SetTextColour(wxColor(pNew->PosiDirection==THOST_FTDC_PD_Long?RGB(255,0,0):RGB(0,128,0)));
            return true;
        case POSITION_FIELD_ID_INSURE:
            UpdateCol_Number(HedgeFlag,CFieldValueTool::HedgeFlag2String(pNew->HedgeFlag,SVR_LANGUAGE));
            return true;
        case POSITION_FIELD_ID_EXCHANGE:
            if(!bnewline) return false;
            UpdateCol_String3(ExchangeID,CFieldValueTool::ExchangeID2Name((char*)pNew->ExchangeID));
            return true;
        case POSITION_FIELD_ID_CURRENCY:
            if(!bnewline&&memcmp(pOld->InstrumentID,pNew->InstrumentID,sizeof(pOld->InstrumentID))==0) 
                return false;
            outItem.SetText(wxString(pNew->Currency));
            return true;
        case POSITION_FIELD_ID_OPENAVGPRICE:
            UpdateCol_Money(AveragePositionPrice);
            return true;
        case POSITION_FIELD_ID_OPENPROFIT:
            UpdateCol_Money(PositionProfit);
            return true;
        case POSITION_FIELD_ID_BUYVOLUMN:
            //UpdateCol_Int(pats_BuyVolumn);
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Long||pOld->Position==pNew->Position))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Long)
            {
                if(pNew->Position==util::GetIntInvalidValue()) tmpstr.Printf("-");
                else tmpstr.Printf("%d",pNew->Position);
                outItem.SetText(tmpstr);
                outItem.SetiColData(pNew->Position);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetiColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_SELVOLUMN:
            //UpdateCol_Int(CloseVolume);
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Short||pOld->Position==pNew->Position))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Short)
            {
                if(pNew->Position==util::GetIntInvalidValue()) tmpstr.Printf("-");
                else tmpstr.Printf("%d",pNew->Position);
                outItem.SetText(tmpstr);
                outItem.SetiColData(pNew->Position);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetiColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_TOTALPROFIT:
            UpdateCol_Money(TotalPositionProfitByDate);
            return true;
        case POSITION_FIELD_ID_CLOSEPROFIT:
            UpdateCol_Money(CloseProfit);
            return true;
        case POSITION_FIELD_ID_ACCOUNT:
            if(!bnewline) return false;
            UpdateCol_String(szAccount);
            return true;
        case POSITION_FIELD_ID_CLOSEVOLUMN:
            UpdateCol_Int(CanCloseVolume);
            return true;
        case POSITION_FIELD_ID_PREPOSITION:
            UpdateCol_Int(YdPositionRemain);
            return true;
        case POSITION_FIELD_ID_TOTALTODAYCLOSEVOLUME:
            UpdateCol_Int(TodayPosition);
            return true;
        case POSITION_FIELD_ID_TODAYENABLECLOSEVOLUME:
            UpdateCol_Int(CanCloseTodayVolume);
            return true;
        case POSITION_FIELD_ID_TOTALPRECLOSEVOLUME:
            UpdateCol_Int(YdPositionRemain);
            return true;
        case POSITION_FIELD_ID_PREENABLECLOSEVOLUME:
            UpdateCol_Int(CanCloseydVolume);
            return true;
        case POSITION_FIELD_ID_TOTAL_PROFIT:
            UpdateCol_Fund(TotalPositionProfitByDate);
            if(pNew->TotalPositionProfitByDate>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
            else if(pNew->TotalPositionProfitByDate<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
            return true;
        case POSITION_FILED_ID_TOTALPOSITION:
            UpdateCol_Int(Position);
            return true;
        case POSITION_FIELD_ID_TOTALBUYPOSITION:
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Long||pOld->Position==pNew->Position))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Long)
            {
                if(pNew->Position==util::GetIntInvalidValue()) tmpstr.Printf("-");
                else tmpstr.Printf("%d",pNew->Position);
                outItem.SetText(tmpstr);
                outItem.SetiColData(pNew->Position);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetiColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_TOTALSELLPOSITION:
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Short||pOld->Position==pNew->Position))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Short)
            {
                if(pNew->Position==util::GetIntInvalidValue()) tmpstr.Printf("-");
                else tmpstr.Printf("%d",pNew->Position);
                outItem.SetText(tmpstr);
                outItem.SetiColData(pNew->Position);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetiColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_TODAYPOSITION:
            UpdateCol_Int(TodayPosition);
            return true;
        case POSITION_FIELD_ID_TODAYBUYPOSITION:
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Long||pOld->TodayPosition==pNew->TodayPosition))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Long)
            {
                if(pNew->TodayPosition==util::GetIntInvalidValue()) tmpstr.Printf("-");
                else tmpstr.Printf("%d",pNew->TodayPosition);
                outItem.SetText(tmpstr);
                outItem.SetiColData(pNew->TodayPosition);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetiColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_TODAYSELLPOSITION:
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Short||pOld->TodayPosition==pNew->TodayPosition))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Short)
            {
                if(pNew->TodayPosition==util::GetIntInvalidValue()) tmpstr.Printf("-");
                else tmpstr.Printf("%d",pNew->TodayPosition);
                outItem.SetText(tmpstr);
                outItem.SetiColData(pNew->TodayPosition);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetiColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_TODAYOPENVOLUME:
            UpdateCol_Int(OpenVolume);
            return true;
        case POSITION_FIELD_ID_TODAYCLOSEVOLUME:
            UpdateCol_Int(CloseVolume);
            return true;
        case POSITION_FIELD_ID_WAITCLOSE:       //平仓挂单量，指平昨
            UpdateCol_IntInt(CloseYesterdayOrderVolume,CloseOrderVolumeYdPart);
            return true;
        case POSITION_FIELD_ID_TODAYWAITCLOSE:  //平今挂单量
            UpdateCol_IntInt(CloseTodayOrderVolume,CloseOrderVolumeTdPart);
            return true;
        case POSITION_FIELD_ID_AVGPRICE:
            UpdateCol_Money(AveragePositionPrice);
            return true;
        case POSITION_FIELD_ID_AVGOPENPRICE:
            UpdateCol_Money(AverageOpenPrice);
            return true;
        case POSITION_FIELD_ID_BUYOPENPRICE:
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Long||pOld->AverageOpenPrice==pNew->AverageOpenPrice))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Long)
            {
                UpdateCol_Money(AverageOpenPrice);
                outItem.SetdColData(pNew->AverageOpenPrice);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetdColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_SELLOPENPRICE:
            if(!bnewline&&(pNew->PosiDirection!=THOST_FTDC_PD_Short||pOld->AverageOpenPrice==pNew->AverageOpenPrice))
                return false;
            if(pNew->PosiDirection==THOST_FTDC_PD_Short)
            {
                UpdateCol_Money(AverageOpenPrice);
                outItem.SetdColData(pNew->AverageOpenPrice);
            }
            else 
            {
                outItem.SetText(wxString("-"));
                outItem.SetdColData(util::GetIntInvalidValue());
            }
            return true;
        case POSITION_FIELD_ID_PROFIT:
            UpdateCol_Fund(PositionProfit);
            if(pNew->PositionProfit>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
            else if(pNew->PositionProfit<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
            return true;
        case POSITION_FIELD_ID_FLOATPROFIT:
            UpdateCol_Fund(PositionProfitByTrade);
            if(pNew->PositionProfitByTrade>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
            else if(pNew->PositionProfitByTrade<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
            return true;
        case POSITION_FIELD_ID_MARGIN:
            UpdateCol_Fund(UseMargin);
            return true;
        case POSITION_FIELD_ID_LASTPRICE:
            UpdateCol_Price(CurPrice);
            return true;
		case POSITION_FIELD_ID_MARGINPERLOT:
            UpdateCol_Money(pats_MarginPerLot);
			return true;
		case POSITION_FIELD_ID_PLBURNRATE:
            UpdateCol_Money(pats_PLBurnRate);
			return true;
		case POSITION_FIELD_ID_OPENPOSITIONEXPOSURE:
            UpdateCol_Money(pats_OpenPositionExposure);
			return true;
		case POSITION_FIELD_ID_BUYPOWERREMAIN:
            UpdateCol_Money(pats_BuyPowerRemain);
			return true;
		case POSITION_FIELD_ID_BUYPOWERUSED:
            UpdateCol_Money(pats_BuyPowerUsed);
			return true;
		case POSITION_FIELD_ID_TOTALMARGINPAID:
            UpdateCol_Money(pats_TotalMarginPaid);
			return true;
	}
    return false;
}

void CPositionInfoList::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nColCount = GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_POSITION ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITION, i);
		pColCfg->Width = GetColumnWidth( i );
	}
}

void CPositionInfoList::OnMAPlatformAdd(wxCommandEvent& event)
{
	Clear();
	ShowAll();
}
void CPositionInfoList::OnMAPlatformDelete(wxCommandEvent& event)
{
	Clear();
	ShowAll();
}

