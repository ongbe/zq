#include "StdAfx.h"
#include "MyFrame.h"


#define  ID_PHRASE_1MIN				23114
#define  ID_PHRASE_5MIN				23115
#define  ID_PHRASE_15MIN			23116
#define  ID_PHRASE_30MIN			23117
#define  ID_PHRASE_60MIN			23118
#define  ID_PHRASE_DAY				23119
#define  ID_PHRASE_WEEK				23120
#define  ID_PHRASE_MONTH			23121


#define  ID_PHRASE_QUIT				23134
#define  ID_DRAW_TRIANGLE_UP		23135
#define  ID_DRAW_TRIANGLE_DOWN		23136

#define  ID_KXIAN					23137
#define  ID_FENSHI					23138




//ISessionIDType	g_SessionID = 0;
//HANDLE	g_hAppPipe = NULL;
//HANDLE	g_hAppPipeThread = NULL;
//BOOL	g_bIsHelloOK=FALSE;
//DWORD	g_dwTryHelloCount = 0;
MyFrame*	g_pAppDlg = NULL;

DEFINE_EVENT_TYPE(wxEVT_Data_InTime)
DEFINE_EVENT_TYPE(wxEVT_GetTime)
DEFINE_EVENT_TYPE(wxEVT_SetLineViewTime)
DEFINE_EVENT_TYPE(wxEVT_ReSend_Message)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
 EVT_MENU(ID_PHRASE_1MIN,	MyFrame::PhraseMin1)
 EVT_MENU(ID_PHRASE_5MIN,	MyFrame::PhraseMin5)
 EVT_MENU(ID_PHRASE_15MIN, MyFrame::PhraseMin15)
 EVT_MENU(ID_PHRASE_30MIN, MyFrame::PhraseMin30)
 EVT_MENU(ID_PHRASE_60MIN, MyFrame::PhraseMin60)
 EVT_MENU(ID_PHRASE_DAY,	MyFrame::PhraseDay)
 EVT_MENU(ID_PHRASE_WEEK,	MyFrame::PhraseWeek)
 EVT_MENU(ID_PHRASE_MONTH, MyFrame::PhraseMonth)
 EVT_MENU(ID_PHRASE_QUIT,			MyFrame::Quit)


 EVT_MENU(ID_DRAW_TRIANGLE_UP,		MyFrame::MarkTriangleUp)
 EVT_MENU(ID_DRAW_TRIANGLE_DOWN,	MyFrame::MarkTriangleDown)

 EVT_MENU(ID_KXIAN,		MyFrame::KXIAN)
 EVT_MENU(ID_FENSHI,	MyFrame::FENSHI)

 EVT_UPDATE_UI(ID_DRAW_TRIANGLE_UP, MyFrame::OnUpdateUI)
 EVT_UPDATE_UI(ID_DRAW_TRIANGLE_DOWN, MyFrame::OnUpdateUI)

 EVT_SIZE(MyFrame::OnSize)
 EVT_CLOSE( MyFrame::OnClose)

 EVT_TIMER(1111, MyFrame::OnTimer1)
 EVT_TIMER(1112, MyFrame::OnTimer1)
 EVT_TIMER(1113, MyFrame::OnTimer1)
 EVT_TIMER(1114, MyFrame::OnTimer1)

 EVT_COMMAND(wxID_ANY, wxEVT_ReSend_Message, MyFrame::OnReSendMessage)

 EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_LEFTDBCLICK,		MyFrame::OnFuncLDBClickKLine)
 EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_KeyDown,			MyFrame::OnFuncKeyDownKLine)

 EVT_COMMAND(wxID_ANY, wxEVT_TCtrl_LEFTDBCLICK,		MyFrame::OnFuncLDBClickTLine)
 EVT_COMMAND(wxID_ANY, wxEVT_TCtrl_KeyDown,			MyFrame::OnFuncKeyDownTLine)

 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_KLINE,	MyFrame::OnFuncReturnKLine)

 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_TLINE,	MyFrame::OnFuncReturnTLine)

 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_BAOJIABIAO,	MyFrame::OnFuncReturnBaoJiaBiao)
 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_BAOJIABIAO2,	MyFrame::OnFuncReturnBaoJiaBiao2)
END_EVENT_TABLE()

void MyFrame::OnClose(wxCloseEvent& evt)
{
	m_pApi->Logout();
	if ( NULL != m_pApi )
	{
		m_pApi->DestroyInstance();
		m_pApi = NULL;
	}
	ClearFData();
	

	m_timerTrans.Stop();
	m_timerClose.Stop();

	if(m_pKLineView)
	{
		delete m_pKLineView;
		m_pKLineView = NULL;
	}
	if(m_pTLineView)
	{
		delete m_pTLineView;
		m_pTLineView = NULL;
	}
	
	if(m_pQPanel)
	{
		delete m_pQPanel;
		m_pQPanel = NULL;
	}
	if(m_splitter)
	{
		delete m_splitter;
		m_splitter = NULL;
	}
	evt.Skip();	
//	Close(true);
	delete this;
	exit(1);	
}
MyFrame::MyFrame(WXHWND hWnd)
: wxFrame(NULL, wxID_ANY, _T("行情"),
		  wxDefaultPosition, wxSize(420, 300),
		  wxCLOSE_BOX| wxNO_FULL_REPAINT_ON_RESIZE)/*,m_timer1(this, 1111)*/,m_timerClose(this,1113), m_timerTrans(this,1114)//,m_timerReconnectPipe(this, 1114)//,m_timerGetTime(this, 1112)
		 //wxRESIZE_BORDER| wxNO_FULL_REPAINT_ON_RESIZE),m_timer1(this, 1111),m_timerGetTime(this, 1112),m_timerClose(this,1113)
		//  wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE),m_timer1(this, 1111),m_timerGetTime(this, 1112),m_timerClose(this,1113)
{
	
	SetIcon(wxIcon(_T("IDI_ZQQUATION")));
	m_parent = hWnd;

	m_timerClose.Start(5000);

	m_nTime = 500;
	m_bInitTimerTrans = false;
	
	m_pKLineView = NULL;	
	m_pTLineView = NULL;
	m_splitter	 = NULL;
	m_pQPanel    = NULL;

	g_pAppDlg = this;	
	m_tmLastTrans.tm_year = 0;

	wxRect rtClient = GetClientRect();
	m_splitter = new KSplitterWindow(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_3D|wxNO_BORDER);
	m_splitter->SetSashGravity(1.0);	
	m_splitter->SetSashSize(1);
	::SetWindowPos((HWND)m_splitter->GetHWND(), HWND_BOTTOM, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE );
	
	
	int nQPanel = 250;
	m_pKLineView = new CKLineView;
	m_pKLineView->CreateDlg(m_splitter, 11121, wxPoint(0,0), wxSize(rtClient.GetWidth()-nQPanel, rtClient.GetHeight()) );
	m_pKLineView->CfgShow(KTYPE_KLINE|KTYPE_VOLUME|KTYPE_SCALES|KTYPE_TECHINDEX);

	m_pTLineView = new CTLineView;
	m_pTLineView->CreateDlg(m_splitter, 11122, wxPoint(0,0), wxSize(rtClient.GetWidth()-nQPanel, rtClient.GetHeight()) );
	m_pTLineView->CfgShow(TTYPE_TLINE|TTYPE_VOLUME|TTYPE_SCALES);
	m_pTLineView->Show();

	m_pQPanel = new CQPanel;
	m_pQPanel->CreateDlg(m_splitter, 11128, wxPoint(rtClient.GetWidth()-nQPanel,0), wxSize(nQPanel, rtClient.GetHeight()) );	
	m_pQPanel->Show();	
	
	m_splitter->SetSize(rtClient.width , rtClient.height);

	
	InitFromCFG();
	m_nTrace = 0;
	GetRegIntValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\zq"),_T("trace"), m_nTrace);

	m_nQueryNum = 0;
	//_asm int 3
	m_pApi = PlusinAPI::GetInstance();
	m_pApi->Init("ZqQuotation", "123", OnRecvApiMsg, (HWND)m_parent, POS_QUOTE, (HWND)m_hWnd);
	m_pApi->Logon();

	m_nRightSpanMiniWidth = 200;
/*	
	m_poTCPDataService = CTCPDataService::GetOnlyOneInstance();
	if(m_poTCPDataService!=NULL)
	{
		m_poTCPDataService->RegisterForm((HWND)m_hWnd);
	}*/

}

