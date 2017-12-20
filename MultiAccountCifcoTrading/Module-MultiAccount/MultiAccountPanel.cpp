#include "Stdafx.h"
#include "MultiAccountPanel.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/MultiAccountCore/CTPLogin.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "../inc/cfgMgr/CfgGlobal.h"
#include "MAUserEditDlg.h"
#include "MAInfoSynSetupDlg.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/zqControl.h"
#include "wx/file.h"

extern PlatformSvrMgr* g_pPlatformMgr;
MultiAccountPanel* MultiAccountPanel::m_MultiAccountPanel=NULL;
BEGIN_EVENT_TABLE(MultiAccountPanel,wxPanel) 
	EVT_SIZE(OnSize)
	EVT_BUTTON(ID_SYNTYPE,OnSynType)
	EVT_BUTTON(ID_ALGO_SETUP,OnAlgoSetup)
	EVT_BUTTON(ID_ADD,OnAdd)		
	EVT_BUTTON(ID_EDIT,OnEdit)	
	EVT_BUTTON(ID_DEL,OnDel)	
	EVT_BUTTON(ID_QUOT_SETUP,OnQuotSetup)
	EVT_BUTTON(ID_LOGOUT_SEL,OnLogoutSel)
	EVT_BUTTON(ID_LOGIN_CHECKED,OnLoginChecked)
	EVT_BUTTON(ID_LOGIN_ALL,OnLoginAll)
	EVT_BUTTON(ID_REQUERY,OnReQuery)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_FUND_UPDATE,OnMaFundUpdate)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_UI_CALLBACK,OnUICallBack)
	EVT_CHOICE(ID_CHOICE,OnStrategyChoice)
	EVT_CHOICE(ID_USERGROUP,OnUserChoice)	
	EVT_COMMAND(wxID_ANY,wxEVT_MA_LIST_UPDATE,OnMaListUpdate)
	EVT_LIST_EXT_CHECKBOX_CHANGED(wxID_ANY,OnListCheckbox)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
	EVT_LIST_EXT_CONTEXT_MENU(OnContext)
	EVT_LIST_EXT_COL_CLICK(wxID_ANY, OnListColClick)
	EVT_SUBSCRIBE(OnSubscrible)
END_EVENT_TABLE()

MultiAccountPanel::MultiAccountPanel(wxWindow *parent,
									 wxWindowID winid,
									 const wxPoint& pos ,
									 const wxSize& size,
									 long style,
									 const wxString& name):
wxPanel(parent,winid,pos,size,style,name),
m_pList(NULL),
m_bFirstAccountLogin(true),
m_pMACfg(NULL)
{
	m_FieldID2ColumnID.clear();
	m_ColumnID2FieldID.clear();

	MA_CORE()->SetUICallback(SetUICallback);
	m_MultiAccountPanel=this;
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(pMgr) m_pMACfg=pMgr->GetMultiAccountCfg();
	SetBackgroundColour(DEFAULT_COLOUR);
	new wxStaticText( this, wxID_STATIC,"下单策略:", wxPoint(0,10), wxSize(70,20), 0 );
	wxArrayString items;
	items.Add("下单板指定手数");
	items.Add("基数乘数方式");
	items.Add("总数分配方式");
	items.Add("按资金向首帐户对齐");
	items.Add("按帐户规模分配手数");
	items.Add("策略板自定义手数方式");
	m_mapCfgItem[Strategy_Base]=ID_ITEM_BASEDMULTIPE;
	m_mapCfgItem[Strategy_Total]=ID_ITEM_DISTRIBUTEBYSUM;
	m_mapCfgItem[Strategy_FundDeviation]=ID_ITEM_ALIGNTOFISRTACCOUNT;
	m_mapCfgItem[Strategy_AccountSize]=ID_ITEM_ACCOUNTSCALEDIVEDED;
	m_mapCfgItem[Strategy_UserDefine]=ID_ITEM_CUSTOMVOLUMN;
	wxChoice* pChoice=new wxChoice( this, ID_CHOICE,wxPoint(70,8), wxSize(150,24), items, 0 );
	pChoice->SetSelection(m_pMACfg?m_pMACfg->nStrategyItem:-1);
	wxButton* pBtn=new wxButton( this, ID_ALGO_SETUP,"策略参数设置",wxPoint(223,8), wxSize(90,24), 0 );
	pBtn->Enable(m_mapCfgItem.find(pChoice->GetSelection())!=m_mapCfgItem.end());
	new wxButton( this, ID_SYNTYPE,"用户资料存储设置",wxPoint(320,8), wxSize(120,24), 0 );
	new wxStaticText( this, wxID_STATIC,"用户分组:", wxPoint(445,10), wxSize(60,20), 0 );
	wxChoice* combMAUser=new wxChoice( this, ID_USERGROUP,wxPoint(510,8), wxSize(150,24), NULL, 0 );
	int Agsel=SetAccountComboBox(combMAUser);
	combMAUser->SetSelection(-1);

	m_pList=new wxExtListCtrl(this,ID_LISTCTRL,wxPoint(0,35),wxSize(400,300),
		wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT);

	InitListCtrl();
	//OnMaUserChange(Agsel);
	new wxButton( this, ID_ADD,"添加",wxPoint(410,35), wxSize(60,24), 0 );
	new wxButton( this, ID_EDIT,"编辑",wxPoint(410,65), wxSize(60,24), 0 );
	new wxButton( this, ID_DEL,"删除",wxPoint(410,95), wxSize(60,24), 0 );
	new wxButton( this, ID_QUOT_SETUP,"配置行情",wxPoint(410,140), wxSize(60,24), 0 );
	new wxButton( this, ID_LOGIN_CHECKED,"登录勾选",wxPoint(410,170), wxSize(60,24), 0 );
	new wxButton( this, ID_LOGIN_ALL,"登录全部",wxPoint(410,200), wxSize(60,24), 0 );
	new wxButton( this, ID_LOGOUT_SEL,"退出登录",wxPoint(410,230), wxSize(60,24), 0 );
	new wxButton( this, ID_REQUERY,"重新查询",wxPoint(410,260), wxSize(60,24), 0 );
}
MultiAccountPanel::~MultiAccountPanel()
{

}

