#include "stdafx.h"
#include <fstream>
#include "wx/textfile.h"

#include "cfgMgr/CfgMgr.h"
#include "J_WeiTuoCanShu.h"
#include "J_XiaDanShuRuShunXu.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(J_WeiTuoCanShuDlg,ConfigPanelBase)
	EVT_BUTTON(ID_BTSET,OnSet)
	EVT_COMBOBOX(ID_COM, OnCom) 
	EVT_BUTTON(ID_SAVE_AND_RETURN,OnSaveAndReturn)
	EVT_LIST_EXT_COL_CLICK(ID_List,OnColClick)	

	EVT_LIST_EXT_BEGIN_LABEL_EDIT(ID_List, BeginEdit)
	EVT_LIST_EXT_END_LABEL_EDIT(ID_List,EndEdit)
	EVT_LIST_EXT_KEY_DOWN(ID_List,OnText)
	EVT_SIZE( OnSize )

	EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturn)
	EVT_RADIOBUTTON(wxID_ANY,OnRadioButton)
	EVT_CHECKBOX(wxID_ANY, OnCheck)
	EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
	EVT_LIST_ITEM_SELECTED(ID_List, Sel)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()

J_WeiTuoCanShuDlg::J_WeiTuoCanShuDlg(wxWindow *parent,
									 wxWindowID winid ,
									 const wxPoint& pos ,
									 const wxSize& size ,
									 long style,
									 const wxString& name)
