#include "StdAfx.h"
#include "../inc/Module-Misc/constants.h"
#include "Module-Misc/globalDefines.h"
#include "CSelfGroupSetPanel.h"
#include "CVarietyInfoSetPanel.h"
#include "CGroupSetPanel.h"
#include "CTableSetPanel.h"
#include "CGeneralSetPanel.h"
#include "CAutoOpenCloseSetPanel.h"
#include "CKeyboardOpenOrderSetPanel.h"
#include "CMouseOpenOrderSetPanel.h"
#include "CAdvancedSetPanel.h"
#include "CShowSetPanel.h"
#include "CFontAndColorSetPanel.h"
#include "CGeneralCancelOrderSetPanel.h"
#include "CRiskManagerSetPanel.h"
#include "CPromptSetPanel.h"
#include "CUserGroupSetPanel.h"
#include "CQuickOrderSetPanel.h"
#include "COrderStrategyPanel.h"
#include "COrderAccountTypePanel.h"

#include "ConfigAppDlg.h"
#include "../inc/gui/EventParam.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

vector<GroupInfo2X> g_vecGroupInfo;
const wxPoint PANEL_POS(172,8);
const wxSize PANEL_SIZE(500,337);

CConfigAppDlg::CConfigAppDlg(wxWindow *parent, const int defaultItemId,IPlatformSingleSvr* pSvr):
				wxDialog(parent, wxID_ANY, LOADSTRING(CONFIG_TITLE),
					wxDefaultPosition, wxSize(700,450)),m_pSvr(pSvr)
{
	DIALOG_CREATE_LOG();
	m_treeCtrl = new wxTreeCtrl( this, ID_TREE, wxPoint(8,6), wxSize(158, 351), 
		wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE|wxTR_HAS_BUTTONS);
	new wxStaticLine( this, wxID_STATIC, wxPoint(167,351), wxSize(492,1), wxLI_HORIZONTAL );
	new wxButton( this, wxID_OK, LOADSTRING(ASKNAMEDLG_OK),wxPoint(478,378), wxSize(75,25) );
	new wxButton( this, wxID_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL),wxPoint(581,378), wxSize(75,25) );

	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if( pMgr != NULL)
	{
		pMgr->CalcUsableShortCutKeys();
	}

	m_panelMap.clear();
	m_treeItemMap.clear();
	InitializeTree();
	if (defaultItemId>=0)
	{
		SetDefaultShowPanel( defaultItemId );
	}
}

CConfigAppDlg::~CConfigAppDlg(void)
{
	g_vecGroupInfo.clear();
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();
	if ( NULL != pMgr )
	{
		pMgr->CalcUsableShortCutKeys();
	}
}

BEGIN_EVENT_TABLE(CConfigAppDlg, wxDialog)
	EVT_TREE_SEL_CHANGED(ID_TREE, OnSelChanged)
	EVT_COMMAND( wxID_ANY, wxEVT_SHOW_PANEL, OnShowPanel)
	EVT_COMMAND( wxID_ANY, wxEVT_GROUP_CHANGE, OnGroupChange)
	EVT_BUTTON(wxID_OK, OnOK)
	EVT_BUTTON(wxID_CANCEL, OnCancel)
END_EVENT_TABLE()

