// Plusin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Psapi.h>
#include <Windows.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include "Plusin.h"
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#include "../inc/Module-Misc/constants.h"
#include "../ISvr/PlatformSvrMgr.h"
#include "../ISvr/KernelStruct.h"
#include "PlusInMgrDlg.h"
#include "PlusinConfigHelper.h"
#include "../inc/Module-Misc/MD5.h"
#include "TCPDataService.h"

// This is the constructor of a class that has been exported.
// see Plusin.h for the class definition
extern PlatformSvrMgr* g_pPlatformMgr;
static CPlusin* s_pPlusin = NULL;

#define PLUSIN_SUBSCRIBE_INSTRUMENT_MAX_NUM	10

CPlusin::CPlusin()
: m_pFnCreatePlusPanel(NULL)
, m_pFnDeletePlusPanel(NULL)
, m_pPlusInDlg(NULL)
, m_bInit(false)
, m_hSendThreadHandle(NULL)
, m_bSendThreadExit(false)
, m_pParentHWnd(NULL)
, m_strOrderModuleCurInstrumentID(wxEmptyString)
{
	InitializeCriticalSection(&m_cs);
	m_mapAppID2Plusin.clear();
	m_setSysPlusIn.clear();
	m_quoteSubscribe.clear();
	m_setPlusinWaitForShow.clear();
	wxASSERT(s_pPlusin==NULL);
	s_pPlusin = this;
}

CPlusin::~CPlusin()
{
	if ( m_bInit )
	{
		UnInit();
	}

	DeleteCriticalSection(&m_cs);
	s_pPlusin = NULL;
}

bool CPlusin::Init( wxWindow* pParent, CreatePlugPane createFunc, DeletePlugPane deleteFunc )
{
	wxASSERT( !m_bInit );
	m_pParentHWnd = (HWND)pParent->GetHWND();
	m_pFnCreatePlusPanel = createFunc;
	m_pFnDeletePlusPanel = deleteFunc;

	SubscribeBusinessData();

	//创建发送数据线程
	m_hSendThreadHandle = CreateThread(NULL, 65535, 
		(LPTHREAD_START_ROUTINE)SendThreadFunc, this, 0,NULL);
	if(m_hSendThreadHandle==NULL)
	{
		DWORD dwErr=GetLastError();
		return false;
	}
	m_bInit = true;
	return true;
}

void CPlusin::UnInit()
{
	UnSubscribeBusinessData();

	m_quoteSubscribe.clear();
	if ( m_hSendThreadHandle )
	{
		m_bSendThreadExit = true;
		DWORD dwExitCode;
		WaitForSingleObject( m_hSendThreadHandle, 1000 );
		if( GetExitCodeThread(m_hSendThreadHandle, &dwExitCode) !=0 && dwExitCode == STILL_ACTIVE )
		{
			TerminateThread( m_hSendThreadHandle, 0 );
		}

		CloseHandle(m_hSendThreadHandle);
		m_hSendThreadHandle = NULL;
	}

	SaveUserPlusIn();
	CloseAllPlusin();

	m_bInit = false;
	m_pParentHWnd = NULL;
	m_pFnCreatePlusPanel = NULL;
	m_pFnDeletePlusPanel = NULL;

	EnterCriticalSection(&m_cs);
	while( !m_msgQueue.empty() )
	{
		TargetMsg targetMsg = m_msgQueue.front();
		m_msgQueue.pop();
		if ( NULL != targetMsg.msg.lpData )
		{
			delete[] targetMsg.msg.lpData;
			targetMsg.msg.lpData = NULL;
		}
	}
	LeaveCriticalSection(&m_cs);
}

BOOL CPlusin::ProcessMessage( WPARAM wParam, COPYDATASTRUCT* pCopyDataStruct )
{
	if ( NULL == pCopyDataStruct || !m_bInit )
	{
		return FALSE;
	}

	if ( NULL == pCopyDataStruct->lpData || pCopyDataStruct->cbData < sizeof(int))
	{
		return FALSE;
	}

	HWND hSourceWnd = (HWND)wParam;
	int nErrorCode = HIWORD(pCopyDataStruct->dwData);
	int nCMDID = LOWORD(pCopyDataStruct->dwData);
	int nRequestId = *(int*)pCopyDataStruct->lpData;
	void* pData = NULL;
	int nDataLen = 0;
	if ( pCopyDataStruct->cbData > sizeof(int))
	{
		pData = (char*)pCopyDataStruct->lpData + sizeof(int);
		nDataLen = pCopyDataStruct->cbData - sizeof(int);
	}

	if ( nCMDID >= CMDID_CONSOLE_MIN && nCMDID <= CMDID_CONSOLE_MAX)
	{
		return ProcessConsoleMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_ACCOUNT_MIN && nCMDID <= CMDID_ACCOUNT_MAX )
	{
		return ProcessAccountMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_INSTRUMENT_MIN && nCMDID <= CMDID_INSTRUMENT_MAX )
	{
		return ProcessInstrumentMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_ALL_ORDER_MIN && nCMDID <= CMDID_ALL_ORDER_MAX )
	{
		return ProcessAllOrderMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_POSITION_MIN && nCMDID <= CMDID_POSITION_MAX )
	{
		return ProcessPositionMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_EXCHANGE_MIN && nCMDID <= CMDID_EXCHANGE_MAX )
	{
		return ProcessExchangeMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_QUOTE_MIN && nCMDID <= CMDID_QUOTE_MAX )
	{
		return ProcessQuoteMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_ORDER_INSERT_MIN && nCMDID <= CMDID_ORDER_INSERT_MAX )
	{
		return ProcessOrderMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
	else if ( nCMDID >= CMDID_CONFIG_MIN && nCMDID <= CMDID_CONFIG_MAX )
	{
		return ProcessConfigMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}

	return FALSE;
}

void CPlusin::ReleasePlusInRC()
{
	//先将句柄无效，或者没有收到心跳的插件清除
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
	for ( ; it != m_mapAppID2Plusin.end(); it++)
	{
		if ( it->second.bStarted )
		{
			HWND hWnd = it->second.PluginMsgHWnd;
			if ( !IsWindow( hWnd ))
			{
				ReleasePlusInRC(it->second.ShowPos, it->first );
				it->second.bStarted = false;
				it->second.ContainerHWnd = NULL;
				it->second.dwLastUpdateTime = 0;
				it->second.PluginMsgHWnd = NULL;
				it->second.PlusinTopHWnd = NULL;

				if ( m_pPlusInDlg != NULL )
				{
					m_pPlusInDlg->UpdatePlusIn( it->first, it->second );
				}
			}
			else
			{
				DWORD dwTime = (GetTickCount() - it->second.dwLastUpdateTime)/1000;
				if ( dwTime > HEARTBEAT_RATE*2 && DEBUG_ON == 0)
				{
					ReleasePlusInRC(it->second.ShowPos, it->first );
					it->second.bStarted = false;
					it->second.ContainerHWnd = NULL;
					it->second.dwLastUpdateTime = 0;
					it->second.PluginMsgHWnd = NULL;
					it->second.PlusinTopHWnd = NULL;

					if ( m_pPlusInDlg != NULL )
					{
						m_pPlusInDlg->UpdatePlusIn( it->first, it->second );
					}

					//关闭插件进程
					DWORD dwProcID;
					::GetWindowThreadProcessId( hWnd, &dwProcID );
					HANDLE handle = ::OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcID );
					if ( NULL != handle )
					{
						::TerminateProcess(handle, 0);
					}
				}
			}
		}
	}
}

void CPlusin::ClosePlusInProc( HWND hWnd )
{
	if ( !IsWindow( hWnd ))
	{
		return;
	}

	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_CLOSE, SUCCESS);
	targetMsg.msg.lpData = new int(0);
	targetMsg.msg.cbData = sizeof(int);

	::SendMessage(targetMsg.hWnd, WM_COPYDATA, 
		(WPARAM)(m_pParentHWnd), (LPARAM)(&targetMsg.msg));

	delete targetMsg.msg.lpData;
	targetMsg.msg.lpData = NULL;
}

