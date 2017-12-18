#include "StdAfx.h"
#include "CForOrderInsertDialog.h"
#include "wx\odcombo.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Misc2/EventParam.h"
#include "../inc/Module-Misc2/KernelStruct.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


BEGIN_EVENT_TABLE(CForOrderInsertDialog, wxDialog)

	EVT_COMMAND(wxID_ANY, wxEVT_CODECLICKED, OnCmbExchange) 
	EVT_COMMAND(wxID_ANY, wxEVT_CODECLICKED, OnCmbTargetID) 
	EVT_COMMAND(wxID_ANY, wxEVT_CODECLICKED, OnCmbExpareDate) 
	EVT_COMMAND(wxID_ANY, wxEVT_CODECLICKED, OnCmbOptionDirection) 
    EVT_BUTTON(ID_CANCEL, OnBtnClose)

	END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CForOrderInsertDialog::CForOrderInsertDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name)
{
	const int nFW = 8, nFH = 20;
	const int nVW = nFW+2, nVH = nFH+8;

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	//oldfont.SetPointSize(9);
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);
	
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;

	ctlPos.y-=nVH/2;

	{		// 交易所
	ctlPos.y += nVH;
	ctlPos.x = 25;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(EXCHANGE), ctlPos, wxSize(nFW*6, nFH));

	wxArrayString strArrList;
	vector<pair<string,vector<PlatformStru_ProductInfo>>> outData;
	// 获取产品列表
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetExchangeID_ProductInfos(outData);
	for(int i=0; i<(int)outData.size(); ++i) {
		strArrList.Add(wxString(outData[i].first.c_str()));
	}
	
	ctlPos.x += nFW*8;
		
	wxOwnerDrawnComboBox* pCmbExchange = new wxOwnerDrawnComboBox(this, ID_EXCHANGE_COMB, wxT(""), 
			wxPoint(ctlPos.x, ctlPos.y), wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);//wxCB_DROPDOWN);//|wxCB_SORT);
	pCmbExchange->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CForOrderInsertDialog::OnCmbExchange), NULL, this);
	}

	{			//标的物品种
	ctlPos.x += nFW*8 + 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(标的物品种), ctlPos, wxSize(nFW*10, nFH));

	ctlPos.x += nFW*10;

	wxArrayString strArrList;

	wxOwnerDrawnComboBox* pCmbTargetID = new wxOwnerDrawnComboBox(this, ID_TARGETPRODUCT_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmbTargetID->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CForOrderInsertDialog::OnCmbTargetID), NULL, this);
	}

	{			//标的物合约
	ctlPos.x += nFW*8 + 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(标的物合约), ctlPos, wxSize(nFW*10, nFH));

	ctlPos.x += nFW*10;

	wxArrayString strArrList;

	wxOwnerDrawnComboBox* pCmbExpireDate = new wxOwnerDrawnComboBox(this, ID_TARGETEXPAIREDATE_COMB, wxT(""), 
			ctlPos, wxSize(nFW*12, nFH), strArrList, wxCB_READONLY);
	pCmbExpireDate->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CForOrderInsertDialog::OnCmbExpareDate), NULL, this);
	}

	{			//期权方向
	ctlPos.x += nFW*12 + 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(方向), ctlPos, wxSize(nFW*4, nFH));

	ctlPos.x += nFW*4;

	wxArrayString strArrList;
	strArrList.Add(wxT("看涨"));
	strArrList.Add(wxT("看跌"));

	wxOwnerDrawnComboBox* pCmbTargetID = new wxOwnerDrawnComboBox(this, ID_OPTIONDIRECTION_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmbTargetID->SetSelection(0);
	pCmbTargetID->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CForOrderInsertDialog::OnCmbOptionDirection), NULL, this);
	}

	{			//期权合约
	ctlPos.y += nVH;
	ctlPos.x = 25;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(期权合约), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;

	wxArrayString strArrList;

	wxOwnerDrawnComboBox* pCmbTargetID = new wxOwnerDrawnComboBox(this, ID_OPTIONINSTRUMENT_COMB, wxT(""), 
			ctlPos, wxSize(nFW*35, nFH), strArrList, wxCB_READONLY);
	}

	{
	ctlPos = wxPoint(650 - 150, 90);
	ctlSz.Set(60, 26);
	wxButton* pBtn = new wxButton( this, ID_OK, LOADSTRING("询价"), ctlPos, ctlSz, 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CForOrderInsertDialog::OnBtnOrder), NULL, this);

	ctlPos = wxPoint(650 - 150 + 70, 90);
	ctlSz.Set(60, 26);
	pBtn = new wxButton( this, ID_CANCEL, LOADSTRING(BTN_CLOSE_TEXT), ctlPos, ctlSz, 0 );	//取消
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CForOrderInsertDialog::OnBtnClose), NULL, this);
	}

	ResetLanguage();

	SetSize(wxSize(650, 150));

	this->CenterOnScreen();

}

CForOrderInsertDialog::~CForOrderInsertDialog()
{
}

CForOrderInsertDialog* CForOrderInsertDialog::CreateDailog(wxWindow* parent)
{
    return new CForOrderInsertDialog(parent, wxID_ANY, wxT("期权询价"));
}