: ConfigPanelBase(parent,winid,pos,size,style,name)
, m_timer( this )
{
	ConfigPanelBase::CreateHeader( "CONFIG_ORDER_PARAM" );

	m_vVarietyInfo.clear();
	m_vecGroupInfo.clear();

	int offset = 30;
	int xoffset = 20;
	DWORD a = GetLastError();

	m_pStaticModifyOrderParam =new wxStaticBox(this, wxID_ANY, LOADSTRING(CONFIG_MODIFY_ORDER_PARAM), wxPoint(10+xoffset, 27+offset), wxSize(442, 376));

	m_pStaticExchange = new wxStaticText(this, wxID_ANY, LOADSTRING(EXCHANGE), wxPoint(19+xoffset, 65+offset), wxDefaultSize, 0, wxT("WxStaticText4"));
	wxArrayString arrayStringFor_WxComboBox1;
	m_pComGroup = new wxOwnerDrawnComboBox(this,ID_COM, wxT(""), wxPoint(76+xoffset, 62+offset), wxSize(106, 20), arrayStringFor_WxComboBox1, wxCB_READONLY, wxDefaultValidator, wxT("WxComboBox1"));
	m_pList = new wxExtListCtrl_Obsolete(this, ID_List, wxPoint(19+xoffset, 88+offset), 
		wxSize(420, 308), wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_HRULES|wxLCEXT_SINGLE_SEL|wxLCEXT_TOGGLE_COLOUR, 
		wxDefaultValidator, wxT("WxListCtrl1"));
	new wxStaticBox(this, wxID_ANY, LOADSTRING(CONFIG_PERSONALITY_SETUP), wxPoint(469+xoffset, 27+offset), wxSize(193, 148));
	BtSet = new wxButton(this, ID_BTSET, LOADSTRING(CONFIG_INPUT_ORDER), wxPoint(500+xoffset, 48+offset), wxSize(111, 25), 0, wxDefaultValidator, wxT("WxButton1"));
	BtSet->Show(false);

	int xsub = 0; int ysub = 20;
	m_pChkOrderConfirm = new wxCheckBox(this, wxID_ANY, LOADSTRING(CONFIG_ORDER_CONFIRM), wxPoint(484+xoffset, 75+offset-ysub), wxSize(170, 17), 0, wxDefaultValidator, wxT("WxCheckBox1"));
	m_pChkOrderPrompt = new wxCheckBox(this, wxID_ANY, LOADSTRING(ORDER_PROMPT), wxPoint(484+xoffset, 95+offset-ysub), wxSize(170, 17), 0, wxDefaultValidator, wxT("WxCheckBox2"));
	m_pChkOrderPrompt->Show(false);
	m_pChkCancelOrder = new wxCheckBox(this, wxID_ANY, LOADSTRING(CONFIRM_BEFORE_CANCEL_ORDER), wxPoint(484+xoffset, 95+offset-ysub), wxSize(170, 17), 0, wxDefaultValidator, wxT("WxCheckBox3"));
	m_pStaticPriceTicker = new wxStaticText(this, wxID_ANY, LOADSTRING(CONFIG_PRICE_TICKER), wxPoint(475+xoffset, 140+offset-ysub), wxDefaultSize, 0, wxT("WxStaticText2"));
	m_pSpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("0"), wxPoint(565+xoffset, 136+offset-ysub), wxSize(77, 21), wxSP_ARROW_KEYS, 0, 999, 0);
	m_pStaticPriceMethod = new wxStaticText(this, wxID_ANY, LOADSTRING(CONFIG_PRICE_METHOD), wxPoint(476+xoffset, 174+offset-ysub), wxDefaultSize, 0, wxT("WxStaticText3"));
	m_pStaticPriceMethod->SetForegroundColour(wxColour(255,0,0));

	int xoff = 200;
	int yoff = 182;
	new wxStaticBox(this, wxID_ANY, LOADSTRING(CONFIG_CLEAR_PRE_SET), wxPoint(471+xoffset+xoff, 210+offset-yoff), wxSize(190, 110));

	m_pRadioNotClear = new wxRadioButton(this, wxID_ANY, LOADSTRING(DO_NOT_CLEAR), wxPoint(484+xoffset+xoff, 231+offset-yoff), wxSize(170, 17),0/*1342242825/*1342373897*/, wxDefaultValidator, wxT("WxRadioButton1"));
	m_pRadioClearAll = new wxRadioButton(this, wxID_ANY, LOADSTRING(CLEAR_ALL), wxPoint(484+xoffset+xoff, 250+offset-yoff), wxSize(170, 17),0, wxDefaultValidator, wxT("WxRadioButton2"));
	m_pRadioClearPriceAndVol = new wxRadioButton(this, wxID_ANY, LOADSTRING(CLEAR_PRICE_AND_VOLUME), wxPoint(484+xoffset+xoff, 271+offset-yoff), wxSize(170, 17), 0/*wxRB_SINGLE/*wxRB_GROUP*/, wxDefaultValidator, wxT("WxRadioButton3"));
	m_pRadioClearHand = new wxRadioButton(this, wxID_ANY, LOADSTRING(ONLY_CLEAR_VOLUME), wxPoint(484+xoffset+xoff, 292+offset-yoff), wxSize(170, 17), 0/*wxRB_SINGLE/*wxRB_GROUP*/, wxDefaultValidator, wxT("WxRadioButton4"));

	wxImageList* pImageList = new wxImageList(16, 16, true,2);
	pImageList->Add( wxIcon(_T("zqSORT_UP") , wxBITMAP_TYPE_ICO_RESOURCE,16,16) );
	pImageList->Add( wxIcon(_T("zqSORT_DOWN"), wxBITMAP_TYPE_ICO_RESOURCE,16,16) );
	m_pList->SetImageList( pImageList, wxIMAGE_LIST_SMALL);

	m_bChanged = false;

	m_pList->InsertColumn(0, LOADSTRING(EXCHANGE));
	m_pList->InsertColumn(1, LOADSTRING(CONTRACTID2));
	m_pList->InsertColumn(2, LOADSTRING(DEFAULT_VOLUME));
	m_pList->InsertColumn(3, LOADSTRING(STOP_PROFIT_PRICE_OFFSET));
	m_pList->InsertColumn(4, LOADSTRING(STOP_LOSE_PRICE_OFFSET));
	m_pList->InsertColumn(5, LOADSTRING(REVERSE_LIMIT_PRICE_OFFSET));
	m_pList->SetColumnWidth(3,100);
	m_pList->SetColumnWidth(4,100);
	m_pList->SetColumnWidth(5,100);

	m_pList->SetColumnCtrlType(2,COLUMN_CTRL_TYPE_EDIT);
	m_pList->SetColumnCtrlType(3,COLUMN_CTRL_TYPE_EDIT);
	m_pList->SetColumnCtrlType(4,COLUMN_CTRL_TYPE_EDIT);
	m_pList->SetColumnCtrlType(5,COLUMN_CTRL_TYPE_EDIT);

    ReadCfg();

    ReadInstrumentList();
    Init();
}