bool CPlusin::UnRegisterPlusin( const wxString& strAppID )
{
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.find( strAppID );
	if ( it != m_mapAppID2Plusin.end() )
	{
		//系统插件不允许删除
		std::set<std::string>::iterator it_sys = m_setSysPlusIn.find(it->second.strPath);
		if ( it_sys != m_setSysPlusIn.end())
		{
			return false;
		}

		if ( it->second.bStarted )
		{
			ClosePlusInProc(it->second.PluginMsgHWnd);
		}
		
		ReleasePlusInRC(it->second.ShowPos, it->first );

		if ( it->second.unMenuPos != MENU_POS_NONE )
		{
			wxCommandEvent myEvent(wxEVT_PLUSIN_MENU_CHANGE);
			myEvent.SetId( it->second.unMenuPos );
			GETTOPWINDOW()->AddPendingEvent(myEvent);
		}
		it = m_mapAppID2Plusin.erase(it);
	}

	return true;
}

void CPlusin::ReleasePlusInRC( int nPos, const wxString& strAppID )
{
	if ( !m_bInit )
	{
		return;
	}
    if(m_pFnDeletePlusPanel)
		m_pFnDeletePlusPanel(nPos, strAppID );
}

void CPlusin::SaveUserPlusIn()
{
	string strPath("user.plugin");
	GlobalFunc::GetPath(strPath);
	wxFile file;
	if(file.Open(strPath.c_str(),wxFile::write))
	{
		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); ++it)
		{
			set<string>::iterator it_set = m_setSysPlusIn.find(it->second.strPath);
			if ( it_set == m_setSysPlusIn.end() )
			{
				string strPlusInPath = it->second.strPath;
				strPlusInPath +="\r\n";
				file.Write(strPlusInPath.c_str(),strPlusInPath.length());
			}
		}
		
		file.Close();
	}
}

void CPlusin::LoadPlusin()
{
	//加载系统插件
	string strPath("system.plugin");
	GlobalFunc::GetPath(strPath);
	if(wxFileExists(strPath.c_str()))
	{
		wxTextFile sysFile;
		if(sysFile.Open(strPath.c_str()))
		{
			int nLineCount = sysFile.GetLineCount();
			for ( int i = 0; i < nLineCount; i++)
			{
				wxString strLine = sysFile.GetLine(i);
				strLine.Trim();
				if ( strLine.IsEmpty())
				{
					continue;
				}

				string strPlusInPath( strLine.c_str() );
				strPlusInPath = "plugin\\" + strPlusInPath;
				GlobalFunc::GetPath(strPlusInPath);

				if ( RegisterPlusin(strPlusInPath.c_str(), false))
				{
					m_setSysPlusIn.insert( strPlusInPath );
					OpenPlusInProc( strPlusInPath.c_str() );
				}
			}
			sysFile.Close();
		}
	}
	//加载用户插件
	strPath = "user.plugin";
	GlobalFunc::GetPath(strPath);
	if(wxFileExists(strPath.c_str()))
	{
		wxTextFile userFile(strPath.c_str());
		if(userFile.Open(strPath.c_str()))
		{
			int nLineCount = userFile.GetLineCount();
			for ( int i = 0; i < nLineCount; i++)
			{
				wxString strPlusinPath = userFile.GetLine(i);
				RegisterPlusin(strPlusinPath, false);
			}
			userFile.Close();
		}
	}

	//发送插件菜单改变消息
	UINT nMenuPos = MENU_POS_NONE;
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
	for ( ; it != m_mapAppID2Plusin.end(); ++it )
	{
		nMenuPos |= it->second.unMenuPos;
	}

	if ( nMenuPos != MENU_POS_NONE )
	{
		wxCommandEvent myEvent(wxEVT_PLUSIN_MENU_CHANGE);
		myEvent.SetId( nMenuPos );
		GETTOPWINDOW()->AddPendingEvent(myEvent);
	}
}

void CPlusin::ShowPlusinDlg(wxWindow* pParent)
{
	if ( !m_bInit || NULL == pParent )
	{
		return;
	}

	PlusInMgrDlg dlg(pParent, m_mapAppID2Plusin, *this);
	m_pPlusInDlg = &dlg;
	dlg.ShowModal();
	m_pPlusInDlg = NULL;
}

void CPlusin::CloseAllPlusin()
{
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
	for ( ; it != m_mapAppID2Plusin.end(); ++it)
	{
		ClosePlusInProc( it->second.PluginMsgHWnd );
	}

	m_mapAppID2Plusin.clear();
}

DWORD CPlusin::SendThreadFunc( void* pData )
{
	CPlusin* pPlusin = (CPlusin*)pData;
	if ( NULL == pPlusin )
	{
		return 0;
	}

	while( !pPlusin->m_bSendThreadExit )
	{
		EnterCriticalSection(&pPlusin->m_cs);
		if ( pPlusin->m_msgQueue.empty())
		{
			LeaveCriticalSection(&pPlusin->m_cs);
			Sleep(1);
			continue;
		}

		TargetMsg targetMsg = pPlusin->m_msgQueue.front();
		pPlusin->m_msgQueue.pop();
		LeaveCriticalSection(&pPlusin->m_cs);

		//发送消息
		if ( DEBUG_ON == 1 )
		{
			::SendMessage(targetMsg.hWnd, WM_COPYDATA, 
				(WPARAM)(pPlusin->m_pParentHWnd), (LPARAM)(&targetMsg.msg));
		}
		else
		{
			//发送超时时间为10秒
			DWORD dwResult = 0;
			::SendMessageTimeout(targetMsg.hWnd, WM_COPYDATA, 
				(WPARAM)(pPlusin->m_pParentHWnd), (LPARAM)(&targetMsg.msg),
				SMTO_NORMAL, 10000, &dwResult);
		}

		if ( NULL != targetMsg.msg.lpData )
		{
			delete[] targetMsg.msg.lpData;
			targetMsg.msg.lpData = NULL;
		}
	}

	return 0;
}

bool CPlusin::PushIntoMsgQueue( const TargetMsg& targetMsg, int nRequestId )
{
	if ( !m_bInit )
	{
		return false;
	}

	TargetMsg tempMsg = targetMsg;
	char* pBuffer = new char[targetMsg.msg.cbData+sizeof(int)];
	memset(pBuffer, 0, targetMsg.msg.cbData+sizeof(int));
	memcpy(pBuffer, &nRequestId, sizeof(int));
	if ( NULL != targetMsg.msg.lpData && targetMsg.msg.cbData > 0)
	{
		memcpy(pBuffer+sizeof(int), targetMsg.msg.lpData, targetMsg.msg.cbData);
	}

	tempMsg.msg.lpData = pBuffer;
	tempMsg.msg.cbData = targetMsg.msg.cbData+sizeof(int);

	EnterCriticalSection(&m_cs);
	m_msgQueue.push(tempMsg);
	LeaveCriticalSection(&m_cs);
	
	return true;
}

bool CPlusin::Send( COPYDATASTRUCT msg, HWND hPlusinTopWnd )
{
	//hPlusinTopWnd为NULL时，群发
	if ( NULL == hPlusinTopWnd )
	{
		bool bRet = true;
		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); ++it)
		{
			if ( it->second.bStarted )
			{
				TargetMsg targetMsg;
				targetMsg.hWnd = it->second.PluginMsgHWnd;
				targetMsg.msg = msg;
				if( !PushIntoMsgQueue(targetMsg, 0))
				{
					bRet = false;
				}
			}
		}

		return bRet;
	}
	else
	{
		HWND hPlusinMsgHWnd = NULL;
		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); ++it)
		{
			PLUGWIN2 plusinWin = it->second;
			if ( hPlusinTopWnd == plusinWin.PlusinTopHWnd && plusinWin.bStarted )
			{
				hPlusinMsgHWnd = plusinWin.PluginMsgHWnd;
				break;
			}
		}

		if ( NULL == hPlusinMsgHWnd || !IsWindow(hPlusinMsgHWnd))
		{
			return false;
		}

		TargetMsg targetMsg;
		targetMsg.hWnd = hPlusinMsgHWnd;
		targetMsg.msg = msg;
		return PushIntoMsgQueue(targetMsg, 0);
	}
}

