#include "StdAfx.h"
#include "Module-Misc2/GlobalFunc.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "ProxySvrSetupDlg.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
#define SET_GRID_STYLE(pGrid)\
do{	\
	pGrid->SetColLabelSize(23);\
	pGrid->EnableDragGridSize(false);\
	pGrid->SetRowLabelSize( 0 );\
	pGrid->EnableDragRowSize(false);\
	pGrid->EnableEditing(true);\
	pGrid->EnableGridLines(true);\
	pGrid->SetScrollLineY(pGrid->GetDefaultRowSize());\
	pGrid->SetCellHighlightPenWidth(0);\
	pGrid->SetCellHighlightROPenWidth(0);\
}while(0)
#define GRID_INSERT_ROWS(pGrid,strvec)\
do{	\
    if(pGrid->GetRows()>0)	\
		pGrid->DeleteRows(0,pGrid->GetRows());\
	pGrid->InsertRows(0,strvec.size());\
	for (int i=0;i<(int)strvec.size();++i)\
	{\
		std::string str=strvec[i];\
		int pos=str.rfind(':');\
		if(pos!=string::npos)\
		{\
			pGrid->SetCellValue(i,0,str.substr(0,pos));\
			pGrid->SetCellValue(i,1,str.substr(pos+1));\
		}\
	}\
}while(0)
CProxySvrSetupDlg::CProxySvrSetupDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,LOADSTRING(PROXYSVRSETUP_TITLE),wxDefaultPosition, wxSize(570,450)),
m_SelRow(0)
{
	{
		SERVER_PARAM_CFG& thisCfg=DEFAULT_SVR()->GetServerParamCfg();
		if(thisCfg.VersionStr.find("simu")==string::npos) 
			m_CTPSimulate=1;
		else
			m_CTPSimulate=0;
		new wxStaticBox( this, wxID_STATIC, LOADSTRING(SOCKS_USESOCKS), wxPoint(5,5), wxSize(550,72), 0 );
		new wxStaticText( this, wxID_STATIC, LOADSTRING(SOCKS_TYPE), wxPoint(24,25), wxSize(87,20), wxALIGN_CENTRE );
		wxArrayString items;
		items.Add(LOADSTRING(SOCKS_NOSOCKS));
		items.Add("socks4");
		items.Add("socks5");
		items.Add("http");
		wxChoice* pChoice=new wxChoice( this, ID_CHOICE,wxPoint(24,45), wxSize(87,20), items, 0 );
		if(thisCfg.ProxyType=="socks4")
			pChoice->SetSelection(1);
		else if(thisCfg.ProxyType=="socks5")
			pChoice->SetSelection(2);
		else if(thisCfg.ProxyType=="http")
			pChoice->SetSelection(3);
		else
			pChoice->SetSelection(0);
		new wxStaticText( this, wxID_STATIC, LOADSTRING(SOCKS_ADDR), wxPoint(127,25), wxSize(145,20), wxALIGN_CENTRE );
		peditAddr=new wxTextCtrl( this, ID_EDIT_ADDR, wxEmptyString, wxPoint(127,45), wxSize(145,20), 0 );
		peditAddr->SetValue(thisCfg.ProxyAddr.c_str());
		peditAddr->Enable(pChoice->GetSelection()>0);
		new wxStaticText( this, wxID_STATIC, LOADSTRING(SOCKS_PORT), wxPoint(289,25), wxSize(45,20), wxALIGN_CENTRE );
		peditPort=new wxTextCtrl( this, ID_EDIT_PORT, wxEmptyString, wxPoint(289,45), wxSize(45,20), 0 );
		peditPort->SetValue(thisCfg.ProxyPort.c_str());
		peditPort->Enable(pChoice->GetSelection()>0);
		new wxStaticText( this, wxID_STATIC, LOADSTRING(SOCKS_USER), wxPoint(350,25), wxSize(85,20), wxALIGN_CENTRE);
		peditUser=new wxTextCtrl( this, ID_EDIT_USER, wxEmptyString, wxPoint(350,45), wxSize(85,20), 0 );
		peditUser->SetValue(thisCfg.ProxyUser.c_str());
		peditUser->Enable(pChoice->GetSelection()>0);	
		new wxStaticText( this, wxID_STATIC, LOADSTRING(SOCKS_PWD), wxPoint(450,25), wxSize(83,20), wxALIGN_CENTRE );
		peditPwd=new wxTextCtrl( this, ID_EDIT_PWD, wxEmptyString, wxPoint(450,45), wxSize(83,20), wxTE_PASSWORD );
		peditPwd->SetValue(thisCfg.ProxyPwd.c_str());
		peditPwd->Enable(pChoice->GetSelection()>0);
	}
	if(!GlobalConfigManager::m_bLocalVersion_MultiAccount)
	{
		//多账号单机版不让改服务器设置
		m_vec=DEFAULT_SVR()->GetServerParamCfg().vec;
		new wxStaticBox( this, wxID_STATIC, LOADSTRING(SOCKS_SETUP), wxPoint(5,90), wxSize(550,285), 0 );
		wxGrid* pGrid1=new wxGrid( this, ID_SVRLIST,wxPoint(14,110), wxSize(110,230),wxWANTS_CHARS|wxBORDER );
		pGrid1->CreateGrid(m_vec.size(),2,wxGrid::wxGridSelectRows);
		SET_GRID_STYLE(pGrid1);
		pGrid1->SetColLabelValue(0,LOADSTRING(SOCKS_SVRLIST));
		pGrid1->SetColLabelValue(1,LOADSTRING(SOCKS_SVRCODE));
		pGrid1->SetColSize(0,50); 
		pGrid1->SetColSize(1,50); 
		for (int i=0;i<(int)m_vec.size();++i)
		{
			pGrid1->SetCellValue(i,0,LOADSTRING_TEXT(m_vec[i].ServerName));
			pGrid1->SetCellValue(i,1,LOADSTRING_TEXT(m_vec[i].BrokerID));
		}
		wxGrid* pGrid2=new wxGrid( this, ID_TRADE_LIST,wxPoint(130,110), wxSize(370,120),wxWANTS_CHARS|wxBORDER);
		pGrid2->CreateGrid(0,2,wxGrid::wxGridSelectRows);
		SET_GRID_STYLE(pGrid2);
		pGrid2->SetColLabelValue(0,LOADSTRING(SOCKS_ADDR_TRADE));
		pGrid2->SetColSize(0,250); 
		pGrid2->SetColLabelValue(1,LOADSTRING(SOCKS_PORT));
		GRID_INSERT_ROWS(pGrid2,m_vec[m_SelRow].TradeAddr);
		wxGrid* pGrid3=new wxGrid( this, ID_QUOT_LIST,wxPoint(130,244), wxSize(370,120),wxWANTS_CHARS|wxBORDER);
		pGrid3->CreateGrid(0,2,wxGrid::wxGridSelectRows);
		SET_GRID_STYLE(pGrid3);	
		pGrid3->SetColLabelValue(0,LOADSTRING(SOCKS_ADDR_QUOT));
		pGrid3->SetColSize(0,250); 
		pGrid3->SetColLabelValue(1,LOADSTRING(SOCKS_PORT));
		GRID_INSERT_ROWS(pGrid3,m_vec[m_SelRow].QuotAddr);
		new wxButton( this, ID_ADD_SVR, LOADSTRING(SOCKS_ADD),wxPoint(14,345), wxSize(45,20), 0 );
		new wxButton( this, ID_DEL_SVR, LOADSTRING(SOCKS_DEL),wxPoint(80,345), wxSize(45,20), 0 );
		new wxButton( this, ID_ADD_TRADE, LOADSTRING(SOCKS_ADD),wxPoint(504,110), wxSize(45,20), 0 );
		new wxButton( this, ID_DEL_TRADE, LOADSTRING(SOCKS_DEL),wxPoint(504,137), wxSize(45,20), 0 );
		new wxButton( this, ID_ADD_QUOT, LOADSTRING(SOCKS_ADD),wxPoint(504,244), wxSize(45,20), 0 );
		new wxButton( this, ID_DEL_QUOT, LOADSTRING(SOCKS_DEL),wxPoint(504,271), wxSize(45,20), 0 );
	}

	int okbtnpoy=385;
	if(GlobalConfigManager::m_bLocalVersion_MultiAccount)
	{
		this->SetSize(-1,-1,570,200);
		okbtnpoy=120;
	}

	new wxButton( this, wxID_OK, LOADSTRING(SOCKS_OK),wxPoint(385,okbtnpoy), wxSize(70,20), 0 );
	new wxButton( this, wxID_CANCEL, LOADSTRING(SOCKS_CANCEL),wxPoint(480,okbtnpoy), wxSize(70,20), 0 );
}

