#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"
#include "../inc/cfgMgr/OrderStrategyRowCfg.h"
#include "COrderStrategyPanel.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#include "../inc/ISvr/PlatformSvrMgr.h"

extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(COrderStrategyPanel, wxPanel)
	EVT_BUTTON(ID_ADD_CONTRACT, OnAddContract)
	EVT_BUTTON(ID_DEL_CONTRACT, OnDelContract)
	EVT_BUTTON(ID_SET_DEFAULT_VOLUME, OnSetDefaultVolume)
	EVT_LIST_EXT_END_LABEL_EDIT(ID_LIST, OnLabelEndEdit)
END_EVENT_TABLE()

COrderStrategyPanel::COrderStrategyPanel(OrderStrategy eStrategy, 
							   wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	m_eStrategy = eStrategy;

	wxString strLabel;
	wxString strSecondColName;
	switch(eStrategy)
	{
	case BaseMultipliedWay:
		strLabel = "基数乘数方式";
		strSecondColName = "默认";
		break;
	case UserDefinedVolumnWay:
		strLabel = "策略版自定义手数方式";
		strSecondColName = "默认手数";
		break;
	case SumDividedWay:
		strLabel = "总数分配方式";
		strSecondColName = "分配权重";
		break;
	default:
		break;
	}
	const int nLeftBorder = 10;
	const int nTop = 5;
	const int nControlHeight = 20;

	int nControlTop = nTop;
	new wxStaticText(this, wxID_ANY, strLabel, wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	new wxStaticText(this, wxID_ANY, "合约代码", wxPoint(nLeftBorder+190, nControlTop), wxSize(50, nControlHeight));
	m_pContractText = new wxTextCtrl(this, ID_INPUT_CONTRACT, "", wxPoint(nLeftBorder+245, nControlTop), wxSize(70, nControlHeight));
	new wxButton(this, ID_ADD_CONTRACT, "增加合约", wxPoint(nLeftBorder+335, nControlTop), wxSize(60, nControlHeight));
	new wxButton(this, ID_DEL_CONTRACT, "删除合约", wxPoint(nLeftBorder+410, nControlTop), wxSize(60, nControlHeight));

	m_pList = new wxExtListCtrl( this, ID_LIST, wxPoint(nLeftBorder, nTop+30), 
		wxSize(480, 275), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE|wxLCEXT_SINGLE_SEL);
	m_pList->InsertColumn(0, "用户", wxALIGN_LEFT, 80);
	m_pList->InsertColumn(1, strSecondColName, wxALIGN_LEFT, 60);
	m_pList->SetColumnCtrlType(1,COLUMN_CTRL_TYPE_EDIT);

	new wxButton(this, ID_SET_DEFAULT_VOLUME, "设置默认手数", wxPoint(nLeftBorder+335, 315), wxSize(80, nControlHeight));

	ReadCfg();
	InitControls();
}

COrderStrategyPanel::~COrderStrategyPanel()
{

}

void COrderStrategyPanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;

	//添加合约列
	pMgr->GetContracts(m_eStrategy, m_vContracts);
	for ( UINT i = 0; i < m_vContracts.size(); i++ )
	{

		int nColCount = m_pList->GetColumnCount();
		m_pList->InsertColumn(nColCount, m_vContracts[i], wxALIGN_LEFT, 50);
		m_pList->SetColumnCtrlType(nColCount, COLUMN_CTRL_TYPE_EDIT);
	}

	int nAccountCount = MA_CORE()->GetTradeAccountCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
		if ( NULL == pTradeAccount )
		{
			continue;
		}

		int nCount = m_pList->GetItemCount();
		m_pList->InsertItem(nCount, "");
		m_pList->SetItem(nCount, 0, pTradeAccount->t.szTradeAccount);

		//查找配置信息
		OrderStrategyRowCfg rowCfg;
		if ( pMgr->GetOrderStrategyData(m_eStrategy, 
			pTradeAccount->t.szTradeAccount, rowCfg))
		{
			wxString strVolume = wxString::Format("%d", rowCfg.GetDefaultValue());
			m_pList->SetItem(nCount, 1, strVolume);
			//填写列信息
			for ( UINT j = 0; j < m_vContracts.size(); j++ )
			{
				int nVolume = rowCfg.GetContractValue(m_vContracts[j]);
				strVolume = wxString::Format("%d", nVolume);
				m_pList->SetItem(nCount, j+2, strVolume);
			}
		}
		else
		{
			m_pList->SetItem(nCount, 1, "1");
			//填写列信息
			for ( UINT j = 0; j < m_vContracts.size(); j++ )
			{
				m_pList->SetItem(nCount, j+2, "1");
			}
		}
	}
}

