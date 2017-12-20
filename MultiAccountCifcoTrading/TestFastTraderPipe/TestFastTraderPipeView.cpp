// TestFastTraderPipeView.cpp : implementation of the CTestFastTraderPipeView class
//

#include "stdafx.h"
#include "TestFastTraderPipe.h"

#include "TestFastTraderPipeDoc.h"
#include "TestFastTraderPipeView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HEARTBEATTIMERID 0x88776655
#define MASTERNAMEDPIPESTRING "\\\\.\\pipe\\FastTrader\\v0_1\\Master"


UINT g_nRspMessageMSNID = ::RegisterWindowMessage("Receive a response message");


ISessionIDType	g_SessionID = 0;
HANDLE	g_hAppPipe = NULL;
HANDLE	g_hAppPipeThread = NULL;
CWnd*	g_pWndMsg = NULL;
CString g_strMessage("Hello");
DWORD	g_dwPushToAppPipeSuccessSum = 0;
DWORD	g_dwPushToAppPipeFailSum = 0;

DWORD WINAPI proc_AppPipe(LPVOID lpvParam)
{
	
	DWORD cbBytesRead=0, cbReplyBytes=0, cbWritten=0; 
	BOOL fSuccess=FALSE; 
 
	DWORD dwTotalBytesAvail = 0;
	DWORD dwMode = PIPE_READMODE_BYTE; 
	BYTE* pReqBuff = NULL; 
	IPackageHeadStru tPackHead;

	while(1) { 

		if(!ConnectNamedPipe(g_hAppPipe, NULL)) {
			if(GetLastError() == ERROR_PIPE_CONNECTED)
				break;
			Sleep(1);
		}

		SetNamedPipeHandleState(g_hAppPipe, &dwMode, NULL, NULL); 
		
		dwTotalBytesAvail = 0;
		do {
			Sleep(1);
			PeekNamedPipe(g_hAppPipe, NULL, 0, NULL, &dwTotalBytesAvail, NULL);
		} while(dwTotalBytesAvail==0);

		if(dwTotalBytesAvail > sizeof(IPackageHeadStru)) {
			pReqBuff = new BYTE[dwTotalBytesAvail];
			ZeroMemory(pReqBuff, dwTotalBytesAvail);
			ReadFile(g_hAppPipe, pReqBuff, dwTotalBytesAvail, &cbBytesRead, NULL);

			if(cbBytesRead != dwTotalBytesAvail) {
				delete pReqBuff;
				DisconnectNamedPipe(g_hAppPipe);
				continue;
			}
			
			ZeroMemory(&tPackHead, sizeof(IPackageHeadStru));
			memcpy(&tPackHead, pReqBuff, sizeof(IPackageHeadStru));

			if(tPackHead.SyncID != ISynIDNum) {
				delete pReqBuff;
				DisconnectNamedPipe(g_hAppPipe);
				continue;
			}
			if(tPackHead.CMDID==ICMD_B_IamFine) {
				TRACE("SessionID := %x, Has receive FastTrader 'I am fine' Message.\n", 
						tPackHead.SessionID);
				g_strMessage.Format("SessionID := %x, Has receive FastTrader 'I am fine' Message.\n", 
						tPackHead.SessionID);
				if(g_pWndMsg!=NULL)
					g_pWndMsg->PostMessage(g_nRspMessageMSNID, 0, NULL);
			}
			else if(tPackHead.CMDID==ICMD_B_rspHeartBeat) {
				memcpy(&g_dwPushToAppPipeSuccessSum, 
						pReqBuff+sizeof(IPackageHeadStru), sizeof(DWORD));
				memcpy(&g_dwPushToAppPipeFailSum, 
						pReqBuff+sizeof(IPackageHeadStru)+sizeof(DWORD), sizeof(DWORD));
				TRACE("SessionID := %x, Has receive FastTrader 'Response Heart Beat' Message.\n", 
						tPackHead.SessionID);
				g_strMessage.Format("SessionID := %x, Has receive FastTrader 'Response Heart Beat' Message.\n", 
						tPackHead.SessionID);
				if(g_pWndMsg!=NULL)
					g_pWndMsg->PostMessage(g_nRspMessageMSNID, 0, NULL);
			}
			else if(tPackHead.CMDID==ICMD_F_rspDepthMarket) {
				TRACE("SessionID := %x, Has receive FastTrader 'Response Depth Market' Message.\n", 
						tPackHead.SessionID);
				g_strMessage.Format("SessionID := %x, Has receive FastTrader 'Response Depth Market' Message.\n", 
						tPackHead.SessionID);
				if(g_pWndMsg!=NULL)
					g_pWndMsg->PostMessage(g_nRspMessageMSNID, 0, NULL);
			}
			else if(tPackHead.CMDID==ICMD_D_ReturnExchangeDateTime) {
				TRACE("SessionID := %x, Has receive FastTrader 'Return Exchange DateTime' Message.\n%s\n", 
						tPackHead.SessionID, pReqBuff+sizeof(IPackageHeadStru));
				
				BYTE *p = pReqBuff+sizeof(IPackageHeadStru);
				g_strMessage.Format("SessionID := %x, Has receive FastTrader 'Return Exchange DateTime' Message.\n%s\n", 
						tPackHead.SessionID, pReqBuff+sizeof(IPackageHeadStru));
				if(g_pWndMsg!=NULL)
					g_pWndMsg->PostMessage(g_nRspMessageMSNID, 0, NULL);
			}
			else if(tPackHead.CMDID==ICMD_D_ReturnOrderInfo) {
				TRACE("SessionID := %x, Has receive FastTrader 'Return AllOrder' Message.\n%s\n", 
						tPackHead.SessionID, pReqBuff+sizeof(IPackageHeadStru));
				
				DWORD nCount = *(DWORD*)(pReqBuff+sizeof(IPackageHeadStru));
				PlatformStru_OrderInfo *p = NULL;
				for(int i=0; i<nCount; i++) {
					p = (PlatformStru_OrderInfo*)(pReqBuff+sizeof(IPackageHeadStru)+sizeof(DWORD)+i*sizeof(PlatformStru_OrderInfo));
					int n = p->OrderStatus;
				}
			}
			else if(tPackHead.CMDID==ICMD_D_ReturnPositionInfo) {
				TRACE("SessionID := %x, Has receive FastTrader 'Return AllPosition' Message.\n%s\n", 
						tPackHead.SessionID, pReqBuff+sizeof(IPackageHeadStru));
				
				DWORD nCount = *(DWORD*)(pReqBuff+sizeof(IPackageHeadStru));
				PlatformStru_Position *p = NULL;
				for(int i=0; i<nCount; i++) {
					p = (PlatformStru_Position*)(pReqBuff+sizeof(IPackageHeadStru)+sizeof(DWORD)+i*sizeof(PlatformStru_Position));
					int n = p->Position;
				}
			}
			else if(tPackHead.CMDID==ICMD_D_ReturnDepthMarket) {
				TRACE("SessionID := %x, Has receive FastTrader 'Return DepthMarket' Message.\n%s\n", 
						tPackHead.SessionID, pReqBuff+sizeof(IPackageHeadStru));
				
				PlatformStru_DepthMarketData *p = NULL;
				if(tPackHead.dwRspMsgNum==0) {
					p = (PlatformStru_DepthMarketData*)(pReqBuff+sizeof(IPackageHeadStru));
					int n = p->AskPrice1;
				}
			}
			else {
				TRACE("SessionID := %x, Has receive FastTrader Other Message.\n", 
						tPackHead.SessionID);
				g_strMessage.Format("SessionID := %x, Has receive FastTrader Other Message.\n", 
						tPackHead.SessionID);
				if(g_pWndMsg!=NULL)
					g_pWndMsg->PostMessage(g_nRspMessageMSNID, 0, NULL);
			}
			delete pReqBuff;
		}
		DisconnectNamedPipe(g_hAppPipe);
	} 
 
	DisconnectNamedPipe(g_hAppPipe); 
	
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeView

IMPLEMENT_DYNCREATE(CTestFastTraderPipeView, CFormView)

BEGIN_MESSAGE_MAP(CTestFastTraderPipeView, CFormView)
	//{{AFX_MSG_MAP(CTestFastTraderPipeView)
	ON_BN_CLICKED(IDC_BTNLOGINFASTTRADER, On_Btn_LoginFastTrader)
	ON_BN_CLICKED(IDC_BTNHELLOFASTTRADER, On_Btn_HelloFastTrader)
	ON_BN_CLICKED(IDC_BTNHEARTBEAT, On_Btn_HeartBeat)
	ON_BN_CLICKED(IDC_BTNAUTOHEARTBEAT, On_Btn_AutoHeartBeat)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNLOGOUTFASTTRADER, On_Btn_LogoutFastTrader)
	ON_BN_CLICKED(IDC_BTNEXCHANGEDATETIME, On_Btn_ExchangeDateTime)
	ON_BN_CLICKED(IDC_BTNQRYALLORDER, On_Btn_QryAllOrder)
	ON_BN_CLICKED(IDC_BTNQRYALLPOSITION, On_Btn_QryAllPosition)
	ON_BN_CLICKED(IDC_BTNQRYDEPTHMARKET, On_Btn_QryDepthMarket)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeView construction/destruction