MyFrame::~MyFrame()
{
	map<string, vector<SSpan*>*>::iterator it = m_mapTransTime.begin();
	while(it != m_mapTransTime.end())
	{
		map<string, vector<SSpan*>*>::iterator itTemp = it;
		it++;
		vector<SSpan*> *pTemp = itTemp->second;

		for(vector<SSpan *>::iterator it = pTemp->begin(); it != pTemp->end(); it++)
		{
			SSpan *p = *it;
			delete p;
			p= NULL;
		}
		pTemp->erase(pTemp->begin(), pTemp->end());	
		delete pTemp;
		pTemp = NULL;
		m_mapTransTime.erase(itTemp);
	}

	if(m_pTLineView)
	{		
		delete m_pTLineView;		
		m_pTLineView = NULL;
	}
	if(m_pKLineView)
	{		
		delete m_pKLineView;		
		m_pKLineView = NULL;
	}
	if(m_pQPanel)
	{		
		delete m_pQPanel;
		m_pQPanel = NULL;
	}
	if(m_splitter)
	{		
		delete m_splitter;
		m_splitter = NULL;
	}
}

void MyFrame::Quit(wxCommandEvent& WXUNUSED(event) )
{	
	ClearFData();
	Close(true);
}
void MyFrame::ClearFData()
{
	map<string, CFData*>::iterator it = m_mapID2FData.begin();
	while(it != m_mapID2FData.end())
	{
		map<string, CFData*>::iterator itTemp = it;
		it++;
		CFData *pTemp = itTemp->second;
		if(pTemp)
		{
			pTemp->SaveFile_TLine();	
			pTemp->SaveFile();	
			pTemp->SaveFile_QIntime();
			delete pTemp;
			pTemp = NULL;
		}
		m_mapID2FData.erase(itTemp);
	}	
}
void MyFrame::PhraseMin1(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_1MIN);
	CheckPhraseUI(PHRASE_1MIN);
}
void MyFrame::PhraseMin5(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_5MIN);
	CheckPhraseUI(PHRASE_5MIN);
}
void MyFrame::PhraseMin15(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_15MIN);
	CheckPhraseUI(PHRASE_15MIN);
}
void MyFrame::PhraseMin30(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_30MIN);
	CheckPhraseUI(PHRASE_30MIN);
}
void MyFrame::PhraseMin60(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_60MIN);
	CheckPhraseUI(PHRASE_60MIN);
}
void MyFrame::PhraseDay(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_DAY);
	CheckPhraseUI(PHRASE_DAY);
}
void MyFrame::PhraseWeek(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_WEEK);
	CheckPhraseUI(PHRASE_WEEK);
}
void MyFrame::PhraseMonth(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->SetPhrase(PHRASE_MONTH);	
	CheckPhraseUI(PHRASE_MONTH);
}
void MyFrame::MarkTriangleUp(wxCommandEvent& event)
{

	GetMenuBar()->Check(ID_DRAW_TRIANGLE_DOWN, false);
	if(m_pKLineView)
		m_pKLineView->SetMarkType(MARK_TRIANGLE_UP);
}
void MyFrame::MarkTriangleDown(wxCommandEvent& event)
{
	GetMenuBar()->Check(ID_DRAW_TRIANGLE_UP, false);
	if(m_pKLineView)
		m_pKLineView->SetMarkType(MARK_TRIANGLE_DOWN);
}

void MyFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
	switch (event.GetId())
	{
	case ID_DRAW_TRIANGLE_UP:
		break;
	case ID_DRAW_TRIANGLE_DOWN:
		break;
	}
}
void MyFrame::CheckPhraseUI(int nID)
{
	if(nID != ID_PHRASE_1MIN)
		GetMenuBar()->Check(ID_PHRASE_1MIN, false);

	if(nID != ID_PHRASE_5MIN)
		GetMenuBar()->Check(ID_PHRASE_5MIN, false);

	if(nID != ID_PHRASE_15MIN)
		GetMenuBar()->Check(ID_PHRASE_15MIN, false);

	if(nID != ID_PHRASE_30MIN)
		GetMenuBar()->Check(ID_PHRASE_30MIN, false);

	if(nID != ID_PHRASE_60MIN)
		GetMenuBar()->Check(ID_PHRASE_60MIN, false);

	if(nID != ID_PHRASE_DAY)
		GetMenuBar()->Check(ID_PHRASE_DAY, false);

	if(nID != ID_PHRASE_WEEK)
		GetMenuBar()->Check(ID_PHRASE_WEEK, false);

	if(nID != ID_PHRASE_MONTH)
		GetMenuBar()->Check(ID_PHRASE_MONTH, false);

	if(nID != ID_PHRASE_QUIT)
		GetMenuBar()->Check(ID_PHRASE_QUIT, false);
	
}
void MyFrame::KXIAN(wxCommandEvent& event)
{
	if(m_pKLineView)
		m_pKLineView->Show();
	if(m_pTLineView)
		m_pTLineView->Show(false);
}
void MyFrame::FENSHI(wxCommandEvent& event)
{
	if(m_pTLineView)
		m_pTLineView->Show();
	if(m_pKLineView)
		m_pKLineView->Show(false);		
}
void MyFrame::OnSize(wxSizeEvent& event)
{
	wxRect rtClient = GetClientRect();
	wxSize sz=event.GetSize();
	
	rtClient = GetClientRect();
	if(rtClient.GetHeight() == 0 || rtClient.GetWidth() ==0)
		return;
	m_splitter->SetMinimumPaneSize(m_nRightSpanMiniWidth);
	m_splitter->SetSize(rtClient.width , rtClient.height);
	m_splitter->UpdateSize();
	if(rtClient.width >300 && rtClient.height >100)
	{		 
		m_splitter->SplitVertically(m_pTLineView, m_pQPanel,-300);	//这个函数只调用第一次有效，空间没有250大小则可能会无效
	}
}
void MyFrame::OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate)
{
	if(pFData == NULL || pData == NULL)
		return;	
	
	//wxString strDate = pFData->TradingDay;
	wxString strDate = pFData->m_strTradingDay;//如果行情里面没有日期，照样不能解决跨日期的问题
	wxString strFutureID = pData->InstrumentID;
	if(isInvalidValue(pData->LastPrice) 
		||isInvalidValue(pData->OpenPrice)
		||isInvalidValue(pData->HighestPrice)
		||isInvalidValue(pData->LowestPrice)
		||isInvalidValue(pData->Volume))
	{		
		if(isInvalidValue(pData->ClosePrice))
			pData->ClosePrice = 0;
		if(isInvalidValue(pData->LastPrice))
			pData->LastPrice = 0;
		if(isInvalidValue(pData->OpenPrice))
			pData->OpenPrice = 0;
		if(isInvalidValue(pData->HighestPrice))
			pData->HighestPrice = 0;
		if(isInvalidValue(pData->LowestPrice))
			pData->LowestPrice = 0;
		if(isInvalidValue(pData->Volume))
			pData->Volume = 0;
		m_pQPanel->SetInTimeData(*pData);

		if(m_nTrace)
			AppendLog_DepthMarketData(pData);
		return;//无效值
	}	
	if(isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
	if(isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
	if(m_nTrace)
		AppendLog_DepthMarketData(pData);
	pFData->SetDataInTime(pData,  m_nTrace);


	if(bUpdate)
	{
		if(m_pTLineView)
		{
			m_pTLineView->SetLastClosePrice(pFData->m_PreSettlementPrice);			
			m_pTLineView->SetInTimeData();
		}
		if(m_pKLineView)
			m_pKLineView->SetInTimeData();		
		if(m_pQPanel)
		{
			m_pQPanel->SetGridLastItem();
			m_pQPanel->SetInTimeData(*pData);
			m_pQPanel->Refresh(false);		
		}
	}
}
void MyFrame::OnTimer1(wxTimerEvent& event)
{
	int nID = event.GetId();

	if(nID == 1113)
	{
		DWORD dwProcessId = 0;
		GetWindowThreadProcessId((HWND)m_parent, &dwProcessId);
		if(dwProcessId == 0)
		{
			close(false);
			delete this;
			exit(0);
		}
	}
	if(nID == 1114)
	{
		if(m_bChangedTime == false)
		{
			m_bChangedTime = true;
			return;
		}
		time_t tmtM = mktime(&m_tmLastTrans);
		tmtM = tmtM + (GetTickCount() - m_dwBeginTickCount - 120)/1000;
		struct tm tmLastTrans = *localtime(&tmtM);
		SetLastTransTime(tmLastTrans);
		m_bChangedTime = false;
	}

}
WXLRESULT MyFrame::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{
	if(message==WM_COPYDATA)
	{
		COPYDATASTRUCT* data = (COPYDATASTRUCT*)lParam;
	}	
	if(message==nDataPkgArriveMSGID) 
	{
		if(lParam==NULL)
			return wxFrame::MSWWindowProc(message, wParam, lParam);
		/*		int nPkgLen = 0, nTakenLen = 0;
		CUniPkg uniPkg;
		Stru_UniPkgHead* pHead = (Stru_UniPkgHead*)lParam;
		nPkgLen = pHead->len + GetPackageHeadTailLen();
		uniPkg.Parse((char*)lParam, nPkgLen, nTakenLen);
		if(uniPkg.IsValidPkg()) {
		switch(pHead->cmdid) {
		case CMDID_CustomerInfo_rspquery:
		case CMDID_CustomerInfo_rspquerySimple:
		char* pBuff = new char[nPkgLen];
		ZeroMemory(pBuff, nPkgLen);
		memcpy(pBuff, (char*)lParam, nPkgLen);
		PostMessage(GetDataPkgArriveNextMSGID(), wParam, (LPARAM)pBuff);
		break;
		};
		}*/
	}
	else if(message==nDataPkgArriveNextMSGID) 
	{
		if(lParam==NULL)
			return wxFrame::MSWWindowProc(message, wParam, lParam);
		/*		Stru_UniPkgHead* pHead = (Stru_UniPkgHead*)lParam;
		switch(pHead->cmdid) {
		case CMDID_CustomerInfo_rspquery:
		break;
		case CMDID_CustomerInfo_rspquerySimple:
		Parse_rspquerySimple((char*)lParam);
		m_pLastQueryRspPkg = (char*)lParam;
		break;
		};*/
	}
	return wxFrame::MSWWindowProc(message,wParam,lParam); 
}
void MyFrame::SetTodayScales(struct tm tmDay, wxString strTransSpan, wxString ExchangeName, vector<struct tm>& vecTimeScales)
{
	struct tm tmScale1;
	tmScale1.tm_year = tmDay.tm_year;//year since 1900
	tmScale1.tm_mon  = tmDay.tm_mon;//[0, 11]
	tmScale1.tm_mday = tmDay.tm_mday;

	string str = strTransSpan.c_str();
	vector<string> vecTime;
	split(str, ' ',vecTime);
	for(int i =0; i< (int)vecTime.size(); i++)
	{
		string strTime = vecTime[i];
		vector<string> vecOne;
		split(strTime, '-',vecOne);
		if(vecOne.size() < 2)
			break;
		wxString strSpan = vecOne[0];
		int nPos = strSpan.find(':');
		if(nPos == -1)
			break;		
		
		wxString strLeft  = strSpan.Left(nPos);		
		int nhour = wxAtoi(strLeft);
		tmScale1.tm_hour = nhour;
		wxString strRight =  strSpan.Right(strSpan.Length()- nPos -1);
		int nmin  = wxAtoi(strRight);
		tmScale1.tm_min = nmin;
		tmScale1.tm_sec = 0;
		vecTimeScales.push_back(tmScale1);

		strSpan = vecOne[1];
		nPos = strSpan.find(':');
		if(nPos == -1)
			break;
		strLeft  = strSpan.Left(nPos);
		nhour = wxAtoi(strLeft);
		tmScale1.tm_hour = nhour;
		strRight =  strSpan.Right(strSpan.Length()- nPos -1);
		nmin  = wxAtoi(strRight);
		tmScale1.tm_min = nmin;
		tmScale1.tm_sec = 0;
		vecTimeScales.push_back(tmScale1);

	}
}
void MyFrame::SetTodayScales(struct tm tmDay, wxString ExchangeName, vector<struct tm>& vecTimeScales)
{
	//上期所，郑商所，大商所；中金
	wxString strExhcange = ExchangeName;
	strExhcange.MakeLower();
	map<string, vector<SSpan*>*>::iterator it = m_mapTransTime.find(strExhcange.c_str());
	
	if(it != m_mapTransTime.end())
	{
		vector<SSpan*>* pVec = it->second;
		for(int i =0; i<(int)pVec->size(); i++)
		{
			SSpan *p = (*pVec)[i];
			struct tm tmScale1;
			tmScale1.tm_year = tmDay.tm_year;//year since 1900
			tmScale1.tm_mon  = tmDay.tm_mon;//[0, 11]
			tmScale1.tm_mday = tmDay.tm_mday;

			tmScale1.tm_hour = p->tmBegin.tm_hour;
			tmScale1.tm_min  = p->tmBegin.tm_min;
			tmScale1.tm_sec  = 0;
			vecTimeScales.push_back(tmScale1);
			tmScale1.tm_hour = p->tmEnd.tm_hour;
			tmScale1.tm_min  = p->tmEnd.tm_min;
			vecTimeScales.push_back(tmScale1);
		}
	}	
	else
	if(ExchangeName == "SHFE" || ExchangeName == "CZCE" || ExchangeName == "DCE" )
	{		
		struct tm tmScale1;
		tmScale1.tm_year = tmDay.tm_year;//year since 1900
		tmScale1.tm_mon  = tmDay.tm_mon;//[0, 11]
		tmScale1.tm_mday = tmDay.tm_mday;
		
		tmScale1.tm_hour = 9;
		tmScale1.tm_min  = 0;
		tmScale1.tm_sec  = 0;
		vecTimeScales.push_back(tmScale1);
		tmScale1.tm_hour = 10;
		tmScale1.tm_min  = 15;
		vecTimeScales.push_back(tmScale1);

		tmScale1.tm_hour = 10;
		tmScale1.tm_min  = 30;
		vecTimeScales.push_back(tmScale1);
		tmScale1.tm_hour = 11;
		tmScale1.tm_min  = 30;
		vecTimeScales.push_back(tmScale1);


		tmScale1.tm_hour = 13;
		tmScale1.tm_min  = 0;
		vecTimeScales.push_back(tmScale1);
		tmScale1.tm_hour = 15;
		tmScale1.tm_min  = 0;
		vecTimeScales.push_back(tmScale1);
	}
	else if (ExchangeName == "CFFEX")//中金
	{
		struct tm tmScale1;
		tmScale1.tm_year = tmDay.tm_year;
		tmScale1.tm_mon  = tmDay.tm_mon;
		tmScale1.tm_mday = tmDay.tm_mday;
		
		tmScale1.tm_hour = 9;
		tmScale1.tm_min  = 15;
		tmScale1.tm_sec  = 0;
		vecTimeScales.push_back(tmScale1);
		tmScale1.tm_hour = 11;
		tmScale1.tm_min  = 30;
		vecTimeScales.push_back(tmScale1);

		tmScale1.tm_hour = 13;
		tmScale1.tm_min  = 0;
		vecTimeScales.push_back(tmScale1);
		tmScale1.tm_hour = 15;
		tmScale1.tm_min  = 15;
		vecTimeScales.push_back(tmScale1);
	}	
}
void MyFrame::OnFuncLDBClickKLine(wxCommandEvent& event)
{
	m_pKLineView->Hide();
	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
}
void MyFrame::OnFuncLDBClickTLine(wxCommandEvent& event)
{
	OnReturnKLine();	
/*	if(m_splitter && m_pTLineView && m_pKLineView)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pTLineView)
			m_splitter->ReplaceWindow(m_pTLineView, m_pKLineView);

		m_pTLineView->Hide();
		EnumPhrase emPhrase = m_pKLineView->GetPhrase();
		m_pKLineView->SetPhrase(emPhrase);//初始化默认为日k线
		m_pKLineView->Show();
	}	*/
}
void MyFrame::OnFuncKeyDownTLine(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_RETURN)
	{

		OnReturnKLine();	
/*		if(m_splitter && m_pTLineView && m_pKLineView)
		{
			wxWindow *pWnd = m_splitter->GetWindow1();
			if(pWnd == m_pTLineView)
				m_splitter->ReplaceWindow(m_pTLineView, m_pKLineView);
			m_pTLineView->Hide();
					
			EnumPhrase emPhrase = m_pKLineView->GetPhrase();
			m_pKLineView->SetPhrase(emPhrase);//初始化默认为日k线
			m_pKLineView->Show();
			m_pKLineView->SetFocus();
		}*/	
	}
	else if(keyCode == WXK_ESCAPE)
	{
		OnReturnBaoJiaoBiao();
	//	m_splitter->Hide();
	//	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
	}
	else if(keyCode == VK_PRIOR)
	{
		OnPrior();
	}
	else if(keyCode == VK_NEXT)
	{
		OnNext();
	}
}
void MyFrame::OnFuncKeyQPanel(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_PRIOR)//这个消息跟上面别的VK_PRIOR不一样，原理不同
	{
		OnPrior();
	}
	else if(keyCode == WXK_NEXT)
	{
		OnNext();
	}
}
void MyFrame::OnFuncKeyDownKLine(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_RETURN)
	{
		OnReturnBaoJiaoBiao();
		//m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
	}
	else if(keyCode == WXK_ESCAPE)
	{
		OnReturnBaoJiaoBiao();
		//m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
	}
	else if(keyCode == VK_PRIOR)
	{
		OnPrior();
	}
	else if(keyCode == VK_NEXT)
	{
		OnNext();
	}
}