BOOL CPlusin::ProcessConsoleLogon( HWND hWnd, void* pData, int nDataLen, int nRequestId )
{
	if ( NULL == pData )
	{
		return FALSE;
	}

	LogonStu* pLogonStru = (LogonStu*)pData;
	//验证插件API版本号
	if ( pLogonStru->dwVersion < 0x01000001 )
	{
		wxMessageBox( LOADSTRING(GUI_PLUSIN_API_VERSION_ERROR),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		return FALSE;
	}

	// 验证句柄是否有效
	HWND hPlusinTopWnd = (HWND)pLogonStru->hPlusinTopWnd;
	if ( !IsWindow( hWnd ) || !IsWindow(hPlusinTopWnd))
	{
		wxMessageBox( LOADSTRING(GUI_INVALID_HANDLE),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		return FALSE;
	}

	// 验证权限
	//if ( CTCPDataService::GetInstance()->LicenceRecved())
	//{
	//	std::string strLicence;
	//	if ( !CTCPDataService::GetInstance()->GetLicence(pLogonStru->szAppID, strLicence))
	//	{
	//		wxMessageBox( LOADSTRING(GUI_PERMISSION_DENIED),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
	//		return FALSE;
	//	}

	//	MD5 md5(pLogonStru->szLicense);
	//	std::string strMD5 = md5.toString();
	//	if ( strLicence != strMD5 )
	//	{
	//		wxMessageBox( LOADSTRING(GUI_PERMISSION_DENIED),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
	//		return FALSE;
	//	}
	//}
	//else
	//{
	//	return FALSE;
	//}

	// 判断插件是否已经启动
	map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.find( pLogonStru->szAppID );
	if ( it == m_mapAppID2Plusin.end() )
	{
		//插件未注册
		return FALSE;
	}

	if ( it->second.bStarted )
	{
		wxMessageBox( LOADSTRING(GUI_PLUSIN_ALREADY_START),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		return FALSE;
	}

	HWND ContainerHWnd = NULL;
	if ( pLogonStru->nPos == NEED_NO_POS_NO_BIND)
	{
		//不需要停靠
	}
	else if ( pLogonStru->nPos == NEED_NO_POS_BUT_BIND )
	{
		//将插件顶层窗体设为交易软件主窗体的子窗体
		::SetWindowLong(hPlusinTopWnd, GWL_HWNDPARENT, (long)GETTOPWINDOW()->GetHWND());
	}
	else
	{
		
		//位置在POS_QUOTE只能容许存在一个
		if(pLogonStru->nPos==POS_QUOTE)
		{
			map<wxString, PLUGWIN2>::iterator it_pos = m_mapAppID2Plusin.begin();
			for( ;it_pos != m_mapAppID2Plusin.end(); ++it_pos )
			{
				if( it_pos->second.ShowPos==POS_QUOTE && it_pos->second.bStarted )
				{
					wxMessageBox(LOADSTRING(PLUGIN_POS_QUOTE_NOT_ONE),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
					return FALSE;
				}
			}
		}

		// 创建容器窗口
		char szTitle[256];
		memset( szTitle, 0, sizeof(szTitle));
		::GetWindowText( hPlusinTopWnd, szTitle, sizeof(szTitle));
		if(m_pFnCreatePlusPanel==NULL)
			return FALSE;
		wxWindow* pWnd = m_pFnCreatePlusPanel( pLogonStru->nPos, szTitle,pLogonStru->szAppID);
		if ( NULL == pWnd )
		{
			return FALSE;
		}

		ContainerHWnd = (HWND)pWnd->GetHWND();
		long style = GetWindowLong(hPlusinTopWnd, GWL_STYLE);
		style &= ~WS_CAPTION;
		style |= WS_CHILDWINDOW;
		style &= ~WS_POPUP;
		::SetWindowLong(hPlusinTopWnd, GWL_STYLE, style);
		::SetParent( hPlusinTopWnd, (HWND)pWnd->GetHWND() );
		
		wxSize size = pWnd->GetClientSize();
		::MoveWindow( hPlusinTopWnd, 0, 0, size.GetWidth(), size.GetHeight(),TRUE);
	}

	it->second.PluginMsgHWnd = hWnd;
	it->second.PlusinTopHWnd = hPlusinTopWnd;
	it->second.ContainerHWnd = ContainerHWnd;
	it->second.ShowPos = pLogonStru->nPos;
	it->second.dwLastUpdateTime = GetTickCount();
	it->second.bStarted = true;

	if ( NULL != m_pPlusInDlg )
	{
		m_pPlusInDlg->UpdatePlusIn( pLogonStru->szAppID, it->second );
	}

	return TRUE;
}

BOOL CPlusin::ProcessConsoleMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	if ( nCMDID == CMDID_CONSOLE_HEARTBEAT )
	{
		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); it++)
		{
			if ( hWnd == it->second.PluginMsgHWnd )
			{
				it->second.dwLastUpdateTime = GetTickCount();
			}
		}
	}
	else if ( nCMDID == CMDID_CONSOLE_LOGON_REQUEST )
	{
		BOOL bRet = ProcessConsoleLogon(hWnd, pData, nDataLen, nRequestId);

		//发送登录响应
		TargetMsg targetMsg;
		memset(&targetMsg, 0, sizeof(targetMsg));
		targetMsg.hWnd = hWnd;
		if ( bRet )
		{
			targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_LOGON_RSP, SUCCESS);
		}
		else
		{
			targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_LOGON_RSP, UNKOWN_ERROR);
		}
		PushIntoMsgQueue(targetMsg, nRequestId);

		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); it++)
		{
			if ( hWnd == it->second.PluginMsgHWnd )
			{
				break;
			}
		}

		if ( it != m_mapAppID2Plusin.end() )
		{
			std::set<wxString>::iterator it_set = m_setPlusinWaitForShow.find(it->first);
			if ( it_set != m_setPlusinWaitForShow.end() )
			{
				if ( bRet )
				{
					memset(&targetMsg, 0, sizeof(targetMsg));
					targetMsg.hWnd = it->second.PluginMsgHWnd;
					targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW, SUCCESS);

					PushIntoMsgQueue(targetMsg, 0);
				}

				m_setPlusinWaitForShow.erase(it_set);
			}
		}
	}
	else if( nCMDID == CMDID_CONSOLE_HIDE )
	{
		map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); ++it )
		{
			if ( it->second.PluginMsgHWnd == hWnd )
			{
				wxWindow* pWin=wxGetWindowFromHWND(it->second.ContainerHWnd);
				if ( NULL != pWin )
				{
					wxCommandEvent evt(wxEVT_PLUGIN_HIDE);
					pWin->ProcessEvent(evt);
				}
			}
		}
	}
	else
	{

	}

	return TRUE;
}

