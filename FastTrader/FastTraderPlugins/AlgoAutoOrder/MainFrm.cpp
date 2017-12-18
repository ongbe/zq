// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"

#include "MainFrm.h"
#include "AlgoAutoOrderDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT nITFCMDARRIVE = ::RegisterWindowMessage("AlgoAutoOrder ApiMsg Arrive");

CMainFrame* g_poMainFrame = NULL;

int CMainFrame::CallbackRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{
	if(nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
	}
	else 
	{
#ifdef _DEBUG
//         if(nCmdId==CMDID_ORDER_RSP_ORDER_INSERT1)
//             TRACE("Receive CMDID_ORDER_RSP_ORDER_INSERT1");
//         else if(nCmdId==CMDID_ORDER_RSP_ORDER_INSERT2)
//             TRACE("Receive CMDID_ORDER_RSP_ORDER_INSERT2");
//         else if(nCmdId==CMDID_ORDER_CANCEL_ORDER_RSP)
//             TRACE("Receive CMDID_ORDER_CANCEL_ORDER_RSP\n");    
//         else if(nCmdId==CMDID_ORDER_RSP_ORDER_ACTION1)
//             TRACE("Receive CMDID_ORDER_RSP_ORDER_ACTION1\n");
//         else if(nCmdId==CMDID_ORDER_RSP_ORDER_ACTION2)
//             TRACE("Receive CMDID_ORDER_RSP_ORDER_ACTION2\n");
//         else if(nCmdId==CMDID_ORDER_ORDER_STATUS_CHANGE)
//             TRACE("Receive CMDID_ORDER_ORDER_STATUS_CHANGE\n");
        
//         if(nCmdId==CMDID_ORDER_TRADE_RTN)
//             TRACE("Receive CMDID_ORDER_TRADE_RTN\n");
//         if (nCmdId==CMDID_ORDER_TRADE_RTN)  
//             AfxMessageBox("nCmdId==CMDID_ORDER_TRADE_RTN\n");      

		//if(nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP");
		//if(nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP");
		//else if(nCmdId==CMDID_CONFIG_CHANGE)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_CHANGE");
		//else if(nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP");
		//else if(nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP");
		//else if(nCmdId==CMDID_CONFIG_INPUT_VOLUMES_RSP)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_INPUT_VOLUMES_RSP");
		//else if(nCmdId==CMDID_ALL_ORDER_LIST_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_LIST_RSP");
		//else if(nCmdId==CMDID_ALL_ORDER_INFO_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_INFO_RSP");
#endif
			//case CMDID_QUOTE_CURRENT_SEL_RSP:
		switch(nCmdId) 
		{
			case CMDID_INSTRUMENT_SHORTNAME_RSP:
			case CMDID_EXCHANGE_LIST_RSP:
			case CMDID_INSTRUMENT_LIST_RSP:
			case CMDID_QUOTE_CURRENT_SEL:
			case CMDID_ORDER_CURRENT_INSTRUENT_RSP:

			case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
			case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
			case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
			case CMDID_INSTRUMENT_INFO_RSP:
			case CMDID_POSITION_INFO_RSP:
			case CMDID_POSITION_INFO_LIST_RSP:
			case CMDID_QUOTE_SNAPSHOT_RSP:

			case CMDID_ALL_ORDER_LIST_RSP:
			case CMDID_ALL_ORDER_INFO_RSP:

			case CMDID_QUOTE_SEND_QUOTE:
			case CMDID_ORDER_ORDER_STATUS_CHANGE:
			case CMDID_ORDER_TRADE_RTN:

			case CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP:
			case CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP:

			case CMDID_CONFIG_CHANGE:
			case CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP:
			case CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP:
			case CMDID_CONFIG_INPUT_VOLUMES_RSP:
            case CMDID_ORDER_INSERT_ORDER_RSP:
            case CMDID_ORDER_RSP_ORDER_INSERT1:
            case CMDID_ORDER_RSP_ORDER_INSERT2:
            case CMDID_ORDER_CANCEL_ORDER_RSP:
            case CMDID_ORDER_RSP_ORDER_ACTION1:				
            case CMDID_ORDER_RSP_ORDER_ACTION2:				
				break;
			default:
				return -1;
				break;
		}
	}

	ITFCMDATA* pITFData = new ITFCMDATA;
	ZeroMemory(pITFData, sizeof(ITFCMDATA));
	pITFData->nSeqNum = nRequestId;
	pITFData->nCmd = nCmdId;
	pITFData->nErrorCode = nErrorCode;
	pITFData->nDataLen = nDataLen;
	if(nDataLen>0) {
		BYTE* pNewData = new BYTE[nDataLen];
		memcpy(pNewData, pData, nDataLen);
		pITFData->pData = pNewData;
	}
	if(g_poMainFrame!=NULL && nITFCMDARRIVE!=0) {
		g_poMainFrame->PostMessage(nITFCMDARRIVE, sizeof(ITFCMDATA), (LPARAM)pITFData);
	}
	else {
		delete pITFData;
	}
	return 0;
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_bExitApp = FALSE;
	m_bWaitForNewBookOrder = FALSE;
}

