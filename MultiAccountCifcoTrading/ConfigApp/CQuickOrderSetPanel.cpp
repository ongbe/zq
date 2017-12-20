#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"
#include "CQuickOrderSetPanel.h"


BEGIN_EVENT_TABLE(CQuickOrderSetPanel, wxPanel)

END_EVENT_TABLE()

CQuickOrderSetPanel::CQuickOrderSetPanel(wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	const int nLeftBorder = 10;
	const int nTop = 5;
	const int nRowDistance = 8;
	const int nControlHeight = 20;

	int nControlTop = nTop;
	new wxStaticText(this, wxID_ANY, "鼠标点报价表及键盘快速功能：", wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	//都无反向持仓时
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "都无反向持仓时", wxPoint(nLeftBorder+20, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComNoOppositePosition = new wxOwnerDrawnComboBox(this, ID_OPPOSITE_POSITION1, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(150, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComNoOppositePosition->Append("按默认手数和策略开仓");

	//部分有反向持仓时
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "部分有反向持仓时", wxPoint(nLeftBorder+20, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComSomeHaveOppositePosition = new wxOwnerDrawnComboBox(this, ID_OPPOSITE_POSITION2, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(300, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComSomeHaveOppositePosition->Append("按默认手数和策略开仓");
	m_pComSomeHaveOppositePosition->Append("按持仓量全平");
	m_pComSomeHaveOppositePosition->Append("有仓账户按默认手数和策略平仓，超出不处理；无仓账户不处理");
	m_pComSomeHaveOppositePosition->Append("有仓账户按默认手数和策略平仓，超出反向开仓；无仓账户不处理");
	m_pComSomeHaveOppositePosition->Append("按默认手数和策略：有仓账户平仓，超出不处理；无仓账户开仓");
	m_pComSomeHaveOppositePosition->Append("按默认手数和策略：有仓账户平仓，超出反向开仓；无仓账户开仓");

	//都有反向持仓时
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "都有反向持仓时", wxPoint(nLeftBorder+20, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComAllHaveOppositePosition = new wxOwnerDrawnComboBox(this, ID_OPPOSITE_POSITION3, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(200, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComAllHaveOppositePosition->Append("按默认手数和策略开仓");
	m_pComAllHaveOppositePosition->Append("按持仓量全平");
	m_pComAllHaveOppositePosition->Append("按默认手数和策略平仓，超出不处理");
	m_pComAllHaveOppositePosition->Append("按默认手数和策略平仓，超出反向开仓");

	//鼠标点持仓或持仓汇总
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "鼠标点持仓或持仓汇总：", wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComMouseClosePosition = new wxOwnerDrawnComboBox(this, ID_MOUSE_CLOSE_POSITION, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(150, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComMouseClosePosition->Append("按持仓量全平");

	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pChkActionBeforeOpenOrder = new wxCheckBox(this, ID_ACTION_BEFORE_OPEN_ORDER, "下单前先撤原同方向挂单", wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "注：多账户自动开平策略所有有效账户视为一个，在部分有反向持仓时，按有仓账户",
		wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "处理，具体请参照说明书。", 
		wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	ReadCfg();
	InitControls();
}

CQuickOrderSetPanel::~CQuickOrderSetPanel()
{

}

void CQuickOrderSetPanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	wxString strLable = GetStrategyString(pOrderCfg->nAllNotHaveOppositePosition);
	m_pComNoOppositePosition->SetStringSelection(strLable);

	strLable = GetStrategyString(pOrderCfg->nSomeHaveOppositePosition);
	m_pComSomeHaveOppositePosition->SetStringSelection(strLable);

	strLable = GetStrategyString(pOrderCfg->nAllHaveOppositePosition);
	m_pComAllHaveOppositePosition->SetStringSelection(strLable);

	strLable = GetStrategyString(pOrderCfg->nMouseClosePositionStrategy);
	m_pComMouseClosePosition->SetStringSelection(strLable);

	m_pChkActionBeforeOpenOrder->SetValue(pOrderCfg->nCancelSameDirOpenOrderBeforeOrder>0 );

}

bool CQuickOrderSetPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	wxString strLable = m_pComNoOppositePosition->GetStringSelection();
	pOrderCfg->nAllNotHaveOppositePosition = GetStrategyInt(strLable);

	strLable = m_pComSomeHaveOppositePosition->GetStringSelection();
	pOrderCfg->nSomeHaveOppositePosition = GetStrategyInt(strLable);

	strLable = m_pComAllHaveOppositePosition->GetStringSelection();
	pOrderCfg->nAllHaveOppositePosition = GetStrategyInt(strLable);

	strLable = m_pComMouseClosePosition->GetStringSelection();
	pOrderCfg->nMouseClosePositionStrategy = GetStrategyInt(strLable);

	if ( m_pChkActionBeforeOpenOrder->GetValue() )
	{
		pOrderCfg->nCancelSameDirOpenOrderBeforeOrder = 1;
	}
	else
	{
		pOrderCfg->nCancelSameDirOpenOrderBeforeOrder = 0;
	}

	return true;
}

void CQuickOrderSetPanel::ReadCfg()
{

}

wxString CQuickOrderSetPanel::GetStrategyString( int nQuickOrderStrategy )
{
	wxString strLable;
	switch(nQuickOrderStrategy)
	{
	case QOrder_OpenDefaultAndStrategy:
		strLable = "按默认手数和策略开仓";
		break;
	case QOrder_CloseAll:
		strLable = "按持仓量全平";
		break;
	case QOrder_AutoCloseDefaultAndNotOpen:
		strLable = "有仓账户按默认手数和策略平仓，超出不处理；无仓账户不处理";
		break;
	case QOrder_AutoReverseOpenAndNotOpen:
		strLable = "有仓账户按默认手数和策略平仓，超出反向开仓；无仓账户不处理";
		break;
	case QOrder_AutoCloseDefaultAndOpen:
		strLable = "按默认手数和策略：有仓账户平仓，超出不处理；无仓账户开仓";
		break;
	case QOrder_AutoReverseOpenAndOpen:
		strLable = "按默认手数和策略：有仓账户平仓，超出反向开仓；无仓账户开仓";
		break;
	case QOrder_AutoCloseDefault:
		strLable = "按默认手数和策略平仓，超出不处理";
		break;
	case QOrder_AutoReverseOpen:
		strLable = "按默认手数和策略平仓，超出反向开仓";
		break;
	default:
		break;
	}

	return strLable;
}

int CQuickOrderSetPanel::GetStrategyInt( const wxString& strLable )
{
	if ( strLable == "按默认手数和策略开仓")
	{
		return QOrder_OpenDefaultAndStrategy;
	}
	else if ( strLable == "按持仓量全平")
	{
		return QOrder_CloseAll;
	}
	else if ( strLable == "有仓账户按默认手数和策略平仓，超出不处理；无仓账户不处理")
	{
		return QOrder_AutoCloseDefaultAndNotOpen;
	}
	else if ( strLable == "有仓账户按默认手数和策略平仓，超出反向开仓；无仓账户不处理")
	{
		return QOrder_AutoReverseOpenAndNotOpen;
	}
	else if ( strLable == "按默认手数和策略：有仓账户平仓，超出不处理；无仓账户开仓")
	{
		return QOrder_AutoCloseDefaultAndOpen;
	}
	else if ( strLable == "按默认手数和策略：有仓账户平仓，超出反向开仓；无仓账户开仓")
	{
		return QOrder_AutoReverseOpenAndOpen;
	}
	else if ( strLable == "按默认手数和策略平仓，超出不处理")
	{
		return QOrder_AutoCloseDefault;
	}
	else if ( strLable == "按默认手数和策略平仓，超出反向开仓")
	{
		return QOrder_AutoReverseOpen;
	}
	else
	{
		return -1;
	}
}