void J_WeiTuoCanShuDlg::OnReturn(wxCommandEvent& event)
{
	if(m_bChanged)
	{
		if(wxMessageBox(LOADSTRING(CONFIG_IS_CHANGED),LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO)!=wxYES)
		{
			return;
		}
	}

	if( m_bSungardStyle )
	{
		event.Skip();
	}
	else
	{
		this->GetParent()->Close();
	}
}

void J_WeiTuoCanShuDlg::OnSaveAndReturn(wxCommandEvent& event)
{
	WriteCfg();
	m_bChanged = false;

	CLOSE_FLOAT_PANEL();
}

void J_WeiTuoCanShuDlg::OnCom(wxCommandEvent& event)
{
	string str =  m_pComGroup->GetValue();
	m_pList->DeleteAllItems();

	for( unsigned int i=0; i<m_vecGroupInfo.size(); i++)
	{
		if(m_vecGroupInfo[i].GroupName == str)
		{
			for( unsigned int j= 0; j<m_vecGroupInfo[i].InstrVec.size(); j++)
			{
				m_pList->InsertItem(j,str);
				m_pList->SetItem(j,1,m_vecGroupInfo[i].InstrVec[j].id);
			}
		}
	}

	SetDefaultVolume();
}

void J_WeiTuoCanShuDlg::OnSet(wxCommandEvent& event)
{
	J_XiaDanShuRuShunXuDlg dlg(this);
	dlg.ShowModal();
}

void J_WeiTuoCanShuDlg::Init()
{
	if ( m_vecGroupInfo.empty() )
	{
		return;
	}

	for( unsigned  int i=0; i<m_vecGroupInfo.size(); i++)
	{
		m_pComGroup->Append(m_vecGroupInfo[i].GroupName);
	}

	string strDefaultGroup = m_vecGroupInfo[0].GroupName;
	m_pComGroup->SetValue( strDefaultGroup );
	for( unsigned int i=0; i<m_vecGroupInfo.size(); i++)
	{
		if(m_vecGroupInfo[i].GroupName == strDefaultGroup )
		{
			for( unsigned int j= 0; j<m_vecGroupInfo[i].InstrVec.size(); j++)
			{
				m_pList->InsertItem(j, strDefaultGroup );
				m_pList->SetItem(j,1,m_vecGroupInfo[i].InstrVec[j].id);
			}
		}
	}

	SetDefaultVolume();
}


void J_WeiTuoCanShuDlg::SetDefaultVolume()
{
	for( int i=0; i< (int)m_vVarietyInfo.size(); i++ )
	{
		std::string str = m_vVarietyInfo[i].InstruName;
		for( int j=0; j< m_pList->GetItemCount(); j++ )
		{
			std::string strName = m_pList->GetSubItemText(j,1);
			if( strName == str)
			{
				char szBuf[50];
				memset(szBuf, 0, sizeof(szBuf));
				if( m_vVarietyInfo[i].volume == -1)
					sprintf(szBuf,"");
				else
					sprintf(szBuf,"%d",m_vVarietyInfo[i].volume );
				m_pList->SetItem(j,2,szBuf);
				m_pList->SetItem(j,3,m_vVarietyInfo[i].profit);
				m_pList->SetItem(j,4,m_vVarietyInfo[i].loss);
				m_pList->SetItem(j,5,m_vVarietyInfo[i].reverse);
				break;
			}
		}
	}
}

