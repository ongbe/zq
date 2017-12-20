/***************************************************************************//**
*   @file         QuotList.cpp
*   @brief        行情板块,类的实现
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/20
********************************************************************************/

#include "stdafx.h"
#include "QuotPanel.h"
#include <vector>

#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/ZqMessageBox.h"
#include "../ConfigApp/Const.h"
#include "wx/file.h"

#include "../inc/gui/EventParam.h"
#include "../inc/gui/MainFrame.h"
#include "../inc/Plusin/Plusin.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
extern CPlusin g_Plusin;
#pragma warning(disable:4018)
#define BUTTON_WIDTH    80
#define BUTTON_HEIGHT	25
#define BUTTON_MARGIN   (BUTTON_HEIGHT+5)
#define LIST_MARGIN		(BUTTON_MARGIN+5)


static CQuotPanel* s_pCQuotPanel=NULL;
static int quotCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
//	WXLOG_INFO("quotCallBackFunc");
	if(data.BID!=BID_RtnDepthMarketData || data.Size!=sizeof(DataRtnDepthMarketData))
		return 0;

	if(!s_pCQuotPanel)
		return 0;

	DataRtnDepthMarketData* pTmp=new DataRtnDepthMarketData();
	memcpy(pTmp, &data, sizeof(DataRtnDepthMarketData));

	//post event to CQuotPanel
	wxCommandEvent evt(wxEVT_QUOT_PANEL_NEW_QUOT, wxID_ANY);
	evt.SetClientData(pTmp);	
	s_pCQuotPanel->AddPendingEvent(evt);

	return 0;
}


BEGIN_EVENT_TABLE(CQuotPanel, wxPanel)
    EVT_COMMAND(wxID_ANY, wxEVT_QUOT_PANEL_NEW_QUOT,  CQuotPanel::OnRcvQuot)
	EVT_SIZE( CQuotPanel::OnSize)
	EVT_CFG_CHANGE(CQuotPanel::OnCfgChanged)
	EVT_PANEL_FOCUS(CQuotPanel::OnPanelFocus)
    EVT_BUTTON(wxID_ANY, CQuotPanel::OnButtonClick)
	EVT_SUBSCRIBE(CQuotPanel::OnSubscrible)
    EVT_GRID_CELL_LEFT_CLICK( CQuotPanel::OnCellLeftClick )
	EVT_GRID_RANGE_SELECT( CQuotPanel::OnRangeSelected )
	EVT_GRID_CELL_LEFT_DCLICK(CQuotPanel::OnCellLeftDoubleClick)
	EVT_GRID_LABEL_RIGHT_CLICK(CQuotPanel::OnRightClick)
	EVT_GRID_CELL_RIGHT_CLICK(CQuotPanel::OnRightClick)
//	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_GET_INSTRUMENTID,  CQuotPanel::OnGetInstrumentID)
	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_SET_GROUP,CQuotPanel::OnSetGroup)
//	EVT_COMMAND(wxID_ANY, wxEVT_QUOT_GET_GROUP,CQuotPanel::OnGetGroup)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_INSTRUMENTID_CHANGED,CQuotPanel::OnSetInstrumentID) 
	//右键菜单
    EVT_MENU_RANGE(ID_MENU_COLUMNCONFIG,ID_MENU_SELECTINSTRUMENT,CQuotPanel::ContextMenuRespond)
    EVT_MENU_RANGE(ID_MENU_ADDTOMYSELF,ID_MENU_ADDTOMYSELF+50,CQuotPanel::ContextMenuRespond)
    EVT_MENU_RANGE(ID_CONTEXT_PLUGINS_BEGIN,ID_CONTEXT_PLUGINS_END,CQuotPanel::ContextMenuRespond)
	EVT_MENU(ID_MENU_AUTO_ADJUST_COL_WIDTH,CQuotPanel::ContextMenuRespond)
	EVT_GRID_COL_SIZE(OnColumnResize)
	EVT_KEY_DOWN(OnKeyDown)
	EVT_COMMAND(wxID_ANY, wxEVT_MAINFRAME_QUOT_SELGROUP,CQuotPanel::OnButtonClick)
#ifdef _USE_MULTI_LANGUAGE
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
#endif
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_MODIFY,CQuotPanel::OnMAPlatformModify)
END_EVENT_TABLE()

