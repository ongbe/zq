#include "StdAfx.h"
#include "MAInfoSynSetupDlg.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#include "../inc/MultiAccountCore/MAConnSocket.h"
#include "../inc/Module-Misc/CryptoAPI.h"
char txtPrompt[]=
"1.用户资料保存后，下次从该位置的加载将自动填写所有保存的数据，免于重复填写的繁琐，但存在一定风险，请谨慎。\n"
"2.每次保存操作，将覆盖原数据且不可恢复\n"
"3.退出保存设置将在每次退出系统时，自动保存多账号里的用户资料到指定位置。启动加载设置将在每次客户端登录时自动从指定位置加载用户资料。\n";
BEGIN_EVENT_TABLE(CMAInfoSynSetupDlg,wxDialog)
	EVT_BUTTON(BTN_SAVE,OnBtnSave)
	EVT_BUTTON(BTN_LOAD,OnBtnLoad)
	EVT_BUTTON(BTN_DEL_LOCAL,OnDelLocal)
	EVT_BUTTON(BTN_DEL_SERVER,OnDelServer)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_SOCKET,OnMASocket)
END_EVENT_TABLE()
CMAInfoSynSetupDlg::CMAInfoSynSetupDlg(wxWindow *parent):
wxDialog(parent,wxID_ANY,"多账号用户资料存储设置",wxDefaultPosition,wxSize(500,300))
{
	wxChoice* pChoice=NULL;
	wxArrayString savestring,loadstring;
	savestring.Add("资料不保存");
	savestring.Add("资料(含密码)保存到本地");
	savestring.Add("资料(含密码)保存到服务器");
	savestring.Add("资料(不含密码)保存到本地");
	savestring.Add("资料(不含密码)保存到服务器");
	loadstring.Add("不加载");
	loadstring.Add("从本地加载");
	loadstring.Add("从服务器加载");
	new wxStaticText(this,wxID_STATIC,txtPrompt,wxPoint(5,5),wxSize(490,80), 0 );
//////////////////////////////////////////////////////////////////////////////////
	int box_x=5,box_y=90;
	new wxStaticBox( this, wxID_STATIC,"保存设置", wxPoint(box_x,box_y), wxSize(240,100), 0 );
	new wxStaticText(this,wxID_STATIC,"退出保存设置:",wxPoint(box_x+5,110),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_AUTO_SAVE,wxPoint(box_x+85,110), wxSize(150,20),savestring, 0 );
    pChoice->SetSelection(MA_CORE()->GetSaveTypeAuto());
	new wxStaticText(this,wxID_STATIC,"手动保存设置:",wxPoint(box_x+5,140),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_MAN_SAVE,wxPoint(box_x+85,140), wxSize(150,20),savestring, 0 );
	pChoice->SetSelection(MA_CORE()->GetSaveTypeManual());
	new wxButton( this, BTN_SAVE,"手动保存",wxPoint(box_x+110,165), wxSize(60,24), 0 );
//////////////////////////////////////////////////////////////////////////////////
	box_x=250,box_y=90;
	new wxStaticBox( this, wxID_STATIC,"加载设置", wxPoint(250,box_y), wxSize(240,100), 0 );
	new wxStaticText(this,wxID_STATIC,"启动加载设置:",wxPoint(box_x+5,110),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_AUTO_LOAD,wxPoint(box_x+85,110), wxSize(150,20),loadstring, 0 );
    pChoice->SetSelection(MA_CORE()->GetLoadTypeAuto());
	new wxStaticText(this,wxID_STATIC,"手动加载设置:",wxPoint(box_x+5,140),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_MAN_LOAD,wxPoint(box_x+85,140), wxSize(150,20), loadstring, 0 );
	pChoice->SetSelection(MA_CORE()->GetLoadTypeManual());
	new wxButton( this, BTN_LOAD,"手动加载",wxPoint(box_x+110,165), wxSize(60,24), 0 );
////////////////////////////////////////////////////////////////////////////////////////
	box_x=5,box_y=200;
	new wxStaticBox( this, wxID_STATIC,"手动删除设置", wxPoint(box_x,box_y), wxSize(480,50), 0 );
	new wxButton( this, BTN_DEL_LOCAL,"删除本地资料",wxPoint(box_x+5,220), wxSize(100,24),  0 );
	new wxButton( this, BTN_DEL_SERVER,"删除服务器资料",wxPoint(box_x+120,220), wxSize(100,24), 0 );
}
CMAInfoSynSetupDlg::~CMAInfoSynSetupDlg(void)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(CMB_AUTO_SAVE,this);
	if(pChoice) MA_CORE()->SetSaveTypeAuto(pChoice->GetSelection());
	pChoice=(wxChoice*)FindWindowById(CMB_AUTO_LOAD,this);
	if(pChoice) MA_CORE()->SetLoadTypeAuto(pChoice->GetSelection());
	pChoice=(wxChoice*)FindWindowById(CMB_MAN_SAVE,this);
	if(pChoice) MA_CORE()->SetSaveTypeManual(pChoice->GetSelection());
	pChoice=(wxChoice*)FindWindowById(CMB_MAN_LOAD,this);
	if(pChoice) MA_CORE()->SetLoadTypeManual(pChoice->GetSelection());
}
void CMAInfoSynSetupDlg::OnBtnSave(wxCommandEvent& event)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(CMB_MAN_SAVE,this);
	if(pChoice==NULL) return;
	switch(pChoice->GetSelection())
	{
	case 1:
		MA_CORE()->SaveDataToLocal(true);
		break;
	case 2:
		MA_CORE()->SaveDataToSvr(true,this);
		break;
	case 3:
		MA_CORE()->SaveDataToLocal(false);
		break;
	case 4:
		MA_CORE()->SaveDataToSvr(false,this);
		break;
	}
}
void CMAInfoSynSetupDlg::OnBtnLoad(wxCommandEvent& event)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(CMB_MAN_LOAD,this);
	if(pChoice==NULL) return;
	switch(pChoice->GetSelection())
	{
	case 1:
		{
			MA_CORE()->LoadFromLocal();
			wxCommandEvent evt(wxEVT_MA_LIST_UPDATE);
			GetParent()->AddPendingEvent(evt);
		}
		break;
	case 2:
		MA_CORE()->LoadFromSvr(this);
		break;
	}

}
void CMAInfoSynSetupDlg::OnDelLocal(wxCommandEvent& event)
{
		MA_CORE()->DelLocal();
}
void CMAInfoSynSetupDlg::OnDelServer(wxCommandEvent& event)
{
		MA_CORE()->DelServer();
}
void CMAInfoSynSetupDlg::OnMASocket(wxCommandEvent& event)
{
	RecvData* pRecvData=(RecvData*)event.GetClientData();
	if(pRecvData==NULL)return;
	if(pRecvData->head.cmdid==Cmd_MA_QryTradeAccount_Rsp)
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			MA_CORE()->ClearAllTradeAccount();
			//保存账号信息
			int count=pRecvData->head.len/sizeof(TradeAccount);
			for (int i=0;i<count;++i)
			{
				CTPTradeAccount info;
				ZeroMemory(&info,sizeof(info));
				info.t=*(TradeAccount*)((char*)pRecvData->pData+sizeof(TradeAccount)*i);
				CCryptoAPI::Decode3DES_ToString(info.t.szPassword);
				MA_CORE()->AddTradeAccount(info);
			}
			wxCommandEvent evt(wxEVT_MA_LIST_UPDATE);
			GetParent()->AddPendingEvent(evt);
			wxMessageBox("获取服务器登录账号信息成功",LOADSTRING(USERLOGINDLG_INFORMATION));
		}
		else
		{
			wxMessageBox(wxString::Format("获取服务器登录账号信息失败\n%s",(char*)(pRecvData->pData)),LOADSTRING(USERLOGINDLG_INFORMATION));
		}
	}
	else if(pRecvData->head.cmdid==Cmd_MA_SaveTradeAccount_Rsp)
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			wxMessageBox("保存账号信息成功",LOADSTRING(USERLOGINDLG_INFORMATION));
		}
		else
		{
			wxMessageBox(wxString::Format("保存账号信息失败\n%s",(char*)(pRecvData->pData)),LOADSTRING(USERLOGINDLG_INFORMATION));
		}
	}
	CMAConnSocket::GetInstance()->UnRegisteWindow(this);
}