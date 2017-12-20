#include "stdafx.h"

#include <wx/textfile.h>
#include "Module-Misc/GLobalFunc.h"
#include "Module-Misc/constants.h"
#include "MouseHandyInputPaneX.h"
#include "..\\inc\\cfgMgr\\InstrumentGroup.h"
#include "..\\inc\\cfgMgr\\InstrumentGroupMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/SelfGroupFunc.h"
#include "ConfigAppDlg.h"
#include "CGroupSetPanel.h"
#include "CSelfGroupSetPanel.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
DEFINE_EVENT_TYPE(wxEVT_CODE_CLICKED)

//IMPLEMENT_DYNAMIC_CLASS(CZiXunHeYue,wxPanel)
extern vector<GroupInfo2X> g_vecGroupInfo;
BEGIN_EVENT_TABLE(CSelfGroupSetPanel,wxPanel)
	EVT_BUTTON(ID_BTN_UP,OnButtonUp)
	EVT_BUTTON(ID_BTN_DOWN,OnButtonDown)
	EVT_BUTTON(ID_BTN_ADD,OnButtonAdd)
	EVT_BUTTON(ID_BTN_DEL,OnButtonDel)
	EVT_BUTTON(ID_BTN_SET,OnButtonSet)

	EVT_COMBOBOX(ID_CBX_EXCHANGE, OnExChangeSel)  
	EVT_COMBOBOX(ID_CBX_INSTRUMENT_GROUP, OnInstrumentGroupSel) 

	EVT_COMMAND(wxID_ANY, wxEVT_CODE_CLICKED, OnShowCodeClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_CODE_MOUSEINPUT, OnMouseInputCode)
	EVT_TEXT(ID_EDIT_INPUT_INSTRUMENT_NAME, OnContractNameChanged)
	EVT_GRID_CMD_CELL_LEFT_DCLICK(ID_BASELIST, OnCellLeftDoubleClick)
	EVT_GRID_CMD_CELL_LEFT_DCLICK(ID_CUSTOMLIST, OnCustomListActivated)
	EVT_TREE_ITEM_EXPANDING(ID_BASELIST,OnTreeItemExpanding)
	EVT_TREE_ITEM_ACTIVATED(ID_BASELIST,OnTreeItemActived)
END_EVENT_TABLE()

CSelfGroupSetPanel::CSelfGroupSetPanel(IPlatformSingleSvr* pSvr,wxWindow *parent,
						 wxWindowID winid ,
						 const wxPoint& pos ,
						 const wxSize& size ,
						 long style,
						 const wxString& name ):
