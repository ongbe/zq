#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"
#include "../inc/cfgMgr/MultiAccountMgr.h"
#include "COrderAccountTypePanel.h"


BEGIN_EVENT_TABLE(COrderAccountTypePanel, wxPanel)

END_EVENT_TABLE()

COrderAccountTypePanel::COrderAccountTypePanel(OrderStrategy eStrategy, 
							   wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	m_eStrategy = eStrategy;

	wxString strLabelLine1;
	wxString strLabelLine2;
	switch(eStrategy)
	{
	case FirstAccountAlign:
		strLabelLine1 = "第一个账户的下单手数与下单板相等，其他账户的下单手数=";
		strLabelLine2 = "下单板手数（设置类型的金额）";
		break;
	case AccountScaleDivided:
		strLabelLine1 = "下单板指定的手数为总数，按照各账户设置资金的权重进行";
		strLabelLine2 = "分配";
		break;
	default:
		break;
	}

	const int nLeftBorder = 50;
	const int nTop = 25;
	const int nControlHeight = 20;

	int nControlTop = nTop;
	new wxStaticText(this, wxID_ANY, strLabelLine1, wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	nControlTop += 25;
	new wxStaticText(this, wxID_ANY, strLabelLine2, wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	nControlTop += 50;
	new wxStaticText(this, wxID_ANY, "用于计算的资金类型", wxPoint(nLeftBorder, nControlTop), wxSize(120, nControlHeight));
	m_pCbxAccountType = new wxOwnerDrawnComboBox(this, ID_COMBOX, "", wxPoint(nLeftBorder+125, nControlTop), wxSize(100, nControlHeight),0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pCbxAccountType->Append("动态权益");
	m_pCbxAccountType->Append("可用资金");
	m_pCbxAccountType->Append("静态权益");

	ReadCfg();
	InitControls();
}

COrderAccountTypePanel::~COrderAccountTypePanel()
{

}

void COrderAccountTypePanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;

	MULTIACCOUNT* pMultiAccount = pMgr->GetMultiAccountCfg();
	switch(m_eStrategy)
	{
	case FirstAccountAlign:
		m_pCbxAccountType->SetSelection(pMultiAccount->nFirstAccountAlign);
		break;
	case AccountScaleDivided:
		m_pCbxAccountType->SetSelection(pMultiAccount->nAccountScaleDivided);
		break;
	default:
		break;
	}
}

bool COrderAccountTypePanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	MULTIACCOUNT* pMultiAccount = pMgr->GetMultiAccountCfg();
	switch(m_eStrategy)
	{
	case FirstAccountAlign:
		pMultiAccount->nFirstAccountAlign = m_pCbxAccountType->GetSelection();
		break;
	case AccountScaleDivided:
		pMultiAccount->nAccountScaleDivided = m_pCbxAccountType->GetSelection();
		break;
	default:
		break;
	}

	return true;
}

void COrderAccountTypePanel::ReadCfg()
{

}