void MultiAccountPanel::ResetValidColText(int nRow, BOOL bIsCheck)
{
	if(m_pList==NULL)
		return;
	if(nRow<0 || nRow>=m_pList->GetItemCount())
		return;

	wxString newText;
	std::map<int, long>::iterator it = 
			m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_VILID);

	if(it!=m_FieldID2ColumnID.end()) {
		newText.Printf("%s", (bIsCheck?" ":""));
		m_pList->SetItem(nRow, it->second, newText);
	}
}

void MultiAccountPanel::OnSize(wxSizeEvent& evt)
{
	int width=evt.GetSize().x;
	int height=evt.GetSize().y;
	if(m_pList)
	{
		m_pList->SetSize(0,35,width-75,height-35);
		FindWindowById(ID_ADD,this)->SetSize(width-65,-1,60,-1);
		FindWindowById(ID_EDIT,this)->SetSize(width-65,-1,60,-1);
		FindWindowById(ID_DEL,this)->SetSize(width-65,-1,60,-1);
		FindWindowById(ID_QUOT_SETUP,this)->SetSize(width-65,-1,60,-1);
		FindWindowById(ID_LOGOUT_SEL,this)->SetSize(width-65,-1,60,-1);
		FindWindowById(ID_LOGIN_CHECKED,this)->SetSize(width-65,-1,60,-1);
		FindWindowById(ID_LOGIN_ALL,this)->SetSize(width-65,-1,60,-1);	
		FindWindowById(ID_REQUERY,this)->SetSize(width-65,-1,60,-1);
	}

}
void MultiAccountPanel::OnSynType(wxCommandEvent& evt)
{
	CMAInfoSynSetupDlg dlg(this);
	dlg.ShowModal();
}
void MultiAccountPanel::OnAlgoSetup(wxCommandEvent& evt)
{
	wxCommandEvent request_event(wxEVT_LOAD_CONFIG_APP);
	wxChoice* pChoice=(wxChoice*)FindWindowById(ID_CHOICE,this);
	request_event.SetInt(m_mapCfgItem[pChoice->GetSelection()]);
	GETTOPWINDOW()->ProcessEvent(request_event);
}
void MultiAccountPanel::OnAdd(wxCommandEvent& evt)
{

	CMAUserEditDlg dlg(NULL);
	if(dlg.ShowModal()==wxID_OK)
	{
		CTPTradeAccount acc;
		ZeroMemory(&acc,sizeof(CTPTradeAccount));
		dlg.GetTradeAccount(acc);
		//获取新账号的ID
		acc.t.nID=MA_CORE()->GetNextTradeAccountId();
		MA_CORE()->AddTradeAccount(acc);
		InsertOneRow(m_pList->GetItemCount(),&acc);	
		m_pList->ReSortItems();
	}

}
void MultiAccountPanel::OnEdit(wxCommandEvent& evt)
{
	int sel=-1,row=m_pList->GetFirstSelected();
	if(row>=0)
		sel=m_pList->GetItemData(row);
	if(sel>=0&&sel<(int)MA_CORE()->GetTradeAccountCount())
	{
		const CTPTradeAccount* pAcc=MA_CORE()->GetTradeAccount(sel);
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(pAcc->t.szTradeAccount);
		if(pSvr)
		{
			wxMessageBox("该账号已经登录过，不能编辑!",LOADSTRING(USERLOGINDLG_INFORMATION));
			return;
		}
		CMAUserEditDlg dlg(pAcc);
		if(dlg.ShowModal()==wxID_OK)
		{
			CTPTradeAccount acc_new;
			ZeroMemory(&acc_new,sizeof(CTPTradeAccount));
			dlg.GetTradeAccount(acc_new);
			//strcpy_s(pAcc->t.szNickName,acc_new.t.szNickName);		
			//strcpy_s(pAcc->t.szTradeAccount,acc_new.t.szTradeAccount);			
			//strcpy_s(pAcc->t.szPassword,acc_new.t.szPassword);				
			//pAcc->t.nBrokerID=acc_new.t.nBrokerID;				
			//pAcc->t.nServerNo=acc_new.t.nServerNo;
			//strcpy_s(pAcc->DynamicPassword,acc_new.DynamicPassword);
            MA_CORE()->SetTradeAccount(sel,acc_new);
			ModifyOneRow(row,&acc_new);
			m_pList->ReSortItems();
		}
	}
	else
		wxMessageBox("请选择帐号",LOADSTRING(USERLOGINDLG_INFORMATION));
}
void MultiAccountPanel::OnDel(wxCommandEvent& evt)
{
	vector<int> vecRows;
	wxString strPrompt="是否确定删除下列行数据：\r\n";
	for (int row=0;row<m_pList->GetItemCount();++row)
	{
		if(m_pList->IsSelected(row))
		{
			vecRows.push_back(row);
			strPrompt+=wxString::Format("第%d行账号:%s",row+1,MA_CORE()->GetTradeAccountName(m_pList->GetItemData(row)).c_str());
		}
	}
	if(vecRows.size()==0)
	{
		wxMessageBox("请选择帐号",LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(wxMessageBox(strPrompt,LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO)==wxYES)
	{
		DeleteItem(vecRows);
	}
}
void MultiAccountPanel::DeleteItem(vector<int> vecRows)
{
	for (int i=vecRows.size()-1;i>=0;--i)
	{
		int row=vecRows[i];
		int sel=m_pList->GetItemData(row);
		if(sel>=0&&sel<MA_CORE()->GetTradeAccountCount())
		{
			const CTPTradeAccount* pAccount=MA_CORE()->GetTradeAccount(sel);
			string strName=pAccount->t.szTradeAccount;
			IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(strName);
			if(pSvr)
			{
				if(pSvr->GetMACurrentAccount())
				{
					wxCommandEvent evt(wxEVT_MA_PLAT_MODIFY,GETTOPWINDOW()->GetId());
					evt.SetInt(NULL);
					GETTOPWINDOW()->ProcessEvent(evt);
				}
				wxCommandEvent evt(wxEVT_MA_PLAT_DELETE,GETTOPWINDOW()->GetId());
				evt.SetInt((int)pSvr);
				GETTOPWINDOW()->ProcessEvent(evt);
				pSvr->StopThread();
				pSvr->Stop();
				g_pPlatformMgr->ReleaseAccount(strName);
			}
			//从配置信息中删除
			CFG_MGR_DEFAULT()->DelAccount(pAccount->t.nID);
			MA_CORE()->EraseTradeAccount(strName,false);
		}
		for (int index=0;index<m_pList->GetItemCount();++index)
		{
			int seldata=m_pList->GetItemData(index);
			if(seldata>sel)
			{
				m_pList->SetItemData(index,--seldata);
			}
		}
		m_pList->DeleteItem(row);
	}
}
void MultiAccountPanel::OnQuotSetup(wxCommandEvent& evt)
{
	int ret=wxNO,row=m_pList->GetFirstSelected();
	int sel=m_pList->GetItemData(row);
	if(sel<0||sel>=MA_CORE()->GetTradeAccountCount()) 
	{
		wxMessageBox("请选择帐号",LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	int pSvr=(int)g_pPlatformMgr->GetPlatformSvr(MA_CORE()->GetTradeAccountName(sel));
	if(pSvr==0)
	{
		wxMessageBox("该帐号没有登录",LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	if(MA_CORE()->GetTradeAccount(sel)->UseQuot)
	{
		wxMessageBox("已经使用该账号行情",LOADSTRING(USERLOGINDLG_INFORMATION));
		return;
	}
	wxString oldname;
	for (int i=0;i<MA_CORE()->GetTradeAccountCount();++i)
	{
		if(MA_CORE()->GetTradeAccount(i)->UseQuot)
		{
			oldname=MA_CORE()->GetTradeAccountName(i);
			break;
		}
	}
	if(oldname.IsEmpty())
		ret=wxMessageBox(wxString::Format("使用账号[%s]的行情?",MA_CORE()->GetTradeAccountName(sel).c_str()),LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO);
	else if(oldname!=MA_CORE()->GetTradeAccountName(sel).c_str())
		ret=wxMessageBox(wxString::Format("使用账号[%s]代替现用账号[%s]的行情?",MA_CORE()->GetTradeAccountName(sel).c_str(),oldname),LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO);
	else
		wxMessageBox(wxString::Format("当前正在使用账号[%s]的行情，无需切换",oldname),LOADSTRING(USERLOGINDLG_INFORMATION),wxOK);
	if(ret==wxYES)
	{
		
		for (int i=0;i<MA_CORE()->GetTradeAccountCount();++i)
		{
			MA_CORE()->SetTradeAccountUseQuot(i,i==sel);
			IPlatformSingleSvr* piSvr=g_pPlatformMgr->GetPlatformSvr(MA_CORE()->GetTradeAccountName(i));
			if(piSvr) piSvr->SetMACurrentAccount(i==sel);
		}
		SetQuotUse(row);
		wxCommandEvent evt(wxEVT_MA_PLAT_MODIFY,GETTOPWINDOW()->GetId());
		evt.SetInt(pSvr);
		GETTOPWINDOW()->ProcessEvent(evt);
	}
}
void MultiAccountPanel::OnLogoutSel(wxCommandEvent& evt)
{
	int row=m_pList->GetFirstSelected();
	if ( row < 0 || row >= m_pList->GetItemCount())
	{
		return;
	}

	int sel=m_pList->GetItemData(row);

	const CTPTradeAccount* pAccount=MA_CORE()->GetTradeAccount(sel);
	string strName=pAccount->t.szTradeAccount;
	IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(strName);
	if(pSvr)
	{
		if(pSvr->GetMACurrentAccount())
		{
			wxCommandEvent evt(wxEVT_MA_PLAT_MODIFY,GETTOPWINDOW()->GetId());
			evt.SetInt(NULL);
			GETTOPWINDOW()->ProcessEvent(evt);
		}
		wxCommandEvent evt(wxEVT_MA_PLAT_DELETE,GETTOPWINDOW()->GetId());
		evt.SetInt((int)pSvr);
		GETTOPWINDOW()->ProcessEvent(evt);
		pSvr->StopThread();
		pSvr->Stop();
		g_pPlatformMgr->ReleaseAccount(strName);
		MA_CORE()->EraseTradeAccount(strName,true);	

		map<string,PlatformStru_TradingAccountInfo>::iterator it_account = m_mapAccountInfo.find(strName);
		if ( it_account != m_mapAccountInfo.end())
		{
			m_mapAccountInfo.erase(it_account);
		}

		for (int col=0;col<m_pList->GetColumnCount();++col)
		{
			if(m_ColumnID2FieldID.find(col)==m_ColumnID2FieldID.end())
				continue;

			switch(m_ColumnID2FieldID[col])
			{
			case MULTIACCOUNT_FIELD_ID_STATUS:
				m_pList->SetItem(row,col,"未登录");
				break;
			case MULTIACCOUNT_FIELD_ID_USEQUOT:
			case MULTIACCOUNT_FIELD_ID_AVALIABLE:	//可用资金
			case MULTIACCOUNT_FIELD_ID_PREBALANCE:	//上次结算准备金
			case MULTIACCOUNT_FIELD_ID_STATICPROFIT:	//静态权益
			case MULTIACCOUNT_FIELD_ID_DYNAMICPROFIT:	//动态权益
			case MULTIACCOUNT_FIELD_ID_POSITIONPROFIT:	//持仓盈亏
			case MULTIACCOUNT_FIELD_ID_CLOSEPROFIT:		//平仓盈亏
			case MULTIACCOUNT_FIELD_ID_RISKDEGREE:		//风险度
				m_pList->SetItem(row,col,wxEmptyString);
			default:
				break;
			}
		}
	}

}

void MultiAccountPanel::OnLoginChecked(wxCommandEvent& evt)
{
	int nCount = m_pList->GetItemCount();
	int bLoginAgain=0;
	for ( int i = 0; i < nCount; i++ )
	{
		if ( m_pList->GetItemChecked(i))
		{
			int ret=LoginSpecialAccount(i);
			if(ret==0)
			{
				bLoginAgain++;
			}
			else if(ret<0)
			{
				SetLoginStatus(i,"参数错误");
			}
		}
	}
	if(bLoginAgain>0)
		wxMessageBox(wxString::Format("选中账号中有%d个已经登录",bLoginAgain));
}

void MultiAccountPanel::OnLoginAll(wxCommandEvent& evt)
{
	for (int row=0;row<m_pList->GetItemCount();row++)
	{
		int sel=m_pList->GetItemData(row);
		if(LoginSpecialAccount(sel)<0)
		{
			SetLoginStatus(row,"参数错误");
		}
	}
}

void MultiAccountPanel::InsertOneRow(int index, const CTPTradeAccount* acc )
{
	if ( m_pList->GetColumnCount() == 0 )
	{
		return;
	}

	m_pList->InsertItem(index,"");	
	m_pList->SetItemData(index,index);
	m_pList->SetItemChecked(index,acc->StrategyValid);
	ResetValidColText(index, acc->StrategyValid);
	ModifyOneRow(index,acc);
	if(Strlen(acc->StatusText)==0)
		SetLoginStatus(index,"未登录");
	else
		SetLoginStatus(index,acc->StatusText);
}
void MultiAccountPanel::ModifyOneRow(int index, const CTPTradeAccount* acc )
{
	std::map<int, long>::iterator it = m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_ID);
	if ( it != m_FieldID2ColumnID.end())
	{
		wxExtListItem  info;
		info.SetId(index);
		info.SetColumn(it->second);
		info.SetText(wxString::Format("%d",acc->t.nID));
		info.SetiColData(acc->t.nID);
		m_pList->SetItem(info);
	}
	
	it = m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_INVESTOR);
	if ( it != m_FieldID2ColumnID.end())
	{
		m_pList->SetItem(index, it->second,acc->t.szNickName);
	}
	
	it = m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_ACCOUNT);
	if ( it != m_FieldID2ColumnID.end())
	{
		m_pList->SetItem(index,it->second,acc->t.szTradeAccount);
	}
}
void MultiAccountPanel::SetLoginStatus(int row,wxString strText)
{
	int sel=m_pList->GetItemData(row);
	MA_CORE()->SetTradeAccountStatusText(sel,strText);
	std::map<int, long>::iterator it = m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_STATUS);
	if ( it == m_FieldID2ColumnID.end())
	{
		return;
	}

	if(row>=0&&row<m_pList->GetItemCount())
	{
		if(strText!=m_pList->GetSubItemText(row,it->second))
			m_pList->SetItem(row,it->second,strText);
	}
}
void MultiAccountPanel::SetQuotUse(int row)
{
	std::map<int, long>::iterator it = m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_USEQUOT);
	if ( it == m_FieldID2ColumnID.end())
	{
		return;
	}

	for (int i=0;i<m_pList->GetItemCount();++i)
	{
		if(i==row)
			m_pList->SetItem(i,it->second,"使用");
		else
			m_pList->SetItem(i,it->second,wxEmptyString);
	}
}
int MultiAccountPanel::LoginSpecialAccount( int row )
{
	if(row<0||row>=m_pList->GetItemCount())
		return -1;
	int sel=m_pList->GetItemData(row);
	//选中项判断
	SERVER_PARAM_CFG& thisServerParamCfg=DEFAULT_SVR()->GetServerParamCfg();
	if(sel<0||sel>=MA_CORE()->GetTradeAccountCount()) 
		return -1;
	const CTPTradeAccount* thisTrader=MA_CORE()->GetTradeAccount(sel);
	if(thisTrader==NULL)
		return -1;
	//合约代码判断
	string strCode;
	int i,count=MA_CORE()->m_vecBrokerInfo.size();
	for (i=0;i<count;++i)
	{
		if(MA_CORE()->m_vecBrokerInfo[i].nBrokerID==thisTrader->t.nBrokerID)
		{
			strCode=MA_CORE()->m_vecBrokerInfo[i].szCode;
			break;
		}
	}
	if(strCode.empty()) 
		return -1;

	//服务器组判断
	vector<string> tradersvr,quotsvr;
	multimap<int,ServerGroup>::iterator SvrGrpIt;
	count=MA_CORE()->m_mapServerGroup.count(thisTrader->t.nBrokerID);
	if (count<1) return false;
	SvrGrpIt=MA_CORE()->m_mapServerGroup.find(thisTrader->t.nBrokerID);
	bool bSelected=false;
	for(int i=0;i<count;++i)
	{
		if(thisTrader->t.nServerNo==SvrGrpIt->second.nServerNo)
		{
			bSelected=true;
		}
		SvrGrpIt++;
	}
	if(!bSelected) return -1;
	//服务器地址判断
	multimap<int,ServerAddr>::iterator AddrIt;
	int addrcount=MA_CORE()->m_mapServerAddr.count(thisTrader->t.nServerNo);
	if(addrcount<1)	return -1;
	AddrIt=MA_CORE()->m_mapServerAddr.find(thisTrader->t.nServerNo);
	for(int i=0;i<addrcount;++i)
	{
		ServerAddr& addr=AddrIt->second;	
		string strAddr="tcp://";
		strAddr+=addr.szServerAddress;
		strAddr+=":";
		strAddr+=wxString::Format("%d",addr.nPort).c_str();
		if(addr.nServerType==0)//交易
			tradersvr.push_back(strAddr);
		else
			quotsvr.push_back(strAddr);
		AddrIt++;
	}
	//创建账户登录
	string curAcc=thisTrader->t.szTradeAccount;
	IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(curAcc);
	g_pPlatformMgr->CreateAccount(curAcc);
	if(pSvr==NULL)
	{
		pSvr=g_pPlatformMgr->GetPlatformSvr(curAcc);
		pSvr->Start();	
		CopySvr(DEFAULT_SVR(),pSvr);
	}
	else
	{
		ServiceStatus ss=pSvr->GetCurrentStatus();
		if(ss!=SS_NOLOGIN)
			return 0;
	}
	CTPLogin* pLoginCore=new CTPLogin(pSvr,this);
	pLoginCore->SetParticipantInfo(strCode,curAcc,thisTrader->t.szPassword,thisTrader->DynamicPassword); 
	pLoginCore->SetTradeFrontAddrs(tradersvr);
	pLoginCore->SetQuotFrontAddrs(quotsvr);
	pLoginCore->SetProxyConnectStr(thisServerParamCfg.ProxyType,thisServerParamCfg.ProxyUser,
		thisServerParamCfg.ProxyPwd,thisServerParamCfg.ProxyAddr,thisServerParamCfg.ProxyPort);
	pLoginCore->Login(thisTrader->t.szTradeAccount,thisTrader->t.szPassword, thisTrader->t.szPassword);
	return 1;
}

int MultiAccountPanel::GetRow(IPlatformSingleSvr* pSvr)
{
	if(pSvr==NULL) return -1;
	for (int row=0;row<m_pList->GetItemCount();row++)
	{
		int sel=m_pList->GetItemData(row);
		if(g_pPlatformMgr->GetPlatformSvr(MA_CORE()->GetTradeAccountName(sel))==pSvr)
			return row;
	}
	return -1;
}
void MultiAccountPanel::SetProgressPosition(IPlatformSingleSvr* pSvr,int TotalStep,int CurStep,int pos)
{

}
void MultiAccountPanel::DoLoginFail(IPlatformSingleSvr* pSvr,wxString strSrc)
{
	pSvr->Logout();	
	string strErrorMsg=strSrc.c_str();
	if(strErrorMsg.empty())
	{
		pSvr->GetErrorField(strErrorMsg);
		if(strErrorMsg.empty()) 
			strErrorMsg=LOADSTRING(USERLOGINDLG_FAILED);
	}
	SetPromptText(pSvr,strErrorMsg.c_str());
	SetProgressPosition(pSvr,TIMER_FINISH);
	pSvr->NotifyStartStopInitQry(false);      //通知底层，初始化查询结束
	CTPLogin* pLogin=CTPLogin::GetCTPLogin(pSvr);
	SAFE_DELETE(pLogin);
	pSvr->StopThread();
	pSvr->Stop();
	g_pPlatformMgr->ReleaseAccount(pSvr);
}
void MultiAccountPanel::SetPromptText(IPlatformSingleSvr* pSvr,wxString strText)
{
	int row=GetRow(pSvr);
	if(row>=0)
		SetLoginStatus(row,strText);
}
void MultiAccountPanel::OnQryFinish(IPlatformSingleSvr* pSvr)
{
	SetPromptText(pSvr,"登录成功");
	if(pSvr==NULL) return; 

	pSvr->SubscribeBusinessData(BID_RtnTrade, GID_MULTI_ACCOUNT, TradeCallBackFunc);	
	pSvr->SubscribeBusinessData(BID_RtnOrder, GID_MULTI_ACCOUNT, AllCallBackFunc);						
	pSvr->SubscribeBusinessData(BID_RspOrderAction1, GID_MULTI_ACCOUNT, AllCallBackFunc);
	pSvr->SubscribeBusinessData(BID_RspOrderAction2, GID_MULTI_ACCOUNT, AllCallBackFunc);
	pSvr->SubscribeBusinessData(BID_RspOrderInsert, GID_MULTI_ACCOUNT, AllCallBackFunc);
	pSvr->SubscribeBusinessData(BID_RtnDepthMarketData, GID_MULTI_ACCOUNT, AllCallBackFunc);
	pSvr->SubscribeBusinessData(BID_RspQryTradingAccount, GID_MULTI_ACCOUNT, AllCallBackFunc);
	pSvr->SubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_MULTI_ACCOUNT, AllCallBackFunc);
	pSvr->SubscribeBusinessData(BID_RspQryTradingNotice, GID_MULTI_ACCOUNT, AllCallBackFunc);
	wxCommandEvent evt(wxEVT_MA_PLAT_ADD,GETTOPWINDOW()->GetId());
	evt.SetInt((int)pSvr);
	GETTOPWINDOW()->ProcessEvent(evt);
	if(m_bFirstAccountLogin)
	{
		MA_CORE()->SetTradeAccountUseQuot(GetRow(pSvr),true);
		pSvr->SetMACurrentAccount(true);
		SetQuotUse(GetRow(pSvr));
		wxCommandEvent evtModify(wxEVT_MA_PLAT_MODIFY,GETTOPWINDOW()->GetId());
		evtModify.SetInt((int)pSvr);
		GETTOPWINDOW()->ProcessEvent(evtModify);
		m_bFirstAccountLogin=false;
	}
	int sel=GetRow(pSvr);
	if(sel>=0)
		MAUpdateOneAccount(sel,MA_CORE()->GetTradeAccountName(sel),false);
	
	m_pList->ReSortItems();
}

