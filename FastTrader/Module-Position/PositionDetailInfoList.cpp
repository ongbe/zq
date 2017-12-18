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
#include "PositionDetailInfoList.h"

#include "time.h"
#include <fstream>
#include <algorithm>
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include "../inc/Module-Misc/constants.h"
#include "../ConfigApp/Const.h"
#include <fstream>
#include "../inc/Module-Misc/orderCommonFunc.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace std;

#if 1
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


CPositionDetailInfoList *CPositionDetailInfoList::m_PostionDetailList = NULL;


BEGIN_EVENT_TABLE(CPositionDetailInfoList, wxExtListCtrl)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryPositionDetailLast,  CPositionDetailInfoList::OnRspQryPositionDetailLast)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryPositionDetailCombLast,  CPositionDetailInfoList::OnRspQryPositionDetailCombLast)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryMarginRate,  CPositionDetailInfoList::OnGetMarginRate)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnTrade,  CPositionDetailInfoList::OnRtnTrade)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnDepthMarketData,  CPositionDetailInfoList::OnRtnDepthMarketData)
	EVT_LIST_EXT_CONTEXT_MENU(CPositionDetailInfoList::OnContext)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()






//----------下面是内部事件响应处理函数，在主线程中执行----------


void CPositionDetailInfoList::OnRspQryPositionDetailLast(wxCommandEvent& evt)
{
    DataRspQryInvestorPositionDetail RspQryData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspQryData,NULL,sizeof(RspQryData),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

    string QryInstrumentID(RspQryData.QryInstrumentID);
    string strAccount(RspQryData.InvestorPositionDetailField.Account);
    if(QryInstrumentID.empty())
    {
        ShowAll();
    }
    else
    {
        UpdateByInstrument_Internal(strAccount,QryInstrumentID);
    }
}

void CPositionDetailInfoList::OnRspQryPositionDetailCombLast(wxCommandEvent& evt)
{
    DataRspQryInvestorPositionCombDetail RspQryData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&RspQryData,NULL,sizeof(RspQryData),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

    string QryInstrumentID=string(RspQryData.QryInstrumentID);
    if(QryInstrumentID.empty())
    {
        ShowAll();
    }
    else
    {
        //QryInstrumentID只能是单一合约
        UpdateByInstrument_Internal(string(RspQryData.InvestorPositionCombineDetailField.Account),QryInstrumentID);
    }
}

//获得某些合约的保证金率，需要更新相应的数值
void CPositionDetailInfoList::OnGetMarginRate(wxCommandEvent& evt)
{
    DataRspQryInstrumentMarginRate MarginRate;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarginRate,NULL,sizeof(MarginRate),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;
    if(MarginRate.RspInfoField.ErrorID!=0)
        return;

    std::string strInstru(MarginRate.InstrumentMarginRateField.InstrumentID);
    if(strInstru.empty())
        return;

    if(g_pPlatformMgr)
    {
        vector<string> LogonAccounts=g_pPlatformMgr->GetLogonAccounts();
        for(int i=0;i<(int)LogonAccounts.size();i++)
            UpdateByInstrument_Internal(LogonAccounts[i],strInstru);
    }
}