void CConfigAppDlg::InitializeTree()
{
	m_treeCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

	wxTreeItemId rootId = m_treeCtrl->AddRoot(wxEmptyString);  
	wxTreeItemId instrumentAndGroupId = m_treeCtrl->AppendItem( rootId, LOADSTRING(IDT_INSTRUMENTGROUP),-1,-1,new wxTreeItemIntData(ID_ITEM_INSTRUMENTGROUP));
	wxTreeItemId selfGroupId = m_treeCtrl->AppendItem( instrumentAndGroupId, LOADSTRING(IDT_SELFGROUP),-1,-1,new wxTreeItemIntData(ID_ITEM_SELFGROUP));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType() != PTYPE_PATSAPI)
	{
		wxTreeItemId varietyInfoSetId = m_treeCtrl->AppendItem( instrumentAndGroupId, LOADSTRING(IDT_VARIETYINFOSET),-1,-1,new wxTreeItemIntData(ID_ITEM_VARIETYINFOSET));
		m_panelMap[ID_ITEM_VARIETYINFOSET] = NULL;
		m_treeItemMap[ID_ITEM_VARIETYINFOSET] = varietyInfoSetId;
	}
	
	wxTreeItemId groupSetId = m_treeCtrl->AppendItem( selfGroupId, LOADSTRING(IDT_GROUPSET),-1,-1,new wxTreeItemIntData(ID_ITEM_GROUPSET)); 
	m_panelMap[ID_ITEM_SELFGROUP] = NULL;	
	m_panelMap[ID_ITEM_GROUPSET] = NULL;
	m_treeItemMap[ID_ITEM_SELFGROUP] = selfGroupId;
	m_treeItemMap[ID_ITEM_GROUPSET] = groupSetId;

	wxTreeItemId userGroupId = m_treeCtrl->AppendItem( rootId, "用户组设置",-1,-1,new wxTreeItemIntData(ID_ITEM_USERGROUP ) );
	m_panelMap[ID_ITEM_USERGROUP] = NULL;
	m_treeItemMap[ID_ITEM_USERGROUP] = userGroupId;

	wxTreeItemId showId = m_treeCtrl->AppendItem( rootId, LOADSTRING(IDT_SHOW),-1,-1,new wxTreeItemIntData(ID_ITEM_SHOW ));
	wxTreeItemId fontAndColorId = m_treeCtrl->AppendItem( showId, LOADSTRING(IDT_FONTANDCOLOR) ,-1,-1,new wxTreeItemIntData(ID_ITEM_FONTANDCOLOR ));
	m_panelMap[ID_ITEM_SHOW] = NULL;
	m_panelMap[ID_ITEM_FONTANDCOLOR] = NULL;
	m_treeItemMap[ID_ITEM_SHOW] = showId;
	m_treeItemMap[ID_ITEM_FONTANDCOLOR] = fontAndColorId;

	wxTreeItemId openOrderId = m_treeCtrl->AppendItem( rootId, LOADSTRING(IDT_OPENORDER) ,-1,-1,new wxTreeItemIntData(ID_ITEM_OPENORDER ));
	wxTreeItemId generalSetId = m_treeCtrl->AppendItem( openOrderId, LOADSTRING(IDT_GENERALSET),-1,-1,new wxTreeItemIntData(ID_ITEM_GENERALSET ) );
	//if ( DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType() != PTYPE_PATSAPI )
	//{
	//	wxTreeItemId autoOpenCloseSetId = m_treeCtrl->AppendItem( openOrderId, LOADSTRING(IDT_AUTOOPENCLOSESET),-1,-1,new wxTreeItemIntData(ID_ITEM_AUTOOPENCLOSESET ) );
	//	m_panelMap[ID_ITEM_AUTOOPENCLOSESET] = NULL;
	//	m_treeItemMap[ID_ITEM_AUTOOPENCLOSESET] = autoOpenCloseSetId;
	//}
	
	wxTreeItemId quickOrderSetId = m_treeCtrl->AppendItem( openOrderId, "快捷下单选项",-1,-1,new wxTreeItemIntData(ID_ITEM_QUICKORDERSET ) );
	m_panelMap[ID_ITEM_QUICKORDERSET] = NULL;
	m_treeItemMap[ID_ITEM_QUICKORDERSET] = quickOrderSetId;

	wxTreeItemId keyboardOpenOrderId = m_treeCtrl->AppendItem( openOrderId, LOADSTRING(IDT_KEYBOARDOPENODER) ,-1,-1,new wxTreeItemIntData(ID_ITEM_KEYBOARDOPENODER ));
	wxTreeItemId mouseOpenOrderId = m_treeCtrl->AppendItem( openOrderId, LOADSTRING(IDT_MOUSEOPENORDER),-1,-1,new wxTreeItemIntData(ID_ITEM_MOUSEOPENORDER ) );
	wxTreeItemId advancedSetId = m_treeCtrl->AppendItem( openOrderId, LOADSTRING(IDT_ADVANCEDSET),-1,-1,new wxTreeItemIntData(ID_ITEM_ADVANCEDSET ) );
	m_panelMap[ID_ITEM_GENERALSET] = NULL;
	m_panelMap[ID_ITEM_KEYBOARDOPENODER] = NULL;
	m_panelMap[ID_ITEM_MOUSEOPENORDER] = NULL;
	m_panelMap[ID_ITEM_ADVANCEDSET] = NULL;
	m_treeItemMap[ID_ITEM_GENERALSET] = generalSetId;
	m_treeItemMap[ID_ITEM_KEYBOARDOPENODER] = keyboardOpenOrderId;
	m_treeItemMap[ID_ITEM_MOUSEOPENORDER] = mouseOpenOrderId;
	m_treeItemMap[ID_ITEM_ADVANCEDSET] = advancedSetId;

	wxTreeItemId orderStrategyId = m_treeCtrl->AppendItem( rootId, "下单策略",-1,-1,new wxTreeItemIntData(ID_ITEM_ORDERSTRATEGY ) );
	m_panelMap[ID_ITEM_ORDERSTRATEGY] = NULL;
	m_treeItemMap[ID_ITEM_ORDERSTRATEGY] = orderStrategyId;

	wxTreeItemId basedMultipeId = m_treeCtrl->AppendItem( orderStrategyId, "基数乘数方式",-1,-1,new wxTreeItemIntData(ID_ITEM_BASEDMULTIPE ) );
	m_panelMap[ID_ITEM_BASEDMULTIPE] = NULL;
	m_treeItemMap[ID_ITEM_BASEDMULTIPE] = basedMultipeId;

	wxTreeItemId customVolumnId = m_treeCtrl->AppendItem( orderStrategyId, "策略板自定义手数方式",-1,-1,new wxTreeItemIntData(ID_ITEM_CUSTOMVOLUMN ) );
	m_panelMap[ID_ITEM_CUSTOMVOLUMN] = NULL;
	m_treeItemMap[ID_ITEM_CUSTOMVOLUMN] = customVolumnId;

	wxTreeItemId distributeBySumId = m_treeCtrl->AppendItem( orderStrategyId, "总数分配方式",-1,-1,new wxTreeItemIntData(ID_ITEM_DISTRIBUTEBYSUM ) );
	m_panelMap[ID_ITEM_DISTRIBUTEBYSUM] = NULL;
	m_treeItemMap[ID_ITEM_DISTRIBUTEBYSUM] = distributeBySumId;

	wxTreeItemId alignToFirstAccountId = m_treeCtrl->AppendItem( orderStrategyId, "按资金向首账户对齐",-1,-1,new wxTreeItemIntData(ID_ITEM_ALIGNTOFISRTACCOUNT ) );
	m_panelMap[ID_ITEM_ALIGNTOFISRTACCOUNT] = NULL;
	m_treeItemMap[ID_ITEM_ALIGNTOFISRTACCOUNT] = alignToFirstAccountId;

	wxTreeItemId accountScaleDividedId = m_treeCtrl->AppendItem( orderStrategyId, "按账户规模分配手数",-1,-1,new wxTreeItemIntData(ID_ITEM_ACCOUNTSCALEDIVEDED ) );
	m_panelMap[ID_ITEM_ACCOUNTSCALEDIVEDED] = NULL;
	m_treeItemMap[ID_ITEM_ACCOUNTSCALEDIVEDED] = accountScaleDividedId;

	wxTreeItemId generalCancelOrderId = m_treeCtrl->AppendItem( rootId, LOADSTRING(IDT_GENERALCANCELORDER),-1,-1,new wxTreeItemIntData(ID_ITEM_GENERALCANCELORDER ) );
	wxTreeItemId riskManagerId = m_treeCtrl->AppendItem( rootId, LOADSTRING(IDT_RISKMANAGER),-1,-1,new wxTreeItemIntData(ID_ITEM_RISKMANAGER ) );
	wxTreeItemId promptId = m_treeCtrl->AppendItem( rootId, LOADSTRING(IDT_PROMPT),-1,-1,new wxTreeItemIntData(ID_ITEM_PROMPT ) );
	m_panelMap[ID_ITEM_GENERALCANCELORDER] = NULL;
	m_panelMap[ID_ITEM_RISKMANAGER] = NULL;
	m_panelMap[ID_ITEM_PROMPT] = NULL;
	m_treeItemMap[ID_ITEM_GENERALCANCELORDER] = generalCancelOrderId;
	m_treeItemMap[ID_ITEM_RISKMANAGER] = riskManagerId;
	m_treeItemMap[ID_ITEM_PROMPT] = promptId;

	wxTreeItemId tableSetId = m_treeCtrl->AppendItem( rootId,	 LOADSTRING(IDT_TABLESET),-1,-1,new wxTreeItemIntData(ID_ITEM_TABLESET ));
	wxTreeItemId quoteTableId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_QUOTETABLE)	,-1,-1,new wxTreeItemIntData(ID_ITEM_QUOTETABLE ));
	wxTreeItemId tradeRecordId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_TRADERECORD),-1,-1,new wxTreeItemIntData(ID_ITEM_TRADERECORD ));
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_THRADE_STATISTICS ))
	{
		wxTreeItemId tradeStatisticId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_TRADESTATISTIC),-1,-1,new wxTreeItemIntData(ID_ITEM_TRADESTATISTIC ));
		m_panelMap[ID_ITEM_TRADESTATISTIC] = NULL;
		m_treeItemMap[ID_ITEM_TRADESTATISTIC] = tradeStatisticId;
	}
	
	wxTreeItemId positionId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_POSITION),-1,-1,new wxTreeItemIntData(ID_ITEM_POSITION ));
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_POSITIONCOMB))
	{
		wxTreeItemId comboPositionId = m_treeCtrl->AppendItem( tableSetId, LOADSTRING(IDT_COMBOPOSITION),-1,-1,new wxTreeItemIntData(ID_ITEM_COMBO_POSITION ));
		m_panelMap[ID_ITEM_COMBO_POSITION] = NULL;
		m_treeItemMap[ID_ITEM_COMBO_POSITION] = comboPositionId;
	}
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_POSITIONDETAIL))
	{
		wxTreeItemId positionDetaildId = m_treeCtrl->AppendItem( tableSetId, LOADSTRING(IDT_POSITIONDETAIL),-1,-1,new wxTreeItemIntData(ID_ITEM_POSITIONDETAIL ));
		m_panelMap[ID_ITEM_POSITIONDETAIL] = NULL;
		m_treeItemMap[ID_ITEM_POSITIONDETAIL] = positionDetaildId;
	}
	
	wxTreeItemId instrumentListId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_INSTRUMENTLIST),-1,-1,new wxTreeItemIntData(ID_ITEM_INSTRUMENTLIST ));
	wxTreeItemId allOrderId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_ALLORDER),-1,-1,new wxTreeItemIntData(ID_ITEM_ALLORDER ));
	wxTreeItemId waitTradeId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_WAITTRADE),-1,-1,new wxTreeItemIntData(ID_ITEM_WAITTRADE ));
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_PARKED ))
	{
		wxTreeItemId parkedOrderId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_PARKEDORDER),-1,-1,new wxTreeItemIntData(ID_ITEM_PARKEDORDER ));
		m_panelMap[ID_ITEM_PARKEDORDER] = NULL;
		m_treeItemMap[ID_ITEM_PARKEDORDER] = parkedOrderId;
	}
	
	wxTreeItemId accountId = m_treeCtrl->AppendItem( tableSetId,	 LOADSTRING(IDT_ACCOUNT),-1,-1,new wxTreeItemIntData(ID_ITEM_ACCOUNT ));
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_BATCH ))
	{
		wxTreeItemId queryOrderBatchId = m_treeCtrl->AppendItem( tableSetId,	LOADSTRING(IDT_QUERYORDERBATCH),-1,-1,new wxTreeItemIntData(ID_ITEM_QUERYORDERBATCH ));
		m_panelMap[ID_ITEM_QUERYORDERBATCH] = NULL;
		m_treeItemMap[ID_ITEM_QUERYORDERBATCH] = queryOrderBatchId;
	}
	
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_CONDITION ))
	{
		wxTreeItemId queryOrderConditionId = m_treeCtrl->AppendItem( tableSetId,	LOADSTRING(IDT_QUERYORDERCONDITION),-1,-1,new wxTreeItemIntData(ID_ITEM_QUERYORDERCONDITION ));
		m_panelMap[ID_ITEM_QUERYORDERCONDITION] = NULL;
		m_treeItemMap[ID_ITEM_QUERYORDERCONDITION] = queryOrderConditionId;
	}
	
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_CONDITION ))
	{
		wxTreeItemId queryOrderPositionId = m_treeCtrl->AppendItem( tableSetId,	LOADSTRING(IDT_QUERYORDERPOSITION),-1,-1,new wxTreeItemIntData(ID_ITEM_QUERYORDERPOSITION ));
		m_panelMap[ID_ITEM_QUERYORDERPOSITION] = NULL;
		m_treeItemMap[ID_ITEM_QUERYORDERPOSITION] = queryOrderPositionId;
	}

	wxTreeItemId multiAccountId = m_treeCtrl->AppendItem( tableSetId, LOADSTRING(GID_MULTI_ACCOUNT_CAPTION),-1,-1,new wxTreeItemIntData(ID_ITEM_MULTIACCOUNT ));
	m_panelMap[ID_ITEM_MULTIACCOUNT] = NULL;
	m_treeItemMap[ID_ITEM_MULTIACCOUNT] = multiAccountId;

	wxTreeItemId multiPositionId = m_treeCtrl->AppendItem( tableSetId, LOADSTRING(GID_MULTI_POSITION_CAPTION),-1,-1,new wxTreeItemIntData(ID_ITEM_MULTIPOSITION ));
	m_panelMap[ID_ITEM_MULTIPOSITION] = NULL;
	m_treeItemMap[ID_ITEM_MULTIPOSITION] = multiPositionId;

	//if ( DEFAULT_SVR() && DEFAULT_SVR()->GetPlatformType() != PTYPE_CTP)
	//{
	//	wxTreeItemId orderInsertId = m_treeCtrl->AppendItem( tableSetId, LOADSTRING(IDT_ORDERINSERT),-1,-1,new wxTreeItemIntData(ID_ITEM_ORDER_INSERT ));
	//	m_panelMap[ID_ITEM_ORDER_INSERT] = NULL;
	//	m_treeItemMap[ID_ITEM_ORDER_INSERT] = orderInsertId;
	//}
	
	m_panelMap[ID_ITEM_QUOTETABLE] = NULL;
	m_panelMap[ID_ITEM_TRADERECORD] = NULL;
	m_panelMap[ID_ITEM_POSITION] = NULL;	
	m_panelMap[ID_ITEM_INSTRUMENTLIST] = NULL;
	m_panelMap[ID_ITEM_ALLORDER] = NULL;
	m_panelMap[ID_ITEM_WAITTRADE] = NULL;
	m_panelMap[ID_ITEM_ACCOUNT] = NULL;
	
	m_treeItemMap[ID_ITEM_QUOTETABLE] = quoteTableId;
	m_treeItemMap[ID_ITEM_TRADERECORD] = tradeRecordId;
	m_treeItemMap[ID_ITEM_POSITION] = positionId;
	m_treeItemMap[ID_ITEM_INSTRUMENTLIST] = instrumentListId;
	m_treeItemMap[ID_ITEM_ALLORDER] = allOrderId;
	m_treeItemMap[ID_ITEM_WAITTRADE] = waitTradeId;
	m_treeItemMap[ID_ITEM_ACCOUNT] = accountId;
}

