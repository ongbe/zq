/////////////////////////////////////////////////////////////////////////////
// Name:        docview.cpp
// Purpose:     Document/view demo
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: docview.cpp 35650 2005-09-23 12:56:45Z MR $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/*
 * Purpose:  Document/view architecture demo for wxWidgets class library - MDI
 */


// For compilers that support precompilation, includes "wx/wx.h".
#include "StdAfx.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#if !wxUSE_MDI_ARCHITECTURE
#error You must set wxUSE_MDI_ARCHITECTURE to 1 in setup.h!
#endif

#include "docview.h"
#include "doc.h"
#include "view.h"
struct SMessage
{
	int nCmdId;
	int nErrorCode;
	void* pData;
	int nDataLen;
	int nRequestId;

	SMessage(int nId, int nCode, void* p, int nLen, int nRId)
	{
		nCmdId			= nId;
		nErrorCode		= nCode;
		if(nLen >0)
		{
			pData       = new char[nLen];
			memcpy(pData, p, nLen);
		}
		else 
			pData  = NULL;

		nDataLen = nLen;
		nRequestId = nRId;
	}

};
CHQFrame *g_pAppDlg = (CHQFrame *) NULL;

IMPLEMENT_APP(MyApp)

MyApp::MyApp(void)
{
    m_docManager = (wxDocManager *) NULL;
}

bool MyApp::OnInit(void)
{
	//wxMessageBox("");
	if(__argc != 2)
	{		
		return false;
	}
	
	wxString lpCmdLine = __argv[1];	
	long lHwnd = atol(lpCmdLine.c_str());
	if(lHwnd == 0||(!IsWindow((HWND)lHwnd)))
	{		
		return false;
	}
	WXHWND hWnd = (WXHWND)lHwnd;
	
  //// Create a document manager
  m_docManager = new wxDocManager;

  //// Create a template relating drawing documents to their views
  (void) new wxDocTemplate((wxDocManager *) m_docManager, _T("Drawing"), _T("*.drw"), _T(""), _T("drw"), _T("Drawing Doc"), _T("Drawing View"),
          CLASSINFO(DrawingDocument), CLASSINFO(DrawingView));

  //// Create the main frame window
 g_pAppDlg = new CHQFrame((wxDocManager *) m_docManager, (wxFrame *) NULL,
                        _T("DocView Demo"), wxPoint(0, 0), wxSize(5000, 4000),
					wxCLOSE_BOX, hWnd);
					//_T("DocView Demo"), wxPoint(0, 0), wxSize(800, 800),
					//wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE, NULL);

 
  g_pAppDlg->InitFromCFG();
  g_pAppDlg->SetIcon(wxIcon(_T("IDI_ZQQUATION")));

  g_pAppDlg->Show(true);
//  SetTopWindow(frame);
  return true;
}

int MyApp::OnExit(void)
{
    delete m_docManager;
    return 0;
}

 
wxMDIChildFrame *MyApp::CreateChildFrame(wxDocument *doc, wxView *view, bool isCanvas)
{
  //// Make a child frame 
	 CHQFrame *pFrame = GetMainFrame();   
	 int width, height;
	 pFrame->GetClientSize(&width, &height);

	 m_subframe = new wxDocMDIChildFrame(doc, view, GetMainFrame(), wxID_ANY, _T("Child Frame"), wxPoint(0, 0), wxSize(width, height),
                          //  	 wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);
						 wxCLOSE_BOX| wxNO_FULL_REPAINT_ON_RESIZE);
	return m_subframe;
}
wxMDIChildFrame *MyApp::GetSubFrame()
{
	return m_subframe;
}
MyFrame *CHQFrame::CreateKFrame(wxView *view, wxMDIChildFrame *parent)
{
	int width, height;
	parent->GetClientSize(&width, &height);

	m_myFrame= new MyFrame(parent, 0, 0, width, height);
	return m_myFrame;
}

 
IMPLEMENT_CLASS(CHQFrame, wxDocMDIParentFrame)
BEGIN_EVENT_TABLE(CHQFrame, wxDocMDIParentFrame)
	EVT_COMMAND(wxID_ANY, wxEVT_ReSend_Message, OnReSendMessage)
    EVT_MENU(DOCVIEW_ABOUT, CHQFrame::OnAbout)
	EVT_CLOSE( OnClose)
	EVT_SIZE(OnSize)
	EVT_TIMER(1114, OnTimer1)
	EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_LEFTDBCLICK,	OnFuncLDBClickKLine)
	EVT_COMMAND(wxID_ANY, wxEVT_TCtrl_KeyDown,		OnFuncKeyDownTLine)
	EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_KeyDown,		OnFuncKeyDownKLine)
	EVT_COMMAND(wxID_ANY, wxEVT_GridEx_KeyDown,		OnFuncKeyQPanel)
	EVT_COMMAND(wxID_ANY, wxEVT_F10HeadCtrl_KeyDown,OnF10HeadCtrlKeyDown)
	EVT_COMMAND(wxID_ANY, wxEVT_RichTextCtrlEx_KeyDown,OnF10HeadCtrlKeyDown)	
	
	EVT_COMMAND(wxID_ANY, wxEVT_RETURN_BAOJIABIAO,	OnReturnBaoJiaBiao)
	EVT_COMMAND(wxID_ANY, wxEVT_MyFrame_AddMySelect, OnMyFrameAddMySelect)
	EVT_COMMAND(wxID_ANY, wxEVT_MyFrame_MaxMize, OnMyFrameMaxMize)
