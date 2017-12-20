#include "StdAfx.h"
#ifdef _USE_TOP_SPLITTER
#include "Algorithm"
#include <wx/aui/auibook.h> 
#include "NewsInfoPanel.h"
#include "IEHtmlWin.h"
#include "SOAPTitle.h"
#include "cfgMgr/CfgMgr.h"


#define BORDER_SIZE 5
#define LIST_TOP	60


extern wxArrayString g_HttpUrls;
int g_category[]=
{
	26506,//网上交易终端
	26528,//中期铜罗盘
	26525,//中期天胶王
	26526//中期棉花通
};
wxString g_Nameprev[]=
{
	"",
	"铜罗盘",
	"天胶通",
	"棉花通"
};
wxString g_GuideFilter[]=
{
	"",
	"中期铜罗盘专家团",
	"中期天胶通专家团",
	"中期棉花通专家团"
};
int  g_NewsInfoId[]=
{
	0,
	121,
	116,
	110
};
//////////////////////////////////////////////////////////////////////////
//CNewsInfoChild
BEGIN_EVENT_TABLE(CNewsInfoChild,wxPanel)
	EVT_SIZE( CNewsInfoChild::OnSize )
	EVT_BUTTON(wxID_ANY,CNewsInfoChild::OnGetList)
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY, CNewsInfoChild::OnActivated)
	EVT_COMMAND(wxID_ANY, wxEVT_SOAP_MSG,OnSoapMsg)
	EVT_TIMER(ID_TIMER_NEWS_INFO_CHILD_PANEL, CNewsInfoChild::OnTimer)
	EVT_CLOSE(OnClose)
END_EVENT_TABLE()

CNewsInfoChild::CNewsInfoChild(wxWindow *parent,
					wxWindowID winid ,
					const wxPoint& pos ,
					const wxSize& size ,
					long style,
					const wxString& name ):
				    wxPanel(parent,winid,pos,size,style,name),m_timer(this,ID_TIMER_NEWS_INFO_CHILD_PANEL)
{
	m_ListCtrl=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLC_REPORT|wxLC_NO_HEADER|wxLC_VRULES|wxLC_HRULES);

	if (m_ListCtrl)
	{
		wxImageList * m_imageListSmall = new wxImageList(16, 16, true);;
		m_imageListSmall->Add(wxIcon(_T("zqICON_LIST")));
		m_ListCtrl->SetImageList(m_imageListSmall, wxIMAGE_LIST_SMALL);

		wxListItem itemCol;
		itemCol.SetText(_T("Column 1"));
		itemCol.SetImage(0);
		m_ListCtrl->InsertColumn(0, itemCol);
		m_ListCtrl->SetColumnWidth(0,100);

		//m_ListCtrl->InsertColumn(0,"",wxLIST_FORMAT_LEFT,100);	
	}


	m_buttonpos=wxPoint(BORDER_SIZE,BORDER_SIZE);
	m_buttonindex=0;
	m_pThread=NULL;
    SetBackgroundColour(DEFAULT_COLOUR); 
	DoQry();
	//m_timer.Start(TIMER_INTERVAL);
}
void CNewsInfoChild::OnClose(wxCloseEvent& evt)
{
	//m_timer.Stop();
	StopTimer();
}

void CNewsInfoChild::OnTimer( wxTimerEvent& event)
{
	DoQry();
	WXLOG_INFO("execute CNewsInfoChild::OnTimer!");
}
void CNewsInfoChild::DoQry()
{
	if(g_NewsInfoId[VIDEOINFO_TYPE]>0)
		m_buttonid=wxString::Format("%d",g_NewsInfoId[VIDEOINFO_TYPE]);
	else if (m_buttonid.IsEmpty())
		return;
	CResearchReportThread * pThread=new CResearchReportThread(this,g_HttpUrls[UrlIndex_ARTICLE]);
	pThread->SetParam("1","",m_buttonid,"50");
	if(pThread->Create()== wxTHREAD_NO_ERROR)
	{
		pThread->Run();
	}

}

void CNewsInfoChild::AddButton(int id,wxString name)
{
	wxButton* pBtn=NULL;
	WXLOG_INFO("%x---(%d,%d)",this,m_buttonpos.x,m_buttonpos.y);
	pBtn=new wxButton(this,id,name,m_buttonpos);
	if(m_buttonindex&&m_buttonindex%4==3)
	{
		m_buttonpos.y+=pBtn->GetEffectiveMinSize().y+BORDER_SIZE;
		m_buttonpos.x=BORDER_SIZE;
	}
	else
	{
		m_buttonpos.x+=pBtn->GetEffectiveMinSize().x+BORDER_SIZE;
	}
	pBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
	m_buttonindex++;
}