CQuotPanel::CQuotPanel(wxWindow *parent,
               wxWindowID id,
               const wxPoint& pos,
               const wxSize& size,
               long style,
			   const wxString& name)
			   :wxPanel(parent, id, pos, size, style, name),
			   m_nCurrentIndex(-1),
			   m_grid(NULL),
               m_GroupButtonInToolBar(true),
				//m_pWriteLog(NULL),
				m_pContextMenu(NULL),
				m_pSvr(NULL)
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	char strFileName[1024];
	ZeroMemory(strFileName, 1024);

	sprintf(strFileName, "UserOperate%04d%02d%02d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	//m_pWriteLog = new zqWriteLog(LOGMODE_LOCALFILE, strFileName);

	WXLOG_INFO("CQuotPanel::CQuotPanel");
    SetBackgroundColour(DEFAULT_COLOUR); 
	m_grid = new MainGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	InitGridUI();
	InitGridData();
	s_pCQuotPanel=this;
	m_pSvr=m_pSvr;
	if(m_pSvr) m_pSvr->SubscribeBusinessData(BID_RtnDepthMarketData, GID_QUOT_TABLE, quotCallBackFunc);
	LoadOldQuotData();

}
void CQuotPanel::SelectCurrentRow()
{
	if(m_grid)
		m_grid->SelectCurrentRow();
}
void CQuotPanel::InitGridUI()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_QUOT_TABLE);
	wxASSERT(pListCfg);
	m_grid->SetLabelBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
	m_grid->SetLabelTextColour(wxColor(pListCfg->HeadColor));
	m_grid->SetDefaultCellBackgroundColour(wxColor(pListCfg->BackgroundColor));
	m_grid->SetCellTextColour(pListCfg->TextColor);
	wxFont cellfont;
	cellfont.SetNativeFontInfoUserDesc(pListCfg->szFont); 
	m_grid->SetDefaultCellFont(cellfont);
	m_grid->SetLabelFont(cellfont);
	int lh=GetLineHeight(cellfont);
	m_grid->SetLabelSize(wxHORIZONTAL,lh);
	m_grid->SetDefaultRowSize(lh,true);

	int groupCnt = pCfgMgr->GetInstrumentGroupCount();
	if(groupCnt<=0)return;
	//根据合约组的索引获取合约组的简要信息
	PlatformStru_DepthMarketData rawQuot;
	memset(&rawQuot, 0, sizeof(rawQuot));
	std::string	instrID;
	vector<string> allInstrumentIDs;
    wxToolBar* tbar=NULL;
    if(m_GroupButtonInToolBar)
        tbar=((MainFrame*)GETTOPWINDOW())->CreateToolbar_Begin();
	for(int i=0;i<groupCnt;++i)
	{
		LPINSTRUMENT_GROUP_CFG  p=pCfgMgr->GetInstrumentGroupInfo(i);
		if(p->IsVisibility == 1)
		{
			OneGroupInfo tmp;
            if(m_GroupButtonInToolBar)
			    tmp.pWxButton=((MainFrame*)GETTOPWINDOW())->CreateToolbar_Item(tbar, wxID_ANY,p->GroupName, wxDefaultPosition, wxSize(72,-1), 0);			
            else
                tmp.pWxButton=new wxButton(this, wxID_ANY, p->GroupName, wxDefaultPosition, wxSize(BUTTON_WIDTH, BUTTON_HEIGHT));
			tmp.table= new QuotGridTable(GID_QUOT_TABLE);
			tmp.pTextData=new map<int,vector<string>>();
			tmp.GroupId=p->GroupId;
			tmp.name=p->GroupName;
			m_AllGroups.push_back(tmp);
			allInstrumentIDs.clear();
				//根据合约组的索引获取 合约总数量
				int instrumentIDcnt=pCfgMgr->GetInstrumentGroupMemberCount(i);
				if(instrumentIDcnt<0) instrumentIDcnt=0;
				if(!tmp.pTextData->empty())
				{
					map<int,vector<string>>::iterator iterA;
					for(iterA=tmp.pTextData->begin();iterA!=tmp.pTextData->end();++iterA)
					{
						iterA->second.clear();
					}
					tmp.pTextData->clear();
				}
				//根据合约组的索引，合约的索引获取 合约名	
				allInstrumentIDs.reserve(instrumentIDcnt);
				for(int n=0; n<instrumentIDcnt; ++n)
				{
					instrID=pCfgMgr->GetInstrumentId(i, n);
					allInstrumentIDs.push_back(instrID);
					vector<string> vec(1,instrID);
					map<int,vector<string>>& strvec=*tmp.pTextData;
					strvec[n]=vec;
				}
				tmp.table->SetNumberRows(allInstrumentIDs.size());
				tmp.table->SetTextData(tmp.pTextData);
				//加载将当前行情
				for(vector<string>::iterator it=allInstrumentIDs.begin(); it!=allInstrumentIDs.end(); ++it)
				{
					std::string strInstrumentID=*it;
					memset(&rawQuot,0,sizeof(rawQuot));
					if(!(m_pSvr&&m_pSvr->GetQuotInfo(strInstrumentID, rawQuot)>=0))
					{
                        InvalidRawQuot(rawQuot,strInstrumentID);
					}
                    tmp.table->LoadNewQuotData(rawQuot,false);
				}
			//}

		}
	}
    if(m_GroupButtonInToolBar)
        ((MainFrame*)GETTOPWINDOW())->CreateToolbar_End(tbar);
	wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED);
	ProcessEvent(cmdevt);
}
void CQuotPanel::InitGridData()
{
	static bool bFirstInitGridData=true;
	if(bFirstInitGridData)
	{
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
		wxASSERT(pCfgMgr);
		LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_QUOT_TABLE);
		wxASSERT(pListCfg);
		int groupCnt = pCfgMgr->GetInstrumentGroupCount();
		if(groupCnt<=0)return;

		vector<string> allInstrumentIDs;

		std::set<std::string> ExchangeInstrumentList;
		ExchangeInstrumentList.clear();
		if(m_pSvr) m_pSvr->GetMainInstrumentList(ExchangeInstrumentList);

		PlatformStru_DepthMarketData rawQuot;
		memset(&rawQuot, 0, sizeof(rawQuot));
		//根据合约组的索引获取合约组的简要信息
		for(int i=0,i_index=0;i<groupCnt;++i)
		{
			LPINSTRUMENT_GROUP_CFG  p=pCfgMgr->GetInstrumentGroupInfo(i);
			if(p->IsVisibility == 1)
			{
				OneGroupInfo& tmp=m_AllGroups[i_index++];
				allInstrumentIDs.clear();
				allInstrumentIDs=*(pCfgMgr->GetGroupMember(i));
				//加载将当前行情
				for(vector<string>::iterator it=allInstrumentIDs.begin(); it!=allInstrumentIDs.end(); ++it)
				{
					std::string strInstrumentID=*it;
					memset(&rawQuot,0,sizeof(rawQuot));
					if(!(m_pSvr&&m_pSvr->GetQuotInfo(strInstrumentID, rawQuot)>=0))
					{
                        InvalidRawQuot(rawQuot,strInstrumentID);
					}
                    tmp.table->LoadNewQuotData(rawQuot,false);
				}
			}
		}
		wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED);
		ProcessEvent(cmdevt);
	}
	bFirstInitGridData=false;
}
void CQuotPanel::LoadUI(bool bFirst,bool bOnlyUI)
{
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_QUOT_TABLE);
	wxASSERT(pListCfg);
	m_grid->SetLabelBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
	m_grid->SetLabelTextColour(wxColor(pListCfg->HeadColor));
	m_grid->SetDefaultCellBackgroundColour(wxColor(pListCfg->BackgroundColor));
	m_grid->SetCellTextColour(pListCfg->TextColor);
	wxFont cellfont;
	cellfont.SetNativeFontInfoUserDesc(pListCfg->szFont); 
	m_grid->SetDefaultCellFont(cellfont);
	m_grid->SetLabelFont(cellfont);
	int lh=GetLineHeight(cellfont);
	m_grid->SetLabelSize(wxHORIZONTAL,lh);
	m_grid->SetDefaultRowSize(lh,true);
    if(bOnlyUI)return;
    //获取合约组的个数
	int groupCnt = pCfgMgr->GetInstrumentGroupCount();
	if(groupCnt<=0)
	{
		Release();
		return;
	}

	std::string	instrID;
	vector<string> allInstrumentIDs;

	std::set<std::string> ExchangeInstrumentList;
	ExchangeInstrumentList.clear();
	if(m_pSvr) m_pSvr->GetMainInstrumentList(ExchangeInstrumentList);

	PlatformStru_DepthMarketData rawQuot;
	memset(&rawQuot, 0, sizeof(rawQuot));

	std::vector<OneGroupInfo> LastAllGroups;
    if(!bFirst)//备份合约组名称和合约组信息
	{
		LastAllGroups=m_AllGroups;
		m_AllGroups.clear();
	}
	
	//根据合约组的索引获取合约组的简要信息
    wxToolBar* tbar=NULL;
    if(m_GroupButtonInToolBar)
        tbar=((MainFrame*)GETTOPWINDOW())->CreateToolbar_Begin();
	for(int i=0;i<groupCnt;++i)
	{
		LPINSTRUMENT_GROUP_CFG  p=pCfgMgr->GetInstrumentGroupInfo(i);
		if(p->IsVisibility == 1)
		{
			int oldindex=GetIndexFromGroupId(p->GroupId,&LastAllGroups);
			OneGroupInfo tmp;
			if(oldindex>=0)
			{
                if(m_GroupButtonInToolBar)
				    tmp.pWxButton=((MainFrame*)GETTOPWINDOW())->CreateToolbar_Item(tbar, wxID_ANY,p->GroupName, wxDefaultPosition, wxSize(72,-1), 0);
                else
                    tmp.pWxButton=LastAllGroups[oldindex].pWxButton;
                tmp.table=LastAllGroups[oldindex].table;
				tmp.pTextData=LastAllGroups[oldindex].pTextData;
				LastAllGroups[oldindex].pTextData=NULL;
			}
			else
			{
                if(m_GroupButtonInToolBar)
				    tmp.pWxButton=((MainFrame*)GETTOPWINDOW())->CreateToolbar_Item(tbar, wxID_ANY,p->GroupName, wxDefaultPosition, wxSize(72,-1), 0);			
                else
                    tmp.pWxButton=new wxButton(this, wxID_ANY,p->GroupName, wxDefaultPosition, wxSize(BUTTON_WIDTH, BUTTON_HEIGHT));
                tmp.table= new QuotGridTable(GID_QUOT_TABLE);
				tmp.pTextData=new map<int,vector<string>>();
			}
			tmp.GroupId=p->GroupId;
			tmp.name=p->GroupName;
			m_AllGroups.push_back(tmp);

			allInstrumentIDs.clear();
			{
				//根据合约组的索引获取 合约总数量
				int instrumentIDcnt=pCfgMgr->GetInstrumentGroupMemberCount(i);
				if(instrumentIDcnt<0) instrumentIDcnt=0;
				if(!tmp.pTextData->empty())
				{
					map<int,vector<string>>::iterator iterA;
					for(iterA=tmp.pTextData->begin();iterA!=tmp.pTextData->end();++iterA)
					{
						iterA->second.clear();
					}
					tmp.pTextData->clear();
				}
				//根据合约组的索引，合约的索引获取 合约名	
				allInstrumentIDs.reserve(instrumentIDcnt);
				for(int n=0; n<instrumentIDcnt; ++n)
				{
					instrID=pCfgMgr->GetInstrumentId(i, n);
					allInstrumentIDs.push_back(instrID);
					vector<string> vec(1,instrID);
					map<int,vector<string>>& strvec=*tmp.pTextData;
					strvec[n]=vec;
				}

			}
			tmp.table->SetNumberRows(allInstrumentIDs.size());
			tmp.table->SetTextData(tmp.pTextData);
			//加载将当前行情
			for(vector<string>::iterator it=allInstrumentIDs.begin(); it!=allInstrumentIDs.end(); ++it)
			{
				std::string strInstrumentID=*it;
				memset(&rawQuot,0,sizeof(rawQuot));
				if(!(m_pSvr&&m_pSvr->GetQuotInfo(strInstrumentID, rawQuot)>=0))
				{
                    InvalidRawQuot(rawQuot,strInstrumentID);
				}
                tmp.table->LoadNewQuotData(rawQuot,false);
			}
		}


	}
    if(m_GroupButtonInToolBar)
        ((MainFrame*)GETTOPWINDOW())->CreateToolbar_End(tbar);
	if(!bFirst)//删除合约组名称和合约组信息备份
	{
		int lastallgroupcount=(int)LastAllGroups.size();
		if(lastallgroupcount>0)
		{
			int oldIndex=m_nCurrentIndex;
			map<int,vector<string>>::iterator iter;
			for(int gg=0;gg<(int)lastallgroupcount;gg++)
			{	
				if(gg==oldIndex&&gg!=GetIndexFromGroupId(LastAllGroups[gg].GroupId))
				{
					wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED,LastAllGroups[gg].pWxButton->GetId());
                    if(m_GroupButtonInToolBar)
                        ((MainFrame*)GETTOPWINDOW())->ProcessEvent(cmdevt);
                    else
                        ProcessEvent(cmdevt);
				}
				if(LastAllGroups[gg].pTextData)
				{
					OneGroupInfo& info=LastAllGroups[gg];
                    if(!m_GroupButtonInToolBar)
                    {
                        wxDELETE(info.pWxButton);
                    }
					for(iter=info.pTextData->begin();iter!=info.pTextData->end();++iter)
					{
						iter->second.clear();
					}
					info.pTextData->clear();
					wxDELETE(info.pTextData);
					if(m_grid->GetTable()==info.table)
					{
						wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED);
						ProcessEvent(cmdevt);
						m_grid->SetTable(NULL,false,wxGrid::wxGridSelectRows);
						UpdateCurrentInstrumentIDs(NULL);
						SelectCurrentRow();
					}
					wxDELETE(info.table);
				}
			}
			LastAllGroups.clear();
		}
		else
		{
			wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED);
			ProcessEvent(cmdevt);
		}
	}
	else
	{
		wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED);
		ProcessEvent(cmdevt);
	}
	int defaultGroupId=pCfgMgr->GetCurrentGruopId();
	if(defaultGroupId>0)
	{
		int defaultBtnId=GetButtonIdFromGroupId(defaultGroupId);
		wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED,defaultBtnId);
        if(m_GroupButtonInToolBar)
		    ((MainFrame*)GETTOPWINDOW())->ProcessEvent(cmdevt);	
        else
            ProcessEvent(cmdevt);	

	}
}
int CQuotPanel::UpdateCurrentInstrumentIDs(QuotGridTable *table)
{
    std::set<std::string> InstrumentIDs;
	BOOL bIsComboCode = FALSE;
	string strCmbSubCode1, strCmbSubCode2;
	InstrumentIDs.clear();
	if(m_pSvr) {
		if(m_pSvr->GetInstrumentList(InstrumentIDs)>0)
		{
			for (std::set<std::string>::iterator it = InstrumentIDs.begin(); it != InstrumentIDs.end(); ++it) 
			{
#ifdef _USE_COMBOINSTRUMENT	
				bIsComboCode = m_pSvr->GetProductClassType(*it) == THOST_FTDC_PC_Combination;
				if(bIsComboCode) {
					ConvertComboInstrumentID(*it, strCmbSubCode1, strCmbSubCode2);
					m_pSvr->SetSubscribeStatus( strCmbSubCode1,S_STATUS_TEMP,S_STATUS_DEL);
					m_pSvr->SetSubscribeStatus( strCmbSubCode2,S_STATUS_TEMP,S_STATUS_DEL);
				}
#endif
				m_pSvr->SetSubscribeStatus(*it,S_STATUS_QUOT,S_STATUS_DEL);
			}
		}
		if(table)
		{
			for(int row=0;row<table->GetNumberRows();row++)
			{
				string strid=table->GetInstrumentIDFromRow(row);
#ifdef _USE_COMBOINSTRUMENT
				bIsComboCode = m_pSvr->GetProductClassType(strid) == THOST_FTDC_PC_Combination;
				if(bIsComboCode) {
					ConvertComboInstrumentID(strid, strCmbSubCode1, strCmbSubCode2);
					m_pSvr->SetSubscribeStatus( strCmbSubCode1,S_STATUS_TEMP);
					m_pSvr->SetSubscribeStatus( strCmbSubCode2,S_STATUS_TEMP);
				}
#endif
				m_pSvr->SetSubscribeStatus(strid,S_STATUS_QUOT);
			}
		}
	}
	return 0;
}
int CQuotPanel::GetIndexFromGroupId(int id,std::vector<OneGroupInfo>* allgroup)
{
	if(allgroup==NULL)allgroup=&m_AllGroups;
	for(int index=0;index<(int)allgroup->size();index++)
	{	
		OneGroupInfo& info=(*allgroup)[index];
		if(info.GroupId==id)
			return index;
	}
	return -1;
}
int CQuotPanel::GetButtonIdFromGroupId(int id,std::vector<OneGroupInfo>* allgroup)
{
	if(allgroup==NULL)allgroup=&m_AllGroups;
	for(int index=0;index<(int)allgroup->size();index++)
	{	
		OneGroupInfo& info=(*allgroup)[index];
		if(info.GroupId==id)
			return info.pWxButton->GetId();
	}
	return 0;
}
//void CQuotPanel::OnGetInstrumentID(wxCommandEvent& evt)
//{
//	wxGridCellCoordsArray arr1=m_grid->GetSelectionBlockTopLeft();
//	if(arr1.size()!=1) return;
//	int selectrow=arr1[0].GetRow();
//	QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
//	if(pTable)
//		evt.SetString(pTable->GetInstrumentIDFromRow(selectrow).c_str());
//
//}
void CQuotPanel::OnSubscrible(wxCommandEvent& evt)
{
	if(evt.GetInt()==0)
	{
		if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_QUOT_TABLE);
	}
}

void CQuotPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();
    if(!m_GroupButtonInToolBar)
    {
        for(int index=0;index<(int)m_AllGroups.size();index++)
        {	
            OneGroupInfo& info=m_AllGroups[index];
            info.pWxButton->SetLabel(info.name);
            info.pWxButton->SetPosition(wxPoint((BUTTON_WIDTH+5)*index, rt.height-30));
        }
    }
    if(m_grid)
    {
        m_grid->SetPosition(wxPoint(0,0));
        if(m_GroupButtonInToolBar)
	        m_grid->SetSize(rt.width,rt.height);
        else
	        m_grid->SetSize(rt.width,rt.height-LIST_MARGIN);
    }
	event.Skip();
}

//底层平台发过来一条新行情
void CQuotPanel::OnRcvQuot(wxCommandEvent& evt)
{

    DataRtnDepthMarketData* pTmp= (DataRtnDepthMarketData*)(evt.GetClientData());
    if(!pTmp) return;
	for(int index=0;index<(int)m_AllGroups.size();index++)
	{
		OneGroupInfo& info=m_AllGroups[index];
		info.table->LoadNewQuotData(pTmp->DepthMarketDataField);
	}

    wxDELETE(pTmp);
    evt.SetClientData(NULL);
}
int CQuotPanel::GetIndexFromCtrlID(int id)
{
	for(int index=0;index<(int)m_AllGroups.size();index++)
	{	
		OneGroupInfo& info=m_AllGroups[index];
		if(info.pWxButton->GetId()==id)
			return index;
	}
	return 0;
}
//切换合约组
void CQuotPanel::OnButtonClick(wxCommandEvent& event)
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}
	wxLogDebug("OnButtonClick %d",GetTickCount());
	int groupcount=(int)m_AllGroups.size();
	if(groupcount==0) return;

    int index=GetIndexFromCtrlID(m_GroupButtonInToolBar?event.GetInt():event.GetId());
	for(int i=0;i<(int)m_AllGroups.size();i++)
	{	
		m_AllGroups[i].pWxButton->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
	}
	m_nCurrentIndex= index;  
	if(m_nCurrentIndex>=0&&m_nCurrentIndex<m_AllGroups.size()) 
	{
		m_AllGroups[m_nCurrentIndex].pWxButton->SetBackgroundColour(wxColor(214,48,18));//红色
		m_grid->SetTable(m_AllGroups[m_nCurrentIndex].table, false,wxGrid::wxGridSelectRows);
		UpdateCurrentInstrumentIDs(m_AllGroups[m_nCurrentIndex].table);
		SelectCurrentRow();
		pCfgMgr->SetCurrentGruopId( m_AllGroups[m_nCurrentIndex].GroupId);
	}
	m_grid->Refresh();
	wxLogDebug("OnButtonClick %d",GetTickCount());
}