CMainFrame::~CMainFrame()
{

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	g_poMainFrame = this;

	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi==NULL) {
		poPlusinApi = PlusinAPI::GetInstance();
		SetPlusinAPI(poPlusinApi);
	}
	poPlusinApi->Init("AlgoAutoOrder", "123", CallbackRecvApiMsg, GetTargetHWnd(), NEED_NO_POS_NO_BIND, GetSafeHwnd());
	poPlusinApi->Logon();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==nITFCMDARRIVE) {
		if(!m_bExitApp)
		{
			ITFCMDATA* pITFData = (ITFCMDATA*)lParam;
    		if(pITFData != NULL) {
				ParseCmd(pITFData->nCmd, pITFData->nErrorCode, pITFData->pData, pITFData->nDataLen, pITFData->nSeqNum);
				if(pITFData->pData!=NULL) {
					delete[] pITFData->pData;
					pITFData->pData = NULL;
				}
				delete pITFData;
				pITFData = NULL;
			}
		}
	}
	else if(message==GetBookOrderCloseMSGID())
	{
		if(!m_bExitApp)
		{
			CWnd* pWnd = (CWnd*)lParam;
			//pWnd->ShowWindow(FALSE);
            
			LockGlobalObject();
			std::set<CWnd*>& setBookOrderDlg = GetBookOrderDlgSet();
			std::set<CWnd*>::iterator it = setBookOrderDlg.find(pWnd);
			if(it!=setBookOrderDlg.end()) 
			{
				it=setBookOrderDlg.erase(it);
				SIZE sizeDlg = {0};
				CRect rectDlg;
				pWnd->GetWindowRect(&rectDlg);
				sizeDlg.cx = rectDlg.Width();
				sizeDlg.cy = rectDlg.Height();
				SetBookDlgSize(sizeDlg);
				delete pWnd;
			}
			UnlockGlobalObject();
		}
	}
	return __super::WindowProc(message, wParam, lParam);
}

// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	if(nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
		ParseCmd_Console(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
	}
	else {
		switch(nCmdId) {
			case CMDID_INSTRUMENT_SHORTNAME_RSP:
			case CMDID_EXCHANGE_LIST_RSP:
			case CMDID_INSTRUMENT_LIST_RSP:
			case CMDID_QUOTE_CURRENT_SEL:
			case CMDID_ORDER_CURRENT_INSTRUENT_RSP:
				ParseCmd_BaseBusiness(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
				break;
			case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
			case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
			case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
			case CMDID_INSTRUMENT_INFO_RSP:
			case CMDID_POSITION_INFO_RSP:
			case CMDID_POSITION_INFO_LIST_RSP:
			case CMDID_QUOTE_SNAPSHOT_RSP:

			case CMDID_ALL_ORDER_LIST_RSP:
			case CMDID_ALL_ORDER_INFO_RSP:

			case CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP:
			case CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP:

			case CMDID_CONFIG_CHANGE:
			case CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP:
			case CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP:
			case CMDID_CONFIG_INPUT_VOLUMES_RSP:
            case CMDID_ORDER_INSERT_ORDER_RSP:
            case CMDID_ORDER_CANCEL_ORDER_RSP:
			
				{
					CAlgoAutoOrderDlg* pDlg = NULL;
					LockGlobalObject();
					CSequenceManager& objSeqMng = GetSequenceManager();
					pDlg = (CAlgoAutoOrderDlg*)objSeqMng.GetSeqNumObject(nSeqNum);
					if(pDlg!=NULL && !pDlg->IsExiting())
					{
						pDlg->ParseCmd(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
					}
					UnlockGlobalObject();
				}
				break;
			case CMDID_ORDER_ORDER_STATUS_CHANGE:
			case CMDID_ORDER_TRADE_RTN:
			case CMDID_QUOTE_SEND_QUOTE:
 				{
					BOOL bIsSuccess = TRUE;
					if(!(nErrorCode == SUCCESS && pData!=NULL))
					{
						bIsSuccess = FALSE;
					}
					else
					{
						if(nCmdId==CMDID_ORDER_ORDER_STATUS_CHANGE
								&& nDataLen%sizeof(PlatformStru_OrderInfo)==0 && nDataLen>0)
							bIsSuccess = TRUE;
						else if(nCmdId==CMDID_ORDER_TRADE_RTN
								&& nDataLen%sizeof(PlatformStru_TradeInfo)==0 && nDataLen>0)
							bIsSuccess = TRUE;
						else if(nCmdId==CMDID_QUOTE_SEND_QUOTE
								&& nDataLen%sizeof(PlatformStru_DepthMarketData)==0 && nDataLen>0)
							bIsSuccess = TRUE;
						else
							bIsSuccess = FALSE;
					}
					if(bIsSuccess)
					{
						CAlgoAutoOrderDlg* pDlg = NULL;
						char szInstrumentID[sizeof(InstrumentIDType)+4];
						switch(nCmdId)
						{
							case CMDID_ORDER_ORDER_STATUS_CHANGE:
								{
								PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)pData;
								ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
								strncpy_s(szInstrumentID, pOrderData->InstrumentID, sizeof(InstrumentIDType));
								}
								break;
							case CMDID_ORDER_TRADE_RTN:
								{
								PlatformStru_TradeInfo* pTradeData = (PlatformStru_TradeInfo*)pData;
								ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
								strncpy_s(szInstrumentID, pTradeData->InstrumentID, sizeof(InstrumentIDType));
								}
								break;
							case CMDID_QUOTE_SEND_QUOTE:
								{
                                //TRACE("CMDID_QUOTE_SEND_QUOTE 推送合约行情\n");                                
								PlatformStru_DepthMarketData* pDepthData = (PlatformStru_DepthMarketData*)pData;
								ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
								strncpy_s(szInstrumentID, pDepthData->InstrumentID, sizeof(InstrumentIDType));
                                //TRACE("PlatformStru_DepthMarketData.NewVolume= %d\n",pDepthData->NewVolume);
								}
								break;
						};

						LockGlobalObject();
						std::set<CWnd*>& setDlg = GetBookOrderDlgSet();
						for(std::set<CWnd*>::iterator it = setDlg.begin(); 
								it != setDlg.end(); it++)
						{
							pDlg = (CAlgoAutoOrderDlg*)(*it);
							if(pDlg!=NULL && !pDlg->IsExiting() && pDlg->GetInstrumentID().CompareNoCase(szInstrumentID)==0)
							{
								pDlg->ParseCmd(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
							}
						}
						UnlockGlobalObject();
					}
				}
				break;
            case CMDID_ORDER_RSP_ORDER_INSERT1:
            case CMDID_ORDER_RSP_ORDER_INSERT2:
                {
                    BOOL bIsSuccess = TRUE;
                    if(!(nErrorCode == SUCCESS && pData!=NULL))
                    {
                        bIsSuccess = FALSE;
                    }
                    else
                    {
                        if(nCmdId==CMDID_ORDER_RSP_ORDER_INSERT1
                            && nDataLen%sizeof(PlatformStru_RspOrderInsert1)==0 && nDataLen>0)
                            bIsSuccess = TRUE;
                        else if(nCmdId==CMDID_ORDER_RSP_ORDER_INSERT2
                            && nDataLen%sizeof(PlatformStru_RspOrderInsert2)==0 && nDataLen>0)
                            bIsSuccess = TRUE;
                        else
                            bIsSuccess = FALSE;
                    }
                    if(bIsSuccess)
                    {
                        LockGlobalObject();
                        CAlgoAutoOrderDlg* pDlg = NULL;
                        std::set<CWnd*>& setDlg = GetBookOrderDlgSet();
                        for(std::set<CWnd*>::iterator it = setDlg.begin(); 
                            it != setDlg.end(); it++)
                        {
                            pDlg = (CAlgoAutoOrderDlg*)(*it);
                            if(pDlg!=NULL && !pDlg->IsExiting())
                            {
                                pDlg->ParseCmd(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
                            }
                        }
                        UnlockGlobalObject();
                    }
                }
                break;
            case CMDID_ORDER_RSP_ORDER_ACTION1:				
            case CMDID_ORDER_RSP_ORDER_ACTION2:	
                {
                    BOOL bIsSuccess = TRUE;
                    if(!(nErrorCode == SUCCESS && pData!=NULL))
                    {
                        bIsSuccess = FALSE;
                    }
                    else
                    {
                        if(nCmdId==CMDID_ORDER_RSP_ORDER_ACTION1
                            && nDataLen%sizeof(PlatformStru_RspOrderAction1)==0 && nDataLen>0)
                            bIsSuccess = TRUE;
                        else if(nCmdId==CMDID_ORDER_RSP_ORDER_ACTION2
                            && nDataLen%sizeof(PlatformStru_RspOrderAction2)==0 && nDataLen>0)
                            bIsSuccess = TRUE;
                        else
                            bIsSuccess = FALSE;
                    }
                    if(bIsSuccess)
                    {
                        LockGlobalObject();
                        CAlgoAutoOrderDlg* pDlg = NULL;
                        std::set<CWnd*>& setDlg = GetBookOrderDlgSet();
                        for(std::set<CWnd*>::iterator it = setDlg.begin(); 
                            it != setDlg.end(); it++)
                        {
                            pDlg = (CAlgoAutoOrderDlg*)(*it);
                            if(pDlg!=NULL && !pDlg->IsExiting())
                            {
                                pDlg->ParseCmd(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
                            }
                        }
                        UnlockGlobalObject();
                    }
                }
                break;
		};
	}
}

BOOL IsTopLevelWin_MayDestroy(HWND hwnd)
{
    DWORD dwProcessID;
	char csClassName[1024]={0};
    GetWindowThreadProcessId(hwnd, &dwProcessID);
	::GetClassName(hwnd, csClassName, 1024);
    if(dwProcessID==GetCurrentProcessId() && _stricmp(csClassName, "#32770")==0)
    {
#ifdef _DEBUG
		char csCaption[1024]={0};
		::GetWindowText(hwnd,csCaption,1024);
		CString strText;
		strText.Format("IsTopLevelWin_MayDestroy() Item:%08X %s(%s)\n", hwnd, csCaption, csClassName);
		TRACE(strText);
#endif
		return TRUE;
    }
	return FALSE;
}

BOOL CALLBACK EnumWinFunc(HWND hwnd, LPARAM lParam)
{
    if(IsTopLevelWin_MayDestroy(hwnd))
	{
		CWnd* pWnd = CWnd::FromHandle(hwnd);
		if(pWnd!=NULL && (pWnd->m_nFlags&WF_CONTINUEMODAL) && (pWnd->m_nFlags&WF_MODALLOOP))
		{
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), NULL);
		}
		else
		{
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), NULL);
			Sleep(10);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDYES, BN_CLICKED), NULL);
		}
	}
	return TRUE;
}

