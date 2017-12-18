#include "StdAfx.h"
#include "COrderInsertCommandPanel.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Orders/COrderInsertEntity.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


IMPLEMENT_CLASS(COrderInsertCommandPanel, wxPanel)


BEGIN_EVENT_TABLE(COrderInsertCommandPanel, wxPanel)
	EVT_SIZE(OnSize)
//	EVT_PAINT(OnPaint)
//	EVT_LEFT_DOWN(OnLeftDown)
//	EVT_SET_CURSOR(OnSetCursor)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderInsertCommandPanel::COrderInsertCommandPanel(wxWindow *parent,
                 wxWindowID id,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxString& name):
	wxPanel(parent,id,pos,size,style,name)
{

	SetBackgroundColour(DEFAULT_COLOUR);

	wxPoint ctlPos(0,0);
	wxSize ctlSz(0,0);

	ctlSz.Set(80+5, 44);
	wxButton* itemButton1 = new wxButton( this, ID_BUTTON_OK, wxEmptyString, ctlPos, ctlSz, 0 );//_("下单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButtonBatchOrder = new wxButton( this, ID_BUTTON_BATCHORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("批量下单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButtonConditionOrder = new wxButton( this, ID_BUTTON_CONDITIONORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("下条件单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButtonPositionOrder = new wxButton( this, ID_BUTTON_POSITIONORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("下盈损单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButtonStopOrder = new wxButton( this, ID_BUTTON_STOPORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("下停止单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButtonFAKFOKOrder = new wxButton( this, ID_BUTTON_FAKFOKORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("下FAKFOK单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButtonPatsOrder = new wxButton( this, ID_BUTTON_PATSORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("下停止单")
	ctlSz.Set(80+5, 44);
	wxButton* itemButton2 = new wxButton( this, ID_BUTTON_CANCEL, wxEmptyString, ctlPos, ctlSz, 0 );//_("取消")
	ctlSz.Set(40+2, 21);
	wxButton* itemButton3 = new wxButton( this, ID_BUTTON_PARKEDORDER, wxEmptyString, ctlPos, ctlSz, 0 );//_("预埋")
	ctlSz.Set(40+2, 21);
	wxButton* itemButton4 = new wxButton( this, ID_BUTTON_MARKETPRICE, wxEmptyString, ctlPos, ctlSz, 0 );

	ResetLanguage();
}

COrderInsertCommandPanel::~COrderInsertCommandPanel()
{
}

void COrderInsertCommandPanel::InsertCtrlID2TabCtrl(vector<UINT>& tabCtrlList)
{
	for(int i=0; i<(int)m_arrTabCtrl.size(); i++) {
		tabCtrlList.push_back((UINT)m_arrTabCtrl[i]);
	}
}