//成交回报
void CPositionDetailInfoList::OnRtnTrade(wxCommandEvent& evt)
{
    DataRtnTrade RtnTrade;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnTrade,NULL,sizeof(RtnTrade),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

    if(!g_pPlatformMgr) return;
    string strAccount(RtnTrade.TradeField.Account);
    IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(strAccount);
    if(!pSvr) return;

    set<string> refreshInstruments;
    if(RtnTrade.TradeField.OffsetFlag==THOST_FTDC_OF_Close||
       RtnTrade.TradeField.OffsetFlag==THOST_FTDC_OF_ForceClose||
       RtnTrade.TradeField.OffsetFlag==THOST_FTDC_OF_CloseToday||
       RtnTrade.TradeField.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
       RtnTrade.TradeField.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
    {
        //平仓成交可能会打散组合持仓明细，从而衍生出对手普通持仓明细，需要更新
        pSvr->GetDerivedInstrumentID_OnCloseTrade(refreshInstruments);
    }

    //将成交对应的合约放到refreshInstruments中
    string strInstru(RtnTrade.TradeField.InstrumentID);
    if(refreshInstruments.find(strInstru)==refreshInstruments.end())
        refreshInstruments.insert(strInstru);

    //更新指定合约的持仓明细
    for(set<string>::const_iterator it=refreshInstruments.begin();it!=refreshInstruments.end();it++)
    {
        if(!it->empty())
            UpdateByInstrument_Internal(string(RtnTrade.TradeField.Account),*it);
    }
}


//行情到来
void CPositionDetailInfoList::OnRtnDepthMarketData(wxCommandEvent& evt)
{
    DataRtnDepthMarketData MarketData;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&MarketData,NULL,sizeof(MarketData),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

    std::string strInstru(MarketData.DepthMarketDataField.InstrumentID);
    if(strInstru.empty())
        return;

    static bool bgo=true;
    if(!bgo) return;

    if(g_pPlatformMgr)
    {
        vector<string> LogonAccounts=g_pPlatformMgr->GetLogonAccounts();
        for(int i=0;i<(int)LogonAccounts.size();i++)
            UpdateByInstrument_Internal(LogonAccounts[i],strInstru);
    }
}

//更新和一个合约ID相关的项
void CPositionDetailInfoList::UpdateByInstrument_Internal(const string& strAccount,const string& strInstrumentID)
{
    if(!g_pPlatformMgr) return;
    IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(strAccount);
    if(!pSvr) return;

    bool bUpdate=false;
    set<long> setFTIDs;
    vector<PlatformStru_PositionDetail> vecValues;

    int PlatformID=g_pPlatformMgr->GetPlatformID(pSvr);

    map<int,map<string,long>>::iterator it_PlatformID=m_PositionDetailDataLastUpdateSeq.find(PlatformID);
    if(it_PlatformID==m_PositionDetailDataLastUpdateSeq.end())
    {
        m_PositionDetailDataLastUpdateSeq[PlatformID]=map<string,long>();
        it_PlatformID=m_PositionDetailDataLastUpdateSeq.find(PlatformID);
    }
    assert(it_PlatformID!=m_PositionDetailDataLastUpdateSeq.end());

    map<string,long>::iterator it_lastseq=it_PlatformID->second.find(strInstrumentID);
    if(it_lastseq==it_PlatformID->second.end())
    {
        it_PlatformID->second.insert(make_pair(strInstrumentID,0));
        it_lastseq=it_PlatformID->second.find(strInstrumentID);
    }
    assert(it_lastseq!=it_PlatformID->second.end());

    long lastseq=it_lastseq->second;
    
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败
    int irlt=pSvr->GetPositionDetails3(strInstrumentID,setFTIDs,vecValues,lastseq);
    if(irlt==0&&lastseq>it_lastseq->second)
    {
        it_lastseq->second=lastseq;
        if(!bUpdate) bUpdate=true;
    }

    //需要更新UI
    if(bUpdate)
        UpdateInstrumentItems2(strAccount,strInstrumentID,setFTIDs,vecValues);
}




CPositionDetailInfoList::CPositionDetailInfoList(wxWindow *parent,
						   wxWindowID id,
						   const wxPoint& pos,
						   const wxSize& size,
						   long style,
						   const wxValidator& validator,
						   const wxString& name)
						   :CDataListCtrl<PositionDetailKey,PlatformStru_PositionDetail>(UpdateListItemCallBack,parent, id, pos, size, style|wxLCEXT_MASK_SORT, validator, name)
{
    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PositionDetailInfoList.log");

    m_PostionDetailList=this;
    Init();
}



CPositionDetailInfoList::~CPositionDetailInfoList(void)
{
    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }
}

void CPositionDetailInfoList::ShowAll()
{
    vector<PlatformStru_PositionDetail> vecDatas,vecData2;
    long curseq;

    if(!g_pPlatformMgr) return;
    vector<IPlatformSingleSvr*> pSvrs=g_pPlatformMgr->GetLogonPlatformSvr();

    for(int i=0;i<(int)pSvrs.size();i++)
    {
        if(i==0) pSvrs[i]->GetPositionDetails(vecDatas,curseq);
        else
        {
            pSvrs[i]->GetPositionDetails(vecData2,curseq);
            vecDatas.insert(vecDatas.end(),vecData2.begin(),vecData2.end());
        }

        int PlatformID=g_pPlatformMgr->GetPlatformID(pSvrs[i]);
        map<int,map<string,long>>::iterator it_PlatformID=m_PositionDetailDataLastUpdateSeq.find(PlatformID);
        if(it_PlatformID!=m_PositionDetailDataLastUpdateSeq.end())
        {
            for(map<string,long>::iterator it_lastseq=it_PlatformID->second.begin();it_lastseq!=it_PlatformID->second.end();it_lastseq++)
                it_lastseq->second=curseq;
        }
    }

    UpdateAllItems(vecDatas);
}


//配置listctrl的参数
void CPositionDetailInfoList::Init(void)
{
	Clear();
	if(GetColumnCount()>0) DeleteAllColumns();
    m_FieldID2ColumnID.clear();

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

        m_TextColor=pListCfg->TextColor;

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

        m_FieldID2ColumnID[pColCfg->id] = i;

    }

    SetCfg(m_TextColor,m_FieldID2ColumnID);

    m_PositionDetailDataLastUpdateSeq.clear();

    ShowAll();
}




//配置文件改变

void CPositionDetailInfoList::OnCfgChanged()
{
    WXLOG_INFO("CPositionDetailInfoList::OnCfgChanged");

    Init();
}