void MultiAccountPanel::CopySvr(IPlatformSingleSvr* pSrcSvr, IPlatformSingleSvr* pDestSvr )
{
	//复制ServerParamCfg
	pDestSvr->GetServerParamCfg()=pSrcSvr->GetServerParamCfg();

}
int MultiAccountPanel::AllCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(m_MultiAccountPanel==NULL) return 0;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(data.pPlatformSvr);
	if(data.BID==BID_RtnDepthMarketData)
	{
		bool bDepthMarketChange=false;
		DataRtnDepthMarketData& refData = *(DataRtnDepthMarketData*)(const_cast<AbstractBusinessData*>(&data));	
		if(refData.bHavePosition)
		{
			wxCommandEvent evt(wxEVT_MA_FUND_UPDATE);
			evt.SetInt((int)pSvr);
			m_MultiAccountPanel->AddPendingEvent(evt);
		}
	}
	else
	{
		wxCommandEvent evt(wxEVT_MA_FUND_UPDATE);
		evt.SetInt((int)pSvr);
		m_MultiAccountPanel->AddPendingEvent(evt);
	}
	return 0;
}
int MultiAccountPanel::TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(m_MultiAccountPanel==NULL) return 0;
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(data.pPlatformSvr);
	if(pSvr&&pSvr->GetModuleDataQry())
	{
		if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
			return 0;
	}
	wxCommandEvent evt(wxEVT_MA_FUND_UPDATE);
	evt.SetInt((int)pSvr);
	m_MultiAccountPanel->AddPendingEvent(evt);
	return 0;
}
void MultiAccountPanel::MAUpdateOneAccount(int sel,string strCurAccount,bool bRefresh)
{
	IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(strCurAccount);
	if(pSvr)
	{
		CTPLogin* pLogin=CTPLogin::GetCTPLogin(pSvr);
		if(pLogin&&pLogin->IsQryEnd())
		{
			PlatformStru_TradingAccountInfo NewAccountInfo;
			memset(&NewAccountInfo,0,sizeof(NewAccountInfo));
			if(pSvr->GetTradingAccount(strCurAccount, NewAccountInfo)==0)
			{
				//if(memcmp(&NewAccountInfo,&m_mapAccountInfo[strCurAccount],sizeof(PlatformStru_TradingAccountInfo))!=0)
				//{
					m_mapAccountInfo[strCurAccount]=NewAccountInfo;
					UpdateUIData(strCurAccount,sel);
				//}
			}
		}
	}
}
void MultiAccountPanel::OnMaFundUpdate( wxCommandEvent& event)
{
	string strCurAccount;
	for (int row=0;row<m_pList->GetItemCount();row++)
	{
		int sel=m_pList->GetItemData(row);
		strCurAccount=MA_CORE()->GetTradeAccountName(sel);
		MAUpdateOneAccount(row,strCurAccount,event.GetInt()==0);
	}
	m_pList->ReSortItems();
}
#define GETFUNDSTR(d,str,t)\
	do{\
	if(util::isInvalidValue(t))\
	str.Printf("-");	\
	else\
	str = GlobalFunc::GetAccountFormatString(t, d).c_str();\
	}while(0)