//配置文件发生变化
void CQuotPanel::OnCfgChanged(wxCommandEvent& evt)
{
    WXLOG_INFO("CQuotPanel::OnCfgChanged");
	LoadUI(false);
	if(m_nCurrentIndex>=0&&m_nCurrentIndex<m_AllGroups.size()) 
	{
		m_grid->SetTable(m_AllGroups[m_nCurrentIndex].table, false,wxGrid::wxGridSelectRows);
		UpdateCurrentInstrumentIDs(m_AllGroups[m_nCurrentIndex].table);
		SelectCurrentRow();
	}
	else if(m_AllGroups.size()>0)
	{
		m_nCurrentIndex=0;
		m_grid->SetTable(m_AllGroups[m_nCurrentIndex].table, false,wxGrid::wxGridSelectRows);
		UpdateCurrentInstrumentIDs(m_AllGroups[m_nCurrentIndex].table);
		SelectCurrentRow();
	}

	m_grid->Refresh();
	OnSize(wxSizeEvent());
}

CQuotPanel::~CQuotPanel()
{
	Release();
}
void CQuotPanel::Release()
{
    SAFE_DELETE(m_grid);
    map<int,vector<string>>::iterator iter;
    for(int index=0;index<(int)m_AllGroups.size();index++)
    {	
        OneGroupInfo& info=m_AllGroups[index];
        if(!m_GroupButtonInToolBar)
        {
            wxDELETE(info.pWxButton);
        }
        for(iter=info.pTextData->begin();iter!=info.pTextData->end();++iter)
        {
            iter->second.clear();
        }
        info.pTextData->clear();
        wxDELETE(info.pTextData);
        wxDELETE(info.table);

    }
    m_AllGroups.clear();
    if(m_GroupButtonInToolBar)
    {
        wxToolBar* tbar=((MainFrame*)GETTOPWINDOW())->CreateToolbar_Begin();
        ((MainFrame*)GETTOPWINDOW())->CreateToolbar_End(tbar);
    }
    //SAFE_DELETE(m_pWriteLog);
}
int CQuotPanel::GetLineHeight(wxFont& cellfont)
{
	// we cache the line height as calling GetTextExtent() is slow
	wxClientDC dc( this );
	wxFont oldfont =dc.GetFont();
	dc.SetFont(cellfont);
	wxCoord y;
	dc.GetTextExtent(_T("H"), NULL, &y);
	dc.SetFont(oldfont);
	y += 6;
	return y ;
}
void CQuotPanel::InvalidRawQuot(PlatformStru_DepthMarketData& rawQuot,std::string& strInstrumentID)
{
     memset(&rawQuot,0,sizeof(PlatformStru_DepthMarketData));

     double maxdouble=std::numeric_limits<double>::max();
	///交易日
	//rawQuot.TradingDay;
	///合约代码
     strcpy(rawQuot.InstrumentID,strInstrumentID.c_str());
	///交易所代码
	//rawQuot.ExchangeID;
	///合约在交易所的代码
	//rawQuot.ExchangeInstID;
	///最新价
	rawQuot.LastPrice=maxdouble;
	///上次结算价
	rawQuot.PreSettlementPrice=maxdouble;
	///昨收盘
	rawQuot.PreClosePrice=maxdouble;
	///昨持仓量
	rawQuot.PreOpenInterest=maxdouble;
	///今开盘
	rawQuot.OpenPrice=maxdouble;
	///最高价
	rawQuot.HighestPrice=maxdouble;
	///最低价
	rawQuot.LowestPrice=maxdouble;
	///数量
	rawQuot.Volume=0;
	///成交金额
	rawQuot.Turnover=0;
	///持仓量
	rawQuot.OpenInterest=maxdouble;
	///今收盘
	rawQuot.ClosePrice=maxdouble;
	///本次结算价
	rawQuot.SettlementPrice=maxdouble;
	///涨停板价
	rawQuot.UpperLimitPrice=maxdouble;
	///跌停板价
	rawQuot.LowerLimitPrice=maxdouble;
	///昨虚实度
	rawQuot.PreDelta=0;
	///今虚实度
	rawQuot.CurrDelta=0;
	///最后修改时间
	strcpy(rawQuot.UpdateTime,"00:00:00");
	///最后修改毫秒
	rawQuot.UpdateMillisec=0;
	///申买价一
	rawQuot.BidPrice1=maxdouble;
	///申买量一
	rawQuot.BidVolume1=0;
	///申卖价一
	rawQuot.AskPrice1=maxdouble;
	///申卖量一
	rawQuot.AskVolume1=0;
	///申买价二
	rawQuot.BidPrice2=maxdouble;
	///申买量二
	rawQuot.BidVolume2=0;
	///申卖价二
	rawQuot.AskPrice2=maxdouble;
	///申卖量二
	rawQuot.AskVolume2=0;
	///申买价三
	rawQuot.BidPrice3=maxdouble;
	///申买量三
	rawQuot.BidVolume3=0;
	///申卖价三
	rawQuot.AskPrice3=maxdouble;
	///申卖量三
	rawQuot.AskVolume3=0;
	///申买价四
	rawQuot.BidPrice4=maxdouble;
	///申买量四
	rawQuot.BidVolume4=0;
	///申卖价四
	rawQuot.AskPrice4=maxdouble;
	///申卖量四
	rawQuot.AskVolume4=-1;
	///申买价五
	rawQuot.BidPrice5=maxdouble;
	///申买量五
	rawQuot.BidVolume5=-1;
	///申卖价五
	rawQuot.AskPrice5=maxdouble;
	///申卖量五
	rawQuot.AskVolume5=0;
	///当日均价
	rawQuot.AveragePrice=maxdouble;
}
void CQuotPanel::OnPanelFocus(wxCommandEvent& evt)
{
	if(m_grid)
		m_grid->SetFocus();

}
void CQuotPanel::OnSetGroup(wxCommandEvent& evt)
{
	int index=evt.GetInt();
	if(index>=0&&index<m_AllGroups.size())
	{
		wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED,m_AllGroups[index].pWxButton->GetId());
        if(m_GroupButtonInToolBar)
		    ((MainFrame*)GETTOPWINDOW())->ProcessEvent(cmdevt);
        else
            ProcessEvent(cmdevt);

	}
}
//void CQuotPanel::OnGetGroup(wxCommandEvent& evt)
//{
//	if(m_nCurrentIndex>=0&&m_nCurrentIndex<m_AllGroups.size())
//	{
//		evt.SetString(m_AllGroups[m_nCurrentIndex].name.c_str());
//		evt.SetInt(m_AllGroups[m_nCurrentIndex].GroupId);
//	}
//
//}