BOOL CPlusin::ProcessAccountMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_ACCOUNT_SUB_ACCOUNT_LIST_QUERY )
	{
		std::vector<std::string> vAccount;
		if ( NULL != DEFAULT_SVR() &&
			DEFAULT_SVR()->GetAccountList(vAccount) &&
			vAccount.size() > 0)
		{
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_SUB_ACCOUNT_LIST_RSP, wErrorCode);
			targetMsg.msg.cbData = sizeof(CommonAccountIDType)*vAccount.size();

			char* pBuffer = new char[targetMsg.msg.cbData];
			memset(pBuffer, 0, targetMsg.msg.cbData);
			for ( size_t i = 0; i < vAccount.size(); i++ )
			{
				memcpy(pBuffer + sizeof(CommonAccountIDType)*i, 
					vAccount[i].c_str(), vAccount[i].length());
			}
			targetMsg.msg.lpData = pBuffer;
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			delete[] pBuffer;
			pBuffer = NULL;
		}
		else
		{
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_SUB_ACCOUNT_LIST_RSP, UNKOWN_ERROR);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_ACCOUNT_SUB_ACCOUNT_INFO_QUERY)
	{
		if ( pData != NULL && nDataLen > 0)
		{
			CommonAccountIDType szSubAccountID;
			memset(szSubAccountID, 0, sizeof(szSubAccountID));
			memcpy(szSubAccountID, pData, nDataLen);
			PlatformStru_TradingAccountInfo outData;
			if ( NULL != DEFAULT_SVR() && 
				DEFAULT_SVR()->GetTradingAccount(szSubAccountID, outData) == 0)
			{
				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP, wErrorCode);
				targetMsg.msg.cbData = sizeof(PlatformStru_TradingAccountInfo);
				targetMsg.msg.lpData = &outData;
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP, wErrorCode);
			}

			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY)
	{
		CommonAccountIDType szAccountID;
		memset(szAccountID, 0, sizeof(szAccountID));
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetTradingAccountID(szAccountID)==0)
		{
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP, wErrorCode);
			targetMsg.msg.cbData = sizeof(szAccountID);
			targetMsg.msg.lpData = szAccountID;
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP, wErrorCode);
		}

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else if ( nCMDID == CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_QUERY)
	{
		//目前无当前使用的子账号信息，暂时使用账号ID
		CommonAccountIDType szAccountID;
		memset(szAccountID, 0, sizeof(szAccountID));
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetTradingAccountID(szAccountID)==0)
		{
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP, wErrorCode);
			targetMsg.msg.cbData = sizeof(szAccountID);
			targetMsg.msg.lpData = szAccountID;
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP, wErrorCode);
		}

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else
	{

	}

	return bRet;
}

BOOL CPlusin::ProcessInstrumentMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_INSTRUMENT_LIST_QUERY )
	{
		if ( NULL != pData && nDataLen > 0)
		{
			CommonExchangeIDType szExchangeID;
			memset(szExchangeID, 0, sizeof(szExchangeID));
			memcpy(szExchangeID, pData, nDataLen);

			std::set<std::string> outData;
			if ( NULL != DEFAULT_SVR() 
				&& DEFAULT_SVR()->GetInstrumentList(outData, szExchangeID) > 0
				&& outData.size() > 0)
			{
				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_LIST_RSP, wErrorCode);
				targetMsg.msg.cbData = sizeof(InstrumentIDType)*outData.size();

				char* pBuffer = new char[targetMsg.msg.cbData];
				memset(pBuffer, 0, targetMsg.msg.cbData);
				std::set<std::string>::iterator it = outData.begin();
				int i = 0;
				for ( ; it != outData.end(); it++ )
				{
					memcpy(pBuffer + i*sizeof(InstrumentIDType), 
						(*it).c_str(), (*it).length());
					i++;
				}
				targetMsg.msg.lpData = pBuffer;
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				delete[] pBuffer;
				pBuffer = NULL;
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_LIST_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_INSTRUMENT_INFO_QUERY )
	{
		if ( NULL != pData && nDataLen > 0)
		{
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(szInstrumentID));
			memcpy(szInstrumentID, pData, nDataLen);

			PlatformStru_InstrumentInfo outData;
			if ( NULL != DEFAULT_SVR() && 
				DEFAULT_SVR()->GetInstrumentInfo(szInstrumentID, outData) == 0)
			{
				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_INFO_RSP, wErrorCode);
				targetMsg.msg.cbData = sizeof(PlatformStru_InstrumentInfo);
				targetMsg.msg.lpData = &outData;
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_INFO_RSP, wErrorCode);
			}

			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_INSTRUMENT_SHORTNAME_QUERY)
	{
		if ( NULL != DEFAULT_SVR() )
		{
			std::string strContractShortName;
			DEFAULT_SVR()->GetContractShortNames(strContractShortName);
			ContractShortNamesType szShortNames;
			memset(szShortNames, 0, sizeof(szShortNames));
			strcpy_s(szShortNames, sizeof(szShortNames)-1, strContractShortName.c_str());
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_SHORTNAME_RSP, wErrorCode);
			targetMsg.msg.cbData = sizeof(ContractShortNamesType);
			targetMsg.msg.lpData = szShortNames;
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_SHORTNAME_RSP, wErrorCode);
		}

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else
	{
	}

	return bRet;
}