void COrderInsertCommandPanel::ResetViewStyle(int nViewStyle)
{

	int nFW = 8, nFH = 22;
	int nVW = nFW, nVH = nFH+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0,0);
	wxWindow* pwinItem=NULL;

	//ctlPos = wxPoint(3, 3);
	pwinItem=FindWindowById(ID_BUTTON_OK, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	//ctlPos = wxPoint(3, 3);
	pwinItem=FindWindowById(ID_BUTTON_BATCHORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	//ctlPos = wxPoint(m_posOrder.x, m_posOrder.y+nVH*6);
	pwinItem=FindWindowById(ID_BUTTON_CONDITIONORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	//ctlPos = wxPoint(m_posOrder.x, m_posOrder.y+nVH*6);
	pwinItem=FindWindowById(ID_BUTTON_POSITIONORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	//ctlPos = wxPoint(m_posOrder.x, m_posOrder.y+nVH*6);
	pwinItem=FindWindowById(ID_BUTTON_STOPORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	//ctlPos = wxPoint(m_posOrder.x, m_posOrder.y+nVH*6);
	pwinItem=FindWindowById(ID_BUTTON_FAKFOKORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	pwinItem=FindWindowById(ID_BUTTON_PATSORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	ctlPos = wxPoint(80+8, 0);
	pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	ctlPos = wxPoint(80+8, nVH-1);
	pwinItem=FindWindowById(ID_BUTTON_PARKEDORDER, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);
	ctlPos = wxPoint(80+8+40+3, nVH-1);
	pwinItem=FindWindowById(ID_BUTTON_MARKETPRICE, this);
	if(pwinItem) pwinItem->SetPosition(ctlPos);

	for(int i=ID_BUTTON_OK; i<=ID_BUTTON_MAX; i++) {
		pwinItem=FindWindowById(i, this);
		if(pwinItem) {
			pwinItem->Show(false);
			pwinItem->Enable(true);
		}
	}

	m_arrTabCtrl.clear();

	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
        switch(nViewStyle) {
        case 0:		// 嵌入面板时
            pwinItem=FindWindowById(ID_BUTTON_OK, this);
            if(pwinItem) pwinItem->Show(true);
            pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
            if(pwinItem) pwinItem->Show(true);
            pwinItem=FindWindowById(ID_BUTTON_PARKEDORDER, this);
            if(pwinItem) pwinItem->Show(true);
            if(pwinItem) pwinItem->Enable(true);
            pwinItem=FindWindowById(ID_BUTTON_MARKETPRICE, this);
            if(pwinItem) pwinItem->Show(true);
            if(pwinItem) pwinItem->Enable(true);
            pwinItem=FindWindowById(ID_BUTTON_BATCHORDER, this);
            if(pwinItem) pwinItem->Show(false);
            pwinItem=FindWindowById(ID_BUTTON_CONDITIONORDER, this);
            if(pwinItem) pwinItem->Show(false);
            pwinItem=FindWindowById(ID_BUTTON_POSITIONORDER, this);
            if(pwinItem) pwinItem->Show(false);
            pwinItem=FindWindowById(ID_BUTTON_STOPORDER, this);
            if(pwinItem) pwinItem->Show(false);
            break;
        case 1:		// 批量下单时
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_BATCHORDER, this);
			if(pwinItem) pwinItem->Show(true);
			m_arrTabCtrl.push_back(ID_BUTTON_BATCHORDER);
			m_arrTabCtrl.push_back(ID_BUTTON_CANCEL);
			break;
		case 2:		// 条件单时
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_CONDITIONORDER, this);
			if(pwinItem) pwinItem->Show(true);
			m_arrTabCtrl.push_back(ID_BUTTON_CONDITIONORDER);
			m_arrTabCtrl.push_back(ID_BUTTON_CANCEL);
			break;
		case 3:		// 盈损单时
			pwinItem=FindWindowById(ID_BUTTON_OK, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_PARKEDORDER, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_MARKETPRICE, this);
			if(pwinItem) pwinItem->Show(true);
			m_arrTabCtrl.push_back(ID_BUTTON_OK);
			m_arrTabCtrl.push_back(ID_BUTTON_CANCEL);
			m_arrTabCtrl.push_back(ID_BUTTON_PARKEDORDER);
			m_arrTabCtrl.push_back(ID_BUTTON_MARKETPRICE);
			break;
		case 4:		// 停止单时
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_STOPORDER, this);
			if(pwinItem) pwinItem->Show(true);
			m_arrTabCtrl.push_back(ID_BUTTON_STOPORDER);
			m_arrTabCtrl.push_back(ID_BUTTON_CANCEL);
			break;
		case 5:		// FOKFAK
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->Show(true);
			pwinItem=FindWindowById(ID_BUTTON_FAKFOKORDER, this);
			if(pwinItem) pwinItem->Show(true);
			m_arrTabCtrl.push_back(ID_BUTTON_FAKFOKORDER);
			m_arrTabCtrl.push_back(ID_BUTTON_CANCEL);
			break;
		};
	}
	else {
		pwinItem=FindWindowById(ID_BUTTON_PATSORDER, this);
		if(pwinItem) pwinItem->Show(true);
		pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
		if(pwinItem) pwinItem->Show(true);
		m_arrTabCtrl.push_back(ID_BUTTON_PATSORDER);
		m_arrTabCtrl.push_back(ID_BUTTON_CANCEL);
	}

	// 然后按风格调整大小

	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		switch(nViewStyle) {
		case 0:		// 嵌入面板时
			ctlSz.Set(80+5, 21);
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->SetSize(ctlSz);
			break;
		case 1:		// 批量下单时
		case 2:		// 条件单时
		case 4:		// 停止单时
		case 5:		// FAKFOK单时
			ctlSz.Set(80+5, 44);
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->SetSize(ctlSz);
			break;
		case 3:		// 盈损单时
			ctlSz.Set(80+5, 21);
			pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
			if(pwinItem) pwinItem->SetSize(ctlSz);
			break;
		};
	}
	else {
		ctlSz.Set(80+5, 44);
		pwinItem=FindWindowById(ID_BUTTON_CANCEL, this);
		if(pwinItem) pwinItem->SetSize(ctlSz);
	}

	if(DEFAULT_SVR()&&!DEFAULT_SVR()->HaveOrderType(UIOT_CMD_PARKED)) 
    {
		wxWindow* pWin1 = FindWindowById(ID_BUTTON_MARKETPRICE, this);
		wxWindow* pWin2 = FindWindowById(ID_BUTTON_PARKEDORDER, this);
		wxWindow* pWin3 = FindWindowById(ID_BUTTON_CANCEL, this);
		pWin2->Show(false);
		pWin1->SetPosition(pWin2->GetPosition());
		pWin1->SetSize(pWin3->GetSize());
	}

}

void COrderInsertCommandPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;

	pWin = FindWindowById(ID_BUTTON_OK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_NEW_ORDER));
	pWin = FindWindowById(ID_BUTTON_PATSORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_NEW_ORDER));
	pWin = FindWindowById(ID_BUTTON_CANCEL, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_CANCEL));

	pWin = FindWindowById(ID_BUTTON_BATCHORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_BATCH));
	pWin = FindWindowById(ID_BUTTON_CONDITIONORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_CONDITION));
	pWin = FindWindowById(ID_BUTTON_POSITIONORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_PROFIT_ORDER));
	pWin = FindWindowById(ID_BUTTON_STOPORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP_ORDER));
	pWin = FindWindowById(ID_BUTTON_FAKFOKORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_NEW_ORDER));
	pWin = FindWindowById(ID_BUTTON_PARKEDORDER, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_PARK));
	pWin = FindWindowById(ID_BUTTON_MARKETPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_MARKET));
}

void COrderInsertCommandPanel::ViewStyleSwitch_DoStop(BOOL bIsStop)
{
	wxWindow* itemButton = NULL;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(bIsStop) {
			itemButton = FindWindowById(ID_BUTTON_OK, this);
			itemButton->Show(false);
			itemButton = FindWindowById(ID_BUTTON_POSITIONORDER, this);
			itemButton->Show(true);
			itemButton = FindWindowById(ID_BUTTON_PARKEDORDER, this);
			itemButton->Enable(false);
			itemButton = FindWindowById(ID_BUTTON_MARKETPRICE, this);
			itemButton->Enable(false);
		}
		else {
			itemButton = FindWindowById(ID_BUTTON_OK, this);
			itemButton->Show(true);
			itemButton = FindWindowById(ID_BUTTON_POSITIONORDER, this);
			itemButton->Show(false);
			itemButton = FindWindowById(ID_BUTTON_PARKEDORDER, this);
			itemButton->Enable(true);
			itemButton = FindWindowById(ID_BUTTON_MARKETPRICE, this);
			itemButton->Enable(true);
		}
	}
}