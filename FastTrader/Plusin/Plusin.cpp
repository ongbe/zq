// Plusin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Psapi.h>
#include <Windows.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include "Plusin.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include "../inc/Module-Misc/constants.h"
#include "../ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc2/KernelStruct.h"
#include "PlusInMgrDlg.h"
#include "PlusinConfigHelper.h"
#include "MD5/MD5Checksum.h"
#include "TCPDataService.h"
#include "../../Miscellaneous/Tools/inc/FileOpr.h"
#include "../inc/Module-Misc2/RuntimeConfigMgr.h"

// This is the constructor of a class that has been exported.
// see Plusin.h for the class definition
extern PlatformSvrMgr* g_pPlatformMgr;
static CPlusin* s_pPlusin = NULL;


CPlusin::CPlusin()
: m_pFnCreatePlusPanel(NULL)
, m_pFnDeletePlusPanel(NULL)
, m_pPlusInDlg(NULL)
, m_bInit(false)
, m_hSendThreadHandle(NULL)
, m_bSendThreadExit(false)
, m_bOrderVerify(false)
, m_hMsgWnd_OrderVerify(NULL)
, m_pParentHWnd(NULL)
, m_strOrderModuleCurInstrumentID(wxEmptyString)
{
	InitializeCriticalSection(&m_cs);
	m_mapAppID2Plusin.clear();
	m_setSysPlusIn.clear();
	m_quoteSubscribe.clear();
	m_setProcessIDWaitForShow.clear();
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
    DWORD threadID;
	m_hSendThreadHandle = CreateThread(NULL, 65535, (LPTHREAD_START_ROUTINE)SendThreadFunc, this, 0,&threadID);
    if(GlobalConfigManager::m_Test_bLogThreadID)
        CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCPlusin::SendThreadFunc",threadID);
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
	else if ( nCMDID >= CMDID_INSERT_ORDER_VERIFY_MIN && nCMDID <= CMDID_INSERT_ORDER_VERIFY_MAX )
	{
		return ProcessInsertOrderVerifyMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
	}
    else
    {
		return ProcessOtherMsg(hSourceWnd, nCMDID, pData, nDataLen, nRequestId);
    }

	return FALSE;
}

void CPlusin::ReleasePlusInRC()
{
	//将句柄无效，或者没有收到心跳的插件清除
	std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
	for ( ; it != m_mapAppID2Plusin.end(); it++)
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();)
		{
            HWND hWnd = it2->first;

			if ( !IsWindow( hWnd ) || (GetTickCount() - it2->second.dwLastUpdateTime)/1000 > HEARTBEAT_RATE*2 && DEBUG_ON == 0)
			{
		        //关闭插件进程
                if(IsWindow(hWnd))
                {
				    DWORD dwProcID;
				    ::GetWindowThreadProcessId( hWnd, &dwProcID );
				    HANDLE handle = ::OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcID );
				    if ( NULL != handle )
					    ::TerminateProcess(handle, 0);
                }
			    
                //审核插件退出时，取消审核功能。
			    if ( hWnd != NULL && hWnd == m_hMsgWnd_OrderVerify )
			    {
				    m_bOrderVerify = false;
				    m_hMsgWnd_OrderVerify = NULL;
			    }

                it2=it->second.m_Instance.erase(it2);
			}
            else it2++;
		}

        if(it->second.m_Instance.empty())
        {
            ReleasePlusInRC(it->second.ShowPos, it->first );

            if ( m_pPlusInDlg != NULL )
	            m_pPlusInDlg->UpdatePlusIn( it->first, it->second );
        }
	}
}

