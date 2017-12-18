#include "StdAfx.h"
#ifdef _USE_TOP_SPLITTER
#include "VideoInfoPanel.h"
#include <wx/aui/auibook.h> 
#include "NewsInfoPanel.h"
#include "IEHtmlWin.h"

extern wxArrayString g_HttpUrls;
int g_UrlIndex[]=
{
	UrlIndex_ALL,
    UrlIndex_COPPER,
	UrlIndex_RUBBER,
	UrlIndex_COTTON
};
//////////////////////////////////////////////////////////////////////////
#include "wininet.h"
const int MAXBLOCKSIZE = 1024;
void  Download_thread(std::string xmlfilename)
{

	HINTERNET hSession  =  InternetOpen( "FastTrader" , INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL,  0 );
	if  (hSession  !=  NULL)
	{
		//设置超时
		DWORD n = 3000;
		InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &n, sizeof(n));

		//char   * Url  =   "http://video.cifco.net/video/video!cxml.shtml" ;
		//char   * Url  =   "http://123" ;
		HINTERNET handle2  =  InternetOpenUrl(hSession, g_HttpUrls[g_UrlIndex[VIDEOINFO_TYPE]], NULL,  0 , INTERNET_FLAG_DONT_CACHE,  0 );

		if  (handle2  !=  NULL)
		{
			byte  Temp[MAXBLOCKSIZE];
			ULONG Number  =   1 ;

			FILE  * stream;
			if ( (stream  =  fopen(  xmlfilename.c_str(),  " wb "  ))  !=  NULL ) 
			{
				while  (Number  >   0)
				{
					InternetReadFile(handle2, Temp, MAXBLOCKSIZE  -   1 ,  & Number);
					fwrite(Temp,  sizeof  ( char ), Number , stream);
				}
				fclose( stream );

				WXLOG_INFO("VideoConfigFile is downloaded!");
				WXLOG_INFO("download link: %s\n", g_HttpUrls[g_UrlIndex[VIDEOINFO_TYPE]]);

			}

			InternetCloseHandle(handle2);
			handle2  =  NULL;
		}
		else
		{
			WXLOG_INFO("Url is not find!");
		}

		InternetCloseHandle(hSession);
		hSession  =  NULL;
	}
	else
	{
		WXLOG_INFO("internet do not connect!");
	}

} 
//////////////////////////////////////////////////////////////////////////
//CVideoInfoPanel
BEGIN_EVENT_TABLE(CVideoInfoPanel,wxPanel)
EVT_SIZE(CVideoInfoPanel::OnSize)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, CVideoInfoPanel::OnActivated)
EVT_TIMER(ID_TIMER_VIDEO_INFO_PANEL, CVideoInfoPanel::OnTimer)
EVT_CLOSE(OnClose)
END_EVENT_TABLE()


//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

CVideoInfoPanel::CVideoInfoPanel(wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name),m_timer(this,ID_TIMER_VIDEO_INFO_PANEL)
{

	m_InfoNotebook=new wxAuiNotebook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP);
	m_ListCtrl=new wxListCtrl(m_InfoNotebook,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLC_REPORT|wxLC_NO_HEADER|wxLC_VRULES|wxLC_HRULES);
	m_InfoNotebook->AddPage(m_ListCtrl,"视频速递");
	GlobalFunc::GetPath(m_xmlfilename);
	m_xmlfilename+="VideoNews.shtml";
	Download_thread(m_xmlfilename);
	parseXML(m_xmlfilename);
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
		
		wxFont font(10, wxSCRIPT, wxNORMAL, wxNORMAL, false, wxT("宋体"));

		std::map<long, wxString>::iterator iter = m_mapIDTitle.begin();
		for (int row=0; iter != m_mapIDTitle.end(); ++iter,++row)
		{
			m_ListCtrl->InsertItem(row,iter->second);
			m_ListCtrl->SetItemFont(row, font);
 			m_ListCtrl->SetItemData(row,iter->first);
		}
	}

	//m_timer.Start(TIMER_INTERVAL);
}
void CVideoInfoPanel::OnClose(wxCloseEvent& evt)
{
	//m_timer.Stop();
	StopTimer();
}
CVideoInfoPanel::~CVideoInfoPanel(void)
{
}