CTestFastTraderPipeView::CTestFastTraderPipeView()
	: CFormView(CTestFastTraderPipeView::IDD)
{
	//{{AFX_DATA_INIT(CTestFastTraderPipeView)
	m_strMasterHost = _T(".");
	m_strMasterPID = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CTestFastTraderPipeView::~CTestFastTraderPipeView()
{
}

void CTestFastTraderPipeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestFastTraderPipeView)
	DDX_Control(pDX, IDC_RICHEDITMSG, m_ctlRichTextMsg);
	DDX_Text(pDX, IDC_EDITMASTERHOST, m_strMasterHost);
	DDV_MaxChars(pDX, m_strMasterHost, 128);
	DDX_Text(pDX, IDC_EDITMASTERPID, m_strMasterPID);
	DDV_MaxChars(pDX, m_strMasterPID, 10);
	//}}AFX_DATA_MAP
}

BOOL CTestFastTraderPipeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTestFastTraderPipeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	if(g_pWndMsg == NULL) {
		g_pWndMsg = this;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeView printing

BOOL CTestFastTraderPipeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestFastTraderPipeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestFastTraderPipeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestFastTraderPipeView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeView diagnostics

#ifdef _DEBUG
void CTestFastTraderPipeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTestFastTraderPipeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTestFastTraderPipeDoc* CTestFastTraderPipeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestFastTraderPipeDoc)));
	return (CTestFastTraderPipeDoc*)m_pDocument;
}
#endif //_DEBUG