void CNewsInfoChild::OnSize(wxSizeEvent& evt)
{
	if(m_ListCtrl==NULL)return;	
	wxSize sz=GetClientSize();
	if(VIDEOINFO_TYPE >0)
		m_ListCtrl->SetSize(sz);
	else
		m_ListCtrl->SetSize(BORDER_SIZE,LIST_TOP,sz.x-2*BORDER_SIZE,sz.y-LIST_TOP-BORDER_SIZE);
	m_ListCtrl->SetColumnWidth(0,sz.x-40);

}
void CNewsInfoChild::OnActivated(wxListEvent& event)
{
	long index=event.GetIndex();	
	if(index>=0 && m_ListCtrl)
	{
		long key = m_ListCtrl->GetItemData(index);
		wxString strId=wxString::Format("%d",key);
		SoapGuideArticleThread* pThread=new SoapGuideArticleThread(this,g_HttpUrls[UrlIndex_ARTICLE],strId);
		if(pThread->Create()== wxTHREAD_NO_ERROR)
		{
			pThread->Run();
		}
	}
}


void CNewsInfoChild::OnSoapMsg(wxCommandEvent& evt)
{
	if(evt.GetExtraLong()>=0)
	{
		std::vector<int>::iterator iter = find(m_vecLast.begin(),m_vecLast.end(),evt.GetInt());
		if (iter == m_vecLast.end())
		{
			wxString str = evt.GetString();
			int curindex=m_ListCtrl->InsertItem(m_ListCtrl->GetItemCount(),evt.GetString());
			m_ListCtrl->SetItemData(curindex,evt.GetInt());
			m_vecLast.push_back(evt.GetInt());
		}
	}
	else
	{
		DetailContentDlg dlg(GETTOPWINDOW());
		dlg.SetTitle(((wxStringClientData*)evt.GetClientObject())->GetData());
		dlg.GetIEHtmlWin()->LoadString(evt.GetString());
		dlg.ShowModal();
	}
}


void CNewsInfoChild::OnGetList(wxCommandEvent& evt)
{

	int oldbtn=atoi(m_buttonid);
	if (oldbtn != evt.GetId() && oldbtn != 0)
	{
		m_ListCtrl->DeleteAllItems();
	}
    CResearchReportThread* m_pThread=new CResearchReportThread(this,g_HttpUrls[UrlIndex_ARTICLE]);
	m_buttonid=wxString::Format("%d",evt.GetId());
	m_pThread->SetParam("1","",m_buttonid,"50");
    if(m_pThread->Create()== wxTHREAD_NO_ERROR)
    {
		wxButton* pBtn=wxDynamicCast(evt.GetEventObject(),wxButton);
		if(pBtn)pBtn->SetBackgroundColour(*wxRED);
        wxWindow* pBtnOld=FindWindowById(oldbtn,this);
		if(pBtnOld)pBtnOld->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
        m_pThread->Run();
    }
}
DetailContentDlg::DetailContentDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(800,600),wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
	m_ie = new wxIEHtmlWin(this);
	m_ie->SetSize(GetClientRect());
}
DetailContentDlg::~DetailContentDlg()
{
}
BEGIN_EVENT_TABLE(DetailContentDlg,wxDialog)
	EVT_SIZE( DetailContentDlg::OnSize )
END_EVENT_TABLE()
void DetailContentDlg::OnSize(wxSizeEvent& evt)
{
	if(m_ie)
	{
		m_ie->SetSize(GetClientRect());
	}
}
//CNewsInfoPanel
CNewsInfoPanel::CNewsInfoPanel(wxWindow *parent,
					wxWindowID winid ,
					const wxPoint& pos ,
					const wxSize& size ,
					long style,
					const wxString& name ):
				    wxPanel(parent,winid,pos,size,style,name),m_pChild_industry(NULL)

{
	m_InfoNotebook = NULL;
	InitInfoNotebook();
	Getlist();

}

CNewsInfoPanel::~CNewsInfoPanel(void)
{

}
BEGIN_EVENT_TABLE(CNewsInfoPanel,wxPanel)
	EVT_SIZE( CNewsInfoPanel::OnSize )
END_EVENT_TABLE()