void CMainFrame::ParseCmd_Console(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_CONSOLE_LOGON_RSP:
			if(nErrorCode==SUCCESS)
			{
				QueryProductShortNames();
				QueryExchangeIDs();
			}
			else
			{
				::MessageBox(NULL, "盯盘建仓平仓功能初始化失败，无法登录到FastTrader", "错误提示", MB_ICONERROR|MB_OK|MB_SYSTEMMODAL);
				::PostMessage(GetSafeHwnd(), WM_CLOSE, 0, NULL);
			}
			break;
		case CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW:
			m_bWaitForNewBookOrder = TRUE;
			if(GetInitStatus()) 
			{
				QueryCurrInstrumentID();
			}
			break;
		case CMDID_CONSOLE_CLOSE:
			{
				m_bExitApp = TRUE;
				::EnumWindows(EnumWinFunc,(LPARAM)this); 
				LockGlobalObject();
				std::set<CWnd*> setDlg = GetBookOrderDlgSet();
				std::set<CWnd*>::iterator it = setDlg.begin();
				for(; it != setDlg.end(); it++) 
				{
					::SetWindowLong((*it)->GetSafeHwnd(), GWL_HWNDPARENT, (long)this->GetSafeHwnd());
				}
				ReleaseGlobalObject();
				UnlockGlobalObject();
				::EnumWindows(EnumWinFunc,(LPARAM)this); 

				::PostMessage(GetSafeHwnd(), WM_CLOSE, 0, NULL);
			}
			break;
		default:
			break;
	};
}