void CQuotPanel::OnSetInstrumentID(wxCommandEvent& evt)
{
//	wxString strInstrumentID=evt.GetString();

    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
	wxString strInstrumentID(EvtParamStr);

	map<int,vector<string>>::iterator iter;
	for(int index=0;index<(int)m_AllGroups.size();index++)
	{	
		OneGroupInfo& info=m_AllGroups[index];
		if(!info.pTextData) continue;
		for(iter=info.pTextData->begin();iter!=info.pTextData->end();++iter)
		{
			vector<string>& vec=iter->second;
			if(vec.size()>0&&vec[0]==strInstrumentID)
			{
				//选中合约组
				wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED,info.pWxButton->GetId());
                if(m_GroupButtonInToolBar)
				    ((MainFrame*)GETTOPWINDOW())->ProcessEvent(cmdevt);
                else
                    ProcessEvent(cmdevt);
				m_grid->SelectBlock(iter->first,0,iter->first,info.table->GetNumberCols()-1);
				m_grid->MakeCellVisible(iter->first,0/*info.table->GetNumberCols()/2*/);
				return;
			}
		}
	}

}
void CQuotPanel::OnRangeSelected( wxGridRangeSelectEvent& evt )
{
	WXLOG_INFO("OnRangeSelected Begin(%d)",GetTickCount()%1000);
	int row1=evt.GetTopLeftCoords().GetRow();
	int row2=evt.GetBottomRightCoords().GetRow();
	if(evt.Selecting()&&row1==row2)
	{
		WXLOG_INFO("OnRangeSelected(%d)",row1);
		QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
		if(pTable)
		{
			wxCommandEvent sendevt(wxEVT_GRID_INSTRUMENTID_CHANGED);
//			sendevt.SetString(pTable->GetInstrumentIDFromRow(row1).c_str());
            if(1)
            {
                DWORD EvtParamID;
                std::string EvtParamStr(pTable->GetInstrumentIDFromRow(row1).c_str());
                CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
                sendevt.SetInt((int)EvtParamID);

				if ( m_pSvr && m_pSvr->SupportPlugins() )
				{
					g_Plusin.SetOrderModuleCurInstrumentID( EvtParamStr);
				}
            }
			GETTOPWINDOW()->AddPendingEvent(sendevt);
		}
	}
	WXLOG_INFO("OnRangeSelected End(%d)",GetTickCount()%1000);
}
void CQuotPanel::OnCellLeftClick(wxGridEvent& evt)
{
	QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
	if(pTable&&pTable->GetDirectionFromColumn(evt.GetCol()))
	{
		wxCommandEvent sendchangeevt(wxEVT_GRID_INSTRUMENTID_CHANGED);
//		sendchangeevt.SetString(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
        if(1)
        {
            DWORD EvtParamID;
            std::string EvtParamStr(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
            sendchangeevt.SetInt((int)EvtParamID);

			if ( m_pSvr && m_pSvr->SupportPlugins() )
			{
				g_Plusin.SetOrderModuleCurInstrumentID( EvtParamStr);
			}
        }
		GETTOPWINDOW()->AddPendingEvent(sendchangeevt);

		wxCommandEvent sendevt(wxEVT_ORDERINSERT_MOUSEORDER);
//		sendevt.SetString(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
		sendevt.SetExtraLong(pTable->GetDirectionFromColumn(evt.GetCol()));
//		sendevt.SetInt(1);
        if(1)
        {
            DWORD EvtParamID;
            std::string EvtParamStr(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,1);
            sendevt.SetInt((int)EvtParamID);
        }
		GETTOPWINDOW()->AddPendingEvent(sendevt);
		wxString strInfo=wxString::Format(LOADFORMATSTRING(USEROPERATE_CLICK_PMT,"%s%c"), 
			pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str(), 
			pTable->GetDirectionFromColumn(evt.GetCol()));
		USERLOG_INFO("%s",strInfo.c_str());
	}
	evt.Skip();
}
void CQuotPanel::OnCellLeftDoubleClick(wxGridEvent& evt)
{
	wxCommandEvent parentevt(wxEVT_QUOT_VIEW_CHANGE);
	GetParent()->ProcessEvent(parentevt);
	if(parentevt.GetSkipped())
	{
		QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
		if(pTable&&pTable->GetDirectionFromColumn(evt.GetCol()))
		{
			wxCommandEvent sendchangeevt(wxEVT_GRID_INSTRUMENTID_CHANGED);
//			sendchangeevt.SetString(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
            if(1)
            {
	            DWORD EvtParamID;
	            std::string EvtParamStr(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
	            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
                sendchangeevt.SetInt((int)EvtParamID);

				if ( m_pSvr && m_pSvr->SupportPlugins() )
				{
					g_Plusin.SetOrderModuleCurInstrumentID( EvtParamStr);
				}
            }
			GETTOPWINDOW()->AddPendingEvent(sendchangeevt);

			wxCommandEvent sendevt(wxEVT_ORDERINSERT_MOUSEORDER);
//			sendevt.SetString(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
			sendevt.SetExtraLong(pTable->GetDirectionFromColumn(evt.GetCol()));
//			sendevt.SetInt(2);
            if(1)
            {
	            DWORD EvtParamID;
	            std::string EvtParamStr(pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str());
	            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,2);
                sendevt.SetInt((int)EvtParamID);
            }
			GETTOPWINDOW()->AddPendingEvent(sendevt);
			wxString strInfo=wxString::Format(LOADFORMATSTRING(USEROPERATE_CLICK_PMT,"%s%c"), 
				pTable->GetInstrumentIDFromRow(evt.GetRow()).c_str(), 
				pTable->GetDirectionFromColumn(evt.GetCol()));
			USERLOG_INFO("%s",strInfo.c_str());
		}
	}
	evt.Skip();
}
void CQuotPanel::OnRightClick(wxGridEvent& evt)
{
	m_RightClickRow=evt.GetRow();
	if(m_RightClickRow>=0)
	{
		int colcount=m_grid->GetTable()->GetNumberCols()-1;
		m_grid->SelectBlock(m_RightClickRow,0,m_RightClickRow,colcount);
	}

	SAFE_DELETE(m_pContextMenu);
	m_pContextMenu=new wxMenu();
	wxMenuItem* pMenuItem=NULL;
	wxCommandEvent request_event(wxEVT_UPDATE_CONFIG_PANE);
	GETTOPWINDOW()->ProcessEvent(request_event);
	bool configpaneopen=request_event.GetInt()?true:false;
	if(m_pSvr&&m_pSvr->HaveOrderType(UIOT_PROFIT))
	{
		pMenuItem=m_pContextMenu->Append(ID_MENU_PROFIT,LOADSTRING(CONTEXTMENU_PROFIT));//交与主窗体响应
		pMenuItem->Enable(!configpaneopen);
	}
	if(m_pSvr&&m_pSvr->HaveOrderType(UIOT_CONDITION))
	{
		pMenuItem=m_pContextMenu->Append(ID_MENU_CONDITION,LOADSTRING(CONTEXTMENU_CONDITION));//交与主窗体响应
		pMenuItem->Enable(!configpaneopen);	
	}
	if( m_pContextMenu->GetMenuItemCount() > 0 )
	{
		m_pContextMenu->AppendSeparator();
	}
	
	m_pContextMenu->Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	pMenuItem=m_pContextMenu->Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	pMenuItem=m_pContextMenu->Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));
	if ( !configpaneopen )
	{
		m_pContextMenu->AppendSeparator();
		pMenuItem=m_pContextMenu->Append(ID_MENU_SELECTINSTRUMENT,LOADSTRING(CONTEXTMENU_SELECTINSTRUMENT));
	}
	if(m_pSvr&&m_pSvr->SupportPlugins())
	{
		vector<wxString> vAppID;
		g_Plusin.GetPlusinMenuInfo(MENU_POS_QUOTE,vAppID);
		if(vAppID.size()>0)
			m_pContextMenu->AppendSeparator();
		wxString strMName;
		PLUGWIN2 plugwin;
		for (int i=0;i<vAppID.size()&&i<MAX_PLUGIN_COUNT;++i)
		{
			if(g_Plusin.GetPlusinByAppID(vAppID[i],plugwin))
			{
				strMName=plugwin.strMenuName;
				if(strMName.IsEmpty()) 
					strMName=wxString::Format("Item %d",i+1);
				m_pContextMenu->Append(ID_CONTEXT_PLUGINS_BEGIN+i,strMName,vAppID[i]);
			}
		}
	}
    if(m_pContextMenu->GetMenuItemCount()>0)
    {
        POINT pt;
        ::GetCursorPos(&pt);
        wxPoint point(pt.x,pt.y);
        point = ScreenToClient(point);
        PopupMenu(m_pContextMenu,point.x,point.y);
    }
    evt.Skip();
}
void CQuotPanel::ContextMenuRespond(wxCommandEvent& event)
{
#define  IDT_ZiXuanHeYue			1002
	int menuid=event.GetId()-ID_MENU_ADDTOMYSELF;
	if(menuid>=0&&menuid<50&&m_RightClickRow>=0)
	{
		QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
		CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
		string InstrumentId=pTable->GetInstrumentIDFromRow(m_RightClickRow);
		if(pCfgMgr->GetInstrumentGroupMemberCount(menuid)>=m_pSvr->GetGroupMaxContractNum())
		{
			wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		}
		else
		{
			pCfgMgr->AddInstrument(menuid,InstrumentId.c_str());
			pCfgMgr->SetCurrentGruopId(0);//解决CITHINC-1131
			wxCommandEvent myEvent(wxEVT_CFG_CHANGE);
			ProcessEvent(myEvent);    
		}   
		return;
	}
	if(m_pSvr&&m_pSvr->SupportPlugins())
	{
		menuid=event.GetId()-ID_CONTEXT_PLUGINS_BEGIN;
		if(m_pContextMenu&&menuid>=0&&menuid<MAX_PLUGIN_COUNT)
		{
			wxMenuItem* pItem=m_pContextMenu->FindItem(event.GetId());
			if(pItem)
			{
				g_Plusin.ShowPlusin(pItem->GetHelp());
			}
		}
	}
	switch(event.GetId())
	{
	case ID_MENU_COLUMNCONFIG:
        SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_QUOTETABLE);
		break;
	case ID_MENU_EXPORT:
		{
			QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
			BEGIN_DO_EXPORT(GID_QUOT_TABLE)
			int colcount=pTable->GetNumberCols();
			for(int col=0;col<colcount;col++)
			{
				strline+=pTable->GetColLabelValue(col);
				if(col+1!=colcount) strline+=",";
			}			    
			strline+="\r\n";
			file.Write(strline.c_str(),strline.size());
			for(int index=0;index<pTable->GetNumberRows();index++)
			{
				strline.Empty();
				for(int col=0;col<colcount;col++)
				{
					strline+=pTable->GetValue(index,col);
					if(col+1!=colcount) strline+=",";
				}
				strline+="\r\n";
				file.Write(strline.c_str(),strline.size());
			}
			END_DO_EXPORT()
		}
		break;
	case ID_MENU_SELECTINSTRUMENT:
        SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_SELFGROUP);
		break;
	case ID_MENU_AUTO_ADJUST_COL_WIDTH:
		m_grid->AutoAdjustColumnWidth();
		SaveColWidth();
		break;
	}

}