void CPositionDetailInfoList::OnContext(wxContextMenuEvent& evt)
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
					PlatformStru_PositionDetail detail;
                    if(!GetValueByRow(i,detail))
                        continue;

					string InstrumentId = detail.InstrumentID;
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

void CPositionDetailInfoList::OnLanguageChanged( int langid)
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


void CPositionDetailInfoList::OnColumnResize( wxExtListEvent& event )
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

bool CPositionDetailInfoList::UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_PositionDetail* pNew=(const PlatformStru_PositionDetail*)pNewData;
    PlatformStru_PositionDetail* pOld=(PlatformStru_PositionDetail*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    outItem.SetTextColour(wxColor(textcol));

    switch(FieldID)
    {
        case POSITION_DETAIL_FIELD_ID_INSTRUMENT:
            if(!bnewline) return false;
            UpdateCol_String(InstrumentID);
            return true;
        case POSITION_DETAIL_FIELD_ID_HEDGEFLAG:
            UpdateCol_Number(HedgeFlag,CFieldValueTool::HedgeFlag2String(pNew->HedgeFlag,SVR_LANGUAGE));
            return true;
        case POSITION_DETAIL_FIELD_ID_DIRECTION:
            UpdateCol_Number(Direction,CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
            outItem.SetTextColour(wxColor(pNew->Direction==THOST_FTDC_D_Buy?RGB(255,0,0):RGB(0,128,0)));
            return true;
        case POSITION_DETAIL_FILED_ID_OPENTIME:
            if(!bnewline) return false;
            UpdateCol_String(OpenDate);
            return true;
        case POSITION_DETAIL_FIELD_ID_TRADEID:
            if(!bnewline) return false;
			UpdateCol_String3(TradeID,wxString(pNew->TradeID).Trim(true).Trim(false));
            outItem.SetbtrynumericAsComparing(true);
            return true;
        case POSITION_DETAIL_FIELD_ID_VOLUME:
            UpdateCol_Int(Volume);
            return true;
        case POSITION_DETAIL_FIELD_ID_OPENPRICE:
            UpdateCol_Price(OpenPrice);
            return true;
        case POSITION_DETAIL_FIELD_ID_COMBINSTRUMENT:
            if(!bnewline) return false;
            UpdateCol_String(CombInstrumentID);
            return true;
        case POSITION_DETAIL_FIELD_ID_EXCHANGE:
            if(!bnewline) return false;
            UpdateCol_String3(ExchangeID,CFieldValueTool::ExchangeID2Name((char*)(pNew->ExchangeID)));
            return true;
        case POSITION_DETAIL_FIEL_ID_CLOSEPROFIT:
            UpdateCol_Fund(CloseProfitByDate);
            if(pNew->CloseProfitByDate>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
            else if(pNew->CloseProfitByDate<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
            return true;
        case POSITION_DETAIL_FIELD_ID_POSITIONPROFIT:
            UpdateCol_Fund(PositionProfitByDate);
            if(pNew->PositionProfitByDate>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
            else if(pNew->PositionProfitByDate<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
            return true;
        case POSITION_DETAIL_FIELD_FLUCTUATION:
            UpdateCol_Fund(PositionProfitByTrade);
            if(pNew->PositionProfitByTrade>0) outItem.SetTextColour(wxColor(RGB(255,0,0)));
            else if(pNew->PositionProfitByTrade<0) outItem.SetTextColour(wxColor(RGB(0,128,0)));
            return true;
        case POSITION_DETAIL_FIELD_ID_MARGIN:
            UpdateCol_Fund(Margin);
            return true;
        case POSITION_DETAIL_FIELD_ID_LASTSETTLEMENTPRICE:
            UpdateCol_Price(LastSettlementPrice);
            return true;
		case POSITION_DETAIL_FIELD_CURPRICE:
            UpdateCol_Price(CurPrice);
			return true;
        case POSITION_DETAIL_FIELD_ID_CLOSEVOLUME:
            UpdateCol_Int(CloseVolume);
            return true;
        case POSITION_DETAIL_FIELD_ID_TYPE:
		{
            if(!bnewline) return false;
			UpdateCol_String3(OpenDate,((pNew->bToday)?LOADSTRING(TODAY_POSITION):LOADSTRING(PRE_POSITION)));
			return true;
		}
        case POSITION_DETAIL_FIELD_ID_COMBSTATE:
            UpdateCol_Number(TradeType,CFieldValueTool::TradeType2CombState(pNew->TradeType,SVR_LANGUAGE));
            return true;
        case POSITION_DETAIL_FIELD_ID_ACCOUNT:
            if(!bnewline) return false;
            UpdateCol_String(Account);
            return true;
    }
    return false;
}

void CPositionDetailInfoList::SaveColWidth()
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
void CPositionDetailInfoList::OnMAPlatformAdd(wxCommandEvent& event)
{
	ShowAll();
}
void CPositionDetailInfoList::OnMAPlatformDelete(wxCommandEvent& event)
{
	ShowAll();
}