void  J_WeiTuoCanShuDlg::ReadCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	m_vVarietyInfo = *(pMgr->GetVarietyInfoVector());
	LPORDER_CFG p = pMgr->GetOrderCfg();
	switch( p->ActionAfterOrder)
	{
	case 1:
		m_pRadioNotClear->SetValue(true);
		break;
	case 2:
		m_pRadioClearAll->SetValue(true);
		break;
	case 3:
		m_pRadioClearPriceAndVol->SetValue(true);
		break;
	case 4:
		m_pRadioClearHand->SetValue( true );
		break;
	}

	switch( p->bOrderConfirm)
	{
	case 1:
		m_pChkOrderConfirm->SetValue(true);
		break;
	case 0:
		m_pChkOrderConfirm->SetValue( false );
		break;
	}

	//switch( p->bOrderFailDlg)
	//{ 
	//case 1:
	//	m_pChkOrderPrompt->SetValue(true);
	//	break;
	//case 0:
	//	m_pChkOrderPrompt->SetValue(false);
	//	break;
	//}

	int i =  p->PositionVolume;
	char buf[20];
	sprintf( buf,"%d",i);
	m_pSpinCtrl->SetValue(buf);

	CANCEL_ORDER_CFG cancelOrderCfg = pMgr->GetCancelOrderCfg();
	m_pChkCancelOrder->SetValue( cancelOrderCfg.bCancelConfirm );
}

void  J_WeiTuoCanShuDlg::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	*(pMgr->GetVarietyInfoVector()) = m_vVarietyInfo;

	LPORDER_CFG p = pMgr->GetOrderCfg();
	p->bOrderConfirm = m_pChkOrderConfirm->GetValue();
	if( m_pRadioNotClear->GetValue())
		p->ActionAfterOrder = 1;
	if( m_pRadioClearAll->GetValue() )
		p->ActionAfterOrder =2;
	if( m_pRadioClearPriceAndVol->GetValue())
		p->ActionAfterOrder = 3;
	if ( m_pRadioClearHand->GetValue())
	{
		p->ActionAfterOrder = 4;
	}
	//p->bOrderSuccessDlg = m_pChkOrderPrompt->GetValue();
	//p->bOrderFailDlg = m_pChkOrderPrompt->GetValue();
	//p->bTradeDlg = m_pChkOrderPrompt->GetValue();
	p->PositionVolume = m_pSpinCtrl->GetValue();

	CANCEL_ORDER_CFG cancelOrderCfg = pMgr->GetCancelOrderCfg();
	cancelOrderCfg.bCancelConfirm = m_pChkCancelOrder->GetValue();
	cancelOrderCfg.bCancelFailDlg = m_pChkCancelOrder->GetValue();
	cancelOrderCfg.bCancelSuccessDlg = m_pChkCancelOrder->GetValue();
	pMgr->SetCancelOrderCfg( cancelOrderCfg );
}

void J_WeiTuoCanShuDlg::OnSize( wxSizeEvent& event )
{
	wxRect rt = this->GetRect();

	m_pStaticModifyOrderParam->SetSize(442, rt.height-100);
	m_pList->SetSize(420, rt.height-170);
}

void J_WeiTuoCanShuDlg::BeginEdit(wxExtListEvent& event)
{

}