BOOL CPlusin::ProcessAllOrderMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_ALL_ORDER_LIST_QUERY)
	{
		std::vector<long> vec;
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetAllOrderFTIDList(vec) > 0)
		{
			targetMsg.msg.cbData = sizeof(long)*vec.size();
			targetMsg.msg.lpData = &vec[0];
			targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_LIST_RSP, SUCCESS);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_LIST_RSP, wErrorCode);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_ALL_ORDER_INFO_QUERY1)
	{
		if ( NULL != pData && nDataLen == sizeof(long))
		{
			long lFTID = *(long*)pData;
			PlatformStru_OrderInfo outData;
			if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetAllOrderInfo(lFTID, outData) == 0)
			{
				targetMsg.msg.cbData = sizeof(outData);
				targetMsg.msg.lpData = &outData;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_INFO_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_INFO_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_ALL_ORDER_INFO_QUERY2)
	{
		if ( NULL != pData && nDataLen == sizeof(OrderKey))
		{
			OrderKey key = *(OrderKey*)pData;
			PlatformStru_OrderInfo outData;
			if ( DEFAULT_SVR() && DEFAULT_SVR()->GetAllOrderInfo(key, outData) == 0)
			{
				targetMsg.msg.cbData = sizeof(outData);
				targetMsg.msg.lpData = &outData;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_INFO_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_INFO_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_ALL_ORDER_WAIT_ORDER_NUM_QUERY )
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinOpenOrderKey))
		{
			PlusinOpenOrderKey key = *(PlusinOpenOrderKey*)pData;
			std::vector<PlatformStru_OrderInfo> vec;
			if ( NULL != DEFAULT_SVR() )
			{
				DEFAULT_SVR()->GetWaitOrders2(key.szInstrumentID, vec);
				int nRecordNum = 0;
				for ( UINT i = 0; i < vec.size(); i++)
				{
					if ( key.cDirection == vec[i].Direction )
					{
						if ( util::isInvalidValue(key.dLimitPrice))
						{
							nRecordNum += vec[i].VolumeTotal;
						}
						else
						{
							if ( util::equals(key.dLimitPrice, vec[i].LimitPrice))
							{
								nRecordNum += vec[i].VolumeTotal;
							}
						}
					}
				}

				PlusinOpenOrderNumRsp rsp;
				rsp.queryCondition = key;
				rsp.nNum = nRecordNum;
				targetMsg.msg.cbData = sizeof(PlusinOpenOrderNumRsp);
				targetMsg.msg.lpData = &rsp;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_ALL_ORDER_WAIT_ORDER_PRICE_QUERY )
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinAcctInstrumentKey))
		{
			PlusinAcctInstrumentKey key = *(PlusinAcctInstrumentKey*)pData;
			std::vector<PlatformStru_OrderInfo> vec;
			if ( NULL != DEFAULT_SVR() )
			{
				DEFAULT_SVR()->GetWaitOrders2(key.szInstrumentID, vec);
				WaitOrderPrice waitOrderPrice;
				for ( UINT i = 0; i < vec.size(); i++)
				{
					if ( i == 0 )
					{
						waitOrderPrice.dHighPrice = vec[i].LimitPrice;
						waitOrderPrice.dLowPrice = vec[i].LimitPrice;
					}
					else
					{
						if ( vec[i].LimitPrice > waitOrderPrice.dHighPrice )
						{
							waitOrderPrice.dHighPrice = vec[i].LimitPrice;
						}

						if ( vec[i].LimitPrice < waitOrderPrice.dLowPrice )
						{
							waitOrderPrice.dLowPrice = vec[i].LimitPrice;
						}
					}
				}

				targetMsg.msg.cbData = sizeof(WaitOrderPrice);
				targetMsg.msg.lpData = &waitOrderPrice;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else
	{

	}
	return bRet;
}

BOOL CPlusin::ProcessPositionMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_POSITION_LIST_QUERY)
	{
		std::vector<long> vec;
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetPositionFTIDList(vec) > 0)
		{
			targetMsg.msg.cbData = sizeof(long)*vec.size();
			targetMsg.msg.lpData = &vec[0];
			targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_LIST_RSP, SUCCESS);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_LIST_RSP, wErrorCode);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_POSITION_INFO_QUERY1)
	{
		if ( NULL != pData && nDataLen == sizeof(long))
		{
			long lFTID = *(long*)pData;
			PlatformStru_Position outData;
			if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetPositionInfo(lFTID, outData) == 0)
			{
				targetMsg.msg.cbData = sizeof(outData);
				targetMsg.msg.lpData = &outData;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_INFO_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_INFO_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_POSITION_INFO_QUERY2)
	{
		if ( NULL != pData && nDataLen == sizeof(long))
		{
			PositionKey key = *(PositionKey*)pData;
			PlatformStru_Position outData;
			if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetPositionInfo(key, outData) == 0)
			{
				targetMsg.msg.cbData = sizeof(outData);
				targetMsg.msg.lpData = &outData;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_INFO_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_INFO_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_POSITION_INFO_LIST_QUERY)
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinAcctInstrumentKey))
		{
			if ( NULL != DEFAULT_SVR() )
			{
				PlusinAcctInstrumentKey key = *(PlusinAcctInstrumentKey*)pData;
				std::vector<PlatformStru_Position> vec;
				long lLastSeq = 0;
				DEFAULT_SVR()->GetPositions3(key.szInstrumentID, vec, lLastSeq);
				if ( !vec.empty())
				{
					targetMsg.msg.cbData = sizeof(PlatformStru_Position)*vec.size();
					targetMsg.msg.lpData = &vec[0];
				}

				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_INFO_LIST_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_INFO_LIST_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_POSITION_COMBO_LIST_QUERY)
	{
		std::vector<long> vec;
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetPositionCombFTIDList(vec) > 0)
		{
			targetMsg.msg.cbData = sizeof(long)*vec.size();
			targetMsg.msg.lpData = &vec[0];
			targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_COMBO_LIST_RSP, SUCCESS);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_COMBO_LIST_RSP, wErrorCode);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_POSITION_COMBO_INFO_QUERY)
	{
		if ( NULL != pData && nDataLen == sizeof(long))
		{
			long lFTID = *(long*)pData;
			PlatformStru_PositionComb outData;
			if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetPositionCombInfo(lFTID, outData) == 0)
			{
				targetMsg.msg.cbData = sizeof(outData);
				targetMsg.msg.lpData = &outData;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_COMBO_INFO_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_COMBO_INFO_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else
	{

	}
	return bRet;
}

BOOL CPlusin::ProcessExchangeMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_EXCHANGE_LIST_QUERY )
	{
		std::vector<std::string> outData;
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetExchangeIDs(outData) > 0
			&& outData.size() > 0)
		{
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_EXCHANGE_LIST_RSP, wErrorCode);
			targetMsg.msg.cbData = sizeof(CommonExchangeIDType)*outData.size();

			char* pBuffer = new char[targetMsg.msg.cbData];
			memset(pBuffer, 0, targetMsg.msg.cbData);
			for ( size_t i = 0; i < outData.size(); i++ )
			{
				memcpy(pBuffer + i*sizeof(CommonExchangeIDType), 
					outData[i].c_str(), outData[i].length());
			}
			targetMsg.msg.lpData = pBuffer;
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			delete[] pBuffer;
			pBuffer = NULL;
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_EXCHANGE_LIST_RSP, wErrorCode);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_EXCHANGE_CURRENT_TIME_QUERY )
	{
		//日期时间格式如下:20120512 17:05:08
		if ( NULL != pData && nDataLen > 0&&DEFAULT_SVR())
		{
			CommonExchangeIDType szExchangeID;
			memset(szExchangeID, 0, sizeof(szExchangeID));
			memcpy(szExchangeID, pData, nDataLen);
			
			std::string strCurrentDay,strCurrentTime;
			strCurrentDay=DEFAULT_SVR()->GetTradingDay(szExchangeID);
			DWORD dwTickcount = DEFAULT_SVR()->GetExchangeTime(szExchangeID, strCurrentTime);
			if ((!strCurrentDay.empty())&& (!strCurrentTime.empty()))
			{
				strCurrentDay+=" ";
				strCurrentDay+=strCurrentTime;
				ExchangeCurTime exchCurTime;
				memset(&exchCurTime, 0, sizeof(exchCurTime));
				exchCurTime.dwTickCount = dwTickcount;
				strcpy(exchCurTime.szExchangeTime, strCurrentDay.c_str());

				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_EXCHANGE_CURRENT_TIME_RSP, wErrorCode);
				targetMsg.msg.cbData = sizeof(exchCurTime);
				targetMsg.msg.lpData = &exchCurTime;
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_EXCHANGE_CURRENT_TIME_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_EXCHANGE_TRADE_TIME_QUERY )
	{
		//交易时段格式如下:201205120915201205121130
		if ( NULL != pData && nDataLen > 0)
		{
			CommonExchangeIDType szExchangeID;
			memset(szExchangeID, 0, sizeof(szExchangeID));
			memcpy(szExchangeID, pData, nDataLen);

			std::string strTradeTime;
			if ( DEFAULT_SVR() )
			{
				std::string strCurrentDay;
				strCurrentDay=DEFAULT_SVR()->GetTradingDay(szExchangeID);
				SERVER_PARAM_CFG paramcfg=DEFAULT_SVR()->GetServerParamCfg();
				if( paramcfg.vec[paramcfg.current].CTPSimulate == 1 )
				{
					strTradeTime =strCurrentDay+"0000";
					strTradeTime+=strCurrentDay+"2400";
				}
				else
				{
					if( strcmp(szExchangeID, "CFFEX") == 0)
					{
						strTradeTime =strCurrentDay+"0915";
						strTradeTime+=strCurrentDay+"1130";
                        strTradeTime+=" ";
						strTradeTime+=strCurrentDay+"1300";
						strTradeTime+=strCurrentDay+"1515";

					}
					else
					{
						strTradeTime =strCurrentDay+"0900";
						strTradeTime+=strCurrentDay+"1015";
						strTradeTime+=" ";
						strTradeTime+=strCurrentDay+"1030";
						strTradeTime+=strCurrentDay+"1130";
						strTradeTime+=" ";
						strTradeTime+=strCurrentDay+"1330";
						strTradeTime+=strCurrentDay+"1500";
					}
				}
			}

			if ( !strTradeTime.empty())
			{
				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_EXCHANGE_TRADE_TIME_RSP, wErrorCode);
				targetMsg.msg.cbData = strTradeTime.length();
				targetMsg.msg.lpData = (void*)strTradeTime.c_str();
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_EXCHANGE_TRADE_TIME_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else
	{

	}

	return bRet;
}

BOOL CPlusin::ProcessQuoteMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_QUOTE_SUBSCRIBE )
	{
		if ( NULL != pData && nDataLen > 0 )
		{
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(InstrumentIDType));
			memcpy(szInstrumentID, pData, nDataLen);

			QUOTE_SUBSCRIBE_MAP::iterator it = m_quoteSubscribe.find(szInstrumentID);
			if ( it != m_quoteSubscribe.end() )
			{
				it->second.insert(hWnd);
			}
			else
			{
				if ( m_quoteSubscribe.size() > PLUSIN_SUBSCRIBE_INSTRUMENT_MAX_NUM)
				{
					wErrorCode = ERROR_ONE;
					targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_SUBSCRIBE_RSP, wErrorCode);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				}
				else
				{
					std::set<HWND> hWndSet;
					hWndSet.clear();
					hWndSet.insert(hWnd);
					m_quoteSubscribe[szInstrumentID] = hWndSet;
					wErrorCode = SUCCESS;
					targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_SUBSCRIBE_RSP, wErrorCode);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);

					if(DEFAULT_SVR()) 
						DEFAULT_SVR()->SetSubscribeStatus(szInstrumentID,S_STATUS_PLUS);
				}
			}
		}
	}
	else if ( nCMDID == CMDID_QUOTE_UNSUBSCRIBE )
	{
		if ( NULL != pData && nDataLen > 0 )
		{
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(szInstrumentID));
			memcpy(szInstrumentID, pData, nDataLen);

			QUOTE_SUBSCRIBE_MAP::iterator it = m_quoteSubscribe.find(szInstrumentID);
			if ( it != m_quoteSubscribe.end() )
			{
				it->second.erase(hWnd);
				if( it->second.empty() && DEFAULT_SVR())
				{
					m_quoteSubscribe.erase(it);
					DEFAULT_SVR()->SetSubscribeStatus(szInstrumentID,S_STATUS_PLUS, S_STATUS_DEL);
				}
			}
		}
	}
	else if ( nCMDID == CMDID_QUOTE_SNAPSHOT_QUERY)
	{
		if ( NULL != pData && nDataLen > 0 )
		{
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(szInstrumentID));
			memcpy(szInstrumentID, pData, nDataLen);

			PlatformStru_DepthMarketData outData;
			if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetQuotInfo(szInstrumentID, outData) == 0)
			{
				targetMsg.msg.cbData = sizeof(outData);
				targetMsg.msg.lpData = &outData;
				targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_SNAPSHOT_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = ERROR_ONE;
				targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_SNAPSHOT_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_QUOTE_PLUSIN_CURRENT_SEL)
	{
		if ( NULL != pData && nDataLen > 0 )
		{
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(szInstrumentID));
			memcpy(szInstrumentID, pData, nDataLen);

			//向报价表发送消息，要求报价表选中该合约

		}
	}
	else if ( nCMDID == CMDID_QUOTE_IS_SIMULATE_QUERY )
	{
		if ( NULL != DEFAULT_SVR() )
		{
			SERVER_PARAM_CFG paramcfg=DEFAULT_SVR()->GetServerParamCfg();
			targetMsg.msg.cbData = sizeof(int);
			targetMsg.msg.lpData = &paramcfg.vec[paramcfg.current].CTPSimulate;
			targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_IS_SIMULATE_RSP, SUCCESS);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_IS_SIMULATE_RSP, wErrorCode);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else
	{

	}

	return bRet;
}