void MultiAccountPanel::UpdateUIData(string acc,int row)
{
	int digit=2;
	wxString strTemp;
	for (int col=0;col<m_pList->GetColumnCount();++col)
	{
		if(m_ColumnID2FieldID.find(col)==m_ColumnID2FieldID.end())
			continue;

		switch(m_ColumnID2FieldID[col])
		{
		case MULTIACCOUNT_FIELD_ID_AVALIABLE:	//可用资金
			GETFUNDSTR(digit,strTemp,m_mapAccountInfo[acc].Available);
			m_pList->SetItem(row,col,strTemp);
			break;
		case MULTIACCOUNT_FIELD_ID_PREBALANCE:	//上次结算准备金
			GETFUNDSTR(digit,strTemp,m_mapAccountInfo[acc].PreBalance);
			m_pList->SetItem(row,col,strTemp);
			break;
		case MULTIACCOUNT_FIELD_ID_STATICPROFIT:	//静态权益
			GETFUNDSTR(digit,strTemp,m_mapAccountInfo[acc].StaticProfit);
			m_pList->SetItem(row,col,strTemp);
			break;
		case MULTIACCOUNT_FIELD_ID_DYNAMICPROFIT:	//动态权益
			GETFUNDSTR(digit,strTemp,m_mapAccountInfo[acc].DynamicProfit);
			m_pList->SetItem(row,col,strTemp);
			break;
		case MULTIACCOUNT_FIELD_ID_POSITIONPROFIT:	//持仓盈亏
			GETFUNDSTR(digit,strTemp,m_mapAccountInfo[acc].PositionProfit);
			m_pList->SetItem(row,col,strTemp);
			break;
		case MULTIACCOUNT_FIELD_ID_CLOSEPROFIT:		//平仓盈亏
			GETFUNDSTR(digit,strTemp,m_mapAccountInfo[acc].CloseProfit);
			m_pList->SetItem(row,col,strTemp);
			break;
		case MULTIACCOUNT_FIELD_ID_RISKDEGREE:		//风险度
			if(util::isInvalidValue(m_mapAccountInfo[acc].RiskDegree))
				strTemp.Printf("-");
			else
				strTemp.Printf("%.2f%%",m_mapAccountInfo[acc].RiskDegree);
			m_pList->SetItem(row,col,strTemp);
			break;
		default:
			break;
		}
	}
}
void MultiAccountPanel::SetUICallback(bool bAcc)
{
	if(m_MultiAccountPanel)
	{
		wxCommandEvent evt(wxEVT_MA_UI_CALLBACK);
		evt.SetInt(bAcc?1:0);
		m_MultiAccountPanel->ProcessEvent(evt);
	}
}
void MultiAccountPanel::OnUICallBack( wxCommandEvent& event)
{
	int index=event.GetInt();
	switch(index)
	{
	case 0:
		((wxChoice*)FindWindowById(ID_CHOICE))->SetSelection(m_pMACfg?m_pMACfg->nStrategyItem:-1);
		break;
	case 1:
		for (int row=0;row<m_pList->GetItemCount();row++)
		{
			int sel=m_pList->GetItemData(row);			
			m_pList->SetItemChecked(row,MA_CORE()->GetTradeAccount(sel)->StrategyValid);
			ResetValidColText(row, MA_CORE()->GetTradeAccount(sel)->StrategyValid);
		}
		m_pList->Refresh();
		break;
	}
}
void MultiAccountPanel::OnStrategyChoice(wxCommandEvent& event)
{
	if(m_pMACfg)
		m_pMACfg->nStrategyItem=(StrategyItem)event.GetInt();
	FindWindowById(ID_ALGO_SETUP,this)->Enable(m_mapCfgItem.find(event.GetInt())!=m_mapCfgItem.end());
}
void MultiAccountPanel::OnMaListUpdate(wxCommandEvent& event)
{
	m_pList->DeleteAllItems();
	map<string,PlatformStru_TradingAccountInfo>::iterator it;
	for (it=m_mapAccountInfo.begin();it!=m_mapAccountInfo.end();++it)
	{
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(it->first);
		if(pSvr)
		{
			if(pSvr->GetMACurrentAccount())
			{
				wxCommandEvent evt(wxEVT_MA_PLAT_MODIFY,GETTOPWINDOW()->GetId());
				evt.SetInt(NULL);
				GETTOPWINDOW()->ProcessEvent(evt);
			}
			wxCommandEvent evt(wxEVT_MA_PLAT_DELETE,GETTOPWINDOW()->GetId());
			evt.SetInt((int)pSvr);
			GETTOPWINDOW()->ProcessEvent(evt);
			pSvr->StopThread();
			pSvr->Stop();
			g_pPlatformMgr->ReleaseAccount(it->first);
		}
	}
	m_mapAccountInfo.clear();
	for (int sel=0;sel<MA_CORE()->GetTradeAccountCount();sel++)
	{
		InsertOneRow(sel,MA_CORE()->GetTradeAccount(sel));
	}
	m_pList->ReSortItems();
}
void MultiAccountPanel::OnListCheckbox(wxExtListEvent& event)
{
	int row=event.GetIndex();
	int nIndex = m_pList->GetItemData(row);
	bool bChecked=m_pList->GetItemChecked(row);
	if(row>=0&&row<MA_CORE()->GetTradeAccountCount())
		MA_CORE()->SetTradeAccountStrategyValid(nIndex,bChecked);
	{
	std::map<int, long>::iterator it = 
			m_FieldID2ColumnID.find(MULTIACCOUNT_FIELD_ID_VILID);

	if(it!=m_FieldID2ColumnID.end()) 
		if(event.GetColumn()==it->second) 
			ResetValidColText(row, bChecked);
	}
}