CProxySvrSetupDlg::~CProxySvrSetupDlg(void)
{

}

BEGIN_EVENT_TABLE(CProxySvrSetupDlg,wxDialog)
    EVT_CHOICE(ID_CHOICE,OnTypeChoice)
	EVT_GRID_CMD_CELL_LEFT_DCLICK(ID_SVRLIST, OnCellLeftDoubleClick)
	EVT_BUTTON(ID_ADD_SVR,OnAddSvr)
	EVT_BUTTON(ID_DEL_SVR,OnDelSvr)
	EVT_BUTTON(ID_ADD_TRADE,OnAddTrade)
	EVT_BUTTON(ID_DEL_TRADE,OnDelTrade)
	EVT_BUTTON(ID_ADD_QUOT,OnAddQuot)
	EVT_BUTTON(ID_DEL_QUOT,OnDelQuot)
	EVT_BUTTON(wxID_OK,OnOK)
	EVT_GRID_CMD_CELL_CHANGE(wxID_ANY,OnCellChanged)
END_EVENT_TABLE()
void CProxySvrSetupDlg::OnTypeChoice(wxCommandEvent& event)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(event.GetId(),this);
	int currentselection=pChoice->GetCurrentSelection();	
	peditAddr->Enable(currentselection>0);
	peditPort->Enable(currentselection>0);
	peditUser->Enable(currentselection>0);
	peditPwd->Enable(currentselection>0);
}
void CProxySvrSetupDlg::OnCellLeftDoubleClick(wxGridEvent& evt)
{
	m_SelRow= evt.GetRow();
	ChangeSvr();
}
void CProxySvrSetupDlg::ChangeSvr()
{
	wxGrid* pGrid2=(wxGrid*)FindWindowById(ID_TRADE_LIST,this);
	wxGrid* pGrid3=(wxGrid*)FindWindowById(ID_QUOT_LIST,this);
	if(m_SelRow>=0&&m_SelRow<(int)m_vec.size())
	{
		GRID_INSERT_ROWS(pGrid2,m_vec[m_SelRow].TradeAddr);
		GRID_INSERT_ROWS(pGrid3,m_vec[m_SelRow].QuotAddr);
	}
	else
	{
		m_SelRow=-1;
		std::vector<std::string> vec;
		GRID_INSERT_ROWS(pGrid2,vec);
		GRID_INSERT_ROWS(pGrid3,vec);
	}
}
void CProxySvrSetupDlg::OnAddSvr(wxCommandEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(ID_SVRLIST,this);
    pGrid->AppendRows();
	SERVER_PARAM_CFG_ITEM item;
	item.CTPSimulate=m_CTPSimulate;
	m_vec.push_back(item);
}
void CProxySvrSetupDlg::OnDelSvr(wxCommandEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(ID_SVRLIST,this);
	wxGridCellCoordsArray tls=pGrid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs=pGrid->GetSelectionBlockBottomRight();
	vector<int> rows;
	int i,j;
	for(i=0;i<(int)tls.size();i++)
	{
		for(int row=tls[i].GetRow();row<=brs[i].GetRow();++row)
		{
			rows.push_back(row);
		}
	}
	std::vector<SERVER_PARAM_CFG_ITEM>::iterator it;
	bool delcur=false;
	for (i=rows.size()-1;i>=0;--i)
	{
		if(rows[i]==m_SelRow)
			delcur=true;
		pGrid->DeleteRows(rows[i]);
		for (j=0,it=m_vec.begin();j<rows[i];j++,++it){}
		m_vec.erase(it);
	}
	m_SelRow=0;
	ChangeSvr();

}
void CProxySvrSetupDlg::OnAddTrade(wxCommandEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(ID_TRADE_LIST,this);
	if(m_SelRow>0)
	{
		pGrid->AppendRows();
		m_vec[m_SelRow].TradeAddr.push_back("");
	}
}
void CProxySvrSetupDlg::OnDelTrade(wxCommandEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(ID_TRADE_LIST,this);
	wxGridCellCoordsArray tls=pGrid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs=pGrid->GetSelectionBlockBottomRight();
	vector<int> rows;
	int i,j;
	for(i=0;i<(int)tls.size();i++)
	{
		for(int row=tls[i].GetRow();row<=brs[i].GetRow();++row)
		{
			rows.push_back(row);
		}
	}
	std::vector<std::string>::iterator it;
	for (i=rows.size()-1;i>=0;--i)
	{
		pGrid->DeleteRows(rows[i]);
		for (j=0,it=m_vec[m_SelRow].TradeAddr.begin();j<rows[i];j++,++it){}
		m_vec[m_SelRow].TradeAddr.erase(it);
	}
}
void CProxySvrSetupDlg::OnAddQuot(wxCommandEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(ID_QUOT_LIST,this);
	if(m_SelRow>0)
	{
		pGrid->AppendRows();
		m_vec[m_SelRow].QuotAddr.push_back("");
	}
}
void CProxySvrSetupDlg::OnDelQuot(wxCommandEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(ID_QUOT_LIST,this);
	wxGridCellCoordsArray tls=pGrid->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs=pGrid->GetSelectionBlockBottomRight();
	vector<int> rows;
	int i,j;
	for(i=0;i<(int)tls.size();i++)
	{
		for(int row=tls[i].GetRow();row<=brs[i].GetRow();++row)
		{
			rows.push_back(row);
		}
	}
	std::vector<std::string>::iterator it;
	for (i=rows.size()-1;i>=0;--i)
	{
		pGrid->DeleteRows(rows[i]);
		for (j=0,it=m_vec[m_SelRow].QuotAddr.begin();j<rows[i];j++,++it){}
		m_vec[m_SelRow].QuotAddr.erase(it);
	}
}
void CProxySvrSetupDlg::OnCellChanged(wxGridEvent& evt)
{
	wxGrid* pGrid=(wxGrid*)FindWindowById(evt.GetId(),this);
	int row=evt.GetRow();
	int col=evt.GetCol();
	switch(evt.GetId())
	{
	case ID_SVRLIST:
		if(col==0)
			m_vec[row].ServerName=pGrid->GetCellValue(row,col);
		else if(col==1)
			m_vec[row].BrokerID=pGrid->GetCellValue(row,col);		
		break;
	case ID_TRADE_LIST:
		m_vec[m_SelRow].TradeAddr[row]=pGrid->GetCellValue(row,0);
		m_vec[m_SelRow].TradeAddr[row]+=":";
		m_vec[m_SelRow].TradeAddr[row]+=pGrid->GetCellValue(row,1);
		break;
	case ID_QUOT_LIST:
		m_vec[m_SelRow].QuotAddr[row]=pGrid->GetCellValue(row,0);
		m_vec[m_SelRow].QuotAddr[row]+=":";
		m_vec[m_SelRow].QuotAddr[row]+=pGrid->GetCellValue(row,1);
		break;
	}
}
void CProxySvrSetupDlg::OnOK(wxCommandEvent& evt)
{
	SERVER_PARAM_CFG& thisCfg=DEFAULT_SVR()->GetServerParamCfg();
	wxChoice* pChoice=(wxChoice*)FindWindowById(ID_CHOICE,this);
	thisCfg.ProxyType=pChoice->GetSelection()>0?pChoice->GetString(pChoice->GetSelection()).c_str():"";
	thisCfg.ProxyAddr=((wxTextCtrl*)FindWindowById(ID_EDIT_ADDR,this))->GetValue().c_str();
	thisCfg.ProxyPort=((wxTextCtrl*)FindWindowById(ID_EDIT_PORT,this))->GetValue().c_str();
	thisCfg.ProxyUser=((wxTextCtrl*)FindWindowById(ID_EDIT_USER,this))->GetValue().c_str();
	thisCfg.ProxyPwd=((wxTextCtrl*)FindWindowById(ID_EDIT_PWD,this))->GetValue().c_str();
	thisCfg.vec=m_vec;
	evt.Skip();
}