BOOL CPlusin::ProcessOrderMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_ORDER_INSERT_ORDER )
	{
		if ( NULL != pData && nDataLen == sizeof(PlatformStru_InputOrder))
		{
			PlatformStru_InputOrder inputOrder = *(PlatformStru_InputOrder*)pData;
			if ( NULL != DEFAULT_SVR() )
			{
				int nRet = DEFAULT_SVR()->ReqOrderInsert(inputOrder, DEFAULT_SVR()->GetNextReqID());
				if ( nRet == 0 )
				{
					targetMsg.msg.cbData = sizeof(inputOrder);
					targetMsg.msg.lpData = &inputOrder;
					targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_INSERT_ORDER_RSP, SUCCESS);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				}
				else
				{
					wErrorCode = nRet;
					targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_INSERT_ORDER_RSP, wErrorCode);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				}
			}
		}
	}
	else if ( nCMDID == CMDID_ORDER_CANCEL_ORDER )
	{
		if ( NULL != pData && nDataLen == sizeof(PlatformStru_InputOrderAction))
		{
			PlatformStru_InputOrderAction inputOrderAction = *(PlatformStru_InputOrderAction*)pData;
			if ( NULL != DEFAULT_SVR() )
			{
				int nRet = DEFAULT_SVR()->ReqOrderAction(inputOrderAction, DEFAULT_SVR()->GetNextReqID());
				if ( nRet == 0 )
				{
					targetMsg.msg.cbData = sizeof(inputOrderAction);
					targetMsg.msg.lpData = &inputOrderAction;
					targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, SUCCESS);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				}
				else
				{
					wErrorCode = nRet;
					targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, wErrorCode);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				}
			}
		}
	}
	else if ( nCMDID == CMDID_ORDER_CANCEL_ORDER_BY_INST )
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinAcctInstrumentKey))
		{
			if ( NULL != DEFAULT_SVR() )
			{
				PlusinAcctInstrumentKey key = *(PlusinAcctInstrumentKey*)pData;
				std::vector<PlatformStru_OrderInfo> vec;
				DEFAULT_SVR()->GetWaitOrders2(key.szInstrumentID, vec);
				for ( size_t i = 0; i < vec.size(); i++ )
				{
					PlatformStru_InputOrderAction ReqData;
					memset(&ReqData, 0, sizeof(ReqData));

					ReqData.Thost.FrontID = vec[i].FrontID;
					ReqData.Thost.SessionID = vec[i].SessionID;
					memcpy(ReqData.Thost.OrderRef, vec[i].OrderRef, sizeof(vec[i].OrderRef));
					memcpy(ReqData.Thost.OrderSysID, vec[i].OrderSysID, sizeof(vec[i].OrderSysID));
					memcpy(ReqData.Thost.ExchangeID, vec[i].ExchangeID, sizeof(vec[i].ExchangeID));
					memcpy(ReqData.Thost.InstrumentID, vec[i].InstrumentID, sizeof(vec[i].InstrumentID));

					int nRet = DEFAULT_SVR()->ReqOrderAction(ReqData);
					if ( nRet == 0 )
					{
						targetMsg.msg.cbData = sizeof(ReqData);
						targetMsg.msg.lpData = &ReqData;
						targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, SUCCESS);
						bRet = PushIntoMsgQueue(targetMsg, nRequestId);
					}
					else
					{
						wErrorCode = nRet;
						targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, wErrorCode);
						bRet = PushIntoMsgQueue(targetMsg, nRequestId);
					}
				}
			}
		}
	}
	else if ( nCMDID == CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTPRICE )
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinAcctInstrumentLmtPriceKey))
		{
			if ( NULL != DEFAULT_SVR() )
			{
				PlusinAcctInstrumentLmtPriceKey key = *(PlusinAcctInstrumentLmtPriceKey*)pData;
				std::vector<PlatformStru_OrderInfo> vec;
				DEFAULT_SVR()->GetWaitOrders2(key.szInstrumentID, vec);
				for ( size_t i = 0; i < vec.size(); i++ )
				{
					if ( !util::equals( key.dLimitPrice, vec[i].LimitPrice))
					{
						continue;
					}

					PlatformStru_InputOrderAction ReqData;
					memset(&ReqData, 0, sizeof(ReqData));

					ReqData.Thost.FrontID = vec[i].FrontID;
					ReqData.Thost.SessionID = vec[i].SessionID;
					memcpy(ReqData.Thost.OrderRef, vec[i].OrderRef, sizeof(vec[i].OrderRef));
					memcpy(ReqData.Thost.OrderSysID, vec[i].OrderSysID, sizeof(vec[i].OrderSysID));
					memcpy(ReqData.Thost.ExchangeID, vec[i].ExchangeID, sizeof(vec[i].ExchangeID));
					memcpy(ReqData.Thost.InstrumentID, vec[i].InstrumentID, sizeof(vec[i].InstrumentID));

					int nRet = DEFAULT_SVR()->ReqOrderAction(ReqData);
					if ( nRet == 0 )
					{
						targetMsg.msg.cbData = sizeof(ReqData);
						targetMsg.msg.lpData = &ReqData;
						targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, SUCCESS);
						bRet = PushIntoMsgQueue(targetMsg, nRequestId);
					}
					else
					{
						wErrorCode = nRet;
						targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, wErrorCode);
						bRet = PushIntoMsgQueue(targetMsg, nRequestId);
					}
				}
			}
		}
	}
	else if ( nCMDID == CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTDIRPRICE )
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinOpenOrderKey))
		{
			if ( NULL != DEFAULT_SVR() )
			{
				PlusinOpenOrderKey key = *(PlusinOpenOrderKey*)pData;
				std::vector<PlatformStru_OrderInfo> vec;
				DEFAULT_SVR()->GetWaitOrders2(key.szInstrumentID, vec);
				for ( size_t i = 0; i < vec.size(); i++ )
				{
					if ( !util::equals( key.dLimitPrice, vec[i].LimitPrice) ||
						key.cDirection != vec[i].Direction )
					{
						continue;
					}

					PlatformStru_InputOrderAction ReqData;
					memset(&ReqData, 0, sizeof(ReqData));

					ReqData.Thost.FrontID = vec[i].FrontID;
					ReqData.Thost.SessionID = vec[i].SessionID;
					memcpy(ReqData.Thost.OrderRef, vec[i].OrderRef, sizeof(vec[i].OrderRef));
					memcpy(ReqData.Thost.OrderSysID, vec[i].OrderSysID, sizeof(vec[i].OrderSysID));
					memcpy(ReqData.Thost.ExchangeID, vec[i].ExchangeID, sizeof(vec[i].ExchangeID));
					memcpy(ReqData.Thost.InstrumentID, vec[i].InstrumentID, sizeof(vec[i].InstrumentID));

					int nRet = DEFAULT_SVR()->ReqOrderAction(ReqData);
					if ( nRet == 0 )
					{
						targetMsg.msg.cbData = sizeof(ReqData);
						targetMsg.msg.lpData = &ReqData;
						targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, SUCCESS);
						bRet = PushIntoMsgQueue(targetMsg, nRequestId);
					}
					else
					{
						wErrorCode = nRet;
						targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CANCEL_ORDER_RSP, wErrorCode);
						bRet = PushIntoMsgQueue(targetMsg, nRequestId);
					}
				}
			}
		}
	}
	else if ( nCMDID == CMDID_ORDER_CURRENT_INSTRUENT_QUERY )
	{
		if ( !m_strOrderModuleCurInstrumentID.IsEmpty())
		{
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(szInstrumentID));
			strcpy(szInstrumentID, m_strOrderModuleCurInstrumentID.c_str());
			targetMsg.msg.cbData = sizeof(szInstrumentID);
			targetMsg.msg.lpData = &szInstrumentID;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CURRENT_INSTRUENT_RSP, SUCCESS);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
		else
		{
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_CURRENT_INSTRUENT_RSP, UNKOWN_ERROR);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else
	{

	}

	return bRet;
}