bool COrderStrategyPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	std::vector<OrderStrategyRowCfg> vData;
	int nCount = m_pList->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		wxString strAccount = m_pList->GetSubItemText(i, 0);
		int nDefaultValue = atoi(m_pList->GetSubItemText(i, 1).c_str());
		OrderStrategyRowCfg rowCfg(strAccount.c_str(), nDefaultValue);
		for ( UINT j = 0; j < m_vContracts.size(); j++ )
		{
			int nContractVolume = atoi(m_pList->GetSubItemText(i, j+2).c_str());
			rowCfg.AddContractVolumn(m_vContracts[j], nContractVolume);
		}

		vData.push_back(rowCfg);
	}

	pMgr->SetOrderStrategyData(m_eStrategy, m_vContracts, vData);

	return true;
}

void COrderStrategyPanel::ReadCfg()
{

}

void COrderStrategyPanel::OnAddContract( wxCommandEvent& event )
{
	wxString strContract = m_pContractText->GetValue();
	if ( strContract.IsEmpty())
	{
		return;
	}

	strContract = strContract.MakeLower();
	if ( IsExist(strContract.c_str()))
	{
		wxMessageBox("该合约已经存在于列表中！");
		return;
	}

	wxString strTarget;
	if ( !MatchInstrument(strContract, strTarget))
	{
		wxMessageBox("无法找到匹配的品种或合约！");
		return;
	}

	m_vContracts.push_back(strTarget.c_str());

	int nColCount = m_pList->GetColumnCount();
	m_pList->InsertColumn(nColCount, strTarget, wxALIGN_LEFT, 50);
	m_pList->SetColumnCtrlType(nColCount, COLUMN_CTRL_TYPE_EDIT);
	int nItemCount = m_pList->GetItemCount();
	for ( int i = 0; i < nItemCount; i++ )
	{
		m_pList->SetItem(i, nColCount, "1");
	}
}

void COrderStrategyPanel::OnDelContract( wxCommandEvent& event )
{
	wxString strContract = m_pContractText->GetValue();
	if ( strContract.IsEmpty())
	{
		return;
	}

	std::vector<std::string>::iterator it = m_vContracts.begin();
	for ( ; it != m_vContracts.end(); it++ )
	{
		wxString strTemp = (*it).c_str();
		if ( strContract.MakeLower() == strTemp.MakeLower())
		{
			m_vContracts.erase(it);
			break;
		}
	}

	int nColCount = m_pList->GetColumnCount();
	for( int i = 2; i < nColCount; i++ )
	{
		wxExtListItem item;
		if( m_pList->GetColumn(i, item) )
		{
			wxString strTemp = item.GetText();
			if ( strContract.MakeLower() == strTemp.MakeLower())
			{
				m_pList->DeleteColumn(i);
				break;
			}
		}
	}

	m_pList->Refresh();
}

bool COrderStrategyPanel::IsExist( const wxString& strContract)
{
	wxString strTempContract = strContract;
	for ( UINT i = 0; i < m_vContracts.size(); i++ )
	{
		wxString strTemp = m_vContracts[i].c_str();
		if ( strTempContract.MakeLower() == strTemp.MakeLower() )
		{
			return true;
		}
	}

	return false;
}

void COrderStrategyPanel::OnSetDefaultVolume( wxCommandEvent& event )
{
	wxCommandEvent myEvent( wxEVT_SHOW_PANEL );
	myEvent.SetInt( ID_ITEM_VARIETYINFOSET );
	GetParent()->AddPendingEvent( myEvent );
}

bool COrderStrategyPanel::MatchInstrument( const wxString& strOri, wxString& strTarget )
{
	if ( NULL == g_pPlatformMgr || NULL == g_pPlatformMgr->GetCurrentPlatformSvr())
	{
		return false;
	}

	IPlatformSingleSvr* pSvr = g_pPlatformMgr->GetCurrentPlatformSvr();

	std::map<std::string, std::string> outData;
	pSvr->GetInstrumentVarietyMap(outData);
	std::map<std::string, std::string>::iterator it = outData.find(strOri.c_str());
	if ( it == outData.end() )
	{
		std::map<std::string, std::set<std::string>> productIDData;
		pSvr->GetProductID(productIDData);

		std::map<std::string, std::string> productKeyMap;
		std::map<std::string, std::set<std::string>>::iterator it_product
			= productIDData.begin();
		for ( ; it_product != productIDData.end() ;it_product++)
		{
			wxString strKey = it_product->first;
			strKey = strKey.MakeLower();
			productKeyMap[strKey.c_str()] = it_product->first;
		}

		std::map<std::string, std::string>::iterator it_productKey
			= productKeyMap.find(strOri.c_str());
		if ( it_productKey == productKeyMap.end() )
		{
			return false;
		}
		else
		{
			strTarget= it_productKey->second;
			return true;
		}
	}
	else
	{
		strTarget = it->second.c_str();
		return true;
	}
}

void COrderStrategyPanel::OnLabelEndEdit( wxExtListEvent& event )
{
	wxString strValue = event.GetLabel();
	if( strValue.empty() )
	{
		event.Veto();
		return;
	}

	if ( !strValue.IsNumber())
	{
		event.Veto();
		return;
	}

	if ( strValue.find('.') != -1 )
	{
		event.Veto();
		return;
	}

	if ( atoi(strValue.c_str()) <= 0 )
	{
		event.Veto();
		return;
	}
}