void MultiAccountPanel::OnListColClick( wxExtListEvent& event )
{
	m_pList->ReSortItems();
}

void MultiAccountPanel::OnMaUserChange(int sel)
{
	if ( m_pList->GetColumnCount() == 0 )
	{
		return;
	}

	set<string> ValidAccounts;
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(pMgr)
	{
		std::vector<AccountGroup> accountGroup;
		pMgr->GetAccountGroup(accountGroup); 
		if(sel>=0&&sel<(int)accountGroup.size())
		{
			AccountGroup& AccGrp=accountGroup[sel];
			AccountStru account;
			for (int i=0;i<AccGrp.GetAccountCount();++i)
			{
				if(AccGrp.GetAccount(i,account))
				{
					ValidAccounts.insert(account.szAccount);
				}
			}
		}
	}
	for (int row=0;row<m_pList->GetItemCount();row++)
	{
		int i=m_pList->GetItemData(row);	
		bool b=ValidAccounts.find(MA_CORE()->GetTradeAccountName(i))!=ValidAccounts.end();
		m_pList->SetItemChecked(row,b);
		ResetValidColText(row, b);
		MA_CORE()->SetTradeAccountStrategyValid(i,b);
	}
	m_pList->Refresh();
}
int MultiAccountPanel::SetAccountComboBox(wxChoice* combMAUser)
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(pMgr)
	{
		int curIndex=-1,curAcc=pMgr->GetCurAccountGroupID();
		std::vector<AccountGroup> accountGroup;
		pMgr->GetAccountGroup(accountGroup); 
		combMAUser->Clear();
		for (size_t i=0;i<accountGroup.size();++i)
		{
			if(accountGroup[i].GetGroupID()==curAcc)
				curIndex=(int)i;
			combMAUser->Append(accountGroup[i].GetGroupName());
		}
		combMAUser->SetSelection(curIndex);
		return curIndex;
	}
	return -1;
}
void MultiAccountPanel::OnUserChoice(wxCommandEvent& event)
{
	int sel=event.GetInt();
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(pMgr)
	{
		std::vector<AccountGroup> accountGroup;
		pMgr->GetAccountGroup(accountGroup); 
		if(sel>=0&&sel<(int)accountGroup.size())
			pMgr->SetCurAccountGroupID(accountGroup[sel].GetGroupID());
	}
	OnMaUserChange(sel);
}
void MultiAccountPanel::OnCfgChanged(wxCommandEvent& event)
{
	InitListCtrl();
	wxChoice* combMAUser=(wxChoice*)FindWindowById(ID_USERGROUP,this);
	int sel=SetAccountComboBox(combMAUser);
	OnMaUserChange(sel);
	for (int i=0;i<MA_CORE()->GetTradeAccountCount();++i)
	{
		IPlatformSingleSvr* piSvr=g_pPlatformMgr->GetPlatformSvr(MA_CORE()->GetTradeAccountName(i));
		if(piSvr&&piSvr->GetMACurrentAccount())
			SetQuotUse(GetRow(piSvr));
	}
	wxCommandEvent evt(wxEVT_MA_FUND_UPDATE);
	ProcessEvent(evt);
}

