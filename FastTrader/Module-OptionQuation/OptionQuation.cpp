// OptionQuation.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "OptionQuation.h"

#include "DlgQuation.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// COptionQuationApp

BEGIN_MESSAGE_MAP(COptionQuationApp, CWinApp)
END_MESSAGE_MAP()


// COptionQuationApp construction
CDlgQuation* g_pDlgQuation = NULL;
HINSTANCE    m_ResHandle = NULL;
COptionQuationApp::COptionQuationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COptionQuationApp object

COptionQuationApp theApp;


// COptionQuationApp initialization

BOOL COptionQuationApp::InitInstance()
{
	CWinApp::InitInstance();
	
	AFX_MODULE_STATE* state = AfxGetModuleState();  
	m_ResHandle = state->m_hCurrentResourceHandle;

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}



