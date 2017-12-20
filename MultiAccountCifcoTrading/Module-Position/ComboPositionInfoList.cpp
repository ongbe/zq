#include "StdAfx.h"
#include "ComboPositionInfoList.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#include "../inc/gui/MainFrame.h"
#include "PositionPanel.h"
extern PlatformSvrMgr* g_pPlatformMgr;

CComboPositionInfoList* g_ComboPostionList = NULL;
BEGIN_EVENT_TABLE(CComboPositionInfoList, wxExtListCtrl)
	EVT_COMMAND(wxID_ANY, wxEVT_RspQryPositionCombLast, CComboPositionInfoList::OnRspQryPositionComboLast)
	EVT_COMMAND(wxID_ANY, wxEVT_RtnTrade, CComboPositionInfoList::OnRtnTrade)
	EVT_LIST_EXT_CONTEXT_MENU(CComboPositionInfoList::OnContext)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CComboPositionInfoList::CComboPositionInfoList(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxValidator& validator,
			const wxString& name):
	CDataListCtrl<PositionCombKey,PlatformStru_PositionComb>(UpdateListItemCallBack,parent, winid, pos, size, style|wxLCEXT_MASK_SORT, validator, name)
{
	g_ComboPostionList = this;


	InitListCtrl();
	ShowAll();
}

CComboPositionInfoList::~CComboPositionInfoList()
{
}

void CComboPositionInfoList::OnCfgChanged( )
{
	ClearAll();
	InitListCtrl();
	ShowAll();
}

void CComboPositionInfoList::OnLanguageChanged( wxCommandEvent& event )
{
	TRANSFORM_COLUMN_TEXT(GID_POSITIONCOMB,this);
}

//初始化列表行列信息
void CComboPositionInfoList::InitListCtrl()
{
	Clear();
	if(GetColumnCount()>0) DeleteAllColumns();
	m_FieldID2ColumnID.clear();

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	if(!pCfgMgr) 
        return;
	LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_POSITIONCOMB);
	if(!pListCfg)
        return;


	/*设置列表颜色*/
	SetHeaderBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
	SetHeaderForegroundColour(pListCfg->HeadColor);
	SetBackgroundColour(pListCfg->BackgroundColor);
	SetForegroundColour(pListCfg->TextColor);
	wxFont font;
	font.SetNativeFontInfoUserDesc(pListCfg->szFont);
	SetFont(font);
	SetOddRowColour(pListCfg->EvenLineBackgroundColor);
	SetEvenRowColour(pListCfg->OddLineBackgroundColor);

	wxExtListItem col; 
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_POSITIONCOMB);
	for(int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITIONCOMB, i);
		//合约左对齐
		if(pColCfg->id == COMBO_POSITION_FIELD_ID_INSTRUMENT )
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//其余右对齐
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

		col.SetText(pCfgMgr->GetColCfgCaption(GID_POSITIONCOMB,pColCfg->id));
		col.SetWidth(pColCfg->Width);
		InsertColumn(i,col); 
		m_FieldID2ColumnID[pColCfg->id] = i;
	}

	SetCfg(pListCfg->TextColor, m_FieldID2ColumnID);
}

void CComboPositionInfoList::OnContext( wxContextMenuEvent& evt )
{
	wxMenu menu;
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	BOOL bIsRowCLick = evt.GetId();//bIsRowCLick=TRUE表示点击内容行，=FALSE表示点击标题栏
	if ( bIsRowCLick )
	{
		menu.AppendSeparator();
		INSERT_ADDTOMYSELF_ITEM(menu);
	}

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
				string InstrumentId = (GetItemText( i )).c_str();
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

		if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_LIST_COMBO_POSITION_CHANGE);}
		return;
	}

	switch(retcmd)
	{
	case ID_MENU_AUTO_ADJUST_COL_WIDTH:
		AutoAdjustColumnWidth();
		SaveColWidth();
		break;
	case ID_MENU_EXPORT://导出列表
		{
			BEGIN_DO_EXPORT(GetId())
				wxExtListItem item;
			int colcount = GetColumnCount();
			for(int col=0;col<colcount;col++)
			{
				if(GetColumn(col,item))
				{
					strline+=item.GetText();
				}
				if(col+1!=colcount) strline+=",";

			}
			strline+="\r\n";
			file.Write(strline.c_str(),strline.size());
			for(int index=0;index< GetItemCount();index++)
			{
				strline.Empty();
				for(int col=0;col<colcount;col++)
				{
					strline+= GetSubItemText(index,col);
					if(col+1!=colcount) strline+=",";
				}
				strline+="\r\n";
				file.Write(strline.c_str(),strline.size());
			}
			END_DO_EXPORT()
		}
		break;
	case ID_MENU_COLUMNCONFIG://表格列设置
		SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_COMBO_POSITION);
		break;
	default:
		break;
	}

	evt.Skip();
}