void CMainFrame::ParseCmd_BaseBusiness(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_INSTRUMENT_SHORTNAME_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL) 
				{
					char* pstrProductShortNames = new char[nDataLen+8];
					ZeroMemory(pstrProductShortNames, nDataLen+8);
					strncpy_s(pstrProductShortNames, nDataLen+8, (char*)pData, nDataLen);
					SetProductShortNames(CString(pstrProductShortNames));
					delete[] pstrProductShortNames;
					pstrProductShortNames = NULL;
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得产品简称表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_EXCHANGE_LIST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(CommonExchangeIDType)==0 && pData!=NULL)
				{
					std::set<std::string> setExchange;
					char strExchange[sizeof(CommonExchangeIDType)+4];
					while(!m_queueExchangeForQuery.empty())
						m_queueExchangeForQuery.pop();
					for (int i=0; i<(int)(nDataLen/sizeof(CommonExchangeIDType)); i++)
					{
						ZeroMemory(strExchange, sizeof(strExchange));
						strncpy_s(strExchange, (char*)pData+i*sizeof(CommonExchangeIDType), 
								sizeof(CommonExchangeIDType));
						m_queueExchangeForQuery.push(std::string(strExchange));
						setExchange.insert(std::string(strExchange));
					}
					LockGlobalObject();
					SetExchangeIDList(setExchange);
					UnlockGlobalObject();
					CString strExchangeID(m_queueExchangeForQuery.front().c_str());
					m_queueExchangeForQuery.pop();
					QueryInstrumentIDs(strExchangeID);
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得交易所列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_INSTRUMENT_LIST_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen%sizeof(InstrumentIDType)==0)
				{
					char szInstrumentID[sizeof(InstrumentIDType)+4];
					for(int i=0; i<(int)(nDataLen/sizeof(InstrumentIDType)); i++)
					{
						ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
						strncpy_s(szInstrumentID, (char*)pData+i*sizeof(InstrumentIDType), sizeof(InstrumentIDType));
						LockGlobalObject();
						GetInstrumentIDList().insert(std::string(szInstrumentID));
						UnlockGlobalObject();
					}
				}
				if(nErrorCode==SUCCESS && 
						((pData!=NULL && nDataLen%sizeof(InstrumentIDType)==0) || (pData==NULL && nDataLen==0)))
				{
					if(!m_queueExchangeForQuery.empty()) {
						CString strExchangeID(m_queueExchangeForQuery.front().c_str());
						m_queueExchangeForQuery.pop();
						QueryInstrumentIDs(strExchangeID);
					}
					else 
					{
						SetInitStatus(TRUE);
						if(m_bWaitForNewBookOrder)
						{
							QueryCurrInstrumentID();
							//ShowNewBookOrderDialog(CString(""));
						}
					}
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法合约列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_QUOTE_CURRENT_SEL:
			break;
		case CMDID_ORDER_CURRENT_INSTRUENT_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(InstrumentIDType))
				{
					char szCurrInstrumentID[sizeof(InstrumentIDType)+4];
					ZeroMemory(szCurrInstrumentID, sizeof(szCurrInstrumentID));
					strncpy_s(szCurrInstrumentID, (char*)pData, nDataLen);
					if(m_bWaitForNewBookOrder)
					{
						m_bWaitForNewBookOrder = FALSE;
// 						if(strlen(szCurrInstrumentID)==0)
// 							::MessageBox(NULL, "未正确选择合约，相关功能暂无法生效", "错误提示", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
						if(!m_bExitApp)
							ShowNewBookOrderDialog(CString(szCurrInstrumentID));
					}
				}
				else 
				{
					if(m_bWaitForNewBookOrder)
					{
						m_bWaitForNewBookOrder = FALSE;
// 						::MessageBox(NULL, "未正确选择合约，相关功能暂无法生效", "错误提示", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
						if(!m_bExitApp)
							ShowNewBookOrderDialog(CString(""));
					}
				}
			}
			break;
		default:
			break;
	}
}

void CMainFrame::QueryProductShortNames()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
		poPlusinApi->Send(CMDID_INSTRUMENT_SHORTNAME_QUERY, NULL, 0, 
				CSequenceManager::GetNextSeqNum());
}