LRESULT CTestFastTraderPipeView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	//已收到消息: 
	if(message == g_nRspMessageMSNID) {
//		int nLineIndex=0, nLineLen=0;
//		//GetDlgItem(IDC_LABELRSPMESSAGE)->SetWindowText(g_strMessage);
//		CString strText;
//		CTime time=CTime::GetCurrentTime();
//		//strText=time.Format("%Y-%m-%d %H:%M:%S.%, ");
//		strText=time.Format("%c, ");
//		strText+=g_strMessage;
//		nLineIndex = m_ctlRichTextMsg.LineIndex(500);
//		if(nLineIndex!=-1) {
//			nLineLen = m_ctlRichTextMsg.LineLength(nLineIndex);
//			m_ctlRichTextMsg.SetSel(nLineIndex, nLineIndex+nLineLen+2);
//			m_ctlRichTextMsg.ReplaceSel("", TRUE);
//		}
//		m_ctlRichTextMsg.SetSel(0, 0);
//		m_ctlRichTextMsg.ReplaceSel(strText, TRUE);
//		TRACE(strText);
		ShowMsg(g_strMessage, m_ctlRichTextMsg);
		CMainFrame* pMain = (CMainFrame*)::AfxGetMainWnd();
		CString strText;
		strText.Format("ToAppPipeSuccess := %u, ToAppPipeFail := %u", 
				g_dwPushToAppPipeSuccessSum, g_dwPushToAppPipeFailSum);
		pMain->ResetStatusBarMsg(strText);
	}
	return CFormView::WindowProc(message, wParam, lParam);
}