bool MyFrame::InitFromCFG()
{	
	wxString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath = filename.substr(0, splitpos+1);
	
	wxString strDir;
	strDir.Printf(_T("%shqData\\hqCfg.xml"), strPath);
	
	TiXmlDocument* pXmlDocment = new TiXmlDocument( strDir.c_str());
	if( NULL == pXmlDocment)
	{
		return false;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return false;
	}

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return false;
	}

	if( std::string(root->Value()) != "hq")
	{
		return false;
	}
	if(!InitCfg(root))
	{
		return false;
	}
	if(m_pKLineView)
	{
		if(!m_pKLineView->InitCfg(root))
		{
			return false;
		}
	}
	if(m_pTLineView)
	{
		if(!m_pTLineView->InitCfg(root))
		{
			return false;
		}
	}
	if(m_pQPanel)
	{
		if(!m_pQPanel->InitCfg(root))
		{
			return false;
		}
	}
	return true;
}
void MyFrame::SetMiniWidth(unsigned long value)
{
	m_nRightSpanMiniWidth = value;
}
bool MyFrame::InitCfg(TiXmlElement *root)
{//各交易所交易时间配置文件

	TiXmlElement *tList2 = root->FirstChildElement("qpanel");
	if(tList2 == NULL)
		return false;

	TiXmlNode *pMiniWidth = tList2->FirstChild("MiniWidth");
	if(pMiniWidth == NULL)
		return false;
	const char *strValue = pMiniWidth->ToElement()->GetText();
	unsigned long value = ColorStr2Long(strValue);
	SetMiniWidth(value);


	TiXmlElement *tList = root->FirstChildElement("exchange");
	if(tList == NULL)
		return false;

	for(TiXmlElement *GroupItem = tList->FirstChildElement("group");
		GroupItem; 
		GroupItem = GroupItem->NextSiblingElement("group"))
	{
		const char *pGroupid = GroupItem->Attribute("id");
		if(!pGroupid) 
		{
			continue;
		}
		string strID = pGroupid;
		const char *pGrouptime = GroupItem->Attribute("time");
		if(!pGrouptime)
		{
			continue;
		}
		vector<SSpan*> *pVector = new vector<SSpan*>;
		string str = pGrouptime;
		vector<string> vecTime;
		split(str, ' ',vecTime);
		for(int i =0; i< (int)vecTime.size(); i++)
		{
			string strTime = vecTime[i];
			vector<string> vecOne;
			split(strTime, '-',vecOne);
			if(vecOne.size() < 2)
				break;
			wxString strSpan = vecOne[0];
			int nPos = strSpan.find(':');
			if(nPos == -1)
				break;
			SSpan *span = new SSpan;
			wxString strLeft  = strSpan.Left(nPos);
			wxString strRight =  strSpan.Right(strSpan.Length()- nPos -1);
			span->tmBegin.tm_hour = wxAtoi(strLeft);
			span->tmBegin.tm_min  = wxAtoi(strRight);

			strSpan = vecOne[1];
			nPos = strSpan.find(':');
			if(nPos == -1)
				break;
			strLeft  = strSpan.Left(nPos);
			strRight =  strSpan.Right(strSpan.Length()- nPos -1);
			span->tmEnd.tm_hour = wxAtoi(strLeft);
			span->tmEnd.tm_min  = wxAtoi(strRight);

			pVector->push_back(span);
		}
		m_mapTransTime.insert(map<string, vector<SSpan*>*>::value_type(strID,  pVector));			
	}

	return true;
}
void MyFrame::OnReSendMessage(wxCommandEvent& event)
{
	SMessage *pMess =  (SMessage *)event.GetClientData();
	if(pMess == NULL)
		return;
	
	OnRecvApiMsg_Process(pMess->nCmdId, pMess->nErrorCode, pMess->pData, pMess->nDataLen, pMess->nRequestId);
	
	if(pMess)
	{
		delete []pMess->pData;
		pMess->pData = NULL;
		delete pMess;
		pMess = NULL;
	}

}
int MyFrame::OnRecvApiMsg_Process( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
		ProcessConsoleMsg(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	}
	else if ( nCmdId >= CMDID_EXCHANGE_MIN && nCmdId <= CMDID_EXCHANGE_MAX )
	{
		ProcessExchangeMsg(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	}
	else if ( nCmdId >= CMDID_INSTRUMENT_MIN && nCmdId <= CMDID_INSTRUMENT_MAX )
	{
		ProcessInstrumentMsg(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	}
	else if ( nCmdId >= CMDID_QUOTE_MIN && nCmdId <= CMDID_QUOTE_MAX )
	{
		ProcessQuoteMsg(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	}
	else if (nCmdId >= CMDID_ACCOUNT_MIN && nCmdId <= CMDID_ACCOUNT_MAX)
	{
		ProcessAccountMsg(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	}
	return 1;
}
int MyFrame::OnRecvApiMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId  )
{
	if(g_pAppDlg == NULL)
		return 0;
	
	SMessage *pMess = new SMessage(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	wxCommandEvent myEvent(wxEVT_ReSend_Message);
	myEvent.SetClientData(pMess);
	g_pAppDlg->AddPendingEvent(myEvent);
	
	return 0;
}
void MyFrame::ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId == CMDID_CONSOLE_LOGON_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			m_pTLineView->Show();
			Show();
			//"登录成功"
		}
		else
		{
			wxCommandEvent myEvent(wxEVT_CLOSE_WINDOW);
			AddPendingEvent(myEvent);
		}
	}
	if ( nCmdId == CMDID_CONSOLE_CLOSE )
	{
		wxCommandEvent myEvent(wxEVT_CLOSE_WINDOW);
		AddPendingEvent(myEvent);
	}
}

void MyFrame::ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId == CMDID_EXCHANGE_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData || nDataLen % 32 != 0)
			{
				//"返回的交易所列表数据格式错误"
			}
			else
			{
				//"交易所列表数据返回："
				char* pTemp = (char*)pData;
				for ( size_t i = 0; i <(size_t) nDataLen/32; i++ )
				{
				//	char szBuf[32];
				//	memset(szBuf, 0, sizeof(szBuf));
				//	memcpy(szBuf, pTemp+i*32, 32);
					//g_pAppDlg->m_listBox.InsertString(//g_pAppDlg->m_listBox.GetCount(), szBuf);
				}
			}
		}
		else
		{
			//"请求交易所列表失败"
		}
	}
	else if ( nCmdId == CMDID_EXCHANGE_CURRENT_TIME_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData )
			{
				//"返回交易所当前时间数据格式错误");
			}
			else
			{
				ExchangeCurTime *pTime = (ExchangeCurTime *)pData;
				if(pTime == NULL)
					return;

				CFData *pFData = g_pAppDlg->GetFDataByQueryID(nRequestId);
				m_mapNum2ID.erase(nRequestId);
				if(pFData == NULL)
					return;

				string str = pTime->szExchangeTime;
				if(str.length() <16)
				{
					wxASSERT(0);
					return;
				}
				pFData->m_strTradingDay =str.substr(0, 8);	
				m_strTradingDay = str.substr(0, 8);	//如果是跨日期的，那么情况要复杂
				

				if(!m_bInitTimerTrans)
				{
					m_bInitTimerTrans = true;
					struct  tm tmLastTrans;
					tmLastTrans.tm_year = atoi(str.substr(0, 4).c_str()) - 1900;
					tmLastTrans.tm_mon  = atoi(str.substr(4, 2).c_str()) - 1;
					tmLastTrans.tm_mday = atoi(str.substr(6, 2).c_str());
					tmLastTrans.tm_hour = atoi(str.substr(9, 2).c_str());
					tmLastTrans.tm_min  = atoi(str.substr(12, 2).c_str());
					tmLastTrans.tm_sec  = atoi(str.substr(15, 2).c_str());	

					m_dwBeginTickCount = pTime->dwTickCount;
					m_tmLastTrans = tmLastTrans;				
					m_bChangedTime = false;
					m_timerTrans.Stop();
					m_timerTrans.Start(m_nTime);
					
				}
				//"返回交易所当前时间				
			}
		}
		else
		{
			//"请求交易所当前时间失败";
		}
	}
	else if ( nCmdId == CMDID_EXCHANGE_TRADE_TIME_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData )
			{
				//"返回交易所交易时间数据格式错误"
			}
			else
			{//返回交易所交易时间
				CFData *pFData = GetFDataByQueryID(nRequestId);
				m_mapNum2ID.erase(nRequestId);
				if(pFData == NULL)
					return;

				char szBuf[256];
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pData, nDataLen);

				string strTimes = szBuf;
				vector<struct tm> vecTimeScales;
				SetTodayScales(strTimes,  vecTimeScales);
				pFData->SetScales(vecTimeScales);//分时交易区间

				int nId = GetNextRequestId();
				m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  pFData->m_ContractName));	

				char szID[32];
				memset(szID, 0, sizeof(szID));
				strcpy(szID, pFData->m_ContractName.c_str());
				m_pApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY,szID, 
					strlen(szID), nId);

				
			}
		}
		else
		{
			//请求交易所交易时间失败
		}
	}
}