void CConfigAppDlg::SwitchShow( wxPanel* pPanel )
{
	if( pPanel != NULL)
	{
		std::map<int, wxPanel*>::iterator it = m_panelMap.begin();
		for ( ; it != m_panelMap.end(); ++it )
		{
			if ( NULL == it->second )
			{
				continue;
			}

			if ( it->second == pPanel )
			{
				::SendMessage((HWND)(it->second)->GetHandle(),WM_SHOWWINDOW,(WPARAM)(TRUE),0);
				(it->second)->Show();
			}
			else
			{
				::SendMessage((HWND)(it->second)->GetHandle(),WM_SHOWWINDOW,(WPARAM)(FALSE),0);
				(it->second)->Show( false );
			}
		}
	}
}

void CConfigAppDlg::OnSelChanged( wxTreeEvent& event )
{   
	wxTreeItemId itemId = event.GetItem();
	if ( itemId.IsOk() )
	{
		wxTreeItemIntData* pItemData = (wxTreeItemIntData*)m_treeCtrl->GetItemData( itemId );
		int strText=pItemData->GetData();
		if( strText == ID_ITEM_INSTRUMENTGROUP )
		{
			wxTreeItemIdValue chockes;
			wxTreeItemId childItemId = m_treeCtrl->GetFirstChild( itemId, chockes );
			m_treeCtrl->SelectItem( childItemId );
		}
		else if( strText == ID_ITEM_SELFGROUP )
		{
			if( m_panelMap[ID_ITEM_SELFGROUP] == NULL)
			{
				m_panelMap[ID_ITEM_SELFGROUP] = new CSelfGroupSetPanel( m_pSvr,this, wxID_ANY,PANEL_POS,PANEL_SIZE);
			}
			SwitchShow( m_panelMap[ID_ITEM_SELFGROUP] );
		}
		else if( strText == ID_ITEM_GROUPSET )
		{
			if( m_panelMap[ID_ITEM_GROUPSET] == NULL )
			{
				m_panelMap[ID_ITEM_GROUPSET] = new CGroupSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_GROUPSET] );
		}
		else if( strText == ID_ITEM_VARIETYINFOSET )
		{
			if( m_panelMap[ID_ITEM_VARIETYINFOSET] == NULL )
			{
				m_panelMap[ID_ITEM_VARIETYINFOSET] = new CVarietyInfoSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_VARIETYINFOSET] );
		}
		else if( strText == ID_ITEM_USERGROUP )
		{
			if( m_panelMap[ID_ITEM_USERGROUP] == NULL)
			{
				m_panelMap[ID_ITEM_USERGROUP] = new CUserGroupSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_USERGROUP] );
		} 
		else if( strText == ID_ITEM_SHOW )
		{
			if( m_panelMap[ID_ITEM_SHOW] == NULL)
			{
				m_panelMap[ID_ITEM_SHOW] = new CShowSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_SHOW] );
		}
		else if( strText == ID_ITEM_FONTANDCOLOR )
		{
			if( m_panelMap[ID_ITEM_FONTANDCOLOR] == NULL)
			{
				m_panelMap[ID_ITEM_FONTANDCOLOR] = new CFontAndColorSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_FONTANDCOLOR] );
		}
		else if( strText == ID_ITEM_OPENORDER )	
		{
			wxTreeItemIdValue chockes;
			wxTreeItemId childItemId = m_treeCtrl->GetFirstChild( itemId, chockes );
			m_treeCtrl->SelectItem( childItemId );
		}
		else if( strText == ID_ITEM_GENERALSET )			    
		{
			if( m_panelMap[ID_ITEM_GENERALSET] == NULL)
			{
				m_panelMap[ID_ITEM_GENERALSET] = new CGeneralSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_GENERALSET] );
		}
		//else if( strText == ID_ITEM_AUTOOPENCLOSESET )
		//{
		//	if( m_panelMap[ID_ITEM_AUTOOPENCLOSESET] == NULL)
		//	{
		//		m_panelMap[ID_ITEM_AUTOOPENCLOSESET] = new CAutoOpenCloseSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
		//	}
		//	SwitchShow(m_panelMap[ID_ITEM_AUTOOPENCLOSESET]);
		//}
		else if( strText == ID_ITEM_QUICKORDERSET )
		{
			if( m_panelMap[ID_ITEM_QUICKORDERSET] == NULL)
			{
				m_panelMap[ID_ITEM_QUICKORDERSET] = new CQuickOrderSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow(m_panelMap[ID_ITEM_QUICKORDERSET]);
		}
		else if( strText == ID_ITEM_KEYBOARDOPENODER )
		{
			if( m_panelMap[ID_ITEM_KEYBOARDOPENODER] == NULL)
			{
				m_panelMap[ID_ITEM_KEYBOARDOPENODER] = new CKeyboardOpenOrderSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow(m_panelMap[ID_ITEM_KEYBOARDOPENODER]);
		}
		else if( strText == ID_ITEM_MOUSEOPENORDER)
		{
			if( m_panelMap[ID_ITEM_MOUSEOPENORDER] == NULL)
			{
				m_panelMap[ID_ITEM_MOUSEOPENORDER] = new CMouseOpenOrderSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow(m_panelMap[ID_ITEM_MOUSEOPENORDER]);
		}
		else if( strText == ID_ITEM_ADVANCEDSET )                
		{
			if( m_panelMap[ID_ITEM_ADVANCEDSET] == NULL)
			{
				m_panelMap[ID_ITEM_ADVANCEDSET] = new CAdvancedSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow(m_panelMap[ID_ITEM_ADVANCEDSET]);
		}
		else if( strText == ID_ITEM_ORDERSTRATEGY )	
		{
			wxTreeItemIdValue chockes;
			wxTreeItemId childItemId = m_treeCtrl->GetFirstChild( itemId, chockes );
			m_treeCtrl->SelectItem( childItemId );
		}
		else if( strText == ID_ITEM_BASEDMULTIPE )
		{
			if( m_panelMap[ID_ITEM_BASEDMULTIPE] == NULL)
			{
				m_panelMap[ID_ITEM_BASEDMULTIPE] = new COrderStrategyPanel( BaseMultipliedWay, this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_BASEDMULTIPE] );
		}
		else if( strText == ID_ITEM_CUSTOMVOLUMN )
		{
			if( m_panelMap[ID_ITEM_CUSTOMVOLUMN] == NULL)
			{
				m_panelMap[ID_ITEM_CUSTOMVOLUMN] = new COrderStrategyPanel( UserDefinedVolumnWay, this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_CUSTOMVOLUMN] );
		} 
		else if( strText == ID_ITEM_DISTRIBUTEBYSUM )
		{
			if( m_panelMap[ID_ITEM_DISTRIBUTEBYSUM] == NULL)
			{
				m_panelMap[ID_ITEM_DISTRIBUTEBYSUM] = new COrderStrategyPanel( SumDividedWay, this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_DISTRIBUTEBYSUM] );
		} 
		else if( strText == ID_ITEM_ALIGNTOFISRTACCOUNT )
		{
			if( m_panelMap[ID_ITEM_ALIGNTOFISRTACCOUNT] == NULL)
			{
				m_panelMap[ID_ITEM_ALIGNTOFISRTACCOUNT] = new COrderAccountTypePanel( FirstAccountAlign, this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_ALIGNTOFISRTACCOUNT] );
		} 
		else if( strText == ID_ITEM_ACCOUNTSCALEDIVEDED )
		{
			if( m_panelMap[ID_ITEM_ACCOUNTSCALEDIVEDED] == NULL)
			{
				m_panelMap[ID_ITEM_ACCOUNTSCALEDIVEDED] = new COrderAccountTypePanel( AccountScaleDivided, this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_ACCOUNTSCALEDIVEDED] );
		} 
		else if( strText == ID_ITEM_GENERALCANCELORDER )
		{
			if( m_panelMap[ID_ITEM_GENERALCANCELORDER] == NULL)
			{
				m_panelMap[ID_ITEM_GENERALCANCELORDER] = new CGeneralCancelOrderSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_GENERALCANCELORDER] );
		} 
		else if( strText == ID_ITEM_RISKMANAGER )
		{
			if( m_panelMap[ID_ITEM_RISKMANAGER] == NULL)
			{
				m_panelMap[ID_ITEM_RISKMANAGER] = new CRiskManagerSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_RISKMANAGER] );
		}
		else if( strText == ID_ITEM_PROMPT )
		{
			if( m_panelMap[ID_ITEM_PROMPT] == NULL )
			{
				m_panelMap[ID_ITEM_PROMPT] = new CPromptSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE );
			}
			SwitchShow( m_panelMap[ID_ITEM_PROMPT] );
		}
		else if( strText == ID_ITEM_TABLESET )
		{
			wxTreeItemIdValue chockes;
			wxTreeItemId childItemId = m_treeCtrl->GetFirstChild( itemId, chockes );
			m_treeCtrl->SelectItem( childItemId );	
		}
		else if( strText == ID_ITEM_QUOTETABLE )
		{
			if( m_panelMap[ID_ITEM_QUOTETABLE] == NULL )
			{
				m_panelMap[ID_ITEM_QUOTETABLE] = new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_QUOT_TABLE );
			}
			SwitchShow( m_panelMap[ID_ITEM_QUOTETABLE] ); 
		}
		else if( strText == ID_ITEM_TRADERECORD )
		{
			if ( m_panelMap[ID_ITEM_TRADERECORD] == NULL )
			{
				m_panelMap[ID_ITEM_TRADERECORD] = new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_THRADE_RECORD );
			}
			SwitchShow( m_panelMap[ID_ITEM_TRADERECORD] );
		}
		else if( strText == ID_ITEM_TRADESTATISTIC )
		{
			if( m_panelMap[ID_ITEM_TRADESTATISTIC] == NULL)
			{
				m_panelMap[ID_ITEM_TRADESTATISTIC] = new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_THRADE_STATISTICS );
			}
			SwitchShow( m_panelMap[ID_ITEM_TRADESTATISTIC] );
		}
		else if( strText == ID_ITEM_POSITION )
		{
			if( m_panelMap[ID_ITEM_POSITION] == NULL)
			{
				m_panelMap[ID_ITEM_POSITION] =  new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_POSITION );
			}
			SwitchShow( m_panelMap[ID_ITEM_POSITION] );
		}
		else if ( strText == ID_ITEM_COMBO_POSITION)
		{
			if( m_panelMap[ID_ITEM_COMBO_POSITION] == NULL)
			{
				m_panelMap[ID_ITEM_COMBO_POSITION] = new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_POSITIONCOMB );
			}
			SwitchShow( m_panelMap[ID_ITEM_COMBO_POSITION] );
		}
		else if ( strText == ID_ITEM_MULTIACCOUNT)
		{
			if( m_panelMap[ID_ITEM_MULTIACCOUNT] == NULL)
			{
				m_panelMap[ID_ITEM_MULTIACCOUNT] = new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_MULTI_ACCOUNT );
			}
			SwitchShow( m_panelMap[ID_ITEM_MULTIACCOUNT] );
		}
		else if ( strText == ID_ITEM_MULTIPOSITION)
		{
			if( m_panelMap[ID_ITEM_MULTIPOSITION] == NULL)
			{
				m_panelMap[ID_ITEM_MULTIPOSITION] = new CTableSetPanel( this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_MULTI_POSITION );
			}
			SwitchShow( m_panelMap[ID_ITEM_MULTIPOSITION] );
		}
		else if( strText == ID_ITEM_POSITIONDETAIL )
		{
			if( m_panelMap[ID_ITEM_POSITIONDETAIL] == NULL )
			{
				m_panelMap[ID_ITEM_POSITIONDETAIL] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_POSITIONDETAIL );
			}
			SwitchShow( m_panelMap[ID_ITEM_POSITIONDETAIL] );
		}
		else if( strText == ID_ITEM_INSTRUMENTLIST )
		{
			if( m_panelMap[ID_ITEM_INSTRUMENTLIST] == NULL)
			{
				m_panelMap[ID_ITEM_INSTRUMENTLIST] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_INSTRUMENT );
			}
			SwitchShow( m_panelMap[ID_ITEM_INSTRUMENTLIST] );
		}
		else if( strText == ID_ITEM_ALLORDER )
		{
			if( m_panelMap[ID_ITEM_ALLORDER] == NULL)
			{
				m_panelMap[ID_ITEM_ALLORDER] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_ALL_ORDER );
			}
			SwitchShow( m_panelMap[ID_ITEM_ALLORDER] );
		}
		else if( strText == ID_ITEM_WAITTRADE )
		{
			if( m_panelMap[ID_ITEM_WAITTRADE] == NULL)
			{
				m_panelMap[ID_ITEM_WAITTRADE] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_OPEN_ORDER );
			}
			SwitchShow( m_panelMap[ID_ITEM_WAITTRADE] );
		}
		else if( strText == ID_ITEM_PARKEDORDER )
		{
			if( m_panelMap[ID_ITEM_PARKEDORDER] == NULL)
			{
				m_panelMap[ID_ITEM_PARKEDORDER] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_QUERY_ORDER_PARKED );
			}
			SwitchShow( m_panelMap[ID_ITEM_PARKEDORDER] );
		}
		else if( strText == ID_ITEM_ACCOUNT )
		{
			if( m_panelMap[ID_ITEM_ACCOUNT] == NULL)
			{
				m_panelMap[ID_ITEM_ACCOUNT] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_ACCOUNT );
			}
			SwitchShow( m_panelMap[ID_ITEM_ACCOUNT] );
		}
		else if( strText == ID_ITEM_QUERYORDERBATCH )
		{
			if( m_panelMap[ID_ITEM_QUERYORDERBATCH] == NULL)
			{
				m_panelMap[ID_ITEM_QUERYORDERBATCH] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_QUERY_ORDER_BATCH );
			}
			SwitchShow( m_panelMap[ID_ITEM_QUERYORDERBATCH] );
		}
		else if( strText == ID_ITEM_QUERYORDERCONDITION )
		{
			if( m_panelMap[ID_ITEM_QUERYORDERCONDITION] == NULL)
			{
				m_panelMap[ID_ITEM_QUERYORDERCONDITION] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_QUERY_ORDER_CONDITION );
			}
			SwitchShow( m_panelMap[ID_ITEM_QUERYORDERCONDITION] );
		}
		else if( strText == ID_ITEM_QUERYORDERPOSITION )
		{
			if( m_panelMap[ID_ITEM_QUERYORDERPOSITION] ==NULL)
			{
				m_panelMap[ID_ITEM_QUERYORDERPOSITION] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_QUERY_ORDER_POSITION);
			}
			SwitchShow ( m_panelMap[ID_ITEM_QUERYORDERPOSITION] );
		}
		else if( strText == ID_ITEM_ORDER_INSERT )
		{
			if( m_panelMap[ID_ITEM_ORDER_INSERT] ==NULL)
			{
				m_panelMap[ID_ITEM_ORDER_INSERT] = new CTableSetPanel(this,wxID_ANY,PANEL_POS,PANEL_SIZE, GID_ORDER_INSERT_PANEL, false);
			}
			SwitchShow ( m_panelMap[ID_ITEM_ORDER_INSERT] );
		}
	}

	event.Skip();

}

