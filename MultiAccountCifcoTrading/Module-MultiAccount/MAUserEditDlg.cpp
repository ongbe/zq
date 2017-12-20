#include "StdAfx.h"
#include "MAUserEditDlg.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
BEGIN_EVENT_TABLE(CMAUserEditDlg,wxDialog)
	EVT_CHOICE(ID_CMB_BROKER,OnBrokerChoice)
	EVT_BUTTON(wxID_OK,OnOK)
	EVT_BUTTON(wxID_CANCEL,OnCancel)
END_EVENT_TABLE()
CMAUserEditDlg::CMAUserEditDlg(const CTPTradeAccount* acc,wxWindow *parent):
wxDialog(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(270,245)),
bIsEditDlg(false)
{

	int nBrokerID=-1,nServerNo=-1;
	wxString strName,strUserID,strPassword,DynamicPassword;
	if(acc)//编辑
	{
		memcpy(&m_TradeAccount,acc,sizeof(CTPTradeAccount));
		strName=acc->t.szNickName;
		strUserID=acc->t.szTradeAccount;
		strPassword=acc->t.szPassword;
		nBrokerID=acc->t.nBrokerID;
		nServerNo=acc->t.nServerNo;
		DynamicPassword=acc->DynamicPassword;
		SetLabel("编辑账号");
		bIsEditDlg=true;
	}
	else//添加
	{
		ZeroMemory(&m_TradeAccount,sizeof(CTPTradeAccount));
		SetLabel("添加账号");
	}

	wxTextValidator validatorNum( wxFILTER_INCLUDE_CHAR_LIST );
	wxArrayString strArrayNum;
	for (int i=0;i<10;i++) strArrayNum.Add(wxString::Format("%c",'0'+i));
	validatorNum.SetIncludes(strArrayNum);
	wxTextValidator validatorAlphaNum( wxFILTER_ALPHANUMERIC );

	new wxStaticText(this,wxID_STATIC,"用户名:",wxPoint(20,22),wxSize(70,20), 0 );
	new wxTextCtrl(this,ID_NAME,strName,wxPoint(90,20),wxSize(150,20),0);
	new wxStaticText(this,wxID_STATIC,"用户账号:",wxPoint(20,47),wxSize(70,20), 0);
	wxTextCtrl* pTextCtrl=new wxTextCtrl(this,ID_USERID,strUserID,wxPoint(90,45),wxSize(150,20),0,validatorNum);
	if(pTextCtrl)pTextCtrl->Enable(!acc);
	new wxStaticText(this,wxID_STATIC,"密码:",wxPoint(20,72),wxSize(70,20), 0 );
	new wxTextCtrl(this,ID_PASSWORD,strPassword,wxPoint(90,70),wxSize(150,20),wxTE_PASSWORD, validatorAlphaNum);

	new wxStaticText(this,wxID_STATIC,"动态密码:",wxPoint(20,97),wxSize(70,20), 0 );
	new wxTextCtrl(this,ID_DYNAMICPASSWORD,DynamicPassword,wxPoint(90,95),wxSize(150,20),wxTE_PASSWORD, validatorAlphaNum);
	new wxStaticText(this,wxID_STATIC,"经纪公司:",wxPoint(20,122),wxSize(70,20), 0 );

	((wxTextCtrl*)FindWindowById(ID_NAME,this))->SetMaxLength(20);
	((wxTextCtrl*)FindWindowById(ID_USERID,this))->SetMaxLength(20);
	((wxTextCtrl*)FindWindowById(ID_PASSWORD,this))->SetMaxLength(12);
	((wxTextCtrl*)FindWindowById(ID_DYNAMICPASSWORD,this))->SetMaxLength(12);

	int count=MA_CORE()->m_vecBrokerInfo.size();
	wxArrayString items;
	wxString strItem;
	int selindex=-1;
	for (int i=0;i<count;++i)
	{
		strItem=MA_CORE()->m_vecBrokerInfo[i].szCompany;
		strItem+="(";
		strItem+=MA_CORE()->m_vecBrokerInfo[i].szCode;
		if(nBrokerID==MA_CORE()->m_vecBrokerInfo[i].nBrokerID)
			selindex=i;
		strItem+=")";
		items.Add(strItem);
	}
	wxChoice* pcmbBroker=new wxChoice( this, ID_CMB_BROKER,wxPoint(90,120), wxSize(150,20), items, 0 );
	pcmbBroker->SetSelection(selindex);
	new wxStaticText(this,wxID_STATIC,"服务器组:",wxPoint(20,147),wxSize(70,20), 0 );
	new wxChoice( this, ID_CMB_SVRGROUP,wxPoint(90,145), wxSize(150,20), NULL, 0 );
	SelectSvrGroupOfBrokerID(selindex>=0?MA_CORE()->m_vecBrokerInfo[selindex].nBrokerID:-1,nServerNo);
	new wxButton( this, wxID_OK,"确定",wxPoint(50,175), wxSize(60,24), 0 );
	new wxButton( this, wxID_CANCEL,"取消",wxPoint(160,175), wxSize(60,24), 0 );
}

