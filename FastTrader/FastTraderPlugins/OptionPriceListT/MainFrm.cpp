// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "OptionPriceListT.h"

#include "MainFrm.h"
#include "DlgOptionTTable.h"
#include "DlgOptionTTablePopup.h"
#include <vector>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT nITFCMDARRIVE = ::RegisterWindowMessage("OptionPriceListT ApiMsg Arrive");

CMainFrame* g_poMainFrame = NULL;

int CMainFrame::CallbackRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{

	if(nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
	}
	else 
	{
#ifdef _DEBUG
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
			case CMDID_INSTRUMENT_INFO_RSP:
			case CMDID_QUOTE_CURRENT_SEL:
			case CMDID_ORDER_CURRENT_INSTRUENT_RSP:
			case CMDID_QUOTE_SNAPSHOT_RSP:
			case CMDID_QUOTE_SEND_QUOTE:
			case CMDID_QUOTE_CURRENT_GROUP_INST_LIST:
			case CMDID_QUOTE_GETCURRENTGROUPMEMBER_RSP:
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

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_bExitApp = FALSE;
	m_bWaitForNewTTableDlg = FALSE;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;

	m_dlgOptionTTable.Create(CDlgOptionTTable::IDD, this);
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_dlgOptionTTable.MoveWindow(&rect);
	m_dlgOptionTTable.ShowWindow(SW_SHOW);

	g_poMainFrame = this;

	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi==NULL) {
		poPlusinApi = PlusinAPI::GetInstance();
		SetPlusinAPI(poPlusinApi);
	}
	if(_GetRunMode()==conModePopup) 
		poPlusinApi->Init("OptionPriceListTPopup", "123", CallbackRecvApiMsg, GetTargetHWnd(), NEED_NO_POS_NO_BIND, GetSafeHwnd());
	else
		poPlusinApi->Init("OptionPriceListT", "123", CallbackRecvApiMsg, GetTargetHWnd(), POS_QUOTE, GetSafeHwnd());
	poPlusinApi->Logon();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	if(_GetRunMode()==conModePopup) {
	//cs.style &= ~WS_EX_STATICEDGE; 
	//cs.style &= ~WS_EX_CLIENTEDGE; 
	//cs.style &= ~WS_EX_WINDOWEDGE; 
	//cs.style &= ~WS_THICKFRAME; 
	//cs.style &= ~WS_DLGFRAME; 
	}
	
	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(message==nITFCMDARRIVE) {
		if(!m_bExitApp) {
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
	else if(message==GetTTableCloseMSGID()) {
		if(!m_bExitApp) {
			CWnd* pWnd = (CWnd*)lParam;
			
			LockGlobalObject();
			std::set<CWnd*>& setTTableDlg = GetTTableDlgSet();
			std::set<CWnd*>::iterator it = setTTableDlg.find(pWnd);
			if(it!=setTTableDlg.end()) {
				it=setTTableDlg.erase(it);
				//SIZE sizeDlg = {0};
				//CRect rectDlg;
				//pWnd->GetWindowRect(&rectDlg);
				//sizeDlg.cx = rectDlg.Width();
				//sizeDlg.cy = rectDlg.Height();
				//SetBookDlgSize(sizeDlg);
				delete pWnd;
			}
			UnlockGlobalObject();
		}
	}
	else if(message==GetColSetChangedMSGID()) {
		if(!m_bExitApp) {
			if(_GetRunMode()!=conModePopup) {
				m_dlgOptionTTable.ResetCols();
			}
			else {
				LockGlobalObject();
				std::set<CWnd*>& setTTableDlg = GetTTableDlgSet();
				std::set<CWnd*>::iterator it;
				for(it = setTTableDlg.begin(); it != setTTableDlg.end(); it++) {
					CDlgOptionTTablePopup* pDlg = (CDlgOptionTTablePopup*)(*it);
					if(pDlg != NULL)
						pDlg->ResetCols();
				}
				UnlockGlobalObject();
			}
		}
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	CFrameWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	::BringWindowToTop(m_dlgOptionTTable.GetSafeHwnd());
	m_dlgOptionTTable.SetFocus();
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent==conTimerRepaint) {
		KillTimer(conTimerRepaint);
		m_dlgOptionTTable.Invalidate();
	}

	CFrameWnd::OnTimer(nIDEvent);
}

//void CMainFrame::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: Add your message handler code here
//	// Do not call CFrameWnd::OnPaint() for painting messages
//
//	SYSTEMTIME sysTime;
//	::GetLocalTime(&sysTime);
//	TRACE("CMainFrame::OnPaint() %02d:%02d:%02d.%03d\n", 
//			sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
//	
//	//SetTimer(conTimerRepaint, 5, NULL);
//	////m_dlgOptionTTable.Invalidate();
//
//}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnClose();
}

void CMainFrame::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	if(nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
		ParseCmd_Console(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
	}
	else {
		switch(nCmdId) {
			//case CMDID_INSTRUMENT_SHORTNAME_RSP:
			case CMDID_EXCHANGE_LIST_RSP:
			case CMDID_INSTRUMENT_LIST_RSP:
			case CMDID_INSTRUMENT_INFO_RSP:
			case CMDID_QUOTE_CURRENT_SEL:
			case CMDID_ORDER_CURRENT_INSTRUENT_RSP:
			case CMDID_QUOTE_CURRENT_GROUP_INST_LIST:
			case CMDID_QUOTE_GETCURRENTGROUPMEMBER_RSP:
				ParseCmd_BaseBusiness(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
				break;

			case CMDID_QUOTE_SNAPSHOT_RSP:
			case CMDID_QUOTE_SEND_QUOTE:
				{
					if(nErrorCode == SUCCESS && pData!=NULL) {
						BOOL bIsOption = FALSE;
						CString strInstrumentID, strExchangeID, strTargetID, strPrice;
						BOOL bDirect = FALSE;
						double fPriceTick = 0.001;
						BOOL bFound = TRUE;
						PlatformStru_DepthMarketData* pDepthData = (PlatformStru_DepthMarketData*)pData;
						std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;

						LockGlobalObject();

						strInstrumentID = pDepthData->InstrumentID;
						it = GetInstrumentInfos().find(pDepthData->InstrumentID);
						if(it != GetInstrumentInfos().end()) {
							strExchangeID = it->second.ExchangeID;
							fPriceTick = it->second.PriceTick;
							bIsOption = it->second.ProductClass==THOST_FTDC_PC_Options;
							if(bIsOption) {
								ParseOptionFormat(CString(pDepthData->InstrumentID), strExchangeID, 
										strTargetID, bDirect, strPrice);
								FindTargetID(CString(pDepthData->InstrumentID), strInstrumentID); 
								//strInstrumentID = strTargetID;
							}
						}

						if(_GetRunMode()==conModePopup) {
							std::set<CWnd*>::iterator itDlg;
							std::set<CWnd*>& setDlg = GetTTableDlgSet();
							for(itDlg = setDlg.begin(); itDlg != setDlg.end(); itDlg++) {
								CDlgOptionTTablePopup* pDlg = (CDlgOptionTTablePopup*)(*itDlg);
								if(pDlg!=NULL) {
									bFound = pDlg->GetInstrumentID().CompareNoCase(strInstrumentID)==0 && 
											!pDlg->IsExiting();
									if(bFound) {
										pDlg->ParseQuot(pDepthData, bIsOption, strTargetID, 
												bDirect, strPrice, fPriceTick);
										break;
									}
								}
							}
						}
						
						UnlockGlobalObject();

						if(_GetRunMode()!=conModePopup) {
							m_dlgOptionTTable.ParseQuot(pDepthData, bIsOption, strTargetID, 
									bDirect, strPrice, fPriceTick);
						}
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
    if(dwProcessID==GetCurrentProcessId() && stricmp(csClassName, "#32770")==0)
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
    if(IsTopLevelWin_MayDestroy(hwnd)) {
		CWnd* pWnd = CWnd::FromHandle(hwnd);
		if(pWnd!=NULL && (pWnd->m_nFlags&WF_CONTINUEMODAL) && (pWnd->m_nFlags&WF_MODALLOOP)) {
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), NULL);
		}
		else {
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
			if(nErrorCode==SUCCESS) {
				QueryProductShortNames();
				QueryExchangeIDs();
			}
			else {
				::MessageBox(NULL, "期权T型报价功能初始化失败，无法登录到FastTrader", "错误提示", MB_ICONERROR|MB_OK|MB_SYSTEMMODAL);
				::PostMessage(GetSafeHwnd(), WM_CLOSE, 0, NULL);
			}
			break;
		case CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW:
			//ShowWindow(SW_SHOW);			
//			SetWindowPos(GetForegroundWindow(), 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
			if(_GetRunMode()==conModePopup) {
				m_bWaitForNewTTableDlg = TRUE;
				if(GetInitStatus()) {
					QueryCurrInstrumentID();
				}
			}
			break;
		case CMDID_CONSOLE_CLOSE:
			{
				m_bExitApp = TRUE;
				::EnumWindows(EnumWinFunc,(LPARAM)this); 
				LockGlobalObject();
				std::set<CWnd*> setDlg = GetTTableDlgSet();
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
					strncpy(pstrProductShortNames, (char*)pData, nDataLen);
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
				if(nErrorCode == SUCCESS && nDataLen%sizeof(CommonExchangeIDType)==0 && pData!=NULL) {
					std::set<std::string> setExchange;
					char strExchange[sizeof(CommonExchangeIDType)+4];
					while(!m_queueExchangeForQuery.empty())
						m_queueExchangeForQuery.pop();
					for (int i=0; i<nDataLen/(int)sizeof(CommonExchangeIDType); i++) {
						ZeroMemory(strExchange, sizeof(strExchange));
						strncpy(strExchange, (char*)pData+i*sizeof(CommonExchangeIDType), 
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
				else {
					//AfxMessageBox("一键下单功能初始化失败，无法获得交易所列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_INSTRUMENT_LIST_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen%sizeof(InstrumentIDType)==0) {
					char szInstrumentID[sizeof(InstrumentIDType)+4];
					for(int i=0; i<nDataLen/(int)sizeof(InstrumentIDType); i++) {
						ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
						strncpy(szInstrumentID, (char*)pData+i*sizeof(InstrumentIDType), sizeof(InstrumentIDType));
						LockGlobalObject();
						GetInstrumentIDList().insert(std::string(szInstrumentID));
						UnlockGlobalObject();
					}
				}
				if(nErrorCode==SUCCESS && 
						((pData!=NULL && nDataLen%sizeof(InstrumentIDType)==0) || 
						(pData==NULL && nDataLen==0))) {
					if(!m_queueExchangeForQuery.empty()) {
						CString strExchangeID(m_queueExchangeForQuery.front().c_str());
						m_queueExchangeForQuery.pop();
						QueryInstrumentIDs(strExchangeID);
					}
					else {
						std::set<std::string>::iterator it;
						std::set<std::string> setInstrument;
						LockGlobalObject();
						setInstrument = GetInstrumentIDList();
						UnlockGlobalObject();

						if(setInstrument.size()>0) {
							for(it = setInstrument.begin(); it != setInstrument.end(); it++) {
								m_queueInstrumentForQuery.push(*it);
							}
							CString strInstrumentID(m_queueInstrumentForQuery.front().c_str());
							m_queueInstrumentForQuery.pop();
							QueryInstrumentInfo(strInstrumentID);
						}
						else {
							//SetInitStatus(TRUE);
							//if(m_bWaitForNewTTableDlg) {
							//	QueryCurrInstrumentID();
							//	//ShowNewBookOrderDialog(CString(""));
							//}
						}
					}
				}
				else {
					//AfxMessageBox("一键下单功能初始化失败，无法获得合约列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_INSTRUMENT_INFO_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_InstrumentInfo)) {
					PlatformStru_InstrumentInfo* pInfo = (PlatformStru_InstrumentInfo*)pData;
					if(pInfo->ProductClass==_GetNewOptionType())
						pInfo->ProductClass=THOST_FTDC_PC_Options;
					LockGlobalObject();
					GetInstrumentInfos().insert(std::make_pair(std::string(pInfo->InstrumentID), *pInfo));
					UnlockGlobalObject();
					if(!m_queueInstrumentForQuery.empty()) {
						CString strInstrumentID(m_queueInstrumentForQuery.front().c_str());
						m_queueInstrumentForQuery.pop();
						QueryInstrumentInfo(strInstrumentID);
					}
					else {
						LockGlobalObject();
						ParseOptionInstrument(GetInstrumentInfos());
						UnlockGlobalObject();
						SetInitStatus(TRUE);
						if(_GetRunMode()!=conModePopup) {
							QueryCurrInstrumentList();
						}
						else {
							if(m_bWaitForNewTTableDlg) {
								QueryCurrInstrumentID();
								//ShowNewBookOrderDialog(CString(""));
							}
						}
					}
				}
				else {
					//AfxMessageBox("一键下单功能初始化失败，无法获得合约信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_QUOTE_CURRENT_GROUP_INST_LIST:
			if(GetInitStatus() && nErrorCode==SUCCESS && pData!=NULL && nDataLen%64==0) {
				char* csInstrID = (char*)pData;
				std::vector<std::string> vecInstr;
				int nItemCount = nDataLen/64;

				for(int i=0; i<nItemCount; i++) {
					vecInstr.push_back(std::string(csInstrID+i*64));
				}
				m_dlgOptionTTable.ResetInstrumentList(vecInstr);
				ActiveDialog(m_dlgOptionTTable.GetSafeHwnd());
			}
			else {
				//AfxMessageBox("一键下单功能初始化失败，无法获得合约信息", MB_ICONERROR|MB_OK);
			}
			break;
		case CMDID_QUOTE_GETCURRENTGROUPMEMBER_RSP:
			if(nErrorCode==SUCCESS && pData!=NULL && nDataLen%32==0) {
				char* csInstrID = (char*)pData;
				std::vector<std::string> vecInstr;
				int nItemCount = nDataLen/32;

				for(int i=0; i<nItemCount; i++) {
					vecInstr.push_back(std::string(csInstrID+i*32));
				}
				m_dlgOptionTTable.ResetInstrumentList(vecInstr);
				ActiveDialog(m_dlgOptionTTable.GetSafeHwnd());
			}
			else {
				//AfxMessageBox("一键下单功能初始化失败，无法获得合约信息", MB_ICONERROR|MB_OK);
			}
			break;
		case CMDID_QUOTE_CURRENT_SEL:
			break;
		case CMDID_ORDER_CURRENT_INSTRUENT_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(InstrumentIDType)) {
					CString strInstrID;
					char szCurrInstrumentID[sizeof(InstrumentIDType)+4];
					ZeroMemory(szCurrInstrumentID, sizeof(szCurrInstrumentID));
					strncpy(szCurrInstrumentID, (char*)pData, nDataLen);
					strInstrID = szCurrInstrumentID;
					if(m_bWaitForNewTTableDlg) {
						m_bWaitForNewTTableDlg = FALSE;
						if(strlen(szCurrInstrumentID)==0) {
							::MessageBox(NULL, "未正确选择合约，相关功能暂无法生效", "错误提示", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
						}
						else {
							BOOL bFound = FALSE;

							LockGlobalObject();
							bFound = GetOptionTargetRelate().find(szCurrInstrumentID) != GetOptionTargetRelate().end();
							if(!bFound) {
								bFound = FindTargetID(CString(szCurrInstrumentID), strInstrID);
							}
							UnlockGlobalObject();
							if(bFound) {
								if(!m_bExitApp)
									ShowNewTTableDialog(strInstrID);
							}
							else {
								::MessageBox(NULL, "无效的标的物合约，无法显示T型报价", "错误提示", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
							}
						}
					}
				}
				else {
					if(m_bWaitForNewTTableDlg) {
						m_bWaitForNewTTableDlg = FALSE;
						::MessageBox(NULL, "未正确选择合约，相关功能暂无法生效", "错误提示", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
						//if(!m_bExitApp)
						//	ShowNewTTableDialog(CString(""));
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
		strncpy(szExchangeID, strExchangeID, sizeof(szExchangeID)-1);
		poPlusinApi->Send(CMDID_INSTRUMENT_LIST_QUERY, szExchangeID, 
				sizeof(szExchangeID), CSequenceManager::GetNextSeqNum());
	}
}

void CMainFrame::QueryInstrumentInfo(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)-1);
		poPlusinApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CMainFrame::QueryCurrInstrumentID()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
		poPlusinApi->Send(CMDID_ORDER_CURRENT_INSTRUENT_QUERY, NULL, 0, 
				CSequenceManager::GetNextSeqNum());
}

void CMainFrame::QueryCurrInstrumentList()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
		poPlusinApi->Send(CMDID_QUOTE_GETCURRENTGROUPMEMBER_QUERY, NULL, 0, 
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
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
	::SetForegroundWindow(hWnd); 
	::SetFocus(hWnd);
	::AttachThreadInput(dwCurID, dwForeID, FALSE);
}

void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default
	
#if 0
	int myCaptionHeight = 0;
	int myLeftBorderWeight = 0;
	int myRightBorderWeight = 0;
	int myBottomBorderWeight = 0;
	
	if(bCalcValidRects) {
		CRect& rc = (CRect&)lpncsp->rgrc[0];//get the client rectangle
#if 1
		rc.top += myCaptionHeight - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYDLGFRAME) - GetSystemMetrics(SM_CYFRAME);
		rc.left += myLeftBorderWeight - GetSystemMetrics(SM_CXDLGFRAME) - GetSystemMetrics(SM_CXFRAME);
		rc.bottom -= myBottomBorderWeight - GetSystemMetrics(SM_CYDLGFRAME) - GetSystemMetrics(SM_CYFRAME);
		rc.right -= myRightBorderWeight - GetSystemMetrics(SM_CXDLGFRAME) - GetSystemMetrics(SM_CXFRAME);
#else
		rc.top += myCaptionHeight - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYDLGFRAME);
		rc.left += myLeftBorderWeight - GetSystemMetrics(SM_CXDLGFRAME);
		rc.bottom -= myBottomBorderWeight - GetSystemMetrics(SM_CYDLGFRAME);
		rc.right -= myRightBorderWeight - GetSystemMetrics(SM_CXDLGFRAME);
#endif
	}
#else
	//if(bCalcValidRects) {
	//	CRect& rc = (CRect&)lpncsp->rgrc[0];//get the client rectangle
	//	rc.top += -GetSystemMetrics(SM_CYFRAME);
	//	rc.left += -GetSystemMetrics(SM_CXFRAME);
	//	rc.bottom -= -GetSystemMetrics(SM_CYFRAME);
	//	rc.right -= -GetSystemMetrics(SM_CXFRAME);
	//}
#endif

	CFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(GetSafeHwnd()!=NULL && m_dlgOptionTTable.GetSafeHwnd()!=NULL) {
		CRect rect;
		//GetWindowRect(&rect);
		//ScreenToClient(&rect);
		GetClientRect(&rect);
		m_dlgOptionTTable.MoveWindow(&rect);
	}
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(conTimerRepaint);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(bShow && GetInitStatus())
		QueryCurrInstrumentList();
}

void CMainFrame::ShowNewTTableDialog(CString& strInstrumentID)
{
	BOOL bIsFound = FALSE;
	CDlgOptionTTablePopup* pDlg = NULL;
	CDlgOptionTTablePopup* pFoundDlg = NULL;
	CDlgOptionTTablePopup* pEmptyDlg = NULL;

	LockGlobalObject();
	std::set<CWnd*>& setDlg = GetTTableDlgSet();
	std::set<CWnd*>::iterator it = setDlg.begin();
	int nDlgCount = setDlg.size();
	for(; it != setDlg.end(); it++) {
		pDlg = (CDlgOptionTTablePopup*)(*it);
		if(pDlg!=NULL) {
			if(pDlg->GetInstrumentID().GetLength()==0) {
				pEmptyDlg = pDlg;
			}
			if(!bIsFound) {
				if(strInstrumentID.CompareNoCase(pDlg->GetInstrumentID())==0) {
					pFoundDlg = pDlg;
					bIsFound = TRUE;
				}
			}
		}
	}
	UnlockGlobalObject();

	if(bIsFound && strInstrumentID.GetLength()!=0) {
		if(pFoundDlg!=NULL)
			ActiveDialog(pFoundDlg->GetSafeHwnd());
		//::MessageBox(NULL, "该合约窗口已存在，不可重复开启", "警告", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
		return;
	}
	//if(nDlgCount>=4) {
	//	::MessageBox(NULL, "已打开窗口过多，请先关闭一键下单窗口后再尝试此操作", "警告", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
	//	return;
	//}
	if(pEmptyDlg!=NULL && strInstrumentID.GetLength()==0)
		return;

	if(pEmptyDlg==NULL) {
		pDlg = new CDlgOptionTTablePopup(NULL);
		pDlg->Create(CDlgOptionTTablePopup::IDD);
	}
	else {
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
