#include "stdafx.h"
#include <list>
#include <fstream>

#include "cfgMgr/CfgMgr.h"
#include "wx/textfile.h"
#include "J_ZiXuanHeYueDlg.h"
#include "../inc/gui-common/FrameBase.h"
#include "../inc/Module-Misc/SelfGroupFunc.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(J_ZiXuanHeYueDlg,ConfigPanelBase)

	EVT_BUTTON(ID_BTN_ADD,OnAdd)
	EVT_BUTTON(ID_BTN_DEL,OnDelete)
	EVT_BUTTON(ID_BTN_UP,OnUp)
	EVT_BUTTON(ID_BTN_DOWN,OnDown)
	EVT_BUTTON(ID_BTN_GROUP_SET,OnSet)
	EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturn)
	EVT_BUTTON(ID_SAVE_AND_RETURN,OnSaveAndReturn)
	EVT_BUTTON(ID_BTCancel,OnCancel)
	EVT_COMBOBOX(ID_COM_EXCHANGE, OnExchangeSel) 
	EVT_COMBOBOX(ID_COM_SELF_GROUP, OnSelfGroupSel)

	EVT_SIZE( OnSize )
	EVT_GRID_CMD_CELL_LEFT_DCLICK(ID_LIST_SYS_GROUP,OnBaseListActivated)
	EVT_GRID_CMD_CELL_LEFT_DCLICK(ID_LIST_SELF_GROUP, OnCustomListActivated)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
	EVT_TREE_ITEM_EXPANDING(ID_LIST_SYS_GROUP,OnTreeItemExpanding)
	EVT_TREE_ITEM_ACTIVATED(ID_LIST_SYS_GROUP,OnTreeItemActived)
END_EVENT_TABLE()

void J_ZiXuanHeYueDlg::OnReturn(wxCommandEvent& event)
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