BOOL CPlusin::ProcessConfigMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_QUERY)
	{
		ORDER_CFG* pOrderCfg = pCfgMgr->GetOrderCfg();
		if ( NULL != pOrderCfg )
		{
			bool bFlag = pOrderCfg->bMouseInputInstrumentId;
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP, wErrorCode);
			targetMsg.msg.lpData = &bFlag;
			targetMsg.msg.cbData = sizeof(bool);
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP, wErrorCode);
		}

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else if ( nCMDID == CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_QUERY )
	{
		ORDER_CFG* pOrderCfg = pCfgMgr->GetOrderCfg();
		if ( NULL != pOrderCfg )
		{
			bool bFlag = pOrderCfg->bMouseInputVolume;
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP, wErrorCode);
			targetMsg.msg.lpData = &bFlag;
			targetMsg.msg.cbData = sizeof(bool);
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP, wErrorCode);
		}

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else if ( nCMDID == CMDID_CONFIG_INPUT_VOLUMES_QUERY)
	{
		ORDER_CFG* pOrderCfg = pCfgMgr->GetOrderCfg();
		if ( NULL != pOrderCfg )
		{
			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_CONFIG_INPUT_VOLUMES_RSP, wErrorCode);
			targetMsg.msg.lpData = (int*)&pOrderCfg->Volume[0];
			targetMsg.msg.cbData = sizeof(int)*9;
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_CONFIG_INPUT_VOLUMES_RSP, wErrorCode);
		}

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else
	{

	}

	return bRet;
}

int CPlusin::CallBackFunc_BID_RtnDepthMarketData( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if( data.BID != BID_RtnDepthMarketData || data.Size != sizeof(DataRtnDepthMarketData))
	{
		return 0;
	}

	if ( s_pPlusin->m_quoteSubscribe.empty() )
	{
		return 0;
	}

	DataRtnDepthMarketData* pMarketData = (DataRtnDepthMarketData*)&data;
	QUOTE_SUBSCRIBE_MAP::iterator it = s_pPlusin->m_quoteSubscribe.find(pMarketData->DepthMarketDataField.InstrumentID);
	if ( it != s_pPlusin->m_quoteSubscribe.end() )
	{
		std::set<HWND> hWndSet = it->second;
		std::set<HWND>::iterator it_hWnd = hWndSet.begin();
		for ( ; it_hWnd != hWndSet.end(); ++it_hWnd )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = *it_hWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_SEND_QUOTE, SUCCESS);
			targetMsg.msg.cbData = sizeof(PlatformStru_DepthMarketData);
			targetMsg.msg.lpData = &(pMarketData->DepthMarketDataField);
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

void CPlusin::SubscribeBusinessData()
{
	if(DEFAULT_SVR()) 
	{
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnDepthMarketData, GID_PLUSIN, CallBackFunc_BID_RtnDepthMarketData);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTrade, GID_PLUSIN, CallBackFunc_BID_RtnTrade);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_PLUSIN, CallBackFunc_BID_RtnOrder);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPosition, GID_PLUSIN, CallBackFunc_BID_RspQryInvestorPosition);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPositionComb, GID_PLUSIN, CallBackFunc_BID_RspQryInvestorComboPosition);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryOrder, GID_PLUSIN, CallBackFunc_BID_RspQryOrder);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingAccount, GID_PLUSIN, CallBackFunc_BID_RspQryTradingAccount);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction1, GID_PLUSIN, RspOrderAction1CallBackFunc);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction2, GID_PLUSIN, RspOrderAction2CallBackFunc);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_PLUSIN, RspOrderInsertCallBackFunc);
	}
}

void CPlusin::UnSubscribeBusinessData()
{
	if(DEFAULT_SVR()) 
	{
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnTrade, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnOrder, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPosition, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPositionComb, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryOrder, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingAccount, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction1, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction2, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderInsert, GID_PLUSIN);
	}
}