void CNewsInfoPanel::OnSize(wxSizeEvent& evt)
{

	wxSize sz=GetClientSize();
	if(m_InfoNotebook)
	{
		m_InfoNotebook->SetSize(sz);
	}
}
void CNewsInfoPanel::InitInfoNotebook()
{
	if(VIDEOINFO_TYPE >0)
	{
		m_InfoNotebook=new wxAuiNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
		m_pChild_industry=new CNewsInfoChild (this);
		m_InfoNotebook->AddPage(m_pChild_industry,g_Nameprev[VIDEOINFO_TYPE]+"资讯");
		m_pChild_Direct=new CResearchReportInfoPanel(m_InfoNotebook,g_HttpUrls[UrlIndex_ARTICLE]);
		m_InfoNotebook->AddPage(m_pChild_Direct,g_Nameprev[VIDEOINFO_TYPE]+"研究报告");
	}
	else
	{
		m_InfoNotebook=new wxAuiNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
		CNewsInfoChild* pChild_farm=new CNewsInfoChild(m_InfoNotebook);
		pChild_farm->AddButton(105,"大豆");
			pChild_farm->AddButton(106,"豆油");
		pChild_farm->AddButton(113,"棕榈油");
		//pChild_farm->AddButton(1043,"黄大豆2号");
		pChild_farm->AddButton(106,"豆粕");
		pChild_farm->AddButton(108,"玉米");
		pChild_farm->AddButton(117,"聚乙烯");
		//pChild_farm->AddButton(2061,"豆类");
		pChild_farm->AddButton(114,"PVC");
		pChild_farm->AddButton(119,"焦炭");
		m_InfoNotebook->AddPage(pChild_farm,"大商所");
		CNewsInfoChild* pChild_industry=new CNewsInfoChild(m_InfoNotebook);
		pChild_industry->AddButton(111,"白砂糖");	
		pChild_industry->AddButton(110,"棉花");	
		pChild_industry->AddButton(112,"菜籽油");
		pChild_industry->AddButton(109,"小麦");
		pChild_industry->AddButton(115,"PTA");		
		pChild_industry->AddButton(104,"早籼稻");
		m_InfoNotebook->AddPage(pChild_industry,"郑商所");
		CNewsInfoChild* pChild_metal=new CNewsInfoChild(m_InfoNotebook);
		//pChild_metal->AddButton(1420,"线材");
		pChild_metal->AddButton(1035,"铜");
		pChild_metal->AddButton(1038,"黄金");
		pChild_metal->AddButton(1037,"锌");
		pChild_metal->AddButton(1036,"铝");
		pChild_metal->AddButton(1040,"天然橡胶");
		pChild_metal->AddButton(1039,"燃料油");
		//pChild_metal->AddButton(1200,"螺纹钢");
		//pChild_metal->AddButton(2062,"钢材");
		pChild_metal->AddButton(1200,"钢材");
		pChild_metal->AddButton(2080,"铅");
		m_InfoNotebook->AddPage(pChild_metal,"上期所");
		CNewsInfoChild* pChild_finance=new CNewsInfoChild(m_InfoNotebook);
		pChild_finance->AddButton(1056,"沪深300指数期货");
		m_InfoNotebook->AddPage(pChild_finance,"中金所");

		m_vecChildPanels.push_back(pChild_farm);
		m_vecChildPanels.push_back(pChild_industry);
		m_vecChildPanels.push_back(pChild_metal);
		m_vecChildPanels.push_back(pChild_finance);
	}
}

void CNewsInfoPanel::StartTimer()
{
	if(VIDEOINFO_TYPE >0)
	{
		if (m_pChild_industry)
		{
			m_pChild_industry->StartTimer();
		}
		if (m_pChild_Direct)
		{
			m_pChild_Direct->StartTimer();
		}
	}
	else
	{
		std::vector<CNewsInfoChild*>::iterator iter = m_vecChildPanels.begin();
		for (; iter != m_vecChildPanels.end(); ++iter)
		{
			CNewsInfoChild *& tmp = *iter;
			if (tmp != NULL)
			{
				tmp->StartTimer();
			}
		}
	}

}

void CNewsInfoPanel::StopTimer()
{
	if(VIDEOINFO_TYPE >0)
	{
		if (m_pChild_industry)
		{
			m_pChild_industry->StopTimer();
		}
		if (m_pChild_Direct)
		{
			m_pChild_Direct->StopTimer();
		}
	}
	else
	{
		std::vector<CNewsInfoChild*>::iterator iter = m_vecChildPanels.begin();
		for (; iter != m_vecChildPanels.end(); ++iter)
		{
			CNewsInfoChild *& tmp = *iter;
			if (tmp != NULL)
			{
				tmp->StopTimer();
			}
		}
	}

}

