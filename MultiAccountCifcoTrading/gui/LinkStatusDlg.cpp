
#include "stdafx.h"
#include <list>
#include "LinkStatusDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

LinkStatusDlg* g_LinkStatusDlg=NULL;
BEGIN_EVENT_TABLE(LinkStatusDlg,wxDialog)
EVT_SIZE(LinkStatusDlg::OnSize)
EVT_COMMAND(wxID_ANY, wxEVT_CONNECTIVITY_STATUS,OnConnectivityStatus)
END_EVENT_TABLE()
LinkStatusDlg::LinkStatusDlg(wxWindow *parent):
            wxDialog(parent, wxID_ANY,LOADSTRING(GUI_LINK_STATUS),wxDefaultPosition, wxSize(620,350),wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxDIALOG_MODELESS )

{
	DIALOG_CREATE_LOG();
	g_LinkStatusDlg=this;
	bHasData=false;
	m_pList=new wxExtListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(616,346),wxLIST_VIEW);
	m_ListHeaders.push_back("GUI_TYPE");
	m_ListHeaders.push_back("GUI_NAME");
	m_ListHeaders.push_back("GUI_COMMEN");
	m_ListHeaders.push_back("GUI_STATUS");
	m_ListHeaders.push_back("GUI_SERVERITY");	

	for(int i=0;i<(int)m_ListHeaders.size();i++)
	{
		m_pList->InsertColumn(i,LOADSTRING_TEXT(m_ListHeaders[i]),wxLISTEXT_FORMAT_LEFT,i==2?200:100);
	}
    if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_ConnectivityStatus,GID_ANY,ConnectivityStatusCallBackFunc);

}
void LinkStatusDlg::OnSize(wxSizeEvent& event)
{
	if(m_pList)
	{
		m_pList->SetSize(event.GetSize()); 
	}
	event.Skip();

}
void LinkStatusDlg::ChangeLanguage()
{
	if(m_pList)
	{
		wxExtListItem item;
		for(int col=0;col<5;col++)
		{
			if(m_pList->GetColumn(col,item))
			{
				item.SetText(LOADSTRING_TEXT(m_ListHeaders[col]));
				m_pList->SetColumn(col,item);
			}
		}
	}
	SetLabel(LOADSTRING(GUI_LINK_STATUS));

}
char DeviceType[][16]={"","Exchange","ORE","TAS","ESA","MD","SARA","Client","BOF","TSF"};
char Status[][16]={"","Running","Closed","Initialising","Offline"};
char Severity[][16]={"","Information","Warning","Attention","Urgent","Fatal"};
void LinkStatusDlg::OnConnectivityStatus(wxCommandEvent& event)
{
    DataConnectivityStatus* pTmp= (DataConnectivityStatus*)(event.GetClientData());
    if(!pTmp) return;
	bHasData=true;
	ConnectivityStatus&  StatusField=pTmp->StatusField; 
	wxString strType=(atoi(StatusField.DeviceType)==1)?StatusField.ExchangeID:StatusField.DeviceType;
	bool bFind=false;
	int indexpos=0;
	for(int i=m_pList->GetItemCount()-1;i>=0;i--)
	{
		if(strType==*((wxString*)m_pList->GetItemData(i)))
		{
			indexpos=atoi(StatusField.DeviceType);
			if(indexpos<0||indexpos>=10) indexpos=0;
			m_pList->SetItem(i,0,DeviceType[indexpos]);
			m_pList->SetItem(i,1,StatusField.DeviceName);
			m_pList->SetItem(i,2,StatusField.Commentary);
			indexpos=atoi(StatusField.Status);
			if(indexpos<0||indexpos>=5) indexpos=0;
			m_pList->SetItem(i,3,Status[indexpos]);
			indexpos=atoi(StatusField.Severity);
			if(indexpos<0||indexpos>=6) indexpos=0;
			m_pList->SetItem(i,4,Severity[indexpos]);
			bFind=true;
			break;
		}
	}
	if(!bFind)
	{
		indexpos=atoi(StatusField.DeviceType);
		if(indexpos<0||indexpos>=10) indexpos=0;
		int index=m_pList->InsertItem(m_pList->GetItemCount(),DeviceType[indexpos]);
		m_pList->SetItem(index,1,StatusField.DeviceName);
		m_pList->SetItem(index,2,StatusField.Commentary);
		indexpos=atoi(StatusField.Status);
		if(indexpos<0||indexpos>=5) indexpos=0;
		m_pList->SetItem(index,3,Status[indexpos]);
		indexpos=atoi(StatusField.Severity);
		if(indexpos<0||indexpos>=6) indexpos=0;
		m_pList->SetItem(index,4,Severity[indexpos]);
		m_pList->SetItemData(index,(long)new wxString(strType));
	}
    wxDELETE(pTmp);
    event.SetClientData(NULL);
}
int LinkStatusDlg::ConnectivityStatusCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_ConnectivityStatus || data.Size!=sizeof(DataConnectivityStatus))
		return 0;	
	DataConnectivityStatus* pTmp=new DataConnectivityStatus();
	memcpy(pTmp, &data, sizeof(DataConnectivityStatus));
	wxCommandEvent evt(wxEVT_CONNECTIVITY_STATUS);
	evt.SetClientData(pTmp);
	if(g_LinkStatusDlg)
		g_LinkStatusDlg->AddPendingEvent(evt);


	return 0;
}