void CTestFastTraderPipeView::ShowMsg(CString& strMsg, CRichEditCtrl& ctlRichText) 
{
	int nLineIndex=0, nLineLen=0;
	//GetDlgItem(IDC_LABELRSPMESSAGE)->SetWindowText(g_strMessage);
	CString strText;
	CTime time=CTime::GetCurrentTime();
	//strText=time.Format("%Y-%m-%d %H:%M:%S.%, ");
	strText=time.Format("%c, ");
	strText+=strMsg;
	nLineIndex = ctlRichText.LineIndex(500);
	if(nLineIndex!=-1) {
		nLineLen = ctlRichText.LineLength(nLineIndex);
		ctlRichText.SetSel(nLineIndex, nLineIndex+nLineLen+2);
		ctlRichText.ReplaceSel("", TRUE);
	}
	ctlRichText.SetSel(0, 0);
	ctlRichText.ReplaceSel(strText, TRUE);
	//TRACE(strText);
}

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeView message handlers

void CTestFastTraderPipeView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDEvent == HEARTBEATTIMERID) {
		//On_Btn_HelloFastTrader();
		//Sleep(15);
		On_Btn_HeartBeat();
	}

	CFormView::OnTimer(nIDEvent);
}

void CTestFastTraderPipeView::On_Btn_LoginFastTrader() 
{
	// TODO: Add your control notification handler code here

	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_NOWAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	ILOGINDATA loginData={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = 0;
	packHead.MsgType = 1;
	packHead.CMDID = ICMD_A_Login;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(ILOGINDATA)+sizeof(PackageTailType);
	packHead.CRC = 0;

	char strComName[128];
	DWORD dwLen = 128;
	GetComputerName(strComName, &dwLen);

	strcpy(loginData.strAppID, "FASTTRADER");
	strcpy(loginData.strLicense, "AABBCCDD");
	strncpy(loginData.strHostName, strComName, dwLen);
	strcpy(loginData.strUserName, "DemoUser");
	strcpy(loginData.strPassword, "password");
	strcpy(loginData.strTraderAccount, "TA_0001");
	loginData.chUseQuot = '0';
	loginData.chUseOrder = '1';
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, &loginData, sizeof(ILOGINDATA));
	dwPos += sizeof(ILOGINDATA);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	DWORD dwStartTime = ::GetTickCount();
	DWORD dwTime = 0;

	dwTotalBytesAvail = 0;
	do {
		Sleep(1);
		PeekNamedPipe(hPipe, NULL, 0, NULL, &dwTotalBytesAvail, NULL);
	} while(dwTotalBytesAvail==0);
	//} while(dwTotalBytesAvail==0 && ::GetTickCount()-dwStartTime<500);

	if(dwTotalBytesAvail > sizeof(IPackageHeadStru)) {
	
		BYTE* pBuff = new BYTE[dwTotalBytesAvail];
		ZeroMemory(pBuff, dwTotalBytesAvail);
		ReadFile(hPipe, pBuff, dwTotalBytesAvail, &cbBytesRead, NULL);
		IPackageHeadStru* pRspPackHead = (IPackageHeadStru*)pBuff;
		ILOGINRETURN* pRspLogin = (ILOGINRETURN*)(pBuff+sizeof(IPackageHeadStru));

		g_SessionID = pRspPackHead->SessionID;

		TRACE("Has receive sessionID := %u, App pipe is '%s'.\n", 
				g_SessionID, pRspLogin->strAppPipeName);	
		if(g_hAppPipeThread!=NULL) {
			TerminateThread(g_hAppPipeThread, -1);
			CloseHandle(g_hAppPipeThread);
			g_hAppPipeThread=NULL;
		}
		if(g_hAppPipe!=NULL) {
			//DisconnectNamedPipe(g_hAppPipe);
			CloseHandle(g_hAppPipe);
			g_hAppPipe=NULL;
		}
		Sleep(5);
		if((g_hAppPipe = CreateNamedPipe(pRspLogin->strAppPipeName, 
				PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
				PIPE_UNLIMITED_INSTANCES, IBUFSIZE, IBUFSIZE, 3000, NULL))
				!=INVALID_HANDLE_VALUE) {
			DWORD dwAppPipeThreadId = 0;
			g_hAppPipeThread = CreateThread(NULL, 0, proc_AppPipe, 
					NULL, 0, &dwAppPipeThreadId); 
			CString strText;
			strText.Format("已创建命名管道: %s", pRspLogin->strAppPipeName);
			GetDlgItem(IDC_LABELAPPPIPE)->SetWindowText(strText);
		}
		else {
			g_hAppPipe = NULL;
		}
		
		delete pBuff;
	}
	CloseHandle(hPipe); 
	
}

