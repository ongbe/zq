
#include "stdafx.h"
#include "cfgMgr/CfgMgr.h"
#include "InstrumentTreePanel.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../ConfigApp/Const.h"
#include "Module-Misc2/GlobalConfigMgr.h"

extern PlatformSvrMgr* g_pPlatformMgr;

#define TREE_LIST_ID 740
#define DUMMY_TEXT   "__dummy_text__"
BEGIN_EVENT_TABLE(InstrumentTreePanel,wxPanel)
	EVT_SIZE(OnSize)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_TREE_ITEM_EXPANDING(TREE_LIST_ID,OnTreeItemExpanding)
	EVT_COMMAND(wxID_ANY, wxEVT_INSTRUMENT_LIST_MARGINRATE_UPDATE,  OnUpdateForInstrument)
	EVT_COMMAND(wxID_ANY, wxEVT_INSTRUMENT_LIST_COMMISSIONRATE_UPDATE,  OnUpdateForInstrument)
	EVT_TREE_ITEM_ACTIVATED(TREE_LIST_ID,OnTreeItemActived)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
	EVT_TREE_ITEM_RIGHT_CLICK(TREE_LIST_ID,OnTreeItemRightClick)
	EVT_LIST_COL_RIGHT_CLICK(TREE_LIST_ID,OnTreeHeaderRightClick)
END_EVENT_TABLE()
InstrumentTreePanel* g_pInstrumentTree=NULL;
InstrumentTreePanel::InstrumentTreePanel(wxWindow *parent,
								 wxWindowID id,
								 const wxPoint& pos,
								 const wxSize& size,
								 long style,
								 const wxString& name)