J_ZiXuanHeYueDlg::J_ZiXuanHeYueDlg(wxWindow *parent,
								   wxWindowID winid ,
								   const wxPoint& pos ,
								   const wxSize& size ,
								   long style,
								   const wxString& name)
								   : ConfigPanelBase(parent,winid,pos,size,style,name),
								   m_pComSysGroup(NULL),
								   m_pListSysGroup(NULL),
								   m_pStaticExChange(NULL),
								   m_treeBase(NULL)
{
	ConfigPanelBase::CreateHeader( "IDT_SELFGROUP" );

	m_pBoxSelfGroupSet = new wxStaticBox(this, wxID_ANY, LOADSTRING(CONTEXTMENU_SELECTINSTRUMENT), wxPoint(21, 47), wxSize(591, 464));

	if(DEFAULT_SVR()&&DEFAULT_SVR()->ContractCountIsFit())
	{
		m_pStaticExChange = new wxStaticText(this, wxID_ANY, LOADSTRING(EXCHANGE), wxPoint(44, 64), wxSize(60, 20), 0, wxT(""));
		m_pComSysGroup = new wxOwnerDrawnComboBox(this, ID_COM_EXCHANGE, wxT(""), wxPoint(127, 64), wxSize(100, 20), 0, wxCB_READONLY, wxDefaultValidator, wxT(""));
		m_pListSysGroup= new InstrumentGrid(this, ID_LIST_SYS_GROUP, wxPoint(44, 91), wxSize(172, 412));
	}
	else
	{
		m_treeBase=new wxTreeCtrl(this,ID_LIST_SYS_GROUP,wxPoint(44, 66), wxSize(172, 437),wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_MULTIPLE);
	}
	m_selfgroupfunc=new SelfGroupFunc(m_treeBase);


	m_pBtnAdd = new wxButton(this, ID_BTN_ADD, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(231, 136), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton3"));
	m_pBtnDel = new wxButton(this, ID_BTN_DEL, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(231, 172), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	m_pStaticSelGroup = new wxStaticText(this, wxID_ANY, LOADSTRING(GROUP), wxPoint(286, 66), wxDefaultSize, 0, wxT(""));
	m_pComSelGroup = new wxOwnerDrawnComboBox(this, ID_COM_SELF_GROUP, wxT(""), wxPoint(339, 63), wxSize(107, 20), 0, wxCB_READONLY, wxDefaultValidator, wxT(""));
	m_pListSelfGroup = new wxGrid(this, ID_LIST_SELF_GROUP, wxPoint(339, 91), wxSize(165, 412), wxWANTS_CHARS|wxBORDER_SIMPLE);
	//wxImageList* pImageList = new wxImageList(16, 16, true,2);
	//pImageList->Add( wxIcon(_T("zqSORT_UP") , wxBITMAP_TYPE_ICO_RESOURCE,16,16) );
	//pImageList->Add( wxIcon(_T("zqSORT_DOWN"), wxBITMAP_TYPE_ICO_RESOURCE,16,16) );
	//m_pListSelfGroup->SetImageList(pImageList, wxIMAGE_LIST_SMALL);


	m_pBtnGroupSet = new wxButton(this, ID_BTN_GROUP_SET, LOADSTRING(IDT_GROUPSET), wxPoint(510, 61), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	new wxBitmapButton(this, ID_BTN_UP, wxBitmap("zqBITMAP_UP"), wxPoint(517, 115));
	new wxBitmapButton(this, ID_BTN_DOWN, wxBitmap("zqBITMAP_DOWN"), wxPoint(517, 181));

	m_pListSelfGroup->CreateGrid(0, 1);
	m_pListSelfGroup->SetColLabelValue(0, LOADSTRING(SELF_CONTRACT_CODE));
	m_pListSelfGroup->SetColLabelSize(23);
	m_pListSelfGroup->EnableDragGridSize(false);
	m_pListSelfGroup->SetRowLabelSize( 0 );
	m_pListSelfGroup->EnableDragRowSize(false);
	m_pListSelfGroup->EnableEditing(false);
	m_pListSelfGroup->EnableGridLines(true);
	m_pListSelfGroup->SetScrollLineY(m_pListSelfGroup->GetDefaultRowSize());
	m_pListSelfGroup->SetCellHighlightPenWidth(0);
	m_pListSelfGroup->SetCellHighlightROPenWidth(0);
	m_pListSelfGroup->SetDefaultColSize(150);
	m_pListSelfGroup->SetSelectionMode(wxGrid::wxGridSelectRows);

	m_GroupInfo2.clear();
	m_vGroup.clear();
	ReadCfg();

    m_SysGroupInfo.clear();
    if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(m_SysGroupInfo);
    Init();

	m_bChanged = false;

	this->Connect(wxEVT_LEFT_DOWN,wxMouseEventHandler(J_ZiXuanHeYueDlg::OnKey),NULL,this);

    string str;
    int grpid=0;
    ((FrameBase*)GETTOPWINDOW())->GetCurInstrumentGrpInfo_QuotPanel(str,grpid);

	m_pComSelGroup->SetValue(str);
	string str1 = m_pComSelGroup->GetValue() ;
	for( unsigned int i=0; i<m_GroupInfo2.size(); i++ )
	{
		if( str1 == m_GroupInfo2[i].GroupName )
		{
			for( unsigned int j=0; j<m_GroupInfo2[i].InstrVec.size(); j++ )
			{
				m_pListSelfGroup->AppendRows();
				m_pListSelfGroup->SetCellValue(m_GroupInfo2[i].InstrVec[j], m_pListSelfGroup->GetNumberRows()-1, 0);
			}
		}
	}
}

void J_ZiXuanHeYueDlg::OnKey(wxMouseEvent& evt)
{
	//wxWindow* p  = this->GetTargetWindow();

	//if(p!=List1&& p!=List2)
	//{
	//	BtSet->SetFocus();

	//}
}

void J_ZiXuanHeYueDlg::OnAdd(wxCommandEvent& event)
{
	if(m_pListSysGroup)
	{
		string strInstrumentName = m_pComSelGroup ->GetValue();

		vector<int> rows;
		m_pListSysGroup->GetSelections(rows);
		for(int index=0; index<(int)rows.size(); index++)
		{ 
			string strInstrumentCode=m_pListSysGroup->GetValue(rows[index],0);
			if( !ExistInCustomList(strInstrumentCode))
			{
				m_bChanged = true;
				if(m_pListSelfGroup->GetNumberRows()>=DEFAULT_SVR()->GetGroupMaxContractNum())
				{
					wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
					break;
				}
				else
				{
					m_pListSelfGroup->AppendRows();
					int nRow = m_pListSelfGroup->GetNumberRows();
					m_pListSelfGroup->SetCellValue( strInstrumentCode, nRow-1, 0 );
					string str = m_pComSelGroup->GetValue() ;
					for( unsigned int i=0; i<m_GroupInfo2.size(); i++)
					{
						if(m_GroupInfo2[i].GroupName == str )
						{
							m_GroupInfo2[i].InstrVec.push_back(strInstrumentCode);
						}
					}
				}
			}
		}
	}
	if(m_treeBase)
	{
		vector<string> contracts;
		if(m_selfgroupfunc) m_selfgroupfunc->GetSelections(contracts);
		for (int index=0;index<(int)contracts.size();++index)
		{
			string strInstrumentCode=contracts[index];
			if( !ExistInCustomList(strInstrumentCode))
			{
				m_bChanged = true;
				if(m_pListSelfGroup->GetNumberRows()>=DEFAULT_SVR()->GetGroupMaxContractNum())
				{
					wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
					break;
				}
				else
				{
					m_pListSelfGroup->AppendRows();
					int nRow = m_pListSelfGroup->GetNumberRows();
					m_pListSelfGroup->SetCellValue( strInstrumentCode, nRow-1, 0 );
					string str = m_pComSelGroup->GetValue() ;
					for( unsigned int i=0; i<m_GroupInfo2.size(); i++)
					{
						if(m_GroupInfo2[i].GroupName == str )
						{
							m_GroupInfo2[i].InstrVec.push_back(strInstrumentCode);
						}
					}
				}
			}
		}
	}

}

void J_ZiXuanHeYueDlg::OnDelete(wxCommandEvent& event)
{
	vector<int> vRows;
	GetCustomSelections(vRows);
	if( vRows.empty() )
		return;

	m_bChanged = true;
	m_pListSelfGroup->DeleteRows( vRows[0], vRows.size());

	wxString str = m_pComSelGroup->GetValue() ; 
	for( unsigned int i=0; i<m_GroupInfo2.size(); i++ )
	{
		if ( str == m_GroupInfo2[i].GroupName )
		{
			m_GroupInfo2[i].InstrVec.clear();           
			for( int j=0; j< m_pListSelfGroup->GetNumberRows(); j++ )
			{
				string str = m_pListSelfGroup->GetCellValue(j, 0);
				m_GroupInfo2[i].InstrVec.push_back(str);
			}
			break;
		}
	}
}


void J_ZiXuanHeYueDlg::OnUp(wxCommandEvent& event)
{
	int index = 0;;
	string strGroupName = m_pComSelGroup ->GetValue();
	for( unsigned int i=0; i< m_GroupInfo2.size(); i++)
	{
		if( strGroupName == m_GroupInfo2[i].GroupName )
		{
			index = i;
			break;
		}
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

		m_bChanged = true;
		bSelectChange = true;
		string str = m_GroupInfo2[index].InstrVec[nRow-1];
		m_GroupInfo2[index].InstrVec[nRow-1] = m_GroupInfo2[index].InstrVec[nRow];
		m_GroupInfo2[index].InstrVec[nRow] = str;

		m_pListSelfGroup->SetCellValue(nRow-1, 0, m_GroupInfo2[index].InstrVec[nRow-1]);
		m_pListSelfGroup->SetCellValue(nRow, 0, m_GroupInfo2[index].InstrVec[nRow]);
	}

	if ( bSelectChange )
	{
		m_pListSelfGroup->SelectBlock(vRows[0]-1,0,vRows[vRows.size()-1]-1,m_pListSelfGroup->GetNumberCols()-1);
	}

}
void J_ZiXuanHeYueDlg::OnDown(wxCommandEvent& event)
{
	int index = 0;;
	string strGroupName = m_pComSelGroup ->GetValue();
	for( unsigned int i=0; i< m_GroupInfo2.size(); i++)
	{
		if( strGroupName == m_GroupInfo2[i].GroupName )
		{
			index = i;
			break;
		}
	}

	vector<int> vRows;
	GetCustomSelections(vRows);
	if( vRows.empty() )
		return;

	bool bSelectChange = false;
	for ( int i = vRows.size()-1 ; i >= 0; i--)
	{
		int nRow = vRows[i];
		if ( nRow == m_pListSelfGroup->GetNumberRows()-1 )
		{
			return;
		}

		m_bChanged = true;
		bSelectChange = true;
		string str = m_GroupInfo2[index].InstrVec[nRow+1];
		m_GroupInfo2[index].InstrVec[nRow+1] = m_GroupInfo2[index].InstrVec[nRow];
		m_GroupInfo2[index].InstrVec[nRow] = str;

		m_pListSelfGroup->SetCellValue(nRow+1, 0, m_GroupInfo2[index].InstrVec[nRow+1]);
		m_pListSelfGroup->SetCellValue(nRow, 0, m_GroupInfo2[index].InstrVec[nRow]);
		m_pListSelfGroup->SelectBlock(nRow+1,0,vRows[vRows.size()-1]+1,m_pListSelfGroup->GetNumberCols()-1);
	}

	if ( bSelectChange )
	{
		m_pListSelfGroup->SelectBlock(vRows[0]+1,0,vRows[vRows.size()-1]+1,m_pListSelfGroup->GetNumberCols()-1);
	}
}

void J_ZiXuanHeYueDlg::OnSet(wxCommandEvent& event)
{
	GroupSetDlg groupSetDlg(this);
	groupSetDlg.InitCtrols( m_vGroup );
	if( wxID_OK!=groupSetDlg.ShowModal() )
	{
		return;
	}

	m_bChanged = true;
	string value = m_pComSelGroup->GetValue();
	m_pComSelGroup->Clear();

	GroupInfo2  group;
	group.GroupId = 0;
	vector<GroupInfo2> temp;
	for( int i=0; i< (int)m_vGroup.size(); i++)
	{
		group.GroupName = m_vGroup[i].GroupName;
		group.GroupId  = m_vGroup[i].GroupId;
		temp.push_back(group);

    	m_pComSelGroup->AppendString(group.GroupName);
	}

	for( unsigned int i=0; i< temp.size();i++)
	{
		string name = temp[i].GroupName;
		for( int j=0; j< (int)m_GroupInfo2.size(); j++)
		{
			group.GroupName = m_GroupInfo2[j].GroupName;
			if( name ==group.GroupName)
			{
				temp[i].InstrVec = m_GroupInfo2[j].InstrVec;
				break;
			}
		}

	}

	m_GroupInfo2.clear();
	m_GroupInfo2 = temp;

	m_pComSelGroup->SetSelection(-1);
	if ( m_pListSelfGroup->GetNumberRows() > 0)
	{
		m_pListSelfGroup->DeleteRows(0, m_pListSelfGroup->GetNumberRows());
	}
}


void J_ZiXuanHeYueDlg::OnSize( wxSizeEvent& event )
{
	wxRect rt = this->GetRect();
	m_pBoxSelfGroupSet->SetSize( 591,rt.height-80);
	// Box = new wxStaticBox(this, wxID_ANY, wxT(""), wxPoint(46, 35+offset), wxSize(591, 464));
	if(m_pListSysGroup)
		m_pListSysGroup->SetSize( 172,rt.height-130);
	if(m_treeBase)
		m_treeBase->SetSize( 172,rt.height-105);
	m_pListSelfGroup->SetSize( 165,rt.height-130);
} 

void J_ZiXuanHeYueDlg::OnExchangeSel(wxCommandEvent& event)
{
	if(m_pComSysGroup&&m_pListSysGroup)
	{
		int sel=m_pComSysGroup->GetSelection();
		m_pListSysGroup->SetTable(sel);
		m_pListSysGroup->Refresh();
	}
}
void J_ZiXuanHeYueDlg::OnTreeItemExpanding(wxTreeEvent& event)
{
	if(m_selfgroupfunc) m_selfgroupfunc->DoTreeItemExpanding(event);
}
void J_ZiXuanHeYueDlg::OnSelfGroupSel(wxCommandEvent& event)
{
	if ( m_pListSelfGroup->GetNumberRows() > 0)
	{
		m_pListSelfGroup->DeleteRows(0, m_pListSelfGroup->GetNumberRows());
	}

	wxString str = m_pComSelGroup->GetValue() ;
	for( unsigned int i=0; i<m_GroupInfo2.size(); i++ )
	{
		if( str == m_GroupInfo2[i].GroupName )
		{
			for( unsigned int j=0; j< m_GroupInfo2[i].InstrVec.size(); j++ )
			{
				m_pListSelfGroup->AppendRows();
				m_pListSelfGroup->SetCellValue(m_GroupInfo2[i].InstrVec[j], m_pListSelfGroup->GetNumberRows()-1, 0);
			}
		}
	}
}
void J_ZiXuanHeYueDlg::OnTreeItemActived(wxTreeEvent& event)
{
	string strGroupName = m_pComSelGroup->GetValue();
    if(!m_selfgroupfunc) return;
	wxString strInstrumentName =m_selfgroupfunc->GetActivedTreeItem(event);
	if ( !ExistInCustomList( strInstrumentName.c_str() ))
	{
		int nRow = m_pListSelfGroup->GetNumberRows();
		if( nRow >=DEFAULT_SVR()->GetGroupMaxContractNum())
		{
			wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}

		m_pListSelfGroup->AppendRows();
		m_pListSelfGroup->SetCellValue(strInstrumentName, m_pListSelfGroup->GetNumberRows()-1, 0);
		m_bChanged = true;
		for( unsigned int i=0; i<m_GroupInfo2.size(); i++)
		{
			if( m_GroupInfo2[i].GroupName == strGroupName )
			{
				m_GroupInfo2[i].InstrVec.push_back( strInstrumentName.c_str());
				break;
			}
		}
	}
	if(m_treeBase)
		m_treeBase->Toggle(event.GetItem());
}
void J_ZiXuanHeYueDlg::Init()
{
	if(m_pComSysGroup&&m_pListSysGroup)
	{
		std::vector<std::string> gridheaders;
		gridheaders.push_back(LOADSTRING(CONTRACT_CODE));
		if(DEFAULT_SVR()&&!DEFAULT_SVR()->ContractNameIsID())   
			gridheaders.push_back(LOADSTRING(CONTRACT_NAME));
		if( m_SysGroupInfo.size() != 0 )
		{
			for( unsigned int i=0; i<m_SysGroupInfo.size() ; i++ )
			{		
				m_pComSysGroup->AppendString(m_SysGroupInfo[i].GroupName);
				m_pListSysGroup->AddInstrumentGridTable(m_SysGroupInfo[i].InstrVec,gridheaders);
			}
			m_pComSysGroup->SetSelection( 0 );
			m_pListSysGroup->SetTable(0);
		}
	}
	if(m_selfgroupfunc) m_selfgroupfunc->AddTreeData();

	if( m_GroupInfo2.size() == 0) return;
	for( unsigned int i=0; i<m_GroupInfo2.size(); i++ )
	{
		m_pComSelGroup->AppendString( m_GroupInfo2[i].GroupName );
	}
}


void  J_ZiXuanHeYueDlg::ReadCfg()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if( !pCfgMgr )
		return;

	int count = pCfgMgr->GetInstrumentGroupCount();
	for( int i =0 ;i<count; i++ )
	{
		GroupInfo2 GroupInfo;
		GroupInfo.GroupId=0;
		LPINSTRUMENT_GROUP_CFG info = pCfgMgr->GetInstrumentGroupInfo(i);
		GroupInfo.GroupName = info->GroupName;
		GroupInfo.GroupId = info->GroupId;
		GroupInfo.InstrVec = *(pCfgMgr->GetGroupMember(i));

		m_GroupInfo2.push_back(GroupInfo); 
		m_vGroup.push_back(*info);
	}
}

void  J_ZiXuanHeYueDlg::WriteCfg()
{
	string sss = m_pComSelGroup->GetValue();
	DWORD  maxid = 0;;

	for( unsigned int i=0; i<m_GroupInfo2.size(); i++) 
	{
		maxid < m_GroupInfo2[i].GroupId ? maxid=m_GroupInfo2[i].GroupId:maxid=maxid;
	}

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if(!pCfgMgr)
		return;

	pCfgMgr->ClearInstrumentGroupVector();

	for( unsigned int i=0; i<m_GroupInfo2.size(); i++) 
	{
		string str = m_GroupInfo2[i].GroupName;
		bool BChecked = m_vGroup[i].IsVisibility;
		if( m_GroupInfo2[i].GroupId!=0)
		{
			pCfgMgr->AddInstrumentGroup(str,BChecked,m_GroupInfo2[i].GroupId);
			if(sss == m_GroupInfo2[i].GroupName)
				pCfgMgr->SetCurrentGruopId( m_GroupInfo2[i].GroupId );
		}

		else
		{
			pCfgMgr->AddInstrumentGroup(str,BChecked,++maxid);
			if( sss ==  m_GroupInfo2[i].GroupName)
				pCfgMgr->SetCurrentGruopId( m_GroupInfo2[i].GroupId );
		}
	}


	for( unsigned int i=0; i<m_GroupInfo2.size(); ++i )
	{
		for( unsigned int j =0; j<m_GroupInfo2[i].InstrVec.size(); j++ )
		{
			pCfgMgr->GetGroupMember(i)->push_back(m_GroupInfo2[i].InstrVec[j]);
		}
	}
}


void J_ZiXuanHeYueDlg::OnSaveAndReturn(wxCommandEvent& event)
{
	m_bChanged = false;
	WriteCfg();

	CLOSE_FLOAT_PANEL();
}

void J_ZiXuanHeYueDlg::OnCancel(wxCommandEvent& event)
{
	//this->EndDialog(wxID_OK);
}

J_ZiXuanHeYueDlg:: ~J_ZiXuanHeYueDlg()
{
	Close();
	SAFE_DELETE(m_selfgroupfunc);
}

int wxCALLBACK Compare1Function(wxExtListItem item1,wxExtListItem item2,long sortData )
{
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

void J_ZiXuanHeYueDlg::OnBaseListActivated(wxGridEvent& evt)
{
	string strGroupName = m_pComSelGroup->GetValue();

	int nSelRow = evt.GetRow();
	wxString strInstrumentName = m_pListSysGroup->GetValue( nSelRow,0 );
	if ( !ExistInCustomList( strInstrumentName.c_str() ))
	{
		int nRow = m_pListSelfGroup->GetNumberRows();
		if( nRow >=DEFAULT_SVR()->GetGroupMaxContractNum())
		{
			wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			return;
		}

		m_pListSelfGroup->AppendRows();
		m_pListSelfGroup->SetCellValue(strInstrumentName, m_pListSelfGroup->GetNumberRows()-1, 0);
		m_bChanged = true;
		for( unsigned int i=0; i<m_GroupInfo2.size(); i++)
		{
			if( m_GroupInfo2[i].GroupName == strGroupName )
			{
				m_GroupInfo2[i].InstrVec.push_back( strInstrumentName.c_str());
				break;
			}
		}
	}
}

void J_ZiXuanHeYueDlg::OnCustomListActivated( wxGridEvent& event )
{
	int nSelRow = event.GetRow();
	m_pListSelfGroup->DeleteRows( nSelRow );
	m_bChanged = true;

	string strGroupName = m_pComSelGroup->GetValue();
	for( unsigned int i = 0; i < m_GroupInfo2.size(); i++ )
	{
		if ( strGroupName == m_GroupInfo2[i].GroupName )
		{
			m_GroupInfo2[i].InstrVec.clear();
			for( int j = 0; j< m_pListSelfGroup->GetNumberRows(); j++ )
			{
				string str = m_pListSelfGroup->GetCellValue(j, 0);
				m_GroupInfo2[i].InstrVec.push_back(str);
			}
			break;
		}
	}
}

bool J_ZiXuanHeYueDlg::ExistInCustomList( const std::string& strInstumentName )
{
	bool bFind = false;
	for( int i = 0;i < m_pListSelfGroup->GetNumberRows(); i++)
	{
		std::string strTemp = m_pListSelfGroup->GetCellValue( i,0 );
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
void J_ZiXuanHeYueDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pBoxSelfGroupSet->SetLabel(LOADSTRING(CONTEXTMENU_SELECTINSTRUMENT));
	if(m_pStaticExChange)
		m_pStaticExChange->SetLabel(LOADSTRING(EXCHANGE));
	//m_pListSysGroup->SetColumn(0, LOADSTRING(CONTRACT_CODE));
	//m_pListSysGroup->SetColumn(1, LOADSTRING(CONTRACT_NAME));

	m_pStaticSelGroup->SetLabel(LOADSTRING(GROUP));
	m_pListSelfGroup->SetColLabelValue(0, LOADSTRING(SELF_CONTRACT_CODE));

	m_pBtnAdd->SetLabel(LOADSTRING(ASKNAMEDLG_ADD));
	m_pBtnDel->SetLabel(LOADSTRING(ASKNAMEDLG_DEL));
	m_pBtnGroupSet->SetLabel(LOADSTRING(IDT_GROUPSET));
}

void J_ZiXuanHeYueDlg::SetGroupVec( vector<INSTRUMENT_GROUP_CFG>& vGroup )
{
	m_vGroup = vGroup;
}

void J_ZiXuanHeYueDlg::GetCustomSelections( vector<int> &vRows )
{
	wxGridCellCoordsArray tls = m_pListSelfGroup->GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs = m_pListSelfGroup->GetSelectionBlockBottomRight();
	for(int i = 0; i < (int)tls.size(); i++)
	{
		for( int row = tls[i].GetRow(); row <= brs[i].GetRow(); ++row)
		{
			vRows.push_back(row);
		}
	}
}