void MyFrame::ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId == CMDID_INSTRUMENT_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData || nDataLen % 64 != 0)
			{
				//"返回的合约列表数据格式错误";
			}
			else
			{
				//"合约列表数据返回：";
			/*	char* pTemp = (char*)pData;
				for ( size_t i = 0; i < (size_t)nDataLen/64; i++ )
				{
					char szBuf[64];
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pTemp+i*64, 64);					
				}*/
			}
		}
		else
		{
			//"请求合约列表失败");
		}
	}
	if ( nCmdId == CMDID_INSTRUMENT_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			CFData *pFData = GetFDataByQueryID(nRequestId);
			m_mapNum2ID.erase(nRequestId);
			if(pFData == NULL)
				return;
			PlatformStru_InstrumentInfo *pInstrumentData = (PlatformStru_InstrumentInfo *)pData;
			if(pData == NULL)
				return;			
			pFData->m_ExchangeName   = pInstrumentData->ExchangeID;
			pFData->m_VolumeMultiple = pInstrumentData->VolumeMultiple;
		//	pFData->m_PriceTick		 = pInstrumentData->PriceTick;
			pFData->SetPriceTick(pInstrumentData->PriceTick);

			char szExchangeID[256];
			memset(szExchangeID, 0, sizeof(szExchangeID));
			strcpy(szExchangeID, pFData->m_ExchangeName.c_str());
		
			
			int nId = GetNextRequestId();
			m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  pFData->m_ContractName));	
			m_pApi->Send(CMDID_EXCHANGE_TRADE_TIME_QUERY, szExchangeID, 
				strlen(szExchangeID) , nId);

			nId = GetNextRequestId();
			m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  pFData->m_ContractName));	
			m_pApi->Send(CMDID_EXCHANGE_CURRENT_TIME_QUERY, szExchangeID, 
				strlen(szExchangeID), nId);

			//请求合约详情成功
		}
		else
		{
			//请求合约详情失败
		}
	}
}