void CPlusin::ClosePlusInProc( HWND hWnd )
{
	if ( !IsWindow( hWnd ))
		return;

	TargetMsg targetMsg;
    int param=0;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_CLOSE, SUCCESS);
	targetMsg.msg.lpData = &param;
	targetMsg.msg.cbData = sizeof(int);

	::SendMessage(targetMsg.hWnd, WM_COPYDATA, (WPARAM)(m_pParentHWnd), (LPARAM)(&targetMsg.msg));
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

        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
			ClosePlusInProc(it2->first);
		
        it->second.m_Instance.clear();

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
					//防止重复启动
					//wxString strConfigFile = strPlusInPath;
					//strConfigFile.Replace(".exe", ".xml");
					//PlusinConfigHelper helper;
					//if ( helper.LoadConfig(strConfigFile))
					//{
					//	if ( !helper.NeedStartWhenRegist() )
					//	{
					//		OpenPlusInProc( strPlusInPath.c_str() );
					//	}
					//}
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
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		    ClosePlusInProc( it2->first );
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
		bool bRet = false;
		std::map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); ++it)
		{
            for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
			{
                if(IsWindow(it2->first))
                {
				    TargetMsg targetMsg;
				    targetMsg.hWnd = it2->first;
				    targetMsg.msg = msg;
				    if( PushIntoMsgQueue(targetMsg, 0))
				    {
                        if(!bRet) bRet=true;
				    }
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
            for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
            {
                if(it2->second.PlusinTopHWnd==hPlusinTopWnd)
                {
                    hPlusinMsgHWnd=it2->first;
                    break;
                }
            }
            if(hPlusinMsgHWnd) break;
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

	//	std::string strMD5 = CMD5Checksum::GetMD5((BYTE*)(pLogonStru->szLicense), strlen(pLogonStru->szLicense));
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

	// 判断插件是否已经注册
	map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.find( pLogonStru->szAppID );
	if ( it == m_mapAppID2Plusin.end() )
	{
		//插件未注册
		return FALSE;
	}

    if(!it->second.bMultiInstance&&it->second.m_Instance.size()>0)
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
                if( it_pos->second.ShowPos==POS_QUOTE && it_pos->second.m_Instance.size()>0 )
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

    PLUGWIN2::Stru_Instance newInst;
	newInst.PluginMsgHWnd = hWnd;
	newInst.PlusinTopHWnd = hPlusinTopWnd;
	newInst.ContainerHWnd = ContainerHWnd;
	newInst.dwLastUpdateTime = GetTickCount();
    it->second.m_Instance[hWnd]=newInst;
    map<HWND,PLUGWIN2::Stru_Instance>::iterator itnew=it->second.m_Instance.find(hWnd);
	it->second.ShowPos = pLogonStru->nPos;

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
		map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin();
		for ( ; it != m_mapAppID2Plusin.end(); it++)
		{
            map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.find(hWnd);
            if(it2!=it->second.m_Instance.end())
		    {
			    it2->second.dwLastUpdateTime = GetTickCount();
                break;
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

        if(bRet)
        {
            DWORD ProcessID;
            GetWindowThreadProcessId( hWnd, &ProcessID );
            set<DWORD>::iterator it_set = m_setProcessIDWaitForShow.find(ProcessID);
            if(it_set!=m_setProcessIDWaitForShow.end())
            {
                m_setProcessIDWaitForShow.erase(it_set);

	            memset(&targetMsg, 0, sizeof(targetMsg));
	            targetMsg.hWnd = hWnd;
	            targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW, SUCCESS);

	            PushIntoMsgQueue(targetMsg, 0);
            }
        }
	}
	else if( nCMDID == CMDID_CONSOLE_HIDE )
	{
		for ( map<wxString, PLUGWIN2>::iterator it = m_mapAppID2Plusin.begin(); it != m_mapAppID2Plusin.end(); ++it )
		{
            map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.find(hWnd);
            if(it2!=it->second.m_Instance.end())
			{
				wxWindow* pWin=wxGetWindowFromHWND(it2->second.ContainerHWnd);
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
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetTradingAccountID(szAccountID,sizeof(szAccountID))==0)
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
		if ( NULL != DEFAULT_SVR() && DEFAULT_SVR()->GetTradingAccountID(szAccountID,sizeof(szAccountID))==0)
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

			if ( NULL != DEFAULT_SVR())
			{
                std::set<std::string> outData;
                DEFAULT_SVR()->GetInstrumentList(outData, szExchangeID);
				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_INSTRUMENT_LIST_RSP, wErrorCode);
				targetMsg.msg.cbData = sizeof(InstrumentIDType)*outData.size();
                if ( outData.empty())
                {
                    targetMsg.msg.lpData = NULL;
                    bRet = PushIntoMsgQueue(targetMsg, nRequestId);
                }
                else
                {
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
			strncpy(szShortNames, strContractShortName.c_str(), sizeof(szShortNames)-1);
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
	else if (   nCMDID == CMDID_PRODUCT_LIST_QUERY || 
                nCMDID == CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY || 
                nCMDID == CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY ||
                nCMDID == CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY)
	{
        //CMDID_PRODUCT_LIST_QUERY                      : 查询指定交易所的品种列表。参数为ExchangeID；成功时返回n*32字符串数组。第一个字符串是ExchangeID，后面是结果(n-1)个ProductID
        //CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY      : 查询指定交易所的期权标的物品种列表。参数为ExchangeID；成功时返回n*32字符串数组。第一个字符串是ExchangeID，后面是结果(n-1)个ProductID
        //CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY: 查询指定标的物品合约对应的期权合约。参数为UnderlyingInstrumentID；成功时返回n*32字符串数组。第一个字符串是UnderlyingInstrumentID，后面是结果(n-1)个期权InstrumentID
        //CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY       : 查询指定品种对应的合约。参数为ProductID；成功时返回n*32字符串数组。第一个字符串是ProductID，后面是结果(n-1)个期权InstrumentID


        int RspCMDID;
	    if(nCMDID == CMDID_PRODUCT_LIST_QUERY) RspCMDID = CMDID_PRODUCT_LIST_RSP;
	    else if(nCMDID == CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY) RspCMDID = CMDID_OPTIONUNDERLYINGPRODUCT_LIST_RSP;
	    else if(nCMDID == CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY) RspCMDID = CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_RSP;
	    else if(nCMDID == CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY) RspCMDID = CMDID_INSTRUMENT_LIST_BYPRODUCTID_RSP;

        if ( NULL != pData && nDataLen > 0)
		{
			if ( NULL != DEFAULT_SVR() && nDataLen<32 )
			{
                int len=strnlen((char*)pData,nDataLen);
                string inID((char*)pData,len);
                vector<string> rltIDs;

                if(nCMDID == CMDID_PRODUCT_LIST_QUERY) 
                    DEFAULT_SVR()->GetProductList(rltIDs, inID);
                else if(nCMDID == CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY) 
                    DEFAULT_SVR()->GetOptionProductIDsByExchangeID(inID,rltIDs);
                else if(nCMDID == CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY) 
                    DEFAULT_SVR()->GetOptionInstrumentIDsByUnderlyingInstrumentID(inID,rltIDs);
                else if(nCMDID == CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY)
                    DEFAULT_SVR()->GetInstrumentListByProductID(inID,rltIDs);

                targetMsg.msg.dwData = MAKELONG(RspCMDID, SUCCESS);
				targetMsg.msg.cbData = (rltIDs.size()+1)*32;

                char* pBuffer = new char[targetMsg.msg.cbData];
                memset(pBuffer, 0, targetMsg.msg.cbData);
                memcpy(pBuffer , inID.c_str(), inID.length()<31?inID.length():31);
                for(int i=0;i<(int)rltIDs.size();i++)
                    memcpy(pBuffer + (i+1)*32, rltIDs[i].c_str(), rltIDs[i].length()<31?rltIDs[i].length():31);
                targetMsg.msg.lpData = pBuffer;
                bRet = PushIntoMsgQueue(targetMsg, nRequestId);
                delete[] pBuffer;
			}
			else
			{
				targetMsg.msg.dwData = MAKELONG(RspCMDID, UNKOWN_ERROR);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_PRODUCT_INFO_QUERY )
	{
		if ( NULL != pData && nDataLen > 0)
		{
            InstrumentIDType szProductID={0};
			memcpy(szProductID, pData, nDataLen);

			PlatformStru_ProductInfo outData;
			if ( NULL != DEFAULT_SVR() && 
				DEFAULT_SVR()->GetProductInfo(szProductID, outData) == 0)
			{
				wErrorCode = SUCCESS;
				targetMsg.msg.dwData = MAKELONG(CMDID_PRODUCT_INFO_RSP, wErrorCode);
				targetMsg.msg.cbData = sizeof(PlatformStru_ProductInfo);
				targetMsg.msg.lpData = &outData;
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_PRODUCT_INFO_RSP, wErrorCode);
			}

			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if ( nCMDID == CMDID_PRODUCT_ALLINFO_QUERY )
	{
        InstrumentIDType szProductID={0};
		memcpy(szProductID, pData, nDataLen);

		vector<pair<string,vector<PlatformStru_ProductInfo>>> outData;
		if ( NULL != DEFAULT_SVR())
        {
			DEFAULT_SVR()->GetExchangeID_ProductInfos(outData);
            int productCount=0;
            for(int i=0;i<(int)outData.size();i++) productCount+=outData[i].second.size();

            PlatformStru_ProductInfo* pInfos=(PlatformStru_ProductInfo*)malloc(productCount*sizeof(PlatformStru_ProductInfo));
            for(int i=0,cnt=0;i<(int)outData.size();i++) 
            {
                for(int j=0;j<(int)outData[i].second.size();j++)
                    pInfos[cnt++]=outData[i].second[j];
            }

			wErrorCode = SUCCESS;
			targetMsg.msg.dwData = MAKELONG(CMDID_PRODUCT_ALLINFO_RSP, wErrorCode);
			targetMsg.msg.cbData = productCount*sizeof(PlatformStru_ProductInfo);
			targetMsg.msg.lpData = pInfos;

            bRet = PushIntoMsgQueue(targetMsg, nRequestId);

            free(pInfos);

		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_PRODUCT_ALLINFO_RSP, wErrorCode);
		    bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if (   nCMDID == CMDID_TRADINGTIMESPAN_QUERY )
	{
        //查询合约的日内交易时段，参数为InstrumentID，不能为空
        //返回合约的日内交易时段，成功时lpData为InstrumentID(32字节)+时间段数组，每个时间段为4+4字节，表示开始和结束时间，格式为hhmm，如0915113013001515表示两个时间段，9:15-11:30和13:00-15:15; cbData=32+时间段数量*8

        if ( NULL != pData && nDataLen > 0)
		{
			if ( NULL != DEFAULT_SVR() && nDataLen<32 )
			{
                int len=strnlen((char*)pData,nDataLen);
                string InstrumentID((char*)pData,len);
                vector<pair<string,string>> rltTS;

                rltTS=DEFAULT_SVR()->GetTradingTimespan2(InstrumentID);

                targetMsg.msg.dwData = MAKELONG(CMDID_TRADINGTIMESPAN_RSP, SUCCESS);
				targetMsg.msg.cbData = 32+rltTS.size()*8;

                char* pBuffer = new char[targetMsg.msg.cbData];
                memset(pBuffer, 0, targetMsg.msg.cbData);
                memcpy(pBuffer , InstrumentID.c_str(), InstrumentID.length()<31?InstrumentID.length():31);
                for(int i=0;i<(int)rltTS.size();i++)
                {
                    memcpy(pBuffer + 32 + i*8, rltTS[i].first.c_str(), rltTS[i].first.length()<=4?rltTS[i].first.length():4);
                    memcpy(pBuffer + 32 + i*8 +4, rltTS[i].second.c_str(), rltTS[i].second.length()<=4?rltTS[i].second.length():4);
                }
                targetMsg.msg.lpData = pBuffer;
                bRet = PushIntoMsgQueue(targetMsg, nRequestId);
                delete[] pBuffer;
			}
			else
			{
				targetMsg.msg.dwData = MAKELONG(CMDID_TRADINGTIMESPAN_RSP, UNKOWN_ERROR);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if (   nCMDID == CMDID_TRADINGDAY_QUERY )
	{
        //查询交易日，参数为空
        //返回交易日，成功时lpData为TradingDay(32字节)，如20141020; cbData=32
		if ( NULL != DEFAULT_SVR() )
		{
            string strTradingDay=DEFAULT_SVR()->GetTradingDay();

            targetMsg.msg.dwData = MAKELONG(CMDID_TRADINGDAY_RSP, SUCCESS);
			targetMsg.msg.cbData = 32;

            char* pBuffer = new char[targetMsg.msg.cbData];
            memset(pBuffer, 0, targetMsg.msg.cbData);
            memcpy(pBuffer , strTradingDay.c_str(), strTradingDay.length()<31?strTradingDay.length():31);
            targetMsg.msg.lpData = pBuffer;
            bRet = PushIntoMsgQueue(targetMsg, nRequestId);
            delete[] pBuffer;
		}
		else
		{
			targetMsg.msg.dwData = MAKELONG(CMDID_TRADINGDAY_RSP, UNKOWN_ERROR);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
	}
	else if (   nCMDID == CMDID_TRADEINFOBRIEF_LIST_QUERY )
	{
        //查询指定合约的成交信息(简要)列表。参数为InstrumentID，可以为空，表示查询全部的成交
        //返回指定合约的成交信息(简要)列表(CMDID_TRADEINFOBRIEF_LIST_RSP)。成功时lpData指向要查询的合约ID+结果PlatformStru_TradeInfoBrief数组，cbData等于32+sizeof(PlatformStru_TradeInfoBrief)*成交记录数量

        int RspCMDID=CMDID_TRADEINFOBRIEF_LIST_RSP;

		if ( NULL != DEFAULT_SVR() && nDataLen<32 )
		{
            string inID;
            if(NULL != pData && nDataLen > 0) inID=string((char*)pData,strnlen((char*)pData,nDataLen));
            vector<PlatformStru_TradeInfoBrief> rlts;

            DEFAULT_SVR()->GetTradeBriefsOfInstrument(inID,rlts);

            targetMsg.msg.dwData = MAKELONG(RspCMDID, SUCCESS);
			targetMsg.msg.cbData = 32+rlts.size()*sizeof(PlatformStru_TradeInfoBrief);

            char* pBuffer = new char[targetMsg.msg.cbData];
            memset(pBuffer, 0, targetMsg.msg.cbData);
            memcpy(pBuffer , inID.c_str(), inID.length()<31?inID.length():31);
            for(int i=0;i<(int)rlts.size();i++)
                memcpy(pBuffer + 32 + i * sizeof(PlatformStru_TradeInfoBrief), &(rlts[i]), sizeof(PlatformStru_TradeInfoBrief));
            targetMsg.msg.lpData = pBuffer;
            bRet = PushIntoMsgQueue(targetMsg, nRequestId);
            delete[] pBuffer;
		}
		else
		{
			targetMsg.msg.dwData = MAKELONG(RspCMDID, UNKOWN_ERROR);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
		}
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
				DEFAULT_SVR()->GetWaitOrders3(key.szInstrumentID, vec);
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
	else if ( nCMDID == CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_QUERY )
	{
		if ( NULL != pData && nDataLen == sizeof(PlusinAcctInstrumentKey))
		{
			PlusinAcctInstrumentKey key = *(PlusinAcctInstrumentKey*)pData;
			std::vector<PlatformStru_OrderInfo> vec;
			if ( NULL != DEFAULT_SVR() )
			{
				DEFAULT_SVR()->GetWaitOrders3(key.szInstrumentID, vec);
				int nRecordNum = 0;
				for ( UINT i = 0; i < vec.size(); i++)
				{
					if ( (THOST_FTDC_OST_PartTradedQueueing == vec[i].OrderStatus ||
						THOST_FTDC_OST_NoTradeQueueing == vec[i].OrderStatus) &&
						(vec[i].CombOffsetFlag[0] == THOST_FTDC_OF_Open ))
					{
						nRecordNum += vec[i].VolumeTotal;
					}
				}

				PlusinAcctOpenOrderNumRsp rsp;
				rsp.queryCondition = key;
				rsp.nNum = nRecordNum;
				targetMsg.msg.cbData = sizeof(PlusinAcctOpenOrderNumRsp);
				targetMsg.msg.lpData = &rsp;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP, wErrorCode);
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
				DEFAULT_SVR()->GetWaitOrders3(key.szInstrumentID, vec);
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
		if ( NULL != pData && nDataLen == sizeof(PositionKey))
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
	else if ( nCMDID == CMDID_ACCT_POSITION_INFO_LIST_QUERY)
	{
		if ( NULL != pData && nDataLen == sizeof(CommonAccountType))
		{
			CommonAccountType key;
			memcpy(key, pData, sizeof(CommonAccountType));
			IPlatformSingleSvr* pSvr = DEFAULT_SVR();
			if ( IS_MULTIACCOUNT_VERSION)
			{
				pSvr = g_pPlatformMgr->GetPlatformSvr(key);
			}

			if ( NULL != pSvr )
			{
				std::vector<PlatformStru_Position> vec;
				pSvr->GetPositions(vec);
				if ( !vec.empty())
				{
					targetMsg.msg.cbData = sizeof(PlatformStru_Position)*vec.size();
					targetMsg.msg.lpData = &vec[0];
				}

				targetMsg.msg.dwData = MAKELONG(CMDID_ACCT_POSITION_INFO_LIST_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ACCT_POSITION_INFO_LIST_RSP, wErrorCode);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
		}
	}
	else if ( nCMDID == CMDID_ACCT_POSITION_DETAIL_LIST_QUERY)
	{
		if ( NULL != pData && nDataLen == sizeof(CommonAccountType))
		{
			CommonAccountType key;
			memcpy(key, pData, sizeof(CommonAccountType));
			IPlatformSingleSvr* pSvr = DEFAULT_SVR();
			if ( IS_MULTIACCOUNT_VERSION)
			{
				pSvr = g_pPlatformMgr->GetPlatformSvr(key);
			}

			if ( NULL != pSvr )
			{
				std::vector<PlatformStru_PositionDetail> vec;
				long lLastSeq = 0;
				pSvr->GetPositionDetails(vec, lLastSeq);
				if ( !vec.empty())
				{
					targetMsg.msg.cbData = sizeof(PlatformStru_PositionDetail)*vec.size();
					targetMsg.msg.lpData = &vec[0];
				}

				targetMsg.msg.dwData = MAKELONG(CMDID_ACCT_POSITION_DETAIL_LIST_RSP, SUCCESS);
				bRet = PushIntoMsgQueue(targetMsg, nRequestId);
			}
			else
			{
				wErrorCode = UNKOWN_ERROR;
				targetMsg.msg.dwData = MAKELONG(CMDID_ACCT_POSITION_DETAIL_LIST_RSP, wErrorCode);
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
			PlatformStru_Position outData;
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
			strCurrentDay=DEFAULT_SVR()->GetTradingDay();
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
				strCurrentDay=DEFAULT_SVR()->GetTradingDay();
				SERVER_PARAM_CFG paramcfg=DEFAULT_SVR()->GetServerParamCfg();
				if( paramcfg.vec[paramcfg.CurSvrGrpID].CTPSimulate == 1 )
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
                if ( (int)m_quoteSubscribe.size() > GlobalConfigManager::MaxSubscribeCountInPlugins)
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
						DEFAULT_SVR()->SetSubscribeStatus(szInstrumentID,GID_PLUSIN);
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
					DEFAULT_SVR()->SetSubscribeStatus(szInstrumentID,GID_PLUSIN, eUnsubscribeMarketData);
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
	else if ( nCMDID == CMDID_QUOTE_GETCURRENTGROUPMEMBER_QUERY)
	{
        //查询当前合约组的合约列表，不带参数
        CfgMgr* pCfgMgr;
        std::vector<std::string>* pInstrumentList;
        if ( NULL != DEFAULT_SVR() && 
            (pCfgMgr = CFG_MGR_DEFAULT())!=NULL &&
            (pInstrumentList = pCfgMgr->GetCurrentGroupMember())!=NULL)
		{
            int cbData=(int)pInstrumentList->size()*32;
            char *lpData=(char*)malloc(cbData);
            if(lpData)
            {
                memset(lpData,0,cbData);
                for(int i=0;i<(int)pInstrumentList->size();i++)
                    strncpy(lpData+i*32,(*pInstrumentList)[i].c_str(),31);

                targetMsg.msg.cbData = cbData;
			    targetMsg.msg.lpData = lpData;
			    targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_GETCURRENTGROUPMEMBER_RSP, SUCCESS);
			    bRet = PushIntoMsgQueue(targetMsg, nRequestId);
                free(lpData);
            }
		}
		else
		{
			wErrorCode = UNKOWN_ERROR;
			targetMsg.msg.dwData = MAKELONG(CMDID_QUOTE_GETCURRENTGROUPMEMBER_RSP, wErrorCode);
			bRet = PushIntoMsgQueue(targetMsg, nRequestId);
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
			targetMsg.msg.lpData = &paramcfg.vec[paramcfg.CurSvrGrpID].CTPSimulate;
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
			if ( m_bOrderVerify )
			{
				//发给审核插件
				bRet = SendNeedVerifyOrder(inputOrder);
			}
			else
			{
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
	}
	if ( nCMDID == CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY )
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
					targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY_RSP, SUCCESS);
					bRet = PushIntoMsgQueue(targetMsg, nRequestId);
				}
				else
				{
					wErrorCode = nRet;
					targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY_RSP, wErrorCode);
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
				DEFAULT_SVR()->GetWaitOrders3(key.szInstrumentID, vec);
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
				DEFAULT_SVR()->GetWaitOrders3(key.szInstrumentID, vec);
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
				DEFAULT_SVR()->GetWaitOrders3(key.szInstrumentID, vec);
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
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPositionCombDetail, GID_PLUSIN, CallBackFunc_BID_RspQryInvestorComboPositionDetail);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryOrder, GID_PLUSIN, CallBackFunc_BID_RspQryOrder);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingAccount, GID_PLUSIN, CallBackFunc_BID_RspQryTradingAccount);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction, GID_PLUSIN, RspOrderAction1CallBackFunc);
		DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderAction, GID_PLUSIN, RspOrderAction2CallBackFunc);
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_PLUSIN, RspOrderInsert1CallBackFunc);
		DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderInsert, GID_PLUSIN, RspOrderInsert2CallBackFunc);
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
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPositionCombDetail, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryOrder, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingAccount, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderAction, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderInsert, GID_PLUSIN);
		DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderInsert, GID_PLUSIN);
	}
}

int CPlusin::CallBackFunc_BID_RtnOrder( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RtnOrder || data.Size!=sizeof(DataRtnOrder))
		return 0;	

	DataRtnOrder* pOrderData = (DataRtnOrder*)&data;
	
	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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

	DataRtnTrade* pTradeData = (DataRtnTrade*)&data;

	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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

	if ( !((DataRspQryInvestorPosition*)&data)->bIsLast)
	{
		return 0;
	}

	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_POSITION_REQUERY, SUCCESS);
			targetMsg.msg.cbData = 0;
			targetMsg.msg.lpData = NULL;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::CallBackFunc_BID_RspQryInvestorComboPositionDetail( const GUIModuleID GID,const AbstractBusinessData& data )
{
	if(data.BID!=BID_RspQryInvestorPositionCombDetail || data.Size!=sizeof(DataRspQryInvestorPositionCombDetail))
		return 0;

	if ( !((DataRspQryInvestorPositionCombDetail*)&data)->bIsLast)
	{
		return 0;
	}

	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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

	if ( !((DataRspQryOrder*)&data)->bIsLast)
	{
		return 0;
	}

	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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

	if ( !((DataRspQryTradingAccount*)&data)->bIsLast)
	{
		return 0;
	}

	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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
		plusin.unMenuPos = helper.GetMenuPos();
        plusin.bMultiInstance = helper.CanMultiInstance();
        plusin.bNeedCurrentInstrumentID=helper.NeedCurrentInstrumentID();
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
				DWORD ProcessID=OpenPlusInProc( plusin.strPath, string(""));
                if(ProcessID!=0xffffffff)
		            m_setProcessIDWaitForShow.insert(ProcessID);
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

    if ( it->second.m_Instance.size()==0 || it->second.bMultiInstance)
	{
        string Param;
        if(it->second.bNeedCurrentInstrumentID) Param=string("CurrentInstrumentID=")+m_strOrderModuleCurInstrumentID;
        else Param=string("");
		DWORD ProcessID=OpenPlusInProc( it->second.strPath, Param);
        if(ProcessID!=0xffffffff)
		    m_setProcessIDWaitForShow.insert(ProcessID);
	}
	else
	{
		TargetMsg targetMsg;
		memset(&targetMsg, 0, sizeof(targetMsg));
        targetMsg.hWnd = it->second.m_Instance.begin()->first;
		targetMsg.msg.dwData = MAKELONG(CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW, SUCCESS);

		PushIntoMsgQueue(targetMsg, 0);
	}
}

int CPlusin::RspOrderAction1CallBackFunc( const GUIModuleID GID,const AbstractBusinessData &data )
{
	if(data.BID!=BID_RspOrderAction || data.Size!=sizeof(DataRspOrderAction))
		return 0;	

	DataRspOrderAction* pOrderAction1 = (DataRspOrderAction*)&data;
	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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
	if(data.BID!=BID_ErrRtnOrderAction || data.Size!=sizeof(DataErrRtnOrderAction))
		return 0;	

	DataErrRtnOrderAction* pOrderAction2 = (DataErrRtnOrderAction*)&data;
	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
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

int CPlusin::RspOrderInsert1CallBackFunc( const GUIModuleID GID,const AbstractBusinessData &data )
{
	if(data.BID!=BID_RspOrderInsert || data.Size!=sizeof(DataRspOrderInsert))
		return 0;	

	DataRspOrderInsert* pOrderInsert1 = (DataRspOrderInsert*)&data;
	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_RSP_ORDER_INSERT1, SUCCESS);
			PlatformStru_RspOrderInsert1 rspOrderInsert1;
			memcpy(&rspOrderInsert1.InputOrderInsertField, &pOrderInsert1->InputOrderField, sizeof(rspOrderInsert1.InputOrderInsertField));
			memcpy(&rspOrderInsert1.RspInfoField, &pOrderInsert1->RspInfoField, sizeof(rspOrderInsert1.RspInfoField));
			targetMsg.msg.cbData = sizeof(PlatformStru_RspOrderAction1);
			targetMsg.msg.lpData = &rspOrderInsert1;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

int CPlusin::RspOrderInsert2CallBackFunc( const GUIModuleID GID,const AbstractBusinessData &data )
{
	if(data.BID!=BID_ErrRtnOrderInsert || data.Size!=sizeof(DataErrRtnOrderInsert))
		return 0;	

	DataErrRtnOrderInsert* pOrderInsert2 = (DataErrRtnOrderInsert*)&data;
	for ( map<wxString, PLUGWIN2>::iterator it = s_pPlusin->m_mapAppID2Plusin.begin(); it != s_pPlusin->m_mapAppID2Plusin.end(); ++it )
	{
        for(map<HWND,PLUGWIN2::Stru_Instance>::iterator it2=it->second.m_Instance.begin();it2!=it->second.m_Instance.end();it2++)
		{
			TargetMsg targetMsg;
			memset(&targetMsg, 0, sizeof(TargetMsg));
			targetMsg.hWnd = it2->second.PluginMsgHWnd;
			targetMsg.msg.dwData = MAKELONG(CMDID_ORDER_RSP_ORDER_INSERT1, SUCCESS);
			PlatformStru_RspOrderInsert2 rspOrderInsert2;
			memcpy(&rspOrderInsert2.OrderInsertField, &pOrderInsert2->InputOrderField, sizeof(rspOrderInsert2.OrderInsertField));
			memcpy(&rspOrderInsert2.RspInfoField, &pOrderInsert2->RspInfoField, sizeof(rspOrderInsert2.RspInfoField));
			targetMsg.msg.cbData = sizeof(PlatformStru_RspOrderAction1);
			targetMsg.msg.lpData = &rspOrderInsert2;
			s_pPlusin->PushIntoMsgQueue(targetMsg, 0);
		}
	}

	return 0;
}

bool CPlusin::GetOrderVerifyStatus()
{
	return m_bOrderVerify;
}

BOOL CPlusin::ProcessInsertOrderVerifyMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if ( nCMDID == CMDID_INSERT_ORDER_VERIFY_OPEN_QUERY)
	{
		m_bOrderVerify = true;
		m_hMsgWnd_OrderVerify = hWnd;

		wErrorCode = SUCCESS;
		targetMsg.msg.dwData = MAKELONG(CMDID_INSERT_ORDER_VERIFY_OPEN_RSP, wErrorCode);
		targetMsg.msg.lpData = NULL;
		targetMsg.msg.cbData = 0;

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}
	else if ( nCMDID == CMDID_INSERT_ORDER_VERIFY_CLOSE_QUERY)
	{
		m_bOrderVerify = false;
		m_hMsgWnd_OrderVerify = NULL;

		wErrorCode = SUCCESS;
		targetMsg.msg.dwData = MAKELONG(CMDID_INSERT_ORDER_VERIFY_CLOSE_RSP, wErrorCode);
		targetMsg.msg.lpData = NULL;
		targetMsg.msg.cbData = 0;

		bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}

	return bRet;
}

BOOL CPlusin::ProcessOtherMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId )
{
	BOOL bRet = FALSE;
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = hWnd;
	WORD wErrorCode = 0;
	if (   nCMDID == CMDID_UI_ColorScheme_QUERY )
	{
        //查询UI配色方案，参数为空
        //返回交易日，返回UI配色方案，成功时lpData指向一个Int, 0:浅底色方案；1:深底色方案; cbData=4
        string strTradingDay=DEFAULT_SVR()->GetTradingDay();

        int ColorScheme=CRuntimeConfigMgr::UI_ColorScheme;
        targetMsg.msg.dwData = MAKELONG(CMDID_UI_ColorScheme_RSP, SUCCESS);
		targetMsg.msg.cbData = 4;
        targetMsg.msg.lpData = &ColorScheme;
        bRet = PushIntoMsgQueue(targetMsg, nRequestId);
	}

	return bRet;
}


bool CPlusin::SendNeedVerifyOrder( PlatformStru_InputOrder& InputOrder )
{
	TargetMsg targetMsg;
	memset(&targetMsg, 0, sizeof(targetMsg));
	targetMsg.hWnd = m_hMsgWnd_OrderVerify;

	targetMsg.msg.cbData = sizeof(InputOrder);
	targetMsg.msg.lpData = &InputOrder;
	targetMsg.msg.dwData = MAKELONG(CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH, SUCCESS);
	return PushIntoMsgQueue(targetMsg, 0);
}

//打开一个进程，返回ProcessID。失败时返回0xffffffff
//命令行参数为：FastTrader的主窗口句柄 Param
DWORD CPlusin::OpenPlusInProc( const wxString& strPath , const string& Param)
{
	if ( NULL == GETTOPWINDOW())
	{
		return 0xfffffff;
	}

	wxString strCmdLine;
	strCmdLine.Printf( wxT("\"%s\" %d %s"),strPath.c_str(),reinterpret_cast<long>(GETTOPWINDOW()->GetHWND()),Param.c_str());
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi={0};

	if(!CreateProcess(NULL,(LPSTR)strCmdLine.c_str(),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		std::string str = LOADSTRING(GUI_LOAD_FAILED);
		wxString strMsg = wxString::Format( str.c_str(), strPath.c_str());
		wxMessageBox( strMsg,LOADSTRING(OIP_ERROR),wxOK|wxICON_WARNING);
		return 0xffffffff;
	}
	else
	{
		//提示用户插件已启动，等待登录
        return pi.dwProcessId;
	}
}