wxPanel(parent,winid,pos,size,style,name),
m_bNoKeyChangeText(TRUE),
m_MouseHandyPopupPane(NULL),
m_cbxExchange(NULL),
m_listBase(NULL),
m_treeBase(NULL),
m_pSvr(pSvr)
{
	//m_MouseHandyPopupPane = new MouseHandyInputPaneX(this);
	//m_MouseHandyPopupPane->Connect(wxEVT_KEY_DOWN, 
	//	wxKeyEventHandler(CSelfGroupSetPanel::OnMousePanelKeyAscii), NULL, this);

	new wxStaticText(this, wxID_ANY, LOADSTRING(INPUT_DOT), wxPoint(14, 11));
	m_editInputInstrumentName = new wxTextCtrl(this, ID_EDIT_INPUT_INSTRUMENT_NAME, 
		wxEmptyString, wxPoint(68, 6), wxSize(117, 19), 0);
	m_editInputInstrumentName->Connect(ID_EDIT_INPUT_INSTRUMENT_NAME, wxEVT_LEFT_UP, 
		wxMouseEventHandler(CSelfGroupSetPanel::OnCodeMouseClicked), NULL, this);
	new wxStaticText(this, wxID_ANY, LOADSTRING(GROUP_DOT), wxPoint(252, 9));
	m_cbxInstrumentGroup = new wxOwnerDrawnComboBox(this, ID_CBX_INSTRUMENT_GROUP,
			wxEmptyString, wxPoint(308, 6), wxSize(119, 20),0,0,wxCB_READONLY);
	new wxButton(this, ID_BTN_SET, LOADSTRING(SETUP), wxPoint(438, 3), wxSize(41, 25), 
			0);
	new wxStaticLine(this, wxID_ANY, wxPoint(177, 355), wxSize(150, -1), wxLI_HORIZONTAL);
	new wxStaticLine(this, wxID_ANY, wxPoint(167, 351), wxSize(492, -1), wxLI_HORIZONTAL);
	
	new wxButton(this, ID_BTN_ADD, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(195, 129), wxSize(50, 25),0);
	new wxButton(this, ID_BTN_DEL, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(195, 236), wxSize(50, 25),0);

	new wxBitmapButton(this, ID_BTN_UP, wxBitmap("zqBITMAP_UP"), wxPoint(439, 129));
	new wxBitmapButton(this, ID_BTN_DOWN, wxBitmap("zqBITMAP_DOWN"), wxPoint(439, 236));

	if(m_pSvr&&m_pSvr->ContractCountIsFit())
	{
		new wxStaticLine(this, wxID_ANY, wxPoint(17, 31), wxSize(167, -1), wxLI_HORIZONTAL);	
		new wxStaticText(this, wxID_ANY, LOADSTRING(EXCHANGE_DOT), wxPoint(14, 45), wxDefaultSize, 0);
		m_cbxExchange = new wxOwnerDrawnComboBox(this, ID_CBX_EXCHANGE, wxEmptyString, wxPoint(68, 43), wxSize(117, 20),0,0,wxCB_READONLY );
		m_listBase = new InstrumentGrid(this, ID_BASELIST, wxPoint(14, 74), wxSize(170, 260),wxWANTS_CHARS|wxBORDER_SIMPLE);
	}
	else
	{
		m_treeBase=new wxTreeCtrl(this,ID_BASELIST,wxPoint(14, 31), wxSize(170, 303),wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_MULTIPLE);
	}
	m_selfgroupfunc=new SelfGroupFunc(m_treeBase);
	m_listCustom = new wxGrid(this, ID_CUSTOMLIST, wxPoint(253, 31), wxSize(178, 303), wxWANTS_CHARS|wxBORDER_SIMPLE);
	wxLogDebug("Tick=%d",GetTickCount());
    ReadCfg();
	wxLogDebug("Tick=%d",GetTickCount());
	if(m_pSvr) m_pSvr->GetInstrumentList(m_vecGroupInfo);
	wxLogDebug("Tick=%d",GetTickCount());
    if(m_MouseHandyPopupPane!=NULL) {
        m_MouseHandyPopupPane->SetCodeGroup(&m_vecGroupInfo);
    }
	wxLogDebug("Tick=%d",GetTickCount());
    InitControls();
	wxLogDebug("Tick=%d",GetTickCount());
}

void CSelfGroupSetPanel::InitControls()
{
	if(m_cbxExchange&&m_listBase)
	{
		std::vector<std::string> gridheaders;
		gridheaders.push_back(LOADSTRING(CONTRACT_CODE));
		if(m_pSvr&&!m_pSvr->ContractNameIsID())   
			gridheaders.push_back(LOADSTRING(CONTRACT_NAME));

		if( m_vecGroupInfo.size() != 0 )
		{
			for( unsigned int i=0; i<m_vecGroupInfo.size() ; i++ )
			{		
				m_cbxExchange->AppendString(m_vecGroupInfo[i].GroupName);
				m_listBase->AddInstrumentGridTable(m_vecGroupInfo[i].InstrVec,gridheaders);
			}
			m_cbxExchange->SetSelection( 0 );
			m_listBase->SetTable(0);
		}
	}
	if(m_selfgroupfunc) m_selfgroupfunc->AddTreeData();

	m_listCustom->CreateGrid(0, 1);
	m_listCustom->SetColLabelValue(0, LOADSTRING(SELF_CONTRACT_CODE));
	m_listCustom->SetColLabelSize(23);
	m_listCustom->EnableDragGridSize(false);
	m_listCustom->SetRowLabelSize( 0 );
	m_listCustom->EnableDragRowSize(false);
	m_listCustom->EnableEditing(false);
	m_listCustom->EnableGridLines(true);
	m_listCustom->SetScrollLineY(m_listCustom->GetDefaultRowSize());
	m_listCustom->SetCellHighlightPenWidth(0);
	m_listCustom->SetCellHighlightROPenWidth(0);
	m_listCustom->SetDefaultColSize(165);
	m_listCustom->SetSelectionMode(wxGrid::wxGridSelectRows);

	ResetSelfGroupItems();
}