void CNewsInfoPanel::Getlist()
{
	if (m_pChild_industry)
	{
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,g_NewsInfoId[VIDEOINFO_TYPE]);
		m_pChild_industry->ProcessEvent(evt);
	}
}
//////////////////////////////////////////////////////////////////////////
//CResearchReportInfoPanel
BEGIN_EVENT_TABLE(CResearchReportInfoPanel,wxPanel)
EVT_SIZE(CResearchReportInfoPanel::OnSize)
EVT_COMMAND(wxID_ANY, wxEVT_SOAP_MSG,OnSoapMsg)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, CResearchReportInfoPanel::OnActivated)
EVT_TIMER(ID_TIMER_RESEARCH_REPORT_INFO_PANEL, CResearchReportInfoPanel::OnTimer)
EVT_CLOSE(OnClose)
END_EVENT_TABLE()
CResearchReportInfoPanel::CResearchReportInfoPanel(wxWindow *parent,
								 wxString& soapurl,
								 wxWindowID winid ,
								 const wxPoint& pos ,
								 const wxSize& size ,
								 long style,
								 const wxString& name ):
wxPanel(parent,winid,pos,size,style,name),
m_soapurl(soapurl),m_timer(this,ID_TIMER_RESEARCH_REPORT_INFO_PANEL)
{
	m_ListCtrl=new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLC_REPORT|wxLC_NO_HEADER|wxLC_VRULES|wxLC_HRULES);
	if (m_ListCtrl)
	{
		wxImageList * m_imageListSmall = new wxImageList(16, 16, true);;
		m_imageListSmall->Add(wxIcon(_T("zqICON_LIST")));
		m_ListCtrl->SetImageList(m_imageListSmall, wxIMAGE_LIST_SMALL);

		wxListItem itemCol;
		itemCol.SetText(_T("Column 1"));
		itemCol.SetImage(0);
		m_ListCtrl->InsertColumn(0, itemCol);
		m_ListCtrl->SetColumnWidth(0,100);
	}
// 	m_InfoNotebook->AddPage(m_ListCtrl,"交易指导");
	m_pThread=new CResearchReportThread(this,m_soapurl);


	m_pThread=new CResearchReportThread(this,m_soapurl);
	m_pThread->SetParam("2","3103",wxString::Format("%d",g_category[VIDEOINFO_TYPE]),"10");
	if(m_pThread->Create()== wxTHREAD_NO_ERROR)
	{
		m_pThread->Run();
	}
	//m_timer.Start(TIMER_INTERVAL);
}
void CResearchReportInfoPanel::OnClose(wxCloseEvent& evt)
{
	//m_timer.Stop();
	StopTimer();
}
CResearchReportInfoPanel::~CResearchReportInfoPanel(void)
{
}

void CResearchReportInfoPanel::OnTimer( wxTimerEvent& event)
{
	CResearchReportThread* pThread=new CResearchReportThread(this,m_soapurl);

	pThread->SetParam("2","3103",wxString::Format("%d",g_category[VIDEOINFO_TYPE]),"10");
	if(pThread->Create()== wxTHREAD_NO_ERROR)
	{
		pThread->Run();
	}

	WXLOG_INFO("execute CResearchReportInfoPanel::OnTimer!");

}

void CResearchReportInfoPanel::OnSize(wxSizeEvent& evt)
{
	if(/*m_InfoNotebook&&*/m_ListCtrl)
	{
		wxSize sz=GetClientSize();
		//m_InfoNotebook->SetSize(0,0,sz.x,sz.y);
		m_ListCtrl->SetColumnWidth(0,sz.x-40);
		m_ListCtrl->SetSize(sz);
	}
}

void CResearchReportInfoPanel::OnSoapMsg(wxCommandEvent& evt)
{
	if(evt.GetExtraLong()>=0)
	{
		std::vector<int>::iterator iter = find(m_vecLast.begin(),m_vecLast.end(),evt.GetInt());
		if (iter == m_vecLast.end())
		{
			wxString str = evt.GetString();
			int curindex=m_ListCtrl->InsertItem(m_ListCtrl->GetItemCount(),evt.GetString());
			m_ListCtrl->SetItemData(curindex,evt.GetInt());
			m_vecLast.push_back(evt.GetInt());
		}
	}
	else
	{
		DetailContentDlg dlg(GETTOPWINDOW());
		dlg.SetTitle(((wxStringClientData*)evt.GetClientObject())->GetData());
		dlg.GetIEHtmlWin()->LoadString(evt.GetString());
		dlg.ShowModal();
	}

}
void CResearchReportInfoPanel::OnActivated(wxListEvent& event)
{
	long index=event.GetIndex();	
	if(index>=0 && m_ListCtrl)
	{
		long key = m_ListCtrl->GetItemData(index);
		wxString strId=wxString::Format("%d",key);
		SoapGuideArticleThread* pThread=new SoapGuideArticleThread(this,m_soapurl,strId);
		if(pThread->Create()== wxTHREAD_NO_ERROR)
		{
			pThread->Run();
		}
	}
}


#endif//_USE_TOP_SPLITTER