void MultiAccountPanel::OnPanelCharHook( wxCommandEvent& evt )
{
	wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();
	if(!pEvent) return;

	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;

	wxChoice* combMAUser=(wxChoice*)FindWindowById(ID_USERGROUP,this);

	int nKeyCode = pEvent->GetKeyCode();
	//获得键盘与字符串的映射表
	map<int,string> addr = pMgr->GetShortCutKeyNameMap();
	map<int,string>::iterator itKey = addr.find(nKeyCode);
	if ( itKey != addr.end() )
	{
		string strKeyName = itKey->second;
		vector<AccountGroup> vAccountGroup;
		pMgr->GetAccountGroup(vAccountGroup);
		for ( UINT i = 0; i < vAccountGroup.size(); i++)
		{
			if ( strKeyName == vAccountGroup[i].GetShortKey())
			{
				pMgr->SetCurAccountGroupID(vAccountGroup[i].GetGroupID());
				combMAUser->SetSelection(i);
				OnMaUserChange(i);
				break;
			}
		}
	}

	evt.Skip();
}

void MultiAccountPanel::InitListCtrl()
{
	m_pList->DeleteAllItems();
	m_pList->DeleteAllColumns();
	m_FieldID2ColumnID.clear();
	m_ColumnID2FieldID.clear();

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
	int nColCount = pCfgMgr->GetListVisibleColCount(GID_MULTI_ACCOUNT);
	for(i = 0;  i < nColCount; i++)
	{
		pColCfg = pCfgMgr->GetListVisibleColCfg(GID_MULTI_ACCOUNT, i);
		if(!pColCfg) continue;

		col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
		col.SetText(pCfgMgr->GetColCfgCaption(GID_MULTI_ACCOUNT,pColCfg->id));
		col.SetWidth(pColCfg->Width);
		col.SetTextColour(pColCfg->TextColor);
		col.SetColData( pColCfg->id );

		m_pList->InsertColumn(i, col);
		if ( pColCfg->id == MULTIACCOUNT_FIELD_ID_VILID)
		{
			m_pList->SetColumnCtrlType(i, COLUMN_CTRL_TYPE_CHECKBOX);
		}

		m_FieldID2ColumnID[pColCfg->id] = i;
		m_ColumnID2FieldID[i]=pColCfg->id;
	}

	for (int i=0;i<MA_CORE()->GetTradeAccountCount();++i)
	{
		InsertOneRow(i,MA_CORE()->GetTradeAccount(i));
	}


	m_pList->ReSortItems();
}