int CPlusin::CallBackFunc_BID_RtnOrder( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RtnOrder || data.Size!=sizeof(DataRtnOrder))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty())
	{
		return 0;
	}

	DataRtnOrder* pOrderData = (DataRtnOrder*)&data;
	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_ORDER_STATUS_CHANGE, SUCCESS);
			targetMsg.msg.cbData = sizeof(PlatformStru_OrderInfo);
			targetMsg.msg.lpData = &(pOrderData->OrderField);
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::CallBackFunc_BID_RtnTrade( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty())
	{
		return 0;
	}

	DataRtnTrade* pTradeData = (DataRtnTrade*)&data;
	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_TRADE_RTN, SUCCESS);
			targetMsg.msg.cbData = sizeof(PlatformStru_TradeInfo);
			targetMsg.msg.lpData = &(pTradeData->TradeField);
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::CallBackFunc_BID_RspQryInvestorPosition( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RspQryInvestorPosition || data.Size!=sizeof(DataRspQryInvestorPosition))
		return 0;

	if ( s_pPlusin->m_mapAppID2Plusin.empty() || !((DataRspQryInvestorPosition*)&data)->bIsLast)
	{
		return 0;
	}

	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_REQUERY, SUCCESS);
			targetMsg.msg.cbData = 0;
			targetMsg.msg.lpData = NULL;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::CallBackFunc_BID_RspQryInvestorComboPosition( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RspQryInvestorPositionComb || data.Size!=sizeof(DataRspQryInvestorPositionComb))
		return 0;

	if ( s_pPlusin->m_mapAppID2Plusin.empty() || !((DataRspQryInvestorPositionComb*)&data)->bIsLast)
	{
		return 0;
	}

	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_COMBO_REQUERY, SUCCESS);
			targetMsg.msg.cbData = 0;
			targetMsg.msg.lpData = NULL;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::CallBackFunc_BID_RspQryOrder( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RspQryOrder || data.Size!=sizeof(DataRspQryOrder))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty() || !((DataRspQryOrder*)&data)->bIsLast)
	{
		return 0;
	}

	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_REQUERY, SUCCESS);
			targetMsg.msg.cbData = 0;
			targetMsg.msg.lpData = NULL;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::CallBackFunc_BID_RspQryTradingAccount( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RspQryTradingAccount || data.Size!=sizeof(DataRspQryTradingAccount))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty() || !((DataRspQryTradingAccount*)&data)->bIsLast)
	{
		return 0;
	}

	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ACCOUNT_REQUERY, SUCCESS);
			targetMsg.msg.cbData = 0;
			targetMsg.msg.lpData = NULL;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

void CPlusin::SetOrderModuleCurInstrumentID( const wxString& strInstrumentID )
{
	m_strOrderModuleCurInstrumentID = strInstrumentID;
}

bool CPlusin::RegisterPlusin( const wxString& strPlusinPath, bool bSendMenuChangeMsg )
{
	wxString strConfigFile = strPlusinPath;
	strConfigFile.Replace(".exe", ".xml");
	PlusinConfigHelper helper;
	if ( helper.LoadConfig(strConfigFile))
	{
		PLUGWIN2 plusin;
		plusin.strPath = strPlusinPath.c_str();
		plusin.bStarted = false;
		plusin.unMenuPos = helper.GetMenuPos();
		plusin.strMenuName = helper.GetMenuName();
		plusin.strSummary = helper.GetSummary();
		plusin.strDesc = helper.GetDesc();

		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.find(helper.GetAppID());
		if ( it == m_mapAppID2Plusin.end())
		{
			m_mapAppID2Plusin[helper.GetAppID()] = plusin;
			if ( NULL != m_pPlusInDlg)
			{
				m_pPlusInDlg->UpdatePlusIn(helper.GetAppID(), plusin);
			}

			if ( bSendMenuChangeMsg && plusin.unMenuPos != MENU_POS_NONE)
			{
				wxCommandEvent myEvent(wxEVT_PLUSIN_MENU_CHANGE);
				myEvent.SetId( plusin.unMenuPos );
				GETTOPWINDOW()->AddPendingEvent(myEvent);
			}

			if ( helper.NeedStartWhenRegist() )
			{
				OpenPlusInProc( plusin.strPath );
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		string str = LOADSTRING(GUI_LOAD_FAILED);
		wxString strMsg = wxString::Format( str.c_str(), strConfigFile.c_str());
		wxMessageBox( strMsg,LOADSTRING(OIP_ERROR),wxOK|wxICON_WARNING);
		
		return false;
	}
}

bool CPlusin::GetPlusinByAppID( const wxString& strAppID, PLUGWIN2& plusin )
{
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.find(strAppID);
	if ( it != m_mapAppID2Plusin.end() )
	{
		plusin = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

void CPlusin::GetPlusinMenuInfo( UINT nMenuPos,vector<wxString>& AppIDs )
{
	AppIDs.clear();
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
	for ( ; it != m_mapAppID2Plusin.end(); ++it )
	{
		if ( nMenuPos & it->second.unMenuPos )
		{
			AppIDs.push_back(it->first);
		}
	}
}

void CPlusin::ShowPlusin( const wxString& strAppID )
{
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.find(strAppID);
	if ( it == m_mapAppID2Plusin.end())
	{
		return;
	}

	if ( !it->second.bStarted )
	{
		OpenPlusInProc( it->second.strPath );
		m_setPlusinWaitForShow.insert(strAppID);
	}
	else
	{
		TargetMsg targetMsg;
		memset(&targetMsg, 0, sizeof(targetMsg));
		targetMsg.hWnd = it->second.PluginMsgHWnd;
		targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW, SUCCESS);

		PushIntoMsgQueue(targetMsg, 0);
	}
}

int CPlusin::RspOrderAction1CallBackFunc( const GUIModuleID GID,const AbstractBusinessData &data )
{
	if(data.BID!=BID_RspOrderAction1 || data.Size!=sizeof(DataRspOrderAction1))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty())
	{
		return 0;
	}

	DataRspOrderAction1* pOrderAction1 = (DataRspOrderAction1*)&data;
	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_RSP_ORDER_ACTION1, SUCCESS);
			PlatformStru_RspOrderAction1 rspOrderAction1;
			memcpy(&rspOrderAction1.InputOrderActionField, &pOrderAction1->InputOrderActionField, sizeof(rspOrderAction1.InputOrderActionField));
			memcpy(&rspOrderAction1.RspInfoField, &pOrderAction1->RspInfoField, sizeof(rspOrderAction1.RspInfoField));
			targetMsg.msg.cbData = sizeof(PlatformStru_RspOrderAction1);
			targetMsg.msg.lpData = &rspOrderAction1;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::RspOrderAction2CallBackFunc( const GUIModuleID GID,const AbstractBusinessData &data )
{
	if(data.BID!=BID_RspOrderAction2 || data.Size!=sizeof(DataRspOrderAction2))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty())
	{
		return 0;
	}

	DataRspOrderAction2* pOrderAction2 = (DataRspOrderAction2*)&data;
	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_RSP_ORDER_ACTION2, SUCCESS);
			PlatformStru_RspOrderAction2 rspOrderAction2;
			memcpy(&rspOrderAction2.OrderActionField, &pOrderAction2->OrderActionField, sizeof(rspOrderAction2.OrderActionField));
			memcpy(&rspOrderAction2.RspInfoField, &pOrderAction2->RspInfoField, sizeof(rspOrderAction2.RspInfoField));
			targetMsg.msg.cbData = sizeof(PlatformStru_RspOrderAction2);
			targetMsg.msg.lpData = &rspOrderAction2;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::RspOrderInsertCallBackFunc( const GUIModuleID GID,const AbstractBusinessData &data )
{
	if(data.BID!=BID_RspOrderInsert || data.Size!=sizeof(DataRspOrderInsert))
		return 0;	

	if ( s_pPlusin->m_mapAppID2Plusin.empty())
	{
		return 0;
	}

	DataRspOrderInsert* pOrderInsert1 = (DataRspOrderInsert*)&data;
	std::map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin();
	for ( ; it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
		if ( it->second.bStarted )
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_RSP_ORDER_INSERT, SUCCESS);
			PlatformStru_RspOrderInsert rspOrderInsert;
			memcpy(&rspOrderInsert.InputOrderInsertField, &pOrderInsert1->InputOrderField, sizeof(rspOrderInsert.InputOrderInsertField));
			memcpy(&rspOrderInsert.RspInfoField, &pOrderInsert1->RspInfoField, sizeof(rspOrderInsert.RspInfoField));
			targetMsg.msg.cbData = sizeof(PlatformStru_RspOrderInsert);
			targetMsg.msg.lpData = &rspOrderInsert;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}