void J_WeiTuoCanShuDlg::EndEdit(wxExtListEvent& event)
{
	int nRow = event.GetIndex();
	int nCol = event.GetColumn();
	string strValue = event.GetLabel();
	if ( nRow >= m_pList->GetItemCount() )
	{
		return;
	}

	string strInstrument = m_pList->GetSubItemText( nRow, 1);
	string strVolumn = m_pList->GetSubItemText( nRow, 2);
	string strProfitPriceOffset = m_pList->GetSubItemText( nRow, 3);
	string strLostPriceOffset = m_pList->GetSubItemText( nRow, 4);
	string strLimitPriceOffset = m_pList->GetSubItemText( nRow, 5);
	switch(nCol)
	{
	case 2:
		strVolumn = strValue;
		break;
	case 3:
		strProfitPriceOffset = strValue;
		break;
	case 4:
		strLostPriceOffset = strValue;
		break;
	case 5:
		strLimitPriceOffset = strValue;
		break;
	default:
		break;
	}

	vector<VarietyInfo>::iterator it = m_vVarietyInfo.begin();
	bool bFind = false;

    for ( ; it!=m_vVarietyInfo.end();)
	{
		if ( strInstrument == (*it).InstruName )
		{
			bFind = true;
			m_bChanged = true;
			if ( strVolumn.empty() && strProfitPriceOffset.empty()
				&& strLostPriceOffset.empty() && strLimitPriceOffset.empty())
			{
				it=m_vVarietyInfo.erase( it );
			}
			else
			{
				(*it).volume = atoi( strVolumn.c_str());
				strncpy((*it).profit, strProfitPriceOffset.c_str(), sizeof((*it).profit)-1);
				strncpy((*it).loss, strLostPriceOffset.c_str(), sizeof((*it).loss)-1);
				strncpy((*it).reverse, strLimitPriceOffset.c_str(), sizeof((*it).reverse)-1);
                ++it;      
				break;
			}
		}
        else
            ++it;
	}

	if ( !bFind )
	{
		VarietyInfo info;
		strncpy(info.InstruName, strInstrument.c_str(), sizeof(info.InstruName)-1);
		info.volume = atoi( strVolumn.c_str());
		strncpy(info.profit, strProfitPriceOffset.c_str(), sizeof(info.profit)-1);
		strncpy(info.loss, strLostPriceOffset.c_str(), sizeof(info.loss)-1);
		strncpy(info.reverse, strLimitPriceOffset.c_str(), sizeof(info.reverse)-1);
		m_vVarietyInfo.push_back(info);
		m_bChanged = true;
	}
}

void J_WeiTuoCanShuDlg::OnText(wxExtListEvent& event)
{

}

void J_WeiTuoCanShuDlg::OnRadioButton(wxCommandEvent& event)
{
	m_bChanged = true;
}
void J_WeiTuoCanShuDlg::OnCheck(wxCommandEvent& event)
{
	m_bChanged = true;
}

int wxCALLBACK MyFunction(const wxExtListItem& item1,const wxExtListItem& item2,long sortData )
{

	//List1->SetItemData(j,(long)&m_GroupInfo[i].InstrVec[j]);
	string str1 = item1.GetText();
	string str2 = item2.GetText();

	int i = stricmp(str1.c_str(), str2.c_str());
	if( i==0)
		return 0;
	else if( sortData==0) //½µÐò
	{
		return ~i;
	}
	else
	{
		return i;

	}

}

void J_WeiTuoCanShuDlg::OnColClick(wxExtListEvent& event)
{
	int colcount = m_pList->GetColumnCount();
	for( int i=0; i<colcount; i++)
		m_pList->SetColumnImage(i,-1);
	int col = event.GetColumn();
	long iasc=event.GetSortAsc()?1:0;

	m_pList->SetColumnImage(col,iasc? 0:1);
	m_pList->SortItems(MyFunction,event.GetColumn(),(long) iasc);
}

void J_WeiTuoCanShuDlg::Sel(wxListEvent& event)
{
}

void J_WeiTuoCanShuDlg::OnPanelCharHook(wxCommandEvent& evt)
{
	wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();
	if (pEvent->GetKeyCode () == WXK_TAB)
	{
		bool backward = pEvent->ShiftDown ();
		wxWindow *win = FindFocus();
		if (win == NULL) return;
		if(backward)
		{
			if(win == m_pRadioClearHand ){m_pRadioClearPriceAndVol->SetFocus();evt.Skip();}
			else if(win==m_pRadioClearPriceAndVol)	{m_pRadioClearAll->SetFocus();evt.Skip();}	
			else if(win==m_pRadioClearAll)	{m_pRadioNotClear->SetFocus();evt.Skip();}	
			else if(win==m_pRadioNotClear)	{BtSet->SetFocus();evt.Skip();}	
			else{}
		}
		else
		{
			if(win==m_pSpinCtrl )			{m_pRadioNotClear->SetFocus();evt.Skip();}
			else if(win==m_pRadioNotClear)		{m_pRadioClearAll->SetFocus();evt.Skip();}
			else if(win==m_pRadioClearAll)		{m_pRadioClearPriceAndVol->SetFocus();evt.Skip();}
			else if(win==m_pRadioClearPriceAndVol)		{m_pRadioClearHand->SetFocus();evt.Skip();}
			else{}
		}
	}
}