void CTestFastTraderPipeView::On_Btn_HelloFastTrader() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = 1;
	packHead.CMDID = ICMD_A_Hello;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType);
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
	
}

void CTestFastTraderPipeView::On_Btn_HeartBeat() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = 1;
	packHead.CMDID = ICMD_A_HeartBeat;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType);
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
		
}

void CTestFastTraderPipeView::On_Btn_AutoHeartBeat() 
{
	// TODO: Add your control notification handler code here
	SetTimer(HEARTBEATTIMERID, 20000, NULL);
}

void CTestFastTraderPipeView::On_Btn_LogoutFastTrader() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = 1;
	packHead.CMDID = ICMD_A_Logout;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType);
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
		
}

void CTestFastTraderPipeView::On_Btn_ExchangeDateTime() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = IMSGTYPE_C;
	packHead.CMDID = ICMD_C_QueryExchangeDateTime;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType)+32;
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	strcpy((char*)(cbBuff+dwPos), "SHFE");
	dwPos += 32;
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
	
}

void CTestFastTraderPipeView::On_Btn_QryAllOrder() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = 3;
	packHead.CMDID = ICMD_C_QueryOrderInfo;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType);
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
		
}

void CTestFastTraderPipeView::On_Btn_QryAllPosition() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = 3;
	packHead.CMDID = ICMD_C_QueryPositionInfo;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType);
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
		
}

void CTestFastTraderPipeView::On_Btn_QryDepthMarket() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData())
		return;

	CString strMasterPipe(MASTERNAMEDPIPESTRING);
	strMasterPipe.Replace(".", m_strMasterHost);
	strMasterPipe+="\\";
	strMasterPipe+=m_strMasterPID;
	
	if(!WaitNamedPipe(strMasterPipe, NMPWAIT_USE_DEFAULT_WAIT)) {
		CString strMessage("FastTrader is busy, Please try again after waiting a moment.\r\n");
		ShowMsg(strMessage, m_ctlRichTextMsg);
		return;
	}

	BOOL fSuccess = FALSE; 
	DWORD cbWritten, dwMode; 
 
	IPackageHeadStru packHead={0};
	PackageTailType packTail = ITailNum;

	IPackageHeadStru rspPackHead={0};

	packHead.SyncID = ISynIDNum;
	packHead.SessionID = g_SessionID;
	packHead.MsgType = 3;
	packHead.CMDID = ICMD_C_QueryDepthMarket;
	packHead.dwRspMsgNum = 0;
	packHead.Length = sizeof(IPackageHeadStru)+64+sizeof(PackageTailType);
	packHead.CRC = 0;
	
	DWORD dwPos = 0;
	BYTE cbBuff[IBUFSIZE];
	ZeroMemory(cbBuff, IBUFSIZE);

	char strInstrumentID[64];
	ZeroMemory(strInstrumentID, sizeof(strInstrumentID));
	strcpy(strInstrumentID, "IF1204");

	memcpy(cbBuff, &packHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	memcpy(cbBuff+dwPos, strInstrumentID, sizeof(strInstrumentID));
	dwPos += sizeof(strInstrumentID);
	memcpy(cbBuff+dwPos, &packTail, sizeof(PackageTailType));
	
	HANDLE hPipe = CreateFile(strMasterPipe.GetBuffer(0), GENERIC_READ | GENERIC_WRITE, 
         0, NULL, OPEN_EXISTING, 0, NULL);
	if(hPipe == INVALID_HANDLE_VALUE) { 
		//if(GetLastError() != ERROR_PIPE_BUSY) 
			return;
	}

	dwMode = PIPE_READMODE_BYTE; 
	SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

	DWORD dwStep = 0, dwTotalBytesAvail = 0, dwSumLen = 0, cbBytesRead = 0;
	fSuccess = WriteFile(hPipe, cbBuff, packHead.Length, &cbWritten, NULL); 

	Sleep(5);
	CloseHandle(hPipe); 
	
}