void CMainFrame::QueryExchangeIDs()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
		poPlusinApi->Send(CMDID_EXCHANGE_LIST_QUERY, NULL, 0, 
				CSequenceManager::GetNextSeqNum());
}

void CMainFrame::QueryInstrumentIDs(CString& strExchangeID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		char szExchangeID[32];
		ZeroMemory(szExchangeID, sizeof(szExchangeID));
		strncpy_s(szExchangeID, strExchangeID, strExchangeID.GetLength());
		poPlusinApi->Send(CMDID_INSTRUMENT_LIST_QUERY, szExchangeID, 
				sizeof(szExchangeID), CSequenceManager::GetNextSeqNum());
	}
}

void CMainFrame::QueryCurrInstrumentID()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
		poPlusinApi->Send(CMDID_ORDER_CURRENT_INSTRUENT_QUERY, NULL, 0, 
				CSequenceManager::GetNextSeqNum());
}

void CMainFrame::ActiveDialog(HWND hWnd)
{
	DWORD dwForeID = 0, dwCurID = 0; 
	HWND hForeWnd = ::GetForegroundWindow(); 
	dwCurID = ::GetCurrentThreadId(); 
	dwForeID = ::GetWindowThreadProcessId(hForeWnd, NULL); 
	::AttachThreadInput(dwCurID, dwForeID, TRUE); 
	::ShowWindow(hWnd, SW_SHOW);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
	//::SetWindowPos(hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
	::SetForegroundWindow(hWnd); 
	::AttachThreadInput(dwCurID, dwForeID, FALSE);
}

void CMainFrame::ShowNewBookOrderDialog(CString& strInstrumentID)
{

	BOOL bIsFound = FALSE;
	CAlgoAutoOrderDlg* pDlg = NULL;
	CAlgoAutoOrderDlg* pFoundDlg = NULL;
	CAlgoAutoOrderDlg* pEmptyDlg = NULL;

	LockGlobalObject();
	std::set<CWnd*>& setDlg = GetBookOrderDlgSet();
	std::set<CWnd*>::iterator it = setDlg.begin();
	int nDlgCount = setDlg.size();
	for(; it != setDlg.end(); it++) 
	{
		pDlg = (CAlgoAutoOrderDlg*)(*it);
		if(pDlg!=NULL)
		{
			if(pDlg->GetInstrumentID().GetLength()==0)
			{
				pEmptyDlg = pDlg;
			}
			if(!bIsFound)
			{
				if(strInstrumentID.CompareNoCase(pDlg->GetInstrumentID())==0) 
				{
					pFoundDlg = pDlg;
					bIsFound = TRUE;
				}
			}
		}
	}
	UnlockGlobalObject();

	if(bIsFound && strInstrumentID.GetLength()!=0) 
	{
		if(pFoundDlg!=NULL)
			ActiveDialog(pFoundDlg->GetSafeHwnd());
		//::MessageBox(NULL, "该合约窗口已存在，不可重复开启", "警告", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
		return;
	}
	if(nDlgCount>=4) 
	{
		::MessageBox(NULL, "已打开窗口过多，请先关闭盯盘建仓平仓窗口后再尝试此操作", "警告", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
		return;
	}
	if(pEmptyDlg!=NULL && strInstrumentID.GetLength()==0)
		return;

	if(pEmptyDlg==NULL)
	{
		pDlg = new CAlgoAutoOrderDlg(NULL);
        pDlg->m_strInstrumentName = strInstrumentID;
		pDlg->Create(CAlgoAutoOrderDlg::IDD);
	}
	else
	{
		pDlg = pEmptyDlg;
	}
	pDlg->SetInstrumentID(strInstrumentID);
	::SetWindowLong(pDlg->GetSafeHwnd(), GWL_HWNDPARENT, (long)::GetDesktopWindow());
	//CRect rectDlgClient;
	//pDlg->GetClientRect(&rectDlgClient);
	//pDlg->ResetDlgSize(rectDlgClient.Width(), rectDlgClient.Height());
	//pDlg->Invalidate();
	ActiveDialog(pDlg->GetSafeHwnd());

	LockGlobalObject();
	setDlg.insert(pDlg);
	UnlockGlobalObject();
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CFrameWnd::OnClose();
}