void MyFrame::ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if(pData == NULL)
		return;

	if ( nCmdId == CMDID_QUOTE_CURRENT_SEL)
	{//初始化的过程 合约ID=》合约详情=》交易所名称=》交易所交易时间=》行情快照=》然后初始化行情界面=》订阅行情
		char szBuf[64];
		memset(szBuf, 0, sizeof(szBuf));
		memcpy(szBuf, pData, nDataLen);
		string strID = szBuf;
		m_strSelFData = strID;		
		for(int n =0; n< (int)m_vecFData.size(); n++)
		{//先发列表信息，在发合约信息:如果插件改正这个顺序，则行情也需要改变
			string strTemp = m_vecFData[n];	
			if(strTemp == m_strSelFData)
				m_nFDataIndex = n;				
		}
		CFData *pFData = NULL;
		map<string,  CFData *>::iterator itFData = m_mapID2FData.find(strID);
		if(itFData !=  m_mapID2FData.end())
		{
			pFData = itFData->second;		
		}
		if(pFData == NULL)
		{
			pFData = new CFData;
			pFData->m_ContractName = strID;
			DelFData(1);
		}
		m_mapID2FData.insert(map<string,  CFData *>::value_type(strID,  pFData));

		int nId = GetNextRequestId();
		m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  strID));	
		m_pApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szBuf, 
			strlen(szBuf), nId);

		nId = GetNextRequestId();
		m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  strID));	
		m_pApi->Send(CMDID_QUOTE_IS_SIMULATE_QUERY, szBuf, 
			strlen(szBuf), nId);

		InitGUI(pFData, NULL, false);
		
	//	csTemp.Format("报价表双击合约：%s", szBuf);
	}
	else if ( nCmdId == CMDID_QUOTE_SEND_QUOTE )
	{
		if ( NULL == pData || nDataLen != sizeof(PlatformStru_DepthMarketData) )
		{
		}
		else
		{
			PlatformStru_DepthMarketData* pMarketData = (PlatformStru_DepthMarketData*)pData;
			if(pMarketData == NULL)
				return;

		//	wxString str;
		//	str.Printf(wxT("\n%s %s %.2f"), pMarketData->InstrumentID, pMarketData->UpdateTime, pMarketData->LastPrice);
		//	OutputDebugString(str.c_str());		

			string strID = pMarketData->InstrumentID;
			map<string,  CFData *>::iterator itFData = m_mapID2FData.find(strID);
			CFData *pFData = NULL;
			if(itFData !=  m_mapID2FData.end())
				pFData = itFData->second;

			if(pFData == NULL)
				return;
			bool bUpdate = false;
			if(m_strSelFData == pFData->m_ContractName)
				bUpdate = true;
			OnDataInTime(pMarketData, pFData, bUpdate);
/*
			if(m_bChangedTime)
			{
				wxString strDate = m_strTradingDay;
				wxString strTime		= pMarketData->UpdateTime;
				if(strDate.Length() < 8 || strTime.Length() <8)
					return;

				struct  tm tmLastTrans;
				tmLastTrans.tm_year	= atoi(strDate.Left(4))-1900;
				tmLastTrans.tm_mon	= atoi(strDate.Mid(4,2))-1;
				tmLastTrans.tm_mday	= atoi(strDate.Right(2));	
				tmLastTrans.tm_hour	= atoi(strTime.Left(2));
				tmLastTrans.tm_min	= atoi(strTime.Mid(3, 2));
				tmLastTrans.tm_sec	= atoi(strTime.Mid(6, 2));;
				tmLastTrans.tm_wday	= 0;
				tmLastTrans.tm_yday	= 0;

				m_timerTrans.Stop();
				m_timerTrans.Start(m_nTime);
			
				SetLastTransTime(tmLastTrans);
				m_bChangedTime = false;

		//		m_tmLastTrans = tmLastTrans;
				

				
				
			}*/
		//("收到行情，合约ID为：%s", pMarketData->InstrumentID);	
		}		
	}
	else if ( nCmdId == CMDID_QUOTE_SUBSCRIBE_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
	//"订阅成功";
		}
		else if ( nErrorCode == ERROR_ONE)
		{
	//"订阅的合约数量超过最大限制";
		}
		else
		{
	//"订阅合约失败，未知错误";
		}
	}
	else if ( nCmdId == CMDID_QUOTE_SNAPSHOT_RSP )
	{	
		CFData *pFData = GetFDataByQueryID(nRequestId);
		m_mapNum2ID.erase(nRequestId);
		if(pFData == NULL)
		{			
			return;
		}
		PlatformStru_DepthMarketData *pQutoData = (PlatformStru_DepthMarketData *)pData;
		if(pData == NULL)
			return;		/*	
		pFData->m_UpperLimitPrice    = pQutoData->UpperLimitPrice;
		pFData->m_LowerLimitPrice    = pQutoData->LowerLimitPrice;
		pFData->m_PreSettlementPrice = pQutoData->PreSettlementPrice;

		if(pFData)
		{
			pFData->InitData();
			pFData->SetFuture(pFData->m_ExchangeName.c_str(), pFData->m_ContractName.c_str(), wxT(""));	
			pFData->SetFuture(pFData->m_ExchangeName.c_str(), pFData->m_ContractName.c_str(), wxT(""), PHRASE_DAY);
			pFData->SetFuture_QIntime(pFData->m_ExchangeName.c_str(), pFData->m_ContractName.c_str(), wxT(""));		
		}*/
		InitGUIData(pFData, pQutoData);
		InitGUI(pFData, pQutoData, true); //已有数据初始化界面
		int nId = GetNextRequestId();
		m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  pFData->m_ContractName));			

		char szID[256];
		memset(szID, 0, sizeof(szID));
		strcpy(szID, pFData->m_ContractName.c_str());
		m_pApi->Send(CMDID_QUOTE_SUBSCRIBE, szID, strlen(szID), nId);	
	
		//"收到行情快照返回");
	}
	else if ( nCmdId == CMDID_QUOTE_CURRENT_GROUP_INST_LIST )
	{
		if ( NULL == pData || nDataLen % 64 != 0)
		{
			//"当前合约组内合约列表数据格式错误");
		}
		else
		{
			//"收到当前合约组内合约列表数据：");
			m_vecFData.clear();
			char* pTemp = (char*)pData;
			for ( size_t i = 0; i < (size_t)nDataLen/64; i++ )
			{
				char szBuf[64];
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pTemp+i*64, 64);

				string strID = szBuf;
				m_vecFData.push_back(strID);	
			//	if(strID == m_strSelFData)
			//		m_nFDataIndex = i;				
			}
		}
	}
	else if ( nCmdId == CMDID_QUOTE_IS_SIMULATE_RSP)
	{
		if ( NULL != pData && nDataLen == sizeof(int))
		{
			int nSimulate = *(int*)pData;

			CFData *pFData = GetFDataByQueryID(nRequestId);
			m_mapNum2ID.erase(nRequestId);
			if(pFData == NULL)
				return;
			pFData->m_Simulate = nSimulate;
		}
		else
		{
			//"是否模拟盘查询失败"
		}
	}
	else
	{

	}
}