CMAUserEditDlg::~CMAUserEditDlg(void)
{
}

void CMAUserEditDlg::SelectSvrGroupOfBrokerID( int BrokerID ,int nSvrNo )
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(ID_CMB_SVRGROUP,this);
	if(pChoice)
	{
		pChoice->Clear();
		multimap<int,ServerGroup>::iterator it1,it2;
		it1=MA_CORE()->m_mapServerGroup.lower_bound(BrokerID);
		it2=MA_CORE()->m_mapServerGroup.upper_bound(BrokerID);
		for(int i=0;it1!=it2;it1++,i++)
		{
			ServerGroup& grp=it1->second;
			pChoice->Append(grp.szServerName,(void*)grp.nServerNo);	
			if(nSvrNo==grp.nServerNo)
				pChoice->SetSelection(i);
		}
	}
}
void CMAUserEditDlg::OnBrokerChoice(wxCommandEvent& event)
{
	int sel=event.GetInt();
	vector<BrokerInfo>& vec=MA_CORE()->m_vecBrokerInfo;
	if(sel>=0&&sel<(int)vec.size())
		SelectSvrGroupOfBrokerID(vec[sel].nBrokerID,-1);
}
void CMAUserEditDlg::OnOK(wxCommandEvent& event)
{
	wxString strName=((wxTextCtrl*)FindWindowById(ID_NAME,this))->GetValue();
	wxString strUserID=((wxTextCtrl*)FindWindowById(ID_USERID,this))->GetValue();
	wxString strPassword=((wxTextCtrl*)FindWindowById(ID_PASSWORD,this))->GetValue();
	if(strName.IsEmpty()||strUserID.IsEmpty()||strPassword.IsEmpty())
	{
		wxMessageBox("请输入用户名、用户账号、密码",LOADSTRING(USERLOGINDLG_INFORMATION));
		return ;
	}

	if ( strName.Length() < 4 || strName.Length() > 20)
	{
		wxMessageBox("用户名必须为4~20位字符");
		return;
	}

	if ( strUserID.Length() < 4 || strUserID.Length() > 20)
	{
		wxMessageBox("用户账号必须为4~20位字符");
		return;
	}

	if ( strPassword.Length() < 6 || strPassword.Length() > 12)
	{
		wxMessageBox("密码必须为6~12位字符");
		return;
	}
	wxChoice* p1=(wxChoice*)FindWindowById(ID_CMB_BROKER,this);
	wxChoice* p2=(wxChoice*)FindWindowById(ID_CMB_SVRGROUP,this);
	if(p1->GetSelection()<0||p2->GetSelection()<0)
	{
		wxMessageBox("请选择经纪公司和服务器组",LOADSTRING(USERLOGINDLG_INFORMATION));
		return ;
	}
	if(!bIsEditDlg&&MA_CORE()->FindTradeAccount(strUserID)>=0)
	{
		wxMessageBox("账号已经存在",LOADSTRING(USERLOGINDLG_INFORMATION));
		return ;
	}
	event.Skip();

}
void CMAUserEditDlg::OnCancel(wxCommandEvent& event)
{
	event.Skip();
}
void CMAUserEditDlg::GetTradeAccount(CTPTradeAccount& acc)
{
	memcpy(&acc,&m_TradeAccount,sizeof(CTPTradeAccount));
	wxString strName=((wxTextCtrl*)FindWindowById(ID_NAME,this))->GetValue();
	wxString strUserID=((wxTextCtrl*)FindWindowById(ID_USERID,this))->GetValue();
	wxString strPassword=((wxTextCtrl*)FindWindowById(ID_PASSWORD,this))->GetValue();
	wxString DynPassword=((wxTextCtrl*)FindWindowById(ID_DYNAMICPASSWORD,this))->GetValue();
	wxChoice* p1=(wxChoice*)FindWindowById(ID_CMB_BROKER,this);
	wxChoice* p2=(wxChoice*)FindWindowById(ID_CMB_SVRGROUP,this);
	strcpy_s(acc.t.szNickName,strName.c_str());
	strcpy_s(acc.t.szTradeAccount,strUserID.c_str());
	strcpy_s(acc.t.szPassword,strPassword.c_str());
	strcpy_s(acc.DynamicPassword,DynPassword.c_str());
	vector<BrokerInfo>& vec=MA_CORE()->m_vecBrokerInfo;
	int sel=p1->GetSelection();
	if(sel>=0&&sel<(int)vec.size())
		acc.t.nBrokerID=vec[sel].nBrokerID;
	sel=p2->GetSelection();
	if(sel>=0)
		acc.t.nServerNo=(int)p2->GetClientData(sel);
	//获取动态密码

}