void J_WeiTuoCanShuDlg::ReadInstrumentList()
{
	std::map<std::string, std::vector<InstrumentInfo>> mapExchToInstrumentID;
	std::set<std::string> outData;
	outData.clear();
	if(CURRENT_SVR()) CURRENT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it = outData.begin();
	for ( ; it != outData.end(); ++it)
	{
		PlatformStru_InstrumentInfo info;
		memset(&info,0,sizeof(info));
		if(CURRENT_SVR()) CURRENT_SVR()->GetInstrumentInfo( *it, info );
		std::string strExchange = info.ExchangeID;
        strExchange = CURRENT_SVR()?CURRENT_SVR()->ExchangeID2String( strExchange.c_str()):"";
		InstrumentInfo instrument;
		instrument.id = info.InstrumentID;
		instrument.name = info.InstrumentName;
		std::map<std::string, std::vector<InstrumentInfo>>::iterator it_find 
			= mapExchToInstrumentID.find( strExchange );
		if ( it_find == mapExchToInstrumentID.end())
		{
			vector<InstrumentInfo> vec;
			vec.push_back( instrument );
			mapExchToInstrumentID[strExchange] = vec;
		}
		else
		{
			mapExchToInstrumentID[strExchange].push_back( instrument );
		}
	}

	std::map<std::string, std::vector<InstrumentInfo>>::iterator it_map 
		= mapExchToInstrumentID.begin();
	for ( ; it_map != mapExchToInstrumentID.end(); ++it_map )
	{
		GroupInfo groupInfo;
		groupInfo.GroupName = it_map->first;
		groupInfo.InstrVec = it_map->second;
		m_vecGroupInfo.push_back( groupInfo );
	}
}
void J_WeiTuoCanShuDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pStaticModifyOrderParam->SetLabel(LOADSTRING(CONFIG_MODIFY_ORDER_PARAM));
	m_pStaticExchange->SetLabel(LOADSTRING(EXCHANGE));
	m_pChkOrderConfirm->SetLabel(LOADSTRING(CONFIG_ORDER_CONFIRM));
	m_pChkOrderPrompt->SetLabel(LOADSTRING(ORDER_PROMPT));
	m_pChkCancelOrder->SetLabel(LOADSTRING(CONFIRM_BEFORE_CANCEL_ORDER));
	m_pStaticPriceTicker->SetLabel(LOADSTRING(CONFIG_PRICE_TICKER));
	m_pStaticPriceMethod->SetLabel(LOADSTRING(CONFIG_PRICE_METHOD));
	m_pRadioNotClear->SetLabel(LOADSTRING(DO_NOT_CLEAR));
	m_pRadioClearAll->SetLabel(LOADSTRING(CLEAR_ALL));
	m_pRadioClearPriceAndVol->SetLabel(LOADSTRING(CLEAR_PRICE_AND_VOLUME));
	m_pRadioClearHand->SetLabel(LOADSTRING(ONLY_CLEAR_VOLUME));

	m_pList->SetColumn(0, LOADSTRING(EXCHANGE));
	m_pList->SetColumn(1, LOADSTRING(CONTRACTID2));
	m_pList->SetColumn(2, LOADSTRING(DEFAULT_VOLUME));
	m_pList->SetColumn(3, LOADSTRING(STOP_PROFIT_PRICE_OFFSET));
	m_pList->SetColumn(4, LOADSTRING(STOP_LOSE_PRICE_OFFSET));
	m_pList->SetColumn(5, LOADSTRING(REVERSE_LIMIT_PRICE_OFFSET));
}
