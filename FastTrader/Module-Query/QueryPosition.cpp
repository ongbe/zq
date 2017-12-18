#include "stdafx.h"
#include "QueryPosition.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

CQueryPositionChild::CQueryPositionChild(wxWindow *parent,int type)
:wxPanel(parent, wxID_ANY),
m_pWxExtListCtrl(NULL),
m_type(type)
{
	m_pWxExtListCtrl=new wxExtListCtrl(this, wxID_ANY,wxDefaultPosition ,GetClientSize(),wxLCEXT_REPORT|wxLCEXT_VRULES);
	wxArrayString HeaderArray;
	if(type==2||type==3)
	{
		HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL1));
		HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL2));
		HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL3));
	}
	HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL4));
	HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL5));
	HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL6));
	HeaderArray.Add(LOADSTRING(QUERYPOSITION_COL7));
	wxExtListItem col;
	col.SetWidth(100);
	for(int i=0; i<(int)HeaderArray.size();++i)
	{  
		if(i==0)
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
		col.SetText(wxString(HeaderArray[i]));
		m_pWxExtListCtrl->InsertColumn(i,col);
	}
	switch(type)
	{
	case 1:
		CreateGUIControls_Total(); 
		break;
	case 2:
		CreateGUIControls_Open(); 
		break;
	case 3:
		CreateGUIControls_Contract(); 
		break;
	}

}

CQueryPositionChild::~CQueryPositionChild()
{
   
}
BEGIN_EVENT_TABLE(CQueryPositionChild,wxPanel)
	EVT_SIZE( CQueryPositionChild::OnSize )
END_EVENT_TABLE()
void CQueryPositionChild::OnSize(wxSizeEvent& evt)
{

	wxSize sz=GetClientSize();
	if(m_pWxExtListCtrl)
	{
		m_pWxExtListCtrl->SetSize(sz);
	}
}
void CQueryPositionChild::CreateGUIControls_Total()
{
	vector<string> accountList;
	accountList.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetAccountList(accountList);
	PlatformStru_AccountPosition posi;
	for(int i=0,RowId=0;i<(int)accountList.size();i++) 
	{
		memset(&posi,0,sizeof(PlatformStru_AccountPosition));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetTotalPosition(accountList[i],&posi))
		{
			m_pWxExtListCtrl->InsertItem(RowId,wxString(accountList[i]));
			m_pWxExtListCtrl->SetItem(RowId,1,GlobalFunc::ConvertToString(atof(posi.Profit))); 
			m_pWxExtListCtrl->SetItem(RowId,2,GlobalFunc::ConvertToString(posi.Buys)); 
			m_pWxExtListCtrl->SetItem(RowId,3,GlobalFunc::ConvertToString(posi.Sells)); 
			RowId++;
		}
	}

}
void CQueryPositionChild::CreateGUIControls_Open()
{
	vector<string> accountList;
	accountList.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetAccountList(accountList);
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetContract(m_vectorContract);
	PlatformStru_AccountPosition posi;
	int RowId=0;
	for(int i=0;i<(int)accountList.size();i++) 
	{
		for(int j=0;j<(int)m_vectorContract.size();j++)
		{
			memset(&posi,0,sizeof(PlatformStru_AccountPosition));
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetOpenPosition(m_vectorContract[j],accountList[i],&posi))
			{
				m_pWxExtListCtrl->InsertItem(RowId,wxString(m_vectorContract[j].ExchangeName));
				m_pWxExtListCtrl->SetItem(RowId,1,wxString(m_vectorContract[j].ContractName)); 
				m_pWxExtListCtrl->SetItem(RowId,2,wxString(m_vectorContract[j].ContractDate)); 
				m_pWxExtListCtrl->SetItem(RowId,3,wxString(accountList[i])); 
				m_pWxExtListCtrl->SetItem(RowId,4,GlobalFunc::ConvertToString(atof(posi.Profit))); 
				m_pWxExtListCtrl->SetItem(RowId,5,GlobalFunc::ConvertToString(posi.Buys)); 
				m_pWxExtListCtrl->SetItem(RowId,6,GlobalFunc::ConvertToString(posi.Sells)); 
				RowId++;
			}
		}

	}
}
void CQueryPositionChild::CreateGUIControls_Contract()
{
	vector<string> accountList;
	accountList.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetAccountList(accountList);
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetContract(m_vectorContract);
	PlatformStru_AccountPosition posi;
	int RowId=0;
	for(int i=0;i<(int)accountList.size();i++) 
	{
		for(int j=0;j<(int)m_vectorContract.size();j++)
		{
			memset(&posi,0,sizeof(PlatformStru_AccountPosition));
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetContractPosition(m_vectorContract[j],accountList[i],&posi))
			{
				m_pWxExtListCtrl->InsertItem(RowId,wxString(m_vectorContract[j].ExchangeName));
				m_pWxExtListCtrl->SetItem(RowId,1,wxString(m_vectorContract[j].ContractName)); 
				m_pWxExtListCtrl->SetItem(RowId,2,wxString(m_vectorContract[j].ContractDate)); 
				m_pWxExtListCtrl->SetItem(RowId,3,wxString(accountList[i])); 
				m_pWxExtListCtrl->SetItem(RowId,4,GlobalFunc::ConvertToString(atof(posi.Profit))); 
				m_pWxExtListCtrl->SetItem(RowId,5,GlobalFunc::ConvertToString(posi.Buys)); 
				m_pWxExtListCtrl->SetItem(RowId,6,GlobalFunc::ConvertToString(posi.Sells)); 
				RowId++;
			}
		}

	}
}
BEGIN_EVENT_TABLE(CQueryPositionDlg,wxDialog)
	EVT_SIZE( CQueryPositionDlg::OnSize )
END_EVENT_TABLE()
CQueryPositionDlg::CQueryPositionDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(QUERY_POSITION),wxDefaultPosition, wxSize(800,280),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER),
m_notebook(NULL)
{
	CreateGUIControls();

}

CQueryPositionDlg::~CQueryPositionDlg()
{
   
}
void CQueryPositionDlg::OnSize(wxSizeEvent& evt)
{
	wxSize sz=GetClientSize();
	if(m_notebook)
	{
		m_notebook->SetSize(sz);
	}
}
void CQueryPositionDlg::CreateGUIControls()
{
	m_notebook=new wxNotebook(this,wxID_ANY,wxDefaultPosition,GetClientSize(),wxBK_TOP);
	m_notebook->AddPage(new CQueryPositionChild(m_notebook,1),LOADSTRING(QUERYPOSITION_TAB1));
	m_notebook->AddPage(new CQueryPositionChild(m_notebook,2),LOADSTRING(QUERYPOSITION_TAB2));
	m_notebook->AddPage(new CQueryPositionChild(m_notebook,3),LOADSTRING(QUERYPOSITION_TAB3));
}