void CComboPositionInfoList::OnRtnTrade( wxCommandEvent& evt )
{
	DataRtnTrade RtnTrade;
	DWORD EvtParamID=(DWORD)evt.GetInt();
	if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,&RtnTrade,NULL,sizeof(RtnTrade),NULL))
		CEventParam::DeleteEventParam(EvtParamID);
	else return;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(RtnTrade.Head.pPlatformSvr);

	if(!pSvr||m_ppSvrSet->find(pSvr)==m_ppSvrSet->end())
		return;

    std::vector<PlatformStru_PositionComb> vecData;
	std::string strTradeID(RtnTrade.TradeField.TradeID);
	if(strTradeID.empty())
		return;

	std::string strInstrumentID;
	pSvr->GetPositionCombs2(strTradeID,strInstrumentID, vecData);
	if ( strInstrumentID.empty() )
	{
		return;
	}

    std::set<long> setFTID;
    int count=vecData.size();
    for(int i=0;i<count;i++)
        setFTID.insert(vecData[i].FTID);

	UpdateInstrumentItems2(strInstrumentID,setFTID,vecData);
}

void CComboPositionInfoList::OnRspQryPositionComboLast( wxCommandEvent& evt )
{
	DataRspQryInvestorPositionComb RspQryData;
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
        std::vector<PlatformStru_PositionComb> vecData;
		std::string strComboInstrument;
		pSvr->GetPositionCombs2(RspQryData.InvestorPositionCombineDetailField.Leg1TradeID,strComboInstrument,vecData);
		if(!vecData.empty())
        {
            std::set<long> setFTID;
            int count=vecData.size();
            for(int i=0;i<count;i++)
                setFTID.insert(vecData[i].FTID);
			UpdateInstrumentItems2(QryInstrumentID,setFTID,vecData);
        }
	}
}

void CComboPositionInfoList::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_POSITIONCOMB ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITIONCOMB, nCol);
	pColCfg->Width = GetColumnWidth( nCol );
}

void CComboPositionInfoList::ShowAll()
{
	std::vector<PlatformStru_PositionComb> vecData,all;
	set<IPlatformSingleSvr*>::iterator it;
	for (it=PositionPanel::m_pSvrSet.begin();it!=PositionPanel::m_pSvrSet.end();++it)
	{
		(*it)->GetPositionCombs(vecData);
		all.insert(all.end(),vecData.begin(),vecData.end());
	}
	UpdateAllItems(all);
}

#define UpdateCol_String(FieldName)  \
	if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
	    return false;\
	outItem.SetText(wxString(pNew->FieldName));
#define UpdateCol_Number(FieldName,DispName)  \
	if(!bnewline&&pOld->FieldName==pNew->FieldName)\
	    return false;\
	outItem.SetText(wxString(DispName));\
    outItem.SetiColData((int)(pNew->FieldName));
#define UpdateCol_Money(FieldName)  \
	if(!bnewline&&pOld->FieldName==pNew->FieldName) \
		return false;\
	if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
	else tmpstr.Printf("%.02lf", pNew->FieldName ); \
		outItem.SetText(tmpstr);\
    outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Volume(FieldName)  \
	if(!bnewline&&pOld->FieldName==pNew->FieldName)\
	    return false;\
	outItem.SetText(wxString(GlobalFunc::ConvertToString(pNew->FieldName)));\
    outItem.SetiColData(pNew->FieldName);

bool CComboPositionInfoList::UpdateListItemCallBack( const void* pNewData,void* pOldData,
													unsigned long textcol,int FieldID,
													int ColID,int RowID,
													bool bnewline,
													wxExtListItem& outItem )
{
	const PlatformStru_PositionComb* pNew=(const PlatformStru_PositionComb*)pNewData;
	PlatformStru_PositionComb* pOld=(PlatformStru_PositionComb*)pOldData;
	bool brlt=false;
	wxString tmpstr;
    
	outItem.SetColumn(ColID);
	outItem.SetId(RowID);
	outItem.SetTextColour(wxColor(textcol));

	std::string strValue;
	switch( FieldID )
	{
	case COMBO_POSITION_FIELD_ID_INSTRUMENT:
		UpdateCol_String(InstrumentID);
		return true;
	case COMBO_POSITION_FIELD_ID_DIRECTION:
		UpdateCol_Number(Direction,CFieldValueTool::Direction2String(pNew->Direction,SVR_LANGUAGE));
		outItem.SetTextColour(wxColor(pNew->Direction==THOST_FTDC_D_Buy?RGB(255,0,0):RGB(0,128,0)));
		return true;
	case COMBO_POSITION_FIELD_ID_VOLUME:
		UpdateCol_Volume(TotalAmt);
		return true;
	case COMBO_POSITION_FIELD_ID_OPENAVGPRICE:
		UpdateCol_Money(OpenPrice);
		return true;
	case COMBO_POSITION_FIELD_ID_HEDGEFLAG:
		UpdateCol_Number(HedgeFlag,CFieldValueTool::HedgeFlag2String(pNew->HedgeFlag,SVR_LANGUAGE));
		return true;
	default:
		return false;
	}

}

void CComboPositionInfoList::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nColCount = GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_POSITIONCOMB ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_POSITIONCOMB, i);
		pColCfg->Width = GetColumnWidth( i );
	}
}
void CComboPositionInfoList::OnMAPlatformAdd(wxCommandEvent& event)
{
	Clear();
	ShowAll();
}
void CComboPositionInfoList::OnMAPlatformDelete(wxCommandEvent& event)
{
	Clear();
	ShowAll();
}

bool CComboPositionInfoList::ReQry()
{
	return true;
}