void CSelfGroupSetPanel::OnButtonSet(wxCommandEvent& event)
{
	wxCommandEvent myEvent( wxEVT_SHOW_PANEL );
	myEvent.SetInt( ID_ITEM_GROUPSET );
	GetParent()->AddPendingEvent( myEvent );
}

void CSelfGroupSetPanel::OnButtonUp(wxCommandEvent& event)
{
	bool bFind = false;
	int nGroupIndex = 0;
	string strGroupName = m_cbxInstrumentGroup->GetValue();
	for( unsigned int i=0; i< g_vecGroupInfo.size(); i++)
	{
		if( strGroupName == g_vecGroupInfo[i].GroupName )
		{
			nGroupIndex = i;
			bFind = true;
			break;
		}
	}

	if ( !bFind )
	{
		return;
	}

	vector<int> vRows;
	GetCustomSelections(vRows);
	if( vRows.empty() )
		return;

	bool bSelectChange = false;
	for ( int i = 0; i < (int)vRows.size(); i++)
	{
		int nRow = vRows[i];
		if ( nRow == 0 )
		{
			return;
		}

		bSelectChange = true;
		string str = g_vecGroupInfo[nGroupIndex].InstrVec[nRow-1];
		g_vecGroupInfo[nGroupIndex].InstrVec[nRow-1] = g_vecGroupInfo[nGroupIndex].InstrVec[nRow];
		g_vecGroupInfo[nGroupIndex].InstrVec[nRow] = str;

		m_listCustom->SetCellValue(nRow-1, 0, g_vecGroupInfo[nGroupIndex].InstrVec[nRow-1]);
		m_listCustom->SetCellValue(nRow, 0, g_vecGroupInfo[nGroupIndex].InstrVec[nRow]);
		
	}

	if ( bSelectChange )
	{
		m_listCustom->SelectBlock(vRows[0]-1,0,vRows[vRows.size()-1]-1,m_listCustom->GetNumberCols()-1);
	}
}

void CSelfGroupSetPanel::OnButtonDown(wxCommandEvent& event)
{
	bool bFind = false;
	int nGroupIndex = 0;
	string strGroupName = m_cbxInstrumentGroup->GetValue();
	for( unsigned int i=0; i< g_vecGroupInfo.size(); i++)
	{
		if( strGroupName == g_vecGroupInfo[i].GroupName )
		{
			nGroupIndex = i;
			bFind = true;
			break;
		}
	}

	if ( !bFind )
	{
		return;
	}

	vector<int> vRows;
	GetCustomSelections(vRows);
	if( vRows.empty() )
		return;

	bool bSelectChange = false;
	for ( int i = vRows.size()-1 ; i >= 0; i--)
	{
		int nRow = vRows[i];
		if ( nRow == m_listCustom->GetNumberRows()-1 )
		{
			return;
		}

		bSelectChange = true;
		string str = g_vecGroupInfo[nGroupIndex].InstrVec[nRow+1];
		g_vecGroupInfo[nGroupIndex].InstrVec[nRow+1] = g_vecGroupInfo[nGroupIndex].InstrVec[nRow];
		g_vecGroupInfo[nGroupIndex].InstrVec[nRow] = str;

		m_listCustom->SetCellValue(nRow, 0, g_vecGroupInfo[nGroupIndex].InstrVec[nRow]);
		m_listCustom->SetCellValue(nRow+1, 0, g_vecGroupInfo[nGroupIndex].InstrVec[nRow+1]);	
	}

	if ( bSelectChange )
	{
		m_listCustom->SelectBlock(vRows[0]+1,0,vRows[vRows.size()-1]+1,m_listCustom->GetNumberCols()-1);
	}
}