void CConfigAppDlg::OnOK( wxCommandEvent& event)
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if( NULL == pCfgMgr)
		return;

	vector<MSG_TYPE> vecMsg;
	if ( NULL != m_panelMap[ID_ITEM_GROUPSET] )
	{
		((CGroupSetPanel*)m_panelMap[ID_ITEM_GROUPSET])->WriteCfg();
	}

	if ( NULL != m_panelMap[ID_ITEM_SELFGROUP] )
	{
		if( ((CSelfGroupSetPanel*)m_panelMap[ID_ITEM_SELFGROUP])->WriteCfg())
		{
			vecMsg.push_back(MSG_INSTRUMENT_GROUP_CHANGE);
		}
	}

	if ( NULL != m_panelMap[ID_ITEM_VARIETYINFOSET] )
	{
		if ( ((CVarietyInfoSetPanel*)m_panelMap[ID_ITEM_VARIETYINFOSET])->WriteCfg())
		{
			//不需要刷新
			//vecMsg.push_back(MSG_VARIETYINFO_CHANGE);
		}
	}

	//用户组
	if ( NULL != m_panelMap[ID_ITEM_USERGROUP] )
	{
		if(((CUserGroupSetPanel*)m_panelMap[ID_ITEM_USERGROUP])->WriteCfg())
		{
			vecMsg.push_back(MSG_USER_GROUP_CHANGE);
		}
	}

	//显示
	if ( NULL != m_panelMap[ID_ITEM_SHOW] )
	{
		if( ((CShowSetPanel*)m_panelMap[ID_ITEM_SHOW])->WriteCfg())
		{
			vecMsg.push_back(MSG_DISPLAY_CHANGE);
		}
	}

	bool bOrderCfgChanged = false;
	if ( NULL != m_panelMap[ID_ITEM_GENERALSET] )
	{
		if( ((CGeneralSetPanel*)m_panelMap[ID_ITEM_GENERALSET])->WriteCfg())
		{
			bOrderCfgChanged = true;
		}
	}

	//if ( NULL != m_panelMap[ID_ITEM_AUTOOPENCLOSESET] )
	//{
	//	if( ((CAutoOpenCloseSetPanel*)m_panelMap[ID_ITEM_AUTOOPENCLOSESET])->WriteCfg())
	//	{
	//		bOrderCfgChanged = true;
	//	}
	//}

	if ( NULL != m_panelMap[ID_ITEM_KEYBOARDOPENODER] )
	{
		if ( ((CKeyboardOpenOrderSetPanel*)m_panelMap[ID_ITEM_KEYBOARDOPENODER])->WriteCfg())
		{
			bOrderCfgChanged = true;
		}
	}

	if( NULL != m_panelMap[ID_ITEM_QUICKORDERSET] )
	{
		((CQuickOrderSetPanel*)m_panelMap[ID_ITEM_QUICKORDERSET])->WriteCfg();
	}

	if( NULL != m_panelMap[ID_ITEM_MOUSEOPENORDER] )
	{
		if( ((CMouseOpenOrderSetPanel*)m_panelMap[ID_ITEM_MOUSEOPENORDER])->WriteCfg())
		{
			bOrderCfgChanged = true;
		}
	}

	if( NULL != m_panelMap[ID_ITEM_ADVANCEDSET] )
	{
		if( ((CAdvancedSetPanel*)m_panelMap[ID_ITEM_ADVANCEDSET])->WriteCfg())
		{
			bOrderCfgChanged = true;
		}
	}

	if ( bOrderCfgChanged )
	{
		vecMsg.push_back( MSG_ORDER_CHANGE );
	}

	if( NULL != m_panelMap[ID_ITEM_BASEDMULTIPE] )
	{
		if( ((COrderStrategyPanel*)m_panelMap[ID_ITEM_BASEDMULTIPE])->WriteCfg())
		{
		}
	}

	if( NULL != m_panelMap[ID_ITEM_CUSTOMVOLUMN] )
	{
		if( ((COrderStrategyPanel*)m_panelMap[ID_ITEM_CUSTOMVOLUMN])->WriteCfg())
		{
		}
	}

	if( NULL != m_panelMap[ID_ITEM_DISTRIBUTEBYSUM] )
	{
		if( ((COrderStrategyPanel*)m_panelMap[ID_ITEM_DISTRIBUTEBYSUM])->WriteCfg())
		{
		}
	}

	if( NULL != m_panelMap[ID_ITEM_ALIGNTOFISRTACCOUNT] )
	{
		if( ((COrderAccountTypePanel*)m_panelMap[ID_ITEM_ALIGNTOFISRTACCOUNT])->WriteCfg())
		{
		}
	}

	if( NULL != m_panelMap[ID_ITEM_ACCOUNTSCALEDIVEDED] )
	{
		if( ((COrderAccountTypePanel*)m_panelMap[ID_ITEM_ACCOUNTSCALEDIVEDED])->WriteCfg())
		{
		}
	}

	// 撤单
	bool bCancelOrderCfgChanged = false;
	if( NULL != m_panelMap[ID_ITEM_GENERALCANCELORDER] )
	{
		if( ((CGeneralCancelOrderSetPanel*)m_panelMap[ID_ITEM_GENERALCANCELORDER])->WriteCfg())
		{
			bCancelOrderCfgChanged = true;
		}
	}

	if ( bCancelOrderCfgChanged )
	{
		vecMsg.push_back(MSG_CANCEL_ORDER_CHANGE);
		//vecMsg.push_back(MSG_LIST_ALL_ORDER_CHANGE);
	}

	//风险管理
	if( NULL != m_panelMap[ID_ITEM_RISKMANAGER] )
	{
		if( ((CRiskManagerSetPanel*)m_panelMap[ID_ITEM_RISKMANAGER])->WriteCfg())
		{
			vecMsg.push_back( MSG_CANCEL_ORDER_CHANGE);
		}
	}

	//提示及反馈
	if( NULL != m_panelMap[ID_ITEM_PROMPT] )
	{
		((CPromptSetPanel*)m_panelMap[ID_ITEM_PROMPT])->WriteCfg();
	}

	//表格设置
	if( NULL != m_panelMap[ID_ITEM_QUOTETABLE] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_QUOTETABLE])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_BAOJIABIAO_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_TRADERECORD] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_TRADERECORD])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_TRADE_RECORD_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_TRADESTATISTIC] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_TRADESTATISTIC])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_TRADE_STATIC_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_POSITION] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_POSITION])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_POSITION_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_POSITIONDETAIL] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_POSITIONDETAIL])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_POSITION_DETAIL_CHANGE);
		}
	}

	if ( NULL != m_panelMap[ID_ITEM_COMBO_POSITION])
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_COMBO_POSITION])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_COMBO_POSITION_CHANGE);
		}
	}

	if ( NULL != m_panelMap[ID_ITEM_MULTIACCOUNT])
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_MULTIACCOUNT])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_MULTIACCOUNT_CHANGE);
		}
	}

	if ( NULL != m_panelMap[ID_ITEM_MULTIPOSITION])
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_MULTIPOSITION])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_MULTIPOSITION_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_INSTRUMENTLIST] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_INSTRUMENTLIST])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_INSTRUMENT_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_ALLORDER] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_ALLORDER])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_ALL_ORDER_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_WAITTRADE] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_WAITTRADE])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_OPEN_ORDER_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_PARKEDORDER] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_PARKEDORDER])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_QUERY_ORDER_PARKED_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_ACCOUNT] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_ACCOUNT])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_ACCOUNT_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_QUERYORDERBATCH] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_QUERYORDERBATCH])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_QUERY_ORDER_BATCH_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_QUERYORDERCONDITION] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_QUERYORDERCONDITION])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_QUERY_ORDER_CONDITION_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_QUERYORDERPOSITION] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_QUERYORDERPOSITION])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_QUERY_ORDER_POSITION_CHANGE);
		}
	}

	if( NULL != m_panelMap[ID_ITEM_ORDER_INSERT] )
	{
		if( ((CTableSetPanel*)m_panelMap[ID_ITEM_ORDER_INSERT])->WriteCfg())
		{
			vecMsg.push_back(MSG_LIST_ORDER_INSERT_CHANGE);
		}
	}

	if ( !vecMsg.empty())
	{
		wxString strMsg;
		for ( unsigned int i = 0; i < vecMsg.size(); i++ )
		{
			if ( i != 0 )
			{
				strMsg += STRING_MSG_SEPERATOR;
			}

			char szBuf[MAX_PATH];
			memset( szBuf, 0, MAX_PATH );
			itoa( static_cast<int>(vecMsg[i]), szBuf, 10 );
			strMsg += szBuf;
		}

		wxCommandEvent myEvent( wxEVT_CONFIG_CHANGED );
//		myEvent.SetString( strMsg.c_str() );
//		GetParent()->AddPendingEvent(myEvent);
        if(1)
        {
            DWORD EvtParamID;
            std::string EvtParamStr(strMsg.c_str());
            if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
            {
                myEvent.SetInt((int)EvtParamID);
                GetParent()->AddPendingEvent(myEvent);
            }
        }
	}

	event.Skip();
}

void CConfigAppDlg::OnCancel(wxCommandEvent& event)
{
	if ( m_panelMap[ID_ITEM_FONTANDCOLOR] != NULL )
	{
		CFontAndColorSetPanel* pPanel = (CFontAndColorSetPanel*)(m_panelMap[ID_ITEM_FONTANDCOLOR]);
		pPanel->Recovery();
	}

	event.Skip();
}

void CConfigAppDlg::OnShowPanel( wxCommandEvent& event )
{
	SetDefaultShowPanel(event.GetInt());
}

void CConfigAppDlg::SetDefaultShowPanel( const int defaultItemId )
{
	std::map<int, wxTreeItemId>::iterator it = m_treeItemMap.find( defaultItemId );
	if ( it != m_treeItemMap.end() )
	{
		m_treeCtrl->SelectItem( it->second );
	}
}

void CConfigAppDlg::OnGroupChange( wxCommandEvent& event )
{
	if ( NULL != m_panelMap[ID_ITEM_SELFGROUP])
	{
		((CSelfGroupSetPanel*)m_panelMap[ID_ITEM_SELFGROUP])->ResetSelfGroupItems();
	}
}