void MultiAccountPanel::OnContext( wxContextMenuEvent& evt )
{
	wxMenu menu;
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	if(menu.GetMenuItemCount()>0)
	{
		POINT pt;
		::GetCursorPos(&pt);
		BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

		switch(retcmd)
		{
		case ID_MENU_AUTO_ADJUST_COL_WIDTH:
			m_pList->AutoAdjustColumnWidth();
			SaveColWidth();
			break;
		case ID_MENU_EXPORT://导出列表
			{
				wxString cap=LOADSTRING(GID_MULTI_ACCOUNT_CAPTION);
				LIST_EXPORT_CSV_FILE(cap, m_pList);
			}
			break;
		case ID_MENU_COLUMNCONFIG://表格列设置
			{
				wxCommandEvent request_event(wxEVT_LOAD_CONFIG_APP);
				request_event.SetInt( ID_ITEM_MULTIACCOUNT );
				GETTOPWINDOW()->ProcessEvent(request_event);
			}
			break;
		default:
			break;
		}
	}
	evt.Skip();
}

void MultiAccountPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nColCount = m_pList->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_MULTI_ACCOUNT ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_MULTI_ACCOUNT, i);
		pColCfg->Width = m_pList->GetColumnWidth( i );
	}
}

void MultiAccountPanel::OnReQuery( wxCommandEvent& evt )
{
	for ( int i = 0; i < (int)MA_CORE()->GetTradeAccountCount(); i++ )
	{
		const CTPTradeAccount* pAcc=MA_CORE()->GetTradeAccount(i);
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(pAcc->t.szTradeAccount);
		if(pSvr)
		{
			PlatformStru_QryTradingAccount QryTradingAccount;
			memset(&QryTradingAccount, 0, sizeof(QryTradingAccount));
			pSvr->ReqQryTradingAccount(QryTradingAccount);
		}
	}
}

void MultiAccountPanel::OnSubscrible( wxCommandEvent& evt )
{
	if ( evt.GetInt())
	{
	}
	else
	{
		switch(MA_CORE()->GetSaveTypeAuto())
		{
		case 1:
			MA_CORE()->SaveDataToLocal(true);
			break;
		case 2:
			MA_CORE()->SaveDataToSvr(true,NULL);
			break;
		case 3:
			MA_CORE()->SaveDataToLocal(false);
			break;
		case 4:
			MA_CORE()->SaveDataToSvr(false,NULL);
			break;
		}
	}
}
