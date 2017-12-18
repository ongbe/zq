#include "StdAfx.h"
#include "COrderListPanel.h"
#include "../inc/Module-Misc/TradeInfoDlg.h" //交易信息提示对话框
#include "../Module-Misc/LogDefine.h"


map<string, DWORD> COrderListPanel::m_mapCancelWait;	// 所有等待的撤单的集合


BEGIN_EVENT_TABLE(COrderListPanel, wxPanel)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderListPanel::COrderListPanel(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	wxPanel(parent,winid,pos,size,style,name)
{
}

COrderListPanel::~COrderListPanel()
{
}

BOOL COrderListPanel::IsFocusInPanel(wxWindow* pCurrWnd)
{
	wxWindow* pWin = NULL, *pWinParent = NULL;
	BOOL bFound = FALSE;
	if((pWin = FindFocus())==NULL)
		return FALSE;
	if(pWin==pCurrWnd)
		return TRUE;
	do
	{
		pWinParent = pWin->GetParent();
		if(pWinParent!=NULL)
		{
			if(pWinParent==pCurrWnd)
			{
				bFound = TRUE;
				break;
			}
		}
		pWin = pWinParent;
	} while(pWinParent!=NULL);
	return bFound;
}

//显示错误信息
void COrderListPanel::ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow)
{
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();
	TRADEINFODLG(this)->ShowTradeMsg(LOADSTRING_TEXT(errormessage), -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = errormessage;
	TRADEINFODLG(this)->WriteLog( pData );
}