void CSelfGroupSetPanel::OnButtonAdd(wxCommandEvent& event)
{
	string strGroupName = m_cbxInstrumentGroup->GetValue();
	if( strGroupName.empty())
	{
		return;
	}

	if( IsSystemGroup( strGroupName ))
	{
		wxMessageBox(LOADSTRING(DEFAULT_GROUP_CANNOT_EDIT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		return;
	}

	m_bNoKeyChangeText = FALSE;
	wxString strInstrumentName = m_editInputInstrumentName->GetValue();
	if ( !strInstrumentName.empty())
	{
		std::string strTempName = GetValidInstrument( strInstrumentName );
		if ( strTempName.empty() )
		{
			wxMessageBox(LOADSTRING(INVALID_CONTRACT_CODE),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			m_editInputInstrumentName->SetValue(wxEmptyString);
			m_bNoKeyChangeText = TRUE;
			return;
		}
		AddInstrumentToGroup( strTempName, strGroupName);
		m_editInputInstrumentName->SetValue(wxEmptyString);
		m_bNoKeyChangeText = TRUE;
		return;
	}
	if(m_listBase)
	{
		vector<int> rows;
		m_listBase->GetSelections(rows);
		wxString instrumentID;
		for(int index=0;index<(int)rows.size();++index)
		{
			instrumentID=m_listBase->GetValue(rows[index],0);
			if(!AddInstrumentToGroup(instrumentID.c_str(), strGroupName ))
				break;
		}
	}
	if(m_treeBase)
	{
	vector<string> contracts;
	if(m_selfgroupfunc) m_selfgroupfunc->GetSelections(contracts);
	for (int index=0;index<(int)contracts.size();++index)
	{
		if(!AddInstrumentToGroup(contracts[index], strGroupName ))
			break;
	}
	}
	m_bNoKeyChangeText = TRUE;
}

void CSelfGroupSetPanel::OnButtonDel(wxCommandEvent& event)
{
	string strGroupName = m_cbxInstrumentGroup->GetValue();
	if( IsSystemGroup( strGroupName ))
	{
		wxMessageBox(LOADSTRING(DEFAULT_GROUP_CANNOT_EDIT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		return;
	}

	vector<int> vRows;
	GetCustomSelections(vRows);
	if( vRows.empty() )
		return;

	m_listCustom->DeleteRows( vRows[0], vRows.size());

	for( unsigned int i=0; i<g_vecGroupInfo.size(); i++ )
	{
		if ( strGroupName == g_vecGroupInfo[i].GroupName )
		{
			g_vecGroupInfo[i].InstrVec.clear();
			for( int j = 0; j< m_listCustom->GetNumberRows(); j++ )
			{
				string str = m_listCustom->GetCellValue(j, 0);
				g_vecGroupInfo[i].InstrVec.push_back(str);
			}
			break;
		}
	}
}

bool CSelfGroupSetPanel::Show( bool bShow )
{
	wxPanel::Show(bShow);
	return true;
}

bool CSelfGroupSetPanel::IsChanged()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return false;
	}

	int nGroupCount = pCfgMgr->GetInstrumentGroupCount();
	if ( g_vecGroupInfo.size() != nGroupCount )
	{
		return true;
	}

	for ( UINT i = 0; i < g_vecGroupInfo.size(); i++)
	{
		GroupInfo2X oGroupInfo = g_vecGroupInfo.at(i);
		INSTRUMENT_GROUP_CFG* pGroupInfo = pCfgMgr->GetInstrumentGroupInfo(i);
		if ( NULL == pGroupInfo )
		{
			return true;
		}

		if ( oGroupInfo.bChecked != pGroupInfo->IsVisibility ||
			oGroupInfo.GroupId != pGroupInfo->GroupId ||
			oGroupInfo.GroupName != pGroupInfo->GroupName )
		{
			return true;
		}

		std::vector<std::string> vInstrument = *(pCfgMgr->GetGroupMember(i));
		if ( oGroupInfo.InstrVec.size() != vInstrument.size() )
		{
			return true;
		}

		for ( UINT j = 0; j < oGroupInfo.InstrVec.size(); j++)
		{
			if ( oGroupInfo.InstrVec.at(j) != vInstrument.at(j) )
			{
				return true;
			}
		}
	}

	return false;
}

bool CSelfGroupSetPanel::WriteCfg()
{
	if ( !IsChanged())
	{
		return false;
	}

	std::string strCurGroup = m_cbxInstrumentGroup->GetValue();  
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();

	pCfgMgr->ClearInstrumentGroupVector();

	for( unsigned int i=0; i<g_vecGroupInfo.size(); i++) 
	{
		string str = g_vecGroupInfo[i].GroupName;
		pCfgMgr->AddInstrumentGroup(str,g_vecGroupInfo[i].bChecked,g_vecGroupInfo[i].GroupId);
		if( strCurGroup == g_vecGroupInfo[i].GroupName)
			pCfgMgr->SetCurrentGruopId( g_vecGroupInfo[i].GroupId );
	}

	for( unsigned int i=0; i<g_vecGroupInfo.size(); ++i )
	{
		for( unsigned int j =0; j<g_vecGroupInfo[i].InstrVec.size(); j++ )
		{
			pCfgMgr->GetGroupMember(i)->push_back(g_vecGroupInfo[i].InstrVec[j]);
			if( strCurGroup == g_vecGroupInfo[i].GroupName)
				pCfgMgr->SetCurrentGruopId( g_vecGroupInfo[i].GroupId );
		}

	}

	return true;
}

void CSelfGroupSetPanel::ReadCfg()
{
	if( g_vecGroupInfo.size() !=0 )
		return;

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if( !pCfgMgr )
		return;

	int count = pCfgMgr->GetInstrumentGroupCount();
	for( int i =0 ; i<count; i++ )
	{
		LPINSTRUMENT_GROUP_CFG info = pCfgMgr->GetInstrumentGroupInfo(i);
		if ( NULL == info )
		{
			continue;
		}

		GroupInfo2X GroupInfo;	
		GroupInfo.GroupName = info->GroupName;
		GroupInfo.InstrVec = *(pCfgMgr->GetGroupMember(i));
		GroupInfo.bChecked = info->IsVisibility;
		GroupInfo.GroupId = info->GroupId;
		g_vecGroupInfo.push_back( GroupInfo ); 
	}
}

void CSelfGroupSetPanel::OnInstrumentGroupSel(wxCommandEvent& event)
{
	if ( m_listCustom->GetNumberRows() > 0)
	{
		m_listCustom->DeleteRows(0, m_listCustom->GetNumberRows());
	}

	wxString str = m_cbxInstrumentGroup->GetValue() ;
	for( unsigned int i=0; i<g_vecGroupInfo.size(); i++ )
	{
		if( str == g_vecGroupInfo[i].GroupName )
		{
			for( unsigned int j=0; j<g_vecGroupInfo[i].InstrVec.size(); j++ )
			{
				m_listCustom->AppendRows();
				m_listCustom->SetCellValue(g_vecGroupInfo[i].InstrVec[j], m_listCustom->GetNumberRows()-1, 0);
			}
		}
	}
}

void CSelfGroupSetPanel::OnExChangeSel( wxCommandEvent& event )
{
	if(m_cbxExchange&&m_listBase)
	{
		int sel=m_cbxExchange->GetSelection();
		m_listBase->SetTable(sel);
		m_listBase->Refresh();
	}
}
void CSelfGroupSetPanel::OnTreeItemExpanding(wxTreeEvent& event)
{
	if(m_selfgroupfunc) m_selfgroupfunc->DoTreeItemExpanding(event);
}
void CSelfGroupSetPanel::OnTreeItemActived(wxTreeEvent& event)
{
	string strGroupName = m_cbxInstrumentGroup->GetValue();
	if( IsSystemGroup( strGroupName ))
	{
		wxMessageBox(LOADSTRING(DEFAULT_GROUP_CANNOT_EDIT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		return;
	}
	if(m_selfgroupfunc) 
	{
		std::string strInstrument=m_selfgroupfunc->GetActivedTreeItem(event);
		AddInstrumentToGroup(strInstrument, strGroupName );
	}
	if(m_treeBase)
		m_treeBase->Toggle(event.GetItem());
}
CSelfGroupSetPanel::~CSelfGroupSetPanel()
{
	SAFE_DELETE(m_selfgroupfunc);
}

void CSelfGroupSetPanel::OnCodeMouseClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_CODE_CLICKED, GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void CSelfGroupSetPanel::OnShowCodeClicked(wxCommandEvent& evt)
{
	if(m_pSvr && m_pSvr->HaveShowInstumentComboList() && m_MouseHandyPopupPane == NULL) {
		std::map<string, set<string>> outData;
		if(m_pSvr->GetProductID(outData) > 0) {
			m_MouseHandyPopupPane = new MouseHandyInputPaneX(this);
			m_MouseHandyPopupPane->Connect(wxEVT_KEY_DOWN, 
					wxKeyEventHandler(CSelfGroupSetPanel::OnMousePanelKeyAscii), NULL, this);
			m_pSvr->GetInstrumentList(m_vecGroupInfo);
			m_MouseHandyPopupPane->SetCodeGroup(&m_vecGroupInfo);
		}
	}
	if(m_MouseHandyPopupPane != NULL) {
		wxPoint pt = m_editInputInstrumentName->GetScreenPosition();
		wxRect rc = m_editInputInstrumentName->GetRect();
		pt.y = pt.y + (m_editInputInstrumentName->GetRect()).GetHeight();
		wxSize size = m_MouseHandyPopupPane->GetSize();
		pt.x = pt.x - size.GetWidth();
		pt.y = pt.y - size.GetHeight();
		m_MouseHandyPopupPane->Position( pt, size );
		m_MouseHandyPopupPane->Popup();
	}
	evt.Skip();
}

void CSelfGroupSetPanel::OnMouseInputCode(wxCommandEvent& evt)
{
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
//	wxString strText = evt.GetString();
	m_bNoKeyChangeText = FALSE;
	wxWindow* window=FindWindowById(ID_EDIT_INPUT_INSTRUMENT_NAME,this);
	window->SetLabel(EvtParamStr);
	m_bNoKeyChangeText = TRUE;
	evt.Skip();
}

void CSelfGroupSetPanel::OnContractNameChanged(wxCommandEvent& event)
{
	wxString strText = event.GetString();
	if ( strText.empty() )
	{
		return;
	}

	if(m_bNoKeyChangeText) 
	{
		if(m_MouseHandyPopupPane)
		{
			wxPoint pt = m_editInputInstrumentName->GetScreenPosition();
			wxRect rc = m_editInputInstrumentName->GetRect();
			pt.y = pt.y + (m_editInputInstrumentName->GetRect()).GetHeight();
			wxSize size = m_MouseHandyPopupPane->GetSize();
			pt.x = pt.x - size.GetWidth();
			pt.y = pt.y - size.GetHeight();
			m_MouseHandyPopupPane->Position( pt, size );
			m_MouseHandyPopupPane->ShowKeyInput(strText);
			wxWindow* window=FindWindowById(ID_EDIT_INPUT_INSTRUMENT_NAME,this);
			if(window) m_MouseHandyPopupPane->Popup(window);
		}
	}
}

void CSelfGroupSetPanel::OnMousePanelKeyAscii(wxKeyEvent& event)
{
	event.Skip();
}
void CSelfGroupSetPanel::OnCellLeftDoubleClick(wxGridEvent& evt)
{
	if(m_listBase)
	{
		string strGroupName = m_cbxInstrumentGroup->GetValue();
		if( IsSystemGroup( strGroupName ))
		{
			wxMessageBox(LOADSTRING(DEFAULT_GROUP_CANNOT_EDIT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}
		int nSelRow = evt.GetRow();
		wxString strInstrumentName = m_listBase->GetValue( nSelRow,0 );
		AddInstrumentToGroup( strInstrumentName.c_str(), strGroupName );
	}

}

void CSelfGroupSetPanel::OnCustomListActivated( wxGridEvent& event )
{
	string strGroupName = m_cbxInstrumentGroup->GetValue();
	if( IsSystemGroup( strGroupName ))
	{
		wxMessageBox(LOADSTRING(DEFAULT_GROUP_CANNOT_EDIT),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		return;
	}

	int nSelRow = event.GetRow();
	m_listCustom->DeleteRows( nSelRow );

	for( unsigned int i = 0; i < g_vecGroupInfo.size(); i++ )
	{
		if ( strGroupName == g_vecGroupInfo[i].GroupName )
		{
			g_vecGroupInfo[i].InstrVec.clear();
			for( int j = 0; j< m_listCustom->GetNumberRows(); j++ )
			{
				string str = m_listCustom->GetCellValue(j, 0);
				g_vecGroupInfo[i].InstrVec.push_back(str);
			}
			break;
		}
	}
}


bool CSelfGroupSetPanel::IsSystemGroup( const std::string& strGroupName )
{
	return false;
}

bool CSelfGroupSetPanel::ExistInCustomList( const std::string& strInstumentName )
{
	bool bFind = false;
	for( int i = 0;i < m_listCustom->GetNumberRows(); i++)
	{
		std::string strTemp = m_listCustom->GetCellValue( i,0 );
		wxString strLeft( strTemp.c_str() );
		wxString strRight( strInstumentName.c_str() );
		if( strLeft.MakeUpper() == strRight.MakeUpper())
		{
			bFind = true;
			break;
		}
	}

	return bFind;
}

bool CSelfGroupSetPanel::AddInstrumentToGroup( const std::string &strInstrumentName, 
									   const std::string& strGroupName )
{
	if(!m_pSvr&&strInstrumentName.empty())
		return true;
	if(m_listCustom->GetNumberRows()>=m_pSvr->GetGroupMaxContractNum())
	{
		wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		return false;
	}
	if ( !ExistInCustomList( strInstrumentName ))
	{
		m_listCustom->AppendRows();
		int nRow = m_listCustom->GetNumberRows();
		m_listCustom->SetCellValue( strInstrumentName, nRow-1, 0 );
		//m_listCustom->SelectRow( nRow );
		for( unsigned int i=0; i<g_vecGroupInfo.size(); i++)
		{
			if( g_vecGroupInfo[i].GroupName == strGroupName )
			{
				g_vecGroupInfo[i].InstrVec.push_back( strInstrumentName.c_str());
				break;
			}
		}
	}
	return true;
}

void CSelfGroupSetPanel::SetCustomListItem( const std::string& strGroupName )
{
	if ( m_listCustom->GetNumberRows() > 0)
	{
		m_listCustom->DeleteRows(0, m_listCustom->GetNumberRows());
	}

	for( unsigned int i=0; i<g_vecGroupInfo.size(); i++ )
	{
		if( strGroupName == g_vecGroupInfo[i].GroupName )
		{
			for( unsigned int j=0; j<g_vecGroupInfo[i].InstrVec.size(); j++ )
			{
				m_listCustom->AppendRows();
				m_listCustom->SetCellValue(g_vecGroupInfo[i].InstrVec[j], m_listCustom->GetNumberRows()-1, 0);
			}
		}
	}
}

std::string CSelfGroupSetPanel::GetValidInstrument( const wxString& strInstrumentName )
{
	
	for ( size_t i = 0; i < m_vecGroupInfo.size(); ++i )
	{
		for( size_t j = 0; j < m_vecGroupInfo[i].InstrVec.size(); ++j )
		{
			std::string strTemp = m_vecGroupInfo[i].InstrVec[j].id;
			if ( strInstrumentName.Upper() == wxString(strTemp.c_str()).Upper())
			{
				return strTemp;
			}
		}
	}

	return wxEmptyString;
}
void CSelfGroupSetPanel::ResetSelfGroupItems()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	wxString strDefaultGroup = pCfgMgr->GetCurrentGroupName();
	if ( m_cbxInstrumentGroup->GetCount() > 0 )
	{
		strDefaultGroup = m_cbxInstrumentGroup->GetString( 0 );
	}

	m_cbxInstrumentGroup->Clear();

	vector<string> vec;
	bool bExistInGroupList = false;
	for( unsigned int i=0; i<g_vecGroupInfo.size(); i++ )
	{
		string strGroupName  = g_vecGroupInfo[i].GroupName;
		vec.push_back( strGroupName );
		if( IsSystemGroup( strGroupName ))
			continue;
		m_cbxInstrumentGroup->AppendString( strGroupName );
		if ( strDefaultGroup == std::string(strGroupName.c_str()))
		{
			bExistInGroupList = true;
		}
	}

	if ( m_cbxInstrumentGroup->GetCount() > 0)
	{
		if ( strDefaultGroup.IsEmpty() )
		{
			strDefaultGroup = m_cbxInstrumentGroup->GetString(0);
			m_cbxInstrumentGroup->SetValue( strDefaultGroup );
			SetCustomListItem( strDefaultGroup.c_str());
		}
		else
		{
			if ( bExistInGroupList )
			{
				m_cbxInstrumentGroup->SetValue( strDefaultGroup );
				SetCustomListItem( strDefaultGroup.c_str() );
			}
			else
			{
				m_listCustom->ClearGrid();
				m_cbxInstrumentGroup->SetValue(wxEmptyString);
			}
		}
	}
}

void CSelfGroupSetPanel::GetCustomSelections( vector<int> &vRows )
{
	wxGridCellCoordsArray tls = m_listCustom->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs = m_listCustom->GetSelectionBlockBottomRight();
	for(int i = 0; i < (int)tls.size(); i++)
	{
		for( int row = tls[i].GetRow(); row <= brs[i].GetRow(); ++row)
		{
			vRows.push_back(row);
		}
	}
}
