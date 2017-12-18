#include "StdAfx.h"
#include "../inc/Module-Position/COptionExecOrderDialog.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "..\\cfgMgr\\CfgMgr.h"
#include "..\inc\Module-Misc\constants.h"
#include "wx\odcombo.h"
//#include "../inc/Module-Misc2/tools_util.h"
//#include "../inc/Module-Misc2/EventParam.h"
//#include "../inc/Module-Misc2/KernelStruct.h"

extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


BEGIN_EVENT_TABLE(COptionExecOrderDialog, wxDialog)

	EVT_BUTTON(ID_CANCEL, OnBtnClose)

END_EVENT_TABLE()
////////////////////////////////////////////////////////////////////////////////////


COptionExecOrderDialog::COptionExecOrderDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name)
{
	const int nFW = 8, nFH = 20;
	const int nVW = nFW+2, nVH = nFH+4;

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	//oldfont.SetPointSize(9);
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);
	
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;


	{		// 合约ID
	pWin = new wxStaticText( this, ID_INSTRUMENTID_STATIC, LOADSTRING(合约), ctlPos, wxSize(nFW*64, nFH));
	}

	{		// 执行类型
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(数量), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;
		
	wxTextCtrl* pTxt = new wxTextCtrl(this, ID_VOLUME_TEXT, wxEmptyString, 
			wxPoint(ctlPos.x, ctlPos.y), wxSize(nFW*8, nFH));
	}

	{		// 执行类型
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(执行类型), ctlPos, wxSize(nFW*8, nFH));

	wxArrayString strArrList;
	strArrList.Add(wxT("执行"));
	strArrList.Add(wxT("放弃"));
	
	ctlPos.x += nFW*8;
		
	wxOwnerDrawnComboBox* pCmb = new wxOwnerDrawnComboBox(this, ID_ACTIONTYPE_COMB, wxT(""), 
			wxPoint(ctlPos.x, ctlPos.y), wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);//wxCB_DROPDOWN);//|wxCB_SORT);
	pCmb->SetSelection(0);
	}

	{			//开平
	ctlPos.x += nFW*8;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(开平), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;

	wxArrayString strArrList;
	strArrList.Add(wxT("开仓"));
	strArrList.Add(wxT("平仓"));
	strArrList.Add(wxT("平今"));
	strArrList.Add(wxT("平昨"));

	wxOwnerDrawnComboBox* pCmb = new wxOwnerDrawnComboBox(this, ID_OFFSETFLAG_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmb->SetSelection(0);
	}

	{			//保留头寸申请的持仓方向
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(保留方向), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;

	wxArrayString strArrList;
	strArrList.Add(wxT("多头"));
	strArrList.Add(wxT("空头"));

	wxOwnerDrawnComboBox* pCmb = new wxOwnerDrawnComboBox(this, ID_POSIDIRECTION_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmb->SetSelection(0);
	}

	{			//期权行权后是否保留期货头寸的标记
	ctlPos.x += nFW*8;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(是否保留), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;

	wxArrayString strArrList;
	strArrList.Add(wxT("保留"));
	strArrList.Add(wxT("不保留"));

	wxOwnerDrawnComboBox* pCmb = new wxOwnerDrawnComboBox(this, ID_RESERVEPOSITIONFLAG_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmb->SetSelection(0);
	}

	{			//期权行权后生成的头寸是否自动平仓
	ctlPos.y += nVH;
	ctlPos.x = 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(自动平仓), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;

	wxArrayString strArrList;
	strArrList.Add(wxT("自动平仓"));
	strArrList.Add(wxT("不自动平仓"));

	wxOwnerDrawnComboBox* pCmb = new wxOwnerDrawnComboBox(this, ID_CLOSEFLAG_COMB, wxT(""), 
			ctlPos, wxSize(nFW*12, nFH), strArrList, wxCB_READONLY);
	pCmb->SetSelection(0);
	}

	{		// 用法提示
	ctlPos.y += nVH;
	ctlPos.x = 5;
	pWin = new wxStaticText( this, ID_INSTRUMENTID_STATIC, LOADSTRING(保留方向: 多头\n是否保留: 中金所选不保留，大商所郑商选保留\n自动平仓:	中金所选自动平仓，大商所郑商选不自动平仓), ctlPos, wxSize(nFW*64, nVH*2));
	}

	{
	ctlPos = wxPoint(280 - 150, 175);
	ctlSz.Set(60, 26);
	wxButton* pBtn = new wxButton( this, ID_OK, LOADSTRING("提交"), ctlPos, ctlSz, 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(COptionExecOrderDialog::OnBtnOrder), NULL, this);

	ctlPos = wxPoint(280 - 150 + 70, 175);
	ctlSz.Set(60, 26);
	pBtn = new wxButton( this, ID_CANCEL, LOADSTRING(BTN_CLOSE_TEXT), ctlPos, ctlSz, 0 );	//取消
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(COptionExecOrderDialog::OnBtnClose), NULL, this);
	}

	ResetLanguage();

	SetSize(wxSize(350, 5+nVH*9+15));

	this->CenterOnScreen();

}

COptionExecOrderDialog::~COptionExecOrderDialog()
{
}

COptionExecOrderDialog* COptionExecOrderDialog::CreateDailog(wxWindow* parent)
{
    return new COptionExecOrderDialog(parent, wxID_ANY, wxT("期权行权"));
}

void COptionExecOrderDialog::ResetLanguage()
{
}

void COptionExecOrderDialog::OnBtnOrder(wxCommandEvent& evt)
{
	if(DEFAULT_SVR()) {
		PlatformStru_InputExecOrderField inputExecOrder;

		strncpy(inputExecOrder.InstrumentID, m_Position.InstrumentID, sizeof(inputExecOrder.InstrumentID)-1);
		inputExecOrder.HedgeFlag = m_Position.HedgeFlag;
		wxTextCtrl* pVolumeTxt = (wxTextCtrl*)FindWindowById(ID_VOLUME_TEXT, this);
		if(pVolumeTxt != NULL) 
			inputExecOrder.Volume = atoi(pVolumeTxt->GetValue().c_str());

		wxOwnerDrawnComboBox* pCmbActionType = (wxOwnerDrawnComboBox*)FindWindowById(ID_ACTIONTYPE_COMB, this);
		if(pCmbActionType != NULL)
			inputExecOrder.ActionType = THOST_FTDC_ACTP_Exec+pCmbActionType->GetSelection();

		wxOwnerDrawnComboBox* pCmbOffsetFlag = (wxOwnerDrawnComboBox*)FindWindowById(ID_OFFSETFLAG_COMB, this);
		if(pCmbOffsetFlag != NULL) {
			inputExecOrder.OffsetFlag = THOST_FTDC_OF_Open+pCmbOffsetFlag->GetSelection();
			if(pCmbOffsetFlag->GetSelection()>1)
				inputExecOrder.OffsetFlag += 1;
		}

		wxOwnerDrawnComboBox* pCmbPosiDirection = (wxOwnerDrawnComboBox*)FindWindowById(ID_POSIDIRECTION_COMB, this);
		if(pCmbPosiDirection != NULL)
			inputExecOrder.PosiDirection = THOST_FTDC_PD_Long+pCmbPosiDirection->GetSelection();

		wxOwnerDrawnComboBox* pCmbReservePositionFlag = (wxOwnerDrawnComboBox*)FindWindowById(ID_RESERVEPOSITIONFLAG_COMB, this);
		if(pCmbReservePositionFlag != NULL)
			inputExecOrder.ReservePositionFlag = THOST_FTDC_EOPF_Reserve+pCmbReservePositionFlag->GetSelection();

		wxOwnerDrawnComboBox* pCmbCloseFlag = (wxOwnerDrawnComboBox*)FindWindowById(ID_CLOSEFLAG_COMB, this);
		if(pCmbCloseFlag != NULL)
			inputExecOrder.CloseFlag = THOST_FTDC_EOCF_AutoClose+pCmbCloseFlag->GetSelection();

		DEFAULT_SVR()->ReqExecOrderInsert(inputExecOrder, 0);
	}

	EndModal(wxID_OK);
}

void COptionExecOrderDialog::OnBtnClose(wxCommandEvent& evt)
{
	EndModal(wxID_CANCEL);
}