void CVideoInfoPanel::OnTimer( wxTimerEvent& event)
{
	WXLOG_INFO("execute CVideoInfoPanel::OnTimer!");
	Download_thread(m_xmlfilename);

	std::map<long, wxString> last = m_mapIDTitle;
	if (!parseXML(m_xmlfilename))
		return;

	if (m_mapIDTitle.size() == last.size())
	{
		return;
	}

	std::map<long, wxString>::iterator iternew = m_mapIDTitle.begin();
	std::map<long, wxString>::iterator iterold = last.begin();
	std::vector<long> vecdel;
	std::vector<long> vecadd;
	int row=0;
	for (;iternew != m_mapIDTitle.end() && iterold != last.end(); ++iternew,++iterold)
	{
		if (iternew->first == iterold->first)
		{
			++row;
			continue;
		}
		else
		{
			vecdel.push_back(row);
			vecadd.push_back(iternew->first);
			++row;
		}
	}


	if (m_ListCtrl)
	{
// 		bool b=false;
// 		int delrow = -1;
// 		int aa;
// 		aa=evt.GetInt();
// 		std::vector<int>::iterator iter = find(m_vecLast.begin(),m_vecLast.end(),aa);
// 		if (iter == m_vecLast.end())
// 		{
// 			int curindex=m_ListCtrl->InsertItem(m_ListCtrl->GetItemCount(),evt.GetString());
// 			m_ListCtrl->SetBackgroundColour(g_ListBKColour);
// 			m_ListCtrl->SetItemData(curindex,evt.GetInt());
// 			aa=m_ListCtrl->GetItemData(curindex);
// 			m_vecLast.push_back(evt.GetInt());
// 		}	

// 		std::map<long, wxString>::iterator iter = m_mapIDTitle.begin();
// 		std::map<long, wxString>::iterator iterLast = last.begin();
// 		for (int row=0; iter != m_mapIDTitle.end(); ++iter,++row)
// 		{
// 			if ()
// 			{
// 			}
// 			m_ListCtrl->GetItemData(row);
// 		}
// 
// 		m_ListCtrl->DeleteAllItems();

		std::vector<long>::iterator iterdel = vecdel.begin();
		for (; iterdel != vecdel.end(); ++iterdel)
		{
			m_ListCtrl->DeleteItem(*iterdel);
		}

		std::vector<long>::iterator iteradd = vecadd.begin();
		for (; iteradd != vecadd.end(); ++iteradd)
		{
			iternew = m_mapIDTitle.find(*iteradd);
			if (iternew != m_mapIDTitle.end())
			{
				wxFont font(10, wxSCRIPT, wxNORMAL, wxNORMAL, false, wxT("宋体"));

				std::map<long, wxString>::iterator iter = m_mapIDTitle.begin();
				int row=m_ListCtrl->GetItemCount();
				for (; iter != m_mapIDTitle.end(); ++iter,++row)
				{
					m_ListCtrl->InsertItem(row,iternew->second);
					m_ListCtrl->SetItemFont(row, font);
					m_ListCtrl->SetItemData(row,iternew->first);
				}
			}
		}

	}

}

void CVideoInfoPanel::OnSize(wxSizeEvent& evt)
{
	if(m_InfoNotebook&&m_ListCtrl)
	{
		wxSize sz=GetClientSize();
		m_InfoNotebook->SetSize(0,0,sz.x,sz.y);
		m_ListCtrl->SetColumnWidth(0,sz.x-40);
	}
}

void CVideoInfoPanel::OnActivated(wxListEvent& event)
{
	long index=event.GetIndex();	
	if(index>=0 && m_ListCtrl)
	{
		long key = m_ListCtrl->GetItemData(index);
		std::map<long,wxString>::iterator iter = m_mapIDTitle.find(key);
		if (iter != m_mapIDTitle.end())
		{
			DetailContentDlg dlg(GETTOPWINDOW());
			dlg.SetTitle("视频速递");
			wxString url;
			url.Printf(g_HttpUrls[UrlIndex_VIDEOPLAYER],iter->first);
			dlg.GetIEHtmlWin()->LoadUrl(url);
			dlg.ShowModal();
		}
	}
}

bool CVideoInfoPanel::parseXML(std::string& xmlfilename)
{
	TiXmlDocument* pXmlDocment = new TiXmlDocument( xmlfilename.c_str());
	if( NULL == pXmlDocment || false == pXmlDocment->LoadFile())
	{
		WXLOG_INFO("视频资讯配置文件未找到！");
		return false;
	}

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return false;
	}

	TiXmlElement *tInstrumentGroupItem = root->FirstChildElement("playlist");
	if(tInstrumentGroupItem == NULL)
		return false;
	
	long val;
	for(TiXmlElement *GroupItem = tInstrumentGroupItem->FirstChildElement("vid"); 
		NULL != GroupItem; 
		GroupItem = GroupItem->NextSiblingElement("vid"))
	{
		const char *Title = GroupItem->Attribute("vTitle");
		char * pTitle = (const_cast<char*>(Title));

		const char *VideoPath = GroupItem->Attribute("vId");
		char * pId = (const_cast<char*>(VideoPath));

		if (pId && pTitle)
		{
			wxString title(U2G(pTitle),wxConvUTF8);
			wxString ID(U2G(pId),wxConvUTF8);
			ID.ToLong(&val);
			m_mapIDTitle.insert( std::make_pair(val,title) );
		}

	}


	return true;
}
#endif