: wxPanel(parent, id, pos, size, style, name),
m_treelist(NULL),
m_root(NULL)
{
	g_pInstrumentTree=this;
    SetBackgroundColour(DEFAULT_COLOUR); 
	m_treelist=new wxTreeListCtrl(this,TREE_LIST_ID,wxDefaultPosition,GetClientSize(),
		wxTR_HAS_BUTTONS|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_MULTIPLE);
	m_treelist->AddColumn(wxEmptyString);
	m_root=m_treelist->AddRoot(wxEmptyString);
	InitializeColumn();
	InitializeData();
}
InstrumentTreePanel::~InstrumentTreePanel()
{
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_INSTRUMENT);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_INSTRUMENT);
}
void InstrumentTreePanel::InitializeColumn(bool onlyText)
{
	while(!onlyText&&m_treelist->GetColumnCount()>1) m_treelist->RemoveColumn(1);
	/*获取配置信息*/
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);
	int flag,nColCount = pCfgMgr->GetListVisibleColCount(GID_INSTRUMENT);
	for(int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, i);
		if(onlyText)
			m_treelist->SetColumnText(i+1,pCfgMgr->GetColCfgCaption(GID_INSTRUMENT,pColCfg->id));
		else
		{
			//"品种代码，合约，合约名"栏，左对齐
			if(pColCfg->id == INSTRUMENT_FIELD_ID_PRODUCTID || pColCfg->id == INSTRUMENT_FIELD_ID_INSTRUMENTID ||pColCfg->id == INSTRUMENT_FIELD_ID_INSTRUMENTNAME )
				flag=wxLIST_FORMAT_LEFT;
			else//其余的，右对齐
				flag=wxLIST_FORMAT_RIGHT;
			m_treelist->AddColumn(pCfgMgr->GetColCfgCaption(GID_INSTRUMENT,pColCfg->id),pColCfg->Width,flag);
		}

	}
}
void InstrumentTreePanel::InitializeData()
{
	static bool bFirstInitData=true;
	if(bFirstInitData)
	{
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_INSTRUMENT, CallBackFunc_BID_RspQryInstrumentMarginRate);
		if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_INSTRUMENT, CallBackFunc_BID_RspQryInstrumentCommissionRate);
        InitializeItem();
		bFirstInitData=false;
	}
}
void InstrumentTreePanel::InitializeItem()
{
	m_treelist->DeleteChildren(m_root);
	vector<string> exchangeIDset;
	exchangeIDset.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetExchangeIDs(exchangeIDset);

	std::map<std::string,std::set<std::string>> ProductIdMap;
	std::map<std::string,std::set<std::string>>::iterator ProductIdMap_iter;
	for(int exch_index=0;exch_index<(int)exchangeIDset.size();exch_index++)
	{
        string exchangename=DEFAULT_SVR()?DEFAULT_SVR()->ExchangeID2String(exchangeIDset[exch_index]):"";
		wxTreeItemId exch_Id=m_treelist->AppendItem(m_root,exchangename);
		ProductIdMap.clear();
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetProductID_InstrumentIDsByExchangeID(ProductIdMap,exchangeIDset[exch_index]);
		for(ProductIdMap_iter=ProductIdMap.begin();ProductIdMap_iter!=ProductIdMap.end();++ProductIdMap_iter)
		{
			wxTreeItemId product_Id=m_treelist->AppendItem(exch_Id,ProductIdMap_iter->first.c_str(),-1,-1,new InstrumentItemData(ProductIdMap_iter->second));
			m_treelist->AppendItem(product_Id,DUMMY_TEXT);
		}
	}
}
void InstrumentTreePanel::OnSize(wxSizeEvent& event)
{
	if(m_treelist)
		m_treelist->SetSize(GetClientSize());
}
bool InstrumentTreePanel::IsTypeNode(wxTreeItemId Id)
{
	wxTreeItemId ActiveId=Id;
	if(!ActiveId)return false;//无效节点
	ActiveId=m_treelist->GetItemParent(ActiveId);
	if(!ActiveId)return false;//根节点
	ActiveId=m_treelist->GetItemParent(ActiveId);
	if(!ActiveId)return false;//交易所节点
	ActiveId=m_treelist->GetItemParent(ActiveId);
	if(!ActiveId)return true;//品种节点
	return false;
}
void InstrumentTreePanel::OnTreeItemExpanding(wxTreeEvent& event)
{
	wxTreeItemId Expand_Id=event.GetItem();
	wxString  strTypeName=m_treelist->GetItemText(Expand_Id);
	if(IsTypeNode(Expand_Id))
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId dummyID=m_treelist->GetFirstChild(Expand_Id,cookie);
		if(m_treelist->GetItemText(dummyID)==DUMMY_TEXT)
		{
			m_treelist->DeleteChildren(Expand_Id);
			InstrumentItemData* pData=(InstrumentItemData*)m_treelist->GetItemData(Expand_Id);
			if(pData)
			{
				std::set<std::string> datas;
				std::set<std::string>::iterator it;
				pData->GetData(datas);
				for(it=datas.begin();it!=datas.end();++it)
				{
					wxTreeItemId InstTreeID=m_treelist->AppendItem(Expand_Id,DisplayInstrument(strTypeName,*it));
					m_Instrument2TreeItemMap[*it]=InstTreeID;
					AddInstrumentInfo(*it,InstTreeID);
				}
			}
		}
	}
	event.Skip();
}
wxString InstrumentTreePanel::DisplayInstrument(wxString  strTypeName,wxString srcstring)
{
	//Pats合约合成代码
	//strInstrumentID = name + "-" + date + "@" + exchange;
	//for(int pos=strInstrumentID.length()-1;pos>=0;pos--) 
	//	if(strInstrumentID[pos]==' ')strInstrumentID[pos]='-';
	//处理外盘合约名，显示合约名到期时间
	if(srcstring.find('@')!=wxString::npos&&srcstring.find('-')!=wxString::npos)
	{
		wxString strShortName=srcstring.BeforeLast('@');
		strShortName=strShortName.Mid(strTypeName.Length()+1);
		for(int pos=strShortName.length()-1;pos>=0;pos--) 
		if(strShortName[pos]=='-')strShortName[pos]=' ';	
		return strShortName;

	}
	return srcstring;
}
void InstrumentTreePanel::AddInstrumentInfo(string strID,wxTreeItemId treeid)
{
	PlatformStru_InstrumentInfo tmpInstrument;
	memset(&tmpInstrument,0,sizeof(tmpInstrument));
	PlatformStru_InstrumentCommissionRate commField;
	memset(&commField,0,sizeof(commField));
	PlatformStru_InstrumentMarginRate     margField;
	memset(&margField,0,sizeof(margField));
    int ret_Instrument=DEFAULT_SVR()?DEFAULT_SVR()->GetInstrumentInfo(strID,tmpInstrument):-1;
    int ret_Comm=DEFAULT_SVR()?DEFAULT_SVR()->GetCommissionRate(strID, commField):-1;
    int ret_Marg=DEFAULT_SVR()?DEFAULT_SVR()->GetMarginRate(strID,margField):-1;
	//重新设置一行
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_INSTRUMENT);
	string strValue;
	for(int i = 0; i < nColCount; i++)           //设置各列的信息
	{
		LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, i);
		GetColValue(pColCfg->id, 
			ret_Instrument==0?(&tmpInstrument):NULL,
			ret_Comm==0?(&commField):NULL,
			ret_Marg==0?(&margField):NULL,  
			strValue);                         
		m_treelist->SetItemText(treeid,i+1, strValue);
	} 
}
/*得到列的数据*/
int InstrumentTreePanel::GetColValue(int ColId, PlatformStru_InstrumentInfo* pField, 
								 PlatformStru_InstrumentCommissionRate* commField,
								 PlatformStru_InstrumentMarginRate* margField,
								 std::string &strValue)
{	
	char buf[256];
	memset(buf,0,sizeof(buf));

	strValue="";
	switch(ColId)
	{
	case INSTRUMENT_FIELD_ID_PRODUCTID:
		if(pField)
			strValue = pField->ProductID;					                                        //品种代码
		break;
	case INSTRUMENT_FIELD_ID_INSTRUMENTID:
		if(pField)
			strValue = pField->InstrumentID;					                             //合约
		break; 
	case INSTRUMENT_FIELD_ID_INSTRUMENTNAME:
		if(pField)
			strValue = pField->InstrumentName;					//合约名					
		break;
	case INSTRUMENT_FIELD_ID_EXCHANGE:            
		if(pField)
            strValue =DEFAULT_SVR()?DEFAULT_SVR()->ExchangeID2String( pField->ExchangeID):"";                  //交易所			
		break;
	case INSTRUMENT_FIELD_ID_MULTIPLY:
		if(pField)
			strValue = GlobalFunc::ConvertToString(pField->VolumeMultiple);					//合约乘数
		break;
	case INSTRUMENT_FIELD_ID_PRICETICK:
		if(pField)
			strValue = Double2String(pField->PriceTick);					//最小价格变动单位
		break;
	case INSTRUMENT_FIELD_ID_PRODUCTCLASS:
		if(pField)
            strValue = CFieldValueTool::ProductType2String(pField->ProductClass,DEFAULT_SVR()?DEFAULT_SVR()->GetServerParamCfg().LanguageId:2);		//产品类型							
		break;
	case INSTRUMENT_FIELD_ID_EXPIREDATE:
		if(pField)
			strValue = GlobalFunc::ConvertToString(pField->ExpireDate);					        //最后日期
		break;
	case INSTRUMENT_FIELD_ID_LONGMARGINRATIO:
		if(!DEFAULT_SVR()->MarginRatioInInstrumentInfo())
		{
			if(margField)
			{
				if(strlen(margField->InstrumentID)==0) break;
				if(!util::equals(margField->LongMarginRatioByMoney,0))
					strValue = Double2String(margField->LongMarginRatioByMoney*100)+"%"; 
				else
					strValue="";
			}
		}
		else
		{
          if(!util::equals(pField->LongMarginRatio,0))
				strValue = Double2String(pField->LongMarginRatio*100)+"%"; 
			else
				strValue="";	
		}
		break;
	case INSTRUMENT_FIELD_ID_SHORTMARGINRATIO:
		if(!DEFAULT_SVR()->MarginRatioInInstrumentInfo())
		{
			if(margField)
			{
				if(strlen(margField->InstrumentID)==0) break;
				if(!util::equals(margField->ShortMarginRatioByMoney,0))
					strValue = Double2String(margField->ShortMarginRatioByMoney*100)+"%";  
				else
					strValue="";
			}
		}
		else
		{
          if(!util::equals(pField->ShortMarginRatio,0))
				strValue = Double2String(pField->ShortMarginRatio*100)+"%"; 
			else
				strValue="";	
		}
		break;

	case INSTRUMENT_FIELD_ID_OPENRATIO:	
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->OpenRatioByVolume,0))
				strValue = Double2String(commField->OpenRatioByVolume);    //开仓手续费 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSERATIO:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->CloseRatioByVolume,0))
				strValue = Double2String(commField->CloseRatioByVolume);					//平仓手续费  
			else
				strValue="";           
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSETODAYRATIO:		
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;	      
			if(!util::equals(commField->CloseTodayRatioByVolume,0))                                     						//平今手续费
				strValue = Double2String(commField->CloseTodayRatioByVolume);   
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->OpenRatioByMoney,0))
				strValue = Double2String(commField->OpenRatioByMoney*100)+"%";     //开仓手续费率 
			else
				strValue="";
		}
		break;

	case INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->CloseRatioByMoney,0))
				strValue = Double2String(commField->CloseRatioByMoney*100)+"%";    //平仓手续费率 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->CloseRatioByMoney,0))
				strValue = Double2String(commField->CloseTodayRatioByMoney*100)+"%";    //平今手续费率 
			else
				strValue="";
		}
		break;

	case INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME:
		if(margField)
		{
			if(strlen(margField->InstrumentID)==0) break;
			if(!util::equals(margField->LongMarginRatioByVolume,0))
				strValue = GlobalFunc::GetAccountFormatString(margField->LongMarginRatioByVolume);    //多头保证金费 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME:
		if(margField)
		{
			if(strlen(margField->InstrumentID)==0) break;
			if(!util::equals(margField->ShortMarginRatioByVolume,0))
				strValue = GlobalFunc::GetAccountFormatString(margField->ShortMarginRatioByVolume);   //空头保证金费 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CURRENCY:
		if ( pField )
		{
			strValue = GlobalFunc::ConvertToString( pField->Currency );
		}
		break;
	case INSTRUMENT_FIELD_ID_TICKSPERPOINT:
		if(pField)
		{
			
		    strValue = GlobalFunc::ConvertToString(pField->TicksPerPoint);   // 
			
		}
		break;
	case INSTRUMENT_FIELD_ID_TICKSIZE:
		if(pField)
		{
			
			strValue = GlobalFunc::ConvertToString(pField->TickSize);   // 
			
		}
		break;

	}
	return 0;

}
//查询保证金率响应的回调函数，更新保证金率的显示
int InstrumentTreePanel::CallBackFunc_BID_RspQryInstrumentMarginRate(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInstrumentMarginRate || data.Size!=sizeof(DataRspQryInstrumentMarginRate))
		return 0;

	wxCommandEvent evt(wxEVT_INSTRUMENT_LIST_MARGINRATE_UPDATE, wxID_ANY);
	evt.SetString( ((DataRspQryInstrumentMarginRate&)data).InstrumentMarginRateField.InstrumentID );
	if( g_pInstrumentTree )
		g_pInstrumentTree->AddPendingEvent(evt);
	return 0;
}