void CForOrderInsertDialog::ResetLanguage()
{
}

void CForOrderInsertDialog::OnCmbExchange(wxCommandEvent& evt)
{
	//vector<pair<string,vector<PlatformStru_ProductInfo>>> outData;
	//// 获取产品列表
	//if(DEFAULT_SVR()) DEFAULT_SVR()->GetExchangeID_ProductInfos(outData);
	//for(int i=0; i<(int)outData.size(); ++i) {
	//	strArrList.Add(wxString(outData[i].first.c_str()));
	//}
	wxOwnerDrawnComboBox* pCmbExchange = (wxOwnerDrawnComboBox*)FindWindowById(ID_EXCHANGE_COMB, this);
	wxString strExchange = pCmbExchange->GetString(pCmbExchange->GetSelection());
	wxOwnerDrawnComboBox* pCmbTargetProduct = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETPRODUCT_COMB, this);
	wxArrayString strArrList;
	vector<string> outProductIDs;

	if(DEFAULT_SVR()) DEFAULT_SVR()->GetOptionProductIDsByExchangeID(string(strExchange.c_str()), outProductIDs);
	for(int i=0; i<(int)outProductIDs.size(); ++i) {
		strArrList.Add(wxString(outProductIDs[i].c_str()));
	}
	pCmbTargetProduct->Clear();
	pCmbTargetProduct->Append(strArrList);

}

void CForOrderInsertDialog::OnCmbTargetID(wxCommandEvent& evt)
{
	wxOwnerDrawnComboBox* pCmbTargetProduct = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETPRODUCT_COMB, this);
	wxString strProduct = pCmbTargetProduct->GetString(pCmbTargetProduct->GetSelection());
	wxOwnerDrawnComboBox* pCmbTargetExpireDate = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETEXPAIREDATE_COMB, this);
	wxArrayString strArrList;
	vector<string> outInstrumentIDs;

	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentListByProductID(string(strProduct.c_str()), outInstrumentIDs);
	for(int i=0; i<(int)outInstrumentIDs.size(); ++i) {
		strArrList.Add(wxString(outInstrumentIDs[i].c_str()));
	}
	pCmbTargetExpireDate->Clear();
	pCmbTargetExpireDate->Append(strArrList);

}

void CForOrderInsertDialog::OnCmbExpareDate(wxCommandEvent& evt)
{
	wxOwnerDrawnComboBox* pCmbTargetExpireDate = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETEXPAIREDATE_COMB, this);
	if(pCmbTargetExpireDate->GetSelection()==-1)
		return;
	wxString strInstrumentID = pCmbTargetExpireDate->GetString(pCmbTargetExpireDate->GetSelection());
	wxOwnerDrawnComboBox* pCmbDirection = (wxOwnerDrawnComboBox*)FindWindowById(ID_OPTIONDIRECTION_COMB, this);
	int nDirection = pCmbDirection->GetSelection();
	wxOwnerDrawnComboBox* pCmbOptionInstrument = (wxOwnerDrawnComboBox*)FindWindowById(ID_OPTIONINSTRUMENT_COMB, this);
	wxArrayString strArrList;
	vector<string> outCallInstrumentIDs;
	vector<string> outPutInstrumentIDs;

	if(DEFAULT_SVR()) DEFAULT_SVR()->GetOptionInstrumentIDsByUnderlyingInstrumentID(
			strInstrumentID.c_str(), outCallInstrumentIDs, outPutInstrumentIDs);
	if(nDirection==0) {
		for(int i=0; i<(int)outCallInstrumentIDs.size(); ++i) {
			strArrList.Add(wxString(outCallInstrumentIDs[i].c_str()));
		}
	}
	else {
		for(int i=0; i<(int)outPutInstrumentIDs.size(); ++i) {
			strArrList.Add(wxString(outPutInstrumentIDs[i].c_str()));
		}
	}
	pCmbOptionInstrument->Clear();
	pCmbOptionInstrument->Append(strArrList);

}

void CForOrderInsertDialog::OnCmbOptionDirection(wxCommandEvent& evt)
{
	OnCmbExpareDate(evt);
}

void CForOrderInsertDialog::OnBtnOrder(wxCommandEvent& evt)
{
	if(!DEFAULT_SVR())
		return;

	wxOwnerDrawnComboBox* pCmbInstrument = (wxOwnerDrawnComboBox*)FindWindowById(ID_OPTIONINSTRUMENT_COMB, this);
	if(pCmbInstrument == NULL)
		return;

	if(pCmbInstrument->GetSelection() < 0) {
		// 提示未选择期权合约
	}
	else {
		wxString strInstrumentID = pCmbInstrument->GetString(pCmbInstrument->GetSelection());
		PlatformStru_InputForQuoteField req;
		strncpy(req.InstrumentID, strInstrumentID.c_str(), sizeof(req.InstrumentID)-1);
		DEFAULT_SVR()->ReqForQuoteInsert(req, 0);
	}
}

void CForOrderInsertDialog::OnBtnClose(wxCommandEvent& evt)
{
	EndModal(wxID_CANCEL);
}