void MyFrame::ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId == CMDID_ACCOUNT_SUB_ACCOUNT_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData || nDataLen % 13 != 0)
			{
				//"返回的子账户列表数据格式错误");
			}
			else
			{
				//m_listBox.InsertString(//m_listBox.GetCount(), "子账户列表数据返回：");
				char* pTemp = (char*)pData;
				for ( size_t i = 0; i < (size_t)nDataLen/13; i++ )
				{
				//	char szBuf[13];
				//	memset(szBuf, 0, sizeof(szBuf));
				//	memcpy(szBuf, pTemp+i*13, 13);
					//m_listBox.InsertString(//m_listBox.GetCount(), szBuf);
				}
			}
		}
		else
		{
			//"请求子账户列表失败");
		}
	}
	if ( nCmdId == CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_TradingAccountInfo
			//"请求子账户详情成功");
		}
		else
		{
			//"请求子账户详情失败");
		}
	}
}
int MyFrame::GetNextRequestId()
{
	m_nQueryNum++;
	return m_nQueryNum;
}
CFData* MyFrame::GetFDataByQueryID(int nQueryNum)
{
	CFData *pFData = NULL;
	string strID;
	map<DWORD,  string>::iterator itNum = m_mapNum2ID.find(nQueryNum);
	if(itNum ==  m_mapNum2ID.end())
		return NULL;

	strID = itNum->second;	
	map<string,  CFData *>::iterator itFData = m_mapID2FData.find(strID);
	if(itFData !=  m_mapID2FData.end())
	{
		pFData = itFData->second;
	}
	return pFData;
}
void MyFrame::SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales)
{//201205120900201515121015 201205121100201205121130 201205131315201205121730
	vector<string> vecTime;
	split(strTimes, ' ',vecTime);
	for(int i=0; i<(int)vecTime.size(); i++)
	{
		string strTime = vecTime[i];
		if(strTime.length() != 24)
			continue;

		struct tm tmScale;
		tmScale.tm_year = atoi(strTime.substr(0, 4).c_str()) - 1900;
		tmScale.tm_mon  = atoi(strTime.substr(4, 2).c_str()) - 1;
		tmScale.tm_mday = atoi(strTime.substr(6, 2).c_str());
		tmScale.tm_hour = atoi(strTime.substr(8, 2).c_str());
		tmScale.tm_min  = atoi(strTime.substr(10, 2).c_str());
		tmScale.tm_sec  = 0;		
		vecTimeScales.push_back(tmScale);

		tmScale.tm_year = atoi(strTime.substr(12, 4).c_str()) - 1900;
		tmScale.tm_mon  = atoi(strTime.substr(16, 2).c_str()) - 1;
		tmScale.tm_mday = atoi(strTime.substr(18, 2).c_str());
		tmScale.tm_hour = atoi(strTime.substr(20, 2).c_str());
		tmScale.tm_min  = atoi(strTime.substr(22, 2).c_str());
		vecTimeScales.push_back(tmScale);
	}
}
void  MyFrame::InitGUIData(CFData *pFData, PlatformStru_DepthMarketData *pQutoData)
{
	pFData->m_UpperLimitPrice    = pQutoData->UpperLimitPrice;
	pFData->m_LowerLimitPrice    = pQutoData->LowerLimitPrice;
	pFData->m_PreSettlementPrice = pQutoData->PreSettlementPrice;
	pFData->m_fMaxValue			 = pQutoData->HighestPrice;
	pFData->m_fMinValue			 = pQutoData->LowestPrice;
	if(pFData)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pTLineView)
			pFData->SetFuture();	
		else
		{
			EnumPhrase enumPhrase = m_pKLineView->GetPhrase();
			pFData->SetPhrase(enumPhrase);			
			//pFData->SetFuture(pFData->m_ExchangeName.c_str(), pFData->m_ContractName.c_str(), wxT(""), enumPhrase);
		}
		pFData->SetFuture_QIntime();		
	
	}
}
void MyFrame::InitGUI(CFData *pData, PlatformStru_DepthMarketData *pQutoData, bool bRemain)
{//两次调用该函数，一次是用于及时显示，第二次请求到合约详情以后才能真正显示正确的信心
	wxWindow *pWnd = m_splitter->GetWindow1();
	if(m_pKLineView)
	{		
		EnumPhrase enumPhrase = m_pKLineView->GetPhrase();
		m_pKLineView->SetFData(pData);
		if(pWnd == m_pKLineView && pQutoData)//行情快照之后才会有交易所名称之类的
			m_pKLineView->SetPhrase(enumPhrase);			
	}		
	if(m_pTLineView)
	{
		m_pTLineView->SetFData(pData);		
		if(!bRemain)
		{			
			if(pWnd == m_pKLineView)
				m_splitter->ReplaceWindow(m_pKLineView, m_pTLineView);
			m_pKLineView->Show(false);
			m_pTLineView->Refresh(false);	
			m_pTLineView->Show();	
			m_pTLineView->SetFocus();
		}	
		else
		{
			if(pQutoData)
				pWnd->Refresh(false);
		}
	}	
	if(m_pQPanel)
	{			
		m_pQPanel->Init();
		m_pQPanel->SetFuture(pData->m_ExchangeName.c_str(), pData->m_ContractName.c_str(), wxT(""));
		m_pQPanel->SetFData(pData);
		if(pQutoData)
			m_pQPanel->InitGrid();//不要重复初始化
		if(pQutoData)
			m_pQPanel->SetInTimeData(*pQutoData);
		m_pQPanel->Refresh(false);	
		m_pQPanel->Show();		
	}
	m_splitter->Show();
}
void MyFrame::SetLastTransTime(struct tm tmLastTrans)
{
	int dwTimeSpan = difftime(mktime(&tmLastTrans), mktime(&m_tmLastTrans));
	if(dwTimeSpan <= 0 && m_tmLastTrans.tm_year != 0)
		return;

	map<string, CFData*>::iterator it = m_mapID2FData.begin();
	while(it != m_mapID2FData.end())
	{
		map<string, CFData*>::iterator itTemp = it;
		it++;
		CFData *pFData = itTemp->second;
		if(!pFData)
			continue;
		if(pFData)
		{
			if(pFData->m_Simulate == 1)
				return;//模拟行情对于这种没有交易的状态不处理，因为模拟交易本身给的交易时间不准确
		}

		if(pFData)
		{
			pFData->SetLastTransTime_TLine(tmLastTrans);
			pFData->SetLastTransTime(tmLastTrans);
		}
		if(m_pTLineView)
			m_pTLineView->SetLastTransTime(tmLastTrans);
		if(m_pKLineView)
			m_pKLineView->SetLastTransTime(tmLastTrans);
	}	
//	m_tmLastTrans = tmLastTrans;
}
void MyFrame::OnPrior()
{
	if(m_vecFData.size() == 0)
		return;
	--m_nFDataIndex;
	if(m_nFDataIndex <0)
		m_nFDataIndex = m_vecFData.size() -1;

	string strID = m_vecFData[m_nFDataIndex];
	m_strSelFData = strID;

	CFData *pFData = NULL;
	map<string,  CFData *>::iterator itFData = m_mapID2FData.find(strID);
	if(itFData !=  m_mapID2FData.end())
	{
		pFData = itFData->second;		
	}
	if(pFData == NULL)
	{
		pFData = new CFData;
		pFData->m_ContractName = strID;
		DelFData(1);
	}
	InitGUI(pFData, NULL, true);
	m_mapID2FData.insert(map<string,  CFData *>::value_type(strID,  pFData));

	char szBuf[64];
	memset(szBuf, 0, sizeof(szBuf));
	strcpy(szBuf, strID.c_str());

	int nId = GetNextRequestId();
	m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  strID));	
	m_pApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szBuf, 
		strlen(szBuf), nId);

	nId = GetNextRequestId();
	m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  strID));	
	m_pApi->Send(CMDID_QUOTE_IS_SIMULATE_QUERY, szBuf, 
		strlen(szBuf), nId);
}
void MyFrame::OnNext()
{
	if(m_vecFData.size() == 0)
		return;
	++m_nFDataIndex;
	if(m_nFDataIndex >= m_vecFData.size())
		m_nFDataIndex = 0;

	string strID = m_vecFData[m_nFDataIndex];
	m_strSelFData = strID;

	CFData *pFData = NULL;
	map<string,  CFData *>::iterator itFData = m_mapID2FData.find(strID);
	if(itFData !=  m_mapID2FData.end())
	{
		pFData = itFData->second;		
	}
	if(pFData == NULL)
	{
		pFData = new CFData;
		pFData->m_ContractName = strID;
		DelFData(1);
	}
	InitGUI(pFData, NULL, true);
	m_mapID2FData.insert(map<string,  CFData *>::value_type(strID,  pFData));

	char szBuf[64];
	memset(szBuf, 0, sizeof(szBuf));
	strcpy(szBuf, strID.c_str());

	int nId = GetNextRequestId();
	m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  strID));	
	m_pApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szBuf, 
		strlen(szBuf), nId);

	nId = GetNextRequestId();
	m_mapNum2ID.insert(map<DWORD, string>::value_type(nId,  strID));	
	m_pApi->Send(CMDID_QUOTE_IS_SIMULATE_QUERY, szBuf, 
		strlen(szBuf), nId);
}
void MyFrame::DelFData(int nCount)
{
	if(m_mapID2FData.size()>nCount )
	{
		map<string, CFData*>::iterator it = m_mapID2FData.begin();
		while(it != m_mapID2FData.end())
		{
			map<string, CFData*>::iterator itTemp = it;
			it++;
			CFData *pTemp = itTemp->second;
			if(pTemp)
			{
				char szID[256];
				memset(szID, 0, sizeof(szID));
				strcpy(szID, pTemp->m_ContractName.c_str());
				m_pApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szID, 
					strlen(szID), GetNextRequestId());//退订该合约

				pTemp->SaveFile_TLine();	
				pTemp->SaveFile();	
				pTemp->SaveFile_QIntime();
				delete pTemp;
				pTemp = NULL;
			}
			m_mapID2FData.erase(itTemp);
			break;//只删除一个，以后删除多个，就改一下break即可
		}	
	}


}
BOOL MyFrame::GetRegIntValue(HKEY hRoot,wxString strKeyName,wxString strValueName,int& nValue)
{
	if(!hRoot)
		return FALSE;

	HKEY hDYXH = NULL;
	long l = ERROR_SUCCESS; 
	strKeyName.Trim(false);
	strKeyName.Trim();
	BOOL bFlag = TRUE;
	if(strKeyName.Length())
		l = RegOpenKeyEx(hRoot,strKeyName.c_str(),NULL,KEY_READ,&hDYXH);
	else
	{
		hDYXH = hRoot;
		bFlag = FALSE;
	}

	if(l == ERROR_SUCCESS && hDYXH)
	{
		DWORD dwType =  REG_DWORD;
		DWORD dwValue = 0;
		DWORD dwSize = sizeof(DWORD);
		l = RegQueryValueEx(hDYXH,strValueName,NULL,&dwType,(LPBYTE)&dwValue,&dwSize);

		if(bFlag)
			RegCloseKey(hDYXH);
		if(ERROR_SUCCESS != l)
			return FALSE;

		nValue = dwValue;
		return TRUE;
	}
	else
		return FALSE;
}
void MyFrame::OnFuncReturnKLine(wxCommandEvent& event)
{
	OnReturnKLine();		
}
void MyFrame::OnFuncReturnTLine(wxCommandEvent& event)
{
	OnReturnTLine();
}
void MyFrame::OnFuncReturnBaoJiaBiao(wxCommandEvent& event)
{
	OnReturnBaoJiaoBiao();
}
void MyFrame::OnFuncReturnBaoJiaBiao2(wxCommandEvent& event)
{
	OnReturnBaoJiaoBiao();
}
void MyFrame::OnReturnKLine()
{
	if(m_splitter && m_pTLineView && m_pKLineView)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pTLineView)
			m_splitter->ReplaceWindow(m_pTLineView, m_pKLineView);

		m_pTLineView->Hide();

		EnumPhrase emPhrase = m_pKLineView->GetPhrase();
		m_pKLineView->SetPhrase(emPhrase);//初始化默认为日k线
		m_pKLineView->Show();
	}	
}
void MyFrame::OnReturnTLine()
{
	if(m_splitter && m_pTLineView && m_pKLineView)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pKLineView)
			m_splitter->ReplaceWindow(m_pKLineView, m_pTLineView);

		m_pKLineView->Hide();
		m_pTLineView->Show();
	}	
}
void MyFrame::OnReturnBaoJiaoBiao()
{
	m_pKLineView->Hide();
	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
}
