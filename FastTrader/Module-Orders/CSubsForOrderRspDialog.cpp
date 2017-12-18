#include "StdAfx.h"
#include "CSubsForOrderRspDialog.h"
#include "wx\odcombo.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Misc2/EventParam.h"
#include "../inc/Module-Misc2/KernelStruct.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


BEGIN_EVENT_TABLE(CSubsForOrderRspDialog, wxDialog)

    EVT_BUTTON(ID_CANCEL, OnBtnClose)

	END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CSubsForOrderRspDialog::CSubsForOrderRspDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name)
{
	const int nFW = 8, nFH = 16;
	const int nVW = nFW+2, nVH = nFH+2;

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	//oldfont.SetPointSize(9);
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);
	
	wxWindow* pWin = NULL;

	{		// 所有期权合约

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(所有期权合约), wxPoint(10, 10), wxSize(nFW*12, nFH));

	wxArrayString arrString;
	vector<string> outData;
	// 获取产品列表
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetOptionInstrumentIDsByUnderlyingInstrumentID(string(), outData);
	for(int i=0; i<(int)outData.size(); ++i) {
		arrString.Add(wxString(outData[i].c_str()));
	}
	
    wxListBox* pListAll = new wxListBox( this, ID_LIST_ALL, wxPoint(10, 10+nFH), wxSize(170, 350), arrString, wxLB_MULTIPLE|wxLB_SORT);
	}

	{		// 已选期权合约

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(已选期权合约), wxPoint(10+170+60, 10), wxSize(nFW*12, nFH));

	wxArrayString arrString;
	
    wxListBox* pListAll = new wxListBox( this, ID_LIST_SELECT, wxPoint(10+170+60, 10+nFH), wxSize(170, 350), arrString, wxLB_MULTIPLE|wxLB_SORT);
	}

	{
	wxButton* pBtn = new wxButton( this, ID_SUBSCRIBE, "-->", wxPoint(10+170+10, 15+nVH+130), wxSize(nFW*5, nFH*3/2), 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnAppend), NULL, this);

	pBtn = new wxButton( this, ID_SUBSCRIBEALL, "<--", wxPoint(10+170+10, 15+nVH+160), wxSize(nFW*5, nFH*3/2), 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnRemove), NULL, this);

	pBtn = new wxButton( this, ID_SUBSCRIBE, LOADSTRING("订阅"), wxPoint(10+45+60, 15+nVH+350), wxSize(nFW*5, nFH*3/2), 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnSubscribe), NULL, this);

	pBtn = new wxButton( this, ID_SUBSCRIBEALL, LOADSTRING("全部订阅"), wxPoint(10+45+60+nFW*6, 15+nVH+350), wxSize(nFW*9, nFH*3/2), 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnSubscribeAll), NULL, this);

	pBtn = new wxButton( this, ID_UNSUBSCRIBE, LOADSTRING("退订"), wxPoint(10+45+60+nFW*6+nFW*10, 15+nVH+350), wxSize(nFW*5, nFH*3/2), 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnUnSubscribe), NULL, this);

	pBtn = new wxButton( this, ID_UNSUBSCRIBEALL, LOADSTRING("全部退订"), wxPoint(10+45+60+nFW*6+nFW*10+nFW*6, 15+nVH+350), wxSize(nFW*9, nFH*3/2), 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnUnSubscribeAll), NULL, this);

	pBtn = new wxButton( this, ID_CANCEL, LOADSTRING(BTN_CLOSE_TEXT), wxPoint(10+45+60+nFW*6+nFW*10+nFW*6+nFW*10, 15+nVH+350), wxSize(nFW*5, nFH*3/2), 0 );	//取消
	pBtn->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CSubsForOrderRspDialog::OnBtnClose), NULL, this);
	}

	ResetLanguage();

	SetSize(wxSize(20+170*2+60+5, 350+90));

	this->CenterOnScreen();

}

CSubsForOrderRspDialog::~CSubsForOrderRspDialog()
{
}

CSubsForOrderRspDialog* CSubsForOrderRspDialog::CreateDailog(wxWindow* parent)
{
    return new CSubsForOrderRspDialog(parent, wxID_ANY, wxT("期权询价订阅退订"));
}

void CSubsForOrderRspDialog::ResetLanguage()
{
}

void CSubsForOrderRspDialog::OnBtnAppend(wxCommandEvent& evt)
{
	set<string> oldSelect;
    wxListBox* pListSelect = (wxListBox*)FindWindowById(ID_LIST_SELECT,this);
    wxListBox* pListAll = (wxListBox*)FindWindowById(ID_LIST_ALL,this);
	if(pListSelect == NULL || pListAll == NULL)
		return;

	for(int i=0; i<pListSelect->GetCount(); i++)
		oldSelect.insert(pListSelect->GetString(i).c_str());

	for(int i=0; i<pListAll->GetCount(); i++) {
		if(pListAll->Selected(i)) 
			oldSelect.insert(pListAll->GetString(i).c_str());
	}

	wxArrayString arrString;
	for(set<string>::iterator it = oldSelect.begin(); it != oldSelect.end(); it++) {
		arrString.Add(wxString((*it).c_str()));
	}

	pListSelect->Clear();
	pListSelect->Append(arrString);
	
}

void CSubsForOrderRspDialog::OnBtnRemove(wxCommandEvent& evt)
{
    wxListBox* pListSelect = (wxListBox*)FindWindowById(ID_LIST_SELECT,this);
	if(pListSelect == NULL)
		return;

	int i = pListSelect->GetCount()-1;
	while(i>=0) {
		if(pListSelect->Selected(i)) {
			pListSelect->Delete(i);
		}
		i--;
	}
}

void CSubsForOrderRspDialog::OnBtnSubscribe(wxCommandEvent& evt)
{
	if(!DEFAULT_SVR())
		return;

	vector<string> oldSelect;
    wxListBox* pListSelect = (wxListBox*)FindWindowById(ID_LIST_SELECT,this);
	if(pListSelect == NULL)
		return;

	for(int i=0; i<pListSelect->GetCount(); i++)
		oldSelect.push_back(pListSelect->GetString(i).c_str());

	if(oldSelect.size()>0) {
		DEFAULT_SVR()->SubscribeForQuoteRsp(oldSelect);
	}
}

void CSubsForOrderRspDialog::OnBtnSubscribeAll(wxCommandEvent& evt)
{
	if(DEFAULT_SVR())
		DEFAULT_SVR()->SubscribeForQuoteRsp(vector<string>());
}

void CSubsForOrderRspDialog::OnBtnUnSubscribe(wxCommandEvent& evt)
{
	if(!DEFAULT_SVR())
		return;

	vector<string> oldSelect;
    wxListBox* pListSelect = (wxListBox*)FindWindowById(ID_LIST_SELECT,this);
	if(pListSelect == NULL)
		return;

	for(int i=0; i<pListSelect->GetCount(); i++)
		oldSelect.push_back(pListSelect->GetString(i).c_str());

	if(oldSelect.size()>0) {
		DEFAULT_SVR()->UnSubscribeForQuoteRsp(oldSelect);
	}
}

void CSubsForOrderRspDialog::OnBtnUnSubscribeAll(wxCommandEvent& evt)
{
	if(DEFAULT_SVR())
		DEFAULT_SVR()->UnSubscribeForQuoteRsp(vector<string>());
}

void CSubsForOrderRspDialog::OnBtnClose(wxCommandEvent& evt)
{
	EndModal(wxID_CANCEL);
}