END_EVENT_TABLE()

CHQFrame::CHQFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long type, WXHWND hWnd):
  wxDocMDIParentFrame(manager, frame, wxID_ANY, title, pos, size, type, _T("CHQFrame")),m_timerTrans(this,1114)
{
  g_pAppDlg = this;
  m_docManager->CreateDocument( wxEmptyString, wxDOC_NEW ); 
  editMenu = (wxMenu *) NULL;
  SetIcon(wxIcon(_T("IDI_ZQQUATION")));
  m_parent = hWnd;

  m_bInitTimerTrans = false;
  m_tmLastTrans.tm_year = 0;
  m_nTime = 500;

  m_nQueryNum = 0;
  m_pApi = PlusinAPI::GetInstance();
  m_pApi->Init("ZqQuotation", "123", OnRecvApiMsg, (HWND)m_parent, POS_QUOTE, (HWND)m_hWnd);
  m_pApi->Logon();//这个函数里面会调用onsize，所以所有的初始化都到这前面做

  m_pApi->Send(CMDID_CONSOLE_WND_SIZE_STATUS_QUERY, NULL, 0, GetNextRequestId()); //返回插件窗口大小状态，其中，0表示正常大小，1表示最大化

}

void CHQFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    (void)wxMessageBox(_T("DocView Demo\nAuthor: Julian Smart\nUsage: docview.exe"), _T("About DocView"));
}
void CHQFrame::OnClose(wxCloseEvent& evt)
{
	m_timerTrans.Stop();
	m_pApi->Logout();
	if ( NULL != m_pApi )
	{
		m_pApi->DestroyInstance();
		m_pApi = NULL;
	}
	ClearFData();

	m_timerTrans.Stop();
	evt.Skip();	
	//	Close(true);
	delete this;
	exit(1);	
}

// Creates a canvas. Called from view.cpp when a new drawing
// view is created.

CHQFrame *GetMainFrame(void)
{
  return g_pAppDlg;
}

void CHQFrame::OnReSendMessage(wxCommandEvent& event)
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
int CHQFrame::OnRecvApiMsg_Process( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
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
int CHQFrame::OnRecvApiMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId  )
{
	if(g_pAppDlg == NULL)
		return 0;

	SMessage *pMess = new SMessage(nCmdId, nErrorCode, pData, nDataLen, nRequestId);
	wxCommandEvent myEvent(wxEVT_ReSend_Message);
	myEvent.SetClientData(pMess);
	g_pAppDlg->AddPendingEvent(myEvent);

	return 0;
}
void CHQFrame::ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId == CMDID_CONSOLE_LOGON_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			m_myFrame->m_pTLineView->Show();
		//	m_myFrame->Show();
		//	Show();
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
	if(nCmdId == CMDID_CONSOLE_WND_SIZE_STATUS_RSP ||  CMDID_CONSOLE_WND_SIZE_STATUS_CHANGED == nCmdId)
	{
		int *p = (int *)pData;
		m_nMaxMize = (*p);
	}
}

void CHQFrame::ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
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

void CHQFrame::ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
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
			pFData->m_InstrumentName   = pInstrumentData->InstrumentName;
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

void CHQFrame::ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
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

		m_myFrame->InitGUI(pFData, NULL, false);

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
			m_myFrame->OnDataInTime(pMarketData, pFData, bUpdate);
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
		m_myFrame->InitGUIData(pFData, pQutoData);
		m_myFrame->InitGUI(pFData, pQutoData, true); //已有数据初始化界面
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