//查询保证金率响应的回调函数，更新保证金率的显示
int InstrumentTreePanel::CallBackFunc_BID_RspQryInstrumentCommissionRate(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInstrumentCommissionRate || data.Size!=sizeof(DataRspQryInstrumentCommissionRate))
		return 0;

	wxCommandEvent evt(wxEVT_INSTRUMENT_LIST_COMMISSIONRATE_UPDATE, wxID_ANY);
	PlatformStru_InstrumentCommissionRate& icr=((DataRspQryInstrumentCommissionRate&)data).InstrumentCommissionRateField;
	//有可能是合约品种
	set<string> InstrumentList;
    IPlatformSingleSvr* pSvr=CURRENT_SVR();
    int ret=pSvr?pSvr->GetInstrumentListByProductID(icr.InstrumentID,InstrumentList):-1;
	if(ret>0)
	{
        //是合约品种
		for(set<string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
		{
            DWORD EvtParamID;
            string EvtParamStr(*it2);
            if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                evt.SetInt((int)EvtParamID);
			if( g_pInstrumentTree )
				g_pInstrumentTree->AddPendingEvent(evt);
		}
	}
	else
	{
        DWORD EvtParamID;
        string EvtParamStr(icr.InstrumentID);
        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
            evt.SetInt((int)EvtParamID);
		if( g_pInstrumentTree )
			g_pInstrumentTree->AddPendingEvent(evt);
	}

	return 0;
}
//有新的手续费和保证金率来了，更新显示
void InstrumentTreePanel::OnUpdateForInstrument(wxCommandEvent& evt)
{
    string EvtParamStr;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;
    wxString strInstrumentID(EvtParamStr.c_str());

    
	std::map<std::string, wxTreeItemId>::iterator it_inst = m_Instrument2TreeItemMap.find(strInstrumentID.c_str());
	if(it_inst == m_Instrument2TreeItemMap.end())
	{
		//当前没有添加，不操作
		return;
	}
	wxTreeItemId treeid=it_inst->second;
	PlatformStru_InstrumentCommissionRate commField;
	memset(&commField,0,sizeof(commField));
	PlatformStru_InstrumentMarginRate margField;
    IPlatformSingleSvr* pSvr=CURRENT_SVR();
    int ret_Comm=pSvr?pSvr->GetCommissionRate(strInstrumentID.c_str(), commField):-1;
    int ret_Marg=pSvr?pSvr->GetMarginRate(strInstrumentID.c_str(),margField):-1;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_INSTRUMENT);
	for(int i = 0; i < nColCount; i++)           //设置各列的信息
	{
		LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, i);
		if( pColCfg->id ==INSTRUMENT_FIELD_ID_LONGMARGINRATIO||
			pColCfg->id ==INSTRUMENT_FIELD_ID_SHORTMARGINRATIO||
			pColCfg->id ==INSTRUMENT_FIELD_ID_OPENRATIO||
			pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSERATIO||
			pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSETODAYRATIO||
			pColCfg->id ==INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY||
			pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY||
			pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY||
			pColCfg->id ==INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME||
			pColCfg->id ==INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME)
		{
			std::string strValue;
			GetColValue(pColCfg->id, 
				NULL,
				ret_Comm==0?(&commField):NULL,
				ret_Marg==0?(&margField):NULL,  
				strValue);
			m_treelist->SetItemText(treeid,i+1, strValue);
		}                     
		
	}
}
void InstrumentTreePanel::OnTreeItemActived(wxTreeEvent& event)
{
	std::map<std::string, wxTreeItemId>::iterator it= m_Instrument2TreeItemMap.begin();
	std::string InstrumentID;
	//获得单击的合约
	for(it;it!=m_Instrument2TreeItemMap.end();++it)
	{
		if(it->second==event.GetItem())
		{
			InstrumentID=it->first;
			break;
		}
	}

	if(!InstrumentID.empty())
	{
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetCommissionRate_TriggerReq(InstrumentID);
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetMarginRate_TriggerReq(InstrumentID);
	}
	event.Skip();
}
//配置文件发生变化
void InstrumentTreePanel::OnCfgChanged(wxCommandEvent& evt)
{
	InitializeColumn();
    InitializeItem();
}
void InstrumentTreePanel::OnLanguageChanged( wxCommandEvent& event )
{
	InitializeColumn(true);
    InitializeItem();
}
void InstrumentTreePanel::OnTreeItemRightClick(wxTreeEvent& event)
{
	OnContext(true);
	event.Skip();
}
void InstrumentTreePanel::OnTreeHeaderRightClick(wxListEvent& event)
{
	OnContext(false);
	event.Skip();
}
void InstrumentTreePanel::OnContext(bool bIsRowCLick)
{
	wxMenu menu;
	//menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if ( bIsRowCLick )
	{
		wxMenu* pAddtoSubMenu = NULL;
		for(int i = 0; i < pCfgMgr->GetInstrumentGroupCount();++i)
		{
			LPINSTRUMENT_GROUP_CFG pGroup = pCfgMgr->GetInstrumentGroupInfo(i);
			if( pGroup->IsVisibility == 1&&DEFAULT_SVR()&&DEFAULT_SVR()->ExchangeName2ID(pGroup->GroupName).empty())
			{
				if( pAddtoSubMenu == NULL)
					pAddtoSubMenu=new wxMenu;
				pAddtoSubMenu->Append(ID_MENU_ADDTOMYSELF+i, pGroup->GroupName);
			}
		}
		if( pAddtoSubMenu ) 
		{
			menu.AppendSubMenu(pAddtoSubMenu,LOADSTRING(CONTEXTMENU_ADDTOMYSELF));
		}
	}

	POINT pt;
	::GetCursorPos(&pt);
	BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

	//加入自选
	if( retcmd - ID_MENU_ADDTOMYSELF >= 0 )
	{
		bool bAdded = false,bOverLimit=false;
		std::map<std::string,wxTreeItemId>::iterator it=m_Instrument2TreeItemMap.begin();
		for(it;it!=m_Instrument2TreeItemMap.end();++it)
		{
			if(m_treelist->IsSelected(it->second))
			{
				if(pCfgMgr->GetInstrumentGroupMemberCount(retcmd - ID_MENU_ADDTOMYSELF)>=DEFAULT_SVR()->GetGroupMaxContractNum())
				{
					bOverLimit=true;
					break;
				}
				pCfgMgr->AddInstrument( retcmd - ID_MENU_ADDTOMYSELF, it->first.c_str()); 
				bAdded = true;
			}	
		}   
		if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);}
		if(bOverLimit)
			wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);	
		return;
	}

	switch(retcmd)
	{
	case ID_MENU_EXPORT://导出列表
		{
			//BEGIN_DO_EXPORT(GetId())
			//	wxExtListItem item;
			//int colcount = m_pListCtrl->GetColumnCount();
			//for(int col=0;col<colcount;col++)
			//{
			//	if(m_pListCtrl->GetColumn(col,item))
			//	{
			//		strline+=item.GetText();
			//	}
			//	if(col+1!=colcount) strline+=",";

			//}
			//strline+="\r\n";
			//file.Write(strline.c_str(),strline.size());
			//for(int index=0;index< m_pListCtrl->GetItemCount();index++)
			//{
			//	strline.Empty();
			//	for(int col=0;col<colcount;col++)
			//	{
			//		strline+= m_pListCtrl->GetSubItemText(index,col);
			//		if(col+1!=colcount) strline+=",";
			//	}
			//	strline+="\r\n";
			//	file.Write(strline.c_str(),strline.size());
			//}
			//END_DO_EXPORT()
		}
		break;
	case ID_MENU_COLUMNCONFIG://表格列设置
        SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_INSTRUMENTLIST);
		break;
	default:
		break;
	}

}