void CQuotPanel::OnColumnResize( wxGridSizeEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetRowOrCol();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_QUOT_TABLE ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUOT_TABLE, nCol);
	pColCfg->Width = m_grid->GetColumnWidth( nCol );
}

void CQuotPanel::OnKeyDown( wxKeyEvent& event )
{
	if ( event.GetKeyCode() == WXK_DELETE )
	{
		CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
		if ( NULL == pCfgMgr )
		{
			return;
		}

		QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
		if ( pTable == NULL )
		{
			return;
		}

		if ( m_nCurrentIndex < 0 || m_nCurrentIndex >= m_AllGroups.size())
		{
			return;
		}

		vector<int> vRows;
		if ( !m_grid->GetSelections(vRows))
		{
			return;
		}

		if ( !vRows.empty() && pCfgMgr->GetMainWindowCfg()->bDelInstrumentWarning )
		{
			//弹出提示框
			//if(wxMessageBox( LOADSTRING(DELETE_INSTRUMENT_CONFIRM), LOADSTRING(OIP_FASTTRADER),wxYES_NO) != wxYES )
			int ret = wxID_CANCEL;
			CZqMessageBox msgbox(this);
			wxString strcaption = LOADSTRING(TITLE_DELETE_INSTRUMENTID);
			wxString strmsg = LOADSTRING(DELETE_INSTRUMENT_CONFIRM);
			msgbox.ResetMessage(strcaption, strmsg, 
				CZqMessageBox::btnYes|CZqMessageBox::btnNo, CZqMessageBox::deleteInstrumentID);
			ret = msgbox.ShowModal();
            if(ret!=wxID_YES)
			{
				return;
			}
		}

		//先从配置中删除选定合约
		for ( int i = 0; i < vRows.size(); i++ )
		{
			string strInstrumentID = pTable->GetInstrumentIDFromRow( vRows[i] );
			pCfgMgr->DelInstrument(m_AllGroups[m_nCurrentIndex].GroupId, strInstrumentID.c_str());
		}

		//在列表中删除
		pTable->DeleteRows( vRows );
		m_grid->SetTable(m_AllGroups[m_nCurrentIndex].table, false,wxGrid::wxGridSelectRows);
		SelectCurrentRow();	
		m_grid->Refresh();
		return;
	}

	event.Skip();
}

#ifdef _USE_MULTI_LANGUAGE
void CQuotPanel::OnLanguageChanged(wxCommandEvent& event)
{
	m_grid->GetGridColLabelWindow()->Refresh();
}
#endif

bool CQuotPanel::GetCurInstrumentGrpInfo(std::string& GrpName,int& GrpID)
{
	if(m_nCurrentIndex>=0&&m_nCurrentIndex<m_AllGroups.size())
    {
        GrpName=m_AllGroups[m_nCurrentIndex].name;
        GrpID=m_AllGroups[m_nCurrentIndex].GroupId;
        return true;
    }
    return false;
}
void CQuotPanel::GetCurInstrumentID(std::string& strInstrument)
{
    strInstrument.clear();
	wxGridCellCoordsArray arr1=m_grid->GetSelectionBlockTopLeft();
	if(arr1.size()!=1) return;
	int selectrow=arr1[0].GetRow();
	QuotGridTable* pTable=(QuotGridTable*)m_grid->GetTable();
	if(pTable)
        strInstrument=pTable->GetInstrumentIDFromRow(selectrow);
}

void CQuotPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_grid )
	{
		return;
	}

	int nColCount = m_grid->GetNumberCols();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_QUOT_TABLE ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_QUOT_TABLE, i);
		pColCfg->Width = m_grid->GetColumnWidth( i );
	}
}
//初始化结束后一次加载全部合约行情，只运行一次
void CQuotPanel::LoadOldQuotData()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	PlatformStru_DepthMarketData rawQuot;
	for(size_t i=0;i<m_AllGroups.size();++i)
	{
		int instrumentIDcnt=pCfgMgr->GetInstrumentGroupMemberCount(i);
		for(int n=0; n<instrumentIDcnt; ++n)
		{
			string strInstrumentID=pCfgMgr->GetInstrumentId(i, n);
			if(!(m_pSvr&&m_pSvr->GetQuotInfo(strInstrumentID, rawQuot)>=0))
			{
				InvalidRawQuot(rawQuot,strInstrumentID);
			}
			m_AllGroups[i].table->LoadNewQuotData(rawQuot,false);
		}
	}
	wxCommandEvent cmdevt(wxEVT_COMMAND_BUTTON_CLICKED);
	if(m_nCurrentIndex>=0&&m_nCurrentIndex<m_AllGroups.size())
		cmdevt.SetInt((int)m_AllGroups[m_nCurrentIndex].pWxButton->GetId());
	ProcessEvent(cmdevt);
}
void CQuotPanel::OnMAPlatformModify(wxCommandEvent& event)
{
	IPlatformSingleSvr* newsvr=(IPlatformSingleSvr*)event.GetInt();
	if(newsvr!=m_pSvr)
	{
		if(m_pSvr)	
			m_pSvr->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_QUOT_TABLE);
		m_pSvr=newsvr;
		if(m_pSvr)
			m_pSvr->SubscribeBusinessData(BID_RtnDepthMarketData, GID_QUOT_TABLE, quotCallBackFunc);
		LoadOldQuotData();
	}
}