void CHQFrame::ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
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
void CHQFrame::ClearFData()
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
int CHQFrame::GetNextRequestId()
{
	m_nQueryNum++;
	return m_nQueryNum;
}
CFData* CHQFrame::GetFDataByQueryID(int nQueryNum)
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
void CHQFrame::SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales)
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
void CHQFrame::SetAddMySelect()
{
	m_pApi->Send(CMDID_QUOTE_ADD_TO_SELFGROUP_REQ, NULL, 0, GetNextRequestId());
}
void CHQFrame::SendMaxMize()
{
	int nValue = m_nMaxMize;
	if(nValue == 0)
		nValue = 1;
	else
		nValue = 0;
	m_pApi->Send(CMDID_CONSOLE_CHANGE_SIZE, &nValue, sizeof(int), GetNextRequestId());
}
int CHQFrame::GetMaxSize()
{
	return m_nMaxMize;
}
void CHQFrame::DelFData(int nCount)
{
	if((int)m_mapID2FData.size()>nCount )
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
void CHQFrame::OnSize(wxSizeEvent& event)
{
	if(m_myFrame == NULL || !::IsWindow((HWND)m_myFrame->GetHWND()))
		return;
	
	wxRect rtClient = GetClientRect();
	wxGetApp().m_subframe->SetSize(rtClient.width, rtClient.height);	
	m_myFrame->SetSize(rtClient.width, rtClient.height);

	if(::IsWindow((HWND)m_F10Panel.GetHWND()))
		m_F10Panel.SetSize(rtClient.width, rtClient.height);
}
void CHQFrame::OnFuncLDBClickKLine(wxCommandEvent& event)
{
	m_myFrame->m_pKLineView->Hide();
	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
}
void CHQFrame::OnFuncKeyDownTLine(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_RETURN)
	{
		m_myFrame->OnReturnKLine();	
	}
	else if(keyCode == WXK_ESCAPE)
	{
	//	OnReturnBaoJiaoBiao();
		m_myFrame->m_pKLineView->Hide();
		m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
	}
	else if(keyCode == VK_PRIOR)
	{
		OnPrior();
	}
	else if(keyCode == VK_NEXT)
	{
		OnNext();
	}
	else if(keyCode == VK_F10)
	{
		OnF10();
	}
}
void CHQFrame::OnPrior()
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
	m_myFrame->InitGUI(pFData, NULL, true);
//	m_F10Panel.SetFData(pFData);
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
void CHQFrame::OnNext()
{
	if(m_vecFData.size() == 0)
		return;
	++m_nFDataIndex;
	if(m_nFDataIndex >= (int)m_vecFData.size())
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
	m_myFrame->InitGUI(pFData, NULL, true);
//	m_F10Panel.SetFData(pFData);
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
void CHQFrame::OnFuncKeyDownKLine(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_RETURN)
	{
		//OnReturnBaoJiaoBiao();	
		m_myFrame->m_pKLineView->Hide();
		m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
	}
	else if(keyCode == WXK_ESCAPE)
	{
		//OnReturnBaoJiaoBiao();	
		m_myFrame->m_pKLineView->Hide();
		m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
	}
	else if(keyCode == VK_PRIOR)
	{
		OnPrior();
	}
	else if(keyCode == VK_NEXT)
	{
		OnNext();
	}
	else if(keyCode == VK_F10)
	{
		OnF10();
	}
}
void CHQFrame::OnFuncKeyQPanel(wxCommandEvent& event)
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
void CHQFrame::OnReturnBaoJiaBiao(wxCommandEvent& event)
{
	m_myFrame->m_pKLineView->Hide();
	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
}
void CHQFrame::OnTimer1(wxTimerEvent& event)
{
	int nID = event.GetId();

	if(nID == 1113)
	{
		/*		DWORD dwProcessId = 0;
		GetWindowThreadProcessId((HWND)m_parent, &dwProcessId);
		if(dwProcessId == 0)
		{
		close(false);
		delete this;
		exit(0);
		}*/
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
void CHQFrame::SetLastTransTime(struct tm tmLastTrans)
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
			//		pFData->SetLastTransTime(tmLastTrans); //对分周期图不生成不交易横盘k线
		}
		if(m_myFrame && m_myFrame->m_pTLineView)
			m_myFrame->m_pTLineView->SetLastTransTime(tmLastTrans);
		//	if(m_pKLineView)
		//		m_pKLineView->SetLastTransTime(tmLastTrans);
	}	
	//	m_tmLastTrans = tmLastTrans;
}
void CHQFrame::OnMyFrameAddMySelect(wxCommandEvent& event)
{
	m_pApi->Send(CMDID_QUOTE_ADD_TO_SELFGROUP_REQ, NULL, 0, GetNextRequestId());
}
void CHQFrame::OnMyFrameMaxMize(wxCommandEvent& event)
{
	int nValue = event.GetInt();
	m_pApi->Send(CMDID_CONSOLE_CHANGE_SIZE, &nValue, sizeof(int), GetNextRequestId());
}
void CHQFrame::OnF10()
{
	wxMDIChildFrame* childFrame = wxGetApp().GetSubFrame();
	if(childFrame == NULL)
		return;
	wxRect rtClient = childFrame->GetClientRect();
	
	if(!::IsWindow((HWND)m_F10Panel.GetHWND()))
		m_F10Panel.CreateDlg(childFrame, 31123, wxPoint(0, 0), wxSize(rtClient.GetWidth(), rtClient.GetHeight()) );
	
	CFData *pData = m_myFrame->GetFData();
	m_F10Panel.SetFData(pData);
	m_F10Panel.Show();
	m_F10Panel.SetFocus();
	m_myFrame->Show(false);
}
bool CHQFrame::InitFromCFG()
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
	if(m_myFrame)
		m_myFrame->InitFromCFG(root);

	
	m_F10Panel.InitCfg(root);

	return true;
}
void CHQFrame::OnF10HeadCtrlKeyDown(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_ESCAPE)
	{
		m_myFrame->Show();
		m_F10Panel.Show